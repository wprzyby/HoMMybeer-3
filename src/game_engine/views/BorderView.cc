/**
 * @file BorderView.cc
 * @author Piotr Kluba
 * @brief View defining the static border around the map
 * @copyright Copyright (c) 2024
 */

#include <BorderView.h>

const sf::Color BorderView::BACKGROUND_COLOR = sf::Color(82, 38, 8);

void BorderView::setBorder() {
  rect_vertical_ = sf::RectangleShape(sf::Vector2f(
      {game_window_offset_.x, game_window_offset_.y + game_window_size_.y}));
  rect_horizontal_ = sf::RectangleShape(sf::Vector2f(
      {game_window_offset_.x + game_window_size_.x, game_window_offset_.y}));
  rect_vertical_.setFillColor(BACKGROUND_COLOR);
  rect_horizontal_.setFillColor(BACKGROUND_COLOR);
}

void BorderView::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  target.draw(rect_vertical_, states);
  target.draw(rect_horizontal_, states);
}