#pragma once

#include <rapidjson/document.h>
#include <unordered_set>

namespace jam
{
  class Scene;

  class ListensMessages
  {
    friend class Scene;

  public:

    ListensMessages(Scene& scene, const std::initializer_list<const char*>& listeners);

    virtual ~ListensMessages() = 0;

  private:

    virtual void socketMessage(const char* message, const rapidjson::Value& data);

  protected:

    bool sendMessage(const char* message, const rapidjson::Value& data);

    void listen(const char* message);

    void stopListening(const char* message);

  private:

    Scene& m_scene;
    std::unordered_set<std::string> m_listening;
  };
}
