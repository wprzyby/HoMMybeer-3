/**
 * @file Map.h
 * @author Piotr Kluba
 * @brief Functionality generating and managing the gamemap
 * @version 0.1
 * @date 2024-04-16
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef SRC_GAME_LOGIC_MAP_H_
#define SRC_GAME_LOGIC_MAP_H_

#include <Field.h>

#include <optional>
#include <vector>

using FieldArray_t = std::vector<std::vector<Field>>;
using FieldCoords = std::pair<int, int>;

class Map {
 private:
  int width_;
  FieldArray_t field_array_;
  std::vector<FieldCoords> starting_locations_;

 public:
  explicit Map(FieldArray_t fields) : field_array_(fields) {
    width_ = fields.size();
  }
  ~Map() = default;
  Field* const getField(
      FieldCoords coords);  // method for getting visual field parameters
  const int getWidth() { return width_; };
};

#endif
