#include "BattlegroundView.h"

#include <Config.h>

#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>
#include <stdexcept>

#include "combat_common.h"
#include "common.h"

BattlegroundView::BattlegroundView(combat::BattlegroundSize battleground_size,
                                   TerrainType terrain_type) {
  setupBattle(battleground_size, terrain_type);
}

bool BattlegroundView::setupBattle(combat::BattlegroundSize battleground_size,
                                   TerrainType terrain_type) {
  battleground_size_ = battleground_size;
  auto* config = Config::getInstance();
  auto metadata = config->getBattleMetadata();
  auto battle_view_size_data = metadata["battle_view_size"];
  battle_view_size_ = sf::Vector2i(battle_view_size_data[0].get<int>(),
                                   battle_view_size_data[1].get<int>());
  auto hex_grid_offset_data = metadata["hex_grid_origin_offset"];
  hex_grid_offset_ = sf::Vector2f(hex_grid_offset_data[0].get<float>(),
                                  hex_grid_offset_data[1].get<float>());
  hex_grid_view_.setPosition(hex_grid_offset_);
  battle_units_view_.setPosition(hex_grid_offset_);
  auto background_texture_size_data = metadata["background_texture_size"];
  background_texture_size_ =
      sf::Vector2i(background_texture_size_data[0].get<int>(),
                   background_texture_size_data[1].get<int>());
  auto control_bar_height = metadata["control_bar_height"].get<int>();
  scale_ = sf::Vector2f(
      static_cast<float>(battle_view_size_.x) / background_texture_size_.x,
      static_cast<float>(battle_view_size_.y - control_bar_height) /
          background_texture_size_.y);
  // hex_grid_view_.setScale(scale_);
  auto background_path = getProjectPath() + "/" +
                         metadata["battleground_backgrounds"]
                                 [config->enumToStringTranslate(terrain_type)]
                                     .get<std::string>();
  return loadBackground(background_path);
}

std::optional<HexFieldCoords> BattlegroundView::translateToHexCoords(
    int x_clicked, int y_clicked) const {
  return hex_grid_view_.translateToHexCoords(x_clicked - hex_grid_offset_.x,
                                             y_clicked - hex_grid_offset_.y);
}

void BattlegroundView::loadState(const combat::BattleState& state) {
  battle_units_view_.clearUnits();
  auto selected = state.getPossibleMoves();
  auto current_unit_coords =
      state.battleground_.getUnitCoords(state.current_unit_id_);
  if (not current_unit_coords.has_value()) {
    throw std::runtime_error("Failed to find current unit coordinates");
  }
  hex_grid_view_.loadGrid(battleground_size_, selected,
                          current_unit_coords.value());
  auto unit_ids = state.battleground_.getAliveUnitsIds();
  std::vector<std::pair<UnitBlock, sf::Vector2f>> units_and_positions_attacker;
  std::vector<std::pair<UnitBlock, sf::Vector2f>> units_and_positions_defender;
  for (const auto& unit_id : unit_ids) {
    auto coords_optional = state.battleground_.getUnitCoords(unit_id);
    if (not coords_optional.has_value()) {
      continue;
    }
    auto position =
        hex_grid_view_.translateToWindowCoords(coords_optional.value());
    auto unit_block = state.hero_units_.at(unit_id);
    if (unit_id.first == combat::HeroRole::ATTACKER) {
      units_and_positions_attacker.emplace_back(unit_block, position);
    } else {
      units_and_positions_defender.emplace_back(unit_block, position);
    }
  }
  battle_units_view_.loadUnits(units_and_positions_attacker, false,
                               hex_grid_view_.hex_outer_radius_);
  battle_units_view_.loadUnits(units_and_positions_defender, true,
                               hex_grid_view_.hex_outer_radius_);
}

bool BattlegroundView::loadBackground(const std::string& background_path) {
  if (!background_texture_.loadFromFile(background_path)) {
    return false;
  }
  background_sprite_.setTexture(background_texture_);
  background_sprite_.setTextureRect(sf::IntRect(
      0, 0, background_texture_size_.x, background_texture_size_.y));
  // background_sprite_.setScale(scale_);
  return true;
}

void BattlegroundView::draw(sf::RenderTarget& target,
                            sf::RenderStates states) const {
  states.transform *= getTransform();
  states.texture = &hex_grid_view_.hex_texture_;
  target.draw(background_sprite_, states);
  target.draw(hex_grid_view_, states);
  target.draw(battle_units_view_, states);
}
