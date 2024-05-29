/**
 * @file Field.h
 * @author Piotr Kluba
 * @brief Functionality responsible for storing properties and managing single
 * fields in the gamemap
 * @version 0.1
 * @date 2024-04-16
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef SRC_GAME_LOGIC_FIELD_H_
#define SRC_GAME_LOGIC_FIELD_H_

#include <Config.h>
#include <MapObject.h>

#include <memory>
#include <utility>


using FieldCoords = std::pair<int, int>;

FieldCoords operator+(const FieldCoords& l, const FieldCoords& r);

class Field {
 private:
  bool grail_here_;
  bool walk_through_;
  TerrainType terrain_type_;
  FieldCoords coords_;
  std::shared_ptr<MapObject> object_;

 public:
  explicit Field(int x = 0, int y = 0,
                 TerrainType terrain_type = TerrainType::GRASS,
                 bool grail = false, bool walk_through = true)
      : grail_here_(grail),
        walk_through_(walk_through),
        terrain_type_(terrain_type),
        coords_(FieldCoords{x, y}),
        object_(nullptr){};
  ~Field() = default;
  FieldCoords getFieldCoords() const { return coords_; };
  bool isWalkable() const { return walk_through_; };
  TerrainType getTerrainType() const { return terrain_type_; }
  const std::shared_ptr<MapObject> getObject() const { return object_; }
  bool setObject(std::shared_ptr<MapObject> object);
  bool deleteObject();
  static std::map<TerrainType, int> resistance; 
};

#endif