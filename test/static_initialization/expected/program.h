#ifndef HXC_PROGRAM_H_INCLUDED
#define HXC_PROGRAM_H_INCLUDED

#include <stdint.h>

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

extern int32_t hxc_AStaticInitDependent_first;

extern int32_t hxc_AStaticInitDependent_second;

extern int32_t hxc_AStaticInitDependent_value;

extern int32_t hxc_MStaticInitTrace_classOrder;

extern int32_t hxc_MStaticInitTrace_phaseValue;

extern int32_t hxc_MStaticInitTrace_trace;

extern int32_t hxc_ZStaticInitPrerequisite_value;

int32_t hxc_MStaticInitTrace_mark(int32_t hxc_digit);

int32_t hxc_MStaticInitTrace_seed(void);

void hxc_StaticInitializationFixture_main(void);

int32_t hxc_StaticInitializationFixture_outcome(void);

void hxc_init_compiler_static_initialization_AStaticInitDependent_class_init(void);

void hxc_init_compiler_static_initialization_AStaticInitDependent_static_field_first(void);

void hxc_init_compiler_static_initialization_AStaticInitDependent_static_field_second(void);

void hxc_init_compiler_static_initialization_AStaticInitDependent_static_field_value(void);

void hxc_init_compiler_static_initialization_MStaticInitTrace_class_init(void);

void hxc_init_compiler_static_initialization_MStaticInitTrace_static_field_classOrder(void);

void hxc_init_compiler_static_initialization_MStaticInitTrace_static_field_phaseValue(void);

void hxc_init_compiler_static_initialization_MStaticInitTrace_static_field_trace(void);

void hxc_init_compiler_static_initialization_ZStaticInitPrerequisite_class_init(void);

void hxc_init_compiler_static_initialization_ZStaticInitPrerequisite_static_field_value(void);

#endif /* HXC_PROGRAM_H_INCLUDED */
