/**
 * @file Hero.h
 * @author Piotr Kluba
 * @brief Functionality responsible for the single hero
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
#include <stack>
#include <UnitBlock.hpp>
#include <string>
#include <utility>
#include <vector>

using FieldCoords = std::pair<int, int>;
using Path = std::stack<FieldCoords>;
using MoveCosts = std::stack<int>;

class Hero {
 private:
  std::string name_;
  FieldCoords hero_coords_;
  Path move_path_;
  int energy_;
  std::vector<UnitBlock> units_;
  void step(FieldCoords step_to);
  int max_energy_;

 public:
  Hero(std::string name, FieldCoords spawn_field_coords,
       int starting_energy = 100)
      : name_(name),
        hero_coords_(spawn_field_coords),
        energy_(starting_energy),
        max_energy_(starting_energy){};
  ~Hero() = default;
  std::string getHeroName() const { return name_; };
  FieldCoords getHeroCoords() const { return hero_coords_; };
  void setMovePath(Path new_path) { move_path_ = new_path; }
  void refillEnergy() { energy_ = max_energy_; }
  bool moveAlong(Path updated_path, MoveCosts costs);
  int getEnergy() const { return energy_; }
  // std::optional<Path> setMoveGoal(FieldCoords goal);
  [[nodiscard]] const std::vector<UnitBlock>& getUnits() const {
    return units_;
  }
  void addUnit(const UnitBlock& unit) { units_.push_back(unit); }
};

#endif