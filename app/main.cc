/**
 * @file main.cc
 * @author Wojciech Przybylski
 * @brief Main game loop
 * @copyright Wojciech Przybylski (c) 2024
 *
 */

#include <Game.h>
#include <MapUtils.h>
#include <MapView.h>
#include <Session.h>
#include <Welcome.h>

#include <SFML/Graphics.hpp>

constexpr static int kWindowWidth = 800;
constexpr static int kWindowHeight = 600;
constexpr static int kTileTypesCount = 4;
const static sf::Vector2u kMapTileSize{256, 192};

int main() {
  Session* session = Session::getInstance();
  MapInfo map_info = generateGrassMap(MapSize::S, 1);
  std::vector<Player> players{
      Player(false, Faction::CASTLE, map_info.starting_locations[0])};
  session->newGame(map_info.map, players, Difficulty::EASY);
  MapView map_view(kWindowWidth, kWindowHeight);
  map_view.loadTileset("./assets/MapTiles.png", kMapTileSize, kTileTypesCount);
  map_view.loadFieldArray(session->game->getMap()->getFieldArray());

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

    // draw map
    window.draw(map_view);

    // end the current frame
    window.display();
  }

  return 0;
}
