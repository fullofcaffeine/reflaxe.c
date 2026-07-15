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
  hxc_owned_string owned = {0};
  hxc_string_view view;
  void *memory = NULL;
  int32_t quotient = 0;
  int32_t remainder = 0;

  HXC_CHECK(hxc_runtime_abi_version() == HXC_RUNTIME_ABI_VERSION);
  HXC_CHECK(strcmp(hxc_status_name(HXC_STATUS_OK), "HXC_STATUS_OK") == 0);
  HXC_CHECK(strcmp(hxc_status_name((hxc_status)42), "HXC_STATUS_UNKNOWN") == 0);
  HXC_CHECK(hxc_allocator_is_valid(&allocator));

  HXC_CHECK(
    hxc_alloc(&allocator, 16u, HXC_ALIGNOF(max_align_t), &memory)
      == HXC_STATUS_OK
  );
  HXC_CHECK(memory != NULL);
  ((uint8_t *)memory)[0] = UINT8_C(0xA5);
  HXC_CHECK(
    hxc_realloc(
      &allocator,
      memory,
      16u,
      32u,
      HXC_ALIGNOF(max_align_t),
      &memory
    ) == HXC_STATUS_OK
  );
  HXC_CHECK(memory != NULL);
  HXC_CHECK(((uint8_t *)memory)[0] == UINT8_C(0xA5));
  HXC_CHECK(
    hxc_free(&allocator, memory, 32u, HXC_ALIGNOF(max_align_t))
      == HXC_STATUS_OK
  );

  view = hxc_string_view_from_cstr("h\xC3\xA9");
  HXC_CHECK(view.length == 3u);
  HXC_CHECK(hxc_string_copy(view, &allocator, &owned) == HXC_STATUS_OK);
  HXC_CHECK(owned.length == 3u);
  HXC_CHECK(owned.capacity == 4u);
  HXC_CHECK(owned.data[0] == UINT8_C('h'));
  HXC_CHECK(owned.data[1] == UINT8_C(0xC3));
  HXC_CHECK(owned.data[2] == UINT8_C(0xA9));
  HXC_CHECK(owned.data[3] == UINT8_C(0));
  hxc_owned_string_dispose(&owned);
  HXC_CHECK(owned.data == NULL && owned.length == 0u && owned.capacity == 0u);

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
