#include <type_traits>

extern "C" {
#include "hxc/program.h"
}

static_assert(std::is_same_v<decltype(hxc_CounterOwner::hxc_child), hxc_OwnedCounter>,
	"an owned Haxe child remains an inline C++-visible subobject");

int hxc_constructor_header_cpp_probe() {
	return 0;
}
