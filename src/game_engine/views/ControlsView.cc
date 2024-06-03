/**
 * @file ControlsView.h
 * @author Piotr Kluba (you@domain.com)
 * @brief Entity representing buttons used to control the game
 * @date 2024-06-02
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <ControlsView.h>

void ControlsView::setControls() {
  background_ = sf::RectangleShape(sf::Vector2f({278, 600}));
  background_.setFillColor(sf::Color(82, 38, 8));
  background_.setPosition(sf::Vector2f({522, 0}));
}

void ControlsView::draw(sf::RenderTarget& target,
                        sf::RenderStates states) const {
  target.draw(background_, states);
}