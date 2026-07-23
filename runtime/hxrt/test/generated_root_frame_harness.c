/*
 * Native observer for the HxcIR-to-C exact-root emission fixture.
 *
 * The generated translation unit owns the root-frame operations. This separate
 * C harness owns the independent runtime observation: collection happens while
 * the generated frame is active, then again after each normal/early return.
 */
#include "hxrt/gc.h"

#include <inttypes.h>
#include <stdio.h>

typedef struct generated_root_payload {
  int32_t marker;
} generated_root_payload;

hxc_gc hxc_program_gc = HXC_GC_INITIALIZER;
hxc_gc_thread hxc_program_thread = HXC_GC_THREAD_INITIALIZER;

static uint32_t generated_root_finalizers = UINT32_C(0);
static bool generated_root_observation_failed = false;

void hxc_probe_collect_success(void *object);
bool hxc_probe_collect_failure(void *object);
void hxc_generated_root_success(void *value_object);
bool hxc_generated_root_failure(void *value_object);

static void generated_root_finalize(void *object) {
  generated_root_payload *payload = (generated_root_payload *)object;
  payload->marker = INT32_C(-1);
  generated_root_finalizers++;
}

static const hxc_type_descriptor generated_root_descriptor = {
  HXC_TYPE_DESCRIPTOR_ABI_VERSION,
  HXC_TYPE_DESCRIPTOR_HAS_FINALIZER,
  sizeof(generated_root_payload),
  HXC_ALIGNOF(generated_root_payload),
  NULL,
  generated_root_finalize
};

static void generated_root_observe(void *object) {
  if (
    hxc_gc_collect(&hxc_program_gc) != HXC_STATUS_OK
    || !hxc_gc_owns_exact(&hxc_program_gc, object)
  ) {
    generated_root_observation_failed = true;
  }
}

void hxc_probe_collect_success(void *object) {
  generated_root_observe(object);
}

bool hxc_probe_collect_failure(void *object) {
  generated_root_observe(object);
  return false;
}

static bool generated_root_allocate(generated_root_payload **out_payload) {
  void *object = NULL;
  if (
    out_payload == NULL
    || hxc_gc_allocate(
      &hxc_program_gc,
      &generated_root_descriptor,
      &object
    ) != HXC_STATUS_OK
  ) {
    return false;
  }
  *out_payload = (generated_root_payload *)object;
  (*out_payload)->marker = INT32_C(42);
  return true;
}

int main(void) {
  const hxc_gc_config config = {
    hxc_default_allocator(),
    1024u,
    NULL,
    NULL
  };
  generated_root_payload *normal = NULL;
  generated_root_payload *failed = NULL;

  if (
    hxc_gc_init(&config, &hxc_program_gc) != HXC_STATUS_OK
    || hxc_gc_thread_register(
      &hxc_program_gc,
      &hxc_program_thread
    ) != HXC_STATUS_OK
    || !generated_root_allocate(&normal)
  ) {
    return 1;
  }

  hxc_generated_root_success(normal);
  if (
    generated_root_observation_failed
    || hxc_gc_collect(&hxc_program_gc) != HXC_STATUS_OK
    || generated_root_finalizers != UINT32_C(1)
    || !generated_root_allocate(&failed)
  ) {
    return 2;
  }

  if (
    hxc_generated_root_failure(failed)
    || generated_root_observation_failed
    || hxc_gc_collect(&hxc_program_gc) != HXC_STATUS_OK
    || generated_root_finalizers != UINT32_C(2)
  ) {
    return 3;
  }

  if (
    hxc_gc_thread_unregister(&hxc_program_thread) != HXC_STATUS_OK
    || hxc_gc_dispose(&hxc_program_gc) != HXC_STATUS_OK
  ) {
    return 4;
  }
  (void)printf(
    "generated-root-frame: OK finalizers=%" PRIu32 "\n",
    generated_root_finalizers
  );
  return 0;
}
