/**
 * @file Map.cc
 * @author Piotr Kluba
 * @brief Functionality generating and managing the gamemap
 * @version 0.1
 * @date 2024-04-16
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <Map.h>

using namespace std;

Map::Map(MapSize size) {
  width_ = static_cast<int>(size);
  field_array_ = vector<vector<Field>>(width_, vector<Field>(width_));
}

Map::~Map() {}

optional<Field*> Map::getField(int x, int y) {
  if (x >= width_ || x < 0 || y >= width_ || y < 0) {
    return {};
  }
  return &field_array_[x][y];
}