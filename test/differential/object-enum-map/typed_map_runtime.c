/*
 * Independent native contract for the private exact-layout typed-map kernel.
 *
 * The Haxe fixture owns language semantics. This C program does not pass
 * through haxe.c, so it can force collisions, lifecycle failures, allocator
 * failures, exact collector tracing, and mutation after iterator snapshots.
 * It is test-only evidence and never application implementation.
 */
#include "hxrt/typed_map.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CHECK(condition) \
  do { \
    if (!(condition)) { \
      (void)fprintf( \
        stderr, \
        "typed-map-runtime: check failed at line %d\n", \
        __LINE__ \
      ); \
      return 1; \
    } \
  } while (false)

typedef struct test_allocator_state {
  size_t successful_allocations;
  size_t releases;
  size_t fail_at;
} test_allocator_state;

typedef struct test_key {
  int32_t logical;
  int32_t provenance;
} test_key;

typedef struct test_value {
  int32_t payload;
} test_value;

typedef struct test_pair {
  test_key key;
  test_value value;
} test_pair;

typedef struct test_policy_state {
  size_t copies;
  size_t destroys;
  size_t live_owners;
  bool fail_next_copy;
  bool broken;
} test_policy_state;

typedef struct test_object {
  int32_t identity;
} test_object;

typedef struct test_object_pair {
  test_object *key;
  test_object *value;
} test_object_pair;

static size_t finalized_test_objects = 0u;

static hxc_status test_allocate(
  void *context,
  size_t size,
  size_t alignment,
  void **out_memory
) {
  test_allocator_state *state = context;
  void *memory;
  (void)alignment;
  if (state == NULL || out_memory == NULL || *out_memory != NULL) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  if (state->successful_allocations == state->fail_at) {
    return HXC_STATUS_OUT_OF_MEMORY;
  }
  memory = malloc(size);
  if (memory == NULL) {
    return HXC_STATUS_OUT_OF_MEMORY;
  }
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
  test_allocator_state *state = context;
  (void)size;
  (void)alignment;
  if (state != NULL) {
    state->releases++;
  }
  free(memory);
}

static uint64_t constant_hash(void *context, const void *key) {
  (void)context;
  (void)key;
  return UINT64_C(3);
}

static bool logical_key_equal(
  void *context,
  const void *left,
  const void *right
) {
  const test_key *left_key = left;
  const test_key *right_key = right;
  (void)context;
  return left_key->logical == right_key->logical;
}

static hxc_status tracked_copy(
  void *context,
  void *destination,
  const void *source
) {
  test_policy_state *state = context;
  if (state == NULL || destination == NULL || source == NULL) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  if (state->fail_next_copy) {
    state->fail_next_copy = false;
    return HXC_STATUS_OUT_OF_MEMORY;
  }
  memcpy(destination, source, sizeof(test_key));
  state->copies++;
  state->live_owners++;
  return HXC_STATUS_OK;
}

static hxc_status tracked_value_copy(
  void *context,
  void *destination,
  const void *source
) {
  test_policy_state *state = context;
  if (state == NULL || destination == NULL || source == NULL) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  if (state->fail_next_copy) {
    state->fail_next_copy = false;
    return HXC_STATUS_OUT_OF_MEMORY;
  }
  memcpy(destination, source, sizeof(test_value));
  state->copies++;
  state->live_owners++;
  return HXC_STATUS_OK;
}

static void tracked_destroy(void *context, void *value) {
  test_policy_state *state = context;
  (void)value;
  if (state == NULL || state->live_owners == 0u) {
    if (state != NULL) {
      state->broken = true;
    }
    return;
  }
  state->destroys++;
  state->live_owners--;
}

static hxc_typed_map_key_ops trivial_key_ops(void) {
  const hxc_typed_map_key_ops result = {
    sizeof(test_key),
    HXC_ALIGNOF(test_key),
    NULL,
    NULL,
    NULL,
    NULL,
    constant_hash,
    logical_key_equal
  };
  return result;
}

static hxc_typed_map_value_ops trivial_value_ops(void) {
  const hxc_typed_map_value_ops result = {
    sizeof(test_value),
    HXC_ALIGNOF(test_value),
    NULL,
    NULL,
    NULL,
    NULL
  };
  return result;
}

static hxc_typed_map_key_ops tracked_key_ops(test_policy_state *state) {
  const hxc_typed_map_key_ops result = {
    sizeof(test_key),
    HXC_ALIGNOF(test_key),
    state,
    tracked_copy,
    tracked_destroy,
    NULL,
    constant_hash,
    logical_key_equal
  };
  return result;
}

static hxc_typed_map_value_ops tracked_value_ops(
  test_policy_state *state
) {
  const hxc_typed_map_value_ops result = {
    sizeof(test_value),
    HXC_ALIGNOF(test_value),
    state,
    tracked_value_copy,
    tracked_destroy,
    NULL
  };
  return result;
}

static int prove_collisions_snapshots_and_allocator_failure(void) {
  test_allocator_state state = {0u, 0u, SIZE_MAX};
  const hxc_allocator allocator = {
    &state,
    test_allocate,
    NULL,
    test_release
  };
  hxc_typed_map_ref *map = NULL;
  hxc_typed_map_ref *copy = NULL;
  hxc_iterator_ref *iterator = NULL;
  hxc_iterator_ref *alias;
  hxc_iterator_element_ops pair_ops = {
    sizeof(test_pair),
    HXC_ALIGNOF(test_pair),
    NULL,
    NULL,
    NULL,
    NULL
  };
  bool found = false;
  bool removed = false;
  size_t index;
  size_t snapshot_count = 0u;
  test_value observed = {0};

  CHECK(hxc_typed_map_ref_create(
    allocator,
    trivial_key_ops(),
    trivial_value_ops(),
    &map
  ) == HXC_STATUS_OK);
  for (index = 0u; index < 6u; index++) {
    const test_key key = {(int32_t)index, (int32_t)(100u + index)};
    const test_value value = {(int32_t)(index * 10u)};
    CHECK(hxc_typed_map_ref_set_copy(map, &key, &value) == HXC_STATUS_OK);
  }
  {
    const test_key replacement = {2, 777};
    const test_value value = {222};
    CHECK(hxc_typed_map_ref_set_copy(
      map,
      &replacement,
      &value
    ) == HXC_STATUS_OK);
    CHECK(hxc_typed_map_ref_get_copy(
      map,
      &replacement,
      &observed,
      &found
    ) == HXC_STATUS_OK);
    CHECK(found && observed.payload == 222);
  }
  state.fail_at = state.successful_allocations;
  {
    const test_key key = {6, 106};
    const test_value value = {60};
    CHECK(hxc_typed_map_ref_set_copy(
      map,
      &key,
      &value
    ) == HXC_STATUS_OUT_OF_MEMORY);
    CHECK(hxc_typed_map_ref_exists(map, &key, &found) == HXC_STATUS_OK);
    CHECK(!found);
  }
  state.fail_at = SIZE_MAX;
  {
    const test_key removed_key = {0, 0};
    const test_key survivor = {5, 0};
    const test_key inserted = {6, 106};
    const test_value value = {60};
    CHECK(hxc_typed_map_ref_remove(
      map,
      &removed_key,
      &removed
    ) == HXC_STATUS_OK);
    CHECK(removed);
    CHECK(hxc_typed_map_ref_set_copy(
      map,
      &inserted,
      &value
    ) == HXC_STATUS_OK);
    CHECK(hxc_typed_map_ref_exists(map, &survivor, &found) == HXC_STATUS_OK);
    CHECK(found);
  }
  CHECK(hxc_typed_map_ref_pair_iterator(
    map,
    pair_ops,
    offsetof(test_pair, key),
    offsetof(test_pair, value),
    &iterator
  ) == HXC_STATUS_OK);
  alias = iterator;
  CHECK(hxc_iterator_ref_retain(alias) == HXC_STATUS_OK);
  CHECK(hxc_typed_map_ref_clear(map) == HXC_STATUS_OK);
  while (true) {
    test_pair pair = {{0, 0}, {0}};
    CHECK(hxc_iterator_ref_has_next(iterator, &found) == HXC_STATUS_OK);
    if (!found) {
      break;
    }
    CHECK(hxc_iterator_ref_next_move(alias, &pair) == HXC_STATUS_OK);
    if (pair.key.logical == 2) {
      CHECK(pair.key.provenance == 777 && pair.value.payload == 222);
    }
    snapshot_count++;
  }
  CHECK(snapshot_count == 6u);
  CHECK(hxc_iterator_ref_release(alias) == HXC_STATUS_OK);
  CHECK(hxc_iterator_ref_release(iterator) == HXC_STATUS_OK);
  iterator = NULL;

  {
    const test_key key = {9, 109};
    const test_value value = {90};
    CHECK(hxc_typed_map_ref_set_copy(map, &key, &value) == HXC_STATUS_OK);
  }
  CHECK(hxc_typed_map_ref_copy(map, &copy) == HXC_STATUS_OK);
  CHECK(copy != NULL && copy != map);
  {
    const test_key key = {9, 999};
    const test_value changed = {91};
    CHECK(hxc_typed_map_ref_set_copy(copy, &key, &changed) == HXC_STATUS_OK);
    CHECK(hxc_typed_map_ref_get_copy(
      map,
      &key,
      &observed,
      &found
    ) == HXC_STATUS_OK);
    CHECK(found && observed.payload == 90);
  }
  state.fail_at = state.successful_allocations;
  {
    hxc_typed_map_ref *failed_copy = NULL;
    CHECK(hxc_typed_map_ref_copy(
      map,
      &failed_copy
    ) == HXC_STATUS_OUT_OF_MEMORY);
    CHECK(failed_copy == NULL);
  }
  state.fail_at = SIZE_MAX;
  CHECK(hxc_typed_map_ref_release(copy) == HXC_STATUS_OK);
  CHECK(hxc_typed_map_ref_release(map) == HXC_STATUS_OK);
  CHECK(state.successful_allocations == state.releases);
  return 0;
}

static int prove_lifecycle_replacement_rollback(void) {
  test_allocator_state allocator_state = {0u, 0u, SIZE_MAX};
  test_policy_state key_state = {0u, 0u, 0u, false, false};
  test_policy_state value_state = {0u, 0u, 0u, false, false};
  const hxc_allocator allocator = {
    &allocator_state,
    test_allocate,
    NULL,
    test_release
  };
  hxc_typed_map_ref *map = NULL;
  test_key key = {4, 40};
  test_value value = {400};
  test_value observed = {0};
  bool found = false;

  CHECK(hxc_typed_map_ref_create(
    allocator,
    tracked_key_ops(&key_state),
    tracked_value_ops(&value_state),
    &map
  ) == HXC_STATUS_OK);
  CHECK(hxc_typed_map_ref_set_copy(map, &key, &value) == HXC_STATUS_OK);

  key.provenance = 41;
  value.payload = 401;
  allocator_state.fail_at = allocator_state.successful_allocations;
  CHECK(hxc_typed_map_ref_set_copy(
    map,
    &key,
    &value
  ) == HXC_STATUS_OUT_OF_MEMORY);
  allocator_state.fail_at = SIZE_MAX;
  CHECK(hxc_typed_map_ref_get_copy(
    map,
    &key,
    &observed,
    &found
  ) == HXC_STATUS_OK);
  CHECK(found && observed.payload == 400);
  tracked_destroy(&value_state, &observed);

  key_state.fail_next_copy = true;
  CHECK(hxc_typed_map_ref_set_copy(
    map,
    &key,
    &value
  ) == HXC_STATUS_OUT_OF_MEMORY);
  value_state.fail_next_copy = true;
  CHECK(hxc_typed_map_ref_set_copy(
    map,
    &key,
    &value
  ) == HXC_STATUS_OUT_OF_MEMORY);
  CHECK(hxc_typed_map_ref_get_copy(
    map,
    &key,
    &observed,
    &found
  ) == HXC_STATUS_OK);
  CHECK(found && observed.payload == 400);
  tracked_destroy(&value_state, &observed);

  CHECK(hxc_typed_map_ref_set_copy(map, &key, &value) == HXC_STATUS_OK);
  CHECK(hxc_typed_map_ref_get_copy(
    map,
    &key,
    &observed,
    &found
  ) == HXC_STATUS_OK);
  CHECK(found && observed.payload == 401);
  tracked_destroy(&value_state, &observed);
  CHECK(hxc_typed_map_ref_release(map) == HXC_STATUS_OK);
  CHECK(!key_state.broken && !value_state.broken);
  CHECK(key_state.live_owners == 0u);
  CHECK(value_state.live_owners == 0u);
  CHECK(key_state.copies == key_state.destroys);
  CHECK(value_state.copies == value_state.destroys);
  CHECK(allocator_state.successful_allocations == allocator_state.releases);
  return 0;
}

static uint64_t object_pointer_hash(void *context, const void *key) {
  test_object *const *object = key;
  (void)context;
  return hxc_typed_map_identity_hash(*object);
}

static bool object_pointer_equal(
  void *context,
  const void *left,
  const void *right
) {
  test_object *const *left_object = left;
  test_object *const *right_object = right;
  (void)context;
  return *left_object == *right_object;
}

static void object_pointer_trace(
  void *context,
  const void *value,
  hxc_trace_visit_fn visit,
  void *visit_context
) {
  test_object *const *object = value;
  (void)context;
  if (*object != NULL) {
    visit(visit_context, *object);
  }
}

static void finalize_test_object(void *object) {
  test_object *value = object;
  if (value != NULL) {
    finalized_test_objects++;
  }
}

static const hxc_type_descriptor TEST_OBJECT_DESCRIPTOR = {
  HXC_TYPE_DESCRIPTOR_ABI_VERSION,
  HXC_TYPE_DESCRIPTOR_HAS_FINALIZER,
  sizeof(test_object),
  HXC_ALIGNOF(test_object),
  NULL,
  finalize_test_object
};

static hxc_typed_map_key_ops object_key_ops(void) {
  const hxc_typed_map_key_ops result = {
    sizeof(test_object *),
    HXC_ALIGNOF(test_object *),
    NULL,
    NULL,
    NULL,
    object_pointer_trace,
    object_pointer_hash,
    object_pointer_equal
  };
  return result;
}

static hxc_typed_map_value_ops object_value_ops(void) {
  const hxc_typed_map_value_ops result = {
    sizeof(test_object *),
    HXC_ALIGNOF(test_object *),
    NULL,
    NULL,
    NULL,
    object_pointer_trace
  };
  return result;
}

static int allocate_test_object(
  hxc_gc *collector,
  int32_t identity,
  test_object **out_object
) {
  void *storage = NULL;
  if (hxc_gc_allocate(
    collector,
    &TEST_OBJECT_DESCRIPTOR,
    &storage
  ) != HXC_STATUS_OK) {
    return 1;
  }
  *out_object = storage;
  (*out_object)->identity = identity;
  return 0;
}

static int prove_collector_and_snapshot_reachability(void) {
  hxc_gc collector = HXC_GC_INITIALIZER;
  const hxc_gc_config config = {
    hxc_default_allocator(),
    SIZE_MAX,
    NULL,
    NULL
  };
  hxc_typed_map_ref *map = NULL;
  hxc_gc_root_table roots = HXC_GC_ROOT_TABLE_INITIALIZER;
  const void *root_slots[3] = {NULL, NULL, NULL};
  hxc_iterator_ref *iterator = NULL;
  hxc_iterator_element_ops pair_ops = {
    sizeof(test_object_pair),
    HXC_ALIGNOF(test_object_pair),
    NULL,
    NULL,
    NULL,
    NULL
  };
  hxc_gc_stats stats = HXC_GC_STATS_INITIALIZER;
  test_object *key = NULL;
  test_object *value = NULL;
  bool removed = false;

  finalized_test_objects = 0u;
  CHECK(hxc_gc_init(&config, &collector) == HXC_STATUS_OK);
  {
    void *storage = NULL;
    CHECK(hxc_gc_allocate(
      &collector,
      hxc_typed_map_type_descriptor(),
      &storage
    ) == HXC_STATUS_OK);
    map = storage;
  }
  root_slots[0] = map;
  CHECK(hxc_gc_root_table_register(
    &collector,
    root_slots,
    3u,
    &roots
  ) == HXC_STATUS_OK);
  CHECK(hxc_typed_map_init_collector_owned(
    &collector,
    hxc_default_allocator(),
    object_key_ops(),
    object_value_ops(),
    map
  ) == HXC_STATUS_OK);
  CHECK(allocate_test_object(&collector, 11, &key) == 0);
  root_slots[1] = key;
  CHECK(allocate_test_object(&collector, 12, &value) == 0);
  root_slots[2] = value;
  CHECK(hxc_typed_map_ref_set_copy(map, &key, &value) == HXC_STATUS_OK);
  root_slots[1] = NULL;
  root_slots[2] = NULL;
  key = NULL;
  value = NULL;
  CHECK(hxc_gc_collect(&collector) == HXC_STATUS_OK);
  CHECK(finalized_test_objects == 0u);

  CHECK(hxc_typed_map_ref_pair_iterator(
    map,
    pair_ops,
    offsetof(test_object_pair, key),
    offsetof(test_object_pair, value),
    &iterator
  ) == HXC_STATUS_OK);
  CHECK(hxc_typed_map_ref_clear(map) == HXC_STATUS_OK);
  CHECK(hxc_gc_collect(&collector) == HXC_STATUS_OK);
  CHECK(finalized_test_objects == 0u);
  {
    test_object_pair pair = {NULL, NULL};
    CHECK(hxc_iterator_ref_next_move(iterator, &pair) == HXC_STATUS_OK);
    CHECK(pair.key->identity == 11 && pair.value->identity == 12);
  }
  CHECK(hxc_iterator_ref_release(iterator) == HXC_STATUS_OK);
  iterator = NULL;
  CHECK(hxc_gc_collect(&collector) == HXC_STATUS_OK);
  CHECK(finalized_test_objects == 2u);

  CHECK(allocate_test_object(&collector, 21, &key) == 0);
  root_slots[1] = key;
  CHECK(allocate_test_object(&collector, 22, &value) == 0);
  root_slots[2] = value;
  CHECK(hxc_typed_map_ref_set_copy(map, &key, &value) == HXC_STATUS_OK);
  root_slots[1] = NULL;
  root_slots[2] = NULL;
  CHECK(hxc_typed_map_ref_remove(map, &key, &removed) == HXC_STATUS_OK);
  CHECK(removed);
  key = NULL;
  value = NULL;
  CHECK(hxc_gc_collect(&collector) == HXC_STATUS_OK);
  CHECK(finalized_test_objects == 4u);
  CHECK(hxc_gc_get_stats(&collector, &stats) == HXC_STATUS_OK);
  CHECK(stats.current_object_count == 1u);

  root_slots[0] = NULL;
  CHECK(hxc_gc_collect(&collector) == HXC_STATUS_OK);
  CHECK(hxc_gc_get_stats(&collector, &stats) == HXC_STATUS_OK);
  CHECK(stats.current_object_count == 0u);
  CHECK(hxc_gc_root_table_unregister(&roots) == HXC_STATUS_OK);
  CHECK(hxc_gc_dispose(&collector) == HXC_STATUS_OK);
  return 0;
}

int main(void) {
  if (prove_collisions_snapshots_and_allocator_failure() != 0) {
    return 1;
  }
  if (prove_lifecycle_replacement_rollback() != 0) {
    return 1;
  }
  return prove_collector_and_snapshot_reachability();
}
