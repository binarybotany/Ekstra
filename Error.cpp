#include "Error.h"

namespace Ekstra {
void Error::Display(HRESULT result, LPCWSTR message) {
  _com_error error(result);
  MessageBox(NULL, message + '\n' + error.Description(), L"Error",
             MB_ICONERROR);
}
}  // namespace Ekstra