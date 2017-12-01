#include <iostream>
#include <Jam/Instance.hpp>
#include <Jam/Scenes/GameScene.hpp>
#include <rapidjson/document.h>

int main() {
  jam::Instance instance;

  instance.currentScene = std::make_unique<jam::GameScene>(instance);

  while (instance.window.isOpen()) {
    instance();
  }

  return 0;
}