/**
 * @file Game.cc
 * @author Piotr Kluba
 * @brief Base game class responsible for managing the state of the current game
 * @version 0.1
 * @date 2024-04-15
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <Game.h>

#include <algorithm>
#include <variant>

#include "Hero.h"
#include "MapObject.h"

using namespace std;

std::vector<FieldCoords> getOccupiedFields(MapObject& map_object) {
  return std::visit([&](auto& arg) { return arg.occupiedFields(); },
                    map_object);
}

int getId(MapObject& map_object) {
  return std::visit([&](auto& arg) { return arg.getId(); }, map_object);
}

FieldCoords getOrigin(MapObject& map_object) {
  return std::visit([&](auto& arg) { return arg.getOrigin(); }, map_object);
}

Game::Game(std::vector<Player> players, Map map,
           std::vector<MapObject> starting_map_objects)
    : players_in_game_(players), game_map_(map), curr_player_idx(0) {
  for (auto& map_object : starting_map_objects) {
    if (addMapObject(map_object)) {
      break;
    }

    FieldCoords wrong_coord;
    for (FieldCoords coord_to_check : getOccupiedFields(map_object)) {
      if (!game_map_.getField(coord_to_check).value()->isWalkable()) {
        wrong_coord = coord_to_check;
        break;
      }
    }
    throw WrongObjectPlacementException(
        wrong_coord, *game_map_.getField(wrong_coord).value()->getObject(),
        map_object);
  }
}

Player* Game::getPlayer(int idx) {
  if (players_in_game_.size() <= idx) {
    return nullptr;
  }
  return &players_in_game_[idx];
}

bool Game::moveCurrPlayer(FieldCoords coords) {
  if (not game_map_.getField(coords).value()->isWalkable()) {
    return false;
  }
  this->getCurrentPlayer()->getCurrentHero()->moveTo(coords);
  return true;
}

bool Game::deleteMapObject(int id) {
  int idx_to_delete = 0;
  auto it = find_if(map_objects_.begin(), map_objects_.end(),
                    [&](auto& map_obj) { return getId(map_obj) == id; });
  if (it != map_objects_.end()) {
    idx_to_delete = distance(map_objects_.begin(), it);
  } else {
    return false;
  }

  std::vector<FieldCoords> fields_to_empty =
      getOccupiedFields(map_objects_[idx_to_delete]);
  for (FieldCoords coords : fields_to_empty) {
    game_map_.deleteObjectFrom(coords);
  }
  map_objects_.erase(map_objects_.begin() + idx_to_delete);
  return true;
}

bool Game::addMapObject(MapObject obj_to_add) {
  FieldCoords origin = getOrigin(obj_to_add);
  for (FieldCoords coord_to_check : getOccupiedFields(obj_to_add)) {
    if (!game_map_.getField(coord_to_check).value()->isWalkable()) {
      return false;
    }
  }
  map_objects_.push_back(obj_to_add);

  for (FieldCoords coord_to_set : getOccupiedFields(obj_to_add)) {
    if (!game_map_.setObjectTo(coord_to_set, obj_to_add)) {
      return false;
    }
  }
  return true;
}

void Game::executeAction(FieldCoords coords) {
  auto ret = game_map_.getField(coords);
  if (!ret.has_value()) {
    return;
  }
  auto field = ret.value();

  if (field->isWalkable()) {
    // TODO
    return;
  }

  auto object_variant = *field->getObject();
  std::visit([&](auto& object) { object.objectAction(this); }, object_variant);
}
