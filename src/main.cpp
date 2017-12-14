#include <iostream>
#include <Jam/Instance.hpp>
#include <Jam/Scenes/TitleScene.hpp>
#include <rapidjson/document.h>

int main() {
  jam::Instance instance;

  instance.currentScene = std::make_unique<jam::TitleScene>(instance);

  while (instance.window.isOpen()) {
    instance();
  }

  return 0;
}