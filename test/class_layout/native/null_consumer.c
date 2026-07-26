/*
 * This file is an independent C consumer, not generated Haxe. It calls the
 * generated private C entry directly so the test can prove that a null Haxe
 * receiver reaches the checked fail-stop boundary seen by a C caller.
 *
 * The generated method also uses the exact-root protocol selected for the
 * retained class graph. Initializing the collector first lets this test reach
 * the intended null check instead of failing earlier because the generated
 * runtime lifecycle was never started.
 */
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
  if (hxc_gc_init(&(struct hxc_gc_config){
        hxc_default_allocator(),
        1048576U,
        NULL,
        NULL,
      }, &hxc_program_gc) != HXC_STATUS_OK ||
      hxc_gc_thread_register(&hxc_program_gc, &hxc_program_gc_thread) != HXC_STATUS_OK) {
    return 10;
  }
  return HXC_FN_WRITE_INHERITED(NULL, INT32_C(1));
}
