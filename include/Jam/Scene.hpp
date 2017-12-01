#pragma once

#include <memory>
#include <map>
#include <SFML/Graphics/View.hpp>

namespace sf
{
  class Drawable;
  class RenderTarget;
}

namespace jam
{
  class Layer;
  class Instance;

  class Scene
  {
  public:

    Scene(Instance& ins);

    virtual ~Scene() = 0;

    Layer& addLayer(const uint32_t weight);

    virtual void update(const float dt);

    virtual void draw(sf::RenderTarget& target);

    virtual void postDraw(sf::RenderTarget& target);

    Instance& getInstance();

    const Instance& getInstance() const;

    void setView(const sf::View& view);

    const sf::View& getView() const;

    virtual void textEvent(const uint32_t code);

  private:

    Instance& m_instance;
    std::multimap<uint32_t, std::unique_ptr<Layer>> m_layers;
    sf::View m_view;
  };
}