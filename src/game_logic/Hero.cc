/**
 * @file Hero.cc
 * @author Piotr Kluba
 * @brief Funcionality responsible for the single hero
 * @version 0.1
 * @date 2024-04-17
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <Hero.h>
#include <Session.h>

using namespace std;

optional<Path> Hero::setMoveGoal(FieldCoords goal) {
  if (!(Session::getInstance()
            ->game->getMap()
            ->getField(goal)
            .value()
            ->isWalkable())) {
    return {};
  }
  Path opt_path = {};
  opt_path.push_back(goal);
  move_path_ = opt_path;
  return opt_path;
}

void Hero::moveTo(FieldCoords coords) {
  while (!(move_path_.empty()) &&
         (energy_ - Field::resistance.at(Session::getInstance()
                                             ->game->getMap()
                                             ->getField(move_path_.back())
                                             .value()
                                             ->getTerrainType())) > 0) {
    step(move_path_.back());
    move_path_.pop_back();
    energy_ = energy_ - Field::resistance.at(Session::getInstance()
                                                 ->game->getMap()
                                                 ->getField(move_path_.back())
                                                 .value()
                                                 ->getTerrainType());
  }
}

void Hero::step(FieldCoords step_to) {
  // checking if field is adjacent and if is possible to move to
  hero_coords_ = step_to;
}