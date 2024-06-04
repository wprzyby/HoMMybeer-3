/**
 * @file PathView.cc
 * @author Piotr Kluba
 * @brief View for drawing the proposed movement path of the hero on the map
 * @copyright Copyright (c) 2024
 */

#include <MapView.h>
#include <PathView.h>

#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <utility>

#include "Hero.h"

void PathView::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  states.transform *= getTransform();
  // apply the tileset texture

  // draw the vertex array
  target.draw(vertices_, states);
}

bool PathView::visible(FieldCoords point_on_path, sf::Vector2u field_offset) {
  return point_on_path.first >= field_offset.y &&
         point_on_path.second >= field_offset.x &&
         point_on_path.first <=
             field_offset.y + (MapView::VISIBLE_MAP_WIDTH + 1) &&
         point_on_path.second <=
             field_offset.x + (MapView::VISIBLE_MAP_WIDTH + 1);
}

void PathView::setPath(const Hero* hero, sf::Vector2u field_offset,
                       sf::Vector2u tile_size) {
  vertices_.clear();
  vertices_.setPrimitiveType(sf::Lines);
  std::pair<Path, int> path = hero->getPathInParts();
  path.first.push(hero->getHeroCoords());
  int path_step = 0;
  while (!path.first.empty()) {
    if (!visible(path.first.top(), field_offset)) {
      path.first.pop();
      ++path_step;
      continue;
    }
    sf::Vertex line_start;
    sf::Vertex line_end;
    if (path_step < path.second) {
      line_start.color = sf::Color::Green;
      line_end.color = sf::Color::Green;
    } else {
      line_start.color = sf::Color::Red;
      line_end.color = sf::Color::Red;
    }

    line_start.position =
        sf::Vector2f((path.first.top().second - field_offset.x) * tile_size.x +
                         (0.5 * tile_size.x),
                     (path.first.top().first - field_offset.y) * tile_size.y +
                         (0.5 * tile_size.y));
    path.first.pop();
    if (!path.first.empty()) {
      line_end.position = sf::Vector2f(
          (path.first.top().second - field_offset.x) * tile_size.x +
              (0.5 * tile_size.x),
          (path.first.top().first - field_offset.y) * tile_size.y +
              (0.5 * tile_size.y));
      vertices_.append(line_start);
      vertices_.append(line_end);
    }

    ++path_step;
  }
}