#pragma once

#include <Jam/Entity.hpp>
#include <Jam/Entities/ListensMessages.hpp>
#include <Jam/Randomizer.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Audio/Music.hpp>
#include <vector>

namespace sf
{
  class View;
}

namespace jam
{
  class Storm : public Entity, public ListensMessages
  {
  private:

    struct Flake: sf::Transformable
    {
      sf::Vector2f currentPos;
      float movementMult;
      float scaleMult;

      Flake();

      void update(const float dt, const sf::View& view, const Randomizer& rand, const float windMult);

      void resetPos(const sf::View& view, const Randomizer& rand);

      bool isDone() const;
    };

  public:

    Storm(Scene& scene, const sf::View& view);

    void update(const float dt) override;

    void draw(sf::RenderTarget& target) override;

    void socketMessage(const char* event, const rapidjson::Value& data) override;

    bool isStorm() const;

  private:

    float windSpeed() const;

  private:

    const sf::View& m_view;
    std::vector<Flake> m_flakes;
    sf::VertexArray m_vertArray;
    sf::Texture& m_texture;
    sf::Music m_sound;
    Randomizer m_rand;
    float m_numFlakes;
    bool m_storm;
  };
}