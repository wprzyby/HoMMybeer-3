/**
 * @file HeroView.h
 * @author Piotr Kluba
 * @brief Drawable hero management class
 * @date 2024-04-19
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef SRC_GAME_ENGINE_VIEWS_HERO_VIEW_H_
#define SRC_GAME_ENGINE_VIEWS_HERO_VIEW_H_

#include <Game.h>
#include <Hero.h>
#include <MapView.h>
#include <Session.h>

#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <string>

class HeroView : public sf::Drawable, public sf::Transformable {
 private:
  inline static sf::Texture heroes_texture_;
  static const int ICON_HEIGHT = 64;
  static const int ICON_WIDTH = 96;
  static const sf::Vector2f HERO_POSITION_OFFSET;
  std::vector<sf::Sprite> sprites_;
  static bool visible(FieldCoords hero_coords, sf::Vector2u field_offset);

 public:
  explicit HeroView() = default;
  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
  static bool loadTileSet(const std::string& tileset_path);
  void setHeroes(std::vector<const Hero*> heroes, sf::Vector2u field_offset,
                 sf::Vector2u tile_size);
};

#endif