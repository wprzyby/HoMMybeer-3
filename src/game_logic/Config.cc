/**
 * @file Config.cc
 * @author Piotr Kluba
 * @brief Singleton used for configuration and metadata storage
 * @version 0.1
 * @date 2024-05-26
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <Config.h>

#include <ctime>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

#include "UnitBlockGenerator.hpp"

using namespace std;
using json = nlohmann::json;

Config* Config::config_ = nullptr;
const std::map<Faction, std::string> Config::kDefaultHeroNames = {
    {Faction::CASTLE, "Andrew the Mediocre"},
    {Faction::FORTRESS, "Peter the Fearful"},
    {Faction::INFERNO, "Stefan the Devil"},
};

const std::map<Difficulty, Inventory> Config::starting_inventories_ = {
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

const std::map<Difficulty, Incomes> Config::starting_incomes_ = {
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

const std::map<Faction, UnitOrigin> Config::faction_to_unit_origin_ = {
    {Faction::CASTLE, UnitOrigin::CASTLE},
    {Faction::FORTRESS, UnitOrigin::FORTRESS},
    {Faction::INFERNO, UnitOrigin::INFERNO}};

// TODO: edit the default values (propably with tests)

const std::map<ResourceType, std::string> Config::resource_type_translator_ = {
    {ResourceType::GOLD, "gold"},      {ResourceType::SULFUR, "sulfur"},
    {ResourceType::GEMS, "gems"},      {ResourceType::ORE, "ore"},
    {ResourceType::WOOD, "wood"},      {ResourceType::CRYSTAL, "crystal"},
    {ResourceType::MERCURY, "mercury"}};

const std::map<GeologicalStructureType, std::string>
    Config::geological_structure_translator_ = {
        {GeologicalStructureType::MOUNTAIN, "Mountain"},
        {GeologicalStructureType::TREE, "Tree"},
        {GeologicalStructureType::BUSH, "Bush"},
        {GeologicalStructureType::LAKE, "Lake"}};

const std::map<Faction, std::string> Config::faction_type_translator_ = {
    {Faction::CASTLE, "castle"},
    {Faction::INFERNO, "inferno"},
    {Faction::FORTRESS, "fortress"},
};

const std::map<TerrainType, std::string> Config::terrain_type_translator_ = {
    {TerrainType::DIRT, "dirt"},
    {TerrainType::GRASS, "grass"},
    {TerrainType::SNOW, "snow"},
    {TerrainType::STONE, "stone"},
};

Config::Config() {
  time_t t = time(0);
  cout << "Config created at: " << asctime(localtime(&t));
  objects_metadata_ = nullptr;
}

Config::~Config() {
  if (config_ != nullptr) {
    delete config_;
  }
}

Config* Config::getInstance() {
  if (config_ == nullptr) {
    config_ = new Config();
  }
  return config_;
}

void Config::loadObjectsData(string objects_metadata_path) {
  ifstream ifs;
  ifs.open(objects_metadata_path);
  objects_metadata_ = json::parse(ifs);
  ifs.close();
}

void Config::loadBattleData(string battle_metadata_path) {
  ifstream ifs;
  ifs.open(battle_metadata_path);
  battle_metadata_ = json::parse(ifs);
  ifs.close();
}

void Config::loadTownData(string metadata_path) {
  ifstream ifs;
  ifs.open(metadata_path);
  town_metadata_ = json::parse(ifs);
  ifs.close();
}
void Config::loadUnitConfig(std::string unit_config_path) {
  ifstream ifs;
  ifs.open(unit_config_path);
  unit_config_.parseFromJson(ifs);
  ifs.close();
}

std::vector<UnitBlock> Config::getStartingUnits(Faction faction) {
  UnitBlockGenerator generator(unit_config_);
  std::vector<UnitBlock> units;
  for (auto i : std::views::iota(0U, STARTING_UNITS_MAX_LEVEL)) {
    auto unit = generator.getUnitBlock(faction_to_unit_origin_.at(faction),
                                       i + 1, STARTING_UNIT_COUNTS[i]);
    if (not unit.has_value()) {
      throw std::runtime_error(
          "Starting unit config invalid - failed to generate units");
    }
    units.push_back(unit.value());
  }
  return units;
}
