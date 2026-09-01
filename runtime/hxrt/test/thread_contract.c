#include "hxrt/thread.h"

#include <stdatomic.h>
#include <stdio.h>

typedef struct condition_fixture {
  hxc_condition condition;
  atomic_bool waiting;
  bool proceed;
  bool completed;
} condition_fixture;

typedef struct semaphore_fixture {
  hxc_semaphore semaphore;
  atomic_bool completed;
} semaphore_fixture;

typedef struct tls_fixture {
  hxc_tls_key *key;
  void *expected;
  bool passed;
} tls_fixture;

static bool check_status(hxc_status actual, hxc_status expected, const char *label) {
  if (actual == expected) {
    return true;
  }
  (void)fprintf(stderr, "%s: expected status %d, got %d\n", label, (int)expected, (int)actual);
  return false;
}

static void *condition_worker(void *context) {
  condition_fixture *fixture = (condition_fixture *)context;
  if (hxc_condition_acquire(&fixture->condition) != HXC_STATUS_OK) {
    return NULL;
  }
  atomic_store_explicit(&fixture->waiting, true, memory_order_release);
  while (!fixture->proceed) {
    if (hxc_condition_wait(&fixture->condition) != HXC_STATUS_OK) {
      (void)hxc_condition_release(&fixture->condition);
      return NULL;
    }
  }
  fixture->completed = true;
  if (hxc_condition_release(&fixture->condition) != HXC_STATUS_OK) {
    return NULL;
  }
  return fixture;
}

static void *semaphore_worker(void *context) {
  semaphore_fixture *fixture = (semaphore_fixture *)context;
  if (hxc_semaphore_acquire(&fixture->semaphore) != HXC_STATUS_OK) {
    return NULL;
  }
  atomic_store_explicit(&fixture->completed, true, memory_order_release);
  return fixture;
}

static void *tls_worker(void *context) {
  tls_fixture *fixture = (tls_fixture *)context;
  void *value = fixture->expected;
  if (hxc_tls_key_get(fixture->key, &value) != HXC_STATUS_OK || value != NULL) {
    return NULL;
  }
  if (hxc_tls_key_set(fixture->key, fixture->expected) != HXC_STATUS_OK) {
    return NULL;
  }
  if (hxc_tls_key_get(fixture->key, &value) != HXC_STATUS_OK || value != fixture->expected) {
    return NULL;
  }
  fixture->passed = true;
  return fixture;
}

int main(void) {
  hxc_mutex mutex = {0};
  condition_fixture condition = {0};
  semaphore_fixture semaphore = {0};
  hxc_tls_key key = {0};
  tls_fixture tls = {0};
  pthread_t worker;
  void *worker_result = NULL;
  bool acquired = false;
  int main_marker = 1;
  int worker_marker = 2;
  void *tls_value = NULL;

  if (!check_status(hxc_mutex_init(&mutex), HXC_STATUS_OK, "mutex init") ||
      !check_status(hxc_mutex_acquire(&mutex), HXC_STATUS_OK, "mutex acquire") ||
      !check_status(hxc_mutex_try_acquire(&mutex, &acquired), HXC_STATUS_OK, "recursive mutex try") || !acquired ||
      !check_status(hxc_mutex_release(&mutex), HXC_STATUS_OK, "recursive mutex release") ||
      !check_status(hxc_mutex_release(&mutex), HXC_STATUS_OK, "mutex release") ||
      !check_status(hxc_mutex_dispose(&mutex), HXC_STATUS_OK, "mutex dispose")) {
    return 1;
  }

  if (!check_status(hxc_condition_init(&condition.condition), HXC_STATUS_OK, "condition init")) {
    return 2;
  }
  atomic_init(&condition.waiting, false);
  if (pthread_create(&worker, NULL, condition_worker, &condition) != 0) {
    return 3;
  }
  while (!atomic_load_explicit(&condition.waiting, memory_order_acquire)) {
  }
  if (!check_status(hxc_condition_acquire(&condition.condition), HXC_STATUS_OK, "condition main acquire")) {
    return 4;
  }
  condition.proceed = true;
  if (!check_status(hxc_condition_signal(&condition.condition), HXC_STATUS_OK, "condition signal") ||
      !check_status(hxc_condition_release(&condition.condition), HXC_STATUS_OK, "condition main release") ||
      pthread_join(worker, &worker_result) != 0 || worker_result != &condition || !condition.completed ||
      !check_status(hxc_condition_dispose(&condition.condition), HXC_STATUS_OK, "condition dispose")) {
    return 5;
  }

  if (!check_status(hxc_semaphore_init(&semaphore.semaphore, 0u), HXC_STATUS_OK, "semaphore init") ||
      !check_status(hxc_semaphore_try_acquire(&semaphore.semaphore, false, 0.0, &acquired), HXC_STATUS_OK, "semaphore nonblocking") || acquired ||
      !check_status(hxc_semaphore_try_acquire(&semaphore.semaphore, true, 0.01, &acquired), HXC_STATUS_OK, "semaphore timeout") || acquired) {
    return 6;
  }
  atomic_init(&semaphore.completed, false);
  if (pthread_create(&worker, NULL, semaphore_worker, &semaphore) != 0 ||
      !check_status(hxc_semaphore_release(&semaphore.semaphore), HXC_STATUS_OK, "semaphore release") ||
      pthread_join(worker, &worker_result) != 0 || worker_result != &semaphore ||
      !atomic_load_explicit(&semaphore.completed, memory_order_acquire) ||
      !check_status(hxc_semaphore_dispose(&semaphore.semaphore), HXC_STATUS_OK, "semaphore dispose")) {
    return 7;
  }

  if (!check_status(hxc_tls_key_init(&key), HXC_STATUS_OK, "TLS init") ||
      !check_status(hxc_tls_key_set(&key, &main_marker), HXC_STATUS_OK, "TLS main set") ||
      !check_status(hxc_tls_key_get(&key, &tls_value), HXC_STATUS_OK, "TLS main get") || tls_value != &main_marker) {
    return 8;
  }
  tls.key = &key;
  tls.expected = &worker_marker;
  if (pthread_create(&worker, NULL, tls_worker, &tls) != 0 || pthread_join(worker, &worker_result) != 0 ||
      worker_result != &tls || !tls.passed ||
      !check_status(hxc_tls_key_get(&key, &tls_value), HXC_STATUS_OK, "TLS main isolation") || tls_value != &main_marker ||
      !check_status(hxc_tls_key_dispose(&key), HXC_STATUS_OK, "TLS dispose")) {
    return 9;
  }

  return 0;
}
