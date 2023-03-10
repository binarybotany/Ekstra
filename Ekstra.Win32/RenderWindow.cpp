#include "pch.h"
#include "RenderWindow.h"

namespace Ekstra {
void RenderWindowController::StartUp(RenderWindow &window, HINSTANCE instance,
                                     std::string name) {
  window.name = std::wstring(name.begin(), name.end());

  if (window.instance == NULL)
    window.instance = (HINSTANCE)GetModuleHandle(NULL);

  ThrowIfFailed(RegisterWindowClass(window));
  ThrowIfFailed(CreateWindowHandle(window));
  Fullscreen(window);

  ShowWindow(window.windowHandle, SW_SHOWDEFAULT);
  UpdateWindow(window.windowHandle);
}

void RenderWindowController::ShutDown(const RenderWindow &window) {
  DestroyWindow(window.windowHandle);
  UnregisterClass(window.windowClass.lpszClassName, window.instance);
}

void RenderWindowController::MessageLoop(const RenderWindow &window) {
  bool receivedMessage = false;
  MSG msg;
  msg.message = WM_NULL;
  PeekMessage(&msg, NULL, 0U, 0U, PM_NOREMOVE);

  while (msg.message != WM_QUIT) {
    receivedMessage = (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE) != 0);

    if (receivedMessage) {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    } else {
      /* renderer.Update(); renderder.Render(); */
    }
  }
}

HRESULT RenderWindowController::RegisterWindowClass(RenderWindow &window) {
  WNDCLASS windowClass;
  ZeroMemory(&windowClass, sizeof(WNDCLASS));

  windowClass.cbClsExtra = NULL;
  windowClass.cbWndExtra = NULL;
  windowClass.hbrBackground = NULL;
  windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
  windowClass.hIcon = NULL;
  windowClass.lpfnWndProc = (WNDPROC)WindowProcedure;
  windowClass.lpszClassName = window.name.c_str();
  windowClass.lpszMenuName = NULL;
  windowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;

  if (!RegisterClass(&windowClass)) {
    HRESULT error = GetLastError();
    if (error != ERROR_CLASS_ALREADY_EXISTS) return HRESULT_FROM_WIN32(error);
  }

  window.windowClass = windowClass;

  return S_OK;
}

HRESULT RenderWindowController::CreateWindowHandle(RenderWindow &window) {
  ZeroMemory(&window.windowSize, sizeof(RECT));

  int defaultWidth = 1024;
  int defaultHeight = 768;

  SetRect(&window.windowSize, 0, 0, defaultWidth, defaultHeight);
  AdjustWindowRect(&window.windowSize, WS_OVERLAPPEDWINDOW, false);

  HWND windowHandle = CreateWindow(
      window.windowClass.lpszClassName, window.name.c_str(),
      WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, CW_USEDEFAULT,
      (window.windowSize.right - window.windowSize.left),
      (window.windowSize.bottom - window.windowSize.top), NULL, NULL,
      window.instance, nullptr);

  if (!windowHandle) {
    HRESULT error = GetLastError();
    return HRESULT_FROM_WIN32(error);
  }

  window.windowHandle = windowHandle;

  return S_OK;
}

void RenderWindowController::Fullscreen(const RenderWindow &window) {
  // Might need to do something with the windowSize RECT here
  DWORD style = GetWindowLongW(window.windowHandle, GWL_STYLE);
  MONITORINFO monitor_info = {sizeof(MONITORINFO)};
  GetMonitorInfoW(
      MonitorFromWindow(window.windowHandle, MONITOR_DEFAULTTOPRIMARY),
      &monitor_info);

  SetWindowLongW(window.windowHandle, GWL_STYLE, style & ~WS_OVERLAPPEDWINDOW);

  SetWindowPos(window.windowHandle, HWND_TOP, monitor_info.rcMonitor.left,
               monitor_info.rcMonitor.top,
               monitor_info.rcMonitor.right - monitor_info.rcMonitor.left,
               monitor_info.rcMonitor.bottom - monitor_info.rcMonitor.top,
               SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
}

LRESULT CALLBACK RenderWindowController::WindowProcedure(HWND window, UINT msg,
                                                         WPARAM wparam,
                                                         LPARAM lparam) {
  switch (msg) {
    case WM_KEYUP:
      if (wparam == VK_ESCAPE) PostQuitMessage(0);
      return 0;
    case WM_CLOSE:
    case WM_DESTROY:
      PostQuitMessage(0);
      return 0;
    default:
      return DefWindowProc(window, msg, wparam, lparam);
  }
}
}  // namespace Ekstra