/*
 * Provisional native-smoke umbrella, not a compiler-selectable hxrt feature.
 *
 * Native fixtures include this file to exercise all checked-in seed slices and
 * legacy inline Int32 helpers together. Generated Haxe and selective packaging
 * never include it: they use direct C, program-local helpers, or exact feature
 * headers. The included runtime layouts are internal ABI and must not cross an
 * application export. This header owns no storage or thread state itself; the
 * included feature contracts define their own ownership and failure effects.
 */
#ifndef HXC_RUNTIME_H_INCLUDED
#define HXC_RUNTIME_H_INCLUDED

/* Provisional all-seed umbrella. Selective packaging uses hxrt feature headers. */

#include "hxrt/abi.h"
#include "hxrt/array.h"
#include "hxrt/bytes.h"
#include "hxrt/gc.h"
#include "hxrt/io.h"
#include "hxrt/object.h"
#include "hxrt/status_name.h"
#include "hxrt/string.h"

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
  /* Haxe masks Int shift counts; shifting unsigned bits avoids signed UB. */
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
    /* C does not promise an arithmetic right shift for negative signed Int. */
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
    /* Haxe wraps this quotient; evaluating the signed C division would be UB. */
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
    /* Avoid the same undefined signed division before producing remainder 0. */
    *out_value = 0;
    return HXC_STATUS_OK;
  }
  *out_value = left % right;
  return HXC_STATUS_OK;
}

#endif /* HXC_RUNTIME_H_INCLUDED */
