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
  int width = 0;
  switch (size) {
    case MapSize::S:
      width = 500;
      break;
    case MapSize::M:
      width = 800;
      break;
    case MapSize::L:
      width = 1200;
      break;
    case MapSize::XL:
      width = 2000;
      break;
    default:
      width = 0;
      break;
  }

  field_array_ = vector<vector<Field>>(width, vector<Field>(width));
}

Map::~Map() {}

Field* Map::getField(int x, int y) {
  // TODO: create size restrictions w. exception throwing
  return &field_array_[x][y];
}