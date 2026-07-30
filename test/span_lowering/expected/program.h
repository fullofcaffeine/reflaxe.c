#ifndef HXC_PROGRAM_H_INCLUDED
#define HXC_PROGRAM_H_INCLUDED

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static inline int32_t hxc_u32_to_i32_bits(uint32_t hxc_l_value)
{
  if (hxc_l_value <= UINT32_C(2147483647))
  {
    return (int32_t)hxc_l_value;
  }
  return INT32_MIN + (int32_t)(hxc_l_value - UINT32_C(2147483648));
}

static inline int32_t hxc_i32_add_wrapping(int32_t hxc_l_left, int32_t hxc_l_right)
{
  return hxc_u32_to_i32_bits((uint32_t)((uint64_t)(uint32_t)hxc_l_left + (uint64_t)(uint32_t)hxc_l_right));
}

static inline int32_t hxc_i32_multiply_wrapping(int32_t hxc_l_left, int32_t hxc_l_right)
{
  return hxc_u32_to_i32_bits((uint32_t)((uint64_t)(uint32_t)hxc_l_left * (uint64_t)(uint32_t)hxc_l_right));
}

struct hxc_SpanFixture_OwnedSpanBuffer;

struct hxc_SpanFixture_FinalSpanMutator;

struct hxc_SpanFixture_ReturnedSpanOwner;

struct hxc_SpanFixture_ReturnedSpanOwner {
  uint8_t hxc_bytes[4];
};

struct hxc_SpanFixture_OwnedSpanBuffer {
  uint8_t hxc_bytes[4];
  int32_t hxc_numbers[4];
};

struct hxc_SpanFixture_FinalSpanMutator {
  unsigned char hxc_storage;
};

void hxc_compiler_constructor_SpanFixture_OwnedSpanBuffer(struct hxc_SpanFixture_OwnedSpanBuffer *hxc_l_self);

void hxc_compiler_constructor_SpanFixture_ReturnedSpanOwner(struct hxc_SpanFixture_ReturnedSpanOwner *hxc_l_self);

int32_t hxc_SpanFixture_checkedAt(int32_t hxc_l_index);

uint8_t hxc_SpanFixture_conditionalAssignment(bool hxc_l_selectReplacement);

int32_t hxc_SpanFixture_constSum(void);

uint8_t hxc_SpanFixture_finalClassParameterRoundTrip(uint8_t hxc_l_replacement);

uint8_t hxc_SpanFixture_forwardRead(const uint8_t *hxc_l_values, size_t hxc_l_length, int32_t hxc_l_index);

uint8_t hxc_SpanFixture_forwardReplace(uint8_t *hxc_l_values, size_t hxc_l_length, int32_t hxc_l_index, uint8_t hxc_l_replacement);

int32_t hxc_SpanFixture_linearIndex(int32_t hxc_l_x, int32_t hxc_l_y, int32_t hxc_l_z);

void hxc_SpanFixture_main(void);

int32_t hxc_SpanFixture_mutableSum(void);

uint8_t hxc_SpanFixture_mutatedGridCell(uint8_t hxc_l_replacement);

int32_t hxc_SpanFixture_ownedFieldRoundTrip(uint8_t hxc_l_replacement);

uint8_t hxc_SpanFixture_parameterRoundTrip(uint8_t hxc_l_replacement);

uint8_t hxc_SpanFixture_readAt(const uint8_t *hxc_l_values, size_t hxc_l_length, int32_t hxc_l_index);

uint8_t hxc_SpanFixture_replaceAt(uint8_t *hxc_l_values, size_t hxc_l_length, int32_t hxc_l_index, uint8_t hxc_l_replacement);

uint8_t hxc_SpanFixture_returnedSpanRoundTrip(void);

uint8_t hxc_SpanFixture_spanBeforeConditionalArgument(bool hxc_l_selectThird);

uint8_t hxc_SpanFixture_zeroedGridCell(void);

uint8_t hxc_SpanConditionalAssignment_assignSelected(uint8_t *hxc_l_values, size_t hxc_l_length, int32_t hxc_l_index, uint8_t hxc_l_whenTrue, uint8_t hxc_l_whenFalse, bool hxc_l_selectTrue);

uint8_t hxc_SpanFixture_FinalSpanMutator_replace(struct hxc_SpanFixture_FinalSpanMutator *hxc_l_self, uint8_t *hxc_l_values, size_t hxc_l_length, int32_t hxc_l_index, uint8_t hxc_l_replacement);

int32_t hxc_SpanFixture_OwnedSpanBuffer_roundTrip(struct hxc_SpanFixture_OwnedSpanBuffer *hxc_l_self, uint8_t hxc_l_replacement);

const uint8_t *hxc_SpanFixture_ReturnedSpanOwner_view(struct hxc_SpanFixture_ReturnedSpanOwner *hxc_l_self, size_t *hxc_l_returned_span_length);

#endif /* HXC_PROGRAM_H_INCLUDED */
