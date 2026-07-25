/*
 * Independent C contract test for the runtime half of Array<String>.join.
 *
 * The generated-Haxe fixture proves compiler integration. This file calls the
 * public hxrt API directly so allocator failure and malformed element storage
 * can be tested without asking haxe.c to generate its own reference oracle.
 */
#include "hxrt/array_join.h"

#include <stdio.h>
#include <string.h>

#define HXC_JOIN_CHECK(condition) \
  do { \
    if (!(condition)) { \
      (void)fprintf(stderr, "array-join-runtime: check failed at line %d\n", __LINE__); \
      return 1; \
    } \
  } while (0)

static hxc_status hxc_join_fail_allocate(
  void *context,
  size_t size,
  size_t alignment,
  void **out_memory
) {
  (void)context;
  (void)size;
  (void)alignment;
  (void)out_memory;
  return HXC_STATUS_OUT_OF_MEMORY;
}

static hxc_status hxc_join_fail_reallocate(
  void *context,
  void *memory,
  size_t old_size,
  size_t new_size,
  size_t alignment,
  void **out_memory
) {
  (void)context;
  (void)memory;
  (void)old_size;
  (void)new_size;
  (void)alignment;
  (void)out_memory;
  return HXC_STATUS_OUT_OF_MEMORY;
}

static void hxc_join_fail_release(
  void *context,
  void *memory,
  size_t size,
  size_t alignment
) {
  (void)context;
  (void)memory;
  (void)size;
  (void)alignment;
}

int main(void) {
  static const uint8_t first_bytes[] = {'c', 'a', 'f', UINT8_C(0xc3), UINT8_C(0xa9)};
  static const uint8_t second_bytes[] = {'a', UINT8_C(0), 'b'};
  static const uint8_t separator_bytes[] = {'|'};
  static const uint8_t expected[] = {
    'c', 'a', 'f', UINT8_C(0xc3), UINT8_C(0xa9), '|', 'a', UINT8_C(0), 'b'
  };
  hxc_allocator allocator = hxc_default_allocator();
  hxc_allocator failing = {
    NULL,
    hxc_join_fail_allocate,
    hxc_join_fail_reallocate,
    hxc_join_fail_release
  };
  hxc_array_ref *array = NULL;
  hxc_string first = {first_bytes, sizeof(first_bytes), false, NULL};
  hxc_string second = {second_bytes, sizeof(second_bytes), false, NULL};
  hxc_string separator = {separator_bytes, sizeof(separator_bytes), false, NULL};
  hxc_string joined = HXC_STRING_INITIALIZER;
  hxc_string failed = HXC_STRING_INITIALIZER;
  int32_t length = 0;

  HXC_JOIN_CHECK(
    hxc_array_ref_create_trivial(
      allocator,
      sizeof(hxc_string),
      HXC_ALIGNOF(hxc_string),
      &array
    ) == HXC_STATUS_OK
  );
  HXC_JOIN_CHECK(hxc_array_ref_push_copy(array, &first, &length) == HXC_STATUS_OK);
  HXC_JOIN_CHECK(length == 1);
  HXC_JOIN_CHECK(hxc_array_ref_push_copy(array, &second, &length) == HXC_STATUS_OK);
  HXC_JOIN_CHECK(length == 2);
  HXC_JOIN_CHECK(
    hxc_array_string_join(array, separator, allocator, &joined) == HXC_STATUS_OK
  );
  HXC_JOIN_CHECK(joined.byte_length == sizeof(expected));
  HXC_JOIN_CHECK(memcmp(joined.data, expected, sizeof(expected)) == 0);
  HXC_JOIN_CHECK(hxc_string_release(&joined) == HXC_STATUS_OK);

  HXC_JOIN_CHECK(
    hxc_array_string_join(array, separator, failing, &failed)
      == HXC_STATUS_OUT_OF_MEMORY
  );
  HXC_JOIN_CHECK(
    failed.data == NULL
      && failed.byte_length == 0u
      && !failed.has_trailing_nul
      && failed.owner == NULL
  );
  HXC_JOIN_CHECK(hxc_array_ref_release(array) == HXC_STATUS_OK);
  return 0;
}
