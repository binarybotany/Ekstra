# ComPtr 

`Microsoft::WRL::ComPtr` is a C++ template smart-pointer for COM objects that is
used extensively in WinRT C++ programming. It is used a lot by the DirectXTK to
properly handle COM reference counting maintenance. ComPtr can be included with
the header file `<wrl/client.h>`. 

## Obtaining a raw pointer from a ComPtr

Use `Get()`

```cpp
ComPtr<ID3D11RasterizerState> rasterizerState;
device->RSSetState(rasterizerState.Get());
```

A number of Direct3D APIs actually take an array of pointers to COM objects 
because they can be passed one ore more objects at once. When you just have one 
object to pass, use `GetAddressOf()` to a function parameter that is `**`. 

`operator&` maps to `ReleaseAndGetAddressOf()`, therefore use `GetAddressOf()`
when calling Direct3D functions that take a pointer to an array of inputs and 
you're passing a single pointer.

```cpp
context->OMSetRenderTargets(
	1, 
	renderTargetView.GetAddressOf(), 
	depthStencilView.Get());
// or
auto rt = renderTargetView.Get();
context->OMSetRenderTargets(1, &rt, depthStencilView.Get());
```

If you would use `&renderTargetView`, the `renderTargetView` variable would get
released before the call to `OMSetRenderTargets` and the result would likely 
crash.

If you're building an array of Direct3D interface pointers, you should build one
as a local variable as there is no way to convert an array of `ComPtr<T>` to an
array of `T*`.

```cpp
ID3D11SamplerState *samplers[] = { sampler1.Get(), sampler2.Get() };
context->PSSetSamplers(0, static_cast<UINT>(std::size(samplers)), samplers);
```

## Initialization

Generally, if you're creating a fresh ComPtr and then use a COM creation 
function or factory, you can use `GetAddressOf` since you know the ComPtr is 
initially `null`.

```cpp
ComPtr<ID3D11Device> device;

hr = D3D11CreateDevice(..., device.GetAddressOf(), ...);
// or 
hr = D3D11CreateDevice(..., &device, ...);
```

If you are using ComPtr as a class member, then you should use 
`ReleaseAndGetAddressOf` to ensure existing object reference is properly 
released. 

```cpp
ComPtr<ID3D11Device> m_device;

hr = D3D11CreateDevice(..., m_device.ReleaseAndGetAddressOf(), ...);
// or 
hr = D3D11CreateDevice(..., &m_device, ...);
```

## Parameters

When passing ComPtr variables by value, this will result in incrementing/ 
decrementing the reference count. You can avoid this by either passing the 
smart-pointer by `const &` or passing a raw pointer instead. Raw pointers are 
generally the more flexible option, but require the use of `Get` at the call 
site.

```cpp
void func(const ComPtr<T> &param);
void func(T* param);
```

When passing ComPtr variables to constructors, you usually want the new class to
increase the reference count, which is handled automatically by assigning the 
raw pointer to a new `ComPtr<T>` or by copying one `ComPtr<T>` to another.

## Obtaining new interfaces

ComPtr provides a much simpler syntax for doing `QueryInterface` calls on COM
objects. `CopyTo` performs a similar operation to `As`.

```cpp
ComPtr<ID3D11Device> device;

hr = D3D11CreateDevice(..., device.GetAddressOf(), ...);
if (SUCCEEDED(hr)) { /* device created */ }

ComPtr<ID3D11Device1> device1;
hr = device.As(&device1);
if (SUCCEEDED(hr)) { 
  // Was able to obtain the ID3D11Device1 interface
  device->...
}

ComPtr<ID3D11Debug> d3dDebug;
hr = device.As(&d3dDebug);
if (SUCCEEDED(hr)) { /* Was able to obtain the ID3D11Debug interface */ }
```

## Clearing a ComPtr

Releasing a reference from a particular ComPtr can be done using `Reset`, which 
sets the ComPtr to `nullptr` releasing any previous reference.

```cpp
ComPtr<ID3D11Debug> d3dDebug;
d3dDebug.Reset();
```

## Checking for null

There is an `operator bool` overload to let you easily check if a ComPtr is 
currently set to null, which is often useful in assert statements.

```cpp
ComPtr<ID3D11Debug> d3dDebug;
if (!d3dDebug) { /* d3dDebug is nullptr */ }
```

## Constructing, assigning and copying ComPtr

If you set a ComPtr to a raw pointer or another ComPtr via the copy constructor,
or the assignment operator, the behavior is to increase the reference count. It 
is assuming the original raw pointer or ComPtr will still be calling `Release`,
and the new copy will also be calling `Release`.

If you want to give a raw pointer to a ComPtr without changing the reference 
count, you need to use `Attach`. If you want to give a pointer in one ComPtr to	
another ComPtr without changing the reference count, in other words, you want to
transfer ownership, you need to `Detach` from the old ComPtr followed by an 
`Attach` to the new one. Alternatively you can use `Swap` to achieve the same 
thing when one of the ComPtr variables is already null.

ComPtr has a move constructor and a move assignment operator, that both 
effectively perform a `Swap`.

## IUnknown methods

ComPtr goes through some lengths to hide the base `IUnknown` methods `AddRef`, 
`Release`, and `QueryInterface`. Instead you should make use of ComPtr 
facilities to achieve the same effect. Doing something like 
`object.Get()->Release()` is asking for trouble.