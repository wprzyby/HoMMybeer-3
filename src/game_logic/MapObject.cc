/**
 * @file MapObject.cc
 * @author Piotr Kluba
 * @brief Contains the funcionality asociated with all objects on the map
 * @copyright Copyright (c) 2024
 *
 */
#include <MapObject.h>
#include <common.hpp>

#include <json.hpp>
#include <string>

using namespace std;
using json = nlohmann::json;

int MapObjectBase::current_id_ = 0;

const vector<FieldCoords> MapObjectBase::occupiedFields() const {
  vector<FieldCoords> retv = {};
  for (FieldCoords coord : space_taken_) {
    retv.push_back(origin_ + coord);
  }
  return retv;
}

GeologicalObject::GeologicalObject(FieldCoords origin,
                                   GeologicalStructureType struct_type,
                                   int variant)
    : MapObjectBase(origin, {}), struct_type_(struct_type), variant_(variant) {
  json metadata = Config::getInstance()->getMetadata();
  metadata = metadata["geological"];
  metadata = metadata[Config::getInstance()->enumToStringTranslate(struct_type)]
                     ["spaces_occupied"][to_string(variant_)];
  for (auto coord : metadata) {
    space_taken_.push_back(FieldCoords{coord[0], coord[1]});
  }

  // TODO: nicer way to convert enum
}

PickableResource::PickableResource(FieldCoords origin,
                                   ResourceType resource_type, int amount)
    : MapObjectBase(origin, {}),
      amount_(amount),
      resource_type_(resource_type) {
  space_taken_.push_back(FieldCoords{0, 1});
}

ResourceGenerator::ResourceGenerator(FieldCoords origin,
                                     ResourceType resource_type,
                                     int weekly_income)
    : MapObjectBase(origin, {}),
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

City::City(FieldCoords origin, Faction type, int owner_id)
    : MapObjectBase(origin, {}), owner_id_(owner_id), type_(type) {
  json metadata = Config::getInstance()->getMetadata();
  metadata = metadata["cities"]["Cities"]["spaces_occupied"]
                     [Config::getInstance()->enumToStringTranslate(type)];
  for (auto coord : metadata) {
    space_taken_.push_back(FieldCoords{coord[0], coord[1]});
  }
}