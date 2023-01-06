#pragma once

#include <Windows.h>

#include <string>

#include "ThrowIfFailed.h"

#ifndef UNICODE
#define UNICODE
#endif

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

namespace Ekstra {
struct RenderWindow {
  HINSTANCE instance;
  HWND windowHandle;
  RECT windowSize;
  WNDCLASS windowClass;

  std::wstring name;
};

struct RenderWindowController {
  static void StartUp(RenderWindow &window, HINSTANCE instance,
                      std::string name);
  static void ShutDown(const RenderWindow &window);
  static void MessageLoop(const RenderWindow &window);

 private:
  static HRESULT RegisterWindowClass(RenderWindow &window);
  static HRESULT CreateWindowHandle(RenderWindow &window);

  static LRESULT CALLBACK WindowProcedure(HWND window, UINT msg, WPARAM wparam,
                                          LPARAM lparam);
};
}  // namespace Ekstra
