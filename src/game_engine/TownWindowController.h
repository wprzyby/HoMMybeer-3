/**
 * @file TownViewController.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2024-06-04
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef SRC_GAME_ENGINE_TOWN_WINDOW_CONTROLLER_H_
#define SRC_GAME_ENGINE_TOWN_WINDOW_CONTROLLER_H_

#include <Controller.h>
#include <MainView.h>
#include <TownView.h>
#include <UnitConfig.h>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/System/Vector2.hpp>
#include <UnitBlockGenerator.hpp>

class TownWindowController : public Controller, public sf::Drawable {
 private:
  TownView town_view_;
  UnitBlockGenerator<UnitConfig> unit_generator_;
  inline static constexpr int HERO_COST = 2500;
  inline static const std::map<unsigned int, unsigned int> UNIT_COSTS = {
      {1, 60}, {2, 100}, {3, 200}};

  inline static sf::Font font_;

 public:
  explicit TownWindowController(sf::Vector2u main_window_size,
                                const UnitConfig& unit_config)
      : town_view_(TownView(main_window_size, font_)),
        unit_generator_(unit_config) {}
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

#endif