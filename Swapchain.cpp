#include "Swapchain.h"

namespace Ekstra {
void SwapchainController::StartUp(Swapchain &swapchain,
                                  const RenderWindow &window,
                                  DeviceResources &resources) {
  using Microsoft::WRL::ComPtr;

  DXGI_SWAP_CHAIN_DESC swapchainDescription;
  ZeroMemory(&swapchainDescription, sizeof(DXGI_SWAP_CHAIN_DESC));
  swapchainDescription.Windowed = TRUE;
  swapchainDescription.BufferCount = 2;
  swapchainDescription.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
  swapchainDescription.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  swapchainDescription.SampleDesc.Count = 1;
  swapchainDescription.SampleDesc.Quality = 0;
  swapchainDescription.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
  swapchainDescription.OutputWindow = window.windowHandle;

  ComPtr<IDXGIDevice3> dxgiDevice;
  resources.device.As(&dxgiDevice);

  ComPtr<IDXGIAdapter> adapter;
  ComPtr<IDXGIFactory> factory;

  HRESULT result = dxgiDevice->GetAdapter(&adapter);

  if (SUCCEEDED(result)) {
    adapter->GetParent(IID_PPV_ARGS(&factory));
    result = factory->CreateSwapChain(
        resources.device.Get(), &swapchainDescription, &swapchain.swapchain);
  }

  ThrowIfFailed(result);

  // Set render target
  result = swapchain.swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D),
                                          (void **)&swapchain.backBuffer);

  ThrowIfFailed(result);

  result = resources.device->CreateRenderTargetView(
      swapchain.backBuffer.Get(), nullptr,
      swapchain.renderTarget.GetAddressOf());

  ThrowIfFailed(result);

  swapchain.backBuffer->GetDesc(&swapchain.description);
}

void SwapchainController::ShutDown(const Swapchain &swapchain) {}
}  // namespace Ekstra