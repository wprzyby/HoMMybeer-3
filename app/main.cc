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
#include <MapView.h>
#include <Welcome.h>

#include <SFML/Graphics.hpp>

constexpr static int kWindowWidth = 800;
constexpr static int kWindowHeight = 600;

int main() {
  MapInfo map_info = generateGrassMap(MapSize::S, 1);
  std::vector<Player> players{
      Player(false, Faction::CASTLE, map_info.starting_locations[0])};
  Game game(players, map_info.map);
  MapView map_view(kWindowWidth, kWindowHeight);
  map_view.loadTileset("./assets/MapTiles.png", sf::Vector2u(256, 192), 4);
  map_view.loadFieldArray(map_info.map.getFieldArray());

  // create the window
  sf::RenderWindow window(sf::VideoMode(kWindowWidth, kWindowHeight),
                          "My window");

  // run the program as long as the window is open
  while (window.isOpen()) {
    // check all the window's events that were triggered since the last
    // iteration of the loop
    sf::Event event;
    while (window.pollEvent(event)) {
      // "close requested" event: we close the window
      if (event.type == sf::Event::Closed) window.close();
    }

    // clear the window with black color
    window.clear(sf::Color::Black);

    window.draw(map_view);

    // end the current frame
    window.display();
  }

  return 0;
}