/**
 * @file ViewController.cc
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2024-06-01
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <MapWindowController.h>
#include <Session.h>

#include "MapView.h"

void MapWindowController::scrollGameView(const sf::Vector2i& translation,
                                         MapView& map_view, HeroView& hero_view,
                                         ObjectsView& objects_view,
                                         const Game& game) {
  sf::Vector2i new_offset = sf::Vector2i(map_pixel_offset_) + translation;
  new_offset.x =
      std::max(std::min(static_cast<int>(game.getMap()->getWidth() *
                                             MapView::MAP_TILE_SIZE.x -
                                         game_window_size_.x - 1),
                        new_offset.x),
               0);
  new_offset.y =
      std::max(std::min(static_cast<int>(game.getMap()->getWidth() *
                                             MapView::MAP_TILE_SIZE.y -
                                         game_window_size_.y - 1),
                        new_offset.y),
               0);
  sf::Vector2u old_field_offset = getFieldOffset();
  map_pixel_offset_ = sf::Vector2u(new_offset);
  sf::Vector2u new_field_offset = getFieldOffset();
  if (old_field_offset != new_field_offset) {
    map_view.setMap(game.getMap()->getFieldArray(), new_field_offset,
                    MapView::MAP_TILE_SIZE);

    hero_view.setHeroes(game.heroesInGame(), new_field_offset,
                        MapView::MAP_TILE_SIZE);
    objects_view.setObjects(game.objectsInGame(), new_field_offset,
                            MapView::MAP_TILE_SIZE);
  }
  map_view.setOrigin(sf::Vector2f(getPartialPixelOffset()));
  hero_view.setOrigin(sf::Vector2f(getPartialPixelOffset()));
  objects_view.setOrigin(sf::Vector2f(getPartialPixelOffset()));
  map_view.setPosition(sf::Vector2f(game_window_offset_));
  hero_view.setPosition(sf::Vector2f(game_window_offset_));
  objects_view.setPosition(sf::Vector2f(game_window_offset_));
}