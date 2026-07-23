#define main hxc_class_layout_generated_main
int main(void);
#include "../src/program.c"
#undef main

#include <stddef.h>
#include <stdint.h>

int main(void);

/** Calling a checked instance method with a null receiver must fail-stop. */
int main(void)
{
  return HXC_FN_WRITE_INHERITED(NULL, INT32_C(1));
}
