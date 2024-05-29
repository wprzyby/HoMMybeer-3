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

std::optional<const Field*> Map::getField(FieldCoords coords) const {
  int x = coords.first;
  int y = coords.second;
  if (x >= width_ || x < 0 || y >= width_ || y < 0) {
    return {};
  }
  return &field_array_[x][y];
}

Field* Map::fieldToModify(FieldCoords coords) const {
  int x = coords.first;
  int y = coords.second;
  if (x >= width_ || x < 0 || y >= width_ || y < 0) {
    return {};
  }
  return const_cast<Field*>(&(field_array_.data()[x][y]));
}