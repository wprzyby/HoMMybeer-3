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
using namespace std;

FieldCoords operator+(const FieldCoords& l, const FieldCoords& r) {
  return {l.first + r.first, l.second + r.second};
}

bool Field::setObject(shared_ptr<MapObject> object) {
  if (object_ != nullptr) {
    return false;
  }
  object_ = object;
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