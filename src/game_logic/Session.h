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

#include <memory>
#include <string>
#include <vector>


enum class SessionState {
  IN_GAME,
  START_MENU,
  IN_GAME_MENU,  // TODO: fill if necessary
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
  SessionState session_state_;

 public:
  Session(Session& other) = delete;
  void operator=(const Session& other) = delete;
  ~Session();
  static Session* getInstance();
  const std::vector<Save> getSavedGames();
  SessionState getSessionState();
  void setSessionState(SessionState new_state) { session_state_ = new_state; };
  void newGame(Map game_map, std::vector<Player> players,
               Difficulty difficulty);
  std::unique_ptr<Game> game;
};

#endif