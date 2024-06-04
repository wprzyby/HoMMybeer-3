#include "combat_common.h"

namespace combat {

std::vector<UnitIdentifier> getAllUnitIds(const UnitContainer& units) {
  std::vector<UnitIdentifier> unit_ids{};
  auto add_hero_unit_ids = [&](HeroRole role, unsigned int unit_count) {
    for (unsigned int i : std::views::iota(0U, unit_count)) {
      unit_ids.emplace_back(role, i);
    }
  };
  add_hero_unit_ids(HeroRole::ATTACKER, units.size().first);
  add_hero_unit_ids(HeroRole::DEFENDER, units.size().second);
  return unit_ids;
}

}  // namespace combat