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

#include <Game.h>
#include <Map.h>

#include <String>
#include <Vector>
#include <memory>

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
  const SessionState getSessionState();
  void setSessionState(SessionState new_state) { session_state_ = new_state; };
  void newGame(MapSize size, int number_of_enemies, Difficulty difficulty);
  std::unique_ptr<Game> game;
};

#endif