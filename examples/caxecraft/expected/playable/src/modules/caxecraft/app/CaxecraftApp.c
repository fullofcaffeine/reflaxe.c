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
    int32_t hxc_l_phaseCell = 0;
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
  hxc_string hxc_l_tmp_cstring_arg_owner_owner_n76 = { 0 };
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
  hxc_call_cstring hxc_l_tmp_string_prepare_cstring_result_n167 = HXC_CALL_CSTRING_INITIALIZER;
  if (hxc_string_prepare_call_cstring(&(hxc_string){ (const uint8_t *)"/", 1, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_prepare_cstring_result_n167) != HXC_STATUS_OK)
  {
    abort();
  }
  DrawText(hxc_l_tmp_string_prepare_cstring_result_n167.data, (int32_t)hxc_l_tmp_load_result_n165, (int32_t)hxc_l_tmp_load_result_n166, (int32_t)14, hxc_l_color);
  if (hxc_call_cstring_dispose(&hxc_l_tmp_string_prepare_cstring_result_n167) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_tmp_load_result_n168 = hxc_l_lineCount;
  int32_t hxc_l_tmp_record_field_load_result_n169 = hxc_l_panel.hxc_x;
  int32_t hxc_l_tmp_record_field_load_result_n170 = hxc_l_panel.hxc_width;
  hxc_caxecraft_app_HudDigits_drawNumber(hxc_l_tmp_load_result_n168, hxc_i32_subtract_wrapping(hxc_i32_add_wrapping(hxc_l_tmp_record_field_load_result_n169, hxc_l_tmp_record_field_load_result_n170), 34), hxc_i32_add_wrapping(hxc_l_panel.hxc_y, 17), 1, hxc_l_color);
  hxc_string hxc_l_tmp_call_result_n173 = hxc_caxecraft_input_ControlPrompts_conversationHelpMessage(hxc_l_view.hxc_promptDevice);
  hxc_l_message = hxc_l_tmp_call_result_n173;
  int32_t hxc_l_x_hf4a805aef76e = hxc_l_help.hxc_x;
  int32_t hxc_l_y_hc61d3628eb77 = hxc_l_help.hxc_y;
  if (hxc_l_uiCatalog == NULL)
  {
    abort();
  }
  hxc_string hxc_l_tmp_instance_call_result_n177 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_uiCatalog, hxc_l_locale, hxc_l_message);
  hxc_l_tmp_cstring_arg_owner_owner_n76 = hxc_l_tmp_instance_call_result_n177;
  hxc_string hxc_l_tmp_cstring_arg_owner_borrow_result_n178 = hxc_l_tmp_cstring_arg_owner_owner_n76;
  int32_t hxc_l_tmp_load_result_n179 = hxc_l_x_hf4a805aef76e;
  int32_t hxc_l_tmp_load_result_n180 = hxc_l_y_hc61d3628eb77;
  hxc_call_cstring hxc_l_tmp_string_prepare_cstring_result_n181 = HXC_CALL_CSTRING_INITIALIZER;
  if (hxc_string_prepare_call_cstring(&hxc_l_tmp_cstring_arg_owner_borrow_result_n178, hxc_default_allocator(), &hxc_l_tmp_string_prepare_cstring_result_n181) != HXC_STATUS_OK)
  {
    abort();
  }
  DrawText(hxc_l_tmp_string_prepare_cstring_result_n181.data, (int32_t)hxc_l_tmp_load_result_n179, (int32_t)hxc_l_tmp_load_result_n180, (int32_t)14, hxc_l_color);
  if (hxc_call_cstring_dispose(&hxc_l_tmp_string_prepare_cstring_result_n181) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_string_release(&hxc_l_tmp_cstring_arg_owner_owner_n76) != HXC_STATUS_OK)
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
    double hxc_l_baseX = 0.0;
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
    double hxc_l_baseZ = 0.0;
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
  const void *volatile hxc_l_gc_roots[755] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 755, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_caxecraft_app_CampaignPortalPreloadResult hxc_l_g_h20d6048e709c = { 0 };
  hxc_string hxc_l_tmp_sys_println_string_argument_owner_n116 = { 0 };
  struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun hxc_l_run_h2b932852df37 = { 0 };
  struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun hxc_l_run_h01967e139705 = { 0 };
  struct hxc_caxecraft_content_RuntimeLevelEditableSourceClaim hxc_l_g_h1d9290112915 = { 0 };
  struct hxc_caxecraft_content_RuntimeLevelEditableSource hxc_l_source_hf8825c3e7561 = { 0 };
  struct hxc_caxecraft_content_RuntimeLevelEditableSource hxc_l_tmp_managed_flow_owner_n17 = { 0 };
  struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun hxc_l_run_h3bea1e3d2ec8 = { 0 };
  struct hxc_caxecraft_app_CaxecraftApp_EditorTestPlayShellSnapshot hxc_l_saved = { 0 };
  struct hxc_caxecraft_editor_EditorOpenResult hxc_l_g_h6904c3060e30 = { 0 };
  hxc_string hxc_l_tmp_sys_println_string_argument_owner_n193 = { 0 };
  struct hxc_caxecraft_editor_EditorPackageOpenResult hxc_l_g_h1d08383de4f8 = { 0 };
  struct hxc_caxecraft_content_CampaignLevelLoadResult hxc_l_g_hb5a295da9005 = { 0 };
  struct hxc_caxecraft_content_RuntimeLevelReceipt hxc_l_tmp_record_field_logicalPath_receiver_owner_n23 = { 0 };
  struct hxc_caxecraft_content_CampaignLevelLoadError hxc_l_error_h44e49fe88f76 = { 0 };
  hxc_string hxc_l_tmp_string_concat_right_owner_n231 = { 0 };
  hxc_string hxc_l_tmp_sys_println_string_argument_owner_n232 = { 0 };
  struct hxc_caxecraft_app_CaxecraftApp_AvailableInteractionTarget hxc_l_tmp_static_call_argument_0_owner_n246 = { 0 };
  struct hxc_array_ref *hxc_l_phases = { 0 };
  struct hxc_caxecraft_editor_EditorPackageError hxc_l_error_hc49dda9de41e = { 0 };
  struct hxc_caxecraft_app_CampaignPortalPreloadResult hxc_l_g_h3e205aaff29c = { 0 };
  hxc_string hxc_l_tmp_sys_println_string_argument_owner_n257 = { 0 };
  hxc_string hxc_l_tmp_string_concat_left_owner_n258 = { 0 };
  hxc_string hxc_l_tmp_string_concat_left_owner_n259 = { 0 };
  hxc_string hxc_l_tmp_string_concat_right_owner_n26 = { 0 };
  hxc_string hxc_l_tmp_string_concat_right_owner_n260 = { 0 };
  hxc_string hxc_l_tmp_sys_println_string_argument_owner_n261 = { 0 };
  hxc_string hxc_l_tmp_sys_println_string_argument_owner_n27 = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n275 = { 0 };
  hxc_string hxc_l_line_hd5cd12f85ec2 = { 0 };
  struct hxc_caxecraft_app_CaxecraftApp_AvailableInteractionTarget hxc_l_g_hb71d32c11dac = { 0 };
  hxc_string hxc_l_id_h415df953ef33 = { 0 };
  struct hxc_caxecraft_app_EditorScreenAction hxc_l_editorActionForFrame = { 0 };
  struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun hxc_l_activeEditorTestPlay = { 0 };
  struct hxc_caxecraft_domain_GameTickResult hxc_l_gameTick = { 0 };
  struct hxc_optional_caxecraft_scenario_FlowTickResult hxc_l_flow = { 0 };
  struct hxc_array_ref *hxc_l_g1_h729617e1d868 = { 0 };
  struct hxc_caxecraft_scenario_FlowPresentationEvent hxc_l_event_h3221db1879e6 = { 0 };
  struct hxc_caxecraft_scenario_FlowPresentationEvent hxc_l_tmp_array_element_owner_n337 = { 0 };
  hxc_string hxc_l_id_hd70653f4f19c = { 0 };
  hxc_string hxc_l_id_hd83d361b952f = { 0 };
  hxc_string hxc_l_exit = { 0 };
  hxc_string hxc_l_tmp_sys_println_string_argument_owner_n344 = { 0 };
  hxc_string hxc_l_tmp_sys_println_string_argument_owner_n348 = { 0 };
  hxc_string hxc_l_tmp_string_concat_left_owner_n350 = { 0 };
  hxc_string hxc_l_tmp_string_concat_left_owner_n351 = { 0 };
  hxc_string hxc_l_tmp_sys_println_string_argument_owner_n352 = { 0 };
  hxc_string hxc_l_pendingCampaignLabel = { 0 };
  struct hxc_array_ref *hxc_l_g1_h50401e609561 = { 0 };
  struct hxc_caxecraft_domain_ActorControllerEvent hxc_l_event_h2fe0dab417fb = { 0 };
  struct hxc_caxecraft_domain_ActorControllerEvent hxc_l_tmp_array_element_owner_n376 = { 0 };
  hxc_string hxc_l_drop = { 0 };
  struct hxc_array_ref *hxc_l_actorPhases = { 0 };
  struct hxc_caxecraft_content_CampaignPackageLoadResult hxc_l_g_hccea2de18028 = { 0 };
  hxc_string hxc_l_swordItemType = { 0 };
  struct hxc_caxecraft_domain_CharacterDamageCommand hxc_l_tmp_instance_call_argument_0_owner_n395 = { 0 };
  hxc_string hxc_l_selectedItemType = { 0 };
  hxc_string hxc_l_placementItemType = { 0 };
  struct hxc_caxecraft_content_RuntimeLevelReceipt hxc_l_tmp_record_field_logicalPath_receiver_owner_n42 = { 0 };
  struct hxc_caxecraft_app_CaxecraftApp_AvailableInteractionTarget hxc_l_tmp_static_call_argument_0_owner_n428 = { 0 };
  struct hxc_array_ref *hxc_l_presentationActorPhases = { 0 };
  struct hxc_array_ref *hxc_l_dialogueActors = { 0 };
  struct hxc_caxecraft_content_CampaignPackageLoadError hxc_l_gerror = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n445 = { 0 };
  hxc_string hxc_l_adventureTagline = { 0 };
  hxc_string hxc_l_levelLabel = { 0 };
  hxc_string hxc_l_tmp_cstring_arg_owner_owner_n467 = { 0 };
  struct hxc_caxecraft_content_RuntimeLevelReceipt hxc_l_tmp_record_field_logicalPath_receiver_owner_n47 = { 0 };
  hxc_string hxc_l_tmp_cstring_arg_owner_owner_n475 = { 0 };
  hxc_string hxc_l_tmp_cstring_arg_owner_owner_n483 = { 0 };
  hxc_string hxc_l_tmp_cstring_arg_owner_owner_n489 = { 0 };
  hxc_string hxc_l_message_hfdae3ea29a06 = { 0 };
  hxc_string hxc_l_tmp_cstring_arg_owner_owner_n493 = { 0 };
  hxc_string hxc_l_message_hb89ed1638090 = { 0 };
  hxc_string hxc_l_tmp_cstring_arg_owner_owner_n496 = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n502 = { 0 };
  hxc_string hxc_l_tmp = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n506 = { 0 };
  hxc_string hxc_l_tmp_static_call_argument_9_owner_n507 = { 0 };
  hxc_string hxc_l_conversationSpeaker = { 0 };
  struct hxc_caxecraft_content_RuntimeLevelReceipt hxc_l_receipt = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n563 = { 0 };
  hxc_string hxc_l_hudView7_hef5dfe6036f4 = { 0 };
  hxc_string hxc_l_hudView8 = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n569 = { 0 };
  hxc_string hxc_l_tmp_sys_println_string_argument_owner_n57 = { 0 };
  hxc_string hxc_l_hudView10 = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n573 = { 0 };
  hxc_string hxc_l_hudView11 = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n579 = { 0 };
  hxc_string hxc_l_tmp_string_concat_right_owner_n58 = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n580 = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n585 = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n586 = { 0 };
  struct hxc_caxecraft_app_HudView hxc_l_hudView_h32a7fe570f99 = { 0 };
  hxc_string hxc_l_tmp_sys_println_string_argument_owner_n59 = { 0 };
  hxc_string hxc_l_tmp_string_concat_right_owner_n60 = { 0 };
  struct hxc_caxecraft_app_EditorTestPlayRequest hxc_l_tmp_instance_call_argument_0_owner_n600 = { 0 };
  struct hxc_caxecraft_app_EditorTestPlayStartResult hxc_l_tmp_enum_switch_subject_owner_n601 = { 0 };
  struct hxc_array_ref *hxc_l_testPhases = { 0 };
  hxc_string hxc_l_tmp_sys_println_string_argument_owner_n61 = { 0 };
  struct hxc_caxecraft_app_CaxecraftApp_EditorTestPlayShellSnapshot hxc_l_snapshot = { 0 };
  struct hxc_caxecraft_app_CaxecraftApp_AvailableInteractionTarget hxc_l_tmp_static_call_argument_0_owner_n622 = { 0 };
  struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun hxc_l_unfinishedEditorTestPlay = { 0 };
  struct hxc_caxecraft_content_RuntimeContentLoadResult hxc_l_g_h14daa0fd57c5 = { 0 };
  hxc_string hxc_l_activeDialogue = { 0 };
  hxc_string hxc_l_latestJournalId = { 0 };
  hxc_string hxc_l_currentObjectiveId = { 0 };
  struct hxc_caxecraft_app_CaxecraftApp_AvailableInteractionTarget hxc_l_tmp_static_call_argument_0_owner_n94 = { 0 };
  struct hxc_array_ref *hxc_l_initialActorPhases = { 0 };
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
  hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n148 = HXC_BORROWED_CSTRING_INITIALIZER;
  if (hxc_string_borrow_cstring(&(hxc_string){ (const uint8_t *)"assets/showcase/title-panorama.png", 34, true, NULL }, &hxc_l_tmp_string_borrow_cstring_result_n148) != HXC_STATUS_OK)
  {
    abort();
  }
  struct Texture hxc_l_tmp_native_call_result_n149 = LoadTexture(hxc_l_tmp_string_borrow_cstring_result_n148.data);
  struct Texture hxc_l_titleTexture = hxc_l_tmp_native_call_result_n149;
  bool hxc_l_tmp_native_call_result_n151 = IsTextureValid(hxc_l_titleTexture);
  bool hxc_l_titleTextureReady = hxc_l_tmp_native_call_result_n151;
  hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n152 = HXC_BORROWED_CSTRING_INITIALIZER;
  if (hxc_string_borrow_cstring(&(hxc_string){ (const uint8_t *)"assets/branding/caxecraft-wordmark.png", 38, true, NULL }, &hxc_l_tmp_string_borrow_cstring_result_n152) != HXC_STATUS_OK)
  {
    abort();
  }
  struct Texture hxc_l_tmp_native_call_result_n153 = LoadTexture(hxc_l_tmp_string_borrow_cstring_result_n152.data);
  struct Texture hxc_l_wordmarkTexture = hxc_l_tmp_native_call_result_n153;
  bool hxc_l_tmp_native_call_result_n155 = IsTextureValid(hxc_l_wordmarkTexture);
  bool hxc_l_wordmarkTextureReady = hxc_l_tmp_native_call_result_n155;
  hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n156 = HXC_BORROWED_CSTRING_INITIALIZER;
  if (hxc_string_borrow_cstring(&(hxc_string){ (const uint8_t *)"assets/atlases/hud.png", 22, true, NULL }, &hxc_l_tmp_string_borrow_cstring_result_n156) != HXC_STATUS_OK)
  {
    abort();
  }
  struct Texture hxc_l_tmp_native_call_result_n157 = LoadTexture(hxc_l_tmp_string_borrow_cstring_result_n156.data);
  struct Texture hxc_l_hudTexture = hxc_l_tmp_native_call_result_n157;
  bool hxc_l_tmp_native_call_result_n159 = IsTextureValid(hxc_l_hudTexture);
  bool hxc_l_hudTextureReady = hxc_l_tmp_native_call_result_n159;
  hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n160 = HXC_BORROWED_CSTRING_INITIALIZER;
  if (hxc_string_borrow_cstring(&(hxc_string){ (const uint8_t *)"assets/atlases/items.png", 24, true, NULL }, &hxc_l_tmp_string_borrow_cstring_result_n160) != HXC_STATUS_OK)
  {
    abort();
  }
  struct Texture hxc_l_tmp_native_call_result_n161 = LoadTexture(hxc_l_tmp_string_borrow_cstring_result_n160.data);
  struct Texture hxc_l_itemTexture = hxc_l_tmp_native_call_result_n161;
  bool hxc_l_tmp_native_call_result_n163 = IsTextureValid(hxc_l_itemTexture);
  bool hxc_l_itemTextureReady = hxc_l_tmp_native_call_result_n163;
  hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n164 = HXC_BORROWED_CSTRING_INITIALIZER;
  if (hxc_string_borrow_cstring(&(hxc_string){ (const uint8_t *)"assets/atlases/adventure-items.png", 34, true, NULL }, &hxc_l_tmp_string_borrow_cstring_result_n164) != HXC_STATUS_OK)
  {
    abort();
  }
  struct Texture hxc_l_tmp_native_call_result_n165 = LoadTexture(hxc_l_tmp_string_borrow_cstring_result_n164.data);
  struct Texture hxc_l_adventureItemTexture = hxc_l_tmp_native_call_result_n165;
  bool hxc_l_tmp_native_call_result_n167 = IsTextureValid(hxc_l_adventureItemTexture);
  bool hxc_l_adventureItemTextureReady = hxc_l_tmp_native_call_result_n167;
  hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n168 = HXC_BORROWED_CSTRING_INITIALIZER;
  if (hxc_string_borrow_cstring(&(hxc_string){ (const uint8_t *)"assets/atlases/entities.png", 27, true, NULL }, &hxc_l_tmp_string_borrow_cstring_result_n168) != HXC_STATUS_OK)
  {
    abort();
  }
  struct Texture hxc_l_tmp_native_call_result_n169 = LoadTexture(hxc_l_tmp_string_borrow_cstring_result_n168.data);
  struct Texture hxc_l_entityTexture = hxc_l_tmp_native_call_result_n169;
  bool hxc_l_tmp_native_call_result_n171 = IsTextureValid(hxc_l_entityTexture);
  bool hxc_l_entityTextureReady = hxc_l_tmp_native_call_result_n171;
  struct Texture hxc_l_tmp_load_result_n172 = hxc_l_hudTexture;
  bool hxc_l_tmp_load_result_n173 = hxc_l_hudTextureReady;
  struct Texture hxc_l_tmp_load_result_n174 = hxc_l_itemTexture;
  bool hxc_l_tmp_load_result_n175 = hxc_l_itemTextureReady;
  struct Texture hxc_l_tmp_load_result_n176 = hxc_l_adventureItemTexture;
  bool hxc_l_tmp_load_result_n177 = hxc_l_adventureItemTextureReady;
  struct Texture hxc_l_tmp_load_result_n178 = hxc_l_entityTexture;
  struct hxc_caxecraft_app_HudResources hxc_l_hudResources = (struct hxc_caxecraft_app_HudResources){ .hxc_adventureItemTexture = hxc_l_tmp_load_result_n176, .hxc_adventureItemTextureReady = hxc_l_tmp_load_result_n177, .hxc_entityTexture = hxc_l_tmp_load_result_n178, .hxc_entityTextureReady = hxc_l_entityTextureReady, .hxc_hudTexture = hxc_l_tmp_load_result_n172, .hxc_hudTextureReady = hxc_l_tmp_load_result_n173, .hxc_itemTexture = hxc_l_tmp_load_result_n174, .hxc_itemTextureReady = hxc_l_tmp_load_result_n175 };
  hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n181 = HXC_BORROWED_CSTRING_INITIALIZER;
  if (hxc_string_borrow_cstring(&(hxc_string){ (const uint8_t *)"assets/atlases/terrain.png", 26, true, NULL }, &hxc_l_tmp_string_borrow_cstring_result_n181) != HXC_STATUS_OK)
  {
    abort();
  }
  struct Texture hxc_l_tmp_native_call_result_n182 = LoadTexture(hxc_l_tmp_string_borrow_cstring_result_n181.data);
  struct Texture hxc_l_terrainTexture = hxc_l_tmp_native_call_result_n182;
  bool hxc_l_tmp_native_call_result_n184 = IsTextureValid(hxc_l_terrainTexture);
  bool hxc_l_terrainTextureReady = hxc_l_tmp_native_call_result_n184;
  hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n185 = HXC_BORROWED_CSTRING_INITIALIZER;
  if (hxc_string_borrow_cstring(&(hxc_string){ (const uint8_t *)"assets/atlases/adventure-terrain.png", 36, true, NULL }, &hxc_l_tmp_string_borrow_cstring_result_n185) != HXC_STATUS_OK)
  {
    abort();
  }
  struct Texture hxc_l_tmp_native_call_result_n186 = LoadTexture(hxc_l_tmp_string_borrow_cstring_result_n185.data);
  struct Texture hxc_l_adventureTerrainTexture = hxc_l_tmp_native_call_result_n186;
  bool hxc_l_tmp_native_call_result_n188 = IsTextureValid(hxc_l_adventureTerrainTexture);
  bool hxc_l_adventureTerrainTextureReady = hxc_l_tmp_native_call_result_n188;
  struct hxc_caxecraft_app_RuntimeTextureAtlasCatalog *hxc_l_tmp_call_result_n189 = hxc_caxecraft_app_RuntimeTextureAtlasCatalog_load((hxc_string){ (const uint8_t *)"assets/caxecraft-runtime-assets.json", 36, true, NULL });
  hxc_l_gc_roots[117] = (const void *)hxc_l_tmp_call_result_n189;
  struct hxc_caxecraft_app_RuntimeTextureAtlasCatalog *hxc_l_loadedRuntimeTextures = hxc_l_tmp_call_result_n189;
  hxc_l_gc_roots[118] = (const void *)hxc_l_loadedRuntimeTextures;
  hxc_l_gc_roots[119] = (const void *)NULL;
  if (!(hxc_l_loadedRuntimeTextures == NULL))
  {
    hxc_l_gc_roots[120] = (const void *)hxc_l_loadedRuntimeTextures;
    struct hxc_caxecraft_app_RuntimeTextureAtlasCatalog *hxc_l_runtimeTextures = hxc_l_loadedRuntimeTextures;
    struct hxc_caxecraft_app_RuntimeVoxelModelCatalog *hxc_l_tmp_managed_class_result_n208 = NULL;
    if (hxc_gc_allocate(&hxc_program_gc, &hxc_caxecraft_app_RuntimeVoxelModelCatalog_descriptor, (void **)&hxc_l_tmp_managed_class_result_n208) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_gc_roots[121] = (const void *)hxc_l_tmp_managed_class_result_n208;
    hxc_compiler_constructor_caxecraft_app_RuntimeVoxelModelCatalog(hxc_l_tmp_managed_class_result_n208);
    hxc_l_gc_roots[122] = (const void *)hxc_l_tmp_managed_class_result_n208;
    struct hxc_caxecraft_app_RuntimeVoxelModelCatalog *hxc_l_runtimeModels = hxc_l_tmp_managed_class_result_n208;
    struct hxc_caxecraft_app_VoxelFrameAnimationPlayer hxc_l_tmp_object_storage_n84 = { 0 };
    struct hxc_caxecraft_app_VoxelFrameAnimationPlayer *hxc_l_tmp_class_object_address_n209 = &hxc_l_tmp_object_storage_n84;
    hxc_compiler_constructor_caxecraft_app_VoxelFrameAnimationPlayer(hxc_l_tmp_class_object_address_n209);
    struct hxc_caxecraft_app_VoxelFrameAnimationPlayer *hxc_l_modelAnimations = hxc_l_tmp_class_object_address_n209;
    double hxc_l_cameraWaterBlend = 0.0;
    enum hxc_caxecraft_domain_PlayerCameraMode hxc_l_cameraMode = hxc_caxecraft_domain_PlayerCameraMode_FirstPerson;
    bool hxc_l_debugHudVisible = false;
    struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n211 = hxc_caxecraft_gameplay_Inventory_starter();
    struct hxc_caxecraft_gameplay_InventoryState hxc_l_inventory = hxc_l_tmp_call_result_n211;
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
    struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n212 = hxc_l_initialLevel;
    hxc_l_gc_roots[123] = (const void *)hxc_l_tmp_load_result_n212;
    if (hxc_l_tmp_load_result_n212 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_class_field_load_result_n213 = (*hxc_l_tmp_load_result_n212).hxc_levelPresentation;
    hxc_l_gc_roots[124] = (const void *)hxc_l_tmp_class_field_load_result_n213;
    if (hxc_l_tmp_class_field_load_result_n213 == NULL)
    {
      abort();
    }
    hxc_string hxc_l_tmp_class_field_load_result_n214 = (*hxc_l_tmp_class_field_load_result_n213).hxc_startingObjective;
    hxc_l_currentObjectiveId = hxc_l_tmp_class_field_load_result_n214;
    if (hxc_string_retain(hxc_l_currentObjectiveId) != HXC_STATUS_OK)
    {
      abort();
    }
    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n215 = hxc_l_initialSession;
    hxc_l_gc_roots[125] = (const void *)hxc_l_tmp_load_result_n215;
    hxc_l_gc_roots[126] = (const void *)hxc_l_initialLevel;
    struct hxc_caxecraft_app_CaxecraftApp_AvailableInteractionTarget hxc_l_tmp_call_result_n217 = hxc_caxecraft_app_CaxecraftApp_nearestAvailableInteraction(hxc_l_tmp_load_result_n215, hxc_l_initialLevel);
    hxc_l_tmp_static_call_argument_0_owner_n94 = hxc_l_tmp_call_result_n217;
    enum hxc_caxecraft_app_InteractionPrompt hxc_l_tmp_call_result_n219 = hxc_caxecraft_app_CaxecraftApp_promptForAvailableInteraction(hxc_l_tmp_static_call_argument_0_owner_n94);
    enum hxc_caxecraft_app_InteractionPrompt hxc_l_availableInteractionPrompt = hxc_l_tmp_call_result_n219;
    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n220 = hxc_l_initialSession;
    hxc_l_gc_roots[127] = (const void *)hxc_l_tmp_load_result_n220;
    if (hxc_l_tmp_load_result_n220 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n221 = hxc_l_initialLevel;
    hxc_l_gc_roots[128] = (const void *)hxc_l_tmp_load_result_n221;
    if (hxc_l_tmp_load_result_n221 == NULL)
    {
      abort();
    }
    int32_t hxc_l_tmp_class_field_load_result_n222 = (*hxc_l_tmp_load_result_n221).hxc_enemyId;
    struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n223 = hxc_caxecraft_domain_GameSession_readCharacter(hxc_l_tmp_load_result_n220, hxc_l_tmp_class_field_load_result_n222);
    struct hxc_caxecraft_domain_Character hxc_l_enemyActor = hxc_l_tmp_instance_call_result_n223;
    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n224 = hxc_l_initialSession;
    hxc_l_gc_roots[129] = (const void *)hxc_l_tmp_load_result_n224;
    if (hxc_l_tmp_load_result_n224 == NULL)
    {
      abort();
    }
    struct hxc_array_ref *hxc_l_tmp_instance_call_result_n225 = hxc_caxecraft_domain_GameSession_actorControllerStateSnapshots(hxc_l_tmp_load_result_n224);
    hxc_l_initialActorPhases = hxc_l_tmp_instance_call_result_n225;
    struct hxc_array_ref *hxc_l_tmp_load_result_n226 = hxc_l_initialActorPhases;
    struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n227 = hxc_l_initialLevel;
    hxc_l_gc_roots[130] = (const void *)hxc_l_tmp_load_result_n227;
    if (hxc_l_tmp_load_result_n227 == NULL)
    {
      abort();
    }
    int32_t hxc_l_tmp_class_field_load_result_n228 = (*hxc_l_tmp_load_result_n227).hxc_enemyId;
    struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation hxc_l_tmp_call_result_n230 = hxc_caxecraft_app_CaxecraftApp_observeActorPhase(hxc_l_tmp_load_result_n226, hxc_l_tmp_class_field_load_result_n228, hxc_caxecraft_domain_ActorControllerPhase_Resting);
    struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation hxc_l_enemyPhase_ha225e19c84ac = hxc_l_tmp_call_result_n230;
    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n231 = hxc_l_initialSession;
    hxc_l_gc_roots[131] = (const void *)hxc_l_tmp_load_result_n231;
    struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n232 = hxc_l_initialLevel;
    hxc_l_gc_roots[132] = (const void *)hxc_l_tmp_load_result_n232;
    bool hxc_l_tmp_call_result_n234 = hxc_caxecraft_app_CaxecraftApp_dialogueActorsAreValid(hxc_l_tmp_load_result_n231, hxc_l_tmp_load_result_n232, hxc_l_initialActorPhases);
    bool hxc_l_tmp_short_circuit_result_n98 = !hxc_l_tmp_call_result_n234;
    if (!!hxc_l_tmp_call_result_n234)
    {
      hxc_l_tmp_short_circuit_result_n98 = hxc_l_enemyActor.hxc_id <= 0;
    }
    bool hxc_l_tmp_short_circuit_load_result_n236 = hxc_l_tmp_short_circuit_result_n98;
    bool hxc_l_tmp_short_circuit_result_n99 = hxc_l_tmp_short_circuit_load_result_n236;
    if (!hxc_l_tmp_short_circuit_load_result_n236)
    {
      hxc_l_tmp_short_circuit_result_n99 = !hxc_l_enemyPhase_ha225e19c84ac.hxc_valid;
    }
    if (hxc_l_tmp_short_circuit_result_n99)
    {
      if (hxc_array_ref_release(hxc_l_initialActorPhases) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_enum_2c0e695d_destroy(&hxc_l_tmp_static_call_argument_0_owner_n94);
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
      if (hxc_array_ref_release(hxc_l_tmp_object_storage_n84.hxc_stateStartTicks) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_l_tmp_object_storage_n84.hxc_stateIds) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_l_tmp_object_storage_n84.hxc_objectIds) != HXC_STATUS_OK)
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
    struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n241 = hxc_l_initialLevel;
    hxc_l_gc_roots[133] = (const void *)hxc_l_tmp_load_result_n241;
    if (hxc_l_tmp_load_result_n241 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_scenario_ScenarioTransform hxc_l_tmp_class_field_load_result_n242 = (*hxc_l_tmp_load_result_n241).hxc_playerSpawn;
    struct hxc_caxecraft_app_RenderPosition hxc_l_tmp_call_result_n243 = hxc_caxecraft_app_SpawnCameraHeading_headingForSpawn(hxc_l_tmp_class_field_load_result_n242);
    struct hxc_caxecraft_app_RenderPosition hxc_l_initialHeading = hxc_l_tmp_call_result_n243;
    double hxc_l_lookX = hxc_l_initialHeading.hxc_x;
    double hxc_l_lookY = hxc_l_initialHeading.hxc_y;
    double hxc_l_lookZ = hxc_l_initialHeading.hxc_z;
    double hxc_l_accumulator = 0.0;
    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n247 = hxc_l_initialSession;
    hxc_l_gc_roots[134] = (const void *)hxc_l_tmp_load_result_n247;
    if (hxc_l_tmp_load_result_n247 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_domain_GameView hxc_l_tmp_instance_call_result_n248 = hxc_caxecraft_domain_GameSession_view(hxc_l_tmp_load_result_n247);
    struct hxc_caxecraft_domain_GameView hxc_l_initialPresentation = hxc_l_tmp_instance_call_result_n248;
    if (!hxc_l_initialPresentation.hxc_valid)
    {
      if (hxc_array_ref_release(hxc_l_initialActorPhases) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_enum_2c0e695d_destroy(&hxc_l_tmp_static_call_argument_0_owner_n94);
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
      if (hxc_array_ref_release(hxc_l_tmp_object_storage_n84.hxc_stateStartTicks) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_l_tmp_object_storage_n84.hxc_stateIds) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_l_tmp_object_storage_n84.hxc_objectIds) != HXC_STATUS_OK)
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
    struct hxc_caxecraft_app_MotionHistory hxc_l_tmp_call_result_n251 = hxc_caxecraft_app_MotionInterpolation_start(hxc_l_initialPresentation.hxc_localPlayer.hxc_body);
    struct hxc_caxecraft_app_MotionHistory hxc_l_motionHistory = hxc_l_tmp_call_result_n251;
    hxc_l_gc_roots[135] = (const void *)NULL;
    struct hxc_caxecraft_app_PreparedCampaignPortal *hxc_l_preparedPortal = NULL;
    hxc_l_gc_roots[136] = (const void *)hxc_l_campaign;
    struct hxc_caxecraft_content_CampaignManifest *hxc_l_initialCampaign = hxc_l_campaign;
    hxc_l_gc_roots[137] = (const void *)hxc_l_campaignLevel;
    struct hxc_caxecraft_content_CampaignLevel *hxc_l_initialCampaignLevel = hxc_l_campaignLevel;
    struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n254 = hxc_l_initialCampaign;
    hxc_l_gc_roots[138] = (const void *)hxc_l_tmp_load_result_n254;
    hxc_l_gc_roots[139] = (const void *)NULL;
    bool hxc_l_tmp_short_circuit_result_n112 = hxc_l_tmp_load_result_n254 != NULL;
    if (hxc_l_tmp_load_result_n254 != NULL)
    {
      hxc_l_gc_roots[140] = (const void *)hxc_l_initialCampaignLevel;
      hxc_l_gc_roots[141] = (const void *)NULL;
      hxc_l_tmp_short_circuit_result_n112 = hxc_l_initialCampaignLevel != NULL;
    }
    if (hxc_l_tmp_short_circuit_result_n112)
    {
      int32_t hxc_l_tmp_record_field_load_result_n257 = hxc_l_initialPresentation.hxc_localPlayer.hxc_id;
      int32_t hxc_l_tmp_record_field_load_result_n258 = hxc_l_initialPresentation.hxc_localPlayer.hxc_vitals.hxc_health;
      struct hxc_caxecraft_content_LevelPlayerOptions hxc_l_portalPlayer = (struct hxc_caxecraft_content_LevelPlayerOptions){ .hxc_aquaticProfile = hxc_l_initialPresentation.hxc_localPlayer.hxc_aquaticProfile, .hxc_entityId = hxc_l_tmp_record_field_load_result_n257, .hxc_initialHealth = hxc_l_tmp_record_field_load_result_n258 };
      struct hxc_caxecraft_content_ContentPackageStore *hxc_l_tmp_load_result_n261 = hxc_l_contentStore;
      hxc_l_gc_roots[142] = (const void *)hxc_l_tmp_load_result_n261;
      struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n262 = hxc_l_initialCampaign;
      hxc_l_gc_roots[143] = (const void *)hxc_l_tmp_load_result_n262;
      struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_load_result_n263 = hxc_l_initialCampaignLevel;
      hxc_l_gc_roots[144] = (const void *)hxc_l_tmp_load_result_n263;
      struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n264 = hxc_l_initialLevel;
      hxc_l_gc_roots[145] = (const void *)hxc_l_tmp_load_result_n264;
      struct hxc_caxecraft_content_ContentGenerationSequence *hxc_l_tmp_load_result_n265 = hxc_l_generationSequence;
      hxc_l_gc_roots[146] = (const void *)hxc_l_tmp_load_result_n265;
      if (hxc_l_tmp_load_result_n265 == NULL)
      {
        abort();
      }
      int32_t hxc_l_tmp_instance_call_result_n266 = hxc_caxecraft_content_ContentGenerationSequence_allocate(hxc_l_tmp_load_result_n265);
      struct hxc_caxecraft_content_LevelPlayerOptions hxc_l_tmp_load_result_n267 = hxc_l_portalPlayer;
      hxc_l_gc_roots[147] = (const void *)(struct hxc_caxecraft_app_CampaignPortalPreloadRequest){ .hxc_campaign = hxc_l_tmp_load_result_n262, .hxc_nextGeneration = hxc_l_tmp_instance_call_result_n266, .hxc_playerOptions = hxc_l_tmp_load_result_n267, .hxc_sourceLevel = hxc_l_tmp_load_result_n263, .hxc_sourceView = hxc_l_tmp_load_result_n264, .hxc_store = hxc_l_tmp_load_result_n261 }.hxc_campaign;
      hxc_l_gc_roots[148] = (const void *)(struct hxc_caxecraft_app_CampaignPortalPreloadRequest){ .hxc_campaign = hxc_l_tmp_load_result_n262, .hxc_nextGeneration = hxc_l_tmp_instance_call_result_n266, .hxc_playerOptions = hxc_l_tmp_load_result_n267, .hxc_sourceLevel = hxc_l_tmp_load_result_n263, .hxc_sourceView = hxc_l_tmp_load_result_n264, .hxc_store = hxc_l_tmp_load_result_n261 }.hxc_sourceLevel;
      hxc_l_gc_roots[149] = (const void *)(struct hxc_caxecraft_app_CampaignPortalPreloadRequest){ .hxc_campaign = hxc_l_tmp_load_result_n262, .hxc_nextGeneration = hxc_l_tmp_instance_call_result_n266, .hxc_playerOptions = hxc_l_tmp_load_result_n267, .hxc_sourceLevel = hxc_l_tmp_load_result_n263, .hxc_sourceView = hxc_l_tmp_load_result_n264, .hxc_store = hxc_l_tmp_load_result_n261 }.hxc_sourceView;
      hxc_l_gc_roots[150] = (const void *)(struct hxc_caxecraft_app_CampaignPortalPreloadRequest){ .hxc_campaign = hxc_l_tmp_load_result_n262, .hxc_nextGeneration = hxc_l_tmp_instance_call_result_n266, .hxc_playerOptions = hxc_l_tmp_load_result_n267, .hxc_sourceLevel = hxc_l_tmp_load_result_n263, .hxc_sourceView = hxc_l_tmp_load_result_n264, .hxc_store = hxc_l_tmp_load_result_n261 }.hxc_store;
      struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_tmp_load_result_n269 = hxc_l_contentRegistry;
      hxc_l_gc_roots[151] = (const void *)hxc_l_tmp_load_result_n269;
      if (hxc_l_tmp_load_result_n269 == NULL)
      {
        abort();
      }
      struct hxc_compiler_interface_dispatch_caxecraft_scenario_ScenarioContentRegistry_value hxc_l_tmp_interface_value_n270 = (struct hxc_compiler_interface_dispatch_caxecraft_scenario_ScenarioContentRegistry_value){ .object = hxc_l_tmp_load_result_n269, .table = &hxc_itable_compiler_interface_dispatch_caxecraft_content_RuntimeContentRegistry_itable_layout_caxecraf_hb3f384641e1a61bf };
      struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_tmp_load_result_n271 = hxc_l_contentRegistry;
      hxc_l_gc_roots[152] = (const void *)hxc_l_tmp_load_result_n271;
      if (hxc_l_tmp_load_result_n271 == NULL)
      {
        abort();
      }
      struct hxc_compiler_interface_dispatch_caxecraft_content_LevelContentResolver_value hxc_l_tmp_interface_value_n272 = (struct hxc_compiler_interface_dispatch_caxecraft_content_LevelContentResolver_value){ .object = hxc_l_tmp_load_result_n271, .table = &hxc_itable_compiler_interface_dispatch_caxecraft_content_RuntimeContentRegistry_itable_layout_caxecraf_h82ca4f24ca7a7b5a };
      struct hxc_caxecraft_app_CampaignPortalPreloadResult hxc_l_tmp_call_result_n273 = hxc_caxecraft_app_CampaignPortal_preloadCampaignPortal((struct hxc_caxecraft_app_CampaignPortalPreloadRequest){ .hxc_campaign = hxc_l_tmp_load_result_n262, .hxc_nextGeneration = hxc_l_tmp_instance_call_result_n266, .hxc_playerOptions = hxc_l_tmp_load_result_n267, .hxc_sourceLevel = hxc_l_tmp_load_result_n263, .hxc_sourceView = hxc_l_tmp_load_result_n264, .hxc_store = hxc_l_tmp_load_result_n261 }, hxc_l_tmp_interface_value_n270, hxc_l_tmp_interface_value_n272);
      hxc_l_gc_roots[153] = hxc_l_tmp_call_result_n273.hxc_tag == hxc_caxecraft_app_CampaignPortalPreloadResult_CampaignPortalReady ? (const void *)hxc_l_tmp_call_result_n273.hxc_payload.hxc_CampaignPortalReady.hxc_portal : NULL;
      hxc_l_g_h20d6048e709c = hxc_l_tmp_call_result_n273;
      hxc_l_gc_roots[154] = hxc_l_g_h20d6048e709c.hxc_tag == hxc_caxecraft_app_CampaignPortalPreloadResult_CampaignPortalReady ? (const void *)hxc_l_g_h20d6048e709c.hxc_payload.hxc_CampaignPortalReady.hxc_portal : NULL;
      switch (hxc_l_g_h20d6048e709c.hxc_tag) {
        case hxc_caxecraft_app_CampaignPortalPreloadResult_CampaignPortalNotDeclared:
          {
            break;
          }
        case hxc_caxecraft_app_CampaignPortalPreloadResult_CampaignPortalReady:
          {
            hxc_l_gc_roots[155] = hxc_l_g_h20d6048e709c.hxc_tag == hxc_caxecraft_app_CampaignPortalPreloadResult_CampaignPortalReady ? (const void *)hxc_l_g_h20d6048e709c.hxc_payload.hxc_CampaignPortalReady.hxc_portal : NULL;
            if (hxc_l_g_h20d6048e709c.hxc_tag != hxc_caxecraft_app_CampaignPortalPreloadResult_CampaignPortalReady)
            {
              abort();
            }
            struct hxc_caxecraft_app_PreparedCampaignPortal *hxc_l_tmp_enum_payload_project_n276 = hxc_l_g_h20d6048e709c.hxc_payload.hxc_CampaignPortalReady.hxc_portal;
            hxc_l_gc_roots[156] = (const void *)hxc_l_tmp_enum_payload_project_n276;
            struct hxc_caxecraft_app_PreparedCampaignPortal *hxc_l_portal_hda9058943127 = hxc_l_tmp_enum_payload_project_n276;
            hxc_l_gc_roots[157] = (const void *)hxc_l_portal_hda9058943127;
            hxc_l_preparedPortal = hxc_l_portal_hda9058943127;
            struct hxc_caxecraft_app_PreparedCampaignPortal *hxc_l_tmp_load_result_n278 = hxc_l_portal_hda9058943127;
            hxc_l_gc_roots[158] = (const void *)hxc_l_tmp_load_result_n278;
            if (hxc_l_tmp_load_result_n278 == NULL)
            {
              abort();
            }
            struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_class_field_load_result_n279 = (*hxc_l_tmp_load_result_n278).hxc_destinationValue;
            hxc_l_gc_roots[159] = (const void *)hxc_l_tmp_class_field_load_result_n279;
            if (hxc_l_tmp_class_field_load_result_n279 == NULL)
            {
              abort();
            }
            hxc_string hxc_l_tmp_class_field_load_result_n280 = (*hxc_l_tmp_class_field_load_result_n279).hxc_id;
            hxc_string hxc_l_tmp_string_concat_result_n281 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
            if (hxc_string_concat_ref((hxc_string){ (const uint8_t *)"caxecraft: campaign-portal=", 27, true, NULL }, hxc_l_tmp_class_field_load_result_n280, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n281) != HXC_STATUS_OK)
            {
              abort();
            }
            hxc_l_tmp_sys_println_string_argument_owner_n116 = hxc_l_tmp_string_concat_result_n281;
            if (hxc_io_println(hxc_l_tmp_sys_println_string_argument_owner_n116) != HXC_STATUS_OK)
            {
              if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n116) != HXC_STATUS_OK)
              {
                abort();
              }
              hxc_enum_9b450cfc_destroy(&hxc_l_g_h20d6048e709c);
              if (hxc_array_ref_release(hxc_l_initialActorPhases) != HXC_STATUS_OK)
              {
                abort();
              }
              hxc_enum_2c0e695d_destroy(&hxc_l_tmp_static_call_argument_0_owner_n94);
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
              if (hxc_array_ref_release(hxc_l_tmp_object_storage_n84.hxc_stateStartTicks) != HXC_STATUS_OK)
              {
                abort();
              }
              if (hxc_array_ref_release(hxc_l_tmp_object_storage_n84.hxc_stateIds) != HXC_STATUS_OK)
              {
                abort();
              }
              if (hxc_array_ref_release(hxc_l_tmp_object_storage_n84.hxc_objectIds) != HXC_STATUS_OK)
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
            if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n116) != HXC_STATUS_OK)
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
      hxc_enum_9b450cfc_destroy(&hxc_l_g_h20d6048e709c);
    }
    bool hxc_l_jumpQueued = false;
    bool hxc_l_swordQueued = false;
    int32_t hxc_l_selectedMode = 0;
    int32_t hxc_l_locale = 0;
    bool hxc_l_showInitialTitle = true;
    enum hxc_caxecraft_app_AppScreen hxc_l_tmp_call_result_n284 = hxc_caxecraft_app_AppScreen_initialScreen(hxc_l_showInitialTitle);
    enum hxc_caxecraft_app_AppScreen hxc_l_screen = hxc_l_tmp_call_result_n284;
    bool hxc_l_tmp_call_result_n286 = hxc_caxecraft_app_AppScreen_capturesPointer(hxc_l_screen);
    if (hxc_l_tmp_call_result_n286)
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
      bool hxc_l_tmp_load_result_n287 = hxc_l_quit;
      bool hxc_l_tmp_short_circuit_result_n137 = !hxc_l_tmp_load_result_n287;
      if (!hxc_l_tmp_load_result_n287)
      {
        bool hxc_l_tmp_native_call_result_n288 = WindowShouldClose();
        hxc_l_tmp_short_circuit_result_n137 = !hxc_l_tmp_native_call_result_n288;
      }
      if (!hxc_l_tmp_short_circuit_result_n137)
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
        hxc_l_run_h2b932852df37 = hxc_l_activeEditorTestPlay;
        if (hxc_optional_9d9d283f_retain(&hxc_l_run_h2b932852df37) != HXC_STATUS_OK)
        {
          abort();
        }
        struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun hxc_l_tmp_load_result_n293 = hxc_l_run_h2b932852df37;
        hxc_l_gc_roots[175] = hxc_l_tmp_load_result_n293.hxc_has_value ? (const void *)hxc_l_tmp_load_result_n293.hxc_value.hxc_runtime : NULL;
        hxc_l_gc_roots[176] = hxc_l_tmp_load_result_n293.hxc_has_value ? (const void *)hxc_l_tmp_load_result_n293.hxc_value.hxc_snapshot.hxc_campaignLevel : NULL;
        hxc_l_gc_roots[177] = hxc_l_tmp_load_result_n293.hxc_has_value ? (const void *)hxc_l_tmp_load_result_n293.hxc_value.hxc_snapshot.hxc_pendingCampaignLevel : NULL;
        hxc_l_gc_roots[178] = hxc_l_tmp_load_result_n293.hxc_has_value ? (const void *)hxc_l_tmp_load_result_n293.hxc_value.hxc_snapshot.hxc_pendingCampaignPortal : NULL;
        hxc_l_gc_roots[179] = hxc_l_tmp_load_result_n293.hxc_has_value ? (const void *)hxc_l_tmp_load_result_n293.hxc_value.hxc_snapshot.hxc_preparedPortal : NULL;
        bool hxc_l_tmp_short_circuit_result_n139 = !hxc_l_tmp_load_result_n293.hxc_has_value;
        if (!!hxc_l_tmp_load_result_n293.hxc_has_value)
        {
          struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun hxc_l_tmp_load_result_n294 = hxc_l_run_h2b932852df37;
          hxc_l_gc_roots[185] = hxc_l_tmp_load_result_n294.hxc_has_value ? (const void *)hxc_l_tmp_load_result_n294.hxc_value.hxc_runtime : NULL;
          hxc_l_gc_roots[186] = hxc_l_tmp_load_result_n294.hxc_has_value ? (const void *)hxc_l_tmp_load_result_n294.hxc_value.hxc_snapshot.hxc_campaignLevel : NULL;
          hxc_l_gc_roots[187] = hxc_l_tmp_load_result_n294.hxc_has_value ? (const void *)hxc_l_tmp_load_result_n294.hxc_value.hxc_snapshot.hxc_pendingCampaignLevel : NULL;
          hxc_l_gc_roots[188] = hxc_l_tmp_load_result_n294.hxc_has_value ? (const void *)hxc_l_tmp_load_result_n294.hxc_value.hxc_snapshot.hxc_pendingCampaignPortal : NULL;
          hxc_l_gc_roots[189] = hxc_l_tmp_load_result_n294.hxc_has_value ? (const void *)hxc_l_tmp_load_result_n294.hxc_value.hxc_snapshot.hxc_preparedPortal : NULL;
          if (!hxc_l_tmp_load_result_n294.hxc_has_value)
          {
            abort();
          }
          hxc_l_gc_roots[190] = (const void *)hxc_l_tmp_load_result_n294.hxc_value.hxc_runtime;
          hxc_l_gc_roots[191] = (const void *)hxc_l_tmp_load_result_n294.hxc_value.hxc_snapshot.hxc_campaignLevel;
          hxc_l_gc_roots[192] = (const void *)hxc_l_tmp_load_result_n294.hxc_value.hxc_snapshot.hxc_pendingCampaignLevel;
          hxc_l_gc_roots[193] = (const void *)hxc_l_tmp_load_result_n294.hxc_value.hxc_snapshot.hxc_pendingCampaignPortal;
          hxc_l_gc_roots[194] = (const void *)hxc_l_tmp_load_result_n294.hxc_value.hxc_snapshot.hxc_preparedPortal;
          struct hxc_caxecraft_app_EditorTestPlayRuntime *hxc_l_tmp_optional_record_field_project_n296 = hxc_l_tmp_load_result_n294.hxc_value.hxc_runtime;
          hxc_l_gc_roots[195] = (const void *)hxc_l_tmp_optional_record_field_project_n296;
          if (hxc_l_tmp_optional_record_field_project_n296 == NULL)
          {
            abort();
          }
          struct hxc_caxecraft_app_ActivePlayableLevel *hxc_l_tmp_class_field_load_result_n297 = (*hxc_l_tmp_optional_record_field_project_n296).hxc_active;
          hxc_l_gc_roots[196] = (const void *)hxc_l_tmp_class_field_load_result_n297;
          hxc_l_gc_roots[197] = (const void *)NULL;
          hxc_l_tmp_short_circuit_result_n139 = hxc_l_tmp_class_field_load_result_n297 == NULL;
        }
        if (hxc_l_tmp_short_circuit_result_n139)
        {
          if (hxc_io_println((hxc_string){ (const uint8_t *)"caxecraft: editor Test Play lost its disposable runtime", 55, true, NULL }) != HXC_STATUS_OK)
          {
            abort();
          }
          hxc_l_gc_roots[198] = hxc_l_run_h2b932852df37.hxc_has_value ? (const void *)hxc_l_run_h2b932852df37.hxc_value.hxc_runtime : NULL;
          hxc_l_gc_roots[199] = hxc_l_run_h2b932852df37.hxc_has_value ? (const void *)hxc_l_run_h2b932852df37.hxc_value.hxc_snapshot.hxc_campaignLevel : NULL;
          hxc_l_gc_roots[200] = hxc_l_run_h2b932852df37.hxc_has_value ? (const void *)hxc_l_run_h2b932852df37.hxc_value.hxc_snapshot.hxc_pendingCampaignLevel : NULL;
          hxc_l_gc_roots[201] = hxc_l_run_h2b932852df37.hxc_has_value ? (const void *)hxc_l_run_h2b932852df37.hxc_value.hxc_snapshot.hxc_pendingCampaignPortal : NULL;
          hxc_l_gc_roots[202] = hxc_l_run_h2b932852df37.hxc_has_value ? (const void *)hxc_l_run_h2b932852df37.hxc_value.hxc_snapshot.hxc_preparedPortal : NULL;
          if (hxc_l_run_h2b932852df37.hxc_has_value)
          {
            struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun hxc_l_tmp_load_result_n300 = hxc_l_run_h2b932852df37;
            hxc_l_gc_roots[203] = hxc_l_tmp_load_result_n300.hxc_has_value ? (const void *)hxc_l_tmp_load_result_n300.hxc_value.hxc_runtime : NULL;
            hxc_l_gc_roots[204] = hxc_l_tmp_load_result_n300.hxc_has_value ? (const void *)hxc_l_tmp_load_result_n300.hxc_value.hxc_snapshot.hxc_campaignLevel : NULL;
            hxc_l_gc_roots[205] = hxc_l_tmp_load_result_n300.hxc_has_value ? (const void *)hxc_l_tmp_load_result_n300.hxc_value.hxc_snapshot.hxc_pendingCampaignLevel : NULL;
            hxc_l_gc_roots[206] = hxc_l_tmp_load_result_n300.hxc_has_value ? (const void *)hxc_l_tmp_load_result_n300.hxc_value.hxc_snapshot.hxc_pendingCampaignPortal : NULL;
            hxc_l_gc_roots[207] = hxc_l_tmp_load_result_n300.hxc_has_value ? (const void *)hxc_l_tmp_load_result_n300.hxc_value.hxc_snapshot.hxc_preparedPortal : NULL;
            if (!hxc_l_tmp_load_result_n300.hxc_has_value)
            {
              abort();
            }
            hxc_l_gc_roots[208] = (const void *)hxc_l_tmp_load_result_n300.hxc_value.hxc_runtime;
            hxc_l_gc_roots[209] = (const void *)hxc_l_tmp_load_result_n300.hxc_value.hxc_snapshot.hxc_campaignLevel;
            hxc_l_gc_roots[210] = (const void *)hxc_l_tmp_load_result_n300.hxc_value.hxc_snapshot.hxc_pendingCampaignLevel;
            hxc_l_gc_roots[211] = (const void *)hxc_l_tmp_load_result_n300.hxc_value.hxc_snapshot.hxc_pendingCampaignPortal;
            hxc_l_gc_roots[212] = (const void *)hxc_l_tmp_load_result_n300.hxc_value.hxc_snapshot.hxc_preparedPortal;
            struct hxc_caxecraft_app_EditorTestPlayRuntime *hxc_l_tmp_optional_record_field_project_n302 = hxc_l_tmp_load_result_n300.hxc_value.hxc_runtime;
            hxc_l_gc_roots[213] = (const void *)hxc_l_tmp_optional_record_field_project_n302;
            if (hxc_l_tmp_optional_record_field_project_n302 == NULL)
            {
              abort();
            }
            hxc_caxecraft_app_EditorTestPlayRuntime_stop(hxc_l_tmp_optional_record_field_project_n302);
          }
          hxc_l_gc_roots[214] = (struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = false }.hxc_has_value ? (const void *)(struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = false }.hxc_value.hxc_runtime : NULL;
          hxc_l_gc_roots[215] = (struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = false }.hxc_has_value ? (const void *)(struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = false }.hxc_value.hxc_snapshot.hxc_campaignLevel : NULL;
          hxc_l_gc_roots[216] = (struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = false }.hxc_has_value ? (const void *)(struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = false }.hxc_value.hxc_snapshot.hxc_pendingCampaignLevel : NULL;
          hxc_l_gc_roots[217] = (struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = false }.hxc_has_value ? (const void *)(struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = false }.hxc_value.hxc_snapshot.hxc_pendingCampaignPortal : NULL;
          hxc_l_gc_roots[218] = (struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = false }.hxc_has_value ? (const void *)(struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = false }.hxc_value.hxc_snapshot.hxc_preparedPortal : NULL;
          hxc_optional_9d9d283f_destroy(&hxc_l_activeEditorTestPlay);
          hxc_l_activeEditorTestPlay = (struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = false };
          struct hxc_caxecraft_app_CaxecraftEditorScreen *hxc_l_tmp_load_result_n303 = hxc_l_editorScreen;
          hxc_l_gc_roots[219] = (const void *)hxc_l_tmp_load_result_n303;
          if (hxc_l_tmp_load_result_n303 == NULL)
          {
            abort();
          }
          hxc_caxecraft_app_CaxecraftEditorScreen_finishTestPlay(hxc_l_tmp_load_result_n303, false);
          hxc_l_quit = true;
          hxc_optional_9d9d283f_destroy(&hxc_l_run_h2b932852df37);
          break;
        }
        else
        {
          hxc_optional_9d9d283f_destroy(&hxc_l_run_h2b932852df37);
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
          struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun hxc_l_tmp_load_result_n305 = hxc_l_activeEditorTestPlay;
          hxc_l_gc_roots[220] = hxc_l_tmp_load_result_n305.hxc_has_value ? (const void *)hxc_l_tmp_load_result_n305.hxc_value.hxc_runtime : NULL;
          hxc_l_gc_roots[221] = hxc_l_tmp_load_result_n305.hxc_has_value ? (const void *)hxc_l_tmp_load_result_n305.hxc_value.hxc_snapshot.hxc_campaignLevel : NULL;
          hxc_l_gc_roots[222] = hxc_l_tmp_load_result_n305.hxc_has_value ? (const void *)hxc_l_tmp_load_result_n305.hxc_value.hxc_snapshot.hxc_pendingCampaignLevel : NULL;
          hxc_l_gc_roots[223] = hxc_l_tmp_load_result_n305.hxc_has_value ? (const void *)hxc_l_tmp_load_result_n305.hxc_value.hxc_snapshot.hxc_pendingCampaignPortal : NULL;
          hxc_l_gc_roots[224] = hxc_l_tmp_load_result_n305.hxc_has_value ? (const void *)hxc_l_tmp_load_result_n305.hxc_value.hxc_snapshot.hxc_preparedPortal : NULL;
          if (!hxc_l_tmp_load_result_n305.hxc_has_value)
          {
            abort();
          }
          hxc_l_gc_roots[225] = (const void *)hxc_l_tmp_load_result_n305.hxc_value.hxc_runtime;
          hxc_l_gc_roots[226] = (const void *)hxc_l_tmp_load_result_n305.hxc_value.hxc_snapshot.hxc_campaignLevel;
          hxc_l_gc_roots[227] = (const void *)hxc_l_tmp_load_result_n305.hxc_value.hxc_snapshot.hxc_pendingCampaignLevel;
          hxc_l_gc_roots[228] = (const void *)hxc_l_tmp_load_result_n305.hxc_value.hxc_snapshot.hxc_pendingCampaignPortal;
          hxc_l_gc_roots[229] = (const void *)hxc_l_tmp_load_result_n305.hxc_value.hxc_snapshot.hxc_preparedPortal;
          struct hxc_caxecraft_app_EditorTestPlayRuntime *hxc_l_tmp_optional_record_field_project_n307 = hxc_l_tmp_load_result_n305.hxc_value.hxc_runtime;
          hxc_l_gc_roots[230] = (const void *)hxc_l_tmp_optional_record_field_project_n307;
          if (hxc_l_tmp_optional_record_field_project_n307 == NULL)
          {
            abort();
          }
          hxc_caxecraft_app_EditorTestPlayRuntime_stop(hxc_l_tmp_optional_record_field_project_n307);
          hxc_l_gc_roots[231] = (struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = false }.hxc_has_value ? (const void *)(struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = false }.hxc_value.hxc_runtime : NULL;
          hxc_l_gc_roots[232] = (struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = false }.hxc_has_value ? (const void *)(struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = false }.hxc_value.hxc_snapshot.hxc_campaignLevel : NULL;
          hxc_l_gc_roots[233] = (struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = false }.hxc_has_value ? (const void *)(struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = false }.hxc_value.hxc_snapshot.hxc_pendingCampaignLevel : NULL;
          hxc_l_gc_roots[234] = (struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = false }.hxc_has_value ? (const void *)(struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = false }.hxc_value.hxc_snapshot.hxc_pendingCampaignPortal : NULL;
          hxc_l_gc_roots[235] = (struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = false }.hxc_has_value ? (const void *)(struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = false }.hxc_value.hxc_snapshot.hxc_preparedPortal : NULL;
          hxc_optional_9d9d283f_destroy(&hxc_l_activeEditorTestPlay);
          hxc_l_activeEditorTestPlay = (struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = false };
          struct hxc_caxecraft_app_CaxecraftEditorScreen *hxc_l_tmp_load_result_n308 = hxc_l_editorScreen;
          hxc_l_gc_roots[236] = (const void *)hxc_l_tmp_load_result_n308;
          if (hxc_l_tmp_load_result_n308 == NULL)
          {
            abort();
          }
          hxc_caxecraft_app_CaxecraftEditorScreen_finishTestPlay(hxc_l_tmp_load_result_n308, false);
          hxc_l_quit = true;
          break;
        }
      }
      bool hxc_l_tmp_call_result_n310 = hxc_caxecraft_app_AppScreen_showsLoading(hxc_l_screen);
      bool hxc_l_loadingAtFrameStart = hxc_l_tmp_call_result_n310;
      hxc_l_gc_roots[237] = (const void *)hxc_l_activeLevel;
      struct hxc_caxecraft_app_ActivePlayableLevel *hxc_l_frameLevelOwner = hxc_l_activeLevel;
      if (hxc_l_screen == hxc_caxecraft_app_AppScreen_EditorTestPlay)
      {
        hxc_l_gc_roots[238] = hxc_l_activeEditorTestPlay.hxc_has_value ? (const void *)hxc_l_activeEditorTestPlay.hxc_value.hxc_runtime : NULL;
        hxc_l_gc_roots[239] = hxc_l_activeEditorTestPlay.hxc_has_value ? (const void *)hxc_l_activeEditorTestPlay.hxc_value.hxc_snapshot.hxc_campaignLevel : NULL;
        hxc_l_gc_roots[240] = hxc_l_activeEditorTestPlay.hxc_has_value ? (const void *)hxc_l_activeEditorTestPlay.hxc_value.hxc_snapshot.hxc_pendingCampaignLevel : NULL;
        hxc_l_gc_roots[241] = hxc_l_activeEditorTestPlay.hxc_has_value ? (const void *)hxc_l_activeEditorTestPlay.hxc_value.hxc_snapshot.hxc_pendingCampaignPortal : NULL;
        hxc_l_gc_roots[242] = hxc_l_activeEditorTestPlay.hxc_has_value ? (const void *)hxc_l_activeEditorTestPlay.hxc_value.hxc_snapshot.hxc_preparedPortal : NULL;
        hxc_l_run_h01967e139705 = hxc_l_activeEditorTestPlay;
        if (hxc_optional_9d9d283f_retain(&hxc_l_run_h01967e139705) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_l_gc_roots[243] = hxc_l_run_h01967e139705.hxc_has_value ? (const void *)hxc_l_run_h01967e139705.hxc_value.hxc_runtime : NULL;
        hxc_l_gc_roots[244] = hxc_l_run_h01967e139705.hxc_has_value ? (const void *)hxc_l_run_h01967e139705.hxc_value.hxc_snapshot.hxc_campaignLevel : NULL;
        hxc_l_gc_roots[245] = hxc_l_run_h01967e139705.hxc_has_value ? (const void *)hxc_l_run_h01967e139705.hxc_value.hxc_snapshot.hxc_pendingCampaignLevel : NULL;
        hxc_l_gc_roots[246] = hxc_l_run_h01967e139705.hxc_has_value ? (const void *)hxc_l_run_h01967e139705.hxc_value.hxc_snapshot.hxc_pendingCampaignPortal : NULL;
        hxc_l_gc_roots[247] = hxc_l_run_h01967e139705.hxc_has_value ? (const void *)hxc_l_run_h01967e139705.hxc_value.hxc_snapshot.hxc_preparedPortal : NULL;
        if (hxc_l_run_h01967e139705.hxc_has_value)
        {
          struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun hxc_l_tmp_load_result_n316 = hxc_l_run_h01967e139705;
          hxc_l_gc_roots[255] = hxc_l_tmp_load_result_n316.hxc_has_value ? (const void *)hxc_l_tmp_load_result_n316.hxc_value.hxc_runtime : NULL;
          hxc_l_gc_roots[256] = hxc_l_tmp_load_result_n316.hxc_has_value ? (const void *)hxc_l_tmp_load_result_n316.hxc_value.hxc_snapshot.hxc_campaignLevel : NULL;
          hxc_l_gc_roots[257] = hxc_l_tmp_load_result_n316.hxc_has_value ? (const void *)hxc_l_tmp_load_result_n316.hxc_value.hxc_snapshot.hxc_pendingCampaignLevel : NULL;
          hxc_l_gc_roots[258] = hxc_l_tmp_load_result_n316.hxc_has_value ? (const void *)hxc_l_tmp_load_result_n316.hxc_value.hxc_snapshot.hxc_pendingCampaignPortal : NULL;
          hxc_l_gc_roots[259] = hxc_l_tmp_load_result_n316.hxc_has_value ? (const void *)hxc_l_tmp_load_result_n316.hxc_value.hxc_snapshot.hxc_preparedPortal : NULL;
          if (!hxc_l_tmp_load_result_n316.hxc_has_value)
          {
            abort();
          }
          hxc_l_gc_roots[260] = (const void *)hxc_l_tmp_load_result_n316.hxc_value.hxc_runtime;
          hxc_l_gc_roots[261] = (const void *)hxc_l_tmp_load_result_n316.hxc_value.hxc_snapshot.hxc_campaignLevel;
          hxc_l_gc_roots[262] = (const void *)hxc_l_tmp_load_result_n316.hxc_value.hxc_snapshot.hxc_pendingCampaignLevel;
          hxc_l_gc_roots[263] = (const void *)hxc_l_tmp_load_result_n316.hxc_value.hxc_snapshot.hxc_pendingCampaignPortal;
          hxc_l_gc_roots[264] = (const void *)hxc_l_tmp_load_result_n316.hxc_value.hxc_snapshot.hxc_preparedPortal;
          struct hxc_caxecraft_app_EditorTestPlayRuntime *hxc_l_tmp_optional_record_field_project_n318 = hxc_l_tmp_load_result_n316.hxc_value.hxc_runtime;
          hxc_l_gc_roots[265] = (const void *)hxc_l_tmp_optional_record_field_project_n318;
          if (hxc_l_tmp_optional_record_field_project_n318 == NULL)
          {
            abort();
          }
          struct hxc_caxecraft_app_ActivePlayableLevel *hxc_l_tmp_class_field_load_result_n319 = (*hxc_l_tmp_optional_record_field_project_n318).hxc_active;
          hxc_l_gc_roots[266] = (const void *)hxc_l_tmp_class_field_load_result_n319;
          struct hxc_caxecraft_app_ActivePlayableLevel *hxc_l_testLevel_h8bcaa44306d5 = hxc_l_tmp_class_field_load_result_n319;
          hxc_l_gc_roots[267] = (const void *)hxc_l_testLevel_h8bcaa44306d5;
          hxc_l_gc_roots[268] = (const void *)NULL;
          if (hxc_l_testLevel_h8bcaa44306d5 != NULL)
          {
            hxc_l_gc_roots[269] = (const void *)hxc_l_testLevel_h8bcaa44306d5;
            hxc_l_frameLevelOwner = hxc_l_testLevel_h8bcaa44306d5;
          }
        }
        hxc_optional_9d9d283f_destroy(&hxc_l_run_h01967e139705);
      }
      struct hxc_caxecraft_app_ActivePlayableLevel *hxc_l_tmp_load_result_n322 = hxc_l_frameLevelOwner;
      hxc_l_gc_roots[248] = (const void *)hxc_l_tmp_load_result_n322;
      if (hxc_l_tmp_load_result_n322 == NULL)
      {
        abort();
      }
      struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_class_field_load_result_n323 = (*hxc_l_tmp_load_result_n322).hxc_activeView;
      hxc_l_gc_roots[249] = (const void *)hxc_l_tmp_class_field_load_result_n323;
      struct hxc_caxecraft_app_PlayableLevelView *hxc_l_levelView_hb85a9a956786 = hxc_l_tmp_class_field_load_result_n323;
      struct hxc_caxecraft_app_ActivePlayableLevel *hxc_l_tmp_load_result_n324 = hxc_l_frameLevelOwner;
      hxc_l_gc_roots[250] = (const void *)hxc_l_tmp_load_result_n324;
      if (hxc_l_tmp_load_result_n324 == NULL)
      {
        abort();
      }
      struct hxc_caxecraft_content_ActiveContent *hxc_l_tmp_class_field_load_result_n325 = (*hxc_l_tmp_load_result_n324).hxc_content;
      hxc_l_gc_roots[251] = (const void *)hxc_l_tmp_class_field_load_result_n325;
      if (hxc_l_tmp_class_field_load_result_n325 == NULL)
      {
        abort();
      }
      struct hxc_caxecraft_content_LoadedContentGeneration *hxc_l_tmp_class_field_load_result_n326 = (*hxc_l_tmp_class_field_load_result_n325).hxc_active;
      hxc_l_gc_roots[252] = (const void *)hxc_l_tmp_class_field_load_result_n326;
      if (hxc_l_tmp_class_field_load_result_n326 == NULL)
      {
        abort();
      }
      struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_class_field_load_result_n327 = (*hxc_l_tmp_class_field_load_result_n326).hxc_ownedSession;
      hxc_l_gc_roots[253] = (const void *)hxc_l_tmp_class_field_load_result_n327;
      struct hxc_caxecraft_domain_GameSession *hxc_l_session = hxc_l_tmp_class_field_load_result_n327;
      struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n328 = hxc_l_session;
      hxc_l_gc_roots[254] = (const void *)hxc_l_tmp_load_result_n328;
      if (hxc_l_tmp_load_result_n328 == NULL)
      {
        abort();
      }
      struct hxc_caxecraft_domain_GameView hxc_l_tmp_instance_call_result_n329 = hxc_caxecraft_domain_GameSession_view(hxc_l_tmp_load_result_n328);
      struct hxc_caxecraft_domain_GameView hxc_l_initialView = hxc_l_tmp_instance_call_result_n329;
      if (!hxc_l_initialView.hxc_valid)
      {
        hxc_l_quit = true;
      }
      struct hxc_caxecraft_domain_Character hxc_l_character = hxc_l_initialView.hxc_localPlayer;
      bool hxc_l_recapturedThisFrame = false;
      bool hxc_l_resetMotionThisFrame = false;
      float hxc_l_tmp_native_call_result_n332 = GetFrameTime();
      double hxc_l_frameSeconds = (double)hxc_l_tmp_native_call_result_n332;
      if (hxc_l_frameSeconds > 0.25)
      {
        hxc_l_frameSeconds = 0.25;
      }
      bool hxc_l_tmp_native_call_result_n334 = IsWindowFocused();
      bool hxc_l_focused = hxc_l_tmp_native_call_result_n334;
      bool hxc_l_tmp_call_result_n336 = hxc_caxecraft_app_AppScreen_capturesPointer(hxc_l_screen);
      bool hxc_l_tmp_call_result_n338 = hxc_caxecraft_app_AppScreen_pausesSimulation(hxc_l_screen);
      bool hxc_l_tmp_load_result_n339 = hxc_l_focused;
      struct hxc_caxecraft_app_RaylibGameInputSample hxc_l_tmp_call_result_n341 = hxc_caxecraft_app_RaylibGameInput_sampleGameInput(hxc_l_tmp_call_result_n336, hxc_l_tmp_call_result_n338, hxc_l_tmp_load_result_n339, hxc_l_frameSeconds);
      struct hxc_caxecraft_app_RaylibGameInputSample hxc_l_realInput = hxc_l_tmp_call_result_n341;
      struct hxc_caxecraft_pilot_GameInputFrame hxc_l_frameInput = hxc_l_realInput.hxc_frame;
      int32_t hxc_l_promptDevice = hxc_l_realInput.hxc_promptDevice;
      bool hxc_l_tmp_call_result_n344 = hxc_caxecraft_app_RaylibGameInput_debugHudTogglePressed();
      if (hxc_l_tmp_call_result_n344)
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
      bool hxc_l_tmp_call_result_n365 = hxc_caxecraft_app_AppScreen_capturesPointer(hxc_l_screen);
      bool hxc_l_tmp_call_result_n366 = hxc_caxecraft_app_RaylibGameInput_cameraTogglePressed(hxc_l_tmp_call_result_n365);
      bool hxc_l_cameraTogglePressed = hxc_l_tmp_call_result_n366;
      enum hxc_caxecraft_app_AppScreen hxc_l_tmp_load_result_n367 = hxc_l_screen;
      bool hxc_l_tmp_load_result_n368 = hxc_l_focused;
      bool hxc_l_tmp_call_result_n370 = hxc_caxecraft_app_AppScreen_stopsEditorTestPlay(hxc_l_tmp_load_result_n367, hxc_l_tmp_load_result_n368, hxc_l_pausePressed);
      if (!hxc_l_tmp_call_result_n370)
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
        bool hxc_l_tmp_load_result_n433 = hxc_l_loadingAtFrameStart;
        bool hxc_l_tmp_short_circuit_result_n183 = hxc_l_tmp_load_result_n433;
        if (hxc_l_tmp_load_result_n433)
        {
          hxc_l_tmp_short_circuit_result_n183 = hxc_l_loadingFramePresented;
        }
        if (hxc_l_tmp_short_circuit_result_n183)
        {
          hxc_l_gc_roots[319] = (const void *)hxc_l_pendingCampaignLevel;
          hxc_l_requestedCampaignLevel = hxc_l_pendingCampaignLevel;
        }
        bool hxc_l_requestedFromCampaignMenu = false;
        struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_load_result_n437 = hxc_l_requestedCampaignLevel;
        hxc_l_gc_roots[320] = (const void *)hxc_l_tmp_load_result_n437;
        hxc_l_gc_roots[321] = (const void *)NULL;
        bool hxc_l_tmp_short_circuit_result_n186 = hxc_l_tmp_load_result_n437 != NULL;
        if (hxc_l_tmp_load_result_n437 != NULL)
        {
          hxc_l_tmp_short_circuit_result_n186 = hxc_l_loadingAtFrameStart;
        }
        bool hxc_l_requestedFromAuthoredExit = hxc_l_tmp_short_circuit_result_n186;
        bool hxc_l_tmp_load_result_n440 = hxc_l_quit;
        bool hxc_l_tmp_short_circuit_result_n187 = !hxc_l_tmp_load_result_n440;
        if (!hxc_l_tmp_load_result_n440)
        {
          enum hxc_caxecraft_app_AppScreen hxc_l_tmp_load_result_n441 = hxc_l_screen;
          bool hxc_l_tmp_call_result_n443 = hxc_caxecraft_app_TitleMenuFlow_allowsCampaignTravel(hxc_l_tmp_load_result_n441, hxc_l_selectedMode);
          hxc_l_tmp_short_circuit_result_n187 = hxc_l_tmp_call_result_n443;
        }
        bool hxc_l_tmp_short_circuit_load_result_n444 = hxc_l_tmp_short_circuit_result_n187;
        bool hxc_l_tmp_short_circuit_result_n188 = hxc_l_tmp_short_circuit_load_result_n444;
        if (hxc_l_tmp_short_circuit_load_result_n444)
        {
          hxc_l_tmp_short_circuit_result_n188 = hxc_l_travelPressed;
        }
        if (hxc_l_tmp_short_circuit_result_n188)
        {
          hxc_l_gc_roots[322] = (const void *)hxc_l_campaign;
          struct hxc_caxecraft_content_CampaignManifest *hxc_l_selectedCampaign_h213f86222d73 = hxc_l_campaign;
          hxc_l_gc_roots[323] = (const void *)hxc_l_campaignLevel;
          struct hxc_caxecraft_content_CampaignLevel *hxc_l_sourceLevel_hc367b181e5b2 = hxc_l_campaignLevel;
          struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n449 = hxc_l_selectedCampaign_h213f86222d73;
          hxc_l_gc_roots[324] = (const void *)hxc_l_tmp_load_result_n449;
          hxc_l_gc_roots[325] = (const void *)NULL;
          bool hxc_l_tmp_short_circuit_result_n191 = hxc_l_tmp_load_result_n449 != NULL;
          if (hxc_l_tmp_load_result_n449 != NULL)
          {
            hxc_l_gc_roots[326] = (const void *)hxc_l_sourceLevel_hc367b181e5b2;
            hxc_l_gc_roots[327] = (const void *)NULL;
            hxc_l_tmp_short_circuit_result_n191 = hxc_l_sourceLevel_hc367b181e5b2 != NULL;
          }
          if (hxc_l_tmp_short_circuit_result_n191)
          {
            struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n452 = hxc_l_selectedCampaign_h213f86222d73;
            hxc_l_gc_roots[328] = (const void *)hxc_l_tmp_load_result_n452;
            if (hxc_l_tmp_load_result_n452 == NULL)
            {
              abort();
            }
            struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_load_result_n453 = hxc_l_sourceLevel_hc367b181e5b2;
            hxc_l_gc_roots[329] = (const void *)hxc_l_tmp_load_result_n453;
            if (hxc_l_tmp_load_result_n453 == NULL)
            {
              abort();
            }
            hxc_string hxc_l_tmp_class_field_load_result_n454 = (*hxc_l_tmp_load_result_n453).hxc_id;
            struct hxc_caxecraft_content_CampaignTransition *hxc_l_tmp_instance_call_result_n455 = hxc_caxecraft_content_CampaignManifest_unambiguousTransitionFrom(hxc_l_tmp_load_result_n452, hxc_l_tmp_class_field_load_result_n454);
            hxc_l_gc_roots[330] = (const void *)hxc_l_tmp_instance_call_result_n455;
            struct hxc_caxecraft_content_CampaignTransition *hxc_l_transition_h788d5dd1efb4 = hxc_l_tmp_instance_call_result_n455;
            hxc_l_gc_roots[331] = (const void *)hxc_l_transition_h788d5dd1efb4;
            hxc_l_gc_roots[332] = (const void *)NULL;
            if (hxc_l_transition_h788d5dd1efb4 == NULL)
            {
              struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_load_result_n457 = hxc_l_sourceLevel_hc367b181e5b2;
              hxc_l_gc_roots[333] = (const void *)hxc_l_tmp_load_result_n457;
              if (hxc_l_tmp_load_result_n457 == NULL)
              {
                abort();
              }
              hxc_string hxc_l_tmp_class_field_load_result_n458 = (*hxc_l_tmp_load_result_n457).hxc_id;
              hxc_string hxc_l_tmp_string_concat_result_n459 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
              if (hxc_string_concat_ref((hxc_string){ (const uint8_t *)"caxecraft: campaign travel requires exactly one outgoing transition from ", 73, true, NULL }, hxc_l_tmp_class_field_load_result_n458, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n459) != HXC_STATUS_OK)
              {
                abort();
              }
              hxc_l_tmp_sys_println_string_argument_owner_n193 = hxc_l_tmp_string_concat_result_n459;
              if (hxc_io_println(hxc_l_tmp_sys_println_string_argument_owner_n193) != HXC_STATUS_OK)
              {
                if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n193) != HXC_STATUS_OK)
                {
                  abort();
                }
                if (hxc_array_ref_release(hxc_l_initialActorPhases) != HXC_STATUS_OK)
                {
                  abort();
                }
                hxc_enum_2c0e695d_destroy(&hxc_l_tmp_static_call_argument_0_owner_n94);
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
                if (hxc_array_ref_release(hxc_l_tmp_object_storage_n84.hxc_stateStartTicks) != HXC_STATUS_OK)
                {
                  abort();
                }
                if (hxc_array_ref_release(hxc_l_tmp_object_storage_n84.hxc_stateIds) != HXC_STATUS_OK)
                {
                  abort();
                }
                if (hxc_array_ref_release(hxc_l_tmp_object_storage_n84.hxc_objectIds) != HXC_STATUS_OK)
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
              if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n193) != HXC_STATUS_OK)
              {
                abort();
              }
            }
            else
            {
              struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n461 = hxc_l_selectedCampaign_h213f86222d73;
              hxc_l_gc_roots[334] = (const void *)hxc_l_tmp_load_result_n461;
              if (hxc_l_tmp_load_result_n461 == NULL)
              {
                abort();
              }
              struct hxc_caxecraft_content_CampaignTransition *hxc_l_tmp_load_result_n462 = hxc_l_transition_h788d5dd1efb4;
              hxc_l_gc_roots[335] = (const void *)hxc_l_tmp_load_result_n462;
              if (hxc_l_tmp_load_result_n462 == NULL)
              {
                abort();
              }
              hxc_string hxc_l_tmp_class_field_load_result_n463 = (*hxc_l_tmp_load_result_n462).hxc_destinationLevel;
              struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_instance_call_result_n464 = hxc_caxecraft_content_CampaignManifest_level(hxc_l_tmp_load_result_n461, hxc_l_tmp_class_field_load_result_n463);
              hxc_l_gc_roots[336] = (const void *)hxc_l_tmp_instance_call_result_n464;
              struct hxc_caxecraft_content_CampaignLevel *hxc_l_destination_h5e51f65bffe3 = hxc_l_tmp_instance_call_result_n464;
              hxc_l_gc_roots[337] = (const void *)hxc_l_destination_h5e51f65bffe3;
              hxc_l_gc_roots[338] = (const void *)NULL;
              if (hxc_l_destination_h5e51f65bffe3 == NULL)
              {
                if (hxc_io_println((hxc_string){ (const uint8_t *)"caxecraft: campaign destination disappeared after manifest validation", 69, true, NULL }) != HXC_STATUS_OK)
                {
                  abort();
                }
              }
              else
              {
                hxc_l_gc_roots[339] = (const void *)hxc_l_destination_h5e51f65bffe3;
                hxc_l_requestedCampaignLevel = hxc_l_destination_h5e51f65bffe3;
              }
            }
          }
        }
        bool hxc_l_tmp_call_result_n468 = hxc_caxecraft_app_AppScreen_showsCampaignSelection(hxc_l_screen);
        bool hxc_l_tmp_short_circuit_result_n195 = hxc_l_tmp_call_result_n468;
        if (hxc_l_tmp_call_result_n468)
        {
          hxc_l_tmp_short_circuit_result_n195 = hxc_l_focused;
        }
        if (hxc_l_tmp_short_circuit_result_n195)
        {
          bool hxc_l_campaignLaunchRequested = hxc_l_menuConfirmPressed;
          bool hxc_l_campaignBackRequested = hxc_l_pausePressed;
          bool hxc_l_tmp_load_result_n473 = hxc_l_menuNextPressed;
          bool hxc_l_tmp_short_circuit_result_n198 = hxc_l_tmp_load_result_n473;
          if (hxc_l_tmp_load_result_n473)
          {
            hxc_l_gc_roots[340] = (const void *)hxc_l_campaign;
            hxc_l_gc_roots[341] = (const void *)NULL;
            hxc_l_tmp_short_circuit_result_n198 = hxc_l_campaign != NULL;
          }
          bool hxc_l_tmp_short_circuit_load_result_n475 = hxc_l_tmp_short_circuit_result_n198;
          bool hxc_l_tmp_short_circuit_result_n199 = hxc_l_tmp_short_circuit_load_result_n475;
          if (hxc_l_tmp_short_circuit_load_result_n475)
          {
            struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n476 = hxc_l_campaign;
            hxc_l_gc_roots[342] = (const void *)hxc_l_tmp_load_result_n476;
            if (hxc_l_tmp_load_result_n476 == NULL)
            {
              abort();
            }
            struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n477 = (*hxc_l_tmp_load_result_n476).hxc_admittedLevels;
            hxc_l_gc_roots[343] = (const void *)hxc_l_tmp_class_field_load_result_n477;
            int32_t hxc_l_tmp_array_length_result_n478;
            if (hxc_array_ref_length(hxc_l_tmp_class_field_load_result_n477, &hxc_l_tmp_array_length_result_n478) != HXC_STATUS_OK)
            {
              abort();
            }
            hxc_l_tmp_short_circuit_result_n199 = hxc_l_tmp_array_length_result_n478 > 0;
          }
          if (hxc_l_tmp_short_circuit_result_n199)
          {
            int32_t hxc_l_tmp_load_result_n480 = hxc_l_selectedCampaignLevelIndex;
            struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n481 = hxc_l_campaign;
            hxc_l_gc_roots[344] = (const void *)hxc_l_tmp_load_result_n481;
            if (hxc_l_tmp_load_result_n481 == NULL)
            {
              abort();
            }
            struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n482 = (*hxc_l_tmp_load_result_n481).hxc_admittedLevels;
            hxc_l_gc_roots[345] = (const void *)hxc_l_tmp_class_field_load_result_n482;
            int32_t hxc_l_tmp_array_length_result_n483;
            if (hxc_array_ref_length(hxc_l_tmp_class_field_load_result_n482, &hxc_l_tmp_array_length_result_n483) != HXC_STATUS_OK)
            {
              abort();
            }
            hxc_l_selectedCampaignLevelIndex = hxc_i32_modulo_zero_safe(hxc_i32_add_wrapping(hxc_l_tmp_load_result_n480, 1), hxc_l_tmp_array_length_result_n483);
          }
          bool hxc_l_tmp_native_call_result_n484 = IsKeyPressed((int32_t)76);
          if (hxc_l_tmp_native_call_result_n484)
          {
            struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_tmp_load_result_n485 = hxc_l_uiCatalog;
            hxc_l_gc_roots[346] = (const void *)hxc_l_tmp_load_result_n485;
            if (hxc_l_tmp_load_result_n485 == NULL)
            {
              abort();
            }
            int32_t hxc_l_tmp_instance_call_result_n487 = hxc_caxecraft_localization_RuntimeUiCatalog_nextLocale(hxc_l_tmp_load_result_n485, hxc_l_locale);
            hxc_l_locale = hxc_l_tmp_instance_call_result_n487;
          }
          struct Vector2 hxc_l_tmp_native_call_result_n488 = GetMousePosition();
          struct Vector2 hxc_l_campaignMouse = hxc_l_tmp_native_call_result_n488;
          double hxc_l_tmp_static_call_argument_0_n202 = (double)hxc_l_campaignMouse.x;
          double hxc_l_tmp_static_call_argument_1_n203 = (double)hxc_l_campaignMouse.y;
          int32_t hxc_l_tmp_native_call_result_n493 = GetScreenWidth();
          int32_t hxc_l_tmp_static_call_argument_2_n204 = (int32_t)hxc_l_tmp_native_call_result_n493;
          int32_t hxc_l_tmp_native_call_result_n494 = GetScreenHeight();
          int32_t hxc_l_tmp_static_call_argument_3_n205 = (int32_t)hxc_l_tmp_native_call_result_n494;
          struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n495 = hxc_l_campaign;
          hxc_l_gc_roots[347] = (const void *)hxc_l_tmp_load_result_n495;
          hxc_l_gc_roots[348] = (const void *)NULL;
          int32_t hxc_l_tmp_conditional_result_n206 = 0;
          if (hxc_l_tmp_load_result_n495 == NULL)
          {
            hxc_l_tmp_conditional_result_n206 = 0;
          }
          else
          {
            struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n496 = hxc_l_campaign;
            hxc_l_gc_roots[349] = (const void *)hxc_l_tmp_load_result_n496;
            if (hxc_l_tmp_load_result_n496 == NULL)
            {
              abort();
            }
            struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n497 = (*hxc_l_tmp_load_result_n496).hxc_admittedLevels;
            hxc_l_gc_roots[350] = (const void *)hxc_l_tmp_class_field_load_result_n497;
            int32_t hxc_l_tmp_array_length_result_n498;
            if (hxc_array_ref_length(hxc_l_tmp_class_field_load_result_n497, &hxc_l_tmp_array_length_result_n498) != HXC_STATUS_OK)
            {
              abort();
            }
            hxc_l_tmp_conditional_result_n206 = hxc_l_tmp_array_length_result_n498;
          }
          int32_t hxc_l_tmp_conditional_load_result_n499 = hxc_l_tmp_conditional_result_n206;
          int32_t hxc_l_tmp_load_result_n500 = hxc_l_selectedCampaignLevelIndex;
          double hxc_l_tmp_static_call_argument_0_load_result_n501 = hxc_l_tmp_static_call_argument_0_n202;
          double hxc_l_tmp_static_call_argument_1_load_result_n502 = hxc_l_tmp_static_call_argument_1_n203;
          int32_t hxc_l_tmp_static_call_argument_2_load_result_n503 = hxc_l_tmp_static_call_argument_2_n204;
          struct hxc_caxecraft_app_CampaignMenuHit hxc_l_tmp_call_result_n505 = hxc_caxecraft_app_CampaignMenu_selectionAt(hxc_l_tmp_static_call_argument_0_load_result_n501, hxc_l_tmp_static_call_argument_1_load_result_n502, hxc_l_tmp_static_call_argument_2_load_result_n503, hxc_l_tmp_static_call_argument_3_n205, hxc_l_tmp_conditional_load_result_n499, hxc_l_tmp_load_result_n500);
          struct hxc_caxecraft_app_CampaignMenuHit hxc_l_campaignChoice = hxc_l_tmp_call_result_n505;
          bool hxc_l_tmp_native_call_result_n506 = IsMouseButtonPressed((int32_t)0);
          if (hxc_l_tmp_native_call_result_n506)
          {
            switch (hxc_l_campaignChoice.hxc_tag) {
              case hxc_caxecraft_app_CampaignMenuHit_NoCampaignMenuHit:
                {
                  break;
                }
              case hxc_caxecraft_app_CampaignMenuHit_LevelHit:
                {
                  if (hxc_l_campaignChoice.hxc_tag != hxc_caxecraft_app_CampaignMenuHit_LevelHit)
                  {
                    abort();
                  }
                  int32_t hxc_l_tmp_enum_payload_project_n509 = hxc_l_campaignChoice.hxc_payload.hxc_LevelHit.hxc_index;
                  int32_t hxc_l_index_h1fd99f53cced = hxc_l_tmp_enum_payload_project_n509;
                  hxc_l_selectedCampaignLevelIndex = hxc_l_index_h1fd99f53cced;
                  break;
                }
              case hxc_caxecraft_app_CampaignMenuHit_LaunchHit:
                {
                  hxc_l_campaignLaunchRequested = true;
                  break;
                }
              case hxc_caxecraft_app_CampaignMenuHit_BackHit:
                {
                  hxc_l_campaignBackRequested = true;
                  break;
                }
              default:
                {
                  abort();
                }
            }
          }
          if (hxc_l_campaignBackRequested)
          {
            enum hxc_caxecraft_app_AppScreen hxc_l_tmp_call_result_n513 = hxc_caxecraft_app_AppScreen_closeCampaignSelection(hxc_l_screen);
            hxc_l_screen = hxc_l_tmp_call_result_n513;
            hxc_l_accumulator = 0.0;
            hxc_l_resetMotionThisFrame = true;
          }
          else
          {
            struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n514 = hxc_l_campaign;
            hxc_l_gc_roots[351] = (const void *)hxc_l_tmp_load_result_n514;
            hxc_l_gc_roots[352] = (const void *)NULL;
            bool hxc_l_tmp_short_circuit_result_n208 = hxc_l_tmp_load_result_n514 != NULL;
            if (hxc_l_tmp_load_result_n514 != NULL)
            {
              hxc_l_tmp_short_circuit_result_n208 = hxc_l_campaignLaunchRequested;
            }
            if (hxc_l_tmp_short_circuit_result_n208)
            {
              struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n517 = hxc_l_campaign;
              hxc_l_gc_roots[353] = (const void *)hxc_l_tmp_load_result_n517;
              if (hxc_l_tmp_load_result_n517 == NULL)
              {
                abort();
              }
              struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n518 = (*hxc_l_tmp_load_result_n517).hxc_admittedLevels;
              hxc_l_gc_roots[354] = (const void *)hxc_l_tmp_class_field_load_result_n518;
              struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_array_get_result_n520;
              if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n518, (size_t)hxc_l_selectedCampaignLevelIndex, &hxc_l_tmp_array_get_result_n520) != HXC_STATUS_OK)
              {
                abort();
              }
              hxc_l_gc_roots[355] = (const void *)hxc_l_tmp_array_get_result_n520;
              hxc_l_requestedCampaignLevel = hxc_l_tmp_array_get_result_n520;
              hxc_l_requestedFromCampaignMenu = true;
            }
          }
        }
        bool hxc_l_tmp_call_result_n522 = hxc_caxecraft_app_AppScreen_showsTitle(hxc_l_screen);
        bool hxc_l_tmp_short_circuit_result_n209 = hxc_l_tmp_call_result_n522;
        if (hxc_l_tmp_call_result_n522)
        {
          hxc_l_tmp_short_circuit_result_n209 = hxc_l_focused;
        }
        if (hxc_l_tmp_short_circuit_result_n209)
        {
          enum hxc_caxecraft_app_AppScreen hxc_l_screenBeforeMenu = hxc_l_screen;
          int32_t hxc_l_modeBeforeMenu = hxc_l_selectedMode;
          enum hxc_caxecraft_app_AppScreen hxc_l_tmp_load_result_n527 = hxc_l_screen;
          struct hxc_caxecraft_app_TitleMenuState hxc_l_tmp_call_result_n529 = hxc_caxecraft_app_TitleMenuFlow_titleMenuState(hxc_l_tmp_load_result_n527, hxc_l_selectedMode);
          struct hxc_caxecraft_app_TitleMenuState hxc_l_menuState = hxc_l_tmp_call_result_n529;
          bool hxc_l_tmp_native_call_result_n530 = IsKeyPressed((int32_t)76);
          if (hxc_l_tmp_native_call_result_n530)
          {
            struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_tmp_load_result_n531 = hxc_l_uiCatalog;
            hxc_l_gc_roots[359] = (const void *)hxc_l_tmp_load_result_n531;
            if (hxc_l_tmp_load_result_n531 == NULL)
            {
              abort();
            }
            int32_t hxc_l_tmp_instance_call_result_n533 = hxc_caxecraft_localization_RuntimeUiCatalog_nextLocale(hxc_l_tmp_load_result_n531, hxc_l_locale);
            hxc_l_locale = hxc_l_tmp_instance_call_result_n533;
          }
          if (hxc_l_menuNextPressed)
          {
            struct hxc_caxecraft_app_TitleMenuState hxc_l_tmp_call_result_n536 = hxc_caxecraft_app_TitleMenuFlow_applyTitleMenuCommand(hxc_l_menuState, 1);
            hxc_l_menuState = hxc_l_tmp_call_result_n536;
          }
          struct Vector2 hxc_l_tmp_native_call_result_n537 = GetMousePosition();
          struct Vector2 hxc_l_menuMouse = hxc_l_tmp_native_call_result_n537;
          struct Vector2 hxc_l_tmp_load_result_n538 = hxc_l_menuMouse;
          struct Vector2 hxc_l_tmp_load_result_n540 = hxc_l_menuMouse;
          int32_t hxc_l_tmp_native_call_result_n542 = GetScreenWidth();
          int32_t hxc_l_tmp_native_call_result_n543 = GetScreenHeight();
          int32_t hxc_l_tmp_call_result_n544 = hxc_caxecraft_app_TitleMenu_selectionAt((double)hxc_l_tmp_load_result_n538.x, (double)hxc_l_tmp_load_result_n540.y, (int32_t)hxc_l_tmp_native_call_result_n542, (int32_t)hxc_l_tmp_native_call_result_n543);
          int32_t hxc_l_hovered = hxc_l_tmp_call_result_n544;
          if (hxc_l_hovered == 0)
          {
            struct hxc_caxecraft_app_TitleMenuState hxc_l_tmp_call_result_n547 = hxc_caxecraft_app_TitleMenuFlow_applyTitleMenuCommand(hxc_l_menuState, 2);
            hxc_l_menuState = hxc_l_tmp_call_result_n547;
          }
          if (hxc_l_hovered == 1)
          {
            struct hxc_caxecraft_app_TitleMenuState hxc_l_tmp_call_result_n550 = hxc_caxecraft_app_TitleMenuFlow_applyTitleMenuCommand(hxc_l_menuState, 3);
            hxc_l_menuState = hxc_l_tmp_call_result_n550;
          }
          int32_t hxc_l_tmp_load_result_n551 = hxc_l_hovered;
          bool hxc_l_tmp_short_circuit_result_n216 = hxc_l_tmp_load_result_n551 >= 0;
          if (hxc_l_tmp_load_result_n551 >= 0)
          {
            bool hxc_l_tmp_native_call_result_n552 = IsMouseButtonPressed((int32_t)0);
            hxc_l_tmp_short_circuit_result_n216 = hxc_l_tmp_native_call_result_n552;
          }
          bool hxc_l_clickedChoice = hxc_l_tmp_short_circuit_result_n216;
          if (hxc_l_clickedChoice)
          {
            struct hxc_caxecraft_app_TitleMenuState hxc_l_tmp_static_call_argument_0_n217 = hxc_l_menuState;
            int32_t hxc_l_tmp_load_result_n556 = hxc_l_hovered;
            int32_t hxc_l_tmp_conditional_result_n218 = 0;
            if (hxc_l_tmp_load_result_n556 == 2)
            {
              hxc_l_tmp_conditional_result_n218 = 5;
            }
            else
            {
              hxc_l_tmp_conditional_result_n218 = 4;
            }
            int32_t hxc_l_tmp_conditional_load_result_n557 = hxc_l_tmp_conditional_result_n218;
            struct hxc_caxecraft_app_TitleMenuState hxc_l_tmp_call_result_n559 = hxc_caxecraft_app_TitleMenuFlow_applyTitleMenuCommand(hxc_l_tmp_static_call_argument_0_n217, hxc_l_tmp_conditional_load_result_n557);
            hxc_l_menuState = hxc_l_tmp_call_result_n559;
          }
          else
          {
            if (hxc_l_menuConfirmPressed)
            {
              struct hxc_caxecraft_app_TitleMenuState hxc_l_tmp_call_result_n562 = hxc_caxecraft_app_TitleMenuFlow_applyTitleMenuCommand(hxc_l_menuState, 4);
              hxc_l_menuState = hxc_l_tmp_call_result_n562;
            }
          }
          hxc_l_screen = hxc_l_menuState.hxc_screen;
          hxc_l_selectedMode = hxc_l_menuState.hxc_mode;
          bool hxc_l_tmp_call_result_n566 = hxc_caxecraft_app_AppScreen_showsCampaignSelection(hxc_l_screen);
          bool hxc_l_tmp_short_circuit_result_n219 = hxc_l_tmp_call_result_n566;
          if (hxc_l_tmp_call_result_n566)
          {
            hxc_l_gc_roots[360] = (const void *)hxc_l_campaign;
            hxc_l_gc_roots[361] = (const void *)NULL;
            hxc_l_tmp_short_circuit_result_n219 = hxc_l_campaign == NULL;
          }
          if (hxc_l_tmp_short_circuit_result_n219)
          {
            hxc_l_screen = hxc_l_screenBeforeMenu;
          }
          enum hxc_caxecraft_app_AppScreen hxc_l_tmp_load_result_n570 = hxc_l_screen;
          enum hxc_caxecraft_app_AppScreen hxc_l_tmp_load_result_n571 = hxc_l_screenBeforeMenu;
          bool hxc_l_tmp_short_circuit_result_n220 = hxc_l_tmp_load_result_n570 != hxc_l_tmp_load_result_n571;
          if (!(hxc_l_tmp_load_result_n570 != hxc_l_tmp_load_result_n571))
          {
            int32_t hxc_l_tmp_load_result_n572 = hxc_l_selectedMode;
            hxc_l_tmp_short_circuit_result_n220 = hxc_l_tmp_load_result_n572 != hxc_l_modeBeforeMenu;
          }
          if (hxc_l_tmp_short_circuit_result_n220)
          {
            hxc_l_accumulator = 0.0;
            hxc_l_resetMotionThisFrame = true;
          }
          enum hxc_caxecraft_app_AppScreen hxc_l_tmp_load_result_n575 = hxc_l_screenBeforeMenu;
          bool hxc_l_tmp_short_circuit_result_n221 = hxc_l_tmp_load_result_n575 == hxc_caxecraft_app_AppScreen_Title;
          if (hxc_l_tmp_load_result_n575 == hxc_caxecraft_app_AppScreen_Title)
          {
            bool hxc_l_tmp_call_result_n578 = hxc_caxecraft_app_AppScreen_isPlaying(hxc_l_screen);
            hxc_l_tmp_short_circuit_result_n221 = hxc_l_tmp_call_result_n578;
          }
          if (hxc_l_tmp_short_circuit_result_n221)
          {
            hxc_l_recapturedThisFrame = true;
            DisableCursor();
          }
          else
          {
            bool hxc_l_tmp_call_result_n581 = hxc_caxecraft_app_AppScreen_showsEditor(hxc_l_screen);
            if (hxc_l_tmp_call_result_n581)
            {
              EnableCursor();
            }
          }
        }
        hxc_l_gc_roots[356] = (const void *)hxc_l_requestedCampaignLevel;
        struct hxc_caxecraft_content_CampaignLevel *hxc_l_destination_ha1c14a301471 = hxc_l_requestedCampaignLevel;
        hxc_l_gc_roots[357] = (const void *)hxc_l_requestedCampaignPortal;
        hxc_l_gc_roots[358] = (const void *)NULL;
        if (hxc_l_requestedCampaignPortal != NULL)
        {
          struct hxc_caxecraft_app_PreparedCampaignPortal *hxc_l_tmp_load_result_n584 = hxc_l_requestedCampaignPortal;
          hxc_l_gc_roots[362] = (const void *)hxc_l_tmp_load_result_n584;
          if (hxc_l_tmp_load_result_n584 == NULL)
          {
            abort();
          }
          struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_class_field_load_result_n585 = (*hxc_l_tmp_load_result_n584).hxc_destinationValue;
          hxc_l_gc_roots[363] = (const void *)hxc_l_tmp_class_field_load_result_n585;
          hxc_l_destination_ha1c14a301471 = hxc_l_tmp_class_field_load_result_n585;
        }
        hxc_l_gc_roots[364] = (const void *)hxc_l_destination_ha1c14a301471;
        hxc_l_gc_roots[365] = (const void *)NULL;
        if (hxc_l_destination_ha1c14a301471 != NULL)
        {
          int32_t hxc_l_tmp_record_field_load_result_n587 = hxc_l_character.hxc_id;
          int32_t hxc_l_tmp_record_field_load_result_n588 = hxc_l_character.hxc_vitals.hxc_health;
          struct hxc_caxecraft_content_LevelPlayerOptions hxc_l_playerOptions = (struct hxc_caxecraft_content_LevelPlayerOptions){ .hxc_aquaticProfile = hxc_l_character.hxc_aquaticProfile, .hxc_entityId = hxc_l_tmp_record_field_load_result_n587, .hxc_initialHealth = hxc_l_tmp_record_field_load_result_n588 };
          struct hxc_caxecraft_content_ContentGenerationSequence *hxc_l_tmp_load_result_n591 = hxc_l_generationSequence;
          hxc_l_gc_roots[366] = (const void *)hxc_l_tmp_load_result_n591;
          if (hxc_l_tmp_load_result_n591 == NULL)
          {
            abort();
          }
          int32_t hxc_l_tmp_instance_call_result_n592 = hxc_caxecraft_content_ContentGenerationSequence_allocate(hxc_l_tmp_load_result_n591);
          int32_t hxc_l_nextGeneration = hxc_l_tmp_instance_call_result_n592;
          hxc_l_gc_roots[367] = (const void *)NULL;
          struct hxc_caxecraft_app_StagedPlayableLevel *hxc_l_stagedDestination = NULL;
          hxc_l_gc_roots[368] = (const void *)hxc_l_requestedCampaignPortal;
          hxc_l_gc_roots[369] = (const void *)NULL;
          if (hxc_l_requestedCampaignPortal != NULL)
          {
            struct hxc_caxecraft_app_PreparedCampaignPortal *hxc_l_tmp_load_result_n594 = hxc_l_requestedCampaignPortal;
            hxc_l_gc_roots[372] = (const void *)hxc_l_tmp_load_result_n594;
            if (hxc_l_tmp_load_result_n594 == NULL)
            {
              abort();
            }
            struct hxc_caxecraft_app_StagedPlayableLevel *hxc_l_tmp_class_field_load_result_n595 = (*hxc_l_tmp_load_result_n594).hxc_stagedValue;
            hxc_l_gc_roots[373] = (const void *)hxc_l_tmp_class_field_load_result_n595;
            hxc_l_stagedDestination = hxc_l_tmp_class_field_load_result_n595;
          }
          hxc_l_gc_roots[374] = (const void *)hxc_l_stagedDestination;
          hxc_l_gc_roots[375] = (const void *)NULL;
          if (hxc_l_stagedDestination == NULL)
          {
            struct hxc_caxecraft_content_ContentPackageStore *hxc_l_tmp_load_result_n597 = hxc_l_contentStore;
            hxc_l_gc_roots[376] = (const void *)hxc_l_tmp_load_result_n597;
            struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_load_result_n598 = hxc_l_destination_ha1c14a301471;
            hxc_l_gc_roots[377] = (const void *)hxc_l_tmp_load_result_n598;
            int32_t hxc_l_tmp_load_result_n599 = hxc_l_nextGeneration;
            struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_tmp_load_result_n600 = hxc_l_contentRegistry;
            hxc_l_gc_roots[378] = (const void *)hxc_l_tmp_load_result_n600;
            if (hxc_l_tmp_load_result_n600 == NULL)
            {
              abort();
            }
            struct hxc_compiler_interface_dispatch_caxecraft_scenario_ScenarioContentRegistry_value hxc_l_tmp_interface_value_n601 = (struct hxc_compiler_interface_dispatch_caxecraft_scenario_ScenarioContentRegistry_value){ .object = hxc_l_tmp_load_result_n600, .table = &hxc_itable_compiler_interface_dispatch_caxecraft_content_RuntimeContentRegistry_itable_layout_caxecraf_hb3f384641e1a61bf };
            struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_tmp_load_result_n602 = hxc_l_contentRegistry;
            hxc_l_gc_roots[379] = (const void *)hxc_l_tmp_load_result_n602;
            if (hxc_l_tmp_load_result_n602 == NULL)
            {
              abort();
            }
            struct hxc_compiler_interface_dispatch_caxecraft_content_LevelContentResolver_value hxc_l_tmp_interface_value_n603 = (struct hxc_compiler_interface_dispatch_caxecraft_content_LevelContentResolver_value){ .object = hxc_l_tmp_load_result_n602, .table = &hxc_itable_compiler_interface_dispatch_caxecraft_content_RuntimeContentRegistry_itable_layout_caxecraf_h82ca4f24ca7a7b5a };
            struct hxc_caxecraft_content_CampaignLevelLoadResult hxc_l_tmp_call_result_n605 = hxc_caxecraft_content_CampaignRuntime_loadCampaignLevel(hxc_l_tmp_load_result_n597, hxc_l_tmp_load_result_n598, hxc_l_tmp_load_result_n599, hxc_l_tmp_interface_value_n601, hxc_l_tmp_interface_value_n603, hxc_l_playerOptions);
            hxc_l_gc_roots[380] = hxc_l_tmp_call_result_n605.hxc_tag == hxc_caxecraft_content_CampaignLevelLoadResult_CampaignLevelReady ? (const void *)hxc_l_tmp_call_result_n605.hxc_payload.hxc_CampaignLevelReady.hxc_candidate : NULL;
            hxc_l_g_hb5a295da9005 = hxc_l_tmp_call_result_n605;
            hxc_l_gc_roots[381] = hxc_l_g_hb5a295da9005.hxc_tag == hxc_caxecraft_content_CampaignLevelLoadResult_CampaignLevelReady ? (const void *)hxc_l_g_hb5a295da9005.hxc_payload.hxc_CampaignLevelReady.hxc_candidate : NULL;
            switch (hxc_l_g_hb5a295da9005.hxc_tag) {
              case hxc_caxecraft_content_CampaignLevelLoadResult_CampaignLevelReady:
                {
                  hxc_l_gc_roots[385] = hxc_l_g_hb5a295da9005.hxc_tag == hxc_caxecraft_content_CampaignLevelLoadResult_CampaignLevelReady ? (const void *)hxc_l_g_hb5a295da9005.hxc_payload.hxc_CampaignLevelReady.hxc_candidate : NULL;
                  if (hxc_l_g_hb5a295da9005.hxc_tag != hxc_caxecraft_content_CampaignLevelLoadResult_CampaignLevelReady)
                  {
                    abort();
                  }
                  struct hxc_caxecraft_content_RuntimeLevelCandidate *hxc_l_tmp_enum_payload_project_n608 = hxc_l_g_hb5a295da9005.hxc_payload.hxc_CampaignLevelReady.hxc_candidate;
                  hxc_l_gc_roots[386] = (const void *)hxc_l_tmp_enum_payload_project_n608;
                  struct hxc_caxecraft_content_RuntimeLevelCandidate *hxc_l_candidate_h82a7d13a8ed0 = hxc_l_tmp_enum_payload_project_n608;
                  hxc_l_gc_roots[387] = (const void *)hxc_l_candidate_h82a7d13a8ed0;
                  struct hxc_caxecraft_app_PlayableLevelStageResult hxc_l_tmp_call_result_n610 = hxc_caxecraft_app_ActivePlayableLevel_stage(hxc_l_candidate_h82a7d13a8ed0);
                  hxc_l_gc_roots[388] = hxc_l_tmp_call_result_n610.hxc_tag == hxc_caxecraft_app_PlayableLevelStageResult_PlayableLevelStaged ? (const void *)hxc_l_tmp_call_result_n610.hxc_payload.hxc_PlayableLevelStaged.hxc_stage : NULL;
                  struct hxc_caxecraft_app_PlayableLevelStageResult hxc_l_g_h9d9237c3bbbc = hxc_l_tmp_call_result_n610;
                  hxc_l_gc_roots[389] = hxc_l_g_h9d9237c3bbbc.hxc_tag == hxc_caxecraft_app_PlayableLevelStageResult_PlayableLevelStaged ? (const void *)hxc_l_g_h9d9237c3bbbc.hxc_payload.hxc_PlayableLevelStaged.hxc_stage : NULL;
                  switch (hxc_l_g_h9d9237c3bbbc.hxc_tag) {
                    case hxc_caxecraft_app_PlayableLevelStageResult_PlayableLevelStaged:
                      {
                        hxc_l_gc_roots[391] = hxc_l_g_h9d9237c3bbbc.hxc_tag == hxc_caxecraft_app_PlayableLevelStageResult_PlayableLevelStaged ? (const void *)hxc_l_g_h9d9237c3bbbc.hxc_payload.hxc_PlayableLevelStaged.hxc_stage : NULL;
                        if (hxc_l_g_h9d9237c3bbbc.hxc_tag != hxc_caxecraft_app_PlayableLevelStageResult_PlayableLevelStaged)
                        {
                          abort();
                        }
                        struct hxc_caxecraft_app_StagedPlayableLevel *hxc_l_tmp_enum_payload_project_n613 = hxc_l_g_h9d9237c3bbbc.hxc_payload.hxc_PlayableLevelStaged.hxc_stage;
                        hxc_l_gc_roots[392] = (const void *)hxc_l_tmp_enum_payload_project_n613;
                        struct hxc_caxecraft_app_StagedPlayableLevel *hxc_l_staged = hxc_l_tmp_enum_payload_project_n613;
                        hxc_l_gc_roots[393] = (const void *)hxc_l_staged;
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
                  hxc_l_gc_roots[390] = hxc_l_g_hb5a295da9005.hxc_tag == hxc_caxecraft_content_CampaignLevelLoadResult_CampaignLevelReady ? (const void *)hxc_l_g_hb5a295da9005.hxc_payload.hxc_CampaignLevelReady.hxc_candidate : NULL;
                  if (hxc_l_g_hb5a295da9005.hxc_tag != hxc_caxecraft_content_CampaignLevelLoadResult_CampaignLevelRejected)
                  {
                    abort();
                  }
                  struct hxc_caxecraft_content_CampaignLevelLoadError hxc_l_tmp_enum_payload_project_n616 = hxc_l_g_hb5a295da9005.hxc_payload.hxc_CampaignLevelRejected.hxc_error;
                  hxc_l_error_h44e49fe88f76 = hxc_l_tmp_enum_payload_project_n616;
                  if (hxc_enum_40863044_retain(&hxc_l_error_h44e49fe88f76) != HXC_STATUS_OK)
                  {
                    abort();
                  }
                  hxc_string hxc_l_tmp_call_result_n618 = hxc_caxecraft_app_CaxecraftApp_campaignLevelLoadFailure(hxc_l_error_h44e49fe88f76);
                  hxc_l_tmp_string_concat_right_owner_n231 = hxc_l_tmp_call_result_n618;
                  hxc_string hxc_l_tmp_string_concat_result_n620 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
                  if (hxc_string_concat_ref((hxc_string){ (const uint8_t *)"caxecraft: campaign level rejected: ", 36, true, NULL }, hxc_l_tmp_string_concat_right_owner_n231, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n620) != HXC_STATUS_OK)
                  {
                    abort();
                  }
                  hxc_l_tmp_sys_println_string_argument_owner_n232 = hxc_l_tmp_string_concat_result_n620;
                  if (hxc_io_println(hxc_l_tmp_sys_println_string_argument_owner_n232) != HXC_STATUS_OK)
                  {
                    if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n232) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n231) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    hxc_enum_40863044_destroy(&hxc_l_error_h44e49fe88f76);
                    hxc_enum_5543d34a_destroy(&hxc_l_g_hb5a295da9005);
                    if (hxc_array_ref_release(hxc_l_initialActorPhases) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    hxc_enum_2c0e695d_destroy(&hxc_l_tmp_static_call_argument_0_owner_n94);
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
                    if (hxc_array_ref_release(hxc_l_tmp_object_storage_n84.hxc_stateStartTicks) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    if (hxc_array_ref_release(hxc_l_tmp_object_storage_n84.hxc_stateIds) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    if (hxc_array_ref_release(hxc_l_tmp_object_storage_n84.hxc_objectIds) != HXC_STATUS_OK)
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
                  if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n232) != HXC_STATUS_OK)
                  {
                    abort();
                  }
                  if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n231) != HXC_STATUS_OK)
                  {
                    abort();
                  }
                  hxc_enum_40863044_destroy(&hxc_l_error_h44e49fe88f76);
                  break;
                }
              default:
                {
                  abort();
                }
            }
            hxc_enum_5543d34a_destroy(&hxc_l_g_hb5a295da9005);
          }
          hxc_l_gc_roots[382] = (const void *)hxc_l_stagedDestination;
          struct hxc_caxecraft_app_StagedPlayableLevel *hxc_l_readyDestination = hxc_l_stagedDestination;
          bool hxc_l_destinationPublished = false;
          hxc_l_gc_roots[383] = (const void *)hxc_l_readyDestination;
          hxc_l_gc_roots[384] = (const void *)NULL;
          if (hxc_l_readyDestination != NULL)
          {
            struct hxc_caxecraft_app_ActivePlayableLevel *hxc_l_tmp_load_result_n624 = hxc_l_activeLevel;
            hxc_l_gc_roots[394] = (const void *)hxc_l_tmp_load_result_n624;
            if (hxc_l_tmp_load_result_n624 == NULL)
            {
              abort();
            }
            hxc_l_gc_roots[395] = (const void *)hxc_l_readyDestination;
            struct hxc_caxecraft_app_PlayableLevelPublicationResult hxc_l_tmp_instance_call_result_n626 = hxc_caxecraft_app_ActivePlayableLevel_publishStaged(hxc_l_tmp_load_result_n624, hxc_l_readyDestination);
            struct hxc_caxecraft_app_PlayableLevelPublicationResult hxc_l_g_hfbab2e12a646 = hxc_l_tmp_instance_call_result_n626;
            switch (hxc_l_g_hfbab2e12a646.hxc_tag) {
              case hxc_caxecraft_app_PlayableLevelPublicationResult_PlayableLevelPublished:
                {
                  if (hxc_l_g_hfbab2e12a646.hxc_tag != hxc_caxecraft_app_PlayableLevelPublicationResult_PlayableLevelPublished)
                  {
                    abort();
                  }
                  int32_t hxc_l_tmp_enum_payload_project_n629 = hxc_l_g_hfbab2e12a646.hxc_payload.hxc_PlayableLevelPublished.hxc_active;
                  int32_t hxc_l_selected = hxc_l_tmp_enum_payload_project_n629;
                  hxc_l_destinationPublished = true;
                  hxc_l_gc_roots[398] = (const void *)hxc_l_destination_ha1c14a301471;
                  hxc_l_campaignLevel = hxc_l_destination_ha1c14a301471;
                  struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_load_result_n631 = hxc_l_destination_ha1c14a301471;
                  hxc_l_gc_roots[399] = (const void *)hxc_l_tmp_load_result_n631;
                  if (hxc_l_tmp_load_result_n631 == NULL)
                  {
                    abort();
                  }
                  hxc_string hxc_l_tmp_class_field_load_result_n632 = (*hxc_l_tmp_load_result_n631).hxc_id;
                  hxc_string hxc_l_tmp_string_assignment_replacement_owner_n237 = hxc_l_tmp_class_field_load_result_n632;
                  if (hxc_string_retain(hxc_l_tmp_string_assignment_replacement_owner_n237) != HXC_STATUS_OK)
                  {
                    abort();
                  }
                  hxc_string hxc_l_tmp_string_assignment_replacement_owned_load_result_n633 = hxc_l_tmp_string_assignment_replacement_owner_n237;
                  if (hxc_string_release(&hxc_l_levelLabel) != HXC_STATUS_OK)
                  {
                    abort();
                  }
                  hxc_l_levelLabel = hxc_l_tmp_string_assignment_replacement_owned_load_result_n633;
                  hxc_l_gc_roots[400] = (const void *)hxc_l_campaign;
                  struct hxc_caxecraft_content_CampaignManifest *hxc_l_selectedCampaign_hc274acfe2c0d = hxc_l_campaign;
                  hxc_l_gc_roots[401] = (const void *)hxc_l_selectedCampaign_hc274acfe2c0d;
                  hxc_l_gc_roots[402] = (const void *)NULL;
                  if (hxc_l_selectedCampaign_hc274acfe2c0d != NULL)
                  {
                    int32_t hxc_l_g_hc35e2356da29 = 0;
                    struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n636 = hxc_l_selectedCampaign_hc274acfe2c0d;
                    hxc_l_gc_roots[403] = (const void *)hxc_l_tmp_load_result_n636;
                    if (hxc_l_tmp_load_result_n636 == NULL)
                    {
                      abort();
                    }
                    struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n637 = (*hxc_l_tmp_load_result_n636).hxc_admittedLevels;
                    hxc_l_gc_roots[404] = (const void *)hxc_l_tmp_class_field_load_result_n637;
                    int32_t hxc_l_tmp_array_length_result_n638;
                    if (hxc_array_ref_length(hxc_l_tmp_class_field_load_result_n637, &hxc_l_tmp_array_length_result_n638) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    int32_t hxc_l_g1_hac968379a140 = hxc_l_tmp_array_length_result_n638;
                    while (1)
                    {
                      int32_t hxc_l_tmp_load_result_n639 = hxc_l_g_hc35e2356da29;
                      if (!(hxc_l_tmp_load_result_n639 < hxc_l_g1_hac968379a140))
                      {
                        break;
                      }
                      int32_t hxc_l_tmp_increment_load_result_n641 = hxc_l_g_hc35e2356da29;
                      hxc_l_g_hc35e2356da29 = hxc_i32_add_wrapping(hxc_l_tmp_increment_load_result_n641, 1);
                      int32_t hxc_l_index_h643b4d73d305 = hxc_l_tmp_increment_load_result_n641;
                      struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n642 = hxc_l_selectedCampaign_hc274acfe2c0d;
                      hxc_l_gc_roots[412] = (const void *)hxc_l_tmp_load_result_n642;
                      if (hxc_l_tmp_load_result_n642 == NULL)
                      {
                        abort();
                      }
                      struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n643 = (*hxc_l_tmp_load_result_n642).hxc_admittedLevels;
                      hxc_l_gc_roots[413] = (const void *)hxc_l_tmp_class_field_load_result_n643;
                      struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_array_get_result_n645;
                      if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n643, (size_t)hxc_l_index_h643b4d73d305, &hxc_l_tmp_array_get_result_n645) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      hxc_l_gc_roots[414] = (const void *)hxc_l_tmp_array_get_result_n645;
                      if (hxc_l_tmp_array_get_result_n645 == NULL)
                      {
                        abort();
                      }
                      hxc_string hxc_l_tmp_class_field_load_result_n646 = (*hxc_l_tmp_array_get_result_n645).hxc_id;
                      struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_load_result_n647 = hxc_l_destination_ha1c14a301471;
                      hxc_l_gc_roots[415] = (const void *)hxc_l_tmp_load_result_n647;
                      if (hxc_l_tmp_load_result_n647 == NULL)
                      {
                        abort();
                      }
                      hxc_string hxc_l_tmp_class_field_load_result_n648 = (*hxc_l_tmp_load_result_n647).hxc_id;
                      bool hxc_l_tmp_string_equality_result_n242 = hxc_l_tmp_class_field_load_result_n646.data == NULL || hxc_l_tmp_class_field_load_result_n648.data == NULL ? hxc_l_tmp_class_field_load_result_n646.data == hxc_l_tmp_class_field_load_result_n648.data : hxc_l_tmp_class_field_load_result_n646.byte_length == hxc_l_tmp_class_field_load_result_n648.byte_length && (hxc_l_tmp_class_field_load_result_n646.byte_length == 0 || memcmp(hxc_l_tmp_class_field_load_result_n646.data, hxc_l_tmp_class_field_load_result_n648.data, hxc_l_tmp_class_field_load_result_n646.byte_length) == 0);
                      if (hxc_l_tmp_string_equality_result_n242)
                      {
                        hxc_l_selectedCampaignLevelIndex = hxc_l_index_h643b4d73d305;
                      }
                    }
                  }
                  struct hxc_caxecraft_app_ActivePlayableLevel *hxc_l_tmp_load_result_n652 = hxc_l_activeLevel;
                  hxc_l_gc_roots[405] = (const void *)hxc_l_tmp_load_result_n652;
                  if (hxc_l_tmp_load_result_n652 == NULL)
                  {
                    abort();
                  }
                  struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_class_field_load_result_n653 = (*hxc_l_tmp_load_result_n652).hxc_activeView;
                  hxc_l_gc_roots[406] = (const void *)hxc_l_tmp_class_field_load_result_n653;
                  hxc_l_levelView_hb85a9a956786 = hxc_l_tmp_class_field_load_result_n653;
                  struct hxc_caxecraft_app_ActivePlayableLevel *hxc_l_tmp_load_result_n654 = hxc_l_activeLevel;
                  hxc_l_gc_roots[407] = (const void *)hxc_l_tmp_load_result_n654;
                  if (hxc_l_tmp_load_result_n654 == NULL)
                  {
                    abort();
                  }
                  struct hxc_caxecraft_content_ActiveContent *hxc_l_tmp_class_field_load_result_n655 = (*hxc_l_tmp_load_result_n654).hxc_content;
                  hxc_l_gc_roots[408] = (const void *)hxc_l_tmp_class_field_load_result_n655;
                  if (hxc_l_tmp_class_field_load_result_n655 == NULL)
                  {
                    abort();
                  }
                  struct hxc_caxecraft_content_LoadedContentGeneration *hxc_l_tmp_class_field_load_result_n656 = (*hxc_l_tmp_class_field_load_result_n655).hxc_active;
                  hxc_l_gc_roots[409] = (const void *)hxc_l_tmp_class_field_load_result_n656;
                  if (hxc_l_tmp_class_field_load_result_n656 == NULL)
                  {
                    abort();
                  }
                  struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_class_field_load_result_n657 = (*hxc_l_tmp_class_field_load_result_n656).hxc_ownedSession;
                  hxc_l_gc_roots[410] = (const void *)hxc_l_tmp_class_field_load_result_n657;
                  hxc_l_session = hxc_l_tmp_class_field_load_result_n657;
                  struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n658 = hxc_l_session;
                  hxc_l_gc_roots[411] = (const void *)hxc_l_tmp_load_result_n658;
                  if (hxc_l_tmp_load_result_n658 == NULL)
                  {
                    abort();
                  }
                  struct hxc_caxecraft_domain_GameView hxc_l_tmp_instance_call_result_n659 = hxc_caxecraft_domain_GameSession_view(hxc_l_tmp_load_result_n658);
                  hxc_l_initialView = hxc_l_tmp_instance_call_result_n659;
                  if (!hxc_l_initialView.hxc_valid)
                  {
                    hxc_l_quit = true;
                  }
                  else
                  {
                    hxc_l_character = hxc_l_initialView.hxc_localPlayer;
                    hxc_l_conversation = (struct hxc_optional_caxecraft_app_ConversationState){ .hxc_has_value = false };
                    hxc_string hxc_l_tmp_string_assignment_replacement_owner_n243 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
                    if (hxc_string_retain(hxc_l_tmp_string_assignment_replacement_owner_n243) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    hxc_string hxc_l_tmp_string_assignment_replacement_owned_load_result_n662 = hxc_l_tmp_string_assignment_replacement_owner_n243;
                    if (hxc_string_release(&hxc_l_activeDialogue) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    hxc_l_activeDialogue = hxc_l_tmp_string_assignment_replacement_owned_load_result_n662;
                    hxc_string hxc_l_tmp_string_assignment_replacement_owner_n244 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
                    if (hxc_string_retain(hxc_l_tmp_string_assignment_replacement_owner_n244) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    hxc_string hxc_l_tmp_string_assignment_replacement_owned_load_result_n663 = hxc_l_tmp_string_assignment_replacement_owner_n244;
                    if (hxc_string_release(&hxc_l_latestJournalId) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    hxc_l_latestJournalId = hxc_l_tmp_string_assignment_replacement_owned_load_result_n663;
                    struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n664 = hxc_l_levelView_hb85a9a956786;
                    hxc_l_gc_roots[416] = (const void *)hxc_l_tmp_load_result_n664;
                    if (hxc_l_tmp_load_result_n664 == NULL)
                    {
                      abort();
                    }
                    struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_class_field_load_result_n665 = (*hxc_l_tmp_load_result_n664).hxc_levelPresentation;
                    hxc_l_gc_roots[417] = (const void *)hxc_l_tmp_class_field_load_result_n665;
                    if (hxc_l_tmp_class_field_load_result_n665 == NULL)
                    {
                      abort();
                    }
                    hxc_string hxc_l_tmp_class_field_load_result_n666 = (*hxc_l_tmp_class_field_load_result_n665).hxc_startingObjective;
                    hxc_string hxc_l_tmp_string_assignment_replacement_owner_n245 = hxc_l_tmp_class_field_load_result_n666;
                    if (hxc_string_retain(hxc_l_tmp_string_assignment_replacement_owner_n245) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    hxc_string hxc_l_tmp_string_assignment_replacement_owned_load_result_n667 = hxc_l_tmp_string_assignment_replacement_owner_n245;
                    if (hxc_string_release(&hxc_l_currentObjectiveId) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    hxc_l_currentObjectiveId = hxc_l_tmp_string_assignment_replacement_owned_load_result_n667;
                    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n668 = hxc_l_session;
                    hxc_l_gc_roots[418] = (const void *)hxc_l_tmp_load_result_n668;
                    hxc_l_gc_roots[419] = (const void *)hxc_l_levelView_hb85a9a956786;
                    struct hxc_caxecraft_app_CaxecraftApp_AvailableInteractionTarget hxc_l_tmp_call_result_n670 = hxc_caxecraft_app_CaxecraftApp_nearestAvailableInteraction(hxc_l_tmp_load_result_n668, hxc_l_levelView_hb85a9a956786);
                    hxc_l_tmp_static_call_argument_0_owner_n246 = hxc_l_tmp_call_result_n670;
                    enum hxc_caxecraft_app_InteractionPrompt hxc_l_tmp_call_result_n672 = hxc_caxecraft_app_CaxecraftApp_promptForAvailableInteraction(hxc_l_tmp_static_call_argument_0_owner_n246);
                    hxc_l_availableInteractionPrompt = hxc_l_tmp_call_result_n672;
                    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n673 = hxc_l_session;
                    hxc_l_gc_roots[420] = (const void *)hxc_l_tmp_load_result_n673;
                    if (hxc_l_tmp_load_result_n673 == NULL)
                    {
                      abort();
                    }
                    struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n674 = hxc_l_levelView_hb85a9a956786;
                    hxc_l_gc_roots[421] = (const void *)hxc_l_tmp_load_result_n674;
                    if (hxc_l_tmp_load_result_n674 == NULL)
                    {
                      abort();
                    }
                    int32_t hxc_l_tmp_class_field_load_result_n675 = (*hxc_l_tmp_load_result_n674).hxc_enemyId;
                    struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n676 = hxc_caxecraft_domain_GameSession_readCharacter(hxc_l_tmp_load_result_n673, hxc_l_tmp_class_field_load_result_n675);
                    hxc_l_enemyActor = hxc_l_tmp_instance_call_result_n676;
                    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n677 = hxc_l_session;
                    hxc_l_gc_roots[422] = (const void *)hxc_l_tmp_load_result_n677;
                    if (hxc_l_tmp_load_result_n677 == NULL)
                    {
                      abort();
                    }
                    struct hxc_array_ref *hxc_l_tmp_instance_call_result_n678 = hxc_caxecraft_domain_GameSession_actorControllerStateSnapshots(hxc_l_tmp_load_result_n677);
                    hxc_l_phases = hxc_l_tmp_instance_call_result_n678;
                    struct hxc_array_ref *hxc_l_tmp_load_result_n679 = hxc_l_phases;
                    struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n680 = hxc_l_levelView_hb85a9a956786;
                    hxc_l_gc_roots[423] = (const void *)hxc_l_tmp_load_result_n680;
                    if (hxc_l_tmp_load_result_n680 == NULL)
                    {
                      abort();
                    }
                    int32_t hxc_l_tmp_class_field_load_result_n681 = (*hxc_l_tmp_load_result_n680).hxc_enemyId;
                    struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation hxc_l_tmp_call_result_n683 = hxc_caxecraft_app_CaxecraftApp_observeActorPhase(hxc_l_tmp_load_result_n679, hxc_l_tmp_class_field_load_result_n681, hxc_caxecraft_domain_ActorControllerPhase_Resting);
                    hxc_l_enemyPhase_ha225e19c84ac = hxc_l_tmp_call_result_n683;
                    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n684 = hxc_l_session;
                    hxc_l_gc_roots[424] = (const void *)hxc_l_tmp_load_result_n684;
                    struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n685 = hxc_l_levelView_hb85a9a956786;
                    hxc_l_gc_roots[425] = (const void *)hxc_l_tmp_load_result_n685;
                    bool hxc_l_tmp_call_result_n687 = hxc_caxecraft_app_CaxecraftApp_dialogueActorsAreValid(hxc_l_tmp_load_result_n684, hxc_l_tmp_load_result_n685, hxc_l_phases);
                    bool hxc_l_tmp_short_circuit_result_n248 = !hxc_l_tmp_call_result_n687;
                    if (!!hxc_l_tmp_call_result_n687)
                    {
                      hxc_l_tmp_short_circuit_result_n248 = hxc_l_enemyActor.hxc_id <= 0;
                    }
                    bool hxc_l_tmp_short_circuit_load_result_n689 = hxc_l_tmp_short_circuit_result_n248;
                    bool hxc_l_tmp_short_circuit_result_n249 = hxc_l_tmp_short_circuit_load_result_n689;
                    if (!hxc_l_tmp_short_circuit_load_result_n689)
                    {
                      hxc_l_tmp_short_circuit_result_n249 = !hxc_l_enemyPhase_ha225e19c84ac.hxc_valid;
                    }
                    if (hxc_l_tmp_short_circuit_result_n249)
                    {
                      hxc_l_quit = true;
                    }
                    hxc_l_swordCombat = (struct hxc_caxecraft_gameplay_SwordCombatState){ .hxc_cooldownTicks = 0 };
                    hxc_l_berryDrop = (struct hxc_caxecraft_gameplay_BerryDropState){ .hxc_active = false, .hxc_amount = 0, .hxc_x = 0.0, .hxc_y = 0.0, .hxc_z = 0.0 };
                    hxc_l_cameraWaterBlend = 0.0;
                    struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n694 = hxc_l_levelView_hb85a9a956786;
                    hxc_l_gc_roots[426] = (const void *)hxc_l_tmp_load_result_n694;
                    if (hxc_l_tmp_load_result_n694 == NULL)
                    {
                      abort();
                    }
                    struct hxc_caxecraft_scenario_ScenarioTransform hxc_l_tmp_class_field_load_result_n695 = (*hxc_l_tmp_load_result_n694).hxc_playerSpawn;
                    struct hxc_caxecraft_app_RenderPosition hxc_l_tmp_call_result_n696 = hxc_caxecraft_app_SpawnCameraHeading_headingForSpawn(hxc_l_tmp_class_field_load_result_n695);
                    struct hxc_caxecraft_app_RenderPosition hxc_l_publishedHeading = hxc_l_tmp_call_result_n696;
                    hxc_l_lookX = hxc_l_publishedHeading.hxc_x;
                    hxc_l_lookY = hxc_l_publishedHeading.hxc_y;
                    hxc_l_lookZ = hxc_l_publishedHeading.hxc_z;
                    hxc_l_accumulator = 0.0;
                    struct hxc_caxecraft_app_MotionHistory hxc_l_tmp_call_result_n701 = hxc_caxecraft_app_MotionInterpolation_start(hxc_l_character.hxc_body);
                    hxc_l_motionHistory = hxc_l_tmp_call_result_n701;
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
                    struct hxc_caxecraft_app_TerrainRenderer *hxc_l_tmp_owned_class_field_address_n702 = &(*hxc_l_self).hxc_terrainRenderer;
                    struct hxc_caxecraft_app_TerrainChunkCache *hxc_l_tmp_owned_class_field_address_n703 = &(*hxc_l_tmp_owned_class_field_address_n702).hxc_cache;
                    if (hxc_l_tmp_owned_class_field_address_n703 == NULL)
                    {
                      abort();
                    }
                    hxc_caxecraft_app_TerrainChunkCache_invalidateAll(hxc_l_tmp_owned_class_field_address_n703);
                    hxc_l_gc_roots[427] = (const void *)NULL;
                    hxc_l_preparedPortal = NULL;
                    struct hxc_caxecraft_app_TerrainRenderer *hxc_l_tmp_owned_class_field_address_n704 = &(*hxc_l_self).hxc_portalTerrainRenderer;
                    struct hxc_caxecraft_app_TerrainChunkCache *hxc_l_tmp_owned_class_field_address_n705 = &(*hxc_l_tmp_owned_class_field_address_n704).hxc_cache;
                    if (hxc_l_tmp_owned_class_field_address_n705 == NULL)
                    {
                      abort();
                    }
                    hxc_caxecraft_app_TerrainChunkCache_invalidateAll(hxc_l_tmp_owned_class_field_address_n705);
                    hxc_l_gc_roots[428] = (const void *)hxc_l_campaign;
                    struct hxc_caxecraft_content_CampaignManifest *hxc_l_nextCampaign = hxc_l_campaign;
                    hxc_l_gc_roots[429] = (const void *)hxc_l_campaignLevel;
                    struct hxc_caxecraft_content_CampaignLevel *hxc_l_nextCampaignLevel = hxc_l_campaignLevel;
                    struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n708 = hxc_l_nextCampaign;
                    hxc_l_gc_roots[430] = (const void *)hxc_l_tmp_load_result_n708;
                    hxc_l_gc_roots[431] = (const void *)NULL;
                    bool hxc_l_tmp_short_circuit_result_n253 = hxc_l_tmp_load_result_n708 != NULL;
                    if (hxc_l_tmp_load_result_n708 != NULL)
                    {
                      hxc_l_gc_roots[432] = (const void *)hxc_l_nextCampaignLevel;
                      hxc_l_gc_roots[433] = (const void *)NULL;
                      hxc_l_tmp_short_circuit_result_n253 = hxc_l_nextCampaignLevel != NULL;
                    }
                    if (hxc_l_tmp_short_circuit_result_n253)
                    {
                      int32_t hxc_l_tmp_record_field_load_result_n711 = hxc_l_character.hxc_id;
                      int32_t hxc_l_tmp_record_field_load_result_n712 = hxc_l_character.hxc_vitals.hxc_health;
                      struct hxc_caxecraft_content_LevelPlayerOptions hxc_l_nextPortalPlayer = (struct hxc_caxecraft_content_LevelPlayerOptions){ .hxc_aquaticProfile = hxc_l_character.hxc_aquaticProfile, .hxc_entityId = hxc_l_tmp_record_field_load_result_n711, .hxc_initialHealth = hxc_l_tmp_record_field_load_result_n712 };
                      struct hxc_caxecraft_content_ContentPackageStore *hxc_l_tmp_load_result_n715 = hxc_l_contentStore;
                      hxc_l_gc_roots[434] = (const void *)hxc_l_tmp_load_result_n715;
                      struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n716 = hxc_l_nextCampaign;
                      hxc_l_gc_roots[435] = (const void *)hxc_l_tmp_load_result_n716;
                      struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_load_result_n717 = hxc_l_nextCampaignLevel;
                      hxc_l_gc_roots[436] = (const void *)hxc_l_tmp_load_result_n717;
                      struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n718 = hxc_l_levelView_hb85a9a956786;
                      hxc_l_gc_roots[437] = (const void *)hxc_l_tmp_load_result_n718;
                      struct hxc_caxecraft_content_ContentGenerationSequence *hxc_l_tmp_load_result_n719 = hxc_l_generationSequence;
                      hxc_l_gc_roots[438] = (const void *)hxc_l_tmp_load_result_n719;
                      if (hxc_l_tmp_load_result_n719 == NULL)
                      {
                        abort();
                      }
                      int32_t hxc_l_tmp_instance_call_result_n720 = hxc_caxecraft_content_ContentGenerationSequence_allocate(hxc_l_tmp_load_result_n719);
                      struct hxc_caxecraft_content_LevelPlayerOptions hxc_l_tmp_load_result_n721 = hxc_l_nextPortalPlayer;
                      hxc_l_gc_roots[439] = (const void *)(struct hxc_caxecraft_app_CampaignPortalPreloadRequest){ .hxc_campaign = hxc_l_tmp_load_result_n716, .hxc_nextGeneration = hxc_l_tmp_instance_call_result_n720, .hxc_playerOptions = hxc_l_tmp_load_result_n721, .hxc_sourceLevel = hxc_l_tmp_load_result_n717, .hxc_sourceView = hxc_l_tmp_load_result_n718, .hxc_store = hxc_l_tmp_load_result_n715 }.hxc_campaign;
                      hxc_l_gc_roots[440] = (const void *)(struct hxc_caxecraft_app_CampaignPortalPreloadRequest){ .hxc_campaign = hxc_l_tmp_load_result_n716, .hxc_nextGeneration = hxc_l_tmp_instance_call_result_n720, .hxc_playerOptions = hxc_l_tmp_load_result_n721, .hxc_sourceLevel = hxc_l_tmp_load_result_n717, .hxc_sourceView = hxc_l_tmp_load_result_n718, .hxc_store = hxc_l_tmp_load_result_n715 }.hxc_sourceLevel;
                      hxc_l_gc_roots[441] = (const void *)(struct hxc_caxecraft_app_CampaignPortalPreloadRequest){ .hxc_campaign = hxc_l_tmp_load_result_n716, .hxc_nextGeneration = hxc_l_tmp_instance_call_result_n720, .hxc_playerOptions = hxc_l_tmp_load_result_n721, .hxc_sourceLevel = hxc_l_tmp_load_result_n717, .hxc_sourceView = hxc_l_tmp_load_result_n718, .hxc_store = hxc_l_tmp_load_result_n715 }.hxc_sourceView;
                      hxc_l_gc_roots[442] = (const void *)(struct hxc_caxecraft_app_CampaignPortalPreloadRequest){ .hxc_campaign = hxc_l_tmp_load_result_n716, .hxc_nextGeneration = hxc_l_tmp_instance_call_result_n720, .hxc_playerOptions = hxc_l_tmp_load_result_n721, .hxc_sourceLevel = hxc_l_tmp_load_result_n717, .hxc_sourceView = hxc_l_tmp_load_result_n718, .hxc_store = hxc_l_tmp_load_result_n715 }.hxc_store;
                      struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_tmp_load_result_n723 = hxc_l_contentRegistry;
                      hxc_l_gc_roots[443] = (const void *)hxc_l_tmp_load_result_n723;
                      if (hxc_l_tmp_load_result_n723 == NULL)
                      {
                        abort();
                      }
                      struct hxc_compiler_interface_dispatch_caxecraft_scenario_ScenarioContentRegistry_value hxc_l_tmp_interface_value_n724 = (struct hxc_compiler_interface_dispatch_caxecraft_scenario_ScenarioContentRegistry_value){ .object = hxc_l_tmp_load_result_n723, .table = &hxc_itable_compiler_interface_dispatch_caxecraft_content_RuntimeContentRegistry_itable_layout_caxecraf_hb3f384641e1a61bf };
                      struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_tmp_load_result_n725 = hxc_l_contentRegistry;
                      hxc_l_gc_roots[444] = (const void *)hxc_l_tmp_load_result_n725;
                      if (hxc_l_tmp_load_result_n725 == NULL)
                      {
                        abort();
                      }
                      struct hxc_compiler_interface_dispatch_caxecraft_content_LevelContentResolver_value hxc_l_tmp_interface_value_n726 = (struct hxc_compiler_interface_dispatch_caxecraft_content_LevelContentResolver_value){ .object = hxc_l_tmp_load_result_n725, .table = &hxc_itable_compiler_interface_dispatch_caxecraft_content_RuntimeContentRegistry_itable_layout_caxecraf_h82ca4f24ca7a7b5a };
                      struct hxc_caxecraft_app_CampaignPortalPreloadResult hxc_l_tmp_call_result_n727 = hxc_caxecraft_app_CampaignPortal_preloadCampaignPortal((struct hxc_caxecraft_app_CampaignPortalPreloadRequest){ .hxc_campaign = hxc_l_tmp_load_result_n716, .hxc_nextGeneration = hxc_l_tmp_instance_call_result_n720, .hxc_playerOptions = hxc_l_tmp_load_result_n721, .hxc_sourceLevel = hxc_l_tmp_load_result_n717, .hxc_sourceView = hxc_l_tmp_load_result_n718, .hxc_store = hxc_l_tmp_load_result_n715 }, hxc_l_tmp_interface_value_n724, hxc_l_tmp_interface_value_n726);
                      hxc_l_gc_roots[445] = hxc_l_tmp_call_result_n727.hxc_tag == hxc_caxecraft_app_CampaignPortalPreloadResult_CampaignPortalReady ? (const void *)hxc_l_tmp_call_result_n727.hxc_payload.hxc_CampaignPortalReady.hxc_portal : NULL;
                      hxc_l_g_h3e205aaff29c = hxc_l_tmp_call_result_n727;
                      hxc_l_gc_roots[446] = hxc_l_g_h3e205aaff29c.hxc_tag == hxc_caxecraft_app_CampaignPortalPreloadResult_CampaignPortalReady ? (const void *)hxc_l_g_h3e205aaff29c.hxc_payload.hxc_CampaignPortalReady.hxc_portal : NULL;
                      switch (hxc_l_g_h3e205aaff29c.hxc_tag) {
                        case hxc_caxecraft_app_CampaignPortalPreloadResult_CampaignPortalNotDeclared:
                          {
                            break;
                          }
                        case hxc_caxecraft_app_CampaignPortalPreloadResult_CampaignPortalReady:
                          {
                            hxc_l_gc_roots[447] = hxc_l_g_h3e205aaff29c.hxc_tag == hxc_caxecraft_app_CampaignPortalPreloadResult_CampaignPortalReady ? (const void *)hxc_l_g_h3e205aaff29c.hxc_payload.hxc_CampaignPortalReady.hxc_portal : NULL;
                            if (hxc_l_g_h3e205aaff29c.hxc_tag != hxc_caxecraft_app_CampaignPortalPreloadResult_CampaignPortalReady)
                            {
                              abort();
                            }
                            struct hxc_caxecraft_app_PreparedCampaignPortal *hxc_l_tmp_enum_payload_project_n730 = hxc_l_g_h3e205aaff29c.hxc_payload.hxc_CampaignPortalReady.hxc_portal;
                            hxc_l_gc_roots[448] = (const void *)hxc_l_tmp_enum_payload_project_n730;
                            struct hxc_caxecraft_app_PreparedCampaignPortal *hxc_l_portal_h479d6cf1eb10 = hxc_l_tmp_enum_payload_project_n730;
                            hxc_l_gc_roots[449] = (const void *)hxc_l_portal_h479d6cf1eb10;
                            hxc_l_preparedPortal = hxc_l_portal_h479d6cf1eb10;
                            struct hxc_caxecraft_app_PreparedCampaignPortal *hxc_l_tmp_load_result_n732 = hxc_l_portal_h479d6cf1eb10;
                            hxc_l_gc_roots[450] = (const void *)hxc_l_tmp_load_result_n732;
                            if (hxc_l_tmp_load_result_n732 == NULL)
                            {
                              abort();
                            }
                            struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_class_field_load_result_n733 = (*hxc_l_tmp_load_result_n732).hxc_destinationValue;
                            hxc_l_gc_roots[451] = (const void *)hxc_l_tmp_class_field_load_result_n733;
                            if (hxc_l_tmp_class_field_load_result_n733 == NULL)
                            {
                              abort();
                            }
                            hxc_string hxc_l_tmp_class_field_load_result_n734 = (*hxc_l_tmp_class_field_load_result_n733).hxc_id;
                            hxc_string hxc_l_tmp_string_concat_result_n735 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
                            if (hxc_string_concat_ref((hxc_string){ (const uint8_t *)"caxecraft: campaign-portal=", 27, true, NULL }, hxc_l_tmp_class_field_load_result_n734, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n735) != HXC_STATUS_OK)
                            {
                              abort();
                            }
                            hxc_l_tmp_sys_println_string_argument_owner_n257 = hxc_l_tmp_string_concat_result_n735;
                            if (hxc_io_println(hxc_l_tmp_sys_println_string_argument_owner_n257) != HXC_STATUS_OK)
                            {
                              if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n257) != HXC_STATUS_OK)
                              {
                                abort();
                              }
                              hxc_enum_9b450cfc_destroy(&hxc_l_g_h3e205aaff29c);
                              if (hxc_array_ref_release(hxc_l_phases) != HXC_STATUS_OK)
                              {
                                abort();
                              }
                              hxc_enum_2c0e695d_destroy(&hxc_l_tmp_static_call_argument_0_owner_n246);
                              if (hxc_array_ref_release(hxc_l_initialActorPhases) != HXC_STATUS_OK)
                              {
                                abort();
                              }
                              hxc_enum_2c0e695d_destroy(&hxc_l_tmp_static_call_argument_0_owner_n94);
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
                              if (hxc_array_ref_release(hxc_l_tmp_object_storage_n84.hxc_stateStartTicks) != HXC_STATUS_OK)
                              {
                                abort();
                              }
                              if (hxc_array_ref_release(hxc_l_tmp_object_storage_n84.hxc_stateIds) != HXC_STATUS_OK)
                              {
                                abort();
                              }
                              if (hxc_array_ref_release(hxc_l_tmp_object_storage_n84.hxc_objectIds) != HXC_STATUS_OK)
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
                            if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n257) != HXC_STATUS_OK)
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
                      hxc_enum_9b450cfc_destroy(&hxc_l_g_h3e205aaff29c);
                    }
                    hxc_l_resetMotionThisFrame = true;
                    if (hxc_l_requestedFromCampaignMenu)
                    {
                      enum hxc_caxecraft_app_AppScreen hxc_l_tmp_call_result_n739 = hxc_caxecraft_app_AppScreen_startSelectedCampaign(hxc_l_screen);
                      hxc_l_screen = hxc_l_tmp_call_result_n739;
                      hxc_l_recapturedThisFrame = true;
                      DisableCursor();
                    }
                    hxc_string hxc_l_tmp_string_concat_result_n741 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
                    if (hxc_string_concat_ref((hxc_string){ (const uint8_t *)"caxecraft: campaign-level=", 26, true, NULL }, hxc_l_levelLabel, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n741) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    hxc_l_tmp_string_concat_left_owner_n258 = hxc_l_tmp_string_concat_result_n741;
                    hxc_string hxc_l_tmp_string_concat_result_n743 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
                    if (hxc_string_concat_ref(hxc_l_tmp_string_concat_left_owner_n258, (hxc_string){ (const uint8_t *)" generation=", 12, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n743) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    hxc_l_tmp_string_concat_left_owner_n259 = hxc_l_tmp_string_concat_result_n743;
                    hxc_string hxc_l_tmp_string_concat_left_borrow_result_n744 = hxc_l_tmp_string_concat_left_owner_n259;
                    hxc_string hxc_l_tmp_std_string_int_result_n746 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
                    if (hxc_string_from_int32(hxc_l_selected, hxc_default_allocator(), &hxc_l_tmp_std_string_int_result_n746) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    hxc_l_tmp_string_concat_right_owner_n260 = hxc_l_tmp_std_string_int_result_n746;
                    hxc_string hxc_l_tmp_string_concat_result_n748 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
                    if (hxc_string_concat_ref(hxc_l_tmp_string_concat_left_borrow_result_n744, hxc_l_tmp_string_concat_right_owner_n260, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n748) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    hxc_l_tmp_sys_println_string_argument_owner_n261 = hxc_l_tmp_string_concat_result_n748;
                    if (hxc_io_println(hxc_l_tmp_sys_println_string_argument_owner_n261) != HXC_STATUS_OK)
                    {
                      if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n261) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n260) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n259) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n258) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      if (hxc_array_ref_release(hxc_l_phases) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      hxc_enum_2c0e695d_destroy(&hxc_l_tmp_static_call_argument_0_owner_n246);
                      if (hxc_array_ref_release(hxc_l_initialActorPhases) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      hxc_enum_2c0e695d_destroy(&hxc_l_tmp_static_call_argument_0_owner_n94);
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
                      if (hxc_array_ref_release(hxc_l_tmp_object_storage_n84.hxc_stateStartTicks) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      if (hxc_array_ref_release(hxc_l_tmp_object_storage_n84.hxc_stateIds) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      if (hxc_array_ref_release(hxc_l_tmp_object_storage_n84.hxc_objectIds) != HXC_STATUS_OK)
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
                    if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n260) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n259) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n258) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    if (hxc_array_ref_release(hxc_l_phases) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    hxc_enum_2c0e695d_destroy(&hxc_l_tmp_static_call_argument_0_owner_n246);
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
          struct hxc_caxecraft_app_PreparedCampaignPortal *hxc_l_tmp_load_result_n750 = hxc_l_requestedCampaignPortal;
          hxc_l_gc_roots[396] = (const void *)hxc_l_tmp_load_result_n750;
          hxc_l_gc_roots[397] = (const void *)NULL;
          bool hxc_l_tmp_short_circuit_result_n262 = hxc_l_tmp_load_result_n750 != NULL;
          if (hxc_l_tmp_load_result_n750 != NULL)
          {
            hxc_l_tmp_short_circuit_result_n262 = !hxc_l_destinationPublished;
          }
          if (hxc_l_tmp_short_circuit_result_n262)
          {
            hxc_l_gc_roots[452] = (const void *)hxc_l_destination_ha1c14a301471;
            hxc_l_pendingCampaignLevel = hxc_l_destination_ha1c14a301471;
            struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_load_result_n754 = hxc_l_destination_ha1c14a301471;
            hxc_l_gc_roots[453] = (const void *)hxc_l_tmp_load_result_n754;
            if (hxc_l_tmp_load_result_n754 == NULL)
            {
              abort();
            }
            hxc_string hxc_l_tmp_class_field_load_result_n755 = (*hxc_l_tmp_load_result_n754).hxc_id;
            hxc_string hxc_l_tmp_string_assignment_replacement_owner_n263 = hxc_l_tmp_class_field_load_result_n755;
            if (hxc_string_retain(hxc_l_tmp_string_assignment_replacement_owner_n263) != HXC_STATUS_OK)
            {
              abort();
            }
            hxc_string hxc_l_tmp_string_assignment_replacement_owned_load_result_n756 = hxc_l_tmp_string_assignment_replacement_owner_n263;
            if (hxc_string_release(&hxc_l_pendingCampaignLabel) != HXC_STATUS_OK)
            {
              abort();
            }
            hxc_l_pendingCampaignLabel = hxc_l_tmp_string_assignment_replacement_owned_load_result_n756;
            hxc_l_loadingFramePresented = false;
            enum hxc_caxecraft_app_AppScreen hxc_l_tmp_call_result_n758 = hxc_caxecraft_app_AppScreen_beginLoading(hxc_l_screen);
            hxc_l_screen = hxc_l_tmp_call_result_n758;
            EnableCursor();
          }
          if (hxc_l_requestedFromAuthoredExit)
          {
            hxc_l_gc_roots[454] = (const void *)NULL;
            hxc_l_pendingCampaignLevel = NULL;
            hxc_string hxc_l_tmp_string_assignment_replacement_owner_n264 = (hxc_string){ (const uint8_t *)"", 0, true, NULL };
            if (hxc_string_retain(hxc_l_tmp_string_assignment_replacement_owner_n264) != HXC_STATUS_OK)
            {
              abort();
            }
            hxc_string hxc_l_tmp_string_assignment_replacement_owned_load_result_n760 = hxc_l_tmp_string_assignment_replacement_owner_n264;
            if (hxc_string_release(&hxc_l_pendingCampaignLabel) != HXC_STATUS_OK)
            {
              abort();
            }
            hxc_l_pendingCampaignLabel = hxc_l_tmp_string_assignment_replacement_owned_load_result_n760;
            hxc_l_loadingFramePresented = false;
            enum hxc_caxecraft_app_AppScreen hxc_l_tmp_call_result_n762 = hxc_caxecraft_app_AppScreen_finishLoading(hxc_l_screen);
            hxc_l_screen = hxc_l_tmp_call_result_n762;
            hxc_l_recapturedThisFrame = true;
            DisableCursor();
          }
        }
        struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n763 = hxc_l_levelView_hb85a9a956786;
        hxc_l_gc_roots[370] = (const void *)hxc_l_tmp_load_result_n763;
        if (hxc_l_tmp_load_result_n763 == NULL)
        {
          abort();
        }
        int32_t hxc_l_tmp_class_field_load_result_n764 = (*hxc_l_tmp_load_result_n763).hxc_enemyId;
        int32_t hxc_l_enemyActorId = hxc_l_tmp_class_field_load_result_n764;
        struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n765 = hxc_l_levelView_hb85a9a956786;
        hxc_l_gc_roots[371] = (const void *)hxc_l_tmp_load_result_n765;
        if (hxc_l_tmp_load_result_n765 == NULL)
        {
          abort();
        }
        struct hxc_caxecraft_scenario_ScenarioTransform hxc_l_tmp_class_field_load_result_n766 = (*hxc_l_tmp_load_result_n765).hxc_playerSpawn;
        struct hxc_caxecraft_scenario_ScenarioTransform hxc_l_spawnTransform = hxc_l_tmp_class_field_load_result_n766;
        if (hxc_l_quitPressed)
        {
          hxc_l_quit = true;
        }
        if (hxc_l_hotbarSelection >= 0)
        {
          struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_load_result_n769 = hxc_l_inventory;
          struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n771 = hxc_caxecraft_gameplay_Inventory_select(hxc_l_tmp_load_result_n769, hxc_l_hotbarSelection);
          hxc_l_inventory = hxc_l_tmp_call_result_n771;
        }
        if (hxc_l_hotbarCycle != 0)
        {
          struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_load_result_n773 = hxc_l_inventory;
          struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n775 = hxc_caxecraft_gameplay_Inventory_cycle(hxc_l_tmp_load_result_n773, hxc_l_hotbarCycle);
          hxc_l_inventory = hxc_l_tmp_call_result_n775;
        }
        bool hxc_l_conversationOwnedInput = false;
        struct hxc_optional_caxecraft_app_ConversationState hxc_l_openConversation = hxc_l_conversation;
        bool hxc_l_tmp_call_result_n778 = hxc_caxecraft_app_AppScreen_isPlaying(hxc_l_screen);
        bool hxc_l_tmp_short_circuit_result_n269 = hxc_l_tmp_call_result_n778;
        if (hxc_l_tmp_call_result_n778)
        {
          hxc_l_tmp_short_circuit_result_n269 = hxc_l_openConversation.hxc_has_value;
        }
        bool hxc_l_tmp_short_circuit_load_result_n780 = hxc_l_tmp_short_circuit_result_n269;
        bool hxc_l_tmp_short_circuit_result_n270 = hxc_l_tmp_short_circuit_load_result_n780;
        if (hxc_l_tmp_short_circuit_load_result_n780)
        {
          bool hxc_l_tmp_string_null_equality_result_n271 = hxc_l_activeDialogue.data != NULL;
          hxc_l_tmp_short_circuit_result_n270 = hxc_l_tmp_string_null_equality_result_n271;
        }
        if (hxc_l_tmp_short_circuit_result_n270)
        {
          hxc_l_conversationOwnedInput = true;
          struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n784 = hxc_l_levelView_hb85a9a956786;
          hxc_l_gc_roots[455] = (const void *)hxc_l_tmp_load_result_n784;
          if (hxc_l_tmp_load_result_n784 == NULL)
          {
            abort();
          }
          struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_class_field_load_result_n785 = (*hxc_l_tmp_load_result_n784).hxc_levelPresentation;
          hxc_l_gc_roots[456] = (const void *)hxc_l_tmp_class_field_load_result_n785;
          struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_levelView1_hf42e827075fc = hxc_l_tmp_class_field_load_result_n785;
          struct hxc_optional_caxecraft_app_ConversationState hxc_l_tmp_load_result_n786 = hxc_l_openConversation;
          if (!hxc_l_tmp_load_result_n786.hxc_has_value)
          {
            abort();
          }
          int32_t hxc_l_openConversation1 = hxc_l_tmp_load_result_n786.hxc_value.hxc_lineIndex;
          hxc_string hxc_l_line_h297d721ea865 = { 0 };
          switch (hxc_l_locale) {
            case 0:
              {
                hxc_l_line_h297d721ea865 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
                if (hxc_string_retain(hxc_l_line_h297d721ea865) != HXC_STATUS_OK)
                {
                  abort();
                }
                break;
              }
            case 1:
              {
                hxc_l_line_h297d721ea865 = (hxc_string){ (const uint8_t *)"es-mx", 5, true, NULL };
                if (hxc_string_retain(hxc_l_line_h297d721ea865) != HXC_STATUS_OK)
                {
                  abort();
                }
                break;
              }
            default:
              {
                hxc_l_line_h297d721ea865 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
                if (hxc_string_retain(hxc_l_line_h297d721ea865) != HXC_STATUS_OK)
                {
                  abort();
                }
                break;
              }
          }
          hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n790 = hxc_l_line_h297d721ea865;
          hxc_l_tmp_managed_flow_owner_n275 = hxc_l_tmp_managed_flow_carrier_move_result_n790;
          struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_load_result_n791 = hxc_l_levelView1_hf42e827075fc;
          hxc_l_gc_roots[457] = (const void *)hxc_l_tmp_load_result_n791;
          if (hxc_l_tmp_load_result_n791 == NULL)
          {
            abort();
          }
          hxc_string hxc_l_tmp_load_result_n792 = hxc_l_activeDialogue;
          int32_t hxc_l_tmp_load_result_n793 = hxc_l_openConversation1;
          hxc_string hxc_l_tmp_instance_call_result_n795 = hxc_caxecraft_content_RuntimeLevelPresentation_dialogueLine(hxc_l_tmp_load_result_n791, hxc_l_tmp_load_result_n792, hxc_l_tmp_load_result_n793, hxc_l_tmp_managed_flow_owner_n275);
          hxc_l_line_hd5cd12f85ec2 = hxc_l_tmp_instance_call_result_n795;
          struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n796 = hxc_l_levelView_hb85a9a956786;
          hxc_l_gc_roots[458] = (const void *)hxc_l_tmp_load_result_n796;
          if (hxc_l_tmp_load_result_n796 == NULL)
          {
            abort();
          }
          struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_class_field_load_result_n797 = (*hxc_l_tmp_load_result_n796).hxc_levelPresentation;
          hxc_l_gc_roots[459] = (const void *)hxc_l_tmp_class_field_load_result_n797;
          if (hxc_l_tmp_class_field_load_result_n797 == NULL)
          {
            abort();
          }
          int32_t hxc_l_tmp_instance_call_result_n799 = hxc_caxecraft_content_RuntimeLevelPresentation_dialogueLineCount(hxc_l_tmp_class_field_load_result_n797, hxc_l_activeDialogue);
          int32_t hxc_l_lineCount = hxc_l_tmp_instance_call_result_n799;
          struct hxc_optional_caxecraft_app_ConversationState hxc_l_tmp_load_result_n800 = hxc_l_openConversation;
          if (!hxc_l_tmp_load_result_n800.hxc_has_value)
          {
            abort();
          }
          hxc_string hxc_l_tmp_load_result_n802 = hxc_l_line_hd5cd12f85ec2;
          if (hxc_l_tmp_load_result_n802.data == NULL)
          {
            abort();
          }
          int32_t hxc_l_tmp_string_length_result_n803;
          if (hxc_string_haxe_length(hxc_l_tmp_load_result_n802, &hxc_l_tmp_string_length_result_n803) != HXC_STATUS_OK)
          {
            abort();
          }
          int32_t hxc_l_tmp_load_result_n804 = hxc_l_lineCount;
          double hxc_l_tmp_load_result_n805 = hxc_l_frameSeconds;
          bool hxc_l_tmp_load_result_n806 = hxc_l_interactPressed;
          struct hxc_caxecraft_app_ConversationAdvance hxc_l_tmp_call_result_n808 = hxc_caxecraft_app_ConversationFlow_advanceConversation(hxc_l_tmp_load_result_n800.hxc_value, hxc_l_tmp_string_length_result_n803, hxc_l_tmp_load_result_n804, hxc_f64_to_i32_saturating(hxc_l_tmp_load_result_n805 * 1000.0), hxc_l_tmp_load_result_n806, hxc_l_riseHeld);
          struct hxc_caxecraft_app_ConversationAdvance hxc_l_g_h50f50561320d = hxc_l_tmp_call_result_n808;
          switch (hxc_l_g_h50f50561320d.hxc_tag) {
            case hxc_caxecraft_app_ConversationAdvance_ConversationContinues:
              {
                if (hxc_l_g_h50f50561320d.hxc_tag != hxc_caxecraft_app_ConversationAdvance_ConversationContinues)
                {
                  abort();
                }
                struct hxc_caxecraft_app_ConversationState hxc_l_tmp_enum_payload_project_n811 = hxc_l_g_h50f50561320d.hxc_payload.hxc_ConversationContinues.hxc_state;
                struct hxc_caxecraft_app_ConversationState hxc_l_next = hxc_l_tmp_enum_payload_project_n811;
                hxc_l_conversation = (struct hxc_optional_caxecraft_app_ConversationState){ .hxc_has_value = true, .hxc_value = hxc_l_next };
                break;
              }
            case hxc_caxecraft_app_ConversationAdvance_ConversationCloses:
              {
                hxc_l_conversation = (struct hxc_optional_caxecraft_app_ConversationState){ .hxc_has_value = false };
                hxc_string hxc_l_tmp_string_assignment_replacement_owner_n280 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
                if (hxc_string_retain(hxc_l_tmp_string_assignment_replacement_owner_n280) != HXC_STATUS_OK)
                {
                  abort();
                }
                hxc_string hxc_l_tmp_string_assignment_replacement_owned_load_result_n814 = hxc_l_tmp_string_assignment_replacement_owner_n280;
                if (hxc_string_release(&hxc_l_activeDialogue) != HXC_STATUS_OK)
                {
                  abort();
                }
                hxc_l_activeDialogue = hxc_l_tmp_string_assignment_replacement_owned_load_result_n814;
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
          if (hxc_string_release(&hxc_l_line_hd5cd12f85ec2) != HXC_STATUS_OK)
          {
            abort();
          }
          if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n275) != HXC_STATUS_OK)
          {
            abort();
          }
        }
        else
        {
          bool hxc_l_tmp_call_result_n816 = hxc_caxecraft_app_AppScreen_isPlaying(hxc_l_screen);
          bool hxc_l_tmp_short_circuit_result_n281 = hxc_l_tmp_call_result_n816;
          if (hxc_l_tmp_call_result_n816)
          {
            hxc_l_tmp_short_circuit_result_n281 = hxc_l_interactPressed;
          }
          if (hxc_l_tmp_short_circuit_result_n281)
          {
            if (hxc_l_character.hxc_vitals.hxc_health <= 0)
            {
              struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n820 = hxc_l_session;
              hxc_l_gc_roots[460] = (const void *)hxc_l_tmp_load_result_n820;
              if (hxc_l_tmp_load_result_n820 == NULL)
              {
                abort();
              }
              struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n821 = hxc_l_session;
              hxc_l_gc_roots[461] = (const void *)hxc_l_tmp_load_result_n821;
              if (hxc_l_tmp_load_result_n821 == NULL)
              {
                abort();
              }
              size_t hxc_l_tmp_length_n823;
              const uint8_t *hxc_l_tmp_instance_call_result_n822 = hxc_caxecraft_domain_GameSession_worldView(hxc_l_tmp_load_result_n821, &hxc_l_tmp_length_n823);
              struct hxc_caxecraft_domain_CharacterBody hxc_l_tmp_call_result_n825 = hxc_caxecraft_app_CaxecraftApp_spawnPlayer(hxc_l_tmp_instance_call_result_n822, hxc_l_tmp_length_n823, hxc_l_spawnTransform);
              struct hxc_caxecraft_domain_LocalCharacterCommandResult hxc_l_tmp_instance_call_result_n826 = hxc_caxecraft_domain_GameSession_reviveLocalPlayerAt(hxc_l_tmp_load_result_n820, hxc_l_tmp_call_result_n825);
              struct hxc_caxecraft_domain_LocalCharacterCommandResult hxc_l_revival = hxc_l_tmp_instance_call_result_n826;
              hxc_l_character = hxc_l_revival.hxc_character;
              if (!hxc_l_revival.hxc_resolved)
              {
                hxc_l_quit = true;
              }
              else
              {
                struct hxc_caxecraft_app_RenderPosition hxc_l_tmp_call_result_n830 = hxc_caxecraft_app_SpawnCameraHeading_headingForSpawn(hxc_l_spawnTransform);
                struct hxc_caxecraft_app_RenderPosition hxc_l_revivedHeading = hxc_l_tmp_call_result_n830;
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
              struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n834 = hxc_l_session;
              hxc_l_gc_roots[462] = (const void *)hxc_l_tmp_load_result_n834;
              hxc_l_gc_roots[463] = (const void *)hxc_l_levelView_hb85a9a956786;
              struct hxc_caxecraft_app_CaxecraftApp_AvailableInteractionTarget hxc_l_tmp_call_result_n836 = hxc_caxecraft_app_CaxecraftApp_nearestAvailableInteraction(hxc_l_tmp_load_result_n834, hxc_l_levelView_hb85a9a956786);
              hxc_l_g_hb71d32c11dac = hxc_l_tmp_call_result_n836;
              switch (hxc_l_g_hb71d32c11dac.hxc_tag) {
                case hxc_caxecraft_app_CaxecraftApp_AvailableInteractionTarget_NoAvailableInteraction:
                  {
                    break;
                  }
                case hxc_caxecraft_app_CaxecraftApp_AvailableInteractionTarget_DialogueInteraction:
                  {
                    if (hxc_l_g_hb71d32c11dac.hxc_tag != hxc_caxecraft_app_CaxecraftApp_AvailableInteractionTarget_DialogueInteraction)
                    {
                      abort();
                    }
                    int32_t hxc_l_tmp_enum_payload_project_n839 = hxc_l_g_hb71d32c11dac.hxc_payload.hxc_DialogueInteraction.hxc_id;
                    int32_t hxc_l_id_hf294d93997bd = hxc_l_tmp_enum_payload_project_n839;
                    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n840 = hxc_l_session;
                    hxc_l_gc_roots[464] = (const void *)hxc_l_tmp_load_result_n840;
                    if (hxc_l_tmp_load_result_n840 == NULL)
                    {
                      abort();
                    }
                    bool hxc_l_tmp_instance_call_result_n842 = hxc_caxecraft_domain_GameSession_interactWithActor(hxc_l_tmp_load_result_n840, hxc_l_id_hf294d93997bd);
                    if (!hxc_l_tmp_instance_call_result_n842)
                    {
                      hxc_l_quit = true;
                    }
                    break;
                  }
                case hxc_caxecraft_app_CaxecraftApp_AvailableInteractionTarget_StatefulObjectInteraction:
                  {
                    if (hxc_l_g_hb71d32c11dac.hxc_tag != hxc_caxecraft_app_CaxecraftApp_AvailableInteractionTarget_StatefulObjectInteraction)
                    {
                      abort();
                    }
                    hxc_string hxc_l_tmp_enum_payload_project_n844 = hxc_l_g_hb71d32c11dac.hxc_payload.hxc_StatefulObjectInteraction.hxc_id;
                    hxc_l_id_h415df953ef33 = hxc_l_tmp_enum_payload_project_n844;
                    if (hxc_string_retain(hxc_l_id_h415df953ef33) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n845 = hxc_l_session;
                    hxc_l_gc_roots[465] = (const void *)hxc_l_tmp_load_result_n845;
                    if (hxc_l_tmp_load_result_n845 == NULL)
                    {
                      abort();
                    }
                    bool hxc_l_tmp_instance_call_result_n847 = hxc_caxecraft_domain_GameSession_interactWithStatefulObject(hxc_l_tmp_load_result_n845, hxc_l_id_h415df953ef33);
                    if (!hxc_l_tmp_instance_call_result_n847)
                    {
                      hxc_l_quit = true;
                    }
                    if (hxc_string_release(&hxc_l_id_h415df953ef33) != HXC_STATUS_OK)
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
              hxc_enum_2c0e695d_destroy(&hxc_l_g_hb71d32c11dac);
            }
          }
        }
        bool hxc_l_tmp_load_result_n848 = hxc_l_focused;
        bool hxc_l_tmp_short_circuit_result_n287 = !hxc_l_tmp_load_result_n848;
        if (!hxc_l_tmp_load_result_n848)
        {
          bool hxc_l_tmp_call_result_n850 = hxc_caxecraft_app_AppScreen_isPlaying(hxc_l_screen);
          hxc_l_tmp_short_circuit_result_n287 = hxc_l_tmp_call_result_n850;
        }
        bool hxc_l_tmp_short_circuit_load_result_n851 = hxc_l_tmp_short_circuit_result_n287;
        bool hxc_l_tmp_short_circuit_result_n288 = hxc_l_tmp_short_circuit_load_result_n851;
        if (hxc_l_tmp_short_circuit_load_result_n851)
        {
          hxc_l_tmp_short_circuit_result_n288 = hxc_l_screen != hxc_caxecraft_app_AppScreen_EditorTestPlay;
        }
        if (hxc_l_tmp_short_circuit_result_n288)
        {
          enum hxc_caxecraft_app_AppScreen hxc_l_tmp_call_result_n856 = hxc_caxecraft_app_AppScreen_loseFocus(hxc_l_screen);
          hxc_l_screen = hxc_l_tmp_call_result_n856;
          hxc_l_jumpQueued = false;
          hxc_l_accumulator = 0.0;
          hxc_l_resetMotionThisFrame = true;
          EnableCursor();
        }
        bool hxc_l_tmp_call_result_n858 = hxc_caxecraft_app_AppScreen_showsTitle(hxc_l_screen);
        bool hxc_l_tmp_short_circuit_result_n289 = !hxc_l_tmp_call_result_n858;
        if (!hxc_l_tmp_call_result_n858)
        {
          bool hxc_l_tmp_call_result_n860 = hxc_caxecraft_app_AppScreen_showsCampaignSelection(hxc_l_screen);
          hxc_l_tmp_short_circuit_result_n289 = !hxc_l_tmp_call_result_n860;
        }
        bool hxc_l_tmp_short_circuit_load_result_n861 = hxc_l_tmp_short_circuit_result_n289;
        bool hxc_l_tmp_short_circuit_result_n290 = hxc_l_tmp_short_circuit_load_result_n861;
        if (hxc_l_tmp_short_circuit_load_result_n861)
        {
          bool hxc_l_tmp_call_result_n863 = hxc_caxecraft_app_AppScreen_showsLoading(hxc_l_screen);
          hxc_l_tmp_short_circuit_result_n290 = !hxc_l_tmp_call_result_n863;
        }
        bool hxc_l_tmp_short_circuit_load_result_n864 = hxc_l_tmp_short_circuit_result_n290;
        bool hxc_l_tmp_short_circuit_result_n291 = hxc_l_tmp_short_circuit_load_result_n864;
        if (hxc_l_tmp_short_circuit_load_result_n864)
        {
          bool hxc_l_tmp_call_result_n866 = hxc_caxecraft_app_AppScreen_showsEditor(hxc_l_screen);
          hxc_l_tmp_short_circuit_result_n291 = !hxc_l_tmp_call_result_n866;
        }
        bool hxc_l_tmp_short_circuit_load_result_n867 = hxc_l_tmp_short_circuit_result_n291;
        bool hxc_l_tmp_short_circuit_result_n292 = hxc_l_tmp_short_circuit_load_result_n867;
        if (hxc_l_tmp_short_circuit_load_result_n867)
        {
          hxc_l_tmp_short_circuit_result_n292 = hxc_l_screen != hxc_caxecraft_app_AppScreen_EditorTestPlay;
        }
        bool hxc_l_tmp_short_circuit_load_result_n870 = hxc_l_tmp_short_circuit_result_n292;
        bool hxc_l_tmp_short_circuit_result_n293 = hxc_l_tmp_short_circuit_load_result_n870;
        if (hxc_l_tmp_short_circuit_load_result_n870)
        {
          hxc_l_tmp_short_circuit_result_n293 = hxc_l_focused;
        }
        bool hxc_l_tmp_short_circuit_load_result_n872 = hxc_l_tmp_short_circuit_result_n293;
        bool hxc_l_tmp_short_circuit_result_n294 = hxc_l_tmp_short_circuit_load_result_n872;
        if (hxc_l_tmp_short_circuit_load_result_n872)
        {
          hxc_l_tmp_short_circuit_result_n294 = hxc_l_character.hxc_vitals.hxc_health > 0;
        }
        bool hxc_l_tmp_short_circuit_load_result_n874 = hxc_l_tmp_short_circuit_result_n294;
        bool hxc_l_tmp_short_circuit_result_n295 = hxc_l_tmp_short_circuit_load_result_n874;
        if (hxc_l_tmp_short_circuit_load_result_n874)
        {
          hxc_l_tmp_short_circuit_result_n295 = hxc_l_pausePressed;
        }
        if (hxc_l_tmp_short_circuit_result_n295)
        {
          enum hxc_caxecraft_app_AppScreen hxc_l_tmp_call_result_n878 = hxc_caxecraft_app_AppScreen_togglePause(hxc_l_screen);
          hxc_l_screen = hxc_l_tmp_call_result_n878;
          hxc_l_accumulator = 0.0;
          hxc_l_resetMotionThisFrame = true;
          bool hxc_l_tmp_call_result_n880 = hxc_caxecraft_app_AppScreen_pausesSimulation(hxc_l_screen);
          if (hxc_l_tmp_call_result_n880)
          {
            hxc_l_jumpQueued = false;
          }
          bool hxc_l_tmp_call_result_n882 = hxc_caxecraft_app_AppScreen_capturesPointer(hxc_l_screen);
          if (hxc_l_tmp_call_result_n882)
          {
            DisableCursor();
          }
          else
          {
            EnableCursor();
          }
        }
        bool hxc_l_tmp_call_result_n884 = hxc_caxecraft_app_AppScreen_showsTitle(hxc_l_screen);
        bool hxc_l_tmp_short_circuit_result_n296 = !hxc_l_tmp_call_result_n884;
        if (!hxc_l_tmp_call_result_n884)
        {
          bool hxc_l_tmp_call_result_n886 = hxc_caxecraft_app_AppScreen_showsCampaignSelection(hxc_l_screen);
          hxc_l_tmp_short_circuit_result_n296 = !hxc_l_tmp_call_result_n886;
        }
        bool hxc_l_tmp_short_circuit_load_result_n887 = hxc_l_tmp_short_circuit_result_n296;
        bool hxc_l_tmp_short_circuit_result_n297 = hxc_l_tmp_short_circuit_load_result_n887;
        if (hxc_l_tmp_short_circuit_load_result_n887)
        {
          bool hxc_l_tmp_call_result_n889 = hxc_caxecraft_app_AppScreen_showsLoading(hxc_l_screen);
          hxc_l_tmp_short_circuit_result_n297 = !hxc_l_tmp_call_result_n889;
        }
        bool hxc_l_tmp_short_circuit_load_result_n890 = hxc_l_tmp_short_circuit_result_n297;
        bool hxc_l_tmp_short_circuit_result_n298 = hxc_l_tmp_short_circuit_load_result_n890;
        if (hxc_l_tmp_short_circuit_load_result_n890)
        {
          bool hxc_l_tmp_call_result_n892 = hxc_caxecraft_app_AppScreen_showsEditor(hxc_l_screen);
          hxc_l_tmp_short_circuit_result_n298 = !hxc_l_tmp_call_result_n892;
        }
        bool hxc_l_tmp_short_circuit_load_result_n893 = hxc_l_tmp_short_circuit_result_n298;
        bool hxc_l_tmp_short_circuit_result_n299 = hxc_l_tmp_short_circuit_load_result_n893;
        if (hxc_l_tmp_short_circuit_load_result_n893)
        {
          hxc_l_tmp_short_circuit_result_n299 = hxc_l_focused;
        }
        bool hxc_l_tmp_short_circuit_load_result_n895 = hxc_l_tmp_short_circuit_result_n299;
        bool hxc_l_tmp_short_circuit_result_n300 = hxc_l_tmp_short_circuit_load_result_n895;
        if (hxc_l_tmp_short_circuit_load_result_n895)
        {
          bool hxc_l_tmp_call_result_n897 = hxc_caxecraft_app_AppScreen_pausesSimulation(hxc_l_screen);
          hxc_l_tmp_short_circuit_result_n300 = hxc_l_tmp_call_result_n897;
        }
        bool hxc_l_tmp_short_circuit_load_result_n898 = hxc_l_tmp_short_circuit_result_n300;
        bool hxc_l_tmp_short_circuit_result_n301 = hxc_l_tmp_short_circuit_load_result_n898;
        if (hxc_l_tmp_short_circuit_load_result_n898)
        {
          hxc_l_tmp_short_circuit_result_n301 = hxc_l_capturePressed;
        }
        if (hxc_l_tmp_short_circuit_result_n301)
        {
          enum hxc_caxecraft_app_AppScreen hxc_l_tmp_call_result_n902 = hxc_caxecraft_app_AppScreen_recapture(hxc_l_screen);
          hxc_l_screen = hxc_l_tmp_call_result_n902;
          hxc_l_recapturedThisFrame = true;
          hxc_l_accumulator = 0.0;
          hxc_l_resetMotionThisFrame = true;
          DisableCursor();
        }
        bool hxc_l_tmp_call_result_n904 = hxc_caxecraft_app_AppScreen_showsTitle(hxc_l_screen);
        bool hxc_l_onTitle = hxc_l_tmp_call_result_n904;
        bool hxc_l_tmp_call_result_n906 = hxc_caxecraft_app_AppScreen_showsCampaignSelection(hxc_l_screen);
        bool hxc_l_onCampaignSelect = hxc_l_tmp_call_result_n906;
        bool hxc_l_tmp_call_result_n908 = hxc_caxecraft_app_AppScreen_showsLoading(hxc_l_screen);
        bool hxc_l_onLoading = hxc_l_tmp_call_result_n908;
        bool hxc_l_tmp_call_result_n910 = hxc_caxecraft_app_AppScreen_showsEditor(hxc_l_screen);
        bool hxc_l_onEditor = hxc_l_tmp_call_result_n910;
        bool hxc_l_tmp_call_result_n912 = hxc_caxecraft_app_AppScreen_pausesSimulation(hxc_l_screen);
        bool hxc_l_paused = hxc_l_tmp_call_result_n912;
        bool hxc_l_tmp_call_result_n914 = hxc_caxecraft_app_AppScreen_capturesPointer(hxc_l_screen);
        bool hxc_l_captured = hxc_l_tmp_call_result_n914;
        int32_t hxc_l_editorNavigationCommand = 0;
        hxc_l_editorActionForFrame = (struct hxc_caxecraft_app_EditorScreenAction){ .hxc_tag = hxc_caxecraft_app_EditorScreenAction_StayInEditor };
        if (hxc_l_onEditor)
        {
          if (hxc_l_self == NULL)
          {
            abort();
          }
          struct hxc_caxecraft_input_NavigationRepeater *hxc_l_tmp_owned_class_field_address_n917 = &(*hxc_l_self).hxc_editorNavigation;
          if (hxc_l_tmp_owned_class_field_address_n917 == NULL)
          {
            abort();
          }
          struct hxc_caxecraft_input_NavigationSample hxc_l_tmp_call_result_n918 = hxc_caxecraft_app_RaylibNavigationInput_samplePrimaryGamepad();
          int32_t hxc_l_tmp_instance_call_result_n920 = hxc_caxecraft_input_NavigationRepeater_advance(hxc_l_tmp_owned_class_field_address_n917, hxc_l_tmp_call_result_n918, hxc_l_frameSeconds);
          hxc_l_editorNavigationCommand = hxc_l_tmp_instance_call_result_n920;
        }
        else
        {
          if (hxc_l_self == NULL)
          {
            abort();
          }
          struct hxc_caxecraft_input_NavigationRepeater *hxc_l_tmp_owned_class_field_address_n921 = &(*hxc_l_self).hxc_editorNavigation;
          if (hxc_l_tmp_owned_class_field_address_n921 == NULL)
          {
            abort();
          }
          hxc_caxecraft_input_NavigationRepeater_release(hxc_l_tmp_owned_class_field_address_n921);
        }
        bool hxc_l_tmp_load_result_n922 = hxc_l_captured;
        bool hxc_l_tmp_short_circuit_result_n310 = hxc_l_tmp_load_result_n922;
        if (hxc_l_tmp_load_result_n922)
        {
          hxc_l_tmp_short_circuit_result_n310 = !hxc_l_conversationOwnedInput;
        }
        if (hxc_l_tmp_short_circuit_result_n310)
        {
          if (hxc_l_cameraTogglePressed)
          {
            enum hxc_caxecraft_domain_PlayerCameraMode hxc_l_tmp_call_result_n927 = hxc_caxecraft_domain_PlayerCamera_togglePlayerCamera(hxc_l_cameraMode);
            hxc_l_cameraMode = hxc_l_tmp_call_result_n927;
          }
          double hxc_l_yawDelta = hxc_l_lookYaw;
          if (hxc_l_yawDelta > 0.25)
          {
            hxc_l_yawDelta = 0.25;
          }
          if (hxc_l_yawDelta < -0.25)
          {
            hxc_l_yawDelta = -0.25;
          }
          double hxc_l_tmp_load_result_n931 = hxc_l_lookX;
          double hxc_l_tmp_load_result_n932 = hxc_l_yawDelta;
          double hxc_l_candidateX = hxc_l_tmp_load_result_n931 + hxc_l_tmp_load_result_n932 * hxc_l_lookZ;
          double hxc_l_tmp_load_result_n934 = hxc_l_lookZ;
          double hxc_l_tmp_load_result_n935 = hxc_l_yawDelta;
          double hxc_l_candidateZ = hxc_l_tmp_load_result_n934 - hxc_l_tmp_load_result_n935 * hxc_l_lookX;
          double hxc_l_tmp_load_result_n937 = hxc_l_candidateX;
          double hxc_l_tmp_load_result_n938 = hxc_l_candidateX;
          double hxc_l_tmp_load_result_n939 = hxc_l_candidateZ;
          double hxc_l_lengthSquared = hxc_l_tmp_load_result_n937 * hxc_l_tmp_load_result_n938 + hxc_l_tmp_load_result_n939 * hxc_l_candidateZ;
          double hxc_l_normalize = 1.5 - 0.5 * hxc_l_lengthSquared;
          double hxc_l_tmp_load_result_n942 = hxc_l_candidateX;
          hxc_l_lookX = hxc_l_tmp_load_result_n942 * hxc_l_normalize;
          double hxc_l_tmp_load_result_n944 = hxc_l_candidateZ;
          hxc_l_lookZ = hxc_l_tmp_load_result_n944 * hxc_l_normalize;
          double hxc_l_tmp_compound_load_result_n946 = hxc_l_lookY;
          hxc_l_lookY = hxc_l_tmp_compound_load_result_n946 + hxc_l_lookPitch;
          if (hxc_l_lookY > 0.90)
          {
            hxc_l_lookY = 0.90;
          }
          if (hxc_l_lookY < -0.90)
          {
            hxc_l_lookY = -0.90;
          }
          if (hxc_l_jumpPressed)
          {
            hxc_l_jumpQueued = true;
          }
        }
        bool hxc_l_tmp_load_result_n951 = hxc_l_captured;
        bool hxc_l_tmp_short_circuit_result_n316 = hxc_l_tmp_load_result_n951;
        if (hxc_l_tmp_load_result_n951)
        {
          hxc_l_tmp_short_circuit_result_n316 = !hxc_l_conversationOwnedInput;
        }
        bool hxc_l_tmp_short_circuit_load_result_n953 = hxc_l_tmp_short_circuit_result_n316;
        bool hxc_l_tmp_short_circuit_result_n317 = hxc_l_tmp_short_circuit_load_result_n953;
        if (hxc_l_tmp_short_circuit_load_result_n953)
        {
          hxc_l_tmp_short_circuit_result_n317 = !hxc_l_recapturedThisFrame;
        }
        bool hxc_l_tmp_short_circuit_load_result_n955 = hxc_l_tmp_short_circuit_result_n317;
        bool hxc_l_tmp_short_circuit_result_n318 = hxc_l_tmp_short_circuit_load_result_n955;
        if (hxc_l_tmp_short_circuit_load_result_n955)
        {
          hxc_l_tmp_short_circuit_result_n318 = hxc_l_primaryPressed;
        }
        bool hxc_l_tmp_short_circuit_load_result_n957 = hxc_l_tmp_short_circuit_result_n318;
        bool hxc_l_tmp_short_circuit_result_n319 = hxc_l_tmp_short_circuit_load_result_n957;
        if (hxc_l_tmp_short_circuit_load_result_n957)
        {
          hxc_l_tmp_short_circuit_result_n319 = hxc_l_selectedMode == 1;
        }
        bool hxc_l_tmp_short_circuit_load_result_n959 = hxc_l_tmp_short_circuit_result_n319;
        bool hxc_l_tmp_short_circuit_result_n320 = hxc_l_tmp_short_circuit_load_result_n959;
        if (hxc_l_tmp_short_circuit_load_result_n959)
        {
          bool hxc_l_tmp_call_result_n962 = hxc_caxecraft_gameplay_Inventory_selectedIs(hxc_l_inventory, hxc_caxecraft_gameplay_ItemKind_CopperSword);
          hxc_l_tmp_short_circuit_result_n320 = hxc_l_tmp_call_result_n962;
        }
        if (hxc_l_tmp_short_circuit_result_n320)
        {
          hxc_l_swordQueued = true;
        }
        bool hxc_l_tmp_load_result_n964 = hxc_l_paused;
        bool hxc_l_tmp_short_circuit_result_n321 = !hxc_l_tmp_load_result_n964;
        if (!hxc_l_tmp_load_result_n964)
        {
          hxc_l_tmp_short_circuit_result_n321 = !hxc_l_conversationOwnedInput;
        }
        if (hxc_l_tmp_short_circuit_result_n321)
        {
          double hxc_l_tmp_compound_load_result_n967 = hxc_l_accumulator;
          hxc_l_accumulator = hxc_l_tmp_compound_load_result_n967 + hxc_l_frameSeconds;
        }
        while (1)
        {
          bool hxc_l_tmp_call_result_n970 = hxc_caxecraft_app_AppScreen_pausesSimulation(hxc_l_screen);
          bool hxc_l_tmp_short_circuit_result_n322 = !hxc_l_tmp_call_result_n970;
          if (!hxc_l_tmp_call_result_n970)
          {
            hxc_l_tmp_short_circuit_result_n322 = !hxc_l_conversation.hxc_has_value;
          }
          bool hxc_l_tmp_short_circuit_load_result_n972 = hxc_l_tmp_short_circuit_result_n322;
          bool hxc_l_tmp_short_circuit_result_n323 = hxc_l_tmp_short_circuit_load_result_n972;
          if (hxc_l_tmp_short_circuit_load_result_n972)
          {
            hxc_l_tmp_short_circuit_result_n323 = hxc_l_accumulator >= 0.05;
          }
          if (!hxc_l_tmp_short_circuit_result_n323)
          {
            break;
          }
          double hxc_l_tmp_load_result_n975 = hxc_l_moveForward;
          double hxc_l_tmp_load_result_n976 = hxc_l_lookX;
          double hxc_l_tmp_load_result_n977 = hxc_l_moveRight;
          double hxc_l_moveX = hxc_l_tmp_load_result_n975 * hxc_l_tmp_load_result_n976 - hxc_l_tmp_load_result_n977 * hxc_l_lookZ;
          double hxc_l_tmp_load_result_n979 = hxc_l_moveForward;
          double hxc_l_tmp_load_result_n980 = hxc_l_lookZ;
          double hxc_l_tmp_load_result_n981 = hxc_l_moveRight;
          double hxc_l_moveZ = hxc_l_tmp_load_result_n979 * hxc_l_tmp_load_result_n980 + hxc_l_tmp_load_result_n981 * hxc_l_lookX;
          double hxc_l_tmp_load_result_n983 = hxc_l_moveForward;
          bool hxc_l_tmp_short_circuit_result_n326 = hxc_l_tmp_load_result_n983 != 0.0;
          if (hxc_l_tmp_load_result_n983 != 0.0)
          {
            hxc_l_tmp_short_circuit_result_n326 = hxc_l_moveRight != 0.0;
          }
          if (hxc_l_tmp_short_circuit_result_n326)
          {
            hxc_l_moveX = hxc_l_moveX * 0.7071067811865476;
            hxc_l_moveZ = hxc_l_moveZ * 0.7071067811865476;
          }
          int32_t hxc_l_tmp_load_result_n988 = hxc_l_selectedMode;
          int32_t hxc_l_tmp_conditional_result_n328 = 0;
          if (hxc_l_tmp_load_result_n988 == 1)
          {
            hxc_l_tmp_conditional_result_n328 = 1;
          }
          else
          {
            hxc_l_tmp_conditional_result_n328 = 0;
          }
          int32_t hxc_l_damagePolicy = hxc_l_tmp_conditional_result_n328;
          bool hxc_l_tmp_load_result_n990 = hxc_l_jumpQueued;
          bool hxc_l_tmp_short_circuit_result_n330 = hxc_l_tmp_load_result_n990;
          if (!hxc_l_tmp_load_result_n990)
          {
            int32_t hxc_l_tmp_record_field_load_result_n991 = hxc_l_character.hxc_aquatic.hxc_medium;
            bool hxc_l_tmp_short_circuit_result_n331 = hxc_l_tmp_record_field_load_result_n991 != 0;
            if (hxc_l_tmp_record_field_load_result_n991 != 0)
            {
              hxc_l_tmp_short_circuit_result_n331 = hxc_l_riseHeld;
            }
            hxc_l_tmp_short_circuit_result_n330 = hxc_l_tmp_short_circuit_result_n331;
          }
          bool hxc_l_rising = hxc_l_tmp_short_circuit_result_n330;
          struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n995 = hxc_l_session;
          hxc_l_gc_roots[468] = (const void *)hxc_l_tmp_load_result_n995;
          if (hxc_l_tmp_load_result_n995 == NULL)
          {
            abort();
          }
          double hxc_l_tmp_load_result_n996 = hxc_l_moveX;
          double hxc_l_tmp_load_result_n997 = hxc_l_moveZ;
          bool hxc_l_tmp_load_result_n998 = hxc_l_rising;
          struct hxc_caxecraft_domain_CharacterIntent hxc_l_tmp_call_result_n1000 = hxc_caxecraft_domain_Aquatics_input(hxc_l_tmp_load_result_n996, hxc_l_tmp_load_result_n997, hxc_l_tmp_load_result_n998, hxc_l_descendHeld);
          struct hxc_caxecraft_domain_GameTickResult hxc_l_tmp_instance_call_result_n1003 = hxc_caxecraft_domain_GameSession_tick(hxc_l_tmp_load_result_n995, (struct hxc_caxecraft_domain_GameTickInput){ .hxc_damagePolicy = hxc_l_damagePolicy, .hxc_intent = hxc_l_tmp_call_result_n1000, .hxc_waterUpdateBudget = 64 });
          hxc_l_gameTick = hxc_l_tmp_instance_call_result_n1003;
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
              struct hxc_caxecraft_app_CaxecraftEditorScreen *hxc_l_tmp_load_result_n1009 = hxc_l_editorScreen;
              hxc_l_gc_roots[469] = (const void *)hxc_l_tmp_load_result_n1009;
              if (hxc_l_tmp_load_result_n1009 == NULL)
              {
                abort();
              }
              struct hxc_optional_caxecraft_scenario_FlowTickResult hxc_l_tmp_load_result_n1010 = hxc_l_flow;
              if (!hxc_l_tmp_load_result_n1010.hxc_has_value)
              {
                abort();
              }
              hxc_caxecraft_app_CaxecraftEditorScreen_observeFlowTrace(hxc_l_tmp_load_result_n1009, hxc_l_tmp_load_result_n1010.hxc_value.hxc_trace);
            }
            struct hxc_optional_caxecraft_scenario_FlowTickResult hxc_l_tmp_load_result_n1013 = hxc_l_flow;
            if (!hxc_l_tmp_load_result_n1013.hxc_has_value)
            {
              abort();
            }
            int32_t hxc_l_tmp_array_length_result_n1016;
            if (hxc_array_ref_length(hxc_l_tmp_load_result_n1013.hxc_value.hxc_diagnostics, &hxc_l_tmp_array_length_result_n1016) != HXC_STATUS_OK)
            {
              abort();
            }
            if (hxc_l_tmp_array_length_result_n1016 == 0)
            {
              int32_t hxc_l_g_hee9bf66e2f69 = 0;
              struct hxc_optional_caxecraft_scenario_FlowTickResult hxc_l_tmp_load_result_n1017 = hxc_l_flow;
              if (!hxc_l_tmp_load_result_n1017.hxc_has_value)
              {
                abort();
              }
              hxc_l_g1_h729617e1d868 = hxc_l_tmp_load_result_n1017.hxc_value.hxc_presentation;
              if (hxc_array_ref_retain(hxc_l_g1_h729617e1d868) != HXC_STATUS_OK)
              {
                abort();
              }
              while (1)
              {
                int32_t hxc_l_tmp_load_result_n1020 = hxc_l_g_hee9bf66e2f69;
                int32_t hxc_l_tmp_array_length_result_n1022;
                if (hxc_array_ref_length(hxc_l_g1_h729617e1d868, &hxc_l_tmp_array_length_result_n1022) != HXC_STATUS_OK)
                {
                  abort();
                }
                if (!(hxc_l_tmp_load_result_n1020 < hxc_l_tmp_array_length_result_n1022))
                {
                  break;
                }
                struct hxc_array_ref *hxc_l_tmp_load_result_n1023 = hxc_l_g1_h729617e1d868;
                struct hxc_caxecraft_scenario_FlowPresentationEvent hxc_l_tmp_array_get_result_n1025;
                if (hxc_array_ref_get_copy(hxc_l_tmp_load_result_n1023, (size_t)hxc_l_g_hee9bf66e2f69, &hxc_l_tmp_array_get_result_n1025) != HXC_STATUS_OK)
                {
                  abort();
                }
                hxc_l_tmp_array_element_owner_n337 = hxc_l_tmp_array_get_result_n1025;
                hxc_l_event_h3221db1879e6 = hxc_l_tmp_array_element_owner_n337;
                if (hxc_enum_147eda7c_retain(&hxc_l_event_h3221db1879e6) != HXC_STATUS_OK)
                {
                  abort();
                }
                hxc_l_g_hee9bf66e2f69 = hxc_i32_add_wrapping(hxc_l_g_hee9bf66e2f69, 1);
                switch (hxc_l_event_h3221db1879e6.hxc_tag) {
                  case hxc_caxecraft_scenario_FlowPresentationEvent_DialogueRequested:
                    {
                      if (hxc_l_event_h3221db1879e6.hxc_tag != hxc_caxecraft_scenario_FlowPresentationEvent_DialogueRequested)
                      {
                        abort();
                      }
                      hxc_string hxc_l_tmp_enum_payload_project_n1030 = hxc_l_event_h3221db1879e6.hxc_payload.hxc_DialogueRequested.hxc_id;
                      hxc_l_id_hd70653f4f19c = hxc_l_tmp_enum_payload_project_n1030;
                      if (hxc_string_retain(hxc_l_id_hd70653f4f19c) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      hxc_string hxc_l_tmp_string_assignment_replacement_owner_n339 = hxc_l_id_hd70653f4f19c;
                      if (hxc_string_retain(hxc_l_tmp_string_assignment_replacement_owner_n339) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      hxc_string hxc_l_tmp_string_assignment_replacement_owned_load_result_n1032 = hxc_l_tmp_string_assignment_replacement_owner_n339;
                      if (hxc_string_release(&hxc_l_activeDialogue) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      hxc_l_activeDialogue = hxc_l_tmp_string_assignment_replacement_owned_load_result_n1032;
                      struct hxc_caxecraft_app_ConversationState hxc_l_tmp_call_result_n1033 = hxc_caxecraft_app_ConversationFlow_beginConversation();
                      hxc_l_conversation = (struct hxc_optional_caxecraft_app_ConversationState){ .hxc_has_value = true, .hxc_value = hxc_l_tmp_call_result_n1033 };
                      if (hxc_string_release(&hxc_l_id_hd70653f4f19c) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      break;
                    }
                  case hxc_caxecraft_scenario_FlowPresentationEvent_JournalAdded:
                    {
                      if (hxc_l_event_h3221db1879e6.hxc_tag != hxc_caxecraft_scenario_FlowPresentationEvent_JournalAdded)
                      {
                        abort();
                      }
                      hxc_string hxc_l_tmp_enum_payload_project_n1036 = hxc_l_event_h3221db1879e6.hxc_payload.hxc_JournalAdded.hxc_id;
                      hxc_l_id_hd83d361b952f = hxc_l_tmp_enum_payload_project_n1036;
                      if (hxc_string_retain(hxc_l_id_hd83d361b952f) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      hxc_string hxc_l_tmp_string_assignment_replacement_owner_n341 = hxc_l_id_hd83d361b952f;
                      if (hxc_string_retain(hxc_l_tmp_string_assignment_replacement_owner_n341) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      hxc_string hxc_l_tmp_string_assignment_replacement_owned_load_result_n1038 = hxc_l_tmp_string_assignment_replacement_owner_n341;
                      if (hxc_string_release(&hxc_l_latestJournalId) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      hxc_l_latestJournalId = hxc_l_tmp_string_assignment_replacement_owned_load_result_n1038;
                      if (hxc_string_release(&hxc_l_id_hd83d361b952f) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      break;
                    }
                  case hxc_caxecraft_scenario_FlowPresentationEvent_CampaignExitRequested:
                    {
                      if (hxc_l_event_h3221db1879e6.hxc_tag != hxc_caxecraft_scenario_FlowPresentationEvent_CampaignExitRequested)
                      {
                        abort();
                      }
                      hxc_string hxc_l_tmp_enum_payload_project_n1040 = hxc_l_event_h3221db1879e6.hxc_payload.hxc_CampaignExitRequested.hxc_exit;
                      hxc_l_exit = hxc_l_tmp_enum_payload_project_n1040;
                      if (hxc_string_retain(hxc_l_exit) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      int32_t hxc_l_tmp_load_result_n1041 = hxc_l_selectedMode;
                      bool hxc_l_tmp_short_circuit_result_n343 = hxc_l_tmp_load_result_n1041 != 1;
                      if (!(hxc_l_tmp_load_result_n1041 != 1))
                      {
                        bool hxc_l_tmp_call_result_n1043 = hxc_caxecraft_app_AppScreen_acceptsCampaignExit(hxc_l_screen);
                        hxc_l_tmp_short_circuit_result_n343 = !hxc_l_tmp_call_result_n1043;
                      }
                      if (hxc_l_tmp_short_circuit_result_n343)
                      {
                        hxc_string hxc_l_tmp_string_concat_result_n1046 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
                        if (hxc_string_concat_ref((hxc_string){ (const uint8_t *)"caxecraft: ignored campaign exit outside active Adventure play: ", 64, true, NULL }, hxc_l_exit, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n1046) != HXC_STATUS_OK)
                        {
                          abort();
                        }
                        hxc_l_tmp_sys_println_string_argument_owner_n344 = hxc_l_tmp_string_concat_result_n1046;
                        if (hxc_io_println(hxc_l_tmp_sys_println_string_argument_owner_n344) != HXC_STATUS_OK)
                        {
                          if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n344) != HXC_STATUS_OK)
                          {
                            abort();
                          }
                          if (hxc_string_release(&hxc_l_exit) != HXC_STATUS_OK)
                          {
                            abort();
                          }
                          hxc_enum_147eda7c_destroy(&hxc_l_event_h3221db1879e6);
                          hxc_array_ce94fc36_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n337);
                          if (hxc_array_ref_release(hxc_l_g1_h729617e1d868) != HXC_STATUS_OK)
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
                          hxc_enum_2c0e695d_destroy(&hxc_l_tmp_static_call_argument_0_owner_n94);
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
                          if (hxc_array_ref_release(hxc_l_tmp_object_storage_n84.hxc_stateStartTicks) != HXC_STATUS_OK)
                          {
                            abort();
                          }
                          if (hxc_array_ref_release(hxc_l_tmp_object_storage_n84.hxc_stateIds) != HXC_STATUS_OK)
                          {
                            abort();
                          }
                          if (hxc_array_ref_release(hxc_l_tmp_object_storage_n84.hxc_objectIds) != HXC_STATUS_OK)
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
                        if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n344) != HXC_STATUS_OK)
                        {
                          abort();
                        }
                      }
                      else
                      {
                        hxc_l_gc_roots[470] = (const void *)hxc_l_campaign;
                        struct hxc_caxecraft_content_CampaignManifest *hxc_l_selectedCampaign_ha1c4d2e73071 = hxc_l_campaign;
                        hxc_l_gc_roots[471] = (const void *)hxc_l_campaignLevel;
                        struct hxc_caxecraft_content_CampaignLevel *hxc_l_sourceLevel_h0c6e8660e234 = hxc_l_campaignLevel;
                        struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n1050 = hxc_l_selectedCampaign_ha1c4d2e73071;
                        hxc_l_gc_roots[472] = (const void *)hxc_l_tmp_load_result_n1050;
                        hxc_l_gc_roots[473] = (const void *)NULL;
                        bool hxc_l_tmp_short_circuit_result_n347 = hxc_l_tmp_load_result_n1050 == NULL;
                        if (!(hxc_l_tmp_load_result_n1050 == NULL))
                        {
                          hxc_l_gc_roots[474] = (const void *)hxc_l_sourceLevel_h0c6e8660e234;
                          hxc_l_gc_roots[475] = (const void *)NULL;
                          hxc_l_tmp_short_circuit_result_n347 = hxc_l_sourceLevel_h0c6e8660e234 == NULL;
                        }
                        if (hxc_l_tmp_short_circuit_result_n347)
                        {
                          hxc_string hxc_l_tmp_string_concat_result_n1054 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
                          if (hxc_string_concat_ref((hxc_string){ (const uint8_t *)"caxecraft: ignored campaign exit without an active campaign: ", 61, true, NULL }, hxc_l_exit, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n1054) != HXC_STATUS_OK)
                          {
                            abort();
                          }
                          hxc_l_tmp_sys_println_string_argument_owner_n348 = hxc_l_tmp_string_concat_result_n1054;
                          if (hxc_io_println(hxc_l_tmp_sys_println_string_argument_owner_n348) != HXC_STATUS_OK)
                          {
                            if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n348) != HXC_STATUS_OK)
                            {
                              abort();
                            }
                            if (hxc_string_release(&hxc_l_exit) != HXC_STATUS_OK)
                            {
                              abort();
                            }
                            hxc_enum_147eda7c_destroy(&hxc_l_event_h3221db1879e6);
                            hxc_array_ce94fc36_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n337);
                            if (hxc_array_ref_release(hxc_l_g1_h729617e1d868) != HXC_STATUS_OK)
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
                            hxc_enum_2c0e695d_destroy(&hxc_l_tmp_static_call_argument_0_owner_n94);
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
                            if (hxc_array_ref_release(hxc_l_tmp_object_storage_n84.hxc_stateStartTicks) != HXC_STATUS_OK)
                            {
                              abort();
                            }
                            if (hxc_array_ref_release(hxc_l_tmp_object_storage_n84.hxc_stateIds) != HXC_STATUS_OK)
                            {
                              abort();
                            }
                            if (hxc_array_ref_release(hxc_l_tmp_object_storage_n84.hxc_objectIds) != HXC_STATUS_OK)
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
                          if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n348) != HXC_STATUS_OK)
                          {
                            abort();
                          }
                        }
                        else
                        {
                          struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n1056 = hxc_l_selectedCampaign_ha1c4d2e73071;
                          hxc_l_gc_roots[476] = (const void *)hxc_l_tmp_load_result_n1056;
                          if (hxc_l_tmp_load_result_n1056 == NULL)
                          {
                            abort();
                          }
                          struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_load_result_n1057 = hxc_l_sourceLevel_h0c6e8660e234;
                          hxc_l_gc_roots[477] = (const void *)hxc_l_tmp_load_result_n1057;
                          if (hxc_l_tmp_load_result_n1057 == NULL)
                          {
                            abort();
                          }
                          hxc_string hxc_l_tmp_class_field_load_result_n1058 = (*hxc_l_tmp_load_result_n1057).hxc_id;
                          struct hxc_caxecraft_content_CampaignTransition *hxc_l_tmp_instance_call_result_n1060 = hxc_caxecraft_content_CampaignManifest_transitionForRequest(hxc_l_tmp_load_result_n1056, hxc_l_tmp_class_field_load_result_n1058, hxc_l_exit);
                          hxc_l_gc_roots[478] = (const void *)hxc_l_tmp_instance_call_result_n1060;
                          struct hxc_caxecraft_content_CampaignTransition *hxc_l_transition_h9c76253dd32a = hxc_l_tmp_instance_call_result_n1060;
                          hxc_l_gc_roots[479] = (const void *)hxc_l_transition_h9c76253dd32a;
                          hxc_l_gc_roots[480] = (const void *)NULL;
                          if (hxc_l_transition_h9c76253dd32a == NULL)
                          {
                            hxc_string hxc_l_tmp_string_concat_result_n1063 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
                            if (hxc_string_concat_ref((hxc_string){ (const uint8_t *)"caxecraft: ignored unknown campaign exit ", 41, true, NULL }, hxc_l_exit, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n1063) != HXC_STATUS_OK)
                            {
                              abort();
                            }
                            hxc_l_tmp_string_concat_left_owner_n350 = hxc_l_tmp_string_concat_result_n1063;
                            hxc_string hxc_l_tmp_string_concat_result_n1065 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
                            if (hxc_string_concat_ref(hxc_l_tmp_string_concat_left_owner_n350, (hxc_string){ (const uint8_t *)" from ", 6, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n1065) != HXC_STATUS_OK)
                            {
                              abort();
                            }
                            hxc_l_tmp_string_concat_left_owner_n351 = hxc_l_tmp_string_concat_result_n1065;
                            hxc_string hxc_l_tmp_string_concat_left_borrow_result_n1066 = hxc_l_tmp_string_concat_left_owner_n351;
                            struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_load_result_n1067 = hxc_l_sourceLevel_h0c6e8660e234;
                            hxc_l_gc_roots[481] = (const void *)hxc_l_tmp_load_result_n1067;
                            if (hxc_l_tmp_load_result_n1067 == NULL)
                            {
                              abort();
                            }
                            hxc_string hxc_l_tmp_class_field_load_result_n1068 = (*hxc_l_tmp_load_result_n1067).hxc_id;
                            hxc_string hxc_l_tmp_string_concat_result_n1069 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
                            if (hxc_string_concat_ref(hxc_l_tmp_string_concat_left_borrow_result_n1066, hxc_l_tmp_class_field_load_result_n1068, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n1069) != HXC_STATUS_OK)
                            {
                              abort();
                            }
                            hxc_l_tmp_sys_println_string_argument_owner_n352 = hxc_l_tmp_string_concat_result_n1069;
                            if (hxc_io_println(hxc_l_tmp_sys_println_string_argument_owner_n352) != HXC_STATUS_OK)
                            {
                              if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n352) != HXC_STATUS_OK)
                              {
                                abort();
                              }
                              if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n351) != HXC_STATUS_OK)
                              {
                                abort();
                              }
                              if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n350) != HXC_STATUS_OK)
                              {
                                abort();
                              }
                              if (hxc_string_release(&hxc_l_exit) != HXC_STATUS_OK)
                              {
                                abort();
                              }
                              hxc_enum_147eda7c_destroy(&hxc_l_event_h3221db1879e6);
                              hxc_array_ce94fc36_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n337);
                              if (hxc_array_ref_release(hxc_l_g1_h729617e1d868) != HXC_STATUS_OK)
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
                              hxc_enum_2c0e695d_destroy(&hxc_l_tmp_static_call_argument_0_owner_n94);
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
                              if (hxc_array_ref_release(hxc_l_tmp_object_storage_n84.hxc_stateStartTicks) != HXC_STATUS_OK)
                              {
                                abort();
                              }
                              if (hxc_array_ref_release(hxc_l_tmp_object_storage_n84.hxc_stateIds) != HXC_STATUS_OK)
                              {
                                abort();
                              }
                              if (hxc_array_ref_release(hxc_l_tmp_object_storage_n84.hxc_objectIds) != HXC_STATUS_OK)
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
                            if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n352) != HXC_STATUS_OK)
                            {
                              abort();
                            }
                            if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n351) != HXC_STATUS_OK)
                            {
                              abort();
                            }
                            if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n350) != HXC_STATUS_OK)
                            {
                              abort();
                            }
                          }
                          else
                          {
                            struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n1071 = hxc_l_selectedCampaign_ha1c4d2e73071;
                            hxc_l_gc_roots[482] = (const void *)hxc_l_tmp_load_result_n1071;
                            if (hxc_l_tmp_load_result_n1071 == NULL)
                            {
                              abort();
                            }
                            struct hxc_caxecraft_content_CampaignTransition *hxc_l_tmp_load_result_n1072 = hxc_l_transition_h9c76253dd32a;
                            hxc_l_gc_roots[483] = (const void *)hxc_l_tmp_load_result_n1072;
                            if (hxc_l_tmp_load_result_n1072 == NULL)
                            {
                              abort();
                            }
                            hxc_string hxc_l_tmp_class_field_load_result_n1073 = (*hxc_l_tmp_load_result_n1072).hxc_destinationLevel;
                            struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_instance_call_result_n1074 = hxc_caxecraft_content_CampaignManifest_level(hxc_l_tmp_load_result_n1071, hxc_l_tmp_class_field_load_result_n1073);
                            hxc_l_gc_roots[484] = (const void *)hxc_l_tmp_instance_call_result_n1074;
                            struct hxc_caxecraft_content_CampaignLevel *hxc_l_nextLevel = hxc_l_tmp_instance_call_result_n1074;
                            hxc_l_gc_roots[485] = (const void *)hxc_l_nextLevel;
                            hxc_l_gc_roots[486] = (const void *)NULL;
                            if (hxc_l_nextLevel == NULL)
                            {
                              if (hxc_io_println((hxc_string){ (const uint8_t *)"caxecraft: campaign destination disappeared after manifest validation", 69, true, NULL }) != HXC_STATUS_OK)
                              {
                                abort();
                              }
                            }
                            else
                            {
                              hxc_l_gc_roots[487] = (const void *)hxc_l_preparedPortal;
                              struct hxc_caxecraft_app_PreparedCampaignPortal *hxc_l_portal_hc129b8cc889b = hxc_l_preparedPortal;
                              struct hxc_caxecraft_app_PreparedCampaignPortal *hxc_l_tmp_load_result_n1077 = hxc_l_portal_hc129b8cc889b;
                              hxc_l_gc_roots[488] = (const void *)hxc_l_tmp_load_result_n1077;
                              hxc_l_gc_roots[489] = (const void *)NULL;
                              bool hxc_l_tmp_short_circuit_result_n355 = hxc_l_tmp_load_result_n1077 != NULL;
                              if (hxc_l_tmp_load_result_n1077 != NULL)
                              {
                                struct hxc_caxecraft_app_PreparedCampaignPortal *hxc_l_tmp_load_result_n1078 = hxc_l_portal_hc129b8cc889b;
                                hxc_l_gc_roots[490] = (const void *)hxc_l_tmp_load_result_n1078;
                                if (hxc_l_tmp_load_result_n1078 == NULL)
                                {
                                  abort();
                                }
                                struct hxc_caxecraft_content_CampaignTransition *hxc_l_tmp_class_field_load_result_n1079 = (*hxc_l_tmp_load_result_n1078).hxc_transitionValue;
                                hxc_l_gc_roots[491] = (const void *)hxc_l_tmp_class_field_load_result_n1079;
                                if (hxc_l_tmp_class_field_load_result_n1079 == NULL)
                                {
                                  abort();
                                }
                                hxc_string hxc_l_tmp_class_field_load_result_n1080 = (*hxc_l_tmp_class_field_load_result_n1079).hxc_exit;
                                struct hxc_caxecraft_content_CampaignTransition *hxc_l_tmp_load_result_n1081 = hxc_l_transition_h9c76253dd32a;
                                hxc_l_gc_roots[492] = (const void *)hxc_l_tmp_load_result_n1081;
                                if (hxc_l_tmp_load_result_n1081 == NULL)
                                {
                                  abort();
                                }
                                hxc_string hxc_l_tmp_class_field_load_result_n1082 = (*hxc_l_tmp_load_result_n1081).hxc_exit;
                                bool hxc_l_tmp_string_equality_result_n356 = hxc_l_tmp_class_field_load_result_n1080.data == NULL || hxc_l_tmp_class_field_load_result_n1082.data == NULL ? hxc_l_tmp_class_field_load_result_n1080.data == hxc_l_tmp_class_field_load_result_n1082.data : hxc_l_tmp_class_field_load_result_n1080.byte_length == hxc_l_tmp_class_field_load_result_n1082.byte_length && (hxc_l_tmp_class_field_load_result_n1080.byte_length == 0 || memcmp(hxc_l_tmp_class_field_load_result_n1080.data, hxc_l_tmp_class_field_load_result_n1082.data, hxc_l_tmp_class_field_load_result_n1080.byte_length) == 0);
                                bool hxc_l_tmp_string_equality_result_result_n1084 = hxc_l_tmp_string_equality_result_n356;
                                bool hxc_l_tmp_short_circuit_result_n357 = hxc_l_tmp_string_equality_result_result_n1084;
                                if (hxc_l_tmp_string_equality_result_result_n1084)
                                {
                                  struct hxc_caxecraft_app_PreparedCampaignPortal *hxc_l_tmp_load_result_n1085 = hxc_l_portal_hc129b8cc889b;
                                  hxc_l_gc_roots[493] = (const void *)hxc_l_tmp_load_result_n1085;
                                  if (hxc_l_tmp_load_result_n1085 == NULL)
                                  {
                                    abort();
                                  }
                                  struct hxc_caxecraft_content_CampaignTransition *hxc_l_tmp_class_field_load_result_n1086 = (*hxc_l_tmp_load_result_n1085).hxc_transitionValue;
                                  hxc_l_gc_roots[494] = (const void *)hxc_l_tmp_class_field_load_result_n1086;
                                  if (hxc_l_tmp_class_field_load_result_n1086 == NULL)
                                  {
                                    abort();
                                  }
                                  hxc_string hxc_l_tmp_class_field_load_result_n1087 = (*hxc_l_tmp_class_field_load_result_n1086).hxc_sourceLevel;
                                  struct hxc_caxecraft_content_CampaignTransition *hxc_l_tmp_load_result_n1088 = hxc_l_transition_h9c76253dd32a;
                                  hxc_l_gc_roots[495] = (const void *)hxc_l_tmp_load_result_n1088;
                                  if (hxc_l_tmp_load_result_n1088 == NULL)
                                  {
                                    abort();
                                  }
                                  hxc_string hxc_l_tmp_class_field_load_result_n1089 = (*hxc_l_tmp_load_result_n1088).hxc_sourceLevel;
                                  bool hxc_l_tmp_string_equality_result_n358 = hxc_l_tmp_class_field_load_result_n1087.data == NULL || hxc_l_tmp_class_field_load_result_n1089.data == NULL ? hxc_l_tmp_class_field_load_result_n1087.data == hxc_l_tmp_class_field_load_result_n1089.data : hxc_l_tmp_class_field_load_result_n1087.byte_length == hxc_l_tmp_class_field_load_result_n1089.byte_length && (hxc_l_tmp_class_field_load_result_n1087.byte_length == 0 || memcmp(hxc_l_tmp_class_field_load_result_n1087.data, hxc_l_tmp_class_field_load_result_n1089.data, hxc_l_tmp_class_field_load_result_n1087.byte_length) == 0);
                                  hxc_l_tmp_short_circuit_result_n357 = hxc_l_tmp_string_equality_result_n358;
                                }
                                bool hxc_l_tmp_short_circuit_load_result_n1092 = hxc_l_tmp_short_circuit_result_n357;
                                bool hxc_l_tmp_short_circuit_result_n359 = hxc_l_tmp_short_circuit_load_result_n1092;
                                if (hxc_l_tmp_short_circuit_load_result_n1092)
                                {
                                  struct hxc_caxecraft_app_PreparedCampaignPortal *hxc_l_tmp_load_result_n1093 = hxc_l_portal_hc129b8cc889b;
                                  hxc_l_gc_roots[496] = (const void *)hxc_l_tmp_load_result_n1093;
                                  if (hxc_l_tmp_load_result_n1093 == NULL)
                                  {
                                    abort();
                                  }
                                  struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_class_field_load_result_n1094 = (*hxc_l_tmp_load_result_n1093).hxc_destinationValue;
                                  hxc_l_gc_roots[497] = (const void *)hxc_l_tmp_class_field_load_result_n1094;
                                  if (hxc_l_tmp_class_field_load_result_n1094 == NULL)
                                  {
                                    abort();
                                  }
                                  hxc_string hxc_l_tmp_class_field_load_result_n1095 = (*hxc_l_tmp_class_field_load_result_n1094).hxc_id;
                                  struct hxc_caxecraft_content_CampaignTransition *hxc_l_tmp_load_result_n1096 = hxc_l_transition_h9c76253dd32a;
                                  hxc_l_gc_roots[498] = (const void *)hxc_l_tmp_load_result_n1096;
                                  if (hxc_l_tmp_load_result_n1096 == NULL)
                                  {
                                    abort();
                                  }
                                  hxc_string hxc_l_tmp_class_field_load_result_n1097 = (*hxc_l_tmp_load_result_n1096).hxc_destinationLevel;
                                  bool hxc_l_tmp_string_equality_result_n360 = hxc_l_tmp_class_field_load_result_n1095.data == NULL || hxc_l_tmp_class_field_load_result_n1097.data == NULL ? hxc_l_tmp_class_field_load_result_n1095.data == hxc_l_tmp_class_field_load_result_n1097.data : hxc_l_tmp_class_field_load_result_n1095.byte_length == hxc_l_tmp_class_field_load_result_n1097.byte_length && (hxc_l_tmp_class_field_load_result_n1095.byte_length == 0 || memcmp(hxc_l_tmp_class_field_load_result_n1095.data, hxc_l_tmp_class_field_load_result_n1097.data, hxc_l_tmp_class_field_load_result_n1095.byte_length) == 0);
                                  hxc_l_tmp_short_circuit_result_n359 = hxc_l_tmp_string_equality_result_n360;
                                }
                                hxc_l_tmp_short_circuit_result_n355 = hxc_l_tmp_short_circuit_result_n359;
                              }
                              if (hxc_l_tmp_short_circuit_result_n355)
                              {
                                hxc_l_gc_roots[499] = (const void *)hxc_l_portal_hc129b8cc889b;
                                hxc_l_pendingCampaignPortal = hxc_l_portal_hc129b8cc889b;
                                hxc_l_gc_roots[500] = (const void *)NULL;
                                hxc_l_preparedPortal = NULL;
                              }
                              else
                              {
                                hxc_l_gc_roots[501] = (const void *)hxc_l_nextLevel;
                                hxc_l_pendingCampaignLevel = hxc_l_nextLevel;
                                struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_load_result_n1104 = hxc_l_nextLevel;
                                hxc_l_gc_roots[502] = (const void *)hxc_l_tmp_load_result_n1104;
                                if (hxc_l_tmp_load_result_n1104 == NULL)
                                {
                                  abort();
                                }
                                hxc_string hxc_l_tmp_class_field_load_result_n1105 = (*hxc_l_tmp_load_result_n1104).hxc_id;
                                hxc_string hxc_l_tmp_string_assignment_replacement_owner_n361 = hxc_l_tmp_class_field_load_result_n1105;
                                if (hxc_string_retain(hxc_l_tmp_string_assignment_replacement_owner_n361) != HXC_STATUS_OK)
                                {
                                  abort();
                                }
                                hxc_string hxc_l_tmp_string_assignment_replacement_owned_load_result_n1106 = hxc_l_tmp_string_assignment_replacement_owner_n361;
                                if (hxc_string_release(&hxc_l_pendingCampaignLabel) != HXC_STATUS_OK)
                                {
                                  abort();
                                }
                                hxc_l_pendingCampaignLabel = hxc_l_tmp_string_assignment_replacement_owned_load_result_n1106;
                                hxc_l_loadingFramePresented = false;
                                enum hxc_caxecraft_app_AppScreen hxc_l_tmp_call_result_n1108 = hxc_caxecraft_app_AppScreen_beginLoading(hxc_l_screen);
                                hxc_l_screen = hxc_l_tmp_call_result_n1108;
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
                hxc_enum_147eda7c_destroy(&hxc_l_event_h3221db1879e6);
                hxc_array_ce94fc36_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n337);
              }
              struct hxc_optional_caxecraft_scenario_FlowTickResult hxc_l_tmp_load_result_n1109 = hxc_l_flow;
              if (!hxc_l_tmp_load_result_n1109.hxc_has_value)
              {
                abort();
              }
              hxc_string hxc_l_tmp_string_assignment_replacement_owner_n362 = hxc_l_tmp_load_result_n1109.hxc_value.hxc_activeObjective;
              if (hxc_string_retain(hxc_l_tmp_string_assignment_replacement_owner_n362) != HXC_STATUS_OK)
              {
                abort();
              }
              hxc_string hxc_l_tmp_string_assignment_replacement_owned_load_result_n1112 = hxc_l_tmp_string_assignment_replacement_owner_n362;
              if (hxc_string_release(&hxc_l_currentObjectiveId) != HXC_STATUS_OK)
              {
                abort();
              }
              hxc_l_currentObjectiveId = hxc_l_tmp_string_assignment_replacement_owned_load_result_n1112;
              if (hxc_array_ref_release(hxc_l_g1_h729617e1d868) != HXC_STATUS_OK)
              {
                abort();
              }
            }
          }
          if (hxc_l_gameTick.hxc_committed)
          {
            struct hxc_caxecraft_app_MotionHistory hxc_l_tmp_load_result_n1114 = hxc_l_motionHistory;
            struct hxc_caxecraft_app_MotionHistory hxc_l_tmp_call_result_n1116 = hxc_caxecraft_app_MotionInterpolation_advance(hxc_l_tmp_load_result_n1114, hxc_l_character.hxc_body);
            hxc_l_motionHistory = hxc_l_tmp_call_result_n1116;
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
              int32_t hxc_l_tmp_load_result_n1120 = hxc_l_pickupIndex;
              struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1121 = hxc_l_levelView_hb85a9a956786;
              hxc_l_gc_roots[503] = (const void *)hxc_l_tmp_load_result_n1121;
              if (hxc_l_tmp_load_result_n1121 == NULL)
              {
                abort();
              }
              struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n1122 = (*hxc_l_tmp_load_result_n1121).hxc_items;
              int32_t hxc_l_tmp_array_length_result_n1123;
              if (hxc_array_ref_length(hxc_l_tmp_class_field_load_result_n1122, &hxc_l_tmp_array_length_result_n1123) != HXC_STATUS_OK)
              {
                abort();
              }
              if (!(hxc_l_tmp_load_result_n1120 < hxc_l_tmp_array_length_result_n1123))
              {
                break;
              }
              struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1124 = hxc_l_levelView_hb85a9a956786;
              hxc_l_gc_roots[504] = (const void *)hxc_l_tmp_load_result_n1124;
              if (hxc_l_tmp_load_result_n1124 == NULL)
              {
                abort();
              }
              struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n1125 = (*hxc_l_tmp_load_result_n1124).hxc_items;
              struct hxc_caxecraft_app_LoadedWorldItem hxc_l_tmp_array_get_result_n1127;
              if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n1125, (size_t)hxc_l_pickupIndex, &hxc_l_tmp_array_get_result_n1127) != HXC_STATUS_OK)
              {
                abort();
              }
              struct hxc_caxecraft_app_LoadedWorldItem hxc_l_loadedItem = hxc_l_tmp_array_get_result_n1127;
              struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1128 = hxc_l_session;
              hxc_l_gc_roots[505] = (const void *)hxc_l_tmp_load_result_n1128;
              if (hxc_l_tmp_load_result_n1128 == NULL)
              {
                abort();
              }
              bool hxc_l_tmp_instance_call_result_n1130 = hxc_caxecraft_domain_GameSession_authoredItemIsActive(hxc_l_tmp_load_result_n1128, hxc_l_pickupIndex);
              bool hxc_l_tmp_short_circuit_result_n365 = hxc_l_tmp_instance_call_result_n1130;
              if (hxc_l_tmp_instance_call_result_n1130)
              {
                double hxc_l_tmp_record_field_load_result_n1131 = hxc_l_character.hxc_body.hxc_x;
                double hxc_l_tmp_record_field_load_result_n1132 = hxc_l_character.hxc_body.hxc_y;
                double hxc_l_tmp_record_field_load_result_n1133 = hxc_l_character.hxc_body.hxc_z;
                int32_t hxc_l_tmp_record_field_load_result_n1134 = hxc_l_loadedItem.hxc_xMilli;
                int32_t hxc_l_tmp_record_field_load_result_n1135 = hxc_l_loadedItem.hxc_yMilli;
                bool hxc_l_tmp_call_result_n1137 = hxc_caxecraft_gameplay_WorldItemPickup_isInRange(hxc_l_tmp_record_field_load_result_n1131, hxc_l_tmp_record_field_load_result_n1132, hxc_l_tmp_record_field_load_result_n1133, hxc_l_tmp_record_field_load_result_n1134, hxc_l_tmp_record_field_load_result_n1135, hxc_l_loadedItem.hxc_zMilli);
                hxc_l_tmp_short_circuit_result_n365 = hxc_l_tmp_call_result_n1137;
              }
              if (hxc_l_tmp_short_circuit_result_n365)
              {
                int32_t hxc_l_itemCode = hxc_l_loadedItem.hxc_storageCode;
                struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_tmp_load_result_n1140 = hxc_l_contentRegistry;
                hxc_l_gc_roots[506] = (const void *)hxc_l_tmp_load_result_n1140;
                if (hxc_l_tmp_load_result_n1140 == NULL)
                {
                  abort();
                }
                enum hxc_caxecraft_content_RuntimeItemUseProfile hxc_l_tmp_instance_call_result_n1142 = hxc_caxecraft_content_RuntimeContentRegistry_itemUseProfile(hxc_l_tmp_load_result_n1140, hxc_l_itemCode);
                bool hxc_l_tmp_short_circuit_result_n367 = hxc_l_tmp_instance_call_result_n1142 == hxc_caxecraft_content_RuntimeItemUseProfile_EquipAquatic;
                if (hxc_l_tmp_instance_call_result_n1142 == hxc_caxecraft_content_RuntimeItemUseProfile_EquipAquatic)
                {
                  struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_tmp_load_result_n1144 = hxc_l_contentRegistry;
                  hxc_l_gc_roots[507] = (const void *)hxc_l_tmp_load_result_n1144;
                  if (hxc_l_tmp_load_result_n1144 == NULL)
                  {
                    abort();
                  }
                  bool hxc_l_tmp_instance_call_result_n1146 = hxc_caxecraft_content_RuntimeContentRegistry_itemProvidesAquaticProfile(hxc_l_tmp_load_result_n1144, hxc_l_itemCode);
                  hxc_l_tmp_short_circuit_result_n367 = hxc_l_tmp_instance_call_result_n1146;
                }
                if (hxc_l_tmp_short_circuit_result_n367)
                {
                  struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_tmp_load_result_n1148 = hxc_l_contentRegistry;
                  hxc_l_gc_roots[508] = (const void *)hxc_l_tmp_load_result_n1148;
                  if (hxc_l_tmp_load_result_n1148 == NULL)
                  {
                    abort();
                  }
                  struct hxc_caxecraft_domain_AquaticProfile hxc_l_tmp_instance_call_result_n1150 = hxc_caxecraft_content_RuntimeContentRegistry_itemAquaticProfile(hxc_l_tmp_load_result_n1148, hxc_l_itemCode);
                  struct hxc_caxecraft_domain_AquaticProfile hxc_l_replacement = hxc_l_tmp_instance_call_result_n1150;
                  struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1151 = hxc_l_session;
                  hxc_l_gc_roots[509] = (const void *)hxc_l_tmp_load_result_n1151;
                  if (hxc_l_tmp_load_result_n1151 == NULL)
                  {
                    abort();
                  }
                  int32_t hxc_l_tmp_load_result_n1152 = hxc_l_pickupIndex;
                  struct hxc_caxecraft_domain_AuthoredAquaticEquipmentResult hxc_l_tmp_instance_call_result_n1154 = hxc_caxecraft_domain_GameSession_collectAuthoredAquaticEquipment(hxc_l_tmp_load_result_n1151, hxc_l_tmp_load_result_n1152, hxc_l_replacement);
                  struct hxc_caxecraft_domain_AuthoredAquaticEquipmentResult hxc_l_equipment = hxc_l_tmp_instance_call_result_n1154;
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
                  struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_tmp_load_result_n1159 = hxc_l_contentRegistry;
                  hxc_l_gc_roots[510] = (const void *)hxc_l_tmp_load_result_n1159;
                  struct hxc_optional_caxecraft_gameplay_ItemKind hxc_l_tmp_call_result_n1161 = hxc_caxecraft_app_RuntimeInventoryBinding_inventoryKindForRuntimeItem(hxc_l_tmp_load_result_n1159, hxc_l_itemCode);
                  struct hxc_optional_caxecraft_gameplay_ItemKind hxc_l_inventoryKind = hxc_l_tmp_call_result_n1161;
                  if (hxc_l_inventoryKind.hxc_has_value)
                  {
                    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1163 = hxc_l_session;
                    hxc_l_gc_roots[511] = (const void *)hxc_l_tmp_load_result_n1163;
                    if (hxc_l_tmp_load_result_n1163 == NULL)
                    {
                      abort();
                    }
                    int32_t hxc_l_tmp_load_result_n1164 = hxc_l_pickupIndex;
                    struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_load_result_n1165 = hxc_l_inventory;
                    struct hxc_optional_caxecraft_gameplay_ItemKind hxc_l_tmp_load_result_n1166 = hxc_l_inventoryKind;
                    if (!hxc_l_tmp_load_result_n1166.hxc_has_value)
                    {
                      abort();
                    }
                    struct hxc_caxecraft_domain_AuthoredInventoryItemResult hxc_l_tmp_instance_call_result_n1169 = hxc_caxecraft_domain_GameSession_collectAuthoredInventoryItem(hxc_l_tmp_load_result_n1163, hxc_l_tmp_load_result_n1164, hxc_l_tmp_load_result_n1165, hxc_l_tmp_load_result_n1166.hxc_value, hxc_l_loadedItem.hxc_quantity);
                    struct hxc_caxecraft_domain_AuthoredInventoryItemResult hxc_l_pickup = hxc_l_tmp_instance_call_result_n1169;
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
              struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1175 = hxc_l_session;
              hxc_l_gc_roots[512] = (const void *)hxc_l_tmp_load_result_n1175;
              if (hxc_l_tmp_load_result_n1175 == NULL)
              {
                abort();
              }
              int32_t hxc_l_tmp_record_field_load_result_n1176 = hxc_l_gameTick.hxc_tickIndex;
              struct hxc_caxecraft_domain_ActorControllerTickResult hxc_l_tmp_instance_call_result_n1178 = hxc_caxecraft_domain_GameSession_stepAuthoredActorControllers(hxc_l_tmp_load_result_n1175, hxc_l_tmp_record_field_load_result_n1176, hxc_l_damagePolicy);
              struct hxc_caxecraft_domain_ActorControllerTickResult hxc_l_actorTick = hxc_l_tmp_instance_call_result_n1178;
              switch (hxc_l_actorTick.hxc_status.hxc_tag) {
                case hxc_caxecraft_domain_ActorControllerTickStatus_ControllersAdvanced:
                  {
                    hxc_l_availableInteractionPrompt = hxc_caxecraft_app_InteractionPrompt_NoInteractionPrompt;
                    int32_t hxc_l_g_h3b0d539c16ff = 0;
                    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1181 = hxc_l_session;
                    hxc_l_gc_roots[517] = (const void *)hxc_l_tmp_load_result_n1181;
                    if (hxc_l_tmp_load_result_n1181 == NULL)
                    {
                      abort();
                    }
                    struct hxc_array_ref *hxc_l_tmp_instance_call_result_n1182 = hxc_caxecraft_domain_GameSession_actorControllerEventSnapshots(hxc_l_tmp_load_result_n1181);
                    hxc_l_g1_h50401e609561 = hxc_l_tmp_instance_call_result_n1182;
                    while (1)
                    {
                      int32_t hxc_l_tmp_load_result_n1183 = hxc_l_g_h3b0d539c16ff;
                      int32_t hxc_l_tmp_array_length_result_n1185;
                      if (hxc_array_ref_length(hxc_l_g1_h50401e609561, &hxc_l_tmp_array_length_result_n1185) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      if (!(hxc_l_tmp_load_result_n1183 < hxc_l_tmp_array_length_result_n1185))
                      {
                        break;
                      }
                      struct hxc_array_ref *hxc_l_tmp_load_result_n1186 = hxc_l_g1_h50401e609561;
                      struct hxc_caxecraft_domain_ActorControllerEvent hxc_l_tmp_array_get_result_n1188;
                      if (hxc_array_ref_get_copy(hxc_l_tmp_load_result_n1186, (size_t)hxc_l_g_h3b0d539c16ff, &hxc_l_tmp_array_get_result_n1188) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      hxc_l_tmp_array_element_owner_n376 = hxc_l_tmp_array_get_result_n1188;
                      hxc_l_event_h2fe0dab417fb = hxc_l_tmp_array_element_owner_n376;
                      if (hxc_enum_8b2a26b8_retain(&hxc_l_event_h2fe0dab417fb) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      hxc_l_g_h3b0d539c16ff = hxc_i32_add_wrapping(hxc_l_g_h3b0d539c16ff, 1);
                      switch (hxc_l_event_h2fe0dab417fb.hxc_tag) {
                        case hxc_caxecraft_domain_ActorControllerEvent_NoControllerEvent:
                          {
                            break;
                          }
                        case hxc_caxecraft_domain_ActorControllerEvent_InteractionAvailable:
                          {
                            if (hxc_l_event_h2fe0dab417fb.hxc_tag != hxc_caxecraft_domain_ActorControllerEvent_InteractionAvailable)
                            {
                              abort();
                            }
                            int32_t hxc_l_tmp_enum_payload_project_n1193 = hxc_l_event_h2fe0dab417fb.hxc_payload.hxc_InteractionAvailable.hxc_characterId;
                            int32_t hxc_l_source_h21cce826e9d5 = hxc_l_tmp_enum_payload_project_n1193;
                            struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1194 = hxc_l_levelView_hb85a9a956786;
                            hxc_l_gc_roots[518] = (const void *)hxc_l_tmp_load_result_n1194;
                            if (hxc_l_tmp_load_result_n1194 == NULL)
                            {
                              abort();
                            }
                            bool hxc_l_tmp_instance_call_result_n1196 = hxc_caxecraft_app_PlayableLevelView_hasDialogueActor(hxc_l_tmp_load_result_n1194, hxc_l_source_h21cce826e9d5);
                            if (hxc_l_tmp_instance_call_result_n1196)
                            {
                              hxc_l_availableInteractionPrompt = hxc_caxecraft_app_InteractionPrompt_TalkInteractionPrompt;
                            }
                            break;
                          }
                        case hxc_caxecraft_domain_ActorControllerEvent_LocalPlayerAttack:
                          {
                            if (hxc_l_event_h2fe0dab417fb.hxc_tag != hxc_caxecraft_domain_ActorControllerEvent_LocalPlayerAttack)
                            {
                              abort();
                            }
                            int32_t hxc_l_tmp_enum_payload_project_n1199 = hxc_l_event_h2fe0dab417fb.hxc_payload.hxc_LocalPlayerAttack.hxc_characterId;
                            int32_t hxc_l_source_h9d3d8bff7e6c = hxc_l_tmp_enum_payload_project_n1199;
                            int32_t hxc_l_tmp_load_result_n1200 = hxc_l_source_h9d3d8bff7e6c;
                            if (hxc_l_tmp_load_result_n1200 == hxc_l_enemyActorId)
                            {
                              hxc_l_enemyAttackFrames = 120;
                            }
                            break;
                          }
                        case hxc_caxecraft_domain_ActorControllerEvent_DropRequested:
                          {
                            if (hxc_l_event_h2fe0dab417fb.hxc_tag != hxc_caxecraft_domain_ActorControllerEvent_DropRequested)
                            {
                              abort();
                            }
                            int32_t hxc_l_tmp_enum_payload_project_n1203 = hxc_l_event_h2fe0dab417fb.hxc_payload.hxc_DropRequested.hxc_characterId;
                            int32_t hxc_l_source_ha9689e8a64b8 = hxc_l_tmp_enum_payload_project_n1203;
                            if (hxc_l_event_h2fe0dab417fb.hxc_tag != hxc_caxecraft_domain_ActorControllerEvent_DropRequested)
                            {
                              abort();
                            }
                            hxc_string hxc_l_tmp_enum_payload_project_n1205 = hxc_l_event_h2fe0dab417fb.hxc_payload.hxc_DropRequested.hxc_drop;
                            hxc_l_drop = hxc_l_tmp_enum_payload_project_n1205;
                            if (hxc_string_retain(hxc_l_drop) != HXC_STATUS_OK)
                            {
                              abort();
                            }
                            struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_tmp_load_result_n1206 = hxc_l_contentRegistry;
                            hxc_l_gc_roots[519] = (const void *)hxc_l_tmp_load_result_n1206;
                            if (hxc_l_tmp_load_result_n1206 == NULL)
                            {
                              abort();
                            }
                            int32_t hxc_l_tmp_instance_call_result_n1208 = hxc_caxecraft_content_RuntimeContentRegistry_dropQuantityById(hxc_l_tmp_load_result_n1206, hxc_l_drop);
                            int32_t hxc_l_quantity = hxc_l_tmp_instance_call_result_n1208;
                            struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1209 = hxc_l_session;
                            hxc_l_gc_roots[520] = (const void *)hxc_l_tmp_load_result_n1209;
                            if (hxc_l_tmp_load_result_n1209 == NULL)
                            {
                              abort();
                            }
                            struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n1211 = hxc_caxecraft_domain_GameSession_readCharacter(hxc_l_tmp_load_result_n1209, hxc_l_source_ha9689e8a64b8);
                            struct hxc_caxecraft_domain_Character hxc_l_defeatedActor = hxc_l_tmp_instance_call_result_n1211;
                            int32_t hxc_l_tmp_load_result_n1212 = hxc_l_quantity;
                            bool hxc_l_tmp_short_circuit_result_n383 = hxc_l_tmp_load_result_n1212 <= 0;
                            if (!(hxc_l_tmp_load_result_n1212 <= 0))
                            {
                              hxc_l_tmp_short_circuit_result_n383 = hxc_l_defeatedActor.hxc_id <= 0;
                            }
                            if (hxc_l_tmp_short_circuit_result_n383)
                            {
                              hxc_l_quit = true;
                            }
                            else
                            {
                              if (!hxc_l_berryDrop.hxc_active)
                              {
                                struct hxc_caxecraft_domain_Character hxc_l_tmp_load_result_n1216 = hxc_l_defeatedActor;
                                struct hxc_caxecraft_gameplay_BerryDropState hxc_l_tmp_call_result_n1218 = hxc_caxecraft_gameplay_BerryDrop_fromDefeatedCharacter(hxc_l_tmp_load_result_n1216, hxc_l_quantity);
                                hxc_l_berryDrop = hxc_l_tmp_call_result_n1218;
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
                      hxc_enum_8b2a26b8_destroy(&hxc_l_event_h2fe0dab417fb);
                      hxc_array_43e9f8ca_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n376);
                    }
                    if (hxc_array_ref_release(hxc_l_g1_h50401e609561) != HXC_STATUS_OK)
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
            struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1219 = hxc_l_session;
            hxc_l_gc_roots[513] = (const void *)hxc_l_tmp_load_result_n1219;
            if (hxc_l_tmp_load_result_n1219 == NULL)
            {
              abort();
            }
            struct hxc_array_ref *hxc_l_tmp_instance_call_result_n1220 = hxc_caxecraft_domain_GameSession_actorControllerStateSnapshots(hxc_l_tmp_load_result_n1219);
            hxc_l_actorPhases = hxc_l_tmp_instance_call_result_n1220;
            struct hxc_array_ref *hxc_l_tmp_load_result_n1221 = hxc_l_actorPhases;
            struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation hxc_l_tmp_call_result_n1224 = hxc_caxecraft_app_CaxecraftApp_observeActorPhase(hxc_l_tmp_load_result_n1221, hxc_l_enemyActorId, hxc_caxecraft_domain_ActorControllerPhase_Defeated);
            hxc_l_enemyPhase_ha225e19c84ac = hxc_l_tmp_call_result_n1224;
            struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1225 = hxc_l_session;
            hxc_l_gc_roots[514] = (const void *)hxc_l_tmp_load_result_n1225;
            if (hxc_l_tmp_load_result_n1225 == NULL)
            {
              abort();
            }
            struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n1227 = hxc_caxecraft_domain_GameSession_readCharacter(hxc_l_tmp_load_result_n1225, hxc_l_enemyActorId);
            hxc_l_enemyActor = hxc_l_tmp_instance_call_result_n1227;
            struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1228 = hxc_l_session;
            hxc_l_gc_roots[515] = (const void *)hxc_l_tmp_load_result_n1228;
            struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1229 = hxc_l_levelView_hb85a9a956786;
            hxc_l_gc_roots[516] = (const void *)hxc_l_tmp_load_result_n1229;
            bool hxc_l_tmp_call_result_n1231 = hxc_caxecraft_app_CaxecraftApp_dialogueActorsAreValid(hxc_l_tmp_load_result_n1228, hxc_l_tmp_load_result_n1229, hxc_l_actorPhases);
            bool hxc_l_tmp_short_circuit_result_n385 = !hxc_l_tmp_call_result_n1231;
            if (!!hxc_l_tmp_call_result_n1231)
            {
              hxc_l_tmp_short_circuit_result_n385 = !hxc_l_enemyPhase_ha225e19c84ac.hxc_valid;
            }
            bool hxc_l_tmp_short_circuit_load_result_n1233 = hxc_l_tmp_short_circuit_result_n385;
            bool hxc_l_tmp_short_circuit_result_n386 = hxc_l_tmp_short_circuit_load_result_n1233;
            if (!hxc_l_tmp_short_circuit_load_result_n1233)
            {
              hxc_l_tmp_short_circuit_result_n386 = hxc_l_enemyActor.hxc_id <= 0;
            }
            if (hxc_l_tmp_short_circuit_result_n386)
            {
              hxc_l_quit = true;
            }
            struct hxc_caxecraft_gameplay_SwordCombatState hxc_l_tmp_call_result_n1237 = hxc_caxecraft_gameplay_SwordCombat_step(hxc_l_swordCombat);
            hxc_l_swordCombat = hxc_l_tmp_call_result_n1237;
            bool hxc_l_tmp_load_result_n1238 = hxc_l_swordQueued;
            bool hxc_l_tmp_short_circuit_result_n387 = hxc_l_tmp_load_result_n1238;
            if (hxc_l_tmp_load_result_n1238)
            {
              hxc_l_tmp_short_circuit_result_n387 = !hxc_l_conversation.hxc_has_value;
            }
            if (hxc_l_tmp_short_circuit_result_n387)
            {
              struct hxc_caxecraft_gameplay_SwordCombatState hxc_l_tmp_load_result_n1241 = hxc_l_swordCombat;
              struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_load_result_n1242 = hxc_l_inventory;
              struct hxc_caxecraft_domain_VitalsState hxc_l_tmp_record_field_load_result_n1243 = hxc_l_character.hxc_vitals;
              struct hxc_caxecraft_domain_Character hxc_l_tmp_load_result_n1244 = hxc_l_enemyActor;
              double hxc_l_tmp_record_field_load_result_n1245 = hxc_l_character.hxc_body.hxc_x;
              double hxc_l_tmp_record_field_load_result_n1246 = hxc_l_character.hxc_body.hxc_z;
              double hxc_l_tmp_load_result_n1247 = hxc_l_lookX;
              int32_t hxc_l_tmp_call_result_n1249 = hxc_caxecraft_gameplay_SwordCombat_decide(hxc_l_tmp_load_result_n1241, hxc_l_tmp_load_result_n1242, hxc_l_tmp_record_field_load_result_n1243, hxc_l_tmp_load_result_n1244, hxc_l_tmp_record_field_load_result_n1245, hxc_l_tmp_record_field_load_result_n1246, hxc_l_tmp_load_result_n1247, hxc_l_lookZ);
              int32_t hxc_l_swordDecision = hxc_l_tmp_call_result_n1249;
              int32_t hxc_l_tmp_load_result_n1250 = hxc_l_swordDecision;
              bool hxc_l_tmp_short_circuit_result_n389 = hxc_l_tmp_load_result_n1250 == 5;
              if (hxc_l_tmp_load_result_n1250 == 5)
              {
                struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1251 = hxc_l_session;
                hxc_l_gc_roots[521] = (const void *)hxc_l_tmp_load_result_n1251;
                if (hxc_l_tmp_load_result_n1251 == NULL)
                {
                  abort();
                }
                bool hxc_l_tmp_instance_call_result_n1253 = hxc_caxecraft_domain_GameSession_characterAcceptsMeleeDamage(hxc_l_tmp_load_result_n1251, hxc_l_enemyActorId);
                hxc_l_tmp_short_circuit_result_n389 = hxc_l_tmp_instance_call_result_n1253;
              }
              if (hxc_l_tmp_short_circuit_result_n389)
              {
                hxc_l_gc_roots[522] = (const void *)hxc_l_contentRegistry;
                hxc_string hxc_l_tmp_call_result_n1257 = hxc_caxecraft_app_RuntimeInventoryBinding_runtimeItemContentIdForInventoryKind(hxc_l_contentRegistry, hxc_caxecraft_gameplay_ItemKind_CopperSword);
                hxc_l_swordItemType = hxc_l_tmp_call_result_n1257;
                bool hxc_l_tmp_string_null_equality_result_n392 = hxc_l_swordItemType.data == NULL;
                bool hxc_l_tmp_string_null_equality_result_result_n1259 = hxc_l_tmp_string_null_equality_result_n392;
                struct hxc_optional_caxecraft_domain_CharacterDamageResult hxc_l_tmp_conditional_result_n393 = (struct hxc_optional_caxecraft_domain_CharacterDamageResult){ .hxc_has_value = false };
                if (hxc_l_tmp_string_null_equality_result_result_n1259)
                {
                  hxc_l_tmp_conditional_result_n393 = (struct hxc_optional_caxecraft_domain_CharacterDamageResult){ .hxc_has_value = false };
                }
                else
                {
                  struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1260 = hxc_l_session;
                  hxc_l_gc_roots[523] = (const void *)hxc_l_tmp_load_result_n1260;
                  if (hxc_l_tmp_load_result_n1260 == NULL)
                  {
                    abort();
                  }
                  int32_t hxc_l_tmp_load_result_n1261 = hxc_l_enemyActorId;
                  hxc_string hxc_l_tmp_enum_payload_0_owner_n394 = hxc_l_swordItemType;
                  if (hxc_string_retain(hxc_l_tmp_enum_payload_0_owner_n394) != HXC_STATUS_OK)
                  {
                    abort();
                  }
                  hxc_l_tmp_instance_call_argument_0_owner_n395 = (struct hxc_caxecraft_domain_CharacterDamageCommand){ .hxc_amount = 1, .hxc_cause = (struct hxc_caxecraft_domain_CharacterDamageCause){ .hxc_tag = hxc_caxecraft_domain_CharacterDamageCause_LocalPlayerItem, .hxc_payload.hxc_LocalPlayerItem.hxc_itemType = hxc_l_tmp_enum_payload_0_owner_n394 }, .hxc_target = hxc_l_tmp_load_result_n1261 };
                  struct hxc_caxecraft_domain_CharacterDamageResult hxc_l_tmp_instance_call_result_n1267 = hxc_caxecraft_domain_GameSession_damageCharacter(hxc_l_tmp_load_result_n1260, hxc_l_tmp_instance_call_argument_0_owner_n395);
                  hxc_l_tmp_conditional_result_n393 = (struct hxc_optional_caxecraft_domain_CharacterDamageResult){ .hxc_has_value = true, .hxc_value = hxc_l_tmp_instance_call_result_n1267 };
                  hxc_record_d86f2bdc_destroy(&hxc_l_tmp_instance_call_argument_0_owner_n395);
                }
                struct hxc_optional_caxecraft_domain_CharacterDamageResult hxc_l_damage = hxc_l_tmp_conditional_result_n393;
                struct hxc_optional_caxecraft_domain_CharacterDamageResult hxc_l_tmp_load_result_n1270 = hxc_l_damage;
                bool hxc_l_tmp_short_circuit_result_n396 = !hxc_l_tmp_load_result_n1270.hxc_has_value;
                if (!!hxc_l_tmp_load_result_n1270.hxc_has_value)
                {
                  struct hxc_optional_caxecraft_domain_CharacterDamageResult hxc_l_tmp_load_result_n1271 = hxc_l_damage;
                  if (!hxc_l_tmp_load_result_n1271.hxc_has_value)
                  {
                    abort();
                  }
                  hxc_l_tmp_short_circuit_result_n396 = !hxc_l_tmp_load_result_n1271.hxc_value.hxc_resolved;
                }
                if (hxc_l_tmp_short_circuit_result_n396)
                {
                  hxc_l_quit = true;
                }
                else
                {
                  struct hxc_optional_caxecraft_domain_CharacterDamageResult hxc_l_tmp_load_result_n1275 = hxc_l_damage;
                  if (!hxc_l_tmp_load_result_n1275.hxc_has_value)
                  {
                    abort();
                  }
                  if (hxc_l_tmp_load_result_n1275.hxc_value.hxc_damageApplied > 0)
                  {
                    struct hxc_optional_caxecraft_domain_CharacterDamageResult hxc_l_tmp_load_result_n1278 = hxc_l_damage;
                    if (!hxc_l_tmp_load_result_n1278.hxc_has_value)
                    {
                      abort();
                    }
                    hxc_l_enemyActor = hxc_l_tmp_load_result_n1278.hxc_value.hxc_character;
                    hxc_l_strikeHitFrames = 16;
                  }
                }
                if (hxc_string_release(&hxc_l_swordItemType) != HXC_STATUS_OK)
                {
                  abort();
                }
              }
              int32_t hxc_l_tmp_load_result_n1281 = hxc_l_swordDecision;
              struct hxc_caxecraft_gameplay_SwordCombatState hxc_l_tmp_call_result_n1283 = hxc_caxecraft_gameplay_SwordCombat_after(hxc_l_tmp_load_result_n1281, hxc_l_swordCombat);
              hxc_l_swordCombat = hxc_l_tmp_call_result_n1283;
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
        struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1285 = hxc_l_session;
        hxc_l_gc_roots[466] = (const void *)hxc_l_tmp_load_result_n1285;
        if (hxc_l_tmp_load_result_n1285 == NULL)
        {
          abort();
        }
        struct hxc_caxecraft_domain_CharacterBody hxc_l_tmp_record_field_load_result_n1287 = hxc_l_character.hxc_body;
        double hxc_l_tmp_load_result_n1288 = hxc_l_lookX;
        double hxc_l_tmp_load_result_n1289 = hxc_l_lookY;
        struct hxc_caxecraft_domain_PlayerCameraView hxc_l_tmp_instance_call_result_n1291 = hxc_caxecraft_domain_GameSession_playerCamera(hxc_l_tmp_load_result_n1285, hxc_caxecraft_domain_PlayerCameraMode_FirstPerson, hxc_l_tmp_record_field_load_result_n1287, hxc_l_tmp_load_result_n1288, hxc_l_tmp_load_result_n1289, hxc_l_lookZ);
        struct hxc_caxecraft_domain_PlayerCameraView hxc_l_interactionView = hxc_l_tmp_instance_call_result_n1291;
        struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1292 = hxc_l_session;
        hxc_l_gc_roots[467] = (const void *)hxc_l_tmp_load_result_n1292;
        if (hxc_l_tmp_load_result_n1292 == NULL)
        {
          abort();
        }
        size_t hxc_l_tmp_length_n1294;
        const uint8_t *hxc_l_tmp_instance_call_result_n1293 = hxc_caxecraft_domain_GameSession_worldView(hxc_l_tmp_load_result_n1292, &hxc_l_tmp_length_n1294);
        double hxc_l_tmp_record_field_load_result_n1295 = hxc_l_interactionView.hxc_interactionOriginX;
        double hxc_l_tmp_record_field_load_result_n1296 = hxc_l_interactionView.hxc_interactionOriginY;
        double hxc_l_tmp_record_field_load_result_n1297 = hxc_l_interactionView.hxc_interactionOriginZ;
        double hxc_l_tmp_record_field_load_result_n1298 = hxc_l_interactionView.hxc_interactionDirectionX;
        double hxc_l_tmp_record_field_load_result_n1299 = hxc_l_interactionView.hxc_interactionDirectionY;
        struct hxc_caxecraft_domain_RaycastHit hxc_l_tmp_call_result_n1301 = hxc_caxecraft_domain_VoxelRaycast_trace(hxc_l_tmp_instance_call_result_n1293, hxc_l_tmp_length_n1294, hxc_l_tmp_record_field_load_result_n1295, hxc_l_tmp_record_field_load_result_n1296, hxc_l_tmp_record_field_load_result_n1297, hxc_l_tmp_record_field_load_result_n1298, hxc_l_tmp_record_field_load_result_n1299, hxc_l_interactionView.hxc_interactionDirectionZ, 7.0);
        struct hxc_caxecraft_domain_RaycastHit hxc_l_hit = hxc_l_tmp_call_result_n1301;
        bool hxc_l_tmp_call_result_n1303 = hxc_caxecraft_app_AppScreen_capturesPointer(hxc_l_screen);
        bool hxc_l_tmp_short_circuit_result_n399 = hxc_l_tmp_call_result_n1303;
        if (hxc_l_tmp_call_result_n1303)
        {
          hxc_l_tmp_short_circuit_result_n399 = !hxc_l_conversation.hxc_has_value;
        }
        bool hxc_l_tmp_short_circuit_load_result_n1305 = hxc_l_tmp_short_circuit_result_n399;
        bool hxc_l_tmp_short_circuit_result_n400 = hxc_l_tmp_short_circuit_load_result_n1305;
        if (hxc_l_tmp_short_circuit_load_result_n1305)
        {
          hxc_l_tmp_short_circuit_result_n400 = !hxc_l_conversationOwnedInput;
        }
        bool hxc_l_tmp_short_circuit_load_result_n1307 = hxc_l_tmp_short_circuit_result_n400;
        bool hxc_l_tmp_short_circuit_result_n401 = hxc_l_tmp_short_circuit_load_result_n1307;
        if (hxc_l_tmp_short_circuit_load_result_n1307)
        {
          hxc_l_tmp_short_circuit_result_n401 = !hxc_l_recapturedThisFrame;
        }
        bool hxc_l_tmp_short_circuit_load_result_n1309 = hxc_l_tmp_short_circuit_result_n401;
        bool hxc_l_tmp_short_circuit_result_n402 = hxc_l_tmp_short_circuit_load_result_n1309;
        if (hxc_l_tmp_short_circuit_load_result_n1309)
        {
          hxc_l_tmp_short_circuit_result_n402 = hxc_l_primaryPressed;
        }
        if (hxc_l_tmp_short_circuit_result_n402)
        {
          if (hxc_l_character.hxc_vitals.hxc_health > 0)
          {
            if (hxc_l_selectedMode == 1)
            {
              bool hxc_l_tmp_call_result_n1316 = hxc_caxecraft_gameplay_Inventory_selectedIs(hxc_l_inventory, hxc_caxecraft_gameplay_ItemKind_CopperSword);
              bool hxc_l_tmp_short_circuit_result_n403 = !hxc_l_tmp_call_result_n1316;
              if (!hxc_l_tmp_call_result_n1316)
              {
                hxc_l_tmp_short_circuit_result_n403 = hxc_l_hit.hxc_hit;
              }
              bool hxc_l_tmp_short_circuit_load_result_n1318 = hxc_l_tmp_short_circuit_result_n403;
              bool hxc_l_tmp_short_circuit_result_n404 = hxc_l_tmp_short_circuit_load_result_n1318;
              if (hxc_l_tmp_short_circuit_load_result_n1318)
              {
                struct hxc_caxecraft_domain_AquaticState hxc_l_tmp_record_field_load_result_n1319 = hxc_l_character.hxc_aquatic;
                bool hxc_l_tmp_call_result_n1321 = hxc_caxecraft_domain_Aquatics_canMine(hxc_l_tmp_record_field_load_result_n1319, hxc_l_character.hxc_aquaticProfile);
                hxc_l_tmp_short_circuit_result_n404 = hxc_l_tmp_call_result_n1321;
              }
              if (hxc_l_tmp_short_circuit_result_n404)
              {
                int32_t hxc_l_tmp_record_field_load_result_n1323 = hxc_l_hit.hxc_cellX;
                int32_t hxc_l_tmp_record_field_load_result_n1324 = hxc_l_hit.hxc_cellY;
                struct hxc_caxecraft_scenario_VoxelPoint hxc_l_tmp_call_result_n1326 = hxc_caxecraft_domain_World_coord(hxc_l_tmp_record_field_load_result_n1323, hxc_l_tmp_record_field_load_result_n1324, hxc_l_hit.hxc_cellZ);
                struct hxc_caxecraft_scenario_VoxelPoint hxc_l_minedCoordinate = hxc_l_tmp_call_result_n1326;
                int32_t hxc_l_sandBefore = hxc_l_inventory.hxc_sand;
                (void)hxc_l_sandBefore;
                struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1328 = hxc_l_session;
                hxc_l_gc_roots[524] = (const void *)hxc_l_tmp_load_result_n1328;
                if (hxc_l_tmp_load_result_n1328 == NULL)
                {
                  abort();
                }
                struct hxc_caxecraft_scenario_VoxelPoint hxc_l_tmp_load_result_n1329 = hxc_l_minedCoordinate;
                struct hxc_caxecraft_gameplay_MiningResult hxc_l_tmp_instance_call_result_n1331 = hxc_caxecraft_domain_GameSession_mineTerrain(hxc_l_tmp_load_result_n1328, hxc_l_tmp_load_result_n1329, hxc_l_inventory);
                struct hxc_caxecraft_gameplay_MiningResult hxc_l_mining = hxc_l_tmp_instance_call_result_n1331;
                hxc_l_inventory = hxc_l_mining.hxc_inventory;
                if (hxc_l_mining.hxc_outcome == 2)
                {
                  if (hxc_l_self == NULL)
                  {
                    abort();
                  }
                  struct hxc_caxecraft_app_TerrainRenderer *hxc_l_tmp_owned_class_field_address_n1334 = &(*hxc_l_self).hxc_terrainRenderer;
                  struct hxc_caxecraft_app_TerrainChunkCache *hxc_l_tmp_owned_class_field_address_n1335 = &(*hxc_l_tmp_owned_class_field_address_n1334).hxc_cache;
                  if (hxc_l_tmp_owned_class_field_address_n1335 == NULL)
                  {
                    abort();
                  }
                  hxc_caxecraft_app_TerrainChunkCache_invalidate(hxc_l_tmp_owned_class_field_address_n1335, hxc_l_minedCoordinate);
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
                int32_t hxc_l_tmp_record_field_load_result_n1340 = hxc_l_hit.hxc_cellX;
                int32_t hxc_l_tmp_record_field_load_result_n1341 = hxc_l_hit.hxc_cellY;
                struct hxc_caxecraft_scenario_VoxelPoint hxc_l_tmp_call_result_n1343 = hxc_caxecraft_domain_World_coord(hxc_l_tmp_record_field_load_result_n1340, hxc_l_tmp_record_field_load_result_n1341, hxc_l_hit.hxc_cellZ);
                struct hxc_caxecraft_scenario_VoxelPoint hxc_l_removedCoordinate = hxc_l_tmp_call_result_n1343;
                struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1344 = hxc_l_session;
                hxc_l_gc_roots[525] = (const void *)hxc_l_tmp_load_result_n1344;
                if (hxc_l_tmp_load_result_n1344 == NULL)
                {
                  abort();
                }
                bool hxc_l_tmp_instance_call_result_n1346 = hxc_caxecraft_domain_GameSession_removeTerrain(hxc_l_tmp_load_result_n1344, hxc_l_removedCoordinate);
                bool hxc_l_removed = hxc_l_tmp_instance_call_result_n1346;
                if (hxc_l_removed)
                {
                  if (hxc_l_self == NULL)
                  {
                    abort();
                  }
                  struct hxc_caxecraft_app_TerrainRenderer *hxc_l_tmp_owned_class_field_address_n1348 = &(*hxc_l_self).hxc_terrainRenderer;
                  struct hxc_caxecraft_app_TerrainChunkCache *hxc_l_tmp_owned_class_field_address_n1349 = &(*hxc_l_tmp_owned_class_field_address_n1348).hxc_cache;
                  if (hxc_l_tmp_owned_class_field_address_n1349 == NULL)
                  {
                    abort();
                  }
                  hxc_caxecraft_app_TerrainChunkCache_invalidate(hxc_l_tmp_owned_class_field_address_n1349, hxc_l_removedCoordinate);
                }
              }
            }
          }
        }
        bool hxc_l_tmp_call_result_n1352 = hxc_caxecraft_app_AppScreen_capturesPointer(hxc_l_screen);
        bool hxc_l_tmp_short_circuit_result_n410 = hxc_l_tmp_call_result_n1352;
        if (hxc_l_tmp_call_result_n1352)
        {
          hxc_l_tmp_short_circuit_result_n410 = !hxc_l_conversation.hxc_has_value;
        }
        bool hxc_l_tmp_short_circuit_load_result_n1354 = hxc_l_tmp_short_circuit_result_n410;
        bool hxc_l_tmp_short_circuit_result_n411 = hxc_l_tmp_short_circuit_load_result_n1354;
        if (hxc_l_tmp_short_circuit_load_result_n1354)
        {
          hxc_l_tmp_short_circuit_result_n411 = !hxc_l_conversationOwnedInput;
        }
        bool hxc_l_tmp_short_circuit_load_result_n1356 = hxc_l_tmp_short_circuit_result_n411;
        bool hxc_l_tmp_short_circuit_result_n412 = hxc_l_tmp_short_circuit_load_result_n1356;
        if (hxc_l_tmp_short_circuit_load_result_n1356)
        {
          hxc_l_tmp_short_circuit_result_n412 = hxc_l_secondaryPressed;
        }
        if (hxc_l_tmp_short_circuit_result_n412)
        {
          if (hxc_l_character.hxc_vitals.hxc_health > 0)
          {
            struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_tmp_load_result_n1360 = hxc_l_contentRegistry;
            hxc_l_gc_roots[526] = (const void *)hxc_l_tmp_load_result_n1360;
            enum hxc_caxecraft_gameplay_ItemKind hxc_l_tmp_call_result_n1362 = hxc_caxecraft_gameplay_Inventory_itemAt(hxc_l_inventory.hxc_selected);
            hxc_string hxc_l_tmp_call_result_n1363 = hxc_caxecraft_app_RuntimeInventoryBinding_runtimeItemContentIdForInventoryKind(hxc_l_tmp_load_result_n1360, hxc_l_tmp_call_result_n1362);
            hxc_l_selectedItemType = hxc_l_tmp_call_result_n1363;
            struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1364 = hxc_l_session;
            hxc_l_gc_roots[527] = (const void *)hxc_l_tmp_load_result_n1364;
            if (hxc_l_tmp_load_result_n1364 == NULL)
            {
              abort();
            }
            struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_load_result_n1365 = hxc_l_inventory;
            struct hxc_caxecraft_domain_LocalRecoveryResult hxc_l_tmp_instance_call_result_n1367 = hxc_caxecraft_domain_GameSession_useSelectedRecovery(hxc_l_tmp_load_result_n1364, hxc_l_tmp_load_result_n1365, hxc_l_selectedItemType);
            struct hxc_caxecraft_domain_LocalRecoveryResult hxc_l_recovery = hxc_l_tmp_instance_call_result_n1367;
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
                  int32_t hxc_l_tmp_record_field_load_result_n1374 = hxc_l_hit.hxc_previousX;
                  int32_t hxc_l_tmp_record_field_load_result_n1375 = hxc_l_hit.hxc_previousY;
                  struct hxc_caxecraft_scenario_VoxelPoint hxc_l_tmp_call_result_n1377 = hxc_caxecraft_domain_World_coord(hxc_l_tmp_record_field_load_result_n1374, hxc_l_tmp_record_field_load_result_n1375, hxc_l_hit.hxc_previousZ);
                  struct hxc_caxecraft_scenario_VoxelPoint hxc_l_placement = hxc_l_tmp_call_result_n1377;
                  enum hxc_caxecraft_domain_BlockKind hxc_l_tmp_call_result_n1379 = hxc_caxecraft_gameplay_Inventory_selectedBlock(hxc_l_inventory);
                  enum hxc_caxecraft_domain_BlockKind hxc_l_selectedBlock = hxc_l_tmp_call_result_n1379;
                  struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_load_result_n1380 = hxc_l_inventory;
                  int32_t hxc_l_tmp_call_result_n1382 = hxc_caxecraft_gameplay_Inventory_countAt(hxc_l_tmp_load_result_n1380, hxc_l_inventory.hxc_selected);
                  bool hxc_l_hasItem = hxc_l_tmp_call_result_n1382 > 0;
                  int32_t hxc_l_tmp_load_result_n1383 = hxc_l_selectedMode;
                  hxc_string hxc_l_tmp_conditional_managed_result_n419 = { 0 };
                  if (hxc_l_tmp_load_result_n1383 == 1)
                  {
                    (void)hxc_l_selectedItemType;
                    hxc_l_tmp_conditional_managed_result_n419 = hxc_l_selectedItemType;
                    if (hxc_string_retain(hxc_l_tmp_conditional_managed_result_n419) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                  }
                  else
                  {
                    hxc_l_tmp_conditional_managed_result_n419 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
                    if (hxc_string_retain(hxc_l_tmp_conditional_managed_result_n419) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                  }
                  hxc_string hxc_l_tmp_conditional_managed_move_result_n1385 = hxc_l_tmp_conditional_managed_result_n419;
                  hxc_l_placementItemType = hxc_l_tmp_conditional_managed_move_result_n1385;
                  bool hxc_l_tmp_load_result_n1386 = hxc_l_hasItem;
                  bool hxc_l_tmp_short_circuit_result_n420 = !hxc_l_tmp_load_result_n1386;
                  if (!!hxc_l_tmp_load_result_n1386)
                  {
                    bool hxc_l_tmp_call_result_n1388 = hxc_caxecraft_domain_World_isPlaceable(hxc_l_selectedBlock);
                    hxc_l_tmp_short_circuit_result_n420 = !hxc_l_tmp_call_result_n1388;
                  }
                  bool hxc_l_tmp_short_circuit_load_result_n1389 = hxc_l_tmp_short_circuit_result_n420;
                  bool hxc_l_tmp_short_circuit_result_n421 = hxc_l_tmp_short_circuit_load_result_n1389;
                  if (!hxc_l_tmp_short_circuit_load_result_n1389)
                  {
                    struct hxc_caxecraft_domain_CharacterBody hxc_l_tmp_record_field_load_result_n1390 = hxc_l_character.hxc_body;
                    bool hxc_l_tmp_call_result_n1392 = hxc_caxecraft_domain_CharacterPhysics_canPlaceAt(hxc_l_tmp_record_field_load_result_n1390, hxc_l_placement);
                    hxc_l_tmp_short_circuit_result_n421 = !hxc_l_tmp_call_result_n1392;
                  }
                  bool hxc_l_tmp_short_circuit_load_result_n1393 = hxc_l_tmp_short_circuit_result_n421;
                  bool hxc_l_tmp_short_circuit_result_n422 = hxc_l_tmp_short_circuit_load_result_n1393;
                  if (!hxc_l_tmp_short_circuit_load_result_n1393)
                  {
                    int32_t hxc_l_tmp_load_result_n1394 = hxc_l_selectedMode;
                    bool hxc_l_tmp_short_circuit_result_n423 = hxc_l_tmp_load_result_n1394 == 1;
                    if (hxc_l_tmp_load_result_n1394 == 1)
                    {
                      bool hxc_l_tmp_string_null_equality_result_n424 = hxc_l_placementItemType.data == NULL;
                      hxc_l_tmp_short_circuit_result_n423 = hxc_l_tmp_string_null_equality_result_n424;
                    }
                    hxc_l_tmp_short_circuit_result_n422 = hxc_l_tmp_short_circuit_result_n423;
                  }
                  bool hxc_l_tmp_short_circuit_load_result_n1398 = hxc_l_tmp_short_circuit_result_n422;
                  bool hxc_l_tmp_short_circuit_result_n425 = hxc_l_tmp_short_circuit_load_result_n1398;
                  if (!hxc_l_tmp_short_circuit_load_result_n1398)
                  {
                    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1399 = hxc_l_session;
                    hxc_l_gc_roots[528] = (const void *)hxc_l_tmp_load_result_n1399;
                    if (hxc_l_tmp_load_result_n1399 == NULL)
                    {
                      abort();
                    }
                    struct hxc_caxecraft_scenario_VoxelPoint hxc_l_tmp_load_result_n1400 = hxc_l_placement;
                    enum hxc_caxecraft_domain_BlockKind hxc_l_tmp_load_result_n1401 = hxc_l_selectedBlock;
                    bool hxc_l_tmp_instance_call_result_n1403 = hxc_caxecraft_domain_GameSession_placeTerrain(hxc_l_tmp_load_result_n1399, hxc_l_tmp_load_result_n1400, hxc_l_tmp_load_result_n1401, hxc_l_placementItemType);
                    hxc_l_tmp_short_circuit_result_n425 = !hxc_l_tmp_instance_call_result_n1403;
                  }
                  if (hxc_l_tmp_short_circuit_result_n425)
                  {
                    hxc_l_placementBlockedFrames = 60;
                  }
                  else
                  {
                    if (hxc_l_self == NULL)
                    {
                      abort();
                    }
                    struct hxc_caxecraft_app_TerrainRenderer *hxc_l_tmp_owned_class_field_address_n1405 = &(*hxc_l_self).hxc_terrainRenderer;
                    struct hxc_caxecraft_app_TerrainChunkCache *hxc_l_tmp_owned_class_field_address_n1406 = &(*hxc_l_tmp_owned_class_field_address_n1405).hxc_cache;
                    if (hxc_l_tmp_owned_class_field_address_n1406 == NULL)
                    {
                      abort();
                    }
                    hxc_caxecraft_app_TerrainChunkCache_invalidate(hxc_l_tmp_owned_class_field_address_n1406, hxc_l_placement);
                    if (hxc_l_selectedMode == 1)
                    {
                      struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n1410 = hxc_caxecraft_gameplay_Inventory_consumeSelected(hxc_l_inventory);
                      hxc_l_inventory = hxc_l_tmp_call_result_n1410;
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
        bool hxc_l_tmp_call_result_n1414 = hxc_caxecraft_app_AppScreen_pausesSimulation(hxc_l_screen);
        if (!hxc_l_tmp_call_result_n1414)
        {
          struct hxc_caxecraft_gameplay_BerryDropState hxc_l_tmp_load_result_n1415 = hxc_l_berryDrop;
          double hxc_l_tmp_record_field_load_result_n1416 = hxc_l_character.hxc_body.hxc_x;
          double hxc_l_tmp_record_field_load_result_n1417 = hxc_l_character.hxc_body.hxc_y;
          bool hxc_l_tmp_call_result_n1419 = hxc_caxecraft_gameplay_BerryDrop_isInRange(hxc_l_tmp_load_result_n1415, hxc_l_tmp_record_field_load_result_n1416, hxc_l_tmp_record_field_load_result_n1417, hxc_l_character.hxc_body.hxc_z);
          if (hxc_l_tmp_call_result_n1419)
          {
            struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_load_result_n1420 = hxc_l_inventory;
            int32_t hxc_l_tmp_call_result_n1423 = hxc_caxecraft_gameplay_Inventory_acceptedAmount(hxc_l_tmp_load_result_n1420, hxc_caxecraft_gameplay_ItemKind_Berries, hxc_l_berryDrop.hxc_amount);
            int32_t hxc_l_acceptedDrop = hxc_l_tmp_call_result_n1423;
            if (hxc_l_acceptedDrop > 0)
            {
              struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_load_result_n1425 = hxc_l_inventory;
              struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n1428 = hxc_caxecraft_gameplay_Inventory_collectItem(hxc_l_tmp_load_result_n1425, hxc_caxecraft_gameplay_ItemKind_Berries, hxc_l_acceptedDrop);
              hxc_l_inventory = hxc_l_tmp_call_result_n1428;
              struct hxc_caxecraft_gameplay_BerryDropState hxc_l_tmp_load_result_n1429 = hxc_l_berryDrop;
              struct hxc_caxecraft_gameplay_BerryDropState hxc_l_tmp_call_result_n1431 = hxc_caxecraft_gameplay_BerryDrop_collectAmount(hxc_l_tmp_load_result_n1429, hxc_l_acceptedDrop);
              hxc_l_berryDrop = hxc_l_tmp_call_result_n1431;
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
        struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1448 = hxc_l_session;
        hxc_l_gc_roots[529] = (const void *)hxc_l_tmp_load_result_n1448;
        if (hxc_l_tmp_load_result_n1448 == NULL)
        {
          abort();
        }
        struct hxc_caxecraft_domain_GameView hxc_l_tmp_instance_call_result_n1449 = hxc_caxecraft_domain_GameSession_view(hxc_l_tmp_load_result_n1448);
        struct hxc_caxecraft_domain_GameView hxc_l_committedView = hxc_l_tmp_instance_call_result_n1449;
        if (!hxc_l_committedView.hxc_valid)
        {
          hxc_l_quit = true;
        }
        else
        {
          hxc_l_character = hxc_l_committedView.hxc_localPlayer;
        }
        struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1452 = hxc_l_session;
        hxc_l_gc_roots[530] = (const void *)hxc_l_tmp_load_result_n1452;
        hxc_l_gc_roots[531] = (const void *)hxc_l_levelView_hb85a9a956786;
        struct hxc_caxecraft_app_CaxecraftApp_AvailableInteractionTarget hxc_l_tmp_call_result_n1454 = hxc_caxecraft_app_CaxecraftApp_nearestAvailableInteraction(hxc_l_tmp_load_result_n1452, hxc_l_levelView_hb85a9a956786);
        hxc_l_tmp_static_call_argument_0_owner_n428 = hxc_l_tmp_call_result_n1454;
        enum hxc_caxecraft_app_InteractionPrompt hxc_l_tmp_call_result_n1456 = hxc_caxecraft_app_CaxecraftApp_promptForAvailableInteraction(hxc_l_tmp_static_call_argument_0_owner_n428);
        hxc_l_availableInteractionPrompt = hxc_l_tmp_call_result_n1456;
        struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1457 = hxc_l_session;
        hxc_l_gc_roots[532] = (const void *)hxc_l_tmp_load_result_n1457;
        if (hxc_l_tmp_load_result_n1457 == NULL)
        {
          abort();
        }
        struct hxc_array_ref *hxc_l_tmp_instance_call_result_n1458 = hxc_caxecraft_domain_GameSession_actorControllerStateSnapshots(hxc_l_tmp_load_result_n1457);
        hxc_l_presentationActorPhases = hxc_l_tmp_instance_call_result_n1458;
        struct hxc_array_ref *hxc_l_tmp_load_result_n1459 = hxc_l_presentationActorPhases;
        struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation hxc_l_tmp_call_result_n1462 = hxc_caxecraft_app_CaxecraftApp_observeActorPhase(hxc_l_tmp_load_result_n1459, hxc_l_enemyActorId, hxc_caxecraft_domain_ActorControllerPhase_Defeated);
        hxc_l_enemyPhase_ha225e19c84ac = hxc_l_tmp_call_result_n1462;
        struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1463 = hxc_l_session;
        hxc_l_gc_roots[533] = (const void *)hxc_l_tmp_load_result_n1463;
        if (hxc_l_tmp_load_result_n1463 == NULL)
        {
          abort();
        }
        struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n1465 = hxc_caxecraft_domain_GameSession_readCharacter(hxc_l_tmp_load_result_n1463, hxc_l_enemyActorId);
        hxc_l_enemyActor = hxc_l_tmp_instance_call_result_n1465;
        struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1466 = hxc_l_session;
        hxc_l_gc_roots[534] = (const void *)hxc_l_tmp_load_result_n1466;
        struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1467 = hxc_l_levelView_hb85a9a956786;
        hxc_l_gc_roots[535] = (const void *)hxc_l_tmp_load_result_n1467;
        bool hxc_l_tmp_call_result_n1469 = hxc_caxecraft_app_CaxecraftApp_dialogueActorsAreValid(hxc_l_tmp_load_result_n1466, hxc_l_tmp_load_result_n1467, hxc_l_presentationActorPhases);
        bool hxc_l_tmp_short_circuit_result_n430 = !hxc_l_tmp_call_result_n1469;
        if (!!hxc_l_tmp_call_result_n1469)
        {
          hxc_l_tmp_short_circuit_result_n430 = !hxc_l_enemyPhase_ha225e19c84ac.hxc_valid;
        }
        bool hxc_l_tmp_short_circuit_load_result_n1471 = hxc_l_tmp_short_circuit_result_n430;
        bool hxc_l_tmp_short_circuit_result_n431 = hxc_l_tmp_short_circuit_load_result_n1471;
        if (!hxc_l_tmp_short_circuit_load_result_n1471)
        {
          hxc_l_tmp_short_circuit_result_n431 = hxc_l_enemyActor.hxc_id <= 0;
        }
        if (hxc_l_tmp_short_circuit_result_n431)
        {
          hxc_l_quit = true;
        }
        struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1474 = hxc_l_session;
        hxc_l_gc_roots[536] = (const void *)hxc_l_tmp_load_result_n1474;
        hxc_l_gc_roots[537] = (const void *)hxc_l_levelView_hb85a9a956786;
        struct hxc_array_ref *hxc_l_tmp_call_result_n1476 = hxc_caxecraft_app_CaxecraftApp_dialogueActorSnapshots(hxc_l_tmp_load_result_n1474, hxc_l_levelView_hb85a9a956786);
        hxc_l_dialogueActors = hxc_l_tmp_call_result_n1476;
        int32_t hxc_l_completedTicks = hxc_l_committedView.hxc_completedTicks;
        if (hxc_l_resetMotionThisFrame)
        {
          struct hxc_caxecraft_app_MotionHistory hxc_l_tmp_call_result_n1480 = hxc_caxecraft_app_MotionInterpolation_start(hxc_l_character.hxc_body);
          hxc_l_motionHistory = hxc_l_tmp_call_result_n1480;
        }
        struct hxc_caxecraft_app_MotionHistory hxc_l_tmp_load_result_n1481 = hxc_l_motionHistory;
        struct hxc_caxecraft_app_RenderPosition hxc_l_tmp_call_result_n1483 = hxc_caxecraft_app_MotionInterpolation_sample(hxc_l_tmp_load_result_n1481, hxc_l_accumulator, 0.05);
        struct hxc_caxecraft_app_RenderPosition hxc_l_renderPosition = hxc_l_tmp_call_result_n1483;
        double hxc_l_tmp_record_field_load_result_n1484 = hxc_l_renderPosition.hxc_x;
        double hxc_l_tmp_record_field_load_result_n1485 = hxc_l_renderPosition.hxc_y;
        struct hxc_caxecraft_domain_CharacterBody hxc_l_tmp_call_result_n1487 = hxc_caxecraft_domain_CharacterPhysics_body(hxc_l_tmp_record_field_load_result_n1484, hxc_l_tmp_record_field_load_result_n1485, hxc_l_renderPosition.hxc_z);
        struct hxc_caxecraft_domain_CharacterBody hxc_l_presentationBody = hxc_l_tmp_call_result_n1487;
        struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1488 = hxc_l_session;
        hxc_l_gc_roots[538] = (const void *)hxc_l_tmp_load_result_n1488;
        if (hxc_l_tmp_load_result_n1488 == NULL)
        {
          abort();
        }
        enum hxc_caxecraft_domain_PlayerCameraMode hxc_l_tmp_load_result_n1489 = hxc_l_cameraMode;
        struct hxc_caxecraft_domain_CharacterBody hxc_l_tmp_load_result_n1490 = hxc_l_presentationBody;
        double hxc_l_tmp_load_result_n1491 = hxc_l_lookX;
        double hxc_l_tmp_load_result_n1492 = hxc_l_lookY;
        struct hxc_caxecraft_domain_PlayerCameraView hxc_l_tmp_instance_call_result_n1494 = hxc_caxecraft_domain_GameSession_playerCamera(hxc_l_tmp_load_result_n1488, hxc_l_tmp_load_result_n1489, hxc_l_tmp_load_result_n1490, hxc_l_tmp_load_result_n1491, hxc_l_tmp_load_result_n1492, hxc_l_lookZ);
        struct hxc_caxecraft_domain_PlayerCameraView hxc_l_cameraView = hxc_l_tmp_instance_call_result_n1494;
        double hxc_l_eyeX = hxc_l_cameraView.hxc_positionX;
        double hxc_l_eyeY = hxc_l_cameraView.hxc_positionY;
        double hxc_l_eyeZ = hxc_l_cameraView.hxc_positionZ;
        double hxc_l_tmp_load_result_n1498 = hxc_l_eyeX;
        double hxc_l_tmp_load_result_n1499 = hxc_l_eyeY;
        struct Vector3 hxc_l_this1_h6be122c8f2aa = (struct Vector3){ .x = (float)hxc_l_tmp_load_result_n1498, .y = (float)hxc_l_tmp_load_result_n1499, .z = (float)hxc_l_eyeZ };
        double hxc_l_y_h7c80da642e8a = hxc_l_cameraView.hxc_targetY;
        double hxc_l_z_h1caf5315a57b = hxc_l_cameraView.hxc_targetZ;
        struct Vector3 hxc_l_tmp_load_result_n1504 = hxc_l_this1_h6be122c8f2aa;
        double hxc_l_tmp_record_field_load_result_n1505 = hxc_l_cameraView.hxc_targetX;
        double hxc_l_tmp_load_result_n1506 = hxc_l_y_h7c80da642e8a;
        struct Camera3D hxc_l_camera = (struct Camera3D){ .position = hxc_l_tmp_load_result_n1504, .target = (struct Vector3){ .x = (float)hxc_l_tmp_record_field_load_result_n1505, .y = (float)hxc_l_tmp_load_result_n1506, .z = (float)hxc_l_z_h1caf5315a57b }, .up = (struct Vector3){ .x = (float)0.0, .y = (float)1.0, .z = (float)0.0 }, .fovy = (float)70.0, .projection = 0 };
        BeginDrawing();
        if (hxc_l_onTitle)
        {
          hxc_string hxc_l_locale1_h8b7aba33e680 = { 0 };
          switch (hxc_l_locale) {
            case 0:
              {
                hxc_l_locale1_h8b7aba33e680 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
                if (hxc_string_retain(hxc_l_locale1_h8b7aba33e680) != HXC_STATUS_OK)
                {
                  abort();
                }
                break;
              }
            case 1:
              {
                hxc_l_locale1_h8b7aba33e680 = (hxc_string){ (const uint8_t *)"es-mx", 5, true, NULL };
                if (hxc_string_retain(hxc_l_locale1_h8b7aba33e680) != HXC_STATUS_OK)
                {
                  abort();
                }
                break;
              }
            default:
              {
                hxc_l_locale1_h8b7aba33e680 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
                if (hxc_string_retain(hxc_l_locale1_h8b7aba33e680) != HXC_STATUS_OK)
                {
                  abort();
                }
                break;
              }
          }
          hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n1513 = hxc_l_locale1_h8b7aba33e680;
          hxc_l_tmp_managed_flow_owner_n445 = hxc_l_tmp_managed_flow_carrier_move_result_n1513;
          struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1514 = hxc_l_levelView_hb85a9a956786;
          hxc_l_gc_roots[539] = (const void *)hxc_l_tmp_load_result_n1514;
          if (hxc_l_tmp_load_result_n1514 == NULL)
          {
            abort();
          }
          struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_class_field_load_result_n1515 = (*hxc_l_tmp_load_result_n1514).hxc_levelPresentation;
          hxc_l_gc_roots[540] = (const void *)hxc_l_tmp_class_field_load_result_n1515;
          if (hxc_l_tmp_class_field_load_result_n1515 == NULL)
          {
            abort();
          }
          hxc_string hxc_l_tmp_instance_call_result_n1517 = hxc_caxecraft_content_RuntimeLevelPresentation_message(hxc_l_tmp_class_field_load_result_n1515, (hxc_string){ (const uint8_t *)"adventure_tagline", 17, true, NULL }, hxc_l_tmp_managed_flow_owner_n445);
          hxc_l_adventureTagline = hxc_l_tmp_instance_call_result_n1517;
          int32_t hxc_l_tmp_native_call_result_n1518 = GetScreenWidth();
          int32_t hxc_l_width_h3cc5c1d53e37 = (int32_t)hxc_l_tmp_native_call_result_n1518;
          int32_t hxc_l_tmp_native_call_result_n1519 = GetScreenHeight();
          int32_t hxc_l_height_haa1fb843124b = (int32_t)hxc_l_tmp_native_call_result_n1519;
          if (hxc_l_titleTextureReady)
          {
            struct Color hxc_l_this1_h1187a729fd43 = (struct Color){ .r = 255, .g = 255, .b = 255, .a = 255 };
            struct Texture hxc_l_tmp_load_result_n1522 = hxc_l_titleTexture;
            double hxc_l_imageAspect = hxc_f64_divide_zero_safe((double)(int32_t)hxc_l_tmp_load_result_n1522.width, (double)(int32_t)hxc_l_titleTexture.height);
            int32_t hxc_l_tmp_load_result_n1526 = hxc_l_width_h3cc5c1d53e37;
            double hxc_l_screenAspect = hxc_f64_divide_zero_safe((double)hxc_l_tmp_load_result_n1526, (double)hxc_l_height_haa1fb843124b);
            double hxc_l_sourceX = 0.0;
            double hxc_l_sourceY = 0.0;
            double hxc_l_sourceWidth = (double)(int32_t)hxc_l_titleTexture.width;
            double hxc_l_sourceHeight = (double)(int32_t)hxc_l_titleTexture.height;
            double hxc_l_tmp_load_result_n1532 = hxc_l_screenAspect;
            if (hxc_l_tmp_load_result_n1532 > hxc_l_imageAspect)
            {
              struct Texture hxc_l_tmp_load_result_n1534 = hxc_l_titleTexture;
              hxc_l_sourceHeight = hxc_f64_divide_zero_safe((double)(int32_t)hxc_l_tmp_load_result_n1534.width, hxc_l_screenAspect);
              struct Texture hxc_l_tmp_load_result_n1537 = hxc_l_titleTexture;
              hxc_l_sourceY = ((double)(int32_t)hxc_l_tmp_load_result_n1537.height - hxc_l_sourceHeight) * 0.5;
            }
            else
            {
              struct Texture hxc_l_tmp_load_result_n1540 = hxc_l_titleTexture;
              hxc_l_sourceWidth = (double)(int32_t)hxc_l_tmp_load_result_n1540.height * hxc_l_screenAspect;
              struct Texture hxc_l_tmp_load_result_n1543 = hxc_l_titleTexture;
              hxc_l_sourceX = ((double)(int32_t)hxc_l_tmp_load_result_n1543.width - hxc_l_sourceWidth) * 0.5;
            }
            struct Texture hxc_l_tmp_load_result_n1546 = hxc_l_titleTexture;
            double hxc_l_tmp_load_result_n1547 = hxc_l_sourceX;
            double hxc_l_tmp_load_result_n1548 = hxc_l_sourceY;
            double hxc_l_tmp_load_result_n1549 = hxc_l_sourceWidth;
            double hxc_l_tmp_load_result_n1550 = hxc_l_sourceHeight;
            int32_t hxc_l_tmp_load_result_n1552 = hxc_l_width_h3cc5c1d53e37;
            int32_t hxc_l_tmp_load_result_n1553 = hxc_l_height_haa1fb843124b;
            DrawTexturePro(hxc_l_tmp_load_result_n1546, (struct Rectangle){ .x = (float)hxc_l_tmp_load_result_n1547, .y = (float)hxc_l_tmp_load_result_n1548, .width = (float)hxc_l_tmp_load_result_n1549, .height = (float)hxc_l_tmp_load_result_n1550 }, (struct Rectangle){ .x = (float)0.0, .y = (float)0.0, .width = (float)((double)hxc_l_tmp_load_result_n1552 + 0.0), .height = (float)((double)hxc_l_tmp_load_result_n1553 + 0.0) }, (struct Vector2){ .x = (float)0.0, .y = (float)0.0 }, (float)0.0, hxc_l_this1_h1187a729fd43);
          }
          else
          {
            ClearBackground((struct Color){ .r = 126, .g = 190, .b = 201, .a = 255 });
          }
          struct Color hxc_l_this1_he55e91849291 = (struct Color){ .r = 4, .g = 14, .b = 23, .a = 132 };
          int32_t hxc_l_tmp_load_result_n1559 = hxc_l_width_h3cc5c1d53e37;
          int32_t hxc_l_tmp_load_result_n1560 = hxc_l_height_haa1fb843124b;
          DrawRectangle((int32_t)0, (int32_t)0, (int32_t)hxc_l_tmp_load_result_n1559, (int32_t)hxc_l_tmp_load_result_n1560, hxc_l_this1_he55e91849291);
          if (hxc_l_wordmarkTextureReady)
          {
            int32_t hxc_l_centerX = hxc_l_width_h3cc5c1d53e37 / 2;
            struct Color hxc_l_this1_h7e1a11413540 = (struct Color){ .r = 255, .g = 255, .b = 255, .a = 255 };
            double hxc_l_scale = hxc_f64_divide_zero_safe((double)560, (double)(int32_t)hxc_l_wordmarkTexture.width);
            double hxc_l_heightScale = hxc_f64_divide_zero_safe((double)132, (double)(int32_t)hxc_l_wordmarkTexture.height);
            double hxc_l_tmp_load_result_n1569 = hxc_l_heightScale;
            if (hxc_l_tmp_load_result_n1569 < hxc_l_scale)
            {
              hxc_l_scale = hxc_l_heightScale;
            }
            struct Texture hxc_l_tmp_load_result_n1572 = hxc_l_wordmarkTexture;
            double hxc_l_width_h5f69545d9907 = (double)(int32_t)hxc_l_tmp_load_result_n1572.width * hxc_l_scale;
            struct Texture hxc_l_tmp_load_result_n1575 = hxc_l_wordmarkTexture;
            double hxc_l_height_heefb7a4c1ffa = (double)(int32_t)hxc_l_tmp_load_result_n1575.height * hxc_l_scale;
            double hxc_l_width1 = (double)(int32_t)hxc_l_wordmarkTexture.width + 0.0;
            double hxc_l_height1 = (double)(int32_t)hxc_l_wordmarkTexture.height + 0.0;
            struct Texture hxc_l_tmp_load_result_n1582 = hxc_l_wordmarkTexture;
            double hxc_l_tmp_load_result_n1583 = hxc_l_width1;
            double hxc_l_tmp_load_result_n1584 = hxc_l_height1;
            int32_t hxc_l_tmp_load_result_n1586 = hxc_l_centerX;
            double hxc_l_tmp_load_result_n1587 = hxc_l_width_h5f69545d9907;
            double hxc_l_tmp_load_result_n1588 = hxc_l_width_h5f69545d9907;
            double hxc_l_tmp_load_result_n1589 = hxc_l_height_heefb7a4c1ffa;
            DrawTexturePro(hxc_l_tmp_load_result_n1582, (struct Rectangle){ .x = (float)0.0, .y = (float)0.0, .width = (float)hxc_l_tmp_load_result_n1583, .height = (float)hxc_l_tmp_load_result_n1584 }, (struct Rectangle){ .x = (float)((double)hxc_l_tmp_load_result_n1586 - hxc_l_tmp_load_result_n1587 * 0.5), .y = (float)28.0, .width = (float)hxc_l_tmp_load_result_n1588, .height = (float)hxc_l_tmp_load_result_n1589 }, (struct Vector2){ .x = (float)0.0, .y = (float)0.0 }, (float)0.0, hxc_l_this1_h7e1a11413540);
          }
          else
          {
            int32_t hxc_l_x_hc6d9f054e54b = hxc_i32_subtract_wrapping(hxc_l_width_h3cc5c1d53e37 / 2, 92);
            struct Color hxc_l_this1_hbacbc2e567f7 = (struct Color){ .r = 242, .g = 249, .b = 245, .a = 255 };
            struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_tmp_load_result_n1595 = hxc_l_uiCatalog;
            hxc_l_gc_roots[541] = (const void *)hxc_l_tmp_load_result_n1595;
            if (hxc_l_tmp_load_result_n1595 == NULL)
            {
              abort();
            }
            hxc_string hxc_l_tmp_instance_call_result_n1597 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_tmp_load_result_n1595, hxc_l_locale, (hxc_string){ (const uint8_t *)"title_fallback", 14, true, NULL });
            hxc_l_tmp_cstring_arg_owner_owner_n467 = hxc_l_tmp_instance_call_result_n1597;
            hxc_string hxc_l_tmp_cstring_arg_owner_borrow_result_n1598 = hxc_l_tmp_cstring_arg_owner_owner_n467;
            int32_t hxc_l_tmp_load_result_n1599 = hxc_l_x_hc6d9f054e54b;
            struct Color hxc_l_tmp_load_result_n1600 = hxc_l_this1_hbacbc2e567f7;
            hxc_call_cstring hxc_l_tmp_string_prepare_cstring_result_n1601 = HXC_CALL_CSTRING_INITIALIZER;
            if (hxc_string_prepare_call_cstring(&hxc_l_tmp_cstring_arg_owner_borrow_result_n1598, hxc_default_allocator(), &hxc_l_tmp_string_prepare_cstring_result_n1601) != HXC_STATUS_OK)
            {
              abort();
            }
            DrawText(hxc_l_tmp_string_prepare_cstring_result_n1601.data, (int32_t)hxc_l_tmp_load_result_n1599, (int32_t)52, (int32_t)34, hxc_l_tmp_load_result_n1600);
            if (hxc_call_cstring_dispose(&hxc_l_tmp_string_prepare_cstring_result_n1601) != HXC_STATUS_OK)
            {
              abort();
            }
            if (hxc_string_release(&hxc_l_tmp_cstring_arg_owner_owner_n467) != HXC_STATUS_OK)
            {
              abort();
            }
          }
          int32_t hxc_l_firstTop = hxc_i32_subtract_wrapping(hxc_l_height_haa1fb843124b, 293);
          int32_t hxc_l_left_h9577c97aa7ea = hxc_i32_subtract_wrapping(hxc_l_width_h3cc5c1d53e37 / 2, 180);
          if (hxc_l_selectedMode == 0)
          {
            struct Color hxc_l_this1_hfb20159be9ab = (struct Color){ .r = 16, .g = 88, .b = 102, .a = 232 };
            int32_t hxc_l_tmp_load_result_n1606 = hxc_l_left_h9577c97aa7ea;
            int32_t hxc_l_tmp_load_result_n1607 = hxc_l_firstTop;
            DrawRectangle((int32_t)hxc_l_tmp_load_result_n1606, (int32_t)hxc_l_tmp_load_result_n1607, (int32_t)360, (int32_t)54, hxc_l_this1_hfb20159be9ab);
            struct Color hxc_l_this1_h2cad555d764c = (struct Color){ .r = 255, .g = 190, .b = 55, .a = 255 };
            int32_t hxc_l_tmp_load_result_n1610 = hxc_l_left_h9577c97aa7ea;
            int32_t hxc_l_tmp_load_result_n1611 = hxc_l_firstTop;
            DrawRectangleLines((int32_t)hxc_l_tmp_load_result_n1610, (int32_t)hxc_l_tmp_load_result_n1611, (int32_t)360, (int32_t)54, hxc_l_this1_h2cad555d764c);
          }
          else
          {
            struct Color hxc_l_this1_h0626e9b0afed = (struct Color){ .r = 6, .g = 26, .b = 36, .a = 218 };
            int32_t hxc_l_tmp_load_result_n1614 = hxc_l_left_h9577c97aa7ea;
            int32_t hxc_l_tmp_load_result_n1615 = hxc_l_firstTop;
            DrawRectangle((int32_t)hxc_l_tmp_load_result_n1614, (int32_t)hxc_l_tmp_load_result_n1615, (int32_t)360, (int32_t)54, hxc_l_this1_h0626e9b0afed);
            struct Color hxc_l_this1_h7b20de195c48 = (struct Color){ .r = 92, .g = 194, .b = 188, .a = 255 };
            int32_t hxc_l_tmp_load_result_n1618 = hxc_l_left_h9577c97aa7ea;
            int32_t hxc_l_tmp_load_result_n1619 = hxc_l_firstTop;
            DrawRectangleLines((int32_t)hxc_l_tmp_load_result_n1618, (int32_t)hxc_l_tmp_load_result_n1619, (int32_t)360, (int32_t)54, hxc_l_this1_h7b20de195c48);
          }
          struct Color hxc_l_this1_h54955b99c405 = (struct Color){ .r = 242, .g = 249, .b = 245, .a = 255 };
          struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_tmp_load_result_n1622 = hxc_l_uiCatalog;
          hxc_l_gc_roots[542] = (const void *)hxc_l_tmp_load_result_n1622;
          if (hxc_l_tmp_load_result_n1622 == NULL)
          {
            abort();
          }
          hxc_string hxc_l_tmp_instance_call_result_n1624 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_tmp_load_result_n1622, hxc_l_locale, (hxc_string){ (const uint8_t *)"menu_creative", 13, true, NULL });
          hxc_l_tmp_cstring_arg_owner_owner_n475 = hxc_l_tmp_instance_call_result_n1624;
          hxc_string hxc_l_tmp_cstring_arg_owner_borrow_result_n1625 = hxc_l_tmp_cstring_arg_owner_owner_n475;
          int32_t hxc_l_tmp_load_result_n1626 = hxc_l_left_h9577c97aa7ea;
          int32_t hxc_l_tmp_load_result_n1627 = hxc_l_firstTop;
          struct Color hxc_l_tmp_load_result_n1628 = hxc_l_this1_h54955b99c405;
          hxc_call_cstring hxc_l_tmp_string_prepare_cstring_result_n1629 = HXC_CALL_CSTRING_INITIALIZER;
          if (hxc_string_prepare_call_cstring(&hxc_l_tmp_cstring_arg_owner_borrow_result_n1625, hxc_default_allocator(), &hxc_l_tmp_string_prepare_cstring_result_n1629) != HXC_STATUS_OK)
          {
            abort();
          }
          DrawText(hxc_l_tmp_string_prepare_cstring_result_n1629.data, (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n1626, 28), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n1627, 16), (int32_t)21, hxc_l_tmp_load_result_n1628);
          if (hxc_call_cstring_dispose(&hxc_l_tmp_string_prepare_cstring_result_n1629) != HXC_STATUS_OK)
          {
            abort();
          }
          int32_t hxc_l_top_hd4a8e371d865 = hxc_i32_add_wrapping(hxc_i32_add_wrapping(hxc_l_firstTop, 54), 14);
          int32_t hxc_l_left_he526cc5a4e77 = hxc_i32_subtract_wrapping(hxc_l_width_h3cc5c1d53e37 / 2, 180);
          if (hxc_l_selectedMode == 1)
          {
            struct Color hxc_l_this1_h6eb9b3a5f859 = (struct Color){ .r = 16, .g = 88, .b = 102, .a = 232 };
            int32_t hxc_l_tmp_load_result_n1634 = hxc_l_left_he526cc5a4e77;
            int32_t hxc_l_tmp_load_result_n1635 = hxc_l_top_hd4a8e371d865;
            DrawRectangle((int32_t)hxc_l_tmp_load_result_n1634, (int32_t)hxc_l_tmp_load_result_n1635, (int32_t)360, (int32_t)54, hxc_l_this1_h6eb9b3a5f859);
            struct Color hxc_l_this1_h4f06d0862927 = (struct Color){ .r = 255, .g = 190, .b = 55, .a = 255 };
            int32_t hxc_l_tmp_load_result_n1638 = hxc_l_left_he526cc5a4e77;
            int32_t hxc_l_tmp_load_result_n1639 = hxc_l_top_hd4a8e371d865;
            DrawRectangleLines((int32_t)hxc_l_tmp_load_result_n1638, (int32_t)hxc_l_tmp_load_result_n1639, (int32_t)360, (int32_t)54, hxc_l_this1_h4f06d0862927);
          }
          else
          {
            struct Color hxc_l_this1_hebc1e4aa1407 = (struct Color){ .r = 6, .g = 26, .b = 36, .a = 218 };
            int32_t hxc_l_tmp_load_result_n1642 = hxc_l_left_he526cc5a4e77;
            int32_t hxc_l_tmp_load_result_n1643 = hxc_l_top_hd4a8e371d865;
            DrawRectangle((int32_t)hxc_l_tmp_load_result_n1642, (int32_t)hxc_l_tmp_load_result_n1643, (int32_t)360, (int32_t)54, hxc_l_this1_hebc1e4aa1407);
            struct Color hxc_l_this1_had540e7d6ab0 = (struct Color){ .r = 92, .g = 194, .b = 188, .a = 255 };
            int32_t hxc_l_tmp_load_result_n1646 = hxc_l_left_he526cc5a4e77;
            int32_t hxc_l_tmp_load_result_n1647 = hxc_l_top_hd4a8e371d865;
            DrawRectangleLines((int32_t)hxc_l_tmp_load_result_n1646, (int32_t)hxc_l_tmp_load_result_n1647, (int32_t)360, (int32_t)54, hxc_l_this1_had540e7d6ab0);
          }
          struct Color hxc_l_this1_h56b4b6f2a1f3 = (struct Color){ .r = 242, .g = 249, .b = 245, .a = 255 };
          struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_tmp_load_result_n1650 = hxc_l_uiCatalog;
          hxc_l_gc_roots[543] = (const void *)hxc_l_tmp_load_result_n1650;
          if (hxc_l_tmp_load_result_n1650 == NULL)
          {
            abort();
          }
          hxc_string hxc_l_tmp_instance_call_result_n1652 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_tmp_load_result_n1650, hxc_l_locale, (hxc_string){ (const uint8_t *)"menu_adventure", 14, true, NULL });
          hxc_l_tmp_cstring_arg_owner_owner_n483 = hxc_l_tmp_instance_call_result_n1652;
          hxc_string hxc_l_tmp_cstring_arg_owner_borrow_result_n1653 = hxc_l_tmp_cstring_arg_owner_owner_n483;
          int32_t hxc_l_tmp_load_result_n1654 = hxc_l_left_he526cc5a4e77;
          int32_t hxc_l_tmp_load_result_n1655 = hxc_l_top_hd4a8e371d865;
          struct Color hxc_l_tmp_load_result_n1656 = hxc_l_this1_h56b4b6f2a1f3;
          hxc_call_cstring hxc_l_tmp_string_prepare_cstring_result_n1657 = HXC_CALL_CSTRING_INITIALIZER;
          if (hxc_string_prepare_call_cstring(&hxc_l_tmp_cstring_arg_owner_borrow_result_n1653, hxc_default_allocator(), &hxc_l_tmp_string_prepare_cstring_result_n1657) != HXC_STATUS_OK)
          {
            abort();
          }
          DrawText(hxc_l_tmp_string_prepare_cstring_result_n1657.data, (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n1654, 28), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n1655, 16), (int32_t)21, hxc_l_tmp_load_result_n1656);
          if (hxc_call_cstring_dispose(&hxc_l_tmp_string_prepare_cstring_result_n1657) != HXC_STATUS_OK)
          {
            abort();
          }
          int32_t hxc_l_top_h210b80880625 = hxc_i32_add_wrapping(hxc_l_firstTop, 136);
          int32_t hxc_l_left_hb678ab451d4a = hxc_i32_subtract_wrapping(hxc_l_width_h3cc5c1d53e37 / 2, 180);
          struct Color hxc_l_this1_he427a56d4ef2 = (struct Color){ .r = 6, .g = 26, .b = 36, .a = 218 };
          int32_t hxc_l_tmp_load_result_n1661 = hxc_l_left_hb678ab451d4a;
          int32_t hxc_l_tmp_load_result_n1662 = hxc_l_top_h210b80880625;
          DrawRectangle((int32_t)hxc_l_tmp_load_result_n1661, (int32_t)hxc_l_tmp_load_result_n1662, (int32_t)360, (int32_t)54, hxc_l_this1_he427a56d4ef2);
          struct Color hxc_l_this1_h68ec3fcab7a7 = (struct Color){ .r = 92, .g = 194, .b = 188, .a = 255 };
          int32_t hxc_l_tmp_load_result_n1665 = hxc_l_left_hb678ab451d4a;
          int32_t hxc_l_tmp_load_result_n1666 = hxc_l_top_h210b80880625;
          DrawRectangleLines((int32_t)hxc_l_tmp_load_result_n1665, (int32_t)hxc_l_tmp_load_result_n1666, (int32_t)360, (int32_t)54, hxc_l_this1_h68ec3fcab7a7);
          struct Color hxc_l_this1_h776f3fda5b59 = (struct Color){ .r = 242, .g = 249, .b = 245, .a = 255 };
          struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_tmp_load_result_n1669 = hxc_l_uiCatalog;
          hxc_l_gc_roots[544] = (const void *)hxc_l_tmp_load_result_n1669;
          if (hxc_l_tmp_load_result_n1669 == NULL)
          {
            abort();
          }
          hxc_string hxc_l_tmp_instance_call_result_n1671 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_tmp_load_result_n1669, hxc_l_locale, (hxc_string){ (const uint8_t *)"menu_editor", 11, true, NULL });
          hxc_l_tmp_cstring_arg_owner_owner_n489 = hxc_l_tmp_instance_call_result_n1671;
          hxc_string hxc_l_tmp_cstring_arg_owner_borrow_result_n1672 = hxc_l_tmp_cstring_arg_owner_owner_n489;
          int32_t hxc_l_tmp_load_result_n1673 = hxc_l_left_hb678ab451d4a;
          int32_t hxc_l_tmp_load_result_n1674 = hxc_l_top_h210b80880625;
          struct Color hxc_l_tmp_load_result_n1675 = hxc_l_this1_h776f3fda5b59;
          hxc_call_cstring hxc_l_tmp_string_prepare_cstring_result_n1676 = HXC_CALL_CSTRING_INITIALIZER;
          if (hxc_string_prepare_call_cstring(&hxc_l_tmp_cstring_arg_owner_borrow_result_n1672, hxc_default_allocator(), &hxc_l_tmp_string_prepare_cstring_result_n1676) != HXC_STATUS_OK)
          {
            abort();
          }
          DrawText(hxc_l_tmp_string_prepare_cstring_result_n1676.data, (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n1673, 28), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n1674, 16), (int32_t)21, hxc_l_tmp_load_result_n1675);
          if (hxc_call_cstring_dispose(&hxc_l_tmp_string_prepare_cstring_result_n1676) != HXC_STATUS_OK)
          {
            abort();
          }
          hxc_string hxc_l_tmp_call_result_n1678 = hxc_caxecraft_input_ControlPrompts_menuInstructionsMessage(hxc_l_promptDevice);
          hxc_l_message_hfdae3ea29a06 = hxc_l_tmp_call_result_n1678;
          int32_t hxc_l_x_h8aa9aa7f6dfc = hxc_i32_subtract_wrapping(hxc_l_width_h3cc5c1d53e37 / 2, 330);
          struct Color hxc_l_this1_h0094ae31b346 = (struct Color){ .r = 229, .g = 241, .b = 235, .a = 255 };
          struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_tmp_load_result_n1681 = hxc_l_uiCatalog;
          hxc_l_gc_roots[545] = (const void *)hxc_l_tmp_load_result_n1681;
          if (hxc_l_tmp_load_result_n1681 == NULL)
          {
            abort();
          }
          int32_t hxc_l_tmp_load_result_n1682 = hxc_l_locale;
          hxc_string hxc_l_tmp_instance_call_result_n1684 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_tmp_load_result_n1681, hxc_l_tmp_load_result_n1682, hxc_l_message_hfdae3ea29a06);
          hxc_l_tmp_cstring_arg_owner_owner_n493 = hxc_l_tmp_instance_call_result_n1684;
          hxc_string hxc_l_tmp_cstring_arg_owner_borrow_result_n1685 = hxc_l_tmp_cstring_arg_owner_owner_n493;
          int32_t hxc_l_tmp_load_result_n1686 = hxc_l_x_h8aa9aa7f6dfc;
          int32_t hxc_l_tmp_load_result_n1687 = hxc_l_height_haa1fb843124b;
          struct Color hxc_l_tmp_load_result_n1688 = hxc_l_this1_h0094ae31b346;
          hxc_call_cstring hxc_l_tmp_string_prepare_cstring_result_n1689 = HXC_CALL_CSTRING_INITIALIZER;
          if (hxc_string_prepare_call_cstring(&hxc_l_tmp_cstring_arg_owner_borrow_result_n1685, hxc_default_allocator(), &hxc_l_tmp_string_prepare_cstring_result_n1689) != HXC_STATUS_OK)
          {
            abort();
          }
          DrawText(hxc_l_tmp_string_prepare_cstring_result_n1689.data, (int32_t)hxc_l_tmp_load_result_n1686, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n1687, 58), (int32_t)16, hxc_l_tmp_load_result_n1688);
          if (hxc_call_cstring_dispose(&hxc_l_tmp_string_prepare_cstring_result_n1689) != HXC_STATUS_OK)
          {
            abort();
          }
          hxc_string hxc_l_tmp_call_result_n1691 = hxc_caxecraft_input_ControlPrompts_controlsMessage(hxc_l_promptDevice);
          hxc_l_message_hb89ed1638090 = hxc_l_tmp_call_result_n1691;
          struct Color hxc_l_this1_h9fcc4808f5b3 = (struct Color){ .r = 142, .g = 210, .b = 205, .a = 255 };
          struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_tmp_load_result_n1693 = hxc_l_uiCatalog;
          hxc_l_gc_roots[546] = (const void *)hxc_l_tmp_load_result_n1693;
          if (hxc_l_tmp_load_result_n1693 == NULL)
          {
            abort();
          }
          int32_t hxc_l_tmp_load_result_n1694 = hxc_l_locale;
          hxc_string hxc_l_tmp_instance_call_result_n1696 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_tmp_load_result_n1693, hxc_l_tmp_load_result_n1694, hxc_l_message_hb89ed1638090);
          hxc_l_tmp_cstring_arg_owner_owner_n496 = hxc_l_tmp_instance_call_result_n1696;
          hxc_string hxc_l_tmp_cstring_arg_owner_borrow_result_n1697 = hxc_l_tmp_cstring_arg_owner_owner_n496;
          int32_t hxc_l_tmp_load_result_n1698 = hxc_l_height_haa1fb843124b;
          struct Color hxc_l_tmp_load_result_n1699 = hxc_l_this1_h9fcc4808f5b3;
          hxc_call_cstring hxc_l_tmp_string_prepare_cstring_result_n1700 = HXC_CALL_CSTRING_INITIALIZER;
          if (hxc_string_prepare_call_cstring(&hxc_l_tmp_cstring_arg_owner_borrow_result_n1697, hxc_default_allocator(), &hxc_l_tmp_string_prepare_cstring_result_n1700) != HXC_STATUS_OK)
          {
            abort();
          }
          DrawText(hxc_l_tmp_string_prepare_cstring_result_n1700.data, (int32_t)18, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n1698, 30), (int32_t)13, hxc_l_tmp_load_result_n1699);
          if (hxc_call_cstring_dispose(&hxc_l_tmp_string_prepare_cstring_result_n1700) != HXC_STATUS_OK)
          {
            abort();
          }
          int32_t hxc_l_tmp_load_result_n1701 = hxc_l_selectedMode;
          bool hxc_l_tmp_short_circuit_result_n497 = hxc_l_tmp_load_result_n1701 == 1;
          if (hxc_l_tmp_load_result_n1701 == 1)
          {
            hxc_string hxc_l_tmp_load_result_n1702 = hxc_l_adventureTagline;
            if (hxc_l_tmp_load_result_n1702.data == NULL)
            {
              abort();
            }
            int32_t hxc_l_tmp_string_length_result_n1703;
            if (hxc_string_haxe_length(hxc_l_tmp_load_result_n1702, &hxc_l_tmp_string_length_result_n1703) != HXC_STATUS_OK)
            {
              abort();
            }
            hxc_l_tmp_short_circuit_result_n497 = hxc_l_tmp_string_length_result_n1703 > 0;
          }
          if (hxc_l_tmp_short_circuit_result_n497)
          {
            int32_t hxc_l_x_hf371c640ecdf = hxc_i32_subtract_wrapping(hxc_l_width_h3cc5c1d53e37 / 2, 230);
            struct Color hxc_l_this1_h3d529d3bfad9 = (struct Color){ .r = 255, .g = 205, .b = 91, .a = 255 };
            hxc_string hxc_l_tmp_load_result_n1707 = hxc_l_adventureTagline;
            int32_t hxc_l_tmp_load_result_n1708 = hxc_l_x_hf371c640ecdf;
            int32_t hxc_l_tmp_load_result_n1709 = hxc_l_firstTop;
            struct Color hxc_l_tmp_load_result_n1710 = hxc_l_this1_h3d529d3bfad9;
            hxc_call_cstring hxc_l_tmp_string_prepare_cstring_result_n1711 = HXC_CALL_CSTRING_INITIALIZER;
            if (hxc_string_prepare_call_cstring(&hxc_l_tmp_load_result_n1707, hxc_default_allocator(), &hxc_l_tmp_string_prepare_cstring_result_n1711) != HXC_STATUS_OK)
            {
              abort();
            }
            DrawText(hxc_l_tmp_string_prepare_cstring_result_n1711.data, (int32_t)hxc_l_tmp_load_result_n1708, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n1709, 36), (int32_t)17, hxc_l_tmp_load_result_n1710);
            if (hxc_call_cstring_dispose(&hxc_l_tmp_string_prepare_cstring_result_n1711) != HXC_STATUS_OK)
            {
              abort();
            }
          }
          if (hxc_string_release(&hxc_l_tmp_cstring_arg_owner_owner_n496) != HXC_STATUS_OK)
          {
            abort();
          }
          if (hxc_string_release(&hxc_l_message_hb89ed1638090) != HXC_STATUS_OK)
          {
            abort();
          }
          if (hxc_string_release(&hxc_l_tmp_cstring_arg_owner_owner_n493) != HXC_STATUS_OK)
          {
            abort();
          }
          if (hxc_string_release(&hxc_l_message_hfdae3ea29a06) != HXC_STATUS_OK)
          {
            abort();
          }
          if (hxc_string_release(&hxc_l_tmp_cstring_arg_owner_owner_n489) != HXC_STATUS_OK)
          {
            abort();
          }
          if (hxc_string_release(&hxc_l_tmp_cstring_arg_owner_owner_n483) != HXC_STATUS_OK)
          {
            abort();
          }
          if (hxc_string_release(&hxc_l_tmp_cstring_arg_owner_owner_n475) != HXC_STATUS_OK)
          {
            abort();
          }
          if (hxc_string_release(&hxc_l_adventureTagline) != HXC_STATUS_OK)
          {
            abort();
          }
          if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n445) != HXC_STATUS_OK)
          {
            abort();
          }
        }
        else
        {
          if (hxc_l_onCampaignSelect)
          {
            hxc_l_gc_roots[547] = (const void *)hxc_l_campaign;
            struct hxc_caxecraft_content_CampaignManifest *hxc_l_selectedCampaign_h8366dc3c49fe = hxc_l_campaign;
            hxc_l_gc_roots[548] = (const void *)hxc_l_selectedCampaign_h8366dc3c49fe;
            hxc_l_gc_roots[549] = (const void *)NULL;
            if (hxc_l_selectedCampaign_h8366dc3c49fe == NULL)
            {
              enum hxc_caxecraft_app_AppScreen hxc_l_tmp_call_result_n1716 = hxc_caxecraft_app_AppScreen_closeCampaignSelection(hxc_l_screen);
              hxc_l_screen = hxc_l_tmp_call_result_n1716;
            }
            else
            {
              hxc_string hxc_l_locale1_h94b5f49c9c4c = { 0 };
              switch (hxc_l_locale) {
                case 0:
                  {
                    hxc_l_locale1_h94b5f49c9c4c = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
                    if (hxc_string_retain(hxc_l_locale1_h94b5f49c9c4c) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    break;
                  }
                case 1:
                  {
                    hxc_l_locale1_h94b5f49c9c4c = (hxc_string){ (const uint8_t *)"es-mx", 5, true, NULL };
                    if (hxc_string_retain(hxc_l_locale1_h94b5f49c9c4c) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    break;
                  }
                default:
                  {
                    hxc_l_locale1_h94b5f49c9c4c = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
                    if (hxc_string_retain(hxc_l_locale1_h94b5f49c9c4c) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    break;
                  }
              }
              hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n1718 = hxc_l_locale1_h94b5f49c9c4c;
              hxc_l_tmp_managed_flow_owner_n502 = hxc_l_tmp_managed_flow_carrier_move_result_n1718;
              struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1719 = hxc_l_levelView_hb85a9a956786;
              hxc_l_gc_roots[550] = (const void *)hxc_l_tmp_load_result_n1719;
              if (hxc_l_tmp_load_result_n1719 == NULL)
              {
                abort();
              }
              struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_class_field_load_result_n1720 = (*hxc_l_tmp_load_result_n1719).hxc_levelPresentation;
              hxc_l_gc_roots[551] = (const void *)hxc_l_tmp_class_field_load_result_n1720;
              struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_this_h0895f1546c18 = hxc_l_tmp_class_field_load_result_n1720;
              struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_load_result_n1721 = hxc_l_this_h0895f1546c18;
              hxc_l_gc_roots[552] = (const void *)hxc_l_tmp_load_result_n1721;
              if (hxc_l_tmp_load_result_n1721 == NULL)
              {
                abort();
              }
              struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_load_result_n1722 = hxc_l_this_h0895f1546c18;
              hxc_l_gc_roots[553] = (const void *)hxc_l_tmp_load_result_n1722;
              if (hxc_l_tmp_load_result_n1722 == NULL)
              {
                abort();
              }
              struct hxc_caxecraft_scenario_ScenarioText hxc_l_tmp_class_field_load_result_n1723 = (*hxc_l_tmp_load_result_n1722).hxc_title;
              hxc_string hxc_l_tmp_instance_call_result_n1725 = hxc_caxecraft_content_RuntimeLevelPresentation_resolve(hxc_l_tmp_load_result_n1721, hxc_l_tmp_class_field_load_result_n1723, hxc_l_tmp_managed_flow_owner_n502);
              hxc_l_tmp = hxc_l_tmp_instance_call_result_n1725;
              hxc_string hxc_l_locale1_h975d1872215a = { 0 };
              switch (hxc_l_locale) {
                case 0:
                  {
                    hxc_l_locale1_h975d1872215a = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
                    if (hxc_string_retain(hxc_l_locale1_h975d1872215a) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    break;
                  }
                case 1:
                  {
                    hxc_l_locale1_h975d1872215a = (hxc_string){ (const uint8_t *)"es-mx", 5, true, NULL };
                    if (hxc_string_retain(hxc_l_locale1_h975d1872215a) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    break;
                  }
                default:
                  {
                    hxc_l_locale1_h975d1872215a = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
                    if (hxc_string_retain(hxc_l_locale1_h975d1872215a) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    break;
                  }
              }
              hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n1727 = hxc_l_locale1_h975d1872215a;
              hxc_l_tmp_managed_flow_owner_n506 = hxc_l_tmp_managed_flow_carrier_move_result_n1727;
              struct Texture hxc_l_tmp_load_result_n1728 = hxc_l_titleTexture;
              bool hxc_l_tmp_load_result_n1729 = hxc_l_titleTextureReady;
              struct Texture hxc_l_tmp_load_result_n1730 = hxc_l_wordmarkTexture;
              bool hxc_l_tmp_load_result_n1731 = hxc_l_wordmarkTextureReady;
              struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n1732 = hxc_l_selectedCampaign_h8366dc3c49fe;
              hxc_l_gc_roots[554] = (const void *)hxc_l_tmp_load_result_n1732;
              int32_t hxc_l_tmp_load_result_n1733 = hxc_l_locale;
              struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_tmp_load_result_n1734 = hxc_l_uiCatalog;
              hxc_l_gc_roots[555] = (const void *)hxc_l_tmp_load_result_n1734;
              int32_t hxc_l_tmp_load_result_n1735 = hxc_l_selectedCampaignLevelIndex;
              hxc_string hxc_l_tmp_load_result_n1736 = hxc_l_tmp;
              struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1737 = hxc_l_levelView_hb85a9a956786;
              hxc_l_gc_roots[556] = (const void *)hxc_l_tmp_load_result_n1737;
              if (hxc_l_tmp_load_result_n1737 == NULL)
              {
                abort();
              }
              struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_class_field_load_result_n1738 = (*hxc_l_tmp_load_result_n1737).hxc_levelPresentation;
              hxc_l_gc_roots[557] = (const void *)hxc_l_tmp_class_field_load_result_n1738;
              if (hxc_l_tmp_class_field_load_result_n1738 == NULL)
              {
                abort();
              }
              hxc_string hxc_l_tmp_instance_call_result_n1740 = hxc_caxecraft_content_RuntimeLevelPresentation_message(hxc_l_tmp_class_field_load_result_n1738, (hxc_string){ (const uint8_t *)"adventure_tagline", 17, true, NULL }, hxc_l_tmp_managed_flow_owner_n506);
              hxc_l_tmp_static_call_argument_9_owner_n507 = hxc_l_tmp_instance_call_result_n1740;
              hxc_string hxc_l_tmp_static_call_argument_9_borrow_result_n1741 = hxc_l_tmp_static_call_argument_9_owner_n507;
              hxc_caxecraft_app_CampaignMenu_draw(hxc_l_tmp_load_result_n1728, hxc_l_tmp_load_result_n1729, hxc_l_tmp_load_result_n1730, hxc_l_tmp_load_result_n1731, hxc_l_tmp_load_result_n1732, hxc_l_tmp_load_result_n1733, hxc_l_tmp_load_result_n1734, hxc_l_tmp_load_result_n1735, hxc_l_tmp_load_result_n1736, hxc_l_tmp_static_call_argument_9_borrow_result_n1741, hxc_l_promptDevice);
              if (hxc_string_release(&hxc_l_tmp_static_call_argument_9_owner_n507) != HXC_STATUS_OK)
              {
                abort();
              }
              if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n506) != HXC_STATUS_OK)
              {
                abort();
              }
              if (hxc_string_release(&hxc_l_tmp) != HXC_STATUS_OK)
              {
                abort();
              }
              if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n502) != HXC_STATUS_OK)
              {
                abort();
              }
            }
          }
          else
          {
            if (hxc_l_onLoading)
            {
              hxc_string hxc_l_tmp_load_result_n1744 = hxc_l_pendingCampaignLabel;
              int32_t hxc_l_tmp_load_result_n1745 = hxc_l_locale;
              hxc_l_gc_roots[558] = (const void *)hxc_l_uiCatalog;
              hxc_caxecraft_app_CaxecraftApp_drawCampaignLoading(hxc_l_tmp_load_result_n1744, hxc_l_tmp_load_result_n1745, hxc_l_uiCatalog);
            }
            else
            {
              if (hxc_l_onEditor)
              {
                struct hxc_caxecraft_app_CaxecraftEditorScreen *hxc_l_tmp_load_result_n1748 = hxc_l_editorScreen;
                hxc_l_gc_roots[559] = (const void *)hxc_l_tmp_load_result_n1748;
                if (hxc_l_tmp_load_result_n1748 == NULL)
                {
                  abort();
                }
                int32_t hxc_l_tmp_load_result_n1749 = hxc_l_locale;
                int32_t hxc_l_tmp_load_result_n1750 = hxc_l_editorNavigationCommand;
                struct hxc_caxecraft_app_HudResources hxc_l_tmp_load_result_n1751 = hxc_l_hudResources;
                struct Texture hxc_l_tmp_load_result_n1752 = hxc_l_terrainTexture;
                bool hxc_l_tmp_load_result_n1753 = hxc_l_terrainTextureReady;
                struct Texture hxc_l_tmp_load_result_n1754 = hxc_l_adventureTerrainTexture;
                bool hxc_l_tmp_load_result_n1755 = hxc_l_adventureTerrainTextureReady;
                hxc_l_gc_roots[560] = (const void *)hxc_l_runtimeTextures;
                hxc_l_gc_roots[561] = (const void *)(struct hxc_caxecraft_app_EditorRenderResources){ .hxc_adventureTerrainTexture = hxc_l_tmp_load_result_n1754, .hxc_adventureTerrainTextureReady = hxc_l_tmp_load_result_n1755, .hxc_runtimeTextures = hxc_l_runtimeTextures, .hxc_shared = hxc_l_tmp_load_result_n1751, .hxc_terrainTexture = hxc_l_tmp_load_result_n1752, .hxc_terrainTextureReady = hxc_l_tmp_load_result_n1753 }.hxc_runtimeTextures;
                struct hxc_caxecraft_app_EditorScreenAction hxc_l_tmp_instance_call_result_n1758 = hxc_caxecraft_app_CaxecraftEditorScreen_draw(hxc_l_tmp_load_result_n1748, hxc_l_tmp_load_result_n1749, hxc_l_tmp_load_result_n1750, (struct hxc_caxecraft_app_EditorRenderResources){ .hxc_adventureTerrainTexture = hxc_l_tmp_load_result_n1754, .hxc_adventureTerrainTextureReady = hxc_l_tmp_load_result_n1755, .hxc_runtimeTextures = hxc_l_runtimeTextures, .hxc_shared = hxc_l_tmp_load_result_n1751, .hxc_terrainTexture = hxc_l_tmp_load_result_n1752, .hxc_terrainTextureReady = hxc_l_tmp_load_result_n1753 });
                hxc_enum_59188943_destroy(&hxc_l_editorActionForFrame);
                hxc_l_editorActionForFrame = hxc_l_tmp_instance_call_result_n1758;
              }
              else
              {
                struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1759 = hxc_l_levelView_hb85a9a956786;
                hxc_l_gc_roots[562] = (const void *)hxc_l_tmp_load_result_n1759;
                if (hxc_l_tmp_load_result_n1759 == NULL)
                {
                  abort();
                }
                struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_class_field_load_result_n1760 = (*hxc_l_tmp_load_result_n1759).hxc_levelPresentation;
                hxc_l_gc_roots[563] = (const void *)hxc_l_tmp_class_field_load_result_n1760;
                if (hxc_l_tmp_class_field_load_result_n1760 == NULL)
                {
                  abort();
                }
                bool hxc_l_tmp_class_field_load_result_n1761 = (*hxc_l_tmp_class_field_load_result_n1760).hxc_environmentAuthored;
                bool hxc_l_hasEnvironment = hxc_l_tmp_class_field_load_result_n1761;
                struct Color hxc_l_color = { 0 };
                if (hxc_l_hasEnvironment)
                {
                  struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1763 = hxc_l_levelView_hb85a9a956786;
                  hxc_l_gc_roots[564] = (const void *)hxc_l_tmp_load_result_n1763;
                  if (hxc_l_tmp_load_result_n1763 == NULL)
                  {
                    abort();
                  }
                  struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_class_field_load_result_n1764 = (*hxc_l_tmp_load_result_n1763).hxc_levelPresentation;
                  hxc_l_gc_roots[565] = (const void *)hxc_l_tmp_class_field_load_result_n1764;
                  if (hxc_l_tmp_class_field_load_result_n1764 == NULL)
                  {
                    abort();
                  }
                  int32_t hxc_l_tmp_class_field_load_result_n1765 = (*hxc_l_tmp_class_field_load_result_n1764).hxc_environmentSkyRedValue;
                  int32_t hxc_l_r = hxc_l_tmp_class_field_load_result_n1765;
                  struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1766 = hxc_l_levelView_hb85a9a956786;
                  hxc_l_gc_roots[566] = (const void *)hxc_l_tmp_load_result_n1766;
                  if (hxc_l_tmp_load_result_n1766 == NULL)
                  {
                    abort();
                  }
                  struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_class_field_load_result_n1767 = (*hxc_l_tmp_load_result_n1766).hxc_levelPresentation;
                  hxc_l_gc_roots[567] = (const void *)hxc_l_tmp_class_field_load_result_n1767;
                  if (hxc_l_tmp_class_field_load_result_n1767 == NULL)
                  {
                    abort();
                  }
                  int32_t hxc_l_tmp_class_field_load_result_n1768 = (*hxc_l_tmp_class_field_load_result_n1767).hxc_environmentSkyGreenValue;
                  int32_t hxc_l_g_hb97f51df9642 = hxc_l_tmp_class_field_load_result_n1768;
                  struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1769 = hxc_l_levelView_hb85a9a956786;
                  hxc_l_gc_roots[568] = (const void *)hxc_l_tmp_load_result_n1769;
                  if (hxc_l_tmp_load_result_n1769 == NULL)
                  {
                    abort();
                  }
                  struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_class_field_load_result_n1770 = (*hxc_l_tmp_load_result_n1769).hxc_levelPresentation;
                  hxc_l_gc_roots[569] = (const void *)hxc_l_tmp_class_field_load_result_n1770;
                  if (hxc_l_tmp_class_field_load_result_n1770 == NULL)
                  {
                    abort();
                  }
                  int32_t hxc_l_tmp_class_field_load_result_n1771 = (*hxc_l_tmp_class_field_load_result_n1770).hxc_environmentSkyBlueValue;
                  int32_t hxc_l_b = hxc_l_tmp_class_field_load_result_n1771;
                  int32_t hxc_l_a = 255;
                  int32_t hxc_l_tmp_load_result_n1772 = hxc_l_r;
                  int32_t hxc_l_tmp_conditional_result_n515 = 0;
                  if (hxc_l_tmp_load_result_n1772 < 0)
                  {
                    hxc_l_tmp_conditional_result_n515 = 0;
                  }
                  else
                  {
                    int32_t hxc_l_tmp_load_result_n1773 = hxc_l_r;
                    int32_t hxc_l_tmp_conditional_result_n516 = 0;
                    if (hxc_l_tmp_load_result_n1773 > 255)
                    {
                      hxc_l_tmp_conditional_result_n516 = 255;
                    }
                    else
                    {
                      hxc_l_tmp_conditional_result_n516 = hxc_l_r;
                    }
                    hxc_l_tmp_conditional_result_n515 = hxc_l_tmp_conditional_result_n516;
                  }
                  uint8_t hxc_l_red_h84b1a6bd75d3 = (uint8_t)hxc_l_tmp_conditional_result_n515;
                  int32_t hxc_l_tmp_load_result_n1777 = hxc_l_g_hb97f51df9642;
                  int32_t hxc_l_tmp_conditional_result_n518 = 0;
                  if (hxc_l_tmp_load_result_n1777 < 0)
                  {
                    hxc_l_tmp_conditional_result_n518 = 0;
                  }
                  else
                  {
                    int32_t hxc_l_tmp_load_result_n1778 = hxc_l_g_hb97f51df9642;
                    int32_t hxc_l_tmp_conditional_result_n519 = 0;
                    if (hxc_l_tmp_load_result_n1778 > 255)
                    {
                      hxc_l_tmp_conditional_result_n519 = 255;
                    }
                    else
                    {
                      hxc_l_tmp_conditional_result_n519 = hxc_l_g_hb97f51df9642;
                    }
                    hxc_l_tmp_conditional_result_n518 = hxc_l_tmp_conditional_result_n519;
                  }
                  uint8_t hxc_l_green_hbdb635887f56 = (uint8_t)hxc_l_tmp_conditional_result_n518;
                  int32_t hxc_l_tmp_load_result_n1782 = hxc_l_b;
                  int32_t hxc_l_tmp_conditional_result_n521 = 0;
                  if (hxc_l_tmp_load_result_n1782 < 0)
                  {
                    hxc_l_tmp_conditional_result_n521 = 0;
                  }
                  else
                  {
                    int32_t hxc_l_tmp_load_result_n1783 = hxc_l_b;
                    int32_t hxc_l_tmp_conditional_result_n522 = 0;
                    if (hxc_l_tmp_load_result_n1783 > 255)
                    {
                      hxc_l_tmp_conditional_result_n522 = 255;
                    }
                    else
                    {
                      hxc_l_tmp_conditional_result_n522 = hxc_l_b;
                    }
                    hxc_l_tmp_conditional_result_n521 = hxc_l_tmp_conditional_result_n522;
                  }
                  uint8_t hxc_l_blue_hf29891f24903 = (uint8_t)hxc_l_tmp_conditional_result_n521;
                  int32_t hxc_l_tmp_load_result_n1787 = hxc_l_a;
                  int32_t hxc_l_tmp_conditional_result_n524 = 0;
                  if (hxc_l_tmp_load_result_n1787 < 0)
                  {
                    hxc_l_tmp_conditional_result_n524 = 0;
                  }
                  else
                  {
                    int32_t hxc_l_tmp_load_result_n1788 = hxc_l_a;
                    int32_t hxc_l_tmp_conditional_result_n525 = 0;
                    if (hxc_l_tmp_load_result_n1788 > 255)
                    {
                      hxc_l_tmp_conditional_result_n525 = 255;
                    }
                    else
                    {
                      hxc_l_tmp_conditional_result_n525 = hxc_l_a;
                    }
                    hxc_l_tmp_conditional_result_n524 = hxc_l_tmp_conditional_result_n525;
                  }
                  uint8_t hxc_l_alpha_h837c3d7cc3e1 = (uint8_t)hxc_l_tmp_conditional_result_n524;
                  uint8_t hxc_l_tmp_load_result_n1792 = hxc_l_red_h84b1a6bd75d3;
                  uint8_t hxc_l_tmp_load_result_n1793 = hxc_l_green_hbdb635887f56;
                  uint8_t hxc_l_tmp_load_result_n1794 = hxc_l_blue_hf29891f24903;
                  hxc_l_color = (struct Color){ .r = hxc_l_tmp_load_result_n1792, .g = hxc_l_tmp_load_result_n1793, .b = hxc_l_tmp_load_result_n1794, .a = hxc_l_alpha_h837c3d7cc3e1 };
                }
                else
                {
                  hxc_l_color = (struct Color){ .r = 126, .g = 190, .b = 201, .a = 255 };
                }
                ClearBackground(hxc_l_color);
                BeginMode3D(hxc_l_camera);
                hxc_l_gc_roots[570] = (const void *)hxc_l_preparedPortal;
                struct hxc_caxecraft_app_PreparedCampaignPortal *hxc_l_portal_hcef399961d30 = hxc_l_preparedPortal;
                if (hxc_l_screen == hxc_caxecraft_app_AppScreen_EditorTestPlay)
                {
                  hxc_l_gc_roots[571] = (const void *)NULL;
                  hxc_l_portal_hcef399961d30 = NULL;
                }
                bool hxc_l_portalVisible = false;
                hxc_l_gc_roots[572] = (const void *)hxc_l_portal_hcef399961d30;
                hxc_l_gc_roots[573] = (const void *)NULL;
                if (hxc_l_portal_hcef399961d30 != NULL)
                {
                  double hxc_l_tmp_record_field_load_result_n1804 = hxc_l_renderPosition.hxc_x;
                  struct hxc_caxecraft_app_PreparedCampaignPortal *hxc_l_tmp_load_result_n1805 = hxc_l_portal_hcef399961d30;
                  hxc_l_gc_roots[574] = (const void *)hxc_l_tmp_load_result_n1805;
                  if (hxc_l_tmp_load_result_n1805 == NULL)
                  {
                    abort();
                  }
                  double hxc_l_tmp_class_field_load_result_n1806 = (*hxc_l_tmp_load_result_n1805).hxc_sourceCenterXValue;
                  double hxc_l_dx = hxc_l_tmp_record_field_load_result_n1804 - hxc_l_tmp_class_field_load_result_n1806;
                  double hxc_l_tmp_record_field_load_result_n1807 = hxc_l_renderPosition.hxc_z;
                  struct hxc_caxecraft_app_PreparedCampaignPortal *hxc_l_tmp_load_result_n1808 = hxc_l_portal_hcef399961d30;
                  hxc_l_gc_roots[575] = (const void *)hxc_l_tmp_load_result_n1808;
                  if (hxc_l_tmp_load_result_n1808 == NULL)
                  {
                    abort();
                  }
                  double hxc_l_tmp_class_field_load_result_n1809 = (*hxc_l_tmp_load_result_n1808).hxc_sourceCenterZValue;
                  double hxc_l_dz = hxc_l_tmp_record_field_load_result_n1807 - hxc_l_tmp_class_field_load_result_n1809;
                  double hxc_l_tmp_load_result_n1810 = hxc_l_dx;
                  double hxc_l_tmp_load_result_n1811 = hxc_l_dx;
                  double hxc_l_tmp_load_result_n1812 = hxc_l_dz;
                  hxc_l_portalVisible = hxc_l_tmp_load_result_n1810 * hxc_l_tmp_load_result_n1811 + hxc_l_tmp_load_result_n1812 * hxc_l_dz <= 100.0;
                }
                else
                {
                  hxc_l_portalVisible = false;
                }
                if (hxc_l_hasEnvironment)
                {
                  struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1815 = hxc_l_levelView_hb85a9a956786;
                  hxc_l_gc_roots[576] = (const void *)hxc_l_tmp_load_result_n1815;
                  if (hxc_l_tmp_load_result_n1815 == NULL)
                  {
                    abort();
                  }
                  struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_class_field_load_result_n1816 = (*hxc_l_tmp_load_result_n1815).hxc_levelPresentation;
                  hxc_l_gc_roots[577] = (const void *)hxc_l_tmp_class_field_load_result_n1816;
                  if (hxc_l_tmp_class_field_load_result_n1816 == NULL)
                  {
                    abort();
                  }
                  bool hxc_l_tmp_class_field_load_result_n1817 = (*hxc_l_tmp_class_field_load_result_n1816).hxc_environmentHasSunValue;
                  struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1818 = hxc_l_levelView_hb85a9a956786;
                  hxc_l_gc_roots[578] = (const void *)hxc_l_tmp_load_result_n1818;
                  if (hxc_l_tmp_load_result_n1818 == NULL)
                  {
                    abort();
                  }
                  struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_class_field_load_result_n1819 = (*hxc_l_tmp_load_result_n1818).hxc_levelPresentation;
                  hxc_l_gc_roots[579] = (const void *)hxc_l_tmp_class_field_load_result_n1819;
                  if (hxc_l_tmp_class_field_load_result_n1819 == NULL)
                  {
                    abort();
                  }
                  int32_t hxc_l_tmp_class_field_load_result_n1820 = (*hxc_l_tmp_class_field_load_result_n1819).hxc_environmentSunXValue;
                  struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1821 = hxc_l_levelView_hb85a9a956786;
                  hxc_l_gc_roots[580] = (const void *)hxc_l_tmp_load_result_n1821;
                  if (hxc_l_tmp_load_result_n1821 == NULL)
                  {
                    abort();
                  }
                  struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_class_field_load_result_n1822 = (*hxc_l_tmp_load_result_n1821).hxc_levelPresentation;
                  hxc_l_gc_roots[581] = (const void *)hxc_l_tmp_class_field_load_result_n1822;
                  if (hxc_l_tmp_class_field_load_result_n1822 == NULL)
                  {
                    abort();
                  }
                  int32_t hxc_l_tmp_class_field_load_result_n1823 = (*hxc_l_tmp_class_field_load_result_n1822).hxc_environmentSunYValue;
                  struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1824 = hxc_l_levelView_hb85a9a956786;
                  hxc_l_gc_roots[582] = (const void *)hxc_l_tmp_load_result_n1824;
                  if (hxc_l_tmp_load_result_n1824 == NULL)
                  {
                    abort();
                  }
                  struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_class_field_load_result_n1825 = (*hxc_l_tmp_load_result_n1824).hxc_levelPresentation;
                  hxc_l_gc_roots[583] = (const void *)hxc_l_tmp_class_field_load_result_n1825;
                  if (hxc_l_tmp_class_field_load_result_n1825 == NULL)
                  {
                    abort();
                  }
                  int32_t hxc_l_tmp_class_field_load_result_n1826 = (*hxc_l_tmp_class_field_load_result_n1825).hxc_environmentSunZValue;
                  struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1827 = hxc_l_levelView_hb85a9a956786;
                  hxc_l_gc_roots[584] = (const void *)hxc_l_tmp_load_result_n1827;
                  if (hxc_l_tmp_load_result_n1827 == NULL)
                  {
                    abort();
                  }
                  struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_class_field_load_result_n1828 = (*hxc_l_tmp_load_result_n1827).hxc_levelPresentation;
                  hxc_l_gc_roots[585] = (const void *)hxc_l_tmp_class_field_load_result_n1828;
                  if (hxc_l_tmp_class_field_load_result_n1828 == NULL)
                  {
                    abort();
                  }
                  int32_t hxc_l_tmp_class_field_load_result_n1829 = (*hxc_l_tmp_class_field_load_result_n1828).hxc_environmentSunRadiusMilliValue;
                  struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1830 = hxc_l_levelView_hb85a9a956786;
                  hxc_l_gc_roots[586] = (const void *)hxc_l_tmp_load_result_n1830;
                  if (hxc_l_tmp_load_result_n1830 == NULL)
                  {
                    abort();
                  }
                  struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_class_field_load_result_n1831 = (*hxc_l_tmp_load_result_n1830).hxc_levelPresentation;
                  hxc_l_gc_roots[587] = (const void *)hxc_l_tmp_class_field_load_result_n1831;
                  if (hxc_l_tmp_class_field_load_result_n1831 == NULL)
                  {
                    abort();
                  }
                  int32_t hxc_l_tmp_class_field_load_result_n1832 = (*hxc_l_tmp_class_field_load_result_n1831).hxc_environmentCloudCountValue;
                  struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1833 = hxc_l_levelView_hb85a9a956786;
                  hxc_l_gc_roots[588] = (const void *)hxc_l_tmp_load_result_n1833;
                  if (hxc_l_tmp_load_result_n1833 == NULL)
                  {
                    abort();
                  }
                  struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_class_field_load_result_n1834 = (*hxc_l_tmp_load_result_n1833).hxc_levelPresentation;
                  hxc_l_gc_roots[589] = (const void *)hxc_l_tmp_class_field_load_result_n1834;
                  if (hxc_l_tmp_class_field_load_result_n1834 == NULL)
                  {
                    abort();
                  }
                  int32_t hxc_l_tmp_class_field_load_result_n1835 = (*hxc_l_tmp_class_field_load_result_n1834).hxc_environmentCloudSpeedMilliValue;
                  struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1836 = hxc_l_levelView_hb85a9a956786;
                  hxc_l_gc_roots[590] = (const void *)hxc_l_tmp_load_result_n1836;
                  if (hxc_l_tmp_load_result_n1836 == NULL)
                  {
                    abort();
                  }
                  struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_class_field_load_result_n1837 = (*hxc_l_tmp_load_result_n1836).hxc_levelPresentation;
                  hxc_l_gc_roots[591] = (const void *)hxc_l_tmp_class_field_load_result_n1837;
                  if (hxc_l_tmp_class_field_load_result_n1837 == NULL)
                  {
                    abort();
                  }
                  int32_t hxc_l_tmp_class_field_load_result_n1838 = (*hxc_l_tmp_class_field_load_result_n1837).hxc_environmentCloudSeedValue;
                  double hxc_l_tmp_load_result_n1839 = hxc_l_eyeX;
                  double hxc_l_tmp_load_result_n1840 = hxc_l_eyeY;
                  double hxc_l_tmp_load_result_n1841 = hxc_l_eyeZ;
                  hxc_caxecraft_app_CaxecraftApp_drawWorldSunAndClouds(hxc_l_tmp_class_field_load_result_n1817, hxc_l_tmp_class_field_load_result_n1820, hxc_l_tmp_class_field_load_result_n1823, hxc_l_tmp_class_field_load_result_n1826, hxc_l_tmp_class_field_load_result_n1829, hxc_l_tmp_class_field_load_result_n1832, hxc_l_tmp_class_field_load_result_n1835, hxc_l_tmp_class_field_load_result_n1838, hxc_l_tmp_load_result_n1839, hxc_l_tmp_load_result_n1840, hxc_l_tmp_load_result_n1841, (double)hxc_l_completedTicks * 0.05);
                  bool hxc_l_tmp_load_result_n1843 = hxc_l_portalVisible;
                  int32_t hxc_l_tmp_conditional_result_n531 = 0;
                  if (!hxc_l_tmp_load_result_n1843)
                  {
                    struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1844 = hxc_l_levelView_hb85a9a956786;
                    hxc_l_gc_roots[592] = (const void *)hxc_l_tmp_load_result_n1844;
                    if (hxc_l_tmp_load_result_n1844 == NULL)
                    {
                      abort();
                    }
                    struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_class_field_load_result_n1845 = (*hxc_l_tmp_load_result_n1844).hxc_levelPresentation;
                    hxc_l_gc_roots[593] = (const void *)hxc_l_tmp_class_field_load_result_n1845;
                    if (hxc_l_tmp_class_field_load_result_n1845 == NULL)
                    {
                      abort();
                    }
                    int32_t hxc_l_tmp_class_field_load_result_n1846 = (*hxc_l_tmp_class_field_load_result_n1845).hxc_environmentHorizonMaskValue;
                    hxc_l_tmp_conditional_result_n531 = hxc_l_tmp_class_field_load_result_n1846;
                  }
                  else
                  {
                    struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1847 = hxc_l_levelView_hb85a9a956786;
                    hxc_l_gc_roots[594] = (const void *)hxc_l_tmp_load_result_n1847;
                    if (hxc_l_tmp_load_result_n1847 == NULL)
                    {
                      abort();
                    }
                    struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_class_field_load_result_n1848 = (*hxc_l_tmp_load_result_n1847).hxc_levelPresentation;
                    hxc_l_gc_roots[595] = (const void *)hxc_l_tmp_class_field_load_result_n1848;
                    if (hxc_l_tmp_class_field_load_result_n1848 == NULL)
                    {
                      abort();
                    }
                    int32_t hxc_l_tmp_class_field_load_result_n1849 = (*hxc_l_tmp_class_field_load_result_n1848).hxc_environmentHorizonMaskValue;
                    struct hxc_caxecraft_app_PreparedCampaignPortal *hxc_l_tmp_load_result_n1850 = hxc_l_portal_hcef399961d30;
                    hxc_l_gc_roots[596] = (const void *)hxc_l_tmp_load_result_n1850;
                    if (hxc_l_tmp_load_result_n1850 == NULL)
                    {
                      abort();
                    }
                    int32_t hxc_l_tmp_class_field_load_result_n1851 = (*hxc_l_tmp_load_result_n1850).hxc_sourceEdgeMaskValue;
                    hxc_l_tmp_conditional_result_n531 = hxc_i32_bit_and(hxc_l_tmp_class_field_load_result_n1849, hxc_i32_bit_not(hxc_l_tmp_class_field_load_result_n1851));
                  }
                  int32_t hxc_l_horizonMask = hxc_l_tmp_conditional_result_n531;
                  if (hxc_l_self == NULL)
                  {
                    abort();
                  }
                  struct hxc_caxecraft_app_DistantHorizonRenderer *hxc_l_tmp_owned_class_field_address_n1853 = &(*hxc_l_self).hxc_distantHorizonRenderer;
                  struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1854 = hxc_l_levelView_hb85a9a956786;
                  hxc_l_gc_roots[597] = (const void *)hxc_l_tmp_load_result_n1854;
                  if (hxc_l_tmp_load_result_n1854 == NULL)
                  {
                    abort();
                  }
                  struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_class_field_load_result_n1855 = (*hxc_l_tmp_load_result_n1854).hxc_levelPresentation;
                  hxc_l_gc_roots[598] = (const void *)hxc_l_tmp_class_field_load_result_n1855;
                  if (hxc_l_tmp_class_field_load_result_n1855 == NULL)
                  {
                    abort();
                  }
                  int32_t hxc_l_tmp_class_field_load_result_n1856 = (*hxc_l_tmp_class_field_load_result_n1855).hxc_worldWidth;
                  (*hxc_l_tmp_owned_class_field_address_n1853).hxc_descriptorValue = hxc_i32_bit_or(hxc_l_tmp_class_field_load_result_n1856, hxc_i32_shift_left_masked(hxc_l_horizonMask, 8));
                  struct hxc_caxecraft_app_DistantHorizonRenderer *hxc_l_tmp_owned_class_field_address_n1858 = &(*hxc_l_self).hxc_distantHorizonRenderer;
                  if (hxc_l_tmp_owned_class_field_address_n1858 == NULL)
                  {
                    abort();
                  }
                  struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1859 = hxc_l_session;
                  hxc_l_gc_roots[599] = (const void *)hxc_l_tmp_load_result_n1859;
                  if (hxc_l_tmp_load_result_n1859 == NULL)
                  {
                    abort();
                  }
                  size_t hxc_l_tmp_length_n1861;
                  const uint8_t *hxc_l_tmp_instance_call_result_n1860 = hxc_caxecraft_domain_GameSession_worldView(hxc_l_tmp_load_result_n1859, &hxc_l_tmp_length_n1861);
                  struct Texture hxc_l_tmp_load_result_n1862 = hxc_l_terrainTexture;
                  bool hxc_l_tmp_load_result_n1863 = hxc_l_terrainTextureReady;
                  struct Texture hxc_l_tmp_load_result_n1864 = hxc_l_adventureTerrainTexture;
                  bool hxc_l_tmp_load_result_n1865 = hxc_l_adventureTerrainTextureReady;
                  struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1866 = hxc_l_levelView_hb85a9a956786;
                  hxc_l_gc_roots[600] = (const void *)hxc_l_tmp_load_result_n1866;
                  if (hxc_l_tmp_load_result_n1866 == NULL)
                  {
                    abort();
                  }
                  int32_t hxc_l_tmp_class_field_load_result_n1867 = (*hxc_l_tmp_load_result_n1866).hxc_waterCell;
                  hxc_caxecraft_app_DistantHorizonRenderer_draw(hxc_l_tmp_owned_class_field_address_n1858, hxc_l_tmp_instance_call_result_n1860, hxc_l_tmp_length_n1861, hxc_l_tmp_load_result_n1862, hxc_l_tmp_load_result_n1863, hxc_l_tmp_load_result_n1864, hxc_l_tmp_load_result_n1865, hxc_l_tmp_class_field_load_result_n1867);
                }
                if (hxc_l_portalVisible)
                {
                  struct hxc_caxecraft_app_PreparedCampaignPortal *hxc_l_tmp_load_result_n1869 = hxc_l_portal_hcef399961d30;
                  hxc_l_gc_roots[601] = (const void *)hxc_l_tmp_load_result_n1869;
                  if (hxc_l_tmp_load_result_n1869 == NULL)
                  {
                    abort();
                  }
                  struct hxc_caxecraft_app_StagedPlayableLevel *hxc_l_tmp_class_field_load_result_n1870 = (*hxc_l_tmp_load_result_n1869).hxc_stagedValue;
                  hxc_l_gc_roots[602] = (const void *)hxc_l_tmp_class_field_load_result_n1870;
                  struct hxc_caxecraft_app_StagedPlayableLevel *hxc_l_preview_haae48dd5bc31 = hxc_l_tmp_class_field_load_result_n1870;
                  if (hxc_l_self == NULL)
                  {
                    abort();
                  }
                  struct hxc_caxecraft_app_TerrainRenderer *hxc_l_tmp_owned_class_field_address_n1871 = &(*hxc_l_self).hxc_portalTerrainRenderer;
                  if (hxc_l_tmp_owned_class_field_address_n1871 == NULL)
                  {
                    abort();
                  }
                  struct hxc_caxecraft_app_StagedPlayableLevel *hxc_l_tmp_load_result_n1872 = hxc_l_preview_haae48dd5bc31;
                  hxc_l_gc_roots[603] = (const void *)hxc_l_tmp_load_result_n1872;
                  if (hxc_l_tmp_load_result_n1872 == NULL)
                  {
                    abort();
                  }
                  struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_class_field_load_result_n1873 = (*hxc_l_tmp_load_result_n1872).hxc_stagedView;
                  hxc_l_gc_roots[604] = (const void *)hxc_l_tmp_class_field_load_result_n1873;
                  if (hxc_l_tmp_class_field_load_result_n1873 == NULL)
                  {
                    abort();
                  }
                  struct hxc_caxecraft_content_LoadedContentGeneration *hxc_l_tmp_class_field_load_result_n1874 = (*hxc_l_tmp_class_field_load_result_n1873).hxc_loadedGeneration;
                  hxc_l_gc_roots[605] = (const void *)hxc_l_tmp_class_field_load_result_n1874;
                  if (hxc_l_tmp_class_field_load_result_n1874 == NULL)
                  {
                    abort();
                  }
                  struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_class_field_load_result_n1875 = (*hxc_l_tmp_class_field_load_result_n1874).hxc_ownedSession;
                  hxc_l_gc_roots[606] = (const void *)hxc_l_tmp_class_field_load_result_n1875;
                  if (hxc_l_tmp_class_field_load_result_n1875 == NULL)
                  {
                    abort();
                  }
                  size_t hxc_l_tmp_length_n1877;
                  const uint8_t *hxc_l_tmp_instance_call_result_n1876 = hxc_caxecraft_domain_GameSession_worldView(hxc_l_tmp_class_field_load_result_n1875, &hxc_l_tmp_length_n1877);
                  struct Texture hxc_l_tmp_load_result_n1878 = hxc_l_terrainTexture;
                  bool hxc_l_tmp_load_result_n1879 = hxc_l_terrainTextureReady;
                  struct Texture hxc_l_tmp_load_result_n1880 = hxc_l_adventureTerrainTexture;
                  bool hxc_l_tmp_load_result_n1881 = hxc_l_adventureTerrainTextureReady;
                  double hxc_l_tmp_record_field_load_result_n1882 = hxc_l_renderPosition.hxc_x;
                  double hxc_l_tmp_record_field_load_result_n1883 = hxc_l_renderPosition.hxc_z;
                  struct hxc_caxecraft_app_PreparedCampaignPortal *hxc_l_tmp_load_result_n1884 = hxc_l_portal_hcef399961d30;
                  hxc_l_gc_roots[607] = (const void *)hxc_l_tmp_load_result_n1884;
                  if (hxc_l_tmp_load_result_n1884 == NULL)
                  {
                    abort();
                  }
                  double hxc_l_tmp_class_field_load_result_n1885 = (*hxc_l_tmp_load_result_n1884).hxc_offsetXValue;
                  struct hxc_caxecraft_app_PreparedCampaignPortal *hxc_l_tmp_load_result_n1886 = hxc_l_portal_hcef399961d30;
                  hxc_l_gc_roots[608] = (const void *)hxc_l_tmp_load_result_n1886;
                  if (hxc_l_tmp_load_result_n1886 == NULL)
                  {
                    abort();
                  }
                  double hxc_l_tmp_class_field_load_result_n1887 = (*hxc_l_tmp_load_result_n1886).hxc_offsetYValue;
                  struct hxc_caxecraft_app_PreparedCampaignPortal *hxc_l_tmp_load_result_n1888 = hxc_l_portal_hcef399961d30;
                  hxc_l_gc_roots[609] = (const void *)hxc_l_tmp_load_result_n1888;
                  if (hxc_l_tmp_load_result_n1888 == NULL)
                  {
                    abort();
                  }
                  double hxc_l_tmp_class_field_load_result_n1889 = (*hxc_l_tmp_load_result_n1888).hxc_offsetZValue;
                  hxc_caxecraft_app_TerrainRenderer_drawTranslated(hxc_l_tmp_owned_class_field_address_n1871, hxc_l_tmp_instance_call_result_n1876, hxc_l_tmp_length_n1877, hxc_l_tmp_load_result_n1878, hxc_l_tmp_load_result_n1879, hxc_l_tmp_load_result_n1880, hxc_l_tmp_load_result_n1881, hxc_l_tmp_record_field_load_result_n1882, hxc_l_tmp_record_field_load_result_n1883, hxc_l_tmp_class_field_load_result_n1885, hxc_l_tmp_class_field_load_result_n1887, hxc_l_tmp_class_field_load_result_n1889);
                }
                if (hxc_l_self == NULL)
                {
                  abort();
                }
                struct hxc_caxecraft_app_TerrainRenderer *hxc_l_tmp_owned_class_field_address_n1890 = &(*hxc_l_self).hxc_terrainRenderer;
                if (hxc_l_tmp_owned_class_field_address_n1890 == NULL)
                {
                  abort();
                }
                struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1891 = hxc_l_session;
                hxc_l_gc_roots[610] = (const void *)hxc_l_tmp_load_result_n1891;
                if (hxc_l_tmp_load_result_n1891 == NULL)
                {
                  abort();
                }
                size_t hxc_l_tmp_length_n1893;
                const uint8_t *hxc_l_tmp_instance_call_result_n1892 = hxc_caxecraft_domain_GameSession_worldView(hxc_l_tmp_load_result_n1891, &hxc_l_tmp_length_n1893);
                struct Texture hxc_l_tmp_load_result_n1894 = hxc_l_terrainTexture;
                bool hxc_l_tmp_load_result_n1895 = hxc_l_terrainTextureReady;
                struct Texture hxc_l_tmp_load_result_n1896 = hxc_l_adventureTerrainTexture;
                bool hxc_l_tmp_load_result_n1897 = hxc_l_adventureTerrainTextureReady;
                double hxc_l_tmp_record_field_load_result_n1898 = hxc_l_renderPosition.hxc_x;
                struct hxc_caxecraft_app_TerrainRenderCounters hxc_l_tmp_instance_call_result_n1900 = hxc_caxecraft_app_TerrainRenderer_draw(hxc_l_tmp_owned_class_field_address_n1890, hxc_l_tmp_instance_call_result_n1892, hxc_l_tmp_length_n1893, hxc_l_tmp_load_result_n1894, hxc_l_tmp_load_result_n1895, hxc_l_tmp_load_result_n1896, hxc_l_tmp_load_result_n1897, hxc_l_tmp_record_field_load_result_n1898, hxc_l_renderPosition.hxc_z);
                struct hxc_caxecraft_app_TerrainRenderCounters hxc_l_renderCounters = hxc_l_tmp_instance_call_result_n1900;
                if (hxc_l_portalVisible)
                {
                  struct hxc_caxecraft_app_PreparedCampaignPortal *hxc_l_tmp_load_result_n1902 = hxc_l_portal_hcef399961d30;
                  hxc_l_gc_roots[611] = (const void *)hxc_l_tmp_load_result_n1902;
                  if (hxc_l_tmp_load_result_n1902 == NULL)
                  {
                    abort();
                  }
                  struct hxc_caxecraft_app_StagedPlayableLevel *hxc_l_tmp_class_field_load_result_n1903 = (*hxc_l_tmp_load_result_n1902).hxc_stagedValue;
                  hxc_l_gc_roots[612] = (const void *)hxc_l_tmp_class_field_load_result_n1903;
                  struct hxc_caxecraft_app_StagedPlayableLevel *hxc_l_preview_ha2984fec2db9 = hxc_l_tmp_class_field_load_result_n1903;
                  struct hxc_caxecraft_app_WaterRenderer *hxc_l_tmp_owned_class_field_address_n1904 = &(*hxc_l_self).hxc_portalWaterRenderer;
                  if (hxc_l_tmp_owned_class_field_address_n1904 == NULL)
                  {
                    abort();
                  }
                  struct hxc_caxecraft_app_StagedPlayableLevel *hxc_l_tmp_load_result_n1905 = hxc_l_preview_ha2984fec2db9;
                  hxc_l_gc_roots[613] = (const void *)hxc_l_tmp_load_result_n1905;
                  if (hxc_l_tmp_load_result_n1905 == NULL)
                  {
                    abort();
                  }
                  struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_class_field_load_result_n1906 = (*hxc_l_tmp_load_result_n1905).hxc_stagedView;
                  hxc_l_gc_roots[614] = (const void *)hxc_l_tmp_class_field_load_result_n1906;
                  if (hxc_l_tmp_class_field_load_result_n1906 == NULL)
                  {
                    abort();
                  }
                  struct hxc_caxecraft_content_LoadedContentGeneration *hxc_l_tmp_class_field_load_result_n1907 = (*hxc_l_tmp_class_field_load_result_n1906).hxc_loadedGeneration;
                  hxc_l_gc_roots[615] = (const void *)hxc_l_tmp_class_field_load_result_n1907;
                  if (hxc_l_tmp_class_field_load_result_n1907 == NULL)
                  {
                    abort();
                  }
                  struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_class_field_load_result_n1908 = (*hxc_l_tmp_class_field_load_result_n1907).hxc_ownedSession;
                  hxc_l_gc_roots[616] = (const void *)hxc_l_tmp_class_field_load_result_n1908;
                  if (hxc_l_tmp_class_field_load_result_n1908 == NULL)
                  {
                    abort();
                  }
                  size_t hxc_l_tmp_length_n1910;
                  const uint8_t *hxc_l_tmp_instance_call_result_n1909 = hxc_caxecraft_domain_GameSession_worldView(hxc_l_tmp_class_field_load_result_n1908, &hxc_l_tmp_length_n1910);
                  struct Texture hxc_l_tmp_load_result_n1911 = hxc_l_terrainTexture;
                  bool hxc_l_tmp_load_result_n1912 = hxc_l_terrainTextureReady;
                  struct hxc_caxecraft_app_StagedPlayableLevel *hxc_l_tmp_load_result_n1913 = hxc_l_preview_ha2984fec2db9;
                  hxc_l_gc_roots[617] = (const void *)hxc_l_tmp_load_result_n1913;
                  if (hxc_l_tmp_load_result_n1913 == NULL)
                  {
                    abort();
                  }
                  struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_class_field_load_result_n1914 = (*hxc_l_tmp_load_result_n1913).hxc_stagedView;
                  hxc_l_gc_roots[618] = (const void *)hxc_l_tmp_class_field_load_result_n1914;
                  if (hxc_l_tmp_class_field_load_result_n1914 == NULL)
                  {
                    abort();
                  }
                  int32_t hxc_l_tmp_class_field_load_result_n1915 = (*hxc_l_tmp_class_field_load_result_n1914).hxc_waterCell;
                  double hxc_l_tmp_load_result_n1916 = hxc_l_eyeX;
                  double hxc_l_tmp_load_result_n1917 = hxc_l_eyeY;
                  double hxc_l_tmp_load_result_n1918 = hxc_l_eyeZ;
                  struct hxc_caxecraft_app_PreparedCampaignPortal *hxc_l_tmp_load_result_n1919 = hxc_l_portal_hcef399961d30;
                  hxc_l_gc_roots[619] = (const void *)hxc_l_tmp_load_result_n1919;
                  if (hxc_l_tmp_load_result_n1919 == NULL)
                  {
                    abort();
                  }
                  double hxc_l_tmp_class_field_load_result_n1920 = (*hxc_l_tmp_load_result_n1919).hxc_offsetXValue;
                  struct hxc_caxecraft_app_PreparedCampaignPortal *hxc_l_tmp_load_result_n1921 = hxc_l_portal_hcef399961d30;
                  hxc_l_gc_roots[620] = (const void *)hxc_l_tmp_load_result_n1921;
                  if (hxc_l_tmp_load_result_n1921 == NULL)
                  {
                    abort();
                  }
                  double hxc_l_tmp_class_field_load_result_n1922 = (*hxc_l_tmp_load_result_n1921).hxc_offsetYValue;
                  struct hxc_caxecraft_app_PreparedCampaignPortal *hxc_l_tmp_load_result_n1923 = hxc_l_portal_hcef399961d30;
                  hxc_l_gc_roots[621] = (const void *)hxc_l_tmp_load_result_n1923;
                  if (hxc_l_tmp_load_result_n1923 == NULL)
                  {
                    abort();
                  }
                  double hxc_l_tmp_class_field_load_result_n1924 = (*hxc_l_tmp_load_result_n1923).hxc_offsetZValue;
                  hxc_caxecraft_app_WaterRenderer_drawTranslated(hxc_l_tmp_owned_class_field_address_n1904, hxc_l_tmp_instance_call_result_n1909, hxc_l_tmp_length_n1910, hxc_l_tmp_load_result_n1911, hxc_l_tmp_load_result_n1912, hxc_l_tmp_class_field_load_result_n1915, hxc_l_tmp_load_result_n1916, hxc_l_tmp_load_result_n1917, hxc_l_tmp_load_result_n1918, hxc_l_tmp_class_field_load_result_n1920, hxc_l_tmp_class_field_load_result_n1922, hxc_l_tmp_class_field_load_result_n1924);
                }
                struct hxc_caxecraft_app_WaterRenderer *hxc_l_tmp_owned_class_field_address_n1925 = &(*hxc_l_self).hxc_waterRenderer;
                if (hxc_l_tmp_owned_class_field_address_n1925 == NULL)
                {
                  abort();
                }
                struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1926 = hxc_l_session;
                hxc_l_gc_roots[622] = (const void *)hxc_l_tmp_load_result_n1926;
                if (hxc_l_tmp_load_result_n1926 == NULL)
                {
                  abort();
                }
                size_t hxc_l_tmp_length_n1928;
                const uint8_t *hxc_l_tmp_instance_call_result_n1927 = hxc_caxecraft_domain_GameSession_worldView(hxc_l_tmp_load_result_n1926, &hxc_l_tmp_length_n1928);
                struct Texture hxc_l_tmp_load_result_n1929 = hxc_l_terrainTexture;
                bool hxc_l_tmp_load_result_n1930 = hxc_l_terrainTextureReady;
                struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1931 = hxc_l_levelView_hb85a9a956786;
                hxc_l_gc_roots[623] = (const void *)hxc_l_tmp_load_result_n1931;
                if (hxc_l_tmp_load_result_n1931 == NULL)
                {
                  abort();
                }
                int32_t hxc_l_tmp_class_field_load_result_n1932 = (*hxc_l_tmp_load_result_n1931).hxc_waterCell;
                double hxc_l_tmp_load_result_n1933 = hxc_l_eyeX;
                double hxc_l_tmp_load_result_n1934 = hxc_l_eyeY;
                struct hxc_caxecraft_app_WaterRenderCounters hxc_l_tmp_instance_call_result_n1936 = hxc_caxecraft_app_WaterRenderer_draw(hxc_l_tmp_owned_class_field_address_n1925, hxc_l_tmp_instance_call_result_n1927, hxc_l_tmp_length_n1928, hxc_l_tmp_load_result_n1929, hxc_l_tmp_load_result_n1930, hxc_l_tmp_class_field_load_result_n1932, hxc_l_tmp_load_result_n1933, hxc_l_tmp_load_result_n1934, hxc_l_eyeZ);
                struct hxc_caxecraft_app_WaterRenderCounters hxc_l_waterCounters = hxc_l_tmp_instance_call_result_n1936;
                int32_t hxc_l_tmp_record_field_load_result_n1937 = hxc_l_renderCounters.hxc_visible;
                int32_t hxc_l_totalVisible = hxc_i32_add_wrapping(hxc_l_tmp_record_field_load_result_n1937, hxc_l_waterCounters.hxc_visible);
                int32_t hxc_l_tmp_record_field_load_result_n1939 = hxc_l_renderCounters.hxc_drawCalls;
                int32_t hxc_l_totalDrawCalls = hxc_i32_add_wrapping(hxc_l_tmp_record_field_load_result_n1939, hxc_l_waterCounters.hxc_drawCalls);
                struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_tmp_load_result_n1941 = hxc_l_contentRegistry;
                hxc_l_gc_roots[624] = (const void *)hxc_l_tmp_load_result_n1941;
                struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1942 = hxc_l_session;
                hxc_l_gc_roots[625] = (const void *)hxc_l_tmp_load_result_n1942;
                struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1943 = hxc_l_levelView_hb85a9a956786;
                hxc_l_gc_roots[626] = (const void *)hxc_l_tmp_load_result_n1943;
                struct Texture hxc_l_tmp_load_result_n1944 = hxc_l_entityTexture;
                bool hxc_l_tmp_load_result_n1945 = hxc_l_entityTextureReady;
                struct Texture hxc_l_tmp_load_result_n1946 = hxc_l_itemTexture;
                bool hxc_l_tmp_load_result_n1947 = hxc_l_itemTextureReady;
                struct Texture hxc_l_tmp_load_result_n1948 = hxc_l_adventureItemTexture;
                bool hxc_l_tmp_load_result_n1949 = hxc_l_adventureItemTextureReady;
                struct Texture hxc_l_tmp_load_result_n1950 = hxc_l_terrainTexture;
                bool hxc_l_tmp_load_result_n1951 = hxc_l_terrainTextureReady;
                struct hxc_caxecraft_app_RuntimeTextureAtlasCatalog *hxc_l_tmp_load_result_n1952 = hxc_l_runtimeTextures;
                hxc_l_gc_roots[627] = (const void *)hxc_l_tmp_load_result_n1952;
                struct hxc_caxecraft_app_RuntimeVoxelModelCatalog *hxc_l_tmp_load_result_n1953 = hxc_l_runtimeModels;
                hxc_l_gc_roots[628] = (const void *)hxc_l_tmp_load_result_n1953;
                struct hxc_caxecraft_app_VoxelFrameAnimationPlayer *hxc_l_tmp_load_result_n1954 = hxc_l_modelAnimations;
                struct hxc_caxecraft_app_ActivePlayableLevel *hxc_l_tmp_load_result_n1955 = hxc_l_frameLevelOwner;
                hxc_l_gc_roots[629] = (const void *)hxc_l_tmp_load_result_n1955;
                if (hxc_l_tmp_load_result_n1955 == NULL)
                {
                  abort();
                }
                struct hxc_caxecraft_content_ActiveContent *hxc_l_tmp_class_field_load_result_n1956 = (*hxc_l_tmp_load_result_n1955).hxc_content;
                hxc_l_gc_roots[630] = (const void *)hxc_l_tmp_class_field_load_result_n1956;
                if (hxc_l_tmp_class_field_load_result_n1956 == NULL)
                {
                  abort();
                }
                struct hxc_caxecraft_content_LoadedContentGeneration *hxc_l_tmp_class_field_load_result_n1957 = (*hxc_l_tmp_class_field_load_result_n1956).hxc_active;
                hxc_l_gc_roots[631] = (const void *)hxc_l_tmp_class_field_load_result_n1957;
                if (hxc_l_tmp_class_field_load_result_n1957 == NULL)
                {
                  abort();
                }
                int32_t hxc_l_tmp_class_field_load_result_n1958 = (*hxc_l_tmp_class_field_load_result_n1957).hxc_generationIdValue;
                hxc_caxecraft_app_StatefulObjectRenderer_drawStatefulObjects(hxc_l_tmp_load_result_n1941, hxc_l_tmp_load_result_n1942, hxc_l_tmp_load_result_n1943, hxc_l_tmp_load_result_n1944, hxc_l_tmp_load_result_n1945, hxc_l_tmp_load_result_n1946, hxc_l_tmp_load_result_n1947, hxc_l_tmp_load_result_n1948, hxc_l_tmp_load_result_n1949, hxc_l_tmp_load_result_n1950, hxc_l_tmp_load_result_n1951, hxc_l_tmp_load_result_n1952, hxc_l_tmp_load_result_n1953, hxc_l_tmp_load_result_n1954, hxc_l_tmp_class_field_load_result_n1958, hxc_l_completedTicks);
                if (hxc_l_cameraView.hxc_avatarVisible)
                {
                  struct Camera3D hxc_l_tmp_load_result_n1961 = hxc_l_camera;
                  struct hxc_caxecraft_app_RuntimeTextureAtlasCatalog *hxc_l_tmp_load_result_n1962 = hxc_l_runtimeTextures;
                  hxc_l_gc_roots[632] = (const void *)hxc_l_tmp_load_result_n1962;
                  double hxc_l_tmp_record_field_load_result_n1963 = hxc_l_renderPosition.hxc_x;
                  double hxc_l_tmp_record_field_load_result_n1964 = hxc_l_renderPosition.hxc_y;
                  hxc_caxecraft_app_CaxecraftApp_drawPlayerAvatar(hxc_l_tmp_load_result_n1961, hxc_l_tmp_load_result_n1962, hxc_l_tmp_record_field_load_result_n1963, hxc_l_tmp_record_field_load_result_n1964, hxc_l_renderPosition.hxc_z);
                }
                struct Camera3D hxc_l_tmp_load_result_n1966 = hxc_l_camera;
                struct Texture hxc_l_tmp_load_result_n1967 = hxc_l_entityTexture;
                bool hxc_l_tmp_load_result_n1968 = hxc_l_entityTextureReady;
                struct hxc_caxecraft_app_RuntimeTextureAtlasCatalog *hxc_l_tmp_load_result_n1969 = hxc_l_runtimeTextures;
                hxc_l_gc_roots[633] = (const void *)hxc_l_tmp_load_result_n1969;
                struct hxc_array_ref *hxc_l_tmp_load_result_n1970 = hxc_l_dialogueActors;
                struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1971 = hxc_l_levelView_hb85a9a956786;
                hxc_l_gc_roots[634] = (const void *)hxc_l_tmp_load_result_n1971;
                struct hxc_caxecraft_domain_Character hxc_l_tmp_load_result_n1972 = hxc_l_enemyActor;
                struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1973 = hxc_l_levelView_hb85a9a956786;
                hxc_l_gc_roots[635] = (const void *)hxc_l_tmp_load_result_n1973;
                if (hxc_l_tmp_load_result_n1973 == NULL)
                {
                  abort();
                }
                hxc_string hxc_l_tmp_class_field_load_result_n1974 = (*hxc_l_tmp_load_result_n1973).hxc_enemyAsset;
                struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1975 = hxc_l_levelView_hb85a9a956786;
                hxc_l_gc_roots[636] = (const void *)hxc_l_tmp_load_result_n1975;
                if (hxc_l_tmp_load_result_n1975 == NULL)
                {
                  abort();
                }
                int32_t hxc_l_tmp_class_field_load_result_n1976 = (*hxc_l_tmp_load_result_n1975).hxc_enemyCell;
                enum hxc_caxecraft_domain_ActorControllerPhase hxc_l_tmp_record_field_load_result_n1977 = hxc_l_enemyPhase_ha225e19c84ac.hxc_phase;
                hxc_caxecraft_app_CaxecraftApp_drawActors(hxc_l_tmp_load_result_n1966, hxc_l_tmp_load_result_n1967, hxc_l_tmp_load_result_n1968, hxc_l_tmp_load_result_n1969, hxc_l_tmp_load_result_n1970, hxc_l_tmp_load_result_n1971, hxc_l_tmp_load_result_n1972, hxc_l_tmp_class_field_load_result_n1974, hxc_l_tmp_class_field_load_result_n1976, hxc_l_tmp_record_field_load_result_n1977, hxc_l_berryDrop);
                struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_tmp_load_result_n1979 = hxc_l_contentRegistry;
                hxc_l_gc_roots[637] = (const void *)hxc_l_tmp_load_result_n1979;
                struct Camera3D hxc_l_tmp_load_result_n1980 = hxc_l_camera;
                struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1981 = hxc_l_session;
                hxc_l_gc_roots[638] = (const void *)hxc_l_tmp_load_result_n1981;
                if (hxc_l_tmp_load_result_n1981 == NULL)
                {
                  abort();
                }
                size_t hxc_l_tmp_length_n1983;
                const int32_t *hxc_l_tmp_instance_call_result_n1982 = hxc_caxecraft_domain_GameSession_authoredItemsView(hxc_l_tmp_load_result_n1981, &hxc_l_tmp_length_n1983);
                struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1984 = hxc_l_levelView_hb85a9a956786;
                hxc_l_gc_roots[639] = (const void *)hxc_l_tmp_load_result_n1984;
                struct Texture hxc_l_tmp_load_result_n1985 = hxc_l_itemTexture;
                bool hxc_l_tmp_load_result_n1986 = hxc_l_itemTextureReady;
                struct Texture hxc_l_tmp_load_result_n1987 = hxc_l_adventureItemTexture;
                hxc_caxecraft_app_AuthoredItemRenderer_drawWorldItems(hxc_l_tmp_load_result_n1979, hxc_l_tmp_load_result_n1980, hxc_l_tmp_instance_call_result_n1982, hxc_l_tmp_length_n1983, hxc_l_tmp_load_result_n1984, hxc_l_tmp_load_result_n1985, hxc_l_tmp_load_result_n1986, hxc_l_tmp_load_result_n1987, hxc_l_adventureItemTextureReady);
                if (hxc_l_hit.hxc_hit)
                {
                  double hxc_l_y_hf3b70647af3e = (double)hxc_l_hit.hxc_cellY + 0.5;
                  double hxc_l_z_hba0f077e1b8c = (double)hxc_l_hit.hxc_cellZ + 0.5;
                  int32_t hxc_l_tmp_record_field_load_result_n1992 = hxc_l_hit.hxc_cellX;
                  double hxc_l_tmp_load_result_n1993 = hxc_l_y_hf3b70647af3e;
                  DrawCubeWires((struct Vector3){ .x = (float)((double)hxc_l_tmp_record_field_load_result_n1992 + 0.5), .y = (float)hxc_l_tmp_load_result_n1993, .z = (float)hxc_l_z_hba0f077e1b8c }, (float)1.04, (float)1.04, (float)1.04, (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 });
                }
                EndMode3D();
                if (hxc_l_cameraWaterBlend > 0.0)
                {
                  int32_t hxc_l_overlayAlpha = hxc_f64_to_i32_saturating(105.0 * hxc_l_cameraWaterBlend);
                  int32_t hxc_l_tmp_native_call_result_n1999 = GetScreenWidth();
                  int32_t hxc_l_width_hd4fc19308a0b = (int32_t)hxc_l_tmp_native_call_result_n1999;
                  int32_t hxc_l_tmp_native_call_result_n2000 = GetScreenHeight();
                  int32_t hxc_l_height_hc1ebe46a832f = (int32_t)hxc_l_tmp_native_call_result_n2000;
                  uint8_t hxc_l_red_h2dfc755189ce = (uint8_t)31;
                  uint8_t hxc_l_green_h6b7c35514065 = (uint8_t)115;
                  uint8_t hxc_l_blue_h3cda2c6dc983 = (uint8_t)154;
                  int32_t hxc_l_tmp_load_result_n2001 = hxc_l_overlayAlpha;
                  int32_t hxc_l_tmp_conditional_result_n547 = 0;
                  if (hxc_l_tmp_load_result_n2001 < 0)
                  {
                    hxc_l_tmp_conditional_result_n547 = 0;
                  }
                  else
                  {
                    int32_t hxc_l_tmp_load_result_n2002 = hxc_l_overlayAlpha;
                    int32_t hxc_l_tmp_conditional_result_n548 = 0;
                    if (hxc_l_tmp_load_result_n2002 > 255)
                    {
                      hxc_l_tmp_conditional_result_n548 = 255;
                    }
                    else
                    {
                      hxc_l_tmp_conditional_result_n548 = hxc_l_overlayAlpha;
                    }
                    hxc_l_tmp_conditional_result_n547 = hxc_l_tmp_conditional_result_n548;
                  }
                  uint8_t hxc_l_alpha_h6b8321f52f6b = (uint8_t)hxc_l_tmp_conditional_result_n547;
                  uint8_t hxc_l_tmp_load_result_n2006 = hxc_l_red_h2dfc755189ce;
                  uint8_t hxc_l_tmp_load_result_n2007 = hxc_l_green_h6b7c35514065;
                  uint8_t hxc_l_tmp_load_result_n2008 = hxc_l_blue_h3cda2c6dc983;
                  struct Color hxc_l_this1_ha9de08294e32 = (struct Color){ .r = hxc_l_tmp_load_result_n2006, .g = hxc_l_tmp_load_result_n2007, .b = hxc_l_tmp_load_result_n2008, .a = hxc_l_alpha_h6b8321f52f6b };
                  int32_t hxc_l_tmp_load_result_n2011 = hxc_l_width_hd4fc19308a0b;
                  int32_t hxc_l_tmp_load_result_n2012 = hxc_l_height_hc1ebe46a832f;
                  DrawRectangle((int32_t)0, (int32_t)0, (int32_t)hxc_l_tmp_load_result_n2011, (int32_t)hxc_l_tmp_load_result_n2012, hxc_l_this1_ha9de08294e32);
                }
                struct hxc_optional_caxecraft_app_ConversationState hxc_l_tmp_load_result_n2014 = hxc_l_conversation;
                bool hxc_l_tmp_short_circuit_result_n551 = !hxc_l_tmp_load_result_n2014.hxc_has_value;
                if (!!hxc_l_tmp_load_result_n2014.hxc_has_value)
                {
                  bool hxc_l_tmp_string_null_equality_result_n552 = hxc_l_activeDialogue.data == NULL;
                  hxc_l_tmp_short_circuit_result_n551 = hxc_l_tmp_string_null_equality_result_n552;
                }
                bool hxc_l_tmp_short_circuit_load_result_n2017 = hxc_l_tmp_short_circuit_result_n551;
                hxc_string hxc_l_tmp_conditional_managed_result_n553 = { 0 };
                if (hxc_l_tmp_short_circuit_load_result_n2017)
                {
                  hxc_l_tmp_conditional_managed_result_n553 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
                  if (hxc_string_retain(hxc_l_tmp_conditional_managed_result_n553) != HXC_STATUS_OK)
                  {
                    abort();
                  }
                }
                else
                {
                  struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n2018 = hxc_l_levelView_hb85a9a956786;
                  hxc_l_gc_roots[640] = (const void *)hxc_l_tmp_load_result_n2018;
                  if (hxc_l_tmp_load_result_n2018 == NULL)
                  {
                    abort();
                  }
                  struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_class_field_load_result_n2019 = (*hxc_l_tmp_load_result_n2018).hxc_levelPresentation;
                  hxc_l_gc_roots[641] = (const void *)hxc_l_tmp_class_field_load_result_n2019;
                  if (hxc_l_tmp_class_field_load_result_n2019 == NULL)
                  {
                    abort();
                  }
                  hxc_string hxc_l_tmp_load_result_n2020 = hxc_l_activeDialogue;
                  struct hxc_optional_caxecraft_app_ConversationState hxc_l_tmp_load_result_n2021 = hxc_l_conversation;
                  if (!hxc_l_tmp_load_result_n2021.hxc_has_value)
                  {
                    abort();
                  }
                  hxc_string hxc_l_tmp_instance_call_result_n2024 = hxc_caxecraft_content_RuntimeLevelPresentation_dialogueSpeaker(hxc_l_tmp_class_field_load_result_n2019, hxc_l_tmp_load_result_n2020, hxc_l_tmp_load_result_n2021.hxc_value.hxc_lineIndex);
                  hxc_l_tmp_conditional_managed_result_n553 = hxc_l_tmp_instance_call_result_n2024;
                }
                hxc_string hxc_l_tmp_conditional_managed_move_result_n2025 = hxc_l_tmp_conditional_managed_result_n553;
                hxc_l_conversationSpeaker = hxc_l_tmp_conditional_managed_move_result_n2025;
                double hxc_l_tmp_record_field_load_result_n2026 = hxc_l_character.hxc_body.hxc_x;
                double hxc_l_tmp_record_field_load_result_n2027 = hxc_l_character.hxc_body.hxc_z;
                struct hxc_caxecraft_domain_VitalsState hxc_l_tmp_record_field_load_result_n2028 = hxc_l_character.hxc_vitals;
                int32_t hxc_l_tmp_load_result_n2029 = hxc_l_aquaticEquipmentCode;
                int32_t hxc_l_tmp_load_result_n2030 = hxc_l_aquaticEquipmentFrames;
                bool hxc_l_tmp_record_field_load_result_n2031 = hxc_l_character.hxc_aquatic.hxc_headSubmerged;
                int32_t hxc_l_tmp_record_field_load_result_n2032 = hxc_l_character.hxc_aquatic.hxc_breathTicks;
                struct hxc_caxecraft_app_HudCharacterView hxc_l_hudView_haf1542a5a189 = (struct hxc_caxecraft_app_HudCharacterView){ .hxc_aquaticEquipmentCode = hxc_l_tmp_load_result_n2029, .hxc_aquaticEquipmentVisible = hxc_l_tmp_load_result_n2030 > 0, .hxc_breathTicks = hxc_l_tmp_record_field_load_result_n2032, .hxc_headSubmerged = hxc_l_tmp_record_field_load_result_n2031, .hxc_maximumBreathTicks = hxc_l_character.hxc_aquaticProfile.hxc_maximumBreathTicks, .hxc_vitals = hxc_l_tmp_record_field_load_result_n2028, .hxc_x = hxc_l_tmp_record_field_load_result_n2026, .hxc_z = hxc_l_tmp_record_field_load_result_n2027 };
                bool hxc_l_hudView1 = hxc_l_placementBlockedFrames > 0;
                bool hxc_l_hudView2 = hxc_l_strikeHitFrames > 0;
                bool hxc_l_hudView3 = hxc_l_enemyDefeatedFrames > 0;
                bool hxc_l_hudView4 = hxc_l_enemyAttackFrames > 0;
                bool hxc_l_hudView5 = hxc_l_pickupFrames > 0;
                bool hxc_l_hudView6 = hxc_l_recoveryFeedbackFrames > 0;
                struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n2041 = hxc_l_levelView_hb85a9a956786;
                hxc_l_gc_roots[642] = (const void *)hxc_l_tmp_load_result_n2041;
                if (hxc_l_tmp_load_result_n2041 == NULL)
                {
                  abort();
                }
                struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_class_field_load_result_n2042 = (*hxc_l_tmp_load_result_n2041).hxc_levelPresentation;
                hxc_l_gc_roots[643] = (const void *)hxc_l_tmp_class_field_load_result_n2042;
                struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_levelView1_hf4d487f50672 = hxc_l_tmp_class_field_load_result_n2042;
                hxc_string hxc_l_hudView7_h10094007b5f6 = { 0 };
                switch (hxc_l_locale) {
                  case 0:
                    {
                      hxc_l_hudView7_h10094007b5f6 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
                      if (hxc_string_retain(hxc_l_hudView7_h10094007b5f6) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      break;
                    }
                  case 1:
                    {
                      hxc_l_hudView7_h10094007b5f6 = (hxc_string){ (const uint8_t *)"es-mx", 5, true, NULL };
                      if (hxc_string_retain(hxc_l_hudView7_h10094007b5f6) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      break;
                    }
                  default:
                    {
                      hxc_l_hudView7_h10094007b5f6 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
                      if (hxc_string_retain(hxc_l_hudView7_h10094007b5f6) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      break;
                    }
                }
                hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n2044 = hxc_l_hudView7_h10094007b5f6;
                hxc_l_tmp_managed_flow_owner_n563 = hxc_l_tmp_managed_flow_carrier_move_result_n2044;
                struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_load_result_n2045 = hxc_l_levelView1_hf4d487f50672;
                hxc_l_gc_roots[644] = (const void *)hxc_l_tmp_load_result_n2045;
                if (hxc_l_tmp_load_result_n2045 == NULL)
                {
                  abort();
                }
                hxc_string hxc_l_tmp_load_result_n2046 = hxc_l_conversationSpeaker;
                hxc_string hxc_l_tmp_instance_call_result_n2048 = hxc_caxecraft_content_RuntimeLevelPresentation_speakerName(hxc_l_tmp_load_result_n2045, hxc_l_tmp_load_result_n2046, hxc_l_tmp_managed_flow_owner_n563);
                hxc_l_hudView7_hef5dfe6036f4 = hxc_l_tmp_instance_call_result_n2048;
                struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n2049 = hxc_l_levelView_hb85a9a956786;
                hxc_l_gc_roots[645] = (const void *)hxc_l_tmp_load_result_n2049;
                if (hxc_l_tmp_load_result_n2049 == NULL)
                {
                  abort();
                }
                hxc_string hxc_l_tmp_instance_call_result_n2051 = hxc_caxecraft_app_PlayableLevelView_dialogueActorPresentationAsset(hxc_l_tmp_load_result_n2049, hxc_l_conversationSpeaker);
                hxc_l_hudView8 = hxc_l_tmp_instance_call_result_n2051;
                struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n2052 = hxc_l_levelView_hb85a9a956786;
                hxc_l_gc_roots[646] = (const void *)hxc_l_tmp_load_result_n2052;
                if (hxc_l_tmp_load_result_n2052 == NULL)
                {
                  abort();
                }
                int32_t hxc_l_tmp_instance_call_result_n2054 = hxc_caxecraft_app_PlayableLevelView_dialogueActorPresentationCell(hxc_l_tmp_load_result_n2052, hxc_l_conversationSpeaker);
                int32_t hxc_l_hudView9 = hxc_l_tmp_instance_call_result_n2054;
                enum hxc_caxecraft_domain_ActorControllerPhase hxc_l_enemyPhase_h073acd63e276 = hxc_l_enemyPhase_ha225e19c84ac.hxc_phase;
                hxc_string hxc_l_locale1_h63a39e50f2a7 = { 0 };
                switch (hxc_l_locale) {
                  case 0:
                    {
                      hxc_l_locale1_h63a39e50f2a7 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
                      if (hxc_string_retain(hxc_l_locale1_h63a39e50f2a7) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      break;
                    }
                  case 1:
                    {
                      hxc_l_locale1_h63a39e50f2a7 = (hxc_string){ (const uint8_t *)"es-mx", 5, true, NULL };
                      if (hxc_string_retain(hxc_l_locale1_h63a39e50f2a7) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      break;
                    }
                  default:
                    {
                      hxc_l_locale1_h63a39e50f2a7 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
                      if (hxc_string_retain(hxc_l_locale1_h63a39e50f2a7) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      break;
                    }
                }
                hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n2057 = hxc_l_locale1_h63a39e50f2a7;
                hxc_l_tmp_managed_flow_owner_n569 = hxc_l_tmp_managed_flow_carrier_move_result_n2057;
                struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n2058 = hxc_l_levelView_hb85a9a956786;
                hxc_l_gc_roots[647] = (const void *)hxc_l_tmp_load_result_n2058;
                if (hxc_l_tmp_load_result_n2058 == NULL)
                {
                  abort();
                }
                struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_class_field_load_result_n2059 = (*hxc_l_tmp_load_result_n2058).hxc_levelPresentation;
                hxc_l_gc_roots[648] = (const void *)hxc_l_tmp_class_field_load_result_n2059;
                struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_this_h2632f68164f7 = hxc_l_tmp_class_field_load_result_n2059;
                struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_load_result_n2060 = hxc_l_this_h2632f68164f7;
                hxc_l_gc_roots[649] = (const void *)hxc_l_tmp_load_result_n2060;
                if (hxc_l_tmp_load_result_n2060 == NULL)
                {
                  abort();
                }
                struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_load_result_n2061 = hxc_l_this_h2632f68164f7;
                hxc_l_gc_roots[650] = (const void *)hxc_l_tmp_load_result_n2061;
                if (hxc_l_tmp_load_result_n2061 == NULL)
                {
                  abort();
                }
                struct hxc_caxecraft_scenario_ScenarioText hxc_l_tmp_class_field_load_result_n2062 = (*hxc_l_tmp_load_result_n2061).hxc_title;
                hxc_string hxc_l_tmp_instance_call_result_n2064 = hxc_caxecraft_content_RuntimeLevelPresentation_resolve(hxc_l_tmp_load_result_n2060, hxc_l_tmp_class_field_load_result_n2062, hxc_l_tmp_managed_flow_owner_n569);
                hxc_l_hudView10 = hxc_l_tmp_instance_call_result_n2064;
                hxc_string hxc_l_locale1_h27a87cab9566 = { 0 };
                switch (hxc_l_locale) {
                  case 0:
                    {
                      hxc_l_locale1_h27a87cab9566 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
                      if (hxc_string_retain(hxc_l_locale1_h27a87cab9566) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      break;
                    }
                  case 1:
                    {
                      hxc_l_locale1_h27a87cab9566 = (hxc_string){ (const uint8_t *)"es-mx", 5, true, NULL };
                      if (hxc_string_retain(hxc_l_locale1_h27a87cab9566) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      break;
                    }
                  default:
                    {
                      hxc_l_locale1_h27a87cab9566 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
                      if (hxc_string_retain(hxc_l_locale1_h27a87cab9566) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      break;
                    }
                }
                hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n2066 = hxc_l_locale1_h27a87cab9566;
                hxc_l_tmp_managed_flow_owner_n573 = hxc_l_tmp_managed_flow_carrier_move_result_n2066;
                struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n2067 = hxc_l_levelView_hb85a9a956786;
                hxc_l_gc_roots[651] = (const void *)hxc_l_tmp_load_result_n2067;
                if (hxc_l_tmp_load_result_n2067 == NULL)
                {
                  abort();
                }
                struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_class_field_load_result_n2068 = (*hxc_l_tmp_load_result_n2067).hxc_levelPresentation;
                hxc_l_gc_roots[652] = (const void *)hxc_l_tmp_class_field_load_result_n2068;
                if (hxc_l_tmp_class_field_load_result_n2068 == NULL)
                {
                  abort();
                }
                hxc_string hxc_l_tmp_load_result_n2069 = hxc_l_currentObjectiveId;
                hxc_string hxc_l_tmp_instance_call_result_n2071 = hxc_caxecraft_content_RuntimeLevelPresentation_objectiveTitle(hxc_l_tmp_class_field_load_result_n2068, hxc_l_tmp_load_result_n2069, hxc_l_tmp_managed_flow_owner_n573);
                hxc_l_hudView11 = hxc_l_tmp_instance_call_result_n2071;
                hxc_string hxc_l_hudView12 = { 0 };
                bool hxc_l_tmp_string_null_equality_result_n576 = hxc_l_latestJournalId.data == NULL;
                if (hxc_l_tmp_string_null_equality_result_n576)
                {
                  hxc_l_hudView12 = (hxc_string){ (const uint8_t *)"", 0, true, NULL };
                  if (hxc_string_retain(hxc_l_hudView12) != HXC_STATUS_OK)
                  {
                    abort();
                  }
                }
                else
                {
                  struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n2074 = hxc_l_levelView_hb85a9a956786;
                  hxc_l_gc_roots[653] = (const void *)hxc_l_tmp_load_result_n2074;
                  if (hxc_l_tmp_load_result_n2074 == NULL)
                  {
                    abort();
                  }
                  struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_class_field_load_result_n2075 = (*hxc_l_tmp_load_result_n2074).hxc_levelPresentation;
                  hxc_l_gc_roots[654] = (const void *)hxc_l_tmp_class_field_load_result_n2075;
                  struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_levelView_h178e8d81caf2 = hxc_l_tmp_class_field_load_result_n2075;
                  hxc_string hxc_l_hudView_h9106443bfb51 = { 0 };
                  switch (hxc_l_locale) {
                    case 0:
                      {
                        hxc_l_hudView_h9106443bfb51 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
                        if (hxc_string_retain(hxc_l_hudView_h9106443bfb51) != HXC_STATUS_OK)
                        {
                          abort();
                        }
                        break;
                      }
                    case 1:
                      {
                        hxc_l_hudView_h9106443bfb51 = (hxc_string){ (const uint8_t *)"es-mx", 5, true, NULL };
                        if (hxc_string_retain(hxc_l_hudView_h9106443bfb51) != HXC_STATUS_OK)
                        {
                          abort();
                        }
                        break;
                      }
                    default:
                      {
                        hxc_l_hudView_h9106443bfb51 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
                        if (hxc_string_retain(hxc_l_hudView_h9106443bfb51) != HXC_STATUS_OK)
                        {
                          abort();
                        }
                        break;
                      }
                  }
                  hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n2077 = hxc_l_hudView_h9106443bfb51;
                  hxc_l_tmp_managed_flow_owner_n579 = hxc_l_tmp_managed_flow_carrier_move_result_n2077;
                  struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_load_result_n2078 = hxc_l_levelView_h178e8d81caf2;
                  hxc_l_gc_roots[655] = (const void *)hxc_l_tmp_load_result_n2078;
                  if (hxc_l_tmp_load_result_n2078 == NULL)
                  {
                    abort();
                  }
                  hxc_string hxc_l_tmp_load_result_n2079 = hxc_l_latestJournalId;
                  hxc_string hxc_l_tmp_instance_call_result_n2081 = hxc_caxecraft_content_RuntimeLevelPresentation_journalTitle(hxc_l_tmp_load_result_n2078, hxc_l_tmp_load_result_n2079, hxc_l_tmp_managed_flow_owner_n579);
                  hxc_l_hudView12 = hxc_l_tmp_instance_call_result_n2081;
                  if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n579) != HXC_STATUS_OK)
                  {
                    abort();
                  }
                }
                hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n2082 = hxc_l_hudView12;
                hxc_l_tmp_managed_flow_owner_n580 = hxc_l_tmp_managed_flow_carrier_move_result_n2082;
                hxc_string hxc_l_hudView13 = { 0 };
                bool hxc_l_tmp_string_null_equality_result_n582 = hxc_l_latestJournalId.data == NULL;
                if (hxc_l_tmp_string_null_equality_result_n582)
                {
                  hxc_l_hudView13 = (hxc_string){ (const uint8_t *)"", 0, true, NULL };
                  if (hxc_string_retain(hxc_l_hudView13) != HXC_STATUS_OK)
                  {
                    abort();
                  }
                }
                else
                {
                  struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n2085 = hxc_l_levelView_hb85a9a956786;
                  hxc_l_gc_roots[656] = (const void *)hxc_l_tmp_load_result_n2085;
                  if (hxc_l_tmp_load_result_n2085 == NULL)
                  {
                    abort();
                  }
                  struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_class_field_load_result_n2086 = (*hxc_l_tmp_load_result_n2085).hxc_levelPresentation;
                  hxc_l_gc_roots[657] = (const void *)hxc_l_tmp_class_field_load_result_n2086;
                  struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_levelView_hca4948a4d864 = hxc_l_tmp_class_field_load_result_n2086;
                  hxc_string hxc_l_hudView_h03eb68db50bb = { 0 };
                  switch (hxc_l_locale) {
                    case 0:
                      {
                        hxc_l_hudView_h03eb68db50bb = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
                        if (hxc_string_retain(hxc_l_hudView_h03eb68db50bb) != HXC_STATUS_OK)
                        {
                          abort();
                        }
                        break;
                      }
                    case 1:
                      {
                        hxc_l_hudView_h03eb68db50bb = (hxc_string){ (const uint8_t *)"es-mx", 5, true, NULL };
                        if (hxc_string_retain(hxc_l_hudView_h03eb68db50bb) != HXC_STATUS_OK)
                        {
                          abort();
                        }
                        break;
                      }
                    default:
                      {
                        hxc_l_hudView_h03eb68db50bb = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
                        if (hxc_string_retain(hxc_l_hudView_h03eb68db50bb) != HXC_STATUS_OK)
                        {
                          abort();
                        }
                        break;
                      }
                  }
                  hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n2088 = hxc_l_hudView_h03eb68db50bb;
                  hxc_l_tmp_managed_flow_owner_n585 = hxc_l_tmp_managed_flow_carrier_move_result_n2088;
                  struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_load_result_n2089 = hxc_l_levelView_hca4948a4d864;
                  hxc_l_gc_roots[658] = (const void *)hxc_l_tmp_load_result_n2089;
                  if (hxc_l_tmp_load_result_n2089 == NULL)
                  {
                    abort();
                  }
                  hxc_string hxc_l_tmp_load_result_n2090 = hxc_l_latestJournalId;
                  hxc_string hxc_l_tmp_instance_call_result_n2092 = hxc_caxecraft_content_RuntimeLevelPresentation_journalBody(hxc_l_tmp_load_result_n2089, hxc_l_tmp_load_result_n2090, hxc_l_tmp_managed_flow_owner_n585);
                  hxc_l_hudView13 = hxc_l_tmp_instance_call_result_n2092;
                  if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n585) != HXC_STATUS_OK)
                  {
                    abort();
                  }
                }
                hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n2093 = hxc_l_hudView13;
                hxc_l_tmp_managed_flow_owner_n586 = hxc_l_tmp_managed_flow_carrier_move_result_n2093;
                bool hxc_l_tmp_load_result_n2094 = hxc_l_debugHudVisible;
                int32_t hxc_l_tmp_load_result_n2095 = hxc_l_totalVisible;
                int32_t hxc_l_tmp_load_result_n2096 = hxc_l_totalDrawCalls;
                int32_t hxc_l_tmp_load_result_n2097 = hxc_l_frameCount;
                int32_t hxc_l_tmp_load_result_n2098 = hxc_l_completedTicks;
                struct hxc_caxecraft_app_HudCharacterView hxc_l_tmp_load_result_n2100 = hxc_l_hudView_haf1542a5a189;
                bool hxc_l_tmp_load_result_n2101 = hxc_l_hudView1;
                bool hxc_l_tmp_load_result_n2102 = hxc_l_hudView2;
                bool hxc_l_tmp_load_result_n2103 = hxc_l_hudView3;
                bool hxc_l_tmp_load_result_n2104 = hxc_l_hudView4;
                bool hxc_l_tmp_load_result_n2105 = hxc_l_hudView5;
                int32_t hxc_l_tmp_load_result_n2106 = hxc_l_pickupAmount;
                int32_t hxc_l_tmp_load_result_n2107 = hxc_l_inventoryFullReason;
                int32_t hxc_l_tmp_load_result_n2108 = hxc_l_recoveryFeedback;
                bool hxc_l_tmp_load_result_n2109 = hxc_l_hudView6;
                bool hxc_l_tmp_load_result_n2111 = hxc_l_paused;
                bool hxc_l_tmp_load_result_n2112 = hxc_l_captured;
                int32_t hxc_l_tmp_load_result_n2113 = hxc_l_promptDevice;
                struct hxc_caxecraft_domain_RaycastHit hxc_l_tmp_load_result_n2114 = hxc_l_hit;
                int32_t hxc_l_tmp_load_result_n2115 = hxc_l_selectedMode;
                int32_t hxc_l_tmp_load_result_n2116 = hxc_l_locale;
                struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_load_result_n2117 = hxc_l_inventory;
                struct hxc_optional_caxecraft_app_ConversationState hxc_l_tmp_load_result_n2118 = hxc_l_conversation;
                hxc_string hxc_l_tmp_record_field_activeDialogue_owner_n588 = hxc_l_activeDialogue;
                if (hxc_string_retain(hxc_l_tmp_record_field_activeDialogue_owner_n588) != HXC_STATUS_OK)
                {
                  abort();
                }
                hxc_string hxc_l_tmp_record_field_activeDialogue_owned_load_result_n2120 = hxc_l_tmp_record_field_activeDialogue_owner_n588;
                hxc_string hxc_l_tmp_record_field_conversationSpeaker_owner_n589 = hxc_l_conversationSpeaker;
                if (hxc_string_retain(hxc_l_tmp_record_field_conversationSpeaker_owner_n589) != HXC_STATUS_OK)
                {
                  abort();
                }
                hxc_string hxc_l_tmp_record_field_conversationSpeaker_owned_load_result_n2122 = hxc_l_tmp_record_field_conversationSpeaker_owner_n589;
                hxc_string hxc_l_tmp_record_field_conversationSpeakerName_owner_n590 = hxc_l_hudView7_hef5dfe6036f4;
                if (hxc_string_retain(hxc_l_tmp_record_field_conversationSpeakerName_owner_n590) != HXC_STATUS_OK)
                {
                  abort();
                }
                hxc_string hxc_l_tmp_record_field_conversationSpeakerName_owned_load_result_n2124 = hxc_l_tmp_record_field_conversationSpeakerName_owner_n590;
                hxc_string hxc_l_tmp_record_field_conversationPortraitAsset_owner_n591 = hxc_l_hudView8;
                if (hxc_string_retain(hxc_l_tmp_record_field_conversationPortraitAsset_owner_n591) != HXC_STATUS_OK)
                {
                  abort();
                }
                hxc_string hxc_l_tmp_record_field_conversationPortraitAsset_owned_load_result_n2126 = hxc_l_tmp_record_field_conversationPortraitAsset_owner_n591;
                int32_t hxc_l_tmp_load_result_n2127 = hxc_l_hudView9;
                enum hxc_caxecraft_app_InteractionPrompt hxc_l_tmp_load_result_n2128 = hxc_l_availableInteractionPrompt;
                struct hxc_caxecraft_domain_Character hxc_l_tmp_load_result_n2129 = hxc_l_enemyActor;
                enum hxc_caxecraft_domain_ActorControllerPhase hxc_l_tmp_load_result_n2130 = hxc_l_enemyPhase_h073acd63e276;
                hxc_string hxc_l_tmp_record_field_levelTitle_owner_n592 = hxc_l_hudView10;
                if (hxc_string_retain(hxc_l_tmp_record_field_levelTitle_owner_n592) != HXC_STATUS_OK)
                {
                  abort();
                }
                hxc_string hxc_l_tmp_record_field_levelTitle_owned_load_result_n2132 = hxc_l_tmp_record_field_levelTitle_owner_n592;
                hxc_string hxc_l_tmp_record_field_objectiveTitle_owner_n593 = hxc_l_hudView11;
                if (hxc_string_retain(hxc_l_tmp_record_field_objectiveTitle_owner_n593) != HXC_STATUS_OK)
                {
                  abort();
                }
                hxc_string hxc_l_tmp_record_field_objectiveTitle_owned_load_result_n2134 = hxc_l_tmp_record_field_objectiveTitle_owner_n593;
                hxc_string hxc_l_tmp_record_field_journalTitle_owner_n594 = hxc_l_tmp_managed_flow_owner_n580;
                if (hxc_string_retain(hxc_l_tmp_record_field_journalTitle_owner_n594) != HXC_STATUS_OK)
                {
                  abort();
                }
                hxc_string hxc_l_tmp_record_field_journalTitle_owned_load_result_n2136 = hxc_l_tmp_record_field_journalTitle_owner_n594;
                hxc_string hxc_l_tmp_record_field_journalBody_owner_n595 = hxc_l_tmp_managed_flow_owner_n586;
                if (hxc_string_retain(hxc_l_tmp_record_field_journalBody_owner_n595) != HXC_STATUS_OK)
                {
                  abort();
                }
                hxc_string hxc_l_tmp_record_field_journalBody_owned_load_result_n2138 = hxc_l_tmp_record_field_journalBody_owner_n595;
                struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n2139 = hxc_l_levelView_hb85a9a956786;
                hxc_l_gc_roots[659] = (const void *)hxc_l_tmp_load_result_n2139;
                if (hxc_l_tmp_load_result_n2139 == NULL)
                {
                  abort();
                }
                struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_class_field_load_result_n2140 = (*hxc_l_tmp_load_result_n2139).hxc_levelPresentation;
                hxc_l_gc_roots[660] = (const void *)hxc_l_tmp_class_field_load_result_n2140;
                hxc_l_gc_roots[661] = (const void *)(struct hxc_caxecraft_app_HudView){ .hxc_activeDialogue = hxc_l_tmp_record_field_activeDialogue_owned_load_result_n2120, .hxc_character = hxc_l_tmp_load_result_n2100, .hxc_conversation = hxc_l_tmp_load_result_n2118, .hxc_conversationPortraitAsset = hxc_l_tmp_record_field_conversationPortraitAsset_owned_load_result_n2126, .hxc_conversationPortraitCell = hxc_l_tmp_load_result_n2127, .hxc_conversationSpeaker = hxc_l_tmp_record_field_conversationSpeaker_owned_load_result_n2122, .hxc_conversationSpeakerName = hxc_l_tmp_record_field_conversationSpeakerName_owned_load_result_n2124, .hxc_debugMetricsVisible = hxc_l_tmp_load_result_n2094, .hxc_enemy = hxc_l_tmp_load_result_n2129, .hxc_enemyPhase = hxc_l_tmp_load_result_n2130, .hxc_feedback = (struct hxc_caxecraft_app_HudFeedbackView){ .hxc_enemyAttacked = hxc_l_tmp_load_result_n2104, .hxc_enemyDefeated = hxc_l_tmp_load_result_n2103, .hxc_inventoryFullReason = hxc_l_tmp_load_result_n2107, .hxc_pickedUp = hxc_l_tmp_load_result_n2105, .hxc_pickupAmount = hxc_l_tmp_load_result_n2106, .hxc_placementBlocked = hxc_l_tmp_load_result_n2101, .hxc_recoveryDecision = hxc_l_tmp_load_result_n2108, .hxc_recoveryVisible = hxc_l_tmp_load_result_n2109, .hxc_strikeHit = hxc_l_tmp_load_result_n2102 }, .hxc_hit = hxc_l_tmp_load_result_n2114, .hxc_interactionPrompt = hxc_l_tmp_load_result_n2128, .hxc_inventory = hxc_l_tmp_load_result_n2117, .hxc_journalBody = hxc_l_tmp_record_field_journalBody_owned_load_result_n2138, .hxc_journalTitle = hxc_l_tmp_record_field_journalTitle_owned_load_result_n2136, .hxc_levelTitle = hxc_l_tmp_record_field_levelTitle_owned_load_result_n2132, .hxc_locale = hxc_l_tmp_load_result_n2116, .hxc_metrics = (struct hxc_caxecraft_app_HudMetricsView){ .hxc_completedTicks = hxc_l_tmp_load_result_n2098, .hxc_drawCalls = hxc_l_tmp_load_result_n2096, .hxc_renderedFrames = hxc_l_tmp_load_result_n2097, .hxc_visibleBlocks = hxc_l_tmp_load_result_n2095 }, .hxc_mode = hxc_l_tmp_load_result_n2115, .hxc_objectiveTitle = hxc_l_tmp_record_field_objectiveTitle_owned_load_result_n2134, .hxc_paused = hxc_l_tmp_load_result_n2111, .hxc_pointerCaptured = hxc_l_tmp_load_result_n2112, .hxc_presentation = hxc_l_tmp_class_field_load_result_n2140, .hxc_promptDevice = hxc_l_tmp_load_result_n2113 }.hxc_presentation;
                hxc_l_hudView_h32a7fe570f99 = (struct hxc_caxecraft_app_HudView){ .hxc_activeDialogue = hxc_l_tmp_record_field_activeDialogue_owned_load_result_n2120, .hxc_character = hxc_l_tmp_load_result_n2100, .hxc_conversation = hxc_l_tmp_load_result_n2118, .hxc_conversationPortraitAsset = hxc_l_tmp_record_field_conversationPortraitAsset_owned_load_result_n2126, .hxc_conversationPortraitCell = hxc_l_tmp_load_result_n2127, .hxc_conversationSpeaker = hxc_l_tmp_record_field_conversationSpeaker_owned_load_result_n2122, .hxc_conversationSpeakerName = hxc_l_tmp_record_field_conversationSpeakerName_owned_load_result_n2124, .hxc_debugMetricsVisible = hxc_l_tmp_load_result_n2094, .hxc_enemy = hxc_l_tmp_load_result_n2129, .hxc_enemyPhase = hxc_l_tmp_load_result_n2130, .hxc_feedback = (struct hxc_caxecraft_app_HudFeedbackView){ .hxc_enemyAttacked = hxc_l_tmp_load_result_n2104, .hxc_enemyDefeated = hxc_l_tmp_load_result_n2103, .hxc_inventoryFullReason = hxc_l_tmp_load_result_n2107, .hxc_pickedUp = hxc_l_tmp_load_result_n2105, .hxc_pickupAmount = hxc_l_tmp_load_result_n2106, .hxc_placementBlocked = hxc_l_tmp_load_result_n2101, .hxc_recoveryDecision = hxc_l_tmp_load_result_n2108, .hxc_recoveryVisible = hxc_l_tmp_load_result_n2109, .hxc_strikeHit = hxc_l_tmp_load_result_n2102 }, .hxc_hit = hxc_l_tmp_load_result_n2114, .hxc_interactionPrompt = hxc_l_tmp_load_result_n2128, .hxc_inventory = hxc_l_tmp_load_result_n2117, .hxc_journalBody = hxc_l_tmp_record_field_journalBody_owned_load_result_n2138, .hxc_journalTitle = hxc_l_tmp_record_field_journalTitle_owned_load_result_n2136, .hxc_levelTitle = hxc_l_tmp_record_field_levelTitle_owned_load_result_n2132, .hxc_locale = hxc_l_tmp_load_result_n2116, .hxc_metrics = (struct hxc_caxecraft_app_HudMetricsView){ .hxc_completedTicks = hxc_l_tmp_load_result_n2098, .hxc_drawCalls = hxc_l_tmp_load_result_n2096, .hxc_renderedFrames = hxc_l_tmp_load_result_n2097, .hxc_visibleBlocks = hxc_l_tmp_load_result_n2095 }, .hxc_mode = hxc_l_tmp_load_result_n2115, .hxc_objectiveTitle = hxc_l_tmp_record_field_objectiveTitle_owned_load_result_n2134, .hxc_paused = hxc_l_tmp_load_result_n2111, .hxc_pointerCaptured = hxc_l_tmp_load_result_n2112, .hxc_presentation = hxc_l_tmp_class_field_load_result_n2140, .hxc_promptDevice = hxc_l_tmp_load_result_n2113 };
                struct hxc_caxecraft_app_HudView hxc_l_tmp_load_result_n2142 = hxc_l_hudView_h32a7fe570f99;
                hxc_l_gc_roots[662] = (const void *)hxc_l_tmp_load_result_n2142.hxc_presentation;
                struct hxc_caxecraft_app_HudResources hxc_l_tmp_load_result_n2143 = hxc_l_hudResources;
                struct hxc_caxecraft_app_RuntimeTextureAtlasCatalog *hxc_l_tmp_load_result_n2144 = hxc_l_runtimeTextures;
                hxc_l_gc_roots[663] = (const void *)hxc_l_tmp_load_result_n2144;
                struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_tmp_load_result_n2145 = hxc_l_contentRegistry;
                hxc_l_gc_roots[664] = (const void *)hxc_l_tmp_load_result_n2145;
                hxc_l_gc_roots[665] = (const void *)hxc_l_uiCatalog;
                hxc_caxecraft_app_CaxecraftApp_drawHud(hxc_l_tmp_load_result_n2142, hxc_l_tmp_load_result_n2143, hxc_l_tmp_load_result_n2144, hxc_l_tmp_load_result_n2145, hxc_l_uiCatalog);
                hxc_record_c5db1a9e_destroy(&hxc_l_hudView_h32a7fe570f99);
                if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n586) != HXC_STATUS_OK)
                {
                  abort();
                }
                if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n580) != HXC_STATUS_OK)
                {
                  abort();
                }
                if (hxc_string_release(&hxc_l_hudView11) != HXC_STATUS_OK)
                {
                  abort();
                }
                if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n573) != HXC_STATUS_OK)
                {
                  abort();
                }
                if (hxc_string_release(&hxc_l_hudView10) != HXC_STATUS_OK)
                {
                  abort();
                }
                if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n569) != HXC_STATUS_OK)
                {
                  abort();
                }
                if (hxc_string_release(&hxc_l_hudView8) != HXC_STATUS_OK)
                {
                  abort();
                }
                if (hxc_string_release(&hxc_l_hudView7_hef5dfe6036f4) != HXC_STATUS_OK)
                {
                  abort();
                }
                if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n563) != HXC_STATUS_OK)
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
              enum hxc_caxecraft_app_AppScreen hxc_l_tmp_call_result_n2149 = hxc_caxecraft_app_AppScreen_closeEditor(hxc_l_screen);
              hxc_l_screen = hxc_l_tmp_call_result_n2149;
              break;
            }
          case hxc_caxecraft_app_EditorScreenAction_StartTestPlay:
            {
              if (hxc_l_editorActionForFrame.hxc_tag != hxc_caxecraft_app_EditorScreenAction_StartTestPlay)
              {
                abort();
              }
              struct hxc_bytes_ref *hxc_l_tmp_enum_payload_project_n2151 = hxc_l_editorActionForFrame.hxc_payload.hxc_StartTestPlay.hxc_canonical;
              struct hxc_bytes_ref *hxc_l_canonical = hxc_l_tmp_enum_payload_project_n2151;
              struct hxc_caxecraft_app_ActivePlayableLevel *hxc_l_tmp_load_result_n2152 = hxc_l_activeLevel;
              hxc_l_gc_roots[666] = (const void *)hxc_l_tmp_load_result_n2152;
              if (hxc_l_tmp_load_result_n2152 == NULL)
              {
                abort();
              }
              struct hxc_caxecraft_content_ActiveContent *hxc_l_tmp_class_field_load_result_n2153 = (*hxc_l_tmp_load_result_n2152).hxc_content;
              hxc_l_gc_roots[667] = (const void *)hxc_l_tmp_class_field_load_result_n2153;
              if (hxc_l_tmp_class_field_load_result_n2153 == NULL)
              {
                abort();
              }
              struct hxc_caxecraft_content_LoadedContentGeneration *hxc_l_tmp_class_field_load_result_n2154 = (*hxc_l_tmp_class_field_load_result_n2153).hxc_active;
              hxc_l_gc_roots[668] = (const void *)hxc_l_tmp_class_field_load_result_n2154;
              if (hxc_l_tmp_class_field_load_result_n2154 == NULL)
              {
                abort();
              }
              struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_class_field_load_result_n2155 = (*hxc_l_tmp_class_field_load_result_n2154).hxc_ownedSession;
              hxc_l_gc_roots[669] = (const void *)hxc_l_tmp_class_field_load_result_n2155;
              if (hxc_l_tmp_class_field_load_result_n2155 == NULL)
              {
                abort();
              }
              struct hxc_caxecraft_domain_GameView hxc_l_tmp_instance_call_result_n2156 = hxc_caxecraft_domain_GameSession_view(hxc_l_tmp_class_field_load_result_n2155);
              struct hxc_caxecraft_domain_Character hxc_l_ordinaryPlayer = hxc_l_tmp_instance_call_result_n2156.hxc_localPlayer;
              struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_tmp_load_result_n2158 = hxc_l_contentRegistry;
              hxc_l_gc_roots[670] = (const void *)hxc_l_tmp_load_result_n2158;
              if (hxc_l_tmp_load_result_n2158 == NULL)
              {
                abort();
              }
              struct hxc_compiler_interface_dispatch_caxecraft_scenario_ScenarioContentRegistry_value hxc_l_tmp_interface_value_n2159 = (struct hxc_compiler_interface_dispatch_caxecraft_scenario_ScenarioContentRegistry_value){ .object = hxc_l_tmp_load_result_n2158, .table = &hxc_itable_compiler_interface_dispatch_caxecraft_content_RuntimeContentRegistry_itable_layout_caxecraf_hb3f384641e1a61bf };
              struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_tmp_load_result_n2160 = hxc_l_contentRegistry;
              hxc_l_gc_roots[671] = (const void *)hxc_l_tmp_load_result_n2160;
              if (hxc_l_tmp_load_result_n2160 == NULL)
              {
                abort();
              }
              struct hxc_compiler_interface_dispatch_caxecraft_content_LevelContentResolver_value hxc_l_tmp_interface_value_n2161 = (struct hxc_compiler_interface_dispatch_caxecraft_content_LevelContentResolver_value){ .object = hxc_l_tmp_load_result_n2160, .table = &hxc_itable_compiler_interface_dispatch_caxecraft_content_RuntimeContentRegistry_itable_layout_caxecraf_h82ca4f24ca7a7b5a };
              struct hxc_caxecraft_content_ContentGenerationSequence *hxc_l_tmp_load_result_n2162 = hxc_l_generationSequence;
              hxc_l_gc_roots[672] = (const void *)hxc_l_tmp_load_result_n2162;
              struct hxc_caxecraft_app_EditorTestPlayRuntime *hxc_l_tmp_managed_class_result_n2163 = NULL;
              if (hxc_gc_allocate(&hxc_program_gc, &hxc_caxecraft_app_EditorTestPlayRuntime_descriptor, (void **)&hxc_l_tmp_managed_class_result_n2163) != HXC_STATUS_OK)
              {
                abort();
              }
              hxc_l_gc_roots[673] = (const void *)hxc_l_tmp_managed_class_result_n2163;
              hxc_compiler_constructor_caxecraft_app_EditorTestPlayRuntime(hxc_l_tmp_managed_class_result_n2163, hxc_l_tmp_interface_value_n2159, hxc_l_tmp_interface_value_n2161, hxc_l_tmp_load_result_n2162);
              hxc_l_gc_roots[674] = (const void *)hxc_l_tmp_managed_class_result_n2163;
              struct hxc_caxecraft_app_EditorTestPlayRuntime *hxc_l_proposedRuntime = hxc_l_tmp_managed_class_result_n2163;
              struct hxc_caxecraft_app_EditorTestPlayRuntime *hxc_l_tmp_load_result_n2164 = hxc_l_proposedRuntime;
              hxc_l_gc_roots[675] = (const void *)hxc_l_tmp_load_result_n2164;
              if (hxc_l_tmp_load_result_n2164 == NULL)
              {
                abort();
              }
              struct hxc_bytes_ref *hxc_l_tmp_record_field_canonical_owner_n599 = hxc_l_canonical;
              if (hxc_bytes_ref_retain(hxc_l_tmp_record_field_canonical_owner_n599) != HXC_STATUS_OK)
              {
                abort();
              }
              struct hxc_bytes_ref *hxc_l_tmp_record_field_canonical_owned_load_result_n2166 = hxc_l_tmp_record_field_canonical_owner_n599;
              int32_t hxc_l_tmp_record_field_load_result_n2167 = hxc_l_ordinaryPlayer.hxc_id;
              int32_t hxc_l_tmp_record_field_load_result_n2168 = hxc_l_ordinaryPlayer.hxc_vitals.hxc_health;
              hxc_l_tmp_instance_call_argument_0_owner_n600 = (struct hxc_caxecraft_app_EditorTestPlayRequest){ .hxc_canonical = hxc_l_tmp_record_field_canonical_owned_load_result_n2166, .hxc_playerOptions = (struct hxc_caxecraft_content_LevelPlayerOptions){ .hxc_aquaticProfile = hxc_l_ordinaryPlayer.hxc_aquaticProfile, .hxc_entityId = hxc_l_tmp_record_field_load_result_n2167, .hxc_initialHealth = hxc_l_tmp_record_field_load_result_n2168 } };
              struct hxc_caxecraft_app_EditorTestPlayStartResult hxc_l_tmp_instance_call_result_n2173 = hxc_caxecraft_app_EditorTestPlayRuntime_start(hxc_l_tmp_load_result_n2164, hxc_l_tmp_instance_call_argument_0_owner_n600);
              hxc_l_tmp_enum_switch_subject_owner_n601 = hxc_l_tmp_instance_call_result_n2173;
              switch (hxc_l_tmp_enum_switch_subject_owner_n601.hxc_tag) {
                case hxc_caxecraft_app_EditorTestPlayStartResult_EditorTestPlayStarted:
                  {
                    struct hxc_caxecraft_app_EditorTestPlayRuntime *hxc_l_tmp_load_result_n2175 = hxc_l_proposedRuntime;
                    hxc_l_gc_roots[676] = (const void *)hxc_l_tmp_load_result_n2175;
                    if (hxc_l_tmp_load_result_n2175 == NULL)
                    {
                      abort();
                    }
                    struct hxc_caxecraft_app_ActivePlayableLevel *hxc_l_tmp_class_field_load_result_n2176 = (*hxc_l_tmp_load_result_n2175).hxc_active;
                    hxc_l_gc_roots[677] = (const void *)hxc_l_tmp_class_field_load_result_n2176;
                    struct hxc_caxecraft_app_ActivePlayableLevel *hxc_l_testOwner = hxc_l_tmp_class_field_load_result_n2176;
                    hxc_l_gc_roots[678] = (const void *)hxc_l_testOwner;
                    hxc_l_gc_roots[679] = (const void *)NULL;
                    if (hxc_l_testOwner == NULL)
                    {
                      struct hxc_caxecraft_app_CaxecraftEditorScreen *hxc_l_tmp_load_result_n2178 = hxc_l_editorScreen;
                      hxc_l_gc_roots[682] = (const void *)hxc_l_tmp_load_result_n2178;
                      if (hxc_l_tmp_load_result_n2178 == NULL)
                      {
                        abort();
                      }
                      hxc_caxecraft_app_CaxecraftEditorScreen_finishTestPlay(hxc_l_tmp_load_result_n2178, false);
                    }
                    else
                    {
                      struct hxc_caxecraft_app_ActivePlayableLevel *hxc_l_tmp_load_result_n2179 = hxc_l_testOwner;
                      hxc_l_gc_roots[683] = (const void *)hxc_l_tmp_load_result_n2179;
                      if (hxc_l_tmp_load_result_n2179 == NULL)
                      {
                        abort();
                      }
                      struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_class_field_load_result_n2180 = (*hxc_l_tmp_load_result_n2179).hxc_activeView;
                      hxc_l_gc_roots[684] = (const void *)hxc_l_tmp_class_field_load_result_n2180;
                      struct hxc_caxecraft_app_PlayableLevelView *hxc_l_testLevel_hc67e923a7029 = hxc_l_tmp_class_field_load_result_n2180;
                      struct hxc_caxecraft_app_ActivePlayableLevel *hxc_l_tmp_load_result_n2181 = hxc_l_testOwner;
                      hxc_l_gc_roots[685] = (const void *)hxc_l_tmp_load_result_n2181;
                      if (hxc_l_tmp_load_result_n2181 == NULL)
                      {
                        abort();
                      }
                      struct hxc_caxecraft_content_ActiveContent *hxc_l_tmp_class_field_load_result_n2182 = (*hxc_l_tmp_load_result_n2181).hxc_content;
                      hxc_l_gc_roots[686] = (const void *)hxc_l_tmp_class_field_load_result_n2182;
                      if (hxc_l_tmp_class_field_load_result_n2182 == NULL)
                      {
                        abort();
                      }
                      struct hxc_caxecraft_content_LoadedContentGeneration *hxc_l_tmp_class_field_load_result_n2183 = (*hxc_l_tmp_class_field_load_result_n2182).hxc_active;
                      hxc_l_gc_roots[687] = (const void *)hxc_l_tmp_class_field_load_result_n2183;
                      if (hxc_l_tmp_class_field_load_result_n2183 == NULL)
                      {
                        abort();
                      }
                      struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_class_field_load_result_n2184 = (*hxc_l_tmp_class_field_load_result_n2183).hxc_ownedSession;
                      hxc_l_gc_roots[688] = (const void *)hxc_l_tmp_class_field_load_result_n2184;
                      struct hxc_caxecraft_domain_GameSession *hxc_l_testSession = hxc_l_tmp_class_field_load_result_n2184;
                      struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n2185 = hxc_l_testSession;
                      hxc_l_gc_roots[689] = (const void *)hxc_l_tmp_load_result_n2185;
                      if (hxc_l_tmp_load_result_n2185 == NULL)
                      {
                        abort();
                      }
                      struct hxc_caxecraft_domain_GameView hxc_l_tmp_instance_call_result_n2186 = hxc_caxecraft_domain_GameSession_view(hxc_l_tmp_load_result_n2185);
                      struct hxc_caxecraft_domain_GameView hxc_l_testView = hxc_l_tmp_instance_call_result_n2186;
                      struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n2187 = hxc_l_testSession;
                      hxc_l_gc_roots[690] = (const void *)hxc_l_tmp_load_result_n2187;
                      if (hxc_l_tmp_load_result_n2187 == NULL)
                      {
                        abort();
                      }
                      struct hxc_array_ref *hxc_l_tmp_instance_call_result_n2188 = hxc_caxecraft_domain_GameSession_actorControllerStateSnapshots(hxc_l_tmp_load_result_n2187);
                      hxc_l_testPhases = hxc_l_tmp_instance_call_result_n2188;
                      struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n2189 = hxc_l_testSession;
                      hxc_l_gc_roots[691] = (const void *)hxc_l_tmp_load_result_n2189;
                      if (hxc_l_tmp_load_result_n2189 == NULL)
                      {
                        abort();
                      }
                      struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n2190 = hxc_l_testLevel_hc67e923a7029;
                      hxc_l_gc_roots[692] = (const void *)hxc_l_tmp_load_result_n2190;
                      if (hxc_l_tmp_load_result_n2190 == NULL)
                      {
                        abort();
                      }
                      int32_t hxc_l_tmp_class_field_load_result_n2191 = (*hxc_l_tmp_load_result_n2190).hxc_enemyId;
                      struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n2192 = hxc_caxecraft_domain_GameSession_readCharacter(hxc_l_tmp_load_result_n2189, hxc_l_tmp_class_field_load_result_n2191);
                      struct hxc_caxecraft_domain_Character hxc_l_testEnemy = hxc_l_tmp_instance_call_result_n2192;
                      struct hxc_array_ref *hxc_l_tmp_load_result_n2193 = hxc_l_testPhases;
                      struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n2194 = hxc_l_testLevel_hc67e923a7029;
                      hxc_l_gc_roots[693] = (const void *)hxc_l_tmp_load_result_n2194;
                      if (hxc_l_tmp_load_result_n2194 == NULL)
                      {
                        abort();
                      }
                      int32_t hxc_l_tmp_class_field_load_result_n2195 = (*hxc_l_tmp_load_result_n2194).hxc_enemyId;
                      struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation hxc_l_tmp_call_result_n2197 = hxc_caxecraft_app_CaxecraftApp_observeActorPhase(hxc_l_tmp_load_result_n2193, hxc_l_tmp_class_field_load_result_n2195, hxc_caxecraft_domain_ActorControllerPhase_Resting);
                      struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation hxc_l_testEnemyPhase = hxc_l_tmp_call_result_n2197;
                      bool hxc_l_tmp_record_field_load_result_n2198 = hxc_l_testView.hxc_valid;
                      bool hxc_l_tmp_short_circuit_result_n609 = !hxc_l_tmp_record_field_load_result_n2198;
                      if (!!hxc_l_tmp_record_field_load_result_n2198)
                      {
                        hxc_l_tmp_short_circuit_result_n609 = hxc_l_testEnemy.hxc_id <= 0;
                      }
                      bool hxc_l_tmp_short_circuit_load_result_n2200 = hxc_l_tmp_short_circuit_result_n609;
                      bool hxc_l_tmp_short_circuit_result_n610 = hxc_l_tmp_short_circuit_load_result_n2200;
                      if (!hxc_l_tmp_short_circuit_load_result_n2200)
                      {
                        hxc_l_tmp_short_circuit_result_n610 = !hxc_l_testEnemyPhase.hxc_valid;
                      }
                      bool hxc_l_tmp_short_circuit_load_result_n2202 = hxc_l_tmp_short_circuit_result_n610;
                      bool hxc_l_tmp_short_circuit_result_n611 = hxc_l_tmp_short_circuit_load_result_n2202;
                      if (!hxc_l_tmp_short_circuit_load_result_n2202)
                      {
                        struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n2203 = hxc_l_testSession;
                        hxc_l_gc_roots[694] = (const void *)hxc_l_tmp_load_result_n2203;
                        struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n2204 = hxc_l_testLevel_hc67e923a7029;
                        hxc_l_gc_roots[695] = (const void *)hxc_l_tmp_load_result_n2204;
                        bool hxc_l_tmp_call_result_n2206 = hxc_caxecraft_app_CaxecraftApp_dialogueActorsAreValid(hxc_l_tmp_load_result_n2203, hxc_l_tmp_load_result_n2204, hxc_l_testPhases);
                        hxc_l_tmp_short_circuit_result_n611 = !hxc_l_tmp_call_result_n2206;
                      }
                      if (hxc_l_tmp_short_circuit_result_n611)
                      {
                        struct hxc_caxecraft_app_EditorTestPlayRuntime *hxc_l_tmp_load_result_n2208 = hxc_l_proposedRuntime;
                        hxc_l_gc_roots[696] = (const void *)hxc_l_tmp_load_result_n2208;
                        if (hxc_l_tmp_load_result_n2208 == NULL)
                        {
                          abort();
                        }
                        hxc_caxecraft_app_EditorTestPlayRuntime_stop(hxc_l_tmp_load_result_n2208);
                        struct hxc_caxecraft_app_CaxecraftEditorScreen *hxc_l_tmp_load_result_n2209 = hxc_l_editorScreen;
                        hxc_l_gc_roots[697] = (const void *)hxc_l_tmp_load_result_n2209;
                        if (hxc_l_tmp_load_result_n2209 == NULL)
                        {
                          abort();
                        }
                        hxc_caxecraft_app_CaxecraftEditorScreen_finishTestPlay(hxc_l_tmp_load_result_n2209, false);
                      }
                      else
                      {
                        struct hxc_caxecraft_app_CaxecraftEditorScreen *hxc_l_tmp_load_result_n2210 = hxc_l_editorScreen;
                        hxc_l_gc_roots[698] = (const void *)hxc_l_tmp_load_result_n2210;
                        if (hxc_l_tmp_load_result_n2210 == NULL)
                        {
                          abort();
                        }
                        bool hxc_l_tmp_instance_call_result_n2211 = hxc_caxecraft_app_CaxecraftEditorScreen_beginTestPlay(hxc_l_tmp_load_result_n2210);
                        if (!hxc_l_tmp_instance_call_result_n2211)
                        {
                          struct hxc_caxecraft_app_EditorTestPlayRuntime *hxc_l_tmp_load_result_n2212 = hxc_l_proposedRuntime;
                          hxc_l_gc_roots[699] = (const void *)hxc_l_tmp_load_result_n2212;
                          if (hxc_l_tmp_load_result_n2212 == NULL)
                          {
                            abort();
                          }
                          hxc_caxecraft_app_EditorTestPlayRuntime_stop(hxc_l_tmp_load_result_n2212);
                          struct hxc_caxecraft_app_CaxecraftEditorScreen *hxc_l_tmp_load_result_n2213 = hxc_l_editorScreen;
                          hxc_l_gc_roots[700] = (const void *)hxc_l_tmp_load_result_n2213;
                          if (hxc_l_tmp_load_result_n2213 == NULL)
                          {
                            abort();
                          }
                          hxc_caxecraft_app_CaxecraftEditorScreen_finishTestPlay(hxc_l_tmp_load_result_n2213, false);
                        }
                        else
                        {
                          struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_load_result_n2214 = hxc_l_inventory;
                          struct hxc_optional_caxecraft_app_ConversationState hxc_l_tmp_load_result_n2215 = hxc_l_conversation;
                          hxc_string hxc_l_tmp_record_field_activeDialogue_owner_n613 = hxc_l_activeDialogue;
                          if (hxc_string_retain(hxc_l_tmp_record_field_activeDialogue_owner_n613) != HXC_STATUS_OK)
                          {
                            abort();
                          }
                          hxc_string hxc_l_tmp_record_field_activeDialogue_owned_load_result_n2217 = hxc_l_tmp_record_field_activeDialogue_owner_n613;
                          hxc_string hxc_l_tmp_record_field_latestJournalId_owner_n614 = hxc_l_latestJournalId;
                          if (hxc_string_retain(hxc_l_tmp_record_field_latestJournalId_owner_n614) != HXC_STATUS_OK)
                          {
                            abort();
                          }
                          hxc_string hxc_l_tmp_record_field_latestJournalId_owned_load_result_n2219 = hxc_l_tmp_record_field_latestJournalId_owner_n614;
                          hxc_string hxc_l_tmp_record_field_currentObjectiveId_owner_n615 = hxc_l_currentObjectiveId;
                          if (hxc_string_retain(hxc_l_tmp_record_field_currentObjectiveId_owner_n615) != HXC_STATUS_OK)
                          {
                            abort();
                          }
                          hxc_string hxc_l_tmp_record_field_currentObjectiveId_owned_load_result_n2221 = hxc_l_tmp_record_field_currentObjectiveId_owner_n615;
                          enum hxc_caxecraft_app_InteractionPrompt hxc_l_tmp_load_result_n2222 = hxc_l_availableInteractionPrompt;
                          struct hxc_caxecraft_domain_Character hxc_l_tmp_load_result_n2223 = hxc_l_enemyActor;
                          struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation hxc_l_tmp_load_result_n2224 = hxc_l_enemyPhase_ha225e19c84ac;
                          struct hxc_caxecraft_gameplay_SwordCombatState hxc_l_tmp_load_result_n2225 = hxc_l_swordCombat;
                          struct hxc_caxecraft_gameplay_BerryDropState hxc_l_tmp_load_result_n2226 = hxc_l_berryDrop;
                          double hxc_l_tmp_load_result_n2227 = hxc_l_cameraWaterBlend;
                          enum hxc_caxecraft_domain_PlayerCameraMode hxc_l_tmp_load_result_n2228 = hxc_l_cameraMode;
                          double hxc_l_tmp_load_result_n2229 = hxc_l_lookX;
                          double hxc_l_tmp_load_result_n2230 = hxc_l_lookY;
                          double hxc_l_tmp_load_result_n2231 = hxc_l_lookZ;
                          double hxc_l_tmp_load_result_n2232 = hxc_l_accumulator;
                          struct hxc_caxecraft_app_MotionHistory hxc_l_tmp_load_result_n2233 = hxc_l_motionHistory;
                          bool hxc_l_tmp_load_result_n2234 = hxc_l_jumpQueued;
                          bool hxc_l_tmp_load_result_n2235 = hxc_l_swordQueued;
                          int32_t hxc_l_tmp_load_result_n2236 = hxc_l_placementBlockedFrames;
                          int32_t hxc_l_tmp_load_result_n2237 = hxc_l_strikeHitFrames;
                          int32_t hxc_l_tmp_load_result_n2238 = hxc_l_enemyDefeatedFrames;
                          int32_t hxc_l_tmp_load_result_n2239 = hxc_l_enemyAttackFrames;
                          int32_t hxc_l_tmp_load_result_n2240 = hxc_l_pickupFrames;
                          int32_t hxc_l_tmp_load_result_n2241 = hxc_l_pickupAmount;
                          int32_t hxc_l_tmp_load_result_n2242 = hxc_l_inventoryFullFrames;
                          int32_t hxc_l_tmp_load_result_n2243 = hxc_l_inventoryFullReason;
                          int32_t hxc_l_tmp_load_result_n2244 = hxc_l_recoveryFeedback;
                          int32_t hxc_l_tmp_load_result_n2245 = hxc_l_recoveryFeedbackFrames;
                          int32_t hxc_l_tmp_load_result_n2246 = hxc_l_aquaticEquipmentCode;
                          int32_t hxc_l_tmp_load_result_n2247 = hxc_l_aquaticEquipmentFrames;
                          struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_load_result_n2248 = hxc_l_campaignLevel;
                          hxc_l_gc_roots[701] = (const void *)hxc_l_tmp_load_result_n2248;
                          struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_load_result_n2249 = hxc_l_pendingCampaignLevel;
                          hxc_l_gc_roots[702] = (const void *)hxc_l_tmp_load_result_n2249;
                          struct hxc_caxecraft_app_PreparedCampaignPortal *hxc_l_tmp_load_result_n2250 = hxc_l_pendingCampaignPortal;
                          hxc_l_gc_roots[703] = (const void *)hxc_l_tmp_load_result_n2250;
                          hxc_string hxc_l_tmp_record_field_pendingCampaignLabel_owner_n616 = hxc_l_pendingCampaignLabel;
                          if (hxc_string_retain(hxc_l_tmp_record_field_pendingCampaignLabel_owner_n616) != HXC_STATUS_OK)
                          {
                            abort();
                          }
                          hxc_string hxc_l_tmp_record_field_pendingCampaignLabel_owned_load_result_n2252 = hxc_l_tmp_record_field_pendingCampaignLabel_owner_n616;
                          bool hxc_l_tmp_load_result_n2253 = hxc_l_loadingFramePresented;
                          hxc_string hxc_l_tmp_record_field_levelLabel_owner_n617 = hxc_l_levelLabel;
                          if (hxc_string_retain(hxc_l_tmp_record_field_levelLabel_owner_n617) != HXC_STATUS_OK)
                          {
                            abort();
                          }
                          hxc_string hxc_l_tmp_record_field_levelLabel_owned_load_result_n2255 = hxc_l_tmp_record_field_levelLabel_owner_n617;
                          int32_t hxc_l_tmp_load_result_n2256 = hxc_l_selectedCampaignLevelIndex;
                          hxc_l_gc_roots[704] = (const void *)hxc_l_preparedPortal;
                          hxc_l_gc_roots[705] = (const void *)(struct hxc_caxecraft_app_CaxecraftApp_EditorTestPlayShellSnapshot){ .hxc_accumulator = hxc_l_tmp_load_result_n2232, .hxc_activeDialogue = hxc_l_tmp_record_field_activeDialogue_owned_load_result_n2217, .hxc_aquaticEquipmentCode = hxc_l_tmp_load_result_n2246, .hxc_aquaticEquipmentFrames = hxc_l_tmp_load_result_n2247, .hxc_availableInteractionPrompt = hxc_l_tmp_load_result_n2222, .hxc_berryDrop = hxc_l_tmp_load_result_n2226, .hxc_cameraMode = hxc_l_tmp_load_result_n2228, .hxc_cameraWaterBlend = hxc_l_tmp_load_result_n2227, .hxc_campaignLevel = hxc_l_tmp_load_result_n2248, .hxc_conversation = hxc_l_tmp_load_result_n2215, .hxc_currentObjectiveId = hxc_l_tmp_record_field_currentObjectiveId_owned_load_result_n2221, .hxc_enemyActor = hxc_l_tmp_load_result_n2223, .hxc_enemyAttackFrames = hxc_l_tmp_load_result_n2239, .hxc_enemyDefeatedFrames = hxc_l_tmp_load_result_n2238, .hxc_enemyPhase = hxc_l_tmp_load_result_n2224, .hxc_inventory = hxc_l_tmp_load_result_n2214, .hxc_inventoryFullFrames = hxc_l_tmp_load_result_n2242, .hxc_inventoryFullReason = hxc_l_tmp_load_result_n2243, .hxc_jumpQueued = hxc_l_tmp_load_result_n2234, .hxc_latestJournalId = hxc_l_tmp_record_field_latestJournalId_owned_load_result_n2219, .hxc_levelLabel = hxc_l_tmp_record_field_levelLabel_owned_load_result_n2255, .hxc_loadingFramePresented = hxc_l_tmp_load_result_n2253, .hxc_lookX = hxc_l_tmp_load_result_n2229, .hxc_lookY = hxc_l_tmp_load_result_n2230, .hxc_lookZ = hxc_l_tmp_load_result_n2231, .hxc_motionHistory = hxc_l_tmp_load_result_n2233, .hxc_pendingCampaignLabel = hxc_l_tmp_record_field_pendingCampaignLabel_owned_load_result_n2252, .hxc_pendingCampaignLevel = hxc_l_tmp_load_result_n2249, .hxc_pendingCampaignPortal = hxc_l_tmp_load_result_n2250, .hxc_pickupAmount = hxc_l_tmp_load_result_n2241, .hxc_pickupFrames = hxc_l_tmp_load_result_n2240, .hxc_placementBlockedFrames = hxc_l_tmp_load_result_n2236, .hxc_preparedPortal = hxc_l_preparedPortal, .hxc_recoveryFeedback = hxc_l_tmp_load_result_n2244, .hxc_recoveryFeedbackFrames = hxc_l_tmp_load_result_n2245, .hxc_selectedCampaignLevelIndex = hxc_l_tmp_load_result_n2256, .hxc_strikeHitFrames = hxc_l_tmp_load_result_n2237, .hxc_swordCombat = hxc_l_tmp_load_result_n2225, .hxc_swordQueued = hxc_l_tmp_load_result_n2235 }.hxc_campaignLevel;
                          hxc_l_gc_roots[706] = (const void *)(struct hxc_caxecraft_app_CaxecraftApp_EditorTestPlayShellSnapshot){ .hxc_accumulator = hxc_l_tmp_load_result_n2232, .hxc_activeDialogue = hxc_l_tmp_record_field_activeDialogue_owned_load_result_n2217, .hxc_aquaticEquipmentCode = hxc_l_tmp_load_result_n2246, .hxc_aquaticEquipmentFrames = hxc_l_tmp_load_result_n2247, .hxc_availableInteractionPrompt = hxc_l_tmp_load_result_n2222, .hxc_berryDrop = hxc_l_tmp_load_result_n2226, .hxc_cameraMode = hxc_l_tmp_load_result_n2228, .hxc_cameraWaterBlend = hxc_l_tmp_load_result_n2227, .hxc_campaignLevel = hxc_l_tmp_load_result_n2248, .hxc_conversation = hxc_l_tmp_load_result_n2215, .hxc_currentObjectiveId = hxc_l_tmp_record_field_currentObjectiveId_owned_load_result_n2221, .hxc_enemyActor = hxc_l_tmp_load_result_n2223, .hxc_enemyAttackFrames = hxc_l_tmp_load_result_n2239, .hxc_enemyDefeatedFrames = hxc_l_tmp_load_result_n2238, .hxc_enemyPhase = hxc_l_tmp_load_result_n2224, .hxc_inventory = hxc_l_tmp_load_result_n2214, .hxc_inventoryFullFrames = hxc_l_tmp_load_result_n2242, .hxc_inventoryFullReason = hxc_l_tmp_load_result_n2243, .hxc_jumpQueued = hxc_l_tmp_load_result_n2234, .hxc_latestJournalId = hxc_l_tmp_record_field_latestJournalId_owned_load_result_n2219, .hxc_levelLabel = hxc_l_tmp_record_field_levelLabel_owned_load_result_n2255, .hxc_loadingFramePresented = hxc_l_tmp_load_result_n2253, .hxc_lookX = hxc_l_tmp_load_result_n2229, .hxc_lookY = hxc_l_tmp_load_result_n2230, .hxc_lookZ = hxc_l_tmp_load_result_n2231, .hxc_motionHistory = hxc_l_tmp_load_result_n2233, .hxc_pendingCampaignLabel = hxc_l_tmp_record_field_pendingCampaignLabel_owned_load_result_n2252, .hxc_pendingCampaignLevel = hxc_l_tmp_load_result_n2249, .hxc_pendingCampaignPortal = hxc_l_tmp_load_result_n2250, .hxc_pickupAmount = hxc_l_tmp_load_result_n2241, .hxc_pickupFrames = hxc_l_tmp_load_result_n2240, .hxc_placementBlockedFrames = hxc_l_tmp_load_result_n2236, .hxc_preparedPortal = hxc_l_preparedPortal, .hxc_recoveryFeedback = hxc_l_tmp_load_result_n2244, .hxc_recoveryFeedbackFrames = hxc_l_tmp_load_result_n2245, .hxc_selectedCampaignLevelIndex = hxc_l_tmp_load_result_n2256, .hxc_strikeHitFrames = hxc_l_tmp_load_result_n2237, .hxc_swordCombat = hxc_l_tmp_load_result_n2225, .hxc_swordQueued = hxc_l_tmp_load_result_n2235 }.hxc_pendingCampaignLevel;
                          hxc_l_gc_roots[707] = (const void *)(struct hxc_caxecraft_app_CaxecraftApp_EditorTestPlayShellSnapshot){ .hxc_accumulator = hxc_l_tmp_load_result_n2232, .hxc_activeDialogue = hxc_l_tmp_record_field_activeDialogue_owned_load_result_n2217, .hxc_aquaticEquipmentCode = hxc_l_tmp_load_result_n2246, .hxc_aquaticEquipmentFrames = hxc_l_tmp_load_result_n2247, .hxc_availableInteractionPrompt = hxc_l_tmp_load_result_n2222, .hxc_berryDrop = hxc_l_tmp_load_result_n2226, .hxc_cameraMode = hxc_l_tmp_load_result_n2228, .hxc_cameraWaterBlend = hxc_l_tmp_load_result_n2227, .hxc_campaignLevel = hxc_l_tmp_load_result_n2248, .hxc_conversation = hxc_l_tmp_load_result_n2215, .hxc_currentObjectiveId = hxc_l_tmp_record_field_currentObjectiveId_owned_load_result_n2221, .hxc_enemyActor = hxc_l_tmp_load_result_n2223, .hxc_enemyAttackFrames = hxc_l_tmp_load_result_n2239, .hxc_enemyDefeatedFrames = hxc_l_tmp_load_result_n2238, .hxc_enemyPhase = hxc_l_tmp_load_result_n2224, .hxc_inventory = hxc_l_tmp_load_result_n2214, .hxc_inventoryFullFrames = hxc_l_tmp_load_result_n2242, .hxc_inventoryFullReason = hxc_l_tmp_load_result_n2243, .hxc_jumpQueued = hxc_l_tmp_load_result_n2234, .hxc_latestJournalId = hxc_l_tmp_record_field_latestJournalId_owned_load_result_n2219, .hxc_levelLabel = hxc_l_tmp_record_field_levelLabel_owned_load_result_n2255, .hxc_loadingFramePresented = hxc_l_tmp_load_result_n2253, .hxc_lookX = hxc_l_tmp_load_result_n2229, .hxc_lookY = hxc_l_tmp_load_result_n2230, .hxc_lookZ = hxc_l_tmp_load_result_n2231, .hxc_motionHistory = hxc_l_tmp_load_result_n2233, .hxc_pendingCampaignLabel = hxc_l_tmp_record_field_pendingCampaignLabel_owned_load_result_n2252, .hxc_pendingCampaignLevel = hxc_l_tmp_load_result_n2249, .hxc_pendingCampaignPortal = hxc_l_tmp_load_result_n2250, .hxc_pickupAmount = hxc_l_tmp_load_result_n2241, .hxc_pickupFrames = hxc_l_tmp_load_result_n2240, .hxc_placementBlockedFrames = hxc_l_tmp_load_result_n2236, .hxc_preparedPortal = hxc_l_preparedPortal, .hxc_recoveryFeedback = hxc_l_tmp_load_result_n2244, .hxc_recoveryFeedbackFrames = hxc_l_tmp_load_result_n2245, .hxc_selectedCampaignLevelIndex = hxc_l_tmp_load_result_n2256, .hxc_strikeHitFrames = hxc_l_tmp_load_result_n2237, .hxc_swordCombat = hxc_l_tmp_load_result_n2225, .hxc_swordQueued = hxc_l_tmp_load_result_n2235 }.hxc_pendingCampaignPortal;
                          hxc_l_gc_roots[708] = (const void *)(struct hxc_caxecraft_app_CaxecraftApp_EditorTestPlayShellSnapshot){ .hxc_accumulator = hxc_l_tmp_load_result_n2232, .hxc_activeDialogue = hxc_l_tmp_record_field_activeDialogue_owned_load_result_n2217, .hxc_aquaticEquipmentCode = hxc_l_tmp_load_result_n2246, .hxc_aquaticEquipmentFrames = hxc_l_tmp_load_result_n2247, .hxc_availableInteractionPrompt = hxc_l_tmp_load_result_n2222, .hxc_berryDrop = hxc_l_tmp_load_result_n2226, .hxc_cameraMode = hxc_l_tmp_load_result_n2228, .hxc_cameraWaterBlend = hxc_l_tmp_load_result_n2227, .hxc_campaignLevel = hxc_l_tmp_load_result_n2248, .hxc_conversation = hxc_l_tmp_load_result_n2215, .hxc_currentObjectiveId = hxc_l_tmp_record_field_currentObjectiveId_owned_load_result_n2221, .hxc_enemyActor = hxc_l_tmp_load_result_n2223, .hxc_enemyAttackFrames = hxc_l_tmp_load_result_n2239, .hxc_enemyDefeatedFrames = hxc_l_tmp_load_result_n2238, .hxc_enemyPhase = hxc_l_tmp_load_result_n2224, .hxc_inventory = hxc_l_tmp_load_result_n2214, .hxc_inventoryFullFrames = hxc_l_tmp_load_result_n2242, .hxc_inventoryFullReason = hxc_l_tmp_load_result_n2243, .hxc_jumpQueued = hxc_l_tmp_load_result_n2234, .hxc_latestJournalId = hxc_l_tmp_record_field_latestJournalId_owned_load_result_n2219, .hxc_levelLabel = hxc_l_tmp_record_field_levelLabel_owned_load_result_n2255, .hxc_loadingFramePresented = hxc_l_tmp_load_result_n2253, .hxc_lookX = hxc_l_tmp_load_result_n2229, .hxc_lookY = hxc_l_tmp_load_result_n2230, .hxc_lookZ = hxc_l_tmp_load_result_n2231, .hxc_motionHistory = hxc_l_tmp_load_result_n2233, .hxc_pendingCampaignLabel = hxc_l_tmp_record_field_pendingCampaignLabel_owned_load_result_n2252, .hxc_pendingCampaignLevel = hxc_l_tmp_load_result_n2249, .hxc_pendingCampaignPortal = hxc_l_tmp_load_result_n2250, .hxc_pickupAmount = hxc_l_tmp_load_result_n2241, .hxc_pickupFrames = hxc_l_tmp_load_result_n2240, .hxc_placementBlockedFrames = hxc_l_tmp_load_result_n2236, .hxc_preparedPortal = hxc_l_preparedPortal, .hxc_recoveryFeedback = hxc_l_tmp_load_result_n2244, .hxc_recoveryFeedbackFrames = hxc_l_tmp_load_result_n2245, .hxc_selectedCampaignLevelIndex = hxc_l_tmp_load_result_n2256, .hxc_strikeHitFrames = hxc_l_tmp_load_result_n2237, .hxc_swordCombat = hxc_l_tmp_load_result_n2225, .hxc_swordQueued = hxc_l_tmp_load_result_n2235 }.hxc_preparedPortal;
                          hxc_l_snapshot = (struct hxc_caxecraft_app_CaxecraftApp_EditorTestPlayShellSnapshot){ .hxc_accumulator = hxc_l_tmp_load_result_n2232, .hxc_activeDialogue = hxc_l_tmp_record_field_activeDialogue_owned_load_result_n2217, .hxc_aquaticEquipmentCode = hxc_l_tmp_load_result_n2246, .hxc_aquaticEquipmentFrames = hxc_l_tmp_load_result_n2247, .hxc_availableInteractionPrompt = hxc_l_tmp_load_result_n2222, .hxc_berryDrop = hxc_l_tmp_load_result_n2226, .hxc_cameraMode = hxc_l_tmp_load_result_n2228, .hxc_cameraWaterBlend = hxc_l_tmp_load_result_n2227, .hxc_campaignLevel = hxc_l_tmp_load_result_n2248, .hxc_conversation = hxc_l_tmp_load_result_n2215, .hxc_currentObjectiveId = hxc_l_tmp_record_field_currentObjectiveId_owned_load_result_n2221, .hxc_enemyActor = hxc_l_tmp_load_result_n2223, .hxc_enemyAttackFrames = hxc_l_tmp_load_result_n2239, .hxc_enemyDefeatedFrames = hxc_l_tmp_load_result_n2238, .hxc_enemyPhase = hxc_l_tmp_load_result_n2224, .hxc_inventory = hxc_l_tmp_load_result_n2214, .hxc_inventoryFullFrames = hxc_l_tmp_load_result_n2242, .hxc_inventoryFullReason = hxc_l_tmp_load_result_n2243, .hxc_jumpQueued = hxc_l_tmp_load_result_n2234, .hxc_latestJournalId = hxc_l_tmp_record_field_latestJournalId_owned_load_result_n2219, .hxc_levelLabel = hxc_l_tmp_record_field_levelLabel_owned_load_result_n2255, .hxc_loadingFramePresented = hxc_l_tmp_load_result_n2253, .hxc_lookX = hxc_l_tmp_load_result_n2229, .hxc_lookY = hxc_l_tmp_load_result_n2230, .hxc_lookZ = hxc_l_tmp_load_result_n2231, .hxc_motionHistory = hxc_l_tmp_load_result_n2233, .hxc_pendingCampaignLabel = hxc_l_tmp_record_field_pendingCampaignLabel_owned_load_result_n2252, .hxc_pendingCampaignLevel = hxc_l_tmp_load_result_n2249, .hxc_pendingCampaignPortal = hxc_l_tmp_load_result_n2250, .hxc_pickupAmount = hxc_l_tmp_load_result_n2241, .hxc_pickupFrames = hxc_l_tmp_load_result_n2240, .hxc_placementBlockedFrames = hxc_l_tmp_load_result_n2236, .hxc_preparedPortal = hxc_l_preparedPortal, .hxc_recoveryFeedback = hxc_l_tmp_load_result_n2244, .hxc_recoveryFeedbackFrames = hxc_l_tmp_load_result_n2245, .hxc_selectedCampaignLevelIndex = hxc_l_tmp_load_result_n2256, .hxc_strikeHitFrames = hxc_l_tmp_load_result_n2237, .hxc_swordCombat = hxc_l_tmp_load_result_n2225, .hxc_swordQueued = hxc_l_tmp_load_result_n2235 };
                          struct hxc_caxecraft_app_EditorTestPlayRuntime *hxc_l_tmp_load_result_n2259 = hxc_l_proposedRuntime;
                          hxc_l_gc_roots[709] = (const void *)hxc_l_tmp_load_result_n2259;
                          hxc_l_gc_roots[710] = (const void *)hxc_l_snapshot.hxc_campaignLevel;
                          hxc_l_gc_roots[711] = (const void *)hxc_l_snapshot.hxc_pendingCampaignLevel;
                          hxc_l_gc_roots[712] = (const void *)hxc_l_snapshot.hxc_pendingCampaignPortal;
                          hxc_l_gc_roots[713] = (const void *)hxc_l_snapshot.hxc_preparedPortal;
                          struct hxc_caxecraft_app_CaxecraftApp_EditorTestPlayShellSnapshot hxc_l_tmp_record_field_snapshot_owner_n618 = hxc_l_snapshot;
                          if (hxc_record_3b3e16aa_retain(&hxc_l_tmp_record_field_snapshot_owner_n618) != HXC_STATUS_OK)
                          {
                            abort();
                          }
                          struct hxc_caxecraft_app_CaxecraftApp_EditorTestPlayShellSnapshot hxc_l_tmp_record_field_snapshot_owned_load_result_n2261 = hxc_l_tmp_record_field_snapshot_owner_n618;
                          hxc_l_gc_roots[714] = (const void *)hxc_l_tmp_record_field_snapshot_owned_load_result_n2261.hxc_campaignLevel;
                          hxc_l_gc_roots[715] = (const void *)hxc_l_tmp_record_field_snapshot_owned_load_result_n2261.hxc_pendingCampaignLevel;
                          hxc_l_gc_roots[716] = (const void *)hxc_l_tmp_record_field_snapshot_owned_load_result_n2261.hxc_pendingCampaignPortal;
                          hxc_l_gc_roots[717] = (const void *)hxc_l_tmp_record_field_snapshot_owned_load_result_n2261.hxc_preparedPortal;
                          struct hxc_caxecraft_app_ActivePlayableLevel *hxc_l_tmp_load_result_n2262 = hxc_l_testOwner;
                          hxc_l_gc_roots[718] = (const void *)hxc_l_tmp_load_result_n2262;
                          if (hxc_l_tmp_load_result_n2262 == NULL)
                          {
                            abort();
                          }
                          struct hxc_caxecraft_content_ActiveContent *hxc_l_tmp_class_field_load_result_n2263 = (*hxc_l_tmp_load_result_n2262).hxc_content;
                          hxc_l_gc_roots[719] = (const void *)hxc_l_tmp_class_field_load_result_n2263;
                          if (hxc_l_tmp_class_field_load_result_n2263 == NULL)
                          {
                            abort();
                          }
                          struct hxc_caxecraft_content_LoadedContentGeneration *hxc_l_tmp_class_field_load_result_n2264 = (*hxc_l_tmp_class_field_load_result_n2263).hxc_active;
                          hxc_l_gc_roots[720] = (const void *)hxc_l_tmp_class_field_load_result_n2264;
                          if (hxc_l_tmp_class_field_load_result_n2264 == NULL)
                          {
                            abort();
                          }
                          int32_t hxc_l_tmp_class_field_load_result_n2265 = (*hxc_l_tmp_class_field_load_result_n2264).hxc_generationIdValue;
                          hxc_l_gc_roots[721] = (const void *)(struct hxc_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_generationId = hxc_l_tmp_class_field_load_result_n2265, .hxc_runtime = hxc_l_tmp_load_result_n2259, .hxc_snapshot = hxc_l_tmp_record_field_snapshot_owned_load_result_n2261 }.hxc_runtime;
                          hxc_l_gc_roots[722] = (const void *)(struct hxc_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_generationId = hxc_l_tmp_class_field_load_result_n2265, .hxc_runtime = hxc_l_tmp_load_result_n2259, .hxc_snapshot = hxc_l_tmp_record_field_snapshot_owned_load_result_n2261 }.hxc_snapshot.hxc_campaignLevel;
                          hxc_l_gc_roots[723] = (const void *)(struct hxc_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_generationId = hxc_l_tmp_class_field_load_result_n2265, .hxc_runtime = hxc_l_tmp_load_result_n2259, .hxc_snapshot = hxc_l_tmp_record_field_snapshot_owned_load_result_n2261 }.hxc_snapshot.hxc_pendingCampaignLevel;
                          hxc_l_gc_roots[724] = (const void *)(struct hxc_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_generationId = hxc_l_tmp_class_field_load_result_n2265, .hxc_runtime = hxc_l_tmp_load_result_n2259, .hxc_snapshot = hxc_l_tmp_record_field_snapshot_owned_load_result_n2261 }.hxc_snapshot.hxc_pendingCampaignPortal;
                          hxc_l_gc_roots[725] = (const void *)(struct hxc_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_generationId = hxc_l_tmp_class_field_load_result_n2265, .hxc_runtime = hxc_l_tmp_load_result_n2259, .hxc_snapshot = hxc_l_tmp_record_field_snapshot_owned_load_result_n2261 }.hxc_snapshot.hxc_preparedPortal;
                          hxc_l_gc_roots[726] = (struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = true, .hxc_value = (struct hxc_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_generationId = hxc_l_tmp_class_field_load_result_n2265, .hxc_runtime = hxc_l_tmp_load_result_n2259, .hxc_snapshot = hxc_l_tmp_record_field_snapshot_owned_load_result_n2261 } }.hxc_has_value ? (const void *)(struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = true, .hxc_value = (struct hxc_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_generationId = hxc_l_tmp_class_field_load_result_n2265, .hxc_runtime = hxc_l_tmp_load_result_n2259, .hxc_snapshot = hxc_l_tmp_record_field_snapshot_owned_load_result_n2261 } }.hxc_value.hxc_runtime : NULL;
                          hxc_l_gc_roots[727] = (struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = true, .hxc_value = (struct hxc_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_generationId = hxc_l_tmp_class_field_load_result_n2265, .hxc_runtime = hxc_l_tmp_load_result_n2259, .hxc_snapshot = hxc_l_tmp_record_field_snapshot_owned_load_result_n2261 } }.hxc_has_value ? (const void *)(struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = true, .hxc_value = (struct hxc_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_generationId = hxc_l_tmp_class_field_load_result_n2265, .hxc_runtime = hxc_l_tmp_load_result_n2259, .hxc_snapshot = hxc_l_tmp_record_field_snapshot_owned_load_result_n2261 } }.hxc_value.hxc_snapshot.hxc_campaignLevel : NULL;
                          hxc_l_gc_roots[728] = (struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = true, .hxc_value = (struct hxc_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_generationId = hxc_l_tmp_class_field_load_result_n2265, .hxc_runtime = hxc_l_tmp_load_result_n2259, .hxc_snapshot = hxc_l_tmp_record_field_snapshot_owned_load_result_n2261 } }.hxc_has_value ? (const void *)(struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = true, .hxc_value = (struct hxc_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_generationId = hxc_l_tmp_class_field_load_result_n2265, .hxc_runtime = hxc_l_tmp_load_result_n2259, .hxc_snapshot = hxc_l_tmp_record_field_snapshot_owned_load_result_n2261 } }.hxc_value.hxc_snapshot.hxc_pendingCampaignLevel : NULL;
                          hxc_l_gc_roots[729] = (struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = true, .hxc_value = (struct hxc_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_generationId = hxc_l_tmp_class_field_load_result_n2265, .hxc_runtime = hxc_l_tmp_load_result_n2259, .hxc_snapshot = hxc_l_tmp_record_field_snapshot_owned_load_result_n2261 } }.hxc_has_value ? (const void *)(struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = true, .hxc_value = (struct hxc_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_generationId = hxc_l_tmp_class_field_load_result_n2265, .hxc_runtime = hxc_l_tmp_load_result_n2259, .hxc_snapshot = hxc_l_tmp_record_field_snapshot_owned_load_result_n2261 } }.hxc_value.hxc_snapshot.hxc_pendingCampaignPortal : NULL;
                          hxc_l_gc_roots[730] = (struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = true, .hxc_value = (struct hxc_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_generationId = hxc_l_tmp_class_field_load_result_n2265, .hxc_runtime = hxc_l_tmp_load_result_n2259, .hxc_snapshot = hxc_l_tmp_record_field_snapshot_owned_load_result_n2261 } }.hxc_has_value ? (const void *)(struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = true, .hxc_value = (struct hxc_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_generationId = hxc_l_tmp_class_field_load_result_n2265, .hxc_runtime = hxc_l_tmp_load_result_n2259, .hxc_snapshot = hxc_l_tmp_record_field_snapshot_owned_load_result_n2261 } }.hxc_value.hxc_snapshot.hxc_preparedPortal : NULL;
                          hxc_optional_9d9d283f_destroy(&hxc_l_activeEditorTestPlay);
                          hxc_l_activeEditorTestPlay = (struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = true, .hxc_value = (struct hxc_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_generationId = hxc_l_tmp_class_field_load_result_n2265, .hxc_runtime = hxc_l_tmp_load_result_n2259, .hxc_snapshot = hxc_l_tmp_record_field_snapshot_owned_load_result_n2261 } };
                          struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n2268 = hxc_caxecraft_gameplay_Inventory_starter();
                          hxc_l_inventory = hxc_l_tmp_call_result_n2268;
                          hxc_l_conversation = (struct hxc_optional_caxecraft_app_ConversationState){ .hxc_has_value = false };
                          hxc_string hxc_l_tmp_string_assignment_replacement_owner_n619 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
                          if (hxc_string_retain(hxc_l_tmp_string_assignment_replacement_owner_n619) != HXC_STATUS_OK)
                          {
                            abort();
                          }
                          hxc_string hxc_l_tmp_string_assignment_replacement_owned_load_result_n2269 = hxc_l_tmp_string_assignment_replacement_owner_n619;
                          if (hxc_string_release(&hxc_l_activeDialogue) != HXC_STATUS_OK)
                          {
                            abort();
                          }
                          hxc_l_activeDialogue = hxc_l_tmp_string_assignment_replacement_owned_load_result_n2269;
                          hxc_string hxc_l_tmp_string_assignment_replacement_owner_n620 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
                          if (hxc_string_retain(hxc_l_tmp_string_assignment_replacement_owner_n620) != HXC_STATUS_OK)
                          {
                            abort();
                          }
                          hxc_string hxc_l_tmp_string_assignment_replacement_owned_load_result_n2270 = hxc_l_tmp_string_assignment_replacement_owner_n620;
                          if (hxc_string_release(&hxc_l_latestJournalId) != HXC_STATUS_OK)
                          {
                            abort();
                          }
                          hxc_l_latestJournalId = hxc_l_tmp_string_assignment_replacement_owned_load_result_n2270;
                          struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n2271 = hxc_l_testLevel_hc67e923a7029;
                          hxc_l_gc_roots[731] = (const void *)hxc_l_tmp_load_result_n2271;
                          if (hxc_l_tmp_load_result_n2271 == NULL)
                          {
                            abort();
                          }
                          struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_class_field_load_result_n2272 = (*hxc_l_tmp_load_result_n2271).hxc_levelPresentation;
                          hxc_l_gc_roots[732] = (const void *)hxc_l_tmp_class_field_load_result_n2272;
                          if (hxc_l_tmp_class_field_load_result_n2272 == NULL)
                          {
                            abort();
                          }
                          hxc_string hxc_l_tmp_class_field_load_result_n2273 = (*hxc_l_tmp_class_field_load_result_n2272).hxc_startingObjective;
                          hxc_string hxc_l_tmp_string_assignment_replacement_owner_n621 = hxc_l_tmp_class_field_load_result_n2273;
                          if (hxc_string_retain(hxc_l_tmp_string_assignment_replacement_owner_n621) != HXC_STATUS_OK)
                          {
                            abort();
                          }
                          hxc_string hxc_l_tmp_string_assignment_replacement_owned_load_result_n2274 = hxc_l_tmp_string_assignment_replacement_owner_n621;
                          if (hxc_string_release(&hxc_l_currentObjectiveId) != HXC_STATUS_OK)
                          {
                            abort();
                          }
                          hxc_l_currentObjectiveId = hxc_l_tmp_string_assignment_replacement_owned_load_result_n2274;
                          struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n2275 = hxc_l_testSession;
                          hxc_l_gc_roots[733] = (const void *)hxc_l_tmp_load_result_n2275;
                          hxc_l_gc_roots[734] = (const void *)hxc_l_testLevel_hc67e923a7029;
                          struct hxc_caxecraft_app_CaxecraftApp_AvailableInteractionTarget hxc_l_tmp_call_result_n2277 = hxc_caxecraft_app_CaxecraftApp_nearestAvailableInteraction(hxc_l_tmp_load_result_n2275, hxc_l_testLevel_hc67e923a7029);
                          hxc_l_tmp_static_call_argument_0_owner_n622 = hxc_l_tmp_call_result_n2277;
                          enum hxc_caxecraft_app_InteractionPrompt hxc_l_tmp_call_result_n2279 = hxc_caxecraft_app_CaxecraftApp_promptForAvailableInteraction(hxc_l_tmp_static_call_argument_0_owner_n622);
                          hxc_l_availableInteractionPrompt = hxc_l_tmp_call_result_n2279;
                          hxc_l_enemyActor = hxc_l_testEnemy;
                          hxc_l_enemyPhase_ha225e19c84ac = hxc_l_testEnemyPhase;
                          hxc_l_swordCombat = (struct hxc_caxecraft_gameplay_SwordCombatState){ .hxc_cooldownTicks = 0 };
                          hxc_l_berryDrop = (struct hxc_caxecraft_gameplay_BerryDropState){ .hxc_active = false, .hxc_amount = 0, .hxc_x = 0.0, .hxc_y = 0.0, .hxc_z = 0.0 };
                          hxc_l_cameraWaterBlend = 0.0;
                          hxc_l_cameraMode = hxc_caxecraft_domain_PlayerCameraMode_FirstPerson;
                          struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n2285 = hxc_l_testLevel_hc67e923a7029;
                          hxc_l_gc_roots[735] = (const void *)hxc_l_tmp_load_result_n2285;
                          if (hxc_l_tmp_load_result_n2285 == NULL)
                          {
                            abort();
                          }
                          struct hxc_caxecraft_scenario_ScenarioTransform hxc_l_tmp_class_field_load_result_n2286 = (*hxc_l_tmp_load_result_n2285).hxc_playerSpawn;
                          struct hxc_caxecraft_app_RenderPosition hxc_l_tmp_call_result_n2287 = hxc_caxecraft_app_SpawnCameraHeading_headingForSpawn(hxc_l_tmp_class_field_load_result_n2286);
                          struct hxc_caxecraft_app_RenderPosition hxc_l_testHeading = hxc_l_tmp_call_result_n2287;
                          hxc_l_lookX = hxc_l_testHeading.hxc_x;
                          hxc_l_lookY = hxc_l_testHeading.hxc_y;
                          hxc_l_lookZ = hxc_l_testHeading.hxc_z;
                          hxc_l_accumulator = 0.0;
                          struct hxc_caxecraft_app_MotionHistory hxc_l_tmp_call_result_n2292 = hxc_caxecraft_app_MotionInterpolation_start(hxc_l_testView.hxc_localPlayer.hxc_body);
                          hxc_l_motionHistory = hxc_l_tmp_call_result_n2292;
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
                          struct hxc_caxecraft_app_TerrainRenderer *hxc_l_tmp_owned_class_field_address_n2293 = &(*hxc_l_self).hxc_terrainRenderer;
                          struct hxc_caxecraft_app_TerrainChunkCache *hxc_l_tmp_owned_class_field_address_n2294 = &(*hxc_l_tmp_owned_class_field_address_n2293).hxc_cache;
                          if (hxc_l_tmp_owned_class_field_address_n2294 == NULL)
                          {
                            abort();
                          }
                          hxc_caxecraft_app_TerrainChunkCache_invalidateAll(hxc_l_tmp_owned_class_field_address_n2294);
                          struct hxc_caxecraft_input_NavigationRepeater *hxc_l_tmp_owned_class_field_address_n2295 = &(*hxc_l_self).hxc_editorNavigation;
                          if (hxc_l_tmp_owned_class_field_address_n2295 == NULL)
                          {
                            abort();
                          }
                          hxc_caxecraft_input_NavigationRepeater_release(hxc_l_tmp_owned_class_field_address_n2295);
                          enum hxc_caxecraft_app_AppScreen hxc_l_tmp_call_result_n2297 = hxc_caxecraft_app_AppScreen_beginEditorTestPlay(hxc_l_screen);
                          hxc_l_screen = hxc_l_tmp_call_result_n2297;
                          DisableCursor();
                          hxc_enum_2c0e695d_destroy(&hxc_l_tmp_static_call_argument_0_owner_n622);
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
                    struct hxc_caxecraft_app_EditorTestPlayRuntime *hxc_l_tmp_load_result_n2298 = hxc_l_proposedRuntime;
                    hxc_l_gc_roots[680] = (const void *)hxc_l_tmp_load_result_n2298;
                    if (hxc_l_tmp_load_result_n2298 == NULL)
                    {
                      abort();
                    }
                    hxc_caxecraft_app_EditorTestPlayRuntime_stop(hxc_l_tmp_load_result_n2298);
                    struct hxc_caxecraft_app_CaxecraftEditorScreen *hxc_l_tmp_load_result_n2299 = hxc_l_editorScreen;
                    hxc_l_gc_roots[681] = (const void *)hxc_l_tmp_load_result_n2299;
                    if (hxc_l_tmp_load_result_n2299 == NULL)
                    {
                      abort();
                    }
                    hxc_caxecraft_app_CaxecraftEditorScreen_finishTestPlay(hxc_l_tmp_load_result_n2299, false);
                    break;
                  }
                default:
                  {
                    abort();
                  }
              }
              hxc_enum_447e0e6e_destroy(&hxc_l_tmp_enum_switch_subject_owner_n601);
              hxc_record_79135587_destroy(&hxc_l_tmp_instance_call_argument_0_owner_n600);
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
        hxc_enum_2c0e695d_destroy(&hxc_l_tmp_static_call_argument_0_owner_n428);
        hxc_enum_59188943_destroy(&hxc_l_editorActionForFrame);
        continue;
      }
      hxc_l_gc_roots[270] = hxc_l_activeEditorTestPlay.hxc_has_value ? (const void *)hxc_l_activeEditorTestPlay.hxc_value.hxc_runtime : NULL;
      hxc_l_gc_roots[271] = hxc_l_activeEditorTestPlay.hxc_has_value ? (const void *)hxc_l_activeEditorTestPlay.hxc_value.hxc_snapshot.hxc_campaignLevel : NULL;
      hxc_l_gc_roots[272] = hxc_l_activeEditorTestPlay.hxc_has_value ? (const void *)hxc_l_activeEditorTestPlay.hxc_value.hxc_snapshot.hxc_pendingCampaignLevel : NULL;
      hxc_l_gc_roots[273] = hxc_l_activeEditorTestPlay.hxc_has_value ? (const void *)hxc_l_activeEditorTestPlay.hxc_value.hxc_snapshot.hxc_pendingCampaignPortal : NULL;
      hxc_l_gc_roots[274] = hxc_l_activeEditorTestPlay.hxc_has_value ? (const void *)hxc_l_activeEditorTestPlay.hxc_value.hxc_snapshot.hxc_preparedPortal : NULL;
      hxc_l_run_h3bea1e3d2ec8 = hxc_l_activeEditorTestPlay;
      if (hxc_optional_9d9d283f_retain(&hxc_l_run_h3bea1e3d2ec8) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_l_gc_roots[275] = hxc_l_run_h3bea1e3d2ec8.hxc_has_value ? (const void *)hxc_l_run_h3bea1e3d2ec8.hxc_value.hxc_runtime : NULL;
      hxc_l_gc_roots[276] = hxc_l_run_h3bea1e3d2ec8.hxc_has_value ? (const void *)hxc_l_run_h3bea1e3d2ec8.hxc_value.hxc_snapshot.hxc_campaignLevel : NULL;
      hxc_l_gc_roots[277] = hxc_l_run_h3bea1e3d2ec8.hxc_has_value ? (const void *)hxc_l_run_h3bea1e3d2ec8.hxc_value.hxc_snapshot.hxc_pendingCampaignLevel : NULL;
      hxc_l_gc_roots[278] = hxc_l_run_h3bea1e3d2ec8.hxc_has_value ? (const void *)hxc_l_run_h3bea1e3d2ec8.hxc_value.hxc_snapshot.hxc_pendingCampaignPortal : NULL;
      hxc_l_gc_roots[279] = hxc_l_run_h3bea1e3d2ec8.hxc_has_value ? (const void *)hxc_l_run_h3bea1e3d2ec8.hxc_value.hxc_snapshot.hxc_preparedPortal : NULL;
      if (!hxc_l_run_h3bea1e3d2ec8.hxc_has_value)
      {
        if (hxc_io_println((hxc_string){ (const uint8_t *)"caxecraft: editor Test Play stop had no active run", 50, true, NULL }) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_l_quit = true;
      }
      else
      {
        struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun hxc_l_tmp_load_result_n373 = hxc_l_run_h3bea1e3d2ec8;
        hxc_l_gc_roots[282] = hxc_l_tmp_load_result_n373.hxc_has_value ? (const void *)hxc_l_tmp_load_result_n373.hxc_value.hxc_runtime : NULL;
        hxc_l_gc_roots[283] = hxc_l_tmp_load_result_n373.hxc_has_value ? (const void *)hxc_l_tmp_load_result_n373.hxc_value.hxc_snapshot.hxc_campaignLevel : NULL;
        hxc_l_gc_roots[284] = hxc_l_tmp_load_result_n373.hxc_has_value ? (const void *)hxc_l_tmp_load_result_n373.hxc_value.hxc_snapshot.hxc_pendingCampaignLevel : NULL;
        hxc_l_gc_roots[285] = hxc_l_tmp_load_result_n373.hxc_has_value ? (const void *)hxc_l_tmp_load_result_n373.hxc_value.hxc_snapshot.hxc_pendingCampaignPortal : NULL;
        hxc_l_gc_roots[286] = hxc_l_tmp_load_result_n373.hxc_has_value ? (const void *)hxc_l_tmp_load_result_n373.hxc_value.hxc_snapshot.hxc_preparedPortal : NULL;
        if (!hxc_l_tmp_load_result_n373.hxc_has_value)
        {
          abort();
        }
        hxc_l_gc_roots[287] = (const void *)hxc_l_tmp_load_result_n373.hxc_value.hxc_runtime;
        hxc_l_gc_roots[288] = (const void *)hxc_l_tmp_load_result_n373.hxc_value.hxc_snapshot.hxc_campaignLevel;
        hxc_l_gc_roots[289] = (const void *)hxc_l_tmp_load_result_n373.hxc_value.hxc_snapshot.hxc_pendingCampaignLevel;
        hxc_l_gc_roots[290] = (const void *)hxc_l_tmp_load_result_n373.hxc_value.hxc_snapshot.hxc_pendingCampaignPortal;
        hxc_l_gc_roots[291] = (const void *)hxc_l_tmp_load_result_n373.hxc_value.hxc_snapshot.hxc_preparedPortal;
        struct hxc_caxecraft_app_EditorTestPlayRuntime *hxc_l_tmp_optional_record_field_project_n375 = hxc_l_tmp_load_result_n373.hxc_value.hxc_runtime;
        hxc_l_gc_roots[292] = (const void *)hxc_l_tmp_optional_record_field_project_n375;
        if (hxc_l_tmp_optional_record_field_project_n375 == NULL)
        {
          abort();
        }
        hxc_caxecraft_app_EditorTestPlayRuntime_stop(hxc_l_tmp_optional_record_field_project_n375);
        hxc_l_gc_roots[293] = (struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = false }.hxc_has_value ? (const void *)(struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = false }.hxc_value.hxc_runtime : NULL;
        hxc_l_gc_roots[294] = (struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = false }.hxc_has_value ? (const void *)(struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = false }.hxc_value.hxc_snapshot.hxc_campaignLevel : NULL;
        hxc_l_gc_roots[295] = (struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = false }.hxc_has_value ? (const void *)(struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = false }.hxc_value.hxc_snapshot.hxc_pendingCampaignLevel : NULL;
        hxc_l_gc_roots[296] = (struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = false }.hxc_has_value ? (const void *)(struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = false }.hxc_value.hxc_snapshot.hxc_pendingCampaignPortal : NULL;
        hxc_l_gc_roots[297] = (struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = false }.hxc_has_value ? (const void *)(struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = false }.hxc_value.hxc_snapshot.hxc_preparedPortal : NULL;
        hxc_optional_9d9d283f_destroy(&hxc_l_activeEditorTestPlay);
        hxc_l_activeEditorTestPlay = (struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = false };
        struct hxc_caxecraft_app_CaxecraftEditorScreen *hxc_l_tmp_load_result_n376 = hxc_l_editorScreen;
        hxc_l_gc_roots[298] = (const void *)hxc_l_tmp_load_result_n376;
        if (hxc_l_tmp_load_result_n376 == NULL)
        {
          abort();
        }
        hxc_caxecraft_app_CaxecraftEditorScreen_finishTestPlay(hxc_l_tmp_load_result_n376, true);
        enum hxc_caxecraft_app_AppScreen hxc_l_tmp_call_result_n378 = hxc_caxecraft_app_AppScreen_finishEditorTestPlay(hxc_l_screen);
        hxc_l_screen = hxc_l_tmp_call_result_n378;
        struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun hxc_l_tmp_load_result_n379 = hxc_l_run_h3bea1e3d2ec8;
        hxc_l_gc_roots[299] = hxc_l_tmp_load_result_n379.hxc_has_value ? (const void *)hxc_l_tmp_load_result_n379.hxc_value.hxc_runtime : NULL;
        hxc_l_gc_roots[300] = hxc_l_tmp_load_result_n379.hxc_has_value ? (const void *)hxc_l_tmp_load_result_n379.hxc_value.hxc_snapshot.hxc_campaignLevel : NULL;
        hxc_l_gc_roots[301] = hxc_l_tmp_load_result_n379.hxc_has_value ? (const void *)hxc_l_tmp_load_result_n379.hxc_value.hxc_snapshot.hxc_pendingCampaignLevel : NULL;
        hxc_l_gc_roots[302] = hxc_l_tmp_load_result_n379.hxc_has_value ? (const void *)hxc_l_tmp_load_result_n379.hxc_value.hxc_snapshot.hxc_pendingCampaignPortal : NULL;
        hxc_l_gc_roots[303] = hxc_l_tmp_load_result_n379.hxc_has_value ? (const void *)hxc_l_tmp_load_result_n379.hxc_value.hxc_snapshot.hxc_preparedPortal : NULL;
        if (!hxc_l_tmp_load_result_n379.hxc_has_value)
        {
          abort();
        }
        hxc_l_gc_roots[304] = (const void *)hxc_l_tmp_load_result_n379.hxc_value.hxc_runtime;
        hxc_l_gc_roots[305] = (const void *)hxc_l_tmp_load_result_n379.hxc_value.hxc_snapshot.hxc_campaignLevel;
        hxc_l_gc_roots[306] = (const void *)hxc_l_tmp_load_result_n379.hxc_value.hxc_snapshot.hxc_pendingCampaignLevel;
        hxc_l_gc_roots[307] = (const void *)hxc_l_tmp_load_result_n379.hxc_value.hxc_snapshot.hxc_pendingCampaignPortal;
        hxc_l_gc_roots[308] = (const void *)hxc_l_tmp_load_result_n379.hxc_value.hxc_snapshot.hxc_preparedPortal;
        hxc_l_gc_roots[309] = (const void *)hxc_l_tmp_load_result_n379.hxc_value.hxc_snapshot.hxc_campaignLevel;
        hxc_l_gc_roots[310] = (const void *)hxc_l_tmp_load_result_n379.hxc_value.hxc_snapshot.hxc_pendingCampaignLevel;
        hxc_l_gc_roots[311] = (const void *)hxc_l_tmp_load_result_n379.hxc_value.hxc_snapshot.hxc_pendingCampaignPortal;
        hxc_l_gc_roots[312] = (const void *)hxc_l_tmp_load_result_n379.hxc_value.hxc_snapshot.hxc_preparedPortal;
        hxc_l_saved = hxc_l_tmp_load_result_n379.hxc_value.hxc_snapshot;
        if (hxc_record_3b3e16aa_retain(&hxc_l_saved) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_l_inventory = hxc_l_saved.hxc_inventory;
        hxc_l_conversation = hxc_l_saved.hxc_conversation;
        hxc_string hxc_l_tmp_string_assignment_replacement_owner_n176 = hxc_l_saved.hxc_activeDialogue;
        if (hxc_string_retain(hxc_l_tmp_string_assignment_replacement_owner_n176) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_string hxc_l_tmp_string_assignment_replacement_owned_load_result_n385 = hxc_l_tmp_string_assignment_replacement_owner_n176;
        if (hxc_string_release(&hxc_l_activeDialogue) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_l_activeDialogue = hxc_l_tmp_string_assignment_replacement_owned_load_result_n385;
        hxc_string hxc_l_tmp_string_assignment_replacement_owner_n177 = hxc_l_saved.hxc_latestJournalId;
        if (hxc_string_retain(hxc_l_tmp_string_assignment_replacement_owner_n177) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_string hxc_l_tmp_string_assignment_replacement_owned_load_result_n387 = hxc_l_tmp_string_assignment_replacement_owner_n177;
        if (hxc_string_release(&hxc_l_latestJournalId) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_l_latestJournalId = hxc_l_tmp_string_assignment_replacement_owned_load_result_n387;
        hxc_string hxc_l_tmp_string_assignment_replacement_owner_n178 = hxc_l_saved.hxc_currentObjectiveId;
        if (hxc_string_retain(hxc_l_tmp_string_assignment_replacement_owner_n178) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_string hxc_l_tmp_string_assignment_replacement_owned_load_result_n389 = hxc_l_tmp_string_assignment_replacement_owner_n178;
        if (hxc_string_release(&hxc_l_currentObjectiveId) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_l_currentObjectiveId = hxc_l_tmp_string_assignment_replacement_owned_load_result_n389;
        hxc_l_availableInteractionPrompt = hxc_l_saved.hxc_availableInteractionPrompt;
        hxc_l_enemyActor = hxc_l_saved.hxc_enemyActor;
        hxc_l_enemyPhase_ha225e19c84ac = hxc_l_saved.hxc_enemyPhase;
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
        hxc_string hxc_l_tmp_string_assignment_replacement_owner_n179 = hxc_l_saved.hxc_pendingCampaignLabel;
        if (hxc_string_retain(hxc_l_tmp_string_assignment_replacement_owner_n179) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_string hxc_l_tmp_string_assignment_replacement_owned_load_result_n420 = hxc_l_tmp_string_assignment_replacement_owner_n179;
        if (hxc_string_release(&hxc_l_pendingCampaignLabel) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_l_pendingCampaignLabel = hxc_l_tmp_string_assignment_replacement_owned_load_result_n420;
        hxc_l_loadingFramePresented = hxc_l_saved.hxc_loadingFramePresented;
        hxc_string hxc_l_tmp_string_assignment_replacement_owner_n180 = hxc_l_saved.hxc_levelLabel;
        if (hxc_string_retain(hxc_l_tmp_string_assignment_replacement_owner_n180) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_string hxc_l_tmp_string_assignment_replacement_owned_load_result_n423 = hxc_l_tmp_string_assignment_replacement_owner_n180;
        if (hxc_string_release(&hxc_l_levelLabel) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_l_levelLabel = hxc_l_tmp_string_assignment_replacement_owned_load_result_n423;
        hxc_l_selectedCampaignLevelIndex = hxc_l_saved.hxc_selectedCampaignLevelIndex;
        hxc_l_gc_roots[316] = (const void *)hxc_l_saved.hxc_preparedPortal;
        hxc_l_preparedPortal = hxc_l_saved.hxc_preparedPortal;
        if (hxc_l_self == NULL)
        {
          abort();
        }
        struct hxc_caxecraft_app_TerrainRenderer *hxc_l_tmp_owned_class_field_address_n426 = &(*hxc_l_self).hxc_terrainRenderer;
        struct hxc_caxecraft_app_TerrainChunkCache *hxc_l_tmp_owned_class_field_address_n427 = &(*hxc_l_tmp_owned_class_field_address_n426).hxc_cache;
        if (hxc_l_tmp_owned_class_field_address_n427 == NULL)
        {
          abort();
        }
        hxc_caxecraft_app_TerrainChunkCache_invalidateAll(hxc_l_tmp_owned_class_field_address_n427);
        struct hxc_caxecraft_input_NavigationRepeater *hxc_l_tmp_owned_class_field_address_n428 = &(*hxc_l_self).hxc_editorNavigation;
        if (hxc_l_tmp_owned_class_field_address_n428 == NULL)
        {
          abort();
        }
        hxc_caxecraft_input_NavigationRepeater_release(hxc_l_tmp_owned_class_field_address_n428);
        EnableCursor();
        hxc_record_3b3e16aa_destroy(&hxc_l_saved);
      }
      hxc_l_frameCount = hxc_i32_add_wrapping(hxc_l_frameCount, 1);
      hxc_optional_9d9d283f_destroy(&hxc_l_run_h3bea1e3d2ec8);
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
      struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun hxc_l_tmp_load_result_n2304 = hxc_l_unfinishedEditorTestPlay;
      hxc_l_gc_roots[736] = hxc_l_tmp_load_result_n2304.hxc_has_value ? (const void *)hxc_l_tmp_load_result_n2304.hxc_value.hxc_runtime : NULL;
      hxc_l_gc_roots[737] = hxc_l_tmp_load_result_n2304.hxc_has_value ? (const void *)hxc_l_tmp_load_result_n2304.hxc_value.hxc_snapshot.hxc_campaignLevel : NULL;
      hxc_l_gc_roots[738] = hxc_l_tmp_load_result_n2304.hxc_has_value ? (const void *)hxc_l_tmp_load_result_n2304.hxc_value.hxc_snapshot.hxc_pendingCampaignLevel : NULL;
      hxc_l_gc_roots[739] = hxc_l_tmp_load_result_n2304.hxc_has_value ? (const void *)hxc_l_tmp_load_result_n2304.hxc_value.hxc_snapshot.hxc_pendingCampaignPortal : NULL;
      hxc_l_gc_roots[740] = hxc_l_tmp_load_result_n2304.hxc_has_value ? (const void *)hxc_l_tmp_load_result_n2304.hxc_value.hxc_snapshot.hxc_preparedPortal : NULL;
      if (!hxc_l_tmp_load_result_n2304.hxc_has_value)
      {
        abort();
      }
      hxc_l_gc_roots[741] = (const void *)hxc_l_tmp_load_result_n2304.hxc_value.hxc_runtime;
      hxc_l_gc_roots[742] = (const void *)hxc_l_tmp_load_result_n2304.hxc_value.hxc_snapshot.hxc_campaignLevel;
      hxc_l_gc_roots[743] = (const void *)hxc_l_tmp_load_result_n2304.hxc_value.hxc_snapshot.hxc_pendingCampaignLevel;
      hxc_l_gc_roots[744] = (const void *)hxc_l_tmp_load_result_n2304.hxc_value.hxc_snapshot.hxc_pendingCampaignPortal;
      hxc_l_gc_roots[745] = (const void *)hxc_l_tmp_load_result_n2304.hxc_value.hxc_snapshot.hxc_preparedPortal;
      struct hxc_caxecraft_app_EditorTestPlayRuntime *hxc_l_tmp_optional_record_field_project_n2306 = hxc_l_tmp_load_result_n2304.hxc_value.hxc_runtime;
      hxc_l_gc_roots[746] = (const void *)hxc_l_tmp_optional_record_field_project_n2306;
      if (hxc_l_tmp_optional_record_field_project_n2306 == NULL)
      {
        abort();
      }
      hxc_caxecraft_app_EditorTestPlayRuntime_stop(hxc_l_tmp_optional_record_field_project_n2306);
      hxc_l_gc_roots[747] = (struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = false }.hxc_has_value ? (const void *)(struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = false }.hxc_value.hxc_runtime : NULL;
      hxc_l_gc_roots[748] = (struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = false }.hxc_has_value ? (const void *)(struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = false }.hxc_value.hxc_snapshot.hxc_campaignLevel : NULL;
      hxc_l_gc_roots[749] = (struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = false }.hxc_has_value ? (const void *)(struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = false }.hxc_value.hxc_snapshot.hxc_pendingCampaignLevel : NULL;
      hxc_l_gc_roots[750] = (struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = false }.hxc_has_value ? (const void *)(struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = false }.hxc_value.hxc_snapshot.hxc_pendingCampaignPortal : NULL;
      hxc_l_gc_roots[751] = (struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = false }.hxc_has_value ? (const void *)(struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = false }.hxc_value.hxc_snapshot.hxc_preparedPortal : NULL;
      hxc_optional_9d9d283f_destroy(&hxc_l_activeEditorTestPlay);
      hxc_l_activeEditorTestPlay = (struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun){ .hxc_has_value = false };
      struct hxc_caxecraft_app_CaxecraftEditorScreen *hxc_l_tmp_load_result_n2307 = hxc_l_editorScreen;
      hxc_l_gc_roots[752] = (const void *)hxc_l_tmp_load_result_n2307;
      if (hxc_l_tmp_load_result_n2307 == NULL)
      {
        abort();
      }
      hxc_caxecraft_app_CaxecraftEditorScreen_finishTestPlay(hxc_l_tmp_load_result_n2307, false);
    }
    EnableCursor();
    struct hxc_caxecraft_app_RuntimeVoxelModelCatalog *hxc_l_tmp_load_result_n2308 = hxc_l_runtimeModels;
    hxc_l_gc_roots[753] = (const void *)hxc_l_tmp_load_result_n2308;
    if (hxc_l_tmp_load_result_n2308 == NULL)
    {
      abort();
    }
    hxc_caxecraft_app_RuntimeVoxelModelCatalog_unload(hxc_l_tmp_load_result_n2308);
    struct hxc_caxecraft_app_RuntimeTextureAtlasCatalog *hxc_l_tmp_load_result_n2309 = hxc_l_runtimeTextures;
    hxc_l_gc_roots[754] = (const void *)hxc_l_tmp_load_result_n2309;
    if (hxc_l_tmp_load_result_n2309 == NULL)
    {
      abort();
    }
    hxc_caxecraft_app_RuntimeTextureAtlasCatalog_unload(hxc_l_tmp_load_result_n2309);
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
    hxc_enum_2c0e695d_destroy(&hxc_l_tmp_static_call_argument_0_owner_n94);
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
    if (hxc_array_ref_release(hxc_l_tmp_object_storage_n84.hxc_stateStartTicks) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_tmp_object_storage_n84.hxc_stateIds) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_tmp_object_storage_n84.hxc_objectIds) != HXC_STATUS_OK)
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
