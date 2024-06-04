/**
 * @file PathView.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2024-06-03
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef SRC_GAME_ENGINE_VIEWS_PATH_VIEW_H_
#define SRC_GAME_ENGINE_VIEWS_PATH_VIEW_H_

#include <Hero.h>

#include <SFML/Graphics.hpp>

class PathView : public sf::Drawable, public sf::Transformable {
 public:
  PathView() = default;
  void setPath(const Hero* hero, sf::Vector2u field_offset,
               sf::Vector2u tile_size);
  static bool visible(FieldCoords point_on_path, sf::Vector2u field_offset);

 private:
  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
  sf::VertexArray vertices_;
};

#endif