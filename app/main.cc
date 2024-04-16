/**
 * @file main.cc
 * @author Wojciech Przybylski
 * @brief
 * @version 0.1
 * @date 2024-04-07
 *
 * @copyright Wojciech Przybylski (c) 2024
 *
 */

#include <Welcome.h>
#include <Session.h>

int main() {
  welcome();
  Session* game_session = Session::getInstance();
  return 0;
}