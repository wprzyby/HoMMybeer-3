/**
 * @file Config.h
 * @author Piotr Kluba
 * @brief Singleton used for configuration and metadata storage
 * @version 0.1
 * @date 2024-05-26
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef SRC_GAME_LOGIC_CONFIG_H_
#define SRC_GAME_LOGIC_CONFIG_H_

#include <json.hpp>
#include <string>

#include "UnitBlock.hpp"
#include "UnitConfig.h"

enum class GeologicalStructureType { MOUNTAIN, LAKE, BUSH, TREE };

enum class Faction { CASTLE, INFERNO, FORTRESS };

enum class ResourceType { GOLD = 0, SULFUR, GEMS, ORE, WOOD, CRYSTAL, MERCURY };

enum class TerrainType : int { SNOW = 0, GRASS, STONE, DIRT };

enum class Difficulty { EASY, NORMAL, HARD };

using Inventory = std::map<ResourceType, int>;
using Incomes = std::map<ResourceType, int>;

class Config {
 private:
  Config();
  static Config* config_;
  nlohmann::json town_metadata_;
  nlohmann::json objects_metadata_;
  nlohmann::json battle_metadata_;
  UnitConfig unit_config_;
  const static std::map<Difficulty, Inventory> starting_inventories_;
  const static std::map<Difficulty, Incomes> starting_incomes_;
  const static std::map<ResourceType, std::string> resource_type_translator_;
  const static std::map<Faction, std::string> faction_type_translator_;
  const static std::map<GeologicalStructureType, std::string>
      geological_structure_translator_;
  const static std::map<TerrainType, std::string> terrain_type_translator_;
  const static std::map<Faction, UnitOrigin> faction_to_unit_origin_;
  constexpr static unsigned int STARTING_UNITS_MAX_LEVEL = 3U;
  constexpr static std::array<unsigned int, STARTING_UNITS_MAX_LEVEL>
      STARTING_UNIT_COUNTS = {15, 10, 5};

 public:
  Config(Config &other) = delete;
  void operator=(const Config &other) = delete;
  ~Config();
  static Config* getInstance();
  void loadTownData(std::string metadata_path = "./TownsMetadata.json");
  const nlohmann::json getTownMetadata() const { return town_metadata_; };
  void loadObjectsData(
      std::string objects_metadata_path = "./ObjectsMetadata.json");
  void loadBattleData(
      std::string objects_metadata_path = "./assets/BattlegroundMetadata.json");
  void loadUnitConfig(std::string unit_config_path);
  const nlohmann::json getObjectsMetadata() const { return objects_metadata_; };
  const nlohmann::json getBattleMetadata() const { return battle_metadata_; };
  UnitConfig getUnitConfig() { return unit_config_; };
  const static std::map<Faction, std::string> kDefaultHeroNames;
  std::vector<UnitBlock> getStartingUnits(Faction faction);
  static Inventory getStartingInventory(Difficulty difficulty) {
    return starting_inventories_.at(difficulty);
  }
  static Incomes getStartingIncomes(Difficulty difficulty) {
    return starting_incomes_.at(difficulty);
  }
  static std::string enumToStringTranslate(ResourceType enum_to_translate) {
    return resource_type_translator_.at(enum_to_translate);
  }
  static std::string enumToStringTranslate(
      GeologicalStructureType enum_to_translate) {
    return geological_structure_translator_.at(enum_to_translate);
  }
  static std::string enumToStringTranslate(Faction enum_to_translate) {
    return faction_type_translator_.at(enum_to_translate);
  }
  std::string enumToStringTranslate(TerrainType enum_to_translate) {
    return terrain_type_translator_.at(enum_to_translate);
  }
};
#endif