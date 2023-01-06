#pragma once

#include "RenderWindow.h"
#include "DeviceResources.h"
#include "Swapchain.h"

namespace Ekstra {
struct Engine {
  RenderWindow renderWindow{};
  DeviceResources deviceResources{};
  Swapchain swapchain{};
};

struct EngineController {
  static void StartUp(Engine &engine, HINSTANCE instance);
  static void ShutDown(const Engine &engine);
  static void Run(const Engine &engine);
};
}  // namespace Ekstra