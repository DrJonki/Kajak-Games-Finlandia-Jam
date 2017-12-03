#include <Jam/Scenes/TitleScene.hpp>
#include <Jam/Instance.hpp>

namespace jam
{
  TitleScene::TitleScene(Instance& ins)
    : Scene(ins),
      m_view(sf::Vector2f(0.5f, 0.5f), sf::Vector2f(1.f, 1.f)),
      m_background(sf::Vector2f(1.f, 1.f)),
      m_titleText()
  {
    m_background.setTexture(&ins.resourceManager.GetTexture("titlebg.jpg"));

    m_titleText[0].setFont(ins.resourceManager.GetFont("AmazDooMLeft.ttf"));
    m_titleText[0].setString("Si");
    m_titleText[1].setFont(ins.resourceManager.GetFont("AmazDooMRight.ttf"));
    m_titleText[1].setString("mO");
    m_titleText[2].setFont(ins.resourceManager.GetFont("AmazDooMLeft.ttf"));
    m_titleText[2].setString("karjala takas");
    for (int i = 0; i < 3; ++i) {
      auto& t = m_titleText[i];
      t.setCharacterSize(128);
      t.setFillColor(sf::Color(33, 33, 33));
      t.setOutlineColor(sf::Color(255, 165, 0));
      t.setOutlineThickness(3.f);

      const float scale = 0.00175f;
      t.setScale(scale, scale * 1.2f);
      t.setPosition(0.05f + 0.12f * i, 0.05f);
    }
    m_titleText[2].setOutlineThickness(3);
    m_titleText[2].setPosition(0.06f, 0.3f);
    m_titleText[2].setScale(0.0005f, 0.0005f);


  }

  void TitleScene::update(const float dt)
  {
    Scene::update(dt);
  }

  void TitleScene::draw(sf::RenderTarget & target)
  {
    Scene::draw(target);

    target.setView(m_view);

    target.draw(m_background);

    for (int i = 0; i < sizeof(m_titleText) / sizeof(m_titleText[0]); ++i) {
      target.draw(m_titleText[i]);
    }
  }
}
