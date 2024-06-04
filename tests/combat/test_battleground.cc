#include <Battleground.h>
#include <gtest/gtest.h>

#include "combat_common.h"

using namespace combat;

TEST(battleground_test, fields_in_battleground) {
  BattlegroundSize size{.odd_rows_count = 5,
                        .odd_rows_size = 15,
                        .even_rows_count = 5,
                        .even_rows_size = 15};
  Battleground battleground(size);

  // EDGES:
  EXPECT_TRUE(battleground.isFieldInBattleground({0, 0, 0}));   // top left
  EXPECT_TRUE(battleground.isFieldInBattleground({1, 4, 0}));   // bottom left
  EXPECT_TRUE(battleground.isFieldInBattleground({0, 0, 14}));  // top right
  EXPECT_TRUE(battleground.isFieldInBattleground({1, 4, 14}));  // bottom right

  // RANDOM ONES IN THE MIDDLE:
  EXPECT_TRUE(battleground.isFieldInBattleground({1, 2, 8}));
  EXPECT_TRUE(battleground.isFieldInBattleground({0, 3, 0}));
  EXPECT_TRUE(battleground.isFieldInBattleground({0, 4, 12}));

  // OUTSIDE OF EDGES:
  EXPECT_FALSE(battleground.isFieldInBattleground({0, 4, 15}));
  EXPECT_FALSE(battleground.isFieldInBattleground({1, 4, 15}));
  EXPECT_FALSE(battleground.isFieldInBattleground({0, 0, 15}));
  EXPECT_FALSE(battleground.isFieldInBattleground({1, 0, 15}));
  EXPECT_FALSE(battleground.isFieldInBattleground({0, 5, 0}));
  EXPECT_FALSE(battleground.isFieldInBattleground({1, 5, 0}));
}

TEST(battleground_test, adjacent_fields) {
  BattlegroundSize size{.odd_rows_count = 5,
                        .odd_rows_size = 15,
                        .even_rows_count = 5,
                        .even_rows_size = 15};
  Battleground battleground(size);

  // top left edge
  auto adjacent_tl = battleground.getAdjacentFields({0, 0, 0});
  EXPECT_EQ(adjacent_tl.size(), 2U);
  EXPECT_TRUE(adjacent_tl.contains({0, 0, 1}));
  EXPECT_TRUE(adjacent_tl.contains({1, 0, 0}));

  // top right edge
  auto adjacent_tr = battleground.getAdjacentFields({0, 0, 14});
  EXPECT_EQ(adjacent_tr.size(), 3U);
  EXPECT_TRUE(adjacent_tr.contains({0, 0, 13}));
  EXPECT_TRUE(adjacent_tr.contains({1, 0, 13}));
  EXPECT_TRUE(adjacent_tr.contains({1, 0, 14}));

  // bottom left edge
  auto adjacent_bl = battleground.getAdjacentFields({1, 4, 0});
  EXPECT_EQ(adjacent_bl.size(), 3U);
  EXPECT_TRUE(adjacent_bl.contains({1, 4, 1}));
  EXPECT_TRUE(adjacent_bl.contains({0, 4, 0}));
  EXPECT_TRUE(adjacent_bl.contains({0, 4, 1}));

  // bottom right edge
  auto adjacent_br = battleground.getAdjacentFields({1, 4, 14});
  EXPECT_EQ(adjacent_br.size(), 2U);
  EXPECT_TRUE(adjacent_br.contains({1, 4, 13}));
  EXPECT_TRUE(adjacent_br.contains({0, 4, 14}));

  // some field with viable adjacent fields on all sides
  auto adjacent_middle = battleground.getAdjacentFields({1, 0, 3});
  EXPECT_EQ(adjacent_middle.size(), 6U);
  EXPECT_TRUE(adjacent_middle.contains({1, 0, 2}));
  EXPECT_TRUE(adjacent_middle.contains({1, 0, 4}));
  EXPECT_TRUE(adjacent_middle.contains({0, 0, 3}));
  EXPECT_TRUE(adjacent_middle.contains({0, 0, 4}));
  EXPECT_TRUE(adjacent_middle.contains({0, 1, 3}));
  EXPECT_TRUE(adjacent_middle.contains({0, 1, 4}));
}

TEST(battleground_test, place_unit_ok) {
  Battleground battleground{};
  UnitIdentifier unit_id{HeroRole::ATTACKER, 0};
  HexFieldCoords coords = {1, 0, 0};
  auto succeeded = battleground.placeUnit(coords, unit_id);

  EXPECT_TRUE(succeeded);
  EXPECT_TRUE(battleground.isFieldOccupied(coords));
  EXPECT_EQ(battleground.getFieldOccupant(coords).value(), unit_id);
  EXPECT_EQ(battleground.getUnitCoords(unit_id).value(), coords);
}

TEST(battleground_test, place_unit_invalid_field) {
  Battleground battleground{};
  UnitIdentifier unit_id{HeroRole::ATTACKER, 0};
  HexFieldCoords coords = {1, 0, 15};
  auto succeeded = battleground.placeUnit(coords, unit_id);

  EXPECT_FALSE(succeeded);
  EXPECT_FALSE(battleground.isFieldOccupied(coords));
  EXPECT_FALSE(battleground.getFieldOccupant(coords).has_value());
  EXPECT_FALSE(battleground.getUnitCoords(unit_id).has_value());
}

TEST(battleground_test, place_unit_field_occupied) {
  Battleground battleground{};
  UnitIdentifier first_unit_id{HeroRole::ATTACKER, 0};
  UnitIdentifier second_unit_id{HeroRole::ATTACKER, 1};
  HexFieldCoords coords = {1, 0, 0};
  battleground.placeUnit(coords, first_unit_id);
  auto succeeded = battleground.placeUnit(coords, second_unit_id);

  EXPECT_FALSE(succeeded);
  EXPECT_TRUE(battleground.isFieldOccupied(coords));
  EXPECT_EQ(battleground.getFieldOccupant(coords).value(), first_unit_id);
  EXPECT_EQ(battleground.getUnitCoords(first_unit_id).value(), coords);
  EXPECT_FALSE(battleground.getUnitCoords(second_unit_id).has_value());
}

TEST(battleground_test, remove_unit_by_coords_ok) {
  Battleground battleground{};
  UnitIdentifier unit_id{HeroRole::ATTACKER, 0};
  HexFieldCoords coords = {1, 0, 0};
  battleground.placeUnit(coords, unit_id);
  auto succeeded = battleground.removeUnitByCoords(coords);

  EXPECT_TRUE(succeeded);
  EXPECT_FALSE(battleground.isFieldOccupied(coords));
  EXPECT_FALSE(battleground.getFieldOccupant(coords).has_value());
  EXPECT_FALSE(battleground.getUnitCoords(unit_id).has_value());
}

TEST(battleground_test, remove_unit_by_coords_no_unit_on_field) {
  Battleground battleground{};
  UnitIdentifier unit_id{HeroRole::ATTACKER, 0};
  HexFieldCoords correct_coords = {1, 0, 0};
  HexFieldCoords incorrect_coords = {1, 0, 1};
  battleground.placeUnit(correct_coords, unit_id);
  auto succeeded = battleground.removeUnitByCoords(incorrect_coords);

  EXPECT_FALSE(succeeded);
  EXPECT_TRUE(battleground.isFieldOccupied(correct_coords));
  EXPECT_EQ(battleground.getFieldOccupant(correct_coords).value(), unit_id);
  EXPECT_EQ(battleground.getUnitCoords(unit_id).value(), correct_coords);
}

TEST(battleground_test, remove_unit_by_identifier_ok) {
  Battleground battleground{};
  UnitIdentifier unit_id{HeroRole::ATTACKER, 0};
  HexFieldCoords coords = {1, 0, 0};
  battleground.placeUnit(coords, unit_id);
  auto succeeded = battleground.removeUnitByIdentifier(unit_id);

  EXPECT_TRUE(succeeded);
  EXPECT_FALSE(battleground.isFieldOccupied(coords));
  EXPECT_FALSE(battleground.getFieldOccupant(coords).has_value());
  EXPECT_FALSE(battleground.getUnitCoords(unit_id).has_value());
}

TEST(battleground_test, remove_unit_by_identifier_no_unit_on_field) {
  Battleground battleground{};
  UnitIdentifier unit_id{HeroRole::ATTACKER, 0};
  auto succeeded = battleground.removeUnitByIdentifier(unit_id);

  EXPECT_FALSE(succeeded);
}

TEST(battleground_test, remove_unit_by_identifier_double_remove) {
  Battleground battleground{};
  UnitIdentifier unit_id{HeroRole::ATTACKER, 0};
  HexFieldCoords coords = {1, 0, 0};
  battleground.placeUnit(coords, unit_id);
  battleground.removeUnitByIdentifier(unit_id);
  auto succeeded = battleground.removeUnitByIdentifier(unit_id);

  EXPECT_FALSE(succeeded);
  EXPECT_FALSE(battleground.isFieldOccupied(coords));
  EXPECT_FALSE(battleground.getFieldOccupant(coords).has_value());
  EXPECT_FALSE(battleground.getUnitCoords(unit_id).has_value());
}

TEST(battleground_test, move_unit_by_coords_ok) {
  Battleground battleground{};
  UnitIdentifier unit_id{HeroRole::ATTACKER, 0};
  HexFieldCoords starting_coords = {1, 0, 0};
  HexFieldCoords destination_coords = {1, 0, 3};
  battleground.placeUnit(starting_coords, unit_id);
  auto succeeded =
      battleground.moveUnitByCoords(starting_coords, destination_coords);

  EXPECT_TRUE(succeeded);
  EXPECT_TRUE(battleground.isFieldOccupied(destination_coords));
  EXPECT_EQ(battleground.getFieldOccupant(destination_coords).value(), unit_id);
  EXPECT_EQ(battleground.getUnitCoords(unit_id).value(), destination_coords);
}

TEST(battleground_test, move_unit_by_coords_invalid_field) {
  Battleground battleground{};
  UnitIdentifier unit_id{HeroRole::ATTACKER, 0};
  HexFieldCoords starting_coords = {1, 0, 0};
  HexFieldCoords destination_coords = {1, 0, 15};
  battleground.placeUnit(starting_coords, unit_id);
  auto succeeded =
      battleground.moveUnitByCoords(starting_coords, destination_coords);

  EXPECT_FALSE(succeeded);
  EXPECT_FALSE(battleground.isFieldOccupied(destination_coords));
  EXPECT_FALSE(battleground.getFieldOccupant(destination_coords).has_value());
  EXPECT_TRUE(battleground.getUnitCoords(unit_id).has_value());
  EXPECT_EQ(battleground.getUnitCoords(unit_id).value(), starting_coords);
}

TEST(battleground_test, move_unit_by_coords_field_occupied) {
  Battleground battleground{};
  UnitIdentifier first_unit_id{HeroRole::ATTACKER, 0};
  UnitIdentifier second_unit_id{HeroRole::ATTACKER, 1};
  HexFieldCoords first_coords = {1, 0, 0};
  HexFieldCoords second_coords = {0, 0, 0};
  battleground.placeUnit(first_coords, first_unit_id);
  battleground.placeUnit(second_coords, second_unit_id);
  auto succeeded = battleground.moveUnitByCoords(first_coords, second_coords);

  EXPECT_FALSE(succeeded);
  EXPECT_TRUE(battleground.isFieldOccupied(first_coords));
  EXPECT_TRUE(battleground.isFieldOccupied(second_coords));
  EXPECT_EQ(battleground.getFieldOccupant(first_coords).value(), first_unit_id);
  EXPECT_EQ(battleground.getFieldOccupant(second_coords).value(),
            second_unit_id);
  EXPECT_EQ(battleground.getUnitCoords(first_unit_id).value(), first_coords);
  EXPECT_EQ(battleground.getUnitCoords(second_unit_id).value(), second_coords);
}

TEST(battleground_test, move_unit_by_identifier_ok) {
  Battleground battleground{};
  UnitIdentifier unit_id{HeroRole::ATTACKER, 0};
  HexFieldCoords starting_coords = {1, 0, 0};
  HexFieldCoords destination_coords = {1, 0, 3};
  battleground.placeUnit(starting_coords, unit_id);
  auto succeeded =
      battleground.moveUnitByIdentifier(unit_id, destination_coords);

  EXPECT_TRUE(succeeded);
  EXPECT_TRUE(battleground.isFieldOccupied(destination_coords));
  EXPECT_EQ(battleground.getFieldOccupant(destination_coords).value(), unit_id);
  EXPECT_EQ(battleground.getUnitCoords(unit_id).value(), destination_coords);
}

TEST(battleground_test, move_unit_by_identifier_invalid_field) {
  Battleground battleground{};
  UnitIdentifier unit_id{HeroRole::ATTACKER, 0};
  HexFieldCoords starting_coords = {1, 0, 0};
  HexFieldCoords destination_coords = {1, 0, 15};
  battleground.placeUnit(starting_coords, unit_id);
  auto succeeded =
      battleground.moveUnitByIdentifier(unit_id, destination_coords);

  EXPECT_FALSE(succeeded);
  EXPECT_FALSE(battleground.isFieldOccupied(destination_coords));
  EXPECT_FALSE(battleground.getFieldOccupant(destination_coords).has_value());
  EXPECT_TRUE(battleground.getUnitCoords(unit_id).has_value());
  EXPECT_EQ(battleground.getUnitCoords(unit_id).value(), starting_coords);
}

TEST(battleground_test, move_unit_by_identifier_field_occupied) {
  Battleground battleground{};
  UnitIdentifier first_unit_id{HeroRole::ATTACKER, 0};
  UnitIdentifier second_unit_id{HeroRole::ATTACKER, 1};
  HexFieldCoords first_coords = {1, 0, 0};
  HexFieldCoords second_coords = {0, 0, 0};
  battleground.placeUnit(first_coords, first_unit_id);
  battleground.placeUnit(second_coords, second_unit_id);
  auto succeeded =
      battleground.moveUnitByIdentifier(first_unit_id, second_coords);

  EXPECT_FALSE(succeeded);
  EXPECT_TRUE(battleground.isFieldOccupied(first_coords));
  EXPECT_TRUE(battleground.isFieldOccupied(second_coords));
  EXPECT_EQ(battleground.getFieldOccupant(first_coords).value(), first_unit_id);
  EXPECT_EQ(battleground.getFieldOccupant(second_coords).value(),
            second_unit_id);
  EXPECT_EQ(battleground.getUnitCoords(first_unit_id).value(), first_coords);
  EXPECT_EQ(battleground.getUnitCoords(second_unit_id).value(), second_coords);
}

TEST(battleground_test, adjacent_enemy_units_none) {
  Battleground battleground{};
  UnitIdentifier first_unit_id{HeroRole::ATTACKER, 0};
  UnitIdentifier second_unit_id{HeroRole::ATTACKER, 1};
  HexFieldCoords first_coords = {1, 0, 0};
  HexFieldCoords second_coords = {0, 0, 0};
  battleground.placeUnit(first_coords, first_unit_id);
  battleground.placeUnit(second_coords, second_unit_id);
  EXPECT_EQ(battleground.getAdjacentEnemyUnitsCoords(first_unit_id).size(), 0U);
}

TEST(battleground_test, adjacent_enemy_units_one) {
  Battleground battleground{};
  UnitIdentifier first_unit_id{HeroRole::ATTACKER, 0};
  UnitIdentifier second_unit_id{HeroRole::DEFENDER, 0};
  HexFieldCoords first_coords = {1, 0, 0};
  HexFieldCoords second_coords = {0, 0, 0};
  battleground.placeUnit(first_coords, first_unit_id);
  battleground.placeUnit(second_coords, second_unit_id);
  EXPECT_EQ(battleground.getAdjacentEnemyUnitsCoords(first_unit_id).size(), 1U);
  EXPECT_TRUE(battleground.getAdjacentEnemyUnitsCoords(first_unit_id)
                  .contains(second_coords));
}

TEST(battleground_test, fields_in_range_field_not_in_battleground) {
  Battleground battleground{};
  HexFieldCoords coords = {0, 0, 15};
  auto result = battleground.getFieldsWithinRange(coords, 1);

  EXPECT_EQ(result.size(), 0U);
}

TEST(battleground_test, fields_in_range_radius_one_edge) {
  Battleground battleground{};
  HexFieldCoords coords = {0, 0, 0};
  auto result = battleground.getFieldsWithinRange(coords, 1);

  EXPECT_EQ(result, battleground.getAdjacentFields(coords));
}

TEST(battleground_test, fields_in_range_radius_one_middle) {
  Battleground battleground{};
  HexFieldCoords coords = {0, 1, 3};
  auto result = battleground.getFieldsWithinRange(coords, 1);

  EXPECT_EQ(result, battleground.getAdjacentFields(coords));
}

TEST(battleground_test, fields_in_range_radius_two_edge) {
  Battleground battleground{};
  HexFieldCoords coords = {0, 0, 0};
  auto result = battleground.getFieldsWithinRange(coords, 2);
  auto adjacent = battleground.getAdjacentFields(coords);
  std::vector<HexFieldCoords> diff;
  std::set_difference(adjacent.begin(), adjacent.end(), result.begin(),
                      result.end(), diff.begin());

  EXPECT_EQ(result.size(), 6U);
  EXPECT_EQ(diff.size(), 0U);
  EXPECT_TRUE(result.contains({0, 0, 2}));
  EXPECT_TRUE(result.contains({1, 0, 1}));
  EXPECT_TRUE(result.contains({0, 1, 0}));
  EXPECT_TRUE(result.contains({0, 1, 1}));
}

TEST(battleground_test, fields_in_range_radius_two_middle) {
  Battleground battleground{};
  HexFieldCoords coords = {0, 1, 3};
  auto result = battleground.getFieldsWithinRange(coords, 2);
  auto adjacent = battleground.getAdjacentFields(coords);
  std::vector<HexFieldCoords> diff;
  std::set_difference(adjacent.begin(), adjacent.end(), result.begin(),
                      result.end(), diff.begin());

  EXPECT_EQ(result.size(), 18U);
  EXPECT_EQ(diff.size(), 0U);
  EXPECT_TRUE(result.contains({0, 0, 2}));
  EXPECT_TRUE(result.contains({0, 0, 3}));
  EXPECT_TRUE(result.contains({0, 0, 4}));
  EXPECT_TRUE(result.contains({1, 0, 1}));
  EXPECT_TRUE(result.contains({1, 0, 4}));
  EXPECT_TRUE(result.contains({0, 1, 1}));
  EXPECT_TRUE(result.contains({0, 1, 5}));
  EXPECT_TRUE(result.contains({1, 1, 1}));
  EXPECT_TRUE(result.contains({1, 1, 4}));
  EXPECT_TRUE(result.contains({0, 2, 2}));
  EXPECT_TRUE(result.contains({0, 2, 3}));
  EXPECT_TRUE(result.contains({0, 2, 4}));
}

TEST(battleground_test, fields_in_range_one_unit_blocking) {
  Battleground battleground{};
  battleground.placeUnit({0, 0, 1}, {HeroRole::ATTACKER, 0});
  HexFieldCoords coords = {0, 0, 0};
  auto result = battleground.getFieldsWithinRange(coords, 1);

  EXPECT_NE(result, battleground.getAdjacentFields(coords));
  EXPECT_EQ(result.size(), 1U);
  EXPECT_TRUE(result.contains({1, 0, 0}));
}

TEST(battleground_test, fields_in_range_two_unit_blocking) {
  Battleground battleground{};
  battleground.placeUnit({0, 0, 1}, {HeroRole::ATTACKER, 0});
  HexFieldCoords coords = {0, 0, 0};
  auto result = battleground.getFieldsWithinRange(coords, 2);

  EXPECT_NE(result, battleground.getAdjacentFields(coords));
  EXPECT_EQ(result.size(), 4U);
  EXPECT_TRUE(result.contains({1, 0, 0}));
  EXPECT_TRUE(result.contains({1, 0, 1}));
  EXPECT_TRUE(result.contains({0, 1, 0}));
  EXPECT_TRUE(result.contains({0, 1, 1}));
}