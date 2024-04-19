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

Field* const Map::getField(FieldCoords coords) {
  int x = coords.first;
  int y = coords.second;
  if (x >= width_ || x < 0 || y >= width_ || y < 0) {
    return {};
  }
  return &field_array_[x][y];
}