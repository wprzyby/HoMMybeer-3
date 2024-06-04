#include "MiniMaxFighter.h"

#include <algorithm>

namespace combat::ai {

Move MiniMaxFighter::makeMove(const BattleState& state,
                              HeroRole maximizing_player_role) {
  if (depth_ == 0 or state.winner_.has_value()) {
    return {};
  }
  float alpha = -std::numeric_limits<float>::infinity();
  float beta = std::numeric_limits<float>::infinity();
  Move best_found_move = std::nullopt;
  auto possible_moves = state.getPossibleMoves();
  std::set<Move> possible_moves_set(possible_moves.begin(),
                                    possible_moves.end());
  possible_moves_set.insert(std::nullopt);
  if (state.currently_moving_ == maximizing_player_role) {
    float best_evaluation = -std::numeric_limits<float>::infinity();

    for (const auto& move : possible_moves_set) {
      auto result = miniMax(state.makeMove(move), depth_ - 1, alpha, beta,
                            maximizing_player_role, evaluation_callable_);
      if (result > best_evaluation) {
        best_evaluation = result;
        best_found_move = move;
      }
      alpha = std::max(alpha, best_evaluation);
      if (best_evaluation >= beta) {
        break;
      }
    }
    return best_found_move;
  }
  float best_evaluation = std::numeric_limits<float>::infinity();

  for (const auto& move : possible_moves_set) {
    auto result = miniMax(state.makeMove(move), depth_ - 1, alpha, beta,
                          maximizing_player_role, evaluation_callable_);
    if (result < best_evaluation) {
      best_evaluation = result;
      best_found_move = move;
    }
    beta = std::min(beta, best_evaluation);
    if (best_evaluation <= alpha) {
      break;
    }
  }
  return best_found_move;
}

float MiniMaxFighter::miniMax(const BattleState& state, unsigned int depth,
                              float alpha, float beta,
                              HeroRole maximizing_player_role,
                              const EvaluationCallable& evaluation_callable) {
  if (depth == 0 or state.winner_.has_value()) {
    return evaluation_callable(state, maximizing_player_role);
  }
  auto possible_moves = state.getPossibleMoves();
  std::set<Move> possible_moves_set(possible_moves.begin(),
                                    possible_moves.end());
  possible_moves_set.insert(std::nullopt);
  if (state.currently_moving_ == maximizing_player_role) {
    float evaluation = -std::numeric_limits<float>::infinity();
    for (const auto& move : possible_moves_set) {
      auto result = miniMax(state.makeMove(move), depth - 1, alpha, beta,
                            maximizing_player_role, evaluation_callable);
      evaluation = std::max(result, evaluation);
      alpha = std::max(alpha, evaluation);
      if (evaluation >= beta) {
        break;
      }
    }
    return evaluation;
  }
  float evaluation = std::numeric_limits<float>::infinity();
  for (const auto& move : possible_moves_set) {
    auto result = miniMax(state.makeMove(move), depth - 1, alpha, beta,
                          maximizing_player_role, evaluation_callable);
    evaluation = std::min(result, evaluation);
    beta = std::min(beta, evaluation);
    if (evaluation <= alpha) {
      break;
    }
  }
  return evaluation;
}
}  // namespace combat::ai
