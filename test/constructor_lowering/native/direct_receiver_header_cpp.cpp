#include <type_traits>

extern "C" {
#include "hxc/program.h"
}

static_assert(std::is_same_v<decltype(hxc_NumberReader::hxc_values), hxc_array_ref *>,
	"the direct receiver keeps its managed Array field visible to C++ consumers");

int hxc_direct_receiver_header_cpp_probe() {
	return 0;
}
