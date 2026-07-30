#include "hxc/program.h"

_Static_assert(offsetof(struct hxc_SpanFixture_OwnedSpanBuffer, hxc_bytes) == 0, "class hxc_SpanFixture_OwnedSpanBuffer first storage field begins at offset zero");

_Static_assert(_Alignof(struct hxc_SpanFixture_OwnedSpanBuffer) >= _Alignof(uint8_t [4]), "class hxc_SpanFixture_OwnedSpanBuffer alignment admits field 0");

_Static_assert(offsetof(struct hxc_SpanFixture_OwnedSpanBuffer, hxc_numbers) >= offsetof(struct hxc_SpanFixture_OwnedSpanBuffer, hxc_bytes) + sizeof(uint8_t [4]), "class hxc_SpanFixture_OwnedSpanBuffer field 1 follows the prior storage without overlap");

_Static_assert(_Alignof(struct hxc_SpanFixture_OwnedSpanBuffer) >= _Alignof(int32_t [4]), "class hxc_SpanFixture_OwnedSpanBuffer alignment admits field 1");

_Static_assert(sizeof(struct hxc_SpanFixture_OwnedSpanBuffer) >= offsetof(struct hxc_SpanFixture_OwnedSpanBuffer, hxc_numbers) + sizeof(int32_t [4]), "class hxc_SpanFixture_OwnedSpanBuffer size contains its final storage member");

_Static_assert(offsetof(struct hxc_SpanFixture_FinalSpanMutator, hxc_storage) == 0, "class hxc_SpanFixture_FinalSpanMutator strict-C empty-storage anchor begins at zero");

_Static_assert(sizeof(struct hxc_SpanFixture_FinalSpanMutator) >= 1, "class hxc_SpanFixture_FinalSpanMutator strict-C empty-storage anchor occupies one byte");

_Static_assert(offsetof(struct hxc_SpanFixture_ReturnedSpanOwner, hxc_bytes) == 0, "class hxc_SpanFixture_ReturnedSpanOwner first storage field begins at offset zero");

_Static_assert(_Alignof(struct hxc_SpanFixture_ReturnedSpanOwner) >= _Alignof(uint8_t [4]), "class hxc_SpanFixture_ReturnedSpanOwner alignment admits field 0");

_Static_assert(sizeof(struct hxc_SpanFixture_ReturnedSpanOwner) >= offsetof(struct hxc_SpanFixture_ReturnedSpanOwner, hxc_bytes) + sizeof(uint8_t [4]), "class hxc_SpanFixture_ReturnedSpanOwner size contains its final storage member");

void hxc_compiler_constructor_SpanFixture_OwnedSpanBuffer(struct hxc_SpanFixture_OwnedSpanBuffer *hxc_l_self)
{
  (void)hxc_l_self;
  return;
}

void hxc_compiler_constructor_SpanFixture_ReturnedSpanOwner(struct hxc_SpanFixture_ReturnedSpanOwner *hxc_l_self)
{
  (*hxc_l_self).hxc_bytes[(size_t)2] = (uint8_t)29;
  return;
}

int32_t hxc_SpanFixture_checkedAt(int32_t hxc_l_index)
{
  int32_t hxc_l_values[4] = { 1, 3, 8, 21 };
  const int32_t *hxc_l_view = hxc_l_values;
  size_t hxc_l_tmp_length_n2 = sizeof(hxc_l_values) / sizeof(hxc_l_values[0]);
  if (hxc_l_index < 0 || (size_t)hxc_l_index >= hxc_l_tmp_length_n2)
  {
    abort();
  }
  int32_t hxc_l_tmp_collection_index_load_result_n0 = hxc_l_view[(size_t)hxc_l_index];
  return hxc_l_tmp_collection_index_load_result_n0;
}

uint8_t hxc_SpanFixture_conditionalAssignment(bool hxc_l_selectReplacement)
{
  uint8_t hxc_l_values[4] = { 0 };
  uint8_t *hxc_l_mutable = hxc_l_values;
  size_t hxc_l_tmp_length_n2 = sizeof(hxc_l_values) / sizeof(hxc_l_values[0]);
  int32_t hxc_l_index = 1;
  hxc_SpanConditionalAssignment_assignSelected(hxc_l_mutable, hxc_l_tmp_length_n2, hxc_l_index, (uint8_t)37, (uint8_t)73, hxc_l_selectReplacement);
  uint8_t hxc_l_whenTrue = (uint8_t)37;
  uint8_t hxc_l_whenFalse = (uint8_t)73;
  int32_t hxc_l_tmp_load_result_n1 = hxc_l_index;
  if (hxc_l_tmp_load_result_n1 < 0 || (size_t)hxc_l_tmp_load_result_n1 >= hxc_l_tmp_length_n2)
  {
    abort();
  }
  uint8_t *hxc_l_tmp_address_n6 = &hxc_l_mutable[(size_t)hxc_l_tmp_load_result_n1];
  uint8_t hxc_l_tmp_conditional_result_n7 = 0;
  if (hxc_l_selectReplacement)
  {
    hxc_l_tmp_conditional_result_n7 = hxc_l_whenTrue;
  }
  else
  {
    hxc_l_tmp_conditional_result_n7 = hxc_l_whenFalse;
  }
  uint8_t hxc_l_tmp_conditional_load_result_n5 = hxc_l_tmp_conditional_result_n7;
  *hxc_l_tmp_address_n6 = hxc_l_tmp_conditional_load_result_n5;
  int32_t hxc_l_tmp_load_result_n7 = hxc_l_index;
  if (hxc_l_tmp_load_result_n7 < 0 || (size_t)hxc_l_tmp_load_result_n7 >= hxc_l_tmp_length_n2)
  {
    abort();
  }
  uint8_t hxc_l_tmp_collection_index_load_result_n8 = hxc_l_mutable[(size_t)hxc_l_tmp_load_result_n7];
  return hxc_l_tmp_collection_index_load_result_n8;
}

int32_t hxc_SpanFixture_constSum(void)
{
  int32_t hxc_l_values[4] = { 2, 4, 8, 16 };
  const int32_t *hxc_l_view = hxc_l_values;
  size_t hxc_l_tmp_length_n1 = sizeof(hxc_l_values) / sizeof(hxc_l_values[0]);
  (void)hxc_l_tmp_length_n1;
  int32_t hxc_l_tmp_collection_index_load_result_n0 = hxc_l_view[(size_t)0];
  int32_t hxc_l_total = hxc_l_tmp_collection_index_load_result_n0;
  size_t hxc_l_tmp_span_loop_index_n3 = 0;
  while (1)
  {
    if (!(hxc_l_tmp_span_loop_index_n3 < 4))
    {
      break;
    }
    size_t hxc_l_tmp_span_loop_body_index_result_n2 = hxc_l_tmp_span_loop_index_n3;
    int32_t hxc_l_tmp_span_loop_element_result_n3 = hxc_l_view[(size_t)hxc_l_tmp_span_loop_body_index_result_n2];
    hxc_l_total = hxc_i32_add_wrapping(hxc_l_total, hxc_l_tmp_span_loop_element_result_n3);
    hxc_l_tmp_span_loop_index_n3 = hxc_l_tmp_span_loop_index_n3 + 1;
  }
  return hxc_l_total;
}

uint8_t hxc_SpanFixture_finalClassParameterRoundTrip(uint8_t hxc_l_replacement)
{
  uint8_t hxc_l_values[4] = { 0 };
  uint8_t *hxc_l_view = hxc_l_values;
  size_t hxc_l_tmp_length_n2 = sizeof(hxc_l_values) / sizeof(hxc_l_values[0]);
  struct hxc_SpanFixture_FinalSpanMutator hxc_l_tmp_object_storage_n4 = { 0 };
  struct hxc_SpanFixture_FinalSpanMutator *hxc_l_tmp_class_object_address_n0 = &hxc_l_tmp_object_storage_n4;
  struct hxc_SpanFixture_FinalSpanMutator *hxc_l_mutator = hxc_l_tmp_class_object_address_n0;
  struct hxc_SpanFixture_FinalSpanMutator *hxc_l_tmp_load_result_n1 = hxc_l_mutator;
  if (hxc_l_tmp_load_result_n1 == NULL)
  {
    abort();
  }
  uint8_t hxc_l_tmp_instance_call_result_n2 = hxc_SpanFixture_FinalSpanMutator_replace(hxc_l_tmp_load_result_n1, hxc_l_view, hxc_l_tmp_length_n2, 2, hxc_l_replacement);
  return hxc_l_tmp_instance_call_result_n2;
}

uint8_t hxc_SpanFixture_forwardRead(const uint8_t *hxc_l_values, size_t hxc_l_length, int32_t hxc_l_index)
{
  const uint8_t *hxc_l_borrow = hxc_l_values;
  size_t hxc_l_tmp_length_n2 = hxc_l_length;
  (void)hxc_l_borrow;
  (void)hxc_l_tmp_length_n2;
  uint8_t hxc_l_tmp_call_result_n0 = hxc_SpanFixture_readAt(hxc_l_values, hxc_l_length, hxc_l_index);
  return hxc_l_tmp_call_result_n0;
}

uint8_t hxc_SpanFixture_forwardReplace(uint8_t *hxc_l_values, size_t hxc_l_length, int32_t hxc_l_index, uint8_t hxc_l_replacement)
{
  uint8_t *hxc_l_borrow = hxc_l_values;
  size_t hxc_l_tmp_length_n3 = hxc_l_length;
  (void)hxc_l_borrow;
  (void)hxc_l_tmp_length_n3;
  uint8_t hxc_l_tmp_call_result_n0 = hxc_SpanFixture_replaceAt(hxc_l_values, hxc_l_length, hxc_l_index, hxc_l_replacement);
  return hxc_l_tmp_call_result_n0;
}

int32_t hxc_SpanFixture_linearIndex(int32_t hxc_l_x, int32_t hxc_l_y, int32_t hxc_l_z)
{
  return hxc_i32_add_wrapping(hxc_l_x, hxc_i32_multiply_wrapping(32, hxc_i32_add_wrapping(hxc_l_y, hxc_i32_multiply_wrapping(16, hxc_l_z))));
}

void hxc_SpanFixture_main(void)
{
  hxc_SpanFixture_mutableSum();
  hxc_SpanFixture_constSum();
  hxc_SpanFixture_checkedAt(2);
  uint8_t hxc_l_tmp_call_result_n0 = hxc_SpanFixture_zeroedGridCell();
  hxc_SpanFixture_parameterRoundTrip(hxc_l_tmp_call_result_n0);
  hxc_SpanFixture_ownedFieldRoundTrip((uint8_t)1);
  hxc_SpanFixture_spanBeforeConditionalArgument(true);
  hxc_SpanFixture_zeroedGridCell();
  uint8_t hxc_l_tmp_call_result_n1 = hxc_SpanFixture_zeroedGridCell();
  hxc_SpanFixture_mutatedGridCell(hxc_l_tmp_call_result_n1);
  hxc_SpanFixture_finalClassParameterRoundTrip((uint8_t)201);
  hxc_SpanFixture_returnedSpanRoundTrip();
  hxc_SpanFixture_conditionalAssignment(true);
  hxc_SpanFixture_conditionalAssignment(false);
  return;
}

int32_t hxc_SpanFixture_mutableSum(void)
{
  int32_t hxc_l_values[4] = { 3, 5, 7, 11 };
  int32_t *hxc_l_view = hxc_l_values;
  size_t hxc_l_tmp_length_n1 = sizeof(hxc_l_values) / sizeof(hxc_l_values[0]);
  (void)hxc_l_tmp_length_n1;
  hxc_l_view[(size_t)1] = 13;
  int32_t hxc_l_total = 0;
  size_t hxc_l_tmp_span_loop_index_n3 = 0;
  while (1)
  {
    if (!(hxc_l_tmp_span_loop_index_n3 < 4))
    {
      break;
    }
    size_t hxc_l_tmp_span_loop_body_index_result_n1 = hxc_l_tmp_span_loop_index_n3;
    int32_t hxc_l_tmp_span_loop_element_result_n2 = hxc_l_view[(size_t)hxc_l_tmp_span_loop_body_index_result_n1];
    hxc_l_total = hxc_i32_add_wrapping(hxc_l_total, hxc_l_tmp_span_loop_element_result_n2);
    hxc_l_tmp_span_loop_index_n3 = hxc_l_tmp_span_loop_index_n3 + 1;
  }
  return hxc_l_total;
}

uint8_t hxc_SpanFixture_mutatedGridCell(uint8_t hxc_l_replacement)
{
  uint8_t hxc_l_values[16384] = { 0 };
  int32_t hxc_l_tmp_call_result_n0 = hxc_SpanFixture_linearIndex(31, 15, 31);
  int32_t hxc_l_index = hxc_l_tmp_call_result_n0;
  int32_t hxc_l_tmp_load_result_n1 = hxc_l_index;
  if (hxc_l_tmp_load_result_n1 < 0 || (size_t)hxc_l_tmp_load_result_n1 >= sizeof(hxc_l_values) / sizeof(hxc_l_values[0]))
  {
    abort();
  }
  hxc_l_values[(size_t)hxc_l_tmp_load_result_n1] = hxc_l_replacement;
  uint8_t *hxc_l_mutable = hxc_l_values;
  size_t hxc_l_tmp_length_n3 = sizeof(hxc_l_values) / sizeof(hxc_l_values[0]);
  int32_t hxc_l_tmp_load_result_n2 = hxc_l_index;
  if (hxc_l_tmp_load_result_n2 < 0 || (size_t)hxc_l_tmp_load_result_n2 >= hxc_l_tmp_length_n3)
  {
    abort();
  }
  hxc_l_mutable[(size_t)hxc_l_tmp_load_result_n2] = hxc_l_replacement;
  uint8_t hxc_l_tmp_collection_index_load_result_n3 = hxc_l_values[(size_t)0];
  uint8_t hxc_l_observed = hxc_l_tmp_collection_index_load_result_n3;
  size_t hxc_l_tmp_span_loop_index_n5 = 0;
  while (1)
  {
    if (!(hxc_l_tmp_span_loop_index_n5 < 16384))
    {
      break;
    }
    size_t hxc_l_tmp_span_loop_body_index_result_n5 = hxc_l_tmp_span_loop_index_n5;
    uint8_t hxc_l_tmp_span_loop_element_result_n6 = hxc_l_mutable[(size_t)hxc_l_tmp_span_loop_body_index_result_n5];
    hxc_l_observed = hxc_l_tmp_span_loop_element_result_n6;
    hxc_l_tmp_span_loop_index_n5 = hxc_l_tmp_span_loop_index_n5 + 1;
  }
  const uint8_t *hxc_l_readOnly = hxc_l_values;
  size_t hxc_l_tmp_length_n6 = sizeof(hxc_l_values) / sizeof(hxc_l_values[0]);
  int32_t hxc_l_tmp_load_result_n8 = hxc_l_index;
  if (hxc_l_tmp_load_result_n8 < 0 || (size_t)hxc_l_tmp_load_result_n8 >= hxc_l_tmp_length_n6)
  {
    abort();
  }
  uint8_t hxc_l_tmp_collection_index_load_result_n9 = hxc_l_readOnly[(size_t)hxc_l_tmp_load_result_n8];
  hxc_l_observed = hxc_l_tmp_collection_index_load_result_n9;
  return hxc_l_observed;
}

int32_t hxc_SpanFixture_ownedFieldRoundTrip(uint8_t hxc_l_replacement)
{
  struct hxc_SpanFixture_OwnedSpanBuffer hxc_l_tmp_object_storage_n2 = { 0 };
  struct hxc_SpanFixture_OwnedSpanBuffer *hxc_l_tmp_class_object_address_n0 = &hxc_l_tmp_object_storage_n2;
  hxc_compiler_constructor_SpanFixture_OwnedSpanBuffer(hxc_l_tmp_class_object_address_n0);
  struct hxc_SpanFixture_OwnedSpanBuffer *hxc_l_buffer = hxc_l_tmp_class_object_address_n0;
  struct hxc_SpanFixture_OwnedSpanBuffer *hxc_l_tmp_load_result_n1 = hxc_l_buffer;
  if (hxc_l_tmp_load_result_n1 == NULL)
  {
    abort();
  }
  int32_t hxc_l_tmp_instance_call_result_n2 = hxc_SpanFixture_OwnedSpanBuffer_roundTrip(hxc_l_tmp_load_result_n1, hxc_l_replacement);
  return hxc_l_tmp_instance_call_result_n2;
}

uint8_t hxc_SpanFixture_parameterRoundTrip(uint8_t hxc_l_replacement)
{
  uint8_t hxc_l_values[4] = { 0 };
  uint8_t *hxc_l_mutable = hxc_l_values;
  size_t hxc_l_tmp_length_n2 = sizeof(hxc_l_values) / sizeof(hxc_l_values[0]);
  hxc_SpanFixture_forwardReplace(hxc_l_mutable, hxc_l_tmp_length_n2, 2, hxc_l_replacement);
  const uint8_t *hxc_l_readOnly = hxc_l_values;
  size_t hxc_l_tmp_length_n3 = sizeof(hxc_l_values) / sizeof(hxc_l_values[0]);
  uint8_t hxc_l_tmp_call_result_n0 = hxc_SpanFixture_forwardRead(hxc_l_readOnly, hxc_l_tmp_length_n3, 2);
  return hxc_l_tmp_call_result_n0;
}

uint8_t hxc_SpanFixture_readAt(const uint8_t *hxc_l_values, size_t hxc_l_length, int32_t hxc_l_index)
{
  const uint8_t *hxc_l_borrow = hxc_l_values;
  size_t hxc_l_tmp_length_n2 = hxc_l_length;
  if (hxc_l_index < 0 || (size_t)hxc_l_index >= hxc_l_tmp_length_n2)
  {
    abort();
  }
  uint8_t hxc_l_tmp_collection_index_load_result_n0 = hxc_l_borrow[(size_t)hxc_l_index];
  return hxc_l_tmp_collection_index_load_result_n0;
}

uint8_t hxc_SpanFixture_replaceAt(uint8_t *hxc_l_values, size_t hxc_l_length, int32_t hxc_l_index, uint8_t hxc_l_replacement)
{
  uint8_t *hxc_l_borrow = hxc_l_values;
  size_t hxc_l_tmp_length_n3 = hxc_l_length;
  if (hxc_l_index < 0 || (size_t)hxc_l_index >= hxc_l_tmp_length_n3)
  {
    abort();
  }
  hxc_l_borrow[(size_t)hxc_l_index] = hxc_l_replacement;
  if (hxc_l_index < 0 || (size_t)hxc_l_index >= hxc_l_tmp_length_n3)
  {
    abort();
  }
  uint8_t hxc_l_tmp_collection_index_load_result_n0 = hxc_l_borrow[(size_t)hxc_l_index];
  return hxc_l_tmp_collection_index_load_result_n0;
}

uint8_t hxc_SpanFixture_returnedSpanRoundTrip(void)
{
  struct hxc_SpanFixture_ReturnedSpanOwner hxc_l_tmp_object_storage_n1 = { 0 };
  struct hxc_SpanFixture_ReturnedSpanOwner *hxc_l_tmp_class_object_address_n0 = &hxc_l_tmp_object_storage_n1;
  hxc_compiler_constructor_SpanFixture_ReturnedSpanOwner(hxc_l_tmp_class_object_address_n0);
  struct hxc_SpanFixture_ReturnedSpanOwner *hxc_l_owner = hxc_l_tmp_class_object_address_n0;
  struct hxc_SpanFixture_ReturnedSpanOwner *hxc_l_tmp_load_result_n1 = hxc_l_owner;
  if (hxc_l_tmp_load_result_n1 == NULL)
  {
    abort();
  }
  size_t hxc_l_tmp_length_n3;
  const uint8_t *hxc_l_tmp_instance_call_result_n2 = hxc_SpanFixture_ReturnedSpanOwner_view(hxc_l_tmp_load_result_n1, &hxc_l_tmp_length_n3);
  uint8_t hxc_l_tmp_call_result_n4 = hxc_SpanFixture_readAt(hxc_l_tmp_instance_call_result_n2, hxc_l_tmp_length_n3, 2);
  return hxc_l_tmp_call_result_n4;
}

uint8_t hxc_SpanFixture_spanBeforeConditionalArgument(bool hxc_l_selectThird)
{
  uint8_t hxc_l_values[4] = { (uint8_t)2, (uint8_t)4, (uint8_t)8, (uint8_t)16 };
  const uint8_t *hxc_l_readOnly = hxc_l_values;
  size_t hxc_l_tmp_length_n2 = sizeof(hxc_l_values) / sizeof(hxc_l_values[0]);
  const uint8_t *hxc_l_tmp_static_call_argument_0_n3 = hxc_l_readOnly;
  size_t hxc_l_tmp_length_n3 = hxc_l_tmp_length_n2;
  int32_t hxc_l_tmp_conditional_result_n4 = 0;
  if (hxc_l_selectThird)
  {
    hxc_l_tmp_conditional_result_n4 = 2;
  }
  else
  {
    hxc_l_tmp_conditional_result_n4 = 1;
  }
  int32_t hxc_l_tmp_conditional_load_result_n0 = hxc_l_tmp_conditional_result_n4;
  uint8_t hxc_l_tmp_call_result_n2 = hxc_SpanFixture_readAt(hxc_l_tmp_static_call_argument_0_n3, hxc_l_tmp_length_n3, hxc_l_tmp_conditional_load_result_n0);
  return hxc_l_tmp_call_result_n2;
}

uint8_t hxc_SpanFixture_zeroedGridCell(void)
{
  uint8_t hxc_l_values[16384] = { 0 };
  const uint8_t *hxc_l_view = hxc_l_values;
  size_t hxc_l_tmp_length_n1 = sizeof(hxc_l_values) / sizeof(hxc_l_values[0]);
  int32_t hxc_l_tmp_call_result_n0 = hxc_SpanFixture_linearIndex(0, 0, 0);
  if (hxc_l_tmp_call_result_n0 < 0 || (size_t)hxc_l_tmp_call_result_n0 >= hxc_l_tmp_length_n1)
  {
    abort();
  }
  uint8_t hxc_l_tmp_collection_index_load_result_n1 = hxc_l_view[(size_t)hxc_l_tmp_call_result_n0];
  return hxc_l_tmp_collection_index_load_result_n1;
}

uint8_t hxc_SpanConditionalAssignment_assignSelected(uint8_t *hxc_l_values, size_t hxc_l_length, int32_t hxc_l_index, uint8_t hxc_l_whenTrue, uint8_t hxc_l_whenFalse, bool hxc_l_selectTrue)
{
  uint8_t *hxc_l_borrow = hxc_l_values;
  size_t hxc_l_tmp_length_n5 = hxc_l_length;
  if (hxc_l_index < 0 || (size_t)hxc_l_index >= hxc_l_tmp_length_n5)
  {
    abort();
  }
  uint8_t *hxc_l_tmp_address_n6 = &hxc_l_borrow[(size_t)hxc_l_index];
  uint8_t hxc_l_tmp_conditional_result_n7 = 0;
  if (hxc_l_selectTrue)
  {
    hxc_l_tmp_conditional_result_n7 = hxc_l_whenTrue;
  }
  else
  {
    hxc_l_tmp_conditional_result_n7 = hxc_l_whenFalse;
  }
  uint8_t hxc_l_tmp_conditional_load_result_n1 = hxc_l_tmp_conditional_result_n7;
  *hxc_l_tmp_address_n6 = hxc_l_tmp_conditional_load_result_n1;
  if (hxc_l_index < 0 || (size_t)hxc_l_index >= hxc_l_tmp_length_n5)
  {
    abort();
  }
  uint8_t hxc_l_tmp_collection_index_load_result_n3 = hxc_l_borrow[(size_t)hxc_l_index];
  return hxc_l_tmp_collection_index_load_result_n3;
}

uint8_t hxc_SpanFixture_FinalSpanMutator_replace(struct hxc_SpanFixture_FinalSpanMutator *hxc_l_self, uint8_t *hxc_l_values, size_t hxc_l_length, int32_t hxc_l_index, uint8_t hxc_l_replacement)
{
  (void)hxc_l_self;
  uint8_t *hxc_l_borrow = hxc_l_values;
  size_t hxc_l_tmp_length_n4 = hxc_l_length;
  if (hxc_l_index < 0 || (size_t)hxc_l_index >= hxc_l_tmp_length_n4)
  {
    abort();
  }
  hxc_l_borrow[(size_t)hxc_l_index] = hxc_l_replacement;
  if (hxc_l_index < 0 || (size_t)hxc_l_index >= hxc_l_tmp_length_n4)
  {
    abort();
  }
  uint8_t hxc_l_tmp_collection_index_load_result_n0 = hxc_l_borrow[(size_t)hxc_l_index];
  return hxc_l_tmp_collection_index_load_result_n0;
}

int32_t hxc_SpanFixture_OwnedSpanBuffer_roundTrip(struct hxc_SpanFixture_OwnedSpanBuffer *hxc_l_self, uint8_t hxc_l_replacement)
{
  if (hxc_l_self == NULL)
  {
    abort();
  }
  (*hxc_l_self).hxc_bytes[(size_t)2] = hxc_l_replacement;
  (*hxc_l_self).hxc_numbers[(size_t)1] = 41;
  const uint8_t *hxc_l_readBytes = (*hxc_l_self).hxc_bytes;
  size_t hxc_l_tmp_length_n2 = 4;
  (void)hxc_l_tmp_length_n2;
  const int32_t *hxc_l_readNumbers = (*hxc_l_self).hxc_numbers;
  size_t hxc_l_tmp_length_n3 = 4;
  (void)hxc_l_tmp_length_n3;
  uint8_t hxc_l_tmp_collection_index_load_result_n0 = hxc_l_readBytes[(size_t)2];
  int32_t hxc_l_tmp_collection_index_load_result_n1 = hxc_l_readNumbers[(size_t)1];
  return hxc_i32_add_wrapping((int32_t)hxc_l_tmp_collection_index_load_result_n0, hxc_l_tmp_collection_index_load_result_n1);
}

const uint8_t *hxc_SpanFixture_ReturnedSpanOwner_view(struct hxc_SpanFixture_ReturnedSpanOwner *hxc_l_self, size_t *hxc_l_returned_span_length)
{
  if (hxc_l_self == NULL)
  {
    abort();
  }
  *hxc_l_returned_span_length = 4;
  return (*hxc_l_self).hxc_bytes;
}

int main(void)
{
  hxc_SpanFixture_main();
  return 0;
}
