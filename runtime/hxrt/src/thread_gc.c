/*
 * Implementation of compiler-selectable feature `thread-gc`.
 *
 * A collector request waits until every other registered thread is parked at
 * a generated safepoint. The requesting thread then uses the unchanged exact
 * collector while holding the coordinator mutex and releases all peers after
 * collection or allocation completes.
 */
#include "hxrt/thread_gc.h"

#include <errno.h>

static bool hxc_gc_thread_coordinator_valid(const hxc_gc_thread_coordinator *coordinator) {
  return coordinator != NULL && coordinator->initialized && coordinator->collector != NULL;
}

static bool hxc_gc_thread_token_valid(const hxc_gc_thread_token *token) {
  return token != NULL && token->registered && token->coordinator != NULL;
}

static hxc_status hxc_gc_thread_stop_world(hxc_gc_thread_token *token) {
  hxc_gc_thread_coordinator *coordinator;
  int error;
  if (!hxc_gc_thread_token_valid(token)) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  coordinator = token->coordinator;
  error = pthread_mutex_lock(&coordinator->mutex);
  if (error != 0) {
    return HXC_STATUS_INTERNAL_ERROR;
  }
  if (coordinator->collection_requested) {
    (void)pthread_mutex_unlock(&coordinator->mutex);
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  coordinator->collection_requested = true;
  coordinator->collector_token = token;
  error = pthread_cond_broadcast(&coordinator->condition);
  while (error == 0 && coordinator->parked_count + 1u < coordinator->registered_count) {
    error = pthread_cond_wait(&coordinator->condition, &coordinator->mutex);
  }
  if (error != 0) {
    coordinator->collection_requested = false;
    coordinator->collector_token = NULL;
    (void)pthread_cond_broadcast(&coordinator->condition);
    (void)pthread_mutex_unlock(&coordinator->mutex);
    return HXC_STATUS_INTERNAL_ERROR;
  }
  return HXC_STATUS_OK;
}

static hxc_status hxc_gc_thread_resume_world(hxc_gc_thread_token *token, hxc_status operation_status) {
  hxc_gc_thread_coordinator *coordinator = token->coordinator;
  int signal_error;
  int unlock_error;
  coordinator->collection_requested = false;
  coordinator->collector_token = NULL;
  signal_error = pthread_cond_broadcast(&coordinator->condition);
  unlock_error = pthread_mutex_unlock(&coordinator->mutex);
  return signal_error == 0 && unlock_error == 0 ? operation_status : HXC_STATUS_INTERNAL_ERROR;
}

hxc_status hxc_gc_thread_coordinator_init(hxc_gc *collector, hxc_gc_thread_coordinator *out_coordinator) {
  if (collector == NULL || !collector->initialized || out_coordinator == NULL || out_coordinator->initialized) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  if (pthread_mutex_init(&out_coordinator->mutex, NULL) != 0) {
    return HXC_STATUS_INTERNAL_ERROR;
  }
  if (pthread_cond_init(&out_coordinator->condition, NULL) != 0) {
    (void)pthread_mutex_destroy(&out_coordinator->mutex);
    return HXC_STATUS_INTERNAL_ERROR;
  }
  out_coordinator->collector = collector;
  out_coordinator->registered_count = 0u;
  out_coordinator->parked_count = 0u;
  out_coordinator->collector_token = NULL;
  out_coordinator->collection_requested = false;
  out_coordinator->initialized = true;
  return HXC_STATUS_OK;
}

hxc_status hxc_gc_thread_coordinator_dispose(hxc_gc_thread_coordinator *coordinator) {
  if (!hxc_gc_thread_coordinator_valid(coordinator) || coordinator->registered_count != 0u || coordinator->collection_requested) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  if (pthread_cond_destroy(&coordinator->condition) != 0 || pthread_mutex_destroy(&coordinator->mutex) != 0) {
    return HXC_STATUS_INTERNAL_ERROR;
  }
  coordinator->collector = NULL;
  coordinator->initialized = false;
  return HXC_STATUS_OK;
}

hxc_status hxc_gc_thread_token_register(hxc_gc_thread_coordinator *coordinator, hxc_gc_thread_token *token) {
  hxc_status status;
  int error;
  if (!hxc_gc_thread_coordinator_valid(coordinator) || token == NULL || token->registered || token->coordinator != NULL) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  error = pthread_mutex_lock(&coordinator->mutex);
  if (error != 0) {
    return HXC_STATUS_INTERNAL_ERROR;
  }
  if (coordinator->collection_requested || coordinator->registered_count == SIZE_MAX) {
    (void)pthread_mutex_unlock(&coordinator->mutex);
    return coordinator->registered_count == SIZE_MAX ? HXC_STATUS_OUT_OF_RANGE : HXC_STATUS_INVALID_ARGUMENT;
  }
  status = hxc_gc_thread_register(coordinator->collector, &token->roots);
  if (status == HXC_STATUS_OK) {
    token->coordinator = coordinator;
    token->registered = true;
    coordinator->registered_count += 1u;
  }
  if (pthread_mutex_unlock(&coordinator->mutex) != 0) {
    return HXC_STATUS_INTERNAL_ERROR;
  }
  return status;
}

hxc_status hxc_gc_thread_token_unregister(hxc_gc_thread_token *token) {
  hxc_gc_thread_coordinator *coordinator;
  hxc_status status;
  if (!hxc_gc_thread_token_valid(token) || token->parked || token->roots.top_frame != NULL) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  coordinator = token->coordinator;
  if (pthread_mutex_lock(&coordinator->mutex) != 0) {
    return HXC_STATUS_INTERNAL_ERROR;
  }
  if (coordinator->collection_requested) {
    (void)pthread_mutex_unlock(&coordinator->mutex);
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  status = hxc_gc_thread_unregister(&token->roots);
  if (status == HXC_STATUS_OK) {
    coordinator->registered_count -= 1u;
    token->coordinator = NULL;
    token->registered = false;
  }
  if (pthread_mutex_unlock(&coordinator->mutex) != 0) {
    return HXC_STATUS_INTERNAL_ERROR;
  }
  return status;
}

hxc_status hxc_gc_thread_safepoint(hxc_gc_thread_token *token) {
  hxc_gc_thread_coordinator *coordinator;
  int error;
  if (!hxc_gc_thread_token_valid(token)) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  coordinator = token->coordinator;
  error = pthread_mutex_lock(&coordinator->mutex);
  if (error != 0) {
    return HXC_STATUS_INTERNAL_ERROR;
  }
  if (coordinator->collection_requested && coordinator->collector_token != token) {
    token->parked = true;
    coordinator->parked_count += 1u;
    error = pthread_cond_broadcast(&coordinator->condition);
    while (error == 0 && coordinator->collection_requested) {
      error = pthread_cond_wait(&coordinator->condition, &coordinator->mutex);
    }
    coordinator->parked_count -= 1u;
    token->parked = false;
  }
  if (pthread_mutex_unlock(&coordinator->mutex) != 0 || error != 0) {
    return HXC_STATUS_INTERNAL_ERROR;
  }
  return HXC_STATUS_OK;
}

hxc_status hxc_gc_thread_collect(hxc_gc_thread_token *token) {
  hxc_status status = hxc_gc_thread_stop_world(token);
  if (status != HXC_STATUS_OK) {
    return status;
  }
  status = hxc_gc_collect(token->coordinator->collector);
  return hxc_gc_thread_resume_world(token, status);
}

hxc_status hxc_gc_thread_allocate(hxc_gc_thread_token *token, const hxc_type_descriptor *descriptor, void **out_object) {
  hxc_status status = hxc_gc_thread_stop_world(token);
  if (status != HXC_STATUS_OK) {
    return status;
  }
  status = hxc_gc_allocate(token->coordinator->collector, descriptor, out_object);
  return hxc_gc_thread_resume_world(token, status);
}
