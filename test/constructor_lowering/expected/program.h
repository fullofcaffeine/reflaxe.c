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

struct hxc_BaseRecord;

struct hxc_LeafRecord;

struct hxc_FailableBase;

struct hxc_FailableLeaf;

struct hxc_EmptyBase;

struct hxc_EmptyLeaf;

struct hxc_BaseRecord {
  int32_t hxc_defaulted;
  int32_t hxc_baseField;
};

struct hxc_LeafRecord {
  struct hxc_BaseRecord hxc_base;
  int32_t hxc_firstLeafField;
  int32_t hxc_secondLeafField;
};

struct hxc_FailableBase {
  int32_t hxc_initializedBeforeFailure;
};

struct hxc_FailableLeaf {
  struct hxc_FailableBase hxc_base;
};

struct hxc_EmptyBase {
  unsigned char hxc_storage;
};

struct hxc_EmptyLeaf {
  struct hxc_EmptyBase hxc_base;
};

extern int32_t hxc_ConstructorTrace_value;

void hxc_compiler_constructor_BaseRecord(struct hxc_BaseRecord *hxc_self, int32_t hxc_value);

bool hxc_compiler_constructor_FailableBase(struct hxc_FailableBase *hxc_self, bool hxc_shouldFail);

bool hxc_compiler_constructor_FailableLeaf(struct hxc_FailableLeaf *hxc_self, bool hxc_shouldFail);

void hxc_compiler_constructor_LeafRecord(struct hxc_LeafRecord *hxc_self, int32_t hxc_value);

int32_t hxc_ConstructorTrace_mark(int32_t hxc_digit);

void hxc_Main_main(void);

void hxc_init_compiler_static_initialization_ConstructorTrace_static_field_value(void);

#endif /* HXC_PROGRAM_H_INCLUDED */
