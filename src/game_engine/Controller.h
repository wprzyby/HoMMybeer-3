/**
 * @file Controller.h
 * @author Piotr Kluba (you@domain.com)
 * @brief Base class for controllers
 * @version 0.1
 * @date 2024-06-03
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef SRC_GAME_ENGINE_CONTROLLER_H_
#define SRC_GAME_ENGINE_CONTROLLER_H_

#include <Session.h>

#include <SFML/Graphics.hpp>

class Controller {
 public:
  Controller() = default;
  virtual void update(sf::Event& event, SessionState session_state,
                      Game& game) = 0;
};

#endif