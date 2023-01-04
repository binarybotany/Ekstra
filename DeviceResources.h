#pragma once

#include <Windows.h>
#include <d3d11.h>
#include <wrl/client.h>

#include "Error.h"

namespace Ekstra {
struct DeviceResources {
  D3D_FEATURE_LEVEL featureLevel;
  Microsoft::WRL::ComPtr<ID3D11Device> device;
  Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext;
};

struct DeviceResourcesController {
  static void StartUp(DeviceResources &resources);
  static void ShutDown(const DeviceResources &resources);
};
}  // namespace Ekstra
