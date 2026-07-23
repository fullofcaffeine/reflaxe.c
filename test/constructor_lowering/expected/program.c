#include "hxc/program.h"

_Static_assert(offsetof(struct hxc_BaseRecord, hxc_defaulted) == 0, "class hxc_BaseRecord first storage field begins at offset zero");

_Static_assert(_Alignof(struct hxc_BaseRecord) >= _Alignof(int32_t), "class hxc_BaseRecord alignment admits field 0");

_Static_assert(offsetof(struct hxc_BaseRecord, hxc_baseField) >= offsetof(struct hxc_BaseRecord, hxc_defaulted) + sizeof(int32_t), "class hxc_BaseRecord field 1 follows the prior storage without overlap");

_Static_assert(_Alignof(struct hxc_BaseRecord) >= _Alignof(int32_t), "class hxc_BaseRecord alignment admits field 1");

_Static_assert(sizeof(struct hxc_BaseRecord) >= offsetof(struct hxc_BaseRecord, hxc_baseField) + sizeof(int32_t), "class hxc_BaseRecord size contains its final storage member");

_Static_assert(offsetof(struct hxc_LeafRecord, hxc_base) == 0, "class hxc_LeafRecord base subobject begins at offset zero");

_Static_assert(_Alignof(struct hxc_LeafRecord) >= _Alignof(struct hxc_BaseRecord), "class hxc_LeafRecord alignment admits its base subobject");

_Static_assert(sizeof(struct hxc_LeafRecord) >= sizeof(struct hxc_BaseRecord), "class hxc_LeafRecord contains its complete base subobject");

_Static_assert(offsetof(struct hxc_LeafRecord, hxc_firstLeafField) >= offsetof(struct hxc_LeafRecord, hxc_base) + sizeof(struct hxc_BaseRecord), "class hxc_LeafRecord field 0 follows the prior storage without overlap");

_Static_assert(_Alignof(struct hxc_LeafRecord) >= _Alignof(int32_t), "class hxc_LeafRecord alignment admits field 0");

_Static_assert(offsetof(struct hxc_LeafRecord, hxc_secondLeafField) >= offsetof(struct hxc_LeafRecord, hxc_firstLeafField) + sizeof(int32_t), "class hxc_LeafRecord field 1 follows the prior storage without overlap");

_Static_assert(_Alignof(struct hxc_LeafRecord) >= _Alignof(int32_t), "class hxc_LeafRecord alignment admits field 1");

_Static_assert(sizeof(struct hxc_LeafRecord) >= offsetof(struct hxc_LeafRecord, hxc_secondLeafField) + sizeof(int32_t), "class hxc_LeafRecord size contains its final storage member");

_Static_assert(offsetof(struct hxc_OwnedCounter, hxc_vtable) == 0, "class hxc_OwnedCounter virtual-table pointer begins at offset zero");

_Static_assert(_Alignof(struct hxc_OwnedCounter) >= _Alignof(const struct hxc_compiler_virtual_dispatch_OwnedCounter_table_layout *), "class hxc_OwnedCounter alignment admits its virtual-table pointer");

_Static_assert(offsetof(struct hxc_OwnedCounter, hxc_value) >= offsetof(struct hxc_OwnedCounter, hxc_vtable) + sizeof(const struct hxc_compiler_virtual_dispatch_OwnedCounter_table_layout *), "class hxc_OwnedCounter field 0 follows the prior storage without overlap");

_Static_assert(_Alignof(struct hxc_OwnedCounter) >= _Alignof(int32_t), "class hxc_OwnedCounter alignment admits field 0");

_Static_assert(sizeof(struct hxc_OwnedCounter) >= offsetof(struct hxc_OwnedCounter, hxc_value) + sizeof(int32_t), "class hxc_OwnedCounter size contains its final storage member");

_Static_assert(offsetof(struct hxc_CounterOwner, hxc_vtable) == 0, "class hxc_CounterOwner virtual-table pointer begins at offset zero");

_Static_assert(_Alignof(struct hxc_CounterOwner) >= _Alignof(const struct hxc_compiler_virtual_dispatch_CounterOwner_table_layout *), "class hxc_CounterOwner alignment admits its virtual-table pointer");

_Static_assert(offsetof(struct hxc_CounterOwner, hxc_child) >= offsetof(struct hxc_CounterOwner, hxc_vtable) + sizeof(const struct hxc_compiler_virtual_dispatch_CounterOwner_table_layout *), "class hxc_CounterOwner field 0 follows the prior storage without overlap");

_Static_assert(_Alignof(struct hxc_CounterOwner) >= _Alignof(struct hxc_OwnedCounter), "class hxc_CounterOwner alignment admits field 0");

_Static_assert(offsetof(struct hxc_CounterOwner, hxc_observedDuringConstruction) >= offsetof(struct hxc_CounterOwner, hxc_child) + sizeof(struct hxc_OwnedCounter), "class hxc_CounterOwner field 1 follows the prior storage without overlap");

_Static_assert(_Alignof(struct hxc_CounterOwner) >= _Alignof(int32_t), "class hxc_CounterOwner alignment admits field 1");

_Static_assert(sizeof(struct hxc_CounterOwner) >= offsetof(struct hxc_CounterOwner, hxc_observedDuringConstruction) + sizeof(int32_t), "class hxc_CounterOwner size contains its final storage member");

_Static_assert(offsetof(struct hxc_FailableBase, hxc_initializedBeforeFailure) == 0, "class hxc_FailableBase first storage field begins at offset zero");

_Static_assert(_Alignof(struct hxc_FailableBase) >= _Alignof(int32_t), "class hxc_FailableBase alignment admits field 0");

_Static_assert(sizeof(struct hxc_FailableBase) >= offsetof(struct hxc_FailableBase, hxc_initializedBeforeFailure) + sizeof(int32_t), "class hxc_FailableBase size contains its final storage member");

_Static_assert(offsetof(struct hxc_FailableLeaf, hxc_base) == 0, "class hxc_FailableLeaf base subobject begins at offset zero");

_Static_assert(_Alignof(struct hxc_FailableLeaf) >= _Alignof(struct hxc_FailableBase), "class hxc_FailableLeaf alignment admits its base subobject");

_Static_assert(sizeof(struct hxc_FailableLeaf) >= sizeof(struct hxc_FailableBase), "class hxc_FailableLeaf contains its complete base subobject");

_Static_assert(sizeof(struct hxc_FailableLeaf) >= offsetof(struct hxc_FailableLeaf, hxc_base) + sizeof(struct hxc_FailableBase), "class hxc_FailableLeaf size contains its final storage member");

_Static_assert(offsetof(struct hxc_EmptyBase, hxc_storage) == 0, "class hxc_EmptyBase strict-C empty-storage anchor begins at zero");

_Static_assert(sizeof(struct hxc_EmptyBase) >= 1, "class hxc_EmptyBase strict-C empty-storage anchor occupies one byte");

_Static_assert(offsetof(struct hxc_EmptyLeaf, hxc_base) == 0, "class hxc_EmptyLeaf base subobject begins at offset zero");

_Static_assert(_Alignof(struct hxc_EmptyLeaf) >= _Alignof(struct hxc_EmptyBase), "class hxc_EmptyLeaf alignment admits its base subobject");

_Static_assert(sizeof(struct hxc_EmptyLeaf) >= sizeof(struct hxc_EmptyBase), "class hxc_EmptyLeaf contains its complete base subobject");

_Static_assert(sizeof(struct hxc_EmptyLeaf) >= offsetof(struct hxc_EmptyLeaf, hxc_base) + sizeof(struct hxc_EmptyBase), "class hxc_EmptyLeaf size contains its final storage member");

int32_t hxc_ConstructorTrace_value;

const struct hxc_compiler_virtual_dispatch_CounterOwner_table_layout hxc_vtable_compiler_virtual_dispatch_CounterOwner = { .hxc_slot_CounterOwner_run = hxc_CounterOwner_run };

const struct hxc_compiler_virtual_dispatch_OwnedCounter_table_layout hxc_vtable_compiler_virtual_dispatch_OwnedCounter = { .hxc_slot_OwnedCounter_add = hxc_OwnedCounter_add };

void hxc_compiler_constructor_BaseRecord(struct hxc_BaseRecord *hxc_self, int32_t hxc_value)
{
  int32_t hxc_tmp_call_result_n0 = hxc_ConstructorTrace_mark(4);
  (*hxc_self).hxc_baseField = hxc_tmp_call_result_n0;
  hxc_ConstructorTrace_mark(5);
  (*hxc_self).hxc_defaulted = hxc_value;
  return;
}

void hxc_compiler_constructor_CounterOwner(struct hxc_CounterOwner *hxc_self)
{
  (*hxc_self).hxc_child.hxc_vtable = &hxc_vtable_compiler_virtual_dispatch_OwnedCounter;
  struct hxc_OwnedCounter *hxc_tmp_owned_class_field_address_n0 = &(*hxc_self).hxc_child;
  hxc_compiler_constructor_OwnedCounter(hxc_tmp_owned_class_field_address_n0, 11);
  struct hxc_OwnedCounter *hxc_tmp_owned_class_field_address_n1 = &(*hxc_self).hxc_child;
  if (hxc_tmp_owned_class_field_address_n1 == NULL)
  {
    abort();
  }
  int32_t hxc_tmp_instance_call_result_n2 = hxc_tmp_owned_class_field_address_n1->hxc_vtable->hxc_slot_OwnedCounter_add(hxc_tmp_owned_class_field_address_n1, 4);
  (*hxc_self).hxc_observedDuringConstruction = hxc_tmp_instance_call_result_n2;
  return;
}

bool hxc_compiler_constructor_FailableBase(struct hxc_FailableBase *hxc_self, bool hxc_shouldFail)
{
  int32_t hxc_tmp_call_result_n0 = hxc_ConstructorTrace_mark(8);
  (*hxc_self).hxc_initializedBeforeFailure = hxc_tmp_call_result_n0;
  struct hxc_EmptyLeaf hxc_tmp_object_storage_n3 = { 0 };
  struct hxc_EmptyLeaf *hxc_tmp_class_object_address_n1 = &hxc_tmp_object_storage_n3;
  struct hxc_EmptyLeaf *hxc_temporary = hxc_tmp_class_object_address_n1;
  bool hxc_tmp_short_circuit_result_n4 = hxc_shouldFail;
  if (hxc_shouldFail)
  {
    hxc_tmp_short_circuit_result_n4 = hxc_temporary != NULL;
  }
  if (!hxc_tmp_short_circuit_result_n4)
  {
    hxc_ConstructorTrace_mark(0);
    return true;
  }
  (void)99;
  return false;
}

bool hxc_compiler_constructor_FailableLeaf(struct hxc_FailableLeaf *hxc_self, bool hxc_shouldFail)
{
  if (!hxc_compiler_constructor_FailableBase(&hxc_self->hxc_base, hxc_shouldFail))
  {
    return false;
  }
  hxc_ConstructorTrace_mark(9);
  return true;
}

void hxc_compiler_constructor_LeafRecord(struct hxc_LeafRecord *hxc_self, int32_t hxc_value)
{
  int32_t hxc_tmp_call_result_n0 = hxc_ConstructorTrace_mark(3);
  (*hxc_self).hxc_secondLeafField = hxc_tmp_call_result_n0;
  int32_t hxc_tmp_call_result_n1 = hxc_ConstructorTrace_mark(2);
  (*hxc_self).hxc_firstLeafField = hxc_tmp_call_result_n1;
  int32_t hxc_tmp_call_result_n2 = hxc_ConstructorTrace_mark(6);
  hxc_compiler_constructor_BaseRecord(&hxc_self->hxc_base, hxc_i32_add_wrapping(hxc_tmp_call_result_n2, hxc_value));
  hxc_ConstructorTrace_mark(7);
  return;
}

void hxc_compiler_constructor_OwnedCounter(struct hxc_OwnedCounter *hxc_self, int32_t hxc_seed)
{
  (*hxc_self).hxc_value = hxc_seed;
  return;
}

int32_t hxc_ConstructorTrace_mark(int32_t hxc_digit)
{
  int32_t hxc_tmp_global_load_result_n0 = hxc_ConstructorTrace_value;
  hxc_ConstructorTrace_value = hxc_i32_add_wrapping(hxc_i32_multiply_wrapping(hxc_tmp_global_load_result_n0, 10), hxc_digit);
  return hxc_digit;
}

int32_t hxc_Main_drive(struct hxc_CounterOwner *hxc_owner, int32_t hxc_delta)
{
  if (hxc_owner == NULL)
  {
    abort();
  }
  int32_t hxc_tmp_instance_call_result_n0 = hxc_owner->hxc_vtable->hxc_slot_CounterOwner_run(hxc_owner, hxc_delta);
  return hxc_tmp_instance_call_result_n0;
}

void hxc_Main_main(void)
{
  hxc_ConstructorTrace_value = 0;
  int32_t hxc_tmp_call_result_n0 = hxc_ConstructorTrace_mark(1);
  struct hxc_LeafRecord hxc_tmp_object_storage_n1 = { 0 };
  struct hxc_LeafRecord *hxc_tmp_class_object_address_n1 = &hxc_tmp_object_storage_n1;
  hxc_compiler_constructor_LeafRecord(hxc_tmp_class_object_address_n1, hxc_tmp_call_result_n0);
  struct hxc_LeafRecord *hxc_leaf = hxc_tmp_class_object_address_n1;
  struct hxc_EmptyLeaf hxc_tmp_object_storage_n3 = { 0 };
  struct hxc_EmptyLeaf *hxc_tmp_class_object_address_n2 = &hxc_tmp_object_storage_n3;
  struct hxc_EmptyLeaf *hxc_empty = hxc_tmp_class_object_address_n2;
  struct hxc_FailableLeaf hxc_tmp_object_storage_n5 = { 0 };
  struct hxc_FailableLeaf *hxc_tmp_class_object_address_n3 = &hxc_tmp_object_storage_n5;
  if (!hxc_compiler_constructor_FailableLeaf(hxc_tmp_class_object_address_n3, false))
  {
    abort();
  }
  struct hxc_FailableLeaf *hxc_failable = hxc_tmp_class_object_address_n3;
  struct hxc_CounterOwner hxc_tmp_object_storage_n7 = { 0 };
  hxc_tmp_object_storage_n7.hxc_vtable = &hxc_vtable_compiler_virtual_dispatch_CounterOwner;
  struct hxc_CounterOwner *hxc_tmp_class_object_address_n4 = &hxc_tmp_object_storage_n7;
  hxc_compiler_constructor_CounterOwner(hxc_tmp_class_object_address_n4);
  struct hxc_CounterOwner *hxc_owner = hxc_tmp_class_object_address_n4;
  struct hxc_CounterOwner *hxc_ownerAlias = hxc_owner;
  int32_t hxc_tmp_call_result_n7 = hxc_Main_drive(hxc_ownerAlias, 2);
  int32_t hxc_ownedResult = hxc_tmp_call_result_n7;
  while (1)
  {
    int32_t hxc_tmp_global_load_result_n8 = hxc_ConstructorTrace_value;
    bool hxc_tmp_short_circuit_result_n10 = hxc_tmp_global_load_result_n8 == 1326457809;
    if (hxc_tmp_global_load_result_n8 == 1326457809)
    {
      struct hxc_LeafRecord *hxc_tmp_load_result_n9 = hxc_leaf;
      if (hxc_tmp_load_result_n9 == NULL)
      {
        abort();
      }
      int32_t hxc_tmp_class_field_load_result_n10 = (*hxc_tmp_load_result_n9).hxc_base.hxc_defaulted;
      hxc_tmp_short_circuit_result_n10 = hxc_tmp_class_field_load_result_n10 == 7;
    }
    bool hxc_tmp_short_circuit_load_result_n11 = hxc_tmp_short_circuit_result_n10;
    bool hxc_tmp_short_circuit_result_n11 = hxc_tmp_short_circuit_load_result_n11;
    if (hxc_tmp_short_circuit_load_result_n11)
    {
      struct hxc_LeafRecord *hxc_tmp_load_result_n12 = hxc_leaf;
      if (hxc_tmp_load_result_n12 == NULL)
      {
        abort();
      }
      int32_t hxc_tmp_class_field_load_result_n13 = (*hxc_tmp_load_result_n12).hxc_base.hxc_baseField;
      hxc_tmp_short_circuit_result_n11 = hxc_tmp_class_field_load_result_n13 == 4;
    }
    bool hxc_tmp_short_circuit_load_result_n14 = hxc_tmp_short_circuit_result_n11;
    bool hxc_tmp_short_circuit_result_n12 = hxc_tmp_short_circuit_load_result_n14;
    if (hxc_tmp_short_circuit_load_result_n14)
    {
      struct hxc_LeafRecord *hxc_tmp_load_result_n15 = hxc_leaf;
      if (hxc_tmp_load_result_n15 == NULL)
      {
        abort();
      }
      int32_t hxc_tmp_class_field_load_result_n16 = (*hxc_tmp_load_result_n15).hxc_firstLeafField;
      hxc_tmp_short_circuit_result_n12 = hxc_tmp_class_field_load_result_n16 == 2;
    }
    bool hxc_tmp_short_circuit_load_result_n17 = hxc_tmp_short_circuit_result_n12;
    bool hxc_tmp_short_circuit_result_n13 = hxc_tmp_short_circuit_load_result_n17;
    if (hxc_tmp_short_circuit_load_result_n17)
    {
      struct hxc_LeafRecord *hxc_tmp_load_result_n18 = hxc_leaf;
      if (hxc_tmp_load_result_n18 == NULL)
      {
        abort();
      }
      int32_t hxc_tmp_class_field_load_result_n19 = (*hxc_tmp_load_result_n18).hxc_secondLeafField;
      hxc_tmp_short_circuit_result_n13 = hxc_tmp_class_field_load_result_n19 == 3;
    }
    bool hxc_tmp_short_circuit_load_result_n20 = hxc_tmp_short_circuit_result_n13;
    bool hxc_tmp_short_circuit_result_n14 = hxc_tmp_short_circuit_load_result_n20;
    if (hxc_tmp_short_circuit_load_result_n20)
    {
      struct hxc_FailableLeaf *hxc_tmp_load_result_n21 = hxc_failable;
      if (hxc_tmp_load_result_n21 == NULL)
      {
        abort();
      }
      int32_t hxc_tmp_class_field_load_result_n22 = (*hxc_tmp_load_result_n21).hxc_base.hxc_initializedBeforeFailure;
      hxc_tmp_short_circuit_result_n14 = hxc_tmp_class_field_load_result_n22 == 8;
    }
    bool hxc_tmp_short_circuit_load_result_n23 = hxc_tmp_short_circuit_result_n14;
    bool hxc_tmp_short_circuit_result_n15 = hxc_tmp_short_circuit_load_result_n23;
    if (hxc_tmp_short_circuit_load_result_n23)
    {
      struct hxc_CounterOwner *hxc_tmp_load_result_n24 = hxc_ownerAlias;
      if (hxc_tmp_load_result_n24 == NULL)
      {
        abort();
      }
      int32_t hxc_tmp_class_field_load_result_n25 = (*hxc_tmp_load_result_n24).hxc_observedDuringConstruction;
      hxc_tmp_short_circuit_result_n15 = hxc_tmp_class_field_load_result_n25 == 15;
    }
    bool hxc_tmp_short_circuit_load_result_n26 = hxc_tmp_short_circuit_result_n15;
    bool hxc_tmp_short_circuit_result_n16 = hxc_tmp_short_circuit_load_result_n26;
    if (hxc_tmp_short_circuit_load_result_n26)
    {
      struct hxc_CounterOwner *hxc_tmp_load_result_n27 = hxc_ownerAlias;
      if (hxc_tmp_load_result_n27 == NULL)
      {
        abort();
      }
      struct hxc_OwnedCounter *hxc_tmp_owned_class_field_address_n28 = &(*hxc_tmp_load_result_n27).hxc_child;
      int32_t hxc_tmp_class_field_load_result_n29 = (*hxc_tmp_owned_class_field_address_n28).hxc_value;
      hxc_tmp_short_circuit_result_n16 = hxc_tmp_class_field_load_result_n29 == 17;
    }
    bool hxc_tmp_short_circuit_load_result_n30 = hxc_tmp_short_circuit_result_n16;
    bool hxc_tmp_short_circuit_result_n17 = hxc_tmp_short_circuit_load_result_n30;
    if (hxc_tmp_short_circuit_load_result_n30)
    {
      hxc_tmp_short_circuit_result_n17 = hxc_ownedResult == 32;
    }
    bool hxc_tmp_short_circuit_load_result_n32 = hxc_tmp_short_circuit_result_n17;
    bool hxc_tmp_short_circuit_result_n18 = hxc_tmp_short_circuit_load_result_n32;
    if (hxc_tmp_short_circuit_load_result_n32)
    {
      hxc_tmp_short_circuit_result_n18 = hxc_empty != NULL;
    }
    if (!!hxc_tmp_short_circuit_result_n18)
    {
      break;
    }
  }
  return;
}

void hxc_init_compiler_static_initialization_ConstructorTrace_static_field_value(void)
{
  hxc_ConstructorTrace_value = 0;
  return;
}

int32_t hxc_CounterOwner_run(struct hxc_CounterOwner *hxc_self, int32_t hxc_delta)
{
  if (hxc_self == NULL)
  {
    abort();
  }
  struct hxc_OwnedCounter *hxc_tmp_owned_class_field_address_n0 = &(*hxc_self).hxc_child;
  struct hxc_OwnedCounter *hxc_this_hfcd6ebfbb493 = hxc_tmp_owned_class_field_address_n0;
  struct hxc_OwnedCounter *hxc_tmp_load_result_n1 = hxc_this_hfcd6ebfbb493;
  if (hxc_tmp_load_result_n1 == NULL)
  {
    abort();
  }
  int32_t hxc_tmp_class_field_load_result_n2 = (*hxc_tmp_load_result_n1).hxc_value;
  bool hxc_tmp_short_circuit_result_n3 = hxc_tmp_class_field_load_result_n2 >= 0;
  if (hxc_tmp_class_field_load_result_n2 >= 0)
  {
    struct hxc_OwnedCounter *hxc_tmp_load_result_n3 = hxc_this_hfcd6ebfbb493;
    if (hxc_tmp_load_result_n3 == NULL)
    {
      abort();
    }
    int32_t hxc_tmp_class_field_load_result_n4 = (*hxc_tmp_load_result_n3).hxc_value;
    hxc_tmp_short_circuit_result_n3 = hxc_tmp_class_field_load_result_n4 == 15;
  }
  if (!!hxc_tmp_short_circuit_result_n3)
  {
    struct hxc_OwnedCounter *hxc_tmp_owned_class_field_address_n6 = &(*hxc_self).hxc_child;
    struct hxc_OwnedCounter *hxc_this_he0144d812e27 = hxc_tmp_owned_class_field_address_n6;
    struct hxc_OwnedCounter *hxc_tmp_load_result_n7 = hxc_this_he0144d812e27;
    if (hxc_tmp_load_result_n7 == NULL)
    {
      abort();
    }
    int32_t hxc_tmp_compound_load_result_n8 = (*hxc_tmp_load_result_n7).hxc_value;
    (*hxc_tmp_load_result_n7).hxc_value = hxc_i32_add_wrapping(hxc_tmp_compound_load_result_n8, hxc_delta);
    struct hxc_OwnedCounter *hxc_tmp_load_result_n9 = hxc_this_he0144d812e27;
    if (hxc_tmp_load_result_n9 == NULL)
    {
      abort();
    }
    int32_t hxc_tmp_class_field_load_result_n10 = (*hxc_tmp_load_result_n9).hxc_value;
    int32_t hxc_tmp_class_field_load_result_n11 = (*hxc_self).hxc_observedDuringConstruction;
    return hxc_i32_add_wrapping(hxc_tmp_class_field_load_result_n10, hxc_tmp_class_field_load_result_n11);
  }
  return -1;
}

int32_t hxc_OwnedCounter_add(struct hxc_OwnedCounter *hxc_self, int32_t hxc_delta)
{
  if (hxc_self == NULL)
  {
    abort();
  }
  int32_t hxc_tmp_compound_load_result_n0 = (*hxc_self).hxc_value;
  (*hxc_self).hxc_value = hxc_i32_add_wrapping(hxc_tmp_compound_load_result_n0, hxc_delta);
  int32_t hxc_tmp_class_field_load_result_n1 = (*hxc_self).hxc_value;
  return hxc_tmp_class_field_load_result_n1;
}

static void hxc_init_compiler_static_initialization_hosted_executable_function_Main_main(void)
{
  hxc_init_compiler_static_initialization_ConstructorTrace_static_field_value();
}

int main(void)
{
  hxc_init_compiler_static_initialization_hosted_executable_function_Main_main();
  hxc_Main_main();
  return 0;
}
