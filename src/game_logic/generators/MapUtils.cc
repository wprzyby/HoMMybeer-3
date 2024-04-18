#include <MapUtils.h>

#include <algorithm>
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