/**
 * @file MapView.h
 * @author Wojciech Przybylski
 * @brief Drawable Map view management class
 * @copyright Copyright (c) 2024
 */

#ifndef SRC_GAME_ENGINE_VIEWS_MAP_VIEW_H_
#define SRC_GAME_ENGINE_VIEWS_MAP_VIEW_H_

#include <Field.h>
#include <Map.h>

#include <SFML/Graphics.hpp>

class MapView : public sf::Drawable, public sf::Transformable {
 public:
  MapView() = default;

  static bool loadTileset(const std::string& tileset_path);
  void setMap(FieldArray_t field_array, sf::Vector2u field_offset,
              sf::Vector2u tile_size);
  static const sf::Vector2u MAP_TILE_SIZE;
  constexpr static int VISIBLE_MAP_WIDTH = 16;

 private:
  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
  inline static sf::Texture tileset_;
  sf::VertexArray vertices_;
  static constexpr unsigned int CORNERS_IN_TRIANGLE = 6;
  static constexpr unsigned int TRIANGLES_IN_SQUARE = 6;
};

#endif  // SRC_GAME_ENGINE_VIEWS_MAP_VIEW_H_