#include "hxc/program.h"

_Static_assert(offsetof(struct hxc_BaseWorker, hxc_vtable) == 0, "class hxc_BaseWorker virtual-table pointer begins at offset zero");

_Static_assert(_Alignof(struct hxc_BaseWorker) >= _Alignof(const struct hxc_compiler_virtual_dispatch_BaseWorker_table_layout *), "class hxc_BaseWorker alignment admits its virtual-table pointer");

_Static_assert(offsetof(struct hxc_BaseWorker, hxc_seed) >= offsetof(struct hxc_BaseWorker, hxc_vtable) + sizeof(const struct hxc_compiler_virtual_dispatch_BaseWorker_table_layout *), "class hxc_BaseWorker field 0 follows the prior storage without overlap");

_Static_assert(_Alignof(struct hxc_BaseWorker) >= _Alignof(int32_t), "class hxc_BaseWorker alignment admits field 0");

_Static_assert(sizeof(struct hxc_BaseWorker) >= offsetof(struct hxc_BaseWorker, hxc_seed) + sizeof(int32_t), "class hxc_BaseWorker size contains its final storage member");

_Static_assert(offsetof(struct hxc_MiddleWorker, hxc_base) == 0, "class hxc_MiddleWorker base subobject begins at offset zero");

_Static_assert(_Alignof(struct hxc_MiddleWorker) >= _Alignof(struct hxc_BaseWorker), "class hxc_MiddleWorker alignment admits its base subobject");

_Static_assert(sizeof(struct hxc_MiddleWorker) >= sizeof(struct hxc_BaseWorker), "class hxc_MiddleWorker contains its complete base subobject");

_Static_assert(sizeof(struct hxc_MiddleWorker) >= offsetof(struct hxc_MiddleWorker, hxc_base) + sizeof(struct hxc_BaseWorker), "class hxc_MiddleWorker size contains its final storage member");

_Static_assert(offsetof(struct hxc_LeafWorker, hxc_base) == 0, "class hxc_LeafWorker base subobject begins at offset zero");

_Static_assert(_Alignof(struct hxc_LeafWorker) >= _Alignof(struct hxc_MiddleWorker), "class hxc_LeafWorker alignment admits its base subobject");

_Static_assert(sizeof(struct hxc_LeafWorker) >= sizeof(struct hxc_MiddleWorker), "class hxc_LeafWorker contains its complete base subobject");

_Static_assert(sizeof(struct hxc_LeafWorker) >= offsetof(struct hxc_LeafWorker, hxc_base) + sizeof(struct hxc_MiddleWorker), "class hxc_LeafWorker size contains its final storage member");

_Static_assert(offsetof(struct hxc_FinalWorker, hxc_seed) == 0, "class hxc_FinalWorker first storage field begins at offset zero");

_Static_assert(_Alignof(struct hxc_FinalWorker) >= _Alignof(int32_t), "class hxc_FinalWorker alignment admits field 0");

_Static_assert(sizeof(struct hxc_FinalWorker) >= offsetof(struct hxc_FinalWorker, hxc_seed) + sizeof(int32_t), "class hxc_FinalWorker size contains its final storage member");

static int32_t hxc_compiler_virtual_dispatch_thunk_slot_BaseWorker_value_method_LeafWorker_value(struct hxc_BaseWorker *hxc_receiver, int32_t hxc_argument_0);

static int32_t hxc_compiler_virtual_dispatch_thunk_slot_BaseWorker_value_method_MiddleWorker_value(struct hxc_BaseWorker *hxc_receiver, int32_t hxc_argument_0);

const struct hxc_compiler_virtual_dispatch_BaseWorker_table_layout hxc_vtable_compiler_virtual_dispatch_LeafWorker = { .hxc_slot_BaseWorker_value = hxc_compiler_virtual_dispatch_thunk_slot_BaseWorker_value_method_LeafWorker_value };

const struct hxc_compiler_virtual_dispatch_BaseWorker_table_layout hxc_vtable_compiler_virtual_dispatch_MiddleWorker = { .hxc_slot_BaseWorker_value = hxc_compiler_virtual_dispatch_thunk_slot_BaseWorker_value_method_MiddleWorker_value };

static int32_t hxc_compiler_virtual_dispatch_thunk_slot_BaseWorker_value_method_LeafWorker_value(struct hxc_BaseWorker *hxc_receiver, int32_t hxc_argument_0)
{
  return hxc_LeafWorker_value((struct hxc_LeafWorker *)hxc_receiver, hxc_argument_0);
}

static int32_t hxc_compiler_virtual_dispatch_thunk_slot_BaseWorker_value_method_MiddleWorker_value(struct hxc_BaseWorker *hxc_receiver, int32_t hxc_argument_0)
{
  return hxc_MiddleWorker_value((struct hxc_MiddleWorker *)hxc_receiver, hxc_argument_0);
}

void hxc_compiler_constructor_BaseWorker(struct hxc_BaseWorker *hxc_self, int32_t hxc_seed)
{
  (*hxc_self).hxc_seed = hxc_seed;
  return;
}

void hxc_compiler_constructor_FinalWorker(struct hxc_FinalWorker *hxc_self, int32_t hxc_seed)
{
  (*hxc_self).hxc_seed = hxc_seed;
  return;
}

void hxc_compiler_constructor_LeafWorker(struct hxc_LeafWorker *hxc_self, int32_t hxc_seed)
{
  hxc_compiler_constructor_MiddleWorker(&hxc_self->hxc_base, hxc_seed);
  return;
}

void hxc_compiler_constructor_MiddleWorker(struct hxc_MiddleWorker *hxc_self, int32_t hxc_seed)
{
  hxc_compiler_constructor_BaseWorker(&hxc_self->hxc_base, hxc_seed);
  return;
}

void hxc_Main_main(void)
{
  struct hxc_LeafWorker hxc_tmp_object_storage_n1 = { 0 };
  hxc_tmp_object_storage_n1.hxc_base.hxc_base.hxc_vtable = &hxc_vtable_compiler_virtual_dispatch_LeafWorker;
  struct hxc_LeafWorker *hxc_tmp_class_object_address_n0 = &hxc_tmp_object_storage_n1;
  hxc_compiler_constructor_LeafWorker(hxc_tmp_class_object_address_n0, 5);
  struct hxc_BaseWorker *hxc_leaf = &hxc_tmp_class_object_address_n0->hxc_base.hxc_base;
  struct hxc_MiddleWorker hxc_tmp_object_storage_n3 = { 0 };
  hxc_tmp_object_storage_n3.hxc_base.hxc_vtable = &hxc_vtable_compiler_virtual_dispatch_MiddleWorker;
  struct hxc_MiddleWorker *hxc_tmp_class_object_address_n1 = &hxc_tmp_object_storage_n3;
  hxc_compiler_constructor_MiddleWorker(hxc_tmp_class_object_address_n1, 2);
  struct hxc_BaseWorker *hxc_middle = &hxc_tmp_class_object_address_n1->hxc_base;
  struct hxc_FinalWorker hxc_tmp_object_storage_n5 = { 0 };
  struct hxc_FinalWorker *hxc_tmp_class_object_address_n2 = &hxc_tmp_object_storage_n5;
  hxc_compiler_constructor_FinalWorker(hxc_tmp_class_object_address_n2, 7);
  struct hxc_FinalWorker *hxc_exact = hxc_tmp_class_object_address_n2;
  struct hxc_LeafWorker hxc_tmp_object_storage_n7 = { 0 };
  hxc_tmp_object_storage_n7.hxc_base.hxc_base.hxc_vtable = &hxc_vtable_compiler_virtual_dispatch_LeafWorker;
  struct hxc_LeafWorker *hxc_tmp_class_object_address_n3 = &hxc_tmp_object_storage_n7;
  hxc_compiler_constructor_LeafWorker(hxc_tmp_class_object_address_n3, 1);
  struct hxc_LeafWorker *hxc_derived = hxc_tmp_class_object_address_n3;
  while (1)
  {
    struct hxc_BaseWorker *hxc_tmp_load_result_n4 = hxc_leaf;
    if (hxc_tmp_load_result_n4 == NULL)
    {
      abort();
    }
    int32_t hxc_tmp_instance_call_result_n5 = hxc_tmp_load_result_n4->hxc_vtable->hxc_slot_BaseWorker_value(hxc_tmp_load_result_n4, 1);
    bool hxc_tmp_short_circuit_result_n8 = hxc_tmp_instance_call_result_n5 == 116;
    if (hxc_tmp_instance_call_result_n5 == 116)
    {
      struct hxc_BaseWorker *hxc_tmp_load_result_n6 = hxc_middle;
      if (hxc_tmp_load_result_n6 == NULL)
      {
        abort();
      }
      int32_t hxc_tmp_instance_call_result_n7 = hxc_tmp_load_result_n6->hxc_vtable->hxc_slot_BaseWorker_value(hxc_tmp_load_result_n6, 3);
      hxc_tmp_short_circuit_result_n8 = hxc_tmp_instance_call_result_n7 == 15;
    }
    bool hxc_tmp_short_circuit_load_result_n8 = hxc_tmp_short_circuit_result_n8;
    bool hxc_tmp_short_circuit_result_n9 = hxc_tmp_short_circuit_load_result_n8;
    if (hxc_tmp_short_circuit_load_result_n8)
    {
      struct hxc_BaseWorker *hxc_tmp_load_result_n9 = hxc_leaf;
      if (hxc_tmp_load_result_n9 == NULL)
      {
        abort();
      }
      int32_t hxc_tmp_instance_call_result_n10 = hxc_BaseWorker_finalValue(hxc_tmp_load_result_n9, 2);
      hxc_tmp_short_circuit_result_n9 = hxc_tmp_instance_call_result_n10 == 8;
    }
    bool hxc_tmp_short_circuit_load_result_n11 = hxc_tmp_short_circuit_result_n9;
    bool hxc_tmp_short_circuit_result_n10 = hxc_tmp_short_circuit_load_result_n11;
    if (hxc_tmp_short_circuit_load_result_n11)
    {
      struct hxc_BaseWorker *hxc_tmp_load_result_n12 = hxc_leaf;
      if (hxc_tmp_load_result_n12 == NULL)
      {
        abort();
      }
      int32_t hxc_tmp_instance_call_result_n13 = hxc_BaseWorker_pinnedValue(hxc_tmp_load_result_n12, 3);
      hxc_tmp_short_circuit_result_n10 = hxc_tmp_instance_call_result_n13 == 10;
    }
    bool hxc_tmp_short_circuit_load_result_n14 = hxc_tmp_short_circuit_result_n10;
    bool hxc_tmp_short_circuit_result_n11 = hxc_tmp_short_circuit_load_result_n14;
    if (hxc_tmp_short_circuit_load_result_n14)
    {
      struct hxc_FinalWorker *hxc_tmp_load_result_n15 = hxc_exact;
      if (hxc_tmp_load_result_n15 == NULL)
      {
        abort();
      }
      int32_t hxc_tmp_instance_call_result_n16 = hxc_FinalWorker_value(hxc_tmp_load_result_n15, 3);
      hxc_tmp_short_circuit_result_n11 = hxc_tmp_instance_call_result_n16 == 10;
    }
    bool hxc_tmp_short_circuit_load_result_n17 = hxc_tmp_short_circuit_result_n11;
    bool hxc_tmp_short_circuit_result_n12 = hxc_tmp_short_circuit_load_result_n17;
    if (hxc_tmp_short_circuit_load_result_n17)
    {
      struct hxc_LeafWorker *hxc_tmp_load_result_n18 = hxc_derived;
      if ((hxc_tmp_load_result_n18 == NULL ? NULL : &hxc_tmp_load_result_n18->hxc_base.hxc_base) == NULL)
      {
        abort();
      }
      int32_t hxc_tmp_instance_call_result_n19 = (hxc_tmp_load_result_n18 == NULL ? NULL : &hxc_tmp_load_result_n18->hxc_base.hxc_base)->hxc_vtable->hxc_slot_BaseWorker_value(hxc_tmp_load_result_n18 == NULL ? NULL : &hxc_tmp_load_result_n18->hxc_base.hxc_base, 2);
      hxc_tmp_short_circuit_result_n12 = hxc_tmp_instance_call_result_n19 == 113;
    }
    if (!!hxc_tmp_short_circuit_result_n12)
    {
      break;
    }
  }
  return;
}

int32_t hxc_BaseWorker_finalValue(struct hxc_BaseWorker *hxc_self, int32_t hxc_delta)
{
  if (hxc_self == NULL)
  {
    abort();
  }
  int32_t hxc_tmp_instance_call_result_n0 = hxc_BaseWorker_privateValue(hxc_self, hxc_delta);
  return hxc_i32_add_wrapping(hxc_tmp_instance_call_result_n0, 1);
}

int32_t hxc_BaseWorker_pinnedValue(struct hxc_BaseWorker *hxc_self, int32_t hxc_delta)
{
  if (hxc_self == NULL)
  {
    abort();
  }
  int32_t hxc_tmp_class_field_load_result_n0 = (*hxc_self).hxc_seed;
  return hxc_i32_add_wrapping(hxc_i32_add_wrapping(hxc_tmp_class_field_load_result_n0, hxc_delta), 2);
}

int32_t hxc_BaseWorker_privateValue(struct hxc_BaseWorker *hxc_self, int32_t hxc_delta)
{
  if (hxc_self == NULL)
  {
    abort();
  }
  int32_t hxc_tmp_class_field_load_result_n0 = (*hxc_self).hxc_seed;
  return hxc_i32_add_wrapping(hxc_tmp_class_field_load_result_n0, hxc_delta);
}

int32_t hxc_BaseWorker_value(struct hxc_BaseWorker *hxc_self, int32_t hxc_delta)
{
  if (hxc_self == NULL)
  {
    abort();
  }
  int32_t hxc_tmp_class_field_load_result_n0 = (*hxc_self).hxc_seed;
  return hxc_i32_add_wrapping(hxc_tmp_class_field_load_result_n0, hxc_delta);
}

int32_t hxc_FinalWorker_value(struct hxc_FinalWorker *hxc_self, int32_t hxc_delta)
{
  if (hxc_self == NULL)
  {
    abort();
  }
  int32_t hxc_tmp_class_field_load_result_n0 = (*hxc_self).hxc_seed;
  return hxc_i32_add_wrapping(hxc_tmp_class_field_load_result_n0, hxc_delta);
}

int32_t hxc_LeafWorker_value(struct hxc_LeafWorker *hxc_self, int32_t hxc_delta)
{
  if ((hxc_self == NULL ? NULL : &hxc_self->hxc_base) == NULL)
  {
    abort();
  }
  int32_t hxc_tmp_instance_call_result_n0 = hxc_MiddleWorker_value(hxc_self == NULL ? NULL : &hxc_self->hxc_base, hxc_delta);
  return hxc_i32_add_wrapping(hxc_tmp_instance_call_result_n0, 100);
}

int32_t hxc_MiddleWorker_value(struct hxc_MiddleWorker *hxc_self, int32_t hxc_delta)
{
  if ((hxc_self == NULL ? NULL : &hxc_self->hxc_base) == NULL)
  {
    abort();
  }
  int32_t hxc_tmp_instance_call_result_n0 = hxc_BaseWorker_value(hxc_self == NULL ? NULL : &hxc_self->hxc_base, hxc_delta);
  return hxc_i32_add_wrapping(hxc_tmp_instance_call_result_n0, 10);
}

int main(void)
{
  hxc_Main_main();
  return 0;
}
