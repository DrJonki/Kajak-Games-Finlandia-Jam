#pragma once

#include <Jam/Scene.hpp>
#include <Jam/Entities/Player.hpp>
#include <vector>
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Audio/Music.hpp>
#include <rapidjson/document.h>
#include <unordered_map>

namespace jam
{
  class LevelScene final : public Scene
  {
  private:

    enum UIState {
      None = 0,
      Dead = 1,
      Leaderboard = 1 << 1,
    };

  public:

    LevelScene(Instance& ins, const rapidjson::Value& playerInfo);

    ~LevelScene() override;

    void update(const float dt) override;

    void draw(sf::RenderTarget& target) override;

    void textEvent(const uint32_t code) override;

    void mousePressed(const int mouseKey, const int x, const int y) override;

    void socketEvent(const char* message, const rapidjson::Value& data) override;
    void spawnBulletHole(sf::Vector2f pos);

  private:

    void quit();

    Layer& m_backgroundLayer;
    Layer& m_propLayer;
    Layer& m_characterLayer;
    Layer& m_stormLayer;
    std::vector<Layer*> m_uiLayers;
    std::size_t m_uiState;

    // Entities
    Player& m_player;

    // Crosshair
    sf::RectangleShape m_crossHair;

    sf::View m_gameView;
    sf::View m_uiView;
    std::vector<sf::CircleShape> m_bulletHoles;
    sf::Music m_backgroundSound[2];
  };
}