#ifndef HXC_RUNTIME_H_INCLUDED
#define HXC_RUNTIME_H_INCLUDED

#include <limits.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#if defined(_WIN32) || defined(__CYGWIN__)
  #if defined(HXC_RUNTIME_BUILD_SHARED)
    #define HXC_API __declspec(dllexport)
  #elif defined(HXC_RUNTIME_USE_SHARED)
    #define HXC_API __declspec(dllimport)
  #else
    #define HXC_API
  #endif
#elif defined(__GNUC__) || defined(__clang__)
  #define HXC_API __attribute__((visibility("default")))
#else
  #define HXC_API
#endif

#if defined(__cplusplus)
  #define HXC_ALIGNOF(T) alignof(T)
#else
  #define HXC_ALIGNOF(T) _Alignof(T)
#endif

#define HXC_RUNTIME_ABI_MAJOR 0u
#define HXC_RUNTIME_ABI_MINOR 1u
#define HXC_RUNTIME_ABI_PATCH 0u
#define HXC_RUNTIME_ABI_VERSION \
  ((HXC_RUNTIME_ABI_MAJOR << 24u) | (HXC_RUNTIME_ABI_MINOR << 12u) | HXC_RUNTIME_ABI_PATCH)

#if defined(__cplusplus)
extern "C" {
#endif

typedef enum hxc_status {
  HXC_STATUS_OK = 0,
  HXC_STATUS_INVALID_ARGUMENT = 1,
  HXC_STATUS_OUT_OF_MEMORY = 2,
  HXC_STATUS_SIZE_OVERFLOW = 3,
  HXC_STATUS_UNSUPPORTED_ALIGNMENT = 4,
  HXC_STATUS_DIVISION_BY_ZERO = 5,
  HXC_STATUS_INTERNAL_ERROR = 255
} hxc_status;

typedef void *(*hxc_allocate_fn)(void *context, size_t size, size_t alignment);
typedef void *(*hxc_reallocate_fn)(
  void *context,
  void *memory,
  size_t old_size,
  size_t new_size,
  size_t alignment
);
typedef void (*hxc_release_fn)(void *context, void *memory, size_t size, size_t alignment);

typedef struct hxc_allocator {
  void *context;
  hxc_allocate_fn allocate;
  hxc_reallocate_fn reallocate;
  hxc_release_fn release;
} hxc_allocator;

typedef struct hxc_string_view {
  const uint8_t *data;
  size_t length;
} hxc_string_view;

typedef struct hxc_owned_string {
  uint8_t *data;
  size_t length;
  size_t capacity;
  hxc_allocator allocator;
} hxc_owned_string;

/** Runtime ABI version used by generated code and exported manifests. */
HXC_API uint32_t hxc_runtime_abi_version(void);

/** Stable symbolic status name; never returns null. */
HXC_API const char *hxc_status_name(hxc_status status);

/** Hosted default allocator. Callbacks are null in `HXC_FREESTANDING` builds. */
HXC_API hxc_allocator hxc_default_allocator(void);

HXC_API bool hxc_allocator_is_valid(const hxc_allocator *allocator);
HXC_API hxc_status hxc_alloc(
  const hxc_allocator *allocator,
  size_t size,
  size_t alignment,
  void **out_memory
);
HXC_API hxc_status hxc_realloc(
  const hxc_allocator *allocator,
  void *memory,
  size_t old_size,
  size_t new_size,
  size_t alignment,
  void **out_memory
);
HXC_API hxc_status hxc_free(
  const hxc_allocator *allocator,
  void *memory,
  size_t size,
  size_t alignment
);

HXC_API hxc_string_view hxc_string_view_from_cstr(const char *value);
HXC_API hxc_status hxc_string_copy(
  hxc_string_view source,
  const hxc_allocator *allocator,
  hxc_owned_string *out_string
);
HXC_API void hxc_owned_string_dispose(hxc_owned_string *value);

/** Convert modulo-2^32 bits to Haxe's signed 32-bit value without UB. */
static inline int32_t hxc_i32_from_u32(uint32_t value) {
  if (value <= (uint32_t)INT32_MAX) {
    return (int32_t)value;
  }
  return (int32_t)(-1 - (int32_t)(UINT32_MAX - value));
}

static inline int32_t hxc_i32_add_wrap(int32_t left, int32_t right) {
  return hxc_i32_from_u32((uint32_t)left + (uint32_t)right);
}

static inline int32_t hxc_i32_sub_wrap(int32_t left, int32_t right) {
  return hxc_i32_from_u32((uint32_t)left - (uint32_t)right);
}

static inline int32_t hxc_i32_mul_wrap(int32_t left, int32_t right) {
  return hxc_i32_from_u32((uint32_t)left * (uint32_t)right);
}

static inline int32_t hxc_i32_neg_wrap(int32_t value) {
  return hxc_i32_from_u32(UINT32_C(0) - (uint32_t)value);
}

static inline int32_t hxc_i32_shl_wrap(int32_t value, int32_t shift) {
  const uint32_t amount = (uint32_t)shift & UINT32_C(31);
  return hxc_i32_from_u32((uint32_t)value << amount);
}

static inline int32_t hxc_i32_shr_arithmetic(int32_t value, int32_t shift) {
  const uint32_t amount = (uint32_t)shift & UINT32_C(31);
  const uint32_t bits = (uint32_t)value;
  uint32_t shifted;

  if (amount == UINT32_C(0)) {
    return value;
  }
  shifted = bits >> amount;
  if (value < 0) {
    shifted |= UINT32_MAX << (UINT32_C(32) - amount);
  }
  return hxc_i32_from_u32(shifted);
}

static inline hxc_status hxc_i32_div(int32_t left, int32_t right, int32_t *out_value) {
  if (out_value == NULL) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  if (right == 0) {
    return HXC_STATUS_DIVISION_BY_ZERO;
  }
  if (left == INT32_MIN && right == -1) {
    *out_value = INT32_MIN;
    return HXC_STATUS_OK;
  }
  *out_value = left / right;
  return HXC_STATUS_OK;
}

static inline hxc_status hxc_i32_mod(int32_t left, int32_t right, int32_t *out_value) {
  if (out_value == NULL) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  if (right == 0) {
    return HXC_STATUS_DIVISION_BY_ZERO;
  }
  if (left == INT32_MIN && right == -1) {
    *out_value = 0;
    return HXC_STATUS_OK;
  }
  *out_value = left % right;
  return HXC_STATUS_OK;
}

#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif /* HXC_RUNTIME_H_INCLUDED */
