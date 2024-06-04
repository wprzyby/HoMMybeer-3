/**
 * @file Battleground.h
 * @author Wojciech Przybylski
 * @brief Class for unit placement, movement and removal within the context of
 * the battlefield
 * @copyright Copyright (c) 2024
 */
#ifndef SRC_GAME_LOGIC_COMBAT_BATTLEGROUND_H
#define SRC_GAME_LOGIC_COMBAT_BATTLEGROUND_H

#include <algorithm>
#include <optional>
#include <set>
#include <utility>
#include <vector>

#include "combat_common.h"

/**
 * using the Hexagonal Efficient Coordinate System coordinate system, described
 * here:
 * https://en.wikipedia.org/wiki/Hexagonal_Efficient_Coordinate_System#/media/File:Hex2RecASA.jpg
 * it is consistently followed throughout this class and other elements using
 * the Battleground class
 */

namespace combat {

struct BattlegroundSize {
  unsigned int odd_rows_count;
  unsigned int odd_rows_size;
  unsigned int even_rows_count;
  unsigned int even_rows_size;
};

class Battleground {
  BattlegroundSize size_;
  std::map<UnitIdentifier, HexFieldCoords> unit_ids_and_coords_;
  bool removeUnitByFindCallable(auto callable) {
    auto find_result = std::find_if(unit_ids_and_coords_.begin(),
                                    unit_ids_and_coords_.end(), callable);
    if (find_result == unit_ids_and_coords_.end()) {
      return false;
    }
    unit_ids_and_coords_.erase(find_result);
    return true;
  }

 public:
  constexpr static BattlegroundSize DEFAULT_SIZE{.odd_rows_count = 5,
                                                 .odd_rows_size = 15,
                                                 .even_rows_count = 5,
                                                 .even_rows_size = 15};
  explicit Battleground(BattlegroundSize size = DEFAULT_SIZE) : size_(size) {}

  // INFO ABOUT BATTLEFIELD
  [[nodiscard]] BattlegroundSize getSize() const { return size_; }
  [[nodiscard]] bool isFieldInBattleground(HexFieldCoords coords) const;
  [[nodiscard]] bool isFieldOccupied(HexFieldCoords coords) const;
  [[nodiscard]] std::optional<UnitIdentifier> getFieldOccupant(
      HexFieldCoords coords) const;
  [[nodiscard]] std::vector<UnitIdentifier> getAliveUnitsIds() const;
  [[nodiscard]] std::set<HexFieldCoords> getAllHeroUnitCoords(
      HeroRole role) const;
  [[nodiscard]] std::set<HexFieldCoords> getAdjacentFields(
      HexFieldCoords coords) const;
  [[nodiscard]] std::set<HexFieldCoords> getAdjacentEnemyUnitsCoords(
      UnitIdentifier unit_id) const;
  [[nodiscard]] std::optional<HexFieldCoords> getUnitCoords(
      UnitIdentifier unit_id) const;
  [[nodiscard]] std::set<HexFieldCoords> getFieldsWithinRange(
      HexFieldCoords origin, unsigned int range) const;
  [[nodiscard]] std::set<HexFieldCoords> getFieldsWithinRange(
      UnitIdentifier unit_id, unsigned int range) const;

  // PLACEMENT, MOVEMENT, REMOVAL
  bool placeUnit(HexFieldCoords coords, UnitIdentifier unit_id);
  bool removeUnitByIdentifier(UnitIdentifier unit_id);
  bool removeUnitByCoords(HexFieldCoords coords);
  bool moveUnitByIdentifier(UnitIdentifier unit_id, HexFieldCoords destination);
  bool moveUnitByCoords(HexFieldCoords current_coords,
                        HexFieldCoords destination);
};

}  // namespace combat

#endif  // SRC_GAME_LOGIC_COMBAT_BATTLEGROUND_H