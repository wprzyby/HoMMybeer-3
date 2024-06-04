/**
 * @file BattleState.cc
 * @author Wojciech Przybylski
 * @brief
 * @copyright Copyright (c) 2024
 */
#include "BattleState.h"

#include <UnitBlock.hpp>
#include <algorithm>
#include <exception>
#include <vector>

#include "Battleground.h"
#include "combat_common.h"

namespace combat {

std::optional<HeroRole> BattleState::checkForWinner(
    const Battleground& battleground) {
  auto alive_units = battleground.getAliveUnitsIds();
  auto is_different_role = [](UnitIdentifier a, UnitIdentifier b) {
    return a.first != b.first;
  };
  bool is_battle_over =
      std::adjacent_find(alive_units.begin(), alive_units.end(),
                         is_different_role) == alive_units.end();
  if (not is_battle_over) {
    return {};
  }
  return alive_units[0].first;
}

UnitQueue BattleState::setupUnitMoveQueue(
    const std::vector<UnitIdentifier>& unit_ids, const UnitContainer& units) {
  if (unit_ids.empty()) {
    return {};
  }
  auto is_unit_faster = [&](UnitIdentifier a, UnitIdentifier b) {
    return units.at(a).speed > units.at(b).speed;
  };
  UnitQueue ret_val{};
  for (auto elem : unit_ids) {
    ret_val.emplace_back(elem);
  }
  std::sort(ret_val.begin(), ret_val.end(), is_unit_faster);
  return ret_val;
}

void BattleState::setupUnitMoveQueue(BattleState& state) {
  auto is_unit_faster = [&](UnitIdentifier a, UnitIdentifier b) {
    return state.hero_units_.at(a).speed > state.hero_units_.at(b).speed;
  };
  auto alive_unit_ids = state.battleground_.getAliveUnitsIds();
  for (auto elem : alive_unit_ids) {
    state.unit_move_queue_.emplace_back(elem);
  }
  std::sort(state.unit_move_queue_.begin(), state.unit_move_queue_.end(),
            is_unit_faster);
}

void BattleState::removeFromMoveQueue(UnitQueue& queue, UnitIdentifier unit) {
  auto find_result = std::find(queue.begin(), queue.end(), unit);
  if (find_result == queue.end()) {
    return;
  }
  queue.erase(find_result);
}

BattleState BattleState::moveWithCurrentUnit(HexFieldCoords destination) const {
  if (round_phase_ != RoundPhase::MOVING) {
    return *this;
  }
  auto range = hero_units_.at(current_unit_id_).speed;
  auto fields_in_range =
      battleground_.getFieldsWithinRange(current_unit_id_, range);
  if (not fields_in_range.contains(destination)) {
    return *this;
  }
  auto new_battleground = battleground_;
  new_battleground.moveUnitByIdentifier(current_unit_id_, destination);
  return BattleState{this->hero_units_,        //
                     this->unit_move_queue_,   //
                     new_battleground,         //
                     this->current_unit_id_,   //
                     this->currently_moving_,  //
                     this->round_phase_,       //
                     this->winner_}
      .passRound();
}

BattleState BattleState::attackWithCurrentUnit(
    HexFieldCoords defender_location) const {
  // checking basic parameters and state viability
  if (round_phase_ != RoundPhase::ATTACKING) {
    return *this;
  }
  auto defender_id_optional = battleground_.getFieldOccupant(defender_location);
  if (not defender_id_optional.has_value()) {
    return *this;
  }
  auto defender_id = defender_id_optional.value();
  if (defender_id.first == currently_moving_) {
    return *this;  // prevents friendly fire
  }
  auto current_unit_coords_optional =
      battleground_.getUnitCoords(current_unit_id_);
  if (not current_unit_coords_optional.has_value()) {
    throw std::exception();  // TODO: make own exception
  }

  auto attacking_unit = hero_units_.at(current_unit_id_);
  auto defending_unit = hero_units_.at(defender_id);

  // checking defender location viability in the context of attacker stats
  if (not attacking_unit.is_ranged and
      not battleground_.getAdjacentFields(current_unit_coords_optional.value())
              .contains(defender_location)) {
    return *this;
  }

  // attacking only after all the checks are successful
  performAttack<DamageRng>(attacking_unit, defending_unit);
  auto new_battleground = battleground_;
  auto new_queue = unit_move_queue_;
  auto remove_unit_from_battle = [&](UnitIdentifier unit_id) {
    removeFromMoveQueue(new_queue, unit_id);
    if (not new_battleground.removeUnitByIdentifier(unit_id)) {
      throw std::exception();  // TODO: make own exception
    }
  };
  if (attacking_unit.unit_count == 0) {
    remove_unit_from_battle(current_unit_id_);
  }
  if (defending_unit.unit_count == 0) {
    remove_unit_from_battle(defender_id);
  }
  auto new_units = hero_units_;
  new_units[current_unit_id_] = attacking_unit;
  new_units[defender_id] = defending_unit;

  auto new_state = BattleState{new_units,                //
                               new_queue,                //
                               new_battleground,         //
                               this->current_unit_id_,   //
                               this->currently_moving_,  //
                               this->round_phase_,       //
                               checkForWinner(new_battleground)};
  passRound(new_state);
  return std::move(new_state);
}

void BattleState::passRound(BattleState& state) {
  if (state.round_phase_ == RoundPhase::MOVING and
      (state.hero_units_.at(state.current_unit_id_).is_ranged or
       state.battleground_.getAdjacentEnemyUnitsCoords(state.current_unit_id_)
               .size() > 0)) {
    state.round_phase_ = RoundPhase::ATTACKING;
    return;
  }
  if (state.unit_move_queue_.empty()) {
    setupUnitMoveQueue(state);
  }
  state.current_unit_id_ = state.unit_move_queue_.front();
  state.unit_move_queue_.pop_front();
  state.round_phase_ = RoundPhase::MOVING;
  state.currently_moving_ = state.current_unit_id_.first;
}

BattleState BattleState::passRound() const {
  if (round_phase_ == RoundPhase::MOVING and
      (hero_units_.at(current_unit_id_).is_ranged or
       battleground_.getAdjacentEnemyUnitsCoords(current_unit_id_).size() >
           0)) {
    return BattleState{this->hero_units_,        //
                       this->unit_move_queue_,   //
                       this->battleground_,      //
                       this->current_unit_id_,   //
                       this->currently_moving_,  //
                       RoundPhase::ATTACKING,    //
                       this->winner_};
  }
  UnitQueue new_queue{};
  if (unit_move_queue_.empty()) {
    new_queue =
        setupUnitMoveQueue(battleground_.getAliveUnitsIds(), hero_units_);
  } else {
    new_queue = unit_move_queue_;
  }
  auto new_current_unit_id = new_queue.front();
  new_queue.pop_front();
  return BattleState{this->hero_units_,          //
                     new_queue,                  //
                     this->battleground_,        //
                     new_current_unit_id,        //
                     new_current_unit_id.first,  //
                     RoundPhase::MOVING,         //
                     this->winner_};
}

std::set<HexFieldCoords> BattleState::getPossibleMoves() const {
  const auto& current_unit = hero_units_.at(current_unit_id_);
  if (round_phase_ == RoundPhase::MOVING) {
    return battleground_.getFieldsWithinRange(current_unit_id_,
                                              current_unit.speed);
  }
  if (not current_unit.is_ranged) {
    return battleground_.getAdjacentEnemyUnitsCoords(current_unit_id_);
  }
  auto opponent = currently_moving_ == HeroRole::ATTACKER ? HeroRole::DEFENDER
                                                          : HeroRole::ATTACKER;
  return battleground_.getAllHeroUnitCoords(opponent);
}

BattleState BattleState::makeMove(
    std::optional<HexFieldCoords> move_target) const {
  if (not move_target.has_value()) {
    return passRound();
  }
  auto coords = move_target.value();
  if (round_phase_ == RoundPhase::MOVING) {
    return this->moveWithCurrentUnit(coords);
  }
  return this->attackWithCurrentUnit(coords);
}

}  // namespace combat