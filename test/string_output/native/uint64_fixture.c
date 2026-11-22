#include "uint64_fixture.h"

static int32_t calls = INT32_C(0);

uint64_t fixture_uint64_value(int32_t index) {
  calls += INT32_C(1);
  switch (index) {
    case 0: return UINT64_C(0);
    case 1: return UINT64_C(4294967297);
    default: return UINT64_MAX;
  }
}

int32_t fixture_uint64_calls(void) {
  return calls;
}
