/**
 * @file EventHandler.cc
 * @author Piotr Kluba (you@domain.com)
 * @brief Observable to handle inputs of the user
 * @date 2024-06-03
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <EventHandler.h>

void EventHandler::handle(sf::Event& event, SessionState session_state,
                          Game& game) {
  for (auto* observer : controllers_) {
    observer->update(event, session_state, game);
  }
}
