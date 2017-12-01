#include <Jam/Scene.hpp>
#include <Jam/Layer.hpp>

namespace jam
{
  Scene::Scene(Instance& ins)
    : m_instance(ins),
      m_layers()
  {}

  Scene::~Scene()
  {}

  Layer& Scene::addLayer(const uint32_t weight)
  {
    auto uniq = std::make_unique<Layer>();
    auto& ref = *uniq;
    m_layers.emplace(weight, std::move(uniq));

    return ref;
  }

  void Scene::update(const float dt)
  {
    for (auto& i : m_layers) {
      i.second->update(dt);
    }
  }

  void Scene::draw(sf::RenderTarget& target)
  {
    for (auto& i : m_layers) {
      i.second->draw(target);
    }
  }


  void Scene::postDraw(sf::RenderTarget& target)
  {}

  Instance& Scene::getInstance()
  {
    return m_instance;
  }


  const Instance& Scene::getInstance() const
  {
    return m_instance;
  }

  void Scene::setView(const sf::View& view)
  {
    m_view = view;
  }

  const sf::View& Scene::getView() const
  {
    return m_view;
  }

  void Scene::textEvent(const uint32_t code)
  {}
}