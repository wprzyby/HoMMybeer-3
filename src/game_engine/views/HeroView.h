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

#include <Session.h>

#include <SFML/Graphics.hpp>
#include <string>

class HeroView : public sf::Drawable, public sf::Transformable {
 private:
  inline static sf::Texture heroes_;
  static const int icon_height_ = 64;
  static const int icon_width_ = 96;
  sf::Sprite sprite_;
  int window_width_;
  int window_height_;
  int map_width_;

 public:
  HeroView(int window_width, int window_height)
      : sprite_(sf::Sprite()),
        window_width_(window_width),
        window_height_(window_height),
        map_width_(Session::getInstance()->game->getMap()->getWidth()) {}
  ~HeroView() = default;
  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
  bool loadTileSet(const std::string& tileset_path);
  void setHero(const Hero& hero);
};

#endif