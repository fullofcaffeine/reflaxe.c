/*
 * Independent native contract for the private hxrt StringMap boundary.
 *
 * `generated/Main.hx` owns the language/compiler end-to-end semantics. This
 * separately authored C fixture deliberately does not pass through haxe.c: it
 * can inject allocator failure and malformed ABI inputs, and therefore catches
 * a bug shared by code generation and the runtime instead of comparing the
 * compiler with itself. It is test-only C, never application implementation.
 */
#include "hxrt/string_map.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CHECK(condition) \
  do { \
    if (!(condition)) { \
      (void)fprintf(stderr, "string-map-runtime: check failed at line %d\n", __LINE__); \
      return 1; \
    } \
  } while (false)

typedef struct failing_allocator_state {
  size_t successful_allocations;
  size_t releases;
  size_t fail_after;
} failing_allocator_state;

typedef struct tracked_value {
  int32_t payload;
} tracked_value;

typedef struct tracked_value_state {
  size_t copies;
  size_t assignments;
  size_t destructions;
  size_t live_owners;
  bool fail_copy;
  bool fail_assign;
} tracked_value_state;

static hxc_status test_allocate(
  void *context,
  size_t size,
  size_t alignment,
  void **out_memory
) {
  failing_allocator_state *state = context;
  void *memory;
  if (state == NULL || out_memory == NULL || *out_memory != NULL) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  if (state->successful_allocations >= state->fail_after) {
    return HXC_STATUS_OUT_OF_MEMORY;
  }
  memory = malloc(size);
  if (memory == NULL) {
    return HXC_STATUS_OUT_OF_MEMORY;
  }
  (void)alignment;
  state->successful_allocations++;
  *out_memory = memory;
  return HXC_STATUS_OK;
}

static void test_release(
  void *context,
  void *memory,
  size_t size,
  size_t alignment
) {
  failing_allocator_state *state = context;
  (void)size;
  (void)alignment;
  if (state != NULL) {
    state->releases++;
  }
  free(memory);
}

static hxc_string literal(const char *text) {
  hxc_string result = HXC_STRING_INITIALIZER;
  result.data = (const uint8_t *)text;
  result.byte_length = strlen(text);
  result.has_trailing_nul = true;
  return result;
}

static hxc_string_map_value_ops bool_ops(void) {
  hxc_string_map_value_ops result = {
    sizeof(bool),
    HXC_ALIGNOF(bool),
    NULL,
    NULL,
    NULL,
    NULL
  };
  return result;
}

static hxc_status tracked_copy(
  void *context,
  void *destination,
  const void *source
) {
  tracked_value_state *state = context;
  if (state == NULL || destination == NULL || source == NULL) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  if (state->fail_copy) {
    return HXC_STATUS_OUT_OF_MEMORY;
  }
  *(tracked_value *)destination = *(const tracked_value *)source;
  state->copies++;
  state->live_owners++;
  return HXC_STATUS_OK;
}

static hxc_status tracked_assign(
  void *context,
  void *destination,
  const void *source
) {
  tracked_value_state *state = context;
  if (state == NULL || destination == NULL || source == NULL) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  if (state->fail_assign) {
    return HXC_STATUS_OUT_OF_MEMORY;
  }
  *(tracked_value *)destination = *(const tracked_value *)source;
  state->assignments++;
  return HXC_STATUS_OK;
}

static void tracked_destroy(void *context, void *value) {
  tracked_value_state *state = context;
  (void)value;
  if (state != NULL) {
    state->destructions++;
    state->live_owners--;
  }
}

static hxc_string_map_value_ops tracked_ops(tracked_value_state *state) {
  hxc_string_map_value_ops result = {
    sizeof(tracked_value),
    HXC_ALIGNOF(tracked_value),
    state,
    tracked_copy,
    tracked_assign,
    tracked_destroy
  };
  return result;
}

static int prove_basic_contract(void) {
  hxc_string_map_ref *map = NULL;
  hxc_string_map_ref *alias;
  bool value = false;
  bool found = true;
  bool removed = true;
  size_t index;
  char key_buffer[32];

  CHECK(hxc_string_map_ref_create(
    hxc_default_allocator(),
    sizeof(bool),
    HXC_ALIGNOF(bool),
    &map
  ) == HXC_STATUS_OK);
  CHECK(map != NULL);
  CHECK(hxc_string_map_ref_exists(map, literal("missing"), &found) == HXC_STATUS_OK);
  CHECK(!found);
  CHECK(hxc_string_map_ref_get_copy(
    map,
    literal("missing"),
    &value,
    &found
  ) == HXC_STATUS_OK);
  CHECK(!found);

  value = false;
  CHECK(hxc_string_map_ref_set_copy(map, literal("alpha"), &value) == HXC_STATUS_OK);
  value = true;
  CHECK(hxc_string_map_ref_set_copy(map, literal("beta"), &value) == HXC_STATUS_OK);
  CHECK(hxc_string_map_ref_set_copy(map, literal(""), &value) == HXC_STATUS_OK);
  value = true;
  CHECK(hxc_string_map_ref_get_copy(map, literal("alpha"), &value, &found) == HXC_STATUS_OK);
  CHECK(found && !value);
  value = false;
  CHECK(hxc_string_map_ref_get_copy(map, literal(""), &value, &found) == HXC_STATUS_OK);
  CHECK(found && value);

  alias = map;
  CHECK(hxc_string_map_ref_retain(alias) == HXC_STATUS_OK);
  value = true;
  CHECK(hxc_string_map_ref_set_copy(alias, literal("alpha"), &value) == HXC_STATUS_OK);
  value = false;
  CHECK(hxc_string_map_ref_get_copy(map, literal("alpha"), &value, &found) == HXC_STATUS_OK);
  CHECK(found && value);

  CHECK(hxc_string_map_ref_remove(map, literal("beta"), &removed) == HXC_STATUS_OK);
  CHECK(removed);
  CHECK(hxc_string_map_ref_remove(map, literal("beta"), &removed) == HXC_STATUS_OK);
  CHECK(!removed);

  for (index = 0u; index < 256u; index++) {
    const int written = snprintf(key_buffer, sizeof(key_buffer), "key-%zu", index);
    CHECK(written > 0 && (size_t)written < sizeof(key_buffer));
    value = (index & 1u) != 0u;
    CHECK(hxc_string_map_ref_set_copy(map, literal(key_buffer), &value) == HXC_STATUS_OK);
  }
  for (index = 0u; index < 256u; index++) {
    const int written = snprintf(key_buffer, sizeof(key_buffer), "key-%zu", index);
    CHECK(written > 0 && (size_t)written < sizeof(key_buffer));
    value = false;
    CHECK(hxc_string_map_ref_get_copy(map, literal(key_buffer), &value, &found) == HXC_STATUS_OK);
    CHECK(found && value == ((index & 1u) != 0u));
  }

  CHECK(hxc_string_map_ref_clear(alias) == HXC_STATUS_OK);
  CHECK(hxc_string_map_ref_exists(map, literal("alpha"), &found) == HXC_STATUS_OK);
  CHECK(!found);
  CHECK(hxc_string_map_ref_release(alias) == HXC_STATUS_OK);
  CHECK(hxc_string_map_ref_release(map) == HXC_STATUS_OK);
  return 0;
}

static int prove_failure_atomic_insertion(void) {
  failing_allocator_state state = {0u, 0u, SIZE_MAX};
  hxc_allocator allocator = {
    &state,
    test_allocate,
    NULL,
    test_release
  };
  hxc_string_map_ref *map = NULL;
  bool value = true;
  bool found = false;

  CHECK(hxc_string_map_ref_create(
    allocator,
    sizeof(bool),
    HXC_ALIGNOF(bool),
    &map
  ) == HXC_STATUS_OK);
  CHECK(hxc_string_map_ref_set_copy(map, literal("stable"), &value) == HXC_STATUS_OK);

  state.fail_after = state.successful_allocations;
  CHECK(hxc_string_map_ref_set_copy(
    map,
    literal("must-not-appear"),
    &value
  ) == HXC_STATUS_OUT_OF_MEMORY);
  CHECK(hxc_string_map_ref_exists(map, literal("stable"), &found) == HXC_STATUS_OK);
  CHECK(found);
  CHECK(hxc_string_map_ref_exists(
    map,
    literal("must-not-appear"),
    &found
  ) == HXC_STATUS_OK);
  CHECK(!found);

  state.fail_after = SIZE_MAX;
  CHECK(hxc_string_map_ref_release(map) == HXC_STATUS_OK);
  CHECK(state.successful_allocations == state.releases);
  return 0;
}

static int prove_managed_value_callbacks(void) {
  tracked_value_state state = {0u, 0u, 0u, 0u, false, false};
  hxc_string_map_value_ops operations = tracked_ops(&state);
  hxc_string_map_ref *map = NULL;
  tracked_value source = {7};
  tracked_value output = {-1};
  bool found = false;
  bool removed = false;

  CHECK(hxc_string_map_value_ops_is_valid(&operations));
  CHECK(hxc_string_map_ref_create_with_ops(
    hxc_default_allocator(),
    operations,
    &map
  ) == HXC_STATUS_OK);
  CHECK(hxc_string_map_ref_set_copy(map, literal("managed"), &source) == HXC_STATUS_OK);
  CHECK(state.copies == 1u && state.live_owners == 1u);

  CHECK(hxc_string_map_ref_get_copy(
    map,
    literal("managed"),
    &output,
    &found
  ) == HXC_STATUS_OK);
  CHECK(found && output.payload == 7);
  CHECK(state.copies == 2u && state.live_owners == 2u);
  operations.destroy(operations.context, &output);
  CHECK(state.destructions == 1u && state.live_owners == 1u);

  source.payload = 11;
  CHECK(hxc_string_map_ref_set_copy(map, literal("managed"), &source) == HXC_STATUS_OK);
  CHECK(state.assignments == 1u && state.live_owners == 1u);

  state.fail_assign = true;
  source.payload = 99;
  CHECK(hxc_string_map_ref_set_copy(
    map,
    literal("managed"),
    &source
  ) == HXC_STATUS_OUT_OF_MEMORY);
  state.fail_assign = false;
  CHECK(hxc_string_map_ref_get_copy(
    map,
    literal("managed"),
    &output,
    &found
  ) == HXC_STATUS_OK);
  CHECK(found && output.payload == 11);
  operations.destroy(operations.context, &output);

  state.fail_copy = true;
  CHECK(hxc_string_map_ref_set_copy(
    map,
    literal("must-not-appear"),
    &source
  ) == HXC_STATUS_OUT_OF_MEMORY);
  state.fail_copy = false;
  CHECK(hxc_string_map_ref_exists(
    map,
    literal("must-not-appear"),
    &found
  ) == HXC_STATUS_OK);
  CHECK(!found);

  CHECK(hxc_string_map_ref_remove(
    map,
    literal("managed"),
    &removed
  ) == HXC_STATUS_OK);
  CHECK(removed && state.live_owners == 0u);
  CHECK(hxc_string_map_ref_release(map) == HXC_STATUS_OK);
  CHECK(state.copies == 3u);
  CHECK(state.assignments == 1u);
  CHECK(state.destructions == 3u);
  return 0;
}

static int prove_invalid_inputs_fail_closed(void) {
  hxc_string_map_ref *map = NULL;
  hxc_string_map_ref *occupied_output;
  hxc_string malformed_key = HXC_STRING_INITIALIZER;
  bool value = true;
  bool result = true;

  CHECK(hxc_string_map_ref_create(
    hxc_default_allocator(),
    sizeof(bool),
    HXC_ALIGNOF(bool),
    &map
  ) == HXC_STATUS_OK);
  occupied_output = map;
  CHECK(hxc_string_map_ref_create(
    hxc_default_allocator(),
    sizeof(bool),
    HXC_ALIGNOF(bool),
    &occupied_output
  ) == HXC_STATUS_INVALID_ARGUMENT);
  CHECK(occupied_output == map);
  occupied_output = NULL;
  {
    CHECK(hxc_string_map_ref_create(
      hxc_default_allocator(),
      0u,
      HXC_ALIGNOF(bool),
      &occupied_output
    ) == HXC_STATUS_INVALID_ARGUMENT);
  }
  CHECK(occupied_output == NULL);
  {
    hxc_string_map_value_ops invalid = bool_ops();
    invalid.copy = tracked_copy;
    CHECK(!hxc_string_map_value_ops_is_valid(&invalid));
    CHECK(hxc_string_map_ref_create_with_ops(
      hxc_default_allocator(),
      invalid,
      &occupied_output
    ) == HXC_STATUS_INVALID_ARGUMENT);
  }
  CHECK(occupied_output == NULL);
  {
    CHECK(hxc_string_map_ref_create(
      hxc_default_allocator(),
      sizeof(bool),
      3u,
      &occupied_output
    ) == HXC_STATUS_INVALID_ARGUMENT);
  }
  CHECK(occupied_output == NULL);

  /*
   * Null<Map<String, Bool>> uses the same pointer carrier as Map itself.
   * Cleanup and alias operations therefore accept NULL as an absent owner;
   * operations that require an actual table still reject it.
   */
  CHECK(hxc_string_map_ref_retain(NULL) == HXC_STATUS_OK);
  CHECK(hxc_string_map_ref_release(NULL) == HXC_STATUS_OK);
  CHECK(hxc_string_map_ref_clear(NULL) == HXC_STATUS_INVALID_ARGUMENT);
  CHECK(hxc_string_map_ref_set_copy(
    map,
    literal("key"),
    NULL
  ) == HXC_STATUS_INVALID_ARGUMENT);
  CHECK(hxc_string_map_ref_exists(
    map,
    literal("key"),
    NULL
  ) == HXC_STATUS_INVALID_ARGUMENT);
  CHECK(hxc_string_map_ref_get_copy(
    map,
    literal("key"),
    NULL,
    &result
  ) == HXC_STATUS_INVALID_ARGUMENT);
  CHECK(hxc_string_map_ref_get_copy(
    map,
    literal("key"),
    &value,
    NULL
  ) == HXC_STATUS_INVALID_ARGUMENT);
  CHECK(hxc_string_map_ref_remove(
    map,
    literal("key"),
    NULL
  ) == HXC_STATUS_INVALID_ARGUMENT);

  malformed_key.byte_length = 1u;
  CHECK(hxc_string_map_ref_exists(
    map,
    malformed_key,
    &result
  ) == HXC_STATUS_INVALID_ARGUMENT);
  CHECK(result);
  CHECK(hxc_string_map_ref_release(map) == HXC_STATUS_OK);
  return 0;
}

int main(void) {
  CHECK(prove_basic_contract() == 0);
  CHECK(prove_failure_atomic_insertion() == 0);
  CHECK(prove_managed_value_callbacks() == 0);
  CHECK(prove_invalid_inputs_fail_closed() == 0);
  return 0;
}
