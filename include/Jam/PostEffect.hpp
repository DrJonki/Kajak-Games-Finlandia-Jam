#pragma once

#include <SFML/Graphics/Shader.hpp>
#include <Jam/Instance.hpp>

namespace jam
{

  class PostEffect
  {
  protected:

  PostEffect(Instance& ins, const std::string& name, const std::string& vertex, const std::string& fragment);

  public:

  virtual ~PostEffect() = 0;

  virtual void update(const float delta);

  sf::Shader& getShader();
  const sf::Shader& getShader() const;

  const std::string& getName() const;

  void setActive(const bool active);

  bool isActive() const;

  Instance &getInstance();

  private:

  Instance &m_instance;
  sf::Shader& m_shader;
  const std::string m_name;
  bool m_active;
  };
}