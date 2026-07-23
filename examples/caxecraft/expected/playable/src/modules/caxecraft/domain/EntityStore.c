#include "hxc/program.h"

void hxc_compiler_constructor_caxecraft_domain_EntityStore(struct hxc_caxecraft_domain_EntityStore *hxc_self)
{
  (*hxc_self).hxc_occupied = false;
  struct hxc_caxecraft_domain_Character hxc_tmp_call_result_n0 = hxc_caxecraft_domain_Character_empty();
  (*hxc_self).hxc_current = hxc_tmp_call_result_n0;
  return;
}

bool hxc_caxecraft_domain_EntityStore_put(struct hxc_caxecraft_domain_EntityStore *hxc_self, struct hxc_caxecraft_domain_Character hxc_character)
{
  if (!(hxc_character.hxc_id <= 0))
  {
    if (hxc_self == NULL)
    {
      abort();
    }
    bool hxc_tmp_class_field_load_result_n1 = (*hxc_self).hxc_occupied;
    bool hxc_tmp_short_circuit_result_n2 = hxc_tmp_class_field_load_result_n1;
    if (hxc_tmp_class_field_load_result_n1)
    {
      struct hxc_caxecraft_domain_Character hxc_tmp_class_field_load_result_n2 = (*hxc_self).hxc_current;
      hxc_tmp_short_circuit_result_n2 = hxc_tmp_class_field_load_result_n2.hxc_id != hxc_character.hxc_id;
    }
    if (!hxc_tmp_short_circuit_result_n2)
    {
      (*hxc_self).hxc_current = hxc_character;
      (*hxc_self).hxc_occupied = true;
      return true;
    }
    return false;
  }
  return false;
}

struct hxc_caxecraft_domain_Character hxc_caxecraft_domain_EntityStore_read(struct hxc_caxecraft_domain_EntityStore *hxc_self, int32_t hxc_id)
{
  if (hxc_self == NULL)
  {
    abort();
  }
  bool hxc_tmp_class_field_load_result_n0 = (*hxc_self).hxc_occupied;
  bool hxc_tmp_short_circuit_result_n2 = hxc_tmp_class_field_load_result_n0;
  if (hxc_tmp_class_field_load_result_n0)
  {
    struct hxc_caxecraft_domain_Character hxc_tmp_class_field_load_result_n1 = (*hxc_self).hxc_current;
    hxc_tmp_short_circuit_result_n2 = hxc_tmp_class_field_load_result_n1.hxc_id == hxc_id;
  }
  if (!hxc_tmp_short_circuit_result_n2)
  {
    struct hxc_caxecraft_domain_Character hxc_tmp_call_result_n5 = hxc_caxecraft_domain_Character_empty();
    return hxc_tmp_call_result_n5;
  }
  struct hxc_caxecraft_domain_Character hxc_tmp_class_field_load_result_n4 = (*hxc_self).hxc_current;
  return hxc_tmp_class_field_load_result_n4;
}

bool hxc_caxecraft_domain_EntityStore_replace(struct hxc_caxecraft_domain_EntityStore *hxc_self, int32_t hxc_id, struct hxc_caxecraft_domain_Character hxc_character)
{
  if (hxc_self == NULL)
  {
    abort();
  }
  bool hxc_tmp_class_field_load_result_n0 = (*hxc_self).hxc_occupied;
  bool hxc_tmp_short_circuit_result_n3 = hxc_tmp_class_field_load_result_n0;
  if (hxc_tmp_class_field_load_result_n0)
  {
    struct hxc_caxecraft_domain_Character hxc_tmp_class_field_load_result_n1 = (*hxc_self).hxc_current;
    hxc_tmp_short_circuit_result_n3 = hxc_tmp_class_field_load_result_n1.hxc_id == hxc_id;
  }
  bool hxc_tmp_short_circuit_load_result_n3 = hxc_tmp_short_circuit_result_n3;
  bool hxc_tmp_short_circuit_result_n4 = !hxc_tmp_short_circuit_load_result_n3;
  if (!!hxc_tmp_short_circuit_load_result_n3)
  {
    hxc_tmp_short_circuit_result_n4 = hxc_character.hxc_id != hxc_id;
  }
  if (!hxc_tmp_short_circuit_result_n4)
  {
    (*hxc_self).hxc_current = hxc_character;
    return true;
  }
  return false;
}
