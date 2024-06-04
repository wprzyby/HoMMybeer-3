/**
 * @file TownViewController.cc
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2024-06-04
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <TownWindowController.h>

#include <SFML/System/Vector2.hpp>

#include "Config.h"
#include "Hero.h"
#include "Session.h"

void TownWindowController::update(sf::Event& event, SessionState session_state,
                                  Game& game) {
  if (session_state == SessionState::LOAD_CASTLE) {
    town_view_.loadTown(Session::getTown());
    Session::setSessionState(SessionState::IN_CASTLE);
  }
  if (session_state == SessionState::IN_CASTLE) {
    if (event.type == sf::Event::KeyPressed) {
      if (event.key.scancode == sf::Keyboard::Scan::Escape) {
        Session::setSessionState(SessionState::REFRESH);
      }
    }
    if (event.type == sf::Event::MouseButtonPressed) {
      if (event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2i where_clicked{event.mouseButton.x, event.mouseButton.y};
        if (town_view_.buyHeroClicked(sf::Vector2f(where_clicked))) {
          if (game.getCurrentPlayer()->updateResourceQuantity(
                  ResourceType::GOLD, -HERO_COST)) {
            FieldCoords spawn_coords =
                game.getCurrentPlayer()->getCurrentHero()->getHeroCoords() +
                FieldCoords{1, 0};
            game.getCurrentPlayer()->addHero(
                Config::kDefaultHeroNames.at(Session::getTown()), spawn_coords,
                game.getCurrentPlayer()->getFaction());
          }
        }
        if (town_view_.buyUnitClicked(sf::Vector2f(where_clicked))
                .has_value()) {
          return;
        }
      }
    }
  }
}