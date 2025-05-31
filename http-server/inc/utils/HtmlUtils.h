#include <filesystem>
#include <unordered_map>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

namespace Html
{
  std::string render(std::filesystem::path html_template, std::unordered_map<std::string, std::string> context);
}
