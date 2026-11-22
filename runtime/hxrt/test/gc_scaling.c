/* Reproducible exact-base lookup scaling workload for the selected collector. */
#include "hxrt/gc.h"

#include <stdio.h>
#include <time.h>

#define HXC_CHECK(condition) \
  do { \
    if (!(condition)) { \
      return 1; \
    } \
  } while (0)

typedef struct gc_scaling_node {
  struct gc_scaling_node *next;
  uint32_t value;
} gc_scaling_node;

static void gc_scaling_trace(
  const void *object,
  hxc_trace_visit_fn visit,
  void *context
) {
  const gc_scaling_node *node = (const gc_scaling_node *)object;
  visit(context, node->next);
}

static const hxc_type_descriptor gc_scaling_descriptor = {
  HXC_TYPE_DESCRIPTOR_ABI_VERSION,
  HXC_TYPE_DESCRIPTOR_HAS_TRACE,
  sizeof(gc_scaling_node),
  HXC_ALIGNOF(gc_scaling_node),
  gc_scaling_trace,
  NULL
};

static hxc_status gc_scaling_allocate(
  hxc_gc *gc,
  gc_scaling_node **out_node
) {
  void *object = NULL;
  const hxc_status status = hxc_gc_allocate(
    gc,
    &gc_scaling_descriptor,
    &object
  );
  if (status == HXC_STATUS_OK) {
    *out_node = (gc_scaling_node *)object;
  }
  return status;
}

static int gc_scaling_case(size_t count, size_t repetitions) {
  hxc_gc gc = HXC_GC_INITIALIZER;
  hxc_gc_config config;
  hxc_gc_thread thread = HXC_GC_THREAD_INITIALIZER;
  hxc_gc_root_frame frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  const void *volatile roots[1] = { NULL };
  gc_scaling_node *first = NULL;
  gc_scaling_node *previous = NULL;
  hxc_gc_stats stats = HXC_GC_STATS_INITIALIZER;
  clock_t started;
  clock_t finished;
  size_t index;

  config.allocator = hxc_default_allocator();
  config.initial_threshold_bytes = SIZE_MAX / 4u;
  config.clock_now = NULL;
  config.clock_context = NULL;
  HXC_CHECK(hxc_gc_init(&config, &gc) == HXC_STATUS_OK);
  HXC_CHECK(hxc_gc_thread_register(&gc, &thread) == HXC_STATUS_OK);
  HXC_CHECK(hxc_gc_root_frame_push(&thread, roots, 1u, &frame) == HXC_STATUS_OK);

  for (index = 0u; index < count; index++) {
    gc_scaling_node *node = NULL;
    HXC_CHECK(gc_scaling_allocate(&gc, &node) == HXC_STATUS_OK);
    node->value = (uint32_t)index;
    if (first == NULL) {
      first = node;
      roots[0] = first;
    } else {
      previous->next = node;
    }
    previous = node;
  }
  HXC_CHECK(first != NULL && previous != NULL);
  previous->next = first;

  started = clock();
  HXC_CHECK(started != (clock_t)-1);
  for (index = 0u; index < repetitions; index++) {
    HXC_CHECK(hxc_gc_collect(&gc) == HXC_STATUS_OK);
  }
  finished = clock();
  HXC_CHECK(finished != (clock_t)-1 && finished >= started);
  HXC_CHECK(hxc_gc_get_stats(&gc, &stats) == HXC_STATUS_OK);
  HXC_CHECK(stats.current_object_count == count);
  HXC_CHECK(stats.exact_index_build_count == (uint64_t)repetitions);
  HXC_CHECK(stats.exact_index_fallback_count == UINT64_C(0));
  HXC_CHECK(
    stats.exact_lookup_count == (uint64_t)repetitions * ((uint64_t)count + UINT64_C(1))
  );
  HXC_CHECK(stats.exact_probe_count >= stats.exact_lookup_count);
  HXC_CHECK(stats.exact_probe_count <= stats.exact_lookup_count * UINT64_C(8));
  HXC_CHECK(stats.exact_index_bytes > 0u);

  (void)printf(
    "gc-scaling: objects=%zu repetitions=%zu clock_ticks=%llu "
    "index_bytes=%zu lookups=%llu probes=%llu fallbacks=%llu\n",
    count,
    repetitions,
    (unsigned long long)(finished - started),
    stats.exact_index_bytes,
    (unsigned long long)stats.exact_lookup_count,
    (unsigned long long)stats.exact_probe_count,
    (unsigned long long)stats.exact_index_fallback_count
  );

  roots[0] = NULL;
  HXC_CHECK(hxc_gc_collect(&gc) == HXC_STATUS_OK);
  HXC_CHECK(hxc_gc_root_frame_pop(&frame) == HXC_STATUS_OK);
  HXC_CHECK(hxc_gc_thread_unregister(&thread) == HXC_STATUS_OK);
  HXC_CHECK(hxc_gc_dispose(&gc) == HXC_STATUS_OK);
  return 0;
}

int main(void) {
  HXC_CHECK(gc_scaling_case(256u, 200u) == 0);
  HXC_CHECK(gc_scaling_case(1024u, 50u) == 0);
  HXC_CHECK(gc_scaling_case(4096u, 5u) == 0);
  return 0;
}
