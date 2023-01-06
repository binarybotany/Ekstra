#pragma once

#include "pch.h"

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
  static void Fullscreen(const RenderWindow &window);

  static LRESULT CALLBACK WindowProcedure(HWND window, UINT msg, WPARAM wparam,
                                          LPARAM lparam);
};
}