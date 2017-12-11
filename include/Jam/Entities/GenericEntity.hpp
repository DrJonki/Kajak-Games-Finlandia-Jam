#pragma once

#include <Jam/Entity.hpp>
#include <SFML/Graphics/Drawable.hpp>

namespace jam
{
  template<typename T>
  class GenericEntity : public Entity, public T
  {
    static_assert(std::is_base_of<sf::Drawable, T>::value, "T must be derived from sf::Drawable");

  public:

    using T::T;

    void draw(sf::RenderTarget& target) override;
  };

  #include <Jam/Entities/GenericEntity.inl>
}