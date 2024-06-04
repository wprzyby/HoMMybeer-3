/**
 * @file test_unit_integration.cc
 * @author Wojciech Przybylski
 * @brief Tests to verify that it's possible to integrate UnitBlockGenerator
 * with the go-to UnitConfig class
 * @copyright Copyright (c) 2024
 */

/**
 * THIS TEST IS RELIANT ON CONTENTS OF FILE test_unit_config.json
 * CHANGES TO THAT FILE WILL NEED TO BE REFLECTED IN THE TEST
 */

#include <UnitConfig.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <UnitBlock.hpp>
#include <UnitBlockGenerator.hpp>
#include <fstream>

class UnitIntegrationTest : public testing::Test {
 protected:
  UnitIntegrationTest() {
    std::string project_name = "hommybeer-3";
    std::filesystem::path cwd = std::filesystem::current_path();
    size_t pos = cwd.string().find(project_name);
    std::string project_path =
        cwd.string().substr(0, pos + project_name.length());
    std::string config_path = project_path + "/tests/test_unit_config.json";
    std::ifstream ifs;
    ifs.open(config_path);
    nlohmann::json json = nlohmann::json::parse(ifs);
    ifs.close();
    unit_config.parseFromJson(json);
  }

  UnitConfig unit_config;
};

TEST_F(UnitIntegrationTest, unit_block_generator_construction) {
  EXPECT_NO_THROW(UnitBlockGenerator{unit_config});
}

TEST_F(UnitIntegrationTest, unit_block_generator_correct_generation) {
  using testing::no_adl::FieldsAre;
  UnitBlockGenerator unit_generator = UnitBlockGenerator{unit_config};
  auto castle_1_block = unit_generator.getUnitBlock(UnitOrigin::CASTLE, 1, 1);
  auto castle_2_block = unit_generator.getUnitBlock(UnitOrigin::CASTLE, 2, 2);
  auto inferno_1_block = unit_generator.getUnitBlock(UnitOrigin::INFERNO, 1, 3);
  auto inferno_2_block = unit_generator.getUnitBlock(UnitOrigin::INFERNO, 2, 4);

  EXPECT_TRUE(castle_1_block.has_value());
  EXPECT_TRUE(castle_2_block.has_value());
  EXPECT_TRUE(inferno_1_block.has_value());
  EXPECT_TRUE(inferno_2_block.has_value());
  EXPECT_THAT(castle_1_block.value(),
              FieldsAre(UnitOrigin::CASTLE, 1, "Lackey", 1, 1, 1, 2, 3, 4, 5, 6,
                        true, true, false));
  EXPECT_THAT(castle_2_block.value(),
              FieldsAre(UnitOrigin::CASTLE, 2, "Knight", 2, 7, 14, 8, 9, 10, 11,
                        12, true, true, true));
  EXPECT_THAT(inferno_1_block.value(),
              FieldsAre(UnitOrigin::INFERNO, 1, "Imp", 3, 6, 18, 5, 4, 3, 2, 1,
                        true, false, false));
  EXPECT_THAT(inferno_2_block.value(),
              FieldsAre(UnitOrigin::INFERNO, 2, "Literally The Devil", 4, 12,
                        48, 11, 10, 9, 8, 7, true, false, true));
}

TEST_F(UnitIntegrationTest, unit_block_generator_not_available_data) {
  UnitBlockGenerator unit_generator = UnitBlockGenerator{unit_config};
  auto result_1 = unit_generator.getUnitBlock(UnitOrigin::FORTRESS, 1, 1);
}
