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
  EXPECT_NE(session->game->getMap()->getField(FieldCoords{0, 0}), nullptr);
  EXPECT_NE(session->game->getMap()->getField(FieldCoords{200, 200}), nullptr);
  EXPECT_NE(session->game->getMap()->getField(FieldCoords{499, 499}), nullptr);
  EXPECT_EQ(session->game->getMap()->getField(FieldCoords{500, 500}), nullptr);
}

TEST(gameInitTests, basicPlayerArrayInitialization) {
  Session* session = Session::getInstance();
  session->newGame(MapSize::S, 3, Difficulty::EASY);
  EXPECT_NE(session->game->getPlayer(0), nullptr);
  EXPECT_NE(session->game->getPlayer(1), nullptr);
  EXPECT_NE(session->game->getPlayer(3), nullptr);
  EXPECT_EQ(session->game->getPlayer(4), nullptr);
}

TEST(gameInitTests, playerInit) {
  Session* session = Session::getInstance();
  session->newGame(MapSize::S, 3, Difficulty::EASY);
  Player p = Player(false);
  EXPECT_NE(p.getHero(0), nullptr);
  EXPECT_EQ(p.getHero(1), nullptr);
}

TEST(gameInitTests, heroInit) {
  Session* session = Session::getInstance();
  session->newGame(MapSize::S, 3, Difficulty::EASY);
  Hero h = Hero("Christian", FieldCoords{20, 20});
  FieldCoords goal_coords = {20, 20};
  EXPECT_EQ(h.getHeroCoords(), goal_coords);
  EXPECT_EQ(h.getHeroName(), "Christian");
}

TEST(heroMovesTests, basicStep) {
  Session* session = Session::getInstance();
  session->newGame(MapSize::S, 3, Difficulty::EASY);
  FieldCoords start_coords = {20, 20};
  Hero h = Hero("Christian", start_coords);
  FieldCoords goal_coords = {21, 21};
  Path p = h.setMoveGoal(goal_coords).value();
  EXPECT_EQ(p.back(), goal_coords);
  h.move();
  EXPECT_EQ(h.getHeroCoords(), goal_coords);
}