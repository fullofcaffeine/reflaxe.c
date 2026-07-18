#include <float.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

_Static_assert(CHAR_BIT == 8, "hxc requires 8-bit bytes");
_Static_assert(sizeof(int8_t) * CHAR_BIT == 8, "int8_t width");
_Static_assert(sizeof(uint8_t) * CHAR_BIT == 8, "uint8_t width");
_Static_assert(sizeof(int16_t) * CHAR_BIT == 16, "int16_t width");
_Static_assert(sizeof(uint16_t) * CHAR_BIT == 16, "uint16_t width");
_Static_assert(sizeof(int32_t) * CHAR_BIT == 32, "int32_t width");
_Static_assert(sizeof(uint32_t) * CHAR_BIT == 32, "uint32_t width");
_Static_assert(sizeof(int64_t) * CHAR_BIT == 64, "int64_t width");
_Static_assert(sizeof(uint64_t) * CHAR_BIT == 64, "uint64_t width");
_Static_assert(INT8_MIN == (-127 - 1), "int8_t range");
_Static_assert(INT16_MIN == (-32767 - 1), "int16_t range");
_Static_assert(INT32_MIN == (-2147483647 - 1), "int32_t range");
_Static_assert(INT64_MIN == (-INT64_C(9223372036854775807) - INT64_C(1)), "int64_t range");
_Static_assert(UINT8_MAX == UINT8_C(255), "uint8_t range");
_Static_assert(UINT16_MAX == UINT16_C(65535), "uint16_t range");
_Static_assert(UINT32_MAX == UINT32_C(4294967295), "uint32_t range");
_Static_assert(UINT64_MAX == UINT64_C(18446744073709551615), "uint64_t range");
_Static_assert(FLT_RADIX == 2, "binary floating radix");
_Static_assert(sizeof(double) * CHAR_BIT == 64, "double storage width");
_Static_assert(DBL_MANT_DIG == 53, "binary64 significand");
_Static_assert(DBL_MAX_EXP == 1024, "binary64 maximum exponent");
_Static_assert(DBL_MIN_EXP == -1021, "binary64 minimum exponent");
_Static_assert(true == 1 && false == 0, "C bool value contract");
_Static_assert((size_t)-1 == SIZE_MAX, "size_t is unsigned");
_Static_assert(PTRDIFF_MIN < 0 && PTRDIFF_MAX > 0, "ptrdiff_t is signed");
_Static_assert(INTPTR_MIN < 0 && INTPTR_MAX > 0, "intptr_t is signed");
_Static_assert((uintptr_t)-1 == UINTPTR_MAX, "uintptr_t is unsigned");

typedef struct hxc_test_optional_i32 {
  bool has_value;
  int32_t value;
} hxc_test_optional_i32;

#define HXC_CHECK(condition) \
  do { \
    if (!(condition)) { \
      (void)fprintf(stderr, "primitive-semantics-native: check failed at line %d\n", __LINE__); \
      return 1; \
    } \
  } while (0)

static int32_t hxc_test_u32_to_i32_bits(uint32_t value) {
  if (value <= (uint32_t)INT32_MAX) {
    return (int32_t)value;
  }
  return (int32_t)(-1 - (int32_t)(UINT32_MAX - value));
}

static int8_t hxc_test_u8_to_i8_bits(uint8_t value) {
  if (value <= (uint8_t)INT8_MAX) {
    return (int8_t)value;
  }
  return (int8_t)(-1 - (int8_t)(UINT8_MAX - value));
}

static int8_t hxc_test_i32_to_i8_wrapping(int32_t value) {
  return hxc_test_u8_to_i8_bits((uint8_t)value);
}

static int32_t hxc_test_f64_to_i32_saturating(double value) {
  if (isnan(value)) {
    return INT32_C(0);
  }
  if (value >= (double)INT32_MAX) {
    return INT32_MAX;
  }
  if (value <= (double)INT32_MIN) {
    return INT32_MIN;
  }
  return (int32_t)value;
}

static hxc_test_optional_i32 hxc_test_optional_i32_present(int32_t value) {
  hxc_test_optional_i32 result;
  result.has_value = true;
  result.value = value;
  return result;
}

static hxc_test_optional_i32 hxc_test_optional_i32_absent(void) {
  hxc_test_optional_i32 result;
  result.has_value = false;
  result.value = INT32_C(0);
  return result;
}

int main(void) {
  const double nan_value = NAN;
  const double negative_zero = -0.0;
  const hxc_test_optional_i32 present = hxc_test_optional_i32_present(INT32_C(42));
  const hxc_test_optional_i32 absent = hxc_test_optional_i32_absent();
  int object = 7;
  void *pointer = &object;
  const intptr_t pointer_bits = (intptr_t)pointer;
  void *round_tripped_pointer = (void *)pointer_bits;

  HXC_CHECK((double)INT32_MIN == -2147483648.0);
  HXC_CHECK((double)INT32_MAX == 2147483647.0);
  HXC_CHECK((double)UINT32_MAX == 4294967295.0);
  HXC_CHECK(hxc_test_u32_to_i32_bits(UINT32_C(0xFFFFFFFF)) == INT32_C(-1));
  HXC_CHECK(hxc_test_u32_to_i32_bits(UINT32_C(0x80000000)) == INT32_MIN);
  HXC_CHECK((uint32_t)INT32_C(-1) == UINT32_MAX);
  HXC_CHECK((uint8_t)-INT32_C(1) == UINT8_MAX);
  HXC_CHECK((uint8_t)INT32_C(256) == (uint8_t)0);
  HXC_CHECK((int32_t)UINT8_MAX == INT32_C(255));
  HXC_CHECK(hxc_test_i32_to_i8_wrapping(INT32_C(130)) == INT8_C(-126));
  HXC_CHECK(hxc_test_i32_to_i8_wrapping(INT32_C(-129)) == INT8_C(127));

  HXC_CHECK(hxc_test_f64_to_i32_saturating(42.875) == INT32_C(42));
  HXC_CHECK(hxc_test_f64_to_i32_saturating(-42.875) == INT32_C(-42));
  HXC_CHECK(hxc_test_f64_to_i32_saturating(negative_zero) == INT32_C(0));
  HXC_CHECK(hxc_test_f64_to_i32_saturating(nan_value) == INT32_C(0));
  HXC_CHECK(hxc_test_f64_to_i32_saturating(INFINITY) == INT32_MAX);
  HXC_CHECK(hxc_test_f64_to_i32_saturating(-INFINITY) == INT32_MIN);
  HXC_CHECK(hxc_test_f64_to_i32_saturating(2147483648.0) == INT32_MAX);
  HXC_CHECK(hxc_test_f64_to_i32_saturating(-2147483649.0) == INT32_MIN);
  HXC_CHECK(isnan(nan_value));
  HXC_CHECK(!(nan_value == nan_value));
  HXC_CHECK(!(nan_value < 0.0) && !(nan_value > 0.0));
  HXC_CHECK(INFINITY > DBL_MAX && -INFINITY < -DBL_MAX);
  HXC_CHECK(negative_zero == 0.0 && signbit(negative_zero));

  HXC_CHECK(present.has_value && present.value == INT32_C(42));
  HXC_CHECK(!absent.has_value);
  HXC_CHECK(round_tripped_pointer == pointer);

  (void)puts("primitive-semantics-native: OK");
  return 0;
}
