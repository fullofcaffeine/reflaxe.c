/* Native contract for nested frames, reverse cleanup, rethrow, and roots. */
#include "hxrt/exception.h"

#include <stdint.h>

typedef struct hxc_exception_test_log {
  char events[8];
  size_t length;
} hxc_exception_test_log;

typedef struct hxc_exception_test_cleanup {
  volatile hxc_exception_test_log *log;
  char event;
} hxc_exception_test_cleanup;

typedef struct hxc_exception_test_root_slot {
  const void *volatile value;
} hxc_exception_test_root_slot;

static hxc_status hxc_exception_test_recursive_raise(void *context) {
  return hxc_exception_raise((const hxc_value *)context);
}

static hxc_status hxc_exception_test_record(void *context) {
  hxc_exception_test_cleanup *cleanup = (hxc_exception_test_cleanup *)context;
  if (cleanup == NULL || cleanup->log == NULL || cleanup->log->length >= 8u) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  cleanup->log->events[cleanup->log->length++] = cleanup->event;
  return HXC_STATUS_OK;
}

static hxc_status hxc_exception_test_root(void *context, const void *payload) {
  hxc_exception_test_root_slot *slot = (hxc_exception_test_root_slot *)context;
  if (slot == NULL) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  slot->value = payload;
  return HXC_STATUS_OK;
}

static int hxc_exception_test_nested(void) {
  static const hxc_dynamic_type int_type = {
    HXC_DYNAMIC_TYPE_ABI_VERSION,
    UINT32_C(1),
    HXC_DYNAMIC_CATEGORY_INT,
    HXC_DYNAMIC_STORAGE_INLINE_INT32
  };
  hxc_exception_frame outer = HXC_EXCEPTION_FRAME_INITIALIZER;
  hxc_exception_frame inner = HXC_EXCEPTION_FRAME_INITIALIZER;
  hxc_exception_cleanup outer_cleanup = HXC_EXCEPTION_CLEANUP_INITIALIZER;
  hxc_exception_cleanup inner_first = HXC_EXCEPTION_CLEANUP_INITIALIZER;
  hxc_exception_cleanup inner_second = HXC_EXCEPTION_CLEANUP_INITIALIZER;
  volatile hxc_exception_test_log log = {{ 0 }, 0u};
  hxc_exception_test_cleanup outer_record = {&log, 'O'};
  hxc_exception_test_cleanup first_record = {&log, 'A'};
  hxc_exception_test_cleanup second_record = {&log, 'B'};
  hxc_value payload = HXC_VALUE_INVALID_INITIALIZER;
  volatile int resumed = 0;

  if (hxc_exception_frame_push(&outer, NULL, NULL) != HXC_STATUS_OK) {
    return 1;
  }
  if (HXC_EXCEPTION_SETJMP(&outer) == 0) {
    if (hxc_exception_cleanup_push(&outer_cleanup, hxc_exception_test_record, &outer_record) != HXC_STATUS_OK
        || hxc_exception_frame_push(&inner, NULL, NULL) != HXC_STATUS_OK) {
      return 2;
    }
    if (HXC_EXCEPTION_SETJMP(&inner) == 0) {
      if (hxc_exception_cleanup_push(&inner_first, hxc_exception_test_record, &first_record) != HXC_STATUS_OK
          || hxc_exception_cleanup_push(&inner_second, hxc_exception_test_record, &second_record) != HXC_STATUS_OK
          || hxc_value_init_int32(&int_type, INT32_C(7), &payload) != HXC_STATUS_OK
          || hxc_exception_raise(&payload) != HXC_STATUS_OK) {
        return 3;
      }
      return 4;
    }
    resumed = 1;
    if (log.length != 2u || log.events[0] != 'B' || log.events[1] != 'A'
        || hxc_exception_frame_payload(&inner, &payload) != HXC_STATUS_OK
        || hxc_exception_frame_pop(&inner) != HXC_STATUS_OK
        || hxc_value_init_int32(&int_type, INT32_C(8), &payload) != HXC_STATUS_OK
        || hxc_exception_raise(&payload) != HXC_STATUS_OK) {
      return 5;
    }
    return 6;
  }
  {
    int32_t caught = 0;
    if (resumed != 1
        || log.length != 3u
        || log.events[2] != 'O'
        || hxc_exception_frame_payload(&outer, &payload) != HXC_STATUS_OK
        || hxc_value_read_int32(&payload, &caught) != HXC_STATUS_OK
        || caught != 8
        || hxc_exception_frame_pop(&outer) != HXC_STATUS_OK) {
      return 7;
    }
  }
  return 0;
}

static int hxc_exception_test_managed_root(void) {
  static const hxc_dynamic_type object_type = {
    HXC_DYNAMIC_TYPE_ABI_VERSION,
    UINT32_C(2),
    HXC_DYNAMIC_CATEGORY_OBJECT,
    HXC_DYNAMIC_STORAGE_MANAGED_REFERENCE
  };
  hxc_exception_frame frame = HXC_EXCEPTION_FRAME_INITIALIZER;
  hxc_value payload = HXC_VALUE_INVALID_INITIALIZER;
  int object = 19;
  hxc_exception_test_root_slot root = {NULL};
  if (hxc_exception_frame_push(&frame, hxc_exception_test_root, &root) != HXC_STATUS_OK) {
    return 1;
  }
  if (HXC_EXCEPTION_SETJMP(&frame) == 0) {
    if (hxc_value_init_managed_reference(&object_type, &object, &payload) != HXC_STATUS_OK
        || hxc_exception_raise(&payload) != HXC_STATUS_OK) {
      return 2;
    }
    return 3;
  }
  if (root.value != &object
      || hxc_exception_frame_take_payload(&frame, &payload) != HXC_STATUS_OK
      || hxc_exception_frame_pop(&frame) != HXC_STATUS_OK
      || root.value != &object
      || hxc_exception_test_root(&root, NULL) != HXC_STATUS_OK
      || root.value != NULL) {
    return 4;
  }
  return 0;
}

static int hxc_exception_test_failed_cleanup(void) {
  static const hxc_dynamic_type int_type = {
    HXC_DYNAMIC_TYPE_ABI_VERSION,
    UINT32_C(3),
    HXC_DYNAMIC_CATEGORY_INT,
    HXC_DYNAMIC_STORAGE_INLINE_INT32
  };
  hxc_exception_frame frame = HXC_EXCEPTION_FRAME_INITIALIZER;
  hxc_exception_cleanup cleanup = HXC_EXCEPTION_CLEANUP_INITIALIZER;
  hxc_value payload = HXC_VALUE_INVALID_INITIALIZER;

  if (hxc_exception_frame_push(&frame, NULL, NULL) != HXC_STATUS_OK
      || hxc_value_init_int32(&int_type, INT32_C(9), &payload) != HXC_STATUS_OK
      || hxc_exception_cleanup_push(&cleanup, hxc_exception_test_recursive_raise, &payload) != HXC_STATUS_OK) {
    return 1;
  }
  if (HXC_EXCEPTION_SETJMP(&frame) != 0
      || hxc_exception_raise(&payload) != HXC_STATUS_INVALID_ARGUMENT
      || frame.has_payload
      || hxc_exception_frame_pop(&frame) != HXC_STATUS_OK) {
    return 2;
  }
  return 0;
}

static int hxc_exception_test_partial_initialization(void) {
  static const hxc_dynamic_type int_type = {
    HXC_DYNAMIC_TYPE_ABI_VERSION,
    UINT32_C(4),
    HXC_DYNAMIC_CATEGORY_INT,
    HXC_DYNAMIC_STORAGE_INLINE_INT32
  };
  hxc_exception_frame frame = HXC_EXCEPTION_FRAME_INITIALIZER;
  hxc_exception_cleanup initialized = HXC_EXCEPTION_CLEANUP_INITIALIZER;
  hxc_exception_cleanup not_initialized = HXC_EXCEPTION_CLEANUP_INITIALIZER;
  volatile hxc_exception_test_log log = {{ 0 }, 0u};
  hxc_exception_test_cleanup initialized_record = {&log, 'I'};
  hxc_exception_test_cleanup not_initialized_record = {&log, 'N'};
  hxc_value payload = HXC_VALUE_INVALID_INITIALIZER;

  if (hxc_exception_frame_push(&frame, NULL, NULL) != HXC_STATUS_OK) {
    return 1;
  }
  if (HXC_EXCEPTION_SETJMP(&frame) == 0) {
    if (hxc_exception_cleanup_push(&initialized, hxc_exception_test_record, &initialized_record) != HXC_STATUS_OK
        || hxc_value_init_int32(&int_type, INT32_C(10), &payload) != HXC_STATUS_OK
        || hxc_exception_raise(&payload) != HXC_STATUS_OK) {
      return 2;
    }
    if (hxc_exception_cleanup_push(&not_initialized, hxc_exception_test_record, &not_initialized_record) == HXC_STATUS_OK) {
      return 3;
    }
  }
  if (log.length != 1u
      || log.events[0] != 'I'
      || not_initialized.active
      || hxc_exception_frame_pop(&frame) != HXC_STATUS_OK) {
    return 4;
  }
  return 0;
}

static int hxc_exception_test_discarded_transfer(void) {
  hxc_exception_frame frame = HXC_EXCEPTION_FRAME_INITIALIZER;
  hxc_exception_cleanup cleanup = HXC_EXCEPTION_CLEANUP_INITIALIZER;
  volatile hxc_exception_test_log log = {{ 0 }, 0u};
  hxc_exception_test_cleanup record = {&log, 'D'};

  if (hxc_exception_frame_push(&frame, NULL, NULL) != HXC_STATUS_OK
      || hxc_exception_cleanup_push(&cleanup, hxc_exception_test_record, &record) != HXC_STATUS_OK
      || hxc_exception_cleanup_discard(&cleanup) != HXC_STATUS_OK
      || cleanup.active
      || log.length != 0u
      || hxc_exception_frame_pop(&frame) != HXC_STATUS_OK) {
    return 1;
  }
  return 0;
}

int main(void) {
  hxc_value invalid = HXC_VALUE_INVALID_INITIALIZER;
  if (hxc_exception_raise(&invalid) != HXC_STATUS_INVALID_ARGUMENT) {
    return 1;
  }
  if (hxc_exception_test_nested() != 0) {
    return 2;
  }
  if (hxc_exception_test_managed_root() != 0) {
    return 3;
  }
  if (hxc_exception_test_failed_cleanup() != 0) {
    return 4;
  }
  if (hxc_exception_test_partial_initialization() != 0) {
    return 5;
  }
  if (hxc_exception_test_discarded_transfer() != 0) {
    return 6;
  }
  return 0;
}
