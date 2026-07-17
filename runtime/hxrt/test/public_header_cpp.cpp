#include "hxc_runtime.h"

#include <array>
#include <cstddef>
#include <cstdio>
#include <type_traits>

extern "C" void hxc_test_c_allocator_layout(std::size_t *values);

static_assert(
  std::is_same<
    hxc_allocate_fn,
    hxc_status (*)(void *, std::size_t, std::size_t, void **)
  >::value,
  "allocator callback signature must agree in C++"
);
static_assert(
  std::is_same<
    hxc_reallocate_fn,
    hxc_status (*)(
      void *,
      void *,
      std::size_t,
      std::size_t,
      std::size_t,
      void **
    )
  >::value,
  "reallocator callback signature must agree in C++"
);
static_assert(std::is_standard_layout<hxc_allocator>::value, "allocator must be C-compatible");
static_assert(std::is_trivially_copyable<hxc_allocator>::value, "allocator handle must be copyable across C ABI calls");
static_assert(std::is_standard_layout<hxc_allocation>::value, "allocation owner must be C-compatible");
static_assert(std::is_trivially_copyable<hxc_allocation>::value, "allocation owner must cross C ABI calls by value");
static_assert(std::is_standard_layout<hxc_string>::value, "private string value must be C-compatible");
static_assert(std::is_trivially_copyable<hxc_string>::value, "private string value must cross internal C ABI calls by value");
static_assert(std::is_standard_layout<hxc_owned_string>::value, "owned string must be C-compatible");
static_assert(std::is_trivially_copyable<hxc_owned_string>::value, "owned string must cross internal C ABI calls by value");
static_assert(std::is_standard_layout<hxc_borrowed_cstring>::value, "borrowed CString must be C-compatible");
static_assert(std::is_standard_layout<hxc_owned_cstring>::value, "owned CString must be C-compatible");

int main() {
  const hxc_string literal = HXC_STRING_LITERAL("C++ UTF-8 \xF0\x9F\x98\x80");
  const auto version = &hxc_runtime_abi_version;
  hxc_allocation empty = HXC_ALLOCATION_INITIALIZER;
  std::array<std::size_t, 12> c_layout{};
  const std::array<std::size_t, 12> cpp_layout{
    sizeof(hxc_allocator),
    alignof(hxc_allocator),
    offsetof(hxc_allocator, context),
    offsetof(hxc_allocator, allocate),
    offsetof(hxc_allocator, reallocate),
    offsetof(hxc_allocator, release),
    sizeof(hxc_allocation),
    alignof(hxc_allocation),
    offsetof(hxc_allocation, memory),
    offsetof(hxc_allocation, size),
    offsetof(hxc_allocation, alignment),
    offsetof(hxc_allocation, allocator),
  };
  if (version() != HXC_RUNTIME_ABI_VERSION) {
    return 1;
  }
  if (!hxc_string_is_valid(literal) || literal.byte_length != 14u) {
    return 4;
  }
  hxc_test_c_allocator_layout(c_layout.data());
  if (c_layout != cpp_layout) {
    return 2;
  }
  if (hxc_allocation_dispose(&empty) != HXC_STATUS_OK) {
    return 3;
  }
  (void)std::puts("public-header-cpp: OK");
  return 0;
}
