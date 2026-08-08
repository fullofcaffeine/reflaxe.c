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
  int32_t hxc_l_totalWidth = hxc_i32_add_wrapping(hxc_i32_multiply_wrapping(9, hxc_l_tmp_load_result_n0), hxc_i32_multiply_wrapping(8, hxc_l_gap));
  int32_t hxc_l_startX = hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_i32_subtract_wrapping(hxc_l_width, hxc_l_totalWidth), (double)2));
  int32_t hxc_l_y = hxc_i32_subtract_wrapping(hxc_l_height, 96);
  int32_t hxc_l_slot = 0;
  while (1)
  {
    if (!(hxc_l_slot < 9))
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
  hxc_string hxc_l_text_hf7097678c1af = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n71 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n74 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n81 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n82 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n84 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n85 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n86 = { 0 };
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
  struct Color hxc_l_text_hdbc09f8564af = (struct Color){ .r = 228, .g = 239, .b = 227, .a = 255 };
  int32_t hxc_l_tmp_load_result_n59 = hxc_l_centerX;
  int32_t hxc_l_tmp_load_result_n60 = hxc_l_centerY;
  int32_t hxc_l_tmp_load_result_n61 = hxc_l_centerX;
  int32_t hxc_l_tmp_load_result_n62 = hxc_l_centerY;
  DrawLine((int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n59, 8), (int32_t)hxc_l_tmp_load_result_n60, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n61, 3), (int32_t)hxc_l_tmp_load_result_n62, hxc_l_text_hdbc09f8564af);
  int32_t hxc_l_tmp_load_result_n64 = hxc_l_centerX;
  int32_t hxc_l_tmp_load_result_n65 = hxc_l_centerY;
  int32_t hxc_l_tmp_load_result_n66 = hxc_l_centerX;
  int32_t hxc_l_tmp_load_result_n67 = hxc_l_centerY;
  DrawLine((int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n64, 3), (int32_t)hxc_l_tmp_load_result_n65, (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n66, 8), (int32_t)hxc_l_tmp_load_result_n67, hxc_l_text_hdbc09f8564af);
  int32_t hxc_l_tmp_load_result_n69 = hxc_l_centerX;
  int32_t hxc_l_tmp_load_result_n70 = hxc_l_centerY;
  int32_t hxc_l_tmp_load_result_n71 = hxc_l_centerX;
  int32_t hxc_l_tmp_load_result_n72 = hxc_l_centerY;
  DrawLine((int32_t)hxc_l_tmp_load_result_n69, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n70, 8), (int32_t)hxc_l_tmp_load_result_n71, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n72, 3), hxc_l_text_hdbc09f8564af);
  int32_t hxc_l_tmp_load_result_n74 = hxc_l_centerX;
  int32_t hxc_l_tmp_load_result_n75 = hxc_l_centerY;
  int32_t hxc_l_tmp_load_result_n76 = hxc_l_centerX;
  int32_t hxc_l_tmp_load_result_n77 = hxc_l_centerY;
  DrawLine((int32_t)hxc_l_tmp_load_result_n74, (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n75, 3), (int32_t)hxc_l_tmp_load_result_n76, (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n77, 8), hxc_l_text_hdbc09f8564af);
  struct Color hxc_l_this1_h0dacb67ed803 = (struct Color){ .r = 15, .g = 27, .b = 39, .a = 220 };
  DrawRectangle((int32_t)18, (int32_t)18, (int32_t)460, (int32_t)108, hxc_l_this1_h0dacb67ed803);
  struct Color hxc_l_this1_hbfab04f34b9c = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
  DrawRectangleLines((int32_t)18, (int32_t)18, (int32_t)460, (int32_t)108, hxc_l_this1_hbfab04f34b9c);
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
  DrawText(hxc_l_tmp_string_borrow_cstring_result_n86.data, (int32_t)32, (int32_t)28, (int32_t)20, hxc_l_text_hdbc09f8564af);
  hxc_string hxc_l_text1 = hxc_l_view.hxc_levelLabel;
  if (hxc_string_retain(hxc_l_text1) != HXC_STATUS_OK)
  {
    abort();
  }
  struct Color hxc_l_this1_h189a8a7c1cd5 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
  hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n91 = HXC_BORROWED_CSTRING_INITIALIZER;
  if (hxc_string_borrow_cstring(&hxc_l_text1, &hxc_l_tmp_string_borrow_cstring_result_n91) != HXC_STATUS_OK)
  {
    abort();
  }
  DrawText(hxc_l_tmp_string_borrow_cstring_result_n91.data, (int32_t)250, (int32_t)30, (int32_t)16, hxc_l_this1_h189a8a7c1cd5);
  hxc_string hxc_l_tmp_instance_call_result_n94 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_uiCatalog, hxc_l_locale, 4);
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n49 = hxc_l_tmp_instance_call_result_n94;
  hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n96 = HXC_BORROWED_CSTRING_INITIALIZER;
  if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n49, &hxc_l_tmp_string_borrow_cstring_result_n96) != HXC_STATUS_OK)
  {
    abort();
  }
  DrawText(hxc_l_tmp_string_borrow_cstring_result_n96.data, (int32_t)32, (int32_t)58, (int32_t)14, hxc_l_text_hdbc09f8564af);
  hxc_caxecraft_app_HudDigits_drawNumber(16384, 82, 59, 5, (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 });
  hxc_string hxc_l_tmp_instance_call_result_n100 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_uiCatalog, hxc_l_locale, 8);
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n50 = hxc_l_tmp_instance_call_result_n100;
  hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n102 = HXC_BORROWED_CSTRING_INITIALIZER;
  if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n50, &hxc_l_tmp_string_borrow_cstring_result_n102) != HXC_STATUS_OK)
  {
    abort();
  }
  DrawText(hxc_l_tmp_string_borrow_cstring_result_n102.data, (int32_t)160, (int32_t)58, (int32_t)14, hxc_l_text_hdbc09f8564af);
  hxc_caxecraft_app_HudDigits_drawNumber(hxc_l_visible, 230, 59, 5, (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 });
  hxc_string hxc_l_tmp_instance_call_result_n107 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_uiCatalog, hxc_l_locale, 5);
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n51 = hxc_l_tmp_instance_call_result_n107;
  hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n109 = HXC_BORROWED_CSTRING_INITIALIZER;
  if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n51, &hxc_l_tmp_string_borrow_cstring_result_n109) != HXC_STATUS_OK)
  {
    abort();
  }
  DrawText(hxc_l_tmp_string_borrow_cstring_result_n109.data, (int32_t)326, (int32_t)58, (int32_t)14, hxc_l_text_hdbc09f8564af);
  hxc_caxecraft_app_HudDigits_drawNumber(hxc_l_drawCalls, 382, 59, 5, (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 });
  hxc_string hxc_l_tmp_instance_call_result_n114 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_uiCatalog, hxc_l_locale, 6);
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n52 = hxc_l_tmp_instance_call_result_n114;
  hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n116 = HXC_BORROWED_CSTRING_INITIALIZER;
  if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n52, &hxc_l_tmp_string_borrow_cstring_result_n116) != HXC_STATUS_OK)
  {
    abort();
  }
  DrawText(hxc_l_tmp_string_borrow_cstring_result_n116.data, (int32_t)32, (int32_t)86, (int32_t)12, hxc_l_text_hdbc09f8564af);
  int32_t hxc_l_tmp_load_result_n118 = hxc_l_frames;
  hxc_caxecraft_app_HudDigits_drawNumber(hxc_l_tmp_load_result_n118, 82, 85, 6, hxc_l_text_hdbc09f8564af);
  hxc_string hxc_l_tmp_instance_call_result_n121 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_uiCatalog, hxc_l_locale, 7);
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n53 = hxc_l_tmp_instance_call_result_n121;
  hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n123 = HXC_BORROWED_CSTRING_INITIALIZER;
  if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n53, &hxc_l_tmp_string_borrow_cstring_result_n123) != HXC_STATUS_OK)
  {
    abort();
  }
  DrawText(hxc_l_tmp_string_borrow_cstring_result_n123.data, (int32_t)174, (int32_t)86, (int32_t)12, hxc_l_text_hdbc09f8564af);
  int32_t hxc_l_tmp_load_result_n125 = hxc_l_updates;
  hxc_caxecraft_app_HudDigits_drawNumber(hxc_l_tmp_load_result_n125, 216, 85, 6, hxc_l_text_hdbc09f8564af);
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
  DrawText(hxc_l_tmp_string_borrow_cstring_result_n153.data, (int32_t)20, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n154, 22), (int32_t)14, hxc_l_text_hdbc09f8564af);
  int32_t hxc_l_tmp_load_result_n156 = hxc_l_mode;
  bool hxc_l_tmp_short_circuit_result_n55 = hxc_l_tmp_load_result_n156 == 1;
  if (hxc_l_tmp_load_result_n156 == 1)
  {
    hxc_string hxc_l_tmp_record_field_project_n157 = hxc_l_view.hxc_objectiveTitle;
    if (hxc_l_tmp_record_field_project_n157.data == NULL)
    {
      abort();
    }
    int32_t hxc_l_tmp_string_length_result_n158;
    if (hxc_string_haxe_length(hxc_l_tmp_record_field_project_n157, &hxc_l_tmp_string_length_result_n158) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_short_circuit_result_n55 = hxc_l_tmp_string_length_result_n158 > 0;
  }
  if (hxc_l_tmp_short_circuit_result_n55)
  {
    hxc_l_text_hf7097678c1af = hxc_l_view.hxc_objectiveTitle;
    if (hxc_string_retain(hxc_l_text_hf7097678c1af) != HXC_STATUS_OK)
    {
      abort();
    }
    struct Color hxc_l_this1_hc08a319e5b78 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
    hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n163 = HXC_BORROWED_CSTRING_INITIALIZER;
    if (hxc_string_borrow_cstring(&hxc_l_text_hf7097678c1af, &hxc_l_tmp_string_borrow_cstring_result_n163) != HXC_STATUS_OK)
    {
      abort();
    }
    DrawText(hxc_l_tmp_string_borrow_cstring_result_n163.data, (int32_t)32, (int32_t)110, (int32_t)14, hxc_l_this1_hc08a319e5b78);
    if (hxc_string_release(&hxc_l_text_hf7097678c1af) != HXC_STATUS_OK)
    {
      abort();
    }
  }
  if (hxc_l_guideInteractionAvailable)
  {
    struct Color hxc_l_this1_hc7c1f71ab729 = (struct Color){ .r = 15, .g = 27, .b = 39, .a = 220 };
    int32_t hxc_l_tmp_load_result_n167 = hxc_l_centerX;
    int32_t hxc_l_tmp_load_result_n168 = hxc_l_centerY;
    DrawRectangle((int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n167, 260), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n168, 54), (int32_t)520, (int32_t)60, hxc_l_this1_hc7c1f71ab729);
    if (hxc_l_guidePhase == hxc_caxecraft_gameplay_GuidePhase_Waiting)
    {
      const char *hxc_l_tmp_call_result_n173 = hxc_caxecraft_localization_FirstPlayableCatalog_text(hxc_l_locale, 13);
      int32_t hxc_l_tmp_load_result_n174 = hxc_l_centerX;
      int32_t hxc_l_tmp_load_result_n175 = hxc_l_centerY;
      DrawText(hxc_l_tmp_call_result_n173, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n174, 110), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n175, 74), (int32_t)18, hxc_l_text_hdbc09f8564af);
    }
    else
    {
      if (hxc_l_guidePhase == hxc_caxecraft_gameplay_GuidePhase_Welcomed)
      {
        const char *hxc_l_tmp_call_result_n180 = hxc_caxecraft_localization_FirstPlayableCatalog_text(hxc_l_locale, 14);
        int32_t hxc_l_tmp_load_result_n181 = hxc_l_centerX;
        int32_t hxc_l_tmp_load_result_n182 = hxc_l_centerY;
        DrawText(hxc_l_tmp_call_result_n180, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n181, 225), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n182, 74), (int32_t)16, hxc_l_text_hdbc09f8564af);
      }
      else
      {
        const char *hxc_l_tmp_call_result_n185 = hxc_caxecraft_localization_FirstPlayableCatalog_text(hxc_l_locale, 12);
        int32_t hxc_l_tmp_load_result_n186 = hxc_l_centerX;
        int32_t hxc_l_tmp_load_result_n187 = hxc_l_centerY;
        DrawText(hxc_l_tmp_call_result_n185, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n186, 205), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n187, 74), (int32_t)16, hxc_l_text_hdbc09f8564af);
      }
    }
  }
  if (hxc_l_enemy.hxc_vitals.hxc_health > 0)
  {
    if (hxc_l_enemyPhase == hxc_caxecraft_domain_ActorControllerPhase_Windup)
    {
      struct Color hxc_l_this1_h2b918229040a = (struct Color){ .r = 218, .g = 65, .b = 72, .a = 255 };
      const char *hxc_l_tmp_call_result_n194 = hxc_caxecraft_localization_FirstPlayableCatalog_text(hxc_l_locale, 11);
      int32_t hxc_l_tmp_load_result_n195 = hxc_l_width;
      DrawText(hxc_l_tmp_call_result_n194, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n195, 300), (int32_t)28, (int32_t)16, hxc_l_this1_h2b918229040a);
    }
    else
    {
      if (hxc_l_enemyPhase == hxc_caxecraft_domain_ActorControllerPhase_Chasing)
      {
        struct Color hxc_l_this1_h6356a4f687ab = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
        const char *hxc_l_tmp_call_result_n201 = hxc_caxecraft_localization_FirstPlayableCatalog_text(hxc_l_locale, 9);
        int32_t hxc_l_tmp_load_result_n202 = hxc_l_width;
        DrawText(hxc_l_tmp_call_result_n201, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n202, 180), (int32_t)28, (int32_t)16, hxc_l_this1_h6356a4f687ab);
      }
    }
  }
  if (hxc_l_strikeHit)
  {
    struct Color hxc_l_this1_hda9f09ce40ee = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
    const char *hxc_l_tmp_call_result_n207 = hxc_caxecraft_localization_FirstPlayableCatalog_text(hxc_l_locale, 7);
    int32_t hxc_l_tmp_load_result_n208 = hxc_l_centerX;
    int32_t hxc_l_tmp_load_result_n209 = hxc_l_centerY;
    DrawText(hxc_l_tmp_call_result_n207, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n208, 70), (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n209, 54), (int32_t)18, hxc_l_this1_hda9f09ce40ee);
  }
  if (hxc_l_enemyDefeated)
  {
    struct Color hxc_l_this1_h25e163bc1263 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
    const char *hxc_l_tmp_call_result_n214 = hxc_caxecraft_localization_FirstPlayableCatalog_text(hxc_l_locale, 10);
    int32_t hxc_l_tmp_load_result_n215 = hxc_l_width;
    DrawText(hxc_l_tmp_call_result_n214, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n215, 285), (int32_t)54, (int32_t)16, hxc_l_this1_h25e163bc1263);
  }
  if (hxc_l_enemyAttacked)
  {
    struct Color hxc_l_this1_hf20f074a0e38 = (struct Color){ .r = 218, .g = 65, .b = 72, .a = 255 };
    const char *hxc_l_tmp_call_result_n220 = hxc_caxecraft_localization_FirstPlayableCatalog_text(hxc_l_locale, 18);
    int32_t hxc_l_tmp_load_result_n221 = hxc_l_width;
    DrawText(hxc_l_tmp_call_result_n220, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n221, 330), (int32_t)82, (int32_t)16, hxc_l_this1_hf20f074a0e38);
  }
  if (hxc_l_pickedUp)
  {
    int32_t hxc_l_tmp_load_result_n224 = hxc_l_pickupAmount;
    int32_t hxc_l_tmp_conditional_result_n65 = 0;
    if (hxc_l_tmp_load_result_n224 == 1)
    {
      hxc_l_tmp_conditional_result_n65 = 2;
    }
    else
    {
      hxc_l_tmp_conditional_result_n65 = 3;
    }
    int32_t hxc_l_pickupMessage = hxc_l_tmp_conditional_result_n65;
    struct Color hxc_l_this1_h7cad73ad9079 = (struct Color){ .r = 174, .g = 78, .b = 136, .a = 255 };
    int32_t hxc_l_tmp_load_result_n227 = hxc_l_locale;
    const char *hxc_l_tmp_call_result_n229 = hxc_caxecraft_localization_FirstPlayableCatalog_text(hxc_l_tmp_load_result_n227, hxc_l_pickupMessage);
    int32_t hxc_l_tmp_load_result_n230 = hxc_l_centerX;
    int32_t hxc_l_tmp_load_result_n231 = hxc_l_centerY;
    DrawText(hxc_l_tmp_call_result_n229, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n230, 48), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n231, 24), (int32_t)18, hxc_l_this1_h7cad73ad9079);
  }
  if (hxc_l_inventoryFullReason == 1)
  {
    struct Color hxc_l_this1_hf853f09201cb = (struct Color){ .r = 238, .g = 113, .b = 78, .a = 255 };
    const char *hxc_l_tmp_call_result_n236 = hxc_caxecraft_localization_FirstPlayableCatalog_text(hxc_l_locale, 5);
    int32_t hxc_l_tmp_load_result_n237 = hxc_l_centerX;
    int32_t hxc_l_tmp_load_result_n238 = hxc_l_centerY;
    DrawText(hxc_l_tmp_call_result_n236, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n237, 150), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n238, 48), (int32_t)16, hxc_l_this1_hf853f09201cb);
  }
  else
  {
    if (hxc_l_inventoryFullReason == 2)
    {
      struct Color hxc_l_this1_h13a458270bb0 = (struct Color){ .r = 238, .g = 113, .b = 78, .a = 255 };
      const char *hxc_l_tmp_call_result_n243 = hxc_caxecraft_localization_FirstPlayableCatalog_text(hxc_l_locale, 6);
      int32_t hxc_l_tmp_load_result_n244 = hxc_l_centerX;
      int32_t hxc_l_tmp_load_result_n245 = hxc_l_centerY;
      DrawText(hxc_l_tmp_call_result_n243, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n244, 155), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n245, 48), (int32_t)16, hxc_l_this1_h13a458270bb0);
    }
  }
  if (hxc_l_recoveryVisible)
  {
    if (hxc_l_recoveryFeedback == 1)
    {
      struct Color hxc_l_this1_h2ae2dff91b01 = (struct Color){ .r = 94, .g = 212, .b = 136, .a = 255 };
      const char *hxc_l_tmp_call_result_n251 = hxc_caxecraft_localization_FirstPlayableCatalog_text(hxc_l_locale, 4);
      int32_t hxc_l_tmp_load_result_n252 = hxc_l_centerX;
      int32_t hxc_l_tmp_load_result_n253 = hxc_l_centerY;
      DrawText(hxc_l_tmp_call_result_n251, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n252, 88), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n253, 24), (int32_t)18, hxc_l_this1_h2ae2dff91b01);
    }
    else
    {
      if (hxc_l_recoveryFeedback == 2)
      {
        struct Color hxc_l_this1_h5abdbb509e2b = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
        hxc_string hxc_l_tmp_instance_call_result_n258 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_uiCatalog, hxc_l_locale, 25);
        hxc_l_tmp_cstring_ref_owner_owner_n71 = hxc_l_tmp_instance_call_result_n258;
        hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n260 = HXC_BORROWED_CSTRING_INITIALIZER;
        if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n71, &hxc_l_tmp_string_borrow_cstring_result_n260) != HXC_STATUS_OK)
        {
          abort();
        }
        int32_t hxc_l_tmp_load_result_n261 = hxc_l_centerX;
        int32_t hxc_l_tmp_load_result_n262 = hxc_l_centerY;
        DrawText(hxc_l_tmp_string_borrow_cstring_result_n260.data, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n261, 96), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n262, 24), (int32_t)18, hxc_l_this1_h5abdbb509e2b);
        if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n71) != HXC_STATUS_OK)
        {
          abort();
        }
      }
      else
      {
        if (hxc_l_recoveryFeedback == 3)
        {
          struct Color hxc_l_this1_hb2a173e9d887 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
          const char *hxc_l_tmp_call_result_n267 = hxc_caxecraft_localization_FirstPlayableCatalog_text(hxc_l_locale, 15);
          int32_t hxc_l_tmp_load_result_n268 = hxc_l_centerX;
          int32_t hxc_l_tmp_load_result_n269 = hxc_l_centerY;
          DrawText(hxc_l_tmp_call_result_n267, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n268, 76), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n269, 24), (int32_t)18, hxc_l_this1_hb2a173e9d887);
        }
      }
    }
  }
  if (hxc_l_aquaticEquipmentVisible)
  {
    struct Color hxc_l_this1_h4e3026473c59 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
    hxc_string hxc_l_tmp_instance_call_result_n274 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_uiCatalog, hxc_l_locale, 0);
    hxc_l_tmp_cstring_ref_owner_owner_n74 = hxc_l_tmp_instance_call_result_n274;
    hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n276 = HXC_BORROWED_CSTRING_INITIALIZER;
    if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n74, &hxc_l_tmp_string_borrow_cstring_result_n276) != HXC_STATUS_OK)
    {
      abort();
    }
    int32_t hxc_l_tmp_load_result_n277 = hxc_l_centerX;
    int32_t hxc_l_tmp_load_result_n278 = hxc_l_centerY;
    DrawText(hxc_l_tmp_string_borrow_cstring_result_n276.data, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n277, 128), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n278, 24), (int32_t)18, hxc_l_this1_h4e3026473c59);
    if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n74) != HXC_STATUS_OK)
    {
      abort();
    }
  }
  if (hxc_l_vitals.hxc_safeTicks > 15)
  {
    struct Color hxc_l_this1_h7eb4e0501246 = (struct Color){ .r = 218, .g = 65, .b = 72, .a = 255 };
    int32_t hxc_l_tmp_load_result_n282 = hxc_l_width;
    int32_t hxc_l_tmp_load_result_n283 = hxc_l_height;
    DrawRectangleLines((int32_t)4, (int32_t)4, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n282, 8), (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n283, 8), hxc_l_this1_h7eb4e0501246);
  }
  if (hxc_l_vitals.hxc_health <= 0)
  {
    struct Color hxc_l_this1_h2340f2031691 = (struct Color){ .r = 15, .g = 27, .b = 39, .a = 220 };
    int32_t hxc_l_tmp_load_result_n287 = hxc_l_centerX;
    int32_t hxc_l_tmp_load_result_n288 = hxc_l_centerY;
    DrawRectangle((int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n287, 250), (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n288, 74), (int32_t)500, (int32_t)148, hxc_l_this1_h2340f2031691);
    struct Color hxc_l_this1_h32cc22fa771e = (struct Color){ .r = 218, .g = 65, .b = 72, .a = 255 };
    int32_t hxc_l_tmp_load_result_n291 = hxc_l_centerX;
    int32_t hxc_l_tmp_load_result_n292 = hxc_l_centerY;
    DrawRectangleLines((int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n291, 250), (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n292, 74), (int32_t)500, (int32_t)148, hxc_l_this1_h32cc22fa771e);
    const char *hxc_l_tmp_call_result_n295 = hxc_caxecraft_localization_FirstPlayableCatalog_text(hxc_l_locale, 8);
    int32_t hxc_l_tmp_load_result_n296 = hxc_l_centerX;
    int32_t hxc_l_tmp_load_result_n297 = hxc_l_centerY;
    DrawText(hxc_l_tmp_call_result_n295, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n296, 122), (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n297, 42), (int32_t)24, hxc_l_text_hdbc09f8564af);
    struct Color hxc_l_this1_hfafbc8e61cd6 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
    const char *hxc_l_tmp_call_result_n301 = hxc_caxecraft_localization_FirstPlayableCatalog_text(hxc_l_locale, 16);
    int32_t hxc_l_tmp_load_result_n302 = hxc_l_centerX;
    int32_t hxc_l_tmp_load_result_n303 = hxc_l_centerY;
    DrawText(hxc_l_tmp_call_result_n301, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n302, 125), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n303, 10), (int32_t)18, hxc_l_this1_hfafbc8e61cd6);
  }
  if (hxc_l_paused)
  {
    struct Color hxc_l_this1_hd64435361e54 = (struct Color){ .r = 15, .g = 27, .b = 39, .a = 220 };
    int32_t hxc_l_tmp_load_result_n307 = hxc_l_centerX;
    int32_t hxc_l_tmp_load_result_n308 = hxc_l_centerY;
    DrawRectangle((int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n307, 170), (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n308, 48), (int32_t)340, (int32_t)96, hxc_l_this1_hd64435361e54);
    struct Color hxc_l_this1_h1e2b1f4ab5d7 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
    int32_t hxc_l_tmp_load_result_n311 = hxc_l_centerX;
    int32_t hxc_l_tmp_load_result_n312 = hxc_l_centerY;
    DrawRectangleLines((int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n311, 170), (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n312, 48), (int32_t)340, (int32_t)96, hxc_l_this1_h1e2b1f4ab5d7);
    hxc_string hxc_l_tmp_instance_call_result_n315 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_uiCatalog, hxc_l_locale, 32);
    hxc_l_tmp_cstring_ref_owner_owner_n81 = hxc_l_tmp_instance_call_result_n315;
    hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n317 = HXC_BORROWED_CSTRING_INITIALIZER;
    if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n81, &hxc_l_tmp_string_borrow_cstring_result_n317) != HXC_STATUS_OK)
    {
      abort();
    }
    int32_t hxc_l_tmp_load_result_n318 = hxc_l_centerX;
    int32_t hxc_l_tmp_load_result_n319 = hxc_l_centerY;
    DrawText(hxc_l_tmp_string_borrow_cstring_result_n317.data, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n318, 48), (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n319, 30), (int32_t)24, hxc_l_text_hdbc09f8564af);
    hxc_string hxc_l_tmp_instance_call_result_n322 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_uiCatalog, hxc_l_locale, 31);
    hxc_l_tmp_cstring_ref_owner_owner_n82 = hxc_l_tmp_instance_call_result_n322;
    hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n324 = HXC_BORROWED_CSTRING_INITIALIZER;
    if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n82, &hxc_l_tmp_string_borrow_cstring_result_n324) != HXC_STATUS_OK)
    {
      abort();
    }
    int32_t hxc_l_tmp_load_result_n325 = hxc_l_centerX;
    int32_t hxc_l_tmp_load_result_n326 = hxc_l_centerY;
    DrawText(hxc_l_tmp_string_borrow_cstring_result_n324.data, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n325, 160), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n326, 8), (int32_t)16, hxc_l_text_hdbc09f8564af);
    if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n82) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n81) != HXC_STATUS_OK)
    {
      abort();
    }
  }
  else
  {
    if (hxc_l_placementBlocked)
    {
      struct Color hxc_l_this1_h78611e163c0c = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
      hxc_string hxc_l_tmp_instance_call_result_n331 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_uiCatalog, hxc_l_locale, 33);
      hxc_l_tmp_cstring_ref_owner_owner_n84 = hxc_l_tmp_instance_call_result_n331;
      hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n333 = HXC_BORROWED_CSTRING_INITIALIZER;
      if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n84, &hxc_l_tmp_string_borrow_cstring_result_n333) != HXC_STATUS_OK)
      {
        abort();
      }
      int32_t hxc_l_tmp_load_result_n334 = hxc_l_centerX;
      int32_t hxc_l_tmp_load_result_n335 = hxc_l_centerY;
      DrawText(hxc_l_tmp_string_borrow_cstring_result_n333.data, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n334, 170), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n335, 26), (int32_t)14, hxc_l_this1_h78611e163c0c);
      if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n84) != HXC_STATUS_OK)
      {
        abort();
      }
    }
    else
    {
      if (!hxc_l_captured)
      {
        hxc_string hxc_l_tmp_instance_call_result_n339 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_uiCatalog, hxc_l_locale, 2);
        hxc_l_tmp_cstring_ref_owner_owner_n85 = hxc_l_tmp_instance_call_result_n339;
        hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n341 = HXC_BORROWED_CSTRING_INITIALIZER;
        if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n85, &hxc_l_tmp_string_borrow_cstring_result_n341) != HXC_STATUS_OK)
        {
          abort();
        }
        int32_t hxc_l_tmp_load_result_n342 = hxc_l_centerX;
        int32_t hxc_l_tmp_load_result_n343 = hxc_l_centerY;
        DrawText(hxc_l_tmp_string_borrow_cstring_result_n341.data, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n342, 90), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n343, 26), (int32_t)14, hxc_l_text_hdbc09f8564af);
        if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n85) != HXC_STATUS_OK)
        {
          abort();
        }
      }
      else
      {
        if (!hxc_l_hit.hxc_hit)
        {
          hxc_string hxc_l_tmp_instance_call_result_n347 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_uiCatalog, hxc_l_locale, 30);
          hxc_l_tmp_cstring_ref_owner_owner_n86 = hxc_l_tmp_instance_call_result_n347;
          hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n349 = HXC_BORROWED_CSTRING_INITIALIZER;
          if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n86, &hxc_l_tmp_string_borrow_cstring_result_n349) != HXC_STATUS_OK)
          {
            abort();
          }
          int32_t hxc_l_tmp_load_result_n350 = hxc_l_centerX;
          int32_t hxc_l_tmp_load_result_n351 = hxc_l_centerY;
          DrawText(hxc_l_tmp_string_borrow_cstring_result_n349.data, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n350, 105), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n351, 26), (int32_t)14, hxc_l_text_hdbc09f8564af);
          if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n86) != HXC_STATUS_OK)
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
  const void *hxc_l_gc_roots[232] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 232, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_string hxc_l_tmp_sys_println_string_argument_owner_n179 = { 0 };
  struct hxc_caxecraft_content_CampaignPackageLoadResult hxc_l_g_h6904c3060e30 = { 0 };
  struct hxc_caxecraft_content_CampaignLevelLoadResult hxc_l_g_h3a886f21d5d2 = { 0 };
  struct hxc_caxecraft_content_RuntimeLevelReceipt hxc_l_tmp_record_field_logicalPath_receiver_owner_n22 = { 0 };
  struct hxc_array_ref *hxc_l_phases = { 0 };
  hxc_string hxc_l_tmp_string_concat_left_owner_n222 = { 0 };
  hxc_string hxc_l_tmp_string_concat_left_owner_n223 = { 0 };
  hxc_string hxc_l_tmp_string_concat_right_owner_n224 = { 0 };
  hxc_string hxc_l_tmp_sys_println_string_argument_owner_n225 = { 0 };
  struct hxc_caxecraft_content_CampaignLevelLoadError hxc_l_error = { 0 };
  hxc_string hxc_l_tmp_string_concat_right_owner_n227 = { 0 };
  hxc_string hxc_l_tmp_sys_println_string_argument_owner_n228 = { 0 };
  struct hxc_caxecraft_content_CampaignPackageLoadError hxc_l_gerror = { 0 };
  hxc_string hxc_l_levelLabel = { 0 };
  struct hxc_caxecraft_content_RuntimeLevelReceipt hxc_l_tmp_record_field_logicalPath_receiver_owner_n26 = { 0 };
  struct hxc_caxecraft_domain_GameTickResult hxc_l_gameTick = { 0 };
  struct hxc_optional_caxecraft_scenario_FlowTickResult hxc_l_flow = { 0 };
  struct hxc_array_ref *hxc_l_g1_h318bce074317 = { 0 };
  struct hxc_caxecraft_scenario_FlowPresentationEvent hxc_l_event_h8f301d6d2834 = { 0 };
  struct hxc_caxecraft_scenario_FlowPresentationEvent hxc_l_tmp_array_element_owner_n275 = { 0 };
  hxc_string hxc_l_id = { 0 };
  hxc_string hxc_l_current = { 0 };
  struct hxc_array_ref *hxc_l_g1_h86761ddd48e2 = { 0 };
  struct hxc_caxecraft_domain_ActorControllerEvent hxc_l_event_h6e7c030f6037 = { 0 };
  struct hxc_caxecraft_domain_ActorControllerEvent hxc_l_tmp_array_element_owner_n293 = { 0 };
  hxc_string hxc_l_drop = { 0 };
  struct hxc_array_ref *hxc_l_actorPhases = { 0 };
  struct hxc_array_ref *hxc_l_presentationActorPhases = { 0 };
  struct hxc_caxecraft_content_RuntimeLevelReceipt hxc_l_receipt = { 0 };
  hxc_string hxc_l_tmp_sys_println_string_argument_owner_n35 = { 0 };
  hxc_string hxc_l_tmp_string_concat_right_owner_n36 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n360 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n368 = { 0 };
  hxc_string hxc_l_tmp_sys_println_string_argument_owner_n37 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n376 = { 0 };
  hxc_string hxc_l_tmp_string_concat_right_owner_n38 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n382 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n385 = { 0 };
  hxc_string hxc_l_tmp_sys_println_string_argument_owner_n39 = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n419 = { 0 };
  hxc_string hxc_l_hudView7 = { 0 };
  struct hxc_caxecraft_app_HudView hxc_l_hudView_hbd7f23c4712c = { 0 };
  struct hxc_caxecraft_content_RuntimeContentLoadResult hxc_l_g_h4ef171f4fe67 = { 0 };
  hxc_string hxc_l_currentObjectiveId = { 0 };
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
  int32_t hxc_l_selectedCampaignLevelIndex = 0;
  struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n64 = hxc_l_campaign;
  hxc_l_gc_roots[55] = (const void *)hxc_l_tmp_load_result_n64;
  hxc_l_gc_roots[56] = (const void *)NULL;
  bool hxc_l_tmp_short_circuit_result_n28 = hxc_l_tmp_load_result_n64 != NULL;
  if (hxc_l_tmp_load_result_n64 != NULL)
  {
    hxc_l_gc_roots[57] = (const void *)hxc_l_campaignLevel;
    hxc_l_gc_roots[58] = (const void *)NULL;
    hxc_l_tmp_short_circuit_result_n28 = hxc_l_campaignLevel != NULL;
  }
  if (hxc_l_tmp_short_circuit_result_n28)
  {
    int32_t hxc_l_g_hfd304fde43bf = 0;
    struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n67 = hxc_l_campaign;
    hxc_l_gc_roots[59] = (const void *)hxc_l_tmp_load_result_n67;
    if (hxc_l_tmp_load_result_n67 == NULL)
    {
      abort();
    }
    struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n68 = (*hxc_l_tmp_load_result_n67).hxc_admittedLevels;
    hxc_l_gc_roots[60] = (const void *)hxc_l_tmp_class_field_load_result_n68;
    int32_t hxc_l_tmp_array_length_result_n69;
    if (hxc_array_ref_length(hxc_l_tmp_class_field_load_result_n68, &hxc_l_tmp_array_length_result_n69) != HXC_STATUS_OK)
    {
      abort();
    }
    int32_t hxc_l_g1_ha7d8dc1c4b54 = hxc_l_tmp_array_length_result_n69;
    while (1)
    {
      int32_t hxc_l_tmp_load_result_n70 = hxc_l_g_hfd304fde43bf;
      if (!(hxc_l_tmp_load_result_n70 < hxc_l_g1_ha7d8dc1c4b54))
      {
        break;
      }
      int32_t hxc_l_tmp_increment_load_result_n72 = hxc_l_g_hfd304fde43bf;
      hxc_l_g_hfd304fde43bf = hxc_i32_add_wrapping(hxc_l_tmp_increment_load_result_n72, 1);
      int32_t hxc_l_index_h62f7caa859d1 = hxc_l_tmp_increment_load_result_n72;
      struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n73 = hxc_l_campaign;
      hxc_l_gc_roots[71] = (const void *)hxc_l_tmp_load_result_n73;
      if (hxc_l_tmp_load_result_n73 == NULL)
      {
        abort();
      }
      struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n74 = (*hxc_l_tmp_load_result_n73).hxc_admittedLevels;
      hxc_l_gc_roots[72] = (const void *)hxc_l_tmp_class_field_load_result_n74;
      struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_array_get_result_n76;
      if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n74, (size_t)hxc_l_index_h62f7caa859d1, &hxc_l_tmp_array_get_result_n76) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_l_gc_roots[73] = (const void *)hxc_l_tmp_array_get_result_n76;
      if (hxc_l_tmp_array_get_result_n76 == NULL)
      {
        abort();
      }
      hxc_string hxc_l_tmp_class_field_load_result_n77 = (*hxc_l_tmp_array_get_result_n76).hxc_id;
      struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_load_result_n78 = hxc_l_campaignLevel;
      hxc_l_gc_roots[74] = (const void *)hxc_l_tmp_load_result_n78;
      if (hxc_l_tmp_load_result_n78 == NULL)
      {
        abort();
      }
      hxc_string hxc_l_tmp_class_field_load_result_n79 = (*hxc_l_tmp_load_result_n78).hxc_id;
      if (hxc_l_tmp_class_field_load_result_n77.data == NULL || hxc_l_tmp_class_field_load_result_n79.data == NULL ? hxc_l_tmp_class_field_load_result_n77.data == hxc_l_tmp_class_field_load_result_n79.data : hxc_l_tmp_class_field_load_result_n77.byte_length == hxc_l_tmp_class_field_load_result_n79.byte_length && (hxc_l_tmp_class_field_load_result_n77.byte_length == 0 || memcmp(hxc_l_tmp_class_field_load_result_n77.data, hxc_l_tmp_class_field_load_result_n79.data, hxc_l_tmp_class_field_load_result_n77.byte_length) == 0))
      {
        hxc_l_selectedCampaignLevelIndex = hxc_l_index_h62f7caa859d1;
      }
    }
  }
  struct hxc_caxecraft_app_ActivePlayableLevel *hxc_l_tmp_load_result_n82 = hxc_l_activeLevel;
  hxc_l_gc_roots[61] = (const void *)hxc_l_tmp_load_result_n82;
  if (hxc_l_tmp_load_result_n82 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_class_field_load_result_n83 = (*hxc_l_tmp_load_result_n82).hxc_activeView;
  hxc_l_gc_roots[62] = (const void *)hxc_l_tmp_class_field_load_result_n83;
  struct hxc_caxecraft_app_PlayableLevelView *hxc_l_initialLevel = hxc_l_tmp_class_field_load_result_n83;
  struct hxc_caxecraft_app_ActivePlayableLevel *hxc_l_tmp_load_result_n84 = hxc_l_activeLevel;
  hxc_l_gc_roots[63] = (const void *)hxc_l_tmp_load_result_n84;
  if (hxc_l_tmp_load_result_n84 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_content_ActiveContent *hxc_l_tmp_class_field_load_result_n85 = (*hxc_l_tmp_load_result_n84).hxc_content;
  hxc_l_gc_roots[64] = (const void *)hxc_l_tmp_class_field_load_result_n85;
  if (hxc_l_tmp_class_field_load_result_n85 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_content_LoadedContentGeneration *hxc_l_tmp_class_field_load_result_n86 = (*hxc_l_tmp_class_field_load_result_n85).hxc_active;
  hxc_l_gc_roots[65] = (const void *)hxc_l_tmp_class_field_load_result_n86;
  if (hxc_l_tmp_class_field_load_result_n86 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_class_field_load_result_n87 = (*hxc_l_tmp_class_field_load_result_n86).hxc_ownedSession;
  hxc_l_gc_roots[66] = (const void *)hxc_l_tmp_class_field_load_result_n87;
  struct hxc_caxecraft_domain_GameSession *hxc_l_initialSession = hxc_l_tmp_class_field_load_result_n87;
  struct hxc_caxecraft_content_RuntimeLevelCandidate *hxc_l_tmp_load_result_n88 = hxc_l_loadedCandidate;
  hxc_l_gc_roots[67] = (const void *)hxc_l_tmp_load_result_n88;
  if (hxc_l_tmp_load_result_n88 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_content_RuntimeLevelReceipt hxc_l_tmp_instance_call_result_n89 = hxc_caxecraft_content_RuntimeLevelCandidate_receipt(hxc_l_tmp_load_result_n88);
  hxc_l_receipt = hxc_l_tmp_instance_call_result_n89;
  if (hxc_io_println((hxc_string){ (const uint8_t *)"caxecraft: content-source=runtime-package", 41, true, NULL }) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_string hxc_l_tmp_string_concat_result_n91 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
  if (hxc_string_concat_ref((hxc_string){ (const uint8_t *)"caxecraft: content-path=", 24, true, NULL }, hxc_l_receipt.hxc_logicalPath, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n91) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_tmp_sys_println_string_argument_owner_n35 = hxc_l_tmp_string_concat_result_n91;
  if (hxc_io_println(hxc_l_tmp_sys_println_string_argument_owner_n35) != HXC_STATUS_OK)
  {
    if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n35) != HXC_STATUS_OK)
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
  if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n35) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_string hxc_l_tmp_std_string_int_result_n94 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
  if (hxc_string_from_int32(hxc_l_receipt.hxc_inputHash, hxc_default_allocator(), &hxc_l_tmp_std_string_int_result_n94) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_tmp_string_concat_right_owner_n36 = hxc_l_tmp_std_string_int_result_n94;
  hxc_string hxc_l_tmp_string_concat_result_n96 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
  if (hxc_string_concat_ref((hxc_string){ (const uint8_t *)"caxecraft: content-input-hash=", 30, true, NULL }, hxc_l_tmp_string_concat_right_owner_n36, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n96) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_tmp_sys_println_string_argument_owner_n37 = hxc_l_tmp_string_concat_result_n96;
  if (hxc_io_println(hxc_l_tmp_sys_println_string_argument_owner_n37) != HXC_STATUS_OK)
  {
    if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n37) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n36) != HXC_STATUS_OK)
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
  if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n37) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n36) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_caxecraft_content_RuntimeContentGeneration *hxc_l_tmp_load_result_n98 = hxc_l_runtimeContent;
  hxc_l_gc_roots[68] = (const void *)hxc_l_tmp_load_result_n98;
  if (hxc_l_tmp_load_result_n98 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_content_RuntimeLevelCandidate *hxc_l_tmp_class_field_load_result_n99 = (*hxc_l_tmp_load_result_n98).hxc_loadedLevel;
  hxc_l_gc_roots[69] = (const void *)hxc_l_tmp_class_field_load_result_n99;
  if (hxc_l_tmp_class_field_load_result_n99 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_content_LoadedContentGeneration *hxc_l_tmp_class_field_load_result_n100 = (*hxc_l_tmp_class_field_load_result_n99).hxc_loadedGeneration;
  hxc_l_gc_roots[70] = (const void *)hxc_l_tmp_class_field_load_result_n100;
  if (hxc_l_tmp_class_field_load_result_n100 == NULL)
  {
    abort();
  }
  int32_t hxc_l_tmp_class_field_load_result_n101 = (*hxc_l_tmp_class_field_load_result_n100).hxc_generationIdValue;
  hxc_string hxc_l_tmp_std_string_int_result_n102 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
  if (hxc_string_from_int32(hxc_l_tmp_class_field_load_result_n101, hxc_default_allocator(), &hxc_l_tmp_std_string_int_result_n102) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_tmp_string_concat_right_owner_n38 = hxc_l_tmp_std_string_int_result_n102;
  hxc_string hxc_l_tmp_string_concat_result_n104 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
  if (hxc_string_concat_ref((hxc_string){ (const uint8_t *)"caxecraft: content-generation=", 30, true, NULL }, hxc_l_tmp_string_concat_right_owner_n38, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n104) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_tmp_sys_println_string_argument_owner_n39 = hxc_l_tmp_string_concat_result_n104;
  if (hxc_io_println(hxc_l_tmp_sys_println_string_argument_owner_n39) != HXC_STATUS_OK)
  {
    if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n39) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n38) != HXC_STATUS_OK)
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
  if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n39) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n38) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_windowFlags = hxc_i32_bit_or(64, 4);
  hxc_l_windowFlags = hxc_i32_bit_or(hxc_l_windowFlags, 8192);
  SetConfigFlags((uint32_t)hxc_l_windowFlags);
  InitWindow((int32_t)1280, (int32_t)720, "Caxecraft \342\200\224 Haxe shaped into careful C");
  bool hxc_l_tmp_native_call_result_n108 = IsWindowReady();
  if (!hxc_l_tmp_native_call_result_n108)
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
  struct Texture hxc_l_tmp_native_call_result_n109 = LoadTexture("assets/showcase/title-panorama.png");
  struct Texture hxc_l_titleTexture = hxc_l_tmp_native_call_result_n109;
  bool hxc_l_tmp_native_call_result_n111 = IsTextureValid(hxc_l_titleTexture);
  bool hxc_l_titleTextureReady = hxc_l_tmp_native_call_result_n111;
  struct Texture hxc_l_tmp_native_call_result_n112 = LoadTexture("assets/branding/caxecraft-wordmark.png");
  struct Texture hxc_l_wordmarkTexture = hxc_l_tmp_native_call_result_n112;
  bool hxc_l_tmp_native_call_result_n114 = IsTextureValid(hxc_l_wordmarkTexture);
  bool hxc_l_wordmarkTextureReady = hxc_l_tmp_native_call_result_n114;
  struct Texture hxc_l_tmp_native_call_result_n115 = LoadTexture("assets/atlases/hud.png");
  struct Texture hxc_l_hudTexture = hxc_l_tmp_native_call_result_n115;
  bool hxc_l_tmp_native_call_result_n117 = IsTextureValid(hxc_l_hudTexture);
  bool hxc_l_hudTextureReady = hxc_l_tmp_native_call_result_n117;
  struct Texture hxc_l_tmp_native_call_result_n118 = LoadTexture("assets/atlases/items.png");
  struct Texture hxc_l_itemTexture = hxc_l_tmp_native_call_result_n118;
  bool hxc_l_tmp_native_call_result_n120 = IsTextureValid(hxc_l_itemTexture);
  bool hxc_l_itemTextureReady = hxc_l_tmp_native_call_result_n120;
  struct Texture hxc_l_tmp_native_call_result_n121 = LoadTexture("assets/atlases/adventure-items.png");
  struct Texture hxc_l_adventureItemTexture = hxc_l_tmp_native_call_result_n121;
  bool hxc_l_tmp_native_call_result_n123 = IsTextureValid(hxc_l_adventureItemTexture);
  bool hxc_l_adventureItemTextureReady = hxc_l_tmp_native_call_result_n123;
  struct Texture hxc_l_tmp_load_result_n124 = hxc_l_hudTexture;
  bool hxc_l_tmp_load_result_n125 = hxc_l_hudTextureReady;
  struct Texture hxc_l_tmp_load_result_n126 = hxc_l_itemTexture;
  bool hxc_l_tmp_load_result_n127 = hxc_l_itemTextureReady;
  struct Texture hxc_l_tmp_load_result_n128 = hxc_l_adventureItemTexture;
  struct hxc_caxecraft_app_HudResources hxc_l_hudResources = (struct hxc_caxecraft_app_HudResources){ .hxc_adventureItemTexture = hxc_l_tmp_load_result_n128, .hxc_adventureItemTextureReady = hxc_l_adventureItemTextureReady, .hxc_hudTexture = hxc_l_tmp_load_result_n124, .hxc_hudTextureReady = hxc_l_tmp_load_result_n125, .hxc_itemTexture = hxc_l_tmp_load_result_n126, .hxc_itemTextureReady = hxc_l_tmp_load_result_n127 };
  struct Texture hxc_l_tmp_native_call_result_n131 = LoadTexture("assets/atlases/entities.png");
  struct Texture hxc_l_entityTexture = hxc_l_tmp_native_call_result_n131;
  bool hxc_l_tmp_native_call_result_n133 = IsTextureValid(hxc_l_entityTexture);
  bool hxc_l_entityTextureReady = hxc_l_tmp_native_call_result_n133;
  struct Texture hxc_l_tmp_native_call_result_n134 = LoadTexture("assets/atlases/terrain.png");
  struct Texture hxc_l_terrainTexture = hxc_l_tmp_native_call_result_n134;
  bool hxc_l_tmp_native_call_result_n136 = IsTextureValid(hxc_l_terrainTexture);
  bool hxc_l_terrainTextureReady = hxc_l_tmp_native_call_result_n136;
  struct Texture hxc_l_tmp_native_call_result_n137 = LoadTexture("assets/atlases/adventure-terrain.png");
  struct Texture hxc_l_adventureTerrainTexture = hxc_l_tmp_native_call_result_n137;
  bool hxc_l_tmp_native_call_result_n139 = IsTextureValid(hxc_l_adventureTerrainTexture);
  bool hxc_l_adventureTerrainTextureReady = hxc_l_tmp_native_call_result_n139;
  double hxc_l_cameraWaterBlend = 0.0;
  struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n140 = hxc_caxecraft_gameplay_Inventory_starter();
  struct hxc_caxecraft_gameplay_InventoryState hxc_l_inventory = hxc_l_tmp_call_result_n140;
  enum hxc_caxecraft_gameplay_GuidePhase hxc_l_guidePhase = hxc_caxecraft_gameplay_GuidePhase_Waiting;
  struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n142 = hxc_l_initialLevel;
  hxc_l_gc_roots[75] = (const void *)hxc_l_tmp_load_result_n142;
  if (hxc_l_tmp_load_result_n142 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_class_field_load_result_n143 = (*hxc_l_tmp_load_result_n142).hxc_levelPresentation;
  hxc_l_gc_roots[76] = (const void *)hxc_l_tmp_class_field_load_result_n143;
  if (hxc_l_tmp_class_field_load_result_n143 == NULL)
  {
    abort();
  }
  hxc_string hxc_l_tmp_class_field_load_result_n144 = (*hxc_l_tmp_class_field_load_result_n143).hxc_startingObjective;
  hxc_l_currentObjectiveId = hxc_l_tmp_class_field_load_result_n144;
  if (hxc_string_retain(hxc_l_currentObjectiveId) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n145 = hxc_l_initialSession;
  hxc_l_gc_roots[77] = (const void *)hxc_l_tmp_load_result_n145;
  if (hxc_l_tmp_load_result_n145 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n146 = hxc_l_initialLevel;
  hxc_l_gc_roots[78] = (const void *)hxc_l_tmp_load_result_n146;
  if (hxc_l_tmp_load_result_n146 == NULL)
  {
    abort();
  }
  int32_t hxc_l_tmp_class_field_load_result_n147 = (*hxc_l_tmp_load_result_n146).hxc_dialogueId;
  bool hxc_l_tmp_instance_call_result_n148 = hxc_caxecraft_domain_GameSession_actorInteractionAvailable(hxc_l_tmp_load_result_n145, hxc_l_tmp_class_field_load_result_n147);
  bool hxc_l_guideInteractionAvailable = hxc_l_tmp_instance_call_result_n148;
  struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n149 = hxc_l_initialSession;
  hxc_l_gc_roots[79] = (const void *)hxc_l_tmp_load_result_n149;
  if (hxc_l_tmp_load_result_n149 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n150 = hxc_l_initialLevel;
  hxc_l_gc_roots[80] = (const void *)hxc_l_tmp_load_result_n150;
  if (hxc_l_tmp_load_result_n150 == NULL)
  {
    abort();
  }
  int32_t hxc_l_tmp_class_field_load_result_n151 = (*hxc_l_tmp_load_result_n150).hxc_dialogueId;
  struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n152 = hxc_caxecraft_domain_GameSession_readCharacter(hxc_l_tmp_load_result_n149, hxc_l_tmp_class_field_load_result_n151);
  struct hxc_caxecraft_domain_Character hxc_l_dialogueActor = hxc_l_tmp_instance_call_result_n152;
  struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n153 = hxc_l_initialSession;
  hxc_l_gc_roots[81] = (const void *)hxc_l_tmp_load_result_n153;
  if (hxc_l_tmp_load_result_n153 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n154 = hxc_l_initialLevel;
  hxc_l_gc_roots[82] = (const void *)hxc_l_tmp_load_result_n154;
  if (hxc_l_tmp_load_result_n154 == NULL)
  {
    abort();
  }
  int32_t hxc_l_tmp_class_field_load_result_n155 = (*hxc_l_tmp_load_result_n154).hxc_enemyId;
  struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n156 = hxc_caxecraft_domain_GameSession_readCharacter(hxc_l_tmp_load_result_n153, hxc_l_tmp_class_field_load_result_n155);
  struct hxc_caxecraft_domain_Character hxc_l_enemyActor = hxc_l_tmp_instance_call_result_n156;
  struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n157 = hxc_l_initialSession;
  hxc_l_gc_roots[83] = (const void *)hxc_l_tmp_load_result_n157;
  if (hxc_l_tmp_load_result_n157 == NULL)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_tmp_instance_call_result_n158 = hxc_caxecraft_domain_GameSession_actorControllerStateSnapshots(hxc_l_tmp_load_result_n157);
  hxc_l_initialActorPhases = hxc_l_tmp_instance_call_result_n158;
  struct hxc_array_ref *hxc_l_tmp_load_result_n159 = hxc_l_initialActorPhases;
  struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n160 = hxc_l_initialLevel;
  hxc_l_gc_roots[84] = (const void *)hxc_l_tmp_load_result_n160;
  if (hxc_l_tmp_load_result_n160 == NULL)
  {
    abort();
  }
  int32_t hxc_l_tmp_class_field_load_result_n161 = (*hxc_l_tmp_load_result_n160).hxc_dialogueId;
  struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation hxc_l_tmp_call_result_n163 = hxc_caxecraft_app_CaxecraftApp_observeActorPhase(hxc_l_tmp_load_result_n159, hxc_l_tmp_class_field_load_result_n161, hxc_caxecraft_domain_ActorControllerPhase_Stationary);
  struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation hxc_l_dialoguePhase = hxc_l_tmp_call_result_n163;
  struct hxc_array_ref *hxc_l_tmp_load_result_n164 = hxc_l_initialActorPhases;
  struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n165 = hxc_l_initialLevel;
  hxc_l_gc_roots[85] = (const void *)hxc_l_tmp_load_result_n165;
  if (hxc_l_tmp_load_result_n165 == NULL)
  {
    abort();
  }
  int32_t hxc_l_tmp_class_field_load_result_n166 = (*hxc_l_tmp_load_result_n165).hxc_enemyId;
  struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation hxc_l_tmp_call_result_n168 = hxc_caxecraft_app_CaxecraftApp_observeActorPhase(hxc_l_tmp_load_result_n164, hxc_l_tmp_class_field_load_result_n166, hxc_caxecraft_domain_ActorControllerPhase_Resting);
  struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation hxc_l_enemyPhase_h70382f3bdc66 = hxc_l_tmp_call_result_n168;
  int32_t hxc_l_tmp_record_field_load_result_n169 = hxc_l_dialogueActor.hxc_id;
  bool hxc_l_tmp_short_circuit_result_n68 = hxc_l_tmp_record_field_load_result_n169 <= 0;
  if (!(hxc_l_tmp_record_field_load_result_n169 <= 0))
  {
    hxc_l_tmp_short_circuit_result_n68 = hxc_l_enemyActor.hxc_id <= 0;
  }
  bool hxc_l_tmp_short_circuit_load_result_n171 = hxc_l_tmp_short_circuit_result_n68;
  bool hxc_l_tmp_short_circuit_result_n69 = hxc_l_tmp_short_circuit_load_result_n171;
  if (!hxc_l_tmp_short_circuit_load_result_n171)
  {
    hxc_l_tmp_short_circuit_result_n69 = !hxc_l_dialoguePhase.hxc_valid;
  }
  bool hxc_l_tmp_short_circuit_load_result_n173 = hxc_l_tmp_short_circuit_result_n69;
  bool hxc_l_tmp_short_circuit_result_n70 = hxc_l_tmp_short_circuit_load_result_n173;
  if (!hxc_l_tmp_short_circuit_load_result_n173)
  {
    hxc_l_tmp_short_circuit_result_n70 = !hxc_l_enemyPhase_h70382f3bdc66.hxc_valid;
  }
  if (hxc_l_tmp_short_circuit_result_n70)
  {
    if (hxc_array_ref_release(hxc_l_initialActorPhases) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_currentObjectiveId) != HXC_STATUS_OK)
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
  struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n178 = hxc_l_initialSession;
  hxc_l_gc_roots[86] = (const void *)hxc_l_tmp_load_result_n178;
  if (hxc_l_tmp_load_result_n178 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_domain_GameView hxc_l_tmp_instance_call_result_n179 = hxc_caxecraft_domain_GameSession_view(hxc_l_tmp_load_result_n178);
  struct hxc_caxecraft_domain_GameView hxc_l_initialPresentation = hxc_l_tmp_instance_call_result_n179;
  if (!hxc_l_initialPresentation.hxc_valid)
  {
    if (hxc_array_ref_release(hxc_l_initialActorPhases) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_currentObjectiveId) != HXC_STATUS_OK)
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
  struct hxc_caxecraft_app_MotionHistory hxc_l_tmp_call_result_n182 = hxc_caxecraft_app_MotionInterpolation_start(hxc_l_initialPresentation.hxc_localPlayer.hxc_body);
  struct hxc_caxecraft_app_MotionHistory hxc_l_motionHistory = hxc_l_tmp_call_result_n182;
  bool hxc_l_jumpQueued = false;
  bool hxc_l_swordQueued = false;
  int32_t hxc_l_selectedMode = 0;
  int32_t hxc_l_locale = 0;
  bool hxc_l_showInitialTitle = true;
  enum hxc_caxecraft_app_AppScreen hxc_l_tmp_call_result_n184 = hxc_caxecraft_app_AppScreen_initialScreen(hxc_l_showInitialTitle);
  enum hxc_caxecraft_app_AppScreen hxc_l_screen = hxc_l_tmp_call_result_n184;
  bool hxc_l_tmp_call_result_n186 = hxc_caxecraft_app_AppScreen_capturesPointer(hxc_l_screen);
  if (hxc_l_tmp_call_result_n186)
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
    bool hxc_l_tmp_load_result_n187 = hxc_l_quit;
    bool hxc_l_tmp_short_circuit_result_n99 = !hxc_l_tmp_load_result_n187;
    if (!hxc_l_tmp_load_result_n187)
    {
      bool hxc_l_tmp_native_call_result_n188 = WindowShouldClose();
      hxc_l_tmp_short_circuit_result_n99 = !hxc_l_tmp_native_call_result_n188;
    }
    if (!hxc_l_tmp_short_circuit_result_n99)
    {
      break;
    }
    struct hxc_caxecraft_app_ActivePlayableLevel *hxc_l_tmp_load_result_n190 = hxc_l_activeLevel;
    hxc_l_gc_roots[87] = (const void *)hxc_l_tmp_load_result_n190;
    if (hxc_l_tmp_load_result_n190 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_class_field_load_result_n191 = (*hxc_l_tmp_load_result_n190).hxc_activeView;
    hxc_l_gc_roots[88] = (const void *)hxc_l_tmp_class_field_load_result_n191;
    struct hxc_caxecraft_app_PlayableLevelView *hxc_l_levelView = hxc_l_tmp_class_field_load_result_n191;
    struct hxc_caxecraft_app_ActivePlayableLevel *hxc_l_tmp_load_result_n192 = hxc_l_activeLevel;
    hxc_l_gc_roots[89] = (const void *)hxc_l_tmp_load_result_n192;
    if (hxc_l_tmp_load_result_n192 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_content_ActiveContent *hxc_l_tmp_class_field_load_result_n193 = (*hxc_l_tmp_load_result_n192).hxc_content;
    hxc_l_gc_roots[90] = (const void *)hxc_l_tmp_class_field_load_result_n193;
    if (hxc_l_tmp_class_field_load_result_n193 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_content_LoadedContentGeneration *hxc_l_tmp_class_field_load_result_n194 = (*hxc_l_tmp_class_field_load_result_n193).hxc_active;
    hxc_l_gc_roots[91] = (const void *)hxc_l_tmp_class_field_load_result_n194;
    if (hxc_l_tmp_class_field_load_result_n194 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_class_field_load_result_n195 = (*hxc_l_tmp_class_field_load_result_n194).hxc_ownedSession;
    hxc_l_gc_roots[92] = (const void *)hxc_l_tmp_class_field_load_result_n195;
    struct hxc_caxecraft_domain_GameSession *hxc_l_session = hxc_l_tmp_class_field_load_result_n195;
    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n196 = hxc_l_session;
    hxc_l_gc_roots[93] = (const void *)hxc_l_tmp_load_result_n196;
    if (hxc_l_tmp_load_result_n196 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_domain_GameView hxc_l_tmp_instance_call_result_n197 = hxc_caxecraft_domain_GameSession_view(hxc_l_tmp_load_result_n196);
    struct hxc_caxecraft_domain_GameView hxc_l_initialView = hxc_l_tmp_instance_call_result_n197;
    if (!hxc_l_initialView.hxc_valid)
    {
      hxc_l_quit = true;
    }
    struct hxc_caxecraft_domain_Character hxc_l_character = hxc_l_initialView.hxc_localPlayer;
    bool hxc_l_recapturedThisFrame = false;
    bool hxc_l_resetMotionThisFrame = false;
    float hxc_l_tmp_native_call_result_n200 = GetFrameTime();
    double hxc_l_frameSeconds = (double)hxc_l_tmp_native_call_result_n200;
    if (hxc_l_frameSeconds > 0.25)
    {
      hxc_l_frameSeconds = 0.25;
    }
    bool hxc_l_tmp_native_call_result_n202 = IsWindowFocused();
    bool hxc_l_focused = hxc_l_tmp_native_call_result_n202;
    bool hxc_l_tmp_call_result_n204 = hxc_caxecraft_app_AppScreen_capturesPointer(hxc_l_screen);
    bool hxc_l_captured_ha937dc084970 = hxc_l_tmp_call_result_n204;
    bool hxc_l_tmp_call_result_n206 = hxc_caxecraft_app_AppScreen_pausesSimulation(hxc_l_screen);
    bool hxc_l_paused_h81510c86da03 = hxc_l_tmp_call_result_n206;
    double hxc_l_forward = 0.0;
    double hxc_l_right = 0.0;
    bool hxc_l_tmp_native_call_result_n207 = IsKeyDown((int32_t)87);
    if (hxc_l_tmp_native_call_result_n207)
    {
      hxc_l_forward = hxc_l_forward + 1.0;
    }
    bool hxc_l_tmp_native_call_result_n209 = IsKeyDown((int32_t)83);
    if (hxc_l_tmp_native_call_result_n209)
    {
      hxc_l_forward = hxc_l_forward - 1.0;
    }
    bool hxc_l_tmp_native_call_result_n211 = IsKeyDown((int32_t)68);
    if (hxc_l_tmp_native_call_result_n211)
    {
      hxc_l_right = hxc_l_right + 1.0;
    }
    bool hxc_l_tmp_native_call_result_n213 = IsKeyDown((int32_t)65);
    if (hxc_l_tmp_native_call_result_n213)
    {
      hxc_l_right = hxc_l_right - 1.0;
    }
    double hxc_l_lookYaw_h4b427ea52cca = 0.0;
    double hxc_l_lookPitch_h341fb5abf219 = 0.0;
    if (hxc_l_captured_ha937dc084970)
    {
      struct Vector2 hxc_l_tmp_native_call_result_n216 = GetMouseDelta();
      struct Vector2 hxc_l_mouse = hxc_l_tmp_native_call_result_n216;
      hxc_l_lookYaw_h4b427ea52cca = -(double)hxc_l_mouse.x * 0.0025;
      hxc_l_lookPitch_h341fb5abf219 = -(double)hxc_l_mouse.y * 0.0025;
    }
    bool hxc_l_tmp_native_call_result_n221 = IsMouseButtonPressed((int32_t)0);
    bool hxc_l_leftPressed = hxc_l_tmp_native_call_result_n221;
    bool hxc_l_tmp_load_result_n222 = hxc_l_captured_ha937dc084970;
    bool hxc_l_tmp_short_circuit_result_n117 = hxc_l_tmp_load_result_n222;
    if (hxc_l_tmp_load_result_n222)
    {
      hxc_l_tmp_short_circuit_result_n117 = hxc_l_leftPressed;
    }
    bool hxc_l_primaryPressed_h2077966ca310 = hxc_l_tmp_short_circuit_result_n117;
    bool hxc_l_tmp_load_result_n225 = hxc_l_captured_ha937dc084970;
    bool hxc_l_tmp_short_circuit_result_n119 = hxc_l_tmp_load_result_n225;
    if (hxc_l_tmp_load_result_n225)
    {
      bool hxc_l_tmp_native_call_result_n226 = IsMouseButtonPressed((int32_t)1);
      hxc_l_tmp_short_circuit_result_n119 = hxc_l_tmp_native_call_result_n226;
    }
    bool hxc_l_secondaryPressed_h0a910fc34e11 = hxc_l_tmp_short_circuit_result_n119;
    bool hxc_l_tmp_load_result_n228 = hxc_l_captured_ha937dc084970;
    bool hxc_l_tmp_short_circuit_result_n121 = hxc_l_tmp_load_result_n228;
    if (hxc_l_tmp_load_result_n228)
    {
      bool hxc_l_tmp_native_call_result_n229 = IsKeyPressed((int32_t)69);
      hxc_l_tmp_short_circuit_result_n121 = hxc_l_tmp_native_call_result_n229;
    }
    bool hxc_l_interactPressed_hd6f15ac879ee = hxc_l_tmp_short_circuit_result_n121;
    bool hxc_l_tmp_load_result_n231 = hxc_l_captured_ha937dc084970;
    bool hxc_l_tmp_short_circuit_result_n123 = hxc_l_tmp_load_result_n231;
    if (hxc_l_tmp_load_result_n231)
    {
      bool hxc_l_tmp_native_call_result_n232 = IsKeyPressed((int32_t)78);
      hxc_l_tmp_short_circuit_result_n123 = hxc_l_tmp_native_call_result_n232;
    }
    bool hxc_l_travelPressed_h072a7f039d00 = hxc_l_tmp_short_circuit_result_n123;
    int32_t hxc_l_hotbarSelection_hbf014f0482d2 = -1;
    bool hxc_l_tmp_native_call_result_n234 = IsKeyPressed((int32_t)49);
    if (hxc_l_tmp_native_call_result_n234)
    {
      hxc_l_hotbarSelection_hbf014f0482d2 = 0;
    }
    bool hxc_l_tmp_native_call_result_n235 = IsKeyPressed((int32_t)50);
    if (hxc_l_tmp_native_call_result_n235)
    {
      hxc_l_hotbarSelection_hbf014f0482d2 = 1;
    }
    bool hxc_l_tmp_native_call_result_n236 = IsKeyPressed((int32_t)51);
    if (hxc_l_tmp_native_call_result_n236)
    {
      hxc_l_hotbarSelection_hbf014f0482d2 = 2;
    }
    bool hxc_l_tmp_native_call_result_n237 = IsKeyPressed((int32_t)52);
    if (hxc_l_tmp_native_call_result_n237)
    {
      hxc_l_hotbarSelection_hbf014f0482d2 = 3;
    }
    bool hxc_l_tmp_native_call_result_n238 = IsKeyPressed((int32_t)53);
    if (hxc_l_tmp_native_call_result_n238)
    {
      hxc_l_hotbarSelection_hbf014f0482d2 = 4;
    }
    bool hxc_l_tmp_native_call_result_n239 = IsKeyPressed((int32_t)54);
    if (hxc_l_tmp_native_call_result_n239)
    {
      hxc_l_hotbarSelection_hbf014f0482d2 = 5;
    }
    bool hxc_l_tmp_native_call_result_n240 = IsKeyPressed((int32_t)55);
    if (hxc_l_tmp_native_call_result_n240)
    {
      hxc_l_hotbarSelection_hbf014f0482d2 = 6;
    }
    bool hxc_l_tmp_native_call_result_n241 = IsKeyPressed((int32_t)56);
    if (hxc_l_tmp_native_call_result_n241)
    {
      hxc_l_hotbarSelection_hbf014f0482d2 = 7;
    }
    float hxc_l_tmp_native_call_result_n242 = GetMouseWheelMove();
    double hxc_l_wheel = (double)hxc_l_tmp_native_call_result_n242;
    int32_t hxc_l_hotbarCycle_h54a5709df70d = 0;
    if (hxc_l_wheel > 0.0)
    {
      hxc_l_hotbarCycle_h54a5709df70d = -1;
    }
    if (hxc_l_wheel < 0.0)
    {
      hxc_l_hotbarCycle_h54a5709df70d = 1;
    }
    bool hxc_l_tmp_native_call_result_n245 = IsKeyPressed((int32_t)32);
    bool hxc_l_jumpPressed_hf3b25329cd7b = hxc_l_tmp_native_call_result_n245;
    bool hxc_l_tmp_native_call_result_n246 = IsKeyPressed((int32_t)256);
    bool hxc_l_pausePressed_h459140fe9151 = hxc_l_tmp_native_call_result_n246;
    bool hxc_l_tmp_native_call_result_n247 = IsKeyPressed((int32_t)81);
    bool hxc_l_quitPressed_ha7a2ce4c2880 = hxc_l_tmp_native_call_result_n247;
    bool hxc_l_tmp_native_call_result_n248 = IsKeyDown((int32_t)340);
    bool hxc_l_descendHeld_ha5f264381ed6 = hxc_l_tmp_native_call_result_n248;
    bool hxc_l_tmp_native_call_result_n249 = IsKeyPressed((int32_t)265);
    bool hxc_l_tmp_short_circuit_result_n132 = hxc_l_tmp_native_call_result_n249;
    if (!hxc_l_tmp_native_call_result_n249)
    {
      bool hxc_l_tmp_native_call_result_n250 = IsKeyPressed((int32_t)264);
      hxc_l_tmp_short_circuit_result_n132 = hxc_l_tmp_native_call_result_n250;
    }
    bool hxc_l_menuNextPressed_hda49829cefbb = hxc_l_tmp_short_circuit_result_n132;
    bool hxc_l_tmp_native_call_result_n252 = IsKeyPressed((int32_t)257);
    bool hxc_l_menuConfirmPressed_hbb1db668e7f9 = hxc_l_tmp_native_call_result_n252;
    double hxc_l_frameInput_moveForward = hxc_l_forward;
    double hxc_l_frameInput_moveRight = hxc_l_right;
    double hxc_l_frameInput_lookYaw = hxc_l_lookYaw_h4b427ea52cca;
    double hxc_l_frameInput_lookPitch = hxc_l_lookPitch_h341fb5abf219;
    bool hxc_l_frameInput_jumpPressed = hxc_l_jumpPressed_hf3b25329cd7b;
    bool hxc_l_tmp_native_call_result_n258 = IsKeyDown((int32_t)32);
    bool hxc_l_frameInput_riseHeld = hxc_l_tmp_native_call_result_n258;
    bool hxc_l_frameInput_descendHeld = hxc_l_descendHeld_ha5f264381ed6;
    bool hxc_l_frameInput_primaryPressed = hxc_l_primaryPressed_h2077966ca310;
    bool hxc_l_frameInput_secondaryPressed = hxc_l_secondaryPressed_h0a910fc34e11;
    bool hxc_l_frameInput_interactPressed = hxc_l_interactPressed_hd6f15ac879ee;
    bool hxc_l_frameInput_travelPressed = hxc_l_travelPressed_h072a7f039d00;
    bool hxc_l_frameInput_menuNextPressed = hxc_l_menuNextPressed_hda49829cefbb;
    bool hxc_l_frameInput_menuConfirmPressed = hxc_l_menuConfirmPressed_hbb1db668e7f9;
    bool hxc_l_frameInput_pausePressed = hxc_l_pausePressed_h459140fe9151;
    bool hxc_l_tmp_load_result_n267 = hxc_l_paused_h81510c86da03;
    bool hxc_l_tmp_short_circuit_result_n149 = hxc_l_tmp_load_result_n267;
    if (hxc_l_tmp_load_result_n267)
    {
      hxc_l_tmp_short_circuit_result_n149 = hxc_l_leftPressed;
    }
    bool hxc_l_frameInput_capturePressed = hxc_l_tmp_short_circuit_result_n149;
    bool hxc_l_frameInput_quitPressed = hxc_l_quitPressed_ha7a2ce4c2880;
    int32_t hxc_l_frameInput_hotbarSelection = hxc_l_hotbarSelection_hbf014f0482d2;
    int32_t hxc_l_frameInput_hotbarCycle = hxc_l_hotbarCycle_h54a5709df70d;
    double hxc_l_moveForward = hxc_l_frameInput_moveForward;
    double hxc_l_moveRight = hxc_l_frameInput_moveRight;
    double hxc_l_lookYaw_h6269b43db395 = hxc_l_frameInput_lookYaw;
    double hxc_l_lookPitch_hf286d80d7193 = hxc_l_frameInput_lookPitch;
    bool hxc_l_jumpPressed_h0ec2ee022c5c = hxc_l_frameInput_jumpPressed;
    bool hxc_l_riseHeld = hxc_l_frameInput_riseHeld;
    bool hxc_l_primaryPressed_h86125a319e35 = hxc_l_frameInput_primaryPressed;
    bool hxc_l_secondaryPressed_he46a7f5dc259 = hxc_l_frameInput_secondaryPressed;
    bool hxc_l_interactPressed_hca6e4535614d = hxc_l_frameInput_interactPressed;
    bool hxc_l_travelPressed_h9eaa3eecd919 = hxc_l_frameInput_travelPressed;
    bool hxc_l_pausePressed_hc815dd822bf6 = hxc_l_frameInput_pausePressed;
    bool hxc_l_capturePressed = hxc_l_frameInput_capturePressed;
    bool hxc_l_quitPressed_h735cfb7844ac = hxc_l_frameInput_quitPressed;
    int32_t hxc_l_hotbarSelection_h2e5159304f3b = hxc_l_frameInput_hotbarSelection;
    int32_t hxc_l_hotbarCycle_h45973208a5fd = hxc_l_frameInput_hotbarCycle;
    bool hxc_l_menuNextPressed_h9a6c5e9cc972 = hxc_l_frameInput_menuNextPressed;
    bool hxc_l_menuConfirmPressed_hf56cce049d76 = hxc_l_frameInput_menuConfirmPressed;
    bool hxc_l_descendHeld_h2072257a4c60 = hxc_l_frameInput_descendHeld;
    hxc_l_gc_roots[94] = (const void *)NULL;
    struct hxc_caxecraft_content_CampaignLevel *hxc_l_requestedCampaignLevel = NULL;
    bool hxc_l_requestedFromCampaignMenu = false;
    bool hxc_l_tmp_load_result_n291 = hxc_l_quit;
    bool hxc_l_tmp_short_circuit_result_n173 = !hxc_l_tmp_load_result_n291;
    if (!hxc_l_tmp_load_result_n291)
    {
      enum hxc_caxecraft_app_AppScreen hxc_l_tmp_load_result_n292 = hxc_l_screen;
      bool hxc_l_tmp_call_result_n294 = hxc_caxecraft_app_TitleMenuFlow_allowsCampaignTravel(hxc_l_tmp_load_result_n292, hxc_l_selectedMode);
      hxc_l_tmp_short_circuit_result_n173 = hxc_l_tmp_call_result_n294;
    }
    bool hxc_l_tmp_short_circuit_load_result_n295 = hxc_l_tmp_short_circuit_result_n173;
    bool hxc_l_tmp_short_circuit_result_n174 = hxc_l_tmp_short_circuit_load_result_n295;
    if (hxc_l_tmp_short_circuit_load_result_n295)
    {
      hxc_l_tmp_short_circuit_result_n174 = hxc_l_travelPressed_h9eaa3eecd919;
    }
    if (hxc_l_tmp_short_circuit_result_n174)
    {
      hxc_l_gc_roots[95] = (const void *)hxc_l_campaign;
      struct hxc_caxecraft_content_CampaignManifest *hxc_l_selectedCampaign_h2f0069f67284 = hxc_l_campaign;
      hxc_l_gc_roots[96] = (const void *)hxc_l_campaignLevel;
      struct hxc_caxecraft_content_CampaignLevel *hxc_l_sourceLevel = hxc_l_campaignLevel;
      struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n300 = hxc_l_selectedCampaign_h2f0069f67284;
      hxc_l_gc_roots[97] = (const void *)hxc_l_tmp_load_result_n300;
      hxc_l_gc_roots[98] = (const void *)NULL;
      bool hxc_l_tmp_short_circuit_result_n177 = hxc_l_tmp_load_result_n300 != NULL;
      if (hxc_l_tmp_load_result_n300 != NULL)
      {
        hxc_l_gc_roots[99] = (const void *)hxc_l_sourceLevel;
        hxc_l_gc_roots[100] = (const void *)NULL;
        hxc_l_tmp_short_circuit_result_n177 = hxc_l_sourceLevel != NULL;
      }
      if (hxc_l_tmp_short_circuit_result_n177)
      {
        struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n303 = hxc_l_selectedCampaign_h2f0069f67284;
        hxc_l_gc_roots[101] = (const void *)hxc_l_tmp_load_result_n303;
        if (hxc_l_tmp_load_result_n303 == NULL)
        {
          abort();
        }
        struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_load_result_n304 = hxc_l_sourceLevel;
        hxc_l_gc_roots[102] = (const void *)hxc_l_tmp_load_result_n304;
        if (hxc_l_tmp_load_result_n304 == NULL)
        {
          abort();
        }
        hxc_string hxc_l_tmp_class_field_load_result_n305 = (*hxc_l_tmp_load_result_n304).hxc_id;
        struct hxc_caxecraft_content_CampaignTransition *hxc_l_tmp_instance_call_result_n306 = hxc_caxecraft_content_CampaignManifest_unambiguousTransitionFrom(hxc_l_tmp_load_result_n303, hxc_l_tmp_class_field_load_result_n305);
        hxc_l_gc_roots[103] = (const void *)hxc_l_tmp_instance_call_result_n306;
        struct hxc_caxecraft_content_CampaignTransition *hxc_l_transition = hxc_l_tmp_instance_call_result_n306;
        hxc_l_gc_roots[104] = (const void *)hxc_l_transition;
        hxc_l_gc_roots[105] = (const void *)NULL;
        if (hxc_l_transition == NULL)
        {
          struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_load_result_n308 = hxc_l_sourceLevel;
          hxc_l_gc_roots[106] = (const void *)hxc_l_tmp_load_result_n308;
          if (hxc_l_tmp_load_result_n308 == NULL)
          {
            abort();
          }
          hxc_string hxc_l_tmp_class_field_load_result_n309 = (*hxc_l_tmp_load_result_n308).hxc_id;
          hxc_string hxc_l_tmp_string_concat_result_n310 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
          if (hxc_string_concat_ref((hxc_string){ (const uint8_t *)"caxecraft: campaign travel requires exactly one outgoing transition from ", 73, true, NULL }, hxc_l_tmp_class_field_load_result_n309, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n310) != HXC_STATUS_OK)
          {
            abort();
          }
          hxc_l_tmp_sys_println_string_argument_owner_n179 = hxc_l_tmp_string_concat_result_n310;
          if (hxc_io_println(hxc_l_tmp_sys_println_string_argument_owner_n179) != HXC_STATUS_OK)
          {
            if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n179) != HXC_STATUS_OK)
            {
              abort();
            }
            if (hxc_array_ref_release(hxc_l_initialActorPhases) != HXC_STATUS_OK)
            {
              abort();
            }
            if (hxc_string_release(&hxc_l_currentObjectiveId) != HXC_STATUS_OK)
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
          if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n179) != HXC_STATUS_OK)
          {
            abort();
          }
        }
        else
        {
          struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n312 = hxc_l_selectedCampaign_h2f0069f67284;
          hxc_l_gc_roots[107] = (const void *)hxc_l_tmp_load_result_n312;
          if (hxc_l_tmp_load_result_n312 == NULL)
          {
            abort();
          }
          struct hxc_caxecraft_content_CampaignTransition *hxc_l_tmp_load_result_n313 = hxc_l_transition;
          hxc_l_gc_roots[108] = (const void *)hxc_l_tmp_load_result_n313;
          if (hxc_l_tmp_load_result_n313 == NULL)
          {
            abort();
          }
          hxc_string hxc_l_tmp_class_field_load_result_n314 = (*hxc_l_tmp_load_result_n313).hxc_destinationLevel;
          struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_instance_call_result_n315 = hxc_caxecraft_content_CampaignManifest_level(hxc_l_tmp_load_result_n312, hxc_l_tmp_class_field_load_result_n314);
          hxc_l_gc_roots[109] = (const void *)hxc_l_tmp_instance_call_result_n315;
          struct hxc_caxecraft_content_CampaignLevel *hxc_l_destination_h9e8f76efb430 = hxc_l_tmp_instance_call_result_n315;
          hxc_l_gc_roots[110] = (const void *)hxc_l_destination_h9e8f76efb430;
          hxc_l_gc_roots[111] = (const void *)NULL;
          if (hxc_l_destination_h9e8f76efb430 == NULL)
          {
            if (hxc_io_println((hxc_string){ (const uint8_t *)"caxecraft: campaign destination disappeared after manifest validation", 69, true, NULL }) != HXC_STATUS_OK)
            {
              abort();
            }
          }
          else
          {
            hxc_l_gc_roots[112] = (const void *)hxc_l_destination_h9e8f76efb430;
            hxc_l_requestedCampaignLevel = hxc_l_destination_h9e8f76efb430;
          }
        }
      }
    }
    bool hxc_l_tmp_call_result_n319 = hxc_caxecraft_app_AppScreen_showsCampaignSelection(hxc_l_screen);
    bool hxc_l_tmp_short_circuit_result_n181 = hxc_l_tmp_call_result_n319;
    if (hxc_l_tmp_call_result_n319)
    {
      hxc_l_tmp_short_circuit_result_n181 = hxc_l_focused;
    }
    if (hxc_l_tmp_short_circuit_result_n181)
    {
      bool hxc_l_campaignLaunchRequested = hxc_l_menuConfirmPressed_hf56cce049d76;
      bool hxc_l_campaignBackRequested = hxc_l_pausePressed_hc815dd822bf6;
      bool hxc_l_tmp_load_result_n324 = hxc_l_menuNextPressed_h9a6c5e9cc972;
      bool hxc_l_tmp_short_circuit_result_n184 = hxc_l_tmp_load_result_n324;
      if (hxc_l_tmp_load_result_n324)
      {
        hxc_l_gc_roots[113] = (const void *)hxc_l_campaign;
        hxc_l_gc_roots[114] = (const void *)NULL;
        hxc_l_tmp_short_circuit_result_n184 = hxc_l_campaign != NULL;
      }
      bool hxc_l_tmp_short_circuit_load_result_n326 = hxc_l_tmp_short_circuit_result_n184;
      bool hxc_l_tmp_short_circuit_result_n185 = hxc_l_tmp_short_circuit_load_result_n326;
      if (hxc_l_tmp_short_circuit_load_result_n326)
      {
        struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n327 = hxc_l_campaign;
        hxc_l_gc_roots[115] = (const void *)hxc_l_tmp_load_result_n327;
        if (hxc_l_tmp_load_result_n327 == NULL)
        {
          abort();
        }
        struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n328 = (*hxc_l_tmp_load_result_n327).hxc_admittedLevels;
        hxc_l_gc_roots[116] = (const void *)hxc_l_tmp_class_field_load_result_n328;
        int32_t hxc_l_tmp_array_length_result_n329;
        if (hxc_array_ref_length(hxc_l_tmp_class_field_load_result_n328, &hxc_l_tmp_array_length_result_n329) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_l_tmp_short_circuit_result_n185 = hxc_l_tmp_array_length_result_n329 > 0;
      }
      if (hxc_l_tmp_short_circuit_result_n185)
      {
        int32_t hxc_l_tmp_load_result_n331 = hxc_l_selectedCampaignLevelIndex;
        struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n332 = hxc_l_campaign;
        hxc_l_gc_roots[117] = (const void *)hxc_l_tmp_load_result_n332;
        if (hxc_l_tmp_load_result_n332 == NULL)
        {
          abort();
        }
        struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n333 = (*hxc_l_tmp_load_result_n332).hxc_admittedLevels;
        hxc_l_gc_roots[118] = (const void *)hxc_l_tmp_class_field_load_result_n333;
        int32_t hxc_l_tmp_array_length_result_n334;
        if (hxc_array_ref_length(hxc_l_tmp_class_field_load_result_n333, &hxc_l_tmp_array_length_result_n334) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_l_selectedCampaignLevelIndex = hxc_i32_modulo_zero_safe(hxc_i32_add_wrapping(hxc_l_tmp_load_result_n331, 1), hxc_l_tmp_array_length_result_n334);
      }
      bool hxc_l_tmp_native_call_result_n335 = IsKeyPressed((int32_t)76);
      if (hxc_l_tmp_native_call_result_n335)
      {
        struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_tmp_load_result_n336 = hxc_l_uiCatalog;
        hxc_l_gc_roots[119] = (const void *)hxc_l_tmp_load_result_n336;
        if (hxc_l_tmp_load_result_n336 == NULL)
        {
          abort();
        }
        int32_t hxc_l_tmp_instance_call_result_n338 = hxc_caxecraft_localization_RuntimeUiCatalog_nextLocale(hxc_l_tmp_load_result_n336, hxc_l_locale);
        hxc_l_locale = hxc_l_tmp_instance_call_result_n338;
      }
      struct Vector2 hxc_l_tmp_native_call_result_n339 = GetMousePosition();
      struct Vector2 hxc_l_campaignMouse = hxc_l_tmp_native_call_result_n339;
      double hxc_l_tmp_static_call_argument_0_n188 = (double)hxc_l_campaignMouse.x;
      double hxc_l_tmp_static_call_argument_1_n189 = (double)hxc_l_campaignMouse.y;
      int32_t hxc_l_tmp_native_call_result_n344 = GetScreenWidth();
      int32_t hxc_l_tmp_static_call_argument_2_n190 = (int32_t)hxc_l_tmp_native_call_result_n344;
      int32_t hxc_l_tmp_native_call_result_n345 = GetScreenHeight();
      int32_t hxc_l_tmp_static_call_argument_3_n191 = (int32_t)hxc_l_tmp_native_call_result_n345;
      struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n346 = hxc_l_campaign;
      hxc_l_gc_roots[120] = (const void *)hxc_l_tmp_load_result_n346;
      hxc_l_gc_roots[121] = (const void *)NULL;
      int32_t hxc_l_tmp_conditional_result_n192 = 0;
      if (hxc_l_tmp_load_result_n346 == NULL)
      {
        hxc_l_tmp_conditional_result_n192 = 0;
      }
      else
      {
        struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n347 = hxc_l_campaign;
        hxc_l_gc_roots[122] = (const void *)hxc_l_tmp_load_result_n347;
        if (hxc_l_tmp_load_result_n347 == NULL)
        {
          abort();
        }
        struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n348 = (*hxc_l_tmp_load_result_n347).hxc_admittedLevels;
        hxc_l_gc_roots[123] = (const void *)hxc_l_tmp_class_field_load_result_n348;
        int32_t hxc_l_tmp_array_length_result_n349;
        if (hxc_array_ref_length(hxc_l_tmp_class_field_load_result_n348, &hxc_l_tmp_array_length_result_n349) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_l_tmp_conditional_result_n192 = hxc_l_tmp_array_length_result_n349;
      }
      int32_t hxc_l_tmp_conditional_load_result_n350 = hxc_l_tmp_conditional_result_n192;
      double hxc_l_tmp_static_call_argument_0_load_result_n351 = hxc_l_tmp_static_call_argument_0_n188;
      double hxc_l_tmp_static_call_argument_1_load_result_n352 = hxc_l_tmp_static_call_argument_1_n189;
      int32_t hxc_l_tmp_static_call_argument_2_load_result_n353 = hxc_l_tmp_static_call_argument_2_n190;
      struct hxc_caxecraft_app_CampaignMenuHit hxc_l_tmp_call_result_n355 = hxc_caxecraft_app_CampaignMenu_selectionAt(hxc_l_tmp_static_call_argument_0_load_result_n351, hxc_l_tmp_static_call_argument_1_load_result_n352, hxc_l_tmp_static_call_argument_2_load_result_n353, hxc_l_tmp_static_call_argument_3_n191, hxc_l_tmp_conditional_load_result_n350);
      struct hxc_caxecraft_app_CampaignMenuHit hxc_l_campaignChoice = hxc_l_tmp_call_result_n355;
      bool hxc_l_tmp_native_call_result_n356 = IsMouseButtonPressed((int32_t)0);
      if (hxc_l_tmp_native_call_result_n356)
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
              int32_t hxc_l_tmp_enum_payload_project_n359 = hxc_l_campaignChoice.hxc_payload.hxc_LevelHit.hxc_index;
              int32_t hxc_l_index_h55b1c118d065 = hxc_l_tmp_enum_payload_project_n359;
              hxc_l_selectedCampaignLevelIndex = hxc_l_index_h55b1c118d065;
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
        enum hxc_caxecraft_app_AppScreen hxc_l_tmp_call_result_n363 = hxc_caxecraft_app_AppScreen_closeCampaignSelection(hxc_l_screen);
        hxc_l_screen = hxc_l_tmp_call_result_n363;
        hxc_l_accumulator = 0.0;
        hxc_l_resetMotionThisFrame = true;
      }
      else
      {
        struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n364 = hxc_l_campaign;
        hxc_l_gc_roots[124] = (const void *)hxc_l_tmp_load_result_n364;
        hxc_l_gc_roots[125] = (const void *)NULL;
        bool hxc_l_tmp_short_circuit_result_n194 = hxc_l_tmp_load_result_n364 != NULL;
        if (hxc_l_tmp_load_result_n364 != NULL)
        {
          hxc_l_tmp_short_circuit_result_n194 = hxc_l_campaignLaunchRequested;
        }
        if (hxc_l_tmp_short_circuit_result_n194)
        {
          struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n367 = hxc_l_campaign;
          hxc_l_gc_roots[126] = (const void *)hxc_l_tmp_load_result_n367;
          if (hxc_l_tmp_load_result_n367 == NULL)
          {
            abort();
          }
          struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n368 = (*hxc_l_tmp_load_result_n367).hxc_admittedLevels;
          hxc_l_gc_roots[127] = (const void *)hxc_l_tmp_class_field_load_result_n368;
          struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_array_get_result_n370;
          if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n368, (size_t)hxc_l_selectedCampaignLevelIndex, &hxc_l_tmp_array_get_result_n370) != HXC_STATUS_OK)
          {
            abort();
          }
          hxc_l_gc_roots[128] = (const void *)hxc_l_tmp_array_get_result_n370;
          hxc_l_requestedCampaignLevel = hxc_l_tmp_array_get_result_n370;
          hxc_l_requestedFromCampaignMenu = true;
        }
      }
    }
    bool hxc_l_tmp_call_result_n372 = hxc_caxecraft_app_AppScreen_showsTitle(hxc_l_screen);
    bool hxc_l_tmp_short_circuit_result_n195 = hxc_l_tmp_call_result_n372;
    if (hxc_l_tmp_call_result_n372)
    {
      hxc_l_tmp_short_circuit_result_n195 = hxc_l_focused;
    }
    if (hxc_l_tmp_short_circuit_result_n195)
    {
      enum hxc_caxecraft_app_AppScreen hxc_l_screenBeforeMenu = hxc_l_screen;
      int32_t hxc_l_modeBeforeMenu = hxc_l_selectedMode;
      enum hxc_caxecraft_app_AppScreen hxc_l_tmp_load_result_n377 = hxc_l_screen;
      struct hxc_caxecraft_app_TitleMenuState hxc_l_tmp_call_result_n379 = hxc_caxecraft_app_TitleMenuFlow_titleMenuState(hxc_l_tmp_load_result_n377, hxc_l_selectedMode);
      struct hxc_caxecraft_app_TitleMenuState hxc_l_menuState = hxc_l_tmp_call_result_n379;
      bool hxc_l_tmp_native_call_result_n380 = IsKeyPressed((int32_t)76);
      if (hxc_l_tmp_native_call_result_n380)
      {
        struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_tmp_load_result_n381 = hxc_l_uiCatalog;
        hxc_l_gc_roots[132] = (const void *)hxc_l_tmp_load_result_n381;
        if (hxc_l_tmp_load_result_n381 == NULL)
        {
          abort();
        }
        int32_t hxc_l_tmp_instance_call_result_n383 = hxc_caxecraft_localization_RuntimeUiCatalog_nextLocale(hxc_l_tmp_load_result_n381, hxc_l_locale);
        hxc_l_locale = hxc_l_tmp_instance_call_result_n383;
      }
      if (hxc_l_menuNextPressed_h9a6c5e9cc972)
      {
        struct hxc_caxecraft_app_TitleMenuState hxc_l_tmp_call_result_n386 = hxc_caxecraft_app_TitleMenuFlow_applyTitleMenuCommand(hxc_l_menuState, 1);
        hxc_l_menuState = hxc_l_tmp_call_result_n386;
      }
      struct Vector2 hxc_l_tmp_native_call_result_n387 = GetMousePosition();
      struct Vector2 hxc_l_menuMouse = hxc_l_tmp_native_call_result_n387;
      struct Vector2 hxc_l_tmp_load_result_n388 = hxc_l_menuMouse;
      struct Vector2 hxc_l_tmp_load_result_n390 = hxc_l_menuMouse;
      int32_t hxc_l_tmp_native_call_result_n392 = GetScreenWidth();
      int32_t hxc_l_tmp_native_call_result_n393 = GetScreenHeight();
      int32_t hxc_l_tmp_call_result_n394 = hxc_caxecraft_app_TitleMenu_selectionAt((double)hxc_l_tmp_load_result_n388.x, (double)hxc_l_tmp_load_result_n390.y, (int32_t)hxc_l_tmp_native_call_result_n392, (int32_t)hxc_l_tmp_native_call_result_n393);
      int32_t hxc_l_hovered = hxc_l_tmp_call_result_n394;
      if (hxc_l_hovered == 0)
      {
        struct hxc_caxecraft_app_TitleMenuState hxc_l_tmp_call_result_n397 = hxc_caxecraft_app_TitleMenuFlow_applyTitleMenuCommand(hxc_l_menuState, 2);
        hxc_l_menuState = hxc_l_tmp_call_result_n397;
      }
      if (hxc_l_hovered == 1)
      {
        struct hxc_caxecraft_app_TitleMenuState hxc_l_tmp_call_result_n400 = hxc_caxecraft_app_TitleMenuFlow_applyTitleMenuCommand(hxc_l_menuState, 3);
        hxc_l_menuState = hxc_l_tmp_call_result_n400;
      }
      int32_t hxc_l_tmp_load_result_n401 = hxc_l_hovered;
      bool hxc_l_tmp_short_circuit_result_n202 = hxc_l_tmp_load_result_n401 >= 0;
      if (hxc_l_tmp_load_result_n401 >= 0)
      {
        bool hxc_l_tmp_native_call_result_n402 = IsMouseButtonPressed((int32_t)0);
        hxc_l_tmp_short_circuit_result_n202 = hxc_l_tmp_native_call_result_n402;
      }
      bool hxc_l_clickedChoice = hxc_l_tmp_short_circuit_result_n202;
      if (hxc_l_clickedChoice)
      {
        struct hxc_caxecraft_app_TitleMenuState hxc_l_tmp_static_call_argument_0_n203 = hxc_l_menuState;
        int32_t hxc_l_tmp_load_result_n406 = hxc_l_hovered;
        int32_t hxc_l_tmp_conditional_result_n204 = 0;
        if (hxc_l_tmp_load_result_n406 == 2)
        {
          hxc_l_tmp_conditional_result_n204 = 5;
        }
        else
        {
          hxc_l_tmp_conditional_result_n204 = 4;
        }
        int32_t hxc_l_tmp_conditional_load_result_n407 = hxc_l_tmp_conditional_result_n204;
        struct hxc_caxecraft_app_TitleMenuState hxc_l_tmp_call_result_n409 = hxc_caxecraft_app_TitleMenuFlow_applyTitleMenuCommand(hxc_l_tmp_static_call_argument_0_n203, hxc_l_tmp_conditional_load_result_n407);
        hxc_l_menuState = hxc_l_tmp_call_result_n409;
      }
      else
      {
        if (hxc_l_menuConfirmPressed_hf56cce049d76)
        {
          struct hxc_caxecraft_app_TitleMenuState hxc_l_tmp_call_result_n412 = hxc_caxecraft_app_TitleMenuFlow_applyTitleMenuCommand(hxc_l_menuState, 4);
          hxc_l_menuState = hxc_l_tmp_call_result_n412;
        }
      }
      hxc_l_screen = hxc_l_menuState.hxc_screen;
      hxc_l_selectedMode = hxc_l_menuState.hxc_mode;
      bool hxc_l_tmp_call_result_n416 = hxc_caxecraft_app_AppScreen_showsCampaignSelection(hxc_l_screen);
      bool hxc_l_tmp_short_circuit_result_n205 = hxc_l_tmp_call_result_n416;
      if (hxc_l_tmp_call_result_n416)
      {
        hxc_l_gc_roots[133] = (const void *)hxc_l_campaign;
        hxc_l_gc_roots[134] = (const void *)NULL;
        hxc_l_tmp_short_circuit_result_n205 = hxc_l_campaign == NULL;
      }
      if (hxc_l_tmp_short_circuit_result_n205)
      {
        hxc_l_screen = hxc_l_screenBeforeMenu;
      }
      enum hxc_caxecraft_app_AppScreen hxc_l_tmp_load_result_n420 = hxc_l_screen;
      enum hxc_caxecraft_app_AppScreen hxc_l_tmp_load_result_n421 = hxc_l_screenBeforeMenu;
      bool hxc_l_tmp_short_circuit_result_n206 = hxc_l_tmp_load_result_n420 != hxc_l_tmp_load_result_n421;
      if (!(hxc_l_tmp_load_result_n420 != hxc_l_tmp_load_result_n421))
      {
        int32_t hxc_l_tmp_load_result_n422 = hxc_l_selectedMode;
        hxc_l_tmp_short_circuit_result_n206 = hxc_l_tmp_load_result_n422 != hxc_l_modeBeforeMenu;
      }
      if (hxc_l_tmp_short_circuit_result_n206)
      {
        hxc_l_accumulator = 0.0;
        hxc_l_resetMotionThisFrame = true;
      }
      enum hxc_caxecraft_app_AppScreen hxc_l_tmp_load_result_n425 = hxc_l_screenBeforeMenu;
      bool hxc_l_tmp_short_circuit_result_n207 = hxc_l_tmp_load_result_n425 == hxc_caxecraft_app_AppScreen_Title;
      if (hxc_l_tmp_load_result_n425 == hxc_caxecraft_app_AppScreen_Title)
      {
        bool hxc_l_tmp_call_result_n428 = hxc_caxecraft_app_AppScreen_isPlaying(hxc_l_screen);
        hxc_l_tmp_short_circuit_result_n207 = hxc_l_tmp_call_result_n428;
      }
      if (hxc_l_tmp_short_circuit_result_n207)
      {
        hxc_l_recapturedThisFrame = true;
        DisableCursor();
      }
      else
      {
        bool hxc_l_tmp_call_result_n431 = hxc_caxecraft_app_AppScreen_showsEditor(hxc_l_screen);
        if (hxc_l_tmp_call_result_n431)
        {
          EnableCursor();
        }
      }
    }
    hxc_l_gc_roots[129] = (const void *)hxc_l_requestedCampaignLevel;
    struct hxc_caxecraft_content_CampaignLevel *hxc_l_destination_h51235fba4206 = hxc_l_requestedCampaignLevel;
    hxc_l_gc_roots[130] = (const void *)hxc_l_destination_h51235fba4206;
    hxc_l_gc_roots[131] = (const void *)NULL;
    if (hxc_l_destination_h51235fba4206 != NULL)
    {
      int32_t hxc_l_tmp_record_field_load_result_n434 = hxc_l_character.hxc_id;
      int32_t hxc_l_tmp_record_field_load_result_n435 = hxc_l_character.hxc_vitals.hxc_health;
      struct hxc_caxecraft_content_LevelPlayerOptions hxc_l_playerOptions = (struct hxc_caxecraft_content_LevelPlayerOptions){ .hxc_aquaticProfile = hxc_l_character.hxc_aquaticProfile, .hxc_entityId = hxc_l_tmp_record_field_load_result_n434, .hxc_initialHealth = hxc_l_tmp_record_field_load_result_n435 };
      struct hxc_caxecraft_app_ActivePlayableLevel *hxc_l_tmp_load_result_n438 = hxc_l_activeLevel;
      hxc_l_gc_roots[135] = (const void *)hxc_l_tmp_load_result_n438;
      if (hxc_l_tmp_load_result_n438 == NULL)
      {
        abort();
      }
      struct hxc_caxecraft_content_ActiveContent *hxc_l_tmp_class_field_load_result_n439 = (*hxc_l_tmp_load_result_n438).hxc_content;
      hxc_l_gc_roots[136] = (const void *)hxc_l_tmp_class_field_load_result_n439;
      if (hxc_l_tmp_class_field_load_result_n439 == NULL)
      {
        abort();
      }
      struct hxc_caxecraft_content_LoadedContentGeneration *hxc_l_tmp_class_field_load_result_n440 = (*hxc_l_tmp_class_field_load_result_n439).hxc_active;
      hxc_l_gc_roots[137] = (const void *)hxc_l_tmp_class_field_load_result_n440;
      if (hxc_l_tmp_class_field_load_result_n440 == NULL)
      {
        abort();
      }
      int32_t hxc_l_tmp_class_field_load_result_n441 = (*hxc_l_tmp_class_field_load_result_n440).hxc_generationIdValue;
      int32_t hxc_l_nextGeneration = hxc_i32_add_wrapping(hxc_l_tmp_class_field_load_result_n441, 1);
      struct hxc_caxecraft_content_ContentPackageStore *hxc_l_tmp_load_result_n442 = hxc_l_contentStore;
      hxc_l_gc_roots[138] = (const void *)hxc_l_tmp_load_result_n442;
      struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_load_result_n443 = hxc_l_destination_h51235fba4206;
      hxc_l_gc_roots[139] = (const void *)hxc_l_tmp_load_result_n443;
      int32_t hxc_l_tmp_load_result_n444 = hxc_l_nextGeneration;
      struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_tmp_load_result_n445 = hxc_l_contentRegistry;
      hxc_l_gc_roots[140] = (const void *)hxc_l_tmp_load_result_n445;
      if (hxc_l_tmp_load_result_n445 == NULL)
      {
        abort();
      }
      struct hxc_compiler_interface_dispatch_caxecraft_scenario_ScenarioContentRegistry_value hxc_l_tmp_interface_value_n446 = (struct hxc_compiler_interface_dispatch_caxecraft_scenario_ScenarioContentRegistry_value){ .object = hxc_l_tmp_load_result_n445, .table = &hxc_itable_compiler_interface_dispatch_caxecraft_content_RuntimeContentRegistry_itable_layout_caxecraf_hb3f384641e1a61bf };
      struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_tmp_load_result_n447 = hxc_l_contentRegistry;
      hxc_l_gc_roots[141] = (const void *)hxc_l_tmp_load_result_n447;
      if (hxc_l_tmp_load_result_n447 == NULL)
      {
        abort();
      }
      struct hxc_compiler_interface_dispatch_caxecraft_content_LevelContentResolver_value hxc_l_tmp_interface_value_n448 = (struct hxc_compiler_interface_dispatch_caxecraft_content_LevelContentResolver_value){ .object = hxc_l_tmp_load_result_n447, .table = &hxc_itable_compiler_interface_dispatch_caxecraft_content_RuntimeContentRegistry_itable_layout_caxecraf_h82ca4f24ca7a7b5a };
      struct hxc_caxecraft_content_CampaignLevelLoadResult hxc_l_tmp_call_result_n450 = hxc_caxecraft_content_CampaignRuntime_loadCampaignLevel(hxc_l_tmp_load_result_n442, hxc_l_tmp_load_result_n443, hxc_l_tmp_load_result_n444, hxc_l_tmp_interface_value_n446, hxc_l_tmp_interface_value_n448, hxc_l_playerOptions);
      hxc_l_gc_roots[142] = hxc_l_tmp_call_result_n450.hxc_tag == hxc_caxecraft_content_CampaignLevelLoadResult_CampaignLevelReady ? (const void *)hxc_l_tmp_call_result_n450.hxc_payload.hxc_CampaignLevelReady.hxc_candidate : NULL;
      hxc_l_g_h3a886f21d5d2 = hxc_l_tmp_call_result_n450;
      hxc_l_gc_roots[143] = hxc_l_g_h3a886f21d5d2.hxc_tag == hxc_caxecraft_content_CampaignLevelLoadResult_CampaignLevelReady ? (const void *)hxc_l_g_h3a886f21d5d2.hxc_payload.hxc_CampaignLevelReady.hxc_candidate : NULL;
      switch (hxc_l_g_h3a886f21d5d2.hxc_tag) {
        case hxc_caxecraft_content_CampaignLevelLoadResult_CampaignLevelReady:
          {
            hxc_l_gc_roots[147] = hxc_l_g_h3a886f21d5d2.hxc_tag == hxc_caxecraft_content_CampaignLevelLoadResult_CampaignLevelReady ? (const void *)hxc_l_g_h3a886f21d5d2.hxc_payload.hxc_CampaignLevelReady.hxc_candidate : NULL;
            if (hxc_l_g_h3a886f21d5d2.hxc_tag != hxc_caxecraft_content_CampaignLevelLoadResult_CampaignLevelReady)
            {
              abort();
            }
            struct hxc_caxecraft_content_RuntimeLevelCandidate *hxc_l_tmp_enum_payload_project_n453 = hxc_l_g_h3a886f21d5d2.hxc_payload.hxc_CampaignLevelReady.hxc_candidate;
            hxc_l_gc_roots[148] = (const void *)hxc_l_tmp_enum_payload_project_n453;
            struct hxc_caxecraft_content_RuntimeLevelCandidate *hxc_l_candidate_h2a47547b7e00 = hxc_l_tmp_enum_payload_project_n453;
            struct hxc_caxecraft_app_ActivePlayableLevel *hxc_l_tmp_load_result_n454 = hxc_l_activeLevel;
            hxc_l_gc_roots[149] = (const void *)hxc_l_tmp_load_result_n454;
            if (hxc_l_tmp_load_result_n454 == NULL)
            {
              abort();
            }
            hxc_l_gc_roots[150] = (const void *)hxc_l_candidate_h2a47547b7e00;
            struct hxc_caxecraft_app_PlayableLevelPublicationResult hxc_l_tmp_instance_call_result_n456 = hxc_caxecraft_app_ActivePlayableLevel_publish(hxc_l_tmp_load_result_n454, hxc_l_candidate_h2a47547b7e00);
            struct hxc_caxecraft_app_PlayableLevelPublicationResult hxc_l_g_hc520dc5cd34a = hxc_l_tmp_instance_call_result_n456;
            switch (hxc_l_g_hc520dc5cd34a.hxc_tag) {
              case hxc_caxecraft_app_PlayableLevelPublicationResult_PlayableLevelPublished:
                {
                  if (hxc_l_g_hc520dc5cd34a.hxc_tag != hxc_caxecraft_app_PlayableLevelPublicationResult_PlayableLevelPublished)
                  {
                    abort();
                  }
                  int32_t hxc_l_tmp_enum_payload_project_n459 = hxc_l_g_hc520dc5cd34a.hxc_payload.hxc_PlayableLevelPublished.hxc_active;
                  int32_t hxc_l_selected = hxc_l_tmp_enum_payload_project_n459;
                  hxc_l_gc_roots[152] = (const void *)hxc_l_destination_h51235fba4206;
                  hxc_l_campaignLevel = hxc_l_destination_h51235fba4206;
                  struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_load_result_n461 = hxc_l_destination_h51235fba4206;
                  hxc_l_gc_roots[153] = (const void *)hxc_l_tmp_load_result_n461;
                  if (hxc_l_tmp_load_result_n461 == NULL)
                  {
                    abort();
                  }
                  hxc_string hxc_l_tmp_class_field_load_result_n462 = (*hxc_l_tmp_load_result_n461).hxc_id;
                  hxc_string hxc_l_tmp_string_assignment_replacement_owner_n215 = hxc_l_tmp_class_field_load_result_n462;
                  if (hxc_string_retain(hxc_l_tmp_string_assignment_replacement_owner_n215) != HXC_STATUS_OK)
                  {
                    abort();
                  }
                  hxc_string hxc_l_tmp_string_assignment_replacement_owned_load_result_n463 = hxc_l_tmp_string_assignment_replacement_owner_n215;
                  if (hxc_string_release(&hxc_l_levelLabel) != HXC_STATUS_OK)
                  {
                    abort();
                  }
                  hxc_l_levelLabel = hxc_l_tmp_string_assignment_replacement_owned_load_result_n463;
                  hxc_l_gc_roots[154] = (const void *)hxc_l_campaign;
                  struct hxc_caxecraft_content_CampaignManifest *hxc_l_selectedCampaign_h5d8509278d60 = hxc_l_campaign;
                  hxc_l_gc_roots[155] = (const void *)hxc_l_selectedCampaign_h5d8509278d60;
                  hxc_l_gc_roots[156] = (const void *)NULL;
                  if (hxc_l_selectedCampaign_h5d8509278d60 != NULL)
                  {
                    int32_t hxc_l_g_he2d328a22092 = 0;
                    struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n466 = hxc_l_selectedCampaign_h5d8509278d60;
                    hxc_l_gc_roots[157] = (const void *)hxc_l_tmp_load_result_n466;
                    if (hxc_l_tmp_load_result_n466 == NULL)
                    {
                      abort();
                    }
                    struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n467 = (*hxc_l_tmp_load_result_n466).hxc_admittedLevels;
                    hxc_l_gc_roots[158] = (const void *)hxc_l_tmp_class_field_load_result_n467;
                    int32_t hxc_l_tmp_array_length_result_n468;
                    if (hxc_array_ref_length(hxc_l_tmp_class_field_load_result_n467, &hxc_l_tmp_array_length_result_n468) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    int32_t hxc_l_g1_hfe83600c8bfe = hxc_l_tmp_array_length_result_n468;
                    while (1)
                    {
                      int32_t hxc_l_tmp_load_result_n469 = hxc_l_g_he2d328a22092;
                      if (!(hxc_l_tmp_load_result_n469 < hxc_l_g1_hfe83600c8bfe))
                      {
                        break;
                      }
                      int32_t hxc_l_tmp_increment_load_result_n471 = hxc_l_g_he2d328a22092;
                      hxc_l_g_he2d328a22092 = hxc_i32_add_wrapping(hxc_l_tmp_increment_load_result_n471, 1);
                      int32_t hxc_l_index_hd660d2316afa = hxc_l_tmp_increment_load_result_n471;
                      struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n472 = hxc_l_selectedCampaign_h5d8509278d60;
                      hxc_l_gc_roots[166] = (const void *)hxc_l_tmp_load_result_n472;
                      if (hxc_l_tmp_load_result_n472 == NULL)
                      {
                        abort();
                      }
                      struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n473 = (*hxc_l_tmp_load_result_n472).hxc_admittedLevels;
                      hxc_l_gc_roots[167] = (const void *)hxc_l_tmp_class_field_load_result_n473;
                      struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_array_get_result_n475;
                      if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n473, (size_t)hxc_l_index_hd660d2316afa, &hxc_l_tmp_array_get_result_n475) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      hxc_l_gc_roots[168] = (const void *)hxc_l_tmp_array_get_result_n475;
                      if (hxc_l_tmp_array_get_result_n475 == NULL)
                      {
                        abort();
                      }
                      hxc_string hxc_l_tmp_class_field_load_result_n476 = (*hxc_l_tmp_array_get_result_n475).hxc_id;
                      struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_load_result_n477 = hxc_l_destination_h51235fba4206;
                      hxc_l_gc_roots[169] = (const void *)hxc_l_tmp_load_result_n477;
                      if (hxc_l_tmp_load_result_n477 == NULL)
                      {
                        abort();
                      }
                      hxc_string hxc_l_tmp_class_field_load_result_n478 = (*hxc_l_tmp_load_result_n477).hxc_id;
                      if (hxc_l_tmp_class_field_load_result_n476.data == NULL || hxc_l_tmp_class_field_load_result_n478.data == NULL ? hxc_l_tmp_class_field_load_result_n476.data == hxc_l_tmp_class_field_load_result_n478.data : hxc_l_tmp_class_field_load_result_n476.byte_length == hxc_l_tmp_class_field_load_result_n478.byte_length && (hxc_l_tmp_class_field_load_result_n476.byte_length == 0 || memcmp(hxc_l_tmp_class_field_load_result_n476.data, hxc_l_tmp_class_field_load_result_n478.data, hxc_l_tmp_class_field_load_result_n476.byte_length) == 0))
                      {
                        hxc_l_selectedCampaignLevelIndex = hxc_l_index_hd660d2316afa;
                      }
                    }
                  }
                  struct hxc_caxecraft_app_ActivePlayableLevel *hxc_l_tmp_load_result_n481 = hxc_l_activeLevel;
                  hxc_l_gc_roots[159] = (const void *)hxc_l_tmp_load_result_n481;
                  if (hxc_l_tmp_load_result_n481 == NULL)
                  {
                    abort();
                  }
                  struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_class_field_load_result_n482 = (*hxc_l_tmp_load_result_n481).hxc_activeView;
                  hxc_l_gc_roots[160] = (const void *)hxc_l_tmp_class_field_load_result_n482;
                  hxc_l_levelView = hxc_l_tmp_class_field_load_result_n482;
                  struct hxc_caxecraft_app_ActivePlayableLevel *hxc_l_tmp_load_result_n483 = hxc_l_activeLevel;
                  hxc_l_gc_roots[161] = (const void *)hxc_l_tmp_load_result_n483;
                  if (hxc_l_tmp_load_result_n483 == NULL)
                  {
                    abort();
                  }
                  struct hxc_caxecraft_content_ActiveContent *hxc_l_tmp_class_field_load_result_n484 = (*hxc_l_tmp_load_result_n483).hxc_content;
                  hxc_l_gc_roots[162] = (const void *)hxc_l_tmp_class_field_load_result_n484;
                  if (hxc_l_tmp_class_field_load_result_n484 == NULL)
                  {
                    abort();
                  }
                  struct hxc_caxecraft_content_LoadedContentGeneration *hxc_l_tmp_class_field_load_result_n485 = (*hxc_l_tmp_class_field_load_result_n484).hxc_active;
                  hxc_l_gc_roots[163] = (const void *)hxc_l_tmp_class_field_load_result_n485;
                  if (hxc_l_tmp_class_field_load_result_n485 == NULL)
                  {
                    abort();
                  }
                  struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_class_field_load_result_n486 = (*hxc_l_tmp_class_field_load_result_n485).hxc_ownedSession;
                  hxc_l_gc_roots[164] = (const void *)hxc_l_tmp_class_field_load_result_n486;
                  hxc_l_session = hxc_l_tmp_class_field_load_result_n486;
                  struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n487 = hxc_l_session;
                  hxc_l_gc_roots[165] = (const void *)hxc_l_tmp_load_result_n487;
                  if (hxc_l_tmp_load_result_n487 == NULL)
                  {
                    abort();
                  }
                  struct hxc_caxecraft_domain_GameView hxc_l_tmp_instance_call_result_n488 = hxc_caxecraft_domain_GameSession_view(hxc_l_tmp_load_result_n487);
                  hxc_l_initialView = hxc_l_tmp_instance_call_result_n488;
                  if (!hxc_l_initialView.hxc_valid)
                  {
                    hxc_l_quit = true;
                  }
                  else
                  {
                    hxc_l_character = hxc_l_initialView.hxc_localPlayer;
                    hxc_l_guidePhase = hxc_caxecraft_gameplay_GuidePhase_Waiting;
                    struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n492 = hxc_l_levelView;
                    hxc_l_gc_roots[170] = (const void *)hxc_l_tmp_load_result_n492;
                    if (hxc_l_tmp_load_result_n492 == NULL)
                    {
                      abort();
                    }
                    struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_class_field_load_result_n493 = (*hxc_l_tmp_load_result_n492).hxc_levelPresentation;
                    hxc_l_gc_roots[171] = (const void *)hxc_l_tmp_class_field_load_result_n493;
                    if (hxc_l_tmp_class_field_load_result_n493 == NULL)
                    {
                      abort();
                    }
                    hxc_string hxc_l_tmp_class_field_load_result_n494 = (*hxc_l_tmp_class_field_load_result_n493).hxc_startingObjective;
                    hxc_string hxc_l_tmp_string_assignment_replacement_owner_n220 = hxc_l_tmp_class_field_load_result_n494;
                    if (hxc_string_retain(hxc_l_tmp_string_assignment_replacement_owner_n220) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    hxc_string hxc_l_tmp_string_assignment_replacement_owned_load_result_n495 = hxc_l_tmp_string_assignment_replacement_owner_n220;
                    if (hxc_string_release(&hxc_l_currentObjectiveId) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    hxc_l_currentObjectiveId = hxc_l_tmp_string_assignment_replacement_owned_load_result_n495;
                    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n496 = hxc_l_session;
                    hxc_l_gc_roots[172] = (const void *)hxc_l_tmp_load_result_n496;
                    if (hxc_l_tmp_load_result_n496 == NULL)
                    {
                      abort();
                    }
                    struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n497 = hxc_l_levelView;
                    hxc_l_gc_roots[173] = (const void *)hxc_l_tmp_load_result_n497;
                    if (hxc_l_tmp_load_result_n497 == NULL)
                    {
                      abort();
                    }
                    int32_t hxc_l_tmp_class_field_load_result_n498 = (*hxc_l_tmp_load_result_n497).hxc_dialogueId;
                    bool hxc_l_tmp_instance_call_result_n499 = hxc_caxecraft_domain_GameSession_actorInteractionAvailable(hxc_l_tmp_load_result_n496, hxc_l_tmp_class_field_load_result_n498);
                    hxc_l_guideInteractionAvailable = hxc_l_tmp_instance_call_result_n499;
                    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n500 = hxc_l_session;
                    hxc_l_gc_roots[174] = (const void *)hxc_l_tmp_load_result_n500;
                    if (hxc_l_tmp_load_result_n500 == NULL)
                    {
                      abort();
                    }
                    struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n501 = hxc_l_levelView;
                    hxc_l_gc_roots[175] = (const void *)hxc_l_tmp_load_result_n501;
                    if (hxc_l_tmp_load_result_n501 == NULL)
                    {
                      abort();
                    }
                    int32_t hxc_l_tmp_class_field_load_result_n502 = (*hxc_l_tmp_load_result_n501).hxc_dialogueId;
                    struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n503 = hxc_caxecraft_domain_GameSession_readCharacter(hxc_l_tmp_load_result_n500, hxc_l_tmp_class_field_load_result_n502);
                    hxc_l_dialogueActor = hxc_l_tmp_instance_call_result_n503;
                    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n504 = hxc_l_session;
                    hxc_l_gc_roots[176] = (const void *)hxc_l_tmp_load_result_n504;
                    if (hxc_l_tmp_load_result_n504 == NULL)
                    {
                      abort();
                    }
                    struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n505 = hxc_l_levelView;
                    hxc_l_gc_roots[177] = (const void *)hxc_l_tmp_load_result_n505;
                    if (hxc_l_tmp_load_result_n505 == NULL)
                    {
                      abort();
                    }
                    int32_t hxc_l_tmp_class_field_load_result_n506 = (*hxc_l_tmp_load_result_n505).hxc_enemyId;
                    struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n507 = hxc_caxecraft_domain_GameSession_readCharacter(hxc_l_tmp_load_result_n504, hxc_l_tmp_class_field_load_result_n506);
                    hxc_l_enemyActor = hxc_l_tmp_instance_call_result_n507;
                    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n508 = hxc_l_session;
                    hxc_l_gc_roots[178] = (const void *)hxc_l_tmp_load_result_n508;
                    if (hxc_l_tmp_load_result_n508 == NULL)
                    {
                      abort();
                    }
                    struct hxc_array_ref *hxc_l_tmp_instance_call_result_n509 = hxc_caxecraft_domain_GameSession_actorControllerStateSnapshots(hxc_l_tmp_load_result_n508);
                    hxc_l_phases = hxc_l_tmp_instance_call_result_n509;
                    struct hxc_array_ref *hxc_l_tmp_load_result_n510 = hxc_l_phases;
                    struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n511 = hxc_l_levelView;
                    hxc_l_gc_roots[179] = (const void *)hxc_l_tmp_load_result_n511;
                    if (hxc_l_tmp_load_result_n511 == NULL)
                    {
                      abort();
                    }
                    int32_t hxc_l_tmp_class_field_load_result_n512 = (*hxc_l_tmp_load_result_n511).hxc_dialogueId;
                    struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation hxc_l_tmp_call_result_n514 = hxc_caxecraft_app_CaxecraftApp_observeActorPhase(hxc_l_tmp_load_result_n510, hxc_l_tmp_class_field_load_result_n512, hxc_caxecraft_domain_ActorControllerPhase_Stationary);
                    hxc_l_dialoguePhase = hxc_l_tmp_call_result_n514;
                    struct hxc_array_ref *hxc_l_tmp_load_result_n515 = hxc_l_phases;
                    struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n516 = hxc_l_levelView;
                    hxc_l_gc_roots[180] = (const void *)hxc_l_tmp_load_result_n516;
                    if (hxc_l_tmp_load_result_n516 == NULL)
                    {
                      abort();
                    }
                    int32_t hxc_l_tmp_class_field_load_result_n517 = (*hxc_l_tmp_load_result_n516).hxc_enemyId;
                    struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation hxc_l_tmp_call_result_n519 = hxc_caxecraft_app_CaxecraftApp_observeActorPhase(hxc_l_tmp_load_result_n515, hxc_l_tmp_class_field_load_result_n517, hxc_caxecraft_domain_ActorControllerPhase_Resting);
                    hxc_l_enemyPhase_h70382f3bdc66 = hxc_l_tmp_call_result_n519;
                    hxc_l_swordCombat = (struct hxc_caxecraft_gameplay_SwordCombatState){ .hxc_cooldownTicks = 0 };
                    hxc_l_berryDrop = (struct hxc_caxecraft_gameplay_BerryDropState){ .hxc_active = false, .hxc_amount = 0, .hxc_x = 0.0, .hxc_y = 0.0, .hxc_z = 0.0 };
                    hxc_l_cameraWaterBlend = 0.0;
                    hxc_l_accumulator = 0.0;
                    struct hxc_caxecraft_app_MotionHistory hxc_l_tmp_call_result_n523 = hxc_caxecraft_app_MotionInterpolation_start(hxc_l_character.hxc_body);
                    hxc_l_motionHistory = hxc_l_tmp_call_result_n523;
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
                    struct hxc_caxecraft_app_TerrainRenderer *hxc_l_tmp_owned_class_field_address_n524 = &(*hxc_l_self).hxc_terrainRenderer;
                    struct hxc_caxecraft_app_TerrainChunkCache *hxc_l_tmp_owned_class_field_address_n525 = &(*hxc_l_tmp_owned_class_field_address_n524).hxc_cache;
                    if (hxc_l_tmp_owned_class_field_address_n525 == NULL)
                    {
                      abort();
                    }
                    hxc_caxecraft_app_TerrainChunkCache_invalidateAll(hxc_l_tmp_owned_class_field_address_n525);
                    hxc_l_resetMotionThisFrame = true;
                    if (hxc_l_requestedFromCampaignMenu)
                    {
                      enum hxc_caxecraft_app_AppScreen hxc_l_tmp_call_result_n528 = hxc_caxecraft_app_AppScreen_startSelectedCampaign(hxc_l_screen);
                      hxc_l_screen = hxc_l_tmp_call_result_n528;
                      hxc_l_recapturedThisFrame = true;
                      DisableCursor();
                    }
                    hxc_string hxc_l_tmp_string_concat_result_n530 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
                    if (hxc_string_concat_ref((hxc_string){ (const uint8_t *)"caxecraft: campaign-level=", 26, true, NULL }, hxc_l_levelLabel, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n530) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    hxc_l_tmp_string_concat_left_owner_n222 = hxc_l_tmp_string_concat_result_n530;
                    hxc_string hxc_l_tmp_string_concat_result_n532 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
                    if (hxc_string_concat_ref(hxc_l_tmp_string_concat_left_owner_n222, (hxc_string){ (const uint8_t *)" generation=", 12, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n532) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    hxc_l_tmp_string_concat_left_owner_n223 = hxc_l_tmp_string_concat_result_n532;
                    hxc_string hxc_l_tmp_string_concat_left_borrow_result_n533 = hxc_l_tmp_string_concat_left_owner_n223;
                    hxc_string hxc_l_tmp_std_string_int_result_n535 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
                    if (hxc_string_from_int32(hxc_l_selected, hxc_default_allocator(), &hxc_l_tmp_std_string_int_result_n535) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    hxc_l_tmp_string_concat_right_owner_n224 = hxc_l_tmp_std_string_int_result_n535;
                    hxc_string hxc_l_tmp_string_concat_result_n537 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
                    if (hxc_string_concat_ref(hxc_l_tmp_string_concat_left_borrow_result_n533, hxc_l_tmp_string_concat_right_owner_n224, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n537) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    hxc_l_tmp_sys_println_string_argument_owner_n225 = hxc_l_tmp_string_concat_result_n537;
                    if (hxc_io_println(hxc_l_tmp_sys_println_string_argument_owner_n225) != HXC_STATUS_OK)
                    {
                      if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n225) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n224) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n223) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n222) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      if (hxc_array_ref_release(hxc_l_phases) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      hxc_enum_5543d34a_destroy(&hxc_l_g_h3a886f21d5d2);
                      if (hxc_array_ref_release(hxc_l_initialActorPhases) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      if (hxc_string_release(&hxc_l_currentObjectiveId) != HXC_STATUS_OK)
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
                    if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n225) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n224) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n223) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n222) != HXC_STATUS_OK)
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
            break;
          }
        case hxc_caxecraft_content_CampaignLevelLoadResult_CampaignLevelRejected:
          {
            hxc_l_gc_roots[151] = hxc_l_g_h3a886f21d5d2.hxc_tag == hxc_caxecraft_content_CampaignLevelLoadResult_CampaignLevelReady ? (const void *)hxc_l_g_h3a886f21d5d2.hxc_payload.hxc_CampaignLevelReady.hxc_candidate : NULL;
            if (hxc_l_g_h3a886f21d5d2.hxc_tag != hxc_caxecraft_content_CampaignLevelLoadResult_CampaignLevelRejected)
            {
              abort();
            }
            struct hxc_caxecraft_content_CampaignLevelLoadError hxc_l_tmp_enum_payload_project_n540 = hxc_l_g_h3a886f21d5d2.hxc_payload.hxc_CampaignLevelRejected.hxc_error;
            hxc_l_error = hxc_l_tmp_enum_payload_project_n540;
            if (hxc_enum_40863044_retain(&hxc_l_error) != HXC_STATUS_OK)
            {
              abort();
            }
            hxc_string hxc_l_tmp_call_result_n542 = hxc_caxecraft_app_CaxecraftApp_campaignLevelLoadFailure(hxc_l_error);
            hxc_l_tmp_string_concat_right_owner_n227 = hxc_l_tmp_call_result_n542;
            hxc_string hxc_l_tmp_string_concat_result_n544 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
            if (hxc_string_concat_ref((hxc_string){ (const uint8_t *)"caxecraft: campaign level rejected: ", 36, true, NULL }, hxc_l_tmp_string_concat_right_owner_n227, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n544) != HXC_STATUS_OK)
            {
              abort();
            }
            hxc_l_tmp_sys_println_string_argument_owner_n228 = hxc_l_tmp_string_concat_result_n544;
            if (hxc_io_println(hxc_l_tmp_sys_println_string_argument_owner_n228) != HXC_STATUS_OK)
            {
              if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n228) != HXC_STATUS_OK)
              {
                abort();
              }
              if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n227) != HXC_STATUS_OK)
              {
                abort();
              }
              hxc_enum_40863044_destroy(&hxc_l_error);
              hxc_enum_5543d34a_destroy(&hxc_l_g_h3a886f21d5d2);
              if (hxc_array_ref_release(hxc_l_initialActorPhases) != HXC_STATUS_OK)
              {
                abort();
              }
              if (hxc_string_release(&hxc_l_currentObjectiveId) != HXC_STATUS_OK)
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
            if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n228) != HXC_STATUS_OK)
            {
              abort();
            }
            if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n227) != HXC_STATUS_OK)
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
      hxc_enum_5543d34a_destroy(&hxc_l_g_h3a886f21d5d2);
    }
    struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n546 = hxc_l_levelView;
    hxc_l_gc_roots[144] = (const void *)hxc_l_tmp_load_result_n546;
    if (hxc_l_tmp_load_result_n546 == NULL)
    {
      abort();
    }
    int32_t hxc_l_tmp_class_field_load_result_n547 = (*hxc_l_tmp_load_result_n546).hxc_dialogueId;
    int32_t hxc_l_dialogueActorId = hxc_l_tmp_class_field_load_result_n547;
    struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n548 = hxc_l_levelView;
    hxc_l_gc_roots[145] = (const void *)hxc_l_tmp_load_result_n548;
    if (hxc_l_tmp_load_result_n548 == NULL)
    {
      abort();
    }
    int32_t hxc_l_tmp_class_field_load_result_n549 = (*hxc_l_tmp_load_result_n548).hxc_enemyId;
    int32_t hxc_l_enemyActorId = hxc_l_tmp_class_field_load_result_n549;
    struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n550 = hxc_l_levelView;
    hxc_l_gc_roots[146] = (const void *)hxc_l_tmp_load_result_n550;
    if (hxc_l_tmp_load_result_n550 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_scenario_ScenarioTransform hxc_l_tmp_class_field_load_result_n551 = (*hxc_l_tmp_load_result_n550).hxc_playerSpawn;
    struct hxc_caxecraft_scenario_ScenarioTransform hxc_l_spawnTransform = hxc_l_tmp_class_field_load_result_n551;
    if (hxc_l_quitPressed_h735cfb7844ac)
    {
      hxc_l_quit = true;
    }
    if (hxc_l_hotbarSelection_h2e5159304f3b >= 0)
    {
      struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_load_result_n554 = hxc_l_inventory;
      struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n556 = hxc_caxecraft_gameplay_Inventory_select(hxc_l_tmp_load_result_n554, hxc_l_hotbarSelection_h2e5159304f3b);
      hxc_l_inventory = hxc_l_tmp_call_result_n556;
    }
    if (hxc_l_hotbarCycle_h45973208a5fd != 0)
    {
      struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_load_result_n558 = hxc_l_inventory;
      struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n560 = hxc_caxecraft_gameplay_Inventory_cycle(hxc_l_tmp_load_result_n558, hxc_l_hotbarCycle_h45973208a5fd);
      hxc_l_inventory = hxc_l_tmp_call_result_n560;
    }
    bool hxc_l_tmp_call_result_n562 = hxc_caxecraft_app_AppScreen_isPlaying(hxc_l_screen);
    bool hxc_l_tmp_short_circuit_result_n232 = hxc_l_tmp_call_result_n562;
    if (hxc_l_tmp_call_result_n562)
    {
      hxc_l_tmp_short_circuit_result_n232 = hxc_l_interactPressed_hca6e4535614d;
    }
    if (hxc_l_tmp_short_circuit_result_n232)
    {
      if (hxc_l_character.hxc_vitals.hxc_health <= 0)
      {
        struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n566 = hxc_l_session;
        hxc_l_gc_roots[181] = (const void *)hxc_l_tmp_load_result_n566;
        if (hxc_l_tmp_load_result_n566 == NULL)
        {
          abort();
        }
        struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n567 = hxc_l_session;
        hxc_l_gc_roots[182] = (const void *)hxc_l_tmp_load_result_n567;
        if (hxc_l_tmp_load_result_n567 == NULL)
        {
          abort();
        }
        size_t hxc_l_tmp_length_n569;
        const uint8_t *hxc_l_tmp_instance_call_result_n568 = hxc_caxecraft_domain_GameSession_worldView(hxc_l_tmp_load_result_n567, &hxc_l_tmp_length_n569);
        struct hxc_caxecraft_domain_CharacterBody hxc_l_tmp_call_result_n571 = hxc_caxecraft_app_CaxecraftApp_spawnPlayer(hxc_l_tmp_instance_call_result_n568, hxc_l_tmp_length_n569, hxc_l_spawnTransform);
        struct hxc_caxecraft_domain_LocalCharacterCommandResult hxc_l_tmp_instance_call_result_n572 = hxc_caxecraft_domain_GameSession_reviveLocalPlayerAt(hxc_l_tmp_load_result_n566, hxc_l_tmp_call_result_n571);
        struct hxc_caxecraft_domain_LocalCharacterCommandResult hxc_l_revival = hxc_l_tmp_instance_call_result_n572;
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
        struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n575 = hxc_l_session;
        hxc_l_gc_roots[183] = (const void *)hxc_l_tmp_load_result_n575;
        if (hxc_l_tmp_load_result_n575 == NULL)
        {
          abort();
        }
        bool hxc_l_tmp_instance_call_result_n577 = hxc_caxecraft_domain_GameSession_actorInteractionAvailable(hxc_l_tmp_load_result_n575, hxc_l_dialogueActorId);
        if (hxc_l_tmp_instance_call_result_n577)
        {
          struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n578 = hxc_l_session;
          hxc_l_gc_roots[184] = (const void *)hxc_l_tmp_load_result_n578;
          if (hxc_l_tmp_load_result_n578 == NULL)
          {
            abort();
          }
          bool hxc_l_tmp_instance_call_result_n580 = hxc_caxecraft_domain_GameSession_interactWithActor(hxc_l_tmp_load_result_n578, hxc_l_dialogueActorId);
          if (!hxc_l_tmp_instance_call_result_n580)
          {
            hxc_l_quit = true;
          }
          else
          {
            bool hxc_l_sharesBerries = hxc_l_guidePhase == hxc_caxecraft_gameplay_GuidePhase_Welcomed;
            if (hxc_l_sharesBerries)
            {
              int32_t hxc_l_tmp_call_result_n586 = hxc_caxecraft_gameplay_Inventory_acceptedAmount(hxc_l_inventory, hxc_caxecraft_gameplay_ItemKind_Berries, 2);
              int32_t hxc_l_acceptedGift = hxc_l_tmp_call_result_n586;
              if (hxc_l_acceptedGift == 2)
              {
                struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_load_result_n588 = hxc_l_inventory;
                struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n591 = hxc_caxecraft_gameplay_Inventory_collectItem(hxc_l_tmp_load_result_n588, hxc_caxecraft_gameplay_ItemKind_Berries, hxc_l_acceptedGift);
                hxc_l_inventory = hxc_l_tmp_call_result_n591;
                enum hxc_caxecraft_gameplay_GuidePhase hxc_l_tmp_call_result_n593 = hxc_caxecraft_app_CaxecraftApp_advanceGuidePhase(hxc_l_guidePhase);
                hxc_l_guidePhase = hxc_l_tmp_call_result_n593;
              }
              else
              {
                hxc_l_inventoryFullReason = 1;
                hxc_l_inventoryFullFrames = 90;
              }
            }
            else
            {
              enum hxc_caxecraft_gameplay_GuidePhase hxc_l_tmp_call_result_n595 = hxc_caxecraft_app_CaxecraftApp_advanceGuidePhase(hxc_l_guidePhase);
              hxc_l_guidePhase = hxc_l_tmp_call_result_n595;
            }
          }
        }
      }
    }
    bool hxc_l_tmp_load_result_n596 = hxc_l_focused;
    bool hxc_l_tmp_short_circuit_result_n236 = !hxc_l_tmp_load_result_n596;
    if (!hxc_l_tmp_load_result_n596)
    {
      bool hxc_l_tmp_call_result_n598 = hxc_caxecraft_app_AppScreen_isPlaying(hxc_l_screen);
      hxc_l_tmp_short_circuit_result_n236 = hxc_l_tmp_call_result_n598;
    }
    if (hxc_l_tmp_short_circuit_result_n236)
    {
      enum hxc_caxecraft_app_AppScreen hxc_l_tmp_call_result_n601 = hxc_caxecraft_app_AppScreen_loseFocus(hxc_l_screen);
      hxc_l_screen = hxc_l_tmp_call_result_n601;
      hxc_l_jumpQueued = false;
      hxc_l_accumulator = 0.0;
      hxc_l_resetMotionThisFrame = true;
      EnableCursor();
    }
    bool hxc_l_tmp_call_result_n603 = hxc_caxecraft_app_AppScreen_showsTitle(hxc_l_screen);
    bool hxc_l_tmp_short_circuit_result_n237 = !hxc_l_tmp_call_result_n603;
    if (!hxc_l_tmp_call_result_n603)
    {
      bool hxc_l_tmp_call_result_n605 = hxc_caxecraft_app_AppScreen_showsCampaignSelection(hxc_l_screen);
      hxc_l_tmp_short_circuit_result_n237 = !hxc_l_tmp_call_result_n605;
    }
    bool hxc_l_tmp_short_circuit_load_result_n606 = hxc_l_tmp_short_circuit_result_n237;
    bool hxc_l_tmp_short_circuit_result_n238 = hxc_l_tmp_short_circuit_load_result_n606;
    if (hxc_l_tmp_short_circuit_load_result_n606)
    {
      bool hxc_l_tmp_call_result_n608 = hxc_caxecraft_app_AppScreen_showsEditor(hxc_l_screen);
      hxc_l_tmp_short_circuit_result_n238 = !hxc_l_tmp_call_result_n608;
    }
    bool hxc_l_tmp_short_circuit_load_result_n609 = hxc_l_tmp_short_circuit_result_n238;
    bool hxc_l_tmp_short_circuit_result_n239 = hxc_l_tmp_short_circuit_load_result_n609;
    if (hxc_l_tmp_short_circuit_load_result_n609)
    {
      hxc_l_tmp_short_circuit_result_n239 = hxc_l_focused;
    }
    bool hxc_l_tmp_short_circuit_load_result_n611 = hxc_l_tmp_short_circuit_result_n239;
    bool hxc_l_tmp_short_circuit_result_n240 = hxc_l_tmp_short_circuit_load_result_n611;
    if (hxc_l_tmp_short_circuit_load_result_n611)
    {
      hxc_l_tmp_short_circuit_result_n240 = hxc_l_pausePressed_hc815dd822bf6;
    }
    if (hxc_l_tmp_short_circuit_result_n240)
    {
      enum hxc_caxecraft_app_AppScreen hxc_l_tmp_call_result_n615 = hxc_caxecraft_app_AppScreen_togglePause(hxc_l_screen);
      hxc_l_screen = hxc_l_tmp_call_result_n615;
      hxc_l_accumulator = 0.0;
      hxc_l_resetMotionThisFrame = true;
      bool hxc_l_tmp_call_result_n617 = hxc_caxecraft_app_AppScreen_pausesSimulation(hxc_l_screen);
      if (hxc_l_tmp_call_result_n617)
      {
        hxc_l_jumpQueued = false;
      }
      bool hxc_l_tmp_call_result_n619 = hxc_caxecraft_app_AppScreen_capturesPointer(hxc_l_screen);
      if (hxc_l_tmp_call_result_n619)
      {
        DisableCursor();
      }
      else
      {
        EnableCursor();
      }
    }
    bool hxc_l_tmp_call_result_n621 = hxc_caxecraft_app_AppScreen_showsTitle(hxc_l_screen);
    bool hxc_l_tmp_short_circuit_result_n241 = !hxc_l_tmp_call_result_n621;
    if (!hxc_l_tmp_call_result_n621)
    {
      bool hxc_l_tmp_call_result_n623 = hxc_caxecraft_app_AppScreen_showsCampaignSelection(hxc_l_screen);
      hxc_l_tmp_short_circuit_result_n241 = !hxc_l_tmp_call_result_n623;
    }
    bool hxc_l_tmp_short_circuit_load_result_n624 = hxc_l_tmp_short_circuit_result_n241;
    bool hxc_l_tmp_short_circuit_result_n242 = hxc_l_tmp_short_circuit_load_result_n624;
    if (hxc_l_tmp_short_circuit_load_result_n624)
    {
      bool hxc_l_tmp_call_result_n626 = hxc_caxecraft_app_AppScreen_showsEditor(hxc_l_screen);
      hxc_l_tmp_short_circuit_result_n242 = !hxc_l_tmp_call_result_n626;
    }
    bool hxc_l_tmp_short_circuit_load_result_n627 = hxc_l_tmp_short_circuit_result_n242;
    bool hxc_l_tmp_short_circuit_result_n243 = hxc_l_tmp_short_circuit_load_result_n627;
    if (hxc_l_tmp_short_circuit_load_result_n627)
    {
      hxc_l_tmp_short_circuit_result_n243 = hxc_l_focused;
    }
    bool hxc_l_tmp_short_circuit_load_result_n629 = hxc_l_tmp_short_circuit_result_n243;
    bool hxc_l_tmp_short_circuit_result_n244 = hxc_l_tmp_short_circuit_load_result_n629;
    if (hxc_l_tmp_short_circuit_load_result_n629)
    {
      bool hxc_l_tmp_call_result_n631 = hxc_caxecraft_app_AppScreen_pausesSimulation(hxc_l_screen);
      hxc_l_tmp_short_circuit_result_n244 = hxc_l_tmp_call_result_n631;
    }
    bool hxc_l_tmp_short_circuit_load_result_n632 = hxc_l_tmp_short_circuit_result_n244;
    bool hxc_l_tmp_short_circuit_result_n245 = hxc_l_tmp_short_circuit_load_result_n632;
    if (hxc_l_tmp_short_circuit_load_result_n632)
    {
      hxc_l_tmp_short_circuit_result_n245 = hxc_l_capturePressed;
    }
    if (hxc_l_tmp_short_circuit_result_n245)
    {
      enum hxc_caxecraft_app_AppScreen hxc_l_tmp_call_result_n636 = hxc_caxecraft_app_AppScreen_recapture(hxc_l_screen);
      hxc_l_screen = hxc_l_tmp_call_result_n636;
      hxc_l_recapturedThisFrame = true;
      hxc_l_accumulator = 0.0;
      hxc_l_resetMotionThisFrame = true;
      DisableCursor();
    }
    bool hxc_l_tmp_call_result_n638 = hxc_caxecraft_app_AppScreen_showsTitle(hxc_l_screen);
    bool hxc_l_onTitle = hxc_l_tmp_call_result_n638;
    bool hxc_l_tmp_call_result_n640 = hxc_caxecraft_app_AppScreen_showsCampaignSelection(hxc_l_screen);
    bool hxc_l_onCampaignSelect = hxc_l_tmp_call_result_n640;
    bool hxc_l_tmp_call_result_n642 = hxc_caxecraft_app_AppScreen_showsEditor(hxc_l_screen);
    bool hxc_l_onEditor = hxc_l_tmp_call_result_n642;
    bool hxc_l_tmp_call_result_n644 = hxc_caxecraft_app_AppScreen_pausesSimulation(hxc_l_screen);
    bool hxc_l_paused_h0af3ff7e4c3b = hxc_l_tmp_call_result_n644;
    bool hxc_l_tmp_call_result_n646 = hxc_caxecraft_app_AppScreen_capturesPointer(hxc_l_screen);
    bool hxc_l_captured_h8abb2355e37f = hxc_l_tmp_call_result_n646;
    int32_t hxc_l_editorNavigationCommand = 0;
    if (hxc_l_onEditor)
    {
      if (hxc_l_self == NULL)
      {
        abort();
      }
      struct hxc_caxecraft_input_NavigationRepeater *hxc_l_tmp_owned_class_field_address_n648 = &(*hxc_l_self).hxc_editorNavigation;
      if (hxc_l_tmp_owned_class_field_address_n648 == NULL)
      {
        abort();
      }
      struct hxc_caxecraft_input_NavigationSample hxc_l_tmp_call_result_n649 = hxc_caxecraft_app_RaylibNavigationInput_samplePrimaryGamepad();
      int32_t hxc_l_tmp_instance_call_result_n651 = hxc_caxecraft_input_NavigationRepeater_advance(hxc_l_tmp_owned_class_field_address_n648, hxc_l_tmp_call_result_n649, hxc_l_frameSeconds);
      hxc_l_editorNavigationCommand = hxc_l_tmp_instance_call_result_n651;
    }
    else
    {
      if (hxc_l_self == NULL)
      {
        abort();
      }
      struct hxc_caxecraft_input_NavigationRepeater *hxc_l_tmp_owned_class_field_address_n652 = &(*hxc_l_self).hxc_editorNavigation;
      if (hxc_l_tmp_owned_class_field_address_n652 == NULL)
      {
        abort();
      }
      hxc_caxecraft_input_NavigationRepeater_release(hxc_l_tmp_owned_class_field_address_n652);
    }
    if (hxc_l_captured_h8abb2355e37f)
    {
      double hxc_l_yawDelta = hxc_l_lookYaw_h6269b43db395;
      if (hxc_l_yawDelta > 0.25)
      {
        hxc_l_yawDelta = 0.25;
      }
      if (hxc_l_yawDelta < -0.25)
      {
        hxc_l_yawDelta = -0.25;
      }
      double hxc_l_tmp_load_result_n657 = hxc_l_lookX;
      double hxc_l_tmp_load_result_n658 = hxc_l_yawDelta;
      double hxc_l_candidateX = hxc_l_tmp_load_result_n657 + hxc_l_tmp_load_result_n658 * hxc_l_lookZ;
      double hxc_l_tmp_load_result_n660 = hxc_l_lookZ;
      double hxc_l_tmp_load_result_n661 = hxc_l_yawDelta;
      double hxc_l_candidateZ = hxc_l_tmp_load_result_n660 - hxc_l_tmp_load_result_n661 * hxc_l_lookX;
      double hxc_l_tmp_load_result_n663 = hxc_l_candidateX;
      double hxc_l_tmp_load_result_n664 = hxc_l_candidateX;
      double hxc_l_tmp_load_result_n665 = hxc_l_candidateZ;
      double hxc_l_lengthSquared = hxc_l_tmp_load_result_n663 * hxc_l_tmp_load_result_n664 + hxc_l_tmp_load_result_n665 * hxc_l_candidateZ;
      double hxc_l_normalize = 1.5 - 0.5 * hxc_l_lengthSquared;
      double hxc_l_tmp_load_result_n668 = hxc_l_candidateX;
      hxc_l_lookX = hxc_l_tmp_load_result_n668 * hxc_l_normalize;
      double hxc_l_tmp_load_result_n670 = hxc_l_candidateZ;
      hxc_l_lookZ = hxc_l_tmp_load_result_n670 * hxc_l_normalize;
      double hxc_l_tmp_compound_load_result_n672 = hxc_l_lookY;
      hxc_l_lookY = hxc_l_tmp_compound_load_result_n672 + hxc_l_lookPitch_hf286d80d7193;
      if (hxc_l_lookY > 0.90)
      {
        hxc_l_lookY = 0.90;
      }
      if (hxc_l_lookY < -0.90)
      {
        hxc_l_lookY = -0.90;
      }
      if (hxc_l_jumpPressed_h0ec2ee022c5c)
      {
        hxc_l_jumpQueued = true;
      }
    }
    bool hxc_l_tmp_load_result_n677 = hxc_l_captured_h8abb2355e37f;
    bool hxc_l_tmp_short_circuit_result_n257 = hxc_l_tmp_load_result_n677;
    if (hxc_l_tmp_load_result_n677)
    {
      hxc_l_tmp_short_circuit_result_n257 = !hxc_l_recapturedThisFrame;
    }
    bool hxc_l_tmp_short_circuit_load_result_n679 = hxc_l_tmp_short_circuit_result_n257;
    bool hxc_l_tmp_short_circuit_result_n258 = hxc_l_tmp_short_circuit_load_result_n679;
    if (hxc_l_tmp_short_circuit_load_result_n679)
    {
      hxc_l_tmp_short_circuit_result_n258 = hxc_l_primaryPressed_h86125a319e35;
    }
    bool hxc_l_tmp_short_circuit_load_result_n681 = hxc_l_tmp_short_circuit_result_n258;
    bool hxc_l_tmp_short_circuit_result_n259 = hxc_l_tmp_short_circuit_load_result_n681;
    if (hxc_l_tmp_short_circuit_load_result_n681)
    {
      hxc_l_tmp_short_circuit_result_n259 = hxc_l_selectedMode == 1;
    }
    bool hxc_l_tmp_short_circuit_load_result_n683 = hxc_l_tmp_short_circuit_result_n259;
    bool hxc_l_tmp_short_circuit_result_n260 = hxc_l_tmp_short_circuit_load_result_n683;
    if (hxc_l_tmp_short_circuit_load_result_n683)
    {
      bool hxc_l_tmp_call_result_n686 = hxc_caxecraft_gameplay_Inventory_selectedIs(hxc_l_inventory, hxc_caxecraft_gameplay_ItemKind_CopperSword);
      hxc_l_tmp_short_circuit_result_n260 = hxc_l_tmp_call_result_n686;
    }
    if (hxc_l_tmp_short_circuit_result_n260)
    {
      hxc_l_swordQueued = true;
    }
    if (!hxc_l_paused_h0af3ff7e4c3b)
    {
      double hxc_l_tmp_compound_load_result_n689 = hxc_l_accumulator;
      hxc_l_accumulator = hxc_l_tmp_compound_load_result_n689 + hxc_l_frameSeconds;
    }
    while (1)
    {
      bool hxc_l_tmp_load_result_n691 = hxc_l_paused_h0af3ff7e4c3b;
      bool hxc_l_tmp_short_circuit_result_n261 = !hxc_l_tmp_load_result_n691;
      if (!hxc_l_tmp_load_result_n691)
      {
        hxc_l_tmp_short_circuit_result_n261 = hxc_l_accumulator >= 0.05;
      }
      if (!hxc_l_tmp_short_circuit_result_n261)
      {
        break;
      }
      double hxc_l_tmp_load_result_n694 = hxc_l_moveForward;
      double hxc_l_tmp_load_result_n695 = hxc_l_lookX;
      double hxc_l_tmp_load_result_n696 = hxc_l_moveRight;
      double hxc_l_moveX = hxc_l_tmp_load_result_n694 * hxc_l_tmp_load_result_n695 - hxc_l_tmp_load_result_n696 * hxc_l_lookZ;
      double hxc_l_tmp_load_result_n698 = hxc_l_moveForward;
      double hxc_l_tmp_load_result_n699 = hxc_l_lookZ;
      double hxc_l_tmp_load_result_n700 = hxc_l_moveRight;
      double hxc_l_moveZ = hxc_l_tmp_load_result_n698 * hxc_l_tmp_load_result_n699 + hxc_l_tmp_load_result_n700 * hxc_l_lookX;
      double hxc_l_tmp_load_result_n702 = hxc_l_moveForward;
      bool hxc_l_tmp_short_circuit_result_n264 = hxc_l_tmp_load_result_n702 != 0.0;
      if (hxc_l_tmp_load_result_n702 != 0.0)
      {
        hxc_l_tmp_short_circuit_result_n264 = hxc_l_moveRight != 0.0;
      }
      if (hxc_l_tmp_short_circuit_result_n264)
      {
        hxc_l_moveX = hxc_l_moveX * 0.7071067811865476;
        hxc_l_moveZ = hxc_l_moveZ * 0.7071067811865476;
      }
      int32_t hxc_l_tmp_load_result_n707 = hxc_l_selectedMode;
      int32_t hxc_l_tmp_conditional_result_n266 = 0;
      if (hxc_l_tmp_load_result_n707 == 1)
      {
        hxc_l_tmp_conditional_result_n266 = 1;
      }
      else
      {
        hxc_l_tmp_conditional_result_n266 = 0;
      }
      int32_t hxc_l_damagePolicy = hxc_l_tmp_conditional_result_n266;
      bool hxc_l_tmp_load_result_n709 = hxc_l_jumpQueued;
      bool hxc_l_tmp_short_circuit_result_n268 = hxc_l_tmp_load_result_n709;
      if (!hxc_l_tmp_load_result_n709)
      {
        int32_t hxc_l_tmp_record_field_load_result_n710 = hxc_l_character.hxc_aquatic.hxc_medium;
        bool hxc_l_tmp_short_circuit_result_n269 = hxc_l_tmp_record_field_load_result_n710 != 0;
        if (hxc_l_tmp_record_field_load_result_n710 != 0)
        {
          hxc_l_tmp_short_circuit_result_n269 = hxc_l_riseHeld;
        }
        hxc_l_tmp_short_circuit_result_n268 = hxc_l_tmp_short_circuit_result_n269;
      }
      bool hxc_l_rising = hxc_l_tmp_short_circuit_result_n268;
      struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n714 = hxc_l_session;
      hxc_l_gc_roots[186] = (const void *)hxc_l_tmp_load_result_n714;
      if (hxc_l_tmp_load_result_n714 == NULL)
      {
        abort();
      }
      double hxc_l_tmp_load_result_n715 = hxc_l_moveX;
      double hxc_l_tmp_load_result_n716 = hxc_l_moveZ;
      bool hxc_l_tmp_load_result_n717 = hxc_l_rising;
      struct hxc_caxecraft_domain_CharacterIntent hxc_l_tmp_call_result_n719 = hxc_caxecraft_domain_Aquatics_input(hxc_l_tmp_load_result_n715, hxc_l_tmp_load_result_n716, hxc_l_tmp_load_result_n717, hxc_l_descendHeld_h2072257a4c60);
      struct hxc_caxecraft_domain_GameTickResult hxc_l_tmp_instance_call_result_n722 = hxc_caxecraft_domain_GameSession_tick(hxc_l_tmp_load_result_n714, (struct hxc_caxecraft_domain_GameTickInput){ .hxc_damagePolicy = hxc_l_damagePolicy, .hxc_intent = hxc_l_tmp_call_result_n719, .hxc_waterUpdateBudget = 64 });
      hxc_l_gameTick = hxc_l_tmp_instance_call_result_n722;
      hxc_l_character = hxc_l_gameTick.hxc_character;
      hxc_l_flow = hxc_l_gameTick.hxc_flow;
      if (hxc_optional_d3b7e5a2_retain(&hxc_l_flow) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_l_flow.hxc_has_value)
      {
        int32_t hxc_l_g_hdf7f77ceb52b = 0;
        struct hxc_optional_caxecraft_scenario_FlowTickResult hxc_l_tmp_load_result_n726 = hxc_l_flow;
        if (!hxc_l_tmp_load_result_n726.hxc_has_value)
        {
          abort();
        }
        hxc_l_g1_h318bce074317 = hxc_l_tmp_load_result_n726.hxc_value.hxc_presentation;
        if (hxc_array_ref_retain(hxc_l_g1_h318bce074317) != HXC_STATUS_OK)
        {
          abort();
        }
        while (1)
        {
          int32_t hxc_l_tmp_load_result_n729 = hxc_l_g_hdf7f77ceb52b;
          int32_t hxc_l_tmp_array_length_result_n731;
          if (hxc_array_ref_length(hxc_l_g1_h318bce074317, &hxc_l_tmp_array_length_result_n731) != HXC_STATUS_OK)
          {
            abort();
          }
          if (!(hxc_l_tmp_load_result_n729 < hxc_l_tmp_array_length_result_n731))
          {
            break;
          }
          struct hxc_array_ref *hxc_l_tmp_load_result_n732 = hxc_l_g1_h318bce074317;
          struct hxc_caxecraft_scenario_FlowPresentationEvent hxc_l_tmp_array_get_result_n734;
          if (hxc_array_ref_get_copy(hxc_l_tmp_load_result_n732, (size_t)hxc_l_g_hdf7f77ceb52b, &hxc_l_tmp_array_get_result_n734) != HXC_STATUS_OK)
          {
            abort();
          }
          hxc_l_tmp_array_element_owner_n275 = hxc_l_tmp_array_get_result_n734;
          hxc_l_event_h8f301d6d2834 = hxc_l_tmp_array_element_owner_n275;
          if (hxc_enum_147eda7c_retain(&hxc_l_event_h8f301d6d2834) != HXC_STATUS_OK)
          {
            abort();
          }
          hxc_l_g_hdf7f77ceb52b = hxc_i32_add_wrapping(hxc_l_g_hdf7f77ceb52b, 1);
          if (hxc_l_event_h8f301d6d2834.hxc_tag == hxc_caxecraft_scenario_FlowPresentationEvent_ObjectiveChanged)
          {
            if (hxc_l_event_h8f301d6d2834.hxc_tag != hxc_caxecraft_scenario_FlowPresentationEvent_ObjectiveChanged)
            {
              abort();
            }
            hxc_string hxc_l_tmp_enum_payload_project_n739 = hxc_l_event_h8f301d6d2834.hxc_payload.hxc_ObjectiveChanged.hxc_id;
            hxc_l_id = hxc_l_tmp_enum_payload_project_n739;
            if (hxc_string_retain(hxc_l_id) != HXC_STATUS_OK)
            {
              abort();
            }
            if (hxc_l_event_h8f301d6d2834.hxc_tag != hxc_caxecraft_scenario_FlowPresentationEvent_ObjectiveChanged)
            {
              abort();
            }
            enum hxc_caxecraft_scenario_ObjectiveState hxc_l_tmp_enum_payload_project_n741 = hxc_l_event_h8f301d6d2834.hxc_payload.hxc_ObjectiveChanged.hxc_value;
            enum hxc_caxecraft_scenario_ObjectiveState hxc_l_state = hxc_l_tmp_enum_payload_project_n741;
            switch (hxc_l_state) {
              case hxc_caxecraft_scenario_ObjectiveState_Active:
                {
                  hxc_string hxc_l_tmp_string_assignment_replacement_owner_n278 = hxc_l_id;
                  if (hxc_string_retain(hxc_l_tmp_string_assignment_replacement_owner_n278) != HXC_STATUS_OK)
                  {
                    abort();
                  }
                  hxc_string hxc_l_tmp_string_assignment_replacement_owned_load_result_n744 = hxc_l_tmp_string_assignment_replacement_owner_n278;
                  if (hxc_string_release(&hxc_l_currentObjectiveId) != HXC_STATUS_OK)
                  {
                    abort();
                  }
                  hxc_l_currentObjectiveId = hxc_l_tmp_string_assignment_replacement_owned_load_result_n744;
                  break;
                }
              case hxc_caxecraft_scenario_ObjectiveState_Hidden:
              case hxc_caxecraft_scenario_ObjectiveState_Complete:
              case hxc_caxecraft_scenario_ObjectiveState_Failed:
                {
                  hxc_l_current = hxc_l_currentObjectiveId;
                  if (hxc_string_retain(hxc_l_current) != HXC_STATUS_OK)
                  {
                    abort();
                  }
                  hxc_string hxc_l_tmp_load_result_n746 = hxc_l_current;
                  bool hxc_l_tmp_short_circuit_result_n280 = hxc_l_tmp_load_result_n746.data != NULL;
                  if (hxc_l_tmp_load_result_n746.data != NULL)
                  {
                    hxc_string hxc_l_tmp_load_result_n747 = hxc_l_current;
                    hxc_l_tmp_short_circuit_result_n280 = hxc_l_tmp_load_result_n747.data == NULL || hxc_l_id.data == NULL ? hxc_l_tmp_load_result_n747.data == hxc_l_id.data : hxc_l_tmp_load_result_n747.byte_length == hxc_l_id.byte_length && (hxc_l_tmp_load_result_n747.byte_length == 0 || memcmp(hxc_l_tmp_load_result_n747.data, hxc_l_id.data, hxc_l_tmp_load_result_n747.byte_length) == 0);
                  }
                  if (hxc_l_tmp_short_circuit_result_n280)
                  {
                    hxc_string hxc_l_tmp_string_assignment_replacement_owner_n281 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
                    if (hxc_string_retain(hxc_l_tmp_string_assignment_replacement_owner_n281) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    hxc_string hxc_l_tmp_string_assignment_replacement_owned_load_result_n751 = hxc_l_tmp_string_assignment_replacement_owner_n281;
                    if (hxc_string_release(&hxc_l_currentObjectiveId) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    hxc_l_currentObjectiveId = hxc_l_tmp_string_assignment_replacement_owned_load_result_n751;
                  }
                  if (hxc_string_release(&hxc_l_current) != HXC_STATUS_OK)
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
            if (hxc_string_release(&hxc_l_id) != HXC_STATUS_OK)
            {
              abort();
            }
          }
          hxc_enum_147eda7c_destroy(&hxc_l_event_h8f301d6d2834);
          hxc_array_ce94fc36_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n275);
        }
        if (hxc_array_ref_release(hxc_l_g1_h318bce074317) != HXC_STATUS_OK)
        {
          abort();
        }
      }
      if (hxc_l_gameTick.hxc_committed)
      {
        struct hxc_caxecraft_app_MotionHistory hxc_l_tmp_load_result_n753 = hxc_l_motionHistory;
        struct hxc_caxecraft_app_MotionHistory hxc_l_tmp_call_result_n755 = hxc_caxecraft_app_MotionInterpolation_advance(hxc_l_tmp_load_result_n753, hxc_l_character.hxc_body);
        hxc_l_motionHistory = hxc_l_tmp_call_result_n755;
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
          int32_t hxc_l_tmp_load_result_n759 = hxc_l_pickupIndex;
          struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n760 = hxc_l_levelView;
          hxc_l_gc_roots[187] = (const void *)hxc_l_tmp_load_result_n760;
          if (hxc_l_tmp_load_result_n760 == NULL)
          {
            abort();
          }
          struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n761 = (*hxc_l_tmp_load_result_n760).hxc_items;
          int32_t hxc_l_tmp_array_length_result_n762;
          if (hxc_array_ref_length(hxc_l_tmp_class_field_load_result_n761, &hxc_l_tmp_array_length_result_n762) != HXC_STATUS_OK)
          {
            abort();
          }
          if (!(hxc_l_tmp_load_result_n759 < hxc_l_tmp_array_length_result_n762))
          {
            break;
          }
          struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n763 = hxc_l_levelView;
          hxc_l_gc_roots[188] = (const void *)hxc_l_tmp_load_result_n763;
          if (hxc_l_tmp_load_result_n763 == NULL)
          {
            abort();
          }
          struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n764 = (*hxc_l_tmp_load_result_n763).hxc_items;
          struct hxc_caxecraft_app_LoadedWorldItem hxc_l_tmp_array_get_result_n766;
          if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n764, (size_t)hxc_l_pickupIndex, &hxc_l_tmp_array_get_result_n766) != HXC_STATUS_OK)
          {
            abort();
          }
          struct hxc_caxecraft_app_LoadedWorldItem hxc_l_loadedItem = hxc_l_tmp_array_get_result_n766;
          struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n767 = hxc_l_session;
          hxc_l_gc_roots[189] = (const void *)hxc_l_tmp_load_result_n767;
          if (hxc_l_tmp_load_result_n767 == NULL)
          {
            abort();
          }
          bool hxc_l_tmp_instance_call_result_n769 = hxc_caxecraft_domain_GameSession_authoredItemIsActive(hxc_l_tmp_load_result_n767, hxc_l_pickupIndex);
          bool hxc_l_tmp_short_circuit_result_n284 = hxc_l_tmp_instance_call_result_n769;
          if (hxc_l_tmp_instance_call_result_n769)
          {
            double hxc_l_tmp_record_field_load_result_n770 = hxc_l_character.hxc_body.hxc_x;
            double hxc_l_tmp_record_field_load_result_n771 = hxc_l_character.hxc_body.hxc_y;
            double hxc_l_tmp_record_field_load_result_n772 = hxc_l_character.hxc_body.hxc_z;
            int32_t hxc_l_tmp_record_field_load_result_n773 = hxc_l_loadedItem.hxc_xMilli;
            int32_t hxc_l_tmp_record_field_load_result_n774 = hxc_l_loadedItem.hxc_yMilli;
            bool hxc_l_tmp_call_result_n776 = hxc_caxecraft_gameplay_WorldItemPickup_isInRange(hxc_l_tmp_record_field_load_result_n770, hxc_l_tmp_record_field_load_result_n771, hxc_l_tmp_record_field_load_result_n772, hxc_l_tmp_record_field_load_result_n773, hxc_l_tmp_record_field_load_result_n774, hxc_l_loadedItem.hxc_zMilli);
            hxc_l_tmp_short_circuit_result_n284 = hxc_l_tmp_call_result_n776;
          }
          if (hxc_l_tmp_short_circuit_result_n284)
          {
            int32_t hxc_l_itemCode = hxc_l_loadedItem.hxc_storageCode;
            struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_tmp_load_result_n779 = hxc_l_contentRegistry;
            hxc_l_gc_roots[190] = (const void *)hxc_l_tmp_load_result_n779;
            if (hxc_l_tmp_load_result_n779 == NULL)
            {
              abort();
            }
            enum hxc_caxecraft_content_RuntimeItemUseProfile hxc_l_tmp_instance_call_result_n781 = hxc_caxecraft_content_RuntimeContentRegistry_itemUseProfile(hxc_l_tmp_load_result_n779, hxc_l_itemCode);
            bool hxc_l_tmp_short_circuit_result_n286 = hxc_l_tmp_instance_call_result_n781 == hxc_caxecraft_content_RuntimeItemUseProfile_EquipAquatic;
            if (hxc_l_tmp_instance_call_result_n781 == hxc_caxecraft_content_RuntimeItemUseProfile_EquipAquatic)
            {
              struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_tmp_load_result_n783 = hxc_l_contentRegistry;
              hxc_l_gc_roots[191] = (const void *)hxc_l_tmp_load_result_n783;
              if (hxc_l_tmp_load_result_n783 == NULL)
              {
                abort();
              }
              bool hxc_l_tmp_instance_call_result_n785 = hxc_caxecraft_content_RuntimeContentRegistry_itemProvidesAquaticProfile(hxc_l_tmp_load_result_n783, hxc_l_itemCode);
              hxc_l_tmp_short_circuit_result_n286 = hxc_l_tmp_instance_call_result_n785;
            }
            if (hxc_l_tmp_short_circuit_result_n286)
            {
              struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_tmp_load_result_n787 = hxc_l_contentRegistry;
              hxc_l_gc_roots[192] = (const void *)hxc_l_tmp_load_result_n787;
              if (hxc_l_tmp_load_result_n787 == NULL)
              {
                abort();
              }
              struct hxc_caxecraft_domain_AquaticProfile hxc_l_tmp_instance_call_result_n789 = hxc_caxecraft_content_RuntimeContentRegistry_itemAquaticProfile(hxc_l_tmp_load_result_n787, hxc_l_itemCode);
              struct hxc_caxecraft_domain_AquaticProfile hxc_l_replacement = hxc_l_tmp_instance_call_result_n789;
              struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n790 = hxc_l_session;
              hxc_l_gc_roots[193] = (const void *)hxc_l_tmp_load_result_n790;
              if (hxc_l_tmp_load_result_n790 == NULL)
              {
                abort();
              }
              int32_t hxc_l_tmp_load_result_n791 = hxc_l_pickupIndex;
              struct hxc_caxecraft_domain_AuthoredAquaticEquipmentResult hxc_l_tmp_instance_call_result_n793 = hxc_caxecraft_domain_GameSession_collectAuthoredAquaticEquipment(hxc_l_tmp_load_result_n790, hxc_l_tmp_load_result_n791, hxc_l_replacement);
              struct hxc_caxecraft_domain_AuthoredAquaticEquipmentResult hxc_l_equipment = hxc_l_tmp_instance_call_result_n793;
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
          struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n801 = hxc_l_session;
          hxc_l_gc_roots[194] = (const void *)hxc_l_tmp_load_result_n801;
          if (hxc_l_tmp_load_result_n801 == NULL)
          {
            abort();
          }
          int32_t hxc_l_tmp_record_field_load_result_n802 = hxc_l_gameTick.hxc_tickIndex;
          struct hxc_caxecraft_domain_ActorControllerTickResult hxc_l_tmp_instance_call_result_n804 = hxc_caxecraft_domain_GameSession_stepAuthoredActorControllers(hxc_l_tmp_load_result_n801, hxc_l_tmp_record_field_load_result_n802, hxc_l_damagePolicy);
          struct hxc_caxecraft_domain_ActorControllerTickResult hxc_l_actorTick = hxc_l_tmp_instance_call_result_n804;
          switch (hxc_l_actorTick.hxc_status.hxc_tag) {
            case hxc_caxecraft_domain_ActorControllerTickStatus_ControllersAdvanced:
              {
                hxc_l_guideInteractionAvailable = false;
                int32_t hxc_l_g_h09ddfce788ab = 0;
                struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n806 = hxc_l_session;
                hxc_l_gc_roots[198] = (const void *)hxc_l_tmp_load_result_n806;
                if (hxc_l_tmp_load_result_n806 == NULL)
                {
                  abort();
                }
                struct hxc_array_ref *hxc_l_tmp_instance_call_result_n807 = hxc_caxecraft_domain_GameSession_actorControllerEventSnapshots(hxc_l_tmp_load_result_n806);
                hxc_l_g1_h86761ddd48e2 = hxc_l_tmp_instance_call_result_n807;
                while (1)
                {
                  int32_t hxc_l_tmp_load_result_n808 = hxc_l_g_h09ddfce788ab;
                  int32_t hxc_l_tmp_array_length_result_n810;
                  if (hxc_array_ref_length(hxc_l_g1_h86761ddd48e2, &hxc_l_tmp_array_length_result_n810) != HXC_STATUS_OK)
                  {
                    abort();
                  }
                  if (!(hxc_l_tmp_load_result_n808 < hxc_l_tmp_array_length_result_n810))
                  {
                    break;
                  }
                  struct hxc_array_ref *hxc_l_tmp_load_result_n811 = hxc_l_g1_h86761ddd48e2;
                  struct hxc_caxecraft_domain_ActorControllerEvent hxc_l_tmp_array_get_result_n813;
                  if (hxc_array_ref_get_copy(hxc_l_tmp_load_result_n811, (size_t)hxc_l_g_h09ddfce788ab, &hxc_l_tmp_array_get_result_n813) != HXC_STATUS_OK)
                  {
                    abort();
                  }
                  hxc_l_tmp_array_element_owner_n293 = hxc_l_tmp_array_get_result_n813;
                  hxc_l_event_h6e7c030f6037 = hxc_l_tmp_array_element_owner_n293;
                  if (hxc_enum_8b2a26b8_retain(&hxc_l_event_h6e7c030f6037) != HXC_STATUS_OK)
                  {
                    abort();
                  }
                  hxc_l_g_h09ddfce788ab = hxc_i32_add_wrapping(hxc_l_g_h09ddfce788ab, 1);
                  switch (hxc_l_event_h6e7c030f6037.hxc_tag) {
                    case hxc_caxecraft_domain_ActorControllerEvent_NoControllerEvent:
                      {
                        break;
                      }
                    case hxc_caxecraft_domain_ActorControllerEvent_InteractionAvailable:
                      {
                        if (hxc_l_event_h6e7c030f6037.hxc_tag != hxc_caxecraft_domain_ActorControllerEvent_InteractionAvailable)
                        {
                          abort();
                        }
                        int32_t hxc_l_tmp_enum_payload_project_n818 = hxc_l_event_h6e7c030f6037.hxc_payload.hxc_InteractionAvailable.hxc_characterId;
                        int32_t hxc_l_source_hbd6ac0610c33 = hxc_l_tmp_enum_payload_project_n818;
                        int32_t hxc_l_tmp_load_result_n819 = hxc_l_source_hbd6ac0610c33;
                        if (hxc_l_tmp_load_result_n819 == hxc_l_dialogueActorId)
                        {
                          hxc_l_guideInteractionAvailable = true;
                        }
                        break;
                      }
                    case hxc_caxecraft_domain_ActorControllerEvent_LocalPlayerAttack:
                      {
                        if (hxc_l_event_h6e7c030f6037.hxc_tag != hxc_caxecraft_domain_ActorControllerEvent_LocalPlayerAttack)
                        {
                          abort();
                        }
                        int32_t hxc_l_tmp_enum_payload_project_n822 = hxc_l_event_h6e7c030f6037.hxc_payload.hxc_LocalPlayerAttack.hxc_characterId;
                        int32_t hxc_l_source_hd06ffa06cddf = hxc_l_tmp_enum_payload_project_n822;
                        int32_t hxc_l_tmp_load_result_n823 = hxc_l_source_hd06ffa06cddf;
                        if (hxc_l_tmp_load_result_n823 == hxc_l_enemyActorId)
                        {
                          hxc_l_enemyAttackFrames = 120;
                        }
                        break;
                      }
                    case hxc_caxecraft_domain_ActorControllerEvent_DropRequested:
                      {
                        if (hxc_l_event_h6e7c030f6037.hxc_tag != hxc_caxecraft_domain_ActorControllerEvent_DropRequested)
                        {
                          abort();
                        }
                        int32_t hxc_l_tmp_enum_payload_project_n826 = hxc_l_event_h6e7c030f6037.hxc_payload.hxc_DropRequested.hxc_characterId;
                        int32_t hxc_l_source_h2f060cc986d3 = hxc_l_tmp_enum_payload_project_n826;
                        if (hxc_l_event_h6e7c030f6037.hxc_tag != hxc_caxecraft_domain_ActorControllerEvent_DropRequested)
                        {
                          abort();
                        }
                        hxc_string hxc_l_tmp_enum_payload_project_n828 = hxc_l_event_h6e7c030f6037.hxc_payload.hxc_DropRequested.hxc_drop;
                        hxc_l_drop = hxc_l_tmp_enum_payload_project_n828;
                        if (hxc_string_retain(hxc_l_drop) != HXC_STATUS_OK)
                        {
                          abort();
                        }
                        struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_tmp_load_result_n829 = hxc_l_contentRegistry;
                        hxc_l_gc_roots[199] = (const void *)hxc_l_tmp_load_result_n829;
                        if (hxc_l_tmp_load_result_n829 == NULL)
                        {
                          abort();
                        }
                        int32_t hxc_l_tmp_instance_call_result_n831 = hxc_caxecraft_content_RuntimeContentRegistry_dropQuantityById(hxc_l_tmp_load_result_n829, hxc_l_drop);
                        int32_t hxc_l_quantity = hxc_l_tmp_instance_call_result_n831;
                        struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n832 = hxc_l_session;
                        hxc_l_gc_roots[200] = (const void *)hxc_l_tmp_load_result_n832;
                        if (hxc_l_tmp_load_result_n832 == NULL)
                        {
                          abort();
                        }
                        struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n834 = hxc_caxecraft_domain_GameSession_readCharacter(hxc_l_tmp_load_result_n832, hxc_l_source_h2f060cc986d3);
                        struct hxc_caxecraft_domain_Character hxc_l_defeatedActor = hxc_l_tmp_instance_call_result_n834;
                        int32_t hxc_l_tmp_load_result_n835 = hxc_l_quantity;
                        bool hxc_l_tmp_short_circuit_result_n300 = hxc_l_tmp_load_result_n835 <= 0;
                        if (!(hxc_l_tmp_load_result_n835 <= 0))
                        {
                          hxc_l_tmp_short_circuit_result_n300 = hxc_l_defeatedActor.hxc_id <= 0;
                        }
                        if (hxc_l_tmp_short_circuit_result_n300)
                        {
                          hxc_l_quit = true;
                        }
                        else
                        {
                          if (!hxc_l_berryDrop.hxc_active)
                          {
                            struct hxc_caxecraft_domain_Character hxc_l_tmp_load_result_n839 = hxc_l_defeatedActor;
                            struct hxc_caxecraft_gameplay_BerryDropState hxc_l_tmp_call_result_n841 = hxc_caxecraft_gameplay_BerryDrop_fromDefeatedCharacter(hxc_l_tmp_load_result_n839, hxc_l_quantity);
                            hxc_l_berryDrop = hxc_l_tmp_call_result_n841;
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
                  hxc_enum_8b2a26b8_destroy(&hxc_l_event_h6e7c030f6037);
                  hxc_array_43e9f8ca_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n293);
                }
                if (hxc_array_ref_release(hxc_l_g1_h86761ddd48e2) != HXC_STATUS_OK)
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
        struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n842 = hxc_l_session;
        hxc_l_gc_roots[195] = (const void *)hxc_l_tmp_load_result_n842;
        if (hxc_l_tmp_load_result_n842 == NULL)
        {
          abort();
        }
        struct hxc_array_ref *hxc_l_tmp_instance_call_result_n843 = hxc_caxecraft_domain_GameSession_actorControllerStateSnapshots(hxc_l_tmp_load_result_n842);
        hxc_l_actorPhases = hxc_l_tmp_instance_call_result_n843;
        struct hxc_array_ref *hxc_l_tmp_load_result_n844 = hxc_l_actorPhases;
        struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation hxc_l_tmp_call_result_n847 = hxc_caxecraft_app_CaxecraftApp_observeActorPhase(hxc_l_tmp_load_result_n844, hxc_l_dialogueActorId, hxc_caxecraft_domain_ActorControllerPhase_Stationary);
        hxc_l_dialoguePhase = hxc_l_tmp_call_result_n847;
        struct hxc_array_ref *hxc_l_tmp_load_result_n848 = hxc_l_actorPhases;
        struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation hxc_l_tmp_call_result_n851 = hxc_caxecraft_app_CaxecraftApp_observeActorPhase(hxc_l_tmp_load_result_n848, hxc_l_enemyActorId, hxc_caxecraft_domain_ActorControllerPhase_Defeated);
        hxc_l_enemyPhase_h70382f3bdc66 = hxc_l_tmp_call_result_n851;
        struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n852 = hxc_l_session;
        hxc_l_gc_roots[196] = (const void *)hxc_l_tmp_load_result_n852;
        if (hxc_l_tmp_load_result_n852 == NULL)
        {
          abort();
        }
        struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n854 = hxc_caxecraft_domain_GameSession_readCharacter(hxc_l_tmp_load_result_n852, hxc_l_dialogueActorId);
        hxc_l_dialogueActor = hxc_l_tmp_instance_call_result_n854;
        struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n855 = hxc_l_session;
        hxc_l_gc_roots[197] = (const void *)hxc_l_tmp_load_result_n855;
        if (hxc_l_tmp_load_result_n855 == NULL)
        {
          abort();
        }
        struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n857 = hxc_caxecraft_domain_GameSession_readCharacter(hxc_l_tmp_load_result_n855, hxc_l_enemyActorId);
        hxc_l_enemyActor = hxc_l_tmp_instance_call_result_n857;
        bool hxc_l_tmp_record_field_load_result_n858 = hxc_l_dialoguePhase.hxc_valid;
        bool hxc_l_tmp_short_circuit_result_n302 = !hxc_l_tmp_record_field_load_result_n858;
        if (!!hxc_l_tmp_record_field_load_result_n858)
        {
          hxc_l_tmp_short_circuit_result_n302 = !hxc_l_enemyPhase_h70382f3bdc66.hxc_valid;
        }
        bool hxc_l_tmp_short_circuit_load_result_n860 = hxc_l_tmp_short_circuit_result_n302;
        bool hxc_l_tmp_short_circuit_result_n303 = hxc_l_tmp_short_circuit_load_result_n860;
        if (!hxc_l_tmp_short_circuit_load_result_n860)
        {
          hxc_l_tmp_short_circuit_result_n303 = hxc_l_dialogueActor.hxc_id <= 0;
        }
        bool hxc_l_tmp_short_circuit_load_result_n862 = hxc_l_tmp_short_circuit_result_n303;
        bool hxc_l_tmp_short_circuit_result_n304 = hxc_l_tmp_short_circuit_load_result_n862;
        if (!hxc_l_tmp_short_circuit_load_result_n862)
        {
          hxc_l_tmp_short_circuit_result_n304 = hxc_l_enemyActor.hxc_id <= 0;
        }
        if (hxc_l_tmp_short_circuit_result_n304)
        {
          hxc_l_quit = true;
        }
        struct hxc_caxecraft_gameplay_SwordCombatState hxc_l_tmp_call_result_n866 = hxc_caxecraft_gameplay_SwordCombat_step(hxc_l_swordCombat);
        hxc_l_swordCombat = hxc_l_tmp_call_result_n866;
        if (hxc_l_swordQueued)
        {
          struct hxc_caxecraft_gameplay_SwordCombatState hxc_l_tmp_load_result_n868 = hxc_l_swordCombat;
          struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_load_result_n869 = hxc_l_inventory;
          struct hxc_caxecraft_domain_VitalsState hxc_l_tmp_record_field_load_result_n870 = hxc_l_character.hxc_vitals;
          struct hxc_caxecraft_domain_Character hxc_l_tmp_load_result_n871 = hxc_l_enemyActor;
          double hxc_l_tmp_record_field_load_result_n872 = hxc_l_character.hxc_body.hxc_x;
          double hxc_l_tmp_record_field_load_result_n873 = hxc_l_character.hxc_body.hxc_z;
          double hxc_l_tmp_load_result_n874 = hxc_l_lookX;
          int32_t hxc_l_tmp_call_result_n876 = hxc_caxecraft_gameplay_SwordCombat_decide(hxc_l_tmp_load_result_n868, hxc_l_tmp_load_result_n869, hxc_l_tmp_record_field_load_result_n870, hxc_l_tmp_load_result_n871, hxc_l_tmp_record_field_load_result_n872, hxc_l_tmp_record_field_load_result_n873, hxc_l_tmp_load_result_n874, hxc_l_lookZ);
          int32_t hxc_l_swordDecision = hxc_l_tmp_call_result_n876;
          if (hxc_l_swordDecision == 5)
          {
            struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n878 = hxc_l_session;
            hxc_l_gc_roots[201] = (const void *)hxc_l_tmp_load_result_n878;
            if (hxc_l_tmp_load_result_n878 == NULL)
            {
              abort();
            }
            struct hxc_caxecraft_domain_CharacterDamageResult hxc_l_tmp_instance_call_result_n880 = hxc_caxecraft_domain_GameSession_damageCharacter(hxc_l_tmp_load_result_n878, hxc_l_enemyActorId, 1);
            struct hxc_caxecraft_domain_CharacterDamageResult hxc_l_damage = hxc_l_tmp_instance_call_result_n880;
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
          int32_t hxc_l_tmp_load_result_n884 = hxc_l_swordDecision;
          struct hxc_caxecraft_gameplay_SwordCombatState hxc_l_tmp_call_result_n886 = hxc_caxecraft_gameplay_SwordCombat_after(hxc_l_tmp_load_result_n884, hxc_l_swordCombat);
          hxc_l_swordCombat = hxc_l_tmp_call_result_n886;
          hxc_l_swordQueued = false;
        }
        if (hxc_array_ref_release(hxc_l_actorPhases) != HXC_STATUS_OK)
        {
          abort();
        }
      }
      hxc_l_jumpQueued = false;
      hxc_l_accumulator = hxc_l_accumulator - 0.05;
      hxc_optional_d3b7e5a2_destroy(&hxc_l_flow);
      hxc_record_b14d5371_destroy(&hxc_l_gameTick);
    }
    double hxc_l_selectionEyeX = hxc_l_character.hxc_body.hxc_x;
    double hxc_l_selectionEyeY = hxc_l_character.hxc_body.hxc_y + 1.62;
    double hxc_l_selectionEyeZ = hxc_l_character.hxc_body.hxc_z;
    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n891 = hxc_l_session;
    hxc_l_gc_roots[185] = (const void *)hxc_l_tmp_load_result_n891;
    if (hxc_l_tmp_load_result_n891 == NULL)
    {
      abort();
    }
    size_t hxc_l_tmp_length_n893;
    const uint8_t *hxc_l_tmp_instance_call_result_n892 = hxc_caxecraft_domain_GameSession_worldView(hxc_l_tmp_load_result_n891, &hxc_l_tmp_length_n893);
    double hxc_l_tmp_load_result_n894 = hxc_l_selectionEyeX;
    double hxc_l_tmp_load_result_n895 = hxc_l_selectionEyeY;
    double hxc_l_tmp_load_result_n896 = hxc_l_selectionEyeZ;
    double hxc_l_tmp_load_result_n897 = hxc_l_lookX;
    double hxc_l_tmp_load_result_n898 = hxc_l_lookY;
    struct hxc_caxecraft_domain_RaycastHit hxc_l_tmp_call_result_n900 = hxc_caxecraft_domain_VoxelRaycast_trace(hxc_l_tmp_instance_call_result_n892, hxc_l_tmp_length_n893, hxc_l_tmp_load_result_n894, hxc_l_tmp_load_result_n895, hxc_l_tmp_load_result_n896, hxc_l_tmp_load_result_n897, hxc_l_tmp_load_result_n898, hxc_l_lookZ, 7.0);
    struct hxc_caxecraft_domain_RaycastHit hxc_l_hit = hxc_l_tmp_call_result_n900;
    bool hxc_l_tmp_load_result_n901 = hxc_l_captured_h8abb2355e37f;
    bool hxc_l_tmp_short_circuit_result_n311 = hxc_l_tmp_load_result_n901;
    if (hxc_l_tmp_load_result_n901)
    {
      hxc_l_tmp_short_circuit_result_n311 = !hxc_l_recapturedThisFrame;
    }
    bool hxc_l_tmp_short_circuit_load_result_n903 = hxc_l_tmp_short_circuit_result_n311;
    bool hxc_l_tmp_short_circuit_result_n312 = hxc_l_tmp_short_circuit_load_result_n903;
    if (hxc_l_tmp_short_circuit_load_result_n903)
    {
      hxc_l_tmp_short_circuit_result_n312 = hxc_l_primaryPressed_h86125a319e35;
    }
    if (hxc_l_tmp_short_circuit_result_n312)
    {
      if (hxc_l_character.hxc_vitals.hxc_health > 0)
      {
        if (hxc_l_selectedMode == 1)
        {
          bool hxc_l_tmp_call_result_n910 = hxc_caxecraft_gameplay_Inventory_selectedIs(hxc_l_inventory, hxc_caxecraft_gameplay_ItemKind_CopperSword);
          bool hxc_l_tmp_short_circuit_result_n313 = !hxc_l_tmp_call_result_n910;
          if (!hxc_l_tmp_call_result_n910)
          {
            hxc_l_tmp_short_circuit_result_n313 = hxc_l_hit.hxc_hit;
          }
          bool hxc_l_tmp_short_circuit_load_result_n912 = hxc_l_tmp_short_circuit_result_n313;
          bool hxc_l_tmp_short_circuit_result_n314 = hxc_l_tmp_short_circuit_load_result_n912;
          if (hxc_l_tmp_short_circuit_load_result_n912)
          {
            struct hxc_caxecraft_domain_AquaticState hxc_l_tmp_record_field_load_result_n913 = hxc_l_character.hxc_aquatic;
            bool hxc_l_tmp_call_result_n915 = hxc_caxecraft_domain_Aquatics_canMine(hxc_l_tmp_record_field_load_result_n913, hxc_l_character.hxc_aquaticProfile);
            hxc_l_tmp_short_circuit_result_n314 = hxc_l_tmp_call_result_n915;
          }
          if (hxc_l_tmp_short_circuit_result_n314)
          {
            int32_t hxc_l_tmp_record_field_load_result_n917 = hxc_l_hit.hxc_cellX;
            int32_t hxc_l_tmp_record_field_load_result_n918 = hxc_l_hit.hxc_cellY;
            struct hxc_caxecraft_scenario_VoxelPoint hxc_l_tmp_call_result_n920 = hxc_caxecraft_domain_World_coord(hxc_l_tmp_record_field_load_result_n917, hxc_l_tmp_record_field_load_result_n918, hxc_l_hit.hxc_cellZ);
            struct hxc_caxecraft_scenario_VoxelPoint hxc_l_minedCoordinate = hxc_l_tmp_call_result_n920;
            int32_t hxc_l_sandBefore = hxc_l_inventory.hxc_sand;
            (void)hxc_l_sandBefore;
            struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n922 = hxc_l_session;
            hxc_l_gc_roots[202] = (const void *)hxc_l_tmp_load_result_n922;
            if (hxc_l_tmp_load_result_n922 == NULL)
            {
              abort();
            }
            struct hxc_caxecraft_scenario_VoxelPoint hxc_l_tmp_load_result_n923 = hxc_l_minedCoordinate;
            struct hxc_caxecraft_gameplay_MiningResult hxc_l_tmp_instance_call_result_n925 = hxc_caxecraft_domain_GameSession_mineTerrain(hxc_l_tmp_load_result_n922, hxc_l_tmp_load_result_n923, hxc_l_inventory);
            struct hxc_caxecraft_gameplay_MiningResult hxc_l_mining = hxc_l_tmp_instance_call_result_n925;
            hxc_l_inventory = hxc_l_mining.hxc_inventory;
            if (hxc_l_mining.hxc_outcome == 2)
            {
              if (hxc_l_self == NULL)
              {
                abort();
              }
              struct hxc_caxecraft_app_TerrainRenderer *hxc_l_tmp_owned_class_field_address_n928 = &(*hxc_l_self).hxc_terrainRenderer;
              struct hxc_caxecraft_app_TerrainChunkCache *hxc_l_tmp_owned_class_field_address_n929 = &(*hxc_l_tmp_owned_class_field_address_n928).hxc_cache;
              if (hxc_l_tmp_owned_class_field_address_n929 == NULL)
              {
                abort();
              }
              hxc_caxecraft_app_TerrainChunkCache_invalidate(hxc_l_tmp_owned_class_field_address_n929, hxc_l_minedCoordinate);
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
            int32_t hxc_l_tmp_record_field_load_result_n933 = hxc_l_hit.hxc_cellX;
            int32_t hxc_l_tmp_record_field_load_result_n934 = hxc_l_hit.hxc_cellY;
            struct hxc_caxecraft_scenario_VoxelPoint hxc_l_tmp_call_result_n936 = hxc_caxecraft_domain_World_coord(hxc_l_tmp_record_field_load_result_n933, hxc_l_tmp_record_field_load_result_n934, hxc_l_hit.hxc_cellZ);
            struct hxc_caxecraft_scenario_VoxelPoint hxc_l_removedCoordinate = hxc_l_tmp_call_result_n936;
            struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n937 = hxc_l_session;
            hxc_l_gc_roots[203] = (const void *)hxc_l_tmp_load_result_n937;
            if (hxc_l_tmp_load_result_n937 == NULL)
            {
              abort();
            }
            bool hxc_l_tmp_instance_call_result_n939 = hxc_caxecraft_domain_GameSession_removeTerrain(hxc_l_tmp_load_result_n937, hxc_l_removedCoordinate);
            bool hxc_l_removed = hxc_l_tmp_instance_call_result_n939;
            if (hxc_l_removed)
            {
              if (hxc_l_self == NULL)
              {
                abort();
              }
              struct hxc_caxecraft_app_TerrainRenderer *hxc_l_tmp_owned_class_field_address_n941 = &(*hxc_l_self).hxc_terrainRenderer;
              struct hxc_caxecraft_app_TerrainChunkCache *hxc_l_tmp_owned_class_field_address_n942 = &(*hxc_l_tmp_owned_class_field_address_n941).hxc_cache;
              if (hxc_l_tmp_owned_class_field_address_n942 == NULL)
              {
                abort();
              }
              hxc_caxecraft_app_TerrainChunkCache_invalidate(hxc_l_tmp_owned_class_field_address_n942, hxc_l_removedCoordinate);
            }
          }
        }
      }
    }
    bool hxc_l_tmp_load_result_n944 = hxc_l_captured_h8abb2355e37f;
    bool hxc_l_tmp_short_circuit_result_n320 = hxc_l_tmp_load_result_n944;
    if (hxc_l_tmp_load_result_n944)
    {
      hxc_l_tmp_short_circuit_result_n320 = hxc_l_secondaryPressed_he46a7f5dc259;
    }
    if (hxc_l_tmp_short_circuit_result_n320)
    {
      if (hxc_l_character.hxc_vitals.hxc_health > 0)
      {
        struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n948 = hxc_l_session;
        hxc_l_gc_roots[204] = (const void *)hxc_l_tmp_load_result_n948;
        if (hxc_l_tmp_load_result_n948 == NULL)
        {
          abort();
        }
        struct hxc_caxecraft_domain_LocalRecoveryResult hxc_l_tmp_instance_call_result_n950 = hxc_caxecraft_domain_GameSession_useSelectedRecovery(hxc_l_tmp_load_result_n948, hxc_l_inventory);
        struct hxc_caxecraft_domain_LocalRecoveryResult hxc_l_recovery = hxc_l_tmp_instance_call_result_n950;
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
              int32_t hxc_l_tmp_record_field_load_result_n957 = hxc_l_hit.hxc_previousX;
              int32_t hxc_l_tmp_record_field_load_result_n958 = hxc_l_hit.hxc_previousY;
              struct hxc_caxecraft_scenario_VoxelPoint hxc_l_tmp_call_result_n960 = hxc_caxecraft_domain_World_coord(hxc_l_tmp_record_field_load_result_n957, hxc_l_tmp_record_field_load_result_n958, hxc_l_hit.hxc_previousZ);
              struct hxc_caxecraft_scenario_VoxelPoint hxc_l_placement = hxc_l_tmp_call_result_n960;
              enum hxc_caxecraft_domain_BlockKind hxc_l_tmp_call_result_n962 = hxc_caxecraft_gameplay_Inventory_selectedBlock(hxc_l_inventory);
              enum hxc_caxecraft_domain_BlockKind hxc_l_selectedBlock = hxc_l_tmp_call_result_n962;
              struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_load_result_n963 = hxc_l_inventory;
              int32_t hxc_l_tmp_call_result_n965 = hxc_caxecraft_gameplay_Inventory_countAt(hxc_l_tmp_load_result_n963, hxc_l_inventory.hxc_selected);
              bool hxc_l_hasItem = hxc_l_tmp_call_result_n965 > 0;
              bool hxc_l_tmp_load_result_n966 = hxc_l_hasItem;
              bool hxc_l_tmp_short_circuit_result_n325 = !hxc_l_tmp_load_result_n966;
              if (!!hxc_l_tmp_load_result_n966)
              {
                bool hxc_l_tmp_call_result_n968 = hxc_caxecraft_domain_World_isPlaceable(hxc_l_selectedBlock);
                hxc_l_tmp_short_circuit_result_n325 = !hxc_l_tmp_call_result_n968;
              }
              bool hxc_l_tmp_short_circuit_load_result_n969 = hxc_l_tmp_short_circuit_result_n325;
              bool hxc_l_tmp_short_circuit_result_n326 = hxc_l_tmp_short_circuit_load_result_n969;
              if (!hxc_l_tmp_short_circuit_load_result_n969)
              {
                struct hxc_caxecraft_domain_CharacterBody hxc_l_tmp_record_field_load_result_n970 = hxc_l_character.hxc_body;
                bool hxc_l_tmp_call_result_n972 = hxc_caxecraft_domain_CharacterPhysics_canPlaceAt(hxc_l_tmp_record_field_load_result_n970, hxc_l_placement);
                hxc_l_tmp_short_circuit_result_n326 = !hxc_l_tmp_call_result_n972;
              }
              bool hxc_l_tmp_short_circuit_load_result_n973 = hxc_l_tmp_short_circuit_result_n326;
              bool hxc_l_tmp_short_circuit_result_n327 = hxc_l_tmp_short_circuit_load_result_n973;
              if (!hxc_l_tmp_short_circuit_load_result_n973)
              {
                struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n974 = hxc_l_session;
                hxc_l_gc_roots[205] = (const void *)hxc_l_tmp_load_result_n974;
                if (hxc_l_tmp_load_result_n974 == NULL)
                {
                  abort();
                }
                struct hxc_caxecraft_scenario_VoxelPoint hxc_l_tmp_load_result_n975 = hxc_l_placement;
                bool hxc_l_tmp_instance_call_result_n977 = hxc_caxecraft_domain_GameSession_placeTerrain(hxc_l_tmp_load_result_n974, hxc_l_tmp_load_result_n975, hxc_l_selectedBlock);
                hxc_l_tmp_short_circuit_result_n327 = !hxc_l_tmp_instance_call_result_n977;
              }
              if (hxc_l_tmp_short_circuit_result_n327)
              {
                hxc_l_placementBlockedFrames = 60;
              }
              else
              {
                if (hxc_l_self == NULL)
                {
                  abort();
                }
                struct hxc_caxecraft_app_TerrainRenderer *hxc_l_tmp_owned_class_field_address_n979 = &(*hxc_l_self).hxc_terrainRenderer;
                struct hxc_caxecraft_app_TerrainChunkCache *hxc_l_tmp_owned_class_field_address_n980 = &(*hxc_l_tmp_owned_class_field_address_n979).hxc_cache;
                if (hxc_l_tmp_owned_class_field_address_n980 == NULL)
                {
                  abort();
                }
                hxc_caxecraft_app_TerrainChunkCache_invalidate(hxc_l_tmp_owned_class_field_address_n980, hxc_l_placement);
                if (hxc_l_selectedMode == 1)
                {
                  struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n984 = hxc_caxecraft_gameplay_Inventory_consumeSelected(hxc_l_inventory);
                  hxc_l_inventory = hxc_l_tmp_call_result_n984;
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
    if (!hxc_l_paused_h0af3ff7e4c3b)
    {
      struct hxc_caxecraft_gameplay_BerryDropState hxc_l_tmp_load_result_n988 = hxc_l_berryDrop;
      double hxc_l_tmp_record_field_load_result_n989 = hxc_l_character.hxc_body.hxc_x;
      double hxc_l_tmp_record_field_load_result_n990 = hxc_l_character.hxc_body.hxc_y;
      bool hxc_l_tmp_call_result_n992 = hxc_caxecraft_gameplay_BerryDrop_isInRange(hxc_l_tmp_load_result_n988, hxc_l_tmp_record_field_load_result_n989, hxc_l_tmp_record_field_load_result_n990, hxc_l_character.hxc_body.hxc_z);
      if (hxc_l_tmp_call_result_n992)
      {
        struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_load_result_n993 = hxc_l_inventory;
        int32_t hxc_l_tmp_call_result_n996 = hxc_caxecraft_gameplay_Inventory_acceptedAmount(hxc_l_tmp_load_result_n993, hxc_caxecraft_gameplay_ItemKind_Berries, hxc_l_berryDrop.hxc_amount);
        int32_t hxc_l_acceptedDrop = hxc_l_tmp_call_result_n996;
        if (hxc_l_acceptedDrop > 0)
        {
          struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_load_result_n998 = hxc_l_inventory;
          struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n1001 = hxc_caxecraft_gameplay_Inventory_collectItem(hxc_l_tmp_load_result_n998, hxc_caxecraft_gameplay_ItemKind_Berries, hxc_l_acceptedDrop);
          hxc_l_inventory = hxc_l_tmp_call_result_n1001;
          struct hxc_caxecraft_gameplay_BerryDropState hxc_l_tmp_load_result_n1002 = hxc_l_berryDrop;
          struct hxc_caxecraft_gameplay_BerryDropState hxc_l_tmp_call_result_n1004 = hxc_caxecraft_gameplay_BerryDrop_collectAmount(hxc_l_tmp_load_result_n1002, hxc_l_acceptedDrop);
          hxc_l_berryDrop = hxc_l_tmp_call_result_n1004;
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
    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1021 = hxc_l_session;
    hxc_l_gc_roots[206] = (const void *)hxc_l_tmp_load_result_n1021;
    if (hxc_l_tmp_load_result_n1021 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_domain_GameView hxc_l_tmp_instance_call_result_n1022 = hxc_caxecraft_domain_GameSession_view(hxc_l_tmp_load_result_n1021);
    struct hxc_caxecraft_domain_GameView hxc_l_committedView = hxc_l_tmp_instance_call_result_n1022;
    if (!hxc_l_committedView.hxc_valid)
    {
      hxc_l_quit = true;
    }
    else
    {
      hxc_l_character = hxc_l_committedView.hxc_localPlayer;
    }
    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1025 = hxc_l_session;
    hxc_l_gc_roots[207] = (const void *)hxc_l_tmp_load_result_n1025;
    if (hxc_l_tmp_load_result_n1025 == NULL)
    {
      abort();
    }
    bool hxc_l_tmp_instance_call_result_n1027 = hxc_caxecraft_domain_GameSession_actorInteractionAvailable(hxc_l_tmp_load_result_n1025, hxc_l_dialogueActorId);
    hxc_l_guideInteractionAvailable = hxc_l_tmp_instance_call_result_n1027;
    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1028 = hxc_l_session;
    hxc_l_gc_roots[208] = (const void *)hxc_l_tmp_load_result_n1028;
    if (hxc_l_tmp_load_result_n1028 == NULL)
    {
      abort();
    }
    struct hxc_array_ref *hxc_l_tmp_instance_call_result_n1029 = hxc_caxecraft_domain_GameSession_actorControllerStateSnapshots(hxc_l_tmp_load_result_n1028);
    hxc_l_presentationActorPhases = hxc_l_tmp_instance_call_result_n1029;
    struct hxc_array_ref *hxc_l_tmp_load_result_n1030 = hxc_l_presentationActorPhases;
    struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation hxc_l_tmp_call_result_n1033 = hxc_caxecraft_app_CaxecraftApp_observeActorPhase(hxc_l_tmp_load_result_n1030, hxc_l_dialogueActorId, hxc_caxecraft_domain_ActorControllerPhase_Stationary);
    hxc_l_dialoguePhase = hxc_l_tmp_call_result_n1033;
    struct hxc_array_ref *hxc_l_tmp_load_result_n1034 = hxc_l_presentationActorPhases;
    struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation hxc_l_tmp_call_result_n1037 = hxc_caxecraft_app_CaxecraftApp_observeActorPhase(hxc_l_tmp_load_result_n1034, hxc_l_enemyActorId, hxc_caxecraft_domain_ActorControllerPhase_Defeated);
    hxc_l_enemyPhase_h70382f3bdc66 = hxc_l_tmp_call_result_n1037;
    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1038 = hxc_l_session;
    hxc_l_gc_roots[209] = (const void *)hxc_l_tmp_load_result_n1038;
    if (hxc_l_tmp_load_result_n1038 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n1040 = hxc_caxecraft_domain_GameSession_readCharacter(hxc_l_tmp_load_result_n1038, hxc_l_dialogueActorId);
    hxc_l_dialogueActor = hxc_l_tmp_instance_call_result_n1040;
    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1041 = hxc_l_session;
    hxc_l_gc_roots[210] = (const void *)hxc_l_tmp_load_result_n1041;
    if (hxc_l_tmp_load_result_n1041 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n1043 = hxc_caxecraft_domain_GameSession_readCharacter(hxc_l_tmp_load_result_n1041, hxc_l_enemyActorId);
    hxc_l_enemyActor = hxc_l_tmp_instance_call_result_n1043;
    bool hxc_l_tmp_record_field_load_result_n1044 = hxc_l_dialoguePhase.hxc_valid;
    bool hxc_l_tmp_short_circuit_result_n331 = !hxc_l_tmp_record_field_load_result_n1044;
    if (!!hxc_l_tmp_record_field_load_result_n1044)
    {
      hxc_l_tmp_short_circuit_result_n331 = !hxc_l_enemyPhase_h70382f3bdc66.hxc_valid;
    }
    bool hxc_l_tmp_short_circuit_load_result_n1046 = hxc_l_tmp_short_circuit_result_n331;
    bool hxc_l_tmp_short_circuit_result_n332 = hxc_l_tmp_short_circuit_load_result_n1046;
    if (!hxc_l_tmp_short_circuit_load_result_n1046)
    {
      hxc_l_tmp_short_circuit_result_n332 = hxc_l_dialogueActor.hxc_id <= 0;
    }
    bool hxc_l_tmp_short_circuit_load_result_n1048 = hxc_l_tmp_short_circuit_result_n332;
    bool hxc_l_tmp_short_circuit_result_n333 = hxc_l_tmp_short_circuit_load_result_n1048;
    if (!hxc_l_tmp_short_circuit_load_result_n1048)
    {
      hxc_l_tmp_short_circuit_result_n333 = hxc_l_enemyActor.hxc_id <= 0;
    }
    if (hxc_l_tmp_short_circuit_result_n333)
    {
      hxc_l_quit = true;
    }
    int32_t hxc_l_completedTicks = hxc_l_committedView.hxc_completedTicks;
    if (hxc_l_resetMotionThisFrame)
    {
      struct hxc_caxecraft_app_MotionHistory hxc_l_tmp_call_result_n1054 = hxc_caxecraft_app_MotionInterpolation_start(hxc_l_character.hxc_body);
      hxc_l_motionHistory = hxc_l_tmp_call_result_n1054;
    }
    struct hxc_caxecraft_app_MotionHistory hxc_l_tmp_load_result_n1055 = hxc_l_motionHistory;
    struct hxc_caxecraft_app_RenderPosition hxc_l_tmp_call_result_n1057 = hxc_caxecraft_app_MotionInterpolation_sample(hxc_l_tmp_load_result_n1055, hxc_l_accumulator, 0.05);
    struct hxc_caxecraft_app_RenderPosition hxc_l_renderPosition = hxc_l_tmp_call_result_n1057;
    double hxc_l_eyeX = hxc_l_renderPosition.hxc_x;
    double hxc_l_eyeY = hxc_l_renderPosition.hxc_y + 1.62;
    double hxc_l_eyeZ = hxc_l_renderPosition.hxc_z;
    double hxc_l_tmp_load_result_n1061 = hxc_l_eyeX;
    double hxc_l_tmp_load_result_n1062 = hxc_l_eyeY;
    double hxc_l_tmp_load_result_n1063 = hxc_l_eyeZ;
    double hxc_l_tmp_load_result_n1065 = hxc_l_eyeX;
    double hxc_l_tmp_load_result_n1066 = hxc_l_lookX;
    double hxc_l_tmp_load_result_n1067 = hxc_l_eyeY;
    double hxc_l_tmp_load_result_n1068 = hxc_l_lookY;
    double hxc_l_tmp_load_result_n1069 = hxc_l_eyeZ;
    struct Camera3D hxc_l_camera = (struct Camera3D){ .position = (struct Vector3){ .x = (float)hxc_l_tmp_load_result_n1061, .y = (float)hxc_l_tmp_load_result_n1062, .z = (float)hxc_l_tmp_load_result_n1063 }, .target = (struct Vector3){ .x = (float)(hxc_l_tmp_load_result_n1065 + hxc_l_tmp_load_result_n1066), .y = (float)(hxc_l_tmp_load_result_n1067 + hxc_l_tmp_load_result_n1068), .z = (float)(hxc_l_tmp_load_result_n1069 + hxc_l_lookZ) }, .up = (struct Vector3){ .x = (float)0.0, .y = (float)1.0, .z = (float)0.0 }, .fovy = (float)70.0, .projection = 0 };
    BeginDrawing();
    if (hxc_l_onTitle)
    {
      int32_t hxc_l_tmp_native_call_result_n1075 = GetScreenWidth();
      int32_t hxc_l_width_h3f9a6d706c62 = (int32_t)hxc_l_tmp_native_call_result_n1075;
      int32_t hxc_l_tmp_native_call_result_n1076 = GetScreenHeight();
      int32_t hxc_l_height_h074fc9df30c8 = (int32_t)hxc_l_tmp_native_call_result_n1076;
      if (hxc_l_titleTextureReady)
      {
        struct Color hxc_l_this1_hb5f35b2c5c95 = (struct Color){ .r = 255, .g = 255, .b = 255, .a = 255 };
        struct Texture hxc_l_tmp_load_result_n1079 = hxc_l_titleTexture;
        double hxc_l_imageAspect = hxc_f64_divide_zero_safe((double)(int32_t)hxc_l_tmp_load_result_n1079.width, (double)(int32_t)hxc_l_titleTexture.height);
        int32_t hxc_l_tmp_load_result_n1083 = hxc_l_width_h3f9a6d706c62;
        double hxc_l_screenAspect = hxc_f64_divide_zero_safe((double)hxc_l_tmp_load_result_n1083, (double)hxc_l_height_h074fc9df30c8);
        double hxc_l_sourceX = 0.0;
        double hxc_l_sourceY = 0.0;
        double hxc_l_sourceWidth = (double)(int32_t)hxc_l_titleTexture.width;
        double hxc_l_sourceHeight = (double)(int32_t)hxc_l_titleTexture.height;
        double hxc_l_tmp_load_result_n1089 = hxc_l_screenAspect;
        if (hxc_l_tmp_load_result_n1089 > hxc_l_imageAspect)
        {
          struct Texture hxc_l_tmp_load_result_n1091 = hxc_l_titleTexture;
          hxc_l_sourceHeight = hxc_f64_divide_zero_safe((double)(int32_t)hxc_l_tmp_load_result_n1091.width, hxc_l_screenAspect);
          struct Texture hxc_l_tmp_load_result_n1094 = hxc_l_titleTexture;
          hxc_l_sourceY = ((double)(int32_t)hxc_l_tmp_load_result_n1094.height - hxc_l_sourceHeight) * 0.5;
        }
        else
        {
          struct Texture hxc_l_tmp_load_result_n1097 = hxc_l_titleTexture;
          hxc_l_sourceWidth = (double)(int32_t)hxc_l_tmp_load_result_n1097.height * hxc_l_screenAspect;
          struct Texture hxc_l_tmp_load_result_n1100 = hxc_l_titleTexture;
          hxc_l_sourceX = ((double)(int32_t)hxc_l_tmp_load_result_n1100.width - hxc_l_sourceWidth) * 0.5;
        }
        struct Texture hxc_l_tmp_load_result_n1103 = hxc_l_titleTexture;
        double hxc_l_tmp_load_result_n1104 = hxc_l_sourceX;
        double hxc_l_tmp_load_result_n1105 = hxc_l_sourceY;
        double hxc_l_tmp_load_result_n1106 = hxc_l_sourceWidth;
        double hxc_l_tmp_load_result_n1107 = hxc_l_sourceHeight;
        int32_t hxc_l_tmp_load_result_n1109 = hxc_l_width_h3f9a6d706c62;
        int32_t hxc_l_tmp_load_result_n1110 = hxc_l_height_h074fc9df30c8;
        DrawTexturePro(hxc_l_tmp_load_result_n1103, (struct Rectangle){ .x = (float)hxc_l_tmp_load_result_n1104, .y = (float)hxc_l_tmp_load_result_n1105, .width = (float)hxc_l_tmp_load_result_n1106, .height = (float)hxc_l_tmp_load_result_n1107 }, (struct Rectangle){ .x = (float)0.0, .y = (float)0.0, .width = (float)((double)hxc_l_tmp_load_result_n1109 + 0.0), .height = (float)((double)hxc_l_tmp_load_result_n1110 + 0.0) }, (struct Vector2){ .x = (float)0.0, .y = (float)0.0 }, (float)0.0, hxc_l_this1_hb5f35b2c5c95);
      }
      else
      {
        ClearBackground((struct Color){ .r = 126, .g = 190, .b = 201, .a = 255 });
      }
      struct Color hxc_l_this1_hce18e5371085 = (struct Color){ .r = 4, .g = 14, .b = 23, .a = 132 };
      int32_t hxc_l_tmp_load_result_n1116 = hxc_l_width_h3f9a6d706c62;
      int32_t hxc_l_tmp_load_result_n1117 = hxc_l_height_h074fc9df30c8;
      DrawRectangle((int32_t)0, (int32_t)0, (int32_t)hxc_l_tmp_load_result_n1116, (int32_t)hxc_l_tmp_load_result_n1117, hxc_l_this1_hce18e5371085);
      if (hxc_l_wordmarkTextureReady)
      {
        int32_t hxc_l_centerX = hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_l_width_h3f9a6d706c62, (double)2));
        struct Color hxc_l_this1_h7fb0ccf6eb78 = (struct Color){ .r = 255, .g = 255, .b = 255, .a = 255 };
        double hxc_l_scale = hxc_f64_divide_zero_safe((double)560, (double)(int32_t)hxc_l_wordmarkTexture.width);
        double hxc_l_heightScale = hxc_f64_divide_zero_safe((double)132, (double)(int32_t)hxc_l_wordmarkTexture.height);
        double hxc_l_tmp_load_result_n1126 = hxc_l_heightScale;
        if (hxc_l_tmp_load_result_n1126 < hxc_l_scale)
        {
          hxc_l_scale = hxc_l_heightScale;
        }
        struct Texture hxc_l_tmp_load_result_n1129 = hxc_l_wordmarkTexture;
        double hxc_l_width_h25dad442d9fd = (double)(int32_t)hxc_l_tmp_load_result_n1129.width * hxc_l_scale;
        struct Texture hxc_l_tmp_load_result_n1132 = hxc_l_wordmarkTexture;
        double hxc_l_height_h867c1b5ebe9e = (double)(int32_t)hxc_l_tmp_load_result_n1132.height * hxc_l_scale;
        double hxc_l_width1 = (double)(int32_t)hxc_l_wordmarkTexture.width + 0.0;
        double hxc_l_height1 = (double)(int32_t)hxc_l_wordmarkTexture.height + 0.0;
        struct Texture hxc_l_tmp_load_result_n1139 = hxc_l_wordmarkTexture;
        double hxc_l_tmp_load_result_n1140 = hxc_l_width1;
        double hxc_l_tmp_load_result_n1141 = hxc_l_height1;
        int32_t hxc_l_tmp_load_result_n1143 = hxc_l_centerX;
        double hxc_l_tmp_load_result_n1144 = hxc_l_width_h25dad442d9fd;
        double hxc_l_tmp_load_result_n1145 = hxc_l_width_h25dad442d9fd;
        double hxc_l_tmp_load_result_n1146 = hxc_l_height_h867c1b5ebe9e;
        DrawTexturePro(hxc_l_tmp_load_result_n1139, (struct Rectangle){ .x = (float)0.0, .y = (float)0.0, .width = (float)hxc_l_tmp_load_result_n1140, .height = (float)hxc_l_tmp_load_result_n1141 }, (struct Rectangle){ .x = (float)((double)hxc_l_tmp_load_result_n1143 - hxc_l_tmp_load_result_n1144 * 0.5), .y = (float)28.0, .width = (float)hxc_l_tmp_load_result_n1145, .height = (float)hxc_l_tmp_load_result_n1146 }, (struct Vector2){ .x = (float)0.0, .y = (float)0.0 }, (float)0.0, hxc_l_this1_h7fb0ccf6eb78);
      }
      else
      {
        int32_t hxc_l_x_h3ed9605bd465 = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_l_width_h3f9a6d706c62, (double)2)), 92);
        struct Color hxc_l_this1_h467e2c132d4e = (struct Color){ .r = 242, .g = 249, .b = 245, .a = 255 };
        struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_tmp_load_result_n1152 = hxc_l_uiCatalog;
        hxc_l_gc_roots[211] = (const void *)hxc_l_tmp_load_result_n1152;
        if (hxc_l_tmp_load_result_n1152 == NULL)
        {
          abort();
        }
        hxc_string hxc_l_tmp_instance_call_result_n1154 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_tmp_load_result_n1152, hxc_l_locale, 34);
        hxc_l_tmp_cstring_ref_owner_owner_n360 = hxc_l_tmp_instance_call_result_n1154;
        hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n1156 = HXC_BORROWED_CSTRING_INITIALIZER;
        if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n360, &hxc_l_tmp_string_borrow_cstring_result_n1156) != HXC_STATUS_OK)
        {
          abort();
        }
        int32_t hxc_l_tmp_load_result_n1157 = hxc_l_x_h3ed9605bd465;
        DrawText(hxc_l_tmp_string_borrow_cstring_result_n1156.data, (int32_t)hxc_l_tmp_load_result_n1157, (int32_t)52, (int32_t)34, hxc_l_this1_h467e2c132d4e);
        if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n360) != HXC_STATUS_OK)
        {
          abort();
        }
      }
      int32_t hxc_l_firstTop = hxc_i32_subtract_wrapping(hxc_l_height_h074fc9df30c8, 293);
      int32_t hxc_l_left_ha42acd952598 = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_l_width_h3f9a6d706c62, (double)2)), 180);
      if (hxc_l_selectedMode == 0)
      {
        struct Color hxc_l_this1_hf1b3ab26a414 = (struct Color){ .r = 16, .g = 88, .b = 102, .a = 232 };
        int32_t hxc_l_tmp_load_result_n1163 = hxc_l_left_ha42acd952598;
        int32_t hxc_l_tmp_load_result_n1164 = hxc_l_firstTop;
        DrawRectangle((int32_t)hxc_l_tmp_load_result_n1163, (int32_t)hxc_l_tmp_load_result_n1164, (int32_t)360, (int32_t)54, hxc_l_this1_hf1b3ab26a414);
        struct Color hxc_l_this1_hfa5706b11602 = (struct Color){ .r = 255, .g = 190, .b = 55, .a = 255 };
        int32_t hxc_l_tmp_load_result_n1167 = hxc_l_left_ha42acd952598;
        int32_t hxc_l_tmp_load_result_n1168 = hxc_l_firstTop;
        DrawRectangleLines((int32_t)hxc_l_tmp_load_result_n1167, (int32_t)hxc_l_tmp_load_result_n1168, (int32_t)360, (int32_t)54, hxc_l_this1_hfa5706b11602);
      }
      else
      {
        struct Color hxc_l_this1_h68d985a2d0f3 = (struct Color){ .r = 6, .g = 26, .b = 36, .a = 218 };
        int32_t hxc_l_tmp_load_result_n1171 = hxc_l_left_ha42acd952598;
        int32_t hxc_l_tmp_load_result_n1172 = hxc_l_firstTop;
        DrawRectangle((int32_t)hxc_l_tmp_load_result_n1171, (int32_t)hxc_l_tmp_load_result_n1172, (int32_t)360, (int32_t)54, hxc_l_this1_h68d985a2d0f3);
        struct Color hxc_l_this1_ha5c613b5a867 = (struct Color){ .r = 92, .g = 194, .b = 188, .a = 255 };
        int32_t hxc_l_tmp_load_result_n1175 = hxc_l_left_ha42acd952598;
        int32_t hxc_l_tmp_load_result_n1176 = hxc_l_firstTop;
        DrawRectangleLines((int32_t)hxc_l_tmp_load_result_n1175, (int32_t)hxc_l_tmp_load_result_n1176, (int32_t)360, (int32_t)54, hxc_l_this1_ha5c613b5a867);
      }
      struct Color hxc_l_this1_hfd769f88defc = (struct Color){ .r = 242, .g = 249, .b = 245, .a = 255 };
      struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_tmp_load_result_n1179 = hxc_l_uiCatalog;
      hxc_l_gc_roots[212] = (const void *)hxc_l_tmp_load_result_n1179;
      if (hxc_l_tmp_load_result_n1179 == NULL)
      {
        abort();
      }
      hxc_string hxc_l_tmp_instance_call_result_n1181 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_tmp_load_result_n1179, hxc_l_locale, 27);
      hxc_l_tmp_cstring_ref_owner_owner_n368 = hxc_l_tmp_instance_call_result_n1181;
      hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n1183 = HXC_BORROWED_CSTRING_INITIALIZER;
      if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n368, &hxc_l_tmp_string_borrow_cstring_result_n1183) != HXC_STATUS_OK)
      {
        abort();
      }
      int32_t hxc_l_tmp_load_result_n1184 = hxc_l_left_ha42acd952598;
      int32_t hxc_l_tmp_load_result_n1185 = hxc_l_firstTop;
      DrawText(hxc_l_tmp_string_borrow_cstring_result_n1183.data, (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n1184, 28), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n1185, 16), (int32_t)21, hxc_l_this1_hfd769f88defc);
      int32_t hxc_l_top_h341355c74d21 = hxc_i32_add_wrapping(hxc_i32_add_wrapping(hxc_l_firstTop, 54), 14);
      int32_t hxc_l_left_h61ab04dd86e1 = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_l_width_h3f9a6d706c62, (double)2)), 180);
      if (hxc_l_selectedMode == 1)
      {
        struct Color hxc_l_this1_hada760542d55 = (struct Color){ .r = 16, .g = 88, .b = 102, .a = 232 };
        int32_t hxc_l_tmp_load_result_n1191 = hxc_l_left_h61ab04dd86e1;
        int32_t hxc_l_tmp_load_result_n1192 = hxc_l_top_h341355c74d21;
        DrawRectangle((int32_t)hxc_l_tmp_load_result_n1191, (int32_t)hxc_l_tmp_load_result_n1192, (int32_t)360, (int32_t)54, hxc_l_this1_hada760542d55);
        struct Color hxc_l_this1_hf1ebe3fa4b17 = (struct Color){ .r = 255, .g = 190, .b = 55, .a = 255 };
        int32_t hxc_l_tmp_load_result_n1195 = hxc_l_left_h61ab04dd86e1;
        int32_t hxc_l_tmp_load_result_n1196 = hxc_l_top_h341355c74d21;
        DrawRectangleLines((int32_t)hxc_l_tmp_load_result_n1195, (int32_t)hxc_l_tmp_load_result_n1196, (int32_t)360, (int32_t)54, hxc_l_this1_hf1ebe3fa4b17);
      }
      else
      {
        struct Color hxc_l_this1_hc15ddba62631 = (struct Color){ .r = 6, .g = 26, .b = 36, .a = 218 };
        int32_t hxc_l_tmp_load_result_n1199 = hxc_l_left_h61ab04dd86e1;
        int32_t hxc_l_tmp_load_result_n1200 = hxc_l_top_h341355c74d21;
        DrawRectangle((int32_t)hxc_l_tmp_load_result_n1199, (int32_t)hxc_l_tmp_load_result_n1200, (int32_t)360, (int32_t)54, hxc_l_this1_hc15ddba62631);
        struct Color hxc_l_this1_h075d852a32a1 = (struct Color){ .r = 92, .g = 194, .b = 188, .a = 255 };
        int32_t hxc_l_tmp_load_result_n1203 = hxc_l_left_h61ab04dd86e1;
        int32_t hxc_l_tmp_load_result_n1204 = hxc_l_top_h341355c74d21;
        DrawRectangleLines((int32_t)hxc_l_tmp_load_result_n1203, (int32_t)hxc_l_tmp_load_result_n1204, (int32_t)360, (int32_t)54, hxc_l_this1_h075d852a32a1);
      }
      struct Color hxc_l_this1_h0fb0cd788821 = (struct Color){ .r = 242, .g = 249, .b = 245, .a = 255 };
      struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_tmp_load_result_n1207 = hxc_l_uiCatalog;
      hxc_l_gc_roots[213] = (const void *)hxc_l_tmp_load_result_n1207;
      if (hxc_l_tmp_load_result_n1207 == NULL)
      {
        abort();
      }
      hxc_string hxc_l_tmp_instance_call_result_n1209 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_tmp_load_result_n1207, hxc_l_locale, 26);
      hxc_l_tmp_cstring_ref_owner_owner_n376 = hxc_l_tmp_instance_call_result_n1209;
      hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n1211 = HXC_BORROWED_CSTRING_INITIALIZER;
      if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n376, &hxc_l_tmp_string_borrow_cstring_result_n1211) != HXC_STATUS_OK)
      {
        abort();
      }
      int32_t hxc_l_tmp_load_result_n1212 = hxc_l_left_h61ab04dd86e1;
      int32_t hxc_l_tmp_load_result_n1213 = hxc_l_top_h341355c74d21;
      DrawText(hxc_l_tmp_string_borrow_cstring_result_n1211.data, (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n1212, 28), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n1213, 16), (int32_t)21, hxc_l_this1_h0fb0cd788821);
      int32_t hxc_l_top_hcd5eb5608d7d = hxc_i32_add_wrapping(hxc_l_firstTop, 136);
      int32_t hxc_l_left_h8c468758f46b = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_l_width_h3f9a6d706c62, (double)2)), 180);
      struct Color hxc_l_this1_hdc79b3c468e2 = (struct Color){ .r = 6, .g = 26, .b = 36, .a = 218 };
      int32_t hxc_l_tmp_load_result_n1218 = hxc_l_left_h8c468758f46b;
      int32_t hxc_l_tmp_load_result_n1219 = hxc_l_top_hcd5eb5608d7d;
      DrawRectangle((int32_t)hxc_l_tmp_load_result_n1218, (int32_t)hxc_l_tmp_load_result_n1219, (int32_t)360, (int32_t)54, hxc_l_this1_hdc79b3c468e2);
      struct Color hxc_l_this1_h9b7d45581483 = (struct Color){ .r = 92, .g = 194, .b = 188, .a = 255 };
      int32_t hxc_l_tmp_load_result_n1222 = hxc_l_left_h8c468758f46b;
      int32_t hxc_l_tmp_load_result_n1223 = hxc_l_top_hcd5eb5608d7d;
      DrawRectangleLines((int32_t)hxc_l_tmp_load_result_n1222, (int32_t)hxc_l_tmp_load_result_n1223, (int32_t)360, (int32_t)54, hxc_l_this1_h9b7d45581483);
      struct Color hxc_l_this1_h1141ffe7fd46 = (struct Color){ .r = 242, .g = 249, .b = 245, .a = 255 };
      struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_tmp_load_result_n1226 = hxc_l_uiCatalog;
      hxc_l_gc_roots[214] = (const void *)hxc_l_tmp_load_result_n1226;
      if (hxc_l_tmp_load_result_n1226 == NULL)
      {
        abort();
      }
      hxc_string hxc_l_tmp_instance_call_result_n1228 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_tmp_load_result_n1226, hxc_l_locale, 28);
      hxc_l_tmp_cstring_ref_owner_owner_n382 = hxc_l_tmp_instance_call_result_n1228;
      hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n1230 = HXC_BORROWED_CSTRING_INITIALIZER;
      if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n382, &hxc_l_tmp_string_borrow_cstring_result_n1230) != HXC_STATUS_OK)
      {
        abort();
      }
      int32_t hxc_l_tmp_load_result_n1231 = hxc_l_left_h8c468758f46b;
      int32_t hxc_l_tmp_load_result_n1232 = hxc_l_top_hcd5eb5608d7d;
      DrawText(hxc_l_tmp_string_borrow_cstring_result_n1230.data, (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n1231, 28), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n1232, 16), (int32_t)21, hxc_l_this1_h1141ffe7fd46);
      int32_t hxc_l_x_he41e6115bd09 = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_l_width_h3f9a6d706c62, (double)2)), 285);
      struct Color hxc_l_this1_h65e5e0b6262a = (struct Color){ .r = 229, .g = 241, .b = 235, .a = 255 };
      struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_tmp_load_result_n1236 = hxc_l_uiCatalog;
      hxc_l_gc_roots[215] = (const void *)hxc_l_tmp_load_result_n1236;
      if (hxc_l_tmp_load_result_n1236 == NULL)
      {
        abort();
      }
      hxc_string hxc_l_tmp_instance_call_result_n1238 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_tmp_load_result_n1236, hxc_l_locale, 29);
      hxc_l_tmp_cstring_ref_owner_owner_n385 = hxc_l_tmp_instance_call_result_n1238;
      hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n1240 = HXC_BORROWED_CSTRING_INITIALIZER;
      if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n385, &hxc_l_tmp_string_borrow_cstring_result_n1240) != HXC_STATUS_OK)
      {
        abort();
      }
      int32_t hxc_l_tmp_load_result_n1241 = hxc_l_x_he41e6115bd09;
      int32_t hxc_l_tmp_load_result_n1242 = hxc_l_height_h074fc9df30c8;
      DrawText(hxc_l_tmp_string_borrow_cstring_result_n1240.data, (int32_t)hxc_l_tmp_load_result_n1241, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n1242, 58), (int32_t)16, hxc_l_this1_h65e5e0b6262a);
      if (hxc_l_selectedMode == 1)
      {
        int32_t hxc_l_x_h98f12e35bef9 = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_l_width_h3f9a6d706c62, (double)2)), 230);
        struct Color hxc_l_this1_h687f6a40c850 = (struct Color){ .r = 255, .g = 205, .b = 91, .a = 255 };
        const char *hxc_l_tmp_call_result_n1248 = hxc_caxecraft_localization_FirstPlayableCatalog_text(hxc_l_locale, 1);
        int32_t hxc_l_tmp_load_result_n1249 = hxc_l_x_h98f12e35bef9;
        int32_t hxc_l_tmp_load_result_n1250 = hxc_l_firstTop;
        DrawText(hxc_l_tmp_call_result_n1248, (int32_t)hxc_l_tmp_load_result_n1249, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n1250, 36), (int32_t)17, hxc_l_this1_h687f6a40c850);
      }
      if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n385) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n382) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n376) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n368) != HXC_STATUS_OK)
      {
        abort();
      }
    }
    else
    {
      if (hxc_l_onCampaignSelect)
      {
        hxc_l_gc_roots[216] = (const void *)hxc_l_campaign;
        struct hxc_caxecraft_content_CampaignManifest *hxc_l_selectedCampaign_h4a79f57cc99e = hxc_l_campaign;
        hxc_l_gc_roots[217] = (const void *)hxc_l_selectedCampaign_h4a79f57cc99e;
        hxc_l_gc_roots[218] = (const void *)NULL;
        if (hxc_l_selectedCampaign_h4a79f57cc99e == NULL)
        {
          enum hxc_caxecraft_app_AppScreen hxc_l_tmp_call_result_n1256 = hxc_caxecraft_app_AppScreen_closeCampaignSelection(hxc_l_screen);
          hxc_l_screen = hxc_l_tmp_call_result_n1256;
        }
        else
        {
          struct Texture hxc_l_tmp_load_result_n1257 = hxc_l_titleTexture;
          bool hxc_l_tmp_load_result_n1258 = hxc_l_titleTextureReady;
          struct Texture hxc_l_tmp_load_result_n1259 = hxc_l_wordmarkTexture;
          bool hxc_l_tmp_load_result_n1260 = hxc_l_wordmarkTextureReady;
          struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n1261 = hxc_l_selectedCampaign_h4a79f57cc99e;
          hxc_l_gc_roots[219] = (const void *)hxc_l_tmp_load_result_n1261;
          int32_t hxc_l_tmp_load_result_n1262 = hxc_l_locale;
          struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_tmp_load_result_n1263 = hxc_l_uiCatalog;
          hxc_l_gc_roots[220] = (const void *)hxc_l_tmp_load_result_n1263;
          hxc_caxecraft_app_CampaignMenu_draw(hxc_l_tmp_load_result_n1257, hxc_l_tmp_load_result_n1258, hxc_l_tmp_load_result_n1259, hxc_l_tmp_load_result_n1260, hxc_l_tmp_load_result_n1261, hxc_l_tmp_load_result_n1262, hxc_l_tmp_load_result_n1263, hxc_l_selectedCampaignLevelIndex);
        }
      }
      else
      {
        if (hxc_l_onEditor)
        {
          struct hxc_caxecraft_app_CaxecraftEditorScreen *hxc_l_tmp_load_result_n1266 = hxc_l_editorScreen;
          hxc_l_gc_roots[221] = (const void *)hxc_l_tmp_load_result_n1266;
          if (hxc_l_tmp_load_result_n1266 == NULL)
          {
            abort();
          }
          int32_t hxc_l_tmp_load_result_n1267 = hxc_l_locale;
          enum hxc_caxecraft_app_EditorScreenAction hxc_l_tmp_instance_call_result_n1269 = hxc_caxecraft_app_CaxecraftEditorScreen_draw(hxc_l_tmp_load_result_n1266, hxc_l_tmp_load_result_n1267, hxc_l_editorNavigationCommand);
          if (hxc_l_tmp_instance_call_result_n1269 == hxc_caxecraft_app_EditorScreenAction_ReturnToTitle)
          {
            enum hxc_caxecraft_app_AppScreen hxc_l_tmp_call_result_n1272 = hxc_caxecraft_app_AppScreen_closeEditor(hxc_l_screen);
            hxc_l_screen = hxc_l_tmp_call_result_n1272;
          }
        }
        else
        {
          ClearBackground((struct Color){ .r = 126, .g = 190, .b = 201, .a = 255 });
          int32_t hxc_l_tmp_native_call_result_n1274 = GetScreenWidth();
          int32_t hxc_l_sunX = hxc_i32_subtract_wrapping((int32_t)hxc_l_tmp_native_call_result_n1274, 260);
          float hxc_l_radius_h961b55477424 = (float)42.0;
          struct Color hxc_l_this1_h64c56e4eee04 = (struct Color){ .r = 255, .g = 226, .b = 136, .a = 255 };
          int32_t hxc_l_tmp_load_result_n1276 = hxc_l_sunX;
          float hxc_l_tmp_load_result_n1277 = hxc_l_radius_h961b55477424;
          DrawCircle((int32_t)hxc_l_tmp_load_result_n1276, (int32_t)86, hxc_l_tmp_load_result_n1277, hxc_l_this1_h64c56e4eee04);
          float hxc_l_radius_hbdb4ba640b5e = (float)30.0;
          struct Color hxc_l_this1_h986025d66f8b = (struct Color){ .r = 255, .g = 246, .b = 205, .a = 255 };
          int32_t hxc_l_tmp_load_result_n1280 = hxc_l_sunX;
          float hxc_l_tmp_load_result_n1281 = hxc_l_radius_hbdb4ba640b5e;
          DrawCircle((int32_t)hxc_l_tmp_load_result_n1280, (int32_t)86, hxc_l_tmp_load_result_n1281, hxc_l_this1_h986025d66f8b);
          BeginMode3D(hxc_l_camera);
          if (hxc_l_self == NULL)
          {
            abort();
          }
          struct hxc_caxecraft_app_TerrainRenderer *hxc_l_tmp_owned_class_field_address_n1284 = &(*hxc_l_self).hxc_terrainRenderer;
          if (hxc_l_tmp_owned_class_field_address_n1284 == NULL)
          {
            abort();
          }
          struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1285 = hxc_l_session;
          hxc_l_gc_roots[222] = (const void *)hxc_l_tmp_load_result_n1285;
          if (hxc_l_tmp_load_result_n1285 == NULL)
          {
            abort();
          }
          size_t hxc_l_tmp_length_n1287;
          const uint8_t *hxc_l_tmp_instance_call_result_n1286 = hxc_caxecraft_domain_GameSession_worldView(hxc_l_tmp_load_result_n1285, &hxc_l_tmp_length_n1287);
          struct Texture hxc_l_tmp_load_result_n1288 = hxc_l_terrainTexture;
          bool hxc_l_tmp_load_result_n1289 = hxc_l_terrainTextureReady;
          struct Texture hxc_l_tmp_load_result_n1290 = hxc_l_adventureTerrainTexture;
          bool hxc_l_tmp_load_result_n1291 = hxc_l_adventureTerrainTextureReady;
          double hxc_l_tmp_record_field_load_result_n1292 = hxc_l_renderPosition.hxc_x;
          struct hxc_caxecraft_app_TerrainRenderCounters hxc_l_tmp_instance_call_result_n1294 = hxc_caxecraft_app_TerrainRenderer_draw(hxc_l_tmp_owned_class_field_address_n1284, hxc_l_tmp_instance_call_result_n1286, hxc_l_tmp_length_n1287, hxc_l_tmp_load_result_n1288, hxc_l_tmp_load_result_n1289, hxc_l_tmp_load_result_n1290, hxc_l_tmp_load_result_n1291, hxc_l_tmp_record_field_load_result_n1292, hxc_l_renderPosition.hxc_z);
          struct hxc_caxecraft_app_TerrainRenderCounters hxc_l_renderCounters = hxc_l_tmp_instance_call_result_n1294;
          struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1295 = hxc_l_session;
          hxc_l_gc_roots[223] = (const void *)hxc_l_tmp_load_result_n1295;
          if (hxc_l_tmp_load_result_n1295 == NULL)
          {
            abort();
          }
          size_t hxc_l_tmp_length_n1297;
          const uint8_t *hxc_l_tmp_instance_call_result_n1296 = hxc_caxecraft_domain_GameSession_worldView(hxc_l_tmp_load_result_n1295, &hxc_l_tmp_length_n1297);
          struct Texture hxc_l_tmp_load_result_n1298 = hxc_l_terrainTexture;
          bool hxc_l_tmp_load_result_n1299 = hxc_l_terrainTextureReady;
          struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1300 = hxc_l_levelView;
          hxc_l_gc_roots[224] = (const void *)hxc_l_tmp_load_result_n1300;
          if (hxc_l_tmp_load_result_n1300 == NULL)
          {
            abort();
          }
          int32_t hxc_l_tmp_class_field_load_result_n1301 = (*hxc_l_tmp_load_result_n1300).hxc_waterCell;
          struct hxc_caxecraft_app_WaterRenderCounters hxc_l_tmp_call_result_n1302 = hxc_caxecraft_app_WaterRenderer_draw(hxc_l_tmp_instance_call_result_n1296, hxc_l_tmp_length_n1297, hxc_l_tmp_load_result_n1298, hxc_l_tmp_load_result_n1299, hxc_l_tmp_class_field_load_result_n1301);
          struct hxc_caxecraft_app_WaterRenderCounters hxc_l_waterCounters = hxc_l_tmp_call_result_n1302;
          int32_t hxc_l_tmp_record_field_load_result_n1303 = hxc_l_renderCounters.hxc_visible;
          int32_t hxc_l_totalVisible = hxc_i32_add_wrapping(hxc_l_tmp_record_field_load_result_n1303, hxc_l_waterCounters.hxc_visible);
          int32_t hxc_l_tmp_record_field_load_result_n1305 = hxc_l_renderCounters.hxc_drawCalls;
          int32_t hxc_l_totalDrawCalls = hxc_i32_add_wrapping(hxc_l_tmp_record_field_load_result_n1305, hxc_l_waterCounters.hxc_drawCalls);
          struct Camera3D hxc_l_tmp_load_result_n1307 = hxc_l_camera;
          struct Texture hxc_l_tmp_load_result_n1308 = hxc_l_entityTexture;
          bool hxc_l_tmp_load_result_n1309 = hxc_l_entityTextureReady;
          struct hxc_caxecraft_domain_Character hxc_l_tmp_load_result_n1310 = hxc_l_dialogueActor;
          struct hxc_caxecraft_domain_Character hxc_l_tmp_load_result_n1311 = hxc_l_enemyActor;
          enum hxc_caxecraft_domain_ActorControllerPhase hxc_l_tmp_record_field_load_result_n1312 = hxc_l_enemyPhase_h70382f3bdc66.hxc_phase;
          hxc_caxecraft_app_CaxecraftApp_drawActors(hxc_l_tmp_load_result_n1307, hxc_l_tmp_load_result_n1308, hxc_l_tmp_load_result_n1309, hxc_l_tmp_load_result_n1310, hxc_l_tmp_load_result_n1311, hxc_l_tmp_record_field_load_result_n1312, hxc_l_berryDrop);
          struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_tmp_load_result_n1314 = hxc_l_contentRegistry;
          hxc_l_gc_roots[225] = (const void *)hxc_l_tmp_load_result_n1314;
          struct Camera3D hxc_l_tmp_load_result_n1315 = hxc_l_camera;
          struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1316 = hxc_l_session;
          hxc_l_gc_roots[226] = (const void *)hxc_l_tmp_load_result_n1316;
          if (hxc_l_tmp_load_result_n1316 == NULL)
          {
            abort();
          }
          size_t hxc_l_tmp_length_n1318;
          const int32_t *hxc_l_tmp_instance_call_result_n1317 = hxc_caxecraft_domain_GameSession_authoredItemsView(hxc_l_tmp_load_result_n1316, &hxc_l_tmp_length_n1318);
          struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1319 = hxc_l_levelView;
          hxc_l_gc_roots[227] = (const void *)hxc_l_tmp_load_result_n1319;
          struct Texture hxc_l_tmp_load_result_n1320 = hxc_l_itemTexture;
          bool hxc_l_tmp_load_result_n1321 = hxc_l_itemTextureReady;
          struct Texture hxc_l_tmp_load_result_n1322 = hxc_l_adventureItemTexture;
          hxc_caxecraft_app_AuthoredItemRenderer_drawWorldItems(hxc_l_tmp_load_result_n1314, hxc_l_tmp_load_result_n1315, hxc_l_tmp_instance_call_result_n1317, hxc_l_tmp_length_n1318, hxc_l_tmp_load_result_n1319, hxc_l_tmp_load_result_n1320, hxc_l_tmp_load_result_n1321, hxc_l_tmp_load_result_n1322, hxc_l_adventureItemTextureReady);
          if (hxc_l_hit.hxc_hit)
          {
            double hxc_l_y = (double)hxc_l_hit.hxc_cellY + 0.5;
            double hxc_l_z = (double)hxc_l_hit.hxc_cellZ + 0.5;
            int32_t hxc_l_tmp_record_field_load_result_n1327 = hxc_l_hit.hxc_cellX;
            double hxc_l_tmp_load_result_n1328 = hxc_l_y;
            DrawCubeWires((struct Vector3){ .x = (float)((double)hxc_l_tmp_record_field_load_result_n1327 + 0.5), .y = (float)hxc_l_tmp_load_result_n1328, .z = (float)hxc_l_z }, (float)1.04, (float)1.04, (float)1.04, (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 });
          }
          EndMode3D();
          if (hxc_l_cameraWaterBlend > 0.0)
          {
            int32_t hxc_l_overlayAlpha = hxc_f64_to_i32_saturating(105.0 * hxc_l_cameraWaterBlend);
            int32_t hxc_l_tmp_native_call_result_n1334 = GetScreenWidth();
            int32_t hxc_l_width_h40aada14a6ce = (int32_t)hxc_l_tmp_native_call_result_n1334;
            int32_t hxc_l_tmp_native_call_result_n1335 = GetScreenHeight();
            int32_t hxc_l_height_h68d557d0c5b1 = (int32_t)hxc_l_tmp_native_call_result_n1335;
            uint8_t hxc_l_red = (uint8_t)31;
            uint8_t hxc_l_green = (uint8_t)115;
            uint8_t hxc_l_blue = (uint8_t)154;
            int32_t hxc_l_tmp_load_result_n1336 = hxc_l_overlayAlpha;
            int32_t hxc_l_tmp_conditional_result_n407 = 0;
            if (hxc_l_tmp_load_result_n1336 < 0)
            {
              hxc_l_tmp_conditional_result_n407 = 0;
            }
            else
            {
              int32_t hxc_l_tmp_load_result_n1337 = hxc_l_overlayAlpha;
              int32_t hxc_l_tmp_conditional_result_n408 = 0;
              if (hxc_l_tmp_load_result_n1337 > 255)
              {
                hxc_l_tmp_conditional_result_n408 = 255;
              }
              else
              {
                hxc_l_tmp_conditional_result_n408 = hxc_l_overlayAlpha;
              }
              hxc_l_tmp_conditional_result_n407 = hxc_l_tmp_conditional_result_n408;
            }
            uint8_t hxc_l_alpha = (uint8_t)hxc_l_tmp_conditional_result_n407;
            uint8_t hxc_l_tmp_load_result_n1341 = hxc_l_red;
            uint8_t hxc_l_tmp_load_result_n1342 = hxc_l_green;
            uint8_t hxc_l_tmp_load_result_n1343 = hxc_l_blue;
            struct Color hxc_l_this1_h890e67330fd6 = (struct Color){ .r = hxc_l_tmp_load_result_n1341, .g = hxc_l_tmp_load_result_n1342, .b = hxc_l_tmp_load_result_n1343, .a = hxc_l_alpha };
            int32_t hxc_l_tmp_load_result_n1346 = hxc_l_width_h40aada14a6ce;
            int32_t hxc_l_tmp_load_result_n1347 = hxc_l_height_h68d557d0c5b1;
            DrawRectangle((int32_t)0, (int32_t)0, (int32_t)hxc_l_tmp_load_result_n1346, (int32_t)hxc_l_tmp_load_result_n1347, hxc_l_this1_h890e67330fd6);
          }
          double hxc_l_tmp_record_field_load_result_n1349 = hxc_l_character.hxc_body.hxc_x;
          double hxc_l_tmp_record_field_load_result_n1350 = hxc_l_character.hxc_body.hxc_z;
          struct hxc_caxecraft_domain_VitalsState hxc_l_tmp_record_field_load_result_n1351 = hxc_l_character.hxc_vitals;
          int32_t hxc_l_tmp_load_result_n1352 = hxc_l_aquaticEquipmentCode;
          int32_t hxc_l_tmp_load_result_n1353 = hxc_l_aquaticEquipmentFrames;
          bool hxc_l_tmp_record_field_load_result_n1354 = hxc_l_character.hxc_aquatic.hxc_headSubmerged;
          int32_t hxc_l_tmp_record_field_load_result_n1355 = hxc_l_character.hxc_aquatic.hxc_breathTicks;
          struct hxc_caxecraft_app_HudCharacterView hxc_l_hudView_h74858b86ffca = (struct hxc_caxecraft_app_HudCharacterView){ .hxc_aquaticEquipmentCode = hxc_l_tmp_load_result_n1352, .hxc_aquaticEquipmentVisible = hxc_l_tmp_load_result_n1353 > 0, .hxc_breathTicks = hxc_l_tmp_record_field_load_result_n1355, .hxc_headSubmerged = hxc_l_tmp_record_field_load_result_n1354, .hxc_maximumBreathTicks = hxc_l_character.hxc_aquaticProfile.hxc_maximumBreathTicks, .hxc_vitals = hxc_l_tmp_record_field_load_result_n1351, .hxc_x = hxc_l_tmp_record_field_load_result_n1349, .hxc_z = hxc_l_tmp_record_field_load_result_n1350 };
          bool hxc_l_hudView1 = hxc_l_placementBlockedFrames > 0;
          bool hxc_l_hudView2 = hxc_l_strikeHitFrames > 0;
          bool hxc_l_hudView3 = hxc_l_enemyDefeatedFrames > 0;
          bool hxc_l_hudView4 = hxc_l_enemyAttackFrames > 0;
          bool hxc_l_hudView5 = hxc_l_pickupFrames > 0;
          bool hxc_l_hudView6 = hxc_l_recoveryFeedbackFrames > 0;
          enum hxc_caxecraft_domain_ActorControllerPhase hxc_l_enemyPhase_h651fef803f7e = hxc_l_enemyPhase_h70382f3bdc66.hxc_phase;
          hxc_string hxc_l_locale1 = { 0 };
          switch (hxc_l_locale) {
            case 0:
              {
                hxc_l_locale1 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
                if (hxc_string_retain(hxc_l_locale1) != HXC_STATUS_OK)
                {
                  abort();
                }
                break;
              }
            case 1:
              {
                hxc_l_locale1 = (hxc_string){ (const uint8_t *)"es-mx", 5, true, NULL };
                if (hxc_string_retain(hxc_l_locale1) != HXC_STATUS_OK)
                {
                  abort();
                }
                break;
              }
            default:
              {
                hxc_l_locale1 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
                if (hxc_string_retain(hxc_l_locale1) != HXC_STATUS_OK)
                {
                  abort();
                }
                break;
              }
          }
          hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n1366 = hxc_l_locale1;
          hxc_l_tmp_managed_flow_owner_n419 = hxc_l_tmp_managed_flow_carrier_move_result_n1366;
          struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1367 = hxc_l_levelView;
          hxc_l_gc_roots[228] = (const void *)hxc_l_tmp_load_result_n1367;
          if (hxc_l_tmp_load_result_n1367 == NULL)
          {
            abort();
          }
          struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_class_field_load_result_n1368 = (*hxc_l_tmp_load_result_n1367).hxc_levelPresentation;
          hxc_l_gc_roots[229] = (const void *)hxc_l_tmp_class_field_load_result_n1368;
          if (hxc_l_tmp_class_field_load_result_n1368 == NULL)
          {
            abort();
          }
          hxc_string hxc_l_tmp_load_result_n1369 = hxc_l_currentObjectiveId;
          hxc_string hxc_l_tmp_instance_call_result_n1371 = hxc_caxecraft_content_RuntimeLevelPresentation_objectiveTitle(hxc_l_tmp_class_field_load_result_n1368, hxc_l_tmp_load_result_n1369, hxc_l_tmp_managed_flow_owner_n419);
          hxc_l_hudView7 = hxc_l_tmp_instance_call_result_n1371;
          int32_t hxc_l_tmp_load_result_n1372 = hxc_l_totalVisible;
          int32_t hxc_l_tmp_load_result_n1373 = hxc_l_totalDrawCalls;
          int32_t hxc_l_tmp_load_result_n1374 = hxc_l_frameCount;
          int32_t hxc_l_tmp_load_result_n1375 = hxc_l_completedTicks;
          struct hxc_caxecraft_app_HudCharacterView hxc_l_tmp_load_result_n1377 = hxc_l_hudView_h74858b86ffca;
          bool hxc_l_tmp_load_result_n1378 = hxc_l_hudView1;
          bool hxc_l_tmp_load_result_n1379 = hxc_l_hudView2;
          bool hxc_l_tmp_load_result_n1380 = hxc_l_hudView3;
          bool hxc_l_tmp_load_result_n1381 = hxc_l_hudView4;
          bool hxc_l_tmp_load_result_n1382 = hxc_l_hudView5;
          int32_t hxc_l_tmp_load_result_n1383 = hxc_l_pickupAmount;
          int32_t hxc_l_tmp_load_result_n1384 = hxc_l_inventoryFullReason;
          int32_t hxc_l_tmp_load_result_n1385 = hxc_l_recoveryFeedback;
          bool hxc_l_tmp_load_result_n1386 = hxc_l_hudView6;
          bool hxc_l_tmp_load_result_n1388 = hxc_l_paused_h0af3ff7e4c3b;
          bool hxc_l_tmp_load_result_n1389 = hxc_l_captured_h8abb2355e37f;
          struct hxc_caxecraft_domain_RaycastHit hxc_l_tmp_load_result_n1390 = hxc_l_hit;
          int32_t hxc_l_tmp_load_result_n1391 = hxc_l_selectedMode;
          int32_t hxc_l_tmp_load_result_n1392 = hxc_l_locale;
          struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_load_result_n1393 = hxc_l_inventory;
          enum hxc_caxecraft_gameplay_GuidePhase hxc_l_tmp_load_result_n1394 = hxc_l_guidePhase;
          bool hxc_l_tmp_load_result_n1395 = hxc_l_guideInteractionAvailable;
          struct hxc_caxecraft_domain_Character hxc_l_tmp_load_result_n1396 = hxc_l_enemyActor;
          enum hxc_caxecraft_domain_ActorControllerPhase hxc_l_tmp_load_result_n1397 = hxc_l_enemyPhase_h651fef803f7e;
          hxc_string hxc_l_tmp_record_field_levelLabel_owner_n422 = hxc_l_levelLabel;
          if (hxc_string_retain(hxc_l_tmp_record_field_levelLabel_owner_n422) != HXC_STATUS_OK)
          {
            abort();
          }
          hxc_string hxc_l_tmp_record_field_levelLabel_owned_load_result_n1399 = hxc_l_tmp_record_field_levelLabel_owner_n422;
          hxc_string hxc_l_tmp_record_field_objectiveTitle_owner_n423 = hxc_l_hudView7;
          if (hxc_string_retain(hxc_l_tmp_record_field_objectiveTitle_owner_n423) != HXC_STATUS_OK)
          {
            abort();
          }
          hxc_l_hudView_hbd7f23c4712c = (struct hxc_caxecraft_app_HudView){ .hxc_character = hxc_l_tmp_load_result_n1377, .hxc_enemy = hxc_l_tmp_load_result_n1396, .hxc_enemyPhase = hxc_l_tmp_load_result_n1397, .hxc_feedback = (struct hxc_caxecraft_app_HudFeedbackView){ .hxc_enemyAttacked = hxc_l_tmp_load_result_n1381, .hxc_enemyDefeated = hxc_l_tmp_load_result_n1380, .hxc_inventoryFullReason = hxc_l_tmp_load_result_n1384, .hxc_pickedUp = hxc_l_tmp_load_result_n1382, .hxc_pickupAmount = hxc_l_tmp_load_result_n1383, .hxc_placementBlocked = hxc_l_tmp_load_result_n1378, .hxc_recoveryDecision = hxc_l_tmp_load_result_n1385, .hxc_recoveryVisible = hxc_l_tmp_load_result_n1386, .hxc_strikeHit = hxc_l_tmp_load_result_n1379 }, .hxc_guideInteractionAvailable = hxc_l_tmp_load_result_n1395, .hxc_guidePhase = hxc_l_tmp_load_result_n1394, .hxc_hit = hxc_l_tmp_load_result_n1390, .hxc_inventory = hxc_l_tmp_load_result_n1393, .hxc_levelLabel = hxc_l_tmp_record_field_levelLabel_owned_load_result_n1399, .hxc_locale = hxc_l_tmp_load_result_n1392, .hxc_metrics = (struct hxc_caxecraft_app_HudMetricsView){ .hxc_completedTicks = hxc_l_tmp_load_result_n1375, .hxc_drawCalls = hxc_l_tmp_load_result_n1373, .hxc_renderedFrames = hxc_l_tmp_load_result_n1374, .hxc_visibleBlocks = hxc_l_tmp_load_result_n1372 }, .hxc_mode = hxc_l_tmp_load_result_n1391, .hxc_objectiveTitle = hxc_l_tmp_record_field_objectiveTitle_owner_n423, .hxc_paused = hxc_l_tmp_load_result_n1388, .hxc_pointerCaptured = hxc_l_tmp_load_result_n1389 };
          struct hxc_caxecraft_app_HudView hxc_l_tmp_load_result_n1403 = hxc_l_hudView_hbd7f23c4712c;
          struct hxc_caxecraft_app_HudResources hxc_l_tmp_load_result_n1404 = hxc_l_hudResources;
          struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_tmp_load_result_n1405 = hxc_l_contentRegistry;
          hxc_l_gc_roots[230] = (const void *)hxc_l_tmp_load_result_n1405;
          hxc_l_gc_roots[231] = (const void *)hxc_l_uiCatalog;
          hxc_caxecraft_app_CaxecraftApp_drawHud(hxc_l_tmp_load_result_n1403, hxc_l_tmp_load_result_n1404, hxc_l_tmp_load_result_n1405, hxc_l_uiCatalog);
          hxc_record_9e5a983a_destroy(&hxc_l_hudView_hbd7f23c4712c);
          if (hxc_string_release(&hxc_l_hudView7) != HXC_STATUS_OK)
          {
            abort();
          }
          if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n419) != HXC_STATUS_OK)
          {
            abort();
          }
        }
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
  if (hxc_string_release(&hxc_l_currentObjectiveId) != HXC_STATUS_OK)
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
