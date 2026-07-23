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

struct hxc_compiler_interface_dispatch_CounterView_table_layout;

struct hxc_compiler_interface_dispatch_CounterView_value;

struct hxc_compiler_interface_dispatch_ResetView_table_layout;

struct hxc_compiler_interface_dispatch_ResetView_value;

struct hxc_Counter;

struct hxc_Counter {
  int32_t hxc_base;
};

struct hxc_compiler_interface_dispatch_CounterView_table_layout {
  int32_t (*hxc_interface_slot_CounterView_doubled)(void *);
  int32_t (*hxc_interface_slot_CounterView_read)(void *, int32_t);
};

struct hxc_compiler_interface_dispatch_CounterView_value {
  void *object;
  const struct hxc_compiler_interface_dispatch_CounterView_table_layout *table;
};

struct hxc_compiler_interface_dispatch_ResetView_table_layout {
  int32_t (*hxc_interface_slot_ResetView_resetValue)(void *);
};

struct hxc_compiler_interface_dispatch_ResetView_value {
  void *object;
  const struct hxc_compiler_interface_dispatch_ResetView_table_layout *table;
};

extern const struct hxc_compiler_interface_dispatch_CounterView_table_layout hxc_itable_compiler_interface_dispatch_Counter_itable_layout_CounterView;

extern const struct hxc_compiler_interface_dispatch_ResetView_table_layout hxc_itable_compiler_interface_dispatch_Counter_itable_layout_ResetView;

void hxc_compiler_constructor_Counter(struct hxc_Counter *hxc_self, int32_t hxc_base);

void hxc_Main_main(void);

int32_t hxc_Counter_doubled(struct hxc_Counter *hxc_self);

int32_t hxc_Counter_read(struct hxc_Counter *hxc_self, int32_t hxc_delta);

int32_t hxc_Counter_resetValue(struct hxc_Counter *hxc_self);

#endif /* HXC_PROGRAM_H_INCLUDED */
