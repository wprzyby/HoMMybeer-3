/**
 * @file Session.cc
 * @author Piotr Kluba
 * @brief The base class responsible for managing the state of a current app
 * instance
 * @version 0.1
 * @date 2024-04-15
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <Session.h>

#include <ctime>
#include <iostream>
#include <utility>

using namespace std;

Session* Session::session_ = nullptr;

Session::Session() : game(Game({}, Map({}))) {
  session_state_ = SessionState::START_MENU;
  time_t t = time(0);
  cout << "Session created at: " << asctime(localtime(&t));
}

Session::~Session() {
  if (session_ != nullptr) {
    delete session_;
  }
}

void Session::saveGameToFile(string filepath) {}

void Session::loadGameFromFile(string filepath) {}

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
