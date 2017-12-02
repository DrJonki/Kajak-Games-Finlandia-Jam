#pragma once

#include <SFML/System/Vector2.hpp>

namespace jam
{
  class Instance;

  class InterpolatesTransform
  {
  public:

    InterpolatesTransform(Instance& ins);

    virtual ~InterpolatesTransform() = 0;

    void updatePosition(const sf::Vector2f& pos, const bool force = false);

    void updateAngle(const float angle, const bool force = false);

    sf::Vector2f getCurrentPos() const;

    float getCurrentAngle() const;

    void update(const float delta);

  private:

    Instance& m_instance;

    sf::Vector2f m_prevPos;
    sf::Vector2f m_nextPos;

    float m_prevAngle;
    float m_nextAngle;

    float m_currentDelta;
  };
}
