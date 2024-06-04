/**
 * @file Field.h
 * @author Piotr Kluba
 * @brief Functionality responsible for storing properties and managing single
 * fields in the game map
 * @copyright Copyright (c) 2024
 */

#ifndef SRC_GAME_LOGIC_FIELD_H
#define SRC_GAME_LOGIC_FIELD_H

#include <Config.h>
#include <MapObject.h>

#include <memory>
#include <utility>

using FieldCoords = std::pair<int, int>;

FieldCoords operator+(const FieldCoords& left, const FieldCoords& right);

class Field {
 private:
  bool grail_here_;
  bool walk_through_;
  TerrainType terrain_type_;
  FieldCoords coords_;
  std::shared_ptr<MapObject> object_;

 public:
  const static std::map<TerrainType, int> RESISTANCE;
  explicit Field(int x_coord = 0, int y_coord = 0,
                 TerrainType terrain_type = TerrainType::GRASS,
                 bool grail = false, bool walk_through = true)
      : grail_here_(grail),
        walk_through_(walk_through),
        terrain_type_(terrain_type),
        coords_(FieldCoords{x_coord, y_coord}),
        object_(nullptr){};
  [[nodiscard]] FieldCoords getFieldCoords() const { return coords_; };
  [[nodiscard]] bool isWalkable() const { return walk_through_; };
  [[nodiscard]] TerrainType getTerrainType() const { return terrain_type_; }
  [[nodiscard]] std::shared_ptr<MapObject> getObject() const { return object_; }
  bool setObject(std::shared_ptr<MapObject> object);
  bool deleteObject();

  static bool isAdjacent(FieldCoords first, FieldCoords second);
};

#endif // SRC_GAME_LOGIC_FIELD_H