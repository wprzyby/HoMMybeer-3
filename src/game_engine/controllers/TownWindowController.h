/**
 * @file TownViewController.h
 * @author Piotr Kluba
 * @brief Controller managing the window when the player is inside the town
 * @copyright Copyright (c) 2024
 */

#ifndef SRC_GAME_ENGINE_CONTROLLERS_TOWN_WINDOW_CONTROLLER_H
#define SRC_GAME_ENGINE_CONTROLLERS_TOWN_WINDOW_CONTROLLER_H

#include <MainView.h>
#include <TownView.h>
#include <UnitConfig.h>

#include <SFML/Graphics.hpp>
#include <UnitBlockGenerator.hpp>

#include "Controller.h"

class TownWindowController : public Controller, public sf::Drawable {
 private:
  TownView town_view_;
  UnitBlockGenerator<UnitConfig> unit_generator_;
  inline static constexpr int HERO_COST = 2500;
  inline static const std::map<unsigned int, unsigned int> UNIT_COSTS = {
      {1, 60}, {2, 100}, {3, 200}};

  inline static sf::Font font_;
  static UnitBlockGenerator<UnitConfig> setupUnitGenerator();

 public:
  explicit TownWindowController(sf::Vector2u main_window_size)
      : town_view_(TownView(main_window_size, font_)),
        unit_generator_(setupUnitGenerator()) {}
  void update(sf::Event& event, SessionState session_state,
              Game& game) override;
  static bool loadFont(const std::string& path) {
    return font_.loadFromFile(path);
  }
  void draw(sf::RenderTarget& target,
            sf::RenderStates /*states*/) const override {
    target.draw(town_view_);
  }
};

#endif  // SRC_GAME_ENGINE_CONTROLLERS_TOWN_WINDOW_CONTROLLER_H