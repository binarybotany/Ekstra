#include "DeviceResources.h"

namespace Ekstra {
void DeviceResourcesController::StartUp(DeviceResources &resources) {
  using Microsoft::WRL::ComPtr;

  D3D_FEATURE_LEVEL levels[] = {
      D3D_FEATURE_LEVEL_9_1,  D3D_FEATURE_LEVEL_9_2,  D3D_FEATURE_LEVEL_9_3,
      D3D_FEATURE_LEVEL_10_0, D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_11_0,
      D3D_FEATURE_LEVEL_11_1,
  };

  UINT deviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#if defined(DEBUG) || defined(_DEBUG)
  deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

  ComPtr<ID3D11Device> device;
  ComPtr<ID3D11DeviceContext> deviceContext;

  HRESULT result =
      D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, 0, deviceFlags,
                        levels, ARRAYSIZE(levels), D3D11_SDK_VERSION, &device,
                        &resources.featureLevel, &deviceContext);

  if (FAILED(result)) {
    // Handle device interface creation failure if it occurs.
    // For example, reduce the feature level requirement, or fail over
    // to WARP rendering.
    ThrowIfFailed(result);
  }

  device.As(&resources.device);
  deviceContext.As(&resources.deviceContext);
}

void DeviceResourcesController::ShutDown(const DeviceResources &resources) {}
}  // namespace Ekstra