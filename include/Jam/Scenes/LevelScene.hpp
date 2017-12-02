#pragma once

#include <Jam/Scene.hpp>
#include <vector>
#include <SFML/Graphics/View.hpp>

namespace jam
{
  class Player;

  class LevelScene final : public Scene
  {
  public:

    LevelScene(Instance& ins);

    ~LevelScene() override;

    void update(const float dt) override;

  private:

    Layer& m_backgroundLayer;
    Layer& m_propLayer;
    Layer& m_characterLayer;
    std::vector<Layer*> m_uiLayers;

    // Entities
    Player& m_player;

    sf::View m_gameView;
  };
}