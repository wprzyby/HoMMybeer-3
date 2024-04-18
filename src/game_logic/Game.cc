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

Player* const Game::getPlayer(int idx) {
  if (players_in_game_.size() <= idx) {
    return nullptr;
  }
  return &players_in_game_[idx];
}

bool Game::moveCurrPlayer(FieldCoords coords) {
  if (not game_map_.getField(coords)->isWalkable()) {
    return false;
  }
  this->getCurrentPlayer()->getCurrentHero()->moveTo(coords);
  return true;
}