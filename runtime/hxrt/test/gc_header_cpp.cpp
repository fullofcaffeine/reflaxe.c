/* C++17 consumer proof for the internal collector header. */
#include "hxrt/gc.h"
#include "hxrt/gc.h"

#include <type_traits>

static_assert(std::is_standard_layout<hxc_gc_config>::value, "GC config must be C-compatible");
static_assert(std::is_standard_layout<hxc_gc_stats>::value, "GC stats must be C-compatible");
static_assert(std::is_standard_layout<hxc_gc_root_frame>::value, "root frames must be C-compatible");

int main() {
  hxc_gc gc = HXC_GC_INITIALIZER;
  hxc_gc_thread thread = HXC_GC_THREAD_INITIALIZER;
  hxc_gc_root_frame frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  hxc_gc_root_table roots = HXC_GC_ROOT_TABLE_INITIALIZER;
  hxc_gc_pin pin = HXC_GC_PIN_INITIALIZER;
  return gc.initialized || thread.registered || frame.active || roots.registered || pin.active ? 1 : 0;
}
