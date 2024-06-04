#include "BattleWindowController.h"

#include <stdexcept>

#include "Session.h"

void BattleWindowController::update(sf::Event& event,
                                    SessionState session_state, Game& game) {
  if (session_state != SessionState::LOAD_BATTLE and
      session_state != SessionState::IN_BATTLE) {
    return;
  }

  if (session_state == SessionState::LOAD_BATTLE) {
    auto attacked_hero_info = Session::getInstance()->getAttackedHeroInfo();
    attacked_player_idx_ = attacked_hero_info.first;
    attacker_hero_idx_ = attacked_hero_info.second;
    const auto* attacked_hero = game.getPlayer(attacked_hero_info.first)
                                    ->getHero(attacked_hero_info.second);
    battle_manager_.setupBattle(*game.getCurrentPlayer()->getCurrentHero(),
                                *attacked_hero);
    battleground_view_.setupBattle(
        combat::Battleground::DEFAULT_SIZE,
        Session::getInstance()->getBattleTerrainType());
    battleground_view_.loadState(battle_manager_.getState());
    Session::getInstance()->setSessionState(SessionState::IN_BATTLE);
    return;
  }

  if (event.type != sf::Event::MouseButtonPressed or
      event.mouseButton.button != sf::Mouse::Left) {
    return;
  }
  auto clicked_coords = battleground_view_.translateToHexCoords(
      event.mouseButton.x, event.mouseButton.y);
  if (not clicked_coords.has_value()) {
    return;
  }
  battle_manager_.makeMove(clicked_coords.value());
  battleground_view_.loadState(battle_manager_.getState());
  if (battle_manager_.getState().winner_.has_value()) {
    handleEndOfBattle(game);
  }
}

void BattleWindowController::handleEndOfBattle(Game& game) {
  auto units_data = battle_manager_.getState().hero_units_.get_underlying();
  auto was_unit_killed = [](const UnitBlock& unit) {
    return unit.unit_count == 0;
  };
  auto get_new_units = [&](combat::HeroRole role) {
    auto new_units = units_data.at(role);
    auto remove_iter =
        std::remove_if(new_units.begin(), new_units.end(), was_unit_killed);
    if (remove_iter != new_units.end()) {
      new_units.erase(remove_iter, new_units.end());
    }
    return new_units;
  };
  game.getCurrentPlayer()->setCurrentHeroUnits(
      get_new_units(combat::HeroRole::ATTACKER));
  game.getPlayer(attacked_player_idx_)
      ->setHeroUnits(attacker_hero_idx_,
                     get_new_units(combat::HeroRole::DEFENDER));
  if (game.getCurrentPlayer()->isDead() or
      game.getPlayer(attacked_player_idx_)->isDead()) {
    Session::getInstance()->setSessionState(SessionState::START_MENU);
    return;
  }
  Session::getInstance()->setSessionState(SessionState::REFRESH);
}

void BattleWindowController::draw(sf::RenderTarget& target,
                                  sf::RenderStates states) const {
  target.draw(battleground_view_, states);
}