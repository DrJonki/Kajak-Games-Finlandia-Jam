#include <Jam/PostEffect.hpp>
#include <Jam/Instance.hpp>

namespace jam
{
  PostEffect::PostEffect(Instance& ins, const std::string& name, const std::string& vertex, const std::string& fragment)
  : m_instance(ins),
    m_shader(ins.resourceManager.GetShader(vertex, fragment)),
    m_name(name),
    m_active(true)
  {
  getShader().setUniform("texture", sf::Shader::CurrentTexture);
  }

  PostEffect::~PostEffect()
  {

  }

  void PostEffect::update(const float delta)
  {}

  sf::Shader& PostEffect::getShader()
  {
  return m_shader;
  }

  const sf::Shader& PostEffect::getShader() const
  {
  return m_shader;
  }

  const std::string& PostEffect::getName() const
  {
  return m_name;
  }

  void PostEffect::setActive(const bool active)
  {
  m_active = active;
  }

  bool PostEffect::isActive() const
  {
  return m_active;
  }

  Instance &PostEffect::getInstance()
  {
  return m_instance;
  }
}