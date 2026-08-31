/*
 * Independent reclamation observer for the generated collection-cycle fixture.
 *
 * The Haxe entry point creates self, mutual, broken, deep, and pressure-driven
 * Array/record/enum graphs. After its exact root frame returns, this driver
 * forces one collection and checks public collector statistics. This separates
 * real unreachable-cycle reclamation from unconditional process teardown.
 */

#include "hxc/program.h"

int main(void) {
  const hxc_gc_config config = {
    hxc_default_allocator(),
    1048576u,
    NULL,
    NULL
  };
  hxc_gc_stats before = HXC_GC_STATS_INITIALIZER;
  hxc_gc_stats after = HXC_GC_STATS_INITIALIZER;

  if (hxc_gc_init(&config, &hxc_program_gc) != HXC_STATUS_OK) {
    return 10;
  }
  if (
    hxc_gc_thread_register(&hxc_program_gc, &hxc_program_gc_thread)
    != HXC_STATUS_OK
  ) {
    return 11;
  }

  hxc_Main_main();
  if (
    hxc_gc_get_stats(&hxc_program_gc, &before) != HXC_STATUS_OK
    || before.current_object_count == 0u
    || before.pressure_collection_count == 0u
  ) {
    return 12;
  }
  if (hxc_gc_collect(&hxc_program_gc) != HXC_STATUS_OK) {
    return 13;
  }
  if (
    hxc_gc_get_stats(&hxc_program_gc, &after) != HXC_STATUS_OK
    || after.current_object_count != 0u
    || after.reclaimed_object_count < before.current_object_count
  ) {
    return 14;
  }

  if (hxc_gc_thread_unregister(&hxc_program_gc_thread) != HXC_STATUS_OK) {
    return 15;
  }
  if (hxc_gc_dispose(&hxc_program_gc) != HXC_STATUS_OK) {
    return 16;
  }
  return 0;
}
