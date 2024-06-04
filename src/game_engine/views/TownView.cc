/**
 * @file TownView.cc
 * @author Wojciech Przybylski, Piotr Kluba
 * @brief View for drawing the insides of the town
 * @copyright Copyright (c) 2024
 */

#include "TownView.h"

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>
#include <format>
#include <string>

#include "Config.h"
#include "game_logic_utils.h"

const sf::Vector2f TownView::BUTTON_SIZE = {150, 80};
const sf::Vector2f TownView::LEFT_BUTTON_POSITION = {100, 400};
const sf::Color TownView::BUTTON_COLOR = sf::Color(56, 37, 13);
const sf::Vector2f TEXT_OFFSET = {5, 20};

void TownView::loadTown(Faction faction) {
  auto* config = Config::getInstance();
  auto metadata = config->getTownMetadata();
  std::string type = metadata.at(Config::enumToStringTranslate(faction));
  std::string texture_path = getProjectPath() + std::format("/{}", type);
  background_size_ = {metadata["background_size"][0].get<int>(),
                      metadata["background_size"][1].get<int>()};
  loadBackground(texture_path);
  loadButtons();
}

bool TownView::loadBackground(const std::string& texture_path) {
  if (not background_texture_.loadFromFile(texture_path)) {
    return false;
  }
  background_sprite_.setTexture(background_texture_);
  background_sprite_.setTextureRect(
      sf::IntRect(0, 0, background_size_.x, background_size_.y));
  background_sprite_.setScale(
      static_cast<float>(window_size_.x) / background_size_.x,
      static_cast<float>(window_size_.y) / background_size_.y);

  return true;
}

void TownView::loadButtons() {
  buy_lvl1_button_ = sf::RectangleShape(BUTTON_SIZE);
  buy_lvl2_button_ = sf::RectangleShape(BUTTON_SIZE);
  buy_lvl3_button_ = sf::RectangleShape(BUTTON_SIZE);
  buy_hero_button_ = sf::RectangleShape(BUTTON_SIZE);
  buy_lvl1_button_.setPosition(LEFT_BUTTON_POSITION);
  buy_lvl2_button_.setPosition(
      LEFT_BUTTON_POSITION.x + BUTTON_SIZE.x + BUTTON_SEPARATION,
      LEFT_BUTTON_POSITION.y);
  buy_lvl3_button_.setPosition(
      LEFT_BUTTON_POSITION.x + 2 * (BUTTON_SIZE.x + BUTTON_SEPARATION),
      LEFT_BUTTON_POSITION.y);
  buy_hero_button_.setPosition(
      LEFT_BUTTON_POSITION.x + 3 * (BUTTON_SIZE.x + BUTTON_SEPARATION),
      LEFT_BUTTON_POSITION.y);
  buy_lvl1_button_.setFillColor(BUTTON_COLOR);
  buy_lvl2_button_.setFillColor(BUTTON_COLOR);
  buy_lvl3_button_.setFillColor(BUTTON_COLOR);
  buy_hero_button_.setFillColor(BUTTON_COLOR);
  buy_lvl1_text_ = sf::Text{"Buy lvl 1 unit", font_};
  buy_lvl2_text_ = sf::Text{"Buy lvl 2 unit", font_};
  buy_lvl3_text_ = sf::Text{"Buy lvl 3 unit", font_};
  buy_hero_text_ = sf::Text{"Buy hero", font_};

  auto set_text_position = [&](sf::Text& text, sf::RectangleShape& button) {
    text.setPosition(button.getPosition() + TEXT_OFFSET);
  };
  set_text_position(buy_lvl1_text_, buy_lvl1_button_);
  set_text_position(buy_lvl2_text_, buy_lvl2_button_);
  set_text_position(buy_lvl3_text_, buy_lvl3_button_);
  set_text_position(buy_hero_text_, buy_hero_button_);

  buy_lvl1_text_.setCharacterSize(CHARACTER_SIZE);
  buy_lvl2_text_.setCharacterSize(CHARACTER_SIZE);
  buy_lvl3_text_.setCharacterSize(CHARACTER_SIZE);
  buy_hero_text_.setCharacterSize(CHARACTER_SIZE);
}

void TownView::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  states.transform *= getTransform();
  target.draw(background_sprite_, states);
  target.draw(buy_lvl1_button_, states);
  target.draw(buy_lvl2_button_, states);
  target.draw(buy_lvl3_button_, states);
  target.draw(buy_hero_button_, states);
  target.draw(buy_lvl1_text_, states);
  target.draw(buy_lvl2_text_, states);
  target.draw(buy_lvl3_text_, states);
  target.draw(buy_hero_text_, states);
}

std::optional<int> TownView::buyUnitClicked(sf::Vector2f clicked_coords) const {
  if (buy_lvl1_button_.getGlobalBounds().contains(clicked_coords)) {
    return 1;
  }
  if (buy_lvl2_button_.getGlobalBounds().contains(clicked_coords)) {
    return 2;
  }
  if (buy_lvl3_button_.getGlobalBounds().contains(clicked_coords)) {
    return 3;
  }
  return std::nullopt;
}