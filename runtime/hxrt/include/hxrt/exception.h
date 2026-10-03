/*
 * hxrt feature: exception.
 *
 * General Haxe exceptions use a thread-local chain of compiler-owned frames.
 * Generated code keeps setjmp in the function that owns each frame, while this
 * slice owns payload transport, reverse cleanup, active-frame checks, and the
 * final same-thread longjmp. No frame or jump token is a public application ABI.
 */
#ifndef HXRT_EXCEPTION_H_INCLUDED
#define HXRT_EXCEPTION_H_INCLUDED

#include <setjmp.h>

#include "hxrt/dynamic.h"

#if defined(__cplusplus)
extern "C" {
#endif

typedef struct hxc_exception_frame hxc_exception_frame;
typedef struct hxc_exception_cleanup hxc_exception_cleanup;

/** Publish or clear the managed object hidden in one frame payload. */
typedef hxc_status (*hxc_exception_root_update_fn)(
  void *context,
  const void *managed_payload
);

/** Run one compiler-registered cleanup; recursive raise attempts fail closed. */
typedef hxc_status (*hxc_exception_cleanup_fn)(void *context);

/** Store a frame's managed payload in one compiler-owned root slot. */
HXC_API hxc_status hxc_exception_root_slot_update(
  void *context,
  const void *managed_payload
);

/** One last-in, first-out cleanup record owned by generated automatic storage. */
struct hxc_exception_cleanup {
  hxc_exception_cleanup *previous;
  hxc_exception_cleanup_fn callback;
  void *context;
  bool active;
};

/** One active lexical handler and its rooted non-owning Dynamic payload. */
struct hxc_exception_frame {
  jmp_buf jump_buffer;
  hxc_exception_frame *previous;
  hxc_exception_cleanup *cleanup_checkpoint;
  /* These fields can change after setjmp and are read after longjmp. */
  volatile hxc_value payload;
  hxc_exception_root_update_fn root_update;
  void *root_context;
  bool active;
  volatile bool has_payload;
};

#define HXC_EXCEPTION_CLEANUP_INITIALIZER \
  { NULL, NULL, NULL, false }

/* C++ value-initialization handles opaque, platform-specific jmp_buf nesting.
 * The zero state is inactive and gives the Dynamic payload its invalid sentinel. */
#ifdef __cplusplus
#define HXC_EXCEPTION_FRAME_INITIALIZER {}
#else
#define HXC_EXCEPTION_FRAME_INITIALIZER \
  { { 0 }, NULL, NULL, HXC_VALUE_INVALID_INITIALIZER, NULL, NULL, false, false }
#endif

/* setjmp must remain in the generated function that owns the automatic frame. */
#define HXC_EXCEPTION_SETJMP(frame_pointer) setjmp((frame_pointer)->jump_buffer)

/** Push one inactive frame on this thread and remember its cleanup checkpoint. */
HXC_API hxc_status hxc_exception_frame_push(
  hxc_exception_frame *frame,
  hxc_exception_root_update_fn root_update,
  void *root_context
);

/** Copy the caught payload while the frame and any managed root remain active. */
HXC_API hxc_status hxc_exception_frame_payload(
  const hxc_exception_frame *frame,
  hxc_value *out_payload
);

/** Copy the payload and transfer its published root to generated function state. */
HXC_API hxc_status hxc_exception_frame_take_payload(
  hxc_exception_frame *frame,
  hxc_value *out_payload
);

/** Pop the current frame after all cleanup records above its checkpoint ended. */
HXC_API hxc_status hxc_exception_frame_pop(hxc_exception_frame *frame);

/**
 * Register one inactive cleanup on this thread's owner stack.
 *
 * Registration is also valid before a frame exists. A later frame records the
 * current stack as its checkpoint, so older owners remain outside that try.
 */
HXC_API hxc_status hxc_exception_cleanup_push(
  hxc_exception_cleanup *cleanup,
  hxc_exception_cleanup_fn callback,
  void *context
);

/** Run and remove the current cleanup on an ordinary lexical exit. */
HXC_API hxc_status hxc_exception_cleanup_run(hxc_exception_cleanup *cleanup);

/** Remove the current cleanup after its owner transfers across a normal exit. */
HXC_API hxc_status hxc_exception_cleanup_discard(hxc_exception_cleanup *cleanup);

/**
 * Raise to the current same-thread frame after reverse cleanup.
 *
 * A valid active target does not return. Missing frames, malformed payloads,
 * root publication failure, recursive raise, and cleanup failure return a
 * status so generated code can fail closed at its existing abort boundary. A
 * failed cleanup unwind clears a successfully published payload root first.
 */
HXC_API hxc_status hxc_exception_raise(const hxc_value *payload);

#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif /* HXRT_EXCEPTION_H_INCLUDED */
