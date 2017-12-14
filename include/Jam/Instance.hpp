#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Network/TcpSocket.hpp>
#include <SFML/Network/UdpSocket.hpp>
#include <rapidjson/document.h>
#include <memory>
#include <Jam/ResourceManager.hpp>
#include <Jam/ConfigManager.hpp>
#include <Jam/PostProcessor.hpp>
#include <set>

namespace sf
{
  class TcpSocket;
  class UdpSocket;
  class Socket;
}

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

  bool sendMessage(const char* message, const bool tcp);

  bool sendMessage(const char* message, rapidjson::Value& data, const bool tcp);

  sf::Time getLastPingTime() const;

  private:

  sf::TcpSocket& tcpSocket();

  sf::UdpSocket& udpSocket();

  void connectTcp();

  public:

  // Globals
  ConfigManager config;
  sf::RenderWindow window;
  sf::RenderTexture framebuffer[2];
  std::unique_ptr<Scene> currentScene;
  ResourceManager resourceManager;
  PostProcessor postProcessor;

  public:

  sf::Clock m_clock;

  private:

  std::pair<sf::TcpSocket, sf::UdpSocket> m_sockets;
  sf::RectangleShape m_quad;
  sf::Clock m_pingTimer;
  sf::Clock m_pingClock;
  sf::Time m_lastPingTime;
  };
}