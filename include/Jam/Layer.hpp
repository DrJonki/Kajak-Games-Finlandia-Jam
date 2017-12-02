#pragma once

#include <memory>
#include <vector>
#include <unordered_map>
#include <Jam/Entity.hpp>
#include <SFML/Graphics/View.hpp>
#include <Jam/Entities/ListensMessages.hpp>

namespace sf
{
  class RenderTarget;
}

namespace jam
{
  class Layer final
  {
  public:

    Layer();

    ~Layer();

    template<typename T, typename ... Args>
    T& insert(const std::string& name, Args&&... args);

    Entity* get(const std::string& name) const;

    std::vector<Entity*> getAll(const std::string& name) const;

    std::vector<Entity*> getAll() const;

    void update(const float dt);

    void draw(sf::RenderTarget& target);

    void setView(const sf::View& view);

    const sf::View& getView() const;

    void setSharedView(const sf::View* view);

    const sf::View* getSharedView() const;

    void setActive(const bool active);

    bool isActive() const;

  private:

    std::unordered_multimap<std::string, std::unique_ptr<Entity>> m_entities;
    const sf::View* m_sharedView;
    sf::View m_view;
    bool m_active;
  };

  #include <Jam/Layer.inl>
}

