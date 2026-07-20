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

struct hxc_Option_ha0e4b5dcc139;

struct hxc_Option_h95f1c4a28dac;

struct hxc_Chain;

enum hxc_Option_tag_h4f842caea9db {
  hxc_Option_None_h00cd578bb80f = 0,
  hxc_Option_Some_h33493695ace2 = 1
};

struct hxc_Option_Some_payload_hc2b8e9073c59 {
  bool hxc_value;
};

union hxc_Option_payload_ha68af457b79c {
  struct hxc_Option_Some_payload_hc2b8e9073c59 hxc_Some;
};

struct hxc_Option_ha0e4b5dcc139 {
  enum hxc_Option_tag_h4f842caea9db hxc_tag;
  union hxc_Option_payload_ha68af457b79c hxc_payload;
};

enum hxc_Option_tag_h51b3904815c1 {
  hxc_Option_None_h506b5e6013bd = 0,
  hxc_Option_Some_ha9454146ff01 = 1
};

struct hxc_Option_Some_payload_h6fa8fca385dc {
  int32_t hxc_value;
};

union hxc_Option_payload_h331368fdb4fc {
  struct hxc_Option_Some_payload_h6fa8fca385dc hxc_Some;
};

struct hxc_Option_h95f1c4a28dac {
  enum hxc_Option_tag_h51b3904815c1 hxc_tag;
  union hxc_Option_payload_h331368fdb4fc hxc_payload;
};

enum hxc_Chain_tag {
  hxc_Chain_End = 0,
  hxc_Chain_Link = 1
};

struct hxc_Chain_End_payload {
  int32_t hxc_value;
};

struct hxc_Chain_Link_payload {
  int32_t hxc_value;
  struct hxc_Chain *hxc_next;
};

union hxc_Chain_payload {
  struct hxc_Chain_End_payload hxc_End;
  struct hxc_Chain_Link_payload hxc_Link;
};

struct hxc_Chain {
  enum hxc_Chain_tag hxc_tag;
  union hxc_Chain_payload hxc_payload;
};

enum hxc_Mode {
  hxc_Mode_Off = 0,
  hxc_Mode_On = 1
};

int32_t hxc_EnumFixture_boolOptionValue(struct hxc_Option_ha0e4b5dcc139 hxc_value_hb6e6538779c8);

int32_t hxc_EnumFixture_guardedValue(struct hxc_Option_h95f1c4a28dac hxc_value_ha201421511a7);

int32_t hxc_EnumFixture_identity(int32_t hxc_value);

void hxc_EnumFixture_main(void);

int32_t hxc_EnumFixture_modeValue(enum hxc_Mode hxc_value);

int32_t hxc_EnumFixture_optionValue(struct hxc_Option_h95f1c4a28dac hxc_value_h2c5c76013588);

int32_t hxc_EnumFixture_recursiveLocal(void);

#endif /* HXC_PROGRAM_H_INCLUDED */
