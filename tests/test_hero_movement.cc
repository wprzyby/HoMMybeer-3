/**
 * @file test_hero_movement.cc
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2024-06-01
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <Game.h>
#include <common.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <testUtils.h>

TEST(heroMovementTests, basicHeroMovementTest) {
  Game game = createBasicGame();
  ASSERT_EQ(game.getCurrentPlayer()->getCurrentHero()->getHeroCoords(),
            (FieldCoords{1, 1}));
  ASSERT_EQ(game.getCurrentPlayer()->getCurrentHero()->getEnergy(), 100);
  Path move_path({FieldCoords{1, 4}, FieldCoords{1, 3}, FieldCoords{1, 2}});
  Path other_path({FieldCoords{1, 2}, FieldCoords{1, 3}, FieldCoords{2, 3}});
  MoveCosts costs({10, 10, 10});
  game.getCurrentPlayer()->getCurrentHero()->setMovePath(move_path);
  EXPECT_FALSE(
      game.getCurrentPlayer()->getCurrentHero()->moveAlong(other_path, costs));
  EXPECT_TRUE(
      game.getCurrentPlayer()->getCurrentHero()->moveAlong(move_path, costs));
  EXPECT_EQ(game.getCurrentPlayer()->getCurrentHero()->getEnergy(), 70);
  ASSERT_EQ(game.getCurrentPlayer()->getCurrentHero()->getHeroCoords(),
            (FieldCoords{1, 4}));
  move_path = Path({FieldCoords{5, 5}, FieldCoords{4, 4}, FieldCoords{3, 4},
                    FieldCoords{2, 4}});
  costs = MoveCosts({20, 20, 20, 20});
  game.getCurrentPlayer()->getCurrentHero()->setMovePath(move_path);
  EXPECT_TRUE(
      game.getCurrentPlayer()->getCurrentHero()->moveAlong(move_path, costs));
  ASSERT_EQ(game.getCurrentPlayer()->getCurrentHero()->getHeroCoords(),
            (FieldCoords{4, 4}));
  EXPECT_EQ(game.getCurrentPlayer()->getCurrentHero()->getEnergy(), 10);
  game.getCurrentPlayer()->getCurrentHero()->refillEnergy();
  EXPECT_EQ(game.getCurrentPlayer()->getCurrentHero()->getEnergy(), 100);
  move_path = Path({FieldCoords{5, 5}});
  costs = MoveCosts({20});
  EXPECT_TRUE(
      game.getCurrentPlayer()->getCurrentHero()->moveAlong(move_path, costs));
  EXPECT_EQ(game.getCurrentPlayer()->getCurrentHero()->getHeroCoords(),
            (FieldCoords{5, 5}));
  EXPECT_EQ(game.getCurrentPlayer()->getCurrentHero()->getEnergy(), 80);
}

TEST(dikstrjaTests, constructGraphTest) {
  Game game = createBasicGame();
  std::set<FieldCoords> graph = game.getMap()->constructGraph(
      game.getCurrentPlayer()->getCurrentHero()->getHeroCoords());
  EXPECT_EQ(graph.size(),
            game.getMap()->getWidth() * game.getMap()->getWidth());
  game.addMapObject(
      std::make_shared<City>(FieldCoords{12, 12}, &game, Faction::CASTLE));
  graph = game.getMap()->constructGraph(
      game.getCurrentPlayer()->getCurrentHero()->getHeroCoords());
  EXPECT_EQ(graph.size(),
            game.getMap()->getWidth() * game.getMap()->getWidth() - 13);
}

TEST(heroMovementTests, heroNotMovingNoPath) {
  Game game = createExampleGame();
  ASSERT_EQ(game.getCurrentPlayer()->getCurrentHero()->getHeroCoords(),
            (FieldCoords{7, 3}));
  game.getCurrentPlayer()->getCurrentHero()->setMovePath({});
  game.getCurrentPlayer()->getCurrentHero()->moveAlong({}, {});
  EXPECT_EQ(game.getCurrentPlayer()->getCurrentHero()->getHeroCoords(),
            (FieldCoords{7, 3}));
}

TEST(heroMovementTests, heroNotMovingNoEnergy) {
  Game game = createExampleGame();
  game.getCurrentPlayer()->addHero("Tired Tristan", FieldCoords{7, 4}, 0);
  game.getCurrentPlayer()->nextHero();
  game.getCurrentPlayer()->getCurrentHero()->setMovePath(
      Path({FieldCoords{7, 5}}));
  game.getCurrentPlayer()->getCurrentHero()->moveAlong(
      Path({FieldCoords{7, 5}}), MoveCosts({10}));
  EXPECT_EQ(game.getCurrentPlayer()->getCurrentHero()->getHeroCoords(),
            (FieldCoords{7, 4}));
}

TEST(dikstrjaTests, basicPathFind) {
  Game game = createExampleGame();
  ASSERT_EQ(game.getCurrentPlayer()->getCurrentHero()->getHeroCoords(),
            (FieldCoords{7, 3}));

  game.executeAction(FieldCoords{7, 6});
  Path expected_path({FieldCoords{7, 6}, FieldCoords{7, 5}, FieldCoords{7, 4}});
  MoveCosts example_costs({5, 5, 5});
  ASSERT_TRUE(game.getCurrentPlayer()->getCurrentHero()->moveAlong(
      expected_path, example_costs));
  EXPECT_EQ(game.getCurrentPlayer()->getCurrentHero()->getHeroCoords(),
            (FieldCoords{7, 6}));
  Path expected_path2({{3, 6}, {4, 6}, {5, 6}, {6, 6}});
  MoveCosts example_costs2({5, 5, 5, 5});
  game.executeAction(FieldCoords{3, 6});
  ASSERT_TRUE(game.getCurrentPlayer()->getCurrentHero()->moveAlong(
      expected_path2, example_costs2));
  EXPECT_EQ(game.getCurrentPlayer()->getCurrentHero()->getHeroCoords(),
            (FieldCoords{3, 6}));
}

TEST(dikstrjaTests, advancedPathFind) {
  Game game = createExampleGame();
  ASSERT_EQ(game.getCurrentPlayer()->getCurrentHero()->getHeroCoords(),
            (FieldCoords{7, 3}));
  game.executeAction(FieldCoords{10, 10});
  game.executeAction(FieldCoords{10, 10});
  EXPECT_EQ(game.getCurrentPlayer()->getCurrentHero()->getHeroCoords(),
            (FieldCoords{10, 10}));
  game.executeAction(FieldCoords{9, 15});
  Path expected_path({{9, 15}, {8, 14}, {8, 13}, {8, 12}, {8, 11}, {9, 10}});
  MoveCosts example_costs({0, 0, 0, 0, 0, 0});
  ASSERT_TRUE(game.getCurrentPlayer()->getCurrentHero()->moveAlong(
      expected_path, example_costs));
  EXPECT_EQ(game.getCurrentPlayer()->getCurrentHero()->getHeroCoords(),
            (FieldCoords{9, 15}));
  game.getCurrentPlayer()->getCurrentHero()->refillEnergy();
  game.executeAction(FieldCoords{10, 10});
  game.executeAction(FieldCoords{10, 10});
  game.executeAction(FieldCoords{10, 6});
  game.executeAction(FieldCoords{10, 6});
  ASSERT_EQ(game.getCurrentPlayer()->getCurrentHero()->getHeroCoords(),
            (FieldCoords{10, 7}));
  ASSERT_TRUE(game.getCurrentPlayer()->getCurrentHero()->moveAlong(
      Path({{10, 6}}), MoveCosts({{10}})));
  ASSERT_EQ(game.getCurrentPlayer()->getCurrentHero()->getHeroCoords(),
            (FieldCoords{10, 7}));
}

TEST(dikstrjaTests, impossiblePathFind) {
  Game game = createExampleGame();
  game.executeAction(FieldCoords{0, 0});
  game.executeAction(FieldCoords{0, 0});
  ASSERT_EQ(game.getCurrentPlayer()->getCurrentHero()->getHeroCoords(),
            (FieldCoords{7, 3}));
  ASSERT_EQ(game.getCurrentPlayer()->getCurrentHero()->getEnergy(), 100);
}