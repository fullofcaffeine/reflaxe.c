#include "hxc/program.h"

void hxc_compiler_constructor_caxecraft_app_CaxecraftApp(struct hxc_caxecraft_app_CaxecraftApp *hxc_self)
{
  struct hxc_caxecraft_app_TerrainRenderer *hxc_tmp_owned_class_field_address_n0 = &(*hxc_self).hxc_terrainRenderer;
  hxc_compiler_constructor_caxecraft_app_TerrainRenderer(hxc_tmp_owned_class_field_address_n0);
  struct hxc_caxecraft_domain_GameSession *hxc_tmp_owned_class_field_address_n1 = &(*hxc_self).hxc_session;
  hxc_compiler_constructor_caxecraft_domain_GameSession(hxc_tmp_owned_class_field_address_n1);
  return;
}

void hxc_caxecraft_app_CaxecraftApp_drawActors(struct Camera3D hxc_camera, struct Texture hxc_entityTexture, bool hxc_entityTextureReady, struct hxc_caxecraft_gameplay_GuideState hxc_guide, struct hxc_caxecraft_gameplay_MosslingState hxc_mossling, struct hxc_caxecraft_gameplay_BerryDropState hxc_berryDrop)
{
  if (hxc_entityTextureReady)
  {
    double hxc_y_h34e6c3d91cc3 = hxc_guide.hxc_y + 0.76;
    double hxc_z_h43670fbe1896 = hxc_guide.hxc_z;
    double hxc_tmp_load_result_n4 = hxc_y_h34e6c3d91cc3;
    hxc_caxecraft_app_CaxecraftAtlas_drawWorldSprite(hxc_camera, hxc_entityTexture, hxc_caxecraft_app_WorldSprite_NiaFront, (struct Vector3){ .x = (float)hxc_guide.hxc_x, .y = (float)hxc_tmp_load_result_n4, .z = (float)hxc_z_h43670fbe1896 }, 0.95, 1.52);
  }
  else
  {
    double hxc_y_hbd779586e650 = hxc_guide.hxc_y + 0.54;
    double hxc_z_h8eb9ddb5867e = hxc_guide.hxc_z;
    double hxc_tmp_load_result_n10 = hxc_y_hbd779586e650;
    DrawCube((struct Vector3){ .x = (float)hxc_guide.hxc_x, .y = (float)hxc_tmp_load_result_n10, .z = (float)hxc_z_h8eb9ddb5867e }, (float)0.50, (float)0.86, (float)0.42, (struct Color){ .r = 42, .g = 150, .b = 160, .a = 255 });
    double hxc_y_hda8be4e9a03f = hxc_guide.hxc_y + 1.18;
    double hxc_z_hff099cebe24b = hxc_guide.hxc_z;
    double hxc_tmp_load_result_n17 = hxc_y_hda8be4e9a03f;
    DrawCube((struct Vector3){ .x = (float)hxc_guide.hxc_x, .y = (float)hxc_tmp_load_result_n17, .z = (float)hxc_z_hff099cebe24b }, (float)0.44, (float)0.44, (float)0.44, (struct Color){ .r = 205, .g = 139, .b = 88, .a = 255 });
    double hxc_y_h1c4ac4f12c88 = hxc_guide.hxc_y + 1.41;
    double hxc_z_hb5b0e2af844a = hxc_guide.hxc_z;
    double hxc_tmp_load_result_n24 = hxc_y_h1c4ac4f12c88;
    DrawCube((struct Vector3){ .x = (float)hxc_guide.hxc_x, .y = (float)hxc_tmp_load_result_n24, .z = (float)hxc_z_hb5b0e2af844a }, (float)0.48, (float)0.16, (float)0.48, (struct Color){ .r = 62, .g = 40, .b = 47, .a = 255 });
  }
  if (hxc_mossling.hxc_health > 0)
  {
    if (hxc_entityTextureReady)
    {
      double hxc_y_ha449916beb09 = hxc_mossling.hxc_y + 0.48;
      double hxc_z_h230527b361f5 = hxc_mossling.hxc_z;
      double hxc_tmp_load_result_n33 = hxc_y_ha449916beb09;
      hxc_caxecraft_app_CaxecraftAtlas_drawWorldSprite(hxc_camera, hxc_entityTexture, hxc_caxecraft_app_WorldSprite_MosslingFront, (struct Vector3){ .x = (float)hxc_mossling.hxc_x, .y = (float)hxc_tmp_load_result_n33, .z = (float)hxc_z_h230527b361f5 }, 1.05, 0.96);
    }
    else
    {
      double hxc_y_h5095690184e6 = hxc_mossling.hxc_y + 0.30;
      double hxc_z_h184072c3cf45 = hxc_mossling.hxc_z;
      double hxc_tmp_load_result_n39 = hxc_y_h5095690184e6;
      DrawCube((struct Vector3){ .x = (float)hxc_mossling.hxc_x, .y = (float)hxc_tmp_load_result_n39, .z = (float)hxc_z_h184072c3cf45 }, (float)0.70, (float)0.54, (float)0.70, (struct Color){ .r = 74, .g = 119, .b = 67, .a = 255 });
      double hxc_y_hf2634cc37a72 = hxc_mossling.hxc_y + 0.66;
      double hxc_z_h1e0eb2a0e720 = hxc_mossling.hxc_z;
      double hxc_tmp_load_result_n46 = hxc_y_hf2634cc37a72;
      DrawCube((struct Vector3){ .x = (float)hxc_mossling.hxc_x, .y = (float)hxc_tmp_load_result_n46, .z = (float)hxc_z_h1e0eb2a0e720 }, (float)0.50, (float)0.34, (float)0.50, (struct Color){ .r = 157, .g = 190, .b = 82, .a = 255 });
    }
    if (hxc_mossling.hxc_mode == hxc_caxecraft_gameplay_MosslingMode_Windup)
    {
      double hxc_y_h58e931360bdd = hxc_mossling.hxc_y + 1.02;
      double hxc_z_h16fa6f8075ff = hxc_mossling.hxc_z;
      double hxc_tmp_load_result_n55 = hxc_y_h58e931360bdd;
      DrawCube((struct Vector3){ .x = (float)hxc_mossling.hxc_x, .y = (float)hxc_tmp_load_result_n55, .z = (float)hxc_z_h16fa6f8075ff }, (float)0.20, (float)0.20, (float)0.20, (struct Color){ .r = 218, .g = 65, .b = 72, .a = 255 });
    }
  }
  if (hxc_berryDrop.hxc_active)
  {
    double hxc_y_hc764c1a68fc8 = hxc_berryDrop.hxc_y;
    double hxc_z_h9d45203d9e6a = hxc_berryDrop.hxc_z;
    double hxc_tmp_load_result_n63 = hxc_y_hc764c1a68fc8;
    DrawCube((struct Vector3){ .x = (float)(hxc_berryDrop.hxc_x - 0.12), .y = (float)hxc_tmp_load_result_n63, .z = (float)hxc_z_h9d45203d9e6a }, (float)0.18, (float)0.18, (float)0.18, (struct Color){ .r = 174, .g = 78, .b = 136, .a = 255 });
    double hxc_y_h21ebf06f4155 = hxc_berryDrop.hxc_y;
    double hxc_z_h076fa7322aba = hxc_berryDrop.hxc_z;
    double hxc_tmp_load_result_n70 = hxc_y_h21ebf06f4155;
    DrawCube((struct Vector3){ .x = (float)(hxc_berryDrop.hxc_x + 0.12), .y = (float)hxc_tmp_load_result_n70, .z = (float)hxc_z_h076fa7322aba }, (float)0.18, (float)0.18, (float)0.18, (struct Color){ .r = 174, .g = 78, .b = 136, .a = 255 });
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
      struct Color hxc_this1_h25941348973e = (struct Color){ .r = 15, .g = 27, .b = 39, .a = 220 };
      int32_t hxc_tmp_load_result_n19 = hxc_x;
      int32_t hxc_tmp_load_result_n20 = hxc_y;
      int32_t hxc_tmp_load_result_n21 = hxc_slotSize;
      int32_t hxc_tmp_load_result_n22 = hxc_slotSize;
      DrawRectangle((int32_t)hxc_tmp_load_result_n19, (int32_t)hxc_tmp_load_result_n20, (int32_t)hxc_tmp_load_result_n21, (int32_t)hxc_tmp_load_result_n22, hxc_this1_h25941348973e);
      if (hxc_slot == hxc_inventory.hxc_selected)
      {
        struct Color hxc_this1_h1b1cf1161e6a = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
        int32_t hxc_tmp_load_result_n27 = hxc_x;
        int32_t hxc_tmp_load_result_n28 = hxc_y;
        int32_t hxc_tmp_load_result_n29 = hxc_slotSize;
        int32_t hxc_tmp_load_result_n30 = hxc_slotSize;
        DrawRectangleLines((int32_t)hxc_tmp_load_result_n27, (int32_t)hxc_tmp_load_result_n28, (int32_t)hxc_tmp_load_result_n29, (int32_t)hxc_tmp_load_result_n30, hxc_this1_h1b1cf1161e6a);
      }
      else
      {
        struct Color hxc_this1_h058c1b8fae69 = (struct Color){ .r = 228, .g = 239, .b = 227, .a = 255 };
        int32_t hxc_tmp_load_result_n33 = hxc_x;
        int32_t hxc_tmp_load_result_n34 = hxc_y;
        int32_t hxc_tmp_load_result_n35 = hxc_slotSize;
        int32_t hxc_tmp_load_result_n36 = hxc_slotSize;
        DrawRectangleLines((int32_t)hxc_tmp_load_result_n33, (int32_t)hxc_tmp_load_result_n34, (int32_t)hxc_tmp_load_result_n35, (int32_t)hxc_tmp_load_result_n36, hxc_this1_h058c1b8fae69);
      }
    }
    if (hxc_itemTextureReady)
    {
      enum hxc_caxecraft_gameplay_ItemKind hxc_tmp_call_result_n39 = hxc_caxecraft_gameplay_Inventory_itemAt(hxc_slot);
      int32_t hxc_tmp_load_result_n40 = hxc_x;
      int32_t hxc_tmp_load_result_n41 = hxc_y;
      hxc_caxecraft_app_CaxecraftAtlas_drawItem(hxc_itemTexture, hxc_tmp_call_result_n39, hxc_i32_add_wrapping(hxc_tmp_load_result_n40, 6), hxc_i32_add_wrapping(hxc_tmp_load_result_n41, 4), hxc_i32_subtract_wrapping(hxc_slotSize, 12));
    }
    int32_t hxc_tmp_call_result_n44 = hxc_caxecraft_gameplay_Inventory_countAt(hxc_inventory, hxc_slot);
    int32_t hxc_tmp_load_result_n45 = hxc_x;
    hxc_caxecraft_app_HudDigits_drawNumber(hxc_tmp_call_result_n44, hxc_i32_add_wrapping(hxc_tmp_load_result_n45, 39), hxc_i32_add_wrapping(hxc_y, 44), 2, (struct Color){ .r = 228, .g = 239, .b = 227, .a = 255 });
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
  double hxc_playerX = hxc_view.hxc_character.hxc_x;
  double hxc_playerZ = hxc_view.hxc_character.hxc_z;
  int32_t hxc_mode = hxc_view.hxc_mode;
  int32_t hxc_locale = hxc_view.hxc_locale;
  struct hxc_caxecraft_gameplay_InventoryState hxc_inventory = hxc_view.hxc_inventory;
  struct hxc_caxecraft_gameplay_GuideState hxc_guide = hxc_view.hxc_guide;
  struct hxc_caxecraft_gameplay_MosslingState hxc_mossling = hxc_view.hxc_mossling;
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
  int32_t hxc_tmp_native_call_result_n56 = GetScreenWidth();
  int32_t hxc_width = (int32_t)hxc_tmp_native_call_result_n56;
  int32_t hxc_tmp_native_call_result_n57 = GetScreenHeight();
  int32_t hxc_height = (int32_t)hxc_tmp_native_call_result_n57;
  int32_t hxc_centerX = hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_width, (double)2));
  int32_t hxc_centerY = hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_height, (double)2));
  struct Color hxc_text = (struct Color){ .r = 228, .g = 239, .b = 227, .a = 255 };
  int32_t hxc_tmp_load_result_n61 = hxc_centerX;
  int32_t hxc_tmp_load_result_n62 = hxc_centerY;
  int32_t hxc_tmp_load_result_n63 = hxc_centerX;
  int32_t hxc_tmp_load_result_n64 = hxc_centerY;
  DrawLine((int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n61, 8), (int32_t)hxc_tmp_load_result_n62, (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n63, 3), (int32_t)hxc_tmp_load_result_n64, hxc_text);
  int32_t hxc_tmp_load_result_n66 = hxc_centerX;
  int32_t hxc_tmp_load_result_n67 = hxc_centerY;
  int32_t hxc_tmp_load_result_n68 = hxc_centerX;
  int32_t hxc_tmp_load_result_n69 = hxc_centerY;
  DrawLine((int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n66, 3), (int32_t)hxc_tmp_load_result_n67, (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n68, 8), (int32_t)hxc_tmp_load_result_n69, hxc_text);
  int32_t hxc_tmp_load_result_n71 = hxc_centerX;
  int32_t hxc_tmp_load_result_n72 = hxc_centerY;
  int32_t hxc_tmp_load_result_n73 = hxc_centerX;
  int32_t hxc_tmp_load_result_n74 = hxc_centerY;
  DrawLine((int32_t)hxc_tmp_load_result_n71, (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n72, 8), (int32_t)hxc_tmp_load_result_n73, (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n74, 3), hxc_text);
  int32_t hxc_tmp_load_result_n76 = hxc_centerX;
  int32_t hxc_tmp_load_result_n77 = hxc_centerY;
  int32_t hxc_tmp_load_result_n78 = hxc_centerX;
  int32_t hxc_tmp_load_result_n79 = hxc_centerY;
  DrawLine((int32_t)hxc_tmp_load_result_n76, (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n77, 3), (int32_t)hxc_tmp_load_result_n78, (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n79, 8), hxc_text);
  struct Color hxc_this1_h5864d7a8e8ee = (struct Color){ .r = 15, .g = 27, .b = 39, .a = 220 };
  DrawRectangle((int32_t)18, (int32_t)18, (int32_t)460, (int32_t)108, hxc_this1_h5864d7a8e8ee);
  struct Color hxc_this1_h872be161d131 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
  DrawRectangleLines((int32_t)18, (int32_t)18, (int32_t)460, (int32_t)108, hxc_this1_h872be161d131);
  const char *hxc_tmp_call_result_n86 = hxc_caxecraft_localization_UiCatalog_text(hxc_locale, 1);
  DrawText(hxc_tmp_call_result_n86, (int32_t)32, (int32_t)28, (int32_t)20, hxc_text);
  const char *hxc_tmp_call_result_n89 = hxc_caxecraft_localization_UiCatalog_text(hxc_locale, 4);
  DrawText(hxc_tmp_call_result_n89, (int32_t)32, (int32_t)58, (int32_t)14, hxc_text);
  hxc_caxecraft_app_HudDigits_drawNumber(16384, 82, 59, 5, (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 });
  const char *hxc_tmp_call_result_n93 = hxc_caxecraft_localization_UiCatalog_text(hxc_locale, 8);
  DrawText(hxc_tmp_call_result_n93, (int32_t)160, (int32_t)58, (int32_t)14, hxc_text);
  hxc_caxecraft_app_HudDigits_drawNumber(hxc_visible, 230, 59, 5, (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 });
  const char *hxc_tmp_call_result_n98 = hxc_caxecraft_localization_UiCatalog_text(hxc_locale, 5);
  DrawText(hxc_tmp_call_result_n98, (int32_t)326, (int32_t)58, (int32_t)14, hxc_text);
  hxc_caxecraft_app_HudDigits_drawNumber(hxc_drawCalls, 382, 59, 5, (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 });
  const char *hxc_tmp_call_result_n103 = hxc_caxecraft_localization_UiCatalog_text(hxc_locale, 6);
  DrawText(hxc_tmp_call_result_n103, (int32_t)32, (int32_t)86, (int32_t)12, hxc_text);
  int32_t hxc_tmp_load_result_n105 = hxc_frames;
  hxc_caxecraft_app_HudDigits_drawNumber(hxc_tmp_load_result_n105, 82, 85, 6, hxc_text);
  const char *hxc_tmp_call_result_n108 = hxc_caxecraft_localization_UiCatalog_text(hxc_locale, 7);
  DrawText(hxc_tmp_call_result_n108, (int32_t)174, (int32_t)86, (int32_t)12, hxc_text);
  int32_t hxc_tmp_load_result_n110 = hxc_updates;
  hxc_caxecraft_app_HudDigits_drawNumber(hxc_tmp_load_result_n110, 216, 85, 6, hxc_text);
  struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_load_result_n112 = hxc_inventory;
  struct Texture hxc_tmp_load_result_n113 = hxc_hudTexture;
  bool hxc_tmp_load_result_n114 = hxc_hudTextureReady;
  struct Texture hxc_tmp_load_result_n115 = hxc_itemTexture;
  bool hxc_tmp_load_result_n116 = hxc_itemTextureReady;
  int32_t hxc_tmp_load_result_n117 = hxc_width;
  hxc_caxecraft_app_CaxecraftApp_drawHotbar(hxc_tmp_load_result_n112, hxc_tmp_load_result_n113, hxc_tmp_load_result_n114, hxc_tmp_load_result_n115, hxc_tmp_load_result_n116, hxc_tmp_load_result_n117, hxc_height);
  struct hxc_caxecraft_domain_VitalsState hxc_tmp_load_result_n119 = hxc_vitals;
  struct Texture hxc_tmp_load_result_n120 = hxc_hudTexture;
  bool hxc_tmp_load_result_n121 = hxc_hudTextureReady;
  hxc_caxecraft_app_CaxecraftApp_drawHealth(hxc_tmp_load_result_n119, hxc_tmp_load_result_n120, hxc_tmp_load_result_n121, hxc_width);
  if (hxc_aquaticEquipmentCode >= 0)
  {
    int32_t hxc_tmp_load_result_n124 = hxc_aquaticEquipmentCode;
    struct Texture hxc_tmp_load_result_n125 = hxc_itemTexture;
    bool hxc_tmp_load_result_n126 = hxc_itemTextureReady;
    struct Texture hxc_tmp_load_result_n127 = hxc_adventureItemTexture;
    bool hxc_tmp_load_result_n128 = hxc_adventureItemTextureReady;
    hxc_caxecraft_app_AuthoredItemRenderer_drawEquippedIcon(hxc_tmp_load_result_n124, hxc_tmp_load_result_n125, hxc_tmp_load_result_n126, hxc_tmp_load_result_n127, hxc_tmp_load_result_n128, hxc_i32_subtract_wrapping(hxc_width, 226), 18, 42);
  }
  if (hxc_headSubmerged)
  {
    int32_t hxc_tmp_load_result_n131 = hxc_breathTicks;
    int32_t hxc_tmp_load_result_n132 = hxc_maximumBreathTicks;
    int32_t hxc_tmp_load_result_n133 = hxc_width;
    hxc_caxecraft_app_CaxecraftApp_drawBreath(hxc_tmp_load_result_n131, hxc_tmp_load_result_n132, hxc_tmp_load_result_n133, hxc_height);
  }
  const char *hxc_tmp_call_result_n136 = hxc_caxecraft_localization_UiCatalog_text(hxc_locale, 3);
  int32_t hxc_tmp_load_result_n137 = hxc_height;
  DrawText(hxc_tmp_call_result_n136, (int32_t)20, (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n137, 22), (int32_t)14, hxc_text);
  if (hxc_mode == 1)
  {
    struct Color hxc_this1_h6e813e4c46b6 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
    const char *hxc_tmp_call_result_n142 = hxc_caxecraft_localization_FirstPlayableCatalog_text(hxc_locale, 0);
    DrawText(hxc_tmp_call_result_n142, (int32_t)32, (int32_t)110, (int32_t)14, hxc_this1_h6e813e4c46b6);
  }
  struct hxc_caxecraft_gameplay_GuideState hxc_tmp_load_result_n144 = hxc_guide;
  double hxc_tmp_load_result_n145 = hxc_playerX;
  bool hxc_tmp_call_result_n147 = hxc_caxecraft_gameplay_GuideNpc_isInRange(hxc_tmp_load_result_n144, hxc_tmp_load_result_n145, hxc_playerZ);
  if (hxc_tmp_call_result_n147)
  {
    struct Color hxc_this1_h63506214cef5 = (struct Color){ .r = 15, .g = 27, .b = 39, .a = 220 };
    int32_t hxc_tmp_load_result_n149 = hxc_centerX;
    int32_t hxc_tmp_load_result_n150 = hxc_centerY;
    DrawRectangle((int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n149, 260), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n150, 54), (int32_t)520, (int32_t)60, hxc_this1_h63506214cef5);
    if (hxc_guide.hxc_phase == hxc_caxecraft_gameplay_GuidePhase_Waiting)
    {
      const char *hxc_tmp_call_result_n155 = hxc_caxecraft_localization_FirstPlayableCatalog_text(hxc_locale, 13);
      int32_t hxc_tmp_load_result_n156 = hxc_centerX;
      int32_t hxc_tmp_load_result_n157 = hxc_centerY;
      DrawText(hxc_tmp_call_result_n155, (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n156, 110), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n157, 74), (int32_t)18, hxc_text);
    }
    else
    {
      if (hxc_guide.hxc_phase == hxc_caxecraft_gameplay_GuidePhase_Welcomed)
      {
        const char *hxc_tmp_call_result_n162 = hxc_caxecraft_localization_FirstPlayableCatalog_text(hxc_locale, 14);
        int32_t hxc_tmp_load_result_n163 = hxc_centerX;
        int32_t hxc_tmp_load_result_n164 = hxc_centerY;
        DrawText(hxc_tmp_call_result_n162, (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n163, 225), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n164, 74), (int32_t)16, hxc_text);
      }
      else
      {
        const char *hxc_tmp_call_result_n167 = hxc_caxecraft_localization_FirstPlayableCatalog_text(hxc_locale, 12);
        int32_t hxc_tmp_load_result_n168 = hxc_centerX;
        int32_t hxc_tmp_load_result_n169 = hxc_centerY;
        DrawText(hxc_tmp_call_result_n167, (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n168, 205), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n169, 74), (int32_t)16, hxc_text);
      }
    }
  }
  if (hxc_mossling.hxc_health > 0)
  {
    if (hxc_mossling.hxc_mode == hxc_caxecraft_gameplay_MosslingMode_Windup)
    {
      struct Color hxc_this1_h45ddb0fd488c = (struct Color){ .r = 218, .g = 65, .b = 72, .a = 255 };
      const char *hxc_tmp_call_result_n176 = hxc_caxecraft_localization_FirstPlayableCatalog_text(hxc_locale, 11);
      int32_t hxc_tmp_load_result_n177 = hxc_width;
      DrawText(hxc_tmp_call_result_n176, (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n177, 300), (int32_t)28, (int32_t)16, hxc_this1_h45ddb0fd488c);
    }
    else
    {
      if (hxc_mossling.hxc_mode == hxc_caxecraft_gameplay_MosslingMode_Chasing)
      {
        struct Color hxc_this1_h1fb073e2795c = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
        const char *hxc_tmp_call_result_n183 = hxc_caxecraft_localization_FirstPlayableCatalog_text(hxc_locale, 9);
        int32_t hxc_tmp_load_result_n184 = hxc_width;
        DrawText(hxc_tmp_call_result_n183, (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n184, 180), (int32_t)28, (int32_t)16, hxc_this1_h1fb073e2795c);
      }
    }
  }
  if (hxc_strikeHit)
  {
    struct Color hxc_this1_h5b34c5e04df2 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
    const char *hxc_tmp_call_result_n189 = hxc_caxecraft_localization_FirstPlayableCatalog_text(hxc_locale, 7);
    int32_t hxc_tmp_load_result_n190 = hxc_centerX;
    int32_t hxc_tmp_load_result_n191 = hxc_centerY;
    DrawText(hxc_tmp_call_result_n189, (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n190, 70), (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n191, 54), (int32_t)18, hxc_this1_h5b34c5e04df2);
  }
  if (hxc_enemyDefeated)
  {
    struct Color hxc_this1_h442f51a7657b = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
    const char *hxc_tmp_call_result_n196 = hxc_caxecraft_localization_FirstPlayableCatalog_text(hxc_locale, 10);
    int32_t hxc_tmp_load_result_n197 = hxc_width;
    DrawText(hxc_tmp_call_result_n196, (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n197, 285), (int32_t)54, (int32_t)16, hxc_this1_h442f51a7657b);
  }
  if (hxc_enemyAttacked)
  {
    struct Color hxc_this1_h5be2888bff39 = (struct Color){ .r = 218, .g = 65, .b = 72, .a = 255 };
    const char *hxc_tmp_call_result_n202 = hxc_caxecraft_localization_FirstPlayableCatalog_text(hxc_locale, 18);
    int32_t hxc_tmp_load_result_n203 = hxc_width;
    DrawText(hxc_tmp_call_result_n202, (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n203, 330), (int32_t)82, (int32_t)16, hxc_this1_h5be2888bff39);
  }
  if (hxc_pickedUp)
  {
    int32_t hxc_tmp_load_result_n206 = hxc_pickupAmount;
    int32_t hxc_tmp_conditional_result_n52 = 0;
    if (hxc_tmp_load_result_n206 == 1)
    {
      hxc_tmp_conditional_result_n52 = 2;
    }
    else
    {
      hxc_tmp_conditional_result_n52 = 3;
    }
    int32_t hxc_pickupMessage = hxc_tmp_conditional_result_n52;
    struct Color hxc_this1_hee6f5e2ca7a7 = (struct Color){ .r = 174, .g = 78, .b = 136, .a = 255 };
    int32_t hxc_tmp_load_result_n209 = hxc_locale;
    const char *hxc_tmp_call_result_n211 = hxc_caxecraft_localization_FirstPlayableCatalog_text(hxc_tmp_load_result_n209, hxc_pickupMessage);
    int32_t hxc_tmp_load_result_n212 = hxc_centerX;
    int32_t hxc_tmp_load_result_n213 = hxc_centerY;
    DrawText(hxc_tmp_call_result_n211, (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n212, 48), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n213, 24), (int32_t)18, hxc_this1_hee6f5e2ca7a7);
  }
  if (hxc_inventoryFullReason == 1)
  {
    struct Color hxc_this1_h54ff2681a352 = (struct Color){ .r = 238, .g = 113, .b = 78, .a = 255 };
    const char *hxc_tmp_call_result_n218 = hxc_caxecraft_localization_FirstPlayableCatalog_text(hxc_locale, 5);
    int32_t hxc_tmp_load_result_n219 = hxc_centerX;
    int32_t hxc_tmp_load_result_n220 = hxc_centerY;
    DrawText(hxc_tmp_call_result_n218, (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n219, 150), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n220, 48), (int32_t)16, hxc_this1_h54ff2681a352);
  }
  else
  {
    if (hxc_inventoryFullReason == 2)
    {
      struct Color hxc_this1_h04ed9900f50d = (struct Color){ .r = 238, .g = 113, .b = 78, .a = 255 };
      const char *hxc_tmp_call_result_n225 = hxc_caxecraft_localization_FirstPlayableCatalog_text(hxc_locale, 6);
      int32_t hxc_tmp_load_result_n226 = hxc_centerX;
      int32_t hxc_tmp_load_result_n227 = hxc_centerY;
      DrawText(hxc_tmp_call_result_n225, (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n226, 155), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n227, 48), (int32_t)16, hxc_this1_h04ed9900f50d);
    }
  }
  if (hxc_recoveryVisible)
  {
    if (hxc_recoveryFeedback == 1)
    {
      struct Color hxc_this1_hdcd82a72ef14 = (struct Color){ .r = 94, .g = 212, .b = 136, .a = 255 };
      const char *hxc_tmp_call_result_n233 = hxc_caxecraft_localization_FirstPlayableCatalog_text(hxc_locale, 4);
      int32_t hxc_tmp_load_result_n234 = hxc_centerX;
      int32_t hxc_tmp_load_result_n235 = hxc_centerY;
      DrawText(hxc_tmp_call_result_n233, (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n234, 88), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n235, 24), (int32_t)18, hxc_this1_hdcd82a72ef14);
    }
    else
    {
      if (hxc_recoveryFeedback == 2)
      {
        struct Color hxc_this1_h14181b01f12e = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
        const char *hxc_tmp_call_result_n240 = hxc_caxecraft_localization_UiCatalog_text(hxc_locale, 9);
        int32_t hxc_tmp_load_result_n241 = hxc_centerX;
        int32_t hxc_tmp_load_result_n242 = hxc_centerY;
        DrawText(hxc_tmp_call_result_n240, (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n241, 96), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n242, 24), (int32_t)18, hxc_this1_h14181b01f12e);
      }
      else
      {
        if (hxc_recoveryFeedback == 3)
        {
          struct Color hxc_this1_h1d32c1958a8c = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
          const char *hxc_tmp_call_result_n247 = hxc_caxecraft_localization_FirstPlayableCatalog_text(hxc_locale, 15);
          int32_t hxc_tmp_load_result_n248 = hxc_centerX;
          int32_t hxc_tmp_load_result_n249 = hxc_centerY;
          DrawText(hxc_tmp_call_result_n247, (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n248, 76), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n249, 24), (int32_t)18, hxc_this1_h1d32c1958a8c);
        }
      }
    }
  }
  if (hxc_aquaticEquipmentVisible)
  {
    struct Color hxc_this1_hb03161b5aa7a = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
    const char *hxc_tmp_call_result_n254 = hxc_caxecraft_localization_UiCatalog_text(hxc_locale, 0);
    int32_t hxc_tmp_load_result_n255 = hxc_centerX;
    int32_t hxc_tmp_load_result_n256 = hxc_centerY;
    DrawText(hxc_tmp_call_result_n254, (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n255, 128), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n256, 24), (int32_t)18, hxc_this1_hb03161b5aa7a);
  }
  if (hxc_vitals.hxc_safeTicks > 15)
  {
    struct Color hxc_this1_hc160e30f6087 = (struct Color){ .r = 218, .g = 65, .b = 72, .a = 255 };
    int32_t hxc_tmp_load_result_n260 = hxc_width;
    int32_t hxc_tmp_load_result_n261 = hxc_height;
    DrawRectangleLines((int32_t)4, (int32_t)4, (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n260, 8), (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n261, 8), hxc_this1_hc160e30f6087);
  }
  if (hxc_vitals.hxc_health <= 0)
  {
    struct Color hxc_this1_h0967470897cf = (struct Color){ .r = 15, .g = 27, .b = 39, .a = 220 };
    int32_t hxc_tmp_load_result_n265 = hxc_centerX;
    int32_t hxc_tmp_load_result_n266 = hxc_centerY;
    DrawRectangle((int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n265, 250), (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n266, 74), (int32_t)500, (int32_t)148, hxc_this1_h0967470897cf);
    struct Color hxc_this1_hbf56e217d4da = (struct Color){ .r = 218, .g = 65, .b = 72, .a = 255 };
    int32_t hxc_tmp_load_result_n269 = hxc_centerX;
    int32_t hxc_tmp_load_result_n270 = hxc_centerY;
    DrawRectangleLines((int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n269, 250), (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n270, 74), (int32_t)500, (int32_t)148, hxc_this1_hbf56e217d4da);
    const char *hxc_tmp_call_result_n273 = hxc_caxecraft_localization_FirstPlayableCatalog_text(hxc_locale, 8);
    int32_t hxc_tmp_load_result_n274 = hxc_centerX;
    int32_t hxc_tmp_load_result_n275 = hxc_centerY;
    DrawText(hxc_tmp_call_result_n273, (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n274, 122), (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n275, 42), (int32_t)24, hxc_text);
    struct Color hxc_this1_ha1fed9d3ab4a = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
    const char *hxc_tmp_call_result_n279 = hxc_caxecraft_localization_FirstPlayableCatalog_text(hxc_locale, 16);
    int32_t hxc_tmp_load_result_n280 = hxc_centerX;
    int32_t hxc_tmp_load_result_n281 = hxc_centerY;
    DrawText(hxc_tmp_call_result_n279, (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n280, 125), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n281, 10), (int32_t)18, hxc_this1_ha1fed9d3ab4a);
  }
  if (hxc_paused)
  {
    struct Color hxc_this1_hf93f42c8fc63 = (struct Color){ .r = 15, .g = 27, .b = 39, .a = 220 };
    int32_t hxc_tmp_load_result_n285 = hxc_centerX;
    int32_t hxc_tmp_load_result_n286 = hxc_centerY;
    DrawRectangle((int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n285, 170), (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n286, 48), (int32_t)340, (int32_t)96, hxc_this1_hf93f42c8fc63);
    struct Color hxc_this1_h2da40acfc183 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
    int32_t hxc_tmp_load_result_n289 = hxc_centerX;
    int32_t hxc_tmp_load_result_n290 = hxc_centerY;
    DrawRectangleLines((int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n289, 170), (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n290, 48), (int32_t)340, (int32_t)96, hxc_this1_h2da40acfc183);
    const char *hxc_tmp_call_result_n293 = hxc_caxecraft_localization_UiCatalog_text(hxc_locale, 15);
    int32_t hxc_tmp_load_result_n294 = hxc_centerX;
    int32_t hxc_tmp_load_result_n295 = hxc_centerY;
    DrawText(hxc_tmp_call_result_n293, (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n294, 48), (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n295, 30), (int32_t)24, hxc_text);
    const char *hxc_tmp_call_result_n298 = hxc_caxecraft_localization_UiCatalog_text(hxc_locale, 14);
    int32_t hxc_tmp_load_result_n299 = hxc_centerX;
    int32_t hxc_tmp_load_result_n300 = hxc_centerY;
    DrawText(hxc_tmp_call_result_n298, (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n299, 160), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n300, 8), (int32_t)16, hxc_text);
  }
  else
  {
    if (hxc_placementBlocked)
    {
      struct Color hxc_this1_hd3e7783e1e00 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
      const char *hxc_tmp_call_result_n305 = hxc_caxecraft_localization_UiCatalog_text(hxc_locale, 16);
      int32_t hxc_tmp_load_result_n306 = hxc_centerX;
      int32_t hxc_tmp_load_result_n307 = hxc_centerY;
      DrawText(hxc_tmp_call_result_n305, (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n306, 170), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n307, 26), (int32_t)14, hxc_this1_hd3e7783e1e00);
    }
    else
    {
      if (!hxc_captured)
      {
        const char *hxc_tmp_call_result_n311 = hxc_caxecraft_localization_UiCatalog_text(hxc_locale, 2);
        int32_t hxc_tmp_load_result_n312 = hxc_centerX;
        int32_t hxc_tmp_load_result_n313 = hxc_centerY;
        DrawText(hxc_tmp_call_result_n311, (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n312, 90), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n313, 26), (int32_t)14, hxc_text);
      }
      else
      {
        if (!hxc_hit.hxc_hit)
        {
          const char *hxc_tmp_call_result_n317 = hxc_caxecraft_localization_UiCatalog_text(hxc_locale, 13);
          int32_t hxc_tmp_load_result_n318 = hxc_centerX;
          int32_t hxc_tmp_load_result_n319 = hxc_centerY;
          DrawText(hxc_tmp_call_result_n317, (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n318, 105), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n319, 26), (int32_t)14, hxc_text);
        }
      }
    }
  }
  return;
}

struct hxc_caxecraft_domain_CharacterBody hxc_caxecraft_app_CaxecraftApp_spawnPlayer(uint8_t *hxc_cells, size_t hxc_length)
{
  uint8_t *hxc_borrow = hxc_cells;
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
  if (hxc_self == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_domain_GameSession *hxc_tmp_owned_class_field_address_n0 = &(*hxc_self).hxc_session;
  struct hxc_caxecraft_content_FirstPlayableSessionLoad hxc_tmp_call_result_n1 = hxc_caxecraft_content_FirstPlayableSessionLoader_loadCandidate(hxc_tmp_owned_class_field_address_n0);
  struct hxc_caxecraft_content_FirstPlayableSessionLoad hxc_loadedLevel = hxc_tmp_call_result_n1;
  if (!hxc_loadedLevel.hxc_valid)
  {
    return;
  }
  int32_t hxc_waterPresentationCell = hxc_loadedLevel.hxc_waterPresentationCell;
  struct hxc_caxecraft_domain_GameSession *hxc_tmp_owned_class_field_address_n4 = &(*hxc_self).hxc_session;
  uint8_t *hxc_cells = (*hxc_tmp_owned_class_field_address_n4).hxc_worldStorage;
  size_t hxc_tmp_length_n3 = 16384;
  struct hxc_caxecraft_domain_GameSession *hxc_tmp_owned_class_field_address_n5 = &(*hxc_self).hxc_session;
  uint8_t *hxc_pendingCells = (*hxc_tmp_owned_class_field_address_n5).hxc_pendingWaterStorage;
  size_t hxc_tmp_length_n4 = 16384;
  struct hxc_caxecraft_domain_GameSession *hxc_tmp_owned_class_field_address_n6 = &(*hxc_self).hxc_session;
  int32_t *hxc_itemActive = (*hxc_tmp_owned_class_field_address_n6).hxc_authoredItemStorage;
  size_t hxc_tmp_length_n5 = 256;
  struct hxc_caxecraft_domain_GameSession *hxc_tmp_owned_class_field_address_n7 = &(*hxc_self).hxc_session;
  const int32_t *hxc_itemActiveRead = (*hxc_tmp_owned_class_field_address_n7).hxc_authoredItemStorage;
  size_t hxc_tmp_length_n6 = 256;
  int32_t hxc_inactiveItem = 0;
  struct hxc_caxecraft_domain_AquaticProfile hxc_tmp_call_result_n8 = hxc_caxecraft_content_BaseContentPack_aquaticProfile(0);
  struct hxc_caxecraft_domain_AquaticProfile hxc_initialAquaticProfile = hxc_tmp_call_result_n8;
  int32_t hxc_initialHealth = 6;
  struct hxc_caxecraft_domain_GameSession *hxc_tmp_owned_class_field_address_n9 = &(*hxc_self).hxc_session;
  if (hxc_tmp_owned_class_field_address_n9 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_domain_CharacterBody hxc_tmp_call_result_n10 = hxc_caxecraft_app_CaxecraftApp_spawnPlayer(hxc_cells, hxc_tmp_length_n3);
  struct hxc_caxecraft_domain_AquaticProfile hxc_tmp_load_result_n11 = hxc_initialAquaticProfile;
  struct hxc_caxecraft_domain_Character hxc_tmp_call_result_n13 = hxc_caxecraft_domain_Character_start(1, hxc_tmp_call_result_n10, hxc_tmp_load_result_n11, hxc_initialHealth);
  bool hxc_tmp_instance_call_result_n14 = hxc_caxecraft_domain_GameSession_bindLocalPlayer(hxc_tmp_owned_class_field_address_n9, hxc_tmp_call_result_n13);
  if (!hxc_tmp_instance_call_result_n14)
  {
    return;
  }
  int32_t hxc_windowFlags = hxc_i32_bit_or(64, 4);
  hxc_windowFlags = hxc_i32_bit_or(hxc_windowFlags, 8192);
  SetConfigFlags((uint32_t)hxc_windowFlags);
  InitWindow((int32_t)1280, (int32_t)720, "Caxecraft \342\200\224 Haxe shaped into careful C");
  bool hxc_tmp_native_call_result_n17 = IsWindowReady();
  if (!hxc_tmp_native_call_result_n17)
  {
    return;
  }
  SetWindowMinSize((int32_t)800, (int32_t)450);
  SetExitKey((int32_t)0);
  SetTargetFPS((int32_t)120);
  EnableCursor();
  struct Texture hxc_tmp_native_call_result_n18 = LoadTexture("assets/showcase/title-panorama.png");
  struct Texture hxc_titleTexture = hxc_tmp_native_call_result_n18;
  bool hxc_tmp_native_call_result_n20 = IsTextureValid(hxc_titleTexture);
  bool hxc_titleTextureReady = hxc_tmp_native_call_result_n20;
  struct Texture hxc_tmp_native_call_result_n21 = LoadTexture("assets/branding/caxecraft-wordmark.png");
  struct Texture hxc_wordmarkTexture = hxc_tmp_native_call_result_n21;
  bool hxc_tmp_native_call_result_n23 = IsTextureValid(hxc_wordmarkTexture);
  bool hxc_wordmarkTextureReady = hxc_tmp_native_call_result_n23;
  struct Texture hxc_tmp_native_call_result_n24 = LoadTexture("assets/atlases/hud.png");
  struct Texture hxc_hudTexture = hxc_tmp_native_call_result_n24;
  bool hxc_tmp_native_call_result_n26 = IsTextureValid(hxc_hudTexture);
  bool hxc_hudTextureReady = hxc_tmp_native_call_result_n26;
  struct Texture hxc_tmp_native_call_result_n27 = LoadTexture("assets/atlases/items.png");
  struct Texture hxc_itemTexture = hxc_tmp_native_call_result_n27;
  bool hxc_tmp_native_call_result_n29 = IsTextureValid(hxc_itemTexture);
  bool hxc_itemTextureReady = hxc_tmp_native_call_result_n29;
  struct Texture hxc_tmp_native_call_result_n30 = LoadTexture("assets/atlases/adventure-items.png");
  struct Texture hxc_adventureItemTexture = hxc_tmp_native_call_result_n30;
  bool hxc_tmp_native_call_result_n32 = IsTextureValid(hxc_adventureItemTexture);
  bool hxc_adventureItemTextureReady = hxc_tmp_native_call_result_n32;
  struct Texture hxc_tmp_load_result_n33 = hxc_hudTexture;
  bool hxc_tmp_load_result_n34 = hxc_hudTextureReady;
  struct Texture hxc_tmp_load_result_n35 = hxc_itemTexture;
  bool hxc_tmp_load_result_n36 = hxc_itemTextureReady;
  struct Texture hxc_tmp_load_result_n37 = hxc_adventureItemTexture;
  struct hxc_caxecraft_app_HudResources hxc_hudResources = (struct hxc_caxecraft_app_HudResources){ .hxc_adventureItemTexture = hxc_tmp_load_result_n37, .hxc_adventureItemTextureReady = hxc_adventureItemTextureReady, .hxc_hudTexture = hxc_tmp_load_result_n33, .hxc_hudTextureReady = hxc_tmp_load_result_n34, .hxc_itemTexture = hxc_tmp_load_result_n35, .hxc_itemTextureReady = hxc_tmp_load_result_n36 };
  struct Texture hxc_tmp_native_call_result_n40 = LoadTexture("assets/atlases/entities.png");
  struct Texture hxc_entityTexture = hxc_tmp_native_call_result_n40;
  bool hxc_tmp_native_call_result_n42 = IsTextureValid(hxc_entityTexture);
  bool hxc_entityTextureReady = hxc_tmp_native_call_result_n42;
  struct Texture hxc_tmp_native_call_result_n43 = LoadTexture("assets/atlases/terrain.png");
  struct Texture hxc_terrainTexture = hxc_tmp_native_call_result_n43;
  bool hxc_tmp_native_call_result_n45 = IsTextureValid(hxc_terrainTexture);
  bool hxc_terrainTextureReady = hxc_tmp_native_call_result_n45;
  struct Texture hxc_tmp_native_call_result_n46 = LoadTexture("assets/atlases/adventure-terrain.png");
  struct Texture hxc_adventureTerrainTexture = hxc_tmp_native_call_result_n46;
  bool hxc_tmp_native_call_result_n48 = IsTextureValid(hxc_adventureTerrainTexture);
  bool hxc_adventureTerrainTextureReady = hxc_tmp_native_call_result_n48;
  double hxc_cameraWaterBlend = 0.0;
  struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_call_result_n49 = hxc_caxecraft_gameplay_Inventory_starter();
  struct hxc_caxecraft_gameplay_InventoryState hxc_inventory = hxc_tmp_call_result_n49;
  struct hxc_caxecraft_gameplay_GuideState hxc_tmp_call_result_n50 = hxc_caxecraft_gameplay_GuideNpc_start(hxc_cells, hxc_tmp_length_n3, 17.5, 13.5);
  struct hxc_caxecraft_gameplay_GuideState hxc_guide = hxc_tmp_call_result_n50;
  struct hxc_caxecraft_gameplay_MosslingState hxc_tmp_call_result_n51 = hxc_caxecraft_gameplay_Mossling_start(hxc_cells, hxc_tmp_length_n3, 15.5, 13.8);
  struct hxc_caxecraft_gameplay_MosslingState hxc_mossling = hxc_tmp_call_result_n51;
  struct hxc_caxecraft_gameplay_SwordCombatState hxc_swordCombat = (struct hxc_caxecraft_gameplay_SwordCombatState){ .hxc_cooldownTicks = 0 };
  struct hxc_caxecraft_gameplay_BerryDropState hxc_berryDrop = (struct hxc_caxecraft_gameplay_BerryDropState){ .hxc_active = false, .hxc_amount = 0, .hxc_x = 0.0, .hxc_y = 0.0, .hxc_z = 0.0 };
  double hxc_lookX = 0.0;
  double hxc_lookY = -0.18;
  double hxc_lookZ = -1.0;
  double hxc_accumulator = 0.0;
  struct hxc_caxecraft_domain_GameSession *hxc_tmp_owned_class_field_address_n54 = &(*hxc_self).hxc_session;
  if (hxc_tmp_owned_class_field_address_n54 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_domain_GameView hxc_tmp_instance_call_result_n55 = hxc_caxecraft_domain_GameSession_view(hxc_tmp_owned_class_field_address_n54);
  struct hxc_caxecraft_domain_GameView hxc_initialPresentation = hxc_tmp_instance_call_result_n55;
  if (!hxc_initialPresentation.hxc_valid)
  {
    return;
  }
  struct hxc_caxecraft_app_MotionHistory hxc_tmp_call_result_n58 = hxc_caxecraft_app_MotionInterpolation_start(hxc_initialPresentation.hxc_localPlayer.hxc_body);
  struct hxc_caxecraft_app_MotionHistory hxc_motionHistory = hxc_tmp_call_result_n58;
  bool hxc_jumpQueued = false;
  bool hxc_swordQueued = false;
  int32_t hxc_selectedMode = 0;
  int32_t hxc_locale = 0;
  bool hxc_showInitialTitle = true;
  enum hxc_caxecraft_app_AppScreen hxc_tmp_call_result_n60 = hxc_caxecraft_app_AppScreen_initialScreen(hxc_showInitialTitle);
  enum hxc_caxecraft_app_AppScreen hxc_screen = hxc_tmp_call_result_n60;
  bool hxc_tmp_call_result_n62 = hxc_caxecraft_app_AppScreen_capturesPointer(hxc_screen);
  if (hxc_tmp_call_result_n62)
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
    bool hxc_tmp_load_result_n63 = hxc_quit;
    bool hxc_tmp_short_circuit_result_n60 = !hxc_tmp_load_result_n63;
    if (!hxc_tmp_load_result_n63)
    {
      bool hxc_tmp_native_call_result_n64 = WindowShouldClose();
      hxc_tmp_short_circuit_result_n60 = !hxc_tmp_native_call_result_n64;
    }
    if (!hxc_tmp_short_circuit_result_n60)
    {
      break;
    }
    struct hxc_caxecraft_domain_GameSession *hxc_tmp_owned_class_field_address_n66 = &(*hxc_self).hxc_session;
    if (hxc_tmp_owned_class_field_address_n66 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_domain_GameView hxc_tmp_instance_call_result_n67 = hxc_caxecraft_domain_GameSession_view(hxc_tmp_owned_class_field_address_n66);
    struct hxc_caxecraft_domain_GameView hxc_initialView = hxc_tmp_instance_call_result_n67;
    if (!hxc_initialView.hxc_valid)
    {
      hxc_quit = true;
    }
    struct hxc_caxecraft_domain_Character hxc_character = hxc_initialView.hxc_localPlayer;
    bool hxc_recapturedThisFrame = false;
    bool hxc_resetMotionThisFrame = false;
    bool hxc_tmp_native_call_result_n70 = IsWindowFocused();
    bool hxc_focused = hxc_tmp_native_call_result_n70;
    bool hxc_tmp_call_result_n72 = hxc_caxecraft_app_AppScreen_capturesPointer(hxc_screen);
    bool hxc_captured_h309593e75e34 = hxc_tmp_call_result_n72;
    bool hxc_tmp_call_result_n74 = hxc_caxecraft_app_AppScreen_pausesSimulation(hxc_screen);
    bool hxc_paused_h41a7122e937d = hxc_tmp_call_result_n74;
    double hxc_forward = 0.0;
    double hxc_right = 0.0;
    bool hxc_tmp_native_call_result_n75 = IsKeyDown((int32_t)87);
    if (hxc_tmp_native_call_result_n75)
    {
      hxc_forward = hxc_forward + 1.0;
    }
    bool hxc_tmp_native_call_result_n77 = IsKeyDown((int32_t)83);
    if (hxc_tmp_native_call_result_n77)
    {
      hxc_forward = hxc_forward - 1.0;
    }
    bool hxc_tmp_native_call_result_n79 = IsKeyDown((int32_t)68);
    if (hxc_tmp_native_call_result_n79)
    {
      hxc_right = hxc_right + 1.0;
    }
    bool hxc_tmp_native_call_result_n81 = IsKeyDown((int32_t)65);
    if (hxc_tmp_native_call_result_n81)
    {
      hxc_right = hxc_right - 1.0;
    }
    double hxc_lookYaw_he85cc0e17018 = 0.0;
    double hxc_lookPitch_he10d27d3c893 = 0.0;
    if (hxc_captured_h309593e75e34)
    {
      struct Vector2 hxc_tmp_native_call_result_n84 = GetMouseDelta();
      struct Vector2 hxc_mouse = hxc_tmp_native_call_result_n84;
      hxc_lookYaw_he85cc0e17018 = -(double)hxc_mouse.x * 0.0025;
      hxc_lookPitch_he10d27d3c893 = -(double)hxc_mouse.y * 0.0025;
    }
    bool hxc_tmp_native_call_result_n89 = IsMouseButtonPressed((int32_t)0);
    bool hxc_leftPressed = hxc_tmp_native_call_result_n89;
    bool hxc_tmp_load_result_n90 = hxc_captured_h309593e75e34;
    bool hxc_tmp_short_circuit_result_n75 = hxc_tmp_load_result_n90;
    if (hxc_tmp_load_result_n90)
    {
      hxc_tmp_short_circuit_result_n75 = hxc_leftPressed;
    }
    bool hxc_primaryPressed_h8fd652c9c2fd = hxc_tmp_short_circuit_result_n75;
    bool hxc_tmp_load_result_n93 = hxc_captured_h309593e75e34;
    bool hxc_tmp_short_circuit_result_n77 = hxc_tmp_load_result_n93;
    if (hxc_tmp_load_result_n93)
    {
      bool hxc_tmp_native_call_result_n94 = IsMouseButtonPressed((int32_t)1);
      hxc_tmp_short_circuit_result_n77 = hxc_tmp_native_call_result_n94;
    }
    bool hxc_secondaryPressed_h8f0313c610db = hxc_tmp_short_circuit_result_n77;
    bool hxc_tmp_load_result_n96 = hxc_captured_h309593e75e34;
    bool hxc_tmp_short_circuit_result_n79 = hxc_tmp_load_result_n96;
    if (hxc_tmp_load_result_n96)
    {
      bool hxc_tmp_native_call_result_n97 = IsKeyPressed((int32_t)69);
      hxc_tmp_short_circuit_result_n79 = hxc_tmp_native_call_result_n97;
    }
    bool hxc_interactPressed_h33f705f7cf13 = hxc_tmp_short_circuit_result_n79;
    int32_t hxc_hotbarSelection_hd2dcc2ac0475 = -1;
    bool hxc_tmp_native_call_result_n99 = IsKeyPressed((int32_t)49);
    if (hxc_tmp_native_call_result_n99)
    {
      hxc_hotbarSelection_hd2dcc2ac0475 = 0;
    }
    bool hxc_tmp_native_call_result_n100 = IsKeyPressed((int32_t)50);
    if (hxc_tmp_native_call_result_n100)
    {
      hxc_hotbarSelection_hd2dcc2ac0475 = 1;
    }
    bool hxc_tmp_native_call_result_n101 = IsKeyPressed((int32_t)51);
    if (hxc_tmp_native_call_result_n101)
    {
      hxc_hotbarSelection_hd2dcc2ac0475 = 2;
    }
    bool hxc_tmp_native_call_result_n102 = IsKeyPressed((int32_t)52);
    if (hxc_tmp_native_call_result_n102)
    {
      hxc_hotbarSelection_hd2dcc2ac0475 = 3;
    }
    bool hxc_tmp_native_call_result_n103 = IsKeyPressed((int32_t)53);
    if (hxc_tmp_native_call_result_n103)
    {
      hxc_hotbarSelection_hd2dcc2ac0475 = 4;
    }
    bool hxc_tmp_native_call_result_n104 = IsKeyPressed((int32_t)54);
    if (hxc_tmp_native_call_result_n104)
    {
      hxc_hotbarSelection_hd2dcc2ac0475 = 5;
    }
    bool hxc_tmp_native_call_result_n105 = IsKeyPressed((int32_t)55);
    if (hxc_tmp_native_call_result_n105)
    {
      hxc_hotbarSelection_hd2dcc2ac0475 = 6;
    }
    bool hxc_tmp_native_call_result_n106 = IsKeyPressed((int32_t)56);
    if (hxc_tmp_native_call_result_n106)
    {
      hxc_hotbarSelection_hd2dcc2ac0475 = 7;
    }
    float hxc_tmp_native_call_result_n107 = GetMouseWheelMove();
    double hxc_wheel = (double)hxc_tmp_native_call_result_n107;
    int32_t hxc_hotbarCycle_hdabdb68c8c6e = 0;
    if (hxc_wheel > 0.0)
    {
      hxc_hotbarCycle_hdabdb68c8c6e = -1;
    }
    if (hxc_wheel < 0.0)
    {
      hxc_hotbarCycle_hdabdb68c8c6e = 1;
    }
    bool hxc_tmp_native_call_result_n110 = IsKeyPressed((int32_t)32);
    bool hxc_jumpPressed_hca362cf58bff = hxc_tmp_native_call_result_n110;
    bool hxc_tmp_native_call_result_n111 = IsKeyPressed((int32_t)256);
    bool hxc_pausePressed_ha052fab26c64 = hxc_tmp_native_call_result_n111;
    bool hxc_tmp_native_call_result_n112 = IsKeyPressed((int32_t)81);
    bool hxc_quitPressed_h144039f97734 = hxc_tmp_native_call_result_n112;
    double hxc_frameInput_moveForward = hxc_forward;
    double hxc_frameInput_moveRight = hxc_right;
    double hxc_frameInput_lookYaw = hxc_lookYaw_he85cc0e17018;
    double hxc_frameInput_lookPitch = hxc_lookPitch_he10d27d3c893;
    bool hxc_frameInput_jumpPressed = hxc_jumpPressed_hca362cf58bff;
    bool hxc_tmp_native_call_result_n118 = IsKeyDown((int32_t)340);
    bool hxc_frameInput_descendHeld = hxc_tmp_native_call_result_n118;
    bool hxc_frameInput_primaryPressed = hxc_primaryPressed_h8fd652c9c2fd;
    bool hxc_frameInput_secondaryPressed = hxc_secondaryPressed_h8f0313c610db;
    bool hxc_frameInput_interactPressed = hxc_interactPressed_h33f705f7cf13;
    bool hxc_frameInput_pausePressed = hxc_pausePressed_ha052fab26c64;
    bool hxc_tmp_load_result_n123 = hxc_paused_h41a7122e937d;
    bool hxc_tmp_short_circuit_result_n97 = hxc_tmp_load_result_n123;
    if (hxc_tmp_load_result_n123)
    {
      hxc_tmp_short_circuit_result_n97 = hxc_leftPressed;
    }
    bool hxc_frameInput_capturePressed = hxc_tmp_short_circuit_result_n97;
    bool hxc_frameInput_quitPressed = hxc_quitPressed_h144039f97734;
    int32_t hxc_frameInput_hotbarSelection = hxc_hotbarSelection_hd2dcc2ac0475;
    int32_t hxc_frameInput_hotbarCycle = hxc_hotbarCycle_hdabdb68c8c6e;
    double hxc_moveForward = hxc_frameInput_moveForward;
    double hxc_moveRight = hxc_frameInput_moveRight;
    double hxc_lookYaw_h6f5199981cd0 = hxc_frameInput_lookYaw;
    double hxc_lookPitch_h5b3b3688df1e = hxc_frameInput_lookPitch;
    bool hxc_jumpPressed_h24b772e3821d = hxc_frameInput_jumpPressed;
    bool hxc_primaryPressed_h54721f68a899 = hxc_frameInput_primaryPressed;
    bool hxc_secondaryPressed_hba76f40dab68 = hxc_frameInput_secondaryPressed;
    bool hxc_interactPressed_h26f6ef5a6ec0 = hxc_frameInput_interactPressed;
    bool hxc_pausePressed_h733d59438383 = hxc_frameInput_pausePressed;
    bool hxc_capturePressed = hxc_frameInput_capturePressed;
    bool hxc_quitPressed_h1f828a8c904d = hxc_frameInput_quitPressed;
    int32_t hxc_hotbarSelection_he3829c9a12d6 = hxc_frameInput_hotbarSelection;
    int32_t hxc_hotbarCycle_hf52ad46c6e59 = hxc_frameInput_hotbarCycle;
    bool hxc_descendHeld = hxc_frameInput_descendHeld;
    if (hxc_quitPressed_h1f828a8c904d)
    {
      hxc_quit = true;
    }
    if (hxc_hotbarSelection_he3829c9a12d6 >= 0)
    {
      struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_load_result_n145 = hxc_inventory;
      struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_call_result_n147 = hxc_caxecraft_gameplay_Inventory_select(hxc_tmp_load_result_n145, hxc_hotbarSelection_he3829c9a12d6);
      hxc_inventory = hxc_tmp_call_result_n147;
    }
    if (hxc_hotbarCycle_hf52ad46c6e59 != 0)
    {
      struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_load_result_n149 = hxc_inventory;
      struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_call_result_n151 = hxc_caxecraft_gameplay_Inventory_cycle(hxc_tmp_load_result_n149, hxc_hotbarCycle_hf52ad46c6e59);
      hxc_inventory = hxc_tmp_call_result_n151;
    }
    bool hxc_tmp_call_result_n153 = hxc_caxecraft_app_AppScreen_isPlaying(hxc_screen);
    bool hxc_tmp_short_circuit_result_n115 = hxc_tmp_call_result_n153;
    if (hxc_tmp_call_result_n153)
    {
      hxc_tmp_short_circuit_result_n115 = hxc_interactPressed_h26f6ef5a6ec0;
    }
    if (hxc_tmp_short_circuit_result_n115)
    {
      if (hxc_character.hxc_vitals.hxc_health <= 0)
      {
        struct hxc_caxecraft_domain_Character hxc_tmp_load_result_n157 = hxc_character;
        struct hxc_caxecraft_domain_CharacterBody hxc_tmp_call_result_n158 = hxc_caxecraft_app_CaxecraftApp_spawnPlayer(hxc_cells, hxc_tmp_length_n3);
        struct hxc_caxecraft_domain_Character hxc_tmp_call_result_n159 = hxc_caxecraft_domain_Character_reviveAt(hxc_tmp_load_result_n157, hxc_tmp_call_result_n158);
        hxc_character = hxc_tmp_call_result_n159;
        hxc_cameraWaterBlend = 0.0;
        hxc_accumulator = 0.0;
        hxc_resetMotionThisFrame = true;
      }
      else
      {
        struct hxc_caxecraft_gameplay_GuideState hxc_tmp_load_result_n160 = hxc_guide;
        double hxc_tmp_record_field_load_result_n161 = hxc_character.hxc_body.hxc_x;
        bool hxc_tmp_call_result_n163 = hxc_caxecraft_gameplay_GuideNpc_isInRange(hxc_tmp_load_result_n160, hxc_tmp_record_field_load_result_n161, hxc_character.hxc_body.hxc_z);
        if (hxc_tmp_call_result_n163)
        {
          bool hxc_tmp_call_result_n165 = hxc_caxecraft_gameplay_GuideNpc_sharesBerriesOnNextInteraction(hxc_guide);
          bool hxc_sharesBerries = hxc_tmp_call_result_n165;
          if (hxc_sharesBerries)
          {
            int32_t hxc_tmp_call_result_n169 = hxc_caxecraft_gameplay_Inventory_acceptedAmount(hxc_inventory, hxc_caxecraft_gameplay_ItemKind_Berries, 2);
            int32_t hxc_acceptedGift = hxc_tmp_call_result_n169;
            if (hxc_acceptedGift == 2)
            {
              struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_load_result_n171 = hxc_inventory;
              struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_call_result_n174 = hxc_caxecraft_gameplay_Inventory_collectItem(hxc_tmp_load_result_n171, hxc_caxecraft_gameplay_ItemKind_Berries, hxc_acceptedGift);
              hxc_inventory = hxc_tmp_call_result_n174;
              struct hxc_caxecraft_gameplay_GuideState hxc_tmp_call_result_n176 = hxc_caxecraft_gameplay_GuideNpc_interact(hxc_guide);
              hxc_guide = hxc_tmp_call_result_n176;
            }
            else
            {
              hxc_inventoryFullReason = 1;
              hxc_inventoryFullFrames = 90;
            }
          }
          else
          {
            struct hxc_caxecraft_gameplay_GuideState hxc_tmp_call_result_n178 = hxc_caxecraft_gameplay_GuideNpc_interact(hxc_guide);
            hxc_guide = hxc_tmp_call_result_n178;
          }
        }
      }
    }
    bool hxc_tmp_call_result_n180 = hxc_caxecraft_app_AppScreen_showsTitle(hxc_screen);
    bool hxc_tmp_short_circuit_result_n118 = hxc_tmp_call_result_n180;
    if (hxc_tmp_call_result_n180)
    {
      hxc_tmp_short_circuit_result_n118 = hxc_focused;
    }
    if (hxc_tmp_short_circuit_result_n118)
    {
      int32_t hxc_modeBeforeInput = hxc_selectedMode;
      bool hxc_tmp_native_call_result_n184 = IsKeyPressed((int32_t)76);
      if (hxc_tmp_native_call_result_n184)
      {
        int32_t hxc_tmp_call_result_n186 = hxc_caxecraft_localization_UiCatalog_nextLocale(hxc_locale);
        hxc_locale = hxc_tmp_call_result_n186;
      }
      bool hxc_tmp_native_call_result_n187 = IsKeyPressed((int32_t)265);
      bool hxc_tmp_short_circuit_result_n120 = hxc_tmp_native_call_result_n187;
      if (!hxc_tmp_native_call_result_n187)
      {
        bool hxc_tmp_native_call_result_n188 = IsKeyPressed((int32_t)264);
        hxc_tmp_short_circuit_result_n120 = hxc_tmp_native_call_result_n188;
      }
      if (hxc_tmp_short_circuit_result_n120)
      {
        int32_t hxc_tmp_load_result_n190 = hxc_selectedMode;
        int32_t hxc_tmp_conditional_result_n121 = 0;
        if (hxc_tmp_load_result_n190 == 0)
        {
          hxc_tmp_conditional_result_n121 = 1;
        }
        else
        {
          hxc_tmp_conditional_result_n121 = 0;
        }
        hxc_selectedMode = hxc_tmp_conditional_result_n121;
      }
      struct Vector2 hxc_tmp_native_call_result_n192 = GetMousePosition();
      struct Vector2 hxc_menuMouse = hxc_tmp_native_call_result_n192;
      struct Vector2 hxc_tmp_load_result_n193 = hxc_menuMouse;
      struct Vector2 hxc_tmp_load_result_n195 = hxc_menuMouse;
      int32_t hxc_tmp_native_call_result_n197 = GetScreenWidth();
      int32_t hxc_tmp_native_call_result_n198 = GetScreenHeight();
      int32_t hxc_tmp_call_result_n199 = hxc_caxecraft_app_TitleMenu_selectionAt((double)hxc_tmp_load_result_n193.x, (double)hxc_tmp_load_result_n195.y, (int32_t)hxc_tmp_native_call_result_n197, (int32_t)hxc_tmp_native_call_result_n198);
      int32_t hxc_hovered = hxc_tmp_call_result_n199;
      if (hxc_hovered == 0)
      {
        hxc_selectedMode = 0;
      }
      if (hxc_hovered == 1)
      {
        hxc_selectedMode = 1;
      }
      int32_t hxc_tmp_load_result_n202 = hxc_selectedMode;
      if (hxc_tmp_load_result_n202 != hxc_modeBeforeInput)
      {
        hxc_accumulator = 0.0;
        hxc_resetMotionThisFrame = true;
      }
      int32_t hxc_tmp_load_result_n204 = hxc_hovered;
      bool hxc_tmp_short_circuit_result_n125 = hxc_tmp_load_result_n204 >= 0;
      if (hxc_tmp_load_result_n204 >= 0)
      {
        bool hxc_tmp_native_call_result_n205 = IsMouseButtonPressed((int32_t)0);
        hxc_tmp_short_circuit_result_n125 = hxc_tmp_native_call_result_n205;
      }
      bool hxc_clickedChoice = hxc_tmp_short_circuit_result_n125;
      bool hxc_tmp_load_result_n207 = hxc_clickedChoice;
      bool hxc_tmp_short_circuit_result_n126 = hxc_tmp_load_result_n207;
      if (!hxc_tmp_load_result_n207)
      {
        bool hxc_tmp_native_call_result_n208 = IsKeyPressed((int32_t)257);
        hxc_tmp_short_circuit_result_n126 = hxc_tmp_native_call_result_n208;
      }
      if (hxc_tmp_short_circuit_result_n126)
      {
        enum hxc_caxecraft_app_AppScreen hxc_tmp_call_result_n211 = hxc_caxecraft_app_AppScreen_startPlaying(hxc_screen);
        hxc_screen = hxc_tmp_call_result_n211;
        hxc_recapturedThisFrame = true;
        hxc_accumulator = 0.0;
        hxc_resetMotionThisFrame = true;
        DisableCursor();
      }
    }
    bool hxc_tmp_load_result_n212 = hxc_focused;
    bool hxc_tmp_short_circuit_result_n127 = !hxc_tmp_load_result_n212;
    if (!hxc_tmp_load_result_n212)
    {
      bool hxc_tmp_call_result_n214 = hxc_caxecraft_app_AppScreen_isPlaying(hxc_screen);
      hxc_tmp_short_circuit_result_n127 = hxc_tmp_call_result_n214;
    }
    if (hxc_tmp_short_circuit_result_n127)
    {
      enum hxc_caxecraft_app_AppScreen hxc_tmp_call_result_n217 = hxc_caxecraft_app_AppScreen_loseFocus(hxc_screen);
      hxc_screen = hxc_tmp_call_result_n217;
      hxc_jumpQueued = false;
      hxc_accumulator = 0.0;
      hxc_resetMotionThisFrame = true;
      EnableCursor();
    }
    bool hxc_tmp_call_result_n219 = hxc_caxecraft_app_AppScreen_showsTitle(hxc_screen);
    bool hxc_tmp_short_circuit_result_n128 = !hxc_tmp_call_result_n219;
    if (!hxc_tmp_call_result_n219)
    {
      hxc_tmp_short_circuit_result_n128 = hxc_focused;
    }
    bool hxc_tmp_short_circuit_load_result_n221 = hxc_tmp_short_circuit_result_n128;
    bool hxc_tmp_short_circuit_result_n129 = hxc_tmp_short_circuit_load_result_n221;
    if (hxc_tmp_short_circuit_load_result_n221)
    {
      hxc_tmp_short_circuit_result_n129 = hxc_pausePressed_h733d59438383;
    }
    if (hxc_tmp_short_circuit_result_n129)
    {
      enum hxc_caxecraft_app_AppScreen hxc_tmp_call_result_n225 = hxc_caxecraft_app_AppScreen_togglePause(hxc_screen);
      hxc_screen = hxc_tmp_call_result_n225;
      hxc_accumulator = 0.0;
      hxc_resetMotionThisFrame = true;
      bool hxc_tmp_call_result_n227 = hxc_caxecraft_app_AppScreen_pausesSimulation(hxc_screen);
      if (hxc_tmp_call_result_n227)
      {
        hxc_jumpQueued = false;
      }
      bool hxc_tmp_call_result_n229 = hxc_caxecraft_app_AppScreen_capturesPointer(hxc_screen);
      if (hxc_tmp_call_result_n229)
      {
        DisableCursor();
      }
      else
      {
        EnableCursor();
      }
    }
    bool hxc_tmp_call_result_n231 = hxc_caxecraft_app_AppScreen_showsTitle(hxc_screen);
    bool hxc_tmp_short_circuit_result_n130 = !hxc_tmp_call_result_n231;
    if (!hxc_tmp_call_result_n231)
    {
      hxc_tmp_short_circuit_result_n130 = hxc_focused;
    }
    bool hxc_tmp_short_circuit_load_result_n233 = hxc_tmp_short_circuit_result_n130;
    bool hxc_tmp_short_circuit_result_n131 = hxc_tmp_short_circuit_load_result_n233;
    if (hxc_tmp_short_circuit_load_result_n233)
    {
      bool hxc_tmp_call_result_n235 = hxc_caxecraft_app_AppScreen_pausesSimulation(hxc_screen);
      hxc_tmp_short_circuit_result_n131 = hxc_tmp_call_result_n235;
    }
    bool hxc_tmp_short_circuit_load_result_n236 = hxc_tmp_short_circuit_result_n131;
    bool hxc_tmp_short_circuit_result_n132 = hxc_tmp_short_circuit_load_result_n236;
    if (hxc_tmp_short_circuit_load_result_n236)
    {
      hxc_tmp_short_circuit_result_n132 = hxc_capturePressed;
    }
    if (hxc_tmp_short_circuit_result_n132)
    {
      enum hxc_caxecraft_app_AppScreen hxc_tmp_call_result_n240 = hxc_caxecraft_app_AppScreen_recapture(hxc_screen);
      hxc_screen = hxc_tmp_call_result_n240;
      hxc_recapturedThisFrame = true;
      hxc_accumulator = 0.0;
      hxc_resetMotionThisFrame = true;
      DisableCursor();
    }
    bool hxc_tmp_call_result_n242 = hxc_caxecraft_app_AppScreen_showsTitle(hxc_screen);
    bool hxc_onTitle = hxc_tmp_call_result_n242;
    bool hxc_tmp_call_result_n244 = hxc_caxecraft_app_AppScreen_pausesSimulation(hxc_screen);
    bool hxc_paused_h39f5af304915 = hxc_tmp_call_result_n244;
    bool hxc_tmp_call_result_n246 = hxc_caxecraft_app_AppScreen_capturesPointer(hxc_screen);
    bool hxc_captured_h6756636cf2ae = hxc_tmp_call_result_n246;
    if (hxc_captured_h6756636cf2ae)
    {
      double hxc_yawDelta = hxc_lookYaw_h6f5199981cd0;
      if (hxc_yawDelta > 0.25)
      {
        hxc_yawDelta = 0.25;
      }
      if (hxc_yawDelta < -0.25)
      {
        hxc_yawDelta = -0.25;
      }
      double hxc_tmp_load_result_n251 = hxc_lookX;
      double hxc_tmp_load_result_n252 = hxc_yawDelta;
      double hxc_candidateX = hxc_tmp_load_result_n251 + hxc_tmp_load_result_n252 * hxc_lookZ;
      double hxc_tmp_load_result_n254 = hxc_lookZ;
      double hxc_tmp_load_result_n255 = hxc_yawDelta;
      double hxc_candidateZ = hxc_tmp_load_result_n254 - hxc_tmp_load_result_n255 * hxc_lookX;
      double hxc_tmp_load_result_n257 = hxc_candidateX;
      double hxc_tmp_load_result_n258 = hxc_candidateX;
      double hxc_tmp_load_result_n259 = hxc_candidateZ;
      double hxc_lengthSquared = hxc_tmp_load_result_n257 * hxc_tmp_load_result_n258 + hxc_tmp_load_result_n259 * hxc_candidateZ;
      double hxc_normalize = 1.5 - 0.5 * hxc_lengthSquared;
      double hxc_tmp_load_result_n262 = hxc_candidateX;
      hxc_lookX = hxc_tmp_load_result_n262 * hxc_normalize;
      double hxc_tmp_load_result_n264 = hxc_candidateZ;
      hxc_lookZ = hxc_tmp_load_result_n264 * hxc_normalize;
      double hxc_tmp_compound_load_result_n266 = hxc_lookY;
      hxc_lookY = hxc_tmp_compound_load_result_n266 + hxc_lookPitch_h5b3b3688df1e;
      if (hxc_lookY > 0.90)
      {
        hxc_lookY = 0.90;
      }
      if (hxc_lookY < -0.90)
      {
        hxc_lookY = -0.90;
      }
      if (hxc_jumpPressed_h24b772e3821d)
      {
        hxc_jumpQueued = true;
      }
    }
    float hxc_tmp_native_call_result_n271 = GetFrameTime();
    double hxc_frameSeconds = (double)hxc_tmp_native_call_result_n271;
    if (hxc_frameSeconds > 0.25)
    {
      hxc_frameSeconds = 0.25;
    }
    bool hxc_tmp_load_result_n273 = hxc_captured_h6756636cf2ae;
    bool hxc_tmp_short_circuit_result_n142 = hxc_tmp_load_result_n273;
    if (hxc_tmp_load_result_n273)
    {
      hxc_tmp_short_circuit_result_n142 = !hxc_recapturedThisFrame;
    }
    bool hxc_tmp_short_circuit_load_result_n275 = hxc_tmp_short_circuit_result_n142;
    bool hxc_tmp_short_circuit_result_n143 = hxc_tmp_short_circuit_load_result_n275;
    if (hxc_tmp_short_circuit_load_result_n275)
    {
      hxc_tmp_short_circuit_result_n143 = hxc_primaryPressed_h54721f68a899;
    }
    bool hxc_tmp_short_circuit_load_result_n277 = hxc_tmp_short_circuit_result_n143;
    bool hxc_tmp_short_circuit_result_n144 = hxc_tmp_short_circuit_load_result_n277;
    if (hxc_tmp_short_circuit_load_result_n277)
    {
      hxc_tmp_short_circuit_result_n144 = hxc_selectedMode == 1;
    }
    bool hxc_tmp_short_circuit_load_result_n279 = hxc_tmp_short_circuit_result_n144;
    bool hxc_tmp_short_circuit_result_n145 = hxc_tmp_short_circuit_load_result_n279;
    if (hxc_tmp_short_circuit_load_result_n279)
    {
      bool hxc_tmp_call_result_n282 = hxc_caxecraft_gameplay_Inventory_selectedIs(hxc_inventory, hxc_caxecraft_gameplay_ItemKind_CopperSword);
      hxc_tmp_short_circuit_result_n145 = hxc_tmp_call_result_n282;
    }
    if (hxc_tmp_short_circuit_result_n145)
    {
      hxc_swordQueued = true;
    }
    if (!hxc_paused_h39f5af304915)
    {
      double hxc_tmp_compound_load_result_n285 = hxc_accumulator;
      hxc_accumulator = hxc_tmp_compound_load_result_n285 + hxc_frameSeconds;
    }
    while (1)
    {
      bool hxc_tmp_load_result_n287 = hxc_paused_h39f5af304915;
      bool hxc_tmp_short_circuit_result_n146 = !hxc_tmp_load_result_n287;
      if (!hxc_tmp_load_result_n287)
      {
        hxc_tmp_short_circuit_result_n146 = hxc_accumulator >= 0.05;
      }
      if (!hxc_tmp_short_circuit_result_n146)
      {
        break;
      }
      double hxc_tmp_load_result_n290 = hxc_moveForward;
      double hxc_tmp_load_result_n291 = hxc_lookX;
      double hxc_tmp_load_result_n292 = hxc_moveRight;
      double hxc_moveX = hxc_tmp_load_result_n290 * hxc_tmp_load_result_n291 - hxc_tmp_load_result_n292 * hxc_lookZ;
      double hxc_tmp_load_result_n294 = hxc_moveForward;
      double hxc_tmp_load_result_n295 = hxc_lookZ;
      double hxc_tmp_load_result_n296 = hxc_moveRight;
      double hxc_moveZ = hxc_tmp_load_result_n294 * hxc_tmp_load_result_n295 + hxc_tmp_load_result_n296 * hxc_lookX;
      double hxc_tmp_load_result_n298 = hxc_moveForward;
      bool hxc_tmp_short_circuit_result_n149 = hxc_tmp_load_result_n298 != 0.0;
      if (hxc_tmp_load_result_n298 != 0.0)
      {
        hxc_tmp_short_circuit_result_n149 = hxc_moveRight != 0.0;
      }
      if (hxc_tmp_short_circuit_result_n149)
      {
        hxc_moveX = hxc_moveX * 0.7071067811865476;
        hxc_moveZ = hxc_moveZ * 0.7071067811865476;
      }
      int32_t hxc_tmp_load_result_n303 = hxc_selectedMode;
      int32_t hxc_tmp_conditional_result_n151 = 0;
      if (hxc_tmp_load_result_n303 == 1)
      {
        hxc_tmp_conditional_result_n151 = 1;
      }
      else
      {
        hxc_tmp_conditional_result_n151 = 0;
      }
      int32_t hxc_damagePolicy = hxc_tmp_conditional_result_n151;
      struct hxc_caxecraft_domain_GameSession *hxc_tmp_owned_class_field_address_n305 = &(*hxc_self).hxc_session;
      if (hxc_tmp_owned_class_field_address_n305 == NULL)
      {
        abort();
      }
      double hxc_tmp_load_result_n306 = hxc_moveX;
      double hxc_tmp_load_result_n307 = hxc_moveZ;
      bool hxc_tmp_load_result_n308 = hxc_jumpQueued;
      struct hxc_caxecraft_domain_CharacterIntent hxc_tmp_call_result_n310 = hxc_caxecraft_domain_Aquatics_input(hxc_tmp_load_result_n306, hxc_tmp_load_result_n307, hxc_tmp_load_result_n308, hxc_descendHeld);
      struct hxc_caxecraft_domain_GameTickResult hxc_tmp_instance_call_result_n313 = hxc_caxecraft_domain_GameSession_tick(hxc_tmp_owned_class_field_address_n305, (struct hxc_caxecraft_domain_GameTickInput){ .hxc_damagePolicy = hxc_damagePolicy, .hxc_intent = hxc_tmp_call_result_n310, .hxc_waterUpdateBudget = 64 });
      struct hxc_caxecraft_domain_GameTickResult hxc_gameTick = hxc_tmp_instance_call_result_n313;
      hxc_character = hxc_gameTick.hxc_character;
      if (hxc_gameTick.hxc_committed)
      {
        struct hxc_caxecraft_app_MotionHistory hxc_tmp_load_result_n316 = hxc_motionHistory;
        struct hxc_caxecraft_app_MotionHistory hxc_tmp_call_result_n318 = hxc_caxecraft_app_MotionInterpolation_advance(hxc_tmp_load_result_n316, hxc_character.hxc_body);
        hxc_motionHistory = hxc_tmp_call_result_n318;
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
          int32_t hxc_tmp_load_result_n323 = hxc_pickupIndex;
          if (hxc_tmp_load_result_n323 < 0 || (size_t)hxc_tmp_load_result_n323 >= hxc_tmp_length_n5)
          {
            abort();
          }
          int32_t hxc_tmp_collection_index_load_result_n324 = hxc_itemActive[(size_t)hxc_tmp_load_result_n323];
          int32_t hxc_tmp_load_result_n325 = hxc_inactiveItem;
          bool hxc_tmp_short_circuit_result_n154 = hxc_tmp_collection_index_load_result_n324 != hxc_tmp_load_result_n325;
          if (hxc_tmp_collection_index_load_result_n324 != hxc_tmp_load_result_n325)
          {
            double hxc_tmp_record_field_load_result_n326 = hxc_character.hxc_body.hxc_x;
            double hxc_tmp_record_field_load_result_n327 = hxc_character.hxc_body.hxc_y;
            double hxc_tmp_record_field_load_result_n328 = hxc_character.hxc_body.hxc_z;
            int32_t hxc_tmp_call_result_n330 = hxc_caxecraft_content_FirstPlayableLevel_itemXMilli(hxc_pickupIndex);
            int32_t hxc_tmp_call_result_n332 = hxc_caxecraft_content_FirstPlayableLevel_itemYMilli(hxc_pickupIndex);
            int32_t hxc_tmp_call_result_n334 = hxc_caxecraft_content_FirstPlayableLevel_itemZMilli(hxc_pickupIndex);
            bool hxc_tmp_call_result_n335 = hxc_caxecraft_gameplay_WorldItemPickup_isInRange(hxc_tmp_record_field_load_result_n326, hxc_tmp_record_field_load_result_n327, hxc_tmp_record_field_load_result_n328, hxc_tmp_call_result_n330, hxc_tmp_call_result_n332, hxc_tmp_call_result_n334);
            hxc_tmp_short_circuit_result_n154 = hxc_tmp_call_result_n335;
          }
          if (hxc_tmp_short_circuit_result_n154)
          {
            int32_t hxc_tmp_call_result_n338 = hxc_caxecraft_content_FirstPlayableLevel_itemStorageCode(hxc_pickupIndex);
            int32_t hxc_itemCode = hxc_tmp_call_result_n338;
            int32_t hxc_tmp_call_result_n340 = hxc_caxecraft_content_BaseContentPack_itemFromValidatedStorageCode(hxc_itemCode);
            int32_t hxc_item = hxc_tmp_call_result_n340;
            int32_t hxc_tmp_call_result_n342 = hxc_caxecraft_content_BaseContentPack_itemUseProfile(hxc_item);
            bool hxc_tmp_short_circuit_result_n157 = hxc_tmp_call_result_n342 == 1;
            if (hxc_tmp_call_result_n342 == 1)
            {
              bool hxc_tmp_call_result_n344 = hxc_caxecraft_content_BaseContentPack_itemProvidesAquaticProfile(hxc_item);
              hxc_tmp_short_circuit_result_n157 = hxc_tmp_call_result_n344;
            }
            if (hxc_tmp_short_circuit_result_n157)
            {
              int32_t hxc_tmp_call_result_n347 = hxc_caxecraft_content_BaseContentPack_itemAquaticProfile(hxc_item);
              struct hxc_caxecraft_domain_AquaticProfile hxc_tmp_call_result_n348 = hxc_caxecraft_content_BaseContentPack_aquaticProfile(hxc_tmp_call_result_n347);
              struct hxc_caxecraft_domain_AquaticProfile hxc_replacement = hxc_tmp_call_result_n348;
              struct hxc_caxecraft_domain_Character hxc_tmp_load_result_n349 = hxc_character;
              struct hxc_caxecraft_domain_Character hxc_tmp_call_result_n351 = hxc_caxecraft_domain_Character_adoptProfile(hxc_tmp_load_result_n349, hxc_replacement);
              hxc_character = hxc_tmp_call_result_n351;
              hxc_aquaticEquipmentCode = hxc_itemCode;
              hxc_aquaticEquipmentFrames = 120;
              int32_t hxc_tmp_load_result_n353 = hxc_pickupIndex;
              if (hxc_tmp_load_result_n353 < 0 || (size_t)hxc_tmp_load_result_n353 >= hxc_tmp_length_n5)
              {
                abort();
              }
              hxc_itemActive[(size_t)hxc_tmp_load_result_n353] = hxc_inactiveItem;
            }
          }
          hxc_pickupIndex = hxc_i32_add_wrapping(hxc_pickupIndex, 1);
        }
      }
      if (hxc_selectedMode == 1)
      {
        if (hxc_character.hxc_vitals.hxc_health > 0)
        {
          struct hxc_caxecraft_gameplay_MosslingState hxc_tmp_load_result_n358 = hxc_mossling;
          double hxc_tmp_record_field_load_result_n359 = hxc_character.hxc_body.hxc_x;
          bool hxc_tmp_call_result_n361 = hxc_caxecraft_gameplay_Mossling_attacksThisTick(hxc_tmp_load_result_n358, hxc_tmp_record_field_load_result_n359, hxc_character.hxc_body.hxc_z);
          bool hxc_mosslingAttacked = hxc_tmp_call_result_n361;
          struct hxc_caxecraft_domain_Character hxc_tmp_load_result_n362 = hxc_character;
          struct hxc_caxecraft_domain_Character hxc_tmp_call_result_n364 = hxc_caxecraft_domain_Character_applyAttack(hxc_tmp_load_result_n362, hxc_mosslingAttacked);
          hxc_character = hxc_tmp_call_result_n364;
          if (hxc_mosslingAttacked)
          {
            hxc_enemyAttackFrames = 120;
          }
          struct hxc_caxecraft_gameplay_MosslingState hxc_tmp_load_result_n366 = hxc_mossling;
          double hxc_tmp_record_field_load_result_n367 = hxc_character.hxc_body.hxc_x;
          double hxc_tmp_record_field_load_result_n368 = hxc_character.hxc_body.hxc_z;
          struct hxc_caxecraft_gameplay_MosslingState hxc_tmp_call_result_n370 = hxc_caxecraft_gameplay_Mossling_step(hxc_cells, hxc_tmp_length_n3, hxc_tmp_load_result_n366, hxc_tmp_record_field_load_result_n367, hxc_tmp_record_field_load_result_n368, hxc_gameTick.hxc_tickIndex);
          hxc_mossling = hxc_tmp_call_result_n370;
        }
        struct hxc_caxecraft_gameplay_SwordCombatState hxc_tmp_call_result_n372 = hxc_caxecraft_gameplay_SwordCombat_step(hxc_swordCombat);
        hxc_swordCombat = hxc_tmp_call_result_n372;
        if (hxc_swordQueued)
        {
          struct hxc_caxecraft_gameplay_SwordCombatState hxc_tmp_load_result_n374 = hxc_swordCombat;
          struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_load_result_n375 = hxc_inventory;
          struct hxc_caxecraft_domain_VitalsState hxc_tmp_record_field_load_result_n376 = hxc_character.hxc_vitals;
          struct hxc_caxecraft_gameplay_MosslingState hxc_tmp_load_result_n377 = hxc_mossling;
          double hxc_tmp_record_field_load_result_n378 = hxc_character.hxc_body.hxc_x;
          double hxc_tmp_record_field_load_result_n379 = hxc_character.hxc_body.hxc_z;
          double hxc_tmp_load_result_n380 = hxc_lookX;
          int32_t hxc_tmp_call_result_n382 = hxc_caxecraft_gameplay_SwordCombat_decide(hxc_tmp_load_result_n374, hxc_tmp_load_result_n375, hxc_tmp_record_field_load_result_n376, hxc_tmp_load_result_n377, hxc_tmp_record_field_load_result_n378, hxc_tmp_record_field_load_result_n379, hxc_tmp_load_result_n380, hxc_lookZ);
          int32_t hxc_swordDecision = hxc_tmp_call_result_n382;
          if (hxc_swordDecision == 5)
          {
            struct hxc_caxecraft_gameplay_MosslingState hxc_tmp_call_result_n385 = hxc_caxecraft_gameplay_Mossling_strike(hxc_mossling);
            hxc_mossling = hxc_tmp_call_result_n385;
            hxc_strikeHitFrames = 16;
            if (hxc_mossling.hxc_health <= 0)
            {
              struct hxc_caxecraft_gameplay_BerryDropState hxc_tmp_call_result_n388 = hxc_caxecraft_gameplay_BerryDrop_fromDefeatedMossling(hxc_mossling);
              hxc_berryDrop = hxc_tmp_call_result_n388;
              hxc_enemyDefeatedFrames = 120;
            }
          }
          int32_t hxc_tmp_load_result_n389 = hxc_swordDecision;
          struct hxc_caxecraft_gameplay_SwordCombatState hxc_tmp_call_result_n391 = hxc_caxecraft_gameplay_SwordCombat_after(hxc_tmp_load_result_n389, hxc_swordCombat);
          hxc_swordCombat = hxc_tmp_call_result_n391;
          hxc_swordQueued = false;
        }
      }
      hxc_jumpQueued = false;
      hxc_accumulator = hxc_accumulator - 0.05;
    }
    struct hxc_caxecraft_domain_GameSession *hxc_tmp_owned_class_field_address_n393 = &(*hxc_self).hxc_session;
    if (hxc_tmp_owned_class_field_address_n393 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_domain_GameView hxc_tmp_instance_call_result_n394 = hxc_caxecraft_domain_GameSession_view(hxc_tmp_owned_class_field_address_n393);
    struct hxc_caxecraft_domain_GameView hxc_committedView = hxc_tmp_instance_call_result_n394;
    if (!hxc_committedView.hxc_valid)
    {
      hxc_quit = true;
    }
    int32_t hxc_completedTicks = hxc_committedView.hxc_completedTicks;
    double hxc_selectionEyeX = hxc_character.hxc_body.hxc_x;
    double hxc_selectionEyeY = hxc_character.hxc_body.hxc_y + 1.62;
    double hxc_selectionEyeZ = hxc_character.hxc_body.hxc_z;
    double hxc_tmp_load_result_n400 = hxc_selectionEyeX;
    double hxc_tmp_load_result_n401 = hxc_selectionEyeY;
    double hxc_tmp_load_result_n402 = hxc_selectionEyeZ;
    double hxc_tmp_load_result_n403 = hxc_lookX;
    double hxc_tmp_load_result_n404 = hxc_lookY;
    struct hxc_caxecraft_domain_RaycastHit hxc_tmp_call_result_n406 = hxc_caxecraft_domain_VoxelRaycast_trace(hxc_cells, hxc_tmp_length_n3, hxc_tmp_load_result_n400, hxc_tmp_load_result_n401, hxc_tmp_load_result_n402, hxc_tmp_load_result_n403, hxc_tmp_load_result_n404, hxc_lookZ, 7.0);
    struct hxc_caxecraft_domain_RaycastHit hxc_hit = hxc_tmp_call_result_n406;
    bool hxc_tmp_load_result_n407 = hxc_captured_h6756636cf2ae;
    bool hxc_tmp_short_circuit_result_n167 = hxc_tmp_load_result_n407;
    if (hxc_tmp_load_result_n407)
    {
      hxc_tmp_short_circuit_result_n167 = !hxc_recapturedThisFrame;
    }
    bool hxc_tmp_short_circuit_load_result_n409 = hxc_tmp_short_circuit_result_n167;
    bool hxc_tmp_short_circuit_result_n168 = hxc_tmp_short_circuit_load_result_n409;
    if (hxc_tmp_short_circuit_load_result_n409)
    {
      hxc_tmp_short_circuit_result_n168 = hxc_primaryPressed_h54721f68a899;
    }
    if (hxc_tmp_short_circuit_result_n168)
    {
      if (hxc_character.hxc_vitals.hxc_health > 0)
      {
        if (hxc_selectedMode == 1)
        {
          bool hxc_tmp_call_result_n416 = hxc_caxecraft_gameplay_Inventory_selectedIs(hxc_inventory, hxc_caxecraft_gameplay_ItemKind_CopperSword);
          bool hxc_tmp_short_circuit_result_n169 = !hxc_tmp_call_result_n416;
          if (!hxc_tmp_call_result_n416)
          {
            hxc_tmp_short_circuit_result_n169 = hxc_hit.hxc_hit;
          }
          bool hxc_tmp_short_circuit_load_result_n418 = hxc_tmp_short_circuit_result_n169;
          bool hxc_tmp_short_circuit_result_n170 = hxc_tmp_short_circuit_load_result_n418;
          if (hxc_tmp_short_circuit_load_result_n418)
          {
            struct hxc_caxecraft_domain_AquaticState hxc_tmp_record_field_load_result_n419 = hxc_character.hxc_aquatic;
            bool hxc_tmp_call_result_n421 = hxc_caxecraft_domain_Aquatics_canMine(hxc_tmp_record_field_load_result_n419, hxc_character.hxc_aquaticProfile);
            hxc_tmp_short_circuit_result_n170 = hxc_tmp_call_result_n421;
          }
          if (hxc_tmp_short_circuit_result_n170)
          {
            int32_t hxc_tmp_record_field_load_result_n423 = hxc_hit.hxc_cellX;
            int32_t hxc_tmp_record_field_load_result_n424 = hxc_hit.hxc_cellY;
            struct hxc_caxecraft_domain_BlockCoord hxc_tmp_call_result_n426 = hxc_caxecraft_domain_World_coord(hxc_tmp_record_field_load_result_n423, hxc_tmp_record_field_load_result_n424, hxc_hit.hxc_cellZ);
            struct hxc_caxecraft_domain_BlockCoord hxc_minedCoordinate = hxc_tmp_call_result_n426;
            struct hxc_caxecraft_domain_BlockCoord hxc_tmp_load_result_n427 = hxc_minedCoordinate;
            struct hxc_caxecraft_gameplay_MiningResult hxc_tmp_call_result_n429 = hxc_caxecraft_gameplay_Mining_attempt(hxc_cells, hxc_tmp_length_n3, hxc_tmp_load_result_n427, hxc_inventory);
            struct hxc_caxecraft_gameplay_MiningResult hxc_mining = hxc_tmp_call_result_n429;
            hxc_inventory = hxc_mining.hxc_inventory;
            if (hxc_mining.hxc_outcome == 2)
            {
              struct hxc_caxecraft_domain_GameSession *hxc_tmp_owned_class_field_address_n432 = &(*hxc_self).hxc_session;
              struct hxc_caxecraft_domain_WaterSimulation *hxc_tmp_owned_class_field_address_n433 = &(*hxc_tmp_owned_class_field_address_n432).hxc_water;
              if (hxc_tmp_owned_class_field_address_n433 == NULL)
              {
                abort();
              }
              hxc_caxecraft_domain_WaterSimulation_terrainChanged(hxc_tmp_owned_class_field_address_n433, hxc_pendingCells, hxc_tmp_length_n4, hxc_minedCoordinate);
              struct hxc_caxecraft_app_TerrainRenderer *hxc_tmp_owned_class_field_address_n435 = &(*hxc_self).hxc_terrainRenderer;
              struct hxc_caxecraft_app_TerrainChunkCache *hxc_tmp_owned_class_field_address_n436 = &(*hxc_tmp_owned_class_field_address_n435).hxc_cache;
              if (hxc_tmp_owned_class_field_address_n436 == NULL)
              {
                abort();
              }
              hxc_caxecraft_app_TerrainChunkCache_invalidate(hxc_tmp_owned_class_field_address_n436, hxc_minedCoordinate);
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
            int32_t hxc_tmp_record_field_load_result_n440 = hxc_hit.hxc_cellX;
            int32_t hxc_tmp_record_field_load_result_n441 = hxc_hit.hxc_cellY;
            struct hxc_caxecraft_domain_BlockCoord hxc_tmp_call_result_n443 = hxc_caxecraft_domain_World_coord(hxc_tmp_record_field_load_result_n440, hxc_tmp_record_field_load_result_n441, hxc_hit.hxc_cellZ);
            struct hxc_caxecraft_domain_BlockCoord hxc_removedCoordinate = hxc_tmp_call_result_n443;
            struct hxc_caxecraft_domain_GameSession *hxc_tmp_owned_class_field_address_n444 = &(*hxc_self).hxc_session;
            struct hxc_caxecraft_domain_WaterSimulation *hxc_tmp_owned_class_field_address_n445 = &(*hxc_tmp_owned_class_field_address_n444).hxc_water;
            if (hxc_tmp_owned_class_field_address_n445 == NULL)
            {
              abort();
            }
            bool hxc_tmp_instance_call_result_n447 = hxc_caxecraft_domain_WaterSimulation_removeTerrain(hxc_tmp_owned_class_field_address_n445, hxc_cells, hxc_tmp_length_n3, hxc_pendingCells, hxc_tmp_length_n4, hxc_removedCoordinate);
            bool hxc_removed = hxc_tmp_instance_call_result_n447;
            if (hxc_removed)
            {
              struct hxc_caxecraft_app_TerrainRenderer *hxc_tmp_owned_class_field_address_n449 = &(*hxc_self).hxc_terrainRenderer;
              struct hxc_caxecraft_app_TerrainChunkCache *hxc_tmp_owned_class_field_address_n450 = &(*hxc_tmp_owned_class_field_address_n449).hxc_cache;
              if (hxc_tmp_owned_class_field_address_n450 == NULL)
              {
                abort();
              }
              hxc_caxecraft_app_TerrainChunkCache_invalidate(hxc_tmp_owned_class_field_address_n450, hxc_removedCoordinate);
            }
          }
        }
      }
    }
    bool hxc_tmp_load_result_n452 = hxc_captured_h6756636cf2ae;
    bool hxc_tmp_short_circuit_result_n175 = hxc_tmp_load_result_n452;
    if (hxc_tmp_load_result_n452)
    {
      hxc_tmp_short_circuit_result_n175 = hxc_secondaryPressed_hba76f40dab68;
    }
    if (hxc_tmp_short_circuit_result_n175)
    {
      if (hxc_character.hxc_vitals.hxc_health > 0)
      {
        struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_load_result_n456 = hxc_inventory;
        int32_t hxc_tmp_call_result_n458 = hxc_caxecraft_gameplay_Recovery_decide(hxc_tmp_load_result_n456, hxc_character.hxc_vitals);
        int32_t hxc_recoveryDecision = hxc_tmp_call_result_n458;
        if (hxc_recoveryDecision != 0)
        {
          hxc_recoveryFeedback = hxc_recoveryDecision;
          hxc_recoveryFeedbackFrames = 90;
          int32_t hxc_tmp_load_result_n461 = hxc_recoveryDecision;
          struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_call_result_n463 = hxc_caxecraft_gameplay_Recovery_applyInventory(hxc_tmp_load_result_n461, hxc_inventory);
          hxc_inventory = hxc_tmp_call_result_n463;
          struct hxc_caxecraft_domain_Character hxc_tmp_load_result_n464 = hxc_character;
          int32_t hxc_tmp_load_result_n465 = hxc_recoveryDecision;
          struct hxc_caxecraft_domain_VitalsState hxc_tmp_call_result_n467 = hxc_caxecraft_gameplay_Recovery_applyVitals(hxc_tmp_load_result_n465, hxc_character.hxc_vitals);
          struct hxc_caxecraft_domain_Character hxc_tmp_call_result_n468 = hxc_caxecraft_domain_Character_withVitals(hxc_tmp_load_result_n464, hxc_tmp_call_result_n467);
          hxc_character = hxc_tmp_call_result_n468;
        }
        else
        {
          if (hxc_hit.hxc_hit)
          {
            int32_t hxc_tmp_record_field_load_result_n470 = hxc_hit.hxc_previousX;
            int32_t hxc_tmp_record_field_load_result_n471 = hxc_hit.hxc_previousY;
            struct hxc_caxecraft_domain_BlockCoord hxc_tmp_call_result_n473 = hxc_caxecraft_domain_World_coord(hxc_tmp_record_field_load_result_n470, hxc_tmp_record_field_load_result_n471, hxc_hit.hxc_previousZ);
            struct hxc_caxecraft_domain_BlockCoord hxc_placement = hxc_tmp_call_result_n473;
            enum hxc_caxecraft_domain_BlockKind hxc_tmp_call_result_n475 = hxc_caxecraft_gameplay_Inventory_selectedBlock(hxc_inventory);
            enum hxc_caxecraft_domain_BlockKind hxc_selectedBlock = hxc_tmp_call_result_n475;
            struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_load_result_n476 = hxc_inventory;
            int32_t hxc_tmp_call_result_n478 = hxc_caxecraft_gameplay_Inventory_countAt(hxc_tmp_load_result_n476, hxc_inventory.hxc_selected);
            bool hxc_hasItem = hxc_tmp_call_result_n478 > 0;
            bool hxc_tmp_load_result_n479 = hxc_hasItem;
            bool hxc_tmp_short_circuit_result_n180 = !hxc_tmp_load_result_n479;
            if (!!hxc_tmp_load_result_n479)
            {
              bool hxc_tmp_call_result_n481 = hxc_caxecraft_domain_World_isPlaceable(hxc_selectedBlock);
              hxc_tmp_short_circuit_result_n180 = !hxc_tmp_call_result_n481;
            }
            bool hxc_tmp_short_circuit_load_result_n482 = hxc_tmp_short_circuit_result_n180;
            bool hxc_tmp_short_circuit_result_n181 = hxc_tmp_short_circuit_load_result_n482;
            if (!hxc_tmp_short_circuit_load_result_n482)
            {
              struct hxc_caxecraft_domain_CharacterBody hxc_tmp_record_field_load_result_n483 = hxc_character.hxc_body;
              bool hxc_tmp_call_result_n485 = hxc_caxecraft_domain_CharacterPhysics_canPlaceAt(hxc_tmp_record_field_load_result_n483, hxc_placement);
              hxc_tmp_short_circuit_result_n181 = !hxc_tmp_call_result_n485;
            }
            bool hxc_tmp_short_circuit_load_result_n486 = hxc_tmp_short_circuit_result_n181;
            bool hxc_tmp_short_circuit_result_n182 = hxc_tmp_short_circuit_load_result_n486;
            if (!hxc_tmp_short_circuit_load_result_n486)
            {
              struct hxc_caxecraft_domain_GameSession *hxc_tmp_owned_class_field_address_n487 = &(*hxc_self).hxc_session;
              struct hxc_caxecraft_domain_WaterSimulation *hxc_tmp_owned_class_field_address_n488 = &(*hxc_tmp_owned_class_field_address_n487).hxc_water;
              if (hxc_tmp_owned_class_field_address_n488 == NULL)
              {
                abort();
              }
              struct hxc_caxecraft_domain_BlockCoord hxc_tmp_load_result_n489 = hxc_placement;
              bool hxc_tmp_instance_call_result_n491 = hxc_caxecraft_domain_WaterSimulation_placeTerrain(hxc_tmp_owned_class_field_address_n488, hxc_cells, hxc_tmp_length_n3, hxc_pendingCells, hxc_tmp_length_n4, hxc_tmp_load_result_n489, hxc_selectedBlock);
              hxc_tmp_short_circuit_result_n182 = !hxc_tmp_instance_call_result_n491;
            }
            if (hxc_tmp_short_circuit_result_n182)
            {
              hxc_placementBlockedFrames = 60;
            }
            else
            {
              struct hxc_caxecraft_app_TerrainRenderer *hxc_tmp_owned_class_field_address_n493 = &(*hxc_self).hxc_terrainRenderer;
              struct hxc_caxecraft_app_TerrainChunkCache *hxc_tmp_owned_class_field_address_n494 = &(*hxc_tmp_owned_class_field_address_n493).hxc_cache;
              if (hxc_tmp_owned_class_field_address_n494 == NULL)
              {
                abort();
              }
              hxc_caxecraft_app_TerrainChunkCache_invalidate(hxc_tmp_owned_class_field_address_n494, hxc_placement);
              if (hxc_selectedMode == 1)
              {
                struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_call_result_n498 = hxc_caxecraft_gameplay_Inventory_consumeSelected(hxc_inventory);
                hxc_inventory = hxc_tmp_call_result_n498;
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
    if (!hxc_paused_h39f5af304915)
    {
      struct hxc_caxecraft_gameplay_BerryDropState hxc_tmp_load_result_n502 = hxc_berryDrop;
      double hxc_tmp_record_field_load_result_n503 = hxc_character.hxc_body.hxc_x;
      double hxc_tmp_record_field_load_result_n504 = hxc_character.hxc_body.hxc_y;
      bool hxc_tmp_call_result_n506 = hxc_caxecraft_gameplay_BerryDrop_isInRange(hxc_tmp_load_result_n502, hxc_tmp_record_field_load_result_n503, hxc_tmp_record_field_load_result_n504, hxc_character.hxc_body.hxc_z);
      if (hxc_tmp_call_result_n506)
      {
        struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_load_result_n507 = hxc_inventory;
        int32_t hxc_tmp_call_result_n510 = hxc_caxecraft_gameplay_Inventory_acceptedAmount(hxc_tmp_load_result_n507, hxc_caxecraft_gameplay_ItemKind_Berries, hxc_berryDrop.hxc_amount);
        int32_t hxc_acceptedDrop = hxc_tmp_call_result_n510;
        if (hxc_acceptedDrop > 0)
        {
          struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_load_result_n512 = hxc_inventory;
          struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_call_result_n515 = hxc_caxecraft_gameplay_Inventory_collectItem(hxc_tmp_load_result_n512, hxc_caxecraft_gameplay_ItemKind_Berries, hxc_acceptedDrop);
          hxc_inventory = hxc_tmp_call_result_n515;
          struct hxc_caxecraft_gameplay_BerryDropState hxc_tmp_load_result_n516 = hxc_berryDrop;
          struct hxc_caxecraft_gameplay_BerryDropState hxc_tmp_call_result_n518 = hxc_caxecraft_gameplay_BerryDrop_collectAmount(hxc_tmp_load_result_n516, hxc_acceptedDrop);
          hxc_berryDrop = hxc_tmp_call_result_n518;
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
    struct hxc_caxecraft_domain_GameSession *hxc_tmp_owned_class_field_address_n535 = &(*hxc_self).hxc_session;
    struct hxc_caxecraft_domain_GameSession *hxc_this = hxc_tmp_owned_class_field_address_n535;
    struct hxc_caxecraft_domain_GameSession *hxc_tmp_load_result_n536 = hxc_this;
    if (hxc_tmp_load_result_n536 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_domain_EntityStore *hxc_tmp_owned_class_field_address_n537 = &(*hxc_tmp_load_result_n536).hxc_entities;
    if (hxc_tmp_owned_class_field_address_n537 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_domain_GameSession *hxc_tmp_load_result_n538 = hxc_this;
    if (hxc_tmp_load_result_n538 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_domain_PlayerAgent hxc_tmp_class_field_load_result_n539 = (*hxc_tmp_load_result_n538).hxc_localPlayer;
    bool hxc_tmp_instance_call_result_n542 = hxc_caxecraft_domain_EntityStore_replace(hxc_tmp_owned_class_field_address_n537, hxc_tmp_class_field_load_result_n539.hxc_characterId, hxc_character);
    if (!hxc_tmp_instance_call_result_n542)
    {
      hxc_quit = true;
    }
    if (hxc_resetMotionThisFrame)
    {
      struct hxc_caxecraft_app_MotionHistory hxc_tmp_call_result_n545 = hxc_caxecraft_app_MotionInterpolation_start(hxc_character.hxc_body);
      hxc_motionHistory = hxc_tmp_call_result_n545;
    }
    struct hxc_caxecraft_app_MotionHistory hxc_tmp_load_result_n546 = hxc_motionHistory;
    struct hxc_caxecraft_app_RenderPosition hxc_tmp_call_result_n548 = hxc_caxecraft_app_MotionInterpolation_sample(hxc_tmp_load_result_n546, hxc_accumulator, 0.05);
    struct hxc_caxecraft_app_RenderPosition hxc_renderPosition = hxc_tmp_call_result_n548;
    double hxc_eyeX = hxc_renderPosition.hxc_x;
    double hxc_eyeY = hxc_renderPosition.hxc_y + 1.62;
    double hxc_eyeZ = hxc_renderPosition.hxc_z;
    double hxc_tmp_load_result_n552 = hxc_eyeX;
    double hxc_tmp_load_result_n553 = hxc_eyeY;
    double hxc_tmp_load_result_n554 = hxc_eyeZ;
    double hxc_tmp_load_result_n556 = hxc_eyeX;
    double hxc_tmp_load_result_n557 = hxc_lookX;
    double hxc_tmp_load_result_n558 = hxc_eyeY;
    double hxc_tmp_load_result_n559 = hxc_lookY;
    double hxc_tmp_load_result_n560 = hxc_eyeZ;
    struct Camera3D hxc_camera = (struct Camera3D){ .position = (struct Vector3){ .x = (float)hxc_tmp_load_result_n552, .y = (float)hxc_tmp_load_result_n553, .z = (float)hxc_tmp_load_result_n554 }, .target = (struct Vector3){ .x = (float)(hxc_tmp_load_result_n556 + hxc_tmp_load_result_n557), .y = (float)(hxc_tmp_load_result_n558 + hxc_tmp_load_result_n559), .z = (float)(hxc_tmp_load_result_n560 + hxc_lookZ) }, .up = (struct Vector3){ .x = (float)0.0, .y = (float)1.0, .z = (float)0.0 }, .fovy = (float)70.0, .projection = 0 };
    BeginDrawing();
    if (hxc_onTitle)
    {
      int32_t hxc_tmp_native_call_result_n566 = GetScreenWidth();
      int32_t hxc_width_h55842f1c0316 = (int32_t)hxc_tmp_native_call_result_n566;
      int32_t hxc_tmp_native_call_result_n567 = GetScreenHeight();
      int32_t hxc_height_ha7fc3b6cf35b = (int32_t)hxc_tmp_native_call_result_n567;
      if (hxc_titleTextureReady)
      {
        struct Color hxc_this1_h1870d894a773 = (struct Color){ .r = 255, .g = 255, .b = 255, .a = 255 };
        struct Texture hxc_tmp_load_result_n570 = hxc_titleTexture;
        double hxc_imageAspect = hxc_f64_divide_zero_safe((double)(int32_t)hxc_tmp_load_result_n570.width, (double)(int32_t)hxc_titleTexture.height);
        int32_t hxc_tmp_load_result_n574 = hxc_width_h55842f1c0316;
        double hxc_screenAspect = hxc_f64_divide_zero_safe((double)hxc_tmp_load_result_n574, (double)hxc_height_ha7fc3b6cf35b);
        double hxc_sourceX = 0.0;
        double hxc_sourceY = 0.0;
        double hxc_sourceWidth = (double)(int32_t)hxc_titleTexture.width;
        double hxc_sourceHeight = (double)(int32_t)hxc_titleTexture.height;
        double hxc_tmp_load_result_n580 = hxc_screenAspect;
        if (hxc_tmp_load_result_n580 > hxc_imageAspect)
        {
          struct Texture hxc_tmp_load_result_n582 = hxc_titleTexture;
          hxc_sourceHeight = hxc_f64_divide_zero_safe((double)(int32_t)hxc_tmp_load_result_n582.width, hxc_screenAspect);
          struct Texture hxc_tmp_load_result_n585 = hxc_titleTexture;
          hxc_sourceY = ((double)(int32_t)hxc_tmp_load_result_n585.height - hxc_sourceHeight) * 0.5;
        }
        else
        {
          struct Texture hxc_tmp_load_result_n588 = hxc_titleTexture;
          hxc_sourceWidth = (double)(int32_t)hxc_tmp_load_result_n588.height * hxc_screenAspect;
          struct Texture hxc_tmp_load_result_n591 = hxc_titleTexture;
          hxc_sourceX = ((double)(int32_t)hxc_tmp_load_result_n591.width - hxc_sourceWidth) * 0.5;
        }
        struct Texture hxc_tmp_load_result_n594 = hxc_titleTexture;
        double hxc_tmp_load_result_n595 = hxc_sourceX;
        double hxc_tmp_load_result_n596 = hxc_sourceY;
        double hxc_tmp_load_result_n597 = hxc_sourceWidth;
        double hxc_tmp_load_result_n598 = hxc_sourceHeight;
        int32_t hxc_tmp_load_result_n600 = hxc_width_h55842f1c0316;
        int32_t hxc_tmp_load_result_n601 = hxc_height_ha7fc3b6cf35b;
        DrawTexturePro(hxc_tmp_load_result_n594, (struct Rectangle){ .x = (float)hxc_tmp_load_result_n595, .y = (float)hxc_tmp_load_result_n596, .width = (float)hxc_tmp_load_result_n597, .height = (float)hxc_tmp_load_result_n598 }, (struct Rectangle){ .x = (float)0.0, .y = (float)0.0, .width = (float)((double)hxc_tmp_load_result_n600 + 0.0), .height = (float)((double)hxc_tmp_load_result_n601 + 0.0) }, (struct Vector2){ .x = (float)0.0, .y = (float)0.0 }, (float)0.0, hxc_this1_h1870d894a773);
      }
      else
      {
        ClearBackground((struct Color){ .r = 126, .g = 190, .b = 201, .a = 255 });
      }
      struct Color hxc_this1_hea3535c501fa = (struct Color){ .r = 4, .g = 14, .b = 23, .a = 132 };
      int32_t hxc_tmp_load_result_n607 = hxc_width_h55842f1c0316;
      int32_t hxc_tmp_load_result_n608 = hxc_height_ha7fc3b6cf35b;
      DrawRectangle((int32_t)0, (int32_t)0, (int32_t)hxc_tmp_load_result_n607, (int32_t)hxc_tmp_load_result_n608, hxc_this1_hea3535c501fa);
      if (hxc_wordmarkTextureReady)
      {
        int32_t hxc_centerX = hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_width_h55842f1c0316, (double)2));
        struct Color hxc_this1_h4bb3e30f7ad7 = (struct Color){ .r = 255, .g = 255, .b = 255, .a = 255 };
        double hxc_scale = hxc_f64_divide_zero_safe((double)560, (double)(int32_t)hxc_wordmarkTexture.width);
        double hxc_heightScale = hxc_f64_divide_zero_safe((double)132, (double)(int32_t)hxc_wordmarkTexture.height);
        double hxc_tmp_load_result_n617 = hxc_heightScale;
        if (hxc_tmp_load_result_n617 < hxc_scale)
        {
          hxc_scale = hxc_heightScale;
        }
        struct Texture hxc_tmp_load_result_n620 = hxc_wordmarkTexture;
        double hxc_width_hd7a651f0fc38 = (double)(int32_t)hxc_tmp_load_result_n620.width * hxc_scale;
        struct Texture hxc_tmp_load_result_n623 = hxc_wordmarkTexture;
        double hxc_height_hafb7783489e7 = (double)(int32_t)hxc_tmp_load_result_n623.height * hxc_scale;
        double hxc_width1 = (double)(int32_t)hxc_wordmarkTexture.width + 0.0;
        double hxc_height1 = (double)(int32_t)hxc_wordmarkTexture.height + 0.0;
        struct Texture hxc_tmp_load_result_n630 = hxc_wordmarkTexture;
        double hxc_tmp_load_result_n631 = hxc_width1;
        double hxc_tmp_load_result_n632 = hxc_height1;
        int32_t hxc_tmp_load_result_n634 = hxc_centerX;
        double hxc_tmp_load_result_n635 = hxc_width_hd7a651f0fc38;
        double hxc_tmp_load_result_n636 = hxc_width_hd7a651f0fc38;
        double hxc_tmp_load_result_n637 = hxc_height_hafb7783489e7;
        DrawTexturePro(hxc_tmp_load_result_n630, (struct Rectangle){ .x = (float)0.0, .y = (float)0.0, .width = (float)hxc_tmp_load_result_n631, .height = (float)hxc_tmp_load_result_n632 }, (struct Rectangle){ .x = (float)((double)hxc_tmp_load_result_n634 - hxc_tmp_load_result_n635 * 0.5), .y = (float)28.0, .width = (float)hxc_tmp_load_result_n636, .height = (float)hxc_tmp_load_result_n637 }, (struct Vector2){ .x = (float)0.0, .y = (float)0.0 }, (float)0.0, hxc_this1_h4bb3e30f7ad7);
      }
      else
      {
        int32_t hxc_x_hc4f8f14c954d = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_width_h55842f1c0316, (double)2)), 92);
        struct Color hxc_this1_h4263b82f4cdc = (struct Color){ .r = 242, .g = 249, .b = 245, .a = 255 };
        const char *hxc_tmp_call_result_n644 = hxc_caxecraft_localization_UiCatalog_text(hxc_locale, 17);
        int32_t hxc_tmp_load_result_n645 = hxc_x_hc4f8f14c954d;
        DrawText(hxc_tmp_call_result_n644, (int32_t)hxc_tmp_load_result_n645, (int32_t)52, (int32_t)34, hxc_this1_h4263b82f4cdc);
      }
      int32_t hxc_firstTop = hxc_i32_subtract_wrapping(hxc_height_ha7fc3b6cf35b, 225);
      int32_t hxc_left_hc850de20ce1e = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_width_h55842f1c0316, (double)2)), 180);
      if (hxc_selectedMode == 0)
      {
        struct Color hxc_this1_h2614f9f4d0a3 = (struct Color){ .r = 16, .g = 88, .b = 102, .a = 232 };
        int32_t hxc_tmp_load_result_n651 = hxc_left_hc850de20ce1e;
        int32_t hxc_tmp_load_result_n652 = hxc_firstTop;
        DrawRectangle((int32_t)hxc_tmp_load_result_n651, (int32_t)hxc_tmp_load_result_n652, (int32_t)360, (int32_t)54, hxc_this1_h2614f9f4d0a3);
        struct Color hxc_this1_h06276a2cb0a6 = (struct Color){ .r = 255, .g = 190, .b = 55, .a = 255 };
        int32_t hxc_tmp_load_result_n655 = hxc_left_hc850de20ce1e;
        int32_t hxc_tmp_load_result_n656 = hxc_firstTop;
        DrawRectangleLines((int32_t)hxc_tmp_load_result_n655, (int32_t)hxc_tmp_load_result_n656, (int32_t)360, (int32_t)54, hxc_this1_h06276a2cb0a6);
      }
      else
      {
        struct Color hxc_this1_h5cf0f224801b = (struct Color){ .r = 6, .g = 26, .b = 36, .a = 218 };
        int32_t hxc_tmp_load_result_n659 = hxc_left_hc850de20ce1e;
        int32_t hxc_tmp_load_result_n660 = hxc_firstTop;
        DrawRectangle((int32_t)hxc_tmp_load_result_n659, (int32_t)hxc_tmp_load_result_n660, (int32_t)360, (int32_t)54, hxc_this1_h5cf0f224801b);
        struct Color hxc_this1_h6b4d4c64ec43 = (struct Color){ .r = 92, .g = 194, .b = 188, .a = 255 };
        int32_t hxc_tmp_load_result_n663 = hxc_left_hc850de20ce1e;
        int32_t hxc_tmp_load_result_n664 = hxc_firstTop;
        DrawRectangleLines((int32_t)hxc_tmp_load_result_n663, (int32_t)hxc_tmp_load_result_n664, (int32_t)360, (int32_t)54, hxc_this1_h6b4d4c64ec43);
      }
      struct Color hxc_this1_hd1958c7bed6b = (struct Color){ .r = 242, .g = 249, .b = 245, .a = 255 };
      const char *hxc_tmp_call_result_n668 = hxc_caxecraft_localization_UiCatalog_text(hxc_locale, 11);
      int32_t hxc_tmp_load_result_n669 = hxc_left_hc850de20ce1e;
      int32_t hxc_tmp_load_result_n670 = hxc_firstTop;
      DrawText(hxc_tmp_call_result_n668, (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n669, 28), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n670, 16), (int32_t)21, hxc_this1_hd1958c7bed6b);
      int32_t hxc_top = hxc_i32_add_wrapping(hxc_i32_add_wrapping(hxc_firstTop, 54), 14);
      int32_t hxc_left_he65bfc121857 = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_width_h55842f1c0316, (double)2)), 180);
      if (hxc_selectedMode == 1)
      {
        struct Color hxc_this1_h20c2b0dc47fd = (struct Color){ .r = 16, .g = 88, .b = 102, .a = 232 };
        int32_t hxc_tmp_load_result_n676 = hxc_left_he65bfc121857;
        int32_t hxc_tmp_load_result_n677 = hxc_top;
        DrawRectangle((int32_t)hxc_tmp_load_result_n676, (int32_t)hxc_tmp_load_result_n677, (int32_t)360, (int32_t)54, hxc_this1_h20c2b0dc47fd);
        struct Color hxc_this1_hf74b7200e366 = (struct Color){ .r = 255, .g = 190, .b = 55, .a = 255 };
        int32_t hxc_tmp_load_result_n680 = hxc_left_he65bfc121857;
        int32_t hxc_tmp_load_result_n681 = hxc_top;
        DrawRectangleLines((int32_t)hxc_tmp_load_result_n680, (int32_t)hxc_tmp_load_result_n681, (int32_t)360, (int32_t)54, hxc_this1_hf74b7200e366);
      }
      else
      {
        struct Color hxc_this1_hc8dbf4b728b0 = (struct Color){ .r = 6, .g = 26, .b = 36, .a = 218 };
        int32_t hxc_tmp_load_result_n684 = hxc_left_he65bfc121857;
        int32_t hxc_tmp_load_result_n685 = hxc_top;
        DrawRectangle((int32_t)hxc_tmp_load_result_n684, (int32_t)hxc_tmp_load_result_n685, (int32_t)360, (int32_t)54, hxc_this1_hc8dbf4b728b0);
        struct Color hxc_this1_h8fa9d7c208ba = (struct Color){ .r = 92, .g = 194, .b = 188, .a = 255 };
        int32_t hxc_tmp_load_result_n688 = hxc_left_he65bfc121857;
        int32_t hxc_tmp_load_result_n689 = hxc_top;
        DrawRectangleLines((int32_t)hxc_tmp_load_result_n688, (int32_t)hxc_tmp_load_result_n689, (int32_t)360, (int32_t)54, hxc_this1_h8fa9d7c208ba);
      }
      struct Color hxc_this1_hf4ee83235bf5 = (struct Color){ .r = 242, .g = 249, .b = 245, .a = 255 };
      const char *hxc_tmp_call_result_n693 = hxc_caxecraft_localization_UiCatalog_text(hxc_locale, 10);
      int32_t hxc_tmp_load_result_n694 = hxc_left_he65bfc121857;
      int32_t hxc_tmp_load_result_n695 = hxc_top;
      DrawText(hxc_tmp_call_result_n693, (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n694, 28), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n695, 16), (int32_t)21, hxc_this1_hf4ee83235bf5);
      int32_t hxc_x_ha552df70c3c1 = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_width_h55842f1c0316, (double)2)), 285);
      struct Color hxc_this1_h457f972477cc = (struct Color){ .r = 229, .g = 241, .b = 235, .a = 255 };
      const char *hxc_tmp_call_result_n700 = hxc_caxecraft_localization_UiCatalog_text(hxc_locale, 12);
      int32_t hxc_tmp_load_result_n701 = hxc_x_ha552df70c3c1;
      int32_t hxc_tmp_load_result_n702 = hxc_height_ha7fc3b6cf35b;
      DrawText(hxc_tmp_call_result_n700, (int32_t)hxc_tmp_load_result_n701, (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n702, 58), (int32_t)16, hxc_this1_h457f972477cc);
      if (hxc_selectedMode == 1)
      {
        int32_t hxc_x_h04fe7ed81046 = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_width_h55842f1c0316, (double)2)), 230);
        struct Color hxc_this1_he882878e4f78 = (struct Color){ .r = 255, .g = 205, .b = 91, .a = 255 };
        const char *hxc_tmp_call_result_n708 = hxc_caxecraft_localization_FirstPlayableCatalog_text(hxc_locale, 1);
        int32_t hxc_tmp_load_result_n709 = hxc_x_h04fe7ed81046;
        int32_t hxc_tmp_load_result_n710 = hxc_firstTop;
        DrawText(hxc_tmp_call_result_n708, (int32_t)hxc_tmp_load_result_n709, (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n710, 36), (int32_t)17, hxc_this1_he882878e4f78);
      }
    }
    else
    {
      ClearBackground((struct Color){ .r = 126, .g = 190, .b = 201, .a = 255 });
      int32_t hxc_tmp_native_call_result_n713 = GetScreenWidth();
      int32_t hxc_sunX = hxc_i32_subtract_wrapping((int32_t)hxc_tmp_native_call_result_n713, 260);
      float hxc_radius_h6ad3ab275a30 = (float)42.0;
      struct Color hxc_this1_h65866ca13a5a = (struct Color){ .r = 255, .g = 226, .b = 136, .a = 255 };
      int32_t hxc_tmp_load_result_n715 = hxc_sunX;
      float hxc_tmp_load_result_n716 = hxc_radius_h6ad3ab275a30;
      DrawCircle((int32_t)hxc_tmp_load_result_n715, (int32_t)86, hxc_tmp_load_result_n716, hxc_this1_h65866ca13a5a);
      float hxc_radius_hde44a8edccd9 = (float)30.0;
      struct Color hxc_this1_h4705f486231e = (struct Color){ .r = 255, .g = 246, .b = 205, .a = 255 };
      int32_t hxc_tmp_load_result_n719 = hxc_sunX;
      float hxc_tmp_load_result_n720 = hxc_radius_hde44a8edccd9;
      DrawCircle((int32_t)hxc_tmp_load_result_n719, (int32_t)86, hxc_tmp_load_result_n720, hxc_this1_h4705f486231e);
      BeginMode3D(hxc_camera);
      struct hxc_caxecraft_app_TerrainRenderer *hxc_tmp_owned_class_field_address_n723 = &(*hxc_self).hxc_terrainRenderer;
      if (hxc_tmp_owned_class_field_address_n723 == NULL)
      {
        abort();
      }
      struct Texture hxc_tmp_load_result_n724 = hxc_terrainTexture;
      bool hxc_tmp_load_result_n725 = hxc_terrainTextureReady;
      struct Texture hxc_tmp_load_result_n726 = hxc_adventureTerrainTexture;
      bool hxc_tmp_load_result_n727 = hxc_adventureTerrainTextureReady;
      double hxc_tmp_record_field_load_result_n728 = hxc_renderPosition.hxc_x;
      struct hxc_caxecraft_app_TerrainRenderCounters hxc_tmp_instance_call_result_n730 = hxc_caxecraft_app_TerrainRenderer_draw(hxc_tmp_owned_class_field_address_n723, hxc_cells, hxc_tmp_length_n3, hxc_tmp_load_result_n724, hxc_tmp_load_result_n725, hxc_tmp_load_result_n726, hxc_tmp_load_result_n727, hxc_tmp_record_field_load_result_n728, hxc_renderPosition.hxc_z);
      struct hxc_caxecraft_app_TerrainRenderCounters hxc_renderCounters = hxc_tmp_instance_call_result_n730;
      struct Texture hxc_tmp_load_result_n731 = hxc_terrainTexture;
      bool hxc_tmp_load_result_n732 = hxc_terrainTextureReady;
      struct hxc_caxecraft_app_WaterRenderCounters hxc_tmp_call_result_n734 = hxc_caxecraft_app_WaterRenderer_draw(hxc_cells, hxc_tmp_length_n3, hxc_tmp_load_result_n731, hxc_tmp_load_result_n732, hxc_waterPresentationCell);
      struct hxc_caxecraft_app_WaterRenderCounters hxc_waterCounters = hxc_tmp_call_result_n734;
      int32_t hxc_tmp_record_field_load_result_n735 = hxc_renderCounters.hxc_visible;
      int32_t hxc_totalVisible = hxc_i32_add_wrapping(hxc_tmp_record_field_load_result_n735, hxc_waterCounters.hxc_visible);
      int32_t hxc_tmp_record_field_load_result_n737 = hxc_renderCounters.hxc_drawCalls;
      int32_t hxc_totalDrawCalls = hxc_i32_add_wrapping(hxc_tmp_record_field_load_result_n737, hxc_waterCounters.hxc_drawCalls);
      struct Camera3D hxc_tmp_load_result_n739 = hxc_camera;
      struct Texture hxc_tmp_load_result_n740 = hxc_entityTexture;
      bool hxc_tmp_load_result_n741 = hxc_entityTextureReady;
      struct hxc_caxecraft_gameplay_GuideState hxc_tmp_load_result_n742 = hxc_guide;
      struct hxc_caxecraft_gameplay_MosslingState hxc_tmp_load_result_n743 = hxc_mossling;
      hxc_caxecraft_app_CaxecraftApp_drawActors(hxc_tmp_load_result_n739, hxc_tmp_load_result_n740, hxc_tmp_load_result_n741, hxc_tmp_load_result_n742, hxc_tmp_load_result_n743, hxc_berryDrop);
      struct Camera3D hxc_tmp_load_result_n745 = hxc_camera;
      struct Texture hxc_tmp_load_result_n746 = hxc_itemTexture;
      bool hxc_tmp_load_result_n747 = hxc_itemTextureReady;
      struct Texture hxc_tmp_load_result_n748 = hxc_adventureItemTexture;
      hxc_caxecraft_app_AuthoredItemRenderer_drawWorldItems(hxc_tmp_load_result_n745, hxc_itemActiveRead, hxc_tmp_length_n6, hxc_tmp_load_result_n746, hxc_tmp_load_result_n747, hxc_tmp_load_result_n748, hxc_adventureItemTextureReady);
      if (hxc_hit.hxc_hit)
      {
        double hxc_y = (double)hxc_hit.hxc_cellY + 0.5;
        double hxc_z = (double)hxc_hit.hxc_cellZ + 0.5;
        int32_t hxc_tmp_record_field_load_result_n753 = hxc_hit.hxc_cellX;
        double hxc_tmp_load_result_n754 = hxc_y;
        DrawCubeWires((struct Vector3){ .x = (float)((double)hxc_tmp_record_field_load_result_n753 + 0.5), .y = (float)hxc_tmp_load_result_n754, .z = (float)hxc_z }, (float)1.04, (float)1.04, (float)1.04, (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 });
      }
      EndMode3D();
      if (hxc_cameraWaterBlend > 0.0)
      {
        int32_t hxc_overlayAlpha = hxc_f64_to_i32_saturating(105.0 * hxc_cameraWaterBlend);
        int32_t hxc_tmp_native_call_result_n760 = GetScreenWidth();
        int32_t hxc_width_h270a69552064 = (int32_t)hxc_tmp_native_call_result_n760;
        int32_t hxc_tmp_native_call_result_n761 = GetScreenHeight();
        int32_t hxc_height_h96aa4cfda873 = (int32_t)hxc_tmp_native_call_result_n761;
        uint8_t hxc_red = (uint8_t)31;
        uint8_t hxc_green = (uint8_t)115;
        uint8_t hxc_blue = (uint8_t)154;
        int32_t hxc_tmp_load_result_n762 = hxc_overlayAlpha;
        int32_t hxc_tmp_conditional_result_n246 = 0;
        if (hxc_tmp_load_result_n762 < 0)
        {
          hxc_tmp_conditional_result_n246 = 0;
        }
        else
        {
          int32_t hxc_tmp_load_result_n763 = hxc_overlayAlpha;
          int32_t hxc_tmp_conditional_result_n247 = 0;
          if (hxc_tmp_load_result_n763 > 255)
          {
            hxc_tmp_conditional_result_n247 = 255;
          }
          else
          {
            hxc_tmp_conditional_result_n247 = hxc_overlayAlpha;
          }
          hxc_tmp_conditional_result_n246 = hxc_tmp_conditional_result_n247;
        }
        uint8_t hxc_alpha = (uint8_t)hxc_tmp_conditional_result_n246;
        uint8_t hxc_tmp_load_result_n767 = hxc_red;
        uint8_t hxc_tmp_load_result_n768 = hxc_green;
        uint8_t hxc_tmp_load_result_n769 = hxc_blue;
        struct Color hxc_this1_h133ac7574505 = (struct Color){ .r = hxc_tmp_load_result_n767, .g = hxc_tmp_load_result_n768, .b = hxc_tmp_load_result_n769, .a = hxc_alpha };
        int32_t hxc_tmp_load_result_n772 = hxc_width_h270a69552064;
        int32_t hxc_tmp_load_result_n773 = hxc_height_h96aa4cfda873;
        DrawRectangle((int32_t)0, (int32_t)0, (int32_t)hxc_tmp_load_result_n772, (int32_t)hxc_tmp_load_result_n773, hxc_this1_h133ac7574505);
      }
      int32_t hxc_tmp_load_result_n775 = hxc_totalVisible;
      int32_t hxc_tmp_load_result_n776 = hxc_totalDrawCalls;
      int32_t hxc_tmp_load_result_n777 = hxc_frameCount;
      int32_t hxc_tmp_load_result_n778 = hxc_completedTicks;
      double hxc_tmp_record_field_load_result_n780 = hxc_character.hxc_body.hxc_x;
      double hxc_tmp_record_field_load_result_n781 = hxc_character.hxc_body.hxc_z;
      struct hxc_caxecraft_domain_VitalsState hxc_tmp_record_field_load_result_n782 = hxc_character.hxc_vitals;
      int32_t hxc_tmp_load_result_n783 = hxc_aquaticEquipmentCode;
      int32_t hxc_tmp_load_result_n784 = hxc_aquaticEquipmentFrames;
      bool hxc_tmp_record_field_load_result_n785 = hxc_character.hxc_aquatic.hxc_headSubmerged;
      int32_t hxc_tmp_record_field_load_result_n786 = hxc_character.hxc_aquatic.hxc_breathTicks;
      int32_t hxc_tmp_record_field_load_result_n787 = hxc_character.hxc_aquaticProfile.hxc_maximumBreathTicks;
      int32_t hxc_tmp_load_result_n789 = hxc_placementBlockedFrames;
      int32_t hxc_tmp_load_result_n790 = hxc_strikeHitFrames;
      int32_t hxc_tmp_load_result_n791 = hxc_enemyDefeatedFrames;
      int32_t hxc_tmp_load_result_n792 = hxc_enemyAttackFrames;
      int32_t hxc_tmp_load_result_n793 = hxc_pickupFrames;
      int32_t hxc_tmp_load_result_n794 = hxc_pickupAmount;
      int32_t hxc_tmp_load_result_n795 = hxc_inventoryFullReason;
      int32_t hxc_tmp_load_result_n796 = hxc_recoveryFeedback;
      int32_t hxc_tmp_load_result_n797 = hxc_recoveryFeedbackFrames;
      bool hxc_tmp_load_result_n799 = hxc_paused_h39f5af304915;
      bool hxc_tmp_load_result_n800 = hxc_captured_h6756636cf2ae;
      struct hxc_caxecraft_domain_RaycastHit hxc_tmp_load_result_n801 = hxc_hit;
      int32_t hxc_tmp_load_result_n802 = hxc_selectedMode;
      int32_t hxc_tmp_load_result_n803 = hxc_locale;
      struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_load_result_n804 = hxc_inventory;
      struct hxc_caxecraft_gameplay_GuideState hxc_tmp_load_result_n805 = hxc_guide;
      struct hxc_caxecraft_app_HudView hxc_hudView = (struct hxc_caxecraft_app_HudView){ .hxc_character = (struct hxc_caxecraft_app_HudCharacterView){ .hxc_aquaticEquipmentCode = hxc_tmp_load_result_n783, .hxc_aquaticEquipmentVisible = hxc_tmp_load_result_n784 > 0, .hxc_breathTicks = hxc_tmp_record_field_load_result_n786, .hxc_headSubmerged = hxc_tmp_record_field_load_result_n785, .hxc_maximumBreathTicks = hxc_tmp_record_field_load_result_n787, .hxc_vitals = hxc_tmp_record_field_load_result_n782, .hxc_x = hxc_tmp_record_field_load_result_n780, .hxc_z = hxc_tmp_record_field_load_result_n781 }, .hxc_feedback = (struct hxc_caxecraft_app_HudFeedbackView){ .hxc_enemyAttacked = hxc_tmp_load_result_n792 > 0, .hxc_enemyDefeated = hxc_tmp_load_result_n791 > 0, .hxc_inventoryFullReason = hxc_tmp_load_result_n795, .hxc_pickedUp = hxc_tmp_load_result_n793 > 0, .hxc_pickupAmount = hxc_tmp_load_result_n794, .hxc_placementBlocked = hxc_tmp_load_result_n789 > 0, .hxc_recoveryDecision = hxc_tmp_load_result_n796, .hxc_recoveryVisible = hxc_tmp_load_result_n797 > 0, .hxc_strikeHit = hxc_tmp_load_result_n790 > 0 }, .hxc_guide = hxc_tmp_load_result_n805, .hxc_hit = hxc_tmp_load_result_n801, .hxc_inventory = hxc_tmp_load_result_n804, .hxc_locale = hxc_tmp_load_result_n803, .hxc_metrics = (struct hxc_caxecraft_app_HudMetricsView){ .hxc_completedTicks = hxc_tmp_load_result_n778, .hxc_drawCalls = hxc_tmp_load_result_n776, .hxc_renderedFrames = hxc_tmp_load_result_n777, .hxc_visibleBlocks = hxc_tmp_load_result_n775 }, .hxc_mode = hxc_tmp_load_result_n802, .hxc_mossling = hxc_mossling, .hxc_paused = hxc_tmp_load_result_n799, .hxc_pointerCaptured = hxc_tmp_load_result_n800 };
      struct hxc_caxecraft_app_HudView hxc_tmp_load_result_n808 = hxc_hudView;
      hxc_caxecraft_app_CaxecraftApp_drawHud(hxc_tmp_load_result_n808, hxc_hudResources);
    }
    EndDrawing();
    hxc_frameCount = hxc_i32_add_wrapping(hxc_frameCount, 1);
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
  return;
}
