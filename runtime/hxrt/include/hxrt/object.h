/*
 * hxrt feature: object.
 *
 * This header is the small, collector-neutral description of one managed
 * object payload. Generated code provides immutable descriptors only for
 * reachable types that actually need managed allocation, exact tracing, or
 * cleanup. Runtime-free classes keep their direct private C structs and never
 * include this header.
 *
 * A descriptor is not reflection metadata. It deliberately contains no Haxe
 * name, field-name table, method table, or dynamic-construction hook. Later
 * reflection support may point at a descriptor, but it owns that extra data.
 */
#ifndef HXRT_OBJECT_H_INCLUDED
#define HXRT_OBJECT_H_INCLUDED

#include "hxrt/base.h"

#if defined(__cplusplus)
extern "C" {
#endif

/* Version this narrow layout independently from the wider internal hxrt ABI. */
#define HXC_TYPE_DESCRIPTOR_ABI_VERSION UINT32_C(1)

#define HXC_TYPE_DESCRIPTOR_HAS_TRACE UINT32_C(1)
#define HXC_TYPE_DESCRIPTOR_HAS_FINALIZER UINT32_C(2)
#define HXC_TYPE_DESCRIPTOR_KNOWN_FLAGS \
  (HXC_TYPE_DESCRIPTOR_HAS_TRACE | HXC_TYPE_DESCRIPTOR_HAS_FINALIZER)

/**
 * Report one exact outgoing managed reference to a tracing backend.
 *
 * `managed_object` is either null or the stable base address of another
 * managed payload. An interior or foreign pointer is not accepted as an
 * object merely because it has an address-shaped value.
 */
typedef void (*hxc_trace_visit_fn)(void *context, const void *managed_object);

/** Visit every live managed reference stored in one payload. */
typedef void (*hxc_type_trace_fn)(
  const void *object,
  hxc_trace_visit_fn visit,
  void *visit_context
);

/**
 * Release non-memory resources owned by one unreachable payload.
 *
 * The future collector controls at-most-once invocation. Portable Haxe does
 * not promise prompt or deterministic finalization, so native resources should
 * normally use explicit ownership and cleanup instead.
 */
typedef void (*hxc_type_finalize_fn)(void *object);

/**
 * Immutable internal facts needed by allocation, tracing, and cleanup.
 *
 * Generated definitions use `sizeof(T)` and `HXC_ALIGNOF(T)`, allowing the C
 * compiler to remain the final authority for the private payload layout.
 */
typedef struct hxc_type_descriptor {
  uint32_t abi_version;
  uint32_t flags;
  size_t object_size;
  size_t object_alignment;
  hxc_type_trace_fn trace;
  hxc_type_finalize_fn finalize;
} hxc_type_descriptor;

/**
 * Collector-neutral prefix for a managed allocation.
 *
 * This prefix is runtime-owned storage adjacent to, but not a member of, the
 * generated Haxe class payload. Collector-private links and mark state belong
 * to the `gc` feature and must not leak into this descriptor contract.
 */
typedef struct hxc_object_header {
  const hxc_type_descriptor *descriptor;
} hxc_object_header;

/** Validate the descriptor version, flags, callbacks, size, and alignment. */
HXC_API bool hxc_type_descriptor_is_valid(
  const hxc_type_descriptor *descriptor
);

/** Initialize an empty header with one validated immutable descriptor. */
HXC_API bool hxc_object_header_init(
  const hxc_type_descriptor *descriptor,
  hxc_object_header *out_header
);

/** Validate that a header points at a valid descriptor. */
HXC_API bool hxc_object_header_is_valid(const hxc_object_header *header);

/**
 * Run the descriptor's exact trace function when it has one.
 *
 * A leaf descriptor succeeds without a visitor. Invalid descriptors, null
 * payloads, or missing visitors for traced objects fail without calling user
 * code.
 */
HXC_API bool hxc_type_descriptor_trace(
  const hxc_type_descriptor *descriptor,
  const void *object,
  hxc_trace_visit_fn visit,
  void *visit_context
);

/**
 * Run the optional finalizer.
 *
 * This helper validates shape and dispatches once per call; the collector owns
 * the separate proof that a particular object is finalized at most once.
 */
HXC_API bool hxc_type_descriptor_finalize(
  const hxc_type_descriptor *descriptor,
  void *object
);

#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif /* HXRT_OBJECT_H_INCLUDED */
