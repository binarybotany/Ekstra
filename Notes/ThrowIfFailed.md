# ThrowIfFailed

When programming COM APIs like Direct3D, it is important to always check the 
`HRESULT` return value for success or failure. This can be done using the
`SUCCEEDED` or `FAILED` macros, but can get tedious when making lots of calls
especially for proper cleanup on exit of every function.

```cpp
hr = device->CreateTexture2D(&depthStencilDesc, nullptr, &depthStencil)):
if (FAILED(hr)) {
  // Clean up for partial success before here
  return hr; // Must keep passing the error code back all the way to the main 
             // loop
}
```

The `DX::ThrowIfFailed` helper can be used whenever a failure is fatal and 
should result is a "fast-fail" of the application.

```cpp
DX::ThrowIfFailed(device->CreateTexture2D(&depthStencilDesc, nullptr, 
                                          &depthStencil));
```

## Runtime error handling

Otherwise traditional `if(FAILED(hr))` and `if(SUCCEEDED(hr))` patterns should 
be used to handle failures that the application can recover from, that are not 
fatal. If you want to handle a specific `HRESULT`, then you might do something 
like this: 

```cpp
HRESULT hr = device->CreateTexture2D(&depthStencilDesc, nullptr, 
                                     &depthStencil));
if (hr == E_INVALIDARG ) {
  // Do something here in response to this specific error
}
DX::ThrowIfFailed(hr);
```

For a case where you want to error handling for an `HRESULT` yourself, be sure 
to use the `SUCCEEDED` or `FAILED` macro: 

```cpp
HRESULT hr = device->CreateTexture2D(&depthStencilDesc, nullptr, 
                                     &depthStencil));
if (FAILED(hr)) { /* Error handling */ }
```

## Basic version

The `ThrowIfFailed` helper is not part of the _DirectX Tool Kit_, it's declared
in some global header in your application. You typically find it in the `pch.h`.

```cpp
#include <exception>

namespace DX {
  inline void ThrowIfFailed(HRESULT hr) {
    if (FAILED(hr)) {
      // Set a breakpoint on this line to catch DirectX API errors
      throw std::exception();
    }
  }
}
```

## Enhancements

The templates all include the basic implementation above, but production use 
might want to utilize a slightly improved version.

```cpp
#include <exception>

namespace DX {
  class com_exception : public std::exception {
   public:
    com_exception(HRESULT hr) : result(hr) {}
      const char *what() const noexcept override {
      const char s_str[64] = {};
      sprintf_s(s_str, "Failure with HRESULT of %08X", 
                static_cast<unsigned int>(result));
      retrun s_str;
    }
   private:
    HRESULT result;
  };

  inline void ThrowIfFailed(HRESULT hr) {
    if (FAILED(hr)) throw com_exception(hr);
  }
}
```