#include "hxc/program.h"

static void hxc_ModuleFunctions_static_initialization(void)
{
  hxc_ModuleFunctions_static_field_base();
}

int main(void)
{
  hxc_ModuleFunctions_static_initialization();
  hxc_ModuleFunctions_main();
  return 0;
}
