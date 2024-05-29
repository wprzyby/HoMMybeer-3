/**
 * @file MapObject.cc
 * @author Piotr Kluba
 * @brief Contains the funcionality asociated with all objects on the map
 * @copyright Copyright (c) 2024
 *
 */

#ifndef SRC_GAME_LOGIC_MAPOBJECT_H_
#define SRC_GAME_LOGIC_MAPOBJECT_H_

#include <Config.h>
#include <Player.h>

#include <common.hpp>
#include <variant>
#include <vector>

class GeologicalObject;
class PickableResource;
class ResourceGenerator;
class City;

using MapObject =
    std::variant<GeologicalObject, PickableResource, ResourceGenerator, City>;

class MapObjectBase {
 private:
  static int current_id_;
  FieldCoords origin_;  // origin is always placed in the top left corner of the
                        // object (according to graphics)

 protected:
  std::vector<FieldCoords> space_taken_;
  int id_;

 public:
  MapObjectBase(FieldCoords origin, std::vector<FieldCoords> space_taken)
      : origin_(origin), space_taken_(space_taken), id_(current_id_++) {}
  ~MapObjectBase() = default;
  const std::vector<FieldCoords> occupiedFields() const;
  int getId() const { return id_; }
  FieldCoords getOrigin() const { return origin_; }
};

class GeologicalObject : public MapObjectBase {
 private:
  GeologicalStructureType struct_type_;
  int variant_;

 public:
  GeologicalObject(FieldCoords origin, GeologicalStructureType struct_type,
                   int variant);
  template <typename Game>
  std::optional<bool> objectAction(Game* /*unused*/) {
    return {};
  }
};

class PickableResource : public MapObjectBase {
 private:
  ResourceType resource_type_;
  int amount_;

 public:
  PickableResource(FieldCoords origin, ResourceType resource_type, int amount);
  template <typename Game>
  std::optional<bool> objectAction(Game* game) {
    bool success = game->getCurrentPlayer()->updateResourceQuantity(
        resource_type_, amount_);
    return game->deleteMapObject(id_) && success;
  }
};

class ResourceGenerator : public MapObjectBase {
 private:
  ResourceType resource_type_;
  int weekly_income_;
  int owner_id_;

 public:
  ResourceGenerator(FieldCoords origin, ResourceType resource_type,
                    int weekly_income);

  template <typename Game>
  std::optional<bool> objectAction(Game* game)  {
    if (game->getCurrentPlayer()->changeIncome(resource_type_,
                                               weekly_income_)) {
      owner_id_ = game->getCurrPlayerId();
      return true;
    }
    return false;
  }
  int getOwnerId() { return owner_id_; }
};

class City : public MapObjectBase {
 private:
  Faction type_;
  int owner_id_;

 public:
  City(FieldCoords origin, Faction type, int owner_id = -1);

  template <typename Game>
  std::optional<bool> objectAction(Game* /*unused*/) {
    return true;
  }
};

#endif