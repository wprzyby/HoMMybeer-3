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
#include "UnitBlockGenerator.hpp"

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
    map_view_.setMap(game.getMap()->getFieldArray(), new_field_offset,
                     MapView::MAP_TILE_SIZE);

    hero_view_.setHeroes(game.heroesInGame(), new_field_offset,
                         MapView::MAP_TILE_SIZE);
    objects_view_.setObjects(game.objectsInGame(), new_field_offset,
                             MapView::MAP_TILE_SIZE);
    path_view_.setPath(game.getCurrentPlayer()->getCurrentHero(),
                       new_field_offset, MapView::MAP_TILE_SIZE);
  }
  map_view_.setOrigin(sf::Vector2f(getPartialPixelOffset()));
  hero_view_.setOrigin(sf::Vector2f(getPartialPixelOffset()));
  objects_view_.setOrigin(sf::Vector2f(getPartialPixelOffset()));
  path_view_.setOrigin(sf::Vector2f(getPartialPixelOffset()));
  map_view_.setPosition(sf::Vector2f(game_window_offset_));
  hero_view_.setPosition(sf::Vector2f(game_window_offset_));
  objects_view_.setPosition(sf::Vector2f(game_window_offset_));
  path_view_.setPosition(sf::Vector2f(game_window_offset_));
}

void MapWindowController::changeMapContents(const Game& game) {
  hero_view_.setHeroes(game.heroesInGame(), getFieldOffset(),
                       MapView::MAP_TILE_SIZE);
  objects_view_.setObjects(game.objectsInGame(), getFieldOffset(),
                           MapView::MAP_TILE_SIZE);
  path_view_.setPath(game.getCurrentPlayer()->getCurrentHero(),
                     getFieldOffset(), MapView::MAP_TILE_SIZE);
  hero_view_.setOrigin(sf::Vector2f(getPartialPixelOffset()));
  objects_view_.setOrigin(sf::Vector2f(getPartialPixelOffset()));
  path_view_.setOrigin(sf::Vector2f(getPartialPixelOffset()));
  hero_view_.setPosition(sf::Vector2f(game_window_offset_));
  objects_view_.setPosition(sf::Vector2f(game_window_offset_));
  path_view_.setPosition(sf::Vector2f(game_window_offset_));
  resources_view_.setResources(game.getCurrentPlayer(), game.getWeekday());
  resources_view_.setUnitAmounts(game.getCurrentPlayer()->getCurrentHero());
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
  auto unit_config = Config::getInstance()->getUnitConfig();
  UnitBlockGenerator unit_generator(unit_config);

  MapInfo map_info = generateLargeExampleMap();
  std::vector<Player> players{
      Player(false, Faction::CASTLE, map_info.starting_locations[0]),
      Player(false, Faction::INFERNO, map_info.starting_locations[1])};
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

  map_view_.setMap(Session::getInstance()->game.getMap()->getFieldArray(),
                   {0, 0}, MapView::MAP_TILE_SIZE);

  hero_view_.setHeroes(Session::getInstance()->game.heroesInGame(), {0, 0},
                       MapView::MAP_TILE_SIZE);
  objects_view_.setObjects(Session::getInstance()->game.objectsInGame(), {0, 0},
                           MapView::MAP_TILE_SIZE);
  path_view_.setPath(
      Session::getInstance()->game.getCurrentPlayer()->getCurrentHero(), {0, 0},
      MapView::MAP_TILE_SIZE);
  changeMapContents(Session::getInstance()->game);
  repositionCamera(Session::getInstance()->game);

  controlls_view_.setControls();
}

void MapWindowController::update(sf::Event& event, SessionState session_state,
                                 Game& game) {
  if (session_state == SessionState::LOAD_GAME) {
    selfInit();
  }
  if (session_state == SessionState::REFRESH) {
    changeMapContents(game);
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
      if (controlls_view_.nextHeroClicked(sf::Vector2f(where_clicked))) {
        game.getCurrentPlayer()->nextHero();
        resources_view_.setUnitAmounts(
            game.getCurrentPlayer()->getCurrentHero());
        repositionCamera(game);
      }
      if (controlls_view_.nextTurnClicked(sf::Vector2f(where_clicked))) {
        game.nextPlayer();
        resources_view_.setResources(game.getCurrentPlayer(),
                                     game.getWeekday());
        resources_view_.setUnitAmounts(
            game.getCurrentPlayer()->getCurrentHero());
        repositionCamera(game);
      }
      if (controlls_view_.exitClicked(sf::Vector2f(where_clicked))) {
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
      auto field_coords = clicked_to_field_coords(where_clicked);
      auto hero_clicked = findHeroClicked(field_coords, game);
      if (hero_clicked.has_value()) {
        if (game.getCurrentPlayer()->getCurrentHero()->getUnits().size() > 0 and
            Field::isAdjacent(
                field_coords,
                game.getCurrentPlayer()->getCurrentHero()->getHeroCoords())) {
          Session::getInstance()->setSessionState(SessionState::LOAD_BATTLE);
          Session::getInstance()->setAttackedHeroInfo(hero_clicked.value());
          Session::getInstance()->setBattleTerrainType(
              game.getMap()->getField(field_coords).value()->getTerrainType());
        }
        return;
      }
      game.executeAction(field_coords);
      changeMapContents(game);
    }
  }
}

void MapWindowController::draw(sf::RenderTarget& target,
                               sf::RenderStates states) const {
  target.draw(map_view_);
  target.draw(path_view_);
  target.draw(objects_view_);
  target.draw(hero_view_);
  target.draw(controlls_view_);
  target.draw(resources_view_);
  target.draw(border_view_);
}
std::optional<std::pair<int, int>> MapWindowController::findHeroClicked(
    FieldCoords field_clicked, const Game& game) {
  for (auto player_idx : std::views::iota(0, game.numPlayers())) {
    for (auto hero_idx :
         std::views::iota(0, game.getPlayer(player_idx)->numHeroes())) {
      if (game.getPlayer(player_idx)->getHero(hero_idx)->getHeroCoords() ==
          field_clicked) {
        return std::pair{player_idx, hero_idx};
      }
    }
  }
  return {};
}