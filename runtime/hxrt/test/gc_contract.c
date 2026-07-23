/*
 * Independent native conformance fixture for the selected precise collector.
 *
 * This is intentionally authored C rather than generated Haxe: it probes the
 * runtime boundary independently of the compiler that will later produce root
 * frames and descriptors. Ordinary-Haxe integration keeps its own generated
 * regression, so neither side can validate itself by construction.
 */
#include "hxrt/gc.h"

#include <stdio.h>

#define HXC_CHECK(condition) \
  do { \
    if (!(condition)) { \
      (void)fprintf(stderr, "gc-contract: check failed at line %d\n", __LINE__); \
      return 1; \
    } \
  } while (0)

typedef struct gc_test_node {
  struct gc_test_node *left;
  struct gc_test_node *right;
  int32_t value;
} gc_test_node;

typedef struct gc_test_bad_trace {
  gc_test_node *child;
} gc_test_bad_trace;

static uint64_t gc_test_clock = UINT64_C(0);
static uint64_t gc_test_finalizer_count = UINT64_C(0);

static uint64_t gc_test_now(void *context) {
  uint64_t *clock = (uint64_t *)context;
  *clock += UINT64_C(7);
  return *clock;
}

static void gc_test_trace_node(
  const void *object,
  hxc_trace_visit_fn visit,
  void *context
) {
  const gc_test_node *node = (const gc_test_node *)object;
  visit(context, node->left);
  visit(context, node->right);
}

static void gc_test_finalize_node(void *object) {
  gc_test_node *node = (gc_test_node *)object;
  node->value = INT32_C(-1);
  gc_test_finalizer_count++;
}

static void gc_test_trace_interior(
  const void *object,
  hxc_trace_visit_fn visit,
  void *context
) {
  const gc_test_bad_trace *bad = (const gc_test_bad_trace *)object;
  visit(context, (const unsigned char *)bad->child + 1u);
}

static void gc_test_finalize_bad_trace(void *object) {
  gc_test_bad_trace *bad = (gc_test_bad_trace *)object;
  bad->child = NULL;
  gc_test_finalizer_count++;
}

static const hxc_type_descriptor gc_test_node_descriptor = {
  HXC_TYPE_DESCRIPTOR_ABI_VERSION,
  HXC_TYPE_DESCRIPTOR_HAS_TRACE | HXC_TYPE_DESCRIPTOR_HAS_FINALIZER,
  sizeof(gc_test_node),
  HXC_ALIGNOF(gc_test_node),
  gc_test_trace_node,
  gc_test_finalize_node
};

static const hxc_type_descriptor gc_test_bad_trace_descriptor = {
  HXC_TYPE_DESCRIPTOR_ABI_VERSION,
  HXC_TYPE_DESCRIPTOR_HAS_TRACE | HXC_TYPE_DESCRIPTOR_HAS_FINALIZER,
  sizeof(gc_test_bad_trace),
  HXC_ALIGNOF(gc_test_bad_trace),
  gc_test_trace_interior,
  gc_test_finalize_bad_trace
};

static hxc_status gc_test_allocate_node(hxc_gc *gc, gc_test_node **out_node) {
  void *object = NULL;
  hxc_status status;
  if (out_node == NULL) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  status = hxc_gc_allocate(gc, &gc_test_node_descriptor, &object);
  if (status == HXC_STATUS_OK) {
    *out_node = (gc_test_node *)object;
  }
  return status;
}

static hxc_status gc_test_allocate_bad_trace(
  hxc_gc *gc,
  gc_test_bad_trace **out_bad
) {
  void *object = NULL;
  hxc_status status;
  if (out_bad == NULL) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  status = hxc_gc_allocate(gc, &gc_test_bad_trace_descriptor, &object);
  if (status == HXC_STATUS_OK) {
    *out_bad = (gc_test_bad_trace *)object;
  }
  return status;
}

static int gc_test_primary_contract(hxc_gc *gc) {
  hxc_gc_thread main_thread = HXC_GC_THREAD_INITIALIZER;
  hxc_gc_thread worker_thread = HXC_GC_THREAD_INITIALIZER;
  hxc_gc_root_frame main_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  hxc_gc_root_frame worker_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  hxc_gc_root_table globals = HXC_GC_ROOT_TABLE_INITIALIZER;
  hxc_gc_pin pin = HXC_GC_PIN_INITIALIZER;
  const void *main_slots[2] = { NULL, NULL };
  const void *worker_slots[1] = { NULL };
  const void *global_slots[1] = { NULL };
  gc_test_node *first = NULL;
  gc_test_node *second = NULL;
  gc_test_node *orphan = NULL;
  gc_test_node *global = NULL;
  gc_test_node *worker = NULL;
  gc_test_node *foreign_retained = NULL;
  gc_test_node *invalid_root = NULL;
  gc_test_node *invalid_child = NULL;
  gc_test_bad_trace *bad_trace = NULL;
  hxc_gc_stats stats = HXC_GC_STATS_INITIALIZER;
  size_t round;

  HXC_CHECK(hxc_gc_thread_register(gc, &main_thread) == HXC_STATUS_OK);
  HXC_CHECK(hxc_gc_thread_register(gc, &worker_thread) == HXC_STATUS_OK);
  HXC_CHECK(hxc_gc_root_frame_push(&main_thread, main_slots, 2u, &main_frame) == HXC_STATUS_OK);
  HXC_CHECK(hxc_gc_root_frame_push(&worker_thread, worker_slots, 1u, &worker_frame) == HXC_STATUS_OK);
  HXC_CHECK(hxc_gc_root_table_register(gc, global_slots, 1u, &globals) == HXC_STATUS_OK);

  /* A rooted cycle stays live while an unrelated object is reclaimed. */
  HXC_CHECK(gc_test_allocate_node(gc, &first) == HXC_STATUS_OK);
  main_slots[0] = first;
  HXC_CHECK(gc_test_allocate_node(gc, &second) == HXC_STATUS_OK);
  HXC_CHECK(gc_test_allocate_node(gc, &orphan) == HXC_STATUS_OK);
  first->left = second;
  second->right = first;
  HXC_CHECK(hxc_gc_collect(gc) == HXC_STATUS_OK);
  HXC_CHECK(hxc_gc_owns_exact(gc, first));
  HXC_CHECK(hxc_gc_owns_exact(gc, second));
  HXC_CHECK(!hxc_gc_owns_exact(gc, orphan));
  main_slots[0] = NULL;
  HXC_CHECK(hxc_gc_collect(gc) == HXC_STATUS_OK);
  HXC_CHECK(!hxc_gc_owns_exact(gc, first));
  HXC_CHECK(!hxc_gc_owns_exact(gc, second));

  /* Global and second-thread roots participate in the same exact mark. */
  HXC_CHECK(gc_test_allocate_node(gc, &global) == HXC_STATUS_OK);
  global_slots[0] = global;
  HXC_CHECK(gc_test_allocate_node(gc, &worker) == HXC_STATUS_OK);
  worker_slots[0] = worker;
  HXC_CHECK(hxc_gc_collect(gc) == HXC_STATUS_OK);
  HXC_CHECK(hxc_gc_owns_exact(gc, global));
  HXC_CHECK(hxc_gc_owns_exact(gc, worker));
  global_slots[0] = NULL;
  worker_slots[0] = NULL;
  HXC_CHECK(hxc_gc_collect(gc) == HXC_STATUS_OK);
  HXC_CHECK(!hxc_gc_owns_exact(gc, global));
  HXC_CHECK(!hxc_gc_owns_exact(gc, worker));

  /* A pin is the only supported way for foreign code to retain a payload. */
  HXC_CHECK(gc_test_allocate_node(gc, &foreign_retained) == HXC_STATUS_OK);
  HXC_CHECK(hxc_gc_pin_object(gc, foreign_retained, &pin) == HXC_STATUS_OK);
  HXC_CHECK(hxc_gc_collect(gc) == HXC_STATUS_OK);
  HXC_CHECK(hxc_gc_owns_exact(gc, foreign_retained));
  HXC_CHECK(
    hxc_gc_pin_object(
      gc,
      (const unsigned char *)foreign_retained + 1u,
      &pin
    ) == HXC_STATUS_INVALID_ARGUMENT
  );
  HXC_CHECK(hxc_gc_unpin_object(&pin) == HXC_STATUS_OK);
  HXC_CHECK(hxc_gc_collect(gc) == HXC_STATUS_OK);
  HXC_CHECK(!hxc_gc_owns_exact(gc, foreign_retained));

  /* An interior root fails the collection instead of retaining by accident. */
  HXC_CHECK(gc_test_allocate_node(gc, &invalid_root) == HXC_STATUS_OK);
  main_slots[0] = (const unsigned char *)invalid_root + 1u;
  HXC_CHECK(hxc_gc_collect(gc) == HXC_STATUS_INVALID_ARGUMENT);
  HXC_CHECK(hxc_gc_owns_exact(gc, invalid_root));
  main_slots[0] = invalid_root;
  HXC_CHECK(hxc_gc_collect(gc) == HXC_STATUS_OK);
  main_slots[0] = NULL;
  HXC_CHECK(hxc_gc_collect(gc) == HXC_STATUS_OK);

  /* A generated trace callback reporting an interior field is an internal bug. */
  HXC_CHECK(gc_test_allocate_bad_trace(gc, &bad_trace) == HXC_STATUS_OK);
  main_slots[0] = bad_trace;
  HXC_CHECK(gc_test_allocate_node(gc, &invalid_child) == HXC_STATUS_OK);
  bad_trace->child = invalid_child;
  HXC_CHECK(hxc_gc_collect(gc) == HXC_STATUS_INTERNAL_ERROR);
  HXC_CHECK(hxc_gc_owns_exact(gc, bad_trace));
  HXC_CHECK(hxc_gc_owns_exact(gc, invalid_child));
  main_slots[0] = NULL;
  HXC_CHECK(hxc_gc_collect(gc) == HXC_STATUS_OK);

  /* Repeated unreachable cycles exercise the intrusive mark worklist/sweep. */
  for (round = 0u; round < 128u; round++) {
    HXC_CHECK(gc_test_allocate_node(gc, &first) == HXC_STATUS_OK);
    main_slots[0] = first;
    HXC_CHECK(gc_test_allocate_node(gc, &second) == HXC_STATUS_OK);
    first->left = second;
    second->right = first;
    main_slots[0] = NULL;
    HXC_CHECK(hxc_gc_collect(gc) == HXC_STATUS_OK);
  }

  HXC_CHECK(hxc_gc_get_stats(gc, &stats) == HXC_STATUS_OK);
  HXC_CHECK(stats.current_object_count == 0u);
  HXC_CHECK(stats.live_payload_bytes == 0u);
  HXC_CHECK(stats.internal_overhead_bytes == 0u);
  HXC_CHECK(stats.reclaimed_object_count == stats.allocation_count);
  HXC_CHECK(stats.reclaimed_payload_bytes == stats.allocated_payload_bytes);
  HXC_CHECK(stats.collection_count >= UINT64_C(128));
  HXC_CHECK(stats.pause_sample_count > stats.collection_count);
  HXC_CHECK(stats.last_pause_ticks == UINT64_C(7));
  HXC_CHECK(stats.maximum_pause_ticks == UINT64_C(7));
  HXC_CHECK(stats.clock_regression_count == UINT64_C(0));

  HXC_CHECK(hxc_gc_root_table_unregister(&globals) == HXC_STATUS_OK);
  HXC_CHECK(hxc_gc_root_frame_pop(&worker_frame) == HXC_STATUS_OK);
  HXC_CHECK(hxc_gc_root_frame_pop(&main_frame) == HXC_STATUS_OK);
  HXC_CHECK(hxc_gc_thread_unregister(&worker_thread) == HXC_STATUS_OK);
  HXC_CHECK(hxc_gc_thread_unregister(&main_thread) == HXC_STATUS_OK);
  return 0;
}

static int gc_test_pressure_contract(hxc_allocator allocator) {
  hxc_gc gc = HXC_GC_INITIALIZER;
  hxc_gc_thread thread = HXC_GC_THREAD_INITIALIZER;
  hxc_gc_root_frame frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  const void *slots[1] = { NULL };
  hxc_gc_config config;
  hxc_gc_stats stats = HXC_GC_STATS_INITIALIZER;
  gc_test_node *first = NULL;
  gc_test_node *second = NULL;

  config.allocator = allocator;
  config.initial_threshold_bytes = sizeof(gc_test_node);
  config.clock_now = gc_test_now;
  config.clock_context = &gc_test_clock;
  HXC_CHECK(hxc_gc_init(&config, &gc) == HXC_STATUS_OK);
  HXC_CHECK(hxc_gc_thread_register(&gc, &thread) == HXC_STATUS_OK);
  HXC_CHECK(hxc_gc_root_frame_push(&thread, slots, 1u, &frame) == HXC_STATUS_OK);
  HXC_CHECK(gc_test_allocate_node(&gc, &first) == HXC_STATUS_OK);
  slots[0] = first;
  HXC_CHECK(gc_test_allocate_node(&gc, &second) == HXC_STATUS_OK);
  HXC_CHECK(hxc_gc_owns_exact(&gc, first));
  HXC_CHECK(hxc_gc_owns_exact(&gc, second));
  slots[0] = NULL;
  HXC_CHECK(hxc_gc_collect(&gc) == HXC_STATUS_OK);
  HXC_CHECK(hxc_gc_get_stats(&gc, &stats) == HXC_STATUS_OK);
  HXC_CHECK(stats.pressure_collection_count >= UINT64_C(1));
  HXC_CHECK(stats.current_object_count == 0u);
  HXC_CHECK(hxc_gc_root_frame_pop(&frame) == HXC_STATUS_OK);
  HXC_CHECK(hxc_gc_thread_unregister(&thread) == HXC_STATUS_OK);
  HXC_CHECK(hxc_gc_dispose(&gc) == HXC_STATUS_OK);
  return 0;
}

int main(void) {
  hxc_allocator allocator = hxc_default_allocator();
  hxc_gc gc = HXC_GC_INITIALIZER;
  hxc_gc_config config;
  hxc_gc_stats final_stats = HXC_GC_STATS_INITIALIZER;
  const uint64_t finalizers_before_pressure = gc_test_finalizer_count;

  HXC_CHECK(hxc_allocator_is_valid(&allocator));
  HXC_CHECK(hxc_type_descriptor_is_valid(&gc_test_node_descriptor));
  config.allocator = allocator;
  config.initial_threshold_bytes = 1024u * 1024u;
  config.clock_now = gc_test_now;
  config.clock_context = &gc_test_clock;
  HXC_CHECK(hxc_gc_init(&config, &gc) == HXC_STATUS_OK);
  HXC_CHECK(gc_test_primary_contract(&gc) == 0);
  HXC_CHECK(hxc_gc_get_stats(&gc, &final_stats) == HXC_STATUS_OK);
  HXC_CHECK(gc_test_finalizer_count == final_stats.allocation_count);
  HXC_CHECK(hxc_gc_dispose(&gc) == HXC_STATUS_OK);
  HXC_CHECK(gc_test_pressure_contract(allocator) == 0);
  HXC_CHECK(gc_test_finalizer_count > finalizers_before_pressure);

  (void)printf(
    "gc-contract: OK allocations=%llu collections=%llu reclaimed=%llu pause_ticks=%llu\n",
    (unsigned long long)final_stats.allocation_count,
    (unsigned long long)final_stats.collection_count,
    (unsigned long long)final_stats.reclaimed_object_count,
    (unsigned long long)final_stats.total_pause_ticks
  );
  return 0;
}
