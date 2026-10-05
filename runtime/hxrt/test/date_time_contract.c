/*
 * Native contract probe for the hosted date-time adapter.
 *
 * The runner starts this program with a fixed POSIX TZ rule. That process
 * boundary makes daylight transitions deterministic without adding timezone
 * mutation to the runtime API itself.
 */
#include "hxrt/date_time.h"

#include <stdint.h>
#include <stdio.h>

static int hxc_test_status_and_range(void) {
  double number = 0.0;
  int32_t offset = 0;

  if (hxc_date_time_wall_milliseconds(NULL) != HXC_STATUS_INVALID_ARGUMENT ||
      hxc_date_time_monotonic_seconds(NULL) != HXC_STATUS_INVALID_ARGUMENT ||
      hxc_date_time_local_to_milliseconds(2024, 0, 1, 0, 0, 0, NULL) != HXC_STATUS_INVALID_ARGUMENT ||
      hxc_date_time_timezone_offset(0.0, NULL) != HXC_STATUS_INVALID_ARGUMENT) {
    return 10;
  }
  if (hxc_date_time_timezone_offset(1.0e300, &offset) != HXC_STATUS_OUT_OF_RANGE ||
      hxc_date_time_timezone_offset(-1.0e300, &offset) != HXC_STATUS_OUT_OF_RANGE ||
      hxc_date_time_timezone_offset(9223372036854775808.0 * 1000.0, &offset) != HXC_STATUS_OUT_OF_RANGE) {
    return 11;
  }
  if (hxc_date_time_wall_milliseconds(&number) != HXC_STATUS_OK || number < 1700000000000.0) {
    return 12;
  }
  return 0;
}

static int hxc_test_clock_kinds(void) {
  double first = 0.0;
  double second = 0.0;

  if (hxc_date_time_monotonic_seconds(&first) != HXC_STATUS_OK ||
      hxc_date_time_monotonic_seconds(&second) != HXC_STATUS_OK || first < 0.0 || second < first) {
    return 20;
  }
  return 0;
}

static int hxc_test_central_time(void) {
  int32_t offset = 0;
  double milliseconds = 0.0;

  if (hxc_date_time_timezone_offset(1710057599000.0, &offset) != HXC_STATUS_OK || offset != 360 ||
      hxc_date_time_timezone_offset(1710057600000.0, &offset) != HXC_STATUS_OK || offset != 300 ||
      hxc_date_time_timezone_offset(1730617199000.0, &offset) != HXC_STATUS_OK || offset != 300 ||
      hxc_date_time_timezone_offset(1730617200000.0, &offset) != HXC_STATUS_OK || offset != 360) {
    return 30;
  }
  if (hxc_date_time_local_to_milliseconds(2024, 2, 10, 2, 30, 0, &milliseconds) != HXC_STATUS_OK ||
      milliseconds != 1710059400000.0) {
    return 31;
  }
  if (hxc_date_time_local_to_milliseconds(2024, 10, 3, 1, 30, 0, &milliseconds) != HXC_STATUS_OK ||
      hxc_date_time_timezone_offset(milliseconds, &offset) != HXC_STATUS_OK ||
      !((milliseconds == 1730615400000.0 && offset == 300) ||
        (milliseconds == 1730619000000.0 && offset == 360))) {
    return 32;
  }
  return 0;
}

int main(void) {
  int result = hxc_test_status_and_range();
  if (result != 0) {
    return result;
  }
  result = hxc_test_clock_kinds();
  if (result != 0) {
    return result;
  }
  result = hxc_test_central_time();
  if (result != 0) {
    return result;
  }
  if (puts("date-time-runtime-ok") == EOF) {
    return 40;
  }
  return 0;
}
