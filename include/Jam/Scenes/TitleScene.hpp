#pragma once

#include <Jam/Scene.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

namespace jam
{
  class TitleScene : public Scene
  {
  public:

    TitleScene(Instance& ins);

    void update(const float dt) override;

    void draw(sf::RenderTarget& target) override;

  private:

    sf::View m_view;
    sf::RectangleShape m_background;
    sf::Text m_titleText[3];
  };
}