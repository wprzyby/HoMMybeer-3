/**
 * @file MainWindowController.h
 * @author Piotr Kluba (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2024-06-04
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef SRC_GAME_ENGINE_MAIN_WINDOW_CONTROLLER_H_
#define SRC_GAME_ENGINE_MAIN_WINDOW_CONTROLLER_H_

#include <Controller.h>
#include <MainView.h>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/System/Vector2.hpp>

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

#endif