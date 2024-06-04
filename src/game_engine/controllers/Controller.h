/**
 * @file Controller.h
 * @author Piotr Kluba
 * @brief Base class for controllers
 * @copyright Copyright (c) 2024
 */

#ifndef SRC_GAME_ENGINE_CONTROLLERS_CONTROLLER_H
#define SRC_GAME_ENGINE_CONTROLLERS_CONTROLLER_H

#include <Session.h>

#include <SFML/Graphics.hpp>

class Controller {
 public:
  Controller() = default;
  virtual void update(sf::Event& event, SessionState session_state,
                      Game& game) = 0;
};

#endif // SRC_GAME_ENGINE_CONTROLLERS_CONTROLLER_H