/*
 * Implementation of compiler-selectable feature `gc`.
 *
 * The collector is a precise, non-moving, stop-the-world mark-and-sweep
 * baseline. Roots and descriptor fields contain exact managed payload bases;
 * arbitrary stack bytes, interior pointers, and foreign allocations are never
 * scanned. Caller-owned collector/root records and an injected clock keep this
 * slice usable in hosted and freestanding builds without hidden global state.
 */
#include "hxrt/gc.h"

struct hxc_gc_allocation {
  hxc_gc_allocation *next;
  hxc_gc_allocation *mark_next;
  hxc_object_header object_header;
  size_t block_size;
  size_t block_alignment;
  size_t payload_offset;
  bool marked;
  bool finalized;
};

typedef struct hxc_gc_mark_context {
  hxc_gc *gc;
  hxc_gc_allocation *pending;
  hxc_status status;
  hxc_status unknown_pointer_status;
} hxc_gc_mark_context;

static bool hxc_gc_is_empty(const hxc_gc *gc) {
  return gc != NULL
    && gc->allocator.context == NULL
    && gc->allocator.allocate == NULL
    && gc->allocator.reallocate == NULL
    && gc->allocator.release == NULL
    && gc->allocations == NULL
    && gc->threads == NULL
    && gc->global_roots == NULL
    && gc->clock_now == NULL
    && gc->clock_context == NULL
    && gc->initial_threshold_bytes == 0u
    && gc->next_collection_bytes == 0u
    && !gc->initialized
    && !gc->collecting;
}

static void hxc_gc_clear(hxc_gc *gc) {
  hxc_gc empty = HXC_GC_INITIALIZER;
  *gc = empty;
}

static void hxc_gc_zero_bytes(void *memory, size_t size) {
  unsigned char *bytes = (unsigned char *)memory;
  size_t index;
  for (index = 0u; index < size; index++) {
    bytes[index] = 0u;
  }
}

static uint64_t hxc_gc_u64_increment(uint64_t value) {
  return value == UINT64_MAX ? UINT64_MAX : value + UINT64_C(1);
}

static uint64_t hxc_gc_u64_add_saturating(uint64_t left, uint64_t right) {
  return right > UINT64_MAX - left ? UINT64_MAX : left + right;
}

static size_t hxc_gc_size_add_saturating(size_t left, size_t right) {
  return right > SIZE_MAX - left ? SIZE_MAX : left + right;
}

static size_t hxc_gc_size_sub_exact(size_t left, size_t right) {
  return right > left ? 0u : left - right;
}

static size_t hxc_gc_payload_offset(size_t payload_alignment) {
  const size_t header_size = sizeof(hxc_gc_allocation);
  const size_t remainder = header_size % payload_alignment;
  return remainder == 0u
    ? header_size
    : header_size + (payload_alignment - remainder);
}

static void *hxc_gc_payload(hxc_gc_allocation *allocation) {
  return (unsigned char *)allocation + allocation->payload_offset;
}

static const void *hxc_gc_payload_const(const hxc_gc_allocation *allocation) {
  return (const unsigned char *)allocation + allocation->payload_offset;
}

static hxc_gc_allocation *hxc_gc_find_allocation(
  hxc_gc *gc,
  const void *object
) {
  hxc_gc_allocation *allocation = gc->allocations;
  while (allocation != NULL) {
    if (hxc_gc_payload_const(allocation) == object) {
      return allocation;
    }
    allocation = allocation->next;
  }
  return NULL;
}

static const hxc_gc_allocation *hxc_gc_find_allocation_const(
  const hxc_gc *gc,
  const void *object
) {
  const hxc_gc_allocation *allocation = gc->allocations;
  while (allocation != NULL) {
    if (hxc_gc_payload_const(allocation) == object) {
      return allocation;
    }
    allocation = allocation->next;
  }
  return NULL;
}

static void hxc_gc_clear_marks(hxc_gc *gc) {
  hxc_gc_allocation *allocation = gc->allocations;
  while (allocation != NULL) {
    allocation->marked = false;
    allocation->mark_next = NULL;
    allocation = allocation->next;
  }
}

static void hxc_gc_mark_exact(
  hxc_gc_mark_context *context,
  const void *object
) {
  hxc_gc_allocation *allocation;
  if (context->status != HXC_STATUS_OK || object == NULL) {
    return;
  }
  allocation = hxc_gc_find_allocation(context->gc, object);
  if (allocation == NULL) {
    context->status = context->unknown_pointer_status;
    return;
  }
  if (!allocation->marked) {
    allocation->marked = true;
    allocation->mark_next = context->pending;
    context->pending = allocation;
  }
}

static void hxc_gc_trace_visit(void *opaque_context, const void *object) {
  hxc_gc_mark_context *context = (hxc_gc_mark_context *)opaque_context;
  /* A descriptor that reports a non-base pointer violates an internal plan. */
  context->unknown_pointer_status = HXC_STATUS_INTERNAL_ERROR;
  hxc_gc_mark_exact(context, object);
}

static hxc_status hxc_gc_mark_slots(
  hxc_gc_mark_context *context,
  const void **slots,
  size_t slot_count
) {
  size_t index;
  if (slots == NULL || slot_count == 0u) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  context->unknown_pointer_status = HXC_STATUS_INVALID_ARGUMENT;
  for (index = 0u; index < slot_count; index++) {
    hxc_gc_mark_exact(context, slots[index]);
    if (context->status != HXC_STATUS_OK) {
      return context->status;
    }
  }
  return HXC_STATUS_OK;
}

static hxc_status hxc_gc_mark_roots(hxc_gc_mark_context *context) {
  hxc_gc_root_table *table = context->gc->global_roots;
  hxc_gc_thread *thread;
  while (table != NULL) {
    if (!table->registered || table->collector != context->gc) {
      return HXC_STATUS_INTERNAL_ERROR;
    }
    context->status = hxc_gc_mark_slots(context, table->slots, table->slot_count);
    if (context->status != HXC_STATUS_OK) {
      return context->status;
    }
    table = table->next;
  }
  thread = context->gc->threads;
  while (thread != NULL) {
    hxc_gc_root_frame *frame;
    if (!thread->registered || thread->collector != context->gc) {
      return HXC_STATUS_INTERNAL_ERROR;
    }
    frame = thread->top_frame;
    while (frame != NULL) {
      if (!frame->active || frame->thread != thread) {
        return HXC_STATUS_INTERNAL_ERROR;
      }
      context->status = hxc_gc_mark_slots(context, frame->slots, frame->slot_count);
      if (context->status != HXC_STATUS_OK) {
        return context->status;
      }
      frame = frame->previous;
    }
    thread = thread->next;
  }
  return HXC_STATUS_OK;
}

static hxc_status hxc_gc_trace_pending(hxc_gc_mark_context *context) {
  while (context->pending != NULL) {
    hxc_gc_allocation *allocation = context->pending;
    const hxc_type_descriptor *descriptor = allocation->object_header.descriptor;
    context->pending = allocation->mark_next;
    allocation->mark_next = NULL;
    context->unknown_pointer_status = HXC_STATUS_INTERNAL_ERROR;
    if (!hxc_type_descriptor_trace(
      descriptor,
      hxc_gc_payload(allocation),
      hxc_gc_trace_visit,
      context
    )) {
      return HXC_STATUS_INTERNAL_ERROR;
    }
    if (context->status != HXC_STATUS_OK) {
      return context->status;
    }
  }
  return HXC_STATUS_OK;
}

static void hxc_gc_record_pause(
  hxc_gc *gc,
  bool measured,
  uint64_t started_at
) {
  uint64_t finished_at;
  uint64_t elapsed;
  if (!measured) {
    return;
  }
  finished_at = gc->clock_now(gc->clock_context);
  if (finished_at < started_at) {
    gc->stats.clock_regression_count = hxc_gc_u64_increment(
      gc->stats.clock_regression_count
    );
    elapsed = UINT64_C(0);
  } else {
    elapsed = finished_at - started_at;
  }
  gc->stats.pause_sample_count = hxc_gc_u64_increment(
    gc->stats.pause_sample_count
  );
  gc->stats.last_pause_ticks = elapsed;
  gc->stats.total_pause_ticks = hxc_gc_u64_add_saturating(
    gc->stats.total_pause_ticks,
    elapsed
  );
  if (elapsed > gc->stats.maximum_pause_ticks) {
    gc->stats.maximum_pause_ticks = elapsed;
  }
}

static void hxc_gc_update_threshold(hxc_gc *gc) {
  size_t doubled_live;
  if (gc->stats.live_payload_bytes > SIZE_MAX / 2u) {
    doubled_live = SIZE_MAX;
  } else {
    doubled_live = gc->stats.live_payload_bytes * 2u;
  }
  gc->next_collection_bytes = doubled_live < gc->initial_threshold_bytes
    ? gc->initial_threshold_bytes
    : doubled_live;
}

static hxc_status hxc_gc_release_allocation(
  hxc_gc *gc,
  hxc_gc_allocation *allocation
) {
  const hxc_type_descriptor *descriptor = allocation->object_header.descriptor;
  const size_t payload_size = descriptor->object_size;
  const size_t overhead = allocation->block_size - payload_size;
  hxc_status status = HXC_STATUS_OK;
  if (!allocation->finalized) {
    allocation->finalized = true;
    if (!hxc_type_descriptor_finalize(descriptor, hxc_gc_payload(allocation))) {
      status = HXC_STATUS_INTERNAL_ERROR;
    }
  }
  if (hxc_free(
    &gc->allocator,
    allocation,
    allocation->block_size,
    allocation->block_alignment
  ) != HXC_STATUS_OK) {
    status = HXC_STATUS_INTERNAL_ERROR;
  }
  gc->stats.current_object_count = hxc_gc_size_sub_exact(
    gc->stats.current_object_count,
    1u
  );
  gc->stats.live_payload_bytes = hxc_gc_size_sub_exact(
    gc->stats.live_payload_bytes,
    payload_size
  );
  gc->stats.internal_overhead_bytes = hxc_gc_size_sub_exact(
    gc->stats.internal_overhead_bytes,
    overhead
  );
  gc->stats.reclaimed_object_count = hxc_gc_u64_increment(
    gc->stats.reclaimed_object_count
  );
  gc->stats.reclaimed_payload_bytes = hxc_gc_size_add_saturating(
    gc->stats.reclaimed_payload_bytes,
    payload_size
  );
  return status;
}

hxc_status hxc_gc_init(const hxc_gc_config *config, hxc_gc *out_gc) {
  hxc_gc initialized = HXC_GC_INITIALIZER;
  if (
    config == NULL
    || out_gc == NULL
    || !hxc_gc_is_empty(out_gc)
    || !hxc_allocator_is_valid(&config->allocator)
    || config->initial_threshold_bytes == 0u
  ) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  initialized.allocator = config->allocator;
  initialized.clock_now = config->clock_now;
  initialized.clock_context = config->clock_context;
  initialized.initial_threshold_bytes = config->initial_threshold_bytes;
  initialized.next_collection_bytes = config->initial_threshold_bytes;
  initialized.initialized = true;
  *out_gc = initialized;
  return HXC_STATUS_OK;
}

hxc_status hxc_gc_dispose(hxc_gc *gc) {
  hxc_status result = HXC_STATUS_OK;
  if (
    gc == NULL
    || !gc->initialized
    || gc->collecting
    || gc->threads != NULL
    || gc->global_roots != NULL
  ) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  while (gc->allocations != NULL) {
    hxc_gc_allocation *allocation = gc->allocations;
    gc->allocations = allocation->next;
    if (hxc_gc_release_allocation(gc, allocation) != HXC_STATUS_OK) {
      result = HXC_STATUS_INTERNAL_ERROR;
    }
  }
  hxc_gc_clear(gc);
  return result;
}

hxc_status hxc_gc_allocate(
  hxc_gc *gc,
  const hxc_type_descriptor *descriptor,
  void **out_object
) {
  size_t payload_offset;
  size_t block_size;
  size_t block_alignment;
  size_t projected_live;
  void *memory = NULL;
  hxc_gc_allocation *allocation;
  hxc_status status;
  if (
    gc == NULL
    || !gc->initialized
    || gc->collecting
    || out_object == NULL
    || !hxc_type_descriptor_is_valid(descriptor)
  ) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  status = hxc_size_add(
    gc->stats.live_payload_bytes,
    descriptor->object_size,
    &projected_live
  );
  if (status != HXC_STATUS_OK) {
    return status;
  }
  if (gc->allocations != NULL && projected_live > gc->next_collection_bytes) {
    gc->stats.pressure_collection_count = hxc_gc_u64_increment(
      gc->stats.pressure_collection_count
    );
    status = hxc_gc_collect(gc);
    if (status != HXC_STATUS_OK) {
      return status;
    }
  }
  payload_offset = hxc_gc_payload_offset(descriptor->object_alignment);
  status = hxc_size_add(payload_offset, descriptor->object_size, &block_size);
  if (status != HXC_STATUS_OK) {
    return status;
  }
  block_alignment = descriptor->object_alignment > HXC_ALIGNOF(hxc_gc_allocation)
    ? descriptor->object_alignment
    : HXC_ALIGNOF(hxc_gc_allocation);
  status = hxc_alloc(&gc->allocator, block_size, block_alignment, &memory);
  if (status != HXC_STATUS_OK) {
    return status;
  }
  allocation = (hxc_gc_allocation *)memory;
  allocation->next = gc->allocations;
  allocation->mark_next = NULL;
  allocation->block_size = block_size;
  allocation->block_alignment = block_alignment;
  allocation->payload_offset = payload_offset;
  allocation->marked = false;
  allocation->finalized = false;
  if (!hxc_object_header_init(descriptor, &allocation->object_header)) {
    (void)hxc_free(&gc->allocator, memory, block_size, block_alignment);
    return HXC_STATUS_INTERNAL_ERROR;
  }
  hxc_gc_zero_bytes(hxc_gc_payload(allocation), descriptor->object_size);
  gc->allocations = allocation;
  gc->stats.allocation_count = hxc_gc_u64_increment(
    gc->stats.allocation_count
  );
  gc->stats.current_object_count = hxc_gc_size_add_saturating(
    gc->stats.current_object_count,
    1u
  );
  gc->stats.peak_object_count = gc->stats.current_object_count > gc->stats.peak_object_count
    ? gc->stats.current_object_count
    : gc->stats.peak_object_count;
  gc->stats.allocated_payload_bytes = hxc_gc_size_add_saturating(
    gc->stats.allocated_payload_bytes,
    descriptor->object_size
  );
  gc->stats.live_payload_bytes = hxc_gc_size_add_saturating(
    gc->stats.live_payload_bytes,
    descriptor->object_size
  );
  gc->stats.peak_live_payload_bytes = gc->stats.live_payload_bytes > gc->stats.peak_live_payload_bytes
    ? gc->stats.live_payload_bytes
    : gc->stats.peak_live_payload_bytes;
  gc->stats.internal_overhead_bytes = hxc_gc_size_add_saturating(
    gc->stats.internal_overhead_bytes,
    block_size - descriptor->object_size
  );
  *out_object = hxc_gc_payload(allocation);
  return HXC_STATUS_OK;
}

hxc_status hxc_gc_collect(hxc_gc *gc) {
  hxc_gc_mark_context context;
  hxc_gc_allocation **link;
  hxc_status status;
  hxc_status sweep_status = HXC_STATUS_OK;
  const bool measured = gc != NULL
    && gc->initialized
    && gc->clock_now != NULL;
  const uint64_t started_at = measured
    ? gc->clock_now(gc->clock_context)
    : UINT64_C(0);
  if (gc == NULL || !gc->initialized || gc->collecting) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  gc->collecting = true;
  hxc_gc_clear_marks(gc);
  context.gc = gc;
  context.pending = NULL;
  context.status = HXC_STATUS_OK;
  context.unknown_pointer_status = HXC_STATUS_INVALID_ARGUMENT;
  status = hxc_gc_mark_roots(&context);
  if (status == HXC_STATUS_OK) {
    status = hxc_gc_trace_pending(&context);
  }
  if (status != HXC_STATUS_OK) {
    hxc_gc_clear_marks(gc);
    gc->collecting = false;
    hxc_gc_record_pause(gc, measured, started_at);
    return status;
  }

  link = &gc->allocations;
  while (*link != NULL) {
    hxc_gc_allocation *allocation = *link;
    if (allocation->marked) {
      allocation->marked = false;
      allocation->mark_next = NULL;
      link = &allocation->next;
    } else {
      *link = allocation->next;
      if (hxc_gc_release_allocation(gc, allocation) != HXC_STATUS_OK) {
        sweep_status = HXC_STATUS_INTERNAL_ERROR;
      }
    }
  }
  gc->stats.collection_count = hxc_gc_u64_increment(
    gc->stats.collection_count
  );
  hxc_gc_update_threshold(gc);
  gc->collecting = false;
  hxc_gc_record_pause(gc, measured, started_at);
  return sweep_status;
}

hxc_status hxc_gc_safepoint(hxc_gc *gc) {
  if (gc == NULL || !gc->initialized || gc->collecting) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  if (
    gc->allocations != NULL
    && gc->stats.live_payload_bytes >= gc->next_collection_bytes
  ) {
    gc->stats.pressure_collection_count = hxc_gc_u64_increment(
      gc->stats.pressure_collection_count
    );
    return hxc_gc_collect(gc);
  }
  return HXC_STATUS_OK;
}

bool hxc_gc_owns_exact(const hxc_gc *gc, const void *object) {
  return gc != NULL
    && gc->initialized
    && object != NULL
    && hxc_gc_find_allocation_const(gc, object) != NULL;
}

hxc_status hxc_gc_get_stats(
  const hxc_gc *gc,
  hxc_gc_stats *out_stats
) {
  if (gc == NULL || !gc->initialized || out_stats == NULL) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  *out_stats = gc->stats;
  return HXC_STATUS_OK;
}

hxc_status hxc_gc_thread_register(
  hxc_gc *gc,
  hxc_gc_thread *thread
) {
  if (
    gc == NULL
    || !gc->initialized
    || gc->collecting
    || thread == NULL
    || thread->registered
    || thread->collector != NULL
    || thread->next != NULL
    || thread->top_frame != NULL
  ) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  thread->collector = gc;
  thread->next = gc->threads;
  thread->registered = true;
  gc->threads = thread;
  return HXC_STATUS_OK;
}

hxc_status hxc_gc_thread_unregister(hxc_gc_thread *thread) {
  hxc_gc *gc;
  hxc_gc_thread **link;
  if (
    thread == NULL
    || !thread->registered
    || thread->collector == NULL
    || thread->top_frame != NULL
  ) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  gc = thread->collector;
  if (!gc->initialized || gc->collecting) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  link = &gc->threads;
  while (*link != NULL && *link != thread) {
    link = &(*link)->next;
  }
  if (*link != thread) {
    return HXC_STATUS_INTERNAL_ERROR;
  }
  *link = thread->next;
  thread->collector = NULL;
  thread->next = NULL;
  thread->registered = false;
  return HXC_STATUS_OK;
}

hxc_status hxc_gc_root_frame_push(
  hxc_gc_thread *thread,
  const void **slots,
  size_t slot_count,
  hxc_gc_root_frame *frame
) {
  if (
    thread == NULL
    || !thread->registered
    || thread->collector == NULL
    || thread->collector->collecting
    || slots == NULL
    || slot_count == 0u
    || frame == NULL
    || frame->active
    || frame->thread != NULL
    || frame->previous != NULL
    || frame->slots != NULL
    || frame->slot_count != 0u
  ) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  frame->thread = thread;
  frame->previous = thread->top_frame;
  frame->slots = slots;
  frame->slot_count = slot_count;
  frame->active = true;
  thread->top_frame = frame;
  return HXC_STATUS_OK;
}

hxc_status hxc_gc_root_frame_pop(hxc_gc_root_frame *frame) {
  hxc_gc_thread *thread;
  if (
    frame == NULL
    || !frame->active
    || frame->thread == NULL
    || frame->slots == NULL
    || frame->slot_count == 0u
  ) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  thread = frame->thread;
  if (
    !thread->registered
    || thread->collector == NULL
    || thread->collector->collecting
    || thread->top_frame != frame
  ) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  thread->top_frame = frame->previous;
  frame->thread = NULL;
  frame->previous = NULL;
  frame->slots = NULL;
  frame->slot_count = 0u;
  frame->active = false;
  return HXC_STATUS_OK;
}

hxc_status hxc_gc_root_table_register(
  hxc_gc *gc,
  const void **slots,
  size_t slot_count,
  hxc_gc_root_table *table
) {
  if (
    gc == NULL
    || !gc->initialized
    || gc->collecting
    || slots == NULL
    || slot_count == 0u
    || table == NULL
    || table->registered
    || table->collector != NULL
    || table->next != NULL
    || table->slots != NULL
    || table->slot_count != 0u
  ) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  table->collector = gc;
  table->next = gc->global_roots;
  table->slots = slots;
  table->slot_count = slot_count;
  table->registered = true;
  gc->global_roots = table;
  return HXC_STATUS_OK;
}

hxc_status hxc_gc_root_table_unregister(hxc_gc_root_table *table) {
  hxc_gc *gc;
  hxc_gc_root_table **link;
  if (
    table == NULL
    || !table->registered
    || table->collector == NULL
    || table->slots == NULL
    || table->slot_count == 0u
  ) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  gc = table->collector;
  if (!gc->initialized || gc->collecting) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  link = &gc->global_roots;
  while (*link != NULL && *link != table) {
    link = &(*link)->next;
  }
  if (*link != table) {
    return HXC_STATUS_INTERNAL_ERROR;
  }
  *link = table->next;
  table->collector = NULL;
  table->next = NULL;
  table->slots = NULL;
  table->slot_count = 0u;
  table->registered = false;
  return HXC_STATUS_OK;
}

hxc_status hxc_gc_pin_object(
  hxc_gc *gc,
  const void *object,
  hxc_gc_pin *pin
) {
  hxc_status status;
  if (
    gc == NULL
    || !gc->initialized
    || object == NULL
    || !hxc_gc_owns_exact(gc, object)
    || pin == NULL
    || pin->active
    || pin->object != NULL
  ) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  pin->object = object;
  status = hxc_gc_root_table_register(gc, &pin->object, 1u, &pin->roots);
  if (status != HXC_STATUS_OK) {
    pin->object = NULL;
    return status;
  }
  pin->active = true;
  return HXC_STATUS_OK;
}

hxc_status hxc_gc_unpin_object(hxc_gc_pin *pin) {
  hxc_status status;
  if (pin == NULL || !pin->active || pin->object == NULL) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  status = hxc_gc_root_table_unregister(&pin->roots);
  if (status != HXC_STATUS_OK) {
    return status;
  }
  pin->object = NULL;
  pin->active = false;
  return HXC_STATUS_OK;
}
