#include <Jam/Entities/Storm.hpp>
#include <Jam/Scene.hpp>
#include <Jam/Instance.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/View.hpp>
#include <cmath>

namespace
{
  const size_t ns_flakes = 500;
  const size_t ns_stormFlakes = 2500;
  const float ns_flakeSize = 16;
}

namespace jam
{
  Storm::Storm(Scene & scene, const sf::View& view)
    : Entity(),
      ListensMessages(scene, {
        "stormBegin",
        "stormEnd",
      }),
      m_view(view),
      m_flakes(ns_stormFlakes),
      m_vertArray(sf::PrimitiveType::Quads),
      m_texture(scene.getInstance().resourceManager.GetTexture("flake.png")),
      m_sound(),
      m_rand(),
      m_numFlakes(static_cast<float>(ns_flakes)),
      m_storm(false)
  {
    m_sound.openFromFile("assets/Audio/effects/snow_storm.ogg");
    m_sound.setLoop(true);
    m_sound.setRelativeToListener(true);
    m_sound.play();

    setActive(false);

    for (auto& itr : m_flakes) {
      itr.scaleMult = m_rand.range(0.6f, 1.4f);
      itr.movementMult = m_rand.range(0.8f, 1.2f);

      itr.resetPos(view, m_rand);

      // Pre-warm
      for (size_t i = 0; i < 100; ++i) {
        itr.update(1.f / 30.f, view, m_rand, 1.f);
      }
    }
  }

  void Storm::update(const float dt)
  {
    const float rate = 200 * dt;

    m_numFlakes = m_storm ? std::min(static_cast<float>(ns_stormFlakes), m_numFlakes + rate) :
                            std::max(static_cast<float>(ns_flakes), m_numFlakes - rate);

    const float minVol = 8.f;
    const float maxVol = 75.f;
    m_sound.setVolume(windSpeed() * (maxVol - minVol) + minVol);

    {
      m_vertArray.clear();

      const auto s = ns_flakeSize;
      const auto ts = sf::Vector2f(m_texture.getSize());
      for (size_t i = 0; i < static_cast<size_t>(m_numFlakes); ++i) {
        auto& itr = m_flakes[i];

        itr.update(dt, m_view, m_rand, 1.f + windSpeed() * 5.f);

        const auto& t = itr.getTransform();

        m_vertArray.append(sf::Vertex(
          t.transformPoint(sf::Vector2f(-0.5f, -0.5f) * ns_flakeSize), sf::Vector2f(0.f, 0.f) // Top left
        ));
        m_vertArray.append(sf::Vertex(
          t.transformPoint(sf::Vector2f(-0.5f, 0.5f) * ns_flakeSize), sf::Vector2f(0.f, ts.y) // Bottom left
        ));
        m_vertArray.append(sf::Vertex(
          t.transformPoint(sf::Vector2f(0.5f, 0.5f) * ns_flakeSize), sf::Vector2f(ts.x, ts.y) // Bottom right
        ));
        m_vertArray.append(sf::Vertex(
          t.transformPoint(sf::Vector2f(0.5f, -0.5f) * ns_flakeSize), sf::Vector2f(ts.x, 0.f) // Top right
        ));
      }
    }
  }

  void Storm::draw(sf::RenderTarget & target)
  {
    sf::View view;
    view.setSize(m_view.getSize());
    view.setCenter(m_view.getCenter());

    target.setView(view);
    target.draw(m_vertArray, &m_texture);
  }

  void Storm::socketMessage(const char * event, const rapidjson::Value & data)
  {
    if (strcmp(event, "stormBegin")) {
      m_storm = true;
    }
    else {
      m_storm = false;
    }
  }

  bool Storm::isStorm() const
  {
    return m_storm;
  }

  float Storm::windSpeed() const
  {
    return (m_numFlakes - ns_flakes) / ns_stormFlakes;
  }

  Storm::Flake::Flake()
    : sf::Transformable()
  {
    // setOrigin(ns_flakeSize / 2, ns_flakeSize / 2);
  }

  inline void Storm::Flake::update(const float dt, const sf::View& view, const Randomizer& rand, const float windMult)
  {
    const sf::Vector2f wind(100.f, 25.f);

    currentPos += wind * dt * movementMult;
    setPosition(
      std::fmod(currentPos.x + view.getCenter().x, view.getSize().x),
      std::fmod(currentPos.y + view.getCenter().y, view.getSize().y)
    );

    const float scaleOffset = 1.f - 0.5f * dt * scaleMult;
    scale(scaleOffset, scaleOffset);

    if (isDone()) {
      setScale(1.f, 1.f);
      resetPos(view, rand);
    }
  }

  void Storm::Flake::resetPos(const sf::View& view, const Randomizer& rand)
  {
    currentPos = sf::Vector2f(
      rand.range(-view.getSize().x / 2, view.getSize().x / 2),
      rand.range(-view.getSize().y / 2, view.getSize().y / 2)
    );
  }

  inline bool Storm::Flake::isDone() const
  {
    return getScale().x <= 0.1f;
  }
}