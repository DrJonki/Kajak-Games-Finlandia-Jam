#include <Jam/Scenes/LevelScene.hpp>
#include <Jam/Entities/Player.hpp>
#include <Jam/Layer.hpp>
#include <Jam/Instance.hpp>
#include <Jam/Entities/Obstacle.hpp>
#include <SFML/Graphics/View.hpp>
#include <Jam/Scenes/TitleScene.hpp>
#include <SFML/Window/Mouse.hpp>
#include <iostream>

namespace jam
{
  LevelScene::LevelScene(Instance& ins, const Player::Faction faction)
    : Scene(ins),
      m_backgroundLayer(addLayer(10)),
      m_propLayer(addLayer(20)),
      m_characterLayer(addLayer(30)),
      m_uiLayers(),
      m_crossHair(sf::Vector2f(20, 20)),
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

    m_crossHair.setTexture(&ins.resourceManager.GetTexture("crosshair.png"));
    m_crossHair.setOrigin(m_crossHair.getSize() * 0.5f);

    m_propLayer.insert<Obstacle>("");
  }

  LevelScene::~LevelScene()
  {
    getInstance().sendMessage("disconnect");
  }

  void LevelScene::update(const float dt)
  {
    Scene::update(dt);

    m_gameView.setCenter(m_player.getPosition());

    const auto mouseWorld = getInstance().window.mapPixelToCoords(sf::Mouse::getPosition(getInstance().window), m_gameView);
    m_crossHair.setPosition(mouseWorld);
  }

  void LevelScene::draw(sf::RenderTarget & target)
  {
    target.clear(sf::Color(222, 222, 222));

    Scene::draw(target);

    target.draw(m_crossHair);
  }

  void LevelScene::textEvent(const uint32_t code)
  {
    if (code == 0x1B) {
      getInstance().currentScene = std::make_unique<TitleScene>(getInstance());
    }
  }

  void LevelScene::mousePressed(const int mouseKey, const int x, const int y)
  {
    if (mouseKey == sf::Mouse::Button::Left) {
      const auto world = getInstance().window.mapPixelToCoords(sf::Vector2i(x, y), m_gameView);

      rapidjson::Document data;
      data.SetObject();
      rapidjson::Value point;
      point.SetArray();
      point.PushBack(world.x, data.GetAllocator());
      point.PushBack(world.y, data.GetAllocator());
      data.AddMember("crosshairPosition", point, data.GetAllocator());

      getInstance().sendMessage("shoot", data);
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