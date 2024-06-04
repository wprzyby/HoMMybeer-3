#include <BattleState.h>
#include <gtest/gtest.h>

#include "Battleground.h"
#include "UnitBlock.hpp"
#include "combat_common.h"


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

BattleState initStateTwoUnits(const UnitContainer& units,
                              BattleState::RoundPhase phase,
                              HexFieldCoords position_attacker = {0, 0, 0},
                              HexFieldCoords position_defender = {0, 0, 14}) {
  Battleground battleground;
  battleground.placeUnit(position_attacker, {HeroRole::ATTACKER, 0});
  battleground.placeUnit(position_defender, {HeroRole::DEFENDER, 0});

  auto queue = BattleState::setupUnitMoveQueue(getAllUnitIds(units), units);
  auto first_moving = queue.front();
  queue.pop_front();
  return BattleState{
      units, queue, battleground, first_moving, first_moving.first, phase, {}};
}

TEST(battlestate_test, simple_initialization) {
  UnitContainer units;
  UnitBlock attacker_unit = UNIT_BLOCK_PROTOTYPE;
  UnitBlock defender_unit = UNIT_BLOCK_PROTOTYPE;
  defender_unit.speed = 6;
  units.insert(HeroRole::ATTACKER, attacker_unit);
  units.insert(HeroRole::DEFENDER, defender_unit);
  BattleState state = initStateTwoUnits(units, BattleState::RoundPhase::MOVING);

  EXPECT_EQ(state.currently_moving_, HeroRole::DEFENDER);
  EXPECT_EQ(state.current_unit_id_, std::make_pair(HeroRole::DEFENDER, 0U));
  EXPECT_EQ(state.unit_move_queue_.size(), 1U);
  EXPECT_EQ(state.unit_move_queue_.front(),
            std::make_pair(HeroRole::ATTACKER, 0U));
}

TEST(battlestate_test, move_possible_moves) {
  UnitContainer units;
  UnitBlock attacker_unit = UNIT_BLOCK_PROTOTYPE;
  UnitBlock defender_unit = UNIT_BLOCK_PROTOTYPE;
  attacker_unit.speed = 2;
  defender_unit.speed = 1;
  units.insert(HeroRole::ATTACKER, attacker_unit);
  units.insert(HeroRole::DEFENDER, defender_unit);
  auto possible_moves =
      initStateTwoUnits(units, BattleState::RoundPhase::MOVING)
          .getPossibleMoves();

  EXPECT_EQ(possible_moves.size(), 6U);
  EXPECT_TRUE(possible_moves.contains({0, 0, 1}));
  EXPECT_TRUE(possible_moves.contains({0, 0, 2}));
  EXPECT_TRUE(possible_moves.contains({1, 0, 0}));
  EXPECT_TRUE(possible_moves.contains({1, 0, 1}));
  EXPECT_TRUE(possible_moves.contains({0, 1, 0}));
  EXPECT_TRUE(possible_moves.contains({0, 1, 1}));
}

TEST(battlestate_test, attack_possible_moves_melee) {
  UnitContainer units;
  UnitBlock attacker_unit = UNIT_BLOCK_PROTOTYPE;
  UnitBlock defender_unit = UNIT_BLOCK_PROTOTYPE;
  attacker_unit.speed = 2;
  defender_unit.speed = 1;
  units.insert(HeroRole::ATTACKER, attacker_unit);
  units.insert(HeroRole::DEFENDER, defender_unit);
  auto possible_moves =
      initStateTwoUnits(units, BattleState::RoundPhase::ATTACKING, {0, 0, 0},
                        {0, 0, 1})
          .getPossibleMoves();

  EXPECT_EQ(possible_moves.size(), 1U);
  EXPECT_TRUE(possible_moves.contains({0, 0, 1}));
}

TEST(battlestate_test, attack_possible_moves_ranged) {
  UnitContainer units;
  UnitBlock attacker_unit = UNIT_BLOCK_PROTOTYPE;
  UnitBlock defender_unit = UNIT_BLOCK_PROTOTYPE;
  attacker_unit.speed = 2;
  attacker_unit.is_ranged = true;
  defender_unit.speed = 1;
  units.insert(HeroRole::ATTACKER, attacker_unit);
  units.insert(HeroRole::DEFENDER, defender_unit);
  auto possible_moves =
      initStateTwoUnits(units, BattleState::RoundPhase::ATTACKING, {0, 0, 0},
                        {0, 0, 14})
          .getPossibleMoves();

  EXPECT_EQ(possible_moves.size(), 1U);
  EXPECT_TRUE(possible_moves.contains({0, 0, 14}));
}

TEST(battlestate_test, move_transition_to_moving) {
  UnitContainer units;
  UnitBlock attacker_unit = UNIT_BLOCK_PROTOTYPE;
  UnitBlock defender_unit = UNIT_BLOCK_PROTOTYPE;
  defender_unit.speed = 6;
  units.insert(HeroRole::ATTACKER, attacker_unit);
  units.insert(HeroRole::DEFENDER, defender_unit);
  BattleState state = initStateTwoUnits(units, BattleState::RoundPhase::MOVING)
                          .makeMove(std::make_tuple(0, 0, 13));

  EXPECT_EQ(state.currently_moving_, HeroRole::ATTACKER);
  EXPECT_EQ(state.current_unit_id_, std::make_pair(HeroRole::ATTACKER, 0U));
  EXPECT_EQ(state.unit_move_queue_.size(), 0U);
  EXPECT_EQ(state.round_phase_, BattleState::RoundPhase::MOVING);
  EXPECT_FALSE(state.battleground_.isFieldOccupied({0, 0, 14}));
  EXPECT_TRUE(state.battleground_.isFieldOccupied({0, 0, 13}));
}

TEST(battlestate_test, move_transition_to_attacking_adjacent) {
  UnitContainer units;
  UnitBlock attacker_unit = UNIT_BLOCK_PROTOTYPE;
  UnitBlock defender_unit = UNIT_BLOCK_PROTOTYPE;
  defender_unit.speed = 15;
  units.insert(HeroRole::ATTACKER, attacker_unit);
  units.insert(HeroRole::DEFENDER, defender_unit);
  BattleState state = initStateTwoUnits(units, BattleState::RoundPhase::MOVING)
                          .makeMove(std::make_tuple(0, 0, 1));

  EXPECT_EQ(state.currently_moving_, HeroRole::DEFENDER);
  EXPECT_EQ(state.current_unit_id_, std::make_pair(HeroRole::DEFENDER, 0U));
  EXPECT_EQ(state.unit_move_queue_.size(), 1U);
  EXPECT_EQ(state.unit_move_queue_.front(),
            std::make_pair(HeroRole::ATTACKER, 0U));
  EXPECT_EQ(state.round_phase_, BattleState::RoundPhase::ATTACKING);
  EXPECT_FALSE(state.battleground_.isFieldOccupied({0, 0, 14}));
  EXPECT_TRUE(state.battleground_.isFieldOccupied({0, 0, 1}));
}

TEST(battlestate_test, move_transition_to_attacking_ranged) {
  UnitContainer units;
  UnitBlock attacker_unit = UNIT_BLOCK_PROTOTYPE;
  UnitBlock defender_unit = UNIT_BLOCK_PROTOTYPE;
  defender_unit.speed = 5;
  defender_unit.is_ranged = true;
  units.insert(HeroRole::ATTACKER, attacker_unit);
  units.insert(HeroRole::DEFENDER, defender_unit);
  BattleState state = initStateTwoUnits(units, BattleState::RoundPhase::MOVING)
                          .makeMove(std::make_tuple(0, 0, 13));

  EXPECT_EQ(state.currently_moving_, HeroRole::DEFENDER);
  EXPECT_EQ(state.current_unit_id_, std::make_pair(HeroRole::DEFENDER, 0U));
  EXPECT_EQ(state.unit_move_queue_.size(), 1U);
  EXPECT_EQ(state.unit_move_queue_.front(),
            std::make_pair(HeroRole::ATTACKER, 0U));
  EXPECT_EQ(state.round_phase_, BattleState::RoundPhase::ATTACKING);
  EXPECT_FALSE(state.battleground_.isFieldOccupied({0, 0, 14}));
  EXPECT_TRUE(state.battleground_.isFieldOccupied({0, 0, 13}));
}

TEST(battlestate_test, attack_transition_to_moving_melee) {
  UnitContainer units;
  UnitBlock attacker_unit = UNIT_BLOCK_PROTOTYPE;
  UnitBlock defender_unit = UNIT_BLOCK_PROTOTYPE;
  defender_unit.speed = 6;
  units.insert(HeroRole::ATTACKER, attacker_unit);
  units.insert(HeroRole::DEFENDER, defender_unit);
  BattleState state =
      initStateTwoUnits(units, BattleState::RoundPhase::ATTACKING, {0, 0, 0},
                        {0, 0, 1})
          .makeMove(std::make_tuple(0, 0, 0));

  EXPECT_EQ(state.currently_moving_, HeroRole::ATTACKER);
  EXPECT_EQ(state.current_unit_id_, std::make_pair(HeroRole::ATTACKER, 0U));
  EXPECT_EQ(state.unit_move_queue_.size(), 0U);
  EXPECT_EQ(state.round_phase_, BattleState::RoundPhase::MOVING);
  EXPECT_TRUE(state.battleground_.isFieldOccupied({0, 0, 0}));
  EXPECT_TRUE(state.battleground_.isFieldOccupied({0, 0, 1}));
  EXPECT_TRUE(
      state.hero_units_.at({HeroRole::ATTACKER, 0}).current_total_hitpoints <
      25);
  EXPECT_TRUE(
      state.hero_units_.at({HeroRole::DEFENDER, 0}).current_total_hitpoints <
      25);
}

TEST(battlestate_test, attack_transition_to_moving_ranged) {
  UnitContainer units;
  UnitBlock attacker_unit = UNIT_BLOCK_PROTOTYPE;
  UnitBlock defender_unit = UNIT_BLOCK_PROTOTYPE;
  defender_unit.speed = 6;
  defender_unit.is_ranged = true;
  units.insert(HeroRole::ATTACKER, attacker_unit);
  units.insert(HeroRole::DEFENDER, defender_unit);
  BattleState state =
      initStateTwoUnits(units, BattleState::RoundPhase::ATTACKING, {0, 0, 0},
                        {0, 0, 14})
          .makeMove(std::make_tuple(0, 0, 0));

  EXPECT_EQ(state.currently_moving_, HeroRole::ATTACKER);
  EXPECT_EQ(state.current_unit_id_, std::make_pair(HeroRole::ATTACKER, 0U));
  EXPECT_EQ(state.unit_move_queue_.size(), 0U);
  EXPECT_EQ(state.round_phase_, BattleState::RoundPhase::MOVING);
  EXPECT_TRUE(state.battleground_.isFieldOccupied({0, 0, 0}));
  EXPECT_TRUE(state.battleground_.isFieldOccupied({0, 0, 14}));
  EXPECT_TRUE(
      state.hero_units_.at({HeroRole::ATTACKER, 0}).current_total_hitpoints <
      25);
  EXPECT_FALSE(
      state.hero_units_.at({HeroRole::DEFENDER, 0}).current_total_hitpoints <
      25);
}

TEST(battlestate_test, reload_unit_queue) {
  UnitContainer units;
  UnitBlock attacker_unit = UNIT_BLOCK_PROTOTYPE;
  UnitBlock defender_unit = UNIT_BLOCK_PROTOTYPE;
  attacker_unit.speed = 6;
  units.insert(HeroRole::ATTACKER, attacker_unit);
  units.insert(HeroRole::DEFENDER, defender_unit);
  BattleState state = initStateTwoUnits(units, BattleState::RoundPhase::MOVING)
                          .makeMove(std::make_tuple(0, 0, 1))
                          .makeMove(std::make_tuple(0, 0, 13));

  EXPECT_EQ(state.currently_moving_, HeroRole::ATTACKER);
  EXPECT_EQ(state.current_unit_id_, std::make_pair(HeroRole::ATTACKER, 0U));
  EXPECT_EQ(state.unit_move_queue_.size(), 1U);
  EXPECT_EQ(state.unit_move_queue_.front(),
            std::make_pair(HeroRole::DEFENDER, 0U));
}

TEST(battlestate_test, attack_to_kill_and_win) {
  UnitContainer units;
  UnitBlock attacker_unit = UNIT_BLOCK_PROTOTYPE;
  UnitBlock defender_unit = UNIT_BLOCK_PROTOTYPE;
  defender_unit.speed = 6;
  attacker_unit.current_total_hitpoints = 3;
  attacker_unit.unit_count = 1;
  units.insert(HeroRole::ATTACKER, attacker_unit);
  units.insert(HeroRole::DEFENDER, defender_unit);
  BattleState state =
      initStateTwoUnits(units, BattleState::RoundPhase::ATTACKING, {0, 0, 0},
                        {0, 0, 1})
          .makeMove(std::make_tuple(0, 0, 0));

  EXPECT_EQ(state.currently_moving_, HeroRole::DEFENDER);
  EXPECT_EQ(state.current_unit_id_, std::make_pair(HeroRole::DEFENDER, 0U));
  EXPECT_EQ(state.unit_move_queue_.size(), 0U);
  EXPECT_EQ(state.round_phase_, BattleState::RoundPhase::MOVING);
  EXPECT_TRUE(state.battleground_.isFieldOccupied({0, 0, 1}));
  EXPECT_FALSE(state.battleground_.isFieldOccupied({0, 0, 0}));
  EXPECT_EQ(
      state.hero_units_.at({HeroRole::ATTACKER, 0}).current_total_hitpoints, 0);
  EXPECT_EQ(
      state.hero_units_.at({HeroRole::DEFENDER, 0}).current_total_hitpoints,
      25);
  EXPECT_TRUE(state.winner_.has_value());
  EXPECT_EQ(state.winner_, HeroRole::DEFENDER);
}

TEST(battlestate_test, pass_transition_from_moving_to_moving) {
  UnitContainer units;
  UnitBlock attacker_unit = UNIT_BLOCK_PROTOTYPE;
  UnitBlock defender_unit = UNIT_BLOCK_PROTOTYPE;
  defender_unit.speed = 6;
  units.insert(HeroRole::ATTACKER, attacker_unit);
  units.insert(HeroRole::DEFENDER, defender_unit);
  BattleState state = initStateTwoUnits(units, BattleState::RoundPhase::MOVING)
                          .makeMove(std::nullopt);

  EXPECT_EQ(state.currently_moving_, HeroRole::ATTACKER);
  EXPECT_EQ(state.current_unit_id_, std::make_pair(HeroRole::ATTACKER, 0U));
  EXPECT_EQ(state.unit_move_queue_.size(), 0U);
  EXPECT_EQ(state.round_phase_, BattleState::RoundPhase::MOVING);
  EXPECT_TRUE(state.battleground_.isFieldOccupied({0, 0, 14}));
}

TEST(battlestate_test, pass_transition_from_moving_to_attacking) {
  UnitContainer units;
  UnitBlock attacker_unit = UNIT_BLOCK_PROTOTYPE;
  UnitBlock defender_unit = UNIT_BLOCK_PROTOTYPE;
  defender_unit.speed = 6;
  units.insert(HeroRole::ATTACKER, attacker_unit);
  units.insert(HeroRole::DEFENDER, defender_unit);
  BattleState state = initStateTwoUnits(units, BattleState::RoundPhase::MOVING,
                                        {0, 0, 0}, {0, 0, 1})
                          .makeMove(std::nullopt);

  EXPECT_EQ(state.currently_moving_, HeroRole::DEFENDER);
  EXPECT_EQ(state.current_unit_id_, std::make_pair(HeroRole::DEFENDER, 0U));
  EXPECT_EQ(state.unit_move_queue_.size(), 1U);
  EXPECT_EQ(state.unit_move_queue_.front(),
            std::make_pair(HeroRole::ATTACKER, 0U));
  EXPECT_EQ(state.round_phase_, BattleState::RoundPhase::ATTACKING);
  EXPECT_TRUE(state.battleground_.isFieldOccupied({0, 0, 1}));
}

TEST(battlestate_test, pass_attacking) {
  UnitContainer units;
  UnitBlock attacker_unit = UNIT_BLOCK_PROTOTYPE;
  UnitBlock defender_unit = UNIT_BLOCK_PROTOTYPE;
  defender_unit.speed = 6;
  units.insert(HeroRole::ATTACKER, attacker_unit);
  units.insert(HeroRole::DEFENDER, defender_unit);
  BattleState state =
      initStateTwoUnits(units, BattleState::RoundPhase::ATTACKING, {0, 0, 0},
                        {0, 0, 1})
          .makeMove(std::nullopt);

  EXPECT_EQ(state.currently_moving_, HeroRole::ATTACKER);
  EXPECT_EQ(state.current_unit_id_, std::make_pair(HeroRole::ATTACKER, 0U));
  EXPECT_EQ(state.unit_move_queue_.size(), 0U);
  EXPECT_EQ(state.round_phase_, BattleState::RoundPhase::MOVING);
}