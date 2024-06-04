/**
 * @file Field.cc
 * @author Piotr Kluba
 * @brief Functionality responsible for storing properties and managing single
 * fields in the gamemap
 * @version 0.1
 * @date 2024-04-16
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <Field.h>

#include <utility>
using namespace std;

FieldCoords operator+(const FieldCoords& l, const FieldCoords& r) {
  return {l.first + r.first, l.second + r.second};
}

map<TerrainType, int> Field::resistance = {
  {TerrainType::GRASS, 10},
  {TerrainType::DIRT, 10},
  {TerrainType::SNOW, 20},
  {TerrainType::STONE, 5}
};

bool Field::setObject(shared_ptr<MapObject> object) {
  if (object_ != nullptr) {
    return false;
  }
  object_ = std::move(object);
  walk_through_ = false;
  return true;
}

bool Field::deleteObject() {
  if (object_ == nullptr) {
    return false;
  }
  object_ = nullptr;
  walk_through_ = true;
  return true;
}

bool Field::isAdjacent(FieldCoords first, FieldCoords second) {
  return (std::abs(first.first - second.first) == 1 || std::abs(first.second-second.second) == 1);
}