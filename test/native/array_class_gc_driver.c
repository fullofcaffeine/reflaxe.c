/*
 * Independent native consumer for the generated Array<Class> graph fixture.
 *
 * The generated executable normally disposes its whole collector at shutdown.
 * Disposal proves cleanup safety but cannot distinguish cycle collection from
 * unconditional process teardown. This driver instead calls the generated
 * Haxe entry function inside an explicitly initialized collector, waits until
 * all generated root frames have returned, forces one mark-and-sweep pass, and
 * checks that every remaining class and Array payload was reclaimed.
 *
 * This C file is test authority, not application implementation and not
 * generated-output evidence. The Haxe fixture remains the owner of the graph;
 * this independent consumer only observes the public hxrt statistics needed to
 * prove the collector did not validate itself against compiler-generated
 * expectations.
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
