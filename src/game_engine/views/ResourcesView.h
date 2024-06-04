/**
 * @file ControlsView.h
 * @author Piotr Kluba (you@domain.com)
 * @brief Entity for displaying resources quantities and incomes
 * @date 2024-06-02
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef SRC_GAME_ENGINE_VIEWS_RESOURCES_VIEW_H_
#define SRC_GAME_ENGINE_VIEWS_RESOURCES_VIEW_H_

#include <SFML/Graphics.hpp>

class ResourcesView : public sf::Drawable, public sf::Transformable {
 private:
  sf::RectangleShape background_;
  sf::Vector2f window_size_;
  sf::Vector2f game_window_size_;
  sf::Vector2f game_window_offset_;

 public:
  ResourcesView(sf::Vector2f window_size, sf::Vector2f game_window_size,
                sf::Vector2f game_window_offset)
      : window_size_(window_size),
        game_window_size_(game_window_size),
        game_window_offset_(game_window_offset) {}
  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
  void setResources();
};

#endif