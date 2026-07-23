#include "hxc/program.h"

_Static_assert(offsetof(struct hxc_SpanFixture_OwnedSpanBuffer, hxc_bytes) == 0, "class hxc_SpanFixture_OwnedSpanBuffer first storage field begins at offset zero");

_Static_assert(_Alignof(struct hxc_SpanFixture_OwnedSpanBuffer) >= _Alignof(uint8_t [4]), "class hxc_SpanFixture_OwnedSpanBuffer alignment admits field 0");

_Static_assert(offsetof(struct hxc_SpanFixture_OwnedSpanBuffer, hxc_numbers) >= offsetof(struct hxc_SpanFixture_OwnedSpanBuffer, hxc_bytes) + sizeof(uint8_t [4]), "class hxc_SpanFixture_OwnedSpanBuffer field 1 follows the prior storage without overlap");

_Static_assert(_Alignof(struct hxc_SpanFixture_OwnedSpanBuffer) >= _Alignof(int32_t [4]), "class hxc_SpanFixture_OwnedSpanBuffer alignment admits field 1");

_Static_assert(sizeof(struct hxc_SpanFixture_OwnedSpanBuffer) >= offsetof(struct hxc_SpanFixture_OwnedSpanBuffer, hxc_numbers) + sizeof(int32_t [4]), "class hxc_SpanFixture_OwnedSpanBuffer size contains its final storage member");

_Static_assert(offsetof(struct hxc_SpanFixture_FinalSpanMutator, hxc_storage) == 0, "class hxc_SpanFixture_FinalSpanMutator strict-C empty-storage anchor begins at zero");

_Static_assert(sizeof(struct hxc_SpanFixture_FinalSpanMutator) >= 1, "class hxc_SpanFixture_FinalSpanMutator strict-C empty-storage anchor occupies one byte");

void hxc_compiler_constructor_SpanFixture_OwnedSpanBuffer(struct hxc_SpanFixture_OwnedSpanBuffer *hxc_self)
{
  (void)hxc_self;
  return;
}

int32_t hxc_SpanFixture_checkedAt(int32_t hxc_index)
{
  int32_t hxc_values[4] = { 1, 3, 8, 21 };
  const int32_t *hxc_view = hxc_values;
  size_t hxc_tmp_length_n2 = sizeof(hxc_values) / sizeof(hxc_values[0]);
  if (hxc_index < 0 || (size_t)hxc_index >= hxc_tmp_length_n2)
  {
    abort();
  }
  int32_t hxc_tmp_collection_index_load_result_n0 = hxc_view[(size_t)hxc_index];
  return hxc_tmp_collection_index_load_result_n0;
}

uint8_t hxc_SpanFixture_conditionalAssignment(bool hxc_selectReplacement)
{
  uint8_t hxc_values[4] = { 0 };
  uint8_t *hxc_mutable = hxc_values;
  size_t hxc_tmp_length_n2 = sizeof(hxc_values) / sizeof(hxc_values[0]);
  uint8_t hxc_tmp_call_result_n0 = hxc_SpanConditionalAssignment_assignSelected(hxc_mutable, hxc_tmp_length_n2, 1, (uint8_t)37, (uint8_t)73, hxc_selectReplacement);
  return hxc_tmp_call_result_n0;
}

int32_t hxc_SpanFixture_constSum(void)
{
  int32_t hxc_values[4] = { 2, 4, 8, 16 };
  const int32_t *hxc_view = hxc_values;
  size_t hxc_tmp_length_n1 = sizeof(hxc_values) / sizeof(hxc_values[0]);
  (void)hxc_tmp_length_n1;
  int32_t hxc_tmp_collection_index_load_result_n0 = hxc_view[(size_t)0];
  int32_t hxc_total = hxc_tmp_collection_index_load_result_n0;
  size_t hxc_tmp_span_loop_index_n3 = 0;
  while (1)
  {
    if (!(hxc_tmp_span_loop_index_n3 < 4))
    {
      break;
    }
    size_t hxc_tmp_span_loop_body_index_result_n2 = hxc_tmp_span_loop_index_n3;
    int32_t hxc_tmp_span_loop_element_result_n3 = hxc_view[(size_t)hxc_tmp_span_loop_body_index_result_n2];
    hxc_total = hxc_i32_add_wrapping(hxc_total, hxc_tmp_span_loop_element_result_n3);
    hxc_tmp_span_loop_index_n3 = hxc_tmp_span_loop_index_n3 + 1;
  }
  return hxc_total;
}

uint8_t hxc_SpanFixture_finalClassParameterRoundTrip(uint8_t hxc_replacement)
{
  uint8_t hxc_values[4] = { 0 };
  uint8_t *hxc_view = hxc_values;
  size_t hxc_tmp_length_n2 = sizeof(hxc_values) / sizeof(hxc_values[0]);
  struct hxc_SpanFixture_FinalSpanMutator hxc_tmp_object_storage_n4 = { 0 };
  struct hxc_SpanFixture_FinalSpanMutator *hxc_tmp_class_object_address_n0 = &hxc_tmp_object_storage_n4;
  struct hxc_SpanFixture_FinalSpanMutator *hxc_mutator = hxc_tmp_class_object_address_n0;
  struct hxc_SpanFixture_FinalSpanMutator *hxc_tmp_load_result_n1 = hxc_mutator;
  if (hxc_tmp_load_result_n1 == NULL)
  {
    abort();
  }
  uint8_t hxc_tmp_instance_call_result_n2 = hxc_SpanFixture_FinalSpanMutator_replace(hxc_tmp_load_result_n1, hxc_view, hxc_tmp_length_n2, 2, hxc_replacement);
  return hxc_tmp_instance_call_result_n2;
}

uint8_t hxc_SpanFixture_forwardRead(const uint8_t *hxc_values, size_t hxc_length, int32_t hxc_index)
{
  const uint8_t *hxc_borrow = hxc_values;
  size_t hxc_tmp_length_n2 = hxc_length;
  (void)hxc_borrow;
  (void)hxc_tmp_length_n2;
  uint8_t hxc_tmp_call_result_n0 = hxc_SpanFixture_readAt(hxc_values, hxc_length, hxc_index);
  return hxc_tmp_call_result_n0;
}

uint8_t hxc_SpanFixture_forwardReplace(uint8_t *hxc_values, size_t hxc_length, int32_t hxc_index, uint8_t hxc_replacement)
{
  uint8_t *hxc_borrow = hxc_values;
  size_t hxc_tmp_length_n3 = hxc_length;
  (void)hxc_borrow;
  (void)hxc_tmp_length_n3;
  uint8_t hxc_tmp_call_result_n0 = hxc_SpanFixture_replaceAt(hxc_values, hxc_length, hxc_index, hxc_replacement);
  return hxc_tmp_call_result_n0;
}

int32_t hxc_SpanFixture_linearIndex(int32_t hxc_x, int32_t hxc_y, int32_t hxc_z)
{
  return hxc_i32_add_wrapping(hxc_x, hxc_i32_multiply_wrapping(32, hxc_i32_add_wrapping(hxc_y, hxc_i32_multiply_wrapping(16, hxc_z))));
}

void hxc_SpanFixture_main(void)
{
  hxc_SpanFixture_mutableSum();
  hxc_SpanFixture_constSum();
  hxc_SpanFixture_checkedAt(2);
  uint8_t hxc_tmp_call_result_n0 = hxc_SpanFixture_zeroedGridCell();
  hxc_SpanFixture_parameterRoundTrip(hxc_tmp_call_result_n0);
  hxc_SpanFixture_ownedFieldRoundTrip((uint8_t)1);
  hxc_SpanFixture_spanBeforeConditionalArgument(true);
  hxc_SpanFixture_zeroedGridCell();
  uint8_t hxc_tmp_call_result_n1 = hxc_SpanFixture_zeroedGridCell();
  hxc_SpanFixture_mutatedGridCell(hxc_tmp_call_result_n1);
  hxc_SpanFixture_finalClassParameterRoundTrip((uint8_t)201);
  hxc_SpanFixture_conditionalAssignment(true);
  hxc_SpanFixture_conditionalAssignment(false);
  return;
}

int32_t hxc_SpanFixture_mutableSum(void)
{
  int32_t hxc_values[4] = { 3, 5, 7, 11 };
  int32_t *hxc_view = hxc_values;
  size_t hxc_tmp_length_n1 = sizeof(hxc_values) / sizeof(hxc_values[0]);
  (void)hxc_tmp_length_n1;
  hxc_view[(size_t)1] = 13;
  int32_t hxc_total = 0;
  size_t hxc_tmp_span_loop_index_n3 = 0;
  while (1)
  {
    if (!(hxc_tmp_span_loop_index_n3 < 4))
    {
      break;
    }
    size_t hxc_tmp_span_loop_body_index_result_n1 = hxc_tmp_span_loop_index_n3;
    int32_t hxc_tmp_span_loop_element_result_n2 = hxc_view[(size_t)hxc_tmp_span_loop_body_index_result_n1];
    hxc_total = hxc_i32_add_wrapping(hxc_total, hxc_tmp_span_loop_element_result_n2);
    hxc_tmp_span_loop_index_n3 = hxc_tmp_span_loop_index_n3 + 1;
  }
  return hxc_total;
}

uint8_t hxc_SpanFixture_mutatedGridCell(uint8_t hxc_replacement)
{
  uint8_t hxc_values[16384] = { 0 };
  int32_t hxc_tmp_call_result_n0 = hxc_SpanFixture_linearIndex(31, 15, 31);
  int32_t hxc_index = hxc_tmp_call_result_n0;
  int32_t hxc_tmp_load_result_n1 = hxc_index;
  if (hxc_tmp_load_result_n1 < 0 || (size_t)hxc_tmp_load_result_n1 >= sizeof(hxc_values) / sizeof(hxc_values[0]))
  {
    abort();
  }
  hxc_values[(size_t)hxc_tmp_load_result_n1] = hxc_replacement;
  uint8_t *hxc_mutable = hxc_values;
  size_t hxc_tmp_length_n3 = sizeof(hxc_values) / sizeof(hxc_values[0]);
  int32_t hxc_tmp_load_result_n2 = hxc_index;
  if (hxc_tmp_load_result_n2 < 0 || (size_t)hxc_tmp_load_result_n2 >= hxc_tmp_length_n3)
  {
    abort();
  }
  hxc_mutable[(size_t)hxc_tmp_load_result_n2] = hxc_replacement;
  uint8_t hxc_tmp_collection_index_load_result_n3 = hxc_values[(size_t)0];
  uint8_t hxc_observed = hxc_tmp_collection_index_load_result_n3;
  size_t hxc_tmp_span_loop_index_n5 = 0;
  while (1)
  {
    if (!(hxc_tmp_span_loop_index_n5 < 16384))
    {
      break;
    }
    size_t hxc_tmp_span_loop_body_index_result_n5 = hxc_tmp_span_loop_index_n5;
    uint8_t hxc_tmp_span_loop_element_result_n6 = hxc_mutable[(size_t)hxc_tmp_span_loop_body_index_result_n5];
    hxc_observed = hxc_tmp_span_loop_element_result_n6;
    hxc_tmp_span_loop_index_n5 = hxc_tmp_span_loop_index_n5 + 1;
  }
  const uint8_t *hxc_readOnly = hxc_values;
  size_t hxc_tmp_length_n6 = sizeof(hxc_values) / sizeof(hxc_values[0]);
  int32_t hxc_tmp_load_result_n8 = hxc_index;
  if (hxc_tmp_load_result_n8 < 0 || (size_t)hxc_tmp_load_result_n8 >= hxc_tmp_length_n6)
  {
    abort();
  }
  uint8_t hxc_tmp_collection_index_load_result_n9 = hxc_readOnly[(size_t)hxc_tmp_load_result_n8];
  hxc_observed = hxc_tmp_collection_index_load_result_n9;
  return hxc_observed;
}

int32_t hxc_SpanFixture_ownedFieldRoundTrip(uint8_t hxc_replacement)
{
  struct hxc_SpanFixture_OwnedSpanBuffer hxc_tmp_object_storage_n2 = { 0 };
  struct hxc_SpanFixture_OwnedSpanBuffer *hxc_tmp_class_object_address_n0 = &hxc_tmp_object_storage_n2;
  hxc_compiler_constructor_SpanFixture_OwnedSpanBuffer(hxc_tmp_class_object_address_n0);
  struct hxc_SpanFixture_OwnedSpanBuffer *hxc_buffer = hxc_tmp_class_object_address_n0;
  struct hxc_SpanFixture_OwnedSpanBuffer *hxc_tmp_load_result_n1 = hxc_buffer;
  if (hxc_tmp_load_result_n1 == NULL)
  {
    abort();
  }
  int32_t hxc_tmp_instance_call_result_n2 = hxc_SpanFixture_OwnedSpanBuffer_roundTrip(hxc_tmp_load_result_n1, hxc_replacement);
  return hxc_tmp_instance_call_result_n2;
}

uint8_t hxc_SpanFixture_parameterRoundTrip(uint8_t hxc_replacement)
{
  uint8_t hxc_values[4] = { 0 };
  uint8_t *hxc_mutable = hxc_values;
  size_t hxc_tmp_length_n2 = sizeof(hxc_values) / sizeof(hxc_values[0]);
  hxc_SpanFixture_forwardReplace(hxc_mutable, hxc_tmp_length_n2, 2, hxc_replacement);
  const uint8_t *hxc_readOnly = hxc_values;
  size_t hxc_tmp_length_n3 = sizeof(hxc_values) / sizeof(hxc_values[0]);
  uint8_t hxc_tmp_call_result_n0 = hxc_SpanFixture_forwardRead(hxc_readOnly, hxc_tmp_length_n3, 2);
  return hxc_tmp_call_result_n0;
}

uint8_t hxc_SpanFixture_readAt(const uint8_t *hxc_values, size_t hxc_length, int32_t hxc_index)
{
  const uint8_t *hxc_borrow = hxc_values;
  size_t hxc_tmp_length_n2 = hxc_length;
  if (hxc_index < 0 || (size_t)hxc_index >= hxc_tmp_length_n2)
  {
    abort();
  }
  uint8_t hxc_tmp_collection_index_load_result_n0 = hxc_borrow[(size_t)hxc_index];
  return hxc_tmp_collection_index_load_result_n0;
}

uint8_t hxc_SpanFixture_replaceAt(uint8_t *hxc_values, size_t hxc_length, int32_t hxc_index, uint8_t hxc_replacement)
{
  uint8_t *hxc_borrow = hxc_values;
  size_t hxc_tmp_length_n3 = hxc_length;
  if (hxc_index < 0 || (size_t)hxc_index >= hxc_tmp_length_n3)
  {
    abort();
  }
  hxc_borrow[(size_t)hxc_index] = hxc_replacement;
  if (hxc_index < 0 || (size_t)hxc_index >= hxc_tmp_length_n3)
  {
    abort();
  }
  uint8_t hxc_tmp_collection_index_load_result_n0 = hxc_borrow[(size_t)hxc_index];
  return hxc_tmp_collection_index_load_result_n0;
}

uint8_t hxc_SpanFixture_spanBeforeConditionalArgument(bool hxc_selectThird)
{
  uint8_t hxc_values[4] = { (uint8_t)2, (uint8_t)4, (uint8_t)8, (uint8_t)16 };
  const uint8_t *hxc_readOnly = hxc_values;
  size_t hxc_tmp_length_n2 = sizeof(hxc_values) / sizeof(hxc_values[0]);
  const uint8_t *hxc_tmp_static_call_argument_0_n3 = hxc_readOnly;
  size_t hxc_tmp_length_n3 = hxc_tmp_length_n2;
  int32_t hxc_tmp_conditional_result_n4 = 0;
  if (hxc_selectThird)
  {
    hxc_tmp_conditional_result_n4 = 2;
  }
  else
  {
    hxc_tmp_conditional_result_n4 = 1;
  }
  int32_t hxc_tmp_conditional_load_result_n0 = hxc_tmp_conditional_result_n4;
  uint8_t hxc_tmp_call_result_n2 = hxc_SpanFixture_readAt(hxc_tmp_static_call_argument_0_n3, hxc_tmp_length_n3, hxc_tmp_conditional_load_result_n0);
  return hxc_tmp_call_result_n2;
}

uint8_t hxc_SpanFixture_zeroedGridCell(void)
{
  uint8_t hxc_values[16384] = { 0 };
  const uint8_t *hxc_view = hxc_values;
  size_t hxc_tmp_length_n1 = sizeof(hxc_values) / sizeof(hxc_values[0]);
  int32_t hxc_tmp_call_result_n0 = hxc_SpanFixture_linearIndex(0, 0, 0);
  if (hxc_tmp_call_result_n0 < 0 || (size_t)hxc_tmp_call_result_n0 >= hxc_tmp_length_n1)
  {
    abort();
  }
  uint8_t hxc_tmp_collection_index_load_result_n1 = hxc_view[(size_t)hxc_tmp_call_result_n0];
  return hxc_tmp_collection_index_load_result_n1;
}

uint8_t hxc_SpanConditionalAssignment_assignSelected(uint8_t *hxc_values, size_t hxc_length, int32_t hxc_index, uint8_t hxc_whenTrue, uint8_t hxc_whenFalse, bool hxc_selectTrue)
{
  uint8_t *hxc_borrow = hxc_values;
  size_t hxc_tmp_length_n5 = hxc_length;
  if (hxc_index < 0 || (size_t)hxc_index >= hxc_tmp_length_n5)
  {
    abort();
  }
  uint8_t hxc_tmp_conditional_result_n6 = 0;
  if (hxc_selectTrue)
  {
    hxc_tmp_conditional_result_n6 = hxc_whenTrue;
  }
  else
  {
    hxc_tmp_conditional_result_n6 = hxc_whenFalse;
  }
  hxc_borrow[(size_t)hxc_index] = hxc_tmp_conditional_result_n6;
  if (hxc_index < 0 || (size_t)hxc_index >= hxc_tmp_length_n5)
  {
    abort();
  }
  uint8_t hxc_tmp_collection_index_load_result_n1 = hxc_borrow[(size_t)hxc_index];
  return hxc_tmp_collection_index_load_result_n1;
}

uint8_t hxc_SpanFixture_FinalSpanMutator_replace(struct hxc_SpanFixture_FinalSpanMutator *hxc_self, uint8_t *hxc_values, size_t hxc_length, int32_t hxc_index, uint8_t hxc_replacement)
{
  (void)hxc_self;
  uint8_t *hxc_borrow = hxc_values;
  size_t hxc_tmp_length_n4 = hxc_length;
  if (hxc_index < 0 || (size_t)hxc_index >= hxc_tmp_length_n4)
  {
    abort();
  }
  hxc_borrow[(size_t)hxc_index] = hxc_replacement;
  if (hxc_index < 0 || (size_t)hxc_index >= hxc_tmp_length_n4)
  {
    abort();
  }
  uint8_t hxc_tmp_collection_index_load_result_n0 = hxc_borrow[(size_t)hxc_index];
  return hxc_tmp_collection_index_load_result_n0;
}

int32_t hxc_SpanFixture_OwnedSpanBuffer_roundTrip(struct hxc_SpanFixture_OwnedSpanBuffer *hxc_self, uint8_t hxc_replacement)
{
  if (hxc_self == NULL)
  {
    abort();
  }
  (*hxc_self).hxc_bytes[(size_t)2] = hxc_replacement;
  (*hxc_self).hxc_numbers[(size_t)1] = 41;
  const uint8_t *hxc_readBytes = (*hxc_self).hxc_bytes;
  size_t hxc_tmp_length_n2 = 4;
  (void)hxc_tmp_length_n2;
  const int32_t *hxc_readNumbers = (*hxc_self).hxc_numbers;
  size_t hxc_tmp_length_n3 = 4;
  (void)hxc_tmp_length_n3;
  uint8_t hxc_tmp_collection_index_load_result_n0 = hxc_readBytes[(size_t)2];
  int32_t hxc_tmp_collection_index_load_result_n1 = hxc_readNumbers[(size_t)1];
  return hxc_i32_add_wrapping((int32_t)hxc_tmp_collection_index_load_result_n0, hxc_tmp_collection_index_load_result_n1);
}

int main(void)
{
  hxc_SpanFixture_main();
  return 0;
}
