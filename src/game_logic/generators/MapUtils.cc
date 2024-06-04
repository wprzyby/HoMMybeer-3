/**
 * @file MapUtils.cc
 * @author Wojciech Przybylski
 * @brief Method definitions for map utils
 * @copyright Copyright (c) 2024
 */

#include <MapUtils.h>

#include <algorithm>
#include <memory>
#include <random>

using FieldArray_t = std::vector<std::vector<Field>>;

MapInfo generateGrassMap(MapSize map_size, int num_of_players) {
  int width = static_cast<int>(map_size);

  FieldArray_t field_array(width, std::vector<Field>(width));
  for (int r = 0; r < width; r++) {
    for (int c = 0; c < width; c++) {
      field_array[r][c] = Field(c, r, TerrainType::GRASS, false, true);
    }
  }

  // get random shuffling of numbers from range [0, width) to use first
  // num_of_players*2 values to generate starting locations
  std::random_device rd;
  std::mt19937 gen(rd());
  std::vector<int> numbers(width);
  std::iota(std::begin(numbers), std::end(numbers), 0);
  std::shuffle(numbers.begin(), numbers.end(), gen);

  std::vector<FieldCoords> starting_locations;
  for (int i = 0; i < num_of_players; ++i) {
    starting_locations.push_back({numbers[2 * i], numbers[2 * i + 1]});
  }

  return MapInfo{Map(field_array), num_of_players, starting_locations};
}

MapInfo generateLargeExampleMap() {
  int width = static_cast<int>(MapSize::L);

  std::vector<FieldCoords> starting_locations = {FieldCoords{7, 3}};
  // defining objects

  // defining field types
  FieldArray_t field_array(width, std::vector<Field>(width));
  for (int r = 0; r < width; r++) {
    for (int c = 0; c < width; c++) {
      field_array[r][c] = Field(c, r, TerrainType::GRASS, false, true);
    }
  }

  return MapInfo{Map(field_array), 1, starting_locations};
}

std::vector<std::shared_ptr<MapObject>> generateExampleStaticObjects() {
  std::vector<std::shared_ptr<MapObject>> starting_objects = {
      std::make_shared<City>(FieldCoords{0, 0}, nullptr, Faction::CASTLE, -1),
      std::make_shared<GeologicalObject>(FieldCoords{-1, 3}, nullptr,
                                         GeologicalStructureType::TREE, 7),
      std::make_shared<GeologicalObject>(FieldCoords{10, 7}, nullptr,
                                         GeologicalStructureType::MOUNTAIN, 1),
      std::make_shared<GeologicalObject>(FieldCoords{7, 10}, nullptr,
                                         GeologicalStructureType::MOUNTAIN, 0),
      std::make_shared<GeologicalObject>(FieldCoords{14, 6}, nullptr,
                                         GeologicalStructureType::MOUNTAIN, 5),
      std::make_shared<GeologicalObject>(FieldCoords{13, 7}, nullptr,
                                         GeologicalStructureType::TREE, 3),
      std::make_shared<ResourceGenerator>(FieldCoords{-1, 10}, nullptr,
                                          ResourceType::WOOD, 3),
      std::make_shared<GeologicalObject>(FieldCoords{3, 15}, nullptr,
                                         GeologicalStructureType::TREE, 7),
      std::make_shared<GeologicalObject>(FieldCoords{5, 17}, nullptr,
                                         GeologicalStructureType::TREE, 8),
      std::make_shared<GeologicalObject>(FieldCoords{3, 17}, nullptr,
                                         GeologicalStructureType::TREE, 5),
      std::make_shared<ResourceGenerator>(FieldCoords{13, 16}, nullptr,
                                          ResourceType::MERCURY, 3),
      std::make_shared<GeologicalObject>(FieldCoords{9, 15}, nullptr,
                                         GeologicalStructureType::MOUNTAIN, 0),
      std::make_shared<GeologicalObject>(FieldCoords{11, 12}, nullptr,
                                         GeologicalStructureType::TREE, 0),
      std::make_shared<GeologicalObject>(FieldCoords{13, 18}, nullptr,
                                         GeologicalStructureType::MOUNTAIN, 2),
      std::make_shared<GeologicalObject>(FieldCoords{1, 19}, nullptr,
                                         GeologicalStructureType::MOUNTAIN, 0),
      std::make_shared<GeologicalObject>(FieldCoords{5, 22}, nullptr,
                                         GeologicalStructureType::MOUNTAIN, 3),
      std::make_shared<GeologicalObject>(FieldCoords{4, 25}, nullptr,
                                         GeologicalStructureType::MOUNTAIN, 4),
      std::make_shared<ResourceGenerator>(FieldCoords{3, 27}, nullptr,
                                          ResourceType::SULFUR, 3),
      std::make_shared<City>(FieldCoords{31, 10}, nullptr, Faction::FORTRESS,
                             -1),
      std::make_shared<GeologicalObject>(FieldCoords{29, 4}, nullptr,
                                         GeologicalStructureType::MOUNTAIN, 1),
      std::make_shared<GeologicalObject>(FieldCoords{32, 8}, nullptr,
                                         GeologicalStructureType::MOUNTAIN, 5),
      std::make_shared<GeologicalObject>(FieldCoords{31, 7}, nullptr,
                                         GeologicalStructureType::TREE, 2),
      std::make_shared<GeologicalObject>(FieldCoords{32, 8}, nullptr,
                                         GeologicalStructureType::TREE, 8),
      std::make_shared<GeologicalObject>(FieldCoords{30, 9}, nullptr,
                                         GeologicalStructureType::TREE, 5),
      std::make_shared<GeologicalObject>(FieldCoords{30, 11}, nullptr,
                                         GeologicalStructureType::TREE, 6),
      std::make_shared<GeologicalObject>(FieldCoords{30, 13}, nullptr,
                                         GeologicalStructureType::TREE, 5),
      std::make_shared<GeologicalObject>(FieldCoords{32, 14}, nullptr,
                                         GeologicalStructureType::TREE, 7),
      std::make_shared<City>(FieldCoords{37, 44}, nullptr, Faction::INFERNO,
                             -1),
      std::make_shared<GeologicalObject>(FieldCoords{35, 44}, nullptr,
                                         GeologicalStructureType::MOUNTAIN, 0),
      std::make_shared<GeologicalObject>(FieldCoords{38, 40}, nullptr,
                                         GeologicalStructureType::MOUNTAIN, 1),
      std::make_shared<GeologicalObject>(FieldCoords{38, 35}, nullptr,
                                         GeologicalStructureType::MOUNTAIN, 0),
      std::make_shared<GeologicalObject>(FieldCoords{37, 39}, nullptr,
                                         GeologicalStructureType::MOUNTAIN, 3),
      std::make_shared<GeologicalObject>(FieldCoords{42, 38}, nullptr,
                                         GeologicalStructureType::MOUNTAIN, 3),
      std::make_shared<GeologicalObject>(FieldCoords{41, 40}, nullptr,
                                         GeologicalStructureType::MOUNTAIN, 2),
      std::make_shared<GeologicalObject>(FieldCoords{36, 36}, nullptr,
                                         GeologicalStructureType::TREE, 7),
      std::make_shared<GeologicalObject>(FieldCoords{34, 38}, nullptr,
                                         GeologicalStructureType::TREE, 8),
      std::make_shared<GeologicalObject>(FieldCoords{34, 41}, nullptr,
                                         GeologicalStructureType::TREE, 3),
      std::make_shared<ResourceGenerator>(FieldCoords{29, 10}, nullptr,
                                          ResourceType::ORE, 3),
      std::make_shared<ResourceGenerator>(FieldCoords{35, 47}, nullptr,
                                          ResourceType::GEMS, 3),
      std::make_shared<ResourceGenerator>(FieldCoords{21, 27}, nullptr,
                                          ResourceType::GOLD, 1000),
      std::make_shared<GeologicalObject>(FieldCoords{19, 29}, nullptr,
                                         GeologicalStructureType::TREE, 7),
      std::make_shared<GeologicalObject>(FieldCoords{19, 26}, nullptr,
                                         GeologicalStructureType::TREE, 6),
      std::make_shared<GeologicalObject>(FieldCoords{20, 24}, nullptr,
                                         GeologicalStructureType::TREE, 3),
      std::make_shared<GeologicalObject>(FieldCoords{21, 30}, nullptr,
                                         GeologicalStructureType::MOUNTAIN, 0),
      std::make_shared<GeologicalObject>(FieldCoords{25, 32}, nullptr,
                                         GeologicalStructureType::MOUNTAIN, 1),
      std::make_shared<GeologicalObject>(FieldCoords{27, 35}, nullptr,
                                         GeologicalStructureType::MOUNTAIN, 2),
      std::make_shared<GeologicalObject>(FieldCoords{28, 33}, nullptr,
                                         GeologicalStructureType::MOUNTAIN, 3),
      std::make_shared<GeologicalObject>(FieldCoords{28, 35}, nullptr,
                                         GeologicalStructureType::MOUNTAIN, 0),
      std::make_shared<GeologicalObject>(FieldCoords{34, 37}, nullptr,
                                         GeologicalStructureType::MOUNTAIN, 4),
      std::make_shared<GeologicalObject>(FieldCoords{12, 1}, nullptr,
                                         GeologicalStructureType::TREE, 8),
      std::make_shared<GeologicalObject>(FieldCoords{12, 4}, nullptr,
                                         GeologicalStructureType::TREE, 4),
      std::make_shared<GeologicalObject>(FieldCoords{10, -1}, nullptr,
                                         GeologicalStructureType::TREE, 7),
      std::make_shared<GeologicalObject>(FieldCoords{15, 18}, nullptr,
                                         GeologicalStructureType::MOUNTAIN, 3),
      std::make_shared<GeologicalObject>(FieldCoords{22, 23}, nullptr,
                                         GeologicalStructureType::TREE, 8),
      std::make_shared<GeologicalObject>(FieldCoords{25, 20}, nullptr,
                                         GeologicalStructureType::MOUNTAIN, 1),
      std::make_shared<GeologicalObject>(FieldCoords{28, 22}, nullptr,
                                         GeologicalStructureType::MOUNTAIN, 3),
      std::make_shared<GeologicalObject>(FieldCoords{28, 19}, nullptr,
                                         GeologicalStructureType::TREE, 8),
      std::make_shared<GeologicalObject>(FieldCoords{29, 16}, nullptr,
                                         GeologicalStructureType::TREE, 8),
      std::make_shared<GeologicalObject>(FieldCoords{28, 18}, nullptr,
                                         GeologicalStructureType::MOUNTAIN, 4),
      std::make_shared<GeologicalObject>(FieldCoords{29, 16}, nullptr,
                                         GeologicalStructureType::MOUNTAIN, 5),
      std::make_shared<ResourceGenerator>(FieldCoords{11, 4}, nullptr,
                                          ResourceType::ORE, 3),
      std::make_shared<GeologicalObject>(FieldCoords{15, 20}, nullptr,
                                         GeologicalStructureType::MOUNTAIN, 0),
      std::make_shared<GeologicalObject>(FieldCoords{2, 4}, nullptr,
                                         GeologicalStructureType::TREE, 0),
  };

  return starting_objects;
}

std::vector<std::shared_ptr<MapObject>> generateExamplePickableObjects() {
  std::vector<std::shared_ptr<MapObject>> starting_objects = {
      std::make_shared<PickableResource>(FieldCoords{5, -1}, nullptr,
                                         ResourceType::CRYSTAL, 5),
      std::make_shared<PickableResource>(FieldCoords{33, 37}, nullptr,
                                         ResourceType::GOLD, 1000),
      std::make_shared<PickableResource>(FieldCoords{19, 25}, nullptr,
                                         ResourceType::WOOD, 5),
  };
  return starting_objects;
}