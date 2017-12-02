#include <Jam/Entities/ListensMessages.hpp>
#include <Jam/Scene.hpp>

namespace jam
{
  ListensMessages::ListensMessages(Scene& scene, const std::initializer_list<const char*>& listeners)
    : m_scene(scene),
      m_listening(listeners.begin(), listeners.end())
  {
    for (auto& it : m_listening) {
      scene.addListener(it, *this);
    }
  }

  ListensMessages::~ListensMessages()
  {
    for (auto& it : m_listening) {
      m_scene.removeListener(it, *this);
    }
  }

  void ListensMessages::socketMessage(const char* message, const rapidjson::Value& data)
  {}
}
