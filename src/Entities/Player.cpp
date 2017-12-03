#include <Jam/Entities/Player.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <rapidjson/pointer.h>
#include <SFML/Graphics/RenderTarget.hpp>
#include <glm/vec2.hpp>

namespace jam
{
  Player::Player(Instance& ins, Scene& scene, const bool controllable)
    : Entity(),
      ListensMessages(scene, {"updateMovement"}),
      InterpolatesTransform(ins),
      sf::CircleShape(10.f),
      m_controllable(controllable)
  {
    if (controllable) {
      listen("forcePosition");
    }

    setFillColor(sf::Color::Blue);
  }

  void Player::update(const float dt)
  {
    if (m_controllable) {
      using sf::Keyboard;

      const float speed = 250.f;
      glm::vec2 currentPos = getCurrentPos();
      glm::vec2 targetDirection(0.f);

      if (sf::Keyboard::isKeyPressed(Keyboard::A)) {
        targetDirection.x = -speed;
      }
      if (sf::Keyboard::isKeyPressed(Keyboard::D)) {
        targetDirection.x = speed;
      }
      if (sf::Keyboard::isKeyPressed(Keyboard::S)) {
        targetDirection.y = speed;
      }
      if (sf::Keyboard::isKeyPressed(Keyboard::W)) {
        targetDirection.y = -speed;
      }

      const auto nextPos = currentPos + targetDirection;
      updatePosition(nextPos);

      rapidjson::Document doc;
      rapidjson::Value positionVector;
      positionVector.SetArray();
      positionVector.PushBack(nextPos.x, doc.GetAllocator());
      positionVector.PushBack(nextPos.y, doc.GetAllocator());

      doc.SetObject();
      doc.AddMember("position", positionVector, doc.GetAllocator());

      sendMessage("updateMovement", doc);
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
    if (m_controllable) return;

    if (strcmp(message, "updateMovement") == 0) {
      updatePosition(glm::vec2(
        static_cast<float>(rapidjson::GetValueByPointer(data, "/position/0")->GetDouble()),
        static_cast<float>(rapidjson::GetValueByPointer(data, "/position/1")->GetDouble())
      ));
    }
  }

}
