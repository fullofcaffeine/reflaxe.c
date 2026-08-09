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

void hxc_caxecraft_app_CaxecraftApp_drawActors(struct Camera3D hxc_l_camera, struct Texture hxc_l_entityTexture, bool hxc_l_entityTextureReady, struct hxc_caxecraft_domain_Character hxc_l_guide, struct hxc_caxecraft_domain_Character hxc_l_enemy, int32_t hxc_l_guideCell, int32_t hxc_l_enemyCell, enum hxc_caxecraft_domain_ActorControllerPhase hxc_l_enemyPhase, struct hxc_caxecraft_gameplay_BerryDropState hxc_l_berryDrop)
{
  if (hxc_l_entityTextureReady)
  {
    double hxc_l_y_h7c3a6bf70922 = hxc_l_guide.hxc_body.hxc_y + 0.76;
    double hxc_l_z_hf15d5492c5c8 = hxc_l_guide.hxc_body.hxc_z;
    double hxc_l_tmp_load_result_n6 = hxc_l_y_h7c3a6bf70922;
    hxc_caxecraft_app_CaxecraftAtlas_drawWorldSprite(hxc_l_camera, hxc_l_entityTexture, hxc_l_guideCell, (struct Vector3){ .x = (float)hxc_l_guide.hxc_body.hxc_x, .y = (float)hxc_l_tmp_load_result_n6, .z = (float)hxc_l_z_hf15d5492c5c8 }, 0.95, 1.52);
  }
  else
  {
    double hxc_l_y_hf37adda717ce = hxc_l_guide.hxc_body.hxc_y + 0.54;
    double hxc_l_z_h5eef9d726f23 = hxc_l_guide.hxc_body.hxc_z;
    double hxc_l_tmp_load_result_n15 = hxc_l_y_hf37adda717ce;
    DrawCube((struct Vector3){ .x = (float)hxc_l_guide.hxc_body.hxc_x, .y = (float)hxc_l_tmp_load_result_n15, .z = (float)hxc_l_z_h5eef9d726f23 }, (float)0.50, (float)0.86, (float)0.42, (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 });
    double hxc_l_y_hb215c904406a = hxc_l_guide.hxc_body.hxc_y + 1.18;
    double hxc_l_z_h5daaf4a0a707 = hxc_l_guide.hxc_body.hxc_z;
    double hxc_l_tmp_load_result_n25 = hxc_l_y_hb215c904406a;
    DrawCube((struct Vector3){ .x = (float)hxc_l_guide.hxc_body.hxc_x, .y = (float)hxc_l_tmp_load_result_n25, .z = (float)hxc_l_z_h5daaf4a0a707 }, (float)0.44, (float)0.44, (float)0.44, (struct Color){ .r = 228, .g = 239, .b = 227, .a = 255 });
    double hxc_l_y_h31135260f3e7 = hxc_l_guide.hxc_body.hxc_y + 1.41;
    double hxc_l_z_hbc6308452a41 = hxc_l_guide.hxc_body.hxc_z;
    double hxc_l_tmp_load_result_n35 = hxc_l_y_h31135260f3e7;
    DrawCube((struct Vector3){ .x = (float)hxc_l_guide.hxc_body.hxc_x, .y = (float)hxc_l_tmp_load_result_n35, .z = (float)hxc_l_z_hbc6308452a41 }, (float)0.48, (float)0.16, (float)0.48, (struct Color){ .r = 15, .g = 27, .b = 39, .a = 220 });
  }
  if (hxc_l_enemy.hxc_vitals.hxc_health > 0)
  {
    if (hxc_l_entityTextureReady)
    {
      double hxc_l_y_h55872bc7aaa9 = hxc_l_enemy.hxc_body.hxc_y + 0.48;
      double hxc_l_z_h5be35bc3380d = hxc_l_enemy.hxc_body.hxc_z;
      double hxc_l_tmp_load_result_n47 = hxc_l_y_h55872bc7aaa9;
      hxc_caxecraft_app_CaxecraftAtlas_drawWorldSprite(hxc_l_camera, hxc_l_entityTexture, hxc_l_enemyCell, (struct Vector3){ .x = (float)hxc_l_enemy.hxc_body.hxc_x, .y = (float)hxc_l_tmp_load_result_n47, .z = (float)hxc_l_z_h5be35bc3380d }, 1.05, 0.96);
    }
    else
    {
      double hxc_l_y_h424aad34c9cc = hxc_l_enemy.hxc_body.hxc_y + 0.30;
      double hxc_l_z_hc6ae99db8711 = hxc_l_enemy.hxc_body.hxc_z;
      double hxc_l_tmp_load_result_n56 = hxc_l_y_h424aad34c9cc;
      DrawCube((struct Vector3){ .x = (float)hxc_l_enemy.hxc_body.hxc_x, .y = (float)hxc_l_tmp_load_result_n56, .z = (float)hxc_l_z_hc6ae99db8711 }, (float)0.70, (float)0.54, (float)0.70, (struct Color){ .r = 218, .g = 65, .b = 72, .a = 255 });
      double hxc_l_y_h69e908438cd3 = hxc_l_enemy.hxc_body.hxc_y + 0.66;
      double hxc_l_z_hccf658b9363b = hxc_l_enemy.hxc_body.hxc_z;
      double hxc_l_tmp_load_result_n66 = hxc_l_y_h69e908438cd3;
      DrawCube((struct Vector3){ .x = (float)hxc_l_enemy.hxc_body.hxc_x, .y = (float)hxc_l_tmp_load_result_n66, .z = (float)hxc_l_z_hccf658b9363b }, (float)0.50, (float)0.34, (float)0.50, (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 });
    }
    if (hxc_l_enemyPhase == hxc_caxecraft_domain_ActorControllerPhase_Windup)
    {
      double hxc_l_y_hb331d99e2b3e = hxc_l_enemy.hxc_body.hxc_y + 1.02;
      double hxc_l_z_hafaabe9a85d7 = hxc_l_enemy.hxc_body.hxc_z;
      double hxc_l_tmp_load_result_n77 = hxc_l_y_hb331d99e2b3e;
      DrawCube((struct Vector3){ .x = (float)hxc_l_enemy.hxc_body.hxc_x, .y = (float)hxc_l_tmp_load_result_n77, .z = (float)hxc_l_z_hafaabe9a85d7 }, (float)0.20, (float)0.20, (float)0.20, (struct Color){ .r = 218, .g = 65, .b = 72, .a = 255 });
    }
  }
  if (hxc_l_berryDrop.hxc_active)
  {
    double hxc_l_y_h17e31c62449d = hxc_l_berryDrop.hxc_y;
    double hxc_l_z_h85195eda2ab8 = hxc_l_berryDrop.hxc_z;
    double hxc_l_tmp_load_result_n85 = hxc_l_y_h17e31c62449d;
    DrawCube((struct Vector3){ .x = (float)(hxc_l_berryDrop.hxc_x - 0.12), .y = (float)hxc_l_tmp_load_result_n85, .z = (float)hxc_l_z_h85195eda2ab8 }, (float)0.18, (float)0.18, (float)0.18, (struct Color){ .r = 174, .g = 78, .b = 136, .a = 255 });
    double hxc_l_y_h7ab22088d987 = hxc_l_berryDrop.hxc_y;
    double hxc_l_z_haaaf458b4aa6 = hxc_l_berryDrop.hxc_z;
    double hxc_l_tmp_load_result_n92 = hxc_l_y_h7ab22088d987;
    DrawCube((struct Vector3){ .x = (float)(hxc_l_berryDrop.hxc_x + 0.12), .y = (float)hxc_l_tmp_load_result_n92, .z = (float)hxc_l_z_haaaf458b4aa6 }, (float)0.18, (float)0.18, (float)0.18, (struct Color){ .r = 174, .g = 78, .b = 136, .a = 255 });
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
  const void *hxc_l_gc_roots[18] = { (const void *)hxc_l_view.hxc_presentation, (const void *)hxc_l_contentRegistry, (const void *)hxc_l_uiCatalog, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 18, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_string hxc_l_text_h71dcae10f79c = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n103 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n104 = { 0 };
  hxc_string hxc_l_text_heef65c2f3bc8 = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n108 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n109 = { 0 };
  hxc_string hxc_l_text_h54fca1407031 = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n113 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n114 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n116 = { 0 };
  hxc_string hxc_l_text_h9bc58c634805 = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n120 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n121 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n123 = { 0 };
  hxc_string hxc_l_text1_hee60b287d3c8 = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n129 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n130 = { 0 };
  hxc_string hxc_l_text_had2b2354bc20 = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n134 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n135 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n138 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n139 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n141 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n142 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n143 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n55 = { 0 };
  hxc_string hxc_l_text_h3e3088d75cfd = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n61 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n62 = { 0 };
  hxc_string hxc_l_text1_hf41e52c4584c = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n66 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n67 = { 0 };
  hxc_string hxc_l_text_h1cdcf0fd5f35 = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n71 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n72 = { 0 };
  hxc_string hxc_l_text_hb5afaf84a8af = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n76 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n77 = { 0 };
  hxc_string hxc_l_text_ha6ee01c27e79 = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n81 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n82 = { 0 };
  hxc_string hxc_l_text_h157957de5dc8 = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n86 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n87 = { 0 };
  hxc_string hxc_l_text_h8342667d3053 = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n91 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n92 = { 0 };
  hxc_string hxc_l_text_h0e8a8f114258 = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n98 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n99 = { 0 };
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
  hxc_string hxc_l_activeDialogue = hxc_l_view.hxc_activeDialogue;
  if (hxc_string_retain(hxc_l_activeDialogue) != HXC_STATUS_OK)
  {
    abort();
  }
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
  struct Color hxc_l_text_hcf65a5a292d3 = (struct Color){ .r = 228, .g = 239, .b = 227, .a = 255 };
  int32_t hxc_l_tmp_load_result_n60 = hxc_l_centerX;
  int32_t hxc_l_tmp_load_result_n61 = hxc_l_centerY;
  int32_t hxc_l_tmp_load_result_n62 = hxc_l_centerX;
  int32_t hxc_l_tmp_load_result_n63 = hxc_l_centerY;
  DrawLine((int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n60, 8), (int32_t)hxc_l_tmp_load_result_n61, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n62, 3), (int32_t)hxc_l_tmp_load_result_n63, hxc_l_text_hcf65a5a292d3);
  int32_t hxc_l_tmp_load_result_n65 = hxc_l_centerX;
  int32_t hxc_l_tmp_load_result_n66 = hxc_l_centerY;
  int32_t hxc_l_tmp_load_result_n67 = hxc_l_centerX;
  int32_t hxc_l_tmp_load_result_n68 = hxc_l_centerY;
  DrawLine((int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n65, 3), (int32_t)hxc_l_tmp_load_result_n66, (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n67, 8), (int32_t)hxc_l_tmp_load_result_n68, hxc_l_text_hcf65a5a292d3);
  int32_t hxc_l_tmp_load_result_n70 = hxc_l_centerX;
  int32_t hxc_l_tmp_load_result_n71 = hxc_l_centerY;
  int32_t hxc_l_tmp_load_result_n72 = hxc_l_centerX;
  int32_t hxc_l_tmp_load_result_n73 = hxc_l_centerY;
  DrawLine((int32_t)hxc_l_tmp_load_result_n70, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n71, 8), (int32_t)hxc_l_tmp_load_result_n72, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n73, 3), hxc_l_text_hcf65a5a292d3);
  int32_t hxc_l_tmp_load_result_n75 = hxc_l_centerX;
  int32_t hxc_l_tmp_load_result_n76 = hxc_l_centerY;
  int32_t hxc_l_tmp_load_result_n77 = hxc_l_centerX;
  int32_t hxc_l_tmp_load_result_n78 = hxc_l_centerY;
  DrawLine((int32_t)hxc_l_tmp_load_result_n75, (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n76, 3), (int32_t)hxc_l_tmp_load_result_n77, (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n78, 8), hxc_l_text_hcf65a5a292d3);
  struct Color hxc_l_this1_h0e9b9dd4d36d = (struct Color){ .r = 15, .g = 27, .b = 39, .a = 220 };
  DrawRectangle((int32_t)18, (int32_t)18, (int32_t)460, (int32_t)108, hxc_l_this1_h0e9b9dd4d36d);
  struct Color hxc_l_this1_h8b147b82d3a1 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
  DrawRectangleLines((int32_t)18, (int32_t)18, (int32_t)460, (int32_t)108, hxc_l_this1_h8b147b82d3a1);
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
  DrawText(hxc_l_tmp_string_borrow_cstring_result_n87.data, (int32_t)32, (int32_t)28, (int32_t)20, hxc_l_text_hcf65a5a292d3);
  hxc_string hxc_l_text1_h359588d2548b = hxc_l_view.hxc_levelLabel;
  if (hxc_string_retain(hxc_l_text1_h359588d2548b) != HXC_STATUS_OK)
  {
    abort();
  }
  struct Color hxc_l_this1_h6e2aeb4dcbd7 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
  hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n92 = HXC_BORROWED_CSTRING_INITIALIZER;
  if (hxc_string_borrow_cstring(&hxc_l_text1_h359588d2548b, &hxc_l_tmp_string_borrow_cstring_result_n92) != HXC_STATUS_OK)
  {
    abort();
  }
  DrawText(hxc_l_tmp_string_borrow_cstring_result_n92.data, (int32_t)250, (int32_t)30, (int32_t)16, hxc_l_this1_h6e2aeb4dcbd7);
  hxc_string hxc_l_tmp_instance_call_result_n95 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_uiCatalog, hxc_l_locale, 4);
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n50 = hxc_l_tmp_instance_call_result_n95;
  hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n97 = HXC_BORROWED_CSTRING_INITIALIZER;
  if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n50, &hxc_l_tmp_string_borrow_cstring_result_n97) != HXC_STATUS_OK)
  {
    abort();
  }
  DrawText(hxc_l_tmp_string_borrow_cstring_result_n97.data, (int32_t)32, (int32_t)58, (int32_t)14, hxc_l_text_hcf65a5a292d3);
  hxc_caxecraft_app_HudDigits_drawNumber(16384, 82, 59, 5, (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 });
  hxc_string hxc_l_tmp_instance_call_result_n101 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_uiCatalog, hxc_l_locale, 8);
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n51 = hxc_l_tmp_instance_call_result_n101;
  hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n103 = HXC_BORROWED_CSTRING_INITIALIZER;
  if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n51, &hxc_l_tmp_string_borrow_cstring_result_n103) != HXC_STATUS_OK)
  {
    abort();
  }
  DrawText(hxc_l_tmp_string_borrow_cstring_result_n103.data, (int32_t)160, (int32_t)58, (int32_t)14, hxc_l_text_hcf65a5a292d3);
  hxc_caxecraft_app_HudDigits_drawNumber(hxc_l_visible, 230, 59, 5, (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 });
  hxc_string hxc_l_tmp_instance_call_result_n108 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_uiCatalog, hxc_l_locale, 5);
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n52 = hxc_l_tmp_instance_call_result_n108;
  hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n110 = HXC_BORROWED_CSTRING_INITIALIZER;
  if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n52, &hxc_l_tmp_string_borrow_cstring_result_n110) != HXC_STATUS_OK)
  {
    abort();
  }
  DrawText(hxc_l_tmp_string_borrow_cstring_result_n110.data, (int32_t)326, (int32_t)58, (int32_t)14, hxc_l_text_hcf65a5a292d3);
  hxc_caxecraft_app_HudDigits_drawNumber(hxc_l_drawCalls, 382, 59, 5, (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 });
  hxc_string hxc_l_tmp_instance_call_result_n115 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_uiCatalog, hxc_l_locale, 6);
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n53 = hxc_l_tmp_instance_call_result_n115;
  hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n117 = HXC_BORROWED_CSTRING_INITIALIZER;
  if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n53, &hxc_l_tmp_string_borrow_cstring_result_n117) != HXC_STATUS_OK)
  {
    abort();
  }
  DrawText(hxc_l_tmp_string_borrow_cstring_result_n117.data, (int32_t)32, (int32_t)86, (int32_t)12, hxc_l_text_hcf65a5a292d3);
  int32_t hxc_l_tmp_load_result_n119 = hxc_l_frames;
  hxc_caxecraft_app_HudDigits_drawNumber(hxc_l_tmp_load_result_n119, 82, 85, 6, hxc_l_text_hcf65a5a292d3);
  hxc_string hxc_l_tmp_instance_call_result_n122 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_uiCatalog, hxc_l_locale, 7);
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n54 = hxc_l_tmp_instance_call_result_n122;
  hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n124 = HXC_BORROWED_CSTRING_INITIALIZER;
  if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n54, &hxc_l_tmp_string_borrow_cstring_result_n124) != HXC_STATUS_OK)
  {
    abort();
  }
  DrawText(hxc_l_tmp_string_borrow_cstring_result_n124.data, (int32_t)174, (int32_t)86, (int32_t)12, hxc_l_text_hcf65a5a292d3);
  int32_t hxc_l_tmp_load_result_n126 = hxc_l_updates;
  hxc_caxecraft_app_HudDigits_drawNumber(hxc_l_tmp_load_result_n126, 216, 85, 6, hxc_l_text_hcf65a5a292d3);
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
  DrawText(hxc_l_tmp_string_borrow_cstring_result_n154.data, (int32_t)20, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n155, 22), (int32_t)14, hxc_l_text_hcf65a5a292d3);
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
    hxc_l_text_h3e3088d75cfd = hxc_l_view.hxc_objectiveTitle;
    if (hxc_string_retain(hxc_l_text_h3e3088d75cfd) != HXC_STATUS_OK)
    {
      abort();
    }
    struct Color hxc_l_this1_h14d1d2ed9e18 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
    hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n164 = HXC_BORROWED_CSTRING_INITIALIZER;
    if (hxc_string_borrow_cstring(&hxc_l_text_h3e3088d75cfd, &hxc_l_tmp_string_borrow_cstring_result_n164) != HXC_STATUS_OK)
    {
      abort();
    }
    DrawText(hxc_l_tmp_string_borrow_cstring_result_n164.data, (int32_t)32, (int32_t)110, (int32_t)14, hxc_l_this1_h14d1d2ed9e18);
    if (hxc_string_release(&hxc_l_text_h3e3088d75cfd) != HXC_STATUS_OK)
    {
      abort();
    }
  }
  if (hxc_l_activeDialogue.data != NULL)
  {
    struct Color hxc_l_this1_h5d72db904be3 = (struct Color){ .r = 15, .g = 27, .b = 39, .a = 220 };
    int32_t hxc_l_tmp_load_result_n168 = hxc_l_centerX;
    int32_t hxc_l_tmp_load_result_n169 = hxc_l_centerY;
    DrawRectangle((int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n168, 260), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n169, 54), (int32_t)520, (int32_t)60, hxc_l_this1_h5d72db904be3);
    hxc_string hxc_l_text1_h79f208a78359 = { 0 };
    switch (hxc_l_locale) {
      case 0:
        {
          hxc_l_text1_h79f208a78359 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
          if (hxc_string_retain(hxc_l_text1_h79f208a78359) != HXC_STATUS_OK)
          {
            abort();
          }
          break;
        }
      case 1:
        {
          hxc_l_text1_h79f208a78359 = (hxc_string){ (const uint8_t *)"es-mx", 5, true, NULL };
          if (hxc_string_retain(hxc_l_text1_h79f208a78359) != HXC_STATUS_OK)
          {
            abort();
          }
          break;
        }
      default:
        {
          hxc_l_text1_h79f208a78359 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
          if (hxc_string_retain(hxc_l_text1_h79f208a78359) != HXC_STATUS_OK)
          {
            abort();
          }
          break;
        }
    }
    hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n172 = hxc_l_text1_h79f208a78359;
    hxc_l_tmp_managed_flow_owner_n61 = hxc_l_tmp_managed_flow_carrier_move_result_n172;
    struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_load_result_n173 = hxc_l_presentation;
    hxc_l_gc_roots[4] = (const void *)hxc_l_tmp_load_result_n173;
    if (hxc_l_tmp_load_result_n173 == NULL)
    {
      abort();
    }
    hxc_string hxc_l_tmp_load_result_n174 = hxc_l_activeDialogue;
    hxc_string hxc_l_tmp_instance_call_result_n176 = hxc_caxecraft_content_RuntimeLevelPresentation_dialogueLine(hxc_l_tmp_load_result_n173, hxc_l_tmp_load_result_n174, 0, hxc_l_tmp_managed_flow_owner_n61);
    hxc_l_tmp_cstring_ref_owner_owner_n62 = hxc_l_tmp_instance_call_result_n176;
    hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n178 = HXC_BORROWED_CSTRING_INITIALIZER;
    if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n62, &hxc_l_tmp_string_borrow_cstring_result_n178) != HXC_STATUS_OK)
    {
      abort();
    }
    int32_t hxc_l_tmp_load_result_n179 = hxc_l_centerX;
    int32_t hxc_l_tmp_load_result_n180 = hxc_l_centerY;
    DrawText(hxc_l_tmp_string_borrow_cstring_result_n178.data, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n179, 225), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n180, 74), (int32_t)16, hxc_l_text_hcf65a5a292d3);
    if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n62) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n61) != HXC_STATUS_OK)
    {
      abort();
    }
  }
  else
  {
    if (hxc_l_guideInteractionAvailable)
    {
      struct Color hxc_l_this1_h715563326262 = (struct Color){ .r = 15, .g = 27, .b = 39, .a = 220 };
      int32_t hxc_l_tmp_load_result_n184 = hxc_l_centerX;
      int32_t hxc_l_tmp_load_result_n185 = hxc_l_centerY;
      DrawRectangle((int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n184, 260), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n185, 54), (int32_t)520, (int32_t)60, hxc_l_this1_h715563326262);
      hxc_string hxc_l_tmp_call_result_n187 = hxc_caxecraft_app_GameplayMessage_gameplayMessageId(11);
      hxc_l_text1_hf41e52c4584c = hxc_l_tmp_call_result_n187;
      hxc_string hxc_l_text2_h1b6e8548f277 = { 0 };
      switch (hxc_l_locale) {
        case 0:
          {
            hxc_l_text2_h1b6e8548f277 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
            if (hxc_string_retain(hxc_l_text2_h1b6e8548f277) != HXC_STATUS_OK)
            {
              abort();
            }
            break;
          }
        case 1:
          {
            hxc_l_text2_h1b6e8548f277 = (hxc_string){ (const uint8_t *)"es-mx", 5, true, NULL };
            if (hxc_string_retain(hxc_l_text2_h1b6e8548f277) != HXC_STATUS_OK)
            {
              abort();
            }
            break;
          }
        default:
          {
            hxc_l_text2_h1b6e8548f277 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
            if (hxc_string_retain(hxc_l_text2_h1b6e8548f277) != HXC_STATUS_OK)
            {
              abort();
            }
            break;
          }
      }
      hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n189 = hxc_l_text2_h1b6e8548f277;
      hxc_l_tmp_managed_flow_owner_n66 = hxc_l_tmp_managed_flow_carrier_move_result_n189;
      struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_load_result_n190 = hxc_l_presentation;
      hxc_l_gc_roots[5] = (const void *)hxc_l_tmp_load_result_n190;
      if (hxc_l_tmp_load_result_n190 == NULL)
      {
        abort();
      }
      hxc_string hxc_l_tmp_load_result_n191 = hxc_l_text1_hf41e52c4584c;
      hxc_string hxc_l_tmp_instance_call_result_n193 = hxc_caxecraft_content_RuntimeLevelPresentation_message(hxc_l_tmp_load_result_n190, hxc_l_tmp_load_result_n191, hxc_l_tmp_managed_flow_owner_n66);
      hxc_l_tmp_cstring_ref_owner_owner_n67 = hxc_l_tmp_instance_call_result_n193;
      hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n195 = HXC_BORROWED_CSTRING_INITIALIZER;
      if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n67, &hxc_l_tmp_string_borrow_cstring_result_n195) != HXC_STATUS_OK)
      {
        abort();
      }
      int32_t hxc_l_tmp_load_result_n196 = hxc_l_centerX;
      int32_t hxc_l_tmp_load_result_n197 = hxc_l_centerY;
      DrawText(hxc_l_tmp_string_borrow_cstring_result_n195.data, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n196, 110), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n197, 74), (int32_t)18, hxc_l_text_hcf65a5a292d3);
      if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n67) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n66) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_text1_hf41e52c4584c) != HXC_STATUS_OK)
      {
        abort();
      }
    }
  }
  if (hxc_l_enemy.hxc_vitals.hxc_health > 0)
  {
    if (hxc_l_enemyPhase == hxc_caxecraft_domain_ActorControllerPhase_Windup)
    {
      struct Color hxc_l_color_hf3d34467f46d = (struct Color){ .r = 218, .g = 65, .b = 72, .a = 255 };
      hxc_string hxc_l_tmp_call_result_n203 = hxc_caxecraft_app_GameplayMessage_gameplayMessageId(9);
      hxc_l_text_h1cdcf0fd5f35 = hxc_l_tmp_call_result_n203;
      hxc_string hxc_l_text1_h42e5fb141880 = { 0 };
      switch (hxc_l_locale) {
        case 0:
          {
            hxc_l_text1_h42e5fb141880 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
            if (hxc_string_retain(hxc_l_text1_h42e5fb141880) != HXC_STATUS_OK)
            {
              abort();
            }
            break;
          }
        case 1:
          {
            hxc_l_text1_h42e5fb141880 = (hxc_string){ (const uint8_t *)"es-mx", 5, true, NULL };
            if (hxc_string_retain(hxc_l_text1_h42e5fb141880) != HXC_STATUS_OK)
            {
              abort();
            }
            break;
          }
        default:
          {
            hxc_l_text1_h42e5fb141880 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
            if (hxc_string_retain(hxc_l_text1_h42e5fb141880) != HXC_STATUS_OK)
            {
              abort();
            }
            break;
          }
      }
      hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n205 = hxc_l_text1_h42e5fb141880;
      hxc_l_tmp_managed_flow_owner_n71 = hxc_l_tmp_managed_flow_carrier_move_result_n205;
      struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_load_result_n206 = hxc_l_presentation;
      hxc_l_gc_roots[6] = (const void *)hxc_l_tmp_load_result_n206;
      if (hxc_l_tmp_load_result_n206 == NULL)
      {
        abort();
      }
      hxc_string hxc_l_tmp_load_result_n207 = hxc_l_text_h1cdcf0fd5f35;
      hxc_string hxc_l_tmp_instance_call_result_n209 = hxc_caxecraft_content_RuntimeLevelPresentation_message(hxc_l_tmp_load_result_n206, hxc_l_tmp_load_result_n207, hxc_l_tmp_managed_flow_owner_n71);
      hxc_l_tmp_cstring_ref_owner_owner_n72 = hxc_l_tmp_instance_call_result_n209;
      hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n211 = HXC_BORROWED_CSTRING_INITIALIZER;
      if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n72, &hxc_l_tmp_string_borrow_cstring_result_n211) != HXC_STATUS_OK)
      {
        abort();
      }
      int32_t hxc_l_tmp_load_result_n212 = hxc_l_width;
      DrawText(hxc_l_tmp_string_borrow_cstring_result_n211.data, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n212, 300), (int32_t)28, (int32_t)16, hxc_l_color_hf3d34467f46d);
      if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n72) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n71) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_text_h1cdcf0fd5f35) != HXC_STATUS_OK)
      {
        abort();
      }
    }
    else
    {
      if (hxc_l_enemyPhase == hxc_caxecraft_domain_ActorControllerPhase_Chasing)
      {
        struct Color hxc_l_color_h7a3967db03e6 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
        hxc_string hxc_l_tmp_call_result_n217 = hxc_caxecraft_app_GameplayMessage_gameplayMessageId(7);
        hxc_l_text_hb5afaf84a8af = hxc_l_tmp_call_result_n217;
        hxc_string hxc_l_text1_hc043ed646845 = { 0 };
        switch (hxc_l_locale) {
          case 0:
            {
              hxc_l_text1_hc043ed646845 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
              if (hxc_string_retain(hxc_l_text1_hc043ed646845) != HXC_STATUS_OK)
              {
                abort();
              }
              break;
            }
          case 1:
            {
              hxc_l_text1_hc043ed646845 = (hxc_string){ (const uint8_t *)"es-mx", 5, true, NULL };
              if (hxc_string_retain(hxc_l_text1_hc043ed646845) != HXC_STATUS_OK)
              {
                abort();
              }
              break;
            }
          default:
            {
              hxc_l_text1_hc043ed646845 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
              if (hxc_string_retain(hxc_l_text1_hc043ed646845) != HXC_STATUS_OK)
              {
                abort();
              }
              break;
            }
        }
        hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n219 = hxc_l_text1_hc043ed646845;
        hxc_l_tmp_managed_flow_owner_n76 = hxc_l_tmp_managed_flow_carrier_move_result_n219;
        struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_load_result_n220 = hxc_l_presentation;
        hxc_l_gc_roots[7] = (const void *)hxc_l_tmp_load_result_n220;
        if (hxc_l_tmp_load_result_n220 == NULL)
        {
          abort();
        }
        hxc_string hxc_l_tmp_load_result_n221 = hxc_l_text_hb5afaf84a8af;
        hxc_string hxc_l_tmp_instance_call_result_n223 = hxc_caxecraft_content_RuntimeLevelPresentation_message(hxc_l_tmp_load_result_n220, hxc_l_tmp_load_result_n221, hxc_l_tmp_managed_flow_owner_n76);
        hxc_l_tmp_cstring_ref_owner_owner_n77 = hxc_l_tmp_instance_call_result_n223;
        hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n225 = HXC_BORROWED_CSTRING_INITIALIZER;
        if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n77, &hxc_l_tmp_string_borrow_cstring_result_n225) != HXC_STATUS_OK)
        {
          abort();
        }
        int32_t hxc_l_tmp_load_result_n226 = hxc_l_width;
        DrawText(hxc_l_tmp_string_borrow_cstring_result_n225.data, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n226, 180), (int32_t)28, (int32_t)16, hxc_l_color_h7a3967db03e6);
        if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n77) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n76) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_string_release(&hxc_l_text_hb5afaf84a8af) != HXC_STATUS_OK)
        {
          abort();
        }
      }
    }
  }
  if (hxc_l_strikeHit)
  {
    struct Color hxc_l_color_h4d65d25df0b6 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
    hxc_string hxc_l_tmp_call_result_n230 = hxc_caxecraft_app_GameplayMessage_gameplayMessageId(5);
    hxc_l_text_ha6ee01c27e79 = hxc_l_tmp_call_result_n230;
    hxc_string hxc_l_text1_h15f98a326409 = { 0 };
    switch (hxc_l_locale) {
      case 0:
        {
          hxc_l_text1_h15f98a326409 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
          if (hxc_string_retain(hxc_l_text1_h15f98a326409) != HXC_STATUS_OK)
          {
            abort();
          }
          break;
        }
      case 1:
        {
          hxc_l_text1_h15f98a326409 = (hxc_string){ (const uint8_t *)"es-mx", 5, true, NULL };
          if (hxc_string_retain(hxc_l_text1_h15f98a326409) != HXC_STATUS_OK)
          {
            abort();
          }
          break;
        }
      default:
        {
          hxc_l_text1_h15f98a326409 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
          if (hxc_string_retain(hxc_l_text1_h15f98a326409) != HXC_STATUS_OK)
          {
            abort();
          }
          break;
        }
    }
    hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n232 = hxc_l_text1_h15f98a326409;
    hxc_l_tmp_managed_flow_owner_n81 = hxc_l_tmp_managed_flow_carrier_move_result_n232;
    struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_load_result_n233 = hxc_l_presentation;
    hxc_l_gc_roots[8] = (const void *)hxc_l_tmp_load_result_n233;
    if (hxc_l_tmp_load_result_n233 == NULL)
    {
      abort();
    }
    hxc_string hxc_l_tmp_load_result_n234 = hxc_l_text_ha6ee01c27e79;
    hxc_string hxc_l_tmp_instance_call_result_n236 = hxc_caxecraft_content_RuntimeLevelPresentation_message(hxc_l_tmp_load_result_n233, hxc_l_tmp_load_result_n234, hxc_l_tmp_managed_flow_owner_n81);
    hxc_l_tmp_cstring_ref_owner_owner_n82 = hxc_l_tmp_instance_call_result_n236;
    hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n238 = HXC_BORROWED_CSTRING_INITIALIZER;
    if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n82, &hxc_l_tmp_string_borrow_cstring_result_n238) != HXC_STATUS_OK)
    {
      abort();
    }
    int32_t hxc_l_tmp_load_result_n239 = hxc_l_centerX;
    int32_t hxc_l_tmp_load_result_n240 = hxc_l_centerY;
    DrawText(hxc_l_tmp_string_borrow_cstring_result_n238.data, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n239, 70), (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n240, 54), (int32_t)18, hxc_l_color_h4d65d25df0b6);
    if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n82) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n81) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_text_ha6ee01c27e79) != HXC_STATUS_OK)
    {
      abort();
    }
  }
  if (hxc_l_enemyDefeated)
  {
    struct Color hxc_l_color_hac182f69fe2c = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
    hxc_string hxc_l_tmp_call_result_n244 = hxc_caxecraft_app_GameplayMessage_gameplayMessageId(8);
    hxc_l_text_h157957de5dc8 = hxc_l_tmp_call_result_n244;
    hxc_string hxc_l_text1_h1b770c1dba5e = { 0 };
    switch (hxc_l_locale) {
      case 0:
        {
          hxc_l_text1_h1b770c1dba5e = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
          if (hxc_string_retain(hxc_l_text1_h1b770c1dba5e) != HXC_STATUS_OK)
          {
            abort();
          }
          break;
        }
      case 1:
        {
          hxc_l_text1_h1b770c1dba5e = (hxc_string){ (const uint8_t *)"es-mx", 5, true, NULL };
          if (hxc_string_retain(hxc_l_text1_h1b770c1dba5e) != HXC_STATUS_OK)
          {
            abort();
          }
          break;
        }
      default:
        {
          hxc_l_text1_h1b770c1dba5e = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
          if (hxc_string_retain(hxc_l_text1_h1b770c1dba5e) != HXC_STATUS_OK)
          {
            abort();
          }
          break;
        }
    }
    hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n246 = hxc_l_text1_h1b770c1dba5e;
    hxc_l_tmp_managed_flow_owner_n86 = hxc_l_tmp_managed_flow_carrier_move_result_n246;
    struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_load_result_n247 = hxc_l_presentation;
    hxc_l_gc_roots[9] = (const void *)hxc_l_tmp_load_result_n247;
    if (hxc_l_tmp_load_result_n247 == NULL)
    {
      abort();
    }
    hxc_string hxc_l_tmp_load_result_n248 = hxc_l_text_h157957de5dc8;
    hxc_string hxc_l_tmp_instance_call_result_n250 = hxc_caxecraft_content_RuntimeLevelPresentation_message(hxc_l_tmp_load_result_n247, hxc_l_tmp_load_result_n248, hxc_l_tmp_managed_flow_owner_n86);
    hxc_l_tmp_cstring_ref_owner_owner_n87 = hxc_l_tmp_instance_call_result_n250;
    hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n252 = HXC_BORROWED_CSTRING_INITIALIZER;
    if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n87, &hxc_l_tmp_string_borrow_cstring_result_n252) != HXC_STATUS_OK)
    {
      abort();
    }
    int32_t hxc_l_tmp_load_result_n253 = hxc_l_width;
    DrawText(hxc_l_tmp_string_borrow_cstring_result_n252.data, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n253, 285), (int32_t)54, (int32_t)16, hxc_l_color_hac182f69fe2c);
    if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n87) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n86) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_text_h157957de5dc8) != HXC_STATUS_OK)
    {
      abort();
    }
  }
  if (hxc_l_enemyAttacked)
  {
    struct Color hxc_l_color_h5b56c9d17824 = (struct Color){ .r = 218, .g = 65, .b = 72, .a = 255 };
    hxc_string hxc_l_tmp_call_result_n257 = hxc_caxecraft_app_GameplayMessage_gameplayMessageId(15);
    hxc_l_text_h8342667d3053 = hxc_l_tmp_call_result_n257;
    hxc_string hxc_l_text1_hac82f94a2374 = { 0 };
    switch (hxc_l_locale) {
      case 0:
        {
          hxc_l_text1_hac82f94a2374 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
          if (hxc_string_retain(hxc_l_text1_hac82f94a2374) != HXC_STATUS_OK)
          {
            abort();
          }
          break;
        }
      case 1:
        {
          hxc_l_text1_hac82f94a2374 = (hxc_string){ (const uint8_t *)"es-mx", 5, true, NULL };
          if (hxc_string_retain(hxc_l_text1_hac82f94a2374) != HXC_STATUS_OK)
          {
            abort();
          }
          break;
        }
      default:
        {
          hxc_l_text1_hac82f94a2374 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
          if (hxc_string_retain(hxc_l_text1_hac82f94a2374) != HXC_STATUS_OK)
          {
            abort();
          }
          break;
        }
    }
    hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n259 = hxc_l_text1_hac82f94a2374;
    hxc_l_tmp_managed_flow_owner_n91 = hxc_l_tmp_managed_flow_carrier_move_result_n259;
    struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_load_result_n260 = hxc_l_presentation;
    hxc_l_gc_roots[10] = (const void *)hxc_l_tmp_load_result_n260;
    if (hxc_l_tmp_load_result_n260 == NULL)
    {
      abort();
    }
    hxc_string hxc_l_tmp_load_result_n261 = hxc_l_text_h8342667d3053;
    hxc_string hxc_l_tmp_instance_call_result_n263 = hxc_caxecraft_content_RuntimeLevelPresentation_message(hxc_l_tmp_load_result_n260, hxc_l_tmp_load_result_n261, hxc_l_tmp_managed_flow_owner_n91);
    hxc_l_tmp_cstring_ref_owner_owner_n92 = hxc_l_tmp_instance_call_result_n263;
    hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n265 = HXC_BORROWED_CSTRING_INITIALIZER;
    if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n92, &hxc_l_tmp_string_borrow_cstring_result_n265) != HXC_STATUS_OK)
    {
      abort();
    }
    int32_t hxc_l_tmp_load_result_n266 = hxc_l_width;
    DrawText(hxc_l_tmp_string_borrow_cstring_result_n265.data, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n266, 330), (int32_t)82, (int32_t)16, hxc_l_color_h5b56c9d17824);
    if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n92) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n91) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_text_h8342667d3053) != HXC_STATUS_OK)
    {
      abort();
    }
  }
  if (hxc_l_pickedUp)
  {
    int32_t hxc_l_tmp_load_result_n269 = hxc_l_pickupAmount;
    int32_t hxc_l_tmp_conditional_result_n94 = 0;
    if (hxc_l_tmp_load_result_n269 == 1)
    {
      hxc_l_tmp_conditional_result_n94 = 0;
    }
    else
    {
      hxc_l_tmp_conditional_result_n94 = 1;
    }
    int32_t hxc_l_pickupMessage = hxc_l_tmp_conditional_result_n94;
    struct Color hxc_l_color_hd348895f77ee = (struct Color){ .r = 174, .g = 78, .b = 136, .a = 255 };
    hxc_string hxc_l_tmp_call_result_n273 = hxc_caxecraft_app_GameplayMessage_gameplayMessageId(hxc_l_pickupMessage);
    hxc_l_text_h0e8a8f114258 = hxc_l_tmp_call_result_n273;
    hxc_string hxc_l_text1_h1056d5591f39 = { 0 };
    switch (hxc_l_locale) {
      case 0:
        {
          hxc_l_text1_h1056d5591f39 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
          if (hxc_string_retain(hxc_l_text1_h1056d5591f39) != HXC_STATUS_OK)
          {
            abort();
          }
          break;
        }
      case 1:
        {
          hxc_l_text1_h1056d5591f39 = (hxc_string){ (const uint8_t *)"es-mx", 5, true, NULL };
          if (hxc_string_retain(hxc_l_text1_h1056d5591f39) != HXC_STATUS_OK)
          {
            abort();
          }
          break;
        }
      default:
        {
          hxc_l_text1_h1056d5591f39 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
          if (hxc_string_retain(hxc_l_text1_h1056d5591f39) != HXC_STATUS_OK)
          {
            abort();
          }
          break;
        }
    }
    hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n275 = hxc_l_text1_h1056d5591f39;
    hxc_l_tmp_managed_flow_owner_n98 = hxc_l_tmp_managed_flow_carrier_move_result_n275;
    struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_load_result_n276 = hxc_l_presentation;
    hxc_l_gc_roots[11] = (const void *)hxc_l_tmp_load_result_n276;
    if (hxc_l_tmp_load_result_n276 == NULL)
    {
      abort();
    }
    hxc_string hxc_l_tmp_load_result_n277 = hxc_l_text_h0e8a8f114258;
    hxc_string hxc_l_tmp_instance_call_result_n279 = hxc_caxecraft_content_RuntimeLevelPresentation_message(hxc_l_tmp_load_result_n276, hxc_l_tmp_load_result_n277, hxc_l_tmp_managed_flow_owner_n98);
    hxc_l_tmp_cstring_ref_owner_owner_n99 = hxc_l_tmp_instance_call_result_n279;
    hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n281 = HXC_BORROWED_CSTRING_INITIALIZER;
    if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n99, &hxc_l_tmp_string_borrow_cstring_result_n281) != HXC_STATUS_OK)
    {
      abort();
    }
    int32_t hxc_l_tmp_load_result_n282 = hxc_l_centerX;
    int32_t hxc_l_tmp_load_result_n283 = hxc_l_centerY;
    DrawText(hxc_l_tmp_string_borrow_cstring_result_n281.data, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n282, 48), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n283, 24), (int32_t)18, hxc_l_color_hd348895f77ee);
    if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n99) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n98) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_text_h0e8a8f114258) != HXC_STATUS_OK)
    {
      abort();
    }
  }
  if (hxc_l_inventoryFullReason == 1)
  {
    struct Color hxc_l_color_h1c6b6e0b6625 = (struct Color){ .r = 238, .g = 113, .b = 78, .a = 255 };
    hxc_string hxc_l_tmp_call_result_n287 = hxc_caxecraft_app_GameplayMessage_gameplayMessageId(3);
    hxc_l_text_h71dcae10f79c = hxc_l_tmp_call_result_n287;
    hxc_string hxc_l_text1_h5dd9f087468e = { 0 };
    switch (hxc_l_locale) {
      case 0:
        {
          hxc_l_text1_h5dd9f087468e = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
          if (hxc_string_retain(hxc_l_text1_h5dd9f087468e) != HXC_STATUS_OK)
          {
            abort();
          }
          break;
        }
      case 1:
        {
          hxc_l_text1_h5dd9f087468e = (hxc_string){ (const uint8_t *)"es-mx", 5, true, NULL };
          if (hxc_string_retain(hxc_l_text1_h5dd9f087468e) != HXC_STATUS_OK)
          {
            abort();
          }
          break;
        }
      default:
        {
          hxc_l_text1_h5dd9f087468e = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
          if (hxc_string_retain(hxc_l_text1_h5dd9f087468e) != HXC_STATUS_OK)
          {
            abort();
          }
          break;
        }
    }
    hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n289 = hxc_l_text1_h5dd9f087468e;
    hxc_l_tmp_managed_flow_owner_n103 = hxc_l_tmp_managed_flow_carrier_move_result_n289;
    struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_load_result_n290 = hxc_l_presentation;
    hxc_l_gc_roots[12] = (const void *)hxc_l_tmp_load_result_n290;
    if (hxc_l_tmp_load_result_n290 == NULL)
    {
      abort();
    }
    hxc_string hxc_l_tmp_load_result_n291 = hxc_l_text_h71dcae10f79c;
    hxc_string hxc_l_tmp_instance_call_result_n293 = hxc_caxecraft_content_RuntimeLevelPresentation_message(hxc_l_tmp_load_result_n290, hxc_l_tmp_load_result_n291, hxc_l_tmp_managed_flow_owner_n103);
    hxc_l_tmp_cstring_ref_owner_owner_n104 = hxc_l_tmp_instance_call_result_n293;
    hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n295 = HXC_BORROWED_CSTRING_INITIALIZER;
    if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n104, &hxc_l_tmp_string_borrow_cstring_result_n295) != HXC_STATUS_OK)
    {
      abort();
    }
    int32_t hxc_l_tmp_load_result_n296 = hxc_l_centerX;
    int32_t hxc_l_tmp_load_result_n297 = hxc_l_centerY;
    DrawText(hxc_l_tmp_string_borrow_cstring_result_n295.data, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n296, 150), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n297, 48), (int32_t)16, hxc_l_color_h1c6b6e0b6625);
    if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n104) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n103) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_text_h71dcae10f79c) != HXC_STATUS_OK)
    {
      abort();
    }
  }
  else
  {
    if (hxc_l_inventoryFullReason == 2)
    {
      struct Color hxc_l_color_hae4972dfc1e8 = (struct Color){ .r = 238, .g = 113, .b = 78, .a = 255 };
      hxc_string hxc_l_tmp_call_result_n301 = hxc_caxecraft_app_GameplayMessage_gameplayMessageId(4);
      hxc_l_text_heef65c2f3bc8 = hxc_l_tmp_call_result_n301;
      hxc_string hxc_l_text1_hd657bda35eff = { 0 };
      switch (hxc_l_locale) {
        case 0:
          {
            hxc_l_text1_hd657bda35eff = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
            if (hxc_string_retain(hxc_l_text1_hd657bda35eff) != HXC_STATUS_OK)
            {
              abort();
            }
            break;
          }
        case 1:
          {
            hxc_l_text1_hd657bda35eff = (hxc_string){ (const uint8_t *)"es-mx", 5, true, NULL };
            if (hxc_string_retain(hxc_l_text1_hd657bda35eff) != HXC_STATUS_OK)
            {
              abort();
            }
            break;
          }
        default:
          {
            hxc_l_text1_hd657bda35eff = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
            if (hxc_string_retain(hxc_l_text1_hd657bda35eff) != HXC_STATUS_OK)
            {
              abort();
            }
            break;
          }
      }
      hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n303 = hxc_l_text1_hd657bda35eff;
      hxc_l_tmp_managed_flow_owner_n108 = hxc_l_tmp_managed_flow_carrier_move_result_n303;
      struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_load_result_n304 = hxc_l_presentation;
      hxc_l_gc_roots[13] = (const void *)hxc_l_tmp_load_result_n304;
      if (hxc_l_tmp_load_result_n304 == NULL)
      {
        abort();
      }
      hxc_string hxc_l_tmp_load_result_n305 = hxc_l_text_heef65c2f3bc8;
      hxc_string hxc_l_tmp_instance_call_result_n307 = hxc_caxecraft_content_RuntimeLevelPresentation_message(hxc_l_tmp_load_result_n304, hxc_l_tmp_load_result_n305, hxc_l_tmp_managed_flow_owner_n108);
      hxc_l_tmp_cstring_ref_owner_owner_n109 = hxc_l_tmp_instance_call_result_n307;
      hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n309 = HXC_BORROWED_CSTRING_INITIALIZER;
      if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n109, &hxc_l_tmp_string_borrow_cstring_result_n309) != HXC_STATUS_OK)
      {
        abort();
      }
      int32_t hxc_l_tmp_load_result_n310 = hxc_l_centerX;
      int32_t hxc_l_tmp_load_result_n311 = hxc_l_centerY;
      DrawText(hxc_l_tmp_string_borrow_cstring_result_n309.data, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n310, 155), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n311, 48), (int32_t)16, hxc_l_color_hae4972dfc1e8);
      if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n109) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n108) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_text_heef65c2f3bc8) != HXC_STATUS_OK)
      {
        abort();
      }
    }
  }
  if (hxc_l_recoveryVisible)
  {
    if (hxc_l_recoveryFeedback == 1)
    {
      struct Color hxc_l_color_h985cbe2688f7 = (struct Color){ .r = 94, .g = 212, .b = 136, .a = 255 };
      hxc_string hxc_l_tmp_call_result_n316 = hxc_caxecraft_app_GameplayMessage_gameplayMessageId(2);
      hxc_l_text_h54fca1407031 = hxc_l_tmp_call_result_n316;
      hxc_string hxc_l_text1_hd69a8f7a7e26 = { 0 };
      switch (hxc_l_locale) {
        case 0:
          {
            hxc_l_text1_hd69a8f7a7e26 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
            if (hxc_string_retain(hxc_l_text1_hd69a8f7a7e26) != HXC_STATUS_OK)
            {
              abort();
            }
            break;
          }
        case 1:
          {
            hxc_l_text1_hd69a8f7a7e26 = (hxc_string){ (const uint8_t *)"es-mx", 5, true, NULL };
            if (hxc_string_retain(hxc_l_text1_hd69a8f7a7e26) != HXC_STATUS_OK)
            {
              abort();
            }
            break;
          }
        default:
          {
            hxc_l_text1_hd69a8f7a7e26 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
            if (hxc_string_retain(hxc_l_text1_hd69a8f7a7e26) != HXC_STATUS_OK)
            {
              abort();
            }
            break;
          }
      }
      hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n318 = hxc_l_text1_hd69a8f7a7e26;
      hxc_l_tmp_managed_flow_owner_n113 = hxc_l_tmp_managed_flow_carrier_move_result_n318;
      struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_load_result_n319 = hxc_l_presentation;
      hxc_l_gc_roots[14] = (const void *)hxc_l_tmp_load_result_n319;
      if (hxc_l_tmp_load_result_n319 == NULL)
      {
        abort();
      }
      hxc_string hxc_l_tmp_load_result_n320 = hxc_l_text_h54fca1407031;
      hxc_string hxc_l_tmp_instance_call_result_n322 = hxc_caxecraft_content_RuntimeLevelPresentation_message(hxc_l_tmp_load_result_n319, hxc_l_tmp_load_result_n320, hxc_l_tmp_managed_flow_owner_n113);
      hxc_l_tmp_cstring_ref_owner_owner_n114 = hxc_l_tmp_instance_call_result_n322;
      hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n324 = HXC_BORROWED_CSTRING_INITIALIZER;
      if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n114, &hxc_l_tmp_string_borrow_cstring_result_n324) != HXC_STATUS_OK)
      {
        abort();
      }
      int32_t hxc_l_tmp_load_result_n325 = hxc_l_centerX;
      int32_t hxc_l_tmp_load_result_n326 = hxc_l_centerY;
      DrawText(hxc_l_tmp_string_borrow_cstring_result_n324.data, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n325, 88), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n326, 24), (int32_t)18, hxc_l_color_h985cbe2688f7);
      if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n114) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n113) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_text_h54fca1407031) != HXC_STATUS_OK)
      {
        abort();
      }
    }
    else
    {
      if (hxc_l_recoveryFeedback == 2)
      {
        struct Color hxc_l_this1_hd0fc4b552ecf = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
        hxc_string hxc_l_tmp_instance_call_result_n331 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_uiCatalog, hxc_l_locale, 25);
        hxc_l_tmp_cstring_ref_owner_owner_n116 = hxc_l_tmp_instance_call_result_n331;
        hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n333 = HXC_BORROWED_CSTRING_INITIALIZER;
        if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n116, &hxc_l_tmp_string_borrow_cstring_result_n333) != HXC_STATUS_OK)
        {
          abort();
        }
        int32_t hxc_l_tmp_load_result_n334 = hxc_l_centerX;
        int32_t hxc_l_tmp_load_result_n335 = hxc_l_centerY;
        DrawText(hxc_l_tmp_string_borrow_cstring_result_n333.data, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n334, 96), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n335, 24), (int32_t)18, hxc_l_this1_hd0fc4b552ecf);
        if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n116) != HXC_STATUS_OK)
        {
          abort();
        }
      }
      else
      {
        if (hxc_l_recoveryFeedback == 3)
        {
          struct Color hxc_l_color_hcf080d7ef359 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
          hxc_string hxc_l_tmp_call_result_n339 = hxc_caxecraft_app_GameplayMessage_gameplayMessageId(13);
          hxc_l_text_h9bc58c634805 = hxc_l_tmp_call_result_n339;
          hxc_string hxc_l_text1_he11a73a0eaee = { 0 };
          switch (hxc_l_locale) {
            case 0:
              {
                hxc_l_text1_he11a73a0eaee = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
                if (hxc_string_retain(hxc_l_text1_he11a73a0eaee) != HXC_STATUS_OK)
                {
                  abort();
                }
                break;
              }
            case 1:
              {
                hxc_l_text1_he11a73a0eaee = (hxc_string){ (const uint8_t *)"es-mx", 5, true, NULL };
                if (hxc_string_retain(hxc_l_text1_he11a73a0eaee) != HXC_STATUS_OK)
                {
                  abort();
                }
                break;
              }
            default:
              {
                hxc_l_text1_he11a73a0eaee = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
                if (hxc_string_retain(hxc_l_text1_he11a73a0eaee) != HXC_STATUS_OK)
                {
                  abort();
                }
                break;
              }
          }
          hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n341 = hxc_l_text1_he11a73a0eaee;
          hxc_l_tmp_managed_flow_owner_n120 = hxc_l_tmp_managed_flow_carrier_move_result_n341;
          struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_load_result_n342 = hxc_l_presentation;
          hxc_l_gc_roots[15] = (const void *)hxc_l_tmp_load_result_n342;
          if (hxc_l_tmp_load_result_n342 == NULL)
          {
            abort();
          }
          hxc_string hxc_l_tmp_load_result_n343 = hxc_l_text_h9bc58c634805;
          hxc_string hxc_l_tmp_instance_call_result_n345 = hxc_caxecraft_content_RuntimeLevelPresentation_message(hxc_l_tmp_load_result_n342, hxc_l_tmp_load_result_n343, hxc_l_tmp_managed_flow_owner_n120);
          hxc_l_tmp_cstring_ref_owner_owner_n121 = hxc_l_tmp_instance_call_result_n345;
          hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n347 = HXC_BORROWED_CSTRING_INITIALIZER;
          if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n121, &hxc_l_tmp_string_borrow_cstring_result_n347) != HXC_STATUS_OK)
          {
            abort();
          }
          int32_t hxc_l_tmp_load_result_n348 = hxc_l_centerX;
          int32_t hxc_l_tmp_load_result_n349 = hxc_l_centerY;
          DrawText(hxc_l_tmp_string_borrow_cstring_result_n347.data, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n348, 76), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n349, 24), (int32_t)18, hxc_l_color_hcf080d7ef359);
          if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n121) != HXC_STATUS_OK)
          {
            abort();
          }
          if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n120) != HXC_STATUS_OK)
          {
            abort();
          }
          if (hxc_string_release(&hxc_l_text_h9bc58c634805) != HXC_STATUS_OK)
          {
            abort();
          }
        }
      }
    }
  }
  if (hxc_l_aquaticEquipmentVisible)
  {
    struct Color hxc_l_this1_h1b89e3abe30a = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
    hxc_string hxc_l_tmp_instance_call_result_n354 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_uiCatalog, hxc_l_locale, 0);
    hxc_l_tmp_cstring_ref_owner_owner_n123 = hxc_l_tmp_instance_call_result_n354;
    hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n356 = HXC_BORROWED_CSTRING_INITIALIZER;
    if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n123, &hxc_l_tmp_string_borrow_cstring_result_n356) != HXC_STATUS_OK)
    {
      abort();
    }
    int32_t hxc_l_tmp_load_result_n357 = hxc_l_centerX;
    int32_t hxc_l_tmp_load_result_n358 = hxc_l_centerY;
    DrawText(hxc_l_tmp_string_borrow_cstring_result_n356.data, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n357, 128), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n358, 24), (int32_t)18, hxc_l_this1_h1b89e3abe30a);
    if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n123) != HXC_STATUS_OK)
    {
      abort();
    }
  }
  if (hxc_l_vitals.hxc_safeTicks > 15)
  {
    struct Color hxc_l_this1_hfd27b699fd71 = (struct Color){ .r = 218, .g = 65, .b = 72, .a = 255 };
    int32_t hxc_l_tmp_load_result_n362 = hxc_l_width;
    int32_t hxc_l_tmp_load_result_n363 = hxc_l_height;
    DrawRectangleLines((int32_t)4, (int32_t)4, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n362, 8), (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n363, 8), hxc_l_this1_hfd27b699fd71);
  }
  if (hxc_l_vitals.hxc_health <= 0)
  {
    struct Color hxc_l_this1_h529b37cc00c8 = (struct Color){ .r = 15, .g = 27, .b = 39, .a = 220 };
    int32_t hxc_l_tmp_load_result_n367 = hxc_l_centerX;
    int32_t hxc_l_tmp_load_result_n368 = hxc_l_centerY;
    DrawRectangle((int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n367, 250), (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n368, 74), (int32_t)500, (int32_t)148, hxc_l_this1_h529b37cc00c8);
    struct Color hxc_l_this1_hae0d34585bbc = (struct Color){ .r = 218, .g = 65, .b = 72, .a = 255 };
    int32_t hxc_l_tmp_load_result_n371 = hxc_l_centerX;
    int32_t hxc_l_tmp_load_result_n372 = hxc_l_centerY;
    DrawRectangleLines((int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n371, 250), (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n372, 74), (int32_t)500, (int32_t)148, hxc_l_this1_hae0d34585bbc);
    hxc_string hxc_l_tmp_call_result_n374 = hxc_caxecraft_app_GameplayMessage_gameplayMessageId(6);
    hxc_l_text1_hee60b287d3c8 = hxc_l_tmp_call_result_n374;
    hxc_string hxc_l_text2_h3dcaabc5e2a7 = { 0 };
    switch (hxc_l_locale) {
      case 0:
        {
          hxc_l_text2_h3dcaabc5e2a7 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
          if (hxc_string_retain(hxc_l_text2_h3dcaabc5e2a7) != HXC_STATUS_OK)
          {
            abort();
          }
          break;
        }
      case 1:
        {
          hxc_l_text2_h3dcaabc5e2a7 = (hxc_string){ (const uint8_t *)"es-mx", 5, true, NULL };
          if (hxc_string_retain(hxc_l_text2_h3dcaabc5e2a7) != HXC_STATUS_OK)
          {
            abort();
          }
          break;
        }
      default:
        {
          hxc_l_text2_h3dcaabc5e2a7 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
          if (hxc_string_retain(hxc_l_text2_h3dcaabc5e2a7) != HXC_STATUS_OK)
          {
            abort();
          }
          break;
        }
    }
    hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n376 = hxc_l_text2_h3dcaabc5e2a7;
    hxc_l_tmp_managed_flow_owner_n129 = hxc_l_tmp_managed_flow_carrier_move_result_n376;
    struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_load_result_n377 = hxc_l_presentation;
    hxc_l_gc_roots[16] = (const void *)hxc_l_tmp_load_result_n377;
    if (hxc_l_tmp_load_result_n377 == NULL)
    {
      abort();
    }
    hxc_string hxc_l_tmp_load_result_n378 = hxc_l_text1_hee60b287d3c8;
    hxc_string hxc_l_tmp_instance_call_result_n380 = hxc_caxecraft_content_RuntimeLevelPresentation_message(hxc_l_tmp_load_result_n377, hxc_l_tmp_load_result_n378, hxc_l_tmp_managed_flow_owner_n129);
    hxc_l_tmp_cstring_ref_owner_owner_n130 = hxc_l_tmp_instance_call_result_n380;
    hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n382 = HXC_BORROWED_CSTRING_INITIALIZER;
    if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n130, &hxc_l_tmp_string_borrow_cstring_result_n382) != HXC_STATUS_OK)
    {
      abort();
    }
    int32_t hxc_l_tmp_load_result_n383 = hxc_l_centerX;
    int32_t hxc_l_tmp_load_result_n384 = hxc_l_centerY;
    DrawText(hxc_l_tmp_string_borrow_cstring_result_n382.data, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n383, 122), (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n384, 42), (int32_t)24, hxc_l_text_hcf65a5a292d3);
    struct Color hxc_l_color_he50f0502ecf1 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
    hxc_string hxc_l_tmp_call_result_n387 = hxc_caxecraft_app_GameplayMessage_gameplayMessageId(14);
    hxc_l_text_had2b2354bc20 = hxc_l_tmp_call_result_n387;
    hxc_string hxc_l_text1_h131741999cdc = { 0 };
    switch (hxc_l_locale) {
      case 0:
        {
          hxc_l_text1_h131741999cdc = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
          if (hxc_string_retain(hxc_l_text1_h131741999cdc) != HXC_STATUS_OK)
          {
            abort();
          }
          break;
        }
      case 1:
        {
          hxc_l_text1_h131741999cdc = (hxc_string){ (const uint8_t *)"es-mx", 5, true, NULL };
          if (hxc_string_retain(hxc_l_text1_h131741999cdc) != HXC_STATUS_OK)
          {
            abort();
          }
          break;
        }
      default:
        {
          hxc_l_text1_h131741999cdc = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
          if (hxc_string_retain(hxc_l_text1_h131741999cdc) != HXC_STATUS_OK)
          {
            abort();
          }
          break;
        }
    }
    hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n389 = hxc_l_text1_h131741999cdc;
    hxc_l_tmp_managed_flow_owner_n134 = hxc_l_tmp_managed_flow_carrier_move_result_n389;
    struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_load_result_n390 = hxc_l_presentation;
    hxc_l_gc_roots[17] = (const void *)hxc_l_tmp_load_result_n390;
    if (hxc_l_tmp_load_result_n390 == NULL)
    {
      abort();
    }
    hxc_string hxc_l_tmp_load_result_n391 = hxc_l_text_had2b2354bc20;
    hxc_string hxc_l_tmp_instance_call_result_n393 = hxc_caxecraft_content_RuntimeLevelPresentation_message(hxc_l_tmp_load_result_n390, hxc_l_tmp_load_result_n391, hxc_l_tmp_managed_flow_owner_n134);
    hxc_l_tmp_cstring_ref_owner_owner_n135 = hxc_l_tmp_instance_call_result_n393;
    hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n395 = HXC_BORROWED_CSTRING_INITIALIZER;
    if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n135, &hxc_l_tmp_string_borrow_cstring_result_n395) != HXC_STATUS_OK)
    {
      abort();
    }
    int32_t hxc_l_tmp_load_result_n396 = hxc_l_centerX;
    int32_t hxc_l_tmp_load_result_n397 = hxc_l_centerY;
    DrawText(hxc_l_tmp_string_borrow_cstring_result_n395.data, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n396, 125), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n397, 10), (int32_t)18, hxc_l_color_he50f0502ecf1);
    if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n135) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n134) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_text_had2b2354bc20) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n130) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n129) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_text1_hee60b287d3c8) != HXC_STATUS_OK)
    {
      abort();
    }
  }
  if (hxc_l_paused)
  {
    struct Color hxc_l_this1_hc7cb94dbeeed = (struct Color){ .r = 15, .g = 27, .b = 39, .a = 220 };
    int32_t hxc_l_tmp_load_result_n401 = hxc_l_centerX;
    int32_t hxc_l_tmp_load_result_n402 = hxc_l_centerY;
    DrawRectangle((int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n401, 170), (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n402, 48), (int32_t)340, (int32_t)96, hxc_l_this1_hc7cb94dbeeed);
    struct Color hxc_l_this1_h3ea4965514d1 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
    int32_t hxc_l_tmp_load_result_n405 = hxc_l_centerX;
    int32_t hxc_l_tmp_load_result_n406 = hxc_l_centerY;
    DrawRectangleLines((int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n405, 170), (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n406, 48), (int32_t)340, (int32_t)96, hxc_l_this1_h3ea4965514d1);
    hxc_string hxc_l_tmp_instance_call_result_n409 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_uiCatalog, hxc_l_locale, 32);
    hxc_l_tmp_cstring_ref_owner_owner_n138 = hxc_l_tmp_instance_call_result_n409;
    hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n411 = HXC_BORROWED_CSTRING_INITIALIZER;
    if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n138, &hxc_l_tmp_string_borrow_cstring_result_n411) != HXC_STATUS_OK)
    {
      abort();
    }
    int32_t hxc_l_tmp_load_result_n412 = hxc_l_centerX;
    int32_t hxc_l_tmp_load_result_n413 = hxc_l_centerY;
    DrawText(hxc_l_tmp_string_borrow_cstring_result_n411.data, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n412, 48), (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n413, 30), (int32_t)24, hxc_l_text_hcf65a5a292d3);
    hxc_string hxc_l_tmp_instance_call_result_n416 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_uiCatalog, hxc_l_locale, 31);
    hxc_l_tmp_cstring_ref_owner_owner_n139 = hxc_l_tmp_instance_call_result_n416;
    hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n418 = HXC_BORROWED_CSTRING_INITIALIZER;
    if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n139, &hxc_l_tmp_string_borrow_cstring_result_n418) != HXC_STATUS_OK)
    {
      abort();
    }
    int32_t hxc_l_tmp_load_result_n419 = hxc_l_centerX;
    int32_t hxc_l_tmp_load_result_n420 = hxc_l_centerY;
    DrawText(hxc_l_tmp_string_borrow_cstring_result_n418.data, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n419, 160), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n420, 8), (int32_t)16, hxc_l_text_hcf65a5a292d3);
    if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n139) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n138) != HXC_STATUS_OK)
    {
      abort();
    }
  }
  else
  {
    if (hxc_l_placementBlocked)
    {
      struct Color hxc_l_this1_h659d4998ac87 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
      hxc_string hxc_l_tmp_instance_call_result_n425 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_uiCatalog, hxc_l_locale, 33);
      hxc_l_tmp_cstring_ref_owner_owner_n141 = hxc_l_tmp_instance_call_result_n425;
      hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n427 = HXC_BORROWED_CSTRING_INITIALIZER;
      if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n141, &hxc_l_tmp_string_borrow_cstring_result_n427) != HXC_STATUS_OK)
      {
        abort();
      }
      int32_t hxc_l_tmp_load_result_n428 = hxc_l_centerX;
      int32_t hxc_l_tmp_load_result_n429 = hxc_l_centerY;
      DrawText(hxc_l_tmp_string_borrow_cstring_result_n427.data, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n428, 170), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n429, 26), (int32_t)14, hxc_l_this1_h659d4998ac87);
      if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n141) != HXC_STATUS_OK)
      {
        abort();
      }
    }
    else
    {
      if (!hxc_l_captured)
      {
        hxc_string hxc_l_tmp_instance_call_result_n433 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_uiCatalog, hxc_l_locale, 2);
        hxc_l_tmp_cstring_ref_owner_owner_n142 = hxc_l_tmp_instance_call_result_n433;
        hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n435 = HXC_BORROWED_CSTRING_INITIALIZER;
        if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n142, &hxc_l_tmp_string_borrow_cstring_result_n435) != HXC_STATUS_OK)
        {
          abort();
        }
        int32_t hxc_l_tmp_load_result_n436 = hxc_l_centerX;
        int32_t hxc_l_tmp_load_result_n437 = hxc_l_centerY;
        DrawText(hxc_l_tmp_string_borrow_cstring_result_n435.data, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n436, 90), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n437, 26), (int32_t)14, hxc_l_text_hcf65a5a292d3);
        if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n142) != HXC_STATUS_OK)
        {
          abort();
        }
      }
      else
      {
        if (!hxc_l_hit.hxc_hit)
        {
          hxc_string hxc_l_tmp_instance_call_result_n441 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_uiCatalog, hxc_l_locale, 30);
          hxc_l_tmp_cstring_ref_owner_owner_n143 = hxc_l_tmp_instance_call_result_n441;
          hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n443 = HXC_BORROWED_CSTRING_INITIALIZER;
          if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n143, &hxc_l_tmp_string_borrow_cstring_result_n443) != HXC_STATUS_OK)
          {
            abort();
          }
          int32_t hxc_l_tmp_load_result_n444 = hxc_l_centerX;
          int32_t hxc_l_tmp_load_result_n445 = hxc_l_centerY;
          DrawText(hxc_l_tmp_string_borrow_cstring_result_n443.data, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n444, 105), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n445, 26), (int32_t)14, hxc_l_text_hcf65a5a292d3);
          if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n143) != HXC_STATUS_OK)
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
  if (hxc_string_release(&hxc_l_text1_h359588d2548b) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n47) != HXC_STATUS_OK)
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
  const void *hxc_l_gc_roots[248] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 248, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_string hxc_l_tmp_sys_println_string_argument_owner_n179 = { 0 };
  struct hxc_caxecraft_content_CampaignPackageLoadResult hxc_l_g_h6904c3060e30 = { 0 };
  struct hxc_caxecraft_content_CampaignLevelLoadResult hxc_l_g_h3a886f21d5d2 = { 0 };
  struct hxc_caxecraft_content_RuntimeLevelReceipt hxc_l_tmp_record_field_logicalPath_receiver_owner_n22 = { 0 };
  struct hxc_array_ref *hxc_l_phases = { 0 };
  hxc_string hxc_l_tmp_string_concat_left_owner_n223 = { 0 };
  hxc_string hxc_l_tmp_string_concat_left_owner_n224 = { 0 };
  hxc_string hxc_l_tmp_string_concat_right_owner_n225 = { 0 };
  hxc_string hxc_l_tmp_sys_println_string_argument_owner_n226 = { 0 };
  struct hxc_caxecraft_content_CampaignLevelLoadError hxc_l_error = { 0 };
  hxc_string hxc_l_tmp_string_concat_right_owner_n228 = { 0 };
  hxc_string hxc_l_tmp_sys_println_string_argument_owner_n229 = { 0 };
  struct hxc_caxecraft_content_CampaignPackageLoadError hxc_l_gerror = { 0 };
  hxc_string hxc_l_levelLabel = { 0 };
  struct hxc_caxecraft_content_RuntimeLevelReceipt hxc_l_tmp_record_field_logicalPath_receiver_owner_n26 = { 0 };
  struct hxc_caxecraft_domain_GameTickResult hxc_l_gameTick = { 0 };
  struct hxc_optional_caxecraft_scenario_FlowTickResult hxc_l_flow = { 0 };
  struct hxc_array_ref *hxc_l_g1_h24ec6a7c61fb = { 0 };
  struct hxc_caxecraft_scenario_FlowPresentationEvent hxc_l_event_h613c6941bfa8 = { 0 };
  struct hxc_caxecraft_scenario_FlowPresentationEvent hxc_l_tmp_array_element_owner_n276 = { 0 };
  hxc_string hxc_l_id = { 0 };
  struct hxc_array_ref *hxc_l_g1_h86761ddd48e2 = { 0 };
  struct hxc_caxecraft_domain_ActorControllerEvent hxc_l_event_h6e7c030f6037 = { 0 };
  struct hxc_caxecraft_domain_ActorControllerEvent hxc_l_tmp_array_element_owner_n293 = { 0 };
  hxc_string hxc_l_drop = { 0 };
  struct hxc_array_ref *hxc_l_actorPhases = { 0 };
  struct hxc_array_ref *hxc_l_presentationActorPhases = { 0 };
  struct hxc_caxecraft_content_RuntimeLevelReceipt hxc_l_receipt = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n341 = { 0 };
  hxc_string hxc_l_adventureTagline = { 0 };
  hxc_string hxc_l_tmp_sys_println_string_argument_owner_n35 = { 0 };
  hxc_string hxc_l_tmp_string_concat_right_owner_n36 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n363 = { 0 };
  hxc_string hxc_l_tmp_sys_println_string_argument_owner_n37 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n371 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n379 = { 0 };
  hxc_string hxc_l_tmp_string_concat_right_owner_n38 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n385 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n388 = { 0 };
  hxc_string hxc_l_tmp_sys_println_string_argument_owner_n39 = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n394 = { 0 };
  hxc_string hxc_l_tmp = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n398 = { 0 };
  hxc_string hxc_l_tmp_static_call_argument_9_owner_n399 = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n430 = { 0 };
  hxc_string hxc_l_hudView7 = { 0 };
  struct hxc_caxecraft_app_HudView hxc_l_hudView_h9b78e944debd = { 0 };
  struct hxc_caxecraft_content_RuntimeContentLoadResult hxc_l_g_h4ef171f4fe67 = { 0 };
  hxc_string hxc_l_activeDialogue = { 0 };
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
  hxc_l_activeDialogue = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
  if (hxc_string_retain(hxc_l_activeDialogue) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n141 = hxc_l_initialLevel;
  hxc_l_gc_roots[75] = (const void *)hxc_l_tmp_load_result_n141;
  if (hxc_l_tmp_load_result_n141 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_class_field_load_result_n142 = (*hxc_l_tmp_load_result_n141).hxc_levelPresentation;
  hxc_l_gc_roots[76] = (const void *)hxc_l_tmp_class_field_load_result_n142;
  if (hxc_l_tmp_class_field_load_result_n142 == NULL)
  {
    abort();
  }
  hxc_string hxc_l_tmp_class_field_load_result_n143 = (*hxc_l_tmp_class_field_load_result_n142).hxc_startingObjective;
  hxc_l_currentObjectiveId = hxc_l_tmp_class_field_load_result_n143;
  if (hxc_string_retain(hxc_l_currentObjectiveId) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n144 = hxc_l_initialSession;
  hxc_l_gc_roots[77] = (const void *)hxc_l_tmp_load_result_n144;
  if (hxc_l_tmp_load_result_n144 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n145 = hxc_l_initialLevel;
  hxc_l_gc_roots[78] = (const void *)hxc_l_tmp_load_result_n145;
  if (hxc_l_tmp_load_result_n145 == NULL)
  {
    abort();
  }
  int32_t hxc_l_tmp_class_field_load_result_n146 = (*hxc_l_tmp_load_result_n145).hxc_dialogueId;
  bool hxc_l_tmp_instance_call_result_n147 = hxc_caxecraft_domain_GameSession_actorInteractionAvailable(hxc_l_tmp_load_result_n144, hxc_l_tmp_class_field_load_result_n146);
  bool hxc_l_guideInteractionAvailable = hxc_l_tmp_instance_call_result_n147;
  struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n148 = hxc_l_initialSession;
  hxc_l_gc_roots[79] = (const void *)hxc_l_tmp_load_result_n148;
  if (hxc_l_tmp_load_result_n148 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n149 = hxc_l_initialLevel;
  hxc_l_gc_roots[80] = (const void *)hxc_l_tmp_load_result_n149;
  if (hxc_l_tmp_load_result_n149 == NULL)
  {
    abort();
  }
  int32_t hxc_l_tmp_class_field_load_result_n150 = (*hxc_l_tmp_load_result_n149).hxc_dialogueId;
  struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n151 = hxc_caxecraft_domain_GameSession_readCharacter(hxc_l_tmp_load_result_n148, hxc_l_tmp_class_field_load_result_n150);
  struct hxc_caxecraft_domain_Character hxc_l_dialogueActor = hxc_l_tmp_instance_call_result_n151;
  struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n152 = hxc_l_initialSession;
  hxc_l_gc_roots[81] = (const void *)hxc_l_tmp_load_result_n152;
  if (hxc_l_tmp_load_result_n152 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n153 = hxc_l_initialLevel;
  hxc_l_gc_roots[82] = (const void *)hxc_l_tmp_load_result_n153;
  if (hxc_l_tmp_load_result_n153 == NULL)
  {
    abort();
  }
  int32_t hxc_l_tmp_class_field_load_result_n154 = (*hxc_l_tmp_load_result_n153).hxc_enemyId;
  struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n155 = hxc_caxecraft_domain_GameSession_readCharacter(hxc_l_tmp_load_result_n152, hxc_l_tmp_class_field_load_result_n154);
  struct hxc_caxecraft_domain_Character hxc_l_enemyActor = hxc_l_tmp_instance_call_result_n155;
  struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n156 = hxc_l_initialSession;
  hxc_l_gc_roots[83] = (const void *)hxc_l_tmp_load_result_n156;
  if (hxc_l_tmp_load_result_n156 == NULL)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_tmp_instance_call_result_n157 = hxc_caxecraft_domain_GameSession_actorControllerStateSnapshots(hxc_l_tmp_load_result_n156);
  hxc_l_initialActorPhases = hxc_l_tmp_instance_call_result_n157;
  struct hxc_array_ref *hxc_l_tmp_load_result_n158 = hxc_l_initialActorPhases;
  struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n159 = hxc_l_initialLevel;
  hxc_l_gc_roots[84] = (const void *)hxc_l_tmp_load_result_n159;
  if (hxc_l_tmp_load_result_n159 == NULL)
  {
    abort();
  }
  int32_t hxc_l_tmp_class_field_load_result_n160 = (*hxc_l_tmp_load_result_n159).hxc_dialogueId;
  struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation hxc_l_tmp_call_result_n162 = hxc_caxecraft_app_CaxecraftApp_observeActorPhase(hxc_l_tmp_load_result_n158, hxc_l_tmp_class_field_load_result_n160, hxc_caxecraft_domain_ActorControllerPhase_Stationary);
  struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation hxc_l_dialoguePhase = hxc_l_tmp_call_result_n162;
  struct hxc_array_ref *hxc_l_tmp_load_result_n163 = hxc_l_initialActorPhases;
  struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n164 = hxc_l_initialLevel;
  hxc_l_gc_roots[85] = (const void *)hxc_l_tmp_load_result_n164;
  if (hxc_l_tmp_load_result_n164 == NULL)
  {
    abort();
  }
  int32_t hxc_l_tmp_class_field_load_result_n165 = (*hxc_l_tmp_load_result_n164).hxc_enemyId;
  struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation hxc_l_tmp_call_result_n167 = hxc_caxecraft_app_CaxecraftApp_observeActorPhase(hxc_l_tmp_load_result_n163, hxc_l_tmp_class_field_load_result_n165, hxc_caxecraft_domain_ActorControllerPhase_Resting);
  struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation hxc_l_enemyPhase_h70382f3bdc66 = hxc_l_tmp_call_result_n167;
  int32_t hxc_l_tmp_record_field_load_result_n168 = hxc_l_dialogueActor.hxc_id;
  bool hxc_l_tmp_short_circuit_result_n68 = hxc_l_tmp_record_field_load_result_n168 <= 0;
  if (!(hxc_l_tmp_record_field_load_result_n168 <= 0))
  {
    hxc_l_tmp_short_circuit_result_n68 = hxc_l_enemyActor.hxc_id <= 0;
  }
  bool hxc_l_tmp_short_circuit_load_result_n170 = hxc_l_tmp_short_circuit_result_n68;
  bool hxc_l_tmp_short_circuit_result_n69 = hxc_l_tmp_short_circuit_load_result_n170;
  if (!hxc_l_tmp_short_circuit_load_result_n170)
  {
    hxc_l_tmp_short_circuit_result_n69 = !hxc_l_dialoguePhase.hxc_valid;
  }
  bool hxc_l_tmp_short_circuit_load_result_n172 = hxc_l_tmp_short_circuit_result_n69;
  bool hxc_l_tmp_short_circuit_result_n70 = hxc_l_tmp_short_circuit_load_result_n172;
  if (!hxc_l_tmp_short_circuit_load_result_n172)
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
    if (hxc_string_release(&hxc_l_activeDialogue) != HXC_STATUS_OK)
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
  struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n177 = hxc_l_initialSession;
  hxc_l_gc_roots[86] = (const void *)hxc_l_tmp_load_result_n177;
  if (hxc_l_tmp_load_result_n177 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_domain_GameView hxc_l_tmp_instance_call_result_n178 = hxc_caxecraft_domain_GameSession_view(hxc_l_tmp_load_result_n177);
  struct hxc_caxecraft_domain_GameView hxc_l_initialPresentation = hxc_l_tmp_instance_call_result_n178;
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
    if (hxc_string_release(&hxc_l_activeDialogue) != HXC_STATUS_OK)
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
  struct hxc_caxecraft_app_MotionHistory hxc_l_tmp_call_result_n181 = hxc_caxecraft_app_MotionInterpolation_start(hxc_l_initialPresentation.hxc_localPlayer.hxc_body);
  struct hxc_caxecraft_app_MotionHistory hxc_l_motionHistory = hxc_l_tmp_call_result_n181;
  bool hxc_l_jumpQueued = false;
  bool hxc_l_swordQueued = false;
  int32_t hxc_l_selectedMode = 0;
  int32_t hxc_l_locale = 0;
  bool hxc_l_showInitialTitle = true;
  enum hxc_caxecraft_app_AppScreen hxc_l_tmp_call_result_n183 = hxc_caxecraft_app_AppScreen_initialScreen(hxc_l_showInitialTitle);
  enum hxc_caxecraft_app_AppScreen hxc_l_screen = hxc_l_tmp_call_result_n183;
  bool hxc_l_tmp_call_result_n185 = hxc_caxecraft_app_AppScreen_capturesPointer(hxc_l_screen);
  if (hxc_l_tmp_call_result_n185)
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
    bool hxc_l_tmp_load_result_n186 = hxc_l_quit;
    bool hxc_l_tmp_short_circuit_result_n99 = !hxc_l_tmp_load_result_n186;
    if (!hxc_l_tmp_load_result_n186)
    {
      bool hxc_l_tmp_native_call_result_n187 = WindowShouldClose();
      hxc_l_tmp_short_circuit_result_n99 = !hxc_l_tmp_native_call_result_n187;
    }
    if (!hxc_l_tmp_short_circuit_result_n99)
    {
      break;
    }
    struct hxc_caxecraft_app_ActivePlayableLevel *hxc_l_tmp_load_result_n189 = hxc_l_activeLevel;
    hxc_l_gc_roots[87] = (const void *)hxc_l_tmp_load_result_n189;
    if (hxc_l_tmp_load_result_n189 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_class_field_load_result_n190 = (*hxc_l_tmp_load_result_n189).hxc_activeView;
    hxc_l_gc_roots[88] = (const void *)hxc_l_tmp_class_field_load_result_n190;
    struct hxc_caxecraft_app_PlayableLevelView *hxc_l_levelView = hxc_l_tmp_class_field_load_result_n190;
    struct hxc_caxecraft_app_ActivePlayableLevel *hxc_l_tmp_load_result_n191 = hxc_l_activeLevel;
    hxc_l_gc_roots[89] = (const void *)hxc_l_tmp_load_result_n191;
    if (hxc_l_tmp_load_result_n191 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_content_ActiveContent *hxc_l_tmp_class_field_load_result_n192 = (*hxc_l_tmp_load_result_n191).hxc_content;
    hxc_l_gc_roots[90] = (const void *)hxc_l_tmp_class_field_load_result_n192;
    if (hxc_l_tmp_class_field_load_result_n192 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_content_LoadedContentGeneration *hxc_l_tmp_class_field_load_result_n193 = (*hxc_l_tmp_class_field_load_result_n192).hxc_active;
    hxc_l_gc_roots[91] = (const void *)hxc_l_tmp_class_field_load_result_n193;
    if (hxc_l_tmp_class_field_load_result_n193 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_class_field_load_result_n194 = (*hxc_l_tmp_class_field_load_result_n193).hxc_ownedSession;
    hxc_l_gc_roots[92] = (const void *)hxc_l_tmp_class_field_load_result_n194;
    struct hxc_caxecraft_domain_GameSession *hxc_l_session = hxc_l_tmp_class_field_load_result_n194;
    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n195 = hxc_l_session;
    hxc_l_gc_roots[93] = (const void *)hxc_l_tmp_load_result_n195;
    if (hxc_l_tmp_load_result_n195 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_domain_GameView hxc_l_tmp_instance_call_result_n196 = hxc_caxecraft_domain_GameSession_view(hxc_l_tmp_load_result_n195);
    struct hxc_caxecraft_domain_GameView hxc_l_initialView = hxc_l_tmp_instance_call_result_n196;
    if (!hxc_l_initialView.hxc_valid)
    {
      hxc_l_quit = true;
    }
    struct hxc_caxecraft_domain_Character hxc_l_character = hxc_l_initialView.hxc_localPlayer;
    bool hxc_l_recapturedThisFrame = false;
    bool hxc_l_resetMotionThisFrame = false;
    float hxc_l_tmp_native_call_result_n199 = GetFrameTime();
    double hxc_l_frameSeconds = (double)hxc_l_tmp_native_call_result_n199;
    if (hxc_l_frameSeconds > 0.25)
    {
      hxc_l_frameSeconds = 0.25;
    }
    bool hxc_l_tmp_native_call_result_n201 = IsWindowFocused();
    bool hxc_l_focused = hxc_l_tmp_native_call_result_n201;
    bool hxc_l_tmp_call_result_n203 = hxc_caxecraft_app_AppScreen_capturesPointer(hxc_l_screen);
    bool hxc_l_captured_ha937dc084970 = hxc_l_tmp_call_result_n203;
    bool hxc_l_tmp_call_result_n205 = hxc_caxecraft_app_AppScreen_pausesSimulation(hxc_l_screen);
    bool hxc_l_paused_h81510c86da03 = hxc_l_tmp_call_result_n205;
    double hxc_l_forward = 0.0;
    double hxc_l_right = 0.0;
    bool hxc_l_tmp_native_call_result_n206 = IsKeyDown((int32_t)87);
    if (hxc_l_tmp_native_call_result_n206)
    {
      hxc_l_forward = hxc_l_forward + 1.0;
    }
    bool hxc_l_tmp_native_call_result_n208 = IsKeyDown((int32_t)83);
    if (hxc_l_tmp_native_call_result_n208)
    {
      hxc_l_forward = hxc_l_forward - 1.0;
    }
    bool hxc_l_tmp_native_call_result_n210 = IsKeyDown((int32_t)68);
    if (hxc_l_tmp_native_call_result_n210)
    {
      hxc_l_right = hxc_l_right + 1.0;
    }
    bool hxc_l_tmp_native_call_result_n212 = IsKeyDown((int32_t)65);
    if (hxc_l_tmp_native_call_result_n212)
    {
      hxc_l_right = hxc_l_right - 1.0;
    }
    double hxc_l_lookYaw_h4b427ea52cca = 0.0;
    double hxc_l_lookPitch_h341fb5abf219 = 0.0;
    if (hxc_l_captured_ha937dc084970)
    {
      struct Vector2 hxc_l_tmp_native_call_result_n215 = GetMouseDelta();
      struct Vector2 hxc_l_mouse = hxc_l_tmp_native_call_result_n215;
      hxc_l_lookYaw_h4b427ea52cca = -(double)hxc_l_mouse.x * 0.0025;
      hxc_l_lookPitch_h341fb5abf219 = -(double)hxc_l_mouse.y * 0.0025;
    }
    bool hxc_l_tmp_native_call_result_n220 = IsMouseButtonPressed((int32_t)0);
    bool hxc_l_leftPressed = hxc_l_tmp_native_call_result_n220;
    bool hxc_l_tmp_load_result_n221 = hxc_l_captured_ha937dc084970;
    bool hxc_l_tmp_short_circuit_result_n117 = hxc_l_tmp_load_result_n221;
    if (hxc_l_tmp_load_result_n221)
    {
      hxc_l_tmp_short_circuit_result_n117 = hxc_l_leftPressed;
    }
    bool hxc_l_primaryPressed_h2077966ca310 = hxc_l_tmp_short_circuit_result_n117;
    bool hxc_l_tmp_load_result_n224 = hxc_l_captured_ha937dc084970;
    bool hxc_l_tmp_short_circuit_result_n119 = hxc_l_tmp_load_result_n224;
    if (hxc_l_tmp_load_result_n224)
    {
      bool hxc_l_tmp_native_call_result_n225 = IsMouseButtonPressed((int32_t)1);
      hxc_l_tmp_short_circuit_result_n119 = hxc_l_tmp_native_call_result_n225;
    }
    bool hxc_l_secondaryPressed_h0a910fc34e11 = hxc_l_tmp_short_circuit_result_n119;
    bool hxc_l_tmp_load_result_n227 = hxc_l_captured_ha937dc084970;
    bool hxc_l_tmp_short_circuit_result_n121 = hxc_l_tmp_load_result_n227;
    if (hxc_l_tmp_load_result_n227)
    {
      bool hxc_l_tmp_native_call_result_n228 = IsKeyPressed((int32_t)69);
      hxc_l_tmp_short_circuit_result_n121 = hxc_l_tmp_native_call_result_n228;
    }
    bool hxc_l_interactPressed_hd6f15ac879ee = hxc_l_tmp_short_circuit_result_n121;
    bool hxc_l_tmp_load_result_n230 = hxc_l_captured_ha937dc084970;
    bool hxc_l_tmp_short_circuit_result_n123 = hxc_l_tmp_load_result_n230;
    if (hxc_l_tmp_load_result_n230)
    {
      bool hxc_l_tmp_native_call_result_n231 = IsKeyPressed((int32_t)78);
      hxc_l_tmp_short_circuit_result_n123 = hxc_l_tmp_native_call_result_n231;
    }
    bool hxc_l_travelPressed_h072a7f039d00 = hxc_l_tmp_short_circuit_result_n123;
    int32_t hxc_l_hotbarSelection_hbf014f0482d2 = -1;
    bool hxc_l_tmp_native_call_result_n233 = IsKeyPressed((int32_t)49);
    if (hxc_l_tmp_native_call_result_n233)
    {
      hxc_l_hotbarSelection_hbf014f0482d2 = 0;
    }
    bool hxc_l_tmp_native_call_result_n234 = IsKeyPressed((int32_t)50);
    if (hxc_l_tmp_native_call_result_n234)
    {
      hxc_l_hotbarSelection_hbf014f0482d2 = 1;
    }
    bool hxc_l_tmp_native_call_result_n235 = IsKeyPressed((int32_t)51);
    if (hxc_l_tmp_native_call_result_n235)
    {
      hxc_l_hotbarSelection_hbf014f0482d2 = 2;
    }
    bool hxc_l_tmp_native_call_result_n236 = IsKeyPressed((int32_t)52);
    if (hxc_l_tmp_native_call_result_n236)
    {
      hxc_l_hotbarSelection_hbf014f0482d2 = 3;
    }
    bool hxc_l_tmp_native_call_result_n237 = IsKeyPressed((int32_t)53);
    if (hxc_l_tmp_native_call_result_n237)
    {
      hxc_l_hotbarSelection_hbf014f0482d2 = 4;
    }
    bool hxc_l_tmp_native_call_result_n238 = IsKeyPressed((int32_t)54);
    if (hxc_l_tmp_native_call_result_n238)
    {
      hxc_l_hotbarSelection_hbf014f0482d2 = 5;
    }
    bool hxc_l_tmp_native_call_result_n239 = IsKeyPressed((int32_t)55);
    if (hxc_l_tmp_native_call_result_n239)
    {
      hxc_l_hotbarSelection_hbf014f0482d2 = 6;
    }
    bool hxc_l_tmp_native_call_result_n240 = IsKeyPressed((int32_t)56);
    if (hxc_l_tmp_native_call_result_n240)
    {
      hxc_l_hotbarSelection_hbf014f0482d2 = 7;
    }
    float hxc_l_tmp_native_call_result_n241 = GetMouseWheelMove();
    double hxc_l_wheel = (double)hxc_l_tmp_native_call_result_n241;
    int32_t hxc_l_hotbarCycle_h54a5709df70d = 0;
    if (hxc_l_wheel > 0.0)
    {
      hxc_l_hotbarCycle_h54a5709df70d = -1;
    }
    if (hxc_l_wheel < 0.0)
    {
      hxc_l_hotbarCycle_h54a5709df70d = 1;
    }
    bool hxc_l_tmp_native_call_result_n244 = IsKeyPressed((int32_t)32);
    bool hxc_l_jumpPressed_hf3b25329cd7b = hxc_l_tmp_native_call_result_n244;
    bool hxc_l_tmp_native_call_result_n245 = IsKeyPressed((int32_t)256);
    bool hxc_l_pausePressed_h459140fe9151 = hxc_l_tmp_native_call_result_n245;
    bool hxc_l_tmp_native_call_result_n246 = IsKeyPressed((int32_t)81);
    bool hxc_l_quitPressed_ha7a2ce4c2880 = hxc_l_tmp_native_call_result_n246;
    bool hxc_l_tmp_native_call_result_n247 = IsKeyDown((int32_t)340);
    bool hxc_l_descendHeld_ha5f264381ed6 = hxc_l_tmp_native_call_result_n247;
    bool hxc_l_tmp_native_call_result_n248 = IsKeyPressed((int32_t)265);
    bool hxc_l_tmp_short_circuit_result_n132 = hxc_l_tmp_native_call_result_n248;
    if (!hxc_l_tmp_native_call_result_n248)
    {
      bool hxc_l_tmp_native_call_result_n249 = IsKeyPressed((int32_t)264);
      hxc_l_tmp_short_circuit_result_n132 = hxc_l_tmp_native_call_result_n249;
    }
    bool hxc_l_menuNextPressed_hda49829cefbb = hxc_l_tmp_short_circuit_result_n132;
    bool hxc_l_tmp_native_call_result_n251 = IsKeyPressed((int32_t)257);
    bool hxc_l_menuConfirmPressed_hbb1db668e7f9 = hxc_l_tmp_native_call_result_n251;
    double hxc_l_frameInput_moveForward = hxc_l_forward;
    double hxc_l_frameInput_moveRight = hxc_l_right;
    double hxc_l_frameInput_lookYaw = hxc_l_lookYaw_h4b427ea52cca;
    double hxc_l_frameInput_lookPitch = hxc_l_lookPitch_h341fb5abf219;
    bool hxc_l_frameInput_jumpPressed = hxc_l_jumpPressed_hf3b25329cd7b;
    bool hxc_l_tmp_native_call_result_n257 = IsKeyDown((int32_t)32);
    bool hxc_l_frameInput_riseHeld = hxc_l_tmp_native_call_result_n257;
    bool hxc_l_frameInput_descendHeld = hxc_l_descendHeld_ha5f264381ed6;
    bool hxc_l_frameInput_primaryPressed = hxc_l_primaryPressed_h2077966ca310;
    bool hxc_l_frameInput_secondaryPressed = hxc_l_secondaryPressed_h0a910fc34e11;
    bool hxc_l_frameInput_interactPressed = hxc_l_interactPressed_hd6f15ac879ee;
    bool hxc_l_frameInput_travelPressed = hxc_l_travelPressed_h072a7f039d00;
    bool hxc_l_frameInput_menuNextPressed = hxc_l_menuNextPressed_hda49829cefbb;
    bool hxc_l_frameInput_menuConfirmPressed = hxc_l_menuConfirmPressed_hbb1db668e7f9;
    bool hxc_l_frameInput_pausePressed = hxc_l_pausePressed_h459140fe9151;
    bool hxc_l_tmp_load_result_n266 = hxc_l_paused_h81510c86da03;
    bool hxc_l_tmp_short_circuit_result_n149 = hxc_l_tmp_load_result_n266;
    if (hxc_l_tmp_load_result_n266)
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
    bool hxc_l_tmp_load_result_n290 = hxc_l_quit;
    bool hxc_l_tmp_short_circuit_result_n173 = !hxc_l_tmp_load_result_n290;
    if (!hxc_l_tmp_load_result_n290)
    {
      enum hxc_caxecraft_app_AppScreen hxc_l_tmp_load_result_n291 = hxc_l_screen;
      bool hxc_l_tmp_call_result_n293 = hxc_caxecraft_app_TitleMenuFlow_allowsCampaignTravel(hxc_l_tmp_load_result_n291, hxc_l_selectedMode);
      hxc_l_tmp_short_circuit_result_n173 = hxc_l_tmp_call_result_n293;
    }
    bool hxc_l_tmp_short_circuit_load_result_n294 = hxc_l_tmp_short_circuit_result_n173;
    bool hxc_l_tmp_short_circuit_result_n174 = hxc_l_tmp_short_circuit_load_result_n294;
    if (hxc_l_tmp_short_circuit_load_result_n294)
    {
      hxc_l_tmp_short_circuit_result_n174 = hxc_l_travelPressed_h9eaa3eecd919;
    }
    if (hxc_l_tmp_short_circuit_result_n174)
    {
      hxc_l_gc_roots[95] = (const void *)hxc_l_campaign;
      struct hxc_caxecraft_content_CampaignManifest *hxc_l_selectedCampaign_h2f0069f67284 = hxc_l_campaign;
      hxc_l_gc_roots[96] = (const void *)hxc_l_campaignLevel;
      struct hxc_caxecraft_content_CampaignLevel *hxc_l_sourceLevel = hxc_l_campaignLevel;
      struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n299 = hxc_l_selectedCampaign_h2f0069f67284;
      hxc_l_gc_roots[97] = (const void *)hxc_l_tmp_load_result_n299;
      hxc_l_gc_roots[98] = (const void *)NULL;
      bool hxc_l_tmp_short_circuit_result_n177 = hxc_l_tmp_load_result_n299 != NULL;
      if (hxc_l_tmp_load_result_n299 != NULL)
      {
        hxc_l_gc_roots[99] = (const void *)hxc_l_sourceLevel;
        hxc_l_gc_roots[100] = (const void *)NULL;
        hxc_l_tmp_short_circuit_result_n177 = hxc_l_sourceLevel != NULL;
      }
      if (hxc_l_tmp_short_circuit_result_n177)
      {
        struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n302 = hxc_l_selectedCampaign_h2f0069f67284;
        hxc_l_gc_roots[101] = (const void *)hxc_l_tmp_load_result_n302;
        if (hxc_l_tmp_load_result_n302 == NULL)
        {
          abort();
        }
        struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_load_result_n303 = hxc_l_sourceLevel;
        hxc_l_gc_roots[102] = (const void *)hxc_l_tmp_load_result_n303;
        if (hxc_l_tmp_load_result_n303 == NULL)
        {
          abort();
        }
        hxc_string hxc_l_tmp_class_field_load_result_n304 = (*hxc_l_tmp_load_result_n303).hxc_id;
        struct hxc_caxecraft_content_CampaignTransition *hxc_l_tmp_instance_call_result_n305 = hxc_caxecraft_content_CampaignManifest_unambiguousTransitionFrom(hxc_l_tmp_load_result_n302, hxc_l_tmp_class_field_load_result_n304);
        hxc_l_gc_roots[103] = (const void *)hxc_l_tmp_instance_call_result_n305;
        struct hxc_caxecraft_content_CampaignTransition *hxc_l_transition = hxc_l_tmp_instance_call_result_n305;
        hxc_l_gc_roots[104] = (const void *)hxc_l_transition;
        hxc_l_gc_roots[105] = (const void *)NULL;
        if (hxc_l_transition == NULL)
        {
          struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_load_result_n307 = hxc_l_sourceLevel;
          hxc_l_gc_roots[106] = (const void *)hxc_l_tmp_load_result_n307;
          if (hxc_l_tmp_load_result_n307 == NULL)
          {
            abort();
          }
          hxc_string hxc_l_tmp_class_field_load_result_n308 = (*hxc_l_tmp_load_result_n307).hxc_id;
          hxc_string hxc_l_tmp_string_concat_result_n309 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
          if (hxc_string_concat_ref((hxc_string){ (const uint8_t *)"caxecraft: campaign travel requires exactly one outgoing transition from ", 73, true, NULL }, hxc_l_tmp_class_field_load_result_n308, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n309) != HXC_STATUS_OK)
          {
            abort();
          }
          hxc_l_tmp_sys_println_string_argument_owner_n179 = hxc_l_tmp_string_concat_result_n309;
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
            if (hxc_string_release(&hxc_l_activeDialogue) != HXC_STATUS_OK)
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
          struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n311 = hxc_l_selectedCampaign_h2f0069f67284;
          hxc_l_gc_roots[107] = (const void *)hxc_l_tmp_load_result_n311;
          if (hxc_l_tmp_load_result_n311 == NULL)
          {
            abort();
          }
          struct hxc_caxecraft_content_CampaignTransition *hxc_l_tmp_load_result_n312 = hxc_l_transition;
          hxc_l_gc_roots[108] = (const void *)hxc_l_tmp_load_result_n312;
          if (hxc_l_tmp_load_result_n312 == NULL)
          {
            abort();
          }
          hxc_string hxc_l_tmp_class_field_load_result_n313 = (*hxc_l_tmp_load_result_n312).hxc_destinationLevel;
          struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_instance_call_result_n314 = hxc_caxecraft_content_CampaignManifest_level(hxc_l_tmp_load_result_n311, hxc_l_tmp_class_field_load_result_n313);
          hxc_l_gc_roots[109] = (const void *)hxc_l_tmp_instance_call_result_n314;
          struct hxc_caxecraft_content_CampaignLevel *hxc_l_destination_h9e8f76efb430 = hxc_l_tmp_instance_call_result_n314;
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
    bool hxc_l_tmp_call_result_n318 = hxc_caxecraft_app_AppScreen_showsCampaignSelection(hxc_l_screen);
    bool hxc_l_tmp_short_circuit_result_n181 = hxc_l_tmp_call_result_n318;
    if (hxc_l_tmp_call_result_n318)
    {
      hxc_l_tmp_short_circuit_result_n181 = hxc_l_focused;
    }
    if (hxc_l_tmp_short_circuit_result_n181)
    {
      bool hxc_l_campaignLaunchRequested = hxc_l_menuConfirmPressed_hf56cce049d76;
      bool hxc_l_campaignBackRequested = hxc_l_pausePressed_hc815dd822bf6;
      bool hxc_l_tmp_load_result_n323 = hxc_l_menuNextPressed_h9a6c5e9cc972;
      bool hxc_l_tmp_short_circuit_result_n184 = hxc_l_tmp_load_result_n323;
      if (hxc_l_tmp_load_result_n323)
      {
        hxc_l_gc_roots[113] = (const void *)hxc_l_campaign;
        hxc_l_gc_roots[114] = (const void *)NULL;
        hxc_l_tmp_short_circuit_result_n184 = hxc_l_campaign != NULL;
      }
      bool hxc_l_tmp_short_circuit_load_result_n325 = hxc_l_tmp_short_circuit_result_n184;
      bool hxc_l_tmp_short_circuit_result_n185 = hxc_l_tmp_short_circuit_load_result_n325;
      if (hxc_l_tmp_short_circuit_load_result_n325)
      {
        struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n326 = hxc_l_campaign;
        hxc_l_gc_roots[115] = (const void *)hxc_l_tmp_load_result_n326;
        if (hxc_l_tmp_load_result_n326 == NULL)
        {
          abort();
        }
        struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n327 = (*hxc_l_tmp_load_result_n326).hxc_admittedLevels;
        hxc_l_gc_roots[116] = (const void *)hxc_l_tmp_class_field_load_result_n327;
        int32_t hxc_l_tmp_array_length_result_n328;
        if (hxc_array_ref_length(hxc_l_tmp_class_field_load_result_n327, &hxc_l_tmp_array_length_result_n328) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_l_tmp_short_circuit_result_n185 = hxc_l_tmp_array_length_result_n328 > 0;
      }
      if (hxc_l_tmp_short_circuit_result_n185)
      {
        int32_t hxc_l_tmp_load_result_n330 = hxc_l_selectedCampaignLevelIndex;
        struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n331 = hxc_l_campaign;
        hxc_l_gc_roots[117] = (const void *)hxc_l_tmp_load_result_n331;
        if (hxc_l_tmp_load_result_n331 == NULL)
        {
          abort();
        }
        struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n332 = (*hxc_l_tmp_load_result_n331).hxc_admittedLevels;
        hxc_l_gc_roots[118] = (const void *)hxc_l_tmp_class_field_load_result_n332;
        int32_t hxc_l_tmp_array_length_result_n333;
        if (hxc_array_ref_length(hxc_l_tmp_class_field_load_result_n332, &hxc_l_tmp_array_length_result_n333) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_l_selectedCampaignLevelIndex = hxc_i32_modulo_zero_safe(hxc_i32_add_wrapping(hxc_l_tmp_load_result_n330, 1), hxc_l_tmp_array_length_result_n333);
      }
      bool hxc_l_tmp_native_call_result_n334 = IsKeyPressed((int32_t)76);
      if (hxc_l_tmp_native_call_result_n334)
      {
        struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_tmp_load_result_n335 = hxc_l_uiCatalog;
        hxc_l_gc_roots[119] = (const void *)hxc_l_tmp_load_result_n335;
        if (hxc_l_tmp_load_result_n335 == NULL)
        {
          abort();
        }
        int32_t hxc_l_tmp_instance_call_result_n337 = hxc_caxecraft_localization_RuntimeUiCatalog_nextLocale(hxc_l_tmp_load_result_n335, hxc_l_locale);
        hxc_l_locale = hxc_l_tmp_instance_call_result_n337;
      }
      struct Vector2 hxc_l_tmp_native_call_result_n338 = GetMousePosition();
      struct Vector2 hxc_l_campaignMouse = hxc_l_tmp_native_call_result_n338;
      double hxc_l_tmp_static_call_argument_0_n188 = (double)hxc_l_campaignMouse.x;
      double hxc_l_tmp_static_call_argument_1_n189 = (double)hxc_l_campaignMouse.y;
      int32_t hxc_l_tmp_native_call_result_n343 = GetScreenWidth();
      int32_t hxc_l_tmp_static_call_argument_2_n190 = (int32_t)hxc_l_tmp_native_call_result_n343;
      int32_t hxc_l_tmp_native_call_result_n344 = GetScreenHeight();
      int32_t hxc_l_tmp_static_call_argument_3_n191 = (int32_t)hxc_l_tmp_native_call_result_n344;
      struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n345 = hxc_l_campaign;
      hxc_l_gc_roots[120] = (const void *)hxc_l_tmp_load_result_n345;
      hxc_l_gc_roots[121] = (const void *)NULL;
      int32_t hxc_l_tmp_conditional_result_n192 = 0;
      if (hxc_l_tmp_load_result_n345 == NULL)
      {
        hxc_l_tmp_conditional_result_n192 = 0;
      }
      else
      {
        struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n346 = hxc_l_campaign;
        hxc_l_gc_roots[122] = (const void *)hxc_l_tmp_load_result_n346;
        if (hxc_l_tmp_load_result_n346 == NULL)
        {
          abort();
        }
        struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n347 = (*hxc_l_tmp_load_result_n346).hxc_admittedLevels;
        hxc_l_gc_roots[123] = (const void *)hxc_l_tmp_class_field_load_result_n347;
        int32_t hxc_l_tmp_array_length_result_n348;
        if (hxc_array_ref_length(hxc_l_tmp_class_field_load_result_n347, &hxc_l_tmp_array_length_result_n348) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_l_tmp_conditional_result_n192 = hxc_l_tmp_array_length_result_n348;
      }
      int32_t hxc_l_tmp_conditional_load_result_n349 = hxc_l_tmp_conditional_result_n192;
      double hxc_l_tmp_static_call_argument_0_load_result_n350 = hxc_l_tmp_static_call_argument_0_n188;
      double hxc_l_tmp_static_call_argument_1_load_result_n351 = hxc_l_tmp_static_call_argument_1_n189;
      int32_t hxc_l_tmp_static_call_argument_2_load_result_n352 = hxc_l_tmp_static_call_argument_2_n190;
      struct hxc_caxecraft_app_CampaignMenuHit hxc_l_tmp_call_result_n354 = hxc_caxecraft_app_CampaignMenu_selectionAt(hxc_l_tmp_static_call_argument_0_load_result_n350, hxc_l_tmp_static_call_argument_1_load_result_n351, hxc_l_tmp_static_call_argument_2_load_result_n352, hxc_l_tmp_static_call_argument_3_n191, hxc_l_tmp_conditional_load_result_n349);
      struct hxc_caxecraft_app_CampaignMenuHit hxc_l_campaignChoice = hxc_l_tmp_call_result_n354;
      bool hxc_l_tmp_native_call_result_n355 = IsMouseButtonPressed((int32_t)0);
      if (hxc_l_tmp_native_call_result_n355)
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
              int32_t hxc_l_tmp_enum_payload_project_n358 = hxc_l_campaignChoice.hxc_payload.hxc_LevelHit.hxc_index;
              int32_t hxc_l_index_h55b1c118d065 = hxc_l_tmp_enum_payload_project_n358;
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
        enum hxc_caxecraft_app_AppScreen hxc_l_tmp_call_result_n362 = hxc_caxecraft_app_AppScreen_closeCampaignSelection(hxc_l_screen);
        hxc_l_screen = hxc_l_tmp_call_result_n362;
        hxc_l_accumulator = 0.0;
        hxc_l_resetMotionThisFrame = true;
      }
      else
      {
        struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n363 = hxc_l_campaign;
        hxc_l_gc_roots[124] = (const void *)hxc_l_tmp_load_result_n363;
        hxc_l_gc_roots[125] = (const void *)NULL;
        bool hxc_l_tmp_short_circuit_result_n194 = hxc_l_tmp_load_result_n363 != NULL;
        if (hxc_l_tmp_load_result_n363 != NULL)
        {
          hxc_l_tmp_short_circuit_result_n194 = hxc_l_campaignLaunchRequested;
        }
        if (hxc_l_tmp_short_circuit_result_n194)
        {
          struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n366 = hxc_l_campaign;
          hxc_l_gc_roots[126] = (const void *)hxc_l_tmp_load_result_n366;
          if (hxc_l_tmp_load_result_n366 == NULL)
          {
            abort();
          }
          struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n367 = (*hxc_l_tmp_load_result_n366).hxc_admittedLevels;
          hxc_l_gc_roots[127] = (const void *)hxc_l_tmp_class_field_load_result_n367;
          struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_array_get_result_n369;
          if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n367, (size_t)hxc_l_selectedCampaignLevelIndex, &hxc_l_tmp_array_get_result_n369) != HXC_STATUS_OK)
          {
            abort();
          }
          hxc_l_gc_roots[128] = (const void *)hxc_l_tmp_array_get_result_n369;
          hxc_l_requestedCampaignLevel = hxc_l_tmp_array_get_result_n369;
          hxc_l_requestedFromCampaignMenu = true;
        }
      }
    }
    bool hxc_l_tmp_call_result_n371 = hxc_caxecraft_app_AppScreen_showsTitle(hxc_l_screen);
    bool hxc_l_tmp_short_circuit_result_n195 = hxc_l_tmp_call_result_n371;
    if (hxc_l_tmp_call_result_n371)
    {
      hxc_l_tmp_short_circuit_result_n195 = hxc_l_focused;
    }
    if (hxc_l_tmp_short_circuit_result_n195)
    {
      enum hxc_caxecraft_app_AppScreen hxc_l_screenBeforeMenu = hxc_l_screen;
      int32_t hxc_l_modeBeforeMenu = hxc_l_selectedMode;
      enum hxc_caxecraft_app_AppScreen hxc_l_tmp_load_result_n376 = hxc_l_screen;
      struct hxc_caxecraft_app_TitleMenuState hxc_l_tmp_call_result_n378 = hxc_caxecraft_app_TitleMenuFlow_titleMenuState(hxc_l_tmp_load_result_n376, hxc_l_selectedMode);
      struct hxc_caxecraft_app_TitleMenuState hxc_l_menuState = hxc_l_tmp_call_result_n378;
      bool hxc_l_tmp_native_call_result_n379 = IsKeyPressed((int32_t)76);
      if (hxc_l_tmp_native_call_result_n379)
      {
        struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_tmp_load_result_n380 = hxc_l_uiCatalog;
        hxc_l_gc_roots[132] = (const void *)hxc_l_tmp_load_result_n380;
        if (hxc_l_tmp_load_result_n380 == NULL)
        {
          abort();
        }
        int32_t hxc_l_tmp_instance_call_result_n382 = hxc_caxecraft_localization_RuntimeUiCatalog_nextLocale(hxc_l_tmp_load_result_n380, hxc_l_locale);
        hxc_l_locale = hxc_l_tmp_instance_call_result_n382;
      }
      if (hxc_l_menuNextPressed_h9a6c5e9cc972)
      {
        struct hxc_caxecraft_app_TitleMenuState hxc_l_tmp_call_result_n385 = hxc_caxecraft_app_TitleMenuFlow_applyTitleMenuCommand(hxc_l_menuState, 1);
        hxc_l_menuState = hxc_l_tmp_call_result_n385;
      }
      struct Vector2 hxc_l_tmp_native_call_result_n386 = GetMousePosition();
      struct Vector2 hxc_l_menuMouse = hxc_l_tmp_native_call_result_n386;
      struct Vector2 hxc_l_tmp_load_result_n387 = hxc_l_menuMouse;
      struct Vector2 hxc_l_tmp_load_result_n389 = hxc_l_menuMouse;
      int32_t hxc_l_tmp_native_call_result_n391 = GetScreenWidth();
      int32_t hxc_l_tmp_native_call_result_n392 = GetScreenHeight();
      int32_t hxc_l_tmp_call_result_n393 = hxc_caxecraft_app_TitleMenu_selectionAt((double)hxc_l_tmp_load_result_n387.x, (double)hxc_l_tmp_load_result_n389.y, (int32_t)hxc_l_tmp_native_call_result_n391, (int32_t)hxc_l_tmp_native_call_result_n392);
      int32_t hxc_l_hovered = hxc_l_tmp_call_result_n393;
      if (hxc_l_hovered == 0)
      {
        struct hxc_caxecraft_app_TitleMenuState hxc_l_tmp_call_result_n396 = hxc_caxecraft_app_TitleMenuFlow_applyTitleMenuCommand(hxc_l_menuState, 2);
        hxc_l_menuState = hxc_l_tmp_call_result_n396;
      }
      if (hxc_l_hovered == 1)
      {
        struct hxc_caxecraft_app_TitleMenuState hxc_l_tmp_call_result_n399 = hxc_caxecraft_app_TitleMenuFlow_applyTitleMenuCommand(hxc_l_menuState, 3);
        hxc_l_menuState = hxc_l_tmp_call_result_n399;
      }
      int32_t hxc_l_tmp_load_result_n400 = hxc_l_hovered;
      bool hxc_l_tmp_short_circuit_result_n202 = hxc_l_tmp_load_result_n400 >= 0;
      if (hxc_l_tmp_load_result_n400 >= 0)
      {
        bool hxc_l_tmp_native_call_result_n401 = IsMouseButtonPressed((int32_t)0);
        hxc_l_tmp_short_circuit_result_n202 = hxc_l_tmp_native_call_result_n401;
      }
      bool hxc_l_clickedChoice = hxc_l_tmp_short_circuit_result_n202;
      if (hxc_l_clickedChoice)
      {
        struct hxc_caxecraft_app_TitleMenuState hxc_l_tmp_static_call_argument_0_n203 = hxc_l_menuState;
        int32_t hxc_l_tmp_load_result_n405 = hxc_l_hovered;
        int32_t hxc_l_tmp_conditional_result_n204 = 0;
        if (hxc_l_tmp_load_result_n405 == 2)
        {
          hxc_l_tmp_conditional_result_n204 = 5;
        }
        else
        {
          hxc_l_tmp_conditional_result_n204 = 4;
        }
        int32_t hxc_l_tmp_conditional_load_result_n406 = hxc_l_tmp_conditional_result_n204;
        struct hxc_caxecraft_app_TitleMenuState hxc_l_tmp_call_result_n408 = hxc_caxecraft_app_TitleMenuFlow_applyTitleMenuCommand(hxc_l_tmp_static_call_argument_0_n203, hxc_l_tmp_conditional_load_result_n406);
        hxc_l_menuState = hxc_l_tmp_call_result_n408;
      }
      else
      {
        if (hxc_l_menuConfirmPressed_hf56cce049d76)
        {
          struct hxc_caxecraft_app_TitleMenuState hxc_l_tmp_call_result_n411 = hxc_caxecraft_app_TitleMenuFlow_applyTitleMenuCommand(hxc_l_menuState, 4);
          hxc_l_menuState = hxc_l_tmp_call_result_n411;
        }
      }
      hxc_l_screen = hxc_l_menuState.hxc_screen;
      hxc_l_selectedMode = hxc_l_menuState.hxc_mode;
      bool hxc_l_tmp_call_result_n415 = hxc_caxecraft_app_AppScreen_showsCampaignSelection(hxc_l_screen);
      bool hxc_l_tmp_short_circuit_result_n205 = hxc_l_tmp_call_result_n415;
      if (hxc_l_tmp_call_result_n415)
      {
        hxc_l_gc_roots[133] = (const void *)hxc_l_campaign;
        hxc_l_gc_roots[134] = (const void *)NULL;
        hxc_l_tmp_short_circuit_result_n205 = hxc_l_campaign == NULL;
      }
      if (hxc_l_tmp_short_circuit_result_n205)
      {
        hxc_l_screen = hxc_l_screenBeforeMenu;
      }
      enum hxc_caxecraft_app_AppScreen hxc_l_tmp_load_result_n419 = hxc_l_screen;
      enum hxc_caxecraft_app_AppScreen hxc_l_tmp_load_result_n420 = hxc_l_screenBeforeMenu;
      bool hxc_l_tmp_short_circuit_result_n206 = hxc_l_tmp_load_result_n419 != hxc_l_tmp_load_result_n420;
      if (!(hxc_l_tmp_load_result_n419 != hxc_l_tmp_load_result_n420))
      {
        int32_t hxc_l_tmp_load_result_n421 = hxc_l_selectedMode;
        hxc_l_tmp_short_circuit_result_n206 = hxc_l_tmp_load_result_n421 != hxc_l_modeBeforeMenu;
      }
      if (hxc_l_tmp_short_circuit_result_n206)
      {
        hxc_l_accumulator = 0.0;
        hxc_l_resetMotionThisFrame = true;
      }
      enum hxc_caxecraft_app_AppScreen hxc_l_tmp_load_result_n424 = hxc_l_screenBeforeMenu;
      bool hxc_l_tmp_short_circuit_result_n207 = hxc_l_tmp_load_result_n424 == hxc_caxecraft_app_AppScreen_Title;
      if (hxc_l_tmp_load_result_n424 == hxc_caxecraft_app_AppScreen_Title)
      {
        bool hxc_l_tmp_call_result_n427 = hxc_caxecraft_app_AppScreen_isPlaying(hxc_l_screen);
        hxc_l_tmp_short_circuit_result_n207 = hxc_l_tmp_call_result_n427;
      }
      if (hxc_l_tmp_short_circuit_result_n207)
      {
        hxc_l_recapturedThisFrame = true;
        DisableCursor();
      }
      else
      {
        bool hxc_l_tmp_call_result_n430 = hxc_caxecraft_app_AppScreen_showsEditor(hxc_l_screen);
        if (hxc_l_tmp_call_result_n430)
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
      int32_t hxc_l_tmp_record_field_load_result_n433 = hxc_l_character.hxc_id;
      int32_t hxc_l_tmp_record_field_load_result_n434 = hxc_l_character.hxc_vitals.hxc_health;
      struct hxc_caxecraft_content_LevelPlayerOptions hxc_l_playerOptions = (struct hxc_caxecraft_content_LevelPlayerOptions){ .hxc_aquaticProfile = hxc_l_character.hxc_aquaticProfile, .hxc_entityId = hxc_l_tmp_record_field_load_result_n433, .hxc_initialHealth = hxc_l_tmp_record_field_load_result_n434 };
      struct hxc_caxecraft_app_ActivePlayableLevel *hxc_l_tmp_load_result_n437 = hxc_l_activeLevel;
      hxc_l_gc_roots[135] = (const void *)hxc_l_tmp_load_result_n437;
      if (hxc_l_tmp_load_result_n437 == NULL)
      {
        abort();
      }
      struct hxc_caxecraft_content_ActiveContent *hxc_l_tmp_class_field_load_result_n438 = (*hxc_l_tmp_load_result_n437).hxc_content;
      hxc_l_gc_roots[136] = (const void *)hxc_l_tmp_class_field_load_result_n438;
      if (hxc_l_tmp_class_field_load_result_n438 == NULL)
      {
        abort();
      }
      struct hxc_caxecraft_content_LoadedContentGeneration *hxc_l_tmp_class_field_load_result_n439 = (*hxc_l_tmp_class_field_load_result_n438).hxc_active;
      hxc_l_gc_roots[137] = (const void *)hxc_l_tmp_class_field_load_result_n439;
      if (hxc_l_tmp_class_field_load_result_n439 == NULL)
      {
        abort();
      }
      int32_t hxc_l_tmp_class_field_load_result_n440 = (*hxc_l_tmp_class_field_load_result_n439).hxc_generationIdValue;
      int32_t hxc_l_nextGeneration = hxc_i32_add_wrapping(hxc_l_tmp_class_field_load_result_n440, 1);
      struct hxc_caxecraft_content_ContentPackageStore *hxc_l_tmp_load_result_n441 = hxc_l_contentStore;
      hxc_l_gc_roots[138] = (const void *)hxc_l_tmp_load_result_n441;
      struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_load_result_n442 = hxc_l_destination_h51235fba4206;
      hxc_l_gc_roots[139] = (const void *)hxc_l_tmp_load_result_n442;
      int32_t hxc_l_tmp_load_result_n443 = hxc_l_nextGeneration;
      struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_tmp_load_result_n444 = hxc_l_contentRegistry;
      hxc_l_gc_roots[140] = (const void *)hxc_l_tmp_load_result_n444;
      if (hxc_l_tmp_load_result_n444 == NULL)
      {
        abort();
      }
      struct hxc_compiler_interface_dispatch_caxecraft_scenario_ScenarioContentRegistry_value hxc_l_tmp_interface_value_n445 = (struct hxc_compiler_interface_dispatch_caxecraft_scenario_ScenarioContentRegistry_value){ .object = hxc_l_tmp_load_result_n444, .table = &hxc_itable_compiler_interface_dispatch_caxecraft_content_RuntimeContentRegistry_itable_layout_caxecraf_hb3f384641e1a61bf };
      struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_tmp_load_result_n446 = hxc_l_contentRegistry;
      hxc_l_gc_roots[141] = (const void *)hxc_l_tmp_load_result_n446;
      if (hxc_l_tmp_load_result_n446 == NULL)
      {
        abort();
      }
      struct hxc_compiler_interface_dispatch_caxecraft_content_LevelContentResolver_value hxc_l_tmp_interface_value_n447 = (struct hxc_compiler_interface_dispatch_caxecraft_content_LevelContentResolver_value){ .object = hxc_l_tmp_load_result_n446, .table = &hxc_itable_compiler_interface_dispatch_caxecraft_content_RuntimeContentRegistry_itable_layout_caxecraf_h82ca4f24ca7a7b5a };
      struct hxc_caxecraft_content_CampaignLevelLoadResult hxc_l_tmp_call_result_n449 = hxc_caxecraft_content_CampaignRuntime_loadCampaignLevel(hxc_l_tmp_load_result_n441, hxc_l_tmp_load_result_n442, hxc_l_tmp_load_result_n443, hxc_l_tmp_interface_value_n445, hxc_l_tmp_interface_value_n447, hxc_l_playerOptions);
      hxc_l_gc_roots[142] = hxc_l_tmp_call_result_n449.hxc_tag == hxc_caxecraft_content_CampaignLevelLoadResult_CampaignLevelReady ? (const void *)hxc_l_tmp_call_result_n449.hxc_payload.hxc_CampaignLevelReady.hxc_candidate : NULL;
      hxc_l_g_h3a886f21d5d2 = hxc_l_tmp_call_result_n449;
      hxc_l_gc_roots[143] = hxc_l_g_h3a886f21d5d2.hxc_tag == hxc_caxecraft_content_CampaignLevelLoadResult_CampaignLevelReady ? (const void *)hxc_l_g_h3a886f21d5d2.hxc_payload.hxc_CampaignLevelReady.hxc_candidate : NULL;
      switch (hxc_l_g_h3a886f21d5d2.hxc_tag) {
        case hxc_caxecraft_content_CampaignLevelLoadResult_CampaignLevelReady:
          {
            hxc_l_gc_roots[147] = hxc_l_g_h3a886f21d5d2.hxc_tag == hxc_caxecraft_content_CampaignLevelLoadResult_CampaignLevelReady ? (const void *)hxc_l_g_h3a886f21d5d2.hxc_payload.hxc_CampaignLevelReady.hxc_candidate : NULL;
            if (hxc_l_g_h3a886f21d5d2.hxc_tag != hxc_caxecraft_content_CampaignLevelLoadResult_CampaignLevelReady)
            {
              abort();
            }
            struct hxc_caxecraft_content_RuntimeLevelCandidate *hxc_l_tmp_enum_payload_project_n452 = hxc_l_g_h3a886f21d5d2.hxc_payload.hxc_CampaignLevelReady.hxc_candidate;
            hxc_l_gc_roots[148] = (const void *)hxc_l_tmp_enum_payload_project_n452;
            struct hxc_caxecraft_content_RuntimeLevelCandidate *hxc_l_candidate_h2a47547b7e00 = hxc_l_tmp_enum_payload_project_n452;
            struct hxc_caxecraft_app_ActivePlayableLevel *hxc_l_tmp_load_result_n453 = hxc_l_activeLevel;
            hxc_l_gc_roots[149] = (const void *)hxc_l_tmp_load_result_n453;
            if (hxc_l_tmp_load_result_n453 == NULL)
            {
              abort();
            }
            hxc_l_gc_roots[150] = (const void *)hxc_l_candidate_h2a47547b7e00;
            struct hxc_caxecraft_app_PlayableLevelPublicationResult hxc_l_tmp_instance_call_result_n455 = hxc_caxecraft_app_ActivePlayableLevel_publish(hxc_l_tmp_load_result_n453, hxc_l_candidate_h2a47547b7e00);
            struct hxc_caxecraft_app_PlayableLevelPublicationResult hxc_l_g_hc520dc5cd34a = hxc_l_tmp_instance_call_result_n455;
            switch (hxc_l_g_hc520dc5cd34a.hxc_tag) {
              case hxc_caxecraft_app_PlayableLevelPublicationResult_PlayableLevelPublished:
                {
                  if (hxc_l_g_hc520dc5cd34a.hxc_tag != hxc_caxecraft_app_PlayableLevelPublicationResult_PlayableLevelPublished)
                  {
                    abort();
                  }
                  int32_t hxc_l_tmp_enum_payload_project_n458 = hxc_l_g_hc520dc5cd34a.hxc_payload.hxc_PlayableLevelPublished.hxc_active;
                  int32_t hxc_l_selected = hxc_l_tmp_enum_payload_project_n458;
                  hxc_l_gc_roots[152] = (const void *)hxc_l_destination_h51235fba4206;
                  hxc_l_campaignLevel = hxc_l_destination_h51235fba4206;
                  struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_load_result_n460 = hxc_l_destination_h51235fba4206;
                  hxc_l_gc_roots[153] = (const void *)hxc_l_tmp_load_result_n460;
                  if (hxc_l_tmp_load_result_n460 == NULL)
                  {
                    abort();
                  }
                  hxc_string hxc_l_tmp_class_field_load_result_n461 = (*hxc_l_tmp_load_result_n460).hxc_id;
                  hxc_string hxc_l_tmp_string_assignment_replacement_owner_n215 = hxc_l_tmp_class_field_load_result_n461;
                  if (hxc_string_retain(hxc_l_tmp_string_assignment_replacement_owner_n215) != HXC_STATUS_OK)
                  {
                    abort();
                  }
                  hxc_string hxc_l_tmp_string_assignment_replacement_owned_load_result_n462 = hxc_l_tmp_string_assignment_replacement_owner_n215;
                  if (hxc_string_release(&hxc_l_levelLabel) != HXC_STATUS_OK)
                  {
                    abort();
                  }
                  hxc_l_levelLabel = hxc_l_tmp_string_assignment_replacement_owned_load_result_n462;
                  hxc_l_gc_roots[154] = (const void *)hxc_l_campaign;
                  struct hxc_caxecraft_content_CampaignManifest *hxc_l_selectedCampaign_h5d8509278d60 = hxc_l_campaign;
                  hxc_l_gc_roots[155] = (const void *)hxc_l_selectedCampaign_h5d8509278d60;
                  hxc_l_gc_roots[156] = (const void *)NULL;
                  if (hxc_l_selectedCampaign_h5d8509278d60 != NULL)
                  {
                    int32_t hxc_l_g_he2d328a22092 = 0;
                    struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n465 = hxc_l_selectedCampaign_h5d8509278d60;
                    hxc_l_gc_roots[157] = (const void *)hxc_l_tmp_load_result_n465;
                    if (hxc_l_tmp_load_result_n465 == NULL)
                    {
                      abort();
                    }
                    struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n466 = (*hxc_l_tmp_load_result_n465).hxc_admittedLevels;
                    hxc_l_gc_roots[158] = (const void *)hxc_l_tmp_class_field_load_result_n466;
                    int32_t hxc_l_tmp_array_length_result_n467;
                    if (hxc_array_ref_length(hxc_l_tmp_class_field_load_result_n466, &hxc_l_tmp_array_length_result_n467) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    int32_t hxc_l_g1_hfe83600c8bfe = hxc_l_tmp_array_length_result_n467;
                    while (1)
                    {
                      int32_t hxc_l_tmp_load_result_n468 = hxc_l_g_he2d328a22092;
                      if (!(hxc_l_tmp_load_result_n468 < hxc_l_g1_hfe83600c8bfe))
                      {
                        break;
                      }
                      int32_t hxc_l_tmp_increment_load_result_n470 = hxc_l_g_he2d328a22092;
                      hxc_l_g_he2d328a22092 = hxc_i32_add_wrapping(hxc_l_tmp_increment_load_result_n470, 1);
                      int32_t hxc_l_index_hd660d2316afa = hxc_l_tmp_increment_load_result_n470;
                      struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n471 = hxc_l_selectedCampaign_h5d8509278d60;
                      hxc_l_gc_roots[166] = (const void *)hxc_l_tmp_load_result_n471;
                      if (hxc_l_tmp_load_result_n471 == NULL)
                      {
                        abort();
                      }
                      struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n472 = (*hxc_l_tmp_load_result_n471).hxc_admittedLevels;
                      hxc_l_gc_roots[167] = (const void *)hxc_l_tmp_class_field_load_result_n472;
                      struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_array_get_result_n474;
                      if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n472, (size_t)hxc_l_index_hd660d2316afa, &hxc_l_tmp_array_get_result_n474) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      hxc_l_gc_roots[168] = (const void *)hxc_l_tmp_array_get_result_n474;
                      if (hxc_l_tmp_array_get_result_n474 == NULL)
                      {
                        abort();
                      }
                      hxc_string hxc_l_tmp_class_field_load_result_n475 = (*hxc_l_tmp_array_get_result_n474).hxc_id;
                      struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_load_result_n476 = hxc_l_destination_h51235fba4206;
                      hxc_l_gc_roots[169] = (const void *)hxc_l_tmp_load_result_n476;
                      if (hxc_l_tmp_load_result_n476 == NULL)
                      {
                        abort();
                      }
                      hxc_string hxc_l_tmp_class_field_load_result_n477 = (*hxc_l_tmp_load_result_n476).hxc_id;
                      if (hxc_l_tmp_class_field_load_result_n475.data == NULL || hxc_l_tmp_class_field_load_result_n477.data == NULL ? hxc_l_tmp_class_field_load_result_n475.data == hxc_l_tmp_class_field_load_result_n477.data : hxc_l_tmp_class_field_load_result_n475.byte_length == hxc_l_tmp_class_field_load_result_n477.byte_length && (hxc_l_tmp_class_field_load_result_n475.byte_length == 0 || memcmp(hxc_l_tmp_class_field_load_result_n475.data, hxc_l_tmp_class_field_load_result_n477.data, hxc_l_tmp_class_field_load_result_n475.byte_length) == 0))
                      {
                        hxc_l_selectedCampaignLevelIndex = hxc_l_index_hd660d2316afa;
                      }
                    }
                  }
                  struct hxc_caxecraft_app_ActivePlayableLevel *hxc_l_tmp_load_result_n480 = hxc_l_activeLevel;
                  hxc_l_gc_roots[159] = (const void *)hxc_l_tmp_load_result_n480;
                  if (hxc_l_tmp_load_result_n480 == NULL)
                  {
                    abort();
                  }
                  struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_class_field_load_result_n481 = (*hxc_l_tmp_load_result_n480).hxc_activeView;
                  hxc_l_gc_roots[160] = (const void *)hxc_l_tmp_class_field_load_result_n481;
                  hxc_l_levelView = hxc_l_tmp_class_field_load_result_n481;
                  struct hxc_caxecraft_app_ActivePlayableLevel *hxc_l_tmp_load_result_n482 = hxc_l_activeLevel;
                  hxc_l_gc_roots[161] = (const void *)hxc_l_tmp_load_result_n482;
                  if (hxc_l_tmp_load_result_n482 == NULL)
                  {
                    abort();
                  }
                  struct hxc_caxecraft_content_ActiveContent *hxc_l_tmp_class_field_load_result_n483 = (*hxc_l_tmp_load_result_n482).hxc_content;
                  hxc_l_gc_roots[162] = (const void *)hxc_l_tmp_class_field_load_result_n483;
                  if (hxc_l_tmp_class_field_load_result_n483 == NULL)
                  {
                    abort();
                  }
                  struct hxc_caxecraft_content_LoadedContentGeneration *hxc_l_tmp_class_field_load_result_n484 = (*hxc_l_tmp_class_field_load_result_n483).hxc_active;
                  hxc_l_gc_roots[163] = (const void *)hxc_l_tmp_class_field_load_result_n484;
                  if (hxc_l_tmp_class_field_load_result_n484 == NULL)
                  {
                    abort();
                  }
                  struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_class_field_load_result_n485 = (*hxc_l_tmp_class_field_load_result_n484).hxc_ownedSession;
                  hxc_l_gc_roots[164] = (const void *)hxc_l_tmp_class_field_load_result_n485;
                  hxc_l_session = hxc_l_tmp_class_field_load_result_n485;
                  struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n486 = hxc_l_session;
                  hxc_l_gc_roots[165] = (const void *)hxc_l_tmp_load_result_n486;
                  if (hxc_l_tmp_load_result_n486 == NULL)
                  {
                    abort();
                  }
                  struct hxc_caxecraft_domain_GameView hxc_l_tmp_instance_call_result_n487 = hxc_caxecraft_domain_GameSession_view(hxc_l_tmp_load_result_n486);
                  hxc_l_initialView = hxc_l_tmp_instance_call_result_n487;
                  if (!hxc_l_initialView.hxc_valid)
                  {
                    hxc_l_quit = true;
                  }
                  else
                  {
                    hxc_l_character = hxc_l_initialView.hxc_localPlayer;
                    hxc_string hxc_l_tmp_string_assignment_replacement_owner_n220 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
                    if (hxc_string_retain(hxc_l_tmp_string_assignment_replacement_owner_n220) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    hxc_string hxc_l_tmp_string_assignment_replacement_owned_load_result_n490 = hxc_l_tmp_string_assignment_replacement_owner_n220;
                    if (hxc_string_release(&hxc_l_activeDialogue) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    hxc_l_activeDialogue = hxc_l_tmp_string_assignment_replacement_owned_load_result_n490;
                    struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n491 = hxc_l_levelView;
                    hxc_l_gc_roots[170] = (const void *)hxc_l_tmp_load_result_n491;
                    if (hxc_l_tmp_load_result_n491 == NULL)
                    {
                      abort();
                    }
                    struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_class_field_load_result_n492 = (*hxc_l_tmp_load_result_n491).hxc_levelPresentation;
                    hxc_l_gc_roots[171] = (const void *)hxc_l_tmp_class_field_load_result_n492;
                    if (hxc_l_tmp_class_field_load_result_n492 == NULL)
                    {
                      abort();
                    }
                    hxc_string hxc_l_tmp_class_field_load_result_n493 = (*hxc_l_tmp_class_field_load_result_n492).hxc_startingObjective;
                    hxc_string hxc_l_tmp_string_assignment_replacement_owner_n221 = hxc_l_tmp_class_field_load_result_n493;
                    if (hxc_string_retain(hxc_l_tmp_string_assignment_replacement_owner_n221) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    hxc_string hxc_l_tmp_string_assignment_replacement_owned_load_result_n494 = hxc_l_tmp_string_assignment_replacement_owner_n221;
                    if (hxc_string_release(&hxc_l_currentObjectiveId) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    hxc_l_currentObjectiveId = hxc_l_tmp_string_assignment_replacement_owned_load_result_n494;
                    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n495 = hxc_l_session;
                    hxc_l_gc_roots[172] = (const void *)hxc_l_tmp_load_result_n495;
                    if (hxc_l_tmp_load_result_n495 == NULL)
                    {
                      abort();
                    }
                    struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n496 = hxc_l_levelView;
                    hxc_l_gc_roots[173] = (const void *)hxc_l_tmp_load_result_n496;
                    if (hxc_l_tmp_load_result_n496 == NULL)
                    {
                      abort();
                    }
                    int32_t hxc_l_tmp_class_field_load_result_n497 = (*hxc_l_tmp_load_result_n496).hxc_dialogueId;
                    bool hxc_l_tmp_instance_call_result_n498 = hxc_caxecraft_domain_GameSession_actorInteractionAvailable(hxc_l_tmp_load_result_n495, hxc_l_tmp_class_field_load_result_n497);
                    hxc_l_guideInteractionAvailable = hxc_l_tmp_instance_call_result_n498;
                    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n499 = hxc_l_session;
                    hxc_l_gc_roots[174] = (const void *)hxc_l_tmp_load_result_n499;
                    if (hxc_l_tmp_load_result_n499 == NULL)
                    {
                      abort();
                    }
                    struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n500 = hxc_l_levelView;
                    hxc_l_gc_roots[175] = (const void *)hxc_l_tmp_load_result_n500;
                    if (hxc_l_tmp_load_result_n500 == NULL)
                    {
                      abort();
                    }
                    int32_t hxc_l_tmp_class_field_load_result_n501 = (*hxc_l_tmp_load_result_n500).hxc_dialogueId;
                    struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n502 = hxc_caxecraft_domain_GameSession_readCharacter(hxc_l_tmp_load_result_n499, hxc_l_tmp_class_field_load_result_n501);
                    hxc_l_dialogueActor = hxc_l_tmp_instance_call_result_n502;
                    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n503 = hxc_l_session;
                    hxc_l_gc_roots[176] = (const void *)hxc_l_tmp_load_result_n503;
                    if (hxc_l_tmp_load_result_n503 == NULL)
                    {
                      abort();
                    }
                    struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n504 = hxc_l_levelView;
                    hxc_l_gc_roots[177] = (const void *)hxc_l_tmp_load_result_n504;
                    if (hxc_l_tmp_load_result_n504 == NULL)
                    {
                      abort();
                    }
                    int32_t hxc_l_tmp_class_field_load_result_n505 = (*hxc_l_tmp_load_result_n504).hxc_enemyId;
                    struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n506 = hxc_caxecraft_domain_GameSession_readCharacter(hxc_l_tmp_load_result_n503, hxc_l_tmp_class_field_load_result_n505);
                    hxc_l_enemyActor = hxc_l_tmp_instance_call_result_n506;
                    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n507 = hxc_l_session;
                    hxc_l_gc_roots[178] = (const void *)hxc_l_tmp_load_result_n507;
                    if (hxc_l_tmp_load_result_n507 == NULL)
                    {
                      abort();
                    }
                    struct hxc_array_ref *hxc_l_tmp_instance_call_result_n508 = hxc_caxecraft_domain_GameSession_actorControllerStateSnapshots(hxc_l_tmp_load_result_n507);
                    hxc_l_phases = hxc_l_tmp_instance_call_result_n508;
                    struct hxc_array_ref *hxc_l_tmp_load_result_n509 = hxc_l_phases;
                    struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n510 = hxc_l_levelView;
                    hxc_l_gc_roots[179] = (const void *)hxc_l_tmp_load_result_n510;
                    if (hxc_l_tmp_load_result_n510 == NULL)
                    {
                      abort();
                    }
                    int32_t hxc_l_tmp_class_field_load_result_n511 = (*hxc_l_tmp_load_result_n510).hxc_dialogueId;
                    struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation hxc_l_tmp_call_result_n513 = hxc_caxecraft_app_CaxecraftApp_observeActorPhase(hxc_l_tmp_load_result_n509, hxc_l_tmp_class_field_load_result_n511, hxc_caxecraft_domain_ActorControllerPhase_Stationary);
                    hxc_l_dialoguePhase = hxc_l_tmp_call_result_n513;
                    struct hxc_array_ref *hxc_l_tmp_load_result_n514 = hxc_l_phases;
                    struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n515 = hxc_l_levelView;
                    hxc_l_gc_roots[180] = (const void *)hxc_l_tmp_load_result_n515;
                    if (hxc_l_tmp_load_result_n515 == NULL)
                    {
                      abort();
                    }
                    int32_t hxc_l_tmp_class_field_load_result_n516 = (*hxc_l_tmp_load_result_n515).hxc_enemyId;
                    struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation hxc_l_tmp_call_result_n518 = hxc_caxecraft_app_CaxecraftApp_observeActorPhase(hxc_l_tmp_load_result_n514, hxc_l_tmp_class_field_load_result_n516, hxc_caxecraft_domain_ActorControllerPhase_Resting);
                    hxc_l_enemyPhase_h70382f3bdc66 = hxc_l_tmp_call_result_n518;
                    hxc_l_swordCombat = (struct hxc_caxecraft_gameplay_SwordCombatState){ .hxc_cooldownTicks = 0 };
                    hxc_l_berryDrop = (struct hxc_caxecraft_gameplay_BerryDropState){ .hxc_active = false, .hxc_amount = 0, .hxc_x = 0.0, .hxc_y = 0.0, .hxc_z = 0.0 };
                    hxc_l_cameraWaterBlend = 0.0;
                    hxc_l_accumulator = 0.0;
                    struct hxc_caxecraft_app_MotionHistory hxc_l_tmp_call_result_n522 = hxc_caxecraft_app_MotionInterpolation_start(hxc_l_character.hxc_body);
                    hxc_l_motionHistory = hxc_l_tmp_call_result_n522;
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
                    struct hxc_caxecraft_app_TerrainRenderer *hxc_l_tmp_owned_class_field_address_n523 = &(*hxc_l_self).hxc_terrainRenderer;
                    struct hxc_caxecraft_app_TerrainChunkCache *hxc_l_tmp_owned_class_field_address_n524 = &(*hxc_l_tmp_owned_class_field_address_n523).hxc_cache;
                    if (hxc_l_tmp_owned_class_field_address_n524 == NULL)
                    {
                      abort();
                    }
                    hxc_caxecraft_app_TerrainChunkCache_invalidateAll(hxc_l_tmp_owned_class_field_address_n524);
                    hxc_l_resetMotionThisFrame = true;
                    if (hxc_l_requestedFromCampaignMenu)
                    {
                      enum hxc_caxecraft_app_AppScreen hxc_l_tmp_call_result_n527 = hxc_caxecraft_app_AppScreen_startSelectedCampaign(hxc_l_screen);
                      hxc_l_screen = hxc_l_tmp_call_result_n527;
                      hxc_l_recapturedThisFrame = true;
                      DisableCursor();
                    }
                    hxc_string hxc_l_tmp_string_concat_result_n529 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
                    if (hxc_string_concat_ref((hxc_string){ (const uint8_t *)"caxecraft: campaign-level=", 26, true, NULL }, hxc_l_levelLabel, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n529) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    hxc_l_tmp_string_concat_left_owner_n223 = hxc_l_tmp_string_concat_result_n529;
                    hxc_string hxc_l_tmp_string_concat_result_n531 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
                    if (hxc_string_concat_ref(hxc_l_tmp_string_concat_left_owner_n223, (hxc_string){ (const uint8_t *)" generation=", 12, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n531) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    hxc_l_tmp_string_concat_left_owner_n224 = hxc_l_tmp_string_concat_result_n531;
                    hxc_string hxc_l_tmp_string_concat_left_borrow_result_n532 = hxc_l_tmp_string_concat_left_owner_n224;
                    hxc_string hxc_l_tmp_std_string_int_result_n534 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
                    if (hxc_string_from_int32(hxc_l_selected, hxc_default_allocator(), &hxc_l_tmp_std_string_int_result_n534) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    hxc_l_tmp_string_concat_right_owner_n225 = hxc_l_tmp_std_string_int_result_n534;
                    hxc_string hxc_l_tmp_string_concat_result_n536 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
                    if (hxc_string_concat_ref(hxc_l_tmp_string_concat_left_borrow_result_n532, hxc_l_tmp_string_concat_right_owner_n225, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n536) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    hxc_l_tmp_sys_println_string_argument_owner_n226 = hxc_l_tmp_string_concat_result_n536;
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
                      if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n224) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n223) != HXC_STATUS_OK)
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
                      if (hxc_string_release(&hxc_l_activeDialogue) != HXC_STATUS_OK)
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
                    if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n224) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n223) != HXC_STATUS_OK)
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
            struct hxc_caxecraft_content_CampaignLevelLoadError hxc_l_tmp_enum_payload_project_n539 = hxc_l_g_h3a886f21d5d2.hxc_payload.hxc_CampaignLevelRejected.hxc_error;
            hxc_l_error = hxc_l_tmp_enum_payload_project_n539;
            if (hxc_enum_40863044_retain(&hxc_l_error) != HXC_STATUS_OK)
            {
              abort();
            }
            hxc_string hxc_l_tmp_call_result_n541 = hxc_caxecraft_app_CaxecraftApp_campaignLevelLoadFailure(hxc_l_error);
            hxc_l_tmp_string_concat_right_owner_n228 = hxc_l_tmp_call_result_n541;
            hxc_string hxc_l_tmp_string_concat_result_n543 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
            if (hxc_string_concat_ref((hxc_string){ (const uint8_t *)"caxecraft: campaign level rejected: ", 36, true, NULL }, hxc_l_tmp_string_concat_right_owner_n228, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n543) != HXC_STATUS_OK)
            {
              abort();
            }
            hxc_l_tmp_sys_println_string_argument_owner_n229 = hxc_l_tmp_string_concat_result_n543;
            if (hxc_io_println(hxc_l_tmp_sys_println_string_argument_owner_n229) != HXC_STATUS_OK)
            {
              if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n229) != HXC_STATUS_OK)
              {
                abort();
              }
              if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n228) != HXC_STATUS_OK)
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
              if (hxc_string_release(&hxc_l_activeDialogue) != HXC_STATUS_OK)
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
            if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n229) != HXC_STATUS_OK)
            {
              abort();
            }
            if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n228) != HXC_STATUS_OK)
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
    struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n545 = hxc_l_levelView;
    hxc_l_gc_roots[144] = (const void *)hxc_l_tmp_load_result_n545;
    if (hxc_l_tmp_load_result_n545 == NULL)
    {
      abort();
    }
    int32_t hxc_l_tmp_class_field_load_result_n546 = (*hxc_l_tmp_load_result_n545).hxc_dialogueId;
    int32_t hxc_l_dialogueActorId = hxc_l_tmp_class_field_load_result_n546;
    struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n547 = hxc_l_levelView;
    hxc_l_gc_roots[145] = (const void *)hxc_l_tmp_load_result_n547;
    if (hxc_l_tmp_load_result_n547 == NULL)
    {
      abort();
    }
    int32_t hxc_l_tmp_class_field_load_result_n548 = (*hxc_l_tmp_load_result_n547).hxc_enemyId;
    int32_t hxc_l_enemyActorId = hxc_l_tmp_class_field_load_result_n548;
    struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n549 = hxc_l_levelView;
    hxc_l_gc_roots[146] = (const void *)hxc_l_tmp_load_result_n549;
    if (hxc_l_tmp_load_result_n549 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_scenario_ScenarioTransform hxc_l_tmp_class_field_load_result_n550 = (*hxc_l_tmp_load_result_n549).hxc_playerSpawn;
    struct hxc_caxecraft_scenario_ScenarioTransform hxc_l_spawnTransform = hxc_l_tmp_class_field_load_result_n550;
    if (hxc_l_quitPressed_h735cfb7844ac)
    {
      hxc_l_quit = true;
    }
    if (hxc_l_hotbarSelection_h2e5159304f3b >= 0)
    {
      struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_load_result_n553 = hxc_l_inventory;
      struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n555 = hxc_caxecraft_gameplay_Inventory_select(hxc_l_tmp_load_result_n553, hxc_l_hotbarSelection_h2e5159304f3b);
      hxc_l_inventory = hxc_l_tmp_call_result_n555;
    }
    if (hxc_l_hotbarCycle_h45973208a5fd != 0)
    {
      struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_load_result_n557 = hxc_l_inventory;
      struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n559 = hxc_caxecraft_gameplay_Inventory_cycle(hxc_l_tmp_load_result_n557, hxc_l_hotbarCycle_h45973208a5fd);
      hxc_l_inventory = hxc_l_tmp_call_result_n559;
    }
    bool hxc_l_tmp_call_result_n561 = hxc_caxecraft_app_AppScreen_isPlaying(hxc_l_screen);
    bool hxc_l_tmp_short_circuit_result_n233 = hxc_l_tmp_call_result_n561;
    if (hxc_l_tmp_call_result_n561)
    {
      hxc_l_tmp_short_circuit_result_n233 = hxc_l_interactPressed_hca6e4535614d;
    }
    if (hxc_l_tmp_short_circuit_result_n233)
    {
      if (hxc_l_character.hxc_vitals.hxc_health <= 0)
      {
        struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n565 = hxc_l_session;
        hxc_l_gc_roots[181] = (const void *)hxc_l_tmp_load_result_n565;
        if (hxc_l_tmp_load_result_n565 == NULL)
        {
          abort();
        }
        struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n566 = hxc_l_session;
        hxc_l_gc_roots[182] = (const void *)hxc_l_tmp_load_result_n566;
        if (hxc_l_tmp_load_result_n566 == NULL)
        {
          abort();
        }
        size_t hxc_l_tmp_length_n568;
        const uint8_t *hxc_l_tmp_instance_call_result_n567 = hxc_caxecraft_domain_GameSession_worldView(hxc_l_tmp_load_result_n566, &hxc_l_tmp_length_n568);
        struct hxc_caxecraft_domain_CharacterBody hxc_l_tmp_call_result_n570 = hxc_caxecraft_app_CaxecraftApp_spawnPlayer(hxc_l_tmp_instance_call_result_n567, hxc_l_tmp_length_n568, hxc_l_spawnTransform);
        struct hxc_caxecraft_domain_LocalCharacterCommandResult hxc_l_tmp_instance_call_result_n571 = hxc_caxecraft_domain_GameSession_reviveLocalPlayerAt(hxc_l_tmp_load_result_n565, hxc_l_tmp_call_result_n570);
        struct hxc_caxecraft_domain_LocalCharacterCommandResult hxc_l_revival = hxc_l_tmp_instance_call_result_n571;
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
        if (hxc_l_activeDialogue.data != NULL)
        {
          hxc_string hxc_l_tmp_string_assignment_replacement_owner_n235 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
          if (hxc_string_retain(hxc_l_tmp_string_assignment_replacement_owner_n235) != HXC_STATUS_OK)
          {
            abort();
          }
          hxc_string hxc_l_tmp_string_assignment_replacement_owned_load_result_n575 = hxc_l_tmp_string_assignment_replacement_owner_n235;
          if (hxc_string_release(&hxc_l_activeDialogue) != HXC_STATUS_OK)
          {
            abort();
          }
          hxc_l_activeDialogue = hxc_l_tmp_string_assignment_replacement_owned_load_result_n575;
        }
        else
        {
          struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n576 = hxc_l_session;
          hxc_l_gc_roots[183] = (const void *)hxc_l_tmp_load_result_n576;
          if (hxc_l_tmp_load_result_n576 == NULL)
          {
            abort();
          }
          bool hxc_l_tmp_instance_call_result_n578 = hxc_caxecraft_domain_GameSession_actorInteractionAvailable(hxc_l_tmp_load_result_n576, hxc_l_dialogueActorId);
          bool hxc_l_tmp_short_circuit_result_n236 = hxc_l_tmp_instance_call_result_n578;
          if (hxc_l_tmp_instance_call_result_n578)
          {
            struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n579 = hxc_l_session;
            hxc_l_gc_roots[184] = (const void *)hxc_l_tmp_load_result_n579;
            if (hxc_l_tmp_load_result_n579 == NULL)
            {
              abort();
            }
            bool hxc_l_tmp_instance_call_result_n581 = hxc_caxecraft_domain_GameSession_interactWithActor(hxc_l_tmp_load_result_n579, hxc_l_dialogueActorId);
            hxc_l_tmp_short_circuit_result_n236 = !hxc_l_tmp_instance_call_result_n581;
          }
          if (hxc_l_tmp_short_circuit_result_n236)
          {
            hxc_l_quit = true;
          }
        }
      }
    }
    bool hxc_l_tmp_load_result_n583 = hxc_l_focused;
    bool hxc_l_tmp_short_circuit_result_n237 = !hxc_l_tmp_load_result_n583;
    if (!hxc_l_tmp_load_result_n583)
    {
      bool hxc_l_tmp_call_result_n585 = hxc_caxecraft_app_AppScreen_isPlaying(hxc_l_screen);
      hxc_l_tmp_short_circuit_result_n237 = hxc_l_tmp_call_result_n585;
    }
    if (hxc_l_tmp_short_circuit_result_n237)
    {
      enum hxc_caxecraft_app_AppScreen hxc_l_tmp_call_result_n588 = hxc_caxecraft_app_AppScreen_loseFocus(hxc_l_screen);
      hxc_l_screen = hxc_l_tmp_call_result_n588;
      hxc_l_jumpQueued = false;
      hxc_l_accumulator = 0.0;
      hxc_l_resetMotionThisFrame = true;
      EnableCursor();
    }
    bool hxc_l_tmp_call_result_n590 = hxc_caxecraft_app_AppScreen_showsTitle(hxc_l_screen);
    bool hxc_l_tmp_short_circuit_result_n238 = !hxc_l_tmp_call_result_n590;
    if (!hxc_l_tmp_call_result_n590)
    {
      bool hxc_l_tmp_call_result_n592 = hxc_caxecraft_app_AppScreen_showsCampaignSelection(hxc_l_screen);
      hxc_l_tmp_short_circuit_result_n238 = !hxc_l_tmp_call_result_n592;
    }
    bool hxc_l_tmp_short_circuit_load_result_n593 = hxc_l_tmp_short_circuit_result_n238;
    bool hxc_l_tmp_short_circuit_result_n239 = hxc_l_tmp_short_circuit_load_result_n593;
    if (hxc_l_tmp_short_circuit_load_result_n593)
    {
      bool hxc_l_tmp_call_result_n595 = hxc_caxecraft_app_AppScreen_showsEditor(hxc_l_screen);
      hxc_l_tmp_short_circuit_result_n239 = !hxc_l_tmp_call_result_n595;
    }
    bool hxc_l_tmp_short_circuit_load_result_n596 = hxc_l_tmp_short_circuit_result_n239;
    bool hxc_l_tmp_short_circuit_result_n240 = hxc_l_tmp_short_circuit_load_result_n596;
    if (hxc_l_tmp_short_circuit_load_result_n596)
    {
      hxc_l_tmp_short_circuit_result_n240 = hxc_l_focused;
    }
    bool hxc_l_tmp_short_circuit_load_result_n598 = hxc_l_tmp_short_circuit_result_n240;
    bool hxc_l_tmp_short_circuit_result_n241 = hxc_l_tmp_short_circuit_load_result_n598;
    if (hxc_l_tmp_short_circuit_load_result_n598)
    {
      hxc_l_tmp_short_circuit_result_n241 = hxc_l_pausePressed_hc815dd822bf6;
    }
    if (hxc_l_tmp_short_circuit_result_n241)
    {
      enum hxc_caxecraft_app_AppScreen hxc_l_tmp_call_result_n602 = hxc_caxecraft_app_AppScreen_togglePause(hxc_l_screen);
      hxc_l_screen = hxc_l_tmp_call_result_n602;
      hxc_l_accumulator = 0.0;
      hxc_l_resetMotionThisFrame = true;
      bool hxc_l_tmp_call_result_n604 = hxc_caxecraft_app_AppScreen_pausesSimulation(hxc_l_screen);
      if (hxc_l_tmp_call_result_n604)
      {
        hxc_l_jumpQueued = false;
      }
      bool hxc_l_tmp_call_result_n606 = hxc_caxecraft_app_AppScreen_capturesPointer(hxc_l_screen);
      if (hxc_l_tmp_call_result_n606)
      {
        DisableCursor();
      }
      else
      {
        EnableCursor();
      }
    }
    bool hxc_l_tmp_call_result_n608 = hxc_caxecraft_app_AppScreen_showsTitle(hxc_l_screen);
    bool hxc_l_tmp_short_circuit_result_n242 = !hxc_l_tmp_call_result_n608;
    if (!hxc_l_tmp_call_result_n608)
    {
      bool hxc_l_tmp_call_result_n610 = hxc_caxecraft_app_AppScreen_showsCampaignSelection(hxc_l_screen);
      hxc_l_tmp_short_circuit_result_n242 = !hxc_l_tmp_call_result_n610;
    }
    bool hxc_l_tmp_short_circuit_load_result_n611 = hxc_l_tmp_short_circuit_result_n242;
    bool hxc_l_tmp_short_circuit_result_n243 = hxc_l_tmp_short_circuit_load_result_n611;
    if (hxc_l_tmp_short_circuit_load_result_n611)
    {
      bool hxc_l_tmp_call_result_n613 = hxc_caxecraft_app_AppScreen_showsEditor(hxc_l_screen);
      hxc_l_tmp_short_circuit_result_n243 = !hxc_l_tmp_call_result_n613;
    }
    bool hxc_l_tmp_short_circuit_load_result_n614 = hxc_l_tmp_short_circuit_result_n243;
    bool hxc_l_tmp_short_circuit_result_n244 = hxc_l_tmp_short_circuit_load_result_n614;
    if (hxc_l_tmp_short_circuit_load_result_n614)
    {
      hxc_l_tmp_short_circuit_result_n244 = hxc_l_focused;
    }
    bool hxc_l_tmp_short_circuit_load_result_n616 = hxc_l_tmp_short_circuit_result_n244;
    bool hxc_l_tmp_short_circuit_result_n245 = hxc_l_tmp_short_circuit_load_result_n616;
    if (hxc_l_tmp_short_circuit_load_result_n616)
    {
      bool hxc_l_tmp_call_result_n618 = hxc_caxecraft_app_AppScreen_pausesSimulation(hxc_l_screen);
      hxc_l_tmp_short_circuit_result_n245 = hxc_l_tmp_call_result_n618;
    }
    bool hxc_l_tmp_short_circuit_load_result_n619 = hxc_l_tmp_short_circuit_result_n245;
    bool hxc_l_tmp_short_circuit_result_n246 = hxc_l_tmp_short_circuit_load_result_n619;
    if (hxc_l_tmp_short_circuit_load_result_n619)
    {
      hxc_l_tmp_short_circuit_result_n246 = hxc_l_capturePressed;
    }
    if (hxc_l_tmp_short_circuit_result_n246)
    {
      enum hxc_caxecraft_app_AppScreen hxc_l_tmp_call_result_n623 = hxc_caxecraft_app_AppScreen_recapture(hxc_l_screen);
      hxc_l_screen = hxc_l_tmp_call_result_n623;
      hxc_l_recapturedThisFrame = true;
      hxc_l_accumulator = 0.0;
      hxc_l_resetMotionThisFrame = true;
      DisableCursor();
    }
    bool hxc_l_tmp_call_result_n625 = hxc_caxecraft_app_AppScreen_showsTitle(hxc_l_screen);
    bool hxc_l_onTitle = hxc_l_tmp_call_result_n625;
    bool hxc_l_tmp_call_result_n627 = hxc_caxecraft_app_AppScreen_showsCampaignSelection(hxc_l_screen);
    bool hxc_l_onCampaignSelect = hxc_l_tmp_call_result_n627;
    bool hxc_l_tmp_call_result_n629 = hxc_caxecraft_app_AppScreen_showsEditor(hxc_l_screen);
    bool hxc_l_onEditor = hxc_l_tmp_call_result_n629;
    bool hxc_l_tmp_call_result_n631 = hxc_caxecraft_app_AppScreen_pausesSimulation(hxc_l_screen);
    bool hxc_l_paused_hfac44b03fb01 = hxc_l_tmp_call_result_n631;
    bool hxc_l_tmp_call_result_n633 = hxc_caxecraft_app_AppScreen_capturesPointer(hxc_l_screen);
    bool hxc_l_captured_h8194fa734f34 = hxc_l_tmp_call_result_n633;
    int32_t hxc_l_editorNavigationCommand = 0;
    if (hxc_l_onEditor)
    {
      if (hxc_l_self == NULL)
      {
        abort();
      }
      struct hxc_caxecraft_input_NavigationRepeater *hxc_l_tmp_owned_class_field_address_n635 = &(*hxc_l_self).hxc_editorNavigation;
      if (hxc_l_tmp_owned_class_field_address_n635 == NULL)
      {
        abort();
      }
      struct hxc_caxecraft_input_NavigationSample hxc_l_tmp_call_result_n636 = hxc_caxecraft_app_RaylibNavigationInput_samplePrimaryGamepad();
      int32_t hxc_l_tmp_instance_call_result_n638 = hxc_caxecraft_input_NavigationRepeater_advance(hxc_l_tmp_owned_class_field_address_n635, hxc_l_tmp_call_result_n636, hxc_l_frameSeconds);
      hxc_l_editorNavigationCommand = hxc_l_tmp_instance_call_result_n638;
    }
    else
    {
      if (hxc_l_self == NULL)
      {
        abort();
      }
      struct hxc_caxecraft_input_NavigationRepeater *hxc_l_tmp_owned_class_field_address_n639 = &(*hxc_l_self).hxc_editorNavigation;
      if (hxc_l_tmp_owned_class_field_address_n639 == NULL)
      {
        abort();
      }
      hxc_caxecraft_input_NavigationRepeater_release(hxc_l_tmp_owned_class_field_address_n639);
    }
    if (hxc_l_captured_h8194fa734f34)
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
      double hxc_l_tmp_load_result_n644 = hxc_l_lookX;
      double hxc_l_tmp_load_result_n645 = hxc_l_yawDelta;
      double hxc_l_candidateX = hxc_l_tmp_load_result_n644 + hxc_l_tmp_load_result_n645 * hxc_l_lookZ;
      double hxc_l_tmp_load_result_n647 = hxc_l_lookZ;
      double hxc_l_tmp_load_result_n648 = hxc_l_yawDelta;
      double hxc_l_candidateZ = hxc_l_tmp_load_result_n647 - hxc_l_tmp_load_result_n648 * hxc_l_lookX;
      double hxc_l_tmp_load_result_n650 = hxc_l_candidateX;
      double hxc_l_tmp_load_result_n651 = hxc_l_candidateX;
      double hxc_l_tmp_load_result_n652 = hxc_l_candidateZ;
      double hxc_l_lengthSquared = hxc_l_tmp_load_result_n650 * hxc_l_tmp_load_result_n651 + hxc_l_tmp_load_result_n652 * hxc_l_candidateZ;
      double hxc_l_normalize = 1.5 - 0.5 * hxc_l_lengthSquared;
      double hxc_l_tmp_load_result_n655 = hxc_l_candidateX;
      hxc_l_lookX = hxc_l_tmp_load_result_n655 * hxc_l_normalize;
      double hxc_l_tmp_load_result_n657 = hxc_l_candidateZ;
      hxc_l_lookZ = hxc_l_tmp_load_result_n657 * hxc_l_normalize;
      double hxc_l_tmp_compound_load_result_n659 = hxc_l_lookY;
      hxc_l_lookY = hxc_l_tmp_compound_load_result_n659 + hxc_l_lookPitch_hf286d80d7193;
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
    bool hxc_l_tmp_load_result_n664 = hxc_l_captured_h8194fa734f34;
    bool hxc_l_tmp_short_circuit_result_n258 = hxc_l_tmp_load_result_n664;
    if (hxc_l_tmp_load_result_n664)
    {
      hxc_l_tmp_short_circuit_result_n258 = !hxc_l_recapturedThisFrame;
    }
    bool hxc_l_tmp_short_circuit_load_result_n666 = hxc_l_tmp_short_circuit_result_n258;
    bool hxc_l_tmp_short_circuit_result_n259 = hxc_l_tmp_short_circuit_load_result_n666;
    if (hxc_l_tmp_short_circuit_load_result_n666)
    {
      hxc_l_tmp_short_circuit_result_n259 = hxc_l_primaryPressed_h86125a319e35;
    }
    bool hxc_l_tmp_short_circuit_load_result_n668 = hxc_l_tmp_short_circuit_result_n259;
    bool hxc_l_tmp_short_circuit_result_n260 = hxc_l_tmp_short_circuit_load_result_n668;
    if (hxc_l_tmp_short_circuit_load_result_n668)
    {
      hxc_l_tmp_short_circuit_result_n260 = hxc_l_selectedMode == 1;
    }
    bool hxc_l_tmp_short_circuit_load_result_n670 = hxc_l_tmp_short_circuit_result_n260;
    bool hxc_l_tmp_short_circuit_result_n261 = hxc_l_tmp_short_circuit_load_result_n670;
    if (hxc_l_tmp_short_circuit_load_result_n670)
    {
      bool hxc_l_tmp_call_result_n673 = hxc_caxecraft_gameplay_Inventory_selectedIs(hxc_l_inventory, hxc_caxecraft_gameplay_ItemKind_CopperSword);
      hxc_l_tmp_short_circuit_result_n261 = hxc_l_tmp_call_result_n673;
    }
    if (hxc_l_tmp_short_circuit_result_n261)
    {
      hxc_l_swordQueued = true;
    }
    if (!hxc_l_paused_hfac44b03fb01)
    {
      double hxc_l_tmp_compound_load_result_n676 = hxc_l_accumulator;
      hxc_l_accumulator = hxc_l_tmp_compound_load_result_n676 + hxc_l_frameSeconds;
    }
    while (1)
    {
      bool hxc_l_tmp_load_result_n678 = hxc_l_paused_hfac44b03fb01;
      bool hxc_l_tmp_short_circuit_result_n262 = !hxc_l_tmp_load_result_n678;
      if (!hxc_l_tmp_load_result_n678)
      {
        hxc_l_tmp_short_circuit_result_n262 = hxc_l_accumulator >= 0.05;
      }
      if (!hxc_l_tmp_short_circuit_result_n262)
      {
        break;
      }
      double hxc_l_tmp_load_result_n681 = hxc_l_moveForward;
      double hxc_l_tmp_load_result_n682 = hxc_l_lookX;
      double hxc_l_tmp_load_result_n683 = hxc_l_moveRight;
      double hxc_l_moveX = hxc_l_tmp_load_result_n681 * hxc_l_tmp_load_result_n682 - hxc_l_tmp_load_result_n683 * hxc_l_lookZ;
      double hxc_l_tmp_load_result_n685 = hxc_l_moveForward;
      double hxc_l_tmp_load_result_n686 = hxc_l_lookZ;
      double hxc_l_tmp_load_result_n687 = hxc_l_moveRight;
      double hxc_l_moveZ = hxc_l_tmp_load_result_n685 * hxc_l_tmp_load_result_n686 + hxc_l_tmp_load_result_n687 * hxc_l_lookX;
      double hxc_l_tmp_load_result_n689 = hxc_l_moveForward;
      bool hxc_l_tmp_short_circuit_result_n265 = hxc_l_tmp_load_result_n689 != 0.0;
      if (hxc_l_tmp_load_result_n689 != 0.0)
      {
        hxc_l_tmp_short_circuit_result_n265 = hxc_l_moveRight != 0.0;
      }
      if (hxc_l_tmp_short_circuit_result_n265)
      {
        hxc_l_moveX = hxc_l_moveX * 0.7071067811865476;
        hxc_l_moveZ = hxc_l_moveZ * 0.7071067811865476;
      }
      int32_t hxc_l_tmp_load_result_n694 = hxc_l_selectedMode;
      int32_t hxc_l_tmp_conditional_result_n267 = 0;
      if (hxc_l_tmp_load_result_n694 == 1)
      {
        hxc_l_tmp_conditional_result_n267 = 1;
      }
      else
      {
        hxc_l_tmp_conditional_result_n267 = 0;
      }
      int32_t hxc_l_damagePolicy = hxc_l_tmp_conditional_result_n267;
      bool hxc_l_tmp_load_result_n696 = hxc_l_jumpQueued;
      bool hxc_l_tmp_short_circuit_result_n269 = hxc_l_tmp_load_result_n696;
      if (!hxc_l_tmp_load_result_n696)
      {
        int32_t hxc_l_tmp_record_field_load_result_n697 = hxc_l_character.hxc_aquatic.hxc_medium;
        bool hxc_l_tmp_short_circuit_result_n270 = hxc_l_tmp_record_field_load_result_n697 != 0;
        if (hxc_l_tmp_record_field_load_result_n697 != 0)
        {
          hxc_l_tmp_short_circuit_result_n270 = hxc_l_riseHeld;
        }
        hxc_l_tmp_short_circuit_result_n269 = hxc_l_tmp_short_circuit_result_n270;
      }
      bool hxc_l_rising = hxc_l_tmp_short_circuit_result_n269;
      struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n701 = hxc_l_session;
      hxc_l_gc_roots[186] = (const void *)hxc_l_tmp_load_result_n701;
      if (hxc_l_tmp_load_result_n701 == NULL)
      {
        abort();
      }
      double hxc_l_tmp_load_result_n702 = hxc_l_moveX;
      double hxc_l_tmp_load_result_n703 = hxc_l_moveZ;
      bool hxc_l_tmp_load_result_n704 = hxc_l_rising;
      struct hxc_caxecraft_domain_CharacterIntent hxc_l_tmp_call_result_n706 = hxc_caxecraft_domain_Aquatics_input(hxc_l_tmp_load_result_n702, hxc_l_tmp_load_result_n703, hxc_l_tmp_load_result_n704, hxc_l_descendHeld_h2072257a4c60);
      struct hxc_caxecraft_domain_GameTickResult hxc_l_tmp_instance_call_result_n709 = hxc_caxecraft_domain_GameSession_tick(hxc_l_tmp_load_result_n701, (struct hxc_caxecraft_domain_GameTickInput){ .hxc_damagePolicy = hxc_l_damagePolicy, .hxc_intent = hxc_l_tmp_call_result_n706, .hxc_waterUpdateBudget = 64 });
      hxc_l_gameTick = hxc_l_tmp_instance_call_result_n709;
      hxc_l_character = hxc_l_gameTick.hxc_character;
      hxc_l_flow = hxc_l_gameTick.hxc_flow;
      if (hxc_optional_b8d3265c_retain(&hxc_l_flow) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_l_flow.hxc_has_value)
      {
        struct hxc_optional_caxecraft_scenario_FlowTickResult hxc_l_tmp_load_result_n713 = hxc_l_flow;
        if (!hxc_l_tmp_load_result_n713.hxc_has_value)
        {
          abort();
        }
        int32_t hxc_l_tmp_array_length_result_n716;
        if (hxc_array_ref_length(hxc_l_tmp_load_result_n713.hxc_value.hxc_diagnostics, &hxc_l_tmp_array_length_result_n716) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_l_tmp_array_length_result_n716 == 0)
        {
          int32_t hxc_l_g_hca219d456021 = 0;
          struct hxc_optional_caxecraft_scenario_FlowTickResult hxc_l_tmp_load_result_n717 = hxc_l_flow;
          if (!hxc_l_tmp_load_result_n717.hxc_has_value)
          {
            abort();
          }
          hxc_l_g1_h24ec6a7c61fb = hxc_l_tmp_load_result_n717.hxc_value.hxc_presentation;
          if (hxc_array_ref_retain(hxc_l_g1_h24ec6a7c61fb) != HXC_STATUS_OK)
          {
            abort();
          }
          while (1)
          {
            int32_t hxc_l_tmp_load_result_n720 = hxc_l_g_hca219d456021;
            int32_t hxc_l_tmp_array_length_result_n722;
            if (hxc_array_ref_length(hxc_l_g1_h24ec6a7c61fb, &hxc_l_tmp_array_length_result_n722) != HXC_STATUS_OK)
            {
              abort();
            }
            if (!(hxc_l_tmp_load_result_n720 < hxc_l_tmp_array_length_result_n722))
            {
              break;
            }
            struct hxc_array_ref *hxc_l_tmp_load_result_n723 = hxc_l_g1_h24ec6a7c61fb;
            struct hxc_caxecraft_scenario_FlowPresentationEvent hxc_l_tmp_array_get_result_n725;
            if (hxc_array_ref_get_copy(hxc_l_tmp_load_result_n723, (size_t)hxc_l_g_hca219d456021, &hxc_l_tmp_array_get_result_n725) != HXC_STATUS_OK)
            {
              abort();
            }
            hxc_l_tmp_array_element_owner_n276 = hxc_l_tmp_array_get_result_n725;
            hxc_l_event_h613c6941bfa8 = hxc_l_tmp_array_element_owner_n276;
            if (hxc_enum_147eda7c_retain(&hxc_l_event_h613c6941bfa8) != HXC_STATUS_OK)
            {
              abort();
            }
            hxc_l_g_hca219d456021 = hxc_i32_add_wrapping(hxc_l_g_hca219d456021, 1);
            if (hxc_l_event_h613c6941bfa8.hxc_tag == hxc_caxecraft_scenario_FlowPresentationEvent_DialogueRequested)
            {
              if (hxc_l_event_h613c6941bfa8.hxc_tag != hxc_caxecraft_scenario_FlowPresentationEvent_DialogueRequested)
              {
                abort();
              }
              hxc_string hxc_l_tmp_enum_payload_project_n730 = hxc_l_event_h613c6941bfa8.hxc_payload.hxc_DialogueRequested.hxc_id;
              hxc_l_id = hxc_l_tmp_enum_payload_project_n730;
              if (hxc_string_retain(hxc_l_id) != HXC_STATUS_OK)
              {
                abort();
              }
              hxc_string hxc_l_tmp_string_assignment_replacement_owner_n278 = hxc_l_id;
              if (hxc_string_retain(hxc_l_tmp_string_assignment_replacement_owner_n278) != HXC_STATUS_OK)
              {
                abort();
              }
              hxc_string hxc_l_tmp_string_assignment_replacement_owned_load_result_n732 = hxc_l_tmp_string_assignment_replacement_owner_n278;
              if (hxc_string_release(&hxc_l_activeDialogue) != HXC_STATUS_OK)
              {
                abort();
              }
              hxc_l_activeDialogue = hxc_l_tmp_string_assignment_replacement_owned_load_result_n732;
              if (hxc_string_release(&hxc_l_id) != HXC_STATUS_OK)
              {
                abort();
              }
            }
            hxc_enum_147eda7c_destroy(&hxc_l_event_h613c6941bfa8);
            hxc_array_ce94fc36_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n276);
          }
          struct hxc_optional_caxecraft_scenario_FlowTickResult hxc_l_tmp_load_result_n733 = hxc_l_flow;
          if (!hxc_l_tmp_load_result_n733.hxc_has_value)
          {
            abort();
          }
          hxc_string hxc_l_tmp_string_assignment_replacement_owner_n279 = hxc_l_tmp_load_result_n733.hxc_value.hxc_activeObjective;
          if (hxc_string_retain(hxc_l_tmp_string_assignment_replacement_owner_n279) != HXC_STATUS_OK)
          {
            abort();
          }
          hxc_string hxc_l_tmp_string_assignment_replacement_owned_load_result_n736 = hxc_l_tmp_string_assignment_replacement_owner_n279;
          if (hxc_string_release(&hxc_l_currentObjectiveId) != HXC_STATUS_OK)
          {
            abort();
          }
          hxc_l_currentObjectiveId = hxc_l_tmp_string_assignment_replacement_owned_load_result_n736;
          if (hxc_array_ref_release(hxc_l_g1_h24ec6a7c61fb) != HXC_STATUS_OK)
          {
            abort();
          }
        }
      }
      if (hxc_l_gameTick.hxc_committed)
      {
        struct hxc_caxecraft_app_MotionHistory hxc_l_tmp_load_result_n738 = hxc_l_motionHistory;
        struct hxc_caxecraft_app_MotionHistory hxc_l_tmp_call_result_n740 = hxc_caxecraft_app_MotionInterpolation_advance(hxc_l_tmp_load_result_n738, hxc_l_character.hxc_body);
        hxc_l_motionHistory = hxc_l_tmp_call_result_n740;
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
          int32_t hxc_l_tmp_load_result_n744 = hxc_l_pickupIndex;
          struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n745 = hxc_l_levelView;
          hxc_l_gc_roots[187] = (const void *)hxc_l_tmp_load_result_n745;
          if (hxc_l_tmp_load_result_n745 == NULL)
          {
            abort();
          }
          struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n746 = (*hxc_l_tmp_load_result_n745).hxc_items;
          int32_t hxc_l_tmp_array_length_result_n747;
          if (hxc_array_ref_length(hxc_l_tmp_class_field_load_result_n746, &hxc_l_tmp_array_length_result_n747) != HXC_STATUS_OK)
          {
            abort();
          }
          if (!(hxc_l_tmp_load_result_n744 < hxc_l_tmp_array_length_result_n747))
          {
            break;
          }
          struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n748 = hxc_l_levelView;
          hxc_l_gc_roots[188] = (const void *)hxc_l_tmp_load_result_n748;
          if (hxc_l_tmp_load_result_n748 == NULL)
          {
            abort();
          }
          struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n749 = (*hxc_l_tmp_load_result_n748).hxc_items;
          struct hxc_caxecraft_app_LoadedWorldItem hxc_l_tmp_array_get_result_n751;
          if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n749, (size_t)hxc_l_pickupIndex, &hxc_l_tmp_array_get_result_n751) != HXC_STATUS_OK)
          {
            abort();
          }
          struct hxc_caxecraft_app_LoadedWorldItem hxc_l_loadedItem = hxc_l_tmp_array_get_result_n751;
          struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n752 = hxc_l_session;
          hxc_l_gc_roots[189] = (const void *)hxc_l_tmp_load_result_n752;
          if (hxc_l_tmp_load_result_n752 == NULL)
          {
            abort();
          }
          bool hxc_l_tmp_instance_call_result_n754 = hxc_caxecraft_domain_GameSession_authoredItemIsActive(hxc_l_tmp_load_result_n752, hxc_l_pickupIndex);
          bool hxc_l_tmp_short_circuit_result_n282 = hxc_l_tmp_instance_call_result_n754;
          if (hxc_l_tmp_instance_call_result_n754)
          {
            double hxc_l_tmp_record_field_load_result_n755 = hxc_l_character.hxc_body.hxc_x;
            double hxc_l_tmp_record_field_load_result_n756 = hxc_l_character.hxc_body.hxc_y;
            double hxc_l_tmp_record_field_load_result_n757 = hxc_l_character.hxc_body.hxc_z;
            int32_t hxc_l_tmp_record_field_load_result_n758 = hxc_l_loadedItem.hxc_xMilli;
            int32_t hxc_l_tmp_record_field_load_result_n759 = hxc_l_loadedItem.hxc_yMilli;
            bool hxc_l_tmp_call_result_n761 = hxc_caxecraft_gameplay_WorldItemPickup_isInRange(hxc_l_tmp_record_field_load_result_n755, hxc_l_tmp_record_field_load_result_n756, hxc_l_tmp_record_field_load_result_n757, hxc_l_tmp_record_field_load_result_n758, hxc_l_tmp_record_field_load_result_n759, hxc_l_loadedItem.hxc_zMilli);
            hxc_l_tmp_short_circuit_result_n282 = hxc_l_tmp_call_result_n761;
          }
          if (hxc_l_tmp_short_circuit_result_n282)
          {
            int32_t hxc_l_itemCode = hxc_l_loadedItem.hxc_storageCode;
            struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_tmp_load_result_n764 = hxc_l_contentRegistry;
            hxc_l_gc_roots[190] = (const void *)hxc_l_tmp_load_result_n764;
            if (hxc_l_tmp_load_result_n764 == NULL)
            {
              abort();
            }
            enum hxc_caxecraft_content_RuntimeItemUseProfile hxc_l_tmp_instance_call_result_n766 = hxc_caxecraft_content_RuntimeContentRegistry_itemUseProfile(hxc_l_tmp_load_result_n764, hxc_l_itemCode);
            bool hxc_l_tmp_short_circuit_result_n284 = hxc_l_tmp_instance_call_result_n766 == hxc_caxecraft_content_RuntimeItemUseProfile_EquipAquatic;
            if (hxc_l_tmp_instance_call_result_n766 == hxc_caxecraft_content_RuntimeItemUseProfile_EquipAquatic)
            {
              struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_tmp_load_result_n768 = hxc_l_contentRegistry;
              hxc_l_gc_roots[191] = (const void *)hxc_l_tmp_load_result_n768;
              if (hxc_l_tmp_load_result_n768 == NULL)
              {
                abort();
              }
              bool hxc_l_tmp_instance_call_result_n770 = hxc_caxecraft_content_RuntimeContentRegistry_itemProvidesAquaticProfile(hxc_l_tmp_load_result_n768, hxc_l_itemCode);
              hxc_l_tmp_short_circuit_result_n284 = hxc_l_tmp_instance_call_result_n770;
            }
            if (hxc_l_tmp_short_circuit_result_n284)
            {
              struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_tmp_load_result_n772 = hxc_l_contentRegistry;
              hxc_l_gc_roots[192] = (const void *)hxc_l_tmp_load_result_n772;
              if (hxc_l_tmp_load_result_n772 == NULL)
              {
                abort();
              }
              struct hxc_caxecraft_domain_AquaticProfile hxc_l_tmp_instance_call_result_n774 = hxc_caxecraft_content_RuntimeContentRegistry_itemAquaticProfile(hxc_l_tmp_load_result_n772, hxc_l_itemCode);
              struct hxc_caxecraft_domain_AquaticProfile hxc_l_replacement = hxc_l_tmp_instance_call_result_n774;
              struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n775 = hxc_l_session;
              hxc_l_gc_roots[193] = (const void *)hxc_l_tmp_load_result_n775;
              if (hxc_l_tmp_load_result_n775 == NULL)
              {
                abort();
              }
              int32_t hxc_l_tmp_load_result_n776 = hxc_l_pickupIndex;
              struct hxc_caxecraft_domain_AuthoredAquaticEquipmentResult hxc_l_tmp_instance_call_result_n778 = hxc_caxecraft_domain_GameSession_collectAuthoredAquaticEquipment(hxc_l_tmp_load_result_n775, hxc_l_tmp_load_result_n776, hxc_l_replacement);
              struct hxc_caxecraft_domain_AuthoredAquaticEquipmentResult hxc_l_equipment = hxc_l_tmp_instance_call_result_n778;
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
              struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_tmp_load_result_n783 = hxc_l_contentRegistry;
              hxc_l_gc_roots[194] = (const void *)hxc_l_tmp_load_result_n783;
              struct hxc_optional_caxecraft_gameplay_ItemKind hxc_l_tmp_call_result_n785 = hxc_caxecraft_app_RuntimeInventoryBinding_inventoryKindForRuntimeItem(hxc_l_tmp_load_result_n783, hxc_l_itemCode);
              struct hxc_optional_caxecraft_gameplay_ItemKind hxc_l_inventoryKind = hxc_l_tmp_call_result_n785;
              if (hxc_l_inventoryKind.hxc_has_value)
              {
                struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n787 = hxc_l_session;
                hxc_l_gc_roots[195] = (const void *)hxc_l_tmp_load_result_n787;
                if (hxc_l_tmp_load_result_n787 == NULL)
                {
                  abort();
                }
                int32_t hxc_l_tmp_load_result_n788 = hxc_l_pickupIndex;
                struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_load_result_n789 = hxc_l_inventory;
                struct hxc_optional_caxecraft_gameplay_ItemKind hxc_l_tmp_load_result_n790 = hxc_l_inventoryKind;
                if (!hxc_l_tmp_load_result_n790.hxc_has_value)
                {
                  abort();
                }
                struct hxc_caxecraft_domain_AuthoredInventoryItemResult hxc_l_tmp_instance_call_result_n793 = hxc_caxecraft_domain_GameSession_collectAuthoredInventoryItem(hxc_l_tmp_load_result_n787, hxc_l_tmp_load_result_n788, hxc_l_tmp_load_result_n789, hxc_l_tmp_load_result_n790.hxc_value, hxc_l_loadedItem.hxc_quantity);
                struct hxc_caxecraft_domain_AuthoredInventoryItemResult hxc_l_pickup = hxc_l_tmp_instance_call_result_n793;
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
          struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n799 = hxc_l_session;
          hxc_l_gc_roots[196] = (const void *)hxc_l_tmp_load_result_n799;
          if (hxc_l_tmp_load_result_n799 == NULL)
          {
            abort();
          }
          int32_t hxc_l_tmp_record_field_load_result_n800 = hxc_l_gameTick.hxc_tickIndex;
          struct hxc_caxecraft_domain_ActorControllerTickResult hxc_l_tmp_instance_call_result_n802 = hxc_caxecraft_domain_GameSession_stepAuthoredActorControllers(hxc_l_tmp_load_result_n799, hxc_l_tmp_record_field_load_result_n800, hxc_l_damagePolicy);
          struct hxc_caxecraft_domain_ActorControllerTickResult hxc_l_actorTick = hxc_l_tmp_instance_call_result_n802;
          switch (hxc_l_actorTick.hxc_status.hxc_tag) {
            case hxc_caxecraft_domain_ActorControllerTickStatus_ControllersAdvanced:
              {
                hxc_l_guideInteractionAvailable = false;
                int32_t hxc_l_g_h09ddfce788ab = 0;
                struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n804 = hxc_l_session;
                hxc_l_gc_roots[200] = (const void *)hxc_l_tmp_load_result_n804;
                if (hxc_l_tmp_load_result_n804 == NULL)
                {
                  abort();
                }
                struct hxc_array_ref *hxc_l_tmp_instance_call_result_n805 = hxc_caxecraft_domain_GameSession_actorControllerEventSnapshots(hxc_l_tmp_load_result_n804);
                hxc_l_g1_h86761ddd48e2 = hxc_l_tmp_instance_call_result_n805;
                while (1)
                {
                  int32_t hxc_l_tmp_load_result_n806 = hxc_l_g_h09ddfce788ab;
                  int32_t hxc_l_tmp_array_length_result_n808;
                  if (hxc_array_ref_length(hxc_l_g1_h86761ddd48e2, &hxc_l_tmp_array_length_result_n808) != HXC_STATUS_OK)
                  {
                    abort();
                  }
                  if (!(hxc_l_tmp_load_result_n806 < hxc_l_tmp_array_length_result_n808))
                  {
                    break;
                  }
                  struct hxc_array_ref *hxc_l_tmp_load_result_n809 = hxc_l_g1_h86761ddd48e2;
                  struct hxc_caxecraft_domain_ActorControllerEvent hxc_l_tmp_array_get_result_n811;
                  if (hxc_array_ref_get_copy(hxc_l_tmp_load_result_n809, (size_t)hxc_l_g_h09ddfce788ab, &hxc_l_tmp_array_get_result_n811) != HXC_STATUS_OK)
                  {
                    abort();
                  }
                  hxc_l_tmp_array_element_owner_n293 = hxc_l_tmp_array_get_result_n811;
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
                        int32_t hxc_l_tmp_enum_payload_project_n816 = hxc_l_event_h6e7c030f6037.hxc_payload.hxc_InteractionAvailable.hxc_characterId;
                        int32_t hxc_l_source_hbd6ac0610c33 = hxc_l_tmp_enum_payload_project_n816;
                        int32_t hxc_l_tmp_load_result_n817 = hxc_l_source_hbd6ac0610c33;
                        if (hxc_l_tmp_load_result_n817 == hxc_l_dialogueActorId)
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
                        int32_t hxc_l_tmp_enum_payload_project_n820 = hxc_l_event_h6e7c030f6037.hxc_payload.hxc_LocalPlayerAttack.hxc_characterId;
                        int32_t hxc_l_source_hd06ffa06cddf = hxc_l_tmp_enum_payload_project_n820;
                        int32_t hxc_l_tmp_load_result_n821 = hxc_l_source_hd06ffa06cddf;
                        if (hxc_l_tmp_load_result_n821 == hxc_l_enemyActorId)
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
                        int32_t hxc_l_tmp_enum_payload_project_n824 = hxc_l_event_h6e7c030f6037.hxc_payload.hxc_DropRequested.hxc_characterId;
                        int32_t hxc_l_source_h2f060cc986d3 = hxc_l_tmp_enum_payload_project_n824;
                        if (hxc_l_event_h6e7c030f6037.hxc_tag != hxc_caxecraft_domain_ActorControllerEvent_DropRequested)
                        {
                          abort();
                        }
                        hxc_string hxc_l_tmp_enum_payload_project_n826 = hxc_l_event_h6e7c030f6037.hxc_payload.hxc_DropRequested.hxc_drop;
                        hxc_l_drop = hxc_l_tmp_enum_payload_project_n826;
                        if (hxc_string_retain(hxc_l_drop) != HXC_STATUS_OK)
                        {
                          abort();
                        }
                        struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_tmp_load_result_n827 = hxc_l_contentRegistry;
                        hxc_l_gc_roots[201] = (const void *)hxc_l_tmp_load_result_n827;
                        if (hxc_l_tmp_load_result_n827 == NULL)
                        {
                          abort();
                        }
                        int32_t hxc_l_tmp_instance_call_result_n829 = hxc_caxecraft_content_RuntimeContentRegistry_dropQuantityById(hxc_l_tmp_load_result_n827, hxc_l_drop);
                        int32_t hxc_l_quantity = hxc_l_tmp_instance_call_result_n829;
                        struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n830 = hxc_l_session;
                        hxc_l_gc_roots[202] = (const void *)hxc_l_tmp_load_result_n830;
                        if (hxc_l_tmp_load_result_n830 == NULL)
                        {
                          abort();
                        }
                        struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n832 = hxc_caxecraft_domain_GameSession_readCharacter(hxc_l_tmp_load_result_n830, hxc_l_source_h2f060cc986d3);
                        struct hxc_caxecraft_domain_Character hxc_l_defeatedActor = hxc_l_tmp_instance_call_result_n832;
                        int32_t hxc_l_tmp_load_result_n833 = hxc_l_quantity;
                        bool hxc_l_tmp_short_circuit_result_n300 = hxc_l_tmp_load_result_n833 <= 0;
                        if (!(hxc_l_tmp_load_result_n833 <= 0))
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
                            struct hxc_caxecraft_domain_Character hxc_l_tmp_load_result_n837 = hxc_l_defeatedActor;
                            struct hxc_caxecraft_gameplay_BerryDropState hxc_l_tmp_call_result_n839 = hxc_caxecraft_gameplay_BerryDrop_fromDefeatedCharacter(hxc_l_tmp_load_result_n837, hxc_l_quantity);
                            hxc_l_berryDrop = hxc_l_tmp_call_result_n839;
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
        struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n840 = hxc_l_session;
        hxc_l_gc_roots[197] = (const void *)hxc_l_tmp_load_result_n840;
        if (hxc_l_tmp_load_result_n840 == NULL)
        {
          abort();
        }
        struct hxc_array_ref *hxc_l_tmp_instance_call_result_n841 = hxc_caxecraft_domain_GameSession_actorControllerStateSnapshots(hxc_l_tmp_load_result_n840);
        hxc_l_actorPhases = hxc_l_tmp_instance_call_result_n841;
        struct hxc_array_ref *hxc_l_tmp_load_result_n842 = hxc_l_actorPhases;
        struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation hxc_l_tmp_call_result_n845 = hxc_caxecraft_app_CaxecraftApp_observeActorPhase(hxc_l_tmp_load_result_n842, hxc_l_dialogueActorId, hxc_caxecraft_domain_ActorControllerPhase_Stationary);
        hxc_l_dialoguePhase = hxc_l_tmp_call_result_n845;
        struct hxc_array_ref *hxc_l_tmp_load_result_n846 = hxc_l_actorPhases;
        struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation hxc_l_tmp_call_result_n849 = hxc_caxecraft_app_CaxecraftApp_observeActorPhase(hxc_l_tmp_load_result_n846, hxc_l_enemyActorId, hxc_caxecraft_domain_ActorControllerPhase_Defeated);
        hxc_l_enemyPhase_h70382f3bdc66 = hxc_l_tmp_call_result_n849;
        struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n850 = hxc_l_session;
        hxc_l_gc_roots[198] = (const void *)hxc_l_tmp_load_result_n850;
        if (hxc_l_tmp_load_result_n850 == NULL)
        {
          abort();
        }
        struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n852 = hxc_caxecraft_domain_GameSession_readCharacter(hxc_l_tmp_load_result_n850, hxc_l_dialogueActorId);
        hxc_l_dialogueActor = hxc_l_tmp_instance_call_result_n852;
        struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n853 = hxc_l_session;
        hxc_l_gc_roots[199] = (const void *)hxc_l_tmp_load_result_n853;
        if (hxc_l_tmp_load_result_n853 == NULL)
        {
          abort();
        }
        struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n855 = hxc_caxecraft_domain_GameSession_readCharacter(hxc_l_tmp_load_result_n853, hxc_l_enemyActorId);
        hxc_l_enemyActor = hxc_l_tmp_instance_call_result_n855;
        bool hxc_l_tmp_record_field_load_result_n856 = hxc_l_dialoguePhase.hxc_valid;
        bool hxc_l_tmp_short_circuit_result_n302 = !hxc_l_tmp_record_field_load_result_n856;
        if (!!hxc_l_tmp_record_field_load_result_n856)
        {
          hxc_l_tmp_short_circuit_result_n302 = !hxc_l_enemyPhase_h70382f3bdc66.hxc_valid;
        }
        bool hxc_l_tmp_short_circuit_load_result_n858 = hxc_l_tmp_short_circuit_result_n302;
        bool hxc_l_tmp_short_circuit_result_n303 = hxc_l_tmp_short_circuit_load_result_n858;
        if (!hxc_l_tmp_short_circuit_load_result_n858)
        {
          hxc_l_tmp_short_circuit_result_n303 = hxc_l_dialogueActor.hxc_id <= 0;
        }
        bool hxc_l_tmp_short_circuit_load_result_n860 = hxc_l_tmp_short_circuit_result_n303;
        bool hxc_l_tmp_short_circuit_result_n304 = hxc_l_tmp_short_circuit_load_result_n860;
        if (!hxc_l_tmp_short_circuit_load_result_n860)
        {
          hxc_l_tmp_short_circuit_result_n304 = hxc_l_enemyActor.hxc_id <= 0;
        }
        if (hxc_l_tmp_short_circuit_result_n304)
        {
          hxc_l_quit = true;
        }
        struct hxc_caxecraft_gameplay_SwordCombatState hxc_l_tmp_call_result_n864 = hxc_caxecraft_gameplay_SwordCombat_step(hxc_l_swordCombat);
        hxc_l_swordCombat = hxc_l_tmp_call_result_n864;
        if (hxc_l_swordQueued)
        {
          struct hxc_caxecraft_gameplay_SwordCombatState hxc_l_tmp_load_result_n866 = hxc_l_swordCombat;
          struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_load_result_n867 = hxc_l_inventory;
          struct hxc_caxecraft_domain_VitalsState hxc_l_tmp_record_field_load_result_n868 = hxc_l_character.hxc_vitals;
          struct hxc_caxecraft_domain_Character hxc_l_tmp_load_result_n869 = hxc_l_enemyActor;
          double hxc_l_tmp_record_field_load_result_n870 = hxc_l_character.hxc_body.hxc_x;
          double hxc_l_tmp_record_field_load_result_n871 = hxc_l_character.hxc_body.hxc_z;
          double hxc_l_tmp_load_result_n872 = hxc_l_lookX;
          int32_t hxc_l_tmp_call_result_n874 = hxc_caxecraft_gameplay_SwordCombat_decide(hxc_l_tmp_load_result_n866, hxc_l_tmp_load_result_n867, hxc_l_tmp_record_field_load_result_n868, hxc_l_tmp_load_result_n869, hxc_l_tmp_record_field_load_result_n870, hxc_l_tmp_record_field_load_result_n871, hxc_l_tmp_load_result_n872, hxc_l_lookZ);
          int32_t hxc_l_swordDecision = hxc_l_tmp_call_result_n874;
          if (hxc_l_swordDecision == 5)
          {
            struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n876 = hxc_l_session;
            hxc_l_gc_roots[203] = (const void *)hxc_l_tmp_load_result_n876;
            if (hxc_l_tmp_load_result_n876 == NULL)
            {
              abort();
            }
            struct hxc_caxecraft_domain_CharacterDamageResult hxc_l_tmp_instance_call_result_n878 = hxc_caxecraft_domain_GameSession_damageCharacter(hxc_l_tmp_load_result_n876, hxc_l_enemyActorId, 1);
            struct hxc_caxecraft_domain_CharacterDamageResult hxc_l_damage = hxc_l_tmp_instance_call_result_n878;
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
          int32_t hxc_l_tmp_load_result_n882 = hxc_l_swordDecision;
          struct hxc_caxecraft_gameplay_SwordCombatState hxc_l_tmp_call_result_n884 = hxc_caxecraft_gameplay_SwordCombat_after(hxc_l_tmp_load_result_n882, hxc_l_swordCombat);
          hxc_l_swordCombat = hxc_l_tmp_call_result_n884;
          hxc_l_swordQueued = false;
        }
        if (hxc_array_ref_release(hxc_l_actorPhases) != HXC_STATUS_OK)
        {
          abort();
        }
      }
      hxc_l_jumpQueued = false;
      hxc_l_accumulator = hxc_l_accumulator - 0.05;
      hxc_optional_b8d3265c_destroy(&hxc_l_flow);
      hxc_record_9ab6a4ce_destroy(&hxc_l_gameTick);
    }
    double hxc_l_selectionEyeX = hxc_l_character.hxc_body.hxc_x;
    double hxc_l_selectionEyeY = hxc_l_character.hxc_body.hxc_y + 1.62;
    double hxc_l_selectionEyeZ = hxc_l_character.hxc_body.hxc_z;
    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n889 = hxc_l_session;
    hxc_l_gc_roots[185] = (const void *)hxc_l_tmp_load_result_n889;
    if (hxc_l_tmp_load_result_n889 == NULL)
    {
      abort();
    }
    size_t hxc_l_tmp_length_n891;
    const uint8_t *hxc_l_tmp_instance_call_result_n890 = hxc_caxecraft_domain_GameSession_worldView(hxc_l_tmp_load_result_n889, &hxc_l_tmp_length_n891);
    double hxc_l_tmp_load_result_n892 = hxc_l_selectionEyeX;
    double hxc_l_tmp_load_result_n893 = hxc_l_selectionEyeY;
    double hxc_l_tmp_load_result_n894 = hxc_l_selectionEyeZ;
    double hxc_l_tmp_load_result_n895 = hxc_l_lookX;
    double hxc_l_tmp_load_result_n896 = hxc_l_lookY;
    struct hxc_caxecraft_domain_RaycastHit hxc_l_tmp_call_result_n898 = hxc_caxecraft_domain_VoxelRaycast_trace(hxc_l_tmp_instance_call_result_n890, hxc_l_tmp_length_n891, hxc_l_tmp_load_result_n892, hxc_l_tmp_load_result_n893, hxc_l_tmp_load_result_n894, hxc_l_tmp_load_result_n895, hxc_l_tmp_load_result_n896, hxc_l_lookZ, 7.0);
    struct hxc_caxecraft_domain_RaycastHit hxc_l_hit = hxc_l_tmp_call_result_n898;
    bool hxc_l_tmp_load_result_n899 = hxc_l_captured_h8194fa734f34;
    bool hxc_l_tmp_short_circuit_result_n311 = hxc_l_tmp_load_result_n899;
    if (hxc_l_tmp_load_result_n899)
    {
      hxc_l_tmp_short_circuit_result_n311 = !hxc_l_recapturedThisFrame;
    }
    bool hxc_l_tmp_short_circuit_load_result_n901 = hxc_l_tmp_short_circuit_result_n311;
    bool hxc_l_tmp_short_circuit_result_n312 = hxc_l_tmp_short_circuit_load_result_n901;
    if (hxc_l_tmp_short_circuit_load_result_n901)
    {
      hxc_l_tmp_short_circuit_result_n312 = hxc_l_primaryPressed_h86125a319e35;
    }
    if (hxc_l_tmp_short_circuit_result_n312)
    {
      if (hxc_l_character.hxc_vitals.hxc_health > 0)
      {
        if (hxc_l_selectedMode == 1)
        {
          bool hxc_l_tmp_call_result_n908 = hxc_caxecraft_gameplay_Inventory_selectedIs(hxc_l_inventory, hxc_caxecraft_gameplay_ItemKind_CopperSword);
          bool hxc_l_tmp_short_circuit_result_n313 = !hxc_l_tmp_call_result_n908;
          if (!hxc_l_tmp_call_result_n908)
          {
            hxc_l_tmp_short_circuit_result_n313 = hxc_l_hit.hxc_hit;
          }
          bool hxc_l_tmp_short_circuit_load_result_n910 = hxc_l_tmp_short_circuit_result_n313;
          bool hxc_l_tmp_short_circuit_result_n314 = hxc_l_tmp_short_circuit_load_result_n910;
          if (hxc_l_tmp_short_circuit_load_result_n910)
          {
            struct hxc_caxecraft_domain_AquaticState hxc_l_tmp_record_field_load_result_n911 = hxc_l_character.hxc_aquatic;
            bool hxc_l_tmp_call_result_n913 = hxc_caxecraft_domain_Aquatics_canMine(hxc_l_tmp_record_field_load_result_n911, hxc_l_character.hxc_aquaticProfile);
            hxc_l_tmp_short_circuit_result_n314 = hxc_l_tmp_call_result_n913;
          }
          if (hxc_l_tmp_short_circuit_result_n314)
          {
            int32_t hxc_l_tmp_record_field_load_result_n915 = hxc_l_hit.hxc_cellX;
            int32_t hxc_l_tmp_record_field_load_result_n916 = hxc_l_hit.hxc_cellY;
            struct hxc_caxecraft_scenario_VoxelPoint hxc_l_tmp_call_result_n918 = hxc_caxecraft_domain_World_coord(hxc_l_tmp_record_field_load_result_n915, hxc_l_tmp_record_field_load_result_n916, hxc_l_hit.hxc_cellZ);
            struct hxc_caxecraft_scenario_VoxelPoint hxc_l_minedCoordinate = hxc_l_tmp_call_result_n918;
            int32_t hxc_l_sandBefore = hxc_l_inventory.hxc_sand;
            (void)hxc_l_sandBefore;
            struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n920 = hxc_l_session;
            hxc_l_gc_roots[204] = (const void *)hxc_l_tmp_load_result_n920;
            if (hxc_l_tmp_load_result_n920 == NULL)
            {
              abort();
            }
            struct hxc_caxecraft_scenario_VoxelPoint hxc_l_tmp_load_result_n921 = hxc_l_minedCoordinate;
            struct hxc_caxecraft_gameplay_MiningResult hxc_l_tmp_instance_call_result_n923 = hxc_caxecraft_domain_GameSession_mineTerrain(hxc_l_tmp_load_result_n920, hxc_l_tmp_load_result_n921, hxc_l_inventory);
            struct hxc_caxecraft_gameplay_MiningResult hxc_l_mining = hxc_l_tmp_instance_call_result_n923;
            hxc_l_inventory = hxc_l_mining.hxc_inventory;
            if (hxc_l_mining.hxc_outcome == 2)
            {
              if (hxc_l_self == NULL)
              {
                abort();
              }
              struct hxc_caxecraft_app_TerrainRenderer *hxc_l_tmp_owned_class_field_address_n926 = &(*hxc_l_self).hxc_terrainRenderer;
              struct hxc_caxecraft_app_TerrainChunkCache *hxc_l_tmp_owned_class_field_address_n927 = &(*hxc_l_tmp_owned_class_field_address_n926).hxc_cache;
              if (hxc_l_tmp_owned_class_field_address_n927 == NULL)
              {
                abort();
              }
              hxc_caxecraft_app_TerrainChunkCache_invalidate(hxc_l_tmp_owned_class_field_address_n927, hxc_l_minedCoordinate);
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
            int32_t hxc_l_tmp_record_field_load_result_n931 = hxc_l_hit.hxc_cellX;
            int32_t hxc_l_tmp_record_field_load_result_n932 = hxc_l_hit.hxc_cellY;
            struct hxc_caxecraft_scenario_VoxelPoint hxc_l_tmp_call_result_n934 = hxc_caxecraft_domain_World_coord(hxc_l_tmp_record_field_load_result_n931, hxc_l_tmp_record_field_load_result_n932, hxc_l_hit.hxc_cellZ);
            struct hxc_caxecraft_scenario_VoxelPoint hxc_l_removedCoordinate = hxc_l_tmp_call_result_n934;
            struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n935 = hxc_l_session;
            hxc_l_gc_roots[205] = (const void *)hxc_l_tmp_load_result_n935;
            if (hxc_l_tmp_load_result_n935 == NULL)
            {
              abort();
            }
            bool hxc_l_tmp_instance_call_result_n937 = hxc_caxecraft_domain_GameSession_removeTerrain(hxc_l_tmp_load_result_n935, hxc_l_removedCoordinate);
            bool hxc_l_removed = hxc_l_tmp_instance_call_result_n937;
            if (hxc_l_removed)
            {
              if (hxc_l_self == NULL)
              {
                abort();
              }
              struct hxc_caxecraft_app_TerrainRenderer *hxc_l_tmp_owned_class_field_address_n939 = &(*hxc_l_self).hxc_terrainRenderer;
              struct hxc_caxecraft_app_TerrainChunkCache *hxc_l_tmp_owned_class_field_address_n940 = &(*hxc_l_tmp_owned_class_field_address_n939).hxc_cache;
              if (hxc_l_tmp_owned_class_field_address_n940 == NULL)
              {
                abort();
              }
              hxc_caxecraft_app_TerrainChunkCache_invalidate(hxc_l_tmp_owned_class_field_address_n940, hxc_l_removedCoordinate);
            }
          }
        }
      }
    }
    bool hxc_l_tmp_load_result_n942 = hxc_l_captured_h8194fa734f34;
    bool hxc_l_tmp_short_circuit_result_n320 = hxc_l_tmp_load_result_n942;
    if (hxc_l_tmp_load_result_n942)
    {
      hxc_l_tmp_short_circuit_result_n320 = hxc_l_secondaryPressed_he46a7f5dc259;
    }
    if (hxc_l_tmp_short_circuit_result_n320)
    {
      if (hxc_l_character.hxc_vitals.hxc_health > 0)
      {
        struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n946 = hxc_l_session;
        hxc_l_gc_roots[206] = (const void *)hxc_l_tmp_load_result_n946;
        if (hxc_l_tmp_load_result_n946 == NULL)
        {
          abort();
        }
        struct hxc_caxecraft_domain_LocalRecoveryResult hxc_l_tmp_instance_call_result_n948 = hxc_caxecraft_domain_GameSession_useSelectedRecovery(hxc_l_tmp_load_result_n946, hxc_l_inventory);
        struct hxc_caxecraft_domain_LocalRecoveryResult hxc_l_recovery = hxc_l_tmp_instance_call_result_n948;
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
              int32_t hxc_l_tmp_record_field_load_result_n955 = hxc_l_hit.hxc_previousX;
              int32_t hxc_l_tmp_record_field_load_result_n956 = hxc_l_hit.hxc_previousY;
              struct hxc_caxecraft_scenario_VoxelPoint hxc_l_tmp_call_result_n958 = hxc_caxecraft_domain_World_coord(hxc_l_tmp_record_field_load_result_n955, hxc_l_tmp_record_field_load_result_n956, hxc_l_hit.hxc_previousZ);
              struct hxc_caxecraft_scenario_VoxelPoint hxc_l_placement = hxc_l_tmp_call_result_n958;
              enum hxc_caxecraft_domain_BlockKind hxc_l_tmp_call_result_n960 = hxc_caxecraft_gameplay_Inventory_selectedBlock(hxc_l_inventory);
              enum hxc_caxecraft_domain_BlockKind hxc_l_selectedBlock = hxc_l_tmp_call_result_n960;
              struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_load_result_n961 = hxc_l_inventory;
              int32_t hxc_l_tmp_call_result_n963 = hxc_caxecraft_gameplay_Inventory_countAt(hxc_l_tmp_load_result_n961, hxc_l_inventory.hxc_selected);
              bool hxc_l_hasItem = hxc_l_tmp_call_result_n963 > 0;
              bool hxc_l_tmp_load_result_n964 = hxc_l_hasItem;
              bool hxc_l_tmp_short_circuit_result_n325 = !hxc_l_tmp_load_result_n964;
              if (!!hxc_l_tmp_load_result_n964)
              {
                bool hxc_l_tmp_call_result_n966 = hxc_caxecraft_domain_World_isPlaceable(hxc_l_selectedBlock);
                hxc_l_tmp_short_circuit_result_n325 = !hxc_l_tmp_call_result_n966;
              }
              bool hxc_l_tmp_short_circuit_load_result_n967 = hxc_l_tmp_short_circuit_result_n325;
              bool hxc_l_tmp_short_circuit_result_n326 = hxc_l_tmp_short_circuit_load_result_n967;
              if (!hxc_l_tmp_short_circuit_load_result_n967)
              {
                struct hxc_caxecraft_domain_CharacterBody hxc_l_tmp_record_field_load_result_n968 = hxc_l_character.hxc_body;
                bool hxc_l_tmp_call_result_n970 = hxc_caxecraft_domain_CharacterPhysics_canPlaceAt(hxc_l_tmp_record_field_load_result_n968, hxc_l_placement);
                hxc_l_tmp_short_circuit_result_n326 = !hxc_l_tmp_call_result_n970;
              }
              bool hxc_l_tmp_short_circuit_load_result_n971 = hxc_l_tmp_short_circuit_result_n326;
              bool hxc_l_tmp_short_circuit_result_n327 = hxc_l_tmp_short_circuit_load_result_n971;
              if (!hxc_l_tmp_short_circuit_load_result_n971)
              {
                struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n972 = hxc_l_session;
                hxc_l_gc_roots[207] = (const void *)hxc_l_tmp_load_result_n972;
                if (hxc_l_tmp_load_result_n972 == NULL)
                {
                  abort();
                }
                struct hxc_caxecraft_scenario_VoxelPoint hxc_l_tmp_load_result_n973 = hxc_l_placement;
                bool hxc_l_tmp_instance_call_result_n975 = hxc_caxecraft_domain_GameSession_placeTerrain(hxc_l_tmp_load_result_n972, hxc_l_tmp_load_result_n973, hxc_l_selectedBlock);
                hxc_l_tmp_short_circuit_result_n327 = !hxc_l_tmp_instance_call_result_n975;
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
                struct hxc_caxecraft_app_TerrainRenderer *hxc_l_tmp_owned_class_field_address_n977 = &(*hxc_l_self).hxc_terrainRenderer;
                struct hxc_caxecraft_app_TerrainChunkCache *hxc_l_tmp_owned_class_field_address_n978 = &(*hxc_l_tmp_owned_class_field_address_n977).hxc_cache;
                if (hxc_l_tmp_owned_class_field_address_n978 == NULL)
                {
                  abort();
                }
                hxc_caxecraft_app_TerrainChunkCache_invalidate(hxc_l_tmp_owned_class_field_address_n978, hxc_l_placement);
                if (hxc_l_selectedMode == 1)
                {
                  struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n982 = hxc_caxecraft_gameplay_Inventory_consumeSelected(hxc_l_inventory);
                  hxc_l_inventory = hxc_l_tmp_call_result_n982;
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
    if (!hxc_l_paused_hfac44b03fb01)
    {
      struct hxc_caxecraft_gameplay_BerryDropState hxc_l_tmp_load_result_n986 = hxc_l_berryDrop;
      double hxc_l_tmp_record_field_load_result_n987 = hxc_l_character.hxc_body.hxc_x;
      double hxc_l_tmp_record_field_load_result_n988 = hxc_l_character.hxc_body.hxc_y;
      bool hxc_l_tmp_call_result_n990 = hxc_caxecraft_gameplay_BerryDrop_isInRange(hxc_l_tmp_load_result_n986, hxc_l_tmp_record_field_load_result_n987, hxc_l_tmp_record_field_load_result_n988, hxc_l_character.hxc_body.hxc_z);
      if (hxc_l_tmp_call_result_n990)
      {
        struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_load_result_n991 = hxc_l_inventory;
        int32_t hxc_l_tmp_call_result_n994 = hxc_caxecraft_gameplay_Inventory_acceptedAmount(hxc_l_tmp_load_result_n991, hxc_caxecraft_gameplay_ItemKind_Berries, hxc_l_berryDrop.hxc_amount);
        int32_t hxc_l_acceptedDrop = hxc_l_tmp_call_result_n994;
        if (hxc_l_acceptedDrop > 0)
        {
          struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_load_result_n996 = hxc_l_inventory;
          struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n999 = hxc_caxecraft_gameplay_Inventory_collectItem(hxc_l_tmp_load_result_n996, hxc_caxecraft_gameplay_ItemKind_Berries, hxc_l_acceptedDrop);
          hxc_l_inventory = hxc_l_tmp_call_result_n999;
          struct hxc_caxecraft_gameplay_BerryDropState hxc_l_tmp_load_result_n1000 = hxc_l_berryDrop;
          struct hxc_caxecraft_gameplay_BerryDropState hxc_l_tmp_call_result_n1002 = hxc_caxecraft_gameplay_BerryDrop_collectAmount(hxc_l_tmp_load_result_n1000, hxc_l_acceptedDrop);
          hxc_l_berryDrop = hxc_l_tmp_call_result_n1002;
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
    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1019 = hxc_l_session;
    hxc_l_gc_roots[208] = (const void *)hxc_l_tmp_load_result_n1019;
    if (hxc_l_tmp_load_result_n1019 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_domain_GameView hxc_l_tmp_instance_call_result_n1020 = hxc_caxecraft_domain_GameSession_view(hxc_l_tmp_load_result_n1019);
    struct hxc_caxecraft_domain_GameView hxc_l_committedView = hxc_l_tmp_instance_call_result_n1020;
    if (!hxc_l_committedView.hxc_valid)
    {
      hxc_l_quit = true;
    }
    else
    {
      hxc_l_character = hxc_l_committedView.hxc_localPlayer;
    }
    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1023 = hxc_l_session;
    hxc_l_gc_roots[209] = (const void *)hxc_l_tmp_load_result_n1023;
    if (hxc_l_tmp_load_result_n1023 == NULL)
    {
      abort();
    }
    bool hxc_l_tmp_instance_call_result_n1025 = hxc_caxecraft_domain_GameSession_actorInteractionAvailable(hxc_l_tmp_load_result_n1023, hxc_l_dialogueActorId);
    hxc_l_guideInteractionAvailable = hxc_l_tmp_instance_call_result_n1025;
    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1026 = hxc_l_session;
    hxc_l_gc_roots[210] = (const void *)hxc_l_tmp_load_result_n1026;
    if (hxc_l_tmp_load_result_n1026 == NULL)
    {
      abort();
    }
    struct hxc_array_ref *hxc_l_tmp_instance_call_result_n1027 = hxc_caxecraft_domain_GameSession_actorControllerStateSnapshots(hxc_l_tmp_load_result_n1026);
    hxc_l_presentationActorPhases = hxc_l_tmp_instance_call_result_n1027;
    struct hxc_array_ref *hxc_l_tmp_load_result_n1028 = hxc_l_presentationActorPhases;
    struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation hxc_l_tmp_call_result_n1031 = hxc_caxecraft_app_CaxecraftApp_observeActorPhase(hxc_l_tmp_load_result_n1028, hxc_l_dialogueActorId, hxc_caxecraft_domain_ActorControllerPhase_Stationary);
    hxc_l_dialoguePhase = hxc_l_tmp_call_result_n1031;
    struct hxc_array_ref *hxc_l_tmp_load_result_n1032 = hxc_l_presentationActorPhases;
    struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation hxc_l_tmp_call_result_n1035 = hxc_caxecraft_app_CaxecraftApp_observeActorPhase(hxc_l_tmp_load_result_n1032, hxc_l_enemyActorId, hxc_caxecraft_domain_ActorControllerPhase_Defeated);
    hxc_l_enemyPhase_h70382f3bdc66 = hxc_l_tmp_call_result_n1035;
    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1036 = hxc_l_session;
    hxc_l_gc_roots[211] = (const void *)hxc_l_tmp_load_result_n1036;
    if (hxc_l_tmp_load_result_n1036 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n1038 = hxc_caxecraft_domain_GameSession_readCharacter(hxc_l_tmp_load_result_n1036, hxc_l_dialogueActorId);
    hxc_l_dialogueActor = hxc_l_tmp_instance_call_result_n1038;
    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1039 = hxc_l_session;
    hxc_l_gc_roots[212] = (const void *)hxc_l_tmp_load_result_n1039;
    if (hxc_l_tmp_load_result_n1039 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n1041 = hxc_caxecraft_domain_GameSession_readCharacter(hxc_l_tmp_load_result_n1039, hxc_l_enemyActorId);
    hxc_l_enemyActor = hxc_l_tmp_instance_call_result_n1041;
    bool hxc_l_tmp_record_field_load_result_n1042 = hxc_l_dialoguePhase.hxc_valid;
    bool hxc_l_tmp_short_circuit_result_n331 = !hxc_l_tmp_record_field_load_result_n1042;
    if (!!hxc_l_tmp_record_field_load_result_n1042)
    {
      hxc_l_tmp_short_circuit_result_n331 = !hxc_l_enemyPhase_h70382f3bdc66.hxc_valid;
    }
    bool hxc_l_tmp_short_circuit_load_result_n1044 = hxc_l_tmp_short_circuit_result_n331;
    bool hxc_l_tmp_short_circuit_result_n332 = hxc_l_tmp_short_circuit_load_result_n1044;
    if (!hxc_l_tmp_short_circuit_load_result_n1044)
    {
      hxc_l_tmp_short_circuit_result_n332 = hxc_l_dialogueActor.hxc_id <= 0;
    }
    bool hxc_l_tmp_short_circuit_load_result_n1046 = hxc_l_tmp_short_circuit_result_n332;
    bool hxc_l_tmp_short_circuit_result_n333 = hxc_l_tmp_short_circuit_load_result_n1046;
    if (!hxc_l_tmp_short_circuit_load_result_n1046)
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
      struct hxc_caxecraft_app_MotionHistory hxc_l_tmp_call_result_n1052 = hxc_caxecraft_app_MotionInterpolation_start(hxc_l_character.hxc_body);
      hxc_l_motionHistory = hxc_l_tmp_call_result_n1052;
    }
    struct hxc_caxecraft_app_MotionHistory hxc_l_tmp_load_result_n1053 = hxc_l_motionHistory;
    struct hxc_caxecraft_app_RenderPosition hxc_l_tmp_call_result_n1055 = hxc_caxecraft_app_MotionInterpolation_sample(hxc_l_tmp_load_result_n1053, hxc_l_accumulator, 0.05);
    struct hxc_caxecraft_app_RenderPosition hxc_l_renderPosition = hxc_l_tmp_call_result_n1055;
    double hxc_l_eyeX = hxc_l_renderPosition.hxc_x;
    double hxc_l_eyeY = hxc_l_renderPosition.hxc_y + 1.62;
    double hxc_l_eyeZ = hxc_l_renderPosition.hxc_z;
    double hxc_l_tmp_load_result_n1059 = hxc_l_eyeX;
    double hxc_l_tmp_load_result_n1060 = hxc_l_eyeY;
    double hxc_l_tmp_load_result_n1061 = hxc_l_eyeZ;
    double hxc_l_tmp_load_result_n1063 = hxc_l_eyeX;
    double hxc_l_tmp_load_result_n1064 = hxc_l_lookX;
    double hxc_l_tmp_load_result_n1065 = hxc_l_eyeY;
    double hxc_l_tmp_load_result_n1066 = hxc_l_lookY;
    double hxc_l_tmp_load_result_n1067 = hxc_l_eyeZ;
    struct Camera3D hxc_l_camera = (struct Camera3D){ .position = (struct Vector3){ .x = (float)hxc_l_tmp_load_result_n1059, .y = (float)hxc_l_tmp_load_result_n1060, .z = (float)hxc_l_tmp_load_result_n1061 }, .target = (struct Vector3){ .x = (float)(hxc_l_tmp_load_result_n1063 + hxc_l_tmp_load_result_n1064), .y = (float)(hxc_l_tmp_load_result_n1065 + hxc_l_tmp_load_result_n1066), .z = (float)(hxc_l_tmp_load_result_n1067 + hxc_l_lookZ) }, .up = (struct Vector3){ .x = (float)0.0, .y = (float)1.0, .z = (float)0.0 }, .fovy = (float)70.0, .projection = 0 };
    BeginDrawing();
    if (hxc_l_onTitle)
    {
      hxc_string hxc_l_locale1_h1f266cda77de = { 0 };
      switch (hxc_l_locale) {
        case 0:
          {
            hxc_l_locale1_h1f266cda77de = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
            if (hxc_string_retain(hxc_l_locale1_h1f266cda77de) != HXC_STATUS_OK)
            {
              abort();
            }
            break;
          }
        case 1:
          {
            hxc_l_locale1_h1f266cda77de = (hxc_string){ (const uint8_t *)"es-mx", 5, true, NULL };
            if (hxc_string_retain(hxc_l_locale1_h1f266cda77de) != HXC_STATUS_OK)
            {
              abort();
            }
            break;
          }
        default:
          {
            hxc_l_locale1_h1f266cda77de = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
            if (hxc_string_retain(hxc_l_locale1_h1f266cda77de) != HXC_STATUS_OK)
            {
              abort();
            }
            break;
          }
      }
      hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n1074 = hxc_l_locale1_h1f266cda77de;
      hxc_l_tmp_managed_flow_owner_n341 = hxc_l_tmp_managed_flow_carrier_move_result_n1074;
      struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1075 = hxc_l_levelView;
      hxc_l_gc_roots[213] = (const void *)hxc_l_tmp_load_result_n1075;
      if (hxc_l_tmp_load_result_n1075 == NULL)
      {
        abort();
      }
      struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_class_field_load_result_n1076 = (*hxc_l_tmp_load_result_n1075).hxc_levelPresentation;
      hxc_l_gc_roots[214] = (const void *)hxc_l_tmp_class_field_load_result_n1076;
      if (hxc_l_tmp_class_field_load_result_n1076 == NULL)
      {
        abort();
      }
      hxc_string hxc_l_tmp_instance_call_result_n1078 = hxc_caxecraft_content_RuntimeLevelPresentation_message(hxc_l_tmp_class_field_load_result_n1076, (hxc_string){ (const uint8_t *)"adventure_tagline", 17, true, NULL }, hxc_l_tmp_managed_flow_owner_n341);
      hxc_l_adventureTagline = hxc_l_tmp_instance_call_result_n1078;
      int32_t hxc_l_tmp_native_call_result_n1079 = GetScreenWidth();
      int32_t hxc_l_width_h831fdd5f219f = (int32_t)hxc_l_tmp_native_call_result_n1079;
      int32_t hxc_l_tmp_native_call_result_n1080 = GetScreenHeight();
      int32_t hxc_l_height_h37d9cab53d48 = (int32_t)hxc_l_tmp_native_call_result_n1080;
      if (hxc_l_titleTextureReady)
      {
        struct Color hxc_l_this1_hb1e1bdf69714 = (struct Color){ .r = 255, .g = 255, .b = 255, .a = 255 };
        struct Texture hxc_l_tmp_load_result_n1083 = hxc_l_titleTexture;
        double hxc_l_imageAspect = hxc_f64_divide_zero_safe((double)(int32_t)hxc_l_tmp_load_result_n1083.width, (double)(int32_t)hxc_l_titleTexture.height);
        int32_t hxc_l_tmp_load_result_n1087 = hxc_l_width_h831fdd5f219f;
        double hxc_l_screenAspect = hxc_f64_divide_zero_safe((double)hxc_l_tmp_load_result_n1087, (double)hxc_l_height_h37d9cab53d48);
        double hxc_l_sourceX = 0.0;
        double hxc_l_sourceY = 0.0;
        double hxc_l_sourceWidth = (double)(int32_t)hxc_l_titleTexture.width;
        double hxc_l_sourceHeight = (double)(int32_t)hxc_l_titleTexture.height;
        double hxc_l_tmp_load_result_n1093 = hxc_l_screenAspect;
        if (hxc_l_tmp_load_result_n1093 > hxc_l_imageAspect)
        {
          struct Texture hxc_l_tmp_load_result_n1095 = hxc_l_titleTexture;
          hxc_l_sourceHeight = hxc_f64_divide_zero_safe((double)(int32_t)hxc_l_tmp_load_result_n1095.width, hxc_l_screenAspect);
          struct Texture hxc_l_tmp_load_result_n1098 = hxc_l_titleTexture;
          hxc_l_sourceY = ((double)(int32_t)hxc_l_tmp_load_result_n1098.height - hxc_l_sourceHeight) * 0.5;
        }
        else
        {
          struct Texture hxc_l_tmp_load_result_n1101 = hxc_l_titleTexture;
          hxc_l_sourceWidth = (double)(int32_t)hxc_l_tmp_load_result_n1101.height * hxc_l_screenAspect;
          struct Texture hxc_l_tmp_load_result_n1104 = hxc_l_titleTexture;
          hxc_l_sourceX = ((double)(int32_t)hxc_l_tmp_load_result_n1104.width - hxc_l_sourceWidth) * 0.5;
        }
        struct Texture hxc_l_tmp_load_result_n1107 = hxc_l_titleTexture;
        double hxc_l_tmp_load_result_n1108 = hxc_l_sourceX;
        double hxc_l_tmp_load_result_n1109 = hxc_l_sourceY;
        double hxc_l_tmp_load_result_n1110 = hxc_l_sourceWidth;
        double hxc_l_tmp_load_result_n1111 = hxc_l_sourceHeight;
        int32_t hxc_l_tmp_load_result_n1113 = hxc_l_width_h831fdd5f219f;
        int32_t hxc_l_tmp_load_result_n1114 = hxc_l_height_h37d9cab53d48;
        DrawTexturePro(hxc_l_tmp_load_result_n1107, (struct Rectangle){ .x = (float)hxc_l_tmp_load_result_n1108, .y = (float)hxc_l_tmp_load_result_n1109, .width = (float)hxc_l_tmp_load_result_n1110, .height = (float)hxc_l_tmp_load_result_n1111 }, (struct Rectangle){ .x = (float)0.0, .y = (float)0.0, .width = (float)((double)hxc_l_tmp_load_result_n1113 + 0.0), .height = (float)((double)hxc_l_tmp_load_result_n1114 + 0.0) }, (struct Vector2){ .x = (float)0.0, .y = (float)0.0 }, (float)0.0, hxc_l_this1_hb1e1bdf69714);
      }
      else
      {
        ClearBackground((struct Color){ .r = 126, .g = 190, .b = 201, .a = 255 });
      }
      struct Color hxc_l_this1_h120a91275d17 = (struct Color){ .r = 4, .g = 14, .b = 23, .a = 132 };
      int32_t hxc_l_tmp_load_result_n1120 = hxc_l_width_h831fdd5f219f;
      int32_t hxc_l_tmp_load_result_n1121 = hxc_l_height_h37d9cab53d48;
      DrawRectangle((int32_t)0, (int32_t)0, (int32_t)hxc_l_tmp_load_result_n1120, (int32_t)hxc_l_tmp_load_result_n1121, hxc_l_this1_h120a91275d17);
      if (hxc_l_wordmarkTextureReady)
      {
        int32_t hxc_l_centerX = hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_l_width_h831fdd5f219f, (double)2));
        struct Color hxc_l_this1_hfae3d0c5ee8d = (struct Color){ .r = 255, .g = 255, .b = 255, .a = 255 };
        double hxc_l_scale = hxc_f64_divide_zero_safe((double)560, (double)(int32_t)hxc_l_wordmarkTexture.width);
        double hxc_l_heightScale = hxc_f64_divide_zero_safe((double)132, (double)(int32_t)hxc_l_wordmarkTexture.height);
        double hxc_l_tmp_load_result_n1130 = hxc_l_heightScale;
        if (hxc_l_tmp_load_result_n1130 < hxc_l_scale)
        {
          hxc_l_scale = hxc_l_heightScale;
        }
        struct Texture hxc_l_tmp_load_result_n1133 = hxc_l_wordmarkTexture;
        double hxc_l_width_h687321093b53 = (double)(int32_t)hxc_l_tmp_load_result_n1133.width * hxc_l_scale;
        struct Texture hxc_l_tmp_load_result_n1136 = hxc_l_wordmarkTexture;
        double hxc_l_height_h9a3fd9e117d3 = (double)(int32_t)hxc_l_tmp_load_result_n1136.height * hxc_l_scale;
        double hxc_l_width1 = (double)(int32_t)hxc_l_wordmarkTexture.width + 0.0;
        double hxc_l_height1 = (double)(int32_t)hxc_l_wordmarkTexture.height + 0.0;
        struct Texture hxc_l_tmp_load_result_n1143 = hxc_l_wordmarkTexture;
        double hxc_l_tmp_load_result_n1144 = hxc_l_width1;
        double hxc_l_tmp_load_result_n1145 = hxc_l_height1;
        int32_t hxc_l_tmp_load_result_n1147 = hxc_l_centerX;
        double hxc_l_tmp_load_result_n1148 = hxc_l_width_h687321093b53;
        double hxc_l_tmp_load_result_n1149 = hxc_l_width_h687321093b53;
        double hxc_l_tmp_load_result_n1150 = hxc_l_height_h9a3fd9e117d3;
        DrawTexturePro(hxc_l_tmp_load_result_n1143, (struct Rectangle){ .x = (float)0.0, .y = (float)0.0, .width = (float)hxc_l_tmp_load_result_n1144, .height = (float)hxc_l_tmp_load_result_n1145 }, (struct Rectangle){ .x = (float)((double)hxc_l_tmp_load_result_n1147 - hxc_l_tmp_load_result_n1148 * 0.5), .y = (float)28.0, .width = (float)hxc_l_tmp_load_result_n1149, .height = (float)hxc_l_tmp_load_result_n1150 }, (struct Vector2){ .x = (float)0.0, .y = (float)0.0 }, (float)0.0, hxc_l_this1_hfae3d0c5ee8d);
      }
      else
      {
        int32_t hxc_l_x_h427167ddfb12 = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_l_width_h831fdd5f219f, (double)2)), 92);
        struct Color hxc_l_this1_h7d7bc1f45994 = (struct Color){ .r = 242, .g = 249, .b = 245, .a = 255 };
        struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_tmp_load_result_n1156 = hxc_l_uiCatalog;
        hxc_l_gc_roots[215] = (const void *)hxc_l_tmp_load_result_n1156;
        if (hxc_l_tmp_load_result_n1156 == NULL)
        {
          abort();
        }
        hxc_string hxc_l_tmp_instance_call_result_n1158 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_tmp_load_result_n1156, hxc_l_locale, 34);
        hxc_l_tmp_cstring_ref_owner_owner_n363 = hxc_l_tmp_instance_call_result_n1158;
        hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n1160 = HXC_BORROWED_CSTRING_INITIALIZER;
        if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n363, &hxc_l_tmp_string_borrow_cstring_result_n1160) != HXC_STATUS_OK)
        {
          abort();
        }
        int32_t hxc_l_tmp_load_result_n1161 = hxc_l_x_h427167ddfb12;
        DrawText(hxc_l_tmp_string_borrow_cstring_result_n1160.data, (int32_t)hxc_l_tmp_load_result_n1161, (int32_t)52, (int32_t)34, hxc_l_this1_h7d7bc1f45994);
        if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n363) != HXC_STATUS_OK)
        {
          abort();
        }
      }
      int32_t hxc_l_firstTop = hxc_i32_subtract_wrapping(hxc_l_height_h37d9cab53d48, 293);
      int32_t hxc_l_left_hcddc22377f80 = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_l_width_h831fdd5f219f, (double)2)), 180);
      if (hxc_l_selectedMode == 0)
      {
        struct Color hxc_l_this1_ha5c613b5a867 = (struct Color){ .r = 16, .g = 88, .b = 102, .a = 232 };
        int32_t hxc_l_tmp_load_result_n1167 = hxc_l_left_hcddc22377f80;
        int32_t hxc_l_tmp_load_result_n1168 = hxc_l_firstTop;
        DrawRectangle((int32_t)hxc_l_tmp_load_result_n1167, (int32_t)hxc_l_tmp_load_result_n1168, (int32_t)360, (int32_t)54, hxc_l_this1_ha5c613b5a867);
        struct Color hxc_l_this1_hfd769f88defc = (struct Color){ .r = 255, .g = 190, .b = 55, .a = 255 };
        int32_t hxc_l_tmp_load_result_n1171 = hxc_l_left_hcddc22377f80;
        int32_t hxc_l_tmp_load_result_n1172 = hxc_l_firstTop;
        DrawRectangleLines((int32_t)hxc_l_tmp_load_result_n1171, (int32_t)hxc_l_tmp_load_result_n1172, (int32_t)360, (int32_t)54, hxc_l_this1_hfd769f88defc);
      }
      else
      {
        struct Color hxc_l_this1_h2bf46de51f33 = (struct Color){ .r = 6, .g = 26, .b = 36, .a = 218 };
        int32_t hxc_l_tmp_load_result_n1175 = hxc_l_left_hcddc22377f80;
        int32_t hxc_l_tmp_load_result_n1176 = hxc_l_firstTop;
        DrawRectangle((int32_t)hxc_l_tmp_load_result_n1175, (int32_t)hxc_l_tmp_load_result_n1176, (int32_t)360, (int32_t)54, hxc_l_this1_h2bf46de51f33);
        struct Color hxc_l_this1_hfa2cf41fa07b = (struct Color){ .r = 92, .g = 194, .b = 188, .a = 255 };
        int32_t hxc_l_tmp_load_result_n1179 = hxc_l_left_hcddc22377f80;
        int32_t hxc_l_tmp_load_result_n1180 = hxc_l_firstTop;
        DrawRectangleLines((int32_t)hxc_l_tmp_load_result_n1179, (int32_t)hxc_l_tmp_load_result_n1180, (int32_t)360, (int32_t)54, hxc_l_this1_hfa2cf41fa07b);
      }
      struct Color hxc_l_this1_h37c29b5bdcf5 = (struct Color){ .r = 242, .g = 249, .b = 245, .a = 255 };
      struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_tmp_load_result_n1183 = hxc_l_uiCatalog;
      hxc_l_gc_roots[216] = (const void *)hxc_l_tmp_load_result_n1183;
      if (hxc_l_tmp_load_result_n1183 == NULL)
      {
        abort();
      }
      hxc_string hxc_l_tmp_instance_call_result_n1185 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_tmp_load_result_n1183, hxc_l_locale, 27);
      hxc_l_tmp_cstring_ref_owner_owner_n371 = hxc_l_tmp_instance_call_result_n1185;
      hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n1187 = HXC_BORROWED_CSTRING_INITIALIZER;
      if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n371, &hxc_l_tmp_string_borrow_cstring_result_n1187) != HXC_STATUS_OK)
      {
        abort();
      }
      int32_t hxc_l_tmp_load_result_n1188 = hxc_l_left_hcddc22377f80;
      int32_t hxc_l_tmp_load_result_n1189 = hxc_l_firstTop;
      DrawText(hxc_l_tmp_string_borrow_cstring_result_n1187.data, (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n1188, 28), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n1189, 16), (int32_t)21, hxc_l_this1_h37c29b5bdcf5);
      int32_t hxc_l_top_hb6a2d20f79d2 = hxc_i32_add_wrapping(hxc_i32_add_wrapping(hxc_l_firstTop, 54), 14);
      int32_t hxc_l_left_hbb51f6b469c5 = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_l_width_h831fdd5f219f, (double)2)), 180);
      if (hxc_l_selectedMode == 1)
      {
        struct Color hxc_l_this1_h075d852a32a1 = (struct Color){ .r = 16, .g = 88, .b = 102, .a = 232 };
        int32_t hxc_l_tmp_load_result_n1195 = hxc_l_left_hbb51f6b469c5;
        int32_t hxc_l_tmp_load_result_n1196 = hxc_l_top_hb6a2d20f79d2;
        DrawRectangle((int32_t)hxc_l_tmp_load_result_n1195, (int32_t)hxc_l_tmp_load_result_n1196, (int32_t)360, (int32_t)54, hxc_l_this1_h075d852a32a1);
        struct Color hxc_l_this1_h0fb0cd788821 = (struct Color){ .r = 255, .g = 190, .b = 55, .a = 255 };
        int32_t hxc_l_tmp_load_result_n1199 = hxc_l_left_hbb51f6b469c5;
        int32_t hxc_l_tmp_load_result_n1200 = hxc_l_top_hb6a2d20f79d2;
        DrawRectangleLines((int32_t)hxc_l_tmp_load_result_n1199, (int32_t)hxc_l_tmp_load_result_n1200, (int32_t)360, (int32_t)54, hxc_l_this1_h0fb0cd788821);
      }
      else
      {
        struct Color hxc_l_this1_h97e2d84fe1a6 = (struct Color){ .r = 6, .g = 26, .b = 36, .a = 218 };
        int32_t hxc_l_tmp_load_result_n1203 = hxc_l_left_hbb51f6b469c5;
        int32_t hxc_l_tmp_load_result_n1204 = hxc_l_top_hb6a2d20f79d2;
        DrawRectangle((int32_t)hxc_l_tmp_load_result_n1203, (int32_t)hxc_l_tmp_load_result_n1204, (int32_t)360, (int32_t)54, hxc_l_this1_h97e2d84fe1a6);
        struct Color hxc_l_this1_he0c40ca71a96 = (struct Color){ .r = 92, .g = 194, .b = 188, .a = 255 };
        int32_t hxc_l_tmp_load_result_n1207 = hxc_l_left_hbb51f6b469c5;
        int32_t hxc_l_tmp_load_result_n1208 = hxc_l_top_hb6a2d20f79d2;
        DrawRectangleLines((int32_t)hxc_l_tmp_load_result_n1207, (int32_t)hxc_l_tmp_load_result_n1208, (int32_t)360, (int32_t)54, hxc_l_this1_he0c40ca71a96);
      }
      struct Color hxc_l_this1_hd3da2428b08f = (struct Color){ .r = 242, .g = 249, .b = 245, .a = 255 };
      struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_tmp_load_result_n1211 = hxc_l_uiCatalog;
      hxc_l_gc_roots[217] = (const void *)hxc_l_tmp_load_result_n1211;
      if (hxc_l_tmp_load_result_n1211 == NULL)
      {
        abort();
      }
      hxc_string hxc_l_tmp_instance_call_result_n1213 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_tmp_load_result_n1211, hxc_l_locale, 26);
      hxc_l_tmp_cstring_ref_owner_owner_n379 = hxc_l_tmp_instance_call_result_n1213;
      hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n1215 = HXC_BORROWED_CSTRING_INITIALIZER;
      if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n379, &hxc_l_tmp_string_borrow_cstring_result_n1215) != HXC_STATUS_OK)
      {
        abort();
      }
      int32_t hxc_l_tmp_load_result_n1216 = hxc_l_left_hbb51f6b469c5;
      int32_t hxc_l_tmp_load_result_n1217 = hxc_l_top_hb6a2d20f79d2;
      DrawText(hxc_l_tmp_string_borrow_cstring_result_n1215.data, (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n1216, 28), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n1217, 16), (int32_t)21, hxc_l_this1_hd3da2428b08f);
      int32_t hxc_l_top_hc597d6de9942 = hxc_i32_add_wrapping(hxc_l_firstTop, 136);
      int32_t hxc_l_left_h17c89919da41 = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_l_width_h831fdd5f219f, (double)2)), 180);
      struct Color hxc_l_this1_h1f91afaa7099 = (struct Color){ .r = 6, .g = 26, .b = 36, .a = 218 };
      int32_t hxc_l_tmp_load_result_n1222 = hxc_l_left_h17c89919da41;
      int32_t hxc_l_tmp_load_result_n1223 = hxc_l_top_hc597d6de9942;
      DrawRectangle((int32_t)hxc_l_tmp_load_result_n1222, (int32_t)hxc_l_tmp_load_result_n1223, (int32_t)360, (int32_t)54, hxc_l_this1_h1f91afaa7099);
      struct Color hxc_l_this1_h6533ab967d3f = (struct Color){ .r = 92, .g = 194, .b = 188, .a = 255 };
      int32_t hxc_l_tmp_load_result_n1226 = hxc_l_left_h17c89919da41;
      int32_t hxc_l_tmp_load_result_n1227 = hxc_l_top_hc597d6de9942;
      DrawRectangleLines((int32_t)hxc_l_tmp_load_result_n1226, (int32_t)hxc_l_tmp_load_result_n1227, (int32_t)360, (int32_t)54, hxc_l_this1_h6533ab967d3f);
      struct Color hxc_l_this1_h65e5e0b6262a = (struct Color){ .r = 242, .g = 249, .b = 245, .a = 255 };
      struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_tmp_load_result_n1230 = hxc_l_uiCatalog;
      hxc_l_gc_roots[218] = (const void *)hxc_l_tmp_load_result_n1230;
      if (hxc_l_tmp_load_result_n1230 == NULL)
      {
        abort();
      }
      hxc_string hxc_l_tmp_instance_call_result_n1232 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_tmp_load_result_n1230, hxc_l_locale, 28);
      hxc_l_tmp_cstring_ref_owner_owner_n385 = hxc_l_tmp_instance_call_result_n1232;
      hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n1234 = HXC_BORROWED_CSTRING_INITIALIZER;
      if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n385, &hxc_l_tmp_string_borrow_cstring_result_n1234) != HXC_STATUS_OK)
      {
        abort();
      }
      int32_t hxc_l_tmp_load_result_n1235 = hxc_l_left_h17c89919da41;
      int32_t hxc_l_tmp_load_result_n1236 = hxc_l_top_hc597d6de9942;
      DrawText(hxc_l_tmp_string_borrow_cstring_result_n1234.data, (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n1235, 28), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n1236, 16), (int32_t)21, hxc_l_this1_h65e5e0b6262a);
      int32_t hxc_l_x_h98f12e35bef9 = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_l_width_h831fdd5f219f, (double)2)), 285);
      struct Color hxc_l_this1_h687f6a40c850 = (struct Color){ .r = 229, .g = 241, .b = 235, .a = 255 };
      struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_tmp_load_result_n1240 = hxc_l_uiCatalog;
      hxc_l_gc_roots[219] = (const void *)hxc_l_tmp_load_result_n1240;
      if (hxc_l_tmp_load_result_n1240 == NULL)
      {
        abort();
      }
      hxc_string hxc_l_tmp_instance_call_result_n1242 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_tmp_load_result_n1240, hxc_l_locale, 29);
      hxc_l_tmp_cstring_ref_owner_owner_n388 = hxc_l_tmp_instance_call_result_n1242;
      hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n1244 = HXC_BORROWED_CSTRING_INITIALIZER;
      if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n388, &hxc_l_tmp_string_borrow_cstring_result_n1244) != HXC_STATUS_OK)
      {
        abort();
      }
      int32_t hxc_l_tmp_load_result_n1245 = hxc_l_x_h98f12e35bef9;
      int32_t hxc_l_tmp_load_result_n1246 = hxc_l_height_h37d9cab53d48;
      DrawText(hxc_l_tmp_string_borrow_cstring_result_n1244.data, (int32_t)hxc_l_tmp_load_result_n1245, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n1246, 58), (int32_t)16, hxc_l_this1_h687f6a40c850);
      int32_t hxc_l_tmp_load_result_n1248 = hxc_l_selectedMode;
      bool hxc_l_tmp_short_circuit_result_n389 = hxc_l_tmp_load_result_n1248 == 1;
      if (hxc_l_tmp_load_result_n1248 == 1)
      {
        hxc_string hxc_l_tmp_load_result_n1249 = hxc_l_adventureTagline;
        if (hxc_l_tmp_load_result_n1249.data == NULL)
        {
          abort();
        }
        int32_t hxc_l_tmp_string_length_result_n1250;
        if (hxc_string_haxe_length(hxc_l_tmp_load_result_n1249, &hxc_l_tmp_string_length_result_n1250) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_l_tmp_short_circuit_result_n389 = hxc_l_tmp_string_length_result_n1250 > 0;
      }
      if (hxc_l_tmp_short_circuit_result_n389)
      {
        int32_t hxc_l_x_hf39b75db97f7 = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_l_width_h831fdd5f219f, (double)2)), 230);
        struct Color hxc_l_this1_h64c56e4eee04 = (struct Color){ .r = 255, .g = 205, .b = 91, .a = 255 };
        hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n1255 = HXC_BORROWED_CSTRING_INITIALIZER;
        if (hxc_string_borrow_cstring(&hxc_l_adventureTagline, &hxc_l_tmp_string_borrow_cstring_result_n1255) != HXC_STATUS_OK)
        {
          abort();
        }
        int32_t hxc_l_tmp_load_result_n1256 = hxc_l_x_hf39b75db97f7;
        int32_t hxc_l_tmp_load_result_n1257 = hxc_l_firstTop;
        DrawText(hxc_l_tmp_string_borrow_cstring_result_n1255.data, (int32_t)hxc_l_tmp_load_result_n1256, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n1257, 36), (int32_t)17, hxc_l_this1_h64c56e4eee04);
      }
      if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n388) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n385) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n379) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n371) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_adventureTagline) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n341) != HXC_STATUS_OK)
      {
        abort();
      }
    }
    else
    {
      if (hxc_l_onCampaignSelect)
      {
        hxc_l_gc_roots[220] = (const void *)hxc_l_campaign;
        struct hxc_caxecraft_content_CampaignManifest *hxc_l_selectedCampaign_h59a580bbe469 = hxc_l_campaign;
        hxc_l_gc_roots[221] = (const void *)hxc_l_selectedCampaign_h59a580bbe469;
        hxc_l_gc_roots[222] = (const void *)NULL;
        if (hxc_l_selectedCampaign_h59a580bbe469 == NULL)
        {
          enum hxc_caxecraft_app_AppScreen hxc_l_tmp_call_result_n1263 = hxc_caxecraft_app_AppScreen_closeCampaignSelection(hxc_l_screen);
          hxc_l_screen = hxc_l_tmp_call_result_n1263;
        }
        else
        {
          hxc_string hxc_l_locale1_h3e03aa75d95f = { 0 };
          switch (hxc_l_locale) {
            case 0:
              {
                hxc_l_locale1_h3e03aa75d95f = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
                if (hxc_string_retain(hxc_l_locale1_h3e03aa75d95f) != HXC_STATUS_OK)
                {
                  abort();
                }
                break;
              }
            case 1:
              {
                hxc_l_locale1_h3e03aa75d95f = (hxc_string){ (const uint8_t *)"es-mx", 5, true, NULL };
                if (hxc_string_retain(hxc_l_locale1_h3e03aa75d95f) != HXC_STATUS_OK)
                {
                  abort();
                }
                break;
              }
            default:
              {
                hxc_l_locale1_h3e03aa75d95f = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
                if (hxc_string_retain(hxc_l_locale1_h3e03aa75d95f) != HXC_STATUS_OK)
                {
                  abort();
                }
                break;
              }
          }
          hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n1265 = hxc_l_locale1_h3e03aa75d95f;
          hxc_l_tmp_managed_flow_owner_n394 = hxc_l_tmp_managed_flow_carrier_move_result_n1265;
          struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1266 = hxc_l_levelView;
          hxc_l_gc_roots[223] = (const void *)hxc_l_tmp_load_result_n1266;
          if (hxc_l_tmp_load_result_n1266 == NULL)
          {
            abort();
          }
          struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_class_field_load_result_n1267 = (*hxc_l_tmp_load_result_n1266).hxc_levelPresentation;
          hxc_l_gc_roots[224] = (const void *)hxc_l_tmp_class_field_load_result_n1267;
          struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_this = hxc_l_tmp_class_field_load_result_n1267;
          struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_load_result_n1268 = hxc_l_this;
          hxc_l_gc_roots[225] = (const void *)hxc_l_tmp_load_result_n1268;
          if (hxc_l_tmp_load_result_n1268 == NULL)
          {
            abort();
          }
          struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_load_result_n1269 = hxc_l_this;
          hxc_l_gc_roots[226] = (const void *)hxc_l_tmp_load_result_n1269;
          if (hxc_l_tmp_load_result_n1269 == NULL)
          {
            abort();
          }
          struct hxc_caxecraft_scenario_ScenarioText hxc_l_tmp_class_field_load_result_n1270 = (*hxc_l_tmp_load_result_n1269).hxc_title;
          hxc_string hxc_l_tmp_instance_call_result_n1272 = hxc_caxecraft_content_RuntimeLevelPresentation_resolve(hxc_l_tmp_load_result_n1268, hxc_l_tmp_class_field_load_result_n1270, hxc_l_tmp_managed_flow_owner_n394);
          hxc_l_tmp = hxc_l_tmp_instance_call_result_n1272;
          hxc_string hxc_l_locale1_hbaafb99b1676 = { 0 };
          switch (hxc_l_locale) {
            case 0:
              {
                hxc_l_locale1_hbaafb99b1676 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
                if (hxc_string_retain(hxc_l_locale1_hbaafb99b1676) != HXC_STATUS_OK)
                {
                  abort();
                }
                break;
              }
            case 1:
              {
                hxc_l_locale1_hbaafb99b1676 = (hxc_string){ (const uint8_t *)"es-mx", 5, true, NULL };
                if (hxc_string_retain(hxc_l_locale1_hbaafb99b1676) != HXC_STATUS_OK)
                {
                  abort();
                }
                break;
              }
            default:
              {
                hxc_l_locale1_hbaafb99b1676 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
                if (hxc_string_retain(hxc_l_locale1_hbaafb99b1676) != HXC_STATUS_OK)
                {
                  abort();
                }
                break;
              }
          }
          hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n1274 = hxc_l_locale1_hbaafb99b1676;
          hxc_l_tmp_managed_flow_owner_n398 = hxc_l_tmp_managed_flow_carrier_move_result_n1274;
          struct Texture hxc_l_tmp_load_result_n1275 = hxc_l_titleTexture;
          bool hxc_l_tmp_load_result_n1276 = hxc_l_titleTextureReady;
          struct Texture hxc_l_tmp_load_result_n1277 = hxc_l_wordmarkTexture;
          bool hxc_l_tmp_load_result_n1278 = hxc_l_wordmarkTextureReady;
          struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n1279 = hxc_l_selectedCampaign_h59a580bbe469;
          hxc_l_gc_roots[227] = (const void *)hxc_l_tmp_load_result_n1279;
          int32_t hxc_l_tmp_load_result_n1280 = hxc_l_locale;
          struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_tmp_load_result_n1281 = hxc_l_uiCatalog;
          hxc_l_gc_roots[228] = (const void *)hxc_l_tmp_load_result_n1281;
          int32_t hxc_l_tmp_load_result_n1282 = hxc_l_selectedCampaignLevelIndex;
          hxc_string hxc_l_tmp_load_result_n1283 = hxc_l_tmp;
          struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1284 = hxc_l_levelView;
          hxc_l_gc_roots[229] = (const void *)hxc_l_tmp_load_result_n1284;
          if (hxc_l_tmp_load_result_n1284 == NULL)
          {
            abort();
          }
          struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_class_field_load_result_n1285 = (*hxc_l_tmp_load_result_n1284).hxc_levelPresentation;
          hxc_l_gc_roots[230] = (const void *)hxc_l_tmp_class_field_load_result_n1285;
          if (hxc_l_tmp_class_field_load_result_n1285 == NULL)
          {
            abort();
          }
          hxc_string hxc_l_tmp_instance_call_result_n1287 = hxc_caxecraft_content_RuntimeLevelPresentation_message(hxc_l_tmp_class_field_load_result_n1285, (hxc_string){ (const uint8_t *)"adventure_tagline", 17, true, NULL }, hxc_l_tmp_managed_flow_owner_n398);
          hxc_l_tmp_static_call_argument_9_owner_n399 = hxc_l_tmp_instance_call_result_n1287;
          hxc_caxecraft_app_CampaignMenu_draw(hxc_l_tmp_load_result_n1275, hxc_l_tmp_load_result_n1276, hxc_l_tmp_load_result_n1277, hxc_l_tmp_load_result_n1278, hxc_l_tmp_load_result_n1279, hxc_l_tmp_load_result_n1280, hxc_l_tmp_load_result_n1281, hxc_l_tmp_load_result_n1282, hxc_l_tmp_load_result_n1283, hxc_l_tmp_static_call_argument_9_owner_n399);
          if (hxc_string_release(&hxc_l_tmp_static_call_argument_9_owner_n399) != HXC_STATUS_OK)
          {
            abort();
          }
          if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n398) != HXC_STATUS_OK)
          {
            abort();
          }
          if (hxc_string_release(&hxc_l_tmp) != HXC_STATUS_OK)
          {
            abort();
          }
          if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n394) != HXC_STATUS_OK)
          {
            abort();
          }
        }
      }
      else
      {
        if (hxc_l_onEditor)
        {
          struct hxc_caxecraft_app_CaxecraftEditorScreen *hxc_l_tmp_load_result_n1290 = hxc_l_editorScreen;
          hxc_l_gc_roots[231] = (const void *)hxc_l_tmp_load_result_n1290;
          if (hxc_l_tmp_load_result_n1290 == NULL)
          {
            abort();
          }
          int32_t hxc_l_tmp_load_result_n1291 = hxc_l_locale;
          enum hxc_caxecraft_app_EditorScreenAction hxc_l_tmp_instance_call_result_n1293 = hxc_caxecraft_app_CaxecraftEditorScreen_draw(hxc_l_tmp_load_result_n1290, hxc_l_tmp_load_result_n1291, hxc_l_editorNavigationCommand);
          if (hxc_l_tmp_instance_call_result_n1293 == hxc_caxecraft_app_EditorScreenAction_ReturnToTitle)
          {
            enum hxc_caxecraft_app_AppScreen hxc_l_tmp_call_result_n1296 = hxc_caxecraft_app_AppScreen_closeEditor(hxc_l_screen);
            hxc_l_screen = hxc_l_tmp_call_result_n1296;
          }
        }
        else
        {
          ClearBackground((struct Color){ .r = 126, .g = 190, .b = 201, .a = 255 });
          int32_t hxc_l_tmp_native_call_result_n1298 = GetScreenWidth();
          int32_t hxc_l_sunX = hxc_i32_subtract_wrapping((int32_t)hxc_l_tmp_native_call_result_n1298, 260);
          float hxc_l_radius_ha565f416ed24 = (float)42.0;
          struct Color hxc_l_this1_h0ec82fa286d4 = (struct Color){ .r = 255, .g = 226, .b = 136, .a = 255 };
          int32_t hxc_l_tmp_load_result_n1300 = hxc_l_sunX;
          float hxc_l_tmp_load_result_n1301 = hxc_l_radius_ha565f416ed24;
          DrawCircle((int32_t)hxc_l_tmp_load_result_n1300, (int32_t)86, hxc_l_tmp_load_result_n1301, hxc_l_this1_h0ec82fa286d4);
          float hxc_l_radius_h85473ce39b9c = (float)30.0;
          struct Color hxc_l_this1_hfc3d72dd72b5 = (struct Color){ .r = 255, .g = 246, .b = 205, .a = 255 };
          int32_t hxc_l_tmp_load_result_n1304 = hxc_l_sunX;
          float hxc_l_tmp_load_result_n1305 = hxc_l_radius_h85473ce39b9c;
          DrawCircle((int32_t)hxc_l_tmp_load_result_n1304, (int32_t)86, hxc_l_tmp_load_result_n1305, hxc_l_this1_hfc3d72dd72b5);
          BeginMode3D(hxc_l_camera);
          if (hxc_l_self == NULL)
          {
            abort();
          }
          struct hxc_caxecraft_app_TerrainRenderer *hxc_l_tmp_owned_class_field_address_n1308 = &(*hxc_l_self).hxc_terrainRenderer;
          if (hxc_l_tmp_owned_class_field_address_n1308 == NULL)
          {
            abort();
          }
          struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1309 = hxc_l_session;
          hxc_l_gc_roots[232] = (const void *)hxc_l_tmp_load_result_n1309;
          if (hxc_l_tmp_load_result_n1309 == NULL)
          {
            abort();
          }
          size_t hxc_l_tmp_length_n1311;
          const uint8_t *hxc_l_tmp_instance_call_result_n1310 = hxc_caxecraft_domain_GameSession_worldView(hxc_l_tmp_load_result_n1309, &hxc_l_tmp_length_n1311);
          struct Texture hxc_l_tmp_load_result_n1312 = hxc_l_terrainTexture;
          bool hxc_l_tmp_load_result_n1313 = hxc_l_terrainTextureReady;
          struct Texture hxc_l_tmp_load_result_n1314 = hxc_l_adventureTerrainTexture;
          bool hxc_l_tmp_load_result_n1315 = hxc_l_adventureTerrainTextureReady;
          double hxc_l_tmp_record_field_load_result_n1316 = hxc_l_renderPosition.hxc_x;
          struct hxc_caxecraft_app_TerrainRenderCounters hxc_l_tmp_instance_call_result_n1318 = hxc_caxecraft_app_TerrainRenderer_draw(hxc_l_tmp_owned_class_field_address_n1308, hxc_l_tmp_instance_call_result_n1310, hxc_l_tmp_length_n1311, hxc_l_tmp_load_result_n1312, hxc_l_tmp_load_result_n1313, hxc_l_tmp_load_result_n1314, hxc_l_tmp_load_result_n1315, hxc_l_tmp_record_field_load_result_n1316, hxc_l_renderPosition.hxc_z);
          struct hxc_caxecraft_app_TerrainRenderCounters hxc_l_renderCounters = hxc_l_tmp_instance_call_result_n1318;
          struct hxc_caxecraft_app_WaterRenderer *hxc_l_tmp_owned_class_field_address_n1319 = &(*hxc_l_self).hxc_waterRenderer;
          if (hxc_l_tmp_owned_class_field_address_n1319 == NULL)
          {
            abort();
          }
          struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1320 = hxc_l_session;
          hxc_l_gc_roots[233] = (const void *)hxc_l_tmp_load_result_n1320;
          if (hxc_l_tmp_load_result_n1320 == NULL)
          {
            abort();
          }
          size_t hxc_l_tmp_length_n1322;
          const uint8_t *hxc_l_tmp_instance_call_result_n1321 = hxc_caxecraft_domain_GameSession_worldView(hxc_l_tmp_load_result_n1320, &hxc_l_tmp_length_n1322);
          struct Texture hxc_l_tmp_load_result_n1323 = hxc_l_terrainTexture;
          bool hxc_l_tmp_load_result_n1324 = hxc_l_terrainTextureReady;
          struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1325 = hxc_l_levelView;
          hxc_l_gc_roots[234] = (const void *)hxc_l_tmp_load_result_n1325;
          if (hxc_l_tmp_load_result_n1325 == NULL)
          {
            abort();
          }
          int32_t hxc_l_tmp_class_field_load_result_n1326 = (*hxc_l_tmp_load_result_n1325).hxc_waterCell;
          double hxc_l_tmp_load_result_n1327 = hxc_l_eyeX;
          double hxc_l_tmp_load_result_n1328 = hxc_l_eyeY;
          struct hxc_caxecraft_app_WaterRenderCounters hxc_l_tmp_instance_call_result_n1330 = hxc_caxecraft_app_WaterRenderer_draw(hxc_l_tmp_owned_class_field_address_n1319, hxc_l_tmp_instance_call_result_n1321, hxc_l_tmp_length_n1322, hxc_l_tmp_load_result_n1323, hxc_l_tmp_load_result_n1324, hxc_l_tmp_class_field_load_result_n1326, hxc_l_tmp_load_result_n1327, hxc_l_tmp_load_result_n1328, hxc_l_eyeZ);
          struct hxc_caxecraft_app_WaterRenderCounters hxc_l_waterCounters = hxc_l_tmp_instance_call_result_n1330;
          int32_t hxc_l_tmp_record_field_load_result_n1331 = hxc_l_renderCounters.hxc_visible;
          int32_t hxc_l_totalVisible = hxc_i32_add_wrapping(hxc_l_tmp_record_field_load_result_n1331, hxc_l_waterCounters.hxc_visible);
          int32_t hxc_l_tmp_record_field_load_result_n1333 = hxc_l_renderCounters.hxc_drawCalls;
          int32_t hxc_l_totalDrawCalls = hxc_i32_add_wrapping(hxc_l_tmp_record_field_load_result_n1333, hxc_l_waterCounters.hxc_drawCalls);
          struct Camera3D hxc_l_tmp_load_result_n1335 = hxc_l_camera;
          struct Texture hxc_l_tmp_load_result_n1336 = hxc_l_entityTexture;
          bool hxc_l_tmp_load_result_n1337 = hxc_l_entityTextureReady;
          struct hxc_caxecraft_domain_Character hxc_l_tmp_load_result_n1338 = hxc_l_dialogueActor;
          struct hxc_caxecraft_domain_Character hxc_l_tmp_load_result_n1339 = hxc_l_enemyActor;
          struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1340 = hxc_l_levelView;
          hxc_l_gc_roots[235] = (const void *)hxc_l_tmp_load_result_n1340;
          if (hxc_l_tmp_load_result_n1340 == NULL)
          {
            abort();
          }
          int32_t hxc_l_tmp_class_field_load_result_n1341 = (*hxc_l_tmp_load_result_n1340).hxc_dialogueCell;
          struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1342 = hxc_l_levelView;
          hxc_l_gc_roots[236] = (const void *)hxc_l_tmp_load_result_n1342;
          if (hxc_l_tmp_load_result_n1342 == NULL)
          {
            abort();
          }
          int32_t hxc_l_tmp_class_field_load_result_n1343 = (*hxc_l_tmp_load_result_n1342).hxc_enemyCell;
          enum hxc_caxecraft_domain_ActorControllerPhase hxc_l_tmp_record_field_load_result_n1344 = hxc_l_enemyPhase_h70382f3bdc66.hxc_phase;
          hxc_caxecraft_app_CaxecraftApp_drawActors(hxc_l_tmp_load_result_n1335, hxc_l_tmp_load_result_n1336, hxc_l_tmp_load_result_n1337, hxc_l_tmp_load_result_n1338, hxc_l_tmp_load_result_n1339, hxc_l_tmp_class_field_load_result_n1341, hxc_l_tmp_class_field_load_result_n1343, hxc_l_tmp_record_field_load_result_n1344, hxc_l_berryDrop);
          struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_tmp_load_result_n1346 = hxc_l_contentRegistry;
          hxc_l_gc_roots[237] = (const void *)hxc_l_tmp_load_result_n1346;
          struct Camera3D hxc_l_tmp_load_result_n1347 = hxc_l_camera;
          struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1348 = hxc_l_session;
          hxc_l_gc_roots[238] = (const void *)hxc_l_tmp_load_result_n1348;
          if (hxc_l_tmp_load_result_n1348 == NULL)
          {
            abort();
          }
          size_t hxc_l_tmp_length_n1350;
          const int32_t *hxc_l_tmp_instance_call_result_n1349 = hxc_caxecraft_domain_GameSession_authoredItemsView(hxc_l_tmp_load_result_n1348, &hxc_l_tmp_length_n1350);
          struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1351 = hxc_l_levelView;
          hxc_l_gc_roots[239] = (const void *)hxc_l_tmp_load_result_n1351;
          struct Texture hxc_l_tmp_load_result_n1352 = hxc_l_itemTexture;
          bool hxc_l_tmp_load_result_n1353 = hxc_l_itemTextureReady;
          struct Texture hxc_l_tmp_load_result_n1354 = hxc_l_adventureItemTexture;
          hxc_caxecraft_app_AuthoredItemRenderer_drawWorldItems(hxc_l_tmp_load_result_n1346, hxc_l_tmp_load_result_n1347, hxc_l_tmp_instance_call_result_n1349, hxc_l_tmp_length_n1350, hxc_l_tmp_load_result_n1351, hxc_l_tmp_load_result_n1352, hxc_l_tmp_load_result_n1353, hxc_l_tmp_load_result_n1354, hxc_l_adventureItemTextureReady);
          if (hxc_l_hit.hxc_hit)
          {
            double hxc_l_y = (double)hxc_l_hit.hxc_cellY + 0.5;
            double hxc_l_z = (double)hxc_l_hit.hxc_cellZ + 0.5;
            int32_t hxc_l_tmp_record_field_load_result_n1359 = hxc_l_hit.hxc_cellX;
            double hxc_l_tmp_load_result_n1360 = hxc_l_y;
            DrawCubeWires((struct Vector3){ .x = (float)((double)hxc_l_tmp_record_field_load_result_n1359 + 0.5), .y = (float)hxc_l_tmp_load_result_n1360, .z = (float)hxc_l_z }, (float)1.04, (float)1.04, (float)1.04, (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 });
          }
          EndMode3D();
          if (hxc_l_cameraWaterBlend > 0.0)
          {
            int32_t hxc_l_overlayAlpha = hxc_f64_to_i32_saturating(105.0 * hxc_l_cameraWaterBlend);
            int32_t hxc_l_tmp_native_call_result_n1366 = GetScreenWidth();
            int32_t hxc_l_width_h43c4b2745294 = (int32_t)hxc_l_tmp_native_call_result_n1366;
            int32_t hxc_l_tmp_native_call_result_n1367 = GetScreenHeight();
            int32_t hxc_l_height_hb169e9513548 = (int32_t)hxc_l_tmp_native_call_result_n1367;
            uint8_t hxc_l_red = (uint8_t)31;
            uint8_t hxc_l_green = (uint8_t)115;
            uint8_t hxc_l_blue = (uint8_t)154;
            int32_t hxc_l_tmp_load_result_n1368 = hxc_l_overlayAlpha;
            int32_t hxc_l_tmp_conditional_result_n418 = 0;
            if (hxc_l_tmp_load_result_n1368 < 0)
            {
              hxc_l_tmp_conditional_result_n418 = 0;
            }
            else
            {
              int32_t hxc_l_tmp_load_result_n1369 = hxc_l_overlayAlpha;
              int32_t hxc_l_tmp_conditional_result_n419 = 0;
              if (hxc_l_tmp_load_result_n1369 > 255)
              {
                hxc_l_tmp_conditional_result_n419 = 255;
              }
              else
              {
                hxc_l_tmp_conditional_result_n419 = hxc_l_overlayAlpha;
              }
              hxc_l_tmp_conditional_result_n418 = hxc_l_tmp_conditional_result_n419;
            }
            uint8_t hxc_l_alpha = (uint8_t)hxc_l_tmp_conditional_result_n418;
            uint8_t hxc_l_tmp_load_result_n1373 = hxc_l_red;
            uint8_t hxc_l_tmp_load_result_n1374 = hxc_l_green;
            uint8_t hxc_l_tmp_load_result_n1375 = hxc_l_blue;
            struct Color hxc_l_this1_h654de48a250f = (struct Color){ .r = hxc_l_tmp_load_result_n1373, .g = hxc_l_tmp_load_result_n1374, .b = hxc_l_tmp_load_result_n1375, .a = hxc_l_alpha };
            int32_t hxc_l_tmp_load_result_n1378 = hxc_l_width_h43c4b2745294;
            int32_t hxc_l_tmp_load_result_n1379 = hxc_l_height_hb169e9513548;
            DrawRectangle((int32_t)0, (int32_t)0, (int32_t)hxc_l_tmp_load_result_n1378, (int32_t)hxc_l_tmp_load_result_n1379, hxc_l_this1_h654de48a250f);
          }
          double hxc_l_tmp_record_field_load_result_n1381 = hxc_l_character.hxc_body.hxc_x;
          double hxc_l_tmp_record_field_load_result_n1382 = hxc_l_character.hxc_body.hxc_z;
          struct hxc_caxecraft_domain_VitalsState hxc_l_tmp_record_field_load_result_n1383 = hxc_l_character.hxc_vitals;
          int32_t hxc_l_tmp_load_result_n1384 = hxc_l_aquaticEquipmentCode;
          int32_t hxc_l_tmp_load_result_n1385 = hxc_l_aquaticEquipmentFrames;
          bool hxc_l_tmp_record_field_load_result_n1386 = hxc_l_character.hxc_aquatic.hxc_headSubmerged;
          int32_t hxc_l_tmp_record_field_load_result_n1387 = hxc_l_character.hxc_aquatic.hxc_breathTicks;
          struct hxc_caxecraft_app_HudCharacterView hxc_l_hudView_hbd7f23c4712c = (struct hxc_caxecraft_app_HudCharacterView){ .hxc_aquaticEquipmentCode = hxc_l_tmp_load_result_n1384, .hxc_aquaticEquipmentVisible = hxc_l_tmp_load_result_n1385 > 0, .hxc_breathTicks = hxc_l_tmp_record_field_load_result_n1387, .hxc_headSubmerged = hxc_l_tmp_record_field_load_result_n1386, .hxc_maximumBreathTicks = hxc_l_character.hxc_aquaticProfile.hxc_maximumBreathTicks, .hxc_vitals = hxc_l_tmp_record_field_load_result_n1383, .hxc_x = hxc_l_tmp_record_field_load_result_n1381, .hxc_z = hxc_l_tmp_record_field_load_result_n1382 };
          bool hxc_l_hudView1 = hxc_l_placementBlockedFrames > 0;
          bool hxc_l_hudView2 = hxc_l_strikeHitFrames > 0;
          bool hxc_l_hudView3 = hxc_l_enemyDefeatedFrames > 0;
          bool hxc_l_hudView4 = hxc_l_enemyAttackFrames > 0;
          bool hxc_l_hudView5 = hxc_l_pickupFrames > 0;
          bool hxc_l_hudView6 = hxc_l_recoveryFeedbackFrames > 0;
          enum hxc_caxecraft_domain_ActorControllerPhase hxc_l_enemyPhase_h9e2fa2329ece = hxc_l_enemyPhase_h70382f3bdc66.hxc_phase;
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
          hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n1398 = hxc_l_locale1_hab7686f0e894;
          hxc_l_tmp_managed_flow_owner_n430 = hxc_l_tmp_managed_flow_carrier_move_result_n1398;
          struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1399 = hxc_l_levelView;
          hxc_l_gc_roots[240] = (const void *)hxc_l_tmp_load_result_n1399;
          if (hxc_l_tmp_load_result_n1399 == NULL)
          {
            abort();
          }
          struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_class_field_load_result_n1400 = (*hxc_l_tmp_load_result_n1399).hxc_levelPresentation;
          hxc_l_gc_roots[241] = (const void *)hxc_l_tmp_class_field_load_result_n1400;
          if (hxc_l_tmp_class_field_load_result_n1400 == NULL)
          {
            abort();
          }
          hxc_string hxc_l_tmp_load_result_n1401 = hxc_l_currentObjectiveId;
          hxc_string hxc_l_tmp_instance_call_result_n1403 = hxc_caxecraft_content_RuntimeLevelPresentation_objectiveTitle(hxc_l_tmp_class_field_load_result_n1400, hxc_l_tmp_load_result_n1401, hxc_l_tmp_managed_flow_owner_n430);
          hxc_l_hudView7 = hxc_l_tmp_instance_call_result_n1403;
          int32_t hxc_l_tmp_load_result_n1404 = hxc_l_totalVisible;
          int32_t hxc_l_tmp_load_result_n1405 = hxc_l_totalDrawCalls;
          int32_t hxc_l_tmp_load_result_n1406 = hxc_l_frameCount;
          int32_t hxc_l_tmp_load_result_n1407 = hxc_l_completedTicks;
          struct hxc_caxecraft_app_HudCharacterView hxc_l_tmp_load_result_n1409 = hxc_l_hudView_hbd7f23c4712c;
          bool hxc_l_tmp_load_result_n1410 = hxc_l_hudView1;
          bool hxc_l_tmp_load_result_n1411 = hxc_l_hudView2;
          bool hxc_l_tmp_load_result_n1412 = hxc_l_hudView3;
          bool hxc_l_tmp_load_result_n1413 = hxc_l_hudView4;
          bool hxc_l_tmp_load_result_n1414 = hxc_l_hudView5;
          int32_t hxc_l_tmp_load_result_n1415 = hxc_l_pickupAmount;
          int32_t hxc_l_tmp_load_result_n1416 = hxc_l_inventoryFullReason;
          int32_t hxc_l_tmp_load_result_n1417 = hxc_l_recoveryFeedback;
          bool hxc_l_tmp_load_result_n1418 = hxc_l_hudView6;
          bool hxc_l_tmp_load_result_n1420 = hxc_l_paused_hfac44b03fb01;
          bool hxc_l_tmp_load_result_n1421 = hxc_l_captured_h8194fa734f34;
          struct hxc_caxecraft_domain_RaycastHit hxc_l_tmp_load_result_n1422 = hxc_l_hit;
          int32_t hxc_l_tmp_load_result_n1423 = hxc_l_selectedMode;
          int32_t hxc_l_tmp_load_result_n1424 = hxc_l_locale;
          struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_load_result_n1425 = hxc_l_inventory;
          hxc_string hxc_l_tmp_record_field_activeDialogue_owner_n433 = hxc_l_activeDialogue;
          if (hxc_string_retain(hxc_l_tmp_record_field_activeDialogue_owner_n433) != HXC_STATUS_OK)
          {
            abort();
          }
          hxc_string hxc_l_tmp_record_field_activeDialogue_owned_load_result_n1427 = hxc_l_tmp_record_field_activeDialogue_owner_n433;
          bool hxc_l_tmp_load_result_n1428 = hxc_l_guideInteractionAvailable;
          struct hxc_caxecraft_domain_Character hxc_l_tmp_load_result_n1429 = hxc_l_enemyActor;
          enum hxc_caxecraft_domain_ActorControllerPhase hxc_l_tmp_load_result_n1430 = hxc_l_enemyPhase_h9e2fa2329ece;
          hxc_string hxc_l_tmp_record_field_levelLabel_owner_n434 = hxc_l_levelLabel;
          if (hxc_string_retain(hxc_l_tmp_record_field_levelLabel_owner_n434) != HXC_STATUS_OK)
          {
            abort();
          }
          hxc_string hxc_l_tmp_record_field_levelLabel_owned_load_result_n1432 = hxc_l_tmp_record_field_levelLabel_owner_n434;
          hxc_string hxc_l_tmp_record_field_objectiveTitle_owner_n435 = hxc_l_hudView7;
          if (hxc_string_retain(hxc_l_tmp_record_field_objectiveTitle_owner_n435) != HXC_STATUS_OK)
          {
            abort();
          }
          hxc_string hxc_l_tmp_record_field_objectiveTitle_owned_load_result_n1434 = hxc_l_tmp_record_field_objectiveTitle_owner_n435;
          struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1435 = hxc_l_levelView;
          hxc_l_gc_roots[242] = (const void *)hxc_l_tmp_load_result_n1435;
          if (hxc_l_tmp_load_result_n1435 == NULL)
          {
            abort();
          }
          struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_class_field_load_result_n1436 = (*hxc_l_tmp_load_result_n1435).hxc_levelPresentation;
          hxc_l_gc_roots[243] = (const void *)hxc_l_tmp_class_field_load_result_n1436;
          hxc_l_gc_roots[244] = (const void *)(struct hxc_caxecraft_app_HudView){ .hxc_activeDialogue = hxc_l_tmp_record_field_activeDialogue_owned_load_result_n1427, .hxc_character = hxc_l_tmp_load_result_n1409, .hxc_enemy = hxc_l_tmp_load_result_n1429, .hxc_enemyPhase = hxc_l_tmp_load_result_n1430, .hxc_feedback = (struct hxc_caxecraft_app_HudFeedbackView){ .hxc_enemyAttacked = hxc_l_tmp_load_result_n1413, .hxc_enemyDefeated = hxc_l_tmp_load_result_n1412, .hxc_inventoryFullReason = hxc_l_tmp_load_result_n1416, .hxc_pickedUp = hxc_l_tmp_load_result_n1414, .hxc_pickupAmount = hxc_l_tmp_load_result_n1415, .hxc_placementBlocked = hxc_l_tmp_load_result_n1410, .hxc_recoveryDecision = hxc_l_tmp_load_result_n1417, .hxc_recoveryVisible = hxc_l_tmp_load_result_n1418, .hxc_strikeHit = hxc_l_tmp_load_result_n1411 }, .hxc_guideInteractionAvailable = hxc_l_tmp_load_result_n1428, .hxc_hit = hxc_l_tmp_load_result_n1422, .hxc_inventory = hxc_l_tmp_load_result_n1425, .hxc_levelLabel = hxc_l_tmp_record_field_levelLabel_owned_load_result_n1432, .hxc_locale = hxc_l_tmp_load_result_n1424, .hxc_metrics = (struct hxc_caxecraft_app_HudMetricsView){ .hxc_completedTicks = hxc_l_tmp_load_result_n1407, .hxc_drawCalls = hxc_l_tmp_load_result_n1405, .hxc_renderedFrames = hxc_l_tmp_load_result_n1406, .hxc_visibleBlocks = hxc_l_tmp_load_result_n1404 }, .hxc_mode = hxc_l_tmp_load_result_n1423, .hxc_objectiveTitle = hxc_l_tmp_record_field_objectiveTitle_owned_load_result_n1434, .hxc_paused = hxc_l_tmp_load_result_n1420, .hxc_pointerCaptured = hxc_l_tmp_load_result_n1421, .hxc_presentation = hxc_l_tmp_class_field_load_result_n1436 }.hxc_presentation;
          hxc_l_hudView_h9b78e944debd = (struct hxc_caxecraft_app_HudView){ .hxc_activeDialogue = hxc_l_tmp_record_field_activeDialogue_owned_load_result_n1427, .hxc_character = hxc_l_tmp_load_result_n1409, .hxc_enemy = hxc_l_tmp_load_result_n1429, .hxc_enemyPhase = hxc_l_tmp_load_result_n1430, .hxc_feedback = (struct hxc_caxecraft_app_HudFeedbackView){ .hxc_enemyAttacked = hxc_l_tmp_load_result_n1413, .hxc_enemyDefeated = hxc_l_tmp_load_result_n1412, .hxc_inventoryFullReason = hxc_l_tmp_load_result_n1416, .hxc_pickedUp = hxc_l_tmp_load_result_n1414, .hxc_pickupAmount = hxc_l_tmp_load_result_n1415, .hxc_placementBlocked = hxc_l_tmp_load_result_n1410, .hxc_recoveryDecision = hxc_l_tmp_load_result_n1417, .hxc_recoveryVisible = hxc_l_tmp_load_result_n1418, .hxc_strikeHit = hxc_l_tmp_load_result_n1411 }, .hxc_guideInteractionAvailable = hxc_l_tmp_load_result_n1428, .hxc_hit = hxc_l_tmp_load_result_n1422, .hxc_inventory = hxc_l_tmp_load_result_n1425, .hxc_levelLabel = hxc_l_tmp_record_field_levelLabel_owned_load_result_n1432, .hxc_locale = hxc_l_tmp_load_result_n1424, .hxc_metrics = (struct hxc_caxecraft_app_HudMetricsView){ .hxc_completedTicks = hxc_l_tmp_load_result_n1407, .hxc_drawCalls = hxc_l_tmp_load_result_n1405, .hxc_renderedFrames = hxc_l_tmp_load_result_n1406, .hxc_visibleBlocks = hxc_l_tmp_load_result_n1404 }, .hxc_mode = hxc_l_tmp_load_result_n1423, .hxc_objectiveTitle = hxc_l_tmp_record_field_objectiveTitle_owned_load_result_n1434, .hxc_paused = hxc_l_tmp_load_result_n1420, .hxc_pointerCaptured = hxc_l_tmp_load_result_n1421, .hxc_presentation = hxc_l_tmp_class_field_load_result_n1436 };
          struct hxc_caxecraft_app_HudView hxc_l_tmp_load_result_n1438 = hxc_l_hudView_h9b78e944debd;
          hxc_l_gc_roots[245] = (const void *)hxc_l_tmp_load_result_n1438.hxc_presentation;
          struct hxc_caxecraft_app_HudResources hxc_l_tmp_load_result_n1439 = hxc_l_hudResources;
          struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_tmp_load_result_n1440 = hxc_l_contentRegistry;
          hxc_l_gc_roots[246] = (const void *)hxc_l_tmp_load_result_n1440;
          hxc_l_gc_roots[247] = (const void *)hxc_l_uiCatalog;
          hxc_caxecraft_app_CaxecraftApp_drawHud(hxc_l_tmp_load_result_n1438, hxc_l_tmp_load_result_n1439, hxc_l_tmp_load_result_n1440, hxc_l_uiCatalog);
          hxc_record_2a245272_destroy(&hxc_l_hudView_h9b78e944debd);
          if (hxc_string_release(&hxc_l_hudView7) != HXC_STATUS_OK)
          {
            abort();
          }
          if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n430) != HXC_STATUS_OK)
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
  if (hxc_string_release(&hxc_l_activeDialogue) != HXC_STATUS_OK)
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
