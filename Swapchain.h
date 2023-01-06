#pragma once

#include <d3d11.h>
#include <dxgi1_3.h>
#include <wrl/client.h>

#include "DeviceResources.h"
#include "RenderWindow.h"

namespace Ekstra {
struct Swapchain {
  Microsoft::WRL::ComPtr<IDXGISwapChain> swapchain;
  Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
  Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTarget;
  D3D11_TEXTURE2D_DESC description;
};

struct SwapchainController {
  static void StartUp(Swapchain &swapchain, const RenderWindow &window,
                      DeviceResources &resources);
  static void ShutDown(const Swapchain &swapchain);
};
}  // namespace Ekstra
