/**
 * @file Session.cc
 * @author Piotr Kluba
 * @brief The base class responsible for managing the state of a current app
 * instance
 * @copyright Copyright (c) 2024
 */

#include <Session.h>

#include <ctime>
#include <iostream>
#include <utility>

Session* Session::session_ = nullptr;

Session::Session() : game(Game({}, Map({}))) {
  session_state_ = SessionState::START_MENU;
  time_t creation_timestamp = time(nullptr);
  std::cout << "Session created at: " << asctime(localtime(&creation_timestamp));
}

Session::~Session() { delete session_; }

void Session::saveGameToFile(const std::string& filepath) {}

void Session::loadGameFromFile(const std::string& filepath) {}

Session* Session::getInstance() {
  if (session_ == nullptr) {
    session_ = new Session();
  }
  return session_;
}

void Session::newGame(
    const Map& game_map, std::vector<Player> players, Difficulty difficulty,
    const std::vector<std::shared_ptr<MapObject>>& starting_objects) {
  game = Game(std::move(players), game_map, starting_objects);
  session_state_ = SessionState::IN_GAME;
}
