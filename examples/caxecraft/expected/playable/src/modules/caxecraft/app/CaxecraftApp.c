#include "hxc/program.h"

void hxc_compiler_constructor_caxecraft_app_CaxecraftApp(struct hxc_caxecraft_app_CaxecraftApp *hxc_l_self)
{
  const void *hxc_l_gc_roots[3] = { (const void *)hxc_l_self, NULL, NULL };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 3, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_caxecraft_app_TerrainRenderer *hxc_l_tmp_owned_class_field_address_n0 = &(*hxc_l_self).hxc_terrainRenderer;
  hxc_compiler_constructor_caxecraft_app_TerrainRenderer(hxc_l_tmp_owned_class_field_address_n0);
  struct hxc_caxecraft_app_CaxecraftEditorScreen *hxc_l_tmp_managed_class_result_n1 = NULL;
  if (hxc_gc_allocate(&hxc_program_gc, &hxc_caxecraft_app_CaxecraftEditorScreen_descriptor, (void **)&hxc_l_tmp_managed_class_result_n1) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_gc_roots[1] = (const void *)hxc_l_tmp_managed_class_result_n1;
  hxc_compiler_constructor_caxecraft_app_CaxecraftEditorScreen(hxc_l_tmp_managed_class_result_n1);
  hxc_l_gc_roots[2] = (const void *)hxc_l_tmp_managed_class_result_n1;
  (*hxc_l_self).hxc_editorScreen = hxc_l_tmp_managed_class_result_n1;
  struct hxc_caxecraft_input_NavigationRepeater *hxc_l_tmp_owned_class_field_address_n2 = &(*hxc_l_self).hxc_editorNavigation;
  hxc_compiler_constructor_caxecraft_input_NavigationRepeater(hxc_l_tmp_owned_class_field_address_n2);
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
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

void hxc_caxecraft_app_CaxecraftApp_drawHud(struct hxc_caxecraft_app_HudView hxc_l_view, struct hxc_caxecraft_app_HudResources hxc_l_resources)
{
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
  struct Color hxc_l_this1_h7c2aaa0768a0 = (struct Color){ .r = 15, .g = 27, .b = 39, .a = 220 };
  DrawRectangle((int32_t)18, (int32_t)18, (int32_t)460, (int32_t)108, hxc_l_this1_h7c2aaa0768a0);
  struct Color hxc_l_this1_he50f396df5c2 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
  DrawRectangleLines((int32_t)18, (int32_t)18, (int32_t)460, (int32_t)108, hxc_l_this1_he50f396df5c2);
  const char *hxc_l_tmp_call_result_n84 = hxc_caxecraft_localization_UiCatalog_text(hxc_l_locale, 1);
  DrawText(hxc_l_tmp_call_result_n84, (int32_t)32, (int32_t)28, (int32_t)20, hxc_l_text);
  const char *hxc_l_tmp_call_result_n87 = hxc_caxecraft_localization_UiCatalog_text(hxc_l_locale, 4);
  DrawText(hxc_l_tmp_call_result_n87, (int32_t)32, (int32_t)58, (int32_t)14, hxc_l_text);
  hxc_caxecraft_app_HudDigits_drawNumber(16384, 82, 59, 5, (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 });
  const char *hxc_l_tmp_call_result_n91 = hxc_caxecraft_localization_UiCatalog_text(hxc_l_locale, 8);
  DrawText(hxc_l_tmp_call_result_n91, (int32_t)160, (int32_t)58, (int32_t)14, hxc_l_text);
  hxc_caxecraft_app_HudDigits_drawNumber(hxc_l_visible, 230, 59, 5, (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 });
  const char *hxc_l_tmp_call_result_n96 = hxc_caxecraft_localization_UiCatalog_text(hxc_l_locale, 5);
  DrawText(hxc_l_tmp_call_result_n96, (int32_t)326, (int32_t)58, (int32_t)14, hxc_l_text);
  hxc_caxecraft_app_HudDigits_drawNumber(hxc_l_drawCalls, 382, 59, 5, (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 });
  const char *hxc_l_tmp_call_result_n101 = hxc_caxecraft_localization_UiCatalog_text(hxc_l_locale, 6);
  DrawText(hxc_l_tmp_call_result_n101, (int32_t)32, (int32_t)86, (int32_t)12, hxc_l_text);
  int32_t hxc_l_tmp_load_result_n103 = hxc_l_frames;
  hxc_caxecraft_app_HudDigits_drawNumber(hxc_l_tmp_load_result_n103, 82, 85, 6, hxc_l_text);
  const char *hxc_l_tmp_call_result_n106 = hxc_caxecraft_localization_UiCatalog_text(hxc_l_locale, 7);
  DrawText(hxc_l_tmp_call_result_n106, (int32_t)174, (int32_t)86, (int32_t)12, hxc_l_text);
  int32_t hxc_l_tmp_load_result_n108 = hxc_l_updates;
  hxc_caxecraft_app_HudDigits_drawNumber(hxc_l_tmp_load_result_n108, 216, 85, 6, hxc_l_text);
  struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_load_result_n110 = hxc_l_inventory;
  struct Texture hxc_l_tmp_load_result_n111 = hxc_l_hudTexture;
  bool hxc_l_tmp_load_result_n112 = hxc_l_hudTextureReady;
  struct Texture hxc_l_tmp_load_result_n113 = hxc_l_itemTexture;
  bool hxc_l_tmp_load_result_n114 = hxc_l_itemTextureReady;
  int32_t hxc_l_tmp_load_result_n115 = hxc_l_width;
  hxc_caxecraft_app_CaxecraftApp_drawHotbar(hxc_l_tmp_load_result_n110, hxc_l_tmp_load_result_n111, hxc_l_tmp_load_result_n112, hxc_l_tmp_load_result_n113, hxc_l_tmp_load_result_n114, hxc_l_tmp_load_result_n115, hxc_l_height);
  struct hxc_caxecraft_domain_VitalsState hxc_l_tmp_load_result_n117 = hxc_l_vitals;
  struct Texture hxc_l_tmp_load_result_n118 = hxc_l_hudTexture;
  bool hxc_l_tmp_load_result_n119 = hxc_l_hudTextureReady;
  hxc_caxecraft_app_CaxecraftApp_drawHealth(hxc_l_tmp_load_result_n117, hxc_l_tmp_load_result_n118, hxc_l_tmp_load_result_n119, hxc_l_width);
  if (hxc_l_aquaticEquipmentCode >= 0)
  {
    int32_t hxc_l_tmp_load_result_n122 = hxc_l_aquaticEquipmentCode;
    struct Texture hxc_l_tmp_load_result_n123 = hxc_l_itemTexture;
    bool hxc_l_tmp_load_result_n124 = hxc_l_itemTextureReady;
    struct Texture hxc_l_tmp_load_result_n125 = hxc_l_adventureItemTexture;
    bool hxc_l_tmp_load_result_n126 = hxc_l_adventureItemTextureReady;
    hxc_caxecraft_app_AuthoredItemRenderer_drawEquippedIcon(hxc_l_tmp_load_result_n122, hxc_l_tmp_load_result_n123, hxc_l_tmp_load_result_n124, hxc_l_tmp_load_result_n125, hxc_l_tmp_load_result_n126, hxc_i32_subtract_wrapping(hxc_l_width, 226), 18, 42);
  }
  if (hxc_l_headSubmerged)
  {
    int32_t hxc_l_tmp_load_result_n129 = hxc_l_breathTicks;
    int32_t hxc_l_tmp_load_result_n130 = hxc_l_maximumBreathTicks;
    int32_t hxc_l_tmp_load_result_n131 = hxc_l_width;
    hxc_caxecraft_app_CaxecraftApp_drawBreath(hxc_l_tmp_load_result_n129, hxc_l_tmp_load_result_n130, hxc_l_tmp_load_result_n131, hxc_l_height);
  }
  const char *hxc_l_tmp_call_result_n134 = hxc_caxecraft_localization_UiCatalog_text(hxc_l_locale, 3);
  int32_t hxc_l_tmp_load_result_n135 = hxc_l_height;
  DrawText(hxc_l_tmp_call_result_n134, (int32_t)20, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n135, 22), (int32_t)14, hxc_l_text);
  if (hxc_l_mode == 1)
  {
    struct Color hxc_l_this1_hf7fbabd37910 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
    const char *hxc_l_tmp_call_result_n140 = hxc_caxecraft_localization_FirstPlayableCatalog_text(hxc_l_locale, 0);
    DrawText(hxc_l_tmp_call_result_n140, (int32_t)32, (int32_t)110, (int32_t)14, hxc_l_this1_hf7fbabd37910);
  }
  if (hxc_l_guideInteractionAvailable)
  {
    struct Color hxc_l_this1_h8198cb1a4cbe = (struct Color){ .r = 15, .g = 27, .b = 39, .a = 220 };
    int32_t hxc_l_tmp_load_result_n144 = hxc_l_centerX;
    int32_t hxc_l_tmp_load_result_n145 = hxc_l_centerY;
    DrawRectangle((int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n144, 260), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n145, 54), (int32_t)520, (int32_t)60, hxc_l_this1_h8198cb1a4cbe);
    if (hxc_l_guidePhase == hxc_caxecraft_gameplay_GuidePhase_Waiting)
    {
      const char *hxc_l_tmp_call_result_n150 = hxc_caxecraft_localization_FirstPlayableCatalog_text(hxc_l_locale, 13);
      int32_t hxc_l_tmp_load_result_n151 = hxc_l_centerX;
      int32_t hxc_l_tmp_load_result_n152 = hxc_l_centerY;
      DrawText(hxc_l_tmp_call_result_n150, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n151, 110), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n152, 74), (int32_t)18, hxc_l_text);
    }
    else
    {
      if (hxc_l_guidePhase == hxc_caxecraft_gameplay_GuidePhase_Welcomed)
      {
        const char *hxc_l_tmp_call_result_n157 = hxc_caxecraft_localization_FirstPlayableCatalog_text(hxc_l_locale, 14);
        int32_t hxc_l_tmp_load_result_n158 = hxc_l_centerX;
        int32_t hxc_l_tmp_load_result_n159 = hxc_l_centerY;
        DrawText(hxc_l_tmp_call_result_n157, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n158, 225), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n159, 74), (int32_t)16, hxc_l_text);
      }
      else
      {
        const char *hxc_l_tmp_call_result_n162 = hxc_caxecraft_localization_FirstPlayableCatalog_text(hxc_l_locale, 12);
        int32_t hxc_l_tmp_load_result_n163 = hxc_l_centerX;
        int32_t hxc_l_tmp_load_result_n164 = hxc_l_centerY;
        DrawText(hxc_l_tmp_call_result_n162, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n163, 205), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n164, 74), (int32_t)16, hxc_l_text);
      }
    }
  }
  if (hxc_l_enemy.hxc_vitals.hxc_health > 0)
  {
    if (hxc_l_enemyPhase == hxc_caxecraft_domain_ActorControllerPhase_Windup)
    {
      struct Color hxc_l_this1_h94855cb50317 = (struct Color){ .r = 218, .g = 65, .b = 72, .a = 255 };
      const char *hxc_l_tmp_call_result_n171 = hxc_caxecraft_localization_FirstPlayableCatalog_text(hxc_l_locale, 11);
      int32_t hxc_l_tmp_load_result_n172 = hxc_l_width;
      DrawText(hxc_l_tmp_call_result_n171, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n172, 300), (int32_t)28, (int32_t)16, hxc_l_this1_h94855cb50317);
    }
    else
    {
      if (hxc_l_enemyPhase == hxc_caxecraft_domain_ActorControllerPhase_Chasing)
      {
        struct Color hxc_l_this1_h1eedf8acdbc9 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
        const char *hxc_l_tmp_call_result_n178 = hxc_caxecraft_localization_FirstPlayableCatalog_text(hxc_l_locale, 9);
        int32_t hxc_l_tmp_load_result_n179 = hxc_l_width;
        DrawText(hxc_l_tmp_call_result_n178, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n179, 180), (int32_t)28, (int32_t)16, hxc_l_this1_h1eedf8acdbc9);
      }
    }
  }
  if (hxc_l_strikeHit)
  {
    struct Color hxc_l_this1_he708863e94f5 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
    const char *hxc_l_tmp_call_result_n184 = hxc_caxecraft_localization_FirstPlayableCatalog_text(hxc_l_locale, 7);
    int32_t hxc_l_tmp_load_result_n185 = hxc_l_centerX;
    int32_t hxc_l_tmp_load_result_n186 = hxc_l_centerY;
    DrawText(hxc_l_tmp_call_result_n184, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n185, 70), (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n186, 54), (int32_t)18, hxc_l_this1_he708863e94f5);
  }
  if (hxc_l_enemyDefeated)
  {
    struct Color hxc_l_this1_hde945947c4f2 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
    const char *hxc_l_tmp_call_result_n191 = hxc_caxecraft_localization_FirstPlayableCatalog_text(hxc_l_locale, 10);
    int32_t hxc_l_tmp_load_result_n192 = hxc_l_width;
    DrawText(hxc_l_tmp_call_result_n191, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n192, 285), (int32_t)54, (int32_t)16, hxc_l_this1_hde945947c4f2);
  }
  if (hxc_l_enemyAttacked)
  {
    struct Color hxc_l_this1_hc78e4f3725b1 = (struct Color){ .r = 218, .g = 65, .b = 72, .a = 255 };
    const char *hxc_l_tmp_call_result_n197 = hxc_caxecraft_localization_FirstPlayableCatalog_text(hxc_l_locale, 18);
    int32_t hxc_l_tmp_load_result_n198 = hxc_l_width;
    DrawText(hxc_l_tmp_call_result_n197, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n198, 330), (int32_t)82, (int32_t)16, hxc_l_this1_hc78e4f3725b1);
  }
  if (hxc_l_pickedUp)
  {
    int32_t hxc_l_tmp_load_result_n201 = hxc_l_pickupAmount;
    int32_t hxc_l_tmp_conditional_result_n52 = 0;
    if (hxc_l_tmp_load_result_n201 == 1)
    {
      hxc_l_tmp_conditional_result_n52 = 2;
    }
    else
    {
      hxc_l_tmp_conditional_result_n52 = 3;
    }
    int32_t hxc_l_pickupMessage = hxc_l_tmp_conditional_result_n52;
    struct Color hxc_l_this1_h0626ec518fa4 = (struct Color){ .r = 174, .g = 78, .b = 136, .a = 255 };
    int32_t hxc_l_tmp_load_result_n204 = hxc_l_locale;
    const char *hxc_l_tmp_call_result_n206 = hxc_caxecraft_localization_FirstPlayableCatalog_text(hxc_l_tmp_load_result_n204, hxc_l_pickupMessage);
    int32_t hxc_l_tmp_load_result_n207 = hxc_l_centerX;
    int32_t hxc_l_tmp_load_result_n208 = hxc_l_centerY;
    DrawText(hxc_l_tmp_call_result_n206, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n207, 48), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n208, 24), (int32_t)18, hxc_l_this1_h0626ec518fa4);
  }
  if (hxc_l_inventoryFullReason == 1)
  {
    struct Color hxc_l_this1_hf34305fbe6c3 = (struct Color){ .r = 238, .g = 113, .b = 78, .a = 255 };
    const char *hxc_l_tmp_call_result_n213 = hxc_caxecraft_localization_FirstPlayableCatalog_text(hxc_l_locale, 5);
    int32_t hxc_l_tmp_load_result_n214 = hxc_l_centerX;
    int32_t hxc_l_tmp_load_result_n215 = hxc_l_centerY;
    DrawText(hxc_l_tmp_call_result_n213, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n214, 150), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n215, 48), (int32_t)16, hxc_l_this1_hf34305fbe6c3);
  }
  else
  {
    if (hxc_l_inventoryFullReason == 2)
    {
      struct Color hxc_l_this1_h1d26745147c2 = (struct Color){ .r = 238, .g = 113, .b = 78, .a = 255 };
      const char *hxc_l_tmp_call_result_n220 = hxc_caxecraft_localization_FirstPlayableCatalog_text(hxc_l_locale, 6);
      int32_t hxc_l_tmp_load_result_n221 = hxc_l_centerX;
      int32_t hxc_l_tmp_load_result_n222 = hxc_l_centerY;
      DrawText(hxc_l_tmp_call_result_n220, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n221, 155), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n222, 48), (int32_t)16, hxc_l_this1_h1d26745147c2);
    }
  }
  if (hxc_l_recoveryVisible)
  {
    if (hxc_l_recoveryFeedback == 1)
    {
      struct Color hxc_l_this1_h635af7fe7a48 = (struct Color){ .r = 94, .g = 212, .b = 136, .a = 255 };
      const char *hxc_l_tmp_call_result_n228 = hxc_caxecraft_localization_FirstPlayableCatalog_text(hxc_l_locale, 4);
      int32_t hxc_l_tmp_load_result_n229 = hxc_l_centerX;
      int32_t hxc_l_tmp_load_result_n230 = hxc_l_centerY;
      DrawText(hxc_l_tmp_call_result_n228, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n229, 88), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n230, 24), (int32_t)18, hxc_l_this1_h635af7fe7a48);
    }
    else
    {
      if (hxc_l_recoveryFeedback == 2)
      {
        struct Color hxc_l_this1_h79a1f7c6738d = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
        const char *hxc_l_tmp_call_result_n235 = hxc_caxecraft_localization_UiCatalog_text(hxc_l_locale, 25);
        int32_t hxc_l_tmp_load_result_n236 = hxc_l_centerX;
        int32_t hxc_l_tmp_load_result_n237 = hxc_l_centerY;
        DrawText(hxc_l_tmp_call_result_n235, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n236, 96), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n237, 24), (int32_t)18, hxc_l_this1_h79a1f7c6738d);
      }
      else
      {
        if (hxc_l_recoveryFeedback == 3)
        {
          struct Color hxc_l_this1_h36ad8ed6c864 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
          const char *hxc_l_tmp_call_result_n242 = hxc_caxecraft_localization_FirstPlayableCatalog_text(hxc_l_locale, 15);
          int32_t hxc_l_tmp_load_result_n243 = hxc_l_centerX;
          int32_t hxc_l_tmp_load_result_n244 = hxc_l_centerY;
          DrawText(hxc_l_tmp_call_result_n242, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n243, 76), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n244, 24), (int32_t)18, hxc_l_this1_h36ad8ed6c864);
        }
      }
    }
  }
  if (hxc_l_aquaticEquipmentVisible)
  {
    struct Color hxc_l_this1_h94233c4871af = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
    const char *hxc_l_tmp_call_result_n249 = hxc_caxecraft_localization_UiCatalog_text(hxc_l_locale, 0);
    int32_t hxc_l_tmp_load_result_n250 = hxc_l_centerX;
    int32_t hxc_l_tmp_load_result_n251 = hxc_l_centerY;
    DrawText(hxc_l_tmp_call_result_n249, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n250, 128), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n251, 24), (int32_t)18, hxc_l_this1_h94233c4871af);
  }
  if (hxc_l_vitals.hxc_safeTicks > 15)
  {
    struct Color hxc_l_this1_h186ae0cc34a2 = (struct Color){ .r = 218, .g = 65, .b = 72, .a = 255 };
    int32_t hxc_l_tmp_load_result_n255 = hxc_l_width;
    int32_t hxc_l_tmp_load_result_n256 = hxc_l_height;
    DrawRectangleLines((int32_t)4, (int32_t)4, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n255, 8), (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n256, 8), hxc_l_this1_h186ae0cc34a2);
  }
  if (hxc_l_vitals.hxc_health <= 0)
  {
    struct Color hxc_l_this1_hcaca8b0e6e05 = (struct Color){ .r = 15, .g = 27, .b = 39, .a = 220 };
    int32_t hxc_l_tmp_load_result_n260 = hxc_l_centerX;
    int32_t hxc_l_tmp_load_result_n261 = hxc_l_centerY;
    DrawRectangle((int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n260, 250), (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n261, 74), (int32_t)500, (int32_t)148, hxc_l_this1_hcaca8b0e6e05);
    struct Color hxc_l_this1_h36534efb0e0f = (struct Color){ .r = 218, .g = 65, .b = 72, .a = 255 };
    int32_t hxc_l_tmp_load_result_n264 = hxc_l_centerX;
    int32_t hxc_l_tmp_load_result_n265 = hxc_l_centerY;
    DrawRectangleLines((int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n264, 250), (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n265, 74), (int32_t)500, (int32_t)148, hxc_l_this1_h36534efb0e0f);
    const char *hxc_l_tmp_call_result_n268 = hxc_caxecraft_localization_FirstPlayableCatalog_text(hxc_l_locale, 8);
    int32_t hxc_l_tmp_load_result_n269 = hxc_l_centerX;
    int32_t hxc_l_tmp_load_result_n270 = hxc_l_centerY;
    DrawText(hxc_l_tmp_call_result_n268, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n269, 122), (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n270, 42), (int32_t)24, hxc_l_text);
    struct Color hxc_l_this1_h26c8d1c19f69 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
    const char *hxc_l_tmp_call_result_n274 = hxc_caxecraft_localization_FirstPlayableCatalog_text(hxc_l_locale, 16);
    int32_t hxc_l_tmp_load_result_n275 = hxc_l_centerX;
    int32_t hxc_l_tmp_load_result_n276 = hxc_l_centerY;
    DrawText(hxc_l_tmp_call_result_n274, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n275, 125), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n276, 10), (int32_t)18, hxc_l_this1_h26c8d1c19f69);
  }
  if (hxc_l_paused)
  {
    struct Color hxc_l_this1_hbcdf386ccbe0 = (struct Color){ .r = 15, .g = 27, .b = 39, .a = 220 };
    int32_t hxc_l_tmp_load_result_n280 = hxc_l_centerX;
    int32_t hxc_l_tmp_load_result_n281 = hxc_l_centerY;
    DrawRectangle((int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n280, 170), (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n281, 48), (int32_t)340, (int32_t)96, hxc_l_this1_hbcdf386ccbe0);
    struct Color hxc_l_this1_haee660760ca3 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
    int32_t hxc_l_tmp_load_result_n284 = hxc_l_centerX;
    int32_t hxc_l_tmp_load_result_n285 = hxc_l_centerY;
    DrawRectangleLines((int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n284, 170), (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n285, 48), (int32_t)340, (int32_t)96, hxc_l_this1_haee660760ca3);
    const char *hxc_l_tmp_call_result_n288 = hxc_caxecraft_localization_UiCatalog_text(hxc_l_locale, 32);
    int32_t hxc_l_tmp_load_result_n289 = hxc_l_centerX;
    int32_t hxc_l_tmp_load_result_n290 = hxc_l_centerY;
    DrawText(hxc_l_tmp_call_result_n288, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n289, 48), (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n290, 30), (int32_t)24, hxc_l_text);
    const char *hxc_l_tmp_call_result_n293 = hxc_caxecraft_localization_UiCatalog_text(hxc_l_locale, 31);
    int32_t hxc_l_tmp_load_result_n294 = hxc_l_centerX;
    int32_t hxc_l_tmp_load_result_n295 = hxc_l_centerY;
    DrawText(hxc_l_tmp_call_result_n293, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n294, 160), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n295, 8), (int32_t)16, hxc_l_text);
  }
  else
  {
    if (hxc_l_placementBlocked)
    {
      struct Color hxc_l_this1_h95bb33c58a18 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
      const char *hxc_l_tmp_call_result_n300 = hxc_caxecraft_localization_UiCatalog_text(hxc_l_locale, 33);
      int32_t hxc_l_tmp_load_result_n301 = hxc_l_centerX;
      int32_t hxc_l_tmp_load_result_n302 = hxc_l_centerY;
      DrawText(hxc_l_tmp_call_result_n300, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n301, 170), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n302, 26), (int32_t)14, hxc_l_this1_h95bb33c58a18);
    }
    else
    {
      if (!hxc_l_captured)
      {
        const char *hxc_l_tmp_call_result_n306 = hxc_caxecraft_localization_UiCatalog_text(hxc_l_locale, 2);
        int32_t hxc_l_tmp_load_result_n307 = hxc_l_centerX;
        int32_t hxc_l_tmp_load_result_n308 = hxc_l_centerY;
        DrawText(hxc_l_tmp_call_result_n306, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n307, 90), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n308, 26), (int32_t)14, hxc_l_text);
      }
      else
      {
        if (!hxc_l_hit.hxc_hit)
        {
          const char *hxc_l_tmp_call_result_n312 = hxc_caxecraft_localization_UiCatalog_text(hxc_l_locale, 30);
          int32_t hxc_l_tmp_load_result_n313 = hxc_l_centerX;
          int32_t hxc_l_tmp_load_result_n314 = hxc_l_centerY;
          DrawText(hxc_l_tmp_call_result_n312, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n313, 105), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n314, 26), (int32_t)14, hxc_l_text);
        }
      }
    }
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
  const void *hxc_l_gc_roots[71] = { (const void *)hxc_l_self, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 71, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_actorBindings = { 0 };
  struct hxc_array_ref *hxc_l_g1_h881bba9116a1 = { 0 };
  struct hxc_caxecraft_content_LoadedActorBinding hxc_l_binding_hfac69da206c9 = { 0 };
  struct hxc_caxecraft_content_LoadedActorBinding hxc_l_tmp_array_element_owner_n16 = { 0 };
  struct hxc_caxecraft_app_CaxecraftApp_PlayableActorBinding hxc_l_tmp_array_push_element_owner_n18 = { 0 };
  struct hxc_array_ref *hxc_l_loadedItems = { 0 };
  struct hxc_array_ref *hxc_l_g1_h0fb221650f0c = { 0 };
  struct hxc_caxecraft_domain_ActorControllerEvent hxc_l_event = { 0 };
  struct hxc_array_ref *hxc_l_g1_hcad3014c33f5 = { 0 };
  hxc_string hxc_l_drop = { 0 };
  struct hxc_array_ref *hxc_l_actorPhases = { 0 };
  struct hxc_caxecraft_content_ResolvedItem hxc_l_binding_h1fed9b751f82 = { 0 };
  struct hxc_caxecraft_content_ResolvedItem hxc_l_tmp_array_element_owner_n23 = { 0 };
  struct hxc_array_ref *hxc_l_presentationActorPhases = { 0 };
  struct hxc_caxecraft_content_ResolvedPlayer hxc_l_tmp_record_field_transform_receiver_owner_n25 = { 0 };
  struct hxc_array_ref *hxc_l_fluidPresentation = { 0 };
  struct hxc_caxecraft_content_ResolvedFluidPresentation hxc_l_tmp_array_element_owner_n28 = { 0 };
  struct hxc_caxecraft_content_ResolvedFluidPresentation hxc_l_request = { 0 };
  struct hxc_caxecraft_content_ResolvedFluidPresentation hxc_l_tmp_array_element_owner_n31 = { 0 };
  struct hxc_caxecraft_content_RuntimeLevelReceipt hxc_l_receipt = { 0 };
  hxc_string hxc_l_tmp_sys_println_string_argument_owner_n33 = { 0 };
  hxc_string hxc_l_tmp_string_concat_right_owner_n34 = { 0 };
  hxc_string hxc_l_tmp_sys_println_string_argument_owner_n35 = { 0 };
  hxc_string hxc_l_tmp_string_concat_right_owner_n36 = { 0 };
  hxc_string hxc_l_tmp_sys_println_string_argument_owner_n37 = { 0 };
  struct hxc_caxecraft_content_RuntimeLevelLoadResult hxc_l_g_h4ef171f4fe67 = { 0 };
  struct hxc_array_ref *hxc_l_initialActorPhases = { 0 };
  struct hxc_caxecraft_content_RuntimeLevelSource hxc_l_tmp_static_call_argument_0_owner_n8 = { 0 };
  int32_t hxc_l_initialHealth = 6;
  hxc_l_gc_roots[1] = (const void *)NULL;
  struct hxc_caxecraft_content_ContentPackageStore *hxc_l_contentStore = NULL;
  struct hxc_caxecraft_content_ContentPackageOpenResult hxc_l_tmp_call_result_n0 = hxc_caxecraft_content_ContentPackageStore_open((hxc_string){ (const uint8_t *)"content", 7, true, NULL }, (hxc_string){ (const uint8_t *)"staged-content", 14, true, NULL }, 16777216);
  hxc_l_gc_roots[2] = hxc_l_tmp_call_result_n0.hxc_tag == hxc_caxecraft_content_ContentPackageOpenResult_PackageStoreOpened ? (const void *)hxc_l_tmp_call_result_n0.hxc_payload.hxc_PackageStoreOpened.hxc_store : NULL;
  struct hxc_caxecraft_content_ContentPackageOpenResult hxc_l_g_h0b6eadae1b17 = hxc_l_tmp_call_result_n0;
  hxc_l_gc_roots[3] = hxc_l_g_h0b6eadae1b17.hxc_tag == hxc_caxecraft_content_ContentPackageOpenResult_PackageStoreOpened ? (const void *)hxc_l_g_h0b6eadae1b17.hxc_payload.hxc_PackageStoreOpened.hxc_store : NULL;
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
  hxc_l_gc_roots[4] = hxc_l_g_h0b6eadae1b17.hxc_tag == hxc_caxecraft_content_ContentPackageOpenResult_PackageStoreOpened ? (const void *)hxc_l_g_h0b6eadae1b17.hxc_payload.hxc_PackageStoreOpened.hxc_store : NULL;
  if (hxc_l_g_h0b6eadae1b17.hxc_tag != hxc_caxecraft_content_ContentPackageOpenResult_PackageStoreOpened)
  {
    abort();
  }
  struct hxc_caxecraft_content_ContentPackageStore *hxc_l_tmp_enum_payload_project_n3 = hxc_l_g_h0b6eadae1b17.hxc_payload.hxc_PackageStoreOpened.hxc_store;
  hxc_l_gc_roots[5] = (const void *)hxc_l_tmp_enum_payload_project_n3;
  struct hxc_caxecraft_content_ContentPackageStore *hxc_l_store = hxc_l_tmp_enum_payload_project_n3;
  hxc_l_gc_roots[6] = (const void *)hxc_l_store;
  hxc_l_contentStore = hxc_l_store;
  hxc_l_gc_roots[7] = (const void *)NULL;
  struct hxc_caxecraft_content_RuntimeLevelCandidate *hxc_l_loadedCandidate = NULL;
  struct hxc_caxecraft_content_ContentPackageStore *hxc_l_tmp_load_result_n5 = hxc_l_contentStore;
  hxc_l_gc_roots[8] = (const void *)hxc_l_tmp_load_result_n5;
  hxc_string hxc_l_tmp_enum_payload_1_owner_n7 = (hxc_string){ (const uint8_t *)"scenarios/first-playable/map.caxemap", 36, true, NULL };
  if (hxc_string_retain(hxc_l_tmp_enum_payload_1_owner_n7) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_gc_roots[9] = (struct hxc_caxecraft_content_RuntimeLevelSource){ .hxc_tag = hxc_caxecraft_content_RuntimeLevelSource_NativePackageFile, .hxc_payload.hxc_NativePackageFile.hxc_store = hxc_l_tmp_load_result_n5, .hxc_payload.hxc_NativePackageFile.hxc_logicalPath = hxc_l_tmp_enum_payload_1_owner_n7 }.hxc_tag == hxc_caxecraft_content_RuntimeLevelSource_NativePackageFile ? (const void *)(struct hxc_caxecraft_content_RuntimeLevelSource){ .hxc_tag = hxc_caxecraft_content_RuntimeLevelSource_NativePackageFile, .hxc_payload.hxc_NativePackageFile.hxc_store = hxc_l_tmp_load_result_n5, .hxc_payload.hxc_NativePackageFile.hxc_logicalPath = hxc_l_tmp_enum_payload_1_owner_n7 }.hxc_payload.hxc_NativePackageFile.hxc_store : NULL;
  hxc_l_tmp_static_call_argument_0_owner_n8 = (struct hxc_caxecraft_content_RuntimeLevelSource){ .hxc_tag = hxc_caxecraft_content_RuntimeLevelSource_NativePackageFile, .hxc_payload.hxc_NativePackageFile.hxc_store = hxc_l_tmp_load_result_n5, .hxc_payload.hxc_NativePackageFile.hxc_logicalPath = hxc_l_tmp_enum_payload_1_owner_n7 };
  struct hxc_caxecraft_content_RuntimeLevelSource hxc_l_tmp_static_call_argument_0_borrow_result_n8 = hxc_l_tmp_static_call_argument_0_owner_n8;
  hxc_l_gc_roots[10] = hxc_l_tmp_static_call_argument_0_borrow_result_n8.hxc_tag == hxc_caxecraft_content_RuntimeLevelSource_NativePackageFile ? (const void *)hxc_l_tmp_static_call_argument_0_borrow_result_n8.hxc_payload.hxc_NativePackageFile.hxc_store : NULL;
  struct hxc_caxecraft_content_BaseContentRegistry *hxc_l_tmp_managed_class_result_n9 = NULL;
  if (hxc_gc_allocate(&hxc_program_gc, &hxc_caxecraft_content_BaseContentRegistry_descriptor, (void **)&hxc_l_tmp_managed_class_result_n9) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_gc_roots[11] = (const void *)hxc_l_tmp_managed_class_result_n9;
  hxc_l_gc_roots[12] = (const void *)hxc_l_tmp_managed_class_result_n9;
  int32_t hxc_l_tmp_load_result_n10 = hxc_l_initialHealth;
  struct hxc_caxecraft_domain_AquaticProfile hxc_l_tmp_call_result_n11 = hxc_caxecraft_content_BaseContentPack_aquaticProfile(0);
  struct hxc_caxecraft_content_RuntimeLevelLoadResult hxc_l_tmp_call_result_n13 = hxc_caxecraft_content_RuntimeLevelLoader_loadRuntimeLevel(hxc_l_tmp_static_call_argument_0_borrow_result_n8, 1, hxc_l_tmp_managed_class_result_n9, (struct hxc_caxecraft_content_LevelPlayerOptions){ .hxc_aquaticProfile = hxc_l_tmp_call_result_n11, .hxc_entityId = 1, .hxc_initialHealth = hxc_l_tmp_load_result_n10 });
  hxc_l_gc_roots[13] = hxc_l_tmp_call_result_n13.hxc_tag == hxc_caxecraft_content_RuntimeLevelLoadResult_RuntimeLevelReady ? (const void *)hxc_l_tmp_call_result_n13.hxc_payload.hxc_RuntimeLevelReady.hxc_candidate : NULL;
  hxc_l_g_h4ef171f4fe67 = hxc_l_tmp_call_result_n13;
  hxc_l_gc_roots[14] = hxc_l_g_h4ef171f4fe67.hxc_tag == hxc_caxecraft_content_RuntimeLevelLoadResult_RuntimeLevelReady ? (const void *)hxc_l_g_h4ef171f4fe67.hxc_payload.hxc_RuntimeLevelReady.hxc_candidate : NULL;
  switch (hxc_l_g_h4ef171f4fe67.hxc_tag) {
    case hxc_caxecraft_content_RuntimeLevelLoadResult_RuntimeLevelReady:
      {
        break;
      }
    case hxc_caxecraft_content_RuntimeLevelLoadResult_RuntimeLevelRejected:
      {
        if (hxc_io_println((hxc_string){ (const uint8_t *)"caxecraft: runtime level rejected", 33, true, NULL }) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_enum_be0a6f10_destroy(&hxc_l_g_h4ef171f4fe67);
        hxc_enum_21bc9bf3_destroy(&hxc_l_tmp_static_call_argument_0_owner_n8);
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
  hxc_l_gc_roots[15] = hxc_l_g_h4ef171f4fe67.hxc_tag == hxc_caxecraft_content_RuntimeLevelLoadResult_RuntimeLevelReady ? (const void *)hxc_l_g_h4ef171f4fe67.hxc_payload.hxc_RuntimeLevelReady.hxc_candidate : NULL;
  if (hxc_l_g_h4ef171f4fe67.hxc_tag != hxc_caxecraft_content_RuntimeLevelLoadResult_RuntimeLevelReady)
  {
    abort();
  }
  struct hxc_caxecraft_content_RuntimeLevelCandidate *hxc_l_tmp_enum_payload_project_n16 = hxc_l_g_h4ef171f4fe67.hxc_payload.hxc_RuntimeLevelReady.hxc_candidate;
  hxc_l_gc_roots[16] = (const void *)hxc_l_tmp_enum_payload_project_n16;
  struct hxc_caxecraft_content_RuntimeLevelCandidate *hxc_l_candidate = hxc_l_tmp_enum_payload_project_n16;
  hxc_l_gc_roots[17] = (const void *)hxc_l_candidate;
  hxc_l_loadedCandidate = hxc_l_candidate;
  struct hxc_caxecraft_content_RuntimeLevelCandidate *hxc_l_tmp_load_result_n18 = hxc_l_loadedCandidate;
  hxc_l_gc_roots[18] = (const void *)hxc_l_tmp_load_result_n18;
  if (hxc_l_tmp_load_result_n18 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_content_LoadedContentGeneration *hxc_l_tmp_class_field_load_result_n19 = (*hxc_l_tmp_load_result_n18).hxc_loadedGeneration;
  hxc_l_gc_roots[19] = (const void *)hxc_l_tmp_class_field_load_result_n19;
  struct hxc_caxecraft_content_ActiveContent *hxc_l_tmp_managed_class_result_n20 = NULL;
  if (hxc_gc_allocate(&hxc_program_gc, &hxc_caxecraft_content_ActiveContent_descriptor, (void **)&hxc_l_tmp_managed_class_result_n20) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_gc_roots[20] = (const void *)hxc_l_tmp_managed_class_result_n20;
  hxc_compiler_constructor_caxecraft_content_ActiveContent(hxc_l_tmp_managed_class_result_n20, hxc_l_tmp_class_field_load_result_n19);
  hxc_l_gc_roots[21] = (const void *)hxc_l_tmp_managed_class_result_n20;
  struct hxc_caxecraft_content_ActiveContent *hxc_l_activeContent = hxc_l_tmp_managed_class_result_n20;
  struct hxc_caxecraft_content_ActiveContent *hxc_l_tmp_load_result_n21 = hxc_l_activeContent;
  hxc_l_gc_roots[22] = (const void *)hxc_l_tmp_load_result_n21;
  if (hxc_l_tmp_load_result_n21 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_content_LoadedContentGeneration *hxc_l_tmp_class_field_load_result_n22 = (*hxc_l_tmp_load_result_n21).hxc_active;
  hxc_l_gc_roots[23] = (const void *)hxc_l_tmp_class_field_load_result_n22;
  if (hxc_l_tmp_class_field_load_result_n22 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_class_field_load_result_n23 = (*hxc_l_tmp_class_field_load_result_n22).hxc_ownedSession;
  hxc_l_gc_roots[24] = (const void *)hxc_l_tmp_class_field_load_result_n23;
  struct hxc_caxecraft_domain_GameSession *hxc_l_session = hxc_l_tmp_class_field_load_result_n23;
  struct hxc_array_ref *hxc_l_tmp_array_create_result_n24 = NULL;
  if (hxc_array_ref_create(hxc_default_allocator(), (hxc_array_element_ops){ sizeof(struct hxc_caxecraft_app_CaxecraftApp_PlayableActorBinding), _Alignof(struct hxc_caxecraft_app_CaxecraftApp_PlayableActorBinding), NULL, hxc_array_346eae85_element_copy, hxc_array_346eae85_element_assign, hxc_array_346eae85_element_destroy }, &hxc_l_tmp_array_create_result_n24) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_actorBindings = hxc_l_tmp_array_create_result_n24;
  int32_t hxc_l_g_haed0f54e2620 = 0;
  struct hxc_caxecraft_content_RuntimeLevelCandidate *hxc_l_tmp_load_result_n25 = hxc_l_loadedCandidate;
  hxc_l_gc_roots[25] = (const void *)hxc_l_tmp_load_result_n25;
  if (hxc_l_tmp_load_result_n25 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_content_LoadedContentGeneration *hxc_l_tmp_class_field_load_result_n26 = (*hxc_l_tmp_load_result_n25).hxc_loadedGeneration;
  hxc_l_gc_roots[26] = (const void *)hxc_l_tmp_class_field_load_result_n26;
  if (hxc_l_tmp_class_field_load_result_n26 == NULL)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_tmp_instance_call_result_n27 = hxc_caxecraft_content_LoadedContentGeneration_actorBindings(hxc_l_tmp_class_field_load_result_n26);
  hxc_l_g1_h881bba9116a1 = hxc_l_tmp_instance_call_result_n27;
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n28 = hxc_l_g_haed0f54e2620;
    int32_t hxc_l_tmp_array_length_result_n30;
    if (hxc_array_ref_length(hxc_l_g1_h881bba9116a1, &hxc_l_tmp_array_length_result_n30) != HXC_STATUS_OK)
    {
      abort();
    }
    if (!(hxc_l_tmp_load_result_n28 < hxc_l_tmp_array_length_result_n30))
    {
      break;
    }
    struct hxc_array_ref *hxc_l_tmp_load_result_n31 = hxc_l_g1_h881bba9116a1;
    struct hxc_caxecraft_content_LoadedActorBinding hxc_l_tmp_array_get_result_n33;
    if (hxc_array_ref_get_copy(hxc_l_tmp_load_result_n31, (size_t)hxc_l_g_haed0f54e2620, &hxc_l_tmp_array_get_result_n33) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_array_element_owner_n16 = hxc_l_tmp_array_get_result_n33;
    hxc_l_binding_hfac69da206c9 = hxc_l_tmp_array_element_owner_n16;
    if (hxc_record_ad1068b2_retain(&hxc_l_binding_hfac69da206c9) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_g_haed0f54e2620 = hxc_i32_add_wrapping(hxc_l_g_haed0f54e2620, 1);
    struct hxc_array_ref *hxc_l_tmp_load_result_n36 = hxc_l_actorBindings;
    int32_t hxc_l_tmp_record_field_load_result_n37 = hxc_l_binding_hfac69da206c9.hxc_entityId;
    struct hxc_caxecraft_content_CharacterSpawnRole hxc_l_tmp_record_field_role_owner_n17 = hxc_l_binding_hfac69da206c9.hxc_role;
    if (hxc_enum_b133bfa7_retain(&hxc_l_tmp_record_field_role_owner_n17) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_array_push_element_owner_n18 = (struct hxc_caxecraft_app_CaxecraftApp_PlayableActorBinding){ .hxc_entityId = hxc_l_tmp_record_field_load_result_n37, .hxc_role = hxc_l_tmp_record_field_role_owner_n17 };
    int32_t hxc_l_tmp_array_push_result_n42;
    if (hxc_array_ref_push_copy(hxc_l_tmp_load_result_n36, &hxc_l_tmp_array_push_element_owner_n18, &hxc_l_tmp_array_push_result_n42) != HXC_STATUS_OK)
    {
      abort();
    }
    (void)hxc_l_tmp_array_push_result_n42;
    hxc_record_10bdee1f_destroy(&hxc_l_tmp_array_push_element_owner_n18);
    hxc_record_ad1068b2_destroy(&hxc_l_binding_hfac69da206c9);
    hxc_array_aa4dc4ac_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n16);
  }
  struct hxc_array_ref *hxc_l_tmp_array_create_result_n43 = NULL;
  if (hxc_array_ref_create_trivial(hxc_default_allocator(), sizeof(struct hxc_caxecraft_app_LoadedWorldItem), _Alignof(struct hxc_caxecraft_app_LoadedWorldItem), &hxc_l_tmp_array_create_result_n43) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_loadedItems = hxc_l_tmp_array_create_result_n43;
  int32_t hxc_l_g_hc17b24fded04 = 0;
  struct hxc_caxecraft_content_RuntimeLevelCandidate *hxc_l_tmp_load_result_n44 = hxc_l_loadedCandidate;
  hxc_l_gc_roots[27] = (const void *)hxc_l_tmp_load_result_n44;
  if (hxc_l_tmp_load_result_n44 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_content_LoadedContentGeneration *hxc_l_tmp_class_field_load_result_n45 = (*hxc_l_tmp_load_result_n44).hxc_loadedGeneration;
  hxc_l_gc_roots[28] = (const void *)hxc_l_tmp_class_field_load_result_n45;
  if (hxc_l_tmp_class_field_load_result_n45 == NULL)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_tmp_instance_call_result_n46 = hxc_caxecraft_content_LoadedContentGeneration_itemBindings(hxc_l_tmp_class_field_load_result_n45);
  hxc_l_g1_hcad3014c33f5 = hxc_l_tmp_instance_call_result_n46;
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n47 = hxc_l_g_hc17b24fded04;
    int32_t hxc_l_tmp_array_length_result_n49;
    if (hxc_array_ref_length(hxc_l_g1_hcad3014c33f5, &hxc_l_tmp_array_length_result_n49) != HXC_STATUS_OK)
    {
      abort();
    }
    if (!(hxc_l_tmp_load_result_n47 < hxc_l_tmp_array_length_result_n49))
    {
      break;
    }
    struct hxc_array_ref *hxc_l_tmp_load_result_n50 = hxc_l_g1_hcad3014c33f5;
    struct hxc_caxecraft_content_ResolvedItem hxc_l_tmp_array_get_result_n52;
    if (hxc_array_ref_get_copy(hxc_l_tmp_load_result_n50, (size_t)hxc_l_g_hc17b24fded04, &hxc_l_tmp_array_get_result_n52) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_array_element_owner_n23 = hxc_l_tmp_array_get_result_n52;
    hxc_l_binding_h1fed9b751f82 = hxc_l_tmp_array_element_owner_n23;
    if (hxc_record_6512ae83_retain(&hxc_l_binding_h1fed9b751f82) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_g_hc17b24fded04 = hxc_i32_add_wrapping(hxc_l_g_hc17b24fded04, 1);
    struct hxc_array_ref *hxc_l_tmp_load_result_n55 = hxc_l_loadedItems;
    int32_t hxc_l_tmp_record_field_load_result_n56 = hxc_l_binding_h1fed9b751f82.hxc_storage;
    int32_t hxc_l_tmp_record_field_load_result_n57 = hxc_l_binding_h1fed9b751f82.hxc_transform.hxc_xMilli;
    int32_t hxc_l_tmp_record_field_load_result_n58 = hxc_l_binding_h1fed9b751f82.hxc_transform.hxc_yMilli;
    int32_t hxc_l_tmp_array_push_result_n61;
    if (hxc_array_ref_push_copy(hxc_l_tmp_load_result_n55, &(struct hxc_caxecraft_app_LoadedWorldItem){ .hxc_storageCode = hxc_l_tmp_record_field_load_result_n56, .hxc_xMilli = hxc_l_tmp_record_field_load_result_n57, .hxc_yMilli = hxc_l_tmp_record_field_load_result_n58, .hxc_zMilli = hxc_l_binding_h1fed9b751f82.hxc_transform.hxc_zMilli }, &hxc_l_tmp_array_push_result_n61) != HXC_STATUS_OK)
    {
      abort();
    }
    (void)hxc_l_tmp_array_push_result_n61;
    hxc_record_6512ae83_destroy(&hxc_l_binding_h1fed9b751f82);
    hxc_array_d5f9659a_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n23);
  }
  struct hxc_caxecraft_content_RuntimeLevelCandidate *hxc_l_tmp_load_result_n62 = hxc_l_loadedCandidate;
  hxc_l_gc_roots[29] = (const void *)hxc_l_tmp_load_result_n62;
  if (hxc_l_tmp_load_result_n62 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_content_LoadedContentGeneration *hxc_l_tmp_class_field_load_result_n63 = (*hxc_l_tmp_load_result_n62).hxc_loadedGeneration;
  hxc_l_gc_roots[30] = (const void *)hxc_l_tmp_class_field_load_result_n63;
  if (hxc_l_tmp_class_field_load_result_n63 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_content_ResolvedLevelPlan *hxc_l_tmp_class_field_load_result_n64 = (*hxc_l_tmp_class_field_load_result_n63).hxc_resolvedPlan;
  hxc_l_gc_roots[31] = (const void *)hxc_l_tmp_class_field_load_result_n64;
  if (hxc_l_tmp_class_field_load_result_n64 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_content_ResolvedPlayer hxc_l_tmp_instance_call_result_n65 = hxc_caxecraft_content_ResolvedLevelPlan_player(hxc_l_tmp_class_field_load_result_n64);
  hxc_l_tmp_record_field_transform_receiver_owner_n25 = hxc_l_tmp_instance_call_result_n65;
  struct hxc_caxecraft_scenario_ScenarioTransform hxc_l_spawnTransform = hxc_l_tmp_record_field_transform_receiver_owner_n25.hxc_transform;
  struct hxc_caxecraft_content_RuntimeLevelCandidate *hxc_l_tmp_load_result_n68 = hxc_l_loadedCandidate;
  hxc_l_gc_roots[32] = (const void *)hxc_l_tmp_load_result_n68;
  if (hxc_l_tmp_load_result_n68 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_content_LoadedContentGeneration *hxc_l_tmp_class_field_load_result_n69 = (*hxc_l_tmp_load_result_n68).hxc_loadedGeneration;
  hxc_l_gc_roots[33] = (const void *)hxc_l_tmp_class_field_load_result_n69;
  if (hxc_l_tmp_class_field_load_result_n69 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_content_ResolvedLevelPresentationPlan *hxc_l_tmp_class_field_load_result_n70 = (*hxc_l_tmp_class_field_load_result_n69).hxc_presentationPlan;
  hxc_l_gc_roots[34] = (const void *)hxc_l_tmp_class_field_load_result_n70;
  if (hxc_l_tmp_class_field_load_result_n70 == NULL)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_tmp_instance_call_result_n71 = hxc_caxecraft_content_ResolvedLevelPresentationPlan_fluidRequests(hxc_l_tmp_class_field_load_result_n70);
  hxc_l_fluidPresentation = hxc_l_tmp_instance_call_result_n71;
  int32_t hxc_l_tmp_array_length_result_n73;
  if (hxc_array_ref_length(hxc_l_fluidPresentation, &hxc_l_tmp_array_length_result_n73) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_l_tmp_array_length_result_n73 == 0)
  {
    if (hxc_array_ref_release(hxc_l_fluidPresentation) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_record_2bd8b029_destroy(&hxc_l_tmp_record_field_transform_receiver_owner_n25);
    if (hxc_array_ref_release(hxc_l_g1_hcad3014c33f5) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_loadedItems) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_g1_h881bba9116a1) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_actorBindings) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_enum_be0a6f10_destroy(&hxc_l_g_h4ef171f4fe67);
    hxc_enum_21bc9bf3_destroy(&hxc_l_tmp_static_call_argument_0_owner_n8);
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return;
  }
  struct hxc_caxecraft_content_ResolvedFluidPresentation hxc_l_tmp_array_get_result_n75;
  if (hxc_array_ref_get_copy(hxc_l_fluidPresentation, (size_t)0, &hxc_l_tmp_array_get_result_n75) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_tmp_array_element_owner_n28 = hxc_l_tmp_array_get_result_n75;
  int32_t hxc_l_waterPresentationCell = hxc_l_tmp_array_element_owner_n28.hxc_cellIndex;
  int32_t hxc_l_g_hfd304fde43bf = 0;
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n78 = hxc_l_g_hfd304fde43bf;
    int32_t hxc_l_tmp_array_length_result_n80;
    if (hxc_array_ref_length(hxc_l_fluidPresentation, &hxc_l_tmp_array_length_result_n80) != HXC_STATUS_OK)
    {
      abort();
    }
    if (!(hxc_l_tmp_load_result_n78 < hxc_l_tmp_array_length_result_n80))
    {
      break;
    }
    struct hxc_array_ref *hxc_l_tmp_load_result_n81 = hxc_l_fluidPresentation;
    struct hxc_caxecraft_content_ResolvedFluidPresentation hxc_l_tmp_array_get_result_n83;
    if (hxc_array_ref_get_copy(hxc_l_tmp_load_result_n81, (size_t)hxc_l_g_hfd304fde43bf, &hxc_l_tmp_array_get_result_n83) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_array_element_owner_n31 = hxc_l_tmp_array_get_result_n83;
    hxc_l_request = hxc_l_tmp_array_element_owner_n31;
    if (hxc_record_c8b1d97c_retain(&hxc_l_request) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_g_hfd304fde43bf = hxc_i32_add_wrapping(hxc_l_g_hfd304fde43bf, 1);
    int32_t hxc_l_tmp_record_field_load_result_n86 = hxc_l_request.hxc_cellIndex;
    if (hxc_l_tmp_record_field_load_result_n86 != hxc_l_waterPresentationCell)
    {
      hxc_record_c8b1d97c_destroy(&hxc_l_request);
      hxc_array_1f383c02_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n31);
      hxc_array_1f383c02_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n28);
      if (hxc_array_ref_release(hxc_l_fluidPresentation) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_record_2bd8b029_destroy(&hxc_l_tmp_record_field_transform_receiver_owner_n25);
      if (hxc_array_ref_release(hxc_l_g1_hcad3014c33f5) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_l_loadedItems) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_l_g1_h881bba9116a1) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_l_actorBindings) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_enum_be0a6f10_destroy(&hxc_l_g_h4ef171f4fe67);
      hxc_enum_21bc9bf3_destroy(&hxc_l_tmp_static_call_argument_0_owner_n8);
      if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
      {
        abort();
      }
      return;
    }
    hxc_record_c8b1d97c_destroy(&hxc_l_request);
    hxc_array_1f383c02_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n31);
  }
  struct hxc_caxecraft_content_RuntimeLevelCandidate *hxc_l_tmp_load_result_n88 = hxc_l_loadedCandidate;
  hxc_l_gc_roots[35] = (const void *)hxc_l_tmp_load_result_n88;
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
  hxc_l_tmp_sys_println_string_argument_owner_n33 = hxc_l_tmp_string_concat_result_n91;
  if (hxc_io_println(hxc_l_tmp_sys_println_string_argument_owner_n33) != HXC_STATUS_OK)
  {
    if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n33) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_record_f6127605_destroy(&hxc_l_receipt);
    hxc_array_1f383c02_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n28);
    if (hxc_array_ref_release(hxc_l_fluidPresentation) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_record_2bd8b029_destroy(&hxc_l_tmp_record_field_transform_receiver_owner_n25);
    if (hxc_array_ref_release(hxc_l_g1_hcad3014c33f5) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_loadedItems) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_g1_h881bba9116a1) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_actorBindings) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_enum_be0a6f10_destroy(&hxc_l_g_h4ef171f4fe67);
    hxc_enum_21bc9bf3_destroy(&hxc_l_tmp_static_call_argument_0_owner_n8);
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    abort();
  }
  if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n33) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_string hxc_l_tmp_std_string_int_result_n94 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
  if (hxc_string_from_int32(hxc_l_receipt.hxc_inputHash, hxc_default_allocator(), &hxc_l_tmp_std_string_int_result_n94) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_tmp_string_concat_right_owner_n34 = hxc_l_tmp_std_string_int_result_n94;
  hxc_string hxc_l_tmp_string_concat_result_n96 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
  if (hxc_string_concat_ref((hxc_string){ (const uint8_t *)"caxecraft: content-input-hash=", 30, true, NULL }, hxc_l_tmp_string_concat_right_owner_n34, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n96) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_tmp_sys_println_string_argument_owner_n35 = hxc_l_tmp_string_concat_result_n96;
  if (hxc_io_println(hxc_l_tmp_sys_println_string_argument_owner_n35) != HXC_STATUS_OK)
  {
    if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n35) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n34) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_record_f6127605_destroy(&hxc_l_receipt);
    hxc_array_1f383c02_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n28);
    if (hxc_array_ref_release(hxc_l_fluidPresentation) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_record_2bd8b029_destroy(&hxc_l_tmp_record_field_transform_receiver_owner_n25);
    if (hxc_array_ref_release(hxc_l_g1_hcad3014c33f5) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_loadedItems) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_g1_h881bba9116a1) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_actorBindings) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_enum_be0a6f10_destroy(&hxc_l_g_h4ef171f4fe67);
    hxc_enum_21bc9bf3_destroy(&hxc_l_tmp_static_call_argument_0_owner_n8);
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
  if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n34) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_caxecraft_content_ActiveContent *hxc_l_tmp_load_result_n98 = hxc_l_activeContent;
  hxc_l_gc_roots[36] = (const void *)hxc_l_tmp_load_result_n98;
  if (hxc_l_tmp_load_result_n98 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_content_LoadedContentGeneration *hxc_l_tmp_class_field_load_result_n99 = (*hxc_l_tmp_load_result_n98).hxc_active;
  hxc_l_gc_roots[37] = (const void *)hxc_l_tmp_class_field_load_result_n99;
  if (hxc_l_tmp_class_field_load_result_n99 == NULL)
  {
    abort();
  }
  int32_t hxc_l_tmp_class_field_load_result_n100 = (*hxc_l_tmp_class_field_load_result_n99).hxc_generationIdValue;
  hxc_string hxc_l_tmp_std_string_int_result_n101 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
  if (hxc_string_from_int32(hxc_l_tmp_class_field_load_result_n100, hxc_default_allocator(), &hxc_l_tmp_std_string_int_result_n101) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_tmp_string_concat_right_owner_n36 = hxc_l_tmp_std_string_int_result_n101;
  hxc_string hxc_l_tmp_string_concat_result_n103 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
  if (hxc_string_concat_ref((hxc_string){ (const uint8_t *)"caxecraft: content-generation=", 30, true, NULL }, hxc_l_tmp_string_concat_right_owner_n36, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n103) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_tmp_sys_println_string_argument_owner_n37 = hxc_l_tmp_string_concat_result_n103;
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
    hxc_array_1f383c02_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n28);
    if (hxc_array_ref_release(hxc_l_fluidPresentation) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_record_2bd8b029_destroy(&hxc_l_tmp_record_field_transform_receiver_owner_n25);
    if (hxc_array_ref_release(hxc_l_g1_hcad3014c33f5) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_loadedItems) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_g1_h881bba9116a1) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_actorBindings) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_enum_be0a6f10_destroy(&hxc_l_g_h4ef171f4fe67);
    hxc_enum_21bc9bf3_destroy(&hxc_l_tmp_static_call_argument_0_owner_n8);
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
  struct hxc_caxecraft_app_CaxecraftApp_PlayableActorSelection hxc_l_tmp_call_result_n106 = hxc_caxecraft_app_CaxecraftApp_selectPlayableActors(hxc_l_actorBindings);
  struct hxc_caxecraft_app_CaxecraftApp_PlayableActorSelection hxc_l_actors = hxc_l_tmp_call_result_n106;
  if (!hxc_l_actors.hxc_valid)
  {
    hxc_record_f6127605_destroy(&hxc_l_receipt);
    hxc_array_1f383c02_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n28);
    if (hxc_array_ref_release(hxc_l_fluidPresentation) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_record_2bd8b029_destroy(&hxc_l_tmp_record_field_transform_receiver_owner_n25);
    if (hxc_array_ref_release(hxc_l_g1_hcad3014c33f5) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_loadedItems) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_g1_h881bba9116a1) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_actorBindings) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_enum_be0a6f10_destroy(&hxc_l_g_h4ef171f4fe67);
    hxc_enum_21bc9bf3_destroy(&hxc_l_tmp_static_call_argument_0_owner_n8);
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
  bool hxc_l_tmp_native_call_result_n112 = IsWindowReady();
  if (!hxc_l_tmp_native_call_result_n112)
  {
    hxc_record_f6127605_destroy(&hxc_l_receipt);
    hxc_array_1f383c02_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n28);
    if (hxc_array_ref_release(hxc_l_fluidPresentation) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_record_2bd8b029_destroy(&hxc_l_tmp_record_field_transform_receiver_owner_n25);
    if (hxc_array_ref_release(hxc_l_g1_hcad3014c33f5) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_loadedItems) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_g1_h881bba9116a1) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_actorBindings) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_enum_be0a6f10_destroy(&hxc_l_g_h4ef171f4fe67);
    hxc_enum_21bc9bf3_destroy(&hxc_l_tmp_static_call_argument_0_owner_n8);
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
  struct Texture hxc_l_tmp_native_call_result_n113 = LoadTexture("assets/showcase/title-panorama.png");
  struct Texture hxc_l_titleTexture = hxc_l_tmp_native_call_result_n113;
  bool hxc_l_tmp_native_call_result_n115 = IsTextureValid(hxc_l_titleTexture);
  bool hxc_l_titleTextureReady = hxc_l_tmp_native_call_result_n115;
  struct Texture hxc_l_tmp_native_call_result_n116 = LoadTexture("assets/branding/caxecraft-wordmark.png");
  struct Texture hxc_l_wordmarkTexture = hxc_l_tmp_native_call_result_n116;
  bool hxc_l_tmp_native_call_result_n118 = IsTextureValid(hxc_l_wordmarkTexture);
  bool hxc_l_wordmarkTextureReady = hxc_l_tmp_native_call_result_n118;
  struct Texture hxc_l_tmp_native_call_result_n119 = LoadTexture("assets/atlases/hud.png");
  struct Texture hxc_l_hudTexture = hxc_l_tmp_native_call_result_n119;
  bool hxc_l_tmp_native_call_result_n121 = IsTextureValid(hxc_l_hudTexture);
  bool hxc_l_hudTextureReady = hxc_l_tmp_native_call_result_n121;
  struct Texture hxc_l_tmp_native_call_result_n122 = LoadTexture("assets/atlases/items.png");
  struct Texture hxc_l_itemTexture = hxc_l_tmp_native_call_result_n122;
  bool hxc_l_tmp_native_call_result_n124 = IsTextureValid(hxc_l_itemTexture);
  bool hxc_l_itemTextureReady = hxc_l_tmp_native_call_result_n124;
  struct Texture hxc_l_tmp_native_call_result_n125 = LoadTexture("assets/atlases/adventure-items.png");
  struct Texture hxc_l_adventureItemTexture = hxc_l_tmp_native_call_result_n125;
  bool hxc_l_tmp_native_call_result_n127 = IsTextureValid(hxc_l_adventureItemTexture);
  bool hxc_l_adventureItemTextureReady = hxc_l_tmp_native_call_result_n127;
  struct Texture hxc_l_tmp_load_result_n128 = hxc_l_hudTexture;
  bool hxc_l_tmp_load_result_n129 = hxc_l_hudTextureReady;
  struct Texture hxc_l_tmp_load_result_n130 = hxc_l_itemTexture;
  bool hxc_l_tmp_load_result_n131 = hxc_l_itemTextureReady;
  struct Texture hxc_l_tmp_load_result_n132 = hxc_l_adventureItemTexture;
  struct hxc_caxecraft_app_HudResources hxc_l_hudResources = (struct hxc_caxecraft_app_HudResources){ .hxc_adventureItemTexture = hxc_l_tmp_load_result_n132, .hxc_adventureItemTextureReady = hxc_l_adventureItemTextureReady, .hxc_hudTexture = hxc_l_tmp_load_result_n128, .hxc_hudTextureReady = hxc_l_tmp_load_result_n129, .hxc_itemTexture = hxc_l_tmp_load_result_n130, .hxc_itemTextureReady = hxc_l_tmp_load_result_n131 };
  struct Texture hxc_l_tmp_native_call_result_n135 = LoadTexture("assets/atlases/entities.png");
  struct Texture hxc_l_entityTexture = hxc_l_tmp_native_call_result_n135;
  bool hxc_l_tmp_native_call_result_n137 = IsTextureValid(hxc_l_entityTexture);
  bool hxc_l_entityTextureReady = hxc_l_tmp_native_call_result_n137;
  struct Texture hxc_l_tmp_native_call_result_n138 = LoadTexture("assets/atlases/terrain.png");
  struct Texture hxc_l_terrainTexture = hxc_l_tmp_native_call_result_n138;
  bool hxc_l_tmp_native_call_result_n140 = IsTextureValid(hxc_l_terrainTexture);
  bool hxc_l_terrainTextureReady = hxc_l_tmp_native_call_result_n140;
  struct Texture hxc_l_tmp_native_call_result_n141 = LoadTexture("assets/atlases/adventure-terrain.png");
  struct Texture hxc_l_adventureTerrainTexture = hxc_l_tmp_native_call_result_n141;
  bool hxc_l_tmp_native_call_result_n143 = IsTextureValid(hxc_l_adventureTerrainTexture);
  bool hxc_l_adventureTerrainTextureReady = hxc_l_tmp_native_call_result_n143;
  double hxc_l_cameraWaterBlend = 0.0;
  struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n144 = hxc_caxecraft_gameplay_Inventory_starter();
  struct hxc_caxecraft_gameplay_InventoryState hxc_l_inventory = hxc_l_tmp_call_result_n144;
  enum hxc_caxecraft_gameplay_GuidePhase hxc_l_guidePhase = hxc_caxecraft_gameplay_GuidePhase_Waiting;
  struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n146 = hxc_l_session;
  hxc_l_gc_roots[38] = (const void *)hxc_l_tmp_load_result_n146;
  if (hxc_l_tmp_load_result_n146 == NULL)
  {
    abort();
  }
  bool hxc_l_tmp_instance_call_result_n148 = hxc_caxecraft_domain_GameSession_actorInteractionAvailable(hxc_l_tmp_load_result_n146, hxc_l_dialogueActorId);
  bool hxc_l_guideInteractionAvailable = hxc_l_tmp_instance_call_result_n148;
  struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n149 = hxc_l_session;
  hxc_l_gc_roots[39] = (const void *)hxc_l_tmp_load_result_n149;
  if (hxc_l_tmp_load_result_n149 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n151 = hxc_caxecraft_domain_GameSession_readCharacter(hxc_l_tmp_load_result_n149, hxc_l_dialogueActorId);
  struct hxc_caxecraft_domain_Character hxc_l_dialogueActor = hxc_l_tmp_instance_call_result_n151;
  struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n152 = hxc_l_session;
  hxc_l_gc_roots[40] = (const void *)hxc_l_tmp_load_result_n152;
  if (hxc_l_tmp_load_result_n152 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n154 = hxc_caxecraft_domain_GameSession_readCharacter(hxc_l_tmp_load_result_n152, hxc_l_enemyActorId);
  struct hxc_caxecraft_domain_Character hxc_l_enemyActor = hxc_l_tmp_instance_call_result_n154;
  struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n155 = hxc_l_session;
  hxc_l_gc_roots[41] = (const void *)hxc_l_tmp_load_result_n155;
  if (hxc_l_tmp_load_result_n155 == NULL)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_tmp_instance_call_result_n156 = hxc_caxecraft_domain_GameSession_actorControllerStateSnapshots(hxc_l_tmp_load_result_n155);
  hxc_l_initialActorPhases = hxc_l_tmp_instance_call_result_n156;
  struct hxc_array_ref *hxc_l_tmp_load_result_n157 = hxc_l_initialActorPhases;
  struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation hxc_l_tmp_call_result_n160 = hxc_caxecraft_app_CaxecraftApp_observeActorPhase(hxc_l_tmp_load_result_n157, hxc_l_dialogueActorId, hxc_caxecraft_domain_ActorControllerPhase_Stationary);
  struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation hxc_l_dialoguePhase = hxc_l_tmp_call_result_n160;
  struct hxc_array_ref *hxc_l_tmp_load_result_n161 = hxc_l_initialActorPhases;
  struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation hxc_l_tmp_call_result_n164 = hxc_caxecraft_app_CaxecraftApp_observeActorPhase(hxc_l_tmp_load_result_n161, hxc_l_enemyActorId, hxc_caxecraft_domain_ActorControllerPhase_Resting);
  struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation hxc_l_enemyPhase = hxc_l_tmp_call_result_n164;
  int32_t hxc_l_tmp_record_field_load_result_n165 = hxc_l_dialogueActor.hxc_id;
  bool hxc_l_tmp_short_circuit_result_n68 = hxc_l_tmp_record_field_load_result_n165 <= 0;
  if (!(hxc_l_tmp_record_field_load_result_n165 <= 0))
  {
    hxc_l_tmp_short_circuit_result_n68 = hxc_l_enemyActor.hxc_id <= 0;
  }
  bool hxc_l_tmp_short_circuit_load_result_n167 = hxc_l_tmp_short_circuit_result_n68;
  bool hxc_l_tmp_short_circuit_result_n69 = hxc_l_tmp_short_circuit_load_result_n167;
  if (!hxc_l_tmp_short_circuit_load_result_n167)
  {
    hxc_l_tmp_short_circuit_result_n69 = !hxc_l_dialoguePhase.hxc_valid;
  }
  bool hxc_l_tmp_short_circuit_load_result_n169 = hxc_l_tmp_short_circuit_result_n69;
  bool hxc_l_tmp_short_circuit_result_n70 = hxc_l_tmp_short_circuit_load_result_n169;
  if (!hxc_l_tmp_short_circuit_load_result_n169)
  {
    hxc_l_tmp_short_circuit_result_n70 = !hxc_l_enemyPhase.hxc_valid;
  }
  if (hxc_l_tmp_short_circuit_result_n70)
  {
    if (hxc_array_ref_release(hxc_l_initialActorPhases) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_record_f6127605_destroy(&hxc_l_receipt);
    hxc_array_1f383c02_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n28);
    if (hxc_array_ref_release(hxc_l_fluidPresentation) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_record_2bd8b029_destroy(&hxc_l_tmp_record_field_transform_receiver_owner_n25);
    if (hxc_array_ref_release(hxc_l_g1_hcad3014c33f5) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_loadedItems) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_g1_h881bba9116a1) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_actorBindings) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_enum_be0a6f10_destroy(&hxc_l_g_h4ef171f4fe67);
    hxc_enum_21bc9bf3_destroy(&hxc_l_tmp_static_call_argument_0_owner_n8);
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
  struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n174 = hxc_l_session;
  hxc_l_gc_roots[42] = (const void *)hxc_l_tmp_load_result_n174;
  if (hxc_l_tmp_load_result_n174 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_domain_GameView hxc_l_tmp_instance_call_result_n175 = hxc_caxecraft_domain_GameSession_view(hxc_l_tmp_load_result_n174);
  struct hxc_caxecraft_domain_GameView hxc_l_initialPresentation = hxc_l_tmp_instance_call_result_n175;
  if (!hxc_l_initialPresentation.hxc_valid)
  {
    if (hxc_array_ref_release(hxc_l_initialActorPhases) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_record_f6127605_destroy(&hxc_l_receipt);
    hxc_array_1f383c02_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n28);
    if (hxc_array_ref_release(hxc_l_fluidPresentation) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_record_2bd8b029_destroy(&hxc_l_tmp_record_field_transform_receiver_owner_n25);
    if (hxc_array_ref_release(hxc_l_g1_hcad3014c33f5) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_loadedItems) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_g1_h881bba9116a1) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_actorBindings) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_enum_be0a6f10_destroy(&hxc_l_g_h4ef171f4fe67);
    hxc_enum_21bc9bf3_destroy(&hxc_l_tmp_static_call_argument_0_owner_n8);
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return;
  }
  struct hxc_caxecraft_app_MotionHistory hxc_l_tmp_call_result_n178 = hxc_caxecraft_app_MotionInterpolation_start(hxc_l_initialPresentation.hxc_localPlayer.hxc_body);
  struct hxc_caxecraft_app_MotionHistory hxc_l_motionHistory = hxc_l_tmp_call_result_n178;
  bool hxc_l_jumpQueued = false;
  bool hxc_l_swordQueued = false;
  int32_t hxc_l_selectedMode = 0;
  int32_t hxc_l_locale = 0;
  bool hxc_l_showInitialTitle = true;
  enum hxc_caxecraft_app_AppScreen hxc_l_tmp_call_result_n180 = hxc_caxecraft_app_AppScreen_initialScreen(hxc_l_showInitialTitle);
  enum hxc_caxecraft_app_AppScreen hxc_l_screen = hxc_l_tmp_call_result_n180;
  bool hxc_l_tmp_call_result_n182 = hxc_caxecraft_app_AppScreen_capturesPointer(hxc_l_screen);
  if (hxc_l_tmp_call_result_n182)
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
    bool hxc_l_tmp_load_result_n183 = hxc_l_quit;
    bool hxc_l_tmp_short_circuit_result_n99 = !hxc_l_tmp_load_result_n183;
    if (!hxc_l_tmp_load_result_n183)
    {
      bool hxc_l_tmp_native_call_result_n184 = WindowShouldClose();
      hxc_l_tmp_short_circuit_result_n99 = !hxc_l_tmp_native_call_result_n184;
    }
    if (!hxc_l_tmp_short_circuit_result_n99)
    {
      break;
    }
    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n186 = hxc_l_session;
    hxc_l_gc_roots[43] = (const void *)hxc_l_tmp_load_result_n186;
    if (hxc_l_tmp_load_result_n186 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_domain_GameView hxc_l_tmp_instance_call_result_n187 = hxc_caxecraft_domain_GameSession_view(hxc_l_tmp_load_result_n186);
    struct hxc_caxecraft_domain_GameView hxc_l_initialView = hxc_l_tmp_instance_call_result_n187;
    if (!hxc_l_initialView.hxc_valid)
    {
      hxc_l_quit = true;
    }
    struct hxc_caxecraft_domain_Character hxc_l_character = hxc_l_initialView.hxc_localPlayer;
    bool hxc_l_recapturedThisFrame = false;
    bool hxc_l_resetMotionThisFrame = false;
    float hxc_l_tmp_native_call_result_n190 = GetFrameTime();
    double hxc_l_frameSeconds = (double)hxc_l_tmp_native_call_result_n190;
    if (hxc_l_frameSeconds > 0.25)
    {
      hxc_l_frameSeconds = 0.25;
    }
    bool hxc_l_tmp_native_call_result_n192 = IsWindowFocused();
    bool hxc_l_focused = hxc_l_tmp_native_call_result_n192;
    bool hxc_l_tmp_call_result_n194 = hxc_caxecraft_app_AppScreen_capturesPointer(hxc_l_screen);
    bool hxc_l_captured_ha2526e286571 = hxc_l_tmp_call_result_n194;
    bool hxc_l_tmp_call_result_n196 = hxc_caxecraft_app_AppScreen_pausesSimulation(hxc_l_screen);
    bool hxc_l_paused_hea66990f24cf = hxc_l_tmp_call_result_n196;
    double hxc_l_forward = 0.0;
    double hxc_l_right = 0.0;
    bool hxc_l_tmp_native_call_result_n197 = IsKeyDown((int32_t)87);
    if (hxc_l_tmp_native_call_result_n197)
    {
      hxc_l_forward = hxc_l_forward + 1.0;
    }
    bool hxc_l_tmp_native_call_result_n199 = IsKeyDown((int32_t)83);
    if (hxc_l_tmp_native_call_result_n199)
    {
      hxc_l_forward = hxc_l_forward - 1.0;
    }
    bool hxc_l_tmp_native_call_result_n201 = IsKeyDown((int32_t)68);
    if (hxc_l_tmp_native_call_result_n201)
    {
      hxc_l_right = hxc_l_right + 1.0;
    }
    bool hxc_l_tmp_native_call_result_n203 = IsKeyDown((int32_t)65);
    if (hxc_l_tmp_native_call_result_n203)
    {
      hxc_l_right = hxc_l_right - 1.0;
    }
    double hxc_l_lookYaw_h739dfcfe3f81 = 0.0;
    double hxc_l_lookPitch_h47e23f7117ed = 0.0;
    if (hxc_l_captured_ha2526e286571)
    {
      struct Vector2 hxc_l_tmp_native_call_result_n206 = GetMouseDelta();
      struct Vector2 hxc_l_mouse = hxc_l_tmp_native_call_result_n206;
      hxc_l_lookYaw_h739dfcfe3f81 = -(double)hxc_l_mouse.x * 0.0025;
      hxc_l_lookPitch_h47e23f7117ed = -(double)hxc_l_mouse.y * 0.0025;
    }
    bool hxc_l_tmp_native_call_result_n211 = IsMouseButtonPressed((int32_t)0);
    bool hxc_l_leftPressed = hxc_l_tmp_native_call_result_n211;
    bool hxc_l_tmp_load_result_n212 = hxc_l_captured_ha2526e286571;
    bool hxc_l_tmp_short_circuit_result_n115 = hxc_l_tmp_load_result_n212;
    if (hxc_l_tmp_load_result_n212)
    {
      hxc_l_tmp_short_circuit_result_n115 = hxc_l_leftPressed;
    }
    bool hxc_l_primaryPressed_he064f431f4bf = hxc_l_tmp_short_circuit_result_n115;
    bool hxc_l_tmp_load_result_n215 = hxc_l_captured_ha2526e286571;
    bool hxc_l_tmp_short_circuit_result_n117 = hxc_l_tmp_load_result_n215;
    if (hxc_l_tmp_load_result_n215)
    {
      bool hxc_l_tmp_native_call_result_n216 = IsMouseButtonPressed((int32_t)1);
      hxc_l_tmp_short_circuit_result_n117 = hxc_l_tmp_native_call_result_n216;
    }
    bool hxc_l_secondaryPressed_h8a854a2f9a9c = hxc_l_tmp_short_circuit_result_n117;
    bool hxc_l_tmp_load_result_n218 = hxc_l_captured_ha2526e286571;
    bool hxc_l_tmp_short_circuit_result_n119 = hxc_l_tmp_load_result_n218;
    if (hxc_l_tmp_load_result_n218)
    {
      bool hxc_l_tmp_native_call_result_n219 = IsKeyPressed((int32_t)69);
      hxc_l_tmp_short_circuit_result_n119 = hxc_l_tmp_native_call_result_n219;
    }
    bool hxc_l_interactPressed_h74a514831a90 = hxc_l_tmp_short_circuit_result_n119;
    int32_t hxc_l_hotbarSelection_h0a17e2929cf6 = -1;
    bool hxc_l_tmp_native_call_result_n221 = IsKeyPressed((int32_t)49);
    if (hxc_l_tmp_native_call_result_n221)
    {
      hxc_l_hotbarSelection_h0a17e2929cf6 = 0;
    }
    bool hxc_l_tmp_native_call_result_n222 = IsKeyPressed((int32_t)50);
    if (hxc_l_tmp_native_call_result_n222)
    {
      hxc_l_hotbarSelection_h0a17e2929cf6 = 1;
    }
    bool hxc_l_tmp_native_call_result_n223 = IsKeyPressed((int32_t)51);
    if (hxc_l_tmp_native_call_result_n223)
    {
      hxc_l_hotbarSelection_h0a17e2929cf6 = 2;
    }
    bool hxc_l_tmp_native_call_result_n224 = IsKeyPressed((int32_t)52);
    if (hxc_l_tmp_native_call_result_n224)
    {
      hxc_l_hotbarSelection_h0a17e2929cf6 = 3;
    }
    bool hxc_l_tmp_native_call_result_n225 = IsKeyPressed((int32_t)53);
    if (hxc_l_tmp_native_call_result_n225)
    {
      hxc_l_hotbarSelection_h0a17e2929cf6 = 4;
    }
    bool hxc_l_tmp_native_call_result_n226 = IsKeyPressed((int32_t)54);
    if (hxc_l_tmp_native_call_result_n226)
    {
      hxc_l_hotbarSelection_h0a17e2929cf6 = 5;
    }
    bool hxc_l_tmp_native_call_result_n227 = IsKeyPressed((int32_t)55);
    if (hxc_l_tmp_native_call_result_n227)
    {
      hxc_l_hotbarSelection_h0a17e2929cf6 = 6;
    }
    bool hxc_l_tmp_native_call_result_n228 = IsKeyPressed((int32_t)56);
    if (hxc_l_tmp_native_call_result_n228)
    {
      hxc_l_hotbarSelection_h0a17e2929cf6 = 7;
    }
    float hxc_l_tmp_native_call_result_n229 = GetMouseWheelMove();
    double hxc_l_wheel = (double)hxc_l_tmp_native_call_result_n229;
    int32_t hxc_l_hotbarCycle_h3fd38d368291 = 0;
    if (hxc_l_wheel > 0.0)
    {
      hxc_l_hotbarCycle_h3fd38d368291 = -1;
    }
    if (hxc_l_wheel < 0.0)
    {
      hxc_l_hotbarCycle_h3fd38d368291 = 1;
    }
    bool hxc_l_tmp_native_call_result_n232 = IsKeyPressed((int32_t)32);
    bool hxc_l_jumpPressed_h09da3deeced9 = hxc_l_tmp_native_call_result_n232;
    bool hxc_l_tmp_native_call_result_n233 = IsKeyPressed((int32_t)256);
    bool hxc_l_pausePressed_h3fe966eb7aee = hxc_l_tmp_native_call_result_n233;
    bool hxc_l_tmp_native_call_result_n234 = IsKeyPressed((int32_t)81);
    bool hxc_l_quitPressed_hd72aa63127f7 = hxc_l_tmp_native_call_result_n234;
    double hxc_l_frameInput_moveForward = hxc_l_forward;
    double hxc_l_frameInput_moveRight = hxc_l_right;
    double hxc_l_frameInput_lookYaw = hxc_l_lookYaw_h739dfcfe3f81;
    double hxc_l_frameInput_lookPitch = hxc_l_lookPitch_h47e23f7117ed;
    bool hxc_l_frameInput_jumpPressed = hxc_l_jumpPressed_h09da3deeced9;
    bool hxc_l_tmp_native_call_result_n240 = IsKeyDown((int32_t)340);
    bool hxc_l_frameInput_descendHeld = hxc_l_tmp_native_call_result_n240;
    bool hxc_l_frameInput_primaryPressed = hxc_l_primaryPressed_he064f431f4bf;
    bool hxc_l_frameInput_secondaryPressed = hxc_l_secondaryPressed_h8a854a2f9a9c;
    bool hxc_l_frameInput_interactPressed = hxc_l_interactPressed_h74a514831a90;
    bool hxc_l_frameInput_pausePressed = hxc_l_pausePressed_h3fe966eb7aee;
    bool hxc_l_tmp_load_result_n245 = hxc_l_paused_hea66990f24cf;
    bool hxc_l_tmp_short_circuit_result_n137 = hxc_l_tmp_load_result_n245;
    if (hxc_l_tmp_load_result_n245)
    {
      hxc_l_tmp_short_circuit_result_n137 = hxc_l_leftPressed;
    }
    bool hxc_l_frameInput_capturePressed = hxc_l_tmp_short_circuit_result_n137;
    bool hxc_l_frameInput_quitPressed = hxc_l_quitPressed_hd72aa63127f7;
    int32_t hxc_l_frameInput_hotbarSelection = hxc_l_hotbarSelection_h0a17e2929cf6;
    int32_t hxc_l_frameInput_hotbarCycle = hxc_l_hotbarCycle_h3fd38d368291;
    double hxc_l_moveForward = hxc_l_frameInput_moveForward;
    double hxc_l_moveRight = hxc_l_frameInput_moveRight;
    double hxc_l_lookYaw_h657da7ce2b3a = hxc_l_frameInput_lookYaw;
    double hxc_l_lookPitch_hb5cfe6bcd89b = hxc_l_frameInput_lookPitch;
    bool hxc_l_jumpPressed_h3b66348ee5f1 = hxc_l_frameInput_jumpPressed;
    bool hxc_l_primaryPressed_h01b630731264 = hxc_l_frameInput_primaryPressed;
    bool hxc_l_secondaryPressed_hed35213a6b62 = hxc_l_frameInput_secondaryPressed;
    bool hxc_l_interactPressed_hdb7bc0153d0e = hxc_l_frameInput_interactPressed;
    bool hxc_l_pausePressed_h4d3208f579a1 = hxc_l_frameInput_pausePressed;
    bool hxc_l_capturePressed = hxc_l_frameInput_capturePressed;
    bool hxc_l_quitPressed_he27cd15ff447 = hxc_l_frameInput_quitPressed;
    int32_t hxc_l_hotbarSelection_h8c6e6e157570 = hxc_l_frameInput_hotbarSelection;
    int32_t hxc_l_hotbarCycle_h351cf5ac8b18 = hxc_l_frameInput_hotbarCycle;
    bool hxc_l_descendHeld = hxc_l_frameInput_descendHeld;
    if (hxc_l_quitPressed_he27cd15ff447)
    {
      hxc_l_quit = true;
    }
    if (hxc_l_hotbarSelection_h8c6e6e157570 >= 0)
    {
      struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_load_result_n267 = hxc_l_inventory;
      struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n269 = hxc_caxecraft_gameplay_Inventory_select(hxc_l_tmp_load_result_n267, hxc_l_hotbarSelection_h8c6e6e157570);
      hxc_l_inventory = hxc_l_tmp_call_result_n269;
    }
    if (hxc_l_hotbarCycle_h351cf5ac8b18 != 0)
    {
      struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_load_result_n271 = hxc_l_inventory;
      struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n273 = hxc_caxecraft_gameplay_Inventory_cycle(hxc_l_tmp_load_result_n271, hxc_l_hotbarCycle_h351cf5ac8b18);
      hxc_l_inventory = hxc_l_tmp_call_result_n273;
    }
    bool hxc_l_tmp_call_result_n275 = hxc_caxecraft_app_AppScreen_isPlaying(hxc_l_screen);
    bool hxc_l_tmp_short_circuit_result_n155 = hxc_l_tmp_call_result_n275;
    if (hxc_l_tmp_call_result_n275)
    {
      hxc_l_tmp_short_circuit_result_n155 = hxc_l_interactPressed_hdb7bc0153d0e;
    }
    if (hxc_l_tmp_short_circuit_result_n155)
    {
      if (hxc_l_character.hxc_vitals.hxc_health <= 0)
      {
        struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n279 = hxc_l_session;
        hxc_l_gc_roots[44] = (const void *)hxc_l_tmp_load_result_n279;
        if (hxc_l_tmp_load_result_n279 == NULL)
        {
          abort();
        }
        struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n280 = hxc_l_session;
        hxc_l_gc_roots[45] = (const void *)hxc_l_tmp_load_result_n280;
        if (hxc_l_tmp_load_result_n280 == NULL)
        {
          abort();
        }
        size_t hxc_l_tmp_length_n282;
        const uint8_t *hxc_l_tmp_instance_call_result_n281 = hxc_caxecraft_domain_GameSession_worldView(hxc_l_tmp_load_result_n280, &hxc_l_tmp_length_n282);
        struct hxc_caxecraft_domain_CharacterBody hxc_l_tmp_call_result_n284 = hxc_caxecraft_app_CaxecraftApp_spawnPlayer(hxc_l_tmp_instance_call_result_n281, hxc_l_tmp_length_n282, hxc_l_spawnTransform);
        struct hxc_caxecraft_domain_LocalCharacterCommandResult hxc_l_tmp_instance_call_result_n285 = hxc_caxecraft_domain_GameSession_reviveLocalPlayerAt(hxc_l_tmp_load_result_n279, hxc_l_tmp_call_result_n284);
        struct hxc_caxecraft_domain_LocalCharacterCommandResult hxc_l_revival = hxc_l_tmp_instance_call_result_n285;
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
        struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n288 = hxc_l_session;
        hxc_l_gc_roots[46] = (const void *)hxc_l_tmp_load_result_n288;
        if (hxc_l_tmp_load_result_n288 == NULL)
        {
          abort();
        }
        bool hxc_l_tmp_instance_call_result_n290 = hxc_caxecraft_domain_GameSession_actorInteractionAvailable(hxc_l_tmp_load_result_n288, hxc_l_dialogueActorId);
        if (hxc_l_tmp_instance_call_result_n290)
        {
          bool hxc_l_sharesBerries = hxc_l_guidePhase == hxc_caxecraft_gameplay_GuidePhase_Welcomed;
          if (hxc_l_sharesBerries)
          {
            int32_t hxc_l_tmp_call_result_n296 = hxc_caxecraft_gameplay_Inventory_acceptedAmount(hxc_l_inventory, hxc_caxecraft_gameplay_ItemKind_Berries, 2);
            int32_t hxc_l_acceptedGift = hxc_l_tmp_call_result_n296;
            if (hxc_l_acceptedGift == 2)
            {
              struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_load_result_n298 = hxc_l_inventory;
              struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n301 = hxc_caxecraft_gameplay_Inventory_collectItem(hxc_l_tmp_load_result_n298, hxc_caxecraft_gameplay_ItemKind_Berries, hxc_l_acceptedGift);
              hxc_l_inventory = hxc_l_tmp_call_result_n301;
              enum hxc_caxecraft_gameplay_GuidePhase hxc_l_tmp_call_result_n303 = hxc_caxecraft_app_CaxecraftApp_advanceGuidePhase(hxc_l_guidePhase);
              hxc_l_guidePhase = hxc_l_tmp_call_result_n303;
            }
            else
            {
              hxc_l_inventoryFullReason = 1;
              hxc_l_inventoryFullFrames = 90;
            }
          }
          else
          {
            enum hxc_caxecraft_gameplay_GuidePhase hxc_l_tmp_call_result_n305 = hxc_caxecraft_app_CaxecraftApp_advanceGuidePhase(hxc_l_guidePhase);
            hxc_l_guidePhase = hxc_l_tmp_call_result_n305;
          }
        }
      }
    }
    bool hxc_l_tmp_call_result_n307 = hxc_caxecraft_app_AppScreen_showsTitle(hxc_l_screen);
    bool hxc_l_tmp_short_circuit_result_n159 = hxc_l_tmp_call_result_n307;
    if (hxc_l_tmp_call_result_n307)
    {
      hxc_l_tmp_short_circuit_result_n159 = hxc_l_focused;
    }
    if (hxc_l_tmp_short_circuit_result_n159)
    {
      int32_t hxc_l_modeBeforeInput = hxc_l_selectedMode;
      bool hxc_l_tmp_native_call_result_n311 = IsKeyPressed((int32_t)76);
      if (hxc_l_tmp_native_call_result_n311)
      {
        int32_t hxc_l_tmp_call_result_n313 = hxc_caxecraft_localization_UiCatalog_nextLocale(hxc_l_locale);
        hxc_l_locale = hxc_l_tmp_call_result_n313;
      }
      bool hxc_l_tmp_native_call_result_n314 = IsKeyPressed((int32_t)265);
      bool hxc_l_tmp_short_circuit_result_n161 = hxc_l_tmp_native_call_result_n314;
      if (!hxc_l_tmp_native_call_result_n314)
      {
        bool hxc_l_tmp_native_call_result_n315 = IsKeyPressed((int32_t)264);
        hxc_l_tmp_short_circuit_result_n161 = hxc_l_tmp_native_call_result_n315;
      }
      if (hxc_l_tmp_short_circuit_result_n161)
      {
        int32_t hxc_l_tmp_load_result_n317 = hxc_l_selectedMode;
        int32_t hxc_l_tmp_conditional_result_n162 = 0;
        if (hxc_l_tmp_load_result_n317 == 0)
        {
          hxc_l_tmp_conditional_result_n162 = 1;
        }
        else
        {
          hxc_l_tmp_conditional_result_n162 = 0;
        }
        hxc_l_selectedMode = hxc_l_tmp_conditional_result_n162;
      }
      struct Vector2 hxc_l_tmp_native_call_result_n319 = GetMousePosition();
      struct Vector2 hxc_l_menuMouse = hxc_l_tmp_native_call_result_n319;
      struct Vector2 hxc_l_tmp_load_result_n320 = hxc_l_menuMouse;
      struct Vector2 hxc_l_tmp_load_result_n322 = hxc_l_menuMouse;
      int32_t hxc_l_tmp_native_call_result_n324 = GetScreenWidth();
      int32_t hxc_l_tmp_native_call_result_n325 = GetScreenHeight();
      int32_t hxc_l_tmp_call_result_n326 = hxc_caxecraft_app_TitleMenu_selectionAt((double)hxc_l_tmp_load_result_n320.x, (double)hxc_l_tmp_load_result_n322.y, (int32_t)hxc_l_tmp_native_call_result_n324, (int32_t)hxc_l_tmp_native_call_result_n325);
      int32_t hxc_l_hovered = hxc_l_tmp_call_result_n326;
      if (hxc_l_hovered == 0)
      {
        hxc_l_selectedMode = 0;
      }
      if (hxc_l_hovered == 1)
      {
        hxc_l_selectedMode = 1;
      }
      int32_t hxc_l_tmp_load_result_n329 = hxc_l_selectedMode;
      if (hxc_l_tmp_load_result_n329 != hxc_l_modeBeforeInput)
      {
        hxc_l_accumulator = 0.0;
        hxc_l_resetMotionThisFrame = true;
      }
      int32_t hxc_l_tmp_load_result_n331 = hxc_l_hovered;
      bool hxc_l_tmp_short_circuit_result_n166 = hxc_l_tmp_load_result_n331 >= 0;
      if (hxc_l_tmp_load_result_n331 >= 0)
      {
        bool hxc_l_tmp_native_call_result_n332 = IsMouseButtonPressed((int32_t)0);
        hxc_l_tmp_short_circuit_result_n166 = hxc_l_tmp_native_call_result_n332;
      }
      bool hxc_l_clickedChoice = hxc_l_tmp_short_circuit_result_n166;
      bool hxc_l_tmp_load_result_n334 = hxc_l_clickedChoice;
      bool hxc_l_tmp_short_circuit_result_n167 = hxc_l_tmp_load_result_n334;
      if (hxc_l_tmp_load_result_n334)
      {
        hxc_l_tmp_short_circuit_result_n167 = hxc_l_hovered == 2;
      }
      if (hxc_l_tmp_short_circuit_result_n167)
      {
        enum hxc_caxecraft_app_AppScreen hxc_l_tmp_call_result_n338 = hxc_caxecraft_app_AppScreen_openEditor(hxc_l_screen);
        hxc_l_screen = hxc_l_tmp_call_result_n338;
        hxc_l_accumulator = 0.0;
        hxc_l_resetMotionThisFrame = true;
        EnableCursor();
      }
      else
      {
        bool hxc_l_tmp_load_result_n339 = hxc_l_clickedChoice;
        bool hxc_l_tmp_short_circuit_result_n168 = hxc_l_tmp_load_result_n339;
        if (!hxc_l_tmp_load_result_n339)
        {
          bool hxc_l_tmp_native_call_result_n340 = IsKeyPressed((int32_t)257);
          hxc_l_tmp_short_circuit_result_n168 = hxc_l_tmp_native_call_result_n340;
        }
        if (hxc_l_tmp_short_circuit_result_n168)
        {
          enum hxc_caxecraft_app_AppScreen hxc_l_tmp_call_result_n343 = hxc_caxecraft_app_AppScreen_startPlaying(hxc_l_screen);
          hxc_l_screen = hxc_l_tmp_call_result_n343;
          hxc_l_recapturedThisFrame = true;
          hxc_l_accumulator = 0.0;
          hxc_l_resetMotionThisFrame = true;
          DisableCursor();
        }
      }
    }
    bool hxc_l_tmp_load_result_n344 = hxc_l_focused;
    bool hxc_l_tmp_short_circuit_result_n169 = !hxc_l_tmp_load_result_n344;
    if (!hxc_l_tmp_load_result_n344)
    {
      bool hxc_l_tmp_call_result_n346 = hxc_caxecraft_app_AppScreen_isPlaying(hxc_l_screen);
      hxc_l_tmp_short_circuit_result_n169 = hxc_l_tmp_call_result_n346;
    }
    if (hxc_l_tmp_short_circuit_result_n169)
    {
      enum hxc_caxecraft_app_AppScreen hxc_l_tmp_call_result_n349 = hxc_caxecraft_app_AppScreen_loseFocus(hxc_l_screen);
      hxc_l_screen = hxc_l_tmp_call_result_n349;
      hxc_l_jumpQueued = false;
      hxc_l_accumulator = 0.0;
      hxc_l_resetMotionThisFrame = true;
      EnableCursor();
    }
    bool hxc_l_tmp_call_result_n351 = hxc_caxecraft_app_AppScreen_showsTitle(hxc_l_screen);
    bool hxc_l_tmp_short_circuit_result_n170 = !hxc_l_tmp_call_result_n351;
    if (!hxc_l_tmp_call_result_n351)
    {
      bool hxc_l_tmp_call_result_n353 = hxc_caxecraft_app_AppScreen_showsEditor(hxc_l_screen);
      hxc_l_tmp_short_circuit_result_n170 = !hxc_l_tmp_call_result_n353;
    }
    bool hxc_l_tmp_short_circuit_load_result_n354 = hxc_l_tmp_short_circuit_result_n170;
    bool hxc_l_tmp_short_circuit_result_n171 = hxc_l_tmp_short_circuit_load_result_n354;
    if (hxc_l_tmp_short_circuit_load_result_n354)
    {
      hxc_l_tmp_short_circuit_result_n171 = hxc_l_focused;
    }
    bool hxc_l_tmp_short_circuit_load_result_n356 = hxc_l_tmp_short_circuit_result_n171;
    bool hxc_l_tmp_short_circuit_result_n172 = hxc_l_tmp_short_circuit_load_result_n356;
    if (hxc_l_tmp_short_circuit_load_result_n356)
    {
      hxc_l_tmp_short_circuit_result_n172 = hxc_l_pausePressed_h4d3208f579a1;
    }
    if (hxc_l_tmp_short_circuit_result_n172)
    {
      enum hxc_caxecraft_app_AppScreen hxc_l_tmp_call_result_n360 = hxc_caxecraft_app_AppScreen_togglePause(hxc_l_screen);
      hxc_l_screen = hxc_l_tmp_call_result_n360;
      hxc_l_accumulator = 0.0;
      hxc_l_resetMotionThisFrame = true;
      bool hxc_l_tmp_call_result_n362 = hxc_caxecraft_app_AppScreen_pausesSimulation(hxc_l_screen);
      if (hxc_l_tmp_call_result_n362)
      {
        hxc_l_jumpQueued = false;
      }
      bool hxc_l_tmp_call_result_n364 = hxc_caxecraft_app_AppScreen_capturesPointer(hxc_l_screen);
      if (hxc_l_tmp_call_result_n364)
      {
        DisableCursor();
      }
      else
      {
        EnableCursor();
      }
    }
    bool hxc_l_tmp_call_result_n366 = hxc_caxecraft_app_AppScreen_showsTitle(hxc_l_screen);
    bool hxc_l_tmp_short_circuit_result_n173 = !hxc_l_tmp_call_result_n366;
    if (!hxc_l_tmp_call_result_n366)
    {
      bool hxc_l_tmp_call_result_n368 = hxc_caxecraft_app_AppScreen_showsEditor(hxc_l_screen);
      hxc_l_tmp_short_circuit_result_n173 = !hxc_l_tmp_call_result_n368;
    }
    bool hxc_l_tmp_short_circuit_load_result_n369 = hxc_l_tmp_short_circuit_result_n173;
    bool hxc_l_tmp_short_circuit_result_n174 = hxc_l_tmp_short_circuit_load_result_n369;
    if (hxc_l_tmp_short_circuit_load_result_n369)
    {
      hxc_l_tmp_short_circuit_result_n174 = hxc_l_focused;
    }
    bool hxc_l_tmp_short_circuit_load_result_n371 = hxc_l_tmp_short_circuit_result_n174;
    bool hxc_l_tmp_short_circuit_result_n175 = hxc_l_tmp_short_circuit_load_result_n371;
    if (hxc_l_tmp_short_circuit_load_result_n371)
    {
      bool hxc_l_tmp_call_result_n373 = hxc_caxecraft_app_AppScreen_pausesSimulation(hxc_l_screen);
      hxc_l_tmp_short_circuit_result_n175 = hxc_l_tmp_call_result_n373;
    }
    bool hxc_l_tmp_short_circuit_load_result_n374 = hxc_l_tmp_short_circuit_result_n175;
    bool hxc_l_tmp_short_circuit_result_n176 = hxc_l_tmp_short_circuit_load_result_n374;
    if (hxc_l_tmp_short_circuit_load_result_n374)
    {
      hxc_l_tmp_short_circuit_result_n176 = hxc_l_capturePressed;
    }
    if (hxc_l_tmp_short_circuit_result_n176)
    {
      enum hxc_caxecraft_app_AppScreen hxc_l_tmp_call_result_n378 = hxc_caxecraft_app_AppScreen_recapture(hxc_l_screen);
      hxc_l_screen = hxc_l_tmp_call_result_n378;
      hxc_l_recapturedThisFrame = true;
      hxc_l_accumulator = 0.0;
      hxc_l_resetMotionThisFrame = true;
      DisableCursor();
    }
    bool hxc_l_tmp_call_result_n380 = hxc_caxecraft_app_AppScreen_showsTitle(hxc_l_screen);
    bool hxc_l_onTitle = hxc_l_tmp_call_result_n380;
    bool hxc_l_tmp_call_result_n382 = hxc_caxecraft_app_AppScreen_showsEditor(hxc_l_screen);
    bool hxc_l_onEditor = hxc_l_tmp_call_result_n382;
    bool hxc_l_tmp_call_result_n384 = hxc_caxecraft_app_AppScreen_pausesSimulation(hxc_l_screen);
    bool hxc_l_paused_h3e2c71f6993e = hxc_l_tmp_call_result_n384;
    bool hxc_l_tmp_call_result_n386 = hxc_caxecraft_app_AppScreen_capturesPointer(hxc_l_screen);
    bool hxc_l_captured_h113184fd3e4f = hxc_l_tmp_call_result_n386;
    int32_t hxc_l_editorNavigationCommand = 0;
    if (hxc_l_onEditor)
    {
      if (hxc_l_self == NULL)
      {
        abort();
      }
      struct hxc_caxecraft_input_NavigationRepeater *hxc_l_tmp_owned_class_field_address_n388 = &(*hxc_l_self).hxc_editorNavigation;
      if (hxc_l_tmp_owned_class_field_address_n388 == NULL)
      {
        abort();
      }
      struct hxc_caxecraft_input_NavigationSample hxc_l_tmp_call_result_n389 = hxc_caxecraft_app_RaylibNavigationInput_samplePrimaryGamepad();
      int32_t hxc_l_tmp_instance_call_result_n391 = hxc_caxecraft_input_NavigationRepeater_advance(hxc_l_tmp_owned_class_field_address_n388, hxc_l_tmp_call_result_n389, hxc_l_frameSeconds);
      hxc_l_editorNavigationCommand = hxc_l_tmp_instance_call_result_n391;
    }
    else
    {
      if (hxc_l_self == NULL)
      {
        abort();
      }
      struct hxc_caxecraft_input_NavigationRepeater *hxc_l_tmp_owned_class_field_address_n392 = &(*hxc_l_self).hxc_editorNavigation;
      if (hxc_l_tmp_owned_class_field_address_n392 == NULL)
      {
        abort();
      }
      hxc_caxecraft_input_NavigationRepeater_release(hxc_l_tmp_owned_class_field_address_n392);
    }
    if (hxc_l_captured_h113184fd3e4f)
    {
      double hxc_l_yawDelta = hxc_l_lookYaw_h657da7ce2b3a;
      if (hxc_l_yawDelta > 0.25)
      {
        hxc_l_yawDelta = 0.25;
      }
      if (hxc_l_yawDelta < -0.25)
      {
        hxc_l_yawDelta = -0.25;
      }
      double hxc_l_tmp_load_result_n397 = hxc_l_lookX;
      double hxc_l_tmp_load_result_n398 = hxc_l_yawDelta;
      double hxc_l_candidateX = hxc_l_tmp_load_result_n397 + hxc_l_tmp_load_result_n398 * hxc_l_lookZ;
      double hxc_l_tmp_load_result_n400 = hxc_l_lookZ;
      double hxc_l_tmp_load_result_n401 = hxc_l_yawDelta;
      double hxc_l_candidateZ = hxc_l_tmp_load_result_n400 - hxc_l_tmp_load_result_n401 * hxc_l_lookX;
      double hxc_l_tmp_load_result_n403 = hxc_l_candidateX;
      double hxc_l_tmp_load_result_n404 = hxc_l_candidateX;
      double hxc_l_tmp_load_result_n405 = hxc_l_candidateZ;
      double hxc_l_lengthSquared = hxc_l_tmp_load_result_n403 * hxc_l_tmp_load_result_n404 + hxc_l_tmp_load_result_n405 * hxc_l_candidateZ;
      double hxc_l_normalize = 1.5 - 0.5 * hxc_l_lengthSquared;
      double hxc_l_tmp_load_result_n408 = hxc_l_candidateX;
      hxc_l_lookX = hxc_l_tmp_load_result_n408 * hxc_l_normalize;
      double hxc_l_tmp_load_result_n410 = hxc_l_candidateZ;
      hxc_l_lookZ = hxc_l_tmp_load_result_n410 * hxc_l_normalize;
      double hxc_l_tmp_compound_load_result_n412 = hxc_l_lookY;
      hxc_l_lookY = hxc_l_tmp_compound_load_result_n412 + hxc_l_lookPitch_hb5cfe6bcd89b;
      if (hxc_l_lookY > 0.90)
      {
        hxc_l_lookY = 0.90;
      }
      if (hxc_l_lookY < -0.90)
      {
        hxc_l_lookY = -0.90;
      }
      if (hxc_l_jumpPressed_h3b66348ee5f1)
      {
        hxc_l_jumpQueued = true;
      }
    }
    bool hxc_l_tmp_load_result_n417 = hxc_l_captured_h113184fd3e4f;
    bool hxc_l_tmp_short_circuit_result_n187 = hxc_l_tmp_load_result_n417;
    if (hxc_l_tmp_load_result_n417)
    {
      hxc_l_tmp_short_circuit_result_n187 = !hxc_l_recapturedThisFrame;
    }
    bool hxc_l_tmp_short_circuit_load_result_n419 = hxc_l_tmp_short_circuit_result_n187;
    bool hxc_l_tmp_short_circuit_result_n188 = hxc_l_tmp_short_circuit_load_result_n419;
    if (hxc_l_tmp_short_circuit_load_result_n419)
    {
      hxc_l_tmp_short_circuit_result_n188 = hxc_l_primaryPressed_h01b630731264;
    }
    bool hxc_l_tmp_short_circuit_load_result_n421 = hxc_l_tmp_short_circuit_result_n188;
    bool hxc_l_tmp_short_circuit_result_n189 = hxc_l_tmp_short_circuit_load_result_n421;
    if (hxc_l_tmp_short_circuit_load_result_n421)
    {
      hxc_l_tmp_short_circuit_result_n189 = hxc_l_selectedMode == 1;
    }
    bool hxc_l_tmp_short_circuit_load_result_n423 = hxc_l_tmp_short_circuit_result_n189;
    bool hxc_l_tmp_short_circuit_result_n190 = hxc_l_tmp_short_circuit_load_result_n423;
    if (hxc_l_tmp_short_circuit_load_result_n423)
    {
      bool hxc_l_tmp_call_result_n426 = hxc_caxecraft_gameplay_Inventory_selectedIs(hxc_l_inventory, hxc_caxecraft_gameplay_ItemKind_CopperSword);
      hxc_l_tmp_short_circuit_result_n190 = hxc_l_tmp_call_result_n426;
    }
    if (hxc_l_tmp_short_circuit_result_n190)
    {
      hxc_l_swordQueued = true;
    }
    if (!hxc_l_paused_h3e2c71f6993e)
    {
      double hxc_l_tmp_compound_load_result_n429 = hxc_l_accumulator;
      hxc_l_accumulator = hxc_l_tmp_compound_load_result_n429 + hxc_l_frameSeconds;
    }
    while (1)
    {
      bool hxc_l_tmp_load_result_n431 = hxc_l_paused_h3e2c71f6993e;
      bool hxc_l_tmp_short_circuit_result_n191 = !hxc_l_tmp_load_result_n431;
      if (!hxc_l_tmp_load_result_n431)
      {
        hxc_l_tmp_short_circuit_result_n191 = hxc_l_accumulator >= 0.05;
      }
      if (!hxc_l_tmp_short_circuit_result_n191)
      {
        break;
      }
      double hxc_l_tmp_load_result_n434 = hxc_l_moveForward;
      double hxc_l_tmp_load_result_n435 = hxc_l_lookX;
      double hxc_l_tmp_load_result_n436 = hxc_l_moveRight;
      double hxc_l_moveX = hxc_l_tmp_load_result_n434 * hxc_l_tmp_load_result_n435 - hxc_l_tmp_load_result_n436 * hxc_l_lookZ;
      double hxc_l_tmp_load_result_n438 = hxc_l_moveForward;
      double hxc_l_tmp_load_result_n439 = hxc_l_lookZ;
      double hxc_l_tmp_load_result_n440 = hxc_l_moveRight;
      double hxc_l_moveZ = hxc_l_tmp_load_result_n438 * hxc_l_tmp_load_result_n439 + hxc_l_tmp_load_result_n440 * hxc_l_lookX;
      double hxc_l_tmp_load_result_n442 = hxc_l_moveForward;
      bool hxc_l_tmp_short_circuit_result_n194 = hxc_l_tmp_load_result_n442 != 0.0;
      if (hxc_l_tmp_load_result_n442 != 0.0)
      {
        hxc_l_tmp_short_circuit_result_n194 = hxc_l_moveRight != 0.0;
      }
      if (hxc_l_tmp_short_circuit_result_n194)
      {
        hxc_l_moveX = hxc_l_moveX * 0.7071067811865476;
        hxc_l_moveZ = hxc_l_moveZ * 0.7071067811865476;
      }
      int32_t hxc_l_tmp_load_result_n447 = hxc_l_selectedMode;
      int32_t hxc_l_tmp_conditional_result_n196 = 0;
      if (hxc_l_tmp_load_result_n447 == 1)
      {
        hxc_l_tmp_conditional_result_n196 = 1;
      }
      else
      {
        hxc_l_tmp_conditional_result_n196 = 0;
      }
      int32_t hxc_l_damagePolicy = hxc_l_tmp_conditional_result_n196;
      struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n449 = hxc_l_session;
      hxc_l_gc_roots[48] = (const void *)hxc_l_tmp_load_result_n449;
      if (hxc_l_tmp_load_result_n449 == NULL)
      {
        abort();
      }
      double hxc_l_tmp_load_result_n450 = hxc_l_moveX;
      double hxc_l_tmp_load_result_n451 = hxc_l_moveZ;
      bool hxc_l_tmp_load_result_n452 = hxc_l_jumpQueued;
      struct hxc_caxecraft_domain_CharacterIntent hxc_l_tmp_call_result_n454 = hxc_caxecraft_domain_Aquatics_input(hxc_l_tmp_load_result_n450, hxc_l_tmp_load_result_n451, hxc_l_tmp_load_result_n452, hxc_l_descendHeld);
      struct hxc_caxecraft_domain_GameTickResult hxc_l_tmp_instance_call_result_n457 = hxc_caxecraft_domain_GameSession_tick(hxc_l_tmp_load_result_n449, (struct hxc_caxecraft_domain_GameTickInput){ .hxc_damagePolicy = hxc_l_damagePolicy, .hxc_intent = hxc_l_tmp_call_result_n454, .hxc_waterUpdateBudget = 64 });
      struct hxc_caxecraft_domain_GameTickResult hxc_l_gameTick = hxc_l_tmp_instance_call_result_n457;
      hxc_l_character = hxc_l_gameTick.hxc_character;
      if (hxc_l_gameTick.hxc_committed)
      {
        struct hxc_caxecraft_app_MotionHistory hxc_l_tmp_load_result_n460 = hxc_l_motionHistory;
        struct hxc_caxecraft_app_MotionHistory hxc_l_tmp_call_result_n462 = hxc_caxecraft_app_MotionInterpolation_advance(hxc_l_tmp_load_result_n460, hxc_l_character.hxc_body);
        hxc_l_motionHistory = hxc_l_tmp_call_result_n462;
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
          int32_t hxc_l_tmp_load_result_n466 = hxc_l_pickupIndex;
          int32_t hxc_l_tmp_array_length_result_n468;
          if (hxc_array_ref_length(hxc_l_loadedItems, &hxc_l_tmp_array_length_result_n468) != HXC_STATUS_OK)
          {
            abort();
          }
          if (!(hxc_l_tmp_load_result_n466 < hxc_l_tmp_array_length_result_n468))
          {
            break;
          }
          struct hxc_array_ref *hxc_l_tmp_load_result_n469 = hxc_l_loadedItems;
          struct hxc_caxecraft_app_LoadedWorldItem hxc_l_tmp_array_get_result_n471;
          if (hxc_array_ref_get_copy(hxc_l_tmp_load_result_n469, (size_t)hxc_l_pickupIndex, &hxc_l_tmp_array_get_result_n471) != HXC_STATUS_OK)
          {
            abort();
          }
          struct hxc_caxecraft_app_LoadedWorldItem hxc_l_loadedItem = hxc_l_tmp_array_get_result_n471;
          struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n472 = hxc_l_session;
          hxc_l_gc_roots[49] = (const void *)hxc_l_tmp_load_result_n472;
          if (hxc_l_tmp_load_result_n472 == NULL)
          {
            abort();
          }
          bool hxc_l_tmp_instance_call_result_n474 = hxc_caxecraft_domain_GameSession_authoredItemIsActive(hxc_l_tmp_load_result_n472, hxc_l_pickupIndex);
          bool hxc_l_tmp_short_circuit_result_n200 = hxc_l_tmp_instance_call_result_n474;
          if (hxc_l_tmp_instance_call_result_n474)
          {
            double hxc_l_tmp_record_field_load_result_n475 = hxc_l_character.hxc_body.hxc_x;
            double hxc_l_tmp_record_field_load_result_n476 = hxc_l_character.hxc_body.hxc_y;
            double hxc_l_tmp_record_field_load_result_n477 = hxc_l_character.hxc_body.hxc_z;
            int32_t hxc_l_tmp_record_field_load_result_n478 = hxc_l_loadedItem.hxc_xMilli;
            int32_t hxc_l_tmp_record_field_load_result_n479 = hxc_l_loadedItem.hxc_yMilli;
            bool hxc_l_tmp_call_result_n481 = hxc_caxecraft_gameplay_WorldItemPickup_isInRange(hxc_l_tmp_record_field_load_result_n475, hxc_l_tmp_record_field_load_result_n476, hxc_l_tmp_record_field_load_result_n477, hxc_l_tmp_record_field_load_result_n478, hxc_l_tmp_record_field_load_result_n479, hxc_l_loadedItem.hxc_zMilli);
            hxc_l_tmp_short_circuit_result_n200 = hxc_l_tmp_call_result_n481;
          }
          if (hxc_l_tmp_short_circuit_result_n200)
          {
            int32_t hxc_l_itemCode = hxc_l_loadedItem.hxc_storageCode;
            int32_t hxc_l_tmp_call_result_n485 = hxc_caxecraft_content_BaseContentPack_itemFromValidatedStorageCode(hxc_l_itemCode);
            int32_t hxc_l_item = hxc_l_tmp_call_result_n485;
            int32_t hxc_l_tmp_call_result_n487 = hxc_caxecraft_content_BaseContentPack_itemUseProfile(hxc_l_item);
            bool hxc_l_tmp_short_circuit_result_n203 = hxc_l_tmp_call_result_n487 == 1;
            if (hxc_l_tmp_call_result_n487 == 1)
            {
              bool hxc_l_tmp_call_result_n489 = hxc_caxecraft_content_BaseContentPack_itemProvidesAquaticProfile(hxc_l_item);
              hxc_l_tmp_short_circuit_result_n203 = hxc_l_tmp_call_result_n489;
            }
            if (hxc_l_tmp_short_circuit_result_n203)
            {
              int32_t hxc_l_tmp_call_result_n492 = hxc_caxecraft_content_BaseContentPack_itemAquaticProfile(hxc_l_item);
              struct hxc_caxecraft_domain_AquaticProfile hxc_l_tmp_call_result_n493 = hxc_caxecraft_content_BaseContentPack_aquaticProfile(hxc_l_tmp_call_result_n492);
              struct hxc_caxecraft_domain_AquaticProfile hxc_l_replacement = hxc_l_tmp_call_result_n493;
              struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n494 = hxc_l_session;
              hxc_l_gc_roots[50] = (const void *)hxc_l_tmp_load_result_n494;
              if (hxc_l_tmp_load_result_n494 == NULL)
              {
                abort();
              }
              int32_t hxc_l_tmp_load_result_n495 = hxc_l_pickupIndex;
              struct hxc_caxecraft_domain_AuthoredAquaticEquipmentResult hxc_l_tmp_instance_call_result_n497 = hxc_caxecraft_domain_GameSession_collectAuthoredAquaticEquipment(hxc_l_tmp_load_result_n494, hxc_l_tmp_load_result_n495, hxc_l_replacement);
              struct hxc_caxecraft_domain_AuthoredAquaticEquipmentResult hxc_l_equipment = hxc_l_tmp_instance_call_result_n497;
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
          struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n505 = hxc_l_session;
          hxc_l_gc_roots[51] = (const void *)hxc_l_tmp_load_result_n505;
          if (hxc_l_tmp_load_result_n505 == NULL)
          {
            abort();
          }
          int32_t hxc_l_tmp_record_field_load_result_n506 = hxc_l_gameTick.hxc_tickIndex;
          struct hxc_caxecraft_domain_ActorControllerTickResult hxc_l_tmp_instance_call_result_n508 = hxc_caxecraft_domain_GameSession_stepAuthoredActorControllers(hxc_l_tmp_load_result_n505, hxc_l_tmp_record_field_load_result_n506, hxc_l_damagePolicy);
          struct hxc_caxecraft_domain_ActorControllerTickResult hxc_l_actorTick = hxc_l_tmp_instance_call_result_n508;
          switch (hxc_l_actorTick.hxc_status.hxc_tag) {
            case hxc_caxecraft_domain_ActorControllerTickStatus_ControllersAdvanced:
              {
                hxc_l_guideInteractionAvailable = false;
                int32_t hxc_l_g_h27d38ceae4c7 = 0;
                struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n510 = hxc_l_session;
                hxc_l_gc_roots[55] = (const void *)hxc_l_tmp_load_result_n510;
                if (hxc_l_tmp_load_result_n510 == NULL)
                {
                  abort();
                }
                struct hxc_array_ref *hxc_l_tmp_instance_call_result_n511 = hxc_caxecraft_domain_GameSession_actorControllerEventSnapshots(hxc_l_tmp_load_result_n510);
                hxc_l_g1_h0fb221650f0c = hxc_l_tmp_instance_call_result_n511;
                while (1)
                {
                  int32_t hxc_l_tmp_load_result_n512 = hxc_l_g_h27d38ceae4c7;
                  int32_t hxc_l_tmp_array_length_result_n514;
                  if (hxc_array_ref_length(hxc_l_g1_h0fb221650f0c, &hxc_l_tmp_array_length_result_n514) != HXC_STATUS_OK)
                  {
                    abort();
                  }
                  if (!(hxc_l_tmp_load_result_n512 < hxc_l_tmp_array_length_result_n514))
                  {
                    break;
                  }
                  struct hxc_array_ref *hxc_l_tmp_load_result_n515 = hxc_l_g1_h0fb221650f0c;
                  struct hxc_caxecraft_domain_ActorControllerEvent hxc_l_tmp_array_get_result_n517;
                  if (hxc_array_ref_get_copy(hxc_l_tmp_load_result_n515, (size_t)hxc_l_g_h27d38ceae4c7, &hxc_l_tmp_array_get_result_n517) != HXC_STATUS_OK)
                  {
                    abort();
                  }
                  hxc_l_event = hxc_l_tmp_array_get_result_n517;
                  if (hxc_enum_8b2a26b8_retain(&hxc_l_event) != HXC_STATUS_OK)
                  {
                    abort();
                  }
                  hxc_l_g_h27d38ceae4c7 = hxc_i32_add_wrapping(hxc_l_g_h27d38ceae4c7, 1);
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
                        int32_t hxc_l_tmp_enum_payload_project_n521 = hxc_l_event.hxc_payload.hxc_InteractionAvailable.hxc_characterId;
                        int32_t hxc_l_source_h6edef9172a7d = hxc_l_tmp_enum_payload_project_n521;
                        int32_t hxc_l_tmp_load_result_n522 = hxc_l_source_h6edef9172a7d;
                        if (hxc_l_tmp_load_result_n522 == hxc_l_dialogueActorId)
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
                        int32_t hxc_l_tmp_enum_payload_project_n525 = hxc_l_event.hxc_payload.hxc_LocalPlayerAttack.hxc_characterId;
                        int32_t hxc_l_source_h3dcf2067af8c = hxc_l_tmp_enum_payload_project_n525;
                        int32_t hxc_l_tmp_load_result_n526 = hxc_l_source_h3dcf2067af8c;
                        if (hxc_l_tmp_load_result_n526 == hxc_l_enemyActorId)
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
                        int32_t hxc_l_tmp_enum_payload_project_n529 = hxc_l_event.hxc_payload.hxc_DropRequested.hxc_characterId;
                        int32_t hxc_l_source_h1de633615c68 = hxc_l_tmp_enum_payload_project_n529;
                        if (hxc_l_event.hxc_tag != hxc_caxecraft_domain_ActorControllerEvent_DropRequested)
                        {
                          abort();
                        }
                        hxc_string hxc_l_tmp_enum_payload_project_n531 = hxc_l_event.hxc_payload.hxc_DropRequested.hxc_drop;
                        hxc_l_drop = hxc_l_tmp_enum_payload_project_n531;
                        if (hxc_string_retain(hxc_l_drop) != HXC_STATUS_OK)
                        {
                          abort();
                        }
                        int32_t hxc_l_tmp_call_result_n533 = hxc_caxecraft_content_BaseContentPack_dropQuantityById(hxc_l_drop);
                        int32_t hxc_l_quantity = hxc_l_tmp_call_result_n533;
                        struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n534 = hxc_l_session;
                        hxc_l_gc_roots[56] = (const void *)hxc_l_tmp_load_result_n534;
                        if (hxc_l_tmp_load_result_n534 == NULL)
                        {
                          abort();
                        }
                        struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n536 = hxc_caxecraft_domain_GameSession_readCharacter(hxc_l_tmp_load_result_n534, hxc_l_source_h1de633615c68);
                        struct hxc_caxecraft_domain_Character hxc_l_defeatedActor = hxc_l_tmp_instance_call_result_n536;
                        int32_t hxc_l_tmp_load_result_n537 = hxc_l_quantity;
                        bool hxc_l_tmp_short_circuit_result_n216 = hxc_l_tmp_load_result_n537 <= 0;
                        if (!(hxc_l_tmp_load_result_n537 <= 0))
                        {
                          hxc_l_tmp_short_circuit_result_n216 = hxc_l_defeatedActor.hxc_id <= 0;
                        }
                        if (hxc_l_tmp_short_circuit_result_n216)
                        {
                          hxc_l_quit = true;
                        }
                        else
                        {
                          if (!hxc_l_berryDrop.hxc_active)
                          {
                            struct hxc_caxecraft_domain_Character hxc_l_tmp_load_result_n541 = hxc_l_defeatedActor;
                            struct hxc_caxecraft_gameplay_BerryDropState hxc_l_tmp_call_result_n543 = hxc_caxecraft_gameplay_BerryDrop_fromDefeatedCharacter(hxc_l_tmp_load_result_n541, hxc_l_quantity);
                            hxc_l_berryDrop = hxc_l_tmp_call_result_n543;
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
                }
                if (hxc_array_ref_release(hxc_l_g1_h0fb221650f0c) != HXC_STATUS_OK)
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
        struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n544 = hxc_l_session;
        hxc_l_gc_roots[52] = (const void *)hxc_l_tmp_load_result_n544;
        if (hxc_l_tmp_load_result_n544 == NULL)
        {
          abort();
        }
        struct hxc_array_ref *hxc_l_tmp_instance_call_result_n545 = hxc_caxecraft_domain_GameSession_actorControllerStateSnapshots(hxc_l_tmp_load_result_n544);
        hxc_l_actorPhases = hxc_l_tmp_instance_call_result_n545;
        struct hxc_array_ref *hxc_l_tmp_load_result_n546 = hxc_l_actorPhases;
        struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation hxc_l_tmp_call_result_n549 = hxc_caxecraft_app_CaxecraftApp_observeActorPhase(hxc_l_tmp_load_result_n546, hxc_l_dialogueActorId, hxc_caxecraft_domain_ActorControllerPhase_Stationary);
        hxc_l_dialoguePhase = hxc_l_tmp_call_result_n549;
        struct hxc_array_ref *hxc_l_tmp_load_result_n550 = hxc_l_actorPhases;
        struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation hxc_l_tmp_call_result_n553 = hxc_caxecraft_app_CaxecraftApp_observeActorPhase(hxc_l_tmp_load_result_n550, hxc_l_enemyActorId, hxc_caxecraft_domain_ActorControllerPhase_Defeated);
        hxc_l_enemyPhase = hxc_l_tmp_call_result_n553;
        struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n554 = hxc_l_session;
        hxc_l_gc_roots[53] = (const void *)hxc_l_tmp_load_result_n554;
        if (hxc_l_tmp_load_result_n554 == NULL)
        {
          abort();
        }
        struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n556 = hxc_caxecraft_domain_GameSession_readCharacter(hxc_l_tmp_load_result_n554, hxc_l_dialogueActorId);
        hxc_l_dialogueActor = hxc_l_tmp_instance_call_result_n556;
        struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n557 = hxc_l_session;
        hxc_l_gc_roots[54] = (const void *)hxc_l_tmp_load_result_n557;
        if (hxc_l_tmp_load_result_n557 == NULL)
        {
          abort();
        }
        struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n559 = hxc_caxecraft_domain_GameSession_readCharacter(hxc_l_tmp_load_result_n557, hxc_l_enemyActorId);
        hxc_l_enemyActor = hxc_l_tmp_instance_call_result_n559;
        bool hxc_l_tmp_record_field_load_result_n560 = hxc_l_dialoguePhase.hxc_valid;
        bool hxc_l_tmp_short_circuit_result_n218 = !hxc_l_tmp_record_field_load_result_n560;
        if (!!hxc_l_tmp_record_field_load_result_n560)
        {
          hxc_l_tmp_short_circuit_result_n218 = !hxc_l_enemyPhase.hxc_valid;
        }
        bool hxc_l_tmp_short_circuit_load_result_n562 = hxc_l_tmp_short_circuit_result_n218;
        bool hxc_l_tmp_short_circuit_result_n219 = hxc_l_tmp_short_circuit_load_result_n562;
        if (!hxc_l_tmp_short_circuit_load_result_n562)
        {
          hxc_l_tmp_short_circuit_result_n219 = hxc_l_dialogueActor.hxc_id <= 0;
        }
        bool hxc_l_tmp_short_circuit_load_result_n564 = hxc_l_tmp_short_circuit_result_n219;
        bool hxc_l_tmp_short_circuit_result_n220 = hxc_l_tmp_short_circuit_load_result_n564;
        if (!hxc_l_tmp_short_circuit_load_result_n564)
        {
          hxc_l_tmp_short_circuit_result_n220 = hxc_l_enemyActor.hxc_id <= 0;
        }
        if (hxc_l_tmp_short_circuit_result_n220)
        {
          hxc_l_quit = true;
        }
        struct hxc_caxecraft_gameplay_SwordCombatState hxc_l_tmp_call_result_n568 = hxc_caxecraft_gameplay_SwordCombat_step(hxc_l_swordCombat);
        hxc_l_swordCombat = hxc_l_tmp_call_result_n568;
        if (hxc_l_swordQueued)
        {
          struct hxc_caxecraft_gameplay_SwordCombatState hxc_l_tmp_load_result_n570 = hxc_l_swordCombat;
          struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_load_result_n571 = hxc_l_inventory;
          struct hxc_caxecraft_domain_VitalsState hxc_l_tmp_record_field_load_result_n572 = hxc_l_character.hxc_vitals;
          struct hxc_caxecraft_domain_Character hxc_l_tmp_load_result_n573 = hxc_l_enemyActor;
          double hxc_l_tmp_record_field_load_result_n574 = hxc_l_character.hxc_body.hxc_x;
          double hxc_l_tmp_record_field_load_result_n575 = hxc_l_character.hxc_body.hxc_z;
          double hxc_l_tmp_load_result_n576 = hxc_l_lookX;
          int32_t hxc_l_tmp_call_result_n578 = hxc_caxecraft_gameplay_SwordCombat_decide(hxc_l_tmp_load_result_n570, hxc_l_tmp_load_result_n571, hxc_l_tmp_record_field_load_result_n572, hxc_l_tmp_load_result_n573, hxc_l_tmp_record_field_load_result_n574, hxc_l_tmp_record_field_load_result_n575, hxc_l_tmp_load_result_n576, hxc_l_lookZ);
          int32_t hxc_l_swordDecision = hxc_l_tmp_call_result_n578;
          if (hxc_l_swordDecision == 5)
          {
            struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n580 = hxc_l_session;
            hxc_l_gc_roots[57] = (const void *)hxc_l_tmp_load_result_n580;
            if (hxc_l_tmp_load_result_n580 == NULL)
            {
              abort();
            }
            struct hxc_caxecraft_domain_CharacterDamageResult hxc_l_tmp_instance_call_result_n582 = hxc_caxecraft_domain_GameSession_damageCharacter(hxc_l_tmp_load_result_n580, hxc_l_enemyActorId, 1);
            struct hxc_caxecraft_domain_CharacterDamageResult hxc_l_damage = hxc_l_tmp_instance_call_result_n582;
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
          int32_t hxc_l_tmp_load_result_n586 = hxc_l_swordDecision;
          struct hxc_caxecraft_gameplay_SwordCombatState hxc_l_tmp_call_result_n588 = hxc_caxecraft_gameplay_SwordCombat_after(hxc_l_tmp_load_result_n586, hxc_l_swordCombat);
          hxc_l_swordCombat = hxc_l_tmp_call_result_n588;
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
    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n593 = hxc_l_session;
    hxc_l_gc_roots[47] = (const void *)hxc_l_tmp_load_result_n593;
    if (hxc_l_tmp_load_result_n593 == NULL)
    {
      abort();
    }
    size_t hxc_l_tmp_length_n595;
    const uint8_t *hxc_l_tmp_instance_call_result_n594 = hxc_caxecraft_domain_GameSession_worldView(hxc_l_tmp_load_result_n593, &hxc_l_tmp_length_n595);
    double hxc_l_tmp_load_result_n596 = hxc_l_selectionEyeX;
    double hxc_l_tmp_load_result_n597 = hxc_l_selectionEyeY;
    double hxc_l_tmp_load_result_n598 = hxc_l_selectionEyeZ;
    double hxc_l_tmp_load_result_n599 = hxc_l_lookX;
    double hxc_l_tmp_load_result_n600 = hxc_l_lookY;
    struct hxc_caxecraft_domain_RaycastHit hxc_l_tmp_call_result_n602 = hxc_caxecraft_domain_VoxelRaycast_trace(hxc_l_tmp_instance_call_result_n594, hxc_l_tmp_length_n595, hxc_l_tmp_load_result_n596, hxc_l_tmp_load_result_n597, hxc_l_tmp_load_result_n598, hxc_l_tmp_load_result_n599, hxc_l_tmp_load_result_n600, hxc_l_lookZ, 7.0);
    struct hxc_caxecraft_domain_RaycastHit hxc_l_hit = hxc_l_tmp_call_result_n602;
    bool hxc_l_tmp_load_result_n603 = hxc_l_captured_h113184fd3e4f;
    bool hxc_l_tmp_short_circuit_result_n227 = hxc_l_tmp_load_result_n603;
    if (hxc_l_tmp_load_result_n603)
    {
      hxc_l_tmp_short_circuit_result_n227 = !hxc_l_recapturedThisFrame;
    }
    bool hxc_l_tmp_short_circuit_load_result_n605 = hxc_l_tmp_short_circuit_result_n227;
    bool hxc_l_tmp_short_circuit_result_n228 = hxc_l_tmp_short_circuit_load_result_n605;
    if (hxc_l_tmp_short_circuit_load_result_n605)
    {
      hxc_l_tmp_short_circuit_result_n228 = hxc_l_primaryPressed_h01b630731264;
    }
    if (hxc_l_tmp_short_circuit_result_n228)
    {
      if (hxc_l_character.hxc_vitals.hxc_health > 0)
      {
        if (hxc_l_selectedMode == 1)
        {
          bool hxc_l_tmp_call_result_n612 = hxc_caxecraft_gameplay_Inventory_selectedIs(hxc_l_inventory, hxc_caxecraft_gameplay_ItemKind_CopperSword);
          bool hxc_l_tmp_short_circuit_result_n229 = !hxc_l_tmp_call_result_n612;
          if (!hxc_l_tmp_call_result_n612)
          {
            hxc_l_tmp_short_circuit_result_n229 = hxc_l_hit.hxc_hit;
          }
          bool hxc_l_tmp_short_circuit_load_result_n614 = hxc_l_tmp_short_circuit_result_n229;
          bool hxc_l_tmp_short_circuit_result_n230 = hxc_l_tmp_short_circuit_load_result_n614;
          if (hxc_l_tmp_short_circuit_load_result_n614)
          {
            struct hxc_caxecraft_domain_AquaticState hxc_l_tmp_record_field_load_result_n615 = hxc_l_character.hxc_aquatic;
            bool hxc_l_tmp_call_result_n617 = hxc_caxecraft_domain_Aquatics_canMine(hxc_l_tmp_record_field_load_result_n615, hxc_l_character.hxc_aquaticProfile);
            hxc_l_tmp_short_circuit_result_n230 = hxc_l_tmp_call_result_n617;
          }
          if (hxc_l_tmp_short_circuit_result_n230)
          {
            int32_t hxc_l_tmp_record_field_load_result_n619 = hxc_l_hit.hxc_cellX;
            int32_t hxc_l_tmp_record_field_load_result_n620 = hxc_l_hit.hxc_cellY;
            struct hxc_caxecraft_scenario_VoxelPoint hxc_l_tmp_call_result_n622 = hxc_caxecraft_domain_World_coord(hxc_l_tmp_record_field_load_result_n619, hxc_l_tmp_record_field_load_result_n620, hxc_l_hit.hxc_cellZ);
            struct hxc_caxecraft_scenario_VoxelPoint hxc_l_minedCoordinate = hxc_l_tmp_call_result_n622;
            struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n623 = hxc_l_session;
            hxc_l_gc_roots[58] = (const void *)hxc_l_tmp_load_result_n623;
            if (hxc_l_tmp_load_result_n623 == NULL)
            {
              abort();
            }
            struct hxc_caxecraft_scenario_VoxelPoint hxc_l_tmp_load_result_n624 = hxc_l_minedCoordinate;
            struct hxc_caxecraft_gameplay_MiningResult hxc_l_tmp_instance_call_result_n626 = hxc_caxecraft_domain_GameSession_mineTerrain(hxc_l_tmp_load_result_n623, hxc_l_tmp_load_result_n624, hxc_l_inventory);
            struct hxc_caxecraft_gameplay_MiningResult hxc_l_mining = hxc_l_tmp_instance_call_result_n626;
            hxc_l_inventory = hxc_l_mining.hxc_inventory;
            if (hxc_l_mining.hxc_outcome == 2)
            {
              if (hxc_l_self == NULL)
              {
                abort();
              }
              struct hxc_caxecraft_app_TerrainRenderer *hxc_l_tmp_owned_class_field_address_n629 = &(*hxc_l_self).hxc_terrainRenderer;
              struct hxc_caxecraft_app_TerrainChunkCache *hxc_l_tmp_owned_class_field_address_n630 = &(*hxc_l_tmp_owned_class_field_address_n629).hxc_cache;
              if (hxc_l_tmp_owned_class_field_address_n630 == NULL)
              {
                abort();
              }
              hxc_caxecraft_app_TerrainChunkCache_invalidate(hxc_l_tmp_owned_class_field_address_n630, hxc_l_minedCoordinate);
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
            int32_t hxc_l_tmp_record_field_load_result_n634 = hxc_l_hit.hxc_cellX;
            int32_t hxc_l_tmp_record_field_load_result_n635 = hxc_l_hit.hxc_cellY;
            struct hxc_caxecraft_scenario_VoxelPoint hxc_l_tmp_call_result_n637 = hxc_caxecraft_domain_World_coord(hxc_l_tmp_record_field_load_result_n634, hxc_l_tmp_record_field_load_result_n635, hxc_l_hit.hxc_cellZ);
            struct hxc_caxecraft_scenario_VoxelPoint hxc_l_removedCoordinate = hxc_l_tmp_call_result_n637;
            struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n638 = hxc_l_session;
            hxc_l_gc_roots[59] = (const void *)hxc_l_tmp_load_result_n638;
            if (hxc_l_tmp_load_result_n638 == NULL)
            {
              abort();
            }
            bool hxc_l_tmp_instance_call_result_n640 = hxc_caxecraft_domain_GameSession_removeTerrain(hxc_l_tmp_load_result_n638, hxc_l_removedCoordinate);
            bool hxc_l_removed = hxc_l_tmp_instance_call_result_n640;
            if (hxc_l_removed)
            {
              if (hxc_l_self == NULL)
              {
                abort();
              }
              struct hxc_caxecraft_app_TerrainRenderer *hxc_l_tmp_owned_class_field_address_n642 = &(*hxc_l_self).hxc_terrainRenderer;
              struct hxc_caxecraft_app_TerrainChunkCache *hxc_l_tmp_owned_class_field_address_n643 = &(*hxc_l_tmp_owned_class_field_address_n642).hxc_cache;
              if (hxc_l_tmp_owned_class_field_address_n643 == NULL)
              {
                abort();
              }
              hxc_caxecraft_app_TerrainChunkCache_invalidate(hxc_l_tmp_owned_class_field_address_n643, hxc_l_removedCoordinate);
            }
          }
        }
      }
    }
    bool hxc_l_tmp_load_result_n645 = hxc_l_captured_h113184fd3e4f;
    bool hxc_l_tmp_short_circuit_result_n235 = hxc_l_tmp_load_result_n645;
    if (hxc_l_tmp_load_result_n645)
    {
      hxc_l_tmp_short_circuit_result_n235 = hxc_l_secondaryPressed_hed35213a6b62;
    }
    if (hxc_l_tmp_short_circuit_result_n235)
    {
      if (hxc_l_character.hxc_vitals.hxc_health > 0)
      {
        struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n649 = hxc_l_session;
        hxc_l_gc_roots[60] = (const void *)hxc_l_tmp_load_result_n649;
        if (hxc_l_tmp_load_result_n649 == NULL)
        {
          abort();
        }
        struct hxc_caxecraft_domain_LocalRecoveryResult hxc_l_tmp_instance_call_result_n651 = hxc_caxecraft_domain_GameSession_useSelectedRecovery(hxc_l_tmp_load_result_n649, hxc_l_inventory);
        struct hxc_caxecraft_domain_LocalRecoveryResult hxc_l_recovery = hxc_l_tmp_instance_call_result_n651;
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
              int32_t hxc_l_tmp_record_field_load_result_n658 = hxc_l_hit.hxc_previousX;
              int32_t hxc_l_tmp_record_field_load_result_n659 = hxc_l_hit.hxc_previousY;
              struct hxc_caxecraft_scenario_VoxelPoint hxc_l_tmp_call_result_n661 = hxc_caxecraft_domain_World_coord(hxc_l_tmp_record_field_load_result_n658, hxc_l_tmp_record_field_load_result_n659, hxc_l_hit.hxc_previousZ);
              struct hxc_caxecraft_scenario_VoxelPoint hxc_l_placement = hxc_l_tmp_call_result_n661;
              enum hxc_caxecraft_domain_BlockKind hxc_l_tmp_call_result_n663 = hxc_caxecraft_gameplay_Inventory_selectedBlock(hxc_l_inventory);
              enum hxc_caxecraft_domain_BlockKind hxc_l_selectedBlock = hxc_l_tmp_call_result_n663;
              struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_load_result_n664 = hxc_l_inventory;
              int32_t hxc_l_tmp_call_result_n666 = hxc_caxecraft_gameplay_Inventory_countAt(hxc_l_tmp_load_result_n664, hxc_l_inventory.hxc_selected);
              bool hxc_l_hasItem = hxc_l_tmp_call_result_n666 > 0;
              bool hxc_l_tmp_load_result_n667 = hxc_l_hasItem;
              bool hxc_l_tmp_short_circuit_result_n240 = !hxc_l_tmp_load_result_n667;
              if (!!hxc_l_tmp_load_result_n667)
              {
                bool hxc_l_tmp_call_result_n669 = hxc_caxecraft_domain_World_isPlaceable(hxc_l_selectedBlock);
                hxc_l_tmp_short_circuit_result_n240 = !hxc_l_tmp_call_result_n669;
              }
              bool hxc_l_tmp_short_circuit_load_result_n670 = hxc_l_tmp_short_circuit_result_n240;
              bool hxc_l_tmp_short_circuit_result_n241 = hxc_l_tmp_short_circuit_load_result_n670;
              if (!hxc_l_tmp_short_circuit_load_result_n670)
              {
                struct hxc_caxecraft_domain_CharacterBody hxc_l_tmp_record_field_load_result_n671 = hxc_l_character.hxc_body;
                bool hxc_l_tmp_call_result_n673 = hxc_caxecraft_domain_CharacterPhysics_canPlaceAt(hxc_l_tmp_record_field_load_result_n671, hxc_l_placement);
                hxc_l_tmp_short_circuit_result_n241 = !hxc_l_tmp_call_result_n673;
              }
              bool hxc_l_tmp_short_circuit_load_result_n674 = hxc_l_tmp_short_circuit_result_n241;
              bool hxc_l_tmp_short_circuit_result_n242 = hxc_l_tmp_short_circuit_load_result_n674;
              if (!hxc_l_tmp_short_circuit_load_result_n674)
              {
                struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n675 = hxc_l_session;
                hxc_l_gc_roots[61] = (const void *)hxc_l_tmp_load_result_n675;
                if (hxc_l_tmp_load_result_n675 == NULL)
                {
                  abort();
                }
                struct hxc_caxecraft_scenario_VoxelPoint hxc_l_tmp_load_result_n676 = hxc_l_placement;
                bool hxc_l_tmp_instance_call_result_n678 = hxc_caxecraft_domain_GameSession_placeTerrain(hxc_l_tmp_load_result_n675, hxc_l_tmp_load_result_n676, hxc_l_selectedBlock);
                hxc_l_tmp_short_circuit_result_n242 = !hxc_l_tmp_instance_call_result_n678;
              }
              if (hxc_l_tmp_short_circuit_result_n242)
              {
                hxc_l_placementBlockedFrames = 60;
              }
              else
              {
                if (hxc_l_self == NULL)
                {
                  abort();
                }
                struct hxc_caxecraft_app_TerrainRenderer *hxc_l_tmp_owned_class_field_address_n680 = &(*hxc_l_self).hxc_terrainRenderer;
                struct hxc_caxecraft_app_TerrainChunkCache *hxc_l_tmp_owned_class_field_address_n681 = &(*hxc_l_tmp_owned_class_field_address_n680).hxc_cache;
                if (hxc_l_tmp_owned_class_field_address_n681 == NULL)
                {
                  abort();
                }
                hxc_caxecraft_app_TerrainChunkCache_invalidate(hxc_l_tmp_owned_class_field_address_n681, hxc_l_placement);
                if (hxc_l_selectedMode == 1)
                {
                  struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n685 = hxc_caxecraft_gameplay_Inventory_consumeSelected(hxc_l_inventory);
                  hxc_l_inventory = hxc_l_tmp_call_result_n685;
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
    if (!hxc_l_paused_h3e2c71f6993e)
    {
      struct hxc_caxecraft_gameplay_BerryDropState hxc_l_tmp_load_result_n689 = hxc_l_berryDrop;
      double hxc_l_tmp_record_field_load_result_n690 = hxc_l_character.hxc_body.hxc_x;
      double hxc_l_tmp_record_field_load_result_n691 = hxc_l_character.hxc_body.hxc_y;
      bool hxc_l_tmp_call_result_n693 = hxc_caxecraft_gameplay_BerryDrop_isInRange(hxc_l_tmp_load_result_n689, hxc_l_tmp_record_field_load_result_n690, hxc_l_tmp_record_field_load_result_n691, hxc_l_character.hxc_body.hxc_z);
      if (hxc_l_tmp_call_result_n693)
      {
        struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_load_result_n694 = hxc_l_inventory;
        int32_t hxc_l_tmp_call_result_n697 = hxc_caxecraft_gameplay_Inventory_acceptedAmount(hxc_l_tmp_load_result_n694, hxc_caxecraft_gameplay_ItemKind_Berries, hxc_l_berryDrop.hxc_amount);
        int32_t hxc_l_acceptedDrop = hxc_l_tmp_call_result_n697;
        if (hxc_l_acceptedDrop > 0)
        {
          struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_load_result_n699 = hxc_l_inventory;
          struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n702 = hxc_caxecraft_gameplay_Inventory_collectItem(hxc_l_tmp_load_result_n699, hxc_caxecraft_gameplay_ItemKind_Berries, hxc_l_acceptedDrop);
          hxc_l_inventory = hxc_l_tmp_call_result_n702;
          struct hxc_caxecraft_gameplay_BerryDropState hxc_l_tmp_load_result_n703 = hxc_l_berryDrop;
          struct hxc_caxecraft_gameplay_BerryDropState hxc_l_tmp_call_result_n705 = hxc_caxecraft_gameplay_BerryDrop_collectAmount(hxc_l_tmp_load_result_n703, hxc_l_acceptedDrop);
          hxc_l_berryDrop = hxc_l_tmp_call_result_n705;
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
    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n722 = hxc_l_session;
    hxc_l_gc_roots[62] = (const void *)hxc_l_tmp_load_result_n722;
    if (hxc_l_tmp_load_result_n722 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_domain_GameView hxc_l_tmp_instance_call_result_n723 = hxc_caxecraft_domain_GameSession_view(hxc_l_tmp_load_result_n722);
    struct hxc_caxecraft_domain_GameView hxc_l_committedView = hxc_l_tmp_instance_call_result_n723;
    if (!hxc_l_committedView.hxc_valid)
    {
      hxc_l_quit = true;
    }
    else
    {
      hxc_l_character = hxc_l_committedView.hxc_localPlayer;
    }
    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n726 = hxc_l_session;
    hxc_l_gc_roots[63] = (const void *)hxc_l_tmp_load_result_n726;
    if (hxc_l_tmp_load_result_n726 == NULL)
    {
      abort();
    }
    bool hxc_l_tmp_instance_call_result_n728 = hxc_caxecraft_domain_GameSession_actorInteractionAvailable(hxc_l_tmp_load_result_n726, hxc_l_dialogueActorId);
    hxc_l_guideInteractionAvailable = hxc_l_tmp_instance_call_result_n728;
    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n729 = hxc_l_session;
    hxc_l_gc_roots[64] = (const void *)hxc_l_tmp_load_result_n729;
    if (hxc_l_tmp_load_result_n729 == NULL)
    {
      abort();
    }
    struct hxc_array_ref *hxc_l_tmp_instance_call_result_n730 = hxc_caxecraft_domain_GameSession_actorControllerStateSnapshots(hxc_l_tmp_load_result_n729);
    hxc_l_presentationActorPhases = hxc_l_tmp_instance_call_result_n730;
    struct hxc_array_ref *hxc_l_tmp_load_result_n731 = hxc_l_presentationActorPhases;
    struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation hxc_l_tmp_call_result_n734 = hxc_caxecraft_app_CaxecraftApp_observeActorPhase(hxc_l_tmp_load_result_n731, hxc_l_dialogueActorId, hxc_caxecraft_domain_ActorControllerPhase_Stationary);
    hxc_l_dialoguePhase = hxc_l_tmp_call_result_n734;
    struct hxc_array_ref *hxc_l_tmp_load_result_n735 = hxc_l_presentationActorPhases;
    struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation hxc_l_tmp_call_result_n738 = hxc_caxecraft_app_CaxecraftApp_observeActorPhase(hxc_l_tmp_load_result_n735, hxc_l_enemyActorId, hxc_caxecraft_domain_ActorControllerPhase_Defeated);
    hxc_l_enemyPhase = hxc_l_tmp_call_result_n738;
    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n739 = hxc_l_session;
    hxc_l_gc_roots[65] = (const void *)hxc_l_tmp_load_result_n739;
    if (hxc_l_tmp_load_result_n739 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n741 = hxc_caxecraft_domain_GameSession_readCharacter(hxc_l_tmp_load_result_n739, hxc_l_dialogueActorId);
    hxc_l_dialogueActor = hxc_l_tmp_instance_call_result_n741;
    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n742 = hxc_l_session;
    hxc_l_gc_roots[66] = (const void *)hxc_l_tmp_load_result_n742;
    if (hxc_l_tmp_load_result_n742 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n744 = hxc_caxecraft_domain_GameSession_readCharacter(hxc_l_tmp_load_result_n742, hxc_l_enemyActorId);
    hxc_l_enemyActor = hxc_l_tmp_instance_call_result_n744;
    bool hxc_l_tmp_record_field_load_result_n745 = hxc_l_dialoguePhase.hxc_valid;
    bool hxc_l_tmp_short_circuit_result_n246 = !hxc_l_tmp_record_field_load_result_n745;
    if (!!hxc_l_tmp_record_field_load_result_n745)
    {
      hxc_l_tmp_short_circuit_result_n246 = !hxc_l_enemyPhase.hxc_valid;
    }
    bool hxc_l_tmp_short_circuit_load_result_n747 = hxc_l_tmp_short_circuit_result_n246;
    bool hxc_l_tmp_short_circuit_result_n247 = hxc_l_tmp_short_circuit_load_result_n747;
    if (!hxc_l_tmp_short_circuit_load_result_n747)
    {
      hxc_l_tmp_short_circuit_result_n247 = hxc_l_dialogueActor.hxc_id <= 0;
    }
    bool hxc_l_tmp_short_circuit_load_result_n749 = hxc_l_tmp_short_circuit_result_n247;
    bool hxc_l_tmp_short_circuit_result_n248 = hxc_l_tmp_short_circuit_load_result_n749;
    if (!hxc_l_tmp_short_circuit_load_result_n749)
    {
      hxc_l_tmp_short_circuit_result_n248 = hxc_l_enemyActor.hxc_id <= 0;
    }
    if (hxc_l_tmp_short_circuit_result_n248)
    {
      hxc_l_quit = true;
    }
    int32_t hxc_l_completedTicks = hxc_l_committedView.hxc_completedTicks;
    if (hxc_l_resetMotionThisFrame)
    {
      struct hxc_caxecraft_app_MotionHistory hxc_l_tmp_call_result_n755 = hxc_caxecraft_app_MotionInterpolation_start(hxc_l_character.hxc_body);
      hxc_l_motionHistory = hxc_l_tmp_call_result_n755;
    }
    struct hxc_caxecraft_app_MotionHistory hxc_l_tmp_load_result_n756 = hxc_l_motionHistory;
    struct hxc_caxecraft_app_RenderPosition hxc_l_tmp_call_result_n758 = hxc_caxecraft_app_MotionInterpolation_sample(hxc_l_tmp_load_result_n756, hxc_l_accumulator, 0.05);
    struct hxc_caxecraft_app_RenderPosition hxc_l_renderPosition = hxc_l_tmp_call_result_n758;
    double hxc_l_eyeX = hxc_l_renderPosition.hxc_x;
    double hxc_l_eyeY = hxc_l_renderPosition.hxc_y + 1.62;
    double hxc_l_eyeZ = hxc_l_renderPosition.hxc_z;
    double hxc_l_tmp_load_result_n762 = hxc_l_eyeX;
    double hxc_l_tmp_load_result_n763 = hxc_l_eyeY;
    double hxc_l_tmp_load_result_n764 = hxc_l_eyeZ;
    double hxc_l_tmp_load_result_n766 = hxc_l_eyeX;
    double hxc_l_tmp_load_result_n767 = hxc_l_lookX;
    double hxc_l_tmp_load_result_n768 = hxc_l_eyeY;
    double hxc_l_tmp_load_result_n769 = hxc_l_lookY;
    double hxc_l_tmp_load_result_n770 = hxc_l_eyeZ;
    struct Camera3D hxc_l_camera = (struct Camera3D){ .position = (struct Vector3){ .x = (float)hxc_l_tmp_load_result_n762, .y = (float)hxc_l_tmp_load_result_n763, .z = (float)hxc_l_tmp_load_result_n764 }, .target = (struct Vector3){ .x = (float)(hxc_l_tmp_load_result_n766 + hxc_l_tmp_load_result_n767), .y = (float)(hxc_l_tmp_load_result_n768 + hxc_l_tmp_load_result_n769), .z = (float)(hxc_l_tmp_load_result_n770 + hxc_l_lookZ) }, .up = (struct Vector3){ .x = (float)0.0, .y = (float)1.0, .z = (float)0.0 }, .fovy = (float)70.0, .projection = 0 };
    BeginDrawing();
    if (hxc_l_onTitle)
    {
      int32_t hxc_l_tmp_native_call_result_n776 = GetScreenWidth();
      int32_t hxc_l_width_hc94ef0952017 = (int32_t)hxc_l_tmp_native_call_result_n776;
      int32_t hxc_l_tmp_native_call_result_n777 = GetScreenHeight();
      int32_t hxc_l_height_h247c63b87d12 = (int32_t)hxc_l_tmp_native_call_result_n777;
      if (hxc_l_titleTextureReady)
      {
        struct Color hxc_l_this1_h53c5fe42188c = (struct Color){ .r = 255, .g = 255, .b = 255, .a = 255 };
        struct Texture hxc_l_tmp_load_result_n780 = hxc_l_titleTexture;
        double hxc_l_imageAspect = hxc_f64_divide_zero_safe((double)(int32_t)hxc_l_tmp_load_result_n780.width, (double)(int32_t)hxc_l_titleTexture.height);
        int32_t hxc_l_tmp_load_result_n784 = hxc_l_width_hc94ef0952017;
        double hxc_l_screenAspect = hxc_f64_divide_zero_safe((double)hxc_l_tmp_load_result_n784, (double)hxc_l_height_h247c63b87d12);
        double hxc_l_sourceX = 0.0;
        double hxc_l_sourceY = 0.0;
        double hxc_l_sourceWidth = (double)(int32_t)hxc_l_titleTexture.width;
        double hxc_l_sourceHeight = (double)(int32_t)hxc_l_titleTexture.height;
        double hxc_l_tmp_load_result_n790 = hxc_l_screenAspect;
        if (hxc_l_tmp_load_result_n790 > hxc_l_imageAspect)
        {
          struct Texture hxc_l_tmp_load_result_n792 = hxc_l_titleTexture;
          hxc_l_sourceHeight = hxc_f64_divide_zero_safe((double)(int32_t)hxc_l_tmp_load_result_n792.width, hxc_l_screenAspect);
          struct Texture hxc_l_tmp_load_result_n795 = hxc_l_titleTexture;
          hxc_l_sourceY = ((double)(int32_t)hxc_l_tmp_load_result_n795.height - hxc_l_sourceHeight) * 0.5;
        }
        else
        {
          struct Texture hxc_l_tmp_load_result_n798 = hxc_l_titleTexture;
          hxc_l_sourceWidth = (double)(int32_t)hxc_l_tmp_load_result_n798.height * hxc_l_screenAspect;
          struct Texture hxc_l_tmp_load_result_n801 = hxc_l_titleTexture;
          hxc_l_sourceX = ((double)(int32_t)hxc_l_tmp_load_result_n801.width - hxc_l_sourceWidth) * 0.5;
        }
        struct Texture hxc_l_tmp_load_result_n804 = hxc_l_titleTexture;
        double hxc_l_tmp_load_result_n805 = hxc_l_sourceX;
        double hxc_l_tmp_load_result_n806 = hxc_l_sourceY;
        double hxc_l_tmp_load_result_n807 = hxc_l_sourceWidth;
        double hxc_l_tmp_load_result_n808 = hxc_l_sourceHeight;
        int32_t hxc_l_tmp_load_result_n810 = hxc_l_width_hc94ef0952017;
        int32_t hxc_l_tmp_load_result_n811 = hxc_l_height_h247c63b87d12;
        DrawTexturePro(hxc_l_tmp_load_result_n804, (struct Rectangle){ .x = (float)hxc_l_tmp_load_result_n805, .y = (float)hxc_l_tmp_load_result_n806, .width = (float)hxc_l_tmp_load_result_n807, .height = (float)hxc_l_tmp_load_result_n808 }, (struct Rectangle){ .x = (float)0.0, .y = (float)0.0, .width = (float)((double)hxc_l_tmp_load_result_n810 + 0.0), .height = (float)((double)hxc_l_tmp_load_result_n811 + 0.0) }, (struct Vector2){ .x = (float)0.0, .y = (float)0.0 }, (float)0.0, hxc_l_this1_h53c5fe42188c);
      }
      else
      {
        ClearBackground((struct Color){ .r = 126, .g = 190, .b = 201, .a = 255 });
      }
      struct Color hxc_l_this1_h1b0453abe07f = (struct Color){ .r = 4, .g = 14, .b = 23, .a = 132 };
      int32_t hxc_l_tmp_load_result_n817 = hxc_l_width_hc94ef0952017;
      int32_t hxc_l_tmp_load_result_n818 = hxc_l_height_h247c63b87d12;
      DrawRectangle((int32_t)0, (int32_t)0, (int32_t)hxc_l_tmp_load_result_n817, (int32_t)hxc_l_tmp_load_result_n818, hxc_l_this1_h1b0453abe07f);
      if (hxc_l_wordmarkTextureReady)
      {
        int32_t hxc_l_centerX = hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_l_width_hc94ef0952017, (double)2));
        struct Color hxc_l_this1_h7e17f9d9a961 = (struct Color){ .r = 255, .g = 255, .b = 255, .a = 255 };
        double hxc_l_scale = hxc_f64_divide_zero_safe((double)560, (double)(int32_t)hxc_l_wordmarkTexture.width);
        double hxc_l_heightScale = hxc_f64_divide_zero_safe((double)132, (double)(int32_t)hxc_l_wordmarkTexture.height);
        double hxc_l_tmp_load_result_n827 = hxc_l_heightScale;
        if (hxc_l_tmp_load_result_n827 < hxc_l_scale)
        {
          hxc_l_scale = hxc_l_heightScale;
        }
        struct Texture hxc_l_tmp_load_result_n830 = hxc_l_wordmarkTexture;
        double hxc_l_width_h36a1d6fedaae = (double)(int32_t)hxc_l_tmp_load_result_n830.width * hxc_l_scale;
        struct Texture hxc_l_tmp_load_result_n833 = hxc_l_wordmarkTexture;
        double hxc_l_height_h9c698903a295 = (double)(int32_t)hxc_l_tmp_load_result_n833.height * hxc_l_scale;
        double hxc_l_width1 = (double)(int32_t)hxc_l_wordmarkTexture.width + 0.0;
        double hxc_l_height1 = (double)(int32_t)hxc_l_wordmarkTexture.height + 0.0;
        struct Texture hxc_l_tmp_load_result_n840 = hxc_l_wordmarkTexture;
        double hxc_l_tmp_load_result_n841 = hxc_l_width1;
        double hxc_l_tmp_load_result_n842 = hxc_l_height1;
        int32_t hxc_l_tmp_load_result_n844 = hxc_l_centerX;
        double hxc_l_tmp_load_result_n845 = hxc_l_width_h36a1d6fedaae;
        double hxc_l_tmp_load_result_n846 = hxc_l_width_h36a1d6fedaae;
        double hxc_l_tmp_load_result_n847 = hxc_l_height_h9c698903a295;
        DrawTexturePro(hxc_l_tmp_load_result_n840, (struct Rectangle){ .x = (float)0.0, .y = (float)0.0, .width = (float)hxc_l_tmp_load_result_n841, .height = (float)hxc_l_tmp_load_result_n842 }, (struct Rectangle){ .x = (float)((double)hxc_l_tmp_load_result_n844 - hxc_l_tmp_load_result_n845 * 0.5), .y = (float)28.0, .width = (float)hxc_l_tmp_load_result_n846, .height = (float)hxc_l_tmp_load_result_n847 }, (struct Vector2){ .x = (float)0.0, .y = (float)0.0 }, (float)0.0, hxc_l_this1_h7e17f9d9a961);
      }
      else
      {
        int32_t hxc_l_x_hb9271ce35b4f = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_l_width_hc94ef0952017, (double)2)), 92);
        struct Color hxc_l_this1_h27ba42cd12d3 = (struct Color){ .r = 242, .g = 249, .b = 245, .a = 255 };
        const char *hxc_l_tmp_call_result_n854 = hxc_caxecraft_localization_UiCatalog_text(hxc_l_locale, 34);
        int32_t hxc_l_tmp_load_result_n855 = hxc_l_x_hb9271ce35b4f;
        DrawText(hxc_l_tmp_call_result_n854, (int32_t)hxc_l_tmp_load_result_n855, (int32_t)52, (int32_t)34, hxc_l_this1_h27ba42cd12d3);
      }
      int32_t hxc_l_firstTop = hxc_i32_subtract_wrapping(hxc_l_height_h247c63b87d12, 293);
      int32_t hxc_l_left_h7fe3cbdb76ef = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_l_width_hc94ef0952017, (double)2)), 180);
      if (hxc_l_selectedMode == 0)
      {
        struct Color hxc_l_this1_ha508043e0bbb = (struct Color){ .r = 16, .g = 88, .b = 102, .a = 232 };
        int32_t hxc_l_tmp_load_result_n861 = hxc_l_left_h7fe3cbdb76ef;
        int32_t hxc_l_tmp_load_result_n862 = hxc_l_firstTop;
        DrawRectangle((int32_t)hxc_l_tmp_load_result_n861, (int32_t)hxc_l_tmp_load_result_n862, (int32_t)360, (int32_t)54, hxc_l_this1_ha508043e0bbb);
        struct Color hxc_l_this1_h7a651cb0d11e = (struct Color){ .r = 255, .g = 190, .b = 55, .a = 255 };
        int32_t hxc_l_tmp_load_result_n865 = hxc_l_left_h7fe3cbdb76ef;
        int32_t hxc_l_tmp_load_result_n866 = hxc_l_firstTop;
        DrawRectangleLines((int32_t)hxc_l_tmp_load_result_n865, (int32_t)hxc_l_tmp_load_result_n866, (int32_t)360, (int32_t)54, hxc_l_this1_h7a651cb0d11e);
      }
      else
      {
        struct Color hxc_l_this1_h1fb0774a93f2 = (struct Color){ .r = 6, .g = 26, .b = 36, .a = 218 };
        int32_t hxc_l_tmp_load_result_n869 = hxc_l_left_h7fe3cbdb76ef;
        int32_t hxc_l_tmp_load_result_n870 = hxc_l_firstTop;
        DrawRectangle((int32_t)hxc_l_tmp_load_result_n869, (int32_t)hxc_l_tmp_load_result_n870, (int32_t)360, (int32_t)54, hxc_l_this1_h1fb0774a93f2);
        struct Color hxc_l_this1_h7a7a8360f29e = (struct Color){ .r = 92, .g = 194, .b = 188, .a = 255 };
        int32_t hxc_l_tmp_load_result_n873 = hxc_l_left_h7fe3cbdb76ef;
        int32_t hxc_l_tmp_load_result_n874 = hxc_l_firstTop;
        DrawRectangleLines((int32_t)hxc_l_tmp_load_result_n873, (int32_t)hxc_l_tmp_load_result_n874, (int32_t)360, (int32_t)54, hxc_l_this1_h7a7a8360f29e);
      }
      struct Color hxc_l_this1_h432df9a6585d = (struct Color){ .r = 242, .g = 249, .b = 245, .a = 255 };
      const char *hxc_l_tmp_call_result_n878 = hxc_caxecraft_localization_UiCatalog_text(hxc_l_locale, 27);
      int32_t hxc_l_tmp_load_result_n879 = hxc_l_left_h7fe3cbdb76ef;
      int32_t hxc_l_tmp_load_result_n880 = hxc_l_firstTop;
      DrawText(hxc_l_tmp_call_result_n878, (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n879, 28), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n880, 16), (int32_t)21, hxc_l_this1_h432df9a6585d);
      int32_t hxc_l_top_hd9512de22531 = hxc_i32_add_wrapping(hxc_i32_add_wrapping(hxc_l_firstTop, 54), 14);
      int32_t hxc_l_left_hdd5dba430571 = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_l_width_hc94ef0952017, (double)2)), 180);
      if (hxc_l_selectedMode == 1)
      {
        struct Color hxc_l_this1_hc051046c2c65 = (struct Color){ .r = 16, .g = 88, .b = 102, .a = 232 };
        int32_t hxc_l_tmp_load_result_n886 = hxc_l_left_hdd5dba430571;
        int32_t hxc_l_tmp_load_result_n887 = hxc_l_top_hd9512de22531;
        DrawRectangle((int32_t)hxc_l_tmp_load_result_n886, (int32_t)hxc_l_tmp_load_result_n887, (int32_t)360, (int32_t)54, hxc_l_this1_hc051046c2c65);
        struct Color hxc_l_this1_h0fb67bbe478e = (struct Color){ .r = 255, .g = 190, .b = 55, .a = 255 };
        int32_t hxc_l_tmp_load_result_n890 = hxc_l_left_hdd5dba430571;
        int32_t hxc_l_tmp_load_result_n891 = hxc_l_top_hd9512de22531;
        DrawRectangleLines((int32_t)hxc_l_tmp_load_result_n890, (int32_t)hxc_l_tmp_load_result_n891, (int32_t)360, (int32_t)54, hxc_l_this1_h0fb67bbe478e);
      }
      else
      {
        struct Color hxc_l_this1_hc8acdb0c5150 = (struct Color){ .r = 6, .g = 26, .b = 36, .a = 218 };
        int32_t hxc_l_tmp_load_result_n894 = hxc_l_left_hdd5dba430571;
        int32_t hxc_l_tmp_load_result_n895 = hxc_l_top_hd9512de22531;
        DrawRectangle((int32_t)hxc_l_tmp_load_result_n894, (int32_t)hxc_l_tmp_load_result_n895, (int32_t)360, (int32_t)54, hxc_l_this1_hc8acdb0c5150);
        struct Color hxc_l_this1_h0ea78289bc18 = (struct Color){ .r = 92, .g = 194, .b = 188, .a = 255 };
        int32_t hxc_l_tmp_load_result_n898 = hxc_l_left_hdd5dba430571;
        int32_t hxc_l_tmp_load_result_n899 = hxc_l_top_hd9512de22531;
        DrawRectangleLines((int32_t)hxc_l_tmp_load_result_n898, (int32_t)hxc_l_tmp_load_result_n899, (int32_t)360, (int32_t)54, hxc_l_this1_h0ea78289bc18);
      }
      struct Color hxc_l_this1_hf9d520b367bb = (struct Color){ .r = 242, .g = 249, .b = 245, .a = 255 };
      const char *hxc_l_tmp_call_result_n903 = hxc_caxecraft_localization_UiCatalog_text(hxc_l_locale, 26);
      int32_t hxc_l_tmp_load_result_n904 = hxc_l_left_hdd5dba430571;
      int32_t hxc_l_tmp_load_result_n905 = hxc_l_top_hd9512de22531;
      DrawText(hxc_l_tmp_call_result_n903, (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n904, 28), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n905, 16), (int32_t)21, hxc_l_this1_hf9d520b367bb);
      int32_t hxc_l_top_h99b5949fca76 = hxc_i32_add_wrapping(hxc_l_firstTop, 136);
      int32_t hxc_l_left_h9542a7455463 = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_l_width_hc94ef0952017, (double)2)), 180);
      struct Color hxc_l_this1_h6eae5be6b0c8 = (struct Color){ .r = 6, .g = 26, .b = 36, .a = 218 };
      int32_t hxc_l_tmp_load_result_n910 = hxc_l_left_h9542a7455463;
      int32_t hxc_l_tmp_load_result_n911 = hxc_l_top_h99b5949fca76;
      DrawRectangle((int32_t)hxc_l_tmp_load_result_n910, (int32_t)hxc_l_tmp_load_result_n911, (int32_t)360, (int32_t)54, hxc_l_this1_h6eae5be6b0c8);
      struct Color hxc_l_this1_h1b314e7b5d9d = (struct Color){ .r = 92, .g = 194, .b = 188, .a = 255 };
      int32_t hxc_l_tmp_load_result_n914 = hxc_l_left_h9542a7455463;
      int32_t hxc_l_tmp_load_result_n915 = hxc_l_top_h99b5949fca76;
      DrawRectangleLines((int32_t)hxc_l_tmp_load_result_n914, (int32_t)hxc_l_tmp_load_result_n915, (int32_t)360, (int32_t)54, hxc_l_this1_h1b314e7b5d9d);
      struct Color hxc_l_this1_he7d320013f59 = (struct Color){ .r = 242, .g = 249, .b = 245, .a = 255 };
      const char *hxc_l_tmp_call_result_n919 = hxc_caxecraft_localization_UiCatalog_text(hxc_l_locale, 28);
      int32_t hxc_l_tmp_load_result_n920 = hxc_l_left_h9542a7455463;
      int32_t hxc_l_tmp_load_result_n921 = hxc_l_top_h99b5949fca76;
      DrawText(hxc_l_tmp_call_result_n919, (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n920, 28), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n921, 16), (int32_t)21, hxc_l_this1_he7d320013f59);
      int32_t hxc_l_x_h011818dcac50 = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_l_width_hc94ef0952017, (double)2)), 285);
      struct Color hxc_l_this1_hc8279b6afe3b = (struct Color){ .r = 229, .g = 241, .b = 235, .a = 255 };
      const char *hxc_l_tmp_call_result_n926 = hxc_caxecraft_localization_UiCatalog_text(hxc_l_locale, 29);
      int32_t hxc_l_tmp_load_result_n927 = hxc_l_x_h011818dcac50;
      int32_t hxc_l_tmp_load_result_n928 = hxc_l_height_h247c63b87d12;
      DrawText(hxc_l_tmp_call_result_n926, (int32_t)hxc_l_tmp_load_result_n927, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n928, 58), (int32_t)16, hxc_l_this1_hc8279b6afe3b);
      if (hxc_l_selectedMode == 1)
      {
        int32_t hxc_l_x_hc3c94e3c9782 = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_l_width_hc94ef0952017, (double)2)), 230);
        struct Color hxc_l_this1_h1986656be165 = (struct Color){ .r = 255, .g = 205, .b = 91, .a = 255 };
        const char *hxc_l_tmp_call_result_n934 = hxc_caxecraft_localization_FirstPlayableCatalog_text(hxc_l_locale, 1);
        int32_t hxc_l_tmp_load_result_n935 = hxc_l_x_hc3c94e3c9782;
        int32_t hxc_l_tmp_load_result_n936 = hxc_l_firstTop;
        DrawText(hxc_l_tmp_call_result_n934, (int32_t)hxc_l_tmp_load_result_n935, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n936, 36), (int32_t)17, hxc_l_this1_h1986656be165);
      }
    }
    else
    {
      if (hxc_l_onEditor)
      {
        if (hxc_l_self == NULL)
        {
          abort();
        }
        struct hxc_caxecraft_app_CaxecraftEditorScreen *hxc_l_tmp_class_field_load_result_n939 = (*hxc_l_self).hxc_editorScreen;
        hxc_l_gc_roots[67] = (const void *)hxc_l_tmp_class_field_load_result_n939;
        if (hxc_l_tmp_class_field_load_result_n939 == NULL)
        {
          abort();
        }
        int32_t hxc_l_tmp_load_result_n940 = hxc_l_locale;
        enum hxc_caxecraft_app_EditorScreenAction hxc_l_tmp_instance_call_result_n942 = hxc_caxecraft_app_CaxecraftEditorScreen_draw(hxc_l_tmp_class_field_load_result_n939, hxc_l_tmp_load_result_n940, hxc_l_editorNavigationCommand);
        if (hxc_l_tmp_instance_call_result_n942 == hxc_caxecraft_app_EditorScreenAction_ReturnToTitle)
        {
          enum hxc_caxecraft_app_AppScreen hxc_l_tmp_call_result_n945 = hxc_caxecraft_app_AppScreen_closeEditor(hxc_l_screen);
          hxc_l_screen = hxc_l_tmp_call_result_n945;
        }
      }
      else
      {
        ClearBackground((struct Color){ .r = 126, .g = 190, .b = 201, .a = 255 });
        int32_t hxc_l_tmp_native_call_result_n947 = GetScreenWidth();
        int32_t hxc_l_sunX = hxc_i32_subtract_wrapping((int32_t)hxc_l_tmp_native_call_result_n947, 260);
        float hxc_l_radius_h8201ffcfaee8 = (float)42.0;
        struct Color hxc_l_this1_h9fa907e931d7 = (struct Color){ .r = 255, .g = 226, .b = 136, .a = 255 };
        int32_t hxc_l_tmp_load_result_n949 = hxc_l_sunX;
        float hxc_l_tmp_load_result_n950 = hxc_l_radius_h8201ffcfaee8;
        DrawCircle((int32_t)hxc_l_tmp_load_result_n949, (int32_t)86, hxc_l_tmp_load_result_n950, hxc_l_this1_h9fa907e931d7);
        float hxc_l_radius_h9b4b7569fa70 = (float)30.0;
        struct Color hxc_l_this1_h6cfd7f210e8b = (struct Color){ .r = 255, .g = 246, .b = 205, .a = 255 };
        int32_t hxc_l_tmp_load_result_n953 = hxc_l_sunX;
        float hxc_l_tmp_load_result_n954 = hxc_l_radius_h9b4b7569fa70;
        DrawCircle((int32_t)hxc_l_tmp_load_result_n953, (int32_t)86, hxc_l_tmp_load_result_n954, hxc_l_this1_h6cfd7f210e8b);
        BeginMode3D(hxc_l_camera);
        if (hxc_l_self == NULL)
        {
          abort();
        }
        struct hxc_caxecraft_app_TerrainRenderer *hxc_l_tmp_owned_class_field_address_n957 = &(*hxc_l_self).hxc_terrainRenderer;
        if (hxc_l_tmp_owned_class_field_address_n957 == NULL)
        {
          abort();
        }
        struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n958 = hxc_l_session;
        hxc_l_gc_roots[68] = (const void *)hxc_l_tmp_load_result_n958;
        if (hxc_l_tmp_load_result_n958 == NULL)
        {
          abort();
        }
        size_t hxc_l_tmp_length_n960;
        const uint8_t *hxc_l_tmp_instance_call_result_n959 = hxc_caxecraft_domain_GameSession_worldView(hxc_l_tmp_load_result_n958, &hxc_l_tmp_length_n960);
        struct Texture hxc_l_tmp_load_result_n961 = hxc_l_terrainTexture;
        bool hxc_l_tmp_load_result_n962 = hxc_l_terrainTextureReady;
        struct Texture hxc_l_tmp_load_result_n963 = hxc_l_adventureTerrainTexture;
        bool hxc_l_tmp_load_result_n964 = hxc_l_adventureTerrainTextureReady;
        double hxc_l_tmp_record_field_load_result_n965 = hxc_l_renderPosition.hxc_x;
        struct hxc_caxecraft_app_TerrainRenderCounters hxc_l_tmp_instance_call_result_n967 = hxc_caxecraft_app_TerrainRenderer_draw(hxc_l_tmp_owned_class_field_address_n957, hxc_l_tmp_instance_call_result_n959, hxc_l_tmp_length_n960, hxc_l_tmp_load_result_n961, hxc_l_tmp_load_result_n962, hxc_l_tmp_load_result_n963, hxc_l_tmp_load_result_n964, hxc_l_tmp_record_field_load_result_n965, hxc_l_renderPosition.hxc_z);
        struct hxc_caxecraft_app_TerrainRenderCounters hxc_l_renderCounters = hxc_l_tmp_instance_call_result_n967;
        struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n968 = hxc_l_session;
        hxc_l_gc_roots[69] = (const void *)hxc_l_tmp_load_result_n968;
        if (hxc_l_tmp_load_result_n968 == NULL)
        {
          abort();
        }
        size_t hxc_l_tmp_length_n970;
        const uint8_t *hxc_l_tmp_instance_call_result_n969 = hxc_caxecraft_domain_GameSession_worldView(hxc_l_tmp_load_result_n968, &hxc_l_tmp_length_n970);
        struct Texture hxc_l_tmp_load_result_n971 = hxc_l_terrainTexture;
        bool hxc_l_tmp_load_result_n972 = hxc_l_terrainTextureReady;
        struct hxc_caxecraft_app_WaterRenderCounters hxc_l_tmp_call_result_n974 = hxc_caxecraft_app_WaterRenderer_draw(hxc_l_tmp_instance_call_result_n969, hxc_l_tmp_length_n970, hxc_l_tmp_load_result_n971, hxc_l_tmp_load_result_n972, hxc_l_waterPresentationCell);
        struct hxc_caxecraft_app_WaterRenderCounters hxc_l_waterCounters = hxc_l_tmp_call_result_n974;
        int32_t hxc_l_tmp_record_field_load_result_n975 = hxc_l_renderCounters.hxc_visible;
        int32_t hxc_l_totalVisible = hxc_i32_add_wrapping(hxc_l_tmp_record_field_load_result_n975, hxc_l_waterCounters.hxc_visible);
        int32_t hxc_l_tmp_record_field_load_result_n977 = hxc_l_renderCounters.hxc_drawCalls;
        int32_t hxc_l_totalDrawCalls = hxc_i32_add_wrapping(hxc_l_tmp_record_field_load_result_n977, hxc_l_waterCounters.hxc_drawCalls);
        struct Camera3D hxc_l_tmp_load_result_n979 = hxc_l_camera;
        struct Texture hxc_l_tmp_load_result_n980 = hxc_l_entityTexture;
        bool hxc_l_tmp_load_result_n981 = hxc_l_entityTextureReady;
        struct hxc_caxecraft_domain_Character hxc_l_tmp_load_result_n982 = hxc_l_dialogueActor;
        struct hxc_caxecraft_domain_Character hxc_l_tmp_load_result_n983 = hxc_l_enemyActor;
        enum hxc_caxecraft_domain_ActorControllerPhase hxc_l_tmp_record_field_load_result_n984 = hxc_l_enemyPhase.hxc_phase;
        hxc_caxecraft_app_CaxecraftApp_drawActors(hxc_l_tmp_load_result_n979, hxc_l_tmp_load_result_n980, hxc_l_tmp_load_result_n981, hxc_l_tmp_load_result_n982, hxc_l_tmp_load_result_n983, hxc_l_tmp_record_field_load_result_n984, hxc_l_berryDrop);
        struct Camera3D hxc_l_tmp_load_result_n986 = hxc_l_camera;
        struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n987 = hxc_l_session;
        hxc_l_gc_roots[70] = (const void *)hxc_l_tmp_load_result_n987;
        if (hxc_l_tmp_load_result_n987 == NULL)
        {
          abort();
        }
        size_t hxc_l_tmp_length_n989;
        const int32_t *hxc_l_tmp_instance_call_result_n988 = hxc_caxecraft_domain_GameSession_authoredItemsView(hxc_l_tmp_load_result_n987, &hxc_l_tmp_length_n989);
        struct hxc_array_ref *hxc_l_tmp_load_result_n990 = hxc_l_loadedItems;
        struct Texture hxc_l_tmp_load_result_n991 = hxc_l_itemTexture;
        bool hxc_l_tmp_load_result_n992 = hxc_l_itemTextureReady;
        struct Texture hxc_l_tmp_load_result_n993 = hxc_l_adventureItemTexture;
        hxc_caxecraft_app_AuthoredItemRenderer_drawWorldItems(hxc_l_tmp_load_result_n986, hxc_l_tmp_instance_call_result_n988, hxc_l_tmp_length_n989, hxc_l_tmp_load_result_n990, hxc_l_tmp_load_result_n991, hxc_l_tmp_load_result_n992, hxc_l_tmp_load_result_n993, hxc_l_adventureItemTextureReady);
        if (hxc_l_hit.hxc_hit)
        {
          double hxc_l_y = (double)hxc_l_hit.hxc_cellY + 0.5;
          double hxc_l_z = (double)hxc_l_hit.hxc_cellZ + 0.5;
          int32_t hxc_l_tmp_record_field_load_result_n998 = hxc_l_hit.hxc_cellX;
          double hxc_l_tmp_load_result_n999 = hxc_l_y;
          DrawCubeWires((struct Vector3){ .x = (float)((double)hxc_l_tmp_record_field_load_result_n998 + 0.5), .y = (float)hxc_l_tmp_load_result_n999, .z = (float)hxc_l_z }, (float)1.04, (float)1.04, (float)1.04, (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 });
        }
        EndMode3D();
        if (hxc_l_cameraWaterBlend > 0.0)
        {
          int32_t hxc_l_overlayAlpha = hxc_f64_to_i32_saturating(105.0 * hxc_l_cameraWaterBlend);
          int32_t hxc_l_tmp_native_call_result_n1005 = GetScreenWidth();
          int32_t hxc_l_width_h924a669cc44a = (int32_t)hxc_l_tmp_native_call_result_n1005;
          int32_t hxc_l_tmp_native_call_result_n1006 = GetScreenHeight();
          int32_t hxc_l_height_he389f44b7921 = (int32_t)hxc_l_tmp_native_call_result_n1006;
          uint8_t hxc_l_red = (uint8_t)31;
          uint8_t hxc_l_green = (uint8_t)115;
          uint8_t hxc_l_blue = (uint8_t)154;
          int32_t hxc_l_tmp_load_result_n1007 = hxc_l_overlayAlpha;
          int32_t hxc_l_tmp_conditional_result_n316 = 0;
          if (hxc_l_tmp_load_result_n1007 < 0)
          {
            hxc_l_tmp_conditional_result_n316 = 0;
          }
          else
          {
            int32_t hxc_l_tmp_load_result_n1008 = hxc_l_overlayAlpha;
            int32_t hxc_l_tmp_conditional_result_n317 = 0;
            if (hxc_l_tmp_load_result_n1008 > 255)
            {
              hxc_l_tmp_conditional_result_n317 = 255;
            }
            else
            {
              hxc_l_tmp_conditional_result_n317 = hxc_l_overlayAlpha;
            }
            hxc_l_tmp_conditional_result_n316 = hxc_l_tmp_conditional_result_n317;
          }
          uint8_t hxc_l_alpha = (uint8_t)hxc_l_tmp_conditional_result_n316;
          uint8_t hxc_l_tmp_load_result_n1012 = hxc_l_red;
          uint8_t hxc_l_tmp_load_result_n1013 = hxc_l_green;
          uint8_t hxc_l_tmp_load_result_n1014 = hxc_l_blue;
          struct Color hxc_l_this1_hc275f0415f1c = (struct Color){ .r = hxc_l_tmp_load_result_n1012, .g = hxc_l_tmp_load_result_n1013, .b = hxc_l_tmp_load_result_n1014, .a = hxc_l_alpha };
          int32_t hxc_l_tmp_load_result_n1017 = hxc_l_width_h924a669cc44a;
          int32_t hxc_l_tmp_load_result_n1018 = hxc_l_height_he389f44b7921;
          DrawRectangle((int32_t)0, (int32_t)0, (int32_t)hxc_l_tmp_load_result_n1017, (int32_t)hxc_l_tmp_load_result_n1018, hxc_l_this1_hc275f0415f1c);
        }
        int32_t hxc_l_tmp_load_result_n1020 = hxc_l_totalVisible;
        int32_t hxc_l_tmp_load_result_n1021 = hxc_l_totalDrawCalls;
        int32_t hxc_l_tmp_load_result_n1022 = hxc_l_frameCount;
        int32_t hxc_l_tmp_load_result_n1023 = hxc_l_completedTicks;
        double hxc_l_tmp_record_field_load_result_n1025 = hxc_l_character.hxc_body.hxc_x;
        double hxc_l_tmp_record_field_load_result_n1026 = hxc_l_character.hxc_body.hxc_z;
        struct hxc_caxecraft_domain_VitalsState hxc_l_tmp_record_field_load_result_n1027 = hxc_l_character.hxc_vitals;
        int32_t hxc_l_tmp_load_result_n1028 = hxc_l_aquaticEquipmentCode;
        int32_t hxc_l_tmp_load_result_n1029 = hxc_l_aquaticEquipmentFrames;
        bool hxc_l_tmp_record_field_load_result_n1030 = hxc_l_character.hxc_aquatic.hxc_headSubmerged;
        int32_t hxc_l_tmp_record_field_load_result_n1031 = hxc_l_character.hxc_aquatic.hxc_breathTicks;
        int32_t hxc_l_tmp_record_field_load_result_n1032 = hxc_l_character.hxc_aquaticProfile.hxc_maximumBreathTicks;
        int32_t hxc_l_tmp_load_result_n1034 = hxc_l_placementBlockedFrames;
        int32_t hxc_l_tmp_load_result_n1035 = hxc_l_strikeHitFrames;
        int32_t hxc_l_tmp_load_result_n1036 = hxc_l_enemyDefeatedFrames;
        int32_t hxc_l_tmp_load_result_n1037 = hxc_l_enemyAttackFrames;
        int32_t hxc_l_tmp_load_result_n1038 = hxc_l_pickupFrames;
        int32_t hxc_l_tmp_load_result_n1039 = hxc_l_pickupAmount;
        int32_t hxc_l_tmp_load_result_n1040 = hxc_l_inventoryFullReason;
        int32_t hxc_l_tmp_load_result_n1041 = hxc_l_recoveryFeedback;
        int32_t hxc_l_tmp_load_result_n1042 = hxc_l_recoveryFeedbackFrames;
        bool hxc_l_tmp_load_result_n1044 = hxc_l_paused_h3e2c71f6993e;
        bool hxc_l_tmp_load_result_n1045 = hxc_l_captured_h113184fd3e4f;
        struct hxc_caxecraft_domain_RaycastHit hxc_l_tmp_load_result_n1046 = hxc_l_hit;
        int32_t hxc_l_tmp_load_result_n1047 = hxc_l_selectedMode;
        int32_t hxc_l_tmp_load_result_n1048 = hxc_l_locale;
        struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_load_result_n1049 = hxc_l_inventory;
        enum hxc_caxecraft_gameplay_GuidePhase hxc_l_tmp_load_result_n1050 = hxc_l_guidePhase;
        bool hxc_l_tmp_load_result_n1051 = hxc_l_guideInteractionAvailable;
        struct hxc_caxecraft_domain_Character hxc_l_tmp_load_result_n1052 = hxc_l_enemyActor;
        struct hxc_caxecraft_app_HudView hxc_l_hudView = (struct hxc_caxecraft_app_HudView){ .hxc_character = (struct hxc_caxecraft_app_HudCharacterView){ .hxc_aquaticEquipmentCode = hxc_l_tmp_load_result_n1028, .hxc_aquaticEquipmentVisible = hxc_l_tmp_load_result_n1029 > 0, .hxc_breathTicks = hxc_l_tmp_record_field_load_result_n1031, .hxc_headSubmerged = hxc_l_tmp_record_field_load_result_n1030, .hxc_maximumBreathTicks = hxc_l_tmp_record_field_load_result_n1032, .hxc_vitals = hxc_l_tmp_record_field_load_result_n1027, .hxc_x = hxc_l_tmp_record_field_load_result_n1025, .hxc_z = hxc_l_tmp_record_field_load_result_n1026 }, .hxc_enemy = hxc_l_tmp_load_result_n1052, .hxc_enemyPhase = hxc_l_enemyPhase.hxc_phase, .hxc_feedback = (struct hxc_caxecraft_app_HudFeedbackView){ .hxc_enemyAttacked = hxc_l_tmp_load_result_n1037 > 0, .hxc_enemyDefeated = hxc_l_tmp_load_result_n1036 > 0, .hxc_inventoryFullReason = hxc_l_tmp_load_result_n1040, .hxc_pickedUp = hxc_l_tmp_load_result_n1038 > 0, .hxc_pickupAmount = hxc_l_tmp_load_result_n1039, .hxc_placementBlocked = hxc_l_tmp_load_result_n1034 > 0, .hxc_recoveryDecision = hxc_l_tmp_load_result_n1041, .hxc_recoveryVisible = hxc_l_tmp_load_result_n1042 > 0, .hxc_strikeHit = hxc_l_tmp_load_result_n1035 > 0 }, .hxc_guideInteractionAvailable = hxc_l_tmp_load_result_n1051, .hxc_guidePhase = hxc_l_tmp_load_result_n1050, .hxc_hit = hxc_l_tmp_load_result_n1046, .hxc_inventory = hxc_l_tmp_load_result_n1049, .hxc_locale = hxc_l_tmp_load_result_n1048, .hxc_metrics = (struct hxc_caxecraft_app_HudMetricsView){ .hxc_completedTicks = hxc_l_tmp_load_result_n1023, .hxc_drawCalls = hxc_l_tmp_load_result_n1021, .hxc_renderedFrames = hxc_l_tmp_load_result_n1022, .hxc_visibleBlocks = hxc_l_tmp_load_result_n1020 }, .hxc_mode = hxc_l_tmp_load_result_n1047, .hxc_paused = hxc_l_tmp_load_result_n1044, .hxc_pointerCaptured = hxc_l_tmp_load_result_n1045 };
        struct hxc_caxecraft_app_HudView hxc_l_tmp_load_result_n1055 = hxc_l_hudView;
        hxc_caxecraft_app_CaxecraftApp_drawHud(hxc_l_tmp_load_result_n1055, hxc_l_hudResources);
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
  hxc_array_1f383c02_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n28);
  if (hxc_array_ref_release(hxc_l_fluidPresentation) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_record_2bd8b029_destroy(&hxc_l_tmp_record_field_transform_receiver_owner_n25);
  if (hxc_array_ref_release(hxc_l_g1_hcad3014c33f5) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_ref_release(hxc_l_loadedItems) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_ref_release(hxc_l_g1_h881bba9116a1) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_ref_release(hxc_l_actorBindings) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_enum_be0a6f10_destroy(&hxc_l_g_h4ef171f4fe67);
  hxc_enum_21bc9bf3_destroy(&hxc_l_tmp_static_call_argument_0_owner_n8);
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return;
}
