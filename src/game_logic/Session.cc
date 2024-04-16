/**
 * @file Session.cc
 * @author Piotr Kluba
 * @brief
 * @version 0.1
 * @date 2024-04-15
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <Session.h>

#include <ctime>
#include <iostream>

using namespace std;

Session* Session::session_ = nullptr;

Session::Session() {
  game = nullptr;
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

void Session::newGame(MapSize size, int number_of_enemies,
                      Difficulty difficulty) {
  game = make_unique<Game>(Game(size, difficulty, number_of_enemies));
  session_state_ = SessionState::IN_GAME;
}
