#include <cstdio>

#include "raygui.h"

/* C++17 checks the same public C ABI and raygui's language guards. */
int main() {
  GuiLoadStyleDefault();
  GuiLock();
  if (!GuiIsLocked()) {
    return 1;
  }
  GuiUnlock();
  if (GuiIsLocked()) {
    return 2;
  }
  std::puts("raygui-cpp-consumer: OK");
  return 0;
}
