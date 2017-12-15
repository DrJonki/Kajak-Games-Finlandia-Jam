#include <Jam/Entities/Obstacle.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <Jam/Instance.hpp>

namespace jam
{
  Obstacle::Obstacle(Instance& ins, const rapidjson::Value& data)
    : Entity(),
      m_type(static_cast<Type>(data["type"].GetInt())),
      m_breakable(data.HasMember("breakable") ? data["breakable"].GetBool() : false),
      m_health(data.HasMember("health") ? data["health"].GetInt() : 0),
      m_drawable()
  {
    switch (m_type)
    {
      case Type::Rock: {
        auto d = std::make_unique<sf::CircleShape>();

        const auto radius = data["radius"].GetFloat();
        d->setRadius(radius);
        d->setOrigin(radius, radius);
        d->setTexture(&ins.resourceManager.GetTexture("rock.png"));

        m_drawable = std::move(d);

        break;
      }
      case Type::House: {
        auto d = std::make_unique<sf::RectangleShape>();

        const auto size = sf::Vector2f(data["size"][0u].GetFloat(), data["size"][1].GetFloat());
        d->setSize(size);
        d->setOrigin(size * 0.5f);
        d->setTexture(&ins.resourceManager.GetTexture("roof_square.png"));

        m_drawable = std::move(d);

        break;
      }
      default: {
        assert(false);
      }
    }

    auto& d = reinterpret_cast<sf::Transformable&>(*m_drawable);
    d.setPosition(data["position"][0u].GetFloat(), data["position"][1].GetFloat());
    d.setRotation(data["angle"].GetFloat());
  }

  Obstacle::Type Obstacle::getType() const
  {
    return m_type;
  }

  bool Obstacle::isBroken() const
  {
    return m_breakable && m_health <= 0;
  }

  const sf::Shape & Obstacle::getShape() const
  {
    return reinterpret_cast<sf::Shape&> (*m_drawable);
    // TODO: insert return statement here
  }

  sf::CircleShape & Obstacle::circleShape()
  {
    return static_cast<sf::CircleShape&>(*m_drawable);
  }

  sf::RectangleShape & Obstacle::rectangleShape()
  {
    return static_cast<sf::RectangleShape&>(*m_drawable);
  }

  void Obstacle::draw(sf::RenderTarget& target)
  {
    target.draw(*m_drawable);
  }
}
