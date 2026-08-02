#include "hxc/program.h"

void hxc_compiler_constructor_caxecraft_app_CaxecraftApp(struct hxc_caxecraft_app_CaxecraftApp *hxc_l_self)
{
  struct hxc_caxecraft_app_TerrainRenderer *hxc_l_tmp_owned_class_field_address_n0 = &(*hxc_l_self).hxc_terrainRenderer;
  hxc_compiler_constructor_caxecraft_app_TerrainRenderer(hxc_l_tmp_owned_class_field_address_n0);
  struct hxc_caxecraft_input_NavigationRepeater *hxc_l_tmp_owned_class_field_address_n1 = &(*hxc_l_self).hxc_editorNavigation;
  hxc_compiler_constructor_caxecraft_input_NavigationRepeater(hxc_l_tmp_owned_class_field_address_n1);
  return;
}

enum hxc_caxecraft_gameplay_GuidePhase hxc_caxecraft_app_CaxecraftApp_advanceGuidePhase(enum hxc_caxecraft_gameplay_GuidePhase hxc_l_phase)
{
  switch (hxc_l_phase) {
    case hxc_caxecraft_gameplay_GuidePhase_Waiting:
      {
        break;
      }
    case hxc_caxecraft_gameplay_GuidePhase_Welcomed:
      {
        return hxc_caxecraft_gameplay_GuidePhase_SharedBerries;
      }
    case hxc_caxecraft_gameplay_GuidePhase_SharedBerries:
      {
        return hxc_caxecraft_gameplay_GuidePhase_SharedBerries;
      }
    default:
      {
        abort();
      }
  }
  return hxc_caxecraft_gameplay_GuidePhase_Welcomed;
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

void hxc_caxecraft_app_CaxecraftApp_drawActors(struct Camera3D hxc_l_camera, struct Texture hxc_l_entityTexture, bool hxc_l_entityTextureReady, struct hxc_caxecraft_domain_Character hxc_l_guide, struct hxc_caxecraft_domain_Character hxc_l_enemy, enum hxc_caxecraft_domain_ActorControllerPhase hxc_l_enemyPhase, struct hxc_caxecraft_gameplay_BerryDropState hxc_l_berryDrop)
{
  if (hxc_l_entityTextureReady)
  {
    double hxc_l_y_h22fd73ca09d1 = hxc_l_guide.hxc_body.hxc_y + 0.76;
    double hxc_l_z_hbe2b70802617 = hxc_l_guide.hxc_body.hxc_z;
    double hxc_l_tmp_load_result_n7 = hxc_l_y_h22fd73ca09d1;
    hxc_caxecraft_app_CaxecraftAtlas_drawWorldSprite(hxc_l_camera, hxc_l_entityTexture, hxc_caxecraft_app_WorldSprite_NiaFront, (struct Vector3){ .x = (float)hxc_l_guide.hxc_body.hxc_x, .y = (float)hxc_l_tmp_load_result_n7, .z = (float)hxc_l_z_hbe2b70802617 }, 0.95, 1.52);
  }
  else
  {
    double hxc_l_y_h3e1c9c7adcd4 = hxc_l_guide.hxc_body.hxc_y + 0.54;
    double hxc_l_z_h7b1ea64e0691 = hxc_l_guide.hxc_body.hxc_z;
    double hxc_l_tmp_load_result_n16 = hxc_l_y_h3e1c9c7adcd4;
    DrawCube((struct Vector3){ .x = (float)hxc_l_guide.hxc_body.hxc_x, .y = (float)hxc_l_tmp_load_result_n16, .z = (float)hxc_l_z_h7b1ea64e0691 }, (float)0.50, (float)0.86, (float)0.42, (struct Color){ .r = 42, .g = 150, .b = 160, .a = 255 });
    double hxc_l_y_h2562ce14c359 = hxc_l_guide.hxc_body.hxc_y + 1.18;
    double hxc_l_z_h5435af74f1f7 = hxc_l_guide.hxc_body.hxc_z;
    double hxc_l_tmp_load_result_n26 = hxc_l_y_h2562ce14c359;
    DrawCube((struct Vector3){ .x = (float)hxc_l_guide.hxc_body.hxc_x, .y = (float)hxc_l_tmp_load_result_n26, .z = (float)hxc_l_z_h5435af74f1f7 }, (float)0.44, (float)0.44, (float)0.44, (struct Color){ .r = 205, .g = 139, .b = 88, .a = 255 });
    double hxc_l_y_h99e36220b049 = hxc_l_guide.hxc_body.hxc_y + 1.41;
    double hxc_l_z_h6c58eb1f3d93 = hxc_l_guide.hxc_body.hxc_z;
    double hxc_l_tmp_load_result_n36 = hxc_l_y_h99e36220b049;
    DrawCube((struct Vector3){ .x = (float)hxc_l_guide.hxc_body.hxc_x, .y = (float)hxc_l_tmp_load_result_n36, .z = (float)hxc_l_z_h6c58eb1f3d93 }, (float)0.48, (float)0.16, (float)0.48, (struct Color){ .r = 62, .g = 40, .b = 47, .a = 255 });
  }
  if (hxc_l_enemy.hxc_vitals.hxc_health > 0)
  {
    if (hxc_l_entityTextureReady)
    {
      double hxc_l_y_h90412ad8ba57 = hxc_l_enemy.hxc_body.hxc_y + 0.48;
      double hxc_l_z_h223bea804323 = hxc_l_enemy.hxc_body.hxc_z;
      double hxc_l_tmp_load_result_n49 = hxc_l_y_h90412ad8ba57;
      hxc_caxecraft_app_CaxecraftAtlas_drawWorldSprite(hxc_l_camera, hxc_l_entityTexture, hxc_caxecraft_app_WorldSprite_MosslingFront, (struct Vector3){ .x = (float)hxc_l_enemy.hxc_body.hxc_x, .y = (float)hxc_l_tmp_load_result_n49, .z = (float)hxc_l_z_h223bea804323 }, 1.05, 0.96);
    }
    else
    {
      double hxc_l_y_h95b71126aca9 = hxc_l_enemy.hxc_body.hxc_y + 0.30;
      double hxc_l_z_h6e89dccf6e30 = hxc_l_enemy.hxc_body.hxc_z;
      double hxc_l_tmp_load_result_n58 = hxc_l_y_h95b71126aca9;
      DrawCube((struct Vector3){ .x = (float)hxc_l_enemy.hxc_body.hxc_x, .y = (float)hxc_l_tmp_load_result_n58, .z = (float)hxc_l_z_h6e89dccf6e30 }, (float)0.70, (float)0.54, (float)0.70, (struct Color){ .r = 74, .g = 119, .b = 67, .a = 255 });
      double hxc_l_y_h963e62dbf6dc = hxc_l_enemy.hxc_body.hxc_y + 0.66;
      double hxc_l_z_hdcbcbb59556a = hxc_l_enemy.hxc_body.hxc_z;
      double hxc_l_tmp_load_result_n68 = hxc_l_y_h963e62dbf6dc;
      DrawCube((struct Vector3){ .x = (float)hxc_l_enemy.hxc_body.hxc_x, .y = (float)hxc_l_tmp_load_result_n68, .z = (float)hxc_l_z_hdcbcbb59556a }, (float)0.50, (float)0.34, (float)0.50, (struct Color){ .r = 157, .g = 190, .b = 82, .a = 255 });
    }
    if (hxc_l_enemyPhase == hxc_caxecraft_domain_ActorControllerPhase_Windup)
    {
      double hxc_l_y_hb2c7f119844b = hxc_l_enemy.hxc_body.hxc_y + 1.02;
      double hxc_l_z_h76cdbb87e4cc = hxc_l_enemy.hxc_body.hxc_z;
      double hxc_l_tmp_load_result_n79 = hxc_l_y_hb2c7f119844b;
      DrawCube((struct Vector3){ .x = (float)hxc_l_enemy.hxc_body.hxc_x, .y = (float)hxc_l_tmp_load_result_n79, .z = (float)hxc_l_z_h76cdbb87e4cc }, (float)0.20, (float)0.20, (float)0.20, (struct Color){ .r = 218, .g = 65, .b = 72, .a = 255 });
    }
  }
  if (hxc_l_berryDrop.hxc_active)
  {
    double hxc_l_y_h66e65d54f1ef = hxc_l_berryDrop.hxc_y;
    double hxc_l_z_h4eb6022f55c0 = hxc_l_berryDrop.hxc_z;
    double hxc_l_tmp_load_result_n87 = hxc_l_y_h66e65d54f1ef;
    DrawCube((struct Vector3){ .x = (float)(hxc_l_berryDrop.hxc_x - 0.12), .y = (float)hxc_l_tmp_load_result_n87, .z = (float)hxc_l_z_h4eb6022f55c0 }, (float)0.18, (float)0.18, (float)0.18, (struct Color){ .r = 174, .g = 78, .b = 136, .a = 255 });
    double hxc_l_y_h28bb0fadf1f2 = hxc_l_berryDrop.hxc_y;
    double hxc_l_z_h0d99ace2de3d = hxc_l_berryDrop.hxc_z;
    double hxc_l_tmp_load_result_n94 = hxc_l_y_h28bb0fadf1f2;
    DrawCube((struct Vector3){ .x = (float)(hxc_l_berryDrop.hxc_x + 0.12), .y = (float)hxc_l_tmp_load_result_n94, .z = (float)hxc_l_z_h0d99ace2de3d }, (float)0.18, (float)0.18, (float)0.18, (struct Color){ .r = 174, .g = 78, .b = 136, .a = 255 });
  }
  return;
}

void hxc_caxecraft_app_CaxecraftApp_drawBreath(int32_t hxc_l_breathTicks, int32_t hxc_l_maximumBreathTicks, int32_t hxc_l_width, int32_t hxc_l_height)
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
  int32_t hxc_l_startX = hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_i32_subtract_wrapping(hxc_l_width, hxc_i32_subtract_wrapping(hxc_i32_multiply_wrapping(hxc_l_bubbleCount, 18), 4)), (double)2));
  int32_t hxc_l_y = hxc_i32_subtract_wrapping(hxc_l_height, 128);
  int32_t hxc_l_bubble = 0;
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n6 = hxc_l_bubble;
    if (!(hxc_l_tmp_load_result_n6 < hxc_l_bubbleCount))
    {
      break;
    }
    int32_t hxc_l_tmp_load_result_n8 = hxc_l_bubble;
    if (hxc_l_tmp_load_result_n8 < hxc_l_filled)
    {
      float hxc_l_radius_h475c1ba65823 = (float)6.0;
      struct Color hxc_l_this1_h90b60f3789b8 = (struct Color){ .r = 164, .g = 235, .b = 246, .a = 255 };
      int32_t hxc_l_tmp_load_result_n11 = hxc_l_startX;
      int32_t hxc_l_tmp_load_result_n12 = hxc_l_bubble;
      int32_t hxc_l_tmp_load_result_n13 = hxc_l_y;
      float hxc_l_tmp_load_result_n14 = hxc_l_radius_h475c1ba65823;
      DrawCircle((int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n11, hxc_i32_multiply_wrapping(hxc_l_tmp_load_result_n12, 18)), (int32_t)hxc_l_tmp_load_result_n13, hxc_l_tmp_load_result_n14, hxc_l_this1_h90b60f3789b8);
    }
    else
    {
      float hxc_l_radius_h99fc1d2f51e4 = (float)6.0;
      struct Color hxc_l_this1_ha0ecb373d9cf = (struct Color){ .r = 49, .g = 82, .b = 103, .a = 210 };
      int32_t hxc_l_tmp_load_result_n17 = hxc_l_startX;
      int32_t hxc_l_tmp_load_result_n18 = hxc_l_bubble;
      int32_t hxc_l_tmp_load_result_n19 = hxc_l_y;
      float hxc_l_tmp_load_result_n20 = hxc_l_radius_h99fc1d2f51e4;
      DrawCircle((int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n17, hxc_i32_multiply_wrapping(hxc_l_tmp_load_result_n18, 18)), (int32_t)hxc_l_tmp_load_result_n19, hxc_l_tmp_load_result_n20, hxc_l_this1_ha0ecb373d9cf);
    }
    hxc_l_bubble = hxc_i32_add_wrapping(hxc_l_bubble, 1);
  }
  return;
}

void hxc_caxecraft_app_CaxecraftApp_drawHealth(struct hxc_caxecraft_domain_VitalsState hxc_l_vitals, struct Texture hxc_l_hudTexture, bool hxc_l_hudTextureReady, int32_t hxc_l_width)
{
  int32_t hxc_l_heart = 0;
  while (1)
  {
    if (!(hxc_l_heart < 3))
    {
      break;
    }
    int32_t hxc_l_x = hxc_i32_add_wrapping(hxc_i32_subtract_wrapping(hxc_l_width, 170), hxc_i32_multiply_wrapping(hxc_l_heart, 50));
    int32_t hxc_l_points = hxc_i32_subtract_wrapping(hxc_l_vitals.hxc_health, hxc_i32_multiply_wrapping(hxc_l_heart, 2));
    if (hxc_l_hudTextureReady)
    {
      if (hxc_l_points >= 2)
      {
        hxc_caxecraft_app_CaxecraftAtlas_drawHudGlyph(hxc_l_hudTexture, hxc_caxecraft_app_HudGlyph_HealthFull, hxc_l_x, 18, 42);
      }
      else
      {
        if (hxc_l_points == 1)
        {
          hxc_caxecraft_app_CaxecraftAtlas_drawHudGlyph(hxc_l_hudTexture, hxc_caxecraft_app_HudGlyph_HealthHalf, hxc_l_x, 18, 42);
        }
        else
        {
          hxc_caxecraft_app_CaxecraftAtlas_drawHudGlyph(hxc_l_hudTexture, hxc_caxecraft_app_HudGlyph_HealthEmpty, hxc_l_x, 18, 42);
        }
      }
    }
    else
    {
      if (hxc_l_points > 0)
      {
        struct Color hxc_l_this1_h4bf1e3cb0db1 = (struct Color){ .r = 218, .g = 65, .b = 72, .a = 255 };
        int32_t hxc_l_tmp_load_result_n14 = hxc_l_x;
        DrawRectangle((int32_t)hxc_l_tmp_load_result_n14, (int32_t)22, (int32_t)34, (int32_t)26, hxc_l_this1_h4bf1e3cb0db1);
      }
      else
      {
        struct Color hxc_l_this1_h62a6326a039a = (struct Color){ .r = 228, .g = 239, .b = 227, .a = 255 };
        int32_t hxc_l_tmp_load_result_n17 = hxc_l_x;
        DrawRectangleLines((int32_t)hxc_l_tmp_load_result_n17, (int32_t)22, (int32_t)34, (int32_t)26, hxc_l_this1_h62a6326a039a);
      }
    }
    hxc_l_heart = hxc_i32_add_wrapping(hxc_l_heart, 1);
  }
  return;
}

void hxc_caxecraft_app_CaxecraftApp_drawHotbar(struct hxc_caxecraft_gameplay_InventoryState hxc_l_inventory, struct Texture hxc_l_hudTexture, bool hxc_l_hudTextureReady, struct Texture hxc_l_itemTexture, bool hxc_l_itemTextureReady, int32_t hxc_l_width, int32_t hxc_l_height)
{
  int32_t hxc_l_slotSize = 64;
  int32_t hxc_l_gap = 4;
  int32_t hxc_l_tmp_load_result_n0 = hxc_l_slotSize;
  int32_t hxc_l_totalWidth = hxc_i32_add_wrapping(hxc_i32_multiply_wrapping(8, hxc_l_tmp_load_result_n0), hxc_i32_multiply_wrapping(7, hxc_l_gap));
  int32_t hxc_l_startX = hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_i32_subtract_wrapping(hxc_l_width, hxc_l_totalWidth), (double)2));
  int32_t hxc_l_y = hxc_i32_subtract_wrapping(hxc_l_height, 96);
  int32_t hxc_l_slot = 0;
  while (1)
  {
    if (!(hxc_l_slot < 8))
    {
      break;
    }
    int32_t hxc_l_tmp_load_result_n4 = hxc_l_startX;
    int32_t hxc_l_tmp_load_result_n5 = hxc_l_slot;
    int32_t hxc_l_tmp_load_result_n6 = hxc_l_slotSize;
    int32_t hxc_l_x = hxc_i32_add_wrapping(hxc_l_tmp_load_result_n4, hxc_i32_multiply_wrapping(hxc_l_tmp_load_result_n5, hxc_i32_add_wrapping(hxc_l_tmp_load_result_n6, hxc_l_gap)));
    if (hxc_l_hudTextureReady)
    {
      if (hxc_l_slot == hxc_l_inventory.hxc_selected)
      {
        int32_t hxc_l_tmp_load_result_n11 = hxc_l_x;
        int32_t hxc_l_tmp_load_result_n12 = hxc_l_y;
        hxc_caxecraft_app_CaxecraftAtlas_drawHotbarFrame(hxc_l_hudTexture, hxc_caxecraft_app_HotbarFrame_Selected, hxc_l_tmp_load_result_n11, hxc_l_tmp_load_result_n12, hxc_l_slotSize);
      }
      else
      {
        int32_t hxc_l_tmp_load_result_n15 = hxc_l_x;
        int32_t hxc_l_tmp_load_result_n16 = hxc_l_y;
        hxc_caxecraft_app_CaxecraftAtlas_drawHotbarFrame(hxc_l_hudTexture, hxc_caxecraft_app_HotbarFrame_Normal, hxc_l_tmp_load_result_n15, hxc_l_tmp_load_result_n16, hxc_l_slotSize);
      }
    }
    else
    {
      struct Color hxc_l_this1 = (struct Color){ .r = 15, .g = 27, .b = 39, .a = 220 };
      int32_t hxc_l_tmp_load_result_n19 = hxc_l_x;
      int32_t hxc_l_tmp_load_result_n20 = hxc_l_y;
      int32_t hxc_l_tmp_load_result_n21 = hxc_l_slotSize;
      int32_t hxc_l_tmp_load_result_n22 = hxc_l_slotSize;
      DrawRectangle((int32_t)hxc_l_tmp_load_result_n19, (int32_t)hxc_l_tmp_load_result_n20, (int32_t)hxc_l_tmp_load_result_n21, (int32_t)hxc_l_tmp_load_result_n22, hxc_l_this1);
      int32_t hxc_l_tmp_load_result_n24 = hxc_l_slot;
      struct Color hxc_l_tmp_conditional_result_n16 = { 0 };
      if (hxc_l_tmp_load_result_n24 == hxc_l_inventory.hxc_selected)
      {
        hxc_l_tmp_conditional_result_n16 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
      }
      else
      {
        hxc_l_tmp_conditional_result_n16 = (struct Color){ .r = 228, .g = 239, .b = 227, .a = 255 };
      }
      struct Color hxc_l_color = hxc_l_tmp_conditional_result_n16;
      int32_t hxc_l_tmp_load_result_n29 = hxc_l_x;
      int32_t hxc_l_tmp_load_result_n30 = hxc_l_y;
      int32_t hxc_l_tmp_load_result_n31 = hxc_l_slotSize;
      int32_t hxc_l_tmp_load_result_n32 = hxc_l_slotSize;
      DrawRectangleLines((int32_t)hxc_l_tmp_load_result_n29, (int32_t)hxc_l_tmp_load_result_n30, (int32_t)hxc_l_tmp_load_result_n31, (int32_t)hxc_l_tmp_load_result_n32, hxc_l_color);
    }
    if (hxc_l_itemTextureReady)
    {
      enum hxc_caxecraft_gameplay_ItemKind hxc_l_tmp_call_result_n35 = hxc_caxecraft_gameplay_Inventory_itemAt(hxc_l_slot);
      int32_t hxc_l_tmp_load_result_n36 = hxc_l_x;
      int32_t hxc_l_tmp_load_result_n37 = hxc_l_y;
      hxc_caxecraft_app_CaxecraftAtlas_drawItem(hxc_l_itemTexture, hxc_l_tmp_call_result_n35, hxc_i32_add_wrapping(hxc_l_tmp_load_result_n36, 6), hxc_i32_add_wrapping(hxc_l_tmp_load_result_n37, 4), hxc_i32_subtract_wrapping(hxc_l_slotSize, 12));
    }
    int32_t hxc_l_tmp_call_result_n40 = hxc_caxecraft_gameplay_Inventory_countAt(hxc_l_inventory, hxc_l_slot);
    int32_t hxc_l_tmp_load_result_n41 = hxc_l_x;
    hxc_caxecraft_app_HudDigits_drawNumber(hxc_l_tmp_call_result_n40, hxc_i32_add_wrapping(hxc_l_tmp_load_result_n41, 39), hxc_i32_add_wrapping(hxc_l_y, 44), 2, (struct Color){ .r = 228, .g = 239, .b = 227, .a = 255 });
    hxc_l_slot = hxc_i32_add_wrapping(hxc_l_slot, 1);
  }
  return;
}

void hxc_caxecraft_app_CaxecraftApp_drawHud(struct hxc_caxecraft_app_HudView hxc_l_view, struct hxc_caxecraft_app_HudResources hxc_l_resources, struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_contentRegistry, struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_uiCatalog)
{
  const void *hxc_l_gc_roots[2] = { (const void *)hxc_l_contentRegistry, (const void *)hxc_l_uiCatalog };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 2, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n54 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n69 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n72 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n79 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n80 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n82 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n83 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n84 = { 0 };
  int32_t hxc_l_visible = hxc_l_view.hxc_metrics.hxc_visibleBlocks;
  int32_t hxc_l_drawCalls = hxc_l_view.hxc_metrics.hxc_drawCalls;
  int32_t hxc_l_frames = hxc_l_view.hxc_metrics.hxc_renderedFrames;
  int32_t hxc_l_updates = hxc_l_view.hxc_metrics.hxc_completedTicks;
  bool hxc_l_paused = hxc_l_view.hxc_paused;
  bool hxc_l_captured = hxc_l_view.hxc_pointerCaptured;
  bool hxc_l_placementBlocked = hxc_l_view.hxc_feedback.hxc_placementBlocked;
  struct hxc_caxecraft_domain_RaycastHit hxc_l_hit = hxc_l_view.hxc_hit;
  int32_t hxc_l_mode = hxc_l_view.hxc_mode;
  int32_t hxc_l_locale = hxc_l_view.hxc_locale;
  struct hxc_caxecraft_gameplay_InventoryState hxc_l_inventory = hxc_l_view.hxc_inventory;
  enum hxc_caxecraft_gameplay_GuidePhase hxc_l_guidePhase = hxc_l_view.hxc_guidePhase;
  bool hxc_l_guideInteractionAvailable = hxc_l_view.hxc_guideInteractionAvailable;
  struct hxc_caxecraft_domain_Character hxc_l_enemy = hxc_l_view.hxc_enemy;
  enum hxc_caxecraft_domain_ActorControllerPhase hxc_l_enemyPhase = hxc_l_view.hxc_enemyPhase;
  struct hxc_caxecraft_domain_VitalsState hxc_l_vitals = hxc_l_view.hxc_character.hxc_vitals;
  bool hxc_l_strikeHit = hxc_l_view.hxc_feedback.hxc_strikeHit;
  bool hxc_l_enemyDefeated = hxc_l_view.hxc_feedback.hxc_enemyDefeated;
  bool hxc_l_enemyAttacked = hxc_l_view.hxc_feedback.hxc_enemyAttacked;
  bool hxc_l_pickedUp = hxc_l_view.hxc_feedback.hxc_pickedUp;
  int32_t hxc_l_pickupAmount = hxc_l_view.hxc_feedback.hxc_pickupAmount;
  int32_t hxc_l_inventoryFullReason = hxc_l_view.hxc_feedback.hxc_inventoryFullReason;
  int32_t hxc_l_recoveryFeedback = hxc_l_view.hxc_feedback.hxc_recoveryDecision;
  bool hxc_l_recoveryVisible = hxc_l_view.hxc_feedback.hxc_recoveryVisible;
  struct Texture hxc_l_hudTexture = hxc_l_resources.hxc_hudTexture;
  bool hxc_l_hudTextureReady = hxc_l_resources.hxc_hudTextureReady;
  struct Texture hxc_l_itemTexture = hxc_l_resources.hxc_itemTexture;
  bool hxc_l_itemTextureReady = hxc_l_resources.hxc_itemTextureReady;
  struct Texture hxc_l_adventureItemTexture = hxc_l_resources.hxc_adventureItemTexture;
  bool hxc_l_adventureItemTextureReady = hxc_l_resources.hxc_adventureItemTextureReady;
  int32_t hxc_l_aquaticEquipmentCode = hxc_l_view.hxc_character.hxc_aquaticEquipmentCode;
  bool hxc_l_aquaticEquipmentVisible = hxc_l_view.hxc_character.hxc_aquaticEquipmentVisible;
  bool hxc_l_headSubmerged = hxc_l_view.hxc_character.hxc_headSubmerged;
  int32_t hxc_l_breathTicks = hxc_l_view.hxc_character.hxc_breathTicks;
  int32_t hxc_l_maximumBreathTicks = hxc_l_view.hxc_character.hxc_maximumBreathTicks;
  int32_t hxc_l_tmp_native_call_result_n54 = GetScreenWidth();
  int32_t hxc_l_width = (int32_t)hxc_l_tmp_native_call_result_n54;
  int32_t hxc_l_tmp_native_call_result_n55 = GetScreenHeight();
  int32_t hxc_l_height = (int32_t)hxc_l_tmp_native_call_result_n55;
  int32_t hxc_l_centerX = hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_l_width, (double)2));
  int32_t hxc_l_centerY = hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_l_height, (double)2));
  struct Color hxc_l_text = (struct Color){ .r = 228, .g = 239, .b = 227, .a = 255 };
  int32_t hxc_l_tmp_load_result_n59 = hxc_l_centerX;
  int32_t hxc_l_tmp_load_result_n60 = hxc_l_centerY;
  int32_t hxc_l_tmp_load_result_n61 = hxc_l_centerX;
  int32_t hxc_l_tmp_load_result_n62 = hxc_l_centerY;
  DrawLine((int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n59, 8), (int32_t)hxc_l_tmp_load_result_n60, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n61, 3), (int32_t)hxc_l_tmp_load_result_n62, hxc_l_text);
  int32_t hxc_l_tmp_load_result_n64 = hxc_l_centerX;
  int32_t hxc_l_tmp_load_result_n65 = hxc_l_centerY;
  int32_t hxc_l_tmp_load_result_n66 = hxc_l_centerX;
  int32_t hxc_l_tmp_load_result_n67 = hxc_l_centerY;
  DrawLine((int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n64, 3), (int32_t)hxc_l_tmp_load_result_n65, (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n66, 8), (int32_t)hxc_l_tmp_load_result_n67, hxc_l_text);
  int32_t hxc_l_tmp_load_result_n69 = hxc_l_centerX;
  int32_t hxc_l_tmp_load_result_n70 = hxc_l_centerY;
  int32_t hxc_l_tmp_load_result_n71 = hxc_l_centerX;
  int32_t hxc_l_tmp_load_result_n72 = hxc_l_centerY;
  DrawLine((int32_t)hxc_l_tmp_load_result_n69, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n70, 8), (int32_t)hxc_l_tmp_load_result_n71, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n72, 3), hxc_l_text);
  int32_t hxc_l_tmp_load_result_n74 = hxc_l_centerX;
  int32_t hxc_l_tmp_load_result_n75 = hxc_l_centerY;
  int32_t hxc_l_tmp_load_result_n76 = hxc_l_centerX;
  int32_t hxc_l_tmp_load_result_n77 = hxc_l_centerY;
  DrawLine((int32_t)hxc_l_tmp_load_result_n74, (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n75, 3), (int32_t)hxc_l_tmp_load_result_n76, (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n77, 8), hxc_l_text);
  struct Color hxc_l_this1_h5869dd58f6fd = (struct Color){ .r = 15, .g = 27, .b = 39, .a = 220 };
  DrawRectangle((int32_t)18, (int32_t)18, (int32_t)460, (int32_t)108, hxc_l_this1_h5869dd58f6fd);
  struct Color hxc_l_this1_h51eca5fd83b7 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
  DrawRectangleLines((int32_t)18, (int32_t)18, (int32_t)460, (int32_t)108, hxc_l_this1_h51eca5fd83b7);
  if (hxc_l_uiCatalog == NULL)
  {
    abort();
  }
  hxc_string hxc_l_tmp_instance_call_result_n84 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_uiCatalog, hxc_l_locale, 1);
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n46 = hxc_l_tmp_instance_call_result_n84;
  hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n86 = HXC_BORROWED_CSTRING_INITIALIZER;
  if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n46, &hxc_l_tmp_string_borrow_cstring_result_n86) != HXC_STATUS_OK)
  {
    abort();
  }
  DrawText(hxc_l_tmp_string_borrow_cstring_result_n86.data, (int32_t)32, (int32_t)28, (int32_t)20, hxc_l_text);
  hxc_string hxc_l_text1 = hxc_l_view.hxc_levelLabel;
  if (hxc_string_retain(hxc_l_text1) != HXC_STATUS_OK)
  {
    abort();
  }
  struct Color hxc_l_this1_ha31088bf5117 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
  hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n91 = HXC_BORROWED_CSTRING_INITIALIZER;
  if (hxc_string_borrow_cstring(&hxc_l_text1, &hxc_l_tmp_string_borrow_cstring_result_n91) != HXC_STATUS_OK)
  {
    abort();
  }
  DrawText(hxc_l_tmp_string_borrow_cstring_result_n91.data, (int32_t)250, (int32_t)30, (int32_t)16, hxc_l_this1_ha31088bf5117);
  hxc_string hxc_l_tmp_instance_call_result_n94 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_uiCatalog, hxc_l_locale, 4);
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n49 = hxc_l_tmp_instance_call_result_n94;
  hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n96 = HXC_BORROWED_CSTRING_INITIALIZER;
  if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n49, &hxc_l_tmp_string_borrow_cstring_result_n96) != HXC_STATUS_OK)
  {
    abort();
  }
  DrawText(hxc_l_tmp_string_borrow_cstring_result_n96.data, (int32_t)32, (int32_t)58, (int32_t)14, hxc_l_text);
  hxc_caxecraft_app_HudDigits_drawNumber(16384, 82, 59, 5, (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 });
  hxc_string hxc_l_tmp_instance_call_result_n100 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_uiCatalog, hxc_l_locale, 8);
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n50 = hxc_l_tmp_instance_call_result_n100;
  hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n102 = HXC_BORROWED_CSTRING_INITIALIZER;
  if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n50, &hxc_l_tmp_string_borrow_cstring_result_n102) != HXC_STATUS_OK)
  {
    abort();
  }
  DrawText(hxc_l_tmp_string_borrow_cstring_result_n102.data, (int32_t)160, (int32_t)58, (int32_t)14, hxc_l_text);
  hxc_caxecraft_app_HudDigits_drawNumber(hxc_l_visible, 230, 59, 5, (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 });
  hxc_string hxc_l_tmp_instance_call_result_n107 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_uiCatalog, hxc_l_locale, 5);
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n51 = hxc_l_tmp_instance_call_result_n107;
  hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n109 = HXC_BORROWED_CSTRING_INITIALIZER;
  if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n51, &hxc_l_tmp_string_borrow_cstring_result_n109) != HXC_STATUS_OK)
  {
    abort();
  }
  DrawText(hxc_l_tmp_string_borrow_cstring_result_n109.data, (int32_t)326, (int32_t)58, (int32_t)14, hxc_l_text);
  hxc_caxecraft_app_HudDigits_drawNumber(hxc_l_drawCalls, 382, 59, 5, (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 });
  hxc_string hxc_l_tmp_instance_call_result_n114 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_uiCatalog, hxc_l_locale, 6);
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n52 = hxc_l_tmp_instance_call_result_n114;
  hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n116 = HXC_BORROWED_CSTRING_INITIALIZER;
  if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n52, &hxc_l_tmp_string_borrow_cstring_result_n116) != HXC_STATUS_OK)
  {
    abort();
  }
  DrawText(hxc_l_tmp_string_borrow_cstring_result_n116.data, (int32_t)32, (int32_t)86, (int32_t)12, hxc_l_text);
  int32_t hxc_l_tmp_load_result_n118 = hxc_l_frames;
  hxc_caxecraft_app_HudDigits_drawNumber(hxc_l_tmp_load_result_n118, 82, 85, 6, hxc_l_text);
  hxc_string hxc_l_tmp_instance_call_result_n121 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_uiCatalog, hxc_l_locale, 7);
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n53 = hxc_l_tmp_instance_call_result_n121;
  hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n123 = HXC_BORROWED_CSTRING_INITIALIZER;
  if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n53, &hxc_l_tmp_string_borrow_cstring_result_n123) != HXC_STATUS_OK)
  {
    abort();
  }
  DrawText(hxc_l_tmp_string_borrow_cstring_result_n123.data, (int32_t)174, (int32_t)86, (int32_t)12, hxc_l_text);
  int32_t hxc_l_tmp_load_result_n125 = hxc_l_updates;
  hxc_caxecraft_app_HudDigits_drawNumber(hxc_l_tmp_load_result_n125, 216, 85, 6, hxc_l_text);
  struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_load_result_n127 = hxc_l_inventory;
  struct Texture hxc_l_tmp_load_result_n128 = hxc_l_hudTexture;
  bool hxc_l_tmp_load_result_n129 = hxc_l_hudTextureReady;
  struct Texture hxc_l_tmp_load_result_n130 = hxc_l_itemTexture;
  bool hxc_l_tmp_load_result_n131 = hxc_l_itemTextureReady;
  int32_t hxc_l_tmp_load_result_n132 = hxc_l_width;
  hxc_caxecraft_app_CaxecraftApp_drawHotbar(hxc_l_tmp_load_result_n127, hxc_l_tmp_load_result_n128, hxc_l_tmp_load_result_n129, hxc_l_tmp_load_result_n130, hxc_l_tmp_load_result_n131, hxc_l_tmp_load_result_n132, hxc_l_height);
  struct hxc_caxecraft_domain_VitalsState hxc_l_tmp_load_result_n134 = hxc_l_vitals;
  struct Texture hxc_l_tmp_load_result_n135 = hxc_l_hudTexture;
  bool hxc_l_tmp_load_result_n136 = hxc_l_hudTextureReady;
  hxc_caxecraft_app_CaxecraftApp_drawHealth(hxc_l_tmp_load_result_n134, hxc_l_tmp_load_result_n135, hxc_l_tmp_load_result_n136, hxc_l_width);
  if (hxc_l_aquaticEquipmentCode >= 0)
  {
    int32_t hxc_l_tmp_load_result_n139 = hxc_l_aquaticEquipmentCode;
    struct Texture hxc_l_tmp_load_result_n140 = hxc_l_itemTexture;
    bool hxc_l_tmp_load_result_n141 = hxc_l_itemTextureReady;
    struct Texture hxc_l_tmp_load_result_n142 = hxc_l_adventureItemTexture;
    bool hxc_l_tmp_load_result_n143 = hxc_l_adventureItemTextureReady;
    hxc_caxecraft_app_AuthoredItemRenderer_drawEquippedIcon(hxc_l_contentRegistry, hxc_l_tmp_load_result_n139, hxc_l_tmp_load_result_n140, hxc_l_tmp_load_result_n141, hxc_l_tmp_load_result_n142, hxc_l_tmp_load_result_n143, hxc_i32_subtract_wrapping(hxc_l_width, 226), 18, 42);
  }
  if (hxc_l_headSubmerged)
  {
    int32_t hxc_l_tmp_load_result_n146 = hxc_l_breathTicks;
    int32_t hxc_l_tmp_load_result_n147 = hxc_l_maximumBreathTicks;
    int32_t hxc_l_tmp_load_result_n148 = hxc_l_width;
    hxc_caxecraft_app_CaxecraftApp_drawBreath(hxc_l_tmp_load_result_n146, hxc_l_tmp_load_result_n147, hxc_l_tmp_load_result_n148, hxc_l_height);
  }
  hxc_string hxc_l_tmp_instance_call_result_n151 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_uiCatalog, hxc_l_locale, 3);
  hxc_l_tmp_cstring_ref_owner_owner_n54 = hxc_l_tmp_instance_call_result_n151;
  hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n153 = HXC_BORROWED_CSTRING_INITIALIZER;
  if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n54, &hxc_l_tmp_string_borrow_cstring_result_n153) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_tmp_load_result_n154 = hxc_l_height;
  DrawText(hxc_l_tmp_string_borrow_cstring_result_n153.data, (int32_t)20, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n154, 22), (int32_t)14, hxc_l_text);
  if (hxc_l_mode == 1)
  {
    struct Color hxc_l_this1_h452ca19d25a7 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
    const char *hxc_l_tmp_call_result_n159 = hxc_caxecraft_localization_FirstPlayableCatalog_text(hxc_l_locale, 0);
    DrawText(hxc_l_tmp_call_result_n159, (int32_t)32, (int32_t)110, (int32_t)14, hxc_l_this1_h452ca19d25a7);
  }
  if (hxc_l_guideInteractionAvailable)
  {
    struct Color hxc_l_this1_h08a4123c263e = (struct Color){ .r = 15, .g = 27, .b = 39, .a = 220 };
    int32_t hxc_l_tmp_load_result_n163 = hxc_l_centerX;
    int32_t hxc_l_tmp_load_result_n164 = hxc_l_centerY;
    DrawRectangle((int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n163, 260), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n164, 54), (int32_t)520, (int32_t)60, hxc_l_this1_h08a4123c263e);
    if (hxc_l_guidePhase == hxc_caxecraft_gameplay_GuidePhase_Waiting)
    {
      const char *hxc_l_tmp_call_result_n169 = hxc_caxecraft_localization_FirstPlayableCatalog_text(hxc_l_locale, 13);
      int32_t hxc_l_tmp_load_result_n170 = hxc_l_centerX;
      int32_t hxc_l_tmp_load_result_n171 = hxc_l_centerY;
      DrawText(hxc_l_tmp_call_result_n169, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n170, 110), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n171, 74), (int32_t)18, hxc_l_text);
    }
    else
    {
      if (hxc_l_guidePhase == hxc_caxecraft_gameplay_GuidePhase_Welcomed)
      {
        const char *hxc_l_tmp_call_result_n176 = hxc_caxecraft_localization_FirstPlayableCatalog_text(hxc_l_locale, 14);
        int32_t hxc_l_tmp_load_result_n177 = hxc_l_centerX;
        int32_t hxc_l_tmp_load_result_n178 = hxc_l_centerY;
        DrawText(hxc_l_tmp_call_result_n176, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n177, 225), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n178, 74), (int32_t)16, hxc_l_text);
      }
      else
      {
        const char *hxc_l_tmp_call_result_n181 = hxc_caxecraft_localization_FirstPlayableCatalog_text(hxc_l_locale, 12);
        int32_t hxc_l_tmp_load_result_n182 = hxc_l_centerX;
        int32_t hxc_l_tmp_load_result_n183 = hxc_l_centerY;
        DrawText(hxc_l_tmp_call_result_n181, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n182, 205), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n183, 74), (int32_t)16, hxc_l_text);
      }
    }
  }
  if (hxc_l_enemy.hxc_vitals.hxc_health > 0)
  {
    if (hxc_l_enemyPhase == hxc_caxecraft_domain_ActorControllerPhase_Windup)
    {
      struct Color hxc_l_this1_h75b106b8d9fd = (struct Color){ .r = 218, .g = 65, .b = 72, .a = 255 };
      const char *hxc_l_tmp_call_result_n190 = hxc_caxecraft_localization_FirstPlayableCatalog_text(hxc_l_locale, 11);
      int32_t hxc_l_tmp_load_result_n191 = hxc_l_width;
      DrawText(hxc_l_tmp_call_result_n190, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n191, 300), (int32_t)28, (int32_t)16, hxc_l_this1_h75b106b8d9fd);
    }
    else
    {
      if (hxc_l_enemyPhase == hxc_caxecraft_domain_ActorControllerPhase_Chasing)
      {
        struct Color hxc_l_this1_h770278252c9d = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
        const char *hxc_l_tmp_call_result_n197 = hxc_caxecraft_localization_FirstPlayableCatalog_text(hxc_l_locale, 9);
        int32_t hxc_l_tmp_load_result_n198 = hxc_l_width;
        DrawText(hxc_l_tmp_call_result_n197, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n198, 180), (int32_t)28, (int32_t)16, hxc_l_this1_h770278252c9d);
      }
    }
  }
  if (hxc_l_strikeHit)
  {
    struct Color hxc_l_this1_h43e5ea78189f = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
    const char *hxc_l_tmp_call_result_n203 = hxc_caxecraft_localization_FirstPlayableCatalog_text(hxc_l_locale, 7);
    int32_t hxc_l_tmp_load_result_n204 = hxc_l_centerX;
    int32_t hxc_l_tmp_load_result_n205 = hxc_l_centerY;
    DrawText(hxc_l_tmp_call_result_n203, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n204, 70), (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n205, 54), (int32_t)18, hxc_l_this1_h43e5ea78189f);
  }
  if (hxc_l_enemyDefeated)
  {
    struct Color hxc_l_this1_h8e46c0ebb33c = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
    const char *hxc_l_tmp_call_result_n210 = hxc_caxecraft_localization_FirstPlayableCatalog_text(hxc_l_locale, 10);
    int32_t hxc_l_tmp_load_result_n211 = hxc_l_width;
    DrawText(hxc_l_tmp_call_result_n210, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n211, 285), (int32_t)54, (int32_t)16, hxc_l_this1_h8e46c0ebb33c);
  }
  if (hxc_l_enemyAttacked)
  {
    struct Color hxc_l_this1_ha9c6dba18a6c = (struct Color){ .r = 218, .g = 65, .b = 72, .a = 255 };
    const char *hxc_l_tmp_call_result_n216 = hxc_caxecraft_localization_FirstPlayableCatalog_text(hxc_l_locale, 18);
    int32_t hxc_l_tmp_load_result_n217 = hxc_l_width;
    DrawText(hxc_l_tmp_call_result_n216, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n217, 330), (int32_t)82, (int32_t)16, hxc_l_this1_ha9c6dba18a6c);
  }
  if (hxc_l_pickedUp)
  {
    int32_t hxc_l_tmp_load_result_n220 = hxc_l_pickupAmount;
    int32_t hxc_l_tmp_conditional_result_n63 = 0;
    if (hxc_l_tmp_load_result_n220 == 1)
    {
      hxc_l_tmp_conditional_result_n63 = 2;
    }
    else
    {
      hxc_l_tmp_conditional_result_n63 = 3;
    }
    int32_t hxc_l_pickupMessage = hxc_l_tmp_conditional_result_n63;
    struct Color hxc_l_this1_h40cf9fda8676 = (struct Color){ .r = 174, .g = 78, .b = 136, .a = 255 };
    int32_t hxc_l_tmp_load_result_n223 = hxc_l_locale;
    const char *hxc_l_tmp_call_result_n225 = hxc_caxecraft_localization_FirstPlayableCatalog_text(hxc_l_tmp_load_result_n223, hxc_l_pickupMessage);
    int32_t hxc_l_tmp_load_result_n226 = hxc_l_centerX;
    int32_t hxc_l_tmp_load_result_n227 = hxc_l_centerY;
    DrawText(hxc_l_tmp_call_result_n225, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n226, 48), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n227, 24), (int32_t)18, hxc_l_this1_h40cf9fda8676);
  }
  if (hxc_l_inventoryFullReason == 1)
  {
    struct Color hxc_l_this1_h75c8e4ea16eb = (struct Color){ .r = 238, .g = 113, .b = 78, .a = 255 };
    const char *hxc_l_tmp_call_result_n232 = hxc_caxecraft_localization_FirstPlayableCatalog_text(hxc_l_locale, 5);
    int32_t hxc_l_tmp_load_result_n233 = hxc_l_centerX;
    int32_t hxc_l_tmp_load_result_n234 = hxc_l_centerY;
    DrawText(hxc_l_tmp_call_result_n232, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n233, 150), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n234, 48), (int32_t)16, hxc_l_this1_h75c8e4ea16eb);
  }
  else
  {
    if (hxc_l_inventoryFullReason == 2)
    {
      struct Color hxc_l_this1_h29f7336446de = (struct Color){ .r = 238, .g = 113, .b = 78, .a = 255 };
      const char *hxc_l_tmp_call_result_n239 = hxc_caxecraft_localization_FirstPlayableCatalog_text(hxc_l_locale, 6);
      int32_t hxc_l_tmp_load_result_n240 = hxc_l_centerX;
      int32_t hxc_l_tmp_load_result_n241 = hxc_l_centerY;
      DrawText(hxc_l_tmp_call_result_n239, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n240, 155), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n241, 48), (int32_t)16, hxc_l_this1_h29f7336446de);
    }
  }
  if (hxc_l_recoveryVisible)
  {
    if (hxc_l_recoveryFeedback == 1)
    {
      struct Color hxc_l_this1_h8ea19ec1b732 = (struct Color){ .r = 94, .g = 212, .b = 136, .a = 255 };
      const char *hxc_l_tmp_call_result_n247 = hxc_caxecraft_localization_FirstPlayableCatalog_text(hxc_l_locale, 4);
      int32_t hxc_l_tmp_load_result_n248 = hxc_l_centerX;
      int32_t hxc_l_tmp_load_result_n249 = hxc_l_centerY;
      DrawText(hxc_l_tmp_call_result_n247, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n248, 88), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n249, 24), (int32_t)18, hxc_l_this1_h8ea19ec1b732);
    }
    else
    {
      if (hxc_l_recoveryFeedback == 2)
      {
        struct Color hxc_l_this1_h066e5fa103c6 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
        hxc_string hxc_l_tmp_instance_call_result_n254 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_uiCatalog, hxc_l_locale, 25);
        hxc_l_tmp_cstring_ref_owner_owner_n69 = hxc_l_tmp_instance_call_result_n254;
        hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n256 = HXC_BORROWED_CSTRING_INITIALIZER;
        if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n69, &hxc_l_tmp_string_borrow_cstring_result_n256) != HXC_STATUS_OK)
        {
          abort();
        }
        int32_t hxc_l_tmp_load_result_n257 = hxc_l_centerX;
        int32_t hxc_l_tmp_load_result_n258 = hxc_l_centerY;
        DrawText(hxc_l_tmp_string_borrow_cstring_result_n256.data, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n257, 96), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n258, 24), (int32_t)18, hxc_l_this1_h066e5fa103c6);
        if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n69) != HXC_STATUS_OK)
        {
          abort();
        }
      }
      else
      {
        if (hxc_l_recoveryFeedback == 3)
        {
          struct Color hxc_l_this1_hc497a81b47f0 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
          const char *hxc_l_tmp_call_result_n263 = hxc_caxecraft_localization_FirstPlayableCatalog_text(hxc_l_locale, 15);
          int32_t hxc_l_tmp_load_result_n264 = hxc_l_centerX;
          int32_t hxc_l_tmp_load_result_n265 = hxc_l_centerY;
          DrawText(hxc_l_tmp_call_result_n263, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n264, 76), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n265, 24), (int32_t)18, hxc_l_this1_hc497a81b47f0);
        }
      }
    }
  }
  if (hxc_l_aquaticEquipmentVisible)
  {
    struct Color hxc_l_this1_h35caa2449eb8 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
    hxc_string hxc_l_tmp_instance_call_result_n270 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_uiCatalog, hxc_l_locale, 0);
    hxc_l_tmp_cstring_ref_owner_owner_n72 = hxc_l_tmp_instance_call_result_n270;
    hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n272 = HXC_BORROWED_CSTRING_INITIALIZER;
    if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n72, &hxc_l_tmp_string_borrow_cstring_result_n272) != HXC_STATUS_OK)
    {
      abort();
    }
    int32_t hxc_l_tmp_load_result_n273 = hxc_l_centerX;
    int32_t hxc_l_tmp_load_result_n274 = hxc_l_centerY;
    DrawText(hxc_l_tmp_string_borrow_cstring_result_n272.data, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n273, 128), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n274, 24), (int32_t)18, hxc_l_this1_h35caa2449eb8);
    if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n72) != HXC_STATUS_OK)
    {
      abort();
    }
  }
  if (hxc_l_vitals.hxc_safeTicks > 15)
  {
    struct Color hxc_l_this1_h2d9c33a5a0ab = (struct Color){ .r = 218, .g = 65, .b = 72, .a = 255 };
    int32_t hxc_l_tmp_load_result_n278 = hxc_l_width;
    int32_t hxc_l_tmp_load_result_n279 = hxc_l_height;
    DrawRectangleLines((int32_t)4, (int32_t)4, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n278, 8), (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n279, 8), hxc_l_this1_h2d9c33a5a0ab);
  }
  if (hxc_l_vitals.hxc_health <= 0)
  {
    struct Color hxc_l_this1_h97a7f4d6b05c = (struct Color){ .r = 15, .g = 27, .b = 39, .a = 220 };
    int32_t hxc_l_tmp_load_result_n283 = hxc_l_centerX;
    int32_t hxc_l_tmp_load_result_n284 = hxc_l_centerY;
    DrawRectangle((int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n283, 250), (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n284, 74), (int32_t)500, (int32_t)148, hxc_l_this1_h97a7f4d6b05c);
    struct Color hxc_l_this1_hed2f4b196aa6 = (struct Color){ .r = 218, .g = 65, .b = 72, .a = 255 };
    int32_t hxc_l_tmp_load_result_n287 = hxc_l_centerX;
    int32_t hxc_l_tmp_load_result_n288 = hxc_l_centerY;
    DrawRectangleLines((int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n287, 250), (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n288, 74), (int32_t)500, (int32_t)148, hxc_l_this1_hed2f4b196aa6);
    const char *hxc_l_tmp_call_result_n291 = hxc_caxecraft_localization_FirstPlayableCatalog_text(hxc_l_locale, 8);
    int32_t hxc_l_tmp_load_result_n292 = hxc_l_centerX;
    int32_t hxc_l_tmp_load_result_n293 = hxc_l_centerY;
    DrawText(hxc_l_tmp_call_result_n291, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n292, 122), (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n293, 42), (int32_t)24, hxc_l_text);
    struct Color hxc_l_this1_h021eead6657e = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
    const char *hxc_l_tmp_call_result_n297 = hxc_caxecraft_localization_FirstPlayableCatalog_text(hxc_l_locale, 16);
    int32_t hxc_l_tmp_load_result_n298 = hxc_l_centerX;
    int32_t hxc_l_tmp_load_result_n299 = hxc_l_centerY;
    DrawText(hxc_l_tmp_call_result_n297, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n298, 125), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n299, 10), (int32_t)18, hxc_l_this1_h021eead6657e);
  }
  if (hxc_l_paused)
  {
    struct Color hxc_l_this1_h71c65fcc31a7 = (struct Color){ .r = 15, .g = 27, .b = 39, .a = 220 };
    int32_t hxc_l_tmp_load_result_n303 = hxc_l_centerX;
    int32_t hxc_l_tmp_load_result_n304 = hxc_l_centerY;
    DrawRectangle((int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n303, 170), (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n304, 48), (int32_t)340, (int32_t)96, hxc_l_this1_h71c65fcc31a7);
    struct Color hxc_l_this1_hd121f12aef01 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
    int32_t hxc_l_tmp_load_result_n307 = hxc_l_centerX;
    int32_t hxc_l_tmp_load_result_n308 = hxc_l_centerY;
    DrawRectangleLines((int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n307, 170), (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n308, 48), (int32_t)340, (int32_t)96, hxc_l_this1_hd121f12aef01);
    hxc_string hxc_l_tmp_instance_call_result_n311 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_uiCatalog, hxc_l_locale, 32);
    hxc_l_tmp_cstring_ref_owner_owner_n79 = hxc_l_tmp_instance_call_result_n311;
    hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n313 = HXC_BORROWED_CSTRING_INITIALIZER;
    if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n79, &hxc_l_tmp_string_borrow_cstring_result_n313) != HXC_STATUS_OK)
    {
      abort();
    }
    int32_t hxc_l_tmp_load_result_n314 = hxc_l_centerX;
    int32_t hxc_l_tmp_load_result_n315 = hxc_l_centerY;
    DrawText(hxc_l_tmp_string_borrow_cstring_result_n313.data, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n314, 48), (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n315, 30), (int32_t)24, hxc_l_text);
    hxc_string hxc_l_tmp_instance_call_result_n318 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_uiCatalog, hxc_l_locale, 31);
    hxc_l_tmp_cstring_ref_owner_owner_n80 = hxc_l_tmp_instance_call_result_n318;
    hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n320 = HXC_BORROWED_CSTRING_INITIALIZER;
    if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n80, &hxc_l_tmp_string_borrow_cstring_result_n320) != HXC_STATUS_OK)
    {
      abort();
    }
    int32_t hxc_l_tmp_load_result_n321 = hxc_l_centerX;
    int32_t hxc_l_tmp_load_result_n322 = hxc_l_centerY;
    DrawText(hxc_l_tmp_string_borrow_cstring_result_n320.data, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n321, 160), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n322, 8), (int32_t)16, hxc_l_text);
    if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n80) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n79) != HXC_STATUS_OK)
    {
      abort();
    }
  }
  else
  {
    if (hxc_l_placementBlocked)
    {
      struct Color hxc_l_this1_h78425a36fffc = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
      hxc_string hxc_l_tmp_instance_call_result_n327 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_uiCatalog, hxc_l_locale, 33);
      hxc_l_tmp_cstring_ref_owner_owner_n82 = hxc_l_tmp_instance_call_result_n327;
      hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n329 = HXC_BORROWED_CSTRING_INITIALIZER;
      if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n82, &hxc_l_tmp_string_borrow_cstring_result_n329) != HXC_STATUS_OK)
      {
        abort();
      }
      int32_t hxc_l_tmp_load_result_n330 = hxc_l_centerX;
      int32_t hxc_l_tmp_load_result_n331 = hxc_l_centerY;
      DrawText(hxc_l_tmp_string_borrow_cstring_result_n329.data, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n330, 170), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n331, 26), (int32_t)14, hxc_l_this1_h78425a36fffc);
      if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n82) != HXC_STATUS_OK)
      {
        abort();
      }
    }
    else
    {
      if (!hxc_l_captured)
      {
        hxc_string hxc_l_tmp_instance_call_result_n335 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_uiCatalog, hxc_l_locale, 2);
        hxc_l_tmp_cstring_ref_owner_owner_n83 = hxc_l_tmp_instance_call_result_n335;
        hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n337 = HXC_BORROWED_CSTRING_INITIALIZER;
        if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n83, &hxc_l_tmp_string_borrow_cstring_result_n337) != HXC_STATUS_OK)
        {
          abort();
        }
        int32_t hxc_l_tmp_load_result_n338 = hxc_l_centerX;
        int32_t hxc_l_tmp_load_result_n339 = hxc_l_centerY;
        DrawText(hxc_l_tmp_string_borrow_cstring_result_n337.data, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n338, 90), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n339, 26), (int32_t)14, hxc_l_text);
        if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n83) != HXC_STATUS_OK)
        {
          abort();
        }
      }
      else
      {
        if (!hxc_l_hit.hxc_hit)
        {
          hxc_string hxc_l_tmp_instance_call_result_n343 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_uiCatalog, hxc_l_locale, 30);
          hxc_l_tmp_cstring_ref_owner_owner_n84 = hxc_l_tmp_instance_call_result_n343;
          hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n345 = HXC_BORROWED_CSTRING_INITIALIZER;
          if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n84, &hxc_l_tmp_string_borrow_cstring_result_n345) != HXC_STATUS_OK)
          {
            abort();
          }
          int32_t hxc_l_tmp_load_result_n346 = hxc_l_centerX;
          int32_t hxc_l_tmp_load_result_n347 = hxc_l_centerY;
          DrawText(hxc_l_tmp_string_borrow_cstring_result_n345.data, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n346, 105), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n347, 26), (int32_t)14, hxc_l_text);
          if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n84) != HXC_STATUS_OK)
          {
            abort();
          }
        }
      }
    }
  }
  if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n54) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n53) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n52) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n51) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n50) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n49) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_string_release(&hxc_l_text1) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n46) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return;
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
  const void *hxc_l_gc_roots[178] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 178, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_string hxc_l_tmp_sys_println_string_argument_owner_n161 = { 0 };
  struct hxc_caxecraft_content_CampaignLevelLoadResult hxc_l_g_h06cfb79d5166 = { 0 };
  struct hxc_array_ref *hxc_l_phases = { 0 };
  hxc_string hxc_l_tmp_string_concat_left_owner_n171 = { 0 };
  hxc_string hxc_l_tmp_string_concat_left_owner_n172 = { 0 };
  hxc_string hxc_l_tmp_string_concat_right_owner_n173 = { 0 };
  hxc_string hxc_l_tmp_sys_println_string_argument_owner_n174 = { 0 };
  struct hxc_caxecraft_content_CampaignLevelLoadError hxc_l_error = { 0 };
  hxc_string hxc_l_tmp_string_concat_right_owner_n176 = { 0 };
  hxc_string hxc_l_tmp_sys_println_string_argument_owner_n177 = { 0 };
  struct hxc_caxecraft_content_CampaignPackageLoadResult hxc_l_g_h6904c3060e30 = { 0 };
  struct hxc_caxecraft_content_RuntimeLevelReceipt hxc_l_tmp_record_field_logicalPath_receiver_owner_n22 = { 0 };
  struct hxc_caxecraft_content_CampaignPackageLoadError hxc_l_gerror = { 0 };
  struct hxc_array_ref *hxc_l_g1 = { 0 };
  struct hxc_caxecraft_domain_ActorControllerEvent hxc_l_event = { 0 };
  struct hxc_caxecraft_domain_ActorControllerEvent hxc_l_tmp_array_element_owner_n235 = { 0 };
  hxc_string hxc_l_drop = { 0 };
  hxc_string hxc_l_levelLabel = { 0 };
  struct hxc_array_ref *hxc_l_actorPhases = { 0 };
  struct hxc_caxecraft_content_RuntimeLevelReceipt hxc_l_tmp_record_field_logicalPath_receiver_owner_n26 = { 0 };
  struct hxc_array_ref *hxc_l_presentationActorPhases = { 0 };
  struct hxc_caxecraft_content_RuntimeLevelReceipt hxc_l_receipt = { 0 };
  hxc_string hxc_l_tmp_sys_println_string_argument_owner_n30 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n301 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n309 = { 0 };
  hxc_string hxc_l_tmp_string_concat_right_owner_n31 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n317 = { 0 };
  hxc_string hxc_l_tmp_sys_println_string_argument_owner_n32 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n323 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n326 = { 0 };
  hxc_string hxc_l_tmp_string_concat_right_owner_n33 = { 0 };
  hxc_string hxc_l_tmp_sys_println_string_argument_owner_n34 = { 0 };
  struct hxc_caxecraft_app_HudView hxc_l_hudView = { 0 };
  struct hxc_array_ref *hxc_l_initialActorPhases = { 0 };
  struct hxc_caxecraft_content_RuntimeContentLoadResult hxc_l_g_h4ef171f4fe67 = { 0 };
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
  hxc_l_gc_roots[6] = (const void *)NULL;
  struct hxc_caxecraft_content_RuntimeContentGeneration *hxc_l_completeCandidate = NULL;
  struct hxc_caxecraft_content_ContentPackageStore *hxc_l_tmp_load_result_n5 = hxc_l_contentStore;
  hxc_l_gc_roots[7] = (const void *)hxc_l_tmp_load_result_n5;
  struct hxc_caxecraft_content_RuntimeContentLoadResult hxc_l_tmp_call_result_n8 = hxc_caxecraft_content_RuntimeContentGeneration_loadRuntimeContent(hxc_l_tmp_load_result_n5, 1, (struct hxc_caxecraft_content_RuntimeContentPlayerOptions){ .hxc_entityId = 1, .hxc_initialHealth = hxc_l_initialHealth });
  hxc_l_gc_roots[8] = hxc_l_tmp_call_result_n8.hxc_tag == hxc_caxecraft_content_RuntimeContentLoadResult_RuntimeContentReady ? (const void *)hxc_l_tmp_call_result_n8.hxc_payload.hxc_RuntimeContentReady.hxc_candidate : NULL;
  hxc_l_g_h4ef171f4fe67 = hxc_l_tmp_call_result_n8;
  hxc_l_gc_roots[9] = hxc_l_g_h4ef171f4fe67.hxc_tag == hxc_caxecraft_content_RuntimeContentLoadResult_RuntimeContentReady ? (const void *)hxc_l_g_h4ef171f4fe67.hxc_payload.hxc_RuntimeContentReady.hxc_candidate : NULL;
  switch (hxc_l_g_h4ef171f4fe67.hxc_tag) {
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
        hxc_enum_fcb4401b_destroy(&hxc_l_g_h4ef171f4fe67);
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
  hxc_l_gc_roots[10] = hxc_l_g_h4ef171f4fe67.hxc_tag == hxc_caxecraft_content_RuntimeContentLoadResult_RuntimeContentReady ? (const void *)hxc_l_g_h4ef171f4fe67.hxc_payload.hxc_RuntimeContentReady.hxc_candidate : NULL;
  if (hxc_l_g_h4ef171f4fe67.hxc_tag != hxc_caxecraft_content_RuntimeContentLoadResult_RuntimeContentReady)
  {
    abort();
  }
  struct hxc_caxecraft_content_RuntimeContentGeneration *hxc_l_tmp_enum_payload_project_n11 = hxc_l_g_h4ef171f4fe67.hxc_payload.hxc_RuntimeContentReady.hxc_candidate;
  hxc_l_gc_roots[11] = (const void *)hxc_l_tmp_enum_payload_project_n11;
  struct hxc_caxecraft_content_RuntimeContentGeneration *hxc_l_candidate_hcf9d4ad136bd = hxc_l_tmp_enum_payload_project_n11;
  hxc_l_gc_roots[12] = (const void *)hxc_l_candidate_hcf9d4ad136bd;
  hxc_l_completeCandidate = hxc_l_candidate_hcf9d4ad136bd;
  struct hxc_caxecraft_content_RuntimeContentGeneration *hxc_l_tmp_load_result_n13 = hxc_l_completeCandidate;
  hxc_l_gc_roots[13] = (const void *)hxc_l_tmp_load_result_n13;
  struct hxc_caxecraft_content_ActiveRuntimeContent *hxc_l_tmp_managed_class_result_n14 = NULL;
  if (hxc_gc_allocate(&hxc_program_gc, &hxc_caxecraft_content_ActiveRuntimeContent_descriptor, (void **)&hxc_l_tmp_managed_class_result_n14) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_gc_roots[14] = (const void *)hxc_l_tmp_managed_class_result_n14;
  hxc_compiler_constructor_caxecraft_content_ActiveRuntimeContent(hxc_l_tmp_managed_class_result_n14, hxc_l_tmp_load_result_n13);
  hxc_l_gc_roots[15] = (const void *)hxc_l_tmp_managed_class_result_n14;
  struct hxc_caxecraft_content_ActiveRuntimeContent *hxc_l_activeRuntimeContent = hxc_l_tmp_managed_class_result_n14;
  struct hxc_caxecraft_content_ActiveRuntimeContent *hxc_l_tmp_load_result_n15 = hxc_l_activeRuntimeContent;
  hxc_l_gc_roots[16] = (const void *)hxc_l_tmp_load_result_n15;
  if (hxc_l_tmp_load_result_n15 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_content_RuntimeContentGeneration *hxc_l_tmp_class_field_load_result_n16 = (*hxc_l_tmp_load_result_n15).hxc_active;
  hxc_l_gc_roots[17] = (const void *)hxc_l_tmp_class_field_load_result_n16;
  struct hxc_caxecraft_content_RuntimeContentGeneration *hxc_l_runtimeContent = hxc_l_tmp_class_field_load_result_n16;
  struct hxc_caxecraft_content_RuntimeContentGeneration *hxc_l_tmp_load_result_n17 = hxc_l_runtimeContent;
  hxc_l_gc_roots[18] = (const void *)hxc_l_tmp_load_result_n17;
  if (hxc_l_tmp_load_result_n17 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_tmp_class_field_load_result_n18 = (*hxc_l_tmp_load_result_n17).hxc_contentRegistry;
  hxc_l_gc_roots[19] = (const void *)hxc_l_tmp_class_field_load_result_n18;
  struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_contentRegistry = hxc_l_tmp_class_field_load_result_n18;
  struct hxc_caxecraft_content_RuntimeContentGeneration *hxc_l_tmp_load_result_n19 = hxc_l_runtimeContent;
  hxc_l_gc_roots[20] = (const void *)hxc_l_tmp_load_result_n19;
  if (hxc_l_tmp_load_result_n19 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_tmp_class_field_load_result_n20 = (*hxc_l_tmp_load_result_n19).hxc_uiCatalog;
  hxc_l_gc_roots[21] = (const void *)hxc_l_tmp_class_field_load_result_n20;
  struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_uiCatalog = hxc_l_tmp_class_field_load_result_n20;
  struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_tmp_load_result_n21 = hxc_l_contentRegistry;
  hxc_l_gc_roots[22] = (const void *)hxc_l_tmp_load_result_n21;
  struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_tmp_load_result_n22 = hxc_l_uiCatalog;
  hxc_l_gc_roots[23] = (const void *)hxc_l_tmp_load_result_n22;
  struct hxc_caxecraft_app_CaxecraftEditorScreen *hxc_l_tmp_managed_class_result_n23 = NULL;
  if (hxc_gc_allocate(&hxc_program_gc, &hxc_caxecraft_app_CaxecraftEditorScreen_descriptor, (void **)&hxc_l_tmp_managed_class_result_n23) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_gc_roots[24] = (const void *)hxc_l_tmp_managed_class_result_n23;
  hxc_compiler_constructor_caxecraft_app_CaxecraftEditorScreen(hxc_l_tmp_managed_class_result_n23, hxc_l_tmp_load_result_n21, hxc_l_tmp_load_result_n22);
  hxc_l_gc_roots[25] = (const void *)hxc_l_tmp_managed_class_result_n23;
  struct hxc_caxecraft_app_CaxecraftEditorScreen *hxc_l_editorScreen = hxc_l_tmp_managed_class_result_n23;
  struct hxc_caxecraft_content_RuntimeContentGeneration *hxc_l_tmp_load_result_n24 = hxc_l_runtimeContent;
  hxc_l_gc_roots[26] = (const void *)hxc_l_tmp_load_result_n24;
  if (hxc_l_tmp_load_result_n24 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_content_RuntimeLevelCandidate *hxc_l_tmp_class_field_load_result_n25 = (*hxc_l_tmp_load_result_n24).hxc_loadedLevel;
  hxc_l_gc_roots[27] = (const void *)hxc_l_tmp_class_field_load_result_n25;
  struct hxc_caxecraft_content_RuntimeLevelCandidate *hxc_l_loadedCandidate = hxc_l_tmp_class_field_load_result_n25;
  hxc_l_gc_roots[28] = (const void *)NULL;
  struct hxc_caxecraft_app_ActivePlayableLevel *hxc_l_activeLevel = NULL;
  hxc_l_gc_roots[29] = (const void *)hxc_l_loadedCandidate;
  struct hxc_caxecraft_app_PlayableLevelCreationResult hxc_l_tmp_call_result_n27 = hxc_caxecraft_app_ActivePlayableLevel_create(hxc_l_loadedCandidate);
  hxc_l_gc_roots[30] = hxc_l_tmp_call_result_n27.hxc_tag == hxc_caxecraft_app_PlayableLevelCreationResult_PlayableLevelCreated ? (const void *)hxc_l_tmp_call_result_n27.hxc_payload.hxc_PlayableLevelCreated.hxc_owner : NULL;
  struct hxc_caxecraft_app_PlayableLevelCreationResult hxc_l_g_h1d9290112915 = hxc_l_tmp_call_result_n27;
  hxc_l_gc_roots[31] = hxc_l_g_h1d9290112915.hxc_tag == hxc_caxecraft_app_PlayableLevelCreationResult_PlayableLevelCreated ? (const void *)hxc_l_g_h1d9290112915.hxc_payload.hxc_PlayableLevelCreated.hxc_owner : NULL;
  switch (hxc_l_g_h1d9290112915.hxc_tag) {
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
        hxc_enum_fcb4401b_destroy(&hxc_l_g_h4ef171f4fe67);
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
  hxc_l_gc_roots[32] = hxc_l_g_h1d9290112915.hxc_tag == hxc_caxecraft_app_PlayableLevelCreationResult_PlayableLevelCreated ? (const void *)hxc_l_g_h1d9290112915.hxc_payload.hxc_PlayableLevelCreated.hxc_owner : NULL;
  if (hxc_l_g_h1d9290112915.hxc_tag != hxc_caxecraft_app_PlayableLevelCreationResult_PlayableLevelCreated)
  {
    abort();
  }
  struct hxc_caxecraft_app_ActivePlayableLevel *hxc_l_tmp_enum_payload_project_n30 = hxc_l_g_h1d9290112915.hxc_payload.hxc_PlayableLevelCreated.hxc_owner;
  hxc_l_gc_roots[33] = (const void *)hxc_l_tmp_enum_payload_project_n30;
  struct hxc_caxecraft_app_ActivePlayableLevel *hxc_l_value = hxc_l_tmp_enum_payload_project_n30;
  hxc_l_gc_roots[34] = (const void *)hxc_l_value;
  hxc_l_activeLevel = hxc_l_value;
  hxc_l_gc_roots[35] = (const void *)NULL;
  struct hxc_caxecraft_content_CampaignManifest *hxc_l_campaign = NULL;
  hxc_l_gc_roots[36] = (const void *)NULL;
  struct hxc_caxecraft_content_CampaignLevel *hxc_l_campaignLevel = NULL;
  hxc_l_gc_roots[37] = (const void *)hxc_l_contentStore;
  struct hxc_caxecraft_content_CampaignPackageLoadResult hxc_l_tmp_call_result_n33 = hxc_caxecraft_content_CampaignRuntime_loadCampaignManifest(hxc_l_contentStore, (hxc_string){ (const uint8_t *)"campaigns/first-adventure/campaign.json", 39, true, NULL });
  hxc_l_gc_roots[38] = hxc_l_tmp_call_result_n33.hxc_tag == hxc_caxecraft_content_CampaignPackageLoadResult_CampaignPackageReady ? (const void *)hxc_l_tmp_call_result_n33.hxc_payload.hxc_CampaignPackageReady.hxc_manifest : NULL;
  hxc_l_g_h6904c3060e30 = hxc_l_tmp_call_result_n33;
  hxc_l_gc_roots[39] = hxc_l_g_h6904c3060e30.hxc_tag == hxc_caxecraft_content_CampaignPackageLoadResult_CampaignPackageReady ? (const void *)hxc_l_g_h6904c3060e30.hxc_payload.hxc_CampaignPackageReady.hxc_manifest : NULL;
  switch (hxc_l_g_h6904c3060e30.hxc_tag) {
    case hxc_caxecraft_content_CampaignPackageLoadResult_CampaignPackageReady:
      {
        hxc_l_gc_roots[40] = hxc_l_g_h6904c3060e30.hxc_tag == hxc_caxecraft_content_CampaignPackageLoadResult_CampaignPackageReady ? (const void *)hxc_l_g_h6904c3060e30.hxc_payload.hxc_CampaignPackageReady.hxc_manifest : NULL;
        if (hxc_l_g_h6904c3060e30.hxc_tag != hxc_caxecraft_content_CampaignPackageLoadResult_CampaignPackageReady)
        {
          abort();
        }
        struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_enum_payload_project_n36 = hxc_l_g_h6904c3060e30.hxc_payload.hxc_CampaignPackageReady.hxc_manifest;
        hxc_l_gc_roots[41] = (const void *)hxc_l_tmp_enum_payload_project_n36;
        struct hxc_caxecraft_content_CampaignManifest *hxc_l_manifest = hxc_l_tmp_enum_payload_project_n36;
        struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n37 = hxc_l_manifest;
        hxc_l_gc_roots[42] = (const void *)hxc_l_tmp_load_result_n37;
        if (hxc_l_tmp_load_result_n37 == NULL)
        {
          abort();
        }
        struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n38 = (*hxc_l_tmp_load_result_n37).hxc_admittedLevels;
        hxc_l_gc_roots[43] = (const void *)hxc_l_tmp_class_field_load_result_n38;
        struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n39 = hxc_l_manifest;
        hxc_l_gc_roots[44] = (const void *)hxc_l_tmp_load_result_n39;
        if (hxc_l_tmp_load_result_n39 == NULL)
        {
          abort();
        }
        int32_t hxc_l_tmp_class_field_load_result_n40 = (*hxc_l_tmp_load_result_n39).hxc_entryIndexValue;
        struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_array_get_result_n41;
        if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n38, (size_t)hxc_l_tmp_class_field_load_result_n40, &hxc_l_tmp_array_get_result_n41) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_l_gc_roots[45] = (const void *)hxc_l_tmp_array_get_result_n41;
        struct hxc_caxecraft_content_CampaignLevel *hxc_l_entry = hxc_l_tmp_array_get_result_n41;
        struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_load_result_n42 = hxc_l_entry;
        hxc_l_gc_roots[46] = (const void *)hxc_l_tmp_load_result_n42;
        if (hxc_l_tmp_load_result_n42 == NULL)
        {
          abort();
        }
        hxc_string hxc_l_tmp_class_field_load_result_n43 = (*hxc_l_tmp_load_result_n42).hxc_logicalPath;
        struct hxc_caxecraft_content_RuntimeLevelCandidate *hxc_l_tmp_load_result_n44 = hxc_l_loadedCandidate;
        hxc_l_gc_roots[47] = (const void *)hxc_l_tmp_load_result_n44;
        if (hxc_l_tmp_load_result_n44 == NULL)
        {
          abort();
        }
        struct hxc_caxecraft_content_RuntimeLevelReceipt hxc_l_tmp_instance_call_result_n45 = hxc_caxecraft_content_RuntimeLevelCandidate_receipt(hxc_l_tmp_load_result_n44);
        hxc_l_tmp_record_field_logicalPath_receiver_owner_n22 = hxc_l_tmp_instance_call_result_n45;
        struct hxc_caxecraft_content_RuntimeLevelReceipt hxc_l_tmp_record_field_logicalPath_receiver_borrow_result_n46 = hxc_l_tmp_record_field_logicalPath_receiver_owner_n22;
        hxc_record_f6127605_destroy(&hxc_l_tmp_record_field_logicalPath_receiver_owner_n22);
        if (!(hxc_l_tmp_class_field_load_result_n43.data == NULL || hxc_l_tmp_record_field_logicalPath_receiver_borrow_result_n46.hxc_logicalPath.data == NULL ? hxc_l_tmp_class_field_load_result_n43.data == hxc_l_tmp_record_field_logicalPath_receiver_borrow_result_n46.hxc_logicalPath.data : hxc_l_tmp_class_field_load_result_n43.byte_length == hxc_l_tmp_record_field_logicalPath_receiver_borrow_result_n46.hxc_logicalPath.byte_length && (hxc_l_tmp_class_field_load_result_n43.byte_length == 0 || memcmp(hxc_l_tmp_class_field_load_result_n43.data, hxc_l_tmp_record_field_logicalPath_receiver_borrow_result_n46.hxc_logicalPath.data, hxc_l_tmp_class_field_load_result_n43.byte_length) == 0)))
        {
          if (hxc_io_println((hxc_string){ (const uint8_t *)"caxecraft: campaign entry does not match the active map", 55, true, NULL }) != HXC_STATUS_OK)
          {
            abort();
          }
          hxc_enum_4b68fff2_destroy(&hxc_l_g_h6904c3060e30);
          hxc_enum_fcb4401b_destroy(&hxc_l_g_h4ef171f4fe67);
          if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
          {
            abort();
          }
          return;
        }
        else
        {
          hxc_l_gc_roots[49] = (const void *)hxc_l_manifest;
          hxc_l_campaign = hxc_l_manifest;
          hxc_l_gc_roots[50] = (const void *)hxc_l_entry;
          hxc_l_campaignLevel = hxc_l_entry;
        }
        break;
      }
    case hxc_caxecraft_content_CampaignPackageLoadResult_CampaignPackageRejected:
      {
        hxc_l_gc_roots[48] = hxc_l_g_h6904c3060e30.hxc_tag == hxc_caxecraft_content_CampaignPackageLoadResult_CampaignPackageReady ? (const void *)hxc_l_g_h6904c3060e30.hxc_payload.hxc_CampaignPackageReady.hxc_manifest : NULL;
        if (hxc_l_g_h6904c3060e30.hxc_tag != hxc_caxecraft_content_CampaignPackageLoadResult_CampaignPackageRejected)
        {
          abort();
        }
        struct hxc_caxecraft_content_CampaignPackageLoadError hxc_l_tmp_enum_payload_project_n52 = hxc_l_g_h6904c3060e30.hxc_payload.hxc_CampaignPackageRejected.hxc_error;
        hxc_l_gerror = hxc_l_tmp_enum_payload_project_n52;
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
          struct hxc_caxecraft_content_ContentPackageError hxc_l_tmp_enum_payload_project_n55 = hxc_l_gerror.hxc_payload.hxc_CampaignManifestSourceRejected.hxc_error;
          if (hxc_l_tmp_enum_payload_project_n55.hxc_tag == hxc_caxecraft_content_ContentPackageError_EntryMissing)
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
            hxc_enum_4b68fff2_destroy(&hxc_l_g_h6904c3060e30);
            hxc_enum_fcb4401b_destroy(&hxc_l_g_h4ef171f4fe67);
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
          hxc_enum_4b68fff2_destroy(&hxc_l_g_h6904c3060e30);
          hxc_enum_fcb4401b_destroy(&hxc_l_g_h4ef171f4fe67);
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
  struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_load_result_n56 = hxc_l_campaignLevel;
  hxc_l_gc_roots[51] = (const void *)hxc_l_tmp_load_result_n56;
  hxc_l_gc_roots[52] = (const void *)NULL;
  hxc_string hxc_l_tmp_conditional_managed_result_n25 = { 0 };
  if (hxc_l_tmp_load_result_n56 == NULL)
  {
    struct hxc_caxecraft_content_RuntimeLevelCandidate *hxc_l_tmp_load_result_n57 = hxc_l_loadedCandidate;
    hxc_l_gc_roots[53] = (const void *)hxc_l_tmp_load_result_n57;
    if (hxc_l_tmp_load_result_n57 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_content_RuntimeLevelReceipt hxc_l_tmp_instance_call_result_n58 = hxc_caxecraft_content_RuntimeLevelCandidate_receipt(hxc_l_tmp_load_result_n57);
    hxc_l_tmp_record_field_logicalPath_receiver_owner_n26 = hxc_l_tmp_instance_call_result_n58;
    (void)hxc_l_tmp_record_field_logicalPath_receiver_owner_n26.hxc_logicalPath;
    hxc_l_tmp_conditional_managed_result_n25 = hxc_l_tmp_record_field_logicalPath_receiver_owner_n26.hxc_logicalPath;
    if (hxc_string_retain(hxc_l_tmp_conditional_managed_result_n25) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_record_f6127605_destroy(&hxc_l_tmp_record_field_logicalPath_receiver_owner_n26);
  }
  else
  {
    struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_load_result_n61 = hxc_l_campaignLevel;
    hxc_l_gc_roots[54] = (const void *)hxc_l_tmp_load_result_n61;
    if (hxc_l_tmp_load_result_n61 == NULL)
    {
      abort();
    }
    hxc_string hxc_l_tmp_class_field_load_result_n62 = (*hxc_l_tmp_load_result_n61).hxc_id;
    (void)hxc_l_tmp_class_field_load_result_n62;
    hxc_l_tmp_conditional_managed_result_n25 = hxc_l_tmp_class_field_load_result_n62;
    if (hxc_string_retain(hxc_l_tmp_conditional_managed_result_n25) != HXC_STATUS_OK)
    {
      abort();
    }
  }
  hxc_string hxc_l_tmp_conditional_managed_move_result_n63 = hxc_l_tmp_conditional_managed_result_n25;
  hxc_l_levelLabel = hxc_l_tmp_conditional_managed_move_result_n63;
  struct hxc_caxecraft_app_ActivePlayableLevel *hxc_l_tmp_load_result_n64 = hxc_l_activeLevel;
  hxc_l_gc_roots[55] = (const void *)hxc_l_tmp_load_result_n64;
  if (hxc_l_tmp_load_result_n64 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_class_field_load_result_n65 = (*hxc_l_tmp_load_result_n64).hxc_activeView;
  hxc_l_gc_roots[56] = (const void *)hxc_l_tmp_class_field_load_result_n65;
  struct hxc_caxecraft_app_PlayableLevelView *hxc_l_initialLevel = hxc_l_tmp_class_field_load_result_n65;
  struct hxc_caxecraft_app_ActivePlayableLevel *hxc_l_tmp_load_result_n66 = hxc_l_activeLevel;
  hxc_l_gc_roots[57] = (const void *)hxc_l_tmp_load_result_n66;
  if (hxc_l_tmp_load_result_n66 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_content_ActiveContent *hxc_l_tmp_class_field_load_result_n67 = (*hxc_l_tmp_load_result_n66).hxc_content;
  hxc_l_gc_roots[58] = (const void *)hxc_l_tmp_class_field_load_result_n67;
  if (hxc_l_tmp_class_field_load_result_n67 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_content_LoadedContentGeneration *hxc_l_tmp_class_field_load_result_n68 = (*hxc_l_tmp_class_field_load_result_n67).hxc_active;
  hxc_l_gc_roots[59] = (const void *)hxc_l_tmp_class_field_load_result_n68;
  if (hxc_l_tmp_class_field_load_result_n68 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_class_field_load_result_n69 = (*hxc_l_tmp_class_field_load_result_n68).hxc_ownedSession;
  hxc_l_gc_roots[60] = (const void *)hxc_l_tmp_class_field_load_result_n69;
  struct hxc_caxecraft_domain_GameSession *hxc_l_initialSession = hxc_l_tmp_class_field_load_result_n69;
  struct hxc_caxecraft_content_RuntimeLevelCandidate *hxc_l_tmp_load_result_n70 = hxc_l_loadedCandidate;
  hxc_l_gc_roots[61] = (const void *)hxc_l_tmp_load_result_n70;
  if (hxc_l_tmp_load_result_n70 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_content_RuntimeLevelReceipt hxc_l_tmp_instance_call_result_n71 = hxc_caxecraft_content_RuntimeLevelCandidate_receipt(hxc_l_tmp_load_result_n70);
  hxc_l_receipt = hxc_l_tmp_instance_call_result_n71;
  if (hxc_io_println((hxc_string){ (const uint8_t *)"caxecraft: content-source=runtime-package", 41, true, NULL }) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_string hxc_l_tmp_string_concat_result_n73 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
  if (hxc_string_concat_ref((hxc_string){ (const uint8_t *)"caxecraft: content-path=", 24, true, NULL }, hxc_l_receipt.hxc_logicalPath, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n73) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_tmp_sys_println_string_argument_owner_n30 = hxc_l_tmp_string_concat_result_n73;
  if (hxc_io_println(hxc_l_tmp_sys_println_string_argument_owner_n30) != HXC_STATUS_OK)
  {
    if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n30) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_record_f6127605_destroy(&hxc_l_receipt);
    if (hxc_string_release(&hxc_l_levelLabel) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_enum_4b68fff2_destroy(&hxc_l_g_h6904c3060e30);
    hxc_enum_fcb4401b_destroy(&hxc_l_g_h4ef171f4fe67);
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    abort();
  }
  if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n30) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_string hxc_l_tmp_std_string_int_result_n76 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
  if (hxc_string_from_int32(hxc_l_receipt.hxc_inputHash, hxc_default_allocator(), &hxc_l_tmp_std_string_int_result_n76) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_tmp_string_concat_right_owner_n31 = hxc_l_tmp_std_string_int_result_n76;
  hxc_string hxc_l_tmp_string_concat_result_n78 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
  if (hxc_string_concat_ref((hxc_string){ (const uint8_t *)"caxecraft: content-input-hash=", 30, true, NULL }, hxc_l_tmp_string_concat_right_owner_n31, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n78) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_tmp_sys_println_string_argument_owner_n32 = hxc_l_tmp_string_concat_result_n78;
  if (hxc_io_println(hxc_l_tmp_sys_println_string_argument_owner_n32) != HXC_STATUS_OK)
  {
    if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n32) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n31) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_record_f6127605_destroy(&hxc_l_receipt);
    if (hxc_string_release(&hxc_l_levelLabel) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_enum_4b68fff2_destroy(&hxc_l_g_h6904c3060e30);
    hxc_enum_fcb4401b_destroy(&hxc_l_g_h4ef171f4fe67);
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    abort();
  }
  if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n32) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n31) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_caxecraft_content_RuntimeContentGeneration *hxc_l_tmp_load_result_n80 = hxc_l_runtimeContent;
  hxc_l_gc_roots[62] = (const void *)hxc_l_tmp_load_result_n80;
  if (hxc_l_tmp_load_result_n80 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_content_RuntimeLevelCandidate *hxc_l_tmp_class_field_load_result_n81 = (*hxc_l_tmp_load_result_n80).hxc_loadedLevel;
  hxc_l_gc_roots[63] = (const void *)hxc_l_tmp_class_field_load_result_n81;
  if (hxc_l_tmp_class_field_load_result_n81 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_content_LoadedContentGeneration *hxc_l_tmp_class_field_load_result_n82 = (*hxc_l_tmp_class_field_load_result_n81).hxc_loadedGeneration;
  hxc_l_gc_roots[64] = (const void *)hxc_l_tmp_class_field_load_result_n82;
  if (hxc_l_tmp_class_field_load_result_n82 == NULL)
  {
    abort();
  }
  int32_t hxc_l_tmp_class_field_load_result_n83 = (*hxc_l_tmp_class_field_load_result_n82).hxc_generationIdValue;
  hxc_string hxc_l_tmp_std_string_int_result_n84 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
  if (hxc_string_from_int32(hxc_l_tmp_class_field_load_result_n83, hxc_default_allocator(), &hxc_l_tmp_std_string_int_result_n84) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_tmp_string_concat_right_owner_n33 = hxc_l_tmp_std_string_int_result_n84;
  hxc_string hxc_l_tmp_string_concat_result_n86 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
  if (hxc_string_concat_ref((hxc_string){ (const uint8_t *)"caxecraft: content-generation=", 30, true, NULL }, hxc_l_tmp_string_concat_right_owner_n33, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n86) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_tmp_sys_println_string_argument_owner_n34 = hxc_l_tmp_string_concat_result_n86;
  if (hxc_io_println(hxc_l_tmp_sys_println_string_argument_owner_n34) != HXC_STATUS_OK)
  {
    if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n34) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n33) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_record_f6127605_destroy(&hxc_l_receipt);
    if (hxc_string_release(&hxc_l_levelLabel) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_enum_4b68fff2_destroy(&hxc_l_g_h6904c3060e30);
    hxc_enum_fcb4401b_destroy(&hxc_l_g_h4ef171f4fe67);
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    abort();
  }
  if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n34) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n33) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_windowFlags = hxc_i32_bit_or(64, 4);
  hxc_l_windowFlags = hxc_i32_bit_or(hxc_l_windowFlags, 8192);
  SetConfigFlags((uint32_t)hxc_l_windowFlags);
  InitWindow((int32_t)1280, (int32_t)720, "Caxecraft \342\200\224 Haxe shaped into careful C");
  bool hxc_l_tmp_native_call_result_n90 = IsWindowReady();
  if (!hxc_l_tmp_native_call_result_n90)
  {
    hxc_record_f6127605_destroy(&hxc_l_receipt);
    if (hxc_string_release(&hxc_l_levelLabel) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_enum_4b68fff2_destroy(&hxc_l_g_h6904c3060e30);
    hxc_enum_fcb4401b_destroy(&hxc_l_g_h4ef171f4fe67);
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
  struct Texture hxc_l_tmp_native_call_result_n91 = LoadTexture("assets/showcase/title-panorama.png");
  struct Texture hxc_l_titleTexture = hxc_l_tmp_native_call_result_n91;
  bool hxc_l_tmp_native_call_result_n93 = IsTextureValid(hxc_l_titleTexture);
  bool hxc_l_titleTextureReady = hxc_l_tmp_native_call_result_n93;
  struct Texture hxc_l_tmp_native_call_result_n94 = LoadTexture("assets/branding/caxecraft-wordmark.png");
  struct Texture hxc_l_wordmarkTexture = hxc_l_tmp_native_call_result_n94;
  bool hxc_l_tmp_native_call_result_n96 = IsTextureValid(hxc_l_wordmarkTexture);
  bool hxc_l_wordmarkTextureReady = hxc_l_tmp_native_call_result_n96;
  struct Texture hxc_l_tmp_native_call_result_n97 = LoadTexture("assets/atlases/hud.png");
  struct Texture hxc_l_hudTexture = hxc_l_tmp_native_call_result_n97;
  bool hxc_l_tmp_native_call_result_n99 = IsTextureValid(hxc_l_hudTexture);
  bool hxc_l_hudTextureReady = hxc_l_tmp_native_call_result_n99;
  struct Texture hxc_l_tmp_native_call_result_n100 = LoadTexture("assets/atlases/items.png");
  struct Texture hxc_l_itemTexture = hxc_l_tmp_native_call_result_n100;
  bool hxc_l_tmp_native_call_result_n102 = IsTextureValid(hxc_l_itemTexture);
  bool hxc_l_itemTextureReady = hxc_l_tmp_native_call_result_n102;
  struct Texture hxc_l_tmp_native_call_result_n103 = LoadTexture("assets/atlases/adventure-items.png");
  struct Texture hxc_l_adventureItemTexture = hxc_l_tmp_native_call_result_n103;
  bool hxc_l_tmp_native_call_result_n105 = IsTextureValid(hxc_l_adventureItemTexture);
  bool hxc_l_adventureItemTextureReady = hxc_l_tmp_native_call_result_n105;
  struct Texture hxc_l_tmp_load_result_n106 = hxc_l_hudTexture;
  bool hxc_l_tmp_load_result_n107 = hxc_l_hudTextureReady;
  struct Texture hxc_l_tmp_load_result_n108 = hxc_l_itemTexture;
  bool hxc_l_tmp_load_result_n109 = hxc_l_itemTextureReady;
  struct Texture hxc_l_tmp_load_result_n110 = hxc_l_adventureItemTexture;
  struct hxc_caxecraft_app_HudResources hxc_l_hudResources = (struct hxc_caxecraft_app_HudResources){ .hxc_adventureItemTexture = hxc_l_tmp_load_result_n110, .hxc_adventureItemTextureReady = hxc_l_adventureItemTextureReady, .hxc_hudTexture = hxc_l_tmp_load_result_n106, .hxc_hudTextureReady = hxc_l_tmp_load_result_n107, .hxc_itemTexture = hxc_l_tmp_load_result_n108, .hxc_itemTextureReady = hxc_l_tmp_load_result_n109 };
  struct Texture hxc_l_tmp_native_call_result_n113 = LoadTexture("assets/atlases/entities.png");
  struct Texture hxc_l_entityTexture = hxc_l_tmp_native_call_result_n113;
  bool hxc_l_tmp_native_call_result_n115 = IsTextureValid(hxc_l_entityTexture);
  bool hxc_l_entityTextureReady = hxc_l_tmp_native_call_result_n115;
  struct Texture hxc_l_tmp_native_call_result_n116 = LoadTexture("assets/atlases/terrain.png");
  struct Texture hxc_l_terrainTexture = hxc_l_tmp_native_call_result_n116;
  bool hxc_l_tmp_native_call_result_n118 = IsTextureValid(hxc_l_terrainTexture);
  bool hxc_l_terrainTextureReady = hxc_l_tmp_native_call_result_n118;
  struct Texture hxc_l_tmp_native_call_result_n119 = LoadTexture("assets/atlases/adventure-terrain.png");
  struct Texture hxc_l_adventureTerrainTexture = hxc_l_tmp_native_call_result_n119;
  bool hxc_l_tmp_native_call_result_n121 = IsTextureValid(hxc_l_adventureTerrainTexture);
  bool hxc_l_adventureTerrainTextureReady = hxc_l_tmp_native_call_result_n121;
  double hxc_l_cameraWaterBlend = 0.0;
  struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n122 = hxc_caxecraft_gameplay_Inventory_starter();
  struct hxc_caxecraft_gameplay_InventoryState hxc_l_inventory = hxc_l_tmp_call_result_n122;
  enum hxc_caxecraft_gameplay_GuidePhase hxc_l_guidePhase = hxc_caxecraft_gameplay_GuidePhase_Waiting;
  struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n124 = hxc_l_initialSession;
  hxc_l_gc_roots[65] = (const void *)hxc_l_tmp_load_result_n124;
  if (hxc_l_tmp_load_result_n124 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n125 = hxc_l_initialLevel;
  hxc_l_gc_roots[66] = (const void *)hxc_l_tmp_load_result_n125;
  if (hxc_l_tmp_load_result_n125 == NULL)
  {
    abort();
  }
  int32_t hxc_l_tmp_class_field_load_result_n126 = (*hxc_l_tmp_load_result_n125).hxc_dialogueId;
  bool hxc_l_tmp_instance_call_result_n127 = hxc_caxecraft_domain_GameSession_actorInteractionAvailable(hxc_l_tmp_load_result_n124, hxc_l_tmp_class_field_load_result_n126);
  bool hxc_l_guideInteractionAvailable = hxc_l_tmp_instance_call_result_n127;
  struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n128 = hxc_l_initialSession;
  hxc_l_gc_roots[67] = (const void *)hxc_l_tmp_load_result_n128;
  if (hxc_l_tmp_load_result_n128 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n129 = hxc_l_initialLevel;
  hxc_l_gc_roots[68] = (const void *)hxc_l_tmp_load_result_n129;
  if (hxc_l_tmp_load_result_n129 == NULL)
  {
    abort();
  }
  int32_t hxc_l_tmp_class_field_load_result_n130 = (*hxc_l_tmp_load_result_n129).hxc_dialogueId;
  struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n131 = hxc_caxecraft_domain_GameSession_readCharacter(hxc_l_tmp_load_result_n128, hxc_l_tmp_class_field_load_result_n130);
  struct hxc_caxecraft_domain_Character hxc_l_dialogueActor = hxc_l_tmp_instance_call_result_n131;
  struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n132 = hxc_l_initialSession;
  hxc_l_gc_roots[69] = (const void *)hxc_l_tmp_load_result_n132;
  if (hxc_l_tmp_load_result_n132 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n133 = hxc_l_initialLevel;
  hxc_l_gc_roots[70] = (const void *)hxc_l_tmp_load_result_n133;
  if (hxc_l_tmp_load_result_n133 == NULL)
  {
    abort();
  }
  int32_t hxc_l_tmp_class_field_load_result_n134 = (*hxc_l_tmp_load_result_n133).hxc_enemyId;
  struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n135 = hxc_caxecraft_domain_GameSession_readCharacter(hxc_l_tmp_load_result_n132, hxc_l_tmp_class_field_load_result_n134);
  struct hxc_caxecraft_domain_Character hxc_l_enemyActor = hxc_l_tmp_instance_call_result_n135;
  struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n136 = hxc_l_initialSession;
  hxc_l_gc_roots[71] = (const void *)hxc_l_tmp_load_result_n136;
  if (hxc_l_tmp_load_result_n136 == NULL)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_tmp_instance_call_result_n137 = hxc_caxecraft_domain_GameSession_actorControllerStateSnapshots(hxc_l_tmp_load_result_n136);
  hxc_l_initialActorPhases = hxc_l_tmp_instance_call_result_n137;
  struct hxc_array_ref *hxc_l_tmp_load_result_n138 = hxc_l_initialActorPhases;
  struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n139 = hxc_l_initialLevel;
  hxc_l_gc_roots[72] = (const void *)hxc_l_tmp_load_result_n139;
  if (hxc_l_tmp_load_result_n139 == NULL)
  {
    abort();
  }
  int32_t hxc_l_tmp_class_field_load_result_n140 = (*hxc_l_tmp_load_result_n139).hxc_dialogueId;
  struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation hxc_l_tmp_call_result_n142 = hxc_caxecraft_app_CaxecraftApp_observeActorPhase(hxc_l_tmp_load_result_n138, hxc_l_tmp_class_field_load_result_n140, hxc_caxecraft_domain_ActorControllerPhase_Stationary);
  struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation hxc_l_dialoguePhase = hxc_l_tmp_call_result_n142;
  struct hxc_array_ref *hxc_l_tmp_load_result_n143 = hxc_l_initialActorPhases;
  struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n144 = hxc_l_initialLevel;
  hxc_l_gc_roots[73] = (const void *)hxc_l_tmp_load_result_n144;
  if (hxc_l_tmp_load_result_n144 == NULL)
  {
    abort();
  }
  int32_t hxc_l_tmp_class_field_load_result_n145 = (*hxc_l_tmp_load_result_n144).hxc_enemyId;
  struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation hxc_l_tmp_call_result_n147 = hxc_caxecraft_app_CaxecraftApp_observeActorPhase(hxc_l_tmp_load_result_n143, hxc_l_tmp_class_field_load_result_n145, hxc_caxecraft_domain_ActorControllerPhase_Resting);
  struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation hxc_l_enemyPhase = hxc_l_tmp_call_result_n147;
  int32_t hxc_l_tmp_record_field_load_result_n148 = hxc_l_dialogueActor.hxc_id;
  bool hxc_l_tmp_short_circuit_result_n62 = hxc_l_tmp_record_field_load_result_n148 <= 0;
  if (!(hxc_l_tmp_record_field_load_result_n148 <= 0))
  {
    hxc_l_tmp_short_circuit_result_n62 = hxc_l_enemyActor.hxc_id <= 0;
  }
  bool hxc_l_tmp_short_circuit_load_result_n150 = hxc_l_tmp_short_circuit_result_n62;
  bool hxc_l_tmp_short_circuit_result_n63 = hxc_l_tmp_short_circuit_load_result_n150;
  if (!hxc_l_tmp_short_circuit_load_result_n150)
  {
    hxc_l_tmp_short_circuit_result_n63 = !hxc_l_dialoguePhase.hxc_valid;
  }
  bool hxc_l_tmp_short_circuit_load_result_n152 = hxc_l_tmp_short_circuit_result_n63;
  bool hxc_l_tmp_short_circuit_result_n64 = hxc_l_tmp_short_circuit_load_result_n152;
  if (!hxc_l_tmp_short_circuit_load_result_n152)
  {
    hxc_l_tmp_short_circuit_result_n64 = !hxc_l_enemyPhase.hxc_valid;
  }
  if (hxc_l_tmp_short_circuit_result_n64)
  {
    if (hxc_array_ref_release(hxc_l_initialActorPhases) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_record_f6127605_destroy(&hxc_l_receipt);
    if (hxc_string_release(&hxc_l_levelLabel) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_enum_4b68fff2_destroy(&hxc_l_g_h6904c3060e30);
    hxc_enum_fcb4401b_destroy(&hxc_l_g_h4ef171f4fe67);
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return;
  }
  struct hxc_caxecraft_gameplay_SwordCombatState hxc_l_swordCombat = (struct hxc_caxecraft_gameplay_SwordCombatState){ .hxc_cooldownTicks = 0 };
  struct hxc_caxecraft_gameplay_BerryDropState hxc_l_berryDrop = (struct hxc_caxecraft_gameplay_BerryDropState){ .hxc_active = false, .hxc_amount = 0, .hxc_x = 0.0, .hxc_y = 0.0, .hxc_z = 0.0 };
  double hxc_l_lookX = 0.0;
  double hxc_l_lookY = -0.18;
  double hxc_l_lookZ = -1.0;
  double hxc_l_accumulator = 0.0;
  struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n157 = hxc_l_initialSession;
  hxc_l_gc_roots[74] = (const void *)hxc_l_tmp_load_result_n157;
  if (hxc_l_tmp_load_result_n157 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_domain_GameView hxc_l_tmp_instance_call_result_n158 = hxc_caxecraft_domain_GameSession_view(hxc_l_tmp_load_result_n157);
  struct hxc_caxecraft_domain_GameView hxc_l_initialPresentation = hxc_l_tmp_instance_call_result_n158;
  if (!hxc_l_initialPresentation.hxc_valid)
  {
    if (hxc_array_ref_release(hxc_l_initialActorPhases) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_record_f6127605_destroy(&hxc_l_receipt);
    if (hxc_string_release(&hxc_l_levelLabel) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_enum_4b68fff2_destroy(&hxc_l_g_h6904c3060e30);
    hxc_enum_fcb4401b_destroy(&hxc_l_g_h4ef171f4fe67);
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return;
  }
  struct hxc_caxecraft_app_MotionHistory hxc_l_tmp_call_result_n161 = hxc_caxecraft_app_MotionInterpolation_start(hxc_l_initialPresentation.hxc_localPlayer.hxc_body);
  struct hxc_caxecraft_app_MotionHistory hxc_l_motionHistory = hxc_l_tmp_call_result_n161;
  bool hxc_l_jumpQueued = false;
  bool hxc_l_swordQueued = false;
  int32_t hxc_l_selectedMode = 0;
  int32_t hxc_l_locale = 0;
  bool hxc_l_showInitialTitle = true;
  enum hxc_caxecraft_app_AppScreen hxc_l_tmp_call_result_n163 = hxc_caxecraft_app_AppScreen_initialScreen(hxc_l_showInitialTitle);
  enum hxc_caxecraft_app_AppScreen hxc_l_screen = hxc_l_tmp_call_result_n163;
  bool hxc_l_tmp_call_result_n165 = hxc_caxecraft_app_AppScreen_capturesPointer(hxc_l_screen);
  if (hxc_l_tmp_call_result_n165)
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
    bool hxc_l_tmp_load_result_n166 = hxc_l_quit;
    bool hxc_l_tmp_short_circuit_result_n93 = !hxc_l_tmp_load_result_n166;
    if (!hxc_l_tmp_load_result_n166)
    {
      bool hxc_l_tmp_native_call_result_n167 = WindowShouldClose();
      hxc_l_tmp_short_circuit_result_n93 = !hxc_l_tmp_native_call_result_n167;
    }
    if (!hxc_l_tmp_short_circuit_result_n93)
    {
      break;
    }
    struct hxc_caxecraft_app_ActivePlayableLevel *hxc_l_tmp_load_result_n169 = hxc_l_activeLevel;
    hxc_l_gc_roots[75] = (const void *)hxc_l_tmp_load_result_n169;
    if (hxc_l_tmp_load_result_n169 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_class_field_load_result_n170 = (*hxc_l_tmp_load_result_n169).hxc_activeView;
    hxc_l_gc_roots[76] = (const void *)hxc_l_tmp_class_field_load_result_n170;
    struct hxc_caxecraft_app_PlayableLevelView *hxc_l_levelView = hxc_l_tmp_class_field_load_result_n170;
    struct hxc_caxecraft_app_ActivePlayableLevel *hxc_l_tmp_load_result_n171 = hxc_l_activeLevel;
    hxc_l_gc_roots[77] = (const void *)hxc_l_tmp_load_result_n171;
    if (hxc_l_tmp_load_result_n171 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_content_ActiveContent *hxc_l_tmp_class_field_load_result_n172 = (*hxc_l_tmp_load_result_n171).hxc_content;
    hxc_l_gc_roots[78] = (const void *)hxc_l_tmp_class_field_load_result_n172;
    if (hxc_l_tmp_class_field_load_result_n172 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_content_LoadedContentGeneration *hxc_l_tmp_class_field_load_result_n173 = (*hxc_l_tmp_class_field_load_result_n172).hxc_active;
    hxc_l_gc_roots[79] = (const void *)hxc_l_tmp_class_field_load_result_n173;
    if (hxc_l_tmp_class_field_load_result_n173 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_class_field_load_result_n174 = (*hxc_l_tmp_class_field_load_result_n173).hxc_ownedSession;
    hxc_l_gc_roots[80] = (const void *)hxc_l_tmp_class_field_load_result_n174;
    struct hxc_caxecraft_domain_GameSession *hxc_l_session = hxc_l_tmp_class_field_load_result_n174;
    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n175 = hxc_l_session;
    hxc_l_gc_roots[81] = (const void *)hxc_l_tmp_load_result_n175;
    if (hxc_l_tmp_load_result_n175 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_domain_GameView hxc_l_tmp_instance_call_result_n176 = hxc_caxecraft_domain_GameSession_view(hxc_l_tmp_load_result_n175);
    struct hxc_caxecraft_domain_GameView hxc_l_initialView = hxc_l_tmp_instance_call_result_n176;
    if (!hxc_l_initialView.hxc_valid)
    {
      hxc_l_quit = true;
    }
    struct hxc_caxecraft_domain_Character hxc_l_character = hxc_l_initialView.hxc_localPlayer;
    bool hxc_l_recapturedThisFrame = false;
    bool hxc_l_resetMotionThisFrame = false;
    float hxc_l_tmp_native_call_result_n179 = GetFrameTime();
    double hxc_l_frameSeconds = (double)hxc_l_tmp_native_call_result_n179;
    if (hxc_l_frameSeconds > 0.25)
    {
      hxc_l_frameSeconds = 0.25;
    }
    bool hxc_l_tmp_native_call_result_n181 = IsWindowFocused();
    bool hxc_l_focused = hxc_l_tmp_native_call_result_n181;
    bool hxc_l_tmp_call_result_n183 = hxc_caxecraft_app_AppScreen_capturesPointer(hxc_l_screen);
    bool hxc_l_captured_hd1d7fa265ed7 = hxc_l_tmp_call_result_n183;
    bool hxc_l_tmp_call_result_n185 = hxc_caxecraft_app_AppScreen_pausesSimulation(hxc_l_screen);
    bool hxc_l_paused_h4d2df82295b3 = hxc_l_tmp_call_result_n185;
    double hxc_l_forward = 0.0;
    double hxc_l_right = 0.0;
    bool hxc_l_tmp_native_call_result_n186 = IsKeyDown((int32_t)87);
    if (hxc_l_tmp_native_call_result_n186)
    {
      hxc_l_forward = hxc_l_forward + 1.0;
    }
    bool hxc_l_tmp_native_call_result_n188 = IsKeyDown((int32_t)83);
    if (hxc_l_tmp_native_call_result_n188)
    {
      hxc_l_forward = hxc_l_forward - 1.0;
    }
    bool hxc_l_tmp_native_call_result_n190 = IsKeyDown((int32_t)68);
    if (hxc_l_tmp_native_call_result_n190)
    {
      hxc_l_right = hxc_l_right + 1.0;
    }
    bool hxc_l_tmp_native_call_result_n192 = IsKeyDown((int32_t)65);
    if (hxc_l_tmp_native_call_result_n192)
    {
      hxc_l_right = hxc_l_right - 1.0;
    }
    double hxc_l_lookYaw_hd0ddf414c947 = 0.0;
    double hxc_l_lookPitch_h56989d5a00aa = 0.0;
    if (hxc_l_captured_hd1d7fa265ed7)
    {
      struct Vector2 hxc_l_tmp_native_call_result_n195 = GetMouseDelta();
      struct Vector2 hxc_l_mouse = hxc_l_tmp_native_call_result_n195;
      hxc_l_lookYaw_hd0ddf414c947 = -(double)hxc_l_mouse.x * 0.0025;
      hxc_l_lookPitch_h56989d5a00aa = -(double)hxc_l_mouse.y * 0.0025;
    }
    bool hxc_l_tmp_native_call_result_n200 = IsMouseButtonPressed((int32_t)0);
    bool hxc_l_leftPressed = hxc_l_tmp_native_call_result_n200;
    bool hxc_l_tmp_load_result_n201 = hxc_l_captured_hd1d7fa265ed7;
    bool hxc_l_tmp_short_circuit_result_n111 = hxc_l_tmp_load_result_n201;
    if (hxc_l_tmp_load_result_n201)
    {
      hxc_l_tmp_short_circuit_result_n111 = hxc_l_leftPressed;
    }
    bool hxc_l_primaryPressed_hb5974ea2e423 = hxc_l_tmp_short_circuit_result_n111;
    bool hxc_l_tmp_load_result_n204 = hxc_l_captured_hd1d7fa265ed7;
    bool hxc_l_tmp_short_circuit_result_n113 = hxc_l_tmp_load_result_n204;
    if (hxc_l_tmp_load_result_n204)
    {
      bool hxc_l_tmp_native_call_result_n205 = IsMouseButtonPressed((int32_t)1);
      hxc_l_tmp_short_circuit_result_n113 = hxc_l_tmp_native_call_result_n205;
    }
    bool hxc_l_secondaryPressed_h7fa37a6dc3a9 = hxc_l_tmp_short_circuit_result_n113;
    bool hxc_l_tmp_load_result_n207 = hxc_l_captured_hd1d7fa265ed7;
    bool hxc_l_tmp_short_circuit_result_n115 = hxc_l_tmp_load_result_n207;
    if (hxc_l_tmp_load_result_n207)
    {
      bool hxc_l_tmp_native_call_result_n208 = IsKeyPressed((int32_t)69);
      hxc_l_tmp_short_circuit_result_n115 = hxc_l_tmp_native_call_result_n208;
    }
    bool hxc_l_interactPressed_h1eb51d653d5f = hxc_l_tmp_short_circuit_result_n115;
    bool hxc_l_tmp_load_result_n210 = hxc_l_captured_hd1d7fa265ed7;
    bool hxc_l_tmp_short_circuit_result_n117 = hxc_l_tmp_load_result_n210;
    if (hxc_l_tmp_load_result_n210)
    {
      bool hxc_l_tmp_native_call_result_n211 = IsKeyPressed((int32_t)78);
      hxc_l_tmp_short_circuit_result_n117 = hxc_l_tmp_native_call_result_n211;
    }
    bool hxc_l_travelPressed_h8151197e2ad8 = hxc_l_tmp_short_circuit_result_n117;
    int32_t hxc_l_hotbarSelection_h9ff7ff027620 = -1;
    bool hxc_l_tmp_native_call_result_n213 = IsKeyPressed((int32_t)49);
    if (hxc_l_tmp_native_call_result_n213)
    {
      hxc_l_hotbarSelection_h9ff7ff027620 = 0;
    }
    bool hxc_l_tmp_native_call_result_n214 = IsKeyPressed((int32_t)50);
    if (hxc_l_tmp_native_call_result_n214)
    {
      hxc_l_hotbarSelection_h9ff7ff027620 = 1;
    }
    bool hxc_l_tmp_native_call_result_n215 = IsKeyPressed((int32_t)51);
    if (hxc_l_tmp_native_call_result_n215)
    {
      hxc_l_hotbarSelection_h9ff7ff027620 = 2;
    }
    bool hxc_l_tmp_native_call_result_n216 = IsKeyPressed((int32_t)52);
    if (hxc_l_tmp_native_call_result_n216)
    {
      hxc_l_hotbarSelection_h9ff7ff027620 = 3;
    }
    bool hxc_l_tmp_native_call_result_n217 = IsKeyPressed((int32_t)53);
    if (hxc_l_tmp_native_call_result_n217)
    {
      hxc_l_hotbarSelection_h9ff7ff027620 = 4;
    }
    bool hxc_l_tmp_native_call_result_n218 = IsKeyPressed((int32_t)54);
    if (hxc_l_tmp_native_call_result_n218)
    {
      hxc_l_hotbarSelection_h9ff7ff027620 = 5;
    }
    bool hxc_l_tmp_native_call_result_n219 = IsKeyPressed((int32_t)55);
    if (hxc_l_tmp_native_call_result_n219)
    {
      hxc_l_hotbarSelection_h9ff7ff027620 = 6;
    }
    bool hxc_l_tmp_native_call_result_n220 = IsKeyPressed((int32_t)56);
    if (hxc_l_tmp_native_call_result_n220)
    {
      hxc_l_hotbarSelection_h9ff7ff027620 = 7;
    }
    float hxc_l_tmp_native_call_result_n221 = GetMouseWheelMove();
    double hxc_l_wheel = (double)hxc_l_tmp_native_call_result_n221;
    int32_t hxc_l_hotbarCycle_hb18969d04caf = 0;
    if (hxc_l_wheel > 0.0)
    {
      hxc_l_hotbarCycle_hb18969d04caf = -1;
    }
    if (hxc_l_wheel < 0.0)
    {
      hxc_l_hotbarCycle_hb18969d04caf = 1;
    }
    bool hxc_l_tmp_native_call_result_n224 = IsKeyPressed((int32_t)32);
    bool hxc_l_jumpPressed_hf1824f30bbf1 = hxc_l_tmp_native_call_result_n224;
    bool hxc_l_tmp_native_call_result_n225 = IsKeyPressed((int32_t)256);
    bool hxc_l_pausePressed_h88fe5f03f566 = hxc_l_tmp_native_call_result_n225;
    bool hxc_l_tmp_native_call_result_n226 = IsKeyPressed((int32_t)81);
    bool hxc_l_quitPressed_h804051008360 = hxc_l_tmp_native_call_result_n226;
    double hxc_l_frameInput_moveForward = hxc_l_forward;
    double hxc_l_frameInput_moveRight = hxc_l_right;
    double hxc_l_frameInput_lookYaw = hxc_l_lookYaw_hd0ddf414c947;
    double hxc_l_frameInput_lookPitch = hxc_l_lookPitch_h56989d5a00aa;
    bool hxc_l_frameInput_jumpPressed = hxc_l_jumpPressed_hf1824f30bbf1;
    bool hxc_l_tmp_native_call_result_n232 = IsKeyDown((int32_t)340);
    bool hxc_l_frameInput_descendHeld = hxc_l_tmp_native_call_result_n232;
    bool hxc_l_frameInput_primaryPressed = hxc_l_primaryPressed_hb5974ea2e423;
    bool hxc_l_frameInput_secondaryPressed = hxc_l_secondaryPressed_h7fa37a6dc3a9;
    bool hxc_l_frameInput_interactPressed = hxc_l_interactPressed_h1eb51d653d5f;
    bool hxc_l_frameInput_travelPressed = hxc_l_travelPressed_h8151197e2ad8;
    bool hxc_l_frameInput_pausePressed = hxc_l_pausePressed_h88fe5f03f566;
    bool hxc_l_tmp_load_result_n238 = hxc_l_paused_h4d2df82295b3;
    bool hxc_l_tmp_short_circuit_result_n136 = hxc_l_tmp_load_result_n238;
    if (hxc_l_tmp_load_result_n238)
    {
      hxc_l_tmp_short_circuit_result_n136 = hxc_l_leftPressed;
    }
    bool hxc_l_frameInput_capturePressed = hxc_l_tmp_short_circuit_result_n136;
    bool hxc_l_frameInput_quitPressed = hxc_l_quitPressed_h804051008360;
    int32_t hxc_l_frameInput_hotbarSelection = hxc_l_hotbarSelection_h9ff7ff027620;
    int32_t hxc_l_frameInput_hotbarCycle = hxc_l_hotbarCycle_hb18969d04caf;
    double hxc_l_moveForward = hxc_l_frameInput_moveForward;
    double hxc_l_moveRight = hxc_l_frameInput_moveRight;
    double hxc_l_lookYaw_h20af0a5a310b = hxc_l_frameInput_lookYaw;
    double hxc_l_lookPitch_h52b57f063b69 = hxc_l_frameInput_lookPitch;
    bool hxc_l_jumpPressed_had758d2ce3e1 = hxc_l_frameInput_jumpPressed;
    bool hxc_l_primaryPressed_h613d1838f9ef = hxc_l_frameInput_primaryPressed;
    bool hxc_l_secondaryPressed_h0bea1f2c4275 = hxc_l_frameInput_secondaryPressed;
    bool hxc_l_interactPressed_h5771439b2b8f = hxc_l_frameInput_interactPressed;
    bool hxc_l_travelPressed_hf7c84727a510 = hxc_l_frameInput_travelPressed;
    bool hxc_l_pausePressed_h4d3208f579a1 = hxc_l_frameInput_pausePressed;
    bool hxc_l_capturePressed = hxc_l_frameInput_capturePressed;
    bool hxc_l_quitPressed_he27cd15ff447 = hxc_l_frameInput_quitPressed;
    int32_t hxc_l_hotbarSelection_h8c6e6e157570 = hxc_l_frameInput_hotbarSelection;
    int32_t hxc_l_hotbarCycle_h351cf5ac8b18 = hxc_l_frameInput_hotbarCycle;
    bool hxc_l_descendHeld = hxc_l_frameInput_descendHeld;
    bool hxc_l_tmp_load_result_n259 = hxc_l_quit;
    bool hxc_l_tmp_short_circuit_result_n155 = !hxc_l_tmp_load_result_n259;
    if (!hxc_l_tmp_load_result_n259)
    {
      bool hxc_l_tmp_call_result_n261 = hxc_caxecraft_app_AppScreen_isPlaying(hxc_l_screen);
      hxc_l_tmp_short_circuit_result_n155 = hxc_l_tmp_call_result_n261;
    }
    bool hxc_l_tmp_short_circuit_load_result_n262 = hxc_l_tmp_short_circuit_result_n155;
    bool hxc_l_tmp_short_circuit_result_n156 = hxc_l_tmp_short_circuit_load_result_n262;
    if (hxc_l_tmp_short_circuit_load_result_n262)
    {
      hxc_l_tmp_short_circuit_result_n156 = hxc_l_travelPressed_hf7c84727a510;
    }
    if (hxc_l_tmp_short_circuit_result_n156)
    {
      hxc_l_gc_roots[82] = (const void *)hxc_l_campaign;
      struct hxc_caxecraft_content_CampaignManifest *hxc_l_selectedCampaign = hxc_l_campaign;
      hxc_l_gc_roots[83] = (const void *)hxc_l_campaignLevel;
      struct hxc_caxecraft_content_CampaignLevel *hxc_l_sourceLevel = hxc_l_campaignLevel;
      struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n267 = hxc_l_selectedCampaign;
      hxc_l_gc_roots[84] = (const void *)hxc_l_tmp_load_result_n267;
      hxc_l_gc_roots[85] = (const void *)NULL;
      bool hxc_l_tmp_short_circuit_result_n159 = hxc_l_tmp_load_result_n267 != NULL;
      if (hxc_l_tmp_load_result_n267 != NULL)
      {
        hxc_l_gc_roots[89] = (const void *)hxc_l_sourceLevel;
        hxc_l_gc_roots[90] = (const void *)NULL;
        hxc_l_tmp_short_circuit_result_n159 = hxc_l_sourceLevel != NULL;
      }
      if (hxc_l_tmp_short_circuit_result_n159)
      {
        struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n270 = hxc_l_selectedCampaign;
        hxc_l_gc_roots[91] = (const void *)hxc_l_tmp_load_result_n270;
        if (hxc_l_tmp_load_result_n270 == NULL)
        {
          abort();
        }
        struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_load_result_n271 = hxc_l_sourceLevel;
        hxc_l_gc_roots[92] = (const void *)hxc_l_tmp_load_result_n271;
        if (hxc_l_tmp_load_result_n271 == NULL)
        {
          abort();
        }
        hxc_string hxc_l_tmp_class_field_load_result_n272 = (*hxc_l_tmp_load_result_n271).hxc_id;
        struct hxc_caxecraft_content_CampaignTransition *hxc_l_tmp_instance_call_result_n273 = hxc_caxecraft_content_CampaignManifest_unambiguousTransitionFrom(hxc_l_tmp_load_result_n270, hxc_l_tmp_class_field_load_result_n272);
        hxc_l_gc_roots[93] = (const void *)hxc_l_tmp_instance_call_result_n273;
        struct hxc_caxecraft_content_CampaignTransition *hxc_l_transition = hxc_l_tmp_instance_call_result_n273;
        hxc_l_gc_roots[94] = (const void *)hxc_l_transition;
        hxc_l_gc_roots[95] = (const void *)NULL;
        if (hxc_l_transition == NULL)
        {
          struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_load_result_n275 = hxc_l_sourceLevel;
          hxc_l_gc_roots[96] = (const void *)hxc_l_tmp_load_result_n275;
          if (hxc_l_tmp_load_result_n275 == NULL)
          {
            abort();
          }
          hxc_string hxc_l_tmp_class_field_load_result_n276 = (*hxc_l_tmp_load_result_n275).hxc_id;
          hxc_string hxc_l_tmp_string_concat_result_n277 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
          if (hxc_string_concat_ref((hxc_string){ (const uint8_t *)"caxecraft: campaign travel requires exactly one outgoing transition from ", 73, true, NULL }, hxc_l_tmp_class_field_load_result_n276, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n277) != HXC_STATUS_OK)
          {
            abort();
          }
          hxc_l_tmp_sys_println_string_argument_owner_n161 = hxc_l_tmp_string_concat_result_n277;
          if (hxc_io_println(hxc_l_tmp_sys_println_string_argument_owner_n161) != HXC_STATUS_OK)
          {
            if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n161) != HXC_STATUS_OK)
            {
              abort();
            }
            if (hxc_array_ref_release(hxc_l_initialActorPhases) != HXC_STATUS_OK)
            {
              abort();
            }
            hxc_record_f6127605_destroy(&hxc_l_receipt);
            if (hxc_string_release(&hxc_l_levelLabel) != HXC_STATUS_OK)
            {
              abort();
            }
            hxc_enum_4b68fff2_destroy(&hxc_l_g_h6904c3060e30);
            hxc_enum_fcb4401b_destroy(&hxc_l_g_h4ef171f4fe67);
            if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
            {
              abort();
            }
            abort();
          }
          if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n161) != HXC_STATUS_OK)
          {
            abort();
          }
        }
        else
        {
          struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n279 = hxc_l_selectedCampaign;
          hxc_l_gc_roots[97] = (const void *)hxc_l_tmp_load_result_n279;
          if (hxc_l_tmp_load_result_n279 == NULL)
          {
            abort();
          }
          struct hxc_caxecraft_content_CampaignTransition *hxc_l_tmp_load_result_n280 = hxc_l_transition;
          hxc_l_gc_roots[98] = (const void *)hxc_l_tmp_load_result_n280;
          if (hxc_l_tmp_load_result_n280 == NULL)
          {
            abort();
          }
          hxc_string hxc_l_tmp_class_field_load_result_n281 = (*hxc_l_tmp_load_result_n280).hxc_destinationLevel;
          struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_instance_call_result_n282 = hxc_caxecraft_content_CampaignManifest_level(hxc_l_tmp_load_result_n279, hxc_l_tmp_class_field_load_result_n281);
          hxc_l_gc_roots[99] = (const void *)hxc_l_tmp_instance_call_result_n282;
          struct hxc_caxecraft_content_CampaignLevel *hxc_l_destination = hxc_l_tmp_instance_call_result_n282;
          hxc_l_gc_roots[100] = (const void *)hxc_l_destination;
          hxc_l_gc_roots[101] = (const void *)NULL;
          if (hxc_l_destination == NULL)
          {
            if (hxc_io_println((hxc_string){ (const uint8_t *)"caxecraft: campaign destination disappeared after manifest validation", 69, true, NULL }) != HXC_STATUS_OK)
            {
              abort();
            }
          }
          else
          {
            int32_t hxc_l_tmp_record_field_load_result_n284 = hxc_l_character.hxc_id;
            int32_t hxc_l_tmp_record_field_load_result_n285 = hxc_l_character.hxc_vitals.hxc_health;
            struct hxc_caxecraft_content_LevelPlayerOptions hxc_l_playerOptions = (struct hxc_caxecraft_content_LevelPlayerOptions){ .hxc_aquaticProfile = hxc_l_character.hxc_aquaticProfile, .hxc_entityId = hxc_l_tmp_record_field_load_result_n284, .hxc_initialHealth = hxc_l_tmp_record_field_load_result_n285 };
            struct hxc_caxecraft_app_ActivePlayableLevel *hxc_l_tmp_load_result_n288 = hxc_l_activeLevel;
            hxc_l_gc_roots[102] = (const void *)hxc_l_tmp_load_result_n288;
            if (hxc_l_tmp_load_result_n288 == NULL)
            {
              abort();
            }
            struct hxc_caxecraft_content_ActiveContent *hxc_l_tmp_class_field_load_result_n289 = (*hxc_l_tmp_load_result_n288).hxc_content;
            hxc_l_gc_roots[103] = (const void *)hxc_l_tmp_class_field_load_result_n289;
            if (hxc_l_tmp_class_field_load_result_n289 == NULL)
            {
              abort();
            }
            struct hxc_caxecraft_content_LoadedContentGeneration *hxc_l_tmp_class_field_load_result_n290 = (*hxc_l_tmp_class_field_load_result_n289).hxc_active;
            hxc_l_gc_roots[104] = (const void *)hxc_l_tmp_class_field_load_result_n290;
            if (hxc_l_tmp_class_field_load_result_n290 == NULL)
            {
              abort();
            }
            int32_t hxc_l_tmp_class_field_load_result_n291 = (*hxc_l_tmp_class_field_load_result_n290).hxc_generationIdValue;
            int32_t hxc_l_nextGeneration = hxc_i32_add_wrapping(hxc_l_tmp_class_field_load_result_n291, 1);
            struct hxc_caxecraft_content_ContentPackageStore *hxc_l_tmp_load_result_n292 = hxc_l_contentStore;
            hxc_l_gc_roots[105] = (const void *)hxc_l_tmp_load_result_n292;
            struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_load_result_n293 = hxc_l_destination;
            hxc_l_gc_roots[106] = (const void *)hxc_l_tmp_load_result_n293;
            int32_t hxc_l_tmp_load_result_n294 = hxc_l_nextGeneration;
            struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_tmp_load_result_n295 = hxc_l_contentRegistry;
            hxc_l_gc_roots[107] = (const void *)hxc_l_tmp_load_result_n295;
            if (hxc_l_tmp_load_result_n295 == NULL)
            {
              abort();
            }
            struct hxc_compiler_interface_dispatch_caxecraft_scenario_ScenarioContentRegistry_value hxc_l_tmp_interface_value_n296 = (struct hxc_compiler_interface_dispatch_caxecraft_scenario_ScenarioContentRegistry_value){ .object = hxc_l_tmp_load_result_n295, .table = &hxc_itable_compiler_interface_dispatch_caxecraft_content_RuntimeContentRegistry_itable_layout_caxecraf_hb3f384641e1a61bf };
            struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_tmp_load_result_n297 = hxc_l_contentRegistry;
            hxc_l_gc_roots[108] = (const void *)hxc_l_tmp_load_result_n297;
            if (hxc_l_tmp_load_result_n297 == NULL)
            {
              abort();
            }
            struct hxc_compiler_interface_dispatch_caxecraft_content_LevelContentResolver_value hxc_l_tmp_interface_value_n298 = (struct hxc_compiler_interface_dispatch_caxecraft_content_LevelContentResolver_value){ .object = hxc_l_tmp_load_result_n297, .table = &hxc_itable_compiler_interface_dispatch_caxecraft_content_RuntimeContentRegistry_itable_layout_caxecraf_h82ca4f24ca7a7b5a };
            struct hxc_caxecraft_content_CampaignLevelLoadResult hxc_l_tmp_call_result_n300 = hxc_caxecraft_content_CampaignRuntime_loadCampaignLevel(hxc_l_tmp_load_result_n292, hxc_l_tmp_load_result_n293, hxc_l_tmp_load_result_n294, hxc_l_tmp_interface_value_n296, hxc_l_tmp_interface_value_n298, hxc_l_playerOptions);
            hxc_l_gc_roots[109] = hxc_l_tmp_call_result_n300.hxc_tag == hxc_caxecraft_content_CampaignLevelLoadResult_CampaignLevelReady ? (const void *)hxc_l_tmp_call_result_n300.hxc_payload.hxc_CampaignLevelReady.hxc_candidate : NULL;
            hxc_l_g_h06cfb79d5166 = hxc_l_tmp_call_result_n300;
            hxc_l_gc_roots[110] = hxc_l_g_h06cfb79d5166.hxc_tag == hxc_caxecraft_content_CampaignLevelLoadResult_CampaignLevelReady ? (const void *)hxc_l_g_h06cfb79d5166.hxc_payload.hxc_CampaignLevelReady.hxc_candidate : NULL;
            switch (hxc_l_g_h06cfb79d5166.hxc_tag) {
              case hxc_caxecraft_content_CampaignLevelLoadResult_CampaignLevelReady:
                {
                  hxc_l_gc_roots[111] = hxc_l_g_h06cfb79d5166.hxc_tag == hxc_caxecraft_content_CampaignLevelLoadResult_CampaignLevelReady ? (const void *)hxc_l_g_h06cfb79d5166.hxc_payload.hxc_CampaignLevelReady.hxc_candidate : NULL;
                  if (hxc_l_g_h06cfb79d5166.hxc_tag != hxc_caxecraft_content_CampaignLevelLoadResult_CampaignLevelReady)
                  {
                    abort();
                  }
                  struct hxc_caxecraft_content_RuntimeLevelCandidate *hxc_l_tmp_enum_payload_project_n303 = hxc_l_g_h06cfb79d5166.hxc_payload.hxc_CampaignLevelReady.hxc_candidate;
                  hxc_l_gc_roots[112] = (const void *)hxc_l_tmp_enum_payload_project_n303;
                  struct hxc_caxecraft_content_RuntimeLevelCandidate *hxc_l_candidate_h84b8effbdbbf = hxc_l_tmp_enum_payload_project_n303;
                  struct hxc_caxecraft_app_ActivePlayableLevel *hxc_l_tmp_load_result_n304 = hxc_l_activeLevel;
                  hxc_l_gc_roots[113] = (const void *)hxc_l_tmp_load_result_n304;
                  if (hxc_l_tmp_load_result_n304 == NULL)
                  {
                    abort();
                  }
                  hxc_l_gc_roots[114] = (const void *)hxc_l_candidate_h84b8effbdbbf;
                  struct hxc_caxecraft_app_PlayableLevelPublicationResult hxc_l_tmp_instance_call_result_n306 = hxc_caxecraft_app_ActivePlayableLevel_publish(hxc_l_tmp_load_result_n304, hxc_l_candidate_h84b8effbdbbf);
                  struct hxc_caxecraft_app_PlayableLevelPublicationResult hxc_l_g_h72e490060055 = hxc_l_tmp_instance_call_result_n306;
                  switch (hxc_l_g_h72e490060055.hxc_tag) {
                    case hxc_caxecraft_app_PlayableLevelPublicationResult_PlayableLevelPublished:
                      {
                        if (hxc_l_g_h72e490060055.hxc_tag != hxc_caxecraft_app_PlayableLevelPublicationResult_PlayableLevelPublished)
                        {
                          abort();
                        }
                        int32_t hxc_l_tmp_enum_payload_project_n309 = hxc_l_g_h72e490060055.hxc_payload.hxc_PlayableLevelPublished.hxc_active;
                        int32_t hxc_l_selected = hxc_l_tmp_enum_payload_project_n309;
                        hxc_l_gc_roots[116] = (const void *)hxc_l_destination;
                        hxc_l_campaignLevel = hxc_l_destination;
                        struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_load_result_n311 = hxc_l_destination;
                        hxc_l_gc_roots[117] = (const void *)hxc_l_tmp_load_result_n311;
                        if (hxc_l_tmp_load_result_n311 == NULL)
                        {
                          abort();
                        }
                        hxc_string hxc_l_tmp_class_field_load_result_n312 = (*hxc_l_tmp_load_result_n311).hxc_id;
                        hxc_string hxc_l_tmp_string_assignment_replacement_owner_n169 = hxc_l_tmp_class_field_load_result_n312;
                        if (hxc_string_retain(hxc_l_tmp_string_assignment_replacement_owner_n169) != HXC_STATUS_OK)
                        {
                          abort();
                        }
                        hxc_string hxc_l_tmp_string_assignment_replacement_owned_load_result_n313 = hxc_l_tmp_string_assignment_replacement_owner_n169;
                        if (hxc_string_release(&hxc_l_levelLabel) != HXC_STATUS_OK)
                        {
                          abort();
                        }
                        hxc_l_levelLabel = hxc_l_tmp_string_assignment_replacement_owned_load_result_n313;
                        struct hxc_caxecraft_app_ActivePlayableLevel *hxc_l_tmp_load_result_n314 = hxc_l_activeLevel;
                        hxc_l_gc_roots[118] = (const void *)hxc_l_tmp_load_result_n314;
                        if (hxc_l_tmp_load_result_n314 == NULL)
                        {
                          abort();
                        }
                        struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_class_field_load_result_n315 = (*hxc_l_tmp_load_result_n314).hxc_activeView;
                        hxc_l_gc_roots[119] = (const void *)hxc_l_tmp_class_field_load_result_n315;
                        hxc_l_levelView = hxc_l_tmp_class_field_load_result_n315;
                        struct hxc_caxecraft_app_ActivePlayableLevel *hxc_l_tmp_load_result_n316 = hxc_l_activeLevel;
                        hxc_l_gc_roots[120] = (const void *)hxc_l_tmp_load_result_n316;
                        if (hxc_l_tmp_load_result_n316 == NULL)
                        {
                          abort();
                        }
                        struct hxc_caxecraft_content_ActiveContent *hxc_l_tmp_class_field_load_result_n317 = (*hxc_l_tmp_load_result_n316).hxc_content;
                        hxc_l_gc_roots[121] = (const void *)hxc_l_tmp_class_field_load_result_n317;
                        if (hxc_l_tmp_class_field_load_result_n317 == NULL)
                        {
                          abort();
                        }
                        struct hxc_caxecraft_content_LoadedContentGeneration *hxc_l_tmp_class_field_load_result_n318 = (*hxc_l_tmp_class_field_load_result_n317).hxc_active;
                        hxc_l_gc_roots[122] = (const void *)hxc_l_tmp_class_field_load_result_n318;
                        if (hxc_l_tmp_class_field_load_result_n318 == NULL)
                        {
                          abort();
                        }
                        struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_class_field_load_result_n319 = (*hxc_l_tmp_class_field_load_result_n318).hxc_ownedSession;
                        hxc_l_gc_roots[123] = (const void *)hxc_l_tmp_class_field_load_result_n319;
                        hxc_l_session = hxc_l_tmp_class_field_load_result_n319;
                        struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n320 = hxc_l_session;
                        hxc_l_gc_roots[124] = (const void *)hxc_l_tmp_load_result_n320;
                        if (hxc_l_tmp_load_result_n320 == NULL)
                        {
                          abort();
                        }
                        struct hxc_caxecraft_domain_GameView hxc_l_tmp_instance_call_result_n321 = hxc_caxecraft_domain_GameSession_view(hxc_l_tmp_load_result_n320);
                        hxc_l_initialView = hxc_l_tmp_instance_call_result_n321;
                        if (!hxc_l_initialView.hxc_valid)
                        {
                          hxc_l_quit = true;
                        }
                        else
                        {
                          hxc_l_character = hxc_l_initialView.hxc_localPlayer;
                          hxc_l_guidePhase = hxc_caxecraft_gameplay_GuidePhase_Waiting;
                          struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n325 = hxc_l_session;
                          hxc_l_gc_roots[125] = (const void *)hxc_l_tmp_load_result_n325;
                          if (hxc_l_tmp_load_result_n325 == NULL)
                          {
                            abort();
                          }
                          struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n326 = hxc_l_levelView;
                          hxc_l_gc_roots[126] = (const void *)hxc_l_tmp_load_result_n326;
                          if (hxc_l_tmp_load_result_n326 == NULL)
                          {
                            abort();
                          }
                          int32_t hxc_l_tmp_class_field_load_result_n327 = (*hxc_l_tmp_load_result_n326).hxc_dialogueId;
                          bool hxc_l_tmp_instance_call_result_n328 = hxc_caxecraft_domain_GameSession_actorInteractionAvailable(hxc_l_tmp_load_result_n325, hxc_l_tmp_class_field_load_result_n327);
                          hxc_l_guideInteractionAvailable = hxc_l_tmp_instance_call_result_n328;
                          struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n329 = hxc_l_session;
                          hxc_l_gc_roots[127] = (const void *)hxc_l_tmp_load_result_n329;
                          if (hxc_l_tmp_load_result_n329 == NULL)
                          {
                            abort();
                          }
                          struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n330 = hxc_l_levelView;
                          hxc_l_gc_roots[128] = (const void *)hxc_l_tmp_load_result_n330;
                          if (hxc_l_tmp_load_result_n330 == NULL)
                          {
                            abort();
                          }
                          int32_t hxc_l_tmp_class_field_load_result_n331 = (*hxc_l_tmp_load_result_n330).hxc_dialogueId;
                          struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n332 = hxc_caxecraft_domain_GameSession_readCharacter(hxc_l_tmp_load_result_n329, hxc_l_tmp_class_field_load_result_n331);
                          hxc_l_dialogueActor = hxc_l_tmp_instance_call_result_n332;
                          struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n333 = hxc_l_session;
                          hxc_l_gc_roots[129] = (const void *)hxc_l_tmp_load_result_n333;
                          if (hxc_l_tmp_load_result_n333 == NULL)
                          {
                            abort();
                          }
                          struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n334 = hxc_l_levelView;
                          hxc_l_gc_roots[130] = (const void *)hxc_l_tmp_load_result_n334;
                          if (hxc_l_tmp_load_result_n334 == NULL)
                          {
                            abort();
                          }
                          int32_t hxc_l_tmp_class_field_load_result_n335 = (*hxc_l_tmp_load_result_n334).hxc_enemyId;
                          struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n336 = hxc_caxecraft_domain_GameSession_readCharacter(hxc_l_tmp_load_result_n333, hxc_l_tmp_class_field_load_result_n335);
                          hxc_l_enemyActor = hxc_l_tmp_instance_call_result_n336;
                          struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n337 = hxc_l_session;
                          hxc_l_gc_roots[131] = (const void *)hxc_l_tmp_load_result_n337;
                          if (hxc_l_tmp_load_result_n337 == NULL)
                          {
                            abort();
                          }
                          struct hxc_array_ref *hxc_l_tmp_instance_call_result_n338 = hxc_caxecraft_domain_GameSession_actorControllerStateSnapshots(hxc_l_tmp_load_result_n337);
                          hxc_l_phases = hxc_l_tmp_instance_call_result_n338;
                          struct hxc_array_ref *hxc_l_tmp_load_result_n339 = hxc_l_phases;
                          struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n340 = hxc_l_levelView;
                          hxc_l_gc_roots[132] = (const void *)hxc_l_tmp_load_result_n340;
                          if (hxc_l_tmp_load_result_n340 == NULL)
                          {
                            abort();
                          }
                          int32_t hxc_l_tmp_class_field_load_result_n341 = (*hxc_l_tmp_load_result_n340).hxc_dialogueId;
                          struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation hxc_l_tmp_call_result_n343 = hxc_caxecraft_app_CaxecraftApp_observeActorPhase(hxc_l_tmp_load_result_n339, hxc_l_tmp_class_field_load_result_n341, hxc_caxecraft_domain_ActorControllerPhase_Stationary);
                          hxc_l_dialoguePhase = hxc_l_tmp_call_result_n343;
                          struct hxc_array_ref *hxc_l_tmp_load_result_n344 = hxc_l_phases;
                          struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n345 = hxc_l_levelView;
                          hxc_l_gc_roots[133] = (const void *)hxc_l_tmp_load_result_n345;
                          if (hxc_l_tmp_load_result_n345 == NULL)
                          {
                            abort();
                          }
                          int32_t hxc_l_tmp_class_field_load_result_n346 = (*hxc_l_tmp_load_result_n345).hxc_enemyId;
                          struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation hxc_l_tmp_call_result_n348 = hxc_caxecraft_app_CaxecraftApp_observeActorPhase(hxc_l_tmp_load_result_n344, hxc_l_tmp_class_field_load_result_n346, hxc_caxecraft_domain_ActorControllerPhase_Resting);
                          hxc_l_enemyPhase = hxc_l_tmp_call_result_n348;
                          hxc_l_swordCombat = (struct hxc_caxecraft_gameplay_SwordCombatState){ .hxc_cooldownTicks = 0 };
                          hxc_l_berryDrop = (struct hxc_caxecraft_gameplay_BerryDropState){ .hxc_active = false, .hxc_amount = 0, .hxc_x = 0.0, .hxc_y = 0.0, .hxc_z = 0.0 };
                          hxc_l_cameraWaterBlend = 0.0;
                          hxc_l_accumulator = 0.0;
                          struct hxc_caxecraft_app_MotionHistory hxc_l_tmp_call_result_n352 = hxc_caxecraft_app_MotionInterpolation_start(hxc_l_character.hxc_body);
                          hxc_l_motionHistory = hxc_l_tmp_call_result_n352;
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
                          struct hxc_caxecraft_app_TerrainRenderer *hxc_l_tmp_owned_class_field_address_n353 = &(*hxc_l_self).hxc_terrainRenderer;
                          struct hxc_caxecraft_app_TerrainChunkCache *hxc_l_tmp_owned_class_field_address_n354 = &(*hxc_l_tmp_owned_class_field_address_n353).hxc_cache;
                          if (hxc_l_tmp_owned_class_field_address_n354 == NULL)
                          {
                            abort();
                          }
                          hxc_caxecraft_app_TerrainChunkCache_invalidateAll(hxc_l_tmp_owned_class_field_address_n354);
                          hxc_l_resetMotionThisFrame = true;
                          hxc_string hxc_l_tmp_string_concat_result_n356 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
                          if (hxc_string_concat_ref((hxc_string){ (const uint8_t *)"caxecraft: campaign-level=", 26, true, NULL }, hxc_l_levelLabel, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n356) != HXC_STATUS_OK)
                          {
                            abort();
                          }
                          hxc_l_tmp_string_concat_left_owner_n171 = hxc_l_tmp_string_concat_result_n356;
                          hxc_string hxc_l_tmp_string_concat_result_n358 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
                          if (hxc_string_concat_ref(hxc_l_tmp_string_concat_left_owner_n171, (hxc_string){ (const uint8_t *)" generation=", 12, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n358) != HXC_STATUS_OK)
                          {
                            abort();
                          }
                          hxc_l_tmp_string_concat_left_owner_n172 = hxc_l_tmp_string_concat_result_n358;
                          hxc_string hxc_l_tmp_string_concat_left_borrow_result_n359 = hxc_l_tmp_string_concat_left_owner_n172;
                          hxc_string hxc_l_tmp_std_string_int_result_n361 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
                          if (hxc_string_from_int32(hxc_l_selected, hxc_default_allocator(), &hxc_l_tmp_std_string_int_result_n361) != HXC_STATUS_OK)
                          {
                            abort();
                          }
                          hxc_l_tmp_string_concat_right_owner_n173 = hxc_l_tmp_std_string_int_result_n361;
                          hxc_string hxc_l_tmp_string_concat_result_n363 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
                          if (hxc_string_concat_ref(hxc_l_tmp_string_concat_left_borrow_result_n359, hxc_l_tmp_string_concat_right_owner_n173, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n363) != HXC_STATUS_OK)
                          {
                            abort();
                          }
                          hxc_l_tmp_sys_println_string_argument_owner_n174 = hxc_l_tmp_string_concat_result_n363;
                          if (hxc_io_println(hxc_l_tmp_sys_println_string_argument_owner_n174) != HXC_STATUS_OK)
                          {
                            if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n174) != HXC_STATUS_OK)
                            {
                              abort();
                            }
                            if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n173) != HXC_STATUS_OK)
                            {
                              abort();
                            }
                            if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n172) != HXC_STATUS_OK)
                            {
                              abort();
                            }
                            if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n171) != HXC_STATUS_OK)
                            {
                              abort();
                            }
                            if (hxc_array_ref_release(hxc_l_phases) != HXC_STATUS_OK)
                            {
                              abort();
                            }
                            hxc_enum_5543d34a_destroy(&hxc_l_g_h06cfb79d5166);
                            if (hxc_array_ref_release(hxc_l_initialActorPhases) != HXC_STATUS_OK)
                            {
                              abort();
                            }
                            hxc_record_f6127605_destroy(&hxc_l_receipt);
                            if (hxc_string_release(&hxc_l_levelLabel) != HXC_STATUS_OK)
                            {
                              abort();
                            }
                            hxc_enum_4b68fff2_destroy(&hxc_l_g_h6904c3060e30);
                            hxc_enum_fcb4401b_destroy(&hxc_l_g_h4ef171f4fe67);
                            if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
                            {
                              abort();
                            }
                            abort();
                          }
                          if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n174) != HXC_STATUS_OK)
                          {
                            abort();
                          }
                          if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n173) != HXC_STATUS_OK)
                          {
                            abort();
                          }
                          if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n172) != HXC_STATUS_OK)
                          {
                            abort();
                          }
                          if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n171) != HXC_STATUS_OK)
                          {
                            abort();
                          }
                          if (hxc_array_ref_release(hxc_l_phases) != HXC_STATUS_OK)
                          {
                            abort();
                          }
                        }
                        break;
                      }
                    case hxc_caxecraft_app_PlayableLevelPublicationResult_PlayableLevelPublicationRejected:
                      {
                        if (hxc_io_println((hxc_string){ (const uint8_t *)"caxecraft: campaign destination could not replace the active level", 66, true, NULL }) != HXC_STATUS_OK)
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
                  hxc_l_gc_roots[115] = hxc_l_g_h06cfb79d5166.hxc_tag == hxc_caxecraft_content_CampaignLevelLoadResult_CampaignLevelReady ? (const void *)hxc_l_g_h06cfb79d5166.hxc_payload.hxc_CampaignLevelReady.hxc_candidate : NULL;
                  if (hxc_l_g_h06cfb79d5166.hxc_tag != hxc_caxecraft_content_CampaignLevelLoadResult_CampaignLevelRejected)
                  {
                    abort();
                  }
                  struct hxc_caxecraft_content_CampaignLevelLoadError hxc_l_tmp_enum_payload_project_n366 = hxc_l_g_h06cfb79d5166.hxc_payload.hxc_CampaignLevelRejected.hxc_error;
                  hxc_l_error = hxc_l_tmp_enum_payload_project_n366;
                  if (hxc_enum_40863044_retain(&hxc_l_error) != HXC_STATUS_OK)
                  {
                    abort();
                  }
                  hxc_string hxc_l_tmp_call_result_n368 = hxc_caxecraft_app_CaxecraftApp_campaignLevelLoadFailure(hxc_l_error);
                  hxc_l_tmp_string_concat_right_owner_n176 = hxc_l_tmp_call_result_n368;
                  hxc_string hxc_l_tmp_string_concat_result_n370 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
                  if (hxc_string_concat_ref((hxc_string){ (const uint8_t *)"caxecraft: campaign travel rejected: ", 37, true, NULL }, hxc_l_tmp_string_concat_right_owner_n176, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n370) != HXC_STATUS_OK)
                  {
                    abort();
                  }
                  hxc_l_tmp_sys_println_string_argument_owner_n177 = hxc_l_tmp_string_concat_result_n370;
                  if (hxc_io_println(hxc_l_tmp_sys_println_string_argument_owner_n177) != HXC_STATUS_OK)
                  {
                    if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n177) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n176) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    hxc_enum_40863044_destroy(&hxc_l_error);
                    hxc_enum_5543d34a_destroy(&hxc_l_g_h06cfb79d5166);
                    if (hxc_array_ref_release(hxc_l_initialActorPhases) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    hxc_record_f6127605_destroy(&hxc_l_receipt);
                    if (hxc_string_release(&hxc_l_levelLabel) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    hxc_enum_4b68fff2_destroy(&hxc_l_g_h6904c3060e30);
                    hxc_enum_fcb4401b_destroy(&hxc_l_g_h4ef171f4fe67);
                    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    abort();
                  }
                  if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n177) != HXC_STATUS_OK)
                  {
                    abort();
                  }
                  if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n176) != HXC_STATUS_OK)
                  {
                    abort();
                  }
                  hxc_enum_40863044_destroy(&hxc_l_error);
                  break;
                }
              default:
                {
                  abort();
                }
            }
            hxc_enum_5543d34a_destroy(&hxc_l_g_h06cfb79d5166);
          }
        }
      }
    }
    struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n372 = hxc_l_levelView;
    hxc_l_gc_roots[86] = (const void *)hxc_l_tmp_load_result_n372;
    if (hxc_l_tmp_load_result_n372 == NULL)
    {
      abort();
    }
    int32_t hxc_l_tmp_class_field_load_result_n373 = (*hxc_l_tmp_load_result_n372).hxc_dialogueId;
    int32_t hxc_l_dialogueActorId = hxc_l_tmp_class_field_load_result_n373;
    struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n374 = hxc_l_levelView;
    hxc_l_gc_roots[87] = (const void *)hxc_l_tmp_load_result_n374;
    if (hxc_l_tmp_load_result_n374 == NULL)
    {
      abort();
    }
    int32_t hxc_l_tmp_class_field_load_result_n375 = (*hxc_l_tmp_load_result_n374).hxc_enemyId;
    int32_t hxc_l_enemyActorId = hxc_l_tmp_class_field_load_result_n375;
    struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n376 = hxc_l_levelView;
    hxc_l_gc_roots[88] = (const void *)hxc_l_tmp_load_result_n376;
    if (hxc_l_tmp_load_result_n376 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_scenario_ScenarioTransform hxc_l_tmp_class_field_load_result_n377 = (*hxc_l_tmp_load_result_n376).hxc_playerSpawn;
    struct hxc_caxecraft_scenario_ScenarioTransform hxc_l_spawnTransform = hxc_l_tmp_class_field_load_result_n377;
    if (hxc_l_quitPressed_he27cd15ff447)
    {
      hxc_l_quit = true;
    }
    if (hxc_l_hotbarSelection_h8c6e6e157570 >= 0)
    {
      struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_load_result_n380 = hxc_l_inventory;
      struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n382 = hxc_caxecraft_gameplay_Inventory_select(hxc_l_tmp_load_result_n380, hxc_l_hotbarSelection_h8c6e6e157570);
      hxc_l_inventory = hxc_l_tmp_call_result_n382;
    }
    if (hxc_l_hotbarCycle_h351cf5ac8b18 != 0)
    {
      struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_load_result_n384 = hxc_l_inventory;
      struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n386 = hxc_caxecraft_gameplay_Inventory_cycle(hxc_l_tmp_load_result_n384, hxc_l_hotbarCycle_h351cf5ac8b18);
      hxc_l_inventory = hxc_l_tmp_call_result_n386;
    }
    bool hxc_l_tmp_call_result_n388 = hxc_caxecraft_app_AppScreen_isPlaying(hxc_l_screen);
    bool hxc_l_tmp_short_circuit_result_n181 = hxc_l_tmp_call_result_n388;
    if (hxc_l_tmp_call_result_n388)
    {
      hxc_l_tmp_short_circuit_result_n181 = hxc_l_interactPressed_h5771439b2b8f;
    }
    if (hxc_l_tmp_short_circuit_result_n181)
    {
      if (hxc_l_character.hxc_vitals.hxc_health <= 0)
      {
        struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n392 = hxc_l_session;
        hxc_l_gc_roots[134] = (const void *)hxc_l_tmp_load_result_n392;
        if (hxc_l_tmp_load_result_n392 == NULL)
        {
          abort();
        }
        struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n393 = hxc_l_session;
        hxc_l_gc_roots[135] = (const void *)hxc_l_tmp_load_result_n393;
        if (hxc_l_tmp_load_result_n393 == NULL)
        {
          abort();
        }
        size_t hxc_l_tmp_length_n395;
        const uint8_t *hxc_l_tmp_instance_call_result_n394 = hxc_caxecraft_domain_GameSession_worldView(hxc_l_tmp_load_result_n393, &hxc_l_tmp_length_n395);
        struct hxc_caxecraft_domain_CharacterBody hxc_l_tmp_call_result_n397 = hxc_caxecraft_app_CaxecraftApp_spawnPlayer(hxc_l_tmp_instance_call_result_n394, hxc_l_tmp_length_n395, hxc_l_spawnTransform);
        struct hxc_caxecraft_domain_LocalCharacterCommandResult hxc_l_tmp_instance_call_result_n398 = hxc_caxecraft_domain_GameSession_reviveLocalPlayerAt(hxc_l_tmp_load_result_n392, hxc_l_tmp_call_result_n397);
        struct hxc_caxecraft_domain_LocalCharacterCommandResult hxc_l_revival = hxc_l_tmp_instance_call_result_n398;
        hxc_l_character = hxc_l_revival.hxc_character;
        if (!hxc_l_revival.hxc_resolved)
        {
          hxc_l_quit = true;
        }
        else
        {
          hxc_l_cameraWaterBlend = 0.0;
          hxc_l_accumulator = 0.0;
          hxc_l_resetMotionThisFrame = true;
        }
      }
      else
      {
        struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n401 = hxc_l_session;
        hxc_l_gc_roots[136] = (const void *)hxc_l_tmp_load_result_n401;
        if (hxc_l_tmp_load_result_n401 == NULL)
        {
          abort();
        }
        bool hxc_l_tmp_instance_call_result_n403 = hxc_caxecraft_domain_GameSession_actorInteractionAvailable(hxc_l_tmp_load_result_n401, hxc_l_dialogueActorId);
        if (hxc_l_tmp_instance_call_result_n403)
        {
          bool hxc_l_sharesBerries = hxc_l_guidePhase == hxc_caxecraft_gameplay_GuidePhase_Welcomed;
          if (hxc_l_sharesBerries)
          {
            int32_t hxc_l_tmp_call_result_n409 = hxc_caxecraft_gameplay_Inventory_acceptedAmount(hxc_l_inventory, hxc_caxecraft_gameplay_ItemKind_Berries, 2);
            int32_t hxc_l_acceptedGift = hxc_l_tmp_call_result_n409;
            if (hxc_l_acceptedGift == 2)
            {
              struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_load_result_n411 = hxc_l_inventory;
              struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n414 = hxc_caxecraft_gameplay_Inventory_collectItem(hxc_l_tmp_load_result_n411, hxc_caxecraft_gameplay_ItemKind_Berries, hxc_l_acceptedGift);
              hxc_l_inventory = hxc_l_tmp_call_result_n414;
              enum hxc_caxecraft_gameplay_GuidePhase hxc_l_tmp_call_result_n416 = hxc_caxecraft_app_CaxecraftApp_advanceGuidePhase(hxc_l_guidePhase);
              hxc_l_guidePhase = hxc_l_tmp_call_result_n416;
            }
            else
            {
              hxc_l_inventoryFullReason = 1;
              hxc_l_inventoryFullFrames = 90;
            }
          }
          else
          {
            enum hxc_caxecraft_gameplay_GuidePhase hxc_l_tmp_call_result_n418 = hxc_caxecraft_app_CaxecraftApp_advanceGuidePhase(hxc_l_guidePhase);
            hxc_l_guidePhase = hxc_l_tmp_call_result_n418;
          }
        }
      }
    }
    bool hxc_l_tmp_call_result_n420 = hxc_caxecraft_app_AppScreen_showsTitle(hxc_l_screen);
    bool hxc_l_tmp_short_circuit_result_n185 = hxc_l_tmp_call_result_n420;
    if (hxc_l_tmp_call_result_n420)
    {
      hxc_l_tmp_short_circuit_result_n185 = hxc_l_focused;
    }
    if (hxc_l_tmp_short_circuit_result_n185)
    {
      int32_t hxc_l_modeBeforeInput = hxc_l_selectedMode;
      bool hxc_l_tmp_native_call_result_n424 = IsKeyPressed((int32_t)76);
      if (hxc_l_tmp_native_call_result_n424)
      {
        struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_tmp_load_result_n425 = hxc_l_uiCatalog;
        hxc_l_gc_roots[137] = (const void *)hxc_l_tmp_load_result_n425;
        if (hxc_l_tmp_load_result_n425 == NULL)
        {
          abort();
        }
        int32_t hxc_l_tmp_instance_call_result_n427 = hxc_caxecraft_localization_RuntimeUiCatalog_nextLocale(hxc_l_tmp_load_result_n425, hxc_l_locale);
        hxc_l_locale = hxc_l_tmp_instance_call_result_n427;
      }
      bool hxc_l_tmp_native_call_result_n428 = IsKeyPressed((int32_t)265);
      bool hxc_l_tmp_short_circuit_result_n187 = hxc_l_tmp_native_call_result_n428;
      if (!hxc_l_tmp_native_call_result_n428)
      {
        bool hxc_l_tmp_native_call_result_n429 = IsKeyPressed((int32_t)264);
        hxc_l_tmp_short_circuit_result_n187 = hxc_l_tmp_native_call_result_n429;
      }
      if (hxc_l_tmp_short_circuit_result_n187)
      {
        int32_t hxc_l_tmp_load_result_n431 = hxc_l_selectedMode;
        int32_t hxc_l_tmp_conditional_result_n188 = 0;
        if (hxc_l_tmp_load_result_n431 == 0)
        {
          hxc_l_tmp_conditional_result_n188 = 1;
        }
        else
        {
          hxc_l_tmp_conditional_result_n188 = 0;
        }
        hxc_l_selectedMode = hxc_l_tmp_conditional_result_n188;
      }
      struct Vector2 hxc_l_tmp_native_call_result_n433 = GetMousePosition();
      struct Vector2 hxc_l_menuMouse = hxc_l_tmp_native_call_result_n433;
      struct Vector2 hxc_l_tmp_load_result_n434 = hxc_l_menuMouse;
      struct Vector2 hxc_l_tmp_load_result_n436 = hxc_l_menuMouse;
      int32_t hxc_l_tmp_native_call_result_n438 = GetScreenWidth();
      int32_t hxc_l_tmp_native_call_result_n439 = GetScreenHeight();
      int32_t hxc_l_tmp_call_result_n440 = hxc_caxecraft_app_TitleMenu_selectionAt((double)hxc_l_tmp_load_result_n434.x, (double)hxc_l_tmp_load_result_n436.y, (int32_t)hxc_l_tmp_native_call_result_n438, (int32_t)hxc_l_tmp_native_call_result_n439);
      int32_t hxc_l_hovered = hxc_l_tmp_call_result_n440;
      if (hxc_l_hovered == 0)
      {
        hxc_l_selectedMode = 0;
      }
      if (hxc_l_hovered == 1)
      {
        hxc_l_selectedMode = 1;
      }
      int32_t hxc_l_tmp_load_result_n443 = hxc_l_selectedMode;
      if (hxc_l_tmp_load_result_n443 != hxc_l_modeBeforeInput)
      {
        hxc_l_accumulator = 0.0;
        hxc_l_resetMotionThisFrame = true;
      }
      int32_t hxc_l_tmp_load_result_n445 = hxc_l_hovered;
      bool hxc_l_tmp_short_circuit_result_n192 = hxc_l_tmp_load_result_n445 >= 0;
      if (hxc_l_tmp_load_result_n445 >= 0)
      {
        bool hxc_l_tmp_native_call_result_n446 = IsMouseButtonPressed((int32_t)0);
        hxc_l_tmp_short_circuit_result_n192 = hxc_l_tmp_native_call_result_n446;
      }
      bool hxc_l_clickedChoice = hxc_l_tmp_short_circuit_result_n192;
      bool hxc_l_tmp_load_result_n448 = hxc_l_clickedChoice;
      bool hxc_l_tmp_short_circuit_result_n193 = hxc_l_tmp_load_result_n448;
      if (hxc_l_tmp_load_result_n448)
      {
        hxc_l_tmp_short_circuit_result_n193 = hxc_l_hovered == 2;
      }
      if (hxc_l_tmp_short_circuit_result_n193)
      {
        enum hxc_caxecraft_app_AppScreen hxc_l_tmp_call_result_n452 = hxc_caxecraft_app_AppScreen_openEditor(hxc_l_screen);
        hxc_l_screen = hxc_l_tmp_call_result_n452;
        hxc_l_accumulator = 0.0;
        hxc_l_resetMotionThisFrame = true;
        EnableCursor();
      }
      else
      {
        bool hxc_l_tmp_load_result_n453 = hxc_l_clickedChoice;
        bool hxc_l_tmp_short_circuit_result_n194 = hxc_l_tmp_load_result_n453;
        if (!hxc_l_tmp_load_result_n453)
        {
          bool hxc_l_tmp_native_call_result_n454 = IsKeyPressed((int32_t)257);
          hxc_l_tmp_short_circuit_result_n194 = hxc_l_tmp_native_call_result_n454;
        }
        if (hxc_l_tmp_short_circuit_result_n194)
        {
          enum hxc_caxecraft_app_AppScreen hxc_l_tmp_call_result_n457 = hxc_caxecraft_app_AppScreen_startPlaying(hxc_l_screen);
          hxc_l_screen = hxc_l_tmp_call_result_n457;
          hxc_l_recapturedThisFrame = true;
          hxc_l_accumulator = 0.0;
          hxc_l_resetMotionThisFrame = true;
          DisableCursor();
        }
      }
    }
    bool hxc_l_tmp_load_result_n458 = hxc_l_focused;
    bool hxc_l_tmp_short_circuit_result_n195 = !hxc_l_tmp_load_result_n458;
    if (!hxc_l_tmp_load_result_n458)
    {
      bool hxc_l_tmp_call_result_n460 = hxc_caxecraft_app_AppScreen_isPlaying(hxc_l_screen);
      hxc_l_tmp_short_circuit_result_n195 = hxc_l_tmp_call_result_n460;
    }
    if (hxc_l_tmp_short_circuit_result_n195)
    {
      enum hxc_caxecraft_app_AppScreen hxc_l_tmp_call_result_n463 = hxc_caxecraft_app_AppScreen_loseFocus(hxc_l_screen);
      hxc_l_screen = hxc_l_tmp_call_result_n463;
      hxc_l_jumpQueued = false;
      hxc_l_accumulator = 0.0;
      hxc_l_resetMotionThisFrame = true;
      EnableCursor();
    }
    bool hxc_l_tmp_call_result_n465 = hxc_caxecraft_app_AppScreen_showsTitle(hxc_l_screen);
    bool hxc_l_tmp_short_circuit_result_n196 = !hxc_l_tmp_call_result_n465;
    if (!hxc_l_tmp_call_result_n465)
    {
      bool hxc_l_tmp_call_result_n467 = hxc_caxecraft_app_AppScreen_showsEditor(hxc_l_screen);
      hxc_l_tmp_short_circuit_result_n196 = !hxc_l_tmp_call_result_n467;
    }
    bool hxc_l_tmp_short_circuit_load_result_n468 = hxc_l_tmp_short_circuit_result_n196;
    bool hxc_l_tmp_short_circuit_result_n197 = hxc_l_tmp_short_circuit_load_result_n468;
    if (hxc_l_tmp_short_circuit_load_result_n468)
    {
      hxc_l_tmp_short_circuit_result_n197 = hxc_l_focused;
    }
    bool hxc_l_tmp_short_circuit_load_result_n470 = hxc_l_tmp_short_circuit_result_n197;
    bool hxc_l_tmp_short_circuit_result_n198 = hxc_l_tmp_short_circuit_load_result_n470;
    if (hxc_l_tmp_short_circuit_load_result_n470)
    {
      hxc_l_tmp_short_circuit_result_n198 = hxc_l_pausePressed_h4d3208f579a1;
    }
    if (hxc_l_tmp_short_circuit_result_n198)
    {
      enum hxc_caxecraft_app_AppScreen hxc_l_tmp_call_result_n474 = hxc_caxecraft_app_AppScreen_togglePause(hxc_l_screen);
      hxc_l_screen = hxc_l_tmp_call_result_n474;
      hxc_l_accumulator = 0.0;
      hxc_l_resetMotionThisFrame = true;
      bool hxc_l_tmp_call_result_n476 = hxc_caxecraft_app_AppScreen_pausesSimulation(hxc_l_screen);
      if (hxc_l_tmp_call_result_n476)
      {
        hxc_l_jumpQueued = false;
      }
      bool hxc_l_tmp_call_result_n478 = hxc_caxecraft_app_AppScreen_capturesPointer(hxc_l_screen);
      if (hxc_l_tmp_call_result_n478)
      {
        DisableCursor();
      }
      else
      {
        EnableCursor();
      }
    }
    bool hxc_l_tmp_call_result_n480 = hxc_caxecraft_app_AppScreen_showsTitle(hxc_l_screen);
    bool hxc_l_tmp_short_circuit_result_n199 = !hxc_l_tmp_call_result_n480;
    if (!hxc_l_tmp_call_result_n480)
    {
      bool hxc_l_tmp_call_result_n482 = hxc_caxecraft_app_AppScreen_showsEditor(hxc_l_screen);
      hxc_l_tmp_short_circuit_result_n199 = !hxc_l_tmp_call_result_n482;
    }
    bool hxc_l_tmp_short_circuit_load_result_n483 = hxc_l_tmp_short_circuit_result_n199;
    bool hxc_l_tmp_short_circuit_result_n200 = hxc_l_tmp_short_circuit_load_result_n483;
    if (hxc_l_tmp_short_circuit_load_result_n483)
    {
      hxc_l_tmp_short_circuit_result_n200 = hxc_l_focused;
    }
    bool hxc_l_tmp_short_circuit_load_result_n485 = hxc_l_tmp_short_circuit_result_n200;
    bool hxc_l_tmp_short_circuit_result_n201 = hxc_l_tmp_short_circuit_load_result_n485;
    if (hxc_l_tmp_short_circuit_load_result_n485)
    {
      bool hxc_l_tmp_call_result_n487 = hxc_caxecraft_app_AppScreen_pausesSimulation(hxc_l_screen);
      hxc_l_tmp_short_circuit_result_n201 = hxc_l_tmp_call_result_n487;
    }
    bool hxc_l_tmp_short_circuit_load_result_n488 = hxc_l_tmp_short_circuit_result_n201;
    bool hxc_l_tmp_short_circuit_result_n202 = hxc_l_tmp_short_circuit_load_result_n488;
    if (hxc_l_tmp_short_circuit_load_result_n488)
    {
      hxc_l_tmp_short_circuit_result_n202 = hxc_l_capturePressed;
    }
    if (hxc_l_tmp_short_circuit_result_n202)
    {
      enum hxc_caxecraft_app_AppScreen hxc_l_tmp_call_result_n492 = hxc_caxecraft_app_AppScreen_recapture(hxc_l_screen);
      hxc_l_screen = hxc_l_tmp_call_result_n492;
      hxc_l_recapturedThisFrame = true;
      hxc_l_accumulator = 0.0;
      hxc_l_resetMotionThisFrame = true;
      DisableCursor();
    }
    bool hxc_l_tmp_call_result_n494 = hxc_caxecraft_app_AppScreen_showsTitle(hxc_l_screen);
    bool hxc_l_onTitle = hxc_l_tmp_call_result_n494;
    bool hxc_l_tmp_call_result_n496 = hxc_caxecraft_app_AppScreen_showsEditor(hxc_l_screen);
    bool hxc_l_onEditor = hxc_l_tmp_call_result_n496;
    bool hxc_l_tmp_call_result_n498 = hxc_caxecraft_app_AppScreen_pausesSimulation(hxc_l_screen);
    bool hxc_l_paused_h0114c9e4c987 = hxc_l_tmp_call_result_n498;
    bool hxc_l_tmp_call_result_n500 = hxc_caxecraft_app_AppScreen_capturesPointer(hxc_l_screen);
    bool hxc_l_captured_h6c16dc4c7d0a = hxc_l_tmp_call_result_n500;
    int32_t hxc_l_editorNavigationCommand = 0;
    if (hxc_l_onEditor)
    {
      if (hxc_l_self == NULL)
      {
        abort();
      }
      struct hxc_caxecraft_input_NavigationRepeater *hxc_l_tmp_owned_class_field_address_n502 = &(*hxc_l_self).hxc_editorNavigation;
      if (hxc_l_tmp_owned_class_field_address_n502 == NULL)
      {
        abort();
      }
      struct hxc_caxecraft_input_NavigationSample hxc_l_tmp_call_result_n503 = hxc_caxecraft_app_RaylibNavigationInput_samplePrimaryGamepad();
      int32_t hxc_l_tmp_instance_call_result_n505 = hxc_caxecraft_input_NavigationRepeater_advance(hxc_l_tmp_owned_class_field_address_n502, hxc_l_tmp_call_result_n503, hxc_l_frameSeconds);
      hxc_l_editorNavigationCommand = hxc_l_tmp_instance_call_result_n505;
    }
    else
    {
      if (hxc_l_self == NULL)
      {
        abort();
      }
      struct hxc_caxecraft_input_NavigationRepeater *hxc_l_tmp_owned_class_field_address_n506 = &(*hxc_l_self).hxc_editorNavigation;
      if (hxc_l_tmp_owned_class_field_address_n506 == NULL)
      {
        abort();
      }
      hxc_caxecraft_input_NavigationRepeater_release(hxc_l_tmp_owned_class_field_address_n506);
    }
    if (hxc_l_captured_h6c16dc4c7d0a)
    {
      double hxc_l_yawDelta = hxc_l_lookYaw_h20af0a5a310b;
      if (hxc_l_yawDelta > 0.25)
      {
        hxc_l_yawDelta = 0.25;
      }
      if (hxc_l_yawDelta < -0.25)
      {
        hxc_l_yawDelta = -0.25;
      }
      double hxc_l_tmp_load_result_n511 = hxc_l_lookX;
      double hxc_l_tmp_load_result_n512 = hxc_l_yawDelta;
      double hxc_l_candidateX = hxc_l_tmp_load_result_n511 + hxc_l_tmp_load_result_n512 * hxc_l_lookZ;
      double hxc_l_tmp_load_result_n514 = hxc_l_lookZ;
      double hxc_l_tmp_load_result_n515 = hxc_l_yawDelta;
      double hxc_l_candidateZ = hxc_l_tmp_load_result_n514 - hxc_l_tmp_load_result_n515 * hxc_l_lookX;
      double hxc_l_tmp_load_result_n517 = hxc_l_candidateX;
      double hxc_l_tmp_load_result_n518 = hxc_l_candidateX;
      double hxc_l_tmp_load_result_n519 = hxc_l_candidateZ;
      double hxc_l_lengthSquared = hxc_l_tmp_load_result_n517 * hxc_l_tmp_load_result_n518 + hxc_l_tmp_load_result_n519 * hxc_l_candidateZ;
      double hxc_l_normalize = 1.5 - 0.5 * hxc_l_lengthSquared;
      double hxc_l_tmp_load_result_n522 = hxc_l_candidateX;
      hxc_l_lookX = hxc_l_tmp_load_result_n522 * hxc_l_normalize;
      double hxc_l_tmp_load_result_n524 = hxc_l_candidateZ;
      hxc_l_lookZ = hxc_l_tmp_load_result_n524 * hxc_l_normalize;
      double hxc_l_tmp_compound_load_result_n526 = hxc_l_lookY;
      hxc_l_lookY = hxc_l_tmp_compound_load_result_n526 + hxc_l_lookPitch_h52b57f063b69;
      if (hxc_l_lookY > 0.90)
      {
        hxc_l_lookY = 0.90;
      }
      if (hxc_l_lookY < -0.90)
      {
        hxc_l_lookY = -0.90;
      }
      if (hxc_l_jumpPressed_had758d2ce3e1)
      {
        hxc_l_jumpQueued = true;
      }
    }
    bool hxc_l_tmp_load_result_n531 = hxc_l_captured_h6c16dc4c7d0a;
    bool hxc_l_tmp_short_circuit_result_n213 = hxc_l_tmp_load_result_n531;
    if (hxc_l_tmp_load_result_n531)
    {
      hxc_l_tmp_short_circuit_result_n213 = !hxc_l_recapturedThisFrame;
    }
    bool hxc_l_tmp_short_circuit_load_result_n533 = hxc_l_tmp_short_circuit_result_n213;
    bool hxc_l_tmp_short_circuit_result_n214 = hxc_l_tmp_short_circuit_load_result_n533;
    if (hxc_l_tmp_short_circuit_load_result_n533)
    {
      hxc_l_tmp_short_circuit_result_n214 = hxc_l_primaryPressed_h613d1838f9ef;
    }
    bool hxc_l_tmp_short_circuit_load_result_n535 = hxc_l_tmp_short_circuit_result_n214;
    bool hxc_l_tmp_short_circuit_result_n215 = hxc_l_tmp_short_circuit_load_result_n535;
    if (hxc_l_tmp_short_circuit_load_result_n535)
    {
      hxc_l_tmp_short_circuit_result_n215 = hxc_l_selectedMode == 1;
    }
    bool hxc_l_tmp_short_circuit_load_result_n537 = hxc_l_tmp_short_circuit_result_n215;
    bool hxc_l_tmp_short_circuit_result_n216 = hxc_l_tmp_short_circuit_load_result_n537;
    if (hxc_l_tmp_short_circuit_load_result_n537)
    {
      bool hxc_l_tmp_call_result_n540 = hxc_caxecraft_gameplay_Inventory_selectedIs(hxc_l_inventory, hxc_caxecraft_gameplay_ItemKind_CopperSword);
      hxc_l_tmp_short_circuit_result_n216 = hxc_l_tmp_call_result_n540;
    }
    if (hxc_l_tmp_short_circuit_result_n216)
    {
      hxc_l_swordQueued = true;
    }
    if (!hxc_l_paused_h0114c9e4c987)
    {
      double hxc_l_tmp_compound_load_result_n543 = hxc_l_accumulator;
      hxc_l_accumulator = hxc_l_tmp_compound_load_result_n543 + hxc_l_frameSeconds;
    }
    while (1)
    {
      bool hxc_l_tmp_load_result_n545 = hxc_l_paused_h0114c9e4c987;
      bool hxc_l_tmp_short_circuit_result_n217 = !hxc_l_tmp_load_result_n545;
      if (!hxc_l_tmp_load_result_n545)
      {
        hxc_l_tmp_short_circuit_result_n217 = hxc_l_accumulator >= 0.05;
      }
      if (!hxc_l_tmp_short_circuit_result_n217)
      {
        break;
      }
      double hxc_l_tmp_load_result_n548 = hxc_l_moveForward;
      double hxc_l_tmp_load_result_n549 = hxc_l_lookX;
      double hxc_l_tmp_load_result_n550 = hxc_l_moveRight;
      double hxc_l_moveX = hxc_l_tmp_load_result_n548 * hxc_l_tmp_load_result_n549 - hxc_l_tmp_load_result_n550 * hxc_l_lookZ;
      double hxc_l_tmp_load_result_n552 = hxc_l_moveForward;
      double hxc_l_tmp_load_result_n553 = hxc_l_lookZ;
      double hxc_l_tmp_load_result_n554 = hxc_l_moveRight;
      double hxc_l_moveZ = hxc_l_tmp_load_result_n552 * hxc_l_tmp_load_result_n553 + hxc_l_tmp_load_result_n554 * hxc_l_lookX;
      double hxc_l_tmp_load_result_n556 = hxc_l_moveForward;
      bool hxc_l_tmp_short_circuit_result_n220 = hxc_l_tmp_load_result_n556 != 0.0;
      if (hxc_l_tmp_load_result_n556 != 0.0)
      {
        hxc_l_tmp_short_circuit_result_n220 = hxc_l_moveRight != 0.0;
      }
      if (hxc_l_tmp_short_circuit_result_n220)
      {
        hxc_l_moveX = hxc_l_moveX * 0.7071067811865476;
        hxc_l_moveZ = hxc_l_moveZ * 0.7071067811865476;
      }
      int32_t hxc_l_tmp_load_result_n561 = hxc_l_selectedMode;
      int32_t hxc_l_tmp_conditional_result_n222 = 0;
      if (hxc_l_tmp_load_result_n561 == 1)
      {
        hxc_l_tmp_conditional_result_n222 = 1;
      }
      else
      {
        hxc_l_tmp_conditional_result_n222 = 0;
      }
      int32_t hxc_l_damagePolicy = hxc_l_tmp_conditional_result_n222;
      struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n563 = hxc_l_session;
      hxc_l_gc_roots[139] = (const void *)hxc_l_tmp_load_result_n563;
      if (hxc_l_tmp_load_result_n563 == NULL)
      {
        abort();
      }
      double hxc_l_tmp_load_result_n564 = hxc_l_moveX;
      double hxc_l_tmp_load_result_n565 = hxc_l_moveZ;
      bool hxc_l_tmp_load_result_n566 = hxc_l_jumpQueued;
      struct hxc_caxecraft_domain_CharacterIntent hxc_l_tmp_call_result_n568 = hxc_caxecraft_domain_Aquatics_input(hxc_l_tmp_load_result_n564, hxc_l_tmp_load_result_n565, hxc_l_tmp_load_result_n566, hxc_l_descendHeld);
      struct hxc_caxecraft_domain_GameTickResult hxc_l_tmp_instance_call_result_n571 = hxc_caxecraft_domain_GameSession_tick(hxc_l_tmp_load_result_n563, (struct hxc_caxecraft_domain_GameTickInput){ .hxc_damagePolicy = hxc_l_damagePolicy, .hxc_intent = hxc_l_tmp_call_result_n568, .hxc_waterUpdateBudget = 64 });
      struct hxc_caxecraft_domain_GameTickResult hxc_l_gameTick = hxc_l_tmp_instance_call_result_n571;
      hxc_l_character = hxc_l_gameTick.hxc_character;
      if (hxc_l_gameTick.hxc_committed)
      {
        struct hxc_caxecraft_app_MotionHistory hxc_l_tmp_load_result_n574 = hxc_l_motionHistory;
        struct hxc_caxecraft_app_MotionHistory hxc_l_tmp_call_result_n576 = hxc_caxecraft_app_MotionInterpolation_advance(hxc_l_tmp_load_result_n574, hxc_l_character.hxc_body);
        hxc_l_motionHistory = hxc_l_tmp_call_result_n576;
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
          int32_t hxc_l_tmp_load_result_n580 = hxc_l_pickupIndex;
          struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n581 = hxc_l_levelView;
          hxc_l_gc_roots[140] = (const void *)hxc_l_tmp_load_result_n581;
          if (hxc_l_tmp_load_result_n581 == NULL)
          {
            abort();
          }
          struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n582 = (*hxc_l_tmp_load_result_n581).hxc_items;
          int32_t hxc_l_tmp_array_length_result_n583;
          if (hxc_array_ref_length(hxc_l_tmp_class_field_load_result_n582, &hxc_l_tmp_array_length_result_n583) != HXC_STATUS_OK)
          {
            abort();
          }
          if (!(hxc_l_tmp_load_result_n580 < hxc_l_tmp_array_length_result_n583))
          {
            break;
          }
          struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n584 = hxc_l_levelView;
          hxc_l_gc_roots[141] = (const void *)hxc_l_tmp_load_result_n584;
          if (hxc_l_tmp_load_result_n584 == NULL)
          {
            abort();
          }
          struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n585 = (*hxc_l_tmp_load_result_n584).hxc_items;
          struct hxc_caxecraft_app_LoadedWorldItem hxc_l_tmp_array_get_result_n587;
          if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n585, (size_t)hxc_l_pickupIndex, &hxc_l_tmp_array_get_result_n587) != HXC_STATUS_OK)
          {
            abort();
          }
          struct hxc_caxecraft_app_LoadedWorldItem hxc_l_loadedItem = hxc_l_tmp_array_get_result_n587;
          struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n588 = hxc_l_session;
          hxc_l_gc_roots[142] = (const void *)hxc_l_tmp_load_result_n588;
          if (hxc_l_tmp_load_result_n588 == NULL)
          {
            abort();
          }
          bool hxc_l_tmp_instance_call_result_n590 = hxc_caxecraft_domain_GameSession_authoredItemIsActive(hxc_l_tmp_load_result_n588, hxc_l_pickupIndex);
          bool hxc_l_tmp_short_circuit_result_n226 = hxc_l_tmp_instance_call_result_n590;
          if (hxc_l_tmp_instance_call_result_n590)
          {
            double hxc_l_tmp_record_field_load_result_n591 = hxc_l_character.hxc_body.hxc_x;
            double hxc_l_tmp_record_field_load_result_n592 = hxc_l_character.hxc_body.hxc_y;
            double hxc_l_tmp_record_field_load_result_n593 = hxc_l_character.hxc_body.hxc_z;
            int32_t hxc_l_tmp_record_field_load_result_n594 = hxc_l_loadedItem.hxc_xMilli;
            int32_t hxc_l_tmp_record_field_load_result_n595 = hxc_l_loadedItem.hxc_yMilli;
            bool hxc_l_tmp_call_result_n597 = hxc_caxecraft_gameplay_WorldItemPickup_isInRange(hxc_l_tmp_record_field_load_result_n591, hxc_l_tmp_record_field_load_result_n592, hxc_l_tmp_record_field_load_result_n593, hxc_l_tmp_record_field_load_result_n594, hxc_l_tmp_record_field_load_result_n595, hxc_l_loadedItem.hxc_zMilli);
            hxc_l_tmp_short_circuit_result_n226 = hxc_l_tmp_call_result_n597;
          }
          if (hxc_l_tmp_short_circuit_result_n226)
          {
            int32_t hxc_l_itemCode = hxc_l_loadedItem.hxc_storageCode;
            struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_tmp_load_result_n600 = hxc_l_contentRegistry;
            hxc_l_gc_roots[143] = (const void *)hxc_l_tmp_load_result_n600;
            if (hxc_l_tmp_load_result_n600 == NULL)
            {
              abort();
            }
            enum hxc_caxecraft_content_RuntimeItemUseProfile hxc_l_tmp_instance_call_result_n602 = hxc_caxecraft_content_RuntimeContentRegistry_itemUseProfile(hxc_l_tmp_load_result_n600, hxc_l_itemCode);
            bool hxc_l_tmp_short_circuit_result_n228 = hxc_l_tmp_instance_call_result_n602 == hxc_caxecraft_content_RuntimeItemUseProfile_EquipAquatic;
            if (hxc_l_tmp_instance_call_result_n602 == hxc_caxecraft_content_RuntimeItemUseProfile_EquipAquatic)
            {
              struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_tmp_load_result_n604 = hxc_l_contentRegistry;
              hxc_l_gc_roots[144] = (const void *)hxc_l_tmp_load_result_n604;
              if (hxc_l_tmp_load_result_n604 == NULL)
              {
                abort();
              }
              bool hxc_l_tmp_instance_call_result_n606 = hxc_caxecraft_content_RuntimeContentRegistry_itemProvidesAquaticProfile(hxc_l_tmp_load_result_n604, hxc_l_itemCode);
              hxc_l_tmp_short_circuit_result_n228 = hxc_l_tmp_instance_call_result_n606;
            }
            if (hxc_l_tmp_short_circuit_result_n228)
            {
              struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_tmp_load_result_n608 = hxc_l_contentRegistry;
              hxc_l_gc_roots[145] = (const void *)hxc_l_tmp_load_result_n608;
              if (hxc_l_tmp_load_result_n608 == NULL)
              {
                abort();
              }
              struct hxc_caxecraft_domain_AquaticProfile hxc_l_tmp_instance_call_result_n610 = hxc_caxecraft_content_RuntimeContentRegistry_itemAquaticProfile(hxc_l_tmp_load_result_n608, hxc_l_itemCode);
              struct hxc_caxecraft_domain_AquaticProfile hxc_l_replacement = hxc_l_tmp_instance_call_result_n610;
              struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n611 = hxc_l_session;
              hxc_l_gc_roots[146] = (const void *)hxc_l_tmp_load_result_n611;
              if (hxc_l_tmp_load_result_n611 == NULL)
              {
                abort();
              }
              int32_t hxc_l_tmp_load_result_n612 = hxc_l_pickupIndex;
              struct hxc_caxecraft_domain_AuthoredAquaticEquipmentResult hxc_l_tmp_instance_call_result_n614 = hxc_caxecraft_domain_GameSession_collectAuthoredAquaticEquipment(hxc_l_tmp_load_result_n611, hxc_l_tmp_load_result_n612, hxc_l_replacement);
              struct hxc_caxecraft_domain_AuthoredAquaticEquipmentResult hxc_l_equipment = hxc_l_tmp_instance_call_result_n614;
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
          }
          hxc_l_pickupIndex = hxc_i32_add_wrapping(hxc_l_pickupIndex, 1);
        }
      }
      if (hxc_l_selectedMode == 1)
      {
        if (hxc_l_character.hxc_vitals.hxc_health > 0)
        {
          struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n622 = hxc_l_session;
          hxc_l_gc_roots[147] = (const void *)hxc_l_tmp_load_result_n622;
          if (hxc_l_tmp_load_result_n622 == NULL)
          {
            abort();
          }
          int32_t hxc_l_tmp_record_field_load_result_n623 = hxc_l_gameTick.hxc_tickIndex;
          struct hxc_caxecraft_domain_ActorControllerTickResult hxc_l_tmp_instance_call_result_n625 = hxc_caxecraft_domain_GameSession_stepAuthoredActorControllers(hxc_l_tmp_load_result_n622, hxc_l_tmp_record_field_load_result_n623, hxc_l_damagePolicy);
          struct hxc_caxecraft_domain_ActorControllerTickResult hxc_l_actorTick = hxc_l_tmp_instance_call_result_n625;
          switch (hxc_l_actorTick.hxc_status.hxc_tag) {
            case hxc_caxecraft_domain_ActorControllerTickStatus_ControllersAdvanced:
              {
                hxc_l_guideInteractionAvailable = false;
                int32_t hxc_l_g_haf7b7ab9367d = 0;
                struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n627 = hxc_l_session;
                hxc_l_gc_roots[151] = (const void *)hxc_l_tmp_load_result_n627;
                if (hxc_l_tmp_load_result_n627 == NULL)
                {
                  abort();
                }
                struct hxc_array_ref *hxc_l_tmp_instance_call_result_n628 = hxc_caxecraft_domain_GameSession_actorControllerEventSnapshots(hxc_l_tmp_load_result_n627);
                hxc_l_g1 = hxc_l_tmp_instance_call_result_n628;
                while (1)
                {
                  int32_t hxc_l_tmp_load_result_n629 = hxc_l_g_haf7b7ab9367d;
                  int32_t hxc_l_tmp_array_length_result_n631;
                  if (hxc_array_ref_length(hxc_l_g1, &hxc_l_tmp_array_length_result_n631) != HXC_STATUS_OK)
                  {
                    abort();
                  }
                  if (!(hxc_l_tmp_load_result_n629 < hxc_l_tmp_array_length_result_n631))
                  {
                    break;
                  }
                  struct hxc_array_ref *hxc_l_tmp_load_result_n632 = hxc_l_g1;
                  struct hxc_caxecraft_domain_ActorControllerEvent hxc_l_tmp_array_get_result_n634;
                  if (hxc_array_ref_get_copy(hxc_l_tmp_load_result_n632, (size_t)hxc_l_g_haf7b7ab9367d, &hxc_l_tmp_array_get_result_n634) != HXC_STATUS_OK)
                  {
                    abort();
                  }
                  hxc_l_tmp_array_element_owner_n235 = hxc_l_tmp_array_get_result_n634;
                  hxc_l_event = hxc_l_tmp_array_element_owner_n235;
                  if (hxc_enum_8b2a26b8_retain(&hxc_l_event) != HXC_STATUS_OK)
                  {
                    abort();
                  }
                  hxc_l_g_haf7b7ab9367d = hxc_i32_add_wrapping(hxc_l_g_haf7b7ab9367d, 1);
                  switch (hxc_l_event.hxc_tag) {
                    case hxc_caxecraft_domain_ActorControllerEvent_NoControllerEvent:
                      {
                        break;
                      }
                    case hxc_caxecraft_domain_ActorControllerEvent_InteractionAvailable:
                      {
                        if (hxc_l_event.hxc_tag != hxc_caxecraft_domain_ActorControllerEvent_InteractionAvailable)
                        {
                          abort();
                        }
                        int32_t hxc_l_tmp_enum_payload_project_n639 = hxc_l_event.hxc_payload.hxc_InteractionAvailable.hxc_characterId;
                        int32_t hxc_l_source_ha2854d97c699 = hxc_l_tmp_enum_payload_project_n639;
                        int32_t hxc_l_tmp_load_result_n640 = hxc_l_source_ha2854d97c699;
                        if (hxc_l_tmp_load_result_n640 == hxc_l_dialogueActorId)
                        {
                          hxc_l_guideInteractionAvailable = true;
                        }
                        break;
                      }
                    case hxc_caxecraft_domain_ActorControllerEvent_LocalPlayerAttack:
                      {
                        if (hxc_l_event.hxc_tag != hxc_caxecraft_domain_ActorControllerEvent_LocalPlayerAttack)
                        {
                          abort();
                        }
                        int32_t hxc_l_tmp_enum_payload_project_n643 = hxc_l_event.hxc_payload.hxc_LocalPlayerAttack.hxc_characterId;
                        int32_t hxc_l_source_h42f8dd19a7d2 = hxc_l_tmp_enum_payload_project_n643;
                        int32_t hxc_l_tmp_load_result_n644 = hxc_l_source_h42f8dd19a7d2;
                        if (hxc_l_tmp_load_result_n644 == hxc_l_enemyActorId)
                        {
                          hxc_l_enemyAttackFrames = 120;
                        }
                        break;
                      }
                    case hxc_caxecraft_domain_ActorControllerEvent_DropRequested:
                      {
                        if (hxc_l_event.hxc_tag != hxc_caxecraft_domain_ActorControllerEvent_DropRequested)
                        {
                          abort();
                        }
                        int32_t hxc_l_tmp_enum_payload_project_n647 = hxc_l_event.hxc_payload.hxc_DropRequested.hxc_characterId;
                        int32_t hxc_l_source_h98cc5a591ebe = hxc_l_tmp_enum_payload_project_n647;
                        if (hxc_l_event.hxc_tag != hxc_caxecraft_domain_ActorControllerEvent_DropRequested)
                        {
                          abort();
                        }
                        hxc_string hxc_l_tmp_enum_payload_project_n649 = hxc_l_event.hxc_payload.hxc_DropRequested.hxc_drop;
                        hxc_l_drop = hxc_l_tmp_enum_payload_project_n649;
                        if (hxc_string_retain(hxc_l_drop) != HXC_STATUS_OK)
                        {
                          abort();
                        }
                        struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_tmp_load_result_n650 = hxc_l_contentRegistry;
                        hxc_l_gc_roots[152] = (const void *)hxc_l_tmp_load_result_n650;
                        if (hxc_l_tmp_load_result_n650 == NULL)
                        {
                          abort();
                        }
                        int32_t hxc_l_tmp_instance_call_result_n652 = hxc_caxecraft_content_RuntimeContentRegistry_dropQuantityById(hxc_l_tmp_load_result_n650, hxc_l_drop);
                        int32_t hxc_l_quantity = hxc_l_tmp_instance_call_result_n652;
                        struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n653 = hxc_l_session;
                        hxc_l_gc_roots[153] = (const void *)hxc_l_tmp_load_result_n653;
                        if (hxc_l_tmp_load_result_n653 == NULL)
                        {
                          abort();
                        }
                        struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n655 = hxc_caxecraft_domain_GameSession_readCharacter(hxc_l_tmp_load_result_n653, hxc_l_source_h98cc5a591ebe);
                        struct hxc_caxecraft_domain_Character hxc_l_defeatedActor = hxc_l_tmp_instance_call_result_n655;
                        int32_t hxc_l_tmp_load_result_n656 = hxc_l_quantity;
                        bool hxc_l_tmp_short_circuit_result_n242 = hxc_l_tmp_load_result_n656 <= 0;
                        if (!(hxc_l_tmp_load_result_n656 <= 0))
                        {
                          hxc_l_tmp_short_circuit_result_n242 = hxc_l_defeatedActor.hxc_id <= 0;
                        }
                        if (hxc_l_tmp_short_circuit_result_n242)
                        {
                          hxc_l_quit = true;
                        }
                        else
                        {
                          if (!hxc_l_berryDrop.hxc_active)
                          {
                            struct hxc_caxecraft_domain_Character hxc_l_tmp_load_result_n660 = hxc_l_defeatedActor;
                            struct hxc_caxecraft_gameplay_BerryDropState hxc_l_tmp_call_result_n662 = hxc_caxecraft_gameplay_BerryDrop_fromDefeatedCharacter(hxc_l_tmp_load_result_n660, hxc_l_quantity);
                            hxc_l_berryDrop = hxc_l_tmp_call_result_n662;
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
                  hxc_enum_8b2a26b8_destroy(&hxc_l_event);
                  hxc_array_43e9f8ca_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n235);
                }
                if (hxc_array_ref_release(hxc_l_g1) != HXC_STATUS_OK)
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
        struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n663 = hxc_l_session;
        hxc_l_gc_roots[148] = (const void *)hxc_l_tmp_load_result_n663;
        if (hxc_l_tmp_load_result_n663 == NULL)
        {
          abort();
        }
        struct hxc_array_ref *hxc_l_tmp_instance_call_result_n664 = hxc_caxecraft_domain_GameSession_actorControllerStateSnapshots(hxc_l_tmp_load_result_n663);
        hxc_l_actorPhases = hxc_l_tmp_instance_call_result_n664;
        struct hxc_array_ref *hxc_l_tmp_load_result_n665 = hxc_l_actorPhases;
        struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation hxc_l_tmp_call_result_n668 = hxc_caxecraft_app_CaxecraftApp_observeActorPhase(hxc_l_tmp_load_result_n665, hxc_l_dialogueActorId, hxc_caxecraft_domain_ActorControllerPhase_Stationary);
        hxc_l_dialoguePhase = hxc_l_tmp_call_result_n668;
        struct hxc_array_ref *hxc_l_tmp_load_result_n669 = hxc_l_actorPhases;
        struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation hxc_l_tmp_call_result_n672 = hxc_caxecraft_app_CaxecraftApp_observeActorPhase(hxc_l_tmp_load_result_n669, hxc_l_enemyActorId, hxc_caxecraft_domain_ActorControllerPhase_Defeated);
        hxc_l_enemyPhase = hxc_l_tmp_call_result_n672;
        struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n673 = hxc_l_session;
        hxc_l_gc_roots[149] = (const void *)hxc_l_tmp_load_result_n673;
        if (hxc_l_tmp_load_result_n673 == NULL)
        {
          abort();
        }
        struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n675 = hxc_caxecraft_domain_GameSession_readCharacter(hxc_l_tmp_load_result_n673, hxc_l_dialogueActorId);
        hxc_l_dialogueActor = hxc_l_tmp_instance_call_result_n675;
        struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n676 = hxc_l_session;
        hxc_l_gc_roots[150] = (const void *)hxc_l_tmp_load_result_n676;
        if (hxc_l_tmp_load_result_n676 == NULL)
        {
          abort();
        }
        struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n678 = hxc_caxecraft_domain_GameSession_readCharacter(hxc_l_tmp_load_result_n676, hxc_l_enemyActorId);
        hxc_l_enemyActor = hxc_l_tmp_instance_call_result_n678;
        bool hxc_l_tmp_record_field_load_result_n679 = hxc_l_dialoguePhase.hxc_valid;
        bool hxc_l_tmp_short_circuit_result_n244 = !hxc_l_tmp_record_field_load_result_n679;
        if (!!hxc_l_tmp_record_field_load_result_n679)
        {
          hxc_l_tmp_short_circuit_result_n244 = !hxc_l_enemyPhase.hxc_valid;
        }
        bool hxc_l_tmp_short_circuit_load_result_n681 = hxc_l_tmp_short_circuit_result_n244;
        bool hxc_l_tmp_short_circuit_result_n245 = hxc_l_tmp_short_circuit_load_result_n681;
        if (!hxc_l_tmp_short_circuit_load_result_n681)
        {
          hxc_l_tmp_short_circuit_result_n245 = hxc_l_dialogueActor.hxc_id <= 0;
        }
        bool hxc_l_tmp_short_circuit_load_result_n683 = hxc_l_tmp_short_circuit_result_n245;
        bool hxc_l_tmp_short_circuit_result_n246 = hxc_l_tmp_short_circuit_load_result_n683;
        if (!hxc_l_tmp_short_circuit_load_result_n683)
        {
          hxc_l_tmp_short_circuit_result_n246 = hxc_l_enemyActor.hxc_id <= 0;
        }
        if (hxc_l_tmp_short_circuit_result_n246)
        {
          hxc_l_quit = true;
        }
        struct hxc_caxecraft_gameplay_SwordCombatState hxc_l_tmp_call_result_n687 = hxc_caxecraft_gameplay_SwordCombat_step(hxc_l_swordCombat);
        hxc_l_swordCombat = hxc_l_tmp_call_result_n687;
        if (hxc_l_swordQueued)
        {
          struct hxc_caxecraft_gameplay_SwordCombatState hxc_l_tmp_load_result_n689 = hxc_l_swordCombat;
          struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_load_result_n690 = hxc_l_inventory;
          struct hxc_caxecraft_domain_VitalsState hxc_l_tmp_record_field_load_result_n691 = hxc_l_character.hxc_vitals;
          struct hxc_caxecraft_domain_Character hxc_l_tmp_load_result_n692 = hxc_l_enemyActor;
          double hxc_l_tmp_record_field_load_result_n693 = hxc_l_character.hxc_body.hxc_x;
          double hxc_l_tmp_record_field_load_result_n694 = hxc_l_character.hxc_body.hxc_z;
          double hxc_l_tmp_load_result_n695 = hxc_l_lookX;
          int32_t hxc_l_tmp_call_result_n697 = hxc_caxecraft_gameplay_SwordCombat_decide(hxc_l_tmp_load_result_n689, hxc_l_tmp_load_result_n690, hxc_l_tmp_record_field_load_result_n691, hxc_l_tmp_load_result_n692, hxc_l_tmp_record_field_load_result_n693, hxc_l_tmp_record_field_load_result_n694, hxc_l_tmp_load_result_n695, hxc_l_lookZ);
          int32_t hxc_l_swordDecision = hxc_l_tmp_call_result_n697;
          if (hxc_l_swordDecision == 5)
          {
            struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n699 = hxc_l_session;
            hxc_l_gc_roots[154] = (const void *)hxc_l_tmp_load_result_n699;
            if (hxc_l_tmp_load_result_n699 == NULL)
            {
              abort();
            }
            struct hxc_caxecraft_domain_CharacterDamageResult hxc_l_tmp_instance_call_result_n701 = hxc_caxecraft_domain_GameSession_damageCharacter(hxc_l_tmp_load_result_n699, hxc_l_enemyActorId, 1);
            struct hxc_caxecraft_domain_CharacterDamageResult hxc_l_damage = hxc_l_tmp_instance_call_result_n701;
            if (!hxc_l_damage.hxc_resolved)
            {
              hxc_l_quit = true;
            }
            else
            {
              if (hxc_l_damage.hxc_damageApplied > 0)
              {
                hxc_l_enemyActor = hxc_l_damage.hxc_character;
                hxc_l_strikeHitFrames = 16;
              }
            }
          }
          int32_t hxc_l_tmp_load_result_n705 = hxc_l_swordDecision;
          struct hxc_caxecraft_gameplay_SwordCombatState hxc_l_tmp_call_result_n707 = hxc_caxecraft_gameplay_SwordCombat_after(hxc_l_tmp_load_result_n705, hxc_l_swordCombat);
          hxc_l_swordCombat = hxc_l_tmp_call_result_n707;
          hxc_l_swordQueued = false;
        }
        if (hxc_array_ref_release(hxc_l_actorPhases) != HXC_STATUS_OK)
        {
          abort();
        }
      }
      hxc_l_jumpQueued = false;
      hxc_l_accumulator = hxc_l_accumulator - 0.05;
    }
    double hxc_l_selectionEyeX = hxc_l_character.hxc_body.hxc_x;
    double hxc_l_selectionEyeY = hxc_l_character.hxc_body.hxc_y + 1.62;
    double hxc_l_selectionEyeZ = hxc_l_character.hxc_body.hxc_z;
    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n712 = hxc_l_session;
    hxc_l_gc_roots[138] = (const void *)hxc_l_tmp_load_result_n712;
    if (hxc_l_tmp_load_result_n712 == NULL)
    {
      abort();
    }
    size_t hxc_l_tmp_length_n714;
    const uint8_t *hxc_l_tmp_instance_call_result_n713 = hxc_caxecraft_domain_GameSession_worldView(hxc_l_tmp_load_result_n712, &hxc_l_tmp_length_n714);
    double hxc_l_tmp_load_result_n715 = hxc_l_selectionEyeX;
    double hxc_l_tmp_load_result_n716 = hxc_l_selectionEyeY;
    double hxc_l_tmp_load_result_n717 = hxc_l_selectionEyeZ;
    double hxc_l_tmp_load_result_n718 = hxc_l_lookX;
    double hxc_l_tmp_load_result_n719 = hxc_l_lookY;
    struct hxc_caxecraft_domain_RaycastHit hxc_l_tmp_call_result_n721 = hxc_caxecraft_domain_VoxelRaycast_trace(hxc_l_tmp_instance_call_result_n713, hxc_l_tmp_length_n714, hxc_l_tmp_load_result_n715, hxc_l_tmp_load_result_n716, hxc_l_tmp_load_result_n717, hxc_l_tmp_load_result_n718, hxc_l_tmp_load_result_n719, hxc_l_lookZ, 7.0);
    struct hxc_caxecraft_domain_RaycastHit hxc_l_hit = hxc_l_tmp_call_result_n721;
    bool hxc_l_tmp_load_result_n722 = hxc_l_captured_h6c16dc4c7d0a;
    bool hxc_l_tmp_short_circuit_result_n253 = hxc_l_tmp_load_result_n722;
    if (hxc_l_tmp_load_result_n722)
    {
      hxc_l_tmp_short_circuit_result_n253 = !hxc_l_recapturedThisFrame;
    }
    bool hxc_l_tmp_short_circuit_load_result_n724 = hxc_l_tmp_short_circuit_result_n253;
    bool hxc_l_tmp_short_circuit_result_n254 = hxc_l_tmp_short_circuit_load_result_n724;
    if (hxc_l_tmp_short_circuit_load_result_n724)
    {
      hxc_l_tmp_short_circuit_result_n254 = hxc_l_primaryPressed_h613d1838f9ef;
    }
    if (hxc_l_tmp_short_circuit_result_n254)
    {
      if (hxc_l_character.hxc_vitals.hxc_health > 0)
      {
        if (hxc_l_selectedMode == 1)
        {
          bool hxc_l_tmp_call_result_n731 = hxc_caxecraft_gameplay_Inventory_selectedIs(hxc_l_inventory, hxc_caxecraft_gameplay_ItemKind_CopperSword);
          bool hxc_l_tmp_short_circuit_result_n255 = !hxc_l_tmp_call_result_n731;
          if (!hxc_l_tmp_call_result_n731)
          {
            hxc_l_tmp_short_circuit_result_n255 = hxc_l_hit.hxc_hit;
          }
          bool hxc_l_tmp_short_circuit_load_result_n733 = hxc_l_tmp_short_circuit_result_n255;
          bool hxc_l_tmp_short_circuit_result_n256 = hxc_l_tmp_short_circuit_load_result_n733;
          if (hxc_l_tmp_short_circuit_load_result_n733)
          {
            struct hxc_caxecraft_domain_AquaticState hxc_l_tmp_record_field_load_result_n734 = hxc_l_character.hxc_aquatic;
            bool hxc_l_tmp_call_result_n736 = hxc_caxecraft_domain_Aquatics_canMine(hxc_l_tmp_record_field_load_result_n734, hxc_l_character.hxc_aquaticProfile);
            hxc_l_tmp_short_circuit_result_n256 = hxc_l_tmp_call_result_n736;
          }
          if (hxc_l_tmp_short_circuit_result_n256)
          {
            int32_t hxc_l_tmp_record_field_load_result_n738 = hxc_l_hit.hxc_cellX;
            int32_t hxc_l_tmp_record_field_load_result_n739 = hxc_l_hit.hxc_cellY;
            struct hxc_caxecraft_scenario_VoxelPoint hxc_l_tmp_call_result_n741 = hxc_caxecraft_domain_World_coord(hxc_l_tmp_record_field_load_result_n738, hxc_l_tmp_record_field_load_result_n739, hxc_l_hit.hxc_cellZ);
            struct hxc_caxecraft_scenario_VoxelPoint hxc_l_minedCoordinate = hxc_l_tmp_call_result_n741;
            struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n742 = hxc_l_session;
            hxc_l_gc_roots[155] = (const void *)hxc_l_tmp_load_result_n742;
            if (hxc_l_tmp_load_result_n742 == NULL)
            {
              abort();
            }
            struct hxc_caxecraft_scenario_VoxelPoint hxc_l_tmp_load_result_n743 = hxc_l_minedCoordinate;
            struct hxc_caxecraft_gameplay_MiningResult hxc_l_tmp_instance_call_result_n745 = hxc_caxecraft_domain_GameSession_mineTerrain(hxc_l_tmp_load_result_n742, hxc_l_tmp_load_result_n743, hxc_l_inventory);
            struct hxc_caxecraft_gameplay_MiningResult hxc_l_mining = hxc_l_tmp_instance_call_result_n745;
            hxc_l_inventory = hxc_l_mining.hxc_inventory;
            if (hxc_l_mining.hxc_outcome == 2)
            {
              if (hxc_l_self == NULL)
              {
                abort();
              }
              struct hxc_caxecraft_app_TerrainRenderer *hxc_l_tmp_owned_class_field_address_n748 = &(*hxc_l_self).hxc_terrainRenderer;
              struct hxc_caxecraft_app_TerrainChunkCache *hxc_l_tmp_owned_class_field_address_n749 = &(*hxc_l_tmp_owned_class_field_address_n748).hxc_cache;
              if (hxc_l_tmp_owned_class_field_address_n749 == NULL)
              {
                abort();
              }
              hxc_caxecraft_app_TerrainChunkCache_invalidate(hxc_l_tmp_owned_class_field_address_n749, hxc_l_minedCoordinate);
            }
            if (hxc_l_mining.hxc_outcome == 1)
            {
              hxc_l_inventoryFullReason = 2;
              hxc_l_inventoryFullFrames = 90;
            }
          }
        }
        else
        {
          if (hxc_l_hit.hxc_hit)
          {
            int32_t hxc_l_tmp_record_field_load_result_n753 = hxc_l_hit.hxc_cellX;
            int32_t hxc_l_tmp_record_field_load_result_n754 = hxc_l_hit.hxc_cellY;
            struct hxc_caxecraft_scenario_VoxelPoint hxc_l_tmp_call_result_n756 = hxc_caxecraft_domain_World_coord(hxc_l_tmp_record_field_load_result_n753, hxc_l_tmp_record_field_load_result_n754, hxc_l_hit.hxc_cellZ);
            struct hxc_caxecraft_scenario_VoxelPoint hxc_l_removedCoordinate = hxc_l_tmp_call_result_n756;
            struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n757 = hxc_l_session;
            hxc_l_gc_roots[156] = (const void *)hxc_l_tmp_load_result_n757;
            if (hxc_l_tmp_load_result_n757 == NULL)
            {
              abort();
            }
            bool hxc_l_tmp_instance_call_result_n759 = hxc_caxecraft_domain_GameSession_removeTerrain(hxc_l_tmp_load_result_n757, hxc_l_removedCoordinate);
            bool hxc_l_removed = hxc_l_tmp_instance_call_result_n759;
            if (hxc_l_removed)
            {
              if (hxc_l_self == NULL)
              {
                abort();
              }
              struct hxc_caxecraft_app_TerrainRenderer *hxc_l_tmp_owned_class_field_address_n761 = &(*hxc_l_self).hxc_terrainRenderer;
              struct hxc_caxecraft_app_TerrainChunkCache *hxc_l_tmp_owned_class_field_address_n762 = &(*hxc_l_tmp_owned_class_field_address_n761).hxc_cache;
              if (hxc_l_tmp_owned_class_field_address_n762 == NULL)
              {
                abort();
              }
              hxc_caxecraft_app_TerrainChunkCache_invalidate(hxc_l_tmp_owned_class_field_address_n762, hxc_l_removedCoordinate);
            }
          }
        }
      }
    }
    bool hxc_l_tmp_load_result_n764 = hxc_l_captured_h6c16dc4c7d0a;
    bool hxc_l_tmp_short_circuit_result_n261 = hxc_l_tmp_load_result_n764;
    if (hxc_l_tmp_load_result_n764)
    {
      hxc_l_tmp_short_circuit_result_n261 = hxc_l_secondaryPressed_h0bea1f2c4275;
    }
    if (hxc_l_tmp_short_circuit_result_n261)
    {
      if (hxc_l_character.hxc_vitals.hxc_health > 0)
      {
        struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n768 = hxc_l_session;
        hxc_l_gc_roots[157] = (const void *)hxc_l_tmp_load_result_n768;
        if (hxc_l_tmp_load_result_n768 == NULL)
        {
          abort();
        }
        struct hxc_caxecraft_domain_LocalRecoveryResult hxc_l_tmp_instance_call_result_n770 = hxc_caxecraft_domain_GameSession_useSelectedRecovery(hxc_l_tmp_load_result_n768, hxc_l_inventory);
        struct hxc_caxecraft_domain_LocalRecoveryResult hxc_l_recovery = hxc_l_tmp_instance_call_result_n770;
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
              int32_t hxc_l_tmp_record_field_load_result_n777 = hxc_l_hit.hxc_previousX;
              int32_t hxc_l_tmp_record_field_load_result_n778 = hxc_l_hit.hxc_previousY;
              struct hxc_caxecraft_scenario_VoxelPoint hxc_l_tmp_call_result_n780 = hxc_caxecraft_domain_World_coord(hxc_l_tmp_record_field_load_result_n777, hxc_l_tmp_record_field_load_result_n778, hxc_l_hit.hxc_previousZ);
              struct hxc_caxecraft_scenario_VoxelPoint hxc_l_placement = hxc_l_tmp_call_result_n780;
              enum hxc_caxecraft_domain_BlockKind hxc_l_tmp_call_result_n782 = hxc_caxecraft_gameplay_Inventory_selectedBlock(hxc_l_inventory);
              enum hxc_caxecraft_domain_BlockKind hxc_l_selectedBlock = hxc_l_tmp_call_result_n782;
              struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_load_result_n783 = hxc_l_inventory;
              int32_t hxc_l_tmp_call_result_n785 = hxc_caxecraft_gameplay_Inventory_countAt(hxc_l_tmp_load_result_n783, hxc_l_inventory.hxc_selected);
              bool hxc_l_hasItem = hxc_l_tmp_call_result_n785 > 0;
              bool hxc_l_tmp_load_result_n786 = hxc_l_hasItem;
              bool hxc_l_tmp_short_circuit_result_n266 = !hxc_l_tmp_load_result_n786;
              if (!!hxc_l_tmp_load_result_n786)
              {
                bool hxc_l_tmp_call_result_n788 = hxc_caxecraft_domain_World_isPlaceable(hxc_l_selectedBlock);
                hxc_l_tmp_short_circuit_result_n266 = !hxc_l_tmp_call_result_n788;
              }
              bool hxc_l_tmp_short_circuit_load_result_n789 = hxc_l_tmp_short_circuit_result_n266;
              bool hxc_l_tmp_short_circuit_result_n267 = hxc_l_tmp_short_circuit_load_result_n789;
              if (!hxc_l_tmp_short_circuit_load_result_n789)
              {
                struct hxc_caxecraft_domain_CharacterBody hxc_l_tmp_record_field_load_result_n790 = hxc_l_character.hxc_body;
                bool hxc_l_tmp_call_result_n792 = hxc_caxecraft_domain_CharacterPhysics_canPlaceAt(hxc_l_tmp_record_field_load_result_n790, hxc_l_placement);
                hxc_l_tmp_short_circuit_result_n267 = !hxc_l_tmp_call_result_n792;
              }
              bool hxc_l_tmp_short_circuit_load_result_n793 = hxc_l_tmp_short_circuit_result_n267;
              bool hxc_l_tmp_short_circuit_result_n268 = hxc_l_tmp_short_circuit_load_result_n793;
              if (!hxc_l_tmp_short_circuit_load_result_n793)
              {
                struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n794 = hxc_l_session;
                hxc_l_gc_roots[158] = (const void *)hxc_l_tmp_load_result_n794;
                if (hxc_l_tmp_load_result_n794 == NULL)
                {
                  abort();
                }
                struct hxc_caxecraft_scenario_VoxelPoint hxc_l_tmp_load_result_n795 = hxc_l_placement;
                bool hxc_l_tmp_instance_call_result_n797 = hxc_caxecraft_domain_GameSession_placeTerrain(hxc_l_tmp_load_result_n794, hxc_l_tmp_load_result_n795, hxc_l_selectedBlock);
                hxc_l_tmp_short_circuit_result_n268 = !hxc_l_tmp_instance_call_result_n797;
              }
              if (hxc_l_tmp_short_circuit_result_n268)
              {
                hxc_l_placementBlockedFrames = 60;
              }
              else
              {
                if (hxc_l_self == NULL)
                {
                  abort();
                }
                struct hxc_caxecraft_app_TerrainRenderer *hxc_l_tmp_owned_class_field_address_n799 = &(*hxc_l_self).hxc_terrainRenderer;
                struct hxc_caxecraft_app_TerrainChunkCache *hxc_l_tmp_owned_class_field_address_n800 = &(*hxc_l_tmp_owned_class_field_address_n799).hxc_cache;
                if (hxc_l_tmp_owned_class_field_address_n800 == NULL)
                {
                  abort();
                }
                hxc_caxecraft_app_TerrainChunkCache_invalidate(hxc_l_tmp_owned_class_field_address_n800, hxc_l_placement);
                if (hxc_l_selectedMode == 1)
                {
                  struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n804 = hxc_caxecraft_gameplay_Inventory_consumeSelected(hxc_l_inventory);
                  hxc_l_inventory = hxc_l_tmp_call_result_n804;
                }
              }
            }
          }
        }
      }
    }
    if (hxc_l_placementBlockedFrames > 0)
    {
      hxc_l_placementBlockedFrames = hxc_i32_subtract_wrapping(hxc_l_placementBlockedFrames, 1);
    }
    if (!hxc_l_paused_h0114c9e4c987)
    {
      struct hxc_caxecraft_gameplay_BerryDropState hxc_l_tmp_load_result_n808 = hxc_l_berryDrop;
      double hxc_l_tmp_record_field_load_result_n809 = hxc_l_character.hxc_body.hxc_x;
      double hxc_l_tmp_record_field_load_result_n810 = hxc_l_character.hxc_body.hxc_y;
      bool hxc_l_tmp_call_result_n812 = hxc_caxecraft_gameplay_BerryDrop_isInRange(hxc_l_tmp_load_result_n808, hxc_l_tmp_record_field_load_result_n809, hxc_l_tmp_record_field_load_result_n810, hxc_l_character.hxc_body.hxc_z);
      if (hxc_l_tmp_call_result_n812)
      {
        struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_load_result_n813 = hxc_l_inventory;
        int32_t hxc_l_tmp_call_result_n816 = hxc_caxecraft_gameplay_Inventory_acceptedAmount(hxc_l_tmp_load_result_n813, hxc_caxecraft_gameplay_ItemKind_Berries, hxc_l_berryDrop.hxc_amount);
        int32_t hxc_l_acceptedDrop = hxc_l_tmp_call_result_n816;
        if (hxc_l_acceptedDrop > 0)
        {
          struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_load_result_n818 = hxc_l_inventory;
          struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n821 = hxc_caxecraft_gameplay_Inventory_collectItem(hxc_l_tmp_load_result_n818, hxc_caxecraft_gameplay_ItemKind_Berries, hxc_l_acceptedDrop);
          hxc_l_inventory = hxc_l_tmp_call_result_n821;
          struct hxc_caxecraft_gameplay_BerryDropState hxc_l_tmp_load_result_n822 = hxc_l_berryDrop;
          struct hxc_caxecraft_gameplay_BerryDropState hxc_l_tmp_call_result_n824 = hxc_caxecraft_gameplay_BerryDrop_collectAmount(hxc_l_tmp_load_result_n822, hxc_l_acceptedDrop);
          hxc_l_berryDrop = hxc_l_tmp_call_result_n824;
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
    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n841 = hxc_l_session;
    hxc_l_gc_roots[159] = (const void *)hxc_l_tmp_load_result_n841;
    if (hxc_l_tmp_load_result_n841 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_domain_GameView hxc_l_tmp_instance_call_result_n842 = hxc_caxecraft_domain_GameSession_view(hxc_l_tmp_load_result_n841);
    struct hxc_caxecraft_domain_GameView hxc_l_committedView = hxc_l_tmp_instance_call_result_n842;
    if (!hxc_l_committedView.hxc_valid)
    {
      hxc_l_quit = true;
    }
    else
    {
      hxc_l_character = hxc_l_committedView.hxc_localPlayer;
    }
    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n845 = hxc_l_session;
    hxc_l_gc_roots[160] = (const void *)hxc_l_tmp_load_result_n845;
    if (hxc_l_tmp_load_result_n845 == NULL)
    {
      abort();
    }
    bool hxc_l_tmp_instance_call_result_n847 = hxc_caxecraft_domain_GameSession_actorInteractionAvailable(hxc_l_tmp_load_result_n845, hxc_l_dialogueActorId);
    hxc_l_guideInteractionAvailable = hxc_l_tmp_instance_call_result_n847;
    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n848 = hxc_l_session;
    hxc_l_gc_roots[161] = (const void *)hxc_l_tmp_load_result_n848;
    if (hxc_l_tmp_load_result_n848 == NULL)
    {
      abort();
    }
    struct hxc_array_ref *hxc_l_tmp_instance_call_result_n849 = hxc_caxecraft_domain_GameSession_actorControllerStateSnapshots(hxc_l_tmp_load_result_n848);
    hxc_l_presentationActorPhases = hxc_l_tmp_instance_call_result_n849;
    struct hxc_array_ref *hxc_l_tmp_load_result_n850 = hxc_l_presentationActorPhases;
    struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation hxc_l_tmp_call_result_n853 = hxc_caxecraft_app_CaxecraftApp_observeActorPhase(hxc_l_tmp_load_result_n850, hxc_l_dialogueActorId, hxc_caxecraft_domain_ActorControllerPhase_Stationary);
    hxc_l_dialoguePhase = hxc_l_tmp_call_result_n853;
    struct hxc_array_ref *hxc_l_tmp_load_result_n854 = hxc_l_presentationActorPhases;
    struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation hxc_l_tmp_call_result_n857 = hxc_caxecraft_app_CaxecraftApp_observeActorPhase(hxc_l_tmp_load_result_n854, hxc_l_enemyActorId, hxc_caxecraft_domain_ActorControllerPhase_Defeated);
    hxc_l_enemyPhase = hxc_l_tmp_call_result_n857;
    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n858 = hxc_l_session;
    hxc_l_gc_roots[162] = (const void *)hxc_l_tmp_load_result_n858;
    if (hxc_l_tmp_load_result_n858 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n860 = hxc_caxecraft_domain_GameSession_readCharacter(hxc_l_tmp_load_result_n858, hxc_l_dialogueActorId);
    hxc_l_dialogueActor = hxc_l_tmp_instance_call_result_n860;
    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n861 = hxc_l_session;
    hxc_l_gc_roots[163] = (const void *)hxc_l_tmp_load_result_n861;
    if (hxc_l_tmp_load_result_n861 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n863 = hxc_caxecraft_domain_GameSession_readCharacter(hxc_l_tmp_load_result_n861, hxc_l_enemyActorId);
    hxc_l_enemyActor = hxc_l_tmp_instance_call_result_n863;
    bool hxc_l_tmp_record_field_load_result_n864 = hxc_l_dialoguePhase.hxc_valid;
    bool hxc_l_tmp_short_circuit_result_n272 = !hxc_l_tmp_record_field_load_result_n864;
    if (!!hxc_l_tmp_record_field_load_result_n864)
    {
      hxc_l_tmp_short_circuit_result_n272 = !hxc_l_enemyPhase.hxc_valid;
    }
    bool hxc_l_tmp_short_circuit_load_result_n866 = hxc_l_tmp_short_circuit_result_n272;
    bool hxc_l_tmp_short_circuit_result_n273 = hxc_l_tmp_short_circuit_load_result_n866;
    if (!hxc_l_tmp_short_circuit_load_result_n866)
    {
      hxc_l_tmp_short_circuit_result_n273 = hxc_l_dialogueActor.hxc_id <= 0;
    }
    bool hxc_l_tmp_short_circuit_load_result_n868 = hxc_l_tmp_short_circuit_result_n273;
    bool hxc_l_tmp_short_circuit_result_n274 = hxc_l_tmp_short_circuit_load_result_n868;
    if (!hxc_l_tmp_short_circuit_load_result_n868)
    {
      hxc_l_tmp_short_circuit_result_n274 = hxc_l_enemyActor.hxc_id <= 0;
    }
    if (hxc_l_tmp_short_circuit_result_n274)
    {
      hxc_l_quit = true;
    }
    int32_t hxc_l_completedTicks = hxc_l_committedView.hxc_completedTicks;
    if (hxc_l_resetMotionThisFrame)
    {
      struct hxc_caxecraft_app_MotionHistory hxc_l_tmp_call_result_n874 = hxc_caxecraft_app_MotionInterpolation_start(hxc_l_character.hxc_body);
      hxc_l_motionHistory = hxc_l_tmp_call_result_n874;
    }
    struct hxc_caxecraft_app_MotionHistory hxc_l_tmp_load_result_n875 = hxc_l_motionHistory;
    struct hxc_caxecraft_app_RenderPosition hxc_l_tmp_call_result_n877 = hxc_caxecraft_app_MotionInterpolation_sample(hxc_l_tmp_load_result_n875, hxc_l_accumulator, 0.05);
    struct hxc_caxecraft_app_RenderPosition hxc_l_renderPosition = hxc_l_tmp_call_result_n877;
    double hxc_l_eyeX = hxc_l_renderPosition.hxc_x;
    double hxc_l_eyeY = hxc_l_renderPosition.hxc_y + 1.62;
    double hxc_l_eyeZ = hxc_l_renderPosition.hxc_z;
    double hxc_l_tmp_load_result_n881 = hxc_l_eyeX;
    double hxc_l_tmp_load_result_n882 = hxc_l_eyeY;
    double hxc_l_tmp_load_result_n883 = hxc_l_eyeZ;
    double hxc_l_tmp_load_result_n885 = hxc_l_eyeX;
    double hxc_l_tmp_load_result_n886 = hxc_l_lookX;
    double hxc_l_tmp_load_result_n887 = hxc_l_eyeY;
    double hxc_l_tmp_load_result_n888 = hxc_l_lookY;
    double hxc_l_tmp_load_result_n889 = hxc_l_eyeZ;
    struct Camera3D hxc_l_camera = (struct Camera3D){ .position = (struct Vector3){ .x = (float)hxc_l_tmp_load_result_n881, .y = (float)hxc_l_tmp_load_result_n882, .z = (float)hxc_l_tmp_load_result_n883 }, .target = (struct Vector3){ .x = (float)(hxc_l_tmp_load_result_n885 + hxc_l_tmp_load_result_n886), .y = (float)(hxc_l_tmp_load_result_n887 + hxc_l_tmp_load_result_n888), .z = (float)(hxc_l_tmp_load_result_n889 + hxc_l_lookZ) }, .up = (struct Vector3){ .x = (float)0.0, .y = (float)1.0, .z = (float)0.0 }, .fovy = (float)70.0, .projection = 0 };
    BeginDrawing();
    if (hxc_l_onTitle)
    {
      int32_t hxc_l_tmp_native_call_result_n895 = GetScreenWidth();
      int32_t hxc_l_width_h8cdb9b1814ce = (int32_t)hxc_l_tmp_native_call_result_n895;
      int32_t hxc_l_tmp_native_call_result_n896 = GetScreenHeight();
      int32_t hxc_l_height_h36ef87944fb6 = (int32_t)hxc_l_tmp_native_call_result_n896;
      if (hxc_l_titleTextureReady)
      {
        struct Color hxc_l_this1_he6dc64b070aa = (struct Color){ .r = 255, .g = 255, .b = 255, .a = 255 };
        struct Texture hxc_l_tmp_load_result_n899 = hxc_l_titleTexture;
        double hxc_l_imageAspect = hxc_f64_divide_zero_safe((double)(int32_t)hxc_l_tmp_load_result_n899.width, (double)(int32_t)hxc_l_titleTexture.height);
        int32_t hxc_l_tmp_load_result_n903 = hxc_l_width_h8cdb9b1814ce;
        double hxc_l_screenAspect = hxc_f64_divide_zero_safe((double)hxc_l_tmp_load_result_n903, (double)hxc_l_height_h36ef87944fb6);
        double hxc_l_sourceX = 0.0;
        double hxc_l_sourceY = 0.0;
        double hxc_l_sourceWidth = (double)(int32_t)hxc_l_titleTexture.width;
        double hxc_l_sourceHeight = (double)(int32_t)hxc_l_titleTexture.height;
        double hxc_l_tmp_load_result_n909 = hxc_l_screenAspect;
        if (hxc_l_tmp_load_result_n909 > hxc_l_imageAspect)
        {
          struct Texture hxc_l_tmp_load_result_n911 = hxc_l_titleTexture;
          hxc_l_sourceHeight = hxc_f64_divide_zero_safe((double)(int32_t)hxc_l_tmp_load_result_n911.width, hxc_l_screenAspect);
          struct Texture hxc_l_tmp_load_result_n914 = hxc_l_titleTexture;
          hxc_l_sourceY = ((double)(int32_t)hxc_l_tmp_load_result_n914.height - hxc_l_sourceHeight) * 0.5;
        }
        else
        {
          struct Texture hxc_l_tmp_load_result_n917 = hxc_l_titleTexture;
          hxc_l_sourceWidth = (double)(int32_t)hxc_l_tmp_load_result_n917.height * hxc_l_screenAspect;
          struct Texture hxc_l_tmp_load_result_n920 = hxc_l_titleTexture;
          hxc_l_sourceX = ((double)(int32_t)hxc_l_tmp_load_result_n920.width - hxc_l_sourceWidth) * 0.5;
        }
        struct Texture hxc_l_tmp_load_result_n923 = hxc_l_titleTexture;
        double hxc_l_tmp_load_result_n924 = hxc_l_sourceX;
        double hxc_l_tmp_load_result_n925 = hxc_l_sourceY;
        double hxc_l_tmp_load_result_n926 = hxc_l_sourceWidth;
        double hxc_l_tmp_load_result_n927 = hxc_l_sourceHeight;
        int32_t hxc_l_tmp_load_result_n929 = hxc_l_width_h8cdb9b1814ce;
        int32_t hxc_l_tmp_load_result_n930 = hxc_l_height_h36ef87944fb6;
        DrawTexturePro(hxc_l_tmp_load_result_n923, (struct Rectangle){ .x = (float)hxc_l_tmp_load_result_n924, .y = (float)hxc_l_tmp_load_result_n925, .width = (float)hxc_l_tmp_load_result_n926, .height = (float)hxc_l_tmp_load_result_n927 }, (struct Rectangle){ .x = (float)0.0, .y = (float)0.0, .width = (float)((double)hxc_l_tmp_load_result_n929 + 0.0), .height = (float)((double)hxc_l_tmp_load_result_n930 + 0.0) }, (struct Vector2){ .x = (float)0.0, .y = (float)0.0 }, (float)0.0, hxc_l_this1_he6dc64b070aa);
      }
      else
      {
        ClearBackground((struct Color){ .r = 126, .g = 190, .b = 201, .a = 255 });
      }
      struct Color hxc_l_this1_hb94710a96b12 = (struct Color){ .r = 4, .g = 14, .b = 23, .a = 132 };
      int32_t hxc_l_tmp_load_result_n936 = hxc_l_width_h8cdb9b1814ce;
      int32_t hxc_l_tmp_load_result_n937 = hxc_l_height_h36ef87944fb6;
      DrawRectangle((int32_t)0, (int32_t)0, (int32_t)hxc_l_tmp_load_result_n936, (int32_t)hxc_l_tmp_load_result_n937, hxc_l_this1_hb94710a96b12);
      if (hxc_l_wordmarkTextureReady)
      {
        int32_t hxc_l_centerX = hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_l_width_h8cdb9b1814ce, (double)2));
        struct Color hxc_l_this1_h1b314e7b5d9d = (struct Color){ .r = 255, .g = 255, .b = 255, .a = 255 };
        double hxc_l_scale = hxc_f64_divide_zero_safe((double)560, (double)(int32_t)hxc_l_wordmarkTexture.width);
        double hxc_l_heightScale = hxc_f64_divide_zero_safe((double)132, (double)(int32_t)hxc_l_wordmarkTexture.height);
        double hxc_l_tmp_load_result_n946 = hxc_l_heightScale;
        if (hxc_l_tmp_load_result_n946 < hxc_l_scale)
        {
          hxc_l_scale = hxc_l_heightScale;
        }
        struct Texture hxc_l_tmp_load_result_n949 = hxc_l_wordmarkTexture;
        double hxc_l_width_h6bf51ca1bfc0 = (double)(int32_t)hxc_l_tmp_load_result_n949.width * hxc_l_scale;
        struct Texture hxc_l_tmp_load_result_n952 = hxc_l_wordmarkTexture;
        double hxc_l_height_h39ffa5420af4 = (double)(int32_t)hxc_l_tmp_load_result_n952.height * hxc_l_scale;
        double hxc_l_width1 = (double)(int32_t)hxc_l_wordmarkTexture.width + 0.0;
        double hxc_l_height1 = (double)(int32_t)hxc_l_wordmarkTexture.height + 0.0;
        struct Texture hxc_l_tmp_load_result_n959 = hxc_l_wordmarkTexture;
        double hxc_l_tmp_load_result_n960 = hxc_l_width1;
        double hxc_l_tmp_load_result_n961 = hxc_l_height1;
        int32_t hxc_l_tmp_load_result_n963 = hxc_l_centerX;
        double hxc_l_tmp_load_result_n964 = hxc_l_width_h6bf51ca1bfc0;
        double hxc_l_tmp_load_result_n965 = hxc_l_width_h6bf51ca1bfc0;
        double hxc_l_tmp_load_result_n966 = hxc_l_height_h39ffa5420af4;
        DrawTexturePro(hxc_l_tmp_load_result_n959, (struct Rectangle){ .x = (float)0.0, .y = (float)0.0, .width = (float)hxc_l_tmp_load_result_n960, .height = (float)hxc_l_tmp_load_result_n961 }, (struct Rectangle){ .x = (float)((double)hxc_l_tmp_load_result_n963 - hxc_l_tmp_load_result_n964 * 0.5), .y = (float)28.0, .width = (float)hxc_l_tmp_load_result_n965, .height = (float)hxc_l_tmp_load_result_n966 }, (struct Vector2){ .x = (float)0.0, .y = (float)0.0 }, (float)0.0, hxc_l_this1_h1b314e7b5d9d);
      }
      else
      {
        int32_t hxc_l_x_h8b2601e827c8 = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_l_width_h8cdb9b1814ce, (double)2)), 92);
        struct Color hxc_l_this1_h9fa907e931d7 = (struct Color){ .r = 242, .g = 249, .b = 245, .a = 255 };
        struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_tmp_load_result_n972 = hxc_l_uiCatalog;
        hxc_l_gc_roots[164] = (const void *)hxc_l_tmp_load_result_n972;
        if (hxc_l_tmp_load_result_n972 == NULL)
        {
          abort();
        }
        hxc_string hxc_l_tmp_instance_call_result_n974 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_tmp_load_result_n972, hxc_l_locale, 34);
        hxc_l_tmp_cstring_ref_owner_owner_n301 = hxc_l_tmp_instance_call_result_n974;
        hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n976 = HXC_BORROWED_CSTRING_INITIALIZER;
        if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n301, &hxc_l_tmp_string_borrow_cstring_result_n976) != HXC_STATUS_OK)
        {
          abort();
        }
        int32_t hxc_l_tmp_load_result_n977 = hxc_l_x_h8b2601e827c8;
        DrawText(hxc_l_tmp_string_borrow_cstring_result_n976.data, (int32_t)hxc_l_tmp_load_result_n977, (int32_t)52, (int32_t)34, hxc_l_this1_h9fa907e931d7);
        if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n301) != HXC_STATUS_OK)
        {
          abort();
        }
      }
      int32_t hxc_l_firstTop = hxc_i32_subtract_wrapping(hxc_l_height_h36ef87944fb6, 293);
      int32_t hxc_l_left_hfb81c22fc699 = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_l_width_h8cdb9b1814ce, (double)2)), 180);
      if (hxc_l_selectedMode == 0)
      {
        struct Color hxc_l_this1_h5c298dbe73e0 = (struct Color){ .r = 16, .g = 88, .b = 102, .a = 232 };
        int32_t hxc_l_tmp_load_result_n983 = hxc_l_left_hfb81c22fc699;
        int32_t hxc_l_tmp_load_result_n984 = hxc_l_firstTop;
        DrawRectangle((int32_t)hxc_l_tmp_load_result_n983, (int32_t)hxc_l_tmp_load_result_n984, (int32_t)360, (int32_t)54, hxc_l_this1_h5c298dbe73e0);
        struct Color hxc_l_this1_hd6ecd99b39e5 = (struct Color){ .r = 255, .g = 190, .b = 55, .a = 255 };
        int32_t hxc_l_tmp_load_result_n987 = hxc_l_left_hfb81c22fc699;
        int32_t hxc_l_tmp_load_result_n988 = hxc_l_firstTop;
        DrawRectangleLines((int32_t)hxc_l_tmp_load_result_n987, (int32_t)hxc_l_tmp_load_result_n988, (int32_t)360, (int32_t)54, hxc_l_this1_hd6ecd99b39e5);
      }
      else
      {
        struct Color hxc_l_this1_had1904d5a268 = (struct Color){ .r = 6, .g = 26, .b = 36, .a = 218 };
        int32_t hxc_l_tmp_load_result_n991 = hxc_l_left_hfb81c22fc699;
        int32_t hxc_l_tmp_load_result_n992 = hxc_l_firstTop;
        DrawRectangle((int32_t)hxc_l_tmp_load_result_n991, (int32_t)hxc_l_tmp_load_result_n992, (int32_t)360, (int32_t)54, hxc_l_this1_had1904d5a268);
        struct Color hxc_l_this1_h45f786090749 = (struct Color){ .r = 92, .g = 194, .b = 188, .a = 255 };
        int32_t hxc_l_tmp_load_result_n995 = hxc_l_left_hfb81c22fc699;
        int32_t hxc_l_tmp_load_result_n996 = hxc_l_firstTop;
        DrawRectangleLines((int32_t)hxc_l_tmp_load_result_n995, (int32_t)hxc_l_tmp_load_result_n996, (int32_t)360, (int32_t)54, hxc_l_this1_h45f786090749);
      }
      struct Color hxc_l_this1_h02e5bb203f76 = (struct Color){ .r = 242, .g = 249, .b = 245, .a = 255 };
      struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_tmp_load_result_n999 = hxc_l_uiCatalog;
      hxc_l_gc_roots[165] = (const void *)hxc_l_tmp_load_result_n999;
      if (hxc_l_tmp_load_result_n999 == NULL)
      {
        abort();
      }
      hxc_string hxc_l_tmp_instance_call_result_n1001 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_tmp_load_result_n999, hxc_l_locale, 27);
      hxc_l_tmp_cstring_ref_owner_owner_n309 = hxc_l_tmp_instance_call_result_n1001;
      hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n1003 = HXC_BORROWED_CSTRING_INITIALIZER;
      if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n309, &hxc_l_tmp_string_borrow_cstring_result_n1003) != HXC_STATUS_OK)
      {
        abort();
      }
      int32_t hxc_l_tmp_load_result_n1004 = hxc_l_left_hfb81c22fc699;
      int32_t hxc_l_tmp_load_result_n1005 = hxc_l_firstTop;
      DrawText(hxc_l_tmp_string_borrow_cstring_result_n1003.data, (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n1004, 28), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n1005, 16), (int32_t)21, hxc_l_this1_h02e5bb203f76);
      int32_t hxc_l_top_h2e1806690418 = hxc_i32_add_wrapping(hxc_i32_add_wrapping(hxc_l_firstTop, 54), 14);
      int32_t hxc_l_left_h2b28d96e5f78 = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_l_width_h8cdb9b1814ce, (double)2)), 180);
      if (hxc_l_selectedMode == 1)
      {
        struct Color hxc_l_this1_hb8a12dd3602e = (struct Color){ .r = 16, .g = 88, .b = 102, .a = 232 };
        int32_t hxc_l_tmp_load_result_n1011 = hxc_l_left_h2b28d96e5f78;
        int32_t hxc_l_tmp_load_result_n1012 = hxc_l_top_h2e1806690418;
        DrawRectangle((int32_t)hxc_l_tmp_load_result_n1011, (int32_t)hxc_l_tmp_load_result_n1012, (int32_t)360, (int32_t)54, hxc_l_this1_hb8a12dd3602e);
        struct Color hxc_l_this1_habddfcaec7d9 = (struct Color){ .r = 255, .g = 190, .b = 55, .a = 255 };
        int32_t hxc_l_tmp_load_result_n1015 = hxc_l_left_h2b28d96e5f78;
        int32_t hxc_l_tmp_load_result_n1016 = hxc_l_top_h2e1806690418;
        DrawRectangleLines((int32_t)hxc_l_tmp_load_result_n1015, (int32_t)hxc_l_tmp_load_result_n1016, (int32_t)360, (int32_t)54, hxc_l_this1_habddfcaec7d9);
      }
      else
      {
        struct Color hxc_l_this1_h1a0285c11d17 = (struct Color){ .r = 6, .g = 26, .b = 36, .a = 218 };
        int32_t hxc_l_tmp_load_result_n1019 = hxc_l_left_h2b28d96e5f78;
        int32_t hxc_l_tmp_load_result_n1020 = hxc_l_top_h2e1806690418;
        DrawRectangle((int32_t)hxc_l_tmp_load_result_n1019, (int32_t)hxc_l_tmp_load_result_n1020, (int32_t)360, (int32_t)54, hxc_l_this1_h1a0285c11d17);
        struct Color hxc_l_this1_h01d672eb5e7b = (struct Color){ .r = 92, .g = 194, .b = 188, .a = 255 };
        int32_t hxc_l_tmp_load_result_n1023 = hxc_l_left_h2b28d96e5f78;
        int32_t hxc_l_tmp_load_result_n1024 = hxc_l_top_h2e1806690418;
        DrawRectangleLines((int32_t)hxc_l_tmp_load_result_n1023, (int32_t)hxc_l_tmp_load_result_n1024, (int32_t)360, (int32_t)54, hxc_l_this1_h01d672eb5e7b);
      }
      struct Color hxc_l_this1_h645748b5825b = (struct Color){ .r = 242, .g = 249, .b = 245, .a = 255 };
      struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_tmp_load_result_n1027 = hxc_l_uiCatalog;
      hxc_l_gc_roots[166] = (const void *)hxc_l_tmp_load_result_n1027;
      if (hxc_l_tmp_load_result_n1027 == NULL)
      {
        abort();
      }
      hxc_string hxc_l_tmp_instance_call_result_n1029 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_tmp_load_result_n1027, hxc_l_locale, 26);
      hxc_l_tmp_cstring_ref_owner_owner_n317 = hxc_l_tmp_instance_call_result_n1029;
      hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n1031 = HXC_BORROWED_CSTRING_INITIALIZER;
      if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n317, &hxc_l_tmp_string_borrow_cstring_result_n1031) != HXC_STATUS_OK)
      {
        abort();
      }
      int32_t hxc_l_tmp_load_result_n1032 = hxc_l_left_h2b28d96e5f78;
      int32_t hxc_l_tmp_load_result_n1033 = hxc_l_top_h2e1806690418;
      DrawText(hxc_l_tmp_string_borrow_cstring_result_n1031.data, (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n1032, 28), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n1033, 16), (int32_t)21, hxc_l_this1_h645748b5825b);
      int32_t hxc_l_top_h29fd11ea96fa = hxc_i32_add_wrapping(hxc_l_firstTop, 136);
      int32_t hxc_l_left_h482f490e00c4 = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_l_width_h8cdb9b1814ce, (double)2)), 180);
      struct Color hxc_l_this1_hdbad86337d55 = (struct Color){ .r = 6, .g = 26, .b = 36, .a = 218 };
      int32_t hxc_l_tmp_load_result_n1038 = hxc_l_left_h482f490e00c4;
      int32_t hxc_l_tmp_load_result_n1039 = hxc_l_top_h29fd11ea96fa;
      DrawRectangle((int32_t)hxc_l_tmp_load_result_n1038, (int32_t)hxc_l_tmp_load_result_n1039, (int32_t)360, (int32_t)54, hxc_l_this1_hdbad86337d55);
      struct Color hxc_l_this1_h5ee6584bb863 = (struct Color){ .r = 92, .g = 194, .b = 188, .a = 255 };
      int32_t hxc_l_tmp_load_result_n1042 = hxc_l_left_h482f490e00c4;
      int32_t hxc_l_tmp_load_result_n1043 = hxc_l_top_h29fd11ea96fa;
      DrawRectangleLines((int32_t)hxc_l_tmp_load_result_n1042, (int32_t)hxc_l_tmp_load_result_n1043, (int32_t)360, (int32_t)54, hxc_l_this1_h5ee6584bb863);
      struct Color hxc_l_this1_h828d9a46f8aa = (struct Color){ .r = 242, .g = 249, .b = 245, .a = 255 };
      struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_tmp_load_result_n1046 = hxc_l_uiCatalog;
      hxc_l_gc_roots[167] = (const void *)hxc_l_tmp_load_result_n1046;
      if (hxc_l_tmp_load_result_n1046 == NULL)
      {
        abort();
      }
      hxc_string hxc_l_tmp_instance_call_result_n1048 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_tmp_load_result_n1046, hxc_l_locale, 28);
      hxc_l_tmp_cstring_ref_owner_owner_n323 = hxc_l_tmp_instance_call_result_n1048;
      hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n1050 = HXC_BORROWED_CSTRING_INITIALIZER;
      if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n323, &hxc_l_tmp_string_borrow_cstring_result_n1050) != HXC_STATUS_OK)
      {
        abort();
      }
      int32_t hxc_l_tmp_load_result_n1051 = hxc_l_left_h482f490e00c4;
      int32_t hxc_l_tmp_load_result_n1052 = hxc_l_top_h29fd11ea96fa;
      DrawText(hxc_l_tmp_string_borrow_cstring_result_n1050.data, (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n1051, 28), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n1052, 16), (int32_t)21, hxc_l_this1_h828d9a46f8aa);
      int32_t hxc_l_x_h3d9dda0555de = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_l_width_h8cdb9b1814ce, (double)2)), 285);
      struct Color hxc_l_this1_h9376a3971edf = (struct Color){ .r = 229, .g = 241, .b = 235, .a = 255 };
      struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_tmp_load_result_n1056 = hxc_l_uiCatalog;
      hxc_l_gc_roots[168] = (const void *)hxc_l_tmp_load_result_n1056;
      if (hxc_l_tmp_load_result_n1056 == NULL)
      {
        abort();
      }
      hxc_string hxc_l_tmp_instance_call_result_n1058 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_tmp_load_result_n1056, hxc_l_locale, 29);
      hxc_l_tmp_cstring_ref_owner_owner_n326 = hxc_l_tmp_instance_call_result_n1058;
      hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n1060 = HXC_BORROWED_CSTRING_INITIALIZER;
      if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n326, &hxc_l_tmp_string_borrow_cstring_result_n1060) != HXC_STATUS_OK)
      {
        abort();
      }
      int32_t hxc_l_tmp_load_result_n1061 = hxc_l_x_h3d9dda0555de;
      int32_t hxc_l_tmp_load_result_n1062 = hxc_l_height_h36ef87944fb6;
      DrawText(hxc_l_tmp_string_borrow_cstring_result_n1060.data, (int32_t)hxc_l_tmp_load_result_n1061, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n1062, 58), (int32_t)16, hxc_l_this1_h9376a3971edf);
      if (hxc_l_selectedMode == 1)
      {
        int32_t hxc_l_x_h45555119f550 = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_l_width_h8cdb9b1814ce, (double)2)), 230);
        struct Color hxc_l_this1_hc6146125a5c6 = (struct Color){ .r = 255, .g = 205, .b = 91, .a = 255 };
        const char *hxc_l_tmp_call_result_n1068 = hxc_caxecraft_localization_FirstPlayableCatalog_text(hxc_l_locale, 1);
        int32_t hxc_l_tmp_load_result_n1069 = hxc_l_x_h45555119f550;
        int32_t hxc_l_tmp_load_result_n1070 = hxc_l_firstTop;
        DrawText(hxc_l_tmp_call_result_n1068, (int32_t)hxc_l_tmp_load_result_n1069, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n1070, 36), (int32_t)17, hxc_l_this1_hc6146125a5c6);
      }
      if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n326) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n323) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n317) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n309) != HXC_STATUS_OK)
      {
        abort();
      }
    }
    else
    {
      if (hxc_l_onEditor)
      {
        struct hxc_caxecraft_app_CaxecraftEditorScreen *hxc_l_tmp_load_result_n1073 = hxc_l_editorScreen;
        hxc_l_gc_roots[169] = (const void *)hxc_l_tmp_load_result_n1073;
        if (hxc_l_tmp_load_result_n1073 == NULL)
        {
          abort();
        }
        int32_t hxc_l_tmp_load_result_n1074 = hxc_l_locale;
        enum hxc_caxecraft_app_EditorScreenAction hxc_l_tmp_instance_call_result_n1076 = hxc_caxecraft_app_CaxecraftEditorScreen_draw(hxc_l_tmp_load_result_n1073, hxc_l_tmp_load_result_n1074, hxc_l_editorNavigationCommand);
        if (hxc_l_tmp_instance_call_result_n1076 == hxc_caxecraft_app_EditorScreenAction_ReturnToTitle)
        {
          enum hxc_caxecraft_app_AppScreen hxc_l_tmp_call_result_n1079 = hxc_caxecraft_app_AppScreen_closeEditor(hxc_l_screen);
          hxc_l_screen = hxc_l_tmp_call_result_n1079;
        }
      }
      else
      {
        ClearBackground((struct Color){ .r = 126, .g = 190, .b = 201, .a = 255 });
        int32_t hxc_l_tmp_native_call_result_n1081 = GetScreenWidth();
        int32_t hxc_l_sunX = hxc_i32_subtract_wrapping((int32_t)hxc_l_tmp_native_call_result_n1081, 260);
        float hxc_l_radius_h749adf846603 = (float)42.0;
        struct Color hxc_l_this1_h8d6e48862b68 = (struct Color){ .r = 255, .g = 226, .b = 136, .a = 255 };
        int32_t hxc_l_tmp_load_result_n1083 = hxc_l_sunX;
        float hxc_l_tmp_load_result_n1084 = hxc_l_radius_h749adf846603;
        DrawCircle((int32_t)hxc_l_tmp_load_result_n1083, (int32_t)86, hxc_l_tmp_load_result_n1084, hxc_l_this1_h8d6e48862b68);
        float hxc_l_radius_hce57578bb889 = (float)30.0;
        struct Color hxc_l_this1_h30c5a20fccb7 = (struct Color){ .r = 255, .g = 246, .b = 205, .a = 255 };
        int32_t hxc_l_tmp_load_result_n1087 = hxc_l_sunX;
        float hxc_l_tmp_load_result_n1088 = hxc_l_radius_hce57578bb889;
        DrawCircle((int32_t)hxc_l_tmp_load_result_n1087, (int32_t)86, hxc_l_tmp_load_result_n1088, hxc_l_this1_h30c5a20fccb7);
        BeginMode3D(hxc_l_camera);
        if (hxc_l_self == NULL)
        {
          abort();
        }
        struct hxc_caxecraft_app_TerrainRenderer *hxc_l_tmp_owned_class_field_address_n1091 = &(*hxc_l_self).hxc_terrainRenderer;
        if (hxc_l_tmp_owned_class_field_address_n1091 == NULL)
        {
          abort();
        }
        struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1092 = hxc_l_session;
        hxc_l_gc_roots[170] = (const void *)hxc_l_tmp_load_result_n1092;
        if (hxc_l_tmp_load_result_n1092 == NULL)
        {
          abort();
        }
        size_t hxc_l_tmp_length_n1094;
        const uint8_t *hxc_l_tmp_instance_call_result_n1093 = hxc_caxecraft_domain_GameSession_worldView(hxc_l_tmp_load_result_n1092, &hxc_l_tmp_length_n1094);
        struct Texture hxc_l_tmp_load_result_n1095 = hxc_l_terrainTexture;
        bool hxc_l_tmp_load_result_n1096 = hxc_l_terrainTextureReady;
        struct Texture hxc_l_tmp_load_result_n1097 = hxc_l_adventureTerrainTexture;
        bool hxc_l_tmp_load_result_n1098 = hxc_l_adventureTerrainTextureReady;
        double hxc_l_tmp_record_field_load_result_n1099 = hxc_l_renderPosition.hxc_x;
        struct hxc_caxecraft_app_TerrainRenderCounters hxc_l_tmp_instance_call_result_n1101 = hxc_caxecraft_app_TerrainRenderer_draw(hxc_l_tmp_owned_class_field_address_n1091, hxc_l_tmp_instance_call_result_n1093, hxc_l_tmp_length_n1094, hxc_l_tmp_load_result_n1095, hxc_l_tmp_load_result_n1096, hxc_l_tmp_load_result_n1097, hxc_l_tmp_load_result_n1098, hxc_l_tmp_record_field_load_result_n1099, hxc_l_renderPosition.hxc_z);
        struct hxc_caxecraft_app_TerrainRenderCounters hxc_l_renderCounters = hxc_l_tmp_instance_call_result_n1101;
        struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1102 = hxc_l_session;
        hxc_l_gc_roots[171] = (const void *)hxc_l_tmp_load_result_n1102;
        if (hxc_l_tmp_load_result_n1102 == NULL)
        {
          abort();
        }
        size_t hxc_l_tmp_length_n1104;
        const uint8_t *hxc_l_tmp_instance_call_result_n1103 = hxc_caxecraft_domain_GameSession_worldView(hxc_l_tmp_load_result_n1102, &hxc_l_tmp_length_n1104);
        struct Texture hxc_l_tmp_load_result_n1105 = hxc_l_terrainTexture;
        bool hxc_l_tmp_load_result_n1106 = hxc_l_terrainTextureReady;
        struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1107 = hxc_l_levelView;
        hxc_l_gc_roots[172] = (const void *)hxc_l_tmp_load_result_n1107;
        if (hxc_l_tmp_load_result_n1107 == NULL)
        {
          abort();
        }
        int32_t hxc_l_tmp_class_field_load_result_n1108 = (*hxc_l_tmp_load_result_n1107).hxc_waterCell;
        struct hxc_caxecraft_app_WaterRenderCounters hxc_l_tmp_call_result_n1109 = hxc_caxecraft_app_WaterRenderer_draw(hxc_l_tmp_instance_call_result_n1103, hxc_l_tmp_length_n1104, hxc_l_tmp_load_result_n1105, hxc_l_tmp_load_result_n1106, hxc_l_tmp_class_field_load_result_n1108);
        struct hxc_caxecraft_app_WaterRenderCounters hxc_l_waterCounters = hxc_l_tmp_call_result_n1109;
        int32_t hxc_l_tmp_record_field_load_result_n1110 = hxc_l_renderCounters.hxc_visible;
        int32_t hxc_l_totalVisible = hxc_i32_add_wrapping(hxc_l_tmp_record_field_load_result_n1110, hxc_l_waterCounters.hxc_visible);
        int32_t hxc_l_tmp_record_field_load_result_n1112 = hxc_l_renderCounters.hxc_drawCalls;
        int32_t hxc_l_totalDrawCalls = hxc_i32_add_wrapping(hxc_l_tmp_record_field_load_result_n1112, hxc_l_waterCounters.hxc_drawCalls);
        struct Camera3D hxc_l_tmp_load_result_n1114 = hxc_l_camera;
        struct Texture hxc_l_tmp_load_result_n1115 = hxc_l_entityTexture;
        bool hxc_l_tmp_load_result_n1116 = hxc_l_entityTextureReady;
        struct hxc_caxecraft_domain_Character hxc_l_tmp_load_result_n1117 = hxc_l_dialogueActor;
        struct hxc_caxecraft_domain_Character hxc_l_tmp_load_result_n1118 = hxc_l_enemyActor;
        enum hxc_caxecraft_domain_ActorControllerPhase hxc_l_tmp_record_field_load_result_n1119 = hxc_l_enemyPhase.hxc_phase;
        hxc_caxecraft_app_CaxecraftApp_drawActors(hxc_l_tmp_load_result_n1114, hxc_l_tmp_load_result_n1115, hxc_l_tmp_load_result_n1116, hxc_l_tmp_load_result_n1117, hxc_l_tmp_load_result_n1118, hxc_l_tmp_record_field_load_result_n1119, hxc_l_berryDrop);
        struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_tmp_load_result_n1121 = hxc_l_contentRegistry;
        hxc_l_gc_roots[173] = (const void *)hxc_l_tmp_load_result_n1121;
        struct Camera3D hxc_l_tmp_load_result_n1122 = hxc_l_camera;
        struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1123 = hxc_l_session;
        hxc_l_gc_roots[174] = (const void *)hxc_l_tmp_load_result_n1123;
        if (hxc_l_tmp_load_result_n1123 == NULL)
        {
          abort();
        }
        size_t hxc_l_tmp_length_n1125;
        const int32_t *hxc_l_tmp_instance_call_result_n1124 = hxc_caxecraft_domain_GameSession_authoredItemsView(hxc_l_tmp_load_result_n1123, &hxc_l_tmp_length_n1125);
        struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1126 = hxc_l_levelView;
        hxc_l_gc_roots[175] = (const void *)hxc_l_tmp_load_result_n1126;
        struct Texture hxc_l_tmp_load_result_n1127 = hxc_l_itemTexture;
        bool hxc_l_tmp_load_result_n1128 = hxc_l_itemTextureReady;
        struct Texture hxc_l_tmp_load_result_n1129 = hxc_l_adventureItemTexture;
        hxc_caxecraft_app_AuthoredItemRenderer_drawWorldItems(hxc_l_tmp_load_result_n1121, hxc_l_tmp_load_result_n1122, hxc_l_tmp_instance_call_result_n1124, hxc_l_tmp_length_n1125, hxc_l_tmp_load_result_n1126, hxc_l_tmp_load_result_n1127, hxc_l_tmp_load_result_n1128, hxc_l_tmp_load_result_n1129, hxc_l_adventureItemTextureReady);
        if (hxc_l_hit.hxc_hit)
        {
          double hxc_l_y = (double)hxc_l_hit.hxc_cellY + 0.5;
          double hxc_l_z = (double)hxc_l_hit.hxc_cellZ + 0.5;
          int32_t hxc_l_tmp_record_field_load_result_n1134 = hxc_l_hit.hxc_cellX;
          double hxc_l_tmp_load_result_n1135 = hxc_l_y;
          DrawCubeWires((struct Vector3){ .x = (float)((double)hxc_l_tmp_record_field_load_result_n1134 + 0.5), .y = (float)hxc_l_tmp_load_result_n1135, .z = (float)hxc_l_z }, (float)1.04, (float)1.04, (float)1.04, (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 });
        }
        EndMode3D();
        if (hxc_l_cameraWaterBlend > 0.0)
        {
          int32_t hxc_l_overlayAlpha = hxc_f64_to_i32_saturating(105.0 * hxc_l_cameraWaterBlend);
          int32_t hxc_l_tmp_native_call_result_n1141 = GetScreenWidth();
          int32_t hxc_l_width_ha00182d86e16 = (int32_t)hxc_l_tmp_native_call_result_n1141;
          int32_t hxc_l_tmp_native_call_result_n1142 = GetScreenHeight();
          int32_t hxc_l_height_hffa0aa38900c = (int32_t)hxc_l_tmp_native_call_result_n1142;
          uint8_t hxc_l_red = (uint8_t)31;
          uint8_t hxc_l_green = (uint8_t)115;
          uint8_t hxc_l_blue = (uint8_t)154;
          int32_t hxc_l_tmp_load_result_n1143 = hxc_l_overlayAlpha;
          int32_t hxc_l_tmp_conditional_result_n347 = 0;
          if (hxc_l_tmp_load_result_n1143 < 0)
          {
            hxc_l_tmp_conditional_result_n347 = 0;
          }
          else
          {
            int32_t hxc_l_tmp_load_result_n1144 = hxc_l_overlayAlpha;
            int32_t hxc_l_tmp_conditional_result_n348 = 0;
            if (hxc_l_tmp_load_result_n1144 > 255)
            {
              hxc_l_tmp_conditional_result_n348 = 255;
            }
            else
            {
              hxc_l_tmp_conditional_result_n348 = hxc_l_overlayAlpha;
            }
            hxc_l_tmp_conditional_result_n347 = hxc_l_tmp_conditional_result_n348;
          }
          uint8_t hxc_l_alpha = (uint8_t)hxc_l_tmp_conditional_result_n347;
          uint8_t hxc_l_tmp_load_result_n1148 = hxc_l_red;
          uint8_t hxc_l_tmp_load_result_n1149 = hxc_l_green;
          uint8_t hxc_l_tmp_load_result_n1150 = hxc_l_blue;
          struct Color hxc_l_this1_hce18e5371085 = (struct Color){ .r = hxc_l_tmp_load_result_n1148, .g = hxc_l_tmp_load_result_n1149, .b = hxc_l_tmp_load_result_n1150, .a = hxc_l_alpha };
          int32_t hxc_l_tmp_load_result_n1153 = hxc_l_width_ha00182d86e16;
          int32_t hxc_l_tmp_load_result_n1154 = hxc_l_height_hffa0aa38900c;
          DrawRectangle((int32_t)0, (int32_t)0, (int32_t)hxc_l_tmp_load_result_n1153, (int32_t)hxc_l_tmp_load_result_n1154, hxc_l_this1_hce18e5371085);
        }
        int32_t hxc_l_tmp_load_result_n1156 = hxc_l_totalVisible;
        int32_t hxc_l_tmp_load_result_n1157 = hxc_l_totalDrawCalls;
        int32_t hxc_l_tmp_load_result_n1158 = hxc_l_frameCount;
        int32_t hxc_l_tmp_load_result_n1159 = hxc_l_completedTicks;
        double hxc_l_tmp_record_field_load_result_n1161 = hxc_l_character.hxc_body.hxc_x;
        double hxc_l_tmp_record_field_load_result_n1162 = hxc_l_character.hxc_body.hxc_z;
        struct hxc_caxecraft_domain_VitalsState hxc_l_tmp_record_field_load_result_n1163 = hxc_l_character.hxc_vitals;
        int32_t hxc_l_tmp_load_result_n1164 = hxc_l_aquaticEquipmentCode;
        int32_t hxc_l_tmp_load_result_n1165 = hxc_l_aquaticEquipmentFrames;
        bool hxc_l_tmp_record_field_load_result_n1166 = hxc_l_character.hxc_aquatic.hxc_headSubmerged;
        int32_t hxc_l_tmp_record_field_load_result_n1167 = hxc_l_character.hxc_aquatic.hxc_breathTicks;
        int32_t hxc_l_tmp_record_field_load_result_n1168 = hxc_l_character.hxc_aquaticProfile.hxc_maximumBreathTicks;
        int32_t hxc_l_tmp_load_result_n1170 = hxc_l_placementBlockedFrames;
        int32_t hxc_l_tmp_load_result_n1171 = hxc_l_strikeHitFrames;
        int32_t hxc_l_tmp_load_result_n1172 = hxc_l_enemyDefeatedFrames;
        int32_t hxc_l_tmp_load_result_n1173 = hxc_l_enemyAttackFrames;
        int32_t hxc_l_tmp_load_result_n1174 = hxc_l_pickupFrames;
        int32_t hxc_l_tmp_load_result_n1175 = hxc_l_pickupAmount;
        int32_t hxc_l_tmp_load_result_n1176 = hxc_l_inventoryFullReason;
        int32_t hxc_l_tmp_load_result_n1177 = hxc_l_recoveryFeedback;
        int32_t hxc_l_tmp_load_result_n1178 = hxc_l_recoveryFeedbackFrames;
        bool hxc_l_tmp_load_result_n1180 = hxc_l_paused_h0114c9e4c987;
        bool hxc_l_tmp_load_result_n1181 = hxc_l_captured_h6c16dc4c7d0a;
        struct hxc_caxecraft_domain_RaycastHit hxc_l_tmp_load_result_n1182 = hxc_l_hit;
        int32_t hxc_l_tmp_load_result_n1183 = hxc_l_selectedMode;
        int32_t hxc_l_tmp_load_result_n1184 = hxc_l_locale;
        struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_load_result_n1185 = hxc_l_inventory;
        enum hxc_caxecraft_gameplay_GuidePhase hxc_l_tmp_load_result_n1186 = hxc_l_guidePhase;
        bool hxc_l_tmp_load_result_n1187 = hxc_l_guideInteractionAvailable;
        struct hxc_caxecraft_domain_Character hxc_l_tmp_load_result_n1188 = hxc_l_enemyActor;
        enum hxc_caxecraft_domain_ActorControllerPhase hxc_l_tmp_record_field_load_result_n1189 = hxc_l_enemyPhase.hxc_phase;
        hxc_string hxc_l_tmp_record_field_levelLabel_owner_n351 = hxc_l_levelLabel;
        if (hxc_string_retain(hxc_l_tmp_record_field_levelLabel_owner_n351) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_l_hudView = (struct hxc_caxecraft_app_HudView){ .hxc_character = (struct hxc_caxecraft_app_HudCharacterView){ .hxc_aquaticEquipmentCode = hxc_l_tmp_load_result_n1164, .hxc_aquaticEquipmentVisible = hxc_l_tmp_load_result_n1165 > 0, .hxc_breathTicks = hxc_l_tmp_record_field_load_result_n1167, .hxc_headSubmerged = hxc_l_tmp_record_field_load_result_n1166, .hxc_maximumBreathTicks = hxc_l_tmp_record_field_load_result_n1168, .hxc_vitals = hxc_l_tmp_record_field_load_result_n1163, .hxc_x = hxc_l_tmp_record_field_load_result_n1161, .hxc_z = hxc_l_tmp_record_field_load_result_n1162 }, .hxc_enemy = hxc_l_tmp_load_result_n1188, .hxc_enemyPhase = hxc_l_tmp_record_field_load_result_n1189, .hxc_feedback = (struct hxc_caxecraft_app_HudFeedbackView){ .hxc_enemyAttacked = hxc_l_tmp_load_result_n1173 > 0, .hxc_enemyDefeated = hxc_l_tmp_load_result_n1172 > 0, .hxc_inventoryFullReason = hxc_l_tmp_load_result_n1176, .hxc_pickedUp = hxc_l_tmp_load_result_n1174 > 0, .hxc_pickupAmount = hxc_l_tmp_load_result_n1175, .hxc_placementBlocked = hxc_l_tmp_load_result_n1170 > 0, .hxc_recoveryDecision = hxc_l_tmp_load_result_n1177, .hxc_recoveryVisible = hxc_l_tmp_load_result_n1178 > 0, .hxc_strikeHit = hxc_l_tmp_load_result_n1171 > 0 }, .hxc_guideInteractionAvailable = hxc_l_tmp_load_result_n1187, .hxc_guidePhase = hxc_l_tmp_load_result_n1186, .hxc_hit = hxc_l_tmp_load_result_n1182, .hxc_inventory = hxc_l_tmp_load_result_n1185, .hxc_levelLabel = hxc_l_tmp_record_field_levelLabel_owner_n351, .hxc_locale = hxc_l_tmp_load_result_n1184, .hxc_metrics = (struct hxc_caxecraft_app_HudMetricsView){ .hxc_completedTicks = hxc_l_tmp_load_result_n1159, .hxc_drawCalls = hxc_l_tmp_load_result_n1157, .hxc_renderedFrames = hxc_l_tmp_load_result_n1158, .hxc_visibleBlocks = hxc_l_tmp_load_result_n1156 }, .hxc_mode = hxc_l_tmp_load_result_n1183, .hxc_paused = hxc_l_tmp_load_result_n1180, .hxc_pointerCaptured = hxc_l_tmp_load_result_n1181 };
        struct hxc_caxecraft_app_HudView hxc_l_tmp_load_result_n1193 = hxc_l_hudView;
        struct hxc_caxecraft_app_HudResources hxc_l_tmp_load_result_n1194 = hxc_l_hudResources;
        struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_tmp_load_result_n1195 = hxc_l_contentRegistry;
        hxc_l_gc_roots[176] = (const void *)hxc_l_tmp_load_result_n1195;
        hxc_l_gc_roots[177] = (const void *)hxc_l_uiCatalog;
        hxc_caxecraft_app_CaxecraftApp_drawHud(hxc_l_tmp_load_result_n1193, hxc_l_tmp_load_result_n1194, hxc_l_tmp_load_result_n1195, hxc_l_uiCatalog);
        hxc_record_a9f9e1e9_destroy(&hxc_l_hudView);
      }
    }
    EndDrawing();
    hxc_l_frameCount = hxc_i32_add_wrapping(hxc_l_frameCount, 1);
    if (hxc_array_ref_release(hxc_l_presentationActorPhases) != HXC_STATUS_OK)
    {
      abort();
    }
  }
  EnableCursor();
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
  if (hxc_array_ref_release(hxc_l_initialActorPhases) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_record_f6127605_destroy(&hxc_l_receipt);
  if (hxc_string_release(&hxc_l_levelLabel) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_enum_4b68fff2_destroy(&hxc_l_g_h6904c3060e30);
  hxc_enum_fcb4401b_destroy(&hxc_l_g_h4ef171f4fe67);
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return;
}
