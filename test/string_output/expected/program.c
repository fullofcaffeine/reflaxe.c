#include "hxc/program.h"

void hxc_Main_main(void)
{
  if (hxc_io_println((hxc_string){ (const uint8_t *)"ASCII", 5, true }) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_io_println((hxc_string){ (const uint8_t *)"\303\251\360\237\231\202", 6, true }) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_io_println((hxc_string){ (const uint8_t *)"embedded\000NUL", 12, true }) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_io_println((hxc_string){ (const uint8_t *)"Main.hx:6: traced", 17, true }) != HXC_STATUS_OK)
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
