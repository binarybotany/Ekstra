#include <Windows.h>

#include "Engine.h"

using Ekstra::Engine;
using Ekstra::EngineController;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine,
                      _In_ int nCmdShow) {
  Engine engine{};
  EngineController::StartUp(engine, hInstance);
  EngineController::Run(engine);
  EngineController::ShutDown(engine);

  return 0;
}