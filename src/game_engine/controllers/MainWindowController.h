/**
 * @file MainWindowController.h
 * @author Piotr Kluba
 * @brief Controller managing the window when the game is in the main menu
 * @copyright Copyright (c) 2024
 */

#ifndef SRC_GAME_ENGINE_CONTROLLERS_MAIN_WINDOW_CONTROLLER_H
#define SRC_GAME_ENGINE_CONTROLLERS_MAIN_WINDOW_CONTROLLER_H

#include <MainView.h>

#include <SFML/Graphics.hpp>

#include "Controller.h"

class MainWindowController : public Controller, public sf::Drawable {
 private:
  MainView main_view_;

 public:
  explicit MainWindowController(sf::Vector2u main_window_size)
      : main_view_(MainView(main_window_size)) {
    main_view_.setMain();
  }
  void update(sf::Event& event, SessionState session_state,
              Game& game) override;
  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

#endif  // SRC_GAME_ENGINE_CONTROLLERS_MAIN_WINDOW_CONTROLLER_H