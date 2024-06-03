/**
 * @file ViewController.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2024-06-01
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef SRC_GAME_ENGINE_VIEWS_VIEW_CONTROLLER_H_
#define SRC_GAME_ENGINE_VIEWS_VIEW_CONTROLLER_H_

#include <Game.h>
#include <HeroView.h>
#include <MapView.h>
#include <ObjectsView.h>

#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <cmath>

class MapView;

class MapWindowController {
 private:
  sf::Vector2u map_pixel_offset_;
  sf::Vector2u game_window_size_;
  sf::Vector2u game_window_offset_;
  sf::Vector2u getFieldOffset() const {
    return sf::Vector2u({static_cast<unsigned int>(std::floor(
                             map_pixel_offset_.x / MapView::MAP_TILE_SIZE.x)),
                         static_cast<unsigned int>(std::floor(
                             map_pixel_offset_.y / MapView::MAP_TILE_SIZE.y))});
  }
  sf::Vector2u getPartialPixelOffset() const {
    return sf::Vector2u({map_pixel_offset_.x % MapView::MAP_TILE_SIZE.x,
                         map_pixel_offset_.y % MapView::MAP_TILE_SIZE.y});
  }

 public:
  MapWindowController(sf::Vector2u game_window_size,
                      sf::Vector2u game_window_offset)
      : map_pixel_offset_(sf::Vector2u({0, 0})),
        game_window_size_(game_window_size),
        game_window_offset_(game_window_offset) {}

  void scrollGameView(const sf::Vector2i& translation, MapView& map_view,
                      HeroView& hero_view, ObjectsView& objects_view,
                      const Game& game);
};

#endif