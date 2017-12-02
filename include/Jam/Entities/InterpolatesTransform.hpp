#pragma once

#include <SFML/System/Vector2.hpp>
#include <glm/vec2.hpp>

namespace jam
{
  class Instance;

  class InterpolatesTransform
  {
  public:

    InterpolatesTransform(Instance& ins);

    virtual ~InterpolatesTransform() = 0;

    void updatePosition(const glm::vec2& pos, const bool force = false);

    // void updateDirection(const glm::vec2& dir, const bool force = false);

    glm::vec2 getCurrentPos() const;

    glm::vec2 getCurrentDirection() const;

    void update(const float delta);

  private:

    Instance& m_instance;

    glm::vec2 m_prevPos;
    glm::vec2 m_nextPos;

    /*glm::vec2 m_prevDirection;
    glm::vec2 m_nextDirection;*/

    float m_currentDelta;
  };
}
