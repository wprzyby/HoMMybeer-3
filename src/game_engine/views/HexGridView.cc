/**
 * @file HexGridView.cc
 * @author Wojciech Przybylski
 * @brief View for drawing the hexagonal grid on top of the battleground
 * background
 * @copyright Copyright (c) 2024
 */

#include "HexGridView.h"

#include <Config.h>
#include <combat_utils.h>
#include <game_logic_utils.h>

#include <SFML/Graphics.hpp>
#include <cmath>
#include <string>

#include "BattlegroundView.h"

HexGridView::HexGridView() {
  auto metadata = Config::getInstance()->getBattleMetadata();
  metadata = metadata["hex_tile_texture"];
  hex_outer_radius_ = metadata["outer_radius"].get<float>();
  tile_types_horizontal_pixel_distance_ =
      metadata["horizontal_distance_between_types"].get<float>();
  auto texture_origin_data = metadata["origin"];
  texture_origin_ = sf::Vector2f(texture_origin_data[0].get<float>(),
                                 texture_origin_data[1].get<float>());
  auto texture_vertices_data = metadata["vertices"];
  for (auto hex_vertex_idx : std::views::iota(0U, HEX_NUMBER_OF_VERTICES)) {
    texture_vertices_.at(hex_vertex_idx) =
        sf::Vector2f(texture_vertices_data[hex_vertex_idx][0].get<float>(),
                     texture_vertices_data[hex_vertex_idx][1].get<float>());
  }
  loadTexture(getProjectPath() + "/" + metadata["file"].get<std::string>());
}
/**
 * @brief Function that employs a search algorithm to translate cartesian
 * coordinates of clicked pixel to specialized coordinate system used in the
 * hexagonal grid
 * @details The algorithm translated to the coordinate system specified here:
 * https://en.wikipedia.org/wiki/Hexagonal_Efficient_Coordinate_System# The
 * translation from hex coords to cartesian is trivial and uses matrix
 * multiplication, but the reverse is not so. The approach that the below
 * function follows is based on the fact that if the a coordinate is determined,
 * the rest can be determined by a simple solvable system of equations. The
 * algorithm thus searches every even row to check if it was clicked. It does so
 * by determining a function that bounds the clicked y coordinate if it were to
 * match the row. Then it checks if the coordinate is between the maximum upper
 * bound and its negative counterpart (between the top and bottom "teeth" of the
 * hexagon). If no even row is hit, it assumes that an odd row was hit,
 * calculates the rest of the coordinates and checks if the found coordinate is
 * viable.
 * @param x_clicked
 * @param y_clicked
 * @return std::optional<combat::HexFieldCoords>
 */
std::optional<combat::HexFieldCoords> HexGridView::translateToHexCoords(
    int x_clicked, int y_clicked) const {
  const static double HORIZONTAL_DISTANCE_BETWEEN_HEXES =
      hex_outer_radius_ * SQRT_3;
  const static double VERTICAL_DISTANCE_BETWEEN_HEXES = hex_outer_radius_ * 1.5;
  bool is_even_row_hit = false;
  for (auto even_row_idx :
       std::views::iota(0U, battleground_size_.even_rows_count)) {
    auto x_translated =
        x_clicked % (static_cast<int>(hex_outer_radius_ * SQRT_3));
    auto y_translated = -y_clicked + hex_outer_radius_ * (1 + 3 * even_row_idx);
    bool slope_negative = x_translated > hex_outer_radius_ * SIN_60_DEGREES;
    double y_upper_bound{};
    if (slope_negative) {
      y_upper_bound = -1 * x_translated / SQRT_3 + hex_outer_radius_ * 1.5;
    } else {
      y_upper_bound = x_translated / SQRT_3 + hex_outer_radius_ / 2;
    }
    if (y_translated < y_upper_bound and y_translated > -y_upper_bound) {
      is_even_row_hit = true;
      break;
    }
  }

  if (is_even_row_hit) {
    return combat::HexFieldCoords{0U, y_clicked / hex_outer_radius_ / 3,
                                  x_clicked / (hex_outer_radius_ * SQRT_3)};
  }
  auto second_coord = (y_clicked / hex_outer_radius_ - 1.5) / 3;
  auto third_coord = x_clicked / (hex_outer_radius_ * SQRT_3) - 0.5;
  if (second_coord < 0 or third_coord < 0) {
    return {};
  }
  return combat::HexFieldCoords{1U, second_coord, third_coord};
}

sf::Vector2f HexGridView::translateToWindowCoords(
    combat::HexFieldCoords coords) const {
  auto a = static_cast<float>(std::get<0>(coords));
  auto r = static_cast<float>(std::get<1>(coords));
  auto c = static_cast<float>(std::get<2>(coords));

  // convert to cartesian as shown
  // here: https://en.wikipedia.org/wiki/Hexagonal_Efficient_Coordinate_System#
  float x = a / 2 + c;
  float y = SQRT_3 * (a / 2 + r);
  x = x * hex_outer_radius_ * SQRT_3;
  y = y * hex_outer_radius_ * SQRT_3;
  y += 3 * hex_outer_radius_ / 2;
  return {x, y};
}

bool HexGridView::loadTexture(const std::string& hex_texture_path) {
  return hex_texture_.loadFromFile(hex_texture_path);
}

bool HexGridView::loadGrid(
    combat::BattlegroundSize size,
    const std::set<combat::HexFieldCoords>& selected_fields,
    combat::HexFieldCoords current_unit_coords) {
  battleground_size_ = size;
  const static double HORIZONTAL_DISTANCE_BETWEEN_HEXES =
      hex_outer_radius_ * SQRT_3;
  const static double VERTICAL_DISTANCE_BETWEEN_HEXES =
      hex_outer_radius_ * 3. / 2.;
  vertices_.setPrimitiveType(sf::Triangles);
  auto hex_count = size.even_rows_count * size.even_rows_size +
                   size.odd_rows_count * size.odd_rows_size;
  auto even_row_hexes_count = size.even_rows_count * size.even_rows_size;
  vertices_.resize(hex_count * VERTICES_IN_HEXAGON);

  auto add_hex_tiles = [&](unsigned int first_battleground_coords,
                           unsigned int rows_count, unsigned int rows_size) {
    for (auto row : std::views::iota(0U, rows_count)) {
      for (auto column : std::views::iota(0U, rows_size)) {
        HexFieldCoords battleground_coords{first_battleground_coords, row,
                                           column};
        bool is_selected = selected_fields.contains(battleground_coords);
        bool is_current_unit = current_unit_coords == battleground_coords;
        auto starting_vertex_idx =
            (row * rows_size + column +
             even_row_hexes_count * first_battleground_coords) *
            VERTICES_IN_HEXAGON;
        double origin_x = HORIZONTAL_DISTANCE_BETWEEN_HEXES *
                          (0.5 * (1. + first_battleground_coords) + column);
        double origin_y =
            hex_outer_radius_ + VERTICAL_DISTANCE_BETWEEN_HEXES *
                                    (row * 2. + first_battleground_coords);
        auto origin = sf::Vector2f(origin_x, origin_y);
        addHexTileToArray(is_selected, is_current_unit, starting_vertex_idx,
                          origin);
      }
    }
  };

  // setting up even rows
  add_hex_tiles(0, size.even_rows_count, size.even_rows_size);

  // setting up odd rows
  add_hex_tiles(1, size.odd_rows_count, size.odd_rows_size);

  return true;
}

void HexGridView::addHexTileToArray(bool is_selected, bool is_current_unit,
                                    unsigned int starting_vertex_idx,
                                    sf::Vector2f origin) {
  sf::Vertex* triangles = &vertices_[starting_vertex_idx];
  for (auto triangle_idx : std::views::iota(0U, TRIANGLES_IN_HEXAGON)) {
    auto vertex_idx = triangle_idx * VERTICES_IN_TRIANGLE;
    triangles[vertex_idx].position = origin;
    triangles[vertex_idx + 1].position = sf::Vector2f(
        origin.x + hex_outer_radius_ *
                       std::sin(SIXTY_DEGREE_ANGLE_RADIANS * triangle_idx),
        origin.y + hex_outer_radius_ *
                       std::cos(SIXTY_DEGREE_ANGLE_RADIANS * triangle_idx));
    triangles[vertex_idx + 2].position = sf::Vector2f(
        origin.x + hex_outer_radius_ * std::sin(SIXTY_DEGREE_ANGLE_RADIANS *
                                                (triangle_idx + 1)),
        origin.y + hex_outer_radius_ * std::cos(SIXTY_DEGREE_ANGLE_RADIANS *
                                                (triangle_idx + 1)));
    auto choose_tile_type = [&](sf::Vector2f coords) {
      if (is_current_unit) {
        return sf::Vector2f{
            coords.x + tile_types_horizontal_pixel_distance_ * 2, coords.y};
      }
      if (is_selected) {
        return sf::Vector2f{coords.x + tile_types_horizontal_pixel_distance_,
                            coords.y};
      }
      return coords;
    };
    triangles[vertex_idx].texCoords = choose_tile_type(texture_origin_);
    triangles[vertex_idx + 1].texCoords =
        choose_tile_type(texture_vertices_.at(triangle_idx));
    triangles[vertex_idx + 2].texCoords = choose_tile_type(
        texture_vertices_.at((triangle_idx + 1) % texture_vertices_.size()));
  }
}

void HexGridView::draw(sf::RenderTarget& target,
                       sf::RenderStates states) const {
  states.transform *= getTransform();
  states.texture = &hex_texture_;
  target.draw(vertices_, states);
}