#include "hxc/program.h"

#include <cstddef>
#include <type_traits>

static_assert(std::is_standard_layout<hxc_optional_Main_ManagedRecord>::value,
              "a managed tagged optional remains a standard-layout C value");
static_assert(offsetof(hxc_optional_Main_ManagedRecord, hxc_has_value) == 0,
              "the presence flag remains the first field");
static_assert(sizeof(hxc_optional_Main_ManagedRecord) >=
                  offsetof(hxc_optional_Main_ManagedRecord, hxc_value) +
                      sizeof(hxc_Main_ManagedRecord),
              "the present payload fits completely inside the optional");

static_assert(std::is_standard_layout<hxc_optional_Main_ManagedChoice>::value,
              "a nullable managed enum remains a standard-layout C value");
static_assert(offsetof(hxc_optional_Main_ManagedChoice, hxc_has_value) == 0,
              "the enum optional keeps its presence flag first");
static_assert(sizeof(hxc_optional_Main_ManagedChoice) >=
                  offsetof(hxc_optional_Main_ManagedChoice, hxc_value) +
                      sizeof(hxc_Main_ManagedChoice),
              "the complete tagged enum fits inside the optional");

extern "C" int hxc_managed_optional_header_probe(void) {
  return 0;
}
