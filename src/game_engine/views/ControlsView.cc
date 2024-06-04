/**
 * @file ControlsView.cc
 * @author Piotr Kluba
 * @brief Entity representing buttons used to control the game
 * @copyright Copyright (c) 2024
 */

#include <ControlsView.h>
#include <game_logic_utils.h>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

void ControlsView::setControls() {
  background_ = sf::RectangleShape(sf::Vector2f(
      {window_size_.x - game_window_offset_.x - game_window_size_.x,
       window_size_.y}));
  background_.setFillColor(BACKGROUND_COLOR);
  background_.setPosition(
      sf::Vector2f({game_window_offset_.x + game_window_size_.x, 0}));
  next_hero_button_ = sf::RectangleShape(BUTT_SIZE);
  next_turn_button_ = sf::RectangleShape(BUTT_SIZE);
  exit_button_ = sf::RectangleShape(BUTT_SIZE);
  next_hero_button_.setPosition(((-game_window_offset_.x + window_size_.x -
                                  game_window_size_.x - BUTT_SIZE.x) /
                                 2) +
                                    game_window_size_.x + game_window_offset_.x,
                                game_window_offset_.y + OUTLINE_THICKNESS);
  next_turn_button_.setPosition(
      ((-game_window_offset_.x + window_size_.x - game_window_size_.x -
        BUTT_SIZE.x) /
       2) +
          game_window_size_.x + game_window_offset_.x,
      game_window_offset_.y + BUTT_SPACER + BUTT_SIZE.y + OUTLINE_THICKNESS);
  exit_button_.setPosition(((-game_window_offset_.x + window_size_.x -
                             game_window_size_.x - BUTT_SIZE.x) /
                            2) +
                               game_window_size_.x + game_window_offset_.x,
                           game_window_offset_.y + 2 * BUTT_SPACER +
                               2 * BUTT_SIZE.y + OUTLINE_THICKNESS);

  next_hero_button_.setFillColor(BUTTON_COLOR);
  next_turn_button_.setFillColor(BUTTON_COLOR);
  exit_button_.setFillColor(BUTTON_COLOR);
  next_hero_button_.setOutlineThickness(3);
  next_turn_button_.setOutlineThickness(3);
  exit_button_.setOutlineThickness(3);
  next_hero_button_.setOutlineColor(sf::Color::White);
  next_turn_button_.setOutlineColor(sf::Color::Blue);
  exit_button_.setOutlineColor(sf::Color::Red);
  next_hero_text_ = sf::Text{"Next Hero", font_};
  next_hero_text_.setOrigin(next_hero_text_.getGlobalBounds().getSize() / 2.F +
                            next_hero_text_.getLocalBounds().getPosition());
  next_hero_text_.setPosition(next_hero_button_.getPosition() +
                              (next_hero_button_.getSize() / 2.F));
  next_turn_text_ = sf::Text{"Next Player", font_};
  next_turn_text_.setOrigin(next_turn_text_.getGlobalBounds().getSize() / 2.F +
                            next_turn_text_.getLocalBounds().getPosition());
  next_turn_text_.setPosition(next_turn_button_.getPosition() +
                              (next_turn_button_.getSize() / 2.F));
  exit_text_ = sf::Text{"Exit", font_};
  exit_text_.setOrigin(exit_text_.getGlobalBounds().getSize() / 2.F +
                       exit_text_.getLocalBounds().getPosition());
  exit_text_.setPosition(exit_button_.getPosition() +
                         (exit_button_.getSize() / 2.F));
  next_hero_text_.setCharacterSize(CHARACTER_SIZE);
  next_turn_text_.setCharacterSize(CHARACTER_SIZE);
  exit_text_.setCharacterSize(CHARACTER_SIZE);
}

void ControlsView::draw(sf::RenderTarget& target,
                        sf::RenderStates states) const {
  target.draw(background_, states);
  target.draw(next_hero_button_, states);
  target.draw(next_turn_button_, states);
  target.draw(exit_button_, states);
  target.draw(next_hero_text_, states);
  target.draw(next_turn_text_, states);
  target.draw(exit_text_, states);
}