#pragma once

#include <Jam/Entity.hpp>
#include <Jam/Entities/ListensMessages.hpp>
#include <Jam/Entities/InterpolatesTransform.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Audio/Sound.hpp>

namespace sf {
  class View;
}

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

    Player(Instance& ins, Scene& scene, const bool controllable, const rapidjson::Value& data, const sf::View& view);

    void offsetHealth(const int health);

    void setHealth(const int health);

    bool isDead() const;
	void shoot();

  private:

    void update(const float dt) override;

    void draw(sf::RenderTarget& target) override;

    void socketMessage(const char* message, const rapidjson::Value& data) override;

  private:

    Instance& m_instance;
    const Faction m_faction;
    const bool m_controllable;
    int m_health;
    glm::vec2 m_speedVec;
    glm::vec2 m_accelVec;
    glm::vec2 m_pointingDir;
    float m_max_speedFloat;
    float m_accelFloat;
    float m_friction;
    const sf::View& m_view;
    glm::vec2 m_targetDirection;
    float m_velocity;
    sf::RectangleShape m_rectangles[4];
	  sf::Sound m_bang_sound;
    float m_playerRotatio;
  };
}
