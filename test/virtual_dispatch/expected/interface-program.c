#include "hxc/program.h"

_Static_assert(offsetof(struct hxc_AlternateCounter, hxc_base) == 0, "class hxc_AlternateCounter first storage field begins at offset zero");

_Static_assert(_Alignof(struct hxc_AlternateCounter) >= _Alignof(int32_t), "class hxc_AlternateCounter alignment admits field 0");

_Static_assert(sizeof(struct hxc_AlternateCounter) >= offsetof(struct hxc_AlternateCounter, hxc_base) + sizeof(int32_t), "class hxc_AlternateCounter size contains its final storage member");

_Static_assert(offsetof(struct hxc_Counter, hxc_base) == 0, "class hxc_Counter first storage field begins at offset zero");

_Static_assert(_Alignof(struct hxc_Counter) >= _Alignof(int32_t), "class hxc_Counter alignment admits field 0");

_Static_assert(sizeof(struct hxc_Counter) >= offsetof(struct hxc_Counter, hxc_base) + sizeof(int32_t), "class hxc_Counter size contains its final storage member");

static int32_t hxc_compiler_virtual_dispatch_thunk_interface_slot_CounterView_doubled_method_AlternateCounter_doubled(void *hxc_l_receiver);

static int32_t hxc_compiler_virtual_dispatch_thunk_interface_slot_CounterView_doubled_method_Counter_doubled(void *hxc_l_receiver);

static int32_t hxc_compiler_virtual_dispatch_thunk_interface_slot_CounterView_read_method_AlternateCounter_read(void *hxc_l_receiver, int32_t hxc_l_argument_0);

static int32_t hxc_compiler_virtual_dispatch_thunk_interface_slot_CounterView_read_method_Counter_read(void *hxc_l_receiver, int32_t hxc_l_argument_0);

static int32_t hxc_compiler_virtual_dispatch_thunk_interface_slot_ReadableView_read_method_AlternateCounter_read(void *hxc_l_receiver, int32_t hxc_l_argument_0);

static int32_t hxc_compiler_virtual_dispatch_thunk_interface_slot_ReadableView_read_method_Counter_read(void *hxc_l_receiver, int32_t hxc_l_argument_0);

static int32_t hxc_compiler_virtual_dispatch_thunk_interface_slot_ResetView_resetValue_method_Counter_resetValue(void *hxc_l_receiver);

const struct hxc_compiler_interface_dispatch_CounterView_table_layout hxc_itable_compiler_interface_dispatch_AlternateCounter_itable_layout_CounterView = { .hxc_interface_slot_CounterView_doubled = hxc_compiler_virtual_dispatch_thunk_interface_slot_CounterView_doubled_method_AlternateCounter_doubled, .hxc_interface_slot_CounterView_read = hxc_compiler_virtual_dispatch_thunk_interface_slot_CounterView_read_method_AlternateCounter_read };

const struct hxc_compiler_interface_dispatch_ReadableView_table_layout hxc_itable_compiler_interface_dispatch_AlternateCounter_itable_layout_ReadableView = { .hxc_interface_slot_ReadableView_read = hxc_compiler_virtual_dispatch_thunk_interface_slot_ReadableView_read_method_AlternateCounter_read };

const struct hxc_compiler_interface_dispatch_CounterView_table_layout hxc_itable_compiler_interface_dispatch_Counter_itable_layout_CounterView = { .hxc_interface_slot_CounterView_doubled = hxc_compiler_virtual_dispatch_thunk_interface_slot_CounterView_doubled_method_Counter_doubled, .hxc_interface_slot_CounterView_read = hxc_compiler_virtual_dispatch_thunk_interface_slot_CounterView_read_method_Counter_read };

const struct hxc_compiler_interface_dispatch_ReadableView_table_layout hxc_itable_compiler_interface_dispatch_Counter_itable_layout_ReadableView = { .hxc_interface_slot_ReadableView_read = hxc_compiler_virtual_dispatch_thunk_interface_slot_ReadableView_read_method_Counter_read };

const struct hxc_compiler_interface_dispatch_ResetView_table_layout hxc_itable_compiler_interface_dispatch_Counter_itable_layout_ResetView = { .hxc_interface_slot_ResetView_resetValue = hxc_compiler_virtual_dispatch_thunk_interface_slot_ResetView_resetValue_method_Counter_resetValue };

static int32_t hxc_compiler_virtual_dispatch_thunk_interface_slot_CounterView_doubled_method_AlternateCounter_doubled(void *hxc_l_receiver)
{
  return hxc_AlternateCounter_doubled((struct hxc_AlternateCounter *)hxc_l_receiver);
}

static int32_t hxc_compiler_virtual_dispatch_thunk_interface_slot_CounterView_doubled_method_Counter_doubled(void *hxc_l_receiver)
{
  return hxc_Counter_doubled((struct hxc_Counter *)hxc_l_receiver);
}

static int32_t hxc_compiler_virtual_dispatch_thunk_interface_slot_CounterView_read_method_AlternateCounter_read(void *hxc_l_receiver, int32_t hxc_l_argument_0)
{
  return hxc_AlternateCounter_read((struct hxc_AlternateCounter *)hxc_l_receiver, hxc_l_argument_0);
}

static int32_t hxc_compiler_virtual_dispatch_thunk_interface_slot_CounterView_read_method_Counter_read(void *hxc_l_receiver, int32_t hxc_l_argument_0)
{
  return hxc_Counter_read((struct hxc_Counter *)hxc_l_receiver, hxc_l_argument_0);
}

static int32_t hxc_compiler_virtual_dispatch_thunk_interface_slot_ReadableView_read_method_AlternateCounter_read(void *hxc_l_receiver, int32_t hxc_l_argument_0)
{
  return hxc_AlternateCounter_read((struct hxc_AlternateCounter *)hxc_l_receiver, hxc_l_argument_0);
}

static int32_t hxc_compiler_virtual_dispatch_thunk_interface_slot_ReadableView_read_method_Counter_read(void *hxc_l_receiver, int32_t hxc_l_argument_0)
{
  return hxc_Counter_read((struct hxc_Counter *)hxc_l_receiver, hxc_l_argument_0);
}

static int32_t hxc_compiler_virtual_dispatch_thunk_interface_slot_ResetView_resetValue_method_Counter_resetValue(void *hxc_l_receiver)
{
  return hxc_Counter_resetValue((struct hxc_Counter *)hxc_l_receiver);
}

void hxc_compiler_constructor_AlternateCounter(struct hxc_AlternateCounter *hxc_l_self, int32_t hxc_l_base)
{
  (*hxc_l_self).hxc_base = hxc_l_base;
  return;
}

void hxc_compiler_constructor_Counter(struct hxc_Counter *hxc_l_self, int32_t hxc_l_base)
{
  (*hxc_l_self).hxc_base = hxc_l_base;
  return;
}

int32_t hxc_Main_inspect(struct hxc_compiler_interface_dispatch_ReadableView_value hxc_l_readable)
{
  int32_t hxc_l_tmp_instance_call_result_n0 = hxc_l_readable.table->hxc_interface_slot_ReadableView_read(hxc_l_readable.object, 2);
  return hxc_l_tmp_instance_call_result_n0;
}

void hxc_Main_main(void)
{
  struct hxc_Counter hxc_l_tmp_object_storage_n1 = { 0 };
  struct hxc_Counter *hxc_l_tmp_class_object_address_n0 = &hxc_l_tmp_object_storage_n1;
  hxc_compiler_constructor_Counter(hxc_l_tmp_class_object_address_n0, 40);
  struct hxc_Counter *hxc_l_concrete = hxc_l_tmp_class_object_address_n0;
  struct hxc_Counter *hxc_l_tmp_load_result_n1 = hxc_l_concrete;
  if (hxc_l_tmp_load_result_n1 == NULL)
  {
    abort();
  }
  struct hxc_compiler_interface_dispatch_CounterView_value hxc_l_tmp_interface_value_n2 = (struct hxc_compiler_interface_dispatch_CounterView_value){ .object = hxc_l_tmp_load_result_n1, .table = &hxc_itable_compiler_interface_dispatch_Counter_itable_layout_CounterView };
  struct hxc_compiler_interface_dispatch_CounterView_value hxc_l_counter = hxc_l_tmp_interface_value_n2;
  struct hxc_AlternateCounter hxc_l_tmp_object_storage_n4 = { 0 };
  struct hxc_AlternateCounter *hxc_l_tmp_class_object_address_n3 = &hxc_l_tmp_object_storage_n4;
  hxc_compiler_constructor_AlternateCounter(hxc_l_tmp_class_object_address_n3, 50);
  struct hxc_AlternateCounter *hxc_l_alternateConcrete = hxc_l_tmp_class_object_address_n3;
  struct hxc_AlternateCounter *hxc_l_tmp_load_result_n4 = hxc_l_alternateConcrete;
  if (hxc_l_tmp_load_result_n4 == NULL)
  {
    abort();
  }
  struct hxc_compiler_interface_dispatch_CounterView_value hxc_l_tmp_interface_value_n5 = (struct hxc_compiler_interface_dispatch_CounterView_value){ .object = hxc_l_tmp_load_result_n4, .table = &hxc_itable_compiler_interface_dispatch_AlternateCounter_itable_layout_CounterView };
  struct hxc_compiler_interface_dispatch_CounterView_value hxc_l_alternate = hxc_l_tmp_interface_value_n5;
  struct hxc_Counter *hxc_l_tmp_load_result_n6 = hxc_l_concrete;
  if (hxc_l_tmp_load_result_n6 == NULL)
  {
    abort();
  }
  struct hxc_compiler_interface_dispatch_ResetView_value hxc_l_tmp_interface_value_n7 = (struct hxc_compiler_interface_dispatch_ResetView_value){ .object = hxc_l_tmp_load_result_n6, .table = &hxc_itable_compiler_interface_dispatch_Counter_itable_layout_ResetView };
  struct hxc_compiler_interface_dispatch_ResetView_value hxc_l_reset = hxc_l_tmp_interface_value_n7;
  while (1)
  {
    int32_t hxc_l_tmp_instance_call_result_n9 = hxc_l_counter.table->hxc_interface_slot_CounterView_read(hxc_l_counter.object, 2);
    bool hxc_l_tmp_short_circuit_result_n7 = hxc_l_tmp_instance_call_result_n9 != 42;
    if (!(hxc_l_tmp_instance_call_result_n9 != 42))
    {
      int32_t hxc_l_tmp_instance_call_result_n11 = hxc_l_counter.table->hxc_interface_slot_CounterView_doubled(hxc_l_counter.object);
      hxc_l_tmp_short_circuit_result_n7 = hxc_l_tmp_instance_call_result_n11 != 80;
    }
    bool hxc_l_tmp_short_circuit_load_result_n12 = hxc_l_tmp_short_circuit_result_n7;
    bool hxc_l_tmp_short_circuit_result_n8 = hxc_l_tmp_short_circuit_load_result_n12;
    if (!hxc_l_tmp_short_circuit_load_result_n12)
    {
      int32_t hxc_l_tmp_instance_call_result_n14 = hxc_l_counter.table->hxc_interface_slot_CounterView_read(hxc_l_counter.object, 1);
      hxc_l_tmp_short_circuit_result_n8 = hxc_l_tmp_instance_call_result_n14 != 41;
    }
    bool hxc_l_tmp_short_circuit_load_result_n15 = hxc_l_tmp_short_circuit_result_n8;
    bool hxc_l_tmp_short_circuit_result_n9 = hxc_l_tmp_short_circuit_load_result_n15;
    if (!hxc_l_tmp_short_circuit_load_result_n15)
    {
      int32_t hxc_l_tmp_instance_call_result_n17 = hxc_l_reset.table->hxc_interface_slot_ResetView_resetValue(hxc_l_reset.object);
      hxc_l_tmp_short_circuit_result_n9 = hxc_l_tmp_instance_call_result_n17 != 0;
    }
    bool hxc_l_tmp_short_circuit_load_result_n18 = hxc_l_tmp_short_circuit_result_n9;
    bool hxc_l_tmp_short_circuit_result_n10 = hxc_l_tmp_short_circuit_load_result_n18;
    if (!hxc_l_tmp_short_circuit_load_result_n18)
    {
      struct hxc_compiler_interface_dispatch_ReadableView_value hxc_l_tmp_interface_upcast_value_n20 = (struct hxc_compiler_interface_dispatch_ReadableView_value){ .object = hxc_l_counter.object, .table = hxc_l_counter.table == &hxc_itable_compiler_interface_dispatch_AlternateCounter_itable_layout_CounterView ? &hxc_itable_compiler_interface_dispatch_AlternateCounter_itable_layout_ReadableView : &hxc_itable_compiler_interface_dispatch_Counter_itable_layout_ReadableView };
      int32_t hxc_l_tmp_call_result_n21 = hxc_Main_inspect(hxc_l_tmp_interface_upcast_value_n20);
      hxc_l_tmp_short_circuit_result_n10 = hxc_l_tmp_call_result_n21 != 42;
    }
    bool hxc_l_tmp_short_circuit_load_result_n22 = hxc_l_tmp_short_circuit_result_n10;
    bool hxc_l_tmp_short_circuit_result_n11 = hxc_l_tmp_short_circuit_load_result_n22;
    if (!hxc_l_tmp_short_circuit_load_result_n22)
    {
      struct hxc_compiler_interface_dispatch_ReadableView_value hxc_l_tmp_interface_upcast_value_n24 = (struct hxc_compiler_interface_dispatch_ReadableView_value){ .object = hxc_l_alternate.object, .table = hxc_l_alternate.table == &hxc_itable_compiler_interface_dispatch_AlternateCounter_itable_layout_CounterView ? &hxc_itable_compiler_interface_dispatch_AlternateCounter_itable_layout_ReadableView : &hxc_itable_compiler_interface_dispatch_Counter_itable_layout_ReadableView };
      int32_t hxc_l_tmp_call_result_n25 = hxc_Main_inspect(hxc_l_tmp_interface_upcast_value_n24);
      hxc_l_tmp_short_circuit_result_n11 = hxc_l_tmp_call_result_n25 != 52;
    }
    if (!hxc_l_tmp_short_circuit_result_n11)
    {
      break;
    }
  }
  return;
}

int32_t hxc_AlternateCounter_doubled(struct hxc_AlternateCounter *hxc_l_self)
{
  if (hxc_l_self == NULL)
  {
    abort();
  }
  int32_t hxc_l_tmp_class_field_load_result_n0 = (*hxc_l_self).hxc_base;
  return hxc_i32_multiply_wrapping(hxc_l_tmp_class_field_load_result_n0, 2);
}

int32_t hxc_AlternateCounter_read(struct hxc_AlternateCounter *hxc_l_self, int32_t hxc_l_delta)
{
  if (hxc_l_self == NULL)
  {
    abort();
  }
  int32_t hxc_l_tmp_class_field_load_result_n0 = (*hxc_l_self).hxc_base;
  return hxc_i32_add_wrapping(hxc_l_tmp_class_field_load_result_n0, hxc_l_delta);
}

int32_t hxc_Counter_doubled(struct hxc_Counter *hxc_l_self)
{
  if (hxc_l_self == NULL)
  {
    abort();
  }
  int32_t hxc_l_tmp_class_field_load_result_n0 = (*hxc_l_self).hxc_base;
  return hxc_i32_multiply_wrapping(hxc_l_tmp_class_field_load_result_n0, 2);
}

int32_t hxc_Counter_read(struct hxc_Counter *hxc_l_self, int32_t hxc_l_delta)
{
  if (hxc_l_self == NULL)
  {
    abort();
  }
  int32_t hxc_l_tmp_class_field_load_result_n0 = (*hxc_l_self).hxc_base;
  return hxc_i32_add_wrapping(hxc_l_tmp_class_field_load_result_n0, hxc_l_delta);
}

int32_t hxc_Counter_resetValue(struct hxc_Counter *hxc_l_self)
{
  (void)hxc_l_self;
  return 0;
}

int main(void)
{
  hxc_Main_main();
  return 0;
}
