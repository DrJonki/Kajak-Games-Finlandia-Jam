#include <Jam/Scenes/TitleScene.hpp>
#include <Jam/Instance.hpp>
#include <Jam/Scenes/LevelScene.hpp>

namespace jam
{
  TitleScene::TitleScene(Instance& ins)
    : Scene(ins),
      m_view(sf::Vector2f(0.5f, 0.5f), sf::Vector2f(1.f, 1.f)),
      m_background(sf::Vector2f(1.f, 1.f)),
      m_titleText(),
      m_connectionText(),
      m_instructionText(),
      m_connectingText(),
      m_findingGame(false),
      m_matchmakingTimer(),
      m_music()
  {
    m_music.setLoop(true);
    m_music.setRelativeToListener(true);
    m_music.openFromFile("assets/Audio/finlandia.ogg");
    m_music.play();

    m_background.setTexture(&ins.resourceManager.GetTexture("titlebg.jpg"));

    m_titleText[0].setFont(ins.resourceManager.GetFont("AmazDooMLeft.ttf"));
    m_titleText[0].setString("Si");
    m_titleText[1].setFont(ins.resourceManager.GetFont("AmazDooMRight.ttf"));
    m_titleText[1].setString("mO");
    m_titleText[2].setFont(ins.resourceManager.GetFont("AmazDooMLeft.ttf"));
    m_titleText[2].setString("karjala takas");
    for (int i = 0; i < 3; ++i) {
      auto& t = m_titleText[i];
      t.setCharacterSize(128);
      t.setFillColor(sf::Color(33, 33, 33));
      t.setOutlineColor(sf::Color(255, 165, 0));
      t.setOutlineThickness(3.f);

      const float scale = 0.00175f;
      t.setScale(scale, scale * 1.2f);
      t.setPosition(0.05f + 0.12f * i, 0.05f);
    }
    m_titleText[2].setOutlineThickness(3);
    m_titleText[2].setPosition(0.06f, 0.3f);
    m_titleText[2].setScale(0.0005f, 0.0005f);

    m_connectionStatus.setFillColor(sf::Color::Red);
    m_connectionStatus.setRadius(0.03f);
    m_connectionStatus.setScale(0.6f, 1.f);
    m_connectionStatus.setPosition(0.925f, 0.05f);

    const auto& font = ins.resourceManager.GetFont("BEBAS.ttf");
    m_connectionText.setFont(font);
    m_connectionText.setPosition(0.9f, 0.09f);
    m_connectionText.setScale(0.001f, 0.001f);
    m_connectionText.setFillColor(sf::Color::Black);
    setConnectionString("Connecting...");

    m_connectingText.setFont(font);
    m_connectingText.setString("Connecting...");
    m_connectingText.setCharacterSize(128);
    m_connectingText.setFillColor(sf::Color::Black);
    m_connectingText.setOutlineColor(sf::Color::White);
    m_connectingText.setOutlineThickness(2.f);
    const auto bounds = m_connectingText.getLocalBounds();
    m_connectingText.setOrigin(bounds.width / 2, bounds.height / 2);
    m_connectingText.setScale(0.0005f, 0.0005f);
    m_connectingText.setPosition(0.5f, 0.5f);

    for (int i = 0; i < 2; ++i) {
      auto& t = m_instructionText[i];
      const float scale = 0.0015f;
      t.setScale(scale, scale);
      t.setFont(font);
      t.setFillColor(sf::Color::Black);
      t.setOutlineColor(sf::Color::White);
      t.setOutlineThickness(2.f);
    }
    m_instructionText[0].setString("ENTER - Start  game");
    m_instructionText[0].setPosition(0.025f, 0.9f);
    m_instructionText[1].setString("ESC - Quit");
    m_instructionText[1].setOrigin(m_instructionText[1].getLocalBounds().width, 0);
    m_instructionText[1].setPosition(0.975f, 0.9f);
  }

  TitleScene::~TitleScene()
  {}

  void TitleScene::update(const float dt)
  {
    Scene::update(dt);

    if (getInstance().getLastPingTime().asSeconds() > 2.f) {
      m_connectionStatus.setFillColor(sf::Color::Red);
      setConnectionString("No connection!");
    }

    {
      const auto ms = getInstance().getLastPingTime().asMilliseconds();
      sf::Color color = sf::Color::Red;

      if (ms < 80) {
        color = sf::Color::Green;
      }
      else if (ms < 160) {
        color = sf::Color::Yellow;
      }

      setConnectionString(std::to_string(ms) + "ms");
      m_connectionStatus.setFillColor(color);
    }

    if (m_findingGame && m_matchmakingTimer.getElapsedTime().asSeconds() >= 5.f) {
      m_findingGame = false;
    }
  }

  void TitleScene::draw(sf::RenderTarget & target)
  {
    Scene::draw(target);

    target.setView(m_view);
    target.draw(m_background);

    for (int i = 0; i < sizeof(m_titleText) / sizeof(m_titleText[0]); ++i) {
      target.draw(m_titleText[i]);
    }

    target.draw(m_connectionStatus);
    target.draw(m_connectionText);

    for (int i = 0; i < 2; ++i) {
      target.draw(m_instructionText[i]);
    }

    if (m_findingGame) {
      target.draw(m_connectingText);
    }
  }

  void TitleScene::socketEvent(const char * event, const rapidjson::Value & data)
  {
    Scene::socketEvent(event, data);

    if (m_findingGame && strcmp(event, "connect") == 0) {
      getInstance().currentScene = std::make_unique<LevelScene>(getInstance(), data);
    }
  }

  void TitleScene::textEvent(const uint32_t code)
  {
    if (!m_findingGame && code == 0xD) { // Enter
      m_findingGame = getInstance().sendMessage("connect", true);
      m_matchmakingTimer.restart();
    }
    else if (code == 0x1B) { // Escape
      getInstance().sendMessage("disconnect", true);
      getInstance().window.close();
    }
  }

  void TitleScene::setConnectionString(const std::string& string)
  {
    m_connectionText.setString(string);
    const auto bounds = m_connectionText.getLocalBounds();
    m_connectionText.setOrigin(bounds.width, bounds.height);
  }
}
