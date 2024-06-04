/**
 * @file test_unit_block_generator.cc
 * @author Wojciech Przybylski
 * @brief Tests for UnitBlockGenerator class
 * @copyright Copyright (c) 2024
 */
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <UnitBlock.hpp>
#include <UnitBlockGenerator.hpp>

#include "gmock/gmock.h"


struct Attributes {
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

class MockConfig {
 public:
  MockConfig() = default;
  MockConfig(MockConfig&) = default;
  std::optional<Attributes> getAttributes(UnitOrigin /*unused*/,
                                          unsigned int /*unused*/) {
    return Attributes{.name = "name",
                      .hitpoints_per_unit = 1,
                      .speed = 2,
                      .min_damage = 3,
                      .max_damage = 4,
                      .attack = 5,
                      .defense = 6,
                      .is_ranged = true,
                      .immune_to_retaliation = false};
  }
};

TEST(unit_block_generator_test, simple_generation) {
  using testing::no_adl::FieldsAre;
  MockConfig config;
  UnitBlockGenerator generator{config};

  auto first_unit = generator.getUnitBlock(UnitOrigin::CASTLE, 1, 5);
  auto second_unit = generator.getUnitBlock(UnitOrigin::CASTLE, 1, 5);

  EXPECT_THAT(first_unit.value(),
              FieldsAre(UnitOrigin::CASTLE, 1, "name", 5, 1, 5, 2, 3, 4, 5, 6,
                        true, true, false));
  EXPECT_THAT(second_unit.value(),
              FieldsAre(UnitOrigin::CASTLE, 1, "name", 5, 1, 5, 2, 3, 4, 5, 6,
                        true, true, false));
}