#include "hxc/program.h"

void hxc_compiler_constructor_caxecraft_domain_EntityStore(struct hxc_caxecraft_domain_EntityStore *hxc_self)
{
  struct hxc_array_ref *hxc_tmp_array_create_result_n0 = NULL;
  if (hxc_array_ref_create_trivial(hxc_default_allocator(), sizeof(struct hxc_caxecraft_domain_Character), _Alignof(struct hxc_caxecraft_domain_Character), &hxc_tmp_array_create_result_n0) != HXC_STATUS_OK)
  {
    abort();
  }
  (*hxc_self).hxc_characters = hxc_tmp_array_create_result_n0;
  return;
}

bool hxc_caxecraft_domain_EntityStore_contains(struct hxc_caxecraft_domain_EntityStore *hxc_self, int32_t hxc_id)
{
  if (hxc_self == NULL)
  {
    abort();
  }
  int32_t hxc_tmp_instance_call_result_n0 = hxc_caxecraft_domain_EntityStore_indexOf(hxc_self, hxc_id);
  return hxc_tmp_instance_call_result_n0 >= 0;
}

int32_t hxc_caxecraft_domain_EntityStore_indexOf(struct hxc_caxecraft_domain_EntityStore *hxc_self, int32_t hxc_id)
{
  if (hxc_id <= 0)
  {
    return -1;
  }
  int32_t hxc_g = 0;
  if (hxc_self == NULL)
  {
    abort();
  }
  struct hxc_array_ref *hxc_tmp_class_field_load_result_n0 = (*hxc_self).hxc_characters;
  int32_t hxc_tmp_array_length_result_n1;
  if (hxc_array_ref_length(hxc_tmp_class_field_load_result_n0, &hxc_tmp_array_length_result_n1) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_g1 = hxc_tmp_array_length_result_n1;
  while (1)
  {
    int32_t hxc_tmp_load_result_n2 = hxc_g;
    if (!(hxc_tmp_load_result_n2 < hxc_g1))
    {
      break;
    }
    int32_t hxc_tmp_increment_load_result_n4 = hxc_g;
    hxc_g = hxc_i32_add_wrapping(hxc_tmp_increment_load_result_n4, 1);
    int32_t hxc_index = hxc_tmp_increment_load_result_n4;
    struct hxc_array_ref *hxc_tmp_class_field_load_result_n5 = (*hxc_self).hxc_characters;
    struct hxc_caxecraft_domain_Character hxc_tmp_array_get_result_n7;
    if (hxc_array_ref_get_copy(hxc_tmp_class_field_load_result_n5, (size_t)hxc_index, &hxc_tmp_array_get_result_n7) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_tmp_array_get_result_n7.hxc_id == hxc_id)
    {
      return hxc_index;
    }
  }
  return -1;
}

bool hxc_caxecraft_domain_EntityStore_put(struct hxc_caxecraft_domain_EntityStore *hxc_self, struct hxc_caxecraft_domain_Character hxc_character)
{
  bool hxc_tmp_short_circuit_result_n2 = hxc_character.hxc_id <= 0;
  if (!(hxc_character.hxc_id <= 0))
  {
    if (hxc_self == NULL)
    {
      abort();
    }
    bool hxc_tmp_instance_call_result_n2 = hxc_caxecraft_domain_EntityStore_contains(hxc_self, hxc_character.hxc_id);
    hxc_tmp_short_circuit_result_n2 = hxc_tmp_instance_call_result_n2;
  }
  bool hxc_tmp_short_circuit_load_result_n3 = hxc_tmp_short_circuit_result_n2;
  bool hxc_tmp_short_circuit_result_n3 = hxc_tmp_short_circuit_load_result_n3;
  if (!hxc_tmp_short_circuit_load_result_n3)
  {
    if (hxc_self == NULL)
    {
      abort();
    }
    struct hxc_array_ref *hxc_tmp_class_field_load_result_n4 = (*hxc_self).hxc_characters;
    int32_t hxc_tmp_array_length_result_n5;
    if (hxc_array_ref_length(hxc_tmp_class_field_load_result_n4, &hxc_tmp_array_length_result_n5) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_tmp_short_circuit_result_n3 = hxc_tmp_array_length_result_n5 >= 64;
  }
  if (!hxc_tmp_short_circuit_result_n3)
  {
    if (hxc_self == NULL)
    {
      abort();
    }
    struct hxc_array_ref *hxc_tmp_class_field_load_result_n7 = (*hxc_self).hxc_characters;
    int32_t hxc_tmp_array_push_result_n8;
    if (hxc_array_ref_push_copy(hxc_tmp_class_field_load_result_n7, &hxc_character, &hxc_tmp_array_push_result_n8) != HXC_STATUS_OK)
    {
      abort();
    }
    (void)hxc_tmp_array_push_result_n8;
    return true;
  }
  return false;
}

struct hxc_caxecraft_domain_Character hxc_caxecraft_domain_EntityStore_read(struct hxc_caxecraft_domain_EntityStore *hxc_self, int32_t hxc_id)
{
  if (hxc_self == NULL)
  {
    abort();
  }
  int32_t hxc_tmp_instance_call_result_n0 = hxc_caxecraft_domain_EntityStore_indexOf(hxc_self, hxc_id);
  int32_t hxc_index = hxc_tmp_instance_call_result_n0;
  if (!(hxc_index < 0))
  {
    struct hxc_array_ref *hxc_tmp_class_field_load_result_n3 = (*hxc_self).hxc_characters;
    struct hxc_caxecraft_domain_Character hxc_tmp_array_get_result_n5;
    if (hxc_array_ref_get_copy(hxc_tmp_class_field_load_result_n3, (size_t)hxc_index, &hxc_tmp_array_get_result_n5) != HXC_STATUS_OK)
    {
      abort();
    }
    return hxc_tmp_array_get_result_n5;
  }
  struct hxc_caxecraft_domain_Character hxc_tmp_call_result_n2 = hxc_caxecraft_domain_Character_empty();
  return hxc_tmp_call_result_n2;
}

bool hxc_caxecraft_domain_EntityStore_replace(struct hxc_caxecraft_domain_EntityStore *hxc_self, int32_t hxc_id, struct hxc_caxecraft_domain_Character hxc_character)
{
  if (hxc_self == NULL)
  {
    abort();
  }
  int32_t hxc_tmp_instance_call_result_n0 = hxc_caxecraft_domain_EntityStore_indexOf(hxc_self, hxc_id);
  int32_t hxc_index = hxc_tmp_instance_call_result_n0;
  int32_t hxc_tmp_load_result_n1 = hxc_index;
  bool hxc_tmp_short_circuit_result_n4 = hxc_tmp_load_result_n1 < 0;
  if (!(hxc_tmp_load_result_n1 < 0))
  {
    hxc_tmp_short_circuit_result_n4 = hxc_character.hxc_id != hxc_id;
  }
  bool hxc_tmp_short_circuit_load_result_n3 = hxc_tmp_short_circuit_result_n4;
  bool hxc_tmp_short_circuit_result_n5 = hxc_tmp_short_circuit_load_result_n3;
  if (!hxc_tmp_short_circuit_load_result_n3)
  {
    hxc_tmp_short_circuit_result_n5 = hxc_character.hxc_id <= 0;
  }
  if (!hxc_tmp_short_circuit_result_n5)
  {
    struct hxc_array_ref *hxc_tmp_class_field_load_result_n6 = (*hxc_self).hxc_characters;
    if (hxc_array_ref_set_copy(hxc_tmp_class_field_load_result_n6, (size_t)hxc_index, &hxc_character) != HXC_STATUS_OK)
    {
      abort();
    }
    (void)hxc_character;
    return true;
  }
  return false;
}

bool hxc_caxecraft_domain_EntityStore_replaceOthers(struct hxc_caxecraft_domain_EntityStore *hxc_self, int32_t hxc_retained, struct hxc_array_ref *hxc_replacements)
{
  struct hxc_array_ref *hxc_next = { 0 };
  if (hxc_self == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_domain_Character hxc_tmp_instance_call_result_n0 = hxc_caxecraft_domain_EntityStore_read(hxc_self, hxc_retained);
  struct hxc_caxecraft_domain_Character hxc_retainedCharacter = hxc_tmp_instance_call_result_n0;
  int32_t hxc_tmp_record_field_load_result_n1 = hxc_retainedCharacter.hxc_id;
  bool hxc_tmp_short_circuit_result_n4 = hxc_tmp_record_field_load_result_n1 <= 0;
  if (!(hxc_tmp_record_field_load_result_n1 <= 0))
  {
    int32_t hxc_tmp_array_length_result_n2;
    if (hxc_array_ref_length(hxc_replacements, &hxc_tmp_array_length_result_n2) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_tmp_short_circuit_result_n4 = hxc_i32_add_wrapping(hxc_tmp_array_length_result_n2, 1) > 64;
  }
  if (hxc_tmp_short_circuit_result_n4)
  {
    return false;
  }
  struct hxc_array_ref *hxc_tmp_array_create_result_n5 = NULL;
  if (hxc_array_ref_create_trivial(hxc_default_allocator(), sizeof(struct hxc_caxecraft_domain_Character), _Alignof(struct hxc_caxecraft_domain_Character), &hxc_tmp_array_create_result_n5) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_tmp_array_create_result_n5->value, &hxc_retainedCharacter) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_next = hxc_tmp_array_create_result_n5;
  int32_t hxc_g_hae738e136b16 = 0;
  while (1)
  {
    int32_t hxc_tmp_load_result_n6 = hxc_g_hae738e136b16;
    int32_t hxc_tmp_array_length_result_n7;
    if (hxc_array_ref_length(hxc_replacements, &hxc_tmp_array_length_result_n7) != HXC_STATUS_OK)
    {
      abort();
    }
    if (!(hxc_tmp_load_result_n6 < hxc_tmp_array_length_result_n7))
    {
      break;
    }
    struct hxc_caxecraft_domain_Character hxc_tmp_array_get_result_n9;
    if (hxc_array_ref_get_copy(hxc_replacements, (size_t)hxc_g_hae738e136b16, &hxc_tmp_array_get_result_n9) != HXC_STATUS_OK)
    {
      abort();
    }
    struct hxc_caxecraft_domain_Character hxc_replacement = hxc_tmp_array_get_result_n9;
    hxc_g_hae738e136b16 = hxc_i32_add_wrapping(hxc_g_hae738e136b16, 1);
    int32_t hxc_tmp_record_field_load_result_n11 = hxc_replacement.hxc_id;
    bool hxc_tmp_short_circuit_result_n8 = hxc_tmp_record_field_load_result_n11 <= 0;
    if (!(hxc_tmp_record_field_load_result_n11 <= 0))
    {
      hxc_tmp_short_circuit_result_n8 = hxc_replacement.hxc_id == hxc_retained;
    }
    if (hxc_tmp_short_circuit_result_n8)
    {
      if (hxc_array_ref_release(hxc_next) != HXC_STATUS_OK)
      {
        abort();
      }
      return false;
    }
    int32_t hxc_g_h53a5c25c54fb = 0;
    while (1)
    {
      int32_t hxc_tmp_load_result_n14 = hxc_g_h53a5c25c54fb;
      int32_t hxc_tmp_array_length_result_n16;
      if (hxc_array_ref_length(hxc_next, &hxc_tmp_array_length_result_n16) != HXC_STATUS_OK)
      {
        abort();
      }
      if (!(hxc_tmp_load_result_n14 < hxc_tmp_array_length_result_n16))
      {
        break;
      }
      struct hxc_array_ref *hxc_tmp_load_result_n17 = hxc_next;
      struct hxc_caxecraft_domain_Character hxc_tmp_array_get_result_n19;
      if (hxc_array_ref_get_copy(hxc_tmp_load_result_n17, (size_t)hxc_g_h53a5c25c54fb, &hxc_tmp_array_get_result_n19) != HXC_STATUS_OK)
      {
        abort();
      }
      struct hxc_caxecraft_domain_Character hxc_accepted = hxc_tmp_array_get_result_n19;
      hxc_g_h53a5c25c54fb = hxc_i32_add_wrapping(hxc_g_h53a5c25c54fb, 1);
      int32_t hxc_tmp_record_field_load_result_n21 = hxc_accepted.hxc_id;
      if (hxc_tmp_record_field_load_result_n21 == hxc_replacement.hxc_id)
      {
        if (hxc_array_ref_release(hxc_next) != HXC_STATUS_OK)
        {
          abort();
        }
        return false;
      }
    }
    struct hxc_array_ref *hxc_tmp_load_result_n23 = hxc_next;
    int32_t hxc_tmp_array_push_result_n25;
    if (hxc_array_ref_push_copy(hxc_tmp_load_result_n23, &hxc_replacement, &hxc_tmp_array_push_result_n25) != HXC_STATUS_OK)
    {
      abort();
    }
    (void)hxc_tmp_array_push_result_n25;
  }
  while (1)
  {
    struct hxc_array_ref *hxc_tmp_class_field_load_result_n26 = (*hxc_self).hxc_characters;
    int32_t hxc_tmp_array_length_result_n27;
    if (hxc_array_ref_length(hxc_tmp_class_field_load_result_n26, &hxc_tmp_array_length_result_n27) != HXC_STATUS_OK)
    {
      abort();
    }
    if (!(hxc_tmp_array_length_result_n27 > 0))
    {
      break;
    }
    struct hxc_array_ref *hxc_tmp_class_field_load_result_n28 = (*hxc_self).hxc_characters;
    struct hxc_optional_caxecraft_domain_Character hxc_tmp_array_pop_result_n29 = (struct hxc_optional_caxecraft_domain_Character){ .hxc_has_value = false };
    if (hxc_array_ref_pop_move(hxc_tmp_class_field_load_result_n28, &hxc_tmp_array_pop_result_n29.hxc_value, &hxc_tmp_array_pop_result_n29.hxc_has_value) != HXC_STATUS_OK)
    {
      abort();
    }
    (void)hxc_tmp_array_pop_result_n29;
  }
  int32_t hxc_g_hd5bd495666ed = 0;
  while (1)
  {
    int32_t hxc_tmp_load_result_n30 = hxc_g_hd5bd495666ed;
    int32_t hxc_tmp_array_length_result_n32;
    if (hxc_array_ref_length(hxc_next, &hxc_tmp_array_length_result_n32) != HXC_STATUS_OK)
    {
      abort();
    }
    if (!(hxc_tmp_load_result_n30 < hxc_tmp_array_length_result_n32))
    {
      break;
    }
    struct hxc_array_ref *hxc_tmp_load_result_n33 = hxc_next;
    struct hxc_caxecraft_domain_Character hxc_tmp_array_get_result_n35;
    if (hxc_array_ref_get_copy(hxc_tmp_load_result_n33, (size_t)hxc_g_hd5bd495666ed, &hxc_tmp_array_get_result_n35) != HXC_STATUS_OK)
    {
      abort();
    }
    struct hxc_caxecraft_domain_Character hxc_character = hxc_tmp_array_get_result_n35;
    hxc_g_hd5bd495666ed = hxc_i32_add_wrapping(hxc_g_hd5bd495666ed, 1);
    struct hxc_array_ref *hxc_tmp_class_field_load_result_n37 = (*hxc_self).hxc_characters;
    int32_t hxc_tmp_array_push_result_n39;
    if (hxc_array_ref_push_copy(hxc_tmp_class_field_load_result_n37, &hxc_character, &hxc_tmp_array_push_result_n39) != HXC_STATUS_OK)
    {
      abort();
    }
    (void)hxc_tmp_array_push_result_n39;
  }
  if (hxc_array_ref_release(hxc_next) != HXC_STATUS_OK)
  {
    abort();
  }
  return true;
}
