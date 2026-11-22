#include "hxc/program.h"

int main() {
  void (*entry)(void) = &hxc_Main_main;
  (void)entry;
  return 0;
}
