#pragma once

#include <Jam/Entity.hpp>
#include <SFML/Graphics.hpp>

namespace jam
{
  class AnimatedSprite : public sf::Sprite
  {
  public:

    AnimatedSprite(
      sf::Texture& texture,
      int width,
      int height,
      int frames,
      float frameSpeed = 2.0f,
      int startFrame = 0
    );
    void update(const float dt);
    void resetAnimation();
    void setSpeed(float speed);
    float getSpeed();
    int getCurFrame();

  private:

    int m_frameWidth;
    int m_frameHeight;
    int m_curFrame;
    int m_startFrame;
    int m_maxFrames;
    float m_frameSpeed;
    float m_startSpeed;
    float m_curTime;

    void updateSprite();
  };
}