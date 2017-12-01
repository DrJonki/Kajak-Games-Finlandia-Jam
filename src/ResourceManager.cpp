#include <Jam/ResourceManager.hpp>
#include <iostream>
#include <cassert>

namespace jam {
    sf::Texture& ResourceManager::GetTexture(const std::string& filePath)
    {
        std::string assetPath = "assets/Textures/" + filePath;
        auto itr = textureFiles.find(assetPath);
        if (itr == textureFiles.end()) {
            std::cout << "Loading " + assetPath + "..." << std::endl;
            sf::Texture& texture = textureFiles[assetPath];
            if (!texture.loadFromFile(assetPath))
            {
                assert(false);
            }
            std::cout << "Loading " + assetPath + " complete!" << std::endl;
            return texture;
        }
        return itr->second;
    }

    sf::SoundBuffer& ResourceManager::GetSoundBuffer(const std::string& filePath)
    {
        std::string assetPath = "assets/Audio/" + filePath;
        auto itr = audioFiles.find(assetPath);
        if (itr == audioFiles.end()) {
            std::cout << "Loading " + assetPath + "..." << std::endl;
            sf::SoundBuffer& soundBuff = audioFiles[assetPath];
            if (!soundBuff.loadFromFile(assetPath))
            {
                assert(false);
            }
            std::cout << "Loading " + assetPath + " complete!" << std::endl;
            return soundBuff;
        }
        return itr->second;
    }

    sf::Font& ResourceManager::GetFont(const std::string& filePath)
    {
        std::string assetPath = "assets/Fonts/" + filePath;
        auto itr = fontFiles.find(assetPath);
        if (itr == fontFiles.end()) {
            std::cout << "Loading " + assetPath + "..." << std::endl;
            sf::Font& font = fontFiles[assetPath];
            if (!font.loadFromFile(assetPath))
            {
                assert(false);
            }
            std::cout << "Loading " + assetPath + " complete!" << std::endl;
            return font;
        }
        return itr->second;
    }

    sf::Shader& ResourceManager::GetShader(const std::string& vertex, const std::string& pixel)
    {
      std::string assetPathV = "assets/Shaders/" + vertex;
      std::string assetPathF = "assets/Shaders/" + pixel;
      std::string combined = assetPathV + assetPathF;

      auto itr = shaderFiles.find(combined);
      if (itr == shaderFiles.end()) {
        sf::Shader& shader = shaderFiles[combined];

        if (!shader.loadFromFile(assetPathV, assetPathF)) {
          assert(false);
        }
        return shader;
      }

      return itr->second;
    }

}
