#pragma once

#include <d3d11.h>
#include <dxgi1_3.h>
#include <wrl/client.h>

#include "DeviceResources.h"
#include "Error.h"
#include "RenderWindow.h"

namespace Ekstra {
struct Swapchain {
  Microsoft::WRL::ComPtr<IDXGIDevice3> dxgiDevice;
  Microsoft::WRL::ComPtr<IDXGISwapChain> swapchain;
};

struct SwapchainController {
  static void StartUp(Swapchain &swapchain, const RenderWindow &window,
                      DeviceResources &resources);
  static void ShutDown(const Swapchain &swapchain);
};
}  // namespace Ekstra
