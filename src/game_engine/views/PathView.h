/**
 * @file PathView.h
 * @author Piotr Kluba
 * @brief View for drawing the proposed movement path of the hero on the map
 * @copyright Copyright (c) 2024
 */

#ifndef SRC_GAME_ENGINE_VIEWS_PATH_VIEW_H
#define SRC_GAME_ENGINE_VIEWS_PATH_VIEW_H

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

#endif  // SRC_GAME_ENGINE_VIEWS_PATH_VIEW_H