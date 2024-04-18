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

#include <memory>

using namespace std;

Map::Map(MapSize size) {
  width_ = static_cast<int>(size);
  field_array_ = vector<vector<Field>>(width_, vector<Field>(width_));
  for (int r = 0; r < width_; r++) {
    for (int c = 0; c < width_; c++) {
      field_array_[r][c] = Field(c, r);
    }
  }
}

Map::~Map() {}

Field* const Map::getField(FieldCoords coords) {
  int x = coords.first;
  int y = coords.second;
  if (x >= width_ || x < 0 || y >= width_ || y < 0) {
    return {};
  }
  return &field_array_[x][y];
}