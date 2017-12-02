#pragma once

#include <memory>
#include <map>
#include <SFML/Graphics/View.hpp>
#include <rapidjson/document.h>

namespace sf
{
  class Drawable;
  class RenderTarget;
}

namespace jam
{
  class Layer;
  class Instance;
  class ListensMessages;

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

    void addListener(const std::string& message, ListensMessages& entity);

    void removeListener(const std::string& message, ListensMessages& entity);

    virtual void textEvent(const uint32_t code);

    virtual void socketEvent(const char* event, const rapidjson::Value& data);

  private:

    Instance& m_instance;
    std::multimap<std::string, ListensMessages*> m_listeners;
    std::multimap<uint32_t, std::unique_ptr<Layer>> m_layers;
    sf::View m_view;
  };
}