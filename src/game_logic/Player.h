/**
 * @file Player.h
 * @author Piotr Kluba, Wojciech Przybylski
 * @brief Structure used to store the properties of the players and for them to
 * take certain actions
 * @copyright Copyright (c) 2024
 */

#ifndef SRC_GAME_LOGIC_PLAYER_H
#define SRC_GAME_LOGIC_PLAYER_H

#include <Config.h>
#include <Hero.h>

#include <map>
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
  [[nodiscard]] const Hero* getHero(
      int idx) const;  // TODO: overload with ex name
  Hero* getHeroToModify(int idx);
  Hero* getCurrentHero() { return &players_heroes_[selected_hero_idx_]; }
  [[nodiscard]] const Hero* getCurrentHero() const {
    return &players_heroes_[selected_hero_idx_];
  }
  void addHero(const std::string& name, FieldCoords spawn_field_coords,
               Faction faction, int starting_energy);
  void addHero(const std::string& name, FieldCoords spawn_field_coords,
               Faction faction);
  void addHero(const Hero& hero) { players_heroes_.push_back(hero); };
  void weeklyIncome();
  bool updateResourceQuantity(ResourceType resource_type, int delta);
  int numHeroes() const { return static_cast<int>(players_heroes_.size()); }
  bool changeIncome(ResourceType resource_type, int delta);
  void refillHeroesEnergy();
  int getResourceAmount(ResourceType resource) const {
    return inventory_.at(resource);
  }
  int getResourceIncome(ResourceType resource) const {
    return income_.at(resource);
  }
  void nextHero();
  Faction getFaction() const { return faction_; }
  void setCurrentHeroUnits(const std::vector<UnitBlock>& units);
  void setHeroUnits(int hero_idx, const std::vector<UnitBlock>& units);
  bool isDead() const;
  bool isAI() const { return is_ai_; }
};

#endif // SRC_GAME_LOGIC_PLAYER_H