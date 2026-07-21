#ifndef HXC_PROGRAM_H_INCLUDED
#define HXC_PROGRAM_H_INCLUDED

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static inline int32_t hxc_u32_to_i32_bits(uint32_t hxc_value)
{
  if (hxc_value <= UINT32_C(2147483647))
  {
    return (int32_t)hxc_value;
  }
  return INT32_MIN + (int32_t)(hxc_value - UINT32_C(2147483648));
}

static inline int32_t hxc_i32_add_wrapping(int32_t hxc_left, int32_t hxc_right)
{
  return hxc_u32_to_i32_bits((uint32_t)((uint64_t)(uint32_t)hxc_left + (uint64_t)(uint32_t)hxc_right));
}

static inline int32_t hxc_i32_multiply_wrapping(int32_t hxc_left, int32_t hxc_right)
{
  return hxc_u32_to_i32_bits((uint32_t)((uint64_t)(uint32_t)hxc_left * (uint64_t)(uint32_t)hxc_right));
}

int32_t hxc_SpanFixture_checkedAt(int32_t hxc_index);

int32_t hxc_SpanFixture_constSum(void);

uint8_t hxc_SpanFixture_forwardRead(const uint8_t *hxc_values, size_t hxc_length, int32_t hxc_index);

uint8_t hxc_SpanFixture_forwardReplace(uint8_t *hxc_values, size_t hxc_length, int32_t hxc_index, uint8_t hxc_replacement);

int32_t hxc_SpanFixture_linearIndex(int32_t hxc_x, int32_t hxc_y, int32_t hxc_z);

void hxc_SpanFixture_main(void);

int32_t hxc_SpanFixture_mutableSum(void);

uint8_t hxc_SpanFixture_mutatedGridCell(uint8_t hxc_replacement);

uint8_t hxc_SpanFixture_parameterRoundTrip(uint8_t hxc_replacement);

uint8_t hxc_SpanFixture_readAt(const uint8_t *hxc_values, size_t hxc_length, int32_t hxc_index);

uint8_t hxc_SpanFixture_replaceAt(uint8_t *hxc_values, size_t hxc_length, int32_t hxc_index, uint8_t hxc_replacement);

uint8_t hxc_SpanFixture_spanBeforeConditionalArgument(bool hxc_selectThird);

uint8_t hxc_SpanFixture_zeroedGridCell(void);

#endif /* HXC_PROGRAM_H_INCLUDED */
