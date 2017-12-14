#pragma once

#include <Jam/Entity.hpp>
#include <Jam/AnimatedSprite.hpp>
#include <Jam/Entities/Particle.hpp>
#include <vector>

namespace jam
{
  class Instance;
  class ParticleEmitter final : public Entity
  {
  public:

  ParticleEmitter(
    Instance& inst,
    std::string texturePath,
    sf::Vector2f textureSize,
    int maxParticles,
    float emitTime,
    float lifetime = 1.f,
    float startSpeed = 0.5f,
    float friction = 0.1f,
    float startAngle = 20.f,
    float startTorgue = 20.f,
    float maxAlpha = 255.f,
    float minAlpha = 0.f
  );

  void emit();
  void setPosition(sf::Vector2f emitPosition);
  void setStartRotation(float rot);
  void stop();

  void update(const float dt) override;

  void draw(sf::RenderTarget& target) override;

  bool isStartedEmitting;
  bool isDone;

  private:

  std::vector<Particle> m_particles;
  sf::Texture& m_texture;
  sf::Vector2f m_textureSize;
  sf::Vector2f m_emitPosition;
  float m_emitTime;
  float m_curTime;
  float m_lifeTime;
  float m_startSpeed;
  float m_friction;
  float m_startAngle;
  float m_startTorgue;
  float m_maxAlpha;
  float m_minAlpha;


  };
}