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

Hero::~Hero() {}

optional<Path> Hero::setMoveGoal(FieldCoords goal) {
  if (!(Session::getInstance()->game->getMap()->getField(goal)->isWalkable())) {
    // pathfinding etc
    return {};
  }
  Path opt_path = {};
  opt_path.push_back(goal);
  move_path_ = opt_path;
  return opt_path;
}

void Hero::move() {}

void Hero::step() {}