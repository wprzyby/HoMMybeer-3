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
#include <map>

class MapView : public sf::Drawable, public sf::Transformable {
  constexpr static int kFieldScale = 5;

 public:
  MapView(int window_width, int window_height)
      : window_width_(window_width), window_height_(window_height) {}
  ~MapView() = default;

  bool loadTileset(const std::string& tileset_path, sf::Vector2u tileSize,
                   const int num_of_tiles);
  void loadFieldArray(FieldArray_t field_array);

 private:
  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
  sf::Texture tileset_;
  int num_of_tiles_;
  sf::Vector2u tile_size_;
  sf::VertexArray vertices_;
  int window_width_;
  int window_height_;
};

#endif  // SRC_GAME_ENGINE_VIEWS_MAP_VIEW_H_