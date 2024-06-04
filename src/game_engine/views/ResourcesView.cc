/**
 * @file ResourcesView.h
 * @author Piotr Kluba (you@domain.com)
 * @brief Entity for displaying resources quantities and incomes
 * @date 2024-06-02
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <ResourcesView.h>

void ResourcesView::setResources() {
  background_ = sf::RectangleShape(sf::Vector2f(
      {game_window_size_.x + game_window_offset_.x,
       window_size_.y - game_window_size_.y - game_window_offset_.y}));
  background_.setFillColor(sf::Color(82, 38, 8));
  background_.setPosition(
      sf::Vector2f({0, game_window_size_.y + game_window_offset_.y}));
}

void ResourcesView::draw(sf::RenderTarget& target,
                         sf::RenderStates states) const {
  target.draw(background_, states);
}