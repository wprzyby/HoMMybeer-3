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

#ifndef SRC_GAME_ENGINE_MAP_WINDOW_CONTROLLER_H_
#define SRC_GAME_ENGINE_MAP_WINDOW_CONTROLLER_H_

#include <Controller.h>
#include <Game.h>
#include <HeroView.h>
#include <MapView.h>
#include <ObjectsView.h>
#include <PathView.h>

#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <cmath>
#include <string>

#include "ControlsView.h"
#include "views/ResourcesView.h"

class MapView;

class MapWindowController : public Controller {
 private:
  inline static const int SCROLL_STEP = 16;

  sf::Vector2u map_pixel_offset_;
  sf::Vector2u game_window_size_;
  sf::Vector2u game_window_offset_;
  MapView* map_view_;
  HeroView* hero_view_;
  ObjectsView* objects_view_;
  PathView* path_view_;
  ControlsView* controlls_view_;
  ResourcesView* resources_view_;
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
                      sf::Vector2u game_window_offset, MapView* map_view,
                      HeroView* hero_view, ObjectsView* objects_view,
                      PathView* path_view, ControlsView* controlls_view,
                      ResourcesView* resources_view)
      : map_pixel_offset_(sf::Vector2u({0, 0})),
        game_window_size_(game_window_size),
        game_window_offset_(game_window_offset),
        map_view_(map_view),
        hero_view_(hero_view),
        objects_view_(objects_view),
        path_view_(path_view),
        controlls_view_(controlls_view),
        resources_view_(resources_view) {}

  void scrollGameView(const sf::Vector2i& translation, const Game& game);

  void changeMapContents(const Game& game);

  void repositionCamera(const Game& game);

  static bool loadFont(const std::string& path) {
    return font.loadFromFile(path);
  }

  void update(sf::Event& event, SessionState session_state,
              Game& game) override;
  inline static sf::Font font;
};

#endif