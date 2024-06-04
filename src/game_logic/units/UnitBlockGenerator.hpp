/**
 * @file UnitBlockGenerator.hpp
 * @author Wojciech Przybylski
 * @brief Class responsible for generating unit blocks according to
 * configuration provided by Config object
 * @copyright Copyright (c) 2024
 */

#ifndef SRC_GAME_LOGIC_UNITS_UNIT_BLOCK_GENERATOR_HPP
#define SRC_GAME_LOGIC_UNITS_UNIT_BLOCK_GENERATOR_HPP

#include "UnitBlock.hpp"

template <typename Config>
class UnitBlockGenerator {
  constexpr static bool DEFAULT_CAN_RETALIATE_INIT{true};

 public:
  explicit UnitBlockGenerator(Config units_config)
      : units_config_(units_config) {}

  std::optional<UnitBlock> getUnitBlock(UnitOrigin type, unsigned int level,
                                        unsigned int unit_count) {
    auto attributes_ret = units_config_.getAttributes(type, level);
    if (!attributes_ret.has_value()) {
      return {};
    }
    auto attributes = attributes_ret.value();
    return UnitBlock{
        .type = type,
        .level = level,
        .name = attributes.name,
        .unit_count = unit_count,
        .hitpoints_per_unit = attributes.hitpoints_per_unit,
        .current_total_hitpoints = attributes.hitpoints_per_unit * unit_count,
        .speed = attributes.speed,
        .min_damage = attributes.min_damage,
        .max_damage = attributes.max_damage,
        .attack = attributes.attack,
        .defense = attributes.defense,
        .can_retaliate = DEFAULT_CAN_RETALIATE_INIT,
        .is_ranged = attributes.is_ranged,
        .immune_to_retaliation = attributes.immune_to_retaliation};
  }

 private:
  Config units_config_;
};

#endif  // SRC_GAME_LOGIC_UNITS_UNIT_BLOCK_GENERATOR_HPP
