#pragma once

#include <Windows.h>
#include <comdef.h>

namespace Ekstra {
struct Error {
  static void Display(HRESULT result, LPCWSTR message);
};
}  // namespace Ekstra