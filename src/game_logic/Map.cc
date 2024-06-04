/**
 * @file Map.cc
 * @author Piotr Kluba
 * @brief Functionality generating and managing the game map
 * @copyright Copyright (c) 2024
 */

#include <Map.h>

#include <set>

std::optional<const Field*> Map::getField(FieldCoords coords) const {
  int x_coord = coords.first;
  int y_coord = coords.second;
  if (x_coord >= width_ || x_coord < 0 || y_coord >= width_ || y_coord < 0) {
    return {};
  }
  return &field_array_[x_coord][y_coord];
}

Field* Map::fieldToModify(FieldCoords coords) {
  int x_coord = coords.first;
  int y_coord = coords.second;
  if (x_coord >= width_ || x_coord < 0 || y_coord >= width_ || y_coord < 0) {
    return {};
  }
  return (&(field_array_[x_coord][y_coord]));
}

std::set<FieldCoords> Map::getViableAdjacent(FieldCoords coords) const {
  std::set<FieldCoords> adjacent = {
      coords + FieldCoords{1, 0},   coords + FieldCoords{1, 1},
      coords + FieldCoords{0, 1},   coords + FieldCoords{-1, 0},
      coords + FieldCoords{-1, -1}, coords + FieldCoords{0, -1},
      coords + FieldCoords{1, -1},  coords + FieldCoords{-1, 1}};
  std::set<FieldCoords> viable_adjacent = {};
  for (FieldCoords coords_to_check : adjacent) {
    if (getField(coords_to_check).has_value() &&
        getField(coords_to_check).value()->isWalkable()) {
      viable_adjacent.insert(coords_to_check);
    }
  }
  return viable_adjacent;
}

std::set<FieldCoords> Map::constructGraph(FieldCoords seed_coords) const {
  std::set<FieldCoords> graph = {};
  std::set<FieldCoords> unvisited = {seed_coords};
  while (unvisited.size() > 0) {
    std::set<FieldCoords> viable_adjacent =
        getViableAdjacent(*unvisited.begin());
    std::set<FieldCoords> to_visit = {};
    for (FieldCoords viable_coords : viable_adjacent) {
      if (!graph.contains(viable_coords) &&
          !unvisited.contains(viable_coords)) {
        to_visit.insert(viable_coords);
      }
    }
    graph.insert(*unvisited.begin());
    unvisited.erase(unvisited.begin());
    unvisited.insert(to_visit.begin(), to_visit.end());
  }
  return graph;
}