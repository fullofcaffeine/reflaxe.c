/*
 * Cross-feature hxrt stress and deterministic allocation-failure contract.
 *
 * Focused feature fixtures prove each API in detail. This executable proves
 * that their shared allocator, collector, and exception cleanup contracts
 * remain compatible under one reproducible seed and limit. Every injected
 * failure is observed before publication, and every temporary owner is
 * released before the next case starts.
 */
#include <inttypes.h>
#include <setjmp.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include "hxrt/abi.h"
#include "hxrt/allocator.h"
#include "hxrt/array.h"
#include "hxrt/dynamic.h"
#include "hxrt/exception.h"
#include "hxrt/gc.h"
#include "hxrt/object.h"
#include "hxrt/string.h"

#if !defined(HXC_STRESS_SEED)
#define HXC_STRESS_SEED UINT32_C(0x5EED1234)
#endif

#if !defined(HXC_STRESS_LIMIT)
#define HXC_STRESS_LIMIT 512u
#endif

#define HXC_STRESS_SITE_ALLOCATE UINT32_C(1)
#define HXC_STRESS_SITE_REALLOCATE UINT32_C(2)
#define HXC_STRESS_NO_FAILURE SIZE_MAX

typedef struct stress_allocator_state {
  hxc_allocator backing;
  size_t fail_at;
  size_t attempt_count;
  size_t live_blocks;
  uint32_t site_mask;
  bool injected_failure;
  bool release_underflow;
} stress_allocator_state;

typedef hxc_status (*stress_case_fn)(stress_allocator_state *state);

typedef struct stress_node {
  struct stress_node *next;
  uint32_t value;
} stress_node;

static const char *stress_phase = "startup";
static size_t stress_string_attempts = 0u;
static size_t stress_array_attempts = 0u;
static size_t stress_gc_attempts = 0u;
static uint32_t stress_random_state = HXC_STRESS_SEED;
static int32_t stress_cleanup_trace[4] = { 0, 0, 0, 0 };
static size_t stress_cleanup_count = 0u;

#define HXC_STRESS_CHECK(condition) \
  do { \
    if (!(condition)) { \
      fprintf( \
        stderr, \
        "runtime-stress: FAIL phase=%s line=%d seed=%" PRIu32 \
        " limit=%u abi=%u.%u.%u\n", \
        stress_phase, \
        __LINE__, \
        (uint32_t)HXC_STRESS_SEED, \
        (unsigned int)HXC_STRESS_LIMIT, \
        (unsigned int)HXC_RUNTIME_ABI_MAJOR, \
        (unsigned int)HXC_RUNTIME_ABI_MINOR, \
        (unsigned int)HXC_RUNTIME_ABI_PATCH \
      ); \
      return false; \
    } \
  } while (false)

static uint32_t stress_random_next(void) {
  uint32_t value = stress_random_state;
  value ^= value << 13u;
  value ^= value >> 17u;
  value ^= value << 5u;
  stress_random_state = value;
  return value;
}

static bool stress_should_fail(
  stress_allocator_state *state,
  uint32_t site
) {
  const size_t attempt = state->attempt_count;
  state->attempt_count++;
  state->site_mask |= site;
  if (attempt == state->fail_at) {
    state->injected_failure = true;
    return true;
  }
  return false;
}

static hxc_status stress_allocate(
  void *context,
  size_t size,
  size_t alignment,
  void **out_memory
) {
  stress_allocator_state *state = (stress_allocator_state *)context;
  hxc_status status;
  if (stress_should_fail(state, HXC_STRESS_SITE_ALLOCATE)) {
    return HXC_STATUS_OUT_OF_MEMORY;
  }
  status = state->backing.allocate(
    state->backing.context,
    size,
    alignment,
    out_memory
  );
  if (status == HXC_STATUS_OK) {
    state->live_blocks++;
  }
  return status;
}

static hxc_status stress_reallocate(
  void *context,
  void *memory,
  size_t old_size,
  size_t new_size,
  size_t alignment,
  void **out_memory
) {
  stress_allocator_state *state = (stress_allocator_state *)context;
  if (stress_should_fail(state, HXC_STRESS_SITE_REALLOCATE)) {
    return HXC_STATUS_OUT_OF_MEMORY;
  }
  if (state->backing.reallocate == NULL) {
    return HXC_STATUS_INTERNAL_ERROR;
  }
  return state->backing.reallocate(
    state->backing.context,
    memory,
    old_size,
    new_size,
    alignment,
    out_memory
  );
}

static void stress_release(
  void *context,
  void *memory,
  size_t size,
  size_t alignment
) {
  stress_allocator_state *state = (stress_allocator_state *)context;
  if (state->live_blocks == 0u) {
    state->release_underflow = true;
  } else {
    state->live_blocks--;
  }
  state->backing.release(state->backing.context, memory, size, alignment);
}

static bool stress_allocator_init(
  size_t fail_at,
  stress_allocator_state *state,
  hxc_allocator *out_allocator
) {
  const hxc_allocator backing = hxc_default_allocator();
  if (!hxc_allocator_is_valid(&backing) || backing.reallocate == NULL) {
    return false;
  }
  state->backing = backing;
  state->fail_at = fail_at;
  state->attempt_count = 0u;
  state->live_blocks = 0u;
  state->site_mask = 0u;
  state->injected_failure = false;
  state->release_underflow = false;
  out_allocator->context = state;
  out_allocator->allocate = stress_allocate;
  out_allocator->reallocate = stress_reallocate;
  out_allocator->release = stress_release;
  return true;
}

static hxc_status stress_string_case(stress_allocator_state *state) {
  hxc_allocator allocator;
  hxc_string_buffer buffer = HXC_STRING_BUFFER_INITIALIZER;
  hxc_owned_string output = HXC_OWNED_STRING_INITIALIZER;
  hxc_status status;
  size_t index;
  if (!stress_allocator_init(state->fail_at, state, &allocator)) {
    return HXC_STATUS_INTERNAL_ERROR;
  }
  status = hxc_string_buffer_init(&allocator, &buffer);
  if (status != HXC_STATUS_OK) {
    return status;
  }
  for (index = 0u; index < 96u; index++) {
    const uint32_t scalar = UINT32_C(0x20) + (uint32_t)(index % 80u);
    status = hxc_string_buffer_append_scalar(&buffer, scalar);
    if (status != HXC_STATUS_OK) {
      (void)hxc_string_buffer_dispose(&buffer);
      return status;
    }
  }
  status = hxc_string_buffer_finish(&buffer, &output);
  if (status != HXC_STATUS_OK) {
    (void)hxc_string_buffer_dispose(&buffer);
    return status;
  }
  if (output.value.byte_length != 96u) {
    (void)hxc_owned_string_dispose(&output);
    return HXC_STATUS_INTERNAL_ERROR;
  }
  status = hxc_owned_string_dispose(&output);
  return status;
}

static hxc_status stress_array_case(stress_allocator_state *state) {
  hxc_allocator allocator;
  hxc_array array = HXC_ARRAY_INITIALIZER;
  hxc_array_element_ops elements;
  hxc_status status;
  size_t index;
  if (!stress_allocator_init(state->fail_at, state, &allocator)) {
    return HXC_STATUS_INTERNAL_ERROR;
  }
  elements.size = sizeof(uint32_t);
  elements.alignment = _Alignof(uint32_t);
  elements.context = NULL;
  elements.copy = NULL;
  elements.assign = NULL;
  elements.destroy = NULL;
  status = hxc_array_init(&allocator, elements, &array);
  if (status != HXC_STATUS_OK) {
    return status;
  }
  for (index = 0u; index < 128u; index++) {
    const uint32_t value = (uint32_t)index ^ HXC_STRESS_SEED;
    status = hxc_array_push_copy(&array, &value);
    if (status != HXC_STATUS_OK) {
      (void)hxc_array_dispose(&array);
      return status;
    }
  }
  for (index = 0u; index < 32u; index++) {
    const size_t remaining = array.length;
    const size_t selected = (size_t)(stress_random_next() % (uint32_t)remaining);
    status = hxc_array_remove_at(&array, selected);
    if (status != HXC_STATUS_OK) {
      (void)hxc_array_dispose(&array);
      return status;
    }
  }
  if (array.length != 96u) {
    (void)hxc_array_dispose(&array);
    return HXC_STATUS_INTERNAL_ERROR;
  }
  return hxc_array_dispose(&array);
}

static void stress_node_trace(
  const void *object,
  hxc_trace_visit_fn visit,
  void *visit_context
) {
  const stress_node *node = (const stress_node *)object;
  if (node->next != NULL) {
    visit(visit_context, node->next);
  }
}

static const hxc_type_descriptor stress_node_descriptor = {
  HXC_TYPE_DESCRIPTOR_ABI_VERSION,
  HXC_TYPE_DESCRIPTOR_HAS_TRACE,
  sizeof(stress_node),
  _Alignof(stress_node),
  stress_node_trace,
  NULL
};

static hxc_status stress_gc_build(
  stress_allocator_state *state,
  size_t node_count,
  bool collect_cycle
) {
  hxc_allocator allocator;
  hxc_gc gc = HXC_GC_INITIALIZER;
  hxc_gc_config config;
  hxc_gc_root_table roots = HXC_GC_ROOT_TABLE_INITIALIZER;
  const void *root_slots[1] = { NULL };
  stress_node *first = NULL;
  stress_node *previous = NULL;
  hxc_status status;
  size_t index;
  if (!stress_allocator_init(state->fail_at, state, &allocator)) {
    return HXC_STATUS_INTERNAL_ERROR;
  }
  config.allocator = allocator;
  config.initial_threshold_bytes = SIZE_MAX;
  config.clock_now = NULL;
  config.clock_context = NULL;
  status = hxc_gc_init(&config, &gc);
  if (status != HXC_STATUS_OK) {
    return status;
  }
  for (index = 0u; index < node_count; index++) {
    stress_node *node = NULL;
    status = hxc_gc_allocate(&gc, &stress_node_descriptor, (void **)&node);
    if (status != HXC_STATUS_OK) {
      (void)hxc_gc_dispose(&gc);
      return status;
    }
    node->value = (uint32_t)index ^ HXC_STRESS_SEED;
    if (previous == NULL) {
      first = node;
    } else {
      previous->next = node;
    }
    previous = node;
  }
  if (first == NULL || previous == NULL) {
    (void)hxc_gc_dispose(&gc);
    return HXC_STATUS_INTERNAL_ERROR;
  }
  previous->next = first;
  root_slots[0] = first;
  status = hxc_gc_root_table_register(&gc, root_slots, 1u, &roots);
  if (status == HXC_STATUS_OK && collect_cycle) {
    status = hxc_gc_collect(&gc);
  }
  if (status == HXC_STATUS_OK && !hxc_gc_owns_exact(&gc, previous)) {
    status = HXC_STATUS_INTERNAL_ERROR;
  }
  root_slots[0] = NULL;
  if (status == HXC_STATUS_OK && collect_cycle) {
    status = hxc_gc_collect(&gc);
  }
  if (roots.registered) {
    const hxc_status unregister_status = hxc_gc_root_table_unregister(&roots);
    if (status == HXC_STATUS_OK) {
      status = unregister_status;
    }
  }
  if (status == HXC_STATUS_OK && collect_cycle) {
    hxc_gc_stats stats = HXC_GC_STATS_INITIALIZER;
    status = hxc_gc_get_stats(&gc, &stats);
    if (status == HXC_STATUS_OK && stats.current_object_count != 0u) {
      status = HXC_STATUS_INTERNAL_ERROR;
    }
  }
  {
    const hxc_status dispose_status = hxc_gc_dispose(&gc);
    if (status == HXC_STATUS_OK) {
      status = dispose_status;
    }
  }
  return status;
}

static hxc_status stress_gc_case(stress_allocator_state *state) {
  size_t count = (size_t)HXC_STRESS_LIMIT;
  if (count > 32u) {
    count = 32u;
  }
  return stress_gc_build(state, count, true);
}

static bool stress_fault_sweep(
  const char *phase,
  stress_case_fn run_case,
  uint32_t required_sites,
  size_t *out_attempts
) {
  stress_allocator_state baseline;
  hxc_status status;
  size_t fail_at;
  stress_phase = phase;
  baseline.fail_at = HXC_STRESS_NO_FAILURE;
  status = run_case(&baseline);
  HXC_STRESS_CHECK(status == HXC_STATUS_OK);
  HXC_STRESS_CHECK(baseline.live_blocks == 0u);
  HXC_STRESS_CHECK(!baseline.release_underflow);
  HXC_STRESS_CHECK(baseline.attempt_count > 0u);
  HXC_STRESS_CHECK((baseline.site_mask & required_sites) == required_sites);
  *out_attempts = baseline.attempt_count;
  for (fail_at = 0u; fail_at < baseline.attempt_count; fail_at++) {
    stress_allocator_state injected;
    injected.fail_at = fail_at;
    status = run_case(&injected);
    HXC_STRESS_CHECK(status == HXC_STATUS_OUT_OF_MEMORY);
    HXC_STRESS_CHECK(injected.injected_failure);
    HXC_STRESS_CHECK(injected.live_blocks == 0u);
    HXC_STRESS_CHECK(!injected.release_underflow);
    HXC_STRESS_CHECK(injected.attempt_count == fail_at + 1u);
  }
  return true;
}

static hxc_status stress_cleanup_record(void *context) {
  const int32_t value = *(const int32_t *)context;
  if (stress_cleanup_count >= 4u) {
    return HXC_STATUS_INTERNAL_ERROR;
  }
  stress_cleanup_trace[stress_cleanup_count] = value;
  stress_cleanup_count++;
  return HXC_STATUS_OK;
}

static bool stress_exception_round(int32_t payload_value) {
  static const hxc_dynamic_type int_type = {
    HXC_DYNAMIC_TYPE_ABI_VERSION,
    UINT32_C(1),
    HXC_DYNAMIC_CATEGORY_INT,
    HXC_DYNAMIC_STORAGE_INLINE_INT32
  };
  static int32_t cleanup_values[4] = { 1, 2, 3, 4 };
  hxc_exception_frame frame = HXC_EXCEPTION_FRAME_INITIALIZER;
  hxc_exception_cleanup cleanups[4] = {
    HXC_EXCEPTION_CLEANUP_INITIALIZER,
    HXC_EXCEPTION_CLEANUP_INITIALIZER,
    HXC_EXCEPTION_CLEANUP_INITIALIZER,
    HXC_EXCEPTION_CLEANUP_INITIALIZER
  };
  hxc_value payload = HXC_VALUE_INVALID_INITIALIZER;
  int32_t observed = 0;
  int jump_result;
  size_t index;
  stress_cleanup_count = 0u;
  if (hxc_exception_frame_push(&frame, NULL, NULL) != HXC_STATUS_OK) {
    return false;
  }
  jump_result = HXC_EXCEPTION_SETJMP(&frame);
  if (jump_result == 0) {
    for (index = 0u; index < 4u; index++) {
      if (hxc_exception_cleanup_push(
        &cleanups[index],
        stress_cleanup_record,
        (void *)&cleanup_values[index]
      ) != HXC_STATUS_OK) {
        return false;
      }
    }
    if (hxc_value_init_int32(&int_type, payload_value, &payload)
      != HXC_STATUS_OK) {
      return false;
    }
    (void)hxc_exception_raise(&payload);
    return false;
  }
  if (hxc_exception_frame_take_payload(&frame, &payload) != HXC_STATUS_OK
    || hxc_value_read_int32(&payload, &observed) != HXC_STATUS_OK
    || observed != payload_value
    || hxc_exception_frame_pop(&frame) != HXC_STATUS_OK) {
    return false;
  }
  return stress_cleanup_count == 4u
    && stress_cleanup_trace[0] == 4
    && stress_cleanup_trace[1] == 3
    && stress_cleanup_trace[2] == 2
    && stress_cleanup_trace[3] == 1;
}

static bool stress_exception_repetition(void) {
  size_t index;
  stress_phase = "exception-cleanup";
  for (index = 0u; index < (size_t)HXC_STRESS_LIMIT; index++) {
    const int32_t payload = (int32_t)(stress_random_next() & UINT32_C(0x7FFFFFFF));
    HXC_STRESS_CHECK(stress_exception_round(payload));
  }
  return true;
}

static bool stress_deep_cycle(void) {
  stress_allocator_state state;
  hxc_status status;
  stress_phase = "deep-cycle";
  state.fail_at = HXC_STRESS_NO_FAILURE;
  status = stress_gc_build(&state, (size_t)HXC_STRESS_LIMIT, true);
  HXC_STRESS_CHECK(status == HXC_STATUS_OK);
  HXC_STRESS_CHECK(state.live_blocks == 0u);
  HXC_STRESS_CHECK(!state.release_underflow);
  HXC_STRESS_CHECK(state.attempt_count == (size_t)HXC_STRESS_LIMIT);
  return true;
}

static bool stress_corrupt_inputs(void) {
  static const uint8_t malformed_bytes[2] = { UINT8_C(0xC0), UINT8_C(0xAF) };
  hxc_byte_view malformed = { malformed_bytes, 2u };
  hxc_owned_string output = HXC_OWNED_STRING_INITIALIZER;
  stress_allocator_state state;
  hxc_allocator allocator;
  hxc_type_descriptor invalid_descriptor = stress_node_descriptor;
  hxc_gc gc = HXC_GC_INITIALIZER;
  hxc_gc_config config;
  void *object = NULL;
  stress_phase = "corrupt-input";
  HXC_STRESS_CHECK(stress_allocator_init(
    HXC_STRESS_NO_FAILURE,
    &state,
    &allocator
  ));
  HXC_STRESS_CHECK(hxc_string_from_utf8_checked(
    malformed,
    &allocator,
    &output
  ) == HXC_STATUS_INVALID_UTF8);
  HXC_STRESS_CHECK(state.attempt_count == 0u);
  config.allocator = allocator;
  config.initial_threshold_bytes = 1024u;
  config.clock_now = NULL;
  config.clock_context = NULL;
  HXC_STRESS_CHECK(hxc_gc_init(&config, &gc) == HXC_STATUS_OK);
  invalid_descriptor.abi_version++;
  HXC_STRESS_CHECK(hxc_gc_allocate(
    &gc,
    &invalid_descriptor,
    &object
  ) == HXC_STATUS_INVALID_ARGUMENT);
  HXC_STRESS_CHECK(object == NULL);
  HXC_STRESS_CHECK(state.attempt_count == 0u);
  HXC_STRESS_CHECK(hxc_gc_dispose(&gc) == HXC_STATUS_OK);
  HXC_STRESS_CHECK(state.live_blocks == 0u);
  HXC_STRESS_CHECK(!state.release_underflow);
  return true;
}

int main(void) {
  stress_random_state = HXC_STRESS_SEED;
  if (HXC_STRESS_LIMIT < 16u || HXC_STRESS_LIMIT > 4096u) {
    fprintf(stderr, "runtime-stress: invalid limit=%u\n", (unsigned int)HXC_STRESS_LIMIT);
    return 2;
  }
  if (!stress_fault_sweep(
    "string-fault-sweep",
    stress_string_case,
    HXC_STRESS_SITE_ALLOCATE | HXC_STRESS_SITE_REALLOCATE,
    &stress_string_attempts
  )
    || !stress_fault_sweep(
      "array-fault-sweep",
      stress_array_case,
      HXC_STRESS_SITE_ALLOCATE | HXC_STRESS_SITE_REALLOCATE,
      &stress_array_attempts
    )
    || !stress_fault_sweep(
      "gc-fault-sweep",
      stress_gc_case,
      HXC_STRESS_SITE_ALLOCATE,
      &stress_gc_attempts
    )
    || !stress_deep_cycle()
    || !stress_exception_repetition()
    || !stress_corrupt_inputs()) {
    return 1;
  }
  printf(
    "runtime-stress: OK abi=%u.%u.%u features=alloc,string,array,gc,dynamic,exception "
    "seed=%" PRIu32 " limit=%u string-attempts=%zu array-attempts=%zu "
    "gc-attempts=%zu threads=not-applicable\n",
    (unsigned int)HXC_RUNTIME_ABI_MAJOR,
    (unsigned int)HXC_RUNTIME_ABI_MINOR,
    (unsigned int)HXC_RUNTIME_ABI_PATCH,
    (uint32_t)HXC_STRESS_SEED,
    (unsigned int)HXC_STRESS_LIMIT,
    stress_string_attempts,
    stress_array_attempts,
    stress_gc_attempts
  );
  return 0;
}
