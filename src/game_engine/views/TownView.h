#ifndef SRC_GAME_ENGINE_VIEWS_TOWN_VIEW_H
#define SRC_GAME_ENGINE_VIEWS_TOWN_VIEW_H

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <optional>

#include "Config.h"

class TownView : public sf::Drawable, public sf::Transformable {
  const static sf::Vector2f BUTTON_SIZE;
  const static sf::Vector2f LEFT_BUTTON_POSITION;
  constexpr static int BUTTON_SEPARATION = 20;
  constexpr static int CHARACTER_SIZE = 25;
  const static sf::Color BUTTON_COLOR;
  sf::Texture background_texture_;
  sf::Sprite background_sprite_;
  sf::Vector2i background_size_;
  sf::Vector2u window_size_;
  sf::RectangleShape buy_lvl1_button_;
  sf::RectangleShape buy_lvl2_button_;
  sf::RectangleShape buy_lvl3_button_;
  sf::RectangleShape buy_hero_button_;
  sf::Text buy_lvl1_text_;
  sf::Text buy_lvl2_text_;
  sf::Text buy_lvl3_text_;
  sf::Text buy_hero_text_;
  sf::Font font_;

  bool loadBackground(const std::string& texture_path);
  void loadButtons();

  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

 public:
  TownView(sf::Vector2u window_size, const sf::Font& font)
      : window_size_(window_size), font_(font) {}

  void loadTown(Faction faction);
  bool buyHeroClicked(sf::Vector2f clicked_coords) const {
    return buy_hero_button_.getGlobalBounds().contains(clicked_coords);
  }
  std::optional<int> buyUnitClicked(sf::Vector2f clicked_coords) const;
};

#endif  // SRC_GAME_ENGINE_VIEWS_TOWN_VIEW_H