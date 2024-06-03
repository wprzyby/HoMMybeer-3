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

#include <Config.h>
#include <Field.h>

#include <optional>
#include <set>
#include <vector>

using FieldArray_t = std::vector<std::vector<Field>>;
using FieldCoords = std::pair<int, int>;

class Map {
 private:
  unsigned int width_;
  FieldArray_t field_array_;
  std::vector<FieldCoords> starting_locations_;
  Field* fieldToModify(FieldCoords coords) const;
  std::set<FieldCoords> getViableAdjacent_(FieldCoords coords) const;

 public:
  explicit Map(FieldArray_t fields) : field_array_(fields) {
    width_ = fields.size() &
             0xFFFFFFFF;  // due to conversion warning; we don't expect the size
                          // to be more than the range of uint
  }
  ~Map() = default;
  std::optional<const Field*> getField(
      FieldCoords coords) const;  // method for getting visual field parameters
  FieldArray_t getFieldArray() const { return field_array_; }
  unsigned int getWidth() const { return width_; };
  bool deleteObjectFrom(FieldCoords coords_to_delete) const {
    return fieldToModify(coords_to_delete)->deleteObject();
  }
  bool setObjectTo(FieldCoords coords_to_delete,
                   std::shared_ptr<MapObject> obj) const {
    return fieldToModify(coords_to_delete)->setObject(obj);
  }
  std::set<FieldCoords> constructGraph(FieldCoords seed_coords) const;
};

#endif
