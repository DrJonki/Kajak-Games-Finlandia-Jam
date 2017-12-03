#include <Jam/Entities/Obstacle.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace jam
{
  Obstacle::Obstacle()
    : Entity(),
      sf::RectangleShape(sf::Vector2f(20, 20))
  {
    setFillColor(sf::Color::Black);
  }

  void Obstacle::draw(sf::RenderTarget& target)
  {
    target.draw(*this);
  }
}
