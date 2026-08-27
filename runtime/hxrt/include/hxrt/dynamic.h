/*
 * hxrt feature: dynamic.
 *
 * Source-required Haxe Dynamic values use this private tagged carrier. Null,
 * Bool, Int, and Float stay inline. Reference-shaped values carry one exact
 * collector base: either the source object or a generated wrapper whose fields
 * retain their real C types. In particular, a Haxe function pointer stays in a
 * typed wrapper field and never passes through a data pointer or integer.
 *
 * The carrier is a non-owning value. HxcIR and generated root tables own its
 * lifetime. The descriptor contains no names or reflection data; its numeric
 * identity connects the carrier to program-generated checked operations.
 */
#ifndef HXRT_DYNAMIC_H_INCLUDED
#define HXRT_DYNAMIC_H_INCLUDED

#include "hxrt/status.h"

#if defined(__cplusplus)
extern "C" {
#endif

/** Version the private Dynamic descriptor independently from other layouts. */
#define HXC_DYNAMIC_TYPE_ABI_VERSION UINT32_C(1)

/** Source-language category of one closed-world Dynamic adapter. */
typedef enum hxc_dynamic_category {
  HXC_DYNAMIC_CATEGORY_NULL = 0,
  HXC_DYNAMIC_CATEGORY_BOOL = 1,
  HXC_DYNAMIC_CATEGORY_INT = 2,
  HXC_DYNAMIC_CATEGORY_FLOAT = 3,
  HXC_DYNAMIC_CATEGORY_STRING = 4,
  HXC_DYNAMIC_CATEGORY_ARRAY = 5,
  HXC_DYNAMIC_CATEGORY_OBJECT = 6,
  HXC_DYNAMIC_CATEGORY_ENUM = 7,
  HXC_DYNAMIC_CATEGORY_FUNCTION = 8,
  HXC_DYNAMIC_CATEGORY_TYPE_VALUE = 9
} hxc_dynamic_category;

/** Active carrier member selected for one exact source category. */
typedef enum hxc_dynamic_storage {
  HXC_DYNAMIC_STORAGE_INLINE_NULL = 0,
  HXC_DYNAMIC_STORAGE_INLINE_BOOL = 1,
  HXC_DYNAMIC_STORAGE_INLINE_INT32 = 2,
  HXC_DYNAMIC_STORAGE_INLINE_FLOAT64 = 3,
  HXC_DYNAMIC_STORAGE_MANAGED_REFERENCE = 4,
  HXC_DYNAMIC_STORAGE_MANAGED_WRAPPER = 5,
  HXC_DYNAMIC_STORAGE_STATIC_TOKEN = 6
} hxc_dynamic_storage;

/**
 * Immutable identity for one reachable Dynamic source type.
 *
 * `type_id` is assigned by the validated program plan. Generated operations
 * use that identity to select one exact adapter; it is not a reflection token
 * and is not stable across independently compiled programs.
 */
typedef struct hxc_dynamic_type {
  uint32_t abi_version;
  uint32_t type_id;
  hxc_dynamic_category category;
  hxc_dynamic_storage storage;
} hxc_dynamic_type;

/**
 * One private Dynamic value with an explicitly tracked active union member.
 *
 * Only the member named by `active_storage` may be read. All pointer-shaped
 * managed storage uses the mutable object member because Haxe objects and
 * anonymous structures can be changed through Dynamic. Static type tokens use
 * a separate const member. Neither member stores a function pointer: generated
 * function wrappers keep the exact function type.
 */
typedef struct hxc_value {
  const hxc_dynamic_type *type;
  hxc_dynamic_storage active_storage;
  union {
    bool boolean;
    int32_t int32_value;
    double float64_value;
    void *object;
    const void *static_token;
  } payload;
} hxc_value;

/** Invalid zero state suitable only as an output sentinel before construction. */
#define HXC_VALUE_INVALID_INITIALIZER \
  { NULL, HXC_DYNAMIC_STORAGE_INLINE_NULL, { false } }

/** Check descriptor version plus the exact category/storage pairing. */
HXC_API bool hxc_dynamic_type_is_valid(const hxc_dynamic_type *type);

/** Check the descriptor, active tag, and required non-null pointer payload. */
HXC_API bool hxc_value_is_valid(const hxc_value *value);

/** Construct the canonical null value. The output is unchanged on failure. */
HXC_API hxc_status hxc_value_init_null(
  const hxc_dynamic_type *type,
  hxc_value *out_value
);

/** Construct an inline Bool. The output is unchanged on failure. */
HXC_API hxc_status hxc_value_init_bool(
  const hxc_dynamic_type *type,
  bool payload,
  hxc_value *out_value
);

/** Construct an inline signed Haxe Int. The output is unchanged on failure. */
HXC_API hxc_status hxc_value_init_int32(
  const hxc_dynamic_type *type,
  int32_t payload,
  hxc_value *out_value
);

/** Construct an inline binary64 Haxe Float. The output is unchanged on failure. */
HXC_API hxc_status hxc_value_init_float64(
  const hxc_dynamic_type *type,
  double payload,
  hxc_value *out_value
);

/**
 * Construct an exact managed object reference.
 *
 * `managed_object` must be the non-null collector allocation base. The runtime
 * does not discover or retain it; the generated root plan owns that proof.
 */
HXC_API hxc_status hxc_value_init_managed_reference(
  const hxc_dynamic_type *type,
  void *managed_object,
  hxc_value *out_value
);

/**
 * Construct a reference to one generated exact typed wrapper.
 *
 * `managed_wrapper` must be the non-null collector base of that wrapper. Its
 * generated layout and trace function preserve the source value's real type.
 */
HXC_API hxc_status hxc_value_init_managed_wrapper(
  const hxc_dynamic_type *type,
  void *managed_wrapper,
  hxc_value *out_value
);

/** Construct an immutable program-owned type token. */
HXC_API hxc_status hxc_value_init_static_token(
  const hxc_dynamic_type *type,
  const void *static_token,
  hxc_value *out_value
);

/** Report whether one valid carrier is the canonical Dynamic null. */
HXC_API hxc_status hxc_value_is_null(
  const hxc_value *value,
  bool *out_is_null
);

/** Read the active Bool member without inspecting any other union member. */
HXC_API hxc_status hxc_value_read_bool(
  const hxc_value *value,
  bool *out_payload
);

/** Read the active Int member without inspecting any other union member. */
HXC_API hxc_status hxc_value_read_int32(
  const hxc_value *value,
  int32_t *out_payload
);

/** Read the active Float member without inspecting any other union member. */
HXC_API hxc_status hxc_value_read_float64(
  const hxc_value *value,
  double *out_payload
);

/** Read one exact managed object base. */
HXC_API hxc_status hxc_value_read_managed_reference(
  const hxc_value *value,
  void **out_managed_object
);

/** Read one generated exact typed wrapper base. */
HXC_API hxc_status hxc_value_read_managed_wrapper(
  const hxc_value *value,
  void **out_managed_wrapper
);

/** Read one immutable program-owned type token. */
HXC_API hxc_status hxc_value_read_static_token(
  const hxc_value *value,
  const void **out_static_token
);

/**
 * Project the collector root hidden by a Dynamic carrier.
 *
 * Scalars and static type tokens produce null. Managed references and wrappers
 * produce their exact non-null allocation base. The output is unchanged when
 * the carrier is malformed.
 */
HXC_API hxc_status hxc_value_managed_payload(
  const hxc_value *value,
  const void **out_managed_object
);

#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif /* HXRT_DYNAMIC_H_INCLUDED */
