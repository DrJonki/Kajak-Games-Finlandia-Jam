#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Network/UdpSocket.hpp>
#include <rapidjson/document.h>
#include <memory>
#include <Jam/ResourceManager.hpp>
#include <Jam/ConfigManager.hpp>
#include <Jam/PostProcessor.hpp>
#include <set>

namespace jam
{
  class Scene;

  class Instance final
  {
    Instance(const Instance&) = delete;
    void operator =(const Instance&) = delete;

  public:

    Instance();

    ~Instance();

    void operator ()();

    bool sendMessage(const char* message);

    bool sendMessage(const char* message, rapidjson::Value& data);

  public:

    // Globals
    ConfigManager config;
    sf::RenderWindow window;
    sf::RenderTexture framebuffer[2];
    std::unique_ptr<Scene> currentScene;
    ResourceManager resourceManager;
    PostProcessor postProcessor;
    sf::UdpSocket socket;

  public:

    sf::Clock m_clock;

  private:

    sf::RectangleShape m_quad;
  };
}