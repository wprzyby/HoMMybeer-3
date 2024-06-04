/**
 * @file BorderView.h
 * @author Piotr Kluba (you@domain.com)
 * @brief Static border class
 * @date 2024-06-02
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef SRC_GAME_ENGINE_VIEWS_BORDER_VIEW_H_
#define SRC_GAME_ENGINE_VIEWS_BORDER_VIEW_H_

#include <SFML/Graphics.hpp>

class BorderView : public sf::Drawable, public sf::Transformable {
  sf::RectangleShape rect_vertical_;
  sf::RectangleShape rect_horizontal_;
  sf::Vector2f game_window_size_;
  sf::Vector2f game_window_offset_;

 public:
  BorderView(sf::Vector2f game_window_offset, sf::Vector2f game_window_size)
      : game_window_size_(game_window_size),
        game_window_offset_(game_window_offset) {}
        
  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
  void setBorder();
};

#endif