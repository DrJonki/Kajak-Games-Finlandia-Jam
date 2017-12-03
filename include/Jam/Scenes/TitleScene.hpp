#pragma once

#include <Jam/Scene.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Audio/Music.hpp>

namespace jam
{
  class TitleScene : public Scene
  {
  public:

    TitleScene(Instance& ins);

    ~TitleScene() override;

    void update(const float dt) override;

    void draw(sf::RenderTarget& target) override;

    void socketEvent(const char* event, const rapidjson::Value& data) override;

    void textEvent(const uint32_t code) override;

  private:

    bool isConnected() const;

    void setConnectionString(const std::string& string);

    float m_pingTimer, m_pongTimer;
    sf::View m_view;
    sf::RectangleShape m_background;
    sf::Text m_titleText[3];

    sf::CircleShape m_connectionStatus;
    sf::Text m_connectionText;
    sf::Clock m_pingClock;

    sf::Text m_instructionText[2];

    bool m_findingGame;
    sf::Text m_connectingText;

    sf::Music m_music;
  };
}