/**
 * @file Game.cc
 * @author Piotr Kluba
 * @brief Base game class responsible for managing the state of the current game
 * @version 0.1
 * @date 2024-04-15
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <Game.h>

#include <algorithm>
#include <memory>

using namespace std;

Game::Game(std::vector<Player> players, Map map,
           std::vector<std::shared_ptr<MapObject>> starting_map_objects)
    : players_in_game_(players), game_map_(map), curr_player_idx(0) {
  for (auto obj : starting_map_objects) {
    if (!addMapObject(obj)) {
      FieldCoords wrong_coord;
      for (FieldCoords coord_to_check : obj->occupiedFields()) {
        if (!game_map_.getField(coord_to_check).value()->isWalkable()) {
          wrong_coord = coord_to_check;
          break;
        }
      }
      throw WrongObjectPlacementException(wrong_coord, *game_map_.getField(wrong_coord).value()->getObject(), *obj);
    }
  }
}

Player* Game::getPlayer(int idx) {
  if (players_in_game_.size() <= idx) {
    return nullptr;
  }
  return &players_in_game_[idx];
}

bool Game::moveCurrPlayer(FieldCoords coords) {
  if (not game_map_.getField(coords).value()->isWalkable()) {
    return false;
  }
  this->getCurrentPlayer()->getCurrentHero()->moveTo(coords);
  return true;
}

bool Game::deleteMapObject(int id) {
  int idx_to_delete = 0;
  auto it = find_if(
      map_objects_.begin(), map_objects_.end(),
      [&id](const shared_ptr<MapObject> obj) { return obj->getId() == id; });
  if (it != map_objects_.end()) {
    idx_to_delete = distance(map_objects_.begin(), it);
  } else {
    return false;
  }

  std::vector<FieldCoords> fields_to_empty =
      map_objects_[idx_to_delete]->occupiedFields();
  for (FieldCoords coords : fields_to_empty) {
    game_map_.deleteObjectFrom(coords);
  }
  map_objects_.erase(map_objects_.begin() + idx_to_delete);
  return true;
}

bool Game::addMapObject(shared_ptr<MapObject> obj_to_add) {
  FieldCoords origin = obj_to_add->getOrigin();
  for (FieldCoords coord_to_check : obj_to_add->occupiedFields()) {
    if (!game_map_.getField(coord_to_check).value()->isWalkable()) {
      return false;
    }
  }
  map_objects_.push_back(obj_to_add);

  for (FieldCoords coord_to_set : obj_to_add->occupiedFields()) {
    if (!game_map_.setObjectTo(coord_to_set, obj_to_add)) {
      return false;
    }
  }
  return true;
}
