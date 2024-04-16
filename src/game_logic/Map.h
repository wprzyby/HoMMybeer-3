/**
 * @file Map.h
 * @author Piotr Kluba
 * @brief
 * @version 0.1
 * @date 2024-04-16
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef SRC_GAME_LOGIC_MAP_H_
#define SRC_GAME_LOGIC_MAP_H_

#include <Field.h>

#include <vector>

enum class MapSize { S, M, L, XL };

class Map {
 private:
  int width_;
  std::vector<std::vector<Field>> field_array_;

 public:
  Map(MapSize size);
  Map(){};
  ~Map();
  class CoordinatesOutOfMap : public std::exception {
    int x_;
    int y_;

   public:
    CoordinatesOutOfMap(int x, int y) : x_(x), y_(y){};
  };
  Field* getField(int x, int y);
};

#endif
