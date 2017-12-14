#include <Jam/Entities/ListensMessages.hpp>
#include <Jam/Instance.hpp>
#include <Jam/Scene.hpp>

namespace jam
{
  ListensMessages::ListensMessages(Scene& scene, const std::initializer_list<const std::string>& listeners)
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

  bool ListensMessages::sendMessage(const char* message, rapidjson::Value& data, const bool tcp)
  {
    return m_scene.getInstance().sendMessage(message, data, tcp);
  }

  void ListensMessages::listen(const std::string& message)
  {
    m_listening.insert(message);
  }

  void ListensMessages::stopListening(const std::string& message)
  {
    m_listening.erase(message);
  }
}
