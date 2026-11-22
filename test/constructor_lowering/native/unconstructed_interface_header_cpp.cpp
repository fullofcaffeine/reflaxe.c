#include <type_traits>

extern "C" {
#include "hxc/program.h"
}

static_assert(
	std::is_same_v<decltype(hxc_DormantOwner::hxc_source), hxc_compiler_interface_dispatch_DormantSource_value>,
	"a type-only interface field keeps its complete C++-visible value carrier");

int hxc_unconstructed_interface_header_cpp_probe() {
	return 0;
}
