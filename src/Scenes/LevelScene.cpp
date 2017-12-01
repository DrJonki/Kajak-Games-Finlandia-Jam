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

    sock.send(str.c_str(), str.length(), "127.0.0.1", 9002);

    std::vector<char> buf(65000);
    size_t received = 0;
    unsigned short port = 0;
    sock.receive(&buf[0], buf.size(), received, sf::IpAddress("127.0.0.1"), port);

    std::cout << "received: " << std::string(&buf[0], received) << std::endl;
  }

  void LevelScene::update(const float dt)
  {
    
  }

}