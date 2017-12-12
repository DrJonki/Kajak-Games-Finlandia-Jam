#include <Jam/Instance.hpp>
#include <Jam/Scene.hpp>
#include <Jam/PostProcessor.hpp>
#include <SFML/Window/Event.hpp>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <iostream>
#include <string>

sf::VideoMode getVideomode(const jam::ConfigManager& config) {
  #ifdef _DEBUG
    return sf::VideoMode(config.integer("VIEW_X"), config.integer("VIEW_Y"));
  #else
    return sf::VideoMode::getDesktopMode();
  #endif
}

sf::Uint32 getStyle() {
  #ifdef _DEBUG
    return sf::Style::Default;
  #else
    return sf::Style::None;
  #endif
}

namespace
{
  const float ns_pingTime = 2.f;
}

namespace jam
{
  Instance::Instance()
    : config(),
      window(getVideomode(config), "Jam", getStyle(), sf::ContextSettings(0u, 0u, 0u, 3u, 0u)),
      framebuffer(),
      currentScene(),
      resourceManager(),
      postProcessor(*this),
      m_clock(),
      m_quad(),
      m_sockets(),
      m_pingTimer(),
      m_pingClock(),
      m_lastPingTime()
  {
    window.setVerticalSyncEnabled(true);
    window.setKeyRepeatEnabled(false);
    window.setMouseCursorVisible(false);

    connectTcp();

    if (udpSocket().bind(sf::Socket::AnyPort) != sf::Socket::Status::Done) {
      throw "Failed to bind UDP port";
    }

    tcpSocket().setBlocking(false);
    udpSocket().setBlocking(false);
  }

  Instance::~Instance()
  {
    tcpConnection().disconnect();
  }

  void Instance::operator ()()
  {
    const auto delta = m_clock.restart().asSeconds();

    if (currentScene) {
      static std::vector<char> buffer(sf::UdpSocket::MaxDatagramSize * 5); // Bigger buffer for TCP messages
      sf::IpAddress addr;
      unsigned short port = 0;
      std::size_t received = 0;

      while (
		m_sockets.first.receive(&buffer[0], buffer.size(), received) == sf::Socket::Done ||
        m_sockets.second.receive(&buffer[0], buffer.size(), received, addr, port) == sf::Socket::Done
      ) {
        rapidjson::Document doc;
        doc.ParseInsitu<rapidjson::kParseStopWhenDoneFlag>(buffer.data());

        if (!doc.HasParseError() && doc.HasMember("package") && doc["package"].IsString()) {
          static const rapidjson::Value dummyData(rapidjson::kObjectType);

          auto pack = doc["package"].GetString();
          if (strcmp(pack, "ping") == 0) {
            sendMessage("pong", false);
          }
          else if (strcmp(pack, "pong") == 0) {
            m_lastPingTime = m_pingTimer.restart();
		  }
		  std::cout << pack << std::endl;

          currentScene->socketEvent(pack, doc.HasMember("data") ? doc["data"] : dummyData);
		}
		else {
			std::cout << "Invalid package" << std::endl;
		}
      }

      currentScene->update(delta);
    }

    static bool firstUpdate = true;
    if (firstUpdate || m_pingClock.getElapsedTime().asSeconds() >= ns_pingTime) {
      firstUpdate = false;
      sendMessage("ping", false);
      m_pingClock.restart();
      m_pingTimer.restart();
    }

    postProcessor.update(delta);
    postProcessor.render(delta);

    if (currentScene)
      currentScene->postDraw(window);

    window.display();

    // Handle events
    sf::Event event;
    while (window.pollEvent(event)) {
      switch (event.type)
      {
      case sf::Event::Closed:
        if (currentScene) {
          currentScene.reset();
        }
        window.close();
        break;
      case sf::Event::Resized:
      {
        const auto view = window.getView().getSize();
        const auto ratio = view.x / view.y;
        window.setSize(sf::Vector2u(
          static_cast<unsigned int>(window.getSize().y * ratio),
          window.getSize().y
        ));
        break;
      }
      case sf::Event::TextEntered:
      {
        currentScene->textEvent(event.text.unicode);
        break;
      }
      case sf::Event::MouseButtonPressed: 
      {
        currentScene->mousePressed(event.mouseButton.button, event.mouseButton.x, event.mouseButton.y);
        break;
      }
      }
    }
  }

  sf::TcpSocket & Instance::tcpSocket()
  {
    return m_sockets.first;
  }

  sf::UdpSocket & Instance::udpSocket()
  {
    return m_sockets.second;
  }

  void Instance::connectTcp()
  {
    if (tcpSocket().connect(sf::IpAddress(config.string("SERVER_ADDRESS")), config.integer("SERVER_PORT_TCP"), sf::seconds(5.f)) != sf::Socket::Done) {
      throw "Failed to connect";
    }
  }

  bool Instance::sendMessage(const char * message, const bool tcp)
  {
    rapidjson::Value val(rapidjson::kObjectType);

    return sendMessage(message, val, tcp);
  }

  bool Instance::sendMessage(const char* message, rapidjson::Value& data, const bool tcp)
  {
    using namespace rapidjson;

    static const sf::IpAddress address = sf::IpAddress(config.string("SERVER_ADDRESS"));
    static const unsigned short port = config.integer("SERVER_PORT_UDP");

    Document doc(rapidjson::kObjectType);
    doc.AddMember(StringRef("package"), StringRef(message), doc.GetAllocator());
    doc.AddMember(StringRef("data"), data, doc.GetAllocator());

    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    doc.Accept(writer);

    if (tcp) {
      size_t sentTotal = 0;

      while(true) {
        size_t sent = 0;
        const auto status = tcpSocket().send(&buffer.GetString()[sentTotal], buffer.GetSize() - sentTotal, sent);
        
        switch (status)
        {
          case sf::Socket::Done:
            return true;

          case sf::Socket::NotReady:
            continue;

          case sf::Socket::Partial: {
            sentTotal += sent;
            continue;
          }

          case sf::Socket::Disconnected: {
            tcpSocket().setBlocking(true);
            connectTcp();
            tcpSocket().setBlocking(false);
          }

          default:
            return false;
        };
      }
    }

    return udpSocket().send(buffer.GetString(), buffer.GetSize(), address, port) == sf::Socket::Done;
  }

  sf::Time Instance::getLastPingTime() const
  {
    return m_lastPingTime;
  }
}
