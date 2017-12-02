#include <Jam/Scenes/LevelScene.hpp>
#include <SFML/Network.hpp>
#include <iostream>

namespace jam
{
  LevelScene::LevelScene(Instance& ins)
    : Scene(ins),
      m_backgroundLayer(addLayer(10)),
      m_propLayer(addLayer(20)),
      m_characterLayer(addLayer(30)),
      m_uiLayers()
  {

  }

  void LevelScene::update(const float dt)
  {
    
  }

}