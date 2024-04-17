/**
 * @file Game.cc
 * @author Piotr Kluba
 * @brief Base game class responsible for managing the state of the current game
 * @version 0.1
 * @date 2024-04-15
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <Game.h>

#include <memory>

using namespace std;

Game::Game(MapSize map_size, Difficulty difficulty, int ai_enemies) {
  difficulty_ = difficulty;
  map_pointer_ = make_shared<Map>(Map(map_size));
  players_in_game_ = {};
  players_in_game_.push_back(Player(false, map_pointer_));
  for (int i = 0; i < ai_enemies; i++) {
    players_in_game_.push_back(Player(true, map_pointer_));
  }
}

Game::~Game() {}

shared_ptr<Map> Game::getMap() { return map_pointer_; }

optional<Player*> Game::getPlayer(int idx) {
  if (players_in_game_.size() <= idx) {
    return {};
  }
  return &players_in_game_[idx];
}