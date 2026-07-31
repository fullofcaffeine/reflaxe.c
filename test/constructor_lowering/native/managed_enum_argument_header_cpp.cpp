#include <type_traits>

extern "C" {
#include "hxc/program.h"
}

static_assert(std::is_class_v<decltype(hxc_Main_buildValues())>,
	"a factory returning a managed enum keeps its complete tagged C representation");

int hxc_managed_enum_argument_header_cpp_probe() {
	return 0;
}
