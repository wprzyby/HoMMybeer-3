/**
 * @file Game.h
 * @author Piotr Kluba
 * @brief Base game class responsible for managing the state of the current game
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
#include <optional>
#include <vector>

enum class Difficulty { EASY, NORMAL, HARD };

class Game {
 private:
  std::shared_ptr<Map> map_pointer_;
  Difficulty difficulty_;
  std::vector<Player> players_in_game_;

 public:
  std::shared_ptr<Map> getMap();
  Game(MapSize map_size, Difficulty difficulty, int ai_enemies);
  std::optional<Player*> getPlayer(int idx);
  ~Game();
};

#endif