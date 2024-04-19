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

void HeroView::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  states.transform *= getTransform();
  // apply the tileset texture
  states.texture = &HeroView::heroes_;

  // draw the vertex array
  target.draw(sprite_, states);
}

bool HeroView::loadTileSet(const std::string& tileset_path) {
  if (!HeroView::heroes_.loadFromFile(tileset_path)) {
    return false;
  }
  return true;
}

void HeroView::setHero(const Hero& hero) {
  int hero_pose = 1;
  int hero_number = 1;
  sprite_.setTexture(HeroView::heroes_);
  sprite_.setTextureRect(sf::IntRect(hero_pose * icon_width_,
                                     hero_number * icon_height_, icon_width_,
                                     icon_height_));
  sprite_.setOrigin(icon_width_ / 2.f, icon_height_ / 2.f);
  // sprite_.setScale()

  sprite_.setPosition(
      sf::Vector2f{(hero.getHeroCoords().first *
                    static_cast<float>(window_width_) / map_width_) +
                       icon_width_ / 2.f,
                   (hero.getHeroCoords().second *
                    static_cast<float>(window_height_) / map_width_) +
                       icon_height_ / 2.f});
}
