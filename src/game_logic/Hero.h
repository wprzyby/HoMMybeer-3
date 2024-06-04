/**
 * @file Hero.h
 * @author Piotr Kluba
 * @brief Functionality responsible for the single hero
 * @copyright Copyright (c) 2024
 */

#ifndef SRC_GAME_LOGIC_HERO_H
#define SRC_GAME_LOGIC_HERO_H

#include <UnitBlock.hpp>
#include <stack>
#include <string>
#include <utility>
#include <vector>

#include "Config.h"

using FieldCoords = std::pair<int, int>;
using Path = std::stack<FieldCoords>;
using MoveCosts = std::stack<int>;

class Hero {
 private:
  inline static constexpr int DEFAULT_STARTING_ENERGY = 100;
  std::string name_;
  FieldCoords hero_coords_;
  Path move_path_;
  MoveCosts current_path_costs_;
  int energy_;
  std::vector<UnitBlock> units_;
  void step(FieldCoords step_to);
  int max_energy_;
  Faction faction_;

 public:
  Hero(std::string name, FieldCoords spawn_field_coords,
       Faction faction = Faction::CASTLE,
       int starting_energy =
           DEFAULT_STARTING_ENERGY)  // TODO remove default faction
      : name_(std::move(name)),
        hero_coords_(std::move(spawn_field_coords)),
        energy_(starting_energy),
        max_energy_(starting_energy),
        faction_(faction) {}
  [[nodiscard]] std::string getHeroName() const { return name_; };
  [[nodiscard]] FieldCoords getHeroCoords() const { return hero_coords_; };
  void setMovePath(Path new_path, MoveCosts move_costs) {
    move_path_ = std::move(new_path);
    current_path_costs_ = std::move(move_costs);
  }
  void refillEnergy() { energy_ = max_energy_; }
  bool moveAlong(const Path& updated_path, MoveCosts costs);
  [[nodiscard]] int getEnergy() const { return energy_; }
  // std::optional<Path> setMoveGoal(FieldCoords goal);
  [[nodiscard]] const std::vector<UnitBlock>& getUnits() const {
    return units_;
  }
  void addUnit(const UnitBlock& unit);
  void setUnits(const std::vector<UnitBlock>& new_units) { units_ = new_units; }
  std::pair<Path, int> getPathInParts() const;
  Faction getFaction() const { return faction_; }
};

#endif // SRC_GAME_LOGIC_HERO_H