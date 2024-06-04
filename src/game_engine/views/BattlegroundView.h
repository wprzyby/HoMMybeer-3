/**
 * @file BattlegroundView.h
 * @author Wojciech Przybylski
 * @brief View aggregating all drawing functionality related to the battleground
 * @copyright Copyright (c) 2024
 */

#ifndef SRC_GAME_ENGINE_VIEWS_BATTLEGROUND_VIEW_H
#define SRC_GAME_ENGINE_VIEWS_BATTLEGROUND_VIEW_H

#include <BattleState.h>
#include <Battleground.h>
#include <Config.h>

#include <SFML/Graphics.hpp>

#include "BattleUnitsView.h"
#include "HexGridView.h"

using combat::HexFieldCoords;

class BattlegroundView : public sf::Drawable, public sf::Transformable {
 public:
  explicit BattlegroundView(combat::BattlegroundSize battleground_size,
                            TerrainType terrain_type);
  BattlegroundView() = default;

  [[nodiscard]] std::optional<HexFieldCoords> translateToHexCoords(
      int x_clicked, int y_clicked) const;

  void loadState(const combat::BattleState& state);
  bool setupBattle(combat::BattlegroundSize battleground_size,
                   TerrainType terrain_type);

 private:
  const static sf::Color CONTROL_BAR_COLOR;
  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
  bool loadBackground(const std::string& background_path);
  sf::Texture background_texture_;
  sf::Vector2i background_texture_size_;
  sf::Sprite background_sprite_;
  sf::RectangleShape bottom_bar_;
  combat::BattlegroundSize battleground_size_{};
  sf::Vector2f scale_;
  sf::Vector2i battle_view_size_;
  sf::Vector2f hex_grid_offset_;
  HexGridView hex_grid_view_;
  BattleUnitsView battle_units_view_;
  int control_bar_height_{};
};

#endif  // SRC_GAME_ENGINE_VIEWS_BATTLEGROUND_VIEW_H