/**
 * @file MapView.cc
 * @author Wojciech Przybylski
 * @brief View for drawing the map
 * @copyright Copyright (c) 2024
 */

#include "MapView.h"

#include <cmath>

const sf::Vector2u MapView::MAP_TILE_SIZE = sf::Vector2u{32, 32};

bool MapView::loadTileset(const std::string& tileset_path) {
  return tileset_.loadFromFile(tileset_path);
}

void MapView::setMap(FieldArray field_array, sf::Vector2u field_offset,
                     sf::Vector2u tile_size) {
  int width = (VISIBLE_MAP_WIDTH + 1);

  vertices_.setPrimitiveType(sf::Triangles);
  vertices_.resize(static_cast<long long>(width * width) * TRIANGLES_IN_SQUARE *
                   CORNERS_IN_TRIANGLE);

  // creating tile map vertex array according to an SFML guide that can be found
  // here: https://www.sfml-dev.org/tutorials/2.6/graphics-vertex-array.php
  for (int row : std::views::iota(0, width)) {
    for (int column : std::views::iota(0, width)) {
      int tile_number = static_cast<int>(
          field_array[column + field_offset.y][row + field_offset.x]
              .getTerrainType());

      // find its position in the tileset texture
      int texture_row = tile_number % (tileset_.getSize().x / tile_size.x);
      int texture_column = tile_number / (tileset_.getSize().x / tile_size.x);

      // get a pointer to the triangles' vertices of the current tile
      sf::Vertex* triangles =
          &vertices_[(row + column * width) * TRIANGLES_IN_SQUARE *
                     CORNERS_IN_TRIANGLE];

      // define the 6 corners of the two triangles
      triangles[0].position =
          sf::Vector2f(row * tile_size.x, column * tile_size.y);
      triangles[1].position =
          sf::Vector2f((row + 1) * tile_size.x, column * tile_size.y);
      triangles[2].position =
          sf::Vector2f(row * tile_size.x, (column + 1) * tile_size.y);
      triangles[3].position =
          sf::Vector2f(row * tile_size.x, (column + 1) * tile_size.y);
      triangles[4].position =
          sf::Vector2f((row + 1) * tile_size.x, column * tile_size.y);
      triangles[5].position =
          sf::Vector2f((row + 1) * tile_size.x, (column + 1) * tile_size.y);

      // define the 6 matching texture coordinates
      triangles[0].texCoords =
          sf::Vector2f(texture_row * tile_size.x, texture_column * tile_size.y);
      triangles[1].texCoords = sf::Vector2f((texture_row + 1) * tile_size.x,
                                            texture_column * tile_size.y);
      triangles[2].texCoords = sf::Vector2f(texture_row * tile_size.x,
                                            (texture_column + 1) * tile_size.y);
      triangles[3].texCoords = sf::Vector2f(texture_row * tile_size.x,
                                            (texture_column + 1) * tile_size.y);
      triangles[4].texCoords = sf::Vector2f((texture_row + 1) * tile_size.x,
                                            texture_column * tile_size.y);
      triangles[5].texCoords = sf::Vector2f((texture_row + 1) * tile_size.x,
                                            (texture_column + 1) * tile_size.y);
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