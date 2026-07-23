#include "hxc/program.h"

_Static_assert(offsetof(struct hxc_Counter, hxc_base) == 0, "class hxc_Counter first storage field begins at offset zero");

_Static_assert(_Alignof(struct hxc_Counter) >= _Alignof(int32_t), "class hxc_Counter alignment admits field 0");

_Static_assert(sizeof(struct hxc_Counter) >= offsetof(struct hxc_Counter, hxc_base) + sizeof(int32_t), "class hxc_Counter size contains its final storage member");

static int32_t hxc_compiler_virtual_dispatch_thunk_interface_slot_CounterView_doubled_method_Counter_doubled(void *hxc_receiver);

static int32_t hxc_compiler_virtual_dispatch_thunk_interface_slot_CounterView_read_method_Counter_read(void *hxc_receiver, int32_t hxc_argument_0);

static int32_t hxc_compiler_virtual_dispatch_thunk_interface_slot_ResetView_resetValue_method_Counter_resetValue(void *hxc_receiver);

const struct hxc_compiler_interface_dispatch_CounterView_table_layout hxc_itable_compiler_interface_dispatch_Counter_itable_layout_CounterView = { .hxc_interface_slot_CounterView_doubled = hxc_compiler_virtual_dispatch_thunk_interface_slot_CounterView_doubled_method_Counter_doubled, .hxc_interface_slot_CounterView_read = hxc_compiler_virtual_dispatch_thunk_interface_slot_CounterView_read_method_Counter_read };

const struct hxc_compiler_interface_dispatch_ResetView_table_layout hxc_itable_compiler_interface_dispatch_Counter_itable_layout_ResetView = { .hxc_interface_slot_ResetView_resetValue = hxc_compiler_virtual_dispatch_thunk_interface_slot_ResetView_resetValue_method_Counter_resetValue };

static int32_t hxc_compiler_virtual_dispatch_thunk_interface_slot_CounterView_doubled_method_Counter_doubled(void *hxc_receiver)
{
  return hxc_Counter_doubled((struct hxc_Counter *)hxc_receiver);
}

static int32_t hxc_compiler_virtual_dispatch_thunk_interface_slot_CounterView_read_method_Counter_read(void *hxc_receiver, int32_t hxc_argument_0)
{
  return hxc_Counter_read((struct hxc_Counter *)hxc_receiver, hxc_argument_0);
}

static int32_t hxc_compiler_virtual_dispatch_thunk_interface_slot_ResetView_resetValue_method_Counter_resetValue(void *hxc_receiver)
{
  return hxc_Counter_resetValue((struct hxc_Counter *)hxc_receiver);
}

void hxc_compiler_constructor_Counter(struct hxc_Counter *hxc_self, int32_t hxc_base)
{
  (*hxc_self).hxc_base = hxc_base;
  return;
}

void hxc_Main_main(void)
{
  struct hxc_Counter hxc_tmp_object_storage_n1 = { 0 };
  struct hxc_Counter *hxc_tmp_class_object_address_n0 = &hxc_tmp_object_storage_n1;
  hxc_compiler_constructor_Counter(hxc_tmp_class_object_address_n0, 40);
  struct hxc_Counter *hxc_concrete = hxc_tmp_class_object_address_n0;
  struct hxc_Counter *hxc_tmp_load_result_n1 = hxc_concrete;
  if (hxc_tmp_load_result_n1 == NULL)
  {
    abort();
  }
  struct hxc_compiler_interface_dispatch_CounterView_value hxc_tmp_interface_value_n2 = (struct hxc_compiler_interface_dispatch_CounterView_value){ .object = hxc_tmp_load_result_n1, .table = &hxc_itable_compiler_interface_dispatch_Counter_itable_layout_CounterView };
  struct hxc_compiler_interface_dispatch_CounterView_value hxc_counter = hxc_tmp_interface_value_n2;
  struct hxc_Counter *hxc_tmp_load_result_n3 = hxc_concrete;
  if (hxc_tmp_load_result_n3 == NULL)
  {
    abort();
  }
  struct hxc_compiler_interface_dispatch_ResetView_value hxc_tmp_interface_value_n4 = (struct hxc_compiler_interface_dispatch_ResetView_value){ .object = hxc_tmp_load_result_n3, .table = &hxc_itable_compiler_interface_dispatch_Counter_itable_layout_ResetView };
  struct hxc_compiler_interface_dispatch_ResetView_value hxc_reset = hxc_tmp_interface_value_n4;
  while (1)
  {
    int32_t hxc_tmp_instance_call_result_n6 = hxc_counter.table->hxc_interface_slot_CounterView_read(hxc_counter.object, 2);
    bool hxc_tmp_short_circuit_result_n4 = hxc_tmp_instance_call_result_n6 != 42;
    if (!(hxc_tmp_instance_call_result_n6 != 42))
    {
      int32_t hxc_tmp_instance_call_result_n8 = hxc_counter.table->hxc_interface_slot_CounterView_doubled(hxc_counter.object);
      hxc_tmp_short_circuit_result_n4 = hxc_tmp_instance_call_result_n8 != 80;
    }
    bool hxc_tmp_short_circuit_load_result_n9 = hxc_tmp_short_circuit_result_n4;
    bool hxc_tmp_short_circuit_result_n5 = hxc_tmp_short_circuit_load_result_n9;
    if (!hxc_tmp_short_circuit_load_result_n9)
    {
      int32_t hxc_tmp_instance_call_result_n11 = hxc_reset.table->hxc_interface_slot_ResetView_resetValue(hxc_reset.object);
      hxc_tmp_short_circuit_result_n5 = hxc_tmp_instance_call_result_n11 != 0;
    }
    if (!hxc_tmp_short_circuit_result_n5)
    {
      break;
    }
  }
  return;
}

int32_t hxc_Counter_doubled(struct hxc_Counter *hxc_self)
{
  if (hxc_self == NULL)
  {
    abort();
  }
  int32_t hxc_tmp_class_field_load_result_n0 = (*hxc_self).hxc_base;
  return hxc_i32_multiply_wrapping(hxc_tmp_class_field_load_result_n0, 2);
}

int32_t hxc_Counter_read(struct hxc_Counter *hxc_self, int32_t hxc_delta)
{
  if (hxc_self == NULL)
  {
    abort();
  }
  int32_t hxc_tmp_class_field_load_result_n0 = (*hxc_self).hxc_base;
  return hxc_i32_add_wrapping(hxc_tmp_class_field_load_result_n0, hxc_delta);
}

int32_t hxc_Counter_resetValue(struct hxc_Counter *hxc_self)
{
  (void)hxc_self;
  return 0;
}

int main(void)
{
  hxc_Main_main();
  return 0;
}
