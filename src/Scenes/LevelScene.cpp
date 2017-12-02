#include <Jam/Scenes/LevelScene.hpp>
#include <Jam/Entities/Player.hpp>
#include <Jam/Layer.hpp>
#include <Jam/Instance.hpp>
#include <Jam/Entities/Obstacle.hpp>
#include <SFML/Graphics/View.hpp>
#include <iostream>

namespace jam
{
  LevelScene::LevelScene(Instance& ins)
    : Scene(ins),
      m_backgroundLayer(addLayer(10)),
      m_propLayer(addLayer(20)),
      m_characterLayer(addLayer(30)),
      m_uiLayers(),
      m_gameView(sf::Vector2f(), sf::Vector2f(ins.config.float_("VIEW_X"), ins.config.float_("VIEW_Y"))),
      m_player(m_characterLayer.insert<Player>("player_self", ins, *this, true))
  {
    m_backgroundLayer.setSharedView(&m_gameView);
    m_propLayer.setSharedView(&m_gameView);
    m_characterLayer.setSharedView(&m_gameView);

    m_propLayer.insert<Obstacle>("");
  }

  LevelScene::~LevelScene()
  {
    
  }

  void LevelScene::update(const float dt)
  {
    Scene::update(dt);

    m_gameView.setCenter(m_player.getPosition());
  }

}