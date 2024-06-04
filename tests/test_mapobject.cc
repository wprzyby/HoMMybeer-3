/**
 * @file test_mapobject.cc
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2024-06-01
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <MapObject.h>
#include <common.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <testUtils.h>

TEST(mapObjectTests, mountainObjectInitializationTest) {
  Config* conf = Config::getInstance();
  std::string path = getProjectPath();
  conf->loadObjectsData(path + "/assets/ObjectsMetadata.json");
  GeologicalObject mountain = GeologicalObject(
      FieldCoords{0, 0}, nullptr, GeologicalStructureType::MOUNTAIN, 0);

  ASSERT_EQ(mountain.occupiedFields().size(), 12);
  EXPECT_THAT(mountain.occupiedFields(),
              testing::UnorderedElementsAre(
                  FieldCoords{2, 1}, FieldCoords{2, 2}, FieldCoords{2, 3},
                  FieldCoords{2, 4}, FieldCoords{3, 1}, FieldCoords{3, 2},
                  FieldCoords{3, 3}, FieldCoords{3, 4}, FieldCoords{3, 5},
                  FieldCoords{4, 3}, FieldCoords{4, 4}, FieldCoords{4, 5}));

  GeologicalObject mountain2 = GeologicalObject(
      FieldCoords{2, 3}, nullptr, GeologicalStructureType::MOUNTAIN, 0);
  EXPECT_THAT(mountain2.occupiedFields(),
              testing::UnorderedElementsAre(
                  FieldCoords{4, 4}, FieldCoords{5, 4}, FieldCoords{4, 5},
                  FieldCoords{5, 5}, FieldCoords{4, 6}, FieldCoords{5, 6},
                  FieldCoords{6, 6}, FieldCoords{4, 7}, FieldCoords{5, 7},
                  FieldCoords{6, 7}, FieldCoords{5, 8}, FieldCoords{6, 8}));
}

TEST(mapObjectInGameTests, addingAndDeletingGeologicalObject) {
  Game game = createBasicGame();
  std::shared_ptr<GeologicalObject> geoObj = std::make_shared<GeologicalObject>(
      FieldCoords{0, 0}, &game, GeologicalStructureType::MOUNTAIN, 0);
  game.addMapObject(geoObj);
  EXPECT_TRUE(game.getMap()->getField(FieldCoords{0, 0}).value()->isWalkable());
  EXPECT_FALSE(
      game.getMap()->getField(FieldCoords{2, 2}).value()->isWalkable());
  EXPECT_FALSE(
      game.getMap()->getField(FieldCoords{3, 3}).value()->isWalkable());
  EXPECT_FALSE(
      game.getMap()->getField(FieldCoords{2, 4}).value()->isWalkable());
  EXPECT_EQ(game.getMap()->getField(FieldCoords{0, 0}).value()->getObject(),
            nullptr);
  EXPECT_EQ(game.getMap()->getField(FieldCoords{4, 5}).value()->getObject(),
            geoObj);
  EXPECT_EQ(game.getMap()->getField(FieldCoords{3, 5}).value()->getObject(),
            geoObj);
  EXPECT_EQ(game.getMap()->getField(FieldCoords{2, 1}).value()->getObject(),
            geoObj);
  game.deleteMapObject(0);
  EXPECT_TRUE(game.getMap()->getField(FieldCoords{0, 0}).value()->isWalkable());
  EXPECT_TRUE(game.getMap()->getField(FieldCoords{2, 2}).value()->isWalkable());
  EXPECT_TRUE(game.getMap()->getField(FieldCoords{3, 3}).value()->isWalkable());
  EXPECT_TRUE(game.getMap()->getField(FieldCoords{2, 4}).value()->isWalkable());
  EXPECT_EQ(game.getMap()->getField(FieldCoords{0, 0}).value()->getObject(),
            nullptr);
  EXPECT_EQ(game.getMap()->getField(FieldCoords{4, 5}).value()->getObject(),
            nullptr);
  EXPECT_EQ(game.getMap()->getField(FieldCoords{3, 5}).value()->getObject(),
            nullptr);
  EXPECT_EQ(game.getMap()->getField(FieldCoords{2, 1}).value()->getObject(),
            nullptr);
}

TEST(mapObjectInGameTests, pickableResourceTest) {
  Game game = createBasicGame();
  std::shared_ptr<PickableResource> ore_stash =
      std::make_shared<PickableResource>(FieldCoords{4, 2}, &game,
                                         ResourceType::ORE, 5);
  game.addMapObject(ore_stash);
  EXPECT_FALSE(
      game.getMap()->getField(FieldCoords{4, 3}).value()->isWalkable());
  EXPECT_TRUE(game.getMap()
                  ->getField(FieldCoords{4, 3})
                  .value()
                  ->getObject()
                  ->objectAction());
  EXPECT_TRUE(game.getMap()->getField(FieldCoords{4, 3}).value()->isWalkable());
  EXPECT_EQ(game.getCurrentPlayer()->getResourceAmount(ResourceType::ORE), 15);
}

TEST(mapObjectInGameTests, resourceGeneratorTest) {
  Game game = createBasicGame();
  std::shared_ptr<ResourceGenerator> wood_workshop =
      std::make_shared<ResourceGenerator>(FieldCoords{3, 3}, nullptr,
                                          ResourceType::WOOD, 2);
  game.addMapObject(wood_workshop);
  EXPECT_FALSE(
      game.getMap()->getField(FieldCoords{5, 4}).value()->isWalkable());
  EXPECT_FALSE(
      game.getMap()->getField(FieldCoords{5, 7}).value()->isWalkable());
  EXPECT_FALSE(
      game.getMap()->getField(FieldCoords{4, 7}).value()->isWalkable());
  EXPECT_EQ(game.getMap()->getField(FieldCoords{4, 7}).value()->getObject(),
            wood_workshop);
  ASSERT_EQ(game.getMap()->getField(FieldCoords{5, 7}).value()->getObject(),
            wood_workshop);

  EXPECT_EQ(game.getMap()->getField(FieldCoords{5, 4}).value()->getObject(),
            wood_workshop);
  EXPECT_TRUE(game.getMap()
                  ->getField(FieldCoords{5, 7})
                  .value()
                  ->getObject()
                  ->objectAction());
  EXPECT_EQ(game.getCurrentPlayer()->getResourceIncome(ResourceType::WOOD), 3);
}

TEST(mapObjectInGameTests, cityInit) {
  Game game = createBasicGame();
  std::shared_ptr<City> castle_city =
      std::make_shared<City>(FieldCoords{5, 5}, &game, Faction::CASTLE);
  game.addMapObject(castle_city);
  EXPECT_FALSE(
      game.getMap()->getField(FieldCoords{10, 6}).value()->isWalkable());
  EXPECT_FALSE(
      game.getMap()->getField(FieldCoords{8, 8}).value()->isWalkable());
  EXPECT_FALSE(
      game.getMap()->getField(FieldCoords{8, 9}).value()->isWalkable());
  EXPECT_EQ(game.getMap()->getField(FieldCoords{9, 10}).value()->getObject(),
            castle_city);
  EXPECT_EQ(game.getMap()->getField(FieldCoords{10, 8}).value()->getObject(),
            castle_city);
}