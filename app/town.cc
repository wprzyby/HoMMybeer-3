#include <TownView.h>

#include <SFML/Graphics.hpp>

#include "Config.h"
#include "common.h"

int main() {
  sf::Font font;
  Config::getInstance()->loadTownData(getProjectPath() + "/assets/TownsMetadata.json");
  font.loadFromFile(getProjectPath() + "/assets/arial.ttf");
  TownView town_view(Faction::CASTLE, {800, 600}, font);
  sf::RenderWindow window(sf::VideoMode(800, 600), "HoMMyBeer 3");
  while (window.isOpen()) {
    // check all the window's events that were triggered since the last
    // iteration of the loop
    sf::Event event{};
    while (window.pollEvent(event)) {
      // "close requested" event: we close the window
      if (event.type == sf::Event::Closed) {
        window.close();
      }
    }
    window.draw(town_view);
    window.display();
  }
}