/**
 * @file test_battle_manager.cc
 * @author Wojciech Przybylski
 * @brief Unit tests for BattleManager class
 * @copyright Copyright (c) 2024
 */

#include <BattleManager.h>
#include <combat_utils.h>
#include <gtest/gtest.h>

#include "Hero.h"

using namespace combat;

const UnitBlock UNIT_BLOCK_PROTOTYPE =
    UnitBlock{.type = UnitOrigin::NEUTRAL,
              .level = 1,
              .name = "test",
              .unit_count = 5,
              .hitpoints_per_unit = 5,
              .current_total_hitpoints = 25,
              .speed = 4,
              .min_damage = 1,
              .max_damage = 1,
              .attack = 0,
              .defense = 0,
              .can_retaliate = true,
              .is_ranged = false,
              .immune_to_retaliation = false};

TEST(battle_manager_test, initial_state) {
  Hero attacking_hero("test1", {0, 0});
  Hero defending_hero("test2", {0, 1});
  attacking_hero.addUnit(UNIT_BLOCK_PROTOTYPE);
  auto defending_hero_unit = UNIT_BLOCK_PROTOTYPE;
  defending_hero_unit.speed = 6;
  defending_hero.addUnit(defending_hero_unit);
  BattleManager battle_manager(attacking_hero, defending_hero);

  EXPECT_EQ(battle_manager.getState().currently_moving_, HeroRole::DEFENDER);
  EXPECT_EQ(battle_manager.getState().current_unit_id_,
            std::make_pair(HeroRole::DEFENDER, 0U));
  EXPECT_EQ(battle_manager.getState().unit_move_queue_.size(), 1U);
  EXPECT_EQ(battle_manager.getState().unit_move_queue_.front(),
            std::make_pair(HeroRole::ATTACKER, 0U));
  EXPECT_EQ(battle_manager.getState().round_phase_,
            BattleState::RoundPhase::MOVING);
  EXPECT_TRUE(
      battle_manager.getState().battleground_.isFieldOccupied({0, 0, 0}));
  EXPECT_TRUE(
      battle_manager.getState().battleground_.isFieldOccupied({0, 0, 14}));
}