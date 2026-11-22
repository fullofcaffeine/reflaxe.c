#include "hxrt/thread_gc.h"

#include <sched.h>
#include <stdatomic.h>

typedef struct worker_fixture {
  hxc_gc_thread_coordinator *coordinator;
  const void *object;
  atomic_int phase;
  atomic_bool clear_root;
  atomic_bool stop;
  bool passed;
} worker_fixture;

static const hxc_type_descriptor LEAF_DESCRIPTOR = {
  HXC_TYPE_DESCRIPTOR_ABI_VERSION,
  0u,
  sizeof(int),
  _Alignof(int),
  NULL,
  NULL
};

static void *root_worker(void *context) {
  worker_fixture *fixture = (worker_fixture *)context;
  hxc_gc_thread_token token = {0};
  hxc_gc_root_frame frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  const void *volatile slots[1] = {fixture->object};

  if (hxc_gc_thread_token_register(fixture->coordinator, &token) != HXC_STATUS_OK
      || hxc_gc_root_frame_push(&token.roots, slots, 1u, &frame) != HXC_STATUS_OK) {
    return NULL;
  }
  atomic_store_explicit(&fixture->phase, 1, memory_order_release);
  while (!atomic_load_explicit(&fixture->stop, memory_order_acquire)) {
    if (atomic_load_explicit(&fixture->clear_root, memory_order_acquire) && slots[0] != NULL) {
      slots[0] = NULL;
      atomic_store_explicit(&fixture->phase, 2, memory_order_release);
    }
    if (hxc_gc_thread_safepoint(&token) != HXC_STATUS_OK) {
      return NULL;
    }
    (void)sched_yield();
  }
  if (hxc_gc_root_frame_pop(&frame) != HXC_STATUS_OK
      || hxc_gc_thread_token_unregister(&token) != HXC_STATUS_OK) {
    return NULL;
  }
  fixture->passed = true;
  return fixture;
}

int main(void) {
  hxc_gc gc = HXC_GC_INITIALIZER;
  hxc_gc_config config = {
    .allocator = hxc_default_allocator(),
    .initial_threshold_bytes = 64u,
    .clock_now = NULL,
    .clock_context = NULL
  };
  hxc_gc_thread_coordinator coordinator = {0};
  hxc_gc_thread_token main_token = {0};
  worker_fixture fixture = {0};
  hxc_gc_stats stats = {0};
  pthread_t worker;
  void *object = NULL;
  void *worker_result = NULL;

  if (hxc_gc_init(&config, &gc) != HXC_STATUS_OK
      || hxc_gc_thread_coordinator_init(&gc, &coordinator) != HXC_STATUS_OK
      || hxc_gc_thread_token_register(&coordinator, &main_token) != HXC_STATUS_OK
      || hxc_gc_thread_allocate(&main_token, &LEAF_DESCRIPTOR, &object) != HXC_STATUS_OK) {
    return 1;
  }
  fixture.coordinator = &coordinator;
  fixture.object = object;
  atomic_init(&fixture.phase, 0);
  atomic_init(&fixture.clear_root, false);
  atomic_init(&fixture.stop, false);
  if (pthread_create(&worker, NULL, root_worker, &fixture) != 0) {
    return 2;
  }
  while (atomic_load_explicit(&fixture.phase, memory_order_acquire) < 1) {
    (void)sched_yield();
  }
  object = NULL;
  if (hxc_gc_thread_collect(&main_token) != HXC_STATUS_OK
      || hxc_gc_get_stats(&gc, &stats) != HXC_STATUS_OK
      || stats.current_object_count != 1u) {
    return 3;
  }

  atomic_store_explicit(&fixture.clear_root, true, memory_order_release);
  while (atomic_load_explicit(&fixture.phase, memory_order_acquire) < 2) {
    (void)sched_yield();
  }
  if (hxc_gc_thread_collect(&main_token) != HXC_STATUS_OK
      || hxc_gc_get_stats(&gc, &stats) != HXC_STATUS_OK
      || stats.current_object_count != 0u) {
    return 4;
  }

  atomic_store_explicit(&fixture.stop, true, memory_order_release);
  if (pthread_join(worker, &worker_result) != 0 || worker_result != &fixture || !fixture.passed
      || hxc_gc_thread_token_unregister(&main_token) != HXC_STATUS_OK
      || hxc_gc_thread_coordinator_dispose(&coordinator) != HXC_STATUS_OK
      || hxc_gc_dispose(&gc) != HXC_STATUS_OK) {
    return 5;
  }
  return 0;
}
