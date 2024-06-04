/**
 * @file BorderView.h
 * @author Piotr Kluba (you@domain.com)
 * @brief Static border class
 * @date 2024-06-02
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <BorderView.h>

void BorderView::setBorder() {
  rect_vertical_ = sf::RectangleShape(sf::Vector2f(
      {game_window_offset_.x, game_window_offset_.y + game_window_size_.y}));
  rect_horizontal_ = sf::RectangleShape(sf::Vector2f(
      {game_window_offset_.x + game_window_size_.x, game_window_offset_.y}));
  rect_vertical_.setFillColor(sf::Color(82, 38, 8));
  rect_horizontal_.setFillColor(sf::Color(82, 38, 8));
}

void BorderView::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  target.draw(rect_vertical_, states);
  target.draw(rect_horizontal_, states);
}