#include <Jam/ConfigManager.hpp>
#include <fstream>
#include <cassert>
#include <string>
#include <sstream>

namespace jam
{
  ConfigManager::ConfigManager()
  {
  std::ifstream file("assets/config.txt");
  assert(file.good());

  std::string line;
  while (std::getline(file, line))
  {
    std::istringstream is_line(line);
    std::string key;
    if (std::getline(is_line, key, '=') && key[0] != '#')
    {
    std::string value;
    if (std::getline(is_line, value))
      m_config[key] = value;
    }
  }
  }

  std::string ConfigManager::string(const std::string& key) const
  {
  auto itr = m_config.find(key);
  if (itr != m_config.end())
    return itr->second;

  return "0";
  }

  int ConfigManager::integer(const std::string& key) const
  {
  return std::atoi(string(key).c_str());
  }

  float ConfigManager::float_(const std::string& key) const
  {
  return static_cast<float>(std::atof(string(key).c_str()));
  }

  bool ConfigManager::boolean(const std::string& key) const
  {
  return string(key) == "true";
  }
}
