/**
 * @file BorderView.h
 * @author Piotr Kluba
 * @brief View defining the static border around the map
 * @copyright Copyright (c) 2024
 */

#ifndef SRC_GAME_ENGINE_VIEWS_BORDER_VIEW_H
#define SRC_GAME_ENGINE_VIEWS_BORDER_VIEW_H

#include <SFML/Graphics.hpp>

class BorderView : public sf::Drawable, public sf::Transformable {
  const static sf::Color BACKGROUND_COLOR;
  sf::RectangleShape rect_vertical_;
  sf::RectangleShape rect_horizontal_;
  sf::Vector2f game_window_size_;
  sf::Vector2f game_window_offset_;
  void setBorder();

 public:
  BorderView(sf::Vector2f game_window_offset, sf::Vector2f game_window_size)
      : game_window_size_(game_window_size),
        game_window_offset_(game_window_offset) {
    setBorder();
  }

  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

#endif // SRC_GAME_ENGINE_VIEWS_BORDER_VIEW_H