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

#include <format>
#include <memory>
#include <optional>
#include <vector>

class Game {
 private:
  std::vector<Player> players_in_game_;
  Map game_map_;
  std::vector<std::shared_ptr<MapObject>> map_objects_;
  Difficulty difficulty_;
  int curr_player_idx;

 public:
  Game(std::vector<Player> players, Map map,
       std::vector<std::shared_ptr<MapObject>> starting_map_objects = {});
  Map* getMap() { return &game_map_; }
  Player* getPlayer(int idx);
  Player* getCurrentPlayer() { return &players_in_game_[curr_player_idx]; }
  ~Game() = default;
  bool moveCurrPlayer(FieldCoords coords);
  bool addMapObject(std::shared_ptr<MapObject> obj_to_add);
  bool deleteMapObject(int id);
  int getCurrPlayerId() const { return curr_player_idx; }
  static constexpr int MAX_PLAYERS = 4;
  class WrongObjectPlacementException : public std::exception {
    FieldCoords conflicting_coords_;
    FieldCoords present_origin_;
    FieldCoords incoming_origin_;

   public:
    WrongObjectPlacementException(FieldCoords conflicting_coords,
                                  MapObject& obj_present, MapObject& obj_added)
        : conflicting_coords_(conflicting_coords),
          present_origin_(obj_present.getOrigin()),
          incoming_origin_(obj_added.getOrigin()) {}
    WrongObjectPlacementException(
        const WrongObjectPlacementException& e) throw()
        : conflicting_coords_(e.conflicting_coords_) {}
    FieldCoords getBadChar() const { return conflicting_coords_; }
    const std::string what() {
      return std::format(
          "Conflicting coords are: {}, {}. Already present origin is: {}, {}. "
          "Conflicting object origin is: {}, {}",
          conflicting_coords_.first, conflicting_coords_.second,
          present_origin_.first, present_origin_.second, incoming_origin_.first,
          incoming_origin_.second);
    }
  };
};

#endif