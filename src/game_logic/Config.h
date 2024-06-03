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

#include <filesystem>
#include <json.hpp>
#include <string>

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
  nlohmann::json metadata_;
  const static std::map<Difficulty, Inventory> starting_inventories_;
  const static std::map<Difficulty, Incomes> starting_incomes_;
  const static std::map<ResourceType, std::string> resource_type_translator_;
  const static std::map<Faction, std::string> faction_type_translator_;
  const static std::map<GeologicalStructureType, std::string>
      geological_structure_translator_;

 public:
  Config(Config& other) = delete;
  void operator=(const Config& other) = delete;
  ~Config();
  static Config* getInstance();
  void loadData(std::string metadata_path = "./ObjectsMetadata.json");
  const nlohmann::json getMetadata() const { return metadata_; };
  const static std::map<Faction, std::string> kDefaultHeroNames;
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
};
#endif