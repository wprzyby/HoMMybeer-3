#ifndef SRC_GAME_ENGINE_VIEWS_HEX_GRID_VIEW_H
#define SRC_GAME_ENGINE_VIEWS_HEX_GRID_VIEW_H

#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>

#include "Battleground.h"
#include "combat_common.h"

constexpr static unsigned int HEX_NUMBER_OF_VERTICES = 6U;
constexpr static unsigned int VERTICES_IN_TRIANGLE = 3U;
constexpr static unsigned int TRIANGLES_IN_HEXAGON = 6U;
constexpr static unsigned int VERTICES_IN_HEXAGON =
    VERTICES_IN_TRIANGLE * TRIANGLES_IN_HEXAGON;
constexpr static double SIXTY_DEGREE_ANGLE_RADIANS =
    2. * std::numbers::pi_v<double> / 6.;
const static double SQRT_3 = std::sqrt(3.);

class BattlegroundView;

class HexGridView : public sf::Drawable, public sf::Transformable {
 public:
  HexGridView();
  bool loadTexture(const std::string& tileset_path);
  bool loadGrid(combat::BattlegroundSize size,
                const std::set<combat::HexFieldCoords>& selected_fields,
                combat::HexFieldCoords current_unit_coords);
  [[nodiscard]] std::optional<combat::HexFieldCoords> translateToHexCoords(
      int x_clicked, int y_clicked) const;
  [[nodiscard]] sf::Vector2f translateToWindowCoords(
      combat::HexFieldCoords coords) const;

 private:
  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
  void addHexTileToArray(bool is_selected, bool is_current_unit,
                         unsigned int starting_vertex_idx, sf::Vector2f origin);
  sf::VertexArray vertices_;
  sf::Texture hex_texture_;
  sf::Vector2f texture_origin_;
  std::array<sf::Vector2f, HEX_NUMBER_OF_VERTICES> texture_vertices_;
  double hex_outer_radius_;
  float tile_types_horizontal_pixel_distance_;
  combat::BattlegroundSize battleground_size_;
  friend class BattlegroundView;
};

#endif  // SRC_GAME_ENGINE_VIEWS_HEX_GRID_VIEW_H