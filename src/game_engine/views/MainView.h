/**
 * @file MainView.h
 * @author Piotr Kluba
 * @brief View for drawing the main menu
 * @copyright Copyright (c) 2024
 */

#ifndef SRC_GAME_ENGINE_VIEWS_MAIN_VIEW_H
#define SRC_GAME_ENGINE_VIEWS_MAIN_VIEW_H

#include <SFML/Graphics.hpp>

class MainView : public sf::Drawable, public sf::Transformable {
  sf::Vector2u main_window_size_;
  sf::RectangleShape background_;
  inline static sf::Texture texture_;

 public:
  explicit MainView(sf::Vector2u main_window_size)
      : main_window_size_(main_window_size) {}

  void draw(sf::RenderTarget& target,
            sf::RenderStates /*states*/) const override {
    target.draw(background_);
  }
  static bool loadTileset(const std::string& path) {
    return texture_.loadFromFile(path);
  }
  void setMain() {
    background_.setSize(sf::Vector2f(main_window_size_));
    background_.setTexture(&texture_);
    background_.setTextureRect(
        sf::IntRect({0, 0}, sf::Vector2i(main_window_size_)));
  }
};
#endif // SRC_GAME_ENGINE_VIEWS_MAIN_VIEW_H