#ifndef SRC_GAME_ENGINE_BATTLE_WINDOW_CONTROLLER_H
#define SRC_GAME_ENGINE_BATTLE_WINDOW_CONTROLLER_H

#include <Hero.h>
#include <SFML/Graphics/Drawable.hpp>

#include "BattleManager.h"
#include "BattlegroundView.h"
#include "Controller.h"

class BattleWindowController : public Controller, public sf::Drawable {
  sf::Vector2u game_window_size_;
  BattlegroundView battleground_view_;
  combat::BattleManager battle_manager_;
  int attacked_player_idx_{-1};
  int attacker_hero_idx_{-1};
  std::optional<const Hero*> findHeroClicked(FieldCoords field_clicked,
                                             const Game& game);
  void handleEndOfBattle(Game& game);
  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

 public:
  BattleWindowController(sf::Vector2u game_window_size)
      : game_window_size_(game_window_size) {}
  void update(sf::Event& event, SessionState session_state,
              Game& game) override;
};

#endif  // SRC_GAME_ENGINE_BATTLE_WINDOW_CONTROLLER_H