/**
 * @file UnitBlock.hpp
 * @author Wojciech Przybylski
 * @brief Unit block simple structure and related basic unit combat
 * functionality
 * @copyright Copyright (c) 2024
 */

#ifndef SRC_GAME_LOGIC_UNITS_UNIT_BLOCK_HPP
#define SRC_GAME_LOGIC_UNITS_UNIT_BLOCK_HPP

#include <cmath>
#include <concepts>
#include <ranges>
#include <string>

enum class UnitOrigin {
  CASTLE,
  FORTRESS,
  INFERNO,
  NEUTRAL,
};

template <typename T>
concept RandomGenerator = requires(T rng, int lower_bound, int upper_bound) {
  {
    rng.getRandIntInclusive(lower_bound, upper_bound)
  } -> std::convertible_to<int>;
};

struct UnitBlock {
  UnitOrigin type;
  unsigned int level;
  std::string name;
  unsigned int unit_count;
  unsigned int hitpoints_per_unit;
  unsigned int current_total_hitpoints;
  unsigned int speed;
  unsigned int min_damage;
  unsigned int max_damage;
  unsigned int attack;
  unsigned int defense;
  bool can_retaliate;
  bool is_ranged;
  bool immune_to_retaliation;
};

/**
 * @brief Performs an attack procedure with attacker and defender, including
 * retaliation by defender if possible
 *
 * @tparam RNG type of random number generator to use in
 * randomized damage selection
 * @param attacker
 * @param defender
 */
template <RandomGenerator RNG>
void performAttack(UnitBlock& attacker, UnitBlock& defender) {
  strike<RNG>(attacker, defender);
  bool is_retaliation_possible = defender.can_retaliate and
                                 not attacker.is_ranged and
                                 not attacker.immune_to_retaliation;
  if (defender.unit_count > 0 and is_retaliation_possible) {
    strike<RNG>(defender, attacker);
    defender.can_retaliate = false;
  }
}

/**
 * @brief Performs single strike on defender with attacker's stats
 *
 * @tparam type of random number generator to use in
 * randomized damage selection
 * @param attacker
 * @param defender
 */
template <RandomGenerator RNG>
void strike(UnitBlock& attacker, UnitBlock& defender) {
  constexpr static double ATTACK_DAMAGE_BONUS_MULTIPLIER = 0.05;
  constexpr static double DEFENSE_DAMAGE_BONUS_MULTIPLIER = 0.025;
  static RNG rng;

  unsigned int base_damage{0};
  for ([[maybe_unused]] unsigned int i :
       std::views::iota(0U, attacker.unit_count)) {
    base_damage +=
        rng.getRandIntInclusive(attacker.min_damage, attacker.max_damage);
  }
  int attack_defense_diff = attacker.attack - defender.defense;
  auto damage_multiplier = attack_defense_diff > 0
                               ? ATTACK_DAMAGE_BONUS_MULTIPLIER
                               : DEFENSE_DAMAGE_BONUS_MULTIPLIER;
  auto damage = static_cast<unsigned int>(
      std::round(base_damage * (1 + damage_multiplier * attack_defense_diff)));

  defender.current_total_hitpoints =
      std::max(static_cast<int>(defender.current_total_hitpoints - damage), 0);
  if (defender.current_total_hitpoints == 0) {
    defender.unit_count = 0;
    return;
  }
  // weird arithmetic but it ensures that if total hit points are a multiple
  // of hitpoints per unit, the unit count will be calculated correctly
  defender.unit_count =
      (defender.current_total_hitpoints - 1) / defender.hitpoints_per_unit + 1;
}

#endif  // SRC_GAME_LOGIC_UNITS_UNIT_BLOCK_HPP