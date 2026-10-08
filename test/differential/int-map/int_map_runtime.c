/*
 * Independent native contract for the private hxrt IntMap boundary.
 *
 * The Haxe fixture proves language-to-C behavior. This separately authored C
 * program deliberately does not pass through haxe.c: it can force allocator
 * failure and malformed ABI calls, so it can catch a runtime bug even when the
 * compiler would otherwise generate matching assumptions. It is test-only C,
 * never application implementation.
 */
#include "hxrt/int_map.h"

#include <stdio.h>
#include <stdlib.h>

#define CHECK(condition) \
  do { \
    if (!(condition)) { \
      (void)fprintf(stderr, "int-map-runtime: check failed at line %d\n", __LINE__); \
      return 1; \
    } \
  } while (false)

typedef struct test_allocator_state {
  size_t allocations;
  size_t releases;
  bool fail;
} test_allocator_state;

typedef struct test_pair {
  int32_t key;
  bool value;
} test_pair;

static hxc_status test_allocate(
  void *context,
  size_t size,
  size_t alignment,
  void **out_memory
) {
  test_allocator_state *state = (test_allocator_state *)context;
  void *memory;
  (void)alignment;
  if (state == NULL || out_memory == NULL || *out_memory != NULL) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  if (state->fail) {
    return HXC_STATUS_OUT_OF_MEMORY;
  }
  memory = malloc(size);
  if (memory == NULL) {
    return HXC_STATUS_OUT_OF_MEMORY;
  }
  state->allocations++;
  *out_memory = memory;
  return HXC_STATUS_OK;
}

static void test_release(
  void *context,
  void *memory,
  size_t size,
  size_t alignment
) {
  test_allocator_state *state = (test_allocator_state *)context;
  (void)size;
  (void)alignment;
  if (state != NULL) {
    state->releases++;
  }
  free(memory);
}

static int prove_contract(void) {
  test_allocator_state state = {0u, 0u, false};
  hxc_allocator allocator = {&state, test_allocate, NULL, test_release};
  hxc_int_bool_map_ref *map = NULL;
  hxc_int_bool_map_ref *copy = NULL;
  hxc_int_bool_map_ref *alias;
  hxc_int_bool_map_ref *occupied_output;
  bool found = true;
  bool value = true;
  bool removed = false;
  int32_t key;
	  hxc_iterator_ref *iterator = NULL;
	  hxc_string rendered = HXC_STRING_INITIALIZER;

  CHECK(hxc_int_bool_map_ref_create(allocator, &map) == HXC_STATUS_OK);
  CHECK(map != NULL);
  CHECK(hxc_int_bool_map_ref_exists(map, INT32_C(-7), &found) == HXC_STATUS_OK);
  CHECK(!found);
  CHECK(hxc_int_bool_map_ref_set(map, INT32_C(-7), false) == HXC_STATUS_OK);
  CHECK(hxc_int_bool_map_ref_exists(map, INT32_C(-7), &found) == HXC_STATUS_OK);
  CHECK(found);
  CHECK(hxc_int_bool_map_ref_get(map, INT32_C(-7), &value, &found) == HXC_STATUS_OK);
  CHECK(found && !value);
  CHECK(hxc_int_bool_map_ref_get(map, INT32_C(-6), &value, &found) == HXC_STATUS_OK);
  CHECK(!found);

  alias = map;
  CHECK(hxc_int_bool_map_ref_retain(alias) == HXC_STATUS_OK);
  CHECK(hxc_int_bool_map_ref_set(alias, INT32_MAX, true) == HXC_STATUS_OK);
  CHECK(hxc_int_bool_map_ref_exists(map, INT32_MAX, &found) == HXC_STATUS_OK);
  CHECK(found);

  CHECK(hxc_int_bool_map_ref_key_iterator(map, &iterator) == HXC_STATUS_OK);
  CHECK(hxc_int_bool_map_ref_clear(map) == HXC_STATUS_OK);
  CHECK(hxc_iterator_ref_has_next(iterator, &found) == HXC_STATUS_OK && found);
  CHECK(hxc_iterator_ref_next_move(iterator, &key) == HXC_STATUS_OK);
  CHECK(hxc_iterator_ref_release(iterator) == HXC_STATUS_OK);
  iterator = NULL;
  CHECK(hxc_int_bool_map_ref_set(map, INT32_C(17), false) == HXC_STATUS_OK);
  CHECK(hxc_int_bool_map_ref_pair_iterator(
    map, sizeof(test_pair), HXC_ALIGNOF(test_pair),
    offsetof(test_pair, key), offsetof(test_pair, value), &iterator
  ) == HXC_STATUS_OK);
  {
    test_pair pair = {0};
    CHECK(hxc_iterator_ref_next_move(iterator, &pair) == HXC_STATUS_OK);
    CHECK(pair.key == INT32_C(17) && !pair.value);
  }
  CHECK(hxc_iterator_ref_release(iterator) == HXC_STATUS_OK);
  iterator = NULL;
  CHECK(hxc_int_bool_map_ref_to_string(map, &rendered) == HXC_STATUS_OK);
  CHECK(rendered.byte_length == 13u);
  CHECK(hxc_string_release(&rendered) == HXC_STATUS_OK);
  CHECK(hxc_int_bool_map_ref_set(map, INT32_MAX, true) == HXC_STATUS_OK);
  /* Keys 0 and 1 share the first slot under the runtime's exact hash. */
  CHECK(hxc_int_bool_map_ref_set(map, INT32_C(0), false) == HXC_STATUS_OK);
  CHECK(hxc_int_bool_map_ref_set(map, INT32_C(1), true) == HXC_STATUS_OK);
  CHECK(hxc_int_bool_map_ref_remove(map, INT32_C(0), &removed) == HXC_STATUS_OK);
  CHECK(removed);
  CHECK(hxc_int_bool_map_ref_remove(map, INT32_C(0), &removed) == HXC_STATUS_OK);
  CHECK(!removed);
  CHECK(hxc_int_bool_map_ref_get(map, INT32_C(1), &value, &found) == HXC_STATUS_OK);
  CHECK(found && value);

  CHECK(hxc_int_bool_map_ref_copy(map, &copy) == HXC_STATUS_OK);
  CHECK(copy != NULL && copy != map);
  CHECK(hxc_int_bool_map_ref_get(copy, INT32_C(1), &value, &found) == HXC_STATUS_OK);
  CHECK(found && value);
  CHECK(hxc_int_bool_map_ref_set(copy, INT32_C(1), false) == HXC_STATUS_OK);
  CHECK(hxc_int_bool_map_ref_remove(copy, INT32_MAX, &removed) == HXC_STATUS_OK);
  CHECK(removed);
  CHECK(hxc_int_bool_map_ref_get(map, INT32_C(1), &value, &found) == HXC_STATUS_OK);
  CHECK(found && value);
  CHECK(hxc_int_bool_map_ref_exists(map, INT32_MAX, &found) == HXC_STATUS_OK);
  CHECK(found);

  for (key = 0; key < 4; key++) {
    CHECK(hxc_int_bool_map_ref_set(map, key, (key & 1) != 0) == HXC_STATUS_OK);
  }
  state.fail = true;
	  CHECK(hxc_int_bool_map_ref_value_iterator(map, &iterator) == HXC_STATUS_OUT_OF_MEMORY);
	  CHECK(iterator == NULL);
  CHECK(hxc_int_bool_map_ref_set(map, INT32_C(99), true) == HXC_STATUS_OUT_OF_MEMORY);
  CHECK(hxc_int_bool_map_ref_exists(alias, INT32_MAX, &found) == HXC_STATUS_OK);
  CHECK(found);
  CHECK(hxc_int_bool_map_ref_exists(alias, INT32_C(99), &found) == HXC_STATUS_OK);
  CHECK(!found);
  occupied_output = map;
  CHECK(hxc_int_bool_map_ref_copy(map, &occupied_output) == HXC_STATUS_INVALID_ARGUMENT);
  CHECK(occupied_output == map);
  occupied_output = NULL;
  CHECK(hxc_int_bool_map_ref_copy(NULL, &occupied_output) == HXC_STATUS_INVALID_ARGUMENT);
  CHECK(occupied_output == NULL);
  CHECK(hxc_int_bool_map_ref_copy(map, NULL) == HXC_STATUS_INVALID_ARGUMENT);
  CHECK(hxc_int_bool_map_ref_copy(map, &occupied_output) == HXC_STATUS_OUT_OF_MEMORY);
  CHECK(occupied_output == NULL);
  CHECK(hxc_int_bool_map_ref_exists(map, INT32_MAX, &found) == HXC_STATUS_OK);
  CHECK(found);
  state.fail = false;
  CHECK(hxc_int_bool_map_ref_clear(alias) == HXC_STATUS_OK);
  CHECK(hxc_int_bool_map_ref_exists(map, INT32_MAX, &found) == HXC_STATUS_OK);
  CHECK(!found);

  CHECK(hxc_int_bool_map_ref_exists(NULL, 0, &found) == HXC_STATUS_INVALID_ARGUMENT);
  CHECK(hxc_int_bool_map_ref_exists(map, 0, NULL) == HXC_STATUS_INVALID_ARGUMENT);
  CHECK(hxc_int_bool_map_ref_get(NULL, 0, &value, &found) == HXC_STATUS_INVALID_ARGUMENT);
  CHECK(hxc_int_bool_map_ref_get(map, 0, NULL, &found) == HXC_STATUS_INVALID_ARGUMENT);
  CHECK(hxc_int_bool_map_ref_get(map, 0, &value, NULL) == HXC_STATUS_INVALID_ARGUMENT);
  CHECK(hxc_int_bool_map_ref_remove(NULL, 0, &removed) == HXC_STATUS_INVALID_ARGUMENT);
  CHECK(hxc_int_bool_map_ref_remove(map, 0, NULL) == HXC_STATUS_INVALID_ARGUMENT);
  CHECK(hxc_int_bool_map_ref_clear(NULL) == HXC_STATUS_INVALID_ARGUMENT);
  CHECK(hxc_int_bool_map_ref_retain(NULL) == HXC_STATUS_OK);
  CHECK(hxc_int_bool_map_ref_release(NULL) == HXC_STATUS_OK);
  CHECK(hxc_int_bool_map_ref_release(alias) == HXC_STATUS_OK);
  CHECK(hxc_int_bool_map_ref_release(copy) == HXC_STATUS_OK);
  CHECK(hxc_int_bool_map_ref_release(map) == HXC_STATUS_OK);
  CHECK(state.allocations == state.releases);
  return 0;
}

int main(void) {
  return prove_contract();
}
