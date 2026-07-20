#include "hxc/program.h"

int32_t hxc_AStaticInitDependent_first;

int32_t hxc_AStaticInitDependent_second;

int32_t hxc_AStaticInitDependent_value;

int32_t hxc_MStaticInitTrace_classOrder;

int32_t hxc_MStaticInitTrace_phaseValue;

int32_t hxc_MStaticInitTrace_trace;

int32_t hxc_ZStaticInitPrerequisite_value;

int32_t hxc_MStaticInitTrace_mark(int32_t hxc_digit)
{
  int32_t hxc_tmp_global_load_result_n0 = hxc_MStaticInitTrace_trace;
  hxc_MStaticInitTrace_trace = hxc_i32_add_wrapping(hxc_i32_multiply_wrapping(hxc_tmp_global_load_result_n0, 10), hxc_digit);
  int32_t hxc_tmp_global_load_result_n1 = hxc_MStaticInitTrace_trace;
  return hxc_tmp_global_load_result_n1;
}

int32_t hxc_MStaticInitTrace_seed(void)
{
  return 1;
}

void hxc_StaticInitializationFixture_main(void)
{
  hxc_StaticInitializationFixture_outcome();
  return;
}

int32_t hxc_StaticInitializationFixture_outcome(void)
{
  int32_t hxc_tmp_global_load_result_n0 = hxc_MStaticInitTrace_classOrder;
  int32_t hxc_tmp_global_load_result_n1 = hxc_MStaticInitTrace_phaseValue;
  int32_t hxc_tmp_global_load_result_n2 = hxc_MStaticInitTrace_trace;
  int32_t hxc_tmp_global_load_result_n3 = hxc_ZStaticInitPrerequisite_value;
  int32_t hxc_tmp_global_load_result_n4 = hxc_AStaticInitDependent_first;
  int32_t hxc_tmp_global_load_result_n5 = hxc_AStaticInitDependent_second;
  int32_t hxc_tmp_global_load_result_n6 = hxc_AStaticInitDependent_value;
  return hxc_i32_add_wrapping(hxc_i32_add_wrapping(hxc_i32_add_wrapping(hxc_i32_add_wrapping(hxc_i32_add_wrapping(hxc_i32_add_wrapping(hxc_tmp_global_load_result_n0, hxc_tmp_global_load_result_n1), hxc_tmp_global_load_result_n2), hxc_tmp_global_load_result_n3), hxc_tmp_global_load_result_n4), hxc_tmp_global_load_result_n5), hxc_tmp_global_load_result_n6);
}

void hxc_init_compiler_static_initialization_AStaticInitDependent_class_init(void)
{
  int32_t hxc_tmp_global_load_result_n0 = hxc_MStaticInitTrace_classOrder;
  hxc_MStaticInitTrace_classOrder = hxc_i32_add_wrapping(hxc_i32_multiply_wrapping(hxc_tmp_global_load_result_n0, 10), 3);
  return;
}

void hxc_init_compiler_static_initialization_AStaticInitDependent_static_field_first(void)
{
  int32_t hxc_tmp_global_load_result_n0 = hxc_ZStaticInitPrerequisite_value;
  hxc_AStaticInitDependent_first = hxc_tmp_global_load_result_n0;
  return;
}

void hxc_init_compiler_static_initialization_AStaticInitDependent_static_field_second(void)
{
  int32_t hxc_tmp_global_load_result_n0 = hxc_AStaticInitDependent_first;
  hxc_AStaticInitDependent_second = hxc_i32_add_wrapping(hxc_tmp_global_load_result_n0, 5);
  return;
}

void hxc_init_compiler_static_initialization_AStaticInitDependent_static_field_value(void)
{
  int32_t hxc_tmp_call_result_n0 = hxc_MStaticInitTrace_mark(3);
  int32_t hxc_tmp_global_load_result_n1 = hxc_ZStaticInitPrerequisite_value;
  int32_t hxc_tmp_global_load_result_n2 = hxc_AStaticInitDependent_second;
  hxc_AStaticInitDependent_value = hxc_i32_add_wrapping(hxc_i32_add_wrapping(hxc_tmp_call_result_n0, hxc_tmp_global_load_result_n1), hxc_tmp_global_load_result_n2);
  return;
}

void hxc_init_compiler_static_initialization_MStaticInitTrace_class_init(void)
{
  hxc_MStaticInitTrace_phaseValue = 3;
  hxc_MStaticInitTrace_classOrder = 1;
  return;
}

void hxc_init_compiler_static_initialization_MStaticInitTrace_static_field_classOrder(void)
{
  int32_t hxc_tmp_global_load_result_n0 = hxc_MStaticInitTrace_classOrder;
  hxc_MStaticInitTrace_classOrder = hxc_tmp_global_load_result_n0;
  return;
}

void hxc_init_compiler_static_initialization_MStaticInitTrace_static_field_phaseValue(void)
{
  hxc_MStaticInitTrace_phaseValue = 7;
  return;
}

void hxc_init_compiler_static_initialization_MStaticInitTrace_static_field_trace(void)
{
  int32_t hxc_tmp_call_result_n0 = hxc_MStaticInitTrace_seed();
  hxc_MStaticInitTrace_trace = hxc_tmp_call_result_n0;
  return;
}

void hxc_init_compiler_static_initialization_ZStaticInitPrerequisite_class_init(void)
{
  int32_t hxc_tmp_global_load_result_n0 = hxc_MStaticInitTrace_classOrder;
  hxc_MStaticInitTrace_classOrder = hxc_i32_add_wrapping(hxc_i32_multiply_wrapping(hxc_tmp_global_load_result_n0, 10), 2);
  return;
}

void hxc_init_compiler_static_initialization_ZStaticInitPrerequisite_static_field_value(void)
{
  int32_t hxc_tmp_call_result_n0 = hxc_MStaticInitTrace_mark(2);
  hxc_ZStaticInitPrerequisite_value = hxc_tmp_call_result_n0;
  return;
}

static void hxc_init_compiler_static_initialization_hosted_executable_function_StaticInitializationFixture_main(void)
{
  hxc_init_compiler_static_initialization_MStaticInitTrace_class_init();
  hxc_init_compiler_static_initialization_ZStaticInitPrerequisite_class_init();
  hxc_init_compiler_static_initialization_AStaticInitDependent_class_init();
  hxc_init_compiler_static_initialization_MStaticInitTrace_static_field_classOrder();
  hxc_init_compiler_static_initialization_MStaticInitTrace_static_field_phaseValue();
  hxc_init_compiler_static_initialization_MStaticInitTrace_static_field_trace();
  hxc_init_compiler_static_initialization_ZStaticInitPrerequisite_static_field_value();
  hxc_init_compiler_static_initialization_AStaticInitDependent_static_field_first();
  hxc_init_compiler_static_initialization_AStaticInitDependent_static_field_second();
  hxc_init_compiler_static_initialization_AStaticInitDependent_static_field_value();
}

int main(void)
{
  hxc_init_compiler_static_initialization_hosted_executable_function_StaticInitializationFixture_main();
  hxc_StaticInitializationFixture_main();
  return 0;
}
