#pragma once

#include <Jam/Scene.hpp>
#include <vector>

namespace jam
{
  class LevelScene : public Scene
  {
  public:

    LevelScene(Instance& ins);

    void update(const float dt) override;

  private:

    Layer& m_backgroundLayer;
    Layer& m_propLayer;
    Layer& m_characterLayer;
    std::vector<Layer*> m_uiLayers;
  };
}