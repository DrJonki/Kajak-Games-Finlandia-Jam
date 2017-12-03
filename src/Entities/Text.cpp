#include <Jam/Entities/Text.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace jam
{
  Text::Text()
    : Entity(),
      sf::Text()
  {
  }

  void Text::draw(sf::RenderTarget & target)
  {
    target.draw(*this);
  }
}
