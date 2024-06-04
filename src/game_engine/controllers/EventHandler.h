/**
 * @file EventHandler.h
 * @author Piotr Kluba
 * @brief Observable to handle inputs of the user
 * @copyright Copyright (c) 2024
 */

#ifndef SRC_GAME_ENGINE_CONTROLLERS_EVENT_HANDLER_H
#define SRC_GAME_ENGINE_CONTROLLERS_EVENT_HANDLER_H

#include <Session.h>

#include <SFML/Graphics.hpp>
#include <utility>
#include <vector>

#include "Controller.h"

class EventHandler {
 private:
  std::vector<Controller*> controllers_;

 public:
  explicit EventHandler(std::vector<Controller*> controllers)
      : controllers_(std::move(controllers)) {}
  void handle(sf::Event& event, SessionState session_state, Game& game);
};

#endif // SRC_GAME_ENGINE_CONTROLLERS_EVENT_HANDLER_H