#pragma once

#include <Jam/Entity.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <rapidjson/document.h>
#include <memory>

namespace jam
{
  class Instance;

  class Obstacle : public Entity
  {
  public:

    enum class Type
    {
      Rock,
      House,
    };

  public:

    Obstacle(Instance& ins, const rapidjson::Value& data);
    
    Type getType() const;

    bool isBroken() const;

    const sf::Shape& getShape() const;
  private:

    sf::CircleShape& circleShape();

    sf::RectangleShape& rectangleShape();

    void draw(sf::RenderTarget& target) override;

  private:

    const Type m_type;
    const bool m_breakable;
    int m_health;
    std::unique_ptr<sf::Drawable> m_drawable;
  };
}
