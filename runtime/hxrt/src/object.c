/*
 * Implementation of compiler-selectable feature `object`.
 *
 * The functions here validate and dispatch immutable descriptor facts. They do
 * not allocate, discover roots, scan memory conservatively, or choose a
 * collector. That separation keeps direct and metal class layouts free of a
 * hidden runtime header and gives the future precise collector one small,
 * versioned contract to consume.
 */
#include "hxrt/object.h"

static bool hxc_object_is_power_of_two(size_t value) {
  return value != 0u && (value & (value - 1u)) == 0u;
}

bool hxc_type_descriptor_is_valid(const hxc_type_descriptor *descriptor) {
  const bool has_trace = descriptor != NULL
    && (descriptor->flags & HXC_TYPE_DESCRIPTOR_HAS_TRACE) != UINT32_C(0);
  const bool has_finalizer = descriptor != NULL
    && (descriptor->flags & HXC_TYPE_DESCRIPTOR_HAS_FINALIZER) != UINT32_C(0);

  return descriptor != NULL
    && descriptor->abi_version == HXC_TYPE_DESCRIPTOR_ABI_VERSION
    && (descriptor->flags & ~HXC_TYPE_DESCRIPTOR_KNOWN_FLAGS) == UINT32_C(0)
    && descriptor->object_size != 0u
    && hxc_object_is_power_of_two(descriptor->object_alignment)
    && descriptor->object_size % descriptor->object_alignment == 0u
    && has_trace == (descriptor->trace != NULL)
    && has_finalizer == (descriptor->finalize != NULL);
}

bool hxc_object_header_init(
  const hxc_type_descriptor *descriptor,
  hxc_object_header *out_header
) {
  hxc_object_header header;
  if (out_header == NULL || !hxc_type_descriptor_is_valid(descriptor)) {
    return false;
  }
  header.descriptor = descriptor;
  *out_header = header;
  return true;
}

bool hxc_object_header_is_valid(const hxc_object_header *header) {
  return header != NULL && hxc_type_descriptor_is_valid(header->descriptor);
}

bool hxc_type_descriptor_trace(
  const hxc_type_descriptor *descriptor,
  const void *object,
  hxc_trace_visit_fn visit,
  void *visit_context
) {
  if (!hxc_type_descriptor_is_valid(descriptor) || object == NULL) {
    return false;
  }
  if (descriptor->trace == NULL) {
    return true;
  }
  if (visit == NULL) {
    return false;
  }
  descriptor->trace(object, visit, visit_context);
  return true;
}

bool hxc_type_descriptor_finalize(
  const hxc_type_descriptor *descriptor,
  void *object
) {
  if (!hxc_type_descriptor_is_valid(descriptor) || object == NULL) {
    return false;
  }
  if (descriptor->finalize != NULL) {
    descriptor->finalize(object);
  }
  return true;
}
