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
  players_heros_ = vector<Hero>{
      Hero(Config::kDefaultHeroNames.at(faction), starting_location)};
}

Player::~Player() {}

void Player::addHero(std::string name, FieldCoords spawn_field_coords,
                     int starting_energy) {
  players_heros_.push_back(Hero(name, spawn_field_coords));
}

Hero* Player::getHero(int idx) {
  if (idx >= players_heros_.size()) {
    return nullptr;
  }
  return &players_heros_[idx];
}

bool Player::updateResourceQuantity(ResourceType resource_type, int delta) {
  if (inventory_[resource_type] + delta < 0) {
    return false;
  }
  inventory_[resource_type] = inventory_[resource_type] + delta;
  return true;
}

bool Player::changeIncome(ResourceType resource_type, int delta) {
  if (income_[resource_type] + delta < 0) {
    return false;
  }
  income_[resource_type] = income_[resource_type] + delta;
  return true;
}

void Player::nextHero() {
  if (selected_hero_idx_ == players_heros_.size() - 1) {
    selected_hero_idx_ = 0;
  } else {
    selected_hero_idx_++;
  }
}