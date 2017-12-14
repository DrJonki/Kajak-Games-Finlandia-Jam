#include <Jam/AnimatedSprite.hpp>
#include <iostream>

namespace jam
{
  AnimatedSprite::AnimatedSprite(
    sf::Texture& texture,
    int width,
    int height,
    int frames,
    float frameSpeed,
    int startFrame
  )
    : sf::Sprite(texture)
  {
    m_frameWidth = width;
    m_frameHeight = height;
    m_startFrame = startFrame;
    m_startSpeed = frameSpeed;
    m_maxFrames = frames - 1;
    resetAnimation();
  }

  void AnimatedSprite::update(const float dt)
  {
    m_curTime += dt;
    if (m_curTime > m_frameSpeed)
    {
      m_curFrame += 1;
      if (m_curFrame > m_maxFrames)
      {
      m_curFrame = m_startFrame;
      }
      updateSprite();
      m_curTime = 0;
    }
  }

  void AnimatedSprite::updateSprite()
  {
    setTextureRect(
      sf::IntRect(
      m_curFrame * m_frameWidth, // rectleft
      0,             // recttop
      m_frameWidth,        // width
      m_frameHeight        // height
      )
    );
  }
  
  void AnimatedSprite::resetAnimation()
  {
    m_curTime = 0;
    m_curFrame = m_startFrame;
    m_frameSpeed = m_startSpeed;
    updateSprite();
  }

  void AnimatedSprite::setSpeed(float speed)
  {
    m_frameSpeed = speed;
  }

  float AnimatedSprite::getSpeed()
  {
    return m_frameSpeed;
  }

  int AnimatedSprite::getCurFrame()
  {
    return m_curFrame;
  }
}