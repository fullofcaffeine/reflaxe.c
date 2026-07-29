#include <type_traits>

extern "C" {
#include "hxc/program.h"
}

static_assert(std::is_pointer_v<decltype(hxc_Main_create(1))>,
	"a Haxe factory returning a class exposes one pointer-shaped managed result");
static_assert(std::is_pointer_v<decltype(hxc_Main_forward(nullptr))>,
	"passing a class reference through a Haxe helper preserves its pointer-shaped ABI");

int hxc_factory_return_header_cpp_probe() {
	return 0;
}
