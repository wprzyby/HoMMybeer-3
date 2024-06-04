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

#include <BorderView.h>
#include <Controller.h>
#include <Game.h>
#include <HeroView.h>
#include <MapView.h>
#include <ObjectsView.h>
#include <PathView.h>

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/System/Vector2.hpp>
#include <cmath>
#include <string>

#include "ControlsView.h"
#include "views/ResourcesView.h"

class MapView;

class MapWindowController : public Controller, public sf::Drawable {
 private:
  inline static const int SCROLL_STEP = 16;

  sf::Vector2u map_pixel_offset_;
  sf::Vector2u game_window_size_;
  sf::Vector2u game_window_offset_;
  MapView map_view_;
  HeroView hero_view_;
  ObjectsView objects_view_;
  PathView path_view_;
  ControlsView controlls_view_;
  ResourcesView resources_view_;
  BorderView border_view_;
  inline static sf::Font font_;
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

  void selfInit();

 public:
  MapWindowController(sf::Vector2u game_window_size,
                      sf::Vector2u game_window_offset,
                      sf::Vector2u main_window_size)
      : map_pixel_offset_(sf::Vector2u({0, 0})),
        game_window_size_(game_window_size),
        game_window_offset_(game_window_offset),
        controlls_view_(ControlsView(sf::Vector2f(main_window_size),
                                     sf::Vector2f(game_window_size),
                                     sf::Vector2f(game_window_offset), font_)),
        resources_view_(ResourcesView(sf::Vector2f(main_window_size),
                                      sf::Vector2f(game_window_size),
                                      sf::Vector2f(game_window_offset), font_)),
        border_view_(BorderView(sf::Vector2f(game_window_offset),
                                sf::Vector2f(game_window_size))) {}

  void scrollGameView(const sf::Vector2i& translation, const Game& game);

  void changeMapContents(const Game& game);

  void repositionCamera(const Game& game);

  static bool loadFont(const std::string& path) {
    return font_.loadFromFile(path);
  }

  void update(sf::Event& event, SessionState session_state,
              Game& game) override;

  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

#endif