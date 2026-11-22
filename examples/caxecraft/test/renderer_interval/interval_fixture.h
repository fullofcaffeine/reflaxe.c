/* Test-only native facts; the observer ABI remains owned by its own header. */
#ifndef INTERVAL_FIXTURE_H_INCLUDED
#define INTERVAL_FIXTURE_H_INCLUDED
#include <stdbool.h>
#include <stdint.h>
#define INTERVAL_FIXTURE_ZERO UINT64_C(0)
#define INTERVAL_FIXTURE_ONE UINT64_C(1)
#define INTERVAL_FIXTURE_WIDE UINT64_C(4294967297)
void interval_fixture_setup(void);
bool interval_fixture_allocate(void);
bool interval_fixture_roots(void);
void interval_fixture_wide_count(void);
void interval_fixture_require(bool condition);
void interval_fixture_finish(void);
#endif
