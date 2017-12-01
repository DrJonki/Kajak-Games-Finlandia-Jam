#pragma once

#include <Jam/Scene.hpp>

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
  };
}