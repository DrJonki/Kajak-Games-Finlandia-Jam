#include <Jam/Layer.hpp>
#include <Jam/Entity.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace jam
{
  Layer::Layer()
  : m_entities(),
    m_sharedView(nullptr),
    m_view(),
    m_active(true)
  {

  }

  Layer::~Layer()
  {}

  void Layer::update(const float dt)
  {
  if (!isActive())
    return;

  for (auto itr = m_entities.begin(); itr != m_entities.end();) {
    if (itr->second->isErased()) {
    itr = m_entities.erase(itr);
    continue;
    }

    itr->second->baseUpdate(dt);
    ++itr;
  }
  }

  void Layer::draw(sf::RenderTarget& target)
  {
  if (!isActive())
    return;

  for (auto& i : m_entities) {
    target.setView(m_sharedView ? *m_sharedView : m_view);
    i.second->baseDraw(target);
  }
  }

  void Layer::setView(const sf::View& view)
  {
  m_view = view;
  }

  const sf::View& Layer::getView() const
  {
  return m_view;
  }

  void Layer::setSharedView(const sf::View* view)
  {
  m_sharedView = view;
  }

  const sf::View* Layer::getSharedView() const
  {
  return m_sharedView;
  }

  Entity* Layer::get(const std::string& name) const
  {
  auto itr = m_entities.find(name);
  if (itr != m_entities.end())
    return itr->second.get();

  return nullptr;
  }

  std::vector<Entity*> Layer::getAll(const std::string& name) const
  {
  auto range = m_entities.equal_range(name);
  std::vector<Entity*> entities;
  for (auto itr = range.first; itr != range.second; ++itr) {
    if (itr->second->isActive())
    entities.push_back(itr->second.get());
  }
  return entities;
  }

  std::vector<Entity*> Layer::getAll() const
  {
  std::vector<Entity*> entities;
  for (auto& i : m_entities) {
    if (i.second->isActive())
    entities.push_back(i.second.get());
  }
  return entities;
  }

  void Layer::setActive(const bool active)
  {
  m_active = active;
  }

  bool Layer::isActive() const
  {
  return m_active;
  }

}
