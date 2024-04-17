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

class Player {
 private:
  bool is_ai_;

 public:
  Player(/* args */);
  ~Player();
};

#endif