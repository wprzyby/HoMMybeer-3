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
#include <iostream>
#include <memory>
#include <queue>
#include <stack>

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
      throw WrongObjectPlacementException(
          wrong_coord, *game_map_.getField(wrong_coord).value()->getObject(),
          *obj);
    }
  }
}

Player* Game::getPlayer(int idx) {
  if (players_in_game_.size() <= idx) {
    return nullptr;
  }
  return &players_in_game_[idx];
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
    if (!game_map_.getField(coord_to_check).has_value() ||
        !game_map_.getField(coord_to_check).value()->isWalkable()) {
      return false;
    }
  }

  for (FieldCoords coord_to_set : obj_to_add->occupiedFields()) {
    if (!game_map_.setObjectTo(coord_to_set, obj_to_add)) {
      return false;
    }
  }
  obj_to_add->setParent(this);
  map_objects_.push_back(obj_to_add);
  return true;
}

std::optional<std::pair<Path, MoveCosts>> Game::findPath_(FieldCoords path_to) {
  FieldCoords current_hero_location =
      players_in_game_[curr_player_idx].getCurrentHero()->getHeroCoords();
  Path retpath = {};
  MoveCosts retcosts = {};
  // pathfinding
  std::set<FieldCoords> all_nodes =
      game_map_.constructGraph(current_hero_location);
  if (!all_nodes.contains(path_to)) {
    return {};
  }
  std::map<FieldCoords, std::pair<int, FieldCoords>> nodes_w_costs_previous;
  for (FieldCoords coord : all_nodes) {
    nodes_w_costs_previous[coord] = {0xFFFFFFF, {}};
  }
  auto getAdjacent = [&](FieldCoords coords) {
    std::vector<FieldCoords> adjacent = {
        coords + FieldCoords{1, 0},   coords + FieldCoords{1, 1},
        coords + FieldCoords{0, 1},   coords + FieldCoords{-1, 0},
        coords + FieldCoords{-1, -1}, coords + FieldCoords{0, -1},
        coords + FieldCoords{1, -1},  coords + FieldCoords{-1, 1}};
    std::vector<FieldCoords> retv = {};
    for (FieldCoords coord_to_check : adjacent) {
      if (all_nodes.contains(coord_to_check)) {
        retv.push_back(coord_to_check);
      }
    }

    return retv;
  };

  auto cmp = [](const std::pair<FieldCoords, int>& left,
                const std::pair<FieldCoords, int>& right) {
    return left.second > right.second;
  };
  std::priority_queue<std::pair<FieldCoords, int>,
                      std::vector<std::pair<FieldCoords, int>>, decltype(cmp)>
      queue;
  nodes_w_costs_previous[current_hero_location] = {0, {}};
  std::set<FieldCoords> visited_fields;
  queue.push({current_hero_location, 0});
  while (!visited_fields.contains(path_to)) {
    std::vector<FieldCoords> adjacent_fields = getAdjacent(queue.top().first);
    for (FieldCoords adjacent : adjacent_fields) {
      if (!visited_fields.contains(adjacent)) {
        int next_resistance =
            (std::abs(adjacent.first - queue.top().first.first) +
                 std::abs(adjacent.second - queue.top().first.second) ==
             1)
                ? Field::resistance.at(
                      game_map_.getField(adjacent).value()->getTerrainType())
                : static_cast<int>(1.41 *
                                   static_cast<float>(Field::resistance.at(
                                       game_map_.getField(adjacent)
                                           .value()
                                           ->getTerrainType())));
        int alt_cost =
            nodes_w_costs_previous[queue.top().first].first + next_resistance;
        if (nodes_w_costs_previous[adjacent].first > alt_cost) {
          nodes_w_costs_previous[adjacent] = {alt_cost, queue.top().first};
          queue.push({adjacent, alt_cost});
        }
      }
    }
    visited_fields.insert(queue.top().first);
    queue.pop();
  }

  FieldCoords current = path_to;
  int current_cost = Field::resistance.at(
      game_map_.getField(current).value()->getTerrainType());

  while (current != current_hero_location) {
    retpath.push(current);
    retcosts.push(current_cost);
    current = nodes_w_costs_previous[current].second;
    current_cost = Field::resistance.at(
        game_map_.getField(current).value()->getTerrainType());
  }

  return std::pair<Path, MoveCosts>{retpath, retcosts};
}

void Game::executeAction(FieldCoords coords) {
  // TODO add action when player clicks at himself (or other hero)

  if (!game_map_.getField(coords).has_value()) {
    return;
  }

  if (game_map_.getField(coords).value()->isWalkable()) {
    auto found_shortest = findPath_(coords);
    if (!found_shortest.has_value()) {
      return;
    }
    if (!this->getCurrentPlayer()->getCurrentHero()->moveAlong(
            found_shortest.value().first, found_shortest.value().second)) {
      this->getCurrentPlayer()->getCurrentHero()->setMovePath(
          found_shortest.value().first);
    }
    return;
  }

  // checks if the object clicked has any fields adjacent to hero
  for (FieldCoords coord :
       game_map_.getField(coords).value()->getObject()->occupiedFields()) {
    if (Field::isAdjacent(
            coord, getCurrentPlayer()->getCurrentHero()->getHeroCoords())) {
      game_map_.getField(coords).value()->getObject()->objectAction();
      return;
    }
  }
  return;
}