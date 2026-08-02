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
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n52 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n67 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n70 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n77 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n78 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n80 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n81 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n82 = { 0 };
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
  struct Color hxc_l_this1_hb3d63b4308c2 = (struct Color){ .r = 15, .g = 27, .b = 39, .a = 220 };
  DrawRectangle((int32_t)18, (int32_t)18, (int32_t)460, (int32_t)108, hxc_l_this1_hb3d63b4308c2);
  struct Color hxc_l_this1_h723838316b81 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
  DrawRectangleLines((int32_t)18, (int32_t)18, (int32_t)460, (int32_t)108, hxc_l_this1_h723838316b81);
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
  hxc_string hxc_l_tmp_instance_call_result_n89 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_uiCatalog, hxc_l_locale, 4);
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n47 = hxc_l_tmp_instance_call_result_n89;
  hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n91 = HXC_BORROWED_CSTRING_INITIALIZER;
  if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n47, &hxc_l_tmp_string_borrow_cstring_result_n91) != HXC_STATUS_OK)
  {
    abort();
  }
  DrawText(hxc_l_tmp_string_borrow_cstring_result_n91.data, (int32_t)32, (int32_t)58, (int32_t)14, hxc_l_text);
  hxc_caxecraft_app_HudDigits_drawNumber(16384, 82, 59, 5, (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 });
  hxc_string hxc_l_tmp_instance_call_result_n95 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_uiCatalog, hxc_l_locale, 8);
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n48 = hxc_l_tmp_instance_call_result_n95;
  hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n97 = HXC_BORROWED_CSTRING_INITIALIZER;
  if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n48, &hxc_l_tmp_string_borrow_cstring_result_n97) != HXC_STATUS_OK)
  {
    abort();
  }
  DrawText(hxc_l_tmp_string_borrow_cstring_result_n97.data, (int32_t)160, (int32_t)58, (int32_t)14, hxc_l_text);
  hxc_caxecraft_app_HudDigits_drawNumber(hxc_l_visible, 230, 59, 5, (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 });
  hxc_string hxc_l_tmp_instance_call_result_n102 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_uiCatalog, hxc_l_locale, 5);
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n49 = hxc_l_tmp_instance_call_result_n102;
  hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n104 = HXC_BORROWED_CSTRING_INITIALIZER;
  if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n49, &hxc_l_tmp_string_borrow_cstring_result_n104) != HXC_STATUS_OK)
  {
    abort();
  }
  DrawText(hxc_l_tmp_string_borrow_cstring_result_n104.data, (int32_t)326, (int32_t)58, (int32_t)14, hxc_l_text);
  hxc_caxecraft_app_HudDigits_drawNumber(hxc_l_drawCalls, 382, 59, 5, (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 });
  hxc_string hxc_l_tmp_instance_call_result_n109 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_uiCatalog, hxc_l_locale, 6);
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n50 = hxc_l_tmp_instance_call_result_n109;
  hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n111 = HXC_BORROWED_CSTRING_INITIALIZER;
  if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n50, &hxc_l_tmp_string_borrow_cstring_result_n111) != HXC_STATUS_OK)
  {
    abort();
  }
  DrawText(hxc_l_tmp_string_borrow_cstring_result_n111.data, (int32_t)32, (int32_t)86, (int32_t)12, hxc_l_text);
  int32_t hxc_l_tmp_load_result_n113 = hxc_l_frames;
  hxc_caxecraft_app_HudDigits_drawNumber(hxc_l_tmp_load_result_n113, 82, 85, 6, hxc_l_text);
  hxc_string hxc_l_tmp_instance_call_result_n116 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_uiCatalog, hxc_l_locale, 7);
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n51 = hxc_l_tmp_instance_call_result_n116;
  hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n118 = HXC_BORROWED_CSTRING_INITIALIZER;
  if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n51, &hxc_l_tmp_string_borrow_cstring_result_n118) != HXC_STATUS_OK)
  {
    abort();
  }
  DrawText(hxc_l_tmp_string_borrow_cstring_result_n118.data, (int32_t)174, (int32_t)86, (int32_t)12, hxc_l_text);
  int32_t hxc_l_tmp_load_result_n120 = hxc_l_updates;
  hxc_caxecraft_app_HudDigits_drawNumber(hxc_l_tmp_load_result_n120, 216, 85, 6, hxc_l_text);
  struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_load_result_n122 = hxc_l_inventory;
  struct Texture hxc_l_tmp_load_result_n123 = hxc_l_hudTexture;
  bool hxc_l_tmp_load_result_n124 = hxc_l_hudTextureReady;
  struct Texture hxc_l_tmp_load_result_n125 = hxc_l_itemTexture;
  bool hxc_l_tmp_load_result_n126 = hxc_l_itemTextureReady;
  int32_t hxc_l_tmp_load_result_n127 = hxc_l_width;
  hxc_caxecraft_app_CaxecraftApp_drawHotbar(hxc_l_tmp_load_result_n122, hxc_l_tmp_load_result_n123, hxc_l_tmp_load_result_n124, hxc_l_tmp_load_result_n125, hxc_l_tmp_load_result_n126, hxc_l_tmp_load_result_n127, hxc_l_height);
  struct hxc_caxecraft_domain_VitalsState hxc_l_tmp_load_result_n129 = hxc_l_vitals;
  struct Texture hxc_l_tmp_load_result_n130 = hxc_l_hudTexture;
  bool hxc_l_tmp_load_result_n131 = hxc_l_hudTextureReady;
  hxc_caxecraft_app_CaxecraftApp_drawHealth(hxc_l_tmp_load_result_n129, hxc_l_tmp_load_result_n130, hxc_l_tmp_load_result_n131, hxc_l_width);
  if (hxc_l_aquaticEquipmentCode >= 0)
  {
    int32_t hxc_l_tmp_load_result_n134 = hxc_l_aquaticEquipmentCode;
    struct Texture hxc_l_tmp_load_result_n135 = hxc_l_itemTexture;
    bool hxc_l_tmp_load_result_n136 = hxc_l_itemTextureReady;
    struct Texture hxc_l_tmp_load_result_n137 = hxc_l_adventureItemTexture;
    bool hxc_l_tmp_load_result_n138 = hxc_l_adventureItemTextureReady;
    hxc_caxecraft_app_AuthoredItemRenderer_drawEquippedIcon(hxc_l_contentRegistry, hxc_l_tmp_load_result_n134, hxc_l_tmp_load_result_n135, hxc_l_tmp_load_result_n136, hxc_l_tmp_load_result_n137, hxc_l_tmp_load_result_n138, hxc_i32_subtract_wrapping(hxc_l_width, 226), 18, 42);
  }
  if (hxc_l_headSubmerged)
  {
    int32_t hxc_l_tmp_load_result_n141 = hxc_l_breathTicks;
    int32_t hxc_l_tmp_load_result_n142 = hxc_l_maximumBreathTicks;
    int32_t hxc_l_tmp_load_result_n143 = hxc_l_width;
    hxc_caxecraft_app_CaxecraftApp_drawBreath(hxc_l_tmp_load_result_n141, hxc_l_tmp_load_result_n142, hxc_l_tmp_load_result_n143, hxc_l_height);
  }
  hxc_string hxc_l_tmp_instance_call_result_n146 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_uiCatalog, hxc_l_locale, 3);
  hxc_l_tmp_cstring_ref_owner_owner_n52 = hxc_l_tmp_instance_call_result_n146;
  hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n148 = HXC_BORROWED_CSTRING_INITIALIZER;
  if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n52, &hxc_l_tmp_string_borrow_cstring_result_n148) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_tmp_load_result_n149 = hxc_l_height;
  DrawText(hxc_l_tmp_string_borrow_cstring_result_n148.data, (int32_t)20, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n149, 22), (int32_t)14, hxc_l_text);
  if (hxc_l_mode == 1)
  {
    struct Color hxc_l_this1_hc1d3703fa81b = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
    const char *hxc_l_tmp_call_result_n154 = hxc_caxecraft_localization_FirstPlayableCatalog_text(hxc_l_locale, 0);
    DrawText(hxc_l_tmp_call_result_n154, (int32_t)32, (int32_t)110, (int32_t)14, hxc_l_this1_hc1d3703fa81b);
  }
  if (hxc_l_guideInteractionAvailable)
  {
    struct Color hxc_l_this1_hb7f5da12dd81 = (struct Color){ .r = 15, .g = 27, .b = 39, .a = 220 };
    int32_t hxc_l_tmp_load_result_n158 = hxc_l_centerX;
    int32_t hxc_l_tmp_load_result_n159 = hxc_l_centerY;
    DrawRectangle((int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n158, 260), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n159, 54), (int32_t)520, (int32_t)60, hxc_l_this1_hb7f5da12dd81);
    if (hxc_l_guidePhase == hxc_caxecraft_gameplay_GuidePhase_Waiting)
    {
      const char *hxc_l_tmp_call_result_n164 = hxc_caxecraft_localization_FirstPlayableCatalog_text(hxc_l_locale, 13);
      int32_t hxc_l_tmp_load_result_n165 = hxc_l_centerX;
      int32_t hxc_l_tmp_load_result_n166 = hxc_l_centerY;
      DrawText(hxc_l_tmp_call_result_n164, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n165, 110), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n166, 74), (int32_t)18, hxc_l_text);
    }
    else
    {
      if (hxc_l_guidePhase == hxc_caxecraft_gameplay_GuidePhase_Welcomed)
      {
        const char *hxc_l_tmp_call_result_n171 = hxc_caxecraft_localization_FirstPlayableCatalog_text(hxc_l_locale, 14);
        int32_t hxc_l_tmp_load_result_n172 = hxc_l_centerX;
        int32_t hxc_l_tmp_load_result_n173 = hxc_l_centerY;
        DrawText(hxc_l_tmp_call_result_n171, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n172, 225), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n173, 74), (int32_t)16, hxc_l_text);
      }
      else
      {
        const char *hxc_l_tmp_call_result_n176 = hxc_caxecraft_localization_FirstPlayableCatalog_text(hxc_l_locale, 12);
        int32_t hxc_l_tmp_load_result_n177 = hxc_l_centerX;
        int32_t hxc_l_tmp_load_result_n178 = hxc_l_centerY;
        DrawText(hxc_l_tmp_call_result_n176, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n177, 205), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n178, 74), (int32_t)16, hxc_l_text);
      }
    }
  }
  if (hxc_l_enemy.hxc_vitals.hxc_health > 0)
  {
    if (hxc_l_enemyPhase == hxc_caxecraft_domain_ActorControllerPhase_Windup)
    {
      struct Color hxc_l_this1_h64556d6c3e26 = (struct Color){ .r = 218, .g = 65, .b = 72, .a = 255 };
      const char *hxc_l_tmp_call_result_n185 = hxc_caxecraft_localization_FirstPlayableCatalog_text(hxc_l_locale, 11);
      int32_t hxc_l_tmp_load_result_n186 = hxc_l_width;
      DrawText(hxc_l_tmp_call_result_n185, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n186, 300), (int32_t)28, (int32_t)16, hxc_l_this1_h64556d6c3e26);
    }
    else
    {
      if (hxc_l_enemyPhase == hxc_caxecraft_domain_ActorControllerPhase_Chasing)
      {
        struct Color hxc_l_this1_he95c589b383c = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
        const char *hxc_l_tmp_call_result_n192 = hxc_caxecraft_localization_FirstPlayableCatalog_text(hxc_l_locale, 9);
        int32_t hxc_l_tmp_load_result_n193 = hxc_l_width;
        DrawText(hxc_l_tmp_call_result_n192, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n193, 180), (int32_t)28, (int32_t)16, hxc_l_this1_he95c589b383c);
      }
    }
  }
  if (hxc_l_strikeHit)
  {
    struct Color hxc_l_this1_hba859f95b28e = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
    const char *hxc_l_tmp_call_result_n198 = hxc_caxecraft_localization_FirstPlayableCatalog_text(hxc_l_locale, 7);
    int32_t hxc_l_tmp_load_result_n199 = hxc_l_centerX;
    int32_t hxc_l_tmp_load_result_n200 = hxc_l_centerY;
    DrawText(hxc_l_tmp_call_result_n198, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n199, 70), (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n200, 54), (int32_t)18, hxc_l_this1_hba859f95b28e);
  }
  if (hxc_l_enemyDefeated)
  {
    struct Color hxc_l_this1_h207a945b2d74 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
    const char *hxc_l_tmp_call_result_n205 = hxc_caxecraft_localization_FirstPlayableCatalog_text(hxc_l_locale, 10);
    int32_t hxc_l_tmp_load_result_n206 = hxc_l_width;
    DrawText(hxc_l_tmp_call_result_n205, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n206, 285), (int32_t)54, (int32_t)16, hxc_l_this1_h207a945b2d74);
  }
  if (hxc_l_enemyAttacked)
  {
    struct Color hxc_l_this1_h713c72543cb3 = (struct Color){ .r = 218, .g = 65, .b = 72, .a = 255 };
    const char *hxc_l_tmp_call_result_n211 = hxc_caxecraft_localization_FirstPlayableCatalog_text(hxc_l_locale, 18);
    int32_t hxc_l_tmp_load_result_n212 = hxc_l_width;
    DrawText(hxc_l_tmp_call_result_n211, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n212, 330), (int32_t)82, (int32_t)16, hxc_l_this1_h713c72543cb3);
  }
  if (hxc_l_pickedUp)
  {
    int32_t hxc_l_tmp_load_result_n215 = hxc_l_pickupAmount;
    int32_t hxc_l_tmp_conditional_result_n61 = 0;
    if (hxc_l_tmp_load_result_n215 == 1)
    {
      hxc_l_tmp_conditional_result_n61 = 2;
    }
    else
    {
      hxc_l_tmp_conditional_result_n61 = 3;
    }
    int32_t hxc_l_pickupMessage = hxc_l_tmp_conditional_result_n61;
    struct Color hxc_l_this1_h0ccbc73441ee = (struct Color){ .r = 174, .g = 78, .b = 136, .a = 255 };
    int32_t hxc_l_tmp_load_result_n218 = hxc_l_locale;
    const char *hxc_l_tmp_call_result_n220 = hxc_caxecraft_localization_FirstPlayableCatalog_text(hxc_l_tmp_load_result_n218, hxc_l_pickupMessage);
    int32_t hxc_l_tmp_load_result_n221 = hxc_l_centerX;
    int32_t hxc_l_tmp_load_result_n222 = hxc_l_centerY;
    DrawText(hxc_l_tmp_call_result_n220, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n221, 48), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n222, 24), (int32_t)18, hxc_l_this1_h0ccbc73441ee);
  }
  if (hxc_l_inventoryFullReason == 1)
  {
    struct Color hxc_l_this1_hefa279a608b4 = (struct Color){ .r = 238, .g = 113, .b = 78, .a = 255 };
    const char *hxc_l_tmp_call_result_n227 = hxc_caxecraft_localization_FirstPlayableCatalog_text(hxc_l_locale, 5);
    int32_t hxc_l_tmp_load_result_n228 = hxc_l_centerX;
    int32_t hxc_l_tmp_load_result_n229 = hxc_l_centerY;
    DrawText(hxc_l_tmp_call_result_n227, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n228, 150), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n229, 48), (int32_t)16, hxc_l_this1_hefa279a608b4);
  }
  else
  {
    if (hxc_l_inventoryFullReason == 2)
    {
      struct Color hxc_l_this1_ha7e12698826d = (struct Color){ .r = 238, .g = 113, .b = 78, .a = 255 };
      const char *hxc_l_tmp_call_result_n234 = hxc_caxecraft_localization_FirstPlayableCatalog_text(hxc_l_locale, 6);
      int32_t hxc_l_tmp_load_result_n235 = hxc_l_centerX;
      int32_t hxc_l_tmp_load_result_n236 = hxc_l_centerY;
      DrawText(hxc_l_tmp_call_result_n234, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n235, 155), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n236, 48), (int32_t)16, hxc_l_this1_ha7e12698826d);
    }
  }
  if (hxc_l_recoveryVisible)
  {
    if (hxc_l_recoveryFeedback == 1)
    {
      struct Color hxc_l_this1_h149342bdcb16 = (struct Color){ .r = 94, .g = 212, .b = 136, .a = 255 };
      const char *hxc_l_tmp_call_result_n242 = hxc_caxecraft_localization_FirstPlayableCatalog_text(hxc_l_locale, 4);
      int32_t hxc_l_tmp_load_result_n243 = hxc_l_centerX;
      int32_t hxc_l_tmp_load_result_n244 = hxc_l_centerY;
      DrawText(hxc_l_tmp_call_result_n242, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n243, 88), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n244, 24), (int32_t)18, hxc_l_this1_h149342bdcb16);
    }
    else
    {
      if (hxc_l_recoveryFeedback == 2)
      {
        struct Color hxc_l_this1_h2773fea1f79e = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
        hxc_string hxc_l_tmp_instance_call_result_n249 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_uiCatalog, hxc_l_locale, 25);
        hxc_l_tmp_cstring_ref_owner_owner_n67 = hxc_l_tmp_instance_call_result_n249;
        hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n251 = HXC_BORROWED_CSTRING_INITIALIZER;
        if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n67, &hxc_l_tmp_string_borrow_cstring_result_n251) != HXC_STATUS_OK)
        {
          abort();
        }
        int32_t hxc_l_tmp_load_result_n252 = hxc_l_centerX;
        int32_t hxc_l_tmp_load_result_n253 = hxc_l_centerY;
        DrawText(hxc_l_tmp_string_borrow_cstring_result_n251.data, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n252, 96), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n253, 24), (int32_t)18, hxc_l_this1_h2773fea1f79e);
        if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n67) != HXC_STATUS_OK)
        {
          abort();
        }
      }
      else
      {
        if (hxc_l_recoveryFeedback == 3)
        {
          struct Color hxc_l_this1_hf53057f112b6 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
          const char *hxc_l_tmp_call_result_n258 = hxc_caxecraft_localization_FirstPlayableCatalog_text(hxc_l_locale, 15);
          int32_t hxc_l_tmp_load_result_n259 = hxc_l_centerX;
          int32_t hxc_l_tmp_load_result_n260 = hxc_l_centerY;
          DrawText(hxc_l_tmp_call_result_n258, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n259, 76), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n260, 24), (int32_t)18, hxc_l_this1_hf53057f112b6);
        }
      }
    }
  }
  if (hxc_l_aquaticEquipmentVisible)
  {
    struct Color hxc_l_this1_h7fac1ef19665 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
    hxc_string hxc_l_tmp_instance_call_result_n265 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_uiCatalog, hxc_l_locale, 0);
    hxc_l_tmp_cstring_ref_owner_owner_n70 = hxc_l_tmp_instance_call_result_n265;
    hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n267 = HXC_BORROWED_CSTRING_INITIALIZER;
    if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n70, &hxc_l_tmp_string_borrow_cstring_result_n267) != HXC_STATUS_OK)
    {
      abort();
    }
    int32_t hxc_l_tmp_load_result_n268 = hxc_l_centerX;
    int32_t hxc_l_tmp_load_result_n269 = hxc_l_centerY;
    DrawText(hxc_l_tmp_string_borrow_cstring_result_n267.data, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n268, 128), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n269, 24), (int32_t)18, hxc_l_this1_h7fac1ef19665);
    if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n70) != HXC_STATUS_OK)
    {
      abort();
    }
  }
  if (hxc_l_vitals.hxc_safeTicks > 15)
  {
    struct Color hxc_l_this1_h705a13fd9a69 = (struct Color){ .r = 218, .g = 65, .b = 72, .a = 255 };
    int32_t hxc_l_tmp_load_result_n273 = hxc_l_width;
    int32_t hxc_l_tmp_load_result_n274 = hxc_l_height;
    DrawRectangleLines((int32_t)4, (int32_t)4, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n273, 8), (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n274, 8), hxc_l_this1_h705a13fd9a69);
  }
  if (hxc_l_vitals.hxc_health <= 0)
  {
    struct Color hxc_l_this1_h41a5fa965b9b = (struct Color){ .r = 15, .g = 27, .b = 39, .a = 220 };
    int32_t hxc_l_tmp_load_result_n278 = hxc_l_centerX;
    int32_t hxc_l_tmp_load_result_n279 = hxc_l_centerY;
    DrawRectangle((int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n278, 250), (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n279, 74), (int32_t)500, (int32_t)148, hxc_l_this1_h41a5fa965b9b);
    struct Color hxc_l_this1_hd17efb02402b = (struct Color){ .r = 218, .g = 65, .b = 72, .a = 255 };
    int32_t hxc_l_tmp_load_result_n282 = hxc_l_centerX;
    int32_t hxc_l_tmp_load_result_n283 = hxc_l_centerY;
    DrawRectangleLines((int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n282, 250), (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n283, 74), (int32_t)500, (int32_t)148, hxc_l_this1_hd17efb02402b);
    const char *hxc_l_tmp_call_result_n286 = hxc_caxecraft_localization_FirstPlayableCatalog_text(hxc_l_locale, 8);
    int32_t hxc_l_tmp_load_result_n287 = hxc_l_centerX;
    int32_t hxc_l_tmp_load_result_n288 = hxc_l_centerY;
    DrawText(hxc_l_tmp_call_result_n286, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n287, 122), (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n288, 42), (int32_t)24, hxc_l_text);
    struct Color hxc_l_this1_h2d7aa62a7cdc = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
    const char *hxc_l_tmp_call_result_n292 = hxc_caxecraft_localization_FirstPlayableCatalog_text(hxc_l_locale, 16);
    int32_t hxc_l_tmp_load_result_n293 = hxc_l_centerX;
    int32_t hxc_l_tmp_load_result_n294 = hxc_l_centerY;
    DrawText(hxc_l_tmp_call_result_n292, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n293, 125), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n294, 10), (int32_t)18, hxc_l_this1_h2d7aa62a7cdc);
  }
  if (hxc_l_paused)
  {
    struct Color hxc_l_this1_hde477d56af19 = (struct Color){ .r = 15, .g = 27, .b = 39, .a = 220 };
    int32_t hxc_l_tmp_load_result_n298 = hxc_l_centerX;
    int32_t hxc_l_tmp_load_result_n299 = hxc_l_centerY;
    DrawRectangle((int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n298, 170), (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n299, 48), (int32_t)340, (int32_t)96, hxc_l_this1_hde477d56af19);
    struct Color hxc_l_this1_h72e3df7ec1c7 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
    int32_t hxc_l_tmp_load_result_n302 = hxc_l_centerX;
    int32_t hxc_l_tmp_load_result_n303 = hxc_l_centerY;
    DrawRectangleLines((int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n302, 170), (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n303, 48), (int32_t)340, (int32_t)96, hxc_l_this1_h72e3df7ec1c7);
    hxc_string hxc_l_tmp_instance_call_result_n306 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_uiCatalog, hxc_l_locale, 32);
    hxc_l_tmp_cstring_ref_owner_owner_n77 = hxc_l_tmp_instance_call_result_n306;
    hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n308 = HXC_BORROWED_CSTRING_INITIALIZER;
    if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n77, &hxc_l_tmp_string_borrow_cstring_result_n308) != HXC_STATUS_OK)
    {
      abort();
    }
    int32_t hxc_l_tmp_load_result_n309 = hxc_l_centerX;
    int32_t hxc_l_tmp_load_result_n310 = hxc_l_centerY;
    DrawText(hxc_l_tmp_string_borrow_cstring_result_n308.data, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n309, 48), (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n310, 30), (int32_t)24, hxc_l_text);
    hxc_string hxc_l_tmp_instance_call_result_n313 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_uiCatalog, hxc_l_locale, 31);
    hxc_l_tmp_cstring_ref_owner_owner_n78 = hxc_l_tmp_instance_call_result_n313;
    hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n315 = HXC_BORROWED_CSTRING_INITIALIZER;
    if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n78, &hxc_l_tmp_string_borrow_cstring_result_n315) != HXC_STATUS_OK)
    {
      abort();
    }
    int32_t hxc_l_tmp_load_result_n316 = hxc_l_centerX;
    int32_t hxc_l_tmp_load_result_n317 = hxc_l_centerY;
    DrawText(hxc_l_tmp_string_borrow_cstring_result_n315.data, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n316, 160), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n317, 8), (int32_t)16, hxc_l_text);
    if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n78) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n77) != HXC_STATUS_OK)
    {
      abort();
    }
  }
  else
  {
    if (hxc_l_placementBlocked)
    {
      struct Color hxc_l_this1_he07ffe176fdd = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
      hxc_string hxc_l_tmp_instance_call_result_n322 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_uiCatalog, hxc_l_locale, 33);
      hxc_l_tmp_cstring_ref_owner_owner_n80 = hxc_l_tmp_instance_call_result_n322;
      hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n324 = HXC_BORROWED_CSTRING_INITIALIZER;
      if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n80, &hxc_l_tmp_string_borrow_cstring_result_n324) != HXC_STATUS_OK)
      {
        abort();
      }
      int32_t hxc_l_tmp_load_result_n325 = hxc_l_centerX;
      int32_t hxc_l_tmp_load_result_n326 = hxc_l_centerY;
      DrawText(hxc_l_tmp_string_borrow_cstring_result_n324.data, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n325, 170), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n326, 26), (int32_t)14, hxc_l_this1_he07ffe176fdd);
      if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n80) != HXC_STATUS_OK)
      {
        abort();
      }
    }
    else
    {
      if (!hxc_l_captured)
      {
        hxc_string hxc_l_tmp_instance_call_result_n330 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_uiCatalog, hxc_l_locale, 2);
        hxc_l_tmp_cstring_ref_owner_owner_n81 = hxc_l_tmp_instance_call_result_n330;
        hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n332 = HXC_BORROWED_CSTRING_INITIALIZER;
        if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n81, &hxc_l_tmp_string_borrow_cstring_result_n332) != HXC_STATUS_OK)
        {
          abort();
        }
        int32_t hxc_l_tmp_load_result_n333 = hxc_l_centerX;
        int32_t hxc_l_tmp_load_result_n334 = hxc_l_centerY;
        DrawText(hxc_l_tmp_string_borrow_cstring_result_n332.data, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n333, 90), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n334, 26), (int32_t)14, hxc_l_text);
        if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n81) != HXC_STATUS_OK)
        {
          abort();
        }
      }
      else
      {
        if (!hxc_l_hit.hxc_hit)
        {
          hxc_string hxc_l_tmp_instance_call_result_n338 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_uiCatalog, hxc_l_locale, 30);
          hxc_l_tmp_cstring_ref_owner_owner_n82 = hxc_l_tmp_instance_call_result_n338;
          hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n340 = HXC_BORROWED_CSTRING_INITIALIZER;
          if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n82, &hxc_l_tmp_string_borrow_cstring_result_n340) != HXC_STATUS_OK)
          {
            abort();
          }
          int32_t hxc_l_tmp_load_result_n341 = hxc_l_centerX;
          int32_t hxc_l_tmp_load_result_n342 = hxc_l_centerY;
          DrawText(hxc_l_tmp_string_borrow_cstring_result_n340.data, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n341, 105), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n342, 26), (int32_t)14, hxc_l_text);
          if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n82) != HXC_STATUS_OK)
          {
            abort();
          }
        }
      }
    }
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
  if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n48) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n47) != HXC_STATUS_OK)
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

struct hxc_caxecraft_app_CaxecraftApp_PlayableActorSelection hxc_caxecraft_app_CaxecraftApp_selectPlayableActors(struct hxc_array_ref *hxc_l_bindings)
{
  struct hxc_caxecraft_app_CaxecraftApp_PlayableActorBinding hxc_l_binding = { 0 };
  struct hxc_caxecraft_app_CaxecraftApp_PlayableActorBinding hxc_l_tmp_array_element_owner_n5 = { 0 };
  int32_t hxc_l_dialogueActorId = 0;
  int32_t hxc_l_enemyActorId = 0;
  int32_t hxc_l_g = 0;
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n0 = hxc_l_g;
    int32_t hxc_l_tmp_array_length_result_n1;
    if (hxc_array_ref_length(hxc_l_bindings, &hxc_l_tmp_array_length_result_n1) != HXC_STATUS_OK)
    {
      abort();
    }
    if (!(hxc_l_tmp_load_result_n0 < hxc_l_tmp_array_length_result_n1))
    {
      break;
    }
    struct hxc_caxecraft_app_CaxecraftApp_PlayableActorBinding hxc_l_tmp_array_get_result_n3;
    if (hxc_array_ref_get_copy(hxc_l_bindings, (size_t)hxc_l_g, &hxc_l_tmp_array_get_result_n3) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_array_element_owner_n5 = hxc_l_tmp_array_get_result_n3;
    hxc_l_binding = hxc_l_tmp_array_element_owner_n5;
    if (hxc_record_10bdee1f_retain(&hxc_l_binding) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_g = hxc_i32_add_wrapping(hxc_l_g, 1);
    switch (hxc_l_binding.hxc_role.hxc_tag) {
      case hxc_caxecraft_content_CharacterSpawnRole_DialogueNpc:
        {
          if (hxc_l_dialogueActorId > 0)
          {
            hxc_record_10bdee1f_destroy(&hxc_l_binding);
            hxc_array_346eae85_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n5);
            return (struct hxc_caxecraft_app_CaxecraftApp_PlayableActorSelection){ .hxc_dialogueActorId = 0, .hxc_enemyActorId = 0, .hxc_valid = false };
          }
          else
          {
            hxc_l_dialogueActorId = hxc_l_binding.hxc_entityId;
          }
          break;
        }
      case hxc_caxecraft_content_CharacterSpawnRole_EnemyActor:
        {
          if (hxc_l_enemyActorId > 0)
          {
            hxc_record_10bdee1f_destroy(&hxc_l_binding);
            hxc_array_346eae85_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n5);
            return (struct hxc_caxecraft_app_CaxecraftApp_PlayableActorSelection){ .hxc_dialogueActorId = 0, .hxc_enemyActorId = 0, .hxc_valid = false };
          }
          else
          {
            hxc_l_enemyActorId = hxc_l_binding.hxc_entityId;
          }
          break;
        }
      default:
        {
          abort();
        }
    }
    hxc_record_10bdee1f_destroy(&hxc_l_binding);
    hxc_array_346eae85_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n5);
  }
  int32_t hxc_l_tmp_load_result_n13 = hxc_l_dialogueActorId;
  bool hxc_l_tmp_short_circuit_result_n6 = hxc_l_tmp_load_result_n13 > 0;
  if (hxc_l_tmp_load_result_n13 > 0)
  {
    hxc_l_tmp_short_circuit_result_n6 = hxc_l_enemyActorId > 0;
  }
  if (!hxc_l_tmp_short_circuit_result_n6)
  {
    return (struct hxc_caxecraft_app_CaxecraftApp_PlayableActorSelection){ .hxc_dialogueActorId = 0, .hxc_enemyActorId = 0, .hxc_valid = false };
  }
  int32_t hxc_l_tmp_load_result_n16 = hxc_l_dialogueActorId;
  return (struct hxc_caxecraft_app_CaxecraftApp_PlayableActorSelection){ .hxc_dialogueActorId = hxc_l_tmp_load_result_n16, .hxc_enemyActorId = hxc_l_enemyActorId, .hxc_valid = true };
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
  const void *hxc_l_gc_roots[91] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 91, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_actorBindings = { 0 };
  struct hxc_array_ref *hxc_l_g1_hf09b4b04b8c7 = { 0 };
  struct hxc_caxecraft_content_LoadedActorBinding hxc_l_binding_h2f68f60cabdb = { 0 };
  struct hxc_caxecraft_content_LoadedActorBinding hxc_l_tmp_array_element_owner_n19 = { 0 };
  struct hxc_caxecraft_app_CaxecraftApp_PlayableActorBinding hxc_l_tmp_array_push_element_owner_n21 = { 0 };
  struct hxc_array_ref *hxc_l_g1_h8a942d6677c9 = { 0 };
  struct hxc_caxecraft_domain_ActorControllerEvent hxc_l_event = { 0 };
  struct hxc_caxecraft_domain_ActorControllerEvent hxc_l_tmp_array_element_owner_n212 = { 0 };
  hxc_string hxc_l_drop = { 0 };
  struct hxc_array_ref *hxc_l_loadedItems = { 0 };
  struct hxc_array_ref *hxc_l_actorPhases = { 0 };
  struct hxc_array_ref *hxc_l_g1_h619b6941f539 = { 0 };
  struct hxc_array_ref *hxc_l_presentationActorPhases = { 0 };
  struct hxc_caxecraft_content_ResolvedItem hxc_l_binding_hfa6e30f4b504 = { 0 };
  struct hxc_caxecraft_content_ResolvedItem hxc_l_tmp_array_element_owner_n26 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n278 = { 0 };
  struct hxc_caxecraft_content_ResolvedPlayer hxc_l_tmp_record_field_transform_receiver_owner_n28 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n286 = { 0 };
  struct hxc_array_ref *hxc_l_fluidPresentation = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n294 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n300 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n303 = { 0 };
  struct hxc_caxecraft_content_ResolvedFluidPresentation hxc_l_tmp_array_element_owner_n31 = { 0 };
  struct hxc_caxecraft_content_ResolvedFluidPresentation hxc_l_request = { 0 };
  struct hxc_caxecraft_content_ResolvedFluidPresentation hxc_l_tmp_array_element_owner_n34 = { 0 };
  struct hxc_caxecraft_content_RuntimeLevelReceipt hxc_l_receipt = { 0 };
  hxc_string hxc_l_tmp_sys_println_string_argument_owner_n36 = { 0 };
  hxc_string hxc_l_tmp_string_concat_right_owner_n37 = { 0 };
  hxc_string hxc_l_tmp_sys_println_string_argument_owner_n38 = { 0 };
  hxc_string hxc_l_tmp_string_concat_right_owner_n39 = { 0 };
  hxc_string hxc_l_tmp_sys_println_string_argument_owner_n40 = { 0 };
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
  struct hxc_caxecraft_content_RuntimeContentGeneration *hxc_l_candidate = hxc_l_tmp_enum_payload_project_n11;
  hxc_l_gc_roots[12] = (const void *)hxc_l_candidate;
  hxc_l_completeCandidate = hxc_l_candidate;
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
  struct hxc_caxecraft_content_ActiveRuntimeContent *hxc_l_activeContent = hxc_l_tmp_managed_class_result_n14;
  struct hxc_caxecraft_content_ActiveRuntimeContent *hxc_l_tmp_load_result_n15 = hxc_l_activeContent;
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
  struct hxc_caxecraft_content_RuntimeLevelCandidate *hxc_l_tmp_load_result_n26 = hxc_l_loadedCandidate;
  hxc_l_gc_roots[28] = (const void *)hxc_l_tmp_load_result_n26;
  if (hxc_l_tmp_load_result_n26 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_content_LoadedContentGeneration *hxc_l_tmp_class_field_load_result_n27 = (*hxc_l_tmp_load_result_n26).hxc_loadedGeneration;
  hxc_l_gc_roots[29] = (const void *)hxc_l_tmp_class_field_load_result_n27;
  if (hxc_l_tmp_class_field_load_result_n27 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_class_field_load_result_n28 = (*hxc_l_tmp_class_field_load_result_n27).hxc_ownedSession;
  hxc_l_gc_roots[30] = (const void *)hxc_l_tmp_class_field_load_result_n28;
  struct hxc_caxecraft_domain_GameSession *hxc_l_session = hxc_l_tmp_class_field_load_result_n28;
  struct hxc_array_ref *hxc_l_tmp_array_create_result_n29 = NULL;
  if (hxc_array_ref_create(hxc_default_allocator(), (hxc_array_element_ops){ sizeof(struct hxc_caxecraft_app_CaxecraftApp_PlayableActorBinding), _Alignof(struct hxc_caxecraft_app_CaxecraftApp_PlayableActorBinding), NULL, hxc_array_346eae85_element_copy, hxc_array_346eae85_element_assign, hxc_array_346eae85_element_destroy }, &hxc_l_tmp_array_create_result_n29) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_actorBindings = hxc_l_tmp_array_create_result_n29;
  int32_t hxc_l_g_hd3a4a0620708 = 0;
  struct hxc_caxecraft_content_RuntimeLevelCandidate *hxc_l_tmp_load_result_n30 = hxc_l_loadedCandidate;
  hxc_l_gc_roots[31] = (const void *)hxc_l_tmp_load_result_n30;
  if (hxc_l_tmp_load_result_n30 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_content_LoadedContentGeneration *hxc_l_tmp_class_field_load_result_n31 = (*hxc_l_tmp_load_result_n30).hxc_loadedGeneration;
  hxc_l_gc_roots[32] = (const void *)hxc_l_tmp_class_field_load_result_n31;
  if (hxc_l_tmp_class_field_load_result_n31 == NULL)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_tmp_instance_call_result_n32 = hxc_caxecraft_content_LoadedContentGeneration_actorBindings(hxc_l_tmp_class_field_load_result_n31);
  hxc_l_g1_hf09b4b04b8c7 = hxc_l_tmp_instance_call_result_n32;
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n33 = hxc_l_g_hd3a4a0620708;
    int32_t hxc_l_tmp_array_length_result_n35;
    if (hxc_array_ref_length(hxc_l_g1_hf09b4b04b8c7, &hxc_l_tmp_array_length_result_n35) != HXC_STATUS_OK)
    {
      abort();
    }
    if (!(hxc_l_tmp_load_result_n33 < hxc_l_tmp_array_length_result_n35))
    {
      break;
    }
    struct hxc_array_ref *hxc_l_tmp_load_result_n36 = hxc_l_g1_hf09b4b04b8c7;
    struct hxc_caxecraft_content_LoadedActorBinding hxc_l_tmp_array_get_result_n38;
    if (hxc_array_ref_get_copy(hxc_l_tmp_load_result_n36, (size_t)hxc_l_g_hd3a4a0620708, &hxc_l_tmp_array_get_result_n38) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_array_element_owner_n19 = hxc_l_tmp_array_get_result_n38;
    hxc_l_binding_h2f68f60cabdb = hxc_l_tmp_array_element_owner_n19;
    if (hxc_record_ad1068b2_retain(&hxc_l_binding_h2f68f60cabdb) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_g_hd3a4a0620708 = hxc_i32_add_wrapping(hxc_l_g_hd3a4a0620708, 1);
    struct hxc_array_ref *hxc_l_tmp_load_result_n41 = hxc_l_actorBindings;
    int32_t hxc_l_tmp_record_field_load_result_n42 = hxc_l_binding_h2f68f60cabdb.hxc_entityId;
    struct hxc_caxecraft_content_CharacterSpawnRole hxc_l_tmp_record_field_role_owner_n20 = hxc_l_binding_h2f68f60cabdb.hxc_role;
    if (hxc_enum_b133bfa7_retain(&hxc_l_tmp_record_field_role_owner_n20) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_array_push_element_owner_n21 = (struct hxc_caxecraft_app_CaxecraftApp_PlayableActorBinding){ .hxc_entityId = hxc_l_tmp_record_field_load_result_n42, .hxc_role = hxc_l_tmp_record_field_role_owner_n20 };
    int32_t hxc_l_tmp_array_push_result_n47;
    if (hxc_array_ref_push_copy(hxc_l_tmp_load_result_n41, &hxc_l_tmp_array_push_element_owner_n21, &hxc_l_tmp_array_push_result_n47) != HXC_STATUS_OK)
    {
      abort();
    }
    (void)hxc_l_tmp_array_push_result_n47;
    hxc_record_10bdee1f_destroy(&hxc_l_tmp_array_push_element_owner_n21);
    hxc_record_ad1068b2_destroy(&hxc_l_binding_h2f68f60cabdb);
    hxc_array_aa4dc4ac_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n19);
  }
  struct hxc_array_ref *hxc_l_tmp_array_create_result_n48 = NULL;
  if (hxc_array_ref_create_trivial(hxc_default_allocator(), sizeof(struct hxc_caxecraft_app_LoadedWorldItem), _Alignof(struct hxc_caxecraft_app_LoadedWorldItem), &hxc_l_tmp_array_create_result_n48) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_loadedItems = hxc_l_tmp_array_create_result_n48;
  int32_t hxc_l_g_h3dd79bee9b4a = 0;
  struct hxc_caxecraft_content_RuntimeLevelCandidate *hxc_l_tmp_load_result_n49 = hxc_l_loadedCandidate;
  hxc_l_gc_roots[33] = (const void *)hxc_l_tmp_load_result_n49;
  if (hxc_l_tmp_load_result_n49 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_content_LoadedContentGeneration *hxc_l_tmp_class_field_load_result_n50 = (*hxc_l_tmp_load_result_n49).hxc_loadedGeneration;
  hxc_l_gc_roots[34] = (const void *)hxc_l_tmp_class_field_load_result_n50;
  if (hxc_l_tmp_class_field_load_result_n50 == NULL)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_tmp_instance_call_result_n51 = hxc_caxecraft_content_LoadedContentGeneration_itemBindings(hxc_l_tmp_class_field_load_result_n50);
  hxc_l_g1_h619b6941f539 = hxc_l_tmp_instance_call_result_n51;
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n52 = hxc_l_g_h3dd79bee9b4a;
    int32_t hxc_l_tmp_array_length_result_n54;
    if (hxc_array_ref_length(hxc_l_g1_h619b6941f539, &hxc_l_tmp_array_length_result_n54) != HXC_STATUS_OK)
    {
      abort();
    }
    if (!(hxc_l_tmp_load_result_n52 < hxc_l_tmp_array_length_result_n54))
    {
      break;
    }
    struct hxc_array_ref *hxc_l_tmp_load_result_n55 = hxc_l_g1_h619b6941f539;
    struct hxc_caxecraft_content_ResolvedItem hxc_l_tmp_array_get_result_n57;
    if (hxc_array_ref_get_copy(hxc_l_tmp_load_result_n55, (size_t)hxc_l_g_h3dd79bee9b4a, &hxc_l_tmp_array_get_result_n57) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_array_element_owner_n26 = hxc_l_tmp_array_get_result_n57;
    hxc_l_binding_hfa6e30f4b504 = hxc_l_tmp_array_element_owner_n26;
    if (hxc_record_6512ae83_retain(&hxc_l_binding_hfa6e30f4b504) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_g_h3dd79bee9b4a = hxc_i32_add_wrapping(hxc_l_g_h3dd79bee9b4a, 1);
    struct hxc_array_ref *hxc_l_tmp_load_result_n60 = hxc_l_loadedItems;
    int32_t hxc_l_tmp_record_field_load_result_n61 = hxc_l_binding_hfa6e30f4b504.hxc_storage;
    int32_t hxc_l_tmp_record_field_load_result_n62 = hxc_l_binding_hfa6e30f4b504.hxc_transform.hxc_xMilli;
    int32_t hxc_l_tmp_record_field_load_result_n63 = hxc_l_binding_hfa6e30f4b504.hxc_transform.hxc_yMilli;
    int32_t hxc_l_tmp_array_push_result_n66;
    if (hxc_array_ref_push_copy(hxc_l_tmp_load_result_n60, &(struct hxc_caxecraft_app_LoadedWorldItem){ .hxc_storageCode = hxc_l_tmp_record_field_load_result_n61, .hxc_xMilli = hxc_l_tmp_record_field_load_result_n62, .hxc_yMilli = hxc_l_tmp_record_field_load_result_n63, .hxc_zMilli = hxc_l_binding_hfa6e30f4b504.hxc_transform.hxc_zMilli }, &hxc_l_tmp_array_push_result_n66) != HXC_STATUS_OK)
    {
      abort();
    }
    (void)hxc_l_tmp_array_push_result_n66;
    hxc_record_6512ae83_destroy(&hxc_l_binding_hfa6e30f4b504);
    hxc_array_d5f9659a_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n26);
  }
  struct hxc_caxecraft_content_RuntimeLevelCandidate *hxc_l_tmp_load_result_n67 = hxc_l_loadedCandidate;
  hxc_l_gc_roots[35] = (const void *)hxc_l_tmp_load_result_n67;
  if (hxc_l_tmp_load_result_n67 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_content_LoadedContentGeneration *hxc_l_tmp_class_field_load_result_n68 = (*hxc_l_tmp_load_result_n67).hxc_loadedGeneration;
  hxc_l_gc_roots[36] = (const void *)hxc_l_tmp_class_field_load_result_n68;
  if (hxc_l_tmp_class_field_load_result_n68 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_content_ResolvedLevelPlan *hxc_l_tmp_class_field_load_result_n69 = (*hxc_l_tmp_class_field_load_result_n68).hxc_resolvedPlan;
  hxc_l_gc_roots[37] = (const void *)hxc_l_tmp_class_field_load_result_n69;
  if (hxc_l_tmp_class_field_load_result_n69 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_content_ResolvedPlayer hxc_l_tmp_instance_call_result_n70 = hxc_caxecraft_content_ResolvedLevelPlan_player(hxc_l_tmp_class_field_load_result_n69);
  hxc_l_tmp_record_field_transform_receiver_owner_n28 = hxc_l_tmp_instance_call_result_n70;
  struct hxc_caxecraft_scenario_ScenarioTransform hxc_l_spawnTransform = hxc_l_tmp_record_field_transform_receiver_owner_n28.hxc_transform;
  struct hxc_caxecraft_content_RuntimeLevelCandidate *hxc_l_tmp_load_result_n73 = hxc_l_loadedCandidate;
  hxc_l_gc_roots[38] = (const void *)hxc_l_tmp_load_result_n73;
  if (hxc_l_tmp_load_result_n73 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_content_LoadedContentGeneration *hxc_l_tmp_class_field_load_result_n74 = (*hxc_l_tmp_load_result_n73).hxc_loadedGeneration;
  hxc_l_gc_roots[39] = (const void *)hxc_l_tmp_class_field_load_result_n74;
  if (hxc_l_tmp_class_field_load_result_n74 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_content_ResolvedLevelPresentationPlan *hxc_l_tmp_class_field_load_result_n75 = (*hxc_l_tmp_class_field_load_result_n74).hxc_presentationPlan;
  hxc_l_gc_roots[40] = (const void *)hxc_l_tmp_class_field_load_result_n75;
  if (hxc_l_tmp_class_field_load_result_n75 == NULL)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_tmp_instance_call_result_n76 = hxc_caxecraft_content_ResolvedLevelPresentationPlan_fluidRequests(hxc_l_tmp_class_field_load_result_n75);
  hxc_l_fluidPresentation = hxc_l_tmp_instance_call_result_n76;
  int32_t hxc_l_tmp_array_length_result_n78;
  if (hxc_array_ref_length(hxc_l_fluidPresentation, &hxc_l_tmp_array_length_result_n78) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_l_tmp_array_length_result_n78 == 0)
  {
    if (hxc_array_ref_release(hxc_l_fluidPresentation) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_record_2bd8b029_destroy(&hxc_l_tmp_record_field_transform_receiver_owner_n28);
    if (hxc_array_ref_release(hxc_l_g1_h619b6941f539) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_loadedItems) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_g1_hf09b4b04b8c7) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_actorBindings) != HXC_STATUS_OK)
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
  struct hxc_caxecraft_content_ResolvedFluidPresentation hxc_l_tmp_array_get_result_n80;
  if (hxc_array_ref_get_copy(hxc_l_fluidPresentation, (size_t)0, &hxc_l_tmp_array_get_result_n80) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_tmp_array_element_owner_n31 = hxc_l_tmp_array_get_result_n80;
  int32_t hxc_l_waterPresentationCell = hxc_l_tmp_array_element_owner_n31.hxc_cellIndex;
  int32_t hxc_l_g_hae784fdd09a4 = 0;
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n83 = hxc_l_g_hae784fdd09a4;
    int32_t hxc_l_tmp_array_length_result_n85;
    if (hxc_array_ref_length(hxc_l_fluidPresentation, &hxc_l_tmp_array_length_result_n85) != HXC_STATUS_OK)
    {
      abort();
    }
    if (!(hxc_l_tmp_load_result_n83 < hxc_l_tmp_array_length_result_n85))
    {
      break;
    }
    struct hxc_array_ref *hxc_l_tmp_load_result_n86 = hxc_l_fluidPresentation;
    struct hxc_caxecraft_content_ResolvedFluidPresentation hxc_l_tmp_array_get_result_n88;
    if (hxc_array_ref_get_copy(hxc_l_tmp_load_result_n86, (size_t)hxc_l_g_hae784fdd09a4, &hxc_l_tmp_array_get_result_n88) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_array_element_owner_n34 = hxc_l_tmp_array_get_result_n88;
    hxc_l_request = hxc_l_tmp_array_element_owner_n34;
    if (hxc_record_c8b1d97c_retain(&hxc_l_request) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_g_hae784fdd09a4 = hxc_i32_add_wrapping(hxc_l_g_hae784fdd09a4, 1);
    int32_t hxc_l_tmp_record_field_load_result_n91 = hxc_l_request.hxc_cellIndex;
    if (hxc_l_tmp_record_field_load_result_n91 != hxc_l_waterPresentationCell)
    {
      hxc_record_c8b1d97c_destroy(&hxc_l_request);
      hxc_array_1f383c02_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n34);
      hxc_array_1f383c02_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n31);
      if (hxc_array_ref_release(hxc_l_fluidPresentation) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_record_2bd8b029_destroy(&hxc_l_tmp_record_field_transform_receiver_owner_n28);
      if (hxc_array_ref_release(hxc_l_g1_h619b6941f539) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_l_loadedItems) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_l_g1_hf09b4b04b8c7) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_l_actorBindings) != HXC_STATUS_OK)
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
    hxc_record_c8b1d97c_destroy(&hxc_l_request);
    hxc_array_1f383c02_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n34);
  }
  struct hxc_caxecraft_content_RuntimeLevelCandidate *hxc_l_tmp_load_result_n93 = hxc_l_loadedCandidate;
  hxc_l_gc_roots[41] = (const void *)hxc_l_tmp_load_result_n93;
  if (hxc_l_tmp_load_result_n93 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_content_RuntimeLevelReceipt hxc_l_tmp_instance_call_result_n94 = hxc_caxecraft_content_RuntimeLevelCandidate_receipt(hxc_l_tmp_load_result_n93);
  hxc_l_receipt = hxc_l_tmp_instance_call_result_n94;
  if (hxc_io_println((hxc_string){ (const uint8_t *)"caxecraft: content-source=runtime-package", 41, true, NULL }) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_string hxc_l_tmp_string_concat_result_n96 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
  if (hxc_string_concat_ref((hxc_string){ (const uint8_t *)"caxecraft: content-path=", 24, true, NULL }, hxc_l_receipt.hxc_logicalPath, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n96) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_tmp_sys_println_string_argument_owner_n36 = hxc_l_tmp_string_concat_result_n96;
  if (hxc_io_println(hxc_l_tmp_sys_println_string_argument_owner_n36) != HXC_STATUS_OK)
  {
    if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n36) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_record_f6127605_destroy(&hxc_l_receipt);
    hxc_array_1f383c02_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n31);
    if (hxc_array_ref_release(hxc_l_fluidPresentation) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_record_2bd8b029_destroy(&hxc_l_tmp_record_field_transform_receiver_owner_n28);
    if (hxc_array_ref_release(hxc_l_g1_h619b6941f539) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_loadedItems) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_g1_hf09b4b04b8c7) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_actorBindings) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_enum_fcb4401b_destroy(&hxc_l_g_h4ef171f4fe67);
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    abort();
  }
  if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n36) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_string hxc_l_tmp_std_string_int_result_n99 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
  if (hxc_string_from_int32(hxc_l_receipt.hxc_inputHash, hxc_default_allocator(), &hxc_l_tmp_std_string_int_result_n99) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_tmp_string_concat_right_owner_n37 = hxc_l_tmp_std_string_int_result_n99;
  hxc_string hxc_l_tmp_string_concat_result_n101 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
  if (hxc_string_concat_ref((hxc_string){ (const uint8_t *)"caxecraft: content-input-hash=", 30, true, NULL }, hxc_l_tmp_string_concat_right_owner_n37, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n101) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_tmp_sys_println_string_argument_owner_n38 = hxc_l_tmp_string_concat_result_n101;
  if (hxc_io_println(hxc_l_tmp_sys_println_string_argument_owner_n38) != HXC_STATUS_OK)
  {
    if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n38) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n37) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_record_f6127605_destroy(&hxc_l_receipt);
    hxc_array_1f383c02_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n31);
    if (hxc_array_ref_release(hxc_l_fluidPresentation) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_record_2bd8b029_destroy(&hxc_l_tmp_record_field_transform_receiver_owner_n28);
    if (hxc_array_ref_release(hxc_l_g1_h619b6941f539) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_loadedItems) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_g1_hf09b4b04b8c7) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_actorBindings) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_enum_fcb4401b_destroy(&hxc_l_g_h4ef171f4fe67);
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    abort();
  }
  if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n38) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n37) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_caxecraft_content_RuntimeContentGeneration *hxc_l_tmp_load_result_n103 = hxc_l_runtimeContent;
  hxc_l_gc_roots[42] = (const void *)hxc_l_tmp_load_result_n103;
  if (hxc_l_tmp_load_result_n103 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_content_RuntimeLevelCandidate *hxc_l_tmp_class_field_load_result_n104 = (*hxc_l_tmp_load_result_n103).hxc_loadedLevel;
  hxc_l_gc_roots[43] = (const void *)hxc_l_tmp_class_field_load_result_n104;
  if (hxc_l_tmp_class_field_load_result_n104 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_content_LoadedContentGeneration *hxc_l_tmp_class_field_load_result_n105 = (*hxc_l_tmp_class_field_load_result_n104).hxc_loadedGeneration;
  hxc_l_gc_roots[44] = (const void *)hxc_l_tmp_class_field_load_result_n105;
  if (hxc_l_tmp_class_field_load_result_n105 == NULL)
  {
    abort();
  }
  int32_t hxc_l_tmp_class_field_load_result_n106 = (*hxc_l_tmp_class_field_load_result_n105).hxc_generationIdValue;
  hxc_string hxc_l_tmp_std_string_int_result_n107 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
  if (hxc_string_from_int32(hxc_l_tmp_class_field_load_result_n106, hxc_default_allocator(), &hxc_l_tmp_std_string_int_result_n107) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_tmp_string_concat_right_owner_n39 = hxc_l_tmp_std_string_int_result_n107;
  hxc_string hxc_l_tmp_string_concat_result_n109 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
  if (hxc_string_concat_ref((hxc_string){ (const uint8_t *)"caxecraft: content-generation=", 30, true, NULL }, hxc_l_tmp_string_concat_right_owner_n39, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n109) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_tmp_sys_println_string_argument_owner_n40 = hxc_l_tmp_string_concat_result_n109;
  if (hxc_io_println(hxc_l_tmp_sys_println_string_argument_owner_n40) != HXC_STATUS_OK)
  {
    if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n40) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n39) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_record_f6127605_destroy(&hxc_l_receipt);
    hxc_array_1f383c02_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n31);
    if (hxc_array_ref_release(hxc_l_fluidPresentation) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_record_2bd8b029_destroy(&hxc_l_tmp_record_field_transform_receiver_owner_n28);
    if (hxc_array_ref_release(hxc_l_g1_h619b6941f539) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_loadedItems) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_g1_hf09b4b04b8c7) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_actorBindings) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_enum_fcb4401b_destroy(&hxc_l_g_h4ef171f4fe67);
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    abort();
  }
  if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n40) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n39) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_caxecraft_app_CaxecraftApp_PlayableActorSelection hxc_l_tmp_call_result_n112 = hxc_caxecraft_app_CaxecraftApp_selectPlayableActors(hxc_l_actorBindings);
  struct hxc_caxecraft_app_CaxecraftApp_PlayableActorSelection hxc_l_actors = hxc_l_tmp_call_result_n112;
  if (!hxc_l_actors.hxc_valid)
  {
    hxc_record_f6127605_destroy(&hxc_l_receipt);
    hxc_array_1f383c02_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n31);
    if (hxc_array_ref_release(hxc_l_fluidPresentation) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_record_2bd8b029_destroy(&hxc_l_tmp_record_field_transform_receiver_owner_n28);
    if (hxc_array_ref_release(hxc_l_g1_h619b6941f539) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_loadedItems) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_g1_hf09b4b04b8c7) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_actorBindings) != HXC_STATUS_OK)
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
  int32_t hxc_l_dialogueActorId = hxc_l_actors.hxc_dialogueActorId;
  int32_t hxc_l_enemyActorId = hxc_l_actors.hxc_enemyActorId;
  int32_t hxc_l_windowFlags = hxc_i32_bit_or(64, 4);
  hxc_l_windowFlags = hxc_i32_bit_or(hxc_l_windowFlags, 8192);
  SetConfigFlags((uint32_t)hxc_l_windowFlags);
  InitWindow((int32_t)1280, (int32_t)720, "Caxecraft \342\200\224 Haxe shaped into careful C");
  bool hxc_l_tmp_native_call_result_n118 = IsWindowReady();
  if (!hxc_l_tmp_native_call_result_n118)
  {
    hxc_record_f6127605_destroy(&hxc_l_receipt);
    hxc_array_1f383c02_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n31);
    if (hxc_array_ref_release(hxc_l_fluidPresentation) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_record_2bd8b029_destroy(&hxc_l_tmp_record_field_transform_receiver_owner_n28);
    if (hxc_array_ref_release(hxc_l_g1_h619b6941f539) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_loadedItems) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_g1_hf09b4b04b8c7) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_actorBindings) != HXC_STATUS_OK)
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
  SetWindowMinSize((int32_t)800, (int32_t)450);
  SetExitKey((int32_t)0);
  SetTargetFPS((int32_t)120);
  EnableCursor();
  struct Texture hxc_l_tmp_native_call_result_n119 = LoadTexture("assets/showcase/title-panorama.png");
  struct Texture hxc_l_titleTexture = hxc_l_tmp_native_call_result_n119;
  bool hxc_l_tmp_native_call_result_n121 = IsTextureValid(hxc_l_titleTexture);
  bool hxc_l_titleTextureReady = hxc_l_tmp_native_call_result_n121;
  struct Texture hxc_l_tmp_native_call_result_n122 = LoadTexture("assets/branding/caxecraft-wordmark.png");
  struct Texture hxc_l_wordmarkTexture = hxc_l_tmp_native_call_result_n122;
  bool hxc_l_tmp_native_call_result_n124 = IsTextureValid(hxc_l_wordmarkTexture);
  bool hxc_l_wordmarkTextureReady = hxc_l_tmp_native_call_result_n124;
  struct Texture hxc_l_tmp_native_call_result_n125 = LoadTexture("assets/atlases/hud.png");
  struct Texture hxc_l_hudTexture = hxc_l_tmp_native_call_result_n125;
  bool hxc_l_tmp_native_call_result_n127 = IsTextureValid(hxc_l_hudTexture);
  bool hxc_l_hudTextureReady = hxc_l_tmp_native_call_result_n127;
  struct Texture hxc_l_tmp_native_call_result_n128 = LoadTexture("assets/atlases/items.png");
  struct Texture hxc_l_itemTexture = hxc_l_tmp_native_call_result_n128;
  bool hxc_l_tmp_native_call_result_n130 = IsTextureValid(hxc_l_itemTexture);
  bool hxc_l_itemTextureReady = hxc_l_tmp_native_call_result_n130;
  struct Texture hxc_l_tmp_native_call_result_n131 = LoadTexture("assets/atlases/adventure-items.png");
  struct Texture hxc_l_adventureItemTexture = hxc_l_tmp_native_call_result_n131;
  bool hxc_l_tmp_native_call_result_n133 = IsTextureValid(hxc_l_adventureItemTexture);
  bool hxc_l_adventureItemTextureReady = hxc_l_tmp_native_call_result_n133;
  struct Texture hxc_l_tmp_load_result_n134 = hxc_l_hudTexture;
  bool hxc_l_tmp_load_result_n135 = hxc_l_hudTextureReady;
  struct Texture hxc_l_tmp_load_result_n136 = hxc_l_itemTexture;
  bool hxc_l_tmp_load_result_n137 = hxc_l_itemTextureReady;
  struct Texture hxc_l_tmp_load_result_n138 = hxc_l_adventureItemTexture;
  struct hxc_caxecraft_app_HudResources hxc_l_hudResources = (struct hxc_caxecraft_app_HudResources){ .hxc_adventureItemTexture = hxc_l_tmp_load_result_n138, .hxc_adventureItemTextureReady = hxc_l_adventureItemTextureReady, .hxc_hudTexture = hxc_l_tmp_load_result_n134, .hxc_hudTextureReady = hxc_l_tmp_load_result_n135, .hxc_itemTexture = hxc_l_tmp_load_result_n136, .hxc_itemTextureReady = hxc_l_tmp_load_result_n137 };
  struct Texture hxc_l_tmp_native_call_result_n141 = LoadTexture("assets/atlases/entities.png");
  struct Texture hxc_l_entityTexture = hxc_l_tmp_native_call_result_n141;
  bool hxc_l_tmp_native_call_result_n143 = IsTextureValid(hxc_l_entityTexture);
  bool hxc_l_entityTextureReady = hxc_l_tmp_native_call_result_n143;
  struct Texture hxc_l_tmp_native_call_result_n144 = LoadTexture("assets/atlases/terrain.png");
  struct Texture hxc_l_terrainTexture = hxc_l_tmp_native_call_result_n144;
  bool hxc_l_tmp_native_call_result_n146 = IsTextureValid(hxc_l_terrainTexture);
  bool hxc_l_terrainTextureReady = hxc_l_tmp_native_call_result_n146;
  struct Texture hxc_l_tmp_native_call_result_n147 = LoadTexture("assets/atlases/adventure-terrain.png");
  struct Texture hxc_l_adventureTerrainTexture = hxc_l_tmp_native_call_result_n147;
  bool hxc_l_tmp_native_call_result_n149 = IsTextureValid(hxc_l_adventureTerrainTexture);
  bool hxc_l_adventureTerrainTextureReady = hxc_l_tmp_native_call_result_n149;
  double hxc_l_cameraWaterBlend = 0.0;
  struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n150 = hxc_caxecraft_gameplay_Inventory_starter();
  struct hxc_caxecraft_gameplay_InventoryState hxc_l_inventory = hxc_l_tmp_call_result_n150;
  enum hxc_caxecraft_gameplay_GuidePhase hxc_l_guidePhase = hxc_caxecraft_gameplay_GuidePhase_Waiting;
  struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n152 = hxc_l_session;
  hxc_l_gc_roots[45] = (const void *)hxc_l_tmp_load_result_n152;
  if (hxc_l_tmp_load_result_n152 == NULL)
  {
    abort();
  }
  bool hxc_l_tmp_instance_call_result_n154 = hxc_caxecraft_domain_GameSession_actorInteractionAvailable(hxc_l_tmp_load_result_n152, hxc_l_dialogueActorId);
  bool hxc_l_guideInteractionAvailable = hxc_l_tmp_instance_call_result_n154;
  struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n155 = hxc_l_session;
  hxc_l_gc_roots[46] = (const void *)hxc_l_tmp_load_result_n155;
  if (hxc_l_tmp_load_result_n155 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n157 = hxc_caxecraft_domain_GameSession_readCharacter(hxc_l_tmp_load_result_n155, hxc_l_dialogueActorId);
  struct hxc_caxecraft_domain_Character hxc_l_dialogueActor = hxc_l_tmp_instance_call_result_n157;
  struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n158 = hxc_l_session;
  hxc_l_gc_roots[47] = (const void *)hxc_l_tmp_load_result_n158;
  if (hxc_l_tmp_load_result_n158 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n160 = hxc_caxecraft_domain_GameSession_readCharacter(hxc_l_tmp_load_result_n158, hxc_l_enemyActorId);
  struct hxc_caxecraft_domain_Character hxc_l_enemyActor = hxc_l_tmp_instance_call_result_n160;
  struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n161 = hxc_l_session;
  hxc_l_gc_roots[48] = (const void *)hxc_l_tmp_load_result_n161;
  if (hxc_l_tmp_load_result_n161 == NULL)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_tmp_instance_call_result_n162 = hxc_caxecraft_domain_GameSession_actorControllerStateSnapshots(hxc_l_tmp_load_result_n161);
  hxc_l_initialActorPhases = hxc_l_tmp_instance_call_result_n162;
  struct hxc_array_ref *hxc_l_tmp_load_result_n163 = hxc_l_initialActorPhases;
  struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation hxc_l_tmp_call_result_n166 = hxc_caxecraft_app_CaxecraftApp_observeActorPhase(hxc_l_tmp_load_result_n163, hxc_l_dialogueActorId, hxc_caxecraft_domain_ActorControllerPhase_Stationary);
  struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation hxc_l_dialoguePhase = hxc_l_tmp_call_result_n166;
  struct hxc_array_ref *hxc_l_tmp_load_result_n167 = hxc_l_initialActorPhases;
  struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation hxc_l_tmp_call_result_n170 = hxc_caxecraft_app_CaxecraftApp_observeActorPhase(hxc_l_tmp_load_result_n167, hxc_l_enemyActorId, hxc_caxecraft_domain_ActorControllerPhase_Resting);
  struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation hxc_l_enemyPhase = hxc_l_tmp_call_result_n170;
  int32_t hxc_l_tmp_record_field_load_result_n171 = hxc_l_dialogueActor.hxc_id;
  bool hxc_l_tmp_short_circuit_result_n71 = hxc_l_tmp_record_field_load_result_n171 <= 0;
  if (!(hxc_l_tmp_record_field_load_result_n171 <= 0))
  {
    hxc_l_tmp_short_circuit_result_n71 = hxc_l_enemyActor.hxc_id <= 0;
  }
  bool hxc_l_tmp_short_circuit_load_result_n173 = hxc_l_tmp_short_circuit_result_n71;
  bool hxc_l_tmp_short_circuit_result_n72 = hxc_l_tmp_short_circuit_load_result_n173;
  if (!hxc_l_tmp_short_circuit_load_result_n173)
  {
    hxc_l_tmp_short_circuit_result_n72 = !hxc_l_dialoguePhase.hxc_valid;
  }
  bool hxc_l_tmp_short_circuit_load_result_n175 = hxc_l_tmp_short_circuit_result_n72;
  bool hxc_l_tmp_short_circuit_result_n73 = hxc_l_tmp_short_circuit_load_result_n175;
  if (!hxc_l_tmp_short_circuit_load_result_n175)
  {
    hxc_l_tmp_short_circuit_result_n73 = !hxc_l_enemyPhase.hxc_valid;
  }
  if (hxc_l_tmp_short_circuit_result_n73)
  {
    if (hxc_array_ref_release(hxc_l_initialActorPhases) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_record_f6127605_destroy(&hxc_l_receipt);
    hxc_array_1f383c02_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n31);
    if (hxc_array_ref_release(hxc_l_fluidPresentation) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_record_2bd8b029_destroy(&hxc_l_tmp_record_field_transform_receiver_owner_n28);
    if (hxc_array_ref_release(hxc_l_g1_h619b6941f539) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_loadedItems) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_g1_hf09b4b04b8c7) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_actorBindings) != HXC_STATUS_OK)
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
  struct hxc_caxecraft_gameplay_SwordCombatState hxc_l_swordCombat = (struct hxc_caxecraft_gameplay_SwordCombatState){ .hxc_cooldownTicks = 0 };
  struct hxc_caxecraft_gameplay_BerryDropState hxc_l_berryDrop = (struct hxc_caxecraft_gameplay_BerryDropState){ .hxc_active = false, .hxc_amount = 0, .hxc_x = 0.0, .hxc_y = 0.0, .hxc_z = 0.0 };
  double hxc_l_lookX = 0.0;
  double hxc_l_lookY = -0.18;
  double hxc_l_lookZ = -1.0;
  double hxc_l_accumulator = 0.0;
  struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n180 = hxc_l_session;
  hxc_l_gc_roots[49] = (const void *)hxc_l_tmp_load_result_n180;
  if (hxc_l_tmp_load_result_n180 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_domain_GameView hxc_l_tmp_instance_call_result_n181 = hxc_caxecraft_domain_GameSession_view(hxc_l_tmp_load_result_n180);
  struct hxc_caxecraft_domain_GameView hxc_l_initialPresentation = hxc_l_tmp_instance_call_result_n181;
  if (!hxc_l_initialPresentation.hxc_valid)
  {
    if (hxc_array_ref_release(hxc_l_initialActorPhases) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_record_f6127605_destroy(&hxc_l_receipt);
    hxc_array_1f383c02_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n31);
    if (hxc_array_ref_release(hxc_l_fluidPresentation) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_record_2bd8b029_destroy(&hxc_l_tmp_record_field_transform_receiver_owner_n28);
    if (hxc_array_ref_release(hxc_l_g1_h619b6941f539) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_loadedItems) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_g1_hf09b4b04b8c7) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_actorBindings) != HXC_STATUS_OK)
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
  struct hxc_caxecraft_app_MotionHistory hxc_l_tmp_call_result_n184 = hxc_caxecraft_app_MotionInterpolation_start(hxc_l_initialPresentation.hxc_localPlayer.hxc_body);
  struct hxc_caxecraft_app_MotionHistory hxc_l_motionHistory = hxc_l_tmp_call_result_n184;
  bool hxc_l_jumpQueued = false;
  bool hxc_l_swordQueued = false;
  int32_t hxc_l_selectedMode = 0;
  int32_t hxc_l_locale = 0;
  bool hxc_l_showInitialTitle = true;
  enum hxc_caxecraft_app_AppScreen hxc_l_tmp_call_result_n186 = hxc_caxecraft_app_AppScreen_initialScreen(hxc_l_showInitialTitle);
  enum hxc_caxecraft_app_AppScreen hxc_l_screen = hxc_l_tmp_call_result_n186;
  bool hxc_l_tmp_call_result_n188 = hxc_caxecraft_app_AppScreen_capturesPointer(hxc_l_screen);
  if (hxc_l_tmp_call_result_n188)
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
    bool hxc_l_tmp_load_result_n189 = hxc_l_quit;
    bool hxc_l_tmp_short_circuit_result_n102 = !hxc_l_tmp_load_result_n189;
    if (!hxc_l_tmp_load_result_n189)
    {
      bool hxc_l_tmp_native_call_result_n190 = WindowShouldClose();
      hxc_l_tmp_short_circuit_result_n102 = !hxc_l_tmp_native_call_result_n190;
    }
    if (!hxc_l_tmp_short_circuit_result_n102)
    {
      break;
    }
    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n192 = hxc_l_session;
    hxc_l_gc_roots[50] = (const void *)hxc_l_tmp_load_result_n192;
    if (hxc_l_tmp_load_result_n192 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_domain_GameView hxc_l_tmp_instance_call_result_n193 = hxc_caxecraft_domain_GameSession_view(hxc_l_tmp_load_result_n192);
    struct hxc_caxecraft_domain_GameView hxc_l_initialView = hxc_l_tmp_instance_call_result_n193;
    if (!hxc_l_initialView.hxc_valid)
    {
      hxc_l_quit = true;
    }
    struct hxc_caxecraft_domain_Character hxc_l_character = hxc_l_initialView.hxc_localPlayer;
    bool hxc_l_recapturedThisFrame = false;
    bool hxc_l_resetMotionThisFrame = false;
    float hxc_l_tmp_native_call_result_n196 = GetFrameTime();
    double hxc_l_frameSeconds = (double)hxc_l_tmp_native_call_result_n196;
    if (hxc_l_frameSeconds > 0.25)
    {
      hxc_l_frameSeconds = 0.25;
    }
    bool hxc_l_tmp_native_call_result_n198 = IsWindowFocused();
    bool hxc_l_focused = hxc_l_tmp_native_call_result_n198;
    bool hxc_l_tmp_call_result_n200 = hxc_caxecraft_app_AppScreen_capturesPointer(hxc_l_screen);
    bool hxc_l_captured_h0f884f9ad79e = hxc_l_tmp_call_result_n200;
    bool hxc_l_tmp_call_result_n202 = hxc_caxecraft_app_AppScreen_pausesSimulation(hxc_l_screen);
    bool hxc_l_paused_h061a51c3071e = hxc_l_tmp_call_result_n202;
    double hxc_l_forward = 0.0;
    double hxc_l_right = 0.0;
    bool hxc_l_tmp_native_call_result_n203 = IsKeyDown((int32_t)87);
    if (hxc_l_tmp_native_call_result_n203)
    {
      hxc_l_forward = hxc_l_forward + 1.0;
    }
    bool hxc_l_tmp_native_call_result_n205 = IsKeyDown((int32_t)83);
    if (hxc_l_tmp_native_call_result_n205)
    {
      hxc_l_forward = hxc_l_forward - 1.0;
    }
    bool hxc_l_tmp_native_call_result_n207 = IsKeyDown((int32_t)68);
    if (hxc_l_tmp_native_call_result_n207)
    {
      hxc_l_right = hxc_l_right + 1.0;
    }
    bool hxc_l_tmp_native_call_result_n209 = IsKeyDown((int32_t)65);
    if (hxc_l_tmp_native_call_result_n209)
    {
      hxc_l_right = hxc_l_right - 1.0;
    }
    double hxc_l_lookYaw_hb4907f53b504 = 0.0;
    double hxc_l_lookPitch_h4289a098969e = 0.0;
    if (hxc_l_captured_h0f884f9ad79e)
    {
      struct Vector2 hxc_l_tmp_native_call_result_n212 = GetMouseDelta();
      struct Vector2 hxc_l_mouse = hxc_l_tmp_native_call_result_n212;
      hxc_l_lookYaw_hb4907f53b504 = -(double)hxc_l_mouse.x * 0.0025;
      hxc_l_lookPitch_h4289a098969e = -(double)hxc_l_mouse.y * 0.0025;
    }
    bool hxc_l_tmp_native_call_result_n217 = IsMouseButtonPressed((int32_t)0);
    bool hxc_l_leftPressed = hxc_l_tmp_native_call_result_n217;
    bool hxc_l_tmp_load_result_n218 = hxc_l_captured_h0f884f9ad79e;
    bool hxc_l_tmp_short_circuit_result_n118 = hxc_l_tmp_load_result_n218;
    if (hxc_l_tmp_load_result_n218)
    {
      hxc_l_tmp_short_circuit_result_n118 = hxc_l_leftPressed;
    }
    bool hxc_l_primaryPressed_h4ce0c062fe50 = hxc_l_tmp_short_circuit_result_n118;
    bool hxc_l_tmp_load_result_n221 = hxc_l_captured_h0f884f9ad79e;
    bool hxc_l_tmp_short_circuit_result_n120 = hxc_l_tmp_load_result_n221;
    if (hxc_l_tmp_load_result_n221)
    {
      bool hxc_l_tmp_native_call_result_n222 = IsMouseButtonPressed((int32_t)1);
      hxc_l_tmp_short_circuit_result_n120 = hxc_l_tmp_native_call_result_n222;
    }
    bool hxc_l_secondaryPressed_h1815b163360b = hxc_l_tmp_short_circuit_result_n120;
    bool hxc_l_tmp_load_result_n224 = hxc_l_captured_h0f884f9ad79e;
    bool hxc_l_tmp_short_circuit_result_n122 = hxc_l_tmp_load_result_n224;
    if (hxc_l_tmp_load_result_n224)
    {
      bool hxc_l_tmp_native_call_result_n225 = IsKeyPressed((int32_t)69);
      hxc_l_tmp_short_circuit_result_n122 = hxc_l_tmp_native_call_result_n225;
    }
    bool hxc_l_interactPressed_h9a7222b43d63 = hxc_l_tmp_short_circuit_result_n122;
    int32_t hxc_l_hotbarSelection_hb4d44b3bf26a = -1;
    bool hxc_l_tmp_native_call_result_n227 = IsKeyPressed((int32_t)49);
    if (hxc_l_tmp_native_call_result_n227)
    {
      hxc_l_hotbarSelection_hb4d44b3bf26a = 0;
    }
    bool hxc_l_tmp_native_call_result_n228 = IsKeyPressed((int32_t)50);
    if (hxc_l_tmp_native_call_result_n228)
    {
      hxc_l_hotbarSelection_hb4d44b3bf26a = 1;
    }
    bool hxc_l_tmp_native_call_result_n229 = IsKeyPressed((int32_t)51);
    if (hxc_l_tmp_native_call_result_n229)
    {
      hxc_l_hotbarSelection_hb4d44b3bf26a = 2;
    }
    bool hxc_l_tmp_native_call_result_n230 = IsKeyPressed((int32_t)52);
    if (hxc_l_tmp_native_call_result_n230)
    {
      hxc_l_hotbarSelection_hb4d44b3bf26a = 3;
    }
    bool hxc_l_tmp_native_call_result_n231 = IsKeyPressed((int32_t)53);
    if (hxc_l_tmp_native_call_result_n231)
    {
      hxc_l_hotbarSelection_hb4d44b3bf26a = 4;
    }
    bool hxc_l_tmp_native_call_result_n232 = IsKeyPressed((int32_t)54);
    if (hxc_l_tmp_native_call_result_n232)
    {
      hxc_l_hotbarSelection_hb4d44b3bf26a = 5;
    }
    bool hxc_l_tmp_native_call_result_n233 = IsKeyPressed((int32_t)55);
    if (hxc_l_tmp_native_call_result_n233)
    {
      hxc_l_hotbarSelection_hb4d44b3bf26a = 6;
    }
    bool hxc_l_tmp_native_call_result_n234 = IsKeyPressed((int32_t)56);
    if (hxc_l_tmp_native_call_result_n234)
    {
      hxc_l_hotbarSelection_hb4d44b3bf26a = 7;
    }
    float hxc_l_tmp_native_call_result_n235 = GetMouseWheelMove();
    double hxc_l_wheel = (double)hxc_l_tmp_native_call_result_n235;
    int32_t hxc_l_hotbarCycle_ha5fc89c5537e = 0;
    if (hxc_l_wheel > 0.0)
    {
      hxc_l_hotbarCycle_ha5fc89c5537e = -1;
    }
    if (hxc_l_wheel < 0.0)
    {
      hxc_l_hotbarCycle_ha5fc89c5537e = 1;
    }
    bool hxc_l_tmp_native_call_result_n238 = IsKeyPressed((int32_t)32);
    bool hxc_l_jumpPressed_he77b75182b6a = hxc_l_tmp_native_call_result_n238;
    bool hxc_l_tmp_native_call_result_n239 = IsKeyPressed((int32_t)256);
    bool hxc_l_pausePressed_hc02c74ed5d48 = hxc_l_tmp_native_call_result_n239;
    bool hxc_l_tmp_native_call_result_n240 = IsKeyPressed((int32_t)81);
    bool hxc_l_quitPressed_h4a48ed0f0cc2 = hxc_l_tmp_native_call_result_n240;
    double hxc_l_frameInput_moveForward = hxc_l_forward;
    double hxc_l_frameInput_moveRight = hxc_l_right;
    double hxc_l_frameInput_lookYaw = hxc_l_lookYaw_hb4907f53b504;
    double hxc_l_frameInput_lookPitch = hxc_l_lookPitch_h4289a098969e;
    bool hxc_l_frameInput_jumpPressed = hxc_l_jumpPressed_he77b75182b6a;
    bool hxc_l_tmp_native_call_result_n246 = IsKeyDown((int32_t)340);
    bool hxc_l_frameInput_descendHeld = hxc_l_tmp_native_call_result_n246;
    bool hxc_l_frameInput_primaryPressed = hxc_l_primaryPressed_h4ce0c062fe50;
    bool hxc_l_frameInput_secondaryPressed = hxc_l_secondaryPressed_h1815b163360b;
    bool hxc_l_frameInput_interactPressed = hxc_l_interactPressed_h9a7222b43d63;
    bool hxc_l_frameInput_pausePressed = hxc_l_pausePressed_hc02c74ed5d48;
    bool hxc_l_tmp_load_result_n251 = hxc_l_paused_h061a51c3071e;
    bool hxc_l_tmp_short_circuit_result_n140 = hxc_l_tmp_load_result_n251;
    if (hxc_l_tmp_load_result_n251)
    {
      hxc_l_tmp_short_circuit_result_n140 = hxc_l_leftPressed;
    }
    bool hxc_l_frameInput_capturePressed = hxc_l_tmp_short_circuit_result_n140;
    bool hxc_l_frameInput_quitPressed = hxc_l_quitPressed_h4a48ed0f0cc2;
    int32_t hxc_l_frameInput_hotbarSelection = hxc_l_hotbarSelection_hb4d44b3bf26a;
    int32_t hxc_l_frameInput_hotbarCycle = hxc_l_hotbarCycle_ha5fc89c5537e;
    double hxc_l_moveForward = hxc_l_frameInput_moveForward;
    double hxc_l_moveRight = hxc_l_frameInput_moveRight;
    double hxc_l_lookYaw_h4ec79428f0ea = hxc_l_frameInput_lookYaw;
    double hxc_l_lookPitch_h1da1297fd85c = hxc_l_frameInput_lookPitch;
    bool hxc_l_jumpPressed_h1cc68e15db02 = hxc_l_frameInput_jumpPressed;
    bool hxc_l_primaryPressed_hff4c9f46b797 = hxc_l_frameInput_primaryPressed;
    bool hxc_l_secondaryPressed_hb34785c06c08 = hxc_l_frameInput_secondaryPressed;
    bool hxc_l_interactPressed_h4b463c00065c = hxc_l_frameInput_interactPressed;
    bool hxc_l_pausePressed_h04e9dd4c01d6 = hxc_l_frameInput_pausePressed;
    bool hxc_l_capturePressed = hxc_l_frameInput_capturePressed;
    bool hxc_l_quitPressed_h01dd2d52fe35 = hxc_l_frameInput_quitPressed;
    int32_t hxc_l_hotbarSelection_h57341ffd20ff = hxc_l_frameInput_hotbarSelection;
    int32_t hxc_l_hotbarCycle_h6f9f0af0c27c = hxc_l_frameInput_hotbarCycle;
    bool hxc_l_descendHeld = hxc_l_frameInput_descendHeld;
    if (hxc_l_quitPressed_h01dd2d52fe35)
    {
      hxc_l_quit = true;
    }
    if (hxc_l_hotbarSelection_h57341ffd20ff >= 0)
    {
      struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_load_result_n273 = hxc_l_inventory;
      struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n275 = hxc_caxecraft_gameplay_Inventory_select(hxc_l_tmp_load_result_n273, hxc_l_hotbarSelection_h57341ffd20ff);
      hxc_l_inventory = hxc_l_tmp_call_result_n275;
    }
    if (hxc_l_hotbarCycle_h6f9f0af0c27c != 0)
    {
      struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_load_result_n277 = hxc_l_inventory;
      struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n279 = hxc_caxecraft_gameplay_Inventory_cycle(hxc_l_tmp_load_result_n277, hxc_l_hotbarCycle_h6f9f0af0c27c);
      hxc_l_inventory = hxc_l_tmp_call_result_n279;
    }
    bool hxc_l_tmp_call_result_n281 = hxc_caxecraft_app_AppScreen_isPlaying(hxc_l_screen);
    bool hxc_l_tmp_short_circuit_result_n158 = hxc_l_tmp_call_result_n281;
    if (hxc_l_tmp_call_result_n281)
    {
      hxc_l_tmp_short_circuit_result_n158 = hxc_l_interactPressed_h4b463c00065c;
    }
    if (hxc_l_tmp_short_circuit_result_n158)
    {
      if (hxc_l_character.hxc_vitals.hxc_health <= 0)
      {
        struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n285 = hxc_l_session;
        hxc_l_gc_roots[51] = (const void *)hxc_l_tmp_load_result_n285;
        if (hxc_l_tmp_load_result_n285 == NULL)
        {
          abort();
        }
        struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n286 = hxc_l_session;
        hxc_l_gc_roots[52] = (const void *)hxc_l_tmp_load_result_n286;
        if (hxc_l_tmp_load_result_n286 == NULL)
        {
          abort();
        }
        size_t hxc_l_tmp_length_n288;
        const uint8_t *hxc_l_tmp_instance_call_result_n287 = hxc_caxecraft_domain_GameSession_worldView(hxc_l_tmp_load_result_n286, &hxc_l_tmp_length_n288);
        struct hxc_caxecraft_domain_CharacterBody hxc_l_tmp_call_result_n290 = hxc_caxecraft_app_CaxecraftApp_spawnPlayer(hxc_l_tmp_instance_call_result_n287, hxc_l_tmp_length_n288, hxc_l_spawnTransform);
        struct hxc_caxecraft_domain_LocalCharacterCommandResult hxc_l_tmp_instance_call_result_n291 = hxc_caxecraft_domain_GameSession_reviveLocalPlayerAt(hxc_l_tmp_load_result_n285, hxc_l_tmp_call_result_n290);
        struct hxc_caxecraft_domain_LocalCharacterCommandResult hxc_l_revival = hxc_l_tmp_instance_call_result_n291;
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
        struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n294 = hxc_l_session;
        hxc_l_gc_roots[53] = (const void *)hxc_l_tmp_load_result_n294;
        if (hxc_l_tmp_load_result_n294 == NULL)
        {
          abort();
        }
        bool hxc_l_tmp_instance_call_result_n296 = hxc_caxecraft_domain_GameSession_actorInteractionAvailable(hxc_l_tmp_load_result_n294, hxc_l_dialogueActorId);
        if (hxc_l_tmp_instance_call_result_n296)
        {
          bool hxc_l_sharesBerries = hxc_l_guidePhase == hxc_caxecraft_gameplay_GuidePhase_Welcomed;
          if (hxc_l_sharesBerries)
          {
            int32_t hxc_l_tmp_call_result_n302 = hxc_caxecraft_gameplay_Inventory_acceptedAmount(hxc_l_inventory, hxc_caxecraft_gameplay_ItemKind_Berries, 2);
            int32_t hxc_l_acceptedGift = hxc_l_tmp_call_result_n302;
            if (hxc_l_acceptedGift == 2)
            {
              struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_load_result_n304 = hxc_l_inventory;
              struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n307 = hxc_caxecraft_gameplay_Inventory_collectItem(hxc_l_tmp_load_result_n304, hxc_caxecraft_gameplay_ItemKind_Berries, hxc_l_acceptedGift);
              hxc_l_inventory = hxc_l_tmp_call_result_n307;
              enum hxc_caxecraft_gameplay_GuidePhase hxc_l_tmp_call_result_n309 = hxc_caxecraft_app_CaxecraftApp_advanceGuidePhase(hxc_l_guidePhase);
              hxc_l_guidePhase = hxc_l_tmp_call_result_n309;
            }
            else
            {
              hxc_l_inventoryFullReason = 1;
              hxc_l_inventoryFullFrames = 90;
            }
          }
          else
          {
            enum hxc_caxecraft_gameplay_GuidePhase hxc_l_tmp_call_result_n311 = hxc_caxecraft_app_CaxecraftApp_advanceGuidePhase(hxc_l_guidePhase);
            hxc_l_guidePhase = hxc_l_tmp_call_result_n311;
          }
        }
      }
    }
    bool hxc_l_tmp_call_result_n313 = hxc_caxecraft_app_AppScreen_showsTitle(hxc_l_screen);
    bool hxc_l_tmp_short_circuit_result_n162 = hxc_l_tmp_call_result_n313;
    if (hxc_l_tmp_call_result_n313)
    {
      hxc_l_tmp_short_circuit_result_n162 = hxc_l_focused;
    }
    if (hxc_l_tmp_short_circuit_result_n162)
    {
      int32_t hxc_l_modeBeforeInput = hxc_l_selectedMode;
      bool hxc_l_tmp_native_call_result_n317 = IsKeyPressed((int32_t)76);
      if (hxc_l_tmp_native_call_result_n317)
      {
        struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_tmp_load_result_n318 = hxc_l_uiCatalog;
        hxc_l_gc_roots[54] = (const void *)hxc_l_tmp_load_result_n318;
        if (hxc_l_tmp_load_result_n318 == NULL)
        {
          abort();
        }
        int32_t hxc_l_tmp_instance_call_result_n320 = hxc_caxecraft_localization_RuntimeUiCatalog_nextLocale(hxc_l_tmp_load_result_n318, hxc_l_locale);
        hxc_l_locale = hxc_l_tmp_instance_call_result_n320;
      }
      bool hxc_l_tmp_native_call_result_n321 = IsKeyPressed((int32_t)265);
      bool hxc_l_tmp_short_circuit_result_n164 = hxc_l_tmp_native_call_result_n321;
      if (!hxc_l_tmp_native_call_result_n321)
      {
        bool hxc_l_tmp_native_call_result_n322 = IsKeyPressed((int32_t)264);
        hxc_l_tmp_short_circuit_result_n164 = hxc_l_tmp_native_call_result_n322;
      }
      if (hxc_l_tmp_short_circuit_result_n164)
      {
        int32_t hxc_l_tmp_load_result_n324 = hxc_l_selectedMode;
        int32_t hxc_l_tmp_conditional_result_n165 = 0;
        if (hxc_l_tmp_load_result_n324 == 0)
        {
          hxc_l_tmp_conditional_result_n165 = 1;
        }
        else
        {
          hxc_l_tmp_conditional_result_n165 = 0;
        }
        hxc_l_selectedMode = hxc_l_tmp_conditional_result_n165;
      }
      struct Vector2 hxc_l_tmp_native_call_result_n326 = GetMousePosition();
      struct Vector2 hxc_l_menuMouse = hxc_l_tmp_native_call_result_n326;
      struct Vector2 hxc_l_tmp_load_result_n327 = hxc_l_menuMouse;
      struct Vector2 hxc_l_tmp_load_result_n329 = hxc_l_menuMouse;
      int32_t hxc_l_tmp_native_call_result_n331 = GetScreenWidth();
      int32_t hxc_l_tmp_native_call_result_n332 = GetScreenHeight();
      int32_t hxc_l_tmp_call_result_n333 = hxc_caxecraft_app_TitleMenu_selectionAt((double)hxc_l_tmp_load_result_n327.x, (double)hxc_l_tmp_load_result_n329.y, (int32_t)hxc_l_tmp_native_call_result_n331, (int32_t)hxc_l_tmp_native_call_result_n332);
      int32_t hxc_l_hovered = hxc_l_tmp_call_result_n333;
      if (hxc_l_hovered == 0)
      {
        hxc_l_selectedMode = 0;
      }
      if (hxc_l_hovered == 1)
      {
        hxc_l_selectedMode = 1;
      }
      int32_t hxc_l_tmp_load_result_n336 = hxc_l_selectedMode;
      if (hxc_l_tmp_load_result_n336 != hxc_l_modeBeforeInput)
      {
        hxc_l_accumulator = 0.0;
        hxc_l_resetMotionThisFrame = true;
      }
      int32_t hxc_l_tmp_load_result_n338 = hxc_l_hovered;
      bool hxc_l_tmp_short_circuit_result_n169 = hxc_l_tmp_load_result_n338 >= 0;
      if (hxc_l_tmp_load_result_n338 >= 0)
      {
        bool hxc_l_tmp_native_call_result_n339 = IsMouseButtonPressed((int32_t)0);
        hxc_l_tmp_short_circuit_result_n169 = hxc_l_tmp_native_call_result_n339;
      }
      bool hxc_l_clickedChoice = hxc_l_tmp_short_circuit_result_n169;
      bool hxc_l_tmp_load_result_n341 = hxc_l_clickedChoice;
      bool hxc_l_tmp_short_circuit_result_n170 = hxc_l_tmp_load_result_n341;
      if (hxc_l_tmp_load_result_n341)
      {
        hxc_l_tmp_short_circuit_result_n170 = hxc_l_hovered == 2;
      }
      if (hxc_l_tmp_short_circuit_result_n170)
      {
        enum hxc_caxecraft_app_AppScreen hxc_l_tmp_call_result_n345 = hxc_caxecraft_app_AppScreen_openEditor(hxc_l_screen);
        hxc_l_screen = hxc_l_tmp_call_result_n345;
        hxc_l_accumulator = 0.0;
        hxc_l_resetMotionThisFrame = true;
        EnableCursor();
      }
      else
      {
        bool hxc_l_tmp_load_result_n346 = hxc_l_clickedChoice;
        bool hxc_l_tmp_short_circuit_result_n171 = hxc_l_tmp_load_result_n346;
        if (!hxc_l_tmp_load_result_n346)
        {
          bool hxc_l_tmp_native_call_result_n347 = IsKeyPressed((int32_t)257);
          hxc_l_tmp_short_circuit_result_n171 = hxc_l_tmp_native_call_result_n347;
        }
        if (hxc_l_tmp_short_circuit_result_n171)
        {
          enum hxc_caxecraft_app_AppScreen hxc_l_tmp_call_result_n350 = hxc_caxecraft_app_AppScreen_startPlaying(hxc_l_screen);
          hxc_l_screen = hxc_l_tmp_call_result_n350;
          hxc_l_recapturedThisFrame = true;
          hxc_l_accumulator = 0.0;
          hxc_l_resetMotionThisFrame = true;
          DisableCursor();
        }
      }
    }
    bool hxc_l_tmp_load_result_n351 = hxc_l_focused;
    bool hxc_l_tmp_short_circuit_result_n172 = !hxc_l_tmp_load_result_n351;
    if (!hxc_l_tmp_load_result_n351)
    {
      bool hxc_l_tmp_call_result_n353 = hxc_caxecraft_app_AppScreen_isPlaying(hxc_l_screen);
      hxc_l_tmp_short_circuit_result_n172 = hxc_l_tmp_call_result_n353;
    }
    if (hxc_l_tmp_short_circuit_result_n172)
    {
      enum hxc_caxecraft_app_AppScreen hxc_l_tmp_call_result_n356 = hxc_caxecraft_app_AppScreen_loseFocus(hxc_l_screen);
      hxc_l_screen = hxc_l_tmp_call_result_n356;
      hxc_l_jumpQueued = false;
      hxc_l_accumulator = 0.0;
      hxc_l_resetMotionThisFrame = true;
      EnableCursor();
    }
    bool hxc_l_tmp_call_result_n358 = hxc_caxecraft_app_AppScreen_showsTitle(hxc_l_screen);
    bool hxc_l_tmp_short_circuit_result_n173 = !hxc_l_tmp_call_result_n358;
    if (!hxc_l_tmp_call_result_n358)
    {
      bool hxc_l_tmp_call_result_n360 = hxc_caxecraft_app_AppScreen_showsEditor(hxc_l_screen);
      hxc_l_tmp_short_circuit_result_n173 = !hxc_l_tmp_call_result_n360;
    }
    bool hxc_l_tmp_short_circuit_load_result_n361 = hxc_l_tmp_short_circuit_result_n173;
    bool hxc_l_tmp_short_circuit_result_n174 = hxc_l_tmp_short_circuit_load_result_n361;
    if (hxc_l_tmp_short_circuit_load_result_n361)
    {
      hxc_l_tmp_short_circuit_result_n174 = hxc_l_focused;
    }
    bool hxc_l_tmp_short_circuit_load_result_n363 = hxc_l_tmp_short_circuit_result_n174;
    bool hxc_l_tmp_short_circuit_result_n175 = hxc_l_tmp_short_circuit_load_result_n363;
    if (hxc_l_tmp_short_circuit_load_result_n363)
    {
      hxc_l_tmp_short_circuit_result_n175 = hxc_l_pausePressed_h04e9dd4c01d6;
    }
    if (hxc_l_tmp_short_circuit_result_n175)
    {
      enum hxc_caxecraft_app_AppScreen hxc_l_tmp_call_result_n367 = hxc_caxecraft_app_AppScreen_togglePause(hxc_l_screen);
      hxc_l_screen = hxc_l_tmp_call_result_n367;
      hxc_l_accumulator = 0.0;
      hxc_l_resetMotionThisFrame = true;
      bool hxc_l_tmp_call_result_n369 = hxc_caxecraft_app_AppScreen_pausesSimulation(hxc_l_screen);
      if (hxc_l_tmp_call_result_n369)
      {
        hxc_l_jumpQueued = false;
      }
      bool hxc_l_tmp_call_result_n371 = hxc_caxecraft_app_AppScreen_capturesPointer(hxc_l_screen);
      if (hxc_l_tmp_call_result_n371)
      {
        DisableCursor();
      }
      else
      {
        EnableCursor();
      }
    }
    bool hxc_l_tmp_call_result_n373 = hxc_caxecraft_app_AppScreen_showsTitle(hxc_l_screen);
    bool hxc_l_tmp_short_circuit_result_n176 = !hxc_l_tmp_call_result_n373;
    if (!hxc_l_tmp_call_result_n373)
    {
      bool hxc_l_tmp_call_result_n375 = hxc_caxecraft_app_AppScreen_showsEditor(hxc_l_screen);
      hxc_l_tmp_short_circuit_result_n176 = !hxc_l_tmp_call_result_n375;
    }
    bool hxc_l_tmp_short_circuit_load_result_n376 = hxc_l_tmp_short_circuit_result_n176;
    bool hxc_l_tmp_short_circuit_result_n177 = hxc_l_tmp_short_circuit_load_result_n376;
    if (hxc_l_tmp_short_circuit_load_result_n376)
    {
      hxc_l_tmp_short_circuit_result_n177 = hxc_l_focused;
    }
    bool hxc_l_tmp_short_circuit_load_result_n378 = hxc_l_tmp_short_circuit_result_n177;
    bool hxc_l_tmp_short_circuit_result_n178 = hxc_l_tmp_short_circuit_load_result_n378;
    if (hxc_l_tmp_short_circuit_load_result_n378)
    {
      bool hxc_l_tmp_call_result_n380 = hxc_caxecraft_app_AppScreen_pausesSimulation(hxc_l_screen);
      hxc_l_tmp_short_circuit_result_n178 = hxc_l_tmp_call_result_n380;
    }
    bool hxc_l_tmp_short_circuit_load_result_n381 = hxc_l_tmp_short_circuit_result_n178;
    bool hxc_l_tmp_short_circuit_result_n179 = hxc_l_tmp_short_circuit_load_result_n381;
    if (hxc_l_tmp_short_circuit_load_result_n381)
    {
      hxc_l_tmp_short_circuit_result_n179 = hxc_l_capturePressed;
    }
    if (hxc_l_tmp_short_circuit_result_n179)
    {
      enum hxc_caxecraft_app_AppScreen hxc_l_tmp_call_result_n385 = hxc_caxecraft_app_AppScreen_recapture(hxc_l_screen);
      hxc_l_screen = hxc_l_tmp_call_result_n385;
      hxc_l_recapturedThisFrame = true;
      hxc_l_accumulator = 0.0;
      hxc_l_resetMotionThisFrame = true;
      DisableCursor();
    }
    bool hxc_l_tmp_call_result_n387 = hxc_caxecraft_app_AppScreen_showsTitle(hxc_l_screen);
    bool hxc_l_onTitle = hxc_l_tmp_call_result_n387;
    bool hxc_l_tmp_call_result_n389 = hxc_caxecraft_app_AppScreen_showsEditor(hxc_l_screen);
    bool hxc_l_onEditor = hxc_l_tmp_call_result_n389;
    bool hxc_l_tmp_call_result_n391 = hxc_caxecraft_app_AppScreen_pausesSimulation(hxc_l_screen);
    bool hxc_l_paused_hc53526590c8c = hxc_l_tmp_call_result_n391;
    bool hxc_l_tmp_call_result_n393 = hxc_caxecraft_app_AppScreen_capturesPointer(hxc_l_screen);
    bool hxc_l_captured_h086e4d2008a5 = hxc_l_tmp_call_result_n393;
    int32_t hxc_l_editorNavigationCommand = 0;
    if (hxc_l_onEditor)
    {
      if (hxc_l_self == NULL)
      {
        abort();
      }
      struct hxc_caxecraft_input_NavigationRepeater *hxc_l_tmp_owned_class_field_address_n395 = &(*hxc_l_self).hxc_editorNavigation;
      if (hxc_l_tmp_owned_class_field_address_n395 == NULL)
      {
        abort();
      }
      struct hxc_caxecraft_input_NavigationSample hxc_l_tmp_call_result_n396 = hxc_caxecraft_app_RaylibNavigationInput_samplePrimaryGamepad();
      int32_t hxc_l_tmp_instance_call_result_n398 = hxc_caxecraft_input_NavigationRepeater_advance(hxc_l_tmp_owned_class_field_address_n395, hxc_l_tmp_call_result_n396, hxc_l_frameSeconds);
      hxc_l_editorNavigationCommand = hxc_l_tmp_instance_call_result_n398;
    }
    else
    {
      if (hxc_l_self == NULL)
      {
        abort();
      }
      struct hxc_caxecraft_input_NavigationRepeater *hxc_l_tmp_owned_class_field_address_n399 = &(*hxc_l_self).hxc_editorNavigation;
      if (hxc_l_tmp_owned_class_field_address_n399 == NULL)
      {
        abort();
      }
      hxc_caxecraft_input_NavigationRepeater_release(hxc_l_tmp_owned_class_field_address_n399);
    }
    if (hxc_l_captured_h086e4d2008a5)
    {
      double hxc_l_yawDelta = hxc_l_lookYaw_h4ec79428f0ea;
      if (hxc_l_yawDelta > 0.25)
      {
        hxc_l_yawDelta = 0.25;
      }
      if (hxc_l_yawDelta < -0.25)
      {
        hxc_l_yawDelta = -0.25;
      }
      double hxc_l_tmp_load_result_n404 = hxc_l_lookX;
      double hxc_l_tmp_load_result_n405 = hxc_l_yawDelta;
      double hxc_l_candidateX = hxc_l_tmp_load_result_n404 + hxc_l_tmp_load_result_n405 * hxc_l_lookZ;
      double hxc_l_tmp_load_result_n407 = hxc_l_lookZ;
      double hxc_l_tmp_load_result_n408 = hxc_l_yawDelta;
      double hxc_l_candidateZ = hxc_l_tmp_load_result_n407 - hxc_l_tmp_load_result_n408 * hxc_l_lookX;
      double hxc_l_tmp_load_result_n410 = hxc_l_candidateX;
      double hxc_l_tmp_load_result_n411 = hxc_l_candidateX;
      double hxc_l_tmp_load_result_n412 = hxc_l_candidateZ;
      double hxc_l_lengthSquared = hxc_l_tmp_load_result_n410 * hxc_l_tmp_load_result_n411 + hxc_l_tmp_load_result_n412 * hxc_l_candidateZ;
      double hxc_l_normalize = 1.5 - 0.5 * hxc_l_lengthSquared;
      double hxc_l_tmp_load_result_n415 = hxc_l_candidateX;
      hxc_l_lookX = hxc_l_tmp_load_result_n415 * hxc_l_normalize;
      double hxc_l_tmp_load_result_n417 = hxc_l_candidateZ;
      hxc_l_lookZ = hxc_l_tmp_load_result_n417 * hxc_l_normalize;
      double hxc_l_tmp_compound_load_result_n419 = hxc_l_lookY;
      hxc_l_lookY = hxc_l_tmp_compound_load_result_n419 + hxc_l_lookPitch_h1da1297fd85c;
      if (hxc_l_lookY > 0.90)
      {
        hxc_l_lookY = 0.90;
      }
      if (hxc_l_lookY < -0.90)
      {
        hxc_l_lookY = -0.90;
      }
      if (hxc_l_jumpPressed_h1cc68e15db02)
      {
        hxc_l_jumpQueued = true;
      }
    }
    bool hxc_l_tmp_load_result_n424 = hxc_l_captured_h086e4d2008a5;
    bool hxc_l_tmp_short_circuit_result_n190 = hxc_l_tmp_load_result_n424;
    if (hxc_l_tmp_load_result_n424)
    {
      hxc_l_tmp_short_circuit_result_n190 = !hxc_l_recapturedThisFrame;
    }
    bool hxc_l_tmp_short_circuit_load_result_n426 = hxc_l_tmp_short_circuit_result_n190;
    bool hxc_l_tmp_short_circuit_result_n191 = hxc_l_tmp_short_circuit_load_result_n426;
    if (hxc_l_tmp_short_circuit_load_result_n426)
    {
      hxc_l_tmp_short_circuit_result_n191 = hxc_l_primaryPressed_hff4c9f46b797;
    }
    bool hxc_l_tmp_short_circuit_load_result_n428 = hxc_l_tmp_short_circuit_result_n191;
    bool hxc_l_tmp_short_circuit_result_n192 = hxc_l_tmp_short_circuit_load_result_n428;
    if (hxc_l_tmp_short_circuit_load_result_n428)
    {
      hxc_l_tmp_short_circuit_result_n192 = hxc_l_selectedMode == 1;
    }
    bool hxc_l_tmp_short_circuit_load_result_n430 = hxc_l_tmp_short_circuit_result_n192;
    bool hxc_l_tmp_short_circuit_result_n193 = hxc_l_tmp_short_circuit_load_result_n430;
    if (hxc_l_tmp_short_circuit_load_result_n430)
    {
      bool hxc_l_tmp_call_result_n433 = hxc_caxecraft_gameplay_Inventory_selectedIs(hxc_l_inventory, hxc_caxecraft_gameplay_ItemKind_CopperSword);
      hxc_l_tmp_short_circuit_result_n193 = hxc_l_tmp_call_result_n433;
    }
    if (hxc_l_tmp_short_circuit_result_n193)
    {
      hxc_l_swordQueued = true;
    }
    if (!hxc_l_paused_hc53526590c8c)
    {
      double hxc_l_tmp_compound_load_result_n436 = hxc_l_accumulator;
      hxc_l_accumulator = hxc_l_tmp_compound_load_result_n436 + hxc_l_frameSeconds;
    }
    while (1)
    {
      bool hxc_l_tmp_load_result_n438 = hxc_l_paused_hc53526590c8c;
      bool hxc_l_tmp_short_circuit_result_n194 = !hxc_l_tmp_load_result_n438;
      if (!hxc_l_tmp_load_result_n438)
      {
        hxc_l_tmp_short_circuit_result_n194 = hxc_l_accumulator >= 0.05;
      }
      if (!hxc_l_tmp_short_circuit_result_n194)
      {
        break;
      }
      double hxc_l_tmp_load_result_n441 = hxc_l_moveForward;
      double hxc_l_tmp_load_result_n442 = hxc_l_lookX;
      double hxc_l_tmp_load_result_n443 = hxc_l_moveRight;
      double hxc_l_moveX = hxc_l_tmp_load_result_n441 * hxc_l_tmp_load_result_n442 - hxc_l_tmp_load_result_n443 * hxc_l_lookZ;
      double hxc_l_tmp_load_result_n445 = hxc_l_moveForward;
      double hxc_l_tmp_load_result_n446 = hxc_l_lookZ;
      double hxc_l_tmp_load_result_n447 = hxc_l_moveRight;
      double hxc_l_moveZ = hxc_l_tmp_load_result_n445 * hxc_l_tmp_load_result_n446 + hxc_l_tmp_load_result_n447 * hxc_l_lookX;
      double hxc_l_tmp_load_result_n449 = hxc_l_moveForward;
      bool hxc_l_tmp_short_circuit_result_n197 = hxc_l_tmp_load_result_n449 != 0.0;
      if (hxc_l_tmp_load_result_n449 != 0.0)
      {
        hxc_l_tmp_short_circuit_result_n197 = hxc_l_moveRight != 0.0;
      }
      if (hxc_l_tmp_short_circuit_result_n197)
      {
        hxc_l_moveX = hxc_l_moveX * 0.7071067811865476;
        hxc_l_moveZ = hxc_l_moveZ * 0.7071067811865476;
      }
      int32_t hxc_l_tmp_load_result_n454 = hxc_l_selectedMode;
      int32_t hxc_l_tmp_conditional_result_n199 = 0;
      if (hxc_l_tmp_load_result_n454 == 1)
      {
        hxc_l_tmp_conditional_result_n199 = 1;
      }
      else
      {
        hxc_l_tmp_conditional_result_n199 = 0;
      }
      int32_t hxc_l_damagePolicy = hxc_l_tmp_conditional_result_n199;
      struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n456 = hxc_l_session;
      hxc_l_gc_roots[56] = (const void *)hxc_l_tmp_load_result_n456;
      if (hxc_l_tmp_load_result_n456 == NULL)
      {
        abort();
      }
      double hxc_l_tmp_load_result_n457 = hxc_l_moveX;
      double hxc_l_tmp_load_result_n458 = hxc_l_moveZ;
      bool hxc_l_tmp_load_result_n459 = hxc_l_jumpQueued;
      struct hxc_caxecraft_domain_CharacterIntent hxc_l_tmp_call_result_n461 = hxc_caxecraft_domain_Aquatics_input(hxc_l_tmp_load_result_n457, hxc_l_tmp_load_result_n458, hxc_l_tmp_load_result_n459, hxc_l_descendHeld);
      struct hxc_caxecraft_domain_GameTickResult hxc_l_tmp_instance_call_result_n464 = hxc_caxecraft_domain_GameSession_tick(hxc_l_tmp_load_result_n456, (struct hxc_caxecraft_domain_GameTickInput){ .hxc_damagePolicy = hxc_l_damagePolicy, .hxc_intent = hxc_l_tmp_call_result_n461, .hxc_waterUpdateBudget = 64 });
      struct hxc_caxecraft_domain_GameTickResult hxc_l_gameTick = hxc_l_tmp_instance_call_result_n464;
      hxc_l_character = hxc_l_gameTick.hxc_character;
      if (hxc_l_gameTick.hxc_committed)
      {
        struct hxc_caxecraft_app_MotionHistory hxc_l_tmp_load_result_n467 = hxc_l_motionHistory;
        struct hxc_caxecraft_app_MotionHistory hxc_l_tmp_call_result_n469 = hxc_caxecraft_app_MotionInterpolation_advance(hxc_l_tmp_load_result_n467, hxc_l_character.hxc_body);
        hxc_l_motionHistory = hxc_l_tmp_call_result_n469;
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
          int32_t hxc_l_tmp_load_result_n473 = hxc_l_pickupIndex;
          int32_t hxc_l_tmp_array_length_result_n475;
          if (hxc_array_ref_length(hxc_l_loadedItems, &hxc_l_tmp_array_length_result_n475) != HXC_STATUS_OK)
          {
            abort();
          }
          if (!(hxc_l_tmp_load_result_n473 < hxc_l_tmp_array_length_result_n475))
          {
            break;
          }
          struct hxc_array_ref *hxc_l_tmp_load_result_n476 = hxc_l_loadedItems;
          struct hxc_caxecraft_app_LoadedWorldItem hxc_l_tmp_array_get_result_n478;
          if (hxc_array_ref_get_copy(hxc_l_tmp_load_result_n476, (size_t)hxc_l_pickupIndex, &hxc_l_tmp_array_get_result_n478) != HXC_STATUS_OK)
          {
            abort();
          }
          struct hxc_caxecraft_app_LoadedWorldItem hxc_l_loadedItem = hxc_l_tmp_array_get_result_n478;
          struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n479 = hxc_l_session;
          hxc_l_gc_roots[57] = (const void *)hxc_l_tmp_load_result_n479;
          if (hxc_l_tmp_load_result_n479 == NULL)
          {
            abort();
          }
          bool hxc_l_tmp_instance_call_result_n481 = hxc_caxecraft_domain_GameSession_authoredItemIsActive(hxc_l_tmp_load_result_n479, hxc_l_pickupIndex);
          bool hxc_l_tmp_short_circuit_result_n203 = hxc_l_tmp_instance_call_result_n481;
          if (hxc_l_tmp_instance_call_result_n481)
          {
            double hxc_l_tmp_record_field_load_result_n482 = hxc_l_character.hxc_body.hxc_x;
            double hxc_l_tmp_record_field_load_result_n483 = hxc_l_character.hxc_body.hxc_y;
            double hxc_l_tmp_record_field_load_result_n484 = hxc_l_character.hxc_body.hxc_z;
            int32_t hxc_l_tmp_record_field_load_result_n485 = hxc_l_loadedItem.hxc_xMilli;
            int32_t hxc_l_tmp_record_field_load_result_n486 = hxc_l_loadedItem.hxc_yMilli;
            bool hxc_l_tmp_call_result_n488 = hxc_caxecraft_gameplay_WorldItemPickup_isInRange(hxc_l_tmp_record_field_load_result_n482, hxc_l_tmp_record_field_load_result_n483, hxc_l_tmp_record_field_load_result_n484, hxc_l_tmp_record_field_load_result_n485, hxc_l_tmp_record_field_load_result_n486, hxc_l_loadedItem.hxc_zMilli);
            hxc_l_tmp_short_circuit_result_n203 = hxc_l_tmp_call_result_n488;
          }
          if (hxc_l_tmp_short_circuit_result_n203)
          {
            int32_t hxc_l_itemCode = hxc_l_loadedItem.hxc_storageCode;
            struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_tmp_load_result_n491 = hxc_l_contentRegistry;
            hxc_l_gc_roots[58] = (const void *)hxc_l_tmp_load_result_n491;
            if (hxc_l_tmp_load_result_n491 == NULL)
            {
              abort();
            }
            enum hxc_caxecraft_content_RuntimeItemUseProfile hxc_l_tmp_instance_call_result_n493 = hxc_caxecraft_content_RuntimeContentRegistry_itemUseProfile(hxc_l_tmp_load_result_n491, hxc_l_itemCode);
            bool hxc_l_tmp_short_circuit_result_n205 = hxc_l_tmp_instance_call_result_n493 == hxc_caxecraft_content_RuntimeItemUseProfile_EquipAquatic;
            if (hxc_l_tmp_instance_call_result_n493 == hxc_caxecraft_content_RuntimeItemUseProfile_EquipAquatic)
            {
              struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_tmp_load_result_n495 = hxc_l_contentRegistry;
              hxc_l_gc_roots[59] = (const void *)hxc_l_tmp_load_result_n495;
              if (hxc_l_tmp_load_result_n495 == NULL)
              {
                abort();
              }
              bool hxc_l_tmp_instance_call_result_n497 = hxc_caxecraft_content_RuntimeContentRegistry_itemProvidesAquaticProfile(hxc_l_tmp_load_result_n495, hxc_l_itemCode);
              hxc_l_tmp_short_circuit_result_n205 = hxc_l_tmp_instance_call_result_n497;
            }
            if (hxc_l_tmp_short_circuit_result_n205)
            {
              struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_tmp_load_result_n499 = hxc_l_contentRegistry;
              hxc_l_gc_roots[60] = (const void *)hxc_l_tmp_load_result_n499;
              if (hxc_l_tmp_load_result_n499 == NULL)
              {
                abort();
              }
              struct hxc_caxecraft_domain_AquaticProfile hxc_l_tmp_instance_call_result_n501 = hxc_caxecraft_content_RuntimeContentRegistry_itemAquaticProfile(hxc_l_tmp_load_result_n499, hxc_l_itemCode);
              struct hxc_caxecraft_domain_AquaticProfile hxc_l_replacement = hxc_l_tmp_instance_call_result_n501;
              struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n502 = hxc_l_session;
              hxc_l_gc_roots[61] = (const void *)hxc_l_tmp_load_result_n502;
              if (hxc_l_tmp_load_result_n502 == NULL)
              {
                abort();
              }
              int32_t hxc_l_tmp_load_result_n503 = hxc_l_pickupIndex;
              struct hxc_caxecraft_domain_AuthoredAquaticEquipmentResult hxc_l_tmp_instance_call_result_n505 = hxc_caxecraft_domain_GameSession_collectAuthoredAquaticEquipment(hxc_l_tmp_load_result_n502, hxc_l_tmp_load_result_n503, hxc_l_replacement);
              struct hxc_caxecraft_domain_AuthoredAquaticEquipmentResult hxc_l_equipment = hxc_l_tmp_instance_call_result_n505;
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
          struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n513 = hxc_l_session;
          hxc_l_gc_roots[62] = (const void *)hxc_l_tmp_load_result_n513;
          if (hxc_l_tmp_load_result_n513 == NULL)
          {
            abort();
          }
          int32_t hxc_l_tmp_record_field_load_result_n514 = hxc_l_gameTick.hxc_tickIndex;
          struct hxc_caxecraft_domain_ActorControllerTickResult hxc_l_tmp_instance_call_result_n516 = hxc_caxecraft_domain_GameSession_stepAuthoredActorControllers(hxc_l_tmp_load_result_n513, hxc_l_tmp_record_field_load_result_n514, hxc_l_damagePolicy);
          struct hxc_caxecraft_domain_ActorControllerTickResult hxc_l_actorTick = hxc_l_tmp_instance_call_result_n516;
          switch (hxc_l_actorTick.hxc_status.hxc_tag) {
            case hxc_caxecraft_domain_ActorControllerTickStatus_ControllersAdvanced:
              {
                hxc_l_guideInteractionAvailable = false;
                int32_t hxc_l_g_h38ebaa39278f = 0;
                struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n518 = hxc_l_session;
                hxc_l_gc_roots[66] = (const void *)hxc_l_tmp_load_result_n518;
                if (hxc_l_tmp_load_result_n518 == NULL)
                {
                  abort();
                }
                struct hxc_array_ref *hxc_l_tmp_instance_call_result_n519 = hxc_caxecraft_domain_GameSession_actorControllerEventSnapshots(hxc_l_tmp_load_result_n518);
                hxc_l_g1_h8a942d6677c9 = hxc_l_tmp_instance_call_result_n519;
                while (1)
                {
                  int32_t hxc_l_tmp_load_result_n520 = hxc_l_g_h38ebaa39278f;
                  int32_t hxc_l_tmp_array_length_result_n522;
                  if (hxc_array_ref_length(hxc_l_g1_h8a942d6677c9, &hxc_l_tmp_array_length_result_n522) != HXC_STATUS_OK)
                  {
                    abort();
                  }
                  if (!(hxc_l_tmp_load_result_n520 < hxc_l_tmp_array_length_result_n522))
                  {
                    break;
                  }
                  struct hxc_array_ref *hxc_l_tmp_load_result_n523 = hxc_l_g1_h8a942d6677c9;
                  struct hxc_caxecraft_domain_ActorControllerEvent hxc_l_tmp_array_get_result_n525;
                  if (hxc_array_ref_get_copy(hxc_l_tmp_load_result_n523, (size_t)hxc_l_g_h38ebaa39278f, &hxc_l_tmp_array_get_result_n525) != HXC_STATUS_OK)
                  {
                    abort();
                  }
                  hxc_l_tmp_array_element_owner_n212 = hxc_l_tmp_array_get_result_n525;
                  hxc_l_event = hxc_l_tmp_array_element_owner_n212;
                  if (hxc_enum_8b2a26b8_retain(&hxc_l_event) != HXC_STATUS_OK)
                  {
                    abort();
                  }
                  hxc_l_g_h38ebaa39278f = hxc_i32_add_wrapping(hxc_l_g_h38ebaa39278f, 1);
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
                        int32_t hxc_l_tmp_enum_payload_project_n530 = hxc_l_event.hxc_payload.hxc_InteractionAvailable.hxc_characterId;
                        int32_t hxc_l_source_h949e74edee1a = hxc_l_tmp_enum_payload_project_n530;
                        int32_t hxc_l_tmp_load_result_n531 = hxc_l_source_h949e74edee1a;
                        if (hxc_l_tmp_load_result_n531 == hxc_l_dialogueActorId)
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
                        int32_t hxc_l_tmp_enum_payload_project_n534 = hxc_l_event.hxc_payload.hxc_LocalPlayerAttack.hxc_characterId;
                        int32_t hxc_l_source_h5a71a337f260 = hxc_l_tmp_enum_payload_project_n534;
                        int32_t hxc_l_tmp_load_result_n535 = hxc_l_source_h5a71a337f260;
                        if (hxc_l_tmp_load_result_n535 == hxc_l_enemyActorId)
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
                        int32_t hxc_l_tmp_enum_payload_project_n538 = hxc_l_event.hxc_payload.hxc_DropRequested.hxc_characterId;
                        int32_t hxc_l_source_h29bbc7d9ddb9 = hxc_l_tmp_enum_payload_project_n538;
                        if (hxc_l_event.hxc_tag != hxc_caxecraft_domain_ActorControllerEvent_DropRequested)
                        {
                          abort();
                        }
                        hxc_string hxc_l_tmp_enum_payload_project_n540 = hxc_l_event.hxc_payload.hxc_DropRequested.hxc_drop;
                        hxc_l_drop = hxc_l_tmp_enum_payload_project_n540;
                        if (hxc_string_retain(hxc_l_drop) != HXC_STATUS_OK)
                        {
                          abort();
                        }
                        struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_tmp_load_result_n541 = hxc_l_contentRegistry;
                        hxc_l_gc_roots[67] = (const void *)hxc_l_tmp_load_result_n541;
                        if (hxc_l_tmp_load_result_n541 == NULL)
                        {
                          abort();
                        }
                        int32_t hxc_l_tmp_instance_call_result_n543 = hxc_caxecraft_content_RuntimeContentRegistry_dropQuantityById(hxc_l_tmp_load_result_n541, hxc_l_drop);
                        int32_t hxc_l_quantity = hxc_l_tmp_instance_call_result_n543;
                        struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n544 = hxc_l_session;
                        hxc_l_gc_roots[68] = (const void *)hxc_l_tmp_load_result_n544;
                        if (hxc_l_tmp_load_result_n544 == NULL)
                        {
                          abort();
                        }
                        struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n546 = hxc_caxecraft_domain_GameSession_readCharacter(hxc_l_tmp_load_result_n544, hxc_l_source_h29bbc7d9ddb9);
                        struct hxc_caxecraft_domain_Character hxc_l_defeatedActor = hxc_l_tmp_instance_call_result_n546;
                        int32_t hxc_l_tmp_load_result_n547 = hxc_l_quantity;
                        bool hxc_l_tmp_short_circuit_result_n219 = hxc_l_tmp_load_result_n547 <= 0;
                        if (!(hxc_l_tmp_load_result_n547 <= 0))
                        {
                          hxc_l_tmp_short_circuit_result_n219 = hxc_l_defeatedActor.hxc_id <= 0;
                        }
                        if (hxc_l_tmp_short_circuit_result_n219)
                        {
                          hxc_l_quit = true;
                        }
                        else
                        {
                          if (!hxc_l_berryDrop.hxc_active)
                          {
                            struct hxc_caxecraft_domain_Character hxc_l_tmp_load_result_n551 = hxc_l_defeatedActor;
                            struct hxc_caxecraft_gameplay_BerryDropState hxc_l_tmp_call_result_n553 = hxc_caxecraft_gameplay_BerryDrop_fromDefeatedCharacter(hxc_l_tmp_load_result_n551, hxc_l_quantity);
                            hxc_l_berryDrop = hxc_l_tmp_call_result_n553;
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
                  hxc_array_43e9f8ca_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n212);
                }
                if (hxc_array_ref_release(hxc_l_g1_h8a942d6677c9) != HXC_STATUS_OK)
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
        struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n554 = hxc_l_session;
        hxc_l_gc_roots[63] = (const void *)hxc_l_tmp_load_result_n554;
        if (hxc_l_tmp_load_result_n554 == NULL)
        {
          abort();
        }
        struct hxc_array_ref *hxc_l_tmp_instance_call_result_n555 = hxc_caxecraft_domain_GameSession_actorControllerStateSnapshots(hxc_l_tmp_load_result_n554);
        hxc_l_actorPhases = hxc_l_tmp_instance_call_result_n555;
        struct hxc_array_ref *hxc_l_tmp_load_result_n556 = hxc_l_actorPhases;
        struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation hxc_l_tmp_call_result_n559 = hxc_caxecraft_app_CaxecraftApp_observeActorPhase(hxc_l_tmp_load_result_n556, hxc_l_dialogueActorId, hxc_caxecraft_domain_ActorControllerPhase_Stationary);
        hxc_l_dialoguePhase = hxc_l_tmp_call_result_n559;
        struct hxc_array_ref *hxc_l_tmp_load_result_n560 = hxc_l_actorPhases;
        struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation hxc_l_tmp_call_result_n563 = hxc_caxecraft_app_CaxecraftApp_observeActorPhase(hxc_l_tmp_load_result_n560, hxc_l_enemyActorId, hxc_caxecraft_domain_ActorControllerPhase_Defeated);
        hxc_l_enemyPhase = hxc_l_tmp_call_result_n563;
        struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n564 = hxc_l_session;
        hxc_l_gc_roots[64] = (const void *)hxc_l_tmp_load_result_n564;
        if (hxc_l_tmp_load_result_n564 == NULL)
        {
          abort();
        }
        struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n566 = hxc_caxecraft_domain_GameSession_readCharacter(hxc_l_tmp_load_result_n564, hxc_l_dialogueActorId);
        hxc_l_dialogueActor = hxc_l_tmp_instance_call_result_n566;
        struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n567 = hxc_l_session;
        hxc_l_gc_roots[65] = (const void *)hxc_l_tmp_load_result_n567;
        if (hxc_l_tmp_load_result_n567 == NULL)
        {
          abort();
        }
        struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n569 = hxc_caxecraft_domain_GameSession_readCharacter(hxc_l_tmp_load_result_n567, hxc_l_enemyActorId);
        hxc_l_enemyActor = hxc_l_tmp_instance_call_result_n569;
        bool hxc_l_tmp_record_field_load_result_n570 = hxc_l_dialoguePhase.hxc_valid;
        bool hxc_l_tmp_short_circuit_result_n221 = !hxc_l_tmp_record_field_load_result_n570;
        if (!!hxc_l_tmp_record_field_load_result_n570)
        {
          hxc_l_tmp_short_circuit_result_n221 = !hxc_l_enemyPhase.hxc_valid;
        }
        bool hxc_l_tmp_short_circuit_load_result_n572 = hxc_l_tmp_short_circuit_result_n221;
        bool hxc_l_tmp_short_circuit_result_n222 = hxc_l_tmp_short_circuit_load_result_n572;
        if (!hxc_l_tmp_short_circuit_load_result_n572)
        {
          hxc_l_tmp_short_circuit_result_n222 = hxc_l_dialogueActor.hxc_id <= 0;
        }
        bool hxc_l_tmp_short_circuit_load_result_n574 = hxc_l_tmp_short_circuit_result_n222;
        bool hxc_l_tmp_short_circuit_result_n223 = hxc_l_tmp_short_circuit_load_result_n574;
        if (!hxc_l_tmp_short_circuit_load_result_n574)
        {
          hxc_l_tmp_short_circuit_result_n223 = hxc_l_enemyActor.hxc_id <= 0;
        }
        if (hxc_l_tmp_short_circuit_result_n223)
        {
          hxc_l_quit = true;
        }
        struct hxc_caxecraft_gameplay_SwordCombatState hxc_l_tmp_call_result_n578 = hxc_caxecraft_gameplay_SwordCombat_step(hxc_l_swordCombat);
        hxc_l_swordCombat = hxc_l_tmp_call_result_n578;
        if (hxc_l_swordQueued)
        {
          struct hxc_caxecraft_gameplay_SwordCombatState hxc_l_tmp_load_result_n580 = hxc_l_swordCombat;
          struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_load_result_n581 = hxc_l_inventory;
          struct hxc_caxecraft_domain_VitalsState hxc_l_tmp_record_field_load_result_n582 = hxc_l_character.hxc_vitals;
          struct hxc_caxecraft_domain_Character hxc_l_tmp_load_result_n583 = hxc_l_enemyActor;
          double hxc_l_tmp_record_field_load_result_n584 = hxc_l_character.hxc_body.hxc_x;
          double hxc_l_tmp_record_field_load_result_n585 = hxc_l_character.hxc_body.hxc_z;
          double hxc_l_tmp_load_result_n586 = hxc_l_lookX;
          int32_t hxc_l_tmp_call_result_n588 = hxc_caxecraft_gameplay_SwordCombat_decide(hxc_l_tmp_load_result_n580, hxc_l_tmp_load_result_n581, hxc_l_tmp_record_field_load_result_n582, hxc_l_tmp_load_result_n583, hxc_l_tmp_record_field_load_result_n584, hxc_l_tmp_record_field_load_result_n585, hxc_l_tmp_load_result_n586, hxc_l_lookZ);
          int32_t hxc_l_swordDecision = hxc_l_tmp_call_result_n588;
          if (hxc_l_swordDecision == 5)
          {
            struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n590 = hxc_l_session;
            hxc_l_gc_roots[69] = (const void *)hxc_l_tmp_load_result_n590;
            if (hxc_l_tmp_load_result_n590 == NULL)
            {
              abort();
            }
            struct hxc_caxecraft_domain_CharacterDamageResult hxc_l_tmp_instance_call_result_n592 = hxc_caxecraft_domain_GameSession_damageCharacter(hxc_l_tmp_load_result_n590, hxc_l_enemyActorId, 1);
            struct hxc_caxecraft_domain_CharacterDamageResult hxc_l_damage = hxc_l_tmp_instance_call_result_n592;
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
          int32_t hxc_l_tmp_load_result_n596 = hxc_l_swordDecision;
          struct hxc_caxecraft_gameplay_SwordCombatState hxc_l_tmp_call_result_n598 = hxc_caxecraft_gameplay_SwordCombat_after(hxc_l_tmp_load_result_n596, hxc_l_swordCombat);
          hxc_l_swordCombat = hxc_l_tmp_call_result_n598;
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
    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n603 = hxc_l_session;
    hxc_l_gc_roots[55] = (const void *)hxc_l_tmp_load_result_n603;
    if (hxc_l_tmp_load_result_n603 == NULL)
    {
      abort();
    }
    size_t hxc_l_tmp_length_n605;
    const uint8_t *hxc_l_tmp_instance_call_result_n604 = hxc_caxecraft_domain_GameSession_worldView(hxc_l_tmp_load_result_n603, &hxc_l_tmp_length_n605);
    double hxc_l_tmp_load_result_n606 = hxc_l_selectionEyeX;
    double hxc_l_tmp_load_result_n607 = hxc_l_selectionEyeY;
    double hxc_l_tmp_load_result_n608 = hxc_l_selectionEyeZ;
    double hxc_l_tmp_load_result_n609 = hxc_l_lookX;
    double hxc_l_tmp_load_result_n610 = hxc_l_lookY;
    struct hxc_caxecraft_domain_RaycastHit hxc_l_tmp_call_result_n612 = hxc_caxecraft_domain_VoxelRaycast_trace(hxc_l_tmp_instance_call_result_n604, hxc_l_tmp_length_n605, hxc_l_tmp_load_result_n606, hxc_l_tmp_load_result_n607, hxc_l_tmp_load_result_n608, hxc_l_tmp_load_result_n609, hxc_l_tmp_load_result_n610, hxc_l_lookZ, 7.0);
    struct hxc_caxecraft_domain_RaycastHit hxc_l_hit = hxc_l_tmp_call_result_n612;
    bool hxc_l_tmp_load_result_n613 = hxc_l_captured_h086e4d2008a5;
    bool hxc_l_tmp_short_circuit_result_n230 = hxc_l_tmp_load_result_n613;
    if (hxc_l_tmp_load_result_n613)
    {
      hxc_l_tmp_short_circuit_result_n230 = !hxc_l_recapturedThisFrame;
    }
    bool hxc_l_tmp_short_circuit_load_result_n615 = hxc_l_tmp_short_circuit_result_n230;
    bool hxc_l_tmp_short_circuit_result_n231 = hxc_l_tmp_short_circuit_load_result_n615;
    if (hxc_l_tmp_short_circuit_load_result_n615)
    {
      hxc_l_tmp_short_circuit_result_n231 = hxc_l_primaryPressed_hff4c9f46b797;
    }
    if (hxc_l_tmp_short_circuit_result_n231)
    {
      if (hxc_l_character.hxc_vitals.hxc_health > 0)
      {
        if (hxc_l_selectedMode == 1)
        {
          bool hxc_l_tmp_call_result_n622 = hxc_caxecraft_gameplay_Inventory_selectedIs(hxc_l_inventory, hxc_caxecraft_gameplay_ItemKind_CopperSword);
          bool hxc_l_tmp_short_circuit_result_n232 = !hxc_l_tmp_call_result_n622;
          if (!hxc_l_tmp_call_result_n622)
          {
            hxc_l_tmp_short_circuit_result_n232 = hxc_l_hit.hxc_hit;
          }
          bool hxc_l_tmp_short_circuit_load_result_n624 = hxc_l_tmp_short_circuit_result_n232;
          bool hxc_l_tmp_short_circuit_result_n233 = hxc_l_tmp_short_circuit_load_result_n624;
          if (hxc_l_tmp_short_circuit_load_result_n624)
          {
            struct hxc_caxecraft_domain_AquaticState hxc_l_tmp_record_field_load_result_n625 = hxc_l_character.hxc_aquatic;
            bool hxc_l_tmp_call_result_n627 = hxc_caxecraft_domain_Aquatics_canMine(hxc_l_tmp_record_field_load_result_n625, hxc_l_character.hxc_aquaticProfile);
            hxc_l_tmp_short_circuit_result_n233 = hxc_l_tmp_call_result_n627;
          }
          if (hxc_l_tmp_short_circuit_result_n233)
          {
            int32_t hxc_l_tmp_record_field_load_result_n629 = hxc_l_hit.hxc_cellX;
            int32_t hxc_l_tmp_record_field_load_result_n630 = hxc_l_hit.hxc_cellY;
            struct hxc_caxecraft_domain_BlockCoord hxc_l_tmp_call_result_n632 = hxc_caxecraft_domain_World_coord(hxc_l_tmp_record_field_load_result_n629, hxc_l_tmp_record_field_load_result_n630, hxc_l_hit.hxc_cellZ);
            struct hxc_caxecraft_domain_BlockCoord hxc_l_minedCoordinate = hxc_l_tmp_call_result_n632;
            struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n633 = hxc_l_session;
            hxc_l_gc_roots[70] = (const void *)hxc_l_tmp_load_result_n633;
            if (hxc_l_tmp_load_result_n633 == NULL)
            {
              abort();
            }
            struct hxc_caxecraft_domain_BlockCoord hxc_l_tmp_load_result_n634 = hxc_l_minedCoordinate;
            struct hxc_caxecraft_gameplay_MiningResult hxc_l_tmp_instance_call_result_n636 = hxc_caxecraft_domain_GameSession_mineTerrain(hxc_l_tmp_load_result_n633, hxc_l_tmp_load_result_n634, hxc_l_inventory);
            struct hxc_caxecraft_gameplay_MiningResult hxc_l_mining = hxc_l_tmp_instance_call_result_n636;
            hxc_l_inventory = hxc_l_mining.hxc_inventory;
            if (hxc_l_mining.hxc_outcome == 2)
            {
              if (hxc_l_self == NULL)
              {
                abort();
              }
              struct hxc_caxecraft_app_TerrainRenderer *hxc_l_tmp_owned_class_field_address_n639 = &(*hxc_l_self).hxc_terrainRenderer;
              struct hxc_caxecraft_app_TerrainChunkCache *hxc_l_tmp_owned_class_field_address_n640 = &(*hxc_l_tmp_owned_class_field_address_n639).hxc_cache;
              if (hxc_l_tmp_owned_class_field_address_n640 == NULL)
              {
                abort();
              }
              hxc_caxecraft_app_TerrainChunkCache_invalidate(hxc_l_tmp_owned_class_field_address_n640, hxc_l_minedCoordinate);
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
            int32_t hxc_l_tmp_record_field_load_result_n644 = hxc_l_hit.hxc_cellX;
            int32_t hxc_l_tmp_record_field_load_result_n645 = hxc_l_hit.hxc_cellY;
            struct hxc_caxecraft_domain_BlockCoord hxc_l_tmp_call_result_n647 = hxc_caxecraft_domain_World_coord(hxc_l_tmp_record_field_load_result_n644, hxc_l_tmp_record_field_load_result_n645, hxc_l_hit.hxc_cellZ);
            struct hxc_caxecraft_domain_BlockCoord hxc_l_removedCoordinate = hxc_l_tmp_call_result_n647;
            struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n648 = hxc_l_session;
            hxc_l_gc_roots[71] = (const void *)hxc_l_tmp_load_result_n648;
            if (hxc_l_tmp_load_result_n648 == NULL)
            {
              abort();
            }
            bool hxc_l_tmp_instance_call_result_n650 = hxc_caxecraft_domain_GameSession_removeTerrain(hxc_l_tmp_load_result_n648, hxc_l_removedCoordinate);
            bool hxc_l_removed = hxc_l_tmp_instance_call_result_n650;
            if (hxc_l_removed)
            {
              if (hxc_l_self == NULL)
              {
                abort();
              }
              struct hxc_caxecraft_app_TerrainRenderer *hxc_l_tmp_owned_class_field_address_n652 = &(*hxc_l_self).hxc_terrainRenderer;
              struct hxc_caxecraft_app_TerrainChunkCache *hxc_l_tmp_owned_class_field_address_n653 = &(*hxc_l_tmp_owned_class_field_address_n652).hxc_cache;
              if (hxc_l_tmp_owned_class_field_address_n653 == NULL)
              {
                abort();
              }
              hxc_caxecraft_app_TerrainChunkCache_invalidate(hxc_l_tmp_owned_class_field_address_n653, hxc_l_removedCoordinate);
            }
          }
        }
      }
    }
    bool hxc_l_tmp_load_result_n655 = hxc_l_captured_h086e4d2008a5;
    bool hxc_l_tmp_short_circuit_result_n238 = hxc_l_tmp_load_result_n655;
    if (hxc_l_tmp_load_result_n655)
    {
      hxc_l_tmp_short_circuit_result_n238 = hxc_l_secondaryPressed_hb34785c06c08;
    }
    if (hxc_l_tmp_short_circuit_result_n238)
    {
      if (hxc_l_character.hxc_vitals.hxc_health > 0)
      {
        struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n659 = hxc_l_session;
        hxc_l_gc_roots[72] = (const void *)hxc_l_tmp_load_result_n659;
        if (hxc_l_tmp_load_result_n659 == NULL)
        {
          abort();
        }
        struct hxc_caxecraft_domain_LocalRecoveryResult hxc_l_tmp_instance_call_result_n661 = hxc_caxecraft_domain_GameSession_useSelectedRecovery(hxc_l_tmp_load_result_n659, hxc_l_inventory);
        struct hxc_caxecraft_domain_LocalRecoveryResult hxc_l_recovery = hxc_l_tmp_instance_call_result_n661;
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
              int32_t hxc_l_tmp_record_field_load_result_n668 = hxc_l_hit.hxc_previousX;
              int32_t hxc_l_tmp_record_field_load_result_n669 = hxc_l_hit.hxc_previousY;
              struct hxc_caxecraft_domain_BlockCoord hxc_l_tmp_call_result_n671 = hxc_caxecraft_domain_World_coord(hxc_l_tmp_record_field_load_result_n668, hxc_l_tmp_record_field_load_result_n669, hxc_l_hit.hxc_previousZ);
              struct hxc_caxecraft_domain_BlockCoord hxc_l_placement = hxc_l_tmp_call_result_n671;
              enum hxc_caxecraft_domain_BlockKind hxc_l_tmp_call_result_n673 = hxc_caxecraft_gameplay_Inventory_selectedBlock(hxc_l_inventory);
              enum hxc_caxecraft_domain_BlockKind hxc_l_selectedBlock = hxc_l_tmp_call_result_n673;
              struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_load_result_n674 = hxc_l_inventory;
              int32_t hxc_l_tmp_call_result_n676 = hxc_caxecraft_gameplay_Inventory_countAt(hxc_l_tmp_load_result_n674, hxc_l_inventory.hxc_selected);
              bool hxc_l_hasItem = hxc_l_tmp_call_result_n676 > 0;
              bool hxc_l_tmp_load_result_n677 = hxc_l_hasItem;
              bool hxc_l_tmp_short_circuit_result_n243 = !hxc_l_tmp_load_result_n677;
              if (!!hxc_l_tmp_load_result_n677)
              {
                bool hxc_l_tmp_call_result_n679 = hxc_caxecraft_domain_World_isPlaceable(hxc_l_selectedBlock);
                hxc_l_tmp_short_circuit_result_n243 = !hxc_l_tmp_call_result_n679;
              }
              bool hxc_l_tmp_short_circuit_load_result_n680 = hxc_l_tmp_short_circuit_result_n243;
              bool hxc_l_tmp_short_circuit_result_n244 = hxc_l_tmp_short_circuit_load_result_n680;
              if (!hxc_l_tmp_short_circuit_load_result_n680)
              {
                struct hxc_caxecraft_domain_CharacterBody hxc_l_tmp_record_field_load_result_n681 = hxc_l_character.hxc_body;
                bool hxc_l_tmp_call_result_n683 = hxc_caxecraft_domain_CharacterPhysics_canPlaceAt(hxc_l_tmp_record_field_load_result_n681, hxc_l_placement);
                hxc_l_tmp_short_circuit_result_n244 = !hxc_l_tmp_call_result_n683;
              }
              bool hxc_l_tmp_short_circuit_load_result_n684 = hxc_l_tmp_short_circuit_result_n244;
              bool hxc_l_tmp_short_circuit_result_n245 = hxc_l_tmp_short_circuit_load_result_n684;
              if (!hxc_l_tmp_short_circuit_load_result_n684)
              {
                struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n685 = hxc_l_session;
                hxc_l_gc_roots[73] = (const void *)hxc_l_tmp_load_result_n685;
                if (hxc_l_tmp_load_result_n685 == NULL)
                {
                  abort();
                }
                struct hxc_caxecraft_domain_BlockCoord hxc_l_tmp_load_result_n686 = hxc_l_placement;
                bool hxc_l_tmp_instance_call_result_n688 = hxc_caxecraft_domain_GameSession_placeTerrain(hxc_l_tmp_load_result_n685, hxc_l_tmp_load_result_n686, hxc_l_selectedBlock);
                hxc_l_tmp_short_circuit_result_n245 = !hxc_l_tmp_instance_call_result_n688;
              }
              if (hxc_l_tmp_short_circuit_result_n245)
              {
                hxc_l_placementBlockedFrames = 60;
              }
              else
              {
                if (hxc_l_self == NULL)
                {
                  abort();
                }
                struct hxc_caxecraft_app_TerrainRenderer *hxc_l_tmp_owned_class_field_address_n690 = &(*hxc_l_self).hxc_terrainRenderer;
                struct hxc_caxecraft_app_TerrainChunkCache *hxc_l_tmp_owned_class_field_address_n691 = &(*hxc_l_tmp_owned_class_field_address_n690).hxc_cache;
                if (hxc_l_tmp_owned_class_field_address_n691 == NULL)
                {
                  abort();
                }
                hxc_caxecraft_app_TerrainChunkCache_invalidate(hxc_l_tmp_owned_class_field_address_n691, hxc_l_placement);
                if (hxc_l_selectedMode == 1)
                {
                  struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n695 = hxc_caxecraft_gameplay_Inventory_consumeSelected(hxc_l_inventory);
                  hxc_l_inventory = hxc_l_tmp_call_result_n695;
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
    if (!hxc_l_paused_hc53526590c8c)
    {
      struct hxc_caxecraft_gameplay_BerryDropState hxc_l_tmp_load_result_n699 = hxc_l_berryDrop;
      double hxc_l_tmp_record_field_load_result_n700 = hxc_l_character.hxc_body.hxc_x;
      double hxc_l_tmp_record_field_load_result_n701 = hxc_l_character.hxc_body.hxc_y;
      bool hxc_l_tmp_call_result_n703 = hxc_caxecraft_gameplay_BerryDrop_isInRange(hxc_l_tmp_load_result_n699, hxc_l_tmp_record_field_load_result_n700, hxc_l_tmp_record_field_load_result_n701, hxc_l_character.hxc_body.hxc_z);
      if (hxc_l_tmp_call_result_n703)
      {
        struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_load_result_n704 = hxc_l_inventory;
        int32_t hxc_l_tmp_call_result_n707 = hxc_caxecraft_gameplay_Inventory_acceptedAmount(hxc_l_tmp_load_result_n704, hxc_caxecraft_gameplay_ItemKind_Berries, hxc_l_berryDrop.hxc_amount);
        int32_t hxc_l_acceptedDrop = hxc_l_tmp_call_result_n707;
        if (hxc_l_acceptedDrop > 0)
        {
          struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_load_result_n709 = hxc_l_inventory;
          struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n712 = hxc_caxecraft_gameplay_Inventory_collectItem(hxc_l_tmp_load_result_n709, hxc_caxecraft_gameplay_ItemKind_Berries, hxc_l_acceptedDrop);
          hxc_l_inventory = hxc_l_tmp_call_result_n712;
          struct hxc_caxecraft_gameplay_BerryDropState hxc_l_tmp_load_result_n713 = hxc_l_berryDrop;
          struct hxc_caxecraft_gameplay_BerryDropState hxc_l_tmp_call_result_n715 = hxc_caxecraft_gameplay_BerryDrop_collectAmount(hxc_l_tmp_load_result_n713, hxc_l_acceptedDrop);
          hxc_l_berryDrop = hxc_l_tmp_call_result_n715;
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
    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n732 = hxc_l_session;
    hxc_l_gc_roots[74] = (const void *)hxc_l_tmp_load_result_n732;
    if (hxc_l_tmp_load_result_n732 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_domain_GameView hxc_l_tmp_instance_call_result_n733 = hxc_caxecraft_domain_GameSession_view(hxc_l_tmp_load_result_n732);
    struct hxc_caxecraft_domain_GameView hxc_l_committedView = hxc_l_tmp_instance_call_result_n733;
    if (!hxc_l_committedView.hxc_valid)
    {
      hxc_l_quit = true;
    }
    else
    {
      hxc_l_character = hxc_l_committedView.hxc_localPlayer;
    }
    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n736 = hxc_l_session;
    hxc_l_gc_roots[75] = (const void *)hxc_l_tmp_load_result_n736;
    if (hxc_l_tmp_load_result_n736 == NULL)
    {
      abort();
    }
    bool hxc_l_tmp_instance_call_result_n738 = hxc_caxecraft_domain_GameSession_actorInteractionAvailable(hxc_l_tmp_load_result_n736, hxc_l_dialogueActorId);
    hxc_l_guideInteractionAvailable = hxc_l_tmp_instance_call_result_n738;
    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n739 = hxc_l_session;
    hxc_l_gc_roots[76] = (const void *)hxc_l_tmp_load_result_n739;
    if (hxc_l_tmp_load_result_n739 == NULL)
    {
      abort();
    }
    struct hxc_array_ref *hxc_l_tmp_instance_call_result_n740 = hxc_caxecraft_domain_GameSession_actorControllerStateSnapshots(hxc_l_tmp_load_result_n739);
    hxc_l_presentationActorPhases = hxc_l_tmp_instance_call_result_n740;
    struct hxc_array_ref *hxc_l_tmp_load_result_n741 = hxc_l_presentationActorPhases;
    struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation hxc_l_tmp_call_result_n744 = hxc_caxecraft_app_CaxecraftApp_observeActorPhase(hxc_l_tmp_load_result_n741, hxc_l_dialogueActorId, hxc_caxecraft_domain_ActorControllerPhase_Stationary);
    hxc_l_dialoguePhase = hxc_l_tmp_call_result_n744;
    struct hxc_array_ref *hxc_l_tmp_load_result_n745 = hxc_l_presentationActorPhases;
    struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation hxc_l_tmp_call_result_n748 = hxc_caxecraft_app_CaxecraftApp_observeActorPhase(hxc_l_tmp_load_result_n745, hxc_l_enemyActorId, hxc_caxecraft_domain_ActorControllerPhase_Defeated);
    hxc_l_enemyPhase = hxc_l_tmp_call_result_n748;
    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n749 = hxc_l_session;
    hxc_l_gc_roots[77] = (const void *)hxc_l_tmp_load_result_n749;
    if (hxc_l_tmp_load_result_n749 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n751 = hxc_caxecraft_domain_GameSession_readCharacter(hxc_l_tmp_load_result_n749, hxc_l_dialogueActorId);
    hxc_l_dialogueActor = hxc_l_tmp_instance_call_result_n751;
    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n752 = hxc_l_session;
    hxc_l_gc_roots[78] = (const void *)hxc_l_tmp_load_result_n752;
    if (hxc_l_tmp_load_result_n752 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n754 = hxc_caxecraft_domain_GameSession_readCharacter(hxc_l_tmp_load_result_n752, hxc_l_enemyActorId);
    hxc_l_enemyActor = hxc_l_tmp_instance_call_result_n754;
    bool hxc_l_tmp_record_field_load_result_n755 = hxc_l_dialoguePhase.hxc_valid;
    bool hxc_l_tmp_short_circuit_result_n249 = !hxc_l_tmp_record_field_load_result_n755;
    if (!!hxc_l_tmp_record_field_load_result_n755)
    {
      hxc_l_tmp_short_circuit_result_n249 = !hxc_l_enemyPhase.hxc_valid;
    }
    bool hxc_l_tmp_short_circuit_load_result_n757 = hxc_l_tmp_short_circuit_result_n249;
    bool hxc_l_tmp_short_circuit_result_n250 = hxc_l_tmp_short_circuit_load_result_n757;
    if (!hxc_l_tmp_short_circuit_load_result_n757)
    {
      hxc_l_tmp_short_circuit_result_n250 = hxc_l_dialogueActor.hxc_id <= 0;
    }
    bool hxc_l_tmp_short_circuit_load_result_n759 = hxc_l_tmp_short_circuit_result_n250;
    bool hxc_l_tmp_short_circuit_result_n251 = hxc_l_tmp_short_circuit_load_result_n759;
    if (!hxc_l_tmp_short_circuit_load_result_n759)
    {
      hxc_l_tmp_short_circuit_result_n251 = hxc_l_enemyActor.hxc_id <= 0;
    }
    if (hxc_l_tmp_short_circuit_result_n251)
    {
      hxc_l_quit = true;
    }
    int32_t hxc_l_completedTicks = hxc_l_committedView.hxc_completedTicks;
    if (hxc_l_resetMotionThisFrame)
    {
      struct hxc_caxecraft_app_MotionHistory hxc_l_tmp_call_result_n765 = hxc_caxecraft_app_MotionInterpolation_start(hxc_l_character.hxc_body);
      hxc_l_motionHistory = hxc_l_tmp_call_result_n765;
    }
    struct hxc_caxecraft_app_MotionHistory hxc_l_tmp_load_result_n766 = hxc_l_motionHistory;
    struct hxc_caxecraft_app_RenderPosition hxc_l_tmp_call_result_n768 = hxc_caxecraft_app_MotionInterpolation_sample(hxc_l_tmp_load_result_n766, hxc_l_accumulator, 0.05);
    struct hxc_caxecraft_app_RenderPosition hxc_l_renderPosition = hxc_l_tmp_call_result_n768;
    double hxc_l_eyeX = hxc_l_renderPosition.hxc_x;
    double hxc_l_eyeY = hxc_l_renderPosition.hxc_y + 1.62;
    double hxc_l_eyeZ = hxc_l_renderPosition.hxc_z;
    double hxc_l_tmp_load_result_n772 = hxc_l_eyeX;
    double hxc_l_tmp_load_result_n773 = hxc_l_eyeY;
    double hxc_l_tmp_load_result_n774 = hxc_l_eyeZ;
    double hxc_l_tmp_load_result_n776 = hxc_l_eyeX;
    double hxc_l_tmp_load_result_n777 = hxc_l_lookX;
    double hxc_l_tmp_load_result_n778 = hxc_l_eyeY;
    double hxc_l_tmp_load_result_n779 = hxc_l_lookY;
    double hxc_l_tmp_load_result_n780 = hxc_l_eyeZ;
    struct Camera3D hxc_l_camera = (struct Camera3D){ .position = (struct Vector3){ .x = (float)hxc_l_tmp_load_result_n772, .y = (float)hxc_l_tmp_load_result_n773, .z = (float)hxc_l_tmp_load_result_n774 }, .target = (struct Vector3){ .x = (float)(hxc_l_tmp_load_result_n776 + hxc_l_tmp_load_result_n777), .y = (float)(hxc_l_tmp_load_result_n778 + hxc_l_tmp_load_result_n779), .z = (float)(hxc_l_tmp_load_result_n780 + hxc_l_lookZ) }, .up = (struct Vector3){ .x = (float)0.0, .y = (float)1.0, .z = (float)0.0 }, .fovy = (float)70.0, .projection = 0 };
    BeginDrawing();
    if (hxc_l_onTitle)
    {
      int32_t hxc_l_tmp_native_call_result_n786 = GetScreenWidth();
      int32_t hxc_l_width_h979e63fa3cf4 = (int32_t)hxc_l_tmp_native_call_result_n786;
      int32_t hxc_l_tmp_native_call_result_n787 = GetScreenHeight();
      int32_t hxc_l_height_hd14445000ca9 = (int32_t)hxc_l_tmp_native_call_result_n787;
      if (hxc_l_titleTextureReady)
      {
        struct Color hxc_l_this1_h26cc9e708fd5 = (struct Color){ .r = 255, .g = 255, .b = 255, .a = 255 };
        struct Texture hxc_l_tmp_load_result_n790 = hxc_l_titleTexture;
        double hxc_l_imageAspect = hxc_f64_divide_zero_safe((double)(int32_t)hxc_l_tmp_load_result_n790.width, (double)(int32_t)hxc_l_titleTexture.height);
        int32_t hxc_l_tmp_load_result_n794 = hxc_l_width_h979e63fa3cf4;
        double hxc_l_screenAspect = hxc_f64_divide_zero_safe((double)hxc_l_tmp_load_result_n794, (double)hxc_l_height_hd14445000ca9);
        double hxc_l_sourceX = 0.0;
        double hxc_l_sourceY = 0.0;
        double hxc_l_sourceWidth = (double)(int32_t)hxc_l_titleTexture.width;
        double hxc_l_sourceHeight = (double)(int32_t)hxc_l_titleTexture.height;
        double hxc_l_tmp_load_result_n800 = hxc_l_screenAspect;
        if (hxc_l_tmp_load_result_n800 > hxc_l_imageAspect)
        {
          struct Texture hxc_l_tmp_load_result_n802 = hxc_l_titleTexture;
          hxc_l_sourceHeight = hxc_f64_divide_zero_safe((double)(int32_t)hxc_l_tmp_load_result_n802.width, hxc_l_screenAspect);
          struct Texture hxc_l_tmp_load_result_n805 = hxc_l_titleTexture;
          hxc_l_sourceY = ((double)(int32_t)hxc_l_tmp_load_result_n805.height - hxc_l_sourceHeight) * 0.5;
        }
        else
        {
          struct Texture hxc_l_tmp_load_result_n808 = hxc_l_titleTexture;
          hxc_l_sourceWidth = (double)(int32_t)hxc_l_tmp_load_result_n808.height * hxc_l_screenAspect;
          struct Texture hxc_l_tmp_load_result_n811 = hxc_l_titleTexture;
          hxc_l_sourceX = ((double)(int32_t)hxc_l_tmp_load_result_n811.width - hxc_l_sourceWidth) * 0.5;
        }
        struct Texture hxc_l_tmp_load_result_n814 = hxc_l_titleTexture;
        double hxc_l_tmp_load_result_n815 = hxc_l_sourceX;
        double hxc_l_tmp_load_result_n816 = hxc_l_sourceY;
        double hxc_l_tmp_load_result_n817 = hxc_l_sourceWidth;
        double hxc_l_tmp_load_result_n818 = hxc_l_sourceHeight;
        int32_t hxc_l_tmp_load_result_n820 = hxc_l_width_h979e63fa3cf4;
        int32_t hxc_l_tmp_load_result_n821 = hxc_l_height_hd14445000ca9;
        DrawTexturePro(hxc_l_tmp_load_result_n814, (struct Rectangle){ .x = (float)hxc_l_tmp_load_result_n815, .y = (float)hxc_l_tmp_load_result_n816, .width = (float)hxc_l_tmp_load_result_n817, .height = (float)hxc_l_tmp_load_result_n818 }, (struct Rectangle){ .x = (float)0.0, .y = (float)0.0, .width = (float)((double)hxc_l_tmp_load_result_n820 + 0.0), .height = (float)((double)hxc_l_tmp_load_result_n821 + 0.0) }, (struct Vector2){ .x = (float)0.0, .y = (float)0.0 }, (float)0.0, hxc_l_this1_h26cc9e708fd5);
      }
      else
      {
        ClearBackground((struct Color){ .r = 126, .g = 190, .b = 201, .a = 255 });
      }
      struct Color hxc_l_this1_h9cec163c7c6c = (struct Color){ .r = 4, .g = 14, .b = 23, .a = 132 };
      int32_t hxc_l_tmp_load_result_n827 = hxc_l_width_h979e63fa3cf4;
      int32_t hxc_l_tmp_load_result_n828 = hxc_l_height_hd14445000ca9;
      DrawRectangle((int32_t)0, (int32_t)0, (int32_t)hxc_l_tmp_load_result_n827, (int32_t)hxc_l_tmp_load_result_n828, hxc_l_this1_h9cec163c7c6c);
      if (hxc_l_wordmarkTextureReady)
      {
        int32_t hxc_l_centerX = hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_l_width_h979e63fa3cf4, (double)2));
        struct Color hxc_l_this1_h5f78b7113fae = (struct Color){ .r = 255, .g = 255, .b = 255, .a = 255 };
        double hxc_l_scale = hxc_f64_divide_zero_safe((double)560, (double)(int32_t)hxc_l_wordmarkTexture.width);
        double hxc_l_heightScale = hxc_f64_divide_zero_safe((double)132, (double)(int32_t)hxc_l_wordmarkTexture.height);
        double hxc_l_tmp_load_result_n837 = hxc_l_heightScale;
        if (hxc_l_tmp_load_result_n837 < hxc_l_scale)
        {
          hxc_l_scale = hxc_l_heightScale;
        }
        struct Texture hxc_l_tmp_load_result_n840 = hxc_l_wordmarkTexture;
        double hxc_l_width_h5d1ca2fe1f54 = (double)(int32_t)hxc_l_tmp_load_result_n840.width * hxc_l_scale;
        struct Texture hxc_l_tmp_load_result_n843 = hxc_l_wordmarkTexture;
        double hxc_l_height_h123b9edcf76d = (double)(int32_t)hxc_l_tmp_load_result_n843.height * hxc_l_scale;
        double hxc_l_width1 = (double)(int32_t)hxc_l_wordmarkTexture.width + 0.0;
        double hxc_l_height1 = (double)(int32_t)hxc_l_wordmarkTexture.height + 0.0;
        struct Texture hxc_l_tmp_load_result_n850 = hxc_l_wordmarkTexture;
        double hxc_l_tmp_load_result_n851 = hxc_l_width1;
        double hxc_l_tmp_load_result_n852 = hxc_l_height1;
        int32_t hxc_l_tmp_load_result_n854 = hxc_l_centerX;
        double hxc_l_tmp_load_result_n855 = hxc_l_width_h5d1ca2fe1f54;
        double hxc_l_tmp_load_result_n856 = hxc_l_width_h5d1ca2fe1f54;
        double hxc_l_tmp_load_result_n857 = hxc_l_height_h123b9edcf76d;
        DrawTexturePro(hxc_l_tmp_load_result_n850, (struct Rectangle){ .x = (float)0.0, .y = (float)0.0, .width = (float)hxc_l_tmp_load_result_n851, .height = (float)hxc_l_tmp_load_result_n852 }, (struct Rectangle){ .x = (float)((double)hxc_l_tmp_load_result_n854 - hxc_l_tmp_load_result_n855 * 0.5), .y = (float)28.0, .width = (float)hxc_l_tmp_load_result_n856, .height = (float)hxc_l_tmp_load_result_n857 }, (struct Vector2){ .x = (float)0.0, .y = (float)0.0 }, (float)0.0, hxc_l_this1_h5f78b7113fae);
      }
      else
      {
        int32_t hxc_l_x_h82c4b8797ce3 = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_l_width_h979e63fa3cf4, (double)2)), 92);
        struct Color hxc_l_this1_ha508043e0bbb = (struct Color){ .r = 242, .g = 249, .b = 245, .a = 255 };
        struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_tmp_load_result_n863 = hxc_l_uiCatalog;
        hxc_l_gc_roots[79] = (const void *)hxc_l_tmp_load_result_n863;
        if (hxc_l_tmp_load_result_n863 == NULL)
        {
          abort();
        }
        hxc_string hxc_l_tmp_instance_call_result_n865 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_tmp_load_result_n863, hxc_l_locale, 34);
        hxc_l_tmp_cstring_ref_owner_owner_n278 = hxc_l_tmp_instance_call_result_n865;
        hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n867 = HXC_BORROWED_CSTRING_INITIALIZER;
        if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n278, &hxc_l_tmp_string_borrow_cstring_result_n867) != HXC_STATUS_OK)
        {
          abort();
        }
        int32_t hxc_l_tmp_load_result_n868 = hxc_l_x_h82c4b8797ce3;
        DrawText(hxc_l_tmp_string_borrow_cstring_result_n867.data, (int32_t)hxc_l_tmp_load_result_n868, (int32_t)52, (int32_t)34, hxc_l_this1_ha508043e0bbb);
        if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n278) != HXC_STATUS_OK)
        {
          abort();
        }
      }
      int32_t hxc_l_firstTop = hxc_i32_subtract_wrapping(hxc_l_height_hd14445000ca9, 293);
      int32_t hxc_l_left_h4d856de50763 = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_l_width_h979e63fa3cf4, (double)2)), 180);
      if (hxc_l_selectedMode == 0)
      {
        struct Color hxc_l_this1_h432df9a6585d = (struct Color){ .r = 16, .g = 88, .b = 102, .a = 232 };
        int32_t hxc_l_tmp_load_result_n874 = hxc_l_left_h4d856de50763;
        int32_t hxc_l_tmp_load_result_n875 = hxc_l_firstTop;
        DrawRectangle((int32_t)hxc_l_tmp_load_result_n874, (int32_t)hxc_l_tmp_load_result_n875, (int32_t)360, (int32_t)54, hxc_l_this1_h432df9a6585d);
        struct Color hxc_l_this1_h9f7bc1fe91c6 = (struct Color){ .r = 255, .g = 190, .b = 55, .a = 255 };
        int32_t hxc_l_tmp_load_result_n878 = hxc_l_left_h4d856de50763;
        int32_t hxc_l_tmp_load_result_n879 = hxc_l_firstTop;
        DrawRectangleLines((int32_t)hxc_l_tmp_load_result_n878, (int32_t)hxc_l_tmp_load_result_n879, (int32_t)360, (int32_t)54, hxc_l_this1_h9f7bc1fe91c6);
      }
      else
      {
        struct Color hxc_l_this1_he6dc64b070aa = (struct Color){ .r = 6, .g = 26, .b = 36, .a = 218 };
        int32_t hxc_l_tmp_load_result_n882 = hxc_l_left_h4d856de50763;
        int32_t hxc_l_tmp_load_result_n883 = hxc_l_firstTop;
        DrawRectangle((int32_t)hxc_l_tmp_load_result_n882, (int32_t)hxc_l_tmp_load_result_n883, (int32_t)360, (int32_t)54, hxc_l_this1_he6dc64b070aa);
        struct Color hxc_l_this1_hc051046c2c65 = (struct Color){ .r = 92, .g = 194, .b = 188, .a = 255 };
        int32_t hxc_l_tmp_load_result_n886 = hxc_l_left_h4d856de50763;
        int32_t hxc_l_tmp_load_result_n887 = hxc_l_firstTop;
        DrawRectangleLines((int32_t)hxc_l_tmp_load_result_n886, (int32_t)hxc_l_tmp_load_result_n887, (int32_t)360, (int32_t)54, hxc_l_this1_hc051046c2c65);
      }
      struct Color hxc_l_this1_h0fb67bbe478e = (struct Color){ .r = 242, .g = 249, .b = 245, .a = 255 };
      struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_tmp_load_result_n890 = hxc_l_uiCatalog;
      hxc_l_gc_roots[80] = (const void *)hxc_l_tmp_load_result_n890;
      if (hxc_l_tmp_load_result_n890 == NULL)
      {
        abort();
      }
      hxc_string hxc_l_tmp_instance_call_result_n892 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_tmp_load_result_n890, hxc_l_locale, 27);
      hxc_l_tmp_cstring_ref_owner_owner_n286 = hxc_l_tmp_instance_call_result_n892;
      hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n894 = HXC_BORROWED_CSTRING_INITIALIZER;
      if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n286, &hxc_l_tmp_string_borrow_cstring_result_n894) != HXC_STATUS_OK)
      {
        abort();
      }
      int32_t hxc_l_tmp_load_result_n895 = hxc_l_left_h4d856de50763;
      int32_t hxc_l_tmp_load_result_n896 = hxc_l_firstTop;
      DrawText(hxc_l_tmp_string_borrow_cstring_result_n894.data, (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n895, 28), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n896, 16), (int32_t)21, hxc_l_this1_h0fb67bbe478e);
      int32_t hxc_l_top_h77c72ad38df9 = hxc_i32_add_wrapping(hxc_i32_add_wrapping(hxc_l_firstTop, 54), 14);
      int32_t hxc_l_left_h4fc0dd0766e2 = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_l_width_h979e63fa3cf4, (double)2)), 180);
      if (hxc_l_selectedMode == 1)
      {
        struct Color hxc_l_this1_he661ad3f00e6 = (struct Color){ .r = 16, .g = 88, .b = 102, .a = 232 };
        int32_t hxc_l_tmp_load_result_n902 = hxc_l_left_h4fc0dd0766e2;
        int32_t hxc_l_tmp_load_result_n903 = hxc_l_top_h77c72ad38df9;
        DrawRectangle((int32_t)hxc_l_tmp_load_result_n902, (int32_t)hxc_l_tmp_load_result_n903, (int32_t)360, (int32_t)54, hxc_l_this1_he661ad3f00e6);
        struct Color hxc_l_this1_hb94710a96b12 = (struct Color){ .r = 255, .g = 190, .b = 55, .a = 255 };
        int32_t hxc_l_tmp_load_result_n906 = hxc_l_left_h4fc0dd0766e2;
        int32_t hxc_l_tmp_load_result_n907 = hxc_l_top_h77c72ad38df9;
        DrawRectangleLines((int32_t)hxc_l_tmp_load_result_n906, (int32_t)hxc_l_tmp_load_result_n907, (int32_t)360, (int32_t)54, hxc_l_this1_hb94710a96b12);
      }
      else
      {
        struct Color hxc_l_this1_h6eae5be6b0c8 = (struct Color){ .r = 6, .g = 26, .b = 36, .a = 218 };
        int32_t hxc_l_tmp_load_result_n910 = hxc_l_left_h4fc0dd0766e2;
        int32_t hxc_l_tmp_load_result_n911 = hxc_l_top_h77c72ad38df9;
        DrawRectangle((int32_t)hxc_l_tmp_load_result_n910, (int32_t)hxc_l_tmp_load_result_n911, (int32_t)360, (int32_t)54, hxc_l_this1_h6eae5be6b0c8);
        struct Color hxc_l_this1_h1b314e7b5d9d = (struct Color){ .r = 92, .g = 194, .b = 188, .a = 255 };
        int32_t hxc_l_tmp_load_result_n914 = hxc_l_left_h4fc0dd0766e2;
        int32_t hxc_l_tmp_load_result_n915 = hxc_l_top_h77c72ad38df9;
        DrawRectangleLines((int32_t)hxc_l_tmp_load_result_n914, (int32_t)hxc_l_tmp_load_result_n915, (int32_t)360, (int32_t)54, hxc_l_this1_h1b314e7b5d9d);
      }
      struct Color hxc_l_this1_he7d320013f59 = (struct Color){ .r = 242, .g = 249, .b = 245, .a = 255 };
      struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_tmp_load_result_n918 = hxc_l_uiCatalog;
      hxc_l_gc_roots[81] = (const void *)hxc_l_tmp_load_result_n918;
      if (hxc_l_tmp_load_result_n918 == NULL)
      {
        abort();
      }
      hxc_string hxc_l_tmp_instance_call_result_n920 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_tmp_load_result_n918, hxc_l_locale, 26);
      hxc_l_tmp_cstring_ref_owner_owner_n294 = hxc_l_tmp_instance_call_result_n920;
      hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n922 = HXC_BORROWED_CSTRING_INITIALIZER;
      if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n294, &hxc_l_tmp_string_borrow_cstring_result_n922) != HXC_STATUS_OK)
      {
        abort();
      }
      int32_t hxc_l_tmp_load_result_n923 = hxc_l_left_h4fc0dd0766e2;
      int32_t hxc_l_tmp_load_result_n924 = hxc_l_top_h77c72ad38df9;
      DrawText(hxc_l_tmp_string_borrow_cstring_result_n922.data, (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n923, 28), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n924, 16), (int32_t)21, hxc_l_this1_he7d320013f59);
      int32_t hxc_l_top_h38c9cdb76215 = hxc_i32_add_wrapping(hxc_l_firstTop, 136);
      int32_t hxc_l_left_hfc5b1e31c3b1 = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_l_width_h979e63fa3cf4, (double)2)), 180);
      struct Color hxc_l_this1_h1986656be165 = (struct Color){ .r = 6, .g = 26, .b = 36, .a = 218 };
      int32_t hxc_l_tmp_load_result_n929 = hxc_l_left_hfc5b1e31c3b1;
      int32_t hxc_l_tmp_load_result_n930 = hxc_l_top_h38c9cdb76215;
      DrawRectangle((int32_t)hxc_l_tmp_load_result_n929, (int32_t)hxc_l_tmp_load_result_n930, (int32_t)360, (int32_t)54, hxc_l_this1_h1986656be165);
      struct Color hxc_l_this1_h3939efaf654e = (struct Color){ .r = 92, .g = 194, .b = 188, .a = 255 };
      int32_t hxc_l_tmp_load_result_n933 = hxc_l_left_hfc5b1e31c3b1;
      int32_t hxc_l_tmp_load_result_n934 = hxc_l_top_h38c9cdb76215;
      DrawRectangleLines((int32_t)hxc_l_tmp_load_result_n933, (int32_t)hxc_l_tmp_load_result_n934, (int32_t)360, (int32_t)54, hxc_l_this1_h3939efaf654e);
      struct Color hxc_l_this1_haf43176cbb21 = (struct Color){ .r = 242, .g = 249, .b = 245, .a = 255 };
      struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_tmp_load_result_n937 = hxc_l_uiCatalog;
      hxc_l_gc_roots[82] = (const void *)hxc_l_tmp_load_result_n937;
      if (hxc_l_tmp_load_result_n937 == NULL)
      {
        abort();
      }
      hxc_string hxc_l_tmp_instance_call_result_n939 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_tmp_load_result_n937, hxc_l_locale, 28);
      hxc_l_tmp_cstring_ref_owner_owner_n300 = hxc_l_tmp_instance_call_result_n939;
      hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n941 = HXC_BORROWED_CSTRING_INITIALIZER;
      if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n300, &hxc_l_tmp_string_borrow_cstring_result_n941) != HXC_STATUS_OK)
      {
        abort();
      }
      int32_t hxc_l_tmp_load_result_n942 = hxc_l_left_hfc5b1e31c3b1;
      int32_t hxc_l_tmp_load_result_n943 = hxc_l_top_h38c9cdb76215;
      DrawText(hxc_l_tmp_string_borrow_cstring_result_n941.data, (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n942, 28), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n943, 16), (int32_t)21, hxc_l_this1_haf43176cbb21);
      int32_t hxc_l_x_h1d8e48adeb93 = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_l_width_h979e63fa3cf4, (double)2)), 285);
      struct Color hxc_l_this1_h6cfd7f210e8b = (struct Color){ .r = 229, .g = 241, .b = 235, .a = 255 };
      struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_tmp_load_result_n947 = hxc_l_uiCatalog;
      hxc_l_gc_roots[83] = (const void *)hxc_l_tmp_load_result_n947;
      if (hxc_l_tmp_load_result_n947 == NULL)
      {
        abort();
      }
      hxc_string hxc_l_tmp_instance_call_result_n949 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_tmp_load_result_n947, hxc_l_locale, 29);
      hxc_l_tmp_cstring_ref_owner_owner_n303 = hxc_l_tmp_instance_call_result_n949;
      hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n951 = HXC_BORROWED_CSTRING_INITIALIZER;
      if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n303, &hxc_l_tmp_string_borrow_cstring_result_n951) != HXC_STATUS_OK)
      {
        abort();
      }
      int32_t hxc_l_tmp_load_result_n952 = hxc_l_x_h1d8e48adeb93;
      int32_t hxc_l_tmp_load_result_n953 = hxc_l_height_hd14445000ca9;
      DrawText(hxc_l_tmp_string_borrow_cstring_result_n951.data, (int32_t)hxc_l_tmp_load_result_n952, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n953, 58), (int32_t)16, hxc_l_this1_h6cfd7f210e8b);
      if (hxc_l_selectedMode == 1)
      {
        int32_t hxc_l_x_hbb90ebd7b5ae = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_l_width_h979e63fa3cf4, (double)2)), 230);
        struct Color hxc_l_this1_hd6ecd99b39e5 = (struct Color){ .r = 255, .g = 205, .b = 91, .a = 255 };
        const char *hxc_l_tmp_call_result_n959 = hxc_caxecraft_localization_FirstPlayableCatalog_text(hxc_l_locale, 1);
        int32_t hxc_l_tmp_load_result_n960 = hxc_l_x_hbb90ebd7b5ae;
        int32_t hxc_l_tmp_load_result_n961 = hxc_l_firstTop;
        DrawText(hxc_l_tmp_call_result_n959, (int32_t)hxc_l_tmp_load_result_n960, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n961, 36), (int32_t)17, hxc_l_this1_hd6ecd99b39e5);
      }
      if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n303) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n300) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n294) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n286) != HXC_STATUS_OK)
      {
        abort();
      }
    }
    else
    {
      if (hxc_l_onEditor)
      {
        struct hxc_caxecraft_app_CaxecraftEditorScreen *hxc_l_tmp_load_result_n964 = hxc_l_editorScreen;
        hxc_l_gc_roots[84] = (const void *)hxc_l_tmp_load_result_n964;
        if (hxc_l_tmp_load_result_n964 == NULL)
        {
          abort();
        }
        int32_t hxc_l_tmp_load_result_n965 = hxc_l_locale;
        enum hxc_caxecraft_app_EditorScreenAction hxc_l_tmp_instance_call_result_n967 = hxc_caxecraft_app_CaxecraftEditorScreen_draw(hxc_l_tmp_load_result_n964, hxc_l_tmp_load_result_n965, hxc_l_editorNavigationCommand);
        if (hxc_l_tmp_instance_call_result_n967 == hxc_caxecraft_app_EditorScreenAction_ReturnToTitle)
        {
          enum hxc_caxecraft_app_AppScreen hxc_l_tmp_call_result_n970 = hxc_caxecraft_app_AppScreen_closeEditor(hxc_l_screen);
          hxc_l_screen = hxc_l_tmp_call_result_n970;
        }
      }
      else
      {
        ClearBackground((struct Color){ .r = 126, .g = 190, .b = 201, .a = 255 });
        int32_t hxc_l_tmp_native_call_result_n972 = GetScreenWidth();
        int32_t hxc_l_sunX = hxc_i32_subtract_wrapping((int32_t)hxc_l_tmp_native_call_result_n972, 260);
        float hxc_l_radius_h45fff96bafe7 = (float)42.0;
        struct Color hxc_l_this1_h02e5bb203f76 = (struct Color){ .r = 255, .g = 226, .b = 136, .a = 255 };
        int32_t hxc_l_tmp_load_result_n974 = hxc_l_sunX;
        float hxc_l_tmp_load_result_n975 = hxc_l_radius_h45fff96bafe7;
        DrawCircle((int32_t)hxc_l_tmp_load_result_n974, (int32_t)86, hxc_l_tmp_load_result_n975, hxc_l_this1_h02e5bb203f76);
        float hxc_l_radius_heed1d337136f = (float)30.0;
        struct Color hxc_l_this1_h79468f0959d2 = (struct Color){ .r = 255, .g = 246, .b = 205, .a = 255 };
        int32_t hxc_l_tmp_load_result_n978 = hxc_l_sunX;
        float hxc_l_tmp_load_result_n979 = hxc_l_radius_heed1d337136f;
        DrawCircle((int32_t)hxc_l_tmp_load_result_n978, (int32_t)86, hxc_l_tmp_load_result_n979, hxc_l_this1_h79468f0959d2);
        BeginMode3D(hxc_l_camera);
        if (hxc_l_self == NULL)
        {
          abort();
        }
        struct hxc_caxecraft_app_TerrainRenderer *hxc_l_tmp_owned_class_field_address_n982 = &(*hxc_l_self).hxc_terrainRenderer;
        if (hxc_l_tmp_owned_class_field_address_n982 == NULL)
        {
          abort();
        }
        struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n983 = hxc_l_session;
        hxc_l_gc_roots[85] = (const void *)hxc_l_tmp_load_result_n983;
        if (hxc_l_tmp_load_result_n983 == NULL)
        {
          abort();
        }
        size_t hxc_l_tmp_length_n985;
        const uint8_t *hxc_l_tmp_instance_call_result_n984 = hxc_caxecraft_domain_GameSession_worldView(hxc_l_tmp_load_result_n983, &hxc_l_tmp_length_n985);
        struct Texture hxc_l_tmp_load_result_n986 = hxc_l_terrainTexture;
        bool hxc_l_tmp_load_result_n987 = hxc_l_terrainTextureReady;
        struct Texture hxc_l_tmp_load_result_n988 = hxc_l_adventureTerrainTexture;
        bool hxc_l_tmp_load_result_n989 = hxc_l_adventureTerrainTextureReady;
        double hxc_l_tmp_record_field_load_result_n990 = hxc_l_renderPosition.hxc_x;
        struct hxc_caxecraft_app_TerrainRenderCounters hxc_l_tmp_instance_call_result_n992 = hxc_caxecraft_app_TerrainRenderer_draw(hxc_l_tmp_owned_class_field_address_n982, hxc_l_tmp_instance_call_result_n984, hxc_l_tmp_length_n985, hxc_l_tmp_load_result_n986, hxc_l_tmp_load_result_n987, hxc_l_tmp_load_result_n988, hxc_l_tmp_load_result_n989, hxc_l_tmp_record_field_load_result_n990, hxc_l_renderPosition.hxc_z);
        struct hxc_caxecraft_app_TerrainRenderCounters hxc_l_renderCounters = hxc_l_tmp_instance_call_result_n992;
        struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n993 = hxc_l_session;
        hxc_l_gc_roots[86] = (const void *)hxc_l_tmp_load_result_n993;
        if (hxc_l_tmp_load_result_n993 == NULL)
        {
          abort();
        }
        size_t hxc_l_tmp_length_n995;
        const uint8_t *hxc_l_tmp_instance_call_result_n994 = hxc_caxecraft_domain_GameSession_worldView(hxc_l_tmp_load_result_n993, &hxc_l_tmp_length_n995);
        struct Texture hxc_l_tmp_load_result_n996 = hxc_l_terrainTexture;
        bool hxc_l_tmp_load_result_n997 = hxc_l_terrainTextureReady;
        struct hxc_caxecraft_app_WaterRenderCounters hxc_l_tmp_call_result_n999 = hxc_caxecraft_app_WaterRenderer_draw(hxc_l_tmp_instance_call_result_n994, hxc_l_tmp_length_n995, hxc_l_tmp_load_result_n996, hxc_l_tmp_load_result_n997, hxc_l_waterPresentationCell);
        struct hxc_caxecraft_app_WaterRenderCounters hxc_l_waterCounters = hxc_l_tmp_call_result_n999;
        int32_t hxc_l_tmp_record_field_load_result_n1000 = hxc_l_renderCounters.hxc_visible;
        int32_t hxc_l_totalVisible = hxc_i32_add_wrapping(hxc_l_tmp_record_field_load_result_n1000, hxc_l_waterCounters.hxc_visible);
        int32_t hxc_l_tmp_record_field_load_result_n1002 = hxc_l_renderCounters.hxc_drawCalls;
        int32_t hxc_l_totalDrawCalls = hxc_i32_add_wrapping(hxc_l_tmp_record_field_load_result_n1002, hxc_l_waterCounters.hxc_drawCalls);
        struct Camera3D hxc_l_tmp_load_result_n1004 = hxc_l_camera;
        struct Texture hxc_l_tmp_load_result_n1005 = hxc_l_entityTexture;
        bool hxc_l_tmp_load_result_n1006 = hxc_l_entityTextureReady;
        struct hxc_caxecraft_domain_Character hxc_l_tmp_load_result_n1007 = hxc_l_dialogueActor;
        struct hxc_caxecraft_domain_Character hxc_l_tmp_load_result_n1008 = hxc_l_enemyActor;
        enum hxc_caxecraft_domain_ActorControllerPhase hxc_l_tmp_record_field_load_result_n1009 = hxc_l_enemyPhase.hxc_phase;
        hxc_caxecraft_app_CaxecraftApp_drawActors(hxc_l_tmp_load_result_n1004, hxc_l_tmp_load_result_n1005, hxc_l_tmp_load_result_n1006, hxc_l_tmp_load_result_n1007, hxc_l_tmp_load_result_n1008, hxc_l_tmp_record_field_load_result_n1009, hxc_l_berryDrop);
        struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_tmp_load_result_n1011 = hxc_l_contentRegistry;
        hxc_l_gc_roots[87] = (const void *)hxc_l_tmp_load_result_n1011;
        struct Camera3D hxc_l_tmp_load_result_n1012 = hxc_l_camera;
        struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1013 = hxc_l_session;
        hxc_l_gc_roots[88] = (const void *)hxc_l_tmp_load_result_n1013;
        if (hxc_l_tmp_load_result_n1013 == NULL)
        {
          abort();
        }
        size_t hxc_l_tmp_length_n1015;
        const int32_t *hxc_l_tmp_instance_call_result_n1014 = hxc_caxecraft_domain_GameSession_authoredItemsView(hxc_l_tmp_load_result_n1013, &hxc_l_tmp_length_n1015);
        struct hxc_array_ref *hxc_l_tmp_load_result_n1016 = hxc_l_loadedItems;
        struct Texture hxc_l_tmp_load_result_n1017 = hxc_l_itemTexture;
        bool hxc_l_tmp_load_result_n1018 = hxc_l_itemTextureReady;
        struct Texture hxc_l_tmp_load_result_n1019 = hxc_l_adventureItemTexture;
        hxc_caxecraft_app_AuthoredItemRenderer_drawWorldItems(hxc_l_tmp_load_result_n1011, hxc_l_tmp_load_result_n1012, hxc_l_tmp_instance_call_result_n1014, hxc_l_tmp_length_n1015, hxc_l_tmp_load_result_n1016, hxc_l_tmp_load_result_n1017, hxc_l_tmp_load_result_n1018, hxc_l_tmp_load_result_n1019, hxc_l_adventureItemTextureReady);
        if (hxc_l_hit.hxc_hit)
        {
          double hxc_l_y = (double)hxc_l_hit.hxc_cellY + 0.5;
          double hxc_l_z = (double)hxc_l_hit.hxc_cellZ + 0.5;
          int32_t hxc_l_tmp_record_field_load_result_n1024 = hxc_l_hit.hxc_cellX;
          double hxc_l_tmp_load_result_n1025 = hxc_l_y;
          DrawCubeWires((struct Vector3){ .x = (float)((double)hxc_l_tmp_record_field_load_result_n1024 + 0.5), .y = (float)hxc_l_tmp_load_result_n1025, .z = (float)hxc_l_z }, (float)1.04, (float)1.04, (float)1.04, (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 });
        }
        EndMode3D();
        if (hxc_l_cameraWaterBlend > 0.0)
        {
          int32_t hxc_l_overlayAlpha = hxc_f64_to_i32_saturating(105.0 * hxc_l_cameraWaterBlend);
          int32_t hxc_l_tmp_native_call_result_n1031 = GetScreenWidth();
          int32_t hxc_l_width_h7e67361171d6 = (int32_t)hxc_l_tmp_native_call_result_n1031;
          int32_t hxc_l_tmp_native_call_result_n1032 = GetScreenHeight();
          int32_t hxc_l_height_h7fac7e0b13e5 = (int32_t)hxc_l_tmp_native_call_result_n1032;
          uint8_t hxc_l_red = (uint8_t)31;
          uint8_t hxc_l_green = (uint8_t)115;
          uint8_t hxc_l_blue = (uint8_t)154;
          int32_t hxc_l_tmp_load_result_n1033 = hxc_l_overlayAlpha;
          int32_t hxc_l_tmp_conditional_result_n324 = 0;
          if (hxc_l_tmp_load_result_n1033 < 0)
          {
            hxc_l_tmp_conditional_result_n324 = 0;
          }
          else
          {
            int32_t hxc_l_tmp_load_result_n1034 = hxc_l_overlayAlpha;
            int32_t hxc_l_tmp_conditional_result_n325 = 0;
            if (hxc_l_tmp_load_result_n1034 > 255)
            {
              hxc_l_tmp_conditional_result_n325 = 255;
            }
            else
            {
              hxc_l_tmp_conditional_result_n325 = hxc_l_overlayAlpha;
            }
            hxc_l_tmp_conditional_result_n324 = hxc_l_tmp_conditional_result_n325;
          }
          uint8_t hxc_l_alpha = (uint8_t)hxc_l_tmp_conditional_result_n324;
          uint8_t hxc_l_tmp_load_result_n1038 = hxc_l_red;
          uint8_t hxc_l_tmp_load_result_n1039 = hxc_l_green;
          uint8_t hxc_l_tmp_load_result_n1040 = hxc_l_blue;
          struct Color hxc_l_this1_h66501733d5f3 = (struct Color){ .r = hxc_l_tmp_load_result_n1038, .g = hxc_l_tmp_load_result_n1039, .b = hxc_l_tmp_load_result_n1040, .a = hxc_l_alpha };
          int32_t hxc_l_tmp_load_result_n1043 = hxc_l_width_h7e67361171d6;
          int32_t hxc_l_tmp_load_result_n1044 = hxc_l_height_h7fac7e0b13e5;
          DrawRectangle((int32_t)0, (int32_t)0, (int32_t)hxc_l_tmp_load_result_n1043, (int32_t)hxc_l_tmp_load_result_n1044, hxc_l_this1_h66501733d5f3);
        }
        int32_t hxc_l_tmp_load_result_n1046 = hxc_l_totalVisible;
        int32_t hxc_l_tmp_load_result_n1047 = hxc_l_totalDrawCalls;
        int32_t hxc_l_tmp_load_result_n1048 = hxc_l_frameCount;
        int32_t hxc_l_tmp_load_result_n1049 = hxc_l_completedTicks;
        double hxc_l_tmp_record_field_load_result_n1051 = hxc_l_character.hxc_body.hxc_x;
        double hxc_l_tmp_record_field_load_result_n1052 = hxc_l_character.hxc_body.hxc_z;
        struct hxc_caxecraft_domain_VitalsState hxc_l_tmp_record_field_load_result_n1053 = hxc_l_character.hxc_vitals;
        int32_t hxc_l_tmp_load_result_n1054 = hxc_l_aquaticEquipmentCode;
        int32_t hxc_l_tmp_load_result_n1055 = hxc_l_aquaticEquipmentFrames;
        bool hxc_l_tmp_record_field_load_result_n1056 = hxc_l_character.hxc_aquatic.hxc_headSubmerged;
        int32_t hxc_l_tmp_record_field_load_result_n1057 = hxc_l_character.hxc_aquatic.hxc_breathTicks;
        int32_t hxc_l_tmp_record_field_load_result_n1058 = hxc_l_character.hxc_aquaticProfile.hxc_maximumBreathTicks;
        int32_t hxc_l_tmp_load_result_n1060 = hxc_l_placementBlockedFrames;
        int32_t hxc_l_tmp_load_result_n1061 = hxc_l_strikeHitFrames;
        int32_t hxc_l_tmp_load_result_n1062 = hxc_l_enemyDefeatedFrames;
        int32_t hxc_l_tmp_load_result_n1063 = hxc_l_enemyAttackFrames;
        int32_t hxc_l_tmp_load_result_n1064 = hxc_l_pickupFrames;
        int32_t hxc_l_tmp_load_result_n1065 = hxc_l_pickupAmount;
        int32_t hxc_l_tmp_load_result_n1066 = hxc_l_inventoryFullReason;
        int32_t hxc_l_tmp_load_result_n1067 = hxc_l_recoveryFeedback;
        int32_t hxc_l_tmp_load_result_n1068 = hxc_l_recoveryFeedbackFrames;
        bool hxc_l_tmp_load_result_n1070 = hxc_l_paused_hc53526590c8c;
        bool hxc_l_tmp_load_result_n1071 = hxc_l_captured_h086e4d2008a5;
        struct hxc_caxecraft_domain_RaycastHit hxc_l_tmp_load_result_n1072 = hxc_l_hit;
        int32_t hxc_l_tmp_load_result_n1073 = hxc_l_selectedMode;
        int32_t hxc_l_tmp_load_result_n1074 = hxc_l_locale;
        struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_load_result_n1075 = hxc_l_inventory;
        enum hxc_caxecraft_gameplay_GuidePhase hxc_l_tmp_load_result_n1076 = hxc_l_guidePhase;
        bool hxc_l_tmp_load_result_n1077 = hxc_l_guideInteractionAvailable;
        struct hxc_caxecraft_domain_Character hxc_l_tmp_load_result_n1078 = hxc_l_enemyActor;
        struct hxc_caxecraft_app_HudView hxc_l_hudView = (struct hxc_caxecraft_app_HudView){ .hxc_character = (struct hxc_caxecraft_app_HudCharacterView){ .hxc_aquaticEquipmentCode = hxc_l_tmp_load_result_n1054, .hxc_aquaticEquipmentVisible = hxc_l_tmp_load_result_n1055 > 0, .hxc_breathTicks = hxc_l_tmp_record_field_load_result_n1057, .hxc_headSubmerged = hxc_l_tmp_record_field_load_result_n1056, .hxc_maximumBreathTicks = hxc_l_tmp_record_field_load_result_n1058, .hxc_vitals = hxc_l_tmp_record_field_load_result_n1053, .hxc_x = hxc_l_tmp_record_field_load_result_n1051, .hxc_z = hxc_l_tmp_record_field_load_result_n1052 }, .hxc_enemy = hxc_l_tmp_load_result_n1078, .hxc_enemyPhase = hxc_l_enemyPhase.hxc_phase, .hxc_feedback = (struct hxc_caxecraft_app_HudFeedbackView){ .hxc_enemyAttacked = hxc_l_tmp_load_result_n1063 > 0, .hxc_enemyDefeated = hxc_l_tmp_load_result_n1062 > 0, .hxc_inventoryFullReason = hxc_l_tmp_load_result_n1066, .hxc_pickedUp = hxc_l_tmp_load_result_n1064 > 0, .hxc_pickupAmount = hxc_l_tmp_load_result_n1065, .hxc_placementBlocked = hxc_l_tmp_load_result_n1060 > 0, .hxc_recoveryDecision = hxc_l_tmp_load_result_n1067, .hxc_recoveryVisible = hxc_l_tmp_load_result_n1068 > 0, .hxc_strikeHit = hxc_l_tmp_load_result_n1061 > 0 }, .hxc_guideInteractionAvailable = hxc_l_tmp_load_result_n1077, .hxc_guidePhase = hxc_l_tmp_load_result_n1076, .hxc_hit = hxc_l_tmp_load_result_n1072, .hxc_inventory = hxc_l_tmp_load_result_n1075, .hxc_locale = hxc_l_tmp_load_result_n1074, .hxc_metrics = (struct hxc_caxecraft_app_HudMetricsView){ .hxc_completedTicks = hxc_l_tmp_load_result_n1049, .hxc_drawCalls = hxc_l_tmp_load_result_n1047, .hxc_renderedFrames = hxc_l_tmp_load_result_n1048, .hxc_visibleBlocks = hxc_l_tmp_load_result_n1046 }, .hxc_mode = hxc_l_tmp_load_result_n1073, .hxc_paused = hxc_l_tmp_load_result_n1070, .hxc_pointerCaptured = hxc_l_tmp_load_result_n1071 };
        struct hxc_caxecraft_app_HudView hxc_l_tmp_load_result_n1081 = hxc_l_hudView;
        struct hxc_caxecraft_app_HudResources hxc_l_tmp_load_result_n1082 = hxc_l_hudResources;
        struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_tmp_load_result_n1083 = hxc_l_contentRegistry;
        hxc_l_gc_roots[89] = (const void *)hxc_l_tmp_load_result_n1083;
        hxc_l_gc_roots[90] = (const void *)hxc_l_uiCatalog;
        hxc_caxecraft_app_CaxecraftApp_drawHud(hxc_l_tmp_load_result_n1081, hxc_l_tmp_load_result_n1082, hxc_l_tmp_load_result_n1083, hxc_l_uiCatalog);
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
  hxc_array_1f383c02_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n31);
  if (hxc_array_ref_release(hxc_l_fluidPresentation) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_record_2bd8b029_destroy(&hxc_l_tmp_record_field_transform_receiver_owner_n28);
  if (hxc_array_ref_release(hxc_l_g1_h619b6941f539) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_ref_release(hxc_l_loadedItems) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_ref_release(hxc_l_g1_hf09b4b04b8c7) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_ref_release(hxc_l_actorBindings) != HXC_STATUS_OK)
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
