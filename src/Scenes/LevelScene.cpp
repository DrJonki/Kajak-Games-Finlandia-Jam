#include <Jam/Scenes/LevelScene.hpp>
#include <Jam/Entities/Player.hpp>
#include <Jam/Layer.hpp>
#include <Jam/Instance.hpp>
#include <Jam/Entities/Obstacle.hpp>
#include <SFML/Graphics/View.hpp>
#include <Jam/Scenes/TitleScene.hpp>
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
      m_uiView(sf::Vector2f(0.5f, 0.5f), sf::Vector2f(1.f, 1.f)),
      m_player(m_characterLayer.insert<Player>("player_self", ins, *this, true))
  {
    m_backgroundLayer.setSharedView(&m_gameView);
    m_propLayer.setSharedView(&m_gameView);
    m_characterLayer.setSharedView(&m_gameView);
  }

  LevelScene::~LevelScene()
  {
    
  }

  void LevelScene::update(const float dt)
  {
    Scene::update(dt);

    m_gameView.setCenter(m_player.getPosition());
  }

  void LevelScene::draw(sf::RenderTarget & target)
  {
    target.clear(sf::Color(222, 222, 222));

    Scene::draw(target);
  }

  void LevelScene::textEvent(const uint32_t code)
  {
    if (code == 0x1B) {
      getInstance().currentScene = std::make_unique<TitleScene>(getInstance());
    }
  }
}