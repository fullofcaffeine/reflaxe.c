/* Forced include for instrumented correctness builds only, never timing builds.
 * Include before runtime source so its optional hooks bind to this test oracle.
 */
#ifndef CAXECRAFT_TEST_INTERVAL_OBSERVER_HOOKS_H_INCLUDED
#define CAXECRAFT_TEST_INTERVAL_OBSERVER_HOOKS_H_INCLUDED
#include "interval_observer.h"
#define HXC_RUNTIME_ALLOC_ATTEMPT() \
  caxecraft_test_interval_record(CAXECRAFT_TEST_INTERVAL_ALLOC_ATTEMPT, UINT64_C(1))
#define HXC_RUNTIME_REALLOC_ATTEMPT() \
  caxecraft_test_interval_record(CAXECRAFT_TEST_INTERVAL_REALLOC_ATTEMPT, UINT64_C(1))
#define HXC_RUNTIME_COLLECT_ATTEMPT() \
  caxecraft_test_interval_record(CAXECRAFT_TEST_INTERVAL_COLLECT_ATTEMPT, UINT64_C(1))
#define HXC_RUNTIME_ROOT_PUSH_ATTEMPT() \
  caxecraft_test_interval_record(CAXECRAFT_TEST_INTERVAL_ROOT_PUSH_ATTEMPT, UINT64_C(1))
#define HXC_RUNTIME_ROOT_POP_ATTEMPT() \
  caxecraft_test_interval_record(CAXECRAFT_TEST_INTERVAL_ROOT_POP_ATTEMPT, UINT64_C(1))
#endif
