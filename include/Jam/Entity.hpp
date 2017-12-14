#pragma once

#include <string>

namespace sf
{
  class RenderTarget;
}

namespace jam
{
  class Entity
  {
    friend class Layer;

  public:

    Entity();

    virtual ~Entity() = 0;

    void erase();

    bool isErased() const;

    void setActive(const bool active);

    bool isActive() const;

    void setID(const std::string& id);

    const std::string& getID() const;

  private:

    void baseUpdate(const float dt);

    void baseDraw(sf::RenderTarget& target);

    virtual void update(const float dt);

    virtual void draw(sf::RenderTarget& target);

  private:

    std::string m_id;
    bool m_active;
    bool m_deleted;
  };
}
