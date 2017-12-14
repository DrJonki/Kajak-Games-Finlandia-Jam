#pragma once

#include <unordered_map>

namespace jam
{
  class ConfigManager final
  {
  public:

  ConfigManager();

  std::string string(const std::string& key) const;

  int integer(const std::string& key) const;

  float float_(const std::string& key) const;

  bool boolean(const std::string& key) const;

  private:

  std::unordered_map<std::string, std::string> m_config;
  };
}
