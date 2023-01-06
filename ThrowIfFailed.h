#pragma once

#include <Windows.h>
#include <exception>
#include <cstdio>

namespace Ekstra {
class com_exception : public std::exception {
 public:
  com_exception(HRESULT hr) : result(hr) {}
  const char *what() const noexcept override {
    static char s_str[64] = {};
    sprintf_s(s_str, "Failure with HRESULT of %08X",
              static_cast<unsigned int>(result));
    return s_str;
  }

 private:
  HRESULT result;
};

inline void ThrowIfFailed(HRESULT hr) {
  if (FAILED(hr)) throw com_exception(hr);
}
}