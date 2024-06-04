/**
 * @file Field.cc
 * @author Piotr Kluba
 * @brief Functionality responsible for storing properties and managing single
 * fields in the game map
 * @copyright Copyright (c) 2024
 */

#include <Field.h>

#include <utility>

FieldCoords operator+(const FieldCoords& left, const FieldCoords& right) {
  return {left.first + right.first, left.second + right.second};
}

const std::map<TerrainType, int> Field::RESISTANCE = {{TerrainType::GRASS, 10},
                                                      {TerrainType::DIRT, 10},
                                                      {TerrainType::SNOW, 20},
                                                      {TerrainType::STONE, 5}};

bool Field::setObject(std::shared_ptr<MapObject> object) {
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
  if (first.first == second.first && first.second == second.second) {
    return false;
  }

  return ((std::abs(first.first - second.first) == 1 ||
           std::abs(first.first - second.first) == 0) &&
          (std::abs(first.second - second.second) == 1 ||
           std::abs(first.second - second.second) == 0));
}