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

#include <Game.h>
#include <Map.h>
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

TEST(gameInitTests, basicMapInitialization) {
  Session* session = Session::getInstance();
  session->newGame(MapSize::S, 3, Difficulty::EASY);
  EXPECT_NE(session->game, nullptr);
  EXPECT_NE(session->game->getMap()->getField(0, 0).value_or(nullptr), nullptr);
  EXPECT_NE(session->game->getMap()->getField(200, 200).value_or(nullptr),
            nullptr);
  EXPECT_NE(session->game->getMap()->getField(499, 499).value_or(nullptr),
            nullptr);
  EXPECT_EQ(session->game->getMap()->getField(500, 500).value_or(nullptr),
            nullptr);
}

TEST(gameInitTests, basicPlayerArrayInitialization) {
  Session* session = Session::getInstance();
  session->newGame(MapSize::S, 3, Difficulty::EASY);
  EXPECT_NE(session->game->getPlayer(0).value_or(nullptr), nullptr);
  EXPECT_NE(session->game->getPlayer(1).value_or(nullptr), nullptr);
  EXPECT_NE(session->game->getPlayer(3).value_or(nullptr), nullptr);
  EXPECT_EQ(session->game->getPlayer(4).value_or(nullptr), nullptr);
}

TEST(gameInitTests, playerInit) {
  Session* session = Session::getInstance();
  session->newGame(MapSize::S, 3, Difficulty::EASY);
  Player p = Player(false, session->game->getMap());
  EXPECT_NE(p.getHero(0).value_or(nullptr), nullptr);
  EXPECT_EQ(p.getHero(1).value_or(nullptr), nullptr);
}

TEST(gameInitTests, heroInit) {
  Session* session = Session::getInstance();
  session->newGame(MapSize::S, 3, Difficulty::EASY);
  Hero h = Hero("Christian",
                session->game->getMap()->getField(20, 20).value_or(nullptr));
  std::pair<int, int> goal_coords = {20, 20};
  EXPECT_EQ(h.getHeroCoords(), goal_coords);
  EXPECT_EQ(h.getHeroName(), "Christian");
}