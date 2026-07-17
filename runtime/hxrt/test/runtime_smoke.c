#include "hxc_runtime.h"

#include <stdio.h>
#include <string.h>

#define HXC_CHECK(condition) \
  do { \
    if (!(condition)) { \
      (void)fprintf(stderr, "runtime-smoke: check failed at line %d\n", __LINE__); \
      return 1; \
    } \
  } while (0)

int main(void) {
  hxc_allocator allocator = hxc_default_allocator();
  hxc_allocation allocation = HXC_ALLOCATION_INITIALIZER;
  hxc_owned_string owned = HXC_OWNED_STRING_INITIALIZER;
  const hxc_string view = HXC_STRING_LITERAL("h\xC3\xA9");
  void *memory = NULL;
  void *previous;
  size_t checked_size = 0u;
  int32_t quotient = 0;
  int32_t remainder = 0;

  HXC_CHECK(hxc_runtime_abi_version() == HXC_RUNTIME_ABI_VERSION);
  HXC_CHECK(HXC_RUNTIME_ABI_MAJOR == 0u && HXC_RUNTIME_ABI_MINOR == 3u);
  HXC_CHECK(strcmp(hxc_status_name(HXC_STATUS_OK), "HXC_STATUS_OK") == 0);
  HXC_CHECK(strcmp(hxc_status_name(HXC_STATUS_INVALID_UTF8), "HXC_STATUS_INVALID_UTF8") == 0);
  HXC_CHECK(strcmp(hxc_status_name((hxc_status)42), "HXC_STATUS_UNKNOWN") == 0);
  HXC_CHECK(hxc_allocator_is_valid(&allocator));
  HXC_CHECK(hxc_allocator_same_identity(&allocator, &allocator));
  HXC_CHECK(hxc_size_add(SIZE_MAX, 1u, &checked_size) == HXC_STATUS_SIZE_OVERFLOW);
  HXC_CHECK(hxc_size_mul(SIZE_MAX, 2u, &checked_size) == HXC_STATUS_SIZE_OVERFLOW);

  memory = &checked_size;
  HXC_CHECK(hxc_alloc(&allocator, 0u, 256u, &memory) == HXC_STATUS_OK);
  HXC_CHECK(memory == NULL);

  HXC_CHECK(
    hxc_alloc(&allocator, 16u, 256u, &memory)
      == HXC_STATUS_OK
  );
  HXC_CHECK(memory != NULL);
  HXC_CHECK(((uintptr_t)memory % (uintptr_t)256u) == (uintptr_t)0u);
  ((uint8_t *)memory)[0] = UINT8_C(0xA5);
  previous = memory;
  HXC_CHECK(
    hxc_realloc(&allocator, memory, 16u, SIZE_MAX, 256u, &memory)
      == HXC_STATUS_SIZE_OVERFLOW
  );
  HXC_CHECK(memory == previous);
  HXC_CHECK(((uint8_t *)memory)[0] == UINT8_C(0xA5));
  HXC_CHECK(
    hxc_realloc(
      &allocator,
      memory,
      16u,
      32u,
      256u,
      &memory
    ) == HXC_STATUS_OK
  );
  HXC_CHECK(memory != NULL);
  HXC_CHECK(((uint8_t *)memory)[0] == UINT8_C(0xA5));
  HXC_CHECK(
    hxc_free(&allocator, memory, 32u, 256u)
      == HXC_STATUS_OK
  );

  HXC_CHECK(
    hxc_allocation_allocate(&allocator, 8u, 8u, 128u, &allocation)
      == HXC_STATUS_OK
  );
  HXC_CHECK(hxc_allocation_is_valid(&allocation));
  HXC_CHECK(hxc_allocator_same_identity(&allocator, &allocation.allocator));
  HXC_CHECK(hxc_allocation_dispose(&allocation) == HXC_STATUS_OK);

  HXC_CHECK(view.byte_length == 3u);
  HXC_CHECK(hxc_string_copy(view, &allocator, &owned) == HXC_STATUS_OK);
  HXC_CHECK(owned.value.byte_length == 3u);
  HXC_CHECK(owned.storage.size == 4u);
  HXC_CHECK(owned.value.data[0] == UINT8_C('h'));
  HXC_CHECK(owned.value.data[1] == UINT8_C(0xC3));
  HXC_CHECK(owned.value.data[2] == UINT8_C(0xA9));
  HXC_CHECK(owned.value.data[3] == UINT8_C(0));
  HXC_CHECK(hxc_owned_string_dispose(&owned) == HXC_STATUS_OK);
  HXC_CHECK(owned.value.data == NULL && owned.storage.memory == NULL);

  HXC_CHECK(hxc_i32_add_wrap(INT32_MAX, 1) == INT32_MIN);
  HXC_CHECK(hxc_i32_sub_wrap(INT32_MIN, 1) == INT32_MAX);
  HXC_CHECK(hxc_i32_mul_wrap(INT32_MAX, 2) == -2);
  HXC_CHECK(hxc_i32_neg_wrap(INT32_MIN) == INT32_MIN);
  HXC_CHECK(hxc_i32_shl_wrap(1, 31) == INT32_MIN);
  HXC_CHECK(hxc_i32_shr_arithmetic(-1, 17) == -1);
  HXC_CHECK(hxc_i32_div(INT32_MIN, -1, &quotient) == HXC_STATUS_OK);
  HXC_CHECK(quotient == INT32_MIN);
  HXC_CHECK(hxc_i32_mod(INT32_MIN, -1, &remainder) == HXC_STATUS_OK);
  HXC_CHECK(remainder == 0);
  HXC_CHECK(hxc_i32_div(1, 0, &quotient) == HXC_STATUS_DIVISION_BY_ZERO);

  (void)puts("runtime-smoke: OK");
  return 0;
}
