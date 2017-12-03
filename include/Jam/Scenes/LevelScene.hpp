#pragma once

#include <Jam/Scene.hpp>
#include <Jam/Entities/Player.hpp>
#include <vector>
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Audio/Music.hpp>
#include <unordered_map>

namespace jam
{
  class LevelScene final : public Scene
  {
  public:

    LevelScene(Instance& ins, const Player::Faction faction);

    ~LevelScene() override;

    void update(const float dt) override;

    void draw(sf::RenderTarget& target) override;

    void textEvent(const uint32_t code) override;

    void mousePressed(const int mouseKey, const int x, const int y) override;

    void socketEvent(const char* message, const rapidjson::Value& data) override;

  private:

    Layer& m_backgroundLayer;
    Layer& m_propLayer;
    Layer& m_characterLayer;
    std::vector<Layer*> m_uiLayers;

    // Entities
    Player& m_player;

    // Crosshair
    sf::RectangleShape m_crossHair;

    sf::View m_gameView;
    sf::View m_uiView;

    sf::Music m_music;
  };
}