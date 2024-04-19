/**
 * @file main.cc
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2024-04-16
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <Field.h>
#include <Game.h>
#include <Map.h>
#include <MapUtils.h>
#include <Session.h>
#include <gtest/gtest.h>

#include <utility>

TEST(sessionTests, allowsOneInstance) {
  Session* sess1 = Session::getInstance();
  Session* sess2 = Session::getInstance();
  EXPECT_EQ(sess1, sess2);
}

TEST(sessionTests, initializesWithNoGame) {
  Session* session = Session::getInstance();
  EXPECT_EQ(session->game, nullptr);
}

TEST(mapUtilsTest, generateGrassMap) {
  MapInfo map_info = generateGrassMap(MapSize::S, 3);
  EXPECT_EQ(map_info.map.getWidth(), static_cast<int>(MapSize::S));
  EXPECT_EQ(map_info.num_of_players, 3);
  EXPECT_EQ(map_info.starting_locations.size(), 3);
  EXPECT_EQ(map_info.map.getField(FieldCoords{1, 1})->isWalkable(), true);
}

TEST(mapTests, basicMapFunctionality) {
  MapInfo map_i = generateGrassMap(MapSize::S, 3);
  Map map = map_i.map;
  EXPECT_NE(map.getField(FieldCoords{0, 0}), nullptr);
  EXPECT_EQ(map.getField(FieldCoords{static_cast<int>(MapSize::S) + 1,
                                     static_cast<int>(MapSize::S) + 1}),
            nullptr);
}