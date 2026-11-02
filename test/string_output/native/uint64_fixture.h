#ifndef HXC_TEST_UINT64_FIXTURE_H
#define HXC_TEST_UINT64_FIXTURE_H

#include <stdint.h>

/* Native values are the oracle input; Haxe owns all text conversion and output. */
uint64_t fixture_uint64_value(int32_t index);
int32_t fixture_uint64_calls(void);

#endif
