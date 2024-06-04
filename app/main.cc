/**
 * @file main.cc
 * @author Wojciech Przybylski, Piotr Kluba
 * @brief Main game loop
 * @copyright Copyright (c) 2024
 *
 */

#include <BattleWindowController.h>
#include <BorderView.h>
#include <Config.h>
#include <ControlsView.h>
#include <EventHandler.h>
#include <Field.h>
#include <Game.h>
#include <HeroView.h>
#include <MainView.h>
#include <MainWindowController.h>
#include <MapView.h>
#include <MapWindowController.h>
#include <ObjectsView.h>
#include <PathView.h>
#include <ResourcesView.h>
#include <Session.h>
#include <TownView.h>
#include <TownWindowController.h>
#include <UnitConfig.h>
#include <game_logic_utils.h>
#include <map_generation.h>

#include <SFML/Graphics.hpp>
#include <cmath>

const static sf::Vector2u MAIN_WINDOW_SIZE = sf::Vector2u{800, 600};
const static sf::Vector2u GAME_WINDOW_SIZE = sf::Vector2u{512, 512};
const static sf::Vector2u GAME_WINDOW_OFFSET = sf::Vector2u{10, 10};

int main() {
  Session* session = Session::getInstance();
  Session::setSessionState(SessionState::START_MENU);
  Config* conf = Config::getInstance();

  std::string path = getProjectPath();
  conf->loadTownData(path + "/metadata/TownsMetadata.json");
  conf->loadObjectsData(path + "/metadata/ObjectsMetadata.json");
  conf->loadBattleData(path + "/metadata/BattlegroundMetadata.json");
  conf->loadUnitConfig(path + "/metadata/UnitConfig.json");

  MapWindowController::loadFont(path + "/assets/fonts/OldLondon.ttf");
  TownWindowController::loadFont(path + "/assets/fonts/Augusta.ttf");
  MainView::loadTileset(path + "/assets/MainPage.png");
  MainWindowController main_controller(MAIN_WINDOW_SIZE);
  MapView::loadTileset(path + "/assets/map/Terrains.png");
  HeroView::loadTileSet(path + "/assets/map/Heroes.png");
  BorderView border_view{sf::Vector2f(GAME_WINDOW_OFFSET),
                         sf::Vector2f(GAME_WINDOW_SIZE)};
  ResourcesView::loadTileset(path + "/assets/map/Resources.png");
  ObjectsView::loadTileSet({{"Cities", path + "/assets/map/Cities.png"},
                            {"Mountain", path + "/assets/map/Mountains.png"},
                            {"Tree", path + "/assets/map/Trees.png"},
                            {"Resources", path + "/assets/map/Resources.png"},
                            {"Mines", path + "/assets/map/Mines.png"}});
  MapWindowController map_controller = MapWindowController(
      GAME_WINDOW_SIZE, GAME_WINDOW_OFFSET, MAIN_WINDOW_SIZE);
  TownWindowController town_controller = TownWindowController(MAIN_WINDOW_SIZE);
  BattleWindowController battle_window_controller(GAME_WINDOW_SIZE);
  EventHandler event_handler({&map_controller, &main_controller,
                              &battle_window_controller, &town_controller});
  // create the window
  sf::RenderWindow window(sf::VideoMode(MAIN_WINDOW_SIZE.x, MAIN_WINDOW_SIZE.y),
                          "HoMMyBeer 3",
                          sf::Style::Titlebar | sf::Style::Close);

  window.setKeyRepeatEnabled(false);

  // run the program as long as the window is open

  while (window.isOpen()) {
    // check all the window's events that were triggered since the last
    // iteration of the loop

    sf::Event event{};
    if (Session::getSessionState() == SessionState::LOAD_GAME) {
      event_handler.handle(event, Session::getSessionState(), session->game);
      Session::setSessionState(SessionState::IN_GAME);
    }
    if (Session::getSessionState() == SessionState::BATTLE_AI_REFRESH) {
      event_handler.handle(event, Session::getSessionState(), session->game);
    }
    if (Session::getSessionState() == SessionState::REFRESH) {
      event_handler.handle(event, Session::getSessionState(), session->game);
      Session::setSessionState(SessionState::IN_GAME);
    }
    while (window.pollEvent(event)) {
      // "close requested" event: we close the window

      if (event.type == sf::Event::Closed) {
        window.close();
      }
      event_handler.handle(event, Session::getSessionState(), session->game);
    }

    // clear the window with black color
    window.clear(sf::Color::Black);

    // drawing depending on state

    switch (Session::getSessionState()) {
      case SessionState::IN_GAME:
        window.draw(map_controller);
        break;

      case SessionState::IN_BATTLE:
      case SessionState::BATTLE_AI_REFRESH:
        window.draw(battle_window_controller);
        break;

      case SessionState::IN_CASTLE:
        window.draw(town_controller);
        break;

      case SessionState::START_MENU:
        window.draw(main_controller);
        break;

      case SessionState::LOAD_GAME:
      case SessionState::LOAD_CASTLE:
      case SessionState::REFRESH:
      case SessionState::LOAD_BATTLE:
        break;
    }

    // end the current frame
    window.display();
  }

  return 0;
}
