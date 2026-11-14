/* Fixed-storage native test oracle. See interval_observer.h for scope and
 * ownership. No allocator, runtime, logging, callback, or generated code enters
 * this implementation. Invalid evidence remains invalid until an inactive reset.
 */
#include "interval_observer.h"

static bool active;
static bool valid = true;
static uint64_t attempts[CAXECRAFT_TEST_INTERVAL_EVENT_COUNT];
static uint64_t completed;

static void add_count(uint64_t *value, uint64_t count) {
  if (count > UINT64_MAX - *value) {
    valid = false;
    return;
  }
  *value += count;
}

void caxecraft_test_interval_reset(void) {
  int event;
  if (active) {
    valid = false;
    return;
  }
  valid = true;
  completed = UINT64_C(0);
  for (event = 0; event < CAXECRAFT_TEST_INTERVAL_EVENT_COUNT; event++)
    attempts[event] = UINT64_C(0);
}

bool caxecraft_test_interval_begin(void) {
  if (active || !valid) {
    valid = false;
    return false;
  }
  active = true;
  return true;
}

bool caxecraft_test_interval_end(void) {
  if (!active) {
    valid = false;
    return false;
  }
  active = false;
  if (valid) add_count(&completed, UINT64_C(1));
  return valid;
}

void caxecraft_test_interval_record(caxecraft_test_interval_event event, uint64_t count) {
  if ((unsigned int)event >= (unsigned int)CAXECRAFT_TEST_INTERVAL_EVENT_COUNT) {
    valid = false;
    return;
  }
  if (active && valid) add_count(&attempts[event], count);
}

uint64_t caxecraft_test_interval_count(caxecraft_test_interval_event event) {
  if ((unsigned int)event >= (unsigned int)CAXECRAFT_TEST_INTERVAL_EVENT_COUNT) {
    valid = false;
    return UINT64_C(0);
  }
  return attempts[event];
}

uint64_t caxecraft_test_interval_completed(void) {
  return completed;
}

bool caxecraft_test_interval_valid(void) {
  return valid && !active;
}
