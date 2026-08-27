#include "hxrt/dynamic.h"

#include <cstdint>
#include <cstdio>
#include <type_traits>

using FixtureFunction = std::int32_t (*)(std::int32_t);

struct FixtureFunctionWrapper {
  FixtureFunction function;
};

static std::int32_t add_one(std::int32_t value) {
  return value + 1;
}

static_assert(std::is_standard_layout<hxc_dynamic_type>::value, "Dynamic type identity must be C-compatible");
static_assert(std::is_trivially_copyable<hxc_dynamic_type>::value, "Dynamic type identity must cross the internal ABI by value");
static_assert(std::is_standard_layout<hxc_value>::value, "Dynamic carrier must be C-compatible");
static_assert(std::is_trivially_copyable<hxc_value>::value, "Dynamic carrier must cross the internal ABI by value");
static_assert(
  std::is_same<
    decltype(&hxc_value_init_int32),
    hxc_status (*)(const hxc_dynamic_type *, std::int32_t, hxc_value *)
  >::value,
  "Dynamic Int constructor signature must agree in C++"
);
static_assert(
  std::is_same<
    decltype(&hxc_value_managed_payload),
    hxc_status (*)(const hxc_value *, const void **)
  >::value,
  "Dynamic managed-root projection signature must agree in C++"
);
static_assert(
  std::is_same<decltype(FixtureFunctionWrapper::function), FixtureFunction>::value,
  "generated function wrappers must retain the exact function pointer type"
);

int main() {
  const hxc_dynamic_type int_type{
    HXC_DYNAMIC_TYPE_ABI_VERSION,
    UINT32_C(1),
    HXC_DYNAMIC_CATEGORY_INT,
    HXC_DYNAMIC_STORAGE_INLINE_INT32,
  };
  const hxc_dynamic_type function_type{
    HXC_DYNAMIC_TYPE_ABI_VERSION,
    UINT32_C(2),
    HXC_DYNAMIC_CATEGORY_FUNCTION,
    HXC_DYNAMIC_STORAGE_MANAGED_WRAPPER,
  };
  hxc_value integer = HXC_VALUE_INVALID_INITIALIZER;
  hxc_value function = HXC_VALUE_INVALID_INITIALIZER;
  std::int32_t observed = 0;
  const void *raw_wrapper = nullptr;
  const FixtureFunctionWrapper wrapper{add_one};
  if (hxc_value_init_int32(&int_type, INT32_C(41), &integer) != HXC_STATUS_OK
      || hxc_value_read_int32(&integer, &observed) != HXC_STATUS_OK
      || observed != INT32_C(41)
      || hxc_value_init_managed_wrapper(&function_type, &wrapper, &function) != HXC_STATUS_OK
      || hxc_value_read_managed_wrapper(&function, &raw_wrapper) != HXC_STATUS_OK
      || static_cast<const FixtureFunctionWrapper *>(raw_wrapper)->function(observed) != INT32_C(42)) {
    return 1;
  }
  (void)std::puts("dynamic-header-cpp: OK");
  return 0;
}
