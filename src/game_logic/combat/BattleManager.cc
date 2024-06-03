/**
 * @file BattleManager.cc
 * @author Wojciech Przybylski
 * @brief
 * @copyright Copyright (c) 2024
 */
#include "BattleManager.h"

#include <vector>

#include "BattleState.h"
#include "Battleground.h"
#include "common.h"

namespace combat {

void BattleManager::placeUnitsOnBattleground(Battleground& battleground,
                                             const UnitContainer& units) {
  // placing attacker units on the left side edge of the battleground
  for (unsigned int i : std::views::iota(0U, units.size().first)) {
    auto first_coord = i % 2;
    auto second_coord = i / 2;
    auto third_coord = 0;
    if (not battleground.placeUnit({first_coord, second_coord, third_coord},
                                   {HeroRole::ATTACKER, i})) {
      throw std::exception();  // TODO: make own exception
    }
  }
  // placing defender units on the right side edge of the battleground
  for (unsigned int i : std::views::iota(0U, units.size().second)) {
    auto first_coord = i % 2;
    auto second_coord = i / 2;
    auto third_coord = first_coord == 0
                           ? battleground.getSize().even_rows_size - 1
                           : battleground.getSize().odd_rows_size - 1;
    if (not battleground.placeUnit({first_coord, second_coord, third_coord},
                                   {HeroRole::DEFENDER, i})) {
      throw std::exception();  // TODO: make own exception
    }
  }
}

BattleState BattleManager::setupInitialState(
    const Hero& attacker, const Hero& defender,
    BattlegroundSize battleground_size) {
  std::map<HeroRole, std::vector<UnitBlock>> units;
  units[HeroRole::ATTACKER] = attacker.getUnits();
  units[HeroRole::DEFENDER] = defender.getUnits();
  UnitContainer unit_container(units);
  Battleground battleground = Battleground(battleground_size);
  placeUnitsOnBattleground(battleground, unit_container);
  UnitQueue initial_unit_queue = BattleState::setupUnitMoveQueue(
      getAllUnitIds(unit_container), unit_container);
  auto initial_unit = initial_unit_queue.front();
  initial_unit_queue.pop_front();

  return BattleState{unit_container,                   //
                     initial_unit_queue,               //
                     battleground,                     //
                     initial_unit,                     //
                     initial_unit.first,               //
                     BattleState::RoundPhase::MOVING,  //
                     {}};
}

std::set<HexFieldCoords> BattleManager::getPossibleMoves() const {
  return state_.getPossibleMoves();
}

void BattleManager::makeMove(std::optional<HexFieldCoords> move_target) {
  state_ = state_.makeMove(move_target);
}

void BattleManager::passMove() { state_ = state_.makeMove(std::nullopt); }

}  // namespace combat