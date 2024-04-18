#include "MapView.h"

bool MapView::loadTileset(const std::string& tileset_path,
                          sf::Vector2u tile_size, const int num_of_tiles) {
  if (!tileset_.loadFromFile(tileset_path)) {
    return false;
  }
  tile_size_ = tile_size;
  num_of_tiles_ = num_of_tiles;
  return true;
}

void MapView::loadFieldArray(FieldArray_t field_array) {
  int width = field_array.size();

  vertices_.setPrimitiveType(sf::Triangles);
  vertices_.resize(width * width * 6);

  for (int i = 0; i < width; ++i)
    for (int j = 0; j < width; ++j) {
      int tile_number = static_cast<int>(field_array[i][j].getTerrainType());

      // find its position in the tileset texture
      int tu = tile_number % (tileset_.getSize().x / tile_size_.x);
      int tv = tile_number / (tileset_.getSize().x / tile_size_.x);

      // get a pointer to the triangles' vertices of the current tile
      sf::Vertex* triangles = &vertices_[(i + j * width) * 6];

      // define the 6 corners of the two triangles
      triangles[0].position = sf::Vector2f(i * tile_size_.x, j * tile_size_.y);
      triangles[1].position =
          sf::Vector2f((i + 1) * tile_size_.x, j * tile_size_.y);
      triangles[2].position =
          sf::Vector2f(i * tile_size_.x, (j + 1) * tile_size_.y);
      triangles[3].position =
          sf::Vector2f(i * tile_size_.x, (j + 1) * tile_size_.y);
      triangles[4].position =
          sf::Vector2f((i + 1) * tile_size_.x, j * tile_size_.y);
      triangles[5].position =
          sf::Vector2f((i + 1) * tile_size_.x, (j + 1) * tile_size_.y);

      // define the 6 matching texture coordinates
      triangles[0].texCoords =
          sf::Vector2f(tu * tile_size_.x, tv * tile_size_.y);
      triangles[1].texCoords =
          sf::Vector2f((tu + 1) * tile_size_.x, tv * tile_size_.y);
      triangles[2].texCoords =
          sf::Vector2f(tu * tile_size_.x, (tv + 1) * tile_size_.y);
      triangles[3].texCoords =
          sf::Vector2f(tu * tile_size_.x, (tv + 1) * tile_size_.y);
      triangles[4].texCoords =
          sf::Vector2f((tu + 1) * tile_size_.x, tv * tile_size_.y);
      triangles[5].texCoords =
          sf::Vector2f((tu + 1) * tile_size_.x, (tv + 1) * tile_size_.y);
    }
  this->setScale(static_cast<float>(window_width_) / (width * tile_size_.x),
                 static_cast<float>(window_height_) / (width * tile_size_.y));
}

void MapView::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  states.transform *= getTransform();
  // apply the tileset texture
  states.texture = &tileset_;

  // draw the vertex array
  target.draw(vertices_, states);
}