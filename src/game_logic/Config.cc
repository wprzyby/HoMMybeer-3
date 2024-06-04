/**
 * @file Config.cc
 * @author Piotr Kluba, Wojciech Przybylski
 * @brief Singleton used for configuration and metadata storage
 * @copyright Copyright (c) 2024
 */

#include <Config.h>

#include <ctime>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

#include "UnitBlockGenerator.hpp"

using json = nlohmann::json;

Config* Config::config_ = nullptr;
const std::map<Faction, std::string> Config::DEFAULT_HERO_NAMES = {
    {Faction::CASTLE, "Andrew the Mediocre"},
    {Faction::FORTRESS, "Peter the Fearful"},
    {Faction::INFERNO, "Stefan the Devil"},
};

const std::map<Difficulty, Inventory> Config::STARTING_INVENTORIES = {
    {Difficulty::EASY,
     {{ResourceType::GOLD, 20000},
      {ResourceType::SULFUR, 5},
      {ResourceType::GEMS, 5},
      {ResourceType::ORE, 10},
      {ResourceType::WOOD, 10},
      {ResourceType::CRYSTAL, 5},
      {ResourceType::MERCURY, 5}}},
    {Difficulty::NORMAL,
     {{ResourceType::GOLD, 2000},
      {ResourceType::SULFUR, 5},
      {ResourceType::GEMS, 5},
      {ResourceType::ORE, 10},
      {ResourceType::WOOD, 10},
      {ResourceType::CRYSTAL, 5},
      {ResourceType::MERCURY, 5}}},
    {Difficulty::HARD,
     {{ResourceType::GOLD, 2000},
      {ResourceType::SULFUR, 5},
      {ResourceType::GEMS, 5},
      {ResourceType::ORE, 10},
      {ResourceType::WOOD, 10},
      {ResourceType::CRYSTAL, 5},
      {ResourceType::MERCURY, 5}}}};

const std::map<Difficulty, Incomes> Config::STARTING_INCOMES = {
    {Difficulty::EASY,
     {{ResourceType::GOLD, 1500},
      {ResourceType::SULFUR, 1},
      {ResourceType::GEMS, 1},
      {ResourceType::ORE, 1},
      {ResourceType::WOOD, 1},
      {ResourceType::CRYSTAL, 1},
      {ResourceType::MERCURY, 1}}},
    {Difficulty::NORMAL,
     {{ResourceType::GOLD, 1500},
      {ResourceType::SULFUR, 1},
      {ResourceType::GEMS, 1},
      {ResourceType::ORE, 1},
      {ResourceType::WOOD, 1},
      {ResourceType::CRYSTAL, 1},
      {ResourceType::MERCURY, 1}}},
    {Difficulty::HARD,
     {{ResourceType::GOLD, 1500},
      {ResourceType::SULFUR, 1},
      {ResourceType::GEMS, 1},
      {ResourceType::ORE, 1},
      {ResourceType::WOOD, 1},
      {ResourceType::CRYSTAL, 1},
      {ResourceType::MERCURY, 1}}}};

const std::map<Faction, UnitOrigin> Config::FACTION_TO_UNIT_ORIGIN = {
    {Faction::CASTLE, UnitOrigin::CASTLE},
    {Faction::FORTRESS, UnitOrigin::FORTRESS},
    {Faction::INFERNO, UnitOrigin::INFERNO}};

const std::map<ResourceType, std::string> Config::RESOURCE_TYPE_TRANSLATIONS = {
    {ResourceType::GOLD, "gold"},      {ResourceType::SULFUR, "sulfur"},
    {ResourceType::GEMS, "gems"},      {ResourceType::ORE, "ore"},
    {ResourceType::WOOD, "wood"},      {ResourceType::CRYSTAL, "crystal"},
    {ResourceType::MERCURY, "mercury"}};

const std::map<GeologicalStructureType, std::string>
    Config::GEOLOGICAL_STRUCTURE_TRANSLATIONS = {
        {GeologicalStructureType::MOUNTAIN, "Mountain"},
        {GeologicalStructureType::TREE, "Tree"},
        {GeologicalStructureType::BUSH, "Bush"},
        {GeologicalStructureType::LAKE, "Lake"}};

const std::map<Faction, std::string> Config::FACTION_TYPE_TRANSLATIONS = {
    {Faction::CASTLE, "castle"},
    {Faction::INFERNO, "inferno"},
    {Faction::FORTRESS, "fortress"},
};

const std::map<TerrainType, std::string> Config::TERRAIN_TYPE_TRANSLATIONS = {
    {TerrainType::DIRT, "dirt"},
    {TerrainType::GRASS, "grass"},
    {TerrainType::SNOW, "snow"},
    {TerrainType::STONE, "stone"},
};

Config::Config() {
  time_t creation_time = time(nullptr);
  std::cout << "Config created at: " << asctime(localtime(&creation_time));
  objects_metadata_ = nullptr;
}

Config::~Config() { delete config_; }

Config* Config::getInstance() {
  if (config_ == nullptr) {
    config_ = new Config();
  }
  return config_;
}

void Config::loadObjectsData(const std::string& objects_metadata_path) {
  std::ifstream ifs;
  ifs.open(objects_metadata_path);
  objects_metadata_ = json::parse(ifs);
  ifs.close();
}

void Config::loadBattleData(const std::string& battle_metadata_path) {
  std::ifstream ifs;
  ifs.open(battle_metadata_path);
  battle_metadata_ = json::parse(ifs);
  ifs.close();
}

void Config::loadTownData(const std::string& metadata_path) {
  std::ifstream ifs;
  ifs.open(metadata_path);
  town_metadata_ = json::parse(ifs);
  ifs.close();
}
void Config::loadUnitConfig(const std::string& unit_config_path) {
  std::ifstream ifs;
  ifs.open(unit_config_path);
  unit_config_.parseFromJson(ifs);
  ifs.close();
}

std::vector<UnitBlock> Config::getStartingUnits(Faction faction) {
  UnitBlockGenerator generator(unit_config_);
  std::vector<UnitBlock> units;
  for (auto level_idx : std::views::iota(0U, STARTING_UNITS_MAX_LEVEL)) {
    auto unit = generator.getUnitBlock(FACTION_TO_UNIT_ORIGIN.at(faction),
                                       level_idx + 1,
                                       STARTING_UNIT_COUNTS.at(level_idx));
    if (not unit.has_value()) {
      throw std::runtime_error(
          "Starting unit config invalid - failed to generate units");
    }
    units.push_back(unit.value());
  }
  return units;
}
