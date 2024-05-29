/**
 * @file test_unit_block.cc
 * @author Wojciech Przybylski
 * @brief
 * @copyright Copyright (c) 2024
 */

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <UnitBlock.hpp>

class MockRngLowest {
 public:
  int getRandIntInclusive(int from, int /*unused*/) { return from; }  // NOLINT
};

class MockRngHighest {
 public:
  int getRandIntInclusive(int /*unused*/, int to) { return to; }  // NOLINT
};

TEST(unit_block_test, strike_simple_case) {
  UnitBlock attacker{.type = UnitOrigin::NEUTRAL,
                     .level = 1,
                     .name = "test",
                     .unit_count = 5,
                     .hitpoints_per_unit = 5,
                     .current_total_hitpoints = 25,
                     .speed = 4,
                     .min_damage = 1,
                     .max_damage = 3,
                     .attack = 0,
                     .defense = 0,
                     .can_retaliate = false,
                     .is_ranged = false,
                     .immune_to_retaliation = false};
  UnitBlock defender = attacker;
  strike<MockRngLowest>(attacker, defender);

  EXPECT_EQ(defender.current_total_hitpoints, 20);
  EXPECT_EQ(defender.unit_count, 4);
}

TEST(unit_block_test, strike_check_rng_working) {
  UnitBlock attacker{.type = UnitOrigin::NEUTRAL,
                     .level = 1,
                     .name = "test",
                     .unit_count = 5,
                     .hitpoints_per_unit = 5,
                     .current_total_hitpoints = 25,
                     .speed = 4,
                     .min_damage = 1,
                     .max_damage = 3,
                     .attack = 0,
                     .defense = 0,
                     .can_retaliate = false,
                     .is_ranged = false,
                     .immune_to_retaliation = false};
  UnitBlock defender = attacker;
  strike<MockRngHighest>(attacker, defender);

  EXPECT_EQ(defender.current_total_hitpoints, 10);
  EXPECT_EQ(defender.unit_count, 2);
}

TEST(unit_block_test, strike_attack_bonus) {
  UnitBlock attacker{.type = UnitOrigin::NEUTRAL,
                     .level = 1,
                     .name = "test",
                     .unit_count = 5,
                     .hitpoints_per_unit = 5,
                     .current_total_hitpoints = 25,
                     .speed = 4,
                     .min_damage = 1,
                     .max_damage = 3,
                     .attack = 5,
                     .defense = 0,
                     .can_retaliate = false,
                     .is_ranged = false,
                     .immune_to_retaliation = false};
  UnitBlock defender = attacker;
  strike<MockRngLowest>(attacker, defender);

  EXPECT_EQ(defender.current_total_hitpoints, 19);
  EXPECT_EQ(defender.unit_count, 4);
}

TEST(unit_block_test, strike_defense_bonus) {
  UnitBlock attacker{.type = UnitOrigin::NEUTRAL,
                     .level = 1,
                     .name = "test",
                     .unit_count = 5,
                     .hitpoints_per_unit = 5,
                     .current_total_hitpoints = 25,
                     .speed = 4,
                     .min_damage = 1,
                     .max_damage = 3,
                     .attack = 0,
                     .defense = 10,
                     .can_retaliate = false,
                     .is_ranged = false,
                     .immune_to_retaliation = false};
  UnitBlock defender = attacker;
  strike<MockRngLowest>(attacker, defender);

  EXPECT_EQ(defender.current_total_hitpoints, 21);
  EXPECT_EQ(defender.unit_count, 5);
}

TEST(unit_block_test, strike_from_non_max_hp) {
  UnitBlock attacker{.type = UnitOrigin::NEUTRAL,
                     .level = 1,
                     .name = "test",
                     .unit_count = 4,
                     .hitpoints_per_unit = 5,
                     .current_total_hitpoints = 20,
                     .speed = 4,
                     .min_damage = 1,
                     .max_damage = 3,
                     .attack = 0,
                     .defense = 0,
                     .can_retaliate = false,
                     .is_ranged = false,
                     .immune_to_retaliation = false};
  UnitBlock defender = attacker;
  strike<MockRngLowest>(attacker, defender);

  EXPECT_EQ(defender.current_total_hitpoints, 16);
  EXPECT_EQ(defender.unit_count, 4);
}

TEST(unit_block_test, strike_to_below_zero_hp) {
  UnitBlock attacker{.type = UnitOrigin::NEUTRAL,
                     .level = 1,
                     .name = "test",
                     .unit_count = 1,
                     .hitpoints_per_unit = 5,
                     .current_total_hitpoints = 3,
                     .speed = 4,
                     .min_damage = 1,
                     .max_damage = 5,
                     .attack = 0,
                     .defense = 0,
                     .can_retaliate = false,
                     .is_ranged = false,
                     .immune_to_retaliation = false};
  UnitBlock defender = attacker;
  strike<MockRngHighest>(attacker, defender);

  EXPECT_EQ(defender.current_total_hitpoints, 0);
  EXPECT_EQ(defender.unit_count, 0);
}

TEST(unit_block_test, perform_attack_no_retaliation) {
  UnitBlock attacker{.type = UnitOrigin::NEUTRAL,
                     .level = 1,
                     .name = "test",
                     .unit_count = 5,
                     .hitpoints_per_unit = 5,
                     .current_total_hitpoints = 25,
                     .speed = 4,
                     .min_damage = 1,
                     .max_damage = 5,
                     .attack = 0,
                     .defense = 0,
                     .can_retaliate = false,
                     .is_ranged = false,
                     .immune_to_retaliation = false};
  UnitBlock defender = attacker;
  performAttack<MockRngLowest>(attacker, defender);

  EXPECT_EQ(defender.current_total_hitpoints, 20);
  EXPECT_EQ(defender.unit_count, 4);

  EXPECT_EQ(attacker.current_total_hitpoints, 25);
  EXPECT_EQ(attacker.unit_count, 5);
}

TEST(unit_block_test, perform_attack_no_retaliation_ranged) {
  UnitBlock attacker{.type = UnitOrigin::NEUTRAL,
                     .level = 1,
                     .name = "test",
                     .unit_count = 5,
                     .hitpoints_per_unit = 5,
                     .current_total_hitpoints = 25,
                     .speed = 4,
                     .min_damage = 1,
                     .max_damage = 5,
                     .attack = 0,
                     .defense = 0,
                     .can_retaliate = true,
                     .is_ranged = true,
                     .immune_to_retaliation = false};
  UnitBlock defender = attacker;
  performAttack<MockRngLowest>(attacker, defender);

  EXPECT_EQ(defender.current_total_hitpoints, 20);
  EXPECT_EQ(defender.unit_count, 4);

  EXPECT_EQ(attacker.current_total_hitpoints, 25);
  EXPECT_EQ(attacker.unit_count, 5);
}

TEST(unit_block_test, perform_attack_no_retaliation_immune) {
  UnitBlock attacker{.type = UnitOrigin::NEUTRAL,
                     .level = 1,
                     .name = "test",
                     .unit_count = 5,
                     .hitpoints_per_unit = 5,
                     .current_total_hitpoints = 25,
                     .speed = 4,
                     .min_damage = 1,
                     .max_damage = 5,
                     .attack = 0,
                     .defense = 0,
                     .can_retaliate = true,
                     .is_ranged = false,
                     .immune_to_retaliation = true};
  UnitBlock defender = attacker;
  performAttack<MockRngLowest>(attacker, defender);

  EXPECT_EQ(defender.current_total_hitpoints, 20);
  EXPECT_EQ(defender.unit_count, 4);
  EXPECT_TRUE(defender.can_retaliate);

  EXPECT_EQ(attacker.current_total_hitpoints, 25);
  EXPECT_EQ(attacker.unit_count, 5);
  EXPECT_TRUE(attacker.can_retaliate);
}

TEST(unit_block_test, perform_attack_no_retaliation_defender_killed) {
  UnitBlock attacker{.type = UnitOrigin::NEUTRAL,
                     .level = 1,
                     .name = "test",
                     .unit_count = 2,
                     .hitpoints_per_unit = 5,
                     .current_total_hitpoints = 10,
                     .speed = 4,
                     .min_damage = 1,
                     .max_damage = 5,
                     .attack = 0,
                     .defense = 0,
                     .can_retaliate = true,
                     .is_ranged = false,
                     .immune_to_retaliation = false};
  UnitBlock defender = attacker;
  performAttack<MockRngHighest>(attacker, defender);

  EXPECT_EQ(defender.current_total_hitpoints, 0);
  EXPECT_EQ(defender.unit_count, 0);
  EXPECT_TRUE(defender.can_retaliate);

  EXPECT_EQ(attacker.current_total_hitpoints, 10);
  EXPECT_EQ(attacker.unit_count, 2);
  EXPECT_TRUE(attacker.can_retaliate);
}

TEST(unit_block_test, perform_attack_with_retaliation) {
  UnitBlock attacker{.type = UnitOrigin::NEUTRAL,
                     .level = 1,
                     .name = "test",
                     .unit_count = 5,
                     .hitpoints_per_unit = 5,
                     .current_total_hitpoints = 25,
                     .speed = 4,
                     .min_damage = 1,
                     .max_damage = 5,
                     .attack = 0,
                     .defense = 0,
                     .can_retaliate = true,
                     .is_ranged = false,
                     .immune_to_retaliation = false};
  UnitBlock defender = attacker;
  performAttack<MockRngLowest>(attacker, defender);

  EXPECT_EQ(defender.current_total_hitpoints, 20);
  EXPECT_EQ(defender.unit_count, 4);
  EXPECT_FALSE(defender.can_retaliate);

  EXPECT_EQ(attacker.current_total_hitpoints, 21);
  EXPECT_EQ(attacker.unit_count, 5);
  EXPECT_TRUE(attacker.can_retaliate);
}
