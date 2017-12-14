#include <Jam/Entities/Player.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <rapidjson/pointer.h>
#include <Jam/Instance.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <glm/vec2.hpp>
#include <glm/geometric.hpp>
#include <glm/gtx/vectorAngle.hpp>
#include <SFML/Window/Mouse.hpp>
#include <iostream>

namespace {
  const float ns_radius = 20.f;
}

namespace jam
{
  Player::Player(Instance& ins, Scene& scene, const bool controllable, const Faction faction, const sf::View& view)
    : Entity(),
      ListensMessages(scene, {"updateMovement"}),
      InterpolatesTransform(ins),
      sf::CircleShape(10.f),
      m_instance(ins),
      m_controllable(controllable),
      m_faction(faction), 
      m_health(100),
    m_speedVec(glm::vec2(0,0)),
    m_accelVec(glm::vec2(0,0)),
    m_friction(0.07f),
    m_accelFloat(1.0f),
    m_max_speedFloat(10),
    m_view(view),
    m_targetDirection(0.f),
    m_velocity(0.f),
    m_rectangles()
  {
    if (controllable) {
      listen("forcePosition");
    for (int i = 0; i < 4; i++) {
      m_rectangles[i].setFillColor(sf::Color::Red);
    }
    }
  m_bang_sound.setBuffer(ins.resourceManager.GetSoundBuffer("effects/bolt_shot_reload.wav"));
  m_bang_sound.setRelativeToListener(controllable);
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

  void Player::shoot()
  {
    m_bang_sound.play();
  }

  void Player::update(const float dt)
  {
    if (m_controllable && !isDead()) {
      using sf::Keyboard;
      auto sfMousePos = m_instance.window.mapPixelToCoords( sf::Mouse::getPosition(m_instance.window), m_view);
      glm::vec2 mousePos = glm::vec2(sfMousePos.x, sfMousePos.y);


      bool input = false;
      const float speed = 750.f * m_instance.config.float_("INTERPOLATION_TICK_LENGTH");
      glm::vec2 currentPos = getCurrentPos();
    glm::vec2 m_lookDir = glm::normalize(mousePos - currentPos);

    m_accelVec = glm::vec2(0.f);
      if (sf::Keyboard::isKeyPressed(Keyboard::W)) {
      m_accelVec += m_accelFloat * m_lookDir;
      input = true;
      }
      if (sf::Keyboard::isKeyPressed(Keyboard::S)) {
      m_accelVec += m_accelFloat * glm::vec2(-m_lookDir.x, -m_lookDir.y);
      input = true;
      }
      if (sf::Keyboard::isKeyPressed(Keyboard::A)) {
      m_accelVec += m_accelFloat * glm::vec2(m_lookDir.y, -m_lookDir.x);
      input = true;
      }
      if (sf::Keyboard::isKeyPressed(Keyboard::D)) {
      m_accelVec += m_accelFloat * glm::vec2(-m_lookDir.y, m_lookDir.x);
      input = true;
      }
    std::cout << m_accelVec.x << " , " << m_accelVec.y << std::endl;

    if (glm::length(m_accelVec) != 0.f) {
      m_targetDirection += m_accelFloat * glm::normalize(m_accelVec);
      if (glm::length(m_targetDirection) > m_max_speedFloat) {
        m_targetDirection = m_max_speedFloat * glm::normalize(m_targetDirection);
      }
    }
    m_velocity = glm::length(m_targetDirection);
    if(!input) {
      if (m_velocity > m_friction ) {
        m_targetDirection -= m_friction * m_targetDirection;
      } else {
        m_targetDirection = glm::vec2(0, 0);
      }
    }

    float rectThickness = 4.f;
    float rectSize = 12.f;
    float rectGap = 10.f;
    float maxInaccuracy = 16;
    float inAccuracy = maxInaccuracy * m_velocity / m_max_speedFloat;
   
      const auto nextPos = currentPos + m_targetDirection;
      updatePosition(nextPos);

    m_rectangles[0].setFillColor(sf::Color::Red);
    m_rectangles[0].setSize(sf::Vector2f(rectSize, rectThickness));
    m_rectangles[0].setPosition(sf::Vector2f(mousePos.x + inAccuracy + rectGap / 2, mousePos.y - rectThickness / 2));

    m_rectangles[1].setFillColor(sf::Color::Red);
    m_rectangles[1].setSize(sf::Vector2f(rectSize, rectThickness));
    m_rectangles[1].setPosition(sf::Vector2f(mousePos.x - rectGap / 2 - inAccuracy - rectSize, mousePos.y - rectThickness / 2));

    m_rectangles[2].setFillColor(sf::Color::Red);
    m_rectangles[2].setSize(sf::Vector2f(rectThickness, rectSize));
    m_rectangles[2].setPosition(sf::Vector2f(mousePos.x - rectThickness / 2, mousePos.y + rectGap / 2 + inAccuracy));

    m_rectangles[3].setFillColor(sf::Color::Red);
    m_rectangles[3].setSize(sf::Vector2f(rectThickness, rectSize));
    m_rectangles[3].setPosition(sf::Vector2f(mousePos.x - rectThickness / 2, mousePos.y - inAccuracy - rectGap / 2 - rectSize));

      if (glm::length(m_targetDirection) > 0) {
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
  if (m_controllable) {
    for (int i = 0; i < 4; i++) {
      target.draw(m_rectangles[i]);
    }
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

    if (m_controllable) return;

    if (strcmp(message, "updateMovement") == 0 && strcmp(getID().c_str(), data["id"].GetString()) == 0) {
      posUpdate();
    }
  }
}
