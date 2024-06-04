/**
 * @file BattleWindowController.h
 * @author Wojciech Przybylski
 * @brief Controller managing the window when the game is in battle state
 * @copyright Copyright (c) 2024
 */

#ifndef SRC_GAME_ENGINE_CONTROLLERS_BATTLE_WINDOW_CONTROLLER_H
#define SRC_GAME_ENGINE_CONTROLLERS_BATTLE_WINDOW_CONTROLLER_H

#include <BattleManager.h>
#include <BattlegroundView.h>
#include <Hero.h>
#include <ai/MiniMaxFighter.h>

#include <SFML/Graphics.hpp>
#include <functional>

#include "Controller.h"

class BattleWindowController : public Controller, public sf::Drawable {
  constexpr static unsigned int MINI_MAX_DEPTH = 3;
  constexpr static unsigned int AI_PLAYER_MOVE_DELAY = 500;  // ms
  const static std::function<float(const combat::BattleState&,
                                   combat::HeroRole)>
      MINI_MAX_STATE_EVAL;
  sf::Vector2u game_window_size_;
  BattlegroundView battleground_view_;
  combat::BattleManager battle_manager_;
  combat::ai::MiniMaxFighter mini_max_fighter_;
  int attacked_player_idx_{-1};
  int attacker_hero_idx_{-1};
  std::optional<const Hero*> findHeroClicked(FieldCoords field_clicked,
                                             const Game& game);
  void handleEndOfBattle(Game& game);
  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
  bool isAiPlaying(const Game& game) const;
  bool makeMoveIsBattleOver(std::optional<combat::HexFieldCoords> move);

 public:
  explicit BattleWindowController(sf::Vector2u game_window_size)
      : game_window_size_(game_window_size),
        mini_max_fighter_(MINI_MAX_STATE_EVAL, MINI_MAX_DEPTH) {}
  void update(sf::Event& event, SessionState session_state,
              Game& game) override;
};

#endif  // SRC_GAME_ENGINE_CONTROLLERS_BATTLE_WINDOW_CONTROLLER_H