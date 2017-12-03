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
  LevelScene::LevelScene(Instance& ins, const Player::Faction faction)
    : Scene(ins),
      m_backgroundLayer(addLayer(10)),
      m_propLayer(addLayer(20)),
      m_characterLayer(addLayer(30)),
      m_uiLayers(),
      m_gameView(sf::Vector2f(), sf::Vector2f(ins.config.float_("VIEW_X"), ins.config.float_("VIEW_Y"))),
      m_uiView(sf::Vector2f(0.5f, 0.5f), sf::Vector2f(1.f, 1.f)),
      m_player(m_characterLayer.insert<Player>("player_self", ins, *this, true, faction)),
      m_music()
  {
    m_music.setLoop(true);
    m_music.setRelativeToListener(true);
    m_music.openFromFile("assets/Audio/sandstorm.ogg");
    m_music.play();

    m_backgroundLayer.setSharedView(&m_gameView);
    m_propLayer.setSharedView(&m_gameView);
    m_characterLayer.setSharedView(&m_gameView);

    m_propLayer.insert<Obstacle>("");
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
      getInstance().sendMessage("disconnect");
      getInstance().currentScene = std::make_unique<TitleScene>(getInstance());
    }
  }

  void LevelScene::socketEvent(const char * message, const rapidjson::Value & data)
  {
    if (strcmp(message, "join") == 0) {
      m_characterLayer.insert<Player>(data["id"].GetString(), getInstance(), *this, false, static_cast<Player::Faction>(data["side"].GetUint()));
    }

    else if (strcmp(message, "leave") == 0) {
      m_characterLayer.get(data["id"].GetString())->erase();
    }
  }
}