#include <Jam/Entities/Player.hpp>

namespace jam
{
  Player::Player(Instance& ins, Scene& scene)
    : Entity(),
      ListensMessages(scene, {}),
      InterpolatesTransform(ins)
  {

  }

  void Player::update(const float dt)
  {
    


  }

  void Player::draw(sf::RenderTarget& target)
  {
    throw std::logic_error("The method or operation is not implemented.");
  }

}
