/**
 * @file MainWindowController.cc
 * @author Piotr Kluba
 * @brief Controller managing the window when the game is in the main menu
 * @copyright Copyright (c) 2024
 */

#include "MainWindowController.h"

#include <Session.h>

void MainWindowController::update(sf::Event& event, SessionState session_state,
                                  Game& /*game*/) {
  if (session_state == SessionState::START_MENU) {
    if (event.type == sf::Event::KeyPressed) {
    }
    if (event.key.scancode == sf::Keyboard::Scan::Enter) {
      Session::getInstance()->setSessionState(SessionState::LOAD_GAME);
    }
  }
}

void MainWindowController::draw(sf::RenderTarget& target,
                                sf::RenderStates /*states*/) const {
  target.draw(main_view_);
}