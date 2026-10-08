/*
 * Implementation of feature `exception`.
 *
 * The generated owner keeps setjmp in its own function. This file contains
 * only thread-local frame/cleanup state, payload rooting, validation, reverse
 * cleanup, and the final same-thread longjmp.
 */
#include "hxrt/exception.h"

static _Thread_local hxc_exception_frame *hxc_exception_top_frame = NULL;
static _Thread_local hxc_exception_cleanup *hxc_exception_top_cleanup = NULL;
static _Thread_local bool hxc_exception_is_unwinding = false;

hxc_status hxc_exception_root_slot_update(
  void *context,
  const void *managed_payload
) {
  const void *volatile *slot = (const void *volatile *)context;
  if (slot == NULL) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  *slot = managed_payload;
  return HXC_STATUS_OK;
}

static hxc_value hxc_exception_payload_snapshot(
  const volatile hxc_value *payload
) {
  hxc_value result = HXC_VALUE_INVALID_INITIALIZER;
  result.type = payload->type;
  result.active_storage = payload->active_storage;
  switch (result.active_storage) {
    case HXC_DYNAMIC_STORAGE_INLINE_NULL:
    case HXC_DYNAMIC_STORAGE_INLINE_BOOL:
      result.payload.boolean = payload->payload.boolean;
      break;
    case HXC_DYNAMIC_STORAGE_INLINE_INT32:
      result.payload.int32_value = payload->payload.int32_value;
      break;
    case HXC_DYNAMIC_STORAGE_INLINE_FLOAT64:
      result.payload.float64_value = payload->payload.float64_value;
      break;
    case HXC_DYNAMIC_STORAGE_MANAGED_REFERENCE:
    case HXC_DYNAMIC_STORAGE_MANAGED_WRAPPER:
      result.payload.object = payload->payload.object;
      break;
    case HXC_DYNAMIC_STORAGE_STATIC_TOKEN:
      result.payload.static_token = payload->payload.static_token;
      break;
  }
  return result;
}

static hxc_status hxc_exception_cancel_raise(
  hxc_exception_frame *target,
  hxc_status failure
) {
  hxc_status clear_status = HXC_STATUS_OK;
  hxc_exception_is_unwinding = false;
  if (target->root_update != NULL) {
    clear_status = target->root_update(target->root_context, NULL);
    if (clear_status != HXC_STATUS_OK) {
      return clear_status;
    }
  }
  target->payload = (hxc_value)HXC_VALUE_INVALID_INITIALIZER;
  target->has_payload = false;
  return failure;
}

hxc_status hxc_exception_frame_push(
  hxc_exception_frame *frame,
  hxc_exception_root_update_fn root_update,
  void *root_context
) {
  if (frame == NULL || frame->active || frame->has_payload || hxc_exception_is_unwinding) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  frame->previous = hxc_exception_top_frame;
  frame->cleanup_checkpoint = hxc_exception_top_cleanup;
  frame->payload = (hxc_value)HXC_VALUE_INVALID_INITIALIZER;
  frame->root_update = root_update;
  frame->root_context = root_context;
  frame->active = true;
  frame->has_payload = false;
  hxc_exception_top_frame = frame;
  return HXC_STATUS_OK;
}

hxc_status hxc_exception_frame_payload(
  const hxc_exception_frame *frame,
  hxc_value *out_payload
) {
  hxc_value payload;
  if (frame == NULL
      || out_payload == NULL
      || !frame->active
      || !frame->has_payload
      || hxc_exception_top_frame != frame) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  payload = hxc_exception_payload_snapshot(&frame->payload);
  if (!hxc_value_is_valid(&payload)) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  *out_payload = payload;
  return HXC_STATUS_OK;
}

hxc_status hxc_exception_frame_take_payload(
  hxc_exception_frame *frame,
  hxc_value *out_payload
) {
  hxc_status status = hxc_exception_frame_payload(frame, out_payload);
  if (status != HXC_STATUS_OK) {
    return status;
  }
  frame->root_update = NULL;
  frame->root_context = NULL;
  return HXC_STATUS_OK;
}

hxc_status hxc_exception_frame_pop(hxc_exception_frame *frame) {
  hxc_status status = HXC_STATUS_OK;
  if (frame == NULL
      || !frame->active
      || hxc_exception_is_unwinding
      || hxc_exception_top_frame != frame
      || hxc_exception_top_cleanup != frame->cleanup_checkpoint) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  if (frame->has_payload && frame->root_update != NULL) {
    status = frame->root_update(frame->root_context, NULL);
    if (status != HXC_STATUS_OK) {
      return status;
    }
  }
  hxc_exception_top_frame = frame->previous;
  frame->previous = NULL;
  frame->cleanup_checkpoint = NULL;
  frame->payload = (hxc_value)HXC_VALUE_INVALID_INITIALIZER;
  frame->root_update = NULL;
  frame->root_context = NULL;
  frame->active = false;
  frame->has_payload = false;
  return HXC_STATUS_OK;
}

hxc_status hxc_exception_cleanup_push(
  hxc_exception_cleanup *cleanup,
  hxc_exception_cleanup_fn callback,
  void *context
) {
  if (cleanup == NULL
      || callback == NULL
      || cleanup->active
      || hxc_exception_is_unwinding) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  cleanup->previous = hxc_exception_top_cleanup;
  cleanup->callback = callback;
  cleanup->context = context;
  cleanup->active = true;
  hxc_exception_top_cleanup = cleanup;
  return HXC_STATUS_OK;
}

hxc_status hxc_exception_cleanup_run(hxc_exception_cleanup *cleanup) {
  hxc_exception_cleanup_fn callback;
  void *context;
  if (cleanup == NULL || !cleanup->active || hxc_exception_top_cleanup != cleanup) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  callback = cleanup->callback;
  context = cleanup->context;
  hxc_exception_top_cleanup = cleanup->previous;
  cleanup->previous = NULL;
  cleanup->callback = NULL;
  cleanup->context = NULL;
  cleanup->active = false;
  return callback(context);
}

hxc_status hxc_exception_cleanup_discard(hxc_exception_cleanup *cleanup) {
  if (cleanup == NULL || !cleanup->active || hxc_exception_top_cleanup != cleanup) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  hxc_exception_top_cleanup = cleanup->previous;
  cleanup->previous = NULL;
  cleanup->callback = NULL;
  cleanup->context = NULL;
  cleanup->active = false;
  return HXC_STATUS_OK;
}

hxc_status hxc_exception_raise(const hxc_value *payload) {
  hxc_exception_frame *target = hxc_exception_top_frame;
  const void *managed_payload = NULL;
  hxc_status status;
  if (target == NULL
      || !target->active
      || hxc_exception_is_unwinding
      || !hxc_value_is_valid(payload)) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  status = hxc_value_managed_payload(payload, &managed_payload);
  if (status != HXC_STATUS_OK) {
    return status;
  }
  if (managed_payload != NULL && target->root_update == NULL) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  if (target->root_update != NULL) {
    status = target->root_update(target->root_context, managed_payload);
    if (status != HXC_STATUS_OK) {
      return status;
    }
  }
  target->payload = *payload;
  target->has_payload = true;
  hxc_exception_is_unwinding = true;
  while (hxc_exception_top_cleanup != target->cleanup_checkpoint) {
    if (hxc_exception_top_cleanup == NULL) {
      return hxc_exception_cancel_raise(target, HXC_STATUS_INTERNAL_ERROR);
    }
    status = hxc_exception_cleanup_run(hxc_exception_top_cleanup);
    if (status != HXC_STATUS_OK) {
      return hxc_exception_cancel_raise(target, status);
    }
  }
  hxc_exception_is_unwinding = false;
  longjmp(target->jump_buffer, 1);
}
