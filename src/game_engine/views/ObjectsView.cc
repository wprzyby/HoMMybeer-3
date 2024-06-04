/**
 * @file ObjectsView.cc
 * @author Piotr Kluba
 * @brief Entity for drawing visible objects
 * @date 2024-06-02
 *
 * @copyright Copyright (c) 2024
 *
 */
#include <ObjectsView.h>

void ObjectsView::draw(sf::RenderTarget& target,
                       sf::RenderStates states) const {
  states.transform *= getTransform();

  // draw the vertex array
  for (const sf::Sprite& sprite : sprites_) {
    target.draw(sprite, states);
  }
}

std::map<std::string, sf::Texture> ObjectsView::textures_ = {
    {"Cities", sf::Texture()},
    {"Mountain", sf::Texture()},
    {"Tree", sf::Texture()},
    {"Resources", sf::Texture()},
    {"Mines", sf::Texture()}};

bool ObjectsView::loadTileSet(
    const std::map<std::string, std::string>& tileset_paths) {
  for (const auto& entry : tileset_paths) {
    if (!ObjectsView::textures_.at(entry.first).loadFromFile(entry.second)) {
      return false;
    }
  }
  return true;
}

bool ObjectsView::visible(const MapObject* object, sf::Vector2u field_offset) {
  for (FieldCoords coord_to_check : object->occupiedFields()) {
    if (coord_to_check.first >= field_offset.y &&
        coord_to_check.second >= field_offset.x &&
        coord_to_check.first <=
            field_offset.y + (MapView::VISIBLE_MAP_WIDTH + 1) &&
        coord_to_check.second <=
            field_offset.x + (MapView::VISIBLE_MAP_WIDTH + 1)) {
      return true;
    }
  }
  return false;
}

void ObjectsView::setObjects(std::vector<const MapObject*> objects,
                             sf::Vector2u field_offset,
                             sf::Vector2u tile_size) {
  sprites_.clear();
  auto cmp = [](const MapObject* left, const MapObject* right) {
    if (left->getOrigin().second == right->getOrigin().second) {
      return left->getOrigin().first > right->getOrigin().first;
    }
    return left->getOrigin().second > right->getOrigin().second;
  };

  std::sort(objects.begin(), objects.end(), cmp);
  for (const MapObject* object : objects) {
    if (!visible(object, field_offset)) {
      continue;
    }
    std::map<std::string, std::string> specs = object->getSpecs();
    sf::Sprite sprite(ObjectsView::textures_.at(specs.at("SubType")));
    std::vector<int> texture_size_inp = Config::getInstance()
                                            ->getMetadata()
                                            .at(specs.at("ObjectType"))
                                            .at(specs.at("SubType"))
                                            .at("texture_size")
                                            .at(specs.at("Variant"));
    std::vector<int> texture_origin_inp = Config::getInstance()
                                              ->getMetadata()
                                              .at(specs.at("ObjectType"))
                                              .at(specs.at("SubType"))
                                              .at("starts_at")
                                              .at(specs.at("Variant"));
    sf::Vector2i texture_size({texture_size_inp[0], texture_size_inp[1]});
    sf::Vector2i texture_origin({texture_origin_inp[0], texture_origin_inp[1]});
    if ((specs.at("SubType") == "Mines" || specs.at("SubType") == "Cities") &&
        object->getOwner() != -1) {
      int texture_row_height = Config::getInstance()
                                   ->getMetadata()
                                   .at(specs.at("ObjectType"))
                                   .at(specs.at("SubType"))
                                   .at("row_height");
      texture_origin.y =
          texture_origin.y + texture_row_height * (object->getOwner() + 1);
    }

    sprite.setTextureRect(sf::IntRect(texture_origin, texture_size));
    FieldCoords origin = object->getOrigin();
    if (origin.first < field_offset.y && origin.second < field_offset.x) {
      sprite.setOrigin(
          sf::Vector2f(-(origin.second - static_cast<int>(field_offset.x)) *
                           static_cast<int>(tile_size.x),
                       -(origin.first - static_cast<int>(field_offset.y)) *
                           static_cast<int>(tile_size.y)));
      sprite.setPosition(sf::Vector2f({0, 0}));
    } else if (origin.first < static_cast<int>(field_offset.y)) {
      sprite.setOrigin(
          sf::Vector2f(0, -(origin.first - static_cast<int>(field_offset.y)) *
                              static_cast<int>(tile_size.y)));
      sprite.setPosition(
          sf::Vector2f((origin.second - static_cast<int>(field_offset.x)) *
                           static_cast<int>(tile_size.x),
                       0));
    } else if (origin.second < static_cast<int>(field_offset.x)) {
      sprite.setOrigin(
          sf::Vector2f(-(origin.second - static_cast<int>(field_offset.x)) *
                           static_cast<int>(tile_size.x),
                       0));
      sprite.setPosition(
          sf::Vector2f(0, (origin.first - static_cast<int>(field_offset.y)) *
                              static_cast<int>(tile_size.y)));
    } else {
      sprite.setPosition(
          sf::Vector2f((origin.second - static_cast<int>(field_offset.x)) *
                           static_cast<int>(tile_size.x),
                       (origin.first - static_cast<int>(field_offset.y)) *
                           static_cast<int>(tile_size.y)));
    }
    sprites_.push_back(sprite);
  }
}