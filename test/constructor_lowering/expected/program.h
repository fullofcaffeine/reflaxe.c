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

struct hxc_compiler_virtual_dispatch_CounterOwner_table_layout;

struct hxc_compiler_virtual_dispatch_OwnedCounter_table_layout;

struct hxc_BaseRecord;

struct hxc_LeafRecord;

struct hxc_OwnedCounter;

struct hxc_CounterOwner;

struct hxc_FailableBase;

struct hxc_FailableLeaf;

struct hxc_EmptyBase;

struct hxc_EmptyLeaf;

struct hxc_compiler_virtual_dispatch_CounterOwner_table_layout {
  int32_t (*hxc_slot_CounterOwner_run)(struct hxc_CounterOwner *, int32_t);
};

struct hxc_compiler_virtual_dispatch_OwnedCounter_table_layout {
  int32_t (*hxc_slot_OwnedCounter_add)(struct hxc_OwnedCounter *, int32_t);
};

struct hxc_BaseRecord {
  int32_t hxc_defaulted;
  int32_t hxc_baseField;
};

struct hxc_EmptyBase {
  unsigned char hxc_storage;
};

struct hxc_FailableBase {
  int32_t hxc_initializedBeforeFailure;
};

struct hxc_FailableLeaf {
  struct hxc_FailableBase hxc_base;
};

struct hxc_EmptyLeaf {
  struct hxc_EmptyBase hxc_base;
};

struct hxc_OwnedCounter {
  const struct hxc_compiler_virtual_dispatch_OwnedCounter_table_layout *hxc_vtable;
  int32_t hxc_value;
};

struct hxc_CounterOwner {
  const struct hxc_compiler_virtual_dispatch_CounterOwner_table_layout *hxc_vtable;
  struct hxc_OwnedCounter hxc_child;
  int32_t hxc_observedDuringConstruction;
};

struct hxc_LeafRecord {
  struct hxc_BaseRecord hxc_base;
  int32_t hxc_firstLeafField;
  int32_t hxc_secondLeafField;
};

extern const struct hxc_compiler_virtual_dispatch_CounterOwner_table_layout hxc_vtable_compiler_virtual_dispatch_CounterOwner;

extern const struct hxc_compiler_virtual_dispatch_OwnedCounter_table_layout hxc_vtable_compiler_virtual_dispatch_OwnedCounter;

extern int32_t hxc_ConstructorTrace_value;

void hxc_compiler_constructor_BaseRecord(struct hxc_BaseRecord *hxc_self, int32_t hxc_value);

void hxc_compiler_constructor_CounterOwner(struct hxc_CounterOwner *hxc_self);

bool hxc_compiler_constructor_FailableBase(struct hxc_FailableBase *hxc_self, bool hxc_shouldFail);

bool hxc_compiler_constructor_FailableLeaf(struct hxc_FailableLeaf *hxc_self, bool hxc_shouldFail);

void hxc_compiler_constructor_LeafRecord(struct hxc_LeafRecord *hxc_self, int32_t hxc_value);

void hxc_compiler_constructor_OwnedCounter(struct hxc_OwnedCounter *hxc_self, int32_t hxc_seed);

int32_t hxc_ConstructorTrace_mark(int32_t hxc_digit);

int32_t hxc_Main_drive(struct hxc_CounterOwner *hxc_owner, int32_t hxc_delta);

void hxc_Main_main(void);

void hxc_init_compiler_static_initialization_ConstructorTrace_static_field_value(void);

int32_t hxc_CounterOwner_run(struct hxc_CounterOwner *hxc_self, int32_t hxc_delta);

int32_t hxc_OwnedCounter_add(struct hxc_OwnedCounter *hxc_self, int32_t hxc_delta);

#endif /* HXC_PROGRAM_H_INCLUDED */
