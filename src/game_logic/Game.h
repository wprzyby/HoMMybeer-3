/**
 * @file Game.h
 * @author Piotr Kluba
 * @brief
 * @version 0.1
 * @date 2024-04-15
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef SRC_GAME_LOGIC_GAME_H_
#define SRC_GAME_LOGIC_GAME_H_

#include <Map.h>
#include <Player.h>

#include <memory>
#include <vector>

enum class Difficulty { EASY, NORMAL, HARD };

class Game {
 private:
  Map map_;
  Difficulty difficulty_;
  std::vector<Player> players_in_game_;

 public:
  std::shared_ptr<Map> map_pointer;
  Game(MapSize map_size, Difficulty difficulty, int ai_enemies);
  ~Game();
};

#endif