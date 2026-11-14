/*
 * hxrt feature: thread (compiler-selectable, hosted POSIX adapter).
 *
 * This internal ABI owns pthread object lifetime and converts native failures
 * into hxc_status. It allocates no memory. Generated Haxe wrappers retain their
 * typed values and use these records only behind target-owned classes.
 */
#ifndef HXRT_THREAD_H_INCLUDED
#define HXRT_THREAD_H_INCLUDED

#include "hxrt/status.h"

#include <pthread.h>

#if defined(__cplusplus)
extern "C" {
#endif

typedef struct hxc_mutex {
  pthread_mutex_t native;
  bool initialized;
} hxc_mutex;

typedef struct hxc_condition {
  pthread_mutex_t mutex;
  pthread_cond_t condition;
  bool initialized;
} hxc_condition;

typedef struct hxc_semaphore {
  pthread_mutex_t mutex;
  pthread_cond_t condition;
  size_t permits;
  bool initialized;
} hxc_semaphore;

typedef struct hxc_tls_key {
  pthread_key_t native;
  bool initialized;
} hxc_tls_key;

HXC_API hxc_status hxc_mutex_init(hxc_mutex *mutex);
HXC_API hxc_status hxc_mutex_dispose(hxc_mutex *mutex);
HXC_API hxc_status hxc_mutex_acquire(hxc_mutex *mutex);
HXC_API hxc_status hxc_mutex_try_acquire(hxc_mutex *mutex, bool *out_acquired);
HXC_API hxc_status hxc_mutex_release(hxc_mutex *mutex);

HXC_API hxc_status hxc_condition_init(hxc_condition *condition);
HXC_API hxc_status hxc_condition_dispose(hxc_condition *condition);
HXC_API hxc_status hxc_condition_acquire(hxc_condition *condition);
HXC_API hxc_status hxc_condition_try_acquire(hxc_condition *condition, bool *out_acquired);
HXC_API hxc_status hxc_condition_release(hxc_condition *condition);
HXC_API hxc_status hxc_condition_wait(hxc_condition *condition);
HXC_API hxc_status hxc_condition_signal(hxc_condition *condition);
HXC_API hxc_status hxc_condition_broadcast(hxc_condition *condition);

HXC_API hxc_status hxc_semaphore_init(hxc_semaphore *semaphore, size_t permits);
HXC_API hxc_status hxc_semaphore_dispose(hxc_semaphore *semaphore);
HXC_API hxc_status hxc_semaphore_acquire(hxc_semaphore *semaphore);
HXC_API hxc_status hxc_semaphore_try_acquire(hxc_semaphore *semaphore, bool has_timeout, double timeout_seconds, bool *out_acquired);
HXC_API hxc_status hxc_semaphore_release(hxc_semaphore *semaphore);

HXC_API hxc_status hxc_tls_key_init(hxc_tls_key *key);
HXC_API hxc_status hxc_tls_key_dispose(hxc_tls_key *key);
HXC_API hxc_status hxc_tls_key_get(const hxc_tls_key *key, void **out_value);
HXC_API hxc_status hxc_tls_key_set(hxc_tls_key *key, void *value);

#if defined(__cplusplus)
}
#endif

#endif /* HXRT_THREAD_H_INCLUDED */
