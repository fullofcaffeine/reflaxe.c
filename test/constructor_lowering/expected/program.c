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

void hxc_compiler_constructor_BaseRecord(struct hxc_BaseRecord *hxc_self, int32_t hxc_value)
{
  int32_t hxc_tmp_call_result_n0 = hxc_ConstructorTrace_mark(4);
  (*hxc_self).hxc_baseField = hxc_tmp_call_result_n0;
  hxc_ConstructorTrace_mark(5);
  (*hxc_self).hxc_defaulted = hxc_value;
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

int32_t hxc_ConstructorTrace_mark(int32_t hxc_digit)
{
  int32_t hxc_tmp_global_load_result_n0 = hxc_ConstructorTrace_value;
  hxc_ConstructorTrace_value = hxc_i32_add_wrapping(hxc_i32_multiply_wrapping(hxc_tmp_global_load_result_n0, 10), hxc_digit);
  return hxc_digit;
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
  while (1)
  {
    int32_t hxc_tmp_global_load_result_n4 = hxc_ConstructorTrace_value;
    bool hxc_tmp_short_circuit_result_n6 = hxc_tmp_global_load_result_n4 == 1326457809;
    if (hxc_tmp_global_load_result_n4 == 1326457809)
    {
      struct hxc_LeafRecord *hxc_tmp_load_result_n5 = hxc_leaf;
      if (hxc_tmp_load_result_n5 == NULL)
      {
        abort();
      }
      int32_t hxc_tmp_class_field_load_result_n6 = (*hxc_tmp_load_result_n5).hxc_base.hxc_defaulted;
      hxc_tmp_short_circuit_result_n6 = hxc_tmp_class_field_load_result_n6 == 7;
    }
    bool hxc_tmp_short_circuit_load_result_n7 = hxc_tmp_short_circuit_result_n6;
    bool hxc_tmp_short_circuit_result_n7 = hxc_tmp_short_circuit_load_result_n7;
    if (hxc_tmp_short_circuit_load_result_n7)
    {
      struct hxc_LeafRecord *hxc_tmp_load_result_n8 = hxc_leaf;
      if (hxc_tmp_load_result_n8 == NULL)
      {
        abort();
      }
      int32_t hxc_tmp_class_field_load_result_n9 = (*hxc_tmp_load_result_n8).hxc_base.hxc_baseField;
      hxc_tmp_short_circuit_result_n7 = hxc_tmp_class_field_load_result_n9 == 4;
    }
    bool hxc_tmp_short_circuit_load_result_n10 = hxc_tmp_short_circuit_result_n7;
    bool hxc_tmp_short_circuit_result_n8 = hxc_tmp_short_circuit_load_result_n10;
    if (hxc_tmp_short_circuit_load_result_n10)
    {
      struct hxc_LeafRecord *hxc_tmp_load_result_n11 = hxc_leaf;
      if (hxc_tmp_load_result_n11 == NULL)
      {
        abort();
      }
      int32_t hxc_tmp_class_field_load_result_n12 = (*hxc_tmp_load_result_n11).hxc_firstLeafField;
      hxc_tmp_short_circuit_result_n8 = hxc_tmp_class_field_load_result_n12 == 2;
    }
    bool hxc_tmp_short_circuit_load_result_n13 = hxc_tmp_short_circuit_result_n8;
    bool hxc_tmp_short_circuit_result_n9 = hxc_tmp_short_circuit_load_result_n13;
    if (hxc_tmp_short_circuit_load_result_n13)
    {
      struct hxc_LeafRecord *hxc_tmp_load_result_n14 = hxc_leaf;
      if (hxc_tmp_load_result_n14 == NULL)
      {
        abort();
      }
      int32_t hxc_tmp_class_field_load_result_n15 = (*hxc_tmp_load_result_n14).hxc_secondLeafField;
      hxc_tmp_short_circuit_result_n9 = hxc_tmp_class_field_load_result_n15 == 3;
    }
    bool hxc_tmp_short_circuit_load_result_n16 = hxc_tmp_short_circuit_result_n9;
    bool hxc_tmp_short_circuit_result_n10 = hxc_tmp_short_circuit_load_result_n16;
    if (hxc_tmp_short_circuit_load_result_n16)
    {
      struct hxc_FailableLeaf *hxc_tmp_load_result_n17 = hxc_failable;
      if (hxc_tmp_load_result_n17 == NULL)
      {
        abort();
      }
      int32_t hxc_tmp_class_field_load_result_n18 = (*hxc_tmp_load_result_n17).hxc_base.hxc_initializedBeforeFailure;
      hxc_tmp_short_circuit_result_n10 = hxc_tmp_class_field_load_result_n18 == 8;
    }
    bool hxc_tmp_short_circuit_load_result_n19 = hxc_tmp_short_circuit_result_n10;
    bool hxc_tmp_short_circuit_result_n11 = hxc_tmp_short_circuit_load_result_n19;
    if (hxc_tmp_short_circuit_load_result_n19)
    {
      hxc_tmp_short_circuit_result_n11 = hxc_empty != NULL;
    }
    if (!!hxc_tmp_short_circuit_result_n11)
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
