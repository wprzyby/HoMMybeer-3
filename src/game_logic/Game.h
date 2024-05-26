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
  std::vector<Player> players_in_game_;
  Map game_map_;
  Difficulty difficulty_;
  int curr_player_idx;

 public:
  Game(std::vector<Player> players, Map map)
      : players_in_game_(players), game_map_(map), curr_player_idx(0) {}
  Map* getMap() { return &game_map_; }
  Player* getPlayer(int idx);
  Player* getCurrentPlayer() {
    return &players_in_game_[curr_player_idx];
  }
  ~Game() = default;
  bool moveCurrPlayer(FieldCoords coords);
};

#endif