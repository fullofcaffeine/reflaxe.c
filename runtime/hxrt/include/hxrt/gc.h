/*
 * hxrt feature: gc.
 *
 * This is the selected precise, non-moving, mark-and-sweep backend from ADR
 * 0005. The compiler registers exact managed base pointers in explicit global
 * tables and stack frames; the collector never guesses roots by scanning raw
 * stack bytes. Payload addresses stay stable until collection proves them
 * unreachable.
 *
 * The records below are internal ABI, not application-export types. Operations
 * are request-local and contain no process-global state. The current runtime
 * has no thread feature, so callers must invoke every operation while managed
 * execution is stopped. Several thread root chains can already be registered
 * and traced; E5.T11 will own synchronization and stop-the-world coordination.
 */
#ifndef HXRT_GC_H_INCLUDED
#define HXRT_GC_H_INCLUDED

#include "hxrt/allocator.h"
#include "hxrt/object.h"

#if defined(__cplusplus)
extern "C" {
#endif

typedef struct hxc_gc hxc_gc;
typedef struct hxc_gc_thread hxc_gc_thread;
typedef struct hxc_gc_root_frame hxc_gc_root_frame;
typedef struct hxc_gc_root_table hxc_gc_root_table;
typedef struct hxc_gc_pin hxc_gc_pin;
typedef struct hxc_gc_allocation hxc_gc_allocation;

/**
 * Return one monotonically increasing application-defined tick value.
 *
 * The runtime does not assume nanoseconds because freestanding targets may
 * have no standard clock. Reports retain raw ticks; the embedding build owns
 * and documents their unit. A null callback explicitly means that pause timing
 * is unavailable, rather than silently substituting a wall clock.
 */
typedef uint64_t (*hxc_gc_clock_fn)(void *context);

typedef struct hxc_gc_config {
  hxc_allocator allocator;
  size_t initial_threshold_bytes;
  hxc_gc_clock_fn clock_now;
  void *clock_context;
} hxc_gc_config;

/** Observable counters; collector-private links and marks remain hidden. */
typedef struct hxc_gc_stats {
  uint64_t allocation_count;
  uint64_t collection_count;
  uint64_t pressure_collection_count;
  uint64_t reclaimed_object_count;
  uint64_t pause_sample_count;
  uint64_t last_pause_ticks;
  uint64_t total_pause_ticks;
  uint64_t maximum_pause_ticks;
  uint64_t clock_regression_count;
  size_t current_object_count;
  size_t peak_object_count;
  size_t allocated_payload_bytes;
  size_t live_payload_bytes;
  size_t peak_live_payload_bytes;
  size_t reclaimed_payload_bytes;
  size_t internal_overhead_bytes;
} hxc_gc_stats;

/**
 * One caller-owned chain representing a participating execution thread.
 *
 * Registration alone does not start or synchronize an operating-system thread.
 * It gives the collector one exact stack-root chain to inspect while the
 * embedding runtime has stopped managed execution.
 */
struct hxc_gc_thread {
  hxc_gc *collector;
  hxc_gc_thread *next;
  hxc_gc_root_frame *top_frame;
  bool registered;
};

/**
 * One lexical stack frame containing compiler-maintained managed base values.
 *
 * `slots` points at an array of `const void *` values. Generated code updates
 * those values when locals change. This avoids the invalid and alias-unsafe
 * conversion from a typed `T **` to `void **`.
 */
struct hxc_gc_root_frame {
  hxc_gc_thread *thread;
  hxc_gc_root_frame *previous;
  const void **slots;
  size_t slot_count;
  bool active;
};

/** A caller-owned exact table for mutable managed global slots. */
struct hxc_gc_root_table {
  hxc_gc *collector;
  hxc_gc_root_table *next;
  const void **slots;
  size_t slot_count;
  bool registered;
};

/**
 * A foreign-retention handle for exactly one managed base pointer.
 *
 * Foreign or interior pointers do not keep an allocation alive. Native code
 * that retains a managed payload across a safepoint keeps this handle live and
 * releases it when the foreign retention ends.
 */
struct hxc_gc_pin {
  hxc_gc_root_table roots;
  const void *object;
  bool active;
};

/**
 * Request-local collector state.
 *
 * The allocation node is intentionally incomplete here: links, marks, and
 * finalization state are backend details. The remaining fields make lifecycle,
 * registered roots, thresholds, and reports inspectable without global state.
 */
struct hxc_gc {
  hxc_allocator allocator;
  hxc_gc_allocation *allocations;
  hxc_gc_thread *threads;
  hxc_gc_root_table *global_roots;
  hxc_gc_clock_fn clock_now;
  void *clock_context;
  size_t initial_threshold_bytes;
  size_t next_collection_bytes;
  hxc_gc_stats stats;
  bool initialized;
  bool collecting;
};

#define HXC_GC_THREAD_INITIALIZER \
  { NULL, NULL, NULL, false }
#define HXC_GC_ROOT_FRAME_INITIALIZER \
  { NULL, NULL, NULL, 0u, false }
#define HXC_GC_ROOT_TABLE_INITIALIZER \
  { NULL, NULL, NULL, 0u, false }
#define HXC_GC_PIN_INITIALIZER \
  { HXC_GC_ROOT_TABLE_INITIALIZER, NULL, false }
#define HXC_GC_STATS_INITIALIZER \
  { 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u }
#define HXC_GC_INITIALIZER \
  { { NULL, NULL, NULL, NULL }, NULL, NULL, NULL, NULL, NULL, 0u, 0u, \
    HXC_GC_STATS_INITIALIZER, false, false }

/** Initialize an empty collector. The output must equal HXC_GC_INITIALIZER. */
HXC_API hxc_status hxc_gc_init(const hxc_gc_config *config, hxc_gc *out_gc);

/**
 * Finalize and release every remaining payload, then clear the collector.
 * All root tables, frames, threads, and pins must already be unregistered.
 */
HXC_API hxc_status hxc_gc_dispose(hxc_gc *gc);

/** Allocate one zero-initialized stable payload described by `descriptor`. */
HXC_API hxc_status hxc_gc_allocate(
  hxc_gc *gc,
  const hxc_type_descriptor *descriptor,
  void **out_object
);

/** Force one exact mark-and-sweep collection. */
HXC_API hxc_status hxc_gc_collect(hxc_gc *gc);

/** Collect only when the deterministic allocation-pressure threshold is due. */
HXC_API hxc_status hxc_gc_safepoint(hxc_gc *gc);

/** True only for the exact base address of a currently live managed payload. */
HXC_API bool hxc_gc_owns_exact(const hxc_gc *gc, const void *object);

/** Copy the current immutable report snapshot. */
HXC_API hxc_status hxc_gc_get_stats(
  const hxc_gc *gc,
  hxc_gc_stats *out_stats
);

/** Register/unregister one stopped-thread root chain. */
HXC_API hxc_status hxc_gc_thread_register(
  hxc_gc *gc,
  hxc_gc_thread *thread
);
HXC_API hxc_status hxc_gc_thread_unregister(hxc_gc_thread *thread);

/** Push/pop a lexical root frame in strict last-in, first-out order. */
HXC_API hxc_status hxc_gc_root_frame_push(
  hxc_gc_thread *thread,
  const void **slots,
  size_t slot_count,
  hxc_gc_root_frame *frame
);
HXC_API hxc_status hxc_gc_root_frame_pop(hxc_gc_root_frame *frame);

/** Register/unregister a mutable exact global-root table. */
HXC_API hxc_status hxc_gc_root_table_register(
  hxc_gc *gc,
  const void **slots,
  size_t slot_count,
  hxc_gc_root_table *table
);
HXC_API hxc_status hxc_gc_root_table_unregister(hxc_gc_root_table *table);

/** Pin/unpin one exact managed base for retention by foreign native code. */
HXC_API hxc_status hxc_gc_pin_object(
  hxc_gc *gc,
  const void *object,
  hxc_gc_pin *pin
);
HXC_API hxc_status hxc_gc_unpin_object(hxc_gc_pin *pin);

#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif /* HXRT_GC_H_INCLUDED */
