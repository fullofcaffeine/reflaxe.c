#define _POSIX_C_SOURCE 200809L

/*
 * Implementation of compiler-selectable feature `thread`.
 *
 * This adapter keeps pthread object lifetime and native error conversion in
 * one checked boundary. It owns no product policy and performs no allocation.
 */
#include "hxrt/thread.h"

#include <errno.h>
#include <math.h>
#include <stdint.h>
#include <time.h>

static bool hxc_mutex_valid(const hxc_mutex *mutex) {
  return mutex != NULL && mutex->initialized;
}

static bool hxc_condition_valid(const hxc_condition *condition) {
  return condition != NULL && condition->initialized;
}

static bool hxc_semaphore_valid(const hxc_semaphore *semaphore) {
  return semaphore != NULL && semaphore->initialized;
}

static hxc_status hxc_pthread_status(int error) {
  return error == 0 ? HXC_STATUS_OK : HXC_STATUS_INTERNAL_ERROR;
}

static hxc_status hxc_recursive_mutex_init(pthread_mutex_t *mutex) {
  pthread_mutexattr_t attributes;
  int error;

  if (mutex == NULL) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  error = pthread_mutexattr_init(&attributes);
  if (error != 0) {
    return HXC_STATUS_INTERNAL_ERROR;
  }
  error = pthread_mutexattr_settype(&attributes, PTHREAD_MUTEX_RECURSIVE);
  if (error == 0) {
    error = pthread_mutex_init(mutex, &attributes);
  }
  if (pthread_mutexattr_destroy(&attributes) != 0 && error == 0) {
    error = EINVAL;
  }
  return hxc_pthread_status(error);
}

static hxc_status hxc_timeout_deadline(double timeout_seconds, struct timespec *out_deadline) {
  double whole_seconds;
  double fractional_seconds;

  if (out_deadline == NULL || !isfinite(timeout_seconds)) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  if (clock_gettime(CLOCK_REALTIME, out_deadline) != 0) {
    return HXC_STATUS_INTERNAL_ERROR;
  }
  if (timeout_seconds < 0.0) {
    timeout_seconds = 0.0;
  }
  fractional_seconds = modf(timeout_seconds, &whole_seconds);
  if (whole_seconds > (double)INT64_MAX) {
    return HXC_STATUS_OUT_OF_RANGE;
  }
  if (whole_seconds > (double)(INT64_MAX - (int64_t)out_deadline->tv_sec)) {
    return HXC_STATUS_OUT_OF_RANGE;
  }
  out_deadline->tv_sec += (time_t)whole_seconds;
  out_deadline->tv_nsec += (long)(fractional_seconds * 1000000000.0);
  if (out_deadline->tv_nsec >= 1000000000L) {
    out_deadline->tv_sec += (time_t)1;
    out_deadline->tv_nsec -= 1000000000L;
  }
  return HXC_STATUS_OK;
}

hxc_status hxc_mutex_init(hxc_mutex *mutex) {
  hxc_status status;
  if (mutex == NULL || mutex->initialized) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  status = hxc_recursive_mutex_init(&mutex->native);
  if (status == HXC_STATUS_OK) {
    mutex->initialized = true;
  }
  return status;
}

hxc_status hxc_mutex_dispose(hxc_mutex *mutex) {
  int error;
  if (!hxc_mutex_valid(mutex)) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  error = pthread_mutex_destroy(&mutex->native);
  if (error == 0) {
    mutex->initialized = false;
  }
  return hxc_pthread_status(error);
}

hxc_status hxc_mutex_acquire(hxc_mutex *mutex) {
  return hxc_mutex_valid(mutex) ? hxc_pthread_status(pthread_mutex_lock(&mutex->native)) : HXC_STATUS_INVALID_ARGUMENT;
}

hxc_status hxc_mutex_try_acquire(hxc_mutex *mutex, bool *out_acquired) {
  int error;
  if (!hxc_mutex_valid(mutex) || out_acquired == NULL) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  error = pthread_mutex_trylock(&mutex->native);
  if (error == 0) {
    *out_acquired = true;
    return HXC_STATUS_OK;
  }
  if (error == EBUSY) {
    *out_acquired = false;
    return HXC_STATUS_OK;
  }
  return HXC_STATUS_INTERNAL_ERROR;
}

hxc_status hxc_mutex_release(hxc_mutex *mutex) {
  return hxc_mutex_valid(mutex) ? hxc_pthread_status(pthread_mutex_unlock(&mutex->native)) : HXC_STATUS_INVALID_ARGUMENT;
}

hxc_status hxc_condition_init(hxc_condition *condition) {
  hxc_status status;
  if (condition == NULL || condition->initialized) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  status = hxc_recursive_mutex_init(&condition->mutex);
  if (status != HXC_STATUS_OK) {
    return status;
  }
  if (pthread_cond_init(&condition->condition, NULL) != 0) {
    (void)pthread_mutex_destroy(&condition->mutex);
    return HXC_STATUS_INTERNAL_ERROR;
  }
  condition->initialized = true;
  return HXC_STATUS_OK;
}

hxc_status hxc_condition_dispose(hxc_condition *condition) {
  int condition_error;
  int mutex_error;
  if (!hxc_condition_valid(condition)) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  condition_error = pthread_cond_destroy(&condition->condition);
  if (condition_error != 0) {
    return HXC_STATUS_INTERNAL_ERROR;
  }
  mutex_error = pthread_mutex_destroy(&condition->mutex);
  if (mutex_error != 0) {
    return HXC_STATUS_INTERNAL_ERROR;
  }
  condition->initialized = false;
  return HXC_STATUS_OK;
}

hxc_status hxc_condition_acquire(hxc_condition *condition) {
  return hxc_condition_valid(condition) ? hxc_pthread_status(pthread_mutex_lock(&condition->mutex)) : HXC_STATUS_INVALID_ARGUMENT;
}

hxc_status hxc_condition_try_acquire(hxc_condition *condition, bool *out_acquired) {
  int error;
  if (!hxc_condition_valid(condition) || out_acquired == NULL) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  error = pthread_mutex_trylock(&condition->mutex);
  if (error == 0) {
    *out_acquired = true;
    return HXC_STATUS_OK;
  }
  if (error == EBUSY) {
    *out_acquired = false;
    return HXC_STATUS_OK;
  }
  return HXC_STATUS_INTERNAL_ERROR;
}

hxc_status hxc_condition_release(hxc_condition *condition) {
  return hxc_condition_valid(condition) ? hxc_pthread_status(pthread_mutex_unlock(&condition->mutex)) : HXC_STATUS_INVALID_ARGUMENT;
}

hxc_status hxc_condition_wait(hxc_condition *condition) {
  return hxc_condition_valid(condition) ? hxc_pthread_status(pthread_cond_wait(&condition->condition, &condition->mutex)) : HXC_STATUS_INVALID_ARGUMENT;
}

hxc_status hxc_condition_signal(hxc_condition *condition) {
  return hxc_condition_valid(condition) ? hxc_pthread_status(pthread_cond_signal(&condition->condition)) : HXC_STATUS_INVALID_ARGUMENT;
}

hxc_status hxc_condition_broadcast(hxc_condition *condition) {
  return hxc_condition_valid(condition) ? hxc_pthread_status(pthread_cond_broadcast(&condition->condition)) : HXC_STATUS_INVALID_ARGUMENT;
}

hxc_status hxc_semaphore_init(hxc_semaphore *semaphore, size_t permits) {
  if (semaphore == NULL || semaphore->initialized) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  if (pthread_mutex_init(&semaphore->mutex, NULL) != 0) {
    return HXC_STATUS_INTERNAL_ERROR;
  }
  if (pthread_cond_init(&semaphore->condition, NULL) != 0) {
    (void)pthread_mutex_destroy(&semaphore->mutex);
    return HXC_STATUS_INTERNAL_ERROR;
  }
  semaphore->permits = permits;
  semaphore->initialized = true;
  return HXC_STATUS_OK;
}

hxc_status hxc_semaphore_dispose(hxc_semaphore *semaphore) {
  if (!hxc_semaphore_valid(semaphore)) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  if (pthread_cond_destroy(&semaphore->condition) != 0) {
    return HXC_STATUS_INTERNAL_ERROR;
  }
  if (pthread_mutex_destroy(&semaphore->mutex) != 0) {
    return HXC_STATUS_INTERNAL_ERROR;
  }
  semaphore->initialized = false;
  semaphore->permits = 0u;
  return HXC_STATUS_OK;
}

hxc_status hxc_semaphore_acquire(hxc_semaphore *semaphore) {
  int error;
  if (!hxc_semaphore_valid(semaphore)) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  error = pthread_mutex_lock(&semaphore->mutex);
  while (error == 0 && semaphore->permits == 0u) {
    error = pthread_cond_wait(&semaphore->condition, &semaphore->mutex);
  }
  if (error == 0) {
    semaphore->permits -= 1u;
  }
  if (pthread_mutex_unlock(&semaphore->mutex) != 0 && error == 0) {
    error = EINVAL;
  }
  return hxc_pthread_status(error);
}

hxc_status hxc_semaphore_try_acquire(hxc_semaphore *semaphore, bool has_timeout, double timeout_seconds, bool *out_acquired) {
  struct timespec deadline;
  hxc_status status;
  int error;
  if (!hxc_semaphore_valid(semaphore) || out_acquired == NULL) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  if (has_timeout) {
    status = hxc_timeout_deadline(timeout_seconds, &deadline);
    if (status != HXC_STATUS_OK) {
      return status;
    }
  }
  error = pthread_mutex_lock(&semaphore->mutex);
  while (error == 0 && semaphore->permits == 0u && has_timeout) {
    error = pthread_cond_timedwait(&semaphore->condition, &semaphore->mutex, &deadline);
  }
  if (error == 0 && semaphore->permits > 0u) {
    semaphore->permits -= 1u;
    *out_acquired = true;
  } else if (error == 0 || error == ETIMEDOUT) {
    *out_acquired = false;
    error = 0;
  }
  if (pthread_mutex_unlock(&semaphore->mutex) != 0 && error == 0) {
    error = EINVAL;
  }
  return hxc_pthread_status(error);
}

hxc_status hxc_semaphore_release(hxc_semaphore *semaphore) {
  int error;
  if (!hxc_semaphore_valid(semaphore)) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  error = pthread_mutex_lock(&semaphore->mutex);
  if (error == 0 && semaphore->permits == SIZE_MAX) {
    error = ERANGE;
  }
  if (error == 0) {
    semaphore->permits += 1u;
    error = pthread_cond_signal(&semaphore->condition);
  }
  if (pthread_mutex_unlock(&semaphore->mutex) != 0 && error == 0) {
    error = EINVAL;
  }
  return error == ERANGE ? HXC_STATUS_OUT_OF_RANGE : hxc_pthread_status(error);
}

hxc_status hxc_tls_key_init(hxc_tls_key *key) {
  if (key == NULL || key->initialized) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  if (pthread_key_create(&key->native, NULL) != 0) {
    return HXC_STATUS_INTERNAL_ERROR;
  }
  key->initialized = true;
  return HXC_STATUS_OK;
}

hxc_status hxc_tls_key_dispose(hxc_tls_key *key) {
  if (key == NULL || !key->initialized) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  if (pthread_key_delete(key->native) != 0) {
    return HXC_STATUS_INTERNAL_ERROR;
  }
  key->initialized = false;
  return HXC_STATUS_OK;
}

hxc_status hxc_tls_key_get(const hxc_tls_key *key, void **out_value) {
  if (key == NULL || !key->initialized || out_value == NULL) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  *out_value = pthread_getspecific(key->native);
  return HXC_STATUS_OK;
}

hxc_status hxc_tls_key_set(hxc_tls_key *key, void *value) {
  if (key == NULL || !key->initialized) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  return hxc_pthread_status(pthread_setspecific(key->native, value));
}
