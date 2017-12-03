#pragma once

#include <Jam/Entity.hpp>
#include <SFML/Graphics/Text.hpp>

namespace jam
{
  class Text : public Entity, public sf::Text
  {
  public:

    Text();

    void draw(sf::RenderTarget& target) override;
  };


}