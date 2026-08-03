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
  const void *hxc_l_gc_roots[230] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 230, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_string hxc_l_tmp_sys_println_string_argument_owner_n178 = { 0 };
  struct hxc_caxecraft_content_CampaignPackageLoadResult hxc_l_g_h6904c3060e30 = { 0 };
  struct hxc_caxecraft_content_CampaignLevelLoadResult hxc_l_g_hbd7f92d003e5 = { 0 };
  struct hxc_array_ref *hxc_l_phases = { 0 };
  struct hxc_caxecraft_content_RuntimeLevelReceipt hxc_l_tmp_record_field_logicalPath_receiver_owner_n22 = { 0 };
  hxc_string hxc_l_tmp_string_concat_left_owner_n220 = { 0 };
  hxc_string hxc_l_tmp_string_concat_left_owner_n221 = { 0 };
  hxc_string hxc_l_tmp_string_concat_right_owner_n222 = { 0 };
  hxc_string hxc_l_tmp_sys_println_string_argument_owner_n223 = { 0 };
  struct hxc_caxecraft_content_CampaignLevelLoadError hxc_l_error = { 0 };
  hxc_string hxc_l_tmp_string_concat_right_owner_n225 = { 0 };
  hxc_string hxc_l_tmp_sys_println_string_argument_owner_n226 = { 0 };
  struct hxc_caxecraft_content_CampaignPackageLoadError hxc_l_gerror = { 0 };
  hxc_string hxc_l_levelLabel = { 0 };
  struct hxc_caxecraft_content_RuntimeLevelReceipt hxc_l_tmp_record_field_logicalPath_receiver_owner_n26 = { 0 };
  struct hxc_array_ref *hxc_l_g1_h7bf1eee5d9d8 = { 0 };
  struct hxc_caxecraft_domain_ActorControllerEvent hxc_l_event = { 0 };
  struct hxc_caxecraft_domain_ActorControllerEvent hxc_l_tmp_array_element_owner_n280 = { 0 };
  hxc_string hxc_l_drop = { 0 };
  struct hxc_array_ref *hxc_l_actorPhases = { 0 };
  struct hxc_array_ref *hxc_l_presentationActorPhases = { 0 };
  struct hxc_caxecraft_content_RuntimeLevelReceipt hxc_l_receipt = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n347 = { 0 };
  hxc_string hxc_l_tmp_sys_println_string_argument_owner_n35 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n355 = { 0 };
  hxc_string hxc_l_tmp_string_concat_right_owner_n36 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n363 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n369 = { 0 };
  hxc_string hxc_l_tmp_sys_println_string_argument_owner_n37 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n372 = { 0 };
  hxc_string hxc_l_tmp_string_concat_right_owner_n38 = { 0 };
  hxc_string hxc_l_tmp_sys_println_string_argument_owner_n39 = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n406 = { 0 };
  hxc_string hxc_l_hudView7 = { 0 };
  struct hxc_caxecraft_app_HudView hxc_l_hudView_h74858b86ffca = { 0 };
  struct hxc_caxecraft_content_RuntimeContentLoadResult hxc_l_g_h4ef171f4fe67 = { 0 };
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
  struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n142 = hxc_l_initialSession;
  hxc_l_gc_roots[75] = (const void *)hxc_l_tmp_load_result_n142;
  if (hxc_l_tmp_load_result_n142 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n143 = hxc_l_initialLevel;
  hxc_l_gc_roots[76] = (const void *)hxc_l_tmp_load_result_n143;
  if (hxc_l_tmp_load_result_n143 == NULL)
  {
    abort();
  }
  int32_t hxc_l_tmp_class_field_load_result_n144 = (*hxc_l_tmp_load_result_n143).hxc_dialogueId;
  bool hxc_l_tmp_instance_call_result_n145 = hxc_caxecraft_domain_GameSession_actorInteractionAvailable(hxc_l_tmp_load_result_n142, hxc_l_tmp_class_field_load_result_n144);
  bool hxc_l_guideInteractionAvailable = hxc_l_tmp_instance_call_result_n145;
  struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n146 = hxc_l_initialSession;
  hxc_l_gc_roots[77] = (const void *)hxc_l_tmp_load_result_n146;
  if (hxc_l_tmp_load_result_n146 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n147 = hxc_l_initialLevel;
  hxc_l_gc_roots[78] = (const void *)hxc_l_tmp_load_result_n147;
  if (hxc_l_tmp_load_result_n147 == NULL)
  {
    abort();
  }
  int32_t hxc_l_tmp_class_field_load_result_n148 = (*hxc_l_tmp_load_result_n147).hxc_dialogueId;
  struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n149 = hxc_caxecraft_domain_GameSession_readCharacter(hxc_l_tmp_load_result_n146, hxc_l_tmp_class_field_load_result_n148);
  struct hxc_caxecraft_domain_Character hxc_l_dialogueActor = hxc_l_tmp_instance_call_result_n149;
  struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n150 = hxc_l_initialSession;
  hxc_l_gc_roots[79] = (const void *)hxc_l_tmp_load_result_n150;
  if (hxc_l_tmp_load_result_n150 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n151 = hxc_l_initialLevel;
  hxc_l_gc_roots[80] = (const void *)hxc_l_tmp_load_result_n151;
  if (hxc_l_tmp_load_result_n151 == NULL)
  {
    abort();
  }
  int32_t hxc_l_tmp_class_field_load_result_n152 = (*hxc_l_tmp_load_result_n151).hxc_enemyId;
  struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n153 = hxc_caxecraft_domain_GameSession_readCharacter(hxc_l_tmp_load_result_n150, hxc_l_tmp_class_field_load_result_n152);
  struct hxc_caxecraft_domain_Character hxc_l_enemyActor = hxc_l_tmp_instance_call_result_n153;
  struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n154 = hxc_l_initialSession;
  hxc_l_gc_roots[81] = (const void *)hxc_l_tmp_load_result_n154;
  if (hxc_l_tmp_load_result_n154 == NULL)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_tmp_instance_call_result_n155 = hxc_caxecraft_domain_GameSession_actorControllerStateSnapshots(hxc_l_tmp_load_result_n154);
  hxc_l_initialActorPhases = hxc_l_tmp_instance_call_result_n155;
  struct hxc_array_ref *hxc_l_tmp_load_result_n156 = hxc_l_initialActorPhases;
  struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n157 = hxc_l_initialLevel;
  hxc_l_gc_roots[82] = (const void *)hxc_l_tmp_load_result_n157;
  if (hxc_l_tmp_load_result_n157 == NULL)
  {
    abort();
  }
  int32_t hxc_l_tmp_class_field_load_result_n158 = (*hxc_l_tmp_load_result_n157).hxc_dialogueId;
  struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation hxc_l_tmp_call_result_n160 = hxc_caxecraft_app_CaxecraftApp_observeActorPhase(hxc_l_tmp_load_result_n156, hxc_l_tmp_class_field_load_result_n158, hxc_caxecraft_domain_ActorControllerPhase_Stationary);
  struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation hxc_l_dialoguePhase = hxc_l_tmp_call_result_n160;
  struct hxc_array_ref *hxc_l_tmp_load_result_n161 = hxc_l_initialActorPhases;
  struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n162 = hxc_l_initialLevel;
  hxc_l_gc_roots[83] = (const void *)hxc_l_tmp_load_result_n162;
  if (hxc_l_tmp_load_result_n162 == NULL)
  {
    abort();
  }
  int32_t hxc_l_tmp_class_field_load_result_n163 = (*hxc_l_tmp_load_result_n162).hxc_enemyId;
  struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation hxc_l_tmp_call_result_n165 = hxc_caxecraft_app_CaxecraftApp_observeActorPhase(hxc_l_tmp_load_result_n161, hxc_l_tmp_class_field_load_result_n163, hxc_caxecraft_domain_ActorControllerPhase_Resting);
  struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation hxc_l_enemyPhase_ha80e5c596f34 = hxc_l_tmp_call_result_n165;
  int32_t hxc_l_tmp_record_field_load_result_n166 = hxc_l_dialogueActor.hxc_id;
  bool hxc_l_tmp_short_circuit_result_n67 = hxc_l_tmp_record_field_load_result_n166 <= 0;
  if (!(hxc_l_tmp_record_field_load_result_n166 <= 0))
  {
    hxc_l_tmp_short_circuit_result_n67 = hxc_l_enemyActor.hxc_id <= 0;
  }
  bool hxc_l_tmp_short_circuit_load_result_n168 = hxc_l_tmp_short_circuit_result_n67;
  bool hxc_l_tmp_short_circuit_result_n68 = hxc_l_tmp_short_circuit_load_result_n168;
  if (!hxc_l_tmp_short_circuit_load_result_n168)
  {
    hxc_l_tmp_short_circuit_result_n68 = !hxc_l_dialoguePhase.hxc_valid;
  }
  bool hxc_l_tmp_short_circuit_load_result_n170 = hxc_l_tmp_short_circuit_result_n68;
  bool hxc_l_tmp_short_circuit_result_n69 = hxc_l_tmp_short_circuit_load_result_n170;
  if (!hxc_l_tmp_short_circuit_load_result_n170)
  {
    hxc_l_tmp_short_circuit_result_n69 = !hxc_l_enemyPhase_ha80e5c596f34.hxc_valid;
  }
  if (hxc_l_tmp_short_circuit_result_n69)
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
  struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n175 = hxc_l_initialSession;
  hxc_l_gc_roots[84] = (const void *)hxc_l_tmp_load_result_n175;
  if (hxc_l_tmp_load_result_n175 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_domain_GameView hxc_l_tmp_instance_call_result_n176 = hxc_caxecraft_domain_GameSession_view(hxc_l_tmp_load_result_n175);
  struct hxc_caxecraft_domain_GameView hxc_l_initialPresentation = hxc_l_tmp_instance_call_result_n176;
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
  struct hxc_caxecraft_app_MotionHistory hxc_l_tmp_call_result_n179 = hxc_caxecraft_app_MotionInterpolation_start(hxc_l_initialPresentation.hxc_localPlayer.hxc_body);
  struct hxc_caxecraft_app_MotionHistory hxc_l_motionHistory = hxc_l_tmp_call_result_n179;
  bool hxc_l_jumpQueued = false;
  bool hxc_l_swordQueued = false;
  int32_t hxc_l_selectedMode = 0;
  int32_t hxc_l_locale = 0;
  bool hxc_l_showInitialTitle = true;
  enum hxc_caxecraft_app_AppScreen hxc_l_tmp_call_result_n181 = hxc_caxecraft_app_AppScreen_initialScreen(hxc_l_showInitialTitle);
  enum hxc_caxecraft_app_AppScreen hxc_l_screen = hxc_l_tmp_call_result_n181;
  bool hxc_l_tmp_call_result_n183 = hxc_caxecraft_app_AppScreen_capturesPointer(hxc_l_screen);
  if (hxc_l_tmp_call_result_n183)
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
    bool hxc_l_tmp_load_result_n184 = hxc_l_quit;
    bool hxc_l_tmp_short_circuit_result_n98 = !hxc_l_tmp_load_result_n184;
    if (!hxc_l_tmp_load_result_n184)
    {
      bool hxc_l_tmp_native_call_result_n185 = WindowShouldClose();
      hxc_l_tmp_short_circuit_result_n98 = !hxc_l_tmp_native_call_result_n185;
    }
    if (!hxc_l_tmp_short_circuit_result_n98)
    {
      break;
    }
    struct hxc_caxecraft_app_ActivePlayableLevel *hxc_l_tmp_load_result_n187 = hxc_l_activeLevel;
    hxc_l_gc_roots[85] = (const void *)hxc_l_tmp_load_result_n187;
    if (hxc_l_tmp_load_result_n187 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_class_field_load_result_n188 = (*hxc_l_tmp_load_result_n187).hxc_activeView;
    hxc_l_gc_roots[86] = (const void *)hxc_l_tmp_class_field_load_result_n188;
    struct hxc_caxecraft_app_PlayableLevelView *hxc_l_levelView = hxc_l_tmp_class_field_load_result_n188;
    struct hxc_caxecraft_app_ActivePlayableLevel *hxc_l_tmp_load_result_n189 = hxc_l_activeLevel;
    hxc_l_gc_roots[87] = (const void *)hxc_l_tmp_load_result_n189;
    if (hxc_l_tmp_load_result_n189 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_content_ActiveContent *hxc_l_tmp_class_field_load_result_n190 = (*hxc_l_tmp_load_result_n189).hxc_content;
    hxc_l_gc_roots[88] = (const void *)hxc_l_tmp_class_field_load_result_n190;
    if (hxc_l_tmp_class_field_load_result_n190 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_content_LoadedContentGeneration *hxc_l_tmp_class_field_load_result_n191 = (*hxc_l_tmp_class_field_load_result_n190).hxc_active;
    hxc_l_gc_roots[89] = (const void *)hxc_l_tmp_class_field_load_result_n191;
    if (hxc_l_tmp_class_field_load_result_n191 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_class_field_load_result_n192 = (*hxc_l_tmp_class_field_load_result_n191).hxc_ownedSession;
    hxc_l_gc_roots[90] = (const void *)hxc_l_tmp_class_field_load_result_n192;
    struct hxc_caxecraft_domain_GameSession *hxc_l_session = hxc_l_tmp_class_field_load_result_n192;
    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n193 = hxc_l_session;
    hxc_l_gc_roots[91] = (const void *)hxc_l_tmp_load_result_n193;
    if (hxc_l_tmp_load_result_n193 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_domain_GameView hxc_l_tmp_instance_call_result_n194 = hxc_caxecraft_domain_GameSession_view(hxc_l_tmp_load_result_n193);
    struct hxc_caxecraft_domain_GameView hxc_l_initialView = hxc_l_tmp_instance_call_result_n194;
    if (!hxc_l_initialView.hxc_valid)
    {
      hxc_l_quit = true;
    }
    struct hxc_caxecraft_domain_Character hxc_l_character = hxc_l_initialView.hxc_localPlayer;
    bool hxc_l_recapturedThisFrame = false;
    bool hxc_l_resetMotionThisFrame = false;
    float hxc_l_tmp_native_call_result_n197 = GetFrameTime();
    double hxc_l_frameSeconds = (double)hxc_l_tmp_native_call_result_n197;
    if (hxc_l_frameSeconds > 0.25)
    {
      hxc_l_frameSeconds = 0.25;
    }
    bool hxc_l_tmp_native_call_result_n199 = IsWindowFocused();
    bool hxc_l_focused = hxc_l_tmp_native_call_result_n199;
    bool hxc_l_tmp_call_result_n201 = hxc_caxecraft_app_AppScreen_capturesPointer(hxc_l_screen);
    bool hxc_l_captured_h891cd0b43970 = hxc_l_tmp_call_result_n201;
    bool hxc_l_tmp_call_result_n203 = hxc_caxecraft_app_AppScreen_pausesSimulation(hxc_l_screen);
    bool hxc_l_paused_hd9b792e32d4e = hxc_l_tmp_call_result_n203;
    double hxc_l_forward = 0.0;
    double hxc_l_right = 0.0;
    bool hxc_l_tmp_native_call_result_n204 = IsKeyDown((int32_t)87);
    if (hxc_l_tmp_native_call_result_n204)
    {
      hxc_l_forward = hxc_l_forward + 1.0;
    }
    bool hxc_l_tmp_native_call_result_n206 = IsKeyDown((int32_t)83);
    if (hxc_l_tmp_native_call_result_n206)
    {
      hxc_l_forward = hxc_l_forward - 1.0;
    }
    bool hxc_l_tmp_native_call_result_n208 = IsKeyDown((int32_t)68);
    if (hxc_l_tmp_native_call_result_n208)
    {
      hxc_l_right = hxc_l_right + 1.0;
    }
    bool hxc_l_tmp_native_call_result_n210 = IsKeyDown((int32_t)65);
    if (hxc_l_tmp_native_call_result_n210)
    {
      hxc_l_right = hxc_l_right - 1.0;
    }
    double hxc_l_lookYaw_h5bdd5686635d = 0.0;
    double hxc_l_lookPitch_ha7ac9f537d74 = 0.0;
    if (hxc_l_captured_h891cd0b43970)
    {
      struct Vector2 hxc_l_tmp_native_call_result_n213 = GetMouseDelta();
      struct Vector2 hxc_l_mouse = hxc_l_tmp_native_call_result_n213;
      hxc_l_lookYaw_h5bdd5686635d = -(double)hxc_l_mouse.x * 0.0025;
      hxc_l_lookPitch_ha7ac9f537d74 = -(double)hxc_l_mouse.y * 0.0025;
    }
    bool hxc_l_tmp_native_call_result_n218 = IsMouseButtonPressed((int32_t)0);
    bool hxc_l_leftPressed = hxc_l_tmp_native_call_result_n218;
    bool hxc_l_tmp_load_result_n219 = hxc_l_captured_h891cd0b43970;
    bool hxc_l_tmp_short_circuit_result_n116 = hxc_l_tmp_load_result_n219;
    if (hxc_l_tmp_load_result_n219)
    {
      hxc_l_tmp_short_circuit_result_n116 = hxc_l_leftPressed;
    }
    bool hxc_l_primaryPressed_h1af73905bdff = hxc_l_tmp_short_circuit_result_n116;
    bool hxc_l_tmp_load_result_n222 = hxc_l_captured_h891cd0b43970;
    bool hxc_l_tmp_short_circuit_result_n118 = hxc_l_tmp_load_result_n222;
    if (hxc_l_tmp_load_result_n222)
    {
      bool hxc_l_tmp_native_call_result_n223 = IsMouseButtonPressed((int32_t)1);
      hxc_l_tmp_short_circuit_result_n118 = hxc_l_tmp_native_call_result_n223;
    }
    bool hxc_l_secondaryPressed_ha8c84c3f2476 = hxc_l_tmp_short_circuit_result_n118;
    bool hxc_l_tmp_load_result_n225 = hxc_l_captured_h891cd0b43970;
    bool hxc_l_tmp_short_circuit_result_n120 = hxc_l_tmp_load_result_n225;
    if (hxc_l_tmp_load_result_n225)
    {
      bool hxc_l_tmp_native_call_result_n226 = IsKeyPressed((int32_t)69);
      hxc_l_tmp_short_circuit_result_n120 = hxc_l_tmp_native_call_result_n226;
    }
    bool hxc_l_interactPressed_h709cc3e1ed4a = hxc_l_tmp_short_circuit_result_n120;
    bool hxc_l_tmp_load_result_n228 = hxc_l_captured_h891cd0b43970;
    bool hxc_l_tmp_short_circuit_result_n122 = hxc_l_tmp_load_result_n228;
    if (hxc_l_tmp_load_result_n228)
    {
      bool hxc_l_tmp_native_call_result_n229 = IsKeyPressed((int32_t)78);
      hxc_l_tmp_short_circuit_result_n122 = hxc_l_tmp_native_call_result_n229;
    }
    bool hxc_l_travelPressed_h8bd5ddb8a405 = hxc_l_tmp_short_circuit_result_n122;
    int32_t hxc_l_hotbarSelection_hb4d44b3bf26a = -1;
    bool hxc_l_tmp_native_call_result_n231 = IsKeyPressed((int32_t)49);
    if (hxc_l_tmp_native_call_result_n231)
    {
      hxc_l_hotbarSelection_hb4d44b3bf26a = 0;
    }
    bool hxc_l_tmp_native_call_result_n232 = IsKeyPressed((int32_t)50);
    if (hxc_l_tmp_native_call_result_n232)
    {
      hxc_l_hotbarSelection_hb4d44b3bf26a = 1;
    }
    bool hxc_l_tmp_native_call_result_n233 = IsKeyPressed((int32_t)51);
    if (hxc_l_tmp_native_call_result_n233)
    {
      hxc_l_hotbarSelection_hb4d44b3bf26a = 2;
    }
    bool hxc_l_tmp_native_call_result_n234 = IsKeyPressed((int32_t)52);
    if (hxc_l_tmp_native_call_result_n234)
    {
      hxc_l_hotbarSelection_hb4d44b3bf26a = 3;
    }
    bool hxc_l_tmp_native_call_result_n235 = IsKeyPressed((int32_t)53);
    if (hxc_l_tmp_native_call_result_n235)
    {
      hxc_l_hotbarSelection_hb4d44b3bf26a = 4;
    }
    bool hxc_l_tmp_native_call_result_n236 = IsKeyPressed((int32_t)54);
    if (hxc_l_tmp_native_call_result_n236)
    {
      hxc_l_hotbarSelection_hb4d44b3bf26a = 5;
    }
    bool hxc_l_tmp_native_call_result_n237 = IsKeyPressed((int32_t)55);
    if (hxc_l_tmp_native_call_result_n237)
    {
      hxc_l_hotbarSelection_hb4d44b3bf26a = 6;
    }
    bool hxc_l_tmp_native_call_result_n238 = IsKeyPressed((int32_t)56);
    if (hxc_l_tmp_native_call_result_n238)
    {
      hxc_l_hotbarSelection_hb4d44b3bf26a = 7;
    }
    float hxc_l_tmp_native_call_result_n239 = GetMouseWheelMove();
    double hxc_l_wheel = (double)hxc_l_tmp_native_call_result_n239;
    int32_t hxc_l_hotbarCycle_ha5fc89c5537e = 0;
    if (hxc_l_wheel > 0.0)
    {
      hxc_l_hotbarCycle_ha5fc89c5537e = -1;
    }
    if (hxc_l_wheel < 0.0)
    {
      hxc_l_hotbarCycle_ha5fc89c5537e = 1;
    }
    bool hxc_l_tmp_native_call_result_n242 = IsKeyPressed((int32_t)32);
    bool hxc_l_jumpPressed_he77b75182b6a = hxc_l_tmp_native_call_result_n242;
    bool hxc_l_tmp_native_call_result_n243 = IsKeyPressed((int32_t)256);
    bool hxc_l_pausePressed_hc02c74ed5d48 = hxc_l_tmp_native_call_result_n243;
    bool hxc_l_tmp_native_call_result_n244 = IsKeyPressed((int32_t)81);
    bool hxc_l_quitPressed_h4a48ed0f0cc2 = hxc_l_tmp_native_call_result_n244;
    bool hxc_l_tmp_native_call_result_n245 = IsKeyDown((int32_t)340);
    bool hxc_l_descendHeld_hd0168cfe3edc = hxc_l_tmp_native_call_result_n245;
    bool hxc_l_tmp_native_call_result_n246 = IsKeyPressed((int32_t)265);
    bool hxc_l_tmp_short_circuit_result_n131 = hxc_l_tmp_native_call_result_n246;
    if (!hxc_l_tmp_native_call_result_n246)
    {
      bool hxc_l_tmp_native_call_result_n247 = IsKeyPressed((int32_t)264);
      hxc_l_tmp_short_circuit_result_n131 = hxc_l_tmp_native_call_result_n247;
    }
    bool hxc_l_menuNextPressed_hf7c2e9ba9e0d = hxc_l_tmp_short_circuit_result_n131;
    bool hxc_l_tmp_native_call_result_n249 = IsKeyPressed((int32_t)257);
    bool hxc_l_menuConfirmPressed_hed3d6ffbd7c7 = hxc_l_tmp_native_call_result_n249;
    double hxc_l_frameInput_moveForward = hxc_l_forward;
    double hxc_l_frameInput_moveRight = hxc_l_right;
    double hxc_l_frameInput_lookYaw = hxc_l_lookYaw_h5bdd5686635d;
    double hxc_l_frameInput_lookPitch = hxc_l_lookPitch_ha7ac9f537d74;
    bool hxc_l_frameInput_jumpPressed = hxc_l_jumpPressed_he77b75182b6a;
    bool hxc_l_tmp_native_call_result_n255 = IsKeyDown((int32_t)32);
    bool hxc_l_frameInput_riseHeld = hxc_l_tmp_native_call_result_n255;
    bool hxc_l_frameInput_descendHeld = hxc_l_descendHeld_hd0168cfe3edc;
    bool hxc_l_frameInput_primaryPressed = hxc_l_primaryPressed_h1af73905bdff;
    bool hxc_l_frameInput_secondaryPressed = hxc_l_secondaryPressed_ha8c84c3f2476;
    bool hxc_l_frameInput_interactPressed = hxc_l_interactPressed_h709cc3e1ed4a;
    bool hxc_l_frameInput_travelPressed = hxc_l_travelPressed_h8bd5ddb8a405;
    bool hxc_l_frameInput_menuNextPressed = hxc_l_menuNextPressed_hf7c2e9ba9e0d;
    bool hxc_l_frameInput_menuConfirmPressed = hxc_l_menuConfirmPressed_hed3d6ffbd7c7;
    bool hxc_l_frameInput_pausePressed = hxc_l_pausePressed_hc02c74ed5d48;
    bool hxc_l_tmp_load_result_n264 = hxc_l_paused_hd9b792e32d4e;
    bool hxc_l_tmp_short_circuit_result_n148 = hxc_l_tmp_load_result_n264;
    if (hxc_l_tmp_load_result_n264)
    {
      hxc_l_tmp_short_circuit_result_n148 = hxc_l_leftPressed;
    }
    bool hxc_l_frameInput_capturePressed = hxc_l_tmp_short_circuit_result_n148;
    bool hxc_l_frameInput_quitPressed = hxc_l_quitPressed_h4a48ed0f0cc2;
    int32_t hxc_l_frameInput_hotbarSelection = hxc_l_hotbarSelection_hb4d44b3bf26a;
    int32_t hxc_l_frameInput_hotbarCycle = hxc_l_hotbarCycle_ha5fc89c5537e;
    double hxc_l_moveForward = hxc_l_frameInput_moveForward;
    double hxc_l_moveRight = hxc_l_frameInput_moveRight;
    double hxc_l_lookYaw_hb85067cf1c5a = hxc_l_frameInput_lookYaw;
    double hxc_l_lookPitch_h7ef5aa1a8cda = hxc_l_frameInput_lookPitch;
    bool hxc_l_jumpPressed_hc40fd78ea0d6 = hxc_l_frameInput_jumpPressed;
    bool hxc_l_riseHeld = hxc_l_frameInput_riseHeld;
    bool hxc_l_primaryPressed_h4a0a8b20cf08 = hxc_l_frameInput_primaryPressed;
    bool hxc_l_secondaryPressed_h3c635e3846cc = hxc_l_frameInput_secondaryPressed;
    bool hxc_l_interactPressed_h550f33a83547 = hxc_l_frameInput_interactPressed;
    bool hxc_l_travelPressed_h11a77c091cdf = hxc_l_frameInput_travelPressed;
    bool hxc_l_pausePressed_h952cc9de954a = hxc_l_frameInput_pausePressed;
    bool hxc_l_capturePressed = hxc_l_frameInput_capturePressed;
    bool hxc_l_quitPressed_hee357f1e842f = hxc_l_frameInput_quitPressed;
    int32_t hxc_l_hotbarSelection_h4d15508de09d = hxc_l_frameInput_hotbarSelection;
    int32_t hxc_l_hotbarCycle_he98d5700e593 = hxc_l_frameInput_hotbarCycle;
    bool hxc_l_menuNextPressed_h4b7fca18ecc3 = hxc_l_frameInput_menuNextPressed;
    bool hxc_l_menuConfirmPressed_h30f1449eeac0 = hxc_l_frameInput_menuConfirmPressed;
    bool hxc_l_descendHeld_h962d79adb546 = hxc_l_frameInput_descendHeld;
    hxc_l_gc_roots[92] = (const void *)NULL;
    struct hxc_caxecraft_content_CampaignLevel *hxc_l_requestedCampaignLevel = NULL;
    bool hxc_l_requestedFromCampaignMenu = false;
    bool hxc_l_tmp_load_result_n288 = hxc_l_quit;
    bool hxc_l_tmp_short_circuit_result_n172 = !hxc_l_tmp_load_result_n288;
    if (!hxc_l_tmp_load_result_n288)
    {
      enum hxc_caxecraft_app_AppScreen hxc_l_tmp_load_result_n289 = hxc_l_screen;
      bool hxc_l_tmp_call_result_n291 = hxc_caxecraft_app_TitleMenuFlow_allowsCampaignTravel(hxc_l_tmp_load_result_n289, hxc_l_selectedMode);
      hxc_l_tmp_short_circuit_result_n172 = hxc_l_tmp_call_result_n291;
    }
    bool hxc_l_tmp_short_circuit_load_result_n292 = hxc_l_tmp_short_circuit_result_n172;
    bool hxc_l_tmp_short_circuit_result_n173 = hxc_l_tmp_short_circuit_load_result_n292;
    if (hxc_l_tmp_short_circuit_load_result_n292)
    {
      hxc_l_tmp_short_circuit_result_n173 = hxc_l_travelPressed_h11a77c091cdf;
    }
    if (hxc_l_tmp_short_circuit_result_n173)
    {
      hxc_l_gc_roots[93] = (const void *)hxc_l_campaign;
      struct hxc_caxecraft_content_CampaignManifest *hxc_l_selectedCampaign_h28bf179e07a3 = hxc_l_campaign;
      hxc_l_gc_roots[94] = (const void *)hxc_l_campaignLevel;
      struct hxc_caxecraft_content_CampaignLevel *hxc_l_sourceLevel = hxc_l_campaignLevel;
      struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n297 = hxc_l_selectedCampaign_h28bf179e07a3;
      hxc_l_gc_roots[95] = (const void *)hxc_l_tmp_load_result_n297;
      hxc_l_gc_roots[96] = (const void *)NULL;
      bool hxc_l_tmp_short_circuit_result_n176 = hxc_l_tmp_load_result_n297 != NULL;
      if (hxc_l_tmp_load_result_n297 != NULL)
      {
        hxc_l_gc_roots[97] = (const void *)hxc_l_sourceLevel;
        hxc_l_gc_roots[98] = (const void *)NULL;
        hxc_l_tmp_short_circuit_result_n176 = hxc_l_sourceLevel != NULL;
      }
      if (hxc_l_tmp_short_circuit_result_n176)
      {
        struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n300 = hxc_l_selectedCampaign_h28bf179e07a3;
        hxc_l_gc_roots[99] = (const void *)hxc_l_tmp_load_result_n300;
        if (hxc_l_tmp_load_result_n300 == NULL)
        {
          abort();
        }
        struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_load_result_n301 = hxc_l_sourceLevel;
        hxc_l_gc_roots[100] = (const void *)hxc_l_tmp_load_result_n301;
        if (hxc_l_tmp_load_result_n301 == NULL)
        {
          abort();
        }
        hxc_string hxc_l_tmp_class_field_load_result_n302 = (*hxc_l_tmp_load_result_n301).hxc_id;
        struct hxc_caxecraft_content_CampaignTransition *hxc_l_tmp_instance_call_result_n303 = hxc_caxecraft_content_CampaignManifest_unambiguousTransitionFrom(hxc_l_tmp_load_result_n300, hxc_l_tmp_class_field_load_result_n302);
        hxc_l_gc_roots[101] = (const void *)hxc_l_tmp_instance_call_result_n303;
        struct hxc_caxecraft_content_CampaignTransition *hxc_l_transition = hxc_l_tmp_instance_call_result_n303;
        hxc_l_gc_roots[102] = (const void *)hxc_l_transition;
        hxc_l_gc_roots[103] = (const void *)NULL;
        if (hxc_l_transition == NULL)
        {
          struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_load_result_n305 = hxc_l_sourceLevel;
          hxc_l_gc_roots[104] = (const void *)hxc_l_tmp_load_result_n305;
          if (hxc_l_tmp_load_result_n305 == NULL)
          {
            abort();
          }
          hxc_string hxc_l_tmp_class_field_load_result_n306 = (*hxc_l_tmp_load_result_n305).hxc_id;
          hxc_string hxc_l_tmp_string_concat_result_n307 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
          if (hxc_string_concat_ref((hxc_string){ (const uint8_t *)"caxecraft: campaign travel requires exactly one outgoing transition from ", 73, true, NULL }, hxc_l_tmp_class_field_load_result_n306, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n307) != HXC_STATUS_OK)
          {
            abort();
          }
          hxc_l_tmp_sys_println_string_argument_owner_n178 = hxc_l_tmp_string_concat_result_n307;
          if (hxc_io_println(hxc_l_tmp_sys_println_string_argument_owner_n178) != HXC_STATUS_OK)
          {
            if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n178) != HXC_STATUS_OK)
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
          if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n178) != HXC_STATUS_OK)
          {
            abort();
          }
        }
        else
        {
          struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n309 = hxc_l_selectedCampaign_h28bf179e07a3;
          hxc_l_gc_roots[105] = (const void *)hxc_l_tmp_load_result_n309;
          if (hxc_l_tmp_load_result_n309 == NULL)
          {
            abort();
          }
          struct hxc_caxecraft_content_CampaignTransition *hxc_l_tmp_load_result_n310 = hxc_l_transition;
          hxc_l_gc_roots[106] = (const void *)hxc_l_tmp_load_result_n310;
          if (hxc_l_tmp_load_result_n310 == NULL)
          {
            abort();
          }
          hxc_string hxc_l_tmp_class_field_load_result_n311 = (*hxc_l_tmp_load_result_n310).hxc_destinationLevel;
          struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_instance_call_result_n312 = hxc_caxecraft_content_CampaignManifest_level(hxc_l_tmp_load_result_n309, hxc_l_tmp_class_field_load_result_n311);
          hxc_l_gc_roots[107] = (const void *)hxc_l_tmp_instance_call_result_n312;
          struct hxc_caxecraft_content_CampaignLevel *hxc_l_destination_ha5debc4bb7cb = hxc_l_tmp_instance_call_result_n312;
          hxc_l_gc_roots[108] = (const void *)hxc_l_destination_ha5debc4bb7cb;
          hxc_l_gc_roots[109] = (const void *)NULL;
          if (hxc_l_destination_ha5debc4bb7cb == NULL)
          {
            if (hxc_io_println((hxc_string){ (const uint8_t *)"caxecraft: campaign destination disappeared after manifest validation", 69, true, NULL }) != HXC_STATUS_OK)
            {
              abort();
            }
          }
          else
          {
            hxc_l_gc_roots[110] = (const void *)hxc_l_destination_ha5debc4bb7cb;
            hxc_l_requestedCampaignLevel = hxc_l_destination_ha5debc4bb7cb;
          }
        }
      }
    }
    bool hxc_l_tmp_call_result_n316 = hxc_caxecraft_app_AppScreen_showsCampaignSelection(hxc_l_screen);
    bool hxc_l_tmp_short_circuit_result_n180 = hxc_l_tmp_call_result_n316;
    if (hxc_l_tmp_call_result_n316)
    {
      hxc_l_tmp_short_circuit_result_n180 = hxc_l_focused;
    }
    if (hxc_l_tmp_short_circuit_result_n180)
    {
      bool hxc_l_campaignLaunchRequested = hxc_l_menuConfirmPressed_h30f1449eeac0;
      bool hxc_l_campaignBackRequested = hxc_l_pausePressed_h952cc9de954a;
      bool hxc_l_tmp_load_result_n321 = hxc_l_menuNextPressed_h4b7fca18ecc3;
      bool hxc_l_tmp_short_circuit_result_n183 = hxc_l_tmp_load_result_n321;
      if (hxc_l_tmp_load_result_n321)
      {
        hxc_l_gc_roots[111] = (const void *)hxc_l_campaign;
        hxc_l_gc_roots[112] = (const void *)NULL;
        hxc_l_tmp_short_circuit_result_n183 = hxc_l_campaign != NULL;
      }
      bool hxc_l_tmp_short_circuit_load_result_n323 = hxc_l_tmp_short_circuit_result_n183;
      bool hxc_l_tmp_short_circuit_result_n184 = hxc_l_tmp_short_circuit_load_result_n323;
      if (hxc_l_tmp_short_circuit_load_result_n323)
      {
        struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n324 = hxc_l_campaign;
        hxc_l_gc_roots[113] = (const void *)hxc_l_tmp_load_result_n324;
        if (hxc_l_tmp_load_result_n324 == NULL)
        {
          abort();
        }
        struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n325 = (*hxc_l_tmp_load_result_n324).hxc_admittedLevels;
        hxc_l_gc_roots[114] = (const void *)hxc_l_tmp_class_field_load_result_n325;
        int32_t hxc_l_tmp_array_length_result_n326;
        if (hxc_array_ref_length(hxc_l_tmp_class_field_load_result_n325, &hxc_l_tmp_array_length_result_n326) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_l_tmp_short_circuit_result_n184 = hxc_l_tmp_array_length_result_n326 > 0;
      }
      if (hxc_l_tmp_short_circuit_result_n184)
      {
        int32_t hxc_l_tmp_load_result_n328 = hxc_l_selectedCampaignLevelIndex;
        struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n329 = hxc_l_campaign;
        hxc_l_gc_roots[115] = (const void *)hxc_l_tmp_load_result_n329;
        if (hxc_l_tmp_load_result_n329 == NULL)
        {
          abort();
        }
        struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n330 = (*hxc_l_tmp_load_result_n329).hxc_admittedLevels;
        hxc_l_gc_roots[116] = (const void *)hxc_l_tmp_class_field_load_result_n330;
        int32_t hxc_l_tmp_array_length_result_n331;
        if (hxc_array_ref_length(hxc_l_tmp_class_field_load_result_n330, &hxc_l_tmp_array_length_result_n331) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_l_selectedCampaignLevelIndex = hxc_i32_modulo_zero_safe(hxc_i32_add_wrapping(hxc_l_tmp_load_result_n328, 1), hxc_l_tmp_array_length_result_n331);
      }
      bool hxc_l_tmp_native_call_result_n332 = IsKeyPressed((int32_t)76);
      if (hxc_l_tmp_native_call_result_n332)
      {
        struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_tmp_load_result_n333 = hxc_l_uiCatalog;
        hxc_l_gc_roots[117] = (const void *)hxc_l_tmp_load_result_n333;
        if (hxc_l_tmp_load_result_n333 == NULL)
        {
          abort();
        }
        int32_t hxc_l_tmp_instance_call_result_n335 = hxc_caxecraft_localization_RuntimeUiCatalog_nextLocale(hxc_l_tmp_load_result_n333, hxc_l_locale);
        hxc_l_locale = hxc_l_tmp_instance_call_result_n335;
      }
      struct Vector2 hxc_l_tmp_native_call_result_n336 = GetMousePosition();
      struct Vector2 hxc_l_campaignMouse = hxc_l_tmp_native_call_result_n336;
      double hxc_l_tmp_static_call_argument_0_n187 = (double)hxc_l_campaignMouse.x;
      double hxc_l_tmp_static_call_argument_1_n188 = (double)hxc_l_campaignMouse.y;
      int32_t hxc_l_tmp_native_call_result_n341 = GetScreenWidth();
      int32_t hxc_l_tmp_static_call_argument_2_n189 = (int32_t)hxc_l_tmp_native_call_result_n341;
      int32_t hxc_l_tmp_native_call_result_n342 = GetScreenHeight();
      int32_t hxc_l_tmp_static_call_argument_3_n190 = (int32_t)hxc_l_tmp_native_call_result_n342;
      struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n343 = hxc_l_campaign;
      hxc_l_gc_roots[118] = (const void *)hxc_l_tmp_load_result_n343;
      hxc_l_gc_roots[119] = (const void *)NULL;
      int32_t hxc_l_tmp_conditional_result_n191 = 0;
      if (hxc_l_tmp_load_result_n343 == NULL)
      {
        hxc_l_tmp_conditional_result_n191 = 0;
      }
      else
      {
        struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n344 = hxc_l_campaign;
        hxc_l_gc_roots[120] = (const void *)hxc_l_tmp_load_result_n344;
        if (hxc_l_tmp_load_result_n344 == NULL)
        {
          abort();
        }
        struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n345 = (*hxc_l_tmp_load_result_n344).hxc_admittedLevels;
        hxc_l_gc_roots[121] = (const void *)hxc_l_tmp_class_field_load_result_n345;
        int32_t hxc_l_tmp_array_length_result_n346;
        if (hxc_array_ref_length(hxc_l_tmp_class_field_load_result_n345, &hxc_l_tmp_array_length_result_n346) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_l_tmp_conditional_result_n191 = hxc_l_tmp_array_length_result_n346;
      }
      int32_t hxc_l_tmp_conditional_load_result_n347 = hxc_l_tmp_conditional_result_n191;
      double hxc_l_tmp_static_call_argument_0_load_result_n348 = hxc_l_tmp_static_call_argument_0_n187;
      double hxc_l_tmp_static_call_argument_1_load_result_n349 = hxc_l_tmp_static_call_argument_1_n188;
      int32_t hxc_l_tmp_static_call_argument_2_load_result_n350 = hxc_l_tmp_static_call_argument_2_n189;
      struct hxc_caxecraft_app_CampaignMenuHit hxc_l_tmp_call_result_n352 = hxc_caxecraft_app_CampaignMenu_selectionAt(hxc_l_tmp_static_call_argument_0_load_result_n348, hxc_l_tmp_static_call_argument_1_load_result_n349, hxc_l_tmp_static_call_argument_2_load_result_n350, hxc_l_tmp_static_call_argument_3_n190, hxc_l_tmp_conditional_load_result_n347);
      struct hxc_caxecraft_app_CampaignMenuHit hxc_l_campaignChoice = hxc_l_tmp_call_result_n352;
      bool hxc_l_tmp_native_call_result_n353 = IsMouseButtonPressed((int32_t)0);
      if (hxc_l_tmp_native_call_result_n353)
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
              int32_t hxc_l_tmp_enum_payload_project_n356 = hxc_l_campaignChoice.hxc_payload.hxc_LevelHit.hxc_index;
              int32_t hxc_l_index_h0f8b3659eb87 = hxc_l_tmp_enum_payload_project_n356;
              hxc_l_selectedCampaignLevelIndex = hxc_l_index_h0f8b3659eb87;
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
        enum hxc_caxecraft_app_AppScreen hxc_l_tmp_call_result_n360 = hxc_caxecraft_app_AppScreen_closeCampaignSelection(hxc_l_screen);
        hxc_l_screen = hxc_l_tmp_call_result_n360;
        hxc_l_accumulator = 0.0;
        hxc_l_resetMotionThisFrame = true;
      }
      else
      {
        struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n361 = hxc_l_campaign;
        hxc_l_gc_roots[122] = (const void *)hxc_l_tmp_load_result_n361;
        hxc_l_gc_roots[123] = (const void *)NULL;
        bool hxc_l_tmp_short_circuit_result_n193 = hxc_l_tmp_load_result_n361 != NULL;
        if (hxc_l_tmp_load_result_n361 != NULL)
        {
          hxc_l_tmp_short_circuit_result_n193 = hxc_l_campaignLaunchRequested;
        }
        if (hxc_l_tmp_short_circuit_result_n193)
        {
          struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n364 = hxc_l_campaign;
          hxc_l_gc_roots[124] = (const void *)hxc_l_tmp_load_result_n364;
          if (hxc_l_tmp_load_result_n364 == NULL)
          {
            abort();
          }
          struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n365 = (*hxc_l_tmp_load_result_n364).hxc_admittedLevels;
          hxc_l_gc_roots[125] = (const void *)hxc_l_tmp_class_field_load_result_n365;
          struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_array_get_result_n367;
          if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n365, (size_t)hxc_l_selectedCampaignLevelIndex, &hxc_l_tmp_array_get_result_n367) != HXC_STATUS_OK)
          {
            abort();
          }
          hxc_l_gc_roots[126] = (const void *)hxc_l_tmp_array_get_result_n367;
          hxc_l_requestedCampaignLevel = hxc_l_tmp_array_get_result_n367;
          hxc_l_requestedFromCampaignMenu = true;
        }
      }
    }
    bool hxc_l_tmp_call_result_n369 = hxc_caxecraft_app_AppScreen_showsTitle(hxc_l_screen);
    bool hxc_l_tmp_short_circuit_result_n194 = hxc_l_tmp_call_result_n369;
    if (hxc_l_tmp_call_result_n369)
    {
      hxc_l_tmp_short_circuit_result_n194 = hxc_l_focused;
    }
    if (hxc_l_tmp_short_circuit_result_n194)
    {
      enum hxc_caxecraft_app_AppScreen hxc_l_screenBeforeMenu = hxc_l_screen;
      int32_t hxc_l_modeBeforeMenu = hxc_l_selectedMode;
      enum hxc_caxecraft_app_AppScreen hxc_l_tmp_load_result_n374 = hxc_l_screen;
      struct hxc_caxecraft_app_TitleMenuState hxc_l_tmp_call_result_n376 = hxc_caxecraft_app_TitleMenuFlow_titleMenuState(hxc_l_tmp_load_result_n374, hxc_l_selectedMode);
      struct hxc_caxecraft_app_TitleMenuState hxc_l_menuState = hxc_l_tmp_call_result_n376;
      bool hxc_l_tmp_native_call_result_n377 = IsKeyPressed((int32_t)76);
      if (hxc_l_tmp_native_call_result_n377)
      {
        struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_tmp_load_result_n378 = hxc_l_uiCatalog;
        hxc_l_gc_roots[130] = (const void *)hxc_l_tmp_load_result_n378;
        if (hxc_l_tmp_load_result_n378 == NULL)
        {
          abort();
        }
        int32_t hxc_l_tmp_instance_call_result_n380 = hxc_caxecraft_localization_RuntimeUiCatalog_nextLocale(hxc_l_tmp_load_result_n378, hxc_l_locale);
        hxc_l_locale = hxc_l_tmp_instance_call_result_n380;
      }
      if (hxc_l_menuNextPressed_h4b7fca18ecc3)
      {
        struct hxc_caxecraft_app_TitleMenuState hxc_l_tmp_call_result_n383 = hxc_caxecraft_app_TitleMenuFlow_applyTitleMenuCommand(hxc_l_menuState, 1);
        hxc_l_menuState = hxc_l_tmp_call_result_n383;
      }
      struct Vector2 hxc_l_tmp_native_call_result_n384 = GetMousePosition();
      struct Vector2 hxc_l_menuMouse = hxc_l_tmp_native_call_result_n384;
      struct Vector2 hxc_l_tmp_load_result_n385 = hxc_l_menuMouse;
      struct Vector2 hxc_l_tmp_load_result_n387 = hxc_l_menuMouse;
      int32_t hxc_l_tmp_native_call_result_n389 = GetScreenWidth();
      int32_t hxc_l_tmp_native_call_result_n390 = GetScreenHeight();
      int32_t hxc_l_tmp_call_result_n391 = hxc_caxecraft_app_TitleMenu_selectionAt((double)hxc_l_tmp_load_result_n385.x, (double)hxc_l_tmp_load_result_n387.y, (int32_t)hxc_l_tmp_native_call_result_n389, (int32_t)hxc_l_tmp_native_call_result_n390);
      int32_t hxc_l_hovered = hxc_l_tmp_call_result_n391;
      if (hxc_l_hovered == 0)
      {
        struct hxc_caxecraft_app_TitleMenuState hxc_l_tmp_call_result_n394 = hxc_caxecraft_app_TitleMenuFlow_applyTitleMenuCommand(hxc_l_menuState, 2);
        hxc_l_menuState = hxc_l_tmp_call_result_n394;
      }
      if (hxc_l_hovered == 1)
      {
        struct hxc_caxecraft_app_TitleMenuState hxc_l_tmp_call_result_n397 = hxc_caxecraft_app_TitleMenuFlow_applyTitleMenuCommand(hxc_l_menuState, 3);
        hxc_l_menuState = hxc_l_tmp_call_result_n397;
      }
      int32_t hxc_l_tmp_load_result_n398 = hxc_l_hovered;
      bool hxc_l_tmp_short_circuit_result_n201 = hxc_l_tmp_load_result_n398 >= 0;
      if (hxc_l_tmp_load_result_n398 >= 0)
      {
        bool hxc_l_tmp_native_call_result_n399 = IsMouseButtonPressed((int32_t)0);
        hxc_l_tmp_short_circuit_result_n201 = hxc_l_tmp_native_call_result_n399;
      }
      bool hxc_l_clickedChoice = hxc_l_tmp_short_circuit_result_n201;
      if (hxc_l_clickedChoice)
      {
        struct hxc_caxecraft_app_TitleMenuState hxc_l_tmp_static_call_argument_0_n202 = hxc_l_menuState;
        int32_t hxc_l_tmp_load_result_n403 = hxc_l_hovered;
        int32_t hxc_l_tmp_conditional_result_n203 = 0;
        if (hxc_l_tmp_load_result_n403 == 2)
        {
          hxc_l_tmp_conditional_result_n203 = 5;
        }
        else
        {
          hxc_l_tmp_conditional_result_n203 = 4;
        }
        int32_t hxc_l_tmp_conditional_load_result_n404 = hxc_l_tmp_conditional_result_n203;
        struct hxc_caxecraft_app_TitleMenuState hxc_l_tmp_call_result_n406 = hxc_caxecraft_app_TitleMenuFlow_applyTitleMenuCommand(hxc_l_tmp_static_call_argument_0_n202, hxc_l_tmp_conditional_load_result_n404);
        hxc_l_menuState = hxc_l_tmp_call_result_n406;
      }
      else
      {
        if (hxc_l_menuConfirmPressed_h30f1449eeac0)
        {
          struct hxc_caxecraft_app_TitleMenuState hxc_l_tmp_call_result_n409 = hxc_caxecraft_app_TitleMenuFlow_applyTitleMenuCommand(hxc_l_menuState, 4);
          hxc_l_menuState = hxc_l_tmp_call_result_n409;
        }
      }
      hxc_l_screen = hxc_l_menuState.hxc_screen;
      hxc_l_selectedMode = hxc_l_menuState.hxc_mode;
      bool hxc_l_tmp_call_result_n413 = hxc_caxecraft_app_AppScreen_showsCampaignSelection(hxc_l_screen);
      bool hxc_l_tmp_short_circuit_result_n204 = hxc_l_tmp_call_result_n413;
      if (hxc_l_tmp_call_result_n413)
      {
        hxc_l_gc_roots[131] = (const void *)hxc_l_campaign;
        hxc_l_gc_roots[132] = (const void *)NULL;
        hxc_l_tmp_short_circuit_result_n204 = hxc_l_campaign == NULL;
      }
      if (hxc_l_tmp_short_circuit_result_n204)
      {
        hxc_l_screen = hxc_l_screenBeforeMenu;
      }
      enum hxc_caxecraft_app_AppScreen hxc_l_tmp_load_result_n417 = hxc_l_screen;
      enum hxc_caxecraft_app_AppScreen hxc_l_tmp_load_result_n418 = hxc_l_screenBeforeMenu;
      bool hxc_l_tmp_short_circuit_result_n205 = hxc_l_tmp_load_result_n417 != hxc_l_tmp_load_result_n418;
      if (!(hxc_l_tmp_load_result_n417 != hxc_l_tmp_load_result_n418))
      {
        int32_t hxc_l_tmp_load_result_n419 = hxc_l_selectedMode;
        hxc_l_tmp_short_circuit_result_n205 = hxc_l_tmp_load_result_n419 != hxc_l_modeBeforeMenu;
      }
      if (hxc_l_tmp_short_circuit_result_n205)
      {
        hxc_l_accumulator = 0.0;
        hxc_l_resetMotionThisFrame = true;
      }
      enum hxc_caxecraft_app_AppScreen hxc_l_tmp_load_result_n422 = hxc_l_screenBeforeMenu;
      bool hxc_l_tmp_short_circuit_result_n206 = hxc_l_tmp_load_result_n422 == hxc_caxecraft_app_AppScreen_Title;
      if (hxc_l_tmp_load_result_n422 == hxc_caxecraft_app_AppScreen_Title)
      {
        bool hxc_l_tmp_call_result_n425 = hxc_caxecraft_app_AppScreen_isPlaying(hxc_l_screen);
        hxc_l_tmp_short_circuit_result_n206 = hxc_l_tmp_call_result_n425;
      }
      if (hxc_l_tmp_short_circuit_result_n206)
      {
        hxc_l_recapturedThisFrame = true;
        DisableCursor();
      }
      else
      {
        bool hxc_l_tmp_call_result_n428 = hxc_caxecraft_app_AppScreen_showsEditor(hxc_l_screen);
        if (hxc_l_tmp_call_result_n428)
        {
          EnableCursor();
        }
      }
    }
    hxc_l_gc_roots[127] = (const void *)hxc_l_requestedCampaignLevel;
    struct hxc_caxecraft_content_CampaignLevel *hxc_l_destination_hc14d4a882c25 = hxc_l_requestedCampaignLevel;
    hxc_l_gc_roots[128] = (const void *)hxc_l_destination_hc14d4a882c25;
    hxc_l_gc_roots[129] = (const void *)NULL;
    if (hxc_l_destination_hc14d4a882c25 != NULL)
    {
      int32_t hxc_l_tmp_record_field_load_result_n431 = hxc_l_character.hxc_id;
      int32_t hxc_l_tmp_record_field_load_result_n432 = hxc_l_character.hxc_vitals.hxc_health;
      struct hxc_caxecraft_content_LevelPlayerOptions hxc_l_playerOptions = (struct hxc_caxecraft_content_LevelPlayerOptions){ .hxc_aquaticProfile = hxc_l_character.hxc_aquaticProfile, .hxc_entityId = hxc_l_tmp_record_field_load_result_n431, .hxc_initialHealth = hxc_l_tmp_record_field_load_result_n432 };
      struct hxc_caxecraft_app_ActivePlayableLevel *hxc_l_tmp_load_result_n435 = hxc_l_activeLevel;
      hxc_l_gc_roots[133] = (const void *)hxc_l_tmp_load_result_n435;
      if (hxc_l_tmp_load_result_n435 == NULL)
      {
        abort();
      }
      struct hxc_caxecraft_content_ActiveContent *hxc_l_tmp_class_field_load_result_n436 = (*hxc_l_tmp_load_result_n435).hxc_content;
      hxc_l_gc_roots[134] = (const void *)hxc_l_tmp_class_field_load_result_n436;
      if (hxc_l_tmp_class_field_load_result_n436 == NULL)
      {
        abort();
      }
      struct hxc_caxecraft_content_LoadedContentGeneration *hxc_l_tmp_class_field_load_result_n437 = (*hxc_l_tmp_class_field_load_result_n436).hxc_active;
      hxc_l_gc_roots[135] = (const void *)hxc_l_tmp_class_field_load_result_n437;
      if (hxc_l_tmp_class_field_load_result_n437 == NULL)
      {
        abort();
      }
      int32_t hxc_l_tmp_class_field_load_result_n438 = (*hxc_l_tmp_class_field_load_result_n437).hxc_generationIdValue;
      int32_t hxc_l_nextGeneration = hxc_i32_add_wrapping(hxc_l_tmp_class_field_load_result_n438, 1);
      struct hxc_caxecraft_content_ContentPackageStore *hxc_l_tmp_load_result_n439 = hxc_l_contentStore;
      hxc_l_gc_roots[136] = (const void *)hxc_l_tmp_load_result_n439;
      struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_load_result_n440 = hxc_l_destination_hc14d4a882c25;
      hxc_l_gc_roots[137] = (const void *)hxc_l_tmp_load_result_n440;
      int32_t hxc_l_tmp_load_result_n441 = hxc_l_nextGeneration;
      struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_tmp_load_result_n442 = hxc_l_contentRegistry;
      hxc_l_gc_roots[138] = (const void *)hxc_l_tmp_load_result_n442;
      if (hxc_l_tmp_load_result_n442 == NULL)
      {
        abort();
      }
      struct hxc_compiler_interface_dispatch_caxecraft_scenario_ScenarioContentRegistry_value hxc_l_tmp_interface_value_n443 = (struct hxc_compiler_interface_dispatch_caxecraft_scenario_ScenarioContentRegistry_value){ .object = hxc_l_tmp_load_result_n442, .table = &hxc_itable_compiler_interface_dispatch_caxecraft_content_RuntimeContentRegistry_itable_layout_caxecraf_hb3f384641e1a61bf };
      struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_tmp_load_result_n444 = hxc_l_contentRegistry;
      hxc_l_gc_roots[139] = (const void *)hxc_l_tmp_load_result_n444;
      if (hxc_l_tmp_load_result_n444 == NULL)
      {
        abort();
      }
      struct hxc_compiler_interface_dispatch_caxecraft_content_LevelContentResolver_value hxc_l_tmp_interface_value_n445 = (struct hxc_compiler_interface_dispatch_caxecraft_content_LevelContentResolver_value){ .object = hxc_l_tmp_load_result_n444, .table = &hxc_itable_compiler_interface_dispatch_caxecraft_content_RuntimeContentRegistry_itable_layout_caxecraf_h82ca4f24ca7a7b5a };
      struct hxc_caxecraft_content_CampaignLevelLoadResult hxc_l_tmp_call_result_n447 = hxc_caxecraft_content_CampaignRuntime_loadCampaignLevel(hxc_l_tmp_load_result_n439, hxc_l_tmp_load_result_n440, hxc_l_tmp_load_result_n441, hxc_l_tmp_interface_value_n443, hxc_l_tmp_interface_value_n445, hxc_l_playerOptions);
      hxc_l_gc_roots[140] = hxc_l_tmp_call_result_n447.hxc_tag == hxc_caxecraft_content_CampaignLevelLoadResult_CampaignLevelReady ? (const void *)hxc_l_tmp_call_result_n447.hxc_payload.hxc_CampaignLevelReady.hxc_candidate : NULL;
      hxc_l_g_hbd7f92d003e5 = hxc_l_tmp_call_result_n447;
      hxc_l_gc_roots[141] = hxc_l_g_hbd7f92d003e5.hxc_tag == hxc_caxecraft_content_CampaignLevelLoadResult_CampaignLevelReady ? (const void *)hxc_l_g_hbd7f92d003e5.hxc_payload.hxc_CampaignLevelReady.hxc_candidate : NULL;
      switch (hxc_l_g_hbd7f92d003e5.hxc_tag) {
        case hxc_caxecraft_content_CampaignLevelLoadResult_CampaignLevelReady:
          {
            hxc_l_gc_roots[145] = hxc_l_g_hbd7f92d003e5.hxc_tag == hxc_caxecraft_content_CampaignLevelLoadResult_CampaignLevelReady ? (const void *)hxc_l_g_hbd7f92d003e5.hxc_payload.hxc_CampaignLevelReady.hxc_candidate : NULL;
            if (hxc_l_g_hbd7f92d003e5.hxc_tag != hxc_caxecraft_content_CampaignLevelLoadResult_CampaignLevelReady)
            {
              abort();
            }
            struct hxc_caxecraft_content_RuntimeLevelCandidate *hxc_l_tmp_enum_payload_project_n450 = hxc_l_g_hbd7f92d003e5.hxc_payload.hxc_CampaignLevelReady.hxc_candidate;
            hxc_l_gc_roots[146] = (const void *)hxc_l_tmp_enum_payload_project_n450;
            struct hxc_caxecraft_content_RuntimeLevelCandidate *hxc_l_candidate_ha950d4aaf402 = hxc_l_tmp_enum_payload_project_n450;
            struct hxc_caxecraft_app_ActivePlayableLevel *hxc_l_tmp_load_result_n451 = hxc_l_activeLevel;
            hxc_l_gc_roots[147] = (const void *)hxc_l_tmp_load_result_n451;
            if (hxc_l_tmp_load_result_n451 == NULL)
            {
              abort();
            }
            hxc_l_gc_roots[148] = (const void *)hxc_l_candidate_ha950d4aaf402;
            struct hxc_caxecraft_app_PlayableLevelPublicationResult hxc_l_tmp_instance_call_result_n453 = hxc_caxecraft_app_ActivePlayableLevel_publish(hxc_l_tmp_load_result_n451, hxc_l_candidate_ha950d4aaf402);
            struct hxc_caxecraft_app_PlayableLevelPublicationResult hxc_l_g_h01c092e1f7b6 = hxc_l_tmp_instance_call_result_n453;
            switch (hxc_l_g_h01c092e1f7b6.hxc_tag) {
              case hxc_caxecraft_app_PlayableLevelPublicationResult_PlayableLevelPublished:
                {
                  if (hxc_l_g_h01c092e1f7b6.hxc_tag != hxc_caxecraft_app_PlayableLevelPublicationResult_PlayableLevelPublished)
                  {
                    abort();
                  }
                  int32_t hxc_l_tmp_enum_payload_project_n456 = hxc_l_g_h01c092e1f7b6.hxc_payload.hxc_PlayableLevelPublished.hxc_active;
                  int32_t hxc_l_selected = hxc_l_tmp_enum_payload_project_n456;
                  hxc_l_gc_roots[150] = (const void *)hxc_l_destination_hc14d4a882c25;
                  hxc_l_campaignLevel = hxc_l_destination_hc14d4a882c25;
                  struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_load_result_n458 = hxc_l_destination_hc14d4a882c25;
                  hxc_l_gc_roots[151] = (const void *)hxc_l_tmp_load_result_n458;
                  if (hxc_l_tmp_load_result_n458 == NULL)
                  {
                    abort();
                  }
                  hxc_string hxc_l_tmp_class_field_load_result_n459 = (*hxc_l_tmp_load_result_n458).hxc_id;
                  hxc_string hxc_l_tmp_string_assignment_replacement_owner_n214 = hxc_l_tmp_class_field_load_result_n459;
                  if (hxc_string_retain(hxc_l_tmp_string_assignment_replacement_owner_n214) != HXC_STATUS_OK)
                  {
                    abort();
                  }
                  hxc_string hxc_l_tmp_string_assignment_replacement_owned_load_result_n460 = hxc_l_tmp_string_assignment_replacement_owner_n214;
                  if (hxc_string_release(&hxc_l_levelLabel) != HXC_STATUS_OK)
                  {
                    abort();
                  }
                  hxc_l_levelLabel = hxc_l_tmp_string_assignment_replacement_owned_load_result_n460;
                  hxc_l_gc_roots[152] = (const void *)hxc_l_campaign;
                  struct hxc_caxecraft_content_CampaignManifest *hxc_l_selectedCampaign_h6414e5cfe6b9 = hxc_l_campaign;
                  hxc_l_gc_roots[153] = (const void *)hxc_l_selectedCampaign_h6414e5cfe6b9;
                  hxc_l_gc_roots[154] = (const void *)NULL;
                  if (hxc_l_selectedCampaign_h6414e5cfe6b9 != NULL)
                  {
                    int32_t hxc_l_g_h9321c79ec6b3 = 0;
                    struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n463 = hxc_l_selectedCampaign_h6414e5cfe6b9;
                    hxc_l_gc_roots[155] = (const void *)hxc_l_tmp_load_result_n463;
                    if (hxc_l_tmp_load_result_n463 == NULL)
                    {
                      abort();
                    }
                    struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n464 = (*hxc_l_tmp_load_result_n463).hxc_admittedLevels;
                    hxc_l_gc_roots[156] = (const void *)hxc_l_tmp_class_field_load_result_n464;
                    int32_t hxc_l_tmp_array_length_result_n465;
                    if (hxc_array_ref_length(hxc_l_tmp_class_field_load_result_n464, &hxc_l_tmp_array_length_result_n465) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    int32_t hxc_l_g1_hfe047ded78fa = hxc_l_tmp_array_length_result_n465;
                    while (1)
                    {
                      int32_t hxc_l_tmp_load_result_n466 = hxc_l_g_h9321c79ec6b3;
                      if (!(hxc_l_tmp_load_result_n466 < hxc_l_g1_hfe047ded78fa))
                      {
                        break;
                      }
                      int32_t hxc_l_tmp_increment_load_result_n468 = hxc_l_g_h9321c79ec6b3;
                      hxc_l_g_h9321c79ec6b3 = hxc_i32_add_wrapping(hxc_l_tmp_increment_load_result_n468, 1);
                      int32_t hxc_l_index_hded83cefec22 = hxc_l_tmp_increment_load_result_n468;
                      struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n469 = hxc_l_selectedCampaign_h6414e5cfe6b9;
                      hxc_l_gc_roots[164] = (const void *)hxc_l_tmp_load_result_n469;
                      if (hxc_l_tmp_load_result_n469 == NULL)
                      {
                        abort();
                      }
                      struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n470 = (*hxc_l_tmp_load_result_n469).hxc_admittedLevels;
                      hxc_l_gc_roots[165] = (const void *)hxc_l_tmp_class_field_load_result_n470;
                      struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_array_get_result_n472;
                      if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n470, (size_t)hxc_l_index_hded83cefec22, &hxc_l_tmp_array_get_result_n472) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      hxc_l_gc_roots[166] = (const void *)hxc_l_tmp_array_get_result_n472;
                      if (hxc_l_tmp_array_get_result_n472 == NULL)
                      {
                        abort();
                      }
                      hxc_string hxc_l_tmp_class_field_load_result_n473 = (*hxc_l_tmp_array_get_result_n472).hxc_id;
                      struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_load_result_n474 = hxc_l_destination_hc14d4a882c25;
                      hxc_l_gc_roots[167] = (const void *)hxc_l_tmp_load_result_n474;
                      if (hxc_l_tmp_load_result_n474 == NULL)
                      {
                        abort();
                      }
                      hxc_string hxc_l_tmp_class_field_load_result_n475 = (*hxc_l_tmp_load_result_n474).hxc_id;
                      if (hxc_l_tmp_class_field_load_result_n473.data == NULL || hxc_l_tmp_class_field_load_result_n475.data == NULL ? hxc_l_tmp_class_field_load_result_n473.data == hxc_l_tmp_class_field_load_result_n475.data : hxc_l_tmp_class_field_load_result_n473.byte_length == hxc_l_tmp_class_field_load_result_n475.byte_length && (hxc_l_tmp_class_field_load_result_n473.byte_length == 0 || memcmp(hxc_l_tmp_class_field_load_result_n473.data, hxc_l_tmp_class_field_load_result_n475.data, hxc_l_tmp_class_field_load_result_n473.byte_length) == 0))
                      {
                        hxc_l_selectedCampaignLevelIndex = hxc_l_index_hded83cefec22;
                      }
                    }
                  }
                  struct hxc_caxecraft_app_ActivePlayableLevel *hxc_l_tmp_load_result_n478 = hxc_l_activeLevel;
                  hxc_l_gc_roots[157] = (const void *)hxc_l_tmp_load_result_n478;
                  if (hxc_l_tmp_load_result_n478 == NULL)
                  {
                    abort();
                  }
                  struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_class_field_load_result_n479 = (*hxc_l_tmp_load_result_n478).hxc_activeView;
                  hxc_l_gc_roots[158] = (const void *)hxc_l_tmp_class_field_load_result_n479;
                  hxc_l_levelView = hxc_l_tmp_class_field_load_result_n479;
                  struct hxc_caxecraft_app_ActivePlayableLevel *hxc_l_tmp_load_result_n480 = hxc_l_activeLevel;
                  hxc_l_gc_roots[159] = (const void *)hxc_l_tmp_load_result_n480;
                  if (hxc_l_tmp_load_result_n480 == NULL)
                  {
                    abort();
                  }
                  struct hxc_caxecraft_content_ActiveContent *hxc_l_tmp_class_field_load_result_n481 = (*hxc_l_tmp_load_result_n480).hxc_content;
                  hxc_l_gc_roots[160] = (const void *)hxc_l_tmp_class_field_load_result_n481;
                  if (hxc_l_tmp_class_field_load_result_n481 == NULL)
                  {
                    abort();
                  }
                  struct hxc_caxecraft_content_LoadedContentGeneration *hxc_l_tmp_class_field_load_result_n482 = (*hxc_l_tmp_class_field_load_result_n481).hxc_active;
                  hxc_l_gc_roots[161] = (const void *)hxc_l_tmp_class_field_load_result_n482;
                  if (hxc_l_tmp_class_field_load_result_n482 == NULL)
                  {
                    abort();
                  }
                  struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_class_field_load_result_n483 = (*hxc_l_tmp_class_field_load_result_n482).hxc_ownedSession;
                  hxc_l_gc_roots[162] = (const void *)hxc_l_tmp_class_field_load_result_n483;
                  hxc_l_session = hxc_l_tmp_class_field_load_result_n483;
                  struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n484 = hxc_l_session;
                  hxc_l_gc_roots[163] = (const void *)hxc_l_tmp_load_result_n484;
                  if (hxc_l_tmp_load_result_n484 == NULL)
                  {
                    abort();
                  }
                  struct hxc_caxecraft_domain_GameView hxc_l_tmp_instance_call_result_n485 = hxc_caxecraft_domain_GameSession_view(hxc_l_tmp_load_result_n484);
                  hxc_l_initialView = hxc_l_tmp_instance_call_result_n485;
                  if (!hxc_l_initialView.hxc_valid)
                  {
                    hxc_l_quit = true;
                  }
                  else
                  {
                    hxc_l_character = hxc_l_initialView.hxc_localPlayer;
                    hxc_l_guidePhase = hxc_caxecraft_gameplay_GuidePhase_Waiting;
                    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n489 = hxc_l_session;
                    hxc_l_gc_roots[168] = (const void *)hxc_l_tmp_load_result_n489;
                    if (hxc_l_tmp_load_result_n489 == NULL)
                    {
                      abort();
                    }
                    struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n490 = hxc_l_levelView;
                    hxc_l_gc_roots[169] = (const void *)hxc_l_tmp_load_result_n490;
                    if (hxc_l_tmp_load_result_n490 == NULL)
                    {
                      abort();
                    }
                    int32_t hxc_l_tmp_class_field_load_result_n491 = (*hxc_l_tmp_load_result_n490).hxc_dialogueId;
                    bool hxc_l_tmp_instance_call_result_n492 = hxc_caxecraft_domain_GameSession_actorInteractionAvailable(hxc_l_tmp_load_result_n489, hxc_l_tmp_class_field_load_result_n491);
                    hxc_l_guideInteractionAvailable = hxc_l_tmp_instance_call_result_n492;
                    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n493 = hxc_l_session;
                    hxc_l_gc_roots[170] = (const void *)hxc_l_tmp_load_result_n493;
                    if (hxc_l_tmp_load_result_n493 == NULL)
                    {
                      abort();
                    }
                    struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n494 = hxc_l_levelView;
                    hxc_l_gc_roots[171] = (const void *)hxc_l_tmp_load_result_n494;
                    if (hxc_l_tmp_load_result_n494 == NULL)
                    {
                      abort();
                    }
                    int32_t hxc_l_tmp_class_field_load_result_n495 = (*hxc_l_tmp_load_result_n494).hxc_dialogueId;
                    struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n496 = hxc_caxecraft_domain_GameSession_readCharacter(hxc_l_tmp_load_result_n493, hxc_l_tmp_class_field_load_result_n495);
                    hxc_l_dialogueActor = hxc_l_tmp_instance_call_result_n496;
                    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n497 = hxc_l_session;
                    hxc_l_gc_roots[172] = (const void *)hxc_l_tmp_load_result_n497;
                    if (hxc_l_tmp_load_result_n497 == NULL)
                    {
                      abort();
                    }
                    struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n498 = hxc_l_levelView;
                    hxc_l_gc_roots[173] = (const void *)hxc_l_tmp_load_result_n498;
                    if (hxc_l_tmp_load_result_n498 == NULL)
                    {
                      abort();
                    }
                    int32_t hxc_l_tmp_class_field_load_result_n499 = (*hxc_l_tmp_load_result_n498).hxc_enemyId;
                    struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n500 = hxc_caxecraft_domain_GameSession_readCharacter(hxc_l_tmp_load_result_n497, hxc_l_tmp_class_field_load_result_n499);
                    hxc_l_enemyActor = hxc_l_tmp_instance_call_result_n500;
                    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n501 = hxc_l_session;
                    hxc_l_gc_roots[174] = (const void *)hxc_l_tmp_load_result_n501;
                    if (hxc_l_tmp_load_result_n501 == NULL)
                    {
                      abort();
                    }
                    struct hxc_array_ref *hxc_l_tmp_instance_call_result_n502 = hxc_caxecraft_domain_GameSession_actorControllerStateSnapshots(hxc_l_tmp_load_result_n501);
                    hxc_l_phases = hxc_l_tmp_instance_call_result_n502;
                    struct hxc_array_ref *hxc_l_tmp_load_result_n503 = hxc_l_phases;
                    struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n504 = hxc_l_levelView;
                    hxc_l_gc_roots[175] = (const void *)hxc_l_tmp_load_result_n504;
                    if (hxc_l_tmp_load_result_n504 == NULL)
                    {
                      abort();
                    }
                    int32_t hxc_l_tmp_class_field_load_result_n505 = (*hxc_l_tmp_load_result_n504).hxc_dialogueId;
                    struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation hxc_l_tmp_call_result_n507 = hxc_caxecraft_app_CaxecraftApp_observeActorPhase(hxc_l_tmp_load_result_n503, hxc_l_tmp_class_field_load_result_n505, hxc_caxecraft_domain_ActorControllerPhase_Stationary);
                    hxc_l_dialoguePhase = hxc_l_tmp_call_result_n507;
                    struct hxc_array_ref *hxc_l_tmp_load_result_n508 = hxc_l_phases;
                    struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n509 = hxc_l_levelView;
                    hxc_l_gc_roots[176] = (const void *)hxc_l_tmp_load_result_n509;
                    if (hxc_l_tmp_load_result_n509 == NULL)
                    {
                      abort();
                    }
                    int32_t hxc_l_tmp_class_field_load_result_n510 = (*hxc_l_tmp_load_result_n509).hxc_enemyId;
                    struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation hxc_l_tmp_call_result_n512 = hxc_caxecraft_app_CaxecraftApp_observeActorPhase(hxc_l_tmp_load_result_n508, hxc_l_tmp_class_field_load_result_n510, hxc_caxecraft_domain_ActorControllerPhase_Resting);
                    hxc_l_enemyPhase_ha80e5c596f34 = hxc_l_tmp_call_result_n512;
                    hxc_l_swordCombat = (struct hxc_caxecraft_gameplay_SwordCombatState){ .hxc_cooldownTicks = 0 };
                    hxc_l_berryDrop = (struct hxc_caxecraft_gameplay_BerryDropState){ .hxc_active = false, .hxc_amount = 0, .hxc_x = 0.0, .hxc_y = 0.0, .hxc_z = 0.0 };
                    hxc_l_cameraWaterBlend = 0.0;
                    hxc_l_accumulator = 0.0;
                    struct hxc_caxecraft_app_MotionHistory hxc_l_tmp_call_result_n516 = hxc_caxecraft_app_MotionInterpolation_start(hxc_l_character.hxc_body);
                    hxc_l_motionHistory = hxc_l_tmp_call_result_n516;
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
                    struct hxc_caxecraft_app_TerrainRenderer *hxc_l_tmp_owned_class_field_address_n517 = &(*hxc_l_self).hxc_terrainRenderer;
                    struct hxc_caxecraft_app_TerrainChunkCache *hxc_l_tmp_owned_class_field_address_n518 = &(*hxc_l_tmp_owned_class_field_address_n517).hxc_cache;
                    if (hxc_l_tmp_owned_class_field_address_n518 == NULL)
                    {
                      abort();
                    }
                    hxc_caxecraft_app_TerrainChunkCache_invalidateAll(hxc_l_tmp_owned_class_field_address_n518);
                    hxc_l_resetMotionThisFrame = true;
                    if (hxc_l_requestedFromCampaignMenu)
                    {
                      enum hxc_caxecraft_app_AppScreen hxc_l_tmp_call_result_n521 = hxc_caxecraft_app_AppScreen_startSelectedCampaign(hxc_l_screen);
                      hxc_l_screen = hxc_l_tmp_call_result_n521;
                      hxc_l_recapturedThisFrame = true;
                      DisableCursor();
                    }
                    hxc_string hxc_l_tmp_string_concat_result_n523 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
                    if (hxc_string_concat_ref((hxc_string){ (const uint8_t *)"caxecraft: campaign-level=", 26, true, NULL }, hxc_l_levelLabel, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n523) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    hxc_l_tmp_string_concat_left_owner_n220 = hxc_l_tmp_string_concat_result_n523;
                    hxc_string hxc_l_tmp_string_concat_result_n525 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
                    if (hxc_string_concat_ref(hxc_l_tmp_string_concat_left_owner_n220, (hxc_string){ (const uint8_t *)" generation=", 12, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n525) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    hxc_l_tmp_string_concat_left_owner_n221 = hxc_l_tmp_string_concat_result_n525;
                    hxc_string hxc_l_tmp_string_concat_left_borrow_result_n526 = hxc_l_tmp_string_concat_left_owner_n221;
                    hxc_string hxc_l_tmp_std_string_int_result_n528 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
                    if (hxc_string_from_int32(hxc_l_selected, hxc_default_allocator(), &hxc_l_tmp_std_string_int_result_n528) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    hxc_l_tmp_string_concat_right_owner_n222 = hxc_l_tmp_std_string_int_result_n528;
                    hxc_string hxc_l_tmp_string_concat_result_n530 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
                    if (hxc_string_concat_ref(hxc_l_tmp_string_concat_left_borrow_result_n526, hxc_l_tmp_string_concat_right_owner_n222, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n530) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    hxc_l_tmp_sys_println_string_argument_owner_n223 = hxc_l_tmp_string_concat_result_n530;
                    if (hxc_io_println(hxc_l_tmp_sys_println_string_argument_owner_n223) != HXC_STATUS_OK)
                    {
                      if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n223) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n222) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n221) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n220) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      if (hxc_array_ref_release(hxc_l_phases) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      hxc_enum_5543d34a_destroy(&hxc_l_g_hbd7f92d003e5);
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
                    if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n223) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n222) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n221) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n220) != HXC_STATUS_OK)
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
            hxc_l_gc_roots[149] = hxc_l_g_hbd7f92d003e5.hxc_tag == hxc_caxecraft_content_CampaignLevelLoadResult_CampaignLevelReady ? (const void *)hxc_l_g_hbd7f92d003e5.hxc_payload.hxc_CampaignLevelReady.hxc_candidate : NULL;
            if (hxc_l_g_hbd7f92d003e5.hxc_tag != hxc_caxecraft_content_CampaignLevelLoadResult_CampaignLevelRejected)
            {
              abort();
            }
            struct hxc_caxecraft_content_CampaignLevelLoadError hxc_l_tmp_enum_payload_project_n533 = hxc_l_g_hbd7f92d003e5.hxc_payload.hxc_CampaignLevelRejected.hxc_error;
            hxc_l_error = hxc_l_tmp_enum_payload_project_n533;
            if (hxc_enum_40863044_retain(&hxc_l_error) != HXC_STATUS_OK)
            {
              abort();
            }
            hxc_string hxc_l_tmp_call_result_n535 = hxc_caxecraft_app_CaxecraftApp_campaignLevelLoadFailure(hxc_l_error);
            hxc_l_tmp_string_concat_right_owner_n225 = hxc_l_tmp_call_result_n535;
            hxc_string hxc_l_tmp_string_concat_result_n537 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
            if (hxc_string_concat_ref((hxc_string){ (const uint8_t *)"caxecraft: campaign level rejected: ", 36, true, NULL }, hxc_l_tmp_string_concat_right_owner_n225, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n537) != HXC_STATUS_OK)
            {
              abort();
            }
            hxc_l_tmp_sys_println_string_argument_owner_n226 = hxc_l_tmp_string_concat_result_n537;
            if (hxc_io_println(hxc_l_tmp_sys_println_string_argument_owner_n226) != HXC_STATUS_OK)
            {
              if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n226) != HXC_STATUS_OK)
              {
                abort();
              }
              if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n225) != HXC_STATUS_OK)
              {
                abort();
              }
              hxc_enum_40863044_destroy(&hxc_l_error);
              hxc_enum_5543d34a_destroy(&hxc_l_g_hbd7f92d003e5);
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
            if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n226) != HXC_STATUS_OK)
            {
              abort();
            }
            if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n225) != HXC_STATUS_OK)
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
      hxc_enum_5543d34a_destroy(&hxc_l_g_hbd7f92d003e5);
    }
    struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n539 = hxc_l_levelView;
    hxc_l_gc_roots[142] = (const void *)hxc_l_tmp_load_result_n539;
    if (hxc_l_tmp_load_result_n539 == NULL)
    {
      abort();
    }
    int32_t hxc_l_tmp_class_field_load_result_n540 = (*hxc_l_tmp_load_result_n539).hxc_dialogueId;
    int32_t hxc_l_dialogueActorId = hxc_l_tmp_class_field_load_result_n540;
    struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n541 = hxc_l_levelView;
    hxc_l_gc_roots[143] = (const void *)hxc_l_tmp_load_result_n541;
    if (hxc_l_tmp_load_result_n541 == NULL)
    {
      abort();
    }
    int32_t hxc_l_tmp_class_field_load_result_n542 = (*hxc_l_tmp_load_result_n541).hxc_enemyId;
    int32_t hxc_l_enemyActorId = hxc_l_tmp_class_field_load_result_n542;
    struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n543 = hxc_l_levelView;
    hxc_l_gc_roots[144] = (const void *)hxc_l_tmp_load_result_n543;
    if (hxc_l_tmp_load_result_n543 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_scenario_ScenarioTransform hxc_l_tmp_class_field_load_result_n544 = (*hxc_l_tmp_load_result_n543).hxc_playerSpawn;
    struct hxc_caxecraft_scenario_ScenarioTransform hxc_l_spawnTransform = hxc_l_tmp_class_field_load_result_n544;
    if (hxc_l_quitPressed_hee357f1e842f)
    {
      hxc_l_quit = true;
    }
    if (hxc_l_hotbarSelection_h4d15508de09d >= 0)
    {
      struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_load_result_n547 = hxc_l_inventory;
      struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n549 = hxc_caxecraft_gameplay_Inventory_select(hxc_l_tmp_load_result_n547, hxc_l_hotbarSelection_h4d15508de09d);
      hxc_l_inventory = hxc_l_tmp_call_result_n549;
    }
    if (hxc_l_hotbarCycle_he98d5700e593 != 0)
    {
      struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_load_result_n551 = hxc_l_inventory;
      struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n553 = hxc_caxecraft_gameplay_Inventory_cycle(hxc_l_tmp_load_result_n551, hxc_l_hotbarCycle_he98d5700e593);
      hxc_l_inventory = hxc_l_tmp_call_result_n553;
    }
    bool hxc_l_tmp_call_result_n555 = hxc_caxecraft_app_AppScreen_isPlaying(hxc_l_screen);
    bool hxc_l_tmp_short_circuit_result_n230 = hxc_l_tmp_call_result_n555;
    if (hxc_l_tmp_call_result_n555)
    {
      hxc_l_tmp_short_circuit_result_n230 = hxc_l_interactPressed_h550f33a83547;
    }
    if (hxc_l_tmp_short_circuit_result_n230)
    {
      if (hxc_l_character.hxc_vitals.hxc_health <= 0)
      {
        struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n559 = hxc_l_session;
        hxc_l_gc_roots[177] = (const void *)hxc_l_tmp_load_result_n559;
        if (hxc_l_tmp_load_result_n559 == NULL)
        {
          abort();
        }
        struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n560 = hxc_l_session;
        hxc_l_gc_roots[178] = (const void *)hxc_l_tmp_load_result_n560;
        if (hxc_l_tmp_load_result_n560 == NULL)
        {
          abort();
        }
        size_t hxc_l_tmp_length_n562;
        const uint8_t *hxc_l_tmp_instance_call_result_n561 = hxc_caxecraft_domain_GameSession_worldView(hxc_l_tmp_load_result_n560, &hxc_l_tmp_length_n562);
        struct hxc_caxecraft_domain_CharacterBody hxc_l_tmp_call_result_n564 = hxc_caxecraft_app_CaxecraftApp_spawnPlayer(hxc_l_tmp_instance_call_result_n561, hxc_l_tmp_length_n562, hxc_l_spawnTransform);
        struct hxc_caxecraft_domain_LocalCharacterCommandResult hxc_l_tmp_instance_call_result_n565 = hxc_caxecraft_domain_GameSession_reviveLocalPlayerAt(hxc_l_tmp_load_result_n559, hxc_l_tmp_call_result_n564);
        struct hxc_caxecraft_domain_LocalCharacterCommandResult hxc_l_revival = hxc_l_tmp_instance_call_result_n565;
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
        struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n568 = hxc_l_session;
        hxc_l_gc_roots[179] = (const void *)hxc_l_tmp_load_result_n568;
        if (hxc_l_tmp_load_result_n568 == NULL)
        {
          abort();
        }
        bool hxc_l_tmp_instance_call_result_n570 = hxc_caxecraft_domain_GameSession_actorInteractionAvailable(hxc_l_tmp_load_result_n568, hxc_l_dialogueActorId);
        if (hxc_l_tmp_instance_call_result_n570)
        {
          bool hxc_l_sharesBerries = hxc_l_guidePhase == hxc_caxecraft_gameplay_GuidePhase_Welcomed;
          if (hxc_l_sharesBerries)
          {
            int32_t hxc_l_tmp_call_result_n576 = hxc_caxecraft_gameplay_Inventory_acceptedAmount(hxc_l_inventory, hxc_caxecraft_gameplay_ItemKind_Berries, 2);
            int32_t hxc_l_acceptedGift = hxc_l_tmp_call_result_n576;
            if (hxc_l_acceptedGift == 2)
            {
              struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_load_result_n578 = hxc_l_inventory;
              struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n581 = hxc_caxecraft_gameplay_Inventory_collectItem(hxc_l_tmp_load_result_n578, hxc_caxecraft_gameplay_ItemKind_Berries, hxc_l_acceptedGift);
              hxc_l_inventory = hxc_l_tmp_call_result_n581;
              enum hxc_caxecraft_gameplay_GuidePhase hxc_l_tmp_call_result_n583 = hxc_caxecraft_app_CaxecraftApp_advanceGuidePhase(hxc_l_guidePhase);
              hxc_l_guidePhase = hxc_l_tmp_call_result_n583;
            }
            else
            {
              hxc_l_inventoryFullReason = 1;
              hxc_l_inventoryFullFrames = 90;
            }
          }
          else
          {
            enum hxc_caxecraft_gameplay_GuidePhase hxc_l_tmp_call_result_n585 = hxc_caxecraft_app_CaxecraftApp_advanceGuidePhase(hxc_l_guidePhase);
            hxc_l_guidePhase = hxc_l_tmp_call_result_n585;
          }
        }
      }
    }
    bool hxc_l_tmp_load_result_n586 = hxc_l_focused;
    bool hxc_l_tmp_short_circuit_result_n234 = !hxc_l_tmp_load_result_n586;
    if (!hxc_l_tmp_load_result_n586)
    {
      bool hxc_l_tmp_call_result_n588 = hxc_caxecraft_app_AppScreen_isPlaying(hxc_l_screen);
      hxc_l_tmp_short_circuit_result_n234 = hxc_l_tmp_call_result_n588;
    }
    if (hxc_l_tmp_short_circuit_result_n234)
    {
      enum hxc_caxecraft_app_AppScreen hxc_l_tmp_call_result_n591 = hxc_caxecraft_app_AppScreen_loseFocus(hxc_l_screen);
      hxc_l_screen = hxc_l_tmp_call_result_n591;
      hxc_l_jumpQueued = false;
      hxc_l_accumulator = 0.0;
      hxc_l_resetMotionThisFrame = true;
      EnableCursor();
    }
    bool hxc_l_tmp_call_result_n593 = hxc_caxecraft_app_AppScreen_showsTitle(hxc_l_screen);
    bool hxc_l_tmp_short_circuit_result_n235 = !hxc_l_tmp_call_result_n593;
    if (!hxc_l_tmp_call_result_n593)
    {
      bool hxc_l_tmp_call_result_n595 = hxc_caxecraft_app_AppScreen_showsCampaignSelection(hxc_l_screen);
      hxc_l_tmp_short_circuit_result_n235 = !hxc_l_tmp_call_result_n595;
    }
    bool hxc_l_tmp_short_circuit_load_result_n596 = hxc_l_tmp_short_circuit_result_n235;
    bool hxc_l_tmp_short_circuit_result_n236 = hxc_l_tmp_short_circuit_load_result_n596;
    if (hxc_l_tmp_short_circuit_load_result_n596)
    {
      bool hxc_l_tmp_call_result_n598 = hxc_caxecraft_app_AppScreen_showsEditor(hxc_l_screen);
      hxc_l_tmp_short_circuit_result_n236 = !hxc_l_tmp_call_result_n598;
    }
    bool hxc_l_tmp_short_circuit_load_result_n599 = hxc_l_tmp_short_circuit_result_n236;
    bool hxc_l_tmp_short_circuit_result_n237 = hxc_l_tmp_short_circuit_load_result_n599;
    if (hxc_l_tmp_short_circuit_load_result_n599)
    {
      hxc_l_tmp_short_circuit_result_n237 = hxc_l_focused;
    }
    bool hxc_l_tmp_short_circuit_load_result_n601 = hxc_l_tmp_short_circuit_result_n237;
    bool hxc_l_tmp_short_circuit_result_n238 = hxc_l_tmp_short_circuit_load_result_n601;
    if (hxc_l_tmp_short_circuit_load_result_n601)
    {
      hxc_l_tmp_short_circuit_result_n238 = hxc_l_pausePressed_h952cc9de954a;
    }
    if (hxc_l_tmp_short_circuit_result_n238)
    {
      enum hxc_caxecraft_app_AppScreen hxc_l_tmp_call_result_n605 = hxc_caxecraft_app_AppScreen_togglePause(hxc_l_screen);
      hxc_l_screen = hxc_l_tmp_call_result_n605;
      hxc_l_accumulator = 0.0;
      hxc_l_resetMotionThisFrame = true;
      bool hxc_l_tmp_call_result_n607 = hxc_caxecraft_app_AppScreen_pausesSimulation(hxc_l_screen);
      if (hxc_l_tmp_call_result_n607)
      {
        hxc_l_jumpQueued = false;
      }
      bool hxc_l_tmp_call_result_n609 = hxc_caxecraft_app_AppScreen_capturesPointer(hxc_l_screen);
      if (hxc_l_tmp_call_result_n609)
      {
        DisableCursor();
      }
      else
      {
        EnableCursor();
      }
    }
    bool hxc_l_tmp_call_result_n611 = hxc_caxecraft_app_AppScreen_showsTitle(hxc_l_screen);
    bool hxc_l_tmp_short_circuit_result_n239 = !hxc_l_tmp_call_result_n611;
    if (!hxc_l_tmp_call_result_n611)
    {
      bool hxc_l_tmp_call_result_n613 = hxc_caxecraft_app_AppScreen_showsCampaignSelection(hxc_l_screen);
      hxc_l_tmp_short_circuit_result_n239 = !hxc_l_tmp_call_result_n613;
    }
    bool hxc_l_tmp_short_circuit_load_result_n614 = hxc_l_tmp_short_circuit_result_n239;
    bool hxc_l_tmp_short_circuit_result_n240 = hxc_l_tmp_short_circuit_load_result_n614;
    if (hxc_l_tmp_short_circuit_load_result_n614)
    {
      bool hxc_l_tmp_call_result_n616 = hxc_caxecraft_app_AppScreen_showsEditor(hxc_l_screen);
      hxc_l_tmp_short_circuit_result_n240 = !hxc_l_tmp_call_result_n616;
    }
    bool hxc_l_tmp_short_circuit_load_result_n617 = hxc_l_tmp_short_circuit_result_n240;
    bool hxc_l_tmp_short_circuit_result_n241 = hxc_l_tmp_short_circuit_load_result_n617;
    if (hxc_l_tmp_short_circuit_load_result_n617)
    {
      hxc_l_tmp_short_circuit_result_n241 = hxc_l_focused;
    }
    bool hxc_l_tmp_short_circuit_load_result_n619 = hxc_l_tmp_short_circuit_result_n241;
    bool hxc_l_tmp_short_circuit_result_n242 = hxc_l_tmp_short_circuit_load_result_n619;
    if (hxc_l_tmp_short_circuit_load_result_n619)
    {
      bool hxc_l_tmp_call_result_n621 = hxc_caxecraft_app_AppScreen_pausesSimulation(hxc_l_screen);
      hxc_l_tmp_short_circuit_result_n242 = hxc_l_tmp_call_result_n621;
    }
    bool hxc_l_tmp_short_circuit_load_result_n622 = hxc_l_tmp_short_circuit_result_n242;
    bool hxc_l_tmp_short_circuit_result_n243 = hxc_l_tmp_short_circuit_load_result_n622;
    if (hxc_l_tmp_short_circuit_load_result_n622)
    {
      hxc_l_tmp_short_circuit_result_n243 = hxc_l_capturePressed;
    }
    if (hxc_l_tmp_short_circuit_result_n243)
    {
      enum hxc_caxecraft_app_AppScreen hxc_l_tmp_call_result_n626 = hxc_caxecraft_app_AppScreen_recapture(hxc_l_screen);
      hxc_l_screen = hxc_l_tmp_call_result_n626;
      hxc_l_recapturedThisFrame = true;
      hxc_l_accumulator = 0.0;
      hxc_l_resetMotionThisFrame = true;
      DisableCursor();
    }
    bool hxc_l_tmp_call_result_n628 = hxc_caxecraft_app_AppScreen_showsTitle(hxc_l_screen);
    bool hxc_l_onTitle = hxc_l_tmp_call_result_n628;
    bool hxc_l_tmp_call_result_n630 = hxc_caxecraft_app_AppScreen_showsCampaignSelection(hxc_l_screen);
    bool hxc_l_onCampaignSelect = hxc_l_tmp_call_result_n630;
    bool hxc_l_tmp_call_result_n632 = hxc_caxecraft_app_AppScreen_showsEditor(hxc_l_screen);
    bool hxc_l_onEditor = hxc_l_tmp_call_result_n632;
    bool hxc_l_tmp_call_result_n634 = hxc_caxecraft_app_AppScreen_pausesSimulation(hxc_l_screen);
    bool hxc_l_paused_h9ad6cd97e1b1 = hxc_l_tmp_call_result_n634;
    bool hxc_l_tmp_call_result_n636 = hxc_caxecraft_app_AppScreen_capturesPointer(hxc_l_screen);
    bool hxc_l_captured_h4e575de23f09 = hxc_l_tmp_call_result_n636;
    int32_t hxc_l_editorNavigationCommand = 0;
    if (hxc_l_onEditor)
    {
      if (hxc_l_self == NULL)
      {
        abort();
      }
      struct hxc_caxecraft_input_NavigationRepeater *hxc_l_tmp_owned_class_field_address_n638 = &(*hxc_l_self).hxc_editorNavigation;
      if (hxc_l_tmp_owned_class_field_address_n638 == NULL)
      {
        abort();
      }
      struct hxc_caxecraft_input_NavigationSample hxc_l_tmp_call_result_n639 = hxc_caxecraft_app_RaylibNavigationInput_samplePrimaryGamepad();
      int32_t hxc_l_tmp_instance_call_result_n641 = hxc_caxecraft_input_NavigationRepeater_advance(hxc_l_tmp_owned_class_field_address_n638, hxc_l_tmp_call_result_n639, hxc_l_frameSeconds);
      hxc_l_editorNavigationCommand = hxc_l_tmp_instance_call_result_n641;
    }
    else
    {
      if (hxc_l_self == NULL)
      {
        abort();
      }
      struct hxc_caxecraft_input_NavigationRepeater *hxc_l_tmp_owned_class_field_address_n642 = &(*hxc_l_self).hxc_editorNavigation;
      if (hxc_l_tmp_owned_class_field_address_n642 == NULL)
      {
        abort();
      }
      hxc_caxecraft_input_NavigationRepeater_release(hxc_l_tmp_owned_class_field_address_n642);
    }
    if (hxc_l_captured_h4e575de23f09)
    {
      double hxc_l_yawDelta = hxc_l_lookYaw_hb85067cf1c5a;
      if (hxc_l_yawDelta > 0.25)
      {
        hxc_l_yawDelta = 0.25;
      }
      if (hxc_l_yawDelta < -0.25)
      {
        hxc_l_yawDelta = -0.25;
      }
      double hxc_l_tmp_load_result_n647 = hxc_l_lookX;
      double hxc_l_tmp_load_result_n648 = hxc_l_yawDelta;
      double hxc_l_candidateX = hxc_l_tmp_load_result_n647 + hxc_l_tmp_load_result_n648 * hxc_l_lookZ;
      double hxc_l_tmp_load_result_n650 = hxc_l_lookZ;
      double hxc_l_tmp_load_result_n651 = hxc_l_yawDelta;
      double hxc_l_candidateZ = hxc_l_tmp_load_result_n650 - hxc_l_tmp_load_result_n651 * hxc_l_lookX;
      double hxc_l_tmp_load_result_n653 = hxc_l_candidateX;
      double hxc_l_tmp_load_result_n654 = hxc_l_candidateX;
      double hxc_l_tmp_load_result_n655 = hxc_l_candidateZ;
      double hxc_l_lengthSquared = hxc_l_tmp_load_result_n653 * hxc_l_tmp_load_result_n654 + hxc_l_tmp_load_result_n655 * hxc_l_candidateZ;
      double hxc_l_normalize = 1.5 - 0.5 * hxc_l_lengthSquared;
      double hxc_l_tmp_load_result_n658 = hxc_l_candidateX;
      hxc_l_lookX = hxc_l_tmp_load_result_n658 * hxc_l_normalize;
      double hxc_l_tmp_load_result_n660 = hxc_l_candidateZ;
      hxc_l_lookZ = hxc_l_tmp_load_result_n660 * hxc_l_normalize;
      double hxc_l_tmp_compound_load_result_n662 = hxc_l_lookY;
      hxc_l_lookY = hxc_l_tmp_compound_load_result_n662 + hxc_l_lookPitch_h7ef5aa1a8cda;
      if (hxc_l_lookY > 0.90)
      {
        hxc_l_lookY = 0.90;
      }
      if (hxc_l_lookY < -0.90)
      {
        hxc_l_lookY = -0.90;
      }
      if (hxc_l_jumpPressed_hc40fd78ea0d6)
      {
        hxc_l_jumpQueued = true;
      }
    }
    bool hxc_l_tmp_load_result_n667 = hxc_l_captured_h4e575de23f09;
    bool hxc_l_tmp_short_circuit_result_n255 = hxc_l_tmp_load_result_n667;
    if (hxc_l_tmp_load_result_n667)
    {
      hxc_l_tmp_short_circuit_result_n255 = !hxc_l_recapturedThisFrame;
    }
    bool hxc_l_tmp_short_circuit_load_result_n669 = hxc_l_tmp_short_circuit_result_n255;
    bool hxc_l_tmp_short_circuit_result_n256 = hxc_l_tmp_short_circuit_load_result_n669;
    if (hxc_l_tmp_short_circuit_load_result_n669)
    {
      hxc_l_tmp_short_circuit_result_n256 = hxc_l_primaryPressed_h4a0a8b20cf08;
    }
    bool hxc_l_tmp_short_circuit_load_result_n671 = hxc_l_tmp_short_circuit_result_n256;
    bool hxc_l_tmp_short_circuit_result_n257 = hxc_l_tmp_short_circuit_load_result_n671;
    if (hxc_l_tmp_short_circuit_load_result_n671)
    {
      hxc_l_tmp_short_circuit_result_n257 = hxc_l_selectedMode == 1;
    }
    bool hxc_l_tmp_short_circuit_load_result_n673 = hxc_l_tmp_short_circuit_result_n257;
    bool hxc_l_tmp_short_circuit_result_n258 = hxc_l_tmp_short_circuit_load_result_n673;
    if (hxc_l_tmp_short_circuit_load_result_n673)
    {
      bool hxc_l_tmp_call_result_n676 = hxc_caxecraft_gameplay_Inventory_selectedIs(hxc_l_inventory, hxc_caxecraft_gameplay_ItemKind_CopperSword);
      hxc_l_tmp_short_circuit_result_n258 = hxc_l_tmp_call_result_n676;
    }
    if (hxc_l_tmp_short_circuit_result_n258)
    {
      hxc_l_swordQueued = true;
    }
    if (!hxc_l_paused_h9ad6cd97e1b1)
    {
      double hxc_l_tmp_compound_load_result_n679 = hxc_l_accumulator;
      hxc_l_accumulator = hxc_l_tmp_compound_load_result_n679 + hxc_l_frameSeconds;
    }
    while (1)
    {
      bool hxc_l_tmp_load_result_n681 = hxc_l_paused_h9ad6cd97e1b1;
      bool hxc_l_tmp_short_circuit_result_n259 = !hxc_l_tmp_load_result_n681;
      if (!hxc_l_tmp_load_result_n681)
      {
        hxc_l_tmp_short_circuit_result_n259 = hxc_l_accumulator >= 0.05;
      }
      if (!hxc_l_tmp_short_circuit_result_n259)
      {
        break;
      }
      double hxc_l_tmp_load_result_n684 = hxc_l_moveForward;
      double hxc_l_tmp_load_result_n685 = hxc_l_lookX;
      double hxc_l_tmp_load_result_n686 = hxc_l_moveRight;
      double hxc_l_moveX = hxc_l_tmp_load_result_n684 * hxc_l_tmp_load_result_n685 - hxc_l_tmp_load_result_n686 * hxc_l_lookZ;
      double hxc_l_tmp_load_result_n688 = hxc_l_moveForward;
      double hxc_l_tmp_load_result_n689 = hxc_l_lookZ;
      double hxc_l_tmp_load_result_n690 = hxc_l_moveRight;
      double hxc_l_moveZ = hxc_l_tmp_load_result_n688 * hxc_l_tmp_load_result_n689 + hxc_l_tmp_load_result_n690 * hxc_l_lookX;
      double hxc_l_tmp_load_result_n692 = hxc_l_moveForward;
      bool hxc_l_tmp_short_circuit_result_n262 = hxc_l_tmp_load_result_n692 != 0.0;
      if (hxc_l_tmp_load_result_n692 != 0.0)
      {
        hxc_l_tmp_short_circuit_result_n262 = hxc_l_moveRight != 0.0;
      }
      if (hxc_l_tmp_short_circuit_result_n262)
      {
        hxc_l_moveX = hxc_l_moveX * 0.7071067811865476;
        hxc_l_moveZ = hxc_l_moveZ * 0.7071067811865476;
      }
      int32_t hxc_l_tmp_load_result_n697 = hxc_l_selectedMode;
      int32_t hxc_l_tmp_conditional_result_n264 = 0;
      if (hxc_l_tmp_load_result_n697 == 1)
      {
        hxc_l_tmp_conditional_result_n264 = 1;
      }
      else
      {
        hxc_l_tmp_conditional_result_n264 = 0;
      }
      int32_t hxc_l_damagePolicy = hxc_l_tmp_conditional_result_n264;
      bool hxc_l_tmp_load_result_n699 = hxc_l_jumpQueued;
      bool hxc_l_tmp_short_circuit_result_n266 = hxc_l_tmp_load_result_n699;
      if (!hxc_l_tmp_load_result_n699)
      {
        int32_t hxc_l_tmp_record_field_load_result_n700 = hxc_l_character.hxc_aquatic.hxc_medium;
        bool hxc_l_tmp_short_circuit_result_n267 = hxc_l_tmp_record_field_load_result_n700 != 0;
        if (hxc_l_tmp_record_field_load_result_n700 != 0)
        {
          hxc_l_tmp_short_circuit_result_n267 = hxc_l_riseHeld;
        }
        hxc_l_tmp_short_circuit_result_n266 = hxc_l_tmp_short_circuit_result_n267;
      }
      bool hxc_l_rising = hxc_l_tmp_short_circuit_result_n266;
      struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n704 = hxc_l_session;
      hxc_l_gc_roots[181] = (const void *)hxc_l_tmp_load_result_n704;
      if (hxc_l_tmp_load_result_n704 == NULL)
      {
        abort();
      }
      double hxc_l_tmp_load_result_n705 = hxc_l_moveX;
      double hxc_l_tmp_load_result_n706 = hxc_l_moveZ;
      bool hxc_l_tmp_load_result_n707 = hxc_l_rising;
      struct hxc_caxecraft_domain_CharacterIntent hxc_l_tmp_call_result_n709 = hxc_caxecraft_domain_Aquatics_input(hxc_l_tmp_load_result_n705, hxc_l_tmp_load_result_n706, hxc_l_tmp_load_result_n707, hxc_l_descendHeld_h962d79adb546);
      struct hxc_caxecraft_domain_GameTickResult hxc_l_tmp_instance_call_result_n712 = hxc_caxecraft_domain_GameSession_tick(hxc_l_tmp_load_result_n704, (struct hxc_caxecraft_domain_GameTickInput){ .hxc_damagePolicy = hxc_l_damagePolicy, .hxc_intent = hxc_l_tmp_call_result_n709, .hxc_waterUpdateBudget = 64 });
      struct hxc_caxecraft_domain_GameTickResult hxc_l_gameTick = hxc_l_tmp_instance_call_result_n712;
      hxc_l_character = hxc_l_gameTick.hxc_character;
      if (hxc_l_gameTick.hxc_committed)
      {
        struct hxc_caxecraft_app_MotionHistory hxc_l_tmp_load_result_n715 = hxc_l_motionHistory;
        struct hxc_caxecraft_app_MotionHistory hxc_l_tmp_call_result_n717 = hxc_caxecraft_app_MotionInterpolation_advance(hxc_l_tmp_load_result_n715, hxc_l_character.hxc_body);
        hxc_l_motionHistory = hxc_l_tmp_call_result_n717;
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
          int32_t hxc_l_tmp_load_result_n721 = hxc_l_pickupIndex;
          struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n722 = hxc_l_levelView;
          hxc_l_gc_roots[182] = (const void *)hxc_l_tmp_load_result_n722;
          if (hxc_l_tmp_load_result_n722 == NULL)
          {
            abort();
          }
          struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n723 = (*hxc_l_tmp_load_result_n722).hxc_items;
          int32_t hxc_l_tmp_array_length_result_n724;
          if (hxc_array_ref_length(hxc_l_tmp_class_field_load_result_n723, &hxc_l_tmp_array_length_result_n724) != HXC_STATUS_OK)
          {
            abort();
          }
          if (!(hxc_l_tmp_load_result_n721 < hxc_l_tmp_array_length_result_n724))
          {
            break;
          }
          struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n725 = hxc_l_levelView;
          hxc_l_gc_roots[183] = (const void *)hxc_l_tmp_load_result_n725;
          if (hxc_l_tmp_load_result_n725 == NULL)
          {
            abort();
          }
          struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n726 = (*hxc_l_tmp_load_result_n725).hxc_items;
          struct hxc_caxecraft_app_LoadedWorldItem hxc_l_tmp_array_get_result_n728;
          if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n726, (size_t)hxc_l_pickupIndex, &hxc_l_tmp_array_get_result_n728) != HXC_STATUS_OK)
          {
            abort();
          }
          struct hxc_caxecraft_app_LoadedWorldItem hxc_l_loadedItem = hxc_l_tmp_array_get_result_n728;
          struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n729 = hxc_l_session;
          hxc_l_gc_roots[184] = (const void *)hxc_l_tmp_load_result_n729;
          if (hxc_l_tmp_load_result_n729 == NULL)
          {
            abort();
          }
          bool hxc_l_tmp_instance_call_result_n731 = hxc_caxecraft_domain_GameSession_authoredItemIsActive(hxc_l_tmp_load_result_n729, hxc_l_pickupIndex);
          bool hxc_l_tmp_short_circuit_result_n271 = hxc_l_tmp_instance_call_result_n731;
          if (hxc_l_tmp_instance_call_result_n731)
          {
            double hxc_l_tmp_record_field_load_result_n732 = hxc_l_character.hxc_body.hxc_x;
            double hxc_l_tmp_record_field_load_result_n733 = hxc_l_character.hxc_body.hxc_y;
            double hxc_l_tmp_record_field_load_result_n734 = hxc_l_character.hxc_body.hxc_z;
            int32_t hxc_l_tmp_record_field_load_result_n735 = hxc_l_loadedItem.hxc_xMilli;
            int32_t hxc_l_tmp_record_field_load_result_n736 = hxc_l_loadedItem.hxc_yMilli;
            bool hxc_l_tmp_call_result_n738 = hxc_caxecraft_gameplay_WorldItemPickup_isInRange(hxc_l_tmp_record_field_load_result_n732, hxc_l_tmp_record_field_load_result_n733, hxc_l_tmp_record_field_load_result_n734, hxc_l_tmp_record_field_load_result_n735, hxc_l_tmp_record_field_load_result_n736, hxc_l_loadedItem.hxc_zMilli);
            hxc_l_tmp_short_circuit_result_n271 = hxc_l_tmp_call_result_n738;
          }
          if (hxc_l_tmp_short_circuit_result_n271)
          {
            int32_t hxc_l_itemCode = hxc_l_loadedItem.hxc_storageCode;
            struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_tmp_load_result_n741 = hxc_l_contentRegistry;
            hxc_l_gc_roots[185] = (const void *)hxc_l_tmp_load_result_n741;
            if (hxc_l_tmp_load_result_n741 == NULL)
            {
              abort();
            }
            enum hxc_caxecraft_content_RuntimeItemUseProfile hxc_l_tmp_instance_call_result_n743 = hxc_caxecraft_content_RuntimeContentRegistry_itemUseProfile(hxc_l_tmp_load_result_n741, hxc_l_itemCode);
            bool hxc_l_tmp_short_circuit_result_n273 = hxc_l_tmp_instance_call_result_n743 == hxc_caxecraft_content_RuntimeItemUseProfile_EquipAquatic;
            if (hxc_l_tmp_instance_call_result_n743 == hxc_caxecraft_content_RuntimeItemUseProfile_EquipAquatic)
            {
              struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_tmp_load_result_n745 = hxc_l_contentRegistry;
              hxc_l_gc_roots[186] = (const void *)hxc_l_tmp_load_result_n745;
              if (hxc_l_tmp_load_result_n745 == NULL)
              {
                abort();
              }
              bool hxc_l_tmp_instance_call_result_n747 = hxc_caxecraft_content_RuntimeContentRegistry_itemProvidesAquaticProfile(hxc_l_tmp_load_result_n745, hxc_l_itemCode);
              hxc_l_tmp_short_circuit_result_n273 = hxc_l_tmp_instance_call_result_n747;
            }
            if (hxc_l_tmp_short_circuit_result_n273)
            {
              struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_tmp_load_result_n749 = hxc_l_contentRegistry;
              hxc_l_gc_roots[187] = (const void *)hxc_l_tmp_load_result_n749;
              if (hxc_l_tmp_load_result_n749 == NULL)
              {
                abort();
              }
              struct hxc_caxecraft_domain_AquaticProfile hxc_l_tmp_instance_call_result_n751 = hxc_caxecraft_content_RuntimeContentRegistry_itemAquaticProfile(hxc_l_tmp_load_result_n749, hxc_l_itemCode);
              struct hxc_caxecraft_domain_AquaticProfile hxc_l_replacement = hxc_l_tmp_instance_call_result_n751;
              struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n752 = hxc_l_session;
              hxc_l_gc_roots[188] = (const void *)hxc_l_tmp_load_result_n752;
              if (hxc_l_tmp_load_result_n752 == NULL)
              {
                abort();
              }
              int32_t hxc_l_tmp_load_result_n753 = hxc_l_pickupIndex;
              struct hxc_caxecraft_domain_AuthoredAquaticEquipmentResult hxc_l_tmp_instance_call_result_n755 = hxc_caxecraft_domain_GameSession_collectAuthoredAquaticEquipment(hxc_l_tmp_load_result_n752, hxc_l_tmp_load_result_n753, hxc_l_replacement);
              struct hxc_caxecraft_domain_AuthoredAquaticEquipmentResult hxc_l_equipment = hxc_l_tmp_instance_call_result_n755;
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
          struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n763 = hxc_l_session;
          hxc_l_gc_roots[189] = (const void *)hxc_l_tmp_load_result_n763;
          if (hxc_l_tmp_load_result_n763 == NULL)
          {
            abort();
          }
          int32_t hxc_l_tmp_record_field_load_result_n764 = hxc_l_gameTick.hxc_tickIndex;
          struct hxc_caxecraft_domain_ActorControllerTickResult hxc_l_tmp_instance_call_result_n766 = hxc_caxecraft_domain_GameSession_stepAuthoredActorControllers(hxc_l_tmp_load_result_n763, hxc_l_tmp_record_field_load_result_n764, hxc_l_damagePolicy);
          struct hxc_caxecraft_domain_ActorControllerTickResult hxc_l_actorTick = hxc_l_tmp_instance_call_result_n766;
          switch (hxc_l_actorTick.hxc_status.hxc_tag) {
            case hxc_caxecraft_domain_ActorControllerTickStatus_ControllersAdvanced:
              {
                hxc_l_guideInteractionAvailable = false;
                int32_t hxc_l_g_ha92280dd19c1 = 0;
                struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n768 = hxc_l_session;
                hxc_l_gc_roots[193] = (const void *)hxc_l_tmp_load_result_n768;
                if (hxc_l_tmp_load_result_n768 == NULL)
                {
                  abort();
                }
                struct hxc_array_ref *hxc_l_tmp_instance_call_result_n769 = hxc_caxecraft_domain_GameSession_actorControllerEventSnapshots(hxc_l_tmp_load_result_n768);
                hxc_l_g1_h7bf1eee5d9d8 = hxc_l_tmp_instance_call_result_n769;
                while (1)
                {
                  int32_t hxc_l_tmp_load_result_n770 = hxc_l_g_ha92280dd19c1;
                  int32_t hxc_l_tmp_array_length_result_n772;
                  if (hxc_array_ref_length(hxc_l_g1_h7bf1eee5d9d8, &hxc_l_tmp_array_length_result_n772) != HXC_STATUS_OK)
                  {
                    abort();
                  }
                  if (!(hxc_l_tmp_load_result_n770 < hxc_l_tmp_array_length_result_n772))
                  {
                    break;
                  }
                  struct hxc_array_ref *hxc_l_tmp_load_result_n773 = hxc_l_g1_h7bf1eee5d9d8;
                  struct hxc_caxecraft_domain_ActorControllerEvent hxc_l_tmp_array_get_result_n775;
                  if (hxc_array_ref_get_copy(hxc_l_tmp_load_result_n773, (size_t)hxc_l_g_ha92280dd19c1, &hxc_l_tmp_array_get_result_n775) != HXC_STATUS_OK)
                  {
                    abort();
                  }
                  hxc_l_tmp_array_element_owner_n280 = hxc_l_tmp_array_get_result_n775;
                  hxc_l_event = hxc_l_tmp_array_element_owner_n280;
                  if (hxc_enum_8b2a26b8_retain(&hxc_l_event) != HXC_STATUS_OK)
                  {
                    abort();
                  }
                  hxc_l_g_ha92280dd19c1 = hxc_i32_add_wrapping(hxc_l_g_ha92280dd19c1, 1);
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
                        int32_t hxc_l_tmp_enum_payload_project_n780 = hxc_l_event.hxc_payload.hxc_InteractionAvailable.hxc_characterId;
                        int32_t hxc_l_source_h22d183040856 = hxc_l_tmp_enum_payload_project_n780;
                        int32_t hxc_l_tmp_load_result_n781 = hxc_l_source_h22d183040856;
                        if (hxc_l_tmp_load_result_n781 == hxc_l_dialogueActorId)
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
                        int32_t hxc_l_tmp_enum_payload_project_n784 = hxc_l_event.hxc_payload.hxc_LocalPlayerAttack.hxc_characterId;
                        int32_t hxc_l_source_he1fe797e26ff = hxc_l_tmp_enum_payload_project_n784;
                        int32_t hxc_l_tmp_load_result_n785 = hxc_l_source_he1fe797e26ff;
                        if (hxc_l_tmp_load_result_n785 == hxc_l_enemyActorId)
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
                        int32_t hxc_l_tmp_enum_payload_project_n788 = hxc_l_event.hxc_payload.hxc_DropRequested.hxc_characterId;
                        int32_t hxc_l_source_h43c4911f59e8 = hxc_l_tmp_enum_payload_project_n788;
                        if (hxc_l_event.hxc_tag != hxc_caxecraft_domain_ActorControllerEvent_DropRequested)
                        {
                          abort();
                        }
                        hxc_string hxc_l_tmp_enum_payload_project_n790 = hxc_l_event.hxc_payload.hxc_DropRequested.hxc_drop;
                        hxc_l_drop = hxc_l_tmp_enum_payload_project_n790;
                        if (hxc_string_retain(hxc_l_drop) != HXC_STATUS_OK)
                        {
                          abort();
                        }
                        struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_tmp_load_result_n791 = hxc_l_contentRegistry;
                        hxc_l_gc_roots[194] = (const void *)hxc_l_tmp_load_result_n791;
                        if (hxc_l_tmp_load_result_n791 == NULL)
                        {
                          abort();
                        }
                        int32_t hxc_l_tmp_instance_call_result_n793 = hxc_caxecraft_content_RuntimeContentRegistry_dropQuantityById(hxc_l_tmp_load_result_n791, hxc_l_drop);
                        int32_t hxc_l_quantity = hxc_l_tmp_instance_call_result_n793;
                        struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n794 = hxc_l_session;
                        hxc_l_gc_roots[195] = (const void *)hxc_l_tmp_load_result_n794;
                        if (hxc_l_tmp_load_result_n794 == NULL)
                        {
                          abort();
                        }
                        struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n796 = hxc_caxecraft_domain_GameSession_readCharacter(hxc_l_tmp_load_result_n794, hxc_l_source_h43c4911f59e8);
                        struct hxc_caxecraft_domain_Character hxc_l_defeatedActor = hxc_l_tmp_instance_call_result_n796;
                        int32_t hxc_l_tmp_load_result_n797 = hxc_l_quantity;
                        bool hxc_l_tmp_short_circuit_result_n287 = hxc_l_tmp_load_result_n797 <= 0;
                        if (!(hxc_l_tmp_load_result_n797 <= 0))
                        {
                          hxc_l_tmp_short_circuit_result_n287 = hxc_l_defeatedActor.hxc_id <= 0;
                        }
                        if (hxc_l_tmp_short_circuit_result_n287)
                        {
                          hxc_l_quit = true;
                        }
                        else
                        {
                          if (!hxc_l_berryDrop.hxc_active)
                          {
                            struct hxc_caxecraft_domain_Character hxc_l_tmp_load_result_n801 = hxc_l_defeatedActor;
                            struct hxc_caxecraft_gameplay_BerryDropState hxc_l_tmp_call_result_n803 = hxc_caxecraft_gameplay_BerryDrop_fromDefeatedCharacter(hxc_l_tmp_load_result_n801, hxc_l_quantity);
                            hxc_l_berryDrop = hxc_l_tmp_call_result_n803;
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
                  hxc_array_43e9f8ca_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n280);
                }
                if (hxc_array_ref_release(hxc_l_g1_h7bf1eee5d9d8) != HXC_STATUS_OK)
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
        struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n804 = hxc_l_session;
        hxc_l_gc_roots[190] = (const void *)hxc_l_tmp_load_result_n804;
        if (hxc_l_tmp_load_result_n804 == NULL)
        {
          abort();
        }
        struct hxc_array_ref *hxc_l_tmp_instance_call_result_n805 = hxc_caxecraft_domain_GameSession_actorControllerStateSnapshots(hxc_l_tmp_load_result_n804);
        hxc_l_actorPhases = hxc_l_tmp_instance_call_result_n805;
        struct hxc_array_ref *hxc_l_tmp_load_result_n806 = hxc_l_actorPhases;
        struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation hxc_l_tmp_call_result_n809 = hxc_caxecraft_app_CaxecraftApp_observeActorPhase(hxc_l_tmp_load_result_n806, hxc_l_dialogueActorId, hxc_caxecraft_domain_ActorControllerPhase_Stationary);
        hxc_l_dialoguePhase = hxc_l_tmp_call_result_n809;
        struct hxc_array_ref *hxc_l_tmp_load_result_n810 = hxc_l_actorPhases;
        struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation hxc_l_tmp_call_result_n813 = hxc_caxecraft_app_CaxecraftApp_observeActorPhase(hxc_l_tmp_load_result_n810, hxc_l_enemyActorId, hxc_caxecraft_domain_ActorControllerPhase_Defeated);
        hxc_l_enemyPhase_ha80e5c596f34 = hxc_l_tmp_call_result_n813;
        struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n814 = hxc_l_session;
        hxc_l_gc_roots[191] = (const void *)hxc_l_tmp_load_result_n814;
        if (hxc_l_tmp_load_result_n814 == NULL)
        {
          abort();
        }
        struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n816 = hxc_caxecraft_domain_GameSession_readCharacter(hxc_l_tmp_load_result_n814, hxc_l_dialogueActorId);
        hxc_l_dialogueActor = hxc_l_tmp_instance_call_result_n816;
        struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n817 = hxc_l_session;
        hxc_l_gc_roots[192] = (const void *)hxc_l_tmp_load_result_n817;
        if (hxc_l_tmp_load_result_n817 == NULL)
        {
          abort();
        }
        struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n819 = hxc_caxecraft_domain_GameSession_readCharacter(hxc_l_tmp_load_result_n817, hxc_l_enemyActorId);
        hxc_l_enemyActor = hxc_l_tmp_instance_call_result_n819;
        bool hxc_l_tmp_record_field_load_result_n820 = hxc_l_dialoguePhase.hxc_valid;
        bool hxc_l_tmp_short_circuit_result_n289 = !hxc_l_tmp_record_field_load_result_n820;
        if (!!hxc_l_tmp_record_field_load_result_n820)
        {
          hxc_l_tmp_short_circuit_result_n289 = !hxc_l_enemyPhase_ha80e5c596f34.hxc_valid;
        }
        bool hxc_l_tmp_short_circuit_load_result_n822 = hxc_l_tmp_short_circuit_result_n289;
        bool hxc_l_tmp_short_circuit_result_n290 = hxc_l_tmp_short_circuit_load_result_n822;
        if (!hxc_l_tmp_short_circuit_load_result_n822)
        {
          hxc_l_tmp_short_circuit_result_n290 = hxc_l_dialogueActor.hxc_id <= 0;
        }
        bool hxc_l_tmp_short_circuit_load_result_n824 = hxc_l_tmp_short_circuit_result_n290;
        bool hxc_l_tmp_short_circuit_result_n291 = hxc_l_tmp_short_circuit_load_result_n824;
        if (!hxc_l_tmp_short_circuit_load_result_n824)
        {
          hxc_l_tmp_short_circuit_result_n291 = hxc_l_enemyActor.hxc_id <= 0;
        }
        if (hxc_l_tmp_short_circuit_result_n291)
        {
          hxc_l_quit = true;
        }
        struct hxc_caxecraft_gameplay_SwordCombatState hxc_l_tmp_call_result_n828 = hxc_caxecraft_gameplay_SwordCombat_step(hxc_l_swordCombat);
        hxc_l_swordCombat = hxc_l_tmp_call_result_n828;
        if (hxc_l_swordQueued)
        {
          struct hxc_caxecraft_gameplay_SwordCombatState hxc_l_tmp_load_result_n830 = hxc_l_swordCombat;
          struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_load_result_n831 = hxc_l_inventory;
          struct hxc_caxecraft_domain_VitalsState hxc_l_tmp_record_field_load_result_n832 = hxc_l_character.hxc_vitals;
          struct hxc_caxecraft_domain_Character hxc_l_tmp_load_result_n833 = hxc_l_enemyActor;
          double hxc_l_tmp_record_field_load_result_n834 = hxc_l_character.hxc_body.hxc_x;
          double hxc_l_tmp_record_field_load_result_n835 = hxc_l_character.hxc_body.hxc_z;
          double hxc_l_tmp_load_result_n836 = hxc_l_lookX;
          int32_t hxc_l_tmp_call_result_n838 = hxc_caxecraft_gameplay_SwordCombat_decide(hxc_l_tmp_load_result_n830, hxc_l_tmp_load_result_n831, hxc_l_tmp_record_field_load_result_n832, hxc_l_tmp_load_result_n833, hxc_l_tmp_record_field_load_result_n834, hxc_l_tmp_record_field_load_result_n835, hxc_l_tmp_load_result_n836, hxc_l_lookZ);
          int32_t hxc_l_swordDecision = hxc_l_tmp_call_result_n838;
          if (hxc_l_swordDecision == 5)
          {
            struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n840 = hxc_l_session;
            hxc_l_gc_roots[196] = (const void *)hxc_l_tmp_load_result_n840;
            if (hxc_l_tmp_load_result_n840 == NULL)
            {
              abort();
            }
            struct hxc_caxecraft_domain_CharacterDamageResult hxc_l_tmp_instance_call_result_n842 = hxc_caxecraft_domain_GameSession_damageCharacter(hxc_l_tmp_load_result_n840, hxc_l_enemyActorId, 1);
            struct hxc_caxecraft_domain_CharacterDamageResult hxc_l_damage = hxc_l_tmp_instance_call_result_n842;
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
          int32_t hxc_l_tmp_load_result_n846 = hxc_l_swordDecision;
          struct hxc_caxecraft_gameplay_SwordCombatState hxc_l_tmp_call_result_n848 = hxc_caxecraft_gameplay_SwordCombat_after(hxc_l_tmp_load_result_n846, hxc_l_swordCombat);
          hxc_l_swordCombat = hxc_l_tmp_call_result_n848;
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
    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n853 = hxc_l_session;
    hxc_l_gc_roots[180] = (const void *)hxc_l_tmp_load_result_n853;
    if (hxc_l_tmp_load_result_n853 == NULL)
    {
      abort();
    }
    size_t hxc_l_tmp_length_n855;
    const uint8_t *hxc_l_tmp_instance_call_result_n854 = hxc_caxecraft_domain_GameSession_worldView(hxc_l_tmp_load_result_n853, &hxc_l_tmp_length_n855);
    double hxc_l_tmp_load_result_n856 = hxc_l_selectionEyeX;
    double hxc_l_tmp_load_result_n857 = hxc_l_selectionEyeY;
    double hxc_l_tmp_load_result_n858 = hxc_l_selectionEyeZ;
    double hxc_l_tmp_load_result_n859 = hxc_l_lookX;
    double hxc_l_tmp_load_result_n860 = hxc_l_lookY;
    struct hxc_caxecraft_domain_RaycastHit hxc_l_tmp_call_result_n862 = hxc_caxecraft_domain_VoxelRaycast_trace(hxc_l_tmp_instance_call_result_n854, hxc_l_tmp_length_n855, hxc_l_tmp_load_result_n856, hxc_l_tmp_load_result_n857, hxc_l_tmp_load_result_n858, hxc_l_tmp_load_result_n859, hxc_l_tmp_load_result_n860, hxc_l_lookZ, 7.0);
    struct hxc_caxecraft_domain_RaycastHit hxc_l_hit = hxc_l_tmp_call_result_n862;
    bool hxc_l_tmp_load_result_n863 = hxc_l_captured_h4e575de23f09;
    bool hxc_l_tmp_short_circuit_result_n298 = hxc_l_tmp_load_result_n863;
    if (hxc_l_tmp_load_result_n863)
    {
      hxc_l_tmp_short_circuit_result_n298 = !hxc_l_recapturedThisFrame;
    }
    bool hxc_l_tmp_short_circuit_load_result_n865 = hxc_l_tmp_short_circuit_result_n298;
    bool hxc_l_tmp_short_circuit_result_n299 = hxc_l_tmp_short_circuit_load_result_n865;
    if (hxc_l_tmp_short_circuit_load_result_n865)
    {
      hxc_l_tmp_short_circuit_result_n299 = hxc_l_primaryPressed_h4a0a8b20cf08;
    }
    if (hxc_l_tmp_short_circuit_result_n299)
    {
      if (hxc_l_character.hxc_vitals.hxc_health > 0)
      {
        if (hxc_l_selectedMode == 1)
        {
          bool hxc_l_tmp_call_result_n872 = hxc_caxecraft_gameplay_Inventory_selectedIs(hxc_l_inventory, hxc_caxecraft_gameplay_ItemKind_CopperSword);
          bool hxc_l_tmp_short_circuit_result_n300 = !hxc_l_tmp_call_result_n872;
          if (!hxc_l_tmp_call_result_n872)
          {
            hxc_l_tmp_short_circuit_result_n300 = hxc_l_hit.hxc_hit;
          }
          bool hxc_l_tmp_short_circuit_load_result_n874 = hxc_l_tmp_short_circuit_result_n300;
          bool hxc_l_tmp_short_circuit_result_n301 = hxc_l_tmp_short_circuit_load_result_n874;
          if (hxc_l_tmp_short_circuit_load_result_n874)
          {
            struct hxc_caxecraft_domain_AquaticState hxc_l_tmp_record_field_load_result_n875 = hxc_l_character.hxc_aquatic;
            bool hxc_l_tmp_call_result_n877 = hxc_caxecraft_domain_Aquatics_canMine(hxc_l_tmp_record_field_load_result_n875, hxc_l_character.hxc_aquaticProfile);
            hxc_l_tmp_short_circuit_result_n301 = hxc_l_tmp_call_result_n877;
          }
          if (hxc_l_tmp_short_circuit_result_n301)
          {
            int32_t hxc_l_tmp_record_field_load_result_n879 = hxc_l_hit.hxc_cellX;
            int32_t hxc_l_tmp_record_field_load_result_n880 = hxc_l_hit.hxc_cellY;
            struct hxc_caxecraft_scenario_VoxelPoint hxc_l_tmp_call_result_n882 = hxc_caxecraft_domain_World_coord(hxc_l_tmp_record_field_load_result_n879, hxc_l_tmp_record_field_load_result_n880, hxc_l_hit.hxc_cellZ);
            struct hxc_caxecraft_scenario_VoxelPoint hxc_l_minedCoordinate = hxc_l_tmp_call_result_n882;
            int32_t hxc_l_sandBefore = hxc_l_inventory.hxc_sand;
            (void)hxc_l_sandBefore;
            struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n884 = hxc_l_session;
            hxc_l_gc_roots[197] = (const void *)hxc_l_tmp_load_result_n884;
            if (hxc_l_tmp_load_result_n884 == NULL)
            {
              abort();
            }
            struct hxc_caxecraft_scenario_VoxelPoint hxc_l_tmp_load_result_n885 = hxc_l_minedCoordinate;
            struct hxc_caxecraft_gameplay_MiningResult hxc_l_tmp_instance_call_result_n887 = hxc_caxecraft_domain_GameSession_mineTerrain(hxc_l_tmp_load_result_n884, hxc_l_tmp_load_result_n885, hxc_l_inventory);
            struct hxc_caxecraft_gameplay_MiningResult hxc_l_mining = hxc_l_tmp_instance_call_result_n887;
            hxc_l_inventory = hxc_l_mining.hxc_inventory;
            if (hxc_l_mining.hxc_outcome == 2)
            {
              if (hxc_l_self == NULL)
              {
                abort();
              }
              struct hxc_caxecraft_app_TerrainRenderer *hxc_l_tmp_owned_class_field_address_n890 = &(*hxc_l_self).hxc_terrainRenderer;
              struct hxc_caxecraft_app_TerrainChunkCache *hxc_l_tmp_owned_class_field_address_n891 = &(*hxc_l_tmp_owned_class_field_address_n890).hxc_cache;
              if (hxc_l_tmp_owned_class_field_address_n891 == NULL)
              {
                abort();
              }
              hxc_caxecraft_app_TerrainChunkCache_invalidate(hxc_l_tmp_owned_class_field_address_n891, hxc_l_minedCoordinate);
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
            int32_t hxc_l_tmp_record_field_load_result_n895 = hxc_l_hit.hxc_cellX;
            int32_t hxc_l_tmp_record_field_load_result_n896 = hxc_l_hit.hxc_cellY;
            struct hxc_caxecraft_scenario_VoxelPoint hxc_l_tmp_call_result_n898 = hxc_caxecraft_domain_World_coord(hxc_l_tmp_record_field_load_result_n895, hxc_l_tmp_record_field_load_result_n896, hxc_l_hit.hxc_cellZ);
            struct hxc_caxecraft_scenario_VoxelPoint hxc_l_removedCoordinate = hxc_l_tmp_call_result_n898;
            struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n899 = hxc_l_session;
            hxc_l_gc_roots[198] = (const void *)hxc_l_tmp_load_result_n899;
            if (hxc_l_tmp_load_result_n899 == NULL)
            {
              abort();
            }
            bool hxc_l_tmp_instance_call_result_n901 = hxc_caxecraft_domain_GameSession_removeTerrain(hxc_l_tmp_load_result_n899, hxc_l_removedCoordinate);
            bool hxc_l_removed = hxc_l_tmp_instance_call_result_n901;
            if (hxc_l_removed)
            {
              if (hxc_l_self == NULL)
              {
                abort();
              }
              struct hxc_caxecraft_app_TerrainRenderer *hxc_l_tmp_owned_class_field_address_n903 = &(*hxc_l_self).hxc_terrainRenderer;
              struct hxc_caxecraft_app_TerrainChunkCache *hxc_l_tmp_owned_class_field_address_n904 = &(*hxc_l_tmp_owned_class_field_address_n903).hxc_cache;
              if (hxc_l_tmp_owned_class_field_address_n904 == NULL)
              {
                abort();
              }
              hxc_caxecraft_app_TerrainChunkCache_invalidate(hxc_l_tmp_owned_class_field_address_n904, hxc_l_removedCoordinate);
            }
          }
        }
      }
    }
    bool hxc_l_tmp_load_result_n906 = hxc_l_captured_h4e575de23f09;
    bool hxc_l_tmp_short_circuit_result_n307 = hxc_l_tmp_load_result_n906;
    if (hxc_l_tmp_load_result_n906)
    {
      hxc_l_tmp_short_circuit_result_n307 = hxc_l_secondaryPressed_h3c635e3846cc;
    }
    if (hxc_l_tmp_short_circuit_result_n307)
    {
      if (hxc_l_character.hxc_vitals.hxc_health > 0)
      {
        struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n910 = hxc_l_session;
        hxc_l_gc_roots[199] = (const void *)hxc_l_tmp_load_result_n910;
        if (hxc_l_tmp_load_result_n910 == NULL)
        {
          abort();
        }
        struct hxc_caxecraft_domain_LocalRecoveryResult hxc_l_tmp_instance_call_result_n912 = hxc_caxecraft_domain_GameSession_useSelectedRecovery(hxc_l_tmp_load_result_n910, hxc_l_inventory);
        struct hxc_caxecraft_domain_LocalRecoveryResult hxc_l_recovery = hxc_l_tmp_instance_call_result_n912;
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
              int32_t hxc_l_tmp_record_field_load_result_n919 = hxc_l_hit.hxc_previousX;
              int32_t hxc_l_tmp_record_field_load_result_n920 = hxc_l_hit.hxc_previousY;
              struct hxc_caxecraft_scenario_VoxelPoint hxc_l_tmp_call_result_n922 = hxc_caxecraft_domain_World_coord(hxc_l_tmp_record_field_load_result_n919, hxc_l_tmp_record_field_load_result_n920, hxc_l_hit.hxc_previousZ);
              struct hxc_caxecraft_scenario_VoxelPoint hxc_l_placement = hxc_l_tmp_call_result_n922;
              enum hxc_caxecraft_domain_BlockKind hxc_l_tmp_call_result_n924 = hxc_caxecraft_gameplay_Inventory_selectedBlock(hxc_l_inventory);
              enum hxc_caxecraft_domain_BlockKind hxc_l_selectedBlock = hxc_l_tmp_call_result_n924;
              struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_load_result_n925 = hxc_l_inventory;
              int32_t hxc_l_tmp_call_result_n927 = hxc_caxecraft_gameplay_Inventory_countAt(hxc_l_tmp_load_result_n925, hxc_l_inventory.hxc_selected);
              bool hxc_l_hasItem = hxc_l_tmp_call_result_n927 > 0;
              bool hxc_l_tmp_load_result_n928 = hxc_l_hasItem;
              bool hxc_l_tmp_short_circuit_result_n312 = !hxc_l_tmp_load_result_n928;
              if (!!hxc_l_tmp_load_result_n928)
              {
                bool hxc_l_tmp_call_result_n930 = hxc_caxecraft_domain_World_isPlaceable(hxc_l_selectedBlock);
                hxc_l_tmp_short_circuit_result_n312 = !hxc_l_tmp_call_result_n930;
              }
              bool hxc_l_tmp_short_circuit_load_result_n931 = hxc_l_tmp_short_circuit_result_n312;
              bool hxc_l_tmp_short_circuit_result_n313 = hxc_l_tmp_short_circuit_load_result_n931;
              if (!hxc_l_tmp_short_circuit_load_result_n931)
              {
                struct hxc_caxecraft_domain_CharacterBody hxc_l_tmp_record_field_load_result_n932 = hxc_l_character.hxc_body;
                bool hxc_l_tmp_call_result_n934 = hxc_caxecraft_domain_CharacterPhysics_canPlaceAt(hxc_l_tmp_record_field_load_result_n932, hxc_l_placement);
                hxc_l_tmp_short_circuit_result_n313 = !hxc_l_tmp_call_result_n934;
              }
              bool hxc_l_tmp_short_circuit_load_result_n935 = hxc_l_tmp_short_circuit_result_n313;
              bool hxc_l_tmp_short_circuit_result_n314 = hxc_l_tmp_short_circuit_load_result_n935;
              if (!hxc_l_tmp_short_circuit_load_result_n935)
              {
                struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n936 = hxc_l_session;
                hxc_l_gc_roots[200] = (const void *)hxc_l_tmp_load_result_n936;
                if (hxc_l_tmp_load_result_n936 == NULL)
                {
                  abort();
                }
                struct hxc_caxecraft_scenario_VoxelPoint hxc_l_tmp_load_result_n937 = hxc_l_placement;
                bool hxc_l_tmp_instance_call_result_n939 = hxc_caxecraft_domain_GameSession_placeTerrain(hxc_l_tmp_load_result_n936, hxc_l_tmp_load_result_n937, hxc_l_selectedBlock);
                hxc_l_tmp_short_circuit_result_n314 = !hxc_l_tmp_instance_call_result_n939;
              }
              if (hxc_l_tmp_short_circuit_result_n314)
              {
                hxc_l_placementBlockedFrames = 60;
              }
              else
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
                hxc_caxecraft_app_TerrainChunkCache_invalidate(hxc_l_tmp_owned_class_field_address_n942, hxc_l_placement);
                if (hxc_l_selectedMode == 1)
                {
                  struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n946 = hxc_caxecraft_gameplay_Inventory_consumeSelected(hxc_l_inventory);
                  hxc_l_inventory = hxc_l_tmp_call_result_n946;
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
    if (!hxc_l_paused_h9ad6cd97e1b1)
    {
      struct hxc_caxecraft_gameplay_BerryDropState hxc_l_tmp_load_result_n950 = hxc_l_berryDrop;
      double hxc_l_tmp_record_field_load_result_n951 = hxc_l_character.hxc_body.hxc_x;
      double hxc_l_tmp_record_field_load_result_n952 = hxc_l_character.hxc_body.hxc_y;
      bool hxc_l_tmp_call_result_n954 = hxc_caxecraft_gameplay_BerryDrop_isInRange(hxc_l_tmp_load_result_n950, hxc_l_tmp_record_field_load_result_n951, hxc_l_tmp_record_field_load_result_n952, hxc_l_character.hxc_body.hxc_z);
      if (hxc_l_tmp_call_result_n954)
      {
        struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_load_result_n955 = hxc_l_inventory;
        int32_t hxc_l_tmp_call_result_n958 = hxc_caxecraft_gameplay_Inventory_acceptedAmount(hxc_l_tmp_load_result_n955, hxc_caxecraft_gameplay_ItemKind_Berries, hxc_l_berryDrop.hxc_amount);
        int32_t hxc_l_acceptedDrop = hxc_l_tmp_call_result_n958;
        if (hxc_l_acceptedDrop > 0)
        {
          struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_load_result_n960 = hxc_l_inventory;
          struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n963 = hxc_caxecraft_gameplay_Inventory_collectItem(hxc_l_tmp_load_result_n960, hxc_caxecraft_gameplay_ItemKind_Berries, hxc_l_acceptedDrop);
          hxc_l_inventory = hxc_l_tmp_call_result_n963;
          struct hxc_caxecraft_gameplay_BerryDropState hxc_l_tmp_load_result_n964 = hxc_l_berryDrop;
          struct hxc_caxecraft_gameplay_BerryDropState hxc_l_tmp_call_result_n966 = hxc_caxecraft_gameplay_BerryDrop_collectAmount(hxc_l_tmp_load_result_n964, hxc_l_acceptedDrop);
          hxc_l_berryDrop = hxc_l_tmp_call_result_n966;
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
    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n983 = hxc_l_session;
    hxc_l_gc_roots[201] = (const void *)hxc_l_tmp_load_result_n983;
    if (hxc_l_tmp_load_result_n983 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_domain_GameView hxc_l_tmp_instance_call_result_n984 = hxc_caxecraft_domain_GameSession_view(hxc_l_tmp_load_result_n983);
    struct hxc_caxecraft_domain_GameView hxc_l_committedView = hxc_l_tmp_instance_call_result_n984;
    if (!hxc_l_committedView.hxc_valid)
    {
      hxc_l_quit = true;
    }
    else
    {
      hxc_l_character = hxc_l_committedView.hxc_localPlayer;
    }
    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n987 = hxc_l_session;
    hxc_l_gc_roots[202] = (const void *)hxc_l_tmp_load_result_n987;
    if (hxc_l_tmp_load_result_n987 == NULL)
    {
      abort();
    }
    bool hxc_l_tmp_instance_call_result_n989 = hxc_caxecraft_domain_GameSession_actorInteractionAvailable(hxc_l_tmp_load_result_n987, hxc_l_dialogueActorId);
    hxc_l_guideInteractionAvailable = hxc_l_tmp_instance_call_result_n989;
    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n990 = hxc_l_session;
    hxc_l_gc_roots[203] = (const void *)hxc_l_tmp_load_result_n990;
    if (hxc_l_tmp_load_result_n990 == NULL)
    {
      abort();
    }
    struct hxc_array_ref *hxc_l_tmp_instance_call_result_n991 = hxc_caxecraft_domain_GameSession_actorControllerStateSnapshots(hxc_l_tmp_load_result_n990);
    hxc_l_presentationActorPhases = hxc_l_tmp_instance_call_result_n991;
    struct hxc_array_ref *hxc_l_tmp_load_result_n992 = hxc_l_presentationActorPhases;
    struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation hxc_l_tmp_call_result_n995 = hxc_caxecraft_app_CaxecraftApp_observeActorPhase(hxc_l_tmp_load_result_n992, hxc_l_dialogueActorId, hxc_caxecraft_domain_ActorControllerPhase_Stationary);
    hxc_l_dialoguePhase = hxc_l_tmp_call_result_n995;
    struct hxc_array_ref *hxc_l_tmp_load_result_n996 = hxc_l_presentationActorPhases;
    struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation hxc_l_tmp_call_result_n999 = hxc_caxecraft_app_CaxecraftApp_observeActorPhase(hxc_l_tmp_load_result_n996, hxc_l_enemyActorId, hxc_caxecraft_domain_ActorControllerPhase_Defeated);
    hxc_l_enemyPhase_ha80e5c596f34 = hxc_l_tmp_call_result_n999;
    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1000 = hxc_l_session;
    hxc_l_gc_roots[204] = (const void *)hxc_l_tmp_load_result_n1000;
    if (hxc_l_tmp_load_result_n1000 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n1002 = hxc_caxecraft_domain_GameSession_readCharacter(hxc_l_tmp_load_result_n1000, hxc_l_dialogueActorId);
    hxc_l_dialogueActor = hxc_l_tmp_instance_call_result_n1002;
    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1003 = hxc_l_session;
    hxc_l_gc_roots[205] = (const void *)hxc_l_tmp_load_result_n1003;
    if (hxc_l_tmp_load_result_n1003 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n1005 = hxc_caxecraft_domain_GameSession_readCharacter(hxc_l_tmp_load_result_n1003, hxc_l_enemyActorId);
    hxc_l_enemyActor = hxc_l_tmp_instance_call_result_n1005;
    bool hxc_l_tmp_record_field_load_result_n1006 = hxc_l_dialoguePhase.hxc_valid;
    bool hxc_l_tmp_short_circuit_result_n318 = !hxc_l_tmp_record_field_load_result_n1006;
    if (!!hxc_l_tmp_record_field_load_result_n1006)
    {
      hxc_l_tmp_short_circuit_result_n318 = !hxc_l_enemyPhase_ha80e5c596f34.hxc_valid;
    }
    bool hxc_l_tmp_short_circuit_load_result_n1008 = hxc_l_tmp_short_circuit_result_n318;
    bool hxc_l_tmp_short_circuit_result_n319 = hxc_l_tmp_short_circuit_load_result_n1008;
    if (!hxc_l_tmp_short_circuit_load_result_n1008)
    {
      hxc_l_tmp_short_circuit_result_n319 = hxc_l_dialogueActor.hxc_id <= 0;
    }
    bool hxc_l_tmp_short_circuit_load_result_n1010 = hxc_l_tmp_short_circuit_result_n319;
    bool hxc_l_tmp_short_circuit_result_n320 = hxc_l_tmp_short_circuit_load_result_n1010;
    if (!hxc_l_tmp_short_circuit_load_result_n1010)
    {
      hxc_l_tmp_short_circuit_result_n320 = hxc_l_enemyActor.hxc_id <= 0;
    }
    if (hxc_l_tmp_short_circuit_result_n320)
    {
      hxc_l_quit = true;
    }
    int32_t hxc_l_completedTicks = hxc_l_committedView.hxc_completedTicks;
    if (hxc_l_resetMotionThisFrame)
    {
      struct hxc_caxecraft_app_MotionHistory hxc_l_tmp_call_result_n1016 = hxc_caxecraft_app_MotionInterpolation_start(hxc_l_character.hxc_body);
      hxc_l_motionHistory = hxc_l_tmp_call_result_n1016;
    }
    struct hxc_caxecraft_app_MotionHistory hxc_l_tmp_load_result_n1017 = hxc_l_motionHistory;
    struct hxc_caxecraft_app_RenderPosition hxc_l_tmp_call_result_n1019 = hxc_caxecraft_app_MotionInterpolation_sample(hxc_l_tmp_load_result_n1017, hxc_l_accumulator, 0.05);
    struct hxc_caxecraft_app_RenderPosition hxc_l_renderPosition = hxc_l_tmp_call_result_n1019;
    double hxc_l_eyeX = hxc_l_renderPosition.hxc_x;
    double hxc_l_eyeY = hxc_l_renderPosition.hxc_y + 1.62;
    double hxc_l_eyeZ = hxc_l_renderPosition.hxc_z;
    double hxc_l_tmp_load_result_n1023 = hxc_l_eyeX;
    double hxc_l_tmp_load_result_n1024 = hxc_l_eyeY;
    double hxc_l_tmp_load_result_n1025 = hxc_l_eyeZ;
    double hxc_l_tmp_load_result_n1027 = hxc_l_eyeX;
    double hxc_l_tmp_load_result_n1028 = hxc_l_lookX;
    double hxc_l_tmp_load_result_n1029 = hxc_l_eyeY;
    double hxc_l_tmp_load_result_n1030 = hxc_l_lookY;
    double hxc_l_tmp_load_result_n1031 = hxc_l_eyeZ;
    struct Camera3D hxc_l_camera = (struct Camera3D){ .position = (struct Vector3){ .x = (float)hxc_l_tmp_load_result_n1023, .y = (float)hxc_l_tmp_load_result_n1024, .z = (float)hxc_l_tmp_load_result_n1025 }, .target = (struct Vector3){ .x = (float)(hxc_l_tmp_load_result_n1027 + hxc_l_tmp_load_result_n1028), .y = (float)(hxc_l_tmp_load_result_n1029 + hxc_l_tmp_load_result_n1030), .z = (float)(hxc_l_tmp_load_result_n1031 + hxc_l_lookZ) }, .up = (struct Vector3){ .x = (float)0.0, .y = (float)1.0, .z = (float)0.0 }, .fovy = (float)70.0, .projection = 0 };
    BeginDrawing();
    if (hxc_l_onTitle)
    {
      int32_t hxc_l_tmp_native_call_result_n1037 = GetScreenWidth();
      int32_t hxc_l_width_ha3da84aa5122 = (int32_t)hxc_l_tmp_native_call_result_n1037;
      int32_t hxc_l_tmp_native_call_result_n1038 = GetScreenHeight();
      int32_t hxc_l_height_h0d2fc182597e = (int32_t)hxc_l_tmp_native_call_result_n1038;
      if (hxc_l_titleTextureReady)
      {
        struct Color hxc_l_this1_h5a14329dc89c = (struct Color){ .r = 255, .g = 255, .b = 255, .a = 255 };
        struct Texture hxc_l_tmp_load_result_n1041 = hxc_l_titleTexture;
        double hxc_l_imageAspect = hxc_f64_divide_zero_safe((double)(int32_t)hxc_l_tmp_load_result_n1041.width, (double)(int32_t)hxc_l_titleTexture.height);
        int32_t hxc_l_tmp_load_result_n1045 = hxc_l_width_ha3da84aa5122;
        double hxc_l_screenAspect = hxc_f64_divide_zero_safe((double)hxc_l_tmp_load_result_n1045, (double)hxc_l_height_h0d2fc182597e);
        double hxc_l_sourceX = 0.0;
        double hxc_l_sourceY = 0.0;
        double hxc_l_sourceWidth = (double)(int32_t)hxc_l_titleTexture.width;
        double hxc_l_sourceHeight = (double)(int32_t)hxc_l_titleTexture.height;
        double hxc_l_tmp_load_result_n1051 = hxc_l_screenAspect;
        if (hxc_l_tmp_load_result_n1051 > hxc_l_imageAspect)
        {
          struct Texture hxc_l_tmp_load_result_n1053 = hxc_l_titleTexture;
          hxc_l_sourceHeight = hxc_f64_divide_zero_safe((double)(int32_t)hxc_l_tmp_load_result_n1053.width, hxc_l_screenAspect);
          struct Texture hxc_l_tmp_load_result_n1056 = hxc_l_titleTexture;
          hxc_l_sourceY = ((double)(int32_t)hxc_l_tmp_load_result_n1056.height - hxc_l_sourceHeight) * 0.5;
        }
        else
        {
          struct Texture hxc_l_tmp_load_result_n1059 = hxc_l_titleTexture;
          hxc_l_sourceWidth = (double)(int32_t)hxc_l_tmp_load_result_n1059.height * hxc_l_screenAspect;
          struct Texture hxc_l_tmp_load_result_n1062 = hxc_l_titleTexture;
          hxc_l_sourceX = ((double)(int32_t)hxc_l_tmp_load_result_n1062.width - hxc_l_sourceWidth) * 0.5;
        }
        struct Texture hxc_l_tmp_load_result_n1065 = hxc_l_titleTexture;
        double hxc_l_tmp_load_result_n1066 = hxc_l_sourceX;
        double hxc_l_tmp_load_result_n1067 = hxc_l_sourceY;
        double hxc_l_tmp_load_result_n1068 = hxc_l_sourceWidth;
        double hxc_l_tmp_load_result_n1069 = hxc_l_sourceHeight;
        int32_t hxc_l_tmp_load_result_n1071 = hxc_l_width_ha3da84aa5122;
        int32_t hxc_l_tmp_load_result_n1072 = hxc_l_height_h0d2fc182597e;
        DrawTexturePro(hxc_l_tmp_load_result_n1065, (struct Rectangle){ .x = (float)hxc_l_tmp_load_result_n1066, .y = (float)hxc_l_tmp_load_result_n1067, .width = (float)hxc_l_tmp_load_result_n1068, .height = (float)hxc_l_tmp_load_result_n1069 }, (struct Rectangle){ .x = (float)0.0, .y = (float)0.0, .width = (float)((double)hxc_l_tmp_load_result_n1071 + 0.0), .height = (float)((double)hxc_l_tmp_load_result_n1072 + 0.0) }, (struct Vector2){ .x = (float)0.0, .y = (float)0.0 }, (float)0.0, hxc_l_this1_h5a14329dc89c);
      }
      else
      {
        ClearBackground((struct Color){ .r = 126, .g = 190, .b = 201, .a = 255 });
      }
      struct Color hxc_l_this1_hbfa72eea2628 = (struct Color){ .r = 4, .g = 14, .b = 23, .a = 132 };
      int32_t hxc_l_tmp_load_result_n1078 = hxc_l_width_ha3da84aa5122;
      int32_t hxc_l_tmp_load_result_n1079 = hxc_l_height_h0d2fc182597e;
      DrawRectangle((int32_t)0, (int32_t)0, (int32_t)hxc_l_tmp_load_result_n1078, (int32_t)hxc_l_tmp_load_result_n1079, hxc_l_this1_hbfa72eea2628);
      if (hxc_l_wordmarkTextureReady)
      {
        int32_t hxc_l_centerX = hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_l_width_ha3da84aa5122, (double)2));
        struct Color hxc_l_this1_hb2a9e55b4c24 = (struct Color){ .r = 255, .g = 255, .b = 255, .a = 255 };
        double hxc_l_scale = hxc_f64_divide_zero_safe((double)560, (double)(int32_t)hxc_l_wordmarkTexture.width);
        double hxc_l_heightScale = hxc_f64_divide_zero_safe((double)132, (double)(int32_t)hxc_l_wordmarkTexture.height);
        double hxc_l_tmp_load_result_n1088 = hxc_l_heightScale;
        if (hxc_l_tmp_load_result_n1088 < hxc_l_scale)
        {
          hxc_l_scale = hxc_l_heightScale;
        }
        struct Texture hxc_l_tmp_load_result_n1091 = hxc_l_wordmarkTexture;
        double hxc_l_width_ha00182d86e16 = (double)(int32_t)hxc_l_tmp_load_result_n1091.width * hxc_l_scale;
        struct Texture hxc_l_tmp_load_result_n1094 = hxc_l_wordmarkTexture;
        double hxc_l_height_hffa0aa38900c = (double)(int32_t)hxc_l_tmp_load_result_n1094.height * hxc_l_scale;
        double hxc_l_width1 = (double)(int32_t)hxc_l_wordmarkTexture.width + 0.0;
        double hxc_l_height1 = (double)(int32_t)hxc_l_wordmarkTexture.height + 0.0;
        struct Texture hxc_l_tmp_load_result_n1101 = hxc_l_wordmarkTexture;
        double hxc_l_tmp_load_result_n1102 = hxc_l_width1;
        double hxc_l_tmp_load_result_n1103 = hxc_l_height1;
        int32_t hxc_l_tmp_load_result_n1105 = hxc_l_centerX;
        double hxc_l_tmp_load_result_n1106 = hxc_l_width_ha00182d86e16;
        double hxc_l_tmp_load_result_n1107 = hxc_l_width_ha00182d86e16;
        double hxc_l_tmp_load_result_n1108 = hxc_l_height_hffa0aa38900c;
        DrawTexturePro(hxc_l_tmp_load_result_n1101, (struct Rectangle){ .x = (float)0.0, .y = (float)0.0, .width = (float)hxc_l_tmp_load_result_n1102, .height = (float)hxc_l_tmp_load_result_n1103 }, (struct Rectangle){ .x = (float)((double)hxc_l_tmp_load_result_n1105 - hxc_l_tmp_load_result_n1106 * 0.5), .y = (float)28.0, .width = (float)hxc_l_tmp_load_result_n1107, .height = (float)hxc_l_tmp_load_result_n1108 }, (struct Vector2){ .x = (float)0.0, .y = (float)0.0 }, (float)0.0, hxc_l_this1_hb2a9e55b4c24);
      }
      else
      {
        int32_t hxc_l_x_hde31d3c7a105 = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_l_width_ha3da84aa5122, (double)2)), 92);
        struct Color hxc_l_this1_h0a9c3a675edf = (struct Color){ .r = 242, .g = 249, .b = 245, .a = 255 };
        struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_tmp_load_result_n1114 = hxc_l_uiCatalog;
        hxc_l_gc_roots[206] = (const void *)hxc_l_tmp_load_result_n1114;
        if (hxc_l_tmp_load_result_n1114 == NULL)
        {
          abort();
        }
        hxc_string hxc_l_tmp_instance_call_result_n1116 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_tmp_load_result_n1114, hxc_l_locale, 34);
        hxc_l_tmp_cstring_ref_owner_owner_n347 = hxc_l_tmp_instance_call_result_n1116;
        hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n1118 = HXC_BORROWED_CSTRING_INITIALIZER;
        if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n347, &hxc_l_tmp_string_borrow_cstring_result_n1118) != HXC_STATUS_OK)
        {
          abort();
        }
        int32_t hxc_l_tmp_load_result_n1119 = hxc_l_x_hde31d3c7a105;
        DrawText(hxc_l_tmp_string_borrow_cstring_result_n1118.data, (int32_t)hxc_l_tmp_load_result_n1119, (int32_t)52, (int32_t)34, hxc_l_this1_h0a9c3a675edf);
        if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n347) != HXC_STATUS_OK)
        {
          abort();
        }
      }
      int32_t hxc_l_firstTop = hxc_i32_subtract_wrapping(hxc_l_height_h0d2fc182597e, 293);
      int32_t hxc_l_left_h98f3d9115e9e = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_l_width_ha3da84aa5122, (double)2)), 180);
      if (hxc_l_selectedMode == 0)
      {
        struct Color hxc_l_this1_h78008170ca66 = (struct Color){ .r = 16, .g = 88, .b = 102, .a = 232 };
        int32_t hxc_l_tmp_load_result_n1125 = hxc_l_left_h98f3d9115e9e;
        int32_t hxc_l_tmp_load_result_n1126 = hxc_l_firstTop;
        DrawRectangle((int32_t)hxc_l_tmp_load_result_n1125, (int32_t)hxc_l_tmp_load_result_n1126, (int32_t)360, (int32_t)54, hxc_l_this1_h78008170ca66);
        struct Color hxc_l_this1_h7fb0ccf6eb78 = (struct Color){ .r = 255, .g = 190, .b = 55, .a = 255 };
        int32_t hxc_l_tmp_load_result_n1129 = hxc_l_left_h98f3d9115e9e;
        int32_t hxc_l_tmp_load_result_n1130 = hxc_l_firstTop;
        DrawRectangleLines((int32_t)hxc_l_tmp_load_result_n1129, (int32_t)hxc_l_tmp_load_result_n1130, (int32_t)360, (int32_t)54, hxc_l_this1_h7fb0ccf6eb78);
      }
      else
      {
        struct Color hxc_l_this1_h120a91275d17 = (struct Color){ .r = 6, .g = 26, .b = 36, .a = 218 };
        int32_t hxc_l_tmp_load_result_n1133 = hxc_l_left_h98f3d9115e9e;
        int32_t hxc_l_tmp_load_result_n1134 = hxc_l_firstTop;
        DrawRectangle((int32_t)hxc_l_tmp_load_result_n1133, (int32_t)hxc_l_tmp_load_result_n1134, (int32_t)360, (int32_t)54, hxc_l_this1_h120a91275d17);
        struct Color hxc_l_this1_hdd5276323e50 = (struct Color){ .r = 92, .g = 194, .b = 188, .a = 255 };
        int32_t hxc_l_tmp_load_result_n1137 = hxc_l_left_h98f3d9115e9e;
        int32_t hxc_l_tmp_load_result_n1138 = hxc_l_firstTop;
        DrawRectangleLines((int32_t)hxc_l_tmp_load_result_n1137, (int32_t)hxc_l_tmp_load_result_n1138, (int32_t)360, (int32_t)54, hxc_l_this1_hdd5276323e50);
      }
      struct Color hxc_l_this1_hfae3d0c5ee8d = (struct Color){ .r = 242, .g = 249, .b = 245, .a = 255 };
      struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_tmp_load_result_n1141 = hxc_l_uiCatalog;
      hxc_l_gc_roots[207] = (const void *)hxc_l_tmp_load_result_n1141;
      if (hxc_l_tmp_load_result_n1141 == NULL)
      {
        abort();
      }
      hxc_string hxc_l_tmp_instance_call_result_n1143 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_tmp_load_result_n1141, hxc_l_locale, 27);
      hxc_l_tmp_cstring_ref_owner_owner_n355 = hxc_l_tmp_instance_call_result_n1143;
      hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n1145 = HXC_BORROWED_CSTRING_INITIALIZER;
      if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n355, &hxc_l_tmp_string_borrow_cstring_result_n1145) != HXC_STATUS_OK)
      {
        abort();
      }
      int32_t hxc_l_tmp_load_result_n1146 = hxc_l_left_h98f3d9115e9e;
      int32_t hxc_l_tmp_load_result_n1147 = hxc_l_firstTop;
      DrawText(hxc_l_tmp_string_borrow_cstring_result_n1145.data, (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n1146, 28), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n1147, 16), (int32_t)21, hxc_l_this1_hfae3d0c5ee8d);
      int32_t hxc_l_top_he0050e91c1f0 = hxc_i32_add_wrapping(hxc_i32_add_wrapping(hxc_l_firstTop, 54), 14);
      int32_t hxc_l_left_h3f3e41e62a00 = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_l_width_ha3da84aa5122, (double)2)), 180);
      if (hxc_l_selectedMode == 1)
      {
        struct Color hxc_l_this1_haa1b293590d2 = (struct Color){ .r = 16, .g = 88, .b = 102, .a = 232 };
        int32_t hxc_l_tmp_load_result_n1153 = hxc_l_left_h3f3e41e62a00;
        int32_t hxc_l_tmp_load_result_n1154 = hxc_l_top_he0050e91c1f0;
        DrawRectangle((int32_t)hxc_l_tmp_load_result_n1153, (int32_t)hxc_l_tmp_load_result_n1154, (int32_t)360, (int32_t)54, hxc_l_this1_haa1b293590d2);
        struct Color hxc_l_this1_h467e2c132d4e = (struct Color){ .r = 255, .g = 190, .b = 55, .a = 255 };
        int32_t hxc_l_tmp_load_result_n1157 = hxc_l_left_h3f3e41e62a00;
        int32_t hxc_l_tmp_load_result_n1158 = hxc_l_top_he0050e91c1f0;
        DrawRectangleLines((int32_t)hxc_l_tmp_load_result_n1157, (int32_t)hxc_l_tmp_load_result_n1158, (int32_t)360, (int32_t)54, hxc_l_this1_h467e2c132d4e);
      }
      else
      {
        struct Color hxc_l_this1_h0714c48529f6 = (struct Color){ .r = 6, .g = 26, .b = 36, .a = 218 };
        int32_t hxc_l_tmp_load_result_n1161 = hxc_l_left_h3f3e41e62a00;
        int32_t hxc_l_tmp_load_result_n1162 = hxc_l_top_he0050e91c1f0;
        DrawRectangle((int32_t)hxc_l_tmp_load_result_n1161, (int32_t)hxc_l_tmp_load_result_n1162, (int32_t)360, (int32_t)54, hxc_l_this1_h0714c48529f6);
        struct Color hxc_l_this1_hd44d6dd79c2d = (struct Color){ .r = 92, .g = 194, .b = 188, .a = 255 };
        int32_t hxc_l_tmp_load_result_n1165 = hxc_l_left_h3f3e41e62a00;
        int32_t hxc_l_tmp_load_result_n1166 = hxc_l_top_he0050e91c1f0;
        DrawRectangleLines((int32_t)hxc_l_tmp_load_result_n1165, (int32_t)hxc_l_tmp_load_result_n1166, (int32_t)360, (int32_t)54, hxc_l_this1_hd44d6dd79c2d);
      }
      struct Color hxc_l_this1_h7d7bc1f45994 = (struct Color){ .r = 242, .g = 249, .b = 245, .a = 255 };
      struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_tmp_load_result_n1169 = hxc_l_uiCatalog;
      hxc_l_gc_roots[208] = (const void *)hxc_l_tmp_load_result_n1169;
      if (hxc_l_tmp_load_result_n1169 == NULL)
      {
        abort();
      }
      hxc_string hxc_l_tmp_instance_call_result_n1171 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_tmp_load_result_n1169, hxc_l_locale, 26);
      hxc_l_tmp_cstring_ref_owner_owner_n363 = hxc_l_tmp_instance_call_result_n1171;
      hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n1173 = HXC_BORROWED_CSTRING_INITIALIZER;
      if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n363, &hxc_l_tmp_string_borrow_cstring_result_n1173) != HXC_STATUS_OK)
      {
        abort();
      }
      int32_t hxc_l_tmp_load_result_n1174 = hxc_l_left_h3f3e41e62a00;
      int32_t hxc_l_tmp_load_result_n1175 = hxc_l_top_he0050e91c1f0;
      DrawText(hxc_l_tmp_string_borrow_cstring_result_n1173.data, (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n1174, 28), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n1175, 16), (int32_t)21, hxc_l_this1_h7d7bc1f45994);
      int32_t hxc_l_top_hbb537468ac97 = hxc_i32_add_wrapping(hxc_l_firstTop, 136);
      int32_t hxc_l_left_hcddc22377f80 = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_l_width_ha3da84aa5122, (double)2)), 180);
      struct Color hxc_l_this1_ha5c613b5a867 = (struct Color){ .r = 6, .g = 26, .b = 36, .a = 218 };
      int32_t hxc_l_tmp_load_result_n1180 = hxc_l_left_hcddc22377f80;
      int32_t hxc_l_tmp_load_result_n1181 = hxc_l_top_hbb537468ac97;
      DrawRectangle((int32_t)hxc_l_tmp_load_result_n1180, (int32_t)hxc_l_tmp_load_result_n1181, (int32_t)360, (int32_t)54, hxc_l_this1_ha5c613b5a867);
      struct Color hxc_l_this1_hfd769f88defc = (struct Color){ .r = 92, .g = 194, .b = 188, .a = 255 };
      int32_t hxc_l_tmp_load_result_n1184 = hxc_l_left_hcddc22377f80;
      int32_t hxc_l_tmp_load_result_n1185 = hxc_l_top_hbb537468ac97;
      DrawRectangleLines((int32_t)hxc_l_tmp_load_result_n1184, (int32_t)hxc_l_tmp_load_result_n1185, (int32_t)360, (int32_t)54, hxc_l_this1_hfd769f88defc);
      struct Color hxc_l_this1_h2bf46de51f33 = (struct Color){ .r = 242, .g = 249, .b = 245, .a = 255 };
      struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_tmp_load_result_n1188 = hxc_l_uiCatalog;
      hxc_l_gc_roots[209] = (const void *)hxc_l_tmp_load_result_n1188;
      if (hxc_l_tmp_load_result_n1188 == NULL)
      {
        abort();
      }
      hxc_string hxc_l_tmp_instance_call_result_n1190 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_tmp_load_result_n1188, hxc_l_locale, 28);
      hxc_l_tmp_cstring_ref_owner_owner_n369 = hxc_l_tmp_instance_call_result_n1190;
      hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n1192 = HXC_BORROWED_CSTRING_INITIALIZER;
      if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n369, &hxc_l_tmp_string_borrow_cstring_result_n1192) != HXC_STATUS_OK)
      {
        abort();
      }
      int32_t hxc_l_tmp_load_result_n1193 = hxc_l_left_hcddc22377f80;
      int32_t hxc_l_tmp_load_result_n1194 = hxc_l_top_hbb537468ac97;
      DrawText(hxc_l_tmp_string_borrow_cstring_result_n1192.data, (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n1193, 28), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n1194, 16), (int32_t)21, hxc_l_this1_h2bf46de51f33);
      int32_t hxc_l_x_hf86d81030467 = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_l_width_ha3da84aa5122, (double)2)), 285);
      struct Color hxc_l_this1_hada760542d55 = (struct Color){ .r = 229, .g = 241, .b = 235, .a = 255 };
      struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_tmp_load_result_n1198 = hxc_l_uiCatalog;
      hxc_l_gc_roots[210] = (const void *)hxc_l_tmp_load_result_n1198;
      if (hxc_l_tmp_load_result_n1198 == NULL)
      {
        abort();
      }
      hxc_string hxc_l_tmp_instance_call_result_n1200 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_tmp_load_result_n1198, hxc_l_locale, 29);
      hxc_l_tmp_cstring_ref_owner_owner_n372 = hxc_l_tmp_instance_call_result_n1200;
      hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n1202 = HXC_BORROWED_CSTRING_INITIALIZER;
      if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n372, &hxc_l_tmp_string_borrow_cstring_result_n1202) != HXC_STATUS_OK)
      {
        abort();
      }
      int32_t hxc_l_tmp_load_result_n1203 = hxc_l_x_hf86d81030467;
      int32_t hxc_l_tmp_load_result_n1204 = hxc_l_height_h0d2fc182597e;
      DrawText(hxc_l_tmp_string_borrow_cstring_result_n1202.data, (int32_t)hxc_l_tmp_load_result_n1203, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n1204, 58), (int32_t)16, hxc_l_this1_hada760542d55);
      if (hxc_l_selectedMode == 1)
      {
        int32_t hxc_l_x_hc8caa767bc01 = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_l_width_ha3da84aa5122, (double)2)), 230);
        struct Color hxc_l_this1_h075d852a32a1 = (struct Color){ .r = 255, .g = 205, .b = 91, .a = 255 };
        const char *hxc_l_tmp_call_result_n1210 = hxc_caxecraft_localization_FirstPlayableCatalog_text(hxc_l_locale, 1);
        int32_t hxc_l_tmp_load_result_n1211 = hxc_l_x_hc8caa767bc01;
        int32_t hxc_l_tmp_load_result_n1212 = hxc_l_firstTop;
        DrawText(hxc_l_tmp_call_result_n1210, (int32_t)hxc_l_tmp_load_result_n1211, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n1212, 36), (int32_t)17, hxc_l_this1_h075d852a32a1);
      }
      if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n372) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n369) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n363) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n355) != HXC_STATUS_OK)
      {
        abort();
      }
    }
    else
    {
      if (hxc_l_onCampaignSelect)
      {
        hxc_l_gc_roots[211] = (const void *)hxc_l_campaign;
        struct hxc_caxecraft_content_CampaignManifest *hxc_l_selectedCampaign_h73568f8e60dc = hxc_l_campaign;
        hxc_l_gc_roots[212] = (const void *)hxc_l_selectedCampaign_h73568f8e60dc;
        hxc_l_gc_roots[213] = (const void *)NULL;
        if (hxc_l_selectedCampaign_h73568f8e60dc == NULL)
        {
          enum hxc_caxecraft_app_AppScreen hxc_l_tmp_call_result_n1218 = hxc_caxecraft_app_AppScreen_closeCampaignSelection(hxc_l_screen);
          hxc_l_screen = hxc_l_tmp_call_result_n1218;
        }
        else
        {
          struct Texture hxc_l_tmp_load_result_n1219 = hxc_l_titleTexture;
          bool hxc_l_tmp_load_result_n1220 = hxc_l_titleTextureReady;
          struct Texture hxc_l_tmp_load_result_n1221 = hxc_l_wordmarkTexture;
          bool hxc_l_tmp_load_result_n1222 = hxc_l_wordmarkTextureReady;
          struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n1223 = hxc_l_selectedCampaign_h73568f8e60dc;
          hxc_l_gc_roots[214] = (const void *)hxc_l_tmp_load_result_n1223;
          int32_t hxc_l_tmp_load_result_n1224 = hxc_l_locale;
          struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_tmp_load_result_n1225 = hxc_l_uiCatalog;
          hxc_l_gc_roots[215] = (const void *)hxc_l_tmp_load_result_n1225;
          hxc_caxecraft_app_CampaignMenu_draw(hxc_l_tmp_load_result_n1219, hxc_l_tmp_load_result_n1220, hxc_l_tmp_load_result_n1221, hxc_l_tmp_load_result_n1222, hxc_l_tmp_load_result_n1223, hxc_l_tmp_load_result_n1224, hxc_l_tmp_load_result_n1225, hxc_l_selectedCampaignLevelIndex);
        }
      }
      else
      {
        if (hxc_l_onEditor)
        {
          struct hxc_caxecraft_app_CaxecraftEditorScreen *hxc_l_tmp_load_result_n1228 = hxc_l_editorScreen;
          hxc_l_gc_roots[216] = (const void *)hxc_l_tmp_load_result_n1228;
          if (hxc_l_tmp_load_result_n1228 == NULL)
          {
            abort();
          }
          int32_t hxc_l_tmp_load_result_n1229 = hxc_l_locale;
          enum hxc_caxecraft_app_EditorScreenAction hxc_l_tmp_instance_call_result_n1231 = hxc_caxecraft_app_CaxecraftEditorScreen_draw(hxc_l_tmp_load_result_n1228, hxc_l_tmp_load_result_n1229, hxc_l_editorNavigationCommand);
          if (hxc_l_tmp_instance_call_result_n1231 == hxc_caxecraft_app_EditorScreenAction_ReturnToTitle)
          {
            enum hxc_caxecraft_app_AppScreen hxc_l_tmp_call_result_n1234 = hxc_caxecraft_app_AppScreen_closeEditor(hxc_l_screen);
            hxc_l_screen = hxc_l_tmp_call_result_n1234;
          }
        }
        else
        {
          ClearBackground((struct Color){ .r = 126, .g = 190, .b = 201, .a = 255 });
          int32_t hxc_l_tmp_native_call_result_n1236 = GetScreenWidth();
          int32_t hxc_l_sunX = hxc_i32_subtract_wrapping((int32_t)hxc_l_tmp_native_call_result_n1236, 260);
          float hxc_l_radius_h4f1760c0c0b3 = (float)42.0;
          struct Color hxc_l_this1_hd3da2428b08f = (struct Color){ .r = 255, .g = 226, .b = 136, .a = 255 };
          int32_t hxc_l_tmp_load_result_n1238 = hxc_l_sunX;
          float hxc_l_tmp_load_result_n1239 = hxc_l_radius_h4f1760c0c0b3;
          DrawCircle((int32_t)hxc_l_tmp_load_result_n1238, (int32_t)86, hxc_l_tmp_load_result_n1239, hxc_l_this1_hd3da2428b08f);
          float hxc_l_radius_he12995c062b8 = (float)30.0;
          struct Color hxc_l_this1_h9b7d45581483 = (struct Color){ .r = 255, .g = 246, .b = 205, .a = 255 };
          int32_t hxc_l_tmp_load_result_n1242 = hxc_l_sunX;
          float hxc_l_tmp_load_result_n1243 = hxc_l_radius_he12995c062b8;
          DrawCircle((int32_t)hxc_l_tmp_load_result_n1242, (int32_t)86, hxc_l_tmp_load_result_n1243, hxc_l_this1_h9b7d45581483);
          BeginMode3D(hxc_l_camera);
          if (hxc_l_self == NULL)
          {
            abort();
          }
          struct hxc_caxecraft_app_TerrainRenderer *hxc_l_tmp_owned_class_field_address_n1246 = &(*hxc_l_self).hxc_terrainRenderer;
          if (hxc_l_tmp_owned_class_field_address_n1246 == NULL)
          {
            abort();
          }
          struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1247 = hxc_l_session;
          hxc_l_gc_roots[217] = (const void *)hxc_l_tmp_load_result_n1247;
          if (hxc_l_tmp_load_result_n1247 == NULL)
          {
            abort();
          }
          size_t hxc_l_tmp_length_n1249;
          const uint8_t *hxc_l_tmp_instance_call_result_n1248 = hxc_caxecraft_domain_GameSession_worldView(hxc_l_tmp_load_result_n1247, &hxc_l_tmp_length_n1249);
          struct Texture hxc_l_tmp_load_result_n1250 = hxc_l_terrainTexture;
          bool hxc_l_tmp_load_result_n1251 = hxc_l_terrainTextureReady;
          struct Texture hxc_l_tmp_load_result_n1252 = hxc_l_adventureTerrainTexture;
          bool hxc_l_tmp_load_result_n1253 = hxc_l_adventureTerrainTextureReady;
          double hxc_l_tmp_record_field_load_result_n1254 = hxc_l_renderPosition.hxc_x;
          struct hxc_caxecraft_app_TerrainRenderCounters hxc_l_tmp_instance_call_result_n1256 = hxc_caxecraft_app_TerrainRenderer_draw(hxc_l_tmp_owned_class_field_address_n1246, hxc_l_tmp_instance_call_result_n1248, hxc_l_tmp_length_n1249, hxc_l_tmp_load_result_n1250, hxc_l_tmp_load_result_n1251, hxc_l_tmp_load_result_n1252, hxc_l_tmp_load_result_n1253, hxc_l_tmp_record_field_load_result_n1254, hxc_l_renderPosition.hxc_z);
          struct hxc_caxecraft_app_TerrainRenderCounters hxc_l_renderCounters = hxc_l_tmp_instance_call_result_n1256;
          struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1257 = hxc_l_session;
          hxc_l_gc_roots[218] = (const void *)hxc_l_tmp_load_result_n1257;
          if (hxc_l_tmp_load_result_n1257 == NULL)
          {
            abort();
          }
          size_t hxc_l_tmp_length_n1259;
          const uint8_t *hxc_l_tmp_instance_call_result_n1258 = hxc_caxecraft_domain_GameSession_worldView(hxc_l_tmp_load_result_n1257, &hxc_l_tmp_length_n1259);
          struct Texture hxc_l_tmp_load_result_n1260 = hxc_l_terrainTexture;
          bool hxc_l_tmp_load_result_n1261 = hxc_l_terrainTextureReady;
          struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1262 = hxc_l_levelView;
          hxc_l_gc_roots[219] = (const void *)hxc_l_tmp_load_result_n1262;
          if (hxc_l_tmp_load_result_n1262 == NULL)
          {
            abort();
          }
          int32_t hxc_l_tmp_class_field_load_result_n1263 = (*hxc_l_tmp_load_result_n1262).hxc_waterCell;
          struct hxc_caxecraft_app_WaterRenderCounters hxc_l_tmp_call_result_n1264 = hxc_caxecraft_app_WaterRenderer_draw(hxc_l_tmp_instance_call_result_n1258, hxc_l_tmp_length_n1259, hxc_l_tmp_load_result_n1260, hxc_l_tmp_load_result_n1261, hxc_l_tmp_class_field_load_result_n1263);
          struct hxc_caxecraft_app_WaterRenderCounters hxc_l_waterCounters = hxc_l_tmp_call_result_n1264;
          int32_t hxc_l_tmp_record_field_load_result_n1265 = hxc_l_renderCounters.hxc_visible;
          int32_t hxc_l_totalVisible = hxc_i32_add_wrapping(hxc_l_tmp_record_field_load_result_n1265, hxc_l_waterCounters.hxc_visible);
          int32_t hxc_l_tmp_record_field_load_result_n1267 = hxc_l_renderCounters.hxc_drawCalls;
          int32_t hxc_l_totalDrawCalls = hxc_i32_add_wrapping(hxc_l_tmp_record_field_load_result_n1267, hxc_l_waterCounters.hxc_drawCalls);
          struct Camera3D hxc_l_tmp_load_result_n1269 = hxc_l_camera;
          struct Texture hxc_l_tmp_load_result_n1270 = hxc_l_entityTexture;
          bool hxc_l_tmp_load_result_n1271 = hxc_l_entityTextureReady;
          struct hxc_caxecraft_domain_Character hxc_l_tmp_load_result_n1272 = hxc_l_dialogueActor;
          struct hxc_caxecraft_domain_Character hxc_l_tmp_load_result_n1273 = hxc_l_enemyActor;
          enum hxc_caxecraft_domain_ActorControllerPhase hxc_l_tmp_record_field_load_result_n1274 = hxc_l_enemyPhase_ha80e5c596f34.hxc_phase;
          hxc_caxecraft_app_CaxecraftApp_drawActors(hxc_l_tmp_load_result_n1269, hxc_l_tmp_load_result_n1270, hxc_l_tmp_load_result_n1271, hxc_l_tmp_load_result_n1272, hxc_l_tmp_load_result_n1273, hxc_l_tmp_record_field_load_result_n1274, hxc_l_berryDrop);
          struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_tmp_load_result_n1276 = hxc_l_contentRegistry;
          hxc_l_gc_roots[220] = (const void *)hxc_l_tmp_load_result_n1276;
          struct Camera3D hxc_l_tmp_load_result_n1277 = hxc_l_camera;
          struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1278 = hxc_l_session;
          hxc_l_gc_roots[221] = (const void *)hxc_l_tmp_load_result_n1278;
          if (hxc_l_tmp_load_result_n1278 == NULL)
          {
            abort();
          }
          size_t hxc_l_tmp_length_n1280;
          const int32_t *hxc_l_tmp_instance_call_result_n1279 = hxc_caxecraft_domain_GameSession_authoredItemsView(hxc_l_tmp_load_result_n1278, &hxc_l_tmp_length_n1280);
          struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1281 = hxc_l_levelView;
          hxc_l_gc_roots[222] = (const void *)hxc_l_tmp_load_result_n1281;
          struct Texture hxc_l_tmp_load_result_n1282 = hxc_l_itemTexture;
          bool hxc_l_tmp_load_result_n1283 = hxc_l_itemTextureReady;
          struct Texture hxc_l_tmp_load_result_n1284 = hxc_l_adventureItemTexture;
          hxc_caxecraft_app_AuthoredItemRenderer_drawWorldItems(hxc_l_tmp_load_result_n1276, hxc_l_tmp_load_result_n1277, hxc_l_tmp_instance_call_result_n1279, hxc_l_tmp_length_n1280, hxc_l_tmp_load_result_n1281, hxc_l_tmp_load_result_n1282, hxc_l_tmp_load_result_n1283, hxc_l_tmp_load_result_n1284, hxc_l_adventureItemTextureReady);
          if (hxc_l_hit.hxc_hit)
          {
            double hxc_l_y = (double)hxc_l_hit.hxc_cellY + 0.5;
            double hxc_l_z = (double)hxc_l_hit.hxc_cellZ + 0.5;
            int32_t hxc_l_tmp_record_field_load_result_n1289 = hxc_l_hit.hxc_cellX;
            double hxc_l_tmp_load_result_n1290 = hxc_l_y;
            DrawCubeWires((struct Vector3){ .x = (float)((double)hxc_l_tmp_record_field_load_result_n1289 + 0.5), .y = (float)hxc_l_tmp_load_result_n1290, .z = (float)hxc_l_z }, (float)1.04, (float)1.04, (float)1.04, (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 });
          }
          EndMode3D();
          if (hxc_l_cameraWaterBlend > 0.0)
          {
            int32_t hxc_l_overlayAlpha = hxc_f64_to_i32_saturating(105.0 * hxc_l_cameraWaterBlend);
            int32_t hxc_l_tmp_native_call_result_n1296 = GetScreenWidth();
            int32_t hxc_l_width_h8fba6d8df16b = (int32_t)hxc_l_tmp_native_call_result_n1296;
            int32_t hxc_l_tmp_native_call_result_n1297 = GetScreenHeight();
            int32_t hxc_l_height_hc7446b411100 = (int32_t)hxc_l_tmp_native_call_result_n1297;
            uint8_t hxc_l_red = (uint8_t)31;
            uint8_t hxc_l_green = (uint8_t)115;
            uint8_t hxc_l_blue = (uint8_t)154;
            int32_t hxc_l_tmp_load_result_n1298 = hxc_l_overlayAlpha;
            int32_t hxc_l_tmp_conditional_result_n394 = 0;
            if (hxc_l_tmp_load_result_n1298 < 0)
            {
              hxc_l_tmp_conditional_result_n394 = 0;
            }
            else
            {
              int32_t hxc_l_tmp_load_result_n1299 = hxc_l_overlayAlpha;
              int32_t hxc_l_tmp_conditional_result_n395 = 0;
              if (hxc_l_tmp_load_result_n1299 > 255)
              {
                hxc_l_tmp_conditional_result_n395 = 255;
              }
              else
              {
                hxc_l_tmp_conditional_result_n395 = hxc_l_overlayAlpha;
              }
              hxc_l_tmp_conditional_result_n394 = hxc_l_tmp_conditional_result_n395;
            }
            uint8_t hxc_l_alpha = (uint8_t)hxc_l_tmp_conditional_result_n394;
            uint8_t hxc_l_tmp_load_result_n1303 = hxc_l_red;
            uint8_t hxc_l_tmp_load_result_n1304 = hxc_l_green;
            uint8_t hxc_l_tmp_load_result_n1305 = hxc_l_blue;
            struct Color hxc_l_this1_hb97b6818e78a = (struct Color){ .r = hxc_l_tmp_load_result_n1303, .g = hxc_l_tmp_load_result_n1304, .b = hxc_l_tmp_load_result_n1305, .a = hxc_l_alpha };
            int32_t hxc_l_tmp_load_result_n1308 = hxc_l_width_h8fba6d8df16b;
            int32_t hxc_l_tmp_load_result_n1309 = hxc_l_height_hc7446b411100;
            DrawRectangle((int32_t)0, (int32_t)0, (int32_t)hxc_l_tmp_load_result_n1308, (int32_t)hxc_l_tmp_load_result_n1309, hxc_l_this1_hb97b6818e78a);
          }
          double hxc_l_tmp_record_field_load_result_n1311 = hxc_l_character.hxc_body.hxc_x;
          double hxc_l_tmp_record_field_load_result_n1312 = hxc_l_character.hxc_body.hxc_z;
          struct hxc_caxecraft_domain_VitalsState hxc_l_tmp_record_field_load_result_n1313 = hxc_l_character.hxc_vitals;
          int32_t hxc_l_tmp_load_result_n1314 = hxc_l_aquaticEquipmentCode;
          int32_t hxc_l_tmp_load_result_n1315 = hxc_l_aquaticEquipmentFrames;
          bool hxc_l_tmp_record_field_load_result_n1316 = hxc_l_character.hxc_aquatic.hxc_headSubmerged;
          int32_t hxc_l_tmp_record_field_load_result_n1317 = hxc_l_character.hxc_aquatic.hxc_breathTicks;
          struct hxc_caxecraft_app_HudCharacterView hxc_l_hudView_hcbf9bb7e6fab = (struct hxc_caxecraft_app_HudCharacterView){ .hxc_aquaticEquipmentCode = hxc_l_tmp_load_result_n1314, .hxc_aquaticEquipmentVisible = hxc_l_tmp_load_result_n1315 > 0, .hxc_breathTicks = hxc_l_tmp_record_field_load_result_n1317, .hxc_headSubmerged = hxc_l_tmp_record_field_load_result_n1316, .hxc_maximumBreathTicks = hxc_l_character.hxc_aquaticProfile.hxc_maximumBreathTicks, .hxc_vitals = hxc_l_tmp_record_field_load_result_n1313, .hxc_x = hxc_l_tmp_record_field_load_result_n1311, .hxc_z = hxc_l_tmp_record_field_load_result_n1312 };
          bool hxc_l_hudView1 = hxc_l_placementBlockedFrames > 0;
          bool hxc_l_hudView2 = hxc_l_strikeHitFrames > 0;
          bool hxc_l_hudView3 = hxc_l_enemyDefeatedFrames > 0;
          bool hxc_l_hudView4 = hxc_l_enemyAttackFrames > 0;
          bool hxc_l_hudView5 = hxc_l_pickupFrames > 0;
          bool hxc_l_hudView6 = hxc_l_recoveryFeedbackFrames > 0;
          enum hxc_caxecraft_domain_ActorControllerPhase hxc_l_enemyPhase_h7d29b09afefe = hxc_l_enemyPhase_ha80e5c596f34.hxc_phase;
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
          hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n1328 = hxc_l_locale1;
          hxc_l_tmp_managed_flow_owner_n406 = hxc_l_tmp_managed_flow_carrier_move_result_n1328;
          struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1329 = hxc_l_levelView;
          hxc_l_gc_roots[223] = (const void *)hxc_l_tmp_load_result_n1329;
          if (hxc_l_tmp_load_result_n1329 == NULL)
          {
            abort();
          }
          struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_class_field_load_result_n1330 = (*hxc_l_tmp_load_result_n1329).hxc_levelPresentation;
          hxc_l_gc_roots[224] = (const void *)hxc_l_tmp_class_field_load_result_n1330;
          struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_this = hxc_l_tmp_class_field_load_result_n1330;
          struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_load_result_n1331 = hxc_l_this;
          hxc_l_gc_roots[225] = (const void *)hxc_l_tmp_load_result_n1331;
          if (hxc_l_tmp_load_result_n1331 == NULL)
          {
            abort();
          }
          struct hxc_optional_caxecraft_scenario_ScenarioText hxc_l_tmp_class_field_load_result_n1332 = (*hxc_l_tmp_load_result_n1331).hxc_objectiveTitle;
          hxc_string hxc_l_tmp_conditional_managed_result_n409 = { 0 };
          if (!hxc_l_tmp_class_field_load_result_n1332.hxc_has_value)
          {
            hxc_l_tmp_conditional_managed_result_n409 = (hxc_string){ (const uint8_t *)"", 0, true, NULL };
            if (hxc_string_retain(hxc_l_tmp_conditional_managed_result_n409) != HXC_STATUS_OK)
            {
              abort();
            }
          }
          else
          {
            struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_load_result_n1333 = hxc_l_this;
            hxc_l_gc_roots[226] = (const void *)hxc_l_tmp_load_result_n1333;
            if (hxc_l_tmp_load_result_n1333 == NULL)
            {
              abort();
            }
            struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_load_result_n1334 = hxc_l_this;
            hxc_l_gc_roots[227] = (const void *)hxc_l_tmp_load_result_n1334;
            if (hxc_l_tmp_load_result_n1334 == NULL)
            {
              abort();
            }
            struct hxc_optional_caxecraft_scenario_ScenarioText hxc_l_tmp_class_field_load_result_n1335 = (*hxc_l_tmp_load_result_n1334).hxc_objectiveTitle;
            if (!hxc_l_tmp_class_field_load_result_n1335.hxc_has_value)
            {
              abort();
            }
            hxc_string hxc_l_tmp_instance_call_result_n1338 = hxc_caxecraft_content_RuntimeLevelPresentation_resolve(hxc_l_tmp_load_result_n1333, hxc_l_tmp_class_field_load_result_n1335.hxc_value, hxc_l_tmp_managed_flow_owner_n406);
            hxc_l_tmp_conditional_managed_result_n409 = hxc_l_tmp_instance_call_result_n1338;
          }
          hxc_string hxc_l_tmp_conditional_managed_move_result_n1339 = hxc_l_tmp_conditional_managed_result_n409;
          hxc_l_hudView7 = hxc_l_tmp_conditional_managed_move_result_n1339;
          int32_t hxc_l_tmp_load_result_n1340 = hxc_l_totalVisible;
          int32_t hxc_l_tmp_load_result_n1341 = hxc_l_totalDrawCalls;
          int32_t hxc_l_tmp_load_result_n1342 = hxc_l_frameCount;
          int32_t hxc_l_tmp_load_result_n1343 = hxc_l_completedTicks;
          struct hxc_caxecraft_app_HudCharacterView hxc_l_tmp_load_result_n1345 = hxc_l_hudView_hcbf9bb7e6fab;
          bool hxc_l_tmp_load_result_n1346 = hxc_l_hudView1;
          bool hxc_l_tmp_load_result_n1347 = hxc_l_hudView2;
          bool hxc_l_tmp_load_result_n1348 = hxc_l_hudView3;
          bool hxc_l_tmp_load_result_n1349 = hxc_l_hudView4;
          bool hxc_l_tmp_load_result_n1350 = hxc_l_hudView5;
          int32_t hxc_l_tmp_load_result_n1351 = hxc_l_pickupAmount;
          int32_t hxc_l_tmp_load_result_n1352 = hxc_l_inventoryFullReason;
          int32_t hxc_l_tmp_load_result_n1353 = hxc_l_recoveryFeedback;
          bool hxc_l_tmp_load_result_n1354 = hxc_l_hudView6;
          bool hxc_l_tmp_load_result_n1356 = hxc_l_paused_h9ad6cd97e1b1;
          bool hxc_l_tmp_load_result_n1357 = hxc_l_captured_h4e575de23f09;
          struct hxc_caxecraft_domain_RaycastHit hxc_l_tmp_load_result_n1358 = hxc_l_hit;
          int32_t hxc_l_tmp_load_result_n1359 = hxc_l_selectedMode;
          int32_t hxc_l_tmp_load_result_n1360 = hxc_l_locale;
          struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_load_result_n1361 = hxc_l_inventory;
          enum hxc_caxecraft_gameplay_GuidePhase hxc_l_tmp_load_result_n1362 = hxc_l_guidePhase;
          bool hxc_l_tmp_load_result_n1363 = hxc_l_guideInteractionAvailable;
          struct hxc_caxecraft_domain_Character hxc_l_tmp_load_result_n1364 = hxc_l_enemyActor;
          enum hxc_caxecraft_domain_ActorControllerPhase hxc_l_tmp_load_result_n1365 = hxc_l_enemyPhase_h7d29b09afefe;
          hxc_string hxc_l_tmp_record_field_levelLabel_owner_n411 = hxc_l_levelLabel;
          if (hxc_string_retain(hxc_l_tmp_record_field_levelLabel_owner_n411) != HXC_STATUS_OK)
          {
            abort();
          }
          hxc_string hxc_l_tmp_record_field_levelLabel_owned_load_result_n1367 = hxc_l_tmp_record_field_levelLabel_owner_n411;
          hxc_string hxc_l_tmp_record_field_objectiveTitle_owner_n412 = hxc_l_hudView7;
          if (hxc_string_retain(hxc_l_tmp_record_field_objectiveTitle_owner_n412) != HXC_STATUS_OK)
          {
            abort();
          }
          hxc_l_hudView_h74858b86ffca = (struct hxc_caxecraft_app_HudView){ .hxc_character = hxc_l_tmp_load_result_n1345, .hxc_enemy = hxc_l_tmp_load_result_n1364, .hxc_enemyPhase = hxc_l_tmp_load_result_n1365, .hxc_feedback = (struct hxc_caxecraft_app_HudFeedbackView){ .hxc_enemyAttacked = hxc_l_tmp_load_result_n1349, .hxc_enemyDefeated = hxc_l_tmp_load_result_n1348, .hxc_inventoryFullReason = hxc_l_tmp_load_result_n1352, .hxc_pickedUp = hxc_l_tmp_load_result_n1350, .hxc_pickupAmount = hxc_l_tmp_load_result_n1351, .hxc_placementBlocked = hxc_l_tmp_load_result_n1346, .hxc_recoveryDecision = hxc_l_tmp_load_result_n1353, .hxc_recoveryVisible = hxc_l_tmp_load_result_n1354, .hxc_strikeHit = hxc_l_tmp_load_result_n1347 }, .hxc_guideInteractionAvailable = hxc_l_tmp_load_result_n1363, .hxc_guidePhase = hxc_l_tmp_load_result_n1362, .hxc_hit = hxc_l_tmp_load_result_n1358, .hxc_inventory = hxc_l_tmp_load_result_n1361, .hxc_levelLabel = hxc_l_tmp_record_field_levelLabel_owned_load_result_n1367, .hxc_locale = hxc_l_tmp_load_result_n1360, .hxc_metrics = (struct hxc_caxecraft_app_HudMetricsView){ .hxc_completedTicks = hxc_l_tmp_load_result_n1343, .hxc_drawCalls = hxc_l_tmp_load_result_n1341, .hxc_renderedFrames = hxc_l_tmp_load_result_n1342, .hxc_visibleBlocks = hxc_l_tmp_load_result_n1340 }, .hxc_mode = hxc_l_tmp_load_result_n1359, .hxc_objectiveTitle = hxc_l_tmp_record_field_objectiveTitle_owner_n412, .hxc_paused = hxc_l_tmp_load_result_n1356, .hxc_pointerCaptured = hxc_l_tmp_load_result_n1357 };
          struct hxc_caxecraft_app_HudView hxc_l_tmp_load_result_n1371 = hxc_l_hudView_h74858b86ffca;
          struct hxc_caxecraft_app_HudResources hxc_l_tmp_load_result_n1372 = hxc_l_hudResources;
          struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_tmp_load_result_n1373 = hxc_l_contentRegistry;
          hxc_l_gc_roots[228] = (const void *)hxc_l_tmp_load_result_n1373;
          hxc_l_gc_roots[229] = (const void *)hxc_l_uiCatalog;
          hxc_caxecraft_app_CaxecraftApp_drawHud(hxc_l_tmp_load_result_n1371, hxc_l_tmp_load_result_n1372, hxc_l_tmp_load_result_n1373, hxc_l_uiCatalog);
          hxc_record_9e5a983a_destroy(&hxc_l_hudView_h74858b86ffca);
          if (hxc_string_release(&hxc_l_hudView7) != HXC_STATUS_OK)
          {
            abort();
          }
          if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n406) != HXC_STATUS_OK)
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
