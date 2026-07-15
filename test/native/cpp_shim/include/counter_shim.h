#ifndef HXC_FIXTURE_COUNTER_SHIM_H_INCLUDED
#define HXC_FIXTURE_COUNTER_SHIM_H_INCLUDED

#include <stdbool.h>
#include <stdint.h>

#if defined(__cplusplus)
extern "C" {
#endif

typedef struct hxc_fixture_counter hxc_fixture_counter;

hxc_fixture_counter *hxc_fixture_counter_create(int32_t initial_value);
bool hxc_fixture_counter_add(hxc_fixture_counter *counter, int32_t amount);
bool hxc_fixture_counter_value(const hxc_fixture_counter *counter, int32_t *out_value);
void hxc_fixture_counter_destroy(hxc_fixture_counter *counter);

#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif /* HXC_FIXTURE_COUNTER_SHIM_H_INCLUDED */
