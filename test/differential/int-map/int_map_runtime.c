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
  hxc_int_bool_map_ref *alias;
  bool found = true;
  int32_t key;

  CHECK(hxc_int_bool_map_ref_create(allocator, &map) == HXC_STATUS_OK);
  CHECK(map != NULL);
  CHECK(hxc_int_bool_map_ref_exists(map, INT32_C(-7), &found) == HXC_STATUS_OK);
  CHECK(!found);
  CHECK(hxc_int_bool_map_ref_set(map, INT32_C(-7), false) == HXC_STATUS_OK);
  CHECK(hxc_int_bool_map_ref_exists(map, INT32_C(-7), &found) == HXC_STATUS_OK);
  CHECK(found);

  alias = map;
  CHECK(hxc_int_bool_map_ref_retain(alias) == HXC_STATUS_OK);
  CHECK(hxc_int_bool_map_ref_set(alias, INT32_MAX, true) == HXC_STATUS_OK);
  CHECK(hxc_int_bool_map_ref_exists(map, INT32_MAX, &found) == HXC_STATUS_OK);
  CHECK(found);

  for (key = 0; key < 4; key++) {
    CHECK(hxc_int_bool_map_ref_set(map, key, (key & 1) != 0) == HXC_STATUS_OK);
  }
  state.fail = true;
  CHECK(hxc_int_bool_map_ref_set(map, INT32_C(99), true) == HXC_STATUS_OUT_OF_MEMORY);
  CHECK(hxc_int_bool_map_ref_exists(alias, INT32_C(-7), &found) == HXC_STATUS_OK);
  CHECK(found);
  CHECK(hxc_int_bool_map_ref_exists(alias, INT32_C(99), &found) == HXC_STATUS_OK);
  CHECK(!found);
  state.fail = false;

  CHECK(hxc_int_bool_map_ref_exists(NULL, 0, &found) == HXC_STATUS_INVALID_ARGUMENT);
  CHECK(hxc_int_bool_map_ref_exists(map, 0, NULL) == HXC_STATUS_INVALID_ARGUMENT);
  CHECK(hxc_int_bool_map_ref_retain(NULL) == HXC_STATUS_OK);
  CHECK(hxc_int_bool_map_ref_release(NULL) == HXC_STATUS_OK);
  CHECK(hxc_int_bool_map_ref_release(alias) == HXC_STATUS_OK);
  CHECK(hxc_int_bool_map_ref_release(map) == HXC_STATUS_OK);
  CHECK(state.allocations == state.releases);
  return 0;
}

int main(void) {
  return prove_contract();
}
