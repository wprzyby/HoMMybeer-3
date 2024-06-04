/**
 * @file MiniMaxFighter.h
 * @author Wojciech Przybylski
 * @brief Class for making battle moves using the minimax algorithm
 * @copyright Copyright (c) 2024
 */

#ifndef SRC_GAME_LOGIC_AI_MINI_MAX_FIGHTER_H
#define SRC_GAME_LOGIC_AI_MINI_MAX_FIGHTER_H

#include <BattleState.h>

#include <functional>
#include <utility>

#include "combat/BattleState.h"
#include "combat/combat_utils.h"

namespace combat::ai {
using Move = std::optional<HexFieldCoords>;
using EvaluationCallable = std::function<float(const BattleState&, HeroRole)>;

class MiniMaxFighter {
 public:
  explicit MiniMaxFighter(EvaluationCallable evaluation_callable,
                          unsigned int depth)
      : evaluation_callable_(std::move(evaluation_callable)), depth_(depth) {}

  Move makeMove(const BattleState& state, HeroRole maximizing_player_role);

 private:
  static float miniMax(const BattleState& state, unsigned int depth,
                       float alpha, float beta, HeroRole maximizing_player_role,
                       const EvaluationCallable& evaluation_callable);
  EvaluationCallable evaluation_callable_;
  unsigned int depth_;
};

}  // namespace combat::ai

#endif  // SRC_GAME_LOGIC_AI_MINI_MAX_FIGHTER_H