/**
 * @file combat_utils.cc
 * @author Wojciech Przybylski
 * @brief Definitions shared by multiple files within the combat library
 * @copyright Copyright (c) 2024
 */

#include "combat_utils.h"

namespace combat {

std::vector<UnitIdentifier> getAllUnitIds(const UnitContainer& units) {
  std::vector<UnitIdentifier> unit_ids{};
  auto add_hero_unit_ids = [&](HeroRole role, unsigned int unit_count) {
    for (unsigned int unit_idx : std::views::iota(0U, unit_count)) {
      unit_ids.emplace_back(role, unit_idx);
    }
  };
  add_hero_unit_ids(HeroRole::ATTACKER, units.size().first);
  add_hero_unit_ids(HeroRole::DEFENDER, units.size().second);
  return unit_ids;
}

}  // namespace combat