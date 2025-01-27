/**
 * @file ResourcesView.h
 * @author Piotr Kluba (.com)
 * @brief View for displaying resources quantities and incomes
 * @copyright Copyright (c) 2024
 */

#include <Config.h>
#include <MapView.h>
#include <ResourcesView.h>

#include <SFML/Graphics.hpp>
#include <format>
#include <map>
#include <vector>

#include "Player.h"
#include "UnitBlock.hpp"

void ResourcesView::setResources(const Player* player, int day) {
  icons_.clear();
  amounts_.clear();
  background_ = sf::RectangleShape(sf::Vector2f(
      {game_window_size_.x + game_window_offset_.x,
       window_size_.y - game_window_size_.y - game_window_offset_.y}));
  background_.setFillColor(BACKGROUND_COLOR);
  background_.setPosition(
      sf::Vector2f({0, game_window_size_.y + game_window_offset_.y}));

  for (int i = 0; i < RESOURCE_TYPES; ++i) {
    sf::Sprite sprite(ResourcesView::icons_texture_);
    std::vector<int> texture_size_inp =
        Config::getInstance()
            ->getObjectsMetadata()
            .at("resources")
            .at("Resources")
            .at("texture_size")
            .at(Config::enumToStringTranslate(static_cast<ResourceType>(i)));
    std::vector<int> texture_origin_inp =
        Config::getInstance()
            ->getObjectsMetadata()
            .at("resources")
            .at("Resources")
            .at("starts_at")
            .at(Config::enumToStringTranslate(static_cast<ResourceType>(i)));
    sf::Vector2i texture_size({texture_size_inp[0], texture_size_inp[1]});
    sf::Vector2i texture_origin({texture_origin_inp[0], texture_origin_inp[1]});
    sprite.setTextureRect(sf::IntRect(texture_origin, texture_size));
    sprite.setOrigin(MapView::MAP_TILE_SIZE.x, 0);
    sprite.setPosition(
        game_window_offset_.x + (3 * MapView::MAP_TILE_SIZE.x * i),
        (1.2 * game_window_offset_.y) + game_window_size_.y);
    icons_.push_back(sprite);
    int current_player_resource =
        player->getResourceAmount(static_cast<ResourceType>(i));
    int current_player_income =
        player->getResourceIncome(static_cast<ResourceType>(i));
    sf::Text text{std::format("{}\n(+{})", current_player_resource,
                              current_player_income),
                  font_};
    text.setCharacterSize(15);
    text.setPosition(game_window_offset_.x + MapView::MAP_TILE_SIZE.x +
                         (3 * MapView::MAP_TILE_SIZE.x * i),
                     (1.2 * game_window_offset_.y) + game_window_size_.y);
    amounts_.push_back(text);
  }
  day_ = sf::Text(std::format("Day {}", day), font_);
  day_.setPosition(game_window_offset_.x + MapView::MAP_TILE_SIZE.x +
                       (3 * MapView::MAP_TILE_SIZE.x * 7),
                   (1.2 * game_window_offset_.y) + game_window_size_.y);
  day_.setCharacterSize(CHARACTER_SIZE);
}

void ResourcesView::setUnitAmounts(const Hero* hero) {
  units_.clear();
  sf::Text text{"Unit counts: ", font_};
  text.setCharacterSize(25);
  text.setPosition(game_window_offset_.x,
                   (1.4 * game_window_offset_.y) + game_window_size_.y + 30);
  units_.emplace_back(text);
  for (int i = 0; i < 3; ++i) {
    std::map<int, int> count_map = {{1, 0}, {2, 0}, {3, 0}};
    Faction curr_faction = static_cast<Faction>(i);
    UnitOrigin curr_org = Config::factionToUnitOriginTranslate(curr_faction);
    auto unit_family = hero->getUnits();
    for (auto block : unit_family) {
      if (block.type == curr_org) {
        count_map.at(block.level) = block.unit_count;
      }
    }
    sf::Text text{
        std::format("{} |{}|{}|{}|",
                    Config::enumToStringTranslate(curr_faction),
                    count_map.at(1), count_map.at(2), count_map.at(3)),
        font_};
    text.setCharacterSize(CHARACTER_SIZE);
    text.setPosition(game_window_offset_.x + 6 * MapView::MAP_TILE_SIZE.x +
                         (5 * MapView::MAP_TILE_SIZE.x * i),
                     (1.4 * game_window_offset_.y) + game_window_size_.y + 30);
    units_.emplace_back(text);
  }
}

void ResourcesView::draw(sf::RenderTarget& target,
                         sf::RenderStates states) const {
  target.draw(background_, states);
  for (const auto& icon : icons_) {
    target.draw(icon);
  }
  for (const auto& text : amounts_) {
    target.draw(text);
  }
  for (const auto& text : units_) {
    target.draw(text);
  }
  target.draw(day_);
}