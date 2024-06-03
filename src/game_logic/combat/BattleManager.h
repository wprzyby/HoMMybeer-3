/**
 * @file BattleManager.h
 * @author Wojciech Przybylski
 * @brief Class for managing the state of combat
 * @copyright Copyright (c) 2024
 */

#ifndef SRC_GAME_LOGIC_COMBAT_BATTLE_MANAGER_H
#define SRC_GAME_LOGIC_COMBAT_BATTLE_MANAGER_H

#include "BattleState.h"
#include "Battleground.h"
#include "common.h"

namespace combat {
class BattleManager {
  BattleState state_;
  static void placeUnitsOnBattleground(Battleground& battleground,
                                       const UnitContainer& units);
  static BattleState setupInitialState(const Hero& attacker,
                                       const Hero& defender,
                                       BattlegroundSize battleground_size);

 public:
  BattleManager(const Hero& attacker, const Hero& defender,
                BattlegroundSize battleground_size = Battleground::DEFAULT_SIZE)
      : state_(setupInitialState(attacker, defender, battleground_size)) {}
  BattleManager(BattleManager&) = delete;
  BattleManager(BattleManager&&) = delete;
  BattleManager operator=(BattleManager&&) = delete;
  BattleManager operator=(const BattleManager&) = delete;
  ~BattleManager() = default;
  [[nodiscard]] const BattleState& getState() const { return state_; }
  [[nodiscard]] std::set<HexFieldCoords> getPossibleMoves() const;
  void makeMove(std::optional<HexFieldCoords> move_target);
  void passMove();
};

}  // namespace combat

#endif  // SRC_GAME_LOGIC_COMBAT_BATTLE_MANAGER_H