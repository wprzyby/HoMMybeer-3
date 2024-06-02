#include <common.h>

std::string getProjectPath(std::string project_name) {
  std::filesystem::path cwd = std::filesystem::current_path();
  int name_length = static_cast<int>(project_name.length());
  size_t pos = cwd.string().find(project_name);
  return cwd.string().substr(0, pos + name_length);
}