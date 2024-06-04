/**
 * @file Session.h
 * @author Piotr Kluba
 * @brief The base class responsible for managing the state of a current app
 * instance
 * @version 0.1
 * @date 2024-04-15
 *
 * @copyright Copyright (c) 2024
 *
 */
#ifndef SRC_GAME_LOGIC_SESSION_H_
#define SRC_GAME_LOGIC_SESSION_H_

#include <Config.h>
#include <Game.h>
#include <Map.h>
#include <MapObject.h>

#include <memory>
#include <string>
#include <vector>

enum class SessionState {
  IN_GAME,
  LOAD_GAME,
  REFRESH,
  START_MENU,
  LOAD_CASTLE,
  LOAD_BATTLE,
  IN_BATTLE,
  IN_CASTLE  // TODO: fill if necessary
};

struct Save {
 public:
  std::string filename;
  int hours_played;
  int heroes_amount;
  int best_hero_level;
};

class Session {
 private:
  void saveGameToFile(std::string filepath);
  Session();
  void loadGameFromFile(std::string filepath);
  static Session* session_;
  inline static SessionState session_state_;
  inline static Faction current_town_;
  std::pair<int, int> attacked_hero_info_;
  TerrainType battle_terrain_type_;

 public:
  Session(Session& other) = delete;
  void operator=(const Session& other) = delete;
  ~Session();
  static Session* getInstance();
  const std::vector<Save> getSavedGames();
  static SessionState getSessionState() { return session_state_; }
  static void setSessionState(SessionState new_state) {
    session_state_ = new_state;
  };
  static void setTown(Faction faction) { current_town_ = faction; }
  static Faction getTown() { return current_town_; }
  std::pair<int, int> getAttackedHeroInfo() const { return attacked_hero_info_; }
  TerrainType getBattleTerrainType() const {return battle_terrain_type_;}
  void setAttackedHeroInfo(std::pair<int, int> hero_info) {
    attacked_hero_info_ = hero_info;
  };
  void setBattleTerrainType(TerrainType type) { battle_terrain_type_ = type; };
  void newGame(
      const Map& game_map, std::vector<Player> players,
      Difficulty difficulty = Difficulty::NORMAL,
      const std::vector<std::shared_ptr<MapObject>>& starting_objects = {});
  Game game;
};

#endif