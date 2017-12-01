#include <Jam/Scenes/LevelScene.hpp>
#include <SFML/Network.hpp>

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

    sf::Packet packet;
    packet << "{ \"data\": \"asdf\" }";

    sock.send(packet, "127.0.0.1", 9002);
  }

  void LevelScene::update(const float dt)
  {
    
  }

}