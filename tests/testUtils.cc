/**
 * @file testUtils.cc
 * @author Piotr Kluba
 * @brief Utilities used in testing
 * @copyright Copyright (c) 2024
 */

#include "testUtils.h"

#include <MapObject.h>
#include <Player.h>
#include <game_logic_utils.h>
#include <map_generation.h>

#include <iostream>
#include <string>

Game createBasicGame() {
  std::string path = getProjectPath();
  Config::getInstance()->loadObjectsData(path +
                                         "/metadata/ObjectsMetadata.json");
  Config::getInstance()->loadUnitConfig(path + "/metadata/UnitConfig.json");

  MapInfo map_info = generateGrassMap(MapSize::L, 1);
  std::vector<Player> players{
      Player(false, Faction::CASTLE, FieldCoords{1, 1})};
  return Game(players, map_info.map);
}

Game createExampleGame() {
  std::string path = getProjectPath();
  Config::getInstance()->loadUnitConfig(path + "/metadata/UnitConfig.json");
  Config::getInstance()->loadObjectsData(path +
                                         "/metadata/ObjectsMetadata.json");
  MapInfo map_info = generateLargeExampleMap();
  std::vector<Player> players{
      Player(false, Faction::CASTLE, map_info.starting_locations[0])};
  std::vector<std::shared_ptr<MapObject>> static_map_objects =
      generateExampleStaticObjects();
  std::vector<std::shared_ptr<MapObject>> pickable_map_objects =
      generateExamplePickableObjects();
  std::vector<std::shared_ptr<MapObject>> starting_objects(
      static_map_objects.size() + pickable_map_objects.size());
  std::merge(static_map_objects.begin(), static_map_objects.end(),
             pickable_map_objects.begin(), pickable_map_objects.end(),
             starting_objects.begin());

  try {
    Game game(players, map_info.map, starting_objects);
  } catch (Game::WrongObjectPlacementException& e) {
    std::cerr << e.what() << '\n';
  }
  return Game(players, map_info.map, starting_objects);
}