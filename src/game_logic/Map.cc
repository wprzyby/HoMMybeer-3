/**
 * @file Map.cc
 * @author Piotr Kluba
 * @brief
 * @version 0.1
 * @date 2024-04-16
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <Map.h>

using namespace std;

Map::Map(MapSize size) {
  switch (size) {
    case MapSize::S:
      width_ = 500;
      break;
    case MapSize::M:
      width_ = 800;
      break;
    case MapSize::L:
      width_ = 1200;
      break;
    case MapSize::XL:
      width_ = 2000;
      break;
    default:
      width_ = 0;
      break;
  }

  field_array_ = vector<vector<Field>>(width_, vector<Field>(width_));
}

Map::~Map() {}

Field* Map::getField(int x, int y) {
  if (x >= width_ || x < 0 || y >= width_ || y < 0) {
    throw Map::CoordinatesOutOfMap(x, y);
  }
  return &field_array_[x][y];
}