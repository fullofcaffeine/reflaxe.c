#include "hxc/program.h"

void hxc_compiler_constructor_caxecraft_app_CaxecraftApp(struct hxc_caxecraft_app_CaxecraftApp *hxc_self)
{
  const void *hxc_gc_roots[3] = { (const void *)hxc_self, NULL, NULL };
  struct hxc_gc_root_frame hxc_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_gc_roots, 3, &hxc_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_caxecraft_app_TerrainRenderer *hxc_tmp_owned_class_field_address_n0 = &(*hxc_self).hxc_terrainRenderer;
  hxc_compiler_constructor_caxecraft_app_TerrainRenderer(hxc_tmp_owned_class_field_address_n0);
  struct hxc_caxecraft_domain_GameSession *hxc_tmp_owned_class_field_address_n1 = &(*hxc_self).hxc_session;
  hxc_compiler_constructor_caxecraft_domain_GameSession(hxc_tmp_owned_class_field_address_n1);
  struct hxc_caxecraft_app_CaxecraftEditorScreen *hxc_tmp_managed_class_result_n2 = NULL;
  if (hxc_gc_allocate(&hxc_program_gc, &hxc_caxecraft_app_CaxecraftEditorScreen_descriptor, (void **)&hxc_tmp_managed_class_result_n2) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_gc_roots[1] = (const void *)hxc_tmp_managed_class_result_n2;
  hxc_compiler_constructor_caxecraft_app_CaxecraftEditorScreen(hxc_tmp_managed_class_result_n2);
  hxc_gc_roots[2] = (const void *)hxc_tmp_managed_class_result_n2;
  (*hxc_self).hxc_editorScreen = hxc_tmp_managed_class_result_n2;
  struct hxc_caxecraft_input_NavigationRepeater *hxc_tmp_owned_class_field_address_n3 = &(*hxc_self).hxc_editorNavigation;
  hxc_compiler_constructor_caxecraft_input_NavigationRepeater(hxc_tmp_owned_class_field_address_n3);
  if (hxc_gc_root_frame_pop(&hxc_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return;
}

enum hxc_caxecraft_gameplay_GuidePhase hxc_caxecraft_app_CaxecraftApp_advanceGuidePhase(enum hxc_caxecraft_gameplay_GuidePhase hxc_phase)
{
  switch (hxc_phase) {
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
  }
  return hxc_caxecraft_gameplay_GuidePhase_Welcomed;
}

void hxc_caxecraft_app_CaxecraftApp_drawActors(struct Camera3D hxc_camera, struct Texture hxc_entityTexture, bool hxc_entityTextureReady, struct hxc_caxecraft_domain_Character hxc_guide, struct hxc_caxecraft_domain_Character hxc_enemy, enum hxc_caxecraft_domain_ActorControllerPhase hxc_enemyPhase, struct hxc_caxecraft_gameplay_BerryDropState hxc_berryDrop)
{
  if (hxc_entityTextureReady)
  {
    double hxc_y_h22fd73ca09d1 = hxc_guide.hxc_body.hxc_y + 0.76;
    double hxc_z_hbe2b70802617 = hxc_guide.hxc_body.hxc_z;
    double hxc_tmp_load_result_n7 = hxc_y_h22fd73ca09d1;
    hxc_caxecraft_app_CaxecraftAtlas_drawWorldSprite(hxc_camera, hxc_entityTexture, hxc_caxecraft_app_WorldSprite_NiaFront, (struct Vector3){ .x = (float)hxc_guide.hxc_body.hxc_x, .y = (float)hxc_tmp_load_result_n7, .z = (float)hxc_z_hbe2b70802617 }, 0.95, 1.52);
  }
  else
  {
    double hxc_y_h3e1c9c7adcd4 = hxc_guide.hxc_body.hxc_y + 0.54;
    double hxc_z_h7b1ea64e0691 = hxc_guide.hxc_body.hxc_z;
    double hxc_tmp_load_result_n16 = hxc_y_h3e1c9c7adcd4;
    DrawCube((struct Vector3){ .x = (float)hxc_guide.hxc_body.hxc_x, .y = (float)hxc_tmp_load_result_n16, .z = (float)hxc_z_h7b1ea64e0691 }, (float)0.50, (float)0.86, (float)0.42, (struct Color){ .r = 42, .g = 150, .b = 160, .a = 255 });
    double hxc_y_h2562ce14c359 = hxc_guide.hxc_body.hxc_y + 1.18;
    double hxc_z_h5435af74f1f7 = hxc_guide.hxc_body.hxc_z;
    double hxc_tmp_load_result_n26 = hxc_y_h2562ce14c359;
    DrawCube((struct Vector3){ .x = (float)hxc_guide.hxc_body.hxc_x, .y = (float)hxc_tmp_load_result_n26, .z = (float)hxc_z_h5435af74f1f7 }, (float)0.44, (float)0.44, (float)0.44, (struct Color){ .r = 205, .g = 139, .b = 88, .a = 255 });
    double hxc_y_h99e36220b049 = hxc_guide.hxc_body.hxc_y + 1.41;
    double hxc_z_h6c58eb1f3d93 = hxc_guide.hxc_body.hxc_z;
    double hxc_tmp_load_result_n36 = hxc_y_h99e36220b049;
    DrawCube((struct Vector3){ .x = (float)hxc_guide.hxc_body.hxc_x, .y = (float)hxc_tmp_load_result_n36, .z = (float)hxc_z_h6c58eb1f3d93 }, (float)0.48, (float)0.16, (float)0.48, (struct Color){ .r = 62, .g = 40, .b = 47, .a = 255 });
  }
  if (hxc_enemy.hxc_vitals.hxc_health > 0)
  {
    if (hxc_entityTextureReady)
    {
      double hxc_y_h90412ad8ba57 = hxc_enemy.hxc_body.hxc_y + 0.48;
      double hxc_z_h223bea804323 = hxc_enemy.hxc_body.hxc_z;
      double hxc_tmp_load_result_n49 = hxc_y_h90412ad8ba57;
      hxc_caxecraft_app_CaxecraftAtlas_drawWorldSprite(hxc_camera, hxc_entityTexture, hxc_caxecraft_app_WorldSprite_MosslingFront, (struct Vector3){ .x = (float)hxc_enemy.hxc_body.hxc_x, .y = (float)hxc_tmp_load_result_n49, .z = (float)hxc_z_h223bea804323 }, 1.05, 0.96);
    }
    else
    {
      double hxc_y_h95b71126aca9 = hxc_enemy.hxc_body.hxc_y + 0.30;
      double hxc_z_h6e89dccf6e30 = hxc_enemy.hxc_body.hxc_z;
      double hxc_tmp_load_result_n58 = hxc_y_h95b71126aca9;
      DrawCube((struct Vector3){ .x = (float)hxc_enemy.hxc_body.hxc_x, .y = (float)hxc_tmp_load_result_n58, .z = (float)hxc_z_h6e89dccf6e30 }, (float)0.70, (float)0.54, (float)0.70, (struct Color){ .r = 74, .g = 119, .b = 67, .a = 255 });
      double hxc_y_h963e62dbf6dc = hxc_enemy.hxc_body.hxc_y + 0.66;
      double hxc_z_hdcbcbb59556a = hxc_enemy.hxc_body.hxc_z;
      double hxc_tmp_load_result_n68 = hxc_y_h963e62dbf6dc;
      DrawCube((struct Vector3){ .x = (float)hxc_enemy.hxc_body.hxc_x, .y = (float)hxc_tmp_load_result_n68, .z = (float)hxc_z_hdcbcbb59556a }, (float)0.50, (float)0.34, (float)0.50, (struct Color){ .r = 157, .g = 190, .b = 82, .a = 255 });
    }
    if (hxc_enemyPhase == hxc_caxecraft_domain_ActorControllerPhase_Windup)
    {
      double hxc_y_hb2c7f119844b = hxc_enemy.hxc_body.hxc_y + 1.02;
      double hxc_z_h76cdbb87e4cc = hxc_enemy.hxc_body.hxc_z;
      double hxc_tmp_load_result_n79 = hxc_y_hb2c7f119844b;
      DrawCube((struct Vector3){ .x = (float)hxc_enemy.hxc_body.hxc_x, .y = (float)hxc_tmp_load_result_n79, .z = (float)hxc_z_h76cdbb87e4cc }, (float)0.20, (float)0.20, (float)0.20, (struct Color){ .r = 218, .g = 65, .b = 72, .a = 255 });
    }
  }
  if (hxc_berryDrop.hxc_active)
  {
    double hxc_y_h66e65d54f1ef = hxc_berryDrop.hxc_y;
    double hxc_z_h4eb6022f55c0 = hxc_berryDrop.hxc_z;
    double hxc_tmp_load_result_n87 = hxc_y_h66e65d54f1ef;
    DrawCube((struct Vector3){ .x = (float)(hxc_berryDrop.hxc_x - 0.12), .y = (float)hxc_tmp_load_result_n87, .z = (float)hxc_z_h4eb6022f55c0 }, (float)0.18, (float)0.18, (float)0.18, (struct Color){ .r = 174, .g = 78, .b = 136, .a = 255 });
    double hxc_y_h28bb0fadf1f2 = hxc_berryDrop.hxc_y;
    double hxc_z_h0d99ace2de3d = hxc_berryDrop.hxc_z;
    double hxc_tmp_load_result_n94 = hxc_y_h28bb0fadf1f2;
    DrawCube((struct Vector3){ .x = (float)(hxc_berryDrop.hxc_x + 0.12), .y = (float)hxc_tmp_load_result_n94, .z = (float)hxc_z_h0d99ace2de3d }, (float)0.18, (float)0.18, (float)0.18, (struct Color){ .r = 174, .g = 78, .b = 136, .a = 255 });
  }
  return;
}

void hxc_caxecraft_app_CaxecraftApp_drawBreath(int32_t hxc_breathTicks, int32_t hxc_maximumBreathTicks, int32_t hxc_width, int32_t hxc_height)
{
  int32_t hxc_bubbleCount = 10;
  int32_t hxc_filled = 0;
  if (hxc_maximumBreathTicks > 0)
  {
    hxc_filled = hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_i32_subtract_wrapping(hxc_i32_add_wrapping(hxc_i32_multiply_wrapping(hxc_breathTicks, hxc_bubbleCount), hxc_maximumBreathTicks), 1), (double)hxc_maximumBreathTicks));
  }
  if (hxc_filled < 0)
  {
    hxc_filled = 0;
  }
  int32_t hxc_tmp_load_result_n2 = hxc_filled;
  if (hxc_tmp_load_result_n2 > hxc_bubbleCount)
  {
    hxc_filled = hxc_bubbleCount;
  }
  int32_t hxc_startX = hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_i32_subtract_wrapping(hxc_width, hxc_i32_subtract_wrapping(hxc_i32_multiply_wrapping(hxc_bubbleCount, 18), 4)), (double)2));
  int32_t hxc_y = hxc_i32_subtract_wrapping(hxc_height, 128);
  int32_t hxc_bubble = 0;
  while (1)
  {
    int32_t hxc_tmp_load_result_n6 = hxc_bubble;
    if (!(hxc_tmp_load_result_n6 < hxc_bubbleCount))
    {
      break;
    }
    int32_t hxc_tmp_load_result_n8 = hxc_bubble;
    if (hxc_tmp_load_result_n8 < hxc_filled)
    {
      float hxc_radius_h475c1ba65823 = (float)6.0;
      struct Color hxc_this1_h90b60f3789b8 = (struct Color){ .r = 164, .g = 235, .b = 246, .a = 255 };
      int32_t hxc_tmp_load_result_n11 = hxc_startX;
      int32_t hxc_tmp_load_result_n12 = hxc_bubble;
      int32_t hxc_tmp_load_result_n13 = hxc_y;
      float hxc_tmp_load_result_n14 = hxc_radius_h475c1ba65823;
      DrawCircle((int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n11, hxc_i32_multiply_wrapping(hxc_tmp_load_result_n12, 18)), (int32_t)hxc_tmp_load_result_n13, hxc_tmp_load_result_n14, hxc_this1_h90b60f3789b8);
    }
    else
    {
      float hxc_radius_h99fc1d2f51e4 = (float)6.0;
      struct Color hxc_this1_ha0ecb373d9cf = (struct Color){ .r = 49, .g = 82, .b = 103, .a = 210 };
      int32_t hxc_tmp_load_result_n17 = hxc_startX;
      int32_t hxc_tmp_load_result_n18 = hxc_bubble;
      int32_t hxc_tmp_load_result_n19 = hxc_y;
      float hxc_tmp_load_result_n20 = hxc_radius_h99fc1d2f51e4;
      DrawCircle((int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n17, hxc_i32_multiply_wrapping(hxc_tmp_load_result_n18, 18)), (int32_t)hxc_tmp_load_result_n19, hxc_tmp_load_result_n20, hxc_this1_ha0ecb373d9cf);
    }
    hxc_bubble = hxc_i32_add_wrapping(hxc_bubble, 1);
  }
  return;
}

void hxc_caxecraft_app_CaxecraftApp_drawHealth(struct hxc_caxecraft_domain_VitalsState hxc_vitals, struct Texture hxc_hudTexture, bool hxc_hudTextureReady, int32_t hxc_width)
{
  int32_t hxc_heart = 0;
  while (1)
  {
    if (!(hxc_heart < 3))
    {
      break;
    }
    int32_t hxc_x = hxc_i32_add_wrapping(hxc_i32_subtract_wrapping(hxc_width, 170), hxc_i32_multiply_wrapping(hxc_heart, 50));
    int32_t hxc_points = hxc_i32_subtract_wrapping(hxc_vitals.hxc_health, hxc_i32_multiply_wrapping(hxc_heart, 2));
    if (hxc_hudTextureReady)
    {
      if (hxc_points >= 2)
      {
        hxc_caxecraft_app_CaxecraftAtlas_drawHudGlyph(hxc_hudTexture, hxc_caxecraft_app_HudGlyph_HealthFull, hxc_x, 18, 42);
      }
      else
      {
        if (hxc_points == 1)
        {
          hxc_caxecraft_app_CaxecraftAtlas_drawHudGlyph(hxc_hudTexture, hxc_caxecraft_app_HudGlyph_HealthHalf, hxc_x, 18, 42);
        }
        else
        {
          hxc_caxecraft_app_CaxecraftAtlas_drawHudGlyph(hxc_hudTexture, hxc_caxecraft_app_HudGlyph_HealthEmpty, hxc_x, 18, 42);
        }
      }
    }
    else
    {
      if (hxc_points > 0)
      {
        struct Color hxc_this1_h4bf1e3cb0db1 = (struct Color){ .r = 218, .g = 65, .b = 72, .a = 255 };
        int32_t hxc_tmp_load_result_n14 = hxc_x;
        DrawRectangle((int32_t)hxc_tmp_load_result_n14, (int32_t)22, (int32_t)34, (int32_t)26, hxc_this1_h4bf1e3cb0db1);
      }
      else
      {
        struct Color hxc_this1_h62a6326a039a = (struct Color){ .r = 228, .g = 239, .b = 227, .a = 255 };
        int32_t hxc_tmp_load_result_n17 = hxc_x;
        DrawRectangleLines((int32_t)hxc_tmp_load_result_n17, (int32_t)22, (int32_t)34, (int32_t)26, hxc_this1_h62a6326a039a);
      }
    }
    hxc_heart = hxc_i32_add_wrapping(hxc_heart, 1);
  }
  return;
}

void hxc_caxecraft_app_CaxecraftApp_drawHotbar(struct hxc_caxecraft_gameplay_InventoryState hxc_inventory, struct Texture hxc_hudTexture, bool hxc_hudTextureReady, struct Texture hxc_itemTexture, bool hxc_itemTextureReady, int32_t hxc_width, int32_t hxc_height)
{
  int32_t hxc_slotSize = 64;
  int32_t hxc_gap = 4;
  int32_t hxc_tmp_load_result_n0 = hxc_slotSize;
  int32_t hxc_totalWidth = hxc_i32_add_wrapping(hxc_i32_multiply_wrapping(8, hxc_tmp_load_result_n0), hxc_i32_multiply_wrapping(7, hxc_gap));
  int32_t hxc_startX = hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_i32_subtract_wrapping(hxc_width, hxc_totalWidth), (double)2));
  int32_t hxc_y = hxc_i32_subtract_wrapping(hxc_height, 96);
  int32_t hxc_slot = 0;
  while (1)
  {
    if (!(hxc_slot < 8))
    {
      break;
    }
    int32_t hxc_tmp_load_result_n4 = hxc_startX;
    int32_t hxc_tmp_load_result_n5 = hxc_slot;
    int32_t hxc_tmp_load_result_n6 = hxc_slotSize;
    int32_t hxc_x = hxc_i32_add_wrapping(hxc_tmp_load_result_n4, hxc_i32_multiply_wrapping(hxc_tmp_load_result_n5, hxc_i32_add_wrapping(hxc_tmp_load_result_n6, hxc_gap)));
    if (hxc_hudTextureReady)
    {
      if (hxc_slot == hxc_inventory.hxc_selected)
      {
        int32_t hxc_tmp_load_result_n11 = hxc_x;
        int32_t hxc_tmp_load_result_n12 = hxc_y;
        hxc_caxecraft_app_CaxecraftAtlas_drawHotbarFrame(hxc_hudTexture, hxc_caxecraft_app_HotbarFrame_Selected, hxc_tmp_load_result_n11, hxc_tmp_load_result_n12, hxc_slotSize);
      }
      else
      {
        int32_t hxc_tmp_load_result_n15 = hxc_x;
        int32_t hxc_tmp_load_result_n16 = hxc_y;
        hxc_caxecraft_app_CaxecraftAtlas_drawHotbarFrame(hxc_hudTexture, hxc_caxecraft_app_HotbarFrame_Normal, hxc_tmp_load_result_n15, hxc_tmp_load_result_n16, hxc_slotSize);
      }
    }
    else
    {
      struct Color hxc_this1 = (struct Color){ .r = 15, .g = 27, .b = 39, .a = 220 };
      int32_t hxc_tmp_load_result_n19 = hxc_x;
      int32_t hxc_tmp_load_result_n20 = hxc_y;
      int32_t hxc_tmp_load_result_n21 = hxc_slotSize;
      int32_t hxc_tmp_load_result_n22 = hxc_slotSize;
      DrawRectangle((int32_t)hxc_tmp_load_result_n19, (int32_t)hxc_tmp_load_result_n20, (int32_t)hxc_tmp_load_result_n21, (int32_t)hxc_tmp_load_result_n22, hxc_this1);
      int32_t hxc_tmp_load_result_n24 = hxc_slot;
      struct Color hxc_tmp_conditional_result_n16;
      if (hxc_tmp_load_result_n24 == hxc_inventory.hxc_selected)
      {
        hxc_tmp_conditional_result_n16 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
      }
      else
      {
        hxc_tmp_conditional_result_n16 = (struct Color){ .r = 228, .g = 239, .b = 227, .a = 255 };
      }
      struct Color hxc_color = hxc_tmp_conditional_result_n16;
      int32_t hxc_tmp_load_result_n29 = hxc_x;
      int32_t hxc_tmp_load_result_n30 = hxc_y;
      int32_t hxc_tmp_load_result_n31 = hxc_slotSize;
      int32_t hxc_tmp_load_result_n32 = hxc_slotSize;
      DrawRectangleLines((int32_t)hxc_tmp_load_result_n29, (int32_t)hxc_tmp_load_result_n30, (int32_t)hxc_tmp_load_result_n31, (int32_t)hxc_tmp_load_result_n32, hxc_color);
    }
    if (hxc_itemTextureReady)
    {
      enum hxc_caxecraft_gameplay_ItemKind hxc_tmp_call_result_n35 = hxc_caxecraft_gameplay_Inventory_itemAt(hxc_slot);
      int32_t hxc_tmp_load_result_n36 = hxc_x;
      int32_t hxc_tmp_load_result_n37 = hxc_y;
      hxc_caxecraft_app_CaxecraftAtlas_drawItem(hxc_itemTexture, hxc_tmp_call_result_n35, hxc_i32_add_wrapping(hxc_tmp_load_result_n36, 6), hxc_i32_add_wrapping(hxc_tmp_load_result_n37, 4), hxc_i32_subtract_wrapping(hxc_slotSize, 12));
    }
    int32_t hxc_tmp_call_result_n40 = hxc_caxecraft_gameplay_Inventory_countAt(hxc_inventory, hxc_slot);
    int32_t hxc_tmp_load_result_n41 = hxc_x;
    hxc_caxecraft_app_HudDigits_drawNumber(hxc_tmp_call_result_n40, hxc_i32_add_wrapping(hxc_tmp_load_result_n41, 39), hxc_i32_add_wrapping(hxc_y, 44), 2, (struct Color){ .r = 228, .g = 239, .b = 227, .a = 255 });
    hxc_slot = hxc_i32_add_wrapping(hxc_slot, 1);
  }
  return;
}

void hxc_caxecraft_app_CaxecraftApp_drawHud(struct hxc_caxecraft_app_HudView hxc_view, struct hxc_caxecraft_app_HudResources hxc_resources)
{
  int32_t hxc_visible = hxc_view.hxc_metrics.hxc_visibleBlocks;
  int32_t hxc_drawCalls = hxc_view.hxc_metrics.hxc_drawCalls;
  int32_t hxc_frames = hxc_view.hxc_metrics.hxc_renderedFrames;
  int32_t hxc_updates = hxc_view.hxc_metrics.hxc_completedTicks;
  bool hxc_paused = hxc_view.hxc_paused;
  bool hxc_captured = hxc_view.hxc_pointerCaptured;
  bool hxc_placementBlocked = hxc_view.hxc_feedback.hxc_placementBlocked;
  struct hxc_caxecraft_domain_RaycastHit hxc_hit = hxc_view.hxc_hit;
  int32_t hxc_mode = hxc_view.hxc_mode;
  int32_t hxc_locale = hxc_view.hxc_locale;
  struct hxc_caxecraft_gameplay_InventoryState hxc_inventory = hxc_view.hxc_inventory;
  enum hxc_caxecraft_gameplay_GuidePhase hxc_guidePhase = hxc_view.hxc_guidePhase;
  bool hxc_guideInteractionAvailable = hxc_view.hxc_guideInteractionAvailable;
  struct hxc_caxecraft_domain_Character hxc_enemy = hxc_view.hxc_enemy;
  enum hxc_caxecraft_domain_ActorControllerPhase hxc_enemyPhase = hxc_view.hxc_enemyPhase;
  struct hxc_caxecraft_domain_VitalsState hxc_vitals = hxc_view.hxc_character.hxc_vitals;
  bool hxc_strikeHit = hxc_view.hxc_feedback.hxc_strikeHit;
  bool hxc_enemyDefeated = hxc_view.hxc_feedback.hxc_enemyDefeated;
  bool hxc_enemyAttacked = hxc_view.hxc_feedback.hxc_enemyAttacked;
  bool hxc_pickedUp = hxc_view.hxc_feedback.hxc_pickedUp;
  int32_t hxc_pickupAmount = hxc_view.hxc_feedback.hxc_pickupAmount;
  int32_t hxc_inventoryFullReason = hxc_view.hxc_feedback.hxc_inventoryFullReason;
  int32_t hxc_recoveryFeedback = hxc_view.hxc_feedback.hxc_recoveryDecision;
  bool hxc_recoveryVisible = hxc_view.hxc_feedback.hxc_recoveryVisible;
  struct Texture hxc_hudTexture = hxc_resources.hxc_hudTexture;
  bool hxc_hudTextureReady = hxc_resources.hxc_hudTextureReady;
  struct Texture hxc_itemTexture = hxc_resources.hxc_itemTexture;
  bool hxc_itemTextureReady = hxc_resources.hxc_itemTextureReady;
  struct Texture hxc_adventureItemTexture = hxc_resources.hxc_adventureItemTexture;
  bool hxc_adventureItemTextureReady = hxc_resources.hxc_adventureItemTextureReady;
  int32_t hxc_aquaticEquipmentCode = hxc_view.hxc_character.hxc_aquaticEquipmentCode;
  bool hxc_aquaticEquipmentVisible = hxc_view.hxc_character.hxc_aquaticEquipmentVisible;
  bool hxc_headSubmerged = hxc_view.hxc_character.hxc_headSubmerged;
  int32_t hxc_breathTicks = hxc_view.hxc_character.hxc_breathTicks;
  int32_t hxc_maximumBreathTicks = hxc_view.hxc_character.hxc_maximumBreathTicks;
  int32_t hxc_tmp_native_call_result_n54 = GetScreenWidth();
  int32_t hxc_width = (int32_t)hxc_tmp_native_call_result_n54;
  int32_t hxc_tmp_native_call_result_n55 = GetScreenHeight();
  int32_t hxc_height = (int32_t)hxc_tmp_native_call_result_n55;
  int32_t hxc_centerX = hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_width, (double)2));
  int32_t hxc_centerY = hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_height, (double)2));
  struct Color hxc_text = (struct Color){ .r = 228, .g = 239, .b = 227, .a = 255 };
  int32_t hxc_tmp_load_result_n59 = hxc_centerX;
  int32_t hxc_tmp_load_result_n60 = hxc_centerY;
  int32_t hxc_tmp_load_result_n61 = hxc_centerX;
  int32_t hxc_tmp_load_result_n62 = hxc_centerY;
  DrawLine((int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n59, 8), (int32_t)hxc_tmp_load_result_n60, (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n61, 3), (int32_t)hxc_tmp_load_result_n62, hxc_text);
  int32_t hxc_tmp_load_result_n64 = hxc_centerX;
  int32_t hxc_tmp_load_result_n65 = hxc_centerY;
  int32_t hxc_tmp_load_result_n66 = hxc_centerX;
  int32_t hxc_tmp_load_result_n67 = hxc_centerY;
  DrawLine((int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n64, 3), (int32_t)hxc_tmp_load_result_n65, (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n66, 8), (int32_t)hxc_tmp_load_result_n67, hxc_text);
  int32_t hxc_tmp_load_result_n69 = hxc_centerX;
  int32_t hxc_tmp_load_result_n70 = hxc_centerY;
  int32_t hxc_tmp_load_result_n71 = hxc_centerX;
  int32_t hxc_tmp_load_result_n72 = hxc_centerY;
  DrawLine((int32_t)hxc_tmp_load_result_n69, (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n70, 8), (int32_t)hxc_tmp_load_result_n71, (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n72, 3), hxc_text);
  int32_t hxc_tmp_load_result_n74 = hxc_centerX;
  int32_t hxc_tmp_load_result_n75 = hxc_centerY;
  int32_t hxc_tmp_load_result_n76 = hxc_centerX;
  int32_t hxc_tmp_load_result_n77 = hxc_centerY;
  DrawLine((int32_t)hxc_tmp_load_result_n74, (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n75, 3), (int32_t)hxc_tmp_load_result_n76, (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n77, 8), hxc_text);
  struct Color hxc_this1_h7c2aaa0768a0 = (struct Color){ .r = 15, .g = 27, .b = 39, .a = 220 };
  DrawRectangle((int32_t)18, (int32_t)18, (int32_t)460, (int32_t)108, hxc_this1_h7c2aaa0768a0);
  struct Color hxc_this1_he50f396df5c2 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
  DrawRectangleLines((int32_t)18, (int32_t)18, (int32_t)460, (int32_t)108, hxc_this1_he50f396df5c2);
  const char *hxc_tmp_call_result_n84 = hxc_caxecraft_localization_UiCatalog_text(hxc_locale, 1);
  DrawText(hxc_tmp_call_result_n84, (int32_t)32, (int32_t)28, (int32_t)20, hxc_text);
  const char *hxc_tmp_call_result_n87 = hxc_caxecraft_localization_UiCatalog_text(hxc_locale, 4);
  DrawText(hxc_tmp_call_result_n87, (int32_t)32, (int32_t)58, (int32_t)14, hxc_text);
  hxc_caxecraft_app_HudDigits_drawNumber(16384, 82, 59, 5, (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 });
  const char *hxc_tmp_call_result_n91 = hxc_caxecraft_localization_UiCatalog_text(hxc_locale, 8);
  DrawText(hxc_tmp_call_result_n91, (int32_t)160, (int32_t)58, (int32_t)14, hxc_text);
  hxc_caxecraft_app_HudDigits_drawNumber(hxc_visible, 230, 59, 5, (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 });
  const char *hxc_tmp_call_result_n96 = hxc_caxecraft_localization_UiCatalog_text(hxc_locale, 5);
  DrawText(hxc_tmp_call_result_n96, (int32_t)326, (int32_t)58, (int32_t)14, hxc_text);
  hxc_caxecraft_app_HudDigits_drawNumber(hxc_drawCalls, 382, 59, 5, (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 });
  const char *hxc_tmp_call_result_n101 = hxc_caxecraft_localization_UiCatalog_text(hxc_locale, 6);
  DrawText(hxc_tmp_call_result_n101, (int32_t)32, (int32_t)86, (int32_t)12, hxc_text);
  int32_t hxc_tmp_load_result_n103 = hxc_frames;
  hxc_caxecraft_app_HudDigits_drawNumber(hxc_tmp_load_result_n103, 82, 85, 6, hxc_text);
  const char *hxc_tmp_call_result_n106 = hxc_caxecraft_localization_UiCatalog_text(hxc_locale, 7);
  DrawText(hxc_tmp_call_result_n106, (int32_t)174, (int32_t)86, (int32_t)12, hxc_text);
  int32_t hxc_tmp_load_result_n108 = hxc_updates;
  hxc_caxecraft_app_HudDigits_drawNumber(hxc_tmp_load_result_n108, 216, 85, 6, hxc_text);
  struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_load_result_n110 = hxc_inventory;
  struct Texture hxc_tmp_load_result_n111 = hxc_hudTexture;
  bool hxc_tmp_load_result_n112 = hxc_hudTextureReady;
  struct Texture hxc_tmp_load_result_n113 = hxc_itemTexture;
  bool hxc_tmp_load_result_n114 = hxc_itemTextureReady;
  int32_t hxc_tmp_load_result_n115 = hxc_width;
  hxc_caxecraft_app_CaxecraftApp_drawHotbar(hxc_tmp_load_result_n110, hxc_tmp_load_result_n111, hxc_tmp_load_result_n112, hxc_tmp_load_result_n113, hxc_tmp_load_result_n114, hxc_tmp_load_result_n115, hxc_height);
  struct hxc_caxecraft_domain_VitalsState hxc_tmp_load_result_n117 = hxc_vitals;
  struct Texture hxc_tmp_load_result_n118 = hxc_hudTexture;
  bool hxc_tmp_load_result_n119 = hxc_hudTextureReady;
  hxc_caxecraft_app_CaxecraftApp_drawHealth(hxc_tmp_load_result_n117, hxc_tmp_load_result_n118, hxc_tmp_load_result_n119, hxc_width);
  if (hxc_aquaticEquipmentCode >= 0)
  {
    int32_t hxc_tmp_load_result_n122 = hxc_aquaticEquipmentCode;
    struct Texture hxc_tmp_load_result_n123 = hxc_itemTexture;
    bool hxc_tmp_load_result_n124 = hxc_itemTextureReady;
    struct Texture hxc_tmp_load_result_n125 = hxc_adventureItemTexture;
    bool hxc_tmp_load_result_n126 = hxc_adventureItemTextureReady;
    hxc_caxecraft_app_AuthoredItemRenderer_drawEquippedIcon(hxc_tmp_load_result_n122, hxc_tmp_load_result_n123, hxc_tmp_load_result_n124, hxc_tmp_load_result_n125, hxc_tmp_load_result_n126, hxc_i32_subtract_wrapping(hxc_width, 226), 18, 42);
  }
  if (hxc_headSubmerged)
  {
    int32_t hxc_tmp_load_result_n129 = hxc_breathTicks;
    int32_t hxc_tmp_load_result_n130 = hxc_maximumBreathTicks;
    int32_t hxc_tmp_load_result_n131 = hxc_width;
    hxc_caxecraft_app_CaxecraftApp_drawBreath(hxc_tmp_load_result_n129, hxc_tmp_load_result_n130, hxc_tmp_load_result_n131, hxc_height);
  }
  const char *hxc_tmp_call_result_n134 = hxc_caxecraft_localization_UiCatalog_text(hxc_locale, 3);
  int32_t hxc_tmp_load_result_n135 = hxc_height;
  DrawText(hxc_tmp_call_result_n134, (int32_t)20, (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n135, 22), (int32_t)14, hxc_text);
  if (hxc_mode == 1)
  {
    struct Color hxc_this1_hf7fbabd37910 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
    const char *hxc_tmp_call_result_n140 = hxc_caxecraft_localization_FirstPlayableCatalog_text(hxc_locale, 0);
    DrawText(hxc_tmp_call_result_n140, (int32_t)32, (int32_t)110, (int32_t)14, hxc_this1_hf7fbabd37910);
  }
  if (hxc_guideInteractionAvailable)
  {
    struct Color hxc_this1_h8198cb1a4cbe = (struct Color){ .r = 15, .g = 27, .b = 39, .a = 220 };
    int32_t hxc_tmp_load_result_n144 = hxc_centerX;
    int32_t hxc_tmp_load_result_n145 = hxc_centerY;
    DrawRectangle((int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n144, 260), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n145, 54), (int32_t)520, (int32_t)60, hxc_this1_h8198cb1a4cbe);
    if (hxc_guidePhase == hxc_caxecraft_gameplay_GuidePhase_Waiting)
    {
      const char *hxc_tmp_call_result_n150 = hxc_caxecraft_localization_FirstPlayableCatalog_text(hxc_locale, 13);
      int32_t hxc_tmp_load_result_n151 = hxc_centerX;
      int32_t hxc_tmp_load_result_n152 = hxc_centerY;
      DrawText(hxc_tmp_call_result_n150, (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n151, 110), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n152, 74), (int32_t)18, hxc_text);
    }
    else
    {
      if (hxc_guidePhase == hxc_caxecraft_gameplay_GuidePhase_Welcomed)
      {
        const char *hxc_tmp_call_result_n157 = hxc_caxecraft_localization_FirstPlayableCatalog_text(hxc_locale, 14);
        int32_t hxc_tmp_load_result_n158 = hxc_centerX;
        int32_t hxc_tmp_load_result_n159 = hxc_centerY;
        DrawText(hxc_tmp_call_result_n157, (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n158, 225), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n159, 74), (int32_t)16, hxc_text);
      }
      else
      {
        const char *hxc_tmp_call_result_n162 = hxc_caxecraft_localization_FirstPlayableCatalog_text(hxc_locale, 12);
        int32_t hxc_tmp_load_result_n163 = hxc_centerX;
        int32_t hxc_tmp_load_result_n164 = hxc_centerY;
        DrawText(hxc_tmp_call_result_n162, (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n163, 205), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n164, 74), (int32_t)16, hxc_text);
      }
    }
  }
  if (hxc_enemy.hxc_vitals.hxc_health > 0)
  {
    if (hxc_enemyPhase == hxc_caxecraft_domain_ActorControllerPhase_Windup)
    {
      struct Color hxc_this1_h94855cb50317 = (struct Color){ .r = 218, .g = 65, .b = 72, .a = 255 };
      const char *hxc_tmp_call_result_n171 = hxc_caxecraft_localization_FirstPlayableCatalog_text(hxc_locale, 11);
      int32_t hxc_tmp_load_result_n172 = hxc_width;
      DrawText(hxc_tmp_call_result_n171, (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n172, 300), (int32_t)28, (int32_t)16, hxc_this1_h94855cb50317);
    }
    else
    {
      if (hxc_enemyPhase == hxc_caxecraft_domain_ActorControllerPhase_Chasing)
      {
        struct Color hxc_this1_h1eedf8acdbc9 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
        const char *hxc_tmp_call_result_n178 = hxc_caxecraft_localization_FirstPlayableCatalog_text(hxc_locale, 9);
        int32_t hxc_tmp_load_result_n179 = hxc_width;
        DrawText(hxc_tmp_call_result_n178, (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n179, 180), (int32_t)28, (int32_t)16, hxc_this1_h1eedf8acdbc9);
      }
    }
  }
  if (hxc_strikeHit)
  {
    struct Color hxc_this1_he708863e94f5 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
    const char *hxc_tmp_call_result_n184 = hxc_caxecraft_localization_FirstPlayableCatalog_text(hxc_locale, 7);
    int32_t hxc_tmp_load_result_n185 = hxc_centerX;
    int32_t hxc_tmp_load_result_n186 = hxc_centerY;
    DrawText(hxc_tmp_call_result_n184, (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n185, 70), (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n186, 54), (int32_t)18, hxc_this1_he708863e94f5);
  }
  if (hxc_enemyDefeated)
  {
    struct Color hxc_this1_hde945947c4f2 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
    const char *hxc_tmp_call_result_n191 = hxc_caxecraft_localization_FirstPlayableCatalog_text(hxc_locale, 10);
    int32_t hxc_tmp_load_result_n192 = hxc_width;
    DrawText(hxc_tmp_call_result_n191, (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n192, 285), (int32_t)54, (int32_t)16, hxc_this1_hde945947c4f2);
  }
  if (hxc_enemyAttacked)
  {
    struct Color hxc_this1_hc78e4f3725b1 = (struct Color){ .r = 218, .g = 65, .b = 72, .a = 255 };
    const char *hxc_tmp_call_result_n197 = hxc_caxecraft_localization_FirstPlayableCatalog_text(hxc_locale, 18);
    int32_t hxc_tmp_load_result_n198 = hxc_width;
    DrawText(hxc_tmp_call_result_n197, (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n198, 330), (int32_t)82, (int32_t)16, hxc_this1_hc78e4f3725b1);
  }
  if (hxc_pickedUp)
  {
    int32_t hxc_tmp_load_result_n201 = hxc_pickupAmount;
    int32_t hxc_tmp_conditional_result_n52 = 0;
    if (hxc_tmp_load_result_n201 == 1)
    {
      hxc_tmp_conditional_result_n52 = 2;
    }
    else
    {
      hxc_tmp_conditional_result_n52 = 3;
    }
    int32_t hxc_pickupMessage = hxc_tmp_conditional_result_n52;
    struct Color hxc_this1_h0626ec518fa4 = (struct Color){ .r = 174, .g = 78, .b = 136, .a = 255 };
    int32_t hxc_tmp_load_result_n204 = hxc_locale;
    const char *hxc_tmp_call_result_n206 = hxc_caxecraft_localization_FirstPlayableCatalog_text(hxc_tmp_load_result_n204, hxc_pickupMessage);
    int32_t hxc_tmp_load_result_n207 = hxc_centerX;
    int32_t hxc_tmp_load_result_n208 = hxc_centerY;
    DrawText(hxc_tmp_call_result_n206, (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n207, 48), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n208, 24), (int32_t)18, hxc_this1_h0626ec518fa4);
  }
  if (hxc_inventoryFullReason == 1)
  {
    struct Color hxc_this1_hf34305fbe6c3 = (struct Color){ .r = 238, .g = 113, .b = 78, .a = 255 };
    const char *hxc_tmp_call_result_n213 = hxc_caxecraft_localization_FirstPlayableCatalog_text(hxc_locale, 5);
    int32_t hxc_tmp_load_result_n214 = hxc_centerX;
    int32_t hxc_tmp_load_result_n215 = hxc_centerY;
    DrawText(hxc_tmp_call_result_n213, (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n214, 150), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n215, 48), (int32_t)16, hxc_this1_hf34305fbe6c3);
  }
  else
  {
    if (hxc_inventoryFullReason == 2)
    {
      struct Color hxc_this1_h1d26745147c2 = (struct Color){ .r = 238, .g = 113, .b = 78, .a = 255 };
      const char *hxc_tmp_call_result_n220 = hxc_caxecraft_localization_FirstPlayableCatalog_text(hxc_locale, 6);
      int32_t hxc_tmp_load_result_n221 = hxc_centerX;
      int32_t hxc_tmp_load_result_n222 = hxc_centerY;
      DrawText(hxc_tmp_call_result_n220, (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n221, 155), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n222, 48), (int32_t)16, hxc_this1_h1d26745147c2);
    }
  }
  if (hxc_recoveryVisible)
  {
    if (hxc_recoveryFeedback == 1)
    {
      struct Color hxc_this1_h635af7fe7a48 = (struct Color){ .r = 94, .g = 212, .b = 136, .a = 255 };
      const char *hxc_tmp_call_result_n228 = hxc_caxecraft_localization_FirstPlayableCatalog_text(hxc_locale, 4);
      int32_t hxc_tmp_load_result_n229 = hxc_centerX;
      int32_t hxc_tmp_load_result_n230 = hxc_centerY;
      DrawText(hxc_tmp_call_result_n228, (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n229, 88), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n230, 24), (int32_t)18, hxc_this1_h635af7fe7a48);
    }
    else
    {
      if (hxc_recoveryFeedback == 2)
      {
        struct Color hxc_this1_h79a1f7c6738d = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
        const char *hxc_tmp_call_result_n235 = hxc_caxecraft_localization_UiCatalog_text(hxc_locale, 25);
        int32_t hxc_tmp_load_result_n236 = hxc_centerX;
        int32_t hxc_tmp_load_result_n237 = hxc_centerY;
        DrawText(hxc_tmp_call_result_n235, (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n236, 96), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n237, 24), (int32_t)18, hxc_this1_h79a1f7c6738d);
      }
      else
      {
        if (hxc_recoveryFeedback == 3)
        {
          struct Color hxc_this1_h36ad8ed6c864 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
          const char *hxc_tmp_call_result_n242 = hxc_caxecraft_localization_FirstPlayableCatalog_text(hxc_locale, 15);
          int32_t hxc_tmp_load_result_n243 = hxc_centerX;
          int32_t hxc_tmp_load_result_n244 = hxc_centerY;
          DrawText(hxc_tmp_call_result_n242, (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n243, 76), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n244, 24), (int32_t)18, hxc_this1_h36ad8ed6c864);
        }
      }
    }
  }
  if (hxc_aquaticEquipmentVisible)
  {
    struct Color hxc_this1_h94233c4871af = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
    const char *hxc_tmp_call_result_n249 = hxc_caxecraft_localization_UiCatalog_text(hxc_locale, 0);
    int32_t hxc_tmp_load_result_n250 = hxc_centerX;
    int32_t hxc_tmp_load_result_n251 = hxc_centerY;
    DrawText(hxc_tmp_call_result_n249, (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n250, 128), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n251, 24), (int32_t)18, hxc_this1_h94233c4871af);
  }
  if (hxc_vitals.hxc_safeTicks > 15)
  {
    struct Color hxc_this1_h186ae0cc34a2 = (struct Color){ .r = 218, .g = 65, .b = 72, .a = 255 };
    int32_t hxc_tmp_load_result_n255 = hxc_width;
    int32_t hxc_tmp_load_result_n256 = hxc_height;
    DrawRectangleLines((int32_t)4, (int32_t)4, (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n255, 8), (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n256, 8), hxc_this1_h186ae0cc34a2);
  }
  if (hxc_vitals.hxc_health <= 0)
  {
    struct Color hxc_this1_hcaca8b0e6e05 = (struct Color){ .r = 15, .g = 27, .b = 39, .a = 220 };
    int32_t hxc_tmp_load_result_n260 = hxc_centerX;
    int32_t hxc_tmp_load_result_n261 = hxc_centerY;
    DrawRectangle((int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n260, 250), (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n261, 74), (int32_t)500, (int32_t)148, hxc_this1_hcaca8b0e6e05);
    struct Color hxc_this1_h36534efb0e0f = (struct Color){ .r = 218, .g = 65, .b = 72, .a = 255 };
    int32_t hxc_tmp_load_result_n264 = hxc_centerX;
    int32_t hxc_tmp_load_result_n265 = hxc_centerY;
    DrawRectangleLines((int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n264, 250), (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n265, 74), (int32_t)500, (int32_t)148, hxc_this1_h36534efb0e0f);
    const char *hxc_tmp_call_result_n268 = hxc_caxecraft_localization_FirstPlayableCatalog_text(hxc_locale, 8);
    int32_t hxc_tmp_load_result_n269 = hxc_centerX;
    int32_t hxc_tmp_load_result_n270 = hxc_centerY;
    DrawText(hxc_tmp_call_result_n268, (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n269, 122), (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n270, 42), (int32_t)24, hxc_text);
    struct Color hxc_this1_h26c8d1c19f69 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
    const char *hxc_tmp_call_result_n274 = hxc_caxecraft_localization_FirstPlayableCatalog_text(hxc_locale, 16);
    int32_t hxc_tmp_load_result_n275 = hxc_centerX;
    int32_t hxc_tmp_load_result_n276 = hxc_centerY;
    DrawText(hxc_tmp_call_result_n274, (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n275, 125), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n276, 10), (int32_t)18, hxc_this1_h26c8d1c19f69);
  }
  if (hxc_paused)
  {
    struct Color hxc_this1_hbcdf386ccbe0 = (struct Color){ .r = 15, .g = 27, .b = 39, .a = 220 };
    int32_t hxc_tmp_load_result_n280 = hxc_centerX;
    int32_t hxc_tmp_load_result_n281 = hxc_centerY;
    DrawRectangle((int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n280, 170), (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n281, 48), (int32_t)340, (int32_t)96, hxc_this1_hbcdf386ccbe0);
    struct Color hxc_this1_haee660760ca3 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
    int32_t hxc_tmp_load_result_n284 = hxc_centerX;
    int32_t hxc_tmp_load_result_n285 = hxc_centerY;
    DrawRectangleLines((int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n284, 170), (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n285, 48), (int32_t)340, (int32_t)96, hxc_this1_haee660760ca3);
    const char *hxc_tmp_call_result_n288 = hxc_caxecraft_localization_UiCatalog_text(hxc_locale, 32);
    int32_t hxc_tmp_load_result_n289 = hxc_centerX;
    int32_t hxc_tmp_load_result_n290 = hxc_centerY;
    DrawText(hxc_tmp_call_result_n288, (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n289, 48), (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n290, 30), (int32_t)24, hxc_text);
    const char *hxc_tmp_call_result_n293 = hxc_caxecraft_localization_UiCatalog_text(hxc_locale, 31);
    int32_t hxc_tmp_load_result_n294 = hxc_centerX;
    int32_t hxc_tmp_load_result_n295 = hxc_centerY;
    DrawText(hxc_tmp_call_result_n293, (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n294, 160), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n295, 8), (int32_t)16, hxc_text);
  }
  else
  {
    if (hxc_placementBlocked)
    {
      struct Color hxc_this1_h95bb33c58a18 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
      const char *hxc_tmp_call_result_n300 = hxc_caxecraft_localization_UiCatalog_text(hxc_locale, 33);
      int32_t hxc_tmp_load_result_n301 = hxc_centerX;
      int32_t hxc_tmp_load_result_n302 = hxc_centerY;
      DrawText(hxc_tmp_call_result_n300, (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n301, 170), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n302, 26), (int32_t)14, hxc_this1_h95bb33c58a18);
    }
    else
    {
      if (!hxc_captured)
      {
        const char *hxc_tmp_call_result_n306 = hxc_caxecraft_localization_UiCatalog_text(hxc_locale, 2);
        int32_t hxc_tmp_load_result_n307 = hxc_centerX;
        int32_t hxc_tmp_load_result_n308 = hxc_centerY;
        DrawText(hxc_tmp_call_result_n306, (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n307, 90), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n308, 26), (int32_t)14, hxc_text);
      }
      else
      {
        if (!hxc_hit.hxc_hit)
        {
          const char *hxc_tmp_call_result_n312 = hxc_caxecraft_localization_UiCatalog_text(hxc_locale, 30);
          int32_t hxc_tmp_load_result_n313 = hxc_centerX;
          int32_t hxc_tmp_load_result_n314 = hxc_centerY;
          DrawText(hxc_tmp_call_result_n312, (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n313, 105), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n314, 26), (int32_t)14, hxc_text);
        }
      }
    }
  }
  return;
}

struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation hxc_caxecraft_app_CaxecraftApp_observeActorPhase(struct hxc_array_ref *hxc_states, int32_t hxc_id, enum hxc_caxecraft_domain_ActorControllerPhase hxc_fallback)
{
  struct hxc_caxecraft_domain_ActorControllerState hxc_state = { 0 };
  struct hxc_caxecraft_domain_ActorControllerState hxc_tmp_array_element_owner_n5 = { 0 };
  int32_t hxc_g = 0;
  while (1)
  {
    int32_t hxc_tmp_load_result_n0 = hxc_g;
    int32_t hxc_tmp_array_length_result_n1;
    if (hxc_array_ref_length(hxc_states, &hxc_tmp_array_length_result_n1) != HXC_STATUS_OK)
    {
      abort();
    }
    if (!(hxc_tmp_load_result_n0 < hxc_tmp_array_length_result_n1))
    {
      break;
    }
    struct hxc_caxecraft_domain_ActorControllerState hxc_tmp_array_get_result_n3;
    if (hxc_array_ref_get_copy(hxc_states, (size_t)hxc_g, &hxc_tmp_array_get_result_n3) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_tmp_array_element_owner_n5 = hxc_tmp_array_get_result_n3;
    hxc_state = hxc_tmp_array_element_owner_n5;
    if (hxc_record_50dff1c7_retain(&hxc_state) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_g = hxc_i32_add_wrapping(hxc_g, 1);
    if (hxc_state.hxc_characterId == hxc_id)
    {
      enum hxc_caxecraft_domain_ActorControllerPhase hxc_tmp_record_field_load_result_n7 = hxc_state.hxc_phase;
      hxc_record_50dff1c7_destroy(&hxc_state);
      hxc_array_3767ae6c_element_destroy(NULL, &hxc_tmp_array_element_owner_n5);
      return (struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation){ .hxc_phase = hxc_tmp_record_field_load_result_n7, .hxc_valid = true };
    }
    hxc_record_50dff1c7_destroy(&hxc_state);
    hxc_array_3767ae6c_element_destroy(NULL, &hxc_tmp_array_element_owner_n5);
  }
  return (struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation){ .hxc_phase = hxc_fallback, .hxc_valid = false };
}

struct hxc_caxecraft_app_CaxecraftApp_PlayableActorSelection hxc_caxecraft_app_CaxecraftApp_selectPlayableActors(struct hxc_array_ref *hxc_bindings)
{
  struct hxc_caxecraft_content_LoadedActorBinding hxc_binding = { 0 };
  struct hxc_caxecraft_content_LoadedActorBinding hxc_tmp_array_element_owner_n5 = { 0 };
  int32_t hxc_dialogueActorId = 0;
  int32_t hxc_enemyActorId = 0;
  int32_t hxc_g = 0;
  while (1)
  {
    int32_t hxc_tmp_load_result_n0 = hxc_g;
    int32_t hxc_tmp_array_length_result_n1;
    if (hxc_array_ref_length(hxc_bindings, &hxc_tmp_array_length_result_n1) != HXC_STATUS_OK)
    {
      abort();
    }
    if (!(hxc_tmp_load_result_n0 < hxc_tmp_array_length_result_n1))
    {
      break;
    }
    struct hxc_caxecraft_content_LoadedActorBinding hxc_tmp_array_get_result_n3;
    if (hxc_array_ref_get_copy(hxc_bindings, (size_t)hxc_g, &hxc_tmp_array_get_result_n3) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_tmp_array_element_owner_n5 = hxc_tmp_array_get_result_n3;
    hxc_binding = hxc_tmp_array_element_owner_n5;
    if (hxc_record_8de0419a_retain(&hxc_binding) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_g = hxc_i32_add_wrapping(hxc_g, 1);
    switch (hxc_binding.hxc_role.hxc_tag) {
      case hxc_caxecraft_content_CharacterSpawnRole_DialogueNpc:
        {
          if (hxc_dialogueActorId > 0)
          {
            hxc_record_8de0419a_destroy(&hxc_binding);
            hxc_array_7c0765de_element_destroy(NULL, &hxc_tmp_array_element_owner_n5);
            return (struct hxc_caxecraft_app_CaxecraftApp_PlayableActorSelection){ .hxc_dialogueActorId = 0, .hxc_enemyActorId = 0, .hxc_valid = false };
          }
          else
          {
            hxc_dialogueActorId = hxc_binding.hxc_entityId;
          }
          break;
        }
      case hxc_caxecraft_content_CharacterSpawnRole_EnemyActor:
        {
          if (hxc_enemyActorId > 0)
          {
            hxc_record_8de0419a_destroy(&hxc_binding);
            hxc_array_7c0765de_element_destroy(NULL, &hxc_tmp_array_element_owner_n5);
            return (struct hxc_caxecraft_app_CaxecraftApp_PlayableActorSelection){ .hxc_dialogueActorId = 0, .hxc_enemyActorId = 0, .hxc_valid = false };
          }
          else
          {
            hxc_enemyActorId = hxc_binding.hxc_entityId;
          }
          break;
        }
    }
    hxc_record_8de0419a_destroy(&hxc_binding);
    hxc_array_7c0765de_element_destroy(NULL, &hxc_tmp_array_element_owner_n5);
  }
  int32_t hxc_tmp_load_result_n13 = hxc_dialogueActorId;
  bool hxc_tmp_short_circuit_result_n6 = hxc_tmp_load_result_n13 > 0;
  if (hxc_tmp_load_result_n13 > 0)
  {
    hxc_tmp_short_circuit_result_n6 = hxc_enemyActorId > 0;
  }
  if (!hxc_tmp_short_circuit_result_n6)
  {
    return (struct hxc_caxecraft_app_CaxecraftApp_PlayableActorSelection){ .hxc_dialogueActorId = 0, .hxc_enemyActorId = 0, .hxc_valid = false };
  }
  int32_t hxc_tmp_load_result_n16 = hxc_dialogueActorId;
  return (struct hxc_caxecraft_app_CaxecraftApp_PlayableActorSelection){ .hxc_dialogueActorId = hxc_tmp_load_result_n16, .hxc_enemyActorId = hxc_enemyActorId, .hxc_valid = true };
}

struct hxc_caxecraft_domain_CharacterBody hxc_caxecraft_app_CaxecraftApp_spawnPlayer(const uint8_t *hxc_cells, size_t hxc_length)
{
  const uint8_t *hxc_borrow = hxc_cells;
  size_t hxc_tmp_length_n1 = hxc_length;
  (void)hxc_borrow;
  (void)hxc_tmp_length_n1;
  double hxc_spawnX = 16.5;
  double hxc_spawnY = 5.0;
  double hxc_spawnZ = 16.5;
  double hxc_tmp_load_result_n0 = hxc_spawnX;
  double hxc_tmp_load_result_n1 = hxc_spawnY;
  struct hxc_caxecraft_domain_CharacterBody hxc_tmp_call_result_n3 = hxc_caxecraft_domain_CharacterPhysics_body(hxc_tmp_load_result_n0, hxc_tmp_load_result_n1, hxc_spawnZ);
  struct hxc_caxecraft_domain_CharacterBody hxc_tmp_call_result_n4 = hxc_caxecraft_domain_CharacterPhysics_recoverSpawn(hxc_cells, hxc_length, hxc_tmp_call_result_n3);
  return hxc_tmp_call_result_n4;
}

void hxc_caxecraft_app_CaxecraftApp_run(struct hxc_caxecraft_app_CaxecraftApp *hxc_self)
{
  const void *hxc_gc_roots[2] = { (const void *)hxc_self, NULL };
  struct hxc_gc_root_frame hxc_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_gc_roots, 2, &hxc_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_array_ref *hxc_g1 = { 0 };
  struct hxc_caxecraft_domain_ActorControllerEvent hxc_event = { 0 };
  struct hxc_caxecraft_domain_ActorControllerEvent hxc_tmp_array_element_owner_n175 = { 0 };
  hxc_string hxc_drop = { 0 };
  struct hxc_array_ref *hxc_actorPhases = { 0 };
  struct hxc_array_ref *hxc_presentationActorPhases = { 0 };
  struct hxc_array_ref *hxc_initialActorPhases = { 0 };
  int32_t hxc_initialHealth = 6;
  if (hxc_self == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_domain_GameSession *hxc_tmp_owned_class_field_address_n0 = &(*hxc_self).hxc_session;
  struct hxc_caxecraft_content_FirstPlayableSessionLoad hxc_tmp_call_result_n2 = hxc_caxecraft_content_FirstPlayableSessionLoader_loadCandidate(hxc_tmp_owned_class_field_address_n0, hxc_initialHealth);
  struct hxc_caxecraft_content_FirstPlayableSessionLoad hxc_loadedLevel = hxc_tmp_call_result_n2;
  if (!hxc_loadedLevel.hxc_valid)
  {
    hxc_record_c82cbeac_destroy(&hxc_loadedLevel);
    if (hxc_gc_root_frame_pop(&hxc_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return;
  }
  struct hxc_caxecraft_app_CaxecraftApp_PlayableActorSelection hxc_tmp_call_result_n5 = hxc_caxecraft_app_CaxecraftApp_selectPlayableActors(hxc_loadedLevel.hxc_actors);
  struct hxc_caxecraft_app_CaxecraftApp_PlayableActorSelection hxc_actors = hxc_tmp_call_result_n5;
  if (!hxc_actors.hxc_valid)
  {
    hxc_record_c82cbeac_destroy(&hxc_loadedLevel);
    if (hxc_gc_root_frame_pop(&hxc_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return;
  }
  int32_t hxc_waterPresentationCell = hxc_loadedLevel.hxc_waterPresentationCell;
  int32_t hxc_dialogueActorId = hxc_actors.hxc_dialogueActorId;
  int32_t hxc_enemyActorId = hxc_actors.hxc_enemyActorId;
  int32_t hxc_windowFlags = hxc_i32_bit_or(64, 4);
  hxc_windowFlags = hxc_i32_bit_or(hxc_windowFlags, 8192);
  SetConfigFlags((uint32_t)hxc_windowFlags);
  InitWindow((int32_t)1280, (int32_t)720, "Caxecraft \342\200\224 Haxe shaped into careful C");
  bool hxc_tmp_native_call_result_n12 = IsWindowReady();
  if (!hxc_tmp_native_call_result_n12)
  {
    hxc_record_c82cbeac_destroy(&hxc_loadedLevel);
    if (hxc_gc_root_frame_pop(&hxc_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return;
  }
  SetWindowMinSize((int32_t)800, (int32_t)450);
  SetExitKey((int32_t)0);
  SetTargetFPS((int32_t)120);
  EnableCursor();
  struct Texture hxc_tmp_native_call_result_n13 = LoadTexture("assets/showcase/title-panorama.png");
  struct Texture hxc_titleTexture = hxc_tmp_native_call_result_n13;
  bool hxc_tmp_native_call_result_n15 = IsTextureValid(hxc_titleTexture);
  bool hxc_titleTextureReady = hxc_tmp_native_call_result_n15;
  struct Texture hxc_tmp_native_call_result_n16 = LoadTexture("assets/branding/caxecraft-wordmark.png");
  struct Texture hxc_wordmarkTexture = hxc_tmp_native_call_result_n16;
  bool hxc_tmp_native_call_result_n18 = IsTextureValid(hxc_wordmarkTexture);
  bool hxc_wordmarkTextureReady = hxc_tmp_native_call_result_n18;
  struct Texture hxc_tmp_native_call_result_n19 = LoadTexture("assets/atlases/hud.png");
  struct Texture hxc_hudTexture = hxc_tmp_native_call_result_n19;
  bool hxc_tmp_native_call_result_n21 = IsTextureValid(hxc_hudTexture);
  bool hxc_hudTextureReady = hxc_tmp_native_call_result_n21;
  struct Texture hxc_tmp_native_call_result_n22 = LoadTexture("assets/atlases/items.png");
  struct Texture hxc_itemTexture = hxc_tmp_native_call_result_n22;
  bool hxc_tmp_native_call_result_n24 = IsTextureValid(hxc_itemTexture);
  bool hxc_itemTextureReady = hxc_tmp_native_call_result_n24;
  struct Texture hxc_tmp_native_call_result_n25 = LoadTexture("assets/atlases/adventure-items.png");
  struct Texture hxc_adventureItemTexture = hxc_tmp_native_call_result_n25;
  bool hxc_tmp_native_call_result_n27 = IsTextureValid(hxc_adventureItemTexture);
  bool hxc_adventureItemTextureReady = hxc_tmp_native_call_result_n27;
  struct Texture hxc_tmp_load_result_n28 = hxc_hudTexture;
  bool hxc_tmp_load_result_n29 = hxc_hudTextureReady;
  struct Texture hxc_tmp_load_result_n30 = hxc_itemTexture;
  bool hxc_tmp_load_result_n31 = hxc_itemTextureReady;
  struct Texture hxc_tmp_load_result_n32 = hxc_adventureItemTexture;
  struct hxc_caxecraft_app_HudResources hxc_hudResources = (struct hxc_caxecraft_app_HudResources){ .hxc_adventureItemTexture = hxc_tmp_load_result_n32, .hxc_adventureItemTextureReady = hxc_adventureItemTextureReady, .hxc_hudTexture = hxc_tmp_load_result_n28, .hxc_hudTextureReady = hxc_tmp_load_result_n29, .hxc_itemTexture = hxc_tmp_load_result_n30, .hxc_itemTextureReady = hxc_tmp_load_result_n31 };
  struct Texture hxc_tmp_native_call_result_n35 = LoadTexture("assets/atlases/entities.png");
  struct Texture hxc_entityTexture = hxc_tmp_native_call_result_n35;
  bool hxc_tmp_native_call_result_n37 = IsTextureValid(hxc_entityTexture);
  bool hxc_entityTextureReady = hxc_tmp_native_call_result_n37;
  struct Texture hxc_tmp_native_call_result_n38 = LoadTexture("assets/atlases/terrain.png");
  struct Texture hxc_terrainTexture = hxc_tmp_native_call_result_n38;
  bool hxc_tmp_native_call_result_n40 = IsTextureValid(hxc_terrainTexture);
  bool hxc_terrainTextureReady = hxc_tmp_native_call_result_n40;
  struct Texture hxc_tmp_native_call_result_n41 = LoadTexture("assets/atlases/adventure-terrain.png");
  struct Texture hxc_adventureTerrainTexture = hxc_tmp_native_call_result_n41;
  bool hxc_tmp_native_call_result_n43 = IsTextureValid(hxc_adventureTerrainTexture);
  bool hxc_adventureTerrainTextureReady = hxc_tmp_native_call_result_n43;
  double hxc_cameraWaterBlend = 0.0;
  struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_call_result_n44 = hxc_caxecraft_gameplay_Inventory_starter();
  struct hxc_caxecraft_gameplay_InventoryState hxc_inventory = hxc_tmp_call_result_n44;
  enum hxc_caxecraft_gameplay_GuidePhase hxc_guidePhase = hxc_caxecraft_gameplay_GuidePhase_Waiting;
  struct hxc_caxecraft_domain_GameSession *hxc_tmp_owned_class_field_address_n46 = &(*hxc_self).hxc_session;
  if (hxc_tmp_owned_class_field_address_n46 == NULL)
  {
    abort();
  }
  bool hxc_tmp_instance_call_result_n48 = hxc_caxecraft_domain_GameSession_actorInteractionAvailable(hxc_tmp_owned_class_field_address_n46, hxc_dialogueActorId);
  bool hxc_guideInteractionAvailable = hxc_tmp_instance_call_result_n48;
  struct hxc_caxecraft_domain_GameSession *hxc_tmp_owned_class_field_address_n49 = &(*hxc_self).hxc_session;
  if (hxc_tmp_owned_class_field_address_n49 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_domain_Character hxc_tmp_instance_call_result_n51 = hxc_caxecraft_domain_GameSession_readCharacter(hxc_tmp_owned_class_field_address_n49, hxc_dialogueActorId);
  struct hxc_caxecraft_domain_Character hxc_dialogueActor = hxc_tmp_instance_call_result_n51;
  struct hxc_caxecraft_domain_GameSession *hxc_tmp_owned_class_field_address_n52 = &(*hxc_self).hxc_session;
  if (hxc_tmp_owned_class_field_address_n52 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_domain_Character hxc_tmp_instance_call_result_n54 = hxc_caxecraft_domain_GameSession_readCharacter(hxc_tmp_owned_class_field_address_n52, hxc_enemyActorId);
  struct hxc_caxecraft_domain_Character hxc_enemyActor = hxc_tmp_instance_call_result_n54;
  struct hxc_caxecraft_domain_GameSession *hxc_tmp_owned_class_field_address_n55 = &(*hxc_self).hxc_session;
  if (hxc_tmp_owned_class_field_address_n55 == NULL)
  {
    abort();
  }
  struct hxc_array_ref *hxc_tmp_instance_call_result_n56 = hxc_caxecraft_domain_GameSession_actorControllerStateSnapshots(hxc_tmp_owned_class_field_address_n55);
  hxc_initialActorPhases = hxc_tmp_instance_call_result_n56;
  struct hxc_array_ref *hxc_tmp_load_result_n57 = hxc_initialActorPhases;
  struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation hxc_tmp_call_result_n60 = hxc_caxecraft_app_CaxecraftApp_observeActorPhase(hxc_tmp_load_result_n57, hxc_dialogueActorId, hxc_caxecraft_domain_ActorControllerPhase_Stationary);
  struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation hxc_dialoguePhase = hxc_tmp_call_result_n60;
  struct hxc_array_ref *hxc_tmp_load_result_n61 = hxc_initialActorPhases;
  struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation hxc_tmp_call_result_n64 = hxc_caxecraft_app_CaxecraftApp_observeActorPhase(hxc_tmp_load_result_n61, hxc_enemyActorId, hxc_caxecraft_domain_ActorControllerPhase_Resting);
  struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation hxc_enemyPhase = hxc_tmp_call_result_n64;
  int32_t hxc_tmp_record_field_load_result_n65 = hxc_dialogueActor.hxc_id;
  bool hxc_tmp_short_circuit_result_n34 = hxc_tmp_record_field_load_result_n65 <= 0;
  if (!(hxc_tmp_record_field_load_result_n65 <= 0))
  {
    hxc_tmp_short_circuit_result_n34 = hxc_enemyActor.hxc_id <= 0;
  }
  bool hxc_tmp_short_circuit_load_result_n67 = hxc_tmp_short_circuit_result_n34;
  bool hxc_tmp_short_circuit_result_n35 = hxc_tmp_short_circuit_load_result_n67;
  if (!hxc_tmp_short_circuit_load_result_n67)
  {
    hxc_tmp_short_circuit_result_n35 = !hxc_dialoguePhase.hxc_valid;
  }
  bool hxc_tmp_short_circuit_load_result_n69 = hxc_tmp_short_circuit_result_n35;
  bool hxc_tmp_short_circuit_result_n36 = hxc_tmp_short_circuit_load_result_n69;
  if (!hxc_tmp_short_circuit_load_result_n69)
  {
    hxc_tmp_short_circuit_result_n36 = !hxc_enemyPhase.hxc_valid;
  }
  if (hxc_tmp_short_circuit_result_n36)
  {
    if (hxc_array_ref_release(hxc_initialActorPhases) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_record_c82cbeac_destroy(&hxc_loadedLevel);
    if (hxc_gc_root_frame_pop(&hxc_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return;
  }
  struct hxc_caxecraft_gameplay_SwordCombatState hxc_swordCombat = (struct hxc_caxecraft_gameplay_SwordCombatState){ .hxc_cooldownTicks = 0 };
  struct hxc_caxecraft_gameplay_BerryDropState hxc_berryDrop = (struct hxc_caxecraft_gameplay_BerryDropState){ .hxc_active = false, .hxc_amount = 0, .hxc_x = 0.0, .hxc_y = 0.0, .hxc_z = 0.0 };
  double hxc_lookX = 0.0;
  double hxc_lookY = -0.18;
  double hxc_lookZ = -1.0;
  double hxc_accumulator = 0.0;
  struct hxc_caxecraft_domain_GameSession *hxc_tmp_owned_class_field_address_n74 = &(*hxc_self).hxc_session;
  if (hxc_tmp_owned_class_field_address_n74 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_domain_GameView hxc_tmp_instance_call_result_n75 = hxc_caxecraft_domain_GameSession_view(hxc_tmp_owned_class_field_address_n74);
  struct hxc_caxecraft_domain_GameView hxc_initialPresentation = hxc_tmp_instance_call_result_n75;
  if (!hxc_initialPresentation.hxc_valid)
  {
    if (hxc_array_ref_release(hxc_initialActorPhases) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_record_c82cbeac_destroy(&hxc_loadedLevel);
    if (hxc_gc_root_frame_pop(&hxc_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return;
  }
  struct hxc_caxecraft_app_MotionHistory hxc_tmp_call_result_n78 = hxc_caxecraft_app_MotionInterpolation_start(hxc_initialPresentation.hxc_localPlayer.hxc_body);
  struct hxc_caxecraft_app_MotionHistory hxc_motionHistory = hxc_tmp_call_result_n78;
  bool hxc_jumpQueued = false;
  bool hxc_swordQueued = false;
  int32_t hxc_selectedMode = 0;
  int32_t hxc_locale = 0;
  bool hxc_showInitialTitle = true;
  enum hxc_caxecraft_app_AppScreen hxc_tmp_call_result_n80 = hxc_caxecraft_app_AppScreen_initialScreen(hxc_showInitialTitle);
  enum hxc_caxecraft_app_AppScreen hxc_screen = hxc_tmp_call_result_n80;
  bool hxc_tmp_call_result_n82 = hxc_caxecraft_app_AppScreen_capturesPointer(hxc_screen);
  if (hxc_tmp_call_result_n82)
  {
    DisableCursor();
  }
  bool hxc_quit = false;
  int32_t hxc_frameCount = 0;
  int32_t hxc_placementBlockedFrames = 0;
  int32_t hxc_strikeHitFrames = 0;
  int32_t hxc_enemyDefeatedFrames = 0;
  int32_t hxc_enemyAttackFrames = 0;
  int32_t hxc_pickupFrames = 0;
  int32_t hxc_pickupAmount = 0;
  int32_t hxc_inventoryFullFrames = 0;
  int32_t hxc_inventoryFullReason = 0;
  int32_t hxc_recoveryFeedback = 0;
  int32_t hxc_recoveryFeedbackFrames = 0;
  int32_t hxc_aquaticEquipmentCode = -1;
  int32_t hxc_aquaticEquipmentFrames = 0;
  while (1)
  {
    bool hxc_tmp_load_result_n83 = hxc_quit;
    bool hxc_tmp_short_circuit_result_n65 = !hxc_tmp_load_result_n83;
    if (!hxc_tmp_load_result_n83)
    {
      bool hxc_tmp_native_call_result_n84 = WindowShouldClose();
      hxc_tmp_short_circuit_result_n65 = !hxc_tmp_native_call_result_n84;
    }
    if (!hxc_tmp_short_circuit_result_n65)
    {
      break;
    }
    struct hxc_caxecraft_domain_GameSession *hxc_tmp_owned_class_field_address_n86 = &(*hxc_self).hxc_session;
    if (hxc_tmp_owned_class_field_address_n86 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_domain_GameView hxc_tmp_instance_call_result_n87 = hxc_caxecraft_domain_GameSession_view(hxc_tmp_owned_class_field_address_n86);
    struct hxc_caxecraft_domain_GameView hxc_initialView = hxc_tmp_instance_call_result_n87;
    if (!hxc_initialView.hxc_valid)
    {
      hxc_quit = true;
    }
    struct hxc_caxecraft_domain_Character hxc_character = hxc_initialView.hxc_localPlayer;
    bool hxc_recapturedThisFrame = false;
    bool hxc_resetMotionThisFrame = false;
    float hxc_tmp_native_call_result_n90 = GetFrameTime();
    double hxc_frameSeconds = (double)hxc_tmp_native_call_result_n90;
    if (hxc_frameSeconds > 0.25)
    {
      hxc_frameSeconds = 0.25;
    }
    bool hxc_tmp_native_call_result_n92 = IsWindowFocused();
    bool hxc_focused = hxc_tmp_native_call_result_n92;
    bool hxc_tmp_call_result_n94 = hxc_caxecraft_app_AppScreen_capturesPointer(hxc_screen);
    bool hxc_captured_h7fbd8ac97dbd = hxc_tmp_call_result_n94;
    bool hxc_tmp_call_result_n96 = hxc_caxecraft_app_AppScreen_pausesSimulation(hxc_screen);
    bool hxc_paused_h99839489a7f5 = hxc_tmp_call_result_n96;
    double hxc_forward = 0.0;
    double hxc_right = 0.0;
    bool hxc_tmp_native_call_result_n97 = IsKeyDown((int32_t)87);
    if (hxc_tmp_native_call_result_n97)
    {
      hxc_forward = hxc_forward + 1.0;
    }
    bool hxc_tmp_native_call_result_n99 = IsKeyDown((int32_t)83);
    if (hxc_tmp_native_call_result_n99)
    {
      hxc_forward = hxc_forward - 1.0;
    }
    bool hxc_tmp_native_call_result_n101 = IsKeyDown((int32_t)68);
    if (hxc_tmp_native_call_result_n101)
    {
      hxc_right = hxc_right + 1.0;
    }
    bool hxc_tmp_native_call_result_n103 = IsKeyDown((int32_t)65);
    if (hxc_tmp_native_call_result_n103)
    {
      hxc_right = hxc_right - 1.0;
    }
    double hxc_lookYaw_h85cd75827812 = 0.0;
    double hxc_lookPitch_h277668977343 = 0.0;
    if (hxc_captured_h7fbd8ac97dbd)
    {
      struct Vector2 hxc_tmp_native_call_result_n106 = GetMouseDelta();
      struct Vector2 hxc_mouse = hxc_tmp_native_call_result_n106;
      hxc_lookYaw_h85cd75827812 = -(double)hxc_mouse.x * 0.0025;
      hxc_lookPitch_h277668977343 = -(double)hxc_mouse.y * 0.0025;
    }
    bool hxc_tmp_native_call_result_n111 = IsMouseButtonPressed((int32_t)0);
    bool hxc_leftPressed = hxc_tmp_native_call_result_n111;
    bool hxc_tmp_load_result_n112 = hxc_captured_h7fbd8ac97dbd;
    bool hxc_tmp_short_circuit_result_n81 = hxc_tmp_load_result_n112;
    if (hxc_tmp_load_result_n112)
    {
      hxc_tmp_short_circuit_result_n81 = hxc_leftPressed;
    }
    bool hxc_primaryPressed_h25deefcf2fe8 = hxc_tmp_short_circuit_result_n81;
    bool hxc_tmp_load_result_n115 = hxc_captured_h7fbd8ac97dbd;
    bool hxc_tmp_short_circuit_result_n83 = hxc_tmp_load_result_n115;
    if (hxc_tmp_load_result_n115)
    {
      bool hxc_tmp_native_call_result_n116 = IsMouseButtonPressed((int32_t)1);
      hxc_tmp_short_circuit_result_n83 = hxc_tmp_native_call_result_n116;
    }
    bool hxc_secondaryPressed_h9eaf291a2229 = hxc_tmp_short_circuit_result_n83;
    bool hxc_tmp_load_result_n118 = hxc_captured_h7fbd8ac97dbd;
    bool hxc_tmp_short_circuit_result_n85 = hxc_tmp_load_result_n118;
    if (hxc_tmp_load_result_n118)
    {
      bool hxc_tmp_native_call_result_n119 = IsKeyPressed((int32_t)69);
      hxc_tmp_short_circuit_result_n85 = hxc_tmp_native_call_result_n119;
    }
    bool hxc_interactPressed_h2743668ffff2 = hxc_tmp_short_circuit_result_n85;
    int32_t hxc_hotbarSelection_hdd87000778bb = -1;
    bool hxc_tmp_native_call_result_n121 = IsKeyPressed((int32_t)49);
    if (hxc_tmp_native_call_result_n121)
    {
      hxc_hotbarSelection_hdd87000778bb = 0;
    }
    bool hxc_tmp_native_call_result_n122 = IsKeyPressed((int32_t)50);
    if (hxc_tmp_native_call_result_n122)
    {
      hxc_hotbarSelection_hdd87000778bb = 1;
    }
    bool hxc_tmp_native_call_result_n123 = IsKeyPressed((int32_t)51);
    if (hxc_tmp_native_call_result_n123)
    {
      hxc_hotbarSelection_hdd87000778bb = 2;
    }
    bool hxc_tmp_native_call_result_n124 = IsKeyPressed((int32_t)52);
    if (hxc_tmp_native_call_result_n124)
    {
      hxc_hotbarSelection_hdd87000778bb = 3;
    }
    bool hxc_tmp_native_call_result_n125 = IsKeyPressed((int32_t)53);
    if (hxc_tmp_native_call_result_n125)
    {
      hxc_hotbarSelection_hdd87000778bb = 4;
    }
    bool hxc_tmp_native_call_result_n126 = IsKeyPressed((int32_t)54);
    if (hxc_tmp_native_call_result_n126)
    {
      hxc_hotbarSelection_hdd87000778bb = 5;
    }
    bool hxc_tmp_native_call_result_n127 = IsKeyPressed((int32_t)55);
    if (hxc_tmp_native_call_result_n127)
    {
      hxc_hotbarSelection_hdd87000778bb = 6;
    }
    bool hxc_tmp_native_call_result_n128 = IsKeyPressed((int32_t)56);
    if (hxc_tmp_native_call_result_n128)
    {
      hxc_hotbarSelection_hdd87000778bb = 7;
    }
    float hxc_tmp_native_call_result_n129 = GetMouseWheelMove();
    double hxc_wheel = (double)hxc_tmp_native_call_result_n129;
    int32_t hxc_hotbarCycle_h1c1164cb9f19 = 0;
    if (hxc_wheel > 0.0)
    {
      hxc_hotbarCycle_h1c1164cb9f19 = -1;
    }
    if (hxc_wheel < 0.0)
    {
      hxc_hotbarCycle_h1c1164cb9f19 = 1;
    }
    bool hxc_tmp_native_call_result_n132 = IsKeyPressed((int32_t)32);
    bool hxc_jumpPressed_h7ade2adc82a8 = hxc_tmp_native_call_result_n132;
    bool hxc_tmp_native_call_result_n133 = IsKeyPressed((int32_t)256);
    bool hxc_pausePressed_h1ad8627623a2 = hxc_tmp_native_call_result_n133;
    bool hxc_tmp_native_call_result_n134 = IsKeyPressed((int32_t)81);
    bool hxc_quitPressed_h9108215560c4 = hxc_tmp_native_call_result_n134;
    double hxc_frameInput_moveForward = hxc_forward;
    double hxc_frameInput_moveRight = hxc_right;
    double hxc_frameInput_lookYaw = hxc_lookYaw_h85cd75827812;
    double hxc_frameInput_lookPitch = hxc_lookPitch_h277668977343;
    bool hxc_frameInput_jumpPressed = hxc_jumpPressed_h7ade2adc82a8;
    bool hxc_tmp_native_call_result_n140 = IsKeyDown((int32_t)340);
    bool hxc_frameInput_descendHeld = hxc_tmp_native_call_result_n140;
    bool hxc_frameInput_primaryPressed = hxc_primaryPressed_h25deefcf2fe8;
    bool hxc_frameInput_secondaryPressed = hxc_secondaryPressed_h9eaf291a2229;
    bool hxc_frameInput_interactPressed = hxc_interactPressed_h2743668ffff2;
    bool hxc_frameInput_pausePressed = hxc_pausePressed_h1ad8627623a2;
    bool hxc_tmp_load_result_n145 = hxc_paused_h99839489a7f5;
    bool hxc_tmp_short_circuit_result_n103 = hxc_tmp_load_result_n145;
    if (hxc_tmp_load_result_n145)
    {
      hxc_tmp_short_circuit_result_n103 = hxc_leftPressed;
    }
    bool hxc_frameInput_capturePressed = hxc_tmp_short_circuit_result_n103;
    bool hxc_frameInput_quitPressed = hxc_quitPressed_h9108215560c4;
    int32_t hxc_frameInput_hotbarSelection = hxc_hotbarSelection_hdd87000778bb;
    int32_t hxc_frameInput_hotbarCycle = hxc_hotbarCycle_h1c1164cb9f19;
    double hxc_moveForward = hxc_frameInput_moveForward;
    double hxc_moveRight = hxc_frameInput_moveRight;
    double hxc_lookYaw_h5e9cb1b1577d = hxc_frameInput_lookYaw;
    double hxc_lookPitch_h0c5bdc451e3f = hxc_frameInput_lookPitch;
    bool hxc_jumpPressed_h512fcc1aafd9 = hxc_frameInput_jumpPressed;
    bool hxc_primaryPressed_haad28c32f864 = hxc_frameInput_primaryPressed;
    bool hxc_secondaryPressed_h010b204dcf5c = hxc_frameInput_secondaryPressed;
    bool hxc_interactPressed_h1eb51d653d5f = hxc_frameInput_interactPressed;
    bool hxc_pausePressed_haabffc866c84 = hxc_frameInput_pausePressed;
    bool hxc_capturePressed = hxc_frameInput_capturePressed;
    bool hxc_quitPressed_h62f97c5a1a50 = hxc_frameInput_quitPressed;
    int32_t hxc_hotbarSelection_h9ff7ff027620 = hxc_frameInput_hotbarSelection;
    int32_t hxc_hotbarCycle_h940eb84b6a87 = hxc_frameInput_hotbarCycle;
    bool hxc_descendHeld = hxc_frameInput_descendHeld;
    if (hxc_quitPressed_h62f97c5a1a50)
    {
      hxc_quit = true;
    }
    if (hxc_hotbarSelection_h9ff7ff027620 >= 0)
    {
      struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_load_result_n167 = hxc_inventory;
      struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_call_result_n169 = hxc_caxecraft_gameplay_Inventory_select(hxc_tmp_load_result_n167, hxc_hotbarSelection_h9ff7ff027620);
      hxc_inventory = hxc_tmp_call_result_n169;
    }
    if (hxc_hotbarCycle_h940eb84b6a87 != 0)
    {
      struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_load_result_n171 = hxc_inventory;
      struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_call_result_n173 = hxc_caxecraft_gameplay_Inventory_cycle(hxc_tmp_load_result_n171, hxc_hotbarCycle_h940eb84b6a87);
      hxc_inventory = hxc_tmp_call_result_n173;
    }
    bool hxc_tmp_call_result_n175 = hxc_caxecraft_app_AppScreen_isPlaying(hxc_screen);
    bool hxc_tmp_short_circuit_result_n121 = hxc_tmp_call_result_n175;
    if (hxc_tmp_call_result_n175)
    {
      hxc_tmp_short_circuit_result_n121 = hxc_interactPressed_h1eb51d653d5f;
    }
    if (hxc_tmp_short_circuit_result_n121)
    {
      if (hxc_character.hxc_vitals.hxc_health <= 0)
      {
        struct hxc_caxecraft_domain_GameSession *hxc_tmp_owned_class_field_address_n179 = &(*hxc_self).hxc_session;
        if (hxc_tmp_owned_class_field_address_n179 == NULL)
        {
          abort();
        }
        struct hxc_caxecraft_domain_GameSession *hxc_tmp_owned_class_field_address_n180 = &(*hxc_self).hxc_session;
        if (hxc_tmp_owned_class_field_address_n180 == NULL)
        {
          abort();
        }
        size_t hxc_tmp_length_n182;
        const uint8_t *hxc_tmp_instance_call_result_n181 = hxc_caxecraft_domain_GameSession_worldView(hxc_tmp_owned_class_field_address_n180, &hxc_tmp_length_n182);
        struct hxc_caxecraft_domain_CharacterBody hxc_tmp_call_result_n183 = hxc_caxecraft_app_CaxecraftApp_spawnPlayer(hxc_tmp_instance_call_result_n181, hxc_tmp_length_n182);
        struct hxc_caxecraft_domain_LocalCharacterCommandResult hxc_tmp_instance_call_result_n184 = hxc_caxecraft_domain_GameSession_reviveLocalPlayerAt(hxc_tmp_owned_class_field_address_n179, hxc_tmp_call_result_n183);
        struct hxc_caxecraft_domain_LocalCharacterCommandResult hxc_revival = hxc_tmp_instance_call_result_n184;
        hxc_character = hxc_revival.hxc_character;
        if (!hxc_revival.hxc_resolved)
        {
          hxc_quit = true;
        }
        else
        {
          hxc_cameraWaterBlend = 0.0;
          hxc_accumulator = 0.0;
          hxc_resetMotionThisFrame = true;
        }
      }
      else
      {
        struct hxc_caxecraft_domain_GameSession *hxc_tmp_owned_class_field_address_n187 = &(*hxc_self).hxc_session;
        if (hxc_tmp_owned_class_field_address_n187 == NULL)
        {
          abort();
        }
        bool hxc_tmp_instance_call_result_n189 = hxc_caxecraft_domain_GameSession_actorInteractionAvailable(hxc_tmp_owned_class_field_address_n187, hxc_dialogueActorId);
        if (hxc_tmp_instance_call_result_n189)
        {
          bool hxc_sharesBerries = hxc_guidePhase == hxc_caxecraft_gameplay_GuidePhase_Welcomed;
          if (hxc_sharesBerries)
          {
            int32_t hxc_tmp_call_result_n195 = hxc_caxecraft_gameplay_Inventory_acceptedAmount(hxc_inventory, hxc_caxecraft_gameplay_ItemKind_Berries, 2);
            int32_t hxc_acceptedGift = hxc_tmp_call_result_n195;
            if (hxc_acceptedGift == 2)
            {
              struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_load_result_n197 = hxc_inventory;
              struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_call_result_n200 = hxc_caxecraft_gameplay_Inventory_collectItem(hxc_tmp_load_result_n197, hxc_caxecraft_gameplay_ItemKind_Berries, hxc_acceptedGift);
              hxc_inventory = hxc_tmp_call_result_n200;
              enum hxc_caxecraft_gameplay_GuidePhase hxc_tmp_call_result_n202 = hxc_caxecraft_app_CaxecraftApp_advanceGuidePhase(hxc_guidePhase);
              hxc_guidePhase = hxc_tmp_call_result_n202;
            }
            else
            {
              hxc_inventoryFullReason = 1;
              hxc_inventoryFullFrames = 90;
            }
          }
          else
          {
            enum hxc_caxecraft_gameplay_GuidePhase hxc_tmp_call_result_n204 = hxc_caxecraft_app_CaxecraftApp_advanceGuidePhase(hxc_guidePhase);
            hxc_guidePhase = hxc_tmp_call_result_n204;
          }
        }
      }
    }
    bool hxc_tmp_call_result_n206 = hxc_caxecraft_app_AppScreen_showsTitle(hxc_screen);
    bool hxc_tmp_short_circuit_result_n125 = hxc_tmp_call_result_n206;
    if (hxc_tmp_call_result_n206)
    {
      hxc_tmp_short_circuit_result_n125 = hxc_focused;
    }
    if (hxc_tmp_short_circuit_result_n125)
    {
      int32_t hxc_modeBeforeInput = hxc_selectedMode;
      bool hxc_tmp_native_call_result_n210 = IsKeyPressed((int32_t)76);
      if (hxc_tmp_native_call_result_n210)
      {
        int32_t hxc_tmp_call_result_n212 = hxc_caxecraft_localization_UiCatalog_nextLocale(hxc_locale);
        hxc_locale = hxc_tmp_call_result_n212;
      }
      bool hxc_tmp_native_call_result_n213 = IsKeyPressed((int32_t)265);
      bool hxc_tmp_short_circuit_result_n127 = hxc_tmp_native_call_result_n213;
      if (!hxc_tmp_native_call_result_n213)
      {
        bool hxc_tmp_native_call_result_n214 = IsKeyPressed((int32_t)264);
        hxc_tmp_short_circuit_result_n127 = hxc_tmp_native_call_result_n214;
      }
      if (hxc_tmp_short_circuit_result_n127)
      {
        int32_t hxc_tmp_load_result_n216 = hxc_selectedMode;
        int32_t hxc_tmp_conditional_result_n128 = 0;
        if (hxc_tmp_load_result_n216 == 0)
        {
          hxc_tmp_conditional_result_n128 = 1;
        }
        else
        {
          hxc_tmp_conditional_result_n128 = 0;
        }
        hxc_selectedMode = hxc_tmp_conditional_result_n128;
      }
      struct Vector2 hxc_tmp_native_call_result_n218 = GetMousePosition();
      struct Vector2 hxc_menuMouse = hxc_tmp_native_call_result_n218;
      struct Vector2 hxc_tmp_load_result_n219 = hxc_menuMouse;
      struct Vector2 hxc_tmp_load_result_n221 = hxc_menuMouse;
      int32_t hxc_tmp_native_call_result_n223 = GetScreenWidth();
      int32_t hxc_tmp_native_call_result_n224 = GetScreenHeight();
      int32_t hxc_tmp_call_result_n225 = hxc_caxecraft_app_TitleMenu_selectionAt((double)hxc_tmp_load_result_n219.x, (double)hxc_tmp_load_result_n221.y, (int32_t)hxc_tmp_native_call_result_n223, (int32_t)hxc_tmp_native_call_result_n224);
      int32_t hxc_hovered = hxc_tmp_call_result_n225;
      if (hxc_hovered == 0)
      {
        hxc_selectedMode = 0;
      }
      if (hxc_hovered == 1)
      {
        hxc_selectedMode = 1;
      }
      int32_t hxc_tmp_load_result_n228 = hxc_selectedMode;
      if (hxc_tmp_load_result_n228 != hxc_modeBeforeInput)
      {
        hxc_accumulator = 0.0;
        hxc_resetMotionThisFrame = true;
      }
      int32_t hxc_tmp_load_result_n230 = hxc_hovered;
      bool hxc_tmp_short_circuit_result_n132 = hxc_tmp_load_result_n230 >= 0;
      if (hxc_tmp_load_result_n230 >= 0)
      {
        bool hxc_tmp_native_call_result_n231 = IsMouseButtonPressed((int32_t)0);
        hxc_tmp_short_circuit_result_n132 = hxc_tmp_native_call_result_n231;
      }
      bool hxc_clickedChoice = hxc_tmp_short_circuit_result_n132;
      bool hxc_tmp_load_result_n233 = hxc_clickedChoice;
      bool hxc_tmp_short_circuit_result_n133 = hxc_tmp_load_result_n233;
      if (hxc_tmp_load_result_n233)
      {
        hxc_tmp_short_circuit_result_n133 = hxc_hovered == 2;
      }
      if (hxc_tmp_short_circuit_result_n133)
      {
        enum hxc_caxecraft_app_AppScreen hxc_tmp_call_result_n237 = hxc_caxecraft_app_AppScreen_openEditor(hxc_screen);
        hxc_screen = hxc_tmp_call_result_n237;
        hxc_accumulator = 0.0;
        hxc_resetMotionThisFrame = true;
        EnableCursor();
      }
      else
      {
        bool hxc_tmp_load_result_n238 = hxc_clickedChoice;
        bool hxc_tmp_short_circuit_result_n134 = hxc_tmp_load_result_n238;
        if (!hxc_tmp_load_result_n238)
        {
          bool hxc_tmp_native_call_result_n239 = IsKeyPressed((int32_t)257);
          hxc_tmp_short_circuit_result_n134 = hxc_tmp_native_call_result_n239;
        }
        if (hxc_tmp_short_circuit_result_n134)
        {
          enum hxc_caxecraft_app_AppScreen hxc_tmp_call_result_n242 = hxc_caxecraft_app_AppScreen_startPlaying(hxc_screen);
          hxc_screen = hxc_tmp_call_result_n242;
          hxc_recapturedThisFrame = true;
          hxc_accumulator = 0.0;
          hxc_resetMotionThisFrame = true;
          DisableCursor();
        }
      }
    }
    bool hxc_tmp_load_result_n243 = hxc_focused;
    bool hxc_tmp_short_circuit_result_n135 = !hxc_tmp_load_result_n243;
    if (!hxc_tmp_load_result_n243)
    {
      bool hxc_tmp_call_result_n245 = hxc_caxecraft_app_AppScreen_isPlaying(hxc_screen);
      hxc_tmp_short_circuit_result_n135 = hxc_tmp_call_result_n245;
    }
    if (hxc_tmp_short_circuit_result_n135)
    {
      enum hxc_caxecraft_app_AppScreen hxc_tmp_call_result_n248 = hxc_caxecraft_app_AppScreen_loseFocus(hxc_screen);
      hxc_screen = hxc_tmp_call_result_n248;
      hxc_jumpQueued = false;
      hxc_accumulator = 0.0;
      hxc_resetMotionThisFrame = true;
      EnableCursor();
    }
    bool hxc_tmp_call_result_n250 = hxc_caxecraft_app_AppScreen_showsTitle(hxc_screen);
    bool hxc_tmp_short_circuit_result_n136 = !hxc_tmp_call_result_n250;
    if (!hxc_tmp_call_result_n250)
    {
      bool hxc_tmp_call_result_n252 = hxc_caxecraft_app_AppScreen_showsEditor(hxc_screen);
      hxc_tmp_short_circuit_result_n136 = !hxc_tmp_call_result_n252;
    }
    bool hxc_tmp_short_circuit_load_result_n253 = hxc_tmp_short_circuit_result_n136;
    bool hxc_tmp_short_circuit_result_n137 = hxc_tmp_short_circuit_load_result_n253;
    if (hxc_tmp_short_circuit_load_result_n253)
    {
      hxc_tmp_short_circuit_result_n137 = hxc_focused;
    }
    bool hxc_tmp_short_circuit_load_result_n255 = hxc_tmp_short_circuit_result_n137;
    bool hxc_tmp_short_circuit_result_n138 = hxc_tmp_short_circuit_load_result_n255;
    if (hxc_tmp_short_circuit_load_result_n255)
    {
      hxc_tmp_short_circuit_result_n138 = hxc_pausePressed_haabffc866c84;
    }
    if (hxc_tmp_short_circuit_result_n138)
    {
      enum hxc_caxecraft_app_AppScreen hxc_tmp_call_result_n259 = hxc_caxecraft_app_AppScreen_togglePause(hxc_screen);
      hxc_screen = hxc_tmp_call_result_n259;
      hxc_accumulator = 0.0;
      hxc_resetMotionThisFrame = true;
      bool hxc_tmp_call_result_n261 = hxc_caxecraft_app_AppScreen_pausesSimulation(hxc_screen);
      if (hxc_tmp_call_result_n261)
      {
        hxc_jumpQueued = false;
      }
      bool hxc_tmp_call_result_n263 = hxc_caxecraft_app_AppScreen_capturesPointer(hxc_screen);
      if (hxc_tmp_call_result_n263)
      {
        DisableCursor();
      }
      else
      {
        EnableCursor();
      }
    }
    bool hxc_tmp_call_result_n265 = hxc_caxecraft_app_AppScreen_showsTitle(hxc_screen);
    bool hxc_tmp_short_circuit_result_n139 = !hxc_tmp_call_result_n265;
    if (!hxc_tmp_call_result_n265)
    {
      bool hxc_tmp_call_result_n267 = hxc_caxecraft_app_AppScreen_showsEditor(hxc_screen);
      hxc_tmp_short_circuit_result_n139 = !hxc_tmp_call_result_n267;
    }
    bool hxc_tmp_short_circuit_load_result_n268 = hxc_tmp_short_circuit_result_n139;
    bool hxc_tmp_short_circuit_result_n140 = hxc_tmp_short_circuit_load_result_n268;
    if (hxc_tmp_short_circuit_load_result_n268)
    {
      hxc_tmp_short_circuit_result_n140 = hxc_focused;
    }
    bool hxc_tmp_short_circuit_load_result_n270 = hxc_tmp_short_circuit_result_n140;
    bool hxc_tmp_short_circuit_result_n141 = hxc_tmp_short_circuit_load_result_n270;
    if (hxc_tmp_short_circuit_load_result_n270)
    {
      bool hxc_tmp_call_result_n272 = hxc_caxecraft_app_AppScreen_pausesSimulation(hxc_screen);
      hxc_tmp_short_circuit_result_n141 = hxc_tmp_call_result_n272;
    }
    bool hxc_tmp_short_circuit_load_result_n273 = hxc_tmp_short_circuit_result_n141;
    bool hxc_tmp_short_circuit_result_n142 = hxc_tmp_short_circuit_load_result_n273;
    if (hxc_tmp_short_circuit_load_result_n273)
    {
      hxc_tmp_short_circuit_result_n142 = hxc_capturePressed;
    }
    if (hxc_tmp_short_circuit_result_n142)
    {
      enum hxc_caxecraft_app_AppScreen hxc_tmp_call_result_n277 = hxc_caxecraft_app_AppScreen_recapture(hxc_screen);
      hxc_screen = hxc_tmp_call_result_n277;
      hxc_recapturedThisFrame = true;
      hxc_accumulator = 0.0;
      hxc_resetMotionThisFrame = true;
      DisableCursor();
    }
    bool hxc_tmp_call_result_n279 = hxc_caxecraft_app_AppScreen_showsTitle(hxc_screen);
    bool hxc_onTitle = hxc_tmp_call_result_n279;
    bool hxc_tmp_call_result_n281 = hxc_caxecraft_app_AppScreen_showsEditor(hxc_screen);
    bool hxc_onEditor = hxc_tmp_call_result_n281;
    bool hxc_tmp_call_result_n283 = hxc_caxecraft_app_AppScreen_pausesSimulation(hxc_screen);
    bool hxc_paused_h86863acb5f2c = hxc_tmp_call_result_n283;
    bool hxc_tmp_call_result_n285 = hxc_caxecraft_app_AppScreen_capturesPointer(hxc_screen);
    bool hxc_captured_h94383143b373 = hxc_tmp_call_result_n285;
    int32_t hxc_editorNavigationCommand = 0;
    if (hxc_onEditor)
    {
      struct hxc_caxecraft_input_NavigationRepeater *hxc_tmp_owned_class_field_address_n287 = &(*hxc_self).hxc_editorNavigation;
      if (hxc_tmp_owned_class_field_address_n287 == NULL)
      {
        abort();
      }
      struct hxc_caxecraft_input_NavigationSample hxc_tmp_call_result_n288 = hxc_caxecraft_app_RaylibNavigationInput_samplePrimaryGamepad();
      int32_t hxc_tmp_instance_call_result_n290 = hxc_caxecraft_input_NavigationRepeater_advance(hxc_tmp_owned_class_field_address_n287, hxc_tmp_call_result_n288, hxc_frameSeconds);
      hxc_editorNavigationCommand = hxc_tmp_instance_call_result_n290;
    }
    else
    {
      struct hxc_caxecraft_input_NavigationRepeater *hxc_tmp_owned_class_field_address_n291 = &(*hxc_self).hxc_editorNavigation;
      if (hxc_tmp_owned_class_field_address_n291 == NULL)
      {
        abort();
      }
      hxc_caxecraft_input_NavigationRepeater_release(hxc_tmp_owned_class_field_address_n291);
    }
    if (hxc_captured_h94383143b373)
    {
      double hxc_yawDelta = hxc_lookYaw_h5e9cb1b1577d;
      if (hxc_yawDelta > 0.25)
      {
        hxc_yawDelta = 0.25;
      }
      if (hxc_yawDelta < -0.25)
      {
        hxc_yawDelta = -0.25;
      }
      double hxc_tmp_load_result_n296 = hxc_lookX;
      double hxc_tmp_load_result_n297 = hxc_yawDelta;
      double hxc_candidateX = hxc_tmp_load_result_n296 + hxc_tmp_load_result_n297 * hxc_lookZ;
      double hxc_tmp_load_result_n299 = hxc_lookZ;
      double hxc_tmp_load_result_n300 = hxc_yawDelta;
      double hxc_candidateZ = hxc_tmp_load_result_n299 - hxc_tmp_load_result_n300 * hxc_lookX;
      double hxc_tmp_load_result_n302 = hxc_candidateX;
      double hxc_tmp_load_result_n303 = hxc_candidateX;
      double hxc_tmp_load_result_n304 = hxc_candidateZ;
      double hxc_lengthSquared = hxc_tmp_load_result_n302 * hxc_tmp_load_result_n303 + hxc_tmp_load_result_n304 * hxc_candidateZ;
      double hxc_normalize = 1.5 - 0.5 * hxc_lengthSquared;
      double hxc_tmp_load_result_n307 = hxc_candidateX;
      hxc_lookX = hxc_tmp_load_result_n307 * hxc_normalize;
      double hxc_tmp_load_result_n309 = hxc_candidateZ;
      hxc_lookZ = hxc_tmp_load_result_n309 * hxc_normalize;
      double hxc_tmp_compound_load_result_n311 = hxc_lookY;
      hxc_lookY = hxc_tmp_compound_load_result_n311 + hxc_lookPitch_h0c5bdc451e3f;
      if (hxc_lookY > 0.90)
      {
        hxc_lookY = 0.90;
      }
      if (hxc_lookY < -0.90)
      {
        hxc_lookY = -0.90;
      }
      if (hxc_jumpPressed_h512fcc1aafd9)
      {
        hxc_jumpQueued = true;
      }
    }
    bool hxc_tmp_load_result_n316 = hxc_captured_h94383143b373;
    bool hxc_tmp_short_circuit_result_n153 = hxc_tmp_load_result_n316;
    if (hxc_tmp_load_result_n316)
    {
      hxc_tmp_short_circuit_result_n153 = !hxc_recapturedThisFrame;
    }
    bool hxc_tmp_short_circuit_load_result_n318 = hxc_tmp_short_circuit_result_n153;
    bool hxc_tmp_short_circuit_result_n154 = hxc_tmp_short_circuit_load_result_n318;
    if (hxc_tmp_short_circuit_load_result_n318)
    {
      hxc_tmp_short_circuit_result_n154 = hxc_primaryPressed_haad28c32f864;
    }
    bool hxc_tmp_short_circuit_load_result_n320 = hxc_tmp_short_circuit_result_n154;
    bool hxc_tmp_short_circuit_result_n155 = hxc_tmp_short_circuit_load_result_n320;
    if (hxc_tmp_short_circuit_load_result_n320)
    {
      hxc_tmp_short_circuit_result_n155 = hxc_selectedMode == 1;
    }
    bool hxc_tmp_short_circuit_load_result_n322 = hxc_tmp_short_circuit_result_n155;
    bool hxc_tmp_short_circuit_result_n156 = hxc_tmp_short_circuit_load_result_n322;
    if (hxc_tmp_short_circuit_load_result_n322)
    {
      bool hxc_tmp_call_result_n325 = hxc_caxecraft_gameplay_Inventory_selectedIs(hxc_inventory, hxc_caxecraft_gameplay_ItemKind_CopperSword);
      hxc_tmp_short_circuit_result_n156 = hxc_tmp_call_result_n325;
    }
    if (hxc_tmp_short_circuit_result_n156)
    {
      hxc_swordQueued = true;
    }
    if (!hxc_paused_h86863acb5f2c)
    {
      double hxc_tmp_compound_load_result_n328 = hxc_accumulator;
      hxc_accumulator = hxc_tmp_compound_load_result_n328 + hxc_frameSeconds;
    }
    while (1)
    {
      bool hxc_tmp_load_result_n330 = hxc_paused_h86863acb5f2c;
      bool hxc_tmp_short_circuit_result_n157 = !hxc_tmp_load_result_n330;
      if (!hxc_tmp_load_result_n330)
      {
        hxc_tmp_short_circuit_result_n157 = hxc_accumulator >= 0.05;
      }
      if (!hxc_tmp_short_circuit_result_n157)
      {
        break;
      }
      double hxc_tmp_load_result_n333 = hxc_moveForward;
      double hxc_tmp_load_result_n334 = hxc_lookX;
      double hxc_tmp_load_result_n335 = hxc_moveRight;
      double hxc_moveX = hxc_tmp_load_result_n333 * hxc_tmp_load_result_n334 - hxc_tmp_load_result_n335 * hxc_lookZ;
      double hxc_tmp_load_result_n337 = hxc_moveForward;
      double hxc_tmp_load_result_n338 = hxc_lookZ;
      double hxc_tmp_load_result_n339 = hxc_moveRight;
      double hxc_moveZ = hxc_tmp_load_result_n337 * hxc_tmp_load_result_n338 + hxc_tmp_load_result_n339 * hxc_lookX;
      double hxc_tmp_load_result_n341 = hxc_moveForward;
      bool hxc_tmp_short_circuit_result_n160 = hxc_tmp_load_result_n341 != 0.0;
      if (hxc_tmp_load_result_n341 != 0.0)
      {
        hxc_tmp_short_circuit_result_n160 = hxc_moveRight != 0.0;
      }
      if (hxc_tmp_short_circuit_result_n160)
      {
        hxc_moveX = hxc_moveX * 0.7071067811865476;
        hxc_moveZ = hxc_moveZ * 0.7071067811865476;
      }
      int32_t hxc_tmp_load_result_n346 = hxc_selectedMode;
      int32_t hxc_tmp_conditional_result_n162 = 0;
      if (hxc_tmp_load_result_n346 == 1)
      {
        hxc_tmp_conditional_result_n162 = 1;
      }
      else
      {
        hxc_tmp_conditional_result_n162 = 0;
      }
      int32_t hxc_damagePolicy = hxc_tmp_conditional_result_n162;
      struct hxc_caxecraft_domain_GameSession *hxc_tmp_owned_class_field_address_n348 = &(*hxc_self).hxc_session;
      if (hxc_tmp_owned_class_field_address_n348 == NULL)
      {
        abort();
      }
      double hxc_tmp_load_result_n349 = hxc_moveX;
      double hxc_tmp_load_result_n350 = hxc_moveZ;
      bool hxc_tmp_load_result_n351 = hxc_jumpQueued;
      struct hxc_caxecraft_domain_CharacterIntent hxc_tmp_call_result_n353 = hxc_caxecraft_domain_Aquatics_input(hxc_tmp_load_result_n349, hxc_tmp_load_result_n350, hxc_tmp_load_result_n351, hxc_descendHeld);
      struct hxc_caxecraft_domain_GameTickResult hxc_tmp_instance_call_result_n356 = hxc_caxecraft_domain_GameSession_tick(hxc_tmp_owned_class_field_address_n348, (struct hxc_caxecraft_domain_GameTickInput){ .hxc_damagePolicy = hxc_damagePolicy, .hxc_intent = hxc_tmp_call_result_n353, .hxc_waterUpdateBudget = 64 });
      struct hxc_caxecraft_domain_GameTickResult hxc_gameTick = hxc_tmp_instance_call_result_n356;
      hxc_character = hxc_gameTick.hxc_character;
      if (hxc_gameTick.hxc_committed)
      {
        struct hxc_caxecraft_app_MotionHistory hxc_tmp_load_result_n359 = hxc_motionHistory;
        struct hxc_caxecraft_app_MotionHistory hxc_tmp_call_result_n361 = hxc_caxecraft_app_MotionInterpolation_advance(hxc_tmp_load_result_n359, hxc_character.hxc_body);
        hxc_motionHistory = hxc_tmp_call_result_n361;
      }
      hxc_cameraWaterBlend = hxc_gameTick.hxc_immersion.hxc_cameraBlend;
      if (!hxc_gameTick.hxc_committed)
      {
        hxc_quit = true;
      }
      if (hxc_character.hxc_vitals.hxc_health > 0)
      {
        int32_t hxc_pickupIndex = 0;
        while (1)
        {
          if (!(hxc_pickupIndex < 1))
          {
            break;
          }
          struct hxc_caxecraft_domain_GameSession *hxc_tmp_owned_class_field_address_n366 = &(*hxc_self).hxc_session;
          if (hxc_tmp_owned_class_field_address_n366 == NULL)
          {
            abort();
          }
          bool hxc_tmp_instance_call_result_n368 = hxc_caxecraft_domain_GameSession_authoredItemIsActive(hxc_tmp_owned_class_field_address_n366, hxc_pickupIndex);
          bool hxc_tmp_short_circuit_result_n165 = hxc_tmp_instance_call_result_n368;
          if (hxc_tmp_instance_call_result_n368)
          {
            double hxc_tmp_record_field_load_result_n369 = hxc_character.hxc_body.hxc_x;
            double hxc_tmp_record_field_load_result_n370 = hxc_character.hxc_body.hxc_y;
            double hxc_tmp_record_field_load_result_n371 = hxc_character.hxc_body.hxc_z;
            int32_t hxc_tmp_call_result_n373 = hxc_caxecraft_content_FirstPlayableLevel_itemXMilli(hxc_pickupIndex);
            int32_t hxc_tmp_call_result_n375 = hxc_caxecraft_content_FirstPlayableLevel_itemYMilli(hxc_pickupIndex);
            int32_t hxc_tmp_call_result_n377 = hxc_caxecraft_content_FirstPlayableLevel_itemZMilli(hxc_pickupIndex);
            bool hxc_tmp_call_result_n378 = hxc_caxecraft_gameplay_WorldItemPickup_isInRange(hxc_tmp_record_field_load_result_n369, hxc_tmp_record_field_load_result_n370, hxc_tmp_record_field_load_result_n371, hxc_tmp_call_result_n373, hxc_tmp_call_result_n375, hxc_tmp_call_result_n377);
            hxc_tmp_short_circuit_result_n165 = hxc_tmp_call_result_n378;
          }
          if (hxc_tmp_short_circuit_result_n165)
          {
            int32_t hxc_tmp_call_result_n381 = hxc_caxecraft_content_FirstPlayableLevel_itemStorageCode(hxc_pickupIndex);
            int32_t hxc_itemCode = hxc_tmp_call_result_n381;
            int32_t hxc_tmp_call_result_n383 = hxc_caxecraft_content_BaseContentPack_itemFromValidatedStorageCode(hxc_itemCode);
            int32_t hxc_item = hxc_tmp_call_result_n383;
            int32_t hxc_tmp_call_result_n385 = hxc_caxecraft_content_BaseContentPack_itemUseProfile(hxc_item);
            bool hxc_tmp_short_circuit_result_n168 = hxc_tmp_call_result_n385 == 1;
            if (hxc_tmp_call_result_n385 == 1)
            {
              bool hxc_tmp_call_result_n387 = hxc_caxecraft_content_BaseContentPack_itemProvidesAquaticProfile(hxc_item);
              hxc_tmp_short_circuit_result_n168 = hxc_tmp_call_result_n387;
            }
            if (hxc_tmp_short_circuit_result_n168)
            {
              int32_t hxc_tmp_call_result_n390 = hxc_caxecraft_content_BaseContentPack_itemAquaticProfile(hxc_item);
              struct hxc_caxecraft_domain_AquaticProfile hxc_tmp_call_result_n391 = hxc_caxecraft_content_BaseContentPack_aquaticProfile(hxc_tmp_call_result_n390);
              struct hxc_caxecraft_domain_AquaticProfile hxc_replacement = hxc_tmp_call_result_n391;
              struct hxc_caxecraft_domain_GameSession *hxc_tmp_owned_class_field_address_n392 = &(*hxc_self).hxc_session;
              if (hxc_tmp_owned_class_field_address_n392 == NULL)
              {
                abort();
              }
              int32_t hxc_tmp_load_result_n393 = hxc_pickupIndex;
              struct hxc_caxecraft_domain_AuthoredAquaticEquipmentResult hxc_tmp_instance_call_result_n395 = hxc_caxecraft_domain_GameSession_collectAuthoredAquaticEquipment(hxc_tmp_owned_class_field_address_n392, hxc_tmp_load_result_n393, hxc_replacement);
              struct hxc_caxecraft_domain_AuthoredAquaticEquipmentResult hxc_equipment = hxc_tmp_instance_call_result_n395;
              hxc_character = hxc_equipment.hxc_character;
              if (!hxc_equipment.hxc_resolved)
              {
                hxc_quit = true;
              }
              else
              {
                if (hxc_equipment.hxc_collected)
                {
                  hxc_aquaticEquipmentCode = hxc_itemCode;
                  hxc_aquaticEquipmentFrames = 120;
                }
              }
            }
          }
          hxc_pickupIndex = hxc_i32_add_wrapping(hxc_pickupIndex, 1);
        }
      }
      if (hxc_selectedMode == 1)
      {
        if (hxc_character.hxc_vitals.hxc_health > 0)
        {
          struct hxc_caxecraft_domain_GameSession *hxc_tmp_owned_class_field_address_n403 = &(*hxc_self).hxc_session;
          if (hxc_tmp_owned_class_field_address_n403 == NULL)
          {
            abort();
          }
          int32_t hxc_tmp_record_field_load_result_n404 = hxc_gameTick.hxc_tickIndex;
          struct hxc_caxecraft_domain_ActorControllerTickResult hxc_tmp_instance_call_result_n406 = hxc_caxecraft_domain_GameSession_stepAuthoredActorControllers(hxc_tmp_owned_class_field_address_n403, hxc_tmp_record_field_load_result_n404, hxc_damagePolicy);
          struct hxc_caxecraft_domain_ActorControllerTickResult hxc_actorTick = hxc_tmp_instance_call_result_n406;
          switch (hxc_actorTick.hxc_status.hxc_tag) {
            case hxc_caxecraft_domain_ActorControllerTickStatus_ControllersAdvanced:
              {
                hxc_guideInteractionAvailable = false;
                int32_t hxc_g = 0;
                struct hxc_caxecraft_domain_GameSession *hxc_tmp_owned_class_field_address_n408 = &(*hxc_self).hxc_session;
                if (hxc_tmp_owned_class_field_address_n408 == NULL)
                {
                  abort();
                }
                struct hxc_array_ref *hxc_tmp_instance_call_result_n409 = hxc_caxecraft_domain_GameSession_actorControllerEventSnapshots(hxc_tmp_owned_class_field_address_n408);
                hxc_g1 = hxc_tmp_instance_call_result_n409;
                while (1)
                {
                  int32_t hxc_tmp_load_result_n410 = hxc_g;
                  int32_t hxc_tmp_array_length_result_n412;
                  if (hxc_array_ref_length(hxc_g1, &hxc_tmp_array_length_result_n412) != HXC_STATUS_OK)
                  {
                    abort();
                  }
                  if (!(hxc_tmp_load_result_n410 < hxc_tmp_array_length_result_n412))
                  {
                    break;
                  }
                  struct hxc_array_ref *hxc_tmp_load_result_n413 = hxc_g1;
                  struct hxc_caxecraft_domain_ActorControllerEvent hxc_tmp_array_get_result_n415;
                  if (hxc_array_ref_get_copy(hxc_tmp_load_result_n413, (size_t)hxc_g, &hxc_tmp_array_get_result_n415) != HXC_STATUS_OK)
                  {
                    abort();
                  }
                  hxc_tmp_array_element_owner_n175 = hxc_tmp_array_get_result_n415;
                  hxc_event = hxc_tmp_array_element_owner_n175;
                  if (hxc_enum_8b2a26b8_retain(&hxc_event) != HXC_STATUS_OK)
                  {
                    abort();
                  }
                  hxc_g = hxc_i32_add_wrapping(hxc_g, 1);
                  switch (hxc_event.hxc_tag) {
                    case hxc_caxecraft_domain_ActorControllerEvent_NoControllerEvent:
                      {
                        break;
                      }
                    case hxc_caxecraft_domain_ActorControllerEvent_InteractionAvailable:
                      {
                        if (hxc_event.hxc_tag != hxc_caxecraft_domain_ActorControllerEvent_InteractionAvailable)
                        {
                          abort();
                        }
                        int32_t hxc_tmp_enum_payload_project_n420 = hxc_event.hxc_payload.hxc_InteractionAvailable.hxc_characterId;
                        int32_t hxc_source_h838cfa102a39 = hxc_tmp_enum_payload_project_n420;
                        int32_t hxc_tmp_load_result_n421 = hxc_source_h838cfa102a39;
                        if (hxc_tmp_load_result_n421 == hxc_dialogueActorId)
                        {
                          hxc_guideInteractionAvailable = true;
                        }
                        break;
                      }
                    case hxc_caxecraft_domain_ActorControllerEvent_LocalPlayerAttack:
                      {
                        if (hxc_event.hxc_tag != hxc_caxecraft_domain_ActorControllerEvent_LocalPlayerAttack)
                        {
                          abort();
                        }
                        int32_t hxc_tmp_enum_payload_project_n424 = hxc_event.hxc_payload.hxc_LocalPlayerAttack.hxc_characterId;
                        int32_t hxc_source_h93677fe4f148 = hxc_tmp_enum_payload_project_n424;
                        int32_t hxc_tmp_load_result_n425 = hxc_source_h93677fe4f148;
                        if (hxc_tmp_load_result_n425 == hxc_enemyActorId)
                        {
                          hxc_enemyAttackFrames = 120;
                        }
                        break;
                      }
                    case hxc_caxecraft_domain_ActorControllerEvent_DropRequested:
                      {
                        if (hxc_event.hxc_tag != hxc_caxecraft_domain_ActorControllerEvent_DropRequested)
                        {
                          abort();
                        }
                        int32_t hxc_tmp_enum_payload_project_n428 = hxc_event.hxc_payload.hxc_DropRequested.hxc_characterId;
                        int32_t hxc_source_h1a989c3a569b = hxc_tmp_enum_payload_project_n428;
                        if (hxc_event.hxc_tag != hxc_caxecraft_domain_ActorControllerEvent_DropRequested)
                        {
                          abort();
                        }
                        hxc_string hxc_tmp_enum_payload_project_n430 = hxc_event.hxc_payload.hxc_DropRequested.hxc_drop;
                        hxc_drop = hxc_tmp_enum_payload_project_n430;
                        if (hxc_string_retain(hxc_drop) != HXC_STATUS_OK)
                        {
                          abort();
                        }
                        int32_t hxc_tmp_call_result_n432 = hxc_caxecraft_content_BaseContentPack_dropQuantityById(hxc_drop);
                        int32_t hxc_quantity = hxc_tmp_call_result_n432;
                        struct hxc_caxecraft_domain_GameSession *hxc_tmp_owned_class_field_address_n433 = &(*hxc_self).hxc_session;
                        if (hxc_tmp_owned_class_field_address_n433 == NULL)
                        {
                          abort();
                        }
                        struct hxc_caxecraft_domain_Character hxc_tmp_instance_call_result_n435 = hxc_caxecraft_domain_GameSession_readCharacter(hxc_tmp_owned_class_field_address_n433, hxc_source_h1a989c3a569b);
                        struct hxc_caxecraft_domain_Character hxc_defeatedActor = hxc_tmp_instance_call_result_n435;
                        int32_t hxc_tmp_load_result_n436 = hxc_quantity;
                        bool hxc_tmp_short_circuit_result_n182 = hxc_tmp_load_result_n436 <= 0;
                        if (!(hxc_tmp_load_result_n436 <= 0))
                        {
                          hxc_tmp_short_circuit_result_n182 = hxc_defeatedActor.hxc_id <= 0;
                        }
                        if (hxc_tmp_short_circuit_result_n182)
                        {
                          hxc_quit = true;
                        }
                        else
                        {
                          if (!hxc_berryDrop.hxc_active)
                          {
                            struct hxc_caxecraft_domain_Character hxc_tmp_load_result_n440 = hxc_defeatedActor;
                            struct hxc_caxecraft_gameplay_BerryDropState hxc_tmp_call_result_n442 = hxc_caxecraft_gameplay_BerryDrop_fromDefeatedCharacter(hxc_tmp_load_result_n440, hxc_quantity);
                            hxc_berryDrop = hxc_tmp_call_result_n442;
                            hxc_enemyDefeatedFrames = 120;
                          }
                        }
                        if (hxc_string_release(&hxc_drop) != HXC_STATUS_OK)
                        {
                          abort();
                        }
                        break;
                      }
                  }
                  hxc_enum_8b2a26b8_destroy(&hxc_event);
                  hxc_array_43e9f8ca_element_destroy(NULL, &hxc_tmp_array_element_owner_n175);
                }
                if (hxc_array_ref_release(hxc_g1) != HXC_STATUS_OK)
                {
                  abort();
                }
                break;
              }
            case hxc_caxecraft_domain_ActorControllerTickStatus_ControllerModelRejected:
              {
                hxc_quit = true;
                break;
              }
            case hxc_caxecraft_domain_ActorControllerTickStatus_ControlledCharacterMissing:
              {
                hxc_quit = true;
                break;
              }
            case hxc_caxecraft_domain_ActorControllerTickStatus_CharacterCommandRejected:
              {
                hxc_quit = true;
                break;
              }
            case hxc_caxecraft_domain_ActorControllerTickStatus_LocalAttackCommandRejected:
              {
                hxc_quit = true;
                break;
              }
          }
        }
        struct hxc_caxecraft_domain_GameSession *hxc_tmp_owned_class_field_address_n443 = &(*hxc_self).hxc_session;
        if (hxc_tmp_owned_class_field_address_n443 == NULL)
        {
          abort();
        }
        struct hxc_array_ref *hxc_tmp_instance_call_result_n444 = hxc_caxecraft_domain_GameSession_actorControllerStateSnapshots(hxc_tmp_owned_class_field_address_n443);
        hxc_actorPhases = hxc_tmp_instance_call_result_n444;
        struct hxc_array_ref *hxc_tmp_load_result_n445 = hxc_actorPhases;
        struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation hxc_tmp_call_result_n448 = hxc_caxecraft_app_CaxecraftApp_observeActorPhase(hxc_tmp_load_result_n445, hxc_dialogueActorId, hxc_caxecraft_domain_ActorControllerPhase_Stationary);
        hxc_dialoguePhase = hxc_tmp_call_result_n448;
        struct hxc_array_ref *hxc_tmp_load_result_n449 = hxc_actorPhases;
        struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation hxc_tmp_call_result_n452 = hxc_caxecraft_app_CaxecraftApp_observeActorPhase(hxc_tmp_load_result_n449, hxc_enemyActorId, hxc_caxecraft_domain_ActorControllerPhase_Defeated);
        hxc_enemyPhase = hxc_tmp_call_result_n452;
        struct hxc_caxecraft_domain_GameSession *hxc_tmp_owned_class_field_address_n453 = &(*hxc_self).hxc_session;
        if (hxc_tmp_owned_class_field_address_n453 == NULL)
        {
          abort();
        }
        struct hxc_caxecraft_domain_Character hxc_tmp_instance_call_result_n455 = hxc_caxecraft_domain_GameSession_readCharacter(hxc_tmp_owned_class_field_address_n453, hxc_dialogueActorId);
        hxc_dialogueActor = hxc_tmp_instance_call_result_n455;
        struct hxc_caxecraft_domain_GameSession *hxc_tmp_owned_class_field_address_n456 = &(*hxc_self).hxc_session;
        if (hxc_tmp_owned_class_field_address_n456 == NULL)
        {
          abort();
        }
        struct hxc_caxecraft_domain_Character hxc_tmp_instance_call_result_n458 = hxc_caxecraft_domain_GameSession_readCharacter(hxc_tmp_owned_class_field_address_n456, hxc_enemyActorId);
        hxc_enemyActor = hxc_tmp_instance_call_result_n458;
        bool hxc_tmp_record_field_load_result_n459 = hxc_dialoguePhase.hxc_valid;
        bool hxc_tmp_short_circuit_result_n184 = !hxc_tmp_record_field_load_result_n459;
        if (!!hxc_tmp_record_field_load_result_n459)
        {
          hxc_tmp_short_circuit_result_n184 = !hxc_enemyPhase.hxc_valid;
        }
        bool hxc_tmp_short_circuit_load_result_n461 = hxc_tmp_short_circuit_result_n184;
        bool hxc_tmp_short_circuit_result_n185 = hxc_tmp_short_circuit_load_result_n461;
        if (!hxc_tmp_short_circuit_load_result_n461)
        {
          hxc_tmp_short_circuit_result_n185 = hxc_dialogueActor.hxc_id <= 0;
        }
        bool hxc_tmp_short_circuit_load_result_n463 = hxc_tmp_short_circuit_result_n185;
        bool hxc_tmp_short_circuit_result_n186 = hxc_tmp_short_circuit_load_result_n463;
        if (!hxc_tmp_short_circuit_load_result_n463)
        {
          hxc_tmp_short_circuit_result_n186 = hxc_enemyActor.hxc_id <= 0;
        }
        if (hxc_tmp_short_circuit_result_n186)
        {
          hxc_quit = true;
        }
        struct hxc_caxecraft_gameplay_SwordCombatState hxc_tmp_call_result_n467 = hxc_caxecraft_gameplay_SwordCombat_step(hxc_swordCombat);
        hxc_swordCombat = hxc_tmp_call_result_n467;
        if (hxc_swordQueued)
        {
          struct hxc_caxecraft_gameplay_SwordCombatState hxc_tmp_load_result_n469 = hxc_swordCombat;
          struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_load_result_n470 = hxc_inventory;
          struct hxc_caxecraft_domain_VitalsState hxc_tmp_record_field_load_result_n471 = hxc_character.hxc_vitals;
          struct hxc_caxecraft_domain_Character hxc_tmp_load_result_n472 = hxc_enemyActor;
          double hxc_tmp_record_field_load_result_n473 = hxc_character.hxc_body.hxc_x;
          double hxc_tmp_record_field_load_result_n474 = hxc_character.hxc_body.hxc_z;
          double hxc_tmp_load_result_n475 = hxc_lookX;
          int32_t hxc_tmp_call_result_n477 = hxc_caxecraft_gameplay_SwordCombat_decide(hxc_tmp_load_result_n469, hxc_tmp_load_result_n470, hxc_tmp_record_field_load_result_n471, hxc_tmp_load_result_n472, hxc_tmp_record_field_load_result_n473, hxc_tmp_record_field_load_result_n474, hxc_tmp_load_result_n475, hxc_lookZ);
          int32_t hxc_swordDecision = hxc_tmp_call_result_n477;
          if (hxc_swordDecision == 5)
          {
            struct hxc_caxecraft_domain_GameSession *hxc_tmp_owned_class_field_address_n479 = &(*hxc_self).hxc_session;
            if (hxc_tmp_owned_class_field_address_n479 == NULL)
            {
              abort();
            }
            struct hxc_caxecraft_domain_CharacterDamageResult hxc_tmp_instance_call_result_n481 = hxc_caxecraft_domain_GameSession_damageCharacter(hxc_tmp_owned_class_field_address_n479, hxc_enemyActorId, 1);
            struct hxc_caxecraft_domain_CharacterDamageResult hxc_damage = hxc_tmp_instance_call_result_n481;
            if (!hxc_damage.hxc_resolved)
            {
              hxc_quit = true;
            }
            else
            {
              if (hxc_damage.hxc_damageApplied > 0)
              {
                hxc_enemyActor = hxc_damage.hxc_character;
                hxc_strikeHitFrames = 16;
              }
            }
          }
          int32_t hxc_tmp_load_result_n485 = hxc_swordDecision;
          struct hxc_caxecraft_gameplay_SwordCombatState hxc_tmp_call_result_n487 = hxc_caxecraft_gameplay_SwordCombat_after(hxc_tmp_load_result_n485, hxc_swordCombat);
          hxc_swordCombat = hxc_tmp_call_result_n487;
          hxc_swordQueued = false;
        }
        if (hxc_array_ref_release(hxc_actorPhases) != HXC_STATUS_OK)
        {
          abort();
        }
      }
      hxc_jumpQueued = false;
      hxc_accumulator = hxc_accumulator - 0.05;
    }
    double hxc_selectionEyeX = hxc_character.hxc_body.hxc_x;
    double hxc_selectionEyeY = hxc_character.hxc_body.hxc_y + 1.62;
    double hxc_selectionEyeZ = hxc_character.hxc_body.hxc_z;
    struct hxc_caxecraft_domain_GameSession *hxc_tmp_owned_class_field_address_n492 = &(*hxc_self).hxc_session;
    if (hxc_tmp_owned_class_field_address_n492 == NULL)
    {
      abort();
    }
    size_t hxc_tmp_length_n494;
    const uint8_t *hxc_tmp_instance_call_result_n493 = hxc_caxecraft_domain_GameSession_worldView(hxc_tmp_owned_class_field_address_n492, &hxc_tmp_length_n494);
    double hxc_tmp_load_result_n495 = hxc_selectionEyeX;
    double hxc_tmp_load_result_n496 = hxc_selectionEyeY;
    double hxc_tmp_load_result_n497 = hxc_selectionEyeZ;
    double hxc_tmp_load_result_n498 = hxc_lookX;
    double hxc_tmp_load_result_n499 = hxc_lookY;
    struct hxc_caxecraft_domain_RaycastHit hxc_tmp_call_result_n501 = hxc_caxecraft_domain_VoxelRaycast_trace(hxc_tmp_instance_call_result_n493, hxc_tmp_length_n494, hxc_tmp_load_result_n495, hxc_tmp_load_result_n496, hxc_tmp_load_result_n497, hxc_tmp_load_result_n498, hxc_tmp_load_result_n499, hxc_lookZ, 7.0);
    struct hxc_caxecraft_domain_RaycastHit hxc_hit = hxc_tmp_call_result_n501;
    bool hxc_tmp_load_result_n502 = hxc_captured_h94383143b373;
    bool hxc_tmp_short_circuit_result_n193 = hxc_tmp_load_result_n502;
    if (hxc_tmp_load_result_n502)
    {
      hxc_tmp_short_circuit_result_n193 = !hxc_recapturedThisFrame;
    }
    bool hxc_tmp_short_circuit_load_result_n504 = hxc_tmp_short_circuit_result_n193;
    bool hxc_tmp_short_circuit_result_n194 = hxc_tmp_short_circuit_load_result_n504;
    if (hxc_tmp_short_circuit_load_result_n504)
    {
      hxc_tmp_short_circuit_result_n194 = hxc_primaryPressed_haad28c32f864;
    }
    if (hxc_tmp_short_circuit_result_n194)
    {
      if (hxc_character.hxc_vitals.hxc_health > 0)
      {
        if (hxc_selectedMode == 1)
        {
          bool hxc_tmp_call_result_n511 = hxc_caxecraft_gameplay_Inventory_selectedIs(hxc_inventory, hxc_caxecraft_gameplay_ItemKind_CopperSword);
          bool hxc_tmp_short_circuit_result_n195 = !hxc_tmp_call_result_n511;
          if (!hxc_tmp_call_result_n511)
          {
            hxc_tmp_short_circuit_result_n195 = hxc_hit.hxc_hit;
          }
          bool hxc_tmp_short_circuit_load_result_n513 = hxc_tmp_short_circuit_result_n195;
          bool hxc_tmp_short_circuit_result_n196 = hxc_tmp_short_circuit_load_result_n513;
          if (hxc_tmp_short_circuit_load_result_n513)
          {
            struct hxc_caxecraft_domain_AquaticState hxc_tmp_record_field_load_result_n514 = hxc_character.hxc_aquatic;
            bool hxc_tmp_call_result_n516 = hxc_caxecraft_domain_Aquatics_canMine(hxc_tmp_record_field_load_result_n514, hxc_character.hxc_aquaticProfile);
            hxc_tmp_short_circuit_result_n196 = hxc_tmp_call_result_n516;
          }
          if (hxc_tmp_short_circuit_result_n196)
          {
            int32_t hxc_tmp_record_field_load_result_n518 = hxc_hit.hxc_cellX;
            int32_t hxc_tmp_record_field_load_result_n519 = hxc_hit.hxc_cellY;
            struct hxc_caxecraft_scenario_VoxelPoint hxc_tmp_call_result_n521 = hxc_caxecraft_domain_World_coord(hxc_tmp_record_field_load_result_n518, hxc_tmp_record_field_load_result_n519, hxc_hit.hxc_cellZ);
            struct hxc_caxecraft_scenario_VoxelPoint hxc_minedCoordinate = hxc_tmp_call_result_n521;
            struct hxc_caxecraft_domain_GameSession *hxc_tmp_owned_class_field_address_n522 = &(*hxc_self).hxc_session;
            if (hxc_tmp_owned_class_field_address_n522 == NULL)
            {
              abort();
            }
            struct hxc_caxecraft_scenario_VoxelPoint hxc_tmp_load_result_n523 = hxc_minedCoordinate;
            struct hxc_caxecraft_gameplay_MiningResult hxc_tmp_instance_call_result_n525 = hxc_caxecraft_domain_GameSession_mineTerrain(hxc_tmp_owned_class_field_address_n522, hxc_tmp_load_result_n523, hxc_inventory);
            struct hxc_caxecraft_gameplay_MiningResult hxc_mining = hxc_tmp_instance_call_result_n525;
            hxc_inventory = hxc_mining.hxc_inventory;
            if (hxc_mining.hxc_outcome == 2)
            {
              struct hxc_caxecraft_app_TerrainRenderer *hxc_tmp_owned_class_field_address_n528 = &(*hxc_self).hxc_terrainRenderer;
              struct hxc_caxecraft_app_TerrainChunkCache *hxc_tmp_owned_class_field_address_n529 = &(*hxc_tmp_owned_class_field_address_n528).hxc_cache;
              if (hxc_tmp_owned_class_field_address_n529 == NULL)
              {
                abort();
              }
              hxc_caxecraft_app_TerrainChunkCache_invalidate(hxc_tmp_owned_class_field_address_n529, hxc_minedCoordinate);
            }
            if (hxc_mining.hxc_outcome == 1)
            {
              hxc_inventoryFullReason = 2;
              hxc_inventoryFullFrames = 90;
            }
          }
        }
        else
        {
          if (hxc_hit.hxc_hit)
          {
            int32_t hxc_tmp_record_field_load_result_n533 = hxc_hit.hxc_cellX;
            int32_t hxc_tmp_record_field_load_result_n534 = hxc_hit.hxc_cellY;
            struct hxc_caxecraft_scenario_VoxelPoint hxc_tmp_call_result_n536 = hxc_caxecraft_domain_World_coord(hxc_tmp_record_field_load_result_n533, hxc_tmp_record_field_load_result_n534, hxc_hit.hxc_cellZ);
            struct hxc_caxecraft_scenario_VoxelPoint hxc_removedCoordinate = hxc_tmp_call_result_n536;
            struct hxc_caxecraft_domain_GameSession *hxc_tmp_owned_class_field_address_n537 = &(*hxc_self).hxc_session;
            if (hxc_tmp_owned_class_field_address_n537 == NULL)
            {
              abort();
            }
            bool hxc_tmp_instance_call_result_n539 = hxc_caxecraft_domain_GameSession_removeTerrain(hxc_tmp_owned_class_field_address_n537, hxc_removedCoordinate);
            bool hxc_removed = hxc_tmp_instance_call_result_n539;
            if (hxc_removed)
            {
              struct hxc_caxecraft_app_TerrainRenderer *hxc_tmp_owned_class_field_address_n541 = &(*hxc_self).hxc_terrainRenderer;
              struct hxc_caxecraft_app_TerrainChunkCache *hxc_tmp_owned_class_field_address_n542 = &(*hxc_tmp_owned_class_field_address_n541).hxc_cache;
              if (hxc_tmp_owned_class_field_address_n542 == NULL)
              {
                abort();
              }
              hxc_caxecraft_app_TerrainChunkCache_invalidate(hxc_tmp_owned_class_field_address_n542, hxc_removedCoordinate);
            }
          }
        }
      }
    }
    bool hxc_tmp_load_result_n544 = hxc_captured_h94383143b373;
    bool hxc_tmp_short_circuit_result_n201 = hxc_tmp_load_result_n544;
    if (hxc_tmp_load_result_n544)
    {
      hxc_tmp_short_circuit_result_n201 = hxc_secondaryPressed_h010b204dcf5c;
    }
    if (hxc_tmp_short_circuit_result_n201)
    {
      if (hxc_character.hxc_vitals.hxc_health > 0)
      {
        struct hxc_caxecraft_domain_GameSession *hxc_tmp_owned_class_field_address_n548 = &(*hxc_self).hxc_session;
        if (hxc_tmp_owned_class_field_address_n548 == NULL)
        {
          abort();
        }
        struct hxc_caxecraft_domain_LocalRecoveryResult hxc_tmp_instance_call_result_n550 = hxc_caxecraft_domain_GameSession_useSelectedRecovery(hxc_tmp_owned_class_field_address_n548, hxc_inventory);
        struct hxc_caxecraft_domain_LocalRecoveryResult hxc_recovery = hxc_tmp_instance_call_result_n550;
        hxc_character = hxc_recovery.hxc_character;
        if (!hxc_recovery.hxc_resolved)
        {
          hxc_quit = true;
        }
        else
        {
          if (hxc_recovery.hxc_decision != 0)
          {
            hxc_recoveryFeedback = hxc_recovery.hxc_decision;
            hxc_recoveryFeedbackFrames = 90;
            hxc_inventory = hxc_recovery.hxc_inventory;
          }
          else
          {
            if (hxc_hit.hxc_hit)
            {
              int32_t hxc_tmp_record_field_load_result_n557 = hxc_hit.hxc_previousX;
              int32_t hxc_tmp_record_field_load_result_n558 = hxc_hit.hxc_previousY;
              struct hxc_caxecraft_scenario_VoxelPoint hxc_tmp_call_result_n560 = hxc_caxecraft_domain_World_coord(hxc_tmp_record_field_load_result_n557, hxc_tmp_record_field_load_result_n558, hxc_hit.hxc_previousZ);
              struct hxc_caxecraft_scenario_VoxelPoint hxc_placement = hxc_tmp_call_result_n560;
              enum hxc_caxecraft_domain_BlockKind hxc_tmp_call_result_n562 = hxc_caxecraft_gameplay_Inventory_selectedBlock(hxc_inventory);
              enum hxc_caxecraft_domain_BlockKind hxc_selectedBlock = hxc_tmp_call_result_n562;
              struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_load_result_n563 = hxc_inventory;
              int32_t hxc_tmp_call_result_n565 = hxc_caxecraft_gameplay_Inventory_countAt(hxc_tmp_load_result_n563, hxc_inventory.hxc_selected);
              bool hxc_hasItem = hxc_tmp_call_result_n565 > 0;
              bool hxc_tmp_load_result_n566 = hxc_hasItem;
              bool hxc_tmp_short_circuit_result_n206 = !hxc_tmp_load_result_n566;
              if (!!hxc_tmp_load_result_n566)
              {
                bool hxc_tmp_call_result_n568 = hxc_caxecraft_domain_World_isPlaceable(hxc_selectedBlock);
                hxc_tmp_short_circuit_result_n206 = !hxc_tmp_call_result_n568;
              }
              bool hxc_tmp_short_circuit_load_result_n569 = hxc_tmp_short_circuit_result_n206;
              bool hxc_tmp_short_circuit_result_n207 = hxc_tmp_short_circuit_load_result_n569;
              if (!hxc_tmp_short_circuit_load_result_n569)
              {
                struct hxc_caxecraft_domain_CharacterBody hxc_tmp_record_field_load_result_n570 = hxc_character.hxc_body;
                bool hxc_tmp_call_result_n572 = hxc_caxecraft_domain_CharacterPhysics_canPlaceAt(hxc_tmp_record_field_load_result_n570, hxc_placement);
                hxc_tmp_short_circuit_result_n207 = !hxc_tmp_call_result_n572;
              }
              bool hxc_tmp_short_circuit_load_result_n573 = hxc_tmp_short_circuit_result_n207;
              bool hxc_tmp_short_circuit_result_n208 = hxc_tmp_short_circuit_load_result_n573;
              if (!hxc_tmp_short_circuit_load_result_n573)
              {
                struct hxc_caxecraft_domain_GameSession *hxc_tmp_owned_class_field_address_n574 = &(*hxc_self).hxc_session;
                if (hxc_tmp_owned_class_field_address_n574 == NULL)
                {
                  abort();
                }
                struct hxc_caxecraft_scenario_VoxelPoint hxc_tmp_load_result_n575 = hxc_placement;
                bool hxc_tmp_instance_call_result_n577 = hxc_caxecraft_domain_GameSession_placeTerrain(hxc_tmp_owned_class_field_address_n574, hxc_tmp_load_result_n575, hxc_selectedBlock);
                hxc_tmp_short_circuit_result_n208 = !hxc_tmp_instance_call_result_n577;
              }
              if (hxc_tmp_short_circuit_result_n208)
              {
                hxc_placementBlockedFrames = 60;
              }
              else
              {
                struct hxc_caxecraft_app_TerrainRenderer *hxc_tmp_owned_class_field_address_n579 = &(*hxc_self).hxc_terrainRenderer;
                struct hxc_caxecraft_app_TerrainChunkCache *hxc_tmp_owned_class_field_address_n580 = &(*hxc_tmp_owned_class_field_address_n579).hxc_cache;
                if (hxc_tmp_owned_class_field_address_n580 == NULL)
                {
                  abort();
                }
                hxc_caxecraft_app_TerrainChunkCache_invalidate(hxc_tmp_owned_class_field_address_n580, hxc_placement);
                if (hxc_selectedMode == 1)
                {
                  struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_call_result_n584 = hxc_caxecraft_gameplay_Inventory_consumeSelected(hxc_inventory);
                  hxc_inventory = hxc_tmp_call_result_n584;
                }
              }
            }
          }
        }
      }
    }
    if (hxc_placementBlockedFrames > 0)
    {
      hxc_placementBlockedFrames = hxc_i32_subtract_wrapping(hxc_placementBlockedFrames, 1);
    }
    if (!hxc_paused_h86863acb5f2c)
    {
      struct hxc_caxecraft_gameplay_BerryDropState hxc_tmp_load_result_n588 = hxc_berryDrop;
      double hxc_tmp_record_field_load_result_n589 = hxc_character.hxc_body.hxc_x;
      double hxc_tmp_record_field_load_result_n590 = hxc_character.hxc_body.hxc_y;
      bool hxc_tmp_call_result_n592 = hxc_caxecraft_gameplay_BerryDrop_isInRange(hxc_tmp_load_result_n588, hxc_tmp_record_field_load_result_n589, hxc_tmp_record_field_load_result_n590, hxc_character.hxc_body.hxc_z);
      if (hxc_tmp_call_result_n592)
      {
        struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_load_result_n593 = hxc_inventory;
        int32_t hxc_tmp_call_result_n596 = hxc_caxecraft_gameplay_Inventory_acceptedAmount(hxc_tmp_load_result_n593, hxc_caxecraft_gameplay_ItemKind_Berries, hxc_berryDrop.hxc_amount);
        int32_t hxc_acceptedDrop = hxc_tmp_call_result_n596;
        if (hxc_acceptedDrop > 0)
        {
          struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_load_result_n598 = hxc_inventory;
          struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_call_result_n601 = hxc_caxecraft_gameplay_Inventory_collectItem(hxc_tmp_load_result_n598, hxc_caxecraft_gameplay_ItemKind_Berries, hxc_acceptedDrop);
          hxc_inventory = hxc_tmp_call_result_n601;
          struct hxc_caxecraft_gameplay_BerryDropState hxc_tmp_load_result_n602 = hxc_berryDrop;
          struct hxc_caxecraft_gameplay_BerryDropState hxc_tmp_call_result_n604 = hxc_caxecraft_gameplay_BerryDrop_collectAmount(hxc_tmp_load_result_n602, hxc_acceptedDrop);
          hxc_berryDrop = hxc_tmp_call_result_n604;
          hxc_pickupAmount = hxc_acceptedDrop;
          hxc_pickupFrames = 90;
        }
        else
        {
          hxc_inventoryFullReason = 1;
          hxc_inventoryFullFrames = 90;
        }
      }
    }
    if (hxc_strikeHitFrames > 0)
    {
      hxc_strikeHitFrames = hxc_i32_subtract_wrapping(hxc_strikeHitFrames, 1);
    }
    if (hxc_enemyDefeatedFrames > 0)
    {
      hxc_enemyDefeatedFrames = hxc_i32_subtract_wrapping(hxc_enemyDefeatedFrames, 1);
    }
    if (hxc_enemyAttackFrames > 0)
    {
      hxc_enemyAttackFrames = hxc_i32_subtract_wrapping(hxc_enemyAttackFrames, 1);
    }
    if (hxc_pickupFrames > 0)
    {
      hxc_pickupFrames = hxc_i32_subtract_wrapping(hxc_pickupFrames, 1);
    }
    if (hxc_inventoryFullFrames > 0)
    {
      hxc_inventoryFullFrames = hxc_i32_subtract_wrapping(hxc_inventoryFullFrames, 1);
      if (hxc_inventoryFullFrames == 0)
      {
        hxc_inventoryFullReason = 0;
      }
    }
    if (hxc_recoveryFeedbackFrames > 0)
    {
      hxc_recoveryFeedbackFrames = hxc_i32_subtract_wrapping(hxc_recoveryFeedbackFrames, 1);
    }
    if (hxc_aquaticEquipmentFrames > 0)
    {
      hxc_aquaticEquipmentFrames = hxc_i32_subtract_wrapping(hxc_aquaticEquipmentFrames, 1);
    }
    struct hxc_caxecraft_domain_GameSession *hxc_tmp_owned_class_field_address_n621 = &(*hxc_self).hxc_session;
    if (hxc_tmp_owned_class_field_address_n621 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_domain_GameView hxc_tmp_instance_call_result_n622 = hxc_caxecraft_domain_GameSession_view(hxc_tmp_owned_class_field_address_n621);
    struct hxc_caxecraft_domain_GameView hxc_committedView = hxc_tmp_instance_call_result_n622;
    if (!hxc_committedView.hxc_valid)
    {
      hxc_quit = true;
    }
    else
    {
      hxc_character = hxc_committedView.hxc_localPlayer;
    }
    struct hxc_caxecraft_domain_GameSession *hxc_tmp_owned_class_field_address_n625 = &(*hxc_self).hxc_session;
    if (hxc_tmp_owned_class_field_address_n625 == NULL)
    {
      abort();
    }
    bool hxc_tmp_instance_call_result_n627 = hxc_caxecraft_domain_GameSession_actorInteractionAvailable(hxc_tmp_owned_class_field_address_n625, hxc_dialogueActorId);
    hxc_guideInteractionAvailable = hxc_tmp_instance_call_result_n627;
    struct hxc_caxecraft_domain_GameSession *hxc_tmp_owned_class_field_address_n628 = &(*hxc_self).hxc_session;
    if (hxc_tmp_owned_class_field_address_n628 == NULL)
    {
      abort();
    }
    struct hxc_array_ref *hxc_tmp_instance_call_result_n629 = hxc_caxecraft_domain_GameSession_actorControllerStateSnapshots(hxc_tmp_owned_class_field_address_n628);
    hxc_presentationActorPhases = hxc_tmp_instance_call_result_n629;
    struct hxc_array_ref *hxc_tmp_load_result_n630 = hxc_presentationActorPhases;
    struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation hxc_tmp_call_result_n633 = hxc_caxecraft_app_CaxecraftApp_observeActorPhase(hxc_tmp_load_result_n630, hxc_dialogueActorId, hxc_caxecraft_domain_ActorControllerPhase_Stationary);
    hxc_dialoguePhase = hxc_tmp_call_result_n633;
    struct hxc_array_ref *hxc_tmp_load_result_n634 = hxc_presentationActorPhases;
    struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation hxc_tmp_call_result_n637 = hxc_caxecraft_app_CaxecraftApp_observeActorPhase(hxc_tmp_load_result_n634, hxc_enemyActorId, hxc_caxecraft_domain_ActorControllerPhase_Defeated);
    hxc_enemyPhase = hxc_tmp_call_result_n637;
    struct hxc_caxecraft_domain_GameSession *hxc_tmp_owned_class_field_address_n638 = &(*hxc_self).hxc_session;
    if (hxc_tmp_owned_class_field_address_n638 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_domain_Character hxc_tmp_instance_call_result_n640 = hxc_caxecraft_domain_GameSession_readCharacter(hxc_tmp_owned_class_field_address_n638, hxc_dialogueActorId);
    hxc_dialogueActor = hxc_tmp_instance_call_result_n640;
    struct hxc_caxecraft_domain_GameSession *hxc_tmp_owned_class_field_address_n641 = &(*hxc_self).hxc_session;
    if (hxc_tmp_owned_class_field_address_n641 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_domain_Character hxc_tmp_instance_call_result_n643 = hxc_caxecraft_domain_GameSession_readCharacter(hxc_tmp_owned_class_field_address_n641, hxc_enemyActorId);
    hxc_enemyActor = hxc_tmp_instance_call_result_n643;
    bool hxc_tmp_record_field_load_result_n644 = hxc_dialoguePhase.hxc_valid;
    bool hxc_tmp_short_circuit_result_n212 = !hxc_tmp_record_field_load_result_n644;
    if (!!hxc_tmp_record_field_load_result_n644)
    {
      hxc_tmp_short_circuit_result_n212 = !hxc_enemyPhase.hxc_valid;
    }
    bool hxc_tmp_short_circuit_load_result_n646 = hxc_tmp_short_circuit_result_n212;
    bool hxc_tmp_short_circuit_result_n213 = hxc_tmp_short_circuit_load_result_n646;
    if (!hxc_tmp_short_circuit_load_result_n646)
    {
      hxc_tmp_short_circuit_result_n213 = hxc_dialogueActor.hxc_id <= 0;
    }
    bool hxc_tmp_short_circuit_load_result_n648 = hxc_tmp_short_circuit_result_n213;
    bool hxc_tmp_short_circuit_result_n214 = hxc_tmp_short_circuit_load_result_n648;
    if (!hxc_tmp_short_circuit_load_result_n648)
    {
      hxc_tmp_short_circuit_result_n214 = hxc_enemyActor.hxc_id <= 0;
    }
    if (hxc_tmp_short_circuit_result_n214)
    {
      hxc_quit = true;
    }
    int32_t hxc_completedTicks = hxc_committedView.hxc_completedTicks;
    if (hxc_resetMotionThisFrame)
    {
      struct hxc_caxecraft_app_MotionHistory hxc_tmp_call_result_n654 = hxc_caxecraft_app_MotionInterpolation_start(hxc_character.hxc_body);
      hxc_motionHistory = hxc_tmp_call_result_n654;
    }
    struct hxc_caxecraft_app_MotionHistory hxc_tmp_load_result_n655 = hxc_motionHistory;
    struct hxc_caxecraft_app_RenderPosition hxc_tmp_call_result_n657 = hxc_caxecraft_app_MotionInterpolation_sample(hxc_tmp_load_result_n655, hxc_accumulator, 0.05);
    struct hxc_caxecraft_app_RenderPosition hxc_renderPosition = hxc_tmp_call_result_n657;
    double hxc_eyeX = hxc_renderPosition.hxc_x;
    double hxc_eyeY = hxc_renderPosition.hxc_y + 1.62;
    double hxc_eyeZ = hxc_renderPosition.hxc_z;
    double hxc_tmp_load_result_n661 = hxc_eyeX;
    double hxc_tmp_load_result_n662 = hxc_eyeY;
    double hxc_tmp_load_result_n663 = hxc_eyeZ;
    double hxc_tmp_load_result_n665 = hxc_eyeX;
    double hxc_tmp_load_result_n666 = hxc_lookX;
    double hxc_tmp_load_result_n667 = hxc_eyeY;
    double hxc_tmp_load_result_n668 = hxc_lookY;
    double hxc_tmp_load_result_n669 = hxc_eyeZ;
    struct Camera3D hxc_camera = (struct Camera3D){ .position = (struct Vector3){ .x = (float)hxc_tmp_load_result_n661, .y = (float)hxc_tmp_load_result_n662, .z = (float)hxc_tmp_load_result_n663 }, .target = (struct Vector3){ .x = (float)(hxc_tmp_load_result_n665 + hxc_tmp_load_result_n666), .y = (float)(hxc_tmp_load_result_n667 + hxc_tmp_load_result_n668), .z = (float)(hxc_tmp_load_result_n669 + hxc_lookZ) }, .up = (struct Vector3){ .x = (float)0.0, .y = (float)1.0, .z = (float)0.0 }, .fovy = (float)70.0, .projection = 0 };
    BeginDrawing();
    if (hxc_onTitle)
    {
      int32_t hxc_tmp_native_call_result_n675 = GetScreenWidth();
      int32_t hxc_width_h504cec6a2eb4 = (int32_t)hxc_tmp_native_call_result_n675;
      int32_t hxc_tmp_native_call_result_n676 = GetScreenHeight();
      int32_t hxc_height_h197302da9393 = (int32_t)hxc_tmp_native_call_result_n676;
      if (hxc_titleTextureReady)
      {
        struct Color hxc_this1_hf4ee83235bf5 = (struct Color){ .r = 255, .g = 255, .b = 255, .a = 255 };
        struct Texture hxc_tmp_load_result_n679 = hxc_titleTexture;
        double hxc_imageAspect = hxc_f64_divide_zero_safe((double)(int32_t)hxc_tmp_load_result_n679.width, (double)(int32_t)hxc_titleTexture.height);
        int32_t hxc_tmp_load_result_n683 = hxc_width_h504cec6a2eb4;
        double hxc_screenAspect = hxc_f64_divide_zero_safe((double)hxc_tmp_load_result_n683, (double)hxc_height_h197302da9393);
        double hxc_sourceX = 0.0;
        double hxc_sourceY = 0.0;
        double hxc_sourceWidth = (double)(int32_t)hxc_titleTexture.width;
        double hxc_sourceHeight = (double)(int32_t)hxc_titleTexture.height;
        double hxc_tmp_load_result_n689 = hxc_screenAspect;
        if (hxc_tmp_load_result_n689 > hxc_imageAspect)
        {
          struct Texture hxc_tmp_load_result_n691 = hxc_titleTexture;
          hxc_sourceHeight = hxc_f64_divide_zero_safe((double)(int32_t)hxc_tmp_load_result_n691.width, hxc_screenAspect);
          struct Texture hxc_tmp_load_result_n694 = hxc_titleTexture;
          hxc_sourceY = ((double)(int32_t)hxc_tmp_load_result_n694.height - hxc_sourceHeight) * 0.5;
        }
        else
        {
          struct Texture hxc_tmp_load_result_n697 = hxc_titleTexture;
          hxc_sourceWidth = (double)(int32_t)hxc_tmp_load_result_n697.height * hxc_screenAspect;
          struct Texture hxc_tmp_load_result_n700 = hxc_titleTexture;
          hxc_sourceX = ((double)(int32_t)hxc_tmp_load_result_n700.width - hxc_sourceWidth) * 0.5;
        }
        struct Texture hxc_tmp_load_result_n703 = hxc_titleTexture;
        double hxc_tmp_load_result_n704 = hxc_sourceX;
        double hxc_tmp_load_result_n705 = hxc_sourceY;
        double hxc_tmp_load_result_n706 = hxc_sourceWidth;
        double hxc_tmp_load_result_n707 = hxc_sourceHeight;
        int32_t hxc_tmp_load_result_n709 = hxc_width_h504cec6a2eb4;
        int32_t hxc_tmp_load_result_n710 = hxc_height_h197302da9393;
        DrawTexturePro(hxc_tmp_load_result_n703, (struct Rectangle){ .x = (float)hxc_tmp_load_result_n704, .y = (float)hxc_tmp_load_result_n705, .width = (float)hxc_tmp_load_result_n706, .height = (float)hxc_tmp_load_result_n707 }, (struct Rectangle){ .x = (float)0.0, .y = (float)0.0, .width = (float)((double)hxc_tmp_load_result_n709 + 0.0), .height = (float)((double)hxc_tmp_load_result_n710 + 0.0) }, (struct Vector2){ .x = (float)0.0, .y = (float)0.0 }, (float)0.0, hxc_this1_hf4ee83235bf5);
      }
      else
      {
        ClearBackground((struct Color){ .r = 126, .g = 190, .b = 201, .a = 255 });
      }
      struct Color hxc_this1_h65866ca13a5a = (struct Color){ .r = 4, .g = 14, .b = 23, .a = 132 };
      int32_t hxc_tmp_load_result_n716 = hxc_width_h504cec6a2eb4;
      int32_t hxc_tmp_load_result_n717 = hxc_height_h197302da9393;
      DrawRectangle((int32_t)0, (int32_t)0, (int32_t)hxc_tmp_load_result_n716, (int32_t)hxc_tmp_load_result_n717, hxc_this1_h65866ca13a5a);
      if (hxc_wordmarkTextureReady)
      {
        int32_t hxc_centerX = hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_width_h504cec6a2eb4, (double)2));
        struct Color hxc_this1_h4705f486231e = (struct Color){ .r = 255, .g = 255, .b = 255, .a = 255 };
        double hxc_scale = hxc_f64_divide_zero_safe((double)560, (double)(int32_t)hxc_wordmarkTexture.width);
        double hxc_heightScale = hxc_f64_divide_zero_safe((double)132, (double)(int32_t)hxc_wordmarkTexture.height);
        double hxc_tmp_load_result_n726 = hxc_heightScale;
        if (hxc_tmp_load_result_n726 < hxc_scale)
        {
          hxc_scale = hxc_heightScale;
        }
        struct Texture hxc_tmp_load_result_n729 = hxc_wordmarkTexture;
        double hxc_width_h02eba707a098 = (double)(int32_t)hxc_tmp_load_result_n729.width * hxc_scale;
        struct Texture hxc_tmp_load_result_n732 = hxc_wordmarkTexture;
        double hxc_height_hbc7d892002b5 = (double)(int32_t)hxc_tmp_load_result_n732.height * hxc_scale;
        double hxc_width1 = (double)(int32_t)hxc_wordmarkTexture.width + 0.0;
        double hxc_height1 = (double)(int32_t)hxc_wordmarkTexture.height + 0.0;
        struct Texture hxc_tmp_load_result_n739 = hxc_wordmarkTexture;
        double hxc_tmp_load_result_n740 = hxc_width1;
        double hxc_tmp_load_result_n741 = hxc_height1;
        int32_t hxc_tmp_load_result_n743 = hxc_centerX;
        double hxc_tmp_load_result_n744 = hxc_width_h02eba707a098;
        double hxc_tmp_load_result_n745 = hxc_width_h02eba707a098;
        double hxc_tmp_load_result_n746 = hxc_height_hbc7d892002b5;
        DrawTexturePro(hxc_tmp_load_result_n739, (struct Rectangle){ .x = (float)0.0, .y = (float)0.0, .width = (float)hxc_tmp_load_result_n740, .height = (float)hxc_tmp_load_result_n741 }, (struct Rectangle){ .x = (float)((double)hxc_tmp_load_result_n743 - hxc_tmp_load_result_n744 * 0.5), .y = (float)28.0, .width = (float)hxc_tmp_load_result_n745, .height = (float)hxc_tmp_load_result_n746 }, (struct Vector2){ .x = (float)0.0, .y = (float)0.0 }, (float)0.0, hxc_this1_h4705f486231e);
      }
      else
      {
        int32_t hxc_x_h14c157519368 = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_width_h504cec6a2eb4, (double)2)), 92);
        struct Color hxc_this1_h10e72277d0b7 = (struct Color){ .r = 242, .g = 249, .b = 245, .a = 255 };
        const char *hxc_tmp_call_result_n753 = hxc_caxecraft_localization_UiCatalog_text(hxc_locale, 34);
        int32_t hxc_tmp_load_result_n754 = hxc_x_h14c157519368;
        DrawText(hxc_tmp_call_result_n753, (int32_t)hxc_tmp_load_result_n754, (int32_t)52, (int32_t)34, hxc_this1_h10e72277d0b7);
      }
      int32_t hxc_firstTop = hxc_i32_subtract_wrapping(hxc_height_h197302da9393, 293);
      int32_t hxc_left_h2a97554ae919 = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_width_h504cec6a2eb4, (double)2)), 180);
      if (hxc_selectedMode == 0)
      {
        struct Color hxc_this1_h81147b7f0edc = (struct Color){ .r = 16, .g = 88, .b = 102, .a = 232 };
        int32_t hxc_tmp_load_result_n760 = hxc_left_h2a97554ae919;
        int32_t hxc_tmp_load_result_n761 = hxc_firstTop;
        DrawRectangle((int32_t)hxc_tmp_load_result_n760, (int32_t)hxc_tmp_load_result_n761, (int32_t)360, (int32_t)54, hxc_this1_h81147b7f0edc);
        struct Color hxc_this1_hbe18ce892915 = (struct Color){ .r = 255, .g = 190, .b = 55, .a = 255 };
        int32_t hxc_tmp_load_result_n764 = hxc_left_h2a97554ae919;
        int32_t hxc_tmp_load_result_n765 = hxc_firstTop;
        DrawRectangleLines((int32_t)hxc_tmp_load_result_n764, (int32_t)hxc_tmp_load_result_n765, (int32_t)360, (int32_t)54, hxc_this1_hbe18ce892915);
      }
      else
      {
        struct Color hxc_this1_h1e4fac92e06c = (struct Color){ .r = 6, .g = 26, .b = 36, .a = 218 };
        int32_t hxc_tmp_load_result_n768 = hxc_left_h2a97554ae919;
        int32_t hxc_tmp_load_result_n769 = hxc_firstTop;
        DrawRectangle((int32_t)hxc_tmp_load_result_n768, (int32_t)hxc_tmp_load_result_n769, (int32_t)360, (int32_t)54, hxc_this1_h1e4fac92e06c);
        struct Color hxc_this1_hb9be82e415dd = (struct Color){ .r = 92, .g = 194, .b = 188, .a = 255 };
        int32_t hxc_tmp_load_result_n772 = hxc_left_h2a97554ae919;
        int32_t hxc_tmp_load_result_n773 = hxc_firstTop;
        DrawRectangleLines((int32_t)hxc_tmp_load_result_n772, (int32_t)hxc_tmp_load_result_n773, (int32_t)360, (int32_t)54, hxc_this1_hb9be82e415dd);
      }
      struct Color hxc_this1_h9e71f3705060 = (struct Color){ .r = 242, .g = 249, .b = 245, .a = 255 };
      const char *hxc_tmp_call_result_n777 = hxc_caxecraft_localization_UiCatalog_text(hxc_locale, 27);
      int32_t hxc_tmp_load_result_n778 = hxc_left_h2a97554ae919;
      int32_t hxc_tmp_load_result_n779 = hxc_firstTop;
      DrawText(hxc_tmp_call_result_n777, (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n778, 28), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n779, 16), (int32_t)21, hxc_this1_h9e71f3705060);
      int32_t hxc_top_h796aee580161 = hxc_i32_add_wrapping(hxc_i32_add_wrapping(hxc_firstTop, 54), 14);
      int32_t hxc_left_h540168b234c7 = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_width_h504cec6a2eb4, (double)2)), 180);
      if (hxc_selectedMode == 1)
      {
        struct Color hxc_this1_hb5724793d605 = (struct Color){ .r = 16, .g = 88, .b = 102, .a = 232 };
        int32_t hxc_tmp_load_result_n785 = hxc_left_h540168b234c7;
        int32_t hxc_tmp_load_result_n786 = hxc_top_h796aee580161;
        DrawRectangle((int32_t)hxc_tmp_load_result_n785, (int32_t)hxc_tmp_load_result_n786, (int32_t)360, (int32_t)54, hxc_this1_hb5724793d605);
        struct Color hxc_this1_ha49e34628fff = (struct Color){ .r = 255, .g = 190, .b = 55, .a = 255 };
        int32_t hxc_tmp_load_result_n789 = hxc_left_h540168b234c7;
        int32_t hxc_tmp_load_result_n790 = hxc_top_h796aee580161;
        DrawRectangleLines((int32_t)hxc_tmp_load_result_n789, (int32_t)hxc_tmp_load_result_n790, (int32_t)360, (int32_t)54, hxc_this1_ha49e34628fff);
      }
      else
      {
        struct Color hxc_this1_hdf8372e0bf6d = (struct Color){ .r = 6, .g = 26, .b = 36, .a = 218 };
        int32_t hxc_tmp_load_result_n793 = hxc_left_h540168b234c7;
        int32_t hxc_tmp_load_result_n794 = hxc_top_h796aee580161;
        DrawRectangle((int32_t)hxc_tmp_load_result_n793, (int32_t)hxc_tmp_load_result_n794, (int32_t)360, (int32_t)54, hxc_this1_hdf8372e0bf6d);
        struct Color hxc_this1_h1eb9b322c3a1 = (struct Color){ .r = 92, .g = 194, .b = 188, .a = 255 };
        int32_t hxc_tmp_load_result_n797 = hxc_left_h540168b234c7;
        int32_t hxc_tmp_load_result_n798 = hxc_top_h796aee580161;
        DrawRectangleLines((int32_t)hxc_tmp_load_result_n797, (int32_t)hxc_tmp_load_result_n798, (int32_t)360, (int32_t)54, hxc_this1_h1eb9b322c3a1);
      }
      struct Color hxc_this1_h8a7ff8d7da20 = (struct Color){ .r = 242, .g = 249, .b = 245, .a = 255 };
      const char *hxc_tmp_call_result_n802 = hxc_caxecraft_localization_UiCatalog_text(hxc_locale, 26);
      int32_t hxc_tmp_load_result_n803 = hxc_left_h540168b234c7;
      int32_t hxc_tmp_load_result_n804 = hxc_top_h796aee580161;
      DrawText(hxc_tmp_call_result_n802, (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n803, 28), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n804, 16), (int32_t)21, hxc_this1_h8a7ff8d7da20);
      int32_t hxc_top_h7f5769ca8f20 = hxc_i32_add_wrapping(hxc_firstTop, 136);
      int32_t hxc_left_h4ab38d92dc09 = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_width_h504cec6a2eb4, (double)2)), 180);
      struct Color hxc_this1_h53c5fe42188c = (struct Color){ .r = 6, .g = 26, .b = 36, .a = 218 };
      int32_t hxc_tmp_load_result_n809 = hxc_left_h4ab38d92dc09;
      int32_t hxc_tmp_load_result_n810 = hxc_top_h7f5769ca8f20;
      DrawRectangle((int32_t)hxc_tmp_load_result_n809, (int32_t)hxc_tmp_load_result_n810, (int32_t)360, (int32_t)54, hxc_this1_h53c5fe42188c);
      struct Color hxc_this1_ha21a62af69ed = (struct Color){ .r = 92, .g = 194, .b = 188, .a = 255 };
      int32_t hxc_tmp_load_result_n813 = hxc_left_h4ab38d92dc09;
      int32_t hxc_tmp_load_result_n814 = hxc_top_h7f5769ca8f20;
      DrawRectangleLines((int32_t)hxc_tmp_load_result_n813, (int32_t)hxc_tmp_load_result_n814, (int32_t)360, (int32_t)54, hxc_this1_ha21a62af69ed);
      struct Color hxc_this1_hb3575ed36a6c = (struct Color){ .r = 242, .g = 249, .b = 245, .a = 255 };
      const char *hxc_tmp_call_result_n818 = hxc_caxecraft_localization_UiCatalog_text(hxc_locale, 28);
      int32_t hxc_tmp_load_result_n819 = hxc_left_h4ab38d92dc09;
      int32_t hxc_tmp_load_result_n820 = hxc_top_h7f5769ca8f20;
      DrawText(hxc_tmp_call_result_n818, (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n819, 28), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n820, 16), (int32_t)21, hxc_this1_hb3575ed36a6c);
      int32_t hxc_x_h948d1ed02b0e = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_width_h504cec6a2eb4, (double)2)), 285);
      struct Color hxc_this1_h4ca41076d8e2 = (struct Color){ .r = 229, .g = 241, .b = 235, .a = 255 };
      const char *hxc_tmp_call_result_n825 = hxc_caxecraft_localization_UiCatalog_text(hxc_locale, 29);
      int32_t hxc_tmp_load_result_n826 = hxc_x_h948d1ed02b0e;
      int32_t hxc_tmp_load_result_n827 = hxc_height_h197302da9393;
      DrawText(hxc_tmp_call_result_n825, (int32_t)hxc_tmp_load_result_n826, (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n827, 58), (int32_t)16, hxc_this1_h4ca41076d8e2);
      if (hxc_selectedMode == 1)
      {
        int32_t hxc_x_hb411b69510fa = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_width_h504cec6a2eb4, (double)2)), 230);
        struct Color hxc_this1_h13e9aaee94f6 = (struct Color){ .r = 255, .g = 205, .b = 91, .a = 255 };
        const char *hxc_tmp_call_result_n833 = hxc_caxecraft_localization_FirstPlayableCatalog_text(hxc_locale, 1);
        int32_t hxc_tmp_load_result_n834 = hxc_x_hb411b69510fa;
        int32_t hxc_tmp_load_result_n835 = hxc_firstTop;
        DrawText(hxc_tmp_call_result_n833, (int32_t)hxc_tmp_load_result_n834, (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n835, 36), (int32_t)17, hxc_this1_h13e9aaee94f6);
      }
    }
    else
    {
      if (hxc_onEditor)
      {
        struct hxc_caxecraft_app_CaxecraftEditorScreen *hxc_tmp_class_field_load_result_n838 = (*hxc_self).hxc_editorScreen;
        hxc_gc_roots[1] = (const void *)hxc_tmp_class_field_load_result_n838;
        if (hxc_tmp_class_field_load_result_n838 == NULL)
        {
          abort();
        }
        int32_t hxc_tmp_load_result_n839 = hxc_locale;
        enum hxc_caxecraft_app_EditorScreenAction hxc_tmp_instance_call_result_n841 = hxc_caxecraft_app_CaxecraftEditorScreen_draw(hxc_tmp_class_field_load_result_n838, hxc_tmp_load_result_n839, hxc_editorNavigationCommand);
        if (hxc_tmp_instance_call_result_n841 == hxc_caxecraft_app_EditorScreenAction_ReturnToTitle)
        {
          enum hxc_caxecraft_app_AppScreen hxc_tmp_call_result_n844 = hxc_caxecraft_app_AppScreen_closeEditor(hxc_screen);
          hxc_screen = hxc_tmp_call_result_n844;
        }
      }
      else
      {
        ClearBackground((struct Color){ .r = 126, .g = 190, .b = 201, .a = 255 });
        int32_t hxc_tmp_native_call_result_n846 = GetScreenWidth();
        int32_t hxc_sunX = hxc_i32_subtract_wrapping((int32_t)hxc_tmp_native_call_result_n846, 260);
        float hxc_radius_hdae0cb24e58d = (float)42.0;
        struct Color hxc_this1_h7e17f9d9a961 = (struct Color){ .r = 255, .g = 226, .b = 136, .a = 255 };
        int32_t hxc_tmp_load_result_n848 = hxc_sunX;
        float hxc_tmp_load_result_n849 = hxc_radius_hdae0cb24e58d;
        DrawCircle((int32_t)hxc_tmp_load_result_n848, (int32_t)86, hxc_tmp_load_result_n849, hxc_this1_h7e17f9d9a961);
        float hxc_radius_h05a0c2d9dbf5 = (float)30.0;
        struct Color hxc_this1_hd7c19fc43bbf = (struct Color){ .r = 255, .g = 246, .b = 205, .a = 255 };
        int32_t hxc_tmp_load_result_n852 = hxc_sunX;
        float hxc_tmp_load_result_n853 = hxc_radius_h05a0c2d9dbf5;
        DrawCircle((int32_t)hxc_tmp_load_result_n852, (int32_t)86, hxc_tmp_load_result_n853, hxc_this1_hd7c19fc43bbf);
        BeginMode3D(hxc_camera);
        struct hxc_caxecraft_app_TerrainRenderer *hxc_tmp_owned_class_field_address_n856 = &(*hxc_self).hxc_terrainRenderer;
        if (hxc_tmp_owned_class_field_address_n856 == NULL)
        {
          abort();
        }
        struct hxc_caxecraft_domain_GameSession *hxc_tmp_owned_class_field_address_n857 = &(*hxc_self).hxc_session;
        if (hxc_tmp_owned_class_field_address_n857 == NULL)
        {
          abort();
        }
        size_t hxc_tmp_length_n859;
        const uint8_t *hxc_tmp_instance_call_result_n858 = hxc_caxecraft_domain_GameSession_worldView(hxc_tmp_owned_class_field_address_n857, &hxc_tmp_length_n859);
        struct Texture hxc_tmp_load_result_n860 = hxc_terrainTexture;
        bool hxc_tmp_load_result_n861 = hxc_terrainTextureReady;
        struct Texture hxc_tmp_load_result_n862 = hxc_adventureTerrainTexture;
        bool hxc_tmp_load_result_n863 = hxc_adventureTerrainTextureReady;
        double hxc_tmp_record_field_load_result_n864 = hxc_renderPosition.hxc_x;
        struct hxc_caxecraft_app_TerrainRenderCounters hxc_tmp_instance_call_result_n866 = hxc_caxecraft_app_TerrainRenderer_draw(hxc_tmp_owned_class_field_address_n856, hxc_tmp_instance_call_result_n858, hxc_tmp_length_n859, hxc_tmp_load_result_n860, hxc_tmp_load_result_n861, hxc_tmp_load_result_n862, hxc_tmp_load_result_n863, hxc_tmp_record_field_load_result_n864, hxc_renderPosition.hxc_z);
        struct hxc_caxecraft_app_TerrainRenderCounters hxc_renderCounters = hxc_tmp_instance_call_result_n866;
        struct hxc_caxecraft_domain_GameSession *hxc_tmp_owned_class_field_address_n867 = &(*hxc_self).hxc_session;
        if (hxc_tmp_owned_class_field_address_n867 == NULL)
        {
          abort();
        }
        size_t hxc_tmp_length_n869;
        const uint8_t *hxc_tmp_instance_call_result_n868 = hxc_caxecraft_domain_GameSession_worldView(hxc_tmp_owned_class_field_address_n867, &hxc_tmp_length_n869);
        struct Texture hxc_tmp_load_result_n870 = hxc_terrainTexture;
        bool hxc_tmp_load_result_n871 = hxc_terrainTextureReady;
        struct hxc_caxecraft_app_WaterRenderCounters hxc_tmp_call_result_n873 = hxc_caxecraft_app_WaterRenderer_draw(hxc_tmp_instance_call_result_n868, hxc_tmp_length_n869, hxc_tmp_load_result_n870, hxc_tmp_load_result_n871, hxc_waterPresentationCell);
        struct hxc_caxecraft_app_WaterRenderCounters hxc_waterCounters = hxc_tmp_call_result_n873;
        int32_t hxc_tmp_record_field_load_result_n874 = hxc_renderCounters.hxc_visible;
        int32_t hxc_totalVisible = hxc_i32_add_wrapping(hxc_tmp_record_field_load_result_n874, hxc_waterCounters.hxc_visible);
        int32_t hxc_tmp_record_field_load_result_n876 = hxc_renderCounters.hxc_drawCalls;
        int32_t hxc_totalDrawCalls = hxc_i32_add_wrapping(hxc_tmp_record_field_load_result_n876, hxc_waterCounters.hxc_drawCalls);
        struct Camera3D hxc_tmp_load_result_n878 = hxc_camera;
        struct Texture hxc_tmp_load_result_n879 = hxc_entityTexture;
        bool hxc_tmp_load_result_n880 = hxc_entityTextureReady;
        struct hxc_caxecraft_domain_Character hxc_tmp_load_result_n881 = hxc_dialogueActor;
        struct hxc_caxecraft_domain_Character hxc_tmp_load_result_n882 = hxc_enemyActor;
        enum hxc_caxecraft_domain_ActorControllerPhase hxc_tmp_record_field_load_result_n883 = hxc_enemyPhase.hxc_phase;
        hxc_caxecraft_app_CaxecraftApp_drawActors(hxc_tmp_load_result_n878, hxc_tmp_load_result_n879, hxc_tmp_load_result_n880, hxc_tmp_load_result_n881, hxc_tmp_load_result_n882, hxc_tmp_record_field_load_result_n883, hxc_berryDrop);
        struct Camera3D hxc_tmp_load_result_n885 = hxc_camera;
        struct hxc_caxecraft_domain_GameSession *hxc_tmp_owned_class_field_address_n886 = &(*hxc_self).hxc_session;
        if (hxc_tmp_owned_class_field_address_n886 == NULL)
        {
          abort();
        }
        size_t hxc_tmp_length_n888;
        const int32_t *hxc_tmp_instance_call_result_n887 = hxc_caxecraft_domain_GameSession_authoredItemsView(hxc_tmp_owned_class_field_address_n886, &hxc_tmp_length_n888);
        struct Texture hxc_tmp_load_result_n889 = hxc_itemTexture;
        bool hxc_tmp_load_result_n890 = hxc_itemTextureReady;
        struct Texture hxc_tmp_load_result_n891 = hxc_adventureItemTexture;
        hxc_caxecraft_app_AuthoredItemRenderer_drawWorldItems(hxc_tmp_load_result_n885, hxc_tmp_instance_call_result_n887, hxc_tmp_length_n888, hxc_tmp_load_result_n889, hxc_tmp_load_result_n890, hxc_tmp_load_result_n891, hxc_adventureItemTextureReady);
        if (hxc_hit.hxc_hit)
        {
          double hxc_y = (double)hxc_hit.hxc_cellY + 0.5;
          double hxc_z = (double)hxc_hit.hxc_cellZ + 0.5;
          int32_t hxc_tmp_record_field_load_result_n896 = hxc_hit.hxc_cellX;
          double hxc_tmp_load_result_n897 = hxc_y;
          DrawCubeWires((struct Vector3){ .x = (float)((double)hxc_tmp_record_field_load_result_n896 + 0.5), .y = (float)hxc_tmp_load_result_n897, .z = (float)hxc_z }, (float)1.04, (float)1.04, (float)1.04, (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 });
        }
        EndMode3D();
        if (hxc_cameraWaterBlend > 0.0)
        {
          int32_t hxc_overlayAlpha = hxc_f64_to_i32_saturating(105.0 * hxc_cameraWaterBlend);
          int32_t hxc_tmp_native_call_result_n903 = GetScreenWidth();
          int32_t hxc_width_h9b42d6c790e0 = (int32_t)hxc_tmp_native_call_result_n903;
          int32_t hxc_tmp_native_call_result_n904 = GetScreenHeight();
          int32_t hxc_height_h08271c6c330e = (int32_t)hxc_tmp_native_call_result_n904;
          uint8_t hxc_red = (uint8_t)31;
          uint8_t hxc_green = (uint8_t)115;
          uint8_t hxc_blue = (uint8_t)154;
          int32_t hxc_tmp_load_result_n905 = hxc_overlayAlpha;
          int32_t hxc_tmp_conditional_result_n282 = 0;
          if (hxc_tmp_load_result_n905 < 0)
          {
            hxc_tmp_conditional_result_n282 = 0;
          }
          else
          {
            int32_t hxc_tmp_load_result_n906 = hxc_overlayAlpha;
            int32_t hxc_tmp_conditional_result_n283 = 0;
            if (hxc_tmp_load_result_n906 > 255)
            {
              hxc_tmp_conditional_result_n283 = 255;
            }
            else
            {
              hxc_tmp_conditional_result_n283 = hxc_overlayAlpha;
            }
            hxc_tmp_conditional_result_n282 = hxc_tmp_conditional_result_n283;
          }
          uint8_t hxc_alpha = (uint8_t)hxc_tmp_conditional_result_n282;
          uint8_t hxc_tmp_load_result_n910 = hxc_red;
          uint8_t hxc_tmp_load_result_n911 = hxc_green;
          uint8_t hxc_tmp_load_result_n912 = hxc_blue;
          struct Color hxc_this1_hc051046c2c65 = (struct Color){ .r = hxc_tmp_load_result_n910, .g = hxc_tmp_load_result_n911, .b = hxc_tmp_load_result_n912, .a = hxc_alpha };
          int32_t hxc_tmp_load_result_n915 = hxc_width_h9b42d6c790e0;
          int32_t hxc_tmp_load_result_n916 = hxc_height_h08271c6c330e;
          DrawRectangle((int32_t)0, (int32_t)0, (int32_t)hxc_tmp_load_result_n915, (int32_t)hxc_tmp_load_result_n916, hxc_this1_hc051046c2c65);
        }
        int32_t hxc_tmp_load_result_n918 = hxc_totalVisible;
        int32_t hxc_tmp_load_result_n919 = hxc_totalDrawCalls;
        int32_t hxc_tmp_load_result_n920 = hxc_frameCount;
        int32_t hxc_tmp_load_result_n921 = hxc_completedTicks;
        double hxc_tmp_record_field_load_result_n923 = hxc_character.hxc_body.hxc_x;
        double hxc_tmp_record_field_load_result_n924 = hxc_character.hxc_body.hxc_z;
        struct hxc_caxecraft_domain_VitalsState hxc_tmp_record_field_load_result_n925 = hxc_character.hxc_vitals;
        int32_t hxc_tmp_load_result_n926 = hxc_aquaticEquipmentCode;
        int32_t hxc_tmp_load_result_n927 = hxc_aquaticEquipmentFrames;
        bool hxc_tmp_record_field_load_result_n928 = hxc_character.hxc_aquatic.hxc_headSubmerged;
        int32_t hxc_tmp_record_field_load_result_n929 = hxc_character.hxc_aquatic.hxc_breathTicks;
        int32_t hxc_tmp_record_field_load_result_n930 = hxc_character.hxc_aquaticProfile.hxc_maximumBreathTicks;
        int32_t hxc_tmp_load_result_n932 = hxc_placementBlockedFrames;
        int32_t hxc_tmp_load_result_n933 = hxc_strikeHitFrames;
        int32_t hxc_tmp_load_result_n934 = hxc_enemyDefeatedFrames;
        int32_t hxc_tmp_load_result_n935 = hxc_enemyAttackFrames;
        int32_t hxc_tmp_load_result_n936 = hxc_pickupFrames;
        int32_t hxc_tmp_load_result_n937 = hxc_pickupAmount;
        int32_t hxc_tmp_load_result_n938 = hxc_inventoryFullReason;
        int32_t hxc_tmp_load_result_n939 = hxc_recoveryFeedback;
        int32_t hxc_tmp_load_result_n940 = hxc_recoveryFeedbackFrames;
        bool hxc_tmp_load_result_n942 = hxc_paused_h86863acb5f2c;
        bool hxc_tmp_load_result_n943 = hxc_captured_h94383143b373;
        struct hxc_caxecraft_domain_RaycastHit hxc_tmp_load_result_n944 = hxc_hit;
        int32_t hxc_tmp_load_result_n945 = hxc_selectedMode;
        int32_t hxc_tmp_load_result_n946 = hxc_locale;
        struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_load_result_n947 = hxc_inventory;
        enum hxc_caxecraft_gameplay_GuidePhase hxc_tmp_load_result_n948 = hxc_guidePhase;
        bool hxc_tmp_load_result_n949 = hxc_guideInteractionAvailable;
        struct hxc_caxecraft_domain_Character hxc_tmp_load_result_n950 = hxc_enemyActor;
        struct hxc_caxecraft_app_HudView hxc_hudView = (struct hxc_caxecraft_app_HudView){ .hxc_character = (struct hxc_caxecraft_app_HudCharacterView){ .hxc_aquaticEquipmentCode = hxc_tmp_load_result_n926, .hxc_aquaticEquipmentVisible = hxc_tmp_load_result_n927 > 0, .hxc_breathTicks = hxc_tmp_record_field_load_result_n929, .hxc_headSubmerged = hxc_tmp_record_field_load_result_n928, .hxc_maximumBreathTicks = hxc_tmp_record_field_load_result_n930, .hxc_vitals = hxc_tmp_record_field_load_result_n925, .hxc_x = hxc_tmp_record_field_load_result_n923, .hxc_z = hxc_tmp_record_field_load_result_n924 }, .hxc_enemy = hxc_tmp_load_result_n950, .hxc_enemyPhase = hxc_enemyPhase.hxc_phase, .hxc_feedback = (struct hxc_caxecraft_app_HudFeedbackView){ .hxc_enemyAttacked = hxc_tmp_load_result_n935 > 0, .hxc_enemyDefeated = hxc_tmp_load_result_n934 > 0, .hxc_inventoryFullReason = hxc_tmp_load_result_n938, .hxc_pickedUp = hxc_tmp_load_result_n936 > 0, .hxc_pickupAmount = hxc_tmp_load_result_n937, .hxc_placementBlocked = hxc_tmp_load_result_n932 > 0, .hxc_recoveryDecision = hxc_tmp_load_result_n939, .hxc_recoveryVisible = hxc_tmp_load_result_n940 > 0, .hxc_strikeHit = hxc_tmp_load_result_n933 > 0 }, .hxc_guideInteractionAvailable = hxc_tmp_load_result_n949, .hxc_guidePhase = hxc_tmp_load_result_n948, .hxc_hit = hxc_tmp_load_result_n944, .hxc_inventory = hxc_tmp_load_result_n947, .hxc_locale = hxc_tmp_load_result_n946, .hxc_metrics = (struct hxc_caxecraft_app_HudMetricsView){ .hxc_completedTicks = hxc_tmp_load_result_n921, .hxc_drawCalls = hxc_tmp_load_result_n919, .hxc_renderedFrames = hxc_tmp_load_result_n920, .hxc_visibleBlocks = hxc_tmp_load_result_n918 }, .hxc_mode = hxc_tmp_load_result_n945, .hxc_paused = hxc_tmp_load_result_n942, .hxc_pointerCaptured = hxc_tmp_load_result_n943 };
        struct hxc_caxecraft_app_HudView hxc_tmp_load_result_n953 = hxc_hudView;
        hxc_caxecraft_app_CaxecraftApp_drawHud(hxc_tmp_load_result_n953, hxc_hudResources);
      }
    }
    EndDrawing();
    hxc_frameCount = hxc_i32_add_wrapping(hxc_frameCount, 1);
    if (hxc_array_ref_release(hxc_presentationActorPhases) != HXC_STATUS_OK)
    {
      abort();
    }
  }
  EnableCursor();
  if (hxc_adventureTerrainTextureReady)
  {
    UnloadTexture(hxc_adventureTerrainTexture);
  }
  if (hxc_terrainTextureReady)
  {
    UnloadTexture(hxc_terrainTexture);
  }
  if (hxc_entityTextureReady)
  {
    UnloadTexture(hxc_entityTexture);
  }
  if (hxc_adventureItemTextureReady)
  {
    UnloadTexture(hxc_adventureItemTexture);
  }
  if (hxc_itemTextureReady)
  {
    UnloadTexture(hxc_itemTexture);
  }
  if (hxc_hudTextureReady)
  {
    UnloadTexture(hxc_hudTexture);
  }
  if (hxc_wordmarkTextureReady)
  {
    UnloadTexture(hxc_wordmarkTexture);
  }
  if (hxc_titleTextureReady)
  {
    UnloadTexture(hxc_titleTexture);
  }
  CloseWindow();
  if (hxc_array_ref_release(hxc_initialActorPhases) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_record_c82cbeac_destroy(&hxc_loadedLevel);
  if (hxc_gc_root_frame_pop(&hxc_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return;
}
