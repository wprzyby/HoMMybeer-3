/**
 * @file test_game.cc
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2024-06-01
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <Game.h>
#include <MapUtils.h>
#include <common.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <testUtils.h>

TEST(gameInitTests, newGame) {
  Game game = createBasicGame();
  EXPECT_NE(game.getPlayer(0), nullptr);
  EXPECT_EQ(game.getPlayer(1), nullptr);
}

TEST(gameInitTests, playerInitTest) {
  Game game = createBasicGame();
  EXPECT_EQ(game.getCurrentPlayer()->getResourceAmount(ResourceType::GOLD),
            2000);
  EXPECT_EQ(game.getCurrentPlayer()->getResourceAmount(ResourceType::GEMS), 5);
  EXPECT_EQ(game.getCurrentPlayer()->getResourceIncome(ResourceType::ORE), 1);
  EXPECT_EQ(game.getCurrentPlayer()->getResourceIncome(ResourceType::WOOD), 1);
  game.getCurrentPlayer()->updateResourceQuantity(ResourceType::SULFUR, 4);
  game.getCurrentPlayer()->changeIncome(ResourceType::WOOD, 4);
  EXPECT_EQ(game.getCurrentPlayer()->getResourceAmount(ResourceType::SULFUR),
            9);
  EXPECT_EQ(game.getCurrentPlayer()->getResourceIncome(ResourceType::WOOD), 5);
  EXPECT_FALSE(
      game.getCurrentPlayer()->updateResourceQuantity(ResourceType::ORE, -20));
  EXPECT_EQ(game.getCurrentPlayer()->getResourceAmount(ResourceType::ORE), 10);
  game.getCurrentPlayer()->addHero("Random Random", FieldCoords{3, 3});
  game.getCurrentPlayer()->nextHero();
  EXPECT_EQ(game.getCurrentPlayer()->getCurrentHero(),
            game.getCurrentPlayer()->getHero(1));
  game.getCurrentPlayer()->nextHero();
  EXPECT_EQ(game.getCurrentPlayer()->getCurrentHero(),
            game.getCurrentPlayer()->getHero(0));
}

TEST(gameInitTests, conflictingObjectsAtInit) {
  Config* conf = Config::getInstance();
  std::string path = getProjectPath();
  conf->loadData(path + "/assets/ObjectsMetadata.json");
  MapInfo map_info = generateGrassMap(MapSize::L, 1);
  std::vector<Player> players{
      Player(false, Faction::CASTLE, map_info.starting_locations[0])};
  std::vector<std::shared_ptr<MapObject>> objects = {
      std::make_shared<GeologicalObject>(FieldCoords{0, 0}, nullptr,
                                         GeologicalStructureType::TREE, 7),
      std::make_shared<GeologicalObject>(FieldCoords{0, 0}, nullptr,
                                         GeologicalStructureType::TREE, 7)};
  EXPECT_THROW(Game(players, map_info.map, objects),
               Game::WrongObjectPlacementException);
}

TEST(gameModificationsTests, conflictingObjectsAtAdding) {
  Config* conf = Config::getInstance();
  std::string path = getProjectPath();
  conf->loadData(path + "/assets/ObjectsMetadata.json");
  MapInfo map_info = generateGrassMap(MapSize::L, 1);
  std::vector<Player> players{
      Player(false, Faction::CASTLE, map_info.starting_locations[0])};
  std::vector<std::shared_ptr<MapObject>> objects = {
      std::make_shared<GeologicalObject>(FieldCoords{0, 0}, nullptr,
                                         GeologicalStructureType::TREE, 7)};
  Game game = Game(players, map_info.map, objects);
  EXPECT_TRUE(game.addMapObject(std::make_shared<PickableResource>(
      FieldCoords{6, 6}, nullptr, ResourceType::GOLD, 50)));
  EXPECT_NE(game.getMap()->getField(FieldCoords{6, 7}).value()->getObject(),
            nullptr);
  EXPECT_FALSE(game.addMapObject(std::make_shared<GeologicalObject>(
      FieldCoords{0, 0}, nullptr, GeologicalStructureType::TREE, 7)));
  EXPECT_FALSE(game.addMapObject(std::make_shared<GeologicalObject>(
      FieldCoords{2, 2}, nullptr, GeologicalStructureType::TREE, 5)));
  EXPECT_EQ(game.getMap()->getField(FieldCoords{4, 4}).value()->getObject(),
            nullptr);
}

TEST(gameIntegrationTests, testActionSequence) {
  Game game = createExampleGame();
  ASSERT_EQ(game.getCurrentPlayer()->getCurrentHero()->getHeroCoords(),
            (FieldCoords{7, 3}));
  ASSERT_EQ(game.getCurrentPlayer()->getResourceAmount(ResourceType::CRYSTAL),
            5);
  game.executeAction(FieldCoords{5, 0});
  ASSERT_EQ(game.getCurrentPlayer()->getResourceAmount(ResourceType::CRYSTAL),
            5);
  ASSERT_EQ(game.getCurrentPlayer()->getCurrentHero()->getHeroCoords(),
            (FieldCoords{7, 3}));
  game.executeAction(FieldCoords{6, 0});
  game.executeAction(FieldCoords{6, 0});
  game.executeAction(FieldCoords{5, 0});
  ASSERT_EQ(game.getCurrentPlayer()->getResourceAmount(ResourceType::CRYSTAL),
            10);
  ASSERT_TRUE(game.getMap()->getField(FieldCoords{5, 0}).value()->isWalkable());
  game.getCurrentPlayer()->getCurrentHero()->refillEnergy();
  game.executeAction(FieldCoords{0, 0});
  game.executeAction(FieldCoords{0, 0});
  ASSERT_EQ(game.getCurrentPlayer()->getCurrentHero()->getHeroCoords(),
            (FieldCoords{0, 0}));
}