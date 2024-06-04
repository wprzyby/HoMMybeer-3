/**
 * @file BattleUnitsView.cc
 * @author Wojciech Przybylski
 * @brief View which draws all units on the battleground
 * @copyright Copyright (c) 2024
 */

#include "BattleUnitsView.h"

#include <SFML/Graphics.hpp>

#include <Config.h>
#include <game_logic_utils.h>

BattleUnitsView::BattleUnitsView() {
  auto* config = Config::getInstance();
  auto metadata = config->getBattleMetadata();
  for (auto [name, coords] : metadata["units_texture_origins"].items()) {
    unit_texture_origins_[name] =
        sf::Vector2f(coords[0].get<float>(), coords[1].get<float>());
  }
  single_unit_texture_size_ =
      sf::Vector2i(metadata["single_unit_texture_size"][0].get<int>(),
                   metadata["single_unit_texture_size"][1].get<int>());
  auto texture_path = getProjectPath() + "/" +
                      metadata["units_texture_file"].get<std::string>();
  if (not font_.loadFromFile(getProjectPath() + "/assets/fonts/arial.ttf")) {
    throw std::runtime_error("Could not load font");
  }
  loadTexture(texture_path);
}

void BattleUnitsView::loadUnits(
    const std::vector<std::pair<UnitBlock, sf::Vector2f>>& units_and_positions,
    bool facing_left, float hex_outer_radius) {
  constexpr static int TEXT_POSITION_VERTICAL_OFFSET = -10;
  constexpr static unsigned int TEXT_SIZE = 20;
  for (const auto& [unit, position] : units_and_positions) {
    sf::Sprite unit_sprite;
    sf::Text unit_count_text;
    unit_sprite.setTexture(units_texture_);
    unit_count_text.setFont(font_);
    unit_count_text.setString(std::to_string(unit.unit_count));
    unit_count_text.setFillColor(sf::Color::Red);
    unit_count_text.setStyle(sf::Text::Bold);
    unit_count_text.setCharacterSize(TEXT_SIZE);
    unit_count_text.setPosition({position.x + single_unit_texture_size_.x / 2,
                                 position.y + TEXT_POSITION_VERTICAL_OFFSET});

    auto unit_texture_origin = unit_texture_origins_[unit.name];
    unit_sprite.setTextureRect(
        sf::IntRect(unit_texture_origin.x, unit_texture_origin.y,
                    single_unit_texture_size_.x, single_unit_texture_size_.y));
    unit_sprite.setOrigin(0, single_unit_texture_size_.y);
    if (facing_left) {
      unit_sprite.setPosition({position.x + 2 * hex_outer_radius, position.y});
      unit_sprite.setScale(-1, 1);
    } else {
      unit_sprite.setPosition(position);
    }
    unit_counts_.push_back(unit_count_text);
    unit_sprites_.push_back(unit_sprite);
  }
}

void BattleUnitsView::clearUnits() {
  unit_sprites_.clear();
  unit_counts_.clear();
}

bool BattleUnitsView::loadTexture(const std::string& texture_path) {
  return units_texture_.loadFromFile(texture_path);
}

void BattleUnitsView::draw(sf::RenderTarget& target,
                           sf::RenderStates states) const {
  states.transform *= getTransform();
  for (const auto& unit_sprite : unit_sprites_) {
    target.draw(unit_sprite, states);
  }
  for (const auto& unit_count_text : unit_counts_) {
    target.draw(unit_count_text, states);
  }
}