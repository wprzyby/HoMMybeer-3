/**
 * @file Player.h
 * @author Piotr Kluba
 * @brief Structure used to store the properties of the players and for them to
 * take certain actions
 * @version 0.1
 * @date 2024-04-16
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef SRC_GAME_LOGIC_PLAYER_H_
#define SRC_GAME_LOGIC_PLAYER_H_

#include <Config.h>
#include <Hero.h>

#include <map>
#include <memory>
#include <optional>
#include <vector>

using Inventory = std::map<ResourceType, int>;
using Incomes = std::map<ResourceType, int>;

class Player {
 private:
  bool is_ai_;
  std::vector<Hero> players_heroes_;
  int selected_hero_idx_;
  Faction faction_;
  Inventory inventory_;
  Incomes income_;

 public:
  Player(
      bool is_ai, Faction faction, FieldCoords starting_location,
      Inventory starting_inventory =
          Config::getStartingInventory(Difficulty::NORMAL),
      Incomes starting_income = Config::getStartingIncomes(Difficulty::NORMAL));
  ~Player();
  const Hero* getHero(int idx) const;  // TODO: overload with ex name
  Hero* getCurrentHero() { return &players_heroes_[selected_hero_idx_]; }
  void addHero(std::string name, FieldCoords spawn_field_coords,
               int starting_energy = 100);
  void addHero(Hero hero) { players_heroes_.push_back(hero); };
  bool updateResourceQuantity(ResourceType resource_type, int delta);
  int numHeroes() const { return static_cast<int>(players_heroes_.size()); }
  bool changeIncome(ResourceType resource_type, int delta);
  int getResourceAmount(ResourceType resource) const {
    return inventory_.at(resource);
  }
  int getResourceIncome(ResourceType resource) const {
    return income_.at(resource);
  }
  void nextHero();
};

#endif