#include "Engine.h"

namespace Ekstra {
void EngineController::StartUp(Engine &engine, HINSTANCE instance) {
  RenderWindowController::StartUp(engine.renderWindow, instance, "Ekstra");
  DeviceResourcesController::StartUp(engine.deviceResources);
  SwapchainController::StartUp(engine.swapchain, engine.renderWindow,
                               engine.deviceResources);
}
void EngineController::ShutDown(const Engine &engine) {
  SwapchainController::ShutDown(engine.swapchain);
  DeviceResourcesController::ShutDown(engine.deviceResources);
  RenderWindowController::ShutDown(engine.renderWindow);
}

void EngineController::Run(const Engine &engine) {
  RenderWindowController::MessageLoop(engine.renderWindow);
}
}  // namespace Ekstra