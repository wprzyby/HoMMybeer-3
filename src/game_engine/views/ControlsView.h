/**
 * @file ControlsView.h
 * @author Piotr Kluba
 * @brief Entity representing buttons used to control the game
 * @copyright Copyright (c) 2024
 */

#ifndef SRC_GAME_ENGINE_VIEWS_CONTROLS_VIEW_H_
#define SRC_GAME_ENGINE_VIEWS_CONTROLS_VIEW_H_

#include <SFML/Graphics.hpp>

class ControlsView : public sf::Drawable, public sf::Transformable {
 private:
  inline static const sf::Vector2f BUTT_SIZE{200, 130};
  inline static const int BUTT_SPACER = 20;
  inline static const int OUTLINE_THICKNESS = 3;
  inline static const sf::Color BUTTON_COLOR = sf::Color(56, 37, 13);
  inline static const sf::Color BACKGROUND_COLOR = sf::Color(82, 38, 8);
  static constexpr unsigned int CHARACTER_SIZE = 30;
  sf::RectangleShape background_;
  sf::RectangleShape next_hero_button_;
  sf::RectangleShape next_turn_button_;
  sf::RectangleShape exit_button_;
  sf::Text next_hero_text_;
  sf::Text next_turn_text_;
  sf::Text exit_text_;
  sf::Vector2f window_size_;
  sf::Vector2f game_window_size_;
  sf::Vector2f game_window_offset_;
  sf::Font font_;

 public:
  ControlsView(sf::Vector2f window_size, sf::Vector2f game_window_size,
               sf::Vector2f game_window_offset, const sf::Font& font)
      : window_size_(window_size),
        game_window_size_(game_window_size),
        game_window_offset_(game_window_offset),
        font_(font) {}
  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
  void setControls();
  bool nextHeroClicked(sf::Vector2f clicked_coords) const {
    return next_hero_button_.getGlobalBounds().contains(clicked_coords);
  }
  bool nextTurnClicked(sf::Vector2f clicked_coords) const {
    return next_turn_button_.getGlobalBounds().contains(clicked_coords);
  }
  bool exitClicked(sf::Vector2f clicked_coords) const {
    return exit_button_.getGlobalBounds().contains(clicked_coords);
  }
};

#endif