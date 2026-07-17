#include "hxrt/io.h"

int main(void) {
  if (hxc_io_println((hxc_string)HXC_STRING_LITERAL("runtime-feature-io")) != HXC_STATUS_OK) {
    return 1;
  }
  return 0;
}
