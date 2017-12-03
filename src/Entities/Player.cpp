#include <Jam/Entities/Player.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <rapidjson/pointer.h>
#include <SFML/Graphics/RenderTarget.hpp>
#include <glm/vec2.hpp>

namespace {
  const float ns_radius = 10.f;
}

namespace jam
{
  Player::Player(Instance& ins, Scene& scene, const bool controllable, const Faction faction)
    : Entity(),
      ListensMessages(scene, {"updateMovement", "dead", "respawn"}),
      InterpolatesTransform(ins),
      sf::CircleShape(10.f),
      m_controllable(controllable),
      m_faction(faction), 
      m_dead(false)
  {
    if (controllable) {
      listen("forcePosition");
    }

    setRadius(ns_radius);
    setOrigin(ns_radius / 2, ns_radius / 2);

    setOutlineThickness(1.f);

    if (faction == Faction::Simo) {
      setFillColor(sf::Color::Blue);
      setOutlineColor(sf::Color::White);
    }
    else if (faction == Faction::Russian) {
      setFillColor(sf::Color::Red);
      setOutlineColor(sf::Color(255, 168, 0));
    }
  }

  bool Player::isDead() const
  {
    return m_dead;
  }

  std::string Player::killedBy() const
  {
    return std::string();
  }

  void Player::update(const float dt)
  {
    if (m_controllable && !isDead()) {
      using sf::Keyboard;

      bool input = false;
      const float baseSpeed = 250.f;
      const float speed = m_faction == Faction::Simo ? baseSpeed : baseSpeed / 4;
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
        rapidjson::Document doc;
        rapidjson::Value positionVector;
        positionVector.SetArray();
        positionVector.PushBack(pos.x, doc.GetAllocator());
        positionVector.PushBack(pos.y, doc.GetAllocator());

        doc.SetObject();
        doc.AddMember("position", positionVector, doc.GetAllocator());

        sendMessage("updateMovement", doc);
      }
    }

    InterpolatesTransform::update(dt);

    const auto currentPos = getCurrentPos();
    setPosition(sf::Vector2f(currentPos.x, currentPos.y));
  }

  void Player::draw(sf::RenderTarget& target)
  {
    if (!isDead()) {
      target.draw(*this);
    }
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

    if (strcmp(message, "dead") == 0) {
      m_dead = true;
    }

    if (strcmp(message, "respawn") == 0) {
      posUpdate();
      m_dead = false;
    }

    if (m_controllable) return;

    if (strcmp(message, "updateMovement") == 0 && strcmp(getID().c_str(), data["id"].GetString()) == 0) {
      posUpdate();
    }
  }
}
