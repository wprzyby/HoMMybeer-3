/**
 * @file Map.h
 * @author Piotr Kluba
 * @brief Functionality generating and managing the game map
 * @copyright Copyright (c) 2024
 */

#ifndef SRC_GAME_LOGIC_MAP_H
#define SRC_GAME_LOGIC_MAP_H

#include <Config.h>
#include <Field.h>

#include <optional>
#include <set>
#include <utility>
#include <vector>

using FieldArray = std::vector<std::vector<Field>>;
using FieldCoords = std::pair<int, int>;

class Map {
 private:
  unsigned int width_;
  FieldArray field_array_;
  std::vector<FieldCoords> starting_locations_;
  [[nodiscard]] Field* fieldToModify(FieldCoords coords);
  [[nodiscard]] std::set<FieldCoords> getViableAdjacent(
      FieldCoords coords) const;

 public:
  explicit Map(const FieldArray& fields)
      : width_(fields.size() & 0xFFFFFFFF), field_array_(fields) {}
  [[nodiscard]] std::optional<const Field*> getField(
      FieldCoords coords) const;  // method for getting visual field parameters
  [[nodiscard]] FieldArray getFieldArray() const { return field_array_; }
  [[nodiscard]] unsigned int getWidth() const { return width_; };
  bool deleteObjectFrom(FieldCoords coords_to_delete) {
    return fieldToModify(coords_to_delete)->deleteObject();
  }
  bool setObjectTo(FieldCoords coords_to_delete,
                   std::shared_ptr<MapObject> obj) {
    return fieldToModify(coords_to_delete)->setObject(std::move(obj));
  }
  [[nodiscard]] std::set<FieldCoords> constructGraph(
      FieldCoords seed_coords) const;
};

#endif // SRC_GAME_LOGIC_MAP_H
