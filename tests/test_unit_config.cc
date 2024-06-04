/**
 * @file test_unit_config.cc
 * @author Wojciech Przybylski
 * @brief Unit tests for UnitConfig class
 * @copyright Copyright (c) 2024
 */

/**
 * THIS TEST IS RELIANT ON CONTENTS OF FILE test_unit_config.json
 * CHANGES TO THAT FILE WILL NEED TO BE REFLECTED IN THE TEST
 */

#include <UnitConfig.h>
#include <game_logic_utils.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <UnitBlock.hpp>
#include <fstream>
#include <optional>
#include <string>

class UnitConfigTest : public testing::Test {
 protected:
  UnitConfigTest() {
    std::string project_path = getProjectPath();
    config_path = project_path + "/tests/test_unit_config.json";

    std::ifstream ifs;
    ifs.open(config_path);
    nlohmann::json json = nlohmann::json::parse(ifs);
    ifs.close();
    unit_config.parseFromJson(json);
  }
  UnitConfig unit_config;
  std::string config_path;
};

TEST_F(UnitConfigTest, file_parse_no_exception) {
  UnitConfig unit_config_parse_file;
  std::ifstream ifs;
  ifs.open(config_path);
  EXPECT_NO_THROW(unit_config_parse_file.parseFromJson(ifs));
  ifs.close();
}

TEST_F(UnitConfigTest, types_present_in_config) {
  auto castle_1 = unit_config.getAttributes(UnitOrigin::CASTLE, 1);
  auto castle_2 = unit_config.getAttributes(UnitOrigin::CASTLE, 2);
  auto inferno_1 = unit_config.getAttributes(UnitOrigin::INFERNO, 1);
  auto inferno_2 = unit_config.getAttributes(UnitOrigin::INFERNO, 2);

  EXPECT_TRUE(castle_1.has_value());
  EXPECT_TRUE(castle_2.has_value());
  EXPECT_TRUE(inferno_1.has_value());
  EXPECT_TRUE(inferno_2.has_value());
}

TEST_F(UnitConfigTest, types_missing_from_config) {
  auto result = unit_config.getAttributes(UnitOrigin::FORTRESS, 1);

  EXPECT_FALSE(result.has_value());
}

TEST_F(UnitConfigTest, levels_missing_from_config) {
  auto castle_3 = unit_config.getAttributes(UnitOrigin::CASTLE, 3);
  auto inferno_3 = unit_config.getAttributes(UnitOrigin::INFERNO, 3);

  EXPECT_FALSE(castle_3.has_value());
  EXPECT_FALSE(inferno_3.has_value());
}

TEST_F(UnitConfigTest, correct_data_read) {
  using testing::no_adl::FieldsAre;

  auto castle_1 = unit_config.getAttributes(UnitOrigin::CASTLE, 1).value();
  auto castle_2 = unit_config.getAttributes(UnitOrigin::CASTLE, 2).value();
  auto inferno_1 = unit_config.getAttributes(UnitOrigin::INFERNO, 1).value();
  auto inferno_2 = unit_config.getAttributes(UnitOrigin::INFERNO, 2).value();

  EXPECT_THAT(castle_1, FieldsAre("Lackey", 1, 2, 3, 4, 5, 6, true, false));
  EXPECT_THAT(castle_2, FieldsAre("Knight", 7, 8, 9, 10, 11, 12, true, true));
  EXPECT_THAT(inferno_1, FieldsAre("Imp", 6, 5, 4, 3, 2, 1, false, false));
  EXPECT_THAT(inferno_2, FieldsAre("Literally The Devil", 12, 11, 10, 9, 8, 7,
                                   false, true));
}