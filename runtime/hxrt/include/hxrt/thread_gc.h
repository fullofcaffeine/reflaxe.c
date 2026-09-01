/*
 * hxrt feature: thread-gc (compiler-selectable coordination adapter).
 *
 * This internal ABI combines the otherwise single-thread collector with the
 * POSIX thread slice. Every managed thread owns one exact root chain and must
 * reach generated safepoints. Single-thread programs never select this file.
 */
#ifndef HXRT_THREAD_GC_H_INCLUDED
#define HXRT_THREAD_GC_H_INCLUDED

#include "hxrt/gc.h"
#include "hxrt/thread.h"

#if defined(__cplusplus)
extern "C" {
#endif

typedef struct hxc_gc_thread_coordinator {
  hxc_gc *collector;
  pthread_mutex_t mutex;
  pthread_cond_t condition;
  size_t registered_count;
  size_t parked_count;
  struct hxc_gc_thread_token *collector_token;
  bool collection_requested;
  bool initialized;
} hxc_gc_thread_coordinator;

typedef struct hxc_gc_thread_token {
  hxc_gc_thread_coordinator *coordinator;
  hxc_gc_thread roots;
  bool registered;
  bool parked;
} hxc_gc_thread_token;

HXC_API hxc_status hxc_gc_thread_coordinator_init(hxc_gc *collector, hxc_gc_thread_coordinator *out_coordinator);
HXC_API hxc_status hxc_gc_thread_coordinator_dispose(hxc_gc_thread_coordinator *coordinator);
HXC_API hxc_status hxc_gc_thread_token_register(hxc_gc_thread_coordinator *coordinator, hxc_gc_thread_token *token);
HXC_API hxc_status hxc_gc_thread_token_unregister(hxc_gc_thread_token *token);
HXC_API hxc_status hxc_gc_thread_safepoint(hxc_gc_thread_token *token);
HXC_API hxc_status hxc_gc_thread_collect(hxc_gc_thread_token *token);
HXC_API hxc_status hxc_gc_thread_allocate(
  hxc_gc_thread_token *token,
  const hxc_type_descriptor *descriptor,
  void **out_object
);

#if defined(__cplusplus)
}
#endif

#endif /* HXRT_THREAD_GC_H_INCLUDED */
