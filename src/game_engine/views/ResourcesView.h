/**
 * @file ControlsView.h
 * @author Piotr Kluba (you@domain.com)
 * @brief Entity for displaying resources quantities and incomes
 * @date 2024-06-02
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef SRC_GAME_ENGINE_VIEWS_RESOURCES_VIEW_H_
#define SRC_GAME_ENGINE_VIEWS_RESOURCES_VIEW_H_

#include <Player.h>

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <vector>

class ResourcesView : public sf::Drawable, public sf::Transformable {
 private:
  inline static constexpr int RESOURCE_TYPES = 7;
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
  static bool loadtileset(std::string path) {
    return icons_texture_.loadFromFile(path);
  }
};

#endif