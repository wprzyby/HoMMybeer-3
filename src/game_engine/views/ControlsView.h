/**
 * @file ControlsView.h
 * @author Piotr Kluba (you@domain.com)
 * @brief Entity representing buttons used to control the game
 * @date 2024-06-02
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef SRC_GAME_ENGINE_VIEWS_CONTROLS_VIEW_H_
#define SRC_GAME_ENGINE_VIEWS_CONTROLS_VIEW_H_

#include <SFML/Graphics.hpp>

class ControlsView : public sf::Drawable, public sf::Transformable {
 private:
  sf::RectangleShape background_;

 public:
  ControlsView() = default;
  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
  void setControls();
};

#endif