/**
 * @file BattleState.h
 * @author Wojciech Przybylski
 * @brief Class defining the state of combat at a given moment, meant to
 * be used as an immutable
 * @copyright Copyright (c) 2024
 */
#ifndef SRC_GAME_LOGIC_COMBAT_BATTLE_STATE_H
#define SRC_GAME_LOGIC_COMBAT_BATTLE_STATE_H

#include <Hero.h>

#include <optional>
#include <random>

#include "Battleground.h"
#include "combat_common.h"

namespace combat {

class BattleState {
 public:
  enum class RoundPhase {
    MOVING,
    ATTACKING,
  };

  UnitContainer hero_units_{};
  UnitQueue unit_move_queue_{};
  Battleground battleground_{};
  UnitIdentifier current_unit_id_{HeroRole::ATTACKER, 0U};
  HeroRole currently_moving_{HeroRole::ATTACKER};
  RoundPhase round_phase_{RoundPhase::MOVING};
  std::optional<HeroRole> winner_{std::nullopt};

 private:
  static std::optional<HeroRole> checkForWinner(
      const Battleground& battleground);
  static void removeFromMoveQueue(UnitQueue& queue, UnitIdentifier unit);
  [[nodiscard]] BattleState moveWithCurrentUnit(
      HexFieldCoords destination) const;
  [[nodiscard]] BattleState attackWithCurrentUnit(
      HexFieldCoords defender_location) const;
  [[nodiscard]] BattleState passRound() const;
  static void passRound(BattleState& state);
  static void setupUnitMoveQueue(BattleState& state);

 public:
  BattleState() = default;
  BattleState(UnitContainer hero_units, UnitQueue unit_move_queue,
              Battleground battleground, UnitIdentifier current_unit_id,
              HeroRole currently_moving, RoundPhase round_phase,
              std::optional<HeroRole> winner)
      : hero_units_(std::move(hero_units)),
        unit_move_queue_(std::move(unit_move_queue)),
        battleground_(std::move(battleground)),
        current_unit_id_(std::move(current_unit_id)),
        currently_moving_(currently_moving),
        round_phase_(round_phase),
        winner_(winner) {}

  [[nodiscard]] static UnitQueue setupUnitMoveQueue(
      const std::vector<UnitIdentifier>& unit_ids, const UnitContainer& units);
  [[nodiscard]] std::set<HexFieldCoords> getPossibleMoves() const;
  [[nodiscard]] BattleState makeMove(
      std::optional<HexFieldCoords> move_target) const;
};

class DamageRng {
 public:
  int getRandIntInclusive(int from, int to) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(from, to);
    return distr(gen);
  }
};

}  // namespace combat

#endif  // SRC_GAME_LOGIC_COMBAT_BATTLE_STATE_H
