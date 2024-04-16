/**
 * @file Field.h
 * @author Piotr Kluba
 * @brief
 * @version 0.1
 * @date 2024-04-16
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef SRC_GAME_LOGIC_FIELD_H_
#define SRC_GAME_LOGIC_FIELD_H_

enum class TerrainType { SNOW, GRASS, STONE, DIRT, STRUCTURE };

class Field {
 private:
  bool grail_here_;
  bool walk_through_;
  TerrainType terrain_type_;

 public:
  Field(TerrainType terrain_type = TerrainType::GRASS, bool grail = false,
        bool walk_through = true)
      : grail_here_(grail),
        walk_through_(walk_through),
        terrain_type_(terrain_type){};
  ~Field();
};

#endif