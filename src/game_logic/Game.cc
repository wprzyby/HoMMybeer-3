/**
 * @file Game.cc
 * @author Piotr Kluba
 * @brief
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
  map_ = Map(map_size);
  difficulty_ = difficulty;
  map_pointer = make_shared<Map>(map_);
}

Game::~Game() {}
