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

#include <Field.h>

#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>

typedef std::vector<FieldCoords> Path;

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
  ~Hero();
  const std::string getHeroName() { return name_; };
  const FieldCoords getHeroCoords() { return hero_coords_; };
  std::optional<Path> setMoveGoal(FieldCoords goal);
  void move();
};

#endif