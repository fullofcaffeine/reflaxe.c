/* Freestanding custom-allocator proof for the selected collector. */
#include "hxrt/gc.h"

#include <stdio.h>

#define HXC_CHECK(condition) \
  do { \
    if (!(condition)) { \
      return 1; \
    } \
  } while (0)

typedef struct gc_arena {
  unsigned char storage[4096];
  size_t used;
  size_t release_count;
} gc_arena;

typedef struct gc_pair {
  struct gc_pair *other;
  int32_t value;
} gc_pair;

static hxc_status gc_arena_allocate(
  void *context,
  size_t size,
  size_t alignment,
  void **out_memory
) {
  gc_arena *arena = (gc_arena *)context;
  const uintptr_t base = (uintptr_t)&arena->storage[arena->used];
  const size_t remainder = (size_t)(base % (uintptr_t)alignment);
  const size_t padding = remainder == 0u ? 0u : alignment - remainder;
  size_t end;
  if (out_memory == NULL || padding > SIZE_MAX - arena->used) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  end = arena->used + padding;
  if (size > sizeof(arena->storage) - end) {
    return HXC_STATUS_OUT_OF_MEMORY;
  }
  *out_memory = &arena->storage[end];
  arena->used = end + size;
  return HXC_STATUS_OK;
}

static void gc_arena_release(
  void *context,
  void *memory,
  size_t size,
  size_t alignment
) {
  gc_arena *arena = (gc_arena *)context;
  (void)memory;
  (void)size;
  (void)alignment;
  arena->release_count++;
}

static void gc_pair_trace(
  const void *object,
  hxc_trace_visit_fn visit,
  void *context
) {
  const gc_pair *pair = (const gc_pair *)object;
  visit(context, pair->other);
}

static const hxc_type_descriptor gc_pair_descriptor = {
  HXC_TYPE_DESCRIPTOR_ABI_VERSION,
  HXC_TYPE_DESCRIPTOR_HAS_TRACE,
  sizeof(gc_pair),
  HXC_ALIGNOF(gc_pair),
  gc_pair_trace,
  NULL
};

static hxc_status gc_pair_allocate(hxc_gc *gc, gc_pair **out_pair) {
  void *object = NULL;
  hxc_status status = hxc_gc_allocate(gc, &gc_pair_descriptor, &object);
  if (status == HXC_STATUS_OK) {
    *out_pair = (gc_pair *)object;
  }
  return status;
}

int main(void) {
  gc_arena arena = { { 0u }, 0u, 0u };
  hxc_allocator allocator;
  hxc_gc gc = HXC_GC_INITIALIZER;
  hxc_gc_config config;
  hxc_gc_thread thread = HXC_GC_THREAD_INITIALIZER;
  hxc_gc_root_frame frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  const void *roots[1] = { NULL };
  gc_pair *left = NULL;
  gc_pair *right = NULL;

  allocator.context = &arena;
  allocator.allocate = gc_arena_allocate;
  allocator.reallocate = NULL;
  allocator.release = gc_arena_release;
  config.allocator = allocator;
  config.initial_threshold_bytes = sizeof(arena.storage);
  config.clock_now = NULL;
  config.clock_context = NULL;
  HXC_CHECK(hxc_gc_init(&config, &gc) == HXC_STATUS_OK);
  HXC_CHECK(hxc_gc_thread_register(&gc, &thread) == HXC_STATUS_OK);
  HXC_CHECK(hxc_gc_root_frame_push(&thread, roots, 1u, &frame) == HXC_STATUS_OK);
  HXC_CHECK(gc_pair_allocate(&gc, &left) == HXC_STATUS_OK);
  roots[0] = left;
  HXC_CHECK(gc_pair_allocate(&gc, &right) == HXC_STATUS_OK);
  left->other = right;
  right->other = left;
  roots[0] = NULL;
  HXC_CHECK(hxc_gc_collect(&gc) == HXC_STATUS_OK);
  HXC_CHECK(arena.release_count == 2u);
  HXC_CHECK(hxc_gc_root_frame_pop(&frame) == HXC_STATUS_OK);
  HXC_CHECK(hxc_gc_thread_unregister(&thread) == HXC_STATUS_OK);
  HXC_CHECK(hxc_gc_dispose(&gc) == HXC_STATUS_OK);
  (void)puts("gc-freestanding: OK");
  return 0;
}
