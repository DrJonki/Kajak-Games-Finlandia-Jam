#include <Jam/Entities/Player.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <rapidjson/pointer.h>
#include <Jam/Instance.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <glm/vec2.hpp>

namespace {
  const float ns_radius = 20.f;
}

namespace jam
{
  Player::Player(Instance& ins, Scene& scene, const bool controllable, const Faction faction)
    : Entity(),
      ListensMessages(scene, {"updateMovement"}),
      InterpolatesTransform(ins),
      sf::CircleShape(10.f),
      m_instance(ins),
      m_controllable(controllable),
      m_faction(faction), 
      m_health(0)
  {
    if (controllable) {
      listen("forcePosition");
    }

    setRadius(ns_radius);
    setOrigin(ns_radius / 2, ns_radius / 2);

    setOutlineThickness(1.f);

    if (faction == Faction::Simo) {
      setOutlineColor(sf::Color::Blue);
      setTexture(&ins.resourceManager.GetTexture("white.jpg"));
    }
    else if (faction == Faction::Russian) {
      setOutlineColor(sf::Color(255, 168, 0));
      setTexture(&ins.resourceManager.GetTexture("cheeki.png"));
    }
  }

  void Player::offsetHealth(const int health)
  {
    m_health += health;
  }

  void Player::setHealth(const int health)
  {
    m_health = health;
  }

  bool Player::isDead() const
  {
    return m_health <= 0;
  }

  void Player::update(const float dt)
  {
    if (m_controllable && !isDead()) {
      using sf::Keyboard;

      bool input = false;
      const float speed = 750.f * m_instance.config.float_("INTERPOLATION_TICK_LENGTH");
      glm::vec2 currentPos = getCurrentPos();
      glm::vec2 targetDirection(0.f);

      if (sf::Keyboard::isKeyPressed(Keyboard::A)) {
        targetDirection.x = -speed; input = true;
      }
      if (sf::Keyboard::isKeyPressed(Keyboard::D)) {
        targetDirection.x = speed; input = true;
      }
      if (sf::Keyboard::isKeyPressed(Keyboard::S)) {
        targetDirection.y = speed; input = true;
      }
      if (sf::Keyboard::isKeyPressed(Keyboard::W)) {
        targetDirection.y = -speed; input = true;
      }

      const auto nextPos = currentPos + targetDirection;
      updatePosition(nextPos);

      if (input) {
        const auto pos = getCurrentPos();
        rapidjson::Document doc(rapidjson::kObjectType);
        rapidjson::Value positionVector(rapidjson::kArrayType);
        positionVector.PushBack(pos.x, doc.GetAllocator());
        positionVector.PushBack(pos.y, doc.GetAllocator());

        doc.AddMember("position", positionVector, doc.GetAllocator());

        sendMessage("updateMovement", doc, false);
      }
    }

    InterpolatesTransform::update(dt);

    const auto currentPos = getCurrentPos();
    setPosition(sf::Vector2f(currentPos.x, currentPos.y));
  }

  void Player::draw(sf::RenderTarget& target)
  {
    target.draw(*this);
  }

  void Player::socketMessage(const char* message, const rapidjson::Value& data)
  {
    auto posUpdate = [this, &data](const bool force = false) {
      updatePosition(glm::vec2(
        static_cast<float>(rapidjson::GetValueByPointer(data, "/position/0")->GetDouble()),
        static_cast<float>(rapidjson::GetValueByPointer(data, "/position/1")->GetDouble())
      ), force);
    };

    if (strcmp(message, "forcePosition") == 0) {
      posUpdate(true);
    }

    if (m_controllable) return;

    if (strcmp(message, "updateMovement") == 0 && strcmp(getID().c_str(), data["id"].GetString()) == 0) {
      posUpdate();
    }
  }
}
