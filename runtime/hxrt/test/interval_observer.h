/*
 * Independent, single-thread native test observation; never a runtime feature.
 *
 * Technical necessity (haxe_c-rl9u): allocator/collector entry hooks need a
 * directly callable C ABI that cannot allocate or enter generated root code.
 * Generated Haxe public exports/callbacks are not supported today.
 * Durable ownership: this independent ABI observer must survive changes to
 * generated names and root emission. It records facts only; Haxe/test drivers
 * own interval selection, acceptance thresholds, and reports. Keep it out of
 * product and timing artifacts. Revisit if a verified allocation/root-free
 * callback boundary can preserve that independence. Existing generated-root
 * native consumers provide the same independent-test ownership precedent.
 *
 * One test thread owns this fixed storage. Concurrent callers are unsupported;
 * admission must reject threaded observation rather than race these counters.
 */
#ifndef CAXECRAFT_TEST_INTERVAL_OBSERVER_H_INCLUDED
#define CAXECRAFT_TEST_INTERVAL_OBSERVER_H_INCLUDED

#include <stdbool.h>
#include <stdint.h>

typedef enum caxecraft_test_interval_event {
  CAXECRAFT_TEST_INTERVAL_ALLOC_ATTEMPT,
  CAXECRAFT_TEST_INTERVAL_REALLOC_ATTEMPT,
  CAXECRAFT_TEST_INTERVAL_COLLECT_ATTEMPT,
  CAXECRAFT_TEST_INTERVAL_ROOT_PUSH_ATTEMPT,
  CAXECRAFT_TEST_INTERVAL_ROOT_POP_ATTEMPT,
  CAXECRAFT_TEST_INTERVAL_EVENT_COUNT
} caxecraft_test_interval_event;

/* Reset between independent runs only. Reset while active invalidates the run. */
void caxecraft_test_interval_reset(void);
/* No rendering arguments: these are unambiguous native interval markers. */
bool caxecraft_test_interval_begin(void);
bool caxecraft_test_interval_end(void);
/* Record API entries, including failures and nested realloc -> alloc entries.
 * A batch count makes overflow independently testable without special setters.
 * Runtime hooks always record one. Outside an interval, valid events are ignored.
 */
void caxecraft_test_interval_record(caxecraft_test_interval_event event, uint64_t count);
uint64_t caxecraft_test_interval_count(caxecraft_test_interval_event event);
uint64_t caxecraft_test_interval_completed(void);
/* False during an unfinished interval or after any sticky protocol/overflow error. */
bool caxecraft_test_interval_valid(void);

#endif
