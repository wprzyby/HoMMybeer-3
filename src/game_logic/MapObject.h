/**
 * @file MapObject.cc
 * @author Piotr Kluba
 * @brief Contains the functionality associated with all objects on the map
 * @copyright Copyright (c) 2024
 *
 */

#ifndef SRC_GAME_LOGIC_MAPOBJECT_H_
#define SRC_GAME_LOGIC_MAPOBJECT_H_

#include <Config.h>
#include <Player.h>

#include <map>
#include <string>
#include <vector>

using FieldCoords = std::pair<int, int>;

class Game;
class MapObject {
 private:
  static int current_id_;
  FieldCoords origin_;  // origin is always placed in the top left corner of the
                        // object (according to graphics)

 protected:
  Game* parent_;
  std::vector<FieldCoords> space_taken_;
  int id_;

 public:
  MapObject(FieldCoords origin, Game* parent,
            std::vector<FieldCoords> space_taken)
      : origin_(origin),
        parent_(parent),
        space_taken_(space_taken),
        id_(current_id_++) {}
  ~MapObject() = default;
  virtual std::optional<bool>
  objectAction() = 0;  // will return true if has action and
                       // completes it, false if has no action
  const std::vector<FieldCoords> occupiedFields() const;
  void setParent(Game* new_parent) { parent_ = new_parent; }
  int getId() const { return id_; }
  FieldCoords getOrigin() const { return origin_; }
  virtual std::map<std::string, std::string> getSpecs() const = 0;
};
class GeologicalObject : public MapObject {
 private:
  GeologicalStructureType struct_type_;
  int variant_;

 public:
  GeologicalObject(FieldCoords origin, Game* parent,
                   GeologicalStructureType struct_type, int variant);
  std::optional<bool> objectAction() override { return {}; }
  std::map<std::string, std::string> getSpecs() const override;
};

class PickableResource : public MapObject {
 private:
  ResourceType resource_type_;
  int amount_;

 public:
  PickableResource(FieldCoords origin, Game* parent, ResourceType resource_type,
                   int amount);
  std::optional<bool> objectAction() override;
  std::map<std::string, std::string> getSpecs() const override;
};

class ResourceGenerator : public MapObject {
 private:
  ResourceType resource_type_;
  int weekly_income_;
  int owner_id_;

 public:
  ResourceGenerator(FieldCoords origin, Game* parent,
                    ResourceType resource_type, int weekly_income);
  std::optional<bool> objectAction() override;
  int getOwnerId() { return owner_id_; }
  std::map<std::string, std::string> getSpecs() const override;
};

class City : public MapObject {
 private:
  Faction type_;
  int owner_id_;

 public:
  City(FieldCoords origin, Game* parent, Faction type, int owner_id = -1);
  std::optional<bool> objectAction() override { return true; }
  std::map<std::string, std::string> getSpecs() const override;
};
#endif