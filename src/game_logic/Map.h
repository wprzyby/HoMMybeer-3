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

enum class MapSize { S = 500, M = 800, L = 1200, XL = 2000 };

class Map {
 private:
  int width_;
  std::vector<std::vector<Field>> field_array_;

 public:
  Map(MapSize size);
  Map(){};
  ~Map();
  std::optional<Field*> getField(int x, int y);
  const int getWidth() { return width_; };
};

#endif
