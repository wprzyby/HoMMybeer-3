/**
 * @file Battleground.cc
 * @author Wojciech Przybylski
 * @brief Class for unit placement, movement and removal within the context of
 * the battlefield
 * @copyright Copyright (c) 2024
 */

#include "Battleground.h"

#include <algorithm>
#include <queue>
#include <set>

#include "combat_utils.h"

namespace combat {

bool Battleground::isFieldOccupied(HexFieldCoords coords) const {
  return getFieldOccupant(coords).has_value();
}

std::optional<UnitIdentifier> Battleground::getFieldOccupant(
    HexFieldCoords coords) const {
  auto do_coords_match = [&](std::pair<UnitIdentifier, HexFieldCoords> arg) {
    return arg.second == coords;
  };
  auto find_result = std::find_if(unit_ids_and_coords_.begin(),
                                  unit_ids_and_coords_.end(), do_coords_match);
  if (find_result == unit_ids_and_coords_.end()) {
    return {};
  }
  return find_result->first;
}

std::vector<UnitIdentifier> Battleground::getAliveUnitsIds() const {
  std::vector<UnitIdentifier> ret_val(unit_ids_and_coords_.size());
  auto get_unit = [](std::pair<UnitIdentifier, HexFieldCoords> arg) {
    return arg.first;
  };
  std::transform(unit_ids_and_coords_.begin(), unit_ids_and_coords_.end(),
                 ret_val.begin(), get_unit);
  return ret_val;
}

std::set<HexFieldCoords> Battleground::getAllHeroUnitCoords(
    HeroRole role) const {
  std::set<HexFieldCoords> ret_val{};
  for (const auto& [identifier, coords] : unit_ids_and_coords_) {
    if (identifier.first == role) {
      ret_val.insert(coords);
    }
  }
  return ret_val;
}

bool Battleground::isFieldInBattleground(HexFieldCoords coords) const {
  if (std::get<0>(coords) != 0 and std::get<0>(coords) != 1) {
    return false;
  }
  if (std::get<0>(coords) == 1) {
    return std::get<1>(coords) < size_.odd_rows_count and
           std::get<2>(coords) < size_.odd_rows_size;
  }
  return std::get<1>(coords) < size_.even_rows_count and
         std::get<2>(coords) < size_.even_rows_size;
}

std::set<HexFieldCoords> Battleground::getAdjacentFields(
    HexFieldCoords coords) const {
  // as per
  // https://en.wikipedia.org/wiki/Hexagonal_Efficient_Coordinate_System#/media/File:HECS_Nearest_Neighbors.png
  // (coord names matched with this example)

  auto get_coords_to_check = [](HexFieldCoords coords) {
    auto a = std::get<0>(coords);
    auto r = std::get<1>(coords);
    auto c = std::get<2>(coords);
    return std::set<HexFieldCoords>{
        {1 - a, r - (1 - a), c - (1 - a)},  //
        {1 - a, r - (1 - a), c + a},        //
        {a, r, c - 1},                      //
        {a, r, c + 1},                      //
        {1 - a, r + a, c - (1 - a)},        //
        {1 - a, r + a, c + a},              //
    };
  };
  std::set<HexFieldCoords> ret_val{};
  for (const auto coords :  // NOLINT(readability-identifier-naming)
       get_coords_to_check(coords)) {
    if (isFieldInBattleground(coords)) {
      ret_val.insert(coords);
    }
  }
  return ret_val;
}

std::set<HexFieldCoords> Battleground::getAdjacentEnemyUnitsCoords(
    UnitIdentifier unit_id) const {
  auto coords = getUnitCoords(unit_id);
  if (not coords.has_value()) {
    return {};
  }
  auto adjacent_fields = getAdjacentFields(coords.value());
  std::set<HexFieldCoords> fields_with_opponents{};
  for (auto field : adjacent_fields) {
    auto field_occupant = getFieldOccupant(field);
    if (field_occupant.has_value() and
        field_occupant.value().first != unit_id.first) {
      fields_with_opponents.insert(field);
    }
  }
  return fields_with_opponents;
}

std::optional<HexFieldCoords> Battleground::getUnitCoords(
    UnitIdentifier unit_id) const {
  if (not unit_ids_and_coords_.contains(unit_id)) {
    return {};
  }
  return unit_ids_and_coords_.at(unit_id);
}

bool Battleground::placeUnit(HexFieldCoords coords, UnitIdentifier unit_id) {
  if (not isFieldInBattleground(coords) or isFieldOccupied(coords)) {
    return false;
  }
  unit_ids_and_coords_.insert({unit_id, coords});
  return true;
}

bool Battleground::removeUnitByIdentifier(UnitIdentifier unit_id) {
  auto unit_iterator = unit_ids_and_coords_.find(unit_id);
  if (unit_iterator == unit_ids_and_coords_.end()) {
    return false;
  }
  unit_ids_and_coords_.erase(unit_iterator);
  return true;
}

bool Battleground::removeUnitByCoords(HexFieldCoords coords) {
  auto are_coords_same = [&](std::pair<UnitIdentifier, HexFieldCoords> arg) {
    return arg.second == coords;
  };
  auto find_result = std::find_if(unit_ids_and_coords_.begin(),
                                  unit_ids_and_coords_.end(), are_coords_same);
  if (find_result == unit_ids_and_coords_.end()) {
    return false;
  }
  unit_ids_and_coords_.erase(find_result);
  return true;
}

bool Battleground::moveUnitByIdentifier(UnitIdentifier unit_id,
                                        HexFieldCoords destination) {
  if (not isFieldInBattleground(destination) or isFieldOccupied(destination)) {
    return false;
  }
  if (not removeUnitByIdentifier(unit_id)) {
    return false;
  }
  placeUnit(destination, unit_id);  // placeUnit return can be ignored because
                                    // the first condition makes sure that it's
                                    // possible to place unit there
  return true;
}

bool Battleground::moveUnitByCoords(HexFieldCoords current_coords,
                                    HexFieldCoords destination) {
  bool current_coords_ok =
      isFieldInBattleground(current_coords) and isFieldOccupied(current_coords);
  bool destination_coords_ok =
      isFieldInBattleground(destination) and not isFieldOccupied(destination);
  if (not current_coords_ok or not destination_coords_ok) {
    return false;
  }
  auto unit_id = getFieldOccupant(current_coords)
                     .value();  // .value() can be called since the coordinates
                                // correctness was checked beforehand
  if (not removeUnitByCoords(current_coords)) {
    return false;
  }
  placeUnit(destination, unit_id);
  return true;
}

std::set<HexFieldCoords> Battleground::getFieldsWithinRange(
    HexFieldCoords origin, unsigned int range) const {
  using FieldNode = std::pair<HexFieldCoords, unsigned int>;
  if (not isFieldInBattleground(origin)) {
    return {};
  }

  // breadth-first search for nodes with distance <= range
  std::set<HexFieldCoords> visited{};
  std::queue<FieldNode> to_visit{};
  to_visit.emplace(origin, 0U);
  while (not to_visit.empty()) {
    auto node = to_visit.front();
    to_visit.pop();
    if (node.second == range) {
      continue;
    }
    for (auto adjacent : getAdjacentFields(node.first)) {
      if (not visited.contains(adjacent) and not isFieldOccupied(adjacent)) {
        visited.insert(adjacent);
        to_visit.emplace(adjacent, node.second + 1U);
      }
    }
  }
  auto origin_iterator = visited.find(origin);
  if (origin_iterator != visited.end()) {
    visited.erase(visited.find(origin));
  }
  return visited;
}

std::set<HexFieldCoords> Battleground::getFieldsWithinRange(
    UnitIdentifier unit_id, unsigned int range) const {
  auto res = getUnitCoords(unit_id);
  if (not res.has_value()) {
    return {};
  }
  return getFieldsWithinRange(res.value(), range);
}

}  // namespace combat