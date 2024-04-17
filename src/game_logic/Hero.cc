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
using namespace std;
Hero::~Hero() {}

const pair<int, int> Hero::getHeroCoords() {
  return field_at_->getFieldCoords();
}