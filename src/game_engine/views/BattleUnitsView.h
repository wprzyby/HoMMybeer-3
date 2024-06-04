#ifndef SRC_GAME_ENGINE_VIEWS_BATTLE_UNITS_VIEW_H
#define SRC_GAME_ENGINE_VIEWS_BATTLE_UNITS_VIEW_H

#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>

#include "UnitBlock.hpp"

class BattleUnitsView : public sf::Drawable, public sf::Transformable {
 public:
  BattleUnitsView();
  void loadUnits(const std::vector<std::pair<UnitBlock, sf::Vector2f>>&
                     units_and_positions,
                 bool facing_left, float hex_outer_radius);
  void clearUnits();

 private:
  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
  bool loadTexture(const std::string& texture_path);
  std::vector<sf::Sprite> unit_sprites_;
  std::vector<sf::Text> unit_counts_;
  sf::Texture units_texture_;
  std::map<std::string, sf::Vector2f> unit_texture_origins_;
  sf::Vector2i single_unit_texture_size_;
  sf::Font font_;
};

#endif  // SRC_GAME_ENGINE_VIEWS_BATTLE_UNITS_VIEW_H