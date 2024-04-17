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
}

Game::~Game() {}

std::shared_ptr<Map> Game::getMap() { return map_pointer_; }