#include <Jam/Entities/InterpolatesTransform.hpp>
#include <Jam/Instance.hpp>
#include <glm/common.hpp>

namespace jam
{
  InterpolatesTransform::InterpolatesTransform(Instance& ins)
    : m_instance(ins),
      m_prevPos(0.f),
      m_nextPos(0.f),
      //m_prevDirection(0, 0),
      //m_nextDirection(0, 0),
      m_currentDelta(0.f)
  {}

  InterpolatesTransform::~InterpolatesTransform()
  {}

  void InterpolatesTransform::updatePosition(const glm::vec2& pos, const bool force /*= false*/)
  {
    m_prevPos = force ? pos : getCurrentPos();
    m_nextPos = pos;

    m_currentDelta = 0.f;
  }

  //void InterpolatesTransform::updateDirection(const glm::vec2& dir, const bool force /*= false*/)
  //{
  //  m_prevDirection = force ? dir : getCurrentDirection();
  //  m_nextDirection = dir;

  //  m_currentDelta = 0.f;
  //}

  glm::vec2 InterpolatesTransform::getCurrentPos() const
  {
    return glm::mix(m_prevPos, m_nextPos, m_currentDelta);
  }

  /*glm::vec2 InterpolatesTransform::getCurrentDirection() const
  {
    return glm::mix(m_prevDirection, m_nextDirection, m_currentDelta);
  }*/

  void InterpolatesTransform::update(const float delta)
  {
    static const float stepLen = m_instance.config.float_("INTERPOLATION_TICK_LENGTH");

    m_currentDelta = std::min(1.f, m_currentDelta + (delta / stepLen));
  }
}
