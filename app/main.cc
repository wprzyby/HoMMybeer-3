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

#include <Game.h>
#include <MapUtils.h>
#include <Welcome.h>

int main() {
  welcome();
  MapInfo map_info = generateGrassMap(MapSize::S, 1);
  std::vector<Player> players{
      Player(false, Faction::CASTLE, map_info.starting_locations[0])};
  Game game(players, map_info.map);
  return 0;
}