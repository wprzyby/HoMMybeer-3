/**
 * @file test_session.cc
 * @author Piotr Kluba
 * @brief Unit tests for Session class
 * @copyright Copyright (c) 2024
 */

#include <Session.h>
#include <game_logic_utils.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <testUtils.h>

TEST(sessionTests, allowsOneInstance) {
  Session* sess1 = Session::getInstance();
  Session* sess2 = Session::getInstance();
  EXPECT_EQ(sess1, sess2);
}