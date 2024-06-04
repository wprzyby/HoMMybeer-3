/**
 * @file HeroView.h
 * @author Piotr Kluba
 * @brief Drawable hero management class
 * @date 2024-04-19
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <Hero.h>
#include <HeroView.h>
#include <Session.h>

#include <SFML/System/Vector2.hpp>

const sf::Vector2f HeroView::HERO_POSITION_OFFSET = sf::Vector2f{32, 32};

void HeroView::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  states.transform *= getTransform();
  // apply the tileset texture
  states.texture = &HeroView::heroes_texture_;

  // draw the vertex array
  for (const sf::Sprite& sprite : sprites_) {
    target.draw(sprite, states);
  }
}
bool HeroView::visible(FieldCoords hero_coords, sf::Vector2u field_offset) {
  return hero_coords.first >= field_offset.y &&
         hero_coords.second >= field_offset.x &&
         hero_coords.first <=
             field_offset.y + (MapView::VISIBLE_MAP_WIDTH + 1) &&
         hero_coords.second <=
             field_offset.x + (MapView::VISIBLE_MAP_WIDTH + 1);
}

bool HeroView::loadTileSet(const std::string& tileset_path) {
  return HeroView::heroes_texture_.loadFromFile(tileset_path);
}
void HeroView::setHeroes(std::vector<const Hero*> heroes,
                         sf::Vector2u field_offset, sf::Vector2u tile_size) {
  sprites_.clear();
  auto cmp = [](const Hero* left, const Hero* right) {
    if (left->getHeroCoords().second == right->getHeroCoords().second) {
      return left->getHeroCoords().first > right->getHeroCoords().first;
    }
    return left->getHeroCoords().second > right->getHeroCoords().second;
  };

  std::sort(heroes.begin(), heroes.end(), cmp);

  for (const Hero* hero : heroes) {
    FieldCoords hero_coords = hero->getHeroCoords();
    if (!visible(hero_coords, field_offset)) {
      continue;
    }
    int hero_pose = 1;
    int hero_number;
    switch (hero->getFaction()) {
      case Faction::CASTLE:
        hero_number = 1;
        break;
      case Faction::INFERNO:
        hero_number = 16;
        break;
      case Faction::FORTRESS:
        hero_number = 14;
        break;
    }
    sf::Sprite sprite(HeroView::heroes_texture_);
    sprite.setOrigin(HERO_POSITION_OFFSET);
    sprite.setTextureRect(sf::IntRect(hero_pose * ICON_WIDTH,
                                      hero_number * ICON_HEIGHT, ICON_WIDTH,
                                      ICON_HEIGHT));
    sprite.setPosition(sf::Vector2f(
        {static_cast<float>(
             (hero_coords.second - static_cast<int>(field_offset.x)) *
             static_cast<int>(tile_size.x)),
         static_cast<float>(
             (hero_coords.first - static_cast<int>(field_offset.y)) *
             static_cast<int>(tile_size.y))}));
    sprites_.push_back(sprite);
  }
}
