/**
 * @file TownViewController.cc
 * @author Piotr Kluba
 * @brief Controller managing the window when the player is inside the town
 * @copyright Copyright (c) 2024
 */

#include "TownWindowController.h"

#include <Config.h>
#include <Hero.h>
#include <Session.h>

#include <SFML/Graphics.hpp>
#include <UnitBlockGenerator.hpp>

UnitBlockGenerator<UnitConfig> TownWindowController::setupUnitGenerator() {
  auto config = Config::getInstance()->getUnitConfig();
  return UnitBlockGenerator(config);
}

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
                Config::DEFAULT_HERO_NAMES.at(Session::getTown()), spawn_coords,
                game.getCurrentPlayer()->getFaction());
          }
        }
        if (town_view_.buyUnitClicked(sf::Vector2f(where_clicked))
                .has_value()) {
          if (game.getCurrentPlayer()->updateResourceQuantity(
                  ResourceType::GOLD,
                  -UNIT_COSTS.at(
                      town_view_.buyUnitClicked(sf::Vector2f(where_clicked))
                          .value()))) {
            game.getCurrentPlayer()->getCurrentHero()->addUnit(
                unit_generator_
                    .getUnitBlock(
                        Config::factionToUnitOriginTranslate(
                            Session::getTown()),
                        town_view_.buyUnitClicked(sf::Vector2f(where_clicked))
                            .value(),
                        1)
                    .value());
          }
        }
      }
    }
  }
}