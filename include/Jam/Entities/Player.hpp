#pragma once

#include <Jam/Entity.hpp>
#include <Jam/Entities/ListensMessages.hpp>
#include <Jam/Entities/InterpolatesTransform.hpp>

namespace jam
{
  class Player final : public Entity, public ListensMessages, public InterpolatesTransform
  {
  public:

    Player(Instance& ins, Scene& scene);

  private:

    void update(const float dt) override;

    void draw(sf::RenderTarget& target) override;
  };
}
