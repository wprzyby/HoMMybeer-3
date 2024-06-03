/**
 * @file ObjectsView.h
 * @author Piotr Kluba
 * @brief Entity for drawing visible objects
 * @date 2024-06-02
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef SRC_GAME_ENGINE_VIEWS_OBJECTS_VIEW_H_
#define SRC_GAME_ENGINE_VIEWS_OBJECTS_VIEW_H_

#include <Hero.h>
#include <MapView.h>
#include <Session.h>

#include <SFML/Graphics.hpp>
#include <map>
#include <string>

class ObjectsView : public sf::Drawable, public sf::Transformable {
 private:
  static std::map<std::string, sf::Texture> textures_;
  std::vector<sf::Sprite> sprites_;
  static bool visible(const MapObject* object, sf::Vector2u field_offset);

 public:
  ObjectsView() = default;
  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
  static bool loadTileSet(
      const std::map<std::string, std::string>& tileset_paths);
  void setObjects(std::vector<const MapObject*> map_objects,
                  sf::Vector2u field_offset, sf::Vector2u tile_size);
};

#endif