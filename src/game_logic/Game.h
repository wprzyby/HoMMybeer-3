/**
 * @file Game.h
 * @author Piotr Kluba
 * @brief Base game class responsible for managing the state of the current game
 * @copyright Copyright (c) 2024
 *
 */

#ifndef SRC_GAME_LOGIC_GAME_H
#define SRC_GAME_LOGIC_GAME_H

#include <Config.h>
#include <Hero.h>
#include <Map.h>
#include <MapObject.h>
#include <Player.h>

#include <format>
#include <memory>
#include <optional>
#include <utility>
#include <vector>

using Path = std::stack<FieldCoords>;
using MoveCosts = std::stack<int>;
class Game {
 private:
  constexpr static unsigned int LAST_DAY_OF_WEEK = 7U;
  inline const static float SQRT_2 = std::sqrt(2.F);
  std::vector<Player> players_in_game_;
  Map game_map_;
  std::vector<std::shared_ptr<MapObject>> map_objects_;
  int curr_player_idx;
  std::optional<std::pair<Path, MoveCosts>> findPath(FieldCoords path_to);
  unsigned int day_of_week_;

 public:
  Game(
      std::vector<Player> players, const Map& map,
      const std::vector<std::shared_ptr<MapObject>>& starting_map_objects = {});
  Game& operator=(const Game& other);
  [[nodiscard]] const Map* getMap() const { return &game_map_; }
  [[nodiscard]] const Player* getPlayer(int idx) const;
  [[nodiscard]] unsigned int getWeekday() const { return day_of_week_; }
  void nextDay();
  Player* getPlayer(int idx);
  void nextPlayer();
  [[nodiscard]] std::vector<const Hero*> heroesInGame()
      const;  // only for visuals
  [[nodiscard]] std::vector<const MapObject*> objectsInGame()
      const;  // only for visuals
  Player* getCurrentPlayer() { return &players_in_game_[curr_player_idx]; }
  [[nodiscard]] const Player* getCurrentPlayer() const {
    return &players_in_game_[curr_player_idx];
  }
  [[nodiscard]] int numPlayers() const {
    return static_cast<int>(players_in_game_.size());
  }
  bool addMapObject(const std::shared_ptr<MapObject>& obj_to_add);
  bool deleteMapObject(int map_object_id);
  [[nodiscard]] int getCurrPlayerId() const { return curr_player_idx; }
  void executeAction(FieldCoords coords);
  static constexpr int MAX_PLAYERS = 4;
  class WrongObjectPlacementException : public std::exception {
    FieldCoords conflicting_coords_;
    FieldCoords present_origin_;
    FieldCoords incoming_origin_;

   public:
    WrongObjectPlacementException(FieldCoords conflicting_coords,
                                  MapObject& obj_present, MapObject& obj_added)
        : conflicting_coords_(std::move(conflicting_coords)),
          present_origin_(obj_present.getOrigin()),
          incoming_origin_(obj_added.getOrigin()) {}
    WrongObjectPlacementException(
        const WrongObjectPlacementException& other) noexcept
        : conflicting_coords_(other.conflicting_coords_) {}
    [[nodiscard]] FieldCoords getBadChar() const { return conflicting_coords_; }
    std::string what() {
      return std::format(
          "Conflicting coords are: {}, {}. Already present origin is: {}, {}. "
          "Conflicting object origin is: {}, {}",
          conflicting_coords_.first, conflicting_coords_.second,
          present_origin_.first, present_origin_.second, incoming_origin_.first,
          incoming_origin_.second);
    }
  };
};

#endif  // SRC_GAME_LOGIC_GAME_H