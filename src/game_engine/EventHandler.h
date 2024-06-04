/**
 * @file EventHandler.h
 * @author Piotr Kluba (you@domain.com)
 * @brief Observable to handle inputs of the user
 * @date 2024-06-03
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <Controller.h>
#include <Session.h>

#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include <utility>
#include <vector>

class EventHandler {
 private:
  std::vector<Controller*> controllers_;

 public:
  explicit EventHandler(std::vector<Controller*> controllers)
      : controllers_(std::move(controllers)) {}
  void handle(sf::Event& event, SessionState session_state, Game& game);
};