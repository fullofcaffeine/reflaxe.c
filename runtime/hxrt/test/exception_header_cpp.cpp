/* C++17 consumer proof for the internal exception header's C ABI surface. */
#include "hxrt/exception.h"

#include <type_traits>

static_assert(std::is_standard_layout<hxc_exception_frame>::value, "exception frame must retain standard layout");
static_assert(std::is_standard_layout<hxc_exception_cleanup>::value, "exception cleanup must retain standard layout");

int main() {
  hxc_exception_frame frame = HXC_EXCEPTION_FRAME_INITIALIZER;
  hxc_exception_cleanup cleanup = HXC_EXCEPTION_CLEANUP_INITIALIZER;
  (void)frame;
  (void)cleanup;
  return 0;
}
