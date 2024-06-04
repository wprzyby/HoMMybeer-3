/**
 * @file Config.h
 * @author Piotr Kluba, Wojciech Przybylski
 * @brief Singleton used for configuration and metadata storage
 * @copyright Copyright (c) 2024
 */

#ifndef SRC_GAME_LOGIC_CONFIG_H
#define SRC_GAME_LOGIC_CONFIG_H

#include <json.hpp>
#include <string>

#include "UnitBlock.hpp"
#include "UnitConfig.h"

enum class GeologicalStructureType { MOUNTAIN, LAKE, BUSH, TREE };

enum class Faction { CASTLE = 0, INFERNO, FORTRESS };

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
  const static std::map<Difficulty, Inventory> STARTING_INVENTORIES;
  const static std::map<Difficulty, Incomes> STARTING_INCOMES;
  const static std::map<ResourceType, std::string> RESOURCE_TYPE_TRANSLATIONS;
  const static std::map<Faction, std::string> FACTION_TYPE_TRANSLATIONS;
  const static std::map<GeologicalStructureType, std::string>
      GEOLOGICAL_STRUCTURE_TRANSLATIONS;
  const static std::map<TerrainType, std::string> TERRAIN_TYPE_TRANSLATIONS;
  const static std::map<Faction, UnitOrigin> FACTION_TO_UNIT_ORIGIN;
  constexpr static unsigned int STARTING_UNITS_MAX_LEVEL = 3U;
  constexpr static std::array<unsigned int, STARTING_UNITS_MAX_LEVEL>
      STARTING_UNIT_COUNTS = {15, 10, 5};

 public:
  Config(Config&) = delete;
  Config(Config&&) = delete;
  void operator=(const Config& other) = delete;
  void operator=(Config&& other) = delete;
  ~Config();
  static Config* getInstance();
  void loadTownData(const std::string& metadata_path);
  nlohmann::json getTownMetadata() const { return town_metadata_; };
  void loadObjectsData(const std::string& objects_metadata_path);
  void loadBattleData(const std::string& battle_metadata_path);
  void loadUnitConfig(const std::string& unit_config_path);
  nlohmann::json getObjectsMetadata() const { return objects_metadata_; };
  nlohmann::json getBattleMetadata() const { return battle_metadata_; };
  UnitConfig getUnitConfig() { return unit_config_; };
  const static std::map<Faction, std::string> DEFAULT_HERO_NAMES;
  std::vector<UnitBlock> getStartingUnits(Faction faction);
  static Inventory getStartingInventory(Difficulty difficulty) {
    return STARTING_INVENTORIES.at(difficulty);
  }
  static Incomes getStartingIncomes(Difficulty difficulty) {
    return STARTING_INCOMES.at(difficulty);
  }
  static std::string enumToStringTranslate(ResourceType enum_to_translate) {
    return RESOURCE_TYPE_TRANSLATIONS.at(enum_to_translate);
  }
  static std::string enumToStringTranslate(
      GeologicalStructureType enum_to_translate) {
    return GEOLOGICAL_STRUCTURE_TRANSLATIONS.at(enum_to_translate);
  }
  static std::string enumToStringTranslate(Faction enum_to_translate) {
    return FACTION_TYPE_TRANSLATIONS.at(enum_to_translate);
  }
  static std::string enumToStringTranslate(TerrainType enum_to_translate) {
    return TERRAIN_TYPE_TRANSLATIONS.at(enum_to_translate);
  }

  static UnitOrigin factionToUnitOriginTranslate(Faction faction) {
    return FACTION_TO_UNIT_ORIGIN.at(faction);
  }
};
#endif // SRC_GAME_LOGIC_CONFIG_H