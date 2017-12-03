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

namespace jam
{
  Instance::Instance()
    : config(),
      window(getVideomode(config), "Jam", getStyle(), sf::ContextSettings(0u, 0u, 0u, 3u, 0u)),
      framebuffer(),
      currentScene(),
      resourceManager(),
      postProcessor(*this),
      socket(),
      m_clock(),
      m_quad()
  {
    window.setVerticalSyncEnabled(true);
    window.setKeyRepeatEnabled(false);
    window.setMouseCursorVisible(
    #ifdef _DEBUG
      false
    #else
      false
    #endif
    );

    if (socket.bind(sf::Socket::AnyPort) != sf::Socket::Status::Done) {
      assert(false);
    }

    socket.setBlocking(false);
  }

  Instance::~Instance()
  {

  }

  void Instance::operator ()()
  {
    const auto delta = m_clock.restart().asSeconds();

    if (currentScene) {
      static std::vector<char> buffer(sf::UdpSocket::MaxDatagramSize);
      sf::IpAddress addr;
      unsigned short port = 0;
      std::size_t received = 0;

      while (socket.receive(&buffer[0], buffer.size(), received, addr, port) == sf::UdpSocket::Done) {
        rapidjson::Document doc;
        doc.ParseInsitu<rapidjson::kParseStopWhenDoneFlag>(buffer.data());

        if (!doc.HasParseError() && doc.HasMember("package")) {
          rapidjson::Value dummyData;
          dummyData.SetObject();

          currentScene->socketEvent(doc["package"].GetString(), doc.HasMember("data") ? doc["data"] : dummyData);
        }
      }

      currentScene->update(
        delta * config.float_("SPEED_MULT") // Global game speed multiplier
      );
    }

    postProcessor.update(delta),
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
      case sf::Event::KeyReleased:
      {
        /*if (event.key.code == sf::Keyboard::Escape)
          window.close();*/

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

  bool Instance::sendMessage(const char * message)
  {
    rapidjson::Value val;
    val.SetObject();

    return sendMessage(message, val);
  }

  bool Instance::sendMessage(const char* message, rapidjson::Value& data)
  {
    using namespace rapidjson;

    static const sf::IpAddress address = sf::IpAddress(config.string("SERVER_ADDRESS"));
    static const unsigned short port = config.integer("SERVER_PORT");

    Document doc;
    doc.SetObject();
    doc.AddMember(StringRef("package"), StringRef(message), doc.GetAllocator());
    doc.AddMember(StringRef("data"), data, doc.GetAllocator());

    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    doc.Accept(writer);

    return socket.send(buffer.GetString(), buffer.GetSize(), address, port) == sf::UdpSocket::Done;
  }
}

