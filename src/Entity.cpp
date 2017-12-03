#include <Jam/Entity.hpp>

namespace jam
{

  Entity::Entity()
    : m_active(true),
      m_deleted(false)
  {

  }

  Entity::~Entity()
  {}


  void Entity::erase()
  {
    m_deleted = true;
  }


  bool Entity::isErased() const
  {
    return m_deleted;
  }

  void Entity::baseUpdate(const float dt)
  {
    if (isActive()) {
      update(dt);
    }
  }

  void Entity::baseDraw(sf::RenderTarget& target)
  {
    if (isActive()) {
      draw(target);
    }
  }

  void Entity::update(const float dt)
  {}

  void Entity::draw(sf::RenderTarget& target)
  {}

  void Entity::setActive(const bool active)
  {
    m_active = active;
  }

  bool Entity::isActive() const
  {
    return m_active;
  }

  void Entity::setID(const std::string & id)
  {
    m_id = id;
  }

  const std::string& Entity::getID() const
  {
    return  m_id;
  }
}
