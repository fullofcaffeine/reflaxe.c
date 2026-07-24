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

struct hxc_compiler_virtual_dispatch_BaseWorker_table_layout;

struct hxc_BaseWorker;

struct hxc_MiddleWorker;

struct hxc_LeafWorker;

struct hxc_FinalWorker;

struct hxc_compiler_virtual_dispatch_BaseWorker_table_layout {
  int32_t (*hxc_slot_BaseWorker_value)(struct hxc_BaseWorker *, int32_t);
};

struct hxc_BaseWorker {
  const struct hxc_compiler_virtual_dispatch_BaseWorker_table_layout *hxc_vtable;
  int32_t hxc_seed;
};

struct hxc_MiddleWorker {
  struct hxc_BaseWorker hxc_base;
};

struct hxc_LeafWorker {
  struct hxc_MiddleWorker hxc_base;
};

struct hxc_FinalWorker {
  int32_t hxc_seed;
};

extern const struct hxc_compiler_virtual_dispatch_BaseWorker_table_layout hxc_vtable_compiler_virtual_dispatch_LeafWorker;

extern const struct hxc_compiler_virtual_dispatch_BaseWorker_table_layout hxc_vtable_compiler_virtual_dispatch_MiddleWorker;

void hxc_compiler_constructor_BaseWorker(struct hxc_BaseWorker *hxc_self, int32_t hxc_seed);

void hxc_compiler_constructor_FinalWorker(struct hxc_FinalWorker *hxc_self, int32_t hxc_seed);

void hxc_compiler_constructor_LeafWorker(struct hxc_LeafWorker *hxc_self, int32_t hxc_seed);

void hxc_compiler_constructor_MiddleWorker(struct hxc_MiddleWorker *hxc_self, int32_t hxc_seed);

void hxc_Main_main(void);

int32_t hxc_BaseWorker_finalValue(struct hxc_BaseWorker *hxc_self, int32_t hxc_delta);

int32_t hxc_BaseWorker_pinnedValue(struct hxc_BaseWorker *hxc_self, int32_t hxc_delta);

int32_t hxc_BaseWorker_privateValue(struct hxc_BaseWorker *hxc_self, int32_t hxc_delta);

int32_t hxc_BaseWorker_value(struct hxc_BaseWorker *hxc_self, int32_t hxc_delta);

int32_t hxc_FinalWorker_value(struct hxc_FinalWorker *hxc_self, int32_t hxc_delta);

int32_t hxc_LeafWorker_value(struct hxc_LeafWorker *hxc_self, int32_t hxc_delta);

int32_t hxc_MiddleWorker_value(struct hxc_MiddleWorker *hxc_self, int32_t hxc_delta);

#endif /* HXC_PROGRAM_H_INCLUDED */
