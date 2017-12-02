#pragma once

#include <Jam/Entity.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

namespace jam
{
  class Obstacle : public Entity, public sf::RectangleShape
  {
  public:

    Obstacle();

  private:

    void draw(sf::RenderTarget& target) override;
  };
}
