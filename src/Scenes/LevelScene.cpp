#include <Jam/Scenes/LevelScene.hpp>
#include <SFML/Network.hpp>
#include <iostream>

namespace jam
{
  LevelScene::LevelScene(Instance& ins)
    : Scene(ins),
      m_backgroundLayer(addLayer(10)),
      m_propLayer(addLayer(20)),
      m_characterLayer(addLayer(30))
  {
    sf::UdpSocket sock;
    sock.bind(sf::Socket::AnyPort);

    const std::string str("{ \"data\": \"asdf\" }");

    //sf::Packet packet;
    //packet.clear();
    //packet << "{ \"data\": \"asdf\" }";
    //packet << 0;
    //std::cout << (char*)packet.getData() << std::endl;

    sock.send(str.c_str(), str.length(), "127.0.0.1", 9002);
  }

  void LevelScene::update(const float dt)
  {
    
  }

}