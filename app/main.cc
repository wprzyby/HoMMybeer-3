/**
 * @file main.cc
 * @author Wojciech Przybylski
 * @brief Main game loop
 * @copyright Wojciech Przybylski (c) 2024
 *
 */

#include <BorderView.h>
#include <Config.h>
#include <ControlsView.h>
#include <Field.h>
#include <Game.h>
#include <HeroView.h>
#include <MapUtils.h>
#include <MapView.h>
#include <MapWindowController.h>
#include <ObjectsView.h>
#include <ResourcesView.h>
#include <Session.h>
#include <common.h>

#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <cmath>

const static sf::Vector2u MAIN_WINDOW_SIZE = sf::Vector2u{800, 600};
const static sf::Vector2u GAME_WINDOW_SIZE = sf::Vector2u{512, 512};
const static sf::Vector2u GAME_WINDOW_OFFSET = sf::Vector2u{10, 10};

int main() {
  Session* session = Session::getInstance();
  Config* conf = Config::getInstance();
  MapWindowController controller =
      MapWindowController(GAME_WINDOW_SIZE, GAME_WINDOW_OFFSET);
  std::string path = getProjectPath();
  conf->loadData(path + "/assets/ObjectsMetadata.json");
  MapInfo map_info = generateLargeExampleMap();
  std::vector<Player> players{
      Player(false, Faction::CASTLE, map_info.starting_locations[0])};
  std::vector<std::shared_ptr<MapObject>> static_map_objects =
      generateExampleStaticObjects();
  std::vector<std::shared_ptr<MapObject>> pickable_map_objects =
      generateExamplePickableObjects();
  std::vector<std::shared_ptr<MapObject>> starting_objects(
      static_map_objects.size() + pickable_map_objects.size());
  std::merge(static_map_objects.begin(), static_map_objects.end(),
             pickable_map_objects.begin(), pickable_map_objects.end(),
             starting_objects.begin());
  session->newGame(map_info.map, players, Difficulty::EASY, starting_objects);
  MapView map_view;
  MapView::loadTileset(path + "/assets/Terrains.png");
  map_view.setMap(session->game.getMap()->getFieldArray(), sf::Vector2u({0, 0}),
                  MapView::MAP_TILE_SIZE);
  HeroView hero_view;
  HeroView::loadTileSet(path + "/assets/Heroes.png");
  hero_view.setHeroes(session->game.heroesInGame(), sf::Vector2u({0, 0}),
                      MapView::MAP_TILE_SIZE);
  map_view.setPosition(sf::Vector2f(GAME_WINDOW_OFFSET));
  hero_view.setPosition(sf::Vector2f(GAME_WINDOW_OFFSET));
  BorderView border_view;
  border_view.setBorder();
  ControlsView controls_view;
  controls_view.setControls();
  ResourcesView resources_view;
  resources_view.setResources();
  ObjectsView objects_view;
  ObjectsView::loadTileSet({{"Cities", path + "/assets/Cities.png"},
                            {"Mountain", path + "/assets/Mountains.png"},
                            {"Tree", path + "/assets/Trees.png"},
                            {"Resources", path + "/assets/Resources.png"},
                            {"Mines", path + "/assets/Mines.png"}});
  objects_view.setPosition(sf::Vector2f(GAME_WINDOW_OFFSET));
  objects_view.setObjects(session->game.objectsInGame(), sf::Vector2u({0, 0}),
                          MapView::MAP_TILE_SIZE);
  // create the window
  sf::RenderWindow window(sf::VideoMode(MAIN_WINDOW_SIZE.x, MAIN_WINDOW_SIZE.y),
                          "HoMMyBeer 3");
  window.setKeyRepeatEnabled(false);

  // run the program as long as the window is open
  while (window.isOpen()) {
    // check all the window's events that were triggered since the last
    // iteration of the loop
    sf::Event event{};
    while (window.pollEvent(event)) {
      // "close requested" event: we close the window
      if (event.type == sf::Event::Closed) {
        window.close();
      }
      // if (event.type == sf::Event::MouseButtonPressed) {
      //   if (event.mouseButton.button == sf::Mouse::Left) {
      //     // int x_clicked = event.mouseButton.x;
      //     // int y_clicked = event.mouseButton.y;
      //     // Session* sess = Session::getInstance();
      //     // int map_width = sess->game->getMap()->getWidth();
      //     // sess->game->moveCurrPlayer(
      //     //     FieldCoords{std::floor(x_clicked * map_width /
      //     kWindowWidth),
      //     //                 std::floor(y_clicked * map_width /
      //     //                 kWindowHeight)});
      //     // hero_view.setHero(
      //     //     *(sess->game->getCurrentPlayer()->getCurrentHero()));
      //   }
      //   if (event.mouseButton.button == sf::Mouse::Right) {
      //   }
      // }
      if (event.type == sf::Event::KeyPressed) {
        sf::Vector2i translation;
        if (event.key.scancode == sf::Keyboard::Scan::Right) {
          translation = {16, 0};
        } else if (event.key.scancode == sf::Keyboard::Scan::Left) {
          translation = {-16, 0};
        } else if (event.key.scancode == sf::Keyboard::Scan::Up) {
          translation = {0, -16};
        } else if (event.key.scancode == sf::Keyboard::Scan::Down) {
          translation = {0, 16};
        }
        controller.scrollGameView(translation, map_view, hero_view,
                                  objects_view, Session::getInstance()->game);
      }
    }

    // clear the window with black color
    window.clear(sf::Color::Black);

    // draw map
    window.draw(map_view);
    window.draw(objects_view);
    window.draw(hero_view);
    window.draw(border_view);
    window.draw(controls_view);
    window.draw(resources_view);

    // end the current frame
    window.display();
  }

  return 0;
}
