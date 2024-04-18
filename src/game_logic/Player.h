/**
 * @file Player.h
 * @author Piotr Kluba
 * @brief Structure used to store the properties of the players and for them to
 * take certain actions
 * @version 0.1
 * @date 2024-04-16
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef SRC_GAME_LOGIC_PLAYER_H_
#define SRC_GAME_LOGIC_PLAYER_H_

#include <Hero.h>

#include <memory>
#include <optional>
#include <vector>

class Player {
 private:
  bool is_ai_;
  std::vector<Hero> players_heros_;

 public:
  Player(bool ai);
  ~Player();
  const Hero* getHero(int idx);  // TODO: overload with ex name
  void addHero(std::string name, FieldCoords spawn_field_coords);
};

#endif