/**
 * @file MapUtils.h
 * @author Wojciech Przybylski
 * @brief Utilities for map creation and manipulation
 * @copyright Copyright (c) 2024
 */

#ifndef SRC_GAME_LOGIC_MAP_UTILS_H_
#define SRC_GAME_LOGIC_MAP_UTILS_H_

#include <Map.h>
#include <MapObject.h>

#include <memory>
#include <vector>

enum class MapSize { S = 20, M = 35, L = 50, XL = 100 };

struct MapInfo {
  Map map;
  int num_of_players;
  std::vector<FieldCoords> starting_locations;
};

MapInfo generateGrassMap(MapSize map_size, int num_of_players);

MapInfo generateLargeExampleMap();

std::vector<std::shared_ptr<MapObject>> generateExampleStaticObjects();
std::vector<std::shared_ptr<MapObject>> generateExamplePickableObjects();

#endif  // SRC_GAME_LOGIC_MAP_UTILS_H_