#pragma once

#include <Windows.h>

#include <memory>

#include "DeviceResources.h"
#include "RenderWindow.h"
#include "Swapchain.h"
#include "ThrowIfFailed.h"

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