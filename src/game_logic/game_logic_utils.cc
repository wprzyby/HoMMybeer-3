/**
 * @file game_logic_utils.cc
 * @author Piotr Kluba
 * @brief Functionality shared by different elements of the game logic module
 * @copyright Copyright (c) 2024
 */

#include <game_logic_utils.h>

#include <filesystem>

std::string getProjectPath(const std::string& project_name) {
  std::filesystem::path cwd = std::filesystem::current_path();
  int name_length = static_cast<int>(project_name.length());
  size_t pos = cwd.string().find(project_name);
  return cwd.string().substr(0, pos + name_length);
}