#include <stdint.h>

static int semihost_call(int operation, const void *argument) {
  register int r0 __asm__("r0") = operation;
  register const void *r1 __asm__("r1") = argument;
  __asm__ volatile("bkpt 0xab" : "+r"(r0) : "r"(r1) : "memory");
  return r0;
}

void reset_handler(void) {
  static const char message[] = "hxc-cortex-m3: OK\n";
  semihost_call(0x04, message);
  semihost_call(0x18, (const void *)(uintptr_t)0x20026u);
  for (;;) {
  }
}

__attribute__((section(".vectors"), used))
const uintptr_t vectors[] = {
  0x20010000u,
  (uintptr_t)reset_handler
};
