/**
 * @file Player.cc
 * @author Piotr Kluba
 * @brief Structure used to store the properties of the players and for them to
 * take certain actions
 * @version 0.1
 * @date 2024-04-16
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <Config.h>
#include <Map.h>
#include <Player.h>

#include <map>
#include <string>

using namespace std;

Player::Player(bool is_ai, Faction faction, FieldCoords starting_location,
               Inventory starting_inventory, Incomes starting_income)
    : is_ai_(is_ai),
      faction_(faction),
      selected_hero_idx_(0),
      inventory_(starting_inventory),
      income_(starting_income) {
  players_heroes_ = vector<Hero>{
      Hero(Config::kDefaultHeroNames.at(faction), starting_location)};
}

Player::~Player() {}

void Player::addHero(std::string name, FieldCoords spawn_field_coords,
                     int starting_energy) {
  players_heroes_.push_back(Hero(name, spawn_field_coords, starting_energy));
}

const Hero* Player::getHero(int idx) const {
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