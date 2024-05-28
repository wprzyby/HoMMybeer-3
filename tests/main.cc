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

#include <Config.h>
#include <Field.h>
#include <Game.h>
#include <Map.h>
#include <MapObject.h>
#include <MapUtils.h>
#include <Session.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <fstream>
#include <iostream>
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

TEST(mapUtilsTest, generateGrassMapTest) {
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

TEST(mapObjectTests, mountainObjectInitializationTest) {
  Config* conf = Config::getInstance();
  std::string path = Config::getProjectPath();
  conf->loadData(path + "/assets/ObjectsMetadata.json");
  GeologicalObject mountain =
      GeologicalObject(FieldCoords{0, 0}, GeologicalStructureType::MOUNTAIN, 0);

  ASSERT_EQ(mountain.occupiedFields().size(), 12);
  EXPECT_THAT(mountain.occupiedFields(),
              testing::UnorderedElementsAre(
                  FieldCoords{1, 2}, FieldCoords{1, 3}, FieldCoords{1, 4},
                  FieldCoords{1, 5}, FieldCoords{2, 1}, FieldCoords{2, 2},
                  FieldCoords{2, 3}, FieldCoords{2, 4}, FieldCoords{2, 5},
                  FieldCoords{3, 1}, FieldCoords{3, 2}, FieldCoords{3, 3}));

  GeologicalObject mountain2 =
      GeologicalObject(FieldCoords{2, 3}, GeologicalStructureType::MOUNTAIN, 0);
  EXPECT_THAT(mountain2.occupiedFields(),
              testing::UnorderedElementsAre(
                  FieldCoords{3, 5}, FieldCoords{3, 6}, FieldCoords{3, 7},
                  FieldCoords{3, 8}, FieldCoords{4, 4}, FieldCoords{4, 5},
                  FieldCoords{4, 6}, FieldCoords{4, 7}, FieldCoords{4, 8},
                  FieldCoords{5, 4}, FieldCoords{5, 5}, FieldCoords{5, 6}));
}

class basicTestGame : public testing::Test {
 protected:
  Session* sess;
  basicTestGame() { sess = Session::getInstance(); }
  void SetUp() override {
    Config* conf = Config::getInstance();
    std::string path = Config::getProjectPath();
    conf->loadData(path + "/assets/ObjectsMetadata.json");
    MapInfo map_info = generateGrassMap(MapSize::L, 1);
    std::vector<Player> players{
        Player(false, Faction::CASTLE, map_info.starting_locations[0])};
    sess->newGame(map_info.map, players, Difficulty::EASY);
  }
  void TearDown() override { sess = nullptr; }
};

TEST_F(basicTestGame, newGame) {
  EXPECT_NE(sess->game, nullptr);
  EXPECT_NE(sess->game->getPlayer(0), nullptr);
  EXPECT_EQ(sess->game->getPlayer(1), nullptr);
}

TEST_F(basicTestGame, addingAndDeletingGeologicalObject) {
  std::shared_ptr<GeologicalObject> geoObj = std::make_shared<GeologicalObject>(
      FieldCoords{0, 0}, GeologicalStructureType::MOUNTAIN, 0);
  sess->game->addMapObject(geoObj);
  EXPECT_TRUE(sess->game->getMap()->getField(FieldCoords{0, 0})->isWalkable());
  EXPECT_FALSE(sess->game->getMap()->getField(FieldCoords{1, 2})->isWalkable());
  EXPECT_FALSE(sess->game->getMap()->getField(FieldCoords{3, 3})->isWalkable());
  EXPECT_FALSE(sess->game->getMap()->getField(FieldCoords{2, 4})->isWalkable());
  EXPECT_EQ(sess->game->getMap()->getField(FieldCoords{0, 0})->getObject(),
            nullptr);
  EXPECT_EQ(sess->game->getMap()->getField(FieldCoords{2, 1})->getObject(),
            geoObj);
  EXPECT_EQ(sess->game->getMap()->getField(FieldCoords{3, 1})->getObject(),
            geoObj);
  EXPECT_EQ(sess->game->getMap()->getField(FieldCoords{2, 5})->getObject(),
            geoObj);
  sess->game->deleteMapObject(0);
  EXPECT_TRUE(sess->game->getMap()->getField(FieldCoords{0, 0})->isWalkable());
  EXPECT_TRUE(sess->game->getMap()->getField(FieldCoords{1, 2})->isWalkable());
  EXPECT_TRUE(sess->game->getMap()->getField(FieldCoords{3, 3})->isWalkable());
  EXPECT_TRUE(sess->game->getMap()->getField(FieldCoords{2, 4})->isWalkable());
  EXPECT_EQ(sess->game->getMap()->getField(FieldCoords{0, 0})->getObject(),
            nullptr);
  EXPECT_EQ(sess->game->getMap()->getField(FieldCoords{2, 1})->getObject(),
            nullptr);
  EXPECT_EQ(sess->game->getMap()->getField(FieldCoords{3, 1})->getObject(),
            nullptr);
  EXPECT_EQ(sess->game->getMap()->getField(FieldCoords{2, 5})->getObject(),
            nullptr);
}

TEST_F(basicTestGame, playerInitTest) {
  EXPECT_EQ(
      sess->game->getCurrentPlayer()->getResourceAmount(ResourceType::GOLD),
      2000);
  EXPECT_EQ(
      sess->game->getCurrentPlayer()->getResourceAmount(ResourceType::GEMS), 5);
  EXPECT_EQ(
      sess->game->getCurrentPlayer()->getResourceIncome(ResourceType::ORE), 1);
  EXPECT_EQ(
      sess->game->getCurrentPlayer()->getResourceIncome(ResourceType::WOOD), 1);
  sess->game->getCurrentPlayer()->updateResourceQuantity(ResourceType::SULFUR,
                                                         4);
  sess->game->getCurrentPlayer()->changeIncome(ResourceType::WOOD, 4);
  EXPECT_EQ(
      sess->game->getCurrentPlayer()->getResourceAmount(ResourceType::SULFUR),
      9);
  EXPECT_EQ(
      sess->game->getCurrentPlayer()->getResourceIncome(ResourceType::WOOD), 5);
  EXPECT_FALSE(sess->game->getCurrentPlayer()->updateResourceQuantity(
      ResourceType::ORE, -20));
  EXPECT_EQ(
      sess->game->getCurrentPlayer()->getResourceAmount(ResourceType::ORE), 10);
}

TEST_F(basicTestGame, pickableResourceTest) {
  std::shared_ptr<PickableResource> ore_stash =
      std::make_shared<PickableResource>(FieldCoords{4, 2}, ResourceType::ORE,
                                         5);
  sess->game->addMapObject(ore_stash);
  EXPECT_FALSE(sess->game->getMap()->getField(FieldCoords{4, 3})->isWalkable());
  EXPECT_TRUE(sess->game->getMap()
                  ->getField(FieldCoords{4, 3})
                  ->getObject()
                  ->objectAction());
  EXPECT_TRUE(sess->game->getMap()->getField(FieldCoords{4, 3})->isWalkable());
  EXPECT_EQ(
      sess->game->getCurrentPlayer()->getResourceAmount(ResourceType::ORE), 15);
}

TEST_F(basicTestGame, resourceGeneratorTest) {
  std::shared_ptr<ResourceGenerator> wood_workshop =
      std::make_shared<ResourceGenerator>(FieldCoords{3, 3}, ResourceType::WOOD,
                                          2);
  sess->game->addMapObject(wood_workshop);
  EXPECT_FALSE(sess->game->getMap()->getField(FieldCoords{5, 4})->isWalkable());
  EXPECT_FALSE(sess->game->getMap()->getField(FieldCoords{5, 7})->isWalkable());
  EXPECT_FALSE(sess->game->getMap()->getField(FieldCoords{4, 7})->isWalkable());
  EXPECT_EQ(sess->game->getMap()->getField(FieldCoords{4, 7})->getObject(),
            wood_workshop);
  EXPECT_EQ(sess->game->getMap()->getField(FieldCoords{5, 4})->getObject(),
            wood_workshop);
  EXPECT_TRUE(sess->game->getMap()
                  ->getField(FieldCoords{5, 7})
                  ->getObject()
                  ->objectAction());
  EXPECT_EQ(
      sess->game->getCurrentPlayer()->getResourceIncome(ResourceType::WOOD), 3);
}

TEST_F(basicTestGame, cityInit) {
  std::shared_ptr<City> castle_city =
      std::make_shared<City>(FieldCoords{5, 5}, Faction::CASTLE);
  sess->game->addMapObject(castle_city);
  EXPECT_FALSE(
      sess->game->getMap()->getField(FieldCoords{10, 6})->isWalkable());
  EXPECT_FALSE(sess->game->getMap()->getField(FieldCoords{8, 8})->isWalkable());
  EXPECT_FALSE(sess->game->getMap()->getField(FieldCoords{8, 9})->isWalkable());
  EXPECT_EQ(sess->game->getMap()->getField(FieldCoords{9, 10})->getObject(),
            castle_city);
  EXPECT_EQ(sess->game->getMap()->getField(FieldCoords{10, 8})->getObject(),
            castle_city);
}