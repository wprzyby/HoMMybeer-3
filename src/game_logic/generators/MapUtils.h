/**
 * @file MapUtils.h
 * @author Wojciech Przybylski
 * @brief Utilities for map creation and manipulation
 * @copyright Copyright (c) 2024
 */

#ifndef SRC_GAME_LOGIC_MAP_UTILS_H_
#define SRC_GAME_LOGIC_MAP_UTILS_H_

#include <Map.h>

enum class MapSize { S = 500, M = 800, L = 1200, XL = 2000 };

struct MapInfo {
  Map map;
  int num_of_players;
  std::vector<FieldCoords> starting_locations;
};

MapInfo generateGrassMap(MapSize map_size, int num_of_players);

#endif  // SRC_GAME_LOGIC_MAP_UTILS_H_