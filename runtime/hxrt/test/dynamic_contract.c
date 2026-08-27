#include "hxrt/dynamic.h"

#include <stdio.h>

typedef int32_t (*fixture_function)(int32_t value);

typedef struct fixture_object {
  int32_t value;
} fixture_object;

typedef struct fixture_function_wrapper {
  fixture_function function;
} fixture_function_wrapper;

static int32_t add_one(int32_t value) {
  return value + 1;
}

static const hxc_dynamic_type null_type = {
  HXC_DYNAMIC_TYPE_ABI_VERSION,
  UINT32_C(0),
  HXC_DYNAMIC_CATEGORY_NULL,
  HXC_DYNAMIC_STORAGE_INLINE_NULL,
};
static const hxc_dynamic_type bool_type = {
  HXC_DYNAMIC_TYPE_ABI_VERSION,
  UINT32_C(1),
  HXC_DYNAMIC_CATEGORY_BOOL,
  HXC_DYNAMIC_STORAGE_INLINE_BOOL,
};
static const hxc_dynamic_type int_type = {
  HXC_DYNAMIC_TYPE_ABI_VERSION,
  UINT32_C(2),
  HXC_DYNAMIC_CATEGORY_INT,
  HXC_DYNAMIC_STORAGE_INLINE_INT32,
};
static const hxc_dynamic_type float_type = {
  HXC_DYNAMIC_TYPE_ABI_VERSION,
  UINT32_C(3),
  HXC_DYNAMIC_CATEGORY_FLOAT,
  HXC_DYNAMIC_STORAGE_INLINE_FLOAT64,
};
static const hxc_dynamic_type object_type = {
  HXC_DYNAMIC_TYPE_ABI_VERSION,
  UINT32_C(4),
  HXC_DYNAMIC_CATEGORY_OBJECT,
  HXC_DYNAMIC_STORAGE_MANAGED_REFERENCE,
};
static const hxc_dynamic_type function_type = {
  HXC_DYNAMIC_TYPE_ABI_VERSION,
  UINT32_C(5),
  HXC_DYNAMIC_CATEGORY_FUNCTION,
  HXC_DYNAMIC_STORAGE_MANAGED_WRAPPER,
};
static const hxc_dynamic_type type_value_type = {
  HXC_DYNAMIC_TYPE_ABI_VERSION,
  UINT32_C(6),
  HXC_DYNAMIC_CATEGORY_TYPE_VALUE,
  HXC_DYNAMIC_STORAGE_STATIC_TOKEN,
};

static int check_descriptor_contract(void) {
  const hxc_dynamic_type wrong_version = {
    UINT32_C(99),
    UINT32_C(7),
    HXC_DYNAMIC_CATEGORY_BOOL,
    HXC_DYNAMIC_STORAGE_INLINE_BOOL,
  };
  const hxc_dynamic_type wrong_pair = {
    HXC_DYNAMIC_TYPE_ABI_VERSION,
    UINT32_C(8),
    HXC_DYNAMIC_CATEGORY_BOOL,
    HXC_DYNAMIC_STORAGE_INLINE_FLOAT64,
  };
  const hxc_dynamic_type wrong_category = {
    HXC_DYNAMIC_TYPE_ABI_VERSION,
    UINT32_C(9),
    (hxc_dynamic_category)99,
    HXC_DYNAMIC_STORAGE_INLINE_INT32,
  };
  const hxc_dynamic_type string_type = {
    HXC_DYNAMIC_TYPE_ABI_VERSION,
    UINT32_C(10),
    HXC_DYNAMIC_CATEGORY_STRING,
    HXC_DYNAMIC_STORAGE_MANAGED_WRAPPER,
  };
  const hxc_dynamic_type array_type = {
    HXC_DYNAMIC_TYPE_ABI_VERSION,
    UINT32_C(11),
    HXC_DYNAMIC_CATEGORY_ARRAY,
    HXC_DYNAMIC_STORAGE_MANAGED_WRAPPER,
  };
  const hxc_dynamic_type object_wrapper_type = {
    HXC_DYNAMIC_TYPE_ABI_VERSION,
    UINT32_C(12),
    HXC_DYNAMIC_CATEGORY_OBJECT,
    HXC_DYNAMIC_STORAGE_MANAGED_WRAPPER,
  };
  const hxc_dynamic_type enum_type = {
    HXC_DYNAMIC_TYPE_ABI_VERSION,
    UINT32_C(13),
    HXC_DYNAMIC_CATEGORY_ENUM,
    HXC_DYNAMIC_STORAGE_MANAGED_WRAPPER,
  };
  return hxc_dynamic_type_is_valid(&null_type)
    && hxc_dynamic_type_is_valid(&bool_type)
    && hxc_dynamic_type_is_valid(&int_type)
    && hxc_dynamic_type_is_valid(&float_type)
    && hxc_dynamic_type_is_valid(&object_type)
    && hxc_dynamic_type_is_valid(&function_type)
    && hxc_dynamic_type_is_valid(&type_value_type)
    && hxc_dynamic_type_is_valid(&string_type)
    && hxc_dynamic_type_is_valid(&array_type)
    && hxc_dynamic_type_is_valid(&object_wrapper_type)
    && hxc_dynamic_type_is_valid(&enum_type)
    && !hxc_dynamic_type_is_valid(NULL)
    && !hxc_dynamic_type_is_valid(&wrong_version)
    && !hxc_dynamic_type_is_valid(&wrong_pair)
    && !hxc_dynamic_type_is_valid(&wrong_category);
}

static int check_scalar_contract(void) {
  hxc_value null_value = HXC_VALUE_INVALID_INITIALIZER;
  hxc_value bool_value = HXC_VALUE_INVALID_INITIALIZER;
  hxc_value int_value = HXC_VALUE_INVALID_INITIALIZER;
  hxc_value float_value = HXC_VALUE_INVALID_INITIALIZER;
  bool is_null = false;
  bool boolean = false;
  int32_t integer = 0;
  double floating = 0.0;
  const void *managed = &int_type;

  if (hxc_value_init_null(&null_type, &null_value) != HXC_STATUS_OK
      || hxc_value_init_bool(&bool_type, true, &bool_value) != HXC_STATUS_OK
      || hxc_value_init_int32(&int_type, -INT32_C(2147483647), &int_value) != HXC_STATUS_OK
      || hxc_value_init_float64(&float_type, -12.5, &float_value) != HXC_STATUS_OK) {
    return 0;
  }
  if (!hxc_value_is_valid(&null_value)
      || !hxc_value_is_valid(&bool_value)
      || !hxc_value_is_valid(&int_value)
      || !hxc_value_is_valid(&float_value)) {
    return 0;
  }
  if (hxc_value_is_null(&null_value, &is_null) != HXC_STATUS_OK
      || !is_null
      || hxc_value_is_null(&bool_value, &is_null) != HXC_STATUS_OK
      || is_null) {
    return 0;
  }
  if (hxc_value_read_bool(&bool_value, &boolean) != HXC_STATUS_OK
      || !boolean
      || hxc_value_read_int32(&int_value, &integer) != HXC_STATUS_OK
      || integer != -INT32_C(2147483647)
      || hxc_value_read_float64(&float_value, &floating) != HXC_STATUS_OK
      || floating != -12.5) {
    return 0;
  }
  if (hxc_value_managed_payload(&null_value, &managed) != HXC_STATUS_OK
      || managed != NULL
      || hxc_value_managed_payload(&bool_value, &managed) != HXC_STATUS_OK
      || managed != NULL
      || hxc_value_managed_payload(&int_value, &managed) != HXC_STATUS_OK
      || managed != NULL
      || hxc_value_managed_payload(&float_value, &managed) != HXC_STATUS_OK
      || managed != NULL) {
    return 0;
  }
  return 1;
}

static int check_reference_contract(void) {
  fixture_object object = {INT32_C(41)};
  fixture_function_wrapper wrapper = {add_one};
  static const uint32_t type_token = UINT32_C(0xCAFE);
  hxc_value object_value = HXC_VALUE_INVALID_INITIALIZER;
  hxc_value function_value = HXC_VALUE_INVALID_INITIALIZER;
  hxc_value token_value = HXC_VALUE_INVALID_INITIALIZER;
  const void *payload = NULL;

  if (hxc_value_init_managed_reference(&object_type, &object, &object_value) != HXC_STATUS_OK
      || hxc_value_init_managed_wrapper(&function_type, &wrapper, &function_value) != HXC_STATUS_OK
      || hxc_value_init_static_token(&type_value_type, &type_token, &token_value) != HXC_STATUS_OK) {
    return 0;
  }
  if (hxc_value_read_managed_reference(&object_value, &payload) != HXC_STATUS_OK
      || payload != &object
      || ((const fixture_object *)payload)->value != INT32_C(41)) {
    return 0;
  }
  if (hxc_value_managed_payload(&object_value, &payload) != HXC_STATUS_OK
      || payload != &object) {
    return 0;
  }
  if (hxc_value_read_managed_wrapper(&function_value, &payload) != HXC_STATUS_OK
      || payload != &wrapper
      || ((const fixture_function_wrapper *)payload)->function(INT32_C(41)) != INT32_C(42)) {
    return 0;
  }
  if (hxc_value_managed_payload(&function_value, &payload) != HXC_STATUS_OK
      || payload != &wrapper) {
    return 0;
  }
  if (hxc_value_read_static_token(&token_value, &payload) != HXC_STATUS_OK
      || payload != &type_token
      || *(const uint32_t *)payload != UINT32_C(0xCAFE)) {
    return 0;
  }
  if (hxc_value_managed_payload(&token_value, &payload) != HXC_STATUS_OK
      || payload != NULL) {
    return 0;
  }
  return 1;
}

static int check_failure_contract(void) {
  hxc_value value = HXC_VALUE_INVALID_INITIALIZER;
  hxc_value malformed = HXC_VALUE_INVALID_INITIALIZER;
  bool boolean = true;
  int32_t integer = INT32_C(73);
  const void *pointer = &bool_type;

  if (hxc_value_init_int32(&int_type, INT32_C(19), &value) != HXC_STATUS_OK) {
    return 0;
  }
  if (hxc_value_init_bool(&int_type, true, &value) != HXC_STATUS_INVALID_ARGUMENT
      || hxc_value_read_int32(&value, &integer) != HXC_STATUS_OK
      || integer != INT32_C(19)) {
    return 0;
  }
  integer = INT32_C(73);
  if (hxc_value_read_int32(NULL, &integer) != HXC_STATUS_INVALID_ARGUMENT
      || integer != INT32_C(73)
      || hxc_value_read_int32(&value, NULL) != HXC_STATUS_INVALID_ARGUMENT
      || hxc_value_read_bool(&value, &boolean) != HXC_STATUS_INVALID_ARGUMENT
      || !boolean) {
    return 0;
  }
  malformed = value;
  malformed.active_storage = (hxc_dynamic_storage)99;
  if (hxc_value_is_valid(&malformed)
      || hxc_value_read_int32(&malformed, &integer) != HXC_STATUS_INVALID_ARGUMENT
      || integer != INT32_C(73)
      || hxc_value_managed_payload(&malformed, &pointer) != HXC_STATUS_INVALID_ARGUMENT
      || pointer != &bool_type) {
    return 0;
  }
  malformed = value;
  malformed.type = &bool_type;
  if (hxc_value_is_valid(&malformed)) {
    return 0;
  }
  if (hxc_value_init_managed_reference(&object_type, NULL, &value) != HXC_STATUS_INVALID_ARGUMENT
      || hxc_value_read_int32(&value, &integer) != HXC_STATUS_OK
      || integer != INT32_C(19)
      || hxc_value_init_managed_wrapper(&object_type, &value, &value) != HXC_STATUS_INVALID_ARGUMENT
      || hxc_value_read_int32(&value, &integer) != HXC_STATUS_OK
      || integer != INT32_C(19)) {
    return 0;
  }
  return 1;
}

int main(void) {
  if (!check_descriptor_contract()
      || !check_scalar_contract()
      || !check_reference_contract()
      || !check_failure_contract()) {
    return 1;
  }
  (void)puts("dynamic-runtime-contract: OK");
  return 0;
}
