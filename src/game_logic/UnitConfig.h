/**
 * @file UnitConfig.h
 * @author Wojciech Przybylski
 * @brief Class for storing information about how to create units
 * @copyright Copyright (c) 2024
 */
#ifndef SRC_GAME_LOGIC_UNIT_CONFIG_H
#define SRC_GAME_LOGIC_UNIT_CONFIG_H

#include <json.hpp>
#include <map>
#include <string>
#include <string_view>

#include "UnitBlock.hpp"

struct UnitAttributes {
  std::string name;
  unsigned int hitpoints_per_unit;
  unsigned int speed;
  unsigned int min_damage;
  unsigned int max_damage;
  unsigned int attack;
  unsigned int defense;
  bool is_ranged;
  bool immune_to_retaliation;
};

class UnitConfig {
  constexpr static std::array<std::pair<std::string_view, UnitOrigin>, 4U>
      UNIT_ORIGIN_STR_AND_ENUM{{{"CASTLE", UnitOrigin::CASTLE},
                                {"FORTRESS", UnitOrigin::FORTRESS},
                                {"INFERNO", UnitOrigin::INFERNO},
                                {"NEUTRAL", UnitOrigin::NEUTRAL}}};

 public:
  UnitConfig() = default;
  void parseFromJson(nlohmann::json& json_object);
  void parseFromJson(auto& json_parsable_stream) {
    nlohmann::json json = nlohmann::json::parse(json_parsable_stream);
    this->parseFromJson(json);
  }
  std::optional<UnitAttributes> getAttributes(UnitOrigin type,
                                              unsigned int level);

 private:
  std::map<UnitOrigin, std::map<unsigned int, UnitAttributes>> config_;
};

#endif  // SRC_GAME_LOGIC_UNIT_CONFIG_H