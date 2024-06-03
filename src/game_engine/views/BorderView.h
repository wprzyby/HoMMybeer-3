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

 public:
  BorderView() = default;
  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
  void setBorder();
};

#endif