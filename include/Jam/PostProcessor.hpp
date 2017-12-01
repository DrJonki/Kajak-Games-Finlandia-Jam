#pragma once

#include <SFML/Graphics/RectangleShape.hpp>
#include <memory>
#include <vector>

namespace sf
{
  class Shader;
}

namespace jam
{
  class PostEffect;
  class Instance;

  class PostProcessor
  {
  public:

    PostProcessor(Instance& ins);

    ~PostProcessor();

    template<typename T, typename ... Args>
    T& createEffect(const std::string& name, Args&&... args);

    void clearEffects();

    void update(const float delta);

    void render(const float delta);

  private:

    const sf::Vector2f m_fboRes;
    Instance& m_instance;
    sf::RectangleShape m_quad;
    std::vector<std::unique_ptr<PostEffect>> m_effects;
  };

  #include <Jam/PostProcessor.inl>
}