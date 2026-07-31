#include <type_traits>

extern "C" {
#include "hxc/program.h"
}

static_assert(std::is_pointer_v<decltype(hxc_Main_TreeParser_parseValue(nullptr, 1))>,
	"a nullable recursive class result keeps its pointer-shaped managed ABI");

int hxc_nullable_recursive_factory_header_cpp_probe() {
	return 0;
}
