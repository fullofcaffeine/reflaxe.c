/*
 * Implementation of compiler-selectable feature `dynamic`.
 *
 * Every constructor initializes one union member and every reader checks the
 * active tag before touching that member. The slice allocates nothing and has
 * no object-descriptor or collector dependency; generated managed adapters add
 * those features only when a reachable Dynamic source value needs them.
 */
#include "hxrt/dynamic.h"

static bool hxc_dynamic_category_storage_matches(
  hxc_dynamic_category category,
  hxc_dynamic_storage storage
) {
  switch (category) {
    case HXC_DYNAMIC_CATEGORY_NULL:
      return storage == HXC_DYNAMIC_STORAGE_INLINE_NULL;
    case HXC_DYNAMIC_CATEGORY_BOOL:
      return storage == HXC_DYNAMIC_STORAGE_INLINE_BOOL;
    case HXC_DYNAMIC_CATEGORY_INT:
      return storage == HXC_DYNAMIC_STORAGE_INLINE_INT32;
    case HXC_DYNAMIC_CATEGORY_FLOAT:
      return storage == HXC_DYNAMIC_STORAGE_INLINE_FLOAT64;
    case HXC_DYNAMIC_CATEGORY_STRING:
    case HXC_DYNAMIC_CATEGORY_ARRAY:
    case HXC_DYNAMIC_CATEGORY_ENUM:
    case HXC_DYNAMIC_CATEGORY_FUNCTION:
      return storage == HXC_DYNAMIC_STORAGE_MANAGED_WRAPPER;
    case HXC_DYNAMIC_CATEGORY_OBJECT:
      return storage == HXC_DYNAMIC_STORAGE_MANAGED_REFERENCE
        || storage == HXC_DYNAMIC_STORAGE_MANAGED_WRAPPER;
    case HXC_DYNAMIC_CATEGORY_TYPE_VALUE:
      return storage == HXC_DYNAMIC_STORAGE_STATIC_TOKEN;
    default:
      return false;
  }
}

static hxc_status hxc_value_init_pointer(
  const hxc_dynamic_type *type,
  hxc_dynamic_storage expected_storage,
  void *payload,
  hxc_value *out_value
) {
  hxc_value value = HXC_VALUE_INVALID_INITIALIZER;
  if (out_value == NULL
      || payload == NULL
      || !hxc_dynamic_type_is_valid(type)
      || type->storage != expected_storage) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  value.type = type;
  value.active_storage = expected_storage;
  value.payload.object = payload;
  *out_value = value;
  return HXC_STATUS_OK;
}

static hxc_status hxc_value_read_pointer(
  const hxc_value *value,
  hxc_dynamic_storage expected_storage,
  void **out_payload
) {
  if (out_payload == NULL
      || !hxc_value_is_valid(value)
      || value->active_storage != expected_storage) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  *out_payload = value->payload.object;
  return HXC_STATUS_OK;
}

bool hxc_dynamic_type_is_valid(const hxc_dynamic_type *type) {
  return type != NULL
    && type->abi_version == HXC_DYNAMIC_TYPE_ABI_VERSION
    && hxc_dynamic_category_storage_matches(type->category, type->storage);
}

bool hxc_value_is_valid(const hxc_value *value) {
  if (value == NULL
      || !hxc_dynamic_type_is_valid(value->type)
      || value->active_storage != value->type->storage) {
    return false;
  }
  switch (value->active_storage) {
    case HXC_DYNAMIC_STORAGE_INLINE_NULL:
    case HXC_DYNAMIC_STORAGE_INLINE_BOOL:
    case HXC_DYNAMIC_STORAGE_INLINE_INT32:
    case HXC_DYNAMIC_STORAGE_INLINE_FLOAT64:
      return true;
    case HXC_DYNAMIC_STORAGE_MANAGED_REFERENCE:
    case HXC_DYNAMIC_STORAGE_MANAGED_WRAPPER:
      return value->payload.object != NULL;
    case HXC_DYNAMIC_STORAGE_STATIC_TOKEN:
      return value->payload.static_token != NULL;
    default:
      return false;
  }
}

hxc_status hxc_value_init_null(
  const hxc_dynamic_type *type,
  hxc_value *out_value
) {
  hxc_value value = HXC_VALUE_INVALID_INITIALIZER;
  if (out_value == NULL
      || !hxc_dynamic_type_is_valid(type)
      || type->category != HXC_DYNAMIC_CATEGORY_NULL) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  value.type = type;
  *out_value = value;
  return HXC_STATUS_OK;
}

hxc_status hxc_value_init_bool(
  const hxc_dynamic_type *type,
  bool payload,
  hxc_value *out_value
) {
  hxc_value value = HXC_VALUE_INVALID_INITIALIZER;
  if (out_value == NULL
      || !hxc_dynamic_type_is_valid(type)
      || type->category != HXC_DYNAMIC_CATEGORY_BOOL) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  value.type = type;
  value.active_storage = HXC_DYNAMIC_STORAGE_INLINE_BOOL;
  value.payload.boolean = payload;
  *out_value = value;
  return HXC_STATUS_OK;
}

hxc_status hxc_value_init_int32(
  const hxc_dynamic_type *type,
  int32_t payload,
  hxc_value *out_value
) {
  hxc_value value = HXC_VALUE_INVALID_INITIALIZER;
  if (out_value == NULL
      || !hxc_dynamic_type_is_valid(type)
      || type->category != HXC_DYNAMIC_CATEGORY_INT) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  value.type = type;
  value.active_storage = HXC_DYNAMIC_STORAGE_INLINE_INT32;
  value.payload.int32_value = payload;
  *out_value = value;
  return HXC_STATUS_OK;
}

hxc_status hxc_value_init_float64(
  const hxc_dynamic_type *type,
  double payload,
  hxc_value *out_value
) {
  hxc_value value = HXC_VALUE_INVALID_INITIALIZER;
  if (out_value == NULL
      || !hxc_dynamic_type_is_valid(type)
      || type->category != HXC_DYNAMIC_CATEGORY_FLOAT) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  value.type = type;
  value.active_storage = HXC_DYNAMIC_STORAGE_INLINE_FLOAT64;
  value.payload.float64_value = payload;
  *out_value = value;
  return HXC_STATUS_OK;
}

hxc_status hxc_value_init_managed_reference(
  const hxc_dynamic_type *type,
  void *managed_object,
  hxc_value *out_value
) {
  return hxc_value_init_pointer(
    type,
    HXC_DYNAMIC_STORAGE_MANAGED_REFERENCE,
    managed_object,
    out_value
  );
}

hxc_status hxc_value_init_managed_wrapper(
  const hxc_dynamic_type *type,
  void *managed_wrapper,
  hxc_value *out_value
) {
  return hxc_value_init_pointer(
    type,
    HXC_DYNAMIC_STORAGE_MANAGED_WRAPPER,
    managed_wrapper,
    out_value
  );
}

hxc_status hxc_value_init_static_token(
  const hxc_dynamic_type *type,
  const void *static_token,
  hxc_value *out_value
) {
  hxc_value value = HXC_VALUE_INVALID_INITIALIZER;
  if (out_value == NULL
      || static_token == NULL
      || !hxc_dynamic_type_is_valid(type)
      || type->storage != HXC_DYNAMIC_STORAGE_STATIC_TOKEN) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  value.type = type;
  value.active_storage = HXC_DYNAMIC_STORAGE_STATIC_TOKEN;
  value.payload.static_token = static_token;
  *out_value = value;
  return HXC_STATUS_OK;
}

hxc_status hxc_value_is_null(
  const hxc_value *value,
  bool *out_is_null
) {
  if (out_is_null == NULL || !hxc_value_is_valid(value)) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  *out_is_null = value->type->category == HXC_DYNAMIC_CATEGORY_NULL;
  return HXC_STATUS_OK;
}

hxc_status hxc_value_read_bool(
  const hxc_value *value,
  bool *out_payload
) {
  if (out_payload == NULL
      || !hxc_value_is_valid(value)
      || value->active_storage != HXC_DYNAMIC_STORAGE_INLINE_BOOL) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  *out_payload = value->payload.boolean;
  return HXC_STATUS_OK;
}

hxc_status hxc_value_read_int32(
  const hxc_value *value,
  int32_t *out_payload
) {
  if (out_payload == NULL
      || !hxc_value_is_valid(value)
      || value->active_storage != HXC_DYNAMIC_STORAGE_INLINE_INT32) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  *out_payload = value->payload.int32_value;
  return HXC_STATUS_OK;
}

hxc_status hxc_value_read_float64(
  const hxc_value *value,
  double *out_payload
) {
  if (out_payload == NULL
      || !hxc_value_is_valid(value)
      || value->active_storage != HXC_DYNAMIC_STORAGE_INLINE_FLOAT64) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  *out_payload = value->payload.float64_value;
  return HXC_STATUS_OK;
}

hxc_status hxc_value_read_managed_reference(
  const hxc_value *value,
  void **out_managed_object
) {
  return hxc_value_read_pointer(
    value,
    HXC_DYNAMIC_STORAGE_MANAGED_REFERENCE,
    out_managed_object
  );
}

hxc_status hxc_value_read_managed_wrapper(
  const hxc_value *value,
  void **out_managed_wrapper
) {
  return hxc_value_read_pointer(
    value,
    HXC_DYNAMIC_STORAGE_MANAGED_WRAPPER,
    out_managed_wrapper
  );
}

hxc_status hxc_value_read_static_token(
  const hxc_value *value,
  const void **out_static_token
) {
  if (out_static_token == NULL
      || !hxc_value_is_valid(value)
      || value->active_storage != HXC_DYNAMIC_STORAGE_STATIC_TOKEN) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  *out_static_token = value->payload.static_token;
  return HXC_STATUS_OK;
}

hxc_status hxc_value_managed_payload(
  const hxc_value *value,
  const void **out_managed_object
) {
  if (out_managed_object == NULL || !hxc_value_is_valid(value)) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  switch (value->active_storage) {
    case HXC_DYNAMIC_STORAGE_MANAGED_REFERENCE:
    case HXC_DYNAMIC_STORAGE_MANAGED_WRAPPER:
      *out_managed_object = value->payload.object;
      return HXC_STATUS_OK;
    case HXC_DYNAMIC_STORAGE_INLINE_NULL:
    case HXC_DYNAMIC_STORAGE_INLINE_BOOL:
    case HXC_DYNAMIC_STORAGE_INLINE_INT32:
    case HXC_DYNAMIC_STORAGE_INLINE_FLOAT64:
    case HXC_DYNAMIC_STORAGE_STATIC_TOKEN:
      *out_managed_object = NULL;
      return HXC_STATUS_OK;
    default:
      return HXC_STATUS_INVALID_ARGUMENT;
  }
}
