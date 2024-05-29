#include "UnitConfig.h"

void from_json(  // NOLINT(readability-identifier-naming) - naming required by
                 // interaction with external lib (nlohmann-json)
    const nlohmann::json& json, UnitAttributes& attributes) {
  json.at("name").get_to(attributes.name);
  json.at("hitpoints_per_unit").get_to(attributes.hitpoints_per_unit);
  json.at("speed").get_to(attributes.speed);
  json.at("min_damage").get_to(attributes.min_damage);
  json.at("max_damage").get_to(attributes.max_damage);
  json.at("attack").get_to(attributes.attack);
  json.at("defense").get_to(attributes.defense);
  json.at("is_ranged").get_to(attributes.is_ranged);
  json.at("immune_to_retaliation").get_to(attributes.immune_to_retaliation);
}

void UnitConfig::parseFromJson(nlohmann::json& json) {
  for (const auto& [unit_type_str, unit_type_config] : json.items()) {
    auto does_typename_match = [&](auto string_and_type) {
      return string_and_type.first == unit_type_str;
    };
    const auto* find_result =
        std::find_if(std::begin(UNIT_ORIGIN_STR_AND_ENUM),
                     std::end(UNIT_ORIGIN_STR_AND_ENUM), does_typename_match);
    if (find_result == std::end(UNIT_ORIGIN_STR_AND_ENUM)) {
      continue;
    }
    UnitOrigin unit_type = find_result->second;
    for (const auto& [lvl_specifier_str, unit_attributes_json] :
         unit_type_config.items()) {
      unsigned int level = std::stoi(lvl_specifier_str);
      auto attributes = unit_attributes_json.template get<UnitAttributes>();
      config_[unit_type][level] = attributes;
    }
  }
}

std::optional<UnitAttributes> UnitConfig::getAttributes(UnitOrigin type,
                                                        unsigned int level) {
  if (config_.find(type) == config_.end()) {
    return {};
  }
  if (config_.at(type).find(level) == config_.at(type).end()) {
    return {};
  }
  return config_.at(type).at(level);
}