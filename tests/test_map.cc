/**
 * @file test_map.cc
 * @author Piotr Kluba (you@domain.com)
 * @brief Tests of Map, its generators and Field
 * @version 0.1
 * @date 2024-06-01
 *
 * @copyright Copyright (c) 2024
 *
 */
#include <Field.h>
#include <Map.h>
#include <MapUtils.h>
#include <common.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <testUtils.h>

TEST(mapUtilsTest, generateGrassMapTest) {
  MapInfo map_info = generateGrassMap(MapSize::S, 3);
  EXPECT_EQ(map_info.map.getWidth(), static_cast<int>(MapSize::S));
  EXPECT_EQ(map_info.num_of_players, 3);
  EXPECT_EQ(map_info.starting_locations.size(), 3);
  EXPECT_EQ(map_info.map.getField(FieldCoords{1, 1}).value()->isWalkable(),
            true);
}

TEST(fieldTests, isAdjacentTest) {
  EXPECT_TRUE(Field::isAdjacent(FieldCoords{0, 0}, FieldCoords{0, 1}));
  EXPECT_TRUE(Field::isAdjacent(FieldCoords{34, 23}, FieldCoords{35, 23}));
  EXPECT_TRUE(Field::isAdjacent(FieldCoords{0, 0}, FieldCoords{-1, 0}));
  EXPECT_TRUE(Field::isAdjacent(FieldCoords{3, 3}, FieldCoords{4, 4}));
  EXPECT_TRUE(Field::isAdjacent(FieldCoords{7, 9}, FieldCoords{7, 8}));
  EXPECT_TRUE(Field::isAdjacent(FieldCoords{5, 4}, FieldCoords{4, 4}));
  EXPECT_TRUE(Field::isAdjacent(FieldCoords{6, 3}, FieldCoords{5, 2}));
  EXPECT_FALSE(Field::isAdjacent(FieldCoords{6, 3}, FieldCoords{8, 3}));
  EXPECT_FALSE(Field::isAdjacent(FieldCoords{0, 0}, FieldCoords{-2, -2}));
  EXPECT_FALSE(Field::isAdjacent(FieldCoords{4, 6}, FieldCoords{4, 4}));
  EXPECT_FALSE(Field::isAdjacent(FieldCoords{2, 43}, FieldCoords{0, 0}));
}

TEST(mapTests, basicMapFunctionality) {
  MapInfo map_i = generateGrassMap(MapSize::S, 3);
  Map map = map_i.map;
  EXPECT_TRUE(map.getField(FieldCoords{0, 0}).has_value());
  EXPECT_FALSE(map.getField(FieldCoords{static_cast<int>(MapSize::S) + 1,
                                        static_cast<int>(MapSize::S) + 1})
                   .has_value());
}

TEST(mapUtilsTest, exampleMapInitTest) {
  Game game = createExampleGame();
  EXPECT_FALSE(
      game.getMap()->getField(FieldCoords{10, 12}).value()->isWalkable());
  EXPECT_FALSE(
      game.getMap()->getField(FieldCoords{21, 26}).value()->isWalkable());
  EXPECT_FALSE(
      game.getMap()->getField(FieldCoords{4, 28}).value()->isWalkable());
  EXPECT_FALSE(
      game.getMap()->getField(FieldCoords{36, 13}).value()->isWalkable());
  EXPECT_FALSE(
      game.getMap()->getField(FieldCoords{22, 29}).value()->isWalkable());
  EXPECT_FALSE(
      game.getMap()->getField(FieldCoords{33, 38}).value()->isWalkable());
  EXPECT_FALSE(
      game.getMap()->getField(FieldCoords{41, 48}).value()->isWalkable());
  EXPECT_FALSE(
      game.getMap()->getField(FieldCoords{41, 39}).value()->isWalkable());
  EXPECT_TRUE(
      game.getMap()->getField(FieldCoords{36, 27}).value()->isWalkable());
  EXPECT_TRUE(
      game.getMap()->getField(FieldCoords{46, 40}).value()->isWalkable());
  EXPECT_TRUE(
      game.getMap()->getField(FieldCoords{21, 43}).value()->isWalkable());
  EXPECT_TRUE(
      game.getMap()->getField(FieldCoords{22, 27}).value()->isWalkable());
  EXPECT_TRUE(
      game.getMap()->getField(FieldCoords{11, 2}).value()->isWalkable());
  EXPECT_TRUE(
      game.getMap()->getField(FieldCoords{10, 17}).value()->isWalkable());
  EXPECT_TRUE(
      game.getMap()->getField(FieldCoords{20, 26}).value()->isWalkable());
}