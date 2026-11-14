#include "hxc/program.h"

void hxc_compiler_constructor_caxecraft_app_CaxecraftApp(struct hxc_caxecraft_app_CaxecraftApp *hxc_l_self)
{
  struct hxc_caxecraft_app_WaterRenderer *hxc_l_tmp_owned_class_field_address_n0 = &(*hxc_l_self).hxc_portalWaterRenderer;
  hxc_compiler_constructor_caxecraft_app_WaterRenderer(hxc_l_tmp_owned_class_field_address_n0);
  struct hxc_caxecraft_app_WaterRenderer *hxc_l_tmp_owned_class_field_address_n1 = &(*hxc_l_self).hxc_waterRenderer;
  hxc_compiler_constructor_caxecraft_app_WaterRenderer(hxc_l_tmp_owned_class_field_address_n1);
  struct hxc_caxecraft_app_DistantHorizonRenderer *hxc_l_tmp_owned_class_field_address_n2 = &(*hxc_l_self).hxc_distantHorizonRenderer;
  hxc_compiler_constructor_caxecraft_app_DistantHorizonRenderer(hxc_l_tmp_owned_class_field_address_n2);
  struct hxc_caxecraft_app_TerrainRenderer *hxc_l_tmp_owned_class_field_address_n3 = &(*hxc_l_self).hxc_portalTerrainRenderer;
  hxc_compiler_constructor_caxecraft_app_TerrainRenderer(hxc_l_tmp_owned_class_field_address_n3);
  struct hxc_caxecraft_app_TerrainRenderer *hxc_l_tmp_owned_class_field_address_n4 = &(*hxc_l_self).hxc_terrainRenderer;
  hxc_compiler_constructor_caxecraft_app_TerrainRenderer(hxc_l_tmp_owned_class_field_address_n4);
  struct hxc_caxecraft_input_NavigationRepeater *hxc_l_tmp_owned_class_field_address_n5 = &(*hxc_l_self).hxc_editorNavigation;
  hxc_compiler_constructor_caxecraft_input_NavigationRepeater(hxc_l_tmp_owned_class_field_address_n5);
  return;
}

hxc_string hxc_caxecraft_app_CaxecraftApp_campaignLevelLoadFailure(struct hxc_caxecraft_content_CampaignLevelLoadError hxc_l_error)
{
  hxc_string hxc_l_path_h58ce759b9caf = { 0 };
  hxc_string hxc_l_tmp_string_concat_right_owner_n10 = { 0 };
  hxc_string hxc_l_tmp_string_concat_left_owner_n11 = { 0 };
  hxc_string hxc_l_path_h91a2fe8434cc = { 0 };
  hxc_string hxc_l_path_h1f0969577fa7 = { 0 };
  hxc_string hxc_l_path_hd8c174ddef11 = { 0 };
  int32_t hxc_l_expected = { 0 };
  int32_t hxc_l_actual = { 0 };
  hxc_string hxc_l_tmp_string_concat_left_owner_n5 = { 0 };
  hxc_string hxc_l_tmp_string_concat_left_owner_n6 = { 0 };
  hxc_string hxc_l_tmp_string_concat_right_owner_n7 = { 0 };
  hxc_string hxc_l_tmp_string_concat_left_owner_n8 = { 0 };
  hxc_string hxc_l_tmp_string_concat_left_owner_n9 = { 0 };
  switch (hxc_l_error.hxc_tag) {
    case hxc_caxecraft_content_CampaignLevelLoadError_CampaignLevelSourceRejected:
      {
        break;
      }
    case hxc_caxecraft_content_CampaignLevelLoadError_CampaignLevelLengthMismatch:
      {
        if (hxc_l_error.hxc_tag != hxc_caxecraft_content_CampaignLevelLoadError_CampaignLevelLengthMismatch)
        {
          abort();
        }
        hxc_string hxc_l_tmp_enum_payload_project_n3 = hxc_l_error.hxc_payload.hxc_CampaignLevelLengthMismatch.hxc_logicalPath;
        hxc_l_path_hd8c174ddef11 = hxc_l_tmp_enum_payload_project_n3;
        if (hxc_string_retain(hxc_l_path_hd8c174ddef11) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_l_error.hxc_tag != hxc_caxecraft_content_CampaignLevelLoadError_CampaignLevelLengthMismatch)
        {
          abort();
        }
        int32_t hxc_l_tmp_enum_payload_project_n4 = hxc_l_error.hxc_payload.hxc_CampaignLevelLengthMismatch.hxc_expected;
        hxc_l_expected = hxc_l_tmp_enum_payload_project_n4;
        if (hxc_l_error.hxc_tag != hxc_caxecraft_content_CampaignLevelLoadError_CampaignLevelLengthMismatch)
        {
          abort();
        }
        int32_t hxc_l_tmp_enum_payload_project_n5 = hxc_l_error.hxc_payload.hxc_CampaignLevelLengthMismatch.hxc_actual;
        hxc_l_actual = hxc_l_tmp_enum_payload_project_n5;
        hxc_string hxc_l_tmp_string_concat_result_n7 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
        if (hxc_string_concat_ref((hxc_string){ (const uint8_t *)"length changed for ", 19, true, NULL }, hxc_l_path_hd8c174ddef11, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n7) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_l_tmp_string_concat_left_owner_n5 = hxc_l_tmp_string_concat_result_n7;
        hxc_string hxc_l_tmp_string_concat_result_n9 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
        if (hxc_string_concat_ref(hxc_l_tmp_string_concat_left_owner_n5, (hxc_string){ (const uint8_t *)" (expected ", 11, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n9) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_l_tmp_string_concat_left_owner_n6 = hxc_l_tmp_string_concat_result_n9;
        hxc_string hxc_l_tmp_string_concat_left_borrow_result_n10 = hxc_l_tmp_string_concat_left_owner_n6;
        hxc_string hxc_l_tmp_std_string_int_result_n12 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
        if (hxc_string_from_int32(hxc_l_expected, hxc_default_allocator(), &hxc_l_tmp_std_string_int_result_n12) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_l_tmp_string_concat_right_owner_n7 = hxc_l_tmp_std_string_int_result_n12;
        hxc_string hxc_l_tmp_string_concat_result_n14 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
        if (hxc_string_concat_ref(hxc_l_tmp_string_concat_left_borrow_result_n10, hxc_l_tmp_string_concat_right_owner_n7, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n14) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_l_tmp_string_concat_left_owner_n8 = hxc_l_tmp_string_concat_result_n14;
        hxc_string hxc_l_tmp_string_concat_result_n16 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
        if (hxc_string_concat_ref(hxc_l_tmp_string_concat_left_owner_n8, (hxc_string){ (const uint8_t *)", found ", 8, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n16) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_l_tmp_string_concat_left_owner_n9 = hxc_l_tmp_string_concat_result_n16;
        hxc_string hxc_l_tmp_string_concat_left_borrow_result_n17 = hxc_l_tmp_string_concat_left_owner_n9;
        hxc_string hxc_l_tmp_std_string_int_result_n19 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
        if (hxc_string_from_int32(hxc_l_actual, hxc_default_allocator(), &hxc_l_tmp_std_string_int_result_n19) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_l_tmp_string_concat_right_owner_n10 = hxc_l_tmp_std_string_int_result_n19;
        hxc_string hxc_l_tmp_string_concat_result_n21 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
        if (hxc_string_concat_ref(hxc_l_tmp_string_concat_left_borrow_result_n17, hxc_l_tmp_string_concat_right_owner_n10, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n21) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_l_tmp_string_concat_left_owner_n11 = hxc_l_tmp_string_concat_result_n21;
        hxc_string hxc_l_tmp_string_concat_result_n23 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
        if (hxc_string_concat_ref(hxc_l_tmp_string_concat_left_owner_n11, (hxc_string){ (const uint8_t *)")", 1, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n23) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n11) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n10) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n9) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n8) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n7) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n6) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n5) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_string_release(&hxc_l_path_hd8c174ddef11) != HXC_STATUS_OK)
        {
          abort();
        }
        return hxc_l_tmp_string_concat_result_n23;
      }
    case hxc_caxecraft_content_CampaignLevelLoadError_CampaignLevelHashMismatch:
      {
        if (hxc_l_error.hxc_tag != hxc_caxecraft_content_CampaignLevelLoadError_CampaignLevelHashMismatch)
        {
          abort();
        }
        hxc_string hxc_l_tmp_enum_payload_project_n24 = hxc_l_error.hxc_payload.hxc_CampaignLevelHashMismatch.hxc_logicalPath;
        hxc_l_path_h91a2fe8434cc = hxc_l_tmp_enum_payload_project_n24;
        if (hxc_string_retain(hxc_l_path_h91a2fe8434cc) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_string hxc_l_tmp_string_concat_result_n26 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
        if (hxc_string_concat_ref((hxc_string){ (const uint8_t *)"SHA-256 receipt changed for ", 28, true, NULL }, hxc_l_path_h91a2fe8434cc, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n26) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_string_release(&hxc_l_path_h91a2fe8434cc) != HXC_STATUS_OK)
        {
          abort();
        }
        return hxc_l_tmp_string_concat_result_n26;
      }
    case hxc_caxecraft_content_CampaignLevelLoadError_CampaignLevelRuntimeRejected:
      {
        if (hxc_l_error.hxc_tag != hxc_caxecraft_content_CampaignLevelLoadError_CampaignLevelRuntimeRejected)
        {
          abort();
        }
        hxc_string hxc_l_tmp_enum_payload_project_n27 = hxc_l_error.hxc_payload.hxc_CampaignLevelRuntimeRejected.hxc_logicalPath;
        hxc_l_path_h1f0969577fa7 = hxc_l_tmp_enum_payload_project_n27;
        if (hxc_string_retain(hxc_l_path_h1f0969577fa7) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_string hxc_l_tmp_string_concat_result_n29 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
        if (hxc_string_concat_ref((hxc_string){ (const uint8_t *)"runtime validation failed for ", 30, true, NULL }, hxc_l_path_h1f0969577fa7, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n29) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_string_release(&hxc_l_path_h1f0969577fa7) != HXC_STATUS_OK)
        {
          abort();
        }
        return hxc_l_tmp_string_concat_result_n29;
      }
    default:
      {
        abort();
      }
  }
  if (hxc_l_error.hxc_tag != hxc_caxecraft_content_CampaignLevelLoadError_CampaignLevelSourceRejected)
  {
    abort();
  }
  hxc_string hxc_l_tmp_enum_payload_project_n0 = hxc_l_error.hxc_payload.hxc_CampaignLevelSourceRejected.hxc_logicalPath;
  hxc_l_path_h58ce759b9caf = hxc_l_tmp_enum_payload_project_n0;
  if (hxc_string_retain(hxc_l_path_h58ce759b9caf) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_string hxc_l_tmp_string_concat_result_n2 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
  if (hxc_string_concat_ref((hxc_string){ (const uint8_t *)"source read failed for ", 23, true, NULL }, hxc_l_path_h58ce759b9caf, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n2) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_string_release(&hxc_l_path_h58ce759b9caf) != HXC_STATUS_OK)
  {
    abort();
  }
  return hxc_l_tmp_string_concat_result_n2;
}

hxc_string hxc_caxecraft_app_CaxecraftApp_conversationSpeakerLabel(hxc_string hxc_l_id)
{
  hxc_string hxc_l_tmp_compound_string_left_owner_n15 = { 0 };
  hxc_string hxc_l_tmp_compound_string_left_owner_n16 = { 0 };
  hxc_string hxc_l_tmp_compound_string_right_owner_n20 = { 0 };
  hxc_string hxc_l_stem = { 0 };
  hxc_string hxc_l_output_b = { 0 };
  hxc_string hxc_l_source = hxc_l_id;
  if (hxc_string_retain(hxc_l_source) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_string hxc_l_tmp_load_result_n0 = hxc_l_source;
  if (hxc_l_tmp_load_result_n0.data == NULL)
  {
    abort();
  }
  if ((hxc_string){ (const uint8_t *)".", 1, true, NULL }.data == NULL)
  {
    abort();
  }
  int32_t hxc_l_tmp_string_last_index_of_result_n1;
  if (hxc_string_last_index_of(hxc_l_tmp_load_result_n0, (hxc_string){ (const uint8_t *)".", 1, true, NULL }, false, 0, &hxc_l_tmp_string_last_index_of_result_n1) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_separator = hxc_l_tmp_string_last_index_of_result_n1;
  int32_t hxc_l_tmp_load_result_n2 = hxc_l_separator;
  bool hxc_l_tmp_short_circuit_result_n4 = hxc_l_tmp_load_result_n2 >= 0;
  if (hxc_l_tmp_load_result_n2 >= 0)
  {
    int32_t hxc_l_tmp_load_result_n3 = hxc_l_separator;
    hxc_string hxc_l_tmp_load_result_n4 = hxc_l_source;
    if (hxc_l_tmp_load_result_n4.data == NULL)
    {
      abort();
    }
    int32_t hxc_l_tmp_string_length_result_n5;
    if (hxc_string_haxe_length(hxc_l_tmp_load_result_n4, &hxc_l_tmp_string_length_result_n5) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_short_circuit_result_n4 = hxc_i32_add_wrapping(hxc_l_tmp_load_result_n3, 1) < hxc_l_tmp_string_length_result_n5;
  }
  bool hxc_l_tmp_short_circuit_load_result_n6 = hxc_l_tmp_short_circuit_result_n4;
  hxc_string hxc_l_tmp_conditional_managed_result_n5 = { 0 };
  if (hxc_l_tmp_short_circuit_load_result_n6)
  {
    hxc_string hxc_l_tmp_load_result_n7 = hxc_l_source;
    if (hxc_l_tmp_load_result_n7.data == NULL)
    {
      abort();
    }
    hxc_string hxc_l_tmp_string_substring_result_n9 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_substring(hxc_l_tmp_load_result_n7, hxc_i32_add_wrapping(hxc_l_separator, 1), false, 0, &hxc_l_tmp_string_substring_result_n9) != HXC_STATUS_OK)
    {
      abort();
    }
    (void)hxc_l_tmp_string_substring_result_n9;
    hxc_string hxc_l_tmp_string_substring_owner_n6 = { 0 };
    hxc_l_tmp_string_substring_owner_n6 = hxc_l_tmp_string_substring_result_n9;
    if (hxc_string_retain(hxc_l_tmp_string_substring_owner_n6) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_string hxc_l_tmp_string_substring_owned_result_n10 = hxc_l_tmp_string_substring_owner_n6;
    (void)hxc_l_tmp_string_substring_owned_result_n10;
    hxc_l_tmp_conditional_managed_result_n5 = hxc_l_tmp_string_substring_owned_result_n10;
  }
  else
  {
    (void)hxc_l_source;
    hxc_l_tmp_conditional_managed_result_n5 = hxc_l_source;
    if (hxc_string_retain(hxc_l_tmp_conditional_managed_result_n5) != HXC_STATUS_OK)
    {
      abort();
    }
  }
  hxc_string hxc_l_tmp_conditional_managed_move_result_n12 = hxc_l_tmp_conditional_managed_result_n5;
  hxc_l_stem = hxc_l_tmp_conditional_managed_move_result_n12;
  hxc_l_output_b = (hxc_string){ (const uint8_t *)"", 0, true, NULL };
  if (hxc_string_retain(hxc_l_output_b) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_g = 0;
  hxc_string hxc_l_tmp_load_result_n13 = hxc_l_stem;
  if (hxc_l_tmp_load_result_n13.data == NULL)
  {
    abort();
  }
  int32_t hxc_l_tmp_string_length_result_n14;
  if (hxc_string_haxe_length(hxc_l_tmp_load_result_n13, &hxc_l_tmp_string_length_result_n14) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_g1 = hxc_l_tmp_string_length_result_n14;
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n15 = hxc_l_g;
    if (!(hxc_l_tmp_load_result_n15 < hxc_l_g1))
    {
      break;
    }
    int32_t hxc_l_tmp_increment_load_result_n17 = hxc_l_g;
    hxc_l_g = hxc_i32_add_wrapping(hxc_l_tmp_increment_load_result_n17, 1);
    int32_t hxc_l_index = hxc_l_tmp_increment_load_result_n17;
    hxc_string hxc_l_tmp_load_result_n18 = hxc_l_stem;
    if (hxc_l_tmp_load_result_n18.data == NULL)
    {
      abort();
    }
    struct hxc_optional_value_h76f8e80e9d2b hxc_l_tmp_string_char_code_at_result_n20 = (struct hxc_optional_value_h76f8e80e9d2b){ .hxc_has_value = false };
    hxc_l_tmp_string_char_code_at_result_n20.hxc_has_value = hxc_string_char_code_at(hxc_l_tmp_load_result_n18, hxc_l_index, &hxc_l_tmp_string_char_code_at_result_n20.hxc_value);
    struct hxc_optional_value_h76f8e80e9d2b hxc_l_code = hxc_l_tmp_string_char_code_at_result_n20;
    struct hxc_optional_value_h76f8e80e9d2b hxc_l_tmp_optional_scalar_equality_optional_n12 = hxc_l_code;
    int32_t hxc_l_tmp_optional_scalar_equality_scalar_n13 = 45;
    bool hxc_l_tmp_optional_scalar_equality_result_n14 = false;
    if (hxc_l_tmp_optional_scalar_equality_optional_n12.hxc_has_value)
    {
      struct hxc_optional_value_h76f8e80e9d2b hxc_l_tmp_optional_scalar_compare_optional_load_result_n23 = hxc_l_tmp_optional_scalar_equality_optional_n12;
      int32_t hxc_l_tmp_optional_scalar_compare_scalar_load_result_n24 = hxc_l_tmp_optional_scalar_equality_scalar_n13;
      if (!hxc_l_tmp_optional_scalar_compare_optional_load_result_n23.hxc_has_value)
      {
        abort();
      }
      hxc_l_tmp_optional_scalar_equality_result_n14 = hxc_l_tmp_optional_scalar_compare_optional_load_result_n23.hxc_value == hxc_l_tmp_optional_scalar_compare_scalar_load_result_n24;
    }
    if (hxc_l_tmp_optional_scalar_equality_result_n14)
    {
      hxc_l_tmp_compound_string_left_owner_n15 = hxc_l_output_b;
      if (hxc_string_retain(hxc_l_tmp_compound_string_left_owner_n15) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_string hxc_l_tmp_string_concat_result_n28 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
      if (hxc_string_concat_ref(hxc_l_tmp_compound_string_left_owner_n15, (hxc_string){ (const uint8_t *)" ", 1, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n28) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_output_b) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_l_output_b = hxc_l_tmp_string_concat_result_n28;
      if (hxc_string_release(&hxc_l_tmp_compound_string_left_owner_n15) != HXC_STATUS_OK)
      {
        abort();
      }
    }
    else
    {
      hxc_l_tmp_compound_string_left_owner_n16 = hxc_l_output_b;
      if (hxc_string_retain(hxc_l_tmp_compound_string_left_owner_n16) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_string hxc_l_tmp_compound_string_left_flow_n17 = hxc_l_tmp_compound_string_left_owner_n16;
      struct hxc_optional_value_h76f8e80e9d2b hxc_l_tmp_load_result_n31 = hxc_l_code;
      if (!hxc_l_tmp_load_result_n31.hxc_has_value)
      {
        abort();
      }
      bool hxc_l_tmp_short_circuit_result_n18 = hxc_l_tmp_load_result_n31.hxc_value >= 97;
      if (hxc_l_tmp_load_result_n31.hxc_value >= 97)
      {
        struct hxc_optional_value_h76f8e80e9d2b hxc_l_tmp_load_result_n33 = hxc_l_code;
        if (!hxc_l_tmp_load_result_n33.hxc_has_value)
        {
          abort();
        }
        hxc_l_tmp_short_circuit_result_n18 = hxc_l_tmp_load_result_n33.hxc_value <= 122;
      }
      bool hxc_l_tmp_short_circuit_load_result_n35 = hxc_l_tmp_short_circuit_result_n18;
      int32_t hxc_l_tmp_conditional_result_n19 = 0;
      if (hxc_l_tmp_short_circuit_load_result_n35)
      {
        struct hxc_optional_value_h76f8e80e9d2b hxc_l_tmp_load_result_n36 = hxc_l_code;
        if (!hxc_l_tmp_load_result_n36.hxc_has_value)
        {
          abort();
        }
        hxc_l_tmp_conditional_result_n19 = hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n36.hxc_value, 32);
      }
      else
      {
        struct hxc_optional_value_h76f8e80e9d2b hxc_l_tmp_load_result_n38 = hxc_l_code;
        if (!hxc_l_tmp_load_result_n38.hxc_has_value)
        {
          abort();
        }
        hxc_l_tmp_conditional_result_n19 = hxc_l_tmp_load_result_n38.hxc_value;
      }
      hxc_string hxc_l_tmp_string_from_scalar_result_n41 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
      if (hxc_string_from_scalar(hxc_l_tmp_conditional_result_n19, hxc_default_allocator(), &hxc_l_tmp_string_from_scalar_result_n41) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_l_tmp_compound_string_right_owner_n20 = hxc_l_tmp_string_from_scalar_result_n41;
      hxc_string hxc_l_tmp_compound_string_right_borrow_result_n42 = hxc_l_tmp_compound_string_right_owner_n20;
      hxc_string hxc_l_tmp_string_concat_result_n44 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
      if (hxc_string_concat_ref(hxc_l_tmp_compound_string_left_flow_n17, hxc_l_tmp_compound_string_right_borrow_result_n42, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n44) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_output_b) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_l_output_b = hxc_l_tmp_string_concat_result_n44;
      if (hxc_string_release(&hxc_l_tmp_compound_string_right_owner_n20) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_compound_string_left_owner_n16) != HXC_STATUS_OK)
      {
        abort();
      }
    }
  }
  hxc_string hxc_l_tmp_load_result_n45 = hxc_l_output_b;
  if (hxc_string_release(&hxc_l_stem) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_string_release(&hxc_l_source) != HXC_STATUS_OK)
  {
    abort();
  }
  return hxc_l_tmp_load_result_n45;
}

struct hxc_array_ref *hxc_caxecraft_app_CaxecraftApp_dialogueActorSnapshots(struct hxc_caxecraft_domain_GameSession *hxc_l_session, struct hxc_caxecraft_app_PlayableLevelView *hxc_l_level)
{
  const void *volatile hxc_l_gc_roots[2] = { NULL, NULL };
  hxc_l_gc_roots[0] = (const void *)hxc_l_session;
  hxc_l_gc_roots[1] = (const void *)hxc_l_level;
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 2, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_caxecraft_app_PlayableDialogueActor hxc_l_tmp_array_element_owner_n6 = { 0 };
  struct hxc_array_ref *hxc_l_tmp_array_create_result_n0 = NULL;
  if (hxc_array_ref_create_trivial(hxc_default_allocator(), sizeof(struct hxc_caxecraft_domain_Character), _Alignof(struct hxc_caxecraft_domain_Character), &hxc_l_tmp_array_create_result_n0) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_actors = hxc_l_tmp_array_create_result_n0;
  int32_t hxc_l_g = 0;
  if (hxc_l_level == NULL)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n1 = (*hxc_l_level).hxc_dialogueActors;
  int32_t hxc_l_tmp_array_length_result_n2;
  if (hxc_array_ref_length(hxc_l_tmp_class_field_load_result_n1, &hxc_l_tmp_array_length_result_n2) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_g1 = hxc_l_tmp_array_length_result_n2;
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n3 = hxc_l_g;
    if (!(hxc_l_tmp_load_result_n3 < hxc_l_g1))
    {
      break;
    }
    int32_t hxc_l_tmp_increment_load_result_n5 = hxc_l_g;
    hxc_l_g = hxc_i32_add_wrapping(hxc_l_tmp_increment_load_result_n5, 1);
    int32_t hxc_l_index = hxc_l_tmp_increment_load_result_n5;
    struct hxc_array_ref *hxc_l_tmp_load_result_n6 = hxc_l_actors;
    if (hxc_l_session == NULL)
    {
      abort();
    }
    struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n7 = (*hxc_l_level).hxc_dialogueActors;
    struct hxc_caxecraft_app_PlayableDialogueActor hxc_l_tmp_array_get_result_n9;
    if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n7, (size_t)hxc_l_index, &hxc_l_tmp_array_get_result_n9) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_array_element_owner_n6 = hxc_l_tmp_array_get_result_n9;
    struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n12 = hxc_caxecraft_domain_GameSession_readCharacter(hxc_l_session, hxc_l_tmp_array_element_owner_n6.hxc_entityId);
    int32_t hxc_l_tmp_array_push_result_n13;
    if (hxc_array_ref_push_copy(hxc_l_tmp_load_result_n6, &hxc_l_tmp_instance_call_result_n12, &hxc_l_tmp_array_push_result_n13) != HXC_STATUS_OK)
    {
      abort();
    }
    (void)hxc_l_tmp_array_push_result_n13;
    hxc_array_b0c1cde1_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n6);
  }
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return hxc_l_actors;
}

bool hxc_caxecraft_app_CaxecraftApp_dialogueActorsAreValid(struct hxc_caxecraft_domain_GameSession *hxc_l_session, struct hxc_caxecraft_app_PlayableLevelView *hxc_l_level, struct hxc_array_ref *hxc_l_states)
{
  const void *volatile hxc_l_gc_roots[2] = { NULL, NULL };
  hxc_l_gc_roots[0] = (const void *)hxc_l_session;
  hxc_l_gc_roots[1] = (const void *)hxc_l_level;
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 2, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_caxecraft_app_PlayableDialogueActor hxc_l_tmp_array_element_owner_n7 = { 0 };
  int32_t hxc_l_g = 0;
  if (hxc_l_level == NULL)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n0 = (*hxc_l_level).hxc_dialogueActors;
  int32_t hxc_l_tmp_array_length_result_n1;
  if (hxc_array_ref_length(hxc_l_tmp_class_field_load_result_n0, &hxc_l_tmp_array_length_result_n1) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_g1 = hxc_l_tmp_array_length_result_n1;
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n2 = hxc_l_g;
    if (!(hxc_l_tmp_load_result_n2 < hxc_l_g1))
    {
      break;
    }
    int32_t hxc_l_tmp_increment_load_result_n4 = hxc_l_g;
    hxc_l_g = hxc_i32_add_wrapping(hxc_l_tmp_increment_load_result_n4, 1);
    int32_t hxc_l_index = hxc_l_tmp_increment_load_result_n4;
    struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n5 = (*hxc_l_level).hxc_dialogueActors;
    struct hxc_caxecraft_app_PlayableDialogueActor hxc_l_tmp_array_get_result_n7;
    if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n5, (size_t)hxc_l_index, &hxc_l_tmp_array_get_result_n7) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_array_element_owner_n7 = hxc_l_tmp_array_get_result_n7;
    int32_t hxc_l_id = hxc_l_tmp_array_element_owner_n7.hxc_entityId;
    if (hxc_l_session == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n11 = hxc_caxecraft_domain_GameSession_readCharacter(hxc_l_session, hxc_l_id);
    bool hxc_l_tmp_short_circuit_result_n8 = hxc_l_tmp_instance_call_result_n11.hxc_id <= 0;
    if (!(hxc_l_tmp_instance_call_result_n11.hxc_id <= 0))
    {
      struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation hxc_l_tmp_call_result_n15 = hxc_caxecraft_app_CaxecraftApp_observeActorPhase(hxc_l_states, hxc_l_id, hxc_caxecraft_domain_ActorControllerPhase_Stationary);
      hxc_l_tmp_short_circuit_result_n8 = !hxc_l_tmp_call_result_n15.hxc_valid;
    }
    if (hxc_l_tmp_short_circuit_result_n8)
    {
      hxc_array_b0c1cde1_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n7);
      if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
      {
        abort();
      }
      return false;
    }
    hxc_array_b0c1cde1_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n7);
  }
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return true;
}

void hxc_caxecraft_app_CaxecraftApp_drawActionNotice(struct hxc_caxecraft_app_HudView hxc_l_view_h96a452c9f6a1, struct hxc_caxecraft_app_HudLayout hxc_l_layout, struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_uiCatalog, int32_t hxc_l_locale, struct Color hxc_l_color)
{
  const void *volatile hxc_l_gc_roots[20] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
  hxc_l_gc_roots[0] = (const void *)hxc_l_view_h96a452c9f6a1.hxc_presentation;
  hxc_l_gc_roots[1] = (const void *)hxc_l_uiCatalog;
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 20, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_string hxc_l_value1_h0783fb68e8f9 = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n12 = { 0 };
  hxc_string hxc_l_value1_hd6efab6a9a5a = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n16 = { 0 };
  hxc_string hxc_l_value1_h61aba56abe01 = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n20 = { 0 };
  hxc_string hxc_l_value1_h715fd969df76 = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n24 = { 0 };
  hxc_string hxc_l_value1_hfd2cbd8eb455 = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n28 = { 0 };
  hxc_string hxc_l_value1_h053b6472ba88 = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n33 = { 0 };
  hxc_string hxc_l_value1_h98f847a4e7e9 = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n37 = { 0 };
  hxc_string hxc_l_value1_hc0a7236697ec = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n41 = { 0 };
  hxc_string hxc_l_value1_he09f927f6cf1 = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n47 = { 0 };
  hxc_string hxc_l_tmp_static_call_argument_1_owner_n48 = { 0 };
  hxc_string hxc_l_tmp_instance_call_argument_1_owner_n8 = { 0 };
  hxc_string hxc_l_value = (hxc_string){ (const uint8_t *)"", 0, true, NULL };
  if (hxc_string_retain(hxc_l_value) != HXC_STATUS_OK)
  {
    abort();
  }
  struct Color hxc_l_noticeColor = hxc_l_color;
  struct hxc_caxecraft_app_HudFeedbackView hxc_l_feedback = hxc_l_view_h96a452c9f6a1.hxc_feedback;
  if (!hxc_l_view_h96a452c9f6a1.hxc_pointerCaptured)
  {
    if (hxc_l_uiCatalog == NULL)
    {
      abort();
    }
    hxc_string hxc_l_tmp_call_result_n3 = hxc_caxecraft_input_ControlPrompts_capturePromptMessage(hxc_l_view_h96a452c9f6a1.hxc_promptDevice);
    hxc_l_tmp_instance_call_argument_1_owner_n8 = hxc_l_tmp_call_result_n3;
    hxc_string hxc_l_tmp_instance_call_result_n5 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_uiCatalog, hxc_l_locale, hxc_l_tmp_instance_call_argument_1_owner_n8);
    if (hxc_string_release(&hxc_l_value) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_value = hxc_l_tmp_instance_call_result_n5;
    if (hxc_string_release(&hxc_l_tmp_instance_call_argument_1_owner_n8) != HXC_STATUS_OK)
    {
      abort();
    }
  }
  else
  {
    if (hxc_l_feedback.hxc_enemyAttacked)
    {
      hxc_l_gc_roots[2] = (const void *)hxc_l_view_h96a452c9f6a1.hxc_presentation;
      struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_view_h271804adcd60 = hxc_l_view_h96a452c9f6a1.hxc_presentation;
      hxc_string hxc_l_tmp_call_result_n8 = hxc_caxecraft_app_GameplayMessage_gameplayMessageId(15);
      hxc_l_value1_h0783fb68e8f9 = hxc_l_tmp_call_result_n8;
      hxc_string hxc_l_value2_h8d50dd99f04f = { 0 };
      switch (hxc_l_locale) {
        case 0:
          {
            hxc_l_value2_h8d50dd99f04f = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
            if (hxc_string_retain(hxc_l_value2_h8d50dd99f04f) != HXC_STATUS_OK)
            {
              abort();
            }
            break;
          }
        case 1:
          {
            hxc_l_value2_h8d50dd99f04f = (hxc_string){ (const uint8_t *)"es-mx", 5, true, NULL };
            if (hxc_string_retain(hxc_l_value2_h8d50dd99f04f) != HXC_STATUS_OK)
            {
              abort();
            }
            break;
          }
        default:
          {
            hxc_l_value2_h8d50dd99f04f = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
            if (hxc_string_retain(hxc_l_value2_h8d50dd99f04f) != HXC_STATUS_OK)
            {
              abort();
            }
            break;
          }
      }
      hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n9 = hxc_l_value2_h8d50dd99f04f;
      hxc_l_tmp_managed_flow_owner_n12 = hxc_l_tmp_managed_flow_carrier_move_result_n9;
      struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_load_result_n10 = hxc_l_view_h271804adcd60;
      hxc_l_gc_roots[3] = (const void *)hxc_l_tmp_load_result_n10;
      if (hxc_l_tmp_load_result_n10 == NULL)
      {
        abort();
      }
      hxc_string hxc_l_tmp_load_result_n11 = hxc_l_value1_h0783fb68e8f9;
      hxc_string hxc_l_tmp_instance_call_result_n13 = hxc_caxecraft_content_RuntimeLevelPresentation_message(hxc_l_tmp_load_result_n10, hxc_l_tmp_load_result_n11, hxc_l_tmp_managed_flow_owner_n12);
      if (hxc_string_release(&hxc_l_value) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_l_value = hxc_l_tmp_instance_call_result_n13;
      hxc_l_noticeColor = (struct Color){ .r = 218, .g = 65, .b = 72, .a = 255 };
      if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n12) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_value1_h0783fb68e8f9) != HXC_STATUS_OK)
      {
        abort();
      }
    }
    else
    {
      if (hxc_l_feedback.hxc_inventoryFullReason == 1)
      {
        hxc_l_gc_roots[4] = (const void *)hxc_l_view_h96a452c9f6a1.hxc_presentation;
        struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_view_h2b3faa1b2f66 = hxc_l_view_h96a452c9f6a1.hxc_presentation;
        hxc_string hxc_l_tmp_call_result_n17 = hxc_caxecraft_app_GameplayMessage_gameplayMessageId(3);
        hxc_l_value1_hd6efab6a9a5a = hxc_l_tmp_call_result_n17;
        hxc_string hxc_l_value2_h078eb2aa9fa2 = { 0 };
        switch (hxc_l_locale) {
          case 0:
            {
              hxc_l_value2_h078eb2aa9fa2 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
              if (hxc_string_retain(hxc_l_value2_h078eb2aa9fa2) != HXC_STATUS_OK)
              {
                abort();
              }
              break;
            }
          case 1:
            {
              hxc_l_value2_h078eb2aa9fa2 = (hxc_string){ (const uint8_t *)"es-mx", 5, true, NULL };
              if (hxc_string_retain(hxc_l_value2_h078eb2aa9fa2) != HXC_STATUS_OK)
              {
                abort();
              }
              break;
            }
          default:
            {
              hxc_l_value2_h078eb2aa9fa2 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
              if (hxc_string_retain(hxc_l_value2_h078eb2aa9fa2) != HXC_STATUS_OK)
              {
                abort();
              }
              break;
            }
        }
        hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n18 = hxc_l_value2_h078eb2aa9fa2;
        hxc_l_tmp_managed_flow_owner_n16 = hxc_l_tmp_managed_flow_carrier_move_result_n18;
        struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_load_result_n19 = hxc_l_view_h2b3faa1b2f66;
        hxc_l_gc_roots[5] = (const void *)hxc_l_tmp_load_result_n19;
        if (hxc_l_tmp_load_result_n19 == NULL)
        {
          abort();
        }
        hxc_string hxc_l_tmp_load_result_n20 = hxc_l_value1_hd6efab6a9a5a;
        hxc_string hxc_l_tmp_instance_call_result_n22 = hxc_caxecraft_content_RuntimeLevelPresentation_message(hxc_l_tmp_load_result_n19, hxc_l_tmp_load_result_n20, hxc_l_tmp_managed_flow_owner_n16);
        if (hxc_string_release(&hxc_l_value) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_l_value = hxc_l_tmp_instance_call_result_n22;
        hxc_l_noticeColor = (struct Color){ .r = 238, .g = 113, .b = 78, .a = 255 };
        if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n16) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_string_release(&hxc_l_value1_hd6efab6a9a5a) != HXC_STATUS_OK)
        {
          abort();
        }
      }
      else
      {
        if (hxc_l_feedback.hxc_inventoryFullReason == 2)
        {
          hxc_l_gc_roots[6] = (const void *)hxc_l_view_h96a452c9f6a1.hxc_presentation;
          struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_view_hab9758794f1b = hxc_l_view_h96a452c9f6a1.hxc_presentation;
          hxc_string hxc_l_tmp_call_result_n26 = hxc_caxecraft_app_GameplayMessage_gameplayMessageId(4);
          hxc_l_value1_h61aba56abe01 = hxc_l_tmp_call_result_n26;
          hxc_string hxc_l_value2_h176f908cb916 = { 0 };
          switch (hxc_l_locale) {
            case 0:
              {
                hxc_l_value2_h176f908cb916 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
                if (hxc_string_retain(hxc_l_value2_h176f908cb916) != HXC_STATUS_OK)
                {
                  abort();
                }
                break;
              }
            case 1:
              {
                hxc_l_value2_h176f908cb916 = (hxc_string){ (const uint8_t *)"es-mx", 5, true, NULL };
                if (hxc_string_retain(hxc_l_value2_h176f908cb916) != HXC_STATUS_OK)
                {
                  abort();
                }
                break;
              }
            default:
              {
                hxc_l_value2_h176f908cb916 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
                if (hxc_string_retain(hxc_l_value2_h176f908cb916) != HXC_STATUS_OK)
                {
                  abort();
                }
                break;
              }
          }
          hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n27 = hxc_l_value2_h176f908cb916;
          hxc_l_tmp_managed_flow_owner_n20 = hxc_l_tmp_managed_flow_carrier_move_result_n27;
          struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_load_result_n28 = hxc_l_view_hab9758794f1b;
          hxc_l_gc_roots[7] = (const void *)hxc_l_tmp_load_result_n28;
          if (hxc_l_tmp_load_result_n28 == NULL)
          {
            abort();
          }
          hxc_string hxc_l_tmp_load_result_n29 = hxc_l_value1_h61aba56abe01;
          hxc_string hxc_l_tmp_instance_call_result_n31 = hxc_caxecraft_content_RuntimeLevelPresentation_message(hxc_l_tmp_load_result_n28, hxc_l_tmp_load_result_n29, hxc_l_tmp_managed_flow_owner_n20);
          if (hxc_string_release(&hxc_l_value) != HXC_STATUS_OK)
          {
            abort();
          }
          hxc_l_value = hxc_l_tmp_instance_call_result_n31;
          hxc_l_noticeColor = (struct Color){ .r = 238, .g = 113, .b = 78, .a = 255 };
          if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n20) != HXC_STATUS_OK)
          {
            abort();
          }
          if (hxc_string_release(&hxc_l_value1_h61aba56abe01) != HXC_STATUS_OK)
          {
            abort();
          }
        }
        else
        {
          if (hxc_l_feedback.hxc_placementBlocked)
          {
            if (hxc_l_uiCatalog == NULL)
            {
              abort();
            }
            hxc_string hxc_l_tmp_instance_call_result_n34 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_uiCatalog, hxc_l_locale, (hxc_string){ (const uint8_t *)"place_blocked", 13, true, NULL });
            if (hxc_string_release(&hxc_l_value) != HXC_STATUS_OK)
            {
              abort();
            }
            hxc_l_value = hxc_l_tmp_instance_call_result_n34;
          }
          else
          {
            if (hxc_l_feedback.hxc_enemyDefeated)
            {
              hxc_l_gc_roots[8] = (const void *)hxc_l_view_h96a452c9f6a1.hxc_presentation;
              struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_view_hf9e0d7544eed = hxc_l_view_h96a452c9f6a1.hxc_presentation;
              hxc_string hxc_l_tmp_call_result_n37 = hxc_caxecraft_app_GameplayMessage_gameplayMessageId(8);
              hxc_l_value1_h715fd969df76 = hxc_l_tmp_call_result_n37;
              hxc_string hxc_l_value2_ha9b3052d9316 = { 0 };
              switch (hxc_l_locale) {
                case 0:
                  {
                    hxc_l_value2_ha9b3052d9316 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
                    if (hxc_string_retain(hxc_l_value2_ha9b3052d9316) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    break;
                  }
                case 1:
                  {
                    hxc_l_value2_ha9b3052d9316 = (hxc_string){ (const uint8_t *)"es-mx", 5, true, NULL };
                    if (hxc_string_retain(hxc_l_value2_ha9b3052d9316) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    break;
                  }
                default:
                  {
                    hxc_l_value2_ha9b3052d9316 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
                    if (hxc_string_retain(hxc_l_value2_ha9b3052d9316) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    break;
                  }
              }
              hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n38 = hxc_l_value2_ha9b3052d9316;
              hxc_l_tmp_managed_flow_owner_n24 = hxc_l_tmp_managed_flow_carrier_move_result_n38;
              struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_load_result_n39 = hxc_l_view_hf9e0d7544eed;
              hxc_l_gc_roots[9] = (const void *)hxc_l_tmp_load_result_n39;
              if (hxc_l_tmp_load_result_n39 == NULL)
              {
                abort();
              }
              hxc_string hxc_l_tmp_load_result_n40 = hxc_l_value1_h715fd969df76;
              hxc_string hxc_l_tmp_instance_call_result_n42 = hxc_caxecraft_content_RuntimeLevelPresentation_message(hxc_l_tmp_load_result_n39, hxc_l_tmp_load_result_n40, hxc_l_tmp_managed_flow_owner_n24);
              if (hxc_string_release(&hxc_l_value) != HXC_STATUS_OK)
              {
                abort();
              }
              hxc_l_value = hxc_l_tmp_instance_call_result_n42;
              if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n24) != HXC_STATUS_OK)
              {
                abort();
              }
              if (hxc_string_release(&hxc_l_value1_h715fd969df76) != HXC_STATUS_OK)
              {
                abort();
              }
            }
            else
            {
              if (hxc_l_feedback.hxc_strikeHit)
              {
                hxc_l_gc_roots[10] = (const void *)hxc_l_view_h96a452c9f6a1.hxc_presentation;
                struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_view_hc5fe71c5c9c8 = hxc_l_view_h96a452c9f6a1.hxc_presentation;
                hxc_string hxc_l_tmp_call_result_n45 = hxc_caxecraft_app_GameplayMessage_gameplayMessageId(5);
                hxc_l_value1_hfd2cbd8eb455 = hxc_l_tmp_call_result_n45;
                hxc_string hxc_l_value2_hc56a34775eee = { 0 };
                switch (hxc_l_locale) {
                  case 0:
                    {
                      hxc_l_value2_hc56a34775eee = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
                      if (hxc_string_retain(hxc_l_value2_hc56a34775eee) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      break;
                    }
                  case 1:
                    {
                      hxc_l_value2_hc56a34775eee = (hxc_string){ (const uint8_t *)"es-mx", 5, true, NULL };
                      if (hxc_string_retain(hxc_l_value2_hc56a34775eee) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      break;
                    }
                  default:
                    {
                      hxc_l_value2_hc56a34775eee = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
                      if (hxc_string_retain(hxc_l_value2_hc56a34775eee) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      break;
                    }
                }
                hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n46 = hxc_l_value2_hc56a34775eee;
                hxc_l_tmp_managed_flow_owner_n28 = hxc_l_tmp_managed_flow_carrier_move_result_n46;
                struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_load_result_n47 = hxc_l_view_hc5fe71c5c9c8;
                hxc_l_gc_roots[11] = (const void *)hxc_l_tmp_load_result_n47;
                if (hxc_l_tmp_load_result_n47 == NULL)
                {
                  abort();
                }
                hxc_string hxc_l_tmp_load_result_n48 = hxc_l_value1_hfd2cbd8eb455;
                hxc_string hxc_l_tmp_instance_call_result_n50 = hxc_caxecraft_content_RuntimeLevelPresentation_message(hxc_l_tmp_load_result_n47, hxc_l_tmp_load_result_n48, hxc_l_tmp_managed_flow_owner_n28);
                if (hxc_string_release(&hxc_l_value) != HXC_STATUS_OK)
                {
                  abort();
                }
                hxc_l_value = hxc_l_tmp_instance_call_result_n50;
                if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n28) != HXC_STATUS_OK)
                {
                  abort();
                }
                if (hxc_string_release(&hxc_l_value1_hfd2cbd8eb455) != HXC_STATUS_OK)
                {
                  abort();
                }
              }
              else
              {
                if (hxc_l_feedback.hxc_pickedUp)
                {
                  hxc_l_gc_roots[12] = (const void *)hxc_l_view_h96a452c9f6a1.hxc_presentation;
                  struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_view_h50f9c97591dc = hxc_l_view_h96a452c9f6a1.hxc_presentation;
                  int32_t hxc_l_tmp_record_field_load_result_n53 = hxc_l_feedback.hxc_pickupAmount;
                  int32_t hxc_l_tmp_conditional_result_n31 = 0;
                  if (hxc_l_tmp_record_field_load_result_n53 == 1)
                  {
                    hxc_l_tmp_conditional_result_n31 = 0;
                  }
                  else
                  {
                    hxc_l_tmp_conditional_result_n31 = 1;
                  }
                  hxc_string hxc_l_tmp_call_result_n55 = hxc_caxecraft_app_GameplayMessage_gameplayMessageId(hxc_l_tmp_conditional_result_n31);
                  hxc_l_value1_h053b6472ba88 = hxc_l_tmp_call_result_n55;
                  hxc_string hxc_l_value2_h044032655968 = { 0 };
                  switch (hxc_l_locale) {
                    case 0:
                      {
                        hxc_l_value2_h044032655968 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
                        if (hxc_string_retain(hxc_l_value2_h044032655968) != HXC_STATUS_OK)
                        {
                          abort();
                        }
                        break;
                      }
                    case 1:
                      {
                        hxc_l_value2_h044032655968 = (hxc_string){ (const uint8_t *)"es-mx", 5, true, NULL };
                        if (hxc_string_retain(hxc_l_value2_h044032655968) != HXC_STATUS_OK)
                        {
                          abort();
                        }
                        break;
                      }
                    default:
                      {
                        hxc_l_value2_h044032655968 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
                        if (hxc_string_retain(hxc_l_value2_h044032655968) != HXC_STATUS_OK)
                        {
                          abort();
                        }
                        break;
                      }
                  }
                  hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n56 = hxc_l_value2_h044032655968;
                  hxc_l_tmp_managed_flow_owner_n33 = hxc_l_tmp_managed_flow_carrier_move_result_n56;
                  struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_load_result_n57 = hxc_l_view_h50f9c97591dc;
                  hxc_l_gc_roots[13] = (const void *)hxc_l_tmp_load_result_n57;
                  if (hxc_l_tmp_load_result_n57 == NULL)
                  {
                    abort();
                  }
                  hxc_string hxc_l_tmp_load_result_n58 = hxc_l_value1_h053b6472ba88;
                  hxc_string hxc_l_tmp_instance_call_result_n60 = hxc_caxecraft_content_RuntimeLevelPresentation_message(hxc_l_tmp_load_result_n57, hxc_l_tmp_load_result_n58, hxc_l_tmp_managed_flow_owner_n33);
                  if (hxc_string_release(&hxc_l_value) != HXC_STATUS_OK)
                  {
                    abort();
                  }
                  hxc_l_value = hxc_l_tmp_instance_call_result_n60;
                  if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n33) != HXC_STATUS_OK)
                  {
                    abort();
                  }
                  if (hxc_string_release(&hxc_l_value1_h053b6472ba88) != HXC_STATUS_OK)
                  {
                    abort();
                  }
                }
                else
                {
                  if (hxc_l_feedback.hxc_recoveryVisible)
                  {
                    if (hxc_l_feedback.hxc_recoveryDecision == 1)
                    {
                      hxc_l_gc_roots[14] = (const void *)hxc_l_view_h96a452c9f6a1.hxc_presentation;
                      struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_view_h609f067f44ec = hxc_l_view_h96a452c9f6a1.hxc_presentation;
                      hxc_string hxc_l_tmp_call_result_n64 = hxc_caxecraft_app_GameplayMessage_gameplayMessageId(2);
                      hxc_l_value1_h98f847a4e7e9 = hxc_l_tmp_call_result_n64;
                      hxc_string hxc_l_value2_h4a0b1fcf2ba1 = { 0 };
                      switch (hxc_l_locale) {
                        case 0:
                          {
                            hxc_l_value2_h4a0b1fcf2ba1 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
                            if (hxc_string_retain(hxc_l_value2_h4a0b1fcf2ba1) != HXC_STATUS_OK)
                            {
                              abort();
                            }
                            break;
                          }
                        case 1:
                          {
                            hxc_l_value2_h4a0b1fcf2ba1 = (hxc_string){ (const uint8_t *)"es-mx", 5, true, NULL };
                            if (hxc_string_retain(hxc_l_value2_h4a0b1fcf2ba1) != HXC_STATUS_OK)
                            {
                              abort();
                            }
                            break;
                          }
                        default:
                          {
                            hxc_l_value2_h4a0b1fcf2ba1 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
                            if (hxc_string_retain(hxc_l_value2_h4a0b1fcf2ba1) != HXC_STATUS_OK)
                            {
                              abort();
                            }
                            break;
                          }
                      }
                      hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n65 = hxc_l_value2_h4a0b1fcf2ba1;
                      hxc_l_tmp_managed_flow_owner_n37 = hxc_l_tmp_managed_flow_carrier_move_result_n65;
                      struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_load_result_n66 = hxc_l_view_h609f067f44ec;
                      hxc_l_gc_roots[15] = (const void *)hxc_l_tmp_load_result_n66;
                      if (hxc_l_tmp_load_result_n66 == NULL)
                      {
                        abort();
                      }
                      hxc_string hxc_l_tmp_load_result_n67 = hxc_l_value1_h98f847a4e7e9;
                      hxc_string hxc_l_tmp_instance_call_result_n69 = hxc_caxecraft_content_RuntimeLevelPresentation_message(hxc_l_tmp_load_result_n66, hxc_l_tmp_load_result_n67, hxc_l_tmp_managed_flow_owner_n37);
                      if (hxc_string_release(&hxc_l_value) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      hxc_l_value = hxc_l_tmp_instance_call_result_n69;
                      if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n37) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      if (hxc_string_release(&hxc_l_value1_h98f847a4e7e9) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                    }
                    else
                    {
                      if (hxc_l_feedback.hxc_recoveryDecision == 2)
                      {
                        if (hxc_l_uiCatalog == NULL)
                        {
                          abort();
                        }
                        hxc_string hxc_l_tmp_instance_call_result_n71 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_uiCatalog, hxc_l_locale, (hxc_string){ (const uint8_t *)"health_full", 11, true, NULL });
                        if (hxc_string_release(&hxc_l_value) != HXC_STATUS_OK)
                        {
                          abort();
                        }
                        hxc_l_value = hxc_l_tmp_instance_call_result_n71;
                      }
                      else
                      {
                        if (hxc_l_feedback.hxc_recoveryDecision == 3)
                        {
                          hxc_l_gc_roots[16] = (const void *)hxc_l_view_h96a452c9f6a1.hxc_presentation;
                          struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_view_h1b2d357ab30c = hxc_l_view_h96a452c9f6a1.hxc_presentation;
                          hxc_string hxc_l_tmp_call_result_n74 = hxc_caxecraft_app_GameplayMessage_gameplayMessageId(13);
                          hxc_l_value1_hc0a7236697ec = hxc_l_tmp_call_result_n74;
                          hxc_string hxc_l_value2_h1ae78524edac = { 0 };
                          switch (hxc_l_locale) {
                            case 0:
                              {
                                hxc_l_value2_h1ae78524edac = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
                                if (hxc_string_retain(hxc_l_value2_h1ae78524edac) != HXC_STATUS_OK)
                                {
                                  abort();
                                }
                                break;
                              }
                            case 1:
                              {
                                hxc_l_value2_h1ae78524edac = (hxc_string){ (const uint8_t *)"es-mx", 5, true, NULL };
                                if (hxc_string_retain(hxc_l_value2_h1ae78524edac) != HXC_STATUS_OK)
                                {
                                  abort();
                                }
                                break;
                              }
                            default:
                              {
                                hxc_l_value2_h1ae78524edac = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
                                if (hxc_string_retain(hxc_l_value2_h1ae78524edac) != HXC_STATUS_OK)
                                {
                                  abort();
                                }
                                break;
                              }
                          }
                          hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n75 = hxc_l_value2_h1ae78524edac;
                          hxc_l_tmp_managed_flow_owner_n41 = hxc_l_tmp_managed_flow_carrier_move_result_n75;
                          struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_load_result_n76 = hxc_l_view_h1b2d357ab30c;
                          hxc_l_gc_roots[17] = (const void *)hxc_l_tmp_load_result_n76;
                          if (hxc_l_tmp_load_result_n76 == NULL)
                          {
                            abort();
                          }
                          hxc_string hxc_l_tmp_load_result_n77 = hxc_l_value1_hc0a7236697ec;
                          hxc_string hxc_l_tmp_instance_call_result_n79 = hxc_caxecraft_content_RuntimeLevelPresentation_message(hxc_l_tmp_load_result_n76, hxc_l_tmp_load_result_n77, hxc_l_tmp_managed_flow_owner_n41);
                          if (hxc_string_release(&hxc_l_value) != HXC_STATUS_OK)
                          {
                            abort();
                          }
                          hxc_l_value = hxc_l_tmp_instance_call_result_n79;
                          if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n41) != HXC_STATUS_OK)
                          {
                            abort();
                          }
                          if (hxc_string_release(&hxc_l_value1_hc0a7236697ec) != HXC_STATUS_OK)
                          {
                            abort();
                          }
                        }
                      }
                    }
                  }
                  else
                  {
                    if (hxc_l_view_h96a452c9f6a1.hxc_character.hxc_aquaticEquipmentVisible)
                    {
                      if (hxc_l_uiCatalog == NULL)
                      {
                        abort();
                      }
                      hxc_string hxc_l_tmp_instance_call_result_n82 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_uiCatalog, hxc_l_locale, (hxc_string){ (const uint8_t *)"aquatic_gear_equipped", 21, true, NULL });
                      if (hxc_string_release(&hxc_l_value) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      hxc_l_value = hxc_l_tmp_instance_call_result_n82;
                    }
                    else
                    {
                      if (hxc_l_view_h96a452c9f6a1.hxc_interactionPrompt != hxc_caxecraft_app_InteractionPrompt_NoInteractionPrompt)
                      {
                        int32_t hxc_l_tmp_conditional_result_n43 = 0;
                        if (hxc_l_view_h96a452c9f6a1.hxc_interactionPrompt == hxc_caxecraft_app_InteractionPrompt_TalkInteractionPrompt)
                        {
                          hxc_l_tmp_conditional_result_n43 = 11;
                        }
                        else
                        {
                          hxc_l_tmp_conditional_result_n43 = 12;
                        }
                        int32_t hxc_l_message = hxc_l_tmp_conditional_result_n43;
                        hxc_l_gc_roots[18] = (const void *)hxc_l_view_h96a452c9f6a1.hxc_presentation;
                        struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_view1 = hxc_l_view_h96a452c9f6a1.hxc_presentation;
                        hxc_string hxc_l_tmp_call_result_n90 = hxc_caxecraft_app_GameplayMessage_gameplayMessageId(hxc_l_message);
                        hxc_l_value1_he09f927f6cf1 = hxc_l_tmp_call_result_n90;
                        hxc_string hxc_l_value2_h194337cf1b6f = { 0 };
                        switch (hxc_l_locale) {
                          case 0:
                            {
                              hxc_l_value2_h194337cf1b6f = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
                              if (hxc_string_retain(hxc_l_value2_h194337cf1b6f) != HXC_STATUS_OK)
                              {
                                abort();
                              }
                              break;
                            }
                          case 1:
                            {
                              hxc_l_value2_h194337cf1b6f = (hxc_string){ (const uint8_t *)"es-mx", 5, true, NULL };
                              if (hxc_string_retain(hxc_l_value2_h194337cf1b6f) != HXC_STATUS_OK)
                              {
                                abort();
                              }
                              break;
                            }
                          default:
                            {
                              hxc_l_value2_h194337cf1b6f = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
                              if (hxc_string_retain(hxc_l_value2_h194337cf1b6f) != HXC_STATUS_OK)
                              {
                                abort();
                              }
                              break;
                            }
                        }
                        hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n91 = hxc_l_value2_h194337cf1b6f;
                        hxc_l_tmp_managed_flow_owner_n47 = hxc_l_tmp_managed_flow_carrier_move_result_n91;
                        struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_load_result_n92 = hxc_l_view1;
                        hxc_l_gc_roots[19] = (const void *)hxc_l_tmp_load_result_n92;
                        if (hxc_l_tmp_load_result_n92 == NULL)
                        {
                          abort();
                        }
                        hxc_string hxc_l_tmp_load_result_n93 = hxc_l_value1_he09f927f6cf1;
                        hxc_string hxc_l_tmp_instance_call_result_n95 = hxc_caxecraft_content_RuntimeLevelPresentation_message(hxc_l_tmp_load_result_n92, hxc_l_tmp_load_result_n93, hxc_l_tmp_managed_flow_owner_n47);
                        if (hxc_string_release(&hxc_l_value) != HXC_STATUS_OK)
                        {
                          abort();
                        }
                        hxc_l_value = hxc_l_tmp_instance_call_result_n95;
                        if (hxc_l_view_h96a452c9f6a1.hxc_promptDevice == 1)
                        {
                          hxc_string hxc_l_tmp_load_result_n97 = hxc_l_value;
                          if (hxc_l_uiCatalog == NULL)
                          {
                            abort();
                          }
                          hxc_string hxc_l_tmp_instance_call_result_n98 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_uiCatalog, hxc_l_locale, (hxc_string){ (const uint8_t *)"interaction_control_gamepad", 27, true, NULL });
                          hxc_l_tmp_static_call_argument_1_owner_n48 = hxc_l_tmp_instance_call_result_n98;
                          hxc_string hxc_l_tmp_call_result_n100 = hxc_caxecraft_input_ControlPrompts_gamepadInteractionPrompt(hxc_l_tmp_load_result_n97, hxc_l_tmp_static_call_argument_1_owner_n48);
                          if (hxc_string_release(&hxc_l_value) != HXC_STATUS_OK)
                          {
                            abort();
                          }
                          hxc_l_value = hxc_l_tmp_call_result_n100;
                          if (hxc_string_release(&hxc_l_tmp_static_call_argument_1_owner_n48) != HXC_STATUS_OK)
                          {
                            abort();
                          }
                        }
                        if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n47) != HXC_STATUS_OK)
                        {
                          abort();
                        }
                        if (hxc_string_release(&hxc_l_value1_he09f927f6cf1) != HXC_STATUS_OK)
                        {
                          abort();
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  hxc_string hxc_l_tmp_load_result_n101 = hxc_l_value;
  if (hxc_l_tmp_load_result_n101.data == NULL)
  {
    abort();
  }
  int32_t hxc_l_tmp_string_length_result_n102;
  if (hxc_string_haxe_length(hxc_l_tmp_load_result_n101, &hxc_l_tmp_string_length_result_n102) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_l_tmp_string_length_result_n102 > 0)
  {
    struct hxc_caxecraft_app_HudRectangle hxc_l_tmp_static_call_argument_0_n49 = hxc_l_layout.hxc_action;
    hxc_string hxc_l_tmp_static_call_argument_1_n50 = hxc_l_value;
    int32_t hxc_l_tmp_conditional_result_n51 = 0;
    if (hxc_l_layout.hxc_compact)
    {
      hxc_l_tmp_conditional_result_n51 = 13;
    }
    else
    {
      hxc_l_tmp_conditional_result_n51 = 15;
    }
    int32_t hxc_l_tmp_static_call_argument_2_n52 = hxc_l_tmp_conditional_result_n51;
    struct Color hxc_l_tmp_static_call_argument_3_n53 = hxc_l_noticeColor;
    struct Color hxc_l_tmp_static_call_argument_4_n54 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
    int32_t hxc_l_tmp_conditional_result_n55 = 0;
    if (hxc_l_layout.hxc_compact)
    {
      hxc_l_tmp_conditional_result_n55 = 31;
    }
    else
    {
      hxc_l_tmp_conditional_result_n55 = 38;
    }
    int32_t hxc_l_tmp_conditional_load_result_n110 = hxc_l_tmp_conditional_result_n55;
    struct hxc_caxecraft_app_HudRectangle hxc_l_tmp_static_call_argument_0_load_result_n111 = hxc_l_tmp_static_call_argument_0_n49;
    hxc_string hxc_l_tmp_static_call_argument_1_load_result_n112 = hxc_l_tmp_static_call_argument_1_n50;
    int32_t hxc_l_tmp_static_call_argument_2_load_result_n113 = hxc_l_tmp_static_call_argument_2_n52;
    struct Color hxc_l_tmp_static_call_argument_3_load_result_n114 = hxc_l_tmp_static_call_argument_3_n53;
    hxc_caxecraft_app_CaxecraftApp_drawNoticePanel(hxc_l_tmp_static_call_argument_0_load_result_n111, hxc_l_tmp_static_call_argument_1_load_result_n112, hxc_l_tmp_static_call_argument_2_load_result_n113, hxc_l_tmp_static_call_argument_3_load_result_n114, hxc_l_tmp_static_call_argument_4_n54, hxc_l_tmp_conditional_load_result_n110);
  }
  if (hxc_string_release(&hxc_l_value) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return;
}

bool hxc_caxecraft_app_CaxecraftApp_drawActorPresentation(struct Camera3D hxc_l_camera, hxc_string hxc_l_asset, int32_t hxc_l_cellIndex, struct Vector3 hxc_l_position, double hxc_l_width, double hxc_l_height, struct Texture hxc_l_entityTexture, bool hxc_l_entityTextureReady, struct hxc_caxecraft_app_RuntimeTextureAtlasCatalog *hxc_l_runtimeTextures)
{
  const void *volatile hxc_l_gc_roots[1] = { NULL };
  hxc_l_gc_roots[0] = (const void *)hxc_l_runtimeTextures;
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 1, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  bool hxc_l_tmp_string_equality_result_n9 = hxc_l_asset.data != NULL && (hxc_l_asset.byte_length == (hxc_string){ (const uint8_t *)"entities", 8, true, NULL }.byte_length && (hxc_l_asset.byte_length == 0 || memcmp(hxc_l_asset.data, (hxc_string){ (const uint8_t *)"entities", 8, true, NULL }.data, hxc_l_asset.byte_length) == 0));
  bool hxc_l_tmp_string_equality_result_result_n1 = hxc_l_tmp_string_equality_result_n9;
  bool hxc_l_tmp_short_circuit_result_n10 = hxc_l_tmp_string_equality_result_result_n1;
  if (hxc_l_tmp_string_equality_result_result_n1)
  {
    hxc_l_tmp_short_circuit_result_n10 = hxc_l_entityTextureReady;
  }
  if (!hxc_l_tmp_short_circuit_result_n10)
  {
    if (hxc_l_runtimeTextures == NULL)
    {
      abort();
    }
    bool hxc_l_tmp_instance_call_result_n3 = hxc_caxecraft_app_RuntimeTextureAtlasCatalog_drawSprite(hxc_l_runtimeTextures, hxc_l_camera, hxc_l_asset, hxc_l_cellIndex, hxc_l_position, hxc_l_width, hxc_l_height);
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return hxc_l_tmp_instance_call_result_n3;
  }
  hxc_caxecraft_app_CaxecraftAtlas_drawEntitySprite(hxc_l_camera, hxc_l_entityTexture, hxc_l_cellIndex, hxc_l_position, hxc_l_width, hxc_l_height);
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return true;
}

void hxc_caxecraft_app_CaxecraftApp_drawActors(struct Camera3D hxc_l_camera, struct Texture hxc_l_entityTexture, bool hxc_l_entityTextureReady, struct hxc_caxecraft_app_RuntimeTextureAtlasCatalog *hxc_l_runtimeTextures, struct hxc_array_ref *hxc_l_dialogueActors, struct hxc_caxecraft_app_PlayableLevelView *hxc_l_level, struct hxc_caxecraft_domain_Character hxc_l_enemy, hxc_string hxc_l_enemyAsset, int32_t hxc_l_enemyCell, enum hxc_caxecraft_domain_ActorControllerPhase hxc_l_enemyPhase, struct hxc_caxecraft_gameplay_BerryDropState hxc_l_berryDrop)
{
  const void *volatile hxc_l_gc_roots[2] = { NULL, NULL };
  hxc_l_gc_roots[0] = (const void *)hxc_l_runtimeTextures;
  hxc_l_gc_roots[1] = (const void *)hxc_l_level;
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 2, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_caxecraft_app_PlayableDialogueActor hxc_l_tmp_array_element_owner_n18 = { 0 };
  struct hxc_caxecraft_app_PlayableDialogueActor hxc_l_tmp_array_element_owner_n19 = { 0 };
  int32_t hxc_l_g = 0;
  if (hxc_l_level == NULL)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n0 = (*hxc_l_level).hxc_dialogueActors;
  int32_t hxc_l_tmp_array_length_result_n1;
  if (hxc_array_ref_length(hxc_l_tmp_class_field_load_result_n0, &hxc_l_tmp_array_length_result_n1) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_g1 = hxc_l_tmp_array_length_result_n1;
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n2 = hxc_l_g;
    if (!(hxc_l_tmp_load_result_n2 < hxc_l_g1))
    {
      break;
    }
    int32_t hxc_l_tmp_increment_load_result_n4 = hxc_l_g;
    hxc_l_g = hxc_i32_add_wrapping(hxc_l_tmp_increment_load_result_n4, 1);
    int32_t hxc_l_index = hxc_l_tmp_increment_load_result_n4;
    struct hxc_caxecraft_domain_Character hxc_l_tmp_array_get_result_n6;
    if (hxc_array_ref_get_copy(hxc_l_dialogueActors, (size_t)hxc_l_index, &hxc_l_tmp_array_get_result_n6) != HXC_STATUS_OK)
    {
      abort();
    }
    struct hxc_caxecraft_domain_Character hxc_l_actor = hxc_l_tmp_array_get_result_n6;
    double hxc_l_y_hb91fc71ee47d = hxc_l_actor.hxc_body.hxc_y + 0.76;
    double hxc_l_z_h5235e2136ed8 = hxc_l_actor.hxc_body.hxc_z;
    double hxc_l_tmp_record_field_load_result_n9 = hxc_l_actor.hxc_body.hxc_x;
    double hxc_l_tmp_load_result_n10 = hxc_l_y_hb91fc71ee47d;
    struct Vector3 hxc_l_position = (struct Vector3){ .x = (float)hxc_l_tmp_record_field_load_result_n9, .y = (float)hxc_l_tmp_load_result_n10, .z = (float)hxc_l_z_h5235e2136ed8 };
    struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n13 = (*hxc_l_level).hxc_dialogueActors;
    struct hxc_caxecraft_app_PlayableDialogueActor hxc_l_tmp_array_get_result_n15;
    if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n13, (size_t)hxc_l_index, &hxc_l_tmp_array_get_result_n15) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_array_element_owner_n18 = hxc_l_tmp_array_get_result_n15;
    struct hxc_caxecraft_app_PlayableDialogueActor hxc_l_tmp_array_element_borrow_result_n16 = hxc_l_tmp_array_element_owner_n18;
    struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n18 = (*hxc_l_level).hxc_dialogueActors;
    struct hxc_caxecraft_app_PlayableDialogueActor hxc_l_tmp_array_get_result_n20;
    if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n18, (size_t)hxc_l_index, &hxc_l_tmp_array_get_result_n20) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_array_element_owner_n19 = hxc_l_tmp_array_get_result_n20;
    struct hxc_caxecraft_app_PlayableDialogueActor hxc_l_tmp_array_element_borrow_result_n21 = hxc_l_tmp_array_element_owner_n19;
    bool hxc_l_tmp_call_result_n24 = hxc_caxecraft_app_CaxecraftApp_drawActorPresentation(hxc_l_camera, hxc_l_tmp_array_element_borrow_result_n16.hxc_presentationAsset, hxc_l_tmp_array_element_borrow_result_n21.hxc_presentationCellIndex, hxc_l_position, 0.95, 1.52, hxc_l_entityTexture, hxc_l_entityTextureReady, hxc_l_runtimeTextures);
    hxc_array_b0c1cde1_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n19);
    hxc_array_b0c1cde1_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n18);
    if (!hxc_l_tmp_call_result_n24)
    {
      double hxc_l_y_he268082b663b = hxc_l_actor.hxc_body.hxc_y + 0.54;
      double hxc_l_z_hd933ca46fb1e = hxc_l_actor.hxc_body.hxc_z;
      double hxc_l_tmp_record_field_load_result_n27 = hxc_l_actor.hxc_body.hxc_x;
      double hxc_l_tmp_load_result_n28 = hxc_l_y_he268082b663b;
      DrawCube((struct Vector3){ .x = (float)hxc_l_tmp_record_field_load_result_n27, .y = (float)hxc_l_tmp_load_result_n28, .z = (float)hxc_l_z_hd933ca46fb1e }, (float)0.50, (float)0.86, (float)0.42, (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 });
      double hxc_l_y_hd3b212f3bfc1 = hxc_l_actor.hxc_body.hxc_y + 1.18;
      double hxc_l_z_h456b1d7933d9 = hxc_l_actor.hxc_body.hxc_z;
      double hxc_l_tmp_record_field_load_result_n34 = hxc_l_actor.hxc_body.hxc_x;
      double hxc_l_tmp_load_result_n35 = hxc_l_y_hd3b212f3bfc1;
      DrawCube((struct Vector3){ .x = (float)hxc_l_tmp_record_field_load_result_n34, .y = (float)hxc_l_tmp_load_result_n35, .z = (float)hxc_l_z_h456b1d7933d9 }, (float)0.44, (float)0.44, (float)0.44, (struct Color){ .r = 228, .g = 239, .b = 227, .a = 255 });
      double hxc_l_y_h95e966b21144 = hxc_l_actor.hxc_body.hxc_y + 1.41;
      double hxc_l_z_h899701fa88e9 = hxc_l_actor.hxc_body.hxc_z;
      double hxc_l_tmp_record_field_load_result_n41 = hxc_l_actor.hxc_body.hxc_x;
      double hxc_l_tmp_load_result_n42 = hxc_l_y_h95e966b21144;
      DrawCube((struct Vector3){ .x = (float)hxc_l_tmp_record_field_load_result_n41, .y = (float)hxc_l_tmp_load_result_n42, .z = (float)hxc_l_z_h899701fa88e9 }, (float)0.48, (float)0.16, (float)0.48, (struct Color){ .r = 15, .g = 27, .b = 39, .a = 220 });
    }
  }
  if (hxc_l_enemy.hxc_vitals.hxc_health > 0)
  {
    bool hxc_l_tmp_short_circuit_result_n27 = hxc_l_enemyPhase == hxc_caxecraft_domain_ActorControllerPhase_Roaring;
    if (!(hxc_l_enemyPhase == hxc_caxecraft_domain_ActorControllerPhase_Roaring))
    {
      hxc_l_tmp_short_circuit_result_n27 = hxc_l_enemyPhase == hxc_caxecraft_domain_ActorControllerPhase_Charging;
    }
    bool hxc_l_tmp_short_circuit_load_result_n50 = hxc_l_tmp_short_circuit_result_n27;
    bool hxc_l_tmp_short_circuit_result_n28 = hxc_l_tmp_short_circuit_load_result_n50;
    if (!hxc_l_tmp_short_circuit_load_result_n50)
    {
      hxc_l_tmp_short_circuit_result_n28 = hxc_l_enemyPhase == hxc_caxecraft_domain_ActorControllerPhase_TailSweep;
    }
    bool hxc_l_tmp_short_circuit_load_result_n52 = hxc_l_tmp_short_circuit_result_n28;
    bool hxc_l_tmp_short_circuit_result_n29 = hxc_l_tmp_short_circuit_load_result_n52;
    if (!hxc_l_tmp_short_circuit_load_result_n52)
    {
      hxc_l_tmp_short_circuit_result_n29 = hxc_l_enemyPhase == hxc_caxecraft_domain_ActorControllerPhase_Stunned;
    }
    bool hxc_l_largeEncounter = hxc_l_tmp_short_circuit_result_n29;
    int32_t hxc_l_phaseCell = { 0 };
    switch (hxc_l_enemyPhase) {
      case hxc_caxecraft_domain_ActorControllerPhase_Roaring:
        {
          hxc_l_phaseCell = hxc_i32_add_wrapping(hxc_l_enemyCell, 4);
          break;
        }
      case hxc_caxecraft_domain_ActorControllerPhase_Charging:
        {
          hxc_l_phaseCell = hxc_i32_add_wrapping(hxc_l_enemyCell, 5);
          break;
        }
      case hxc_caxecraft_domain_ActorControllerPhase_TailSweep:
        {
          hxc_l_phaseCell = hxc_i32_add_wrapping(hxc_l_enemyCell, 6);
          break;
        }
      case hxc_caxecraft_domain_ActorControllerPhase_Stunned:
        {
          hxc_l_phaseCell = hxc_i32_add_wrapping(hxc_l_enemyCell, 7);
          break;
        }
      default:
        {
          hxc_l_phaseCell = hxc_l_enemyCell;
          break;
        }
    }
    bool hxc_l_tmp_load_result_n55 = hxc_l_largeEncounter;
    double hxc_l_tmp_conditional_result_n32 = 0.0;
    if (hxc_l_tmp_load_result_n55)
    {
      hxc_l_tmp_conditional_result_n32 = 3.15;
    }
    else
    {
      hxc_l_tmp_conditional_result_n32 = 1.05;
    }
    double hxc_l_spriteWidth = hxc_l_tmp_conditional_result_n32;
    bool hxc_l_tmp_load_result_n57 = hxc_l_largeEncounter;
    double hxc_l_tmp_conditional_result_n34 = 0.0;
    if (hxc_l_tmp_load_result_n57)
    {
      hxc_l_tmp_conditional_result_n34 = 2.88;
    }
    else
    {
      hxc_l_tmp_conditional_result_n34 = 0.96;
    }
    double hxc_l_spriteHeight = hxc_l_tmp_conditional_result_n34;
    bool hxc_l_tmp_load_result_n59 = hxc_l_largeEncounter;
    double hxc_l_tmp_conditional_result_n36 = 0.0;
    if (hxc_l_tmp_load_result_n59)
    {
      hxc_l_tmp_conditional_result_n36 = hxc_l_enemy.hxc_body.hxc_y + 1.44;
    }
    else
    {
      hxc_l_tmp_conditional_result_n36 = hxc_l_enemy.hxc_body.hxc_y + 0.48;
    }
    double hxc_l_spriteY = hxc_l_tmp_conditional_result_n36;
    double hxc_l_z_hf8e9723b471d = hxc_l_enemy.hxc_body.hxc_z;
    int32_t hxc_l_tmp_load_result_n67 = hxc_l_phaseCell;
    double hxc_l_tmp_load_result_n70 = hxc_l_spriteY;
    double hxc_l_tmp_load_result_n71 = hxc_l_z_hf8e9723b471d;
    double hxc_l_tmp_load_result_n73 = hxc_l_spriteWidth;
    bool hxc_l_tmp_call_result_n75 = hxc_caxecraft_app_CaxecraftApp_drawActorPresentation(hxc_l_camera, hxc_l_enemyAsset, hxc_l_tmp_load_result_n67, (struct Vector3){ .x = (float)hxc_l_enemy.hxc_body.hxc_x, .y = (float)hxc_l_tmp_load_result_n70, .z = (float)hxc_l_tmp_load_result_n71 }, hxc_l_tmp_load_result_n73, hxc_l_spriteHeight, hxc_l_entityTexture, hxc_l_entityTextureReady, hxc_l_runtimeTextures);
    if (!hxc_l_tmp_call_result_n75)
    {
      bool hxc_l_tmp_load_result_n76 = hxc_l_largeEncounter;
      double hxc_l_tmp_conditional_result_n39 = 0.0;
      if (hxc_l_tmp_load_result_n76)
      {
        hxc_l_tmp_conditional_result_n39 = 1.8;
      }
      else
      {
        hxc_l_tmp_conditional_result_n39 = 0.70;
      }
      double hxc_l_fallbackWidth = hxc_l_tmp_conditional_result_n39;
      bool hxc_l_tmp_load_result_n78 = hxc_l_largeEncounter;
      double hxc_l_tmp_conditional_result_n41 = 0.0;
      if (hxc_l_tmp_load_result_n78)
      {
        hxc_l_tmp_conditional_result_n41 = 1.8;
      }
      else
      {
        hxc_l_tmp_conditional_result_n41 = 0.54;
      }
      double hxc_l_fallbackHeight = hxc_l_tmp_conditional_result_n41;
      double hxc_l_y_h1b508d5a1d1a = hxc_l_enemy.hxc_body.hxc_y + hxc_l_fallbackHeight * 0.5;
      double hxc_l_z_h1aea9de0009a = hxc_l_enemy.hxc_body.hxc_z;
      double hxc_l_tmp_load_result_n87 = hxc_l_y_h1b508d5a1d1a;
      double hxc_l_tmp_load_result_n88 = hxc_l_z_h1aea9de0009a;
      double hxc_l_tmp_load_result_n90 = hxc_l_fallbackWidth;
      double hxc_l_tmp_load_result_n91 = hxc_l_fallbackHeight;
      DrawCube((struct Vector3){ .x = (float)hxc_l_enemy.hxc_body.hxc_x, .y = (float)hxc_l_tmp_load_result_n87, .z = (float)hxc_l_tmp_load_result_n88 }, (float)hxc_l_tmp_load_result_n90, (float)hxc_l_tmp_load_result_n91, (float)hxc_l_fallbackWidth, (struct Color){ .r = 218, .g = 65, .b = 72, .a = 255 });
      double hxc_l_y_hd0701ce9a87b = hxc_l_enemy.hxc_body.hxc_y + 0.66;
      double hxc_l_z_h98f670e29779 = hxc_l_enemy.hxc_body.hxc_z;
      double hxc_l_tmp_load_result_n100 = hxc_l_y_hd0701ce9a87b;
      DrawCube((struct Vector3){ .x = (float)hxc_l_enemy.hxc_body.hxc_x, .y = (float)hxc_l_tmp_load_result_n100, .z = (float)hxc_l_z_h98f670e29779 }, (float)0.50, (float)0.34, (float)0.50, (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 });
    }
    bool hxc_l_tmp_short_circuit_result_n46 = hxc_l_enemyPhase == hxc_caxecraft_domain_ActorControllerPhase_Windup;
    if (!(hxc_l_enemyPhase == hxc_caxecraft_domain_ActorControllerPhase_Windup))
    {
      hxc_l_tmp_short_circuit_result_n46 = hxc_l_enemyPhase == hxc_caxecraft_domain_ActorControllerPhase_Roaring;
    }
    bool hxc_l_tmp_short_circuit_load_result_n106 = hxc_l_tmp_short_circuit_result_n46;
    bool hxc_l_tmp_short_circuit_result_n47 = hxc_l_tmp_short_circuit_load_result_n106;
    if (!hxc_l_tmp_short_circuit_load_result_n106)
    {
      hxc_l_tmp_short_circuit_result_n47 = hxc_l_enemyPhase == hxc_caxecraft_domain_ActorControllerPhase_TailSweep;
    }
    if (hxc_l_tmp_short_circuit_result_n47)
    {
      double hxc_l_y_h486fbd00fe80 = hxc_l_enemy.hxc_body.hxc_y + 1.02;
      double hxc_l_z_hcc60112c2cfc = hxc_l_enemy.hxc_body.hxc_z;
      double hxc_l_tmp_load_result_n115 = hxc_l_y_h486fbd00fe80;
      DrawCube((struct Vector3){ .x = (float)hxc_l_enemy.hxc_body.hxc_x, .y = (float)hxc_l_tmp_load_result_n115, .z = (float)hxc_l_z_hcc60112c2cfc }, (float)0.20, (float)0.20, (float)0.20, (struct Color){ .r = 218, .g = 65, .b = 72, .a = 255 });
    }
  }
  if (hxc_l_berryDrop.hxc_active)
  {
    double hxc_l_y_h3ee637a76855 = hxc_l_berryDrop.hxc_y;
    double hxc_l_z_h25111f09e0dd = hxc_l_berryDrop.hxc_z;
    double hxc_l_tmp_load_result_n123 = hxc_l_y_h3ee637a76855;
    DrawCube((struct Vector3){ .x = (float)(hxc_l_berryDrop.hxc_x - 0.12), .y = (float)hxc_l_tmp_load_result_n123, .z = (float)hxc_l_z_h25111f09e0dd }, (float)0.18, (float)0.18, (float)0.18, (struct Color){ .r = 174, .g = 78, .b = 136, .a = 255 });
    double hxc_l_y_hfbc022e17cf5 = hxc_l_berryDrop.hxc_y;
    double hxc_l_z_hd216b67d2d03 = hxc_l_berryDrop.hxc_z;
    double hxc_l_tmp_load_result_n130 = hxc_l_y_hfbc022e17cf5;
    DrawCube((struct Vector3){ .x = (float)(hxc_l_berryDrop.hxc_x + 0.12), .y = (float)hxc_l_tmp_load_result_n130, .z = (float)hxc_l_z_hd216b67d2d03 }, (float)0.18, (float)0.18, (float)0.18, (struct Color){ .r = 174, .g = 78, .b = 136, .a = 255 });
  }
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return;
}

void hxc_caxecraft_app_CaxecraftApp_drawBreath(int32_t hxc_l_breathTicks, int32_t hxc_l_maximumBreathTicks, struct hxc_caxecraft_app_HudLayout hxc_l_layout, int32_t hxc_l_width)
{
  int32_t hxc_l_bubbleCount = 10;
  int32_t hxc_l_filled = 0;
  if (hxc_l_maximumBreathTicks > 0)
  {
    hxc_l_filled = hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_i32_subtract_wrapping(hxc_i32_add_wrapping(hxc_i32_multiply_wrapping(hxc_l_breathTicks, hxc_l_bubbleCount), hxc_l_maximumBreathTicks), 1), (double)hxc_l_maximumBreathTicks));
  }
  if (hxc_l_filled < 0)
  {
    hxc_l_filled = 0;
  }
  int32_t hxc_l_tmp_load_result_n2 = hxc_l_filled;
  if (hxc_l_tmp_load_result_n2 > hxc_l_bubbleCount)
  {
    hxc_l_filled = hxc_l_bubbleCount;
  }
  int32_t hxc_l_startX = hxc_i32_subtract_wrapping(hxc_l_width, hxc_i32_subtract_wrapping(hxc_i32_multiply_wrapping(hxc_l_bubbleCount, 18), 4)) / 2;
  int32_t hxc_l_y = hxc_i32_add_wrapping(hxc_l_layout.hxc_breath.hxc_y, 6);
  int32_t hxc_l_bubble = 0;
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n8 = hxc_l_bubble;
    if (!(hxc_l_tmp_load_result_n8 < hxc_l_bubbleCount))
    {
      break;
    }
    int32_t hxc_l_tmp_load_result_n10 = hxc_l_bubble;
    enum hxc_caxecraft_app_HudBreathGlyph hxc_l_tmp_call_result_n12 = hxc_caxecraft_app_HudLayout_hudBreathGlyph(hxc_l_tmp_load_result_n10, hxc_l_filled);
    switch (hxc_l_tmp_call_result_n12) {
      case hxc_caxecraft_app_HudBreathGlyph_FilledBreath:
        {
          float hxc_l_radius = (float)6.0;
          struct Color hxc_l_this1_hdf9a2b6ff5f4 = (struct Color){ .r = 164, .g = 235, .b = 246, .a = 255 };
          int32_t hxc_l_tmp_load_result_n14 = hxc_l_startX;
          int32_t hxc_l_tmp_load_result_n15 = hxc_l_bubble;
          int32_t hxc_l_tmp_load_result_n16 = hxc_l_y;
          float hxc_l_tmp_load_result_n17 = hxc_l_radius;
          DrawCircle((int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n14, hxc_i32_multiply_wrapping(hxc_l_tmp_load_result_n15, 18)), (int32_t)hxc_l_tmp_load_result_n16, hxc_l_tmp_load_result_n17, hxc_l_this1_hdf9a2b6ff5f4);
          break;
        }
      case hxc_caxecraft_app_HudBreathGlyph_DepletedBreath:
        {
          struct Color hxc_l_this1_hf605df1773a0 = (struct Color){ .r = 49, .g = 82, .b = 103, .a = 210 };
          int32_t hxc_l_tmp_load_result_n20 = hxc_l_startX;
          int32_t hxc_l_tmp_load_result_n21 = hxc_l_bubble;
          int32_t hxc_l_tmp_load_result_n22 = hxc_l_y;
          DrawRectangleLines((int32_t)hxc_i32_subtract_wrapping(hxc_i32_add_wrapping(hxc_l_tmp_load_result_n20, hxc_i32_multiply_wrapping(hxc_l_tmp_load_result_n21, 18)), 6), (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n22, 6), (int32_t)12, (int32_t)12, hxc_l_this1_hf605df1773a0);
          break;
        }
      default:
        {
          abort();
        }
    }
    hxc_l_bubble = hxc_i32_add_wrapping(hxc_l_bubble, 1);
  }
  return;
}

void hxc_caxecraft_app_CaxecraftApp_drawCampaignLoading(hxc_string hxc_l_destinationLabel, int32_t hxc_l_locale, struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_catalog)
{
  const void *volatile hxc_l_gc_roots[1] = { NULL };
  hxc_l_gc_roots[0] = (const void *)hxc_l_catalog;
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 1, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_tmp_native_call_result_n0 = GetScreenWidth();
  int32_t hxc_l_width = (int32_t)hxc_l_tmp_native_call_result_n0;
  int32_t hxc_l_tmp_native_call_result_n1 = GetScreenHeight();
  int32_t hxc_l_height = (int32_t)hxc_l_tmp_native_call_result_n1;
  int32_t hxc_l_panelWidth = 460;
  int32_t hxc_l_panelHeight = 150;
  int32_t hxc_l_tmp_load_result_n2 = hxc_l_width;
  int32_t hxc_l_panelX = hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n2, hxc_l_panelWidth) / 2;
  int32_t hxc_l_tmp_load_result_n4 = hxc_l_height;
  int32_t hxc_l_panelY = hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n4, hxc_l_panelHeight) / 2;
  ClearBackground((struct Color){ .r = 126, .g = 190, .b = 201, .a = 255 });
  struct Color hxc_l_this1_h1eae82e59d2a = (struct Color){ .r = 15, .g = 27, .b = 39, .a = 220 };
  int32_t hxc_l_tmp_load_result_n8 = hxc_l_panelX;
  int32_t hxc_l_tmp_load_result_n9 = hxc_l_panelY;
  int32_t hxc_l_tmp_load_result_n10 = hxc_l_panelWidth;
  int32_t hxc_l_tmp_load_result_n11 = hxc_l_panelHeight;
  DrawRectangle((int32_t)hxc_l_tmp_load_result_n8, (int32_t)hxc_l_tmp_load_result_n9, (int32_t)hxc_l_tmp_load_result_n10, (int32_t)hxc_l_tmp_load_result_n11, hxc_l_this1_h1eae82e59d2a);
  struct Color hxc_l_this1_hf7cac3db3d49 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
  int32_t hxc_l_tmp_load_result_n14 = hxc_l_panelX;
  int32_t hxc_l_tmp_load_result_n15 = hxc_l_panelY;
  int32_t hxc_l_tmp_load_result_n16 = hxc_l_panelWidth;
  int32_t hxc_l_tmp_load_result_n17 = hxc_l_panelHeight;
  DrawRectangleLines((int32_t)hxc_l_tmp_load_result_n14, (int32_t)hxc_l_tmp_load_result_n15, (int32_t)hxc_l_tmp_load_result_n16, (int32_t)hxc_l_tmp_load_result_n17, hxc_l_this1_hf7cac3db3d49);
  struct Color hxc_l_this1_h85843f7d7a96 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
  if (hxc_l_catalog == NULL)
  {
    abort();
  }
  hxc_string hxc_l_tmp_instance_call_result_n20 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_catalog, hxc_l_locale, (hxc_string){ (const uint8_t *)"menu_adventure", 14, true, NULL });
  hxc_string hxc_l_tmp_cstring_arg_owner_owner_n12 = hxc_l_tmp_instance_call_result_n20;
  hxc_string hxc_l_tmp_cstring_arg_owner_borrow_result_n21 = hxc_l_tmp_cstring_arg_owner_owner_n12;
  int32_t hxc_l_tmp_load_result_n22 = hxc_l_panelX;
  int32_t hxc_l_tmp_load_result_n23 = hxc_l_panelY;
  struct Color hxc_l_tmp_load_result_n24 = hxc_l_this1_h85843f7d7a96;
  hxc_call_cstring hxc_l_tmp_string_prepare_cstring_result_n25 = HXC_CALL_CSTRING_INITIALIZER;
  if (hxc_string_prepare_call_cstring(&hxc_l_tmp_cstring_arg_owner_borrow_result_n21, hxc_default_allocator(), &hxc_l_tmp_string_prepare_cstring_result_n25) != HXC_STATUS_OK)
  {
    abort();
  }
  DrawText(hxc_l_tmp_string_prepare_cstring_result_n25.data, (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n22, 28), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n23, 24), (int32_t)22, hxc_l_tmp_load_result_n24);
  if (hxc_call_cstring_dispose(&hxc_l_tmp_string_prepare_cstring_result_n25) != HXC_STATUS_OK)
  {
    abort();
  }
  struct Color hxc_l_this1_hd6c4b58ab21e = (struct Color){ .r = 228, .g = 239, .b = 227, .a = 255 };
  int32_t hxc_l_tmp_load_result_n27 = hxc_l_panelX;
  int32_t hxc_l_tmp_load_result_n28 = hxc_l_panelY;
  struct Color hxc_l_tmp_load_result_n29 = hxc_l_this1_hd6c4b58ab21e;
  hxc_call_cstring hxc_l_tmp_string_prepare_cstring_result_n30 = HXC_CALL_CSTRING_INITIALIZER;
  if (hxc_string_prepare_call_cstring(&hxc_l_destinationLabel, hxc_default_allocator(), &hxc_l_tmp_string_prepare_cstring_result_n30) != HXC_STATUS_OK)
  {
    abort();
  }
  DrawText(hxc_l_tmp_string_prepare_cstring_result_n30.data, (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n27, 28), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n28, 76), (int32_t)30, hxc_l_tmp_load_result_n29);
  if (hxc_call_cstring_dispose(&hxc_l_tmp_string_prepare_cstring_result_n30) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_string_release(&hxc_l_tmp_cstring_arg_owner_owner_n12) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return;
}

void hxc_caxecraft_app_CaxecraftApp_drawCloud(double hxc_l_eyeX, double hxc_l_eyeY, double hxc_l_eyeZ, double hxc_l_simulationSeconds, double hxc_l_baseX, double hxc_l_baseZ, double hxc_l_speed, int32_t hxc_l_variant)
{
  double hxc_l_relativeX = hxc_l_baseX + hxc_l_simulationSeconds * hxc_l_speed;
  while (1)
  {
    if (!(hxc_l_relativeX > 72.0))
    {
      break;
    }
    hxc_l_relativeX = hxc_l_relativeX - 144.0;
  }
  double hxc_l_x = hxc_l_eyeX + hxc_l_relativeX;
  double hxc_l_y = hxc_l_eyeY + 20.0 + (double)hxc_l_variant * 2.2;
  double hxc_l_z = hxc_l_eyeZ + hxc_l_baseZ;
  struct Color hxc_l_bright = (struct Color){ .r = 239, .g = 246, .b = 238, .a = 225 };
  struct Color hxc_l_shade = (struct Color){ .r = 199, .g = 219, .b = 216, .a = 205 };
  double hxc_l_tmp_load_result_n5 = hxc_l_x;
  double hxc_l_tmp_load_result_n6 = hxc_l_y;
  double hxc_l_tmp_load_result_n7 = hxc_l_z;
  DrawCube((struct Vector3){ .x = (float)hxc_l_tmp_load_result_n5, .y = (float)hxc_l_tmp_load_result_n6, .z = (float)hxc_l_tmp_load_result_n7 }, (float)12.0, (float)2.2, (float)5.0, hxc_l_bright);
  double hxc_l_tmp_load_result_n10 = hxc_l_x;
  double hxc_l_tmp_load_result_n11 = hxc_l_y;
  double hxc_l_tmp_load_result_n12 = hxc_l_z;
  DrawCube((struct Vector3){ .x = (float)(hxc_l_tmp_load_result_n10 - 4.0), .y = (float)(hxc_l_tmp_load_result_n11 + 1.3), .z = (float)hxc_l_tmp_load_result_n12 }, (float)5.0, (float)2.0, (float)4.0, hxc_l_bright);
  double hxc_l_tmp_load_result_n15 = hxc_l_x;
  double hxc_l_tmp_load_result_n16 = hxc_l_y;
  double hxc_l_tmp_load_result_n17 = hxc_l_z;
  DrawCube((struct Vector3){ .x = (float)(hxc_l_tmp_load_result_n15 + 3.5), .y = (float)(hxc_l_tmp_load_result_n16 + 1.0), .z = (float)(hxc_l_tmp_load_result_n17 + 0.4) }, (float)6.0, (float)2.5, (float)4.5, hxc_l_bright);
  double hxc_l_tmp_load_result_n20 = hxc_l_x;
  double hxc_l_tmp_load_result_n21 = hxc_l_y;
  double hxc_l_tmp_load_result_n22 = hxc_l_z;
  DrawCube((struct Vector3){ .x = (float)hxc_l_tmp_load_result_n20, .y = (float)(hxc_l_tmp_load_result_n21 - 1.2), .z = (float)hxc_l_tmp_load_result_n22 }, (float)10.0, (float)0.7, (float)4.2, hxc_l_shade);
  return;
}

void hxc_caxecraft_app_CaxecraftApp_drawConversation(struct hxc_caxecraft_app_HudView hxc_l_view, struct hxc_caxecraft_app_HudResources hxc_l_resources, struct hxc_caxecraft_app_RuntimeTextureAtlasCatalog *hxc_l_runtimeTextures, struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_uiCatalog, int32_t hxc_l_locale, int32_t hxc_l_width_hf2ade07c703f, int32_t hxc_l_height_h54d8cf3f7625, struct Color hxc_l_color)
{
  const void *volatile hxc_l_gc_roots[6] = { NULL, NULL, NULL, NULL, NULL, NULL };
  hxc_l_gc_roots[0] = (const void *)hxc_l_view.hxc_presentation;
  hxc_l_gc_roots[1] = (const void *)hxc_l_runtimeTextures;
  hxc_l_gc_roots[2] = (const void *)hxc_l_uiCatalog;
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 6, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_string hxc_l_tmp_cstring_arg_owner_owner_n44 = { 0 };
  hxc_string hxc_l_text = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n53 = { 0 };
  hxc_string hxc_l_source_h07e6fabe1236 = { 0 };
  hxc_string hxc_l_visibleText = { 0 };
  hxc_string hxc_l_message = { 0 };
  hxc_string hxc_l_tmp_cstring_arg_owner_owner_n77 = { 0 };
  struct hxc_optional_caxecraft_app_ConversationState hxc_l_conversation = hxc_l_view.hxc_conversation;
  hxc_string hxc_l_activeDialogue = hxc_l_view.hxc_activeDialogue;
  if (hxc_string_retain(hxc_l_activeDialogue) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_optional_caxecraft_app_ConversationState hxc_l_tmp_load_result_n2 = hxc_l_conversation;
  bool hxc_l_tmp_short_circuit_result_n10 = !hxc_l_tmp_load_result_n2.hxc_has_value;
  if (!!hxc_l_tmp_load_result_n2.hxc_has_value)
  {
    bool hxc_l_tmp_string_null_equality_result_n11 = hxc_l_activeDialogue.data == NULL;
    hxc_l_tmp_short_circuit_result_n10 = hxc_l_tmp_string_null_equality_result_n11;
  }
  if (hxc_l_tmp_short_circuit_result_n10)
  {
    if (hxc_string_release(&hxc_l_activeDialogue) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return;
  }
  struct hxc_caxecraft_app_HudModalLayout hxc_l_tmp_call_result_n6 = hxc_caxecraft_app_HudLayout_hudModalLayout(hxc_l_width_hf2ade07c703f, hxc_l_height_h54d8cf3f7625, false);
  struct hxc_caxecraft_app_HudModalLayout hxc_l_layout = hxc_l_tmp_call_result_n6;
  struct hxc_caxecraft_app_HudRectangle hxc_l_panel = hxc_l_layout.hxc_conversation;
  struct hxc_caxecraft_app_HudRectangle hxc_l_portrait = hxc_l_layout.hxc_conversationPortrait;
  struct hxc_caxecraft_app_HudRectangle hxc_l_textArea = hxc_l_layout.hxc_conversationText;
  struct hxc_caxecraft_app_HudRectangle hxc_l_help = hxc_l_layout.hxc_conversationHelp;
  int32_t hxc_l_x_hb0326ad6c646 = hxc_l_panel.hxc_x;
  int32_t hxc_l_y_hfe2c79687366 = hxc_l_panel.hxc_y;
  int32_t hxc_l_width_h2c5976d21fcf = hxc_l_panel.hxc_width;
  int32_t hxc_l_height_h0489bee4a970 = hxc_l_panel.hxc_height;
  struct Color hxc_l_this1_h9732b9a50425 = (struct Color){ .r = 15, .g = 27, .b = 39, .a = 220 };
  int32_t hxc_l_tmp_load_result_n16 = hxc_l_x_hb0326ad6c646;
  int32_t hxc_l_tmp_load_result_n17 = hxc_l_y_hfe2c79687366;
  int32_t hxc_l_tmp_load_result_n18 = hxc_l_width_h2c5976d21fcf;
  int32_t hxc_l_tmp_load_result_n19 = hxc_l_height_h0489bee4a970;
  DrawRectangle((int32_t)hxc_l_tmp_load_result_n16, (int32_t)hxc_l_tmp_load_result_n17, (int32_t)hxc_l_tmp_load_result_n18, (int32_t)hxc_l_tmp_load_result_n19, hxc_l_this1_h9732b9a50425);
  int32_t hxc_l_x_h18ebe7e70f48 = hxc_l_panel.hxc_x;
  int32_t hxc_l_y_ha3bfc652bfd3 = hxc_l_panel.hxc_y;
  int32_t hxc_l_width_h140fcd7ef2b2 = hxc_l_panel.hxc_width;
  int32_t hxc_l_height_h9f5ab6495f1b = hxc_l_panel.hxc_height;
  struct Color hxc_l_this1_h237fd027a5b6 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
  int32_t hxc_l_tmp_load_result_n26 = hxc_l_x_h18ebe7e70f48;
  int32_t hxc_l_tmp_load_result_n27 = hxc_l_y_ha3bfc652bfd3;
  int32_t hxc_l_tmp_load_result_n28 = hxc_l_width_h140fcd7ef2b2;
  int32_t hxc_l_tmp_load_result_n29 = hxc_l_height_h9f5ab6495f1b;
  DrawRectangleLines((int32_t)hxc_l_tmp_load_result_n26, (int32_t)hxc_l_tmp_load_result_n27, (int32_t)hxc_l_tmp_load_result_n28, (int32_t)hxc_l_tmp_load_result_n29, hxc_l_this1_h237fd027a5b6);
  bool hxc_l_portraitDrawn = false;
  bool hxc_l_tmp_string_equality_result_n28 = hxc_l_view.hxc_conversationPortraitAsset.data != NULL && (hxc_l_view.hxc_conversationPortraitAsset.byte_length == (hxc_string){ (const uint8_t *)"entities", 8, true, NULL }.byte_length && (hxc_l_view.hxc_conversationPortraitAsset.byte_length == 0 || memcmp(hxc_l_view.hxc_conversationPortraitAsset.data, (hxc_string){ (const uint8_t *)"entities", 8, true, NULL }.data, hxc_l_view.hxc_conversationPortraitAsset.byte_length) == 0));
  bool hxc_l_tmp_string_equality_result_result_n33 = hxc_l_tmp_string_equality_result_n28;
  bool hxc_l_tmp_short_circuit_result_n29 = hxc_l_tmp_string_equality_result_result_n33;
  if (hxc_l_tmp_string_equality_result_result_n33)
  {
    hxc_l_tmp_short_circuit_result_n29 = hxc_l_resources.hxc_entityTextureReady;
  }
  if (hxc_l_tmp_short_circuit_result_n29)
  {
    int32_t hxc_l_tmp_record_field_load_result_n38 = hxc_l_portrait.hxc_x;
    int32_t hxc_l_tmp_record_field_load_result_n39 = hxc_l_portrait.hxc_y;
    int32_t hxc_l_tmp_record_field_load_result_n40 = hxc_l_portrait.hxc_width;
    bool hxc_l_tmp_call_result_n42 = hxc_caxecraft_app_CaxecraftAtlas_drawEntityPortrait(hxc_l_resources.hxc_entityTexture, hxc_l_view.hxc_conversationPortraitCell, hxc_l_tmp_record_field_load_result_n38, hxc_l_tmp_record_field_load_result_n39, hxc_l_tmp_record_field_load_result_n40, hxc_l_portrait.hxc_height);
    hxc_l_portraitDrawn = hxc_l_tmp_call_result_n42;
  }
  else
  {
    if (hxc_l_runtimeTextures == NULL)
    {
      abort();
    }
    int32_t hxc_l_tmp_record_field_load_result_n45 = hxc_l_portrait.hxc_x;
    int32_t hxc_l_tmp_record_field_load_result_n46 = hxc_l_portrait.hxc_y;
    int32_t hxc_l_tmp_record_field_load_result_n47 = hxc_l_portrait.hxc_width;
    bool hxc_l_tmp_instance_call_result_n49 = hxc_caxecraft_app_RuntimeTextureAtlasCatalog_drawCell(hxc_l_runtimeTextures, hxc_l_view.hxc_conversationPortraitAsset, hxc_l_view.hxc_conversationPortraitCell, hxc_l_tmp_record_field_load_result_n45, hxc_l_tmp_record_field_load_result_n46, hxc_l_tmp_record_field_load_result_n47, hxc_l_portrait.hxc_height);
    hxc_l_portraitDrawn = hxc_l_tmp_instance_call_result_n49;
  }
  if (!hxc_l_portraitDrawn)
  {
    int32_t hxc_l_x_hdc9c01740eb7 = hxc_l_portrait.hxc_x;
    int32_t hxc_l_y_h7f62bff8e6f4 = hxc_l_portrait.hxc_y;
    int32_t hxc_l_width_h4fe030096f57 = hxc_l_portrait.hxc_width;
    int32_t hxc_l_height_h077b8c4f0487 = hxc_l_portrait.hxc_height;
    struct Color hxc_l_this1_h675c65da6712 = (struct Color){ .r = 126, .g = 190, .b = 201, .a = 255 };
    int32_t hxc_l_tmp_load_result_n56 = hxc_l_x_hdc9c01740eb7;
    int32_t hxc_l_tmp_load_result_n57 = hxc_l_y_h7f62bff8e6f4;
    int32_t hxc_l_tmp_load_result_n58 = hxc_l_width_h4fe030096f57;
    int32_t hxc_l_tmp_load_result_n59 = hxc_l_height_h077b8c4f0487;
    DrawRectangle((int32_t)hxc_l_tmp_load_result_n56, (int32_t)hxc_l_tmp_load_result_n57, (int32_t)hxc_l_tmp_load_result_n58, (int32_t)hxc_l_tmp_load_result_n59, hxc_l_this1_h675c65da6712);
    int32_t hxc_l_x_h5da863831b16 = hxc_l_portrait.hxc_x;
    int32_t hxc_l_y_hfccf8cbcbbda = hxc_l_portrait.hxc_y;
    int32_t hxc_l_width_h88ded0ebaa32 = hxc_l_portrait.hxc_width;
    int32_t hxc_l_height_h38f7aa80e4cf = hxc_l_portrait.hxc_height;
    struct Color hxc_l_this1_h54d2a6bdc579 = (struct Color){ .r = 228, .g = 239, .b = 227, .a = 255 };
    int32_t hxc_l_tmp_load_result_n66 = hxc_l_x_h5da863831b16;
    int32_t hxc_l_tmp_load_result_n67 = hxc_l_y_hfccf8cbcbbda;
    int32_t hxc_l_tmp_load_result_n68 = hxc_l_width_h88ded0ebaa32;
    int32_t hxc_l_tmp_load_result_n69 = hxc_l_height_h38f7aa80e4cf;
    DrawRectangleLines((int32_t)hxc_l_tmp_load_result_n66, (int32_t)hxc_l_tmp_load_result_n67, (int32_t)hxc_l_tmp_load_result_n68, (int32_t)hxc_l_tmp_load_result_n69, hxc_l_this1_h54d2a6bdc579);
  }
  bool hxc_l_tmp_string_null_equality_result_n40 = hxc_l_view.hxc_conversationSpeaker.data == NULL;
  if (hxc_l_tmp_string_null_equality_result_n40)
  {
    int32_t hxc_l_x_hf0b7a1944152 = hxc_l_textArea.hxc_x;
    int32_t hxc_l_y_h111329d97451 = hxc_i32_add_wrapping(hxc_l_panel.hxc_y, 18);
    struct Color hxc_l_this1_h8b36392ff255 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
    if (hxc_l_uiCatalog == NULL)
    {
      abort();
    }
    hxc_string hxc_l_tmp_instance_call_result_n76 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_uiCatalog, hxc_l_locale, (hxc_string){ (const uint8_t *)"conversation_narrator", 21, true, NULL });
    hxc_l_tmp_cstring_arg_owner_owner_n44 = hxc_l_tmp_instance_call_result_n76;
    hxc_string hxc_l_tmp_cstring_arg_owner_borrow_result_n77 = hxc_l_tmp_cstring_arg_owner_owner_n44;
    int32_t hxc_l_tmp_load_result_n78 = hxc_l_x_hf0b7a1944152;
    int32_t hxc_l_tmp_load_result_n79 = hxc_l_y_h111329d97451;
    struct Color hxc_l_tmp_load_result_n80 = hxc_l_this1_h8b36392ff255;
    hxc_call_cstring hxc_l_tmp_string_prepare_cstring_result_n81 = HXC_CALL_CSTRING_INITIALIZER;
    if (hxc_string_prepare_call_cstring(&hxc_l_tmp_cstring_arg_owner_borrow_result_n77, hxc_default_allocator(), &hxc_l_tmp_string_prepare_cstring_result_n81) != HXC_STATUS_OK)
    {
      abort();
    }
    DrawText(hxc_l_tmp_string_prepare_cstring_result_n81.data, (int32_t)hxc_l_tmp_load_result_n78, (int32_t)hxc_l_tmp_load_result_n79, (int32_t)20, hxc_l_tmp_load_result_n80);
    if (hxc_call_cstring_dispose(&hxc_l_tmp_string_prepare_cstring_result_n81) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_cstring_arg_owner_owner_n44) != HXC_STATUS_OK)
    {
      abort();
    }
  }
  else
  {
    hxc_string hxc_l_tmp_record_field_project_n82 = hxc_l_view.hxc_conversationSpeakerName;
    if (hxc_l_tmp_record_field_project_n82.data == NULL)
    {
      abort();
    }
    int32_t hxc_l_tmp_string_length_result_n83;
    if (hxc_string_haxe_length(hxc_l_tmp_record_field_project_n82, &hxc_l_tmp_string_length_result_n83) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_string hxc_l_tmp_conditional_managed_result_n46 = { 0 };
    if (hxc_l_tmp_string_length_result_n83 == 0)
    {
      hxc_string hxc_l_tmp_call_result_n85 = hxc_caxecraft_app_CaxecraftApp_conversationSpeakerLabel(hxc_l_view.hxc_conversationSpeaker);
      hxc_l_tmp_conditional_managed_result_n46 = hxc_l_tmp_call_result_n85;
    }
    else
    {
      (void)hxc_l_view.hxc_conversationSpeakerName;
      hxc_l_tmp_conditional_managed_result_n46 = hxc_l_view.hxc_conversationSpeakerName;
      if (hxc_string_retain(hxc_l_tmp_conditional_managed_result_n46) != HXC_STATUS_OK)
      {
        abort();
      }
    }
    hxc_string hxc_l_tmp_conditional_managed_move_result_n87 = hxc_l_tmp_conditional_managed_result_n46;
    hxc_l_text = hxc_l_tmp_conditional_managed_move_result_n87;
    int32_t hxc_l_x_hed286e93de60 = hxc_l_textArea.hxc_x;
    int32_t hxc_l_y_h699fd8078309 = hxc_i32_add_wrapping(hxc_l_panel.hxc_y, 18);
    struct Color hxc_l_this1_h5853c06ff5f6 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
    hxc_string hxc_l_tmp_load_result_n91 = hxc_l_text;
    int32_t hxc_l_tmp_load_result_n92 = hxc_l_x_hed286e93de60;
    int32_t hxc_l_tmp_load_result_n93 = hxc_l_y_h699fd8078309;
    struct Color hxc_l_tmp_load_result_n94 = hxc_l_this1_h5853c06ff5f6;
    hxc_call_cstring hxc_l_tmp_string_prepare_cstring_result_n95 = HXC_CALL_CSTRING_INITIALIZER;
    if (hxc_string_prepare_call_cstring(&hxc_l_tmp_load_result_n91, hxc_default_allocator(), &hxc_l_tmp_string_prepare_cstring_result_n95) != HXC_STATUS_OK)
    {
      abort();
    }
    DrawText(hxc_l_tmp_string_prepare_cstring_result_n95.data, (int32_t)hxc_l_tmp_load_result_n92, (int32_t)hxc_l_tmp_load_result_n93, (int32_t)20, hxc_l_tmp_load_result_n94);
    if (hxc_call_cstring_dispose(&hxc_l_tmp_string_prepare_cstring_result_n95) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_text) != HXC_STATUS_OK)
    {
      abort();
    }
  }
  hxc_l_gc_roots[3] = (const void *)hxc_l_view.hxc_presentation;
  struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_view1 = hxc_l_view.hxc_presentation;
  struct hxc_optional_caxecraft_app_ConversationState hxc_l_tmp_load_result_n97 = hxc_l_conversation;
  if (!hxc_l_tmp_load_result_n97.hxc_has_value)
  {
    abort();
  }
  int32_t hxc_l_conversation1 = hxc_l_tmp_load_result_n97.hxc_value.hxc_lineIndex;
  hxc_string hxc_l_source_h5540a3f89825 = { 0 };
  switch (hxc_l_locale) {
    case 0:
      {
        hxc_l_source_h5540a3f89825 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
        if (hxc_string_retain(hxc_l_source_h5540a3f89825) != HXC_STATUS_OK)
        {
          abort();
        }
        break;
      }
    case 1:
      {
        hxc_l_source_h5540a3f89825 = (hxc_string){ (const uint8_t *)"es-mx", 5, true, NULL };
        if (hxc_string_retain(hxc_l_source_h5540a3f89825) != HXC_STATUS_OK)
        {
          abort();
        }
        break;
      }
    default:
      {
        hxc_l_source_h5540a3f89825 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
        if (hxc_string_retain(hxc_l_source_h5540a3f89825) != HXC_STATUS_OK)
        {
          abort();
        }
        break;
      }
  }
  hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n100 = hxc_l_source_h5540a3f89825;
  hxc_l_tmp_managed_flow_owner_n53 = hxc_l_tmp_managed_flow_carrier_move_result_n100;
  struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_load_result_n101 = hxc_l_view1;
  hxc_l_gc_roots[4] = (const void *)hxc_l_tmp_load_result_n101;
  if (hxc_l_tmp_load_result_n101 == NULL)
  {
    abort();
  }
  hxc_string hxc_l_tmp_load_result_n102 = hxc_l_activeDialogue;
  int32_t hxc_l_tmp_load_result_n103 = hxc_l_conversation1;
  hxc_string hxc_l_tmp_instance_call_result_n105 = hxc_caxecraft_content_RuntimeLevelPresentation_dialogueLine(hxc_l_tmp_load_result_n101, hxc_l_tmp_load_result_n102, hxc_l_tmp_load_result_n103, hxc_l_tmp_managed_flow_owner_n53);
  hxc_l_source_h07e6fabe1236 = hxc_l_tmp_instance_call_result_n105;
  struct hxc_optional_caxecraft_app_ConversationState hxc_l_tmp_load_result_n106 = hxc_l_conversation;
  if (!hxc_l_tmp_load_result_n106.hxc_has_value)
  {
    abort();
  }
  hxc_string hxc_l_tmp_load_result_n109 = hxc_l_source_h07e6fabe1236;
  if (hxc_l_tmp_load_result_n109.data == NULL)
  {
    abort();
  }
  int32_t hxc_l_tmp_string_length_result_n110;
  if (hxc_string_haxe_length(hxc_l_tmp_load_result_n109, &hxc_l_tmp_string_length_result_n110) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_tmp_conditional_result_n56 = 0;
  if (hxc_l_tmp_load_result_n106.hxc_value.hxc_visibleCharacters < hxc_l_tmp_string_length_result_n110)
  {
    struct hxc_optional_caxecraft_app_ConversationState hxc_l_tmp_load_result_n111 = hxc_l_conversation;
    if (!hxc_l_tmp_load_result_n111.hxc_has_value)
    {
      abort();
    }
    hxc_l_tmp_conditional_result_n56 = hxc_l_tmp_load_result_n111.hxc_value.hxc_visibleCharacters;
  }
  else
  {
    hxc_string hxc_l_tmp_load_result_n114 = hxc_l_source_h07e6fabe1236;
    if (hxc_l_tmp_load_result_n114.data == NULL)
    {
      abort();
    }
    int32_t hxc_l_tmp_string_length_result_n115;
    if (hxc_string_haxe_length(hxc_l_tmp_load_result_n114, &hxc_l_tmp_string_length_result_n115) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_conditional_result_n56 = hxc_l_tmp_string_length_result_n115;
  }
  int32_t hxc_l_visibleCount = hxc_l_tmp_conditional_result_n56;
  hxc_string hxc_l_tmp_load_result_n117 = hxc_l_source_h07e6fabe1236;
  if (hxc_l_tmp_load_result_n117.data == NULL)
  {
    abort();
  }
  hxc_string hxc_l_tmp_string_substring_result_n119 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
  if (hxc_string_substring(hxc_l_tmp_load_result_n117, 0, true, hxc_l_visibleCount, &hxc_l_tmp_string_substring_result_n119) != HXC_STATUS_OK)
  {
    abort();
  }
  (void)hxc_l_tmp_string_substring_result_n119;
  hxc_string hxc_l_tmp_string_substring_owner_n58 = { 0 };
  hxc_l_tmp_string_substring_owner_n58 = hxc_l_tmp_string_substring_result_n119;
  if (hxc_string_retain(hxc_l_tmp_string_substring_owner_n58) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_string hxc_l_tmp_string_substring_owned_result_n120 = hxc_l_tmp_string_substring_owner_n58;
  hxc_l_visibleText = hxc_l_tmp_string_substring_owned_result_n120;
  int32_t hxc_l_bodySize = 18;
  int32_t hxc_l_lineHeight = 25;
  struct hxc_caxecraft_app_HudRectangle hxc_l_tmp_load_result_n121 = hxc_l_textArea;
  int32_t hxc_l_tmp_call_result_n123 = hxc_caxecraft_app_HudLayout_hudWrappedCharacterLimit(hxc_l_tmp_load_result_n121, hxc_l_bodySize);
  int32_t hxc_l_characterLimit = hxc_l_tmp_call_result_n123;
  struct hxc_caxecraft_app_HudRectangle hxc_l_tmp_load_result_n124 = hxc_l_textArea;
  int32_t hxc_l_tmp_call_result_n126 = hxc_caxecraft_app_HudLayout_hudWrappedLineLimit(hxc_l_tmp_load_result_n124, hxc_l_lineHeight);
  int32_t hxc_l_lineLimit = hxc_l_tmp_call_result_n126;
  hxc_string hxc_l_tmp_static_call_argument_0_n63 = hxc_l_source_h07e6fabe1236;
  int32_t hxc_l_tmp_load_result_n128 = hxc_l_characterLimit;
  int32_t hxc_l_tmp_conditional_result_n64 = 0;
  if (hxc_l_tmp_load_result_n128 < 70)
  {
    hxc_l_tmp_conditional_result_n64 = hxc_l_characterLimit;
  }
  else
  {
    hxc_l_tmp_conditional_result_n64 = 70;
  }
  int32_t hxc_l_tmp_conditional_load_result_n130 = hxc_l_tmp_conditional_result_n64;
  int32_t hxc_l_tmp_load_result_n131 = hxc_l_lineLimit;
  bool hxc_l_tmp_call_result_n133 = hxc_caxecraft_app_HudLayout_hudWrappedTextFits(hxc_l_tmp_static_call_argument_0_n63, hxc_l_tmp_conditional_load_result_n130, hxc_l_tmp_load_result_n131);
  if (!hxc_l_tmp_call_result_n133)
  {
    hxc_l_bodySize = 16;
    hxc_l_lineHeight = 22;
    struct hxc_caxecraft_app_HudRectangle hxc_l_tmp_load_result_n134 = hxc_l_textArea;
    int32_t hxc_l_tmp_call_result_n136 = hxc_caxecraft_app_HudLayout_hudWrappedCharacterLimit(hxc_l_tmp_load_result_n134, hxc_l_bodySize);
    hxc_l_characterLimit = hxc_l_tmp_call_result_n136;
    struct hxc_caxecraft_app_HudRectangle hxc_l_tmp_load_result_n137 = hxc_l_textArea;
    int32_t hxc_l_tmp_call_result_n139 = hxc_caxecraft_app_HudLayout_hudWrappedLineLimit(hxc_l_tmp_load_result_n137, hxc_l_lineHeight);
    hxc_l_lineLimit = hxc_l_tmp_call_result_n139;
  }
  hxc_string hxc_l_tmp_static_call_argument_0_n65 = hxc_l_visibleText;
  int32_t hxc_l_tmp_static_call_argument_1_n66 = hxc_l_textArea.hxc_x;
  int32_t hxc_l_tmp_static_call_argument_2_n67 = hxc_l_textArea.hxc_y;
  int32_t hxc_l_tmp_static_call_argument_3_n68 = hxc_l_bodySize;
  int32_t hxc_l_tmp_load_result_n144 = hxc_l_characterLimit;
  int32_t hxc_l_tmp_conditional_result_n69 = 0;
  if (hxc_l_tmp_load_result_n144 < 70)
  {
    hxc_l_tmp_conditional_result_n69 = hxc_l_characterLimit;
  }
  else
  {
    hxc_l_tmp_conditional_result_n69 = 70;
  }
  int32_t hxc_l_tmp_conditional_load_result_n146 = hxc_l_tmp_conditional_result_n69;
  int32_t hxc_l_tmp_load_result_n147 = hxc_l_lineHeight;
  int32_t hxc_l_tmp_load_result_n148 = hxc_l_lineLimit;
  hxc_string hxc_l_tmp_static_call_argument_0_load_result_n149 = hxc_l_tmp_static_call_argument_0_n65;
  int32_t hxc_l_tmp_static_call_argument_1_load_result_n150 = hxc_l_tmp_static_call_argument_1_n66;
  int32_t hxc_l_tmp_static_call_argument_2_load_result_n151 = hxc_l_tmp_static_call_argument_2_n67;
  hxc_caxecraft_app_CaxecraftApp_drawWrappedText(hxc_l_tmp_static_call_argument_0_load_result_n149, hxc_l_tmp_static_call_argument_1_load_result_n150, hxc_l_tmp_static_call_argument_2_load_result_n151, hxc_l_tmp_static_call_argument_3_n68, hxc_l_tmp_conditional_load_result_n146, hxc_l_tmp_load_result_n147, hxc_l_tmp_load_result_n148, hxc_l_color);
  struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_record_field_project_n153 = hxc_l_view.hxc_presentation;
  hxc_l_gc_roots[5] = (const void *)hxc_l_tmp_record_field_project_n153;
  if (hxc_l_tmp_record_field_project_n153 == NULL)
  {
    abort();
  }
  int32_t hxc_l_tmp_instance_call_result_n155 = hxc_caxecraft_content_RuntimeLevelPresentation_dialogueLineCount(hxc_l_tmp_record_field_project_n153, hxc_l_activeDialogue);
  int32_t hxc_l_lineCount = hxc_l_tmp_instance_call_result_n155;
  struct hxc_optional_caxecraft_app_ConversationState hxc_l_tmp_load_result_n156 = hxc_l_conversation;
  if (!hxc_l_tmp_load_result_n156.hxc_has_value)
  {
    abort();
  }
  int32_t hxc_l_tmp_record_field_load_result_n159 = hxc_l_panel.hxc_x;
  int32_t hxc_l_tmp_record_field_load_result_n160 = hxc_l_panel.hxc_width;
  hxc_caxecraft_app_HudDigits_drawNumber(hxc_i32_add_wrapping(hxc_l_tmp_load_result_n156.hxc_value.hxc_lineIndex, 1), hxc_i32_subtract_wrapping(hxc_i32_add_wrapping(hxc_l_tmp_record_field_load_result_n159, hxc_l_tmp_record_field_load_result_n160), 76), hxc_i32_add_wrapping(hxc_l_panel.hxc_y, 17), 1, hxc_l_color);
  int32_t hxc_l_tmp_record_field_load_result_n162 = hxc_l_panel.hxc_x;
  int32_t hxc_l_x_h228e1aa9f4ea = hxc_i32_subtract_wrapping(hxc_i32_add_wrapping(hxc_l_tmp_record_field_load_result_n162, hxc_l_panel.hxc_width), 52);
  int32_t hxc_l_y_h8e3f6f45302c = hxc_i32_add_wrapping(hxc_l_panel.hxc_y, 18);
  int32_t hxc_l_tmp_load_result_n165 = hxc_l_x_h228e1aa9f4ea;
  int32_t hxc_l_tmp_load_result_n166 = hxc_l_y_h8e3f6f45302c;
  hxc_string hxc_l_tmp_cstring_arg_constant_owner_n73 = (hxc_string){ (const uint8_t *)"/", 1, true, NULL };
  hxc_call_cstring hxc_l_tmp_string_prepare_cstring_result_n168 = HXC_CALL_CSTRING_INITIALIZER;
  if (hxc_string_prepare_call_cstring(&hxc_l_tmp_cstring_arg_constant_owner_n73, hxc_default_allocator(), &hxc_l_tmp_string_prepare_cstring_result_n168) != HXC_STATUS_OK)
  {
    abort();
  }
  DrawText(hxc_l_tmp_string_prepare_cstring_result_n168.data, (int32_t)hxc_l_tmp_load_result_n165, (int32_t)hxc_l_tmp_load_result_n166, (int32_t)14, hxc_l_color);
  if (hxc_call_cstring_dispose(&hxc_l_tmp_string_prepare_cstring_result_n168) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_tmp_load_result_n169 = hxc_l_lineCount;
  int32_t hxc_l_tmp_record_field_load_result_n170 = hxc_l_panel.hxc_x;
  int32_t hxc_l_tmp_record_field_load_result_n171 = hxc_l_panel.hxc_width;
  hxc_caxecraft_app_HudDigits_drawNumber(hxc_l_tmp_load_result_n169, hxc_i32_subtract_wrapping(hxc_i32_add_wrapping(hxc_l_tmp_record_field_load_result_n170, hxc_l_tmp_record_field_load_result_n171), 34), hxc_i32_add_wrapping(hxc_l_panel.hxc_y, 17), 1, hxc_l_color);
  hxc_string hxc_l_tmp_call_result_n174 = hxc_caxecraft_input_ControlPrompts_conversationHelpMessage(hxc_l_view.hxc_promptDevice);
  hxc_l_message = hxc_l_tmp_call_result_n174;
  int32_t hxc_l_x_h09f55371747a = hxc_l_help.hxc_x;
  int32_t hxc_l_y_hd21834f6a415 = hxc_l_help.hxc_y;
  if (hxc_l_uiCatalog == NULL)
  {
    abort();
  }
  hxc_string hxc_l_tmp_instance_call_result_n178 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_uiCatalog, hxc_l_locale, hxc_l_message);
  hxc_l_tmp_cstring_arg_owner_owner_n77 = hxc_l_tmp_instance_call_result_n178;
  hxc_string hxc_l_tmp_cstring_arg_owner_borrow_result_n179 = hxc_l_tmp_cstring_arg_owner_owner_n77;
  int32_t hxc_l_tmp_load_result_n180 = hxc_l_x_h09f55371747a;
  int32_t hxc_l_tmp_load_result_n181 = hxc_l_y_hd21834f6a415;
  hxc_call_cstring hxc_l_tmp_string_prepare_cstring_result_n182 = HXC_CALL_CSTRING_INITIALIZER;
  if (hxc_string_prepare_call_cstring(&hxc_l_tmp_cstring_arg_owner_borrow_result_n179, hxc_default_allocator(), &hxc_l_tmp_string_prepare_cstring_result_n182) != HXC_STATUS_OK)
  {
    abort();
  }
  DrawText(hxc_l_tmp_string_prepare_cstring_result_n182.data, (int32_t)hxc_l_tmp_load_result_n180, (int32_t)hxc_l_tmp_load_result_n181, (int32_t)14, hxc_l_color);
  if (hxc_call_cstring_dispose(&hxc_l_tmp_string_prepare_cstring_result_n182) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_string_release(&hxc_l_tmp_cstring_arg_owner_owner_n77) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_string_release(&hxc_l_message) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_string_release(&hxc_l_visibleText) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_string_release(&hxc_l_source_h07e6fabe1236) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n53) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_string_release(&hxc_l_activeDialogue) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return;
}

void hxc_caxecraft_app_CaxecraftApp_drawCrosshair(struct hxc_caxecraft_app_HudLayout hxc_l_layout, struct Texture hxc_l_hudTexture, bool hxc_l_hudTextureReady, int32_t hxc_l_centerX, int32_t hxc_l_centerY, bool hxc_l_targetAvailable, struct Color hxc_l_color)
{
  int32_t hxc_l_size = hxc_l_layout.hxc_crosshairSize;
  if (hxc_l_hudTextureReady)
  {
    int32_t hxc_l_tmp_load_result_n2 = hxc_l_size;
    int32_t hxc_l_tmp_load_result_n3 = hxc_l_size;
    hxc_caxecraft_app_CaxecraftAtlas_drawHudGlyph(hxc_l_hudTexture, hxc_caxecraft_app_HudGlyph_Crosshair, hxc_i32_subtract_wrapping(hxc_l_centerX, hxc_l_tmp_load_result_n2 / 2), hxc_i32_subtract_wrapping(hxc_l_centerY, hxc_l_tmp_load_result_n3 / 2), hxc_l_size);
  }
  else
  {
    int32_t hxc_l_arm = hxc_l_size / 2;
    DrawLine((int32_t)hxc_i32_subtract_wrapping(hxc_l_centerX, hxc_l_arm), (int32_t)hxc_l_centerY, (int32_t)hxc_i32_subtract_wrapping(hxc_l_centerX, 3), (int32_t)hxc_l_centerY, hxc_l_color);
    DrawLine((int32_t)hxc_i32_add_wrapping(hxc_l_centerX, 3), (int32_t)hxc_l_centerY, (int32_t)hxc_i32_add_wrapping(hxc_l_centerX, hxc_l_arm), (int32_t)hxc_l_centerY, hxc_l_color);
    DrawLine((int32_t)hxc_l_centerX, (int32_t)hxc_i32_subtract_wrapping(hxc_l_centerY, hxc_l_arm), (int32_t)hxc_l_centerX, (int32_t)hxc_i32_subtract_wrapping(hxc_l_centerY, 3), hxc_l_color);
    DrawLine((int32_t)hxc_l_centerX, (int32_t)hxc_i32_add_wrapping(hxc_l_centerY, 3), (int32_t)hxc_l_centerX, (int32_t)hxc_i32_add_wrapping(hxc_l_centerY, hxc_l_arm), hxc_l_color);
  }
  if (hxc_l_targetAvailable)
  {
    int32_t hxc_l_ringSize = hxc_i32_add_wrapping(hxc_l_size, 6);
    int32_t hxc_l_x = hxc_i32_subtract_wrapping(hxc_l_centerX, hxc_l_ringSize / 2);
    int32_t hxc_l_y = hxc_i32_subtract_wrapping(hxc_l_centerY, hxc_l_ringSize / 2);
    struct Color hxc_l_this1 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
    int32_t hxc_l_tmp_load_result_n14 = hxc_l_x;
    int32_t hxc_l_tmp_load_result_n15 = hxc_l_y;
    int32_t hxc_l_tmp_load_result_n16 = hxc_l_ringSize;
    int32_t hxc_l_tmp_load_result_n17 = hxc_l_ringSize;
    DrawRectangleLines((int32_t)hxc_l_tmp_load_result_n14, (int32_t)hxc_l_tmp_load_result_n15, (int32_t)hxc_l_tmp_load_result_n16, (int32_t)hxc_l_tmp_load_result_n17, hxc_l_this1);
  }
  return;
}

void hxc_caxecraft_app_CaxecraftApp_drawDefeat(struct hxc_caxecraft_app_HudView hxc_l_view, struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_uiCatalog, int32_t hxc_l_width_h0218579225f3, int32_t hxc_l_height_h8715ce602c8d, struct Color hxc_l_color)
{
  const void *volatile hxc_l_gc_roots[2] = { NULL, NULL };
  hxc_l_gc_roots[0] = (const void *)hxc_l_view.hxc_presentation;
  hxc_l_gc_roots[1] = (const void *)hxc_l_uiCatalog;
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 2, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_caxecraft_app_HudModalLayout hxc_l_tmp_call_result_n0 = hxc_caxecraft_app_HudLayout_hudModalLayout(hxc_l_width_h0218579225f3, hxc_l_height_h8715ce602c8d, false);
  struct hxc_caxecraft_app_HudRectangle hxc_l_panel = hxc_l_tmp_call_result_n0.hxc_defeat;
  int32_t hxc_l_x_h9c028eac760f = hxc_l_panel.hxc_x;
  int32_t hxc_l_y_h911bddc6c0bd = hxc_l_panel.hxc_y;
  int32_t hxc_l_width_h97a900a282a2 = hxc_l_panel.hxc_width;
  int32_t hxc_l_height_h6bb27ef6599a = hxc_l_panel.hxc_height;
  struct Color hxc_l_this1_hfc13d539ad2c = (struct Color){ .r = 15, .g = 27, .b = 39, .a = 220 };
  int32_t hxc_l_tmp_load_result_n7 = hxc_l_x_h9c028eac760f;
  int32_t hxc_l_tmp_load_result_n8 = hxc_l_y_h911bddc6c0bd;
  int32_t hxc_l_tmp_load_result_n9 = hxc_l_width_h97a900a282a2;
  int32_t hxc_l_tmp_load_result_n10 = hxc_l_height_h6bb27ef6599a;
  DrawRectangle((int32_t)hxc_l_tmp_load_result_n7, (int32_t)hxc_l_tmp_load_result_n8, (int32_t)hxc_l_tmp_load_result_n9, (int32_t)hxc_l_tmp_load_result_n10, hxc_l_this1_hfc13d539ad2c);
  int32_t hxc_l_x_h29dcf0e1c3cc = hxc_l_panel.hxc_x;
  int32_t hxc_l_y_hd1195eb68ab5 = hxc_l_panel.hxc_y;
  int32_t hxc_l_width_hf4aedcbc0b48 = hxc_l_panel.hxc_width;
  int32_t hxc_l_height_h1bc961eeae45 = hxc_l_panel.hxc_height;
  struct Color hxc_l_this1_h085942179e01 = (struct Color){ .r = 218, .g = 65, .b = 72, .a = 255 };
  int32_t hxc_l_tmp_load_result_n17 = hxc_l_x_h29dcf0e1c3cc;
  int32_t hxc_l_tmp_load_result_n18 = hxc_l_y_hd1195eb68ab5;
  int32_t hxc_l_tmp_load_result_n19 = hxc_l_width_hf4aedcbc0b48;
  int32_t hxc_l_tmp_load_result_n20 = hxc_l_height_h1bc961eeae45;
  DrawRectangleLines((int32_t)hxc_l_tmp_load_result_n17, (int32_t)hxc_l_tmp_load_result_n18, (int32_t)hxc_l_tmp_load_result_n19, (int32_t)hxc_l_tmp_load_result_n20, hxc_l_this1_h085942179e01);
  int32_t hxc_l_x_hfddb81e15ac6 = hxc_i32_add_wrapping(hxc_l_panel.hxc_x, 28);
  int32_t hxc_l_y_h33ccf418872f = hxc_i32_add_wrapping(hxc_l_panel.hxc_y, 32);
  if (hxc_l_uiCatalog == NULL)
  {
    abort();
  }
  hxc_string hxc_l_tmp_instance_call_result_n25 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_uiCatalog, hxc_l_view.hxc_locale, (hxc_string){ (const uint8_t *)"player_fallen", 13, true, NULL });
  hxc_string hxc_l_tmp_cstring_arg_owner_owner_n18 = hxc_l_tmp_instance_call_result_n25;
  hxc_string hxc_l_tmp_cstring_arg_owner_borrow_result_n26 = hxc_l_tmp_cstring_arg_owner_owner_n18;
  int32_t hxc_l_tmp_load_result_n27 = hxc_l_x_hfddb81e15ac6;
  int32_t hxc_l_tmp_load_result_n28 = hxc_l_y_h33ccf418872f;
  hxc_call_cstring hxc_l_tmp_string_prepare_cstring_result_n29 = HXC_CALL_CSTRING_INITIALIZER;
  if (hxc_string_prepare_call_cstring(&hxc_l_tmp_cstring_arg_owner_borrow_result_n26, hxc_default_allocator(), &hxc_l_tmp_string_prepare_cstring_result_n29) != HXC_STATUS_OK)
  {
    abort();
  }
  DrawText(hxc_l_tmp_string_prepare_cstring_result_n29.data, (int32_t)hxc_l_tmp_load_result_n27, (int32_t)hxc_l_tmp_load_result_n28, (int32_t)24, hxc_l_color);
  if (hxc_call_cstring_dispose(&hxc_l_tmp_string_prepare_cstring_result_n29) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_locale = hxc_l_view.hxc_locale;
  hxc_string hxc_l_tmp_call_result_n32 = hxc_caxecraft_input_ControlPrompts_returnPromptMessage(hxc_l_view.hxc_promptDevice);
  hxc_string hxc_l_message = hxc_l_tmp_call_result_n32;
  int32_t hxc_l_x_h97f54ef70862 = hxc_i32_add_wrapping(hxc_l_panel.hxc_x, 28);
  int32_t hxc_l_y_h9f9bb96bf7bd = hxc_i32_add_wrapping(hxc_l_panel.hxc_y, 84);
  struct Color hxc_l_this1_hf9aa04c5674e = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
  int32_t hxc_l_tmp_load_result_n36 = hxc_l_locale;
  hxc_string hxc_l_tmp_instance_call_result_n38 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_uiCatalog, hxc_l_tmp_load_result_n36, hxc_l_message);
  hxc_string hxc_l_tmp_cstring_arg_owner_owner_n24 = hxc_l_tmp_instance_call_result_n38;
  hxc_string hxc_l_tmp_cstring_arg_owner_borrow_result_n39 = hxc_l_tmp_cstring_arg_owner_owner_n24;
  int32_t hxc_l_tmp_load_result_n40 = hxc_l_x_h97f54ef70862;
  int32_t hxc_l_tmp_load_result_n41 = hxc_l_y_h9f9bb96bf7bd;
  struct Color hxc_l_tmp_load_result_n42 = hxc_l_this1_hf9aa04c5674e;
  hxc_call_cstring hxc_l_tmp_string_prepare_cstring_result_n43 = HXC_CALL_CSTRING_INITIALIZER;
  if (hxc_string_prepare_call_cstring(&hxc_l_tmp_cstring_arg_owner_borrow_result_n39, hxc_default_allocator(), &hxc_l_tmp_string_prepare_cstring_result_n43) != HXC_STATUS_OK)
  {
    abort();
  }
  DrawText(hxc_l_tmp_string_prepare_cstring_result_n43.data, (int32_t)hxc_l_tmp_load_result_n40, (int32_t)hxc_l_tmp_load_result_n41, (int32_t)18, hxc_l_tmp_load_result_n42);
  if (hxc_call_cstring_dispose(&hxc_l_tmp_string_prepare_cstring_result_n43) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_string_release(&hxc_l_tmp_cstring_arg_owner_owner_n24) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_string_release(&hxc_l_message) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_string_release(&hxc_l_tmp_cstring_arg_owner_owner_n18) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return;
}

void hxc_caxecraft_app_CaxecraftApp_drawDiagnostics(struct hxc_caxecraft_app_HudView hxc_l_view, struct hxc_caxecraft_app_HudLayout hxc_l_layout, struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_uiCatalog, int32_t hxc_l_locale, struct Color hxc_l_color)
{
  const void *volatile hxc_l_gc_roots[2] = { NULL, NULL };
  hxc_l_gc_roots[0] = (const void *)hxc_l_view.hxc_presentation;
  hxc_l_gc_roots[1] = (const void *)hxc_l_uiCatalog;
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 2, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_caxecraft_app_HudRectangle hxc_l_panel = hxc_l_layout.hxc_diagnostics;
  struct hxc_caxecraft_app_HudMetricsView hxc_l_metrics = hxc_l_view.hxc_metrics;
  int32_t hxc_l_x_hdc7cc28f3af9 = hxc_l_panel.hxc_x;
  int32_t hxc_l_y_h75c2c06f0607 = hxc_l_panel.hxc_y;
  int32_t hxc_l_width_h0ee007e5c9e9 = hxc_l_panel.hxc_width;
  int32_t hxc_l_height_hf5c7016aa317 = hxc_l_panel.hxc_height;
  struct Color hxc_l_this1_h656a12eb072b = (struct Color){ .r = 15, .g = 27, .b = 39, .a = 220 };
  int32_t hxc_l_tmp_load_result_n7 = hxc_l_x_hdc7cc28f3af9;
  int32_t hxc_l_tmp_load_result_n8 = hxc_l_y_h75c2c06f0607;
  int32_t hxc_l_tmp_load_result_n9 = hxc_l_width_h0ee007e5c9e9;
  int32_t hxc_l_tmp_load_result_n10 = hxc_l_height_hf5c7016aa317;
  DrawRectangle((int32_t)hxc_l_tmp_load_result_n7, (int32_t)hxc_l_tmp_load_result_n8, (int32_t)hxc_l_tmp_load_result_n9, (int32_t)hxc_l_tmp_load_result_n10, hxc_l_this1_h656a12eb072b);
  int32_t hxc_l_x_h88f3edf3100b = hxc_l_panel.hxc_x;
  int32_t hxc_l_y_hd3da2c5beeea = hxc_l_panel.hxc_y;
  int32_t hxc_l_width_hc943617fc90a = hxc_l_panel.hxc_width;
  int32_t hxc_l_height_h1767aa85de97 = hxc_l_panel.hxc_height;
  struct Color hxc_l_this1_hf6c76823977a = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
  int32_t hxc_l_tmp_load_result_n17 = hxc_l_x_h88f3edf3100b;
  int32_t hxc_l_tmp_load_result_n18 = hxc_l_y_hd3da2c5beeea;
  int32_t hxc_l_tmp_load_result_n19 = hxc_l_width_hc943617fc90a;
  int32_t hxc_l_tmp_load_result_n20 = hxc_l_height_h1767aa85de97;
  DrawRectangleLines((int32_t)hxc_l_tmp_load_result_n17, (int32_t)hxc_l_tmp_load_result_n18, (int32_t)hxc_l_tmp_load_result_n19, (int32_t)hxc_l_tmp_load_result_n20, hxc_l_this1_hf6c76823977a);
  int32_t hxc_l_x_h719658ee098d = hxc_i32_add_wrapping(hxc_l_panel.hxc_x, 10);
  int32_t hxc_l_y_hff96be7136dd = hxc_i32_add_wrapping(hxc_l_panel.hxc_y, 8);
  if (hxc_l_uiCatalog == NULL)
  {
    abort();
  }
  hxc_string hxc_l_tmp_instance_call_result_n24 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_uiCatalog, hxc_l_locale, (hxc_string){ (const uint8_t *)"debug_visible", 13, true, NULL });
  hxc_string hxc_l_tmp_cstring_arg_owner_owner_n19 = hxc_l_tmp_instance_call_result_n24;
  hxc_string hxc_l_tmp_cstring_arg_owner_borrow_result_n25 = hxc_l_tmp_cstring_arg_owner_owner_n19;
  int32_t hxc_l_tmp_load_result_n26 = hxc_l_x_h719658ee098d;
  int32_t hxc_l_tmp_load_result_n27 = hxc_l_y_hff96be7136dd;
  hxc_call_cstring hxc_l_tmp_string_prepare_cstring_result_n28 = HXC_CALL_CSTRING_INITIALIZER;
  if (hxc_string_prepare_call_cstring(&hxc_l_tmp_cstring_arg_owner_borrow_result_n25, hxc_default_allocator(), &hxc_l_tmp_string_prepare_cstring_result_n28) != HXC_STATUS_OK)
  {
    abort();
  }
  DrawText(hxc_l_tmp_string_prepare_cstring_result_n28.data, (int32_t)hxc_l_tmp_load_result_n26, (int32_t)hxc_l_tmp_load_result_n27, (int32_t)12, hxc_l_color);
  if (hxc_call_cstring_dispose(&hxc_l_tmp_string_prepare_cstring_result_n28) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_tmp_record_field_load_result_n29 = hxc_l_metrics.hxc_visibleBlocks;
  int32_t hxc_l_tmp_record_field_load_result_n30 = hxc_l_panel.hxc_x;
  hxc_caxecraft_app_HudDigits_drawNumber(hxc_l_tmp_record_field_load_result_n29, hxc_i32_add_wrapping(hxc_l_tmp_record_field_load_result_n30, 76), hxc_i32_add_wrapping(hxc_l_panel.hxc_y, 7), 5, (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 });
  int32_t hxc_l_x_h074d45614b23 = hxc_i32_add_wrapping(hxc_l_panel.hxc_x, 162);
  int32_t hxc_l_y_hea0536814e3b = hxc_i32_add_wrapping(hxc_l_panel.hxc_y, 8);
  hxc_string hxc_l_tmp_instance_call_result_n35 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_uiCatalog, hxc_l_locale, (hxc_string){ (const uint8_t *)"debug_draws", 11, true, NULL });
  hxc_string hxc_l_tmp_cstring_arg_owner_owner_n22 = hxc_l_tmp_instance_call_result_n35;
  hxc_string hxc_l_tmp_cstring_arg_owner_borrow_result_n36 = hxc_l_tmp_cstring_arg_owner_owner_n22;
  int32_t hxc_l_tmp_load_result_n37 = hxc_l_x_h074d45614b23;
  int32_t hxc_l_tmp_load_result_n38 = hxc_l_y_hea0536814e3b;
  hxc_call_cstring hxc_l_tmp_string_prepare_cstring_result_n39 = HXC_CALL_CSTRING_INITIALIZER;
  if (hxc_string_prepare_call_cstring(&hxc_l_tmp_cstring_arg_owner_borrow_result_n36, hxc_default_allocator(), &hxc_l_tmp_string_prepare_cstring_result_n39) != HXC_STATUS_OK)
  {
    abort();
  }
  DrawText(hxc_l_tmp_string_prepare_cstring_result_n39.data, (int32_t)hxc_l_tmp_load_result_n37, (int32_t)hxc_l_tmp_load_result_n38, (int32_t)12, hxc_l_color);
  if (hxc_call_cstring_dispose(&hxc_l_tmp_string_prepare_cstring_result_n39) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_tmp_record_field_load_result_n40 = hxc_l_metrics.hxc_drawCalls;
  int32_t hxc_l_tmp_record_field_load_result_n41 = hxc_l_panel.hxc_x;
  hxc_caxecraft_app_HudDigits_drawNumber(hxc_l_tmp_record_field_load_result_n40, hxc_i32_add_wrapping(hxc_l_tmp_record_field_load_result_n41, 214), hxc_i32_add_wrapping(hxc_l_panel.hxc_y, 7), 5, (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 });
  int32_t hxc_l_x_h5a3df5ff667b = hxc_i32_add_wrapping(hxc_l_panel.hxc_x, 10);
  int32_t hxc_l_y_h921ac9e08a8c = hxc_i32_add_wrapping(hxc_l_panel.hxc_y, 32);
  hxc_string hxc_l_tmp_instance_call_result_n46 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_uiCatalog, hxc_l_locale, (hxc_string){ (const uint8_t *)"debug_frame", 11, true, NULL });
  hxc_string hxc_l_tmp_cstring_arg_owner_owner_n25 = hxc_l_tmp_instance_call_result_n46;
  hxc_string hxc_l_tmp_cstring_arg_owner_borrow_result_n47 = hxc_l_tmp_cstring_arg_owner_owner_n25;
  int32_t hxc_l_tmp_load_result_n48 = hxc_l_x_h5a3df5ff667b;
  int32_t hxc_l_tmp_load_result_n49 = hxc_l_y_h921ac9e08a8c;
  hxc_call_cstring hxc_l_tmp_string_prepare_cstring_result_n50 = HXC_CALL_CSTRING_INITIALIZER;
  if (hxc_string_prepare_call_cstring(&hxc_l_tmp_cstring_arg_owner_borrow_result_n47, hxc_default_allocator(), &hxc_l_tmp_string_prepare_cstring_result_n50) != HXC_STATUS_OK)
  {
    abort();
  }
  DrawText(hxc_l_tmp_string_prepare_cstring_result_n50.data, (int32_t)hxc_l_tmp_load_result_n48, (int32_t)hxc_l_tmp_load_result_n49, (int32_t)12, hxc_l_color);
  if (hxc_call_cstring_dispose(&hxc_l_tmp_string_prepare_cstring_result_n50) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_tmp_record_field_load_result_n51 = hxc_l_metrics.hxc_renderedFrames;
  int32_t hxc_l_tmp_record_field_load_result_n52 = hxc_l_panel.hxc_x;
  hxc_caxecraft_app_HudDigits_drawNumber(hxc_l_tmp_record_field_load_result_n51, hxc_i32_add_wrapping(hxc_l_tmp_record_field_load_result_n52, 62), hxc_i32_add_wrapping(hxc_l_panel.hxc_y, 31), 6, hxc_l_color);
  int32_t hxc_l_x_h21ca4e6181f6 = hxc_i32_add_wrapping(hxc_l_panel.hxc_x, 164);
  int32_t hxc_l_y_h178eed612fe5 = hxc_i32_add_wrapping(hxc_l_panel.hxc_y, 32);
  hxc_string hxc_l_tmp_instance_call_result_n56 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_uiCatalog, hxc_l_locale, (hxc_string){ (const uint8_t *)"debug_tick", 10, true, NULL });
  hxc_string hxc_l_tmp_cstring_arg_owner_owner_n28 = hxc_l_tmp_instance_call_result_n56;
  hxc_string hxc_l_tmp_cstring_arg_owner_borrow_result_n57 = hxc_l_tmp_cstring_arg_owner_owner_n28;
  int32_t hxc_l_tmp_load_result_n58 = hxc_l_x_h21ca4e6181f6;
  int32_t hxc_l_tmp_load_result_n59 = hxc_l_y_h178eed612fe5;
  hxc_call_cstring hxc_l_tmp_string_prepare_cstring_result_n60 = HXC_CALL_CSTRING_INITIALIZER;
  if (hxc_string_prepare_call_cstring(&hxc_l_tmp_cstring_arg_owner_borrow_result_n57, hxc_default_allocator(), &hxc_l_tmp_string_prepare_cstring_result_n60) != HXC_STATUS_OK)
  {
    abort();
  }
  DrawText(hxc_l_tmp_string_prepare_cstring_result_n60.data, (int32_t)hxc_l_tmp_load_result_n58, (int32_t)hxc_l_tmp_load_result_n59, (int32_t)12, hxc_l_color);
  if (hxc_call_cstring_dispose(&hxc_l_tmp_string_prepare_cstring_result_n60) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_tmp_record_field_load_result_n61 = hxc_l_metrics.hxc_completedTicks;
  int32_t hxc_l_tmp_record_field_load_result_n62 = hxc_l_panel.hxc_x;
  hxc_caxecraft_app_HudDigits_drawNumber(hxc_l_tmp_record_field_load_result_n61, hxc_i32_add_wrapping(hxc_l_tmp_record_field_load_result_n62, 206), hxc_i32_add_wrapping(hxc_l_panel.hxc_y, 31), 6, hxc_l_color);
  if (hxc_string_release(&hxc_l_tmp_cstring_arg_owner_owner_n28) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_string_release(&hxc_l_tmp_cstring_arg_owner_owner_n25) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_string_release(&hxc_l_tmp_cstring_arg_owner_owner_n22) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_string_release(&hxc_l_tmp_cstring_arg_owner_owner_n19) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return;
}

void hxc_caxecraft_app_CaxecraftApp_drawEnemyAlert(struct hxc_caxecraft_app_HudView hxc_l_view_h9930f80d93d3, struct hxc_caxecraft_app_HudLayout hxc_l_layout, int32_t hxc_l_locale, struct Color hxc_l_color)
{
  const void *volatile hxc_l_gc_roots[3] = { NULL, NULL, NULL };
  hxc_l_gc_roots[0] = (const void *)hxc_l_view_h9930f80d93d3.hxc_presentation;
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 3, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_string hxc_l_value_h90721403ea60 = { 0 };
  hxc_string hxc_l_value_h70586a7ff5e5 = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n9 = { 0 };
  if (hxc_l_view_h9930f80d93d3.hxc_enemy.hxc_vitals.hxc_health <= 0)
  {
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return;
  }
  int32_t hxc_l_message = 7;
  struct Color hxc_l_border = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
  switch (hxc_l_view_h9930f80d93d3.hxc_enemyPhase) {
    case hxc_caxecraft_domain_ActorControllerPhase_Windup:
    case hxc_caxecraft_domain_ActorControllerPhase_Roaring:
      {
        hxc_l_message = 9;
        hxc_l_border = (struct Color){ .r = 218, .g = 65, .b = 72, .a = 255 };
        break;
      }
    case hxc_caxecraft_domain_ActorControllerPhase_Chasing:
    case hxc_caxecraft_domain_ActorControllerPhase_Charging:
      {
        break;
      }
    case hxc_caxecraft_domain_ActorControllerPhase_TailSweep:
      {
        hxc_l_message = 15;
        hxc_l_border = (struct Color){ .r = 218, .g = 65, .b = 72, .a = 255 };
        break;
      }
    case hxc_caxecraft_domain_ActorControllerPhase_Stunned:
      {
        hxc_l_message = 16;
        break;
      }
    case hxc_caxecraft_domain_ActorControllerPhase_Stationary:
    case hxc_caxecraft_domain_ActorControllerPhase_Resting:
    case hxc_caxecraft_domain_ActorControllerPhase_Wandering:
    case hxc_caxecraft_domain_ActorControllerPhase_Returning:
    case hxc_caxecraft_domain_ActorControllerPhase_Recovering:
    case hxc_caxecraft_domain_ActorControllerPhase_Defeated:
      {
        if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
        {
          abort();
        }
        return;
      }
    default:
      {
        abort();
      }
  }
  hxc_l_gc_roots[1] = (const void *)hxc_l_view_h9930f80d93d3.hxc_presentation;
  struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_view_h978171fb40f9 = hxc_l_view_h9930f80d93d3.hxc_presentation;
  hxc_string hxc_l_tmp_call_result_n9 = hxc_caxecraft_app_GameplayMessage_gameplayMessageId(hxc_l_message);
  hxc_l_value_h70586a7ff5e5 = hxc_l_tmp_call_result_n9;
  hxc_string hxc_l_value1 = { 0 };
  switch (hxc_l_locale) {
    case 0:
      {
        hxc_l_value1 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
        if (hxc_string_retain(hxc_l_value1) != HXC_STATUS_OK)
        {
          abort();
        }
        break;
      }
    case 1:
      {
        hxc_l_value1 = (hxc_string){ (const uint8_t *)"es-mx", 5, true, NULL };
        if (hxc_string_retain(hxc_l_value1) != HXC_STATUS_OK)
        {
          abort();
        }
        break;
      }
    default:
      {
        hxc_l_value1 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
        if (hxc_string_retain(hxc_l_value1) != HXC_STATUS_OK)
        {
          abort();
        }
        break;
      }
  }
  hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n10 = hxc_l_value1;
  hxc_l_tmp_managed_flow_owner_n9 = hxc_l_tmp_managed_flow_carrier_move_result_n10;
  struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_load_result_n11 = hxc_l_view_h978171fb40f9;
  hxc_l_gc_roots[2] = (const void *)hxc_l_tmp_load_result_n11;
  if (hxc_l_tmp_load_result_n11 == NULL)
  {
    abort();
  }
  hxc_string hxc_l_tmp_load_result_n12 = hxc_l_value_h70586a7ff5e5;
  hxc_string hxc_l_tmp_instance_call_result_n14 = hxc_caxecraft_content_RuntimeLevelPresentation_message(hxc_l_tmp_load_result_n11, hxc_l_tmp_load_result_n12, hxc_l_tmp_managed_flow_owner_n9);
  hxc_l_value_h90721403ea60 = hxc_l_tmp_instance_call_result_n14;
  hxc_string hxc_l_tmp_load_result_n15 = hxc_l_value_h90721403ea60;
  if (hxc_l_tmp_load_result_n15.data == NULL)
  {
    abort();
  }
  int32_t hxc_l_tmp_string_length_result_n16;
  if (hxc_string_haxe_length(hxc_l_tmp_load_result_n15, &hxc_l_tmp_string_length_result_n16) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_l_tmp_string_length_result_n16 == 0)
  {
    if (hxc_string_release(&hxc_l_value_h90721403ea60) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n9) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_value_h70586a7ff5e5) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return;
  }
  struct hxc_caxecraft_app_HudRectangle hxc_l_tmp_static_call_argument_0_n11 = hxc_l_layout.hxc_alert;
  hxc_string hxc_l_tmp_static_call_argument_1_n12 = hxc_l_value_h90721403ea60;
  int32_t hxc_l_tmp_conditional_result_n13 = 0;
  if (hxc_l_layout.hxc_compact)
  {
    hxc_l_tmp_conditional_result_n13 = 13;
  }
  else
  {
    hxc_l_tmp_conditional_result_n13 = 15;
  }
  int32_t hxc_l_tmp_static_call_argument_2_n14 = hxc_l_tmp_conditional_result_n13;
  struct Color hxc_l_tmp_static_call_argument_3_n15 = hxc_l_color;
  struct Color hxc_l_tmp_static_call_argument_4_n16 = hxc_l_border;
  int32_t hxc_l_tmp_conditional_result_n17 = 0;
  if (hxc_l_layout.hxc_compact)
  {
    hxc_l_tmp_conditional_result_n17 = 34;
  }
  else
  {
    hxc_l_tmp_conditional_result_n17 = 40;
  }
  int32_t hxc_l_tmp_conditional_load_result_n23 = hxc_l_tmp_conditional_result_n17;
  struct hxc_caxecraft_app_HudRectangle hxc_l_tmp_static_call_argument_0_load_result_n24 = hxc_l_tmp_static_call_argument_0_n11;
  hxc_string hxc_l_tmp_static_call_argument_1_load_result_n25 = hxc_l_tmp_static_call_argument_1_n12;
  int32_t hxc_l_tmp_static_call_argument_2_load_result_n26 = hxc_l_tmp_static_call_argument_2_n14;
  struct Color hxc_l_tmp_static_call_argument_3_load_result_n27 = hxc_l_tmp_static_call_argument_3_n15;
  hxc_caxecraft_app_CaxecraftApp_drawNoticePanel(hxc_l_tmp_static_call_argument_0_load_result_n24, hxc_l_tmp_static_call_argument_1_load_result_n25, hxc_l_tmp_static_call_argument_2_load_result_n26, hxc_l_tmp_static_call_argument_3_load_result_n27, hxc_l_tmp_static_call_argument_4_n16, hxc_l_tmp_conditional_load_result_n23);
  if (hxc_string_release(&hxc_l_value_h90721403ea60) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n9) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_string_release(&hxc_l_value_h70586a7ff5e5) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return;
}

void hxc_caxecraft_app_CaxecraftApp_drawHealth(struct hxc_caxecraft_domain_VitalsState hxc_l_vitals, struct Texture hxc_l_hudTexture, bool hxc_l_hudTextureReady, struct hxc_caxecraft_app_HudLayout hxc_l_layout)
{
  int32_t hxc_l_heart = 0;
  while (1)
  {
    if (!(hxc_l_heart < 3))
    {
      break;
    }
    int32_t hxc_l_x = hxc_i32_add_wrapping(hxc_l_layout.hxc_health.hxc_x, hxc_i32_multiply_wrapping(hxc_l_heart, hxc_i32_add_wrapping(hxc_l_layout.hxc_heartSize, hxc_l_layout.hxc_heartGap)));
    int32_t hxc_l_points = hxc_i32_subtract_wrapping(hxc_l_vitals.hxc_health, hxc_i32_multiply_wrapping(hxc_l_heart, 2));
    if (hxc_l_hudTextureReady)
    {
      if (hxc_l_points >= 2)
      {
        hxc_caxecraft_app_CaxecraftAtlas_drawHudGlyph(hxc_l_hudTexture, hxc_caxecraft_app_HudGlyph_HealthFull, hxc_l_x, hxc_l_layout.hxc_health.hxc_y, hxc_l_layout.hxc_heartSize);
      }
      else
      {
        if (hxc_l_points == 1)
        {
          hxc_caxecraft_app_CaxecraftAtlas_drawHudGlyph(hxc_l_hudTexture, hxc_caxecraft_app_HudGlyph_HealthHalf, hxc_l_x, hxc_l_layout.hxc_health.hxc_y, hxc_l_layout.hxc_heartSize);
        }
        else
        {
          hxc_caxecraft_app_CaxecraftAtlas_drawHudGlyph(hxc_l_hudTexture, hxc_caxecraft_app_HudGlyph_HealthEmpty, hxc_l_x, hxc_l_layout.hxc_health.hxc_y, hxc_l_layout.hxc_heartSize);
        }
      }
    }
    else
    {
      if (hxc_l_points > 0)
      {
        int32_t hxc_l_y_h1e28f0776181 = hxc_i32_add_wrapping(hxc_l_layout.hxc_health.hxc_y, 4);
        int32_t hxc_l_width_h852184939ce3 = hxc_i32_subtract_wrapping(hxc_l_layout.hxc_heartSize, 8);
        int32_t hxc_l_height_h147379500232 = hxc_i32_subtract_wrapping(hxc_l_layout.hxc_heartSize, 12);
        struct Color hxc_l_this1_h716355ff29b8 = (struct Color){ .r = 218, .g = 65, .b = 72, .a = 255 };
        int32_t hxc_l_tmp_load_result_n31 = hxc_l_x;
        int32_t hxc_l_tmp_load_result_n32 = hxc_l_y_h1e28f0776181;
        int32_t hxc_l_tmp_load_result_n33 = hxc_l_width_h852184939ce3;
        int32_t hxc_l_tmp_load_result_n34 = hxc_l_height_h147379500232;
        DrawRectangle((int32_t)hxc_l_tmp_load_result_n31, (int32_t)hxc_l_tmp_load_result_n32, (int32_t)hxc_l_tmp_load_result_n33, (int32_t)hxc_l_tmp_load_result_n34, hxc_l_this1_h716355ff29b8);
      }
      else
      {
        int32_t hxc_l_y_h16669888d9ab = hxc_i32_add_wrapping(hxc_l_layout.hxc_health.hxc_y, 4);
        int32_t hxc_l_width_h90196b2b3bdc = hxc_i32_subtract_wrapping(hxc_l_layout.hxc_heartSize, 8);
        int32_t hxc_l_height_hfaab7a6471e7 = hxc_i32_subtract_wrapping(hxc_l_layout.hxc_heartSize, 12);
        struct Color hxc_l_this1_he5d8be999c1a = (struct Color){ .r = 228, .g = 239, .b = 227, .a = 255 };
        int32_t hxc_l_tmp_load_result_n41 = hxc_l_x;
        int32_t hxc_l_tmp_load_result_n42 = hxc_l_y_h16669888d9ab;
        int32_t hxc_l_tmp_load_result_n43 = hxc_l_width_h90196b2b3bdc;
        int32_t hxc_l_tmp_load_result_n44 = hxc_l_height_hfaab7a6471e7;
        DrawRectangleLines((int32_t)hxc_l_tmp_load_result_n41, (int32_t)hxc_l_tmp_load_result_n42, (int32_t)hxc_l_tmp_load_result_n43, (int32_t)hxc_l_tmp_load_result_n44, hxc_l_this1_he5d8be999c1a);
      }
    }
    hxc_l_heart = hxc_i32_add_wrapping(hxc_l_heart, 1);
  }
  return;
}

void hxc_caxecraft_app_CaxecraftApp_drawHotbar(struct hxc_caxecraft_gameplay_InventoryState hxc_l_inventory, struct Texture hxc_l_hudTexture, bool hxc_l_hudTextureReady, struct Texture hxc_l_itemTexture, bool hxc_l_itemTextureReady, struct hxc_caxecraft_app_HudLayout hxc_l_layout)
{
  int32_t hxc_l_slotSize = hxc_l_layout.hxc_slotSize;
  int32_t hxc_l_gap = hxc_l_layout.hxc_slotGap;
  int32_t hxc_l_startX = hxc_l_layout.hxc_hotbar.hxc_x;
  int32_t hxc_l_y = hxc_l_layout.hxc_hotbar.hxc_y;
  int32_t hxc_l_slot = 0;
  while (1)
  {
    if (!(hxc_l_slot < 9))
    {
      break;
    }
    int32_t hxc_l_tmp_load_result_n7 = hxc_l_startX;
    int32_t hxc_l_tmp_load_result_n8 = hxc_l_slot;
    int32_t hxc_l_tmp_load_result_n9 = hxc_l_slotSize;
    int32_t hxc_l_x = hxc_i32_add_wrapping(hxc_l_tmp_load_result_n7, hxc_i32_multiply_wrapping(hxc_l_tmp_load_result_n8, hxc_i32_add_wrapping(hxc_l_tmp_load_result_n9, hxc_l_gap)));
    if (hxc_l_hudTextureReady)
    {
      if (hxc_l_slot == hxc_l_inventory.hxc_selected)
      {
        int32_t hxc_l_tmp_load_result_n14 = hxc_l_x;
        int32_t hxc_l_tmp_load_result_n15 = hxc_l_y;
        hxc_caxecraft_app_CaxecraftAtlas_drawHotbarFrame(hxc_l_hudTexture, hxc_caxecraft_app_HotbarFrame_Selected, hxc_l_tmp_load_result_n14, hxc_l_tmp_load_result_n15, hxc_l_slotSize);
      }
      else
      {
        int32_t hxc_l_tmp_load_result_n18 = hxc_l_x;
        int32_t hxc_l_tmp_load_result_n19 = hxc_l_y;
        hxc_caxecraft_app_CaxecraftAtlas_drawHotbarFrame(hxc_l_hudTexture, hxc_caxecraft_app_HotbarFrame_Normal, hxc_l_tmp_load_result_n18, hxc_l_tmp_load_result_n19, hxc_l_slotSize);
      }
    }
    else
    {
      struct Color hxc_l_this1 = (struct Color){ .r = 15, .g = 27, .b = 39, .a = 220 };
      int32_t hxc_l_tmp_load_result_n22 = hxc_l_x;
      int32_t hxc_l_tmp_load_result_n23 = hxc_l_y;
      int32_t hxc_l_tmp_load_result_n24 = hxc_l_slotSize;
      int32_t hxc_l_tmp_load_result_n25 = hxc_l_slotSize;
      DrawRectangle((int32_t)hxc_l_tmp_load_result_n22, (int32_t)hxc_l_tmp_load_result_n23, (int32_t)hxc_l_tmp_load_result_n24, (int32_t)hxc_l_tmp_load_result_n25, hxc_l_this1);
      int32_t hxc_l_tmp_load_result_n27 = hxc_l_slot;
      struct Color hxc_l_tmp_conditional_result_n14 = { 0 };
      if (hxc_l_tmp_load_result_n27 == hxc_l_inventory.hxc_selected)
      {
        hxc_l_tmp_conditional_result_n14 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
      }
      else
      {
        hxc_l_tmp_conditional_result_n14 = (struct Color){ .r = 228, .g = 239, .b = 227, .a = 255 };
      }
      struct Color hxc_l_color = hxc_l_tmp_conditional_result_n14;
      int32_t hxc_l_tmp_load_result_n32 = hxc_l_x;
      int32_t hxc_l_tmp_load_result_n33 = hxc_l_y;
      int32_t hxc_l_tmp_load_result_n34 = hxc_l_slotSize;
      int32_t hxc_l_tmp_load_result_n35 = hxc_l_slotSize;
      DrawRectangleLines((int32_t)hxc_l_tmp_load_result_n32, (int32_t)hxc_l_tmp_load_result_n33, (int32_t)hxc_l_tmp_load_result_n34, (int32_t)hxc_l_tmp_load_result_n35, hxc_l_color);
    }
    if (hxc_l_itemTextureReady)
    {
      enum hxc_caxecraft_gameplay_ItemKind hxc_l_tmp_call_result_n38 = hxc_caxecraft_gameplay_Inventory_itemAt(hxc_l_slot);
      int32_t hxc_l_tmp_load_result_n39 = hxc_l_x;
      int32_t hxc_l_tmp_load_result_n40 = hxc_l_y;
      hxc_caxecraft_app_CaxecraftAtlas_drawItem(hxc_l_itemTexture, hxc_l_tmp_call_result_n38, hxc_i32_add_wrapping(hxc_l_tmp_load_result_n39, 6), hxc_i32_add_wrapping(hxc_l_tmp_load_result_n40, 4), hxc_i32_subtract_wrapping(hxc_l_slotSize, 12));
    }
    int32_t hxc_l_tmp_call_result_n43 = hxc_caxecraft_gameplay_Inventory_countAt(hxc_l_inventory, hxc_l_slot);
    int32_t hxc_l_tmp_load_result_n44 = hxc_l_x;
    int32_t hxc_l_tmp_load_result_n45 = hxc_l_slotSize;
    int32_t hxc_l_tmp_load_result_n46 = hxc_l_y;
    hxc_caxecraft_app_HudDigits_drawNumber(hxc_l_tmp_call_result_n43, hxc_i32_subtract_wrapping(hxc_i32_add_wrapping(hxc_l_tmp_load_result_n44, hxc_l_tmp_load_result_n45), 25), hxc_i32_subtract_wrapping(hxc_i32_add_wrapping(hxc_l_tmp_load_result_n46, hxc_l_slotSize), 20), 2, (struct Color){ .r = 228, .g = 239, .b = 227, .a = 255 });
    hxc_l_slot = hxc_i32_add_wrapping(hxc_l_slot, 1);
  }
  return;
}

void hxc_caxecraft_app_CaxecraftApp_drawHud(struct hxc_caxecraft_app_HudView hxc_l_view, struct hxc_caxecraft_app_HudResources hxc_l_resources, struct hxc_caxecraft_app_RuntimeTextureAtlasCatalog *hxc_l_runtimeTextures, struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_contentRegistry, struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_uiCatalog)
{
  const void *volatile hxc_l_gc_roots[5] = { NULL, NULL, NULL, NULL, NULL };
  hxc_l_gc_roots[0] = (const void *)hxc_l_view.hxc_presentation;
  hxc_l_gc_roots[1] = (const void *)hxc_l_runtimeTextures;
  hxc_l_gc_roots[2] = (const void *)hxc_l_contentRegistry;
  hxc_l_gc_roots[3] = (const void *)hxc_l_uiCatalog;
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 5, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  bool hxc_l_paused = hxc_l_view.hxc_paused;
  int32_t hxc_l_mode = hxc_l_view.hxc_mode;
  int32_t hxc_l_locale = hxc_l_view.hxc_locale;
  struct hxc_caxecraft_gameplay_InventoryState hxc_l_inventory = hxc_l_view.hxc_inventory;
  struct hxc_optional_caxecraft_app_ConversationState hxc_l_conversation = hxc_l_view.hxc_conversation;
  struct hxc_caxecraft_domain_VitalsState hxc_l_vitals = hxc_l_view.hxc_character.hxc_vitals;
  struct Texture hxc_l_hudTexture = hxc_l_resources.hxc_hudTexture;
  bool hxc_l_hudTextureReady = hxc_l_resources.hxc_hudTextureReady;
  struct Texture hxc_l_itemTexture = hxc_l_resources.hxc_itemTexture;
  bool hxc_l_itemTextureReady = hxc_l_resources.hxc_itemTextureReady;
  struct Texture hxc_l_adventureItemTexture = hxc_l_resources.hxc_adventureItemTexture;
  bool hxc_l_adventureItemTextureReady = hxc_l_resources.hxc_adventureItemTextureReady;
  int32_t hxc_l_aquaticEquipmentCode = hxc_l_view.hxc_character.hxc_aquaticEquipmentCode;
  bool hxc_l_headSubmerged = hxc_l_view.hxc_character.hxc_headSubmerged;
  int32_t hxc_l_breathTicks = hxc_l_view.hxc_character.hxc_breathTicks;
  int32_t hxc_l_maximumBreathTicks = hxc_l_view.hxc_character.hxc_maximumBreathTicks;
  hxc_l_gc_roots[4] = (const void *)hxc_l_view.hxc_presentation;
  struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_presentation = hxc_l_view.hxc_presentation;
  (void)hxc_l_presentation;
  int32_t hxc_l_tmp_native_call_result_n22 = GetScreenWidth();
  int32_t hxc_l_width = (int32_t)hxc_l_tmp_native_call_result_n22;
  int32_t hxc_l_tmp_native_call_result_n23 = GetScreenHeight();
  int32_t hxc_l_height = (int32_t)hxc_l_tmp_native_call_result_n23;
  int32_t hxc_l_centerX = hxc_l_width / 2;
  int32_t hxc_l_centerY = hxc_l_height / 2;
  int32_t hxc_l_tmp_load_result_n26 = hxc_l_width;
  struct hxc_caxecraft_app_HudLayout hxc_l_tmp_call_result_n28 = hxc_caxecraft_app_HudLayout_hudLayout(hxc_l_tmp_load_result_n26, hxc_l_height, 9);
  struct hxc_caxecraft_app_HudLayout hxc_l_layout = hxc_l_tmp_call_result_n28;
  struct Color hxc_l_text = (struct Color){ .r = 228, .g = 239, .b = 227, .a = 255 };
  int32_t hxc_l_tmp_record_field_load_result_n30 = hxc_l_vitals.hxc_health;
  bool hxc_l_tmp_load_result_n31 = hxc_l_paused;
  enum hxc_caxecraft_app_HudReplacementState hxc_l_tmp_call_result_n33 = hxc_caxecraft_app_HudLayout_hudReplacementState(hxc_l_tmp_record_field_load_result_n30 <= 0, hxc_l_tmp_load_result_n31, hxc_l_conversation.hxc_has_value);
  switch (hxc_l_tmp_call_result_n33) {
    case hxc_caxecraft_app_HudReplacementState_NormalHud:
      {
        bool hxc_l_tmp_record_field_project_n35 = hxc_l_view.hxc_hit.hxc_hit;
        bool hxc_l_tmp_short_circuit_result_n29 = hxc_l_tmp_record_field_project_n35;
        if (!hxc_l_tmp_record_field_project_n35)
        {
          hxc_l_tmp_short_circuit_result_n29 = hxc_l_view.hxc_interactionPrompt != hxc_caxecraft_app_InteractionPrompt_NoInteractionPrompt;
        }
        bool hxc_l_targetAvailable = hxc_l_tmp_short_circuit_result_n29;
        struct hxc_caxecraft_app_HudLayout hxc_l_tmp_load_result_n39 = hxc_l_layout;
        struct Texture hxc_l_tmp_load_result_n40 = hxc_l_hudTexture;
        bool hxc_l_tmp_load_result_n41 = hxc_l_hudTextureReady;
        int32_t hxc_l_tmp_load_result_n42 = hxc_l_centerX;
        int32_t hxc_l_tmp_load_result_n43 = hxc_l_centerY;
        bool hxc_l_tmp_load_result_n44 = hxc_l_targetAvailable;
        hxc_caxecraft_app_CaxecraftApp_drawCrosshair(hxc_l_tmp_load_result_n39, hxc_l_tmp_load_result_n40, hxc_l_tmp_load_result_n41, hxc_l_tmp_load_result_n42, hxc_l_tmp_load_result_n43, hxc_l_tmp_load_result_n44, hxc_l_text);
        int32_t hxc_l_tmp_load_result_n46 = hxc_l_mode;
        bool hxc_l_tmp_short_circuit_result_n30 = hxc_l_tmp_load_result_n46 == 1;
        if (hxc_l_tmp_load_result_n46 == 1)
        {
          hxc_string hxc_l_tmp_record_field_project_n47 = hxc_l_view.hxc_objectiveTitle;
          if (hxc_l_tmp_record_field_project_n47.data == NULL)
          {
            abort();
          }
          int32_t hxc_l_tmp_string_length_result_n48;
          if (hxc_string_haxe_length(hxc_l_tmp_record_field_project_n47, &hxc_l_tmp_string_length_result_n48) != HXC_STATUS_OK)
          {
            abort();
          }
          hxc_l_tmp_short_circuit_result_n30 = hxc_l_tmp_string_length_result_n48 > 0;
        }
        if (hxc_l_tmp_short_circuit_result_n30)
        {
          struct hxc_caxecraft_app_HudLayout hxc_l_tmp_load_result_n50 = hxc_l_layout;
          struct Texture hxc_l_tmp_load_result_n53 = hxc_l_hudTexture;
          bool hxc_l_tmp_load_result_n54 = hxc_l_hudTextureReady;
          hxc_caxecraft_app_CaxecraftApp_drawObjective(hxc_l_tmp_load_result_n50, hxc_l_view.hxc_levelTitle, hxc_l_view.hxc_objectiveTitle, hxc_l_tmp_load_result_n53, hxc_l_tmp_load_result_n54, hxc_l_text);
        }
        struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_load_result_n56 = hxc_l_inventory;
        struct Texture hxc_l_tmp_load_result_n57 = hxc_l_hudTexture;
        bool hxc_l_tmp_load_result_n58 = hxc_l_hudTextureReady;
        struct Texture hxc_l_tmp_load_result_n59 = hxc_l_itemTexture;
        bool hxc_l_tmp_load_result_n60 = hxc_l_itemTextureReady;
        hxc_caxecraft_app_CaxecraftApp_drawHotbar(hxc_l_tmp_load_result_n56, hxc_l_tmp_load_result_n57, hxc_l_tmp_load_result_n58, hxc_l_tmp_load_result_n59, hxc_l_tmp_load_result_n60, hxc_l_layout);
        struct hxc_caxecraft_domain_VitalsState hxc_l_tmp_load_result_n62 = hxc_l_vitals;
        struct Texture hxc_l_tmp_load_result_n63 = hxc_l_hudTexture;
        bool hxc_l_tmp_load_result_n64 = hxc_l_hudTextureReady;
        hxc_caxecraft_app_CaxecraftApp_drawHealth(hxc_l_tmp_load_result_n62, hxc_l_tmp_load_result_n63, hxc_l_tmp_load_result_n64, hxc_l_layout);
        if (hxc_l_aquaticEquipmentCode >= 0)
        {
          int32_t hxc_l_tmp_load_result_n67 = hxc_l_aquaticEquipmentCode;
          struct Texture hxc_l_tmp_load_result_n68 = hxc_l_itemTexture;
          bool hxc_l_tmp_load_result_n69 = hxc_l_itemTextureReady;
          struct Texture hxc_l_tmp_load_result_n70 = hxc_l_adventureItemTexture;
          bool hxc_l_tmp_load_result_n71 = hxc_l_adventureItemTextureReady;
          int32_t hxc_l_tmp_record_field_load_result_n72 = hxc_l_layout.hxc_equipment.hxc_x;
          int32_t hxc_l_tmp_record_field_load_result_n73 = hxc_l_layout.hxc_equipment.hxc_y;
          hxc_caxecraft_app_AuthoredItemRenderer_drawEquippedIcon(hxc_l_contentRegistry, hxc_l_tmp_load_result_n67, hxc_l_tmp_load_result_n68, hxc_l_tmp_load_result_n69, hxc_l_tmp_load_result_n70, hxc_l_tmp_load_result_n71, hxc_l_tmp_record_field_load_result_n72, hxc_l_tmp_record_field_load_result_n73, hxc_l_layout.hxc_equipment.hxc_width);
        }
        if (hxc_l_headSubmerged)
        {
          int32_t hxc_l_tmp_load_result_n76 = hxc_l_breathTicks;
          int32_t hxc_l_tmp_load_result_n77 = hxc_l_maximumBreathTicks;
          struct hxc_caxecraft_app_HudLayout hxc_l_tmp_load_result_n78 = hxc_l_layout;
          hxc_caxecraft_app_CaxecraftApp_drawBreath(hxc_l_tmp_load_result_n76, hxc_l_tmp_load_result_n77, hxc_l_tmp_load_result_n78, hxc_l_width);
        }
        struct hxc_caxecraft_app_HudLayout hxc_l_tmp_load_result_n80 = hxc_l_layout;
        int32_t hxc_l_tmp_load_result_n81 = hxc_l_locale;
        hxc_caxecraft_app_CaxecraftApp_drawEnemyAlert(hxc_l_view, hxc_l_tmp_load_result_n80, hxc_l_tmp_load_result_n81, hxc_l_text);
        struct hxc_caxecraft_app_HudLayout hxc_l_tmp_load_result_n83 = hxc_l_layout;
        int32_t hxc_l_tmp_load_result_n84 = hxc_l_locale;
        hxc_caxecraft_app_CaxecraftApp_drawActionNotice(hxc_l_view, hxc_l_tmp_load_result_n83, hxc_l_uiCatalog, hxc_l_tmp_load_result_n84, hxc_l_text);
        if (hxc_l_view.hxc_debugMetricsVisible)
        {
          struct hxc_caxecraft_app_HudLayout hxc_l_tmp_load_result_n87 = hxc_l_layout;
          int32_t hxc_l_tmp_load_result_n88 = hxc_l_locale;
          hxc_caxecraft_app_CaxecraftApp_drawDiagnostics(hxc_l_view, hxc_l_tmp_load_result_n87, hxc_l_uiCatalog, hxc_l_tmp_load_result_n88, hxc_l_text);
        }
        if (hxc_l_vitals.hxc_safeTicks > 15)
        {
          struct Color hxc_l_this1 = (struct Color){ .r = 218, .g = 65, .b = 72, .a = 255 };
          int32_t hxc_l_tmp_load_result_n92 = hxc_l_width;
          int32_t hxc_l_tmp_load_result_n93 = hxc_l_height;
          DrawRectangleLines((int32_t)4, (int32_t)4, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n92, 8), (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n93, 8), hxc_l_this1);
        }
        break;
      }
    case hxc_caxecraft_app_HudReplacementState_ConversationHud:
      {
        int32_t hxc_l_tmp_load_result_n95 = hxc_l_locale;
        int32_t hxc_l_tmp_load_result_n96 = hxc_l_width;
        int32_t hxc_l_tmp_load_result_n97 = hxc_l_height;
        hxc_caxecraft_app_CaxecraftApp_drawConversation(hxc_l_view, hxc_l_resources, hxc_l_runtimeTextures, hxc_l_uiCatalog, hxc_l_tmp_load_result_n95, hxc_l_tmp_load_result_n96, hxc_l_tmp_load_result_n97, hxc_l_text);
        break;
      }
    case hxc_caxecraft_app_HudReplacementState_PausedHud:
      {
        int32_t hxc_l_tmp_load_result_n99 = hxc_l_width;
        int32_t hxc_l_tmp_load_result_n100 = hxc_l_height;
        hxc_caxecraft_app_CaxecraftApp_drawPause(hxc_l_view, hxc_l_uiCatalog, hxc_l_tmp_load_result_n99, hxc_l_tmp_load_result_n100, hxc_l_text);
        break;
      }
    case hxc_caxecraft_app_HudReplacementState_DefeatedHud:
      {
        int32_t hxc_l_tmp_load_result_n102 = hxc_l_width;
        int32_t hxc_l_tmp_load_result_n103 = hxc_l_height;
        hxc_caxecraft_app_CaxecraftApp_drawDefeat(hxc_l_view, hxc_l_uiCatalog, hxc_l_tmp_load_result_n102, hxc_l_tmp_load_result_n103, hxc_l_text);
        break;
      }
    default:
      {
        abort();
      }
  }
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return;
}

void hxc_caxecraft_app_CaxecraftApp_drawNoticePanel(struct hxc_caxecraft_app_HudRectangle hxc_l_panel, hxc_string hxc_l_value, int32_t hxc_l_fontSize, struct Color hxc_l_color, struct Color hxc_l_border, int32_t hxc_l_maximumCharacters)
{
  int32_t hxc_l_x = hxc_l_panel.hxc_x;
  int32_t hxc_l_y_h04a85cdea06f = hxc_l_panel.hxc_y;
  int32_t hxc_l_width_hfc908bbea26f = hxc_l_panel.hxc_width;
  int32_t hxc_l_height_hbef6ddefef98 = hxc_l_panel.hxc_height;
  struct Color hxc_l_this1 = (struct Color){ .r = 15, .g = 27, .b = 39, .a = 220 };
  int32_t hxc_l_tmp_load_result_n5 = hxc_l_x;
  int32_t hxc_l_tmp_load_result_n6 = hxc_l_y_h04a85cdea06f;
  int32_t hxc_l_tmp_load_result_n7 = hxc_l_width_hfc908bbea26f;
  int32_t hxc_l_tmp_load_result_n8 = hxc_l_height_hbef6ddefef98;
  DrawRectangle((int32_t)hxc_l_tmp_load_result_n5, (int32_t)hxc_l_tmp_load_result_n6, (int32_t)hxc_l_tmp_load_result_n7, (int32_t)hxc_l_tmp_load_result_n8, hxc_l_this1);
  int32_t hxc_l_y_hbf0a02cc77fc = hxc_l_panel.hxc_y;
  int32_t hxc_l_width_h985d0dc76ddc = hxc_l_panel.hxc_width;
  int32_t hxc_l_height_he075e0b490e7 = hxc_l_panel.hxc_height;
  int32_t hxc_l_tmp_load_result_n14 = hxc_l_y_hbf0a02cc77fc;
  int32_t hxc_l_tmp_load_result_n15 = hxc_l_width_h985d0dc76ddc;
  DrawRectangleLines((int32_t)hxc_l_panel.hxc_x, (int32_t)hxc_l_tmp_load_result_n14, (int32_t)hxc_l_tmp_load_result_n15, (int32_t)hxc_l_height_he075e0b490e7, hxc_l_border);
  hxc_caxecraft_app_CaxecraftApp_drawWrappedText(hxc_l_value, hxc_i32_add_wrapping(hxc_l_panel.hxc_x, 12), hxc_i32_add_wrapping(hxc_l_panel.hxc_y, 9), hxc_l_fontSize, hxc_l_maximumCharacters, hxc_i32_add_wrapping(hxc_l_fontSize, 2), 2, hxc_l_color);
  return;
}

void hxc_caxecraft_app_CaxecraftApp_drawObjective(struct hxc_caxecraft_app_HudLayout hxc_l_layout, hxc_string hxc_l_levelLabel, hxc_string hxc_l_objectiveTitle, struct Texture hxc_l_hudTexture, bool hxc_l_hudTextureReady, struct Color hxc_l_color)
{
  struct hxc_caxecraft_app_HudRectangle hxc_l_panel = hxc_l_layout.hxc_objective;
  int32_t hxc_l_x_h38efeffed649 = hxc_l_panel.hxc_x;
  int32_t hxc_l_y_h72f8519f9ffd = hxc_l_panel.hxc_y;
  int32_t hxc_l_width_hd8ddb9ef2819 = hxc_l_panel.hxc_width;
  int32_t hxc_l_height_h573578fadcd2 = hxc_l_panel.hxc_height;
  struct Color hxc_l_this1_ha1a2d97f58ee = (struct Color){ .r = 15, .g = 27, .b = 39, .a = 220 };
  int32_t hxc_l_tmp_load_result_n6 = hxc_l_x_h38efeffed649;
  int32_t hxc_l_tmp_load_result_n7 = hxc_l_y_h72f8519f9ffd;
  int32_t hxc_l_tmp_load_result_n8 = hxc_l_width_hd8ddb9ef2819;
  int32_t hxc_l_tmp_load_result_n9 = hxc_l_height_h573578fadcd2;
  DrawRectangle((int32_t)hxc_l_tmp_load_result_n6, (int32_t)hxc_l_tmp_load_result_n7, (int32_t)hxc_l_tmp_load_result_n8, (int32_t)hxc_l_tmp_load_result_n9, hxc_l_this1_ha1a2d97f58ee);
  int32_t hxc_l_x_h1bb803d532ef = hxc_l_panel.hxc_x;
  int32_t hxc_l_y_h2fd64b066d9c = hxc_l_panel.hxc_y;
  int32_t hxc_l_width_h0b9a7bbe06b2 = hxc_l_panel.hxc_width;
  int32_t hxc_l_height_hd264ad1ac386 = hxc_l_panel.hxc_height;
  struct Color hxc_l_this1_h6aa6c7ec0c83 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
  int32_t hxc_l_tmp_load_result_n16 = hxc_l_x_h1bb803d532ef;
  int32_t hxc_l_tmp_load_result_n17 = hxc_l_y_h2fd64b066d9c;
  int32_t hxc_l_tmp_load_result_n18 = hxc_l_width_h0b9a7bbe06b2;
  int32_t hxc_l_tmp_load_result_n19 = hxc_l_height_hd264ad1ac386;
  DrawRectangleLines((int32_t)hxc_l_tmp_load_result_n16, (int32_t)hxc_l_tmp_load_result_n17, (int32_t)hxc_l_tmp_load_result_n18, (int32_t)hxc_l_tmp_load_result_n19, hxc_l_this1_h6aa6c7ec0c83);
  if (hxc_l_hudTextureReady)
  {
    struct Texture hxc_l_tmp_static_call_argument_0_n17 = hxc_l_hudTexture;
    enum hxc_caxecraft_app_HudGlyph hxc_l_tmp_static_call_argument_1_n18 = hxc_caxecraft_app_HudGlyph_QuestMarker;
    int32_t hxc_l_tmp_static_call_argument_2_n19 = hxc_i32_add_wrapping(hxc_l_panel.hxc_x, 9);
    int32_t hxc_l_tmp_static_call_argument_3_n20 = hxc_i32_add_wrapping(hxc_l_panel.hxc_y, 9);
    int32_t hxc_l_tmp_conditional_result_n21 = 0;
    if (hxc_l_layout.hxc_compact)
    {
      hxc_l_tmp_conditional_result_n21 = 34;
    }
    else
    {
      hxc_l_tmp_conditional_result_n21 = 44;
    }
    int32_t hxc_l_tmp_conditional_load_result_n25 = hxc_l_tmp_conditional_result_n21;
    struct Texture hxc_l_tmp_static_call_argument_0_load_result_n26 = hxc_l_tmp_static_call_argument_0_n17;
    enum hxc_caxecraft_app_HudGlyph hxc_l_tmp_static_call_argument_1_load_result_n27 = hxc_l_tmp_static_call_argument_1_n18;
    int32_t hxc_l_tmp_static_call_argument_2_load_result_n28 = hxc_l_tmp_static_call_argument_2_n19;
    hxc_caxecraft_app_CaxecraftAtlas_drawHudGlyph(hxc_l_tmp_static_call_argument_0_load_result_n26, hxc_l_tmp_static_call_argument_1_load_result_n27, hxc_l_tmp_static_call_argument_2_load_result_n28, hxc_l_tmp_static_call_argument_3_n20, hxc_l_tmp_conditional_load_result_n25);
  }
  int32_t hxc_l_x_h28861f75ebaa = hxc_i32_add_wrapping(hxc_l_panel.hxc_x, 56);
  int32_t hxc_l_y_h638f5fae23c4 = hxc_i32_add_wrapping(hxc_l_panel.hxc_y, 8);
  int32_t hxc_l_tmp_conditional_result_n25 = 0;
  if (hxc_l_layout.hxc_compact)
  {
    hxc_l_tmp_conditional_result_n25 = 12;
  }
  else
  {
    hxc_l_tmp_conditional_result_n25 = 14;
  }
  int32_t hxc_l_fontSize = hxc_l_tmp_conditional_result_n25;
  struct Color hxc_l_this1_h279d0520bc65 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
  int32_t hxc_l_tmp_load_result_n35 = hxc_l_x_h28861f75ebaa;
  int32_t hxc_l_tmp_load_result_n36 = hxc_l_y_h638f5fae23c4;
  int32_t hxc_l_tmp_load_result_n37 = hxc_l_fontSize;
  struct Color hxc_l_tmp_load_result_n38 = hxc_l_this1_h279d0520bc65;
  hxc_call_cstring hxc_l_tmp_string_prepare_cstring_result_n39 = HXC_CALL_CSTRING_INITIALIZER;
  if (hxc_string_prepare_call_cstring(&hxc_l_levelLabel, hxc_default_allocator(), &hxc_l_tmp_string_prepare_cstring_result_n39) != HXC_STATUS_OK)
  {
    abort();
  }
  DrawText(hxc_l_tmp_string_prepare_cstring_result_n39.data, (int32_t)hxc_l_tmp_load_result_n35, (int32_t)hxc_l_tmp_load_result_n36, (int32_t)hxc_l_tmp_load_result_n37, hxc_l_tmp_load_result_n38);
  if (hxc_call_cstring_dispose(&hxc_l_tmp_string_prepare_cstring_result_n39) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_string hxc_l_tmp_static_call_argument_0_n27 = hxc_l_objectiveTitle;
  int32_t hxc_l_tmp_static_call_argument_1_n28 = hxc_i32_add_wrapping(hxc_l_panel.hxc_x, 56);
  int32_t hxc_l_tmp_static_call_argument_2_n29 = hxc_i32_add_wrapping(hxc_l_panel.hxc_y, 27);
  int32_t hxc_l_tmp_conditional_result_n30 = 0;
  if (hxc_l_layout.hxc_compact)
  {
    hxc_l_tmp_conditional_result_n30 = 13;
  }
  else
  {
    hxc_l_tmp_conditional_result_n30 = 14;
  }
  int32_t hxc_l_tmp_static_call_argument_3_n31 = hxc_l_tmp_conditional_result_n30;
  int32_t hxc_l_tmp_conditional_result_n32 = 0;
  if (hxc_l_layout.hxc_compact)
  {
    hxc_l_tmp_conditional_result_n32 = 31;
  }
  else
  {
    hxc_l_tmp_conditional_result_n32 = 42;
  }
  int32_t hxc_l_tmp_conditional_load_result_n45 = hxc_l_tmp_conditional_result_n32;
  hxc_string hxc_l_tmp_static_call_argument_0_load_result_n46 = hxc_l_tmp_static_call_argument_0_n27;
  int32_t hxc_l_tmp_static_call_argument_1_load_result_n47 = hxc_l_tmp_static_call_argument_1_n28;
  int32_t hxc_l_tmp_static_call_argument_2_load_result_n48 = hxc_l_tmp_static_call_argument_2_n29;
  hxc_caxecraft_app_CaxecraftApp_drawWrappedText(hxc_l_tmp_static_call_argument_0_load_result_n46, hxc_l_tmp_static_call_argument_1_load_result_n47, hxc_l_tmp_static_call_argument_2_load_result_n48, hxc_l_tmp_static_call_argument_3_n31, hxc_l_tmp_conditional_load_result_n45, 16, 2, hxc_l_color);
  return;
}

void hxc_caxecraft_app_CaxecraftApp_drawPause(struct hxc_caxecraft_app_HudView hxc_l_view, struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_uiCatalog, int32_t hxc_l_width_hf32a215f4e06, int32_t hxc_l_height_h274b1254a435, struct Color hxc_l_color)
{
  const void *volatile hxc_l_gc_roots[2] = { NULL, NULL };
  hxc_l_gc_roots[0] = (const void *)hxc_l_view.hxc_presentation;
  hxc_l_gc_roots[1] = (const void *)hxc_l_uiCatalog;
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 2, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_string hxc_l_tmp_cstring_arg_owner_owner_n20 = { 0 };
  hxc_string hxc_l_text = { 0 };
  hxc_string hxc_l_message = { 0 };
  hxc_string hxc_l_tmp_cstring_arg_owner_owner_n29 = { 0 };
  hxc_string hxc_l_tmp_record_field_project_n0 = hxc_l_view.hxc_journalTitle;
  if (hxc_l_tmp_record_field_project_n0.data == NULL)
  {
    abort();
  }
  int32_t hxc_l_tmp_string_length_result_n1;
  if (hxc_string_haxe_length(hxc_l_tmp_record_field_project_n0, &hxc_l_tmp_string_length_result_n1) != HXC_STATUS_OK)
  {
    abort();
  }
  bool hxc_l_tmp_short_circuit_result_n6 = hxc_l_tmp_string_length_result_n1 > 0;
  if (!(hxc_l_tmp_string_length_result_n1 > 0))
  {
    hxc_string hxc_l_tmp_record_field_project_n2 = hxc_l_view.hxc_journalBody;
    if (hxc_l_tmp_record_field_project_n2.data == NULL)
    {
      abort();
    }
    int32_t hxc_l_tmp_string_length_result_n3;
    if (hxc_string_haxe_length(hxc_l_tmp_record_field_project_n2, &hxc_l_tmp_string_length_result_n3) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_short_circuit_result_n6 = hxc_l_tmp_string_length_result_n3 > 0;
  }
  bool hxc_l_hasJournal = hxc_l_tmp_short_circuit_result_n6;
  struct hxc_caxecraft_app_HudModalLayout hxc_l_tmp_call_result_n6 = hxc_caxecraft_app_HudLayout_hudModalLayout(hxc_l_width_hf32a215f4e06, hxc_l_height_h274b1254a435, hxc_l_hasJournal);
  struct hxc_caxecraft_app_HudRectangle hxc_l_panel = hxc_l_tmp_call_result_n6.hxc_pause;
  int32_t hxc_l_x_h3517546dc80d = hxc_l_panel.hxc_x;
  int32_t hxc_l_y_hee1e6c7bc568 = hxc_l_panel.hxc_y;
  int32_t hxc_l_width_h02205d3954bf = hxc_l_panel.hxc_width;
  int32_t hxc_l_height_hf70f30264fe3 = hxc_l_panel.hxc_height;
  struct Color hxc_l_this1_h6a62894f5352 = (struct Color){ .r = 15, .g = 27, .b = 39, .a = 220 };
  int32_t hxc_l_tmp_load_result_n13 = hxc_l_x_h3517546dc80d;
  int32_t hxc_l_tmp_load_result_n14 = hxc_l_y_hee1e6c7bc568;
  int32_t hxc_l_tmp_load_result_n15 = hxc_l_width_h02205d3954bf;
  int32_t hxc_l_tmp_load_result_n16 = hxc_l_height_hf70f30264fe3;
  DrawRectangle((int32_t)hxc_l_tmp_load_result_n13, (int32_t)hxc_l_tmp_load_result_n14, (int32_t)hxc_l_tmp_load_result_n15, (int32_t)hxc_l_tmp_load_result_n16, hxc_l_this1_h6a62894f5352);
  int32_t hxc_l_x_h17b4ceadf966 = hxc_l_panel.hxc_x;
  int32_t hxc_l_y_h874898f67e41 = hxc_l_panel.hxc_y;
  int32_t hxc_l_width_h1078f6fa332a = hxc_l_panel.hxc_width;
  int32_t hxc_l_height_hb2c7ebae8e7e = hxc_l_panel.hxc_height;
  struct Color hxc_l_this1_hb096de8ec5db = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
  int32_t hxc_l_tmp_load_result_n23 = hxc_l_x_h17b4ceadf966;
  int32_t hxc_l_tmp_load_result_n24 = hxc_l_y_h874898f67e41;
  int32_t hxc_l_tmp_load_result_n25 = hxc_l_width_h1078f6fa332a;
  int32_t hxc_l_tmp_load_result_n26 = hxc_l_height_hb2c7ebae8e7e;
  DrawRectangleLines((int32_t)hxc_l_tmp_load_result_n23, (int32_t)hxc_l_tmp_load_result_n24, (int32_t)hxc_l_tmp_load_result_n25, (int32_t)hxc_l_tmp_load_result_n26, hxc_l_this1_hb096de8ec5db);
  int32_t hxc_l_x_hd59e8622a9d1 = hxc_i32_add_wrapping(hxc_l_panel.hxc_x, 30);
  int32_t hxc_l_y_h8302c1bfe5cf = hxc_i32_add_wrapping(hxc_l_panel.hxc_y, 18);
  if (hxc_l_uiCatalog == NULL)
  {
    abort();
  }
  hxc_string hxc_l_tmp_instance_call_result_n31 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_uiCatalog, hxc_l_view.hxc_locale, (hxc_string){ (const uint8_t *)"pause_title", 11, true, NULL });
  hxc_l_tmp_cstring_arg_owner_owner_n20 = hxc_l_tmp_instance_call_result_n31;
  hxc_string hxc_l_tmp_cstring_arg_owner_borrow_result_n32 = hxc_l_tmp_cstring_arg_owner_owner_n20;
  int32_t hxc_l_tmp_load_result_n33 = hxc_l_x_hd59e8622a9d1;
  int32_t hxc_l_tmp_load_result_n34 = hxc_l_y_h8302c1bfe5cf;
  hxc_call_cstring hxc_l_tmp_string_prepare_cstring_result_n35 = HXC_CALL_CSTRING_INITIALIZER;
  if (hxc_string_prepare_call_cstring(&hxc_l_tmp_cstring_arg_owner_borrow_result_n32, hxc_default_allocator(), &hxc_l_tmp_string_prepare_cstring_result_n35) != HXC_STATUS_OK)
  {
    abort();
  }
  DrawText(hxc_l_tmp_string_prepare_cstring_result_n35.data, (int32_t)hxc_l_tmp_load_result_n33, (int32_t)hxc_l_tmp_load_result_n34, (int32_t)24, hxc_l_color);
  if (hxc_call_cstring_dispose(&hxc_l_tmp_string_prepare_cstring_result_n35) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_l_hasJournal)
  {
    hxc_l_text = hxc_l_view.hxc_journalTitle;
    if (hxc_string_retain(hxc_l_text) != HXC_STATUS_OK)
    {
      abort();
    }
    int32_t hxc_l_x_h08b87fa3fa4d = hxc_i32_add_wrapping(hxc_l_panel.hxc_x, 30);
    int32_t hxc_l_y_h9e95b6826b66 = hxc_i32_add_wrapping(hxc_l_panel.hxc_y, 68);
    struct Color hxc_l_this1_h1f8371d50717 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
    hxc_string hxc_l_tmp_load_result_n41 = hxc_l_text;
    int32_t hxc_l_tmp_load_result_n42 = hxc_l_x_h08b87fa3fa4d;
    int32_t hxc_l_tmp_load_result_n43 = hxc_l_y_h9e95b6826b66;
    struct Color hxc_l_tmp_load_result_n44 = hxc_l_this1_h1f8371d50717;
    hxc_call_cstring hxc_l_tmp_string_prepare_cstring_result_n45 = HXC_CALL_CSTRING_INITIALIZER;
    if (hxc_string_prepare_call_cstring(&hxc_l_tmp_load_result_n41, hxc_default_allocator(), &hxc_l_tmp_string_prepare_cstring_result_n45) != HXC_STATUS_OK)
    {
      abort();
    }
    DrawText(hxc_l_tmp_string_prepare_cstring_result_n45.data, (int32_t)hxc_l_tmp_load_result_n42, (int32_t)hxc_l_tmp_load_result_n43, (int32_t)18, hxc_l_tmp_load_result_n44);
    if (hxc_call_cstring_dispose(&hxc_l_tmp_string_prepare_cstring_result_n45) != HXC_STATUS_OK)
    {
      abort();
    }
    int32_t hxc_l_tmp_record_field_load_result_n47 = hxc_l_panel.hxc_x;
    int32_t hxc_l_tmp_record_field_load_result_n48 = hxc_l_panel.hxc_y;
    hxc_caxecraft_app_CaxecraftApp_drawWrappedText(hxc_l_view.hxc_journalBody, hxc_i32_add_wrapping(hxc_l_tmp_record_field_load_result_n47, 30), hxc_i32_add_wrapping(hxc_l_tmp_record_field_load_result_n48, 104), 16, hxc_i32_subtract_wrapping(hxc_l_panel.hxc_width, 60) / 8, 22, 3, hxc_l_color);
    if (hxc_string_release(&hxc_l_text) != HXC_STATUS_OK)
    {
      abort();
    }
  }
  int32_t hxc_l_locale = hxc_l_view.hxc_locale;
  hxc_string hxc_l_tmp_call_result_n52 = hxc_caxecraft_input_ControlPrompts_pauseHelpMessage(hxc_l_view.hxc_promptDevice);
  hxc_l_message = hxc_l_tmp_call_result_n52;
  int32_t hxc_l_x_hbb313ecaca8b = hxc_i32_add_wrapping(hxc_l_panel.hxc_x, 30);
  int32_t hxc_l_tmp_record_field_load_result_n54 = hxc_l_panel.hxc_y;
  int32_t hxc_l_y_h0f490dc400ff = hxc_i32_subtract_wrapping(hxc_i32_add_wrapping(hxc_l_tmp_record_field_load_result_n54, hxc_l_panel.hxc_height), 34);
  int32_t hxc_l_tmp_load_result_n56 = hxc_l_locale;
  hxc_string hxc_l_tmp_instance_call_result_n58 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_uiCatalog, hxc_l_tmp_load_result_n56, hxc_l_message);
  hxc_l_tmp_cstring_arg_owner_owner_n29 = hxc_l_tmp_instance_call_result_n58;
  hxc_string hxc_l_tmp_cstring_arg_owner_borrow_result_n59 = hxc_l_tmp_cstring_arg_owner_owner_n29;
  int32_t hxc_l_tmp_load_result_n60 = hxc_l_x_hbb313ecaca8b;
  int32_t hxc_l_tmp_load_result_n61 = hxc_l_y_h0f490dc400ff;
  hxc_call_cstring hxc_l_tmp_string_prepare_cstring_result_n62 = HXC_CALL_CSTRING_INITIALIZER;
  if (hxc_string_prepare_call_cstring(&hxc_l_tmp_cstring_arg_owner_borrow_result_n59, hxc_default_allocator(), &hxc_l_tmp_string_prepare_cstring_result_n62) != HXC_STATUS_OK)
  {
    abort();
  }
  DrawText(hxc_l_tmp_string_prepare_cstring_result_n62.data, (int32_t)hxc_l_tmp_load_result_n60, (int32_t)hxc_l_tmp_load_result_n61, (int32_t)16, hxc_l_color);
  if (hxc_call_cstring_dispose(&hxc_l_tmp_string_prepare_cstring_result_n62) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_string_release(&hxc_l_tmp_cstring_arg_owner_owner_n29) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_string_release(&hxc_l_message) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_string_release(&hxc_l_tmp_cstring_arg_owner_owner_n20) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return;
}

void hxc_caxecraft_app_CaxecraftApp_drawPlayerAvatar(struct Camera3D hxc_l_camera, struct hxc_caxecraft_app_RuntimeTextureAtlasCatalog *hxc_l_runtimeTextures, double hxc_l_x, double hxc_l_y, double hxc_l_z)
{
  const void *volatile hxc_l_gc_roots[1] = { NULL };
  hxc_l_gc_roots[0] = (const void *)hxc_l_runtimeTextures;
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 1, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_l_runtimeTextures == NULL)
  {
    abort();
  }
  bool hxc_l_tmp_instance_call_result_n1 = hxc_caxecraft_app_RuntimeTextureAtlasCatalog_drawSprite(hxc_l_runtimeTextures, hxc_l_camera, (hxc_string){ (const uint8_t *)"adventure-characters", 20, true, NULL }, 3, (struct Vector3){ .x = (float)hxc_l_x, .y = (float)(hxc_l_y + 0.71), .z = (float)hxc_l_z }, 0.78, 1.42);
  if (!hxc_l_tmp_instance_call_result_n1)
  {
    DrawCube((struct Vector3){ .x = (float)hxc_l_x, .y = (float)(hxc_l_y + 0.48), .z = (float)hxc_l_z }, (float)0.52, (float)0.96, (float)0.34, (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 });
    DrawCube((struct Vector3){ .x = (float)hxc_l_x, .y = (float)(hxc_l_y + 1.16), .z = (float)hxc_l_z }, (float)0.38, (float)0.36, (float)0.36, (struct Color){ .r = 228, .g = 239, .b = 227, .a = 255 });
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return;
  }
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return;
}

void hxc_caxecraft_app_CaxecraftApp_drawWorldSunAndClouds(bool hxc_l_hasSun, int32_t hxc_l_sunX, int32_t hxc_l_sunY, int32_t hxc_l_sunZ, int32_t hxc_l_sunRadiusMilli, int32_t hxc_l_cloudCount, int32_t hxc_l_cloudSpeedMilli, int32_t hxc_l_cloudSeed, double hxc_l_eyeX, double hxc_l_eyeY, double hxc_l_eyeZ, double hxc_l_simulationSeconds)
{
  if (hxc_l_hasSun)
  {
    struct Vector3 hxc_l_center = (struct Vector3){ .x = (float)(hxc_l_eyeX + (double)hxc_l_sunX), .y = (float)(hxc_l_eyeY + (double)hxc_l_sunY), .z = (float)(hxc_l_eyeZ + (double)hxc_l_sunZ) };
    double hxc_l_radius = hxc_f64_divide_zero_safe((double)hxc_l_sunRadiusMilli, 1000.0);
    struct Vector3 hxc_l_tmp_load_result_n1 = hxc_l_center;
    DrawSphere(hxc_l_tmp_load_result_n1, (float)hxc_l_radius, (struct Color){ .r = 255, .g = 246, .b = 205, .a = 255 });
    struct Vector3 hxc_l_tmp_load_result_n4 = hxc_l_center;
    DrawSphere(hxc_l_tmp_load_result_n4, (float)(hxc_l_radius * 1.4), (struct Color){ .r = 255, .g = 226, .b = 136, .a = 88 });
  }
  double hxc_l_cloudSpeed = hxc_f64_divide_zero_safe((double)hxc_l_cloudSpeedMilli, 1000.0);
  int32_t hxc_l_index = 0;
  while (1)
  {
    if (!(hxc_l_index < hxc_l_cloudCount))
    {
      break;
    }
    int32_t hxc_l_noise = hxc_i32_bit_and(hxc_i32_add_wrapping(hxc_i32_add_wrapping(hxc_i32_multiply_wrapping(hxc_l_cloudSeed, 1103515245), hxc_i32_multiply_wrapping(hxc_l_index, 12345)), 1013904223), 2147483647);
    double hxc_l_along = (double)hxc_i32_modulo_zero_safe(hxc_l_noise, 101) - 50.0;
    double hxc_l_distance = 48.0 + (double)hxc_i32_modulo_zero_safe(hxc_l_noise / 101, 25);
    double hxc_l_baseX = { 0 };
    switch (hxc_i32_modulo_zero_safe(hxc_l_index, 4)) {
      case 1:
        {
          hxc_l_baseX = hxc_l_distance;
          break;
        }
      case 0:
      case 2:
        {
          hxc_l_baseX = hxc_l_along;
          break;
        }
      default:
        {
          hxc_l_baseX = -hxc_l_distance;
          break;
        }
    }
    double hxc_l_baseZ = { 0 };
    switch (hxc_i32_modulo_zero_safe(hxc_l_index, 4)) {
      case 0:
        {
          hxc_l_baseZ = -hxc_l_distance;
          break;
        }
      case 1:
      case 3:
        {
          hxc_l_baseZ = hxc_l_along;
          break;
        }
      default:
        {
          hxc_l_baseZ = hxc_l_distance;
          break;
        }
    }
    double hxc_l_tmp_load_result_n19 = hxc_l_baseX;
    double hxc_l_tmp_load_result_n20 = hxc_l_baseZ;
    double hxc_l_tmp_load_result_n21 = hxc_l_cloudSpeed;
    hxc_caxecraft_app_CaxecraftApp_drawCloud(hxc_l_eyeX, hxc_l_eyeY, hxc_l_eyeZ, hxc_l_simulationSeconds, hxc_l_tmp_load_result_n19, hxc_l_tmp_load_result_n20, hxc_l_tmp_load_result_n21, hxc_i32_modulo_zero_safe(hxc_l_noise, 3));
    hxc_l_index = hxc_i32_add_wrapping(hxc_l_index, 1);
  }
  return;
}

void hxc_caxecraft_app_CaxecraftApp_drawWrappedText(hxc_string hxc_l_value, int32_t hxc_l_x, int32_t hxc_l_y, int32_t hxc_l_fontSize, int32_t hxc_l_maximumCharacters, int32_t hxc_l_lineHeight, int32_t hxc_l_maximumLines, struct Color hxc_l_color)
{
  hxc_string hxc_l_word = { 0 };
  hxc_string hxc_l_tmp_array_element_owner_n13 = { 0 };
  hxc_string hxc_l_candidate = { 0 };
  hxc_string hxc_l_tmp_string_concat_left_owner_n16 = { 0 };
  if (hxc_l_value.data == NULL)
  {
    abort();
  }
  if ((hxc_string){ (const uint8_t *)" ", 1, true, NULL }.data == NULL)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_tmp_string_split_result_n0 = NULL;
  if (hxc_string_split(hxc_l_value, (hxc_string){ (const uint8_t *)" ", 1, true, NULL }, hxc_default_allocator(), (hxc_array_element_ops){ sizeof(hxc_string), _Alignof(hxc_string), NULL, hxc_array_87af1a2c_element_copy, hxc_array_87af1a2c_element_assign, hxc_array_87af1a2c_element_destroy }, &hxc_l_tmp_string_split_result_n0) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_words = hxc_l_tmp_string_split_result_n0;
  hxc_string hxc_l_line = (hxc_string){ (const uint8_t *)"", 0, true, NULL };
  if (hxc_string_retain(hxc_l_line) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_lineIndex = 0;
  int32_t hxc_l_g = 0;
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n1 = hxc_l_g;
    int32_t hxc_l_tmp_array_length_result_n3;
    if (hxc_array_ref_length(hxc_l_words, &hxc_l_tmp_array_length_result_n3) != HXC_STATUS_OK)
    {
      abort();
    }
    if (!(hxc_l_tmp_load_result_n1 < hxc_l_tmp_array_length_result_n3))
    {
      break;
    }
    struct hxc_array_ref *hxc_l_tmp_load_result_n4 = hxc_l_words;
    hxc_string hxc_l_tmp_array_get_result_n6;
    if (hxc_array_ref_get_copy(hxc_l_tmp_load_result_n4, (size_t)hxc_l_g, &hxc_l_tmp_array_get_result_n6) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_array_element_owner_n13 = hxc_l_tmp_array_get_result_n6;
    hxc_l_word = hxc_l_tmp_array_element_owner_n13;
    if (hxc_string_retain(hxc_l_word) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_g = hxc_i32_add_wrapping(hxc_l_g, 1);
    hxc_string hxc_l_tmp_load_result_n9 = hxc_l_line;
    if (hxc_l_tmp_load_result_n9.data == NULL)
    {
      abort();
    }
    int32_t hxc_l_tmp_string_length_result_n10;
    if (hxc_string_haxe_length(hxc_l_tmp_load_result_n9, &hxc_l_tmp_string_length_result_n10) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_string hxc_l_tmp_conditional_managed_result_n15 = { 0 };
    if (hxc_l_tmp_string_length_result_n10 == 0)
    {
      (void)hxc_l_word;
      hxc_l_tmp_conditional_managed_result_n15 = hxc_l_word;
      if (hxc_string_retain(hxc_l_tmp_conditional_managed_result_n15) != HXC_STATUS_OK)
      {
        abort();
      }
    }
    else
    {
      hxc_string hxc_l_tmp_string_concat_result_n13 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
      if (hxc_string_concat_ref(hxc_l_line, (hxc_string){ (const uint8_t *)" ", 1, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n13) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_l_tmp_string_concat_left_owner_n16 = hxc_l_tmp_string_concat_result_n13;
      hxc_string hxc_l_tmp_string_concat_left_borrow_result_n14 = hxc_l_tmp_string_concat_left_owner_n16;
      hxc_string hxc_l_tmp_string_concat_result_n16 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
      if (hxc_string_concat_ref(hxc_l_tmp_string_concat_left_borrow_result_n14, hxc_l_word, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n16) != HXC_STATUS_OK)
      {
        abort();
      }
      (void)hxc_l_tmp_string_concat_result_n16;
      hxc_l_tmp_conditional_managed_result_n15 = hxc_l_tmp_string_concat_result_n16;
      if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n16) != HXC_STATUS_OK)
      {
        abort();
      }
    }
    hxc_string hxc_l_tmp_conditional_managed_move_result_n17 = hxc_l_tmp_conditional_managed_result_n15;
    hxc_l_candidate = hxc_l_tmp_conditional_managed_move_result_n17;
    hxc_string hxc_l_tmp_load_result_n18 = hxc_l_line;
    if (hxc_l_tmp_load_result_n18.data == NULL)
    {
      abort();
    }
    int32_t hxc_l_tmp_string_length_result_n19;
    if (hxc_string_haxe_length(hxc_l_tmp_load_result_n18, &hxc_l_tmp_string_length_result_n19) != HXC_STATUS_OK)
    {
      abort();
    }
    bool hxc_l_tmp_short_circuit_result_n17 = hxc_l_tmp_string_length_result_n19 > 0;
    if (hxc_l_tmp_string_length_result_n19 > 0)
    {
      hxc_string hxc_l_tmp_load_result_n20 = hxc_l_candidate;
      if (hxc_l_tmp_load_result_n20.data == NULL)
      {
        abort();
      }
      int32_t hxc_l_tmp_string_length_result_n21;
      if (hxc_string_haxe_length(hxc_l_tmp_load_result_n20, &hxc_l_tmp_string_length_result_n21) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_l_tmp_short_circuit_result_n17 = hxc_l_tmp_string_length_result_n21 > hxc_l_maximumCharacters;
    }
    if (hxc_l_tmp_short_circuit_result_n17)
    {
      if (hxc_l_lineIndex >= hxc_l_maximumLines)
      {
        if (hxc_string_release(&hxc_l_candidate) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_string_release(&hxc_l_word) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_string_release(&hxc_l_tmp_array_element_owner_n13) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_string_release(&hxc_l_line) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_array_ref_release(hxc_l_words) != HXC_STATUS_OK)
        {
          abort();
        }
        return;
      }
      else
      {
        hxc_string hxc_l_tmp_load_result_n24 = hxc_l_line;
        int32_t hxc_l_tmp_load_result_n25 = hxc_l_lineIndex;
        hxc_call_cstring hxc_l_tmp_string_prepare_cstring_result_n26 = HXC_CALL_CSTRING_INITIALIZER;
        if (hxc_string_prepare_call_cstring(&hxc_l_tmp_load_result_n24, hxc_default_allocator(), &hxc_l_tmp_string_prepare_cstring_result_n26) != HXC_STATUS_OK)
        {
          abort();
        }
        DrawText(hxc_l_tmp_string_prepare_cstring_result_n26.data, (int32_t)hxc_l_x, (int32_t)hxc_i32_add_wrapping(hxc_l_y, hxc_i32_multiply_wrapping(hxc_l_tmp_load_result_n25, hxc_l_lineHeight)), (int32_t)hxc_l_fontSize, hxc_l_color);
        if (hxc_call_cstring_dispose(&hxc_l_tmp_string_prepare_cstring_result_n26) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_l_lineIndex = hxc_i32_add_wrapping(hxc_l_lineIndex, 1);
        hxc_string hxc_l_tmp_string_assignment_replacement_owner_n18 = hxc_l_word;
        if (hxc_string_retain(hxc_l_tmp_string_assignment_replacement_owner_n18) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_string hxc_l_tmp_string_assignment_replacement_owned_load_result_n29 = hxc_l_tmp_string_assignment_replacement_owner_n18;
        if (hxc_string_release(&hxc_l_line) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_l_line = hxc_l_tmp_string_assignment_replacement_owned_load_result_n29;
      }
    }
    else
    {
      hxc_string hxc_l_tmp_string_assignment_replacement_owner_n19 = hxc_l_candidate;
      if (hxc_string_retain(hxc_l_tmp_string_assignment_replacement_owner_n19) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_string hxc_l_tmp_string_assignment_replacement_owned_load_result_n31 = hxc_l_tmp_string_assignment_replacement_owner_n19;
      if (hxc_string_release(&hxc_l_line) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_l_line = hxc_l_tmp_string_assignment_replacement_owned_load_result_n31;
    }
    if (hxc_string_release(&hxc_l_candidate) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_word) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_array_element_owner_n13) != HXC_STATUS_OK)
    {
      abort();
    }
  }
  hxc_string hxc_l_tmp_load_result_n32 = hxc_l_line;
  if (hxc_l_tmp_load_result_n32.data == NULL)
  {
    abort();
  }
  int32_t hxc_l_tmp_string_length_result_n33;
  if (hxc_string_haxe_length(hxc_l_tmp_load_result_n32, &hxc_l_tmp_string_length_result_n33) != HXC_STATUS_OK)
  {
    abort();
  }
  bool hxc_l_tmp_short_circuit_result_n20 = hxc_l_tmp_string_length_result_n33 > 0;
  if (hxc_l_tmp_string_length_result_n33 > 0)
  {
    hxc_l_tmp_short_circuit_result_n20 = hxc_l_lineIndex < hxc_l_maximumLines;
  }
  if (hxc_l_tmp_short_circuit_result_n20)
  {
    hxc_string hxc_l_tmp_load_result_n36 = hxc_l_line;
    int32_t hxc_l_tmp_load_result_n37 = hxc_l_lineIndex;
    hxc_call_cstring hxc_l_tmp_string_prepare_cstring_result_n38 = HXC_CALL_CSTRING_INITIALIZER;
    if (hxc_string_prepare_call_cstring(&hxc_l_tmp_load_result_n36, hxc_default_allocator(), &hxc_l_tmp_string_prepare_cstring_result_n38) != HXC_STATUS_OK)
    {
      abort();
    }
    DrawText(hxc_l_tmp_string_prepare_cstring_result_n38.data, (int32_t)hxc_l_x, (int32_t)hxc_i32_add_wrapping(hxc_l_y, hxc_i32_multiply_wrapping(hxc_l_tmp_load_result_n37, hxc_l_lineHeight)), (int32_t)hxc_l_fontSize, hxc_l_color);
    if (hxc_call_cstring_dispose(&hxc_l_tmp_string_prepare_cstring_result_n38) != HXC_STATUS_OK)
    {
      abort();
    }
  }
  if (hxc_string_release(&hxc_l_line) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_ref_release(hxc_l_words) != HXC_STATUS_OK)
  {
    abort();
  }
  return;
}

struct hxc_caxecraft_app_CaxecraftApp_AvailableInteractionTarget hxc_caxecraft_app_CaxecraftApp_nearestAvailableInteraction(struct hxc_caxecraft_domain_GameSession *hxc_l_session, struct hxc_caxecraft_app_PlayableLevelView *hxc_l_level)
{
  const void *volatile hxc_l_gc_roots[2] = { NULL, NULL };
  hxc_l_gc_roots[0] = (const void *)hxc_l_session;
  hxc_l_gc_roots[1] = (const void *)hxc_l_level;
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 2, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_caxecraft_app_PlayableDialogueActor hxc_l_tmp_array_element_owner_n11 = { 0 };
  hxc_string hxc_l_id_h33cf02b74d7d = { 0 };
  struct hxc_caxecraft_content_LoadedStatefulObjectBinding hxc_l_tmp_array_element_owner_n21 = { 0 };
  struct hxc_caxecraft_app_CaxecraftApp_AvailableInteractionTarget hxc_l_selected = { 0 };
  if (hxc_l_session == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_domain_GameView hxc_l_tmp_instance_call_result_n0 = hxc_caxecraft_domain_GameSession_view(hxc_l_session);
  struct hxc_caxecraft_domain_GameView hxc_l_view = hxc_l_tmp_instance_call_result_n0;
  if (!hxc_l_view.hxc_valid)
  {
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return (struct hxc_caxecraft_app_CaxecraftApp_AvailableInteractionTarget){ .hxc_tag = hxc_caxecraft_app_CaxecraftApp_AvailableInteractionTarget_NoAvailableInteraction };
  }
  struct hxc_caxecraft_domain_Character hxc_l_player = hxc_l_view.hxc_localPlayer;
  hxc_l_selected = (struct hxc_caxecraft_app_CaxecraftApp_AvailableInteractionTarget){ .hxc_tag = hxc_caxecraft_app_CaxecraftApp_AvailableInteractionTarget_NoAvailableInteraction };
  bool hxc_l_hasSelection = false;
  double hxc_l_selectedDistance = 0.0;
  int32_t hxc_l_g_h765e84b05247 = 0;
  if (hxc_l_level == NULL)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n5 = (*hxc_l_level).hxc_dialogueActors;
  int32_t hxc_l_tmp_array_length_result_n6;
  if (hxc_array_ref_length(hxc_l_tmp_class_field_load_result_n5, &hxc_l_tmp_array_length_result_n6) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_g1_hed0f9a52ee6f = hxc_l_tmp_array_length_result_n6;
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n7 = hxc_l_g_h765e84b05247;
    if (!(hxc_l_tmp_load_result_n7 < hxc_l_g1_hed0f9a52ee6f))
    {
      break;
    }
    int32_t hxc_l_tmp_increment_load_result_n9 = hxc_l_g_h765e84b05247;
    hxc_l_g_h765e84b05247 = hxc_i32_add_wrapping(hxc_l_tmp_increment_load_result_n9, 1);
    int32_t hxc_l_index_h8f1651a3f1fe = hxc_l_tmp_increment_load_result_n9;
    struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n10 = (*hxc_l_level).hxc_dialogueActors;
    struct hxc_caxecraft_app_PlayableDialogueActor hxc_l_tmp_array_get_result_n12;
    if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n10, (size_t)hxc_l_index_h8f1651a3f1fe, &hxc_l_tmp_array_get_result_n12) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_array_element_owner_n11 = hxc_l_tmp_array_get_result_n12;
    int32_t hxc_l_id_h56714afcdfb2 = hxc_l_tmp_array_element_owner_n11.hxc_entityId;
    bool hxc_l_tmp_instance_call_result_n16 = hxc_caxecraft_domain_GameSession_actorInteractionAvailable(hxc_l_session, hxc_l_id_h56714afcdfb2);
    if (hxc_l_tmp_instance_call_result_n16)
    {
      struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n18 = hxc_caxecraft_domain_GameSession_readCharacter(hxc_l_session, hxc_l_id_h56714afcdfb2);
      struct hxc_caxecraft_domain_Character hxc_l_actor = hxc_l_tmp_instance_call_result_n18;
      double hxc_l_tmp_record_field_load_result_n19 = hxc_l_actor.hxc_body.hxc_x;
      double hxc_l_dx_h8617ebe438d1 = hxc_l_tmp_record_field_load_result_n19 - hxc_l_player.hxc_body.hxc_x;
      double hxc_l_tmp_record_field_load_result_n21 = hxc_l_actor.hxc_body.hxc_z;
      double hxc_l_dz_h9de9817f4f27 = hxc_l_tmp_record_field_load_result_n21 - hxc_l_player.hxc_body.hxc_z;
      double hxc_l_tmp_load_result_n23 = hxc_l_dx_h8617ebe438d1;
      double hxc_l_tmp_load_result_n24 = hxc_l_dx_h8617ebe438d1;
      double hxc_l_tmp_load_result_n25 = hxc_l_dz_h9de9817f4f27;
      double hxc_l_distance_hc7d13aad7729 = hxc_l_tmp_load_result_n23 * hxc_l_tmp_load_result_n24 + hxc_l_tmp_load_result_n25 * hxc_l_dz_h9de9817f4f27;
      bool hxc_l_tmp_load_result_n27 = hxc_l_hasSelection;
      bool hxc_l_tmp_short_circuit_result_n16 = !hxc_l_tmp_load_result_n27;
      if (!!hxc_l_tmp_load_result_n27)
      {
        double hxc_l_tmp_load_result_n28 = hxc_l_distance_hc7d13aad7729;
        hxc_l_tmp_short_circuit_result_n16 = hxc_l_tmp_load_result_n28 < hxc_l_selectedDistance;
      }
      if (hxc_l_tmp_short_circuit_result_n16)
      {
        int32_t hxc_l_tmp_load_result_n31 = hxc_l_id_h56714afcdfb2;
        hxc_enum_2c0e695d_destroy(&hxc_l_selected);
        hxc_l_selected = (struct hxc_caxecraft_app_CaxecraftApp_AvailableInteractionTarget){ .hxc_tag = hxc_caxecraft_app_CaxecraftApp_AvailableInteractionTarget_DialogueInteraction, .hxc_payload.hxc_DialogueInteraction.hxc_id = hxc_l_tmp_load_result_n31 };
        hxc_l_hasSelection = true;
        hxc_l_selectedDistance = hxc_l_distance_hc7d13aad7729;
      }
    }
    hxc_array_b0c1cde1_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n11);
  }
  int32_t hxc_l_g_he9d1ba1f5d8e = 0;
  struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n34 = (*hxc_l_level).hxc_statefulObjects;
  int32_t hxc_l_tmp_array_length_result_n35;
  if (hxc_array_ref_length(hxc_l_tmp_class_field_load_result_n34, &hxc_l_tmp_array_length_result_n35) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_g1_h545ff4e7dafd = hxc_l_tmp_array_length_result_n35;
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n36 = hxc_l_g_he9d1ba1f5d8e;
    if (!(hxc_l_tmp_load_result_n36 < hxc_l_g1_h545ff4e7dafd))
    {
      break;
    }
    int32_t hxc_l_tmp_increment_load_result_n38 = hxc_l_g_he9d1ba1f5d8e;
    hxc_l_g_he9d1ba1f5d8e = hxc_i32_add_wrapping(hxc_l_tmp_increment_load_result_n38, 1);
    int32_t hxc_l_index_hc82eb94a18eb = hxc_l_tmp_increment_load_result_n38;
    struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n39 = (*hxc_l_level).hxc_statefulObjects;
    struct hxc_caxecraft_content_LoadedStatefulObjectBinding hxc_l_tmp_array_get_result_n41;
    if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n39, (size_t)hxc_l_index_hc82eb94a18eb, &hxc_l_tmp_array_get_result_n41) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_array_element_owner_n21 = hxc_l_tmp_array_get_result_n41;
    hxc_l_id_h33cf02b74d7d = hxc_l_tmp_array_element_owner_n21.hxc_authoredId;
    if (hxc_string_retain(hxc_l_id_h33cf02b74d7d) != HXC_STATUS_OK)
    {
      abort();
    }
    bool hxc_l_tmp_instance_call_result_n45 = hxc_caxecraft_domain_GameSession_statefulObjectInteractionAvailable(hxc_l_session, hxc_l_id_h33cf02b74d7d);
    if (hxc_l_tmp_instance_call_result_n45)
    {
      struct hxc_caxecraft_scenario_ScenarioTransform hxc_l_tmp_instance_call_result_n47 = hxc_caxecraft_app_PlayableLevelView_statefulObjectTransformAt(hxc_l_level, hxc_l_index_hc82eb94a18eb);
      struct hxc_caxecraft_scenario_ScenarioTransform hxc_l_transform = hxc_l_tmp_instance_call_result_n47;
      int32_t hxc_l_tmp_record_field_load_result_n48 = hxc_l_transform.hxc_xMilli;
      double hxc_l_dx_h80303f4628fd = hxc_f64_divide_zero_safe((double)hxc_l_tmp_record_field_load_result_n48, 1000.0) - hxc_l_player.hxc_body.hxc_x;
      int32_t hxc_l_tmp_record_field_load_result_n50 = hxc_l_transform.hxc_zMilli;
      double hxc_l_dz_h2f6487e3c701 = hxc_f64_divide_zero_safe((double)hxc_l_tmp_record_field_load_result_n50, 1000.0) - hxc_l_player.hxc_body.hxc_z;
      double hxc_l_tmp_load_result_n52 = hxc_l_dx_h80303f4628fd;
      double hxc_l_tmp_load_result_n53 = hxc_l_dx_h80303f4628fd;
      double hxc_l_tmp_load_result_n54 = hxc_l_dz_h2f6487e3c701;
      double hxc_l_distance_h8fb6aa6c9ec5 = hxc_l_tmp_load_result_n52 * hxc_l_tmp_load_result_n53 + hxc_l_tmp_load_result_n54 * hxc_l_dz_h2f6487e3c701;
      bool hxc_l_tmp_load_result_n56 = hxc_l_hasSelection;
      bool hxc_l_tmp_short_circuit_result_n26 = !hxc_l_tmp_load_result_n56;
      if (!!hxc_l_tmp_load_result_n56)
      {
        double hxc_l_tmp_load_result_n57 = hxc_l_distance_h8fb6aa6c9ec5;
        hxc_l_tmp_short_circuit_result_n26 = hxc_l_tmp_load_result_n57 < hxc_l_selectedDistance;
      }
      if (hxc_l_tmp_short_circuit_result_n26)
      {
        hxc_string hxc_l_tmp_enum_payload_0_owner_n27 = hxc_l_id_h33cf02b74d7d;
        if (hxc_string_retain(hxc_l_tmp_enum_payload_0_owner_n27) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_string hxc_l_tmp_enum_payload_0_owned_load_result_n61 = hxc_l_tmp_enum_payload_0_owner_n27;
        hxc_enum_2c0e695d_destroy(&hxc_l_selected);
        hxc_l_selected = (struct hxc_caxecraft_app_CaxecraftApp_AvailableInteractionTarget){ .hxc_tag = hxc_caxecraft_app_CaxecraftApp_AvailableInteractionTarget_StatefulObjectInteraction, .hxc_payload.hxc_StatefulObjectInteraction.hxc_id = hxc_l_tmp_enum_payload_0_owned_load_result_n61 };
        hxc_l_hasSelection = true;
        hxc_l_selectedDistance = hxc_l_distance_h8fb6aa6c9ec5;
      }
    }
    if (hxc_string_release(&hxc_l_id_h33cf02b74d7d) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_array_4d8613ea_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n21);
  }
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return hxc_l_selected;
}

struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation hxc_caxecraft_app_CaxecraftApp_observeActorPhase(struct hxc_array_ref *hxc_l_states, int32_t hxc_l_id, enum hxc_caxecraft_domain_ActorControllerPhase hxc_l_fallback)
{
  struct hxc_caxecraft_domain_ActorControllerState hxc_l_state = { 0 };
  struct hxc_caxecraft_domain_ActorControllerState hxc_l_tmp_array_element_owner_n5 = { 0 };
  int32_t hxc_l_g = 0;
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n0 = hxc_l_g;
    int32_t hxc_l_tmp_array_length_result_n1;
    if (hxc_array_ref_length(hxc_l_states, &hxc_l_tmp_array_length_result_n1) != HXC_STATUS_OK)
    {
      abort();
    }
    if (!(hxc_l_tmp_load_result_n0 < hxc_l_tmp_array_length_result_n1))
    {
      break;
    }
    struct hxc_caxecraft_domain_ActorControllerState hxc_l_tmp_array_get_result_n3;
    if (hxc_array_ref_get_copy(hxc_l_states, (size_t)hxc_l_g, &hxc_l_tmp_array_get_result_n3) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_array_element_owner_n5 = hxc_l_tmp_array_get_result_n3;
    hxc_l_state = hxc_l_tmp_array_element_owner_n5;
    if (hxc_record_50dff1c7_retain(&hxc_l_state) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_g = hxc_i32_add_wrapping(hxc_l_g, 1);
    if (hxc_l_state.hxc_characterId == hxc_l_id)
    {
      enum hxc_caxecraft_domain_ActorControllerPhase hxc_l_tmp_record_field_load_result_n7 = hxc_l_state.hxc_phase;
      hxc_record_50dff1c7_destroy(&hxc_l_state);
      hxc_array_3767ae6c_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n5);
      return (struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation){ .hxc_phase = hxc_l_tmp_record_field_load_result_n7, .hxc_valid = true };
    }
    hxc_record_50dff1c7_destroy(&hxc_l_state);
    hxc_array_3767ae6c_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n5);
  }
  return (struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation){ .hxc_phase = hxc_l_fallback, .hxc_valid = false };
}

enum hxc_caxecraft_app_InteractionPrompt hxc_caxecraft_app_CaxecraftApp_promptForAvailableInteraction(struct hxc_caxecraft_app_CaxecraftApp_AvailableInteractionTarget hxc_l_target)
{
  enum hxc_caxecraft_app_InteractionTargetKind hxc_l_tmp = { 0 };
  switch (hxc_l_target.hxc_tag) {
    case hxc_caxecraft_app_CaxecraftApp_AvailableInteractionTarget_NoAvailableInteraction:
      {
        hxc_l_tmp = hxc_caxecraft_app_InteractionTargetKind_NoInteractionTarget;
        break;
      }
    case hxc_caxecraft_app_CaxecraftApp_AvailableInteractionTarget_DialogueInteraction:
      {
        hxc_l_tmp = hxc_caxecraft_app_InteractionTargetKind_DialogueInteractionTarget;
        break;
      }
    case hxc_caxecraft_app_CaxecraftApp_AvailableInteractionTarget_StatefulObjectInteraction:
      {
        hxc_l_tmp = hxc_caxecraft_app_InteractionTargetKind_MechanismInteractionTarget;
        break;
      }
    default:
      {
        abort();
      }
  }
  enum hxc_caxecraft_app_InteractionPrompt hxc_l_tmp_call_result_n4 = hxc_caxecraft_app_InteractionPrompt_interactionPrompt(hxc_l_tmp);
  return hxc_l_tmp_call_result_n4;
}

struct hxc_caxecraft_domain_CharacterBody hxc_caxecraft_app_CaxecraftApp_spawnPlayer(const uint8_t *hxc_l_cells, size_t hxc_l_length, struct hxc_caxecraft_scenario_ScenarioTransform hxc_l_transform)
{
  const uint8_t *hxc_l_borrow = hxc_l_cells;
  size_t hxc_l_tmp_length_n2 = hxc_l_length;
  (void)hxc_l_borrow;
  (void)hxc_l_tmp_length_n2;
  double hxc_l_spawnX = hxc_f64_divide_zero_safe((double)hxc_l_transform.hxc_xMilli, 1000.0);
  double hxc_l_spawnY = hxc_f64_divide_zero_safe((double)hxc_l_transform.hxc_yMilli, 1000.0);
  double hxc_l_spawnZ = hxc_f64_divide_zero_safe((double)hxc_l_transform.hxc_zMilli, 1000.0);
  double hxc_l_tmp_load_result_n3 = hxc_l_spawnX;
  double hxc_l_tmp_load_result_n4 = hxc_l_spawnY;
  struct hxc_caxecraft_domain_CharacterBody hxc_l_tmp_call_result_n6 = hxc_caxecraft_domain_CharacterPhysics_body(hxc_l_tmp_load_result_n3, hxc_l_tmp_load_result_n4, hxc_l_spawnZ);
  struct hxc_caxecraft_domain_CharacterBody hxc_l_tmp_call_result_n7 = hxc_caxecraft_domain_CharacterPhysics_recoverSpawn(hxc_l_cells, hxc_l_length, hxc_l_tmp_call_result_n6);
  return hxc_l_tmp_call_result_n7;
}

void hxc_caxecraft_app_CaxecraftApp_run(struct hxc_caxecraft_app_CaxecraftApp *hxc_l_self)
{
  const void *volatile hxc_l_gc_roots[753] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 753, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_string hxc_l_currentObjectiveId = { 0 };
  struct hxc_caxecraft_app_CaxecraftApp_AvailableInteractionTarget hxc_l_tmp_static_call_argument_0_owner_n102 = { 0 };
  struct hxc_array_ref *hxc_l_initialActorPhases = { 0 };
  struct hxc_caxecraft_app_CampaignPortalPreloadResult hxc_l_g_h5245aa56229c = { 0 };
  hxc_string hxc_l_tmp_sys_println_string_argument_owner_n124 = { 0 };
  struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun hxc_l_run_hefd5b31713eb = { 0 };
  struct hxc_caxecraft_content_RuntimeLevelEditableSourceClaim hxc_l_g_h1d9290112915 = { 0 };
  struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun hxc_l_run_hfdde9b22adea = { 0 };
  struct hxc_caxecraft_content_RuntimeLevelEditableSource hxc_l_source_hf8825c3e7561 = { 0 };
  struct hxc_caxecraft_content_RuntimeLevelEditableSource hxc_l_tmp_managed_flow_owner_n17 = { 0 };
  struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun hxc_l_run_hab9bcd6c1cb0 = { 0 };
  struct hxc_caxecraft_app_CaxecraftApp_EditorTestPlayShellSnapshot hxc_l_saved = { 0 };
  struct hxc_caxecraft_editor_EditorOpenResult hxc_l_g_h6904c3060e30 = { 0 };
  hxc_string hxc_l_tmp_sys_println_string_argument_owner_n201 = { 0 };
  struct hxc_caxecraft_editor_EditorPackageOpenResult hxc_l_g_h1d08383de4f8 = { 0 };
  struct hxc_caxecraft_content_RuntimeLevelReceipt hxc_l_tmp_record_field_logicalPath_receiver_owner_n23 = { 0 };
  struct hxc_caxecraft_content_CampaignLevelLoadResult hxc_l_g_haf30e22646ec = { 0 };
  struct hxc_caxecraft_content_CampaignLevelLoadError hxc_l_error_h5f6d8c94e058 = { 0 };
  hxc_string hxc_l_tmp_string_concat_right_owner_n235 = { 0 };
  hxc_string hxc_l_tmp_sys_println_string_argument_owner_n236 = { 0 };
  struct hxc_caxecraft_editor_EditorPackageError hxc_l_error_hc49dda9de41e = { 0 };
  struct hxc_caxecraft_app_CaxecraftApp_AvailableInteractionTarget hxc_l_tmp_static_call_argument_0_owner_n250 = { 0 };
  struct hxc_array_ref *hxc_l_phases = { 0 };
  struct hxc_caxecraft_app_CampaignPortalPreloadResult hxc_l_g_hff7fb6e9172a = { 0 };
  hxc_string hxc_l_tmp_string_concat_right_owner_n26 = { 0 };
  hxc_string hxc_l_tmp_sys_println_string_argument_owner_n261 = { 0 };
  hxc_string hxc_l_tmp_string_concat_left_owner_n262 = { 0 };
  hxc_string hxc_l_tmp_string_concat_left_owner_n263 = { 0 };
  hxc_string hxc_l_tmp_string_concat_right_owner_n264 = { 0 };
  hxc_string hxc_l_tmp_sys_println_string_argument_owner_n265 = { 0 };
  hxc_string hxc_l_tmp_sys_println_string_argument_owner_n27 = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n279 = { 0 };
  hxc_string hxc_l_line_hdce933b4ff16 = { 0 };
  struct hxc_caxecraft_app_CaxecraftApp_AvailableInteractionTarget hxc_l_g_hcb81b40a6ef2 = { 0 };
  hxc_string hxc_l_id_h6856e05d5724 = { 0 };
  struct hxc_caxecraft_app_EditorScreenAction hxc_l_editorActionForFrame = { 0 };
  struct hxc_caxecraft_domain_GameTickResult hxc_l_gameTick = { 0 };
  struct hxc_optional_caxecraft_scenario_FlowTickResult hxc_l_flow = { 0 };
  struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun hxc_l_activeEditorTestPlay = { 0 };
  struct hxc_array_ref *hxc_l_g1_ha7862ac5d1a2 = { 0 };
  struct hxc_caxecraft_scenario_FlowPresentationEvent hxc_l_event_h7f4d6d6090dd = { 0 };
  struct hxc_caxecraft_scenario_FlowPresentationEvent hxc_l_tmp_array_element_owner_n322 = { 0 };
  hxc_string hxc_l_id_h1c394c562c93 = { 0 };
  hxc_string hxc_l_id_h04c30e76beca = { 0 };
  hxc_string hxc_l_exit = { 0 };
  hxc_string hxc_l_tmp_sys_println_string_argument_owner_n329 = { 0 };
  hxc_string hxc_l_tmp_sys_println_string_argument_owner_n333 = { 0 };
  hxc_string hxc_l_tmp_string_concat_left_owner_n335 = { 0 };
  hxc_string hxc_l_tmp_string_concat_left_owner_n336 = { 0 };
  hxc_string hxc_l_tmp_sys_println_string_argument_owner_n337 = { 0 };
  struct hxc_array_ref *hxc_l_g1_h170455b768c5 = { 0 };
  struct hxc_caxecraft_domain_ActorControllerEvent hxc_l_event_h2a3a461d3d07 = { 0 };
  struct hxc_caxecraft_domain_ActorControllerEvent hxc_l_tmp_array_element_owner_n361 = { 0 };
  hxc_string hxc_l_drop = { 0 };
  struct hxc_array_ref *hxc_l_actorPhases = { 0 };
  hxc_string hxc_l_pendingCampaignLabel = { 0 };
  hxc_string hxc_l_swordItemType = { 0 };
  struct hxc_caxecraft_domain_CharacterDamageCommand hxc_l_tmp_instance_call_argument_0_owner_n380 = { 0 };
  struct hxc_caxecraft_content_CampaignPackageLoadResult hxc_l_g_hccea2de18028 = { 0 };
  hxc_string hxc_l_selectedItemType = { 0 };
  hxc_string hxc_l_placementItemType = { 0 };
  struct hxc_caxecraft_app_CaxecraftApp_AvailableInteractionTarget hxc_l_tmp_static_call_argument_0_owner_n413 = { 0 };
  struct hxc_array_ref *hxc_l_presentationActorPhases = { 0 };
  struct hxc_array_ref *hxc_l_dialogueActors = { 0 };
  struct hxc_caxecraft_content_RuntimeLevelReceipt hxc_l_tmp_record_field_logicalPath_receiver_owner_n42 = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n430 = { 0 };
  hxc_string hxc_l_adventureTagline = { 0 };
  struct hxc_caxecraft_content_CampaignPackageLoadError hxc_l_gerror = { 0 };
  hxc_string hxc_l_levelLabel = { 0 };
  hxc_string hxc_l_tmp_cstring_arg_owner_owner_n452 = { 0 };
  hxc_string hxc_l_tmp_cstring_arg_owner_owner_n460 = { 0 };
  hxc_string hxc_l_tmp_cstring_arg_owner_owner_n468 = { 0 };
  struct hxc_caxecraft_content_RuntimeLevelReceipt hxc_l_tmp_record_field_logicalPath_receiver_owner_n47 = { 0 };
  hxc_string hxc_l_tmp_cstring_arg_owner_owner_n474 = { 0 };
  hxc_string hxc_l_message_h7f14c8a2a95a = { 0 };
  hxc_string hxc_l_tmp_cstring_arg_owner_owner_n478 = { 0 };
  hxc_string hxc_l_message_h1254a0b8dc08 = { 0 };
  hxc_string hxc_l_tmp_cstring_arg_owner_owner_n481 = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n487 = { 0 };
  hxc_string hxc_l_tmp = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n491 = { 0 };
  hxc_string hxc_l_tmp_static_call_argument_9_owner_n492 = { 0 };
  hxc_string hxc_l_conversationSpeaker = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n548 = { 0 };
  hxc_string hxc_l_hudView7_hea459203f814 = { 0 };
  hxc_string hxc_l_hudView8 = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n554 = { 0 };
  hxc_string hxc_l_hudView10 = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n558 = { 0 };
  hxc_string hxc_l_hudView11 = { 0 };
  struct hxc_caxecraft_content_RuntimeLevelReceipt hxc_l_receipt = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n564 = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n565 = { 0 };
  hxc_string hxc_l_tmp_sys_println_string_argument_owner_n57 = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n570 = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n571 = { 0 };
  struct hxc_caxecraft_app_HudView hxc_l_hudView_hb228802b1420 = { 0 };
  hxc_string hxc_l_tmp_string_concat_right_owner_n58 = { 0 };
  struct hxc_caxecraft_app_EditorTestPlayRequest hxc_l_tmp_instance_call_argument_0_owner_n585 = { 0 };
  struct hxc_caxecraft_app_EditorTestPlayStartResult hxc_l_tmp_enum_switch_subject_owner_n586 = { 0 };
  hxc_string hxc_l_tmp_sys_println_string_argument_owner_n59 = { 0 };
  struct hxc_array_ref *hxc_l_testPhases = { 0 };
  struct hxc_caxecraft_app_CaxecraftApp_EditorTestPlayShellSnapshot hxc_l_snapshot = { 0 };
  hxc_string hxc_l_tmp_string_concat_right_owner_n60 = { 0 };
  struct hxc_caxecraft_app_CaxecraftApp_AvailableInteractionTarget hxc_l_tmp_static_call_argument_0_owner_n607 = { 0 };
  struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun hxc_l_unfinishedEditorTestPlay = { 0 };
  hxc_string hxc_l_tmp_sys_println_string_argument_owner_n61 = { 0 };
  struct hxc_caxecraft_content_RuntimeContentLoadResult hxc_l_g_h14daa0fd57c5 = { 0 };
  hxc_string hxc_l_activeDialogue = { 0 };
  hxc_string hxc_l_latestJournalId = { 0 };
  int32_t hxc_l_initialHealth = 6;
  hxc_l_gc_roots[0] = (const void *)NULL;
  struct hxc_caxecraft_content_ContentPackageStore *hxc_l_contentStore = NULL;
  struct hxc_caxecraft_content_ContentPackageOpenResult hxc_l_tmp_call_result_n0 = hxc_caxecraft_content_ContentPackageStore_open((hxc_string){ (const uint8_t *)"content", 7, true, NULL }, (hxc_string){ (const uint8_t *)"staged-content", 14, true, NULL }, 16777216);
  hxc_l_gc_roots[1] = hxc_l_tmp_call_result_n0.hxc_tag == hxc_caxecraft_content_ContentPackageOpenResult_PackageStoreOpened ? (const void *)hxc_l_tmp_call_result_n0.hxc_payload.hxc_PackageStoreOpened.hxc_store : NULL;
  struct hxc_caxecraft_content_ContentPackageOpenResult hxc_l_g_h0b6eadae1b17 = hxc_l_tmp_call_result_n0;
  hxc_l_gc_roots[2] = hxc_l_g_h0b6eadae1b17.hxc_tag == hxc_caxecraft_content_ContentPackageOpenResult_PackageStoreOpened ? (const void *)hxc_l_g_h0b6eadae1b17.hxc_payload.hxc_PackageStoreOpened.hxc_store : NULL;
  switch (hxc_l_g_h0b6eadae1b17.hxc_tag) {
    case hxc_caxecraft_content_ContentPackageOpenResult_PackageStoreOpened:
      {
        break;
      }
    case hxc_caxecraft_content_ContentPackageOpenResult_PackageStoreRejected:
      {
        if (hxc_io_println((hxc_string){ (const uint8_t *)"caxecraft: runtime content root rejected", 40, true, NULL }) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
        {
          abort();
        }
        return;
      }
    default:
      {
        abort();
      }
  }
  hxc_l_gc_roots[3] = hxc_l_g_h0b6eadae1b17.hxc_tag == hxc_caxecraft_content_ContentPackageOpenResult_PackageStoreOpened ? (const void *)hxc_l_g_h0b6eadae1b17.hxc_payload.hxc_PackageStoreOpened.hxc_store : NULL;
  if (hxc_l_g_h0b6eadae1b17.hxc_tag != hxc_caxecraft_content_ContentPackageOpenResult_PackageStoreOpened)
  {
    abort();
  }
  struct hxc_caxecraft_content_ContentPackageStore *hxc_l_tmp_enum_payload_project_n3 = hxc_l_g_h0b6eadae1b17.hxc_payload.hxc_PackageStoreOpened.hxc_store;
  hxc_l_gc_roots[4] = (const void *)hxc_l_tmp_enum_payload_project_n3;
  struct hxc_caxecraft_content_ContentPackageStore *hxc_l_store = hxc_l_tmp_enum_payload_project_n3;
  hxc_l_gc_roots[5] = (const void *)hxc_l_store;
  hxc_l_contentStore = hxc_l_store;
  struct hxc_caxecraft_content_ContentGenerationSequence *hxc_l_tmp_managed_class_result_n5 = NULL;
  if (hxc_gc_allocate(&hxc_program_gc, &hxc_caxecraft_content_ContentGenerationSequence_descriptor, (void **)&hxc_l_tmp_managed_class_result_n5) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_gc_roots[6] = (const void *)hxc_l_tmp_managed_class_result_n5;
  hxc_compiler_constructor_caxecraft_content_ContentGenerationSequence(hxc_l_tmp_managed_class_result_n5);
  hxc_l_gc_roots[7] = (const void *)hxc_l_tmp_managed_class_result_n5;
  struct hxc_caxecraft_content_ContentGenerationSequence *hxc_l_generationSequence = hxc_l_tmp_managed_class_result_n5;
  hxc_l_gc_roots[8] = (const void *)NULL;
  struct hxc_caxecraft_content_RuntimeContentGeneration *hxc_l_completeCandidate = NULL;
  struct hxc_caxecraft_content_ContentPackageStore *hxc_l_tmp_load_result_n6 = hxc_l_contentStore;
  hxc_l_gc_roots[9] = (const void *)hxc_l_tmp_load_result_n6;
  struct hxc_caxecraft_content_ContentGenerationSequence *hxc_l_tmp_load_result_n7 = hxc_l_generationSequence;
  hxc_l_gc_roots[10] = (const void *)hxc_l_tmp_load_result_n7;
  if (hxc_l_tmp_load_result_n7 == NULL)
  {
    abort();
  }
  int32_t hxc_l_tmp_instance_call_result_n8 = hxc_caxecraft_content_ContentGenerationSequence_allocate(hxc_l_tmp_load_result_n7);
  struct hxc_caxecraft_content_RuntimeContentLoadResult hxc_l_tmp_call_result_n11 = hxc_caxecraft_content_RuntimeContentGeneration_loadRuntimeContent(hxc_l_tmp_load_result_n6, hxc_l_tmp_instance_call_result_n8, (struct hxc_caxecraft_content_RuntimeContentPlayerOptions){ .hxc_entityId = 1, .hxc_initialHealth = hxc_l_initialHealth });
  hxc_l_gc_roots[11] = hxc_l_tmp_call_result_n11.hxc_tag == hxc_caxecraft_content_RuntimeContentLoadResult_RuntimeContentReady ? (const void *)hxc_l_tmp_call_result_n11.hxc_payload.hxc_RuntimeContentReady.hxc_candidate : NULL;
  hxc_l_g_h14daa0fd57c5 = hxc_l_tmp_call_result_n11;
  hxc_l_gc_roots[12] = hxc_l_g_h14daa0fd57c5.hxc_tag == hxc_caxecraft_content_RuntimeContentLoadResult_RuntimeContentReady ? (const void *)hxc_l_g_h14daa0fd57c5.hxc_payload.hxc_RuntimeContentReady.hxc_candidate : NULL;
  switch (hxc_l_g_h14daa0fd57c5.hxc_tag) {
    case hxc_caxecraft_content_RuntimeContentLoadResult_RuntimeContentReady:
      {
        break;
      }
    case hxc_caxecraft_content_RuntimeContentLoadResult_RuntimeContentRejected:
      {
        if (hxc_io_println((hxc_string){ (const uint8_t *)"caxecraft: runtime content rejected", 35, true, NULL }) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_enum_fcb4401b_destroy(&hxc_l_g_h14daa0fd57c5);
        if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
        {
          abort();
        }
        return;
      }
    default:
      {
        abort();
      }
  }
  hxc_l_gc_roots[13] = hxc_l_g_h14daa0fd57c5.hxc_tag == hxc_caxecraft_content_RuntimeContentLoadResult_RuntimeContentReady ? (const void *)hxc_l_g_h14daa0fd57c5.hxc_payload.hxc_RuntimeContentReady.hxc_candidate : NULL;
  if (hxc_l_g_h14daa0fd57c5.hxc_tag != hxc_caxecraft_content_RuntimeContentLoadResult_RuntimeContentReady)
  {
    abort();
  }
  struct hxc_caxecraft_content_RuntimeContentGeneration *hxc_l_tmp_enum_payload_project_n14 = hxc_l_g_h14daa0fd57c5.hxc_payload.hxc_RuntimeContentReady.hxc_candidate;
  hxc_l_gc_roots[14] = (const void *)hxc_l_tmp_enum_payload_project_n14;
  struct hxc_caxecraft_content_RuntimeContentGeneration *hxc_l_candidate_haa37e6ad944f = hxc_l_tmp_enum_payload_project_n14;
  hxc_l_gc_roots[15] = (const void *)hxc_l_candidate_haa37e6ad944f;
  hxc_l_completeCandidate = hxc_l_candidate_haa37e6ad944f;
  struct hxc_caxecraft_content_RuntimeContentGeneration *hxc_l_tmp_load_result_n16 = hxc_l_completeCandidate;
  hxc_l_gc_roots[16] = (const void *)hxc_l_tmp_load_result_n16;
  struct hxc_caxecraft_content_ActiveRuntimeContent *hxc_l_tmp_managed_class_result_n17 = NULL;
  if (hxc_gc_allocate(&hxc_program_gc, &hxc_caxecraft_content_ActiveRuntimeContent_descriptor, (void **)&hxc_l_tmp_managed_class_result_n17) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_gc_roots[17] = (const void *)hxc_l_tmp_managed_class_result_n17;
  hxc_compiler_constructor_caxecraft_content_ActiveRuntimeContent(hxc_l_tmp_managed_class_result_n17, hxc_l_tmp_load_result_n16);
  hxc_l_gc_roots[18] = (const void *)hxc_l_tmp_managed_class_result_n17;
  struct hxc_caxecraft_content_ActiveRuntimeContent *hxc_l_activeRuntimeContent = hxc_l_tmp_managed_class_result_n17;
  struct hxc_caxecraft_content_ActiveRuntimeContent *hxc_l_tmp_load_result_n18 = hxc_l_activeRuntimeContent;
  hxc_l_gc_roots[19] = (const void *)hxc_l_tmp_load_result_n18;
  if (hxc_l_tmp_load_result_n18 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_content_RuntimeContentGeneration *hxc_l_tmp_class_field_load_result_n19 = (*hxc_l_tmp_load_result_n18).hxc_active;
  hxc_l_gc_roots[20] = (const void *)hxc_l_tmp_class_field_load_result_n19;
  struct hxc_caxecraft_content_RuntimeContentGeneration *hxc_l_runtimeContent = hxc_l_tmp_class_field_load_result_n19;
  struct hxc_caxecraft_content_RuntimeContentGeneration *hxc_l_tmp_load_result_n20 = hxc_l_runtimeContent;
  hxc_l_gc_roots[21] = (const void *)hxc_l_tmp_load_result_n20;
  if (hxc_l_tmp_load_result_n20 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_tmp_class_field_load_result_n21 = (*hxc_l_tmp_load_result_n20).hxc_contentRegistry;
  hxc_l_gc_roots[22] = (const void *)hxc_l_tmp_class_field_load_result_n21;
  struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_contentRegistry = hxc_l_tmp_class_field_load_result_n21;
  struct hxc_caxecraft_content_RuntimeContentGeneration *hxc_l_tmp_load_result_n22 = hxc_l_runtimeContent;
  hxc_l_gc_roots[23] = (const void *)hxc_l_tmp_load_result_n22;
  if (hxc_l_tmp_load_result_n22 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_tmp_class_field_load_result_n23 = (*hxc_l_tmp_load_result_n22).hxc_uiCatalog;
  hxc_l_gc_roots[24] = (const void *)hxc_l_tmp_class_field_load_result_n23;
  struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_uiCatalog = hxc_l_tmp_class_field_load_result_n23;
  struct hxc_caxecraft_content_RuntimeContentGeneration *hxc_l_tmp_load_result_n24 = hxc_l_runtimeContent;
  hxc_l_gc_roots[25] = (const void *)hxc_l_tmp_load_result_n24;
  if (hxc_l_tmp_load_result_n24 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_content_RuntimeLevelCandidate *hxc_l_tmp_class_field_load_result_n25 = (*hxc_l_tmp_load_result_n24).hxc_loadedLevel;
  hxc_l_gc_roots[26] = (const void *)hxc_l_tmp_class_field_load_result_n25;
  struct hxc_caxecraft_content_RuntimeLevelCandidate *hxc_l_loadedCandidate = hxc_l_tmp_class_field_load_result_n25;
  struct hxc_caxecraft_content_RuntimeLevelEditableSource hxc_l_editorSource = { 0 };
  struct hxc_caxecraft_content_RuntimeLevelCandidate *hxc_l_tmp_load_result_n26 = hxc_l_loadedCandidate;
  hxc_l_gc_roots[27] = (const void *)hxc_l_tmp_load_result_n26;
  if (hxc_l_tmp_load_result_n26 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_content_RuntimeLevelEditableSourceClaim hxc_l_tmp_instance_call_result_n27 = hxc_caxecraft_content_RuntimeLevelCandidate_claimEditableSource(hxc_l_tmp_load_result_n26);
  hxc_l_gc_roots[28] = hxc_l_tmp_instance_call_result_n27.hxc_tag == hxc_caxecraft_content_RuntimeLevelEditableSourceClaim_EditableSourceClaimed ? (const void *)hxc_l_tmp_instance_call_result_n27.hxc_payload.hxc_EditableSourceClaimed.hxc_source.hxc_parsed.hxc_candidate.hxc_flow.hxc_rules : NULL;
  hxc_l_gc_roots[29] = hxc_l_tmp_instance_call_result_n27.hxc_tag == hxc_caxecraft_content_RuntimeLevelEditableSourceClaim_EditableSourceClaimed ? (const void *)hxc_l_tmp_instance_call_result_n27.hxc_payload.hxc_EditableSourceClaimed.hxc_source.hxc_parsed.hxc_candidate.hxc_flow.hxc_sequences : NULL;
  hxc_l_g_h1d9290112915 = hxc_l_tmp_instance_call_result_n27;
  hxc_l_gc_roots[30] = hxc_l_g_h1d9290112915.hxc_tag == hxc_caxecraft_content_RuntimeLevelEditableSourceClaim_EditableSourceClaimed ? (const void *)hxc_l_g_h1d9290112915.hxc_payload.hxc_EditableSourceClaimed.hxc_source.hxc_parsed.hxc_candidate.hxc_flow.hxc_rules : NULL;
  hxc_l_gc_roots[31] = hxc_l_g_h1d9290112915.hxc_tag == hxc_caxecraft_content_RuntimeLevelEditableSourceClaim_EditableSourceClaimed ? (const void *)hxc_l_g_h1d9290112915.hxc_payload.hxc_EditableSourceClaimed.hxc_source.hxc_parsed.hxc_candidate.hxc_flow.hxc_sequences : NULL;
  switch (hxc_l_g_h1d9290112915.hxc_tag) {
    case hxc_caxecraft_content_RuntimeLevelEditableSourceClaim_EditableSourceClaimed:
      {
        break;
      }
    case hxc_caxecraft_content_RuntimeLevelEditableSourceClaim_EditableSourceAlreadyClaimed:
      {
        if (hxc_io_println((hxc_string){ (const uint8_t *)"caxecraft: active level editor source was already claimed", 57, true, NULL }) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_enum_d4806d01_destroy(&hxc_l_g_h1d9290112915);
        hxc_enum_fcb4401b_destroy(&hxc_l_g_h14daa0fd57c5);
        if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
        {
          abort();
        }
        return;
      }
    default:
      {
        abort();
      }
  }
  hxc_l_gc_roots[32] = hxc_l_g_h1d9290112915.hxc_tag == hxc_caxecraft_content_RuntimeLevelEditableSourceClaim_EditableSourceClaimed ? (const void *)hxc_l_g_h1d9290112915.hxc_payload.hxc_EditableSourceClaimed.hxc_source.hxc_parsed.hxc_candidate.hxc_flow.hxc_rules : NULL;
  hxc_l_gc_roots[33] = hxc_l_g_h1d9290112915.hxc_tag == hxc_caxecraft_content_RuntimeLevelEditableSourceClaim_EditableSourceClaimed ? (const void *)hxc_l_g_h1d9290112915.hxc_payload.hxc_EditableSourceClaimed.hxc_source.hxc_parsed.hxc_candidate.hxc_flow.hxc_sequences : NULL;
  if (hxc_l_g_h1d9290112915.hxc_tag != hxc_caxecraft_content_RuntimeLevelEditableSourceClaim_EditableSourceClaimed)
  {
    abort();
  }
  struct hxc_caxecraft_content_RuntimeLevelEditableSource hxc_l_tmp_enum_payload_project_n30 = hxc_l_g_h1d9290112915.hxc_payload.hxc_EditableSourceClaimed.hxc_source;
  hxc_l_gc_roots[34] = (const void *)hxc_l_tmp_enum_payload_project_n30.hxc_parsed.hxc_candidate.hxc_flow.hxc_rules;
  hxc_l_gc_roots[35] = (const void *)hxc_l_tmp_enum_payload_project_n30.hxc_parsed.hxc_candidate.hxc_flow.hxc_sequences;
  hxc_l_source_hf8825c3e7561 = hxc_l_tmp_enum_payload_project_n30;
  if (hxc_record_8aab5967_retain(&hxc_l_source_hf8825c3e7561) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_gc_roots[36] = (const void *)hxc_l_source_hf8825c3e7561.hxc_parsed.hxc_candidate.hxc_flow.hxc_rules;
  hxc_l_gc_roots[37] = (const void *)hxc_l_source_hf8825c3e7561.hxc_parsed.hxc_candidate.hxc_flow.hxc_sequences;
  hxc_l_editorSource = hxc_l_source_hf8825c3e7561;
  if (hxc_record_8aab5967_retain(&hxc_l_editorSource) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_record_8aab5967_destroy(&hxc_l_source_hf8825c3e7561);
  struct hxc_caxecraft_content_RuntimeLevelEditableSource hxc_l_tmp_managed_flow_carrier_move_result_n32 = hxc_l_editorSource;
  hxc_l_gc_roots[38] = (const void *)hxc_l_tmp_managed_flow_carrier_move_result_n32.hxc_parsed.hxc_candidate.hxc_flow.hxc_rules;
  hxc_l_gc_roots[39] = (const void *)hxc_l_tmp_managed_flow_carrier_move_result_n32.hxc_parsed.hxc_candidate.hxc_flow.hxc_sequences;
  hxc_l_tmp_managed_flow_owner_n17 = hxc_l_tmp_managed_flow_carrier_move_result_n32;
  hxc_l_gc_roots[40] = (const void *)NULL;
  struct hxc_caxecraft_editor_EditorSession *hxc_l_editorSession = NULL;
  struct hxc_bytes_ref *hxc_l_tmp_record_field_load_result_n33 = hxc_l_tmp_managed_flow_owner_n17.hxc_bytes;
  struct hxc_caxecraft_scenario_ParsedScenario hxc_l_tmp_record_field_load_result_n34 = hxc_l_tmp_managed_flow_owner_n17.hxc_parsed;
  hxc_l_gc_roots[41] = (const void *)hxc_l_tmp_record_field_load_result_n34.hxc_candidate.hxc_flow.hxc_rules;
  hxc_l_gc_roots[42] = (const void *)hxc_l_tmp_record_field_load_result_n34.hxc_candidate.hxc_flow.hxc_sequences;
  struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_tmp_load_result_n35 = hxc_l_contentRegistry;
  hxc_l_gc_roots[43] = (const void *)hxc_l_tmp_load_result_n35;
  if (hxc_l_tmp_load_result_n35 == NULL)
  {
    abort();
  }
  struct hxc_compiler_interface_dispatch_caxecraft_scenario_ScenarioContentRegistry_value hxc_l_tmp_interface_value_n36 = (struct hxc_compiler_interface_dispatch_caxecraft_scenario_ScenarioContentRegistry_value){ .object = hxc_l_tmp_load_result_n35, .table = &hxc_itable_compiler_interface_dispatch_caxecraft_content_RuntimeContentRegistry_itable_layout_caxecraf_hb3f384641e1a61bf };
  struct hxc_caxecraft_editor_EditorOpenResult hxc_l_tmp_call_result_n37 = hxc_caxecraft_editor_EditorSession_openParsed(hxc_l_tmp_record_field_load_result_n33, hxc_l_tmp_record_field_load_result_n34, hxc_l_tmp_interface_value_n36, (struct hxc_optional_caxecraft_editor_EditorSettings){ .hxc_has_value = false });
  hxc_l_gc_roots[44] = hxc_l_tmp_call_result_n37.hxc_tag == hxc_caxecraft_editor_EditorOpenResult_EditorOpened ? (const void *)hxc_l_tmp_call_result_n37.hxc_payload.hxc_EditorOpened.hxc_session : NULL;
  hxc_l_g_h6904c3060e30 = hxc_l_tmp_call_result_n37;
  hxc_l_gc_roots[45] = hxc_l_g_h6904c3060e30.hxc_tag == hxc_caxecraft_editor_EditorOpenResult_EditorOpened ? (const void *)hxc_l_g_h6904c3060e30.hxc_payload.hxc_EditorOpened.hxc_session : NULL;
  switch (hxc_l_g_h6904c3060e30.hxc_tag) {
    case hxc_caxecraft_editor_EditorOpenResult_EditorOpened:
      {
        break;
      }
    case hxc_caxecraft_editor_EditorOpenResult_EditorOpenRejected:
      {
        if (hxc_io_println((hxc_string){ (const uint8_t *)"caxecraft: active level rejected by editor", 42, true, NULL }) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_enum_777c0102_destroy(&hxc_l_g_h6904c3060e30);
        hxc_record_8aab5967_destroy(&hxc_l_tmp_managed_flow_owner_n17);
        hxc_enum_d4806d01_destroy(&hxc_l_g_h1d9290112915);
        hxc_enum_fcb4401b_destroy(&hxc_l_g_h14daa0fd57c5);
        if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
        {
          abort();
        }
        return;
      }
    default:
      {
        abort();
      }
  }
  hxc_l_gc_roots[46] = hxc_l_g_h6904c3060e30.hxc_tag == hxc_caxecraft_editor_EditorOpenResult_EditorOpened ? (const void *)hxc_l_g_h6904c3060e30.hxc_payload.hxc_EditorOpened.hxc_session : NULL;
  if (hxc_l_g_h6904c3060e30.hxc_tag != hxc_caxecraft_editor_EditorOpenResult_EditorOpened)
  {
    abort();
  }
  struct hxc_caxecraft_editor_EditorSession *hxc_l_tmp_enum_payload_project_n40 = hxc_l_g_h6904c3060e30.hxc_payload.hxc_EditorOpened.hxc_session;
  hxc_l_gc_roots[47] = (const void *)hxc_l_tmp_enum_payload_project_n40;
  struct hxc_caxecraft_editor_EditorSession *hxc_l_value_hf3a8ec549210 = hxc_l_tmp_enum_payload_project_n40;
  hxc_l_gc_roots[48] = (const void *)hxc_l_value_hf3a8ec549210;
  hxc_l_editorSession = hxc_l_value_hf3a8ec549210;
  hxc_l_gc_roots[49] = (const void *)NULL;
  struct hxc_caxecraft_editor_EditorPackageSession *hxc_l_editorPackage = NULL;
  struct hxc_caxecraft_content_RuntimeLevelCandidate *hxc_l_tmp_load_result_n42 = hxc_l_loadedCandidate;
  hxc_l_gc_roots[50] = (const void *)hxc_l_tmp_load_result_n42;
  if (hxc_l_tmp_load_result_n42 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_content_RuntimeLevelReceipt hxc_l_tmp_instance_call_result_n43 = hxc_caxecraft_content_RuntimeLevelCandidate_receipt(hxc_l_tmp_load_result_n42);
  hxc_l_tmp_record_field_logicalPath_receiver_owner_n23 = hxc_l_tmp_instance_call_result_n43;
  struct hxc_caxecraft_content_RuntimeLevelReceipt hxc_l_tmp_record_field_logicalPath_receiver_borrow_result_n44 = hxc_l_tmp_record_field_logicalPath_receiver_owner_n23;
  hxc_l_gc_roots[51] = (const void *)hxc_l_editorSession;
  struct hxc_caxecraft_editor_EditorPackageOpenResult hxc_l_tmp_call_result_n47 = hxc_caxecraft_editor_EditorPackageSession_attach((hxc_string){ (const uint8_t *)"content", 7, true, NULL }, (hxc_string){ (const uint8_t *)".", 1, true, NULL }, (hxc_string){ (const uint8_t *)"caxecraft.package.json", 22, true, NULL }, hxc_l_tmp_record_field_logicalPath_receiver_borrow_result_n44.hxc_logicalPath, hxc_l_editorSession);
  hxc_l_gc_roots[52] = hxc_l_tmp_call_result_n47.hxc_tag == hxc_caxecraft_editor_EditorPackageOpenResult_EditorPackageOpened ? (const void *)hxc_l_tmp_call_result_n47.hxc_payload.hxc_EditorPackageOpened.hxc_value : NULL;
  hxc_l_g_h1d08383de4f8 = hxc_l_tmp_call_result_n47;
  hxc_l_gc_roots[53] = hxc_l_g_h1d08383de4f8.hxc_tag == hxc_caxecraft_editor_EditorPackageOpenResult_EditorPackageOpened ? (const void *)hxc_l_g_h1d08383de4f8.hxc_payload.hxc_EditorPackageOpened.hxc_value : NULL;
  switch (hxc_l_g_h1d08383de4f8.hxc_tag) {
    case hxc_caxecraft_editor_EditorPackageOpenResult_EditorPackageOpened:
      {
        break;
      }
    case hxc_caxecraft_editor_EditorPackageOpenResult_EditorPackageOpenRejected:
      {
        hxc_l_gc_roots[57] = hxc_l_g_h1d08383de4f8.hxc_tag == hxc_caxecraft_editor_EditorPackageOpenResult_EditorPackageOpened ? (const void *)hxc_l_g_h1d08383de4f8.hxc_payload.hxc_EditorPackageOpened.hxc_value : NULL;
        if (hxc_l_g_h1d08383de4f8.hxc_tag != hxc_caxecraft_editor_EditorPackageOpenResult_EditorPackageOpenRejected)
        {
          abort();
        }
        struct hxc_caxecraft_editor_EditorPackageError hxc_l_tmp_enum_payload_project_n53 = hxc_l_g_h1d08383de4f8.hxc_payload.hxc_EditorPackageOpenRejected.hxc_error;
        hxc_l_error_hc49dda9de41e = hxc_l_tmp_enum_payload_project_n53;
        if (hxc_enum_0c746018_retain(&hxc_l_error_hc49dda9de41e) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_string hxc_l_tmp_call_result_n55 = hxc_caxecraft_editor_EditorPackageSession_editorPackageErrorMessage(hxc_l_error_hc49dda9de41e);
        hxc_l_tmp_string_concat_right_owner_n26 = hxc_l_tmp_call_result_n55;
        hxc_string hxc_l_tmp_string_concat_result_n57 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
        if (hxc_string_concat_ref((hxc_string){ (const uint8_t *)"caxecraft: editor package rejected: ", 36, true, NULL }, hxc_l_tmp_string_concat_right_owner_n26, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n57) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_l_tmp_sys_println_string_argument_owner_n27 = hxc_l_tmp_string_concat_result_n57;
        if (hxc_io_println(hxc_l_tmp_sys_println_string_argument_owner_n27) != HXC_STATUS_OK)
        {
          if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n27) != HXC_STATUS_OK)
          {
            abort();
          }
          if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n26) != HXC_STATUS_OK)
          {
            abort();
          }
          hxc_enum_0c746018_destroy(&hxc_l_error_hc49dda9de41e);
          hxc_enum_11853fd1_destroy(&hxc_l_g_h1d08383de4f8);
          hxc_record_f6127605_destroy(&hxc_l_tmp_record_field_logicalPath_receiver_owner_n23);
          hxc_enum_777c0102_destroy(&hxc_l_g_h6904c3060e30);
          hxc_record_8aab5967_destroy(&hxc_l_tmp_managed_flow_owner_n17);
          hxc_enum_d4806d01_destroy(&hxc_l_g_h1d9290112915);
          hxc_enum_fcb4401b_destroy(&hxc_l_g_h14daa0fd57c5);
          if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
          {
            abort();
          }
          abort();
        }
        if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n27) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n26) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_enum_0c746018_destroy(&hxc_l_error_hc49dda9de41e);
        hxc_enum_11853fd1_destroy(&hxc_l_g_h1d08383de4f8);
        hxc_record_f6127605_destroy(&hxc_l_tmp_record_field_logicalPath_receiver_owner_n23);
        hxc_enum_777c0102_destroy(&hxc_l_g_h6904c3060e30);
        hxc_record_8aab5967_destroy(&hxc_l_tmp_managed_flow_owner_n17);
        hxc_enum_d4806d01_destroy(&hxc_l_g_h1d9290112915);
        hxc_enum_fcb4401b_destroy(&hxc_l_g_h14daa0fd57c5);
        if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
        {
          abort();
        }
        return;
      }
    default:
      {
        abort();
      }
  }
  hxc_l_gc_roots[54] = hxc_l_g_h1d08383de4f8.hxc_tag == hxc_caxecraft_editor_EditorPackageOpenResult_EditorPackageOpened ? (const void *)hxc_l_g_h1d08383de4f8.hxc_payload.hxc_EditorPackageOpened.hxc_value : NULL;
  if (hxc_l_g_h1d08383de4f8.hxc_tag != hxc_caxecraft_editor_EditorPackageOpenResult_EditorPackageOpened)
  {
    abort();
  }
  struct hxc_caxecraft_editor_EditorPackageSession *hxc_l_tmp_enum_payload_project_n50 = hxc_l_g_h1d08383de4f8.hxc_payload.hxc_EditorPackageOpened.hxc_value;
  hxc_l_gc_roots[55] = (const void *)hxc_l_tmp_enum_payload_project_n50;
  struct hxc_caxecraft_editor_EditorPackageSession *hxc_l_value_h6771cfe5e0ae = hxc_l_tmp_enum_payload_project_n50;
  hxc_l_gc_roots[56] = (const void *)hxc_l_value_h6771cfe5e0ae;
  hxc_l_editorPackage = hxc_l_value_h6771cfe5e0ae;
  struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_tmp_load_result_n59 = hxc_l_contentRegistry;
  hxc_l_gc_roots[58] = (const void *)hxc_l_tmp_load_result_n59;
  struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_tmp_load_result_n60 = hxc_l_uiCatalog;
  hxc_l_gc_roots[59] = (const void *)hxc_l_tmp_load_result_n60;
  struct hxc_caxecraft_editor_EditorPackageSession *hxc_l_tmp_load_result_n61 = hxc_l_editorPackage;
  hxc_l_gc_roots[60] = (const void *)hxc_l_tmp_load_result_n61;
  struct hxc_caxecraft_app_CaxecraftEditorScreen *hxc_l_tmp_managed_class_result_n62 = NULL;
  if (hxc_gc_allocate(&hxc_program_gc, &hxc_caxecraft_app_CaxecraftEditorScreen_descriptor, (void **)&hxc_l_tmp_managed_class_result_n62) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_gc_roots[61] = (const void *)hxc_l_tmp_managed_class_result_n62;
  hxc_compiler_constructor_caxecraft_app_CaxecraftEditorScreen(hxc_l_tmp_managed_class_result_n62, hxc_l_tmp_load_result_n59, hxc_l_tmp_load_result_n60, hxc_l_tmp_load_result_n61);
  hxc_l_gc_roots[62] = (const void *)hxc_l_tmp_managed_class_result_n62;
  struct hxc_caxecraft_app_CaxecraftEditorScreen *hxc_l_editorScreen = hxc_l_tmp_managed_class_result_n62;
  hxc_l_gc_roots[63] = (const void *)NULL;
  struct hxc_caxecraft_app_ActivePlayableLevel *hxc_l_activeLevel = NULL;
  hxc_l_gc_roots[64] = (const void *)hxc_l_loadedCandidate;
  struct hxc_caxecraft_app_PlayableLevelCreationResult hxc_l_tmp_call_result_n64 = hxc_caxecraft_app_ActivePlayableLevel_create(hxc_l_loadedCandidate);
  hxc_l_gc_roots[65] = hxc_l_tmp_call_result_n64.hxc_tag == hxc_caxecraft_app_PlayableLevelCreationResult_PlayableLevelCreated ? (const void *)hxc_l_tmp_call_result_n64.hxc_payload.hxc_PlayableLevelCreated.hxc_owner : NULL;
  struct hxc_caxecraft_app_PlayableLevelCreationResult hxc_l_g_hb9b52a9fca9f = hxc_l_tmp_call_result_n64;
  hxc_l_gc_roots[66] = hxc_l_g_hb9b52a9fca9f.hxc_tag == hxc_caxecraft_app_PlayableLevelCreationResult_PlayableLevelCreated ? (const void *)hxc_l_g_hb9b52a9fca9f.hxc_payload.hxc_PlayableLevelCreated.hxc_owner : NULL;
  switch (hxc_l_g_hb9b52a9fca9f.hxc_tag) {
    case hxc_caxecraft_app_PlayableLevelCreationResult_PlayableLevelCreated:
      {
        break;
      }
    case hxc_caxecraft_app_PlayableLevelCreationResult_PlayableLevelCreationRejected:
      {
        if (hxc_io_println((hxc_string){ (const uint8_t *)"caxecraft: initial level lacks required playable bindings", 57, true, NULL }) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_enum_11853fd1_destroy(&hxc_l_g_h1d08383de4f8);
        hxc_record_f6127605_destroy(&hxc_l_tmp_record_field_logicalPath_receiver_owner_n23);
        hxc_enum_777c0102_destroy(&hxc_l_g_h6904c3060e30);
        hxc_record_8aab5967_destroy(&hxc_l_tmp_managed_flow_owner_n17);
        hxc_enum_d4806d01_destroy(&hxc_l_g_h1d9290112915);
        hxc_enum_fcb4401b_destroy(&hxc_l_g_h14daa0fd57c5);
        if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
        {
          abort();
        }
        return;
      }
    default:
      {
        abort();
      }
  }
  hxc_l_gc_roots[67] = hxc_l_g_hb9b52a9fca9f.hxc_tag == hxc_caxecraft_app_PlayableLevelCreationResult_PlayableLevelCreated ? (const void *)hxc_l_g_hb9b52a9fca9f.hxc_payload.hxc_PlayableLevelCreated.hxc_owner : NULL;
  if (hxc_l_g_hb9b52a9fca9f.hxc_tag != hxc_caxecraft_app_PlayableLevelCreationResult_PlayableLevelCreated)
  {
    abort();
  }
  struct hxc_caxecraft_app_ActivePlayableLevel *hxc_l_tmp_enum_payload_project_n67 = hxc_l_g_hb9b52a9fca9f.hxc_payload.hxc_PlayableLevelCreated.hxc_owner;
  hxc_l_gc_roots[68] = (const void *)hxc_l_tmp_enum_payload_project_n67;
  struct hxc_caxecraft_app_ActivePlayableLevel *hxc_l_value_hcd690b943e15 = hxc_l_tmp_enum_payload_project_n67;
  hxc_l_gc_roots[69] = (const void *)hxc_l_value_hcd690b943e15;
  hxc_l_activeLevel = hxc_l_value_hcd690b943e15;
  hxc_l_gc_roots[70] = (struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = false }.hxc_has_value ? (const void *)(struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = false }.hxc_value.hxc_runtime : NULL;
  hxc_l_gc_roots[71] = (struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = false }.hxc_has_value ? (const void *)(struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = false }.hxc_value.hxc_snapshot.hxc_campaignLevel : NULL;
  hxc_l_gc_roots[72] = (struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = false }.hxc_has_value ? (const void *)(struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = false }.hxc_value.hxc_snapshot.hxc_pendingCampaignLevel : NULL;
  hxc_l_gc_roots[73] = (struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = false }.hxc_has_value ? (const void *)(struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = false }.hxc_value.hxc_snapshot.hxc_pendingCampaignPortal : NULL;
  hxc_l_gc_roots[74] = (struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = false }.hxc_has_value ? (const void *)(struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = false }.hxc_value.hxc_snapshot.hxc_preparedPortal : NULL;
  hxc_l_activeEditorTestPlay = (struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = false };
  hxc_l_gc_roots[75] = (const void *)NULL;
  struct hxc_caxecraft_content_CampaignManifest *hxc_l_campaign = NULL;
  hxc_l_gc_roots[76] = (const void *)NULL;
  struct hxc_caxecraft_content_CampaignLevel *hxc_l_campaignLevel = NULL;
  hxc_l_gc_roots[77] = (const void *)NULL;
  struct hxc_caxecraft_content_CampaignLevel *hxc_l_pendingCampaignLevel = NULL;
  hxc_l_gc_roots[78] = (const void *)NULL;
  struct hxc_caxecraft_app_PreparedCampaignPortal *hxc_l_pendingCampaignPortal = NULL;
  hxc_l_pendingCampaignLabel = (hxc_string){ (const uint8_t *)"", 0, true, NULL };
  if (hxc_string_retain(hxc_l_pendingCampaignLabel) != HXC_STATUS_OK)
  {
    abort();
  }
  bool hxc_l_loadingFramePresented = false;
  hxc_l_gc_roots[79] = (const void *)hxc_l_contentStore;
  struct hxc_caxecraft_content_CampaignPackageLoadResult hxc_l_tmp_call_result_n70 = hxc_caxecraft_content_CampaignRuntime_loadCampaignManifest(hxc_l_contentStore, (hxc_string){ (const uint8_t *)"campaigns/first-adventure/campaign.json", 39, true, NULL });
  hxc_l_gc_roots[80] = hxc_l_tmp_call_result_n70.hxc_tag == hxc_caxecraft_content_CampaignPackageLoadResult_CampaignPackageReady ? (const void *)hxc_l_tmp_call_result_n70.hxc_payload.hxc_CampaignPackageReady.hxc_manifest : NULL;
  hxc_l_g_hccea2de18028 = hxc_l_tmp_call_result_n70;
  hxc_l_gc_roots[81] = hxc_l_g_hccea2de18028.hxc_tag == hxc_caxecraft_content_CampaignPackageLoadResult_CampaignPackageReady ? (const void *)hxc_l_g_hccea2de18028.hxc_payload.hxc_CampaignPackageReady.hxc_manifest : NULL;
  switch (hxc_l_g_hccea2de18028.hxc_tag) {
    case hxc_caxecraft_content_CampaignPackageLoadResult_CampaignPackageReady:
      {
        hxc_l_gc_roots[82] = hxc_l_g_hccea2de18028.hxc_tag == hxc_caxecraft_content_CampaignPackageLoadResult_CampaignPackageReady ? (const void *)hxc_l_g_hccea2de18028.hxc_payload.hxc_CampaignPackageReady.hxc_manifest : NULL;
        if (hxc_l_g_hccea2de18028.hxc_tag != hxc_caxecraft_content_CampaignPackageLoadResult_CampaignPackageReady)
        {
          abort();
        }
        struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_enum_payload_project_n73 = hxc_l_g_hccea2de18028.hxc_payload.hxc_CampaignPackageReady.hxc_manifest;
        hxc_l_gc_roots[83] = (const void *)hxc_l_tmp_enum_payload_project_n73;
        struct hxc_caxecraft_content_CampaignManifest *hxc_l_manifest = hxc_l_tmp_enum_payload_project_n73;
        struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n74 = hxc_l_manifest;
        hxc_l_gc_roots[84] = (const void *)hxc_l_tmp_load_result_n74;
        if (hxc_l_tmp_load_result_n74 == NULL)
        {
          abort();
        }
        struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n75 = (*hxc_l_tmp_load_result_n74).hxc_admittedLevels;
        hxc_l_gc_roots[85] = (const void *)hxc_l_tmp_class_field_load_result_n75;
        struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n76 = hxc_l_manifest;
        hxc_l_gc_roots[86] = (const void *)hxc_l_tmp_load_result_n76;
        if (hxc_l_tmp_load_result_n76 == NULL)
        {
          abort();
        }
        int32_t hxc_l_tmp_class_field_load_result_n77 = (*hxc_l_tmp_load_result_n76).hxc_entryIndexValue;
        struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_array_get_result_n78;
        if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n75, (size_t)hxc_l_tmp_class_field_load_result_n77, &hxc_l_tmp_array_get_result_n78) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_l_gc_roots[87] = (const void *)hxc_l_tmp_array_get_result_n78;
        struct hxc_caxecraft_content_CampaignLevel *hxc_l_entry = hxc_l_tmp_array_get_result_n78;
        struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_load_result_n79 = hxc_l_entry;
        hxc_l_gc_roots[88] = (const void *)hxc_l_tmp_load_result_n79;
        if (hxc_l_tmp_load_result_n79 == NULL)
        {
          abort();
        }
        hxc_string hxc_l_tmp_class_field_load_result_n80 = (*hxc_l_tmp_load_result_n79).hxc_logicalPath;
        struct hxc_caxecraft_content_RuntimeLevelCandidate *hxc_l_tmp_load_result_n81 = hxc_l_loadedCandidate;
        hxc_l_gc_roots[89] = (const void *)hxc_l_tmp_load_result_n81;
        if (hxc_l_tmp_load_result_n81 == NULL)
        {
          abort();
        }
        struct hxc_caxecraft_content_RuntimeLevelReceipt hxc_l_tmp_instance_call_result_n82 = hxc_caxecraft_content_RuntimeLevelCandidate_receipt(hxc_l_tmp_load_result_n81);
        hxc_l_tmp_record_field_logicalPath_receiver_owner_n42 = hxc_l_tmp_instance_call_result_n82;
        bool hxc_l_tmp_string_equality_result_n43 = !(hxc_l_tmp_class_field_load_result_n80.data == NULL || hxc_l_tmp_record_field_logicalPath_receiver_owner_n42.hxc_logicalPath.data == NULL ? hxc_l_tmp_class_field_load_result_n80.data == hxc_l_tmp_record_field_logicalPath_receiver_owner_n42.hxc_logicalPath.data : hxc_l_tmp_class_field_load_result_n80.byte_length == hxc_l_tmp_record_field_logicalPath_receiver_owner_n42.hxc_logicalPath.byte_length && (hxc_l_tmp_class_field_load_result_n80.byte_length == 0 || memcmp(hxc_l_tmp_class_field_load_result_n80.data, hxc_l_tmp_record_field_logicalPath_receiver_owner_n42.hxc_logicalPath.data, hxc_l_tmp_class_field_load_result_n80.byte_length) == 0));
        bool hxc_l_tmp_string_equality_result_result_n86 = hxc_l_tmp_string_equality_result_n43;
        hxc_record_f6127605_destroy(&hxc_l_tmp_record_field_logicalPath_receiver_owner_n42);
        if (hxc_l_tmp_string_equality_result_result_n86)
        {
          if (hxc_io_println((hxc_string){ (const uint8_t *)"caxecraft: campaign entry does not match the active map", 55, true, NULL }) != HXC_STATUS_OK)
          {
            abort();
          }
          hxc_enum_4b68fff2_destroy(&hxc_l_g_hccea2de18028);
          if (hxc_string_release(&hxc_l_pendingCampaignLabel) != HXC_STATUS_OK)
          {
            abort();
          }
          hxc_optional_9d9d283f_destroy(&hxc_l_activeEditorTestPlay);
          hxc_enum_11853fd1_destroy(&hxc_l_g_h1d08383de4f8);
          hxc_record_f6127605_destroy(&hxc_l_tmp_record_field_logicalPath_receiver_owner_n23);
          hxc_enum_777c0102_destroy(&hxc_l_g_h6904c3060e30);
          hxc_record_8aab5967_destroy(&hxc_l_tmp_managed_flow_owner_n17);
          hxc_enum_d4806d01_destroy(&hxc_l_g_h1d9290112915);
          hxc_enum_fcb4401b_destroy(&hxc_l_g_h14daa0fd57c5);
          if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
          {
            abort();
          }
          return;
        }
        else
        {
          hxc_l_gc_roots[91] = (const void *)hxc_l_manifest;
          hxc_l_campaign = hxc_l_manifest;
          hxc_l_gc_roots[92] = (const void *)hxc_l_entry;
          hxc_l_campaignLevel = hxc_l_entry;
        }
        break;
      }
    case hxc_caxecraft_content_CampaignPackageLoadResult_CampaignPackageRejected:
      {
        hxc_l_gc_roots[90] = hxc_l_g_hccea2de18028.hxc_tag == hxc_caxecraft_content_CampaignPackageLoadResult_CampaignPackageReady ? (const void *)hxc_l_g_hccea2de18028.hxc_payload.hxc_CampaignPackageReady.hxc_manifest : NULL;
        if (hxc_l_g_hccea2de18028.hxc_tag != hxc_caxecraft_content_CampaignPackageLoadResult_CampaignPackageRejected)
        {
          abort();
        }
        struct hxc_caxecraft_content_CampaignPackageLoadError hxc_l_tmp_enum_payload_project_n90 = hxc_l_g_hccea2de18028.hxc_payload.hxc_CampaignPackageRejected.hxc_error;
        hxc_l_gerror = hxc_l_tmp_enum_payload_project_n90;
        if (hxc_enum_7e02e9cb_retain(&hxc_l_gerror) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_l_gerror.hxc_tag == hxc_caxecraft_content_CampaignPackageLoadError_CampaignManifestSourceRejected)
        {
          if (hxc_l_gerror.hxc_tag != hxc_caxecraft_content_CampaignPackageLoadError_CampaignManifestSourceRejected)
          {
            abort();
          }
          struct hxc_caxecraft_content_ContentPackageError hxc_l_tmp_enum_payload_project_n93 = hxc_l_gerror.hxc_payload.hxc_CampaignManifestSourceRejected.hxc_error;
          if (hxc_l_tmp_enum_payload_project_n93.hxc_tag == hxc_caxecraft_content_ContentPackageError_EntryMissing)
          {
            if (hxc_io_println((hxc_string){ (const uint8_t *)"caxecraft: campaign-source=standalone-map", 41, true, NULL }) != HXC_STATUS_OK)
            {
              abort();
            }
            hxc_enum_7e02e9cb_destroy(&hxc_l_gerror);
          }
          else
          {
            if (hxc_io_println((hxc_string){ (const uint8_t *)"caxecraft: campaign manifest rejected: campaigns/first-adventure/campaign.json", 78, true, NULL }) != HXC_STATUS_OK)
            {
              abort();
            }
            hxc_enum_7e02e9cb_destroy(&hxc_l_gerror);
            hxc_enum_4b68fff2_destroy(&hxc_l_g_hccea2de18028);
            if (hxc_string_release(&hxc_l_pendingCampaignLabel) != HXC_STATUS_OK)
            {
              abort();
            }
            hxc_optional_9d9d283f_destroy(&hxc_l_activeEditorTestPlay);
            hxc_enum_11853fd1_destroy(&hxc_l_g_h1d08383de4f8);
            hxc_record_f6127605_destroy(&hxc_l_tmp_record_field_logicalPath_receiver_owner_n23);
            hxc_enum_777c0102_destroy(&hxc_l_g_h6904c3060e30);
            hxc_record_8aab5967_destroy(&hxc_l_tmp_managed_flow_owner_n17);
            hxc_enum_d4806d01_destroy(&hxc_l_g_h1d9290112915);
            hxc_enum_fcb4401b_destroy(&hxc_l_g_h14daa0fd57c5);
            if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
            {
              abort();
            }
            return;
          }
        }
        else
        {
          if (hxc_io_println((hxc_string){ (const uint8_t *)"caxecraft: campaign manifest rejected: campaigns/first-adventure/campaign.json", 78, true, NULL }) != HXC_STATUS_OK)
          {
            abort();
          }
          hxc_enum_7e02e9cb_destroy(&hxc_l_gerror);
          hxc_enum_4b68fff2_destroy(&hxc_l_g_hccea2de18028);
          if (hxc_string_release(&hxc_l_pendingCampaignLabel) != HXC_STATUS_OK)
          {
            abort();
          }
          hxc_optional_9d9d283f_destroy(&hxc_l_activeEditorTestPlay);
          hxc_enum_11853fd1_destroy(&hxc_l_g_h1d08383de4f8);
          hxc_record_f6127605_destroy(&hxc_l_tmp_record_field_logicalPath_receiver_owner_n23);
          hxc_enum_777c0102_destroy(&hxc_l_g_h6904c3060e30);
          hxc_record_8aab5967_destroy(&hxc_l_tmp_managed_flow_owner_n17);
          hxc_enum_d4806d01_destroy(&hxc_l_g_h1d9290112915);
          hxc_enum_fcb4401b_destroy(&hxc_l_g_h14daa0fd57c5);
          if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
          {
            abort();
          }
          return;
        }
        break;
      }
    default:
      {
        abort();
      }
  }
  struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_load_result_n94 = hxc_l_campaignLevel;
  hxc_l_gc_roots[93] = (const void *)hxc_l_tmp_load_result_n94;
  hxc_l_gc_roots[94] = (const void *)NULL;
  hxc_string hxc_l_tmp_conditional_managed_result_n46 = { 0 };
  if (hxc_l_tmp_load_result_n94 == NULL)
  {
    struct hxc_caxecraft_content_RuntimeLevelCandidate *hxc_l_tmp_load_result_n95 = hxc_l_loadedCandidate;
    hxc_l_gc_roots[95] = (const void *)hxc_l_tmp_load_result_n95;
    if (hxc_l_tmp_load_result_n95 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_content_RuntimeLevelReceipt hxc_l_tmp_instance_call_result_n96 = hxc_caxecraft_content_RuntimeLevelCandidate_receipt(hxc_l_tmp_load_result_n95);
    hxc_l_tmp_record_field_logicalPath_receiver_owner_n47 = hxc_l_tmp_instance_call_result_n96;
    (void)hxc_l_tmp_record_field_logicalPath_receiver_owner_n47.hxc_logicalPath;
    hxc_l_tmp_conditional_managed_result_n46 = hxc_l_tmp_record_field_logicalPath_receiver_owner_n47.hxc_logicalPath;
    if (hxc_string_retain(hxc_l_tmp_conditional_managed_result_n46) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_record_f6127605_destroy(&hxc_l_tmp_record_field_logicalPath_receiver_owner_n47);
  }
  else
  {
    struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_load_result_n99 = hxc_l_campaignLevel;
    hxc_l_gc_roots[96] = (const void *)hxc_l_tmp_load_result_n99;
    if (hxc_l_tmp_load_result_n99 == NULL)
    {
      abort();
    }
    hxc_string hxc_l_tmp_class_field_load_result_n100 = (*hxc_l_tmp_load_result_n99).hxc_id;
    (void)hxc_l_tmp_class_field_load_result_n100;
    hxc_l_tmp_conditional_managed_result_n46 = hxc_l_tmp_class_field_load_result_n100;
    if (hxc_string_retain(hxc_l_tmp_conditional_managed_result_n46) != HXC_STATUS_OK)
    {
      abort();
    }
  }
  hxc_string hxc_l_tmp_conditional_managed_move_result_n101 = hxc_l_tmp_conditional_managed_result_n46;
  hxc_l_levelLabel = hxc_l_tmp_conditional_managed_move_result_n101;
  int32_t hxc_l_selectedCampaignLevelIndex = 0;
  struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n102 = hxc_l_campaign;
  hxc_l_gc_roots[97] = (const void *)hxc_l_tmp_load_result_n102;
  hxc_l_gc_roots[98] = (const void *)NULL;
  bool hxc_l_tmp_short_circuit_result_n49 = hxc_l_tmp_load_result_n102 != NULL;
  if (hxc_l_tmp_load_result_n102 != NULL)
  {
    hxc_l_gc_roots[99] = (const void *)hxc_l_campaignLevel;
    hxc_l_gc_roots[100] = (const void *)NULL;
    hxc_l_tmp_short_circuit_result_n49 = hxc_l_campaignLevel != NULL;
  }
  if (hxc_l_tmp_short_circuit_result_n49)
  {
    int32_t hxc_l_g_h28d8b51e2b9a = 0;
    struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n105 = hxc_l_campaign;
    hxc_l_gc_roots[101] = (const void *)hxc_l_tmp_load_result_n105;
    if (hxc_l_tmp_load_result_n105 == NULL)
    {
      abort();
    }
    struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n106 = (*hxc_l_tmp_load_result_n105).hxc_admittedLevels;
    hxc_l_gc_roots[102] = (const void *)hxc_l_tmp_class_field_load_result_n106;
    int32_t hxc_l_tmp_array_length_result_n107;
    if (hxc_array_ref_length(hxc_l_tmp_class_field_load_result_n106, &hxc_l_tmp_array_length_result_n107) != HXC_STATUS_OK)
    {
      abort();
    }
    int32_t hxc_l_g1_h0aca575657dc = hxc_l_tmp_array_length_result_n107;
    while (1)
    {
      int32_t hxc_l_tmp_load_result_n108 = hxc_l_g_h28d8b51e2b9a;
      if (!(hxc_l_tmp_load_result_n108 < hxc_l_g1_h0aca575657dc))
      {
        break;
      }
      int32_t hxc_l_tmp_increment_load_result_n110 = hxc_l_g_h28d8b51e2b9a;
      hxc_l_g_h28d8b51e2b9a = hxc_i32_add_wrapping(hxc_l_tmp_increment_load_result_n110, 1);
      int32_t hxc_l_index_hd1c05fdef16d = hxc_l_tmp_increment_load_result_n110;
      struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n111 = hxc_l_campaign;
      hxc_l_gc_roots[113] = (const void *)hxc_l_tmp_load_result_n111;
      if (hxc_l_tmp_load_result_n111 == NULL)
      {
        abort();
      }
      struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n112 = (*hxc_l_tmp_load_result_n111).hxc_admittedLevels;
      hxc_l_gc_roots[114] = (const void *)hxc_l_tmp_class_field_load_result_n112;
      struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_array_get_result_n114;
      if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n112, (size_t)hxc_l_index_hd1c05fdef16d, &hxc_l_tmp_array_get_result_n114) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_l_gc_roots[115] = (const void *)hxc_l_tmp_array_get_result_n114;
      if (hxc_l_tmp_array_get_result_n114 == NULL)
      {
        abort();
      }
      hxc_string hxc_l_tmp_class_field_load_result_n115 = (*hxc_l_tmp_array_get_result_n114).hxc_id;
      struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_load_result_n116 = hxc_l_campaignLevel;
      hxc_l_gc_roots[116] = (const void *)hxc_l_tmp_load_result_n116;
      if (hxc_l_tmp_load_result_n116 == NULL)
      {
        abort();
      }
      hxc_string hxc_l_tmp_class_field_load_result_n117 = (*hxc_l_tmp_load_result_n116).hxc_id;
      bool hxc_l_tmp_string_equality_result_n53 = hxc_l_tmp_class_field_load_result_n115.data == NULL || hxc_l_tmp_class_field_load_result_n117.data == NULL ? hxc_l_tmp_class_field_load_result_n115.data == hxc_l_tmp_class_field_load_result_n117.data : hxc_l_tmp_class_field_load_result_n115.byte_length == hxc_l_tmp_class_field_load_result_n117.byte_length && (hxc_l_tmp_class_field_load_result_n115.byte_length == 0 || memcmp(hxc_l_tmp_class_field_load_result_n115.data, hxc_l_tmp_class_field_load_result_n117.data, hxc_l_tmp_class_field_load_result_n115.byte_length) == 0);
      if (hxc_l_tmp_string_equality_result_n53)
      {
        hxc_l_selectedCampaignLevelIndex = hxc_l_index_hd1c05fdef16d;
      }
    }
  }
  struct hxc_caxecraft_app_ActivePlayableLevel *hxc_l_tmp_load_result_n121 = hxc_l_activeLevel;
  hxc_l_gc_roots[103] = (const void *)hxc_l_tmp_load_result_n121;
  if (hxc_l_tmp_load_result_n121 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_class_field_load_result_n122 = (*hxc_l_tmp_load_result_n121).hxc_activeView;
  hxc_l_gc_roots[104] = (const void *)hxc_l_tmp_class_field_load_result_n122;
  struct hxc_caxecraft_app_PlayableLevelView *hxc_l_initialLevel = hxc_l_tmp_class_field_load_result_n122;
  struct hxc_caxecraft_app_ActivePlayableLevel *hxc_l_tmp_load_result_n123 = hxc_l_activeLevel;
  hxc_l_gc_roots[105] = (const void *)hxc_l_tmp_load_result_n123;
  if (hxc_l_tmp_load_result_n123 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_content_ActiveContent *hxc_l_tmp_class_field_load_result_n124 = (*hxc_l_tmp_load_result_n123).hxc_content;
  hxc_l_gc_roots[106] = (const void *)hxc_l_tmp_class_field_load_result_n124;
  if (hxc_l_tmp_class_field_load_result_n124 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_content_LoadedContentGeneration *hxc_l_tmp_class_field_load_result_n125 = (*hxc_l_tmp_class_field_load_result_n124).hxc_active;
  hxc_l_gc_roots[107] = (const void *)hxc_l_tmp_class_field_load_result_n125;
  if (hxc_l_tmp_class_field_load_result_n125 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_class_field_load_result_n126 = (*hxc_l_tmp_class_field_load_result_n125).hxc_ownedSession;
  hxc_l_gc_roots[108] = (const void *)hxc_l_tmp_class_field_load_result_n126;
  struct hxc_caxecraft_domain_GameSession *hxc_l_initialSession = hxc_l_tmp_class_field_load_result_n126;
  struct hxc_caxecraft_content_RuntimeLevelCandidate *hxc_l_tmp_load_result_n127 = hxc_l_loadedCandidate;
  hxc_l_gc_roots[109] = (const void *)hxc_l_tmp_load_result_n127;
  if (hxc_l_tmp_load_result_n127 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_content_RuntimeLevelReceipt hxc_l_tmp_instance_call_result_n128 = hxc_caxecraft_content_RuntimeLevelCandidate_receipt(hxc_l_tmp_load_result_n127);
  hxc_l_receipt = hxc_l_tmp_instance_call_result_n128;
  if (hxc_io_println((hxc_string){ (const uint8_t *)"caxecraft: content-source=runtime-package", 41, true, NULL }) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_string hxc_l_tmp_string_concat_result_n130 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
  if (hxc_string_concat_ref((hxc_string){ (const uint8_t *)"caxecraft: content-path=", 24, true, NULL }, hxc_l_receipt.hxc_logicalPath, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n130) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_tmp_sys_println_string_argument_owner_n57 = hxc_l_tmp_string_concat_result_n130;
  if (hxc_io_println(hxc_l_tmp_sys_println_string_argument_owner_n57) != HXC_STATUS_OK)
  {
    if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n57) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_record_f6127605_destroy(&hxc_l_receipt);
    if (hxc_string_release(&hxc_l_levelLabel) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_enum_4b68fff2_destroy(&hxc_l_g_hccea2de18028);
    if (hxc_string_release(&hxc_l_pendingCampaignLabel) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_optional_9d9d283f_destroy(&hxc_l_activeEditorTestPlay);
    hxc_enum_11853fd1_destroy(&hxc_l_g_h1d08383de4f8);
    hxc_record_f6127605_destroy(&hxc_l_tmp_record_field_logicalPath_receiver_owner_n23);
    hxc_enum_777c0102_destroy(&hxc_l_g_h6904c3060e30);
    hxc_record_8aab5967_destroy(&hxc_l_tmp_managed_flow_owner_n17);
    hxc_enum_d4806d01_destroy(&hxc_l_g_h1d9290112915);
    hxc_enum_fcb4401b_destroy(&hxc_l_g_h14daa0fd57c5);
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    abort();
  }
  if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n57) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_string hxc_l_tmp_std_string_int_result_n133 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
  if (hxc_string_from_int32(hxc_l_receipt.hxc_inputHash, hxc_default_allocator(), &hxc_l_tmp_std_string_int_result_n133) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_tmp_string_concat_right_owner_n58 = hxc_l_tmp_std_string_int_result_n133;
  hxc_string hxc_l_tmp_string_concat_result_n135 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
  if (hxc_string_concat_ref((hxc_string){ (const uint8_t *)"caxecraft: content-input-hash=", 30, true, NULL }, hxc_l_tmp_string_concat_right_owner_n58, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n135) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_tmp_sys_println_string_argument_owner_n59 = hxc_l_tmp_string_concat_result_n135;
  if (hxc_io_println(hxc_l_tmp_sys_println_string_argument_owner_n59) != HXC_STATUS_OK)
  {
    if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n59) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n58) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_record_f6127605_destroy(&hxc_l_receipt);
    if (hxc_string_release(&hxc_l_levelLabel) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_enum_4b68fff2_destroy(&hxc_l_g_hccea2de18028);
    if (hxc_string_release(&hxc_l_pendingCampaignLabel) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_optional_9d9d283f_destroy(&hxc_l_activeEditorTestPlay);
    hxc_enum_11853fd1_destroy(&hxc_l_g_h1d08383de4f8);
    hxc_record_f6127605_destroy(&hxc_l_tmp_record_field_logicalPath_receiver_owner_n23);
    hxc_enum_777c0102_destroy(&hxc_l_g_h6904c3060e30);
    hxc_record_8aab5967_destroy(&hxc_l_tmp_managed_flow_owner_n17);
    hxc_enum_d4806d01_destroy(&hxc_l_g_h1d9290112915);
    hxc_enum_fcb4401b_destroy(&hxc_l_g_h14daa0fd57c5);
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    abort();
  }
  if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n59) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n58) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_caxecraft_content_RuntimeContentGeneration *hxc_l_tmp_load_result_n137 = hxc_l_runtimeContent;
  hxc_l_gc_roots[110] = (const void *)hxc_l_tmp_load_result_n137;
  if (hxc_l_tmp_load_result_n137 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_content_RuntimeLevelCandidate *hxc_l_tmp_class_field_load_result_n138 = (*hxc_l_tmp_load_result_n137).hxc_loadedLevel;
  hxc_l_gc_roots[111] = (const void *)hxc_l_tmp_class_field_load_result_n138;
  if (hxc_l_tmp_class_field_load_result_n138 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_content_LoadedContentGeneration *hxc_l_tmp_class_field_load_result_n139 = (*hxc_l_tmp_class_field_load_result_n138).hxc_loadedGeneration;
  hxc_l_gc_roots[112] = (const void *)hxc_l_tmp_class_field_load_result_n139;
  if (hxc_l_tmp_class_field_load_result_n139 == NULL)
  {
    abort();
  }
  int32_t hxc_l_tmp_class_field_load_result_n140 = (*hxc_l_tmp_class_field_load_result_n139).hxc_generationIdValue;
  hxc_string hxc_l_tmp_std_string_int_result_n141 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
  if (hxc_string_from_int32(hxc_l_tmp_class_field_load_result_n140, hxc_default_allocator(), &hxc_l_tmp_std_string_int_result_n141) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_tmp_string_concat_right_owner_n60 = hxc_l_tmp_std_string_int_result_n141;
  hxc_string hxc_l_tmp_string_concat_result_n143 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
  if (hxc_string_concat_ref((hxc_string){ (const uint8_t *)"caxecraft: content-generation=", 30, true, NULL }, hxc_l_tmp_string_concat_right_owner_n60, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n143) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_tmp_sys_println_string_argument_owner_n61 = hxc_l_tmp_string_concat_result_n143;
  if (hxc_io_println(hxc_l_tmp_sys_println_string_argument_owner_n61) != HXC_STATUS_OK)
  {
    if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n61) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n60) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_record_f6127605_destroy(&hxc_l_receipt);
    if (hxc_string_release(&hxc_l_levelLabel) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_enum_4b68fff2_destroy(&hxc_l_g_hccea2de18028);
    if (hxc_string_release(&hxc_l_pendingCampaignLabel) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_optional_9d9d283f_destroy(&hxc_l_activeEditorTestPlay);
    hxc_enum_11853fd1_destroy(&hxc_l_g_h1d08383de4f8);
    hxc_record_f6127605_destroy(&hxc_l_tmp_record_field_logicalPath_receiver_owner_n23);
    hxc_enum_777c0102_destroy(&hxc_l_g_h6904c3060e30);
    hxc_record_8aab5967_destroy(&hxc_l_tmp_managed_flow_owner_n17);
    hxc_enum_d4806d01_destroy(&hxc_l_g_h1d9290112915);
    hxc_enum_fcb4401b_destroy(&hxc_l_g_h14daa0fd57c5);
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    abort();
  }
  if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n61) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n60) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_windowFlags = hxc_i32_bit_or(64, 4);
  hxc_l_windowFlags = hxc_i32_bit_or(hxc_l_windowFlags, 8192);
  SetConfigFlags((uint32_t)hxc_l_windowFlags);
  InitWindow((int32_t)1280, (int32_t)720, "Caxecraft \342\200\224 Haxe shaped into careful C");
  bool hxc_l_tmp_native_call_result_n147 = IsWindowReady();
  if (!hxc_l_tmp_native_call_result_n147)
  {
    hxc_record_f6127605_destroy(&hxc_l_receipt);
    if (hxc_string_release(&hxc_l_levelLabel) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_enum_4b68fff2_destroy(&hxc_l_g_hccea2de18028);
    if (hxc_string_release(&hxc_l_pendingCampaignLabel) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_optional_9d9d283f_destroy(&hxc_l_activeEditorTestPlay);
    hxc_enum_11853fd1_destroy(&hxc_l_g_h1d08383de4f8);
    hxc_record_f6127605_destroy(&hxc_l_tmp_record_field_logicalPath_receiver_owner_n23);
    hxc_enum_777c0102_destroy(&hxc_l_g_h6904c3060e30);
    hxc_record_8aab5967_destroy(&hxc_l_tmp_managed_flow_owner_n17);
    hxc_enum_d4806d01_destroy(&hxc_l_g_h1d9290112915);
    hxc_enum_fcb4401b_destroy(&hxc_l_g_h14daa0fd57c5);
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return;
  }
  SetWindowMinSize((int32_t)800, (int32_t)450);
  SetExitKey((int32_t)0);
  SetTargetFPS((int32_t)120);
  EnableCursor();
  hxc_string hxc_l_tmp_cstring_ref_constant_owner_n64 = (hxc_string){ (const uint8_t *)"assets/showcase/title-panorama.png", 34, true, NULL };
  hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n149 = HXC_BORROWED_CSTRING_INITIALIZER;
  if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_constant_owner_n64, &hxc_l_tmp_string_borrow_cstring_result_n149) != HXC_STATUS_OK)
  {
    abort();
  }
  struct Texture hxc_l_tmp_native_call_result_n150 = LoadTexture(hxc_l_tmp_string_borrow_cstring_result_n149.data);
  struct Texture hxc_l_titleTexture = hxc_l_tmp_native_call_result_n150;
  bool hxc_l_tmp_native_call_result_n152 = IsTextureValid(hxc_l_titleTexture);
  bool hxc_l_titleTextureReady = hxc_l_tmp_native_call_result_n152;
  hxc_string hxc_l_tmp_cstring_ref_constant_owner_n67 = (hxc_string){ (const uint8_t *)"assets/branding/caxecraft-wordmark.png", 38, true, NULL };
  hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n154 = HXC_BORROWED_CSTRING_INITIALIZER;
  if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_constant_owner_n67, &hxc_l_tmp_string_borrow_cstring_result_n154) != HXC_STATUS_OK)
  {
    abort();
  }
  struct Texture hxc_l_tmp_native_call_result_n155 = LoadTexture(hxc_l_tmp_string_borrow_cstring_result_n154.data);
  struct Texture hxc_l_wordmarkTexture = hxc_l_tmp_native_call_result_n155;
  bool hxc_l_tmp_native_call_result_n157 = IsTextureValid(hxc_l_wordmarkTexture);
  bool hxc_l_wordmarkTextureReady = hxc_l_tmp_native_call_result_n157;
  hxc_string hxc_l_tmp_cstring_ref_constant_owner_n70 = (hxc_string){ (const uint8_t *)"assets/atlases/hud.png", 22, true, NULL };
  hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n159 = HXC_BORROWED_CSTRING_INITIALIZER;
  if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_constant_owner_n70, &hxc_l_tmp_string_borrow_cstring_result_n159) != HXC_STATUS_OK)
  {
    abort();
  }
  struct Texture hxc_l_tmp_native_call_result_n160 = LoadTexture(hxc_l_tmp_string_borrow_cstring_result_n159.data);
  struct Texture hxc_l_hudTexture = hxc_l_tmp_native_call_result_n160;
  bool hxc_l_tmp_native_call_result_n162 = IsTextureValid(hxc_l_hudTexture);
  bool hxc_l_hudTextureReady = hxc_l_tmp_native_call_result_n162;
  hxc_string hxc_l_tmp_cstring_ref_constant_owner_n73 = (hxc_string){ (const uint8_t *)"assets/atlases/items.png", 24, true, NULL };
  hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n164 = HXC_BORROWED_CSTRING_INITIALIZER;
  if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_constant_owner_n73, &hxc_l_tmp_string_borrow_cstring_result_n164) != HXC_STATUS_OK)
  {
    abort();
  }
  struct Texture hxc_l_tmp_native_call_result_n165 = LoadTexture(hxc_l_tmp_string_borrow_cstring_result_n164.data);
  struct Texture hxc_l_itemTexture = hxc_l_tmp_native_call_result_n165;
  bool hxc_l_tmp_native_call_result_n167 = IsTextureValid(hxc_l_itemTexture);
  bool hxc_l_itemTextureReady = hxc_l_tmp_native_call_result_n167;
  hxc_string hxc_l_tmp_cstring_ref_constant_owner_n76 = (hxc_string){ (const uint8_t *)"assets/atlases/adventure-items.png", 34, true, NULL };
  hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n169 = HXC_BORROWED_CSTRING_INITIALIZER;
  if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_constant_owner_n76, &hxc_l_tmp_string_borrow_cstring_result_n169) != HXC_STATUS_OK)
  {
    abort();
  }
  struct Texture hxc_l_tmp_native_call_result_n170 = LoadTexture(hxc_l_tmp_string_borrow_cstring_result_n169.data);
  struct Texture hxc_l_adventureItemTexture = hxc_l_tmp_native_call_result_n170;
  bool hxc_l_tmp_native_call_result_n172 = IsTextureValid(hxc_l_adventureItemTexture);
  bool hxc_l_adventureItemTextureReady = hxc_l_tmp_native_call_result_n172;
  hxc_string hxc_l_tmp_cstring_ref_constant_owner_n79 = (hxc_string){ (const uint8_t *)"assets/atlases/entities.png", 27, true, NULL };
  hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n174 = HXC_BORROWED_CSTRING_INITIALIZER;
  if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_constant_owner_n79, &hxc_l_tmp_string_borrow_cstring_result_n174) != HXC_STATUS_OK)
  {
    abort();
  }
  struct Texture hxc_l_tmp_native_call_result_n175 = LoadTexture(hxc_l_tmp_string_borrow_cstring_result_n174.data);
  struct Texture hxc_l_entityTexture = hxc_l_tmp_native_call_result_n175;
  bool hxc_l_tmp_native_call_result_n177 = IsTextureValid(hxc_l_entityTexture);
  bool hxc_l_entityTextureReady = hxc_l_tmp_native_call_result_n177;
  struct Texture hxc_l_tmp_load_result_n178 = hxc_l_hudTexture;
  bool hxc_l_tmp_load_result_n179 = hxc_l_hudTextureReady;
  struct Texture hxc_l_tmp_load_result_n180 = hxc_l_itemTexture;
  bool hxc_l_tmp_load_result_n181 = hxc_l_itemTextureReady;
  struct Texture hxc_l_tmp_load_result_n182 = hxc_l_adventureItemTexture;
  bool hxc_l_tmp_load_result_n183 = hxc_l_adventureItemTextureReady;
  struct Texture hxc_l_tmp_load_result_n184 = hxc_l_entityTexture;
  struct hxc_caxecraft_app_HudResources hxc_l_hudResources = (struct hxc_caxecraft_app_HudResources){ .hxc_adventureItemTexture = hxc_l_tmp_load_result_n182, .hxc_adventureItemTextureReady = hxc_l_tmp_load_result_n183, .hxc_entityTexture = hxc_l_tmp_load_result_n184, .hxc_entityTextureReady = hxc_l_entityTextureReady, .hxc_hudTexture = hxc_l_tmp_load_result_n178, .hxc_hudTextureReady = hxc_l_tmp_load_result_n179, .hxc_itemTexture = hxc_l_tmp_load_result_n180, .hxc_itemTextureReady = hxc_l_tmp_load_result_n181 };
  hxc_string hxc_l_tmp_cstring_ref_constant_owner_n83 = (hxc_string){ (const uint8_t *)"assets/atlases/terrain.png", 26, true, NULL };
  hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n188 = HXC_BORROWED_CSTRING_INITIALIZER;
  if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_constant_owner_n83, &hxc_l_tmp_string_borrow_cstring_result_n188) != HXC_STATUS_OK)
  {
    abort();
  }
  struct Texture hxc_l_tmp_native_call_result_n189 = LoadTexture(hxc_l_tmp_string_borrow_cstring_result_n188.data);
  struct Texture hxc_l_terrainTexture = hxc_l_tmp_native_call_result_n189;
  bool hxc_l_tmp_native_call_result_n191 = IsTextureValid(hxc_l_terrainTexture);
  bool hxc_l_terrainTextureReady = hxc_l_tmp_native_call_result_n191;
  hxc_string hxc_l_tmp_cstring_ref_constant_owner_n86 = (hxc_string){ (const uint8_t *)"assets/atlases/adventure-terrain.png", 36, true, NULL };
  hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n193 = HXC_BORROWED_CSTRING_INITIALIZER;
  if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_constant_owner_n86, &hxc_l_tmp_string_borrow_cstring_result_n193) != HXC_STATUS_OK)
  {
    abort();
  }
  struct Texture hxc_l_tmp_native_call_result_n194 = LoadTexture(hxc_l_tmp_string_borrow_cstring_result_n193.data);
  struct Texture hxc_l_adventureTerrainTexture = hxc_l_tmp_native_call_result_n194;
  bool hxc_l_tmp_native_call_result_n196 = IsTextureValid(hxc_l_adventureTerrainTexture);
  bool hxc_l_adventureTerrainTextureReady = hxc_l_tmp_native_call_result_n196;
  struct hxc_caxecraft_app_RuntimeTextureAtlasCatalog *hxc_l_tmp_call_result_n197 = hxc_caxecraft_app_RuntimeTextureAtlasCatalog_load((hxc_string){ (const uint8_t *)"assets/caxecraft-runtime-assets.json", 36, true, NULL });
  hxc_l_gc_roots[117] = (const void *)hxc_l_tmp_call_result_n197;
  struct hxc_caxecraft_app_RuntimeTextureAtlasCatalog *hxc_l_loadedRuntimeTextures = hxc_l_tmp_call_result_n197;
  hxc_l_gc_roots[118] = (const void *)hxc_l_loadedRuntimeTextures;
  hxc_l_gc_roots[119] = (const void *)NULL;
  if (!(hxc_l_loadedRuntimeTextures == NULL))
  {
    hxc_l_gc_roots[120] = (const void *)hxc_l_loadedRuntimeTextures;
    struct hxc_caxecraft_app_RuntimeTextureAtlasCatalog *hxc_l_runtimeTextures = hxc_l_loadedRuntimeTextures;
    struct hxc_caxecraft_app_RuntimeVoxelModelCatalog *hxc_l_tmp_managed_class_result_n216 = NULL;
    if (hxc_gc_allocate(&hxc_program_gc, &hxc_caxecraft_app_RuntimeVoxelModelCatalog_descriptor, (void **)&hxc_l_tmp_managed_class_result_n216) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_gc_roots[121] = (const void *)hxc_l_tmp_managed_class_result_n216;
    hxc_compiler_constructor_caxecraft_app_RuntimeVoxelModelCatalog(hxc_l_tmp_managed_class_result_n216);
    hxc_l_gc_roots[122] = (const void *)hxc_l_tmp_managed_class_result_n216;
    struct hxc_caxecraft_app_RuntimeVoxelModelCatalog *hxc_l_runtimeModels = hxc_l_tmp_managed_class_result_n216;
    struct hxc_caxecraft_app_VoxelFrameAnimationPlayer hxc_l_tmp_object_storage_n92 = { 0 };
    struct hxc_caxecraft_app_VoxelFrameAnimationPlayer *hxc_l_tmp_class_object_address_n217 = &hxc_l_tmp_object_storage_n92;
    hxc_compiler_constructor_caxecraft_app_VoxelFrameAnimationPlayer(hxc_l_tmp_class_object_address_n217);
    struct hxc_caxecraft_app_VoxelFrameAnimationPlayer *hxc_l_modelAnimations = hxc_l_tmp_class_object_address_n217;
    double hxc_l_cameraWaterBlend = 0.0;
    enum hxc_caxecraft_domain_PlayerCameraMode hxc_l_cameraMode = hxc_caxecraft_domain_PlayerCameraMode_FirstPerson;
    bool hxc_l_debugHudVisible = false;
    struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n219 = hxc_caxecraft_gameplay_Inventory_starter();
    struct hxc_caxecraft_gameplay_InventoryState hxc_l_inventory = hxc_l_tmp_call_result_n219;
    struct hxc_optional_caxecraft_app_ConversationState hxc_l_conversation = (struct hxc_optional_caxecraft_app_ConversationState){ .hxc_has_value = false };
    hxc_l_activeDialogue = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_retain(hxc_l_activeDialogue) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_latestJournalId = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_retain(hxc_l_latestJournalId) != HXC_STATUS_OK)
    {
      abort();
    }
    struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n220 = hxc_l_initialLevel;
    hxc_l_gc_roots[123] = (const void *)hxc_l_tmp_load_result_n220;
    if (hxc_l_tmp_load_result_n220 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_class_field_load_result_n221 = (*hxc_l_tmp_load_result_n220).hxc_levelPresentation;
    hxc_l_gc_roots[124] = (const void *)hxc_l_tmp_class_field_load_result_n221;
    if (hxc_l_tmp_class_field_load_result_n221 == NULL)
    {
      abort();
    }
    hxc_string hxc_l_tmp_class_field_load_result_n222 = (*hxc_l_tmp_class_field_load_result_n221).hxc_startingObjective;
    hxc_l_currentObjectiveId = hxc_l_tmp_class_field_load_result_n222;
    if (hxc_string_retain(hxc_l_currentObjectiveId) != HXC_STATUS_OK)
    {
      abort();
    }
    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n223 = hxc_l_initialSession;
    hxc_l_gc_roots[125] = (const void *)hxc_l_tmp_load_result_n223;
    hxc_l_gc_roots[126] = (const void *)hxc_l_initialLevel;
    struct hxc_caxecraft_app_CaxecraftApp_AvailableInteractionTarget hxc_l_tmp_call_result_n225 = hxc_caxecraft_app_CaxecraftApp_nearestAvailableInteraction(hxc_l_tmp_load_result_n223, hxc_l_initialLevel);
    hxc_l_tmp_static_call_argument_0_owner_n102 = hxc_l_tmp_call_result_n225;
    enum hxc_caxecraft_app_InteractionPrompt hxc_l_tmp_call_result_n227 = hxc_caxecraft_app_CaxecraftApp_promptForAvailableInteraction(hxc_l_tmp_static_call_argument_0_owner_n102);
    enum hxc_caxecraft_app_InteractionPrompt hxc_l_availableInteractionPrompt = hxc_l_tmp_call_result_n227;
    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n228 = hxc_l_initialSession;
    hxc_l_gc_roots[127] = (const void *)hxc_l_tmp_load_result_n228;
    if (hxc_l_tmp_load_result_n228 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n229 = hxc_l_initialLevel;
    hxc_l_gc_roots[128] = (const void *)hxc_l_tmp_load_result_n229;
    if (hxc_l_tmp_load_result_n229 == NULL)
    {
      abort();
    }
    int32_t hxc_l_tmp_class_field_load_result_n230 = (*hxc_l_tmp_load_result_n229).hxc_enemyId;
    struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n231 = hxc_caxecraft_domain_GameSession_readCharacter(hxc_l_tmp_load_result_n228, hxc_l_tmp_class_field_load_result_n230);
    struct hxc_caxecraft_domain_Character hxc_l_enemyActor = hxc_l_tmp_instance_call_result_n231;
    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n232 = hxc_l_initialSession;
    hxc_l_gc_roots[129] = (const void *)hxc_l_tmp_load_result_n232;
    if (hxc_l_tmp_load_result_n232 == NULL)
    {
      abort();
    }
    struct hxc_array_ref *hxc_l_tmp_instance_call_result_n233 = hxc_caxecraft_domain_GameSession_actorControllerStateSnapshots(hxc_l_tmp_load_result_n232);
    hxc_l_initialActorPhases = hxc_l_tmp_instance_call_result_n233;
    struct hxc_array_ref *hxc_l_tmp_load_result_n234 = hxc_l_initialActorPhases;
    struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n235 = hxc_l_initialLevel;
    hxc_l_gc_roots[130] = (const void *)hxc_l_tmp_load_result_n235;
    if (hxc_l_tmp_load_result_n235 == NULL)
    {
      abort();
    }
    int32_t hxc_l_tmp_class_field_load_result_n236 = (*hxc_l_tmp_load_result_n235).hxc_enemyId;
    struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation hxc_l_tmp_call_result_n238 = hxc_caxecraft_app_CaxecraftApp_observeActorPhase(hxc_l_tmp_load_result_n234, hxc_l_tmp_class_field_load_result_n236, hxc_caxecraft_domain_ActorControllerPhase_Resting);
    struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation hxc_l_enemyPhase_h906f9c33685e = hxc_l_tmp_call_result_n238;
    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n239 = hxc_l_initialSession;
    hxc_l_gc_roots[131] = (const void *)hxc_l_tmp_load_result_n239;
    struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n240 = hxc_l_initialLevel;
    hxc_l_gc_roots[132] = (const void *)hxc_l_tmp_load_result_n240;
    bool hxc_l_tmp_call_result_n242 = hxc_caxecraft_app_CaxecraftApp_dialogueActorsAreValid(hxc_l_tmp_load_result_n239, hxc_l_tmp_load_result_n240, hxc_l_initialActorPhases);
    bool hxc_l_tmp_short_circuit_result_n106 = !hxc_l_tmp_call_result_n242;
    if (!!hxc_l_tmp_call_result_n242)
    {
      hxc_l_tmp_short_circuit_result_n106 = hxc_l_enemyActor.hxc_id <= 0;
    }
    bool hxc_l_tmp_short_circuit_load_result_n244 = hxc_l_tmp_short_circuit_result_n106;
    bool hxc_l_tmp_short_circuit_result_n107 = hxc_l_tmp_short_circuit_load_result_n244;
    if (!hxc_l_tmp_short_circuit_load_result_n244)
    {
      hxc_l_tmp_short_circuit_result_n107 = !hxc_l_enemyPhase_h906f9c33685e.hxc_valid;
    }
    if (hxc_l_tmp_short_circuit_result_n107)
    {
      if (hxc_array_ref_release(hxc_l_initialActorPhases) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_enum_2c0e695d_destroy(&hxc_l_tmp_static_call_argument_0_owner_n102);
      if (hxc_string_release(&hxc_l_currentObjectiveId) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_latestJournalId) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_activeDialogue) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_l_tmp_object_storage_n92.hxc_stateStartTicks) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_l_tmp_object_storage_n92.hxc_stateIds) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_l_tmp_object_storage_n92.hxc_objectIds) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_record_f6127605_destroy(&hxc_l_receipt);
      if (hxc_string_release(&hxc_l_levelLabel) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_enum_4b68fff2_destroy(&hxc_l_g_hccea2de18028);
      if (hxc_string_release(&hxc_l_pendingCampaignLabel) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_optional_9d9d283f_destroy(&hxc_l_activeEditorTestPlay);
      hxc_enum_11853fd1_destroy(&hxc_l_g_h1d08383de4f8);
      hxc_record_f6127605_destroy(&hxc_l_tmp_record_field_logicalPath_receiver_owner_n23);
      hxc_enum_777c0102_destroy(&hxc_l_g_h6904c3060e30);
      hxc_record_8aab5967_destroy(&hxc_l_tmp_managed_flow_owner_n17);
      hxc_enum_d4806d01_destroy(&hxc_l_g_h1d9290112915);
      hxc_enum_fcb4401b_destroy(&hxc_l_g_h14daa0fd57c5);
      if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
      {
        abort();
      }
      return;
    }
    struct hxc_caxecraft_gameplay_SwordCombatState hxc_l_swordCombat = (struct hxc_caxecraft_gameplay_SwordCombatState){ .hxc_cooldownTicks = 0 };
    struct hxc_caxecraft_gameplay_BerryDropState hxc_l_berryDrop = (struct hxc_caxecraft_gameplay_BerryDropState){ .hxc_active = false, .hxc_amount = 0, .hxc_x = 0.0, .hxc_y = 0.0, .hxc_z = 0.0 };
    struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n249 = hxc_l_initialLevel;
    hxc_l_gc_roots[133] = (const void *)hxc_l_tmp_load_result_n249;
    if (hxc_l_tmp_load_result_n249 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_scenario_ScenarioTransform hxc_l_tmp_class_field_load_result_n250 = (*hxc_l_tmp_load_result_n249).hxc_playerSpawn;
    struct hxc_caxecraft_app_RenderPosition hxc_l_tmp_call_result_n251 = hxc_caxecraft_app_SpawnCameraHeading_headingForSpawn(hxc_l_tmp_class_field_load_result_n250);
    struct hxc_caxecraft_app_RenderPosition hxc_l_initialHeading = hxc_l_tmp_call_result_n251;
    double hxc_l_lookX = hxc_l_initialHeading.hxc_x;
    double hxc_l_lookY = hxc_l_initialHeading.hxc_y;
    double hxc_l_lookZ = hxc_l_initialHeading.hxc_z;
    double hxc_l_accumulator = 0.0;
    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n255 = hxc_l_initialSession;
    hxc_l_gc_roots[134] = (const void *)hxc_l_tmp_load_result_n255;
    if (hxc_l_tmp_load_result_n255 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_domain_GameView hxc_l_tmp_instance_call_result_n256 = hxc_caxecraft_domain_GameSession_view(hxc_l_tmp_load_result_n255);
    struct hxc_caxecraft_domain_GameView hxc_l_initialPresentation = hxc_l_tmp_instance_call_result_n256;
    if (!hxc_l_initialPresentation.hxc_valid)
    {
      if (hxc_array_ref_release(hxc_l_initialActorPhases) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_enum_2c0e695d_destroy(&hxc_l_tmp_static_call_argument_0_owner_n102);
      if (hxc_string_release(&hxc_l_currentObjectiveId) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_latestJournalId) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_activeDialogue) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_l_tmp_object_storage_n92.hxc_stateStartTicks) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_l_tmp_object_storage_n92.hxc_stateIds) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_l_tmp_object_storage_n92.hxc_objectIds) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_record_f6127605_destroy(&hxc_l_receipt);
      if (hxc_string_release(&hxc_l_levelLabel) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_enum_4b68fff2_destroy(&hxc_l_g_hccea2de18028);
      if (hxc_string_release(&hxc_l_pendingCampaignLabel) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_optional_9d9d283f_destroy(&hxc_l_activeEditorTestPlay);
      hxc_enum_11853fd1_destroy(&hxc_l_g_h1d08383de4f8);
      hxc_record_f6127605_destroy(&hxc_l_tmp_record_field_logicalPath_receiver_owner_n23);
      hxc_enum_777c0102_destroy(&hxc_l_g_h6904c3060e30);
      hxc_record_8aab5967_destroy(&hxc_l_tmp_managed_flow_owner_n17);
      hxc_enum_d4806d01_destroy(&hxc_l_g_h1d9290112915);
      hxc_enum_fcb4401b_destroy(&hxc_l_g_h14daa0fd57c5);
      if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
      {
        abort();
      }
      return;
    }
    struct hxc_caxecraft_app_MotionHistory hxc_l_tmp_call_result_n259 = hxc_caxecraft_app_MotionInterpolation_start(hxc_l_initialPresentation.hxc_localPlayer.hxc_body);
    struct hxc_caxecraft_app_MotionHistory hxc_l_motionHistory = hxc_l_tmp_call_result_n259;
    hxc_l_gc_roots[135] = (const void *)NULL;
    struct hxc_caxecraft_app_PreparedCampaignPortal *hxc_l_preparedPortal = NULL;
    hxc_l_gc_roots[136] = (const void *)hxc_l_campaign;
    struct hxc_caxecraft_content_CampaignManifest *hxc_l_initialCampaign = hxc_l_campaign;
    hxc_l_gc_roots[137] = (const void *)hxc_l_campaignLevel;
    struct hxc_caxecraft_content_CampaignLevel *hxc_l_initialCampaignLevel = hxc_l_campaignLevel;
    struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n262 = hxc_l_initialCampaign;
    hxc_l_gc_roots[138] = (const void *)hxc_l_tmp_load_result_n262;
    hxc_l_gc_roots[139] = (const void *)NULL;
    bool hxc_l_tmp_short_circuit_result_n120 = hxc_l_tmp_load_result_n262 != NULL;
    if (hxc_l_tmp_load_result_n262 != NULL)
    {
      hxc_l_gc_roots[140] = (const void *)hxc_l_initialCampaignLevel;
      hxc_l_gc_roots[141] = (const void *)NULL;
      hxc_l_tmp_short_circuit_result_n120 = hxc_l_initialCampaignLevel != NULL;
    }
    if (hxc_l_tmp_short_circuit_result_n120)
    {
      int32_t hxc_l_tmp_record_field_load_result_n265 = hxc_l_initialPresentation.hxc_localPlayer.hxc_id;
      int32_t hxc_l_tmp_record_field_load_result_n266 = hxc_l_initialPresentation.hxc_localPlayer.hxc_vitals.hxc_health;
      struct hxc_caxecraft_content_LevelPlayerOptions hxc_l_portalPlayer = (struct hxc_caxecraft_content_LevelPlayerOptions){ .hxc_aquaticProfile = hxc_l_initialPresentation.hxc_localPlayer.hxc_aquaticProfile, .hxc_entityId = hxc_l_tmp_record_field_load_result_n265, .hxc_initialHealth = hxc_l_tmp_record_field_load_result_n266 };
      struct hxc_caxecraft_content_ContentPackageStore *hxc_l_tmp_load_result_n269 = hxc_l_contentStore;
      hxc_l_gc_roots[142] = (const void *)hxc_l_tmp_load_result_n269;
      struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n270 = hxc_l_initialCampaign;
      hxc_l_gc_roots[143] = (const void *)hxc_l_tmp_load_result_n270;
      struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_load_result_n271 = hxc_l_initialCampaignLevel;
      hxc_l_gc_roots[144] = (const void *)hxc_l_tmp_load_result_n271;
      struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n272 = hxc_l_initialLevel;
      hxc_l_gc_roots[145] = (const void *)hxc_l_tmp_load_result_n272;
      struct hxc_caxecraft_content_ContentGenerationSequence *hxc_l_tmp_load_result_n273 = hxc_l_generationSequence;
      hxc_l_gc_roots[146] = (const void *)hxc_l_tmp_load_result_n273;
      if (hxc_l_tmp_load_result_n273 == NULL)
      {
        abort();
      }
      int32_t hxc_l_tmp_instance_call_result_n274 = hxc_caxecraft_content_ContentGenerationSequence_allocate(hxc_l_tmp_load_result_n273);
      struct hxc_caxecraft_content_LevelPlayerOptions hxc_l_tmp_load_result_n275 = hxc_l_portalPlayer;
      hxc_l_gc_roots[147] = (const void *)(struct hxc_caxecraft_app_CampaignPortalPreloadRequest){ .hxc_campaign = hxc_l_tmp_load_result_n270, .hxc_nextGeneration = hxc_l_tmp_instance_call_result_n274, .hxc_playerOptions = hxc_l_tmp_load_result_n275, .hxc_sourceLevel = hxc_l_tmp_load_result_n271, .hxc_sourceView = hxc_l_tmp_load_result_n272, .hxc_store = hxc_l_tmp_load_result_n269 }.hxc_campaign;
      hxc_l_gc_roots[148] = (const void *)(struct hxc_caxecraft_app_CampaignPortalPreloadRequest){ .hxc_campaign = hxc_l_tmp_load_result_n270, .hxc_nextGeneration = hxc_l_tmp_instance_call_result_n274, .hxc_playerOptions = hxc_l_tmp_load_result_n275, .hxc_sourceLevel = hxc_l_tmp_load_result_n271, .hxc_sourceView = hxc_l_tmp_load_result_n272, .hxc_store = hxc_l_tmp_load_result_n269 }.hxc_sourceLevel;
      hxc_l_gc_roots[149] = (const void *)(struct hxc_caxecraft_app_CampaignPortalPreloadRequest){ .hxc_campaign = hxc_l_tmp_load_result_n270, .hxc_nextGeneration = hxc_l_tmp_instance_call_result_n274, .hxc_playerOptions = hxc_l_tmp_load_result_n275, .hxc_sourceLevel = hxc_l_tmp_load_result_n271, .hxc_sourceView = hxc_l_tmp_load_result_n272, .hxc_store = hxc_l_tmp_load_result_n269 }.hxc_sourceView;
      hxc_l_gc_roots[150] = (const void *)(struct hxc_caxecraft_app_CampaignPortalPreloadRequest){ .hxc_campaign = hxc_l_tmp_load_result_n270, .hxc_nextGeneration = hxc_l_tmp_instance_call_result_n274, .hxc_playerOptions = hxc_l_tmp_load_result_n275, .hxc_sourceLevel = hxc_l_tmp_load_result_n271, .hxc_sourceView = hxc_l_tmp_load_result_n272, .hxc_store = hxc_l_tmp_load_result_n269 }.hxc_store;
      struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_tmp_load_result_n277 = hxc_l_contentRegistry;
      hxc_l_gc_roots[151] = (const void *)hxc_l_tmp_load_result_n277;
      if (hxc_l_tmp_load_result_n277 == NULL)
      {
        abort();
      }
      struct hxc_compiler_interface_dispatch_caxecraft_scenario_ScenarioContentRegistry_value hxc_l_tmp_interface_value_n278 = (struct hxc_compiler_interface_dispatch_caxecraft_scenario_ScenarioContentRegistry_value){ .object = hxc_l_tmp_load_result_n277, .table = &hxc_itable_compiler_interface_dispatch_caxecraft_content_RuntimeContentRegistry_itable_layout_caxecraf_hb3f384641e1a61bf };
      struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_tmp_load_result_n279 = hxc_l_contentRegistry;
      hxc_l_gc_roots[152] = (const void *)hxc_l_tmp_load_result_n279;
      if (hxc_l_tmp_load_result_n279 == NULL)
      {
        abort();
      }
      struct hxc_compiler_interface_dispatch_caxecraft_content_LevelContentResolver_value hxc_l_tmp_interface_value_n280 = (struct hxc_compiler_interface_dispatch_caxecraft_content_LevelContentResolver_value){ .object = hxc_l_tmp_load_result_n279, .table = &hxc_itable_compiler_interface_dispatch_caxecraft_content_RuntimeContentRegistry_itable_layout_caxecraf_h82ca4f24ca7a7b5a };
      struct hxc_caxecraft_app_CampaignPortalPreloadResult hxc_l_tmp_call_result_n281 = hxc_caxecraft_app_CampaignPortal_preloadCampaignPortal((struct hxc_caxecraft_app_CampaignPortalPreloadRequest){ .hxc_campaign = hxc_l_tmp_load_result_n270, .hxc_nextGeneration = hxc_l_tmp_instance_call_result_n274, .hxc_playerOptions = hxc_l_tmp_load_result_n275, .hxc_sourceLevel = hxc_l_tmp_load_result_n271, .hxc_sourceView = hxc_l_tmp_load_result_n272, .hxc_store = hxc_l_tmp_load_result_n269 }, hxc_l_tmp_interface_value_n278, hxc_l_tmp_interface_value_n280);
      hxc_l_gc_roots[153] = hxc_l_tmp_call_result_n281.hxc_tag == hxc_caxecraft_app_CampaignPortalPreloadResult_CampaignPortalReady ? (const void *)hxc_l_tmp_call_result_n281.hxc_payload.hxc_CampaignPortalReady.hxc_portal : NULL;
      hxc_l_g_h5245aa56229c = hxc_l_tmp_call_result_n281;
      hxc_l_gc_roots[154] = hxc_l_g_h5245aa56229c.hxc_tag == hxc_caxecraft_app_CampaignPortalPreloadResult_CampaignPortalReady ? (const void *)hxc_l_g_h5245aa56229c.hxc_payload.hxc_CampaignPortalReady.hxc_portal : NULL;
      switch (hxc_l_g_h5245aa56229c.hxc_tag) {
        case hxc_caxecraft_app_CampaignPortalPreloadResult_CampaignPortalNotDeclared:
          {
            break;
          }
        case hxc_caxecraft_app_CampaignPortalPreloadResult_CampaignPortalReady:
          {
            hxc_l_gc_roots[155] = hxc_l_g_h5245aa56229c.hxc_tag == hxc_caxecraft_app_CampaignPortalPreloadResult_CampaignPortalReady ? (const void *)hxc_l_g_h5245aa56229c.hxc_payload.hxc_CampaignPortalReady.hxc_portal : NULL;
            if (hxc_l_g_h5245aa56229c.hxc_tag != hxc_caxecraft_app_CampaignPortalPreloadResult_CampaignPortalReady)
            {
              abort();
            }
            struct hxc_caxecraft_app_PreparedCampaignPortal *hxc_l_tmp_enum_payload_project_n284 = hxc_l_g_h5245aa56229c.hxc_payload.hxc_CampaignPortalReady.hxc_portal;
            hxc_l_gc_roots[156] = (const void *)hxc_l_tmp_enum_payload_project_n284;
            struct hxc_caxecraft_app_PreparedCampaignPortal *hxc_l_portal_h6534aab6a27c = hxc_l_tmp_enum_payload_project_n284;
            hxc_l_gc_roots[157] = (const void *)hxc_l_portal_h6534aab6a27c;
            hxc_l_preparedPortal = hxc_l_portal_h6534aab6a27c;
            struct hxc_caxecraft_app_PreparedCampaignPortal *hxc_l_tmp_load_result_n286 = hxc_l_portal_h6534aab6a27c;
            hxc_l_gc_roots[158] = (const void *)hxc_l_tmp_load_result_n286;
            if (hxc_l_tmp_load_result_n286 == NULL)
            {
              abort();
            }
            struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_class_field_load_result_n287 = (*hxc_l_tmp_load_result_n286).hxc_destinationValue;
            hxc_l_gc_roots[159] = (const void *)hxc_l_tmp_class_field_load_result_n287;
            if (hxc_l_tmp_class_field_load_result_n287 == NULL)
            {
              abort();
            }
            hxc_string hxc_l_tmp_class_field_load_result_n288 = (*hxc_l_tmp_class_field_load_result_n287).hxc_id;
            hxc_string hxc_l_tmp_string_concat_result_n289 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
            if (hxc_string_concat_ref((hxc_string){ (const uint8_t *)"caxecraft: campaign-portal=", 27, true, NULL }, hxc_l_tmp_class_field_load_result_n288, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n289) != HXC_STATUS_OK)
            {
              abort();
            }
            hxc_l_tmp_sys_println_string_argument_owner_n124 = hxc_l_tmp_string_concat_result_n289;
            if (hxc_io_println(hxc_l_tmp_sys_println_string_argument_owner_n124) != HXC_STATUS_OK)
            {
              if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n124) != HXC_STATUS_OK)
              {
                abort();
              }
              hxc_enum_9b450cfc_destroy(&hxc_l_g_h5245aa56229c);
              if (hxc_array_ref_release(hxc_l_initialActorPhases) != HXC_STATUS_OK)
              {
                abort();
              }
              hxc_enum_2c0e695d_destroy(&hxc_l_tmp_static_call_argument_0_owner_n102);
              if (hxc_string_release(&hxc_l_currentObjectiveId) != HXC_STATUS_OK)
              {
                abort();
              }
              if (hxc_string_release(&hxc_l_latestJournalId) != HXC_STATUS_OK)
              {
                abort();
              }
              if (hxc_string_release(&hxc_l_activeDialogue) != HXC_STATUS_OK)
              {
                abort();
              }
              if (hxc_array_ref_release(hxc_l_tmp_object_storage_n92.hxc_stateStartTicks) != HXC_STATUS_OK)
              {
                abort();
              }
              if (hxc_array_ref_release(hxc_l_tmp_object_storage_n92.hxc_stateIds) != HXC_STATUS_OK)
              {
                abort();
              }
              if (hxc_array_ref_release(hxc_l_tmp_object_storage_n92.hxc_objectIds) != HXC_STATUS_OK)
              {
                abort();
              }
              hxc_record_f6127605_destroy(&hxc_l_receipt);
              if (hxc_string_release(&hxc_l_levelLabel) != HXC_STATUS_OK)
              {
                abort();
              }
              hxc_enum_4b68fff2_destroy(&hxc_l_g_hccea2de18028);
              if (hxc_string_release(&hxc_l_pendingCampaignLabel) != HXC_STATUS_OK)
              {
                abort();
              }
              hxc_optional_9d9d283f_destroy(&hxc_l_activeEditorTestPlay);
              hxc_enum_11853fd1_destroy(&hxc_l_g_h1d08383de4f8);
              hxc_record_f6127605_destroy(&hxc_l_tmp_record_field_logicalPath_receiver_owner_n23);
              hxc_enum_777c0102_destroy(&hxc_l_g_h6904c3060e30);
              hxc_record_8aab5967_destroy(&hxc_l_tmp_managed_flow_owner_n17);
              hxc_enum_d4806d01_destroy(&hxc_l_g_h1d9290112915);
              hxc_enum_fcb4401b_destroy(&hxc_l_g_h14daa0fd57c5);
              if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
              {
                abort();
              }
              abort();
            }
            if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n124) != HXC_STATUS_OK)
            {
              abort();
            }
            break;
          }
        case hxc_caxecraft_app_CampaignPortalPreloadResult_CampaignPortalPreloadRejected:
          {
            if (hxc_io_println((hxc_string){ (const uint8_t *)"caxecraft: campaign portal rejected; ordinary loading remains available", 71, true, NULL }) != HXC_STATUS_OK)
            {
              abort();
            }
            break;
          }
        default:
          {
            abort();
          }
      }
      hxc_enum_9b450cfc_destroy(&hxc_l_g_h5245aa56229c);
    }
    bool hxc_l_jumpQueued = false;
    bool hxc_l_swordQueued = false;
    int32_t hxc_l_selectedMode = 0;
    int32_t hxc_l_locale = 0;
    bool hxc_l_showInitialTitle = true;
    enum hxc_caxecraft_app_AppScreen hxc_l_tmp_call_result_n292 = hxc_caxecraft_app_AppScreen_initialScreen(hxc_l_showInitialTitle);
    enum hxc_caxecraft_app_AppScreen hxc_l_screen = hxc_l_tmp_call_result_n292;
    bool hxc_l_tmp_call_result_n294 = hxc_caxecraft_app_AppScreen_capturesPointer(hxc_l_screen);
    if (hxc_l_tmp_call_result_n294)
    {
      DisableCursor();
    }
    bool hxc_l_quit = false;
    int32_t hxc_l_frameCount = 0;
    int32_t hxc_l_placementBlockedFrames = 0;
    int32_t hxc_l_strikeHitFrames = 0;
    int32_t hxc_l_enemyDefeatedFrames = 0;
    int32_t hxc_l_enemyAttackFrames = 0;
    int32_t hxc_l_pickupFrames = 0;
    int32_t hxc_l_pickupAmount = 0;
    int32_t hxc_l_inventoryFullFrames = 0;
    int32_t hxc_l_inventoryFullReason = 0;
    int32_t hxc_l_recoveryFeedback = 0;
    int32_t hxc_l_recoveryFeedbackFrames = 0;
    int32_t hxc_l_aquaticEquipmentCode = -1;
    int32_t hxc_l_aquaticEquipmentFrames = 0;
    while (1)
    {
      bool hxc_l_tmp_load_result_n295 = hxc_l_quit;
      bool hxc_l_tmp_short_circuit_result_n145 = !hxc_l_tmp_load_result_n295;
      if (!hxc_l_tmp_load_result_n295)
      {
        bool hxc_l_tmp_native_call_result_n296 = WindowShouldClose();
        hxc_l_tmp_short_circuit_result_n145 = !hxc_l_tmp_native_call_result_n296;
      }
      if (!hxc_l_tmp_short_circuit_result_n145)
      {
        break;
      }
      if (hxc_l_screen == hxc_caxecraft_app_AppScreen_EditorTestPlay)
      {
        hxc_l_gc_roots[170] = hxc_l_activeEditorTestPlay.hxc_has_value ? (const void *)hxc_l_activeEditorTestPlay.hxc_value.hxc_runtime : NULL;
        hxc_l_gc_roots[171] = hxc_l_activeEditorTestPlay.hxc_has_value ? (const void *)hxc_l_activeEditorTestPlay.hxc_value.hxc_snapshot.hxc_campaignLevel : NULL;
        hxc_l_gc_roots[172] = hxc_l_activeEditorTestPlay.hxc_has_value ? (const void *)hxc_l_activeEditorTestPlay.hxc_value.hxc_snapshot.hxc_pendingCampaignLevel : NULL;
        hxc_l_gc_roots[173] = hxc_l_activeEditorTestPlay.hxc_has_value ? (const void *)hxc_l_activeEditorTestPlay.hxc_value.hxc_snapshot.hxc_pendingCampaignPortal : NULL;
        hxc_l_gc_roots[174] = hxc_l_activeEditorTestPlay.hxc_has_value ? (const void *)hxc_l_activeEditorTestPlay.hxc_value.hxc_snapshot.hxc_preparedPortal : NULL;
        hxc_l_run_hefd5b31713eb = hxc_l_activeEditorTestPlay;
        if (hxc_optional_9d9d283f_retain(&hxc_l_run_hefd5b31713eb) != HXC_STATUS_OK)
        {
          abort();
        }
        struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun hxc_l_tmp_load_result_n301 = hxc_l_run_hefd5b31713eb;
        hxc_l_gc_roots[175] = hxc_l_tmp_load_result_n301.hxc_has_value ? (const void *)hxc_l_tmp_load_result_n301.hxc_value.hxc_runtime : NULL;
        hxc_l_gc_roots[176] = hxc_l_tmp_load_result_n301.hxc_has_value ? (const void *)hxc_l_tmp_load_result_n301.hxc_value.hxc_snapshot.hxc_campaignLevel : NULL;
        hxc_l_gc_roots[177] = hxc_l_tmp_load_result_n301.hxc_has_value ? (const void *)hxc_l_tmp_load_result_n301.hxc_value.hxc_snapshot.hxc_pendingCampaignLevel : NULL;
        hxc_l_gc_roots[178] = hxc_l_tmp_load_result_n301.hxc_has_value ? (const void *)hxc_l_tmp_load_result_n301.hxc_value.hxc_snapshot.hxc_pendingCampaignPortal : NULL;
        hxc_l_gc_roots[179] = hxc_l_tmp_load_result_n301.hxc_has_value ? (const void *)hxc_l_tmp_load_result_n301.hxc_value.hxc_snapshot.hxc_preparedPortal : NULL;
        bool hxc_l_tmp_short_circuit_result_n147 = !hxc_l_tmp_load_result_n301.hxc_has_value;
        if (!!hxc_l_tmp_load_result_n301.hxc_has_value)
        {
          struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun hxc_l_tmp_load_result_n302 = hxc_l_run_hefd5b31713eb;
          hxc_l_gc_roots[185] = hxc_l_tmp_load_result_n302.hxc_has_value ? (const void *)hxc_l_tmp_load_result_n302.hxc_value.hxc_runtime : NULL;
          hxc_l_gc_roots[186] = hxc_l_tmp_load_result_n302.hxc_has_value ? (const void *)hxc_l_tmp_load_result_n302.hxc_value.hxc_snapshot.hxc_campaignLevel : NULL;
          hxc_l_gc_roots[187] = hxc_l_tmp_load_result_n302.hxc_has_value ? (const void *)hxc_l_tmp_load_result_n302.hxc_value.hxc_snapshot.hxc_pendingCampaignLevel : NULL;
          hxc_l_gc_roots[188] = hxc_l_tmp_load_result_n302.hxc_has_value ? (const void *)hxc_l_tmp_load_result_n302.hxc_value.hxc_snapshot.hxc_pendingCampaignPortal : NULL;
          hxc_l_gc_roots[189] = hxc_l_tmp_load_result_n302.hxc_has_value ? (const void *)hxc_l_tmp_load_result_n302.hxc_value.hxc_snapshot.hxc_preparedPortal : NULL;
          if (!hxc_l_tmp_load_result_n302.hxc_has_value)
          {
            abort();
          }
          hxc_l_gc_roots[190] = (const void *)hxc_l_tmp_load_result_n302.hxc_value.hxc_runtime;
          hxc_l_gc_roots[191] = (const void *)hxc_l_tmp_load_result_n302.hxc_value.hxc_snapshot.hxc_campaignLevel;
          hxc_l_gc_roots[192] = (const void *)hxc_l_tmp_load_result_n302.hxc_value.hxc_snapshot.hxc_pendingCampaignLevel;
          hxc_l_gc_roots[193] = (const void *)hxc_l_tmp_load_result_n302.hxc_value.hxc_snapshot.hxc_pendingCampaignPortal;
          hxc_l_gc_roots[194] = (const void *)hxc_l_tmp_load_result_n302.hxc_value.hxc_snapshot.hxc_preparedPortal;
          struct hxc_caxecraft_app_EditorTestPlayRuntime *hxc_l_tmp_optional_record_field_project_n304 = hxc_l_tmp_load_result_n302.hxc_value.hxc_runtime;
          hxc_l_gc_roots[195] = (const void *)hxc_l_tmp_optional_record_field_project_n304;
          if (hxc_l_tmp_optional_record_field_project_n304 == NULL)
          {
            abort();
          }
          struct hxc_caxecraft_app_ActivePlayableLevel *hxc_l_tmp_class_field_load_result_n305 = (*hxc_l_tmp_optional_record_field_project_n304).hxc_active;
          hxc_l_gc_roots[196] = (const void *)hxc_l_tmp_class_field_load_result_n305;
          hxc_l_gc_roots[197] = (const void *)NULL;
          hxc_l_tmp_short_circuit_result_n147 = hxc_l_tmp_class_field_load_result_n305 == NULL;
        }
        if (hxc_l_tmp_short_circuit_result_n147)
        {
          if (hxc_io_println((hxc_string){ (const uint8_t *)"caxecraft: editor Test Play lost its disposable runtime", 55, true, NULL }) != HXC_STATUS_OK)
          {
            abort();
          }
          hxc_l_gc_roots[198] = hxc_l_run_hefd5b31713eb.hxc_has_value ? (const void *)hxc_l_run_hefd5b31713eb.hxc_value.hxc_runtime : NULL;
          hxc_l_gc_roots[199] = hxc_l_run_hefd5b31713eb.hxc_has_value ? (const void *)hxc_l_run_hefd5b31713eb.hxc_value.hxc_snapshot.hxc_campaignLevel : NULL;
          hxc_l_gc_roots[200] = hxc_l_run_hefd5b31713eb.hxc_has_value ? (const void *)hxc_l_run_hefd5b31713eb.hxc_value.hxc_snapshot.hxc_pendingCampaignLevel : NULL;
          hxc_l_gc_roots[201] = hxc_l_run_hefd5b31713eb.hxc_has_value ? (const void *)hxc_l_run_hefd5b31713eb.hxc_value.hxc_snapshot.hxc_pendingCampaignPortal : NULL;
          hxc_l_gc_roots[202] = hxc_l_run_hefd5b31713eb.hxc_has_value ? (const void *)hxc_l_run_hefd5b31713eb.hxc_value.hxc_snapshot.hxc_preparedPortal : NULL;
          if (hxc_l_run_hefd5b31713eb.hxc_has_value)
          {
            struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun hxc_l_tmp_load_result_n308 = hxc_l_run_hefd5b31713eb;
            hxc_l_gc_roots[203] = hxc_l_tmp_load_result_n308.hxc_has_value ? (const void *)hxc_l_tmp_load_result_n308.hxc_value.hxc_runtime : NULL;
            hxc_l_gc_roots[204] = hxc_l_tmp_load_result_n308.hxc_has_value ? (const void *)hxc_l_tmp_load_result_n308.hxc_value.hxc_snapshot.hxc_campaignLevel : NULL;
            hxc_l_gc_roots[205] = hxc_l_tmp_load_result_n308.hxc_has_value ? (const void *)hxc_l_tmp_load_result_n308.hxc_value.hxc_snapshot.hxc_pendingCampaignLevel : NULL;
            hxc_l_gc_roots[206] = hxc_l_tmp_load_result_n308.hxc_has_value ? (const void *)hxc_l_tmp_load_result_n308.hxc_value.hxc_snapshot.hxc_pendingCampaignPortal : NULL;
            hxc_l_gc_roots[207] = hxc_l_tmp_load_result_n308.hxc_has_value ? (const void *)hxc_l_tmp_load_result_n308.hxc_value.hxc_snapshot.hxc_preparedPortal : NULL;
            if (!hxc_l_tmp_load_result_n308.hxc_has_value)
            {
              abort();
            }
            hxc_l_gc_roots[208] = (const void *)hxc_l_tmp_load_result_n308.hxc_value.hxc_runtime;
            hxc_l_gc_roots[209] = (const void *)hxc_l_tmp_load_result_n308.hxc_value.hxc_snapshot.hxc_campaignLevel;
            hxc_l_gc_roots[210] = (const void *)hxc_l_tmp_load_result_n308.hxc_value.hxc_snapshot.hxc_pendingCampaignLevel;
            hxc_l_gc_roots[211] = (const void *)hxc_l_tmp_load_result_n308.hxc_value.hxc_snapshot.hxc_pendingCampaignPortal;
            hxc_l_gc_roots[212] = (const void *)hxc_l_tmp_load_result_n308.hxc_value.hxc_snapshot.hxc_preparedPortal;
            struct hxc_caxecraft_app_EditorTestPlayRuntime *hxc_l_tmp_optional_record_field_project_n310 = hxc_l_tmp_load_result_n308.hxc_value.hxc_runtime;
            hxc_l_gc_roots[213] = (const void *)hxc_l_tmp_optional_record_field_project_n310;
            if (hxc_l_tmp_optional_record_field_project_n310 == NULL)
            {
              abort();
            }
            hxc_caxecraft_app_EditorTestPlayRuntime_stop(hxc_l_tmp_optional_record_field_project_n310);
          }
          hxc_l_gc_roots[214] = (struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = false }.hxc_has_value ? (const void *)(struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = false }.hxc_value.hxc_runtime : NULL;
          hxc_l_gc_roots[215] = (struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = false }.hxc_has_value ? (const void *)(struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = false }.hxc_value.hxc_snapshot.hxc_campaignLevel : NULL;
          hxc_l_gc_roots[216] = (struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = false }.hxc_has_value ? (const void *)(struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = false }.hxc_value.hxc_snapshot.hxc_pendingCampaignLevel : NULL;
          hxc_l_gc_roots[217] = (struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = false }.hxc_has_value ? (const void *)(struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = false }.hxc_value.hxc_snapshot.hxc_pendingCampaignPortal : NULL;
          hxc_l_gc_roots[218] = (struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = false }.hxc_has_value ? (const void *)(struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = false }.hxc_value.hxc_snapshot.hxc_preparedPortal : NULL;
          hxc_optional_9d9d283f_destroy(&hxc_l_activeEditorTestPlay);
          hxc_l_activeEditorTestPlay = (struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = false };
          struct hxc_caxecraft_app_CaxecraftEditorScreen *hxc_l_tmp_load_result_n311 = hxc_l_editorScreen;
          hxc_l_gc_roots[219] = (const void *)hxc_l_tmp_load_result_n311;
          if (hxc_l_tmp_load_result_n311 == NULL)
          {
            abort();
          }
          hxc_caxecraft_app_CaxecraftEditorScreen_finishTestPlay(hxc_l_tmp_load_result_n311, false);
          hxc_l_quit = true;
          hxc_optional_9d9d283f_destroy(&hxc_l_run_hefd5b31713eb);
          break;
        }
        else
        {
          hxc_optional_9d9d283f_destroy(&hxc_l_run_hefd5b31713eb);
        }
      }
      else
      {
        hxc_l_gc_roots[180] = hxc_l_activeEditorTestPlay.hxc_has_value ? (const void *)hxc_l_activeEditorTestPlay.hxc_value.hxc_runtime : NULL;
        hxc_l_gc_roots[181] = hxc_l_activeEditorTestPlay.hxc_has_value ? (const void *)hxc_l_activeEditorTestPlay.hxc_value.hxc_snapshot.hxc_campaignLevel : NULL;
        hxc_l_gc_roots[182] = hxc_l_activeEditorTestPlay.hxc_has_value ? (const void *)hxc_l_activeEditorTestPlay.hxc_value.hxc_snapshot.hxc_pendingCampaignLevel : NULL;
        hxc_l_gc_roots[183] = hxc_l_activeEditorTestPlay.hxc_has_value ? (const void *)hxc_l_activeEditorTestPlay.hxc_value.hxc_snapshot.hxc_pendingCampaignPortal : NULL;
        hxc_l_gc_roots[184] = hxc_l_activeEditorTestPlay.hxc_has_value ? (const void *)hxc_l_activeEditorTestPlay.hxc_value.hxc_snapshot.hxc_preparedPortal : NULL;
        if (hxc_l_activeEditorTestPlay.hxc_has_value)
        {
          if (hxc_io_println((hxc_string){ (const uint8_t *)"caxecraft: editor Test Play runtime escaped its screen", 54, true, NULL }) != HXC_STATUS_OK)
          {
            abort();
          }
          struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun hxc_l_tmp_load_result_n313 = hxc_l_activeEditorTestPlay;
          hxc_l_gc_roots[220] = hxc_l_tmp_load_result_n313.hxc_has_value ? (const void *)hxc_l_tmp_load_result_n313.hxc_value.hxc_runtime : NULL;
          hxc_l_gc_roots[221] = hxc_l_tmp_load_result_n313.hxc_has_value ? (const void *)hxc_l_tmp_load_result_n313.hxc_value.hxc_snapshot.hxc_campaignLevel : NULL;
          hxc_l_gc_roots[222] = hxc_l_tmp_load_result_n313.hxc_has_value ? (const void *)hxc_l_tmp_load_result_n313.hxc_value.hxc_snapshot.hxc_pendingCampaignLevel : NULL;
          hxc_l_gc_roots[223] = hxc_l_tmp_load_result_n313.hxc_has_value ? (const void *)hxc_l_tmp_load_result_n313.hxc_value.hxc_snapshot.hxc_pendingCampaignPortal : NULL;
          hxc_l_gc_roots[224] = hxc_l_tmp_load_result_n313.hxc_has_value ? (const void *)hxc_l_tmp_load_result_n313.hxc_value.hxc_snapshot.hxc_preparedPortal : NULL;
          if (!hxc_l_tmp_load_result_n313.hxc_has_value)
          {
            abort();
          }
          hxc_l_gc_roots[225] = (const void *)hxc_l_tmp_load_result_n313.hxc_value.hxc_runtime;
          hxc_l_gc_roots[226] = (const void *)hxc_l_tmp_load_result_n313.hxc_value.hxc_snapshot.hxc_campaignLevel;
          hxc_l_gc_roots[227] = (const void *)hxc_l_tmp_load_result_n313.hxc_value.hxc_snapshot.hxc_pendingCampaignLevel;
          hxc_l_gc_roots[228] = (const void *)hxc_l_tmp_load_result_n313.hxc_value.hxc_snapshot.hxc_pendingCampaignPortal;
          hxc_l_gc_roots[229] = (const void *)hxc_l_tmp_load_result_n313.hxc_value.hxc_snapshot.hxc_preparedPortal;
          struct hxc_caxecraft_app_EditorTestPlayRuntime *hxc_l_tmp_optional_record_field_project_n315 = hxc_l_tmp_load_result_n313.hxc_value.hxc_runtime;
          hxc_l_gc_roots[230] = (const void *)hxc_l_tmp_optional_record_field_project_n315;
          if (hxc_l_tmp_optional_record_field_project_n315 == NULL)
          {
            abort();
          }
          hxc_caxecraft_app_EditorTestPlayRuntime_stop(hxc_l_tmp_optional_record_field_project_n315);
          hxc_l_gc_roots[231] = (struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = false }.hxc_has_value ? (const void *)(struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = false }.hxc_value.hxc_runtime : NULL;
          hxc_l_gc_roots[232] = (struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = false }.hxc_has_value ? (const void *)(struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = false }.hxc_value.hxc_snapshot.hxc_campaignLevel : NULL;
          hxc_l_gc_roots[233] = (struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = false }.hxc_has_value ? (const void *)(struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = false }.hxc_value.hxc_snapshot.hxc_pendingCampaignLevel : NULL;
          hxc_l_gc_roots[234] = (struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = false }.hxc_has_value ? (const void *)(struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = false }.hxc_value.hxc_snapshot.hxc_pendingCampaignPortal : NULL;
          hxc_l_gc_roots[235] = (struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = false }.hxc_has_value ? (const void *)(struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = false }.hxc_value.hxc_snapshot.hxc_preparedPortal : NULL;
          hxc_optional_9d9d283f_destroy(&hxc_l_activeEditorTestPlay);
          hxc_l_activeEditorTestPlay = (struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = false };
          struct hxc_caxecraft_app_CaxecraftEditorScreen *hxc_l_tmp_load_result_n316 = hxc_l_editorScreen;
          hxc_l_gc_roots[236] = (const void *)hxc_l_tmp_load_result_n316;
          if (hxc_l_tmp_load_result_n316 == NULL)
          {
            abort();
          }
          hxc_caxecraft_app_CaxecraftEditorScreen_finishTestPlay(hxc_l_tmp_load_result_n316, false);
          hxc_l_quit = true;
          break;
        }
      }
      bool hxc_l_tmp_call_result_n318 = hxc_caxecraft_app_AppScreen_showsLoading(hxc_l_screen);
      bool hxc_l_loadingAtFrameStart = hxc_l_tmp_call_result_n318;
      hxc_l_gc_roots[237] = (const void *)hxc_l_activeLevel;
      struct hxc_caxecraft_app_ActivePlayableLevel *hxc_l_frameLevelOwner = hxc_l_activeLevel;
      if (hxc_l_screen == hxc_caxecraft_app_AppScreen_EditorTestPlay)
      {
        hxc_l_gc_roots[238] = hxc_l_activeEditorTestPlay.hxc_has_value ? (const void *)hxc_l_activeEditorTestPlay.hxc_value.hxc_runtime : NULL;
        hxc_l_gc_roots[239] = hxc_l_activeEditorTestPlay.hxc_has_value ? (const void *)hxc_l_activeEditorTestPlay.hxc_value.hxc_snapshot.hxc_campaignLevel : NULL;
        hxc_l_gc_roots[240] = hxc_l_activeEditorTestPlay.hxc_has_value ? (const void *)hxc_l_activeEditorTestPlay.hxc_value.hxc_snapshot.hxc_pendingCampaignLevel : NULL;
        hxc_l_gc_roots[241] = hxc_l_activeEditorTestPlay.hxc_has_value ? (const void *)hxc_l_activeEditorTestPlay.hxc_value.hxc_snapshot.hxc_pendingCampaignPortal : NULL;
        hxc_l_gc_roots[242] = hxc_l_activeEditorTestPlay.hxc_has_value ? (const void *)hxc_l_activeEditorTestPlay.hxc_value.hxc_snapshot.hxc_preparedPortal : NULL;
        hxc_l_run_hfdde9b22adea = hxc_l_activeEditorTestPlay;
        if (hxc_optional_9d9d283f_retain(&hxc_l_run_hfdde9b22adea) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_l_gc_roots[243] = hxc_l_run_hfdde9b22adea.hxc_has_value ? (const void *)hxc_l_run_hfdde9b22adea.hxc_value.hxc_runtime : NULL;
        hxc_l_gc_roots[244] = hxc_l_run_hfdde9b22adea.hxc_has_value ? (const void *)hxc_l_run_hfdde9b22adea.hxc_value.hxc_snapshot.hxc_campaignLevel : NULL;
        hxc_l_gc_roots[245] = hxc_l_run_hfdde9b22adea.hxc_has_value ? (const void *)hxc_l_run_hfdde9b22adea.hxc_value.hxc_snapshot.hxc_pendingCampaignLevel : NULL;
        hxc_l_gc_roots[246] = hxc_l_run_hfdde9b22adea.hxc_has_value ? (const void *)hxc_l_run_hfdde9b22adea.hxc_value.hxc_snapshot.hxc_pendingCampaignPortal : NULL;
        hxc_l_gc_roots[247] = hxc_l_run_hfdde9b22adea.hxc_has_value ? (const void *)hxc_l_run_hfdde9b22adea.hxc_value.hxc_snapshot.hxc_preparedPortal : NULL;
        if (hxc_l_run_hfdde9b22adea.hxc_has_value)
        {
          struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun hxc_l_tmp_load_result_n324 = hxc_l_run_hfdde9b22adea;
          hxc_l_gc_roots[255] = hxc_l_tmp_load_result_n324.hxc_has_value ? (const void *)hxc_l_tmp_load_result_n324.hxc_value.hxc_runtime : NULL;
          hxc_l_gc_roots[256] = hxc_l_tmp_load_result_n324.hxc_has_value ? (const void *)hxc_l_tmp_load_result_n324.hxc_value.hxc_snapshot.hxc_campaignLevel : NULL;
          hxc_l_gc_roots[257] = hxc_l_tmp_load_result_n324.hxc_has_value ? (const void *)hxc_l_tmp_load_result_n324.hxc_value.hxc_snapshot.hxc_pendingCampaignLevel : NULL;
          hxc_l_gc_roots[258] = hxc_l_tmp_load_result_n324.hxc_has_value ? (const void *)hxc_l_tmp_load_result_n324.hxc_value.hxc_snapshot.hxc_pendingCampaignPortal : NULL;
          hxc_l_gc_roots[259] = hxc_l_tmp_load_result_n324.hxc_has_value ? (const void *)hxc_l_tmp_load_result_n324.hxc_value.hxc_snapshot.hxc_preparedPortal : NULL;
          if (!hxc_l_tmp_load_result_n324.hxc_has_value)
          {
            abort();
          }
          hxc_l_gc_roots[260] = (const void *)hxc_l_tmp_load_result_n324.hxc_value.hxc_runtime;
          hxc_l_gc_roots[261] = (const void *)hxc_l_tmp_load_result_n324.hxc_value.hxc_snapshot.hxc_campaignLevel;
          hxc_l_gc_roots[262] = (const void *)hxc_l_tmp_load_result_n324.hxc_value.hxc_snapshot.hxc_pendingCampaignLevel;
          hxc_l_gc_roots[263] = (const void *)hxc_l_tmp_load_result_n324.hxc_value.hxc_snapshot.hxc_pendingCampaignPortal;
          hxc_l_gc_roots[264] = (const void *)hxc_l_tmp_load_result_n324.hxc_value.hxc_snapshot.hxc_preparedPortal;
          struct hxc_caxecraft_app_EditorTestPlayRuntime *hxc_l_tmp_optional_record_field_project_n326 = hxc_l_tmp_load_result_n324.hxc_value.hxc_runtime;
          hxc_l_gc_roots[265] = (const void *)hxc_l_tmp_optional_record_field_project_n326;
          if (hxc_l_tmp_optional_record_field_project_n326 == NULL)
          {
            abort();
          }
          struct hxc_caxecraft_app_ActivePlayableLevel *hxc_l_tmp_class_field_load_result_n327 = (*hxc_l_tmp_optional_record_field_project_n326).hxc_active;
          hxc_l_gc_roots[266] = (const void *)hxc_l_tmp_class_field_load_result_n327;
          struct hxc_caxecraft_app_ActivePlayableLevel *hxc_l_testLevel_hf602be188e18 = hxc_l_tmp_class_field_load_result_n327;
          hxc_l_gc_roots[267] = (const void *)hxc_l_testLevel_hf602be188e18;
          hxc_l_gc_roots[268] = (const void *)NULL;
          if (hxc_l_testLevel_hf602be188e18 != NULL)
          {
            hxc_l_gc_roots[269] = (const void *)hxc_l_testLevel_hf602be188e18;
            hxc_l_frameLevelOwner = hxc_l_testLevel_hf602be188e18;
          }
        }
        hxc_optional_9d9d283f_destroy(&hxc_l_run_hfdde9b22adea);
      }
      struct hxc_caxecraft_app_ActivePlayableLevel *hxc_l_tmp_load_result_n330 = hxc_l_frameLevelOwner;
      hxc_l_gc_roots[248] = (const void *)hxc_l_tmp_load_result_n330;
      if (hxc_l_tmp_load_result_n330 == NULL)
      {
        abort();
      }
      struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_class_field_load_result_n331 = (*hxc_l_tmp_load_result_n330).hxc_activeView;
      hxc_l_gc_roots[249] = (const void *)hxc_l_tmp_class_field_load_result_n331;
      struct hxc_caxecraft_app_PlayableLevelView *hxc_l_levelView_h37960af8bef4 = hxc_l_tmp_class_field_load_result_n331;
      struct hxc_caxecraft_app_ActivePlayableLevel *hxc_l_tmp_load_result_n332 = hxc_l_frameLevelOwner;
      hxc_l_gc_roots[250] = (const void *)hxc_l_tmp_load_result_n332;
      if (hxc_l_tmp_load_result_n332 == NULL)
      {
        abort();
      }
      struct hxc_caxecraft_content_ActiveContent *hxc_l_tmp_class_field_load_result_n333 = (*hxc_l_tmp_load_result_n332).hxc_content;
      hxc_l_gc_roots[251] = (const void *)hxc_l_tmp_class_field_load_result_n333;
      if (hxc_l_tmp_class_field_load_result_n333 == NULL)
      {
        abort();
      }
      struct hxc_caxecraft_content_LoadedContentGeneration *hxc_l_tmp_class_field_load_result_n334 = (*hxc_l_tmp_class_field_load_result_n333).hxc_active;
      hxc_l_gc_roots[252] = (const void *)hxc_l_tmp_class_field_load_result_n334;
      if (hxc_l_tmp_class_field_load_result_n334 == NULL)
      {
        abort();
      }
      struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_class_field_load_result_n335 = (*hxc_l_tmp_class_field_load_result_n334).hxc_ownedSession;
      hxc_l_gc_roots[253] = (const void *)hxc_l_tmp_class_field_load_result_n335;
      struct hxc_caxecraft_domain_GameSession *hxc_l_session = hxc_l_tmp_class_field_load_result_n335;
      struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n336 = hxc_l_session;
      hxc_l_gc_roots[254] = (const void *)hxc_l_tmp_load_result_n336;
      if (hxc_l_tmp_load_result_n336 == NULL)
      {
        abort();
      }
      struct hxc_caxecraft_domain_GameView hxc_l_tmp_instance_call_result_n337 = hxc_caxecraft_domain_GameSession_view(hxc_l_tmp_load_result_n336);
      struct hxc_caxecraft_domain_GameView hxc_l_initialView = hxc_l_tmp_instance_call_result_n337;
      if (!hxc_l_initialView.hxc_valid)
      {
        hxc_l_quit = true;
      }
      struct hxc_caxecraft_domain_Character hxc_l_character = hxc_l_initialView.hxc_localPlayer;
      bool hxc_l_recapturedThisFrame = false;
      bool hxc_l_resetMotionThisFrame = false;
      float hxc_l_tmp_native_call_result_n340 = GetFrameTime();
      double hxc_l_frameSeconds = (double)hxc_l_tmp_native_call_result_n340;
      if (hxc_l_frameSeconds > 0.25)
      {
        hxc_l_frameSeconds = 0.25;
      }
      bool hxc_l_tmp_native_call_result_n342 = IsWindowFocused();
      bool hxc_l_focused = hxc_l_tmp_native_call_result_n342;
      bool hxc_l_tmp_call_result_n344 = hxc_caxecraft_app_AppScreen_capturesPointer(hxc_l_screen);
      bool hxc_l_tmp_call_result_n346 = hxc_caxecraft_app_AppScreen_pausesSimulation(hxc_l_screen);
      bool hxc_l_tmp_load_result_n347 = hxc_l_focused;
      struct hxc_caxecraft_app_RaylibGameInputSample hxc_l_tmp_call_result_n349 = hxc_caxecraft_app_RaylibGameInput_sampleGameInput(hxc_l_tmp_call_result_n344, hxc_l_tmp_call_result_n346, hxc_l_tmp_load_result_n347, hxc_l_frameSeconds);
      struct hxc_caxecraft_app_RaylibGameInputSample hxc_l_realInput = hxc_l_tmp_call_result_n349;
      struct hxc_caxecraft_pilot_GameInputFrame hxc_l_frameInput = hxc_l_realInput.hxc_frame;
      int32_t hxc_l_promptDevice = hxc_l_realInput.hxc_promptDevice;
      bool hxc_l_tmp_call_result_n352 = hxc_caxecraft_app_RaylibGameInput_debugHudTogglePressed();
      if (hxc_l_tmp_call_result_n352)
      {
        hxc_l_debugHudVisible = !hxc_l_debugHudVisible;
      }
      double hxc_l_moveForward = hxc_l_frameInput.hxc_moveForward;
      double hxc_l_moveRight = hxc_l_frameInput.hxc_moveRight;
      double hxc_l_lookYaw = hxc_l_frameInput.hxc_lookYaw;
      double hxc_l_lookPitch = hxc_l_frameInput.hxc_lookPitch;
      bool hxc_l_jumpPressed = hxc_l_frameInput.hxc_jumpPressed;
      bool hxc_l_riseHeld = hxc_l_frameInput.hxc_riseHeld;
      bool hxc_l_primaryPressed = hxc_l_frameInput.hxc_primaryPressed;
      bool hxc_l_secondaryPressed = hxc_l_frameInput.hxc_secondaryPressed;
      bool hxc_l_interactPressed = hxc_l_frameInput.hxc_interactPressed;
      bool hxc_l_travelPressed = hxc_l_frameInput.hxc_travelPressed;
      bool hxc_l_pausePressed = hxc_l_frameInput.hxc_pausePressed;
      bool hxc_l_capturePressed = hxc_l_frameInput.hxc_capturePressed;
      bool hxc_l_quitPressed = hxc_l_frameInput.hxc_quitPressed;
      int32_t hxc_l_hotbarSelection = hxc_l_frameInput.hxc_hotbarSelection;
      int32_t hxc_l_hotbarCycle = hxc_l_frameInput.hxc_hotbarCycle;
      bool hxc_l_menuNextPressed = hxc_l_frameInput.hxc_menuNextPressed;
      bool hxc_l_menuConfirmPressed = hxc_l_frameInput.hxc_menuConfirmPressed;
      bool hxc_l_descendHeld = hxc_l_frameInput.hxc_descendHeld;
      bool hxc_l_tmp_call_result_n373 = hxc_caxecraft_app_AppScreen_capturesPointer(hxc_l_screen);
      bool hxc_l_tmp_call_result_n374 = hxc_caxecraft_app_RaylibGameInput_cameraTogglePressed(hxc_l_tmp_call_result_n373);
      bool hxc_l_cameraTogglePressed = hxc_l_tmp_call_result_n374;
      enum hxc_caxecraft_app_AppScreen hxc_l_tmp_load_result_n375 = hxc_l_screen;
      bool hxc_l_tmp_load_result_n376 = hxc_l_focused;
      bool hxc_l_tmp_call_result_n378 = hxc_caxecraft_app_AppScreen_stopsEditorTestPlay(hxc_l_tmp_load_result_n375, hxc_l_tmp_load_result_n376, hxc_l_pausePressed);
      if (!hxc_l_tmp_call_result_n378)
      {
        hxc_l_gc_roots[280] = (const void *)NULL;
        struct hxc_caxecraft_content_CampaignLevel *hxc_l_requestedCampaignLevel = NULL;
        hxc_l_gc_roots[281] = (const void *)NULL;
        struct hxc_caxecraft_app_PreparedCampaignPortal *hxc_l_requestedCampaignPortal = NULL;
        if (hxc_l_screen == hxc_caxecraft_app_AppScreen_Playing)
        {
          hxc_l_gc_roots[317] = (const void *)hxc_l_pendingCampaignPortal;
          hxc_l_requestedCampaignPortal = hxc_l_pendingCampaignPortal;
          hxc_l_gc_roots[318] = (const void *)NULL;
          hxc_l_pendingCampaignPortal = NULL;
        }
        bool hxc_l_tmp_load_result_n441 = hxc_l_loadingAtFrameStart;
        bool hxc_l_tmp_short_circuit_result_n191 = hxc_l_tmp_load_result_n441;
        if (hxc_l_tmp_load_result_n441)
        {
          hxc_l_tmp_short_circuit_result_n191 = hxc_l_loadingFramePresented;
        }
        if (hxc_l_tmp_short_circuit_result_n191)
        {
          hxc_l_gc_roots[319] = (const void *)hxc_l_pendingCampaignLevel;
          hxc_l_requestedCampaignLevel = hxc_l_pendingCampaignLevel;
        }
        bool hxc_l_requestedFromCampaignMenu = false;
        struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_load_result_n445 = hxc_l_requestedCampaignLevel;
        hxc_l_gc_roots[320] = (const void *)hxc_l_tmp_load_result_n445;
        hxc_l_gc_roots[321] = (const void *)NULL;
        bool hxc_l_tmp_short_circuit_result_n194 = hxc_l_tmp_load_result_n445 != NULL;
        if (hxc_l_tmp_load_result_n445 != NULL)
        {
          hxc_l_tmp_short_circuit_result_n194 = hxc_l_loadingAtFrameStart;
        }
        bool hxc_l_requestedFromAuthoredExit = hxc_l_tmp_short_circuit_result_n194;
        bool hxc_l_tmp_load_result_n448 = hxc_l_quit;
        bool hxc_l_tmp_short_circuit_result_n195 = !hxc_l_tmp_load_result_n448;
        if (!hxc_l_tmp_load_result_n448)
        {
          enum hxc_caxecraft_app_AppScreen hxc_l_tmp_load_result_n449 = hxc_l_screen;
          bool hxc_l_tmp_call_result_n451 = hxc_caxecraft_app_TitleMenuFlow_allowsCampaignTravel(hxc_l_tmp_load_result_n449, hxc_l_selectedMode);
          hxc_l_tmp_short_circuit_result_n195 = hxc_l_tmp_call_result_n451;
        }
        bool hxc_l_tmp_short_circuit_load_result_n452 = hxc_l_tmp_short_circuit_result_n195;
        bool hxc_l_tmp_short_circuit_result_n196 = hxc_l_tmp_short_circuit_load_result_n452;
        if (hxc_l_tmp_short_circuit_load_result_n452)
        {
          hxc_l_tmp_short_circuit_result_n196 = hxc_l_travelPressed;
        }
        if (hxc_l_tmp_short_circuit_result_n196)
        {
          hxc_l_gc_roots[322] = (const void *)hxc_l_campaign;
          struct hxc_caxecraft_content_CampaignManifest *hxc_l_selectedCampaign_h056902ecea1f = hxc_l_campaign;
          hxc_l_gc_roots[323] = (const void *)hxc_l_campaignLevel;
          struct hxc_caxecraft_content_CampaignLevel *hxc_l_sourceLevel_hbecb7b0acfef = hxc_l_campaignLevel;
          struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n457 = hxc_l_selectedCampaign_h056902ecea1f;
          hxc_l_gc_roots[324] = (const void *)hxc_l_tmp_load_result_n457;
          hxc_l_gc_roots[325] = (const void *)NULL;
          bool hxc_l_tmp_short_circuit_result_n199 = hxc_l_tmp_load_result_n457 != NULL;
          if (hxc_l_tmp_load_result_n457 != NULL)
          {
            hxc_l_gc_roots[326] = (const void *)hxc_l_sourceLevel_hbecb7b0acfef;
            hxc_l_gc_roots[327] = (const void *)NULL;
            hxc_l_tmp_short_circuit_result_n199 = hxc_l_sourceLevel_hbecb7b0acfef != NULL;
          }
          if (hxc_l_tmp_short_circuit_result_n199)
          {
            struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n460 = hxc_l_selectedCampaign_h056902ecea1f;
            hxc_l_gc_roots[328] = (const void *)hxc_l_tmp_load_result_n460;
            if (hxc_l_tmp_load_result_n460 == NULL)
            {
              abort();
            }
            struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_load_result_n461 = hxc_l_sourceLevel_hbecb7b0acfef;
            hxc_l_gc_roots[329] = (const void *)hxc_l_tmp_load_result_n461;
            if (hxc_l_tmp_load_result_n461 == NULL)
            {
              abort();
            }
            hxc_string hxc_l_tmp_class_field_load_result_n462 = (*hxc_l_tmp_load_result_n461).hxc_id;
            struct hxc_caxecraft_content_CampaignTransition *hxc_l_tmp_instance_call_result_n463 = hxc_caxecraft_content_CampaignManifest_unambiguousTransitionFrom(hxc_l_tmp_load_result_n460, hxc_l_tmp_class_field_load_result_n462);
            hxc_l_gc_roots[330] = (const void *)hxc_l_tmp_instance_call_result_n463;
            struct hxc_caxecraft_content_CampaignTransition *hxc_l_transition_h2d4c4c8573fd = hxc_l_tmp_instance_call_result_n463;
            hxc_l_gc_roots[331] = (const void *)hxc_l_transition_h2d4c4c8573fd;
            hxc_l_gc_roots[332] = (const void *)NULL;
            if (hxc_l_transition_h2d4c4c8573fd == NULL)
            {
              struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_load_result_n465 = hxc_l_sourceLevel_hbecb7b0acfef;
              hxc_l_gc_roots[333] = (const void *)hxc_l_tmp_load_result_n465;
              if (hxc_l_tmp_load_result_n465 == NULL)
              {
                abort();
              }
              hxc_string hxc_l_tmp_class_field_load_result_n466 = (*hxc_l_tmp_load_result_n465).hxc_id;
              hxc_string hxc_l_tmp_string_concat_result_n467 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
              if (hxc_string_concat_ref((hxc_string){ (const uint8_t *)"caxecraft: campaign travel requires exactly one outgoing transition from ", 73, true, NULL }, hxc_l_tmp_class_field_load_result_n466, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n467) != HXC_STATUS_OK)
              {
                abort();
              }
              hxc_l_tmp_sys_println_string_argument_owner_n201 = hxc_l_tmp_string_concat_result_n467;
              if (hxc_io_println(hxc_l_tmp_sys_println_string_argument_owner_n201) != HXC_STATUS_OK)
              {
                if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n201) != HXC_STATUS_OK)
                {
                  abort();
                }
                if (hxc_array_ref_release(hxc_l_initialActorPhases) != HXC_STATUS_OK)
                {
                  abort();
                }
                hxc_enum_2c0e695d_destroy(&hxc_l_tmp_static_call_argument_0_owner_n102);
                if (hxc_string_release(&hxc_l_currentObjectiveId) != HXC_STATUS_OK)
                {
                  abort();
                }
                if (hxc_string_release(&hxc_l_latestJournalId) != HXC_STATUS_OK)
                {
                  abort();
                }
                if (hxc_string_release(&hxc_l_activeDialogue) != HXC_STATUS_OK)
                {
                  abort();
                }
                if (hxc_array_ref_release(hxc_l_tmp_object_storage_n92.hxc_stateStartTicks) != HXC_STATUS_OK)
                {
                  abort();
                }
                if (hxc_array_ref_release(hxc_l_tmp_object_storage_n92.hxc_stateIds) != HXC_STATUS_OK)
                {
                  abort();
                }
                if (hxc_array_ref_release(hxc_l_tmp_object_storage_n92.hxc_objectIds) != HXC_STATUS_OK)
                {
                  abort();
                }
                hxc_record_f6127605_destroy(&hxc_l_receipt);
                if (hxc_string_release(&hxc_l_levelLabel) != HXC_STATUS_OK)
                {
                  abort();
                }
                hxc_enum_4b68fff2_destroy(&hxc_l_g_hccea2de18028);
                if (hxc_string_release(&hxc_l_pendingCampaignLabel) != HXC_STATUS_OK)
                {
                  abort();
                }
                hxc_optional_9d9d283f_destroy(&hxc_l_activeEditorTestPlay);
                hxc_enum_11853fd1_destroy(&hxc_l_g_h1d08383de4f8);
                hxc_record_f6127605_destroy(&hxc_l_tmp_record_field_logicalPath_receiver_owner_n23);
                hxc_enum_777c0102_destroy(&hxc_l_g_h6904c3060e30);
                hxc_record_8aab5967_destroy(&hxc_l_tmp_managed_flow_owner_n17);
                hxc_enum_d4806d01_destroy(&hxc_l_g_h1d9290112915);
                hxc_enum_fcb4401b_destroy(&hxc_l_g_h14daa0fd57c5);
                if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
                {
                  abort();
                }
                abort();
              }
              if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n201) != HXC_STATUS_OK)
              {
                abort();
              }
            }
            else
            {
              struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n469 = hxc_l_selectedCampaign_h056902ecea1f;
              hxc_l_gc_roots[334] = (const void *)hxc_l_tmp_load_result_n469;
              if (hxc_l_tmp_load_result_n469 == NULL)
              {
                abort();
              }
              struct hxc_caxecraft_content_CampaignTransition *hxc_l_tmp_load_result_n470 = hxc_l_transition_h2d4c4c8573fd;
              hxc_l_gc_roots[335] = (const void *)hxc_l_tmp_load_result_n470;
              if (hxc_l_tmp_load_result_n470 == NULL)
              {
                abort();
              }
              hxc_string hxc_l_tmp_class_field_load_result_n471 = (*hxc_l_tmp_load_result_n470).hxc_destinationLevel;
              struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_instance_call_result_n472 = hxc_caxecraft_content_CampaignManifest_level(hxc_l_tmp_load_result_n469, hxc_l_tmp_class_field_load_result_n471);
              hxc_l_gc_roots[336] = (const void *)hxc_l_tmp_instance_call_result_n472;
              struct hxc_caxecraft_content_CampaignLevel *hxc_l_destination_hb6d279dc093a = hxc_l_tmp_instance_call_result_n472;
              hxc_l_gc_roots[337] = (const void *)hxc_l_destination_hb6d279dc093a;
              hxc_l_gc_roots[338] = (const void *)NULL;
              if (hxc_l_destination_hb6d279dc093a == NULL)
              {
                if (hxc_io_println((hxc_string){ (const uint8_t *)"caxecraft: campaign destination disappeared after manifest validation", 69, true, NULL }) != HXC_STATUS_OK)
                {
                  abort();
                }
              }
              else
              {
                hxc_l_gc_roots[339] = (const void *)hxc_l_destination_hb6d279dc093a;
                hxc_l_requestedCampaignLevel = hxc_l_destination_hb6d279dc093a;
              }
            }
          }
        }
        bool hxc_l_tmp_call_result_n476 = hxc_caxecraft_app_AppScreen_showsCampaignSelection(hxc_l_screen);
        bool hxc_l_tmp_short_circuit_result_n203 = hxc_l_tmp_call_result_n476;
        if (hxc_l_tmp_call_result_n476)
        {
          hxc_l_tmp_short_circuit_result_n203 = hxc_l_focused;
        }
        if (hxc_l_tmp_short_circuit_result_n203)
        {
          bool hxc_l_tmp_native_call_result_n479 = IsKeyPressed((int32_t)76);
          if (hxc_l_tmp_native_call_result_n479)
          {
            struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_tmp_load_result_n480 = hxc_l_uiCatalog;
            hxc_l_gc_roots[340] = (const void *)hxc_l_tmp_load_result_n480;
            if (hxc_l_tmp_load_result_n480 == NULL)
            {
              abort();
            }
            int32_t hxc_l_tmp_instance_call_result_n482 = hxc_caxecraft_localization_RuntimeUiCatalog_nextLocale(hxc_l_tmp_load_result_n480, hxc_l_locale);
            hxc_l_locale = hxc_l_tmp_instance_call_result_n482;
          }
          struct Vector2 hxc_l_tmp_native_call_result_n483 = GetMousePosition();
          struct Vector2 hxc_l_campaignMouse = hxc_l_tmp_native_call_result_n483;
          double hxc_l_tmp_static_call_argument_0_n206 = (double)hxc_l_campaignMouse.x;
          double hxc_l_tmp_static_call_argument_1_n207 = (double)hxc_l_campaignMouse.y;
          int32_t hxc_l_tmp_native_call_result_n488 = GetScreenWidth();
          int32_t hxc_l_tmp_static_call_argument_2_n208 = (int32_t)hxc_l_tmp_native_call_result_n488;
          int32_t hxc_l_tmp_native_call_result_n489 = GetScreenHeight();
          int32_t hxc_l_tmp_static_call_argument_3_n209 = (int32_t)hxc_l_tmp_native_call_result_n489;
          struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n490 = hxc_l_campaign;
          hxc_l_gc_roots[341] = (const void *)hxc_l_tmp_load_result_n490;
          hxc_l_gc_roots[342] = (const void *)NULL;
          int32_t hxc_l_tmp_conditional_result_n210 = 0;
          if (hxc_l_tmp_load_result_n490 == NULL)
          {
            hxc_l_tmp_conditional_result_n210 = 0;
          }
          else
          {
            struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n491 = hxc_l_campaign;
            hxc_l_gc_roots[343] = (const void *)hxc_l_tmp_load_result_n491;
            if (hxc_l_tmp_load_result_n491 == NULL)
            {
              abort();
            }
            struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n492 = (*hxc_l_tmp_load_result_n491).hxc_admittedLevels;
            hxc_l_gc_roots[344] = (const void *)hxc_l_tmp_class_field_load_result_n492;
            int32_t hxc_l_tmp_array_length_result_n493;
            if (hxc_array_ref_length(hxc_l_tmp_class_field_load_result_n492, &hxc_l_tmp_array_length_result_n493) != HXC_STATUS_OK)
            {
              abort();
            }
            hxc_l_tmp_conditional_result_n210 = hxc_l_tmp_array_length_result_n493;
          }
          int32_t hxc_l_tmp_conditional_load_result_n494 = hxc_l_tmp_conditional_result_n210;
          int32_t hxc_l_tmp_load_result_n495 = hxc_l_selectedCampaignLevelIndex;
          double hxc_l_tmp_static_call_argument_0_load_result_n496 = hxc_l_tmp_static_call_argument_0_n206;
          double hxc_l_tmp_static_call_argument_1_load_result_n497 = hxc_l_tmp_static_call_argument_1_n207;
          int32_t hxc_l_tmp_static_call_argument_2_load_result_n498 = hxc_l_tmp_static_call_argument_2_n208;
          struct hxc_caxecraft_app_CampaignMenuHit hxc_l_tmp_call_result_n500 = hxc_caxecraft_app_CampaignMenu_selectionAt(hxc_l_tmp_static_call_argument_0_load_result_n496, hxc_l_tmp_static_call_argument_1_load_result_n497, hxc_l_tmp_static_call_argument_2_load_result_n498, hxc_l_tmp_static_call_argument_3_n209, hxc_l_tmp_conditional_load_result_n494, hxc_l_tmp_load_result_n495);
          struct hxc_caxecraft_app_CampaignMenuHit hxc_l_campaignChoice = hxc_l_tmp_call_result_n500;
          bool hxc_l_tmp_native_call_result_n501 = IsMouseButtonPressed((int32_t)0);
          bool hxc_l_campaignPointerPressed = hxc_l_tmp_native_call_result_n501;
          int32_t hxc_l_tmp_record_field_selectedLevelIndex_n213 = hxc_l_selectedCampaignLevelIndex;
          struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n503 = hxc_l_campaign;
          hxc_l_gc_roots[345] = (const void *)hxc_l_tmp_load_result_n503;
          hxc_l_gc_roots[346] = (const void *)NULL;
          int32_t hxc_l_tmp_conditional_result_n214 = 0;
          if (hxc_l_tmp_load_result_n503 == NULL)
          {
            hxc_l_tmp_conditional_result_n214 = 0;
          }
          else
          {
            struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n504 = hxc_l_campaign;
            hxc_l_gc_roots[347] = (const void *)hxc_l_tmp_load_result_n504;
            if (hxc_l_tmp_load_result_n504 == NULL)
            {
              abort();
            }
            struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n505 = (*hxc_l_tmp_load_result_n504).hxc_admittedLevels;
            hxc_l_gc_roots[348] = (const void *)hxc_l_tmp_class_field_load_result_n505;
            int32_t hxc_l_tmp_array_length_result_n506;
            if (hxc_array_ref_length(hxc_l_tmp_class_field_load_result_n505, &hxc_l_tmp_array_length_result_n506) != HXC_STATUS_OK)
            {
              abort();
            }
            hxc_l_tmp_conditional_result_n214 = hxc_l_tmp_array_length_result_n506;
          }
          int32_t hxc_l_tmp_conditional_load_result_n507 = hxc_l_tmp_conditional_result_n214;
          bool hxc_l_tmp_load_result_n508 = hxc_l_menuNextPressed;
          bool hxc_l_tmp_load_result_n509 = hxc_l_menuConfirmPressed;
          bool hxc_l_tmp_load_result_n510 = hxc_l_pausePressed;
          struct hxc_caxecraft_app_CampaignMenuHit hxc_l_tmp_load_result_n511 = hxc_l_campaignChoice;
          bool hxc_l_tmp_load_result_n512 = hxc_l_campaignPointerPressed;
          struct hxc_caxecraft_app_CampaignMenuFrameOutcome hxc_l_tmp_call_result_n515 = hxc_caxecraft_app_CampaignMenuFlow_advanceCampaignMenuFrame((struct hxc_caxecraft_app_CampaignMenuFrameInput){ .hxc_cancelSelection = hxc_l_tmp_load_result_n510, .hxc_confirmSelection = hxc_l_tmp_load_result_n509, .hxc_cycleSelection = hxc_l_tmp_load_result_n508, .hxc_levelCount = hxc_l_tmp_conditional_load_result_n507, .hxc_pointerHit = hxc_l_tmp_load_result_n511, .hxc_pointerPressed = hxc_l_tmp_load_result_n512, .hxc_selectedLevelIndex = hxc_l_tmp_record_field_selectedLevelIndex_n213, .hxc_selectionEnabled = true });
          struct hxc_caxecraft_app_CampaignMenuFrameOutcome hxc_l_campaignFrame = hxc_l_tmp_call_result_n515;
          hxc_l_selectedCampaignLevelIndex = hxc_l_campaignFrame.hxc_selectedLevelIndex;
          if (hxc_l_campaignFrame.hxc_backRequested)
          {
            enum hxc_caxecraft_app_AppScreen hxc_l_tmp_call_result_n519 = hxc_caxecraft_app_AppScreen_closeCampaignSelection(hxc_l_screen);
            hxc_l_screen = hxc_l_tmp_call_result_n519;
            hxc_l_accumulator = 0.0;
            hxc_l_resetMotionThisFrame = true;
          }
          else
          {
            struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n520 = hxc_l_campaign;
            hxc_l_gc_roots[349] = (const void *)hxc_l_tmp_load_result_n520;
            hxc_l_gc_roots[350] = (const void *)NULL;
            bool hxc_l_tmp_short_circuit_result_n215 = hxc_l_tmp_load_result_n520 != NULL;
            if (hxc_l_tmp_load_result_n520 != NULL)
            {
              hxc_l_tmp_short_circuit_result_n215 = hxc_l_campaignFrame.hxc_launchRequested;
            }
            if (hxc_l_tmp_short_circuit_result_n215)
            {
              struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n523 = hxc_l_campaign;
              hxc_l_gc_roots[351] = (const void *)hxc_l_tmp_load_result_n523;
              if (hxc_l_tmp_load_result_n523 == NULL)
              {
                abort();
              }
              struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n524 = (*hxc_l_tmp_load_result_n523).hxc_admittedLevels;
              hxc_l_gc_roots[352] = (const void *)hxc_l_tmp_class_field_load_result_n524;
              struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_array_get_result_n526;
              if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n524, (size_t)hxc_l_selectedCampaignLevelIndex, &hxc_l_tmp_array_get_result_n526) != HXC_STATUS_OK)
              {
                abort();
              }
              hxc_l_gc_roots[353] = (const void *)hxc_l_tmp_array_get_result_n526;
              hxc_l_requestedCampaignLevel = hxc_l_tmp_array_get_result_n526;
              hxc_l_requestedFromCampaignMenu = true;
            }
          }
        }
        bool hxc_l_tmp_call_result_n528 = hxc_caxecraft_app_AppScreen_showsTitle(hxc_l_screen);
        bool hxc_l_tmp_short_circuit_result_n216 = hxc_l_tmp_call_result_n528;
        if (hxc_l_tmp_call_result_n528)
        {
          hxc_l_tmp_short_circuit_result_n216 = hxc_l_focused;
        }
        if (hxc_l_tmp_short_circuit_result_n216)
        {
          enum hxc_caxecraft_app_AppScreen hxc_l_screenBeforeMenu = hxc_l_screen;
          int32_t hxc_l_modeBeforeMenu = hxc_l_selectedMode;
          bool hxc_l_tmp_native_call_result_n533 = IsKeyPressed((int32_t)76);
          if (hxc_l_tmp_native_call_result_n533)
          {
            struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_tmp_load_result_n534 = hxc_l_uiCatalog;
            hxc_l_gc_roots[357] = (const void *)hxc_l_tmp_load_result_n534;
            if (hxc_l_tmp_load_result_n534 == NULL)
            {
              abort();
            }
            int32_t hxc_l_tmp_instance_call_result_n536 = hxc_caxecraft_localization_RuntimeUiCatalog_nextLocale(hxc_l_tmp_load_result_n534, hxc_l_locale);
            hxc_l_locale = hxc_l_tmp_instance_call_result_n536;
          }
          struct Vector2 hxc_l_tmp_native_call_result_n537 = GetMousePosition();
          struct Vector2 hxc_l_menuMouse = hxc_l_tmp_native_call_result_n537;
          struct Vector2 hxc_l_tmp_load_result_n538 = hxc_l_menuMouse;
          struct Vector2 hxc_l_tmp_load_result_n540 = hxc_l_menuMouse;
          int32_t hxc_l_tmp_native_call_result_n542 = GetScreenWidth();
          int32_t hxc_l_tmp_native_call_result_n543 = GetScreenHeight();
          int32_t hxc_l_tmp_call_result_n544 = hxc_caxecraft_app_TitleMenu_selectionAt((double)hxc_l_tmp_load_result_n538.x, (double)hxc_l_tmp_load_result_n540.y, (int32_t)hxc_l_tmp_native_call_result_n542, (int32_t)hxc_l_tmp_native_call_result_n543);
          int32_t hxc_l_hovered = hxc_l_tmp_call_result_n544;
          int32_t hxc_l_tmp_load_result_n545 = hxc_l_hovered;
          bool hxc_l_tmp_short_circuit_result_n222 = hxc_l_tmp_load_result_n545 >= 0;
          if (hxc_l_tmp_load_result_n545 >= 0)
          {
            bool hxc_l_tmp_native_call_result_n546 = IsMouseButtonPressed((int32_t)0);
            hxc_l_tmp_short_circuit_result_n222 = hxc_l_tmp_native_call_result_n546;
          }
          bool hxc_l_pointerPressed = hxc_l_tmp_short_circuit_result_n222;
          enum hxc_caxecraft_app_AppScreen hxc_l_tmp_load_result_n548 = hxc_l_screen;
          struct hxc_caxecraft_app_TitleMenuState hxc_l_tmp_call_result_n550 = hxc_caxecraft_app_TitleMenuFlow_titleMenuState(hxc_l_tmp_load_result_n548, hxc_l_selectedMode);
          struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n551 = hxc_l_campaign;
          hxc_l_gc_roots[358] = (const void *)hxc_l_tmp_load_result_n551;
          hxc_l_gc_roots[359] = (const void *)NULL;
          bool hxc_l_tmp_load_result_n552 = hxc_l_menuNextPressed;
          bool hxc_l_tmp_load_result_n553 = hxc_l_menuConfirmPressed;
          int32_t hxc_l_tmp_load_result_n554 = hxc_l_hovered;
          struct hxc_caxecraft_app_TitleMenuState hxc_l_tmp_call_result_n557 = hxc_caxecraft_app_TitleMenuFlow_advanceTitleMenuFrame((struct hxc_caxecraft_app_TitleMenuFrameInput){ .hxc_campaignAvailable = hxc_l_tmp_load_result_n551 != NULL, .hxc_confirmMode = hxc_l_tmp_load_result_n553, .hxc_cycleMode = hxc_l_tmp_load_result_n552, .hxc_pointerChoice = hxc_l_tmp_load_result_n554, .hxc_pointerPressed = hxc_l_pointerPressed, .hxc_state = hxc_l_tmp_call_result_n550 });
          struct hxc_caxecraft_app_TitleMenuState hxc_l_menuState = hxc_l_tmp_call_result_n557;
          hxc_l_screen = hxc_l_menuState.hxc_screen;
          hxc_l_selectedMode = hxc_l_menuState.hxc_mode;
          enum hxc_caxecraft_app_AppScreen hxc_l_tmp_load_result_n560 = hxc_l_screen;
          enum hxc_caxecraft_app_AppScreen hxc_l_tmp_load_result_n561 = hxc_l_screenBeforeMenu;
          bool hxc_l_tmp_short_circuit_result_n224 = hxc_l_tmp_load_result_n560 != hxc_l_tmp_load_result_n561;
          if (!(hxc_l_tmp_load_result_n560 != hxc_l_tmp_load_result_n561))
          {
            int32_t hxc_l_tmp_load_result_n562 = hxc_l_selectedMode;
            hxc_l_tmp_short_circuit_result_n224 = hxc_l_tmp_load_result_n562 != hxc_l_modeBeforeMenu;
          }
          if (hxc_l_tmp_short_circuit_result_n224)
          {
            hxc_l_accumulator = 0.0;
            hxc_l_resetMotionThisFrame = true;
          }
          enum hxc_caxecraft_app_AppScreen hxc_l_tmp_load_result_n565 = hxc_l_screenBeforeMenu;
          bool hxc_l_tmp_short_circuit_result_n225 = hxc_l_tmp_load_result_n565 == hxc_caxecraft_app_AppScreen_Title;
          if (hxc_l_tmp_load_result_n565 == hxc_caxecraft_app_AppScreen_Title)
          {
            bool hxc_l_tmp_call_result_n568 = hxc_caxecraft_app_AppScreen_isPlaying(hxc_l_screen);
            hxc_l_tmp_short_circuit_result_n225 = hxc_l_tmp_call_result_n568;
          }
          if (hxc_l_tmp_short_circuit_result_n225)
          {
            hxc_l_recapturedThisFrame = true;
            DisableCursor();
          }
          else
          {
            bool hxc_l_tmp_call_result_n571 = hxc_caxecraft_app_AppScreen_showsEditor(hxc_l_screen);
            if (hxc_l_tmp_call_result_n571)
            {
              EnableCursor();
            }
          }
        }
        hxc_l_gc_roots[354] = (const void *)hxc_l_requestedCampaignLevel;
        struct hxc_caxecraft_content_CampaignLevel *hxc_l_destination_h0d1fdb1e5aab = hxc_l_requestedCampaignLevel;
        hxc_l_gc_roots[355] = (const void *)hxc_l_requestedCampaignPortal;
        hxc_l_gc_roots[356] = (const void *)NULL;
        if (hxc_l_requestedCampaignPortal != NULL)
        {
          struct hxc_caxecraft_app_PreparedCampaignPortal *hxc_l_tmp_load_result_n574 = hxc_l_requestedCampaignPortal;
          hxc_l_gc_roots[360] = (const void *)hxc_l_tmp_load_result_n574;
          if (hxc_l_tmp_load_result_n574 == NULL)
          {
            abort();
          }
          struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_class_field_load_result_n575 = (*hxc_l_tmp_load_result_n574).hxc_destinationValue;
          hxc_l_gc_roots[361] = (const void *)hxc_l_tmp_class_field_load_result_n575;
          hxc_l_destination_h0d1fdb1e5aab = hxc_l_tmp_class_field_load_result_n575;
        }
        hxc_l_gc_roots[362] = (const void *)hxc_l_destination_h0d1fdb1e5aab;
        hxc_l_gc_roots[363] = (const void *)NULL;
        if (hxc_l_destination_h0d1fdb1e5aab != NULL)
        {
          int32_t hxc_l_tmp_record_field_load_result_n577 = hxc_l_character.hxc_id;
          int32_t hxc_l_tmp_record_field_load_result_n578 = hxc_l_character.hxc_vitals.hxc_health;
          struct hxc_caxecraft_content_LevelPlayerOptions hxc_l_playerOptions = (struct hxc_caxecraft_content_LevelPlayerOptions){ .hxc_aquaticProfile = hxc_l_character.hxc_aquaticProfile, .hxc_entityId = hxc_l_tmp_record_field_load_result_n577, .hxc_initialHealth = hxc_l_tmp_record_field_load_result_n578 };
          struct hxc_caxecraft_content_ContentGenerationSequence *hxc_l_tmp_load_result_n581 = hxc_l_generationSequence;
          hxc_l_gc_roots[364] = (const void *)hxc_l_tmp_load_result_n581;
          if (hxc_l_tmp_load_result_n581 == NULL)
          {
            abort();
          }
          int32_t hxc_l_tmp_instance_call_result_n582 = hxc_caxecraft_content_ContentGenerationSequence_allocate(hxc_l_tmp_load_result_n581);
          int32_t hxc_l_nextGeneration = hxc_l_tmp_instance_call_result_n582;
          hxc_l_gc_roots[365] = (const void *)NULL;
          struct hxc_caxecraft_app_StagedPlayableLevel *hxc_l_stagedDestination = NULL;
          hxc_l_gc_roots[366] = (const void *)hxc_l_requestedCampaignPortal;
          hxc_l_gc_roots[367] = (const void *)NULL;
          if (hxc_l_requestedCampaignPortal != NULL)
          {
            struct hxc_caxecraft_app_PreparedCampaignPortal *hxc_l_tmp_load_result_n584 = hxc_l_requestedCampaignPortal;
            hxc_l_gc_roots[370] = (const void *)hxc_l_tmp_load_result_n584;
            if (hxc_l_tmp_load_result_n584 == NULL)
            {
              abort();
            }
            struct hxc_caxecraft_app_StagedPlayableLevel *hxc_l_tmp_class_field_load_result_n585 = (*hxc_l_tmp_load_result_n584).hxc_stagedValue;
            hxc_l_gc_roots[371] = (const void *)hxc_l_tmp_class_field_load_result_n585;
            hxc_l_stagedDestination = hxc_l_tmp_class_field_load_result_n585;
          }
          hxc_l_gc_roots[372] = (const void *)hxc_l_stagedDestination;
          hxc_l_gc_roots[373] = (const void *)NULL;
          if (hxc_l_stagedDestination == NULL)
          {
            struct hxc_caxecraft_content_ContentPackageStore *hxc_l_tmp_load_result_n587 = hxc_l_contentStore;
            hxc_l_gc_roots[374] = (const void *)hxc_l_tmp_load_result_n587;
            struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_load_result_n588 = hxc_l_destination_h0d1fdb1e5aab;
            hxc_l_gc_roots[375] = (const void *)hxc_l_tmp_load_result_n588;
            int32_t hxc_l_tmp_load_result_n589 = hxc_l_nextGeneration;
            struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_tmp_load_result_n590 = hxc_l_contentRegistry;
            hxc_l_gc_roots[376] = (const void *)hxc_l_tmp_load_result_n590;
            if (hxc_l_tmp_load_result_n590 == NULL)
            {
              abort();
            }
            struct hxc_compiler_interface_dispatch_caxecraft_scenario_ScenarioContentRegistry_value hxc_l_tmp_interface_value_n591 = (struct hxc_compiler_interface_dispatch_caxecraft_scenario_ScenarioContentRegistry_value){ .object = hxc_l_tmp_load_result_n590, .table = &hxc_itable_compiler_interface_dispatch_caxecraft_content_RuntimeContentRegistry_itable_layout_caxecraf_hb3f384641e1a61bf };
            struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_tmp_load_result_n592 = hxc_l_contentRegistry;
            hxc_l_gc_roots[377] = (const void *)hxc_l_tmp_load_result_n592;
            if (hxc_l_tmp_load_result_n592 == NULL)
            {
              abort();
            }
            struct hxc_compiler_interface_dispatch_caxecraft_content_LevelContentResolver_value hxc_l_tmp_interface_value_n593 = (struct hxc_compiler_interface_dispatch_caxecraft_content_LevelContentResolver_value){ .object = hxc_l_tmp_load_result_n592, .table = &hxc_itable_compiler_interface_dispatch_caxecraft_content_RuntimeContentRegistry_itable_layout_caxecraf_h82ca4f24ca7a7b5a };
            struct hxc_caxecraft_content_CampaignLevelLoadResult hxc_l_tmp_call_result_n595 = hxc_caxecraft_content_CampaignRuntime_loadCampaignLevel(hxc_l_tmp_load_result_n587, hxc_l_tmp_load_result_n588, hxc_l_tmp_load_result_n589, hxc_l_tmp_interface_value_n591, hxc_l_tmp_interface_value_n593, hxc_l_playerOptions);
            hxc_l_gc_roots[378] = hxc_l_tmp_call_result_n595.hxc_tag == hxc_caxecraft_content_CampaignLevelLoadResult_CampaignLevelReady ? (const void *)hxc_l_tmp_call_result_n595.hxc_payload.hxc_CampaignLevelReady.hxc_candidate : NULL;
            hxc_l_g_haf30e22646ec = hxc_l_tmp_call_result_n595;
            hxc_l_gc_roots[379] = hxc_l_g_haf30e22646ec.hxc_tag == hxc_caxecraft_content_CampaignLevelLoadResult_CampaignLevelReady ? (const void *)hxc_l_g_haf30e22646ec.hxc_payload.hxc_CampaignLevelReady.hxc_candidate : NULL;
            switch (hxc_l_g_haf30e22646ec.hxc_tag) {
              case hxc_caxecraft_content_CampaignLevelLoadResult_CampaignLevelReady:
                {
                  hxc_l_gc_roots[383] = hxc_l_g_haf30e22646ec.hxc_tag == hxc_caxecraft_content_CampaignLevelLoadResult_CampaignLevelReady ? (const void *)hxc_l_g_haf30e22646ec.hxc_payload.hxc_CampaignLevelReady.hxc_candidate : NULL;
                  if (hxc_l_g_haf30e22646ec.hxc_tag != hxc_caxecraft_content_CampaignLevelLoadResult_CampaignLevelReady)
                  {
                    abort();
                  }
                  struct hxc_caxecraft_content_RuntimeLevelCandidate *hxc_l_tmp_enum_payload_project_n598 = hxc_l_g_haf30e22646ec.hxc_payload.hxc_CampaignLevelReady.hxc_candidate;
                  hxc_l_gc_roots[384] = (const void *)hxc_l_tmp_enum_payload_project_n598;
                  struct hxc_caxecraft_content_RuntimeLevelCandidate *hxc_l_candidate_h08e97f469efd = hxc_l_tmp_enum_payload_project_n598;
                  hxc_l_gc_roots[385] = (const void *)hxc_l_candidate_h08e97f469efd;
                  struct hxc_caxecraft_app_PlayableLevelStageResult hxc_l_tmp_call_result_n600 = hxc_caxecraft_app_ActivePlayableLevel_stage(hxc_l_candidate_h08e97f469efd);
                  hxc_l_gc_roots[386] = hxc_l_tmp_call_result_n600.hxc_tag == hxc_caxecraft_app_PlayableLevelStageResult_PlayableLevelStaged ? (const void *)hxc_l_tmp_call_result_n600.hxc_payload.hxc_PlayableLevelStaged.hxc_stage : NULL;
                  struct hxc_caxecraft_app_PlayableLevelStageResult hxc_l_g_haf7b7ab9367d = hxc_l_tmp_call_result_n600;
                  hxc_l_gc_roots[387] = hxc_l_g_haf7b7ab9367d.hxc_tag == hxc_caxecraft_app_PlayableLevelStageResult_PlayableLevelStaged ? (const void *)hxc_l_g_haf7b7ab9367d.hxc_payload.hxc_PlayableLevelStaged.hxc_stage : NULL;
                  switch (hxc_l_g_haf7b7ab9367d.hxc_tag) {
                    case hxc_caxecraft_app_PlayableLevelStageResult_PlayableLevelStaged:
                      {
                        hxc_l_gc_roots[389] = hxc_l_g_haf7b7ab9367d.hxc_tag == hxc_caxecraft_app_PlayableLevelStageResult_PlayableLevelStaged ? (const void *)hxc_l_g_haf7b7ab9367d.hxc_payload.hxc_PlayableLevelStaged.hxc_stage : NULL;
                        if (hxc_l_g_haf7b7ab9367d.hxc_tag != hxc_caxecraft_app_PlayableLevelStageResult_PlayableLevelStaged)
                        {
                          abort();
                        }
                        struct hxc_caxecraft_app_StagedPlayableLevel *hxc_l_tmp_enum_payload_project_n603 = hxc_l_g_haf7b7ab9367d.hxc_payload.hxc_PlayableLevelStaged.hxc_stage;
                        hxc_l_gc_roots[390] = (const void *)hxc_l_tmp_enum_payload_project_n603;
                        struct hxc_caxecraft_app_StagedPlayableLevel *hxc_l_staged = hxc_l_tmp_enum_payload_project_n603;
                        hxc_l_gc_roots[391] = (const void *)hxc_l_staged;
                        hxc_l_stagedDestination = hxc_l_staged;
                        break;
                      }
                    case hxc_caxecraft_app_PlayableLevelStageResult_PlayableLevelStageRejected:
                      {
                        if (hxc_io_println((hxc_string){ (const uint8_t *)"caxecraft: campaign level lacks required playable bindings", 58, true, NULL }) != HXC_STATUS_OK)
                        {
                          abort();
                        }
                        break;
                      }
                    default:
                      {
                        abort();
                      }
                  }
                  break;
                }
              case hxc_caxecraft_content_CampaignLevelLoadResult_CampaignLevelRejected:
                {
                  hxc_l_gc_roots[388] = hxc_l_g_haf30e22646ec.hxc_tag == hxc_caxecraft_content_CampaignLevelLoadResult_CampaignLevelReady ? (const void *)hxc_l_g_haf30e22646ec.hxc_payload.hxc_CampaignLevelReady.hxc_candidate : NULL;
                  if (hxc_l_g_haf30e22646ec.hxc_tag != hxc_caxecraft_content_CampaignLevelLoadResult_CampaignLevelRejected)
                  {
                    abort();
                  }
                  struct hxc_caxecraft_content_CampaignLevelLoadError hxc_l_tmp_enum_payload_project_n606 = hxc_l_g_haf30e22646ec.hxc_payload.hxc_CampaignLevelRejected.hxc_error;
                  hxc_l_error_h5f6d8c94e058 = hxc_l_tmp_enum_payload_project_n606;
                  if (hxc_enum_40863044_retain(&hxc_l_error_h5f6d8c94e058) != HXC_STATUS_OK)
                  {
                    abort();
                  }
                  hxc_string hxc_l_tmp_call_result_n608 = hxc_caxecraft_app_CaxecraftApp_campaignLevelLoadFailure(hxc_l_error_h5f6d8c94e058);
                  hxc_l_tmp_string_concat_right_owner_n235 = hxc_l_tmp_call_result_n608;
                  hxc_string hxc_l_tmp_string_concat_result_n610 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
                  if (hxc_string_concat_ref((hxc_string){ (const uint8_t *)"caxecraft: campaign level rejected: ", 36, true, NULL }, hxc_l_tmp_string_concat_right_owner_n235, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n610) != HXC_STATUS_OK)
                  {
                    abort();
                  }
                  hxc_l_tmp_sys_println_string_argument_owner_n236 = hxc_l_tmp_string_concat_result_n610;
                  if (hxc_io_println(hxc_l_tmp_sys_println_string_argument_owner_n236) != HXC_STATUS_OK)
                  {
                    if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n236) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n235) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    hxc_enum_40863044_destroy(&hxc_l_error_h5f6d8c94e058);
                    hxc_enum_5543d34a_destroy(&hxc_l_g_haf30e22646ec);
                    if (hxc_array_ref_release(hxc_l_initialActorPhases) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    hxc_enum_2c0e695d_destroy(&hxc_l_tmp_static_call_argument_0_owner_n102);
                    if (hxc_string_release(&hxc_l_currentObjectiveId) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    if (hxc_string_release(&hxc_l_latestJournalId) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    if (hxc_string_release(&hxc_l_activeDialogue) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    if (hxc_array_ref_release(hxc_l_tmp_object_storage_n92.hxc_stateStartTicks) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    if (hxc_array_ref_release(hxc_l_tmp_object_storage_n92.hxc_stateIds) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    if (hxc_array_ref_release(hxc_l_tmp_object_storage_n92.hxc_objectIds) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    hxc_record_f6127605_destroy(&hxc_l_receipt);
                    if (hxc_string_release(&hxc_l_levelLabel) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    hxc_enum_4b68fff2_destroy(&hxc_l_g_hccea2de18028);
                    if (hxc_string_release(&hxc_l_pendingCampaignLabel) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    hxc_optional_9d9d283f_destroy(&hxc_l_activeEditorTestPlay);
                    hxc_enum_11853fd1_destroy(&hxc_l_g_h1d08383de4f8);
                    hxc_record_f6127605_destroy(&hxc_l_tmp_record_field_logicalPath_receiver_owner_n23);
                    hxc_enum_777c0102_destroy(&hxc_l_g_h6904c3060e30);
                    hxc_record_8aab5967_destroy(&hxc_l_tmp_managed_flow_owner_n17);
                    hxc_enum_d4806d01_destroy(&hxc_l_g_h1d9290112915);
                    hxc_enum_fcb4401b_destroy(&hxc_l_g_h14daa0fd57c5);
                    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    abort();
                  }
                  if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n236) != HXC_STATUS_OK)
                  {
                    abort();
                  }
                  if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n235) != HXC_STATUS_OK)
                  {
                    abort();
                  }
                  hxc_enum_40863044_destroy(&hxc_l_error_h5f6d8c94e058);
                  break;
                }
              default:
                {
                  abort();
                }
            }
            hxc_enum_5543d34a_destroy(&hxc_l_g_haf30e22646ec);
          }
          hxc_l_gc_roots[380] = (const void *)hxc_l_stagedDestination;
          struct hxc_caxecraft_app_StagedPlayableLevel *hxc_l_readyDestination = hxc_l_stagedDestination;
          bool hxc_l_destinationPublished = false;
          hxc_l_gc_roots[381] = (const void *)hxc_l_readyDestination;
          hxc_l_gc_roots[382] = (const void *)NULL;
          if (hxc_l_readyDestination != NULL)
          {
            struct hxc_caxecraft_app_ActivePlayableLevel *hxc_l_tmp_load_result_n614 = hxc_l_activeLevel;
            hxc_l_gc_roots[392] = (const void *)hxc_l_tmp_load_result_n614;
            if (hxc_l_tmp_load_result_n614 == NULL)
            {
              abort();
            }
            hxc_l_gc_roots[393] = (const void *)hxc_l_readyDestination;
            struct hxc_caxecraft_app_PlayableLevelPublicationResult hxc_l_tmp_instance_call_result_n616 = hxc_caxecraft_app_ActivePlayableLevel_publishStaged(hxc_l_tmp_load_result_n614, hxc_l_readyDestination);
            struct hxc_caxecraft_app_PlayableLevelPublicationResult hxc_l_g_hc35e2356da29 = hxc_l_tmp_instance_call_result_n616;
            switch (hxc_l_g_hc35e2356da29.hxc_tag) {
              case hxc_caxecraft_app_PlayableLevelPublicationResult_PlayableLevelPublished:
                {
                  if (hxc_l_g_hc35e2356da29.hxc_tag != hxc_caxecraft_app_PlayableLevelPublicationResult_PlayableLevelPublished)
                  {
                    abort();
                  }
                  int32_t hxc_l_tmp_enum_payload_project_n619 = hxc_l_g_hc35e2356da29.hxc_payload.hxc_PlayableLevelPublished.hxc_active;
                  int32_t hxc_l_selected = hxc_l_tmp_enum_payload_project_n619;
                  hxc_l_destinationPublished = true;
                  hxc_l_gc_roots[396] = (const void *)hxc_l_destination_h0d1fdb1e5aab;
                  hxc_l_campaignLevel = hxc_l_destination_h0d1fdb1e5aab;
                  struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_load_result_n621 = hxc_l_destination_h0d1fdb1e5aab;
                  hxc_l_gc_roots[397] = (const void *)hxc_l_tmp_load_result_n621;
                  if (hxc_l_tmp_load_result_n621 == NULL)
                  {
                    abort();
                  }
                  hxc_string hxc_l_tmp_class_field_load_result_n622 = (*hxc_l_tmp_load_result_n621).hxc_id;
                  hxc_string hxc_l_tmp_string_assignment_replacement_owner_n241 = hxc_l_tmp_class_field_load_result_n622;
                  if (hxc_string_retain(hxc_l_tmp_string_assignment_replacement_owner_n241) != HXC_STATUS_OK)
                  {
                    abort();
                  }
                  hxc_string hxc_l_tmp_string_assignment_replacement_owned_load_result_n623 = hxc_l_tmp_string_assignment_replacement_owner_n241;
                  if (hxc_string_release(&hxc_l_levelLabel) != HXC_STATUS_OK)
                  {
                    abort();
                  }
                  hxc_l_levelLabel = hxc_l_tmp_string_assignment_replacement_owned_load_result_n623;
                  hxc_l_gc_roots[398] = (const void *)hxc_l_campaign;
                  struct hxc_caxecraft_content_CampaignManifest *hxc_l_selectedCampaign_hd5612c096562 = hxc_l_campaign;
                  hxc_l_gc_roots[399] = (const void *)hxc_l_selectedCampaign_hd5612c096562;
                  hxc_l_gc_roots[400] = (const void *)NULL;
                  if (hxc_l_selectedCampaign_hd5612c096562 != NULL)
                  {
                    int32_t hxc_l_g_h97e1dc294ecb = 0;
                    struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n626 = hxc_l_selectedCampaign_hd5612c096562;
                    hxc_l_gc_roots[401] = (const void *)hxc_l_tmp_load_result_n626;
                    if (hxc_l_tmp_load_result_n626 == NULL)
                    {
                      abort();
                    }
                    struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n627 = (*hxc_l_tmp_load_result_n626).hxc_admittedLevels;
                    hxc_l_gc_roots[402] = (const void *)hxc_l_tmp_class_field_load_result_n627;
                    int32_t hxc_l_tmp_array_length_result_n628;
                    if (hxc_array_ref_length(hxc_l_tmp_class_field_load_result_n627, &hxc_l_tmp_array_length_result_n628) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    int32_t hxc_l_g1_h8aec70436166 = hxc_l_tmp_array_length_result_n628;
                    while (1)
                    {
                      int32_t hxc_l_tmp_load_result_n629 = hxc_l_g_h97e1dc294ecb;
                      if (!(hxc_l_tmp_load_result_n629 < hxc_l_g1_h8aec70436166))
                      {
                        break;
                      }
                      int32_t hxc_l_tmp_increment_load_result_n631 = hxc_l_g_h97e1dc294ecb;
                      hxc_l_g_h97e1dc294ecb = hxc_i32_add_wrapping(hxc_l_tmp_increment_load_result_n631, 1);
                      int32_t hxc_l_index_hb15f8c3e6c2b = hxc_l_tmp_increment_load_result_n631;
                      struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n632 = hxc_l_selectedCampaign_hd5612c096562;
                      hxc_l_gc_roots[410] = (const void *)hxc_l_tmp_load_result_n632;
                      if (hxc_l_tmp_load_result_n632 == NULL)
                      {
                        abort();
                      }
                      struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n633 = (*hxc_l_tmp_load_result_n632).hxc_admittedLevels;
                      hxc_l_gc_roots[411] = (const void *)hxc_l_tmp_class_field_load_result_n633;
                      struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_array_get_result_n635;
                      if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n633, (size_t)hxc_l_index_hb15f8c3e6c2b, &hxc_l_tmp_array_get_result_n635) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      hxc_l_gc_roots[412] = (const void *)hxc_l_tmp_array_get_result_n635;
                      if (hxc_l_tmp_array_get_result_n635 == NULL)
                      {
                        abort();
                      }
                      hxc_string hxc_l_tmp_class_field_load_result_n636 = (*hxc_l_tmp_array_get_result_n635).hxc_id;
                      struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_load_result_n637 = hxc_l_destination_h0d1fdb1e5aab;
                      hxc_l_gc_roots[413] = (const void *)hxc_l_tmp_load_result_n637;
                      if (hxc_l_tmp_load_result_n637 == NULL)
                      {
                        abort();
                      }
                      hxc_string hxc_l_tmp_class_field_load_result_n638 = (*hxc_l_tmp_load_result_n637).hxc_id;
                      bool hxc_l_tmp_string_equality_result_n246 = hxc_l_tmp_class_field_load_result_n636.data == NULL || hxc_l_tmp_class_field_load_result_n638.data == NULL ? hxc_l_tmp_class_field_load_result_n636.data == hxc_l_tmp_class_field_load_result_n638.data : hxc_l_tmp_class_field_load_result_n636.byte_length == hxc_l_tmp_class_field_load_result_n638.byte_length && (hxc_l_tmp_class_field_load_result_n636.byte_length == 0 || memcmp(hxc_l_tmp_class_field_load_result_n636.data, hxc_l_tmp_class_field_load_result_n638.data, hxc_l_tmp_class_field_load_result_n636.byte_length) == 0);
                      if (hxc_l_tmp_string_equality_result_n246)
                      {
                        hxc_l_selectedCampaignLevelIndex = hxc_l_index_hb15f8c3e6c2b;
                      }
                    }
                  }
                  struct hxc_caxecraft_app_ActivePlayableLevel *hxc_l_tmp_load_result_n642 = hxc_l_activeLevel;
                  hxc_l_gc_roots[403] = (const void *)hxc_l_tmp_load_result_n642;
                  if (hxc_l_tmp_load_result_n642 == NULL)
                  {
                    abort();
                  }
                  struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_class_field_load_result_n643 = (*hxc_l_tmp_load_result_n642).hxc_activeView;
                  hxc_l_gc_roots[404] = (const void *)hxc_l_tmp_class_field_load_result_n643;
                  hxc_l_levelView_h37960af8bef4 = hxc_l_tmp_class_field_load_result_n643;
                  struct hxc_caxecraft_app_ActivePlayableLevel *hxc_l_tmp_load_result_n644 = hxc_l_activeLevel;
                  hxc_l_gc_roots[405] = (const void *)hxc_l_tmp_load_result_n644;
                  if (hxc_l_tmp_load_result_n644 == NULL)
                  {
                    abort();
                  }
                  struct hxc_caxecraft_content_ActiveContent *hxc_l_tmp_class_field_load_result_n645 = (*hxc_l_tmp_load_result_n644).hxc_content;
                  hxc_l_gc_roots[406] = (const void *)hxc_l_tmp_class_field_load_result_n645;
                  if (hxc_l_tmp_class_field_load_result_n645 == NULL)
                  {
                    abort();
                  }
                  struct hxc_caxecraft_content_LoadedContentGeneration *hxc_l_tmp_class_field_load_result_n646 = (*hxc_l_tmp_class_field_load_result_n645).hxc_active;
                  hxc_l_gc_roots[407] = (const void *)hxc_l_tmp_class_field_load_result_n646;
                  if (hxc_l_tmp_class_field_load_result_n646 == NULL)
                  {
                    abort();
                  }
                  struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_class_field_load_result_n647 = (*hxc_l_tmp_class_field_load_result_n646).hxc_ownedSession;
                  hxc_l_gc_roots[408] = (const void *)hxc_l_tmp_class_field_load_result_n647;
                  hxc_l_session = hxc_l_tmp_class_field_load_result_n647;
                  struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n648 = hxc_l_session;
                  hxc_l_gc_roots[409] = (const void *)hxc_l_tmp_load_result_n648;
                  if (hxc_l_tmp_load_result_n648 == NULL)
                  {
                    abort();
                  }
                  struct hxc_caxecraft_domain_GameView hxc_l_tmp_instance_call_result_n649 = hxc_caxecraft_domain_GameSession_view(hxc_l_tmp_load_result_n648);
                  hxc_l_initialView = hxc_l_tmp_instance_call_result_n649;
                  if (!hxc_l_initialView.hxc_valid)
                  {
                    hxc_l_quit = true;
                  }
                  else
                  {
                    hxc_l_character = hxc_l_initialView.hxc_localPlayer;
                    hxc_l_conversation = (struct hxc_optional_caxecraft_app_ConversationState){ .hxc_has_value = false };
                    hxc_string hxc_l_tmp_string_assignment_replacement_owner_n247 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
                    if (hxc_string_retain(hxc_l_tmp_string_assignment_replacement_owner_n247) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    hxc_string hxc_l_tmp_string_assignment_replacement_owned_load_result_n652 = hxc_l_tmp_string_assignment_replacement_owner_n247;
                    if (hxc_string_release(&hxc_l_activeDialogue) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    hxc_l_activeDialogue = hxc_l_tmp_string_assignment_replacement_owned_load_result_n652;
                    hxc_string hxc_l_tmp_string_assignment_replacement_owner_n248 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
                    if (hxc_string_retain(hxc_l_tmp_string_assignment_replacement_owner_n248) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    hxc_string hxc_l_tmp_string_assignment_replacement_owned_load_result_n653 = hxc_l_tmp_string_assignment_replacement_owner_n248;
                    if (hxc_string_release(&hxc_l_latestJournalId) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    hxc_l_latestJournalId = hxc_l_tmp_string_assignment_replacement_owned_load_result_n653;
                    struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n654 = hxc_l_levelView_h37960af8bef4;
                    hxc_l_gc_roots[414] = (const void *)hxc_l_tmp_load_result_n654;
                    if (hxc_l_tmp_load_result_n654 == NULL)
                    {
                      abort();
                    }
                    struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_class_field_load_result_n655 = (*hxc_l_tmp_load_result_n654).hxc_levelPresentation;
                    hxc_l_gc_roots[415] = (const void *)hxc_l_tmp_class_field_load_result_n655;
                    if (hxc_l_tmp_class_field_load_result_n655 == NULL)
                    {
                      abort();
                    }
                    hxc_string hxc_l_tmp_class_field_load_result_n656 = (*hxc_l_tmp_class_field_load_result_n655).hxc_startingObjective;
                    hxc_string hxc_l_tmp_string_assignment_replacement_owner_n249 = hxc_l_tmp_class_field_load_result_n656;
                    if (hxc_string_retain(hxc_l_tmp_string_assignment_replacement_owner_n249) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    hxc_string hxc_l_tmp_string_assignment_replacement_owned_load_result_n657 = hxc_l_tmp_string_assignment_replacement_owner_n249;
                    if (hxc_string_release(&hxc_l_currentObjectiveId) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    hxc_l_currentObjectiveId = hxc_l_tmp_string_assignment_replacement_owned_load_result_n657;
                    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n658 = hxc_l_session;
                    hxc_l_gc_roots[416] = (const void *)hxc_l_tmp_load_result_n658;
                    hxc_l_gc_roots[417] = (const void *)hxc_l_levelView_h37960af8bef4;
                    struct hxc_caxecraft_app_CaxecraftApp_AvailableInteractionTarget hxc_l_tmp_call_result_n660 = hxc_caxecraft_app_CaxecraftApp_nearestAvailableInteraction(hxc_l_tmp_load_result_n658, hxc_l_levelView_h37960af8bef4);
                    hxc_l_tmp_static_call_argument_0_owner_n250 = hxc_l_tmp_call_result_n660;
                    enum hxc_caxecraft_app_InteractionPrompt hxc_l_tmp_call_result_n662 = hxc_caxecraft_app_CaxecraftApp_promptForAvailableInteraction(hxc_l_tmp_static_call_argument_0_owner_n250);
                    hxc_l_availableInteractionPrompt = hxc_l_tmp_call_result_n662;
                    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n663 = hxc_l_session;
                    hxc_l_gc_roots[418] = (const void *)hxc_l_tmp_load_result_n663;
                    if (hxc_l_tmp_load_result_n663 == NULL)
                    {
                      abort();
                    }
                    struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n664 = hxc_l_levelView_h37960af8bef4;
                    hxc_l_gc_roots[419] = (const void *)hxc_l_tmp_load_result_n664;
                    if (hxc_l_tmp_load_result_n664 == NULL)
                    {
                      abort();
                    }
                    int32_t hxc_l_tmp_class_field_load_result_n665 = (*hxc_l_tmp_load_result_n664).hxc_enemyId;
                    struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n666 = hxc_caxecraft_domain_GameSession_readCharacter(hxc_l_tmp_load_result_n663, hxc_l_tmp_class_field_load_result_n665);
                    hxc_l_enemyActor = hxc_l_tmp_instance_call_result_n666;
                    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n667 = hxc_l_session;
                    hxc_l_gc_roots[420] = (const void *)hxc_l_tmp_load_result_n667;
                    if (hxc_l_tmp_load_result_n667 == NULL)
                    {
                      abort();
                    }
                    struct hxc_array_ref *hxc_l_tmp_instance_call_result_n668 = hxc_caxecraft_domain_GameSession_actorControllerStateSnapshots(hxc_l_tmp_load_result_n667);
                    hxc_l_phases = hxc_l_tmp_instance_call_result_n668;
                    struct hxc_array_ref *hxc_l_tmp_load_result_n669 = hxc_l_phases;
                    struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n670 = hxc_l_levelView_h37960af8bef4;
                    hxc_l_gc_roots[421] = (const void *)hxc_l_tmp_load_result_n670;
                    if (hxc_l_tmp_load_result_n670 == NULL)
                    {
                      abort();
                    }
                    int32_t hxc_l_tmp_class_field_load_result_n671 = (*hxc_l_tmp_load_result_n670).hxc_enemyId;
                    struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation hxc_l_tmp_call_result_n673 = hxc_caxecraft_app_CaxecraftApp_observeActorPhase(hxc_l_tmp_load_result_n669, hxc_l_tmp_class_field_load_result_n671, hxc_caxecraft_domain_ActorControllerPhase_Resting);
                    hxc_l_enemyPhase_h906f9c33685e = hxc_l_tmp_call_result_n673;
                    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n674 = hxc_l_session;
                    hxc_l_gc_roots[422] = (const void *)hxc_l_tmp_load_result_n674;
                    struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n675 = hxc_l_levelView_h37960af8bef4;
                    hxc_l_gc_roots[423] = (const void *)hxc_l_tmp_load_result_n675;
                    bool hxc_l_tmp_call_result_n677 = hxc_caxecraft_app_CaxecraftApp_dialogueActorsAreValid(hxc_l_tmp_load_result_n674, hxc_l_tmp_load_result_n675, hxc_l_phases);
                    bool hxc_l_tmp_short_circuit_result_n252 = !hxc_l_tmp_call_result_n677;
                    if (!!hxc_l_tmp_call_result_n677)
                    {
                      hxc_l_tmp_short_circuit_result_n252 = hxc_l_enemyActor.hxc_id <= 0;
                    }
                    bool hxc_l_tmp_short_circuit_load_result_n679 = hxc_l_tmp_short_circuit_result_n252;
                    bool hxc_l_tmp_short_circuit_result_n253 = hxc_l_tmp_short_circuit_load_result_n679;
                    if (!hxc_l_tmp_short_circuit_load_result_n679)
                    {
                      hxc_l_tmp_short_circuit_result_n253 = !hxc_l_enemyPhase_h906f9c33685e.hxc_valid;
                    }
                    if (hxc_l_tmp_short_circuit_result_n253)
                    {
                      hxc_l_quit = true;
                    }
                    hxc_l_swordCombat = (struct hxc_caxecraft_gameplay_SwordCombatState){ .hxc_cooldownTicks = 0 };
                    hxc_l_berryDrop = (struct hxc_caxecraft_gameplay_BerryDropState){ .hxc_active = false, .hxc_amount = 0, .hxc_x = 0.0, .hxc_y = 0.0, .hxc_z = 0.0 };
                    hxc_l_cameraWaterBlend = 0.0;
                    struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n684 = hxc_l_levelView_h37960af8bef4;
                    hxc_l_gc_roots[424] = (const void *)hxc_l_tmp_load_result_n684;
                    if (hxc_l_tmp_load_result_n684 == NULL)
                    {
                      abort();
                    }
                    struct hxc_caxecraft_scenario_ScenarioTransform hxc_l_tmp_class_field_load_result_n685 = (*hxc_l_tmp_load_result_n684).hxc_playerSpawn;
                    struct hxc_caxecraft_app_RenderPosition hxc_l_tmp_call_result_n686 = hxc_caxecraft_app_SpawnCameraHeading_headingForSpawn(hxc_l_tmp_class_field_load_result_n685);
                    struct hxc_caxecraft_app_RenderPosition hxc_l_publishedHeading = hxc_l_tmp_call_result_n686;
                    hxc_l_lookX = hxc_l_publishedHeading.hxc_x;
                    hxc_l_lookY = hxc_l_publishedHeading.hxc_y;
                    hxc_l_lookZ = hxc_l_publishedHeading.hxc_z;
                    hxc_l_accumulator = 0.0;
                    struct hxc_caxecraft_app_MotionHistory hxc_l_tmp_call_result_n691 = hxc_caxecraft_app_MotionInterpolation_start(hxc_l_character.hxc_body);
                    hxc_l_motionHistory = hxc_l_tmp_call_result_n691;
                    hxc_l_jumpQueued = false;
                    hxc_l_swordQueued = false;
                    hxc_l_placementBlockedFrames = 0;
                    hxc_l_strikeHitFrames = 0;
                    hxc_l_enemyDefeatedFrames = 0;
                    hxc_l_enemyAttackFrames = 0;
                    hxc_l_pickupFrames = 0;
                    hxc_l_inventoryFullFrames = 0;
                    hxc_l_inventoryFullReason = 0;
                    hxc_l_recoveryFeedbackFrames = 0;
                    hxc_l_aquaticEquipmentFrames = 0;
                    if (hxc_l_self == NULL)
                    {
                      abort();
                    }
                    struct hxc_caxecraft_app_TerrainRenderer *hxc_l_tmp_owned_class_field_address_n692 = &(*hxc_l_self).hxc_terrainRenderer;
                    struct hxc_caxecraft_app_TerrainChunkCache *hxc_l_tmp_owned_class_field_address_n693 = &(*hxc_l_tmp_owned_class_field_address_n692).hxc_cache;
                    if (hxc_l_tmp_owned_class_field_address_n693 == NULL)
                    {
                      abort();
                    }
                    hxc_caxecraft_app_TerrainChunkCache_invalidateAll(hxc_l_tmp_owned_class_field_address_n693);
                    hxc_l_gc_roots[425] = (const void *)NULL;
                    hxc_l_preparedPortal = NULL;
                    struct hxc_caxecraft_app_TerrainRenderer *hxc_l_tmp_owned_class_field_address_n694 = &(*hxc_l_self).hxc_portalTerrainRenderer;
                    struct hxc_caxecraft_app_TerrainChunkCache *hxc_l_tmp_owned_class_field_address_n695 = &(*hxc_l_tmp_owned_class_field_address_n694).hxc_cache;
                    if (hxc_l_tmp_owned_class_field_address_n695 == NULL)
                    {
                      abort();
                    }
                    hxc_caxecraft_app_TerrainChunkCache_invalidateAll(hxc_l_tmp_owned_class_field_address_n695);
                    hxc_l_gc_roots[426] = (const void *)hxc_l_campaign;
                    struct hxc_caxecraft_content_CampaignManifest *hxc_l_nextCampaign = hxc_l_campaign;
                    hxc_l_gc_roots[427] = (const void *)hxc_l_campaignLevel;
                    struct hxc_caxecraft_content_CampaignLevel *hxc_l_nextCampaignLevel = hxc_l_campaignLevel;
                    struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n698 = hxc_l_nextCampaign;
                    hxc_l_gc_roots[428] = (const void *)hxc_l_tmp_load_result_n698;
                    hxc_l_gc_roots[429] = (const void *)NULL;
                    bool hxc_l_tmp_short_circuit_result_n257 = hxc_l_tmp_load_result_n698 != NULL;
                    if (hxc_l_tmp_load_result_n698 != NULL)
                    {
                      hxc_l_gc_roots[430] = (const void *)hxc_l_nextCampaignLevel;
                      hxc_l_gc_roots[431] = (const void *)NULL;
                      hxc_l_tmp_short_circuit_result_n257 = hxc_l_nextCampaignLevel != NULL;
                    }
                    if (hxc_l_tmp_short_circuit_result_n257)
                    {
                      int32_t hxc_l_tmp_record_field_load_result_n701 = hxc_l_character.hxc_id;
                      int32_t hxc_l_tmp_record_field_load_result_n702 = hxc_l_character.hxc_vitals.hxc_health;
                      struct hxc_caxecraft_content_LevelPlayerOptions hxc_l_nextPortalPlayer = (struct hxc_caxecraft_content_LevelPlayerOptions){ .hxc_aquaticProfile = hxc_l_character.hxc_aquaticProfile, .hxc_entityId = hxc_l_tmp_record_field_load_result_n701, .hxc_initialHealth = hxc_l_tmp_record_field_load_result_n702 };
                      struct hxc_caxecraft_content_ContentPackageStore *hxc_l_tmp_load_result_n705 = hxc_l_contentStore;
                      hxc_l_gc_roots[432] = (const void *)hxc_l_tmp_load_result_n705;
                      struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n706 = hxc_l_nextCampaign;
                      hxc_l_gc_roots[433] = (const void *)hxc_l_tmp_load_result_n706;
                      struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_load_result_n707 = hxc_l_nextCampaignLevel;
                      hxc_l_gc_roots[434] = (const void *)hxc_l_tmp_load_result_n707;
                      struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n708 = hxc_l_levelView_h37960af8bef4;
                      hxc_l_gc_roots[435] = (const void *)hxc_l_tmp_load_result_n708;
                      struct hxc_caxecraft_content_ContentGenerationSequence *hxc_l_tmp_load_result_n709 = hxc_l_generationSequence;
                      hxc_l_gc_roots[436] = (const void *)hxc_l_tmp_load_result_n709;
                      if (hxc_l_tmp_load_result_n709 == NULL)
                      {
                        abort();
                      }
                      int32_t hxc_l_tmp_instance_call_result_n710 = hxc_caxecraft_content_ContentGenerationSequence_allocate(hxc_l_tmp_load_result_n709);
                      struct hxc_caxecraft_content_LevelPlayerOptions hxc_l_tmp_load_result_n711 = hxc_l_nextPortalPlayer;
                      hxc_l_gc_roots[437] = (const void *)(struct hxc_caxecraft_app_CampaignPortalPreloadRequest){ .hxc_campaign = hxc_l_tmp_load_result_n706, .hxc_nextGeneration = hxc_l_tmp_instance_call_result_n710, .hxc_playerOptions = hxc_l_tmp_load_result_n711, .hxc_sourceLevel = hxc_l_tmp_load_result_n707, .hxc_sourceView = hxc_l_tmp_load_result_n708, .hxc_store = hxc_l_tmp_load_result_n705 }.hxc_campaign;
                      hxc_l_gc_roots[438] = (const void *)(struct hxc_caxecraft_app_CampaignPortalPreloadRequest){ .hxc_campaign = hxc_l_tmp_load_result_n706, .hxc_nextGeneration = hxc_l_tmp_instance_call_result_n710, .hxc_playerOptions = hxc_l_tmp_load_result_n711, .hxc_sourceLevel = hxc_l_tmp_load_result_n707, .hxc_sourceView = hxc_l_tmp_load_result_n708, .hxc_store = hxc_l_tmp_load_result_n705 }.hxc_sourceLevel;
                      hxc_l_gc_roots[439] = (const void *)(struct hxc_caxecraft_app_CampaignPortalPreloadRequest){ .hxc_campaign = hxc_l_tmp_load_result_n706, .hxc_nextGeneration = hxc_l_tmp_instance_call_result_n710, .hxc_playerOptions = hxc_l_tmp_load_result_n711, .hxc_sourceLevel = hxc_l_tmp_load_result_n707, .hxc_sourceView = hxc_l_tmp_load_result_n708, .hxc_store = hxc_l_tmp_load_result_n705 }.hxc_sourceView;
                      hxc_l_gc_roots[440] = (const void *)(struct hxc_caxecraft_app_CampaignPortalPreloadRequest){ .hxc_campaign = hxc_l_tmp_load_result_n706, .hxc_nextGeneration = hxc_l_tmp_instance_call_result_n710, .hxc_playerOptions = hxc_l_tmp_load_result_n711, .hxc_sourceLevel = hxc_l_tmp_load_result_n707, .hxc_sourceView = hxc_l_tmp_load_result_n708, .hxc_store = hxc_l_tmp_load_result_n705 }.hxc_store;
                      struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_tmp_load_result_n713 = hxc_l_contentRegistry;
                      hxc_l_gc_roots[441] = (const void *)hxc_l_tmp_load_result_n713;
                      if (hxc_l_tmp_load_result_n713 == NULL)
                      {
                        abort();
                      }
                      struct hxc_compiler_interface_dispatch_caxecraft_scenario_ScenarioContentRegistry_value hxc_l_tmp_interface_value_n714 = (struct hxc_compiler_interface_dispatch_caxecraft_scenario_ScenarioContentRegistry_value){ .object = hxc_l_tmp_load_result_n713, .table = &hxc_itable_compiler_interface_dispatch_caxecraft_content_RuntimeContentRegistry_itable_layout_caxecraf_hb3f384641e1a61bf };
                      struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_tmp_load_result_n715 = hxc_l_contentRegistry;
                      hxc_l_gc_roots[442] = (const void *)hxc_l_tmp_load_result_n715;
                      if (hxc_l_tmp_load_result_n715 == NULL)
                      {
                        abort();
                      }
                      struct hxc_compiler_interface_dispatch_caxecraft_content_LevelContentResolver_value hxc_l_tmp_interface_value_n716 = (struct hxc_compiler_interface_dispatch_caxecraft_content_LevelContentResolver_value){ .object = hxc_l_tmp_load_result_n715, .table = &hxc_itable_compiler_interface_dispatch_caxecraft_content_RuntimeContentRegistry_itable_layout_caxecraf_h82ca4f24ca7a7b5a };
                      struct hxc_caxecraft_app_CampaignPortalPreloadResult hxc_l_tmp_call_result_n717 = hxc_caxecraft_app_CampaignPortal_preloadCampaignPortal((struct hxc_caxecraft_app_CampaignPortalPreloadRequest){ .hxc_campaign = hxc_l_tmp_load_result_n706, .hxc_nextGeneration = hxc_l_tmp_instance_call_result_n710, .hxc_playerOptions = hxc_l_tmp_load_result_n711, .hxc_sourceLevel = hxc_l_tmp_load_result_n707, .hxc_sourceView = hxc_l_tmp_load_result_n708, .hxc_store = hxc_l_tmp_load_result_n705 }, hxc_l_tmp_interface_value_n714, hxc_l_tmp_interface_value_n716);
                      hxc_l_gc_roots[443] = hxc_l_tmp_call_result_n717.hxc_tag == hxc_caxecraft_app_CampaignPortalPreloadResult_CampaignPortalReady ? (const void *)hxc_l_tmp_call_result_n717.hxc_payload.hxc_CampaignPortalReady.hxc_portal : NULL;
                      hxc_l_g_hff7fb6e9172a = hxc_l_tmp_call_result_n717;
                      hxc_l_gc_roots[444] = hxc_l_g_hff7fb6e9172a.hxc_tag == hxc_caxecraft_app_CampaignPortalPreloadResult_CampaignPortalReady ? (const void *)hxc_l_g_hff7fb6e9172a.hxc_payload.hxc_CampaignPortalReady.hxc_portal : NULL;
                      switch (hxc_l_g_hff7fb6e9172a.hxc_tag) {
                        case hxc_caxecraft_app_CampaignPortalPreloadResult_CampaignPortalNotDeclared:
                          {
                            break;
                          }
                        case hxc_caxecraft_app_CampaignPortalPreloadResult_CampaignPortalReady:
                          {
                            hxc_l_gc_roots[445] = hxc_l_g_hff7fb6e9172a.hxc_tag == hxc_caxecraft_app_CampaignPortalPreloadResult_CampaignPortalReady ? (const void *)hxc_l_g_hff7fb6e9172a.hxc_payload.hxc_CampaignPortalReady.hxc_portal : NULL;
                            if (hxc_l_g_hff7fb6e9172a.hxc_tag != hxc_caxecraft_app_CampaignPortalPreloadResult_CampaignPortalReady)
                            {
                              abort();
                            }
                            struct hxc_caxecraft_app_PreparedCampaignPortal *hxc_l_tmp_enum_payload_project_n720 = hxc_l_g_hff7fb6e9172a.hxc_payload.hxc_CampaignPortalReady.hxc_portal;
                            hxc_l_gc_roots[446] = (const void *)hxc_l_tmp_enum_payload_project_n720;
                            struct hxc_caxecraft_app_PreparedCampaignPortal *hxc_l_portal_h257ca28070fe = hxc_l_tmp_enum_payload_project_n720;
                            hxc_l_gc_roots[447] = (const void *)hxc_l_portal_h257ca28070fe;
                            hxc_l_preparedPortal = hxc_l_portal_h257ca28070fe;
                            struct hxc_caxecraft_app_PreparedCampaignPortal *hxc_l_tmp_load_result_n722 = hxc_l_portal_h257ca28070fe;
                            hxc_l_gc_roots[448] = (const void *)hxc_l_tmp_load_result_n722;
                            if (hxc_l_tmp_load_result_n722 == NULL)
                            {
                              abort();
                            }
                            struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_class_field_load_result_n723 = (*hxc_l_tmp_load_result_n722).hxc_destinationValue;
                            hxc_l_gc_roots[449] = (const void *)hxc_l_tmp_class_field_load_result_n723;
                            if (hxc_l_tmp_class_field_load_result_n723 == NULL)
                            {
                              abort();
                            }
                            hxc_string hxc_l_tmp_class_field_load_result_n724 = (*hxc_l_tmp_class_field_load_result_n723).hxc_id;
                            hxc_string hxc_l_tmp_string_concat_result_n725 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
                            if (hxc_string_concat_ref((hxc_string){ (const uint8_t *)"caxecraft: campaign-portal=", 27, true, NULL }, hxc_l_tmp_class_field_load_result_n724, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n725) != HXC_STATUS_OK)
                            {
                              abort();
                            }
                            hxc_l_tmp_sys_println_string_argument_owner_n261 = hxc_l_tmp_string_concat_result_n725;
                            if (hxc_io_println(hxc_l_tmp_sys_println_string_argument_owner_n261) != HXC_STATUS_OK)
                            {
                              if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n261) != HXC_STATUS_OK)
                              {
                                abort();
                              }
                              hxc_enum_9b450cfc_destroy(&hxc_l_g_hff7fb6e9172a);
                              if (hxc_array_ref_release(hxc_l_phases) != HXC_STATUS_OK)
                              {
                                abort();
                              }
                              hxc_enum_2c0e695d_destroy(&hxc_l_tmp_static_call_argument_0_owner_n250);
                              if (hxc_array_ref_release(hxc_l_initialActorPhases) != HXC_STATUS_OK)
                              {
                                abort();
                              }
                              hxc_enum_2c0e695d_destroy(&hxc_l_tmp_static_call_argument_0_owner_n102);
                              if (hxc_string_release(&hxc_l_currentObjectiveId) != HXC_STATUS_OK)
                              {
                                abort();
                              }
                              if (hxc_string_release(&hxc_l_latestJournalId) != HXC_STATUS_OK)
                              {
                                abort();
                              }
                              if (hxc_string_release(&hxc_l_activeDialogue) != HXC_STATUS_OK)
                              {
                                abort();
                              }
                              if (hxc_array_ref_release(hxc_l_tmp_object_storage_n92.hxc_stateStartTicks) != HXC_STATUS_OK)
                              {
                                abort();
                              }
                              if (hxc_array_ref_release(hxc_l_tmp_object_storage_n92.hxc_stateIds) != HXC_STATUS_OK)
                              {
                                abort();
                              }
                              if (hxc_array_ref_release(hxc_l_tmp_object_storage_n92.hxc_objectIds) != HXC_STATUS_OK)
                              {
                                abort();
                              }
                              hxc_record_f6127605_destroy(&hxc_l_receipt);
                              if (hxc_string_release(&hxc_l_levelLabel) != HXC_STATUS_OK)
                              {
                                abort();
                              }
                              hxc_enum_4b68fff2_destroy(&hxc_l_g_hccea2de18028);
                              if (hxc_string_release(&hxc_l_pendingCampaignLabel) != HXC_STATUS_OK)
                              {
                                abort();
                              }
                              hxc_optional_9d9d283f_destroy(&hxc_l_activeEditorTestPlay);
                              hxc_enum_11853fd1_destroy(&hxc_l_g_h1d08383de4f8);
                              hxc_record_f6127605_destroy(&hxc_l_tmp_record_field_logicalPath_receiver_owner_n23);
                              hxc_enum_777c0102_destroy(&hxc_l_g_h6904c3060e30);
                              hxc_record_8aab5967_destroy(&hxc_l_tmp_managed_flow_owner_n17);
                              hxc_enum_d4806d01_destroy(&hxc_l_g_h1d9290112915);
                              hxc_enum_fcb4401b_destroy(&hxc_l_g_h14daa0fd57c5);
                              if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
                              {
                                abort();
                              }
                              abort();
                            }
                            if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n261) != HXC_STATUS_OK)
                            {
                              abort();
                            }
                            break;
                          }
                        case hxc_caxecraft_app_CampaignPortalPreloadResult_CampaignPortalPreloadRejected:
                          {
                            if (hxc_io_println((hxc_string){ (const uint8_t *)"caxecraft: campaign portal rejected; ordinary loading remains available", 71, true, NULL }) != HXC_STATUS_OK)
                            {
                              abort();
                            }
                            break;
                          }
                        default:
                          {
                            abort();
                          }
                      }
                      hxc_enum_9b450cfc_destroy(&hxc_l_g_hff7fb6e9172a);
                    }
                    hxc_l_resetMotionThisFrame = true;
                    if (hxc_l_requestedFromCampaignMenu)
                    {
                      enum hxc_caxecraft_app_AppScreen hxc_l_tmp_call_result_n729 = hxc_caxecraft_app_AppScreen_startSelectedCampaign(hxc_l_screen);
                      hxc_l_screen = hxc_l_tmp_call_result_n729;
                      hxc_l_recapturedThisFrame = true;
                      DisableCursor();
                    }
                    hxc_string hxc_l_tmp_string_concat_result_n731 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
                    if (hxc_string_concat_ref((hxc_string){ (const uint8_t *)"caxecraft: campaign-level=", 26, true, NULL }, hxc_l_levelLabel, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n731) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    hxc_l_tmp_string_concat_left_owner_n262 = hxc_l_tmp_string_concat_result_n731;
                    hxc_string hxc_l_tmp_string_concat_result_n733 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
                    if (hxc_string_concat_ref(hxc_l_tmp_string_concat_left_owner_n262, (hxc_string){ (const uint8_t *)" generation=", 12, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n733) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    hxc_l_tmp_string_concat_left_owner_n263 = hxc_l_tmp_string_concat_result_n733;
                    hxc_string hxc_l_tmp_string_concat_left_borrow_result_n734 = hxc_l_tmp_string_concat_left_owner_n263;
                    hxc_string hxc_l_tmp_std_string_int_result_n736 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
                    if (hxc_string_from_int32(hxc_l_selected, hxc_default_allocator(), &hxc_l_tmp_std_string_int_result_n736) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    hxc_l_tmp_string_concat_right_owner_n264 = hxc_l_tmp_std_string_int_result_n736;
                    hxc_string hxc_l_tmp_string_concat_result_n738 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
                    if (hxc_string_concat_ref(hxc_l_tmp_string_concat_left_borrow_result_n734, hxc_l_tmp_string_concat_right_owner_n264, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n738) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    hxc_l_tmp_sys_println_string_argument_owner_n265 = hxc_l_tmp_string_concat_result_n738;
                    if (hxc_io_println(hxc_l_tmp_sys_println_string_argument_owner_n265) != HXC_STATUS_OK)
                    {
                      if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n265) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n264) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n263) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n262) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      if (hxc_array_ref_release(hxc_l_phases) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      hxc_enum_2c0e695d_destroy(&hxc_l_tmp_static_call_argument_0_owner_n250);
                      if (hxc_array_ref_release(hxc_l_initialActorPhases) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      hxc_enum_2c0e695d_destroy(&hxc_l_tmp_static_call_argument_0_owner_n102);
                      if (hxc_string_release(&hxc_l_currentObjectiveId) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      if (hxc_string_release(&hxc_l_latestJournalId) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      if (hxc_string_release(&hxc_l_activeDialogue) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      if (hxc_array_ref_release(hxc_l_tmp_object_storage_n92.hxc_stateStartTicks) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      if (hxc_array_ref_release(hxc_l_tmp_object_storage_n92.hxc_stateIds) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      if (hxc_array_ref_release(hxc_l_tmp_object_storage_n92.hxc_objectIds) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      hxc_record_f6127605_destroy(&hxc_l_receipt);
                      if (hxc_string_release(&hxc_l_levelLabel) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      hxc_enum_4b68fff2_destroy(&hxc_l_g_hccea2de18028);
                      if (hxc_string_release(&hxc_l_pendingCampaignLabel) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      hxc_optional_9d9d283f_destroy(&hxc_l_activeEditorTestPlay);
                      hxc_enum_11853fd1_destroy(&hxc_l_g_h1d08383de4f8);
                      hxc_record_f6127605_destroy(&hxc_l_tmp_record_field_logicalPath_receiver_owner_n23);
                      hxc_enum_777c0102_destroy(&hxc_l_g_h6904c3060e30);
                      hxc_record_8aab5967_destroy(&hxc_l_tmp_managed_flow_owner_n17);
                      hxc_enum_d4806d01_destroy(&hxc_l_g_h1d9290112915);
                      hxc_enum_fcb4401b_destroy(&hxc_l_g_h14daa0fd57c5);
                      if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      abort();
                    }
                    if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n265) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n264) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n263) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n262) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    if (hxc_array_ref_release(hxc_l_phases) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    hxc_enum_2c0e695d_destroy(&hxc_l_tmp_static_call_argument_0_owner_n250);
                  }
                  break;
                }
              case hxc_caxecraft_app_PlayableLevelPublicationResult_PlayableLevelPublicationRejected:
                {
                  if (hxc_io_println((hxc_string){ (const uint8_t *)"caxecraft: campaign level could not replace the active level", 60, true, NULL }) != HXC_STATUS_OK)
                  {
                    abort();
                  }
                  break;
                }
              default:
                {
                  abort();
                }
            }
          }
          struct hxc_caxecraft_app_PreparedCampaignPortal *hxc_l_tmp_load_result_n740 = hxc_l_requestedCampaignPortal;
          hxc_l_gc_roots[394] = (const void *)hxc_l_tmp_load_result_n740;
          hxc_l_gc_roots[395] = (const void *)NULL;
          bool hxc_l_tmp_short_circuit_result_n266 = hxc_l_tmp_load_result_n740 != NULL;
          if (hxc_l_tmp_load_result_n740 != NULL)
          {
            hxc_l_tmp_short_circuit_result_n266 = !hxc_l_destinationPublished;
          }
          if (hxc_l_tmp_short_circuit_result_n266)
          {
            hxc_l_gc_roots[450] = (const void *)hxc_l_destination_h0d1fdb1e5aab;
            hxc_l_pendingCampaignLevel = hxc_l_destination_h0d1fdb1e5aab;
            struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_load_result_n744 = hxc_l_destination_h0d1fdb1e5aab;
            hxc_l_gc_roots[451] = (const void *)hxc_l_tmp_load_result_n744;
            if (hxc_l_tmp_load_result_n744 == NULL)
            {
              abort();
            }
            hxc_string hxc_l_tmp_class_field_load_result_n745 = (*hxc_l_tmp_load_result_n744).hxc_id;
            hxc_string hxc_l_tmp_string_assignment_replacement_owner_n267 = hxc_l_tmp_class_field_load_result_n745;
            if (hxc_string_retain(hxc_l_tmp_string_assignment_replacement_owner_n267) != HXC_STATUS_OK)
            {
              abort();
            }
            hxc_string hxc_l_tmp_string_assignment_replacement_owned_load_result_n746 = hxc_l_tmp_string_assignment_replacement_owner_n267;
            if (hxc_string_release(&hxc_l_pendingCampaignLabel) != HXC_STATUS_OK)
            {
              abort();
            }
            hxc_l_pendingCampaignLabel = hxc_l_tmp_string_assignment_replacement_owned_load_result_n746;
            hxc_l_loadingFramePresented = false;
            enum hxc_caxecraft_app_AppScreen hxc_l_tmp_call_result_n748 = hxc_caxecraft_app_AppScreen_beginLoading(hxc_l_screen);
            hxc_l_screen = hxc_l_tmp_call_result_n748;
            EnableCursor();
          }
          if (hxc_l_requestedFromAuthoredExit)
          {
            hxc_l_gc_roots[452] = (const void *)NULL;
            hxc_l_pendingCampaignLevel = NULL;
            hxc_string hxc_l_tmp_string_assignment_replacement_owner_n268 = (hxc_string){ (const uint8_t *)"", 0, true, NULL };
            if (hxc_string_retain(hxc_l_tmp_string_assignment_replacement_owner_n268) != HXC_STATUS_OK)
            {
              abort();
            }
            hxc_string hxc_l_tmp_string_assignment_replacement_owned_load_result_n750 = hxc_l_tmp_string_assignment_replacement_owner_n268;
            if (hxc_string_release(&hxc_l_pendingCampaignLabel) != HXC_STATUS_OK)
            {
              abort();
            }
            hxc_l_pendingCampaignLabel = hxc_l_tmp_string_assignment_replacement_owned_load_result_n750;
            hxc_l_loadingFramePresented = false;
            enum hxc_caxecraft_app_AppScreen hxc_l_tmp_call_result_n752 = hxc_caxecraft_app_AppScreen_finishLoading(hxc_l_screen);
            hxc_l_screen = hxc_l_tmp_call_result_n752;
            hxc_l_recapturedThisFrame = true;
            DisableCursor();
          }
        }
        struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n753 = hxc_l_levelView_h37960af8bef4;
        hxc_l_gc_roots[368] = (const void *)hxc_l_tmp_load_result_n753;
        if (hxc_l_tmp_load_result_n753 == NULL)
        {
          abort();
        }
        int32_t hxc_l_tmp_class_field_load_result_n754 = (*hxc_l_tmp_load_result_n753).hxc_enemyId;
        int32_t hxc_l_enemyActorId = hxc_l_tmp_class_field_load_result_n754;
        struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n755 = hxc_l_levelView_h37960af8bef4;
        hxc_l_gc_roots[369] = (const void *)hxc_l_tmp_load_result_n755;
        if (hxc_l_tmp_load_result_n755 == NULL)
        {
          abort();
        }
        struct hxc_caxecraft_scenario_ScenarioTransform hxc_l_tmp_class_field_load_result_n756 = (*hxc_l_tmp_load_result_n755).hxc_playerSpawn;
        struct hxc_caxecraft_scenario_ScenarioTransform hxc_l_spawnTransform = hxc_l_tmp_class_field_load_result_n756;
        if (hxc_l_quitPressed)
        {
          hxc_l_quit = true;
        }
        if (hxc_l_hotbarSelection >= 0)
        {
          struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_load_result_n759 = hxc_l_inventory;
          struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n761 = hxc_caxecraft_gameplay_Inventory_select(hxc_l_tmp_load_result_n759, hxc_l_hotbarSelection);
          hxc_l_inventory = hxc_l_tmp_call_result_n761;
        }
        if (hxc_l_hotbarCycle != 0)
        {
          struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_load_result_n763 = hxc_l_inventory;
          struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n765 = hxc_caxecraft_gameplay_Inventory_cycle(hxc_l_tmp_load_result_n763, hxc_l_hotbarCycle);
          hxc_l_inventory = hxc_l_tmp_call_result_n765;
        }
        bool hxc_l_conversationOwnedInput = false;
        struct hxc_optional_caxecraft_app_ConversationState hxc_l_openConversation = hxc_l_conversation;
        bool hxc_l_tmp_call_result_n768 = hxc_caxecraft_app_AppScreen_isPlaying(hxc_l_screen);
        bool hxc_l_tmp_short_circuit_result_n273 = hxc_l_tmp_call_result_n768;
        if (hxc_l_tmp_call_result_n768)
        {
          hxc_l_tmp_short_circuit_result_n273 = hxc_l_openConversation.hxc_has_value;
        }
        bool hxc_l_tmp_short_circuit_load_result_n770 = hxc_l_tmp_short_circuit_result_n273;
        bool hxc_l_tmp_short_circuit_result_n274 = hxc_l_tmp_short_circuit_load_result_n770;
        if (hxc_l_tmp_short_circuit_load_result_n770)
        {
          bool hxc_l_tmp_string_null_equality_result_n275 = hxc_l_activeDialogue.data != NULL;
          hxc_l_tmp_short_circuit_result_n274 = hxc_l_tmp_string_null_equality_result_n275;
        }
        if (hxc_l_tmp_short_circuit_result_n274)
        {
          hxc_l_conversationOwnedInput = true;
          struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n774 = hxc_l_levelView_h37960af8bef4;
          hxc_l_gc_roots[453] = (const void *)hxc_l_tmp_load_result_n774;
          if (hxc_l_tmp_load_result_n774 == NULL)
          {
            abort();
          }
          struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_class_field_load_result_n775 = (*hxc_l_tmp_load_result_n774).hxc_levelPresentation;
          hxc_l_gc_roots[454] = (const void *)hxc_l_tmp_class_field_load_result_n775;
          struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_levelView1_h48fffc92706d = hxc_l_tmp_class_field_load_result_n775;
          struct hxc_optional_caxecraft_app_ConversationState hxc_l_tmp_load_result_n776 = hxc_l_openConversation;
          if (!hxc_l_tmp_load_result_n776.hxc_has_value)
          {
            abort();
          }
          int32_t hxc_l_openConversation1 = hxc_l_tmp_load_result_n776.hxc_value.hxc_lineIndex;
          hxc_string hxc_l_line_h11d8ff15b8a6 = { 0 };
          switch (hxc_l_locale) {
            case 0:
              {
                hxc_l_line_h11d8ff15b8a6 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
                if (hxc_string_retain(hxc_l_line_h11d8ff15b8a6) != HXC_STATUS_OK)
                {
                  abort();
                }
                break;
              }
            case 1:
              {
                hxc_l_line_h11d8ff15b8a6 = (hxc_string){ (const uint8_t *)"es-mx", 5, true, NULL };
                if (hxc_string_retain(hxc_l_line_h11d8ff15b8a6) != HXC_STATUS_OK)
                {
                  abort();
                }
                break;
              }
            default:
              {
                hxc_l_line_h11d8ff15b8a6 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
                if (hxc_string_retain(hxc_l_line_h11d8ff15b8a6) != HXC_STATUS_OK)
                {
                  abort();
                }
                break;
              }
          }
          hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n780 = hxc_l_line_h11d8ff15b8a6;
          hxc_l_tmp_managed_flow_owner_n279 = hxc_l_tmp_managed_flow_carrier_move_result_n780;
          struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_load_result_n781 = hxc_l_levelView1_h48fffc92706d;
          hxc_l_gc_roots[455] = (const void *)hxc_l_tmp_load_result_n781;
          if (hxc_l_tmp_load_result_n781 == NULL)
          {
            abort();
          }
          hxc_string hxc_l_tmp_load_result_n782 = hxc_l_activeDialogue;
          int32_t hxc_l_tmp_load_result_n783 = hxc_l_openConversation1;
          hxc_string hxc_l_tmp_instance_call_result_n785 = hxc_caxecraft_content_RuntimeLevelPresentation_dialogueLine(hxc_l_tmp_load_result_n781, hxc_l_tmp_load_result_n782, hxc_l_tmp_load_result_n783, hxc_l_tmp_managed_flow_owner_n279);
          hxc_l_line_hdce933b4ff16 = hxc_l_tmp_instance_call_result_n785;
          struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n786 = hxc_l_levelView_h37960af8bef4;
          hxc_l_gc_roots[456] = (const void *)hxc_l_tmp_load_result_n786;
          if (hxc_l_tmp_load_result_n786 == NULL)
          {
            abort();
          }
          struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_class_field_load_result_n787 = (*hxc_l_tmp_load_result_n786).hxc_levelPresentation;
          hxc_l_gc_roots[457] = (const void *)hxc_l_tmp_class_field_load_result_n787;
          if (hxc_l_tmp_class_field_load_result_n787 == NULL)
          {
            abort();
          }
          int32_t hxc_l_tmp_instance_call_result_n789 = hxc_caxecraft_content_RuntimeLevelPresentation_dialogueLineCount(hxc_l_tmp_class_field_load_result_n787, hxc_l_activeDialogue);
          int32_t hxc_l_lineCount = hxc_l_tmp_instance_call_result_n789;
          struct hxc_optional_caxecraft_app_ConversationState hxc_l_tmp_load_result_n790 = hxc_l_openConversation;
          if (!hxc_l_tmp_load_result_n790.hxc_has_value)
          {
            abort();
          }
          hxc_string hxc_l_tmp_load_result_n792 = hxc_l_line_hdce933b4ff16;
          if (hxc_l_tmp_load_result_n792.data == NULL)
          {
            abort();
          }
          int32_t hxc_l_tmp_string_length_result_n793;
          if (hxc_string_haxe_length(hxc_l_tmp_load_result_n792, &hxc_l_tmp_string_length_result_n793) != HXC_STATUS_OK)
          {
            abort();
          }
          int32_t hxc_l_tmp_load_result_n794 = hxc_l_lineCount;
          double hxc_l_tmp_load_result_n795 = hxc_l_frameSeconds;
          bool hxc_l_tmp_load_result_n796 = hxc_l_interactPressed;
          struct hxc_caxecraft_app_ConversationAdvance hxc_l_tmp_call_result_n798 = hxc_caxecraft_app_ConversationFlow_advanceConversation(hxc_l_tmp_load_result_n790.hxc_value, hxc_l_tmp_string_length_result_n793, hxc_l_tmp_load_result_n794, hxc_f64_to_i32_saturating(hxc_l_tmp_load_result_n795 * 1000.0), hxc_l_tmp_load_result_n796, hxc_l_riseHeld);
          struct hxc_caxecraft_app_ConversationAdvance hxc_l_g_hc14e800e5c8a = hxc_l_tmp_call_result_n798;
          switch (hxc_l_g_hc14e800e5c8a.hxc_tag) {
            case hxc_caxecraft_app_ConversationAdvance_ConversationContinues:
              {
                if (hxc_l_g_hc14e800e5c8a.hxc_tag != hxc_caxecraft_app_ConversationAdvance_ConversationContinues)
                {
                  abort();
                }
                struct hxc_caxecraft_app_ConversationState hxc_l_tmp_enum_payload_project_n801 = hxc_l_g_hc14e800e5c8a.hxc_payload.hxc_ConversationContinues.hxc_state;
                struct hxc_caxecraft_app_ConversationState hxc_l_next = hxc_l_tmp_enum_payload_project_n801;
                hxc_l_conversation = (struct hxc_optional_caxecraft_app_ConversationState){ .hxc_has_value = true, .hxc_value = hxc_l_next };
                break;
              }
            case hxc_caxecraft_app_ConversationAdvance_ConversationCloses:
              {
                hxc_l_conversation = (struct hxc_optional_caxecraft_app_ConversationState){ .hxc_has_value = false };
                hxc_string hxc_l_tmp_string_assignment_replacement_owner_n284 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
                if (hxc_string_retain(hxc_l_tmp_string_assignment_replacement_owner_n284) != HXC_STATUS_OK)
                {
                  abort();
                }
                hxc_string hxc_l_tmp_string_assignment_replacement_owned_load_result_n804 = hxc_l_tmp_string_assignment_replacement_owner_n284;
                if (hxc_string_release(&hxc_l_activeDialogue) != HXC_STATUS_OK)
                {
                  abort();
                }
                hxc_l_activeDialogue = hxc_l_tmp_string_assignment_replacement_owned_load_result_n804;
                break;
              }
            default:
              {
                abort();
              }
          }
          hxc_l_accumulator = 0.0;
          hxc_l_jumpQueued = false;
          hxc_l_swordQueued = false;
          if (hxc_string_release(&hxc_l_line_hdce933b4ff16) != HXC_STATUS_OK)
          {
            abort();
          }
          if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n279) != HXC_STATUS_OK)
          {
            abort();
          }
        }
        else
        {
          bool hxc_l_tmp_call_result_n806 = hxc_caxecraft_app_AppScreen_isPlaying(hxc_l_screen);
          bool hxc_l_tmp_short_circuit_result_n285 = hxc_l_tmp_call_result_n806;
          if (hxc_l_tmp_call_result_n806)
          {
            hxc_l_tmp_short_circuit_result_n285 = hxc_l_interactPressed;
          }
          if (hxc_l_tmp_short_circuit_result_n285)
          {
            if (hxc_l_character.hxc_vitals.hxc_health <= 0)
            {
              struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n810 = hxc_l_session;
              hxc_l_gc_roots[458] = (const void *)hxc_l_tmp_load_result_n810;
              if (hxc_l_tmp_load_result_n810 == NULL)
              {
                abort();
              }
              struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n811 = hxc_l_session;
              hxc_l_gc_roots[459] = (const void *)hxc_l_tmp_load_result_n811;
              if (hxc_l_tmp_load_result_n811 == NULL)
              {
                abort();
              }
              size_t hxc_l_tmp_length_n813;
              const uint8_t *hxc_l_tmp_instance_call_result_n812 = hxc_caxecraft_domain_GameSession_worldView(hxc_l_tmp_load_result_n811, &hxc_l_tmp_length_n813);
              struct hxc_caxecraft_domain_CharacterBody hxc_l_tmp_call_result_n815 = hxc_caxecraft_app_CaxecraftApp_spawnPlayer(hxc_l_tmp_instance_call_result_n812, hxc_l_tmp_length_n813, hxc_l_spawnTransform);
              struct hxc_caxecraft_domain_LocalCharacterCommandResult hxc_l_tmp_instance_call_result_n816 = hxc_caxecraft_domain_GameSession_reviveLocalPlayerAt(hxc_l_tmp_load_result_n810, hxc_l_tmp_call_result_n815);
              struct hxc_caxecraft_domain_LocalCharacterCommandResult hxc_l_revival = hxc_l_tmp_instance_call_result_n816;
              hxc_l_character = hxc_l_revival.hxc_character;
              if (!hxc_l_revival.hxc_resolved)
              {
                hxc_l_quit = true;
              }
              else
              {
                struct hxc_caxecraft_app_RenderPosition hxc_l_tmp_call_result_n820 = hxc_caxecraft_app_SpawnCameraHeading_headingForSpawn(hxc_l_spawnTransform);
                struct hxc_caxecraft_app_RenderPosition hxc_l_revivedHeading = hxc_l_tmp_call_result_n820;
                hxc_l_lookX = hxc_l_revivedHeading.hxc_x;
                hxc_l_lookY = hxc_l_revivedHeading.hxc_y;
                hxc_l_lookZ = hxc_l_revivedHeading.hxc_z;
                hxc_l_cameraWaterBlend = 0.0;
                hxc_l_accumulator = 0.0;
                hxc_l_resetMotionThisFrame = true;
              }
            }
            else
            {
              struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n824 = hxc_l_session;
              hxc_l_gc_roots[460] = (const void *)hxc_l_tmp_load_result_n824;
              hxc_l_gc_roots[461] = (const void *)hxc_l_levelView_h37960af8bef4;
              struct hxc_caxecraft_app_CaxecraftApp_AvailableInteractionTarget hxc_l_tmp_call_result_n826 = hxc_caxecraft_app_CaxecraftApp_nearestAvailableInteraction(hxc_l_tmp_load_result_n824, hxc_l_levelView_h37960af8bef4);
              hxc_l_g_hcb81b40a6ef2 = hxc_l_tmp_call_result_n826;
              switch (hxc_l_g_hcb81b40a6ef2.hxc_tag) {
                case hxc_caxecraft_app_CaxecraftApp_AvailableInteractionTarget_NoAvailableInteraction:
                  {
                    break;
                  }
                case hxc_caxecraft_app_CaxecraftApp_AvailableInteractionTarget_DialogueInteraction:
                  {
                    if (hxc_l_g_hcb81b40a6ef2.hxc_tag != hxc_caxecraft_app_CaxecraftApp_AvailableInteractionTarget_DialogueInteraction)
                    {
                      abort();
                    }
                    int32_t hxc_l_tmp_enum_payload_project_n829 = hxc_l_g_hcb81b40a6ef2.hxc_payload.hxc_DialogueInteraction.hxc_id;
                    int32_t hxc_l_id_haf782cae5e65 = hxc_l_tmp_enum_payload_project_n829;
                    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n830 = hxc_l_session;
                    hxc_l_gc_roots[462] = (const void *)hxc_l_tmp_load_result_n830;
                    if (hxc_l_tmp_load_result_n830 == NULL)
                    {
                      abort();
                    }
                    bool hxc_l_tmp_instance_call_result_n832 = hxc_caxecraft_domain_GameSession_interactWithActor(hxc_l_tmp_load_result_n830, hxc_l_id_haf782cae5e65);
                    if (!hxc_l_tmp_instance_call_result_n832)
                    {
                      hxc_l_quit = true;
                    }
                    break;
                  }
                case hxc_caxecraft_app_CaxecraftApp_AvailableInteractionTarget_StatefulObjectInteraction:
                  {
                    if (hxc_l_g_hcb81b40a6ef2.hxc_tag != hxc_caxecraft_app_CaxecraftApp_AvailableInteractionTarget_StatefulObjectInteraction)
                    {
                      abort();
                    }
                    hxc_string hxc_l_tmp_enum_payload_project_n834 = hxc_l_g_hcb81b40a6ef2.hxc_payload.hxc_StatefulObjectInteraction.hxc_id;
                    hxc_l_id_h6856e05d5724 = hxc_l_tmp_enum_payload_project_n834;
                    if (hxc_string_retain(hxc_l_id_h6856e05d5724) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n835 = hxc_l_session;
                    hxc_l_gc_roots[463] = (const void *)hxc_l_tmp_load_result_n835;
                    if (hxc_l_tmp_load_result_n835 == NULL)
                    {
                      abort();
                    }
                    bool hxc_l_tmp_instance_call_result_n837 = hxc_caxecraft_domain_GameSession_interactWithStatefulObject(hxc_l_tmp_load_result_n835, hxc_l_id_h6856e05d5724);
                    if (!hxc_l_tmp_instance_call_result_n837)
                    {
                      hxc_l_quit = true;
                    }
                    if (hxc_string_release(&hxc_l_id_h6856e05d5724) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    break;
                  }
                default:
                  {
                    abort();
                  }
              }
              hxc_enum_2c0e695d_destroy(&hxc_l_g_hcb81b40a6ef2);
            }
          }
        }
        enum hxc_caxecraft_app_AppScreen hxc_l_tmp_load_result_n838 = hxc_l_screen;
        bool hxc_l_tmp_load_result_n839 = hxc_l_focused;
        int32_t hxc_l_tmp_record_field_load_result_n840 = hxc_l_character.hxc_vitals.hxc_health;
        bool hxc_l_tmp_load_result_n841 = hxc_l_pausePressed;
        struct hxc_caxecraft_app_PlayScreenFrameOutcome hxc_l_tmp_call_result_n844 = hxc_caxecraft_app_AppScreen_advancePlayScreenFrame((struct hxc_caxecraft_app_PlayScreenFrameInput){ .hxc_capturePressed = hxc_l_capturePressed, .hxc_focused = hxc_l_tmp_load_result_n839, .hxc_pausePressed = hxc_l_tmp_load_result_n841, .hxc_playerDefeated = hxc_l_tmp_record_field_load_result_n840 <= 0, .hxc_screen = hxc_l_tmp_load_result_n838 });
        struct hxc_caxecraft_app_PlayScreenFrameOutcome hxc_l_playScreenFrame = hxc_l_tmp_call_result_n844;
        hxc_l_screen = hxc_l_playScreenFrame.hxc_screen;
        if (hxc_l_playScreenFrame.hxc_clearQueuedJump)
        {
          hxc_l_jumpQueued = false;
        }
        if (hxc_l_playScreenFrame.hxc_resetSimulationClock)
        {
          hxc_l_accumulator = 0.0;
        }
        if (hxc_l_playScreenFrame.hxc_resetMotion)
        {
          hxc_l_resetMotionThisFrame = true;
        }
        if (hxc_l_playScreenFrame.hxc_recapturedThisFrame)
        {
          hxc_l_recapturedThisFrame = true;
        }
        switch (hxc_l_playScreenFrame.hxc_pointer) {
          case hxc_caxecraft_app_AppPointerDirective_PreservePointer:
            {
              break;
            }
          case hxc_caxecraft_app_AppPointerDirective_CapturePointer:
            {
              DisableCursor();
              break;
            }
          case hxc_caxecraft_app_AppPointerDirective_ReleasePointer:
            {
              EnableCursor();
              break;
            }
          default:
            {
              abort();
            }
        }
        bool hxc_l_tmp_call_result_n852 = hxc_caxecraft_app_AppScreen_showsTitle(hxc_l_screen);
        bool hxc_l_onTitle = hxc_l_tmp_call_result_n852;
        bool hxc_l_tmp_call_result_n854 = hxc_caxecraft_app_AppScreen_showsCampaignSelection(hxc_l_screen);
        bool hxc_l_onCampaignSelect = hxc_l_tmp_call_result_n854;
        bool hxc_l_tmp_call_result_n856 = hxc_caxecraft_app_AppScreen_showsLoading(hxc_l_screen);
        bool hxc_l_onLoading = hxc_l_tmp_call_result_n856;
        bool hxc_l_tmp_call_result_n858 = hxc_caxecraft_app_AppScreen_showsEditor(hxc_l_screen);
        bool hxc_l_onEditor = hxc_l_tmp_call_result_n858;
        bool hxc_l_tmp_call_result_n860 = hxc_caxecraft_app_AppScreen_pausesSimulation(hxc_l_screen);
        bool hxc_l_paused = hxc_l_tmp_call_result_n860;
        bool hxc_l_tmp_call_result_n862 = hxc_caxecraft_app_AppScreen_capturesPointer(hxc_l_screen);
        bool hxc_l_captured = hxc_l_tmp_call_result_n862;
        int32_t hxc_l_editorNavigationCommand = 0;
        hxc_l_editorActionForFrame = (struct hxc_caxecraft_app_EditorScreenAction){ .hxc_tag = hxc_caxecraft_app_EditorScreenAction_StayInEditor };
        if (hxc_l_onEditor)
        {
          if (hxc_l_self == NULL)
          {
            abort();
          }
          struct hxc_caxecraft_input_NavigationRepeater *hxc_l_tmp_owned_class_field_address_n865 = &(*hxc_l_self).hxc_editorNavigation;
          if (hxc_l_tmp_owned_class_field_address_n865 == NULL)
          {
            abort();
          }
          struct hxc_caxecraft_input_NavigationSample hxc_l_tmp_call_result_n866 = hxc_caxecraft_app_RaylibNavigationInput_samplePrimaryGamepad();
          int32_t hxc_l_tmp_instance_call_result_n868 = hxc_caxecraft_input_NavigationRepeater_advance(hxc_l_tmp_owned_class_field_address_n865, hxc_l_tmp_call_result_n866, hxc_l_frameSeconds);
          hxc_l_editorNavigationCommand = hxc_l_tmp_instance_call_result_n868;
        }
        else
        {
          if (hxc_l_self == NULL)
          {
            abort();
          }
          struct hxc_caxecraft_input_NavigationRepeater *hxc_l_tmp_owned_class_field_address_n869 = &(*hxc_l_self).hxc_editorNavigation;
          if (hxc_l_tmp_owned_class_field_address_n869 == NULL)
          {
            abort();
          }
          hxc_caxecraft_input_NavigationRepeater_release(hxc_l_tmp_owned_class_field_address_n869);
        }
        enum hxc_caxecraft_domain_PlayerCameraMode hxc_l_tmp_load_result_n870 = hxc_l_cameraMode;
        double hxc_l_tmp_load_result_n871 = hxc_l_lookX;
        double hxc_l_tmp_load_result_n872 = hxc_l_lookY;
        double hxc_l_tmp_load_result_n873 = hxc_l_lookZ;
        bool hxc_l_tmp_load_result_n874 = hxc_l_captured;
        bool hxc_l_tmp_load_result_n875 = hxc_l_conversationOwnedInput;
        bool hxc_l_tmp_load_result_n876 = hxc_l_cameraTogglePressed;
        double hxc_l_tmp_load_result_n877 = hxc_l_lookYaw;
        double hxc_l_tmp_load_result_n878 = hxc_l_lookPitch;
        bool hxc_l_tmp_load_result_n879 = hxc_l_jumpPressed;
        struct hxc_caxecraft_domain_PlayerLookFrameOutcome hxc_l_tmp_call_result_n882 = hxc_caxecraft_domain_PlayerCamera_advancePlayerLookFrame((struct hxc_caxecraft_domain_PlayerLookFrameInput){ .hxc_blockedByConversation = hxc_l_tmp_load_result_n875, .hxc_captured = hxc_l_tmp_load_result_n874, .hxc_jumpPressed = hxc_l_tmp_load_result_n879, .hxc_jumpQueued = hxc_l_jumpQueued, .hxc_lookX = hxc_l_tmp_load_result_n871, .hxc_lookY = hxc_l_tmp_load_result_n872, .hxc_lookZ = hxc_l_tmp_load_result_n873, .hxc_mode = hxc_l_tmp_load_result_n870, .hxc_pitchDelta = hxc_l_tmp_load_result_n878, .hxc_togglePressed = hxc_l_tmp_load_result_n876, .hxc_yawDelta = hxc_l_tmp_load_result_n877 });
        struct hxc_caxecraft_domain_PlayerLookFrameOutcome hxc_l_lookFrame = hxc_l_tmp_call_result_n882;
        hxc_l_cameraMode = hxc_l_lookFrame.hxc_mode;
        hxc_l_lookX = hxc_l_lookFrame.hxc_lookX;
        hxc_l_lookY = hxc_l_lookFrame.hxc_lookY;
        hxc_l_lookZ = hxc_l_lookFrame.hxc_lookZ;
        hxc_l_jumpQueued = hxc_l_lookFrame.hxc_jumpQueued;
        bool hxc_l_tmp_load_result_n888 = hxc_l_captured;
        bool hxc_l_tmp_short_circuit_result_n301 = hxc_l_tmp_load_result_n888;
        if (hxc_l_tmp_load_result_n888)
        {
          hxc_l_tmp_short_circuit_result_n301 = !hxc_l_conversationOwnedInput;
        }
        bool hxc_l_tmp_short_circuit_load_result_n890 = hxc_l_tmp_short_circuit_result_n301;
        bool hxc_l_tmp_short_circuit_result_n302 = hxc_l_tmp_short_circuit_load_result_n890;
        if (hxc_l_tmp_short_circuit_load_result_n890)
        {
          hxc_l_tmp_short_circuit_result_n302 = !hxc_l_recapturedThisFrame;
        }
        bool hxc_l_tmp_short_circuit_load_result_n892 = hxc_l_tmp_short_circuit_result_n302;
        bool hxc_l_tmp_short_circuit_result_n303 = hxc_l_tmp_short_circuit_load_result_n892;
        if (hxc_l_tmp_short_circuit_load_result_n892)
        {
          hxc_l_tmp_short_circuit_result_n303 = hxc_l_primaryPressed;
        }
        bool hxc_l_tmp_short_circuit_load_result_n894 = hxc_l_tmp_short_circuit_result_n303;
        bool hxc_l_tmp_short_circuit_result_n304 = hxc_l_tmp_short_circuit_load_result_n894;
        if (hxc_l_tmp_short_circuit_load_result_n894)
        {
          hxc_l_tmp_short_circuit_result_n304 = hxc_l_selectedMode == 1;
        }
        bool hxc_l_tmp_short_circuit_load_result_n896 = hxc_l_tmp_short_circuit_result_n304;
        bool hxc_l_tmp_short_circuit_result_n305 = hxc_l_tmp_short_circuit_load_result_n896;
        if (hxc_l_tmp_short_circuit_load_result_n896)
        {
          bool hxc_l_tmp_call_result_n899 = hxc_caxecraft_gameplay_Inventory_selectedIs(hxc_l_inventory, hxc_caxecraft_gameplay_ItemKind_CopperSword);
          hxc_l_tmp_short_circuit_result_n305 = hxc_l_tmp_call_result_n899;
        }
        if (hxc_l_tmp_short_circuit_result_n305)
        {
          hxc_l_swordQueued = true;
        }
        bool hxc_l_tmp_load_result_n901 = hxc_l_paused;
        bool hxc_l_tmp_short_circuit_result_n306 = !hxc_l_tmp_load_result_n901;
        if (!hxc_l_tmp_load_result_n901)
        {
          hxc_l_tmp_short_circuit_result_n306 = !hxc_l_conversationOwnedInput;
        }
        if (hxc_l_tmp_short_circuit_result_n306)
        {
          double hxc_l_tmp_compound_load_result_n904 = hxc_l_accumulator;
          hxc_l_accumulator = hxc_l_tmp_compound_load_result_n904 + hxc_l_frameSeconds;
        }
        while (1)
        {
          bool hxc_l_tmp_call_result_n907 = hxc_caxecraft_app_AppScreen_pausesSimulation(hxc_l_screen);
          bool hxc_l_tmp_short_circuit_result_n307 = !hxc_l_tmp_call_result_n907;
          if (!hxc_l_tmp_call_result_n907)
          {
            hxc_l_tmp_short_circuit_result_n307 = !hxc_l_conversation.hxc_has_value;
          }
          bool hxc_l_tmp_short_circuit_load_result_n909 = hxc_l_tmp_short_circuit_result_n307;
          bool hxc_l_tmp_short_circuit_result_n308 = hxc_l_tmp_short_circuit_load_result_n909;
          if (hxc_l_tmp_short_circuit_load_result_n909)
          {
            hxc_l_tmp_short_circuit_result_n308 = hxc_l_accumulator >= 0.05;
          }
          if (!hxc_l_tmp_short_circuit_result_n308)
          {
            break;
          }
          double hxc_l_tmp_load_result_n912 = hxc_l_moveForward;
          double hxc_l_tmp_load_result_n913 = hxc_l_lookX;
          double hxc_l_tmp_load_result_n914 = hxc_l_moveRight;
          double hxc_l_moveX = hxc_l_tmp_load_result_n912 * hxc_l_tmp_load_result_n913 - hxc_l_tmp_load_result_n914 * hxc_l_lookZ;
          double hxc_l_tmp_load_result_n916 = hxc_l_moveForward;
          double hxc_l_tmp_load_result_n917 = hxc_l_lookZ;
          double hxc_l_tmp_load_result_n918 = hxc_l_moveRight;
          double hxc_l_moveZ = hxc_l_tmp_load_result_n916 * hxc_l_tmp_load_result_n917 + hxc_l_tmp_load_result_n918 * hxc_l_lookX;
          double hxc_l_tmp_load_result_n920 = hxc_l_moveForward;
          bool hxc_l_tmp_short_circuit_result_n311 = hxc_l_tmp_load_result_n920 != 0.0;
          if (hxc_l_tmp_load_result_n920 != 0.0)
          {
            hxc_l_tmp_short_circuit_result_n311 = hxc_l_moveRight != 0.0;
          }
          if (hxc_l_tmp_short_circuit_result_n311)
          {
            hxc_l_moveX = hxc_l_moveX * 0.7071067811865476;
            hxc_l_moveZ = hxc_l_moveZ * 0.7071067811865476;
          }
          int32_t hxc_l_tmp_load_result_n925 = hxc_l_selectedMode;
          int32_t hxc_l_tmp_conditional_result_n313 = 0;
          if (hxc_l_tmp_load_result_n925 == 1)
          {
            hxc_l_tmp_conditional_result_n313 = 1;
          }
          else
          {
            hxc_l_tmp_conditional_result_n313 = 0;
          }
          int32_t hxc_l_damagePolicy = hxc_l_tmp_conditional_result_n313;
          bool hxc_l_tmp_load_result_n927 = hxc_l_jumpQueued;
          bool hxc_l_tmp_short_circuit_result_n315 = hxc_l_tmp_load_result_n927;
          if (!hxc_l_tmp_load_result_n927)
          {
            int32_t hxc_l_tmp_record_field_load_result_n928 = hxc_l_character.hxc_aquatic.hxc_medium;
            bool hxc_l_tmp_short_circuit_result_n316 = hxc_l_tmp_record_field_load_result_n928 != 0;
            if (hxc_l_tmp_record_field_load_result_n928 != 0)
            {
              hxc_l_tmp_short_circuit_result_n316 = hxc_l_riseHeld;
            }
            hxc_l_tmp_short_circuit_result_n315 = hxc_l_tmp_short_circuit_result_n316;
          }
          bool hxc_l_rising = hxc_l_tmp_short_circuit_result_n315;
          struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n932 = hxc_l_session;
          hxc_l_gc_roots[466] = (const void *)hxc_l_tmp_load_result_n932;
          if (hxc_l_tmp_load_result_n932 == NULL)
          {
            abort();
          }
          double hxc_l_tmp_load_result_n933 = hxc_l_moveX;
          double hxc_l_tmp_load_result_n934 = hxc_l_moveZ;
          bool hxc_l_tmp_load_result_n935 = hxc_l_rising;
          struct hxc_caxecraft_domain_CharacterIntent hxc_l_tmp_call_result_n937 = hxc_caxecraft_domain_Aquatics_input(hxc_l_tmp_load_result_n933, hxc_l_tmp_load_result_n934, hxc_l_tmp_load_result_n935, hxc_l_descendHeld);
          struct hxc_caxecraft_domain_GameTickResult hxc_l_tmp_instance_call_result_n940 = hxc_caxecraft_domain_GameSession_tick(hxc_l_tmp_load_result_n932, (struct hxc_caxecraft_domain_GameTickInput){ .hxc_damagePolicy = hxc_l_damagePolicy, .hxc_intent = hxc_l_tmp_call_result_n937, .hxc_waterUpdateBudget = 64 });
          hxc_l_gameTick = hxc_l_tmp_instance_call_result_n940;
          hxc_l_character = hxc_l_gameTick.hxc_character;
          hxc_l_flow = hxc_l_gameTick.hxc_flow;
          if (hxc_optional_594996a9_retain(&hxc_l_flow) != HXC_STATUS_OK)
          {
            abort();
          }
          if (hxc_l_flow.hxc_has_value)
          {
            if (hxc_l_screen == hxc_caxecraft_app_AppScreen_EditorTestPlay)
            {
              struct hxc_caxecraft_app_CaxecraftEditorScreen *hxc_l_tmp_load_result_n946 = hxc_l_editorScreen;
              hxc_l_gc_roots[467] = (const void *)hxc_l_tmp_load_result_n946;
              if (hxc_l_tmp_load_result_n946 == NULL)
              {
                abort();
              }
              struct hxc_optional_caxecraft_scenario_FlowTickResult hxc_l_tmp_load_result_n947 = hxc_l_flow;
              if (!hxc_l_tmp_load_result_n947.hxc_has_value)
              {
                abort();
              }
              hxc_caxecraft_app_CaxecraftEditorScreen_observeFlowTrace(hxc_l_tmp_load_result_n946, hxc_l_tmp_load_result_n947.hxc_value.hxc_trace);
            }
            struct hxc_optional_caxecraft_scenario_FlowTickResult hxc_l_tmp_load_result_n950 = hxc_l_flow;
            if (!hxc_l_tmp_load_result_n950.hxc_has_value)
            {
              abort();
            }
            int32_t hxc_l_tmp_array_length_result_n953;
            if (hxc_array_ref_length(hxc_l_tmp_load_result_n950.hxc_value.hxc_diagnostics, &hxc_l_tmp_array_length_result_n953) != HXC_STATUS_OK)
            {
              abort();
            }
            if (hxc_l_tmp_array_length_result_n953 == 0)
            {
              int32_t hxc_l_g_h307a504862a3 = 0;
              struct hxc_optional_caxecraft_scenario_FlowTickResult hxc_l_tmp_load_result_n954 = hxc_l_flow;
              if (!hxc_l_tmp_load_result_n954.hxc_has_value)
              {
                abort();
              }
              hxc_l_g1_ha7862ac5d1a2 = hxc_l_tmp_load_result_n954.hxc_value.hxc_presentation;
              if (hxc_array_ref_retain(hxc_l_g1_ha7862ac5d1a2) != HXC_STATUS_OK)
              {
                abort();
              }
              while (1)
              {
                int32_t hxc_l_tmp_load_result_n957 = hxc_l_g_h307a504862a3;
                int32_t hxc_l_tmp_array_length_result_n959;
                if (hxc_array_ref_length(hxc_l_g1_ha7862ac5d1a2, &hxc_l_tmp_array_length_result_n959) != HXC_STATUS_OK)
                {
                  abort();
                }
                if (!(hxc_l_tmp_load_result_n957 < hxc_l_tmp_array_length_result_n959))
                {
                  break;
                }
                struct hxc_array_ref *hxc_l_tmp_load_result_n960 = hxc_l_g1_ha7862ac5d1a2;
                struct hxc_caxecraft_scenario_FlowPresentationEvent hxc_l_tmp_array_get_result_n962;
                if (hxc_array_ref_get_copy(hxc_l_tmp_load_result_n960, (size_t)hxc_l_g_h307a504862a3, &hxc_l_tmp_array_get_result_n962) != HXC_STATUS_OK)
                {
                  abort();
                }
                hxc_l_tmp_array_element_owner_n322 = hxc_l_tmp_array_get_result_n962;
                hxc_l_event_h7f4d6d6090dd = hxc_l_tmp_array_element_owner_n322;
                if (hxc_enum_147eda7c_retain(&hxc_l_event_h7f4d6d6090dd) != HXC_STATUS_OK)
                {
                  abort();
                }
                hxc_l_g_h307a504862a3 = hxc_i32_add_wrapping(hxc_l_g_h307a504862a3, 1);
                switch (hxc_l_event_h7f4d6d6090dd.hxc_tag) {
                  case hxc_caxecraft_scenario_FlowPresentationEvent_DialogueRequested:
                    {
                      if (hxc_l_event_h7f4d6d6090dd.hxc_tag != hxc_caxecraft_scenario_FlowPresentationEvent_DialogueRequested)
                      {
                        abort();
                      }
                      hxc_string hxc_l_tmp_enum_payload_project_n967 = hxc_l_event_h7f4d6d6090dd.hxc_payload.hxc_DialogueRequested.hxc_id;
                      hxc_l_id_h1c394c562c93 = hxc_l_tmp_enum_payload_project_n967;
                      if (hxc_string_retain(hxc_l_id_h1c394c562c93) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      hxc_string hxc_l_tmp_string_assignment_replacement_owner_n324 = hxc_l_id_h1c394c562c93;
                      if (hxc_string_retain(hxc_l_tmp_string_assignment_replacement_owner_n324) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      hxc_string hxc_l_tmp_string_assignment_replacement_owned_load_result_n969 = hxc_l_tmp_string_assignment_replacement_owner_n324;
                      if (hxc_string_release(&hxc_l_activeDialogue) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      hxc_l_activeDialogue = hxc_l_tmp_string_assignment_replacement_owned_load_result_n969;
                      struct hxc_caxecraft_app_ConversationState hxc_l_tmp_call_result_n970 = hxc_caxecraft_app_ConversationFlow_beginConversation();
                      hxc_l_conversation = (struct hxc_optional_caxecraft_app_ConversationState){ .hxc_has_value = true, .hxc_value = hxc_l_tmp_call_result_n970 };
                      if (hxc_string_release(&hxc_l_id_h1c394c562c93) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      break;
                    }
                  case hxc_caxecraft_scenario_FlowPresentationEvent_JournalAdded:
                    {
                      if (hxc_l_event_h7f4d6d6090dd.hxc_tag != hxc_caxecraft_scenario_FlowPresentationEvent_JournalAdded)
                      {
                        abort();
                      }
                      hxc_string hxc_l_tmp_enum_payload_project_n973 = hxc_l_event_h7f4d6d6090dd.hxc_payload.hxc_JournalAdded.hxc_id;
                      hxc_l_id_h04c30e76beca = hxc_l_tmp_enum_payload_project_n973;
                      if (hxc_string_retain(hxc_l_id_h04c30e76beca) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      hxc_string hxc_l_tmp_string_assignment_replacement_owner_n326 = hxc_l_id_h04c30e76beca;
                      if (hxc_string_retain(hxc_l_tmp_string_assignment_replacement_owner_n326) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      hxc_string hxc_l_tmp_string_assignment_replacement_owned_load_result_n975 = hxc_l_tmp_string_assignment_replacement_owner_n326;
                      if (hxc_string_release(&hxc_l_latestJournalId) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      hxc_l_latestJournalId = hxc_l_tmp_string_assignment_replacement_owned_load_result_n975;
                      if (hxc_string_release(&hxc_l_id_h04c30e76beca) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      break;
                    }
                  case hxc_caxecraft_scenario_FlowPresentationEvent_CampaignExitRequested:
                    {
                      if (hxc_l_event_h7f4d6d6090dd.hxc_tag != hxc_caxecraft_scenario_FlowPresentationEvent_CampaignExitRequested)
                      {
                        abort();
                      }
                      hxc_string hxc_l_tmp_enum_payload_project_n977 = hxc_l_event_h7f4d6d6090dd.hxc_payload.hxc_CampaignExitRequested.hxc_exit;
                      hxc_l_exit = hxc_l_tmp_enum_payload_project_n977;
                      if (hxc_string_retain(hxc_l_exit) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      int32_t hxc_l_tmp_load_result_n978 = hxc_l_selectedMode;
                      bool hxc_l_tmp_short_circuit_result_n328 = hxc_l_tmp_load_result_n978 != 1;
                      if (!(hxc_l_tmp_load_result_n978 != 1))
                      {
                        bool hxc_l_tmp_call_result_n980 = hxc_caxecraft_app_AppScreen_acceptsCampaignExit(hxc_l_screen);
                        hxc_l_tmp_short_circuit_result_n328 = !hxc_l_tmp_call_result_n980;
                      }
                      if (hxc_l_tmp_short_circuit_result_n328)
                      {
                        hxc_string hxc_l_tmp_string_concat_result_n983 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
                        if (hxc_string_concat_ref((hxc_string){ (const uint8_t *)"caxecraft: ignored campaign exit outside active Adventure play: ", 64, true, NULL }, hxc_l_exit, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n983) != HXC_STATUS_OK)
                        {
                          abort();
                        }
                        hxc_l_tmp_sys_println_string_argument_owner_n329 = hxc_l_tmp_string_concat_result_n983;
                        if (hxc_io_println(hxc_l_tmp_sys_println_string_argument_owner_n329) != HXC_STATUS_OK)
                        {
                          if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n329) != HXC_STATUS_OK)
                          {
                            abort();
                          }
                          if (hxc_string_release(&hxc_l_exit) != HXC_STATUS_OK)
                          {
                            abort();
                          }
                          hxc_enum_147eda7c_destroy(&hxc_l_event_h7f4d6d6090dd);
                          hxc_array_ce94fc36_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n322);
                          if (hxc_array_ref_release(hxc_l_g1_ha7862ac5d1a2) != HXC_STATUS_OK)
                          {
                            abort();
                          }
                          hxc_optional_594996a9_destroy(&hxc_l_flow);
                          hxc_record_a5e9158f_destroy(&hxc_l_gameTick);
                          hxc_enum_59188943_destroy(&hxc_l_editorActionForFrame);
                          if (hxc_array_ref_release(hxc_l_initialActorPhases) != HXC_STATUS_OK)
                          {
                            abort();
                          }
                          hxc_enum_2c0e695d_destroy(&hxc_l_tmp_static_call_argument_0_owner_n102);
                          if (hxc_string_release(&hxc_l_currentObjectiveId) != HXC_STATUS_OK)
                          {
                            abort();
                          }
                          if (hxc_string_release(&hxc_l_latestJournalId) != HXC_STATUS_OK)
                          {
                            abort();
                          }
                          if (hxc_string_release(&hxc_l_activeDialogue) != HXC_STATUS_OK)
                          {
                            abort();
                          }
                          if (hxc_array_ref_release(hxc_l_tmp_object_storage_n92.hxc_stateStartTicks) != HXC_STATUS_OK)
                          {
                            abort();
                          }
                          if (hxc_array_ref_release(hxc_l_tmp_object_storage_n92.hxc_stateIds) != HXC_STATUS_OK)
                          {
                            abort();
                          }
                          if (hxc_array_ref_release(hxc_l_tmp_object_storage_n92.hxc_objectIds) != HXC_STATUS_OK)
                          {
                            abort();
                          }
                          hxc_record_f6127605_destroy(&hxc_l_receipt);
                          if (hxc_string_release(&hxc_l_levelLabel) != HXC_STATUS_OK)
                          {
                            abort();
                          }
                          hxc_enum_4b68fff2_destroy(&hxc_l_g_hccea2de18028);
                          if (hxc_string_release(&hxc_l_pendingCampaignLabel) != HXC_STATUS_OK)
                          {
                            abort();
                          }
                          hxc_optional_9d9d283f_destroy(&hxc_l_activeEditorTestPlay);
                          hxc_enum_11853fd1_destroy(&hxc_l_g_h1d08383de4f8);
                          hxc_record_f6127605_destroy(&hxc_l_tmp_record_field_logicalPath_receiver_owner_n23);
                          hxc_enum_777c0102_destroy(&hxc_l_g_h6904c3060e30);
                          hxc_record_8aab5967_destroy(&hxc_l_tmp_managed_flow_owner_n17);
                          hxc_enum_d4806d01_destroy(&hxc_l_g_h1d9290112915);
                          hxc_enum_fcb4401b_destroy(&hxc_l_g_h14daa0fd57c5);
                          if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
                          {
                            abort();
                          }
                          abort();
                        }
                        if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n329) != HXC_STATUS_OK)
                        {
                          abort();
                        }
                      }
                      else
                      {
                        hxc_l_gc_roots[468] = (const void *)hxc_l_campaign;
                        struct hxc_caxecraft_content_CampaignManifest *hxc_l_selectedCampaign_hbf26328e4af3 = hxc_l_campaign;
                        hxc_l_gc_roots[469] = (const void *)hxc_l_campaignLevel;
                        struct hxc_caxecraft_content_CampaignLevel *hxc_l_sourceLevel_ha063b0e88abf = hxc_l_campaignLevel;
                        struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n987 = hxc_l_selectedCampaign_hbf26328e4af3;
                        hxc_l_gc_roots[470] = (const void *)hxc_l_tmp_load_result_n987;
                        hxc_l_gc_roots[471] = (const void *)NULL;
                        bool hxc_l_tmp_short_circuit_result_n332 = hxc_l_tmp_load_result_n987 == NULL;
                        if (!(hxc_l_tmp_load_result_n987 == NULL))
                        {
                          hxc_l_gc_roots[472] = (const void *)hxc_l_sourceLevel_ha063b0e88abf;
                          hxc_l_gc_roots[473] = (const void *)NULL;
                          hxc_l_tmp_short_circuit_result_n332 = hxc_l_sourceLevel_ha063b0e88abf == NULL;
                        }
                        if (hxc_l_tmp_short_circuit_result_n332)
                        {
                          hxc_string hxc_l_tmp_string_concat_result_n991 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
                          if (hxc_string_concat_ref((hxc_string){ (const uint8_t *)"caxecraft: ignored campaign exit without an active campaign: ", 61, true, NULL }, hxc_l_exit, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n991) != HXC_STATUS_OK)
                          {
                            abort();
                          }
                          hxc_l_tmp_sys_println_string_argument_owner_n333 = hxc_l_tmp_string_concat_result_n991;
                          if (hxc_io_println(hxc_l_tmp_sys_println_string_argument_owner_n333) != HXC_STATUS_OK)
                          {
                            if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n333) != HXC_STATUS_OK)
                            {
                              abort();
                            }
                            if (hxc_string_release(&hxc_l_exit) != HXC_STATUS_OK)
                            {
                              abort();
                            }
                            hxc_enum_147eda7c_destroy(&hxc_l_event_h7f4d6d6090dd);
                            hxc_array_ce94fc36_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n322);
                            if (hxc_array_ref_release(hxc_l_g1_ha7862ac5d1a2) != HXC_STATUS_OK)
                            {
                              abort();
                            }
                            hxc_optional_594996a9_destroy(&hxc_l_flow);
                            hxc_record_a5e9158f_destroy(&hxc_l_gameTick);
                            hxc_enum_59188943_destroy(&hxc_l_editorActionForFrame);
                            if (hxc_array_ref_release(hxc_l_initialActorPhases) != HXC_STATUS_OK)
                            {
                              abort();
                            }
                            hxc_enum_2c0e695d_destroy(&hxc_l_tmp_static_call_argument_0_owner_n102);
                            if (hxc_string_release(&hxc_l_currentObjectiveId) != HXC_STATUS_OK)
                            {
                              abort();
                            }
                            if (hxc_string_release(&hxc_l_latestJournalId) != HXC_STATUS_OK)
                            {
                              abort();
                            }
                            if (hxc_string_release(&hxc_l_activeDialogue) != HXC_STATUS_OK)
                            {
                              abort();
                            }
                            if (hxc_array_ref_release(hxc_l_tmp_object_storage_n92.hxc_stateStartTicks) != HXC_STATUS_OK)
                            {
                              abort();
                            }
                            if (hxc_array_ref_release(hxc_l_tmp_object_storage_n92.hxc_stateIds) != HXC_STATUS_OK)
                            {
                              abort();
                            }
                            if (hxc_array_ref_release(hxc_l_tmp_object_storage_n92.hxc_objectIds) != HXC_STATUS_OK)
                            {
                              abort();
                            }
                            hxc_record_f6127605_destroy(&hxc_l_receipt);
                            if (hxc_string_release(&hxc_l_levelLabel) != HXC_STATUS_OK)
                            {
                              abort();
                            }
                            hxc_enum_4b68fff2_destroy(&hxc_l_g_hccea2de18028);
                            if (hxc_string_release(&hxc_l_pendingCampaignLabel) != HXC_STATUS_OK)
                            {
                              abort();
                            }
                            hxc_optional_9d9d283f_destroy(&hxc_l_activeEditorTestPlay);
                            hxc_enum_11853fd1_destroy(&hxc_l_g_h1d08383de4f8);
                            hxc_record_f6127605_destroy(&hxc_l_tmp_record_field_logicalPath_receiver_owner_n23);
                            hxc_enum_777c0102_destroy(&hxc_l_g_h6904c3060e30);
                            hxc_record_8aab5967_destroy(&hxc_l_tmp_managed_flow_owner_n17);
                            hxc_enum_d4806d01_destroy(&hxc_l_g_h1d9290112915);
                            hxc_enum_fcb4401b_destroy(&hxc_l_g_h14daa0fd57c5);
                            if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
                            {
                              abort();
                            }
                            abort();
                          }
                          if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n333) != HXC_STATUS_OK)
                          {
                            abort();
                          }
                        }
                        else
                        {
                          struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n993 = hxc_l_selectedCampaign_hbf26328e4af3;
                          hxc_l_gc_roots[474] = (const void *)hxc_l_tmp_load_result_n993;
                          if (hxc_l_tmp_load_result_n993 == NULL)
                          {
                            abort();
                          }
                          struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_load_result_n994 = hxc_l_sourceLevel_ha063b0e88abf;
                          hxc_l_gc_roots[475] = (const void *)hxc_l_tmp_load_result_n994;
                          if (hxc_l_tmp_load_result_n994 == NULL)
                          {
                            abort();
                          }
                          hxc_string hxc_l_tmp_class_field_load_result_n995 = (*hxc_l_tmp_load_result_n994).hxc_id;
                          struct hxc_caxecraft_content_CampaignTransition *hxc_l_tmp_instance_call_result_n997 = hxc_caxecraft_content_CampaignManifest_transitionForRequest(hxc_l_tmp_load_result_n993, hxc_l_tmp_class_field_load_result_n995, hxc_l_exit);
                          hxc_l_gc_roots[476] = (const void *)hxc_l_tmp_instance_call_result_n997;
                          struct hxc_caxecraft_content_CampaignTransition *hxc_l_transition_ha600a5fa424d = hxc_l_tmp_instance_call_result_n997;
                          hxc_l_gc_roots[477] = (const void *)hxc_l_transition_ha600a5fa424d;
                          hxc_l_gc_roots[478] = (const void *)NULL;
                          if (hxc_l_transition_ha600a5fa424d == NULL)
                          {
                            hxc_string hxc_l_tmp_string_concat_result_n1000 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
                            if (hxc_string_concat_ref((hxc_string){ (const uint8_t *)"caxecraft: ignored unknown campaign exit ", 41, true, NULL }, hxc_l_exit, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n1000) != HXC_STATUS_OK)
                            {
                              abort();
                            }
                            hxc_l_tmp_string_concat_left_owner_n335 = hxc_l_tmp_string_concat_result_n1000;
                            hxc_string hxc_l_tmp_string_concat_result_n1002 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
                            if (hxc_string_concat_ref(hxc_l_tmp_string_concat_left_owner_n335, (hxc_string){ (const uint8_t *)" from ", 6, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n1002) != HXC_STATUS_OK)
                            {
                              abort();
                            }
                            hxc_l_tmp_string_concat_left_owner_n336 = hxc_l_tmp_string_concat_result_n1002;
                            hxc_string hxc_l_tmp_string_concat_left_borrow_result_n1003 = hxc_l_tmp_string_concat_left_owner_n336;
                            struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_load_result_n1004 = hxc_l_sourceLevel_ha063b0e88abf;
                            hxc_l_gc_roots[479] = (const void *)hxc_l_tmp_load_result_n1004;
                            if (hxc_l_tmp_load_result_n1004 == NULL)
                            {
                              abort();
                            }
                            hxc_string hxc_l_tmp_class_field_load_result_n1005 = (*hxc_l_tmp_load_result_n1004).hxc_id;
                            hxc_string hxc_l_tmp_string_concat_result_n1006 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
                            if (hxc_string_concat_ref(hxc_l_tmp_string_concat_left_borrow_result_n1003, hxc_l_tmp_class_field_load_result_n1005, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n1006) != HXC_STATUS_OK)
                            {
                              abort();
                            }
                            hxc_l_tmp_sys_println_string_argument_owner_n337 = hxc_l_tmp_string_concat_result_n1006;
                            if (hxc_io_println(hxc_l_tmp_sys_println_string_argument_owner_n337) != HXC_STATUS_OK)
                            {
                              if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n337) != HXC_STATUS_OK)
                              {
                                abort();
                              }
                              if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n336) != HXC_STATUS_OK)
                              {
                                abort();
                              }
                              if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n335) != HXC_STATUS_OK)
                              {
                                abort();
                              }
                              if (hxc_string_release(&hxc_l_exit) != HXC_STATUS_OK)
                              {
                                abort();
                              }
                              hxc_enum_147eda7c_destroy(&hxc_l_event_h7f4d6d6090dd);
                              hxc_array_ce94fc36_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n322);
                              if (hxc_array_ref_release(hxc_l_g1_ha7862ac5d1a2) != HXC_STATUS_OK)
                              {
                                abort();
                              }
                              hxc_optional_594996a9_destroy(&hxc_l_flow);
                              hxc_record_a5e9158f_destroy(&hxc_l_gameTick);
                              hxc_enum_59188943_destroy(&hxc_l_editorActionForFrame);
                              if (hxc_array_ref_release(hxc_l_initialActorPhases) != HXC_STATUS_OK)
                              {
                                abort();
                              }
                              hxc_enum_2c0e695d_destroy(&hxc_l_tmp_static_call_argument_0_owner_n102);
                              if (hxc_string_release(&hxc_l_currentObjectiveId) != HXC_STATUS_OK)
                              {
                                abort();
                              }
                              if (hxc_string_release(&hxc_l_latestJournalId) != HXC_STATUS_OK)
                              {
                                abort();
                              }
                              if (hxc_string_release(&hxc_l_activeDialogue) != HXC_STATUS_OK)
                              {
                                abort();
                              }
                              if (hxc_array_ref_release(hxc_l_tmp_object_storage_n92.hxc_stateStartTicks) != HXC_STATUS_OK)
                              {
                                abort();
                              }
                              if (hxc_array_ref_release(hxc_l_tmp_object_storage_n92.hxc_stateIds) != HXC_STATUS_OK)
                              {
                                abort();
                              }
                              if (hxc_array_ref_release(hxc_l_tmp_object_storage_n92.hxc_objectIds) != HXC_STATUS_OK)
                              {
                                abort();
                              }
                              hxc_record_f6127605_destroy(&hxc_l_receipt);
                              if (hxc_string_release(&hxc_l_levelLabel) != HXC_STATUS_OK)
                              {
                                abort();
                              }
                              hxc_enum_4b68fff2_destroy(&hxc_l_g_hccea2de18028);
                              if (hxc_string_release(&hxc_l_pendingCampaignLabel) != HXC_STATUS_OK)
                              {
                                abort();
                              }
                              hxc_optional_9d9d283f_destroy(&hxc_l_activeEditorTestPlay);
                              hxc_enum_11853fd1_destroy(&hxc_l_g_h1d08383de4f8);
                              hxc_record_f6127605_destroy(&hxc_l_tmp_record_field_logicalPath_receiver_owner_n23);
                              hxc_enum_777c0102_destroy(&hxc_l_g_h6904c3060e30);
                              hxc_record_8aab5967_destroy(&hxc_l_tmp_managed_flow_owner_n17);
                              hxc_enum_d4806d01_destroy(&hxc_l_g_h1d9290112915);
                              hxc_enum_fcb4401b_destroy(&hxc_l_g_h14daa0fd57c5);
                              if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
                              {
                                abort();
                              }
                              abort();
                            }
                            if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n337) != HXC_STATUS_OK)
                            {
                              abort();
                            }
                            if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n336) != HXC_STATUS_OK)
                            {
                              abort();
                            }
                            if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n335) != HXC_STATUS_OK)
                            {
                              abort();
                            }
                          }
                          else
                          {
                            struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n1008 = hxc_l_selectedCampaign_hbf26328e4af3;
                            hxc_l_gc_roots[480] = (const void *)hxc_l_tmp_load_result_n1008;
                            if (hxc_l_tmp_load_result_n1008 == NULL)
                            {
                              abort();
                            }
                            struct hxc_caxecraft_content_CampaignTransition *hxc_l_tmp_load_result_n1009 = hxc_l_transition_ha600a5fa424d;
                            hxc_l_gc_roots[481] = (const void *)hxc_l_tmp_load_result_n1009;
                            if (hxc_l_tmp_load_result_n1009 == NULL)
                            {
                              abort();
                            }
                            hxc_string hxc_l_tmp_class_field_load_result_n1010 = (*hxc_l_tmp_load_result_n1009).hxc_destinationLevel;
                            struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_instance_call_result_n1011 = hxc_caxecraft_content_CampaignManifest_level(hxc_l_tmp_load_result_n1008, hxc_l_tmp_class_field_load_result_n1010);
                            hxc_l_gc_roots[482] = (const void *)hxc_l_tmp_instance_call_result_n1011;
                            struct hxc_caxecraft_content_CampaignLevel *hxc_l_nextLevel = hxc_l_tmp_instance_call_result_n1011;
                            hxc_l_gc_roots[483] = (const void *)hxc_l_nextLevel;
                            hxc_l_gc_roots[484] = (const void *)NULL;
                            if (hxc_l_nextLevel == NULL)
                            {
                              if (hxc_io_println((hxc_string){ (const uint8_t *)"caxecraft: campaign destination disappeared after manifest validation", 69, true, NULL }) != HXC_STATUS_OK)
                              {
                                abort();
                              }
                            }
                            else
                            {
                              hxc_l_gc_roots[485] = (const void *)hxc_l_preparedPortal;
                              struct hxc_caxecraft_app_PreparedCampaignPortal *hxc_l_portal_h919137c2c307 = hxc_l_preparedPortal;
                              struct hxc_caxecraft_app_PreparedCampaignPortal *hxc_l_tmp_load_result_n1014 = hxc_l_portal_h919137c2c307;
                              hxc_l_gc_roots[486] = (const void *)hxc_l_tmp_load_result_n1014;
                              hxc_l_gc_roots[487] = (const void *)NULL;
                              bool hxc_l_tmp_short_circuit_result_n340 = hxc_l_tmp_load_result_n1014 != NULL;
                              if (hxc_l_tmp_load_result_n1014 != NULL)
                              {
                                struct hxc_caxecraft_app_PreparedCampaignPortal *hxc_l_tmp_load_result_n1015 = hxc_l_portal_h919137c2c307;
                                hxc_l_gc_roots[488] = (const void *)hxc_l_tmp_load_result_n1015;
                                if (hxc_l_tmp_load_result_n1015 == NULL)
                                {
                                  abort();
                                }
                                struct hxc_caxecraft_content_CampaignTransition *hxc_l_tmp_class_field_load_result_n1016 = (*hxc_l_tmp_load_result_n1015).hxc_transitionValue;
                                hxc_l_gc_roots[489] = (const void *)hxc_l_tmp_class_field_load_result_n1016;
                                if (hxc_l_tmp_class_field_load_result_n1016 == NULL)
                                {
                                  abort();
                                }
                                hxc_string hxc_l_tmp_class_field_load_result_n1017 = (*hxc_l_tmp_class_field_load_result_n1016).hxc_exit;
                                struct hxc_caxecraft_content_CampaignTransition *hxc_l_tmp_load_result_n1018 = hxc_l_transition_ha600a5fa424d;
                                hxc_l_gc_roots[490] = (const void *)hxc_l_tmp_load_result_n1018;
                                if (hxc_l_tmp_load_result_n1018 == NULL)
                                {
                                  abort();
                                }
                                hxc_string hxc_l_tmp_class_field_load_result_n1019 = (*hxc_l_tmp_load_result_n1018).hxc_exit;
                                bool hxc_l_tmp_string_equality_result_n341 = hxc_l_tmp_class_field_load_result_n1017.data == NULL || hxc_l_tmp_class_field_load_result_n1019.data == NULL ? hxc_l_tmp_class_field_load_result_n1017.data == hxc_l_tmp_class_field_load_result_n1019.data : hxc_l_tmp_class_field_load_result_n1017.byte_length == hxc_l_tmp_class_field_load_result_n1019.byte_length && (hxc_l_tmp_class_field_load_result_n1017.byte_length == 0 || memcmp(hxc_l_tmp_class_field_load_result_n1017.data, hxc_l_tmp_class_field_load_result_n1019.data, hxc_l_tmp_class_field_load_result_n1017.byte_length) == 0);
                                bool hxc_l_tmp_string_equality_result_result_n1021 = hxc_l_tmp_string_equality_result_n341;
                                bool hxc_l_tmp_short_circuit_result_n342 = hxc_l_tmp_string_equality_result_result_n1021;
                                if (hxc_l_tmp_string_equality_result_result_n1021)
                                {
                                  struct hxc_caxecraft_app_PreparedCampaignPortal *hxc_l_tmp_load_result_n1022 = hxc_l_portal_h919137c2c307;
                                  hxc_l_gc_roots[491] = (const void *)hxc_l_tmp_load_result_n1022;
                                  if (hxc_l_tmp_load_result_n1022 == NULL)
                                  {
                                    abort();
                                  }
                                  struct hxc_caxecraft_content_CampaignTransition *hxc_l_tmp_class_field_load_result_n1023 = (*hxc_l_tmp_load_result_n1022).hxc_transitionValue;
                                  hxc_l_gc_roots[492] = (const void *)hxc_l_tmp_class_field_load_result_n1023;
                                  if (hxc_l_tmp_class_field_load_result_n1023 == NULL)
                                  {
                                    abort();
                                  }
                                  hxc_string hxc_l_tmp_class_field_load_result_n1024 = (*hxc_l_tmp_class_field_load_result_n1023).hxc_sourceLevel;
                                  struct hxc_caxecraft_content_CampaignTransition *hxc_l_tmp_load_result_n1025 = hxc_l_transition_ha600a5fa424d;
                                  hxc_l_gc_roots[493] = (const void *)hxc_l_tmp_load_result_n1025;
                                  if (hxc_l_tmp_load_result_n1025 == NULL)
                                  {
                                    abort();
                                  }
                                  hxc_string hxc_l_tmp_class_field_load_result_n1026 = (*hxc_l_tmp_load_result_n1025).hxc_sourceLevel;
                                  bool hxc_l_tmp_string_equality_result_n343 = hxc_l_tmp_class_field_load_result_n1024.data == NULL || hxc_l_tmp_class_field_load_result_n1026.data == NULL ? hxc_l_tmp_class_field_load_result_n1024.data == hxc_l_tmp_class_field_load_result_n1026.data : hxc_l_tmp_class_field_load_result_n1024.byte_length == hxc_l_tmp_class_field_load_result_n1026.byte_length && (hxc_l_tmp_class_field_load_result_n1024.byte_length == 0 || memcmp(hxc_l_tmp_class_field_load_result_n1024.data, hxc_l_tmp_class_field_load_result_n1026.data, hxc_l_tmp_class_field_load_result_n1024.byte_length) == 0);
                                  hxc_l_tmp_short_circuit_result_n342 = hxc_l_tmp_string_equality_result_n343;
                                }
                                bool hxc_l_tmp_short_circuit_load_result_n1029 = hxc_l_tmp_short_circuit_result_n342;
                                bool hxc_l_tmp_short_circuit_result_n344 = hxc_l_tmp_short_circuit_load_result_n1029;
                                if (hxc_l_tmp_short_circuit_load_result_n1029)
                                {
                                  struct hxc_caxecraft_app_PreparedCampaignPortal *hxc_l_tmp_load_result_n1030 = hxc_l_portal_h919137c2c307;
                                  hxc_l_gc_roots[494] = (const void *)hxc_l_tmp_load_result_n1030;
                                  if (hxc_l_tmp_load_result_n1030 == NULL)
                                  {
                                    abort();
                                  }
                                  struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_class_field_load_result_n1031 = (*hxc_l_tmp_load_result_n1030).hxc_destinationValue;
                                  hxc_l_gc_roots[495] = (const void *)hxc_l_tmp_class_field_load_result_n1031;
                                  if (hxc_l_tmp_class_field_load_result_n1031 == NULL)
                                  {
                                    abort();
                                  }
                                  hxc_string hxc_l_tmp_class_field_load_result_n1032 = (*hxc_l_tmp_class_field_load_result_n1031).hxc_id;
                                  struct hxc_caxecraft_content_CampaignTransition *hxc_l_tmp_load_result_n1033 = hxc_l_transition_ha600a5fa424d;
                                  hxc_l_gc_roots[496] = (const void *)hxc_l_tmp_load_result_n1033;
                                  if (hxc_l_tmp_load_result_n1033 == NULL)
                                  {
                                    abort();
                                  }
                                  hxc_string hxc_l_tmp_class_field_load_result_n1034 = (*hxc_l_tmp_load_result_n1033).hxc_destinationLevel;
                                  bool hxc_l_tmp_string_equality_result_n345 = hxc_l_tmp_class_field_load_result_n1032.data == NULL || hxc_l_tmp_class_field_load_result_n1034.data == NULL ? hxc_l_tmp_class_field_load_result_n1032.data == hxc_l_tmp_class_field_load_result_n1034.data : hxc_l_tmp_class_field_load_result_n1032.byte_length == hxc_l_tmp_class_field_load_result_n1034.byte_length && (hxc_l_tmp_class_field_load_result_n1032.byte_length == 0 || memcmp(hxc_l_tmp_class_field_load_result_n1032.data, hxc_l_tmp_class_field_load_result_n1034.data, hxc_l_tmp_class_field_load_result_n1032.byte_length) == 0);
                                  hxc_l_tmp_short_circuit_result_n344 = hxc_l_tmp_string_equality_result_n345;
                                }
                                hxc_l_tmp_short_circuit_result_n340 = hxc_l_tmp_short_circuit_result_n344;
                              }
                              if (hxc_l_tmp_short_circuit_result_n340)
                              {
                                hxc_l_gc_roots[497] = (const void *)hxc_l_portal_h919137c2c307;
                                hxc_l_pendingCampaignPortal = hxc_l_portal_h919137c2c307;
                                hxc_l_gc_roots[498] = (const void *)NULL;
                                hxc_l_preparedPortal = NULL;
                              }
                              else
                              {
                                hxc_l_gc_roots[499] = (const void *)hxc_l_nextLevel;
                                hxc_l_pendingCampaignLevel = hxc_l_nextLevel;
                                struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_load_result_n1041 = hxc_l_nextLevel;
                                hxc_l_gc_roots[500] = (const void *)hxc_l_tmp_load_result_n1041;
                                if (hxc_l_tmp_load_result_n1041 == NULL)
                                {
                                  abort();
                                }
                                hxc_string hxc_l_tmp_class_field_load_result_n1042 = (*hxc_l_tmp_load_result_n1041).hxc_id;
                                hxc_string hxc_l_tmp_string_assignment_replacement_owner_n346 = hxc_l_tmp_class_field_load_result_n1042;
                                if (hxc_string_retain(hxc_l_tmp_string_assignment_replacement_owner_n346) != HXC_STATUS_OK)
                                {
                                  abort();
                                }
                                hxc_string hxc_l_tmp_string_assignment_replacement_owned_load_result_n1043 = hxc_l_tmp_string_assignment_replacement_owner_n346;
                                if (hxc_string_release(&hxc_l_pendingCampaignLabel) != HXC_STATUS_OK)
                                {
                                  abort();
                                }
                                hxc_l_pendingCampaignLabel = hxc_l_tmp_string_assignment_replacement_owned_load_result_n1043;
                                hxc_l_loadingFramePresented = false;
                                enum hxc_caxecraft_app_AppScreen hxc_l_tmp_call_result_n1045 = hxc_caxecraft_app_AppScreen_beginLoading(hxc_l_screen);
                                hxc_l_screen = hxc_l_tmp_call_result_n1045;
                                EnableCursor();
                              }
                              hxc_l_accumulator = 0.0;
                              hxc_l_jumpQueued = false;
                            }
                          }
                        }
                      }
                      if (hxc_string_release(&hxc_l_exit) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      break;
                    }
                  default:
                    {
                      break;
                    }
                }
                hxc_enum_147eda7c_destroy(&hxc_l_event_h7f4d6d6090dd);
                hxc_array_ce94fc36_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n322);
              }
              struct hxc_optional_caxecraft_scenario_FlowTickResult hxc_l_tmp_load_result_n1046 = hxc_l_flow;
              if (!hxc_l_tmp_load_result_n1046.hxc_has_value)
              {
                abort();
              }
              hxc_string hxc_l_tmp_string_assignment_replacement_owner_n347 = hxc_l_tmp_load_result_n1046.hxc_value.hxc_activeObjective;
              if (hxc_string_retain(hxc_l_tmp_string_assignment_replacement_owner_n347) != HXC_STATUS_OK)
              {
                abort();
              }
              hxc_string hxc_l_tmp_string_assignment_replacement_owned_load_result_n1049 = hxc_l_tmp_string_assignment_replacement_owner_n347;
              if (hxc_string_release(&hxc_l_currentObjectiveId) != HXC_STATUS_OK)
              {
                abort();
              }
              hxc_l_currentObjectiveId = hxc_l_tmp_string_assignment_replacement_owned_load_result_n1049;
              if (hxc_array_ref_release(hxc_l_g1_ha7862ac5d1a2) != HXC_STATUS_OK)
              {
                abort();
              }
            }
          }
          if (hxc_l_gameTick.hxc_committed)
          {
            struct hxc_caxecraft_app_MotionHistory hxc_l_tmp_load_result_n1051 = hxc_l_motionHistory;
            struct hxc_caxecraft_app_MotionHistory hxc_l_tmp_call_result_n1053 = hxc_caxecraft_app_MotionInterpolation_advance(hxc_l_tmp_load_result_n1051, hxc_l_character.hxc_body);
            hxc_l_motionHistory = hxc_l_tmp_call_result_n1053;
          }
          hxc_l_cameraWaterBlend = hxc_l_gameTick.hxc_immersion.hxc_cameraBlend;
          if (!hxc_l_gameTick.hxc_committed)
          {
            hxc_l_quit = true;
          }
          if (hxc_l_character.hxc_vitals.hxc_health > 0)
          {
            int32_t hxc_l_pickupIndex = 0;
            while (1)
            {
              int32_t hxc_l_tmp_load_result_n1057 = hxc_l_pickupIndex;
              struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1058 = hxc_l_levelView_h37960af8bef4;
              hxc_l_gc_roots[501] = (const void *)hxc_l_tmp_load_result_n1058;
              if (hxc_l_tmp_load_result_n1058 == NULL)
              {
                abort();
              }
              struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n1059 = (*hxc_l_tmp_load_result_n1058).hxc_items;
              int32_t hxc_l_tmp_array_length_result_n1060;
              if (hxc_array_ref_length(hxc_l_tmp_class_field_load_result_n1059, &hxc_l_tmp_array_length_result_n1060) != HXC_STATUS_OK)
              {
                abort();
              }
              if (!(hxc_l_tmp_load_result_n1057 < hxc_l_tmp_array_length_result_n1060))
              {
                break;
              }
              struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1061 = hxc_l_levelView_h37960af8bef4;
              hxc_l_gc_roots[502] = (const void *)hxc_l_tmp_load_result_n1061;
              if (hxc_l_tmp_load_result_n1061 == NULL)
              {
                abort();
              }
              struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n1062 = (*hxc_l_tmp_load_result_n1061).hxc_items;
              struct hxc_caxecraft_app_LoadedWorldItem hxc_l_tmp_array_get_result_n1064;
              if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n1062, (size_t)hxc_l_pickupIndex, &hxc_l_tmp_array_get_result_n1064) != HXC_STATUS_OK)
              {
                abort();
              }
              struct hxc_caxecraft_app_LoadedWorldItem hxc_l_loadedItem = hxc_l_tmp_array_get_result_n1064;
              struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1065 = hxc_l_session;
              hxc_l_gc_roots[503] = (const void *)hxc_l_tmp_load_result_n1065;
              if (hxc_l_tmp_load_result_n1065 == NULL)
              {
                abort();
              }
              bool hxc_l_tmp_instance_call_result_n1067 = hxc_caxecraft_domain_GameSession_authoredItemIsActive(hxc_l_tmp_load_result_n1065, hxc_l_pickupIndex);
              bool hxc_l_tmp_short_circuit_result_n350 = hxc_l_tmp_instance_call_result_n1067;
              if (hxc_l_tmp_instance_call_result_n1067)
              {
                double hxc_l_tmp_record_field_load_result_n1068 = hxc_l_character.hxc_body.hxc_x;
                double hxc_l_tmp_record_field_load_result_n1069 = hxc_l_character.hxc_body.hxc_y;
                double hxc_l_tmp_record_field_load_result_n1070 = hxc_l_character.hxc_body.hxc_z;
                int32_t hxc_l_tmp_record_field_load_result_n1071 = hxc_l_loadedItem.hxc_xMilli;
                int32_t hxc_l_tmp_record_field_load_result_n1072 = hxc_l_loadedItem.hxc_yMilli;
                bool hxc_l_tmp_call_result_n1074 = hxc_caxecraft_gameplay_WorldItemPickup_isInRange(hxc_l_tmp_record_field_load_result_n1068, hxc_l_tmp_record_field_load_result_n1069, hxc_l_tmp_record_field_load_result_n1070, hxc_l_tmp_record_field_load_result_n1071, hxc_l_tmp_record_field_load_result_n1072, hxc_l_loadedItem.hxc_zMilli);
                hxc_l_tmp_short_circuit_result_n350 = hxc_l_tmp_call_result_n1074;
              }
              if (hxc_l_tmp_short_circuit_result_n350)
              {
                int32_t hxc_l_itemCode = hxc_l_loadedItem.hxc_storageCode;
                struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_tmp_load_result_n1077 = hxc_l_contentRegistry;
                hxc_l_gc_roots[504] = (const void *)hxc_l_tmp_load_result_n1077;
                if (hxc_l_tmp_load_result_n1077 == NULL)
                {
                  abort();
                }
                enum hxc_caxecraft_content_RuntimeItemUseProfile hxc_l_tmp_instance_call_result_n1079 = hxc_caxecraft_content_RuntimeContentRegistry_itemUseProfile(hxc_l_tmp_load_result_n1077, hxc_l_itemCode);
                bool hxc_l_tmp_short_circuit_result_n352 = hxc_l_tmp_instance_call_result_n1079 == hxc_caxecraft_content_RuntimeItemUseProfile_EquipAquatic;
                if (hxc_l_tmp_instance_call_result_n1079 == hxc_caxecraft_content_RuntimeItemUseProfile_EquipAquatic)
                {
                  struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_tmp_load_result_n1081 = hxc_l_contentRegistry;
                  hxc_l_gc_roots[505] = (const void *)hxc_l_tmp_load_result_n1081;
                  if (hxc_l_tmp_load_result_n1081 == NULL)
                  {
                    abort();
                  }
                  bool hxc_l_tmp_instance_call_result_n1083 = hxc_caxecraft_content_RuntimeContentRegistry_itemProvidesAquaticProfile(hxc_l_tmp_load_result_n1081, hxc_l_itemCode);
                  hxc_l_tmp_short_circuit_result_n352 = hxc_l_tmp_instance_call_result_n1083;
                }
                if (hxc_l_tmp_short_circuit_result_n352)
                {
                  struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_tmp_load_result_n1085 = hxc_l_contentRegistry;
                  hxc_l_gc_roots[506] = (const void *)hxc_l_tmp_load_result_n1085;
                  if (hxc_l_tmp_load_result_n1085 == NULL)
                  {
                    abort();
                  }
                  struct hxc_caxecraft_domain_AquaticProfile hxc_l_tmp_instance_call_result_n1087 = hxc_caxecraft_content_RuntimeContentRegistry_itemAquaticProfile(hxc_l_tmp_load_result_n1085, hxc_l_itemCode);
                  struct hxc_caxecraft_domain_AquaticProfile hxc_l_replacement = hxc_l_tmp_instance_call_result_n1087;
                  struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1088 = hxc_l_session;
                  hxc_l_gc_roots[507] = (const void *)hxc_l_tmp_load_result_n1088;
                  if (hxc_l_tmp_load_result_n1088 == NULL)
                  {
                    abort();
                  }
                  int32_t hxc_l_tmp_load_result_n1089 = hxc_l_pickupIndex;
                  struct hxc_caxecraft_domain_AuthoredAquaticEquipmentResult hxc_l_tmp_instance_call_result_n1091 = hxc_caxecraft_domain_GameSession_collectAuthoredAquaticEquipment(hxc_l_tmp_load_result_n1088, hxc_l_tmp_load_result_n1089, hxc_l_replacement);
                  struct hxc_caxecraft_domain_AuthoredAquaticEquipmentResult hxc_l_equipment = hxc_l_tmp_instance_call_result_n1091;
                  hxc_l_character = hxc_l_equipment.hxc_character;
                  if (!hxc_l_equipment.hxc_resolved)
                  {
                    hxc_l_quit = true;
                  }
                  else
                  {
                    if (hxc_l_equipment.hxc_collected)
                    {
                      hxc_l_aquaticEquipmentCode = hxc_l_itemCode;
                      hxc_l_aquaticEquipmentFrames = 120;
                    }
                  }
                }
                else
                {
                  struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_tmp_load_result_n1096 = hxc_l_contentRegistry;
                  hxc_l_gc_roots[508] = (const void *)hxc_l_tmp_load_result_n1096;
                  struct hxc_optional_caxecraft_gameplay_ItemKind hxc_l_tmp_call_result_n1098 = hxc_caxecraft_app_RuntimeInventoryBinding_inventoryKindForRuntimeItem(hxc_l_tmp_load_result_n1096, hxc_l_itemCode);
                  struct hxc_optional_caxecraft_gameplay_ItemKind hxc_l_inventoryKind = hxc_l_tmp_call_result_n1098;
                  if (hxc_l_inventoryKind.hxc_has_value)
                  {
                    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1100 = hxc_l_session;
                    hxc_l_gc_roots[509] = (const void *)hxc_l_tmp_load_result_n1100;
                    if (hxc_l_tmp_load_result_n1100 == NULL)
                    {
                      abort();
                    }
                    int32_t hxc_l_tmp_load_result_n1101 = hxc_l_pickupIndex;
                    struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_load_result_n1102 = hxc_l_inventory;
                    struct hxc_optional_caxecraft_gameplay_ItemKind hxc_l_tmp_load_result_n1103 = hxc_l_inventoryKind;
                    if (!hxc_l_tmp_load_result_n1103.hxc_has_value)
                    {
                      abort();
                    }
                    struct hxc_caxecraft_domain_AuthoredInventoryItemResult hxc_l_tmp_instance_call_result_n1106 = hxc_caxecraft_domain_GameSession_collectAuthoredInventoryItem(hxc_l_tmp_load_result_n1100, hxc_l_tmp_load_result_n1101, hxc_l_tmp_load_result_n1102, hxc_l_tmp_load_result_n1103.hxc_value, hxc_l_loadedItem.hxc_quantity);
                    struct hxc_caxecraft_domain_AuthoredInventoryItemResult hxc_l_pickup = hxc_l_tmp_instance_call_result_n1106;
                    hxc_l_inventory = hxc_l_pickup.hxc_inventory;
                    if (!hxc_l_pickup.hxc_resolved)
                    {
                      hxc_l_quit = true;
                    }
                  }
                }
              }
              hxc_l_pickupIndex = hxc_i32_add_wrapping(hxc_l_pickupIndex, 1);
            }
          }
          if (hxc_l_selectedMode == 1)
          {
            if (hxc_l_character.hxc_vitals.hxc_health > 0)
            {
              struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1112 = hxc_l_session;
              hxc_l_gc_roots[510] = (const void *)hxc_l_tmp_load_result_n1112;
              if (hxc_l_tmp_load_result_n1112 == NULL)
              {
                abort();
              }
              int32_t hxc_l_tmp_record_field_load_result_n1113 = hxc_l_gameTick.hxc_tickIndex;
              struct hxc_caxecraft_domain_ActorControllerTickResult hxc_l_tmp_instance_call_result_n1115 = hxc_caxecraft_domain_GameSession_stepAuthoredActorControllers(hxc_l_tmp_load_result_n1112, hxc_l_tmp_record_field_load_result_n1113, hxc_l_damagePolicy);
              struct hxc_caxecraft_domain_ActorControllerTickResult hxc_l_actorTick = hxc_l_tmp_instance_call_result_n1115;
              switch (hxc_l_actorTick.hxc_status.hxc_tag) {
                case hxc_caxecraft_domain_ActorControllerTickStatus_ControllersAdvanced:
                  {
                    hxc_l_availableInteractionPrompt = hxc_caxecraft_app_InteractionPrompt_NoInteractionPrompt;
                    int32_t hxc_l_g_hb6e3f028a55e = 0;
                    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1118 = hxc_l_session;
                    hxc_l_gc_roots[515] = (const void *)hxc_l_tmp_load_result_n1118;
                    if (hxc_l_tmp_load_result_n1118 == NULL)
                    {
                      abort();
                    }
                    struct hxc_array_ref *hxc_l_tmp_instance_call_result_n1119 = hxc_caxecraft_domain_GameSession_actorControllerEventSnapshots(hxc_l_tmp_load_result_n1118);
                    hxc_l_g1_h170455b768c5 = hxc_l_tmp_instance_call_result_n1119;
                    while (1)
                    {
                      int32_t hxc_l_tmp_load_result_n1120 = hxc_l_g_hb6e3f028a55e;
                      int32_t hxc_l_tmp_array_length_result_n1122;
                      if (hxc_array_ref_length(hxc_l_g1_h170455b768c5, &hxc_l_tmp_array_length_result_n1122) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      if (!(hxc_l_tmp_load_result_n1120 < hxc_l_tmp_array_length_result_n1122))
                      {
                        break;
                      }
                      struct hxc_array_ref *hxc_l_tmp_load_result_n1123 = hxc_l_g1_h170455b768c5;
                      struct hxc_caxecraft_domain_ActorControllerEvent hxc_l_tmp_array_get_result_n1125;
                      if (hxc_array_ref_get_copy(hxc_l_tmp_load_result_n1123, (size_t)hxc_l_g_hb6e3f028a55e, &hxc_l_tmp_array_get_result_n1125) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      hxc_l_tmp_array_element_owner_n361 = hxc_l_tmp_array_get_result_n1125;
                      hxc_l_event_h2a3a461d3d07 = hxc_l_tmp_array_element_owner_n361;
                      if (hxc_enum_8b2a26b8_retain(&hxc_l_event_h2a3a461d3d07) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      hxc_l_g_hb6e3f028a55e = hxc_i32_add_wrapping(hxc_l_g_hb6e3f028a55e, 1);
                      switch (hxc_l_event_h2a3a461d3d07.hxc_tag) {
                        case hxc_caxecraft_domain_ActorControllerEvent_NoControllerEvent:
                          {
                            break;
                          }
                        case hxc_caxecraft_domain_ActorControllerEvent_InteractionAvailable:
                          {
                            if (hxc_l_event_h2a3a461d3d07.hxc_tag != hxc_caxecraft_domain_ActorControllerEvent_InteractionAvailable)
                            {
                              abort();
                            }
                            int32_t hxc_l_tmp_enum_payload_project_n1130 = hxc_l_event_h2a3a461d3d07.hxc_payload.hxc_InteractionAvailable.hxc_characterId;
                            int32_t hxc_l_source_h1e92c3ffda89 = hxc_l_tmp_enum_payload_project_n1130;
                            struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1131 = hxc_l_levelView_h37960af8bef4;
                            hxc_l_gc_roots[516] = (const void *)hxc_l_tmp_load_result_n1131;
                            if (hxc_l_tmp_load_result_n1131 == NULL)
                            {
                              abort();
                            }
                            bool hxc_l_tmp_instance_call_result_n1133 = hxc_caxecraft_app_PlayableLevelView_hasDialogueActor(hxc_l_tmp_load_result_n1131, hxc_l_source_h1e92c3ffda89);
                            if (hxc_l_tmp_instance_call_result_n1133)
                            {
                              hxc_l_availableInteractionPrompt = hxc_caxecraft_app_InteractionPrompt_TalkInteractionPrompt;
                            }
                            break;
                          }
                        case hxc_caxecraft_domain_ActorControllerEvent_LocalPlayerAttack:
                          {
                            if (hxc_l_event_h2a3a461d3d07.hxc_tag != hxc_caxecraft_domain_ActorControllerEvent_LocalPlayerAttack)
                            {
                              abort();
                            }
                            int32_t hxc_l_tmp_enum_payload_project_n1136 = hxc_l_event_h2a3a461d3d07.hxc_payload.hxc_LocalPlayerAttack.hxc_characterId;
                            int32_t hxc_l_source_h041d7dfff114 = hxc_l_tmp_enum_payload_project_n1136;
                            int32_t hxc_l_tmp_load_result_n1137 = hxc_l_source_h041d7dfff114;
                            if (hxc_l_tmp_load_result_n1137 == hxc_l_enemyActorId)
                            {
                              hxc_l_enemyAttackFrames = 120;
                            }
                            break;
                          }
                        case hxc_caxecraft_domain_ActorControllerEvent_DropRequested:
                          {
                            if (hxc_l_event_h2a3a461d3d07.hxc_tag != hxc_caxecraft_domain_ActorControllerEvent_DropRequested)
                            {
                              abort();
                            }
                            int32_t hxc_l_tmp_enum_payload_project_n1140 = hxc_l_event_h2a3a461d3d07.hxc_payload.hxc_DropRequested.hxc_characterId;
                            int32_t hxc_l_source_h0a3610ddaad8 = hxc_l_tmp_enum_payload_project_n1140;
                            if (hxc_l_event_h2a3a461d3d07.hxc_tag != hxc_caxecraft_domain_ActorControllerEvent_DropRequested)
                            {
                              abort();
                            }
                            hxc_string hxc_l_tmp_enum_payload_project_n1142 = hxc_l_event_h2a3a461d3d07.hxc_payload.hxc_DropRequested.hxc_drop;
                            hxc_l_drop = hxc_l_tmp_enum_payload_project_n1142;
                            if (hxc_string_retain(hxc_l_drop) != HXC_STATUS_OK)
                            {
                              abort();
                            }
                            struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_tmp_load_result_n1143 = hxc_l_contentRegistry;
                            hxc_l_gc_roots[517] = (const void *)hxc_l_tmp_load_result_n1143;
                            if (hxc_l_tmp_load_result_n1143 == NULL)
                            {
                              abort();
                            }
                            int32_t hxc_l_tmp_instance_call_result_n1145 = hxc_caxecraft_content_RuntimeContentRegistry_dropQuantityById(hxc_l_tmp_load_result_n1143, hxc_l_drop);
                            int32_t hxc_l_quantity = hxc_l_tmp_instance_call_result_n1145;
                            struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1146 = hxc_l_session;
                            hxc_l_gc_roots[518] = (const void *)hxc_l_tmp_load_result_n1146;
                            if (hxc_l_tmp_load_result_n1146 == NULL)
                            {
                              abort();
                            }
                            struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n1148 = hxc_caxecraft_domain_GameSession_readCharacter(hxc_l_tmp_load_result_n1146, hxc_l_source_h0a3610ddaad8);
                            struct hxc_caxecraft_domain_Character hxc_l_defeatedActor = hxc_l_tmp_instance_call_result_n1148;
                            int32_t hxc_l_tmp_load_result_n1149 = hxc_l_quantity;
                            bool hxc_l_tmp_short_circuit_result_n368 = hxc_l_tmp_load_result_n1149 <= 0;
                            if (!(hxc_l_tmp_load_result_n1149 <= 0))
                            {
                              hxc_l_tmp_short_circuit_result_n368 = hxc_l_defeatedActor.hxc_id <= 0;
                            }
                            if (hxc_l_tmp_short_circuit_result_n368)
                            {
                              hxc_l_quit = true;
                            }
                            else
                            {
                              if (!hxc_l_berryDrop.hxc_active)
                              {
                                struct hxc_caxecraft_domain_Character hxc_l_tmp_load_result_n1153 = hxc_l_defeatedActor;
                                struct hxc_caxecraft_gameplay_BerryDropState hxc_l_tmp_call_result_n1155 = hxc_caxecraft_gameplay_BerryDrop_fromDefeatedCharacter(hxc_l_tmp_load_result_n1153, hxc_l_quantity);
                                hxc_l_berryDrop = hxc_l_tmp_call_result_n1155;
                                hxc_l_enemyDefeatedFrames = 120;
                              }
                            }
                            if (hxc_string_release(&hxc_l_drop) != HXC_STATUS_OK)
                            {
                              abort();
                            }
                            break;
                          }
                        default:
                          {
                            abort();
                          }
                      }
                      hxc_enum_8b2a26b8_destroy(&hxc_l_event_h2a3a461d3d07);
                      hxc_array_43e9f8ca_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n361);
                    }
                    if (hxc_array_ref_release(hxc_l_g1_h170455b768c5) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    break;
                  }
                case hxc_caxecraft_domain_ActorControllerTickStatus_ControllerModelRejected:
                  {
                    hxc_l_quit = true;
                    break;
                  }
                case hxc_caxecraft_domain_ActorControllerTickStatus_ControlledCharacterMissing:
                  {
                    hxc_l_quit = true;
                    break;
                  }
                case hxc_caxecraft_domain_ActorControllerTickStatus_CharacterCommandRejected:
                  {
                    hxc_l_quit = true;
                    break;
                  }
                case hxc_caxecraft_domain_ActorControllerTickStatus_LocalAttackCommandRejected:
                  {
                    hxc_l_quit = true;
                    break;
                  }
                default:
                  {
                    abort();
                  }
              }
            }
            struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1156 = hxc_l_session;
            hxc_l_gc_roots[511] = (const void *)hxc_l_tmp_load_result_n1156;
            if (hxc_l_tmp_load_result_n1156 == NULL)
            {
              abort();
            }
            struct hxc_array_ref *hxc_l_tmp_instance_call_result_n1157 = hxc_caxecraft_domain_GameSession_actorControllerStateSnapshots(hxc_l_tmp_load_result_n1156);
            hxc_l_actorPhases = hxc_l_tmp_instance_call_result_n1157;
            struct hxc_array_ref *hxc_l_tmp_load_result_n1158 = hxc_l_actorPhases;
            struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation hxc_l_tmp_call_result_n1161 = hxc_caxecraft_app_CaxecraftApp_observeActorPhase(hxc_l_tmp_load_result_n1158, hxc_l_enemyActorId, hxc_caxecraft_domain_ActorControllerPhase_Defeated);
            hxc_l_enemyPhase_h906f9c33685e = hxc_l_tmp_call_result_n1161;
            struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1162 = hxc_l_session;
            hxc_l_gc_roots[512] = (const void *)hxc_l_tmp_load_result_n1162;
            if (hxc_l_tmp_load_result_n1162 == NULL)
            {
              abort();
            }
            struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n1164 = hxc_caxecraft_domain_GameSession_readCharacter(hxc_l_tmp_load_result_n1162, hxc_l_enemyActorId);
            hxc_l_enemyActor = hxc_l_tmp_instance_call_result_n1164;
            struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1165 = hxc_l_session;
            hxc_l_gc_roots[513] = (const void *)hxc_l_tmp_load_result_n1165;
            struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1166 = hxc_l_levelView_h37960af8bef4;
            hxc_l_gc_roots[514] = (const void *)hxc_l_tmp_load_result_n1166;
            bool hxc_l_tmp_call_result_n1168 = hxc_caxecraft_app_CaxecraftApp_dialogueActorsAreValid(hxc_l_tmp_load_result_n1165, hxc_l_tmp_load_result_n1166, hxc_l_actorPhases);
            bool hxc_l_tmp_short_circuit_result_n370 = !hxc_l_tmp_call_result_n1168;
            if (!!hxc_l_tmp_call_result_n1168)
            {
              hxc_l_tmp_short_circuit_result_n370 = !hxc_l_enemyPhase_h906f9c33685e.hxc_valid;
            }
            bool hxc_l_tmp_short_circuit_load_result_n1170 = hxc_l_tmp_short_circuit_result_n370;
            bool hxc_l_tmp_short_circuit_result_n371 = hxc_l_tmp_short_circuit_load_result_n1170;
            if (!hxc_l_tmp_short_circuit_load_result_n1170)
            {
              hxc_l_tmp_short_circuit_result_n371 = hxc_l_enemyActor.hxc_id <= 0;
            }
            if (hxc_l_tmp_short_circuit_result_n371)
            {
              hxc_l_quit = true;
            }
            struct hxc_caxecraft_gameplay_SwordCombatState hxc_l_tmp_call_result_n1174 = hxc_caxecraft_gameplay_SwordCombat_step(hxc_l_swordCombat);
            hxc_l_swordCombat = hxc_l_tmp_call_result_n1174;
            bool hxc_l_tmp_load_result_n1175 = hxc_l_swordQueued;
            bool hxc_l_tmp_short_circuit_result_n372 = hxc_l_tmp_load_result_n1175;
            if (hxc_l_tmp_load_result_n1175)
            {
              hxc_l_tmp_short_circuit_result_n372 = !hxc_l_conversation.hxc_has_value;
            }
            if (hxc_l_tmp_short_circuit_result_n372)
            {
              struct hxc_caxecraft_gameplay_SwordCombatState hxc_l_tmp_load_result_n1178 = hxc_l_swordCombat;
              struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_load_result_n1179 = hxc_l_inventory;
              struct hxc_caxecraft_domain_VitalsState hxc_l_tmp_record_field_load_result_n1180 = hxc_l_character.hxc_vitals;
              struct hxc_caxecraft_domain_Character hxc_l_tmp_load_result_n1181 = hxc_l_enemyActor;
              double hxc_l_tmp_record_field_load_result_n1182 = hxc_l_character.hxc_body.hxc_x;
              double hxc_l_tmp_record_field_load_result_n1183 = hxc_l_character.hxc_body.hxc_z;
              double hxc_l_tmp_load_result_n1184 = hxc_l_lookX;
              int32_t hxc_l_tmp_call_result_n1186 = hxc_caxecraft_gameplay_SwordCombat_decide(hxc_l_tmp_load_result_n1178, hxc_l_tmp_load_result_n1179, hxc_l_tmp_record_field_load_result_n1180, hxc_l_tmp_load_result_n1181, hxc_l_tmp_record_field_load_result_n1182, hxc_l_tmp_record_field_load_result_n1183, hxc_l_tmp_load_result_n1184, hxc_l_lookZ);
              int32_t hxc_l_swordDecision = hxc_l_tmp_call_result_n1186;
              int32_t hxc_l_tmp_load_result_n1187 = hxc_l_swordDecision;
              bool hxc_l_tmp_short_circuit_result_n374 = hxc_l_tmp_load_result_n1187 == 5;
              if (hxc_l_tmp_load_result_n1187 == 5)
              {
                struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1188 = hxc_l_session;
                hxc_l_gc_roots[519] = (const void *)hxc_l_tmp_load_result_n1188;
                if (hxc_l_tmp_load_result_n1188 == NULL)
                {
                  abort();
                }
                bool hxc_l_tmp_instance_call_result_n1190 = hxc_caxecraft_domain_GameSession_characterAcceptsMeleeDamage(hxc_l_tmp_load_result_n1188, hxc_l_enemyActorId);
                hxc_l_tmp_short_circuit_result_n374 = hxc_l_tmp_instance_call_result_n1190;
              }
              if (hxc_l_tmp_short_circuit_result_n374)
              {
                hxc_l_gc_roots[520] = (const void *)hxc_l_contentRegistry;
                hxc_string hxc_l_tmp_call_result_n1194 = hxc_caxecraft_app_RuntimeInventoryBinding_runtimeItemContentIdForInventoryKind(hxc_l_contentRegistry, hxc_caxecraft_gameplay_ItemKind_CopperSword);
                hxc_l_swordItemType = hxc_l_tmp_call_result_n1194;
                bool hxc_l_tmp_string_null_equality_result_n377 = hxc_l_swordItemType.data == NULL;
                bool hxc_l_tmp_string_null_equality_result_result_n1196 = hxc_l_tmp_string_null_equality_result_n377;
                struct hxc_optional_caxecraft_domain_CharacterDamageResult hxc_l_tmp_conditional_result_n378 = (struct hxc_optional_caxecraft_domain_CharacterDamageResult){ .hxc_has_value = false };
                if (hxc_l_tmp_string_null_equality_result_result_n1196)
                {
                  hxc_l_tmp_conditional_result_n378 = (struct hxc_optional_caxecraft_domain_CharacterDamageResult){ .hxc_has_value = false };
                }
                else
                {
                  struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1197 = hxc_l_session;
                  hxc_l_gc_roots[521] = (const void *)hxc_l_tmp_load_result_n1197;
                  if (hxc_l_tmp_load_result_n1197 == NULL)
                  {
                    abort();
                  }
                  int32_t hxc_l_tmp_load_result_n1198 = hxc_l_enemyActorId;
                  hxc_string hxc_l_tmp_enum_payload_0_owner_n379 = hxc_l_swordItemType;
                  if (hxc_string_retain(hxc_l_tmp_enum_payload_0_owner_n379) != HXC_STATUS_OK)
                  {
                    abort();
                  }
                  hxc_l_tmp_instance_call_argument_0_owner_n380 = (struct hxc_caxecraft_domain_CharacterDamageCommand){ .hxc_amount = 1, .hxc_cause = (struct hxc_caxecraft_domain_CharacterDamageCause){ .hxc_tag = hxc_caxecraft_domain_CharacterDamageCause_LocalPlayerItem, .hxc_payload.hxc_LocalPlayerItem.hxc_itemType = hxc_l_tmp_enum_payload_0_owner_n379 }, .hxc_target = hxc_l_tmp_load_result_n1198 };
                  struct hxc_caxecraft_domain_CharacterDamageResult hxc_l_tmp_instance_call_result_n1204 = hxc_caxecraft_domain_GameSession_damageCharacter(hxc_l_tmp_load_result_n1197, hxc_l_tmp_instance_call_argument_0_owner_n380);
                  hxc_l_tmp_conditional_result_n378 = (struct hxc_optional_caxecraft_domain_CharacterDamageResult){ .hxc_has_value = true, .hxc_value = hxc_l_tmp_instance_call_result_n1204 };
                  hxc_record_d86f2bdc_destroy(&hxc_l_tmp_instance_call_argument_0_owner_n380);
                }
                struct hxc_optional_caxecraft_domain_CharacterDamageResult hxc_l_damage = hxc_l_tmp_conditional_result_n378;
                struct hxc_optional_caxecraft_domain_CharacterDamageResult hxc_l_tmp_load_result_n1207 = hxc_l_damage;
                bool hxc_l_tmp_short_circuit_result_n381 = !hxc_l_tmp_load_result_n1207.hxc_has_value;
                if (!!hxc_l_tmp_load_result_n1207.hxc_has_value)
                {
                  struct hxc_optional_caxecraft_domain_CharacterDamageResult hxc_l_tmp_load_result_n1208 = hxc_l_damage;
                  if (!hxc_l_tmp_load_result_n1208.hxc_has_value)
                  {
                    abort();
                  }
                  hxc_l_tmp_short_circuit_result_n381 = !hxc_l_tmp_load_result_n1208.hxc_value.hxc_resolved;
                }
                if (hxc_l_tmp_short_circuit_result_n381)
                {
                  hxc_l_quit = true;
                }
                else
                {
                  struct hxc_optional_caxecraft_domain_CharacterDamageResult hxc_l_tmp_load_result_n1212 = hxc_l_damage;
                  if (!hxc_l_tmp_load_result_n1212.hxc_has_value)
                  {
                    abort();
                  }
                  if (hxc_l_tmp_load_result_n1212.hxc_value.hxc_damageApplied > 0)
                  {
                    struct hxc_optional_caxecraft_domain_CharacterDamageResult hxc_l_tmp_load_result_n1215 = hxc_l_damage;
                    if (!hxc_l_tmp_load_result_n1215.hxc_has_value)
                    {
                      abort();
                    }
                    hxc_l_enemyActor = hxc_l_tmp_load_result_n1215.hxc_value.hxc_character;
                    hxc_l_strikeHitFrames = 16;
                  }
                }
                if (hxc_string_release(&hxc_l_swordItemType) != HXC_STATUS_OK)
                {
                  abort();
                }
              }
              int32_t hxc_l_tmp_load_result_n1218 = hxc_l_swordDecision;
              struct hxc_caxecraft_gameplay_SwordCombatState hxc_l_tmp_call_result_n1220 = hxc_caxecraft_gameplay_SwordCombat_after(hxc_l_tmp_load_result_n1218, hxc_l_swordCombat);
              hxc_l_swordCombat = hxc_l_tmp_call_result_n1220;
              hxc_l_swordQueued = false;
            }
            if (hxc_array_ref_release(hxc_l_actorPhases) != HXC_STATUS_OK)
            {
              abort();
            }
          }
          hxc_l_jumpQueued = false;
          hxc_l_accumulator = hxc_l_accumulator - 0.05;
          hxc_optional_594996a9_destroy(&hxc_l_flow);
          hxc_record_a5e9158f_destroy(&hxc_l_gameTick);
        }
        struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1222 = hxc_l_session;
        hxc_l_gc_roots[464] = (const void *)hxc_l_tmp_load_result_n1222;
        if (hxc_l_tmp_load_result_n1222 == NULL)
        {
          abort();
        }
        struct hxc_caxecraft_domain_CharacterBody hxc_l_tmp_record_field_load_result_n1224 = hxc_l_character.hxc_body;
        double hxc_l_tmp_load_result_n1225 = hxc_l_lookX;
        double hxc_l_tmp_load_result_n1226 = hxc_l_lookY;
        struct hxc_caxecraft_domain_PlayerCameraView hxc_l_tmp_instance_call_result_n1228 = hxc_caxecraft_domain_GameSession_playerCamera(hxc_l_tmp_load_result_n1222, hxc_caxecraft_domain_PlayerCameraMode_FirstPerson, hxc_l_tmp_record_field_load_result_n1224, hxc_l_tmp_load_result_n1225, hxc_l_tmp_load_result_n1226, hxc_l_lookZ);
        struct hxc_caxecraft_domain_PlayerCameraView hxc_l_interactionView = hxc_l_tmp_instance_call_result_n1228;
        struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1229 = hxc_l_session;
        hxc_l_gc_roots[465] = (const void *)hxc_l_tmp_load_result_n1229;
        if (hxc_l_tmp_load_result_n1229 == NULL)
        {
          abort();
        }
        size_t hxc_l_tmp_length_n1231;
        const uint8_t *hxc_l_tmp_instance_call_result_n1230 = hxc_caxecraft_domain_GameSession_worldView(hxc_l_tmp_load_result_n1229, &hxc_l_tmp_length_n1231);
        double hxc_l_tmp_record_field_load_result_n1232 = hxc_l_interactionView.hxc_interactionOriginX;
        double hxc_l_tmp_record_field_load_result_n1233 = hxc_l_interactionView.hxc_interactionOriginY;
        double hxc_l_tmp_record_field_load_result_n1234 = hxc_l_interactionView.hxc_interactionOriginZ;
        double hxc_l_tmp_record_field_load_result_n1235 = hxc_l_interactionView.hxc_interactionDirectionX;
        double hxc_l_tmp_record_field_load_result_n1236 = hxc_l_interactionView.hxc_interactionDirectionY;
        struct hxc_caxecraft_domain_RaycastHit hxc_l_tmp_call_result_n1238 = hxc_caxecraft_domain_VoxelRaycast_trace(hxc_l_tmp_instance_call_result_n1230, hxc_l_tmp_length_n1231, hxc_l_tmp_record_field_load_result_n1232, hxc_l_tmp_record_field_load_result_n1233, hxc_l_tmp_record_field_load_result_n1234, hxc_l_tmp_record_field_load_result_n1235, hxc_l_tmp_record_field_load_result_n1236, hxc_l_interactionView.hxc_interactionDirectionZ, 7.0);
        struct hxc_caxecraft_domain_RaycastHit hxc_l_hit = hxc_l_tmp_call_result_n1238;
        bool hxc_l_tmp_call_result_n1240 = hxc_caxecraft_app_AppScreen_capturesPointer(hxc_l_screen);
        bool hxc_l_tmp_short_circuit_result_n384 = hxc_l_tmp_call_result_n1240;
        if (hxc_l_tmp_call_result_n1240)
        {
          hxc_l_tmp_short_circuit_result_n384 = !hxc_l_conversation.hxc_has_value;
        }
        bool hxc_l_tmp_short_circuit_load_result_n1242 = hxc_l_tmp_short_circuit_result_n384;
        bool hxc_l_tmp_short_circuit_result_n385 = hxc_l_tmp_short_circuit_load_result_n1242;
        if (hxc_l_tmp_short_circuit_load_result_n1242)
        {
          hxc_l_tmp_short_circuit_result_n385 = !hxc_l_conversationOwnedInput;
        }
        bool hxc_l_tmp_short_circuit_load_result_n1244 = hxc_l_tmp_short_circuit_result_n385;
        bool hxc_l_tmp_short_circuit_result_n386 = hxc_l_tmp_short_circuit_load_result_n1244;
        if (hxc_l_tmp_short_circuit_load_result_n1244)
        {
          hxc_l_tmp_short_circuit_result_n386 = !hxc_l_recapturedThisFrame;
        }
        bool hxc_l_tmp_short_circuit_load_result_n1246 = hxc_l_tmp_short_circuit_result_n386;
        bool hxc_l_tmp_short_circuit_result_n387 = hxc_l_tmp_short_circuit_load_result_n1246;
        if (hxc_l_tmp_short_circuit_load_result_n1246)
        {
          hxc_l_tmp_short_circuit_result_n387 = hxc_l_primaryPressed;
        }
        if (hxc_l_tmp_short_circuit_result_n387)
        {
          if (hxc_l_character.hxc_vitals.hxc_health > 0)
          {
            if (hxc_l_selectedMode == 1)
            {
              bool hxc_l_tmp_call_result_n1253 = hxc_caxecraft_gameplay_Inventory_selectedIs(hxc_l_inventory, hxc_caxecraft_gameplay_ItemKind_CopperSword);
              bool hxc_l_tmp_short_circuit_result_n388 = !hxc_l_tmp_call_result_n1253;
              if (!hxc_l_tmp_call_result_n1253)
              {
                hxc_l_tmp_short_circuit_result_n388 = hxc_l_hit.hxc_hit;
              }
              bool hxc_l_tmp_short_circuit_load_result_n1255 = hxc_l_tmp_short_circuit_result_n388;
              bool hxc_l_tmp_short_circuit_result_n389 = hxc_l_tmp_short_circuit_load_result_n1255;
              if (hxc_l_tmp_short_circuit_load_result_n1255)
              {
                struct hxc_caxecraft_domain_AquaticState hxc_l_tmp_record_field_load_result_n1256 = hxc_l_character.hxc_aquatic;
                bool hxc_l_tmp_call_result_n1258 = hxc_caxecraft_domain_Aquatics_canMine(hxc_l_tmp_record_field_load_result_n1256, hxc_l_character.hxc_aquaticProfile);
                hxc_l_tmp_short_circuit_result_n389 = hxc_l_tmp_call_result_n1258;
              }
              if (hxc_l_tmp_short_circuit_result_n389)
              {
                int32_t hxc_l_tmp_record_field_load_result_n1260 = hxc_l_hit.hxc_cellX;
                int32_t hxc_l_tmp_record_field_load_result_n1261 = hxc_l_hit.hxc_cellY;
                struct hxc_caxecraft_scenario_VoxelPoint hxc_l_tmp_call_result_n1263 = hxc_caxecraft_domain_World_coord(hxc_l_tmp_record_field_load_result_n1260, hxc_l_tmp_record_field_load_result_n1261, hxc_l_hit.hxc_cellZ);
                struct hxc_caxecraft_scenario_VoxelPoint hxc_l_minedCoordinate = hxc_l_tmp_call_result_n1263;
                int32_t hxc_l_sandBefore = hxc_l_inventory.hxc_sand;
                (void)hxc_l_sandBefore;
                struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1265 = hxc_l_session;
                hxc_l_gc_roots[522] = (const void *)hxc_l_tmp_load_result_n1265;
                if (hxc_l_tmp_load_result_n1265 == NULL)
                {
                  abort();
                }
                struct hxc_caxecraft_scenario_VoxelPoint hxc_l_tmp_load_result_n1266 = hxc_l_minedCoordinate;
                struct hxc_caxecraft_gameplay_MiningResult hxc_l_tmp_instance_call_result_n1268 = hxc_caxecraft_domain_GameSession_mineTerrain(hxc_l_tmp_load_result_n1265, hxc_l_tmp_load_result_n1266, hxc_l_inventory);
                struct hxc_caxecraft_gameplay_MiningResult hxc_l_mining = hxc_l_tmp_instance_call_result_n1268;
                hxc_l_inventory = hxc_l_mining.hxc_inventory;
                if (hxc_l_mining.hxc_outcome == 2)
                {
                  if (hxc_l_self == NULL)
                  {
                    abort();
                  }
                  struct hxc_caxecraft_app_TerrainRenderer *hxc_l_tmp_owned_class_field_address_n1271 = &(*hxc_l_self).hxc_terrainRenderer;
                  struct hxc_caxecraft_app_TerrainChunkCache *hxc_l_tmp_owned_class_field_address_n1272 = &(*hxc_l_tmp_owned_class_field_address_n1271).hxc_cache;
                  if (hxc_l_tmp_owned_class_field_address_n1272 == NULL)
                  {
                    abort();
                  }
                  hxc_caxecraft_app_TerrainChunkCache_invalidate(hxc_l_tmp_owned_class_field_address_n1272, hxc_l_minedCoordinate);
                }
                if (hxc_l_mining.hxc_outcome == 1)
                {
                  hxc_l_inventoryFullReason = 2;
                  hxc_l_inventoryFullFrames = 90;
                }
                if (hxc_l_mining.hxc_outcome == 3)
                {
                  hxc_l_quit = true;
                }
              }
            }
            else
            {
              if (hxc_l_hit.hxc_hit)
              {
                int32_t hxc_l_tmp_record_field_load_result_n1277 = hxc_l_hit.hxc_cellX;
                int32_t hxc_l_tmp_record_field_load_result_n1278 = hxc_l_hit.hxc_cellY;
                struct hxc_caxecraft_scenario_VoxelPoint hxc_l_tmp_call_result_n1280 = hxc_caxecraft_domain_World_coord(hxc_l_tmp_record_field_load_result_n1277, hxc_l_tmp_record_field_load_result_n1278, hxc_l_hit.hxc_cellZ);
                struct hxc_caxecraft_scenario_VoxelPoint hxc_l_removedCoordinate = hxc_l_tmp_call_result_n1280;
                struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1281 = hxc_l_session;
                hxc_l_gc_roots[523] = (const void *)hxc_l_tmp_load_result_n1281;
                if (hxc_l_tmp_load_result_n1281 == NULL)
                {
                  abort();
                }
                bool hxc_l_tmp_instance_call_result_n1283 = hxc_caxecraft_domain_GameSession_removeTerrain(hxc_l_tmp_load_result_n1281, hxc_l_removedCoordinate);
                bool hxc_l_removed = hxc_l_tmp_instance_call_result_n1283;
                if (hxc_l_removed)
                {
                  if (hxc_l_self == NULL)
                  {
                    abort();
                  }
                  struct hxc_caxecraft_app_TerrainRenderer *hxc_l_tmp_owned_class_field_address_n1285 = &(*hxc_l_self).hxc_terrainRenderer;
                  struct hxc_caxecraft_app_TerrainChunkCache *hxc_l_tmp_owned_class_field_address_n1286 = &(*hxc_l_tmp_owned_class_field_address_n1285).hxc_cache;
                  if (hxc_l_tmp_owned_class_field_address_n1286 == NULL)
                  {
                    abort();
                  }
                  hxc_caxecraft_app_TerrainChunkCache_invalidate(hxc_l_tmp_owned_class_field_address_n1286, hxc_l_removedCoordinate);
                }
              }
            }
          }
        }
        bool hxc_l_tmp_call_result_n1289 = hxc_caxecraft_app_AppScreen_capturesPointer(hxc_l_screen);
        bool hxc_l_tmp_short_circuit_result_n395 = hxc_l_tmp_call_result_n1289;
        if (hxc_l_tmp_call_result_n1289)
        {
          hxc_l_tmp_short_circuit_result_n395 = !hxc_l_conversation.hxc_has_value;
        }
        bool hxc_l_tmp_short_circuit_load_result_n1291 = hxc_l_tmp_short_circuit_result_n395;
        bool hxc_l_tmp_short_circuit_result_n396 = hxc_l_tmp_short_circuit_load_result_n1291;
        if (hxc_l_tmp_short_circuit_load_result_n1291)
        {
          hxc_l_tmp_short_circuit_result_n396 = !hxc_l_conversationOwnedInput;
        }
        bool hxc_l_tmp_short_circuit_load_result_n1293 = hxc_l_tmp_short_circuit_result_n396;
        bool hxc_l_tmp_short_circuit_result_n397 = hxc_l_tmp_short_circuit_load_result_n1293;
        if (hxc_l_tmp_short_circuit_load_result_n1293)
        {
          hxc_l_tmp_short_circuit_result_n397 = hxc_l_secondaryPressed;
        }
        if (hxc_l_tmp_short_circuit_result_n397)
        {
          if (hxc_l_character.hxc_vitals.hxc_health > 0)
          {
            struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_tmp_load_result_n1297 = hxc_l_contentRegistry;
            hxc_l_gc_roots[524] = (const void *)hxc_l_tmp_load_result_n1297;
            enum hxc_caxecraft_gameplay_ItemKind hxc_l_tmp_call_result_n1299 = hxc_caxecraft_gameplay_Inventory_itemAt(hxc_l_inventory.hxc_selected);
            hxc_string hxc_l_tmp_call_result_n1300 = hxc_caxecraft_app_RuntimeInventoryBinding_runtimeItemContentIdForInventoryKind(hxc_l_tmp_load_result_n1297, hxc_l_tmp_call_result_n1299);
            hxc_l_selectedItemType = hxc_l_tmp_call_result_n1300;
            struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1301 = hxc_l_session;
            hxc_l_gc_roots[525] = (const void *)hxc_l_tmp_load_result_n1301;
            if (hxc_l_tmp_load_result_n1301 == NULL)
            {
              abort();
            }
            struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_load_result_n1302 = hxc_l_inventory;
            struct hxc_caxecraft_domain_LocalRecoveryResult hxc_l_tmp_instance_call_result_n1304 = hxc_caxecraft_domain_GameSession_useSelectedRecovery(hxc_l_tmp_load_result_n1301, hxc_l_tmp_load_result_n1302, hxc_l_selectedItemType);
            struct hxc_caxecraft_domain_LocalRecoveryResult hxc_l_recovery = hxc_l_tmp_instance_call_result_n1304;
            hxc_l_character = hxc_l_recovery.hxc_character;
            if (!hxc_l_recovery.hxc_resolved)
            {
              hxc_l_quit = true;
            }
            else
            {
              if (hxc_l_recovery.hxc_decision != 0)
              {
                hxc_l_recoveryFeedback = hxc_l_recovery.hxc_decision;
                hxc_l_recoveryFeedbackFrames = 90;
                hxc_l_inventory = hxc_l_recovery.hxc_inventory;
              }
              else
              {
                if (hxc_l_hit.hxc_hit)
                {
                  int32_t hxc_l_tmp_record_field_load_result_n1311 = hxc_l_hit.hxc_previousX;
                  int32_t hxc_l_tmp_record_field_load_result_n1312 = hxc_l_hit.hxc_previousY;
                  struct hxc_caxecraft_scenario_VoxelPoint hxc_l_tmp_call_result_n1314 = hxc_caxecraft_domain_World_coord(hxc_l_tmp_record_field_load_result_n1311, hxc_l_tmp_record_field_load_result_n1312, hxc_l_hit.hxc_previousZ);
                  struct hxc_caxecraft_scenario_VoxelPoint hxc_l_placement = hxc_l_tmp_call_result_n1314;
                  enum hxc_caxecraft_domain_BlockKind hxc_l_tmp_call_result_n1316 = hxc_caxecraft_gameplay_Inventory_selectedBlock(hxc_l_inventory);
                  enum hxc_caxecraft_domain_BlockKind hxc_l_selectedBlock = hxc_l_tmp_call_result_n1316;
                  struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_load_result_n1317 = hxc_l_inventory;
                  int32_t hxc_l_tmp_call_result_n1319 = hxc_caxecraft_gameplay_Inventory_countAt(hxc_l_tmp_load_result_n1317, hxc_l_inventory.hxc_selected);
                  bool hxc_l_hasItem = hxc_l_tmp_call_result_n1319 > 0;
                  int32_t hxc_l_tmp_load_result_n1320 = hxc_l_selectedMode;
                  hxc_string hxc_l_tmp_conditional_managed_result_n404 = { 0 };
                  if (hxc_l_tmp_load_result_n1320 == 1)
                  {
                    (void)hxc_l_selectedItemType;
                    hxc_l_tmp_conditional_managed_result_n404 = hxc_l_selectedItemType;
                    if (hxc_string_retain(hxc_l_tmp_conditional_managed_result_n404) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                  }
                  else
                  {
                    hxc_l_tmp_conditional_managed_result_n404 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
                    if (hxc_string_retain(hxc_l_tmp_conditional_managed_result_n404) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                  }
                  hxc_string hxc_l_tmp_conditional_managed_move_result_n1322 = hxc_l_tmp_conditional_managed_result_n404;
                  hxc_l_placementItemType = hxc_l_tmp_conditional_managed_move_result_n1322;
                  bool hxc_l_tmp_load_result_n1323 = hxc_l_hasItem;
                  bool hxc_l_tmp_short_circuit_result_n405 = !hxc_l_tmp_load_result_n1323;
                  if (!!hxc_l_tmp_load_result_n1323)
                  {
                    bool hxc_l_tmp_call_result_n1325 = hxc_caxecraft_domain_World_isPlaceable(hxc_l_selectedBlock);
                    hxc_l_tmp_short_circuit_result_n405 = !hxc_l_tmp_call_result_n1325;
                  }
                  bool hxc_l_tmp_short_circuit_load_result_n1326 = hxc_l_tmp_short_circuit_result_n405;
                  bool hxc_l_tmp_short_circuit_result_n406 = hxc_l_tmp_short_circuit_load_result_n1326;
                  if (!hxc_l_tmp_short_circuit_load_result_n1326)
                  {
                    struct hxc_caxecraft_domain_CharacterBody hxc_l_tmp_record_field_load_result_n1327 = hxc_l_character.hxc_body;
                    bool hxc_l_tmp_call_result_n1329 = hxc_caxecraft_domain_CharacterPhysics_canPlaceAt(hxc_l_tmp_record_field_load_result_n1327, hxc_l_placement);
                    hxc_l_tmp_short_circuit_result_n406 = !hxc_l_tmp_call_result_n1329;
                  }
                  bool hxc_l_tmp_short_circuit_load_result_n1330 = hxc_l_tmp_short_circuit_result_n406;
                  bool hxc_l_tmp_short_circuit_result_n407 = hxc_l_tmp_short_circuit_load_result_n1330;
                  if (!hxc_l_tmp_short_circuit_load_result_n1330)
                  {
                    int32_t hxc_l_tmp_load_result_n1331 = hxc_l_selectedMode;
                    bool hxc_l_tmp_short_circuit_result_n408 = hxc_l_tmp_load_result_n1331 == 1;
                    if (hxc_l_tmp_load_result_n1331 == 1)
                    {
                      bool hxc_l_tmp_string_null_equality_result_n409 = hxc_l_placementItemType.data == NULL;
                      hxc_l_tmp_short_circuit_result_n408 = hxc_l_tmp_string_null_equality_result_n409;
                    }
                    hxc_l_tmp_short_circuit_result_n407 = hxc_l_tmp_short_circuit_result_n408;
                  }
                  bool hxc_l_tmp_short_circuit_load_result_n1335 = hxc_l_tmp_short_circuit_result_n407;
                  bool hxc_l_tmp_short_circuit_result_n410 = hxc_l_tmp_short_circuit_load_result_n1335;
                  if (!hxc_l_tmp_short_circuit_load_result_n1335)
                  {
                    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1336 = hxc_l_session;
                    hxc_l_gc_roots[526] = (const void *)hxc_l_tmp_load_result_n1336;
                    if (hxc_l_tmp_load_result_n1336 == NULL)
                    {
                      abort();
                    }
                    struct hxc_caxecraft_scenario_VoxelPoint hxc_l_tmp_load_result_n1337 = hxc_l_placement;
                    enum hxc_caxecraft_domain_BlockKind hxc_l_tmp_load_result_n1338 = hxc_l_selectedBlock;
                    bool hxc_l_tmp_instance_call_result_n1340 = hxc_caxecraft_domain_GameSession_placeTerrain(hxc_l_tmp_load_result_n1336, hxc_l_tmp_load_result_n1337, hxc_l_tmp_load_result_n1338, hxc_l_placementItemType);
                    hxc_l_tmp_short_circuit_result_n410 = !hxc_l_tmp_instance_call_result_n1340;
                  }
                  if (hxc_l_tmp_short_circuit_result_n410)
                  {
                    hxc_l_placementBlockedFrames = 60;
                  }
                  else
                  {
                    if (hxc_l_self == NULL)
                    {
                      abort();
                    }
                    struct hxc_caxecraft_app_TerrainRenderer *hxc_l_tmp_owned_class_field_address_n1342 = &(*hxc_l_self).hxc_terrainRenderer;
                    struct hxc_caxecraft_app_TerrainChunkCache *hxc_l_tmp_owned_class_field_address_n1343 = &(*hxc_l_tmp_owned_class_field_address_n1342).hxc_cache;
                    if (hxc_l_tmp_owned_class_field_address_n1343 == NULL)
                    {
                      abort();
                    }
                    hxc_caxecraft_app_TerrainChunkCache_invalidate(hxc_l_tmp_owned_class_field_address_n1343, hxc_l_placement);
                    if (hxc_l_selectedMode == 1)
                    {
                      struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n1347 = hxc_caxecraft_gameplay_Inventory_consumeSelected(hxc_l_inventory);
                      hxc_l_inventory = hxc_l_tmp_call_result_n1347;
                    }
                  }
                  if (hxc_string_release(&hxc_l_placementItemType) != HXC_STATUS_OK)
                  {
                    abort();
                  }
                }
              }
            }
            if (hxc_string_release(&hxc_l_selectedItemType) != HXC_STATUS_OK)
            {
              abort();
            }
          }
        }
        if (hxc_l_placementBlockedFrames > 0)
        {
          hxc_l_placementBlockedFrames = hxc_i32_subtract_wrapping(hxc_l_placementBlockedFrames, 1);
        }
        bool hxc_l_tmp_call_result_n1351 = hxc_caxecraft_app_AppScreen_pausesSimulation(hxc_l_screen);
        if (!hxc_l_tmp_call_result_n1351)
        {
          struct hxc_caxecraft_gameplay_BerryDropState hxc_l_tmp_load_result_n1352 = hxc_l_berryDrop;
          double hxc_l_tmp_record_field_load_result_n1353 = hxc_l_character.hxc_body.hxc_x;
          double hxc_l_tmp_record_field_load_result_n1354 = hxc_l_character.hxc_body.hxc_y;
          bool hxc_l_tmp_call_result_n1356 = hxc_caxecraft_gameplay_BerryDrop_isInRange(hxc_l_tmp_load_result_n1352, hxc_l_tmp_record_field_load_result_n1353, hxc_l_tmp_record_field_load_result_n1354, hxc_l_character.hxc_body.hxc_z);
          if (hxc_l_tmp_call_result_n1356)
          {
            struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_load_result_n1357 = hxc_l_inventory;
            int32_t hxc_l_tmp_call_result_n1360 = hxc_caxecraft_gameplay_Inventory_acceptedAmount(hxc_l_tmp_load_result_n1357, hxc_caxecraft_gameplay_ItemKind_Berries, hxc_l_berryDrop.hxc_amount);
            int32_t hxc_l_acceptedDrop = hxc_l_tmp_call_result_n1360;
            if (hxc_l_acceptedDrop > 0)
            {
              struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_load_result_n1362 = hxc_l_inventory;
              struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n1365 = hxc_caxecraft_gameplay_Inventory_collectItem(hxc_l_tmp_load_result_n1362, hxc_caxecraft_gameplay_ItemKind_Berries, hxc_l_acceptedDrop);
              hxc_l_inventory = hxc_l_tmp_call_result_n1365;
              struct hxc_caxecraft_gameplay_BerryDropState hxc_l_tmp_load_result_n1366 = hxc_l_berryDrop;
              struct hxc_caxecraft_gameplay_BerryDropState hxc_l_tmp_call_result_n1368 = hxc_caxecraft_gameplay_BerryDrop_collectAmount(hxc_l_tmp_load_result_n1366, hxc_l_acceptedDrop);
              hxc_l_berryDrop = hxc_l_tmp_call_result_n1368;
              hxc_l_pickupAmount = hxc_l_acceptedDrop;
              hxc_l_pickupFrames = 90;
            }
            else
            {
              hxc_l_inventoryFullReason = 1;
              hxc_l_inventoryFullFrames = 90;
            }
          }
        }
        if (hxc_l_strikeHitFrames > 0)
        {
          hxc_l_strikeHitFrames = hxc_i32_subtract_wrapping(hxc_l_strikeHitFrames, 1);
        }
        if (hxc_l_enemyDefeatedFrames > 0)
        {
          hxc_l_enemyDefeatedFrames = hxc_i32_subtract_wrapping(hxc_l_enemyDefeatedFrames, 1);
        }
        if (hxc_l_enemyAttackFrames > 0)
        {
          hxc_l_enemyAttackFrames = hxc_i32_subtract_wrapping(hxc_l_enemyAttackFrames, 1);
        }
        if (hxc_l_pickupFrames > 0)
        {
          hxc_l_pickupFrames = hxc_i32_subtract_wrapping(hxc_l_pickupFrames, 1);
        }
        if (hxc_l_inventoryFullFrames > 0)
        {
          hxc_l_inventoryFullFrames = hxc_i32_subtract_wrapping(hxc_l_inventoryFullFrames, 1);
          if (hxc_l_inventoryFullFrames == 0)
          {
            hxc_l_inventoryFullReason = 0;
          }
        }
        if (hxc_l_recoveryFeedbackFrames > 0)
        {
          hxc_l_recoveryFeedbackFrames = hxc_i32_subtract_wrapping(hxc_l_recoveryFeedbackFrames, 1);
        }
        if (hxc_l_aquaticEquipmentFrames > 0)
        {
          hxc_l_aquaticEquipmentFrames = hxc_i32_subtract_wrapping(hxc_l_aquaticEquipmentFrames, 1);
        }
        struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1385 = hxc_l_session;
        hxc_l_gc_roots[527] = (const void *)hxc_l_tmp_load_result_n1385;
        if (hxc_l_tmp_load_result_n1385 == NULL)
        {
          abort();
        }
        struct hxc_caxecraft_domain_GameView hxc_l_tmp_instance_call_result_n1386 = hxc_caxecraft_domain_GameSession_view(hxc_l_tmp_load_result_n1385);
        struct hxc_caxecraft_domain_GameView hxc_l_committedView = hxc_l_tmp_instance_call_result_n1386;
        if (!hxc_l_committedView.hxc_valid)
        {
          hxc_l_quit = true;
        }
        else
        {
          hxc_l_character = hxc_l_committedView.hxc_localPlayer;
        }
        struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1389 = hxc_l_session;
        hxc_l_gc_roots[528] = (const void *)hxc_l_tmp_load_result_n1389;
        hxc_l_gc_roots[529] = (const void *)hxc_l_levelView_h37960af8bef4;
        struct hxc_caxecraft_app_CaxecraftApp_AvailableInteractionTarget hxc_l_tmp_call_result_n1391 = hxc_caxecraft_app_CaxecraftApp_nearestAvailableInteraction(hxc_l_tmp_load_result_n1389, hxc_l_levelView_h37960af8bef4);
        hxc_l_tmp_static_call_argument_0_owner_n413 = hxc_l_tmp_call_result_n1391;
        enum hxc_caxecraft_app_InteractionPrompt hxc_l_tmp_call_result_n1393 = hxc_caxecraft_app_CaxecraftApp_promptForAvailableInteraction(hxc_l_tmp_static_call_argument_0_owner_n413);
        hxc_l_availableInteractionPrompt = hxc_l_tmp_call_result_n1393;
        struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1394 = hxc_l_session;
        hxc_l_gc_roots[530] = (const void *)hxc_l_tmp_load_result_n1394;
        if (hxc_l_tmp_load_result_n1394 == NULL)
        {
          abort();
        }
        struct hxc_array_ref *hxc_l_tmp_instance_call_result_n1395 = hxc_caxecraft_domain_GameSession_actorControllerStateSnapshots(hxc_l_tmp_load_result_n1394);
        hxc_l_presentationActorPhases = hxc_l_tmp_instance_call_result_n1395;
        struct hxc_array_ref *hxc_l_tmp_load_result_n1396 = hxc_l_presentationActorPhases;
        struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation hxc_l_tmp_call_result_n1399 = hxc_caxecraft_app_CaxecraftApp_observeActorPhase(hxc_l_tmp_load_result_n1396, hxc_l_enemyActorId, hxc_caxecraft_domain_ActorControllerPhase_Defeated);
        hxc_l_enemyPhase_h906f9c33685e = hxc_l_tmp_call_result_n1399;
        struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1400 = hxc_l_session;
        hxc_l_gc_roots[531] = (const void *)hxc_l_tmp_load_result_n1400;
        if (hxc_l_tmp_load_result_n1400 == NULL)
        {
          abort();
        }
        struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n1402 = hxc_caxecraft_domain_GameSession_readCharacter(hxc_l_tmp_load_result_n1400, hxc_l_enemyActorId);
        hxc_l_enemyActor = hxc_l_tmp_instance_call_result_n1402;
        struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1403 = hxc_l_session;
        hxc_l_gc_roots[532] = (const void *)hxc_l_tmp_load_result_n1403;
        struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1404 = hxc_l_levelView_h37960af8bef4;
        hxc_l_gc_roots[533] = (const void *)hxc_l_tmp_load_result_n1404;
        bool hxc_l_tmp_call_result_n1406 = hxc_caxecraft_app_CaxecraftApp_dialogueActorsAreValid(hxc_l_tmp_load_result_n1403, hxc_l_tmp_load_result_n1404, hxc_l_presentationActorPhases);
        bool hxc_l_tmp_short_circuit_result_n415 = !hxc_l_tmp_call_result_n1406;
        if (!!hxc_l_tmp_call_result_n1406)
        {
          hxc_l_tmp_short_circuit_result_n415 = !hxc_l_enemyPhase_h906f9c33685e.hxc_valid;
        }
        bool hxc_l_tmp_short_circuit_load_result_n1408 = hxc_l_tmp_short_circuit_result_n415;
        bool hxc_l_tmp_short_circuit_result_n416 = hxc_l_tmp_short_circuit_load_result_n1408;
        if (!hxc_l_tmp_short_circuit_load_result_n1408)
        {
          hxc_l_tmp_short_circuit_result_n416 = hxc_l_enemyActor.hxc_id <= 0;
        }
        if (hxc_l_tmp_short_circuit_result_n416)
        {
          hxc_l_quit = true;
        }
        struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1411 = hxc_l_session;
        hxc_l_gc_roots[534] = (const void *)hxc_l_tmp_load_result_n1411;
        hxc_l_gc_roots[535] = (const void *)hxc_l_levelView_h37960af8bef4;
        struct hxc_array_ref *hxc_l_tmp_call_result_n1413 = hxc_caxecraft_app_CaxecraftApp_dialogueActorSnapshots(hxc_l_tmp_load_result_n1411, hxc_l_levelView_h37960af8bef4);
        hxc_l_dialogueActors = hxc_l_tmp_call_result_n1413;
        int32_t hxc_l_completedTicks = hxc_l_committedView.hxc_completedTicks;
        if (hxc_l_resetMotionThisFrame)
        {
          struct hxc_caxecraft_app_MotionHistory hxc_l_tmp_call_result_n1417 = hxc_caxecraft_app_MotionInterpolation_start(hxc_l_character.hxc_body);
          hxc_l_motionHistory = hxc_l_tmp_call_result_n1417;
        }
        struct hxc_caxecraft_app_MotionHistory hxc_l_tmp_load_result_n1418 = hxc_l_motionHistory;
        struct hxc_caxecraft_app_RenderPosition hxc_l_tmp_call_result_n1420 = hxc_caxecraft_app_MotionInterpolation_sample(hxc_l_tmp_load_result_n1418, hxc_l_accumulator, 0.05);
        struct hxc_caxecraft_app_RenderPosition hxc_l_renderPosition = hxc_l_tmp_call_result_n1420;
        double hxc_l_tmp_record_field_load_result_n1421 = hxc_l_renderPosition.hxc_x;
        double hxc_l_tmp_record_field_load_result_n1422 = hxc_l_renderPosition.hxc_y;
        struct hxc_caxecraft_domain_CharacterBody hxc_l_tmp_call_result_n1424 = hxc_caxecraft_domain_CharacterPhysics_body(hxc_l_tmp_record_field_load_result_n1421, hxc_l_tmp_record_field_load_result_n1422, hxc_l_renderPosition.hxc_z);
        struct hxc_caxecraft_domain_CharacterBody hxc_l_presentationBody = hxc_l_tmp_call_result_n1424;
        struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1425 = hxc_l_session;
        hxc_l_gc_roots[536] = (const void *)hxc_l_tmp_load_result_n1425;
        if (hxc_l_tmp_load_result_n1425 == NULL)
        {
          abort();
        }
        enum hxc_caxecraft_domain_PlayerCameraMode hxc_l_tmp_load_result_n1426 = hxc_l_cameraMode;
        struct hxc_caxecraft_domain_CharacterBody hxc_l_tmp_load_result_n1427 = hxc_l_presentationBody;
        double hxc_l_tmp_load_result_n1428 = hxc_l_lookX;
        double hxc_l_tmp_load_result_n1429 = hxc_l_lookY;
        struct hxc_caxecraft_domain_PlayerCameraView hxc_l_tmp_instance_call_result_n1431 = hxc_caxecraft_domain_GameSession_playerCamera(hxc_l_tmp_load_result_n1425, hxc_l_tmp_load_result_n1426, hxc_l_tmp_load_result_n1427, hxc_l_tmp_load_result_n1428, hxc_l_tmp_load_result_n1429, hxc_l_lookZ);
        struct hxc_caxecraft_domain_PlayerCameraView hxc_l_cameraView = hxc_l_tmp_instance_call_result_n1431;
        double hxc_l_eyeX = hxc_l_cameraView.hxc_positionX;
        double hxc_l_eyeY = hxc_l_cameraView.hxc_positionY;
        double hxc_l_eyeZ = hxc_l_cameraView.hxc_positionZ;
        double hxc_l_tmp_load_result_n1435 = hxc_l_eyeX;
        double hxc_l_tmp_load_result_n1436 = hxc_l_eyeY;
        struct Vector3 hxc_l_this1_h07c01f6f3cda = (struct Vector3){ .x = (float)hxc_l_tmp_load_result_n1435, .y = (float)hxc_l_tmp_load_result_n1436, .z = (float)hxc_l_eyeZ };
        double hxc_l_y_hf2989bbe3cce = hxc_l_cameraView.hxc_targetY;
        double hxc_l_z_ha5824b896e42 = hxc_l_cameraView.hxc_targetZ;
        struct Vector3 hxc_l_tmp_load_result_n1441 = hxc_l_this1_h07c01f6f3cda;
        double hxc_l_tmp_record_field_load_result_n1442 = hxc_l_cameraView.hxc_targetX;
        double hxc_l_tmp_load_result_n1443 = hxc_l_y_hf2989bbe3cce;
        struct Camera3D hxc_l_camera = (struct Camera3D){ .position = hxc_l_tmp_load_result_n1441, .target = (struct Vector3){ .x = (float)hxc_l_tmp_record_field_load_result_n1442, .y = (float)hxc_l_tmp_load_result_n1443, .z = (float)hxc_l_z_ha5824b896e42 }, .up = (struct Vector3){ .x = (float)0.0, .y = (float)1.0, .z = (float)0.0 }, .fovy = (float)70.0, .projection = 0 };
        BeginDrawing();
        if (hxc_l_onTitle)
        {
          hxc_string hxc_l_locale1_hab7686f0e894 = { 0 };
          switch (hxc_l_locale) {
            case 0:
              {
                hxc_l_locale1_hab7686f0e894 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
                if (hxc_string_retain(hxc_l_locale1_hab7686f0e894) != HXC_STATUS_OK)
                {
                  abort();
                }
                break;
              }
            case 1:
              {
                hxc_l_locale1_hab7686f0e894 = (hxc_string){ (const uint8_t *)"es-mx", 5, true, NULL };
                if (hxc_string_retain(hxc_l_locale1_hab7686f0e894) != HXC_STATUS_OK)
                {
                  abort();
                }
                break;
              }
            default:
              {
                hxc_l_locale1_hab7686f0e894 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
                if (hxc_string_retain(hxc_l_locale1_hab7686f0e894) != HXC_STATUS_OK)
                {
                  abort();
                }
                break;
              }
          }
          hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n1450 = hxc_l_locale1_hab7686f0e894;
          hxc_l_tmp_managed_flow_owner_n430 = hxc_l_tmp_managed_flow_carrier_move_result_n1450;
          struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1451 = hxc_l_levelView_h37960af8bef4;
          hxc_l_gc_roots[537] = (const void *)hxc_l_tmp_load_result_n1451;
          if (hxc_l_tmp_load_result_n1451 == NULL)
          {
            abort();
          }
          struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_class_field_load_result_n1452 = (*hxc_l_tmp_load_result_n1451).hxc_levelPresentation;
          hxc_l_gc_roots[538] = (const void *)hxc_l_tmp_class_field_load_result_n1452;
          if (hxc_l_tmp_class_field_load_result_n1452 == NULL)
          {
            abort();
          }
          hxc_string hxc_l_tmp_instance_call_result_n1454 = hxc_caxecraft_content_RuntimeLevelPresentation_message(hxc_l_tmp_class_field_load_result_n1452, (hxc_string){ (const uint8_t *)"adventure_tagline", 17, true, NULL }, hxc_l_tmp_managed_flow_owner_n430);
          hxc_l_adventureTagline = hxc_l_tmp_instance_call_result_n1454;
          int32_t hxc_l_tmp_native_call_result_n1455 = GetScreenWidth();
          int32_t hxc_l_width_h0664c42da507 = (int32_t)hxc_l_tmp_native_call_result_n1455;
          int32_t hxc_l_tmp_native_call_result_n1456 = GetScreenHeight();
          int32_t hxc_l_height_h155d0e361ffc = (int32_t)hxc_l_tmp_native_call_result_n1456;
          if (hxc_l_titleTextureReady)
          {
            struct Color hxc_l_this1_hd2354924dcc9 = (struct Color){ .r = 255, .g = 255, .b = 255, .a = 255 };
            struct Texture hxc_l_tmp_load_result_n1459 = hxc_l_titleTexture;
            double hxc_l_imageAspect = hxc_f64_divide_zero_safe((double)(int32_t)hxc_l_tmp_load_result_n1459.width, (double)(int32_t)hxc_l_titleTexture.height);
            int32_t hxc_l_tmp_load_result_n1463 = hxc_l_width_h0664c42da507;
            double hxc_l_screenAspect = hxc_f64_divide_zero_safe((double)hxc_l_tmp_load_result_n1463, (double)hxc_l_height_h155d0e361ffc);
            double hxc_l_sourceX = 0.0;
            double hxc_l_sourceY = 0.0;
            double hxc_l_sourceWidth = (double)(int32_t)hxc_l_titleTexture.width;
            double hxc_l_sourceHeight = (double)(int32_t)hxc_l_titleTexture.height;
            double hxc_l_tmp_load_result_n1469 = hxc_l_screenAspect;
            if (hxc_l_tmp_load_result_n1469 > hxc_l_imageAspect)
            {
              struct Texture hxc_l_tmp_load_result_n1471 = hxc_l_titleTexture;
              hxc_l_sourceHeight = hxc_f64_divide_zero_safe((double)(int32_t)hxc_l_tmp_load_result_n1471.width, hxc_l_screenAspect);
              struct Texture hxc_l_tmp_load_result_n1474 = hxc_l_titleTexture;
              hxc_l_sourceY = ((double)(int32_t)hxc_l_tmp_load_result_n1474.height - hxc_l_sourceHeight) * 0.5;
            }
            else
            {
              struct Texture hxc_l_tmp_load_result_n1477 = hxc_l_titleTexture;
              hxc_l_sourceWidth = (double)(int32_t)hxc_l_tmp_load_result_n1477.height * hxc_l_screenAspect;
              struct Texture hxc_l_tmp_load_result_n1480 = hxc_l_titleTexture;
              hxc_l_sourceX = ((double)(int32_t)hxc_l_tmp_load_result_n1480.width - hxc_l_sourceWidth) * 0.5;
            }
            struct Texture hxc_l_tmp_load_result_n1483 = hxc_l_titleTexture;
            double hxc_l_tmp_load_result_n1484 = hxc_l_sourceX;
            double hxc_l_tmp_load_result_n1485 = hxc_l_sourceY;
            double hxc_l_tmp_load_result_n1486 = hxc_l_sourceWidth;
            double hxc_l_tmp_load_result_n1487 = hxc_l_sourceHeight;
            int32_t hxc_l_tmp_load_result_n1489 = hxc_l_width_h0664c42da507;
            int32_t hxc_l_tmp_load_result_n1490 = hxc_l_height_h155d0e361ffc;
            DrawTexturePro(hxc_l_tmp_load_result_n1483, (struct Rectangle){ .x = (float)hxc_l_tmp_load_result_n1484, .y = (float)hxc_l_tmp_load_result_n1485, .width = (float)hxc_l_tmp_load_result_n1486, .height = (float)hxc_l_tmp_load_result_n1487 }, (struct Rectangle){ .x = (float)0.0, .y = (float)0.0, .width = (float)((double)hxc_l_tmp_load_result_n1489 + 0.0), .height = (float)((double)hxc_l_tmp_load_result_n1490 + 0.0) }, (struct Vector2){ .x = (float)0.0, .y = (float)0.0 }, (float)0.0, hxc_l_this1_hd2354924dcc9);
          }
          else
          {
            ClearBackground((struct Color){ .r = 126, .g = 190, .b = 201, .a = 255 });
          }
          struct Color hxc_l_this1_h714e8f5d1e94 = (struct Color){ .r = 4, .g = 14, .b = 23, .a = 132 };
          int32_t hxc_l_tmp_load_result_n1496 = hxc_l_width_h0664c42da507;
          int32_t hxc_l_tmp_load_result_n1497 = hxc_l_height_h155d0e361ffc;
          DrawRectangle((int32_t)0, (int32_t)0, (int32_t)hxc_l_tmp_load_result_n1496, (int32_t)hxc_l_tmp_load_result_n1497, hxc_l_this1_h714e8f5d1e94);
          if (hxc_l_wordmarkTextureReady)
          {
            int32_t hxc_l_centerX = hxc_l_width_h0664c42da507 / 2;
            struct Color hxc_l_this1_h3dd4334b8a53 = (struct Color){ .r = 255, .g = 255, .b = 255, .a = 255 };
            double hxc_l_scale = hxc_f64_divide_zero_safe((double)560, (double)(int32_t)hxc_l_wordmarkTexture.width);
            double hxc_l_heightScale = hxc_f64_divide_zero_safe((double)132, (double)(int32_t)hxc_l_wordmarkTexture.height);
            double hxc_l_tmp_load_result_n1506 = hxc_l_heightScale;
            if (hxc_l_tmp_load_result_n1506 < hxc_l_scale)
            {
              hxc_l_scale = hxc_l_heightScale;
            }
            struct Texture hxc_l_tmp_load_result_n1509 = hxc_l_wordmarkTexture;
            double hxc_l_width_h7f9ef7ee4a54 = (double)(int32_t)hxc_l_tmp_load_result_n1509.width * hxc_l_scale;
            struct Texture hxc_l_tmp_load_result_n1512 = hxc_l_wordmarkTexture;
            double hxc_l_height_hff04b47300a6 = (double)(int32_t)hxc_l_tmp_load_result_n1512.height * hxc_l_scale;
            double hxc_l_width1 = (double)(int32_t)hxc_l_wordmarkTexture.width + 0.0;
            double hxc_l_height1 = (double)(int32_t)hxc_l_wordmarkTexture.height + 0.0;
            struct Texture hxc_l_tmp_load_result_n1519 = hxc_l_wordmarkTexture;
            double hxc_l_tmp_load_result_n1520 = hxc_l_width1;
            double hxc_l_tmp_load_result_n1521 = hxc_l_height1;
            int32_t hxc_l_tmp_load_result_n1523 = hxc_l_centerX;
            double hxc_l_tmp_load_result_n1524 = hxc_l_width_h7f9ef7ee4a54;
            double hxc_l_tmp_load_result_n1525 = hxc_l_width_h7f9ef7ee4a54;
            double hxc_l_tmp_load_result_n1526 = hxc_l_height_hff04b47300a6;
            DrawTexturePro(hxc_l_tmp_load_result_n1519, (struct Rectangle){ .x = (float)0.0, .y = (float)0.0, .width = (float)hxc_l_tmp_load_result_n1520, .height = (float)hxc_l_tmp_load_result_n1521 }, (struct Rectangle){ .x = (float)((double)hxc_l_tmp_load_result_n1523 - hxc_l_tmp_load_result_n1524 * 0.5), .y = (float)28.0, .width = (float)hxc_l_tmp_load_result_n1525, .height = (float)hxc_l_tmp_load_result_n1526 }, (struct Vector2){ .x = (float)0.0, .y = (float)0.0 }, (float)0.0, hxc_l_this1_h3dd4334b8a53);
          }
          else
          {
            int32_t hxc_l_x_hfb0a47dd4a1d = hxc_i32_subtract_wrapping(hxc_l_width_h0664c42da507 / 2, 92);
            struct Color hxc_l_this1_haa9780155458 = (struct Color){ .r = 242, .g = 249, .b = 245, .a = 255 };
            struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_tmp_load_result_n1532 = hxc_l_uiCatalog;
            hxc_l_gc_roots[539] = (const void *)hxc_l_tmp_load_result_n1532;
            if (hxc_l_tmp_load_result_n1532 == NULL)
            {
              abort();
            }
            hxc_string hxc_l_tmp_instance_call_result_n1534 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_tmp_load_result_n1532, hxc_l_locale, (hxc_string){ (const uint8_t *)"title_fallback", 14, true, NULL });
            hxc_l_tmp_cstring_arg_owner_owner_n452 = hxc_l_tmp_instance_call_result_n1534;
            hxc_string hxc_l_tmp_cstring_arg_owner_borrow_result_n1535 = hxc_l_tmp_cstring_arg_owner_owner_n452;
            int32_t hxc_l_tmp_load_result_n1536 = hxc_l_x_hfb0a47dd4a1d;
            struct Color hxc_l_tmp_load_result_n1537 = hxc_l_this1_haa9780155458;
            hxc_call_cstring hxc_l_tmp_string_prepare_cstring_result_n1538 = HXC_CALL_CSTRING_INITIALIZER;
            if (hxc_string_prepare_call_cstring(&hxc_l_tmp_cstring_arg_owner_borrow_result_n1535, hxc_default_allocator(), &hxc_l_tmp_string_prepare_cstring_result_n1538) != HXC_STATUS_OK)
            {
              abort();
            }
            DrawText(hxc_l_tmp_string_prepare_cstring_result_n1538.data, (int32_t)hxc_l_tmp_load_result_n1536, (int32_t)52, (int32_t)34, hxc_l_tmp_load_result_n1537);
            if (hxc_call_cstring_dispose(&hxc_l_tmp_string_prepare_cstring_result_n1538) != HXC_STATUS_OK)
            {
              abort();
            }
            if (hxc_string_release(&hxc_l_tmp_cstring_arg_owner_owner_n452) != HXC_STATUS_OK)
            {
              abort();
            }
          }
          int32_t hxc_l_firstTop = hxc_i32_subtract_wrapping(hxc_l_height_h155d0e361ffc, 293);
          int32_t hxc_l_left_habbc889e8ffb = hxc_i32_subtract_wrapping(hxc_l_width_h0664c42da507 / 2, 180);
          if (hxc_l_selectedMode == 0)
          {
            struct Color hxc_l_this1_h818136b2fe33 = (struct Color){ .r = 16, .g = 88, .b = 102, .a = 232 };
            int32_t hxc_l_tmp_load_result_n1543 = hxc_l_left_habbc889e8ffb;
            int32_t hxc_l_tmp_load_result_n1544 = hxc_l_firstTop;
            DrawRectangle((int32_t)hxc_l_tmp_load_result_n1543, (int32_t)hxc_l_tmp_load_result_n1544, (int32_t)360, (int32_t)54, hxc_l_this1_h818136b2fe33);
            struct Color hxc_l_this1_he55e91849291 = (struct Color){ .r = 255, .g = 190, .b = 55, .a = 255 };
            int32_t hxc_l_tmp_load_result_n1547 = hxc_l_left_habbc889e8ffb;
            int32_t hxc_l_tmp_load_result_n1548 = hxc_l_firstTop;
            DrawRectangleLines((int32_t)hxc_l_tmp_load_result_n1547, (int32_t)hxc_l_tmp_load_result_n1548, (int32_t)360, (int32_t)54, hxc_l_this1_he55e91849291);
          }
          else
          {
            struct Color hxc_l_this1_h3e87160666df = (struct Color){ .r = 6, .g = 26, .b = 36, .a = 218 };
            int32_t hxc_l_tmp_load_result_n1551 = hxc_l_left_habbc889e8ffb;
            int32_t hxc_l_tmp_load_result_n1552 = hxc_l_firstTop;
            DrawRectangle((int32_t)hxc_l_tmp_load_result_n1551, (int32_t)hxc_l_tmp_load_result_n1552, (int32_t)360, (int32_t)54, hxc_l_this1_h3e87160666df);
            struct Color hxc_l_this1_h7e1a11413540 = (struct Color){ .r = 92, .g = 194, .b = 188, .a = 255 };
            int32_t hxc_l_tmp_load_result_n1555 = hxc_l_left_habbc889e8ffb;
            int32_t hxc_l_tmp_load_result_n1556 = hxc_l_firstTop;
            DrawRectangleLines((int32_t)hxc_l_tmp_load_result_n1555, (int32_t)hxc_l_tmp_load_result_n1556, (int32_t)360, (int32_t)54, hxc_l_this1_h7e1a11413540);
          }
          struct Color hxc_l_this1_h05f74e8d7c08 = (struct Color){ .r = 242, .g = 249, .b = 245, .a = 255 };
          struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_tmp_load_result_n1559 = hxc_l_uiCatalog;
          hxc_l_gc_roots[540] = (const void *)hxc_l_tmp_load_result_n1559;
          if (hxc_l_tmp_load_result_n1559 == NULL)
          {
            abort();
          }
          hxc_string hxc_l_tmp_instance_call_result_n1561 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_tmp_load_result_n1559, hxc_l_locale, (hxc_string){ (const uint8_t *)"menu_creative", 13, true, NULL });
          hxc_l_tmp_cstring_arg_owner_owner_n460 = hxc_l_tmp_instance_call_result_n1561;
          hxc_string hxc_l_tmp_cstring_arg_owner_borrow_result_n1562 = hxc_l_tmp_cstring_arg_owner_owner_n460;
          int32_t hxc_l_tmp_load_result_n1563 = hxc_l_left_habbc889e8ffb;
          int32_t hxc_l_tmp_load_result_n1564 = hxc_l_firstTop;
          struct Color hxc_l_tmp_load_result_n1565 = hxc_l_this1_h05f74e8d7c08;
          hxc_call_cstring hxc_l_tmp_string_prepare_cstring_result_n1566 = HXC_CALL_CSTRING_INITIALIZER;
          if (hxc_string_prepare_call_cstring(&hxc_l_tmp_cstring_arg_owner_borrow_result_n1562, hxc_default_allocator(), &hxc_l_tmp_string_prepare_cstring_result_n1566) != HXC_STATUS_OK)
          {
            abort();
          }
          DrawText(hxc_l_tmp_string_prepare_cstring_result_n1566.data, (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n1563, 28), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n1564, 16), (int32_t)21, hxc_l_tmp_load_result_n1565);
          if (hxc_call_cstring_dispose(&hxc_l_tmp_string_prepare_cstring_result_n1566) != HXC_STATUS_OK)
          {
            abort();
          }
          int32_t hxc_l_top_h51ca93fcbede = hxc_i32_add_wrapping(hxc_i32_add_wrapping(hxc_l_firstTop, 54), 14);
          int32_t hxc_l_left_h21b4164a7854 = hxc_i32_subtract_wrapping(hxc_l_width_h0664c42da507 / 2, 180);
          if (hxc_l_selectedMode == 1)
          {
            struct Color hxc_l_this1_h71138cc6223b = (struct Color){ .r = 16, .g = 88, .b = 102, .a = 232 };
            int32_t hxc_l_tmp_load_result_n1571 = hxc_l_left_h21b4164a7854;
            int32_t hxc_l_tmp_load_result_n1572 = hxc_l_top_h51ca93fcbede;
            DrawRectangle((int32_t)hxc_l_tmp_load_result_n1571, (int32_t)hxc_l_tmp_load_result_n1572, (int32_t)360, (int32_t)54, hxc_l_this1_h71138cc6223b);
            struct Color hxc_l_this1_h58513c7c7269 = (struct Color){ .r = 255, .g = 190, .b = 55, .a = 255 };
            int32_t hxc_l_tmp_load_result_n1575 = hxc_l_left_h21b4164a7854;
            int32_t hxc_l_tmp_load_result_n1576 = hxc_l_top_h51ca93fcbede;
            DrawRectangleLines((int32_t)hxc_l_tmp_load_result_n1575, (int32_t)hxc_l_tmp_load_result_n1576, (int32_t)360, (int32_t)54, hxc_l_this1_h58513c7c7269);
          }
          else
          {
            struct Color hxc_l_this1_ha567ec608778 = (struct Color){ .r = 6, .g = 26, .b = 36, .a = 218 };
            int32_t hxc_l_tmp_load_result_n1579 = hxc_l_left_h21b4164a7854;
            int32_t hxc_l_tmp_load_result_n1580 = hxc_l_top_h51ca93fcbede;
            DrawRectangle((int32_t)hxc_l_tmp_load_result_n1579, (int32_t)hxc_l_tmp_load_result_n1580, (int32_t)360, (int32_t)54, hxc_l_this1_ha567ec608778);
            struct Color hxc_l_this1_hbacbc2e567f7 = (struct Color){ .r = 92, .g = 194, .b = 188, .a = 255 };
            int32_t hxc_l_tmp_load_result_n1583 = hxc_l_left_h21b4164a7854;
            int32_t hxc_l_tmp_load_result_n1584 = hxc_l_top_h51ca93fcbede;
            DrawRectangleLines((int32_t)hxc_l_tmp_load_result_n1583, (int32_t)hxc_l_tmp_load_result_n1584, (int32_t)360, (int32_t)54, hxc_l_this1_hbacbc2e567f7);
          }
          struct Color hxc_l_this1_ha6175af6df83 = (struct Color){ .r = 242, .g = 249, .b = 245, .a = 255 };
          struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_tmp_load_result_n1587 = hxc_l_uiCatalog;
          hxc_l_gc_roots[541] = (const void *)hxc_l_tmp_load_result_n1587;
          if (hxc_l_tmp_load_result_n1587 == NULL)
          {
            abort();
          }
          hxc_string hxc_l_tmp_instance_call_result_n1589 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_tmp_load_result_n1587, hxc_l_locale, (hxc_string){ (const uint8_t *)"menu_adventure", 14, true, NULL });
          hxc_l_tmp_cstring_arg_owner_owner_n468 = hxc_l_tmp_instance_call_result_n1589;
          hxc_string hxc_l_tmp_cstring_arg_owner_borrow_result_n1590 = hxc_l_tmp_cstring_arg_owner_owner_n468;
          int32_t hxc_l_tmp_load_result_n1591 = hxc_l_left_h21b4164a7854;
          int32_t hxc_l_tmp_load_result_n1592 = hxc_l_top_h51ca93fcbede;
          struct Color hxc_l_tmp_load_result_n1593 = hxc_l_this1_ha6175af6df83;
          hxc_call_cstring hxc_l_tmp_string_prepare_cstring_result_n1594 = HXC_CALL_CSTRING_INITIALIZER;
          if (hxc_string_prepare_call_cstring(&hxc_l_tmp_cstring_arg_owner_borrow_result_n1590, hxc_default_allocator(), &hxc_l_tmp_string_prepare_cstring_result_n1594) != HXC_STATUS_OK)
          {
            abort();
          }
          DrawText(hxc_l_tmp_string_prepare_cstring_result_n1594.data, (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n1591, 28), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n1592, 16), (int32_t)21, hxc_l_tmp_load_result_n1593);
          if (hxc_call_cstring_dispose(&hxc_l_tmp_string_prepare_cstring_result_n1594) != HXC_STATUS_OK)
          {
            abort();
          }
          int32_t hxc_l_top_h9dac91fbb12b = hxc_i32_add_wrapping(hxc_l_firstTop, 136);
          int32_t hxc_l_left_hee82ba90fcf5 = hxc_i32_subtract_wrapping(hxc_l_width_h0664c42da507 / 2, 180);
          struct Color hxc_l_this1_h2cad555d764c = (struct Color){ .r = 6, .g = 26, .b = 36, .a = 218 };
          int32_t hxc_l_tmp_load_result_n1598 = hxc_l_left_hee82ba90fcf5;
          int32_t hxc_l_tmp_load_result_n1599 = hxc_l_top_h9dac91fbb12b;
          DrawRectangle((int32_t)hxc_l_tmp_load_result_n1598, (int32_t)hxc_l_tmp_load_result_n1599, (int32_t)360, (int32_t)54, hxc_l_this1_h2cad555d764c);
          struct Color hxc_l_this1_h0626e9b0afed = (struct Color){ .r = 92, .g = 194, .b = 188, .a = 255 };
          int32_t hxc_l_tmp_load_result_n1602 = hxc_l_left_hee82ba90fcf5;
          int32_t hxc_l_tmp_load_result_n1603 = hxc_l_top_h9dac91fbb12b;
          DrawRectangleLines((int32_t)hxc_l_tmp_load_result_n1602, (int32_t)hxc_l_tmp_load_result_n1603, (int32_t)360, (int32_t)54, hxc_l_this1_h0626e9b0afed);
          struct Color hxc_l_this1_h7b20de195c48 = (struct Color){ .r = 242, .g = 249, .b = 245, .a = 255 };
          struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_tmp_load_result_n1606 = hxc_l_uiCatalog;
          hxc_l_gc_roots[542] = (const void *)hxc_l_tmp_load_result_n1606;
          if (hxc_l_tmp_load_result_n1606 == NULL)
          {
            abort();
          }
          hxc_string hxc_l_tmp_instance_call_result_n1608 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_tmp_load_result_n1606, hxc_l_locale, (hxc_string){ (const uint8_t *)"menu_editor", 11, true, NULL });
          hxc_l_tmp_cstring_arg_owner_owner_n474 = hxc_l_tmp_instance_call_result_n1608;
          hxc_string hxc_l_tmp_cstring_arg_owner_borrow_result_n1609 = hxc_l_tmp_cstring_arg_owner_owner_n474;
          int32_t hxc_l_tmp_load_result_n1610 = hxc_l_left_hee82ba90fcf5;
          int32_t hxc_l_tmp_load_result_n1611 = hxc_l_top_h9dac91fbb12b;
          struct Color hxc_l_tmp_load_result_n1612 = hxc_l_this1_h7b20de195c48;
          hxc_call_cstring hxc_l_tmp_string_prepare_cstring_result_n1613 = HXC_CALL_CSTRING_INITIALIZER;
          if (hxc_string_prepare_call_cstring(&hxc_l_tmp_cstring_arg_owner_borrow_result_n1609, hxc_default_allocator(), &hxc_l_tmp_string_prepare_cstring_result_n1613) != HXC_STATUS_OK)
          {
            abort();
          }
          DrawText(hxc_l_tmp_string_prepare_cstring_result_n1613.data, (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n1610, 28), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n1611, 16), (int32_t)21, hxc_l_tmp_load_result_n1612);
          if (hxc_call_cstring_dispose(&hxc_l_tmp_string_prepare_cstring_result_n1613) != HXC_STATUS_OK)
          {
            abort();
          }
          hxc_string hxc_l_tmp_call_result_n1615 = hxc_caxecraft_input_ControlPrompts_menuInstructionsMessage(hxc_l_promptDevice);
          hxc_l_message_h7f14c8a2a95a = hxc_l_tmp_call_result_n1615;
          int32_t hxc_l_x_h4adcc5111e0c = hxc_i32_subtract_wrapping(hxc_l_width_h0664c42da507 / 2, 330);
          struct Color hxc_l_this1_h0043448f89aa = (struct Color){ .r = 229, .g = 241, .b = 235, .a = 255 };
          struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_tmp_load_result_n1618 = hxc_l_uiCatalog;
          hxc_l_gc_roots[543] = (const void *)hxc_l_tmp_load_result_n1618;
          if (hxc_l_tmp_load_result_n1618 == NULL)
          {
            abort();
          }
          int32_t hxc_l_tmp_load_result_n1619 = hxc_l_locale;
          hxc_string hxc_l_tmp_instance_call_result_n1621 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_tmp_load_result_n1618, hxc_l_tmp_load_result_n1619, hxc_l_message_h7f14c8a2a95a);
          hxc_l_tmp_cstring_arg_owner_owner_n478 = hxc_l_tmp_instance_call_result_n1621;
          hxc_string hxc_l_tmp_cstring_arg_owner_borrow_result_n1622 = hxc_l_tmp_cstring_arg_owner_owner_n478;
          int32_t hxc_l_tmp_load_result_n1623 = hxc_l_x_h4adcc5111e0c;
          int32_t hxc_l_tmp_load_result_n1624 = hxc_l_height_h155d0e361ffc;
          struct Color hxc_l_tmp_load_result_n1625 = hxc_l_this1_h0043448f89aa;
          hxc_call_cstring hxc_l_tmp_string_prepare_cstring_result_n1626 = HXC_CALL_CSTRING_INITIALIZER;
          if (hxc_string_prepare_call_cstring(&hxc_l_tmp_cstring_arg_owner_borrow_result_n1622, hxc_default_allocator(), &hxc_l_tmp_string_prepare_cstring_result_n1626) != HXC_STATUS_OK)
          {
            abort();
          }
          DrawText(hxc_l_tmp_string_prepare_cstring_result_n1626.data, (int32_t)hxc_l_tmp_load_result_n1623, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n1624, 58), (int32_t)16, hxc_l_tmp_load_result_n1625);
          if (hxc_call_cstring_dispose(&hxc_l_tmp_string_prepare_cstring_result_n1626) != HXC_STATUS_OK)
          {
            abort();
          }
          hxc_string hxc_l_tmp_call_result_n1628 = hxc_caxecraft_input_ControlPrompts_controlsMessage(hxc_l_promptDevice);
          hxc_l_message_h1254a0b8dc08 = hxc_l_tmp_call_result_n1628;
          struct Color hxc_l_this1_hebc1e4aa1407 = (struct Color){ .r = 142, .g = 210, .b = 205, .a = 255 };
          struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_tmp_load_result_n1630 = hxc_l_uiCatalog;
          hxc_l_gc_roots[544] = (const void *)hxc_l_tmp_load_result_n1630;
          if (hxc_l_tmp_load_result_n1630 == NULL)
          {
            abort();
          }
          int32_t hxc_l_tmp_load_result_n1631 = hxc_l_locale;
          hxc_string hxc_l_tmp_instance_call_result_n1633 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_tmp_load_result_n1630, hxc_l_tmp_load_result_n1631, hxc_l_message_h1254a0b8dc08);
          hxc_l_tmp_cstring_arg_owner_owner_n481 = hxc_l_tmp_instance_call_result_n1633;
          hxc_string hxc_l_tmp_cstring_arg_owner_borrow_result_n1634 = hxc_l_tmp_cstring_arg_owner_owner_n481;
          int32_t hxc_l_tmp_load_result_n1635 = hxc_l_height_h155d0e361ffc;
          struct Color hxc_l_tmp_load_result_n1636 = hxc_l_this1_hebc1e4aa1407;
          hxc_call_cstring hxc_l_tmp_string_prepare_cstring_result_n1637 = HXC_CALL_CSTRING_INITIALIZER;
          if (hxc_string_prepare_call_cstring(&hxc_l_tmp_cstring_arg_owner_borrow_result_n1634, hxc_default_allocator(), &hxc_l_tmp_string_prepare_cstring_result_n1637) != HXC_STATUS_OK)
          {
            abort();
          }
          DrawText(hxc_l_tmp_string_prepare_cstring_result_n1637.data, (int32_t)18, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n1635, 30), (int32_t)13, hxc_l_tmp_load_result_n1636);
          if (hxc_call_cstring_dispose(&hxc_l_tmp_string_prepare_cstring_result_n1637) != HXC_STATUS_OK)
          {
            abort();
          }
          int32_t hxc_l_tmp_load_result_n1638 = hxc_l_selectedMode;
          bool hxc_l_tmp_short_circuit_result_n482 = hxc_l_tmp_load_result_n1638 == 1;
          if (hxc_l_tmp_load_result_n1638 == 1)
          {
            hxc_string hxc_l_tmp_load_result_n1639 = hxc_l_adventureTagline;
            if (hxc_l_tmp_load_result_n1639.data == NULL)
            {
              abort();
            }
            int32_t hxc_l_tmp_string_length_result_n1640;
            if (hxc_string_haxe_length(hxc_l_tmp_load_result_n1639, &hxc_l_tmp_string_length_result_n1640) != HXC_STATUS_OK)
            {
              abort();
            }
            hxc_l_tmp_short_circuit_result_n482 = hxc_l_tmp_string_length_result_n1640 > 0;
          }
          if (hxc_l_tmp_short_circuit_result_n482)
          {
            int32_t hxc_l_x_hf9863d60a525 = hxc_i32_subtract_wrapping(hxc_l_width_h0664c42da507 / 2, 230);
            struct Color hxc_l_this1_h795baea2f3d6 = (struct Color){ .r = 255, .g = 205, .b = 91, .a = 255 };
            hxc_string hxc_l_tmp_load_result_n1644 = hxc_l_adventureTagline;
            int32_t hxc_l_tmp_load_result_n1645 = hxc_l_x_hf9863d60a525;
            int32_t hxc_l_tmp_load_result_n1646 = hxc_l_firstTop;
            struct Color hxc_l_tmp_load_result_n1647 = hxc_l_this1_h795baea2f3d6;
            hxc_call_cstring hxc_l_tmp_string_prepare_cstring_result_n1648 = HXC_CALL_CSTRING_INITIALIZER;
            if (hxc_string_prepare_call_cstring(&hxc_l_tmp_load_result_n1644, hxc_default_allocator(), &hxc_l_tmp_string_prepare_cstring_result_n1648) != HXC_STATUS_OK)
            {
              abort();
            }
            DrawText(hxc_l_tmp_string_prepare_cstring_result_n1648.data, (int32_t)hxc_l_tmp_load_result_n1645, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n1646, 36), (int32_t)17, hxc_l_tmp_load_result_n1647);
            if (hxc_call_cstring_dispose(&hxc_l_tmp_string_prepare_cstring_result_n1648) != HXC_STATUS_OK)
            {
              abort();
            }
          }
          if (hxc_string_release(&hxc_l_tmp_cstring_arg_owner_owner_n481) != HXC_STATUS_OK)
          {
            abort();
          }
          if (hxc_string_release(&hxc_l_message_h1254a0b8dc08) != HXC_STATUS_OK)
          {
            abort();
          }
          if (hxc_string_release(&hxc_l_tmp_cstring_arg_owner_owner_n478) != HXC_STATUS_OK)
          {
            abort();
          }
          if (hxc_string_release(&hxc_l_message_h7f14c8a2a95a) != HXC_STATUS_OK)
          {
            abort();
          }
          if (hxc_string_release(&hxc_l_tmp_cstring_arg_owner_owner_n474) != HXC_STATUS_OK)
          {
            abort();
          }
          if (hxc_string_release(&hxc_l_tmp_cstring_arg_owner_owner_n468) != HXC_STATUS_OK)
          {
            abort();
          }
          if (hxc_string_release(&hxc_l_tmp_cstring_arg_owner_owner_n460) != HXC_STATUS_OK)
          {
            abort();
          }
          if (hxc_string_release(&hxc_l_adventureTagline) != HXC_STATUS_OK)
          {
            abort();
          }
          if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n430) != HXC_STATUS_OK)
          {
            abort();
          }
        }
        else
        {
          if (hxc_l_onCampaignSelect)
          {
            hxc_l_gc_roots[545] = (const void *)hxc_l_campaign;
            struct hxc_caxecraft_content_CampaignManifest *hxc_l_selectedCampaign_h02a372a88efc = hxc_l_campaign;
            hxc_l_gc_roots[546] = (const void *)hxc_l_selectedCampaign_h02a372a88efc;
            hxc_l_gc_roots[547] = (const void *)NULL;
            if (hxc_l_selectedCampaign_h02a372a88efc == NULL)
            {
              enum hxc_caxecraft_app_AppScreen hxc_l_tmp_call_result_n1653 = hxc_caxecraft_app_AppScreen_closeCampaignSelection(hxc_l_screen);
              hxc_l_screen = hxc_l_tmp_call_result_n1653;
            }
            else
            {
              hxc_string hxc_l_locale1_h610d33e6e2e1 = { 0 };
              switch (hxc_l_locale) {
                case 0:
                  {
                    hxc_l_locale1_h610d33e6e2e1 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
                    if (hxc_string_retain(hxc_l_locale1_h610d33e6e2e1) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    break;
                  }
                case 1:
                  {
                    hxc_l_locale1_h610d33e6e2e1 = (hxc_string){ (const uint8_t *)"es-mx", 5, true, NULL };
                    if (hxc_string_retain(hxc_l_locale1_h610d33e6e2e1) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    break;
                  }
                default:
                  {
                    hxc_l_locale1_h610d33e6e2e1 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
                    if (hxc_string_retain(hxc_l_locale1_h610d33e6e2e1) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    break;
                  }
              }
              hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n1655 = hxc_l_locale1_h610d33e6e2e1;
              hxc_l_tmp_managed_flow_owner_n487 = hxc_l_tmp_managed_flow_carrier_move_result_n1655;
              struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1656 = hxc_l_levelView_h37960af8bef4;
              hxc_l_gc_roots[548] = (const void *)hxc_l_tmp_load_result_n1656;
              if (hxc_l_tmp_load_result_n1656 == NULL)
              {
                abort();
              }
              struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_class_field_load_result_n1657 = (*hxc_l_tmp_load_result_n1656).hxc_levelPresentation;
              hxc_l_gc_roots[549] = (const void *)hxc_l_tmp_class_field_load_result_n1657;
              struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_this_haa66807e3a96 = hxc_l_tmp_class_field_load_result_n1657;
              struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_load_result_n1658 = hxc_l_this_haa66807e3a96;
              hxc_l_gc_roots[550] = (const void *)hxc_l_tmp_load_result_n1658;
              if (hxc_l_tmp_load_result_n1658 == NULL)
              {
                abort();
              }
              struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_load_result_n1659 = hxc_l_this_haa66807e3a96;
              hxc_l_gc_roots[551] = (const void *)hxc_l_tmp_load_result_n1659;
              if (hxc_l_tmp_load_result_n1659 == NULL)
              {
                abort();
              }
              struct hxc_caxecraft_scenario_ScenarioText hxc_l_tmp_class_field_load_result_n1660 = (*hxc_l_tmp_load_result_n1659).hxc_title;
              hxc_string hxc_l_tmp_instance_call_result_n1662 = hxc_caxecraft_content_RuntimeLevelPresentation_resolve(hxc_l_tmp_load_result_n1658, hxc_l_tmp_class_field_load_result_n1660, hxc_l_tmp_managed_flow_owner_n487);
              hxc_l_tmp = hxc_l_tmp_instance_call_result_n1662;
              hxc_string hxc_l_locale1_h6d75ca83f465 = { 0 };
              switch (hxc_l_locale) {
                case 0:
                  {
                    hxc_l_locale1_h6d75ca83f465 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
                    if (hxc_string_retain(hxc_l_locale1_h6d75ca83f465) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    break;
                  }
                case 1:
                  {
                    hxc_l_locale1_h6d75ca83f465 = (hxc_string){ (const uint8_t *)"es-mx", 5, true, NULL };
                    if (hxc_string_retain(hxc_l_locale1_h6d75ca83f465) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    break;
                  }
                default:
                  {
                    hxc_l_locale1_h6d75ca83f465 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
                    if (hxc_string_retain(hxc_l_locale1_h6d75ca83f465) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    break;
                  }
              }
              hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n1664 = hxc_l_locale1_h6d75ca83f465;
              hxc_l_tmp_managed_flow_owner_n491 = hxc_l_tmp_managed_flow_carrier_move_result_n1664;
              struct Texture hxc_l_tmp_load_result_n1665 = hxc_l_titleTexture;
              bool hxc_l_tmp_load_result_n1666 = hxc_l_titleTextureReady;
              struct Texture hxc_l_tmp_load_result_n1667 = hxc_l_wordmarkTexture;
              bool hxc_l_tmp_load_result_n1668 = hxc_l_wordmarkTextureReady;
              struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n1669 = hxc_l_selectedCampaign_h02a372a88efc;
              hxc_l_gc_roots[552] = (const void *)hxc_l_tmp_load_result_n1669;
              int32_t hxc_l_tmp_load_result_n1670 = hxc_l_locale;
              struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_tmp_load_result_n1671 = hxc_l_uiCatalog;
              hxc_l_gc_roots[553] = (const void *)hxc_l_tmp_load_result_n1671;
              int32_t hxc_l_tmp_load_result_n1672 = hxc_l_selectedCampaignLevelIndex;
              hxc_string hxc_l_tmp_load_result_n1673 = hxc_l_tmp;
              struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1674 = hxc_l_levelView_h37960af8bef4;
              hxc_l_gc_roots[554] = (const void *)hxc_l_tmp_load_result_n1674;
              if (hxc_l_tmp_load_result_n1674 == NULL)
              {
                abort();
              }
              struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_class_field_load_result_n1675 = (*hxc_l_tmp_load_result_n1674).hxc_levelPresentation;
              hxc_l_gc_roots[555] = (const void *)hxc_l_tmp_class_field_load_result_n1675;
              if (hxc_l_tmp_class_field_load_result_n1675 == NULL)
              {
                abort();
              }
              hxc_string hxc_l_tmp_instance_call_result_n1677 = hxc_caxecraft_content_RuntimeLevelPresentation_message(hxc_l_tmp_class_field_load_result_n1675, (hxc_string){ (const uint8_t *)"adventure_tagline", 17, true, NULL }, hxc_l_tmp_managed_flow_owner_n491);
              hxc_l_tmp_static_call_argument_9_owner_n492 = hxc_l_tmp_instance_call_result_n1677;
              hxc_string hxc_l_tmp_static_call_argument_9_borrow_result_n1678 = hxc_l_tmp_static_call_argument_9_owner_n492;
              hxc_caxecraft_app_CampaignMenu_draw(hxc_l_tmp_load_result_n1665, hxc_l_tmp_load_result_n1666, hxc_l_tmp_load_result_n1667, hxc_l_tmp_load_result_n1668, hxc_l_tmp_load_result_n1669, hxc_l_tmp_load_result_n1670, hxc_l_tmp_load_result_n1671, hxc_l_tmp_load_result_n1672, hxc_l_tmp_load_result_n1673, hxc_l_tmp_static_call_argument_9_borrow_result_n1678, hxc_l_promptDevice);
              if (hxc_string_release(&hxc_l_tmp_static_call_argument_9_owner_n492) != HXC_STATUS_OK)
              {
                abort();
              }
              if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n491) != HXC_STATUS_OK)
              {
                abort();
              }
              if (hxc_string_release(&hxc_l_tmp) != HXC_STATUS_OK)
              {
                abort();
              }
              if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n487) != HXC_STATUS_OK)
              {
                abort();
              }
            }
          }
          else
          {
            if (hxc_l_onLoading)
            {
              hxc_string hxc_l_tmp_load_result_n1681 = hxc_l_pendingCampaignLabel;
              int32_t hxc_l_tmp_load_result_n1682 = hxc_l_locale;
              hxc_l_gc_roots[556] = (const void *)hxc_l_uiCatalog;
              hxc_caxecraft_app_CaxecraftApp_drawCampaignLoading(hxc_l_tmp_load_result_n1681, hxc_l_tmp_load_result_n1682, hxc_l_uiCatalog);
            }
            else
            {
              if (hxc_l_onEditor)
              {
                struct hxc_caxecraft_app_CaxecraftEditorScreen *hxc_l_tmp_load_result_n1685 = hxc_l_editorScreen;
                hxc_l_gc_roots[557] = (const void *)hxc_l_tmp_load_result_n1685;
                if (hxc_l_tmp_load_result_n1685 == NULL)
                {
                  abort();
                }
                int32_t hxc_l_tmp_load_result_n1686 = hxc_l_locale;
                int32_t hxc_l_tmp_load_result_n1687 = hxc_l_editorNavigationCommand;
                struct hxc_caxecraft_app_HudResources hxc_l_tmp_load_result_n1688 = hxc_l_hudResources;
                struct Texture hxc_l_tmp_load_result_n1689 = hxc_l_terrainTexture;
                bool hxc_l_tmp_load_result_n1690 = hxc_l_terrainTextureReady;
                struct Texture hxc_l_tmp_load_result_n1691 = hxc_l_adventureTerrainTexture;
                bool hxc_l_tmp_load_result_n1692 = hxc_l_adventureTerrainTextureReady;
                hxc_l_gc_roots[558] = (const void *)hxc_l_runtimeTextures;
                hxc_l_gc_roots[559] = (const void *)(struct hxc_caxecraft_app_EditorRenderResources){ .hxc_adventureTerrainTexture = hxc_l_tmp_load_result_n1691, .hxc_adventureTerrainTextureReady = hxc_l_tmp_load_result_n1692, .hxc_runtimeTextures = hxc_l_runtimeTextures, .hxc_shared = hxc_l_tmp_load_result_n1688, .hxc_terrainTexture = hxc_l_tmp_load_result_n1689, .hxc_terrainTextureReady = hxc_l_tmp_load_result_n1690 }.hxc_runtimeTextures;
                struct hxc_caxecraft_app_EditorScreenAction hxc_l_tmp_instance_call_result_n1695 = hxc_caxecraft_app_CaxecraftEditorScreen_draw(hxc_l_tmp_load_result_n1685, hxc_l_tmp_load_result_n1686, hxc_l_tmp_load_result_n1687, (struct hxc_caxecraft_app_EditorRenderResources){ .hxc_adventureTerrainTexture = hxc_l_tmp_load_result_n1691, .hxc_adventureTerrainTextureReady = hxc_l_tmp_load_result_n1692, .hxc_runtimeTextures = hxc_l_runtimeTextures, .hxc_shared = hxc_l_tmp_load_result_n1688, .hxc_terrainTexture = hxc_l_tmp_load_result_n1689, .hxc_terrainTextureReady = hxc_l_tmp_load_result_n1690 });
                hxc_enum_59188943_destroy(&hxc_l_editorActionForFrame);
                hxc_l_editorActionForFrame = hxc_l_tmp_instance_call_result_n1695;
              }
              else
              {
                struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1696 = hxc_l_levelView_h37960af8bef4;
                hxc_l_gc_roots[560] = (const void *)hxc_l_tmp_load_result_n1696;
                if (hxc_l_tmp_load_result_n1696 == NULL)
                {
                  abort();
                }
                struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_class_field_load_result_n1697 = (*hxc_l_tmp_load_result_n1696).hxc_levelPresentation;
                hxc_l_gc_roots[561] = (const void *)hxc_l_tmp_class_field_load_result_n1697;
                if (hxc_l_tmp_class_field_load_result_n1697 == NULL)
                {
                  abort();
                }
                bool hxc_l_tmp_class_field_load_result_n1698 = (*hxc_l_tmp_class_field_load_result_n1697).hxc_environmentAuthored;
                bool hxc_l_hasEnvironment = hxc_l_tmp_class_field_load_result_n1698;
                struct Color hxc_l_color = { 0 };
                if (hxc_l_hasEnvironment)
                {
                  struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1700 = hxc_l_levelView_h37960af8bef4;
                  hxc_l_gc_roots[562] = (const void *)hxc_l_tmp_load_result_n1700;
                  if (hxc_l_tmp_load_result_n1700 == NULL)
                  {
                    abort();
                  }
                  struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_class_field_load_result_n1701 = (*hxc_l_tmp_load_result_n1700).hxc_levelPresentation;
                  hxc_l_gc_roots[563] = (const void *)hxc_l_tmp_class_field_load_result_n1701;
                  if (hxc_l_tmp_class_field_load_result_n1701 == NULL)
                  {
                    abort();
                  }
                  int32_t hxc_l_tmp_class_field_load_result_n1702 = (*hxc_l_tmp_class_field_load_result_n1701).hxc_environmentSkyRedValue;
                  int32_t hxc_l_r = hxc_l_tmp_class_field_load_result_n1702;
                  struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1703 = hxc_l_levelView_h37960af8bef4;
                  hxc_l_gc_roots[564] = (const void *)hxc_l_tmp_load_result_n1703;
                  if (hxc_l_tmp_load_result_n1703 == NULL)
                  {
                    abort();
                  }
                  struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_class_field_load_result_n1704 = (*hxc_l_tmp_load_result_n1703).hxc_levelPresentation;
                  hxc_l_gc_roots[565] = (const void *)hxc_l_tmp_class_field_load_result_n1704;
                  if (hxc_l_tmp_class_field_load_result_n1704 == NULL)
                  {
                    abort();
                  }
                  int32_t hxc_l_tmp_class_field_load_result_n1705 = (*hxc_l_tmp_class_field_load_result_n1704).hxc_environmentSkyGreenValue;
                  int32_t hxc_l_g_he3478ffa57b4 = hxc_l_tmp_class_field_load_result_n1705;
                  struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1706 = hxc_l_levelView_h37960af8bef4;
                  hxc_l_gc_roots[566] = (const void *)hxc_l_tmp_load_result_n1706;
                  if (hxc_l_tmp_load_result_n1706 == NULL)
                  {
                    abort();
                  }
                  struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_class_field_load_result_n1707 = (*hxc_l_tmp_load_result_n1706).hxc_levelPresentation;
                  hxc_l_gc_roots[567] = (const void *)hxc_l_tmp_class_field_load_result_n1707;
                  if (hxc_l_tmp_class_field_load_result_n1707 == NULL)
                  {
                    abort();
                  }
                  int32_t hxc_l_tmp_class_field_load_result_n1708 = (*hxc_l_tmp_class_field_load_result_n1707).hxc_environmentSkyBlueValue;
                  int32_t hxc_l_b = hxc_l_tmp_class_field_load_result_n1708;
                  int32_t hxc_l_a = 255;
                  int32_t hxc_l_tmp_load_result_n1709 = hxc_l_r;
                  int32_t hxc_l_tmp_conditional_result_n500 = 0;
                  if (hxc_l_tmp_load_result_n1709 < 0)
                  {
                    hxc_l_tmp_conditional_result_n500 = 0;
                  }
                  else
                  {
                    int32_t hxc_l_tmp_load_result_n1710 = hxc_l_r;
                    int32_t hxc_l_tmp_conditional_result_n501 = 0;
                    if (hxc_l_tmp_load_result_n1710 > 255)
                    {
                      hxc_l_tmp_conditional_result_n501 = 255;
                    }
                    else
                    {
                      hxc_l_tmp_conditional_result_n501 = hxc_l_r;
                    }
                    hxc_l_tmp_conditional_result_n500 = hxc_l_tmp_conditional_result_n501;
                  }
                  uint8_t hxc_l_red_h61edd9ca4382 = (uint8_t)hxc_l_tmp_conditional_result_n500;
                  int32_t hxc_l_tmp_load_result_n1714 = hxc_l_g_he3478ffa57b4;
                  int32_t hxc_l_tmp_conditional_result_n503 = 0;
                  if (hxc_l_tmp_load_result_n1714 < 0)
                  {
                    hxc_l_tmp_conditional_result_n503 = 0;
                  }
                  else
                  {
                    int32_t hxc_l_tmp_load_result_n1715 = hxc_l_g_he3478ffa57b4;
                    int32_t hxc_l_tmp_conditional_result_n504 = 0;
                    if (hxc_l_tmp_load_result_n1715 > 255)
                    {
                      hxc_l_tmp_conditional_result_n504 = 255;
                    }
                    else
                    {
                      hxc_l_tmp_conditional_result_n504 = hxc_l_g_he3478ffa57b4;
                    }
                    hxc_l_tmp_conditional_result_n503 = hxc_l_tmp_conditional_result_n504;
                  }
                  uint8_t hxc_l_green_hec8902eb716f = (uint8_t)hxc_l_tmp_conditional_result_n503;
                  int32_t hxc_l_tmp_load_result_n1719 = hxc_l_b;
                  int32_t hxc_l_tmp_conditional_result_n506 = 0;
                  if (hxc_l_tmp_load_result_n1719 < 0)
                  {
                    hxc_l_tmp_conditional_result_n506 = 0;
                  }
                  else
                  {
                    int32_t hxc_l_tmp_load_result_n1720 = hxc_l_b;
                    int32_t hxc_l_tmp_conditional_result_n507 = 0;
                    if (hxc_l_tmp_load_result_n1720 > 255)
                    {
                      hxc_l_tmp_conditional_result_n507 = 255;
                    }
                    else
                    {
                      hxc_l_tmp_conditional_result_n507 = hxc_l_b;
                    }
                    hxc_l_tmp_conditional_result_n506 = hxc_l_tmp_conditional_result_n507;
                  }
                  uint8_t hxc_l_blue_h08fb5796d969 = (uint8_t)hxc_l_tmp_conditional_result_n506;
                  int32_t hxc_l_tmp_load_result_n1724 = hxc_l_a;
                  int32_t hxc_l_tmp_conditional_result_n509 = 0;
                  if (hxc_l_tmp_load_result_n1724 < 0)
                  {
                    hxc_l_tmp_conditional_result_n509 = 0;
                  }
                  else
                  {
                    int32_t hxc_l_tmp_load_result_n1725 = hxc_l_a;
                    int32_t hxc_l_tmp_conditional_result_n510 = 0;
                    if (hxc_l_tmp_load_result_n1725 > 255)
                    {
                      hxc_l_tmp_conditional_result_n510 = 255;
                    }
                    else
                    {
                      hxc_l_tmp_conditional_result_n510 = hxc_l_a;
                    }
                    hxc_l_tmp_conditional_result_n509 = hxc_l_tmp_conditional_result_n510;
                  }
                  uint8_t hxc_l_alpha_h06e8caa0093c = (uint8_t)hxc_l_tmp_conditional_result_n509;
                  uint8_t hxc_l_tmp_load_result_n1729 = hxc_l_red_h61edd9ca4382;
                  uint8_t hxc_l_tmp_load_result_n1730 = hxc_l_green_hec8902eb716f;
                  uint8_t hxc_l_tmp_load_result_n1731 = hxc_l_blue_h08fb5796d969;
                  hxc_l_color = (struct Color){ .r = hxc_l_tmp_load_result_n1729, .g = hxc_l_tmp_load_result_n1730, .b = hxc_l_tmp_load_result_n1731, .a = hxc_l_alpha_h06e8caa0093c };
                }
                else
                {
                  hxc_l_color = (struct Color){ .r = 126, .g = 190, .b = 201, .a = 255 };
                }
                ClearBackground(hxc_l_color);
                BeginMode3D(hxc_l_camera);
                hxc_l_gc_roots[568] = (const void *)hxc_l_preparedPortal;
                struct hxc_caxecraft_app_PreparedCampaignPortal *hxc_l_portal_h5495b2eb0abd = hxc_l_preparedPortal;
                if (hxc_l_screen == hxc_caxecraft_app_AppScreen_EditorTestPlay)
                {
                  hxc_l_gc_roots[569] = (const void *)NULL;
                  hxc_l_portal_h5495b2eb0abd = NULL;
                }
                bool hxc_l_portalVisible = { 0 };
                hxc_l_gc_roots[570] = (const void *)hxc_l_portal_h5495b2eb0abd;
                hxc_l_gc_roots[571] = (const void *)NULL;
                if (hxc_l_portal_h5495b2eb0abd != NULL)
                {
                  double hxc_l_tmp_record_field_load_result_n1741 = hxc_l_renderPosition.hxc_x;
                  struct hxc_caxecraft_app_PreparedCampaignPortal *hxc_l_tmp_load_result_n1742 = hxc_l_portal_h5495b2eb0abd;
                  hxc_l_gc_roots[572] = (const void *)hxc_l_tmp_load_result_n1742;
                  if (hxc_l_tmp_load_result_n1742 == NULL)
                  {
                    abort();
                  }
                  double hxc_l_tmp_class_field_load_result_n1743 = (*hxc_l_tmp_load_result_n1742).hxc_sourceCenterXValue;
                  double hxc_l_dx = hxc_l_tmp_record_field_load_result_n1741 - hxc_l_tmp_class_field_load_result_n1743;
                  double hxc_l_tmp_record_field_load_result_n1744 = hxc_l_renderPosition.hxc_z;
                  struct hxc_caxecraft_app_PreparedCampaignPortal *hxc_l_tmp_load_result_n1745 = hxc_l_portal_h5495b2eb0abd;
                  hxc_l_gc_roots[573] = (const void *)hxc_l_tmp_load_result_n1745;
                  if (hxc_l_tmp_load_result_n1745 == NULL)
                  {
                    abort();
                  }
                  double hxc_l_tmp_class_field_load_result_n1746 = (*hxc_l_tmp_load_result_n1745).hxc_sourceCenterZValue;
                  double hxc_l_dz = hxc_l_tmp_record_field_load_result_n1744 - hxc_l_tmp_class_field_load_result_n1746;
                  double hxc_l_tmp_load_result_n1747 = hxc_l_dx;
                  double hxc_l_tmp_load_result_n1748 = hxc_l_dx;
                  double hxc_l_tmp_load_result_n1749 = hxc_l_dz;
                  hxc_l_portalVisible = hxc_l_tmp_load_result_n1747 * hxc_l_tmp_load_result_n1748 + hxc_l_tmp_load_result_n1749 * hxc_l_dz <= 100.0;
                }
                else
                {
                  hxc_l_portalVisible = false;
                }
                if (hxc_l_hasEnvironment)
                {
                  struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1752 = hxc_l_levelView_h37960af8bef4;
                  hxc_l_gc_roots[574] = (const void *)hxc_l_tmp_load_result_n1752;
                  if (hxc_l_tmp_load_result_n1752 == NULL)
                  {
                    abort();
                  }
                  struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_class_field_load_result_n1753 = (*hxc_l_tmp_load_result_n1752).hxc_levelPresentation;
                  hxc_l_gc_roots[575] = (const void *)hxc_l_tmp_class_field_load_result_n1753;
                  if (hxc_l_tmp_class_field_load_result_n1753 == NULL)
                  {
                    abort();
                  }
                  bool hxc_l_tmp_class_field_load_result_n1754 = (*hxc_l_tmp_class_field_load_result_n1753).hxc_environmentHasSunValue;
                  struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1755 = hxc_l_levelView_h37960af8bef4;
                  hxc_l_gc_roots[576] = (const void *)hxc_l_tmp_load_result_n1755;
                  if (hxc_l_tmp_load_result_n1755 == NULL)
                  {
                    abort();
                  }
                  struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_class_field_load_result_n1756 = (*hxc_l_tmp_load_result_n1755).hxc_levelPresentation;
                  hxc_l_gc_roots[577] = (const void *)hxc_l_tmp_class_field_load_result_n1756;
                  if (hxc_l_tmp_class_field_load_result_n1756 == NULL)
                  {
                    abort();
                  }
                  int32_t hxc_l_tmp_class_field_load_result_n1757 = (*hxc_l_tmp_class_field_load_result_n1756).hxc_environmentSunXValue;
                  struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1758 = hxc_l_levelView_h37960af8bef4;
                  hxc_l_gc_roots[578] = (const void *)hxc_l_tmp_load_result_n1758;
                  if (hxc_l_tmp_load_result_n1758 == NULL)
                  {
                    abort();
                  }
                  struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_class_field_load_result_n1759 = (*hxc_l_tmp_load_result_n1758).hxc_levelPresentation;
                  hxc_l_gc_roots[579] = (const void *)hxc_l_tmp_class_field_load_result_n1759;
                  if (hxc_l_tmp_class_field_load_result_n1759 == NULL)
                  {
                    abort();
                  }
                  int32_t hxc_l_tmp_class_field_load_result_n1760 = (*hxc_l_tmp_class_field_load_result_n1759).hxc_environmentSunYValue;
                  struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1761 = hxc_l_levelView_h37960af8bef4;
                  hxc_l_gc_roots[580] = (const void *)hxc_l_tmp_load_result_n1761;
                  if (hxc_l_tmp_load_result_n1761 == NULL)
                  {
                    abort();
                  }
                  struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_class_field_load_result_n1762 = (*hxc_l_tmp_load_result_n1761).hxc_levelPresentation;
                  hxc_l_gc_roots[581] = (const void *)hxc_l_tmp_class_field_load_result_n1762;
                  if (hxc_l_tmp_class_field_load_result_n1762 == NULL)
                  {
                    abort();
                  }
                  int32_t hxc_l_tmp_class_field_load_result_n1763 = (*hxc_l_tmp_class_field_load_result_n1762).hxc_environmentSunZValue;
                  struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1764 = hxc_l_levelView_h37960af8bef4;
                  hxc_l_gc_roots[582] = (const void *)hxc_l_tmp_load_result_n1764;
                  if (hxc_l_tmp_load_result_n1764 == NULL)
                  {
                    abort();
                  }
                  struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_class_field_load_result_n1765 = (*hxc_l_tmp_load_result_n1764).hxc_levelPresentation;
                  hxc_l_gc_roots[583] = (const void *)hxc_l_tmp_class_field_load_result_n1765;
                  if (hxc_l_tmp_class_field_load_result_n1765 == NULL)
                  {
                    abort();
                  }
                  int32_t hxc_l_tmp_class_field_load_result_n1766 = (*hxc_l_tmp_class_field_load_result_n1765).hxc_environmentSunRadiusMilliValue;
                  struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1767 = hxc_l_levelView_h37960af8bef4;
                  hxc_l_gc_roots[584] = (const void *)hxc_l_tmp_load_result_n1767;
                  if (hxc_l_tmp_load_result_n1767 == NULL)
                  {
                    abort();
                  }
                  struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_class_field_load_result_n1768 = (*hxc_l_tmp_load_result_n1767).hxc_levelPresentation;
                  hxc_l_gc_roots[585] = (const void *)hxc_l_tmp_class_field_load_result_n1768;
                  if (hxc_l_tmp_class_field_load_result_n1768 == NULL)
                  {
                    abort();
                  }
                  int32_t hxc_l_tmp_class_field_load_result_n1769 = (*hxc_l_tmp_class_field_load_result_n1768).hxc_environmentCloudCountValue;
                  struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1770 = hxc_l_levelView_h37960af8bef4;
                  hxc_l_gc_roots[586] = (const void *)hxc_l_tmp_load_result_n1770;
                  if (hxc_l_tmp_load_result_n1770 == NULL)
                  {
                    abort();
                  }
                  struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_class_field_load_result_n1771 = (*hxc_l_tmp_load_result_n1770).hxc_levelPresentation;
                  hxc_l_gc_roots[587] = (const void *)hxc_l_tmp_class_field_load_result_n1771;
                  if (hxc_l_tmp_class_field_load_result_n1771 == NULL)
                  {
                    abort();
                  }
                  int32_t hxc_l_tmp_class_field_load_result_n1772 = (*hxc_l_tmp_class_field_load_result_n1771).hxc_environmentCloudSpeedMilliValue;
                  struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1773 = hxc_l_levelView_h37960af8bef4;
                  hxc_l_gc_roots[588] = (const void *)hxc_l_tmp_load_result_n1773;
                  if (hxc_l_tmp_load_result_n1773 == NULL)
                  {
                    abort();
                  }
                  struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_class_field_load_result_n1774 = (*hxc_l_tmp_load_result_n1773).hxc_levelPresentation;
                  hxc_l_gc_roots[589] = (const void *)hxc_l_tmp_class_field_load_result_n1774;
                  if (hxc_l_tmp_class_field_load_result_n1774 == NULL)
                  {
                    abort();
                  }
                  int32_t hxc_l_tmp_class_field_load_result_n1775 = (*hxc_l_tmp_class_field_load_result_n1774).hxc_environmentCloudSeedValue;
                  double hxc_l_tmp_load_result_n1776 = hxc_l_eyeX;
                  double hxc_l_tmp_load_result_n1777 = hxc_l_eyeY;
                  double hxc_l_tmp_load_result_n1778 = hxc_l_eyeZ;
                  hxc_caxecraft_app_CaxecraftApp_drawWorldSunAndClouds(hxc_l_tmp_class_field_load_result_n1754, hxc_l_tmp_class_field_load_result_n1757, hxc_l_tmp_class_field_load_result_n1760, hxc_l_tmp_class_field_load_result_n1763, hxc_l_tmp_class_field_load_result_n1766, hxc_l_tmp_class_field_load_result_n1769, hxc_l_tmp_class_field_load_result_n1772, hxc_l_tmp_class_field_load_result_n1775, hxc_l_tmp_load_result_n1776, hxc_l_tmp_load_result_n1777, hxc_l_tmp_load_result_n1778, (double)hxc_l_completedTicks * 0.05);
                  bool hxc_l_tmp_load_result_n1780 = hxc_l_portalVisible;
                  int32_t hxc_l_tmp_conditional_result_n516 = 0;
                  if (!hxc_l_tmp_load_result_n1780)
                  {
                    struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1781 = hxc_l_levelView_h37960af8bef4;
                    hxc_l_gc_roots[590] = (const void *)hxc_l_tmp_load_result_n1781;
                    if (hxc_l_tmp_load_result_n1781 == NULL)
                    {
                      abort();
                    }
                    struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_class_field_load_result_n1782 = (*hxc_l_tmp_load_result_n1781).hxc_levelPresentation;
                    hxc_l_gc_roots[591] = (const void *)hxc_l_tmp_class_field_load_result_n1782;
                    if (hxc_l_tmp_class_field_load_result_n1782 == NULL)
                    {
                      abort();
                    }
                    int32_t hxc_l_tmp_class_field_load_result_n1783 = (*hxc_l_tmp_class_field_load_result_n1782).hxc_environmentHorizonMaskValue;
                    hxc_l_tmp_conditional_result_n516 = hxc_l_tmp_class_field_load_result_n1783;
                  }
                  else
                  {
                    struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1784 = hxc_l_levelView_h37960af8bef4;
                    hxc_l_gc_roots[592] = (const void *)hxc_l_tmp_load_result_n1784;
                    if (hxc_l_tmp_load_result_n1784 == NULL)
                    {
                      abort();
                    }
                    struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_class_field_load_result_n1785 = (*hxc_l_tmp_load_result_n1784).hxc_levelPresentation;
                    hxc_l_gc_roots[593] = (const void *)hxc_l_tmp_class_field_load_result_n1785;
                    if (hxc_l_tmp_class_field_load_result_n1785 == NULL)
                    {
                      abort();
                    }
                    int32_t hxc_l_tmp_class_field_load_result_n1786 = (*hxc_l_tmp_class_field_load_result_n1785).hxc_environmentHorizonMaskValue;
                    struct hxc_caxecraft_app_PreparedCampaignPortal *hxc_l_tmp_load_result_n1787 = hxc_l_portal_h5495b2eb0abd;
                    hxc_l_gc_roots[594] = (const void *)hxc_l_tmp_load_result_n1787;
                    if (hxc_l_tmp_load_result_n1787 == NULL)
                    {
                      abort();
                    }
                    int32_t hxc_l_tmp_class_field_load_result_n1788 = (*hxc_l_tmp_load_result_n1787).hxc_sourceEdgeMaskValue;
                    hxc_l_tmp_conditional_result_n516 = hxc_i32_bit_and(hxc_l_tmp_class_field_load_result_n1786, hxc_i32_bit_not(hxc_l_tmp_class_field_load_result_n1788));
                  }
                  int32_t hxc_l_horizonMask = hxc_l_tmp_conditional_result_n516;
                  if (hxc_l_self == NULL)
                  {
                    abort();
                  }
                  struct hxc_caxecraft_app_DistantHorizonRenderer *hxc_l_tmp_owned_class_field_address_n1790 = &(*hxc_l_self).hxc_distantHorizonRenderer;
                  struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1791 = hxc_l_levelView_h37960af8bef4;
                  hxc_l_gc_roots[595] = (const void *)hxc_l_tmp_load_result_n1791;
                  if (hxc_l_tmp_load_result_n1791 == NULL)
                  {
                    abort();
                  }
                  struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_class_field_load_result_n1792 = (*hxc_l_tmp_load_result_n1791).hxc_levelPresentation;
                  hxc_l_gc_roots[596] = (const void *)hxc_l_tmp_class_field_load_result_n1792;
                  if (hxc_l_tmp_class_field_load_result_n1792 == NULL)
                  {
                    abort();
                  }
                  int32_t hxc_l_tmp_class_field_load_result_n1793 = (*hxc_l_tmp_class_field_load_result_n1792).hxc_worldWidth;
                  (*hxc_l_tmp_owned_class_field_address_n1790).hxc_descriptorValue = hxc_i32_bit_or(hxc_l_tmp_class_field_load_result_n1793, hxc_i32_shift_left_masked(hxc_l_horizonMask, 8));
                  struct hxc_caxecraft_app_DistantHorizonRenderer *hxc_l_tmp_owned_class_field_address_n1795 = &(*hxc_l_self).hxc_distantHorizonRenderer;
                  if (hxc_l_tmp_owned_class_field_address_n1795 == NULL)
                  {
                    abort();
                  }
                  struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1796 = hxc_l_session;
                  hxc_l_gc_roots[597] = (const void *)hxc_l_tmp_load_result_n1796;
                  if (hxc_l_tmp_load_result_n1796 == NULL)
                  {
                    abort();
                  }
                  size_t hxc_l_tmp_length_n1798;
                  const uint8_t *hxc_l_tmp_instance_call_result_n1797 = hxc_caxecraft_domain_GameSession_worldView(hxc_l_tmp_load_result_n1796, &hxc_l_tmp_length_n1798);
                  struct Texture hxc_l_tmp_load_result_n1799 = hxc_l_terrainTexture;
                  bool hxc_l_tmp_load_result_n1800 = hxc_l_terrainTextureReady;
                  struct Texture hxc_l_tmp_load_result_n1801 = hxc_l_adventureTerrainTexture;
                  bool hxc_l_tmp_load_result_n1802 = hxc_l_adventureTerrainTextureReady;
                  struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1803 = hxc_l_levelView_h37960af8bef4;
                  hxc_l_gc_roots[598] = (const void *)hxc_l_tmp_load_result_n1803;
                  if (hxc_l_tmp_load_result_n1803 == NULL)
                  {
                    abort();
                  }
                  int32_t hxc_l_tmp_class_field_load_result_n1804 = (*hxc_l_tmp_load_result_n1803).hxc_waterCell;
                  hxc_caxecraft_app_DistantHorizonRenderer_draw(hxc_l_tmp_owned_class_field_address_n1795, hxc_l_tmp_instance_call_result_n1797, hxc_l_tmp_length_n1798, hxc_l_tmp_load_result_n1799, hxc_l_tmp_load_result_n1800, hxc_l_tmp_load_result_n1801, hxc_l_tmp_load_result_n1802, hxc_l_tmp_class_field_load_result_n1804);
                }
                if (hxc_l_portalVisible)
                {
                  struct hxc_caxecraft_app_PreparedCampaignPortal *hxc_l_tmp_load_result_n1806 = hxc_l_portal_h5495b2eb0abd;
                  hxc_l_gc_roots[599] = (const void *)hxc_l_tmp_load_result_n1806;
                  if (hxc_l_tmp_load_result_n1806 == NULL)
                  {
                    abort();
                  }
                  struct hxc_caxecraft_app_StagedPlayableLevel *hxc_l_tmp_class_field_load_result_n1807 = (*hxc_l_tmp_load_result_n1806).hxc_stagedValue;
                  hxc_l_gc_roots[600] = (const void *)hxc_l_tmp_class_field_load_result_n1807;
                  struct hxc_caxecraft_app_StagedPlayableLevel *hxc_l_preview_h01b171c98658 = hxc_l_tmp_class_field_load_result_n1807;
                  if (hxc_l_self == NULL)
                  {
                    abort();
                  }
                  struct hxc_caxecraft_app_TerrainRenderer *hxc_l_tmp_owned_class_field_address_n1808 = &(*hxc_l_self).hxc_portalTerrainRenderer;
                  if (hxc_l_tmp_owned_class_field_address_n1808 == NULL)
                  {
                    abort();
                  }
                  struct hxc_caxecraft_app_StagedPlayableLevel *hxc_l_tmp_load_result_n1809 = hxc_l_preview_h01b171c98658;
                  hxc_l_gc_roots[601] = (const void *)hxc_l_tmp_load_result_n1809;
                  if (hxc_l_tmp_load_result_n1809 == NULL)
                  {
                    abort();
                  }
                  struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_class_field_load_result_n1810 = (*hxc_l_tmp_load_result_n1809).hxc_stagedView;
                  hxc_l_gc_roots[602] = (const void *)hxc_l_tmp_class_field_load_result_n1810;
                  if (hxc_l_tmp_class_field_load_result_n1810 == NULL)
                  {
                    abort();
                  }
                  struct hxc_caxecraft_content_LoadedContentGeneration *hxc_l_tmp_class_field_load_result_n1811 = (*hxc_l_tmp_class_field_load_result_n1810).hxc_loadedGeneration;
                  hxc_l_gc_roots[603] = (const void *)hxc_l_tmp_class_field_load_result_n1811;
                  if (hxc_l_tmp_class_field_load_result_n1811 == NULL)
                  {
                    abort();
                  }
                  struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_class_field_load_result_n1812 = (*hxc_l_tmp_class_field_load_result_n1811).hxc_ownedSession;
                  hxc_l_gc_roots[604] = (const void *)hxc_l_tmp_class_field_load_result_n1812;
                  if (hxc_l_tmp_class_field_load_result_n1812 == NULL)
                  {
                    abort();
                  }
                  size_t hxc_l_tmp_length_n1814;
                  const uint8_t *hxc_l_tmp_instance_call_result_n1813 = hxc_caxecraft_domain_GameSession_worldView(hxc_l_tmp_class_field_load_result_n1812, &hxc_l_tmp_length_n1814);
                  struct Texture hxc_l_tmp_load_result_n1815 = hxc_l_terrainTexture;
                  bool hxc_l_tmp_load_result_n1816 = hxc_l_terrainTextureReady;
                  struct Texture hxc_l_tmp_load_result_n1817 = hxc_l_adventureTerrainTexture;
                  bool hxc_l_tmp_load_result_n1818 = hxc_l_adventureTerrainTextureReady;
                  double hxc_l_tmp_record_field_load_result_n1819 = hxc_l_renderPosition.hxc_x;
                  double hxc_l_tmp_record_field_load_result_n1820 = hxc_l_renderPosition.hxc_z;
                  struct hxc_caxecraft_app_PreparedCampaignPortal *hxc_l_tmp_load_result_n1821 = hxc_l_portal_h5495b2eb0abd;
                  hxc_l_gc_roots[605] = (const void *)hxc_l_tmp_load_result_n1821;
                  if (hxc_l_tmp_load_result_n1821 == NULL)
                  {
                    abort();
                  }
                  double hxc_l_tmp_class_field_load_result_n1822 = (*hxc_l_tmp_load_result_n1821).hxc_offsetXValue;
                  struct hxc_caxecraft_app_PreparedCampaignPortal *hxc_l_tmp_load_result_n1823 = hxc_l_portal_h5495b2eb0abd;
                  hxc_l_gc_roots[606] = (const void *)hxc_l_tmp_load_result_n1823;
                  if (hxc_l_tmp_load_result_n1823 == NULL)
                  {
                    abort();
                  }
                  double hxc_l_tmp_class_field_load_result_n1824 = (*hxc_l_tmp_load_result_n1823).hxc_offsetYValue;
                  struct hxc_caxecraft_app_PreparedCampaignPortal *hxc_l_tmp_load_result_n1825 = hxc_l_portal_h5495b2eb0abd;
                  hxc_l_gc_roots[607] = (const void *)hxc_l_tmp_load_result_n1825;
                  if (hxc_l_tmp_load_result_n1825 == NULL)
                  {
                    abort();
                  }
                  double hxc_l_tmp_class_field_load_result_n1826 = (*hxc_l_tmp_load_result_n1825).hxc_offsetZValue;
                  hxc_caxecraft_app_TerrainRenderer_drawTranslated(hxc_l_tmp_owned_class_field_address_n1808, hxc_l_tmp_instance_call_result_n1813, hxc_l_tmp_length_n1814, hxc_l_tmp_load_result_n1815, hxc_l_tmp_load_result_n1816, hxc_l_tmp_load_result_n1817, hxc_l_tmp_load_result_n1818, hxc_l_tmp_record_field_load_result_n1819, hxc_l_tmp_record_field_load_result_n1820, hxc_l_tmp_class_field_load_result_n1822, hxc_l_tmp_class_field_load_result_n1824, hxc_l_tmp_class_field_load_result_n1826);
                }
                if (hxc_l_self == NULL)
                {
                  abort();
                }
                struct hxc_caxecraft_app_TerrainRenderer *hxc_l_tmp_owned_class_field_address_n1827 = &(*hxc_l_self).hxc_terrainRenderer;
                if (hxc_l_tmp_owned_class_field_address_n1827 == NULL)
                {
                  abort();
                }
                struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1828 = hxc_l_session;
                hxc_l_gc_roots[608] = (const void *)hxc_l_tmp_load_result_n1828;
                if (hxc_l_tmp_load_result_n1828 == NULL)
                {
                  abort();
                }
                size_t hxc_l_tmp_length_n1830;
                const uint8_t *hxc_l_tmp_instance_call_result_n1829 = hxc_caxecraft_domain_GameSession_worldView(hxc_l_tmp_load_result_n1828, &hxc_l_tmp_length_n1830);
                struct Texture hxc_l_tmp_load_result_n1831 = hxc_l_terrainTexture;
                bool hxc_l_tmp_load_result_n1832 = hxc_l_terrainTextureReady;
                struct Texture hxc_l_tmp_load_result_n1833 = hxc_l_adventureTerrainTexture;
                bool hxc_l_tmp_load_result_n1834 = hxc_l_adventureTerrainTextureReady;
                double hxc_l_tmp_record_field_load_result_n1835 = hxc_l_renderPosition.hxc_x;
                struct hxc_caxecraft_app_TerrainRenderCounters hxc_l_tmp_instance_call_result_n1837 = hxc_caxecraft_app_TerrainRenderer_draw(hxc_l_tmp_owned_class_field_address_n1827, hxc_l_tmp_instance_call_result_n1829, hxc_l_tmp_length_n1830, hxc_l_tmp_load_result_n1831, hxc_l_tmp_load_result_n1832, hxc_l_tmp_load_result_n1833, hxc_l_tmp_load_result_n1834, hxc_l_tmp_record_field_load_result_n1835, hxc_l_renderPosition.hxc_z);
                struct hxc_caxecraft_app_TerrainRenderCounters hxc_l_renderCounters = hxc_l_tmp_instance_call_result_n1837;
                if (hxc_l_portalVisible)
                {
                  struct hxc_caxecraft_app_PreparedCampaignPortal *hxc_l_tmp_load_result_n1839 = hxc_l_portal_h5495b2eb0abd;
                  hxc_l_gc_roots[609] = (const void *)hxc_l_tmp_load_result_n1839;
                  if (hxc_l_tmp_load_result_n1839 == NULL)
                  {
                    abort();
                  }
                  struct hxc_caxecraft_app_StagedPlayableLevel *hxc_l_tmp_class_field_load_result_n1840 = (*hxc_l_tmp_load_result_n1839).hxc_stagedValue;
                  hxc_l_gc_roots[610] = (const void *)hxc_l_tmp_class_field_load_result_n1840;
                  struct hxc_caxecraft_app_StagedPlayableLevel *hxc_l_preview_hbd1792c0ff0a = hxc_l_tmp_class_field_load_result_n1840;
                  struct hxc_caxecraft_app_WaterRenderer *hxc_l_tmp_owned_class_field_address_n1841 = &(*hxc_l_self).hxc_portalWaterRenderer;
                  if (hxc_l_tmp_owned_class_field_address_n1841 == NULL)
                  {
                    abort();
                  }
                  struct hxc_caxecraft_app_StagedPlayableLevel *hxc_l_tmp_load_result_n1842 = hxc_l_preview_hbd1792c0ff0a;
                  hxc_l_gc_roots[611] = (const void *)hxc_l_tmp_load_result_n1842;
                  if (hxc_l_tmp_load_result_n1842 == NULL)
                  {
                    abort();
                  }
                  struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_class_field_load_result_n1843 = (*hxc_l_tmp_load_result_n1842).hxc_stagedView;
                  hxc_l_gc_roots[612] = (const void *)hxc_l_tmp_class_field_load_result_n1843;
                  if (hxc_l_tmp_class_field_load_result_n1843 == NULL)
                  {
                    abort();
                  }
                  struct hxc_caxecraft_content_LoadedContentGeneration *hxc_l_tmp_class_field_load_result_n1844 = (*hxc_l_tmp_class_field_load_result_n1843).hxc_loadedGeneration;
                  hxc_l_gc_roots[613] = (const void *)hxc_l_tmp_class_field_load_result_n1844;
                  if (hxc_l_tmp_class_field_load_result_n1844 == NULL)
                  {
                    abort();
                  }
                  struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_class_field_load_result_n1845 = (*hxc_l_tmp_class_field_load_result_n1844).hxc_ownedSession;
                  hxc_l_gc_roots[614] = (const void *)hxc_l_tmp_class_field_load_result_n1845;
                  if (hxc_l_tmp_class_field_load_result_n1845 == NULL)
                  {
                    abort();
                  }
                  size_t hxc_l_tmp_length_n1847;
                  const uint8_t *hxc_l_tmp_instance_call_result_n1846 = hxc_caxecraft_domain_GameSession_worldView(hxc_l_tmp_class_field_load_result_n1845, &hxc_l_tmp_length_n1847);
                  struct Texture hxc_l_tmp_load_result_n1848 = hxc_l_terrainTexture;
                  bool hxc_l_tmp_load_result_n1849 = hxc_l_terrainTextureReady;
                  struct hxc_caxecraft_app_StagedPlayableLevel *hxc_l_tmp_load_result_n1850 = hxc_l_preview_hbd1792c0ff0a;
                  hxc_l_gc_roots[615] = (const void *)hxc_l_tmp_load_result_n1850;
                  if (hxc_l_tmp_load_result_n1850 == NULL)
                  {
                    abort();
                  }
                  struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_class_field_load_result_n1851 = (*hxc_l_tmp_load_result_n1850).hxc_stagedView;
                  hxc_l_gc_roots[616] = (const void *)hxc_l_tmp_class_field_load_result_n1851;
                  if (hxc_l_tmp_class_field_load_result_n1851 == NULL)
                  {
                    abort();
                  }
                  int32_t hxc_l_tmp_class_field_load_result_n1852 = (*hxc_l_tmp_class_field_load_result_n1851).hxc_waterCell;
                  double hxc_l_tmp_load_result_n1853 = hxc_l_eyeX;
                  double hxc_l_tmp_load_result_n1854 = hxc_l_eyeY;
                  double hxc_l_tmp_load_result_n1855 = hxc_l_eyeZ;
                  struct hxc_caxecraft_app_PreparedCampaignPortal *hxc_l_tmp_load_result_n1856 = hxc_l_portal_h5495b2eb0abd;
                  hxc_l_gc_roots[617] = (const void *)hxc_l_tmp_load_result_n1856;
                  if (hxc_l_tmp_load_result_n1856 == NULL)
                  {
                    abort();
                  }
                  double hxc_l_tmp_class_field_load_result_n1857 = (*hxc_l_tmp_load_result_n1856).hxc_offsetXValue;
                  struct hxc_caxecraft_app_PreparedCampaignPortal *hxc_l_tmp_load_result_n1858 = hxc_l_portal_h5495b2eb0abd;
                  hxc_l_gc_roots[618] = (const void *)hxc_l_tmp_load_result_n1858;
                  if (hxc_l_tmp_load_result_n1858 == NULL)
                  {
                    abort();
                  }
                  double hxc_l_tmp_class_field_load_result_n1859 = (*hxc_l_tmp_load_result_n1858).hxc_offsetYValue;
                  struct hxc_caxecraft_app_PreparedCampaignPortal *hxc_l_tmp_load_result_n1860 = hxc_l_portal_h5495b2eb0abd;
                  hxc_l_gc_roots[619] = (const void *)hxc_l_tmp_load_result_n1860;
                  if (hxc_l_tmp_load_result_n1860 == NULL)
                  {
                    abort();
                  }
                  double hxc_l_tmp_class_field_load_result_n1861 = (*hxc_l_tmp_load_result_n1860).hxc_offsetZValue;
                  hxc_caxecraft_app_WaterRenderer_drawTranslated(hxc_l_tmp_owned_class_field_address_n1841, hxc_l_tmp_instance_call_result_n1846, hxc_l_tmp_length_n1847, hxc_l_tmp_load_result_n1848, hxc_l_tmp_load_result_n1849, hxc_l_tmp_class_field_load_result_n1852, hxc_l_tmp_load_result_n1853, hxc_l_tmp_load_result_n1854, hxc_l_tmp_load_result_n1855, hxc_l_tmp_class_field_load_result_n1857, hxc_l_tmp_class_field_load_result_n1859, hxc_l_tmp_class_field_load_result_n1861);
                }
                struct hxc_caxecraft_app_WaterRenderer *hxc_l_tmp_owned_class_field_address_n1862 = &(*hxc_l_self).hxc_waterRenderer;
                if (hxc_l_tmp_owned_class_field_address_n1862 == NULL)
                {
                  abort();
                }
                struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1863 = hxc_l_session;
                hxc_l_gc_roots[620] = (const void *)hxc_l_tmp_load_result_n1863;
                if (hxc_l_tmp_load_result_n1863 == NULL)
                {
                  abort();
                }
                size_t hxc_l_tmp_length_n1865;
                const uint8_t *hxc_l_tmp_instance_call_result_n1864 = hxc_caxecraft_domain_GameSession_worldView(hxc_l_tmp_load_result_n1863, &hxc_l_tmp_length_n1865);
                struct Texture hxc_l_tmp_load_result_n1866 = hxc_l_terrainTexture;
                bool hxc_l_tmp_load_result_n1867 = hxc_l_terrainTextureReady;
                struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1868 = hxc_l_levelView_h37960af8bef4;
                hxc_l_gc_roots[621] = (const void *)hxc_l_tmp_load_result_n1868;
                if (hxc_l_tmp_load_result_n1868 == NULL)
                {
                  abort();
                }
                int32_t hxc_l_tmp_class_field_load_result_n1869 = (*hxc_l_tmp_load_result_n1868).hxc_waterCell;
                double hxc_l_tmp_load_result_n1870 = hxc_l_eyeX;
                double hxc_l_tmp_load_result_n1871 = hxc_l_eyeY;
                struct hxc_caxecraft_app_WaterRenderCounters hxc_l_tmp_instance_call_result_n1873 = hxc_caxecraft_app_WaterRenderer_draw(hxc_l_tmp_owned_class_field_address_n1862, hxc_l_tmp_instance_call_result_n1864, hxc_l_tmp_length_n1865, hxc_l_tmp_load_result_n1866, hxc_l_tmp_load_result_n1867, hxc_l_tmp_class_field_load_result_n1869, hxc_l_tmp_load_result_n1870, hxc_l_tmp_load_result_n1871, hxc_l_eyeZ);
                struct hxc_caxecraft_app_WaterRenderCounters hxc_l_waterCounters = hxc_l_tmp_instance_call_result_n1873;
                int32_t hxc_l_tmp_record_field_load_result_n1874 = hxc_l_renderCounters.hxc_visible;
                int32_t hxc_l_totalVisible = hxc_i32_add_wrapping(hxc_l_tmp_record_field_load_result_n1874, hxc_l_waterCounters.hxc_visible);
                int32_t hxc_l_tmp_record_field_load_result_n1876 = hxc_l_renderCounters.hxc_drawCalls;
                int32_t hxc_l_totalDrawCalls = hxc_i32_add_wrapping(hxc_l_tmp_record_field_load_result_n1876, hxc_l_waterCounters.hxc_drawCalls);
                struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_tmp_load_result_n1878 = hxc_l_contentRegistry;
                hxc_l_gc_roots[622] = (const void *)hxc_l_tmp_load_result_n1878;
                struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1879 = hxc_l_session;
                hxc_l_gc_roots[623] = (const void *)hxc_l_tmp_load_result_n1879;
                struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1880 = hxc_l_levelView_h37960af8bef4;
                hxc_l_gc_roots[624] = (const void *)hxc_l_tmp_load_result_n1880;
                struct Texture hxc_l_tmp_load_result_n1881 = hxc_l_entityTexture;
                bool hxc_l_tmp_load_result_n1882 = hxc_l_entityTextureReady;
                struct Texture hxc_l_tmp_load_result_n1883 = hxc_l_itemTexture;
                bool hxc_l_tmp_load_result_n1884 = hxc_l_itemTextureReady;
                struct Texture hxc_l_tmp_load_result_n1885 = hxc_l_adventureItemTexture;
                bool hxc_l_tmp_load_result_n1886 = hxc_l_adventureItemTextureReady;
                struct Texture hxc_l_tmp_load_result_n1887 = hxc_l_terrainTexture;
                bool hxc_l_tmp_load_result_n1888 = hxc_l_terrainTextureReady;
                struct hxc_caxecraft_app_RuntimeTextureAtlasCatalog *hxc_l_tmp_load_result_n1889 = hxc_l_runtimeTextures;
                hxc_l_gc_roots[625] = (const void *)hxc_l_tmp_load_result_n1889;
                struct hxc_caxecraft_app_RuntimeVoxelModelCatalog *hxc_l_tmp_load_result_n1890 = hxc_l_runtimeModels;
                hxc_l_gc_roots[626] = (const void *)hxc_l_tmp_load_result_n1890;
                struct hxc_caxecraft_app_VoxelFrameAnimationPlayer *hxc_l_tmp_load_result_n1891 = hxc_l_modelAnimations;
                struct hxc_caxecraft_app_ActivePlayableLevel *hxc_l_tmp_load_result_n1892 = hxc_l_frameLevelOwner;
                hxc_l_gc_roots[627] = (const void *)hxc_l_tmp_load_result_n1892;
                if (hxc_l_tmp_load_result_n1892 == NULL)
                {
                  abort();
                }
                struct hxc_caxecraft_content_ActiveContent *hxc_l_tmp_class_field_load_result_n1893 = (*hxc_l_tmp_load_result_n1892).hxc_content;
                hxc_l_gc_roots[628] = (const void *)hxc_l_tmp_class_field_load_result_n1893;
                if (hxc_l_tmp_class_field_load_result_n1893 == NULL)
                {
                  abort();
                }
                struct hxc_caxecraft_content_LoadedContentGeneration *hxc_l_tmp_class_field_load_result_n1894 = (*hxc_l_tmp_class_field_load_result_n1893).hxc_active;
                hxc_l_gc_roots[629] = (const void *)hxc_l_tmp_class_field_load_result_n1894;
                if (hxc_l_tmp_class_field_load_result_n1894 == NULL)
                {
                  abort();
                }
                int32_t hxc_l_tmp_class_field_load_result_n1895 = (*hxc_l_tmp_class_field_load_result_n1894).hxc_generationIdValue;
                hxc_caxecraft_app_StatefulObjectRenderer_drawStatefulObjects(hxc_l_tmp_load_result_n1878, hxc_l_tmp_load_result_n1879, hxc_l_tmp_load_result_n1880, hxc_l_tmp_load_result_n1881, hxc_l_tmp_load_result_n1882, hxc_l_tmp_load_result_n1883, hxc_l_tmp_load_result_n1884, hxc_l_tmp_load_result_n1885, hxc_l_tmp_load_result_n1886, hxc_l_tmp_load_result_n1887, hxc_l_tmp_load_result_n1888, hxc_l_tmp_load_result_n1889, hxc_l_tmp_load_result_n1890, hxc_l_tmp_load_result_n1891, hxc_l_tmp_class_field_load_result_n1895, hxc_l_completedTicks);
                if (hxc_l_cameraView.hxc_avatarVisible)
                {
                  struct Camera3D hxc_l_tmp_load_result_n1898 = hxc_l_camera;
                  struct hxc_caxecraft_app_RuntimeTextureAtlasCatalog *hxc_l_tmp_load_result_n1899 = hxc_l_runtimeTextures;
                  hxc_l_gc_roots[630] = (const void *)hxc_l_tmp_load_result_n1899;
                  double hxc_l_tmp_record_field_load_result_n1900 = hxc_l_renderPosition.hxc_x;
                  double hxc_l_tmp_record_field_load_result_n1901 = hxc_l_renderPosition.hxc_y;
                  hxc_caxecraft_app_CaxecraftApp_drawPlayerAvatar(hxc_l_tmp_load_result_n1898, hxc_l_tmp_load_result_n1899, hxc_l_tmp_record_field_load_result_n1900, hxc_l_tmp_record_field_load_result_n1901, hxc_l_renderPosition.hxc_z);
                }
                struct Camera3D hxc_l_tmp_load_result_n1903 = hxc_l_camera;
                struct Texture hxc_l_tmp_load_result_n1904 = hxc_l_entityTexture;
                bool hxc_l_tmp_load_result_n1905 = hxc_l_entityTextureReady;
                struct hxc_caxecraft_app_RuntimeTextureAtlasCatalog *hxc_l_tmp_load_result_n1906 = hxc_l_runtimeTextures;
                hxc_l_gc_roots[631] = (const void *)hxc_l_tmp_load_result_n1906;
                struct hxc_array_ref *hxc_l_tmp_load_result_n1907 = hxc_l_dialogueActors;
                struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1908 = hxc_l_levelView_h37960af8bef4;
                hxc_l_gc_roots[632] = (const void *)hxc_l_tmp_load_result_n1908;
                struct hxc_caxecraft_domain_Character hxc_l_tmp_load_result_n1909 = hxc_l_enemyActor;
                struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1910 = hxc_l_levelView_h37960af8bef4;
                hxc_l_gc_roots[633] = (const void *)hxc_l_tmp_load_result_n1910;
                if (hxc_l_tmp_load_result_n1910 == NULL)
                {
                  abort();
                }
                hxc_string hxc_l_tmp_class_field_load_result_n1911 = (*hxc_l_tmp_load_result_n1910).hxc_enemyAsset;
                struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1912 = hxc_l_levelView_h37960af8bef4;
                hxc_l_gc_roots[634] = (const void *)hxc_l_tmp_load_result_n1912;
                if (hxc_l_tmp_load_result_n1912 == NULL)
                {
                  abort();
                }
                int32_t hxc_l_tmp_class_field_load_result_n1913 = (*hxc_l_tmp_load_result_n1912).hxc_enemyCell;
                enum hxc_caxecraft_domain_ActorControllerPhase hxc_l_tmp_record_field_load_result_n1914 = hxc_l_enemyPhase_h906f9c33685e.hxc_phase;
                hxc_caxecraft_app_CaxecraftApp_drawActors(hxc_l_tmp_load_result_n1903, hxc_l_tmp_load_result_n1904, hxc_l_tmp_load_result_n1905, hxc_l_tmp_load_result_n1906, hxc_l_tmp_load_result_n1907, hxc_l_tmp_load_result_n1908, hxc_l_tmp_load_result_n1909, hxc_l_tmp_class_field_load_result_n1911, hxc_l_tmp_class_field_load_result_n1913, hxc_l_tmp_record_field_load_result_n1914, hxc_l_berryDrop);
                struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_tmp_load_result_n1916 = hxc_l_contentRegistry;
                hxc_l_gc_roots[635] = (const void *)hxc_l_tmp_load_result_n1916;
                struct Camera3D hxc_l_tmp_load_result_n1917 = hxc_l_camera;
                struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1918 = hxc_l_session;
                hxc_l_gc_roots[636] = (const void *)hxc_l_tmp_load_result_n1918;
                if (hxc_l_tmp_load_result_n1918 == NULL)
                {
                  abort();
                }
                size_t hxc_l_tmp_length_n1920;
                const int32_t *hxc_l_tmp_instance_call_result_n1919 = hxc_caxecraft_domain_GameSession_authoredItemsView(hxc_l_tmp_load_result_n1918, &hxc_l_tmp_length_n1920);
                struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1921 = hxc_l_levelView_h37960af8bef4;
                hxc_l_gc_roots[637] = (const void *)hxc_l_tmp_load_result_n1921;
                struct Texture hxc_l_tmp_load_result_n1922 = hxc_l_itemTexture;
                bool hxc_l_tmp_load_result_n1923 = hxc_l_itemTextureReady;
                struct Texture hxc_l_tmp_load_result_n1924 = hxc_l_adventureItemTexture;
                hxc_caxecraft_app_AuthoredItemRenderer_drawWorldItems(hxc_l_tmp_load_result_n1916, hxc_l_tmp_load_result_n1917, hxc_l_tmp_instance_call_result_n1919, hxc_l_tmp_length_n1920, hxc_l_tmp_load_result_n1921, hxc_l_tmp_load_result_n1922, hxc_l_tmp_load_result_n1923, hxc_l_tmp_load_result_n1924, hxc_l_adventureItemTextureReady);
                if (hxc_l_hit.hxc_hit)
                {
                  double hxc_l_y_h9a8d358ba71f = (double)hxc_l_hit.hxc_cellY + 0.5;
                  double hxc_l_z_h6cfeff4f71f6 = (double)hxc_l_hit.hxc_cellZ + 0.5;
                  int32_t hxc_l_tmp_record_field_load_result_n1929 = hxc_l_hit.hxc_cellX;
                  double hxc_l_tmp_load_result_n1930 = hxc_l_y_h9a8d358ba71f;
                  DrawCubeWires((struct Vector3){ .x = (float)((double)hxc_l_tmp_record_field_load_result_n1929 + 0.5), .y = (float)hxc_l_tmp_load_result_n1930, .z = (float)hxc_l_z_h6cfeff4f71f6 }, (float)1.04, (float)1.04, (float)1.04, (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 });
                }
                EndMode3D();
                if (hxc_l_cameraWaterBlend > 0.0)
                {
                  int32_t hxc_l_overlayAlpha = hxc_f64_to_i32_saturating(105.0 * hxc_l_cameraWaterBlend);
                  int32_t hxc_l_tmp_native_call_result_n1936 = GetScreenWidth();
                  int32_t hxc_l_width_he49c1c9e5172 = (int32_t)hxc_l_tmp_native_call_result_n1936;
                  int32_t hxc_l_tmp_native_call_result_n1937 = GetScreenHeight();
                  int32_t hxc_l_height_h3f2c561783ee = (int32_t)hxc_l_tmp_native_call_result_n1937;
                  uint8_t hxc_l_red_hd42595d50725 = (uint8_t)31;
                  uint8_t hxc_l_green_hd85c664fdb44 = (uint8_t)115;
                  uint8_t hxc_l_blue_h5d658e597e97 = (uint8_t)154;
                  int32_t hxc_l_tmp_load_result_n1938 = hxc_l_overlayAlpha;
                  int32_t hxc_l_tmp_conditional_result_n532 = 0;
                  if (hxc_l_tmp_load_result_n1938 < 0)
                  {
                    hxc_l_tmp_conditional_result_n532 = 0;
                  }
                  else
                  {
                    int32_t hxc_l_tmp_load_result_n1939 = hxc_l_overlayAlpha;
                    int32_t hxc_l_tmp_conditional_result_n533 = 0;
                    if (hxc_l_tmp_load_result_n1939 > 255)
                    {
                      hxc_l_tmp_conditional_result_n533 = 255;
                    }
                    else
                    {
                      hxc_l_tmp_conditional_result_n533 = hxc_l_overlayAlpha;
                    }
                    hxc_l_tmp_conditional_result_n532 = hxc_l_tmp_conditional_result_n533;
                  }
                  uint8_t hxc_l_alpha_he909c05b142c = (uint8_t)hxc_l_tmp_conditional_result_n532;
                  uint8_t hxc_l_tmp_load_result_n1943 = hxc_l_red_hd42595d50725;
                  uint8_t hxc_l_tmp_load_result_n1944 = hxc_l_green_hd85c664fdb44;
                  uint8_t hxc_l_tmp_load_result_n1945 = hxc_l_blue_h5d658e597e97;
                  struct Color hxc_l_this1_hbfbbd03c9bb5 = (struct Color){ .r = hxc_l_tmp_load_result_n1943, .g = hxc_l_tmp_load_result_n1944, .b = hxc_l_tmp_load_result_n1945, .a = hxc_l_alpha_he909c05b142c };
                  int32_t hxc_l_tmp_load_result_n1948 = hxc_l_width_he49c1c9e5172;
                  int32_t hxc_l_tmp_load_result_n1949 = hxc_l_height_h3f2c561783ee;
                  DrawRectangle((int32_t)0, (int32_t)0, (int32_t)hxc_l_tmp_load_result_n1948, (int32_t)hxc_l_tmp_load_result_n1949, hxc_l_this1_hbfbbd03c9bb5);
                }
                struct hxc_optional_caxecraft_app_ConversationState hxc_l_tmp_load_result_n1951 = hxc_l_conversation;
                bool hxc_l_tmp_short_circuit_result_n536 = !hxc_l_tmp_load_result_n1951.hxc_has_value;
                if (!!hxc_l_tmp_load_result_n1951.hxc_has_value)
                {
                  bool hxc_l_tmp_string_null_equality_result_n537 = hxc_l_activeDialogue.data == NULL;
                  hxc_l_tmp_short_circuit_result_n536 = hxc_l_tmp_string_null_equality_result_n537;
                }
                bool hxc_l_tmp_short_circuit_load_result_n1954 = hxc_l_tmp_short_circuit_result_n536;
                hxc_string hxc_l_tmp_conditional_managed_result_n538 = { 0 };
                if (hxc_l_tmp_short_circuit_load_result_n1954)
                {
                  hxc_l_tmp_conditional_managed_result_n538 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
                  if (hxc_string_retain(hxc_l_tmp_conditional_managed_result_n538) != HXC_STATUS_OK)
                  {
                    abort();
                  }
                }
                else
                {
                  struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1955 = hxc_l_levelView_h37960af8bef4;
                  hxc_l_gc_roots[638] = (const void *)hxc_l_tmp_load_result_n1955;
                  if (hxc_l_tmp_load_result_n1955 == NULL)
                  {
                    abort();
                  }
                  struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_class_field_load_result_n1956 = (*hxc_l_tmp_load_result_n1955).hxc_levelPresentation;
                  hxc_l_gc_roots[639] = (const void *)hxc_l_tmp_class_field_load_result_n1956;
                  if (hxc_l_tmp_class_field_load_result_n1956 == NULL)
                  {
                    abort();
                  }
                  hxc_string hxc_l_tmp_load_result_n1957 = hxc_l_activeDialogue;
                  struct hxc_optional_caxecraft_app_ConversationState hxc_l_tmp_load_result_n1958 = hxc_l_conversation;
                  if (!hxc_l_tmp_load_result_n1958.hxc_has_value)
                  {
                    abort();
                  }
                  hxc_string hxc_l_tmp_instance_call_result_n1961 = hxc_caxecraft_content_RuntimeLevelPresentation_dialogueSpeaker(hxc_l_tmp_class_field_load_result_n1956, hxc_l_tmp_load_result_n1957, hxc_l_tmp_load_result_n1958.hxc_value.hxc_lineIndex);
                  hxc_l_tmp_conditional_managed_result_n538 = hxc_l_tmp_instance_call_result_n1961;
                }
                hxc_string hxc_l_tmp_conditional_managed_move_result_n1962 = hxc_l_tmp_conditional_managed_result_n538;
                hxc_l_conversationSpeaker = hxc_l_tmp_conditional_managed_move_result_n1962;
                double hxc_l_tmp_record_field_load_result_n1963 = hxc_l_character.hxc_body.hxc_x;
                double hxc_l_tmp_record_field_load_result_n1964 = hxc_l_character.hxc_body.hxc_z;
                struct hxc_caxecraft_domain_VitalsState hxc_l_tmp_record_field_load_result_n1965 = hxc_l_character.hxc_vitals;
                int32_t hxc_l_tmp_load_result_n1966 = hxc_l_aquaticEquipmentCode;
                int32_t hxc_l_tmp_load_result_n1967 = hxc_l_aquaticEquipmentFrames;
                bool hxc_l_tmp_record_field_load_result_n1968 = hxc_l_character.hxc_aquatic.hxc_headSubmerged;
                int32_t hxc_l_tmp_record_field_load_result_n1969 = hxc_l_character.hxc_aquatic.hxc_breathTicks;
                struct hxc_caxecraft_app_HudCharacterView hxc_l_hudView_hf2a694892032 = (struct hxc_caxecraft_app_HudCharacterView){ .hxc_aquaticEquipmentCode = hxc_l_tmp_load_result_n1966, .hxc_aquaticEquipmentVisible = hxc_l_tmp_load_result_n1967 > 0, .hxc_breathTicks = hxc_l_tmp_record_field_load_result_n1969, .hxc_headSubmerged = hxc_l_tmp_record_field_load_result_n1968, .hxc_maximumBreathTicks = hxc_l_character.hxc_aquaticProfile.hxc_maximumBreathTicks, .hxc_vitals = hxc_l_tmp_record_field_load_result_n1965, .hxc_x = hxc_l_tmp_record_field_load_result_n1963, .hxc_z = hxc_l_tmp_record_field_load_result_n1964 };
                bool hxc_l_hudView1 = hxc_l_placementBlockedFrames > 0;
                bool hxc_l_hudView2 = hxc_l_strikeHitFrames > 0;
                bool hxc_l_hudView3 = hxc_l_enemyDefeatedFrames > 0;
                bool hxc_l_hudView4 = hxc_l_enemyAttackFrames > 0;
                bool hxc_l_hudView5 = hxc_l_pickupFrames > 0;
                bool hxc_l_hudView6 = hxc_l_recoveryFeedbackFrames > 0;
                struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1978 = hxc_l_levelView_h37960af8bef4;
                hxc_l_gc_roots[640] = (const void *)hxc_l_tmp_load_result_n1978;
                if (hxc_l_tmp_load_result_n1978 == NULL)
                {
                  abort();
                }
                struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_class_field_load_result_n1979 = (*hxc_l_tmp_load_result_n1978).hxc_levelPresentation;
                hxc_l_gc_roots[641] = (const void *)hxc_l_tmp_class_field_load_result_n1979;
                struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_levelView1_h3048d8aa6837 = hxc_l_tmp_class_field_load_result_n1979;
                hxc_string hxc_l_hudView7_h63055266f297 = { 0 };
                switch (hxc_l_locale) {
                  case 0:
                    {
                      hxc_l_hudView7_h63055266f297 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
                      if (hxc_string_retain(hxc_l_hudView7_h63055266f297) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      break;
                    }
                  case 1:
                    {
                      hxc_l_hudView7_h63055266f297 = (hxc_string){ (const uint8_t *)"es-mx", 5, true, NULL };
                      if (hxc_string_retain(hxc_l_hudView7_h63055266f297) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      break;
                    }
                  default:
                    {
                      hxc_l_hudView7_h63055266f297 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
                      if (hxc_string_retain(hxc_l_hudView7_h63055266f297) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      break;
                    }
                }
                hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n1981 = hxc_l_hudView7_h63055266f297;
                hxc_l_tmp_managed_flow_owner_n548 = hxc_l_tmp_managed_flow_carrier_move_result_n1981;
                struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_load_result_n1982 = hxc_l_levelView1_h3048d8aa6837;
                hxc_l_gc_roots[642] = (const void *)hxc_l_tmp_load_result_n1982;
                if (hxc_l_tmp_load_result_n1982 == NULL)
                {
                  abort();
                }
                hxc_string hxc_l_tmp_load_result_n1983 = hxc_l_conversationSpeaker;
                hxc_string hxc_l_tmp_instance_call_result_n1985 = hxc_caxecraft_content_RuntimeLevelPresentation_speakerName(hxc_l_tmp_load_result_n1982, hxc_l_tmp_load_result_n1983, hxc_l_tmp_managed_flow_owner_n548);
                hxc_l_hudView7_hea459203f814 = hxc_l_tmp_instance_call_result_n1985;
                struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1986 = hxc_l_levelView_h37960af8bef4;
                hxc_l_gc_roots[643] = (const void *)hxc_l_tmp_load_result_n1986;
                if (hxc_l_tmp_load_result_n1986 == NULL)
                {
                  abort();
                }
                hxc_string hxc_l_tmp_instance_call_result_n1988 = hxc_caxecraft_app_PlayableLevelView_dialogueActorPresentationAsset(hxc_l_tmp_load_result_n1986, hxc_l_conversationSpeaker);
                hxc_l_hudView8 = hxc_l_tmp_instance_call_result_n1988;
                struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1989 = hxc_l_levelView_h37960af8bef4;
                hxc_l_gc_roots[644] = (const void *)hxc_l_tmp_load_result_n1989;
                if (hxc_l_tmp_load_result_n1989 == NULL)
                {
                  abort();
                }
                int32_t hxc_l_tmp_instance_call_result_n1991 = hxc_caxecraft_app_PlayableLevelView_dialogueActorPresentationCell(hxc_l_tmp_load_result_n1989, hxc_l_conversationSpeaker);
                int32_t hxc_l_hudView9 = hxc_l_tmp_instance_call_result_n1991;
                enum hxc_caxecraft_domain_ActorControllerPhase hxc_l_enemyPhase_hc9c3162dd48c = hxc_l_enemyPhase_h906f9c33685e.hxc_phase;
                hxc_string hxc_l_locale1_h345c68c8f5a7 = { 0 };
                switch (hxc_l_locale) {
                  case 0:
                    {
                      hxc_l_locale1_h345c68c8f5a7 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
                      if (hxc_string_retain(hxc_l_locale1_h345c68c8f5a7) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      break;
                    }
                  case 1:
                    {
                      hxc_l_locale1_h345c68c8f5a7 = (hxc_string){ (const uint8_t *)"es-mx", 5, true, NULL };
                      if (hxc_string_retain(hxc_l_locale1_h345c68c8f5a7) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      break;
                    }
                  default:
                    {
                      hxc_l_locale1_h345c68c8f5a7 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
                      if (hxc_string_retain(hxc_l_locale1_h345c68c8f5a7) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      break;
                    }
                }
                hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n1994 = hxc_l_locale1_h345c68c8f5a7;
                hxc_l_tmp_managed_flow_owner_n554 = hxc_l_tmp_managed_flow_carrier_move_result_n1994;
                struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1995 = hxc_l_levelView_h37960af8bef4;
                hxc_l_gc_roots[645] = (const void *)hxc_l_tmp_load_result_n1995;
                if (hxc_l_tmp_load_result_n1995 == NULL)
                {
                  abort();
                }
                struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_class_field_load_result_n1996 = (*hxc_l_tmp_load_result_n1995).hxc_levelPresentation;
                hxc_l_gc_roots[646] = (const void *)hxc_l_tmp_class_field_load_result_n1996;
                struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_this_h59f5cc927946 = hxc_l_tmp_class_field_load_result_n1996;
                struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_load_result_n1997 = hxc_l_this_h59f5cc927946;
                hxc_l_gc_roots[647] = (const void *)hxc_l_tmp_load_result_n1997;
                if (hxc_l_tmp_load_result_n1997 == NULL)
                {
                  abort();
                }
                struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_load_result_n1998 = hxc_l_this_h59f5cc927946;
                hxc_l_gc_roots[648] = (const void *)hxc_l_tmp_load_result_n1998;
                if (hxc_l_tmp_load_result_n1998 == NULL)
                {
                  abort();
                }
                struct hxc_caxecraft_scenario_ScenarioText hxc_l_tmp_class_field_load_result_n1999 = (*hxc_l_tmp_load_result_n1998).hxc_title;
                hxc_string hxc_l_tmp_instance_call_result_n2001 = hxc_caxecraft_content_RuntimeLevelPresentation_resolve(hxc_l_tmp_load_result_n1997, hxc_l_tmp_class_field_load_result_n1999, hxc_l_tmp_managed_flow_owner_n554);
                hxc_l_hudView10 = hxc_l_tmp_instance_call_result_n2001;
                hxc_string hxc_l_locale1_h1927aa0446fc = { 0 };
                switch (hxc_l_locale) {
                  case 0:
                    {
                      hxc_l_locale1_h1927aa0446fc = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
                      if (hxc_string_retain(hxc_l_locale1_h1927aa0446fc) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      break;
                    }
                  case 1:
                    {
                      hxc_l_locale1_h1927aa0446fc = (hxc_string){ (const uint8_t *)"es-mx", 5, true, NULL };
                      if (hxc_string_retain(hxc_l_locale1_h1927aa0446fc) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      break;
                    }
                  default:
                    {
                      hxc_l_locale1_h1927aa0446fc = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
                      if (hxc_string_retain(hxc_l_locale1_h1927aa0446fc) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      break;
                    }
                }
                hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n2003 = hxc_l_locale1_h1927aa0446fc;
                hxc_l_tmp_managed_flow_owner_n558 = hxc_l_tmp_managed_flow_carrier_move_result_n2003;
                struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n2004 = hxc_l_levelView_h37960af8bef4;
                hxc_l_gc_roots[649] = (const void *)hxc_l_tmp_load_result_n2004;
                if (hxc_l_tmp_load_result_n2004 == NULL)
                {
                  abort();
                }
                struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_class_field_load_result_n2005 = (*hxc_l_tmp_load_result_n2004).hxc_levelPresentation;
                hxc_l_gc_roots[650] = (const void *)hxc_l_tmp_class_field_load_result_n2005;
                if (hxc_l_tmp_class_field_load_result_n2005 == NULL)
                {
                  abort();
                }
                hxc_string hxc_l_tmp_load_result_n2006 = hxc_l_currentObjectiveId;
                hxc_string hxc_l_tmp_instance_call_result_n2008 = hxc_caxecraft_content_RuntimeLevelPresentation_objectiveTitle(hxc_l_tmp_class_field_load_result_n2005, hxc_l_tmp_load_result_n2006, hxc_l_tmp_managed_flow_owner_n558);
                hxc_l_hudView11 = hxc_l_tmp_instance_call_result_n2008;
                hxc_string hxc_l_hudView12 = { 0 };
                bool hxc_l_tmp_string_null_equality_result_n561 = hxc_l_latestJournalId.data == NULL;
                if (hxc_l_tmp_string_null_equality_result_n561)
                {
                  hxc_l_hudView12 = (hxc_string){ (const uint8_t *)"", 0, true, NULL };
                  if (hxc_string_retain(hxc_l_hudView12) != HXC_STATUS_OK)
                  {
                    abort();
                  }
                }
                else
                {
                  struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n2011 = hxc_l_levelView_h37960af8bef4;
                  hxc_l_gc_roots[651] = (const void *)hxc_l_tmp_load_result_n2011;
                  if (hxc_l_tmp_load_result_n2011 == NULL)
                  {
                    abort();
                  }
                  struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_class_field_load_result_n2012 = (*hxc_l_tmp_load_result_n2011).hxc_levelPresentation;
                  hxc_l_gc_roots[652] = (const void *)hxc_l_tmp_class_field_load_result_n2012;
                  struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_levelView_he9fc7d28cc19 = hxc_l_tmp_class_field_load_result_n2012;
                  hxc_string hxc_l_hudView_h6849d8ef94c5 = { 0 };
                  switch (hxc_l_locale) {
                    case 0:
                      {
                        hxc_l_hudView_h6849d8ef94c5 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
                        if (hxc_string_retain(hxc_l_hudView_h6849d8ef94c5) != HXC_STATUS_OK)
                        {
                          abort();
                        }
                        break;
                      }
                    case 1:
                      {
                        hxc_l_hudView_h6849d8ef94c5 = (hxc_string){ (const uint8_t *)"es-mx", 5, true, NULL };
                        if (hxc_string_retain(hxc_l_hudView_h6849d8ef94c5) != HXC_STATUS_OK)
                        {
                          abort();
                        }
                        break;
                      }
                    default:
                      {
                        hxc_l_hudView_h6849d8ef94c5 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
                        if (hxc_string_retain(hxc_l_hudView_h6849d8ef94c5) != HXC_STATUS_OK)
                        {
                          abort();
                        }
                        break;
                      }
                  }
                  hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n2014 = hxc_l_hudView_h6849d8ef94c5;
                  hxc_l_tmp_managed_flow_owner_n564 = hxc_l_tmp_managed_flow_carrier_move_result_n2014;
                  struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_load_result_n2015 = hxc_l_levelView_he9fc7d28cc19;
                  hxc_l_gc_roots[653] = (const void *)hxc_l_tmp_load_result_n2015;
                  if (hxc_l_tmp_load_result_n2015 == NULL)
                  {
                    abort();
                  }
                  hxc_string hxc_l_tmp_load_result_n2016 = hxc_l_latestJournalId;
                  hxc_string hxc_l_tmp_instance_call_result_n2018 = hxc_caxecraft_content_RuntimeLevelPresentation_journalTitle(hxc_l_tmp_load_result_n2015, hxc_l_tmp_load_result_n2016, hxc_l_tmp_managed_flow_owner_n564);
                  hxc_l_hudView12 = hxc_l_tmp_instance_call_result_n2018;
                  if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n564) != HXC_STATUS_OK)
                  {
                    abort();
                  }
                }
                hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n2019 = hxc_l_hudView12;
                hxc_l_tmp_managed_flow_owner_n565 = hxc_l_tmp_managed_flow_carrier_move_result_n2019;
                hxc_string hxc_l_hudView13 = { 0 };
                bool hxc_l_tmp_string_null_equality_result_n567 = hxc_l_latestJournalId.data == NULL;
                if (hxc_l_tmp_string_null_equality_result_n567)
                {
                  hxc_l_hudView13 = (hxc_string){ (const uint8_t *)"", 0, true, NULL };
                  if (hxc_string_retain(hxc_l_hudView13) != HXC_STATUS_OK)
                  {
                    abort();
                  }
                }
                else
                {
                  struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n2022 = hxc_l_levelView_h37960af8bef4;
                  hxc_l_gc_roots[654] = (const void *)hxc_l_tmp_load_result_n2022;
                  if (hxc_l_tmp_load_result_n2022 == NULL)
                  {
                    abort();
                  }
                  struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_class_field_load_result_n2023 = (*hxc_l_tmp_load_result_n2022).hxc_levelPresentation;
                  hxc_l_gc_roots[655] = (const void *)hxc_l_tmp_class_field_load_result_n2023;
                  struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_levelView_h55301f3799b0 = hxc_l_tmp_class_field_load_result_n2023;
                  hxc_string hxc_l_hudView_h729d1639a8d7 = { 0 };
                  switch (hxc_l_locale) {
                    case 0:
                      {
                        hxc_l_hudView_h729d1639a8d7 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
                        if (hxc_string_retain(hxc_l_hudView_h729d1639a8d7) != HXC_STATUS_OK)
                        {
                          abort();
                        }
                        break;
                      }
                    case 1:
                      {
                        hxc_l_hudView_h729d1639a8d7 = (hxc_string){ (const uint8_t *)"es-mx", 5, true, NULL };
                        if (hxc_string_retain(hxc_l_hudView_h729d1639a8d7) != HXC_STATUS_OK)
                        {
                          abort();
                        }
                        break;
                      }
                    default:
                      {
                        hxc_l_hudView_h729d1639a8d7 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
                        if (hxc_string_retain(hxc_l_hudView_h729d1639a8d7) != HXC_STATUS_OK)
                        {
                          abort();
                        }
                        break;
                      }
                  }
                  hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n2025 = hxc_l_hudView_h729d1639a8d7;
                  hxc_l_tmp_managed_flow_owner_n570 = hxc_l_tmp_managed_flow_carrier_move_result_n2025;
                  struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_load_result_n2026 = hxc_l_levelView_h55301f3799b0;
                  hxc_l_gc_roots[656] = (const void *)hxc_l_tmp_load_result_n2026;
                  if (hxc_l_tmp_load_result_n2026 == NULL)
                  {
                    abort();
                  }
                  hxc_string hxc_l_tmp_load_result_n2027 = hxc_l_latestJournalId;
                  hxc_string hxc_l_tmp_instance_call_result_n2029 = hxc_caxecraft_content_RuntimeLevelPresentation_journalBody(hxc_l_tmp_load_result_n2026, hxc_l_tmp_load_result_n2027, hxc_l_tmp_managed_flow_owner_n570);
                  hxc_l_hudView13 = hxc_l_tmp_instance_call_result_n2029;
                  if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n570) != HXC_STATUS_OK)
                  {
                    abort();
                  }
                }
                hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n2030 = hxc_l_hudView13;
                hxc_l_tmp_managed_flow_owner_n571 = hxc_l_tmp_managed_flow_carrier_move_result_n2030;
                bool hxc_l_tmp_load_result_n2031 = hxc_l_debugHudVisible;
                int32_t hxc_l_tmp_load_result_n2032 = hxc_l_totalVisible;
                int32_t hxc_l_tmp_load_result_n2033 = hxc_l_totalDrawCalls;
                int32_t hxc_l_tmp_load_result_n2034 = hxc_l_frameCount;
                int32_t hxc_l_tmp_load_result_n2035 = hxc_l_completedTicks;
                struct hxc_caxecraft_app_HudCharacterView hxc_l_tmp_load_result_n2037 = hxc_l_hudView_hf2a694892032;
                bool hxc_l_tmp_load_result_n2038 = hxc_l_hudView1;
                bool hxc_l_tmp_load_result_n2039 = hxc_l_hudView2;
                bool hxc_l_tmp_load_result_n2040 = hxc_l_hudView3;
                bool hxc_l_tmp_load_result_n2041 = hxc_l_hudView4;
                bool hxc_l_tmp_load_result_n2042 = hxc_l_hudView5;
                int32_t hxc_l_tmp_load_result_n2043 = hxc_l_pickupAmount;
                int32_t hxc_l_tmp_load_result_n2044 = hxc_l_inventoryFullReason;
                int32_t hxc_l_tmp_load_result_n2045 = hxc_l_recoveryFeedback;
                bool hxc_l_tmp_load_result_n2046 = hxc_l_hudView6;
                bool hxc_l_tmp_load_result_n2048 = hxc_l_paused;
                bool hxc_l_tmp_load_result_n2049 = hxc_l_captured;
                int32_t hxc_l_tmp_load_result_n2050 = hxc_l_promptDevice;
                struct hxc_caxecraft_domain_RaycastHit hxc_l_tmp_load_result_n2051 = hxc_l_hit;
                int32_t hxc_l_tmp_load_result_n2052 = hxc_l_selectedMode;
                int32_t hxc_l_tmp_load_result_n2053 = hxc_l_locale;
                struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_load_result_n2054 = hxc_l_inventory;
                struct hxc_optional_caxecraft_app_ConversationState hxc_l_tmp_load_result_n2055 = hxc_l_conversation;
                hxc_string hxc_l_tmp_record_field_activeDialogue_owner_n573 = hxc_l_activeDialogue;
                if (hxc_string_retain(hxc_l_tmp_record_field_activeDialogue_owner_n573) != HXC_STATUS_OK)
                {
                  abort();
                }
                hxc_string hxc_l_tmp_record_field_activeDialogue_owned_load_result_n2057 = hxc_l_tmp_record_field_activeDialogue_owner_n573;
                hxc_string hxc_l_tmp_record_field_conversationSpeaker_owner_n574 = hxc_l_conversationSpeaker;
                if (hxc_string_retain(hxc_l_tmp_record_field_conversationSpeaker_owner_n574) != HXC_STATUS_OK)
                {
                  abort();
                }
                hxc_string hxc_l_tmp_record_field_conversationSpeaker_owned_load_result_n2059 = hxc_l_tmp_record_field_conversationSpeaker_owner_n574;
                hxc_string hxc_l_tmp_record_field_conversationSpeakerName_owner_n575 = hxc_l_hudView7_hea459203f814;
                if (hxc_string_retain(hxc_l_tmp_record_field_conversationSpeakerName_owner_n575) != HXC_STATUS_OK)
                {
                  abort();
                }
                hxc_string hxc_l_tmp_record_field_conversationSpeakerName_owned_load_result_n2061 = hxc_l_tmp_record_field_conversationSpeakerName_owner_n575;
                hxc_string hxc_l_tmp_record_field_conversationPortraitAsset_owner_n576 = hxc_l_hudView8;
                if (hxc_string_retain(hxc_l_tmp_record_field_conversationPortraitAsset_owner_n576) != HXC_STATUS_OK)
                {
                  abort();
                }
                hxc_string hxc_l_tmp_record_field_conversationPortraitAsset_owned_load_result_n2063 = hxc_l_tmp_record_field_conversationPortraitAsset_owner_n576;
                int32_t hxc_l_tmp_load_result_n2064 = hxc_l_hudView9;
                enum hxc_caxecraft_app_InteractionPrompt hxc_l_tmp_load_result_n2065 = hxc_l_availableInteractionPrompt;
                struct hxc_caxecraft_domain_Character hxc_l_tmp_load_result_n2066 = hxc_l_enemyActor;
                enum hxc_caxecraft_domain_ActorControllerPhase hxc_l_tmp_load_result_n2067 = hxc_l_enemyPhase_hc9c3162dd48c;
                hxc_string hxc_l_tmp_record_field_levelTitle_owner_n577 = hxc_l_hudView10;
                if (hxc_string_retain(hxc_l_tmp_record_field_levelTitle_owner_n577) != HXC_STATUS_OK)
                {
                  abort();
                }
                hxc_string hxc_l_tmp_record_field_levelTitle_owned_load_result_n2069 = hxc_l_tmp_record_field_levelTitle_owner_n577;
                hxc_string hxc_l_tmp_record_field_objectiveTitle_owner_n578 = hxc_l_hudView11;
                if (hxc_string_retain(hxc_l_tmp_record_field_objectiveTitle_owner_n578) != HXC_STATUS_OK)
                {
                  abort();
                }
                hxc_string hxc_l_tmp_record_field_objectiveTitle_owned_load_result_n2071 = hxc_l_tmp_record_field_objectiveTitle_owner_n578;
                hxc_string hxc_l_tmp_record_field_journalTitle_owner_n579 = hxc_l_tmp_managed_flow_owner_n565;
                if (hxc_string_retain(hxc_l_tmp_record_field_journalTitle_owner_n579) != HXC_STATUS_OK)
                {
                  abort();
                }
                hxc_string hxc_l_tmp_record_field_journalTitle_owned_load_result_n2073 = hxc_l_tmp_record_field_journalTitle_owner_n579;
                hxc_string hxc_l_tmp_record_field_journalBody_owner_n580 = hxc_l_tmp_managed_flow_owner_n571;
                if (hxc_string_retain(hxc_l_tmp_record_field_journalBody_owner_n580) != HXC_STATUS_OK)
                {
                  abort();
                }
                hxc_string hxc_l_tmp_record_field_journalBody_owned_load_result_n2075 = hxc_l_tmp_record_field_journalBody_owner_n580;
                struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n2076 = hxc_l_levelView_h37960af8bef4;
                hxc_l_gc_roots[657] = (const void *)hxc_l_tmp_load_result_n2076;
                if (hxc_l_tmp_load_result_n2076 == NULL)
                {
                  abort();
                }
                struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_class_field_load_result_n2077 = (*hxc_l_tmp_load_result_n2076).hxc_levelPresentation;
                hxc_l_gc_roots[658] = (const void *)hxc_l_tmp_class_field_load_result_n2077;
                hxc_l_gc_roots[659] = (const void *)(struct hxc_caxecraft_app_HudView){ .hxc_activeDialogue = hxc_l_tmp_record_field_activeDialogue_owned_load_result_n2057, .hxc_character = hxc_l_tmp_load_result_n2037, .hxc_conversation = hxc_l_tmp_load_result_n2055, .hxc_conversationPortraitAsset = hxc_l_tmp_record_field_conversationPortraitAsset_owned_load_result_n2063, .hxc_conversationPortraitCell = hxc_l_tmp_load_result_n2064, .hxc_conversationSpeaker = hxc_l_tmp_record_field_conversationSpeaker_owned_load_result_n2059, .hxc_conversationSpeakerName = hxc_l_tmp_record_field_conversationSpeakerName_owned_load_result_n2061, .hxc_debugMetricsVisible = hxc_l_tmp_load_result_n2031, .hxc_enemy = hxc_l_tmp_load_result_n2066, .hxc_enemyPhase = hxc_l_tmp_load_result_n2067, .hxc_feedback = (struct hxc_caxecraft_app_HudFeedbackView){ .hxc_enemyAttacked = hxc_l_tmp_load_result_n2041, .hxc_enemyDefeated = hxc_l_tmp_load_result_n2040, .hxc_inventoryFullReason = hxc_l_tmp_load_result_n2044, .hxc_pickedUp = hxc_l_tmp_load_result_n2042, .hxc_pickupAmount = hxc_l_tmp_load_result_n2043, .hxc_placementBlocked = hxc_l_tmp_load_result_n2038, .hxc_recoveryDecision = hxc_l_tmp_load_result_n2045, .hxc_recoveryVisible = hxc_l_tmp_load_result_n2046, .hxc_strikeHit = hxc_l_tmp_load_result_n2039 }, .hxc_hit = hxc_l_tmp_load_result_n2051, .hxc_interactionPrompt = hxc_l_tmp_load_result_n2065, .hxc_inventory = hxc_l_tmp_load_result_n2054, .hxc_journalBody = hxc_l_tmp_record_field_journalBody_owned_load_result_n2075, .hxc_journalTitle = hxc_l_tmp_record_field_journalTitle_owned_load_result_n2073, .hxc_levelTitle = hxc_l_tmp_record_field_levelTitle_owned_load_result_n2069, .hxc_locale = hxc_l_tmp_load_result_n2053, .hxc_metrics = (struct hxc_caxecraft_app_HudMetricsView){ .hxc_completedTicks = hxc_l_tmp_load_result_n2035, .hxc_drawCalls = hxc_l_tmp_load_result_n2033, .hxc_renderedFrames = hxc_l_tmp_load_result_n2034, .hxc_visibleBlocks = hxc_l_tmp_load_result_n2032 }, .hxc_mode = hxc_l_tmp_load_result_n2052, .hxc_objectiveTitle = hxc_l_tmp_record_field_objectiveTitle_owned_load_result_n2071, .hxc_paused = hxc_l_tmp_load_result_n2048, .hxc_pointerCaptured = hxc_l_tmp_load_result_n2049, .hxc_presentation = hxc_l_tmp_class_field_load_result_n2077, .hxc_promptDevice = hxc_l_tmp_load_result_n2050 }.hxc_presentation;
                hxc_l_hudView_hb228802b1420 = (struct hxc_caxecraft_app_HudView){ .hxc_activeDialogue = hxc_l_tmp_record_field_activeDialogue_owned_load_result_n2057, .hxc_character = hxc_l_tmp_load_result_n2037, .hxc_conversation = hxc_l_tmp_load_result_n2055, .hxc_conversationPortraitAsset = hxc_l_tmp_record_field_conversationPortraitAsset_owned_load_result_n2063, .hxc_conversationPortraitCell = hxc_l_tmp_load_result_n2064, .hxc_conversationSpeaker = hxc_l_tmp_record_field_conversationSpeaker_owned_load_result_n2059, .hxc_conversationSpeakerName = hxc_l_tmp_record_field_conversationSpeakerName_owned_load_result_n2061, .hxc_debugMetricsVisible = hxc_l_tmp_load_result_n2031, .hxc_enemy = hxc_l_tmp_load_result_n2066, .hxc_enemyPhase = hxc_l_tmp_load_result_n2067, .hxc_feedback = (struct hxc_caxecraft_app_HudFeedbackView){ .hxc_enemyAttacked = hxc_l_tmp_load_result_n2041, .hxc_enemyDefeated = hxc_l_tmp_load_result_n2040, .hxc_inventoryFullReason = hxc_l_tmp_load_result_n2044, .hxc_pickedUp = hxc_l_tmp_load_result_n2042, .hxc_pickupAmount = hxc_l_tmp_load_result_n2043, .hxc_placementBlocked = hxc_l_tmp_load_result_n2038, .hxc_recoveryDecision = hxc_l_tmp_load_result_n2045, .hxc_recoveryVisible = hxc_l_tmp_load_result_n2046, .hxc_strikeHit = hxc_l_tmp_load_result_n2039 }, .hxc_hit = hxc_l_tmp_load_result_n2051, .hxc_interactionPrompt = hxc_l_tmp_load_result_n2065, .hxc_inventory = hxc_l_tmp_load_result_n2054, .hxc_journalBody = hxc_l_tmp_record_field_journalBody_owned_load_result_n2075, .hxc_journalTitle = hxc_l_tmp_record_field_journalTitle_owned_load_result_n2073, .hxc_levelTitle = hxc_l_tmp_record_field_levelTitle_owned_load_result_n2069, .hxc_locale = hxc_l_tmp_load_result_n2053, .hxc_metrics = (struct hxc_caxecraft_app_HudMetricsView){ .hxc_completedTicks = hxc_l_tmp_load_result_n2035, .hxc_drawCalls = hxc_l_tmp_load_result_n2033, .hxc_renderedFrames = hxc_l_tmp_load_result_n2034, .hxc_visibleBlocks = hxc_l_tmp_load_result_n2032 }, .hxc_mode = hxc_l_tmp_load_result_n2052, .hxc_objectiveTitle = hxc_l_tmp_record_field_objectiveTitle_owned_load_result_n2071, .hxc_paused = hxc_l_tmp_load_result_n2048, .hxc_pointerCaptured = hxc_l_tmp_load_result_n2049, .hxc_presentation = hxc_l_tmp_class_field_load_result_n2077, .hxc_promptDevice = hxc_l_tmp_load_result_n2050 };
                struct hxc_caxecraft_app_HudView hxc_l_tmp_load_result_n2079 = hxc_l_hudView_hb228802b1420;
                hxc_l_gc_roots[660] = (const void *)hxc_l_tmp_load_result_n2079.hxc_presentation;
                struct hxc_caxecraft_app_HudResources hxc_l_tmp_load_result_n2080 = hxc_l_hudResources;
                struct hxc_caxecraft_app_RuntimeTextureAtlasCatalog *hxc_l_tmp_load_result_n2081 = hxc_l_runtimeTextures;
                hxc_l_gc_roots[661] = (const void *)hxc_l_tmp_load_result_n2081;
                struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_tmp_load_result_n2082 = hxc_l_contentRegistry;
                hxc_l_gc_roots[662] = (const void *)hxc_l_tmp_load_result_n2082;
                hxc_l_gc_roots[663] = (const void *)hxc_l_uiCatalog;
                hxc_caxecraft_app_CaxecraftApp_drawHud(hxc_l_tmp_load_result_n2079, hxc_l_tmp_load_result_n2080, hxc_l_tmp_load_result_n2081, hxc_l_tmp_load_result_n2082, hxc_l_uiCatalog);
                hxc_record_c5db1a9e_destroy(&hxc_l_hudView_hb228802b1420);
                if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n571) != HXC_STATUS_OK)
                {
                  abort();
                }
                if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n565) != HXC_STATUS_OK)
                {
                  abort();
                }
                if (hxc_string_release(&hxc_l_hudView11) != HXC_STATUS_OK)
                {
                  abort();
                }
                if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n558) != HXC_STATUS_OK)
                {
                  abort();
                }
                if (hxc_string_release(&hxc_l_hudView10) != HXC_STATUS_OK)
                {
                  abort();
                }
                if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n554) != HXC_STATUS_OK)
                {
                  abort();
                }
                if (hxc_string_release(&hxc_l_hudView8) != HXC_STATUS_OK)
                {
                  abort();
                }
                if (hxc_string_release(&hxc_l_hudView7_hea459203f814) != HXC_STATUS_OK)
                {
                  abort();
                }
                if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n548) != HXC_STATUS_OK)
                {
                  abort();
                }
                if (hxc_string_release(&hxc_l_conversationSpeaker) != HXC_STATUS_OK)
                {
                  abort();
                }
              }
            }
          }
        }
        EndDrawing();
        switch (hxc_l_editorActionForFrame.hxc_tag) {
          case hxc_caxecraft_app_EditorScreenAction_StayInEditor:
            {
              break;
            }
          case hxc_caxecraft_app_EditorScreenAction_ReturnToTitle:
            {
              enum hxc_caxecraft_app_AppScreen hxc_l_tmp_call_result_n2086 = hxc_caxecraft_app_AppScreen_closeEditor(hxc_l_screen);
              hxc_l_screen = hxc_l_tmp_call_result_n2086;
              break;
            }
          case hxc_caxecraft_app_EditorScreenAction_StartTestPlay:
            {
              if (hxc_l_editorActionForFrame.hxc_tag != hxc_caxecraft_app_EditorScreenAction_StartTestPlay)
              {
                abort();
              }
              struct hxc_bytes_ref *hxc_l_tmp_enum_payload_project_n2088 = hxc_l_editorActionForFrame.hxc_payload.hxc_StartTestPlay.hxc_canonical;
              struct hxc_bytes_ref *hxc_l_canonical = hxc_l_tmp_enum_payload_project_n2088;
              struct hxc_caxecraft_app_ActivePlayableLevel *hxc_l_tmp_load_result_n2089 = hxc_l_activeLevel;
              hxc_l_gc_roots[664] = (const void *)hxc_l_tmp_load_result_n2089;
              if (hxc_l_tmp_load_result_n2089 == NULL)
              {
                abort();
              }
              struct hxc_caxecraft_content_ActiveContent *hxc_l_tmp_class_field_load_result_n2090 = (*hxc_l_tmp_load_result_n2089).hxc_content;
              hxc_l_gc_roots[665] = (const void *)hxc_l_tmp_class_field_load_result_n2090;
              if (hxc_l_tmp_class_field_load_result_n2090 == NULL)
              {
                abort();
              }
              struct hxc_caxecraft_content_LoadedContentGeneration *hxc_l_tmp_class_field_load_result_n2091 = (*hxc_l_tmp_class_field_load_result_n2090).hxc_active;
              hxc_l_gc_roots[666] = (const void *)hxc_l_tmp_class_field_load_result_n2091;
              if (hxc_l_tmp_class_field_load_result_n2091 == NULL)
              {
                abort();
              }
              struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_class_field_load_result_n2092 = (*hxc_l_tmp_class_field_load_result_n2091).hxc_ownedSession;
              hxc_l_gc_roots[667] = (const void *)hxc_l_tmp_class_field_load_result_n2092;
              if (hxc_l_tmp_class_field_load_result_n2092 == NULL)
              {
                abort();
              }
              struct hxc_caxecraft_domain_GameView hxc_l_tmp_instance_call_result_n2093 = hxc_caxecraft_domain_GameSession_view(hxc_l_tmp_class_field_load_result_n2092);
              struct hxc_caxecraft_domain_Character hxc_l_ordinaryPlayer = hxc_l_tmp_instance_call_result_n2093.hxc_localPlayer;
              struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_tmp_load_result_n2095 = hxc_l_contentRegistry;
              hxc_l_gc_roots[668] = (const void *)hxc_l_tmp_load_result_n2095;
              if (hxc_l_tmp_load_result_n2095 == NULL)
              {
                abort();
              }
              struct hxc_compiler_interface_dispatch_caxecraft_scenario_ScenarioContentRegistry_value hxc_l_tmp_interface_value_n2096 = (struct hxc_compiler_interface_dispatch_caxecraft_scenario_ScenarioContentRegistry_value){ .object = hxc_l_tmp_load_result_n2095, .table = &hxc_itable_compiler_interface_dispatch_caxecraft_content_RuntimeContentRegistry_itable_layout_caxecraf_hb3f384641e1a61bf };
              struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_tmp_load_result_n2097 = hxc_l_contentRegistry;
              hxc_l_gc_roots[669] = (const void *)hxc_l_tmp_load_result_n2097;
              if (hxc_l_tmp_load_result_n2097 == NULL)
              {
                abort();
              }
              struct hxc_compiler_interface_dispatch_caxecraft_content_LevelContentResolver_value hxc_l_tmp_interface_value_n2098 = (struct hxc_compiler_interface_dispatch_caxecraft_content_LevelContentResolver_value){ .object = hxc_l_tmp_load_result_n2097, .table = &hxc_itable_compiler_interface_dispatch_caxecraft_content_RuntimeContentRegistry_itable_layout_caxecraf_h82ca4f24ca7a7b5a };
              struct hxc_caxecraft_content_ContentGenerationSequence *hxc_l_tmp_load_result_n2099 = hxc_l_generationSequence;
              hxc_l_gc_roots[670] = (const void *)hxc_l_tmp_load_result_n2099;
              struct hxc_caxecraft_app_EditorTestPlayRuntime *hxc_l_tmp_managed_class_result_n2100 = NULL;
              if (hxc_gc_allocate(&hxc_program_gc, &hxc_caxecraft_app_EditorTestPlayRuntime_descriptor, (void **)&hxc_l_tmp_managed_class_result_n2100) != HXC_STATUS_OK)
              {
                abort();
              }
              hxc_l_gc_roots[671] = (const void *)hxc_l_tmp_managed_class_result_n2100;
              hxc_compiler_constructor_caxecraft_app_EditorTestPlayRuntime(hxc_l_tmp_managed_class_result_n2100, hxc_l_tmp_interface_value_n2096, hxc_l_tmp_interface_value_n2098, hxc_l_tmp_load_result_n2099);
              hxc_l_gc_roots[672] = (const void *)hxc_l_tmp_managed_class_result_n2100;
              struct hxc_caxecraft_app_EditorTestPlayRuntime *hxc_l_proposedRuntime = hxc_l_tmp_managed_class_result_n2100;
              struct hxc_caxecraft_app_EditorTestPlayRuntime *hxc_l_tmp_load_result_n2101 = hxc_l_proposedRuntime;
              hxc_l_gc_roots[673] = (const void *)hxc_l_tmp_load_result_n2101;
              if (hxc_l_tmp_load_result_n2101 == NULL)
              {
                abort();
              }
              struct hxc_bytes_ref *hxc_l_tmp_record_field_canonical_owner_n584 = hxc_l_canonical;
              if (hxc_bytes_ref_retain(hxc_l_tmp_record_field_canonical_owner_n584) != HXC_STATUS_OK)
              {
                abort();
              }
              struct hxc_bytes_ref *hxc_l_tmp_record_field_canonical_owned_load_result_n2103 = hxc_l_tmp_record_field_canonical_owner_n584;
              int32_t hxc_l_tmp_record_field_load_result_n2104 = hxc_l_ordinaryPlayer.hxc_id;
              int32_t hxc_l_tmp_record_field_load_result_n2105 = hxc_l_ordinaryPlayer.hxc_vitals.hxc_health;
              hxc_l_tmp_instance_call_argument_0_owner_n585 = (struct hxc_caxecraft_app_EditorTestPlayRequest){ .hxc_canonical = hxc_l_tmp_record_field_canonical_owned_load_result_n2103, .hxc_playerOptions = (struct hxc_caxecraft_content_LevelPlayerOptions){ .hxc_aquaticProfile = hxc_l_ordinaryPlayer.hxc_aquaticProfile, .hxc_entityId = hxc_l_tmp_record_field_load_result_n2104, .hxc_initialHealth = hxc_l_tmp_record_field_load_result_n2105 } };
              struct hxc_caxecraft_app_EditorTestPlayStartResult hxc_l_tmp_instance_call_result_n2110 = hxc_caxecraft_app_EditorTestPlayRuntime_start(hxc_l_tmp_load_result_n2101, hxc_l_tmp_instance_call_argument_0_owner_n585);
              hxc_l_tmp_enum_switch_subject_owner_n586 = hxc_l_tmp_instance_call_result_n2110;
              switch (hxc_l_tmp_enum_switch_subject_owner_n586.hxc_tag) {
                case hxc_caxecraft_app_EditorTestPlayStartResult_EditorTestPlayStarted:
                  {
                    struct hxc_caxecraft_app_EditorTestPlayRuntime *hxc_l_tmp_load_result_n2112 = hxc_l_proposedRuntime;
                    hxc_l_gc_roots[674] = (const void *)hxc_l_tmp_load_result_n2112;
                    if (hxc_l_tmp_load_result_n2112 == NULL)
                    {
                      abort();
                    }
                    struct hxc_caxecraft_app_ActivePlayableLevel *hxc_l_tmp_class_field_load_result_n2113 = (*hxc_l_tmp_load_result_n2112).hxc_active;
                    hxc_l_gc_roots[675] = (const void *)hxc_l_tmp_class_field_load_result_n2113;
                    struct hxc_caxecraft_app_ActivePlayableLevel *hxc_l_testOwner = hxc_l_tmp_class_field_load_result_n2113;
                    hxc_l_gc_roots[676] = (const void *)hxc_l_testOwner;
                    hxc_l_gc_roots[677] = (const void *)NULL;
                    if (hxc_l_testOwner == NULL)
                    {
                      struct hxc_caxecraft_app_CaxecraftEditorScreen *hxc_l_tmp_load_result_n2115 = hxc_l_editorScreen;
                      hxc_l_gc_roots[680] = (const void *)hxc_l_tmp_load_result_n2115;
                      if (hxc_l_tmp_load_result_n2115 == NULL)
                      {
                        abort();
                      }
                      hxc_caxecraft_app_CaxecraftEditorScreen_finishTestPlay(hxc_l_tmp_load_result_n2115, false);
                    }
                    else
                    {
                      struct hxc_caxecraft_app_ActivePlayableLevel *hxc_l_tmp_load_result_n2116 = hxc_l_testOwner;
                      hxc_l_gc_roots[681] = (const void *)hxc_l_tmp_load_result_n2116;
                      if (hxc_l_tmp_load_result_n2116 == NULL)
                      {
                        abort();
                      }
                      struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_class_field_load_result_n2117 = (*hxc_l_tmp_load_result_n2116).hxc_activeView;
                      hxc_l_gc_roots[682] = (const void *)hxc_l_tmp_class_field_load_result_n2117;
                      struct hxc_caxecraft_app_PlayableLevelView *hxc_l_testLevel_hf7ba2c1e808b = hxc_l_tmp_class_field_load_result_n2117;
                      struct hxc_caxecraft_app_ActivePlayableLevel *hxc_l_tmp_load_result_n2118 = hxc_l_testOwner;
                      hxc_l_gc_roots[683] = (const void *)hxc_l_tmp_load_result_n2118;
                      if (hxc_l_tmp_load_result_n2118 == NULL)
                      {
                        abort();
                      }
                      struct hxc_caxecraft_content_ActiveContent *hxc_l_tmp_class_field_load_result_n2119 = (*hxc_l_tmp_load_result_n2118).hxc_content;
                      hxc_l_gc_roots[684] = (const void *)hxc_l_tmp_class_field_load_result_n2119;
                      if (hxc_l_tmp_class_field_load_result_n2119 == NULL)
                      {
                        abort();
                      }
                      struct hxc_caxecraft_content_LoadedContentGeneration *hxc_l_tmp_class_field_load_result_n2120 = (*hxc_l_tmp_class_field_load_result_n2119).hxc_active;
                      hxc_l_gc_roots[685] = (const void *)hxc_l_tmp_class_field_load_result_n2120;
                      if (hxc_l_tmp_class_field_load_result_n2120 == NULL)
                      {
                        abort();
                      }
                      struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_class_field_load_result_n2121 = (*hxc_l_tmp_class_field_load_result_n2120).hxc_ownedSession;
                      hxc_l_gc_roots[686] = (const void *)hxc_l_tmp_class_field_load_result_n2121;
                      struct hxc_caxecraft_domain_GameSession *hxc_l_testSession = hxc_l_tmp_class_field_load_result_n2121;
                      struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n2122 = hxc_l_testSession;
                      hxc_l_gc_roots[687] = (const void *)hxc_l_tmp_load_result_n2122;
                      if (hxc_l_tmp_load_result_n2122 == NULL)
                      {
                        abort();
                      }
                      struct hxc_caxecraft_domain_GameView hxc_l_tmp_instance_call_result_n2123 = hxc_caxecraft_domain_GameSession_view(hxc_l_tmp_load_result_n2122);
                      struct hxc_caxecraft_domain_GameView hxc_l_testView = hxc_l_tmp_instance_call_result_n2123;
                      struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n2124 = hxc_l_testSession;
                      hxc_l_gc_roots[688] = (const void *)hxc_l_tmp_load_result_n2124;
                      if (hxc_l_tmp_load_result_n2124 == NULL)
                      {
                        abort();
                      }
                      struct hxc_array_ref *hxc_l_tmp_instance_call_result_n2125 = hxc_caxecraft_domain_GameSession_actorControllerStateSnapshots(hxc_l_tmp_load_result_n2124);
                      hxc_l_testPhases = hxc_l_tmp_instance_call_result_n2125;
                      struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n2126 = hxc_l_testSession;
                      hxc_l_gc_roots[689] = (const void *)hxc_l_tmp_load_result_n2126;
                      if (hxc_l_tmp_load_result_n2126 == NULL)
                      {
                        abort();
                      }
                      struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n2127 = hxc_l_testLevel_hf7ba2c1e808b;
                      hxc_l_gc_roots[690] = (const void *)hxc_l_tmp_load_result_n2127;
                      if (hxc_l_tmp_load_result_n2127 == NULL)
                      {
                        abort();
                      }
                      int32_t hxc_l_tmp_class_field_load_result_n2128 = (*hxc_l_tmp_load_result_n2127).hxc_enemyId;
                      struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n2129 = hxc_caxecraft_domain_GameSession_readCharacter(hxc_l_tmp_load_result_n2126, hxc_l_tmp_class_field_load_result_n2128);
                      struct hxc_caxecraft_domain_Character hxc_l_testEnemy = hxc_l_tmp_instance_call_result_n2129;
                      struct hxc_array_ref *hxc_l_tmp_load_result_n2130 = hxc_l_testPhases;
                      struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n2131 = hxc_l_testLevel_hf7ba2c1e808b;
                      hxc_l_gc_roots[691] = (const void *)hxc_l_tmp_load_result_n2131;
                      if (hxc_l_tmp_load_result_n2131 == NULL)
                      {
                        abort();
                      }
                      int32_t hxc_l_tmp_class_field_load_result_n2132 = (*hxc_l_tmp_load_result_n2131).hxc_enemyId;
                      struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation hxc_l_tmp_call_result_n2134 = hxc_caxecraft_app_CaxecraftApp_observeActorPhase(hxc_l_tmp_load_result_n2130, hxc_l_tmp_class_field_load_result_n2132, hxc_caxecraft_domain_ActorControllerPhase_Resting);
                      struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation hxc_l_testEnemyPhase = hxc_l_tmp_call_result_n2134;
                      bool hxc_l_tmp_record_field_load_result_n2135 = hxc_l_testView.hxc_valid;
                      bool hxc_l_tmp_short_circuit_result_n594 = !hxc_l_tmp_record_field_load_result_n2135;
                      if (!!hxc_l_tmp_record_field_load_result_n2135)
                      {
                        hxc_l_tmp_short_circuit_result_n594 = hxc_l_testEnemy.hxc_id <= 0;
                      }
                      bool hxc_l_tmp_short_circuit_load_result_n2137 = hxc_l_tmp_short_circuit_result_n594;
                      bool hxc_l_tmp_short_circuit_result_n595 = hxc_l_tmp_short_circuit_load_result_n2137;
                      if (!hxc_l_tmp_short_circuit_load_result_n2137)
                      {
                        hxc_l_tmp_short_circuit_result_n595 = !hxc_l_testEnemyPhase.hxc_valid;
                      }
                      bool hxc_l_tmp_short_circuit_load_result_n2139 = hxc_l_tmp_short_circuit_result_n595;
                      bool hxc_l_tmp_short_circuit_result_n596 = hxc_l_tmp_short_circuit_load_result_n2139;
                      if (!hxc_l_tmp_short_circuit_load_result_n2139)
                      {
                        struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n2140 = hxc_l_testSession;
                        hxc_l_gc_roots[692] = (const void *)hxc_l_tmp_load_result_n2140;
                        struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n2141 = hxc_l_testLevel_hf7ba2c1e808b;
                        hxc_l_gc_roots[693] = (const void *)hxc_l_tmp_load_result_n2141;
                        bool hxc_l_tmp_call_result_n2143 = hxc_caxecraft_app_CaxecraftApp_dialogueActorsAreValid(hxc_l_tmp_load_result_n2140, hxc_l_tmp_load_result_n2141, hxc_l_testPhases);
                        hxc_l_tmp_short_circuit_result_n596 = !hxc_l_tmp_call_result_n2143;
                      }
                      if (hxc_l_tmp_short_circuit_result_n596)
                      {
                        struct hxc_caxecraft_app_EditorTestPlayRuntime *hxc_l_tmp_load_result_n2145 = hxc_l_proposedRuntime;
                        hxc_l_gc_roots[694] = (const void *)hxc_l_tmp_load_result_n2145;
                        if (hxc_l_tmp_load_result_n2145 == NULL)
                        {
                          abort();
                        }
                        hxc_caxecraft_app_EditorTestPlayRuntime_stop(hxc_l_tmp_load_result_n2145);
                        struct hxc_caxecraft_app_CaxecraftEditorScreen *hxc_l_tmp_load_result_n2146 = hxc_l_editorScreen;
                        hxc_l_gc_roots[695] = (const void *)hxc_l_tmp_load_result_n2146;
                        if (hxc_l_tmp_load_result_n2146 == NULL)
                        {
                          abort();
                        }
                        hxc_caxecraft_app_CaxecraftEditorScreen_finishTestPlay(hxc_l_tmp_load_result_n2146, false);
                      }
                      else
                      {
                        struct hxc_caxecraft_app_CaxecraftEditorScreen *hxc_l_tmp_load_result_n2147 = hxc_l_editorScreen;
                        hxc_l_gc_roots[696] = (const void *)hxc_l_tmp_load_result_n2147;
                        if (hxc_l_tmp_load_result_n2147 == NULL)
                        {
                          abort();
                        }
                        bool hxc_l_tmp_instance_call_result_n2148 = hxc_caxecraft_app_CaxecraftEditorScreen_beginTestPlay(hxc_l_tmp_load_result_n2147);
                        if (!hxc_l_tmp_instance_call_result_n2148)
                        {
                          struct hxc_caxecraft_app_EditorTestPlayRuntime *hxc_l_tmp_load_result_n2149 = hxc_l_proposedRuntime;
                          hxc_l_gc_roots[697] = (const void *)hxc_l_tmp_load_result_n2149;
                          if (hxc_l_tmp_load_result_n2149 == NULL)
                          {
                            abort();
                          }
                          hxc_caxecraft_app_EditorTestPlayRuntime_stop(hxc_l_tmp_load_result_n2149);
                          struct hxc_caxecraft_app_CaxecraftEditorScreen *hxc_l_tmp_load_result_n2150 = hxc_l_editorScreen;
                          hxc_l_gc_roots[698] = (const void *)hxc_l_tmp_load_result_n2150;
                          if (hxc_l_tmp_load_result_n2150 == NULL)
                          {
                            abort();
                          }
                          hxc_caxecraft_app_CaxecraftEditorScreen_finishTestPlay(hxc_l_tmp_load_result_n2150, false);
                        }
                        else
                        {
                          struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_load_result_n2151 = hxc_l_inventory;
                          struct hxc_optional_caxecraft_app_ConversationState hxc_l_tmp_load_result_n2152 = hxc_l_conversation;
                          hxc_string hxc_l_tmp_record_field_activeDialogue_owner_n598 = hxc_l_activeDialogue;
                          if (hxc_string_retain(hxc_l_tmp_record_field_activeDialogue_owner_n598) != HXC_STATUS_OK)
                          {
                            abort();
                          }
                          hxc_string hxc_l_tmp_record_field_activeDialogue_owned_load_result_n2154 = hxc_l_tmp_record_field_activeDialogue_owner_n598;
                          hxc_string hxc_l_tmp_record_field_latestJournalId_owner_n599 = hxc_l_latestJournalId;
                          if (hxc_string_retain(hxc_l_tmp_record_field_latestJournalId_owner_n599) != HXC_STATUS_OK)
                          {
                            abort();
                          }
                          hxc_string hxc_l_tmp_record_field_latestJournalId_owned_load_result_n2156 = hxc_l_tmp_record_field_latestJournalId_owner_n599;
                          hxc_string hxc_l_tmp_record_field_currentObjectiveId_owner_n600 = hxc_l_currentObjectiveId;
                          if (hxc_string_retain(hxc_l_tmp_record_field_currentObjectiveId_owner_n600) != HXC_STATUS_OK)
                          {
                            abort();
                          }
                          hxc_string hxc_l_tmp_record_field_currentObjectiveId_owned_load_result_n2158 = hxc_l_tmp_record_field_currentObjectiveId_owner_n600;
                          enum hxc_caxecraft_app_InteractionPrompt hxc_l_tmp_load_result_n2159 = hxc_l_availableInteractionPrompt;
                          struct hxc_caxecraft_domain_Character hxc_l_tmp_load_result_n2160 = hxc_l_enemyActor;
                          struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation hxc_l_tmp_load_result_n2161 = hxc_l_enemyPhase_h906f9c33685e;
                          struct hxc_caxecraft_gameplay_SwordCombatState hxc_l_tmp_load_result_n2162 = hxc_l_swordCombat;
                          struct hxc_caxecraft_gameplay_BerryDropState hxc_l_tmp_load_result_n2163 = hxc_l_berryDrop;
                          double hxc_l_tmp_load_result_n2164 = hxc_l_cameraWaterBlend;
                          enum hxc_caxecraft_domain_PlayerCameraMode hxc_l_tmp_load_result_n2165 = hxc_l_cameraMode;
                          double hxc_l_tmp_load_result_n2166 = hxc_l_lookX;
                          double hxc_l_tmp_load_result_n2167 = hxc_l_lookY;
                          double hxc_l_tmp_load_result_n2168 = hxc_l_lookZ;
                          double hxc_l_tmp_load_result_n2169 = hxc_l_accumulator;
                          struct hxc_caxecraft_app_MotionHistory hxc_l_tmp_load_result_n2170 = hxc_l_motionHistory;
                          bool hxc_l_tmp_load_result_n2171 = hxc_l_jumpQueued;
                          bool hxc_l_tmp_load_result_n2172 = hxc_l_swordQueued;
                          int32_t hxc_l_tmp_load_result_n2173 = hxc_l_placementBlockedFrames;
                          int32_t hxc_l_tmp_load_result_n2174 = hxc_l_strikeHitFrames;
                          int32_t hxc_l_tmp_load_result_n2175 = hxc_l_enemyDefeatedFrames;
                          int32_t hxc_l_tmp_load_result_n2176 = hxc_l_enemyAttackFrames;
                          int32_t hxc_l_tmp_load_result_n2177 = hxc_l_pickupFrames;
                          int32_t hxc_l_tmp_load_result_n2178 = hxc_l_pickupAmount;
                          int32_t hxc_l_tmp_load_result_n2179 = hxc_l_inventoryFullFrames;
                          int32_t hxc_l_tmp_load_result_n2180 = hxc_l_inventoryFullReason;
                          int32_t hxc_l_tmp_load_result_n2181 = hxc_l_recoveryFeedback;
                          int32_t hxc_l_tmp_load_result_n2182 = hxc_l_recoveryFeedbackFrames;
                          int32_t hxc_l_tmp_load_result_n2183 = hxc_l_aquaticEquipmentCode;
                          int32_t hxc_l_tmp_load_result_n2184 = hxc_l_aquaticEquipmentFrames;
                          struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_load_result_n2185 = hxc_l_campaignLevel;
                          hxc_l_gc_roots[699] = (const void *)hxc_l_tmp_load_result_n2185;
                          struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_load_result_n2186 = hxc_l_pendingCampaignLevel;
                          hxc_l_gc_roots[700] = (const void *)hxc_l_tmp_load_result_n2186;
                          struct hxc_caxecraft_app_PreparedCampaignPortal *hxc_l_tmp_load_result_n2187 = hxc_l_pendingCampaignPortal;
                          hxc_l_gc_roots[701] = (const void *)hxc_l_tmp_load_result_n2187;
                          hxc_string hxc_l_tmp_record_field_pendingCampaignLabel_owner_n601 = hxc_l_pendingCampaignLabel;
                          if (hxc_string_retain(hxc_l_tmp_record_field_pendingCampaignLabel_owner_n601) != HXC_STATUS_OK)
                          {
                            abort();
                          }
                          hxc_string hxc_l_tmp_record_field_pendingCampaignLabel_owned_load_result_n2189 = hxc_l_tmp_record_field_pendingCampaignLabel_owner_n601;
                          bool hxc_l_tmp_load_result_n2190 = hxc_l_loadingFramePresented;
                          hxc_string hxc_l_tmp_record_field_levelLabel_owner_n602 = hxc_l_levelLabel;
                          if (hxc_string_retain(hxc_l_tmp_record_field_levelLabel_owner_n602) != HXC_STATUS_OK)
                          {
                            abort();
                          }
                          hxc_string hxc_l_tmp_record_field_levelLabel_owned_load_result_n2192 = hxc_l_tmp_record_field_levelLabel_owner_n602;
                          int32_t hxc_l_tmp_load_result_n2193 = hxc_l_selectedCampaignLevelIndex;
                          hxc_l_gc_roots[702] = (const void *)hxc_l_preparedPortal;
                          hxc_l_gc_roots[703] = (const void *)(struct hxc_caxecraft_app_CaxecraftApp_EditorTestPlayShellSnapshot){ .hxc_accumulator = hxc_l_tmp_load_result_n2169, .hxc_activeDialogue = hxc_l_tmp_record_field_activeDialogue_owned_load_result_n2154, .hxc_aquaticEquipmentCode = hxc_l_tmp_load_result_n2183, .hxc_aquaticEquipmentFrames = hxc_l_tmp_load_result_n2184, .hxc_availableInteractionPrompt = hxc_l_tmp_load_result_n2159, .hxc_berryDrop = hxc_l_tmp_load_result_n2163, .hxc_cameraMode = hxc_l_tmp_load_result_n2165, .hxc_cameraWaterBlend = hxc_l_tmp_load_result_n2164, .hxc_campaignLevel = hxc_l_tmp_load_result_n2185, .hxc_conversation = hxc_l_tmp_load_result_n2152, .hxc_currentObjectiveId = hxc_l_tmp_record_field_currentObjectiveId_owned_load_result_n2158, .hxc_enemyActor = hxc_l_tmp_load_result_n2160, .hxc_enemyAttackFrames = hxc_l_tmp_load_result_n2176, .hxc_enemyDefeatedFrames = hxc_l_tmp_load_result_n2175, .hxc_enemyPhase = hxc_l_tmp_load_result_n2161, .hxc_inventory = hxc_l_tmp_load_result_n2151, .hxc_inventoryFullFrames = hxc_l_tmp_load_result_n2179, .hxc_inventoryFullReason = hxc_l_tmp_load_result_n2180, .hxc_jumpQueued = hxc_l_tmp_load_result_n2171, .hxc_latestJournalId = hxc_l_tmp_record_field_latestJournalId_owned_load_result_n2156, .hxc_levelLabel = hxc_l_tmp_record_field_levelLabel_owned_load_result_n2192, .hxc_loadingFramePresented = hxc_l_tmp_load_result_n2190, .hxc_lookX = hxc_l_tmp_load_result_n2166, .hxc_lookY = hxc_l_tmp_load_result_n2167, .hxc_lookZ = hxc_l_tmp_load_result_n2168, .hxc_motionHistory = hxc_l_tmp_load_result_n2170, .hxc_pendingCampaignLabel = hxc_l_tmp_record_field_pendingCampaignLabel_owned_load_result_n2189, .hxc_pendingCampaignLevel = hxc_l_tmp_load_result_n2186, .hxc_pendingCampaignPortal = hxc_l_tmp_load_result_n2187, .hxc_pickupAmount = hxc_l_tmp_load_result_n2178, .hxc_pickupFrames = hxc_l_tmp_load_result_n2177, .hxc_placementBlockedFrames = hxc_l_tmp_load_result_n2173, .hxc_preparedPortal = hxc_l_preparedPortal, .hxc_recoveryFeedback = hxc_l_tmp_load_result_n2181, .hxc_recoveryFeedbackFrames = hxc_l_tmp_load_result_n2182, .hxc_selectedCampaignLevelIndex = hxc_l_tmp_load_result_n2193, .hxc_strikeHitFrames = hxc_l_tmp_load_result_n2174, .hxc_swordCombat = hxc_l_tmp_load_result_n2162, .hxc_swordQueued = hxc_l_tmp_load_result_n2172 }.hxc_campaignLevel;
                          hxc_l_gc_roots[704] = (const void *)(struct hxc_caxecraft_app_CaxecraftApp_EditorTestPlayShellSnapshot){ .hxc_accumulator = hxc_l_tmp_load_result_n2169, .hxc_activeDialogue = hxc_l_tmp_record_field_activeDialogue_owned_load_result_n2154, .hxc_aquaticEquipmentCode = hxc_l_tmp_load_result_n2183, .hxc_aquaticEquipmentFrames = hxc_l_tmp_load_result_n2184, .hxc_availableInteractionPrompt = hxc_l_tmp_load_result_n2159, .hxc_berryDrop = hxc_l_tmp_load_result_n2163, .hxc_cameraMode = hxc_l_tmp_load_result_n2165, .hxc_cameraWaterBlend = hxc_l_tmp_load_result_n2164, .hxc_campaignLevel = hxc_l_tmp_load_result_n2185, .hxc_conversation = hxc_l_tmp_load_result_n2152, .hxc_currentObjectiveId = hxc_l_tmp_record_field_currentObjectiveId_owned_load_result_n2158, .hxc_enemyActor = hxc_l_tmp_load_result_n2160, .hxc_enemyAttackFrames = hxc_l_tmp_load_result_n2176, .hxc_enemyDefeatedFrames = hxc_l_tmp_load_result_n2175, .hxc_enemyPhase = hxc_l_tmp_load_result_n2161, .hxc_inventory = hxc_l_tmp_load_result_n2151, .hxc_inventoryFullFrames = hxc_l_tmp_load_result_n2179, .hxc_inventoryFullReason = hxc_l_tmp_load_result_n2180, .hxc_jumpQueued = hxc_l_tmp_load_result_n2171, .hxc_latestJournalId = hxc_l_tmp_record_field_latestJournalId_owned_load_result_n2156, .hxc_levelLabel = hxc_l_tmp_record_field_levelLabel_owned_load_result_n2192, .hxc_loadingFramePresented = hxc_l_tmp_load_result_n2190, .hxc_lookX = hxc_l_tmp_load_result_n2166, .hxc_lookY = hxc_l_tmp_load_result_n2167, .hxc_lookZ = hxc_l_tmp_load_result_n2168, .hxc_motionHistory = hxc_l_tmp_load_result_n2170, .hxc_pendingCampaignLabel = hxc_l_tmp_record_field_pendingCampaignLabel_owned_load_result_n2189, .hxc_pendingCampaignLevel = hxc_l_tmp_load_result_n2186, .hxc_pendingCampaignPortal = hxc_l_tmp_load_result_n2187, .hxc_pickupAmount = hxc_l_tmp_load_result_n2178, .hxc_pickupFrames = hxc_l_tmp_load_result_n2177, .hxc_placementBlockedFrames = hxc_l_tmp_load_result_n2173, .hxc_preparedPortal = hxc_l_preparedPortal, .hxc_recoveryFeedback = hxc_l_tmp_load_result_n2181, .hxc_recoveryFeedbackFrames = hxc_l_tmp_load_result_n2182, .hxc_selectedCampaignLevelIndex = hxc_l_tmp_load_result_n2193, .hxc_strikeHitFrames = hxc_l_tmp_load_result_n2174, .hxc_swordCombat = hxc_l_tmp_load_result_n2162, .hxc_swordQueued = hxc_l_tmp_load_result_n2172 }.hxc_pendingCampaignLevel;
                          hxc_l_gc_roots[705] = (const void *)(struct hxc_caxecraft_app_CaxecraftApp_EditorTestPlayShellSnapshot){ .hxc_accumulator = hxc_l_tmp_load_result_n2169, .hxc_activeDialogue = hxc_l_tmp_record_field_activeDialogue_owned_load_result_n2154, .hxc_aquaticEquipmentCode = hxc_l_tmp_load_result_n2183, .hxc_aquaticEquipmentFrames = hxc_l_tmp_load_result_n2184, .hxc_availableInteractionPrompt = hxc_l_tmp_load_result_n2159, .hxc_berryDrop = hxc_l_tmp_load_result_n2163, .hxc_cameraMode = hxc_l_tmp_load_result_n2165, .hxc_cameraWaterBlend = hxc_l_tmp_load_result_n2164, .hxc_campaignLevel = hxc_l_tmp_load_result_n2185, .hxc_conversation = hxc_l_tmp_load_result_n2152, .hxc_currentObjectiveId = hxc_l_tmp_record_field_currentObjectiveId_owned_load_result_n2158, .hxc_enemyActor = hxc_l_tmp_load_result_n2160, .hxc_enemyAttackFrames = hxc_l_tmp_load_result_n2176, .hxc_enemyDefeatedFrames = hxc_l_tmp_load_result_n2175, .hxc_enemyPhase = hxc_l_tmp_load_result_n2161, .hxc_inventory = hxc_l_tmp_load_result_n2151, .hxc_inventoryFullFrames = hxc_l_tmp_load_result_n2179, .hxc_inventoryFullReason = hxc_l_tmp_load_result_n2180, .hxc_jumpQueued = hxc_l_tmp_load_result_n2171, .hxc_latestJournalId = hxc_l_tmp_record_field_latestJournalId_owned_load_result_n2156, .hxc_levelLabel = hxc_l_tmp_record_field_levelLabel_owned_load_result_n2192, .hxc_loadingFramePresented = hxc_l_tmp_load_result_n2190, .hxc_lookX = hxc_l_tmp_load_result_n2166, .hxc_lookY = hxc_l_tmp_load_result_n2167, .hxc_lookZ = hxc_l_tmp_load_result_n2168, .hxc_motionHistory = hxc_l_tmp_load_result_n2170, .hxc_pendingCampaignLabel = hxc_l_tmp_record_field_pendingCampaignLabel_owned_load_result_n2189, .hxc_pendingCampaignLevel = hxc_l_tmp_load_result_n2186, .hxc_pendingCampaignPortal = hxc_l_tmp_load_result_n2187, .hxc_pickupAmount = hxc_l_tmp_load_result_n2178, .hxc_pickupFrames = hxc_l_tmp_load_result_n2177, .hxc_placementBlockedFrames = hxc_l_tmp_load_result_n2173, .hxc_preparedPortal = hxc_l_preparedPortal, .hxc_recoveryFeedback = hxc_l_tmp_load_result_n2181, .hxc_recoveryFeedbackFrames = hxc_l_tmp_load_result_n2182, .hxc_selectedCampaignLevelIndex = hxc_l_tmp_load_result_n2193, .hxc_strikeHitFrames = hxc_l_tmp_load_result_n2174, .hxc_swordCombat = hxc_l_tmp_load_result_n2162, .hxc_swordQueued = hxc_l_tmp_load_result_n2172 }.hxc_pendingCampaignPortal;
                          hxc_l_gc_roots[706] = (const void *)(struct hxc_caxecraft_app_CaxecraftApp_EditorTestPlayShellSnapshot){ .hxc_accumulator = hxc_l_tmp_load_result_n2169, .hxc_activeDialogue = hxc_l_tmp_record_field_activeDialogue_owned_load_result_n2154, .hxc_aquaticEquipmentCode = hxc_l_tmp_load_result_n2183, .hxc_aquaticEquipmentFrames = hxc_l_tmp_load_result_n2184, .hxc_availableInteractionPrompt = hxc_l_tmp_load_result_n2159, .hxc_berryDrop = hxc_l_tmp_load_result_n2163, .hxc_cameraMode = hxc_l_tmp_load_result_n2165, .hxc_cameraWaterBlend = hxc_l_tmp_load_result_n2164, .hxc_campaignLevel = hxc_l_tmp_load_result_n2185, .hxc_conversation = hxc_l_tmp_load_result_n2152, .hxc_currentObjectiveId = hxc_l_tmp_record_field_currentObjectiveId_owned_load_result_n2158, .hxc_enemyActor = hxc_l_tmp_load_result_n2160, .hxc_enemyAttackFrames = hxc_l_tmp_load_result_n2176, .hxc_enemyDefeatedFrames = hxc_l_tmp_load_result_n2175, .hxc_enemyPhase = hxc_l_tmp_load_result_n2161, .hxc_inventory = hxc_l_tmp_load_result_n2151, .hxc_inventoryFullFrames = hxc_l_tmp_load_result_n2179, .hxc_inventoryFullReason = hxc_l_tmp_load_result_n2180, .hxc_jumpQueued = hxc_l_tmp_load_result_n2171, .hxc_latestJournalId = hxc_l_tmp_record_field_latestJournalId_owned_load_result_n2156, .hxc_levelLabel = hxc_l_tmp_record_field_levelLabel_owned_load_result_n2192, .hxc_loadingFramePresented = hxc_l_tmp_load_result_n2190, .hxc_lookX = hxc_l_tmp_load_result_n2166, .hxc_lookY = hxc_l_tmp_load_result_n2167, .hxc_lookZ = hxc_l_tmp_load_result_n2168, .hxc_motionHistory = hxc_l_tmp_load_result_n2170, .hxc_pendingCampaignLabel = hxc_l_tmp_record_field_pendingCampaignLabel_owned_load_result_n2189, .hxc_pendingCampaignLevel = hxc_l_tmp_load_result_n2186, .hxc_pendingCampaignPortal = hxc_l_tmp_load_result_n2187, .hxc_pickupAmount = hxc_l_tmp_load_result_n2178, .hxc_pickupFrames = hxc_l_tmp_load_result_n2177, .hxc_placementBlockedFrames = hxc_l_tmp_load_result_n2173, .hxc_preparedPortal = hxc_l_preparedPortal, .hxc_recoveryFeedback = hxc_l_tmp_load_result_n2181, .hxc_recoveryFeedbackFrames = hxc_l_tmp_load_result_n2182, .hxc_selectedCampaignLevelIndex = hxc_l_tmp_load_result_n2193, .hxc_strikeHitFrames = hxc_l_tmp_load_result_n2174, .hxc_swordCombat = hxc_l_tmp_load_result_n2162, .hxc_swordQueued = hxc_l_tmp_load_result_n2172 }.hxc_preparedPortal;
                          hxc_l_snapshot = (struct hxc_caxecraft_app_CaxecraftApp_EditorTestPlayShellSnapshot){ .hxc_accumulator = hxc_l_tmp_load_result_n2169, .hxc_activeDialogue = hxc_l_tmp_record_field_activeDialogue_owned_load_result_n2154, .hxc_aquaticEquipmentCode = hxc_l_tmp_load_result_n2183, .hxc_aquaticEquipmentFrames = hxc_l_tmp_load_result_n2184, .hxc_availableInteractionPrompt = hxc_l_tmp_load_result_n2159, .hxc_berryDrop = hxc_l_tmp_load_result_n2163, .hxc_cameraMode = hxc_l_tmp_load_result_n2165, .hxc_cameraWaterBlend = hxc_l_tmp_load_result_n2164, .hxc_campaignLevel = hxc_l_tmp_load_result_n2185, .hxc_conversation = hxc_l_tmp_load_result_n2152, .hxc_currentObjectiveId = hxc_l_tmp_record_field_currentObjectiveId_owned_load_result_n2158, .hxc_enemyActor = hxc_l_tmp_load_result_n2160, .hxc_enemyAttackFrames = hxc_l_tmp_load_result_n2176, .hxc_enemyDefeatedFrames = hxc_l_tmp_load_result_n2175, .hxc_enemyPhase = hxc_l_tmp_load_result_n2161, .hxc_inventory = hxc_l_tmp_load_result_n2151, .hxc_inventoryFullFrames = hxc_l_tmp_load_result_n2179, .hxc_inventoryFullReason = hxc_l_tmp_load_result_n2180, .hxc_jumpQueued = hxc_l_tmp_load_result_n2171, .hxc_latestJournalId = hxc_l_tmp_record_field_latestJournalId_owned_load_result_n2156, .hxc_levelLabel = hxc_l_tmp_record_field_levelLabel_owned_load_result_n2192, .hxc_loadingFramePresented = hxc_l_tmp_load_result_n2190, .hxc_lookX = hxc_l_tmp_load_result_n2166, .hxc_lookY = hxc_l_tmp_load_result_n2167, .hxc_lookZ = hxc_l_tmp_load_result_n2168, .hxc_motionHistory = hxc_l_tmp_load_result_n2170, .hxc_pendingCampaignLabel = hxc_l_tmp_record_field_pendingCampaignLabel_owned_load_result_n2189, .hxc_pendingCampaignLevel = hxc_l_tmp_load_result_n2186, .hxc_pendingCampaignPortal = hxc_l_tmp_load_result_n2187, .hxc_pickupAmount = hxc_l_tmp_load_result_n2178, .hxc_pickupFrames = hxc_l_tmp_load_result_n2177, .hxc_placementBlockedFrames = hxc_l_tmp_load_result_n2173, .hxc_preparedPortal = hxc_l_preparedPortal, .hxc_recoveryFeedback = hxc_l_tmp_load_result_n2181, .hxc_recoveryFeedbackFrames = hxc_l_tmp_load_result_n2182, .hxc_selectedCampaignLevelIndex = hxc_l_tmp_load_result_n2193, .hxc_strikeHitFrames = hxc_l_tmp_load_result_n2174, .hxc_swordCombat = hxc_l_tmp_load_result_n2162, .hxc_swordQueued = hxc_l_tmp_load_result_n2172 };
                          struct hxc_caxecraft_app_EditorTestPlayRuntime *hxc_l_tmp_load_result_n2196 = hxc_l_proposedRuntime;
                          hxc_l_gc_roots[707] = (const void *)hxc_l_tmp_load_result_n2196;
                          hxc_l_gc_roots[708] = (const void *)hxc_l_snapshot.hxc_campaignLevel;
                          hxc_l_gc_roots[709] = (const void *)hxc_l_snapshot.hxc_pendingCampaignLevel;
                          hxc_l_gc_roots[710] = (const void *)hxc_l_snapshot.hxc_pendingCampaignPortal;
                          hxc_l_gc_roots[711] = (const void *)hxc_l_snapshot.hxc_preparedPortal;
                          struct hxc_caxecraft_app_CaxecraftApp_EditorTestPlayShellSnapshot hxc_l_tmp_record_field_snapshot_owner_n603 = hxc_l_snapshot;
                          if (hxc_record_3b3e16aa_retain(&hxc_l_tmp_record_field_snapshot_owner_n603) != HXC_STATUS_OK)
                          {
                            abort();
                          }
                          struct hxc_caxecraft_app_CaxecraftApp_EditorTestPlayShellSnapshot hxc_l_tmp_record_field_snapshot_owned_load_result_n2198 = hxc_l_tmp_record_field_snapshot_owner_n603;
                          hxc_l_gc_roots[712] = (const void *)hxc_l_tmp_record_field_snapshot_owned_load_result_n2198.hxc_campaignLevel;
                          hxc_l_gc_roots[713] = (const void *)hxc_l_tmp_record_field_snapshot_owned_load_result_n2198.hxc_pendingCampaignLevel;
                          hxc_l_gc_roots[714] = (const void *)hxc_l_tmp_record_field_snapshot_owned_load_result_n2198.hxc_pendingCampaignPortal;
                          hxc_l_gc_roots[715] = (const void *)hxc_l_tmp_record_field_snapshot_owned_load_result_n2198.hxc_preparedPortal;
                          struct hxc_caxecraft_app_ActivePlayableLevel *hxc_l_tmp_load_result_n2199 = hxc_l_testOwner;
                          hxc_l_gc_roots[716] = (const void *)hxc_l_tmp_load_result_n2199;
                          if (hxc_l_tmp_load_result_n2199 == NULL)
                          {
                            abort();
                          }
                          struct hxc_caxecraft_content_ActiveContent *hxc_l_tmp_class_field_load_result_n2200 = (*hxc_l_tmp_load_result_n2199).hxc_content;
                          hxc_l_gc_roots[717] = (const void *)hxc_l_tmp_class_field_load_result_n2200;
                          if (hxc_l_tmp_class_field_load_result_n2200 == NULL)
                          {
                            abort();
                          }
                          struct hxc_caxecraft_content_LoadedContentGeneration *hxc_l_tmp_class_field_load_result_n2201 = (*hxc_l_tmp_class_field_load_result_n2200).hxc_active;
                          hxc_l_gc_roots[718] = (const void *)hxc_l_tmp_class_field_load_result_n2201;
                          if (hxc_l_tmp_class_field_load_result_n2201 == NULL)
                          {
                            abort();
                          }
                          int32_t hxc_l_tmp_class_field_load_result_n2202 = (*hxc_l_tmp_class_field_load_result_n2201).hxc_generationIdValue;
                          hxc_l_gc_roots[719] = (const void *)(struct hxc_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_generationId = hxc_l_tmp_class_field_load_result_n2202, .hxc_runtime = hxc_l_tmp_load_result_n2196, .hxc_snapshot = hxc_l_tmp_record_field_snapshot_owned_load_result_n2198 }.hxc_runtime;
                          hxc_l_gc_roots[720] = (const void *)(struct hxc_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_generationId = hxc_l_tmp_class_field_load_result_n2202, .hxc_runtime = hxc_l_tmp_load_result_n2196, .hxc_snapshot = hxc_l_tmp_record_field_snapshot_owned_load_result_n2198 }.hxc_snapshot.hxc_campaignLevel;
                          hxc_l_gc_roots[721] = (const void *)(struct hxc_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_generationId = hxc_l_tmp_class_field_load_result_n2202, .hxc_runtime = hxc_l_tmp_load_result_n2196, .hxc_snapshot = hxc_l_tmp_record_field_snapshot_owned_load_result_n2198 }.hxc_snapshot.hxc_pendingCampaignLevel;
                          hxc_l_gc_roots[722] = (const void *)(struct hxc_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_generationId = hxc_l_tmp_class_field_load_result_n2202, .hxc_runtime = hxc_l_tmp_load_result_n2196, .hxc_snapshot = hxc_l_tmp_record_field_snapshot_owned_load_result_n2198 }.hxc_snapshot.hxc_pendingCampaignPortal;
                          hxc_l_gc_roots[723] = (const void *)(struct hxc_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_generationId = hxc_l_tmp_class_field_load_result_n2202, .hxc_runtime = hxc_l_tmp_load_result_n2196, .hxc_snapshot = hxc_l_tmp_record_field_snapshot_owned_load_result_n2198 }.hxc_snapshot.hxc_preparedPortal;
                          hxc_l_gc_roots[724] = (struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = true, .hxc_value = (struct hxc_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_generationId = hxc_l_tmp_class_field_load_result_n2202, .hxc_runtime = hxc_l_tmp_load_result_n2196, .hxc_snapshot = hxc_l_tmp_record_field_snapshot_owned_load_result_n2198 } }.hxc_has_value ? (const void *)(struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = true, .hxc_value = (struct hxc_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_generationId = hxc_l_tmp_class_field_load_result_n2202, .hxc_runtime = hxc_l_tmp_load_result_n2196, .hxc_snapshot = hxc_l_tmp_record_field_snapshot_owned_load_result_n2198 } }.hxc_value.hxc_runtime : NULL;
                          hxc_l_gc_roots[725] = (struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = true, .hxc_value = (struct hxc_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_generationId = hxc_l_tmp_class_field_load_result_n2202, .hxc_runtime = hxc_l_tmp_load_result_n2196, .hxc_snapshot = hxc_l_tmp_record_field_snapshot_owned_load_result_n2198 } }.hxc_has_value ? (const void *)(struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = true, .hxc_value = (struct hxc_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_generationId = hxc_l_tmp_class_field_load_result_n2202, .hxc_runtime = hxc_l_tmp_load_result_n2196, .hxc_snapshot = hxc_l_tmp_record_field_snapshot_owned_load_result_n2198 } }.hxc_value.hxc_snapshot.hxc_campaignLevel : NULL;
                          hxc_l_gc_roots[726] = (struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = true, .hxc_value = (struct hxc_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_generationId = hxc_l_tmp_class_field_load_result_n2202, .hxc_runtime = hxc_l_tmp_load_result_n2196, .hxc_snapshot = hxc_l_tmp_record_field_snapshot_owned_load_result_n2198 } }.hxc_has_value ? (const void *)(struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = true, .hxc_value = (struct hxc_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_generationId = hxc_l_tmp_class_field_load_result_n2202, .hxc_runtime = hxc_l_tmp_load_result_n2196, .hxc_snapshot = hxc_l_tmp_record_field_snapshot_owned_load_result_n2198 } }.hxc_value.hxc_snapshot.hxc_pendingCampaignLevel : NULL;
                          hxc_l_gc_roots[727] = (struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = true, .hxc_value = (struct hxc_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_generationId = hxc_l_tmp_class_field_load_result_n2202, .hxc_runtime = hxc_l_tmp_load_result_n2196, .hxc_snapshot = hxc_l_tmp_record_field_snapshot_owned_load_result_n2198 } }.hxc_has_value ? (const void *)(struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = true, .hxc_value = (struct hxc_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_generationId = hxc_l_tmp_class_field_load_result_n2202, .hxc_runtime = hxc_l_tmp_load_result_n2196, .hxc_snapshot = hxc_l_tmp_record_field_snapshot_owned_load_result_n2198 } }.hxc_value.hxc_snapshot.hxc_pendingCampaignPortal : NULL;
                          hxc_l_gc_roots[728] = (struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = true, .hxc_value = (struct hxc_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_generationId = hxc_l_tmp_class_field_load_result_n2202, .hxc_runtime = hxc_l_tmp_load_result_n2196, .hxc_snapshot = hxc_l_tmp_record_field_snapshot_owned_load_result_n2198 } }.hxc_has_value ? (const void *)(struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = true, .hxc_value = (struct hxc_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_generationId = hxc_l_tmp_class_field_load_result_n2202, .hxc_runtime = hxc_l_tmp_load_result_n2196, .hxc_snapshot = hxc_l_tmp_record_field_snapshot_owned_load_result_n2198 } }.hxc_value.hxc_snapshot.hxc_preparedPortal : NULL;
                          hxc_optional_9d9d283f_destroy(&hxc_l_activeEditorTestPlay);
                          hxc_l_activeEditorTestPlay = (struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = true, .hxc_value = (struct hxc_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_generationId = hxc_l_tmp_class_field_load_result_n2202, .hxc_runtime = hxc_l_tmp_load_result_n2196, .hxc_snapshot = hxc_l_tmp_record_field_snapshot_owned_load_result_n2198 } };
                          struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n2205 = hxc_caxecraft_gameplay_Inventory_starter();
                          hxc_l_inventory = hxc_l_tmp_call_result_n2205;
                          hxc_l_conversation = (struct hxc_optional_caxecraft_app_ConversationState){ .hxc_has_value = false };
                          hxc_string hxc_l_tmp_string_assignment_replacement_owner_n604 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
                          if (hxc_string_retain(hxc_l_tmp_string_assignment_replacement_owner_n604) != HXC_STATUS_OK)
                          {
                            abort();
                          }
                          hxc_string hxc_l_tmp_string_assignment_replacement_owned_load_result_n2206 = hxc_l_tmp_string_assignment_replacement_owner_n604;
                          if (hxc_string_release(&hxc_l_activeDialogue) != HXC_STATUS_OK)
                          {
                            abort();
                          }
                          hxc_l_activeDialogue = hxc_l_tmp_string_assignment_replacement_owned_load_result_n2206;
                          hxc_string hxc_l_tmp_string_assignment_replacement_owner_n605 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
                          if (hxc_string_retain(hxc_l_tmp_string_assignment_replacement_owner_n605) != HXC_STATUS_OK)
                          {
                            abort();
                          }
                          hxc_string hxc_l_tmp_string_assignment_replacement_owned_load_result_n2207 = hxc_l_tmp_string_assignment_replacement_owner_n605;
                          if (hxc_string_release(&hxc_l_latestJournalId) != HXC_STATUS_OK)
                          {
                            abort();
                          }
                          hxc_l_latestJournalId = hxc_l_tmp_string_assignment_replacement_owned_load_result_n2207;
                          struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n2208 = hxc_l_testLevel_hf7ba2c1e808b;
                          hxc_l_gc_roots[729] = (const void *)hxc_l_tmp_load_result_n2208;
                          if (hxc_l_tmp_load_result_n2208 == NULL)
                          {
                            abort();
                          }
                          struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_class_field_load_result_n2209 = (*hxc_l_tmp_load_result_n2208).hxc_levelPresentation;
                          hxc_l_gc_roots[730] = (const void *)hxc_l_tmp_class_field_load_result_n2209;
                          if (hxc_l_tmp_class_field_load_result_n2209 == NULL)
                          {
                            abort();
                          }
                          hxc_string hxc_l_tmp_class_field_load_result_n2210 = (*hxc_l_tmp_class_field_load_result_n2209).hxc_startingObjective;
                          hxc_string hxc_l_tmp_string_assignment_replacement_owner_n606 = hxc_l_tmp_class_field_load_result_n2210;
                          if (hxc_string_retain(hxc_l_tmp_string_assignment_replacement_owner_n606) != HXC_STATUS_OK)
                          {
                            abort();
                          }
                          hxc_string hxc_l_tmp_string_assignment_replacement_owned_load_result_n2211 = hxc_l_tmp_string_assignment_replacement_owner_n606;
                          if (hxc_string_release(&hxc_l_currentObjectiveId) != HXC_STATUS_OK)
                          {
                            abort();
                          }
                          hxc_l_currentObjectiveId = hxc_l_tmp_string_assignment_replacement_owned_load_result_n2211;
                          struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n2212 = hxc_l_testSession;
                          hxc_l_gc_roots[731] = (const void *)hxc_l_tmp_load_result_n2212;
                          hxc_l_gc_roots[732] = (const void *)hxc_l_testLevel_hf7ba2c1e808b;
                          struct hxc_caxecraft_app_CaxecraftApp_AvailableInteractionTarget hxc_l_tmp_call_result_n2214 = hxc_caxecraft_app_CaxecraftApp_nearestAvailableInteraction(hxc_l_tmp_load_result_n2212, hxc_l_testLevel_hf7ba2c1e808b);
                          hxc_l_tmp_static_call_argument_0_owner_n607 = hxc_l_tmp_call_result_n2214;
                          enum hxc_caxecraft_app_InteractionPrompt hxc_l_tmp_call_result_n2216 = hxc_caxecraft_app_CaxecraftApp_promptForAvailableInteraction(hxc_l_tmp_static_call_argument_0_owner_n607);
                          hxc_l_availableInteractionPrompt = hxc_l_tmp_call_result_n2216;
                          hxc_l_enemyActor = hxc_l_testEnemy;
                          hxc_l_enemyPhase_h906f9c33685e = hxc_l_testEnemyPhase;
                          hxc_l_swordCombat = (struct hxc_caxecraft_gameplay_SwordCombatState){ .hxc_cooldownTicks = 0 };
                          hxc_l_berryDrop = (struct hxc_caxecraft_gameplay_BerryDropState){ .hxc_active = false, .hxc_amount = 0, .hxc_x = 0.0, .hxc_y = 0.0, .hxc_z = 0.0 };
                          hxc_l_cameraWaterBlend = 0.0;
                          hxc_l_cameraMode = hxc_caxecraft_domain_PlayerCameraMode_FirstPerson;
                          struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n2222 = hxc_l_testLevel_hf7ba2c1e808b;
                          hxc_l_gc_roots[733] = (const void *)hxc_l_tmp_load_result_n2222;
                          if (hxc_l_tmp_load_result_n2222 == NULL)
                          {
                            abort();
                          }
                          struct hxc_caxecraft_scenario_ScenarioTransform hxc_l_tmp_class_field_load_result_n2223 = (*hxc_l_tmp_load_result_n2222).hxc_playerSpawn;
                          struct hxc_caxecraft_app_RenderPosition hxc_l_tmp_call_result_n2224 = hxc_caxecraft_app_SpawnCameraHeading_headingForSpawn(hxc_l_tmp_class_field_load_result_n2223);
                          struct hxc_caxecraft_app_RenderPosition hxc_l_testHeading = hxc_l_tmp_call_result_n2224;
                          hxc_l_lookX = hxc_l_testHeading.hxc_x;
                          hxc_l_lookY = hxc_l_testHeading.hxc_y;
                          hxc_l_lookZ = hxc_l_testHeading.hxc_z;
                          hxc_l_accumulator = 0.0;
                          struct hxc_caxecraft_app_MotionHistory hxc_l_tmp_call_result_n2229 = hxc_caxecraft_app_MotionInterpolation_start(hxc_l_testView.hxc_localPlayer.hxc_body);
                          hxc_l_motionHistory = hxc_l_tmp_call_result_n2229;
                          hxc_l_jumpQueued = false;
                          hxc_l_swordQueued = false;
                          hxc_l_placementBlockedFrames = 0;
                          hxc_l_strikeHitFrames = 0;
                          hxc_l_enemyDefeatedFrames = 0;
                          hxc_l_enemyAttackFrames = 0;
                          hxc_l_pickupFrames = 0;
                          hxc_l_pickupAmount = 0;
                          hxc_l_inventoryFullFrames = 0;
                          hxc_l_inventoryFullReason = 0;
                          hxc_l_recoveryFeedback = 0;
                          hxc_l_recoveryFeedbackFrames = 0;
                          hxc_l_aquaticEquipmentCode = -1;
                          hxc_l_aquaticEquipmentFrames = 0;
                          if (hxc_l_self == NULL)
                          {
                            abort();
                          }
                          struct hxc_caxecraft_app_TerrainRenderer *hxc_l_tmp_owned_class_field_address_n2230 = &(*hxc_l_self).hxc_terrainRenderer;
                          struct hxc_caxecraft_app_TerrainChunkCache *hxc_l_tmp_owned_class_field_address_n2231 = &(*hxc_l_tmp_owned_class_field_address_n2230).hxc_cache;
                          if (hxc_l_tmp_owned_class_field_address_n2231 == NULL)
                          {
                            abort();
                          }
                          hxc_caxecraft_app_TerrainChunkCache_invalidateAll(hxc_l_tmp_owned_class_field_address_n2231);
                          struct hxc_caxecraft_input_NavigationRepeater *hxc_l_tmp_owned_class_field_address_n2232 = &(*hxc_l_self).hxc_editorNavigation;
                          if (hxc_l_tmp_owned_class_field_address_n2232 == NULL)
                          {
                            abort();
                          }
                          hxc_caxecraft_input_NavigationRepeater_release(hxc_l_tmp_owned_class_field_address_n2232);
                          enum hxc_caxecraft_app_AppScreen hxc_l_tmp_call_result_n2234 = hxc_caxecraft_app_AppScreen_beginEditorTestPlay(hxc_l_screen);
                          hxc_l_screen = hxc_l_tmp_call_result_n2234;
                          DisableCursor();
                          hxc_enum_2c0e695d_destroy(&hxc_l_tmp_static_call_argument_0_owner_n607);
                          hxc_record_3b3e16aa_destroy(&hxc_l_snapshot);
                        }
                      }
                      if (hxc_array_ref_release(hxc_l_testPhases) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                    }
                    break;
                  }
                case hxc_caxecraft_app_EditorTestPlayStartResult_EditorTestPlayRejected:
                  {
                    struct hxc_caxecraft_app_EditorTestPlayRuntime *hxc_l_tmp_load_result_n2235 = hxc_l_proposedRuntime;
                    hxc_l_gc_roots[678] = (const void *)hxc_l_tmp_load_result_n2235;
                    if (hxc_l_tmp_load_result_n2235 == NULL)
                    {
                      abort();
                    }
                    hxc_caxecraft_app_EditorTestPlayRuntime_stop(hxc_l_tmp_load_result_n2235);
                    struct hxc_caxecraft_app_CaxecraftEditorScreen *hxc_l_tmp_load_result_n2236 = hxc_l_editorScreen;
                    hxc_l_gc_roots[679] = (const void *)hxc_l_tmp_load_result_n2236;
                    if (hxc_l_tmp_load_result_n2236 == NULL)
                    {
                      abort();
                    }
                    hxc_caxecraft_app_CaxecraftEditorScreen_finishTestPlay(hxc_l_tmp_load_result_n2236, false);
                    break;
                  }
                default:
                  {
                    abort();
                  }
              }
              hxc_enum_447e0e6e_destroy(&hxc_l_tmp_enum_switch_subject_owner_n586);
              hxc_record_79135587_destroy(&hxc_l_tmp_instance_call_argument_0_owner_n585);
              break;
            }
          default:
            {
              abort();
            }
        }
        if (hxc_l_onLoading)
        {
          hxc_l_loadingFramePresented = true;
        }
        hxc_l_frameCount = hxc_i32_add_wrapping(hxc_l_frameCount, 1);
        if (hxc_array_ref_release(hxc_l_dialogueActors) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_array_ref_release(hxc_l_presentationActorPhases) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_enum_2c0e695d_destroy(&hxc_l_tmp_static_call_argument_0_owner_n413);
        hxc_enum_59188943_destroy(&hxc_l_editorActionForFrame);
        continue;
      }
      hxc_l_gc_roots[270] = hxc_l_activeEditorTestPlay.hxc_has_value ? (const void *)hxc_l_activeEditorTestPlay.hxc_value.hxc_runtime : NULL;
      hxc_l_gc_roots[271] = hxc_l_activeEditorTestPlay.hxc_has_value ? (const void *)hxc_l_activeEditorTestPlay.hxc_value.hxc_snapshot.hxc_campaignLevel : NULL;
      hxc_l_gc_roots[272] = hxc_l_activeEditorTestPlay.hxc_has_value ? (const void *)hxc_l_activeEditorTestPlay.hxc_value.hxc_snapshot.hxc_pendingCampaignLevel : NULL;
      hxc_l_gc_roots[273] = hxc_l_activeEditorTestPlay.hxc_has_value ? (const void *)hxc_l_activeEditorTestPlay.hxc_value.hxc_snapshot.hxc_pendingCampaignPortal : NULL;
      hxc_l_gc_roots[274] = hxc_l_activeEditorTestPlay.hxc_has_value ? (const void *)hxc_l_activeEditorTestPlay.hxc_value.hxc_snapshot.hxc_preparedPortal : NULL;
      hxc_l_run_hab9bcd6c1cb0 = hxc_l_activeEditorTestPlay;
      if (hxc_optional_9d9d283f_retain(&hxc_l_run_hab9bcd6c1cb0) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_l_gc_roots[275] = hxc_l_run_hab9bcd6c1cb0.hxc_has_value ? (const void *)hxc_l_run_hab9bcd6c1cb0.hxc_value.hxc_runtime : NULL;
      hxc_l_gc_roots[276] = hxc_l_run_hab9bcd6c1cb0.hxc_has_value ? (const void *)hxc_l_run_hab9bcd6c1cb0.hxc_value.hxc_snapshot.hxc_campaignLevel : NULL;
      hxc_l_gc_roots[277] = hxc_l_run_hab9bcd6c1cb0.hxc_has_value ? (const void *)hxc_l_run_hab9bcd6c1cb0.hxc_value.hxc_snapshot.hxc_pendingCampaignLevel : NULL;
      hxc_l_gc_roots[278] = hxc_l_run_hab9bcd6c1cb0.hxc_has_value ? (const void *)hxc_l_run_hab9bcd6c1cb0.hxc_value.hxc_snapshot.hxc_pendingCampaignPortal : NULL;
      hxc_l_gc_roots[279] = hxc_l_run_hab9bcd6c1cb0.hxc_has_value ? (const void *)hxc_l_run_hab9bcd6c1cb0.hxc_value.hxc_snapshot.hxc_preparedPortal : NULL;
      if (!hxc_l_run_hab9bcd6c1cb0.hxc_has_value)
      {
        if (hxc_io_println((hxc_string){ (const uint8_t *)"caxecraft: editor Test Play stop had no active run", 50, true, NULL }) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_l_quit = true;
      }
      else
      {
        struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun hxc_l_tmp_load_result_n381 = hxc_l_run_hab9bcd6c1cb0;
        hxc_l_gc_roots[282] = hxc_l_tmp_load_result_n381.hxc_has_value ? (const void *)hxc_l_tmp_load_result_n381.hxc_value.hxc_runtime : NULL;
        hxc_l_gc_roots[283] = hxc_l_tmp_load_result_n381.hxc_has_value ? (const void *)hxc_l_tmp_load_result_n381.hxc_value.hxc_snapshot.hxc_campaignLevel : NULL;
        hxc_l_gc_roots[284] = hxc_l_tmp_load_result_n381.hxc_has_value ? (const void *)hxc_l_tmp_load_result_n381.hxc_value.hxc_snapshot.hxc_pendingCampaignLevel : NULL;
        hxc_l_gc_roots[285] = hxc_l_tmp_load_result_n381.hxc_has_value ? (const void *)hxc_l_tmp_load_result_n381.hxc_value.hxc_snapshot.hxc_pendingCampaignPortal : NULL;
        hxc_l_gc_roots[286] = hxc_l_tmp_load_result_n381.hxc_has_value ? (const void *)hxc_l_tmp_load_result_n381.hxc_value.hxc_snapshot.hxc_preparedPortal : NULL;
        if (!hxc_l_tmp_load_result_n381.hxc_has_value)
        {
          abort();
        }
        hxc_l_gc_roots[287] = (const void *)hxc_l_tmp_load_result_n381.hxc_value.hxc_runtime;
        hxc_l_gc_roots[288] = (const void *)hxc_l_tmp_load_result_n381.hxc_value.hxc_snapshot.hxc_campaignLevel;
        hxc_l_gc_roots[289] = (const void *)hxc_l_tmp_load_result_n381.hxc_value.hxc_snapshot.hxc_pendingCampaignLevel;
        hxc_l_gc_roots[290] = (const void *)hxc_l_tmp_load_result_n381.hxc_value.hxc_snapshot.hxc_pendingCampaignPortal;
        hxc_l_gc_roots[291] = (const void *)hxc_l_tmp_load_result_n381.hxc_value.hxc_snapshot.hxc_preparedPortal;
        struct hxc_caxecraft_app_EditorTestPlayRuntime *hxc_l_tmp_optional_record_field_project_n383 = hxc_l_tmp_load_result_n381.hxc_value.hxc_runtime;
        hxc_l_gc_roots[292] = (const void *)hxc_l_tmp_optional_record_field_project_n383;
        if (hxc_l_tmp_optional_record_field_project_n383 == NULL)
        {
          abort();
        }
        hxc_caxecraft_app_EditorTestPlayRuntime_stop(hxc_l_tmp_optional_record_field_project_n383);
        hxc_l_gc_roots[293] = (struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = false }.hxc_has_value ? (const void *)(struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = false }.hxc_value.hxc_runtime : NULL;
        hxc_l_gc_roots[294] = (struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = false }.hxc_has_value ? (const void *)(struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = false }.hxc_value.hxc_snapshot.hxc_campaignLevel : NULL;
        hxc_l_gc_roots[295] = (struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = false }.hxc_has_value ? (const void *)(struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = false }.hxc_value.hxc_snapshot.hxc_pendingCampaignLevel : NULL;
        hxc_l_gc_roots[296] = (struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = false }.hxc_has_value ? (const void *)(struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = false }.hxc_value.hxc_snapshot.hxc_pendingCampaignPortal : NULL;
        hxc_l_gc_roots[297] = (struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = false }.hxc_has_value ? (const void *)(struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = false }.hxc_value.hxc_snapshot.hxc_preparedPortal : NULL;
        hxc_optional_9d9d283f_destroy(&hxc_l_activeEditorTestPlay);
        hxc_l_activeEditorTestPlay = (struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = false };
        struct hxc_caxecraft_app_CaxecraftEditorScreen *hxc_l_tmp_load_result_n384 = hxc_l_editorScreen;
        hxc_l_gc_roots[298] = (const void *)hxc_l_tmp_load_result_n384;
        if (hxc_l_tmp_load_result_n384 == NULL)
        {
          abort();
        }
        hxc_caxecraft_app_CaxecraftEditorScreen_finishTestPlay(hxc_l_tmp_load_result_n384, true);
        enum hxc_caxecraft_app_AppScreen hxc_l_tmp_call_result_n386 = hxc_caxecraft_app_AppScreen_finishEditorTestPlay(hxc_l_screen);
        hxc_l_screen = hxc_l_tmp_call_result_n386;
        struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun hxc_l_tmp_load_result_n387 = hxc_l_run_hab9bcd6c1cb0;
        hxc_l_gc_roots[299] = hxc_l_tmp_load_result_n387.hxc_has_value ? (const void *)hxc_l_tmp_load_result_n387.hxc_value.hxc_runtime : NULL;
        hxc_l_gc_roots[300] = hxc_l_tmp_load_result_n387.hxc_has_value ? (const void *)hxc_l_tmp_load_result_n387.hxc_value.hxc_snapshot.hxc_campaignLevel : NULL;
        hxc_l_gc_roots[301] = hxc_l_tmp_load_result_n387.hxc_has_value ? (const void *)hxc_l_tmp_load_result_n387.hxc_value.hxc_snapshot.hxc_pendingCampaignLevel : NULL;
        hxc_l_gc_roots[302] = hxc_l_tmp_load_result_n387.hxc_has_value ? (const void *)hxc_l_tmp_load_result_n387.hxc_value.hxc_snapshot.hxc_pendingCampaignPortal : NULL;
        hxc_l_gc_roots[303] = hxc_l_tmp_load_result_n387.hxc_has_value ? (const void *)hxc_l_tmp_load_result_n387.hxc_value.hxc_snapshot.hxc_preparedPortal : NULL;
        if (!hxc_l_tmp_load_result_n387.hxc_has_value)
        {
          abort();
        }
        hxc_l_gc_roots[304] = (const void *)hxc_l_tmp_load_result_n387.hxc_value.hxc_runtime;
        hxc_l_gc_roots[305] = (const void *)hxc_l_tmp_load_result_n387.hxc_value.hxc_snapshot.hxc_campaignLevel;
        hxc_l_gc_roots[306] = (const void *)hxc_l_tmp_load_result_n387.hxc_value.hxc_snapshot.hxc_pendingCampaignLevel;
        hxc_l_gc_roots[307] = (const void *)hxc_l_tmp_load_result_n387.hxc_value.hxc_snapshot.hxc_pendingCampaignPortal;
        hxc_l_gc_roots[308] = (const void *)hxc_l_tmp_load_result_n387.hxc_value.hxc_snapshot.hxc_preparedPortal;
        hxc_l_gc_roots[309] = (const void *)hxc_l_tmp_load_result_n387.hxc_value.hxc_snapshot.hxc_campaignLevel;
        hxc_l_gc_roots[310] = (const void *)hxc_l_tmp_load_result_n387.hxc_value.hxc_snapshot.hxc_pendingCampaignLevel;
        hxc_l_gc_roots[311] = (const void *)hxc_l_tmp_load_result_n387.hxc_value.hxc_snapshot.hxc_pendingCampaignPortal;
        hxc_l_gc_roots[312] = (const void *)hxc_l_tmp_load_result_n387.hxc_value.hxc_snapshot.hxc_preparedPortal;
        hxc_l_saved = hxc_l_tmp_load_result_n387.hxc_value.hxc_snapshot;
        if (hxc_record_3b3e16aa_retain(&hxc_l_saved) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_l_inventory = hxc_l_saved.hxc_inventory;
        hxc_l_conversation = hxc_l_saved.hxc_conversation;
        hxc_string hxc_l_tmp_string_assignment_replacement_owner_n184 = hxc_l_saved.hxc_activeDialogue;
        if (hxc_string_retain(hxc_l_tmp_string_assignment_replacement_owner_n184) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_string hxc_l_tmp_string_assignment_replacement_owned_load_result_n393 = hxc_l_tmp_string_assignment_replacement_owner_n184;
        if (hxc_string_release(&hxc_l_activeDialogue) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_l_activeDialogue = hxc_l_tmp_string_assignment_replacement_owned_load_result_n393;
        hxc_string hxc_l_tmp_string_assignment_replacement_owner_n185 = hxc_l_saved.hxc_latestJournalId;
        if (hxc_string_retain(hxc_l_tmp_string_assignment_replacement_owner_n185) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_string hxc_l_tmp_string_assignment_replacement_owned_load_result_n395 = hxc_l_tmp_string_assignment_replacement_owner_n185;
        if (hxc_string_release(&hxc_l_latestJournalId) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_l_latestJournalId = hxc_l_tmp_string_assignment_replacement_owned_load_result_n395;
        hxc_string hxc_l_tmp_string_assignment_replacement_owner_n186 = hxc_l_saved.hxc_currentObjectiveId;
        if (hxc_string_retain(hxc_l_tmp_string_assignment_replacement_owner_n186) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_string hxc_l_tmp_string_assignment_replacement_owned_load_result_n397 = hxc_l_tmp_string_assignment_replacement_owner_n186;
        if (hxc_string_release(&hxc_l_currentObjectiveId) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_l_currentObjectiveId = hxc_l_tmp_string_assignment_replacement_owned_load_result_n397;
        hxc_l_availableInteractionPrompt = hxc_l_saved.hxc_availableInteractionPrompt;
        hxc_l_enemyActor = hxc_l_saved.hxc_enemyActor;
        hxc_l_enemyPhase_h906f9c33685e = hxc_l_saved.hxc_enemyPhase;
        hxc_l_swordCombat = hxc_l_saved.hxc_swordCombat;
        hxc_l_berryDrop = hxc_l_saved.hxc_berryDrop;
        hxc_l_cameraWaterBlend = hxc_l_saved.hxc_cameraWaterBlend;
        hxc_l_cameraMode = hxc_l_saved.hxc_cameraMode;
        hxc_l_lookX = hxc_l_saved.hxc_lookX;
        hxc_l_lookY = hxc_l_saved.hxc_lookY;
        hxc_l_lookZ = hxc_l_saved.hxc_lookZ;
        hxc_l_accumulator = hxc_l_saved.hxc_accumulator;
        hxc_l_motionHistory = hxc_l_saved.hxc_motionHistory;
        hxc_l_jumpQueued = hxc_l_saved.hxc_jumpQueued;
        hxc_l_swordQueued = hxc_l_saved.hxc_swordQueued;
        hxc_l_placementBlockedFrames = hxc_l_saved.hxc_placementBlockedFrames;
        hxc_l_strikeHitFrames = hxc_l_saved.hxc_strikeHitFrames;
        hxc_l_enemyDefeatedFrames = hxc_l_saved.hxc_enemyDefeatedFrames;
        hxc_l_enemyAttackFrames = hxc_l_saved.hxc_enemyAttackFrames;
        hxc_l_pickupFrames = hxc_l_saved.hxc_pickupFrames;
        hxc_l_pickupAmount = hxc_l_saved.hxc_pickupAmount;
        hxc_l_inventoryFullFrames = hxc_l_saved.hxc_inventoryFullFrames;
        hxc_l_inventoryFullReason = hxc_l_saved.hxc_inventoryFullReason;
        hxc_l_recoveryFeedback = hxc_l_saved.hxc_recoveryFeedback;
        hxc_l_recoveryFeedbackFrames = hxc_l_saved.hxc_recoveryFeedbackFrames;
        hxc_l_aquaticEquipmentCode = hxc_l_saved.hxc_aquaticEquipmentCode;
        hxc_l_aquaticEquipmentFrames = hxc_l_saved.hxc_aquaticEquipmentFrames;
        hxc_l_gc_roots[313] = (const void *)hxc_l_saved.hxc_campaignLevel;
        hxc_l_campaignLevel = hxc_l_saved.hxc_campaignLevel;
        hxc_l_gc_roots[314] = (const void *)hxc_l_saved.hxc_pendingCampaignLevel;
        hxc_l_pendingCampaignLevel = hxc_l_saved.hxc_pendingCampaignLevel;
        hxc_l_gc_roots[315] = (const void *)hxc_l_saved.hxc_pendingCampaignPortal;
        hxc_l_pendingCampaignPortal = hxc_l_saved.hxc_pendingCampaignPortal;
        hxc_string hxc_l_tmp_string_assignment_replacement_owner_n187 = hxc_l_saved.hxc_pendingCampaignLabel;
        if (hxc_string_retain(hxc_l_tmp_string_assignment_replacement_owner_n187) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_string hxc_l_tmp_string_assignment_replacement_owned_load_result_n428 = hxc_l_tmp_string_assignment_replacement_owner_n187;
        if (hxc_string_release(&hxc_l_pendingCampaignLabel) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_l_pendingCampaignLabel = hxc_l_tmp_string_assignment_replacement_owned_load_result_n428;
        hxc_l_loadingFramePresented = hxc_l_saved.hxc_loadingFramePresented;
        hxc_string hxc_l_tmp_string_assignment_replacement_owner_n188 = hxc_l_saved.hxc_levelLabel;
        if (hxc_string_retain(hxc_l_tmp_string_assignment_replacement_owner_n188) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_string hxc_l_tmp_string_assignment_replacement_owned_load_result_n431 = hxc_l_tmp_string_assignment_replacement_owner_n188;
        if (hxc_string_release(&hxc_l_levelLabel) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_l_levelLabel = hxc_l_tmp_string_assignment_replacement_owned_load_result_n431;
        hxc_l_selectedCampaignLevelIndex = hxc_l_saved.hxc_selectedCampaignLevelIndex;
        hxc_l_gc_roots[316] = (const void *)hxc_l_saved.hxc_preparedPortal;
        hxc_l_preparedPortal = hxc_l_saved.hxc_preparedPortal;
        if (hxc_l_self == NULL)
        {
          abort();
        }
        struct hxc_caxecraft_app_TerrainRenderer *hxc_l_tmp_owned_class_field_address_n434 = &(*hxc_l_self).hxc_terrainRenderer;
        struct hxc_caxecraft_app_TerrainChunkCache *hxc_l_tmp_owned_class_field_address_n435 = &(*hxc_l_tmp_owned_class_field_address_n434).hxc_cache;
        if (hxc_l_tmp_owned_class_field_address_n435 == NULL)
        {
          abort();
        }
        hxc_caxecraft_app_TerrainChunkCache_invalidateAll(hxc_l_tmp_owned_class_field_address_n435);
        struct hxc_caxecraft_input_NavigationRepeater *hxc_l_tmp_owned_class_field_address_n436 = &(*hxc_l_self).hxc_editorNavigation;
        if (hxc_l_tmp_owned_class_field_address_n436 == NULL)
        {
          abort();
        }
        hxc_caxecraft_input_NavigationRepeater_release(hxc_l_tmp_owned_class_field_address_n436);
        EnableCursor();
        hxc_record_3b3e16aa_destroy(&hxc_l_saved);
      }
      hxc_l_frameCount = hxc_i32_add_wrapping(hxc_l_frameCount, 1);
      hxc_optional_9d9d283f_destroy(&hxc_l_run_hab9bcd6c1cb0);
    }
    hxc_l_gc_roots[160] = hxc_l_activeEditorTestPlay.hxc_has_value ? (const void *)hxc_l_activeEditorTestPlay.hxc_value.hxc_runtime : NULL;
    hxc_l_gc_roots[161] = hxc_l_activeEditorTestPlay.hxc_has_value ? (const void *)hxc_l_activeEditorTestPlay.hxc_value.hxc_snapshot.hxc_campaignLevel : NULL;
    hxc_l_gc_roots[162] = hxc_l_activeEditorTestPlay.hxc_has_value ? (const void *)hxc_l_activeEditorTestPlay.hxc_value.hxc_snapshot.hxc_pendingCampaignLevel : NULL;
    hxc_l_gc_roots[163] = hxc_l_activeEditorTestPlay.hxc_has_value ? (const void *)hxc_l_activeEditorTestPlay.hxc_value.hxc_snapshot.hxc_pendingCampaignPortal : NULL;
    hxc_l_gc_roots[164] = hxc_l_activeEditorTestPlay.hxc_has_value ? (const void *)hxc_l_activeEditorTestPlay.hxc_value.hxc_snapshot.hxc_preparedPortal : NULL;
    hxc_l_unfinishedEditorTestPlay = hxc_l_activeEditorTestPlay;
    if (hxc_optional_9d9d283f_retain(&hxc_l_unfinishedEditorTestPlay) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_gc_roots[165] = hxc_l_unfinishedEditorTestPlay.hxc_has_value ? (const void *)hxc_l_unfinishedEditorTestPlay.hxc_value.hxc_runtime : NULL;
    hxc_l_gc_roots[166] = hxc_l_unfinishedEditorTestPlay.hxc_has_value ? (const void *)hxc_l_unfinishedEditorTestPlay.hxc_value.hxc_snapshot.hxc_campaignLevel : NULL;
    hxc_l_gc_roots[167] = hxc_l_unfinishedEditorTestPlay.hxc_has_value ? (const void *)hxc_l_unfinishedEditorTestPlay.hxc_value.hxc_snapshot.hxc_pendingCampaignLevel : NULL;
    hxc_l_gc_roots[168] = hxc_l_unfinishedEditorTestPlay.hxc_has_value ? (const void *)hxc_l_unfinishedEditorTestPlay.hxc_value.hxc_snapshot.hxc_pendingCampaignPortal : NULL;
    hxc_l_gc_roots[169] = hxc_l_unfinishedEditorTestPlay.hxc_has_value ? (const void *)hxc_l_unfinishedEditorTestPlay.hxc_value.hxc_snapshot.hxc_preparedPortal : NULL;
    if (hxc_l_unfinishedEditorTestPlay.hxc_has_value)
    {
      struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun hxc_l_tmp_load_result_n2241 = hxc_l_unfinishedEditorTestPlay;
      hxc_l_gc_roots[734] = hxc_l_tmp_load_result_n2241.hxc_has_value ? (const void *)hxc_l_tmp_load_result_n2241.hxc_value.hxc_runtime : NULL;
      hxc_l_gc_roots[735] = hxc_l_tmp_load_result_n2241.hxc_has_value ? (const void *)hxc_l_tmp_load_result_n2241.hxc_value.hxc_snapshot.hxc_campaignLevel : NULL;
      hxc_l_gc_roots[736] = hxc_l_tmp_load_result_n2241.hxc_has_value ? (const void *)hxc_l_tmp_load_result_n2241.hxc_value.hxc_snapshot.hxc_pendingCampaignLevel : NULL;
      hxc_l_gc_roots[737] = hxc_l_tmp_load_result_n2241.hxc_has_value ? (const void *)hxc_l_tmp_load_result_n2241.hxc_value.hxc_snapshot.hxc_pendingCampaignPortal : NULL;
      hxc_l_gc_roots[738] = hxc_l_tmp_load_result_n2241.hxc_has_value ? (const void *)hxc_l_tmp_load_result_n2241.hxc_value.hxc_snapshot.hxc_preparedPortal : NULL;
      if (!hxc_l_tmp_load_result_n2241.hxc_has_value)
      {
        abort();
      }
      hxc_l_gc_roots[739] = (const void *)hxc_l_tmp_load_result_n2241.hxc_value.hxc_runtime;
      hxc_l_gc_roots[740] = (const void *)hxc_l_tmp_load_result_n2241.hxc_value.hxc_snapshot.hxc_campaignLevel;
      hxc_l_gc_roots[741] = (const void *)hxc_l_tmp_load_result_n2241.hxc_value.hxc_snapshot.hxc_pendingCampaignLevel;
      hxc_l_gc_roots[742] = (const void *)hxc_l_tmp_load_result_n2241.hxc_value.hxc_snapshot.hxc_pendingCampaignPortal;
      hxc_l_gc_roots[743] = (const void *)hxc_l_tmp_load_result_n2241.hxc_value.hxc_snapshot.hxc_preparedPortal;
      struct hxc_caxecraft_app_EditorTestPlayRuntime *hxc_l_tmp_optional_record_field_project_n2243 = hxc_l_tmp_load_result_n2241.hxc_value.hxc_runtime;
      hxc_l_gc_roots[744] = (const void *)hxc_l_tmp_optional_record_field_project_n2243;
      if (hxc_l_tmp_optional_record_field_project_n2243 == NULL)
      {
        abort();
      }
      hxc_caxecraft_app_EditorTestPlayRuntime_stop(hxc_l_tmp_optional_record_field_project_n2243);
      hxc_l_gc_roots[745] = (struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = false }.hxc_has_value ? (const void *)(struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = false }.hxc_value.hxc_runtime : NULL;
      hxc_l_gc_roots[746] = (struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = false }.hxc_has_value ? (const void *)(struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = false }.hxc_value.hxc_snapshot.hxc_campaignLevel : NULL;
      hxc_l_gc_roots[747] = (struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = false }.hxc_has_value ? (const void *)(struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = false }.hxc_value.hxc_snapshot.hxc_pendingCampaignLevel : NULL;
      hxc_l_gc_roots[748] = (struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = false }.hxc_has_value ? (const void *)(struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = false }.hxc_value.hxc_snapshot.hxc_pendingCampaignPortal : NULL;
      hxc_l_gc_roots[749] = (struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = false }.hxc_has_value ? (const void *)(struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = false }.hxc_value.hxc_snapshot.hxc_preparedPortal : NULL;
      hxc_optional_9d9d283f_destroy(&hxc_l_activeEditorTestPlay);
      hxc_l_activeEditorTestPlay = (struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = false };
      struct hxc_caxecraft_app_CaxecraftEditorScreen *hxc_l_tmp_load_result_n2244 = hxc_l_editorScreen;
      hxc_l_gc_roots[750] = (const void *)hxc_l_tmp_load_result_n2244;
      if (hxc_l_tmp_load_result_n2244 == NULL)
      {
        abort();
      }
      hxc_caxecraft_app_CaxecraftEditorScreen_finishTestPlay(hxc_l_tmp_load_result_n2244, false);
    }
    EnableCursor();
    struct hxc_caxecraft_app_RuntimeVoxelModelCatalog *hxc_l_tmp_load_result_n2245 = hxc_l_runtimeModels;
    hxc_l_gc_roots[751] = (const void *)hxc_l_tmp_load_result_n2245;
    if (hxc_l_tmp_load_result_n2245 == NULL)
    {
      abort();
    }
    hxc_caxecraft_app_RuntimeVoxelModelCatalog_unload(hxc_l_tmp_load_result_n2245);
    struct hxc_caxecraft_app_RuntimeTextureAtlasCatalog *hxc_l_tmp_load_result_n2246 = hxc_l_runtimeTextures;
    hxc_l_gc_roots[752] = (const void *)hxc_l_tmp_load_result_n2246;
    if (hxc_l_tmp_load_result_n2246 == NULL)
    {
      abort();
    }
    hxc_caxecraft_app_RuntimeTextureAtlasCatalog_unload(hxc_l_tmp_load_result_n2246);
    if (hxc_l_adventureTerrainTextureReady)
    {
      UnloadTexture(hxc_l_adventureTerrainTexture);
    }
    if (hxc_l_terrainTextureReady)
    {
      UnloadTexture(hxc_l_terrainTexture);
    }
    if (hxc_l_entityTextureReady)
    {
      UnloadTexture(hxc_l_entityTexture);
    }
    if (hxc_l_adventureItemTextureReady)
    {
      UnloadTexture(hxc_l_adventureItemTexture);
    }
    if (hxc_l_itemTextureReady)
    {
      UnloadTexture(hxc_l_itemTexture);
    }
    if (hxc_l_hudTextureReady)
    {
      UnloadTexture(hxc_l_hudTexture);
    }
    if (hxc_l_wordmarkTextureReady)
    {
      UnloadTexture(hxc_l_wordmarkTexture);
    }
    if (hxc_l_titleTextureReady)
    {
      UnloadTexture(hxc_l_titleTexture);
    }
    CloseWindow();
    hxc_optional_9d9d283f_destroy(&hxc_l_unfinishedEditorTestPlay);
    if (hxc_array_ref_release(hxc_l_initialActorPhases) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_enum_2c0e695d_destroy(&hxc_l_tmp_static_call_argument_0_owner_n102);
    if (hxc_string_release(&hxc_l_currentObjectiveId) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_latestJournalId) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_activeDialogue) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_tmp_object_storage_n92.hxc_stateStartTicks) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_tmp_object_storage_n92.hxc_stateIds) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_tmp_object_storage_n92.hxc_objectIds) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_record_f6127605_destroy(&hxc_l_receipt);
    if (hxc_string_release(&hxc_l_levelLabel) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_enum_4b68fff2_destroy(&hxc_l_g_hccea2de18028);
    if (hxc_string_release(&hxc_l_pendingCampaignLabel) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_optional_9d9d283f_destroy(&hxc_l_activeEditorTestPlay);
    hxc_enum_11853fd1_destroy(&hxc_l_g_h1d08383de4f8);
    hxc_record_f6127605_destroy(&hxc_l_tmp_record_field_logicalPath_receiver_owner_n23);
    hxc_enum_777c0102_destroy(&hxc_l_g_h6904c3060e30);
    hxc_record_8aab5967_destroy(&hxc_l_tmp_managed_flow_owner_n17);
    hxc_enum_d4806d01_destroy(&hxc_l_g_h1d9290112915);
    hxc_enum_fcb4401b_destroy(&hxc_l_g_h14daa0fd57c5);
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return;
  }
  if (hxc_l_adventureTerrainTextureReady)
  {
    UnloadTexture(hxc_l_adventureTerrainTexture);
  }
  if (hxc_l_terrainTextureReady)
  {
    UnloadTexture(hxc_l_terrainTexture);
  }
  if (hxc_l_entityTextureReady)
  {
    UnloadTexture(hxc_l_entityTexture);
  }
  if (hxc_l_adventureItemTextureReady)
  {
    UnloadTexture(hxc_l_adventureItemTexture);
  }
  if (hxc_l_itemTextureReady)
  {
    UnloadTexture(hxc_l_itemTexture);
  }
  if (hxc_l_hudTextureReady)
  {
    UnloadTexture(hxc_l_hudTexture);
  }
  if (hxc_l_wordmarkTextureReady)
  {
    UnloadTexture(hxc_l_wordmarkTexture);
  }
  if (hxc_l_titleTextureReady)
  {
    UnloadTexture(hxc_l_titleTexture);
  }
  CloseWindow();
  hxc_record_f6127605_destroy(&hxc_l_receipt);
  if (hxc_string_release(&hxc_l_levelLabel) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_enum_4b68fff2_destroy(&hxc_l_g_hccea2de18028);
  if (hxc_string_release(&hxc_l_pendingCampaignLabel) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_optional_9d9d283f_destroy(&hxc_l_activeEditorTestPlay);
  hxc_enum_11853fd1_destroy(&hxc_l_g_h1d08383de4f8);
  hxc_record_f6127605_destroy(&hxc_l_tmp_record_field_logicalPath_receiver_owner_n23);
  hxc_enum_777c0102_destroy(&hxc_l_g_h6904c3060e30);
  hxc_record_8aab5967_destroy(&hxc_l_tmp_managed_flow_owner_n17);
  hxc_enum_d4806d01_destroy(&hxc_l_g_h1d9290112915);
  hxc_enum_fcb4401b_destroy(&hxc_l_g_h14daa0fd57c5);
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return;
}
