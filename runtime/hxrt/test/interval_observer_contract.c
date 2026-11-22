/* Independent expectations for runtime entry observation. No generated names,
 * compiler planner, or observer implementation is used to derive these counts.
 * See interval_observer.h for the two reasons this test boundary is native C.
 */
#include "interval_observer.h"
#include "hxrt/gc.h"
#include <stdio.h>

#define CHECK(condition) do { if (!(condition)) { \
  (void)fprintf(stderr, "interval-observer: failed line %d\n", __LINE__); \
  return 1; } } while (0)

static bool allocation_seen_before_callback;
static bool collection_seen_before_clock;
static uint64_t clock_calls;

static hxc_status fail_allocate(void *context, size_t size, size_t alignment,
  void **out_memory) {
  (void)context; (void)size; (void)alignment; (void)out_memory;
  allocation_seen_before_callback =
    caxecraft_test_interval_count(CAXECRAFT_TEST_INTERVAL_ALLOC_ATTEMPT) != UINT64_C(0);
  return HXC_STATUS_OUT_OF_MEMORY;
}
static void unused_release(void *context, void *memory, size_t size, size_t alignment) {
  (void)context; (void)memory; (void)size; (void)alignment;
}
static uint64_t observe_clock(void *context) {
  (void)context;
  collection_seen_before_clock = collection_seen_before_clock &&
    caxecraft_test_interval_count(CAXECRAFT_TEST_INTERVAL_COLLECT_ATTEMPT) == UINT64_C(1);
  return ++clock_calls;
}

int main(void) {
  hxc_allocator failing = { NULL, fail_allocate, NULL, unused_release };
  hxc_allocator hosted = hxc_default_allocator();
  hxc_gc gc = HXC_GC_INITIALIZER;
  hxc_gc_config config = { hosted, 1024u, observe_clock, NULL };
  hxc_gc_thread thread = HXC_GC_THREAD_INITIALIZER;
  hxc_gc_root_frame frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  const void *volatile slots[1] = { NULL };
  void *memory = NULL;
  void *resized = NULL;
  int event;

  caxecraft_test_interval_reset();
  CHECK(hxc_alloc(&hosted, 8u, 1u, &memory) == HXC_STATUS_OK);
  CHECK(hxc_free(&hosted, memory, 8u, 1u) == HXC_STATUS_OK);
  memory = NULL;
  CHECK(hxc_gc_collect(NULL) == HXC_STATUS_INVALID_ARGUMENT);
  for (event = 0; event < CAXECRAFT_TEST_INTERVAL_EVENT_COUNT; event++)
    CHECK(caxecraft_test_interval_count((caxecraft_test_interval_event)event) == 0u);
  CHECK(caxecraft_test_interval_valid());
  CHECK(caxecraft_test_interval_completed() == 0u);

  CHECK(caxecraft_test_interval_begin());
  CHECK(!caxecraft_test_interval_valid()); /* unfinished evidence cannot pass */
  CHECK(hxc_alloc(&failing, 8u, 1u, &memory) == HXC_STATUS_OUT_OF_MEMORY);
  CHECK(memory == NULL && allocation_seen_before_callback);
  CHECK(hxc_alloc(NULL, 8u, 1u, &memory) == HXC_STATUS_INVALID_ARGUMENT);
  CHECK(hxc_alloc(&hosted, 0u, 1u, &memory) == HXC_STATUS_OK);
  CHECK(caxecraft_test_interval_count(CAXECRAFT_TEST_INTERVAL_ALLOC_ATTEMPT) == 3u);
  CHECK(hxc_realloc(&failing, NULL, 0u, 8u, 1u, &memory) == HXC_STATUS_OUT_OF_MEMORY);
  CHECK(caxecraft_test_interval_count(CAXECRAFT_TEST_INTERVAL_REALLOC_ATTEMPT) == 1u);
  CHECK(caxecraft_test_interval_count(CAXECRAFT_TEST_INTERVAL_ALLOC_ATTEMPT) == 4u);
  CHECK(caxecraft_test_interval_end());
  CHECK(caxecraft_test_interval_valid() && caxecraft_test_interval_completed() == 1u);
  CHECK(caxecraft_test_interval_begin());
  CHECK(caxecraft_test_interval_end());
  CHECK(caxecraft_test_interval_completed() == 2u);
  CHECK(caxecraft_test_interval_count(CAXECRAFT_TEST_INTERVAL_ALLOC_ATTEMPT) == 4u);

  /* A real fallback resize enters both APIs; a callback resize enters realloc
   * only. The latter's internal host allocation is not another API entry. */
  caxecraft_test_interval_reset();
  hosted.reallocate = NULL;
  CHECK(hxc_alloc(&hosted, 8u, 1u, &memory) == HXC_STATUS_OK);
  CHECK(caxecraft_test_interval_begin());
  CHECK(hxc_realloc(&hosted, memory, 8u, 16u, 1u, &resized) == HXC_STATUS_OK);
  CHECK(caxecraft_test_interval_count(CAXECRAFT_TEST_INTERVAL_REALLOC_ATTEMPT) == 1u);
  CHECK(caxecraft_test_interval_count(CAXECRAFT_TEST_INTERVAL_ALLOC_ATTEMPT) == 1u);
  CHECK(caxecraft_test_interval_end());
  CHECK(hxc_free(&hosted, resized, 16u, 1u) == HXC_STATUS_OK);
  hosted = hxc_default_allocator();
  CHECK(hxc_alloc(&hosted, 8u, 1u, &memory) == HXC_STATUS_OK);
  caxecraft_test_interval_reset();
  CHECK(caxecraft_test_interval_begin());
  CHECK(hxc_realloc(&hosted, memory, 8u, 16u, 1u, &resized) == HXC_STATUS_OK);
  CHECK(caxecraft_test_interval_count(CAXECRAFT_TEST_INTERVAL_REALLOC_ATTEMPT) == 1u);
  CHECK(caxecraft_test_interval_count(CAXECRAFT_TEST_INTERVAL_ALLOC_ATTEMPT) == 0u);
  CHECK(caxecraft_test_interval_end());
  CHECK(hxc_free(&hosted, resized, 16u, 1u) == HXC_STATUS_OK);

  caxecraft_test_interval_reset();
  CHECK(hxc_gc_init(&config, &gc) == HXC_STATUS_OK);
  CHECK(hxc_gc_thread_register(&gc, &thread) == HXC_STATUS_OK);
  collection_seen_before_clock = true;
  CHECK(caxecraft_test_interval_begin());
  CHECK(hxc_gc_collect(&gc) == HXC_STATUS_OK);
  CHECK(clock_calls == 2u && collection_seen_before_clock);
  CHECK(hxc_gc_collect(NULL) == HXC_STATUS_INVALID_ARGUMENT);
  CHECK(caxecraft_test_interval_count(CAXECRAFT_TEST_INTERVAL_COLLECT_ATTEMPT) == 2u);
  CHECK(hxc_gc_root_frame_push(&thread, slots, 1u, &frame) == HXC_STATUS_OK);
  CHECK(hxc_gc_root_frame_push(&thread, slots, 1u, &frame) == HXC_STATUS_INVALID_ARGUMENT);
  CHECK(hxc_gc_root_frame_pop(&frame) == HXC_STATUS_OK);
  CHECK(hxc_gc_root_frame_pop(&frame) == HXC_STATUS_INVALID_ARGUMENT);
  /* Attempts include failures; equal counts do NOT establish root balance. */
  CHECK(caxecraft_test_interval_count(CAXECRAFT_TEST_INTERVAL_ROOT_PUSH_ATTEMPT) == 2u);
  CHECK(caxecraft_test_interval_count(CAXECRAFT_TEST_INTERVAL_ROOT_POP_ATTEMPT) == 2u);
  CHECK(caxecraft_test_interval_end());
  CHECK(hxc_gc_thread_unregister(&thread) == HXC_STATUS_OK);
  CHECK(hxc_gc_dispose(&gc) == HXC_STATUS_OK);

  caxecraft_test_interval_reset();
  CHECK(caxecraft_test_interval_begin());
  CHECK(!caxecraft_test_interval_begin());
  CHECK(!caxecraft_test_interval_end());
  CHECK(!caxecraft_test_interval_valid());
  caxecraft_test_interval_reset();
  CHECK(!caxecraft_test_interval_end());
  CHECK(!caxecraft_test_interval_begin());
  CHECK(!caxecraft_test_interval_valid());
  caxecraft_test_interval_reset();
  CHECK(caxecraft_test_interval_begin());
  caxecraft_test_interval_reset(); /* cannot erase an unfinished interval */
  CHECK(!caxecraft_test_interval_end());
  CHECK(!caxecraft_test_interval_valid());
  caxecraft_test_interval_reset();
  CHECK(caxecraft_test_interval_begin());
  caxecraft_test_interval_record(CAXECRAFT_TEST_INTERVAL_ALLOC_ATTEMPT, UINT64_MAX);
  caxecraft_test_interval_record(CAXECRAFT_TEST_INTERVAL_ALLOC_ATTEMPT, UINT64_C(1));
  CHECK(caxecraft_test_interval_count(CAXECRAFT_TEST_INTERVAL_ALLOC_ATTEMPT) == UINT64_MAX);
  CHECK(!caxecraft_test_interval_end());
  CHECK(!caxecraft_test_interval_valid());

  (void)puts("interval-observer: OK");
  return 0;
}
