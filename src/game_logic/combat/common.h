/**
 * @file common.h
 * @author Wojciech Przybylski
 * @brief Definitions shared by multiple files within the combat library
 * @copyright Copyright (c) 2024
 */

#ifndef SRC_GAME_LOGIC_COMBAT_COMMON_H
#define SRC_GAME_LOGIC_COMBAT_COMMON_H

#include <Hero.h>

#include <deque>
#include <map>
#include <utility>
#include <vector>

namespace combat {

enum class HeroRole { ATTACKER, DEFENDER };
using UnitIdentifier = std::pair<HeroRole, unsigned int>;
using UnitQueue = std::deque<UnitIdentifier>;
using HexFieldCoords = std::tuple<int, int, int>;

class UnitContainer {
  std::map<HeroRole, std::vector<UnitBlock>> data_;

 public:
  explicit UnitContainer(std::map<HeroRole, std::vector<UnitBlock>> data)
      : data_(std::move(data)) {}
  UnitContainer() = default;
  UnitBlock& operator[](const UnitIdentifier& unit_id) {
    return data_[unit_id.first][unit_id.second];
  }
  [[nodiscard]] const UnitBlock& at(const UnitIdentifier& unit_id) const {
    return data_.at(unit_id.first)[unit_id.second];
  }
  [[nodiscard]] std::pair<unsigned int, unsigned int> size() const {
    return {data_.at(HeroRole::ATTACKER).size(),
            data_.at(HeroRole::DEFENDER).size()};
  }
  void insert(HeroRole role, const UnitBlock& unit) {
    if (data_.find(role) == data_.end()) {
      data_.insert({role, {}});
    }
    data_.at(role).push_back(unit);
  }
};

std::vector<UnitIdentifier> getAllUnitIds(const UnitContainer& units);

}  // namespace combat

#endif  // SRC_GAME_LOGIC_COMBAT_COMMON_H