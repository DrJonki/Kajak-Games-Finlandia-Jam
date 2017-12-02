#include <Jam/Entities/InterpolatesTransform.hpp>
#include <Jam/Instance.hpp>
#include <glm/common.hpp>

namespace jam
{
  InterpolatesTransform::InterpolatesTransform(Instance& ins)
    : m_instance(ins),
      m_prevPos(0, 0),
      m_nextPos(0, 0),
      m_prevAngle(0.f),
      m_nextAngle(0.f),
      m_currentDelta(0.f)
  {}

  InterpolatesTransform::~InterpolatesTransform()
  {}

  void InterpolatesTransform::updatePosition(const sf::Vector2f& pos, const bool force /*= false*/)
  {
    m_prevPos = force ? pos : getCurrentPos();
    m_nextPos = pos;
  }

  void InterpolatesTransform::updateAngle(const float angle, const bool force /*= false*/)
  {
    m_prevAngle = force ? angle : getCurrentAngle();
    m_nextAngle = angle;
  }

  sf::Vector2f InterpolatesTransform::getCurrentPos() const
  {
    const auto prev = glm::vec2(m_prevPos.x, m_prevPos.y);
    const auto next = glm::vec2(m_nextPos.x, m_nextPos.y);
    const auto result = glm::mix(prev, next, m_currentDelta);

    return sf::Vector2f(result.x, result.y);
  }

  float InterpolatesTransform::getCurrentAngle() const
  {
    return glm::mix(m_prevAngle, m_nextAngle, m_currentDelta);
  }

  void InterpolatesTransform::update(const float delta)
  {
    static const float stepLen = m_instance.config.float_("INTERPOLATION_TICK_LENGTH");

    m_currentDelta = std::min(1.f, m_currentDelta + (delta / stepLen));
  }
}
