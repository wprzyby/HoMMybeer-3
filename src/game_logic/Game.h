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

#include <Config.h>
#include <Map.h>
#include <MapObject.h>
#include <Player.h>

#include <memory>
#include <optional>
#include <vector>

class Game {
 private:
  std::vector<Player> players_in_game_;
  std::vector<std::shared_ptr<MapObject>> map_objects_;
  Map game_map_;
  Difficulty difficulty_;
  int curr_player_idx;

 public:
  Game(std::vector<Player> players, Map map)
      : players_in_game_(players), game_map_(map), curr_player_idx(0) {}
  Map* getMap() { return &game_map_; }
  Player* getPlayer(int idx);
  Player* getCurrentPlayer() { return &players_in_game_[curr_player_idx]; }
  ~Game() = default;
  bool moveCurrPlayer(FieldCoords coords);
  bool addMapObject(std::shared_ptr<MapObject> obj_to_add);
  bool deleteMapObject(int id);
  int getCurrPlayerId() const { return curr_player_idx; }
  static constexpr int MAX_PLAYERS = 4;
};

#endif