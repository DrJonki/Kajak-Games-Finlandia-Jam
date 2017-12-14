#pragma once

#include <Jam/Entity.hpp>
#include <SFML/Graphics.hpp>
#include <Jam/Randomizer.hpp>

namespace jam
{
  class Particle : public sf::Sprite
  {
  public:

  Particle(
    sf::Texture& texture,
    float width,
    float height,
    sf::Vector2f startPosition,
    float lifetime,
    float startSpeed,
    float friction,
    float startAngle,
    float startTorgue,
    float maxAlpha,
    float minAlpha
  );
  void update(const float dt);
  bool isDone = false;

  private:
  Randomizer m_random;

  float m_width;
  float m_height;

  sf::Vector2f m_startPosition;

  float m_startSpeed;
  float m_friction;
  float m_curSpeed;

  float m_curTime;
  float m_lifeTime;

  float m_torgue;

  float m_maxAlpha;
  float m_minAlpha;

  sf::Vector2f m_direction;

  void moveParticle(const float dt);
  };
}