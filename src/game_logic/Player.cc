/**
 * @file Player.cc
 * @author Piotr Kluba, Wojciech Przybylski
 * @brief Structure used to store the properties of the players and for them to
 * take certain actions
 * @copyright Copyright (c) 2024
 */

#include <Config.h>
#include <Map.h>
#include <Player.h>

#include <map>
#include <string>
#include <utility>

using namespace std;

Player::Player(bool is_ai, Faction faction, FieldCoords starting_location,
               Inventory starting_inventory, Incomes starting_income)
    : is_ai_(is_ai),
      faction_(faction),
      selected_hero_idx_(0),
      inventory_(std::move(starting_inventory)),
      income_(std::move(starting_income)) {
  addHero(Config::DEFAULT_HERO_NAMES.at(faction), starting_location, faction_);
}

void Player::addHero(const std::string& name, FieldCoords spawn_field_coords,
                     Faction faction, int starting_energy) {
  Hero new_hero(name, spawn_field_coords, faction, starting_energy);
  new_hero.setUnits(Config::getInstance()->getStartingUnits(faction_));
  players_heroes_.push_back(new_hero);
}

void Player::addHero(const std::string& name, FieldCoords spawn_field_coords,
                     Faction faction) {
  Hero new_hero(name, spawn_field_coords, faction);
  new_hero.setUnits(Config::getInstance()->getStartingUnits(faction_));
  players_heroes_.push_back(new_hero);
}

const Hero* Player::getHero(int idx) const {
  if (idx >= players_heroes_.size()) {
    return nullptr;
  }
  return &players_heroes_[idx];
}

Hero* Player::getHeroToModify(int idx) {
  if (idx >= players_heroes_.size()) {
    return nullptr;
  }
  return &players_heroes_[idx];
}

bool Player::updateResourceQuantity(ResourceType resource_type, int delta) {
  if (inventory_.at(resource_type) + delta < 0) {
    return false;
  }
  inventory_.at(resource_type) = inventory_.at(resource_type) + delta;
  return true;
}

bool Player::changeIncome(ResourceType resource_type, int delta) {
  if (income_.at(resource_type) + delta < 0) {
    return false;
  }
  income_.at(resource_type) = income_.at(resource_type) + delta;
  return true;
}

void Player::nextHero() {
  if (selected_hero_idx_ == players_heroes_.size() - 1) {
    selected_hero_idx_ = 0;
  } else {
    selected_hero_idx_++;
  }
}

void Player::refillHeroesEnergy() {
  for (Hero& hero : players_heroes_) {
    hero.refillEnergy();
  }
}

void Player::weeklyIncome() {
  for (auto resource_income : income_) {
    updateResourceQuantity(resource_income.first, resource_income.second);
  }
}

void Player::setCurrentHeroUnits(const std::vector<UnitBlock>& units) {
  setHeroUnits(selected_hero_idx_, units);
}
void Player::setHeroUnits(int hero_idx, const std::vector<UnitBlock>& units) {
  if (hero_idx >= players_heroes_.size()) {
    return;
  }
  players_heroes_[hero_idx].setUnits(units);
  if (players_heroes_[hero_idx].getUnits().size() != 0) {
    return;
  }
  if (hero_idx != selected_hero_idx_) {
    players_heroes_.erase(players_heroes_.begin() + hero_idx);
    return;
  }
  if (players_heroes_.size() == 1) {
    players_heroes_.pop_back();
    return;
  }
  auto index = selected_hero_idx_;
  nextHero();
  players_heroes_.erase(players_heroes_.begin() + index);
}
bool Player::isDead() const { return players_heroes_.size() == 0; }