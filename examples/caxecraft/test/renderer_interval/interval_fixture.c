/* Independent runtime/ABI test consumer, never application behavior.
 * Technical need: invoke native collector/allocator operations and inspect an
 * exact count without relying on unsupported generated Haxe exports.
 * Durable value: compiler/import regressions cannot manufacture the independent
 * native expectations. Haxe owns the tested interval and acceptance assertions.
 * This is the same bounded test ownership as hxrt's generated root-frame harness.
 */
#include "interval_fixture.h"
#include "interval_observer.h"
#include "hxrt/gc.h"
#include <stdio.h>
#include <stdlib.h>

static hxc_gc collector = HXC_GC_INITIALIZER;
static hxc_gc_thread thread = HXC_GC_THREAD_INITIALIZER;

void interval_fixture_require(bool condition) {
  if (!condition) {
    (void)fputs("interval-import: expectation failed\n", stderr);
    abort();
  }
}

void interval_fixture_setup(void) {
  const hxc_gc_config config = { hxc_default_allocator(), 1024u, NULL, NULL };
  interval_fixture_require(hxc_gc_init(&config, &collector) == HXC_STATUS_OK);
  interval_fixture_require(hxc_gc_thread_register(&collector, &thread) == HXC_STATUS_OK);
}

bool interval_fixture_allocate(void) {
  const hxc_allocator allocator = hxc_default_allocator();
  void *memory = NULL;
  if (hxc_alloc(&allocator, 8u, 1u, &memory) != HXC_STATUS_OK) return false;
  return hxc_free(&allocator, memory, 8u, 1u) == HXC_STATUS_OK;
}

bool interval_fixture_roots(void) {
  hxc_gc_root_frame frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  const void *volatile slots[1] = { NULL };
  if (hxc_gc_root_frame_push(&thread, slots, 1u, &frame) != HXC_STATUS_OK) return false;
  return hxc_gc_root_frame_pop(&frame) == HXC_STATUS_OK;
}

void interval_fixture_wide_count(void) {
  /* Synthetic event only; do not present this as actual root work. */
  caxecraft_test_interval_record(CAXECRAFT_TEST_INTERVAL_ROOT_PUSH_ATTEMPT, INTERVAL_FIXTURE_WIDE);
}

void interval_fixture_finish(void) {
  interval_fixture_require(hxc_gc_thread_unregister(&thread) == HXC_STATUS_OK);
  interval_fixture_require(hxc_gc_dispose(&collector) == HXC_STATUS_OK);
  (void)puts("interval-import: OK allocation=1 root-push=1 root-pop=1 wide=4294967297");
}
