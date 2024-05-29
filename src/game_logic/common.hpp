#ifndef SRC_GAME_LOGIC_COMMON_HPP
#define SRC_GAME_LOGIC_COMMON_HPP

#include <utility>

using FieldCoords = std::pair<int, int>;

FieldCoords operator+(const FieldCoords& l, const FieldCoords& r);

#endif  // SRC_GAME_LOGIC_COMMON_HPP