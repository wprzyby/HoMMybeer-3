/**
 * @file MapObject.cc
 * @author Piotr Kluba
 * @brief Contains the functionality associated with all objects on the map
 * @copyright Copyright (c) 2024
 *
 */
#include <MapObject.h>
#include <Session.h>

#include <iostream>
#include <json.hpp>
#include <string>

using namespace std;
using json = nlohmann::json;

int MapObject::current_id_ = 0;

const vector<FieldCoords> MapObject::occupiedFields() const {
  vector<FieldCoords> retv = {};
  for (FieldCoords coord : space_taken_) {
    retv.push_back(origin_ + coord);
  }
  return retv;
}

GeologicalObject::GeologicalObject(FieldCoords origin, Game* parent,
                                   GeologicalStructureType struct_type,
                                   int variant)
    : MapObject(origin, parent, {}),
      struct_type_(struct_type),
      variant_(variant) {
  json metadata = Config::getInstance()->getMetadata();
  metadata = metadata["geological"];
  metadata = metadata[Config::getInstance()->enumToStringTranslate(struct_type)]
                     ["spaces_occupied"][to_string(variant_)];
  for (auto coord : metadata) {
    space_taken_.push_back(FieldCoords{coord[0], coord[1]});
  }

  // TODO: nicer way to convert enum
}

PickableResource::PickableResource(FieldCoords origin, Game* parent,
                                   ResourceType resource_type, int amount)
    : MapObject(origin, parent, {}),
      amount_(amount),
      resource_type_(resource_type) {
  space_taken_.push_back(FieldCoords{0, 1});
}

std::optional<bool> PickableResource::objectAction() {
  bool success =
      parent_->getCurrentPlayer()->updateResourceQuantity(
          resource_type_, amount_);
  return parent_->deleteMapObject(id_) && success;
}

ResourceGenerator::ResourceGenerator(FieldCoords origin, Game* parent,
                                     ResourceType resource_type,
                                     int weekly_income)
    : MapObject(origin, parent, {}),
      weekly_income_(weekly_income),
      owner_id_(-1),
      resource_type_(resource_type) {
  json metadata = Config::getInstance()->getMetadata();
  metadata =
      metadata["resources"]["Mines"]["spaces_occupied"]
              [Config::getInstance()->enumToStringTranslate(resource_type)];
  for (auto coord : metadata) {
    space_taken_.push_back(FieldCoords{coord[0], coord[1]});
  }
}

std::optional<bool> ResourceGenerator::objectAction() {
  if (parent_->getCurrentPlayer()->changeIncome(
          resource_type_, weekly_income_)) {
    owner_id_ = parent_->getCurrPlayerId();
    return true;
  }
  return false;
}

City::City(FieldCoords origin, Game* parent, Faction type, int owner_id)
    : MapObject(origin, parent, {}), owner_id_(owner_id), type_(type) {
  json metadata = Config::getInstance()->getMetadata();
  metadata = metadata["cities"]["Cities"]["spaces_occupied"]
                     [Config::getInstance()->enumToStringTranslate(type)];
  for (auto coord : metadata) {
    space_taken_.push_back(FieldCoords{coord[0], coord[1]});
  }
}