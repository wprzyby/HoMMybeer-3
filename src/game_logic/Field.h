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

#include <utility>

typedef std::pair<int, int> FieldCoords;

enum class TerrainType { SNOW, GRASS, STONE, DIRT, STRUCTURE };

class Field {
 private:
  bool grail_here_;
  bool walk_through_;
  TerrainType terrain_type_;
  std::pair<int, int> coords_;

 public:
  Field(int x = 0, int y = 0, TerrainType terrain_type = TerrainType::GRASS,
        bool grail = false, bool walk_through = true)
      : grail_here_(grail),
        walk_through_(walk_through),
        terrain_type_(terrain_type),
        coords_(std::pair<int, int>{x, y}){};
  ~Field();
  const std::pair<int, int> getFieldCoords() { return coords_; };
  const bool isWalkable() { return walk_through_; };
};

#endif