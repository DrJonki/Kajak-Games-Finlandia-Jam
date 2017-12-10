#pragma once

#include <Jam/Entity.hpp>
#include <Jam/Entities/ListensMessages.hpp>
#include <Jam/Entities/InterpolatesTransform.hpp>
#include <SFML/Graphics/CircleShape.hpp>

namespace jam
{
  class Player final : public Entity, public ListensMessages, public InterpolatesTransform, public sf::CircleShape
  {
  public:

    enum class Faction {
      Simo,
      Russian,
    };

  public:

    Player(Instance& ins, Scene& scene, const bool controllable, const Faction faction);

    bool isDead() const;

  private:

    void update(const float dt) override;

    void draw(sf::RenderTarget& target) override;

    void socketMessage(const char* message, const rapidjson::Value& data) override;

  private:

    Instance& m_instance;
    const Faction m_faction;
    const bool m_controllable;
    bool m_dead;
  };
}
