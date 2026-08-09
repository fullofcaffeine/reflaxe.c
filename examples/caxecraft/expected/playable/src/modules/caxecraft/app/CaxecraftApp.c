#include "hxc/program.h"

void hxc_compiler_constructor_caxecraft_app_CaxecraftApp(struct hxc_caxecraft_app_CaxecraftApp *hxc_l_self)
{
  struct hxc_caxecraft_app_WaterRenderer *hxc_l_tmp_owned_class_field_address_n0 = &(*hxc_l_self).hxc_waterRenderer;
  hxc_compiler_constructor_caxecraft_app_WaterRenderer(hxc_l_tmp_owned_class_field_address_n0);
  struct hxc_caxecraft_app_TerrainRenderer *hxc_l_tmp_owned_class_field_address_n1 = &(*hxc_l_self).hxc_terrainRenderer;
  hxc_compiler_constructor_caxecraft_app_TerrainRenderer(hxc_l_tmp_owned_class_field_address_n1);
  struct hxc_caxecraft_input_NavigationRepeater *hxc_l_tmp_owned_class_field_address_n2 = &(*hxc_l_self).hxc_editorNavigation;
  hxc_compiler_constructor_caxecraft_input_NavigationRepeater(hxc_l_tmp_owned_class_field_address_n2);
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
  const void *hxc_l_gc_roots[19] = { (const void *)hxc_l_view.hxc_presentation, (const void *)hxc_l_contentRegistry, (const void *)hxc_l_uiCatalog, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 19, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_string hxc_l_text_h5e4d2698c6aa = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n102 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n103 = { 0 };
  hxc_string hxc_l_text_h2969ab0ae2bb = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n107 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n108 = { 0 };
  hxc_string hxc_l_text_hf64b6b3f6e73 = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n112 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n113 = { 0 };
  hxc_string hxc_l_text_h36fcc5058efc = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n117 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n118 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n120 = { 0 };
  hxc_string hxc_l_text_h8a0e0a0381bb = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n124 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n125 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n127 = { 0 };
  hxc_string hxc_l_text1_hab565604a420 = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n133 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n134 = { 0 };
  hxc_string hxc_l_text_ha0346c749ef1 = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n138 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n139 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n142 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n143 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n145 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n146 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n147 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n55 = { 0 };
  hxc_string hxc_l_text_hf0cdd789724f = { 0 };
  hxc_string hxc_l_text1_h2dc3beea7319 = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n62 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n63 = { 0 };
  hxc_string hxc_l_text1_hd177824d8da0 = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n66 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n67 = { 0 };
  hxc_string hxc_l_text1_h240643b8a835 = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n70 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n71 = { 0 };
  hxc_string hxc_l_text_h6ad4c16fd72d = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n75 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n76 = { 0 };
  hxc_string hxc_l_text_h62a045f39fd0 = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n80 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n81 = { 0 };
  hxc_string hxc_l_text_hc2bd8c347136 = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n85 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n86 = { 0 };
  hxc_string hxc_l_text_he6b508ade1f1 = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n90 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n91 = { 0 };
  hxc_string hxc_l_text_hb97746981573 = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n95 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n96 = { 0 };
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
  hxc_l_gc_roots[3] = (const void *)hxc_l_view.hxc_presentation;
  struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_presentation = hxc_l_view.hxc_presentation;
  int32_t hxc_l_tmp_native_call_result_n55 = GetScreenWidth();
  int32_t hxc_l_width = (int32_t)hxc_l_tmp_native_call_result_n55;
  int32_t hxc_l_tmp_native_call_result_n56 = GetScreenHeight();
  int32_t hxc_l_height = (int32_t)hxc_l_tmp_native_call_result_n56;
  int32_t hxc_l_centerX = hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_l_width, (double)2));
  int32_t hxc_l_centerY = hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_l_height, (double)2));
  struct Color hxc_l_text_hd08b8496149a = (struct Color){ .r = 228, .g = 239, .b = 227, .a = 255 };
  int32_t hxc_l_tmp_load_result_n60 = hxc_l_centerX;
  int32_t hxc_l_tmp_load_result_n61 = hxc_l_centerY;
  int32_t hxc_l_tmp_load_result_n62 = hxc_l_centerX;
  int32_t hxc_l_tmp_load_result_n63 = hxc_l_centerY;
  DrawLine((int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n60, 8), (int32_t)hxc_l_tmp_load_result_n61, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n62, 3), (int32_t)hxc_l_tmp_load_result_n63, hxc_l_text_hd08b8496149a);
  int32_t hxc_l_tmp_load_result_n65 = hxc_l_centerX;
  int32_t hxc_l_tmp_load_result_n66 = hxc_l_centerY;
  int32_t hxc_l_tmp_load_result_n67 = hxc_l_centerX;
  int32_t hxc_l_tmp_load_result_n68 = hxc_l_centerY;
  DrawLine((int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n65, 3), (int32_t)hxc_l_tmp_load_result_n66, (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n67, 8), (int32_t)hxc_l_tmp_load_result_n68, hxc_l_text_hd08b8496149a);
  int32_t hxc_l_tmp_load_result_n70 = hxc_l_centerX;
  int32_t hxc_l_tmp_load_result_n71 = hxc_l_centerY;
  int32_t hxc_l_tmp_load_result_n72 = hxc_l_centerX;
  int32_t hxc_l_tmp_load_result_n73 = hxc_l_centerY;
  DrawLine((int32_t)hxc_l_tmp_load_result_n70, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n71, 8), (int32_t)hxc_l_tmp_load_result_n72, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n73, 3), hxc_l_text_hd08b8496149a);
  int32_t hxc_l_tmp_load_result_n75 = hxc_l_centerX;
  int32_t hxc_l_tmp_load_result_n76 = hxc_l_centerY;
  int32_t hxc_l_tmp_load_result_n77 = hxc_l_centerX;
  int32_t hxc_l_tmp_load_result_n78 = hxc_l_centerY;
  DrawLine((int32_t)hxc_l_tmp_load_result_n75, (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n76, 3), (int32_t)hxc_l_tmp_load_result_n77, (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n78, 8), hxc_l_text_hd08b8496149a);
  struct Color hxc_l_this1_h2c80e3b32f7b = (struct Color){ .r = 15, .g = 27, .b = 39, .a = 220 };
  DrawRectangle((int32_t)18, (int32_t)18, (int32_t)460, (int32_t)108, hxc_l_this1_h2c80e3b32f7b);
  struct Color hxc_l_this1_h195bd45cc370 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
  DrawRectangleLines((int32_t)18, (int32_t)18, (int32_t)460, (int32_t)108, hxc_l_this1_h195bd45cc370);
  if (hxc_l_uiCatalog == NULL)
  {
    abort();
  }
  hxc_string hxc_l_tmp_instance_call_result_n85 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_uiCatalog, hxc_l_locale, 1);
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n47 = hxc_l_tmp_instance_call_result_n85;
  hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n87 = HXC_BORROWED_CSTRING_INITIALIZER;
  if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n47, &hxc_l_tmp_string_borrow_cstring_result_n87) != HXC_STATUS_OK)
  {
    abort();
  }
  DrawText(hxc_l_tmp_string_borrow_cstring_result_n87.data, (int32_t)32, (int32_t)28, (int32_t)20, hxc_l_text_hd08b8496149a);
  hxc_string hxc_l_text1_h1d4071cc2348 = hxc_l_view.hxc_levelLabel;
  if (hxc_string_retain(hxc_l_text1_h1d4071cc2348) != HXC_STATUS_OK)
  {
    abort();
  }
  struct Color hxc_l_this1_hcd27aa71aa90 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
  hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n92 = HXC_BORROWED_CSTRING_INITIALIZER;
  if (hxc_string_borrow_cstring(&hxc_l_text1_h1d4071cc2348, &hxc_l_tmp_string_borrow_cstring_result_n92) != HXC_STATUS_OK)
  {
    abort();
  }
  DrawText(hxc_l_tmp_string_borrow_cstring_result_n92.data, (int32_t)250, (int32_t)30, (int32_t)16, hxc_l_this1_hcd27aa71aa90);
  hxc_string hxc_l_tmp_instance_call_result_n95 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_uiCatalog, hxc_l_locale, 4);
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n50 = hxc_l_tmp_instance_call_result_n95;
  hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n97 = HXC_BORROWED_CSTRING_INITIALIZER;
  if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n50, &hxc_l_tmp_string_borrow_cstring_result_n97) != HXC_STATUS_OK)
  {
    abort();
  }
  DrawText(hxc_l_tmp_string_borrow_cstring_result_n97.data, (int32_t)32, (int32_t)58, (int32_t)14, hxc_l_text_hd08b8496149a);
  hxc_caxecraft_app_HudDigits_drawNumber(16384, 82, 59, 5, (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 });
  hxc_string hxc_l_tmp_instance_call_result_n101 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_uiCatalog, hxc_l_locale, 8);
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n51 = hxc_l_tmp_instance_call_result_n101;
  hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n103 = HXC_BORROWED_CSTRING_INITIALIZER;
  if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n51, &hxc_l_tmp_string_borrow_cstring_result_n103) != HXC_STATUS_OK)
  {
    abort();
  }
  DrawText(hxc_l_tmp_string_borrow_cstring_result_n103.data, (int32_t)160, (int32_t)58, (int32_t)14, hxc_l_text_hd08b8496149a);
  hxc_caxecraft_app_HudDigits_drawNumber(hxc_l_visible, 230, 59, 5, (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 });
  hxc_string hxc_l_tmp_instance_call_result_n108 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_uiCatalog, hxc_l_locale, 5);
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n52 = hxc_l_tmp_instance_call_result_n108;
  hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n110 = HXC_BORROWED_CSTRING_INITIALIZER;
  if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n52, &hxc_l_tmp_string_borrow_cstring_result_n110) != HXC_STATUS_OK)
  {
    abort();
  }
  DrawText(hxc_l_tmp_string_borrow_cstring_result_n110.data, (int32_t)326, (int32_t)58, (int32_t)14, hxc_l_text_hd08b8496149a);
  hxc_caxecraft_app_HudDigits_drawNumber(hxc_l_drawCalls, 382, 59, 5, (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 });
  hxc_string hxc_l_tmp_instance_call_result_n115 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_uiCatalog, hxc_l_locale, 6);
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n53 = hxc_l_tmp_instance_call_result_n115;
  hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n117 = HXC_BORROWED_CSTRING_INITIALIZER;
  if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n53, &hxc_l_tmp_string_borrow_cstring_result_n117) != HXC_STATUS_OK)
  {
    abort();
  }
  DrawText(hxc_l_tmp_string_borrow_cstring_result_n117.data, (int32_t)32, (int32_t)86, (int32_t)12, hxc_l_text_hd08b8496149a);
  int32_t hxc_l_tmp_load_result_n119 = hxc_l_frames;
  hxc_caxecraft_app_HudDigits_drawNumber(hxc_l_tmp_load_result_n119, 82, 85, 6, hxc_l_text_hd08b8496149a);
  hxc_string hxc_l_tmp_instance_call_result_n122 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_uiCatalog, hxc_l_locale, 7);
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n54 = hxc_l_tmp_instance_call_result_n122;
  hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n124 = HXC_BORROWED_CSTRING_INITIALIZER;
  if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n54, &hxc_l_tmp_string_borrow_cstring_result_n124) != HXC_STATUS_OK)
  {
    abort();
  }
  DrawText(hxc_l_tmp_string_borrow_cstring_result_n124.data, (int32_t)174, (int32_t)86, (int32_t)12, hxc_l_text_hd08b8496149a);
  int32_t hxc_l_tmp_load_result_n126 = hxc_l_updates;
  hxc_caxecraft_app_HudDigits_drawNumber(hxc_l_tmp_load_result_n126, 216, 85, 6, hxc_l_text_hd08b8496149a);
  struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_load_result_n128 = hxc_l_inventory;
  struct Texture hxc_l_tmp_load_result_n129 = hxc_l_hudTexture;
  bool hxc_l_tmp_load_result_n130 = hxc_l_hudTextureReady;
  struct Texture hxc_l_tmp_load_result_n131 = hxc_l_itemTexture;
  bool hxc_l_tmp_load_result_n132 = hxc_l_itemTextureReady;
  int32_t hxc_l_tmp_load_result_n133 = hxc_l_width;
  hxc_caxecraft_app_CaxecraftApp_drawHotbar(hxc_l_tmp_load_result_n128, hxc_l_tmp_load_result_n129, hxc_l_tmp_load_result_n130, hxc_l_tmp_load_result_n131, hxc_l_tmp_load_result_n132, hxc_l_tmp_load_result_n133, hxc_l_height);
  struct hxc_caxecraft_domain_VitalsState hxc_l_tmp_load_result_n135 = hxc_l_vitals;
  struct Texture hxc_l_tmp_load_result_n136 = hxc_l_hudTexture;
  bool hxc_l_tmp_load_result_n137 = hxc_l_hudTextureReady;
  hxc_caxecraft_app_CaxecraftApp_drawHealth(hxc_l_tmp_load_result_n135, hxc_l_tmp_load_result_n136, hxc_l_tmp_load_result_n137, hxc_l_width);
  if (hxc_l_aquaticEquipmentCode >= 0)
  {
    int32_t hxc_l_tmp_load_result_n140 = hxc_l_aquaticEquipmentCode;
    struct Texture hxc_l_tmp_load_result_n141 = hxc_l_itemTexture;
    bool hxc_l_tmp_load_result_n142 = hxc_l_itemTextureReady;
    struct Texture hxc_l_tmp_load_result_n143 = hxc_l_adventureItemTexture;
    bool hxc_l_tmp_load_result_n144 = hxc_l_adventureItemTextureReady;
    hxc_caxecraft_app_AuthoredItemRenderer_drawEquippedIcon(hxc_l_contentRegistry, hxc_l_tmp_load_result_n140, hxc_l_tmp_load_result_n141, hxc_l_tmp_load_result_n142, hxc_l_tmp_load_result_n143, hxc_l_tmp_load_result_n144, hxc_i32_subtract_wrapping(hxc_l_width, 226), 18, 42);
  }
  if (hxc_l_headSubmerged)
  {
    int32_t hxc_l_tmp_load_result_n147 = hxc_l_breathTicks;
    int32_t hxc_l_tmp_load_result_n148 = hxc_l_maximumBreathTicks;
    int32_t hxc_l_tmp_load_result_n149 = hxc_l_width;
    hxc_caxecraft_app_CaxecraftApp_drawBreath(hxc_l_tmp_load_result_n147, hxc_l_tmp_load_result_n148, hxc_l_tmp_load_result_n149, hxc_l_height);
  }
  hxc_string hxc_l_tmp_instance_call_result_n152 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_uiCatalog, hxc_l_locale, 3);
  hxc_l_tmp_cstring_ref_owner_owner_n55 = hxc_l_tmp_instance_call_result_n152;
  hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n154 = HXC_BORROWED_CSTRING_INITIALIZER;
  if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n55, &hxc_l_tmp_string_borrow_cstring_result_n154) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_tmp_load_result_n155 = hxc_l_height;
  DrawText(hxc_l_tmp_string_borrow_cstring_result_n154.data, (int32_t)20, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n155, 22), (int32_t)14, hxc_l_text_hd08b8496149a);
  int32_t hxc_l_tmp_load_result_n157 = hxc_l_mode;
  bool hxc_l_tmp_short_circuit_result_n56 = hxc_l_tmp_load_result_n157 == 1;
  if (hxc_l_tmp_load_result_n157 == 1)
  {
    hxc_string hxc_l_tmp_record_field_project_n158 = hxc_l_view.hxc_objectiveTitle;
    if (hxc_l_tmp_record_field_project_n158.data == NULL)
    {
      abort();
    }
    int32_t hxc_l_tmp_string_length_result_n159;
    if (hxc_string_haxe_length(hxc_l_tmp_record_field_project_n158, &hxc_l_tmp_string_length_result_n159) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_short_circuit_result_n56 = hxc_l_tmp_string_length_result_n159 > 0;
  }
  if (hxc_l_tmp_short_circuit_result_n56)
  {
    hxc_l_text_hf0cdd789724f = hxc_l_view.hxc_objectiveTitle;
    if (hxc_string_retain(hxc_l_text_hf0cdd789724f) != HXC_STATUS_OK)
    {
      abort();
    }
    struct Color hxc_l_this1_ha3dd3980a90e = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
    hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n164 = HXC_BORROWED_CSTRING_INITIALIZER;
    if (hxc_string_borrow_cstring(&hxc_l_text_hf0cdd789724f, &hxc_l_tmp_string_borrow_cstring_result_n164) != HXC_STATUS_OK)
    {
      abort();
    }
    DrawText(hxc_l_tmp_string_borrow_cstring_result_n164.data, (int32_t)32, (int32_t)110, (int32_t)14, hxc_l_this1_ha3dd3980a90e);
    if (hxc_string_release(&hxc_l_text_hf0cdd789724f) != HXC_STATUS_OK)
    {
      abort();
    }
  }
  if (hxc_l_guideInteractionAvailable)
  {
    struct Color hxc_l_this1_h155d0521ddfa = (struct Color){ .r = 15, .g = 27, .b = 39, .a = 220 };
    int32_t hxc_l_tmp_load_result_n168 = hxc_l_centerX;
    int32_t hxc_l_tmp_load_result_n169 = hxc_l_centerY;
    DrawRectangle((int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n168, 260), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n169, 54), (int32_t)520, (int32_t)60, hxc_l_this1_h155d0521ddfa);
    if (hxc_l_guidePhase == hxc_caxecraft_gameplay_GuidePhase_Waiting)
    {
      hxc_string hxc_l_tmp_call_result_n173 = hxc_caxecraft_app_GameplayMessage_gameplayMessageId(11);
      hxc_l_text1_h2dc3beea7319 = hxc_l_tmp_call_result_n173;
      hxc_string hxc_l_text2_hf00c5a09a6b0 = { 0 };
      switch (hxc_l_locale) {
        case 0:
          {
            hxc_l_text2_hf00c5a09a6b0 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
            if (hxc_string_retain(hxc_l_text2_hf00c5a09a6b0) != HXC_STATUS_OK)
            {
              abort();
            }
            break;
          }
        case 1:
          {
            hxc_l_text2_hf00c5a09a6b0 = (hxc_string){ (const uint8_t *)"es-mx", 5, true, NULL };
            if (hxc_string_retain(hxc_l_text2_hf00c5a09a6b0) != HXC_STATUS_OK)
            {
              abort();
            }
            break;
          }
        default:
          {
            hxc_l_text2_hf00c5a09a6b0 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
            if (hxc_string_retain(hxc_l_text2_hf00c5a09a6b0) != HXC_STATUS_OK)
            {
              abort();
            }
            break;
          }
      }
      hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n175 = hxc_l_text2_hf00c5a09a6b0;
      hxc_l_tmp_managed_flow_owner_n62 = hxc_l_tmp_managed_flow_carrier_move_result_n175;
      struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_load_result_n176 = hxc_l_presentation;
      hxc_l_gc_roots[4] = (const void *)hxc_l_tmp_load_result_n176;
      if (hxc_l_tmp_load_result_n176 == NULL)
      {
        abort();
      }
      hxc_string hxc_l_tmp_load_result_n177 = hxc_l_text1_h2dc3beea7319;
      hxc_string hxc_l_tmp_instance_call_result_n179 = hxc_caxecraft_content_RuntimeLevelPresentation_message(hxc_l_tmp_load_result_n176, hxc_l_tmp_load_result_n177, hxc_l_tmp_managed_flow_owner_n62);
      hxc_l_tmp_cstring_ref_owner_owner_n63 = hxc_l_tmp_instance_call_result_n179;
      hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n181 = HXC_BORROWED_CSTRING_INITIALIZER;
      if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n63, &hxc_l_tmp_string_borrow_cstring_result_n181) != HXC_STATUS_OK)
      {
        abort();
      }
      int32_t hxc_l_tmp_load_result_n182 = hxc_l_centerX;
      int32_t hxc_l_tmp_load_result_n183 = hxc_l_centerY;
      DrawText(hxc_l_tmp_string_borrow_cstring_result_n181.data, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n182, 110), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n183, 74), (int32_t)18, hxc_l_text_hd08b8496149a);
      if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n63) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n62) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_text1_h2dc3beea7319) != HXC_STATUS_OK)
      {
        abort();
      }
    }
    else
    {
      if (hxc_l_guidePhase == hxc_caxecraft_gameplay_GuidePhase_Welcomed)
      {
        hxc_string hxc_l_tmp_call_result_n187 = hxc_caxecraft_app_GameplayMessage_gameplayMessageId(12);
        hxc_l_text1_hd177824d8da0 = hxc_l_tmp_call_result_n187;
        hxc_string hxc_l_text2_h5769c26d0440 = { 0 };
        switch (hxc_l_locale) {
          case 0:
            {
              hxc_l_text2_h5769c26d0440 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
              if (hxc_string_retain(hxc_l_text2_h5769c26d0440) != HXC_STATUS_OK)
              {
                abort();
              }
              break;
            }
          case 1:
            {
              hxc_l_text2_h5769c26d0440 = (hxc_string){ (const uint8_t *)"es-mx", 5, true, NULL };
              if (hxc_string_retain(hxc_l_text2_h5769c26d0440) != HXC_STATUS_OK)
              {
                abort();
              }
              break;
            }
          default:
            {
              hxc_l_text2_h5769c26d0440 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
              if (hxc_string_retain(hxc_l_text2_h5769c26d0440) != HXC_STATUS_OK)
              {
                abort();
              }
              break;
            }
        }
        hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n189 = hxc_l_text2_h5769c26d0440;
        hxc_l_tmp_managed_flow_owner_n66 = hxc_l_tmp_managed_flow_carrier_move_result_n189;
        struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_load_result_n190 = hxc_l_presentation;
        hxc_l_gc_roots[5] = (const void *)hxc_l_tmp_load_result_n190;
        if (hxc_l_tmp_load_result_n190 == NULL)
        {
          abort();
        }
        hxc_string hxc_l_tmp_load_result_n191 = hxc_l_text1_hd177824d8da0;
        hxc_string hxc_l_tmp_instance_call_result_n193 = hxc_caxecraft_content_RuntimeLevelPresentation_message(hxc_l_tmp_load_result_n190, hxc_l_tmp_load_result_n191, hxc_l_tmp_managed_flow_owner_n66);
        hxc_l_tmp_cstring_ref_owner_owner_n67 = hxc_l_tmp_instance_call_result_n193;
        hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n195 = HXC_BORROWED_CSTRING_INITIALIZER;
        if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n67, &hxc_l_tmp_string_borrow_cstring_result_n195) != HXC_STATUS_OK)
        {
          abort();
        }
        int32_t hxc_l_tmp_load_result_n196 = hxc_l_centerX;
        int32_t hxc_l_tmp_load_result_n197 = hxc_l_centerY;
        DrawText(hxc_l_tmp_string_borrow_cstring_result_n195.data, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n196, 225), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n197, 74), (int32_t)16, hxc_l_text_hd08b8496149a);
        if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n67) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n66) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_string_release(&hxc_l_text1_hd177824d8da0) != HXC_STATUS_OK)
        {
          abort();
        }
      }
      else
      {
        hxc_string hxc_l_tmp_call_result_n199 = hxc_caxecraft_app_GameplayMessage_gameplayMessageId(10);
        hxc_l_text1_h240643b8a835 = hxc_l_tmp_call_result_n199;
        hxc_string hxc_l_text2_h2aa4a164d3d7 = { 0 };
        switch (hxc_l_locale) {
          case 0:
            {
              hxc_l_text2_h2aa4a164d3d7 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
              if (hxc_string_retain(hxc_l_text2_h2aa4a164d3d7) != HXC_STATUS_OK)
              {
                abort();
              }
              break;
            }
          case 1:
            {
              hxc_l_text2_h2aa4a164d3d7 = (hxc_string){ (const uint8_t *)"es-mx", 5, true, NULL };
              if (hxc_string_retain(hxc_l_text2_h2aa4a164d3d7) != HXC_STATUS_OK)
              {
                abort();
              }
              break;
            }
          default:
            {
              hxc_l_text2_h2aa4a164d3d7 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
              if (hxc_string_retain(hxc_l_text2_h2aa4a164d3d7) != HXC_STATUS_OK)
              {
                abort();
              }
              break;
            }
        }
        hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n201 = hxc_l_text2_h2aa4a164d3d7;
        hxc_l_tmp_managed_flow_owner_n70 = hxc_l_tmp_managed_flow_carrier_move_result_n201;
        struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_load_result_n202 = hxc_l_presentation;
        hxc_l_gc_roots[6] = (const void *)hxc_l_tmp_load_result_n202;
        if (hxc_l_tmp_load_result_n202 == NULL)
        {
          abort();
        }
        hxc_string hxc_l_tmp_load_result_n203 = hxc_l_text1_h240643b8a835;
        hxc_string hxc_l_tmp_instance_call_result_n205 = hxc_caxecraft_content_RuntimeLevelPresentation_message(hxc_l_tmp_load_result_n202, hxc_l_tmp_load_result_n203, hxc_l_tmp_managed_flow_owner_n70);
        hxc_l_tmp_cstring_ref_owner_owner_n71 = hxc_l_tmp_instance_call_result_n205;
        hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n207 = HXC_BORROWED_CSTRING_INITIALIZER;
        if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n71, &hxc_l_tmp_string_borrow_cstring_result_n207) != HXC_STATUS_OK)
        {
          abort();
        }
        int32_t hxc_l_tmp_load_result_n208 = hxc_l_centerX;
        int32_t hxc_l_tmp_load_result_n209 = hxc_l_centerY;
        DrawText(hxc_l_tmp_string_borrow_cstring_result_n207.data, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n208, 205), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n209, 74), (int32_t)16, hxc_l_text_hd08b8496149a);
        if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n71) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n70) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_string_release(&hxc_l_text1_h240643b8a835) != HXC_STATUS_OK)
        {
          abort();
        }
      }
    }
  }
  if (hxc_l_enemy.hxc_vitals.hxc_health > 0)
  {
    if (hxc_l_enemyPhase == hxc_caxecraft_domain_ActorControllerPhase_Windup)
    {
      struct Color hxc_l_color_hd46304ca0f2e = (struct Color){ .r = 218, .g = 65, .b = 72, .a = 255 };
      hxc_string hxc_l_tmp_call_result_n215 = hxc_caxecraft_app_GameplayMessage_gameplayMessageId(9);
      hxc_l_text_h6ad4c16fd72d = hxc_l_tmp_call_result_n215;
      hxc_string hxc_l_text1_h184ef86c96eb = { 0 };
      switch (hxc_l_locale) {
        case 0:
          {
            hxc_l_text1_h184ef86c96eb = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
            if (hxc_string_retain(hxc_l_text1_h184ef86c96eb) != HXC_STATUS_OK)
            {
              abort();
            }
            break;
          }
        case 1:
          {
            hxc_l_text1_h184ef86c96eb = (hxc_string){ (const uint8_t *)"es-mx", 5, true, NULL };
            if (hxc_string_retain(hxc_l_text1_h184ef86c96eb) != HXC_STATUS_OK)
            {
              abort();
            }
            break;
          }
        default:
          {
            hxc_l_text1_h184ef86c96eb = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
            if (hxc_string_retain(hxc_l_text1_h184ef86c96eb) != HXC_STATUS_OK)
            {
              abort();
            }
            break;
          }
      }
      hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n217 = hxc_l_text1_h184ef86c96eb;
      hxc_l_tmp_managed_flow_owner_n75 = hxc_l_tmp_managed_flow_carrier_move_result_n217;
      struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_load_result_n218 = hxc_l_presentation;
      hxc_l_gc_roots[7] = (const void *)hxc_l_tmp_load_result_n218;
      if (hxc_l_tmp_load_result_n218 == NULL)
      {
        abort();
      }
      hxc_string hxc_l_tmp_load_result_n219 = hxc_l_text_h6ad4c16fd72d;
      hxc_string hxc_l_tmp_instance_call_result_n221 = hxc_caxecraft_content_RuntimeLevelPresentation_message(hxc_l_tmp_load_result_n218, hxc_l_tmp_load_result_n219, hxc_l_tmp_managed_flow_owner_n75);
      hxc_l_tmp_cstring_ref_owner_owner_n76 = hxc_l_tmp_instance_call_result_n221;
      hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n223 = HXC_BORROWED_CSTRING_INITIALIZER;
      if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n76, &hxc_l_tmp_string_borrow_cstring_result_n223) != HXC_STATUS_OK)
      {
        abort();
      }
      int32_t hxc_l_tmp_load_result_n224 = hxc_l_width;
      DrawText(hxc_l_tmp_string_borrow_cstring_result_n223.data, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n224, 300), (int32_t)28, (int32_t)16, hxc_l_color_hd46304ca0f2e);
      if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n76) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n75) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_text_h6ad4c16fd72d) != HXC_STATUS_OK)
      {
        abort();
      }
    }
    else
    {
      if (hxc_l_enemyPhase == hxc_caxecraft_domain_ActorControllerPhase_Chasing)
      {
        struct Color hxc_l_color_hd523d77bc4db = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
        hxc_string hxc_l_tmp_call_result_n229 = hxc_caxecraft_app_GameplayMessage_gameplayMessageId(7);
        hxc_l_text_h62a045f39fd0 = hxc_l_tmp_call_result_n229;
        hxc_string hxc_l_text1_hbdd666f2d838 = { 0 };
        switch (hxc_l_locale) {
          case 0:
            {
              hxc_l_text1_hbdd666f2d838 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
              if (hxc_string_retain(hxc_l_text1_hbdd666f2d838) != HXC_STATUS_OK)
              {
                abort();
              }
              break;
            }
          case 1:
            {
              hxc_l_text1_hbdd666f2d838 = (hxc_string){ (const uint8_t *)"es-mx", 5, true, NULL };
              if (hxc_string_retain(hxc_l_text1_hbdd666f2d838) != HXC_STATUS_OK)
              {
                abort();
              }
              break;
            }
          default:
            {
              hxc_l_text1_hbdd666f2d838 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
              if (hxc_string_retain(hxc_l_text1_hbdd666f2d838) != HXC_STATUS_OK)
              {
                abort();
              }
              break;
            }
        }
        hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n231 = hxc_l_text1_hbdd666f2d838;
        hxc_l_tmp_managed_flow_owner_n80 = hxc_l_tmp_managed_flow_carrier_move_result_n231;
        struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_load_result_n232 = hxc_l_presentation;
        hxc_l_gc_roots[8] = (const void *)hxc_l_tmp_load_result_n232;
        if (hxc_l_tmp_load_result_n232 == NULL)
        {
          abort();
        }
        hxc_string hxc_l_tmp_load_result_n233 = hxc_l_text_h62a045f39fd0;
        hxc_string hxc_l_tmp_instance_call_result_n235 = hxc_caxecraft_content_RuntimeLevelPresentation_message(hxc_l_tmp_load_result_n232, hxc_l_tmp_load_result_n233, hxc_l_tmp_managed_flow_owner_n80);
        hxc_l_tmp_cstring_ref_owner_owner_n81 = hxc_l_tmp_instance_call_result_n235;
        hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n237 = HXC_BORROWED_CSTRING_INITIALIZER;
        if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n81, &hxc_l_tmp_string_borrow_cstring_result_n237) != HXC_STATUS_OK)
        {
          abort();
        }
        int32_t hxc_l_tmp_load_result_n238 = hxc_l_width;
        DrawText(hxc_l_tmp_string_borrow_cstring_result_n237.data, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n238, 180), (int32_t)28, (int32_t)16, hxc_l_color_hd523d77bc4db);
        if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n81) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n80) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_string_release(&hxc_l_text_h62a045f39fd0) != HXC_STATUS_OK)
        {
          abort();
        }
      }
    }
  }
  if (hxc_l_strikeHit)
  {
    struct Color hxc_l_color_hd917451af541 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
    hxc_string hxc_l_tmp_call_result_n242 = hxc_caxecraft_app_GameplayMessage_gameplayMessageId(5);
    hxc_l_text_hc2bd8c347136 = hxc_l_tmp_call_result_n242;
    hxc_string hxc_l_text1_hc732a944065c = { 0 };
    switch (hxc_l_locale) {
      case 0:
        {
          hxc_l_text1_hc732a944065c = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
          if (hxc_string_retain(hxc_l_text1_hc732a944065c) != HXC_STATUS_OK)
          {
            abort();
          }
          break;
        }
      case 1:
        {
          hxc_l_text1_hc732a944065c = (hxc_string){ (const uint8_t *)"es-mx", 5, true, NULL };
          if (hxc_string_retain(hxc_l_text1_hc732a944065c) != HXC_STATUS_OK)
          {
            abort();
          }
          break;
        }
      default:
        {
          hxc_l_text1_hc732a944065c = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
          if (hxc_string_retain(hxc_l_text1_hc732a944065c) != HXC_STATUS_OK)
          {
            abort();
          }
          break;
        }
    }
    hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n244 = hxc_l_text1_hc732a944065c;
    hxc_l_tmp_managed_flow_owner_n85 = hxc_l_tmp_managed_flow_carrier_move_result_n244;
    struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_load_result_n245 = hxc_l_presentation;
    hxc_l_gc_roots[9] = (const void *)hxc_l_tmp_load_result_n245;
    if (hxc_l_tmp_load_result_n245 == NULL)
    {
      abort();
    }
    hxc_string hxc_l_tmp_load_result_n246 = hxc_l_text_hc2bd8c347136;
    hxc_string hxc_l_tmp_instance_call_result_n248 = hxc_caxecraft_content_RuntimeLevelPresentation_message(hxc_l_tmp_load_result_n245, hxc_l_tmp_load_result_n246, hxc_l_tmp_managed_flow_owner_n85);
    hxc_l_tmp_cstring_ref_owner_owner_n86 = hxc_l_tmp_instance_call_result_n248;
    hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n250 = HXC_BORROWED_CSTRING_INITIALIZER;
    if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n86, &hxc_l_tmp_string_borrow_cstring_result_n250) != HXC_STATUS_OK)
    {
      abort();
    }
    int32_t hxc_l_tmp_load_result_n251 = hxc_l_centerX;
    int32_t hxc_l_tmp_load_result_n252 = hxc_l_centerY;
    DrawText(hxc_l_tmp_string_borrow_cstring_result_n250.data, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n251, 70), (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n252, 54), (int32_t)18, hxc_l_color_hd917451af541);
    if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n86) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n85) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_text_hc2bd8c347136) != HXC_STATUS_OK)
    {
      abort();
    }
  }
  if (hxc_l_enemyDefeated)
  {
    struct Color hxc_l_color_h9a75e7b5d4f6 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
    hxc_string hxc_l_tmp_call_result_n256 = hxc_caxecraft_app_GameplayMessage_gameplayMessageId(8);
    hxc_l_text_he6b508ade1f1 = hxc_l_tmp_call_result_n256;
    hxc_string hxc_l_text1_h4561d1a543c1 = { 0 };
    switch (hxc_l_locale) {
      case 0:
        {
          hxc_l_text1_h4561d1a543c1 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
          if (hxc_string_retain(hxc_l_text1_h4561d1a543c1) != HXC_STATUS_OK)
          {
            abort();
          }
          break;
        }
      case 1:
        {
          hxc_l_text1_h4561d1a543c1 = (hxc_string){ (const uint8_t *)"es-mx", 5, true, NULL };
          if (hxc_string_retain(hxc_l_text1_h4561d1a543c1) != HXC_STATUS_OK)
          {
            abort();
          }
          break;
        }
      default:
        {
          hxc_l_text1_h4561d1a543c1 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
          if (hxc_string_retain(hxc_l_text1_h4561d1a543c1) != HXC_STATUS_OK)
          {
            abort();
          }
          break;
        }
    }
    hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n258 = hxc_l_text1_h4561d1a543c1;
    hxc_l_tmp_managed_flow_owner_n90 = hxc_l_tmp_managed_flow_carrier_move_result_n258;
    struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_load_result_n259 = hxc_l_presentation;
    hxc_l_gc_roots[10] = (const void *)hxc_l_tmp_load_result_n259;
    if (hxc_l_tmp_load_result_n259 == NULL)
    {
      abort();
    }
    hxc_string hxc_l_tmp_load_result_n260 = hxc_l_text_he6b508ade1f1;
    hxc_string hxc_l_tmp_instance_call_result_n262 = hxc_caxecraft_content_RuntimeLevelPresentation_message(hxc_l_tmp_load_result_n259, hxc_l_tmp_load_result_n260, hxc_l_tmp_managed_flow_owner_n90);
    hxc_l_tmp_cstring_ref_owner_owner_n91 = hxc_l_tmp_instance_call_result_n262;
    hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n264 = HXC_BORROWED_CSTRING_INITIALIZER;
    if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n91, &hxc_l_tmp_string_borrow_cstring_result_n264) != HXC_STATUS_OK)
    {
      abort();
    }
    int32_t hxc_l_tmp_load_result_n265 = hxc_l_width;
    DrawText(hxc_l_tmp_string_borrow_cstring_result_n264.data, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n265, 285), (int32_t)54, (int32_t)16, hxc_l_color_h9a75e7b5d4f6);
    if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n91) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n90) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_text_he6b508ade1f1) != HXC_STATUS_OK)
    {
      abort();
    }
  }
  if (hxc_l_enemyAttacked)
  {
    struct Color hxc_l_color_h39707b0ee52d = (struct Color){ .r = 218, .g = 65, .b = 72, .a = 255 };
    hxc_string hxc_l_tmp_call_result_n269 = hxc_caxecraft_app_GameplayMessage_gameplayMessageId(15);
    hxc_l_text_hb97746981573 = hxc_l_tmp_call_result_n269;
    hxc_string hxc_l_text1_h29dc81d55dde = { 0 };
    switch (hxc_l_locale) {
      case 0:
        {
          hxc_l_text1_h29dc81d55dde = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
          if (hxc_string_retain(hxc_l_text1_h29dc81d55dde) != HXC_STATUS_OK)
          {
            abort();
          }
          break;
        }
      case 1:
        {
          hxc_l_text1_h29dc81d55dde = (hxc_string){ (const uint8_t *)"es-mx", 5, true, NULL };
          if (hxc_string_retain(hxc_l_text1_h29dc81d55dde) != HXC_STATUS_OK)
          {
            abort();
          }
          break;
        }
      default:
        {
          hxc_l_text1_h29dc81d55dde = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
          if (hxc_string_retain(hxc_l_text1_h29dc81d55dde) != HXC_STATUS_OK)
          {
            abort();
          }
          break;
        }
    }
    hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n271 = hxc_l_text1_h29dc81d55dde;
    hxc_l_tmp_managed_flow_owner_n95 = hxc_l_tmp_managed_flow_carrier_move_result_n271;
    struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_load_result_n272 = hxc_l_presentation;
    hxc_l_gc_roots[11] = (const void *)hxc_l_tmp_load_result_n272;
    if (hxc_l_tmp_load_result_n272 == NULL)
    {
      abort();
    }
    hxc_string hxc_l_tmp_load_result_n273 = hxc_l_text_hb97746981573;
    hxc_string hxc_l_tmp_instance_call_result_n275 = hxc_caxecraft_content_RuntimeLevelPresentation_message(hxc_l_tmp_load_result_n272, hxc_l_tmp_load_result_n273, hxc_l_tmp_managed_flow_owner_n95);
    hxc_l_tmp_cstring_ref_owner_owner_n96 = hxc_l_tmp_instance_call_result_n275;
    hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n277 = HXC_BORROWED_CSTRING_INITIALIZER;
    if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n96, &hxc_l_tmp_string_borrow_cstring_result_n277) != HXC_STATUS_OK)
    {
      abort();
    }
    int32_t hxc_l_tmp_load_result_n278 = hxc_l_width;
    DrawText(hxc_l_tmp_string_borrow_cstring_result_n277.data, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n278, 330), (int32_t)82, (int32_t)16, hxc_l_color_h39707b0ee52d);
    if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n96) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n95) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_text_hb97746981573) != HXC_STATUS_OK)
    {
      abort();
    }
  }
  if (hxc_l_pickedUp)
  {
    int32_t hxc_l_tmp_load_result_n281 = hxc_l_pickupAmount;
    int32_t hxc_l_tmp_conditional_result_n98 = 0;
    if (hxc_l_tmp_load_result_n281 == 1)
    {
      hxc_l_tmp_conditional_result_n98 = 0;
    }
    else
    {
      hxc_l_tmp_conditional_result_n98 = 1;
    }
    int32_t hxc_l_pickupMessage = hxc_l_tmp_conditional_result_n98;
    struct Color hxc_l_color_h0a443dee2646 = (struct Color){ .r = 174, .g = 78, .b = 136, .a = 255 };
    hxc_string hxc_l_tmp_call_result_n285 = hxc_caxecraft_app_GameplayMessage_gameplayMessageId(hxc_l_pickupMessage);
    hxc_l_text_h5e4d2698c6aa = hxc_l_tmp_call_result_n285;
    hxc_string hxc_l_text1_haffe65dee171 = { 0 };
    switch (hxc_l_locale) {
      case 0:
        {
          hxc_l_text1_haffe65dee171 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
          if (hxc_string_retain(hxc_l_text1_haffe65dee171) != HXC_STATUS_OK)
          {
            abort();
          }
          break;
        }
      case 1:
        {
          hxc_l_text1_haffe65dee171 = (hxc_string){ (const uint8_t *)"es-mx", 5, true, NULL };
          if (hxc_string_retain(hxc_l_text1_haffe65dee171) != HXC_STATUS_OK)
          {
            abort();
          }
          break;
        }
      default:
        {
          hxc_l_text1_haffe65dee171 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
          if (hxc_string_retain(hxc_l_text1_haffe65dee171) != HXC_STATUS_OK)
          {
            abort();
          }
          break;
        }
    }
    hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n287 = hxc_l_text1_haffe65dee171;
    hxc_l_tmp_managed_flow_owner_n102 = hxc_l_tmp_managed_flow_carrier_move_result_n287;
    struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_load_result_n288 = hxc_l_presentation;
    hxc_l_gc_roots[12] = (const void *)hxc_l_tmp_load_result_n288;
    if (hxc_l_tmp_load_result_n288 == NULL)
    {
      abort();
    }
    hxc_string hxc_l_tmp_load_result_n289 = hxc_l_text_h5e4d2698c6aa;
    hxc_string hxc_l_tmp_instance_call_result_n291 = hxc_caxecraft_content_RuntimeLevelPresentation_message(hxc_l_tmp_load_result_n288, hxc_l_tmp_load_result_n289, hxc_l_tmp_managed_flow_owner_n102);
    hxc_l_tmp_cstring_ref_owner_owner_n103 = hxc_l_tmp_instance_call_result_n291;
    hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n293 = HXC_BORROWED_CSTRING_INITIALIZER;
    if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n103, &hxc_l_tmp_string_borrow_cstring_result_n293) != HXC_STATUS_OK)
    {
      abort();
    }
    int32_t hxc_l_tmp_load_result_n294 = hxc_l_centerX;
    int32_t hxc_l_tmp_load_result_n295 = hxc_l_centerY;
    DrawText(hxc_l_tmp_string_borrow_cstring_result_n293.data, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n294, 48), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n295, 24), (int32_t)18, hxc_l_color_h0a443dee2646);
    if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n103) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n102) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_text_h5e4d2698c6aa) != HXC_STATUS_OK)
    {
      abort();
    }
  }
  if (hxc_l_inventoryFullReason == 1)
  {
    struct Color hxc_l_color_he3b5cf084f65 = (struct Color){ .r = 238, .g = 113, .b = 78, .a = 255 };
    hxc_string hxc_l_tmp_call_result_n299 = hxc_caxecraft_app_GameplayMessage_gameplayMessageId(3);
    hxc_l_text_h2969ab0ae2bb = hxc_l_tmp_call_result_n299;
    hxc_string hxc_l_text1_h9fa5f6d67f02 = { 0 };
    switch (hxc_l_locale) {
      case 0:
        {
          hxc_l_text1_h9fa5f6d67f02 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
          if (hxc_string_retain(hxc_l_text1_h9fa5f6d67f02) != HXC_STATUS_OK)
          {
            abort();
          }
          break;
        }
      case 1:
        {
          hxc_l_text1_h9fa5f6d67f02 = (hxc_string){ (const uint8_t *)"es-mx", 5, true, NULL };
          if (hxc_string_retain(hxc_l_text1_h9fa5f6d67f02) != HXC_STATUS_OK)
          {
            abort();
          }
          break;
        }
      default:
        {
          hxc_l_text1_h9fa5f6d67f02 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
          if (hxc_string_retain(hxc_l_text1_h9fa5f6d67f02) != HXC_STATUS_OK)
          {
            abort();
          }
          break;
        }
    }
    hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n301 = hxc_l_text1_h9fa5f6d67f02;
    hxc_l_tmp_managed_flow_owner_n107 = hxc_l_tmp_managed_flow_carrier_move_result_n301;
    struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_load_result_n302 = hxc_l_presentation;
    hxc_l_gc_roots[13] = (const void *)hxc_l_tmp_load_result_n302;
    if (hxc_l_tmp_load_result_n302 == NULL)
    {
      abort();
    }
    hxc_string hxc_l_tmp_load_result_n303 = hxc_l_text_h2969ab0ae2bb;
    hxc_string hxc_l_tmp_instance_call_result_n305 = hxc_caxecraft_content_RuntimeLevelPresentation_message(hxc_l_tmp_load_result_n302, hxc_l_tmp_load_result_n303, hxc_l_tmp_managed_flow_owner_n107);
    hxc_l_tmp_cstring_ref_owner_owner_n108 = hxc_l_tmp_instance_call_result_n305;
    hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n307 = HXC_BORROWED_CSTRING_INITIALIZER;
    if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n108, &hxc_l_tmp_string_borrow_cstring_result_n307) != HXC_STATUS_OK)
    {
      abort();
    }
    int32_t hxc_l_tmp_load_result_n308 = hxc_l_centerX;
    int32_t hxc_l_tmp_load_result_n309 = hxc_l_centerY;
    DrawText(hxc_l_tmp_string_borrow_cstring_result_n307.data, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n308, 150), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n309, 48), (int32_t)16, hxc_l_color_he3b5cf084f65);
    if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n108) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n107) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_text_h2969ab0ae2bb) != HXC_STATUS_OK)
    {
      abort();
    }
  }
  else
  {
    if (hxc_l_inventoryFullReason == 2)
    {
      struct Color hxc_l_color_hade850a466cf = (struct Color){ .r = 238, .g = 113, .b = 78, .a = 255 };
      hxc_string hxc_l_tmp_call_result_n313 = hxc_caxecraft_app_GameplayMessage_gameplayMessageId(4);
      hxc_l_text_hf64b6b3f6e73 = hxc_l_tmp_call_result_n313;
      hxc_string hxc_l_text1_h4b32e92ffda8 = { 0 };
      switch (hxc_l_locale) {
        case 0:
          {
            hxc_l_text1_h4b32e92ffda8 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
            if (hxc_string_retain(hxc_l_text1_h4b32e92ffda8) != HXC_STATUS_OK)
            {
              abort();
            }
            break;
          }
        case 1:
          {
            hxc_l_text1_h4b32e92ffda8 = (hxc_string){ (const uint8_t *)"es-mx", 5, true, NULL };
            if (hxc_string_retain(hxc_l_text1_h4b32e92ffda8) != HXC_STATUS_OK)
            {
              abort();
            }
            break;
          }
        default:
          {
            hxc_l_text1_h4b32e92ffda8 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
            if (hxc_string_retain(hxc_l_text1_h4b32e92ffda8) != HXC_STATUS_OK)
            {
              abort();
            }
            break;
          }
      }
      hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n315 = hxc_l_text1_h4b32e92ffda8;
      hxc_l_tmp_managed_flow_owner_n112 = hxc_l_tmp_managed_flow_carrier_move_result_n315;
      struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_load_result_n316 = hxc_l_presentation;
      hxc_l_gc_roots[14] = (const void *)hxc_l_tmp_load_result_n316;
      if (hxc_l_tmp_load_result_n316 == NULL)
      {
        abort();
      }
      hxc_string hxc_l_tmp_load_result_n317 = hxc_l_text_hf64b6b3f6e73;
      hxc_string hxc_l_tmp_instance_call_result_n319 = hxc_caxecraft_content_RuntimeLevelPresentation_message(hxc_l_tmp_load_result_n316, hxc_l_tmp_load_result_n317, hxc_l_tmp_managed_flow_owner_n112);
      hxc_l_tmp_cstring_ref_owner_owner_n113 = hxc_l_tmp_instance_call_result_n319;
      hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n321 = HXC_BORROWED_CSTRING_INITIALIZER;
      if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n113, &hxc_l_tmp_string_borrow_cstring_result_n321) != HXC_STATUS_OK)
      {
        abort();
      }
      int32_t hxc_l_tmp_load_result_n322 = hxc_l_centerX;
      int32_t hxc_l_tmp_load_result_n323 = hxc_l_centerY;
      DrawText(hxc_l_tmp_string_borrow_cstring_result_n321.data, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n322, 155), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n323, 48), (int32_t)16, hxc_l_color_hade850a466cf);
      if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n113) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n112) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_text_hf64b6b3f6e73) != HXC_STATUS_OK)
      {
        abort();
      }
    }
  }
  if (hxc_l_recoveryVisible)
  {
    if (hxc_l_recoveryFeedback == 1)
    {
      struct Color hxc_l_color_h1d86c85f0810 = (struct Color){ .r = 94, .g = 212, .b = 136, .a = 255 };
      hxc_string hxc_l_tmp_call_result_n328 = hxc_caxecraft_app_GameplayMessage_gameplayMessageId(2);
      hxc_l_text_h36fcc5058efc = hxc_l_tmp_call_result_n328;
      hxc_string hxc_l_text1_h7311547d3f77 = { 0 };
      switch (hxc_l_locale) {
        case 0:
          {
            hxc_l_text1_h7311547d3f77 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
            if (hxc_string_retain(hxc_l_text1_h7311547d3f77) != HXC_STATUS_OK)
            {
              abort();
            }
            break;
          }
        case 1:
          {
            hxc_l_text1_h7311547d3f77 = (hxc_string){ (const uint8_t *)"es-mx", 5, true, NULL };
            if (hxc_string_retain(hxc_l_text1_h7311547d3f77) != HXC_STATUS_OK)
            {
              abort();
            }
            break;
          }
        default:
          {
            hxc_l_text1_h7311547d3f77 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
            if (hxc_string_retain(hxc_l_text1_h7311547d3f77) != HXC_STATUS_OK)
            {
              abort();
            }
            break;
          }
      }
      hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n330 = hxc_l_text1_h7311547d3f77;
      hxc_l_tmp_managed_flow_owner_n117 = hxc_l_tmp_managed_flow_carrier_move_result_n330;
      struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_load_result_n331 = hxc_l_presentation;
      hxc_l_gc_roots[15] = (const void *)hxc_l_tmp_load_result_n331;
      if (hxc_l_tmp_load_result_n331 == NULL)
      {
        abort();
      }
      hxc_string hxc_l_tmp_load_result_n332 = hxc_l_text_h36fcc5058efc;
      hxc_string hxc_l_tmp_instance_call_result_n334 = hxc_caxecraft_content_RuntimeLevelPresentation_message(hxc_l_tmp_load_result_n331, hxc_l_tmp_load_result_n332, hxc_l_tmp_managed_flow_owner_n117);
      hxc_l_tmp_cstring_ref_owner_owner_n118 = hxc_l_tmp_instance_call_result_n334;
      hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n336 = HXC_BORROWED_CSTRING_INITIALIZER;
      if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n118, &hxc_l_tmp_string_borrow_cstring_result_n336) != HXC_STATUS_OK)
      {
        abort();
      }
      int32_t hxc_l_tmp_load_result_n337 = hxc_l_centerX;
      int32_t hxc_l_tmp_load_result_n338 = hxc_l_centerY;
      DrawText(hxc_l_tmp_string_borrow_cstring_result_n336.data, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n337, 88), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n338, 24), (int32_t)18, hxc_l_color_h1d86c85f0810);
      if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n118) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n117) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_text_h36fcc5058efc) != HXC_STATUS_OK)
      {
        abort();
      }
    }
    else
    {
      if (hxc_l_recoveryFeedback == 2)
      {
        struct Color hxc_l_this1_hec6d49a5c4cd = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
        hxc_string hxc_l_tmp_instance_call_result_n343 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_uiCatalog, hxc_l_locale, 25);
        hxc_l_tmp_cstring_ref_owner_owner_n120 = hxc_l_tmp_instance_call_result_n343;
        hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n345 = HXC_BORROWED_CSTRING_INITIALIZER;
        if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n120, &hxc_l_tmp_string_borrow_cstring_result_n345) != HXC_STATUS_OK)
        {
          abort();
        }
        int32_t hxc_l_tmp_load_result_n346 = hxc_l_centerX;
        int32_t hxc_l_tmp_load_result_n347 = hxc_l_centerY;
        DrawText(hxc_l_tmp_string_borrow_cstring_result_n345.data, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n346, 96), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n347, 24), (int32_t)18, hxc_l_this1_hec6d49a5c4cd);
        if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n120) != HXC_STATUS_OK)
        {
          abort();
        }
      }
      else
      {
        if (hxc_l_recoveryFeedback == 3)
        {
          struct Color hxc_l_color_h04870f1d7fa5 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
          hxc_string hxc_l_tmp_call_result_n351 = hxc_caxecraft_app_GameplayMessage_gameplayMessageId(13);
          hxc_l_text_h8a0e0a0381bb = hxc_l_tmp_call_result_n351;
          hxc_string hxc_l_text1_hd2408ada71cd = { 0 };
          switch (hxc_l_locale) {
            case 0:
              {
                hxc_l_text1_hd2408ada71cd = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
                if (hxc_string_retain(hxc_l_text1_hd2408ada71cd) != HXC_STATUS_OK)
                {
                  abort();
                }
                break;
              }
            case 1:
              {
                hxc_l_text1_hd2408ada71cd = (hxc_string){ (const uint8_t *)"es-mx", 5, true, NULL };
                if (hxc_string_retain(hxc_l_text1_hd2408ada71cd) != HXC_STATUS_OK)
                {
                  abort();
                }
                break;
              }
            default:
              {
                hxc_l_text1_hd2408ada71cd = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
                if (hxc_string_retain(hxc_l_text1_hd2408ada71cd) != HXC_STATUS_OK)
                {
                  abort();
                }
                break;
              }
          }
          hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n353 = hxc_l_text1_hd2408ada71cd;
          hxc_l_tmp_managed_flow_owner_n124 = hxc_l_tmp_managed_flow_carrier_move_result_n353;
          struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_load_result_n354 = hxc_l_presentation;
          hxc_l_gc_roots[16] = (const void *)hxc_l_tmp_load_result_n354;
          if (hxc_l_tmp_load_result_n354 == NULL)
          {
            abort();
          }
          hxc_string hxc_l_tmp_load_result_n355 = hxc_l_text_h8a0e0a0381bb;
          hxc_string hxc_l_tmp_instance_call_result_n357 = hxc_caxecraft_content_RuntimeLevelPresentation_message(hxc_l_tmp_load_result_n354, hxc_l_tmp_load_result_n355, hxc_l_tmp_managed_flow_owner_n124);
          hxc_l_tmp_cstring_ref_owner_owner_n125 = hxc_l_tmp_instance_call_result_n357;
          hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n359 = HXC_BORROWED_CSTRING_INITIALIZER;
          if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n125, &hxc_l_tmp_string_borrow_cstring_result_n359) != HXC_STATUS_OK)
          {
            abort();
          }
          int32_t hxc_l_tmp_load_result_n360 = hxc_l_centerX;
          int32_t hxc_l_tmp_load_result_n361 = hxc_l_centerY;
          DrawText(hxc_l_tmp_string_borrow_cstring_result_n359.data, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n360, 76), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n361, 24), (int32_t)18, hxc_l_color_h04870f1d7fa5);
          if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n125) != HXC_STATUS_OK)
          {
            abort();
          }
          if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n124) != HXC_STATUS_OK)
          {
            abort();
          }
          if (hxc_string_release(&hxc_l_text_h8a0e0a0381bb) != HXC_STATUS_OK)
          {
            abort();
          }
        }
      }
    }
  }
  if (hxc_l_aquaticEquipmentVisible)
  {
    struct Color hxc_l_this1_he4d32ce7273e = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
    hxc_string hxc_l_tmp_instance_call_result_n366 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_uiCatalog, hxc_l_locale, 0);
    hxc_l_tmp_cstring_ref_owner_owner_n127 = hxc_l_tmp_instance_call_result_n366;
    hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n368 = HXC_BORROWED_CSTRING_INITIALIZER;
    if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n127, &hxc_l_tmp_string_borrow_cstring_result_n368) != HXC_STATUS_OK)
    {
      abort();
    }
    int32_t hxc_l_tmp_load_result_n369 = hxc_l_centerX;
    int32_t hxc_l_tmp_load_result_n370 = hxc_l_centerY;
    DrawText(hxc_l_tmp_string_borrow_cstring_result_n368.data, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n369, 128), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n370, 24), (int32_t)18, hxc_l_this1_he4d32ce7273e);
    if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n127) != HXC_STATUS_OK)
    {
      abort();
    }
  }
  if (hxc_l_vitals.hxc_safeTicks > 15)
  {
    struct Color hxc_l_this1_hc8a97507397a = (struct Color){ .r = 218, .g = 65, .b = 72, .a = 255 };
    int32_t hxc_l_tmp_load_result_n374 = hxc_l_width;
    int32_t hxc_l_tmp_load_result_n375 = hxc_l_height;
    DrawRectangleLines((int32_t)4, (int32_t)4, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n374, 8), (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n375, 8), hxc_l_this1_hc8a97507397a);
  }
  if (hxc_l_vitals.hxc_health <= 0)
  {
    struct Color hxc_l_this1_ha0159345a785 = (struct Color){ .r = 15, .g = 27, .b = 39, .a = 220 };
    int32_t hxc_l_tmp_load_result_n379 = hxc_l_centerX;
    int32_t hxc_l_tmp_load_result_n380 = hxc_l_centerY;
    DrawRectangle((int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n379, 250), (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n380, 74), (int32_t)500, (int32_t)148, hxc_l_this1_ha0159345a785);
    struct Color hxc_l_this1_ha10fbe797fd0 = (struct Color){ .r = 218, .g = 65, .b = 72, .a = 255 };
    int32_t hxc_l_tmp_load_result_n383 = hxc_l_centerX;
    int32_t hxc_l_tmp_load_result_n384 = hxc_l_centerY;
    DrawRectangleLines((int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n383, 250), (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n384, 74), (int32_t)500, (int32_t)148, hxc_l_this1_ha10fbe797fd0);
    hxc_string hxc_l_tmp_call_result_n386 = hxc_caxecraft_app_GameplayMessage_gameplayMessageId(6);
    hxc_l_text1_hab565604a420 = hxc_l_tmp_call_result_n386;
    hxc_string hxc_l_text2_hf5c0ff9979b0 = { 0 };
    switch (hxc_l_locale) {
      case 0:
        {
          hxc_l_text2_hf5c0ff9979b0 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
          if (hxc_string_retain(hxc_l_text2_hf5c0ff9979b0) != HXC_STATUS_OK)
          {
            abort();
          }
          break;
        }
      case 1:
        {
          hxc_l_text2_hf5c0ff9979b0 = (hxc_string){ (const uint8_t *)"es-mx", 5, true, NULL };
          if (hxc_string_retain(hxc_l_text2_hf5c0ff9979b0) != HXC_STATUS_OK)
          {
            abort();
          }
          break;
        }
      default:
        {
          hxc_l_text2_hf5c0ff9979b0 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
          if (hxc_string_retain(hxc_l_text2_hf5c0ff9979b0) != HXC_STATUS_OK)
          {
            abort();
          }
          break;
        }
    }
    hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n388 = hxc_l_text2_hf5c0ff9979b0;
    hxc_l_tmp_managed_flow_owner_n133 = hxc_l_tmp_managed_flow_carrier_move_result_n388;
    struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_load_result_n389 = hxc_l_presentation;
    hxc_l_gc_roots[17] = (const void *)hxc_l_tmp_load_result_n389;
    if (hxc_l_tmp_load_result_n389 == NULL)
    {
      abort();
    }
    hxc_string hxc_l_tmp_load_result_n390 = hxc_l_text1_hab565604a420;
    hxc_string hxc_l_tmp_instance_call_result_n392 = hxc_caxecraft_content_RuntimeLevelPresentation_message(hxc_l_tmp_load_result_n389, hxc_l_tmp_load_result_n390, hxc_l_tmp_managed_flow_owner_n133);
    hxc_l_tmp_cstring_ref_owner_owner_n134 = hxc_l_tmp_instance_call_result_n392;
    hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n394 = HXC_BORROWED_CSTRING_INITIALIZER;
    if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n134, &hxc_l_tmp_string_borrow_cstring_result_n394) != HXC_STATUS_OK)
    {
      abort();
    }
    int32_t hxc_l_tmp_load_result_n395 = hxc_l_centerX;
    int32_t hxc_l_tmp_load_result_n396 = hxc_l_centerY;
    DrawText(hxc_l_tmp_string_borrow_cstring_result_n394.data, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n395, 122), (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n396, 42), (int32_t)24, hxc_l_text_hd08b8496149a);
    struct Color hxc_l_color_h5fdbe0c723c7 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
    hxc_string hxc_l_tmp_call_result_n399 = hxc_caxecraft_app_GameplayMessage_gameplayMessageId(14);
    hxc_l_text_ha0346c749ef1 = hxc_l_tmp_call_result_n399;
    hxc_string hxc_l_text1_hb68b92d8434f = { 0 };
    switch (hxc_l_locale) {
      case 0:
        {
          hxc_l_text1_hb68b92d8434f = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
          if (hxc_string_retain(hxc_l_text1_hb68b92d8434f) != HXC_STATUS_OK)
          {
            abort();
          }
          break;
        }
      case 1:
        {
          hxc_l_text1_hb68b92d8434f = (hxc_string){ (const uint8_t *)"es-mx", 5, true, NULL };
          if (hxc_string_retain(hxc_l_text1_hb68b92d8434f) != HXC_STATUS_OK)
          {
            abort();
          }
          break;
        }
      default:
        {
          hxc_l_text1_hb68b92d8434f = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
          if (hxc_string_retain(hxc_l_text1_hb68b92d8434f) != HXC_STATUS_OK)
          {
            abort();
          }
          break;
        }
    }
    hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n401 = hxc_l_text1_hb68b92d8434f;
    hxc_l_tmp_managed_flow_owner_n138 = hxc_l_tmp_managed_flow_carrier_move_result_n401;
    struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_load_result_n402 = hxc_l_presentation;
    hxc_l_gc_roots[18] = (const void *)hxc_l_tmp_load_result_n402;
    if (hxc_l_tmp_load_result_n402 == NULL)
    {
      abort();
    }
    hxc_string hxc_l_tmp_load_result_n403 = hxc_l_text_ha0346c749ef1;
    hxc_string hxc_l_tmp_instance_call_result_n405 = hxc_caxecraft_content_RuntimeLevelPresentation_message(hxc_l_tmp_load_result_n402, hxc_l_tmp_load_result_n403, hxc_l_tmp_managed_flow_owner_n138);
    hxc_l_tmp_cstring_ref_owner_owner_n139 = hxc_l_tmp_instance_call_result_n405;
    hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n407 = HXC_BORROWED_CSTRING_INITIALIZER;
    if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n139, &hxc_l_tmp_string_borrow_cstring_result_n407) != HXC_STATUS_OK)
    {
      abort();
    }
    int32_t hxc_l_tmp_load_result_n408 = hxc_l_centerX;
    int32_t hxc_l_tmp_load_result_n409 = hxc_l_centerY;
    DrawText(hxc_l_tmp_string_borrow_cstring_result_n407.data, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n408, 125), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n409, 10), (int32_t)18, hxc_l_color_h5fdbe0c723c7);
    if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n139) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n138) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_text_ha0346c749ef1) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n134) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n133) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_text1_hab565604a420) != HXC_STATUS_OK)
    {
      abort();
    }
  }
  if (hxc_l_paused)
  {
    struct Color hxc_l_this1_hb11bf76494dd = (struct Color){ .r = 15, .g = 27, .b = 39, .a = 220 };
    int32_t hxc_l_tmp_load_result_n413 = hxc_l_centerX;
    int32_t hxc_l_tmp_load_result_n414 = hxc_l_centerY;
    DrawRectangle((int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n413, 170), (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n414, 48), (int32_t)340, (int32_t)96, hxc_l_this1_hb11bf76494dd);
    struct Color hxc_l_this1_h643c5e1a119b = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
    int32_t hxc_l_tmp_load_result_n417 = hxc_l_centerX;
    int32_t hxc_l_tmp_load_result_n418 = hxc_l_centerY;
    DrawRectangleLines((int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n417, 170), (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n418, 48), (int32_t)340, (int32_t)96, hxc_l_this1_h643c5e1a119b);
    hxc_string hxc_l_tmp_instance_call_result_n421 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_uiCatalog, hxc_l_locale, 32);
    hxc_l_tmp_cstring_ref_owner_owner_n142 = hxc_l_tmp_instance_call_result_n421;
    hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n423 = HXC_BORROWED_CSTRING_INITIALIZER;
    if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n142, &hxc_l_tmp_string_borrow_cstring_result_n423) != HXC_STATUS_OK)
    {
      abort();
    }
    int32_t hxc_l_tmp_load_result_n424 = hxc_l_centerX;
    int32_t hxc_l_tmp_load_result_n425 = hxc_l_centerY;
    DrawText(hxc_l_tmp_string_borrow_cstring_result_n423.data, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n424, 48), (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n425, 30), (int32_t)24, hxc_l_text_hd08b8496149a);
    hxc_string hxc_l_tmp_instance_call_result_n428 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_uiCatalog, hxc_l_locale, 31);
    hxc_l_tmp_cstring_ref_owner_owner_n143 = hxc_l_tmp_instance_call_result_n428;
    hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n430 = HXC_BORROWED_CSTRING_INITIALIZER;
    if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n143, &hxc_l_tmp_string_borrow_cstring_result_n430) != HXC_STATUS_OK)
    {
      abort();
    }
    int32_t hxc_l_tmp_load_result_n431 = hxc_l_centerX;
    int32_t hxc_l_tmp_load_result_n432 = hxc_l_centerY;
    DrawText(hxc_l_tmp_string_borrow_cstring_result_n430.data, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n431, 160), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n432, 8), (int32_t)16, hxc_l_text_hd08b8496149a);
    if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n143) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n142) != HXC_STATUS_OK)
    {
      abort();
    }
  }
  else
  {
    if (hxc_l_placementBlocked)
    {
      struct Color hxc_l_this1_h83e419e1a65b = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
      hxc_string hxc_l_tmp_instance_call_result_n437 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_uiCatalog, hxc_l_locale, 33);
      hxc_l_tmp_cstring_ref_owner_owner_n145 = hxc_l_tmp_instance_call_result_n437;
      hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n439 = HXC_BORROWED_CSTRING_INITIALIZER;
      if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n145, &hxc_l_tmp_string_borrow_cstring_result_n439) != HXC_STATUS_OK)
      {
        abort();
      }
      int32_t hxc_l_tmp_load_result_n440 = hxc_l_centerX;
      int32_t hxc_l_tmp_load_result_n441 = hxc_l_centerY;
      DrawText(hxc_l_tmp_string_borrow_cstring_result_n439.data, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n440, 170), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n441, 26), (int32_t)14, hxc_l_this1_h83e419e1a65b);
      if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n145) != HXC_STATUS_OK)
      {
        abort();
      }
    }
    else
    {
      if (!hxc_l_captured)
      {
        hxc_string hxc_l_tmp_instance_call_result_n445 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_uiCatalog, hxc_l_locale, 2);
        hxc_l_tmp_cstring_ref_owner_owner_n146 = hxc_l_tmp_instance_call_result_n445;
        hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n447 = HXC_BORROWED_CSTRING_INITIALIZER;
        if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n146, &hxc_l_tmp_string_borrow_cstring_result_n447) != HXC_STATUS_OK)
        {
          abort();
        }
        int32_t hxc_l_tmp_load_result_n448 = hxc_l_centerX;
        int32_t hxc_l_tmp_load_result_n449 = hxc_l_centerY;
        DrawText(hxc_l_tmp_string_borrow_cstring_result_n447.data, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n448, 90), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n449, 26), (int32_t)14, hxc_l_text_hd08b8496149a);
        if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n146) != HXC_STATUS_OK)
        {
          abort();
        }
      }
      else
      {
        if (!hxc_l_hit.hxc_hit)
        {
          hxc_string hxc_l_tmp_instance_call_result_n453 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_uiCatalog, hxc_l_locale, 30);
          hxc_l_tmp_cstring_ref_owner_owner_n147 = hxc_l_tmp_instance_call_result_n453;
          hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n455 = HXC_BORROWED_CSTRING_INITIALIZER;
          if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n147, &hxc_l_tmp_string_borrow_cstring_result_n455) != HXC_STATUS_OK)
          {
            abort();
          }
          int32_t hxc_l_tmp_load_result_n456 = hxc_l_centerX;
          int32_t hxc_l_tmp_load_result_n457 = hxc_l_centerY;
          DrawText(hxc_l_tmp_string_borrow_cstring_result_n455.data, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n456, 105), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n457, 26), (int32_t)14, hxc_l_text_hd08b8496149a);
          if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n147) != HXC_STATUS_OK)
          {
            abort();
          }
        }
      }
    }
  }
  if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n55) != HXC_STATUS_OK)
  {
    abort();
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
  if (hxc_string_release(&hxc_l_text1_h1d4071cc2348) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n47) != HXC_STATUS_OK)
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
  const void *hxc_l_gc_roots[246] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 246, &hxc_l_gc_frame) != HXC_STATUS_OK)
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
  struct hxc_array_ref *hxc_l_g1_h4fb5d97fff90 = { 0 };
  struct hxc_caxecraft_domain_ActorControllerEvent hxc_l_event_h4aa8988b4fdd = { 0 };
  struct hxc_caxecraft_domain_ActorControllerEvent hxc_l_tmp_array_element_owner_n295 = { 0 };
  hxc_string hxc_l_drop = { 0 };
  struct hxc_array_ref *hxc_l_actorPhases = { 0 };
  struct hxc_array_ref *hxc_l_presentationActorPhases = { 0 };
  struct hxc_caxecraft_content_RuntimeLevelReceipt hxc_l_receipt = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n343 = { 0 };
  hxc_string hxc_l_adventureTagline = { 0 };
  hxc_string hxc_l_tmp_sys_println_string_argument_owner_n35 = { 0 };
  hxc_string hxc_l_tmp_string_concat_right_owner_n36 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n365 = { 0 };
  hxc_string hxc_l_tmp_sys_println_string_argument_owner_n37 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n373 = { 0 };
  hxc_string hxc_l_tmp_string_concat_right_owner_n38 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n381 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n387 = { 0 };
  hxc_string hxc_l_tmp_sys_println_string_argument_owner_n39 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n390 = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n396 = { 0 };
  hxc_string hxc_l_tmp = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n400 = { 0 };
  hxc_string hxc_l_tmp_static_call_argument_9_owner_n401 = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n432 = { 0 };
  hxc_string hxc_l_hudView7 = { 0 };
  struct hxc_caxecraft_app_HudView hxc_l_hudView_h0a58428167f7 = { 0 };
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
            else
            {
              struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_tmp_load_result_n798 = hxc_l_contentRegistry;
              hxc_l_gc_roots[194] = (const void *)hxc_l_tmp_load_result_n798;
              struct hxc_optional_caxecraft_gameplay_ItemKind hxc_l_tmp_call_result_n800 = hxc_caxecraft_app_RuntimeInventoryBinding_inventoryKindForRuntimeItem(hxc_l_tmp_load_result_n798, hxc_l_itemCode);
              struct hxc_optional_caxecraft_gameplay_ItemKind hxc_l_inventoryKind = hxc_l_tmp_call_result_n800;
              if (hxc_l_inventoryKind.hxc_has_value)
              {
                struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n802 = hxc_l_session;
                hxc_l_gc_roots[195] = (const void *)hxc_l_tmp_load_result_n802;
                if (hxc_l_tmp_load_result_n802 == NULL)
                {
                  abort();
                }
                int32_t hxc_l_tmp_load_result_n803 = hxc_l_pickupIndex;
                struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_load_result_n804 = hxc_l_inventory;
                struct hxc_optional_caxecraft_gameplay_ItemKind hxc_l_tmp_load_result_n805 = hxc_l_inventoryKind;
                if (!hxc_l_tmp_load_result_n805.hxc_has_value)
                {
                  abort();
                }
                struct hxc_caxecraft_domain_AuthoredInventoryItemResult hxc_l_tmp_instance_call_result_n808 = hxc_caxecraft_domain_GameSession_collectAuthoredInventoryItem(hxc_l_tmp_load_result_n802, hxc_l_tmp_load_result_n803, hxc_l_tmp_load_result_n804, hxc_l_tmp_load_result_n805.hxc_value, hxc_l_loadedItem.hxc_quantity);
                struct hxc_caxecraft_domain_AuthoredInventoryItemResult hxc_l_pickup = hxc_l_tmp_instance_call_result_n808;
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
          struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n814 = hxc_l_session;
          hxc_l_gc_roots[196] = (const void *)hxc_l_tmp_load_result_n814;
          if (hxc_l_tmp_load_result_n814 == NULL)
          {
            abort();
          }
          int32_t hxc_l_tmp_record_field_load_result_n815 = hxc_l_gameTick.hxc_tickIndex;
          struct hxc_caxecraft_domain_ActorControllerTickResult hxc_l_tmp_instance_call_result_n817 = hxc_caxecraft_domain_GameSession_stepAuthoredActorControllers(hxc_l_tmp_load_result_n814, hxc_l_tmp_record_field_load_result_n815, hxc_l_damagePolicy);
          struct hxc_caxecraft_domain_ActorControllerTickResult hxc_l_actorTick = hxc_l_tmp_instance_call_result_n817;
          switch (hxc_l_actorTick.hxc_status.hxc_tag) {
            case hxc_caxecraft_domain_ActorControllerTickStatus_ControllersAdvanced:
              {
                hxc_l_guideInteractionAvailable = false;
                int32_t hxc_l_g_ha2c07b413467 = 0;
                struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n819 = hxc_l_session;
                hxc_l_gc_roots[200] = (const void *)hxc_l_tmp_load_result_n819;
                if (hxc_l_tmp_load_result_n819 == NULL)
                {
                  abort();
                }
                struct hxc_array_ref *hxc_l_tmp_instance_call_result_n820 = hxc_caxecraft_domain_GameSession_actorControllerEventSnapshots(hxc_l_tmp_load_result_n819);
                hxc_l_g1_h4fb5d97fff90 = hxc_l_tmp_instance_call_result_n820;
                while (1)
                {
                  int32_t hxc_l_tmp_load_result_n821 = hxc_l_g_ha2c07b413467;
                  int32_t hxc_l_tmp_array_length_result_n823;
                  if (hxc_array_ref_length(hxc_l_g1_h4fb5d97fff90, &hxc_l_tmp_array_length_result_n823) != HXC_STATUS_OK)
                  {
                    abort();
                  }
                  if (!(hxc_l_tmp_load_result_n821 < hxc_l_tmp_array_length_result_n823))
                  {
                    break;
                  }
                  struct hxc_array_ref *hxc_l_tmp_load_result_n824 = hxc_l_g1_h4fb5d97fff90;
                  struct hxc_caxecraft_domain_ActorControllerEvent hxc_l_tmp_array_get_result_n826;
                  if (hxc_array_ref_get_copy(hxc_l_tmp_load_result_n824, (size_t)hxc_l_g_ha2c07b413467, &hxc_l_tmp_array_get_result_n826) != HXC_STATUS_OK)
                  {
                    abort();
                  }
                  hxc_l_tmp_array_element_owner_n295 = hxc_l_tmp_array_get_result_n826;
                  hxc_l_event_h4aa8988b4fdd = hxc_l_tmp_array_element_owner_n295;
                  if (hxc_enum_8b2a26b8_retain(&hxc_l_event_h4aa8988b4fdd) != HXC_STATUS_OK)
                  {
                    abort();
                  }
                  hxc_l_g_ha2c07b413467 = hxc_i32_add_wrapping(hxc_l_g_ha2c07b413467, 1);
                  switch (hxc_l_event_h4aa8988b4fdd.hxc_tag) {
                    case hxc_caxecraft_domain_ActorControllerEvent_NoControllerEvent:
                      {
                        break;
                      }
                    case hxc_caxecraft_domain_ActorControllerEvent_InteractionAvailable:
                      {
                        if (hxc_l_event_h4aa8988b4fdd.hxc_tag != hxc_caxecraft_domain_ActorControllerEvent_InteractionAvailable)
                        {
                          abort();
                        }
                        int32_t hxc_l_tmp_enum_payload_project_n831 = hxc_l_event_h4aa8988b4fdd.hxc_payload.hxc_InteractionAvailable.hxc_characterId;
                        int32_t hxc_l_source_h2f060cc986d3 = hxc_l_tmp_enum_payload_project_n831;
                        int32_t hxc_l_tmp_load_result_n832 = hxc_l_source_h2f060cc986d3;
                        if (hxc_l_tmp_load_result_n832 == hxc_l_dialogueActorId)
                        {
                          hxc_l_guideInteractionAvailable = true;
                        }
                        break;
                      }
                    case hxc_caxecraft_domain_ActorControllerEvent_LocalPlayerAttack:
                      {
                        if (hxc_l_event_h4aa8988b4fdd.hxc_tag != hxc_caxecraft_domain_ActorControllerEvent_LocalPlayerAttack)
                        {
                          abort();
                        }
                        int32_t hxc_l_tmp_enum_payload_project_n835 = hxc_l_event_h4aa8988b4fdd.hxc_payload.hxc_LocalPlayerAttack.hxc_characterId;
                        int32_t hxc_l_source_h5d99ecd1d57d = hxc_l_tmp_enum_payload_project_n835;
                        int32_t hxc_l_tmp_load_result_n836 = hxc_l_source_h5d99ecd1d57d;
                        if (hxc_l_tmp_load_result_n836 == hxc_l_enemyActorId)
                        {
                          hxc_l_enemyAttackFrames = 120;
                        }
                        break;
                      }
                    case hxc_caxecraft_domain_ActorControllerEvent_DropRequested:
                      {
                        if (hxc_l_event_h4aa8988b4fdd.hxc_tag != hxc_caxecraft_domain_ActorControllerEvent_DropRequested)
                        {
                          abort();
                        }
                        int32_t hxc_l_tmp_enum_payload_project_n839 = hxc_l_event_h4aa8988b4fdd.hxc_payload.hxc_DropRequested.hxc_characterId;
                        int32_t hxc_l_source_hf1dd1dfd0429 = hxc_l_tmp_enum_payload_project_n839;
                        if (hxc_l_event_h4aa8988b4fdd.hxc_tag != hxc_caxecraft_domain_ActorControllerEvent_DropRequested)
                        {
                          abort();
                        }
                        hxc_string hxc_l_tmp_enum_payload_project_n841 = hxc_l_event_h4aa8988b4fdd.hxc_payload.hxc_DropRequested.hxc_drop;
                        hxc_l_drop = hxc_l_tmp_enum_payload_project_n841;
                        if (hxc_string_retain(hxc_l_drop) != HXC_STATUS_OK)
                        {
                          abort();
                        }
                        struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_tmp_load_result_n842 = hxc_l_contentRegistry;
                        hxc_l_gc_roots[201] = (const void *)hxc_l_tmp_load_result_n842;
                        if (hxc_l_tmp_load_result_n842 == NULL)
                        {
                          abort();
                        }
                        int32_t hxc_l_tmp_instance_call_result_n844 = hxc_caxecraft_content_RuntimeContentRegistry_dropQuantityById(hxc_l_tmp_load_result_n842, hxc_l_drop);
                        int32_t hxc_l_quantity = hxc_l_tmp_instance_call_result_n844;
                        struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n845 = hxc_l_session;
                        hxc_l_gc_roots[202] = (const void *)hxc_l_tmp_load_result_n845;
                        if (hxc_l_tmp_load_result_n845 == NULL)
                        {
                          abort();
                        }
                        struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n847 = hxc_caxecraft_domain_GameSession_readCharacter(hxc_l_tmp_load_result_n845, hxc_l_source_hf1dd1dfd0429);
                        struct hxc_caxecraft_domain_Character hxc_l_defeatedActor = hxc_l_tmp_instance_call_result_n847;
                        int32_t hxc_l_tmp_load_result_n848 = hxc_l_quantity;
                        bool hxc_l_tmp_short_circuit_result_n302 = hxc_l_tmp_load_result_n848 <= 0;
                        if (!(hxc_l_tmp_load_result_n848 <= 0))
                        {
                          hxc_l_tmp_short_circuit_result_n302 = hxc_l_defeatedActor.hxc_id <= 0;
                        }
                        if (hxc_l_tmp_short_circuit_result_n302)
                        {
                          hxc_l_quit = true;
                        }
                        else
                        {
                          if (!hxc_l_berryDrop.hxc_active)
                          {
                            struct hxc_caxecraft_domain_Character hxc_l_tmp_load_result_n852 = hxc_l_defeatedActor;
                            struct hxc_caxecraft_gameplay_BerryDropState hxc_l_tmp_call_result_n854 = hxc_caxecraft_gameplay_BerryDrop_fromDefeatedCharacter(hxc_l_tmp_load_result_n852, hxc_l_quantity);
                            hxc_l_berryDrop = hxc_l_tmp_call_result_n854;
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
                  hxc_enum_8b2a26b8_destroy(&hxc_l_event_h4aa8988b4fdd);
                  hxc_array_43e9f8ca_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n295);
                }
                if (hxc_array_ref_release(hxc_l_g1_h4fb5d97fff90) != HXC_STATUS_OK)
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
        struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n855 = hxc_l_session;
        hxc_l_gc_roots[197] = (const void *)hxc_l_tmp_load_result_n855;
        if (hxc_l_tmp_load_result_n855 == NULL)
        {
          abort();
        }
        struct hxc_array_ref *hxc_l_tmp_instance_call_result_n856 = hxc_caxecraft_domain_GameSession_actorControllerStateSnapshots(hxc_l_tmp_load_result_n855);
        hxc_l_actorPhases = hxc_l_tmp_instance_call_result_n856;
        struct hxc_array_ref *hxc_l_tmp_load_result_n857 = hxc_l_actorPhases;
        struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation hxc_l_tmp_call_result_n860 = hxc_caxecraft_app_CaxecraftApp_observeActorPhase(hxc_l_tmp_load_result_n857, hxc_l_dialogueActorId, hxc_caxecraft_domain_ActorControllerPhase_Stationary);
        hxc_l_dialoguePhase = hxc_l_tmp_call_result_n860;
        struct hxc_array_ref *hxc_l_tmp_load_result_n861 = hxc_l_actorPhases;
        struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation hxc_l_tmp_call_result_n864 = hxc_caxecraft_app_CaxecraftApp_observeActorPhase(hxc_l_tmp_load_result_n861, hxc_l_enemyActorId, hxc_caxecraft_domain_ActorControllerPhase_Defeated);
        hxc_l_enemyPhase_h70382f3bdc66 = hxc_l_tmp_call_result_n864;
        struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n865 = hxc_l_session;
        hxc_l_gc_roots[198] = (const void *)hxc_l_tmp_load_result_n865;
        if (hxc_l_tmp_load_result_n865 == NULL)
        {
          abort();
        }
        struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n867 = hxc_caxecraft_domain_GameSession_readCharacter(hxc_l_tmp_load_result_n865, hxc_l_dialogueActorId);
        hxc_l_dialogueActor = hxc_l_tmp_instance_call_result_n867;
        struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n868 = hxc_l_session;
        hxc_l_gc_roots[199] = (const void *)hxc_l_tmp_load_result_n868;
        if (hxc_l_tmp_load_result_n868 == NULL)
        {
          abort();
        }
        struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n870 = hxc_caxecraft_domain_GameSession_readCharacter(hxc_l_tmp_load_result_n868, hxc_l_enemyActorId);
        hxc_l_enemyActor = hxc_l_tmp_instance_call_result_n870;
        bool hxc_l_tmp_record_field_load_result_n871 = hxc_l_dialoguePhase.hxc_valid;
        bool hxc_l_tmp_short_circuit_result_n304 = !hxc_l_tmp_record_field_load_result_n871;
        if (!!hxc_l_tmp_record_field_load_result_n871)
        {
          hxc_l_tmp_short_circuit_result_n304 = !hxc_l_enemyPhase_h70382f3bdc66.hxc_valid;
        }
        bool hxc_l_tmp_short_circuit_load_result_n873 = hxc_l_tmp_short_circuit_result_n304;
        bool hxc_l_tmp_short_circuit_result_n305 = hxc_l_tmp_short_circuit_load_result_n873;
        if (!hxc_l_tmp_short_circuit_load_result_n873)
        {
          hxc_l_tmp_short_circuit_result_n305 = hxc_l_dialogueActor.hxc_id <= 0;
        }
        bool hxc_l_tmp_short_circuit_load_result_n875 = hxc_l_tmp_short_circuit_result_n305;
        bool hxc_l_tmp_short_circuit_result_n306 = hxc_l_tmp_short_circuit_load_result_n875;
        if (!hxc_l_tmp_short_circuit_load_result_n875)
        {
          hxc_l_tmp_short_circuit_result_n306 = hxc_l_enemyActor.hxc_id <= 0;
        }
        if (hxc_l_tmp_short_circuit_result_n306)
        {
          hxc_l_quit = true;
        }
        struct hxc_caxecraft_gameplay_SwordCombatState hxc_l_tmp_call_result_n879 = hxc_caxecraft_gameplay_SwordCombat_step(hxc_l_swordCombat);
        hxc_l_swordCombat = hxc_l_tmp_call_result_n879;
        if (hxc_l_swordQueued)
        {
          struct hxc_caxecraft_gameplay_SwordCombatState hxc_l_tmp_load_result_n881 = hxc_l_swordCombat;
          struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_load_result_n882 = hxc_l_inventory;
          struct hxc_caxecraft_domain_VitalsState hxc_l_tmp_record_field_load_result_n883 = hxc_l_character.hxc_vitals;
          struct hxc_caxecraft_domain_Character hxc_l_tmp_load_result_n884 = hxc_l_enemyActor;
          double hxc_l_tmp_record_field_load_result_n885 = hxc_l_character.hxc_body.hxc_x;
          double hxc_l_tmp_record_field_load_result_n886 = hxc_l_character.hxc_body.hxc_z;
          double hxc_l_tmp_load_result_n887 = hxc_l_lookX;
          int32_t hxc_l_tmp_call_result_n889 = hxc_caxecraft_gameplay_SwordCombat_decide(hxc_l_tmp_load_result_n881, hxc_l_tmp_load_result_n882, hxc_l_tmp_record_field_load_result_n883, hxc_l_tmp_load_result_n884, hxc_l_tmp_record_field_load_result_n885, hxc_l_tmp_record_field_load_result_n886, hxc_l_tmp_load_result_n887, hxc_l_lookZ);
          int32_t hxc_l_swordDecision = hxc_l_tmp_call_result_n889;
          if (hxc_l_swordDecision == 5)
          {
            struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n891 = hxc_l_session;
            hxc_l_gc_roots[203] = (const void *)hxc_l_tmp_load_result_n891;
            if (hxc_l_tmp_load_result_n891 == NULL)
            {
              abort();
            }
            struct hxc_caxecraft_domain_CharacterDamageResult hxc_l_tmp_instance_call_result_n893 = hxc_caxecraft_domain_GameSession_damageCharacter(hxc_l_tmp_load_result_n891, hxc_l_enemyActorId, 1);
            struct hxc_caxecraft_domain_CharacterDamageResult hxc_l_damage = hxc_l_tmp_instance_call_result_n893;
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
          int32_t hxc_l_tmp_load_result_n897 = hxc_l_swordDecision;
          struct hxc_caxecraft_gameplay_SwordCombatState hxc_l_tmp_call_result_n899 = hxc_caxecraft_gameplay_SwordCombat_after(hxc_l_tmp_load_result_n897, hxc_l_swordCombat);
          hxc_l_swordCombat = hxc_l_tmp_call_result_n899;
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
    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n904 = hxc_l_session;
    hxc_l_gc_roots[185] = (const void *)hxc_l_tmp_load_result_n904;
    if (hxc_l_tmp_load_result_n904 == NULL)
    {
      abort();
    }
    size_t hxc_l_tmp_length_n906;
    const uint8_t *hxc_l_tmp_instance_call_result_n905 = hxc_caxecraft_domain_GameSession_worldView(hxc_l_tmp_load_result_n904, &hxc_l_tmp_length_n906);
    double hxc_l_tmp_load_result_n907 = hxc_l_selectionEyeX;
    double hxc_l_tmp_load_result_n908 = hxc_l_selectionEyeY;
    double hxc_l_tmp_load_result_n909 = hxc_l_selectionEyeZ;
    double hxc_l_tmp_load_result_n910 = hxc_l_lookX;
    double hxc_l_tmp_load_result_n911 = hxc_l_lookY;
    struct hxc_caxecraft_domain_RaycastHit hxc_l_tmp_call_result_n913 = hxc_caxecraft_domain_VoxelRaycast_trace(hxc_l_tmp_instance_call_result_n905, hxc_l_tmp_length_n906, hxc_l_tmp_load_result_n907, hxc_l_tmp_load_result_n908, hxc_l_tmp_load_result_n909, hxc_l_tmp_load_result_n910, hxc_l_tmp_load_result_n911, hxc_l_lookZ, 7.0);
    struct hxc_caxecraft_domain_RaycastHit hxc_l_hit = hxc_l_tmp_call_result_n913;
    bool hxc_l_tmp_load_result_n914 = hxc_l_captured_h8abb2355e37f;
    bool hxc_l_tmp_short_circuit_result_n313 = hxc_l_tmp_load_result_n914;
    if (hxc_l_tmp_load_result_n914)
    {
      hxc_l_tmp_short_circuit_result_n313 = !hxc_l_recapturedThisFrame;
    }
    bool hxc_l_tmp_short_circuit_load_result_n916 = hxc_l_tmp_short_circuit_result_n313;
    bool hxc_l_tmp_short_circuit_result_n314 = hxc_l_tmp_short_circuit_load_result_n916;
    if (hxc_l_tmp_short_circuit_load_result_n916)
    {
      hxc_l_tmp_short_circuit_result_n314 = hxc_l_primaryPressed_h86125a319e35;
    }
    if (hxc_l_tmp_short_circuit_result_n314)
    {
      if (hxc_l_character.hxc_vitals.hxc_health > 0)
      {
        if (hxc_l_selectedMode == 1)
        {
          bool hxc_l_tmp_call_result_n923 = hxc_caxecraft_gameplay_Inventory_selectedIs(hxc_l_inventory, hxc_caxecraft_gameplay_ItemKind_CopperSword);
          bool hxc_l_tmp_short_circuit_result_n315 = !hxc_l_tmp_call_result_n923;
          if (!hxc_l_tmp_call_result_n923)
          {
            hxc_l_tmp_short_circuit_result_n315 = hxc_l_hit.hxc_hit;
          }
          bool hxc_l_tmp_short_circuit_load_result_n925 = hxc_l_tmp_short_circuit_result_n315;
          bool hxc_l_tmp_short_circuit_result_n316 = hxc_l_tmp_short_circuit_load_result_n925;
          if (hxc_l_tmp_short_circuit_load_result_n925)
          {
            struct hxc_caxecraft_domain_AquaticState hxc_l_tmp_record_field_load_result_n926 = hxc_l_character.hxc_aquatic;
            bool hxc_l_tmp_call_result_n928 = hxc_caxecraft_domain_Aquatics_canMine(hxc_l_tmp_record_field_load_result_n926, hxc_l_character.hxc_aquaticProfile);
            hxc_l_tmp_short_circuit_result_n316 = hxc_l_tmp_call_result_n928;
          }
          if (hxc_l_tmp_short_circuit_result_n316)
          {
            int32_t hxc_l_tmp_record_field_load_result_n930 = hxc_l_hit.hxc_cellX;
            int32_t hxc_l_tmp_record_field_load_result_n931 = hxc_l_hit.hxc_cellY;
            struct hxc_caxecraft_scenario_VoxelPoint hxc_l_tmp_call_result_n933 = hxc_caxecraft_domain_World_coord(hxc_l_tmp_record_field_load_result_n930, hxc_l_tmp_record_field_load_result_n931, hxc_l_hit.hxc_cellZ);
            struct hxc_caxecraft_scenario_VoxelPoint hxc_l_minedCoordinate = hxc_l_tmp_call_result_n933;
            int32_t hxc_l_sandBefore = hxc_l_inventory.hxc_sand;
            (void)hxc_l_sandBefore;
            struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n935 = hxc_l_session;
            hxc_l_gc_roots[204] = (const void *)hxc_l_tmp_load_result_n935;
            if (hxc_l_tmp_load_result_n935 == NULL)
            {
              abort();
            }
            struct hxc_caxecraft_scenario_VoxelPoint hxc_l_tmp_load_result_n936 = hxc_l_minedCoordinate;
            struct hxc_caxecraft_gameplay_MiningResult hxc_l_tmp_instance_call_result_n938 = hxc_caxecraft_domain_GameSession_mineTerrain(hxc_l_tmp_load_result_n935, hxc_l_tmp_load_result_n936, hxc_l_inventory);
            struct hxc_caxecraft_gameplay_MiningResult hxc_l_mining = hxc_l_tmp_instance_call_result_n938;
            hxc_l_inventory = hxc_l_mining.hxc_inventory;
            if (hxc_l_mining.hxc_outcome == 2)
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
              hxc_caxecraft_app_TerrainChunkCache_invalidate(hxc_l_tmp_owned_class_field_address_n942, hxc_l_minedCoordinate);
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
            int32_t hxc_l_tmp_record_field_load_result_n946 = hxc_l_hit.hxc_cellX;
            int32_t hxc_l_tmp_record_field_load_result_n947 = hxc_l_hit.hxc_cellY;
            struct hxc_caxecraft_scenario_VoxelPoint hxc_l_tmp_call_result_n949 = hxc_caxecraft_domain_World_coord(hxc_l_tmp_record_field_load_result_n946, hxc_l_tmp_record_field_load_result_n947, hxc_l_hit.hxc_cellZ);
            struct hxc_caxecraft_scenario_VoxelPoint hxc_l_removedCoordinate = hxc_l_tmp_call_result_n949;
            struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n950 = hxc_l_session;
            hxc_l_gc_roots[205] = (const void *)hxc_l_tmp_load_result_n950;
            if (hxc_l_tmp_load_result_n950 == NULL)
            {
              abort();
            }
            bool hxc_l_tmp_instance_call_result_n952 = hxc_caxecraft_domain_GameSession_removeTerrain(hxc_l_tmp_load_result_n950, hxc_l_removedCoordinate);
            bool hxc_l_removed = hxc_l_tmp_instance_call_result_n952;
            if (hxc_l_removed)
            {
              if (hxc_l_self == NULL)
              {
                abort();
              }
              struct hxc_caxecraft_app_TerrainRenderer *hxc_l_tmp_owned_class_field_address_n954 = &(*hxc_l_self).hxc_terrainRenderer;
              struct hxc_caxecraft_app_TerrainChunkCache *hxc_l_tmp_owned_class_field_address_n955 = &(*hxc_l_tmp_owned_class_field_address_n954).hxc_cache;
              if (hxc_l_tmp_owned_class_field_address_n955 == NULL)
              {
                abort();
              }
              hxc_caxecraft_app_TerrainChunkCache_invalidate(hxc_l_tmp_owned_class_field_address_n955, hxc_l_removedCoordinate);
            }
          }
        }
      }
    }
    bool hxc_l_tmp_load_result_n957 = hxc_l_captured_h8abb2355e37f;
    bool hxc_l_tmp_short_circuit_result_n322 = hxc_l_tmp_load_result_n957;
    if (hxc_l_tmp_load_result_n957)
    {
      hxc_l_tmp_short_circuit_result_n322 = hxc_l_secondaryPressed_he46a7f5dc259;
    }
    if (hxc_l_tmp_short_circuit_result_n322)
    {
      if (hxc_l_character.hxc_vitals.hxc_health > 0)
      {
        struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n961 = hxc_l_session;
        hxc_l_gc_roots[206] = (const void *)hxc_l_tmp_load_result_n961;
        if (hxc_l_tmp_load_result_n961 == NULL)
        {
          abort();
        }
        struct hxc_caxecraft_domain_LocalRecoveryResult hxc_l_tmp_instance_call_result_n963 = hxc_caxecraft_domain_GameSession_useSelectedRecovery(hxc_l_tmp_load_result_n961, hxc_l_inventory);
        struct hxc_caxecraft_domain_LocalRecoveryResult hxc_l_recovery = hxc_l_tmp_instance_call_result_n963;
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
              int32_t hxc_l_tmp_record_field_load_result_n970 = hxc_l_hit.hxc_previousX;
              int32_t hxc_l_tmp_record_field_load_result_n971 = hxc_l_hit.hxc_previousY;
              struct hxc_caxecraft_scenario_VoxelPoint hxc_l_tmp_call_result_n973 = hxc_caxecraft_domain_World_coord(hxc_l_tmp_record_field_load_result_n970, hxc_l_tmp_record_field_load_result_n971, hxc_l_hit.hxc_previousZ);
              struct hxc_caxecraft_scenario_VoxelPoint hxc_l_placement = hxc_l_tmp_call_result_n973;
              enum hxc_caxecraft_domain_BlockKind hxc_l_tmp_call_result_n975 = hxc_caxecraft_gameplay_Inventory_selectedBlock(hxc_l_inventory);
              enum hxc_caxecraft_domain_BlockKind hxc_l_selectedBlock = hxc_l_tmp_call_result_n975;
              struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_load_result_n976 = hxc_l_inventory;
              int32_t hxc_l_tmp_call_result_n978 = hxc_caxecraft_gameplay_Inventory_countAt(hxc_l_tmp_load_result_n976, hxc_l_inventory.hxc_selected);
              bool hxc_l_hasItem = hxc_l_tmp_call_result_n978 > 0;
              bool hxc_l_tmp_load_result_n979 = hxc_l_hasItem;
              bool hxc_l_tmp_short_circuit_result_n327 = !hxc_l_tmp_load_result_n979;
              if (!!hxc_l_tmp_load_result_n979)
              {
                bool hxc_l_tmp_call_result_n981 = hxc_caxecraft_domain_World_isPlaceable(hxc_l_selectedBlock);
                hxc_l_tmp_short_circuit_result_n327 = !hxc_l_tmp_call_result_n981;
              }
              bool hxc_l_tmp_short_circuit_load_result_n982 = hxc_l_tmp_short_circuit_result_n327;
              bool hxc_l_tmp_short_circuit_result_n328 = hxc_l_tmp_short_circuit_load_result_n982;
              if (!hxc_l_tmp_short_circuit_load_result_n982)
              {
                struct hxc_caxecraft_domain_CharacterBody hxc_l_tmp_record_field_load_result_n983 = hxc_l_character.hxc_body;
                bool hxc_l_tmp_call_result_n985 = hxc_caxecraft_domain_CharacterPhysics_canPlaceAt(hxc_l_tmp_record_field_load_result_n983, hxc_l_placement);
                hxc_l_tmp_short_circuit_result_n328 = !hxc_l_tmp_call_result_n985;
              }
              bool hxc_l_tmp_short_circuit_load_result_n986 = hxc_l_tmp_short_circuit_result_n328;
              bool hxc_l_tmp_short_circuit_result_n329 = hxc_l_tmp_short_circuit_load_result_n986;
              if (!hxc_l_tmp_short_circuit_load_result_n986)
              {
                struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n987 = hxc_l_session;
                hxc_l_gc_roots[207] = (const void *)hxc_l_tmp_load_result_n987;
                if (hxc_l_tmp_load_result_n987 == NULL)
                {
                  abort();
                }
                struct hxc_caxecraft_scenario_VoxelPoint hxc_l_tmp_load_result_n988 = hxc_l_placement;
                bool hxc_l_tmp_instance_call_result_n990 = hxc_caxecraft_domain_GameSession_placeTerrain(hxc_l_tmp_load_result_n987, hxc_l_tmp_load_result_n988, hxc_l_selectedBlock);
                hxc_l_tmp_short_circuit_result_n329 = !hxc_l_tmp_instance_call_result_n990;
              }
              if (hxc_l_tmp_short_circuit_result_n329)
              {
                hxc_l_placementBlockedFrames = 60;
              }
              else
              {
                if (hxc_l_self == NULL)
                {
                  abort();
                }
                struct hxc_caxecraft_app_TerrainRenderer *hxc_l_tmp_owned_class_field_address_n992 = &(*hxc_l_self).hxc_terrainRenderer;
                struct hxc_caxecraft_app_TerrainChunkCache *hxc_l_tmp_owned_class_field_address_n993 = &(*hxc_l_tmp_owned_class_field_address_n992).hxc_cache;
                if (hxc_l_tmp_owned_class_field_address_n993 == NULL)
                {
                  abort();
                }
                hxc_caxecraft_app_TerrainChunkCache_invalidate(hxc_l_tmp_owned_class_field_address_n993, hxc_l_placement);
                if (hxc_l_selectedMode == 1)
                {
                  struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n997 = hxc_caxecraft_gameplay_Inventory_consumeSelected(hxc_l_inventory);
                  hxc_l_inventory = hxc_l_tmp_call_result_n997;
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
      struct hxc_caxecraft_gameplay_BerryDropState hxc_l_tmp_load_result_n1001 = hxc_l_berryDrop;
      double hxc_l_tmp_record_field_load_result_n1002 = hxc_l_character.hxc_body.hxc_x;
      double hxc_l_tmp_record_field_load_result_n1003 = hxc_l_character.hxc_body.hxc_y;
      bool hxc_l_tmp_call_result_n1005 = hxc_caxecraft_gameplay_BerryDrop_isInRange(hxc_l_tmp_load_result_n1001, hxc_l_tmp_record_field_load_result_n1002, hxc_l_tmp_record_field_load_result_n1003, hxc_l_character.hxc_body.hxc_z);
      if (hxc_l_tmp_call_result_n1005)
      {
        struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_load_result_n1006 = hxc_l_inventory;
        int32_t hxc_l_tmp_call_result_n1009 = hxc_caxecraft_gameplay_Inventory_acceptedAmount(hxc_l_tmp_load_result_n1006, hxc_caxecraft_gameplay_ItemKind_Berries, hxc_l_berryDrop.hxc_amount);
        int32_t hxc_l_acceptedDrop = hxc_l_tmp_call_result_n1009;
        if (hxc_l_acceptedDrop > 0)
        {
          struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_load_result_n1011 = hxc_l_inventory;
          struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n1014 = hxc_caxecraft_gameplay_Inventory_collectItem(hxc_l_tmp_load_result_n1011, hxc_caxecraft_gameplay_ItemKind_Berries, hxc_l_acceptedDrop);
          hxc_l_inventory = hxc_l_tmp_call_result_n1014;
          struct hxc_caxecraft_gameplay_BerryDropState hxc_l_tmp_load_result_n1015 = hxc_l_berryDrop;
          struct hxc_caxecraft_gameplay_BerryDropState hxc_l_tmp_call_result_n1017 = hxc_caxecraft_gameplay_BerryDrop_collectAmount(hxc_l_tmp_load_result_n1015, hxc_l_acceptedDrop);
          hxc_l_berryDrop = hxc_l_tmp_call_result_n1017;
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
    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1034 = hxc_l_session;
    hxc_l_gc_roots[208] = (const void *)hxc_l_tmp_load_result_n1034;
    if (hxc_l_tmp_load_result_n1034 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_domain_GameView hxc_l_tmp_instance_call_result_n1035 = hxc_caxecraft_domain_GameSession_view(hxc_l_tmp_load_result_n1034);
    struct hxc_caxecraft_domain_GameView hxc_l_committedView = hxc_l_tmp_instance_call_result_n1035;
    if (!hxc_l_committedView.hxc_valid)
    {
      hxc_l_quit = true;
    }
    else
    {
      hxc_l_character = hxc_l_committedView.hxc_localPlayer;
    }
    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1038 = hxc_l_session;
    hxc_l_gc_roots[209] = (const void *)hxc_l_tmp_load_result_n1038;
    if (hxc_l_tmp_load_result_n1038 == NULL)
    {
      abort();
    }
    bool hxc_l_tmp_instance_call_result_n1040 = hxc_caxecraft_domain_GameSession_actorInteractionAvailable(hxc_l_tmp_load_result_n1038, hxc_l_dialogueActorId);
    hxc_l_guideInteractionAvailable = hxc_l_tmp_instance_call_result_n1040;
    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1041 = hxc_l_session;
    hxc_l_gc_roots[210] = (const void *)hxc_l_tmp_load_result_n1041;
    if (hxc_l_tmp_load_result_n1041 == NULL)
    {
      abort();
    }
    struct hxc_array_ref *hxc_l_tmp_instance_call_result_n1042 = hxc_caxecraft_domain_GameSession_actorControllerStateSnapshots(hxc_l_tmp_load_result_n1041);
    hxc_l_presentationActorPhases = hxc_l_tmp_instance_call_result_n1042;
    struct hxc_array_ref *hxc_l_tmp_load_result_n1043 = hxc_l_presentationActorPhases;
    struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation hxc_l_tmp_call_result_n1046 = hxc_caxecraft_app_CaxecraftApp_observeActorPhase(hxc_l_tmp_load_result_n1043, hxc_l_dialogueActorId, hxc_caxecraft_domain_ActorControllerPhase_Stationary);
    hxc_l_dialoguePhase = hxc_l_tmp_call_result_n1046;
    struct hxc_array_ref *hxc_l_tmp_load_result_n1047 = hxc_l_presentationActorPhases;
    struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation hxc_l_tmp_call_result_n1050 = hxc_caxecraft_app_CaxecraftApp_observeActorPhase(hxc_l_tmp_load_result_n1047, hxc_l_enemyActorId, hxc_caxecraft_domain_ActorControllerPhase_Defeated);
    hxc_l_enemyPhase_h70382f3bdc66 = hxc_l_tmp_call_result_n1050;
    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1051 = hxc_l_session;
    hxc_l_gc_roots[211] = (const void *)hxc_l_tmp_load_result_n1051;
    if (hxc_l_tmp_load_result_n1051 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n1053 = hxc_caxecraft_domain_GameSession_readCharacter(hxc_l_tmp_load_result_n1051, hxc_l_dialogueActorId);
    hxc_l_dialogueActor = hxc_l_tmp_instance_call_result_n1053;
    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1054 = hxc_l_session;
    hxc_l_gc_roots[212] = (const void *)hxc_l_tmp_load_result_n1054;
    if (hxc_l_tmp_load_result_n1054 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n1056 = hxc_caxecraft_domain_GameSession_readCharacter(hxc_l_tmp_load_result_n1054, hxc_l_enemyActorId);
    hxc_l_enemyActor = hxc_l_tmp_instance_call_result_n1056;
    bool hxc_l_tmp_record_field_load_result_n1057 = hxc_l_dialoguePhase.hxc_valid;
    bool hxc_l_tmp_short_circuit_result_n333 = !hxc_l_tmp_record_field_load_result_n1057;
    if (!!hxc_l_tmp_record_field_load_result_n1057)
    {
      hxc_l_tmp_short_circuit_result_n333 = !hxc_l_enemyPhase_h70382f3bdc66.hxc_valid;
    }
    bool hxc_l_tmp_short_circuit_load_result_n1059 = hxc_l_tmp_short_circuit_result_n333;
    bool hxc_l_tmp_short_circuit_result_n334 = hxc_l_tmp_short_circuit_load_result_n1059;
    if (!hxc_l_tmp_short_circuit_load_result_n1059)
    {
      hxc_l_tmp_short_circuit_result_n334 = hxc_l_dialogueActor.hxc_id <= 0;
    }
    bool hxc_l_tmp_short_circuit_load_result_n1061 = hxc_l_tmp_short_circuit_result_n334;
    bool hxc_l_tmp_short_circuit_result_n335 = hxc_l_tmp_short_circuit_load_result_n1061;
    if (!hxc_l_tmp_short_circuit_load_result_n1061)
    {
      hxc_l_tmp_short_circuit_result_n335 = hxc_l_enemyActor.hxc_id <= 0;
    }
    if (hxc_l_tmp_short_circuit_result_n335)
    {
      hxc_l_quit = true;
    }
    int32_t hxc_l_completedTicks = hxc_l_committedView.hxc_completedTicks;
    if (hxc_l_resetMotionThisFrame)
    {
      struct hxc_caxecraft_app_MotionHistory hxc_l_tmp_call_result_n1067 = hxc_caxecraft_app_MotionInterpolation_start(hxc_l_character.hxc_body);
      hxc_l_motionHistory = hxc_l_tmp_call_result_n1067;
    }
    struct hxc_caxecraft_app_MotionHistory hxc_l_tmp_load_result_n1068 = hxc_l_motionHistory;
    struct hxc_caxecraft_app_RenderPosition hxc_l_tmp_call_result_n1070 = hxc_caxecraft_app_MotionInterpolation_sample(hxc_l_tmp_load_result_n1068, hxc_l_accumulator, 0.05);
    struct hxc_caxecraft_app_RenderPosition hxc_l_renderPosition = hxc_l_tmp_call_result_n1070;
    double hxc_l_eyeX = hxc_l_renderPosition.hxc_x;
    double hxc_l_eyeY = hxc_l_renderPosition.hxc_y + 1.62;
    double hxc_l_eyeZ = hxc_l_renderPosition.hxc_z;
    double hxc_l_tmp_load_result_n1074 = hxc_l_eyeX;
    double hxc_l_tmp_load_result_n1075 = hxc_l_eyeY;
    double hxc_l_tmp_load_result_n1076 = hxc_l_eyeZ;
    double hxc_l_tmp_load_result_n1078 = hxc_l_eyeX;
    double hxc_l_tmp_load_result_n1079 = hxc_l_lookX;
    double hxc_l_tmp_load_result_n1080 = hxc_l_eyeY;
    double hxc_l_tmp_load_result_n1081 = hxc_l_lookY;
    double hxc_l_tmp_load_result_n1082 = hxc_l_eyeZ;
    struct Camera3D hxc_l_camera = (struct Camera3D){ .position = (struct Vector3){ .x = (float)hxc_l_tmp_load_result_n1074, .y = (float)hxc_l_tmp_load_result_n1075, .z = (float)hxc_l_tmp_load_result_n1076 }, .target = (struct Vector3){ .x = (float)(hxc_l_tmp_load_result_n1078 + hxc_l_tmp_load_result_n1079), .y = (float)(hxc_l_tmp_load_result_n1080 + hxc_l_tmp_load_result_n1081), .z = (float)(hxc_l_tmp_load_result_n1082 + hxc_l_lookZ) }, .up = (struct Vector3){ .x = (float)0.0, .y = (float)1.0, .z = (float)0.0 }, .fovy = (float)70.0, .projection = 0 };
    BeginDrawing();
    if (hxc_l_onTitle)
    {
      hxc_string hxc_l_locale1_h0700d164bd9f = { 0 };
      switch (hxc_l_locale) {
        case 0:
          {
            hxc_l_locale1_h0700d164bd9f = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
            if (hxc_string_retain(hxc_l_locale1_h0700d164bd9f) != HXC_STATUS_OK)
            {
              abort();
            }
            break;
          }
        case 1:
          {
            hxc_l_locale1_h0700d164bd9f = (hxc_string){ (const uint8_t *)"es-mx", 5, true, NULL };
            if (hxc_string_retain(hxc_l_locale1_h0700d164bd9f) != HXC_STATUS_OK)
            {
              abort();
            }
            break;
          }
        default:
          {
            hxc_l_locale1_h0700d164bd9f = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
            if (hxc_string_retain(hxc_l_locale1_h0700d164bd9f) != HXC_STATUS_OK)
            {
              abort();
            }
            break;
          }
      }
      hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n1089 = hxc_l_locale1_h0700d164bd9f;
      hxc_l_tmp_managed_flow_owner_n343 = hxc_l_tmp_managed_flow_carrier_move_result_n1089;
      struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1090 = hxc_l_levelView;
      hxc_l_gc_roots[213] = (const void *)hxc_l_tmp_load_result_n1090;
      if (hxc_l_tmp_load_result_n1090 == NULL)
      {
        abort();
      }
      struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_class_field_load_result_n1091 = (*hxc_l_tmp_load_result_n1090).hxc_levelPresentation;
      hxc_l_gc_roots[214] = (const void *)hxc_l_tmp_class_field_load_result_n1091;
      if (hxc_l_tmp_class_field_load_result_n1091 == NULL)
      {
        abort();
      }
      hxc_string hxc_l_tmp_instance_call_result_n1093 = hxc_caxecraft_content_RuntimeLevelPresentation_message(hxc_l_tmp_class_field_load_result_n1091, (hxc_string){ (const uint8_t *)"adventure_tagline", 17, true, NULL }, hxc_l_tmp_managed_flow_owner_n343);
      hxc_l_adventureTagline = hxc_l_tmp_instance_call_result_n1093;
      int32_t hxc_l_tmp_native_call_result_n1094 = GetScreenWidth();
      int32_t hxc_l_width_h7644f82bd34f = (int32_t)hxc_l_tmp_native_call_result_n1094;
      int32_t hxc_l_tmp_native_call_result_n1095 = GetScreenHeight();
      int32_t hxc_l_height_hab0c42b4ff0b = (int32_t)hxc_l_tmp_native_call_result_n1095;
      if (hxc_l_titleTextureReady)
      {
        struct Color hxc_l_this1_h956ec650a98d = (struct Color){ .r = 255, .g = 255, .b = 255, .a = 255 };
        struct Texture hxc_l_tmp_load_result_n1098 = hxc_l_titleTexture;
        double hxc_l_imageAspect = hxc_f64_divide_zero_safe((double)(int32_t)hxc_l_tmp_load_result_n1098.width, (double)(int32_t)hxc_l_titleTexture.height);
        int32_t hxc_l_tmp_load_result_n1102 = hxc_l_width_h7644f82bd34f;
        double hxc_l_screenAspect = hxc_f64_divide_zero_safe((double)hxc_l_tmp_load_result_n1102, (double)hxc_l_height_hab0c42b4ff0b);
        double hxc_l_sourceX = 0.0;
        double hxc_l_sourceY = 0.0;
        double hxc_l_sourceWidth = (double)(int32_t)hxc_l_titleTexture.width;
        double hxc_l_sourceHeight = (double)(int32_t)hxc_l_titleTexture.height;
        double hxc_l_tmp_load_result_n1108 = hxc_l_screenAspect;
        if (hxc_l_tmp_load_result_n1108 > hxc_l_imageAspect)
        {
          struct Texture hxc_l_tmp_load_result_n1110 = hxc_l_titleTexture;
          hxc_l_sourceHeight = hxc_f64_divide_zero_safe((double)(int32_t)hxc_l_tmp_load_result_n1110.width, hxc_l_screenAspect);
          struct Texture hxc_l_tmp_load_result_n1113 = hxc_l_titleTexture;
          hxc_l_sourceY = ((double)(int32_t)hxc_l_tmp_load_result_n1113.height - hxc_l_sourceHeight) * 0.5;
        }
        else
        {
          struct Texture hxc_l_tmp_load_result_n1116 = hxc_l_titleTexture;
          hxc_l_sourceWidth = (double)(int32_t)hxc_l_tmp_load_result_n1116.height * hxc_l_screenAspect;
          struct Texture hxc_l_tmp_load_result_n1119 = hxc_l_titleTexture;
          hxc_l_sourceX = ((double)(int32_t)hxc_l_tmp_load_result_n1119.width - hxc_l_sourceWidth) * 0.5;
        }
        struct Texture hxc_l_tmp_load_result_n1122 = hxc_l_titleTexture;
        double hxc_l_tmp_load_result_n1123 = hxc_l_sourceX;
        double hxc_l_tmp_load_result_n1124 = hxc_l_sourceY;
        double hxc_l_tmp_load_result_n1125 = hxc_l_sourceWidth;
        double hxc_l_tmp_load_result_n1126 = hxc_l_sourceHeight;
        int32_t hxc_l_tmp_load_result_n1128 = hxc_l_width_h7644f82bd34f;
        int32_t hxc_l_tmp_load_result_n1129 = hxc_l_height_hab0c42b4ff0b;
        DrawTexturePro(hxc_l_tmp_load_result_n1122, (struct Rectangle){ .x = (float)hxc_l_tmp_load_result_n1123, .y = (float)hxc_l_tmp_load_result_n1124, .width = (float)hxc_l_tmp_load_result_n1125, .height = (float)hxc_l_tmp_load_result_n1126 }, (struct Rectangle){ .x = (float)0.0, .y = (float)0.0, .width = (float)((double)hxc_l_tmp_load_result_n1128 + 0.0), .height = (float)((double)hxc_l_tmp_load_result_n1129 + 0.0) }, (struct Vector2){ .x = (float)0.0, .y = (float)0.0 }, (float)0.0, hxc_l_this1_h956ec650a98d);
      }
      else
      {
        ClearBackground((struct Color){ .r = 126, .g = 190, .b = 201, .a = 255 });
      }
      struct Color hxc_l_this1_hfae3d0c5ee8d = (struct Color){ .r = 4, .g = 14, .b = 23, .a = 132 };
      int32_t hxc_l_tmp_load_result_n1135 = hxc_l_width_h7644f82bd34f;
      int32_t hxc_l_tmp_load_result_n1136 = hxc_l_height_hab0c42b4ff0b;
      DrawRectangle((int32_t)0, (int32_t)0, (int32_t)hxc_l_tmp_load_result_n1135, (int32_t)hxc_l_tmp_load_result_n1136, hxc_l_this1_hfae3d0c5ee8d);
      if (hxc_l_wordmarkTextureReady)
      {
        int32_t hxc_l_centerX = hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_l_width_h7644f82bd34f, (double)2));
        struct Color hxc_l_this1_h3961179d5921 = (struct Color){ .r = 255, .g = 255, .b = 255, .a = 255 };
        double hxc_l_scale = hxc_f64_divide_zero_safe((double)560, (double)(int32_t)hxc_l_wordmarkTexture.width);
        double hxc_l_heightScale = hxc_f64_divide_zero_safe((double)132, (double)(int32_t)hxc_l_wordmarkTexture.height);
        double hxc_l_tmp_load_result_n1145 = hxc_l_heightScale;
        if (hxc_l_tmp_load_result_n1145 < hxc_l_scale)
        {
          hxc_l_scale = hxc_l_heightScale;
        }
        struct Texture hxc_l_tmp_load_result_n1148 = hxc_l_wordmarkTexture;
        double hxc_l_width_h569ed16dc804 = (double)(int32_t)hxc_l_tmp_load_result_n1148.width * hxc_l_scale;
        struct Texture hxc_l_tmp_load_result_n1151 = hxc_l_wordmarkTexture;
        double hxc_l_height_h1556df8bb1a1 = (double)(int32_t)hxc_l_tmp_load_result_n1151.height * hxc_l_scale;
        double hxc_l_width1 = (double)(int32_t)hxc_l_wordmarkTexture.width + 0.0;
        double hxc_l_height1 = (double)(int32_t)hxc_l_wordmarkTexture.height + 0.0;
        struct Texture hxc_l_tmp_load_result_n1158 = hxc_l_wordmarkTexture;
        double hxc_l_tmp_load_result_n1159 = hxc_l_width1;
        double hxc_l_tmp_load_result_n1160 = hxc_l_height1;
        int32_t hxc_l_tmp_load_result_n1162 = hxc_l_centerX;
        double hxc_l_tmp_load_result_n1163 = hxc_l_width_h569ed16dc804;
        double hxc_l_tmp_load_result_n1164 = hxc_l_width_h569ed16dc804;
        double hxc_l_tmp_load_result_n1165 = hxc_l_height_h1556df8bb1a1;
        DrawTexturePro(hxc_l_tmp_load_result_n1158, (struct Rectangle){ .x = (float)0.0, .y = (float)0.0, .width = (float)hxc_l_tmp_load_result_n1159, .height = (float)hxc_l_tmp_load_result_n1160 }, (struct Rectangle){ .x = (float)((double)hxc_l_tmp_load_result_n1162 - hxc_l_tmp_load_result_n1163 * 0.5), .y = (float)28.0, .width = (float)hxc_l_tmp_load_result_n1164, .height = (float)hxc_l_tmp_load_result_n1165 }, (struct Vector2){ .x = (float)0.0, .y = (float)0.0 }, (float)0.0, hxc_l_this1_h3961179d5921);
      }
      else
      {
        int32_t hxc_l_x_h799a44f5230b = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_l_width_h7644f82bd34f, (double)2)), 92);
        struct Color hxc_l_this1_hfa5706b11602 = (struct Color){ .r = 242, .g = 249, .b = 245, .a = 255 };
        struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_tmp_load_result_n1171 = hxc_l_uiCatalog;
        hxc_l_gc_roots[215] = (const void *)hxc_l_tmp_load_result_n1171;
        if (hxc_l_tmp_load_result_n1171 == NULL)
        {
          abort();
        }
        hxc_string hxc_l_tmp_instance_call_result_n1173 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_tmp_load_result_n1171, hxc_l_locale, 34);
        hxc_l_tmp_cstring_ref_owner_owner_n365 = hxc_l_tmp_instance_call_result_n1173;
        hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n1175 = HXC_BORROWED_CSTRING_INITIALIZER;
        if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n365, &hxc_l_tmp_string_borrow_cstring_result_n1175) != HXC_STATUS_OK)
        {
          abort();
        }
        int32_t hxc_l_tmp_load_result_n1176 = hxc_l_x_h799a44f5230b;
        DrawText(hxc_l_tmp_string_borrow_cstring_result_n1175.data, (int32_t)hxc_l_tmp_load_result_n1176, (int32_t)52, (int32_t)34, hxc_l_this1_hfa5706b11602);
        if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n365) != HXC_STATUS_OK)
        {
          abort();
        }
      }
      int32_t hxc_l_firstTop = hxc_i32_subtract_wrapping(hxc_l_height_hab0c42b4ff0b, 293);
      int32_t hxc_l_left_hfc3ce22180f0 = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_l_width_h7644f82bd34f, (double)2)), 180);
      if (hxc_l_selectedMode == 0)
      {
        struct Color hxc_l_this1_h2bf46de51f33 = (struct Color){ .r = 16, .g = 88, .b = 102, .a = 232 };
        int32_t hxc_l_tmp_load_result_n1182 = hxc_l_left_hfc3ce22180f0;
        int32_t hxc_l_tmp_load_result_n1183 = hxc_l_firstTop;
        DrawRectangle((int32_t)hxc_l_tmp_load_result_n1182, (int32_t)hxc_l_tmp_load_result_n1183, (int32_t)360, (int32_t)54, hxc_l_this1_h2bf46de51f33);
        struct Color hxc_l_this1_hfa2cf41fa07b = (struct Color){ .r = 255, .g = 190, .b = 55, .a = 255 };
        int32_t hxc_l_tmp_load_result_n1186 = hxc_l_left_hfc3ce22180f0;
        int32_t hxc_l_tmp_load_result_n1187 = hxc_l_firstTop;
        DrawRectangleLines((int32_t)hxc_l_tmp_load_result_n1186, (int32_t)hxc_l_tmp_load_result_n1187, (int32_t)360, (int32_t)54, hxc_l_this1_hfa2cf41fa07b);
      }
      else
      {
        struct Color hxc_l_this1_h37c29b5bdcf5 = (struct Color){ .r = 6, .g = 26, .b = 36, .a = 218 };
        int32_t hxc_l_tmp_load_result_n1190 = hxc_l_left_hfc3ce22180f0;
        int32_t hxc_l_tmp_load_result_n1191 = hxc_l_firstTop;
        DrawRectangle((int32_t)hxc_l_tmp_load_result_n1190, (int32_t)hxc_l_tmp_load_result_n1191, (int32_t)360, (int32_t)54, hxc_l_this1_h37c29b5bdcf5);
        struct Color hxc_l_this1_hada760542d55 = (struct Color){ .r = 92, .g = 194, .b = 188, .a = 255 };
        int32_t hxc_l_tmp_load_result_n1194 = hxc_l_left_hfc3ce22180f0;
        int32_t hxc_l_tmp_load_result_n1195 = hxc_l_firstTop;
        DrawRectangleLines((int32_t)hxc_l_tmp_load_result_n1194, (int32_t)hxc_l_tmp_load_result_n1195, (int32_t)360, (int32_t)54, hxc_l_this1_hada760542d55);
      }
      struct Color hxc_l_this1_hf1ebe3fa4b17 = (struct Color){ .r = 242, .g = 249, .b = 245, .a = 255 };
      struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_tmp_load_result_n1198 = hxc_l_uiCatalog;
      hxc_l_gc_roots[216] = (const void *)hxc_l_tmp_load_result_n1198;
      if (hxc_l_tmp_load_result_n1198 == NULL)
      {
        abort();
      }
      hxc_string hxc_l_tmp_instance_call_result_n1200 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_tmp_load_result_n1198, hxc_l_locale, 27);
      hxc_l_tmp_cstring_ref_owner_owner_n373 = hxc_l_tmp_instance_call_result_n1200;
      hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n1202 = HXC_BORROWED_CSTRING_INITIALIZER;
      if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n373, &hxc_l_tmp_string_borrow_cstring_result_n1202) != HXC_STATUS_OK)
      {
        abort();
      }
      int32_t hxc_l_tmp_load_result_n1203 = hxc_l_left_hfc3ce22180f0;
      int32_t hxc_l_tmp_load_result_n1204 = hxc_l_firstTop;
      DrawText(hxc_l_tmp_string_borrow_cstring_result_n1202.data, (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n1203, 28), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n1204, 16), (int32_t)21, hxc_l_this1_hf1ebe3fa4b17);
      int32_t hxc_l_top_hb915979d1d02 = hxc_i32_add_wrapping(hxc_i32_add_wrapping(hxc_l_firstTop, 54), 14);
      int32_t hxc_l_left_hf112ab9bea20 = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_l_width_h7644f82bd34f, (double)2)), 180);
      if (hxc_l_selectedMode == 1)
      {
        struct Color hxc_l_this1_h97e2d84fe1a6 = (struct Color){ .r = 16, .g = 88, .b = 102, .a = 232 };
        int32_t hxc_l_tmp_load_result_n1210 = hxc_l_left_hf112ab9bea20;
        int32_t hxc_l_tmp_load_result_n1211 = hxc_l_top_hb915979d1d02;
        DrawRectangle((int32_t)hxc_l_tmp_load_result_n1210, (int32_t)hxc_l_tmp_load_result_n1211, (int32_t)360, (int32_t)54, hxc_l_this1_h97e2d84fe1a6);
        struct Color hxc_l_this1_he0c40ca71a96 = (struct Color){ .r = 255, .g = 190, .b = 55, .a = 255 };
        int32_t hxc_l_tmp_load_result_n1214 = hxc_l_left_hf112ab9bea20;
        int32_t hxc_l_tmp_load_result_n1215 = hxc_l_top_hb915979d1d02;
        DrawRectangleLines((int32_t)hxc_l_tmp_load_result_n1214, (int32_t)hxc_l_tmp_load_result_n1215, (int32_t)360, (int32_t)54, hxc_l_this1_he0c40ca71a96);
      }
      else
      {
        struct Color hxc_l_this1_hd3da2428b08f = (struct Color){ .r = 6, .g = 26, .b = 36, .a = 218 };
        int32_t hxc_l_tmp_load_result_n1218 = hxc_l_left_hf112ab9bea20;
        int32_t hxc_l_tmp_load_result_n1219 = hxc_l_top_hb915979d1d02;
        DrawRectangle((int32_t)hxc_l_tmp_load_result_n1218, (int32_t)hxc_l_tmp_load_result_n1219, (int32_t)360, (int32_t)54, hxc_l_this1_hd3da2428b08f);
        struct Color hxc_l_this1_hdc79b3c468e2 = (struct Color){ .r = 92, .g = 194, .b = 188, .a = 255 };
        int32_t hxc_l_tmp_load_result_n1222 = hxc_l_left_hf112ab9bea20;
        int32_t hxc_l_tmp_load_result_n1223 = hxc_l_top_hb915979d1d02;
        DrawRectangleLines((int32_t)hxc_l_tmp_load_result_n1222, (int32_t)hxc_l_tmp_load_result_n1223, (int32_t)360, (int32_t)54, hxc_l_this1_hdc79b3c468e2);
      }
      struct Color hxc_l_this1_h9b7d45581483 = (struct Color){ .r = 242, .g = 249, .b = 245, .a = 255 };
      struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_tmp_load_result_n1226 = hxc_l_uiCatalog;
      hxc_l_gc_roots[217] = (const void *)hxc_l_tmp_load_result_n1226;
      if (hxc_l_tmp_load_result_n1226 == NULL)
      {
        abort();
      }
      hxc_string hxc_l_tmp_instance_call_result_n1228 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_tmp_load_result_n1226, hxc_l_locale, 26);
      hxc_l_tmp_cstring_ref_owner_owner_n381 = hxc_l_tmp_instance_call_result_n1228;
      hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n1230 = HXC_BORROWED_CSTRING_INITIALIZER;
      if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n381, &hxc_l_tmp_string_borrow_cstring_result_n1230) != HXC_STATUS_OK)
      {
        abort();
      }
      int32_t hxc_l_tmp_load_result_n1231 = hxc_l_left_hf112ab9bea20;
      int32_t hxc_l_tmp_load_result_n1232 = hxc_l_top_hb915979d1d02;
      DrawText(hxc_l_tmp_string_borrow_cstring_result_n1230.data, (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n1231, 28), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n1232, 16), (int32_t)21, hxc_l_this1_h9b7d45581483);
      int32_t hxc_l_top_hfc44e3a59c96 = hxc_i32_add_wrapping(hxc_l_firstTop, 136);
      int32_t hxc_l_left_h01644e78d0f0 = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_l_width_h7644f82bd34f, (double)2)), 180);
      struct Color hxc_l_this1_h65e5e0b6262a = (struct Color){ .r = 6, .g = 26, .b = 36, .a = 218 };
      int32_t hxc_l_tmp_load_result_n1237 = hxc_l_left_h01644e78d0f0;
      int32_t hxc_l_tmp_load_result_n1238 = hxc_l_top_hfc44e3a59c96;
      DrawRectangle((int32_t)hxc_l_tmp_load_result_n1237, (int32_t)hxc_l_tmp_load_result_n1238, (int32_t)360, (int32_t)54, hxc_l_this1_h65e5e0b6262a);
      struct Color hxc_l_this1_h826f3ab9f493 = (struct Color){ .r = 92, .g = 194, .b = 188, .a = 255 };
      int32_t hxc_l_tmp_load_result_n1241 = hxc_l_left_h01644e78d0f0;
      int32_t hxc_l_tmp_load_result_n1242 = hxc_l_top_hfc44e3a59c96;
      DrawRectangleLines((int32_t)hxc_l_tmp_load_result_n1241, (int32_t)hxc_l_tmp_load_result_n1242, (int32_t)360, (int32_t)54, hxc_l_this1_h826f3ab9f493);
      struct Color hxc_l_this1_hfc659da9244b = (struct Color){ .r = 242, .g = 249, .b = 245, .a = 255 };
      struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_tmp_load_result_n1245 = hxc_l_uiCatalog;
      hxc_l_gc_roots[218] = (const void *)hxc_l_tmp_load_result_n1245;
      if (hxc_l_tmp_load_result_n1245 == NULL)
      {
        abort();
      }
      hxc_string hxc_l_tmp_instance_call_result_n1247 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_tmp_load_result_n1245, hxc_l_locale, 28);
      hxc_l_tmp_cstring_ref_owner_owner_n387 = hxc_l_tmp_instance_call_result_n1247;
      hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n1249 = HXC_BORROWED_CSTRING_INITIALIZER;
      if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n387, &hxc_l_tmp_string_borrow_cstring_result_n1249) != HXC_STATUS_OK)
      {
        abort();
      }
      int32_t hxc_l_tmp_load_result_n1250 = hxc_l_left_h01644e78d0f0;
      int32_t hxc_l_tmp_load_result_n1251 = hxc_l_top_hfc44e3a59c96;
      DrawText(hxc_l_tmp_string_borrow_cstring_result_n1249.data, (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n1250, 28), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n1251, 16), (int32_t)21, hxc_l_this1_hfc659da9244b);
      int32_t hxc_l_x_h285f9ae97e48 = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_l_width_h7644f82bd34f, (double)2)), 285);
      struct Color hxc_l_this1_h7fd8cce904ea = (struct Color){ .r = 229, .g = 241, .b = 235, .a = 255 };
      struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_tmp_load_result_n1255 = hxc_l_uiCatalog;
      hxc_l_gc_roots[219] = (const void *)hxc_l_tmp_load_result_n1255;
      if (hxc_l_tmp_load_result_n1255 == NULL)
      {
        abort();
      }
      hxc_string hxc_l_tmp_instance_call_result_n1257 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_tmp_load_result_n1255, hxc_l_locale, 29);
      hxc_l_tmp_cstring_ref_owner_owner_n390 = hxc_l_tmp_instance_call_result_n1257;
      hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n1259 = HXC_BORROWED_CSTRING_INITIALIZER;
      if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n390, &hxc_l_tmp_string_borrow_cstring_result_n1259) != HXC_STATUS_OK)
      {
        abort();
      }
      int32_t hxc_l_tmp_load_result_n1260 = hxc_l_x_h285f9ae97e48;
      int32_t hxc_l_tmp_load_result_n1261 = hxc_l_height_hab0c42b4ff0b;
      DrawText(hxc_l_tmp_string_borrow_cstring_result_n1259.data, (int32_t)hxc_l_tmp_load_result_n1260, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n1261, 58), (int32_t)16, hxc_l_this1_h7fd8cce904ea);
      int32_t hxc_l_tmp_load_result_n1263 = hxc_l_selectedMode;
      bool hxc_l_tmp_short_circuit_result_n391 = hxc_l_tmp_load_result_n1263 == 1;
      if (hxc_l_tmp_load_result_n1263 == 1)
      {
        hxc_string hxc_l_tmp_load_result_n1264 = hxc_l_adventureTagline;
        if (hxc_l_tmp_load_result_n1264.data == NULL)
        {
          abort();
        }
        int32_t hxc_l_tmp_string_length_result_n1265;
        if (hxc_string_haxe_length(hxc_l_tmp_load_result_n1264, &hxc_l_tmp_string_length_result_n1265) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_l_tmp_short_circuit_result_n391 = hxc_l_tmp_string_length_result_n1265 > 0;
      }
      if (hxc_l_tmp_short_circuit_result_n391)
      {
        int32_t hxc_l_x_h124422bda379 = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_l_width_h7644f82bd34f, (double)2)), 230);
        struct Color hxc_l_this1_h986025d66f8b = (struct Color){ .r = 255, .g = 205, .b = 91, .a = 255 };
        hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n1270 = HXC_BORROWED_CSTRING_INITIALIZER;
        if (hxc_string_borrow_cstring(&hxc_l_adventureTagline, &hxc_l_tmp_string_borrow_cstring_result_n1270) != HXC_STATUS_OK)
        {
          abort();
        }
        int32_t hxc_l_tmp_load_result_n1271 = hxc_l_x_h124422bda379;
        int32_t hxc_l_tmp_load_result_n1272 = hxc_l_firstTop;
        DrawText(hxc_l_tmp_string_borrow_cstring_result_n1270.data, (int32_t)hxc_l_tmp_load_result_n1271, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n1272, 36), (int32_t)17, hxc_l_this1_h986025d66f8b);
      }
      if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n390) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n387) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n381) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n373) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_adventureTagline) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n343) != HXC_STATUS_OK)
      {
        abort();
      }
    }
    else
    {
      if (hxc_l_onCampaignSelect)
      {
        hxc_l_gc_roots[220] = (const void *)hxc_l_campaign;
        struct hxc_caxecraft_content_CampaignManifest *hxc_l_selectedCampaign_hfefbd14f6a51 = hxc_l_campaign;
        hxc_l_gc_roots[221] = (const void *)hxc_l_selectedCampaign_hfefbd14f6a51;
        hxc_l_gc_roots[222] = (const void *)NULL;
        if (hxc_l_selectedCampaign_hfefbd14f6a51 == NULL)
        {
          enum hxc_caxecraft_app_AppScreen hxc_l_tmp_call_result_n1278 = hxc_caxecraft_app_AppScreen_closeCampaignSelection(hxc_l_screen);
          hxc_l_screen = hxc_l_tmp_call_result_n1278;
        }
        else
        {
          hxc_string hxc_l_locale1_h516b763e22f3 = { 0 };
          switch (hxc_l_locale) {
            case 0:
              {
                hxc_l_locale1_h516b763e22f3 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
                if (hxc_string_retain(hxc_l_locale1_h516b763e22f3) != HXC_STATUS_OK)
                {
                  abort();
                }
                break;
              }
            case 1:
              {
                hxc_l_locale1_h516b763e22f3 = (hxc_string){ (const uint8_t *)"es-mx", 5, true, NULL };
                if (hxc_string_retain(hxc_l_locale1_h516b763e22f3) != HXC_STATUS_OK)
                {
                  abort();
                }
                break;
              }
            default:
              {
                hxc_l_locale1_h516b763e22f3 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
                if (hxc_string_retain(hxc_l_locale1_h516b763e22f3) != HXC_STATUS_OK)
                {
                  abort();
                }
                break;
              }
          }
          hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n1280 = hxc_l_locale1_h516b763e22f3;
          hxc_l_tmp_managed_flow_owner_n396 = hxc_l_tmp_managed_flow_carrier_move_result_n1280;
          struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1281 = hxc_l_levelView;
          hxc_l_gc_roots[223] = (const void *)hxc_l_tmp_load_result_n1281;
          if (hxc_l_tmp_load_result_n1281 == NULL)
          {
            abort();
          }
          struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_class_field_load_result_n1282 = (*hxc_l_tmp_load_result_n1281).hxc_levelPresentation;
          hxc_l_gc_roots[224] = (const void *)hxc_l_tmp_class_field_load_result_n1282;
          struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_this = hxc_l_tmp_class_field_load_result_n1282;
          struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_load_result_n1283 = hxc_l_this;
          hxc_l_gc_roots[225] = (const void *)hxc_l_tmp_load_result_n1283;
          if (hxc_l_tmp_load_result_n1283 == NULL)
          {
            abort();
          }
          struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_load_result_n1284 = hxc_l_this;
          hxc_l_gc_roots[226] = (const void *)hxc_l_tmp_load_result_n1284;
          if (hxc_l_tmp_load_result_n1284 == NULL)
          {
            abort();
          }
          struct hxc_caxecraft_scenario_ScenarioText hxc_l_tmp_class_field_load_result_n1285 = (*hxc_l_tmp_load_result_n1284).hxc_title;
          hxc_string hxc_l_tmp_instance_call_result_n1287 = hxc_caxecraft_content_RuntimeLevelPresentation_resolve(hxc_l_tmp_load_result_n1283, hxc_l_tmp_class_field_load_result_n1285, hxc_l_tmp_managed_flow_owner_n396);
          hxc_l_tmp = hxc_l_tmp_instance_call_result_n1287;
          hxc_string hxc_l_locale1_hfa244cde12a1 = { 0 };
          switch (hxc_l_locale) {
            case 0:
              {
                hxc_l_locale1_hfa244cde12a1 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
                if (hxc_string_retain(hxc_l_locale1_hfa244cde12a1) != HXC_STATUS_OK)
                {
                  abort();
                }
                break;
              }
            case 1:
              {
                hxc_l_locale1_hfa244cde12a1 = (hxc_string){ (const uint8_t *)"es-mx", 5, true, NULL };
                if (hxc_string_retain(hxc_l_locale1_hfa244cde12a1) != HXC_STATUS_OK)
                {
                  abort();
                }
                break;
              }
            default:
              {
                hxc_l_locale1_hfa244cde12a1 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
                if (hxc_string_retain(hxc_l_locale1_hfa244cde12a1) != HXC_STATUS_OK)
                {
                  abort();
                }
                break;
              }
          }
          hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n1289 = hxc_l_locale1_hfa244cde12a1;
          hxc_l_tmp_managed_flow_owner_n400 = hxc_l_tmp_managed_flow_carrier_move_result_n1289;
          struct Texture hxc_l_tmp_load_result_n1290 = hxc_l_titleTexture;
          bool hxc_l_tmp_load_result_n1291 = hxc_l_titleTextureReady;
          struct Texture hxc_l_tmp_load_result_n1292 = hxc_l_wordmarkTexture;
          bool hxc_l_tmp_load_result_n1293 = hxc_l_wordmarkTextureReady;
          struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n1294 = hxc_l_selectedCampaign_hfefbd14f6a51;
          hxc_l_gc_roots[227] = (const void *)hxc_l_tmp_load_result_n1294;
          int32_t hxc_l_tmp_load_result_n1295 = hxc_l_locale;
          struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_tmp_load_result_n1296 = hxc_l_uiCatalog;
          hxc_l_gc_roots[228] = (const void *)hxc_l_tmp_load_result_n1296;
          int32_t hxc_l_tmp_load_result_n1297 = hxc_l_selectedCampaignLevelIndex;
          hxc_string hxc_l_tmp_load_result_n1298 = hxc_l_tmp;
          struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1299 = hxc_l_levelView;
          hxc_l_gc_roots[229] = (const void *)hxc_l_tmp_load_result_n1299;
          if (hxc_l_tmp_load_result_n1299 == NULL)
          {
            abort();
          }
          struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_class_field_load_result_n1300 = (*hxc_l_tmp_load_result_n1299).hxc_levelPresentation;
          hxc_l_gc_roots[230] = (const void *)hxc_l_tmp_class_field_load_result_n1300;
          if (hxc_l_tmp_class_field_load_result_n1300 == NULL)
          {
            abort();
          }
          hxc_string hxc_l_tmp_instance_call_result_n1302 = hxc_caxecraft_content_RuntimeLevelPresentation_message(hxc_l_tmp_class_field_load_result_n1300, (hxc_string){ (const uint8_t *)"adventure_tagline", 17, true, NULL }, hxc_l_tmp_managed_flow_owner_n400);
          hxc_l_tmp_static_call_argument_9_owner_n401 = hxc_l_tmp_instance_call_result_n1302;
          hxc_caxecraft_app_CampaignMenu_draw(hxc_l_tmp_load_result_n1290, hxc_l_tmp_load_result_n1291, hxc_l_tmp_load_result_n1292, hxc_l_tmp_load_result_n1293, hxc_l_tmp_load_result_n1294, hxc_l_tmp_load_result_n1295, hxc_l_tmp_load_result_n1296, hxc_l_tmp_load_result_n1297, hxc_l_tmp_load_result_n1298, hxc_l_tmp_static_call_argument_9_owner_n401);
          if (hxc_string_release(&hxc_l_tmp_static_call_argument_9_owner_n401) != HXC_STATUS_OK)
          {
            abort();
          }
          if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n400) != HXC_STATUS_OK)
          {
            abort();
          }
          if (hxc_string_release(&hxc_l_tmp) != HXC_STATUS_OK)
          {
            abort();
          }
          if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n396) != HXC_STATUS_OK)
          {
            abort();
          }
        }
      }
      else
      {
        if (hxc_l_onEditor)
        {
          struct hxc_caxecraft_app_CaxecraftEditorScreen *hxc_l_tmp_load_result_n1305 = hxc_l_editorScreen;
          hxc_l_gc_roots[231] = (const void *)hxc_l_tmp_load_result_n1305;
          if (hxc_l_tmp_load_result_n1305 == NULL)
          {
            abort();
          }
          int32_t hxc_l_tmp_load_result_n1306 = hxc_l_locale;
          enum hxc_caxecraft_app_EditorScreenAction hxc_l_tmp_instance_call_result_n1308 = hxc_caxecraft_app_CaxecraftEditorScreen_draw(hxc_l_tmp_load_result_n1305, hxc_l_tmp_load_result_n1306, hxc_l_editorNavigationCommand);
          if (hxc_l_tmp_instance_call_result_n1308 == hxc_caxecraft_app_EditorScreenAction_ReturnToTitle)
          {
            enum hxc_caxecraft_app_AppScreen hxc_l_tmp_call_result_n1311 = hxc_caxecraft_app_AppScreen_closeEditor(hxc_l_screen);
            hxc_l_screen = hxc_l_tmp_call_result_n1311;
          }
        }
        else
        {
          ClearBackground((struct Color){ .r = 126, .g = 190, .b = 201, .a = 255 });
          int32_t hxc_l_tmp_native_call_result_n1313 = GetScreenWidth();
          int32_t hxc_l_sunX = hxc_i32_subtract_wrapping((int32_t)hxc_l_tmp_native_call_result_n1313, 260);
          float hxc_l_radius_h85473ce39b9c = (float)42.0;
          struct Color hxc_l_this1_hfc3d72dd72b5 = (struct Color){ .r = 255, .g = 226, .b = 136, .a = 255 };
          int32_t hxc_l_tmp_load_result_n1315 = hxc_l_sunX;
          float hxc_l_tmp_load_result_n1316 = hxc_l_radius_h85473ce39b9c;
          DrawCircle((int32_t)hxc_l_tmp_load_result_n1315, (int32_t)86, hxc_l_tmp_load_result_n1316, hxc_l_this1_hfc3d72dd72b5);
          float hxc_l_radius_hb50bb79eaa9f = (float)30.0;
          struct Color hxc_l_this1_hec1771a09d88 = (struct Color){ .r = 255, .g = 246, .b = 205, .a = 255 };
          int32_t hxc_l_tmp_load_result_n1319 = hxc_l_sunX;
          float hxc_l_tmp_load_result_n1320 = hxc_l_radius_hb50bb79eaa9f;
          DrawCircle((int32_t)hxc_l_tmp_load_result_n1319, (int32_t)86, hxc_l_tmp_load_result_n1320, hxc_l_this1_hec1771a09d88);
          BeginMode3D(hxc_l_camera);
          if (hxc_l_self == NULL)
          {
            abort();
          }
          struct hxc_caxecraft_app_TerrainRenderer *hxc_l_tmp_owned_class_field_address_n1323 = &(*hxc_l_self).hxc_terrainRenderer;
          if (hxc_l_tmp_owned_class_field_address_n1323 == NULL)
          {
            abort();
          }
          struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1324 = hxc_l_session;
          hxc_l_gc_roots[232] = (const void *)hxc_l_tmp_load_result_n1324;
          if (hxc_l_tmp_load_result_n1324 == NULL)
          {
            abort();
          }
          size_t hxc_l_tmp_length_n1326;
          const uint8_t *hxc_l_tmp_instance_call_result_n1325 = hxc_caxecraft_domain_GameSession_worldView(hxc_l_tmp_load_result_n1324, &hxc_l_tmp_length_n1326);
          struct Texture hxc_l_tmp_load_result_n1327 = hxc_l_terrainTexture;
          bool hxc_l_tmp_load_result_n1328 = hxc_l_terrainTextureReady;
          struct Texture hxc_l_tmp_load_result_n1329 = hxc_l_adventureTerrainTexture;
          bool hxc_l_tmp_load_result_n1330 = hxc_l_adventureTerrainTextureReady;
          double hxc_l_tmp_record_field_load_result_n1331 = hxc_l_renderPosition.hxc_x;
          struct hxc_caxecraft_app_TerrainRenderCounters hxc_l_tmp_instance_call_result_n1333 = hxc_caxecraft_app_TerrainRenderer_draw(hxc_l_tmp_owned_class_field_address_n1323, hxc_l_tmp_instance_call_result_n1325, hxc_l_tmp_length_n1326, hxc_l_tmp_load_result_n1327, hxc_l_tmp_load_result_n1328, hxc_l_tmp_load_result_n1329, hxc_l_tmp_load_result_n1330, hxc_l_tmp_record_field_load_result_n1331, hxc_l_renderPosition.hxc_z);
          struct hxc_caxecraft_app_TerrainRenderCounters hxc_l_renderCounters = hxc_l_tmp_instance_call_result_n1333;
          struct hxc_caxecraft_app_WaterRenderer *hxc_l_tmp_owned_class_field_address_n1334 = &(*hxc_l_self).hxc_waterRenderer;
          if (hxc_l_tmp_owned_class_field_address_n1334 == NULL)
          {
            abort();
          }
          struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1335 = hxc_l_session;
          hxc_l_gc_roots[233] = (const void *)hxc_l_tmp_load_result_n1335;
          if (hxc_l_tmp_load_result_n1335 == NULL)
          {
            abort();
          }
          size_t hxc_l_tmp_length_n1337;
          const uint8_t *hxc_l_tmp_instance_call_result_n1336 = hxc_caxecraft_domain_GameSession_worldView(hxc_l_tmp_load_result_n1335, &hxc_l_tmp_length_n1337);
          struct Texture hxc_l_tmp_load_result_n1338 = hxc_l_terrainTexture;
          bool hxc_l_tmp_load_result_n1339 = hxc_l_terrainTextureReady;
          struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1340 = hxc_l_levelView;
          hxc_l_gc_roots[234] = (const void *)hxc_l_tmp_load_result_n1340;
          if (hxc_l_tmp_load_result_n1340 == NULL)
          {
            abort();
          }
          int32_t hxc_l_tmp_class_field_load_result_n1341 = (*hxc_l_tmp_load_result_n1340).hxc_waterCell;
          double hxc_l_tmp_load_result_n1342 = hxc_l_eyeX;
          double hxc_l_tmp_load_result_n1343 = hxc_l_eyeY;
          struct hxc_caxecraft_app_WaterRenderCounters hxc_l_tmp_instance_call_result_n1345 = hxc_caxecraft_app_WaterRenderer_draw(hxc_l_tmp_owned_class_field_address_n1334, hxc_l_tmp_instance_call_result_n1336, hxc_l_tmp_length_n1337, hxc_l_tmp_load_result_n1338, hxc_l_tmp_load_result_n1339, hxc_l_tmp_class_field_load_result_n1341, hxc_l_tmp_load_result_n1342, hxc_l_tmp_load_result_n1343, hxc_l_eyeZ);
          struct hxc_caxecraft_app_WaterRenderCounters hxc_l_waterCounters = hxc_l_tmp_instance_call_result_n1345;
          int32_t hxc_l_tmp_record_field_load_result_n1346 = hxc_l_renderCounters.hxc_visible;
          int32_t hxc_l_totalVisible = hxc_i32_add_wrapping(hxc_l_tmp_record_field_load_result_n1346, hxc_l_waterCounters.hxc_visible);
          int32_t hxc_l_tmp_record_field_load_result_n1348 = hxc_l_renderCounters.hxc_drawCalls;
          int32_t hxc_l_totalDrawCalls = hxc_i32_add_wrapping(hxc_l_tmp_record_field_load_result_n1348, hxc_l_waterCounters.hxc_drawCalls);
          struct Camera3D hxc_l_tmp_load_result_n1350 = hxc_l_camera;
          struct Texture hxc_l_tmp_load_result_n1351 = hxc_l_entityTexture;
          bool hxc_l_tmp_load_result_n1352 = hxc_l_entityTextureReady;
          struct hxc_caxecraft_domain_Character hxc_l_tmp_load_result_n1353 = hxc_l_dialogueActor;
          struct hxc_caxecraft_domain_Character hxc_l_tmp_load_result_n1354 = hxc_l_enemyActor;
          enum hxc_caxecraft_domain_ActorControllerPhase hxc_l_tmp_record_field_load_result_n1355 = hxc_l_enemyPhase_h70382f3bdc66.hxc_phase;
          hxc_caxecraft_app_CaxecraftApp_drawActors(hxc_l_tmp_load_result_n1350, hxc_l_tmp_load_result_n1351, hxc_l_tmp_load_result_n1352, hxc_l_tmp_load_result_n1353, hxc_l_tmp_load_result_n1354, hxc_l_tmp_record_field_load_result_n1355, hxc_l_berryDrop);
          struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_tmp_load_result_n1357 = hxc_l_contentRegistry;
          hxc_l_gc_roots[235] = (const void *)hxc_l_tmp_load_result_n1357;
          struct Camera3D hxc_l_tmp_load_result_n1358 = hxc_l_camera;
          struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1359 = hxc_l_session;
          hxc_l_gc_roots[236] = (const void *)hxc_l_tmp_load_result_n1359;
          if (hxc_l_tmp_load_result_n1359 == NULL)
          {
            abort();
          }
          size_t hxc_l_tmp_length_n1361;
          const int32_t *hxc_l_tmp_instance_call_result_n1360 = hxc_caxecraft_domain_GameSession_authoredItemsView(hxc_l_tmp_load_result_n1359, &hxc_l_tmp_length_n1361);
          struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1362 = hxc_l_levelView;
          hxc_l_gc_roots[237] = (const void *)hxc_l_tmp_load_result_n1362;
          struct Texture hxc_l_tmp_load_result_n1363 = hxc_l_itemTexture;
          bool hxc_l_tmp_load_result_n1364 = hxc_l_itemTextureReady;
          struct Texture hxc_l_tmp_load_result_n1365 = hxc_l_adventureItemTexture;
          hxc_caxecraft_app_AuthoredItemRenderer_drawWorldItems(hxc_l_tmp_load_result_n1357, hxc_l_tmp_load_result_n1358, hxc_l_tmp_instance_call_result_n1360, hxc_l_tmp_length_n1361, hxc_l_tmp_load_result_n1362, hxc_l_tmp_load_result_n1363, hxc_l_tmp_load_result_n1364, hxc_l_tmp_load_result_n1365, hxc_l_adventureItemTextureReady);
          if (hxc_l_hit.hxc_hit)
          {
            double hxc_l_y = (double)hxc_l_hit.hxc_cellY + 0.5;
            double hxc_l_z = (double)hxc_l_hit.hxc_cellZ + 0.5;
            int32_t hxc_l_tmp_record_field_load_result_n1370 = hxc_l_hit.hxc_cellX;
            double hxc_l_tmp_load_result_n1371 = hxc_l_y;
            DrawCubeWires((struct Vector3){ .x = (float)((double)hxc_l_tmp_record_field_load_result_n1370 + 0.5), .y = (float)hxc_l_tmp_load_result_n1371, .z = (float)hxc_l_z }, (float)1.04, (float)1.04, (float)1.04, (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 });
          }
          EndMode3D();
          if (hxc_l_cameraWaterBlend > 0.0)
          {
            int32_t hxc_l_overlayAlpha = hxc_f64_to_i32_saturating(105.0 * hxc_l_cameraWaterBlend);
            int32_t hxc_l_tmp_native_call_result_n1377 = GetScreenWidth();
            int32_t hxc_l_width_h3bb9f35d7dd8 = (int32_t)hxc_l_tmp_native_call_result_n1377;
            int32_t hxc_l_tmp_native_call_result_n1378 = GetScreenHeight();
            int32_t hxc_l_height_h95429cedc191 = (int32_t)hxc_l_tmp_native_call_result_n1378;
            uint8_t hxc_l_red = (uint8_t)31;
            uint8_t hxc_l_green = (uint8_t)115;
            uint8_t hxc_l_blue = (uint8_t)154;
            int32_t hxc_l_tmp_load_result_n1379 = hxc_l_overlayAlpha;
            int32_t hxc_l_tmp_conditional_result_n420 = 0;
            if (hxc_l_tmp_load_result_n1379 < 0)
            {
              hxc_l_tmp_conditional_result_n420 = 0;
            }
            else
            {
              int32_t hxc_l_tmp_load_result_n1380 = hxc_l_overlayAlpha;
              int32_t hxc_l_tmp_conditional_result_n421 = 0;
              if (hxc_l_tmp_load_result_n1380 > 255)
              {
                hxc_l_tmp_conditional_result_n421 = 255;
              }
              else
              {
                hxc_l_tmp_conditional_result_n421 = hxc_l_overlayAlpha;
              }
              hxc_l_tmp_conditional_result_n420 = hxc_l_tmp_conditional_result_n421;
            }
            uint8_t hxc_l_alpha = (uint8_t)hxc_l_tmp_conditional_result_n420;
            uint8_t hxc_l_tmp_load_result_n1384 = hxc_l_red;
            uint8_t hxc_l_tmp_load_result_n1385 = hxc_l_green;
            uint8_t hxc_l_tmp_load_result_n1386 = hxc_l_blue;
            struct Color hxc_l_this1_h37d4e35a0f2d = (struct Color){ .r = hxc_l_tmp_load_result_n1384, .g = hxc_l_tmp_load_result_n1385, .b = hxc_l_tmp_load_result_n1386, .a = hxc_l_alpha };
            int32_t hxc_l_tmp_load_result_n1389 = hxc_l_width_h3bb9f35d7dd8;
            int32_t hxc_l_tmp_load_result_n1390 = hxc_l_height_h95429cedc191;
            DrawRectangle((int32_t)0, (int32_t)0, (int32_t)hxc_l_tmp_load_result_n1389, (int32_t)hxc_l_tmp_load_result_n1390, hxc_l_this1_h37d4e35a0f2d);
          }
          double hxc_l_tmp_record_field_load_result_n1392 = hxc_l_character.hxc_body.hxc_x;
          double hxc_l_tmp_record_field_load_result_n1393 = hxc_l_character.hxc_body.hxc_z;
          struct hxc_caxecraft_domain_VitalsState hxc_l_tmp_record_field_load_result_n1394 = hxc_l_character.hxc_vitals;
          int32_t hxc_l_tmp_load_result_n1395 = hxc_l_aquaticEquipmentCode;
          int32_t hxc_l_tmp_load_result_n1396 = hxc_l_aquaticEquipmentFrames;
          bool hxc_l_tmp_record_field_load_result_n1397 = hxc_l_character.hxc_aquatic.hxc_headSubmerged;
          int32_t hxc_l_tmp_record_field_load_result_n1398 = hxc_l_character.hxc_aquatic.hxc_breathTicks;
          struct hxc_caxecraft_app_HudCharacterView hxc_l_hudView_h463ec0a60db3 = (struct hxc_caxecraft_app_HudCharacterView){ .hxc_aquaticEquipmentCode = hxc_l_tmp_load_result_n1395, .hxc_aquaticEquipmentVisible = hxc_l_tmp_load_result_n1396 > 0, .hxc_breathTicks = hxc_l_tmp_record_field_load_result_n1398, .hxc_headSubmerged = hxc_l_tmp_record_field_load_result_n1397, .hxc_maximumBreathTicks = hxc_l_character.hxc_aquaticProfile.hxc_maximumBreathTicks, .hxc_vitals = hxc_l_tmp_record_field_load_result_n1394, .hxc_x = hxc_l_tmp_record_field_load_result_n1392, .hxc_z = hxc_l_tmp_record_field_load_result_n1393 };
          bool hxc_l_hudView1 = hxc_l_placementBlockedFrames > 0;
          bool hxc_l_hudView2 = hxc_l_strikeHitFrames > 0;
          bool hxc_l_hudView3 = hxc_l_enemyDefeatedFrames > 0;
          bool hxc_l_hudView4 = hxc_l_enemyAttackFrames > 0;
          bool hxc_l_hudView5 = hxc_l_pickupFrames > 0;
          bool hxc_l_hudView6 = hxc_l_recoveryFeedbackFrames > 0;
          enum hxc_caxecraft_domain_ActorControllerPhase hxc_l_enemyPhase_h1aefe16e6eea = hxc_l_enemyPhase_h70382f3bdc66.hxc_phase;
          hxc_string hxc_l_locale1_h2b2ff433a2ad = { 0 };
          switch (hxc_l_locale) {
            case 0:
              {
                hxc_l_locale1_h2b2ff433a2ad = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
                if (hxc_string_retain(hxc_l_locale1_h2b2ff433a2ad) != HXC_STATUS_OK)
                {
                  abort();
                }
                break;
              }
            case 1:
              {
                hxc_l_locale1_h2b2ff433a2ad = (hxc_string){ (const uint8_t *)"es-mx", 5, true, NULL };
                if (hxc_string_retain(hxc_l_locale1_h2b2ff433a2ad) != HXC_STATUS_OK)
                {
                  abort();
                }
                break;
              }
            default:
              {
                hxc_l_locale1_h2b2ff433a2ad = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
                if (hxc_string_retain(hxc_l_locale1_h2b2ff433a2ad) != HXC_STATUS_OK)
                {
                  abort();
                }
                break;
              }
          }
          hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n1409 = hxc_l_locale1_h2b2ff433a2ad;
          hxc_l_tmp_managed_flow_owner_n432 = hxc_l_tmp_managed_flow_carrier_move_result_n1409;
          struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1410 = hxc_l_levelView;
          hxc_l_gc_roots[238] = (const void *)hxc_l_tmp_load_result_n1410;
          if (hxc_l_tmp_load_result_n1410 == NULL)
          {
            abort();
          }
          struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_class_field_load_result_n1411 = (*hxc_l_tmp_load_result_n1410).hxc_levelPresentation;
          hxc_l_gc_roots[239] = (const void *)hxc_l_tmp_class_field_load_result_n1411;
          if (hxc_l_tmp_class_field_load_result_n1411 == NULL)
          {
            abort();
          }
          hxc_string hxc_l_tmp_load_result_n1412 = hxc_l_currentObjectiveId;
          hxc_string hxc_l_tmp_instance_call_result_n1414 = hxc_caxecraft_content_RuntimeLevelPresentation_objectiveTitle(hxc_l_tmp_class_field_load_result_n1411, hxc_l_tmp_load_result_n1412, hxc_l_tmp_managed_flow_owner_n432);
          hxc_l_hudView7 = hxc_l_tmp_instance_call_result_n1414;
          int32_t hxc_l_tmp_load_result_n1415 = hxc_l_totalVisible;
          int32_t hxc_l_tmp_load_result_n1416 = hxc_l_totalDrawCalls;
          int32_t hxc_l_tmp_load_result_n1417 = hxc_l_frameCount;
          int32_t hxc_l_tmp_load_result_n1418 = hxc_l_completedTicks;
          struct hxc_caxecraft_app_HudCharacterView hxc_l_tmp_load_result_n1420 = hxc_l_hudView_h463ec0a60db3;
          bool hxc_l_tmp_load_result_n1421 = hxc_l_hudView1;
          bool hxc_l_tmp_load_result_n1422 = hxc_l_hudView2;
          bool hxc_l_tmp_load_result_n1423 = hxc_l_hudView3;
          bool hxc_l_tmp_load_result_n1424 = hxc_l_hudView4;
          bool hxc_l_tmp_load_result_n1425 = hxc_l_hudView5;
          int32_t hxc_l_tmp_load_result_n1426 = hxc_l_pickupAmount;
          int32_t hxc_l_tmp_load_result_n1427 = hxc_l_inventoryFullReason;
          int32_t hxc_l_tmp_load_result_n1428 = hxc_l_recoveryFeedback;
          bool hxc_l_tmp_load_result_n1429 = hxc_l_hudView6;
          bool hxc_l_tmp_load_result_n1431 = hxc_l_paused_h0af3ff7e4c3b;
          bool hxc_l_tmp_load_result_n1432 = hxc_l_captured_h8abb2355e37f;
          struct hxc_caxecraft_domain_RaycastHit hxc_l_tmp_load_result_n1433 = hxc_l_hit;
          int32_t hxc_l_tmp_load_result_n1434 = hxc_l_selectedMode;
          int32_t hxc_l_tmp_load_result_n1435 = hxc_l_locale;
          struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_load_result_n1436 = hxc_l_inventory;
          enum hxc_caxecraft_gameplay_GuidePhase hxc_l_tmp_load_result_n1437 = hxc_l_guidePhase;
          bool hxc_l_tmp_load_result_n1438 = hxc_l_guideInteractionAvailable;
          struct hxc_caxecraft_domain_Character hxc_l_tmp_load_result_n1439 = hxc_l_enemyActor;
          enum hxc_caxecraft_domain_ActorControllerPhase hxc_l_tmp_load_result_n1440 = hxc_l_enemyPhase_h1aefe16e6eea;
          hxc_string hxc_l_tmp_record_field_levelLabel_owner_n435 = hxc_l_levelLabel;
          if (hxc_string_retain(hxc_l_tmp_record_field_levelLabel_owner_n435) != HXC_STATUS_OK)
          {
            abort();
          }
          hxc_string hxc_l_tmp_record_field_levelLabel_owned_load_result_n1442 = hxc_l_tmp_record_field_levelLabel_owner_n435;
          hxc_string hxc_l_tmp_record_field_objectiveTitle_owner_n436 = hxc_l_hudView7;
          if (hxc_string_retain(hxc_l_tmp_record_field_objectiveTitle_owner_n436) != HXC_STATUS_OK)
          {
            abort();
          }
          hxc_string hxc_l_tmp_record_field_objectiveTitle_owned_load_result_n1444 = hxc_l_tmp_record_field_objectiveTitle_owner_n436;
          struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1445 = hxc_l_levelView;
          hxc_l_gc_roots[240] = (const void *)hxc_l_tmp_load_result_n1445;
          if (hxc_l_tmp_load_result_n1445 == NULL)
          {
            abort();
          }
          struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_class_field_load_result_n1446 = (*hxc_l_tmp_load_result_n1445).hxc_levelPresentation;
          hxc_l_gc_roots[241] = (const void *)hxc_l_tmp_class_field_load_result_n1446;
          hxc_l_gc_roots[242] = (const void *)(struct hxc_caxecraft_app_HudView){ .hxc_character = hxc_l_tmp_load_result_n1420, .hxc_enemy = hxc_l_tmp_load_result_n1439, .hxc_enemyPhase = hxc_l_tmp_load_result_n1440, .hxc_feedback = (struct hxc_caxecraft_app_HudFeedbackView){ .hxc_enemyAttacked = hxc_l_tmp_load_result_n1424, .hxc_enemyDefeated = hxc_l_tmp_load_result_n1423, .hxc_inventoryFullReason = hxc_l_tmp_load_result_n1427, .hxc_pickedUp = hxc_l_tmp_load_result_n1425, .hxc_pickupAmount = hxc_l_tmp_load_result_n1426, .hxc_placementBlocked = hxc_l_tmp_load_result_n1421, .hxc_recoveryDecision = hxc_l_tmp_load_result_n1428, .hxc_recoveryVisible = hxc_l_tmp_load_result_n1429, .hxc_strikeHit = hxc_l_tmp_load_result_n1422 }, .hxc_guideInteractionAvailable = hxc_l_tmp_load_result_n1438, .hxc_guidePhase = hxc_l_tmp_load_result_n1437, .hxc_hit = hxc_l_tmp_load_result_n1433, .hxc_inventory = hxc_l_tmp_load_result_n1436, .hxc_levelLabel = hxc_l_tmp_record_field_levelLabel_owned_load_result_n1442, .hxc_locale = hxc_l_tmp_load_result_n1435, .hxc_metrics = (struct hxc_caxecraft_app_HudMetricsView){ .hxc_completedTicks = hxc_l_tmp_load_result_n1418, .hxc_drawCalls = hxc_l_tmp_load_result_n1416, .hxc_renderedFrames = hxc_l_tmp_load_result_n1417, .hxc_visibleBlocks = hxc_l_tmp_load_result_n1415 }, .hxc_mode = hxc_l_tmp_load_result_n1434, .hxc_objectiveTitle = hxc_l_tmp_record_field_objectiveTitle_owned_load_result_n1444, .hxc_paused = hxc_l_tmp_load_result_n1431, .hxc_pointerCaptured = hxc_l_tmp_load_result_n1432, .hxc_presentation = hxc_l_tmp_class_field_load_result_n1446 }.hxc_presentation;
          hxc_l_hudView_h0a58428167f7 = (struct hxc_caxecraft_app_HudView){ .hxc_character = hxc_l_tmp_load_result_n1420, .hxc_enemy = hxc_l_tmp_load_result_n1439, .hxc_enemyPhase = hxc_l_tmp_load_result_n1440, .hxc_feedback = (struct hxc_caxecraft_app_HudFeedbackView){ .hxc_enemyAttacked = hxc_l_tmp_load_result_n1424, .hxc_enemyDefeated = hxc_l_tmp_load_result_n1423, .hxc_inventoryFullReason = hxc_l_tmp_load_result_n1427, .hxc_pickedUp = hxc_l_tmp_load_result_n1425, .hxc_pickupAmount = hxc_l_tmp_load_result_n1426, .hxc_placementBlocked = hxc_l_tmp_load_result_n1421, .hxc_recoveryDecision = hxc_l_tmp_load_result_n1428, .hxc_recoveryVisible = hxc_l_tmp_load_result_n1429, .hxc_strikeHit = hxc_l_tmp_load_result_n1422 }, .hxc_guideInteractionAvailable = hxc_l_tmp_load_result_n1438, .hxc_guidePhase = hxc_l_tmp_load_result_n1437, .hxc_hit = hxc_l_tmp_load_result_n1433, .hxc_inventory = hxc_l_tmp_load_result_n1436, .hxc_levelLabel = hxc_l_tmp_record_field_levelLabel_owned_load_result_n1442, .hxc_locale = hxc_l_tmp_load_result_n1435, .hxc_metrics = (struct hxc_caxecraft_app_HudMetricsView){ .hxc_completedTicks = hxc_l_tmp_load_result_n1418, .hxc_drawCalls = hxc_l_tmp_load_result_n1416, .hxc_renderedFrames = hxc_l_tmp_load_result_n1417, .hxc_visibleBlocks = hxc_l_tmp_load_result_n1415 }, .hxc_mode = hxc_l_tmp_load_result_n1434, .hxc_objectiveTitle = hxc_l_tmp_record_field_objectiveTitle_owned_load_result_n1444, .hxc_paused = hxc_l_tmp_load_result_n1431, .hxc_pointerCaptured = hxc_l_tmp_load_result_n1432, .hxc_presentation = hxc_l_tmp_class_field_load_result_n1446 };
          struct hxc_caxecraft_app_HudView hxc_l_tmp_load_result_n1448 = hxc_l_hudView_h0a58428167f7;
          hxc_l_gc_roots[243] = (const void *)hxc_l_tmp_load_result_n1448.hxc_presentation;
          struct hxc_caxecraft_app_HudResources hxc_l_tmp_load_result_n1449 = hxc_l_hudResources;
          struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_tmp_load_result_n1450 = hxc_l_contentRegistry;
          hxc_l_gc_roots[244] = (const void *)hxc_l_tmp_load_result_n1450;
          hxc_l_gc_roots[245] = (const void *)hxc_l_uiCatalog;
          hxc_caxecraft_app_CaxecraftApp_drawHud(hxc_l_tmp_load_result_n1448, hxc_l_tmp_load_result_n1449, hxc_l_tmp_load_result_n1450, hxc_l_uiCatalog);
          hxc_record_d16efdcf_destroy(&hxc_l_hudView_h0a58428167f7);
          if (hxc_string_release(&hxc_l_hudView7) != HXC_STATUS_OK)
          {
            abort();
          }
          if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n432) != HXC_STATUS_OK)
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
