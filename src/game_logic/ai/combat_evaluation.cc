#include "combat_evaluation.h"

#include "combat_common.h"

namespace combat::ai {

float simpleEvaluate(const BattleState& state,
                     HeroRole maximizing_player_role) {
  if (state.winner_.has_value()) {
    return state.winner_.value() == maximizing_player_role
               ? std::numeric_limits<float>::infinity()
               : -std::numeric_limits<float>::infinity();
  }
  int max_player_units_hp = 0;
  int min_player_units_hp = 0;
  auto aggregate_hp_max_player = [&](const UnitBlock& unit) {
    max_player_units_hp += unit.current_total_hitpoints;
  };
  auto aggregate_hp_min_player = [&](const UnitBlock& unit) {
    min_player_units_hp += unit.current_total_hitpoints;
  };
  auto max_player_units =
      state.hero_units_.at_underlying(maximizing_player_role);
  std::for_each(max_player_units.begin(), max_player_units.end(),
                aggregate_hp_max_player);
  auto min_player_role = maximizing_player_role == HeroRole::ATTACKER
                             ? HeroRole::DEFENDER
                             : HeroRole::ATTACKER;
  auto min_player_units = state.hero_units_.at_underlying(min_player_role);
  std::for_each(min_player_units.begin(), min_player_units.end(),
                aggregate_hp_min_player);
  int move_bonus = state.currently_moving_ == maximizing_player_role ? 30 : -30;

  return max_player_units_hp - min_player_units_hp + move_bonus;
}

float complexEvaluate(const BattleState& state,
                      HeroRole maximizing_player_role) {
  if (state.winner_.has_value()) {
    return state.winner_.value() == maximizing_player_role
               ? std::numeric_limits<float>::infinity()
               : -std::numeric_limits<float>::infinity();
  }

  // gathering information
  bool is_max_player_moving = state.currently_moving_ == maximizing_player_role;
  int max_player_units_hp = 0;
  int min_player_units_hp = 0;
  int max_player_damage_potential = 0;
  int min_player_damage_potential = 0;

  auto aggregate_stats_max_player = [&](const UnitBlock& unit) {
    max_player_units_hp += unit.current_total_hitpoints;
    max_player_damage_potential += unit.min_damage * unit.unit_count;
  };
  auto aggregate_stats_min_player = [&](const UnitBlock& unit) {
    min_player_units_hp += unit.current_total_hitpoints;
    min_player_damage_potential += unit.min_damage * unit.unit_count;
  };
  auto max_player_units =
      state.hero_units_.at_underlying(maximizing_player_role);
  std::for_each(max_player_units.begin(), max_player_units.end(),
                aggregate_stats_max_player);
  auto min_player_role = maximizing_player_role == HeroRole::ATTACKER
                             ? HeroRole::DEFENDER
                             : HeroRole::ATTACKER;
  auto min_player_units = state.hero_units_.at_underlying(min_player_role);
  std::for_each(min_player_units.begin(), min_player_units.end(),
                aggregate_stats_min_player);

  bool is_next_unit_max_players =
      state.unit_move_queue_.front().first == maximizing_player_role;

  auto hp_diff = max_player_units_hp - min_player_units_hp;
  auto damage_potential_diff =
      max_player_damage_potential - min_player_damage_potential;
  bool has_damage_potential_advantage = damage_potential_diff > 0;

  // calculating score
  float score = 0;
  if (is_max_player_moving and is_next_unit_max_players) {
    score += has_damage_potential_advantage ? damage_potential_diff * 3
                                            : damage_potential_diff * 0.5;
  } else if (is_max_player_moving) {
    score += has_damage_potential_advantage ? damage_potential_diff * 2
                                            : damage_potential_diff * 0.75;
  } else if (is_next_unit_max_players) {
    score += damage_potential_diff;
  } else {
    
    score += has_damage_potential_advantage ? damage_potential_diff * 0.5
                                            : damage_potential_diff * 3;
  }
  score += hp_diff * 2;
  return score;
}

}  // namespace combat::ai