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

#include <Map.h>
#include <Player.h>

#include <map>
#include <string>

using namespace std;

const static std::map<Faction, std::string> kDefaultHeroNames{
    {Faction::CASTLE, "Andrew the Mediocre"},
    {Faction::FORTRESS, "Peter the Fearful"},
    {Faction::INFERNO, "Stefan the Devil"},
};

Player::Player(bool is_ai, Faction faction, FieldCoords starting_location)
    : is_ai_(is_ai), faction_(faction), selected_hero_idx_(0) {
  players_heros_ =
      vector<Hero>{Hero(kDefaultHeroNames.at(faction), starting_location)};
}

Player::~Player() {}

void Player::addHero(std::string name, FieldCoords spawn_field_coords) {
  players_heros_.push_back(Hero(name, spawn_field_coords));
}

Hero* const Player::getHero(int idx) {
  if (idx >= players_heros_.size()) {
    return nullptr;
  }
  return &players_heros_[idx];
}
