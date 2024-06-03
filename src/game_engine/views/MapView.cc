/**
 * @file MapView.cc
 * @author Wojciech Przybylski
 * @brief Method definitions for MapView class
 * @copyright Copyright (c) 2024
 */

#include <MapView.h>

#include <cmath>

const sf::Vector2u MapView::MAP_TILE_SIZE = sf::Vector2u{32, 32};

bool MapView::loadTileset(const std::string& tileset_path) {
  return tileset_.loadFromFile(tileset_path);
}

void MapView::setMap(FieldArray_t field_array, sf::Vector2u field_offset,
                     sf::Vector2u tile_size) {
  int width = (VISIBLE_MAP_WIDTH + 1);

  vertices_.setPrimitiveType(sf::Triangles);
  vertices_.resize(static_cast<long long>(width * width) * TRIANGLES_IN_SQUARE *
                   CORNERS_IN_TRIANGLE);

  for (int i = 0; i < width; ++i) {
    for (int j = 0; j < width; ++j) {
      int tile_number = static_cast<int>(
          field_array[j + field_offset.y][i + field_offset.x].getTerrainType());

      // find its position in the tileset texture
      int tu = tile_number % (tileset_.getSize().x / tile_size.x);
      int tv = tile_number / (tileset_.getSize().x / tile_size.x);

      // get a pointer to the triangles' vertices of the current tile
      sf::Vertex* triangles = &vertices_[(i + j * width) * TRIANGLES_IN_SQUARE *
                                         CORNERS_IN_TRIANGLE];

      // define the 6 corners of the two triangles
      triangles[0].position = sf::Vector2f(i * tile_size.x, j * tile_size.y);
      triangles[1].position =
          sf::Vector2f((i + 1) * tile_size.x, j * tile_size.y);
      triangles[2].position =
          sf::Vector2f(i * tile_size.x, (j + 1) * tile_size.y);
      triangles[3].position =
          sf::Vector2f(i * tile_size.x, (j + 1) * tile_size.y);
      triangles[4].position =
          sf::Vector2f((i + 1) * tile_size.x, j * tile_size.y);
      triangles[5].position =
          sf::Vector2f((i + 1) * tile_size.x, (j + 1) * tile_size.y);

      // define the 6 matching texture coordinates
      triangles[0].texCoords = sf::Vector2f(tu * tile_size.x, tv * tile_size.y);
      triangles[1].texCoords =
          sf::Vector2f((tu + 1) * tile_size.x, tv * tile_size.y);
      triangles[2].texCoords =
          sf::Vector2f(tu * tile_size.x, (tv + 1) * tile_size.y);
      triangles[3].texCoords =
          sf::Vector2f(tu * tile_size.x, (tv + 1) * tile_size.y);
      triangles[4].texCoords =
          sf::Vector2f((tu + 1) * tile_size.x, tv * tile_size.y);
      triangles[5].texCoords =
          sf::Vector2f((tu + 1) * tile_size.x, (tv + 1) * tile_size.y);
    }
  }
}

void MapView::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  states.transform *= getTransform();
  // apply the tileset texture
  states.texture = &tileset_;

  // draw the vertex array
  target.draw(vertices_, states);
}