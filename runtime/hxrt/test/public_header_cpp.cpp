#include "hxc_runtime.h"

#include <cstdio>
#include <type_traits>

static_assert(std::is_standard_layout<hxc_allocator>::value, "allocator must be C-compatible");
static_assert(std::is_standard_layout<hxc_string_view>::value, "string view must be C-compatible");

int main() {
  const auto version = &hxc_runtime_abi_version;
  if (version() != HXC_RUNTIME_ABI_VERSION) {
    return 1;
  }
  (void)std::puts("public-header-cpp: OK");
  return 0;
}
