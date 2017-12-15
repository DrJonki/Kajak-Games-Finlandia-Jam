#include <Jam/Scenes/LevelScene.hpp>
#include <Jam/Entities/Player.hpp>
#include <Jam/Layer.hpp>
#include <Jam/Instance.hpp>
#include <Jam/Entities/Obstacle.hpp>
#include <SFML/Graphics/View.hpp>
#include <Jam/Scenes/TitleScene.hpp>
#include <Jam/Entities/GenericEntity.hpp>
#include <Jam/Entities/Storm.hpp>
#include <SFML/Window/Mouse.hpp>
#include <iostream>

namespace jam
{
  LevelScene::LevelScene(Instance& ins, const rapidjson::Value& data)
    : Scene(ins),
      m_backgroundLayer(addLayer(10)),
      m_propLayer(addLayer(20)),
      m_characterLayer(addLayer(30)),
      m_stormLayer(addLayer(32)),
      m_uiLayers(),
      m_uiState(UIState::None),
      m_crossHair(sf::Vector2f(20, 20)),
      m_gameView(sf::Vector2f(), sf::Vector2f(data["view"][0u].GetFloat(), data["view"][1].GetFloat())),
      m_uiView(sf::Vector2f(0.5f, 0.5f), sf::Vector2f(1.f, 1.f)),
      m_player(m_characterLayer.insert<Player>(data["id"].GetString(), ins, *this, true, data, m_gameView)),
      m_backgroundSound()
  {
    m_backgroundSound[0].setLoop(true);
    m_backgroundSound[0].setVolume(15);
    m_backgroundSound[0].setRelativeToListener(true);
    m_backgroundSound[0].openFromFile("assets/Audio/effects/background_gunfire_distant.ogg");
    m_backgroundSound[0].play();

    m_backgroundSound[1].setLoop(true);
    m_backgroundSound[1].setVolume(25);
    m_backgroundSound[1].setRelativeToListener(true);
    m_backgroundSound[1].openFromFile("assets/Audio/effects/background_gunfire.ogg");
    m_backgroundSound[1].play();

    m_backgroundLayer.setSharedView(&m_gameView);
    m_propLayer.setSharedView(&m_gameView);
    m_characterLayer.setSharedView(&m_gameView);
    m_stormLayer.setSharedView(&m_gameView);

    m_crossHair.setTexture(&ins.resourceManager.GetTexture("crosshair.png"));
    m_crossHair.setOrigin(m_crossHair.getSize() * 0.5f);

    for (int i = 0; i < 2; ++i) {
      m_uiLayers.push_back(&addLayer(40 + i));
      m_uiLayers.back()->setSharedView(&m_uiView);
    }

    m_stormLayer.insert<Storm>("storm", *this, m_gameView);

    const auto& font = ins.resourceManager.GetFont("BEBAS.ttf");

    auto& deathUiLayer = m_uiLayers[static_cast<int>(UIState::Dead)];
    {
      auto& deathText = deathUiLayer->insert<GenericEntity<sf::Text>>("deathText");
      deathText.setFont(font);
      deathText.setCharacterSize(128);
      deathText.setOutlineThickness(5.f);
      deathText.setOutlineColor(sf::Color::White);
      deathText.setFillColor(sf::Color::Black);
      deathText.setString("DEAD");
      const auto bounds = deathText.getLocalBounds();
      deathText.setOrigin(bounds.width / 2, bounds.height / 2);
      deathText.setScale(0.0005f, 0.0005f);
      deathText.setPosition(0.5f, 0.45f);
    } {
      auto& deathText = deathUiLayer->insert<GenericEntity<sf::Text>>("deathText2");
      deathText.setFont(font);
      deathText.setCharacterSize(128);
      deathText.setOutlineThickness(5.f);
      deathText.setOutlineColor(sf::Color::White);
      deathText.setFillColor(sf::Color::Black);
      deathText.setString("Waiting  for  respawn");
      const auto bounds = deathText.getLocalBounds();
      deathText.setOrigin(bounds.width / 2, bounds.height / 2);
      deathText.setScale(0.00025f, 0.00025f);
      deathText.setPosition(0.5f, 0.55f);
    }

    // Other players
    {
      auto& playerArray = data["players"];

      for (auto& itr : playerArray.GetArray()) {
        m_characterLayer.insert<Player>(data["id"].GetString(), ins, *this, false, data, m_gameView);
      }
    }

    // Level generation
    {
      auto& levelData = data["level"];
      auto& propArray = levelData["props"];

      for (auto& itr : propArray.GetArray()) {
        m_propLayer.insert<Obstacle>(itr["id"].GetString(), getInstance(), itr);
      }
    }
  }

  LevelScene::~LevelScene()
  {
    getInstance().sendMessage("disconnect", true);
  }

  void LevelScene::update(const float dt)
  {
    Scene::update(dt);
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)  && !m_player.isDead() && m_player.getTriggerReady()) {
      m_player.shoot();
    }

    if (m_player.isDead()) {
      m_uiState |= UIState::Dead;
    }
    else {
      m_uiState &= ~UIState::Dead;
    }

    m_gameView.setCenter(m_player.getPosition());

    for (std::size_t i = 0; i < m_uiLayers.size(); ++i) {
      m_uiLayers[i]->setActive(i == static_cast<std::size_t>((m_uiState & 1 << (i + 1)) != 0));
    }

    const auto mouseWorld = getInstance().window.mapPixelToCoords(sf::Mouse::getPosition(getInstance().window), m_gameView);
    m_crossHair.setPosition(mouseWorld);
  }

  void LevelScene::draw(sf::RenderTarget & target)
  {
    target.clear(sf::Color(222, 222, 222));

    for (auto& itr : m_bulletHoles) {
      target.draw(itr);
    }

    Scene::draw(target);

    //if (!m_player.isDead())
      //target.draw(m_crossHair);
  }

  void LevelScene::textEvent(const uint32_t code)
  {
    if (code == 0x1B) { // Escape
      quit();
    }
  }

  void LevelScene::mousePressed(const int mouseKey, const int x, const int y)
  {
    if (mouseKey == sf::Mouse::Button::Left && !m_player.isDead() && m_player.getTriggerReady()) {

      const auto world = getInstance().window.mapPixelToCoords(sf::Vector2i(x, y), m_gameView) + m_player.getInAccuracy();
      spawnBulletHole(world);
      rapidjson::Document data(rapidjson::kObjectType);
      rapidjson::Value point(rapidjson::kArrayType);
      point.PushBack(world.x, data.GetAllocator());
      point.PushBack(world.y, data.GetAllocator());
      data.AddMember("crosshairPosition", point, data.GetAllocator());
      rapidjson::Value weaponType(m_player.getCurrentWeapon());
      data.AddMember("weaponType", weaponType, data.GetAllocator());
      getInstance().sendMessage("shoot", data, false);
    }
  }

  void LevelScene::spawnBulletHole(sf::Vector2f pos) {
    sf::CircleShape bullet = sf::CircleShape();
    bullet.setPosition(pos);
    bullet.setRadius(2);
    bullet.setFillColor(sf::Color(40, 40, 40));
    m_bulletHoles.push_back(bullet);
  }

  void LevelScene::socketEvent(const char * message, const rapidjson::Value & data)
  {
    Scene::socketEvent(message, data);

    auto findPlayerId = [&data]() -> const char* {
      return data["id"].GetString();
    };

    if (strcmp(message, "join") == 0) {
      m_characterLayer.insert<Player>(findPlayerId(), getInstance(), *this, false, data, m_gameView);
    }

    else if (strcmp(message, "leave") == 0) {
      m_characterLayer.get(findPlayerId())->erase();
    }

    else if (strcmp(message, "kick") == 0) {
      quit();
    }

    else if (strcmp(message, "restart") == 0) {
      quit();
    }

    else if (strcmp(message, "crash") == 0) {
      quit();
    }
  }

  void LevelScene::quit()
  {
    getInstance().currentScene = std::make_unique<TitleScene>(getInstance());
  }
}