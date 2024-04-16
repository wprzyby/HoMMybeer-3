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
#include <Session.h>
#include <gtest/gtest.h>

TEST(sessionTests, allowsOneInstance) {
  Session* sess1 = Session::getInstance();
  Session* sess2 = Session::getInstance();
  EXPECT_EQ(sess1, sess2);
}

TEST(sessionTests, initializesWithNoGame) {
  Session* session = Session::getInstance();
  EXPECT_EQ(session->game, nullptr);
}

TEST(gameInitTests, basicGameInitialization) {
  Session* session = Session::getInstance();
  session->newGame(MapSize::S, 3, Difficulty::EASY);
  EXPECT_NE(session->game, nullptr);
  EXPECT_NO_THROW(Field* f = session->game->map_pointer->getField(0, 0));
  EXPECT_NO_THROW(Field* f = session->game->map_pointer->getField(200, 200));
  EXPECT_NO_THROW(Field* f = session->game->map_pointer->getField(499, 499));
  EXPECT_ANY_THROW(Field* f = session->game->map_pointer->getField(500, 500));
}