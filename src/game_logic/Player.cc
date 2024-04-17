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
#include <Session.h>

using namespace std;

Player::Player(bool ai, std::shared_ptr<Map> map) {
  is_ai_ = ai;
  int x = map->getWidth() / 2;  // TODO: starting positions of heros
  players_heros_ = vector<Hero>{Hero(
      "Andrew the mediocre",
      map->getField(x, x).value_or(nullptr))};  // TODO: add handling bad field
}

Player::~Player() {}

void Player::addHero(std::string name, Field* spawn_field) {
  players_heros_.push_back(Hero(name, spawn_field));
}

optional<Hero*> Player::getHero(int idx) {
  if (idx >= players_heros_.size()) {
    return {};
  }
  return &players_heros_[idx];
}