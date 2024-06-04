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

#include <MapUtils.h>
#include <MapWindowController.h>
#include <Session.h>

#include <SFML/System/Vector2.hpp>
#include <cmath>

#include "Hero.h"
#include "MapView.h"

void MapWindowController::scrollGameView(const sf::Vector2i& translation,
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
    map_view_->setMap(game.getMap()->getFieldArray(), new_field_offset,
                      MapView::MAP_TILE_SIZE);

    hero_view_->setHeroes(game.heroesInGame(), new_field_offset,
                          MapView::MAP_TILE_SIZE);
    objects_view_->setObjects(game.objectsInGame(), new_field_offset,
                              MapView::MAP_TILE_SIZE);
    path_view_->setPath(game.getCurrentPlayer()->getCurrentHero(),
                        new_field_offset, MapView::MAP_TILE_SIZE);
  }
  map_view_->setOrigin(sf::Vector2f(getPartialPixelOffset()));
  hero_view_->setOrigin(sf::Vector2f(getPartialPixelOffset()));
  objects_view_->setOrigin(sf::Vector2f(getPartialPixelOffset()));
  path_view_->setOrigin(sf::Vector2f(getPartialPixelOffset()));
  map_view_->setPosition(sf::Vector2f(game_window_offset_));
  hero_view_->setPosition(sf::Vector2f(game_window_offset_));
  objects_view_->setPosition(sf::Vector2f(game_window_offset_));
  path_view_->setPosition(sf::Vector2f(game_window_offset_));
}

void MapWindowController::changeMapContents(const Game& game) {
  hero_view_->setHeroes(game.heroesInGame(), getFieldOffset(),
                        MapView::MAP_TILE_SIZE);
  objects_view_->setObjects(game.objectsInGame(), getFieldOffset(),
                            MapView::MAP_TILE_SIZE);
  path_view_->setPath(game.getCurrentPlayer()->getCurrentHero(),
                      getFieldOffset(), MapView::MAP_TILE_SIZE);
  hero_view_->setOrigin(sf::Vector2f(getPartialPixelOffset()));
  objects_view_->setOrigin(sf::Vector2f(getPartialPixelOffset()));
  path_view_->setOrigin(sf::Vector2f(getPartialPixelOffset()));
  hero_view_->setPosition(sf::Vector2f(game_window_offset_));
  objects_view_->setPosition(sf::Vector2f(game_window_offset_));
  path_view_->setPosition(sf::Vector2f(game_window_offset_));
  resources_view_->setResources(game.getCurrentPlayer(), game.getWeekday());
}

void MapWindowController::repositionCamera(const Game& game) {
  FieldCoords center_coords =
      game.getCurrentPlayer()->getCurrentHero()->getHeroCoords();
  sf::Vector2i new_offset = {
      static_cast<int>(
          (center_coords.second - (MapView::VISIBLE_MAP_WIDTH / 2)) *
          MapView::MAP_TILE_SIZE.x),
      static_cast<int>(
          (center_coords.first - (MapView::VISIBLE_MAP_WIDTH / 2)) *
          MapView::MAP_TILE_SIZE.y)};
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
  sf::Vector2i translation = new_offset - sf::Vector2i(map_pixel_offset_);
  scrollGameView(translation, game);
}

void MapWindowController::selfInit() {
  MapInfo map_info = generateLargeExampleMap();
  std::vector<Player> players{
      Player(false, Faction::CASTLE, map_info.starting_locations[0]),
      Player(false, Faction::INFERNO, FieldCoords{45, 47})};
  std::vector<std::shared_ptr<MapObject>> static_map_objects =
      generateExampleStaticObjects();
  std::vector<std::shared_ptr<MapObject>> pickable_map_objects =
      generateExamplePickableObjects();
  std::vector<std::shared_ptr<MapObject>> starting_objects(
      static_map_objects.size() + pickable_map_objects.size());
  std::merge(static_map_objects.begin(), static_map_objects.end(),
             pickable_map_objects.begin(), pickable_map_objects.end(),
             starting_objects.begin());
  Session::getInstance()->newGame(map_info.map, players, Difficulty::EASY,
                                  starting_objects);

  map_view_->setMap(Session::getInstance()->game.getMap()->getFieldArray(),
                    {0, 0}, MapView::MAP_TILE_SIZE);

  hero_view_->setHeroes(Session::getInstance()->game.heroesInGame(), {0, 0},
                        MapView::MAP_TILE_SIZE);
  objects_view_->setObjects(Session::getInstance()->game.objectsInGame(),
                            {0, 0}, MapView::MAP_TILE_SIZE);
  path_view_->setPath(
      Session::getInstance()->game.getCurrentPlayer()->getCurrentHero(), {0, 0},
      MapView::MAP_TILE_SIZE);
  changeMapContents(Session::getInstance()->game);
  repositionCamera(Session::getInstance()->game);

  controlls_view_->setControls();
}

void MapWindowController::update(sf::Event& event, SessionState session_state,
                                 Game& game) {
  if (session_state == SessionState::LOAD_GAME) {
    selfInit();
  }

  if (session_state != SessionState::IN_GAME) {
    return;
  }

  if (event.type == sf::Event::KeyPressed) {
    sf::Vector2i translation;
    if (event.key.scancode == sf::Keyboard::Scan::Right) {
      translation = {SCROLL_STEP, 0};
    } else if (event.key.scancode == sf::Keyboard::Scan::Left) {
      translation = {-SCROLL_STEP, 0};
    } else if (event.key.scancode == sf::Keyboard::Scan::Up) {
      translation = {0, -SCROLL_STEP};
    } else if (event.key.scancode == sf::Keyboard::Scan::Down) {
      translation = {0, SCROLL_STEP};
    }
    scrollGameView(translation, game);
  }
  if (event.type == sf::Event::MouseButtonPressed) {
    if (event.mouseButton.button == sf::Mouse::Left) {
      sf::Vector2i where_clicked{event.mouseButton.x, event.mouseButton.y};
      if (controlls_view_->nextHeroClicked(sf::Vector2f(where_clicked))) {
        game.getCurrentPlayer()->nextHero();
        repositionCamera(game);
      }
      if (controlls_view_->nextTurnClicked(sf::Vector2f(where_clicked))) {
        game.nextPlayer();
        resources_view_->setResources(game.getCurrentPlayer(),
                                      game.getWeekday());
        repositionCamera(game);
      }
      if (controlls_view_->exitClicked(sf::Vector2f(where_clicked))) {
        Session::getInstance()->setSessionState(SessionState::START_MENU);
        // TODO popup if sure to exit
      }
      if (where_clicked.x < game_window_offset_.x ||
          where_clicked.y < game_window_offset_.y ||
          where_clicked.x > game_window_offset_.x + game_window_size_.x ||
          where_clicked.y > game_window_offset_.y + game_window_size_.y) {
        return;
      }
      auto clicked_to_field_coords = [&](sf::Vector2i& clicked) {
        FieldCoords retval;
        retval.second = static_cast<int>(
            std::floor(static_cast<double>(clicked.x - game_window_offset_.x +
                                           map_pixel_offset_.x) /
                       static_cast<double>(MapView::MAP_TILE_SIZE.x)));
        retval.first = static_cast<int>(
            std::floor(static_cast<double>(clicked.y - game_window_offset_.y +
                                           map_pixel_offset_.y) /
                       static_cast<double>(MapView::MAP_TILE_SIZE.y)));
        return retval;
      };
      game.executeAction(clicked_to_field_coords(where_clicked));
      changeMapContents(game);
    }
  }
}