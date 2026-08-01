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
  const void *hxc_l_gc_roots[75] = { (const void *)hxc_l_self, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 75, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_actorBindings = { 0 };
  struct hxc_array_ref *hxc_l_g1_hb98ea081892b = { 0 };
  struct hxc_caxecraft_content_LoadedActorBinding hxc_l_binding_hc1b57b45b4a2 = { 0 };
  struct hxc_caxecraft_content_LoadedActorBinding hxc_l_tmp_array_element_owner_n17 = { 0 };
  struct hxc_caxecraft_app_CaxecraftApp_PlayableActorBinding hxc_l_tmp_array_push_element_owner_n19 = { 0 };
  struct hxc_array_ref *hxc_l_loadedItems = { 0 };
  struct hxc_array_ref *hxc_l_g1_hecc87e72e1d5 = { 0 };
  struct hxc_caxecraft_domain_ActorControllerEvent hxc_l_event = { 0 };
  struct hxc_caxecraft_domain_ActorControllerEvent hxc_l_tmp_array_element_owner_n211 = { 0 };
  hxc_string hxc_l_drop = { 0 };
  struct hxc_array_ref *hxc_l_actorPhases = { 0 };
  struct hxc_array_ref *hxc_l_g1_h16b7ad605ace = { 0 };
  struct hxc_caxecraft_content_ResolvedItem hxc_l_binding_h5388213540e0 = { 0 };
  struct hxc_caxecraft_content_ResolvedItem hxc_l_tmp_array_element_owner_n24 = { 0 };
  struct hxc_array_ref *hxc_l_presentationActorPhases = { 0 };
  struct hxc_caxecraft_content_ResolvedPlayer hxc_l_tmp_record_field_transform_receiver_owner_n26 = { 0 };
  struct hxc_array_ref *hxc_l_fluidPresentation = { 0 };
  struct hxc_caxecraft_content_ResolvedFluidPresentation hxc_l_tmp_array_element_owner_n29 = { 0 };
  struct hxc_caxecraft_content_ResolvedFluidPresentation hxc_l_request = { 0 };
  struct hxc_caxecraft_content_ResolvedFluidPresentation hxc_l_tmp_array_element_owner_n32 = { 0 };
  struct hxc_caxecraft_content_RuntimeLevelReceipt hxc_l_receipt = { 0 };
  hxc_string hxc_l_tmp_sys_println_string_argument_owner_n34 = { 0 };
  hxc_string hxc_l_tmp_string_concat_right_owner_n35 = { 0 };
  hxc_string hxc_l_tmp_sys_println_string_argument_owner_n36 = { 0 };
  hxc_string hxc_l_tmp_string_concat_right_owner_n37 = { 0 };
  hxc_string hxc_l_tmp_sys_println_string_argument_owner_n38 = { 0 };
  struct hxc_array_ref *hxc_l_initialActorPhases = { 0 };
  struct hxc_caxecraft_content_RuntimeLevelLoadResult hxc_l_g_h14daa0fd57c5 = { 0 };
  struct hxc_caxecraft_content_RuntimeLevelSource hxc_l_tmp_static_call_argument_0_owner_n9 = { 0 };
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
  struct hxc_caxecraft_content_BaseContentRegistry *hxc_l_tmp_managed_class_result_n5 = NULL;
  if (hxc_gc_allocate(&hxc_program_gc, &hxc_caxecraft_content_BaseContentRegistry_descriptor, (void **)&hxc_l_tmp_managed_class_result_n5) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_gc_roots[7] = (const void *)hxc_l_tmp_managed_class_result_n5;
  hxc_l_gc_roots[8] = (const void *)hxc_l_tmp_managed_class_result_n5;
  struct hxc_caxecraft_content_BaseContentRegistry *hxc_l_baseRegistry = hxc_l_tmp_managed_class_result_n5;
  hxc_l_gc_roots[9] = (const void *)NULL;
  struct hxc_caxecraft_content_RuntimeLevelCandidate *hxc_l_loadedCandidate = NULL;
  struct hxc_caxecraft_content_ContentPackageStore *hxc_l_tmp_load_result_n6 = hxc_l_contentStore;
  hxc_l_gc_roots[10] = (const void *)hxc_l_tmp_load_result_n6;
  hxc_string hxc_l_tmp_enum_payload_1_owner_n8 = (hxc_string){ (const uint8_t *)"scenarios/first-playable/map.caxemap", 36, true, NULL };
  if (hxc_string_retain(hxc_l_tmp_enum_payload_1_owner_n8) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_gc_roots[11] = (struct hxc_caxecraft_content_RuntimeLevelSource){ .hxc_tag = hxc_caxecraft_content_RuntimeLevelSource_NativePackageFile, .hxc_payload.hxc_NativePackageFile.hxc_store = hxc_l_tmp_load_result_n6, .hxc_payload.hxc_NativePackageFile.hxc_logicalPath = hxc_l_tmp_enum_payload_1_owner_n8 }.hxc_tag == hxc_caxecraft_content_RuntimeLevelSource_NativePackageFile ? (const void *)(struct hxc_caxecraft_content_RuntimeLevelSource){ .hxc_tag = hxc_caxecraft_content_RuntimeLevelSource_NativePackageFile, .hxc_payload.hxc_NativePackageFile.hxc_store = hxc_l_tmp_load_result_n6, .hxc_payload.hxc_NativePackageFile.hxc_logicalPath = hxc_l_tmp_enum_payload_1_owner_n8 }.hxc_payload.hxc_NativePackageFile.hxc_store : NULL;
  hxc_l_gc_roots[12] = (struct hxc_caxecraft_content_RuntimeLevelSource){ .hxc_tag = hxc_caxecraft_content_RuntimeLevelSource_NativePackageFile, .hxc_payload.hxc_NativePackageFile.hxc_store = hxc_l_tmp_load_result_n6, .hxc_payload.hxc_NativePackageFile.hxc_logicalPath = hxc_l_tmp_enum_payload_1_owner_n8 }.hxc_tag == hxc_caxecraft_content_RuntimeLevelSource_AdmittedPackageBytes ? (const void *)(struct hxc_caxecraft_content_RuntimeLevelSource){ .hxc_tag = hxc_caxecraft_content_RuntimeLevelSource_NativePackageFile, .hxc_payload.hxc_NativePackageFile.hxc_store = hxc_l_tmp_load_result_n6, .hxc_payload.hxc_NativePackageFile.hxc_logicalPath = hxc_l_tmp_enum_payload_1_owner_n8 }.hxc_payload.hxc_AdmittedPackageBytes.hxc_content : NULL;
  hxc_l_tmp_static_call_argument_0_owner_n9 = (struct hxc_caxecraft_content_RuntimeLevelSource){ .hxc_tag = hxc_caxecraft_content_RuntimeLevelSource_NativePackageFile, .hxc_payload.hxc_NativePackageFile.hxc_store = hxc_l_tmp_load_result_n6, .hxc_payload.hxc_NativePackageFile.hxc_logicalPath = hxc_l_tmp_enum_payload_1_owner_n8 };
  struct hxc_caxecraft_content_RuntimeLevelSource hxc_l_tmp_static_call_argument_0_borrow_result_n9 = hxc_l_tmp_static_call_argument_0_owner_n9;
  hxc_l_gc_roots[13] = hxc_l_tmp_static_call_argument_0_borrow_result_n9.hxc_tag == hxc_caxecraft_content_RuntimeLevelSource_NativePackageFile ? (const void *)hxc_l_tmp_static_call_argument_0_borrow_result_n9.hxc_payload.hxc_NativePackageFile.hxc_store : NULL;
  hxc_l_gc_roots[14] = hxc_l_tmp_static_call_argument_0_borrow_result_n9.hxc_tag == hxc_caxecraft_content_RuntimeLevelSource_AdmittedPackageBytes ? (const void *)hxc_l_tmp_static_call_argument_0_borrow_result_n9.hxc_payload.hxc_AdmittedPackageBytes.hxc_content : NULL;
  struct hxc_caxecraft_content_BaseContentRegistry *hxc_l_tmp_load_result_n10 = hxc_l_baseRegistry;
  hxc_l_gc_roots[15] = (const void *)hxc_l_tmp_load_result_n10;
  if (hxc_l_tmp_load_result_n10 == NULL)
  {
    abort();
  }
  struct hxc_compiler_interface_dispatch_caxecraft_scenario_ScenarioContentRegistry_value hxc_l_tmp_interface_value_n11 = (struct hxc_compiler_interface_dispatch_caxecraft_scenario_ScenarioContentRegistry_value){ .object = hxc_l_tmp_load_result_n10, .table = &hxc_itable_compiler_interface_dispatch_caxecraft_content_BaseContentRegistry_itable_layout_caxecraft_s_h6e0cbd310f7fd037 };
  struct hxc_caxecraft_content_BaseContentRegistry *hxc_l_tmp_load_result_n12 = hxc_l_baseRegistry;
  hxc_l_gc_roots[16] = (const void *)hxc_l_tmp_load_result_n12;
  if (hxc_l_tmp_load_result_n12 == NULL)
  {
    abort();
  }
  struct hxc_compiler_interface_dispatch_caxecraft_content_LevelContentResolver_value hxc_l_tmp_interface_value_n13 = (struct hxc_compiler_interface_dispatch_caxecraft_content_LevelContentResolver_value){ .object = hxc_l_tmp_load_result_n12, .table = &hxc_itable_compiler_interface_dispatch_caxecraft_content_BaseContentRegistry_itable_layout_caxecraft_c_h0a40ae11dcf4897c };
  int32_t hxc_l_tmp_load_result_n14 = hxc_l_initialHealth;
  struct hxc_caxecraft_domain_AquaticProfile hxc_l_tmp_call_result_n15 = hxc_caxecraft_content_BaseContentPack_aquaticProfile(0);
  struct hxc_caxecraft_content_RuntimeLevelLoadResult hxc_l_tmp_call_result_n17 = hxc_caxecraft_content_RuntimeLevelLoader_loadRuntimeLevel(hxc_l_tmp_static_call_argument_0_borrow_result_n9, 1, hxc_l_tmp_interface_value_n11, hxc_l_tmp_interface_value_n13, (struct hxc_caxecraft_content_LevelPlayerOptions){ .hxc_aquaticProfile = hxc_l_tmp_call_result_n15, .hxc_entityId = 1, .hxc_initialHealth = hxc_l_tmp_load_result_n14 });
  hxc_l_gc_roots[17] = hxc_l_tmp_call_result_n17.hxc_tag == hxc_caxecraft_content_RuntimeLevelLoadResult_RuntimeLevelReady ? (const void *)hxc_l_tmp_call_result_n17.hxc_payload.hxc_RuntimeLevelReady.hxc_candidate : NULL;
  hxc_l_g_h14daa0fd57c5 = hxc_l_tmp_call_result_n17;
  hxc_l_gc_roots[18] = hxc_l_g_h14daa0fd57c5.hxc_tag == hxc_caxecraft_content_RuntimeLevelLoadResult_RuntimeLevelReady ? (const void *)hxc_l_g_h14daa0fd57c5.hxc_payload.hxc_RuntimeLevelReady.hxc_candidate : NULL;
  switch (hxc_l_g_h14daa0fd57c5.hxc_tag) {
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
        hxc_enum_be0a6f10_destroy(&hxc_l_g_h14daa0fd57c5);
        hxc_enum_21bc9bf3_destroy(&hxc_l_tmp_static_call_argument_0_owner_n9);
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
  hxc_l_gc_roots[19] = hxc_l_g_h14daa0fd57c5.hxc_tag == hxc_caxecraft_content_RuntimeLevelLoadResult_RuntimeLevelReady ? (const void *)hxc_l_g_h14daa0fd57c5.hxc_payload.hxc_RuntimeLevelReady.hxc_candidate : NULL;
  if (hxc_l_g_h14daa0fd57c5.hxc_tag != hxc_caxecraft_content_RuntimeLevelLoadResult_RuntimeLevelReady)
  {
    abort();
  }
  struct hxc_caxecraft_content_RuntimeLevelCandidate *hxc_l_tmp_enum_payload_project_n20 = hxc_l_g_h14daa0fd57c5.hxc_payload.hxc_RuntimeLevelReady.hxc_candidate;
  hxc_l_gc_roots[20] = (const void *)hxc_l_tmp_enum_payload_project_n20;
  struct hxc_caxecraft_content_RuntimeLevelCandidate *hxc_l_candidate = hxc_l_tmp_enum_payload_project_n20;
  hxc_l_gc_roots[21] = (const void *)hxc_l_candidate;
  hxc_l_loadedCandidate = hxc_l_candidate;
  struct hxc_caxecraft_content_RuntimeLevelCandidate *hxc_l_tmp_load_result_n22 = hxc_l_loadedCandidate;
  hxc_l_gc_roots[22] = (const void *)hxc_l_tmp_load_result_n22;
  if (hxc_l_tmp_load_result_n22 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_content_LoadedContentGeneration *hxc_l_tmp_class_field_load_result_n23 = (*hxc_l_tmp_load_result_n22).hxc_loadedGeneration;
  hxc_l_gc_roots[23] = (const void *)hxc_l_tmp_class_field_load_result_n23;
  struct hxc_caxecraft_content_ActiveContent *hxc_l_tmp_managed_class_result_n24 = NULL;
  if (hxc_gc_allocate(&hxc_program_gc, &hxc_caxecraft_content_ActiveContent_descriptor, (void **)&hxc_l_tmp_managed_class_result_n24) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_gc_roots[24] = (const void *)hxc_l_tmp_managed_class_result_n24;
  hxc_compiler_constructor_caxecraft_content_ActiveContent(hxc_l_tmp_managed_class_result_n24, hxc_l_tmp_class_field_load_result_n23);
  hxc_l_gc_roots[25] = (const void *)hxc_l_tmp_managed_class_result_n24;
  struct hxc_caxecraft_content_ActiveContent *hxc_l_activeContent = hxc_l_tmp_managed_class_result_n24;
  struct hxc_caxecraft_content_ActiveContent *hxc_l_tmp_load_result_n25 = hxc_l_activeContent;
  hxc_l_gc_roots[26] = (const void *)hxc_l_tmp_load_result_n25;
  if (hxc_l_tmp_load_result_n25 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_content_LoadedContentGeneration *hxc_l_tmp_class_field_load_result_n26 = (*hxc_l_tmp_load_result_n25).hxc_active;
  hxc_l_gc_roots[27] = (const void *)hxc_l_tmp_class_field_load_result_n26;
  if (hxc_l_tmp_class_field_load_result_n26 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_class_field_load_result_n27 = (*hxc_l_tmp_class_field_load_result_n26).hxc_ownedSession;
  hxc_l_gc_roots[28] = (const void *)hxc_l_tmp_class_field_load_result_n27;
  struct hxc_caxecraft_domain_GameSession *hxc_l_session = hxc_l_tmp_class_field_load_result_n27;
  struct hxc_array_ref *hxc_l_tmp_array_create_result_n28 = NULL;
  if (hxc_array_ref_create(hxc_default_allocator(), (hxc_array_element_ops){ sizeof(struct hxc_caxecraft_app_CaxecraftApp_PlayableActorBinding), _Alignof(struct hxc_caxecraft_app_CaxecraftApp_PlayableActorBinding), NULL, hxc_array_346eae85_element_copy, hxc_array_346eae85_element_assign, hxc_array_346eae85_element_destroy }, &hxc_l_tmp_array_create_result_n28) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_actorBindings = hxc_l_tmp_array_create_result_n28;
  int32_t hxc_l_g_h09e723f022f5 = 0;
  struct hxc_caxecraft_content_RuntimeLevelCandidate *hxc_l_tmp_load_result_n29 = hxc_l_loadedCandidate;
  hxc_l_gc_roots[29] = (const void *)hxc_l_tmp_load_result_n29;
  if (hxc_l_tmp_load_result_n29 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_content_LoadedContentGeneration *hxc_l_tmp_class_field_load_result_n30 = (*hxc_l_tmp_load_result_n29).hxc_loadedGeneration;
  hxc_l_gc_roots[30] = (const void *)hxc_l_tmp_class_field_load_result_n30;
  if (hxc_l_tmp_class_field_load_result_n30 == NULL)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_tmp_instance_call_result_n31 = hxc_caxecraft_content_LoadedContentGeneration_actorBindings(hxc_l_tmp_class_field_load_result_n30);
  hxc_l_g1_hb98ea081892b = hxc_l_tmp_instance_call_result_n31;
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n32 = hxc_l_g_h09e723f022f5;
    int32_t hxc_l_tmp_array_length_result_n34;
    if (hxc_array_ref_length(hxc_l_g1_hb98ea081892b, &hxc_l_tmp_array_length_result_n34) != HXC_STATUS_OK)
    {
      abort();
    }
    if (!(hxc_l_tmp_load_result_n32 < hxc_l_tmp_array_length_result_n34))
    {
      break;
    }
    struct hxc_array_ref *hxc_l_tmp_load_result_n35 = hxc_l_g1_hb98ea081892b;
    struct hxc_caxecraft_content_LoadedActorBinding hxc_l_tmp_array_get_result_n37;
    if (hxc_array_ref_get_copy(hxc_l_tmp_load_result_n35, (size_t)hxc_l_g_h09e723f022f5, &hxc_l_tmp_array_get_result_n37) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_array_element_owner_n17 = hxc_l_tmp_array_get_result_n37;
    hxc_l_binding_hc1b57b45b4a2 = hxc_l_tmp_array_element_owner_n17;
    if (hxc_record_ad1068b2_retain(&hxc_l_binding_hc1b57b45b4a2) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_g_h09e723f022f5 = hxc_i32_add_wrapping(hxc_l_g_h09e723f022f5, 1);
    struct hxc_array_ref *hxc_l_tmp_load_result_n40 = hxc_l_actorBindings;
    int32_t hxc_l_tmp_record_field_load_result_n41 = hxc_l_binding_hc1b57b45b4a2.hxc_entityId;
    struct hxc_caxecraft_content_CharacterSpawnRole hxc_l_tmp_record_field_role_owner_n18 = hxc_l_binding_hc1b57b45b4a2.hxc_role;
    if (hxc_enum_b133bfa7_retain(&hxc_l_tmp_record_field_role_owner_n18) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_array_push_element_owner_n19 = (struct hxc_caxecraft_app_CaxecraftApp_PlayableActorBinding){ .hxc_entityId = hxc_l_tmp_record_field_load_result_n41, .hxc_role = hxc_l_tmp_record_field_role_owner_n18 };
    int32_t hxc_l_tmp_array_push_result_n46;
    if (hxc_array_ref_push_copy(hxc_l_tmp_load_result_n40, &hxc_l_tmp_array_push_element_owner_n19, &hxc_l_tmp_array_push_result_n46) != HXC_STATUS_OK)
    {
      abort();
    }
    (void)hxc_l_tmp_array_push_result_n46;
    hxc_record_10bdee1f_destroy(&hxc_l_tmp_array_push_element_owner_n19);
    hxc_record_ad1068b2_destroy(&hxc_l_binding_hc1b57b45b4a2);
    hxc_array_aa4dc4ac_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n17);
  }
  struct hxc_array_ref *hxc_l_tmp_array_create_result_n47 = NULL;
  if (hxc_array_ref_create_trivial(hxc_default_allocator(), sizeof(struct hxc_caxecraft_app_LoadedWorldItem), _Alignof(struct hxc_caxecraft_app_LoadedWorldItem), &hxc_l_tmp_array_create_result_n47) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_loadedItems = hxc_l_tmp_array_create_result_n47;
  int32_t hxc_l_g_hefb1fb503026 = 0;
  struct hxc_caxecraft_content_RuntimeLevelCandidate *hxc_l_tmp_load_result_n48 = hxc_l_loadedCandidate;
  hxc_l_gc_roots[31] = (const void *)hxc_l_tmp_load_result_n48;
  if (hxc_l_tmp_load_result_n48 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_content_LoadedContentGeneration *hxc_l_tmp_class_field_load_result_n49 = (*hxc_l_tmp_load_result_n48).hxc_loadedGeneration;
  hxc_l_gc_roots[32] = (const void *)hxc_l_tmp_class_field_load_result_n49;
  if (hxc_l_tmp_class_field_load_result_n49 == NULL)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_tmp_instance_call_result_n50 = hxc_caxecraft_content_LoadedContentGeneration_itemBindings(hxc_l_tmp_class_field_load_result_n49);
  hxc_l_g1_h16b7ad605ace = hxc_l_tmp_instance_call_result_n50;
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n51 = hxc_l_g_hefb1fb503026;
    int32_t hxc_l_tmp_array_length_result_n53;
    if (hxc_array_ref_length(hxc_l_g1_h16b7ad605ace, &hxc_l_tmp_array_length_result_n53) != HXC_STATUS_OK)
    {
      abort();
    }
    if (!(hxc_l_tmp_load_result_n51 < hxc_l_tmp_array_length_result_n53))
    {
      break;
    }
    struct hxc_array_ref *hxc_l_tmp_load_result_n54 = hxc_l_g1_h16b7ad605ace;
    struct hxc_caxecraft_content_ResolvedItem hxc_l_tmp_array_get_result_n56;
    if (hxc_array_ref_get_copy(hxc_l_tmp_load_result_n54, (size_t)hxc_l_g_hefb1fb503026, &hxc_l_tmp_array_get_result_n56) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_array_element_owner_n24 = hxc_l_tmp_array_get_result_n56;
    hxc_l_binding_h5388213540e0 = hxc_l_tmp_array_element_owner_n24;
    if (hxc_record_6512ae83_retain(&hxc_l_binding_h5388213540e0) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_g_hefb1fb503026 = hxc_i32_add_wrapping(hxc_l_g_hefb1fb503026, 1);
    struct hxc_array_ref *hxc_l_tmp_load_result_n59 = hxc_l_loadedItems;
    int32_t hxc_l_tmp_record_field_load_result_n60 = hxc_l_binding_h5388213540e0.hxc_storage;
    int32_t hxc_l_tmp_record_field_load_result_n61 = hxc_l_binding_h5388213540e0.hxc_transform.hxc_xMilli;
    int32_t hxc_l_tmp_record_field_load_result_n62 = hxc_l_binding_h5388213540e0.hxc_transform.hxc_yMilli;
    int32_t hxc_l_tmp_array_push_result_n65;
    if (hxc_array_ref_push_copy(hxc_l_tmp_load_result_n59, &(struct hxc_caxecraft_app_LoadedWorldItem){ .hxc_storageCode = hxc_l_tmp_record_field_load_result_n60, .hxc_xMilli = hxc_l_tmp_record_field_load_result_n61, .hxc_yMilli = hxc_l_tmp_record_field_load_result_n62, .hxc_zMilli = hxc_l_binding_h5388213540e0.hxc_transform.hxc_zMilli }, &hxc_l_tmp_array_push_result_n65) != HXC_STATUS_OK)
    {
      abort();
    }
    (void)hxc_l_tmp_array_push_result_n65;
    hxc_record_6512ae83_destroy(&hxc_l_binding_h5388213540e0);
    hxc_array_d5f9659a_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n24);
  }
  struct hxc_caxecraft_content_RuntimeLevelCandidate *hxc_l_tmp_load_result_n66 = hxc_l_loadedCandidate;
  hxc_l_gc_roots[33] = (const void *)hxc_l_tmp_load_result_n66;
  if (hxc_l_tmp_load_result_n66 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_content_LoadedContentGeneration *hxc_l_tmp_class_field_load_result_n67 = (*hxc_l_tmp_load_result_n66).hxc_loadedGeneration;
  hxc_l_gc_roots[34] = (const void *)hxc_l_tmp_class_field_load_result_n67;
  if (hxc_l_tmp_class_field_load_result_n67 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_content_ResolvedLevelPlan *hxc_l_tmp_class_field_load_result_n68 = (*hxc_l_tmp_class_field_load_result_n67).hxc_resolvedPlan;
  hxc_l_gc_roots[35] = (const void *)hxc_l_tmp_class_field_load_result_n68;
  if (hxc_l_tmp_class_field_load_result_n68 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_content_ResolvedPlayer hxc_l_tmp_instance_call_result_n69 = hxc_caxecraft_content_ResolvedLevelPlan_player(hxc_l_tmp_class_field_load_result_n68);
  hxc_l_tmp_record_field_transform_receiver_owner_n26 = hxc_l_tmp_instance_call_result_n69;
  struct hxc_caxecraft_scenario_ScenarioTransform hxc_l_spawnTransform = hxc_l_tmp_record_field_transform_receiver_owner_n26.hxc_transform;
  struct hxc_caxecraft_content_RuntimeLevelCandidate *hxc_l_tmp_load_result_n72 = hxc_l_loadedCandidate;
  hxc_l_gc_roots[36] = (const void *)hxc_l_tmp_load_result_n72;
  if (hxc_l_tmp_load_result_n72 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_content_LoadedContentGeneration *hxc_l_tmp_class_field_load_result_n73 = (*hxc_l_tmp_load_result_n72).hxc_loadedGeneration;
  hxc_l_gc_roots[37] = (const void *)hxc_l_tmp_class_field_load_result_n73;
  if (hxc_l_tmp_class_field_load_result_n73 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_content_ResolvedLevelPresentationPlan *hxc_l_tmp_class_field_load_result_n74 = (*hxc_l_tmp_class_field_load_result_n73).hxc_presentationPlan;
  hxc_l_gc_roots[38] = (const void *)hxc_l_tmp_class_field_load_result_n74;
  if (hxc_l_tmp_class_field_load_result_n74 == NULL)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_tmp_instance_call_result_n75 = hxc_caxecraft_content_ResolvedLevelPresentationPlan_fluidRequests(hxc_l_tmp_class_field_load_result_n74);
  hxc_l_fluidPresentation = hxc_l_tmp_instance_call_result_n75;
  int32_t hxc_l_tmp_array_length_result_n77;
  if (hxc_array_ref_length(hxc_l_fluidPresentation, &hxc_l_tmp_array_length_result_n77) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_l_tmp_array_length_result_n77 == 0)
  {
    if (hxc_array_ref_release(hxc_l_fluidPresentation) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_record_2bd8b029_destroy(&hxc_l_tmp_record_field_transform_receiver_owner_n26);
    if (hxc_array_ref_release(hxc_l_g1_h16b7ad605ace) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_loadedItems) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_g1_hb98ea081892b) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_actorBindings) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_enum_be0a6f10_destroy(&hxc_l_g_h14daa0fd57c5);
    hxc_enum_21bc9bf3_destroy(&hxc_l_tmp_static_call_argument_0_owner_n9);
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return;
  }
  struct hxc_caxecraft_content_ResolvedFluidPresentation hxc_l_tmp_array_get_result_n79;
  if (hxc_array_ref_get_copy(hxc_l_fluidPresentation, (size_t)0, &hxc_l_tmp_array_get_result_n79) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_tmp_array_element_owner_n29 = hxc_l_tmp_array_get_result_n79;
  int32_t hxc_l_waterPresentationCell = hxc_l_tmp_array_element_owner_n29.hxc_cellIndex;
  int32_t hxc_l_g_hb9b52a9fca9f = 0;
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n82 = hxc_l_g_hb9b52a9fca9f;
    int32_t hxc_l_tmp_array_length_result_n84;
    if (hxc_array_ref_length(hxc_l_fluidPresentation, &hxc_l_tmp_array_length_result_n84) != HXC_STATUS_OK)
    {
      abort();
    }
    if (!(hxc_l_tmp_load_result_n82 < hxc_l_tmp_array_length_result_n84))
    {
      break;
    }
    struct hxc_array_ref *hxc_l_tmp_load_result_n85 = hxc_l_fluidPresentation;
    struct hxc_caxecraft_content_ResolvedFluidPresentation hxc_l_tmp_array_get_result_n87;
    if (hxc_array_ref_get_copy(hxc_l_tmp_load_result_n85, (size_t)hxc_l_g_hb9b52a9fca9f, &hxc_l_tmp_array_get_result_n87) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_array_element_owner_n32 = hxc_l_tmp_array_get_result_n87;
    hxc_l_request = hxc_l_tmp_array_element_owner_n32;
    if (hxc_record_c8b1d97c_retain(&hxc_l_request) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_g_hb9b52a9fca9f = hxc_i32_add_wrapping(hxc_l_g_hb9b52a9fca9f, 1);
    int32_t hxc_l_tmp_record_field_load_result_n90 = hxc_l_request.hxc_cellIndex;
    if (hxc_l_tmp_record_field_load_result_n90 != hxc_l_waterPresentationCell)
    {
      hxc_record_c8b1d97c_destroy(&hxc_l_request);
      hxc_array_1f383c02_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n32);
      hxc_array_1f383c02_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n29);
      if (hxc_array_ref_release(hxc_l_fluidPresentation) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_record_2bd8b029_destroy(&hxc_l_tmp_record_field_transform_receiver_owner_n26);
      if (hxc_array_ref_release(hxc_l_g1_h16b7ad605ace) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_l_loadedItems) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_l_g1_hb98ea081892b) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_l_actorBindings) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_enum_be0a6f10_destroy(&hxc_l_g_h14daa0fd57c5);
      hxc_enum_21bc9bf3_destroy(&hxc_l_tmp_static_call_argument_0_owner_n9);
      if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
      {
        abort();
      }
      return;
    }
    hxc_record_c8b1d97c_destroy(&hxc_l_request);
    hxc_array_1f383c02_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n32);
  }
  struct hxc_caxecraft_content_RuntimeLevelCandidate *hxc_l_tmp_load_result_n92 = hxc_l_loadedCandidate;
  hxc_l_gc_roots[39] = (const void *)hxc_l_tmp_load_result_n92;
  if (hxc_l_tmp_load_result_n92 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_content_RuntimeLevelReceipt hxc_l_tmp_instance_call_result_n93 = hxc_caxecraft_content_RuntimeLevelCandidate_receipt(hxc_l_tmp_load_result_n92);
  hxc_l_receipt = hxc_l_tmp_instance_call_result_n93;
  if (hxc_io_println((hxc_string){ (const uint8_t *)"caxecraft: content-source=runtime-package", 41, true, NULL }) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_string hxc_l_tmp_string_concat_result_n95 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
  if (hxc_string_concat_ref((hxc_string){ (const uint8_t *)"caxecraft: content-path=", 24, true, NULL }, hxc_l_receipt.hxc_logicalPath, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n95) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_tmp_sys_println_string_argument_owner_n34 = hxc_l_tmp_string_concat_result_n95;
  if (hxc_io_println(hxc_l_tmp_sys_println_string_argument_owner_n34) != HXC_STATUS_OK)
  {
    if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n34) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_record_f6127605_destroy(&hxc_l_receipt);
    hxc_array_1f383c02_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n29);
    if (hxc_array_ref_release(hxc_l_fluidPresentation) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_record_2bd8b029_destroy(&hxc_l_tmp_record_field_transform_receiver_owner_n26);
    if (hxc_array_ref_release(hxc_l_g1_h16b7ad605ace) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_loadedItems) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_g1_hb98ea081892b) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_actorBindings) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_enum_be0a6f10_destroy(&hxc_l_g_h14daa0fd57c5);
    hxc_enum_21bc9bf3_destroy(&hxc_l_tmp_static_call_argument_0_owner_n9);
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
  hxc_string hxc_l_tmp_std_string_int_result_n98 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
  if (hxc_string_from_int32(hxc_l_receipt.hxc_inputHash, hxc_default_allocator(), &hxc_l_tmp_std_string_int_result_n98) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_tmp_string_concat_right_owner_n35 = hxc_l_tmp_std_string_int_result_n98;
  hxc_string hxc_l_tmp_string_concat_result_n100 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
  if (hxc_string_concat_ref((hxc_string){ (const uint8_t *)"caxecraft: content-input-hash=", 30, true, NULL }, hxc_l_tmp_string_concat_right_owner_n35, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n100) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_tmp_sys_println_string_argument_owner_n36 = hxc_l_tmp_string_concat_result_n100;
  if (hxc_io_println(hxc_l_tmp_sys_println_string_argument_owner_n36) != HXC_STATUS_OK)
  {
    if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n36) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n35) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_record_f6127605_destroy(&hxc_l_receipt);
    hxc_array_1f383c02_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n29);
    if (hxc_array_ref_release(hxc_l_fluidPresentation) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_record_2bd8b029_destroy(&hxc_l_tmp_record_field_transform_receiver_owner_n26);
    if (hxc_array_ref_release(hxc_l_g1_h16b7ad605ace) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_loadedItems) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_g1_hb98ea081892b) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_actorBindings) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_enum_be0a6f10_destroy(&hxc_l_g_h14daa0fd57c5);
    hxc_enum_21bc9bf3_destroy(&hxc_l_tmp_static_call_argument_0_owner_n9);
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
  if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n35) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_caxecraft_content_ActiveContent *hxc_l_tmp_load_result_n102 = hxc_l_activeContent;
  hxc_l_gc_roots[40] = (const void *)hxc_l_tmp_load_result_n102;
  if (hxc_l_tmp_load_result_n102 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_content_LoadedContentGeneration *hxc_l_tmp_class_field_load_result_n103 = (*hxc_l_tmp_load_result_n102).hxc_active;
  hxc_l_gc_roots[41] = (const void *)hxc_l_tmp_class_field_load_result_n103;
  if (hxc_l_tmp_class_field_load_result_n103 == NULL)
  {
    abort();
  }
  int32_t hxc_l_tmp_class_field_load_result_n104 = (*hxc_l_tmp_class_field_load_result_n103).hxc_generationIdValue;
  hxc_string hxc_l_tmp_std_string_int_result_n105 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
  if (hxc_string_from_int32(hxc_l_tmp_class_field_load_result_n104, hxc_default_allocator(), &hxc_l_tmp_std_string_int_result_n105) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_tmp_string_concat_right_owner_n37 = hxc_l_tmp_std_string_int_result_n105;
  hxc_string hxc_l_tmp_string_concat_result_n107 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
  if (hxc_string_concat_ref((hxc_string){ (const uint8_t *)"caxecraft: content-generation=", 30, true, NULL }, hxc_l_tmp_string_concat_right_owner_n37, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n107) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_tmp_sys_println_string_argument_owner_n38 = hxc_l_tmp_string_concat_result_n107;
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
    hxc_array_1f383c02_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n29);
    if (hxc_array_ref_release(hxc_l_fluidPresentation) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_record_2bd8b029_destroy(&hxc_l_tmp_record_field_transform_receiver_owner_n26);
    if (hxc_array_ref_release(hxc_l_g1_h16b7ad605ace) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_loadedItems) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_g1_hb98ea081892b) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_actorBindings) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_enum_be0a6f10_destroy(&hxc_l_g_h14daa0fd57c5);
    hxc_enum_21bc9bf3_destroy(&hxc_l_tmp_static_call_argument_0_owner_n9);
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
  struct hxc_caxecraft_app_CaxecraftApp_PlayableActorSelection hxc_l_tmp_call_result_n110 = hxc_caxecraft_app_CaxecraftApp_selectPlayableActors(hxc_l_actorBindings);
  struct hxc_caxecraft_app_CaxecraftApp_PlayableActorSelection hxc_l_actors = hxc_l_tmp_call_result_n110;
  if (!hxc_l_actors.hxc_valid)
  {
    hxc_record_f6127605_destroy(&hxc_l_receipt);
    hxc_array_1f383c02_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n29);
    if (hxc_array_ref_release(hxc_l_fluidPresentation) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_record_2bd8b029_destroy(&hxc_l_tmp_record_field_transform_receiver_owner_n26);
    if (hxc_array_ref_release(hxc_l_g1_h16b7ad605ace) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_loadedItems) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_g1_hb98ea081892b) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_actorBindings) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_enum_be0a6f10_destroy(&hxc_l_g_h14daa0fd57c5);
    hxc_enum_21bc9bf3_destroy(&hxc_l_tmp_static_call_argument_0_owner_n9);
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
  bool hxc_l_tmp_native_call_result_n116 = IsWindowReady();
  if (!hxc_l_tmp_native_call_result_n116)
  {
    hxc_record_f6127605_destroy(&hxc_l_receipt);
    hxc_array_1f383c02_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n29);
    if (hxc_array_ref_release(hxc_l_fluidPresentation) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_record_2bd8b029_destroy(&hxc_l_tmp_record_field_transform_receiver_owner_n26);
    if (hxc_array_ref_release(hxc_l_g1_h16b7ad605ace) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_loadedItems) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_g1_hb98ea081892b) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_actorBindings) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_enum_be0a6f10_destroy(&hxc_l_g_h14daa0fd57c5);
    hxc_enum_21bc9bf3_destroy(&hxc_l_tmp_static_call_argument_0_owner_n9);
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
  struct Texture hxc_l_tmp_native_call_result_n117 = LoadTexture("assets/showcase/title-panorama.png");
  struct Texture hxc_l_titleTexture = hxc_l_tmp_native_call_result_n117;
  bool hxc_l_tmp_native_call_result_n119 = IsTextureValid(hxc_l_titleTexture);
  bool hxc_l_titleTextureReady = hxc_l_tmp_native_call_result_n119;
  struct Texture hxc_l_tmp_native_call_result_n120 = LoadTexture("assets/branding/caxecraft-wordmark.png");
  struct Texture hxc_l_wordmarkTexture = hxc_l_tmp_native_call_result_n120;
  bool hxc_l_tmp_native_call_result_n122 = IsTextureValid(hxc_l_wordmarkTexture);
  bool hxc_l_wordmarkTextureReady = hxc_l_tmp_native_call_result_n122;
  struct Texture hxc_l_tmp_native_call_result_n123 = LoadTexture("assets/atlases/hud.png");
  struct Texture hxc_l_hudTexture = hxc_l_tmp_native_call_result_n123;
  bool hxc_l_tmp_native_call_result_n125 = IsTextureValid(hxc_l_hudTexture);
  bool hxc_l_hudTextureReady = hxc_l_tmp_native_call_result_n125;
  struct Texture hxc_l_tmp_native_call_result_n126 = LoadTexture("assets/atlases/items.png");
  struct Texture hxc_l_itemTexture = hxc_l_tmp_native_call_result_n126;
  bool hxc_l_tmp_native_call_result_n128 = IsTextureValid(hxc_l_itemTexture);
  bool hxc_l_itemTextureReady = hxc_l_tmp_native_call_result_n128;
  struct Texture hxc_l_tmp_native_call_result_n129 = LoadTexture("assets/atlases/adventure-items.png");
  struct Texture hxc_l_adventureItemTexture = hxc_l_tmp_native_call_result_n129;
  bool hxc_l_tmp_native_call_result_n131 = IsTextureValid(hxc_l_adventureItemTexture);
  bool hxc_l_adventureItemTextureReady = hxc_l_tmp_native_call_result_n131;
  struct Texture hxc_l_tmp_load_result_n132 = hxc_l_hudTexture;
  bool hxc_l_tmp_load_result_n133 = hxc_l_hudTextureReady;
  struct Texture hxc_l_tmp_load_result_n134 = hxc_l_itemTexture;
  bool hxc_l_tmp_load_result_n135 = hxc_l_itemTextureReady;
  struct Texture hxc_l_tmp_load_result_n136 = hxc_l_adventureItemTexture;
  struct hxc_caxecraft_app_HudResources hxc_l_hudResources = (struct hxc_caxecraft_app_HudResources){ .hxc_adventureItemTexture = hxc_l_tmp_load_result_n136, .hxc_adventureItemTextureReady = hxc_l_adventureItemTextureReady, .hxc_hudTexture = hxc_l_tmp_load_result_n132, .hxc_hudTextureReady = hxc_l_tmp_load_result_n133, .hxc_itemTexture = hxc_l_tmp_load_result_n134, .hxc_itemTextureReady = hxc_l_tmp_load_result_n135 };
  struct Texture hxc_l_tmp_native_call_result_n139 = LoadTexture("assets/atlases/entities.png");
  struct Texture hxc_l_entityTexture = hxc_l_tmp_native_call_result_n139;
  bool hxc_l_tmp_native_call_result_n141 = IsTextureValid(hxc_l_entityTexture);
  bool hxc_l_entityTextureReady = hxc_l_tmp_native_call_result_n141;
  struct Texture hxc_l_tmp_native_call_result_n142 = LoadTexture("assets/atlases/terrain.png");
  struct Texture hxc_l_terrainTexture = hxc_l_tmp_native_call_result_n142;
  bool hxc_l_tmp_native_call_result_n144 = IsTextureValid(hxc_l_terrainTexture);
  bool hxc_l_terrainTextureReady = hxc_l_tmp_native_call_result_n144;
  struct Texture hxc_l_tmp_native_call_result_n145 = LoadTexture("assets/atlases/adventure-terrain.png");
  struct Texture hxc_l_adventureTerrainTexture = hxc_l_tmp_native_call_result_n145;
  bool hxc_l_tmp_native_call_result_n147 = IsTextureValid(hxc_l_adventureTerrainTexture);
  bool hxc_l_adventureTerrainTextureReady = hxc_l_tmp_native_call_result_n147;
  double hxc_l_cameraWaterBlend = 0.0;
  struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n148 = hxc_caxecraft_gameplay_Inventory_starter();
  struct hxc_caxecraft_gameplay_InventoryState hxc_l_inventory = hxc_l_tmp_call_result_n148;
  enum hxc_caxecraft_gameplay_GuidePhase hxc_l_guidePhase = hxc_caxecraft_gameplay_GuidePhase_Waiting;
  struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n150 = hxc_l_session;
  hxc_l_gc_roots[42] = (const void *)hxc_l_tmp_load_result_n150;
  if (hxc_l_tmp_load_result_n150 == NULL)
  {
    abort();
  }
  bool hxc_l_tmp_instance_call_result_n152 = hxc_caxecraft_domain_GameSession_actorInteractionAvailable(hxc_l_tmp_load_result_n150, hxc_l_dialogueActorId);
  bool hxc_l_guideInteractionAvailable = hxc_l_tmp_instance_call_result_n152;
  struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n153 = hxc_l_session;
  hxc_l_gc_roots[43] = (const void *)hxc_l_tmp_load_result_n153;
  if (hxc_l_tmp_load_result_n153 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n155 = hxc_caxecraft_domain_GameSession_readCharacter(hxc_l_tmp_load_result_n153, hxc_l_dialogueActorId);
  struct hxc_caxecraft_domain_Character hxc_l_dialogueActor = hxc_l_tmp_instance_call_result_n155;
  struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n156 = hxc_l_session;
  hxc_l_gc_roots[44] = (const void *)hxc_l_tmp_load_result_n156;
  if (hxc_l_tmp_load_result_n156 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n158 = hxc_caxecraft_domain_GameSession_readCharacter(hxc_l_tmp_load_result_n156, hxc_l_enemyActorId);
  struct hxc_caxecraft_domain_Character hxc_l_enemyActor = hxc_l_tmp_instance_call_result_n158;
  struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n159 = hxc_l_session;
  hxc_l_gc_roots[45] = (const void *)hxc_l_tmp_load_result_n159;
  if (hxc_l_tmp_load_result_n159 == NULL)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_tmp_instance_call_result_n160 = hxc_caxecraft_domain_GameSession_actorControllerStateSnapshots(hxc_l_tmp_load_result_n159);
  hxc_l_initialActorPhases = hxc_l_tmp_instance_call_result_n160;
  struct hxc_array_ref *hxc_l_tmp_load_result_n161 = hxc_l_initialActorPhases;
  struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation hxc_l_tmp_call_result_n164 = hxc_caxecraft_app_CaxecraftApp_observeActorPhase(hxc_l_tmp_load_result_n161, hxc_l_dialogueActorId, hxc_caxecraft_domain_ActorControllerPhase_Stationary);
  struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation hxc_l_dialoguePhase = hxc_l_tmp_call_result_n164;
  struct hxc_array_ref *hxc_l_tmp_load_result_n165 = hxc_l_initialActorPhases;
  struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation hxc_l_tmp_call_result_n168 = hxc_caxecraft_app_CaxecraftApp_observeActorPhase(hxc_l_tmp_load_result_n165, hxc_l_enemyActorId, hxc_caxecraft_domain_ActorControllerPhase_Resting);
  struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation hxc_l_enemyPhase = hxc_l_tmp_call_result_n168;
  int32_t hxc_l_tmp_record_field_load_result_n169 = hxc_l_dialogueActor.hxc_id;
  bool hxc_l_tmp_short_circuit_result_n69 = hxc_l_tmp_record_field_load_result_n169 <= 0;
  if (!(hxc_l_tmp_record_field_load_result_n169 <= 0))
  {
    hxc_l_tmp_short_circuit_result_n69 = hxc_l_enemyActor.hxc_id <= 0;
  }
  bool hxc_l_tmp_short_circuit_load_result_n171 = hxc_l_tmp_short_circuit_result_n69;
  bool hxc_l_tmp_short_circuit_result_n70 = hxc_l_tmp_short_circuit_load_result_n171;
  if (!hxc_l_tmp_short_circuit_load_result_n171)
  {
    hxc_l_tmp_short_circuit_result_n70 = !hxc_l_dialoguePhase.hxc_valid;
  }
  bool hxc_l_tmp_short_circuit_load_result_n173 = hxc_l_tmp_short_circuit_result_n70;
  bool hxc_l_tmp_short_circuit_result_n71 = hxc_l_tmp_short_circuit_load_result_n173;
  if (!hxc_l_tmp_short_circuit_load_result_n173)
  {
    hxc_l_tmp_short_circuit_result_n71 = !hxc_l_enemyPhase.hxc_valid;
  }
  if (hxc_l_tmp_short_circuit_result_n71)
  {
    if (hxc_array_ref_release(hxc_l_initialActorPhases) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_record_f6127605_destroy(&hxc_l_receipt);
    hxc_array_1f383c02_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n29);
    if (hxc_array_ref_release(hxc_l_fluidPresentation) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_record_2bd8b029_destroy(&hxc_l_tmp_record_field_transform_receiver_owner_n26);
    if (hxc_array_ref_release(hxc_l_g1_h16b7ad605ace) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_loadedItems) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_g1_hb98ea081892b) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_actorBindings) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_enum_be0a6f10_destroy(&hxc_l_g_h14daa0fd57c5);
    hxc_enum_21bc9bf3_destroy(&hxc_l_tmp_static_call_argument_0_owner_n9);
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
  struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n178 = hxc_l_session;
  hxc_l_gc_roots[46] = (const void *)hxc_l_tmp_load_result_n178;
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
    hxc_record_f6127605_destroy(&hxc_l_receipt);
    hxc_array_1f383c02_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n29);
    if (hxc_array_ref_release(hxc_l_fluidPresentation) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_record_2bd8b029_destroy(&hxc_l_tmp_record_field_transform_receiver_owner_n26);
    if (hxc_array_ref_release(hxc_l_g1_h16b7ad605ace) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_loadedItems) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_g1_hb98ea081892b) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_actorBindings) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_enum_be0a6f10_destroy(&hxc_l_g_h14daa0fd57c5);
    hxc_enum_21bc9bf3_destroy(&hxc_l_tmp_static_call_argument_0_owner_n9);
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
    bool hxc_l_tmp_short_circuit_result_n100 = !hxc_l_tmp_load_result_n187;
    if (!hxc_l_tmp_load_result_n187)
    {
      bool hxc_l_tmp_native_call_result_n188 = WindowShouldClose();
      hxc_l_tmp_short_circuit_result_n100 = !hxc_l_tmp_native_call_result_n188;
    }
    if (!hxc_l_tmp_short_circuit_result_n100)
    {
      break;
    }
    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n190 = hxc_l_session;
    hxc_l_gc_roots[47] = (const void *)hxc_l_tmp_load_result_n190;
    if (hxc_l_tmp_load_result_n190 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_domain_GameView hxc_l_tmp_instance_call_result_n191 = hxc_caxecraft_domain_GameSession_view(hxc_l_tmp_load_result_n190);
    struct hxc_caxecraft_domain_GameView hxc_l_initialView = hxc_l_tmp_instance_call_result_n191;
    if (!hxc_l_initialView.hxc_valid)
    {
      hxc_l_quit = true;
    }
    struct hxc_caxecraft_domain_Character hxc_l_character = hxc_l_initialView.hxc_localPlayer;
    bool hxc_l_recapturedThisFrame = false;
    bool hxc_l_resetMotionThisFrame = false;
    float hxc_l_tmp_native_call_result_n194 = GetFrameTime();
    double hxc_l_frameSeconds = (double)hxc_l_tmp_native_call_result_n194;
    if (hxc_l_frameSeconds > 0.25)
    {
      hxc_l_frameSeconds = 0.25;
    }
    bool hxc_l_tmp_native_call_result_n196 = IsWindowFocused();
    bool hxc_l_focused = hxc_l_tmp_native_call_result_n196;
    bool hxc_l_tmp_call_result_n198 = hxc_caxecraft_app_AppScreen_capturesPointer(hxc_l_screen);
    bool hxc_l_captured_h891cd0b43970 = hxc_l_tmp_call_result_n198;
    bool hxc_l_tmp_call_result_n200 = hxc_caxecraft_app_AppScreen_pausesSimulation(hxc_l_screen);
    bool hxc_l_paused_hd9b792e32d4e = hxc_l_tmp_call_result_n200;
    double hxc_l_forward = 0.0;
    double hxc_l_right = 0.0;
    bool hxc_l_tmp_native_call_result_n201 = IsKeyDown((int32_t)87);
    if (hxc_l_tmp_native_call_result_n201)
    {
      hxc_l_forward = hxc_l_forward + 1.0;
    }
    bool hxc_l_tmp_native_call_result_n203 = IsKeyDown((int32_t)83);
    if (hxc_l_tmp_native_call_result_n203)
    {
      hxc_l_forward = hxc_l_forward - 1.0;
    }
    bool hxc_l_tmp_native_call_result_n205 = IsKeyDown((int32_t)68);
    if (hxc_l_tmp_native_call_result_n205)
    {
      hxc_l_right = hxc_l_right + 1.0;
    }
    bool hxc_l_tmp_native_call_result_n207 = IsKeyDown((int32_t)65);
    if (hxc_l_tmp_native_call_result_n207)
    {
      hxc_l_right = hxc_l_right - 1.0;
    }
    double hxc_l_lookYaw_h5bdd5686635d = 0.0;
    double hxc_l_lookPitch_ha7ac9f537d74 = 0.0;
    if (hxc_l_captured_h891cd0b43970)
    {
      struct Vector2 hxc_l_tmp_native_call_result_n210 = GetMouseDelta();
      struct Vector2 hxc_l_mouse = hxc_l_tmp_native_call_result_n210;
      hxc_l_lookYaw_h5bdd5686635d = -(double)hxc_l_mouse.x * 0.0025;
      hxc_l_lookPitch_ha7ac9f537d74 = -(double)hxc_l_mouse.y * 0.0025;
    }
    bool hxc_l_tmp_native_call_result_n215 = IsMouseButtonPressed((int32_t)0);
    bool hxc_l_leftPressed = hxc_l_tmp_native_call_result_n215;
    bool hxc_l_tmp_load_result_n216 = hxc_l_captured_h891cd0b43970;
    bool hxc_l_tmp_short_circuit_result_n116 = hxc_l_tmp_load_result_n216;
    if (hxc_l_tmp_load_result_n216)
    {
      hxc_l_tmp_short_circuit_result_n116 = hxc_l_leftPressed;
    }
    bool hxc_l_primaryPressed_h1af73905bdff = hxc_l_tmp_short_circuit_result_n116;
    bool hxc_l_tmp_load_result_n219 = hxc_l_captured_h891cd0b43970;
    bool hxc_l_tmp_short_circuit_result_n118 = hxc_l_tmp_load_result_n219;
    if (hxc_l_tmp_load_result_n219)
    {
      bool hxc_l_tmp_native_call_result_n220 = IsMouseButtonPressed((int32_t)1);
      hxc_l_tmp_short_circuit_result_n118 = hxc_l_tmp_native_call_result_n220;
    }
    bool hxc_l_secondaryPressed_ha8c84c3f2476 = hxc_l_tmp_short_circuit_result_n118;
    bool hxc_l_tmp_load_result_n222 = hxc_l_captured_h891cd0b43970;
    bool hxc_l_tmp_short_circuit_result_n120 = hxc_l_tmp_load_result_n222;
    if (hxc_l_tmp_load_result_n222)
    {
      bool hxc_l_tmp_native_call_result_n223 = IsKeyPressed((int32_t)69);
      hxc_l_tmp_short_circuit_result_n120 = hxc_l_tmp_native_call_result_n223;
    }
    bool hxc_l_interactPressed_h709cc3e1ed4a = hxc_l_tmp_short_circuit_result_n120;
    int32_t hxc_l_hotbarSelection_he6c92f132080 = -1;
    bool hxc_l_tmp_native_call_result_n225 = IsKeyPressed((int32_t)49);
    if (hxc_l_tmp_native_call_result_n225)
    {
      hxc_l_hotbarSelection_he6c92f132080 = 0;
    }
    bool hxc_l_tmp_native_call_result_n226 = IsKeyPressed((int32_t)50);
    if (hxc_l_tmp_native_call_result_n226)
    {
      hxc_l_hotbarSelection_he6c92f132080 = 1;
    }
    bool hxc_l_tmp_native_call_result_n227 = IsKeyPressed((int32_t)51);
    if (hxc_l_tmp_native_call_result_n227)
    {
      hxc_l_hotbarSelection_he6c92f132080 = 2;
    }
    bool hxc_l_tmp_native_call_result_n228 = IsKeyPressed((int32_t)52);
    if (hxc_l_tmp_native_call_result_n228)
    {
      hxc_l_hotbarSelection_he6c92f132080 = 3;
    }
    bool hxc_l_tmp_native_call_result_n229 = IsKeyPressed((int32_t)53);
    if (hxc_l_tmp_native_call_result_n229)
    {
      hxc_l_hotbarSelection_he6c92f132080 = 4;
    }
    bool hxc_l_tmp_native_call_result_n230 = IsKeyPressed((int32_t)54);
    if (hxc_l_tmp_native_call_result_n230)
    {
      hxc_l_hotbarSelection_he6c92f132080 = 5;
    }
    bool hxc_l_tmp_native_call_result_n231 = IsKeyPressed((int32_t)55);
    if (hxc_l_tmp_native_call_result_n231)
    {
      hxc_l_hotbarSelection_he6c92f132080 = 6;
    }
    bool hxc_l_tmp_native_call_result_n232 = IsKeyPressed((int32_t)56);
    if (hxc_l_tmp_native_call_result_n232)
    {
      hxc_l_hotbarSelection_he6c92f132080 = 7;
    }
    float hxc_l_tmp_native_call_result_n233 = GetMouseWheelMove();
    double hxc_l_wheel = (double)hxc_l_tmp_native_call_result_n233;
    int32_t hxc_l_hotbarCycle_hd017c47d4271 = 0;
    if (hxc_l_wheel > 0.0)
    {
      hxc_l_hotbarCycle_hd017c47d4271 = -1;
    }
    if (hxc_l_wheel < 0.0)
    {
      hxc_l_hotbarCycle_hd017c47d4271 = 1;
    }
    bool hxc_l_tmp_native_call_result_n236 = IsKeyPressed((int32_t)32);
    bool hxc_l_jumpPressed_h597e38984c80 = hxc_l_tmp_native_call_result_n236;
    bool hxc_l_tmp_native_call_result_n237 = IsKeyPressed((int32_t)256);
    bool hxc_l_pausePressed_hc79ba631a4fb = hxc_l_tmp_native_call_result_n237;
    bool hxc_l_tmp_native_call_result_n238 = IsKeyPressed((int32_t)81);
    bool hxc_l_quitPressed_h0bee7a736de8 = hxc_l_tmp_native_call_result_n238;
    double hxc_l_frameInput_moveForward = hxc_l_forward;
    double hxc_l_frameInput_moveRight = hxc_l_right;
    double hxc_l_frameInput_lookYaw = hxc_l_lookYaw_h5bdd5686635d;
    double hxc_l_frameInput_lookPitch = hxc_l_lookPitch_ha7ac9f537d74;
    bool hxc_l_frameInput_jumpPressed = hxc_l_jumpPressed_h597e38984c80;
    bool hxc_l_tmp_native_call_result_n244 = IsKeyDown((int32_t)340);
    bool hxc_l_frameInput_descendHeld = hxc_l_tmp_native_call_result_n244;
    bool hxc_l_frameInput_primaryPressed = hxc_l_primaryPressed_h1af73905bdff;
    bool hxc_l_frameInput_secondaryPressed = hxc_l_secondaryPressed_ha8c84c3f2476;
    bool hxc_l_frameInput_interactPressed = hxc_l_interactPressed_h709cc3e1ed4a;
    bool hxc_l_frameInput_pausePressed = hxc_l_pausePressed_hc79ba631a4fb;
    bool hxc_l_tmp_load_result_n249 = hxc_l_paused_hd9b792e32d4e;
    bool hxc_l_tmp_short_circuit_result_n138 = hxc_l_tmp_load_result_n249;
    if (hxc_l_tmp_load_result_n249)
    {
      hxc_l_tmp_short_circuit_result_n138 = hxc_l_leftPressed;
    }
    bool hxc_l_frameInput_capturePressed = hxc_l_tmp_short_circuit_result_n138;
    bool hxc_l_frameInput_quitPressed = hxc_l_quitPressed_h0bee7a736de8;
    int32_t hxc_l_frameInput_hotbarSelection = hxc_l_hotbarSelection_he6c92f132080;
    int32_t hxc_l_frameInput_hotbarCycle = hxc_l_hotbarCycle_hd017c47d4271;
    double hxc_l_moveForward = hxc_l_frameInput_moveForward;
    double hxc_l_moveRight = hxc_l_frameInput_moveRight;
    double hxc_l_lookYaw_h3c3384525697 = hxc_l_frameInput_lookYaw;
    double hxc_l_lookPitch_h9def09d81040 = hxc_l_frameInput_lookPitch;
    bool hxc_l_jumpPressed_h5efb6f4438f6 = hxc_l_frameInput_jumpPressed;
    bool hxc_l_primaryPressed_hddf6382a4cfd = hxc_l_frameInput_primaryPressed;
    bool hxc_l_secondaryPressed_h5a095466600c = hxc_l_frameInput_secondaryPressed;
    bool hxc_l_interactPressed_h52fb1c90a21f = hxc_l_frameInput_interactPressed;
    bool hxc_l_pausePressed_h6b7e48a5ab98 = hxc_l_frameInput_pausePressed;
    bool hxc_l_capturePressed = hxc_l_frameInput_capturePressed;
    bool hxc_l_quitPressed_he783cbae0bc6 = hxc_l_frameInput_quitPressed;
    int32_t hxc_l_hotbarSelection_h439288b85718 = hxc_l_frameInput_hotbarSelection;
    int32_t hxc_l_hotbarCycle_hca78ca5ac973 = hxc_l_frameInput_hotbarCycle;
    bool hxc_l_descendHeld = hxc_l_frameInput_descendHeld;
    if (hxc_l_quitPressed_he783cbae0bc6)
    {
      hxc_l_quit = true;
    }
    if (hxc_l_hotbarSelection_h439288b85718 >= 0)
    {
      struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_load_result_n271 = hxc_l_inventory;
      struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n273 = hxc_caxecraft_gameplay_Inventory_select(hxc_l_tmp_load_result_n271, hxc_l_hotbarSelection_h439288b85718);
      hxc_l_inventory = hxc_l_tmp_call_result_n273;
    }
    if (hxc_l_hotbarCycle_hca78ca5ac973 != 0)
    {
      struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_load_result_n275 = hxc_l_inventory;
      struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n277 = hxc_caxecraft_gameplay_Inventory_cycle(hxc_l_tmp_load_result_n275, hxc_l_hotbarCycle_hca78ca5ac973);
      hxc_l_inventory = hxc_l_tmp_call_result_n277;
    }
    bool hxc_l_tmp_call_result_n279 = hxc_caxecraft_app_AppScreen_isPlaying(hxc_l_screen);
    bool hxc_l_tmp_short_circuit_result_n156 = hxc_l_tmp_call_result_n279;
    if (hxc_l_tmp_call_result_n279)
    {
      hxc_l_tmp_short_circuit_result_n156 = hxc_l_interactPressed_h52fb1c90a21f;
    }
    if (hxc_l_tmp_short_circuit_result_n156)
    {
      if (hxc_l_character.hxc_vitals.hxc_health <= 0)
      {
        struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n283 = hxc_l_session;
        hxc_l_gc_roots[48] = (const void *)hxc_l_tmp_load_result_n283;
        if (hxc_l_tmp_load_result_n283 == NULL)
        {
          abort();
        }
        struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n284 = hxc_l_session;
        hxc_l_gc_roots[49] = (const void *)hxc_l_tmp_load_result_n284;
        if (hxc_l_tmp_load_result_n284 == NULL)
        {
          abort();
        }
        size_t hxc_l_tmp_length_n286;
        const uint8_t *hxc_l_tmp_instance_call_result_n285 = hxc_caxecraft_domain_GameSession_worldView(hxc_l_tmp_load_result_n284, &hxc_l_tmp_length_n286);
        struct hxc_caxecraft_domain_CharacterBody hxc_l_tmp_call_result_n288 = hxc_caxecraft_app_CaxecraftApp_spawnPlayer(hxc_l_tmp_instance_call_result_n285, hxc_l_tmp_length_n286, hxc_l_spawnTransform);
        struct hxc_caxecraft_domain_LocalCharacterCommandResult hxc_l_tmp_instance_call_result_n289 = hxc_caxecraft_domain_GameSession_reviveLocalPlayerAt(hxc_l_tmp_load_result_n283, hxc_l_tmp_call_result_n288);
        struct hxc_caxecraft_domain_LocalCharacterCommandResult hxc_l_revival = hxc_l_tmp_instance_call_result_n289;
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
        struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n292 = hxc_l_session;
        hxc_l_gc_roots[50] = (const void *)hxc_l_tmp_load_result_n292;
        if (hxc_l_tmp_load_result_n292 == NULL)
        {
          abort();
        }
        bool hxc_l_tmp_instance_call_result_n294 = hxc_caxecraft_domain_GameSession_actorInteractionAvailable(hxc_l_tmp_load_result_n292, hxc_l_dialogueActorId);
        if (hxc_l_tmp_instance_call_result_n294)
        {
          bool hxc_l_sharesBerries = hxc_l_guidePhase == hxc_caxecraft_gameplay_GuidePhase_Welcomed;
          if (hxc_l_sharesBerries)
          {
            int32_t hxc_l_tmp_call_result_n300 = hxc_caxecraft_gameplay_Inventory_acceptedAmount(hxc_l_inventory, hxc_caxecraft_gameplay_ItemKind_Berries, 2);
            int32_t hxc_l_acceptedGift = hxc_l_tmp_call_result_n300;
            if (hxc_l_acceptedGift == 2)
            {
              struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_load_result_n302 = hxc_l_inventory;
              struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n305 = hxc_caxecraft_gameplay_Inventory_collectItem(hxc_l_tmp_load_result_n302, hxc_caxecraft_gameplay_ItemKind_Berries, hxc_l_acceptedGift);
              hxc_l_inventory = hxc_l_tmp_call_result_n305;
              enum hxc_caxecraft_gameplay_GuidePhase hxc_l_tmp_call_result_n307 = hxc_caxecraft_app_CaxecraftApp_advanceGuidePhase(hxc_l_guidePhase);
              hxc_l_guidePhase = hxc_l_tmp_call_result_n307;
            }
            else
            {
              hxc_l_inventoryFullReason = 1;
              hxc_l_inventoryFullFrames = 90;
            }
          }
          else
          {
            enum hxc_caxecraft_gameplay_GuidePhase hxc_l_tmp_call_result_n309 = hxc_caxecraft_app_CaxecraftApp_advanceGuidePhase(hxc_l_guidePhase);
            hxc_l_guidePhase = hxc_l_tmp_call_result_n309;
          }
        }
      }
    }
    bool hxc_l_tmp_call_result_n311 = hxc_caxecraft_app_AppScreen_showsTitle(hxc_l_screen);
    bool hxc_l_tmp_short_circuit_result_n160 = hxc_l_tmp_call_result_n311;
    if (hxc_l_tmp_call_result_n311)
    {
      hxc_l_tmp_short_circuit_result_n160 = hxc_l_focused;
    }
    if (hxc_l_tmp_short_circuit_result_n160)
    {
      int32_t hxc_l_modeBeforeInput = hxc_l_selectedMode;
      bool hxc_l_tmp_native_call_result_n315 = IsKeyPressed((int32_t)76);
      if (hxc_l_tmp_native_call_result_n315)
      {
        int32_t hxc_l_tmp_call_result_n317 = hxc_caxecraft_localization_UiCatalog_nextLocale(hxc_l_locale);
        hxc_l_locale = hxc_l_tmp_call_result_n317;
      }
      bool hxc_l_tmp_native_call_result_n318 = IsKeyPressed((int32_t)265);
      bool hxc_l_tmp_short_circuit_result_n162 = hxc_l_tmp_native_call_result_n318;
      if (!hxc_l_tmp_native_call_result_n318)
      {
        bool hxc_l_tmp_native_call_result_n319 = IsKeyPressed((int32_t)264);
        hxc_l_tmp_short_circuit_result_n162 = hxc_l_tmp_native_call_result_n319;
      }
      if (hxc_l_tmp_short_circuit_result_n162)
      {
        int32_t hxc_l_tmp_load_result_n321 = hxc_l_selectedMode;
        int32_t hxc_l_tmp_conditional_result_n163 = 0;
        if (hxc_l_tmp_load_result_n321 == 0)
        {
          hxc_l_tmp_conditional_result_n163 = 1;
        }
        else
        {
          hxc_l_tmp_conditional_result_n163 = 0;
        }
        hxc_l_selectedMode = hxc_l_tmp_conditional_result_n163;
      }
      struct Vector2 hxc_l_tmp_native_call_result_n323 = GetMousePosition();
      struct Vector2 hxc_l_menuMouse = hxc_l_tmp_native_call_result_n323;
      struct Vector2 hxc_l_tmp_load_result_n324 = hxc_l_menuMouse;
      struct Vector2 hxc_l_tmp_load_result_n326 = hxc_l_menuMouse;
      int32_t hxc_l_tmp_native_call_result_n328 = GetScreenWidth();
      int32_t hxc_l_tmp_native_call_result_n329 = GetScreenHeight();
      int32_t hxc_l_tmp_call_result_n330 = hxc_caxecraft_app_TitleMenu_selectionAt((double)hxc_l_tmp_load_result_n324.x, (double)hxc_l_tmp_load_result_n326.y, (int32_t)hxc_l_tmp_native_call_result_n328, (int32_t)hxc_l_tmp_native_call_result_n329);
      int32_t hxc_l_hovered = hxc_l_tmp_call_result_n330;
      if (hxc_l_hovered == 0)
      {
        hxc_l_selectedMode = 0;
      }
      if (hxc_l_hovered == 1)
      {
        hxc_l_selectedMode = 1;
      }
      int32_t hxc_l_tmp_load_result_n333 = hxc_l_selectedMode;
      if (hxc_l_tmp_load_result_n333 != hxc_l_modeBeforeInput)
      {
        hxc_l_accumulator = 0.0;
        hxc_l_resetMotionThisFrame = true;
      }
      int32_t hxc_l_tmp_load_result_n335 = hxc_l_hovered;
      bool hxc_l_tmp_short_circuit_result_n167 = hxc_l_tmp_load_result_n335 >= 0;
      if (hxc_l_tmp_load_result_n335 >= 0)
      {
        bool hxc_l_tmp_native_call_result_n336 = IsMouseButtonPressed((int32_t)0);
        hxc_l_tmp_short_circuit_result_n167 = hxc_l_tmp_native_call_result_n336;
      }
      bool hxc_l_clickedChoice = hxc_l_tmp_short_circuit_result_n167;
      bool hxc_l_tmp_load_result_n338 = hxc_l_clickedChoice;
      bool hxc_l_tmp_short_circuit_result_n168 = hxc_l_tmp_load_result_n338;
      if (hxc_l_tmp_load_result_n338)
      {
        hxc_l_tmp_short_circuit_result_n168 = hxc_l_hovered == 2;
      }
      if (hxc_l_tmp_short_circuit_result_n168)
      {
        enum hxc_caxecraft_app_AppScreen hxc_l_tmp_call_result_n342 = hxc_caxecraft_app_AppScreen_openEditor(hxc_l_screen);
        hxc_l_screen = hxc_l_tmp_call_result_n342;
        hxc_l_accumulator = 0.0;
        hxc_l_resetMotionThisFrame = true;
        EnableCursor();
      }
      else
      {
        bool hxc_l_tmp_load_result_n343 = hxc_l_clickedChoice;
        bool hxc_l_tmp_short_circuit_result_n169 = hxc_l_tmp_load_result_n343;
        if (!hxc_l_tmp_load_result_n343)
        {
          bool hxc_l_tmp_native_call_result_n344 = IsKeyPressed((int32_t)257);
          hxc_l_tmp_short_circuit_result_n169 = hxc_l_tmp_native_call_result_n344;
        }
        if (hxc_l_tmp_short_circuit_result_n169)
        {
          enum hxc_caxecraft_app_AppScreen hxc_l_tmp_call_result_n347 = hxc_caxecraft_app_AppScreen_startPlaying(hxc_l_screen);
          hxc_l_screen = hxc_l_tmp_call_result_n347;
          hxc_l_recapturedThisFrame = true;
          hxc_l_accumulator = 0.0;
          hxc_l_resetMotionThisFrame = true;
          DisableCursor();
        }
      }
    }
    bool hxc_l_tmp_load_result_n348 = hxc_l_focused;
    bool hxc_l_tmp_short_circuit_result_n170 = !hxc_l_tmp_load_result_n348;
    if (!hxc_l_tmp_load_result_n348)
    {
      bool hxc_l_tmp_call_result_n350 = hxc_caxecraft_app_AppScreen_isPlaying(hxc_l_screen);
      hxc_l_tmp_short_circuit_result_n170 = hxc_l_tmp_call_result_n350;
    }
    if (hxc_l_tmp_short_circuit_result_n170)
    {
      enum hxc_caxecraft_app_AppScreen hxc_l_tmp_call_result_n353 = hxc_caxecraft_app_AppScreen_loseFocus(hxc_l_screen);
      hxc_l_screen = hxc_l_tmp_call_result_n353;
      hxc_l_jumpQueued = false;
      hxc_l_accumulator = 0.0;
      hxc_l_resetMotionThisFrame = true;
      EnableCursor();
    }
    bool hxc_l_tmp_call_result_n355 = hxc_caxecraft_app_AppScreen_showsTitle(hxc_l_screen);
    bool hxc_l_tmp_short_circuit_result_n171 = !hxc_l_tmp_call_result_n355;
    if (!hxc_l_tmp_call_result_n355)
    {
      bool hxc_l_tmp_call_result_n357 = hxc_caxecraft_app_AppScreen_showsEditor(hxc_l_screen);
      hxc_l_tmp_short_circuit_result_n171 = !hxc_l_tmp_call_result_n357;
    }
    bool hxc_l_tmp_short_circuit_load_result_n358 = hxc_l_tmp_short_circuit_result_n171;
    bool hxc_l_tmp_short_circuit_result_n172 = hxc_l_tmp_short_circuit_load_result_n358;
    if (hxc_l_tmp_short_circuit_load_result_n358)
    {
      hxc_l_tmp_short_circuit_result_n172 = hxc_l_focused;
    }
    bool hxc_l_tmp_short_circuit_load_result_n360 = hxc_l_tmp_short_circuit_result_n172;
    bool hxc_l_tmp_short_circuit_result_n173 = hxc_l_tmp_short_circuit_load_result_n360;
    if (hxc_l_tmp_short_circuit_load_result_n360)
    {
      hxc_l_tmp_short_circuit_result_n173 = hxc_l_pausePressed_h6b7e48a5ab98;
    }
    if (hxc_l_tmp_short_circuit_result_n173)
    {
      enum hxc_caxecraft_app_AppScreen hxc_l_tmp_call_result_n364 = hxc_caxecraft_app_AppScreen_togglePause(hxc_l_screen);
      hxc_l_screen = hxc_l_tmp_call_result_n364;
      hxc_l_accumulator = 0.0;
      hxc_l_resetMotionThisFrame = true;
      bool hxc_l_tmp_call_result_n366 = hxc_caxecraft_app_AppScreen_pausesSimulation(hxc_l_screen);
      if (hxc_l_tmp_call_result_n366)
      {
        hxc_l_jumpQueued = false;
      }
      bool hxc_l_tmp_call_result_n368 = hxc_caxecraft_app_AppScreen_capturesPointer(hxc_l_screen);
      if (hxc_l_tmp_call_result_n368)
      {
        DisableCursor();
      }
      else
      {
        EnableCursor();
      }
    }
    bool hxc_l_tmp_call_result_n370 = hxc_caxecraft_app_AppScreen_showsTitle(hxc_l_screen);
    bool hxc_l_tmp_short_circuit_result_n174 = !hxc_l_tmp_call_result_n370;
    if (!hxc_l_tmp_call_result_n370)
    {
      bool hxc_l_tmp_call_result_n372 = hxc_caxecraft_app_AppScreen_showsEditor(hxc_l_screen);
      hxc_l_tmp_short_circuit_result_n174 = !hxc_l_tmp_call_result_n372;
    }
    bool hxc_l_tmp_short_circuit_load_result_n373 = hxc_l_tmp_short_circuit_result_n174;
    bool hxc_l_tmp_short_circuit_result_n175 = hxc_l_tmp_short_circuit_load_result_n373;
    if (hxc_l_tmp_short_circuit_load_result_n373)
    {
      hxc_l_tmp_short_circuit_result_n175 = hxc_l_focused;
    }
    bool hxc_l_tmp_short_circuit_load_result_n375 = hxc_l_tmp_short_circuit_result_n175;
    bool hxc_l_tmp_short_circuit_result_n176 = hxc_l_tmp_short_circuit_load_result_n375;
    if (hxc_l_tmp_short_circuit_load_result_n375)
    {
      bool hxc_l_tmp_call_result_n377 = hxc_caxecraft_app_AppScreen_pausesSimulation(hxc_l_screen);
      hxc_l_tmp_short_circuit_result_n176 = hxc_l_tmp_call_result_n377;
    }
    bool hxc_l_tmp_short_circuit_load_result_n378 = hxc_l_tmp_short_circuit_result_n176;
    bool hxc_l_tmp_short_circuit_result_n177 = hxc_l_tmp_short_circuit_load_result_n378;
    if (hxc_l_tmp_short_circuit_load_result_n378)
    {
      hxc_l_tmp_short_circuit_result_n177 = hxc_l_capturePressed;
    }
    if (hxc_l_tmp_short_circuit_result_n177)
    {
      enum hxc_caxecraft_app_AppScreen hxc_l_tmp_call_result_n382 = hxc_caxecraft_app_AppScreen_recapture(hxc_l_screen);
      hxc_l_screen = hxc_l_tmp_call_result_n382;
      hxc_l_recapturedThisFrame = true;
      hxc_l_accumulator = 0.0;
      hxc_l_resetMotionThisFrame = true;
      DisableCursor();
    }
    bool hxc_l_tmp_call_result_n384 = hxc_caxecraft_app_AppScreen_showsTitle(hxc_l_screen);
    bool hxc_l_onTitle = hxc_l_tmp_call_result_n384;
    bool hxc_l_tmp_call_result_n386 = hxc_caxecraft_app_AppScreen_showsEditor(hxc_l_screen);
    bool hxc_l_onEditor = hxc_l_tmp_call_result_n386;
    bool hxc_l_tmp_call_result_n388 = hxc_caxecraft_app_AppScreen_pausesSimulation(hxc_l_screen);
    bool hxc_l_paused_he6d3ac7c8a8b = hxc_l_tmp_call_result_n388;
    bool hxc_l_tmp_call_result_n390 = hxc_caxecraft_app_AppScreen_capturesPointer(hxc_l_screen);
    bool hxc_l_captured_h58da63bbdd3c = hxc_l_tmp_call_result_n390;
    int32_t hxc_l_editorNavigationCommand = 0;
    if (hxc_l_onEditor)
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
      struct hxc_caxecraft_input_NavigationSample hxc_l_tmp_call_result_n393 = hxc_caxecraft_app_RaylibNavigationInput_samplePrimaryGamepad();
      int32_t hxc_l_tmp_instance_call_result_n395 = hxc_caxecraft_input_NavigationRepeater_advance(hxc_l_tmp_owned_class_field_address_n392, hxc_l_tmp_call_result_n393, hxc_l_frameSeconds);
      hxc_l_editorNavigationCommand = hxc_l_tmp_instance_call_result_n395;
    }
    else
    {
      if (hxc_l_self == NULL)
      {
        abort();
      }
      struct hxc_caxecraft_input_NavigationRepeater *hxc_l_tmp_owned_class_field_address_n396 = &(*hxc_l_self).hxc_editorNavigation;
      if (hxc_l_tmp_owned_class_field_address_n396 == NULL)
      {
        abort();
      }
      hxc_caxecraft_input_NavigationRepeater_release(hxc_l_tmp_owned_class_field_address_n396);
    }
    if (hxc_l_captured_h58da63bbdd3c)
    {
      double hxc_l_yawDelta = hxc_l_lookYaw_h3c3384525697;
      if (hxc_l_yawDelta > 0.25)
      {
        hxc_l_yawDelta = 0.25;
      }
      if (hxc_l_yawDelta < -0.25)
      {
        hxc_l_yawDelta = -0.25;
      }
      double hxc_l_tmp_load_result_n401 = hxc_l_lookX;
      double hxc_l_tmp_load_result_n402 = hxc_l_yawDelta;
      double hxc_l_candidateX = hxc_l_tmp_load_result_n401 + hxc_l_tmp_load_result_n402 * hxc_l_lookZ;
      double hxc_l_tmp_load_result_n404 = hxc_l_lookZ;
      double hxc_l_tmp_load_result_n405 = hxc_l_yawDelta;
      double hxc_l_candidateZ = hxc_l_tmp_load_result_n404 - hxc_l_tmp_load_result_n405 * hxc_l_lookX;
      double hxc_l_tmp_load_result_n407 = hxc_l_candidateX;
      double hxc_l_tmp_load_result_n408 = hxc_l_candidateX;
      double hxc_l_tmp_load_result_n409 = hxc_l_candidateZ;
      double hxc_l_lengthSquared = hxc_l_tmp_load_result_n407 * hxc_l_tmp_load_result_n408 + hxc_l_tmp_load_result_n409 * hxc_l_candidateZ;
      double hxc_l_normalize = 1.5 - 0.5 * hxc_l_lengthSquared;
      double hxc_l_tmp_load_result_n412 = hxc_l_candidateX;
      hxc_l_lookX = hxc_l_tmp_load_result_n412 * hxc_l_normalize;
      double hxc_l_tmp_load_result_n414 = hxc_l_candidateZ;
      hxc_l_lookZ = hxc_l_tmp_load_result_n414 * hxc_l_normalize;
      double hxc_l_tmp_compound_load_result_n416 = hxc_l_lookY;
      hxc_l_lookY = hxc_l_tmp_compound_load_result_n416 + hxc_l_lookPitch_h9def09d81040;
      if (hxc_l_lookY > 0.90)
      {
        hxc_l_lookY = 0.90;
      }
      if (hxc_l_lookY < -0.90)
      {
        hxc_l_lookY = -0.90;
      }
      if (hxc_l_jumpPressed_h5efb6f4438f6)
      {
        hxc_l_jumpQueued = true;
      }
    }
    bool hxc_l_tmp_load_result_n421 = hxc_l_captured_h58da63bbdd3c;
    bool hxc_l_tmp_short_circuit_result_n188 = hxc_l_tmp_load_result_n421;
    if (hxc_l_tmp_load_result_n421)
    {
      hxc_l_tmp_short_circuit_result_n188 = !hxc_l_recapturedThisFrame;
    }
    bool hxc_l_tmp_short_circuit_load_result_n423 = hxc_l_tmp_short_circuit_result_n188;
    bool hxc_l_tmp_short_circuit_result_n189 = hxc_l_tmp_short_circuit_load_result_n423;
    if (hxc_l_tmp_short_circuit_load_result_n423)
    {
      hxc_l_tmp_short_circuit_result_n189 = hxc_l_primaryPressed_hddf6382a4cfd;
    }
    bool hxc_l_tmp_short_circuit_load_result_n425 = hxc_l_tmp_short_circuit_result_n189;
    bool hxc_l_tmp_short_circuit_result_n190 = hxc_l_tmp_short_circuit_load_result_n425;
    if (hxc_l_tmp_short_circuit_load_result_n425)
    {
      hxc_l_tmp_short_circuit_result_n190 = hxc_l_selectedMode == 1;
    }
    bool hxc_l_tmp_short_circuit_load_result_n427 = hxc_l_tmp_short_circuit_result_n190;
    bool hxc_l_tmp_short_circuit_result_n191 = hxc_l_tmp_short_circuit_load_result_n427;
    if (hxc_l_tmp_short_circuit_load_result_n427)
    {
      bool hxc_l_tmp_call_result_n430 = hxc_caxecraft_gameplay_Inventory_selectedIs(hxc_l_inventory, hxc_caxecraft_gameplay_ItemKind_CopperSword);
      hxc_l_tmp_short_circuit_result_n191 = hxc_l_tmp_call_result_n430;
    }
    if (hxc_l_tmp_short_circuit_result_n191)
    {
      hxc_l_swordQueued = true;
    }
    if (!hxc_l_paused_he6d3ac7c8a8b)
    {
      double hxc_l_tmp_compound_load_result_n433 = hxc_l_accumulator;
      hxc_l_accumulator = hxc_l_tmp_compound_load_result_n433 + hxc_l_frameSeconds;
    }
    while (1)
    {
      bool hxc_l_tmp_load_result_n435 = hxc_l_paused_he6d3ac7c8a8b;
      bool hxc_l_tmp_short_circuit_result_n192 = !hxc_l_tmp_load_result_n435;
      if (!hxc_l_tmp_load_result_n435)
      {
        hxc_l_tmp_short_circuit_result_n192 = hxc_l_accumulator >= 0.05;
      }
      if (!hxc_l_tmp_short_circuit_result_n192)
      {
        break;
      }
      double hxc_l_tmp_load_result_n438 = hxc_l_moveForward;
      double hxc_l_tmp_load_result_n439 = hxc_l_lookX;
      double hxc_l_tmp_load_result_n440 = hxc_l_moveRight;
      double hxc_l_moveX = hxc_l_tmp_load_result_n438 * hxc_l_tmp_load_result_n439 - hxc_l_tmp_load_result_n440 * hxc_l_lookZ;
      double hxc_l_tmp_load_result_n442 = hxc_l_moveForward;
      double hxc_l_tmp_load_result_n443 = hxc_l_lookZ;
      double hxc_l_tmp_load_result_n444 = hxc_l_moveRight;
      double hxc_l_moveZ = hxc_l_tmp_load_result_n442 * hxc_l_tmp_load_result_n443 + hxc_l_tmp_load_result_n444 * hxc_l_lookX;
      double hxc_l_tmp_load_result_n446 = hxc_l_moveForward;
      bool hxc_l_tmp_short_circuit_result_n195 = hxc_l_tmp_load_result_n446 != 0.0;
      if (hxc_l_tmp_load_result_n446 != 0.0)
      {
        hxc_l_tmp_short_circuit_result_n195 = hxc_l_moveRight != 0.0;
      }
      if (hxc_l_tmp_short_circuit_result_n195)
      {
        hxc_l_moveX = hxc_l_moveX * 0.7071067811865476;
        hxc_l_moveZ = hxc_l_moveZ * 0.7071067811865476;
      }
      int32_t hxc_l_tmp_load_result_n451 = hxc_l_selectedMode;
      int32_t hxc_l_tmp_conditional_result_n197 = 0;
      if (hxc_l_tmp_load_result_n451 == 1)
      {
        hxc_l_tmp_conditional_result_n197 = 1;
      }
      else
      {
        hxc_l_tmp_conditional_result_n197 = 0;
      }
      int32_t hxc_l_damagePolicy = hxc_l_tmp_conditional_result_n197;
      struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n453 = hxc_l_session;
      hxc_l_gc_roots[52] = (const void *)hxc_l_tmp_load_result_n453;
      if (hxc_l_tmp_load_result_n453 == NULL)
      {
        abort();
      }
      double hxc_l_tmp_load_result_n454 = hxc_l_moveX;
      double hxc_l_tmp_load_result_n455 = hxc_l_moveZ;
      bool hxc_l_tmp_load_result_n456 = hxc_l_jumpQueued;
      struct hxc_caxecraft_domain_CharacterIntent hxc_l_tmp_call_result_n458 = hxc_caxecraft_domain_Aquatics_input(hxc_l_tmp_load_result_n454, hxc_l_tmp_load_result_n455, hxc_l_tmp_load_result_n456, hxc_l_descendHeld);
      struct hxc_caxecraft_domain_GameTickResult hxc_l_tmp_instance_call_result_n461 = hxc_caxecraft_domain_GameSession_tick(hxc_l_tmp_load_result_n453, (struct hxc_caxecraft_domain_GameTickInput){ .hxc_damagePolicy = hxc_l_damagePolicy, .hxc_intent = hxc_l_tmp_call_result_n458, .hxc_waterUpdateBudget = 64 });
      struct hxc_caxecraft_domain_GameTickResult hxc_l_gameTick = hxc_l_tmp_instance_call_result_n461;
      hxc_l_character = hxc_l_gameTick.hxc_character;
      if (hxc_l_gameTick.hxc_committed)
      {
        struct hxc_caxecraft_app_MotionHistory hxc_l_tmp_load_result_n464 = hxc_l_motionHistory;
        struct hxc_caxecraft_app_MotionHistory hxc_l_tmp_call_result_n466 = hxc_caxecraft_app_MotionInterpolation_advance(hxc_l_tmp_load_result_n464, hxc_l_character.hxc_body);
        hxc_l_motionHistory = hxc_l_tmp_call_result_n466;
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
          int32_t hxc_l_tmp_load_result_n470 = hxc_l_pickupIndex;
          int32_t hxc_l_tmp_array_length_result_n472;
          if (hxc_array_ref_length(hxc_l_loadedItems, &hxc_l_tmp_array_length_result_n472) != HXC_STATUS_OK)
          {
            abort();
          }
          if (!(hxc_l_tmp_load_result_n470 < hxc_l_tmp_array_length_result_n472))
          {
            break;
          }
          struct hxc_array_ref *hxc_l_tmp_load_result_n473 = hxc_l_loadedItems;
          struct hxc_caxecraft_app_LoadedWorldItem hxc_l_tmp_array_get_result_n475;
          if (hxc_array_ref_get_copy(hxc_l_tmp_load_result_n473, (size_t)hxc_l_pickupIndex, &hxc_l_tmp_array_get_result_n475) != HXC_STATUS_OK)
          {
            abort();
          }
          struct hxc_caxecraft_app_LoadedWorldItem hxc_l_loadedItem = hxc_l_tmp_array_get_result_n475;
          struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n476 = hxc_l_session;
          hxc_l_gc_roots[53] = (const void *)hxc_l_tmp_load_result_n476;
          if (hxc_l_tmp_load_result_n476 == NULL)
          {
            abort();
          }
          bool hxc_l_tmp_instance_call_result_n478 = hxc_caxecraft_domain_GameSession_authoredItemIsActive(hxc_l_tmp_load_result_n476, hxc_l_pickupIndex);
          bool hxc_l_tmp_short_circuit_result_n201 = hxc_l_tmp_instance_call_result_n478;
          if (hxc_l_tmp_instance_call_result_n478)
          {
            double hxc_l_tmp_record_field_load_result_n479 = hxc_l_character.hxc_body.hxc_x;
            double hxc_l_tmp_record_field_load_result_n480 = hxc_l_character.hxc_body.hxc_y;
            double hxc_l_tmp_record_field_load_result_n481 = hxc_l_character.hxc_body.hxc_z;
            int32_t hxc_l_tmp_record_field_load_result_n482 = hxc_l_loadedItem.hxc_xMilli;
            int32_t hxc_l_tmp_record_field_load_result_n483 = hxc_l_loadedItem.hxc_yMilli;
            bool hxc_l_tmp_call_result_n485 = hxc_caxecraft_gameplay_WorldItemPickup_isInRange(hxc_l_tmp_record_field_load_result_n479, hxc_l_tmp_record_field_load_result_n480, hxc_l_tmp_record_field_load_result_n481, hxc_l_tmp_record_field_load_result_n482, hxc_l_tmp_record_field_load_result_n483, hxc_l_loadedItem.hxc_zMilli);
            hxc_l_tmp_short_circuit_result_n201 = hxc_l_tmp_call_result_n485;
          }
          if (hxc_l_tmp_short_circuit_result_n201)
          {
            int32_t hxc_l_itemCode = hxc_l_loadedItem.hxc_storageCode;
            int32_t hxc_l_tmp_call_result_n489 = hxc_caxecraft_content_BaseContentPack_itemFromValidatedStorageCode(hxc_l_itemCode);
            int32_t hxc_l_item = hxc_l_tmp_call_result_n489;
            int32_t hxc_l_tmp_call_result_n491 = hxc_caxecraft_content_BaseContentPack_itemUseProfile(hxc_l_item);
            bool hxc_l_tmp_short_circuit_result_n204 = hxc_l_tmp_call_result_n491 == 1;
            if (hxc_l_tmp_call_result_n491 == 1)
            {
              bool hxc_l_tmp_call_result_n493 = hxc_caxecraft_content_BaseContentPack_itemProvidesAquaticProfile(hxc_l_item);
              hxc_l_tmp_short_circuit_result_n204 = hxc_l_tmp_call_result_n493;
            }
            if (hxc_l_tmp_short_circuit_result_n204)
            {
              int32_t hxc_l_tmp_call_result_n496 = hxc_caxecraft_content_BaseContentPack_itemAquaticProfile(hxc_l_item);
              struct hxc_caxecraft_domain_AquaticProfile hxc_l_tmp_call_result_n497 = hxc_caxecraft_content_BaseContentPack_aquaticProfile(hxc_l_tmp_call_result_n496);
              struct hxc_caxecraft_domain_AquaticProfile hxc_l_replacement = hxc_l_tmp_call_result_n497;
              struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n498 = hxc_l_session;
              hxc_l_gc_roots[54] = (const void *)hxc_l_tmp_load_result_n498;
              if (hxc_l_tmp_load_result_n498 == NULL)
              {
                abort();
              }
              int32_t hxc_l_tmp_load_result_n499 = hxc_l_pickupIndex;
              struct hxc_caxecraft_domain_AuthoredAquaticEquipmentResult hxc_l_tmp_instance_call_result_n501 = hxc_caxecraft_domain_GameSession_collectAuthoredAquaticEquipment(hxc_l_tmp_load_result_n498, hxc_l_tmp_load_result_n499, hxc_l_replacement);
              struct hxc_caxecraft_domain_AuthoredAquaticEquipmentResult hxc_l_equipment = hxc_l_tmp_instance_call_result_n501;
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
          struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n509 = hxc_l_session;
          hxc_l_gc_roots[55] = (const void *)hxc_l_tmp_load_result_n509;
          if (hxc_l_tmp_load_result_n509 == NULL)
          {
            abort();
          }
          int32_t hxc_l_tmp_record_field_load_result_n510 = hxc_l_gameTick.hxc_tickIndex;
          struct hxc_caxecraft_domain_ActorControllerTickResult hxc_l_tmp_instance_call_result_n512 = hxc_caxecraft_domain_GameSession_stepAuthoredActorControllers(hxc_l_tmp_load_result_n509, hxc_l_tmp_record_field_load_result_n510, hxc_l_damagePolicy);
          struct hxc_caxecraft_domain_ActorControllerTickResult hxc_l_actorTick = hxc_l_tmp_instance_call_result_n512;
          switch (hxc_l_actorTick.hxc_status.hxc_tag) {
            case hxc_caxecraft_domain_ActorControllerTickStatus_ControllersAdvanced:
              {
                hxc_l_guideInteractionAvailable = false;
                int32_t hxc_l_g_h3dd44276de3c = 0;
                struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n514 = hxc_l_session;
                hxc_l_gc_roots[59] = (const void *)hxc_l_tmp_load_result_n514;
                if (hxc_l_tmp_load_result_n514 == NULL)
                {
                  abort();
                }
                struct hxc_array_ref *hxc_l_tmp_instance_call_result_n515 = hxc_caxecraft_domain_GameSession_actorControllerEventSnapshots(hxc_l_tmp_load_result_n514);
                hxc_l_g1_hecc87e72e1d5 = hxc_l_tmp_instance_call_result_n515;
                while (1)
                {
                  int32_t hxc_l_tmp_load_result_n516 = hxc_l_g_h3dd44276de3c;
                  int32_t hxc_l_tmp_array_length_result_n518;
                  if (hxc_array_ref_length(hxc_l_g1_hecc87e72e1d5, &hxc_l_tmp_array_length_result_n518) != HXC_STATUS_OK)
                  {
                    abort();
                  }
                  if (!(hxc_l_tmp_load_result_n516 < hxc_l_tmp_array_length_result_n518))
                  {
                    break;
                  }
                  struct hxc_array_ref *hxc_l_tmp_load_result_n519 = hxc_l_g1_hecc87e72e1d5;
                  struct hxc_caxecraft_domain_ActorControllerEvent hxc_l_tmp_array_get_result_n521;
                  if (hxc_array_ref_get_copy(hxc_l_tmp_load_result_n519, (size_t)hxc_l_g_h3dd44276de3c, &hxc_l_tmp_array_get_result_n521) != HXC_STATUS_OK)
                  {
                    abort();
                  }
                  hxc_l_tmp_array_element_owner_n211 = hxc_l_tmp_array_get_result_n521;
                  hxc_l_event = hxc_l_tmp_array_element_owner_n211;
                  if (hxc_enum_8b2a26b8_retain(&hxc_l_event) != HXC_STATUS_OK)
                  {
                    abort();
                  }
                  hxc_l_g_h3dd44276de3c = hxc_i32_add_wrapping(hxc_l_g_h3dd44276de3c, 1);
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
                        int32_t hxc_l_tmp_enum_payload_project_n526 = hxc_l_event.hxc_payload.hxc_InteractionAvailable.hxc_characterId;
                        int32_t hxc_l_source_h1de633615c68 = hxc_l_tmp_enum_payload_project_n526;
                        int32_t hxc_l_tmp_load_result_n527 = hxc_l_source_h1de633615c68;
                        if (hxc_l_tmp_load_result_n527 == hxc_l_dialogueActorId)
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
                        int32_t hxc_l_tmp_enum_payload_project_n530 = hxc_l_event.hxc_payload.hxc_LocalPlayerAttack.hxc_characterId;
                        int32_t hxc_l_source_h949e74edee1a = hxc_l_tmp_enum_payload_project_n530;
                        int32_t hxc_l_tmp_load_result_n531 = hxc_l_source_h949e74edee1a;
                        if (hxc_l_tmp_load_result_n531 == hxc_l_enemyActorId)
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
                        int32_t hxc_l_tmp_enum_payload_project_n534 = hxc_l_event.hxc_payload.hxc_DropRequested.hxc_characterId;
                        int32_t hxc_l_source_h5a71a337f260 = hxc_l_tmp_enum_payload_project_n534;
                        if (hxc_l_event.hxc_tag != hxc_caxecraft_domain_ActorControllerEvent_DropRequested)
                        {
                          abort();
                        }
                        hxc_string hxc_l_tmp_enum_payload_project_n536 = hxc_l_event.hxc_payload.hxc_DropRequested.hxc_drop;
                        hxc_l_drop = hxc_l_tmp_enum_payload_project_n536;
                        if (hxc_string_retain(hxc_l_drop) != HXC_STATUS_OK)
                        {
                          abort();
                        }
                        int32_t hxc_l_tmp_call_result_n538 = hxc_caxecraft_content_BaseContentPack_dropQuantityById(hxc_l_drop);
                        int32_t hxc_l_quantity = hxc_l_tmp_call_result_n538;
                        struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n539 = hxc_l_session;
                        hxc_l_gc_roots[60] = (const void *)hxc_l_tmp_load_result_n539;
                        if (hxc_l_tmp_load_result_n539 == NULL)
                        {
                          abort();
                        }
                        struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n541 = hxc_caxecraft_domain_GameSession_readCharacter(hxc_l_tmp_load_result_n539, hxc_l_source_h5a71a337f260);
                        struct hxc_caxecraft_domain_Character hxc_l_defeatedActor = hxc_l_tmp_instance_call_result_n541;
                        int32_t hxc_l_tmp_load_result_n542 = hxc_l_quantity;
                        bool hxc_l_tmp_short_circuit_result_n218 = hxc_l_tmp_load_result_n542 <= 0;
                        if (!(hxc_l_tmp_load_result_n542 <= 0))
                        {
                          hxc_l_tmp_short_circuit_result_n218 = hxc_l_defeatedActor.hxc_id <= 0;
                        }
                        if (hxc_l_tmp_short_circuit_result_n218)
                        {
                          hxc_l_quit = true;
                        }
                        else
                        {
                          if (!hxc_l_berryDrop.hxc_active)
                          {
                            struct hxc_caxecraft_domain_Character hxc_l_tmp_load_result_n546 = hxc_l_defeatedActor;
                            struct hxc_caxecraft_gameplay_BerryDropState hxc_l_tmp_call_result_n548 = hxc_caxecraft_gameplay_BerryDrop_fromDefeatedCharacter(hxc_l_tmp_load_result_n546, hxc_l_quantity);
                            hxc_l_berryDrop = hxc_l_tmp_call_result_n548;
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
                  hxc_array_43e9f8ca_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n211);
                }
                if (hxc_array_ref_release(hxc_l_g1_hecc87e72e1d5) != HXC_STATUS_OK)
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
        struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n549 = hxc_l_session;
        hxc_l_gc_roots[56] = (const void *)hxc_l_tmp_load_result_n549;
        if (hxc_l_tmp_load_result_n549 == NULL)
        {
          abort();
        }
        struct hxc_array_ref *hxc_l_tmp_instance_call_result_n550 = hxc_caxecraft_domain_GameSession_actorControllerStateSnapshots(hxc_l_tmp_load_result_n549);
        hxc_l_actorPhases = hxc_l_tmp_instance_call_result_n550;
        struct hxc_array_ref *hxc_l_tmp_load_result_n551 = hxc_l_actorPhases;
        struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation hxc_l_tmp_call_result_n554 = hxc_caxecraft_app_CaxecraftApp_observeActorPhase(hxc_l_tmp_load_result_n551, hxc_l_dialogueActorId, hxc_caxecraft_domain_ActorControllerPhase_Stationary);
        hxc_l_dialoguePhase = hxc_l_tmp_call_result_n554;
        struct hxc_array_ref *hxc_l_tmp_load_result_n555 = hxc_l_actorPhases;
        struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation hxc_l_tmp_call_result_n558 = hxc_caxecraft_app_CaxecraftApp_observeActorPhase(hxc_l_tmp_load_result_n555, hxc_l_enemyActorId, hxc_caxecraft_domain_ActorControllerPhase_Defeated);
        hxc_l_enemyPhase = hxc_l_tmp_call_result_n558;
        struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n559 = hxc_l_session;
        hxc_l_gc_roots[57] = (const void *)hxc_l_tmp_load_result_n559;
        if (hxc_l_tmp_load_result_n559 == NULL)
        {
          abort();
        }
        struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n561 = hxc_caxecraft_domain_GameSession_readCharacter(hxc_l_tmp_load_result_n559, hxc_l_dialogueActorId);
        hxc_l_dialogueActor = hxc_l_tmp_instance_call_result_n561;
        struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n562 = hxc_l_session;
        hxc_l_gc_roots[58] = (const void *)hxc_l_tmp_load_result_n562;
        if (hxc_l_tmp_load_result_n562 == NULL)
        {
          abort();
        }
        struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n564 = hxc_caxecraft_domain_GameSession_readCharacter(hxc_l_tmp_load_result_n562, hxc_l_enemyActorId);
        hxc_l_enemyActor = hxc_l_tmp_instance_call_result_n564;
        bool hxc_l_tmp_record_field_load_result_n565 = hxc_l_dialoguePhase.hxc_valid;
        bool hxc_l_tmp_short_circuit_result_n220 = !hxc_l_tmp_record_field_load_result_n565;
        if (!!hxc_l_tmp_record_field_load_result_n565)
        {
          hxc_l_tmp_short_circuit_result_n220 = !hxc_l_enemyPhase.hxc_valid;
        }
        bool hxc_l_tmp_short_circuit_load_result_n567 = hxc_l_tmp_short_circuit_result_n220;
        bool hxc_l_tmp_short_circuit_result_n221 = hxc_l_tmp_short_circuit_load_result_n567;
        if (!hxc_l_tmp_short_circuit_load_result_n567)
        {
          hxc_l_tmp_short_circuit_result_n221 = hxc_l_dialogueActor.hxc_id <= 0;
        }
        bool hxc_l_tmp_short_circuit_load_result_n569 = hxc_l_tmp_short_circuit_result_n221;
        bool hxc_l_tmp_short_circuit_result_n222 = hxc_l_tmp_short_circuit_load_result_n569;
        if (!hxc_l_tmp_short_circuit_load_result_n569)
        {
          hxc_l_tmp_short_circuit_result_n222 = hxc_l_enemyActor.hxc_id <= 0;
        }
        if (hxc_l_tmp_short_circuit_result_n222)
        {
          hxc_l_quit = true;
        }
        struct hxc_caxecraft_gameplay_SwordCombatState hxc_l_tmp_call_result_n573 = hxc_caxecraft_gameplay_SwordCombat_step(hxc_l_swordCombat);
        hxc_l_swordCombat = hxc_l_tmp_call_result_n573;
        if (hxc_l_swordQueued)
        {
          struct hxc_caxecraft_gameplay_SwordCombatState hxc_l_tmp_load_result_n575 = hxc_l_swordCombat;
          struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_load_result_n576 = hxc_l_inventory;
          struct hxc_caxecraft_domain_VitalsState hxc_l_tmp_record_field_load_result_n577 = hxc_l_character.hxc_vitals;
          struct hxc_caxecraft_domain_Character hxc_l_tmp_load_result_n578 = hxc_l_enemyActor;
          double hxc_l_tmp_record_field_load_result_n579 = hxc_l_character.hxc_body.hxc_x;
          double hxc_l_tmp_record_field_load_result_n580 = hxc_l_character.hxc_body.hxc_z;
          double hxc_l_tmp_load_result_n581 = hxc_l_lookX;
          int32_t hxc_l_tmp_call_result_n583 = hxc_caxecraft_gameplay_SwordCombat_decide(hxc_l_tmp_load_result_n575, hxc_l_tmp_load_result_n576, hxc_l_tmp_record_field_load_result_n577, hxc_l_tmp_load_result_n578, hxc_l_tmp_record_field_load_result_n579, hxc_l_tmp_record_field_load_result_n580, hxc_l_tmp_load_result_n581, hxc_l_lookZ);
          int32_t hxc_l_swordDecision = hxc_l_tmp_call_result_n583;
          if (hxc_l_swordDecision == 5)
          {
            struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n585 = hxc_l_session;
            hxc_l_gc_roots[61] = (const void *)hxc_l_tmp_load_result_n585;
            if (hxc_l_tmp_load_result_n585 == NULL)
            {
              abort();
            }
            struct hxc_caxecraft_domain_CharacterDamageResult hxc_l_tmp_instance_call_result_n587 = hxc_caxecraft_domain_GameSession_damageCharacter(hxc_l_tmp_load_result_n585, hxc_l_enemyActorId, 1);
            struct hxc_caxecraft_domain_CharacterDamageResult hxc_l_damage = hxc_l_tmp_instance_call_result_n587;
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
          int32_t hxc_l_tmp_load_result_n591 = hxc_l_swordDecision;
          struct hxc_caxecraft_gameplay_SwordCombatState hxc_l_tmp_call_result_n593 = hxc_caxecraft_gameplay_SwordCombat_after(hxc_l_tmp_load_result_n591, hxc_l_swordCombat);
          hxc_l_swordCombat = hxc_l_tmp_call_result_n593;
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
    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n598 = hxc_l_session;
    hxc_l_gc_roots[51] = (const void *)hxc_l_tmp_load_result_n598;
    if (hxc_l_tmp_load_result_n598 == NULL)
    {
      abort();
    }
    size_t hxc_l_tmp_length_n600;
    const uint8_t *hxc_l_tmp_instance_call_result_n599 = hxc_caxecraft_domain_GameSession_worldView(hxc_l_tmp_load_result_n598, &hxc_l_tmp_length_n600);
    double hxc_l_tmp_load_result_n601 = hxc_l_selectionEyeX;
    double hxc_l_tmp_load_result_n602 = hxc_l_selectionEyeY;
    double hxc_l_tmp_load_result_n603 = hxc_l_selectionEyeZ;
    double hxc_l_tmp_load_result_n604 = hxc_l_lookX;
    double hxc_l_tmp_load_result_n605 = hxc_l_lookY;
    struct hxc_caxecraft_domain_RaycastHit hxc_l_tmp_call_result_n607 = hxc_caxecraft_domain_VoxelRaycast_trace(hxc_l_tmp_instance_call_result_n599, hxc_l_tmp_length_n600, hxc_l_tmp_load_result_n601, hxc_l_tmp_load_result_n602, hxc_l_tmp_load_result_n603, hxc_l_tmp_load_result_n604, hxc_l_tmp_load_result_n605, hxc_l_lookZ, 7.0);
    struct hxc_caxecraft_domain_RaycastHit hxc_l_hit = hxc_l_tmp_call_result_n607;
    bool hxc_l_tmp_load_result_n608 = hxc_l_captured_h58da63bbdd3c;
    bool hxc_l_tmp_short_circuit_result_n229 = hxc_l_tmp_load_result_n608;
    if (hxc_l_tmp_load_result_n608)
    {
      hxc_l_tmp_short_circuit_result_n229 = !hxc_l_recapturedThisFrame;
    }
    bool hxc_l_tmp_short_circuit_load_result_n610 = hxc_l_tmp_short_circuit_result_n229;
    bool hxc_l_tmp_short_circuit_result_n230 = hxc_l_tmp_short_circuit_load_result_n610;
    if (hxc_l_tmp_short_circuit_load_result_n610)
    {
      hxc_l_tmp_short_circuit_result_n230 = hxc_l_primaryPressed_hddf6382a4cfd;
    }
    if (hxc_l_tmp_short_circuit_result_n230)
    {
      if (hxc_l_character.hxc_vitals.hxc_health > 0)
      {
        if (hxc_l_selectedMode == 1)
        {
          bool hxc_l_tmp_call_result_n617 = hxc_caxecraft_gameplay_Inventory_selectedIs(hxc_l_inventory, hxc_caxecraft_gameplay_ItemKind_CopperSword);
          bool hxc_l_tmp_short_circuit_result_n231 = !hxc_l_tmp_call_result_n617;
          if (!hxc_l_tmp_call_result_n617)
          {
            hxc_l_tmp_short_circuit_result_n231 = hxc_l_hit.hxc_hit;
          }
          bool hxc_l_tmp_short_circuit_load_result_n619 = hxc_l_tmp_short_circuit_result_n231;
          bool hxc_l_tmp_short_circuit_result_n232 = hxc_l_tmp_short_circuit_load_result_n619;
          if (hxc_l_tmp_short_circuit_load_result_n619)
          {
            struct hxc_caxecraft_domain_AquaticState hxc_l_tmp_record_field_load_result_n620 = hxc_l_character.hxc_aquatic;
            bool hxc_l_tmp_call_result_n622 = hxc_caxecraft_domain_Aquatics_canMine(hxc_l_tmp_record_field_load_result_n620, hxc_l_character.hxc_aquaticProfile);
            hxc_l_tmp_short_circuit_result_n232 = hxc_l_tmp_call_result_n622;
          }
          if (hxc_l_tmp_short_circuit_result_n232)
          {
            int32_t hxc_l_tmp_record_field_load_result_n624 = hxc_l_hit.hxc_cellX;
            int32_t hxc_l_tmp_record_field_load_result_n625 = hxc_l_hit.hxc_cellY;
            struct hxc_caxecraft_scenario_VoxelPoint hxc_l_tmp_call_result_n627 = hxc_caxecraft_domain_World_coord(hxc_l_tmp_record_field_load_result_n624, hxc_l_tmp_record_field_load_result_n625, hxc_l_hit.hxc_cellZ);
            struct hxc_caxecraft_scenario_VoxelPoint hxc_l_minedCoordinate = hxc_l_tmp_call_result_n627;
            struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n628 = hxc_l_session;
            hxc_l_gc_roots[62] = (const void *)hxc_l_tmp_load_result_n628;
            if (hxc_l_tmp_load_result_n628 == NULL)
            {
              abort();
            }
            struct hxc_caxecraft_scenario_VoxelPoint hxc_l_tmp_load_result_n629 = hxc_l_minedCoordinate;
            struct hxc_caxecraft_gameplay_MiningResult hxc_l_tmp_instance_call_result_n631 = hxc_caxecraft_domain_GameSession_mineTerrain(hxc_l_tmp_load_result_n628, hxc_l_tmp_load_result_n629, hxc_l_inventory);
            struct hxc_caxecraft_gameplay_MiningResult hxc_l_mining = hxc_l_tmp_instance_call_result_n631;
            hxc_l_inventory = hxc_l_mining.hxc_inventory;
            if (hxc_l_mining.hxc_outcome == 2)
            {
              if (hxc_l_self == NULL)
              {
                abort();
              }
              struct hxc_caxecraft_app_TerrainRenderer *hxc_l_tmp_owned_class_field_address_n634 = &(*hxc_l_self).hxc_terrainRenderer;
              struct hxc_caxecraft_app_TerrainChunkCache *hxc_l_tmp_owned_class_field_address_n635 = &(*hxc_l_tmp_owned_class_field_address_n634).hxc_cache;
              if (hxc_l_tmp_owned_class_field_address_n635 == NULL)
              {
                abort();
              }
              hxc_caxecraft_app_TerrainChunkCache_invalidate(hxc_l_tmp_owned_class_field_address_n635, hxc_l_minedCoordinate);
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
            int32_t hxc_l_tmp_record_field_load_result_n639 = hxc_l_hit.hxc_cellX;
            int32_t hxc_l_tmp_record_field_load_result_n640 = hxc_l_hit.hxc_cellY;
            struct hxc_caxecraft_scenario_VoxelPoint hxc_l_tmp_call_result_n642 = hxc_caxecraft_domain_World_coord(hxc_l_tmp_record_field_load_result_n639, hxc_l_tmp_record_field_load_result_n640, hxc_l_hit.hxc_cellZ);
            struct hxc_caxecraft_scenario_VoxelPoint hxc_l_removedCoordinate = hxc_l_tmp_call_result_n642;
            struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n643 = hxc_l_session;
            hxc_l_gc_roots[63] = (const void *)hxc_l_tmp_load_result_n643;
            if (hxc_l_tmp_load_result_n643 == NULL)
            {
              abort();
            }
            bool hxc_l_tmp_instance_call_result_n645 = hxc_caxecraft_domain_GameSession_removeTerrain(hxc_l_tmp_load_result_n643, hxc_l_removedCoordinate);
            bool hxc_l_removed = hxc_l_tmp_instance_call_result_n645;
            if (hxc_l_removed)
            {
              if (hxc_l_self == NULL)
              {
                abort();
              }
              struct hxc_caxecraft_app_TerrainRenderer *hxc_l_tmp_owned_class_field_address_n647 = &(*hxc_l_self).hxc_terrainRenderer;
              struct hxc_caxecraft_app_TerrainChunkCache *hxc_l_tmp_owned_class_field_address_n648 = &(*hxc_l_tmp_owned_class_field_address_n647).hxc_cache;
              if (hxc_l_tmp_owned_class_field_address_n648 == NULL)
              {
                abort();
              }
              hxc_caxecraft_app_TerrainChunkCache_invalidate(hxc_l_tmp_owned_class_field_address_n648, hxc_l_removedCoordinate);
            }
          }
        }
      }
    }
    bool hxc_l_tmp_load_result_n650 = hxc_l_captured_h58da63bbdd3c;
    bool hxc_l_tmp_short_circuit_result_n237 = hxc_l_tmp_load_result_n650;
    if (hxc_l_tmp_load_result_n650)
    {
      hxc_l_tmp_short_circuit_result_n237 = hxc_l_secondaryPressed_h5a095466600c;
    }
    if (hxc_l_tmp_short_circuit_result_n237)
    {
      if (hxc_l_character.hxc_vitals.hxc_health > 0)
      {
        struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n654 = hxc_l_session;
        hxc_l_gc_roots[64] = (const void *)hxc_l_tmp_load_result_n654;
        if (hxc_l_tmp_load_result_n654 == NULL)
        {
          abort();
        }
        struct hxc_caxecraft_domain_LocalRecoveryResult hxc_l_tmp_instance_call_result_n656 = hxc_caxecraft_domain_GameSession_useSelectedRecovery(hxc_l_tmp_load_result_n654, hxc_l_inventory);
        struct hxc_caxecraft_domain_LocalRecoveryResult hxc_l_recovery = hxc_l_tmp_instance_call_result_n656;
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
              int32_t hxc_l_tmp_record_field_load_result_n663 = hxc_l_hit.hxc_previousX;
              int32_t hxc_l_tmp_record_field_load_result_n664 = hxc_l_hit.hxc_previousY;
              struct hxc_caxecraft_scenario_VoxelPoint hxc_l_tmp_call_result_n666 = hxc_caxecraft_domain_World_coord(hxc_l_tmp_record_field_load_result_n663, hxc_l_tmp_record_field_load_result_n664, hxc_l_hit.hxc_previousZ);
              struct hxc_caxecraft_scenario_VoxelPoint hxc_l_placement = hxc_l_tmp_call_result_n666;
              enum hxc_caxecraft_domain_BlockKind hxc_l_tmp_call_result_n668 = hxc_caxecraft_gameplay_Inventory_selectedBlock(hxc_l_inventory);
              enum hxc_caxecraft_domain_BlockKind hxc_l_selectedBlock = hxc_l_tmp_call_result_n668;
              struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_load_result_n669 = hxc_l_inventory;
              int32_t hxc_l_tmp_call_result_n671 = hxc_caxecraft_gameplay_Inventory_countAt(hxc_l_tmp_load_result_n669, hxc_l_inventory.hxc_selected);
              bool hxc_l_hasItem = hxc_l_tmp_call_result_n671 > 0;
              bool hxc_l_tmp_load_result_n672 = hxc_l_hasItem;
              bool hxc_l_tmp_short_circuit_result_n242 = !hxc_l_tmp_load_result_n672;
              if (!!hxc_l_tmp_load_result_n672)
              {
                bool hxc_l_tmp_call_result_n674 = hxc_caxecraft_domain_World_isPlaceable(hxc_l_selectedBlock);
                hxc_l_tmp_short_circuit_result_n242 = !hxc_l_tmp_call_result_n674;
              }
              bool hxc_l_tmp_short_circuit_load_result_n675 = hxc_l_tmp_short_circuit_result_n242;
              bool hxc_l_tmp_short_circuit_result_n243 = hxc_l_tmp_short_circuit_load_result_n675;
              if (!hxc_l_tmp_short_circuit_load_result_n675)
              {
                struct hxc_caxecraft_domain_CharacterBody hxc_l_tmp_record_field_load_result_n676 = hxc_l_character.hxc_body;
                bool hxc_l_tmp_call_result_n678 = hxc_caxecraft_domain_CharacterPhysics_canPlaceAt(hxc_l_tmp_record_field_load_result_n676, hxc_l_placement);
                hxc_l_tmp_short_circuit_result_n243 = !hxc_l_tmp_call_result_n678;
              }
              bool hxc_l_tmp_short_circuit_load_result_n679 = hxc_l_tmp_short_circuit_result_n243;
              bool hxc_l_tmp_short_circuit_result_n244 = hxc_l_tmp_short_circuit_load_result_n679;
              if (!hxc_l_tmp_short_circuit_load_result_n679)
              {
                struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n680 = hxc_l_session;
                hxc_l_gc_roots[65] = (const void *)hxc_l_tmp_load_result_n680;
                if (hxc_l_tmp_load_result_n680 == NULL)
                {
                  abort();
                }
                struct hxc_caxecraft_scenario_VoxelPoint hxc_l_tmp_load_result_n681 = hxc_l_placement;
                bool hxc_l_tmp_instance_call_result_n683 = hxc_caxecraft_domain_GameSession_placeTerrain(hxc_l_tmp_load_result_n680, hxc_l_tmp_load_result_n681, hxc_l_selectedBlock);
                hxc_l_tmp_short_circuit_result_n244 = !hxc_l_tmp_instance_call_result_n683;
              }
              if (hxc_l_tmp_short_circuit_result_n244)
              {
                hxc_l_placementBlockedFrames = 60;
              }
              else
              {
                if (hxc_l_self == NULL)
                {
                  abort();
                }
                struct hxc_caxecraft_app_TerrainRenderer *hxc_l_tmp_owned_class_field_address_n685 = &(*hxc_l_self).hxc_terrainRenderer;
                struct hxc_caxecraft_app_TerrainChunkCache *hxc_l_tmp_owned_class_field_address_n686 = &(*hxc_l_tmp_owned_class_field_address_n685).hxc_cache;
                if (hxc_l_tmp_owned_class_field_address_n686 == NULL)
                {
                  abort();
                }
                hxc_caxecraft_app_TerrainChunkCache_invalidate(hxc_l_tmp_owned_class_field_address_n686, hxc_l_placement);
                if (hxc_l_selectedMode == 1)
                {
                  struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n690 = hxc_caxecraft_gameplay_Inventory_consumeSelected(hxc_l_inventory);
                  hxc_l_inventory = hxc_l_tmp_call_result_n690;
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
    if (!hxc_l_paused_he6d3ac7c8a8b)
    {
      struct hxc_caxecraft_gameplay_BerryDropState hxc_l_tmp_load_result_n694 = hxc_l_berryDrop;
      double hxc_l_tmp_record_field_load_result_n695 = hxc_l_character.hxc_body.hxc_x;
      double hxc_l_tmp_record_field_load_result_n696 = hxc_l_character.hxc_body.hxc_y;
      bool hxc_l_tmp_call_result_n698 = hxc_caxecraft_gameplay_BerryDrop_isInRange(hxc_l_tmp_load_result_n694, hxc_l_tmp_record_field_load_result_n695, hxc_l_tmp_record_field_load_result_n696, hxc_l_character.hxc_body.hxc_z);
      if (hxc_l_tmp_call_result_n698)
      {
        struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_load_result_n699 = hxc_l_inventory;
        int32_t hxc_l_tmp_call_result_n702 = hxc_caxecraft_gameplay_Inventory_acceptedAmount(hxc_l_tmp_load_result_n699, hxc_caxecraft_gameplay_ItemKind_Berries, hxc_l_berryDrop.hxc_amount);
        int32_t hxc_l_acceptedDrop = hxc_l_tmp_call_result_n702;
        if (hxc_l_acceptedDrop > 0)
        {
          struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_load_result_n704 = hxc_l_inventory;
          struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n707 = hxc_caxecraft_gameplay_Inventory_collectItem(hxc_l_tmp_load_result_n704, hxc_caxecraft_gameplay_ItemKind_Berries, hxc_l_acceptedDrop);
          hxc_l_inventory = hxc_l_tmp_call_result_n707;
          struct hxc_caxecraft_gameplay_BerryDropState hxc_l_tmp_load_result_n708 = hxc_l_berryDrop;
          struct hxc_caxecraft_gameplay_BerryDropState hxc_l_tmp_call_result_n710 = hxc_caxecraft_gameplay_BerryDrop_collectAmount(hxc_l_tmp_load_result_n708, hxc_l_acceptedDrop);
          hxc_l_berryDrop = hxc_l_tmp_call_result_n710;
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
    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n727 = hxc_l_session;
    hxc_l_gc_roots[66] = (const void *)hxc_l_tmp_load_result_n727;
    if (hxc_l_tmp_load_result_n727 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_domain_GameView hxc_l_tmp_instance_call_result_n728 = hxc_caxecraft_domain_GameSession_view(hxc_l_tmp_load_result_n727);
    struct hxc_caxecraft_domain_GameView hxc_l_committedView = hxc_l_tmp_instance_call_result_n728;
    if (!hxc_l_committedView.hxc_valid)
    {
      hxc_l_quit = true;
    }
    else
    {
      hxc_l_character = hxc_l_committedView.hxc_localPlayer;
    }
    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n731 = hxc_l_session;
    hxc_l_gc_roots[67] = (const void *)hxc_l_tmp_load_result_n731;
    if (hxc_l_tmp_load_result_n731 == NULL)
    {
      abort();
    }
    bool hxc_l_tmp_instance_call_result_n733 = hxc_caxecraft_domain_GameSession_actorInteractionAvailable(hxc_l_tmp_load_result_n731, hxc_l_dialogueActorId);
    hxc_l_guideInteractionAvailable = hxc_l_tmp_instance_call_result_n733;
    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n734 = hxc_l_session;
    hxc_l_gc_roots[68] = (const void *)hxc_l_tmp_load_result_n734;
    if (hxc_l_tmp_load_result_n734 == NULL)
    {
      abort();
    }
    struct hxc_array_ref *hxc_l_tmp_instance_call_result_n735 = hxc_caxecraft_domain_GameSession_actorControllerStateSnapshots(hxc_l_tmp_load_result_n734);
    hxc_l_presentationActorPhases = hxc_l_tmp_instance_call_result_n735;
    struct hxc_array_ref *hxc_l_tmp_load_result_n736 = hxc_l_presentationActorPhases;
    struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation hxc_l_tmp_call_result_n739 = hxc_caxecraft_app_CaxecraftApp_observeActorPhase(hxc_l_tmp_load_result_n736, hxc_l_dialogueActorId, hxc_caxecraft_domain_ActorControllerPhase_Stationary);
    hxc_l_dialoguePhase = hxc_l_tmp_call_result_n739;
    struct hxc_array_ref *hxc_l_tmp_load_result_n740 = hxc_l_presentationActorPhases;
    struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation hxc_l_tmp_call_result_n743 = hxc_caxecraft_app_CaxecraftApp_observeActorPhase(hxc_l_tmp_load_result_n740, hxc_l_enemyActorId, hxc_caxecraft_domain_ActorControllerPhase_Defeated);
    hxc_l_enemyPhase = hxc_l_tmp_call_result_n743;
    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n744 = hxc_l_session;
    hxc_l_gc_roots[69] = (const void *)hxc_l_tmp_load_result_n744;
    if (hxc_l_tmp_load_result_n744 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n746 = hxc_caxecraft_domain_GameSession_readCharacter(hxc_l_tmp_load_result_n744, hxc_l_dialogueActorId);
    hxc_l_dialogueActor = hxc_l_tmp_instance_call_result_n746;
    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n747 = hxc_l_session;
    hxc_l_gc_roots[70] = (const void *)hxc_l_tmp_load_result_n747;
    if (hxc_l_tmp_load_result_n747 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n749 = hxc_caxecraft_domain_GameSession_readCharacter(hxc_l_tmp_load_result_n747, hxc_l_enemyActorId);
    hxc_l_enemyActor = hxc_l_tmp_instance_call_result_n749;
    bool hxc_l_tmp_record_field_load_result_n750 = hxc_l_dialoguePhase.hxc_valid;
    bool hxc_l_tmp_short_circuit_result_n248 = !hxc_l_tmp_record_field_load_result_n750;
    if (!!hxc_l_tmp_record_field_load_result_n750)
    {
      hxc_l_tmp_short_circuit_result_n248 = !hxc_l_enemyPhase.hxc_valid;
    }
    bool hxc_l_tmp_short_circuit_load_result_n752 = hxc_l_tmp_short_circuit_result_n248;
    bool hxc_l_tmp_short_circuit_result_n249 = hxc_l_tmp_short_circuit_load_result_n752;
    if (!hxc_l_tmp_short_circuit_load_result_n752)
    {
      hxc_l_tmp_short_circuit_result_n249 = hxc_l_dialogueActor.hxc_id <= 0;
    }
    bool hxc_l_tmp_short_circuit_load_result_n754 = hxc_l_tmp_short_circuit_result_n249;
    bool hxc_l_tmp_short_circuit_result_n250 = hxc_l_tmp_short_circuit_load_result_n754;
    if (!hxc_l_tmp_short_circuit_load_result_n754)
    {
      hxc_l_tmp_short_circuit_result_n250 = hxc_l_enemyActor.hxc_id <= 0;
    }
    if (hxc_l_tmp_short_circuit_result_n250)
    {
      hxc_l_quit = true;
    }
    int32_t hxc_l_completedTicks = hxc_l_committedView.hxc_completedTicks;
    if (hxc_l_resetMotionThisFrame)
    {
      struct hxc_caxecraft_app_MotionHistory hxc_l_tmp_call_result_n760 = hxc_caxecraft_app_MotionInterpolation_start(hxc_l_character.hxc_body);
      hxc_l_motionHistory = hxc_l_tmp_call_result_n760;
    }
    struct hxc_caxecraft_app_MotionHistory hxc_l_tmp_load_result_n761 = hxc_l_motionHistory;
    struct hxc_caxecraft_app_RenderPosition hxc_l_tmp_call_result_n763 = hxc_caxecraft_app_MotionInterpolation_sample(hxc_l_tmp_load_result_n761, hxc_l_accumulator, 0.05);
    struct hxc_caxecraft_app_RenderPosition hxc_l_renderPosition = hxc_l_tmp_call_result_n763;
    double hxc_l_eyeX = hxc_l_renderPosition.hxc_x;
    double hxc_l_eyeY = hxc_l_renderPosition.hxc_y + 1.62;
    double hxc_l_eyeZ = hxc_l_renderPosition.hxc_z;
    double hxc_l_tmp_load_result_n767 = hxc_l_eyeX;
    double hxc_l_tmp_load_result_n768 = hxc_l_eyeY;
    double hxc_l_tmp_load_result_n769 = hxc_l_eyeZ;
    double hxc_l_tmp_load_result_n771 = hxc_l_eyeX;
    double hxc_l_tmp_load_result_n772 = hxc_l_lookX;
    double hxc_l_tmp_load_result_n773 = hxc_l_eyeY;
    double hxc_l_tmp_load_result_n774 = hxc_l_lookY;
    double hxc_l_tmp_load_result_n775 = hxc_l_eyeZ;
    struct Camera3D hxc_l_camera = (struct Camera3D){ .position = (struct Vector3){ .x = (float)hxc_l_tmp_load_result_n767, .y = (float)hxc_l_tmp_load_result_n768, .z = (float)hxc_l_tmp_load_result_n769 }, .target = (struct Vector3){ .x = (float)(hxc_l_tmp_load_result_n771 + hxc_l_tmp_load_result_n772), .y = (float)(hxc_l_tmp_load_result_n773 + hxc_l_tmp_load_result_n774), .z = (float)(hxc_l_tmp_load_result_n775 + hxc_l_lookZ) }, .up = (struct Vector3){ .x = (float)0.0, .y = (float)1.0, .z = (float)0.0 }, .fovy = (float)70.0, .projection = 0 };
    BeginDrawing();
    if (hxc_l_onTitle)
    {
      int32_t hxc_l_tmp_native_call_result_n781 = GetScreenWidth();
      int32_t hxc_l_width_h833d4f4070a0 = (int32_t)hxc_l_tmp_native_call_result_n781;
      int32_t hxc_l_tmp_native_call_result_n782 = GetScreenHeight();
      int32_t hxc_l_height_h4525b9049d1f = (int32_t)hxc_l_tmp_native_call_result_n782;
      if (hxc_l_titleTextureReady)
      {
        struct Color hxc_l_this1_hb3575ed36a6c = (struct Color){ .r = 255, .g = 255, .b = 255, .a = 255 };
        struct Texture hxc_l_tmp_load_result_n785 = hxc_l_titleTexture;
        double hxc_l_imageAspect = hxc_f64_divide_zero_safe((double)(int32_t)hxc_l_tmp_load_result_n785.width, (double)(int32_t)hxc_l_titleTexture.height);
        int32_t hxc_l_tmp_load_result_n789 = hxc_l_width_h833d4f4070a0;
        double hxc_l_screenAspect = hxc_f64_divide_zero_safe((double)hxc_l_tmp_load_result_n789, (double)hxc_l_height_h4525b9049d1f);
        double hxc_l_sourceX = 0.0;
        double hxc_l_sourceY = 0.0;
        double hxc_l_sourceWidth = (double)(int32_t)hxc_l_titleTexture.width;
        double hxc_l_sourceHeight = (double)(int32_t)hxc_l_titleTexture.height;
        double hxc_l_tmp_load_result_n795 = hxc_l_screenAspect;
        if (hxc_l_tmp_load_result_n795 > hxc_l_imageAspect)
        {
          struct Texture hxc_l_tmp_load_result_n797 = hxc_l_titleTexture;
          hxc_l_sourceHeight = hxc_f64_divide_zero_safe((double)(int32_t)hxc_l_tmp_load_result_n797.width, hxc_l_screenAspect);
          struct Texture hxc_l_tmp_load_result_n800 = hxc_l_titleTexture;
          hxc_l_sourceY = ((double)(int32_t)hxc_l_tmp_load_result_n800.height - hxc_l_sourceHeight) * 0.5;
        }
        else
        {
          struct Texture hxc_l_tmp_load_result_n803 = hxc_l_titleTexture;
          hxc_l_sourceWidth = (double)(int32_t)hxc_l_tmp_load_result_n803.height * hxc_l_screenAspect;
          struct Texture hxc_l_tmp_load_result_n806 = hxc_l_titleTexture;
          hxc_l_sourceX = ((double)(int32_t)hxc_l_tmp_load_result_n806.width - hxc_l_sourceWidth) * 0.5;
        }
        struct Texture hxc_l_tmp_load_result_n809 = hxc_l_titleTexture;
        double hxc_l_tmp_load_result_n810 = hxc_l_sourceX;
        double hxc_l_tmp_load_result_n811 = hxc_l_sourceY;
        double hxc_l_tmp_load_result_n812 = hxc_l_sourceWidth;
        double hxc_l_tmp_load_result_n813 = hxc_l_sourceHeight;
        int32_t hxc_l_tmp_load_result_n815 = hxc_l_width_h833d4f4070a0;
        int32_t hxc_l_tmp_load_result_n816 = hxc_l_height_h4525b9049d1f;
        DrawTexturePro(hxc_l_tmp_load_result_n809, (struct Rectangle){ .x = (float)hxc_l_tmp_load_result_n810, .y = (float)hxc_l_tmp_load_result_n811, .width = (float)hxc_l_tmp_load_result_n812, .height = (float)hxc_l_tmp_load_result_n813 }, (struct Rectangle){ .x = (float)0.0, .y = (float)0.0, .width = (float)((double)hxc_l_tmp_load_result_n815 + 0.0), .height = (float)((double)hxc_l_tmp_load_result_n816 + 0.0) }, (struct Vector2){ .x = (float)0.0, .y = (float)0.0 }, (float)0.0, hxc_l_this1_hb3575ed36a6c);
      }
      else
      {
        ClearBackground((struct Color){ .r = 126, .g = 190, .b = 201, .a = 255 });
      }
      struct Color hxc_l_this1_h7e17f9d9a961 = (struct Color){ .r = 4, .g = 14, .b = 23, .a = 132 };
      int32_t hxc_l_tmp_load_result_n822 = hxc_l_width_h833d4f4070a0;
      int32_t hxc_l_tmp_load_result_n823 = hxc_l_height_h4525b9049d1f;
      DrawRectangle((int32_t)0, (int32_t)0, (int32_t)hxc_l_tmp_load_result_n822, (int32_t)hxc_l_tmp_load_result_n823, hxc_l_this1_h7e17f9d9a961);
      if (hxc_l_wordmarkTextureReady)
      {
        int32_t hxc_l_centerX = hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_l_width_h833d4f4070a0, (double)2));
        struct Color hxc_l_this1_hd7c19fc43bbf = (struct Color){ .r = 255, .g = 255, .b = 255, .a = 255 };
        double hxc_l_scale = hxc_f64_divide_zero_safe((double)560, (double)(int32_t)hxc_l_wordmarkTexture.width);
        double hxc_l_heightScale = hxc_f64_divide_zero_safe((double)132, (double)(int32_t)hxc_l_wordmarkTexture.height);
        double hxc_l_tmp_load_result_n832 = hxc_l_heightScale;
        if (hxc_l_tmp_load_result_n832 < hxc_l_scale)
        {
          hxc_l_scale = hxc_l_heightScale;
        }
        struct Texture hxc_l_tmp_load_result_n835 = hxc_l_wordmarkTexture;
        double hxc_l_width_h1ee04e2636c6 = (double)(int32_t)hxc_l_tmp_load_result_n835.width * hxc_l_scale;
        struct Texture hxc_l_tmp_load_result_n838 = hxc_l_wordmarkTexture;
        double hxc_l_height_h12a64c1baf46 = (double)(int32_t)hxc_l_tmp_load_result_n838.height * hxc_l_scale;
        double hxc_l_width1 = (double)(int32_t)hxc_l_wordmarkTexture.width + 0.0;
        double hxc_l_height1 = (double)(int32_t)hxc_l_wordmarkTexture.height + 0.0;
        struct Texture hxc_l_tmp_load_result_n845 = hxc_l_wordmarkTexture;
        double hxc_l_tmp_load_result_n846 = hxc_l_width1;
        double hxc_l_tmp_load_result_n847 = hxc_l_height1;
        int32_t hxc_l_tmp_load_result_n849 = hxc_l_centerX;
        double hxc_l_tmp_load_result_n850 = hxc_l_width_h1ee04e2636c6;
        double hxc_l_tmp_load_result_n851 = hxc_l_width_h1ee04e2636c6;
        double hxc_l_tmp_load_result_n852 = hxc_l_height_h12a64c1baf46;
        DrawTexturePro(hxc_l_tmp_load_result_n845, (struct Rectangle){ .x = (float)0.0, .y = (float)0.0, .width = (float)hxc_l_tmp_load_result_n846, .height = (float)hxc_l_tmp_load_result_n847 }, (struct Rectangle){ .x = (float)((double)hxc_l_tmp_load_result_n849 - hxc_l_tmp_load_result_n850 * 0.5), .y = (float)28.0, .width = (float)hxc_l_tmp_load_result_n851, .height = (float)hxc_l_tmp_load_result_n852 }, (struct Vector2){ .x = (float)0.0, .y = (float)0.0 }, (float)0.0, hxc_l_this1_hd7c19fc43bbf);
      }
      else
      {
        int32_t hxc_l_x_h11977b11b784 = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_l_width_h833d4f4070a0, (double)2)), 92);
        struct Color hxc_l_this1_hed080d0e4477 = (struct Color){ .r = 242, .g = 249, .b = 245, .a = 255 };
        const char *hxc_l_tmp_call_result_n859 = hxc_caxecraft_localization_UiCatalog_text(hxc_l_locale, 34);
        int32_t hxc_l_tmp_load_result_n860 = hxc_l_x_h11977b11b784;
        DrawText(hxc_l_tmp_call_result_n859, (int32_t)hxc_l_tmp_load_result_n860, (int32_t)52, (int32_t)34, hxc_l_this1_hed080d0e4477);
      }
      int32_t hxc_l_firstTop = hxc_i32_subtract_wrapping(hxc_l_height_h4525b9049d1f, 293);
      int32_t hxc_l_left_h7cda3f81af09 = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_l_width_h833d4f4070a0, (double)2)), 180);
      if (hxc_l_selectedMode == 0)
      {
        struct Color hxc_l_this1_h1fb0774a93f2 = (struct Color){ .r = 16, .g = 88, .b = 102, .a = 232 };
        int32_t hxc_l_tmp_load_result_n866 = hxc_l_left_h7cda3f81af09;
        int32_t hxc_l_tmp_load_result_n867 = hxc_l_firstTop;
        DrawRectangle((int32_t)hxc_l_tmp_load_result_n866, (int32_t)hxc_l_tmp_load_result_n867, (int32_t)360, (int32_t)54, hxc_l_this1_h1fb0774a93f2);
        struct Color hxc_l_this1_h7a7a8360f29e = (struct Color){ .r = 255, .g = 190, .b = 55, .a = 255 };
        int32_t hxc_l_tmp_load_result_n870 = hxc_l_left_h7cda3f81af09;
        int32_t hxc_l_tmp_load_result_n871 = hxc_l_firstTop;
        DrawRectangleLines((int32_t)hxc_l_tmp_load_result_n870, (int32_t)hxc_l_tmp_load_result_n871, (int32_t)360, (int32_t)54, hxc_l_this1_h7a7a8360f29e);
      }
      else
      {
        struct Color hxc_l_this1_h432df9a6585d = (struct Color){ .r = 6, .g = 26, .b = 36, .a = 218 };
        int32_t hxc_l_tmp_load_result_n874 = hxc_l_left_h7cda3f81af09;
        int32_t hxc_l_tmp_load_result_n875 = hxc_l_firstTop;
        DrawRectangle((int32_t)hxc_l_tmp_load_result_n874, (int32_t)hxc_l_tmp_load_result_n875, (int32_t)360, (int32_t)54, hxc_l_this1_h432df9a6585d);
        struct Color hxc_l_this1_h9f7bc1fe91c6 = (struct Color){ .r = 92, .g = 194, .b = 188, .a = 255 };
        int32_t hxc_l_tmp_load_result_n878 = hxc_l_left_h7cda3f81af09;
        int32_t hxc_l_tmp_load_result_n879 = hxc_l_firstTop;
        DrawRectangleLines((int32_t)hxc_l_tmp_load_result_n878, (int32_t)hxc_l_tmp_load_result_n879, (int32_t)360, (int32_t)54, hxc_l_this1_h9f7bc1fe91c6);
      }
      struct Color hxc_l_this1_he6dc64b070aa = (struct Color){ .r = 242, .g = 249, .b = 245, .a = 255 };
      const char *hxc_l_tmp_call_result_n883 = hxc_caxecraft_localization_UiCatalog_text(hxc_l_locale, 27);
      int32_t hxc_l_tmp_load_result_n884 = hxc_l_left_h7cda3f81af09;
      int32_t hxc_l_tmp_load_result_n885 = hxc_l_firstTop;
      DrawText(hxc_l_tmp_call_result_n883, (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n884, 28), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n885, 16), (int32_t)21, hxc_l_this1_he6dc64b070aa);
      int32_t hxc_l_top_hd2bc341829aa = hxc_i32_add_wrapping(hxc_i32_add_wrapping(hxc_l_firstTop, 54), 14);
      int32_t hxc_l_left_h73313b38a952 = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_l_width_h833d4f4070a0, (double)2)), 180);
      if (hxc_l_selectedMode == 1)
      {
        struct Color hxc_l_this1_hc8acdb0c5150 = (struct Color){ .r = 16, .g = 88, .b = 102, .a = 232 };
        int32_t hxc_l_tmp_load_result_n891 = hxc_l_left_h73313b38a952;
        int32_t hxc_l_tmp_load_result_n892 = hxc_l_top_hd2bc341829aa;
        DrawRectangle((int32_t)hxc_l_tmp_load_result_n891, (int32_t)hxc_l_tmp_load_result_n892, (int32_t)360, (int32_t)54, hxc_l_this1_hc8acdb0c5150);
        struct Color hxc_l_this1_h0ea78289bc18 = (struct Color){ .r = 255, .g = 190, .b = 55, .a = 255 };
        int32_t hxc_l_tmp_load_result_n895 = hxc_l_left_h73313b38a952;
        int32_t hxc_l_tmp_load_result_n896 = hxc_l_top_hd2bc341829aa;
        DrawRectangleLines((int32_t)hxc_l_tmp_load_result_n895, (int32_t)hxc_l_tmp_load_result_n896, (int32_t)360, (int32_t)54, hxc_l_this1_h0ea78289bc18);
      }
      else
      {
        struct Color hxc_l_this1_hf9d520b367bb = (struct Color){ .r = 6, .g = 26, .b = 36, .a = 218 };
        int32_t hxc_l_tmp_load_result_n899 = hxc_l_left_h73313b38a952;
        int32_t hxc_l_tmp_load_result_n900 = hxc_l_top_hd2bc341829aa;
        DrawRectangle((int32_t)hxc_l_tmp_load_result_n899, (int32_t)hxc_l_tmp_load_result_n900, (int32_t)360, (int32_t)54, hxc_l_this1_hf9d520b367bb);
        struct Color hxc_l_this1_he661ad3f00e6 = (struct Color){ .r = 92, .g = 194, .b = 188, .a = 255 };
        int32_t hxc_l_tmp_load_result_n903 = hxc_l_left_h73313b38a952;
        int32_t hxc_l_tmp_load_result_n904 = hxc_l_top_hd2bc341829aa;
        DrawRectangleLines((int32_t)hxc_l_tmp_load_result_n903, (int32_t)hxc_l_tmp_load_result_n904, (int32_t)360, (int32_t)54, hxc_l_this1_he661ad3f00e6);
      }
      struct Color hxc_l_this1_hb94710a96b12 = (struct Color){ .r = 242, .g = 249, .b = 245, .a = 255 };
      const char *hxc_l_tmp_call_result_n908 = hxc_caxecraft_localization_UiCatalog_text(hxc_l_locale, 26);
      int32_t hxc_l_tmp_load_result_n909 = hxc_l_left_h73313b38a952;
      int32_t hxc_l_tmp_load_result_n910 = hxc_l_top_hd2bc341829aa;
      DrawText(hxc_l_tmp_call_result_n908, (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n909, 28), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n910, 16), (int32_t)21, hxc_l_this1_hb94710a96b12);
      int32_t hxc_l_top_hb18965590851 = hxc_i32_add_wrapping(hxc_l_firstTop, 136);
      int32_t hxc_l_left_hb420e7aaef10 = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_l_width_h833d4f4070a0, (double)2)), 180);
      struct Color hxc_l_this1_he7d320013f59 = (struct Color){ .r = 6, .g = 26, .b = 36, .a = 218 };
      int32_t hxc_l_tmp_load_result_n915 = hxc_l_left_hb420e7aaef10;
      int32_t hxc_l_tmp_load_result_n916 = hxc_l_top_hb18965590851;
      DrawRectangle((int32_t)hxc_l_tmp_load_result_n915, (int32_t)hxc_l_tmp_load_result_n916, (int32_t)360, (int32_t)54, hxc_l_this1_he7d320013f59);
      struct Color hxc_l_this1_h6992162311e4 = (struct Color){ .r = 92, .g = 194, .b = 188, .a = 255 };
      int32_t hxc_l_tmp_load_result_n919 = hxc_l_left_hb420e7aaef10;
      int32_t hxc_l_tmp_load_result_n920 = hxc_l_top_hb18965590851;
      DrawRectangleLines((int32_t)hxc_l_tmp_load_result_n919, (int32_t)hxc_l_tmp_load_result_n920, (int32_t)360, (int32_t)54, hxc_l_this1_h6992162311e4);
      struct Color hxc_l_this1_hc8279b6afe3b = (struct Color){ .r = 242, .g = 249, .b = 245, .a = 255 };
      const char *hxc_l_tmp_call_result_n924 = hxc_caxecraft_localization_UiCatalog_text(hxc_l_locale, 28);
      int32_t hxc_l_tmp_load_result_n925 = hxc_l_left_hb420e7aaef10;
      int32_t hxc_l_tmp_load_result_n926 = hxc_l_top_hb18965590851;
      DrawText(hxc_l_tmp_call_result_n924, (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n925, 28), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n926, 16), (int32_t)21, hxc_l_this1_hc8279b6afe3b);
      int32_t hxc_l_x_hc3c94e3c9782 = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_l_width_h833d4f4070a0, (double)2)), 285);
      struct Color hxc_l_this1_h1986656be165 = (struct Color){ .r = 229, .g = 241, .b = 235, .a = 255 };
      const char *hxc_l_tmp_call_result_n931 = hxc_caxecraft_localization_UiCatalog_text(hxc_l_locale, 29);
      int32_t hxc_l_tmp_load_result_n932 = hxc_l_x_hc3c94e3c9782;
      int32_t hxc_l_tmp_load_result_n933 = hxc_l_height_h4525b9049d1f;
      DrawText(hxc_l_tmp_call_result_n931, (int32_t)hxc_l_tmp_load_result_n932, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n933, 58), (int32_t)16, hxc_l_this1_h1986656be165);
      if (hxc_l_selectedMode == 1)
      {
        int32_t hxc_l_x_ha5add8bc578e = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_l_width_h833d4f4070a0, (double)2)), 230);
        struct Color hxc_l_this1_haf43176cbb21 = (struct Color){ .r = 255, .g = 205, .b = 91, .a = 255 };
        const char *hxc_l_tmp_call_result_n939 = hxc_caxecraft_localization_FirstPlayableCatalog_text(hxc_l_locale, 1);
        int32_t hxc_l_tmp_load_result_n940 = hxc_l_x_ha5add8bc578e;
        int32_t hxc_l_tmp_load_result_n941 = hxc_l_firstTop;
        DrawText(hxc_l_tmp_call_result_n939, (int32_t)hxc_l_tmp_load_result_n940, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n941, 36), (int32_t)17, hxc_l_this1_haf43176cbb21);
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
        struct hxc_caxecraft_app_CaxecraftEditorScreen *hxc_l_tmp_class_field_load_result_n944 = (*hxc_l_self).hxc_editorScreen;
        hxc_l_gc_roots[71] = (const void *)hxc_l_tmp_class_field_load_result_n944;
        if (hxc_l_tmp_class_field_load_result_n944 == NULL)
        {
          abort();
        }
        int32_t hxc_l_tmp_load_result_n945 = hxc_l_locale;
        enum hxc_caxecraft_app_EditorScreenAction hxc_l_tmp_instance_call_result_n947 = hxc_caxecraft_app_CaxecraftEditorScreen_draw(hxc_l_tmp_class_field_load_result_n944, hxc_l_tmp_load_result_n945, hxc_l_editorNavigationCommand);
        if (hxc_l_tmp_instance_call_result_n947 == hxc_caxecraft_app_EditorScreenAction_ReturnToTitle)
        {
          enum hxc_caxecraft_app_AppScreen hxc_l_tmp_call_result_n950 = hxc_caxecraft_app_AppScreen_closeEditor(hxc_l_screen);
          hxc_l_screen = hxc_l_tmp_call_result_n950;
        }
      }
      else
      {
        ClearBackground((struct Color){ .r = 126, .g = 190, .b = 201, .a = 255 });
        int32_t hxc_l_tmp_native_call_result_n952 = GetScreenWidth();
        int32_t hxc_l_sunX = hxc_i32_subtract_wrapping((int32_t)hxc_l_tmp_native_call_result_n952, 260);
        float hxc_l_radius_h9b4b7569fa70 = (float)42.0;
        struct Color hxc_l_this1_h6cfd7f210e8b = (struct Color){ .r = 255, .g = 226, .b = 136, .a = 255 };
        int32_t hxc_l_tmp_load_result_n954 = hxc_l_sunX;
        float hxc_l_tmp_load_result_n955 = hxc_l_radius_h9b4b7569fa70;
        DrawCircle((int32_t)hxc_l_tmp_load_result_n954, (int32_t)86, hxc_l_tmp_load_result_n955, hxc_l_this1_h6cfd7f210e8b);
        float hxc_l_radius_h0834185051b5 = (float)30.0;
        struct Color hxc_l_this1_h5c298dbe73e0 = (struct Color){ .r = 255, .g = 246, .b = 205, .a = 255 };
        int32_t hxc_l_tmp_load_result_n958 = hxc_l_sunX;
        float hxc_l_tmp_load_result_n959 = hxc_l_radius_h0834185051b5;
        DrawCircle((int32_t)hxc_l_tmp_load_result_n958, (int32_t)86, hxc_l_tmp_load_result_n959, hxc_l_this1_h5c298dbe73e0);
        BeginMode3D(hxc_l_camera);
        if (hxc_l_self == NULL)
        {
          abort();
        }
        struct hxc_caxecraft_app_TerrainRenderer *hxc_l_tmp_owned_class_field_address_n962 = &(*hxc_l_self).hxc_terrainRenderer;
        if (hxc_l_tmp_owned_class_field_address_n962 == NULL)
        {
          abort();
        }
        struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n963 = hxc_l_session;
        hxc_l_gc_roots[72] = (const void *)hxc_l_tmp_load_result_n963;
        if (hxc_l_tmp_load_result_n963 == NULL)
        {
          abort();
        }
        size_t hxc_l_tmp_length_n965;
        const uint8_t *hxc_l_tmp_instance_call_result_n964 = hxc_caxecraft_domain_GameSession_worldView(hxc_l_tmp_load_result_n963, &hxc_l_tmp_length_n965);
        struct Texture hxc_l_tmp_load_result_n966 = hxc_l_terrainTexture;
        bool hxc_l_tmp_load_result_n967 = hxc_l_terrainTextureReady;
        struct Texture hxc_l_tmp_load_result_n968 = hxc_l_adventureTerrainTexture;
        bool hxc_l_tmp_load_result_n969 = hxc_l_adventureTerrainTextureReady;
        double hxc_l_tmp_record_field_load_result_n970 = hxc_l_renderPosition.hxc_x;
        struct hxc_caxecraft_app_TerrainRenderCounters hxc_l_tmp_instance_call_result_n972 = hxc_caxecraft_app_TerrainRenderer_draw(hxc_l_tmp_owned_class_field_address_n962, hxc_l_tmp_instance_call_result_n964, hxc_l_tmp_length_n965, hxc_l_tmp_load_result_n966, hxc_l_tmp_load_result_n967, hxc_l_tmp_load_result_n968, hxc_l_tmp_load_result_n969, hxc_l_tmp_record_field_load_result_n970, hxc_l_renderPosition.hxc_z);
        struct hxc_caxecraft_app_TerrainRenderCounters hxc_l_renderCounters = hxc_l_tmp_instance_call_result_n972;
        struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n973 = hxc_l_session;
        hxc_l_gc_roots[73] = (const void *)hxc_l_tmp_load_result_n973;
        if (hxc_l_tmp_load_result_n973 == NULL)
        {
          abort();
        }
        size_t hxc_l_tmp_length_n975;
        const uint8_t *hxc_l_tmp_instance_call_result_n974 = hxc_caxecraft_domain_GameSession_worldView(hxc_l_tmp_load_result_n973, &hxc_l_tmp_length_n975);
        struct Texture hxc_l_tmp_load_result_n976 = hxc_l_terrainTexture;
        bool hxc_l_tmp_load_result_n977 = hxc_l_terrainTextureReady;
        struct hxc_caxecraft_app_WaterRenderCounters hxc_l_tmp_call_result_n979 = hxc_caxecraft_app_WaterRenderer_draw(hxc_l_tmp_instance_call_result_n974, hxc_l_tmp_length_n975, hxc_l_tmp_load_result_n976, hxc_l_tmp_load_result_n977, hxc_l_waterPresentationCell);
        struct hxc_caxecraft_app_WaterRenderCounters hxc_l_waterCounters = hxc_l_tmp_call_result_n979;
        int32_t hxc_l_tmp_record_field_load_result_n980 = hxc_l_renderCounters.hxc_visible;
        int32_t hxc_l_totalVisible = hxc_i32_add_wrapping(hxc_l_tmp_record_field_load_result_n980, hxc_l_waterCounters.hxc_visible);
        int32_t hxc_l_tmp_record_field_load_result_n982 = hxc_l_renderCounters.hxc_drawCalls;
        int32_t hxc_l_totalDrawCalls = hxc_i32_add_wrapping(hxc_l_tmp_record_field_load_result_n982, hxc_l_waterCounters.hxc_drawCalls);
        struct Camera3D hxc_l_tmp_load_result_n984 = hxc_l_camera;
        struct Texture hxc_l_tmp_load_result_n985 = hxc_l_entityTexture;
        bool hxc_l_tmp_load_result_n986 = hxc_l_entityTextureReady;
        struct hxc_caxecraft_domain_Character hxc_l_tmp_load_result_n987 = hxc_l_dialogueActor;
        struct hxc_caxecraft_domain_Character hxc_l_tmp_load_result_n988 = hxc_l_enemyActor;
        enum hxc_caxecraft_domain_ActorControllerPhase hxc_l_tmp_record_field_load_result_n989 = hxc_l_enemyPhase.hxc_phase;
        hxc_caxecraft_app_CaxecraftApp_drawActors(hxc_l_tmp_load_result_n984, hxc_l_tmp_load_result_n985, hxc_l_tmp_load_result_n986, hxc_l_tmp_load_result_n987, hxc_l_tmp_load_result_n988, hxc_l_tmp_record_field_load_result_n989, hxc_l_berryDrop);
        struct Camera3D hxc_l_tmp_load_result_n991 = hxc_l_camera;
        struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n992 = hxc_l_session;
        hxc_l_gc_roots[74] = (const void *)hxc_l_tmp_load_result_n992;
        if (hxc_l_tmp_load_result_n992 == NULL)
        {
          abort();
        }
        size_t hxc_l_tmp_length_n994;
        const int32_t *hxc_l_tmp_instance_call_result_n993 = hxc_caxecraft_domain_GameSession_authoredItemsView(hxc_l_tmp_load_result_n992, &hxc_l_tmp_length_n994);
        struct hxc_array_ref *hxc_l_tmp_load_result_n995 = hxc_l_loadedItems;
        struct Texture hxc_l_tmp_load_result_n996 = hxc_l_itemTexture;
        bool hxc_l_tmp_load_result_n997 = hxc_l_itemTextureReady;
        struct Texture hxc_l_tmp_load_result_n998 = hxc_l_adventureItemTexture;
        hxc_caxecraft_app_AuthoredItemRenderer_drawWorldItems(hxc_l_tmp_load_result_n991, hxc_l_tmp_instance_call_result_n993, hxc_l_tmp_length_n994, hxc_l_tmp_load_result_n995, hxc_l_tmp_load_result_n996, hxc_l_tmp_load_result_n997, hxc_l_tmp_load_result_n998, hxc_l_adventureItemTextureReady);
        if (hxc_l_hit.hxc_hit)
        {
          double hxc_l_y = (double)hxc_l_hit.hxc_cellY + 0.5;
          double hxc_l_z = (double)hxc_l_hit.hxc_cellZ + 0.5;
          int32_t hxc_l_tmp_record_field_load_result_n1003 = hxc_l_hit.hxc_cellX;
          double hxc_l_tmp_load_result_n1004 = hxc_l_y;
          DrawCubeWires((struct Vector3){ .x = (float)((double)hxc_l_tmp_record_field_load_result_n1003 + 0.5), .y = (float)hxc_l_tmp_load_result_n1004, .z = (float)hxc_l_z }, (float)1.04, (float)1.04, (float)1.04, (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 });
        }
        EndMode3D();
        if (hxc_l_cameraWaterBlend > 0.0)
        {
          int32_t hxc_l_overlayAlpha = hxc_f64_to_i32_saturating(105.0 * hxc_l_cameraWaterBlend);
          int32_t hxc_l_tmp_native_call_result_n1010 = GetScreenWidth();
          int32_t hxc_l_width_h2cf47b6d0360 = (int32_t)hxc_l_tmp_native_call_result_n1010;
          int32_t hxc_l_tmp_native_call_result_n1011 = GetScreenHeight();
          int32_t hxc_l_height_h547947dc2c95 = (int32_t)hxc_l_tmp_native_call_result_n1011;
          uint8_t hxc_l_red = (uint8_t)31;
          uint8_t hxc_l_green = (uint8_t)115;
          uint8_t hxc_l_blue = (uint8_t)154;
          int32_t hxc_l_tmp_load_result_n1012 = hxc_l_overlayAlpha;
          int32_t hxc_l_tmp_conditional_result_n318 = 0;
          if (hxc_l_tmp_load_result_n1012 < 0)
          {
            hxc_l_tmp_conditional_result_n318 = 0;
          }
          else
          {
            int32_t hxc_l_tmp_load_result_n1013 = hxc_l_overlayAlpha;
            int32_t hxc_l_tmp_conditional_result_n319 = 0;
            if (hxc_l_tmp_load_result_n1013 > 255)
            {
              hxc_l_tmp_conditional_result_n319 = 255;
            }
            else
            {
              hxc_l_tmp_conditional_result_n319 = hxc_l_overlayAlpha;
            }
            hxc_l_tmp_conditional_result_n318 = hxc_l_tmp_conditional_result_n319;
          }
          uint8_t hxc_l_alpha = (uint8_t)hxc_l_tmp_conditional_result_n318;
          uint8_t hxc_l_tmp_load_result_n1017 = hxc_l_red;
          uint8_t hxc_l_tmp_load_result_n1018 = hxc_l_green;
          uint8_t hxc_l_tmp_load_result_n1019 = hxc_l_blue;
          struct Color hxc_l_this1_hdbad86337d55 = (struct Color){ .r = hxc_l_tmp_load_result_n1017, .g = hxc_l_tmp_load_result_n1018, .b = hxc_l_tmp_load_result_n1019, .a = hxc_l_alpha };
          int32_t hxc_l_tmp_load_result_n1022 = hxc_l_width_h2cf47b6d0360;
          int32_t hxc_l_tmp_load_result_n1023 = hxc_l_height_h547947dc2c95;
          DrawRectangle((int32_t)0, (int32_t)0, (int32_t)hxc_l_tmp_load_result_n1022, (int32_t)hxc_l_tmp_load_result_n1023, hxc_l_this1_hdbad86337d55);
        }
        int32_t hxc_l_tmp_load_result_n1025 = hxc_l_totalVisible;
        int32_t hxc_l_tmp_load_result_n1026 = hxc_l_totalDrawCalls;
        int32_t hxc_l_tmp_load_result_n1027 = hxc_l_frameCount;
        int32_t hxc_l_tmp_load_result_n1028 = hxc_l_completedTicks;
        double hxc_l_tmp_record_field_load_result_n1030 = hxc_l_character.hxc_body.hxc_x;
        double hxc_l_tmp_record_field_load_result_n1031 = hxc_l_character.hxc_body.hxc_z;
        struct hxc_caxecraft_domain_VitalsState hxc_l_tmp_record_field_load_result_n1032 = hxc_l_character.hxc_vitals;
        int32_t hxc_l_tmp_load_result_n1033 = hxc_l_aquaticEquipmentCode;
        int32_t hxc_l_tmp_load_result_n1034 = hxc_l_aquaticEquipmentFrames;
        bool hxc_l_tmp_record_field_load_result_n1035 = hxc_l_character.hxc_aquatic.hxc_headSubmerged;
        int32_t hxc_l_tmp_record_field_load_result_n1036 = hxc_l_character.hxc_aquatic.hxc_breathTicks;
        int32_t hxc_l_tmp_record_field_load_result_n1037 = hxc_l_character.hxc_aquaticProfile.hxc_maximumBreathTicks;
        int32_t hxc_l_tmp_load_result_n1039 = hxc_l_placementBlockedFrames;
        int32_t hxc_l_tmp_load_result_n1040 = hxc_l_strikeHitFrames;
        int32_t hxc_l_tmp_load_result_n1041 = hxc_l_enemyDefeatedFrames;
        int32_t hxc_l_tmp_load_result_n1042 = hxc_l_enemyAttackFrames;
        int32_t hxc_l_tmp_load_result_n1043 = hxc_l_pickupFrames;
        int32_t hxc_l_tmp_load_result_n1044 = hxc_l_pickupAmount;
        int32_t hxc_l_tmp_load_result_n1045 = hxc_l_inventoryFullReason;
        int32_t hxc_l_tmp_load_result_n1046 = hxc_l_recoveryFeedback;
        int32_t hxc_l_tmp_load_result_n1047 = hxc_l_recoveryFeedbackFrames;
        bool hxc_l_tmp_load_result_n1049 = hxc_l_paused_he6d3ac7c8a8b;
        bool hxc_l_tmp_load_result_n1050 = hxc_l_captured_h58da63bbdd3c;
        struct hxc_caxecraft_domain_RaycastHit hxc_l_tmp_load_result_n1051 = hxc_l_hit;
        int32_t hxc_l_tmp_load_result_n1052 = hxc_l_selectedMode;
        int32_t hxc_l_tmp_load_result_n1053 = hxc_l_locale;
        struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_load_result_n1054 = hxc_l_inventory;
        enum hxc_caxecraft_gameplay_GuidePhase hxc_l_tmp_load_result_n1055 = hxc_l_guidePhase;
        bool hxc_l_tmp_load_result_n1056 = hxc_l_guideInteractionAvailable;
        struct hxc_caxecraft_domain_Character hxc_l_tmp_load_result_n1057 = hxc_l_enemyActor;
        struct hxc_caxecraft_app_HudView hxc_l_hudView = (struct hxc_caxecraft_app_HudView){ .hxc_character = (struct hxc_caxecraft_app_HudCharacterView){ .hxc_aquaticEquipmentCode = hxc_l_tmp_load_result_n1033, .hxc_aquaticEquipmentVisible = hxc_l_tmp_load_result_n1034 > 0, .hxc_breathTicks = hxc_l_tmp_record_field_load_result_n1036, .hxc_headSubmerged = hxc_l_tmp_record_field_load_result_n1035, .hxc_maximumBreathTicks = hxc_l_tmp_record_field_load_result_n1037, .hxc_vitals = hxc_l_tmp_record_field_load_result_n1032, .hxc_x = hxc_l_tmp_record_field_load_result_n1030, .hxc_z = hxc_l_tmp_record_field_load_result_n1031 }, .hxc_enemy = hxc_l_tmp_load_result_n1057, .hxc_enemyPhase = hxc_l_enemyPhase.hxc_phase, .hxc_feedback = (struct hxc_caxecraft_app_HudFeedbackView){ .hxc_enemyAttacked = hxc_l_tmp_load_result_n1042 > 0, .hxc_enemyDefeated = hxc_l_tmp_load_result_n1041 > 0, .hxc_inventoryFullReason = hxc_l_tmp_load_result_n1045, .hxc_pickedUp = hxc_l_tmp_load_result_n1043 > 0, .hxc_pickupAmount = hxc_l_tmp_load_result_n1044, .hxc_placementBlocked = hxc_l_tmp_load_result_n1039 > 0, .hxc_recoveryDecision = hxc_l_tmp_load_result_n1046, .hxc_recoveryVisible = hxc_l_tmp_load_result_n1047 > 0, .hxc_strikeHit = hxc_l_tmp_load_result_n1040 > 0 }, .hxc_guideInteractionAvailable = hxc_l_tmp_load_result_n1056, .hxc_guidePhase = hxc_l_tmp_load_result_n1055, .hxc_hit = hxc_l_tmp_load_result_n1051, .hxc_inventory = hxc_l_tmp_load_result_n1054, .hxc_locale = hxc_l_tmp_load_result_n1053, .hxc_metrics = (struct hxc_caxecraft_app_HudMetricsView){ .hxc_completedTicks = hxc_l_tmp_load_result_n1028, .hxc_drawCalls = hxc_l_tmp_load_result_n1026, .hxc_renderedFrames = hxc_l_tmp_load_result_n1027, .hxc_visibleBlocks = hxc_l_tmp_load_result_n1025 }, .hxc_mode = hxc_l_tmp_load_result_n1052, .hxc_paused = hxc_l_tmp_load_result_n1049, .hxc_pointerCaptured = hxc_l_tmp_load_result_n1050 };
        struct hxc_caxecraft_app_HudView hxc_l_tmp_load_result_n1060 = hxc_l_hudView;
        hxc_caxecraft_app_CaxecraftApp_drawHud(hxc_l_tmp_load_result_n1060, hxc_l_hudResources);
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
  hxc_array_1f383c02_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n29);
  if (hxc_array_ref_release(hxc_l_fluidPresentation) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_record_2bd8b029_destroy(&hxc_l_tmp_record_field_transform_receiver_owner_n26);
  if (hxc_array_ref_release(hxc_l_g1_h16b7ad605ace) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_ref_release(hxc_l_loadedItems) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_ref_release(hxc_l_g1_hb98ea081892b) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_ref_release(hxc_l_actorBindings) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_enum_be0a6f10_destroy(&hxc_l_g_h14daa0fd57c5);
  hxc_enum_21bc9bf3_destroy(&hxc_l_tmp_static_call_argument_0_owner_n9);
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return;
}
