#include "hxc/program.h"

void hxc_Main_main(void)
{
  if (hxc_io_println((hxc_string){ (const uint8_t *)"Hello from hxc", 14, true, NULL }) != HXC_STATUS_OK)
  {
    abort();
  }
  return;
}

int main(void)
{
  hxc_Main_main();
  return 0;
}
