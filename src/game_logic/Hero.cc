/**
 * @file Hero.cc
 * @author Piotr Kluba
 * @brief Functionality responsible for the single hero
 * @version 0.1
 * @date 2024-04-17
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <Hero.h>
#include <Session.h>

using namespace std;

bool Hero::moveAlong(Path updated_path, MoveCosts costs) {
  if (updated_path != move_path_) {
    return false;
  }

  while (!(move_path_.empty()) && (energy_ - costs.top() > 0)) {
    step(move_path_.top());
    move_path_.pop();
    energy_ = energy_ - costs.top();
    costs.pop();
  }
  return true;
}

void Hero::step(FieldCoords step_to) {
  // checking if field is adjacent and if is possible to move to
  hero_coords_ = step_to;
}

std::pair<Path, int> Hero::getPathInParts() const {
  Path analyzed_path = move_path_;
  int analyzed_energy = energy_;
  MoveCosts analyzed_costs = current_path_costs_;

  int retval = 0;
  while (!(analyzed_path.empty()) &&
         (analyzed_energy - analyzed_costs.top() > 0)) {
    analyzed_path.pop();
    analyzed_energy = analyzed_energy - analyzed_costs.top();

    analyzed_costs.pop();
    ++retval;
  }
  return {move_path_, retval};
}