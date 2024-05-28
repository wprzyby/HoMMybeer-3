/**
 * @file Hero.h
 * @author Piotr Kluba
 * @brief Funcionality responsible for the single hero
 * @version 0.1
 * @date 2024-04-17
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef SRC_GAME_LOGIC_HERO_H_
#define SRC_GAME_LOGIC_HERO_H_

#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>

using FieldCoords = std::pair<int, int>;
using Path = std::vector<FieldCoords>;

class Hero {
 private:
  std::string name_;
  FieldCoords hero_coords_;
  Path move_path_;
  float energy_;
  void step(FieldCoords step_to);

 public:
  Hero(std::string name, FieldCoords spawn_field_coords)
      : name_(name),
        hero_coords_(spawn_field_coords),
        move_path_({}),
        energy_(10000){};
  ~Hero() = default;
  std::string getHeroName() const { return name_; };
  FieldCoords getHeroCoords() const { return hero_coords_; };
  // std::optional<Path> setMoveGoal(FieldCoords goal);
  void moveTo(FieldCoords coords);
};

#endif