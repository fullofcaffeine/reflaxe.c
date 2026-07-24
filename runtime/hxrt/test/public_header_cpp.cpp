/*
 * Fixture-only C++17 consumer of the provisional C runtime umbrella.
 *
 * This file is deliberately .cpp: a C++ compiler must prove that the headers'
 * extern "C" linkage, callback signatures, standard-layout records, alignment,
 * and C/C++ layout facts agree. Reflaxe.C still emits C, not C++; this fixture is
 * never generated or packaged and does not establish a stable public C++ API.
 */
#include "hxc_runtime.h"

#include <array>
#include <cstddef>
#include <cstdio>
#include <type_traits>

extern "C" void hxc_test_c_allocator_layout(std::size_t *values);

static_assert(HXC_RUNTIME_ABI_MAJOR == 0u, "C++ consumer must see the reviewed runtime ABI major");
static_assert(HXC_RUNTIME_ABI_MINOR == 10u, "C++ consumer must see the reviewed runtime ABI minor");
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
static_assert(std::is_standard_layout<hxc_array_element_ops>::value, "array element strategy must be C-compatible");
static_assert(std::is_trivially_copyable<hxc_array_element_ops>::value, "array element strategy must cross C ABI calls by value");
static_assert(std::is_standard_layout<hxc_array>::value, "array owner must be C-compatible");
static_assert(std::is_trivially_copyable<hxc_array>::value, "array owner must cross C ABI calls by value");
static_assert(std::is_standard_layout<hxc_array_ref>::value, "shared Array container must be C-compatible");
static_assert(std::is_trivially_copyable<hxc_array_ref>::value, "shared Array container must have a C-compatible object representation");
static_assert(std::is_standard_layout<hxc_bytes_ref>::value, "shared Bytes container must be C-compatible");
static_assert(std::is_trivially_copyable<hxc_bytes_ref>::value, "shared Bytes container must have a C-compatible object representation");
static_assert(
  std::is_same<
    hxc_array_copy_fn,
    hxc_status (*)(void *, void *, const void *)
  >::value,
  "array copy callback signature must agree in C++"
);
static_assert(
  std::is_same<
    hxc_array_assign_fn,
    hxc_status (*)(void *, void *, const void *)
  >::value,
  "array assignment callback signature must agree in C++"
);
static_assert(
  std::is_same<hxc_array_destroy_fn, void (*)(void *, void *)>::value,
  "array destructor callback signature must agree in C++"
);
static_assert(
  std::is_same<decltype(&hxc_array_ref_retain), hxc_status (*)(hxc_array_ref *)>::value,
  "shared Array retain signature must agree in C++"
);
static_assert(
	std::is_same<decltype(&hxc_array_ref_release), hxc_status (*)(hxc_array_ref *)>::value,
	"shared Array release signature must agree in C++"
);
static_assert(
  std::is_same<
    decltype(&hxc_array_ref_init_in_place),
    hxc_status (*)(hxc_allocator, hxc_array_element_ops, hxc_array_ref *)
  >::value,
  "in-place shared Array initialization signature must agree in C++"
);
static_assert(
  std::is_same<
    decltype(&hxc_array_ref_dispose_in_place),
    hxc_status (*)(hxc_array_ref *)
  >::value,
  "in-place shared Array disposal signature must agree in C++"
);
static_assert(
  std::is_same<decltype(&hxc_bytes_ref_retain), hxc_status (*)(hxc_bytes_ref *)>::value,
  "shared Bytes retain signature must agree in C++"
);
static_assert(
  std::is_same<decltype(&hxc_bytes_ref_release), hxc_status (*)(hxc_bytes_ref *)>::value,
  "shared Bytes release signature must agree in C++"
);
static_assert(std::is_standard_layout<hxc_string>::value, "private string value must be C-compatible");
static_assert(std::is_trivially_copyable<hxc_string>::value, "private string value must cross internal C ABI calls by value");
static_assert(std::is_standard_layout<hxc_owned_string>::value, "owned string must be C-compatible");
static_assert(std::is_trivially_copyable<hxc_owned_string>::value, "owned string must cross internal C ABI calls by value");
static_assert(std::is_standard_layout<hxc_borrowed_cstring>::value, "borrowed CString must be C-compatible");
static_assert(std::is_standard_layout<hxc_owned_cstring>::value, "owned CString must be C-compatible");
static_assert(
  std::is_same<decltype(&hxc_io_println), hxc_status (*)(hxc_string)>::value,
  "hosted output signature must agree in C++"
);

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
  const hxc_string missing = HXC_STRING_INITIALIZER;
  const hxc_string empty_string = HXC_STRING_EMPTY_INITIALIZER;
  if (hxc_string_is_valid(missing)
      || !hxc_string_is_valid(empty_string)
      || empty_string.data == nullptr) {
    return 5;
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
