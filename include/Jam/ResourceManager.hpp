#pragma once

#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Shader.hpp>

namespace jam
{
  class ResourceManager
  {
    public:

      sf::Texture& GetTexture(const std::string& filePath);
      sf::SoundBuffer& GetSoundBuffer(const std::string& filePath);
      sf::Font& GetFont(const std::string& filePath);
      sf::Shader& GetShader(const std::string& vertex, const std::string& pixel);

    private:

      std::map<std::string, sf::Texture> textureFiles;
      std::map<std::string, sf::SoundBuffer> audioFiles;
      std::map<std::string, sf::Font> fontFiles;
      std::map<std::string, sf::Shader> shaderFiles;
  };
}