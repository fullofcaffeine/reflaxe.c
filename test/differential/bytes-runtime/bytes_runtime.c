#include "hxrt/bytes.h"

#include <stdio.h>

#define HXC_TEST_CHECK(condition) \
  do { \
    if (!(condition)) { \
      (void)fprintf(stderr, "bytes-runtime: check failed at line %d\n", __LINE__); \
      return 1; \
    } \
  } while (0)

#define HXC_TEST_BANK_SIZE 512u
#define HXC_TEST_BANK_COUNT 10u

typedef union hxc_test_bank {
  max_align_t alignment;
  unsigned char bytes[HXC_TEST_BANK_SIZE];
} hxc_test_bank;

typedef struct hxc_test_arena {
  hxc_test_bank banks[HXC_TEST_BANK_COUNT];
  bool used[HXC_TEST_BANK_COUNT];
  size_t sizes[HXC_TEST_BANK_COUNT];
  size_t alignments[HXC_TEST_BANK_COUNT];
  size_t successful_allocations_before_failure;
  bool failure_armed;
  bool invalid_release;
  size_t allocation_count;
  size_t release_count;
} hxc_test_arena;

static hxc_status hxc_test_allocate(
  void *context,
  size_t size,
  size_t alignment,
  void **out_memory
) {
  hxc_test_arena *arena = (hxc_test_arena *)context;
  size_t index;
  if (arena->failure_armed) {
    if (arena->successful_allocations_before_failure == 0u) {
      return HXC_STATUS_OUT_OF_MEMORY;
    }
    arena->successful_allocations_before_failure--;
  }
  if (size == 0u
    || size > HXC_TEST_BANK_SIZE
    || alignment > HXC_ALIGNOF(max_align_t)
    || out_memory == NULL) {
    return HXC_STATUS_OUT_OF_MEMORY;
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

static hxc_allocator hxc_test_allocator(hxc_test_arena *arena) {
  hxc_allocator allocator;
  allocator.context = arena;
  allocator.allocate = hxc_test_allocate;
  allocator.reallocate = NULL;
  allocator.release = hxc_test_release;
  return allocator;
}

static int hxc_expect_byte(
  const hxc_bytes_ref *bytes,
  int32_t position,
  int32_t expected
) {
  int32_t actual = -1;
  return hxc_bytes_ref_get(bytes, position, &actual) != HXC_STATUS_OK
    || actual != expected;
}

int main(void) {
  hxc_test_arena arena = {0};
  const hxc_allocator allocator = hxc_test_allocator(&arena);
  hxc_bytes_ref *bytes = NULL;
  hxc_bytes_ref *copy = NULL;
  hxc_bytes_ref *shorter = NULL;
  hxc_bytes_ref *text = NULL;
  hxc_bytes_ref *failed = NULL;
  int32_t value = -1;
  int32_t order = 7;
  const uint8_t shorter_data[] = {UINT8_C(255)};
  const uint8_t text_data[] = {UINT8_C(72), UINT8_C(0), UINT8_C(120)};
  const hxc_string text_view = {text_data, 3u, true};

  HXC_TEST_CHECK(hxc_bytes_ref_create_zeroed(allocator, 8, &bytes) == HXC_STATUS_OK);
  HXC_TEST_CHECK(hxc_bytes_ref_is_valid(bytes));
  HXC_TEST_CHECK(hxc_bytes_ref_length(bytes, &value) == HXC_STATUS_OK && value == 8);
  HXC_TEST_CHECK(!hxc_expect_byte(bytes, 0, 0) && !hxc_expect_byte(bytes, 7, 0));
  HXC_TEST_CHECK(hxc_bytes_ref_retain(bytes) == HXC_STATUS_OK && bytes->references == 2u);
  HXC_TEST_CHECK(hxc_bytes_ref_release(bytes) == HXC_STATUS_OK && bytes->references == 1u);

  HXC_TEST_CHECK(hxc_bytes_ref_set(bytes, 0, 0x141) == HXC_STATUS_OK);
  HXC_TEST_CHECK(hxc_bytes_ref_set(bytes, 1, -1) == HXC_STATUS_OK);
  HXC_TEST_CHECK(!hxc_expect_byte(bytes, 0, 0x41) && !hxc_expect_byte(bytes, 1, 0xff));
  HXC_TEST_CHECK(hxc_bytes_ref_fill(bytes, 2, 4, 0x122) == HXC_STATUS_OK);
  HXC_TEST_CHECK(hxc_bytes_ref_blit(bytes, 3, bytes, 0, 5) == HXC_STATUS_OK);
  HXC_TEST_CHECK(!hxc_expect_byte(bytes, 3, 0x41));
  HXC_TEST_CHECK(!hxc_expect_byte(bytes, 4, 0xff));
  HXC_TEST_CHECK(!hxc_expect_byte(bytes, 7, 0x22));
  HXC_TEST_CHECK(hxc_bytes_ref_blit(bytes, 0, bytes, 2, 4) == HXC_STATUS_OK);
  HXC_TEST_CHECK(!hxc_expect_byte(bytes, 0, 0x22));
  HXC_TEST_CHECK(!hxc_expect_byte(bytes, 1, 0x41));

  HXC_TEST_CHECK(hxc_bytes_ref_sub(bytes, 0, 4, &copy) == HXC_STATUS_OK);
  HXC_TEST_CHECK(hxc_bytes_ref_set(copy, 0, 0x33) == HXC_STATUS_OK);
  HXC_TEST_CHECK(!hxc_expect_byte(copy, 0, 0x33) && !hxc_expect_byte(bytes, 0, 0x22));
  HXC_TEST_CHECK(hxc_bytes_ref_create_copy(allocator, shorter_data, 1u, &shorter) == HXC_STATUS_OK);
  HXC_TEST_CHECK(hxc_bytes_ref_compare(shorter, copy, &order) == HXC_STATUS_OK && order > 0);
  HXC_TEST_CHECK(hxc_bytes_ref_compare(copy, copy, &order) == HXC_STATUS_OK && order == 0);
  HXC_TEST_CHECK(hxc_bytes_ref_create_utf8_copy(allocator, text_view, &text) == HXC_STATUS_OK);
  HXC_TEST_CHECK(!hxc_expect_byte(text, 0, 72) && !hxc_expect_byte(text, 1, 0) && !hxc_expect_byte(text, 2, 120));

  value = 99;
  HXC_TEST_CHECK(hxc_bytes_ref_get(bytes, -1, &value) == HXC_STATUS_OUT_OF_RANGE && value == 99);
  HXC_TEST_CHECK(hxc_bytes_ref_get(bytes, 8, &value) == HXC_STATUS_OUT_OF_RANGE && value == 99);
  HXC_TEST_CHECK(hxc_bytes_ref_get(NULL, 0, &value) == HXC_STATUS_INVALID_ARGUMENT && value == 99);
  HXC_TEST_CHECK(hxc_bytes_ref_set(bytes, 8, 0) == HXC_STATUS_OUT_OF_RANGE);
  HXC_TEST_CHECK(hxc_bytes_ref_fill(bytes, 7, 2, 0) == HXC_STATUS_OUT_OF_RANGE);
  HXC_TEST_CHECK(hxc_bytes_ref_blit(bytes, 0, bytes, -1, 1) == HXC_STATUS_OUT_OF_RANGE);
  HXC_TEST_CHECK(hxc_bytes_ref_sub(bytes, 0, -1, &failed) == HXC_STATUS_OUT_OF_RANGE && failed == NULL);
  HXC_TEST_CHECK(hxc_bytes_ref_create_zeroed(allocator, -1, &failed) == HXC_STATUS_OUT_OF_RANGE && failed == NULL);

  bytes->references = SIZE_MAX;
  HXC_TEST_CHECK(hxc_bytes_ref_retain(bytes) == HXC_STATUS_SIZE_OVERFLOW);
  bytes->references = 1u;

  arena.failure_armed = true;
  arena.successful_allocations_before_failure = 1u;
  HXC_TEST_CHECK(hxc_bytes_ref_create_zeroed(allocator, 4, &failed) == HXC_STATUS_OUT_OF_MEMORY);
  HXC_TEST_CHECK(failed == NULL);
  arena.failure_armed = false;

  HXC_TEST_CHECK(hxc_bytes_ref_release(text) == HXC_STATUS_OK);
  HXC_TEST_CHECK(hxc_bytes_ref_release(shorter) == HXC_STATUS_OK);
  HXC_TEST_CHECK(hxc_bytes_ref_release(copy) == HXC_STATUS_OK);
  HXC_TEST_CHECK(hxc_bytes_ref_release(bytes) == HXC_STATUS_OK);
  HXC_TEST_CHECK(!arena.invalid_release);
  HXC_TEST_CHECK(arena.allocation_count == arena.release_count);
  return 0;
}
