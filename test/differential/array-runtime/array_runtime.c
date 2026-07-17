#include "hxrt/array.h"

#include <inttypes.h>
#include <stdio.h>

#define HXC_TEST_CHECK(condition) \
  do { \
    if (!(condition)) { \
      (void)fprintf(stderr, "array-runtime: check failed at line %d\n", __LINE__); \
      return 1; \
    } \
  } while (0)

#define HXC_TEST_BANK_SIZE 4096u
#define HXC_TEST_BANK_COUNT 2u

typedef union hxc_test_bank {
  max_align_t alignment;
  unsigned char bytes[HXC_TEST_BANK_SIZE];
} hxc_test_bank;

typedef struct hxc_test_arena {
  hxc_test_bank banks[HXC_TEST_BANK_COUNT];
  bool used[HXC_TEST_BANK_COUNT];
  size_t sizes[HXC_TEST_BANK_COUNT];
  size_t alignments[HXC_TEST_BANK_COUNT];
  bool force_failure;
  bool invalid_release;
  size_t allocation_count;
  size_t reallocation_count;
  size_t release_count;
} hxc_test_arena;

typedef struct hxc_test_object {
  int identifier;
  size_t references;
} hxc_test_object;

typedef struct hxc_test_lifecycle {
  bool copy_failure_armed;
  size_t copies_before_failure;
  bool assign_failure;
  bool invalid_release;
  size_t copies;
  size_t assignments;
  size_t destructions;
} hxc_test_lifecycle;

static hxc_status hxc_test_allocate(
  void *context,
  size_t size,
  size_t alignment,
  void **out_memory
) {
  hxc_test_arena *arena = (hxc_test_arena *)context;
  size_t index;
  if (arena->force_failure) {
    return HXC_STATUS_OUT_OF_MEMORY;
  }
  if (size == 0u || size > HXC_TEST_BANK_SIZE || out_memory == NULL) {
    return HXC_STATUS_OUT_OF_MEMORY;
  }
  if (alignment > HXC_ALIGNOF(max_align_t)) {
    return HXC_STATUS_UNSUPPORTED_ALIGNMENT;
  }
  for (index = 0u; index < HXC_TEST_BANK_COUNT; index++) {
    if (!arena->used[index]) {
      arena->used[index] = true;
      arena->sizes[index] = size;
      arena->alignments[index] = alignment;
      arena->allocation_count++;
      *out_memory = arena->banks[index].bytes;
      return HXC_STATUS_OK;
    }
  }
  return HXC_STATUS_OUT_OF_MEMORY;
}

static void hxc_test_release(
  void *context,
  void *memory,
  size_t size,
  size_t alignment
) {
  hxc_test_arena *arena = (hxc_test_arena *)context;
  size_t index;
  for (index = 0u; index < HXC_TEST_BANK_COUNT; index++) {
    if (memory == arena->banks[index].bytes) {
      if (!arena->used[index]
        || arena->sizes[index] != size
        || arena->alignments[index] != alignment) {
        arena->invalid_release = true;
        return;
      }
      arena->used[index] = false;
      arena->sizes[index] = 0u;
      arena->alignments[index] = 0u;
      arena->release_count++;
      return;
    }
  }
  arena->invalid_release = true;
}

static hxc_status hxc_test_reallocate(
  void *context,
  void *memory,
  size_t old_size,
  size_t new_size,
  size_t alignment,
  void **out_memory
) {
  hxc_test_arena *arena = (hxc_test_arena *)context;
  void *replacement = NULL;
  size_t copy_size;
  size_t index;
  hxc_status status;
  if (arena->force_failure) {
    return HXC_STATUS_OUT_OF_MEMORY;
  }
  status = hxc_test_allocate(
    context,
    new_size,
    alignment,
    &replacement
  );
  if (status != HXC_STATUS_OK) {
    return status;
  }
  copy_size = old_size < new_size ? old_size : new_size;
  for (index = 0u; index < copy_size; index++) {
    ((unsigned char *)replacement)[index] =
      ((const unsigned char *)memory)[index];
  }
  hxc_test_release(context, memory, old_size, alignment);
  if (arena->invalid_release) {
    hxc_test_release(context, replacement, new_size, alignment);
    return HXC_STATUS_INTERNAL_ERROR;
  }
  arena->reallocation_count++;
  *out_memory = replacement;
  return HXC_STATUS_OK;
}

static hxc_allocator hxc_test_allocator(hxc_test_arena *arena) {
  hxc_allocator allocator;
  allocator.context = arena;
  allocator.allocate = hxc_test_allocate;
  allocator.reallocate = hxc_test_reallocate;
  allocator.release = hxc_test_release;
  return allocator;
}

static hxc_array_element_ops hxc_test_i32_ops(void) {
  hxc_array_element_ops elements;
  elements.size = sizeof(int32_t);
  elements.alignment = HXC_ALIGNOF(int32_t);
  elements.context = NULL;
  elements.copy = NULL;
  elements.assign = NULL;
  elements.destroy = NULL;
  return elements;
}

static void hxc_test_retain(hxc_test_object *object) {
  if (object != NULL) {
    object->references++;
  }
}

static void hxc_test_drop(
  hxc_test_lifecycle *lifecycle,
  hxc_test_object *object
) {
  if (object != NULL) {
    if (object->references == 0u) {
      lifecycle->invalid_release = true;
    } else {
      object->references--;
    }
  }
}

static hxc_status hxc_test_ref_copy(
  void *context,
  void *destination,
  const void *source
) {
  hxc_test_lifecycle *lifecycle = (hxc_test_lifecycle *)context;
  hxc_test_object *value = *(hxc_test_object *const *)source;
  if (lifecycle->copy_failure_armed) {
    if (lifecycle->copies_before_failure == 0u) {
      return HXC_STATUS_OUT_OF_MEMORY;
    }
    lifecycle->copies_before_failure--;
  }
  hxc_test_retain(value);
  *(hxc_test_object **)destination = value;
  lifecycle->copies++;
  return HXC_STATUS_OK;
}

static hxc_status hxc_test_ref_assign(
  void *context,
  void *destination,
  const void *source
) {
  hxc_test_lifecycle *lifecycle = (hxc_test_lifecycle *)context;
  hxc_test_object *replacement = *(hxc_test_object *const *)source;
  hxc_test_object *previous = *(hxc_test_object **)destination;
  if (lifecycle->assign_failure) {
    return HXC_STATUS_OUT_OF_MEMORY;
  }
  hxc_test_retain(replacement);
  hxc_test_drop(lifecycle, previous);
  *(hxc_test_object **)destination = replacement;
  lifecycle->assignments++;
  return HXC_STATUS_OK;
}

static void hxc_test_ref_destroy(void *context, void *element) {
  hxc_test_lifecycle *lifecycle = (hxc_test_lifecycle *)context;
  hxc_test_object *value = *(hxc_test_object **)element;
  hxc_test_drop(lifecycle, value);
  *(hxc_test_object **)element = NULL;
  lifecycle->destructions++;
}

static hxc_array_element_ops hxc_test_ref_ops(
  hxc_test_lifecycle *lifecycle
) {
  hxc_array_element_ops elements;
  elements.size = sizeof(hxc_test_object *);
  elements.alignment = HXC_ALIGNOF(hxc_test_object *);
  elements.context = lifecycle;
  elements.copy = hxc_test_ref_copy;
  elements.assign = hxc_test_ref_assign;
  elements.destroy = hxc_test_ref_destroy;
  return elements;
}

static int hxc_test_expect_i32(
  const hxc_array *array,
  const int32_t *expected,
  size_t length
) {
  size_t index;
  if (!hxc_array_is_valid(array) || array->length != length) {
    return 1;
  }
  for (index = 0u; index < length; index++) {
    const void *element = NULL;
    if (hxc_array_at_const(array, index, &element) != HXC_STATUS_OK
      || *(const int32_t *)element != expected[index]) {
      return 1;
    }
  }
  return 0;
}

static int hxc_test_expect_refs(
  const hxc_array *array,
  const int *expected,
  size_t length
) {
  size_t index;
  if (!hxc_array_is_valid(array) || array->length != length) {
    return 1;
  }
  for (index = 0u; index < length; index++) {
    const void *element = NULL;
    hxc_test_object *value;
    if (hxc_array_at_const(array, index, &element) != HXC_STATUS_OK) {
      return 1;
    }
    value = *(hxc_test_object *const *)element;
    if ((expected[index] == 0 && value != NULL)
      || (expected[index] != 0
        && (value == NULL || value->identifier != expected[index]))) {
      return 1;
    }
  }
  return 0;
}

static int hxc_test_invalid_contract(const hxc_allocator *allocator) {
  hxc_array array = HXC_ARRAY_INITIALIZER;
  hxc_array_element_ops elements = hxc_test_i32_ops();

  elements.size = 0u;
  HXC_TEST_CHECK(!hxc_array_element_ops_is_valid(&elements));
  HXC_TEST_CHECK(
    hxc_array_init(allocator, elements, &array)
      == HXC_STATUS_INVALID_ARGUMENT
  );
  HXC_TEST_CHECK(hxc_array_dispose(&array) == HXC_STATUS_OK);

  elements = hxc_test_i32_ops();
  elements.copy = hxc_test_ref_copy;
  HXC_TEST_CHECK(!hxc_array_element_ops_is_valid(&elements));
  HXC_TEST_CHECK(
    hxc_array_init(allocator, elements, &array)
      == HXC_STATUS_INVALID_ARGUMENT
  );
  return 0;
}

static int hxc_test_reference_array(
  hxc_test_arena *arena,
  const hxc_allocator *allocator
) {
  static const int after_insert[] = {1, 1, 2, 1, 2, 1};
  static const int after_assign[] = {1, 1, 1, 1, 2, 1};
  static const int after_shrink[] = {1, 1, 1};
  static const int after_remove[] = {1, 1};
  static const int with_null_defaults[] = {1, 1, 0, 0};
  hxc_test_lifecycle lifecycle = {0};
  hxc_test_object first = {1, 1u};
  hxc_test_object second = {2, 1u};
  hxc_test_object *first_value = &first;
  hxc_test_object *second_value = &second;
  hxc_test_object *null_value = NULL;
  hxc_array array = HXC_ARRAY_INITIALIZER;
  hxc_array_element_ops elements = hxc_test_ref_ops(&lifecycle);
  const void *source = NULL;
  size_t length_before;
  size_t capacity_before;

  HXC_TEST_CHECK(hxc_array_element_ops_is_valid(&elements));
  HXC_TEST_CHECK(
    hxc_array_init(allocator, elements, &array) == HXC_STATUS_OK
  );
  HXC_TEST_CHECK(array.length == 0u && array.capacity == 0u);
  HXC_TEST_CHECK(hxc_array_push_copy(&array, &first_value) == HXC_STATUS_OK);
  HXC_TEST_CHECK(hxc_array_push_copy(&array, &second_value) == HXC_STATUS_OK);
  HXC_TEST_CHECK(hxc_array_push_copy(&array, &first_value) == HXC_STATUS_OK);
  HXC_TEST_CHECK(hxc_array_push_copy(&array, &second_value) == HXC_STATUS_OK);
  HXC_TEST_CHECK(
    hxc_array_at_const(&array, 0u, &source) == HXC_STATUS_OK
  );
  HXC_TEST_CHECK(hxc_array_push_copy(&array, source) == HXC_STATUS_OK);
  HXC_TEST_CHECK(arena->reallocation_count != 0u);
  HXC_TEST_CHECK(
    hxc_array_at_const(&array, array.length - 1u, &source) == HXC_STATUS_OK
  );
  HXC_TEST_CHECK(
    hxc_array_insert_copy(&array, 1u, source) == HXC_STATUS_OK
  );
  HXC_TEST_CHECK(
    hxc_test_expect_refs(&array, after_insert, 6u) == 0
  );
  HXC_TEST_CHECK(first.references == 5u && second.references == 3u);

  HXC_TEST_CHECK(
    hxc_array_at_const(&array, 0u, &source) == HXC_STATUS_OK
  );
  HXC_TEST_CHECK(hxc_array_set_copy(&array, 2u, source) == HXC_STATUS_OK);
  HXC_TEST_CHECK(hxc_test_expect_refs(&array, after_assign, 6u) == 0);
  HXC_TEST_CHECK(first.references == 6u && second.references == 2u);
  HXC_TEST_CHECK(hxc_array_set_copy(&array, 0u, source) == HXC_STATUS_OK);

  lifecycle.assign_failure = true;
  HXC_TEST_CHECK(
    hxc_array_set_copy(&array, 4u, source) == HXC_STATUS_OUT_OF_MEMORY
  );
  lifecycle.assign_failure = false;
  HXC_TEST_CHECK(hxc_test_expect_refs(&array, after_assign, 6u) == 0);
  HXC_TEST_CHECK(first.references == 6u && second.references == 2u);

  length_before = array.length;
  lifecycle.copy_failure_armed = true;
  lifecycle.copies_before_failure = 0u;
  HXC_TEST_CHECK(
    hxc_array_insert_copy(&array, 2u, source) == HXC_STATUS_OUT_OF_MEMORY
  );
  lifecycle.copy_failure_armed = false;
  HXC_TEST_CHECK(array.length == length_before);
  HXC_TEST_CHECK(hxc_test_expect_refs(&array, after_assign, 6u) == 0);
  HXC_TEST_CHECK(first.references == 6u && second.references == 2u);

  lifecycle.copy_failure_armed = true;
  lifecycle.copies_before_failure = 1u;
  capacity_before = array.capacity;
  HXC_TEST_CHECK(
    hxc_array_resize(&array, 8u, &second_value)
      == HXC_STATUS_OUT_OF_MEMORY
  );
  lifecycle.copy_failure_armed = false;
  HXC_TEST_CHECK(array.length == 6u && array.capacity >= capacity_before);
  HXC_TEST_CHECK(hxc_test_expect_refs(&array, after_assign, 6u) == 0);
  HXC_TEST_CHECK(first.references == 6u && second.references == 2u);

  HXC_TEST_CHECK(
    hxc_array_resize(&array, 8u, &second_value) == HXC_STATUS_OK
  );
  HXC_TEST_CHECK(first.references == 6u && second.references == 4u);
  HXC_TEST_CHECK(hxc_array_resize(&array, 3u, NULL) == HXC_STATUS_OK);
  HXC_TEST_CHECK(hxc_test_expect_refs(&array, after_shrink, 3u) == 0);
  HXC_TEST_CHECK(first.references == 4u && second.references == 1u);
  HXC_TEST_CHECK(hxc_array_remove_at(&array, 1u) == HXC_STATUS_OK);
  HXC_TEST_CHECK(hxc_test_expect_refs(&array, after_remove, 2u) == 0);
  HXC_TEST_CHECK(first.references == 3u && second.references == 1u);

  HXC_TEST_CHECK(
    hxc_array_resize(&array, 4u, &null_value) == HXC_STATUS_OK
  );
  HXC_TEST_CHECK(
    hxc_test_expect_refs(&array, with_null_defaults, 4u) == 0
  );
  HXC_TEST_CHECK(first.references == 3u && second.references == 1u);
  HXC_TEST_CHECK(hxc_array_resize(&array, 2u, NULL) == HXC_STATUS_OK);

  HXC_TEST_CHECK(hxc_array_dispose(&array) == HXC_STATUS_OK);
  HXC_TEST_CHECK(first.references == 1u && second.references == 1u);
  HXC_TEST_CHECK(!lifecycle.invalid_release && !arena->invalid_release);
  hxc_test_drop(&lifecycle, &first);
  hxc_test_drop(&lifecycle, &second);
  HXC_TEST_CHECK(first.references == 0u && second.references == 0u);
  HXC_TEST_CHECK(!lifecycle.invalid_release);
  return 0;
}

static int hxc_test_primitive_array(
  hxc_test_arena *arena,
  const hxc_allocator *allocator,
  hxc_array *out_array
) {
  static const int32_t initial[] = {1, 2, 3, 4};
  static const int32_t after_push[] = {1, 2, 3, 4, 1};
  static const int32_t after_insert[] = {1, 3, 2, 3, 4, 1};
  static const int32_t after_set[] = {1, 3, 1, 3, 4, 1};
  static const int32_t after_remove_four[] = {1, 3, 1, 3, 1};
  static const int32_t before_resize[] = {1, 3, 1, 1};
  static const int32_t final_values[] = {1, 3, 1, 1, 0};
  hxc_array array = HXC_ARRAY_INITIALIZER;
  hxc_array_element_ops elements = hxc_test_i32_ops();
  void *mutable_element = &array;
  const void *source = NULL;
  int32_t zero = 0;
  size_t index;
  size_t length_before;
  size_t capacity_before;
  void *memory_before;

  HXC_TEST_CHECK(
    hxc_array_init(allocator, elements, &array) == HXC_STATUS_OK
  );
  HXC_TEST_CHECK(hxc_array_is_valid(&array));
  HXC_TEST_CHECK(
    hxc_array_at(&array, 0u, &mutable_element) == HXC_STATUS_OUT_OF_RANGE
  );
  HXC_TEST_CHECK(mutable_element == &array);
  for (index = 0u; index < sizeof(initial) / sizeof(initial[0]); index++) {
    HXC_TEST_CHECK(
      hxc_array_push_copy(&array, &initial[index]) == HXC_STATUS_OK
    );
  }
  HXC_TEST_CHECK(hxc_test_expect_i32(&array, initial, 4u) == 0);
  HXC_TEST_CHECK(
    hxc_array_at_const(&array, 0u, &source) == HXC_STATUS_OK
  );
  HXC_TEST_CHECK(hxc_array_push_copy(&array, source) == HXC_STATUS_OK);
  HXC_TEST_CHECK(hxc_test_expect_i32(&array, after_push, 5u) == 0);

  HXC_TEST_CHECK(
    hxc_array_at_const(&array, 2u, &source) == HXC_STATUS_OK
  );
  HXC_TEST_CHECK(
    hxc_array_insert_copy(&array, 1u, source) == HXC_STATUS_OK
  );
  HXC_TEST_CHECK(hxc_test_expect_i32(&array, after_insert, 6u) == 0);
  HXC_TEST_CHECK(
    hxc_array_at_const(&array, array.length - 1u, &source) == HXC_STATUS_OK
  );
  HXC_TEST_CHECK(hxc_array_set_copy(&array, 2u, source) == HXC_STATUS_OK);
  HXC_TEST_CHECK(hxc_test_expect_i32(&array, after_set, 6u) == 0);
  HXC_TEST_CHECK(hxc_array_remove_at(&array, 4u) == HXC_STATUS_OK);
  HXC_TEST_CHECK(
    hxc_test_expect_i32(&array, after_remove_four, 5u) == 0
  );
  HXC_TEST_CHECK(hxc_array_remove_at(&array, 3u) == HXC_STATUS_OK);
  HXC_TEST_CHECK(hxc_test_expect_i32(&array, before_resize, 4u) == 0);

  HXC_TEST_CHECK(hxc_array_resize(&array, 6u, &zero) == HXC_STATUS_OK);
  HXC_TEST_CHECK(hxc_array_resize(&array, 5u, NULL) == HXC_STATUS_OK);
  HXC_TEST_CHECK(hxc_test_expect_i32(&array, final_values, 5u) == 0);

  length_before = array.length;
  capacity_before = array.capacity;
  memory_before = array.storage.memory;
  arena->force_failure = true;
  HXC_TEST_CHECK(
    hxc_array_reserve(&array, capacity_before + 1u)
      == HXC_STATUS_OUT_OF_MEMORY
  );
  arena->force_failure = false;
  HXC_TEST_CHECK(
    array.length == length_before
    && array.capacity == capacity_before
    && array.storage.memory == memory_before
  );
  HXC_TEST_CHECK(hxc_test_expect_i32(&array, final_values, 5u) == 0);
  HXC_TEST_CHECK(
    hxc_array_reserve(&array, (SIZE_MAX / sizeof(int32_t)) + 1u)
      == HXC_STATUS_SIZE_OVERFLOW
  );
  HXC_TEST_CHECK(hxc_test_expect_i32(&array, final_values, 5u) == 0);
  HXC_TEST_CHECK(hxc_array_remove_at(&array, 5u) == HXC_STATUS_OUT_OF_RANGE);
  HXC_TEST_CHECK(hxc_array_move(&array, out_array) == HXC_STATUS_OK);
  HXC_TEST_CHECK(hxc_array_dispose(&array) == HXC_STATUS_OK);
  return 0;
}

int main(void) {
  hxc_test_arena arena = {0};
  hxc_allocator allocator = hxc_test_allocator(&arena);
  hxc_array values = HXC_ARRAY_INITIALIZER;
  const void *element = NULL;
  size_t index;

  HXC_TEST_CHECK(hxc_allocator_is_valid(&allocator));
  HXC_TEST_CHECK(hxc_test_invalid_contract(&allocator) == 0);
  HXC_TEST_CHECK(hxc_test_reference_array(&arena, &allocator) == 0);
  HXC_TEST_CHECK(hxc_test_primitive_array(&arena, &allocator, &values) == 0);
  HXC_TEST_CHECK(hxc_array_is_valid(&values));
  HXC_TEST_CHECK(!arena.invalid_release);

  (void)printf("%zu:", values.length);
  for (index = 0u; index < values.length; index++) {
    HXC_TEST_CHECK(
      hxc_array_at_const(&values, index, &element) == HXC_STATUS_OK
    );
    if (index != 0u) {
      (void)putchar(',');
    }
    (void)printf("%" PRId32, *(const int32_t *)element);
  }
  (void)putchar('\n');
  HXC_TEST_CHECK(hxc_array_dispose(&values) == HXC_STATUS_OK);
  HXC_TEST_CHECK(!arena.invalid_release);
  HXC_TEST_CHECK(arena.allocation_count == arena.release_count);
  HXC_TEST_CHECK(!arena.used[0] && !arena.used[1]);
  return 0;
}
