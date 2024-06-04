#ifndef SRC_GAME_LOGIC_AI_COMBAT_EVALUATION_H
#define SRC_GAME_LOGIC_AI_COMBAT_EVALUATION_H

#include <BattleState.h>

namespace combat::ai {

float simpleEvaluate(const BattleState& state, HeroRole maximizing_player_role);
float complexEvaluate(const BattleState& state, HeroRole maximizing_player_role);

}  // namespace combat::ai

#endif  // SRC_GAME_LOGIC_AI_COMBAT_EVALUATION_H