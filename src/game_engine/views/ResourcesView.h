/**
 * @file ResourcesView.h
 * @author Piotr Kluba
 * @brief View for displaying resources quantities and incomes
 * @copyright Copyright (c) 2024
 */

#ifndef SRC_GAME_ENGINE_VIEWS_RESOURCES_VIEW_H_
#define SRC_GAME_ENGINE_VIEWS_RESOURCES_VIEW_H_

#include <Player.h>

#include <SFML/Graphics.hpp>
#include <vector>

class ResourcesView : public sf::Drawable, public sf::Transformable {
 private:
  constexpr static int RESOURCE_TYPES = 7;
  constexpr static unsigned int CHARACTER_SIZE = 25;
  inline const static sf::Color BACKGROUND_COLOR = sf::Color(82, 38, 8);
  sf::RectangleShape background_;
  inline static sf::Texture icons_texture_;
  std::vector<sf::Sprite> icons_;
  std::vector<sf::Text> amounts_;
  std::vector<sf::Text> units_;
  sf::Text day_;
  sf::Vector2f window_size_;
  sf::Vector2f game_window_size_;
  sf::Vector2f game_window_offset_;
  sf::Font font_;

 public:
  ResourcesView(sf::Vector2f window_size, sf::Vector2f game_window_size,
                sf::Vector2f game_window_offset, sf::Font font)
      : window_size_(window_size),
        game_window_size_(game_window_size),
        game_window_offset_(game_window_offset),
        font_(font) {}
  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
  void setResources(const Player* player, int day);
  void setUnitAmounts(const Hero* hero);
  static bool loadTileset(std::string path) {
    return icons_texture_.loadFromFile(path);
  }
};

#endif