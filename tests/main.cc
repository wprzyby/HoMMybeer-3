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

#include <iostream>

TEST(sessionTests, allowsOneInstance) {
  Session* sess1 = Session::getInstance();
  Session* sess2 = Session::getInstance();
  EXPECT_EQ(sess1, sess2);
}

TEST(mapUtilsTest, generateGrassMapTest) {
  MapInfo map_info = generateGrassMap(MapSize::S, 3);
  EXPECT_EQ(map_info.map.getWidth(), static_cast<int>(MapSize::S));
  EXPECT_EQ(map_info.num_of_players, 3);
  EXPECT_EQ(map_info.starting_locations.size(), 3);
  EXPECT_EQ(map_info.map.getField(FieldCoords{1, 1}).value()->isWalkable(),
            true);
}

TEST(mapTests, basicMapFunctionality) {
  MapInfo map_i = generateGrassMap(MapSize::S, 3);
  Map map = map_i.map;
  EXPECT_TRUE(map.getField(FieldCoords{0, 0}).has_value());
  EXPECT_FALSE(map.getField(FieldCoords{static_cast<int>(MapSize::S) + 1,
                                        static_cast<int>(MapSize::S) + 1})
                   .has_value());
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
                  FieldCoords{2, 1}, FieldCoords{2, 2}, FieldCoords{2, 3},
                  FieldCoords{2, 4}, FieldCoords{3, 1}, FieldCoords{3, 2},
                  FieldCoords{3, 3}, FieldCoords{3, 4}, FieldCoords{3, 5},
                  FieldCoords{4, 3}, FieldCoords{4, 4}, FieldCoords{4, 5}));

  GeologicalObject mountain2 =
      GeologicalObject(FieldCoords{2, 3}, GeologicalStructureType::MOUNTAIN, 0);
  EXPECT_THAT(mountain2.occupiedFields(),
              testing::UnorderedElementsAre(
                  FieldCoords{4, 4}, FieldCoords{5, 4}, FieldCoords{4, 5},
                  FieldCoords{5, 5}, FieldCoords{4, 6}, FieldCoords{5, 6},
                  FieldCoords{6, 6}, FieldCoords{4, 7}, FieldCoords{5, 7},
                  FieldCoords{6, 7}, FieldCoords{5, 8}, FieldCoords{6, 8}));
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
  EXPECT_NE(sess->game.getPlayer(0), nullptr);
  EXPECT_EQ(sess->game.getPlayer(1), nullptr);
}

TEST_F(basicTestGame, addingAndDeletingGeologicalObject) {
  GeologicalObject geoObj =
      GeologicalObject(FieldCoords{0, 0}, GeologicalStructureType::MOUNTAIN, 0);
  sess->game.addMapObject(geoObj);
  EXPECT_TRUE(
      sess->game.getMap()->getField(FieldCoords{0, 0}).value()->isWalkable());
  EXPECT_FALSE(
      sess->game.getMap()->getField(FieldCoords{2, 2}).value()->isWalkable());
  EXPECT_FALSE(
      sess->game.getMap()->getField(FieldCoords{3, 3}).value()->isWalkable());
  EXPECT_FALSE(
      sess->game.getMap()->getField(FieldCoords{2, 4}).value()->isWalkable());
  EXPECT_EQ(
      sess->game.getMap()->getField(FieldCoords{0, 0}).value()->getObject(),
      nullptr);
  EXPECT_EQ(std::get<GeologicalObject>(*sess->game.getMap()
                                            ->getField(FieldCoords{4, 5})
                                            .value()
                                            ->getObject())
                .getId(),
            geoObj.getId());
  EXPECT_EQ(std::get<GeologicalObject>(*sess->game.getMap()
                                            ->getField(FieldCoords{3, 5})
                                            .value()
                                            ->getObject())
                .getId(),
            geoObj.getId());
  EXPECT_EQ(std::get<GeologicalObject>(*sess->game.getMap()
                                            ->getField(FieldCoords{2, 1})
                                            .value()
                                            ->getObject())
                .getId(),
            geoObj.getId());
  sess->game.deleteMapObject(0);
  EXPECT_TRUE(
      sess->game.getMap()->getField(FieldCoords{0, 0}).value()->isWalkable());
  EXPECT_TRUE(
      sess->game.getMap()->getField(FieldCoords{2, 2}).value()->isWalkable());
  EXPECT_TRUE(
      sess->game.getMap()->getField(FieldCoords{3, 3}).value()->isWalkable());
  EXPECT_TRUE(
      sess->game.getMap()->getField(FieldCoords{2, 4}).value()->isWalkable());
  EXPECT_EQ(
      sess->game.getMap()->getField(FieldCoords{0, 0}).value()->getObject(),
      nullptr);
  EXPECT_EQ(
      sess->game.getMap()->getField(FieldCoords{4, 5}).value()->getObject(),
      nullptr);
  EXPECT_EQ(
      sess->game.getMap()->getField(FieldCoords{3, 5}).value()->getObject(),
      nullptr);
  EXPECT_EQ(
      sess->game.getMap()->getField(FieldCoords{2, 1}).value()->getObject(),
      nullptr);
}

TEST_F(basicTestGame, playerInitTest) {
  EXPECT_EQ(
      sess->game.getCurrentPlayer()->getResourceAmount(ResourceType::GOLD),
      2000);
  EXPECT_EQ(
      sess->game.getCurrentPlayer()->getResourceAmount(ResourceType::GEMS), 5);
  EXPECT_EQ(sess->game.getCurrentPlayer()->getResourceIncome(ResourceType::ORE),
            1);
  EXPECT_EQ(
      sess->game.getCurrentPlayer()->getResourceIncome(ResourceType::WOOD), 1);
  sess->game.getCurrentPlayer()->updateResourceQuantity(ResourceType::SULFUR,
                                                        4);
  sess->game.getCurrentPlayer()->changeIncome(ResourceType::WOOD, 4);
  EXPECT_EQ(
      sess->game.getCurrentPlayer()->getResourceAmount(ResourceType::SULFUR),
      9);
  EXPECT_EQ(
      sess->game.getCurrentPlayer()->getResourceIncome(ResourceType::WOOD), 5);
  EXPECT_FALSE(sess->game.getCurrentPlayer()->updateResourceQuantity(
      ResourceType::ORE, -20));
  EXPECT_EQ(sess->game.getCurrentPlayer()->getResourceAmount(ResourceType::ORE),
            10);
}

TEST_F(basicTestGame, pickableResourceTest) {
  PickableResource ore_stash =
      PickableResource(FieldCoords{4, 2}, ResourceType::ORE, 5);
  sess->game.addMapObject(ore_stash);
  EXPECT_FALSE(
      sess->game.getMap()->getField(FieldCoords{4, 3}).value()->isWalkable());
  sess->game.executeAction(FieldCoords{4, 3});
  EXPECT_TRUE(
      sess->game.getMap()->getField(FieldCoords{4, 3}).value()->isWalkable());
  EXPECT_EQ(sess->game.getCurrentPlayer()->getResourceAmount(ResourceType::ORE),
            15);
}

TEST_F(basicTestGame, resourceGeneratorTest) {
  ResourceGenerator wood_workshop =
      ResourceGenerator(FieldCoords{3, 3}, ResourceType::WOOD, 2);
  sess->game.addMapObject(wood_workshop);
  EXPECT_FALSE(
      sess->game.getMap()->getField(FieldCoords{5, 4}).value()->isWalkable());
  EXPECT_FALSE(
      sess->game.getMap()->getField(FieldCoords{5, 7}).value()->isWalkable());
  EXPECT_FALSE(
      sess->game.getMap()->getField(FieldCoords{4, 7}).value()->isWalkable());
  EXPECT_EQ(std::get<ResourceGenerator>(*sess->game.getMap()
                                             ->getField(FieldCoords{4, 7})
                                             .value()
                                             ->getObject())
                .getId(),
            wood_workshop.getId());
  EXPECT_EQ(std::get<ResourceGenerator>(*sess->game.getMap()
                                             ->getField(FieldCoords{5, 4})
                                             .value()
                                             ->getObject())
                .getId(),
            wood_workshop.getId());
  sess->game.executeAction(FieldCoords{5, 7});
  EXPECT_EQ(
      sess->game.getCurrentPlayer()->getResourceIncome(ResourceType::WOOD), 3);
}

TEST_F(basicTestGame, cityInit) {
  City castle_city = City(FieldCoords{5, 5}, Faction::CASTLE);
  sess->game.addMapObject(castle_city);
  EXPECT_FALSE(
      sess->game.getMap()->getField(FieldCoords{10, 6}).value()->isWalkable());
  EXPECT_FALSE(
      sess->game.getMap()->getField(FieldCoords{8, 8}).value()->isWalkable());
  EXPECT_FALSE(
      sess->game.getMap()->getField(FieldCoords{8, 9}).value()->isWalkable());
  EXPECT_EQ(std::get<City>(*sess->game.getMap()
                                ->getField(FieldCoords{9, 10})
                                .value()
                                ->getObject())
                .getId(),
            castle_city.getId());
  EXPECT_EQ(std::get<City>(*sess->game.getMap()
                                ->getField(FieldCoords{10, 8})
                                .value()
                                ->getObject())
                .getId(),
            castle_city.getId());
}

TEST(mapObjectTests, conflictingObjectsAtInit) {
  Config* conf = Config::getInstance();
  std::string path = Config::getProjectPath();
  conf->loadData(path + "/assets/ObjectsMetadata.json");
  MapInfo map_info = generateGrassMap(MapSize::L, 1);
  std::vector<Player> players{
      Player(false, Faction::CASTLE, map_info.starting_locations[0])};
  std::vector<MapObject> objects = {
      GeologicalObject(FieldCoords{0, 0}, GeologicalStructureType::TREE, 7),
      GeologicalObject(FieldCoords{0, 0}, GeologicalStructureType::TREE, 7)};
  Session* sess = Session::getInstance();
  EXPECT_THROW(sess->newGame(map_info.map, players, Difficulty::EASY, objects),
               Game::WrongObjectPlacementException);
}

TEST(mapObjectTests, conflictingObjectsAtAdding) {
  Config* conf = Config::getInstance();
  std::string path = Config::getProjectPath();
  conf->loadData(path + "/assets/ObjectsMetadata.json");
  MapInfo map_info = generateGrassMap(MapSize::L, 1);
  std::vector<Player> players{
      Player(false, Faction::CASTLE, map_info.starting_locations[0])};
  std::vector<MapObject> objects = {
      GeologicalObject(FieldCoords{0, 0}, GeologicalStructureType::TREE, 7)};
  Session* sess = Session::getInstance();
  sess->newGame(map_info.map, players, Difficulty::EASY, objects);
  EXPECT_TRUE(sess->game.addMapObject(
      PickableResource(FieldCoords{6, 6}, ResourceType::GOLD, 50)));
  EXPECT_NE(
      sess->game.getMap()->getField(FieldCoords{6, 7}).value()->getObject(),
      nullptr);
  EXPECT_FALSE(sess->game.addMapObject(
      GeologicalObject(FieldCoords{0, 0}, GeologicalStructureType::TREE, 7)));
  EXPECT_FALSE(sess->game.addMapObject(
      GeologicalObject(FieldCoords{2, 2}, GeologicalStructureType::TREE, 5)));
  EXPECT_EQ(
      sess->game.getMap()->getField(FieldCoords{4, 4}).value()->getObject(),
      nullptr);
}

class TestMapTests : public testing::Test {
 protected:
  Session* sess;
  TestMapTests() { sess = Session::getInstance(); }
  void SetUp() override {
    Config* conf = Config::getInstance();
    std::string path = Config::getProjectPath();
    conf->loadData(path + "/assets/ObjectsMetadata.json");
    MapInfo map_info = generateLargeExampleMap();
    std::vector<Player> players{
        Player(false, Faction::CASTLE, map_info.starting_locations[0])};
    std::vector<MapObject> starting_objects = generateExampleStaticObjects();
    std::vector<MapObject> pickable_map_objects =
        generateExamplePickableObjects();
    starting_objects.insert(starting_objects.end(),
                            pickable_map_objects.begin(),
                            pickable_map_objects.end());

    try {
      sess->newGame(map_info.map, players, Difficulty::EASY, starting_objects);
    } catch (Game::WrongObjectPlacementException& e) {
      std::cerr << e.what() << '\n';
    }
  }
  void TearDown() override { sess = nullptr; }
};

TEST_F(TestMapTests, exampleMapInitTest) {
  EXPECT_FALSE(
      sess->game.getMap()->getField(FieldCoords{10, 12}).value()->isWalkable());
  EXPECT_FALSE(
      sess->game.getMap()->getField(FieldCoords{21, 26}).value()->isWalkable());
  EXPECT_FALSE(
      sess->game.getMap()->getField(FieldCoords{4, 28}).value()->isWalkable());
  EXPECT_FALSE(
      sess->game.getMap()->getField(FieldCoords{36, 13}).value()->isWalkable());
  EXPECT_FALSE(
      sess->game.getMap()->getField(FieldCoords{22, 29}).value()->isWalkable());
  EXPECT_FALSE(
      sess->game.getMap()->getField(FieldCoords{33, 38}).value()->isWalkable());
  EXPECT_FALSE(
      sess->game.getMap()->getField(FieldCoords{41, 48}).value()->isWalkable());
  EXPECT_FALSE(
      sess->game.getMap()->getField(FieldCoords{41, 39}).value()->isWalkable());
  EXPECT_TRUE(
      sess->game.getMap()->getField(FieldCoords{36, 27}).value()->isWalkable());
  EXPECT_TRUE(
      sess->game.getMap()->getField(FieldCoords{46, 40}).value()->isWalkable());
  EXPECT_TRUE(
      sess->game.getMap()->getField(FieldCoords{21, 43}).value()->isWalkable());
  EXPECT_TRUE(
      sess->game.getMap()->getField(FieldCoords{22, 27}).value()->isWalkable());
  EXPECT_TRUE(
      sess->game.getMap()->getField(FieldCoords{11, 2}).value()->isWalkable());
  EXPECT_TRUE(
      sess->game.getMap()->getField(FieldCoords{10, 17}).value()->isWalkable());
  EXPECT_TRUE(
      sess->game.getMap()->getField(FieldCoords{20, 26}).value()->isWalkable());
}