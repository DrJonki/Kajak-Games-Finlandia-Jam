#include <Jam/PostProcessor.hpp>
#include <Jam/Instance.hpp>
#include <Jam/Scene.hpp>
#include <Jam/PostEffect.hpp>
#include <SFML/Graphics/Shader.hpp>

namespace jam
{
  PostProcessor::PostProcessor(Instance& ins)
  : m_instance(ins),
    m_effects(),
    m_fboRes(sf::Vector2f(ins.window.getSize()) * ins.config.float_("FBO_UPSCALE"))
  {
  for (std::size_t i = 0; i < 2; ++i) {
    auto& fbo = ins.framebuffer[i];
    fbo.create(static_cast<unsigned int>(m_fboRes.x), static_cast<unsigned int>(m_fboRes.y));
    fbo.setSmooth(true);
  }
  }


  PostProcessor::~PostProcessor()
  {}

  void PostProcessor::clearEffects()
  {
  m_effects.clear();
  }

  void PostProcessor::update(const float delta)
  {
  //for (auto& i : m_effects) {
  //  i->update(delta);
  //}
  }

  void PostProcessor::render(const float delta)
  {
  auto& framebuffer = m_instance.framebuffer;
  auto& window = m_instance.window;
  auto& currentScene = m_instance.currentScene;

  window.setView(window.getDefaultView());
  m_quad.setSize(sf::Vector2f(m_fboRes));

  if (currentScene) {
    framebuffer[0].clear();
    currentScene->draw(framebuffer[0]);
    framebuffer[0].display();

    bool zero = true;
    for (auto& i : m_effects) {
    i->update(delta);
    if (!i->isActive()) {
      continue;
    }

    zero = !zero;

    auto& writeFbo = framebuffer[!zero];
    auto& readFbo = framebuffer[zero];

    m_quad.setTexture(&readFbo.getTexture());

    writeFbo.setView(sf::View(m_fboRes * 0.5f, m_fboRes));
    writeFbo.clear();
    writeFbo.draw(m_quad, sf::RenderStates(&i->getShader()));
    writeFbo.display();
    }

    window.clear();
    window.setView(sf::View(m_fboRes * 0.5f, window.getDefaultView().getSize()));
    m_quad.setTexture(&framebuffer[!zero].getTexture());
    window.draw(m_quad);
  }

  }

}