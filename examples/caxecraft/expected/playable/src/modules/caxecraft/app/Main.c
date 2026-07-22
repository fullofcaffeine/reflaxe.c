#include "hxc/program.h"

void hxc_caxecraft_app_Main_drawActors(struct Camera3D hxc_camera, struct Texture hxc_entityTexture, bool hxc_entityTextureReady, struct hxc_caxecraft_gameplay_GuideState hxc_guide, struct hxc_caxecraft_gameplay_MosslingState hxc_mossling, struct hxc_caxecraft_gameplay_BerryDropState hxc_berryDrop)
{
  if (hxc_entityTextureReady)
  {
    double hxc_y_h27ff5a98b2eb = hxc_guide.hxc_y + 0.76;
    double hxc_z_h5118686df5f5 = hxc_guide.hxc_z;
    double hxc_tmp_load_result_n4 = hxc_y_h27ff5a98b2eb;
    hxc_caxecraft_app_CaxecraftAtlas_drawWorldSprite(hxc_camera, hxc_entityTexture, hxc_caxecraft_app_WorldSprite_NiaFront, (struct Vector3){ .x = (float)hxc_guide.hxc_x, .y = (float)hxc_tmp_load_result_n4, .z = (float)hxc_z_h5118686df5f5 }, 0.95, 1.52);
  }
  else
  {
    double hxc_y_hba2aaf01c1bd = hxc_guide.hxc_y + 0.54;
    double hxc_z_h4ed103e17079 = hxc_guide.hxc_z;
    double hxc_tmp_load_result_n10 = hxc_y_hba2aaf01c1bd;
    DrawCube((struct Vector3){ .x = (float)hxc_guide.hxc_x, .y = (float)hxc_tmp_load_result_n10, .z = (float)hxc_z_h4ed103e17079 }, (float)0.50, (float)0.86, (float)0.42, (struct Color){ .r = 42, .g = 150, .b = 160, .a = 255 });
    double hxc_y_h3b9af273325f = hxc_guide.hxc_y + 1.18;
    double hxc_z_h88f5b2b1d103 = hxc_guide.hxc_z;
    double hxc_tmp_load_result_n17 = hxc_y_h3b9af273325f;
    DrawCube((struct Vector3){ .x = (float)hxc_guide.hxc_x, .y = (float)hxc_tmp_load_result_n17, .z = (float)hxc_z_h88f5b2b1d103 }, (float)0.44, (float)0.44, (float)0.44, (struct Color){ .r = 205, .g = 139, .b = 88, .a = 255 });
    double hxc_y_hae84d517e8a5 = hxc_guide.hxc_y + 1.41;
    double hxc_z_h8d2482539037 = hxc_guide.hxc_z;
    double hxc_tmp_load_result_n24 = hxc_y_hae84d517e8a5;
    DrawCube((struct Vector3){ .x = (float)hxc_guide.hxc_x, .y = (float)hxc_tmp_load_result_n24, .z = (float)hxc_z_h8d2482539037 }, (float)0.48, (float)0.16, (float)0.48, (struct Color){ .r = 62, .g = 40, .b = 47, .a = 255 });
  }
  if (hxc_mossling.hxc_health > 0)
  {
    if (hxc_entityTextureReady)
    {
      double hxc_y_h48aea1133ff5 = hxc_mossling.hxc_y + 0.48;
      double hxc_z_hd819d2485212 = hxc_mossling.hxc_z;
      double hxc_tmp_load_result_n33 = hxc_y_h48aea1133ff5;
      hxc_caxecraft_app_CaxecraftAtlas_drawWorldSprite(hxc_camera, hxc_entityTexture, hxc_caxecraft_app_WorldSprite_MosslingFront, (struct Vector3){ .x = (float)hxc_mossling.hxc_x, .y = (float)hxc_tmp_load_result_n33, .z = (float)hxc_z_hd819d2485212 }, 1.05, 0.96);
    }
    else
    {
      double hxc_y_h5f2a160550af = hxc_mossling.hxc_y + 0.30;
      double hxc_z_h8f077d8fcfd2 = hxc_mossling.hxc_z;
      double hxc_tmp_load_result_n39 = hxc_y_h5f2a160550af;
      DrawCube((struct Vector3){ .x = (float)hxc_mossling.hxc_x, .y = (float)hxc_tmp_load_result_n39, .z = (float)hxc_z_h8f077d8fcfd2 }, (float)0.70, (float)0.54, (float)0.70, (struct Color){ .r = 74, .g = 119, .b = 67, .a = 255 });
      double hxc_y_h4d804ad6c22c = hxc_mossling.hxc_y + 0.66;
      double hxc_z_h4fd1e9f0625a = hxc_mossling.hxc_z;
      double hxc_tmp_load_result_n46 = hxc_y_h4d804ad6c22c;
      DrawCube((struct Vector3){ .x = (float)hxc_mossling.hxc_x, .y = (float)hxc_tmp_load_result_n46, .z = (float)hxc_z_h4fd1e9f0625a }, (float)0.50, (float)0.34, (float)0.50, (struct Color){ .r = 157, .g = 190, .b = 82, .a = 255 });
    }
    int32_t hxc_code = hxc_mossling.hxc_modeCode;
    int32_t hxc_tmp_load_result_n51 = hxc_code;
    int32_t hxc_tmp_conditional_result_n21 = 0;
    if (hxc_tmp_load_result_n51 == 1)
    {
      hxc_tmp_conditional_result_n21 = 1;
    }
    else
    {
      int32_t hxc_tmp_load_result_n52 = hxc_code;
      int32_t hxc_tmp_conditional_result_n22 = 0;
      if (hxc_tmp_load_result_n52 == 2)
      {
        hxc_tmp_conditional_result_n22 = 2;
      }
      else
      {
        int32_t hxc_tmp_load_result_n53 = hxc_code;
        int32_t hxc_tmp_conditional_result_n23 = 0;
        if (hxc_tmp_load_result_n53 == 3)
        {
          hxc_tmp_conditional_result_n23 = 3;
        }
        else
        {
          int32_t hxc_tmp_load_result_n54 = hxc_code;
          int32_t hxc_tmp_conditional_result_n24 = 0;
          if (hxc_tmp_load_result_n54 == 4)
          {
            hxc_tmp_conditional_result_n24 = 4;
          }
          else
          {
            int32_t hxc_tmp_load_result_n55 = hxc_code;
            int32_t hxc_tmp_conditional_result_n25 = 0;
            if (hxc_tmp_load_result_n55 == 5)
            {
              hxc_tmp_conditional_result_n25 = 5;
            }
            else
            {
              hxc_tmp_conditional_result_n25 = 0;
            }
            hxc_tmp_conditional_result_n24 = hxc_tmp_conditional_result_n25;
          }
          hxc_tmp_conditional_result_n23 = hxc_tmp_conditional_result_n24;
        }
        hxc_tmp_conditional_result_n22 = hxc_tmp_conditional_result_n23;
      }
      hxc_tmp_conditional_result_n21 = hxc_tmp_conditional_result_n22;
    }
    if (hxc_tmp_conditional_result_n21 == 4)
    {
      double hxc_y_h5c452c5bc759 = hxc_mossling.hxc_y + 1.02;
      double hxc_z_hf46de1055cf9 = hxc_mossling.hxc_z;
      double hxc_tmp_load_result_n64 = hxc_y_h5c452c5bc759;
      DrawCube((struct Vector3){ .x = (float)hxc_mossling.hxc_x, .y = (float)hxc_tmp_load_result_n64, .z = (float)hxc_z_hf46de1055cf9 }, (float)0.20, (float)0.20, (float)0.20, (struct Color){ .r = 218, .g = 65, .b = 72, .a = 255 });
    }
  }
  if (hxc_berryDrop.hxc_active)
  {
    double hxc_y_h4e26cc754ee3 = hxc_berryDrop.hxc_y;
    double hxc_z_h903a76602863 = hxc_berryDrop.hxc_z;
    double hxc_tmp_load_result_n72 = hxc_y_h4e26cc754ee3;
    DrawCube((struct Vector3){ .x = (float)(hxc_berryDrop.hxc_x - 0.12), .y = (float)hxc_tmp_load_result_n72, .z = (float)hxc_z_h903a76602863 }, (float)0.18, (float)0.18, (float)0.18, (struct Color){ .r = 174, .g = 78, .b = 136, .a = 255 });
    double hxc_y_h18cd4219de5c = hxc_berryDrop.hxc_y;
    double hxc_z_h1afa978e68e4 = hxc_berryDrop.hxc_z;
    double hxc_tmp_load_result_n79 = hxc_y_h18cd4219de5c;
    DrawCube((struct Vector3){ .x = (float)(hxc_berryDrop.hxc_x + 0.12), .y = (float)hxc_tmp_load_result_n79, .z = (float)hxc_z_h1afa978e68e4 }, (float)0.18, (float)0.18, (float)0.18, (struct Color){ .r = 174, .g = 78, .b = 136, .a = 255 });
  }
  return;
}

void hxc_caxecraft_app_Main_drawBreath(int32_t hxc_breathTicks, int32_t hxc_maximumBreathTicks, int32_t hxc_width, int32_t hxc_height)
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
      float hxc_radius_h146433b51616 = (float)6.0;
      struct Color hxc_this1_h578620430a97 = (struct Color){ .r = 164, .g = 235, .b = 246, .a = 255 };
      int32_t hxc_tmp_load_result_n11 = hxc_startX;
      int32_t hxc_tmp_load_result_n12 = hxc_bubble;
      int32_t hxc_tmp_load_result_n13 = hxc_y;
      float hxc_tmp_load_result_n14 = hxc_radius_h146433b51616;
      DrawCircle((int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n11, hxc_i32_multiply_wrapping(hxc_tmp_load_result_n12, 18)), (int32_t)hxc_tmp_load_result_n13, hxc_tmp_load_result_n14, hxc_this1_h578620430a97);
    }
    else
    {
      float hxc_radius_h864a87a11b3c = (float)6.0;
      struct Color hxc_this1_h0f503c58ed35 = (struct Color){ .r = 49, .g = 82, .b = 103, .a = 210 };
      int32_t hxc_tmp_load_result_n17 = hxc_startX;
      int32_t hxc_tmp_load_result_n18 = hxc_bubble;
      int32_t hxc_tmp_load_result_n19 = hxc_y;
      float hxc_tmp_load_result_n20 = hxc_radius_h864a87a11b3c;
      DrawCircle((int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n17, hxc_i32_multiply_wrapping(hxc_tmp_load_result_n18, 18)), (int32_t)hxc_tmp_load_result_n19, hxc_tmp_load_result_n20, hxc_this1_h0f503c58ed35);
    }
    hxc_bubble = hxc_i32_add_wrapping(hxc_bubble, 1);
  }
  return;
}

void hxc_caxecraft_app_Main_drawHealth(struct hxc_caxecraft_gameplay_PlayerVitalsState hxc_vitals, struct Texture hxc_hudTexture, bool hxc_hudTextureReady, int32_t hxc_width)
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
        struct Color hxc_this1_hc6d174737eec = (struct Color){ .r = 218, .g = 65, .b = 72, .a = 255 };
        int32_t hxc_tmp_load_result_n14 = hxc_x;
        DrawRectangle((int32_t)hxc_tmp_load_result_n14, (int32_t)22, (int32_t)34, (int32_t)26, hxc_this1_hc6d174737eec);
      }
      else
      {
        struct Color hxc_this1_he4c4e4ab13e3 = (struct Color){ .r = 228, .g = 239, .b = 227, .a = 255 };
        int32_t hxc_tmp_load_result_n17 = hxc_x;
        DrawRectangleLines((int32_t)hxc_tmp_load_result_n17, (int32_t)22, (int32_t)34, (int32_t)26, hxc_this1_he4c4e4ab13e3);
      }
    }
    hxc_heart = hxc_i32_add_wrapping(hxc_heart, 1);
  }
  return;
}

void hxc_caxecraft_app_Main_drawHotbar(struct hxc_caxecraft_gameplay_InventoryState hxc_inventory, struct Texture hxc_hudTexture, bool hxc_hudTextureReady, struct Texture hxc_itemTexture, bool hxc_itemTextureReady, int32_t hxc_width, int32_t hxc_height)
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
      struct Color hxc_this1_h8936f36c3a72 = (struct Color){ .r = 15, .g = 27, .b = 39, .a = 220 };
      int32_t hxc_tmp_load_result_n19 = hxc_x;
      int32_t hxc_tmp_load_result_n20 = hxc_y;
      int32_t hxc_tmp_load_result_n21 = hxc_slotSize;
      int32_t hxc_tmp_load_result_n22 = hxc_slotSize;
      DrawRectangle((int32_t)hxc_tmp_load_result_n19, (int32_t)hxc_tmp_load_result_n20, (int32_t)hxc_tmp_load_result_n21, (int32_t)hxc_tmp_load_result_n22, hxc_this1_h8936f36c3a72);
      if (hxc_slot == hxc_inventory.hxc_selected)
      {
        struct Color hxc_this1_h3a88e57146ce = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
        int32_t hxc_tmp_load_result_n27 = hxc_x;
        int32_t hxc_tmp_load_result_n28 = hxc_y;
        int32_t hxc_tmp_load_result_n29 = hxc_slotSize;
        int32_t hxc_tmp_load_result_n30 = hxc_slotSize;
        DrawRectangleLines((int32_t)hxc_tmp_load_result_n27, (int32_t)hxc_tmp_load_result_n28, (int32_t)hxc_tmp_load_result_n29, (int32_t)hxc_tmp_load_result_n30, hxc_this1_h3a88e57146ce);
      }
      else
      {
        struct Color hxc_this1_hee27b04a2b56 = (struct Color){ .r = 228, .g = 239, .b = 227, .a = 255 };
        int32_t hxc_tmp_load_result_n33 = hxc_x;
        int32_t hxc_tmp_load_result_n34 = hxc_y;
        int32_t hxc_tmp_load_result_n35 = hxc_slotSize;
        int32_t hxc_tmp_load_result_n36 = hxc_slotSize;
        DrawRectangleLines((int32_t)hxc_tmp_load_result_n33, (int32_t)hxc_tmp_load_result_n34, (int32_t)hxc_tmp_load_result_n35, (int32_t)hxc_tmp_load_result_n36, hxc_this1_hee27b04a2b56);
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

void hxc_caxecraft_app_Main_drawHud(int32_t hxc_visible, int32_t hxc_drawCalls, int32_t hxc_frames, int32_t hxc_updates, bool hxc_paused, bool hxc_captured, bool hxc_placementBlocked, struct hxc_caxecraft_domain_RaycastHit hxc_hit, double hxc_playerX, double hxc_playerZ, int32_t hxc_mode, int32_t hxc_locale, struct hxc_caxecraft_gameplay_InventoryState hxc_inventory, struct hxc_caxecraft_gameplay_GuideState hxc_guide, struct hxc_caxecraft_gameplay_MosslingState hxc_mossling, struct hxc_caxecraft_gameplay_PlayerVitalsState hxc_vitals, bool hxc_strikeHit, bool hxc_enemyDefeated, bool hxc_enemyAttacked, bool hxc_pickedUp, int32_t hxc_pickupAmount, int32_t hxc_inventoryFullReason, int32_t hxc_recoveryFeedback, bool hxc_recoveryVisible, struct Texture hxc_hudTexture, bool hxc_hudTextureReady, struct Texture hxc_itemTexture, bool hxc_itemTextureReady, bool hxc_headSubmerged, int32_t hxc_breathTicks, int32_t hxc_maximumBreathTicks)
{
  int32_t hxc_tmp_native_call_result_n0 = GetScreenWidth();
  int32_t hxc_width = (int32_t)hxc_tmp_native_call_result_n0;
  int32_t hxc_tmp_native_call_result_n1 = GetScreenHeight();
  int32_t hxc_height = (int32_t)hxc_tmp_native_call_result_n1;
  int32_t hxc_centerX = hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_width, (double)2));
  int32_t hxc_centerY = hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_height, (double)2));
  struct Color hxc_text = (struct Color){ .r = 228, .g = 239, .b = 227, .a = 255 };
  int32_t hxc_tmp_load_result_n5 = hxc_centerX;
  int32_t hxc_tmp_load_result_n6 = hxc_centerY;
  int32_t hxc_tmp_load_result_n7 = hxc_centerX;
  int32_t hxc_tmp_load_result_n8 = hxc_centerY;
  DrawLine((int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n5, 8), (int32_t)hxc_tmp_load_result_n6, (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n7, 3), (int32_t)hxc_tmp_load_result_n8, hxc_text);
  int32_t hxc_tmp_load_result_n10 = hxc_centerX;
  int32_t hxc_tmp_load_result_n11 = hxc_centerY;
  int32_t hxc_tmp_load_result_n12 = hxc_centerX;
  int32_t hxc_tmp_load_result_n13 = hxc_centerY;
  DrawLine((int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n10, 3), (int32_t)hxc_tmp_load_result_n11, (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n12, 8), (int32_t)hxc_tmp_load_result_n13, hxc_text);
  int32_t hxc_tmp_load_result_n15 = hxc_centerX;
  int32_t hxc_tmp_load_result_n16 = hxc_centerY;
  int32_t hxc_tmp_load_result_n17 = hxc_centerX;
  int32_t hxc_tmp_load_result_n18 = hxc_centerY;
  DrawLine((int32_t)hxc_tmp_load_result_n15, (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n16, 8), (int32_t)hxc_tmp_load_result_n17, (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n18, 3), hxc_text);
  int32_t hxc_tmp_load_result_n20 = hxc_centerX;
  int32_t hxc_tmp_load_result_n21 = hxc_centerY;
  int32_t hxc_tmp_load_result_n22 = hxc_centerX;
  int32_t hxc_tmp_load_result_n23 = hxc_centerY;
  DrawLine((int32_t)hxc_tmp_load_result_n20, (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n21, 3), (int32_t)hxc_tmp_load_result_n22, (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n23, 8), hxc_text);
  struct Color hxc_this1_hd4942058cefc = (struct Color){ .r = 15, .g = 27, .b = 39, .a = 220 };
  DrawRectangle((int32_t)18, (int32_t)18, (int32_t)460, (int32_t)108, hxc_this1_hd4942058cefc);
  struct Color hxc_this1_h90e153b90172 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
  DrawRectangleLines((int32_t)18, (int32_t)18, (int32_t)460, (int32_t)108, hxc_this1_h90e153b90172);
  hxc_caxecraft_localization_UiCatalog_draw(hxc_locale, 0, 32, 28, 20, hxc_text);
  hxc_caxecraft_localization_UiCatalog_draw(hxc_locale, 3, 32, 58, 14, hxc_text);
  hxc_caxecraft_app_HudDigits_drawNumber(16384, 82, 59, 5, (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 });
  hxc_caxecraft_localization_UiCatalog_draw(hxc_locale, 7, 160, 58, 14, hxc_text);
  hxc_caxecraft_app_HudDigits_drawNumber(hxc_visible, 230, 59, 5, (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 });
  hxc_caxecraft_localization_UiCatalog_draw(hxc_locale, 4, 326, 58, 14, hxc_text);
  hxc_caxecraft_app_HudDigits_drawNumber(hxc_drawCalls, 382, 59, 5, (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 });
  hxc_caxecraft_localization_UiCatalog_draw(hxc_locale, 5, 32, 86, 12, hxc_text);
  hxc_caxecraft_app_HudDigits_drawNumber(hxc_frames, 82, 85, 6, hxc_text);
  hxc_caxecraft_localization_UiCatalog_draw(hxc_locale, 6, 174, 86, 12, hxc_text);
  hxc_caxecraft_app_HudDigits_drawNumber(hxc_updates, 216, 85, 6, hxc_text);
  int32_t hxc_tmp_load_result_n40 = hxc_width;
  hxc_caxecraft_app_Main_drawHotbar(hxc_inventory, hxc_hudTexture, hxc_hudTextureReady, hxc_itemTexture, hxc_itemTextureReady, hxc_tmp_load_result_n40, hxc_height);
  hxc_caxecraft_app_Main_drawHealth(hxc_vitals, hxc_hudTexture, hxc_hudTextureReady, hxc_width);
  if (hxc_headSubmerged)
  {
    int32_t hxc_tmp_load_result_n43 = hxc_width;
    hxc_caxecraft_app_Main_drawBreath(hxc_breathTicks, hxc_maximumBreathTicks, hxc_tmp_load_result_n43, hxc_height);
  }
  int32_t hxc_tmp_load_result_n45 = hxc_height;
  hxc_caxecraft_localization_UiCatalog_draw(hxc_locale, 2, 20, hxc_i32_subtract_wrapping(hxc_tmp_load_result_n45, 22), 14, hxc_text);
  if (hxc_mode == 1)
  {
    hxc_caxecraft_localization_FirstPlayableCatalog_draw(hxc_locale, 0, 32, 110, 14, (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 });
  }
  bool hxc_tmp_call_result_n48 = hxc_caxecraft_gameplay_GuideNpc_isInRange(hxc_guide, hxc_playerX, hxc_playerZ);
  if (hxc_tmp_call_result_n48)
  {
    struct Color hxc_this1_h21caa9e3fcf1 = (struct Color){ .r = 15, .g = 27, .b = 39, .a = 220 };
    int32_t hxc_tmp_load_result_n50 = hxc_centerX;
    int32_t hxc_tmp_load_result_n51 = hxc_centerY;
    DrawRectangle((int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n50, 260), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n51, 54), (int32_t)520, (int32_t)60, hxc_this1_h21caa9e3fcf1);
    int32_t hxc_code_h297318e8c7b0 = hxc_guide.hxc_phaseCode;
    int32_t hxc_tmp_load_result_n54 = hxc_code_h297318e8c7b0;
    int32_t hxc_tmp_conditional_result_n40 = 0;
    if (hxc_tmp_load_result_n54 == 1)
    {
      hxc_tmp_conditional_result_n40 = 1;
    }
    else
    {
      int32_t hxc_tmp_load_result_n55 = hxc_code_h297318e8c7b0;
      int32_t hxc_tmp_conditional_result_n41 = 0;
      if (hxc_tmp_load_result_n55 == 2)
      {
        hxc_tmp_conditional_result_n41 = 2;
      }
      else
      {
        hxc_tmp_conditional_result_n41 = 0;
      }
      hxc_tmp_conditional_result_n40 = hxc_tmp_conditional_result_n41;
    }
    if (hxc_tmp_conditional_result_n40 == 0)
    {
      int32_t hxc_tmp_load_result_n58 = hxc_centerX;
      int32_t hxc_tmp_load_result_n59 = hxc_centerY;
      hxc_caxecraft_localization_FirstPlayableCatalog_draw(hxc_locale, 13, hxc_i32_subtract_wrapping(hxc_tmp_load_result_n58, 110), hxc_i32_add_wrapping(hxc_tmp_load_result_n59, 74), 18, hxc_text);
    }
    else
    {
      int32_t hxc_code_h8f3c827fc342 = hxc_guide.hxc_phaseCode;
      int32_t hxc_tmp_load_result_n62 = hxc_code_h8f3c827fc342;
      int32_t hxc_tmp_conditional_result_n43 = 0;
      if (hxc_tmp_load_result_n62 == 1)
      {
        hxc_tmp_conditional_result_n43 = 1;
      }
      else
      {
        int32_t hxc_tmp_load_result_n63 = hxc_code_h8f3c827fc342;
        int32_t hxc_tmp_conditional_result_n44 = 0;
        if (hxc_tmp_load_result_n63 == 2)
        {
          hxc_tmp_conditional_result_n44 = 2;
        }
        else
        {
          hxc_tmp_conditional_result_n44 = 0;
        }
        hxc_tmp_conditional_result_n43 = hxc_tmp_conditional_result_n44;
      }
      if (hxc_tmp_conditional_result_n43 == 1)
      {
        int32_t hxc_tmp_load_result_n66 = hxc_centerX;
        int32_t hxc_tmp_load_result_n67 = hxc_centerY;
        hxc_caxecraft_localization_FirstPlayableCatalog_draw(hxc_locale, 14, hxc_i32_subtract_wrapping(hxc_tmp_load_result_n66, 225), hxc_i32_add_wrapping(hxc_tmp_load_result_n67, 74), 16, hxc_text);
      }
      else
      {
        int32_t hxc_tmp_load_result_n69 = hxc_centerX;
        int32_t hxc_tmp_load_result_n70 = hxc_centerY;
        hxc_caxecraft_localization_FirstPlayableCatalog_draw(hxc_locale, 12, hxc_i32_subtract_wrapping(hxc_tmp_load_result_n69, 205), hxc_i32_add_wrapping(hxc_tmp_load_result_n70, 74), 16, hxc_text);
      }
    }
  }
  if (hxc_mossling.hxc_health > 0)
  {
    int32_t hxc_code_h0012cc8417fc = hxc_mossling.hxc_modeCode;
    int32_t hxc_tmp_load_result_n74 = hxc_code_h0012cc8417fc;
    int32_t hxc_tmp_conditional_result_n46 = 0;
    if (hxc_tmp_load_result_n74 == 1)
    {
      hxc_tmp_conditional_result_n46 = 1;
    }
    else
    {
      int32_t hxc_tmp_load_result_n75 = hxc_code_h0012cc8417fc;
      int32_t hxc_tmp_conditional_result_n47 = 0;
      if (hxc_tmp_load_result_n75 == 2)
      {
        hxc_tmp_conditional_result_n47 = 2;
      }
      else
      {
        int32_t hxc_tmp_load_result_n76 = hxc_code_h0012cc8417fc;
        int32_t hxc_tmp_conditional_result_n48 = 0;
        if (hxc_tmp_load_result_n76 == 3)
        {
          hxc_tmp_conditional_result_n48 = 3;
        }
        else
        {
          int32_t hxc_tmp_load_result_n77 = hxc_code_h0012cc8417fc;
          int32_t hxc_tmp_conditional_result_n49 = 0;
          if (hxc_tmp_load_result_n77 == 4)
          {
            hxc_tmp_conditional_result_n49 = 4;
          }
          else
          {
            int32_t hxc_tmp_load_result_n78 = hxc_code_h0012cc8417fc;
            int32_t hxc_tmp_conditional_result_n50 = 0;
            if (hxc_tmp_load_result_n78 == 5)
            {
              hxc_tmp_conditional_result_n50 = 5;
            }
            else
            {
              hxc_tmp_conditional_result_n50 = 0;
            }
            hxc_tmp_conditional_result_n49 = hxc_tmp_conditional_result_n50;
          }
          hxc_tmp_conditional_result_n48 = hxc_tmp_conditional_result_n49;
        }
        hxc_tmp_conditional_result_n47 = hxc_tmp_conditional_result_n48;
      }
      hxc_tmp_conditional_result_n46 = hxc_tmp_conditional_result_n47;
    }
    if (hxc_tmp_conditional_result_n46 == 4)
    {
      hxc_caxecraft_localization_FirstPlayableCatalog_draw(hxc_locale, 11, hxc_i32_subtract_wrapping(hxc_width, 300), 28, 16, (struct Color){ .r = 218, .g = 65, .b = 72, .a = 255 });
    }
    else
    {
      int32_t hxc_code_h21abe55c24d8 = hxc_mossling.hxc_modeCode;
      int32_t hxc_tmp_load_result_n87 = hxc_code_h21abe55c24d8;
      int32_t hxc_tmp_conditional_result_n52 = 0;
      if (hxc_tmp_load_result_n87 == 1)
      {
        hxc_tmp_conditional_result_n52 = 1;
      }
      else
      {
        int32_t hxc_tmp_load_result_n88 = hxc_code_h21abe55c24d8;
        int32_t hxc_tmp_conditional_result_n53 = 0;
        if (hxc_tmp_load_result_n88 == 2)
        {
          hxc_tmp_conditional_result_n53 = 2;
        }
        else
        {
          int32_t hxc_tmp_load_result_n89 = hxc_code_h21abe55c24d8;
          int32_t hxc_tmp_conditional_result_n54 = 0;
          if (hxc_tmp_load_result_n89 == 3)
          {
            hxc_tmp_conditional_result_n54 = 3;
          }
          else
          {
            int32_t hxc_tmp_load_result_n90 = hxc_code_h21abe55c24d8;
            int32_t hxc_tmp_conditional_result_n55 = 0;
            if (hxc_tmp_load_result_n90 == 4)
            {
              hxc_tmp_conditional_result_n55 = 4;
            }
            else
            {
              int32_t hxc_tmp_load_result_n91 = hxc_code_h21abe55c24d8;
              int32_t hxc_tmp_conditional_result_n56 = 0;
              if (hxc_tmp_load_result_n91 == 5)
              {
                hxc_tmp_conditional_result_n56 = 5;
              }
              else
              {
                hxc_tmp_conditional_result_n56 = 0;
              }
              hxc_tmp_conditional_result_n55 = hxc_tmp_conditional_result_n56;
            }
            hxc_tmp_conditional_result_n54 = hxc_tmp_conditional_result_n55;
          }
          hxc_tmp_conditional_result_n53 = hxc_tmp_conditional_result_n54;
        }
        hxc_tmp_conditional_result_n52 = hxc_tmp_conditional_result_n53;
      }
      if (hxc_tmp_conditional_result_n52 == 1)
      {
        hxc_caxecraft_localization_FirstPlayableCatalog_draw(hxc_locale, 9, hxc_i32_subtract_wrapping(hxc_width, 180), 28, 16, (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 });
      }
    }
  }
  if (hxc_strikeHit)
  {
    int32_t hxc_tmp_load_result_n99 = hxc_centerX;
    hxc_caxecraft_localization_FirstPlayableCatalog_draw(hxc_locale, 7, hxc_i32_subtract_wrapping(hxc_tmp_load_result_n99, 70), hxc_i32_subtract_wrapping(hxc_centerY, 54), 18, (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 });
  }
  if (hxc_enemyDefeated)
  {
    hxc_caxecraft_localization_FirstPlayableCatalog_draw(hxc_locale, 10, hxc_i32_subtract_wrapping(hxc_width, 285), 54, 16, (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 });
  }
  if (hxc_enemyAttacked)
  {
    hxc_caxecraft_localization_FirstPlayableCatalog_draw(hxc_locale, 18, hxc_i32_subtract_wrapping(hxc_width, 330), 82, 16, (struct Color){ .r = 218, .g = 65, .b = 72, .a = 255 });
  }
  if (hxc_pickedUp)
  {
    int32_t hxc_tmp_conditional_result_n58 = 0;
    if (hxc_pickupAmount == 1)
    {
      hxc_tmp_conditional_result_n58 = 2;
    }
    else
    {
      hxc_tmp_conditional_result_n58 = 3;
    }
    int32_t hxc_pickupMessage = hxc_tmp_conditional_result_n58;
    int32_t hxc_tmp_load_result_n107 = hxc_pickupMessage;
    int32_t hxc_tmp_load_result_n108 = hxc_centerX;
    hxc_caxecraft_localization_FirstPlayableCatalog_draw(hxc_locale, hxc_tmp_load_result_n107, hxc_i32_subtract_wrapping(hxc_tmp_load_result_n108, 48), hxc_i32_add_wrapping(hxc_centerY, 24), 18, (struct Color){ .r = 174, .g = 78, .b = 136, .a = 255 });
  }
  if (hxc_inventoryFullReason == 1)
  {
    int32_t hxc_tmp_load_result_n111 = hxc_centerX;
    hxc_caxecraft_localization_FirstPlayableCatalog_draw(hxc_locale, 5, hxc_i32_subtract_wrapping(hxc_tmp_load_result_n111, 150), hxc_i32_add_wrapping(hxc_centerY, 48), 16, (struct Color){ .r = 238, .g = 113, .b = 78, .a = 255 });
  }
  else
  {
    if (hxc_inventoryFullReason == 2)
    {
      int32_t hxc_tmp_load_result_n114 = hxc_centerX;
      hxc_caxecraft_localization_FirstPlayableCatalog_draw(hxc_locale, 6, hxc_i32_subtract_wrapping(hxc_tmp_load_result_n114, 155), hxc_i32_add_wrapping(hxc_centerY, 48), 16, (struct Color){ .r = 238, .g = 113, .b = 78, .a = 255 });
    }
  }
  if (hxc_recoveryVisible)
  {
    if (hxc_recoveryFeedback == 1)
    {
      int32_t hxc_tmp_load_result_n117 = hxc_centerX;
      hxc_caxecraft_localization_FirstPlayableCatalog_draw(hxc_locale, 4, hxc_i32_subtract_wrapping(hxc_tmp_load_result_n117, 88), hxc_i32_add_wrapping(hxc_centerY, 24), 18, (struct Color){ .r = 94, .g = 212, .b = 136, .a = 255 });
    }
    else
    {
      if (hxc_recoveryFeedback == 2)
      {
        int32_t hxc_tmp_load_result_n120 = hxc_centerX;
        hxc_caxecraft_localization_UiCatalog_draw(hxc_locale, 8, hxc_i32_subtract_wrapping(hxc_tmp_load_result_n120, 96), hxc_i32_add_wrapping(hxc_centerY, 24), 18, (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 });
      }
      else
      {
        if (hxc_recoveryFeedback == 3)
        {
          int32_t hxc_tmp_load_result_n123 = hxc_centerX;
          hxc_caxecraft_localization_FirstPlayableCatalog_draw(hxc_locale, 15, hxc_i32_subtract_wrapping(hxc_tmp_load_result_n123, 76), hxc_i32_add_wrapping(hxc_centerY, 24), 18, (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 });
        }
      }
    }
  }
  if (hxc_vitals.hxc_safeTicks > 15)
  {
    struct Color hxc_this1_h8c98ecea273e = (struct Color){ .r = 218, .g = 65, .b = 72, .a = 255 };
    int32_t hxc_tmp_load_result_n128 = hxc_width;
    int32_t hxc_tmp_load_result_n129 = hxc_height;
    DrawRectangleLines((int32_t)4, (int32_t)4, (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n128, 8), (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n129, 8), hxc_this1_h8c98ecea273e);
  }
  if (hxc_vitals.hxc_health <= 0)
  {
    struct Color hxc_this1_he02f30709a30 = (struct Color){ .r = 15, .g = 27, .b = 39, .a = 220 };
    int32_t hxc_tmp_load_result_n133 = hxc_centerX;
    int32_t hxc_tmp_load_result_n134 = hxc_centerY;
    DrawRectangle((int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n133, 250), (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n134, 74), (int32_t)500, (int32_t)148, hxc_this1_he02f30709a30);
    struct Color hxc_this1_hfb5da7aeb4bc = (struct Color){ .r = 218, .g = 65, .b = 72, .a = 255 };
    int32_t hxc_tmp_load_result_n137 = hxc_centerX;
    int32_t hxc_tmp_load_result_n138 = hxc_centerY;
    DrawRectangleLines((int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n137, 250), (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n138, 74), (int32_t)500, (int32_t)148, hxc_this1_hfb5da7aeb4bc);
    int32_t hxc_tmp_load_result_n140 = hxc_centerX;
    int32_t hxc_tmp_load_result_n141 = hxc_centerY;
    hxc_caxecraft_localization_FirstPlayableCatalog_draw(hxc_locale, 8, hxc_i32_subtract_wrapping(hxc_tmp_load_result_n140, 122), hxc_i32_subtract_wrapping(hxc_tmp_load_result_n141, 42), 24, hxc_text);
    int32_t hxc_tmp_load_result_n143 = hxc_centerX;
    hxc_caxecraft_localization_FirstPlayableCatalog_draw(hxc_locale, 16, hxc_i32_subtract_wrapping(hxc_tmp_load_result_n143, 125), hxc_i32_add_wrapping(hxc_centerY, 10), 18, (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 });
  }
  if (hxc_paused)
  {
    struct Color hxc_this1_h90dae089355b = (struct Color){ .r = 15, .g = 27, .b = 39, .a = 220 };
    int32_t hxc_tmp_load_result_n147 = hxc_centerX;
    int32_t hxc_tmp_load_result_n148 = hxc_centerY;
    DrawRectangle((int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n147, 170), (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n148, 48), (int32_t)340, (int32_t)96, hxc_this1_h90dae089355b);
    struct Color hxc_this1_hc74bd2935f1b = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
    int32_t hxc_tmp_load_result_n151 = hxc_centerX;
    int32_t hxc_tmp_load_result_n152 = hxc_centerY;
    DrawRectangleLines((int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n151, 170), (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n152, 48), (int32_t)340, (int32_t)96, hxc_this1_hc74bd2935f1b);
    int32_t hxc_tmp_load_result_n154 = hxc_centerX;
    int32_t hxc_tmp_load_result_n155 = hxc_centerY;
    hxc_caxecraft_localization_UiCatalog_draw(hxc_locale, 14, hxc_i32_subtract_wrapping(hxc_tmp_load_result_n154, 48), hxc_i32_subtract_wrapping(hxc_tmp_load_result_n155, 30), 24, hxc_text);
    int32_t hxc_tmp_load_result_n157 = hxc_centerX;
    int32_t hxc_tmp_load_result_n158 = hxc_centerY;
    hxc_caxecraft_localization_UiCatalog_draw(hxc_locale, 13, hxc_i32_subtract_wrapping(hxc_tmp_load_result_n157, 160), hxc_i32_add_wrapping(hxc_tmp_load_result_n158, 8), 16, hxc_text);
  }
  else
  {
    if (hxc_placementBlocked)
    {
      int32_t hxc_tmp_load_result_n160 = hxc_centerX;
      hxc_caxecraft_localization_UiCatalog_draw(hxc_locale, 15, hxc_i32_subtract_wrapping(hxc_tmp_load_result_n160, 170), hxc_i32_add_wrapping(hxc_centerY, 26), 14, (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 });
    }
    else
    {
      if (!hxc_captured)
      {
        int32_t hxc_tmp_load_result_n163 = hxc_centerX;
        int32_t hxc_tmp_load_result_n164 = hxc_centerY;
        hxc_caxecraft_localization_UiCatalog_draw(hxc_locale, 1, hxc_i32_subtract_wrapping(hxc_tmp_load_result_n163, 90), hxc_i32_add_wrapping(hxc_tmp_load_result_n164, 26), 14, hxc_text);
      }
      else
      {
        if (!hxc_hit.hxc_hit)
        {
          int32_t hxc_tmp_load_result_n167 = hxc_centerX;
          int32_t hxc_tmp_load_result_n168 = hxc_centerY;
          hxc_caxecraft_localization_UiCatalog_draw(hxc_locale, 12, hxc_i32_subtract_wrapping(hxc_tmp_load_result_n167, 105), hxc_i32_add_wrapping(hxc_tmp_load_result_n168, 26), 14, hxc_text);
        }
      }
    }
  }
  return;
}

void hxc_caxecraft_app_Main_main(void)
{
  uint8_t hxc_storage[16384] = { 0 };
  uint8_t *hxc_cells = hxc_storage;
  size_t hxc_tmp_length_n1 = sizeof(hxc_storage) / sizeof(hxc_storage[0]);
  uint8_t hxc_pendingStorage[16384] = { 0 };
  uint8_t *hxc_pendingCells = hxc_pendingStorage;
  size_t hxc_tmp_length_n3 = sizeof(hxc_pendingStorage) / sizeof(hxc_pendingStorage[0]);
  struct hxc_caxecraft_domain_WaterSimulation hxc_tmp_object_storage_n5 = { 0 };
  struct hxc_caxecraft_domain_WaterSimulation *hxc_tmp_class_object_address_n0 = &hxc_tmp_object_storage_n5;
  hxc_compiler_constructor_caxecraft_domain_WaterSimulation(hxc_tmp_class_object_address_n0);
  struct hxc_caxecraft_domain_WaterSimulation *hxc_water = hxc_tmp_class_object_address_n0;
  bool hxc_tmp_call_result_n1 = hxc_caxecraft_content_FirstPlayableLevel_loadTerrain(hxc_cells, hxc_tmp_length_n1);
  if (!hxc_tmp_call_result_n1)
  {
    return;
  }
  struct hxc_caxecraft_domain_WaterSimulation *hxc_tmp_load_result_n2 = hxc_water;
  if (hxc_tmp_load_result_n2 == NULL)
  {
    abort();
  }
  hxc_caxecraft_domain_WaterSimulation_resetPending(hxc_tmp_load_result_n2, hxc_pendingCells, hxc_tmp_length_n3);
  bool hxc_fluidsLoaded = true;
  int32_t hxc_fluidIndex = 0;
  while (1)
  {
    if (!(hxc_fluidIndex < 2))
    {
      break;
    }
    int32_t hxc_tmp_call_result_n5 = hxc_caxecraft_content_FirstPlayableLevel_fluidX(hxc_fluidIndex);
    int32_t hxc_tmp_call_result_n7 = hxc_caxecraft_content_FirstPlayableLevel_fluidY(hxc_fluidIndex);
    int32_t hxc_tmp_call_result_n9 = hxc_caxecraft_content_FirstPlayableLevel_fluidZ(hxc_fluidIndex);
    struct hxc_caxecraft_domain_BlockCoord hxc_tmp_call_result_n10 = hxc_caxecraft_domain_World_coord(hxc_tmp_call_result_n5, hxc_tmp_call_result_n7, hxc_tmp_call_result_n9);
    struct hxc_caxecraft_domain_BlockCoord hxc_coordinate = hxc_tmp_call_result_n10;
    int32_t hxc_tmp_call_result_n12 = hxc_caxecraft_content_FirstPlayableLevel_fluidKind(hxc_fluidIndex);
    int32_t hxc_kind = hxc_tmp_call_result_n12;
    if (hxc_kind == 0)
    {
      struct hxc_caxecraft_domain_WaterSimulation *hxc_tmp_load_result_n14 = hxc_water;
      if (hxc_tmp_load_result_n14 == NULL)
      {
        abort();
      }
      struct hxc_caxecraft_domain_BlockCoord hxc_tmp_load_result_n15 = hxc_coordinate;
      int32_t hxc_tmp_call_result_n17 = hxc_caxecraft_content_FirstPlayableLevel_fluidWidth(hxc_fluidIndex);
      int32_t hxc_tmp_call_result_n19 = hxc_caxecraft_content_FirstPlayableLevel_fluidHeight(hxc_fluidIndex);
      int32_t hxc_tmp_call_result_n21 = hxc_caxecraft_content_FirstPlayableLevel_fluidDepth(hxc_fluidIndex);
      bool hxc_tmp_instance_call_result_n22 = hxc_caxecraft_domain_WaterSimulation_placeInitialVolume(hxc_tmp_load_result_n14, hxc_cells, hxc_tmp_length_n1, hxc_pendingCells, hxc_tmp_length_n3, hxc_tmp_load_result_n15, hxc_tmp_call_result_n17, hxc_tmp_call_result_n19, hxc_tmp_call_result_n21);
      if (!hxc_tmp_instance_call_result_n22)
      {
        hxc_fluidsLoaded = false;
      }
    }
    else
    {
      if (hxc_kind == 1)
      {
        struct hxc_caxecraft_domain_WaterSimulation *hxc_tmp_load_result_n24 = hxc_water;
        if (hxc_tmp_load_result_n24 == NULL)
        {
          abort();
        }
        bool hxc_tmp_instance_call_result_n26 = hxc_caxecraft_domain_WaterSimulation_placeSource(hxc_tmp_load_result_n24, hxc_cells, hxc_tmp_length_n1, hxc_pendingCells, hxc_tmp_length_n3, hxc_coordinate);
        if (!hxc_tmp_instance_call_result_n26)
        {
          hxc_fluidsLoaded = false;
        }
      }
      else
      {
        hxc_fluidsLoaded = false;
      }
    }
    hxc_fluidIndex = hxc_i32_add_wrapping(hxc_fluidIndex, 1);
  }
  if (!hxc_fluidsLoaded)
  {
    return;
  }
  int32_t hxc_tmp_call_result_n29 = hxc_caxecraft_content_FirstPlayableLevel_fluidPresentationCell(0);
  int32_t hxc_waterPresentationCell = hxc_tmp_call_result_n29;
  if (hxc_waterPresentationCell < 0)
  {
    return;
  }
  struct hxc_caxecraft_domain_AquaticProfile hxc_tmp_call_result_n31 = hxc_caxecraft_content_BaseContentPack_aquaticProfile(0);
  struct hxc_caxecraft_domain_AquaticProfile hxc_aquaticCapability = hxc_tmp_call_result_n31;
  int32_t hxc_windowFlags = hxc_i32_bit_or(64, 4);
  hxc_windowFlags = hxc_i32_bit_or(hxc_windowFlags, 8192);
  SetConfigFlags((uint32_t)hxc_windowFlags);
  InitWindow((int32_t)1280, (int32_t)720, "Caxecraft \342\200\224 Haxe shaped into careful C");
  SetWindowMinSize((int32_t)800, (int32_t)450);
  SetExitKey((int32_t)0);
  SetTargetFPS((int32_t)120);
  EnableCursor();
  struct Texture hxc_tmp_native_call_result_n34 = LoadTexture("assets/showcase/title-panorama.png");
  struct Texture hxc_titleTexture = hxc_tmp_native_call_result_n34;
  bool hxc_tmp_native_call_result_n36 = IsTextureValid(hxc_titleTexture);
  bool hxc_titleTextureReady = hxc_tmp_native_call_result_n36;
  struct Texture hxc_tmp_native_call_result_n37 = LoadTexture("assets/branding/caxecraft-wordmark.png");
  struct Texture hxc_wordmarkTexture = hxc_tmp_native_call_result_n37;
  bool hxc_tmp_native_call_result_n39 = IsTextureValid(hxc_wordmarkTexture);
  bool hxc_wordmarkTextureReady = hxc_tmp_native_call_result_n39;
  struct Texture hxc_tmp_native_call_result_n40 = LoadTexture("assets/atlases/hud.png");
  struct Texture hxc_hudTexture = hxc_tmp_native_call_result_n40;
  bool hxc_tmp_native_call_result_n42 = IsTextureValid(hxc_hudTexture);
  bool hxc_hudTextureReady = hxc_tmp_native_call_result_n42;
  struct Texture hxc_tmp_native_call_result_n43 = LoadTexture("assets/atlases/items.png");
  struct Texture hxc_itemTexture = hxc_tmp_native_call_result_n43;
  bool hxc_tmp_native_call_result_n45 = IsTextureValid(hxc_itemTexture);
  bool hxc_itemTextureReady = hxc_tmp_native_call_result_n45;
  struct Texture hxc_tmp_native_call_result_n46 = LoadTexture("assets/atlases/entities.png");
  struct Texture hxc_entityTexture = hxc_tmp_native_call_result_n46;
  bool hxc_tmp_native_call_result_n48 = IsTextureValid(hxc_entityTexture);
  bool hxc_entityTextureReady = hxc_tmp_native_call_result_n48;
  struct Texture hxc_tmp_native_call_result_n49 = LoadTexture("assets/atlases/terrain.png");
  struct Texture hxc_terrainTexture = hxc_tmp_native_call_result_n49;
  bool hxc_tmp_native_call_result_n51 = IsTextureValid(hxc_terrainTexture);
  bool hxc_terrainTextureReady = hxc_tmp_native_call_result_n51;
  struct hxc_caxecraft_domain_PlayerState hxc_tmp_call_result_n52 = hxc_caxecraft_app_Main_spawnPlayer(hxc_cells, hxc_tmp_length_n1);
  struct hxc_caxecraft_domain_PlayerState hxc_player = hxc_tmp_call_result_n52;
  struct hxc_caxecraft_domain_PlayerAquaticState hxc_tmp_call_result_n54 = hxc_caxecraft_domain_PlayerAquatics_start(hxc_aquaticCapability);
  struct hxc_caxecraft_domain_PlayerAquaticState hxc_aquatic = hxc_tmp_call_result_n54;
  double hxc_cameraWaterBlend = 0.0;
  struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_call_result_n55 = hxc_caxecraft_gameplay_Inventory_starter();
  struct hxc_caxecraft_gameplay_InventoryState hxc_inventory = hxc_tmp_call_result_n55;
  struct hxc_caxecraft_gameplay_GuideState hxc_tmp_call_result_n56 = hxc_caxecraft_gameplay_GuideNpc_start(hxc_cells, hxc_tmp_length_n1, 17.5, 13.5);
  struct hxc_caxecraft_gameplay_GuideState hxc_guide = hxc_tmp_call_result_n56;
  struct hxc_caxecraft_gameplay_MosslingState hxc_tmp_call_result_n57 = hxc_caxecraft_gameplay_Mossling_start(hxc_cells, hxc_tmp_length_n1, 15.5, 13.8);
  struct hxc_caxecraft_gameplay_MosslingState hxc_mossling = hxc_tmp_call_result_n57;
  int32_t hxc_initialHealth = 6;
  struct hxc_caxecraft_gameplay_PlayerVitalsState hxc_tmp_call_result_n59 = hxc_caxecraft_gameplay_PlayerVitals_make(hxc_initialHealth, 0);
  struct hxc_caxecraft_gameplay_PlayerVitalsState hxc_vitals = hxc_tmp_call_result_n59;
  struct hxc_caxecraft_gameplay_SwordCombatState hxc_swordCombat = (struct hxc_caxecraft_gameplay_SwordCombatState){ .hxc_cooldownTicks = 0 };
  struct hxc_caxecraft_gameplay_BerryDropState hxc_berryDrop = (struct hxc_caxecraft_gameplay_BerryDropState){ .hxc_active = false, .hxc_amount = 0, .hxc_x = 0.0, .hxc_y = 0.0, .hxc_z = 0.0 };
  double hxc_lookX = 0.0;
  double hxc_lookY = -0.18;
  double hxc_lookZ = -1.0;
  double hxc_accumulator = 0.0;
  bool hxc_jumpQueued = false;
  bool hxc_swordQueued = false;
  int32_t hxc_selectedMode = 0;
  int32_t hxc_locale = 0;
  bool hxc_onTitle = true;
  bool hxc_paused = true;
  bool hxc_captured = false;
  bool hxc_quit = false;
  int32_t hxc_frameCount = 0;
  int32_t hxc_updateCount = 0;
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
  while (1)
  {
    bool hxc_tmp_load_result_n62 = hxc_quit;
    bool hxc_tmp_short_circuit_result_n59 = !hxc_tmp_load_result_n62;
    if (!hxc_tmp_load_result_n62)
    {
      bool hxc_tmp_native_call_result_n63 = WindowShouldClose();
      hxc_tmp_short_circuit_result_n59 = !hxc_tmp_native_call_result_n63;
    }
    if (!hxc_tmp_short_circuit_result_n59)
    {
      break;
    }
    bool hxc_recapturedThisFrame = false;
    bool hxc_tmp_native_call_result_n65 = IsWindowFocused();
    bool hxc_focused = hxc_tmp_native_call_result_n65;
    double hxc_forward = 0.0;
    double hxc_right = 0.0;
    bool hxc_tmp_native_call_result_n66 = IsKeyDown((int32_t)87);
    if (hxc_tmp_native_call_result_n66)
    {
      hxc_forward = hxc_forward + 1.0;
    }
    bool hxc_tmp_native_call_result_n68 = IsKeyDown((int32_t)83);
    if (hxc_tmp_native_call_result_n68)
    {
      hxc_forward = hxc_forward - 1.0;
    }
    bool hxc_tmp_native_call_result_n70 = IsKeyDown((int32_t)68);
    if (hxc_tmp_native_call_result_n70)
    {
      hxc_right = hxc_right + 1.0;
    }
    bool hxc_tmp_native_call_result_n72 = IsKeyDown((int32_t)65);
    if (hxc_tmp_native_call_result_n72)
    {
      hxc_right = hxc_right - 1.0;
    }
    double hxc_lookYaw_h8b92d857e48e = 0.0;
    double hxc_lookPitch_h3d0d246b89a4 = 0.0;
    if (hxc_captured)
    {
      struct Vector2 hxc_tmp_native_call_result_n75 = GetMouseDelta();
      struct Vector2 hxc_mouse = hxc_tmp_native_call_result_n75;
      hxc_lookYaw_h8b92d857e48e = -(double)hxc_mouse.x * 0.0025;
      hxc_lookPitch_h3d0d246b89a4 = -(double)hxc_mouse.y * 0.0025;
    }
    bool hxc_tmp_native_call_result_n80 = IsMouseButtonPressed((int32_t)0);
    bool hxc_leftPressed = hxc_tmp_native_call_result_n80;
    bool hxc_tmp_load_result_n81 = hxc_captured;
    bool hxc_tmp_short_circuit_result_n69 = hxc_tmp_load_result_n81;
    if (hxc_tmp_load_result_n81)
    {
      hxc_tmp_short_circuit_result_n69 = hxc_leftPressed;
    }
    bool hxc_primaryPressed_h8c9063a3548b = hxc_tmp_short_circuit_result_n69;
    bool hxc_tmp_load_result_n84 = hxc_captured;
    bool hxc_tmp_short_circuit_result_n71 = hxc_tmp_load_result_n84;
    if (hxc_tmp_load_result_n84)
    {
      bool hxc_tmp_native_call_result_n85 = IsMouseButtonPressed((int32_t)1);
      hxc_tmp_short_circuit_result_n71 = hxc_tmp_native_call_result_n85;
    }
    bool hxc_secondaryPressed_hb493a4bd4536 = hxc_tmp_short_circuit_result_n71;
    bool hxc_tmp_load_result_n87 = hxc_captured;
    bool hxc_tmp_short_circuit_result_n73 = hxc_tmp_load_result_n87;
    if (hxc_tmp_load_result_n87)
    {
      bool hxc_tmp_native_call_result_n88 = IsKeyPressed((int32_t)69);
      hxc_tmp_short_circuit_result_n73 = hxc_tmp_native_call_result_n88;
    }
    bool hxc_interactPressed_h2a53c2c9f04b = hxc_tmp_short_circuit_result_n73;
    int32_t hxc_hotbarSelection_h7179e65a477e = -1;
    bool hxc_tmp_native_call_result_n90 = IsKeyPressed((int32_t)49);
    if (hxc_tmp_native_call_result_n90)
    {
      hxc_hotbarSelection_h7179e65a477e = 0;
    }
    bool hxc_tmp_native_call_result_n91 = IsKeyPressed((int32_t)50);
    if (hxc_tmp_native_call_result_n91)
    {
      hxc_hotbarSelection_h7179e65a477e = 1;
    }
    bool hxc_tmp_native_call_result_n92 = IsKeyPressed((int32_t)51);
    if (hxc_tmp_native_call_result_n92)
    {
      hxc_hotbarSelection_h7179e65a477e = 2;
    }
    bool hxc_tmp_native_call_result_n93 = IsKeyPressed((int32_t)52);
    if (hxc_tmp_native_call_result_n93)
    {
      hxc_hotbarSelection_h7179e65a477e = 3;
    }
    bool hxc_tmp_native_call_result_n94 = IsKeyPressed((int32_t)53);
    if (hxc_tmp_native_call_result_n94)
    {
      hxc_hotbarSelection_h7179e65a477e = 4;
    }
    bool hxc_tmp_native_call_result_n95 = IsKeyPressed((int32_t)54);
    if (hxc_tmp_native_call_result_n95)
    {
      hxc_hotbarSelection_h7179e65a477e = 5;
    }
    bool hxc_tmp_native_call_result_n96 = IsKeyPressed((int32_t)55);
    if (hxc_tmp_native_call_result_n96)
    {
      hxc_hotbarSelection_h7179e65a477e = 6;
    }
    bool hxc_tmp_native_call_result_n97 = IsKeyPressed((int32_t)56);
    if (hxc_tmp_native_call_result_n97)
    {
      hxc_hotbarSelection_h7179e65a477e = 7;
    }
    float hxc_tmp_native_call_result_n98 = GetMouseWheelMove();
    double hxc_wheel = (double)hxc_tmp_native_call_result_n98;
    int32_t hxc_hotbarCycle_h4e4851143c62 = 0;
    if (hxc_wheel > 0.0)
    {
      hxc_hotbarCycle_h4e4851143c62 = -1;
    }
    if (hxc_wheel < 0.0)
    {
      hxc_hotbarCycle_h4e4851143c62 = 1;
    }
    bool hxc_tmp_native_call_result_n101 = IsKeyPressed((int32_t)32);
    bool hxc_jumpPressed_ha063416180cc = hxc_tmp_native_call_result_n101;
    bool hxc_tmp_native_call_result_n102 = IsKeyPressed((int32_t)256);
    bool hxc_pausePressed_hfed98c992f8a = hxc_tmp_native_call_result_n102;
    bool hxc_tmp_native_call_result_n103 = IsKeyPressed((int32_t)81);
    bool hxc_quitPressed_h3189bdba9a73 = hxc_tmp_native_call_result_n103;
    double hxc_frameInput_moveForward = hxc_forward;
    double hxc_frameInput_moveRight = hxc_right;
    double hxc_frameInput_lookYaw = hxc_lookYaw_h8b92d857e48e;
    double hxc_frameInput_lookPitch = hxc_lookPitch_h3d0d246b89a4;
    bool hxc_frameInput_jumpPressed = hxc_jumpPressed_ha063416180cc;
    bool hxc_tmp_native_call_result_n109 = IsKeyDown((int32_t)340);
    bool hxc_frameInput_descendHeld = hxc_tmp_native_call_result_n109;
    bool hxc_frameInput_primaryPressed = hxc_primaryPressed_h8c9063a3548b;
    bool hxc_frameInput_secondaryPressed = hxc_secondaryPressed_hb493a4bd4536;
    bool hxc_frameInput_interactPressed = hxc_interactPressed_h2a53c2c9f04b;
    bool hxc_frameInput_pausePressed = hxc_pausePressed_hfed98c992f8a;
    bool hxc_tmp_load_result_n114 = hxc_paused;
    bool hxc_tmp_short_circuit_result_n91 = hxc_tmp_load_result_n114;
    if (hxc_tmp_load_result_n114)
    {
      hxc_tmp_short_circuit_result_n91 = hxc_leftPressed;
    }
    bool hxc_frameInput_capturePressed = hxc_tmp_short_circuit_result_n91;
    bool hxc_frameInput_quitPressed = hxc_quitPressed_h3189bdba9a73;
    int32_t hxc_frameInput_hotbarSelection = hxc_hotbarSelection_h7179e65a477e;
    int32_t hxc_frameInput_hotbarCycle = hxc_hotbarCycle_h4e4851143c62;
    double hxc_moveForward = hxc_frameInput_moveForward;
    double hxc_moveRight = hxc_frameInput_moveRight;
    double hxc_lookYaw_h34b6981c82d6 = hxc_frameInput_lookYaw;
    double hxc_lookPitch_h0c3bc7abcfb3 = hxc_frameInput_lookPitch;
    bool hxc_jumpPressed_h2e1d4fd64f1f = hxc_frameInput_jumpPressed;
    bool hxc_primaryPressed_h103917e2fe17 = hxc_frameInput_primaryPressed;
    bool hxc_secondaryPressed_hb044c653932c = hxc_frameInput_secondaryPressed;
    bool hxc_interactPressed_h1dd20e7e5692 = hxc_frameInput_interactPressed;
    bool hxc_pausePressed_h63c490f3b83e = hxc_frameInput_pausePressed;
    bool hxc_capturePressed = hxc_frameInput_capturePressed;
    bool hxc_quitPressed_h847dcbe22d86 = hxc_frameInput_quitPressed;
    int32_t hxc_hotbarSelection_hcea7232b142e = hxc_frameInput_hotbarSelection;
    int32_t hxc_hotbarCycle_h758630c4f35c = hxc_frameInput_hotbarCycle;
    bool hxc_descendHeld = hxc_frameInput_descendHeld;
    if (hxc_quitPressed_h847dcbe22d86)
    {
      hxc_quit = true;
    }
    if (hxc_hotbarSelection_hcea7232b142e >= 0)
    {
      struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_load_result_n136 = hxc_inventory;
      struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_call_result_n138 = hxc_caxecraft_gameplay_Inventory_select(hxc_tmp_load_result_n136, hxc_hotbarSelection_hcea7232b142e);
      hxc_inventory = hxc_tmp_call_result_n138;
    }
    if (hxc_hotbarCycle_h758630c4f35c != 0)
    {
      struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_load_result_n140 = hxc_inventory;
      struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_call_result_n142 = hxc_caxecraft_gameplay_Inventory_cycle(hxc_tmp_load_result_n140, hxc_hotbarCycle_h758630c4f35c);
      hxc_inventory = hxc_tmp_call_result_n142;
    }
    bool hxc_tmp_load_result_n143 = hxc_onTitle;
    bool hxc_tmp_short_circuit_result_n109 = !hxc_tmp_load_result_n143;
    if (!hxc_tmp_load_result_n143)
    {
      hxc_tmp_short_circuit_result_n109 = !hxc_paused;
    }
    bool hxc_tmp_short_circuit_load_result_n145 = hxc_tmp_short_circuit_result_n109;
    bool hxc_tmp_short_circuit_result_n110 = hxc_tmp_short_circuit_load_result_n145;
    if (hxc_tmp_short_circuit_load_result_n145)
    {
      hxc_tmp_short_circuit_result_n110 = hxc_interactPressed_h1dd20e7e5692;
    }
    if (hxc_tmp_short_circuit_result_n110)
    {
      if (hxc_vitals.hxc_health <= 0)
      {
        struct hxc_caxecraft_gameplay_PlayerVitalsState hxc_tmp_call_result_n149 = hxc_caxecraft_gameplay_PlayerVitals_make(6, 0);
        hxc_vitals = hxc_tmp_call_result_n149;
        struct hxc_caxecraft_domain_PlayerState hxc_tmp_call_result_n150 = hxc_caxecraft_app_Main_spawnPlayer(hxc_cells, hxc_tmp_length_n1);
        hxc_player = hxc_tmp_call_result_n150;
        struct hxc_caxecraft_domain_PlayerAquaticState hxc_tmp_call_result_n152 = hxc_caxecraft_domain_PlayerAquatics_start(hxc_aquaticCapability);
        hxc_aquatic = hxc_tmp_call_result_n152;
        hxc_cameraWaterBlend = 0.0;
      }
      else
      {
        struct hxc_caxecraft_gameplay_GuideState hxc_tmp_load_result_n153 = hxc_guide;
        double hxc_tmp_record_field_load_result_n154 = hxc_player.hxc_x;
        bool hxc_tmp_call_result_n156 = hxc_caxecraft_gameplay_GuideNpc_isInRange(hxc_tmp_load_result_n153, hxc_tmp_record_field_load_result_n154, hxc_player.hxc_z);
        if (hxc_tmp_call_result_n156)
        {
          bool hxc_tmp_call_result_n158 = hxc_caxecraft_gameplay_GuideNpc_sharesBerriesOnNextInteraction(hxc_guide);
          bool hxc_sharesBerries = hxc_tmp_call_result_n158;
          if (hxc_sharesBerries)
          {
            int32_t hxc_tmp_call_result_n162 = hxc_caxecraft_gameplay_Inventory_acceptedAmount(hxc_inventory, hxc_caxecraft_gameplay_ItemKind_Berries, 2);
            int32_t hxc_acceptedGift = hxc_tmp_call_result_n162;
            if (hxc_acceptedGift == 2)
            {
              struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_load_result_n164 = hxc_inventory;
              struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_call_result_n167 = hxc_caxecraft_gameplay_Inventory_collectItem(hxc_tmp_load_result_n164, hxc_caxecraft_gameplay_ItemKind_Berries, hxc_acceptedGift);
              hxc_inventory = hxc_tmp_call_result_n167;
              struct hxc_caxecraft_gameplay_GuideState hxc_tmp_call_result_n169 = hxc_caxecraft_gameplay_GuideNpc_interact(hxc_guide);
              hxc_guide = hxc_tmp_call_result_n169;
            }
            else
            {
              hxc_inventoryFullReason = 1;
              hxc_inventoryFullFrames = 90;
            }
          }
          else
          {
            struct hxc_caxecraft_gameplay_GuideState hxc_tmp_call_result_n171 = hxc_caxecraft_gameplay_GuideNpc_interact(hxc_guide);
            hxc_guide = hxc_tmp_call_result_n171;
          }
        }
      }
    }
    bool hxc_tmp_load_result_n172 = hxc_onTitle;
    bool hxc_tmp_short_circuit_result_n113 = hxc_tmp_load_result_n172;
    if (hxc_tmp_load_result_n172)
    {
      hxc_tmp_short_circuit_result_n113 = hxc_focused;
    }
    if (hxc_tmp_short_circuit_result_n113)
    {
      bool hxc_tmp_native_call_result_n175 = IsKeyPressed((int32_t)76);
      if (hxc_tmp_native_call_result_n175)
      {
        int32_t hxc_tmp_call_result_n177 = hxc_caxecraft_localization_UiCatalog_nextLocale(hxc_locale);
        hxc_locale = hxc_tmp_call_result_n177;
      }
      bool hxc_tmp_native_call_result_n178 = IsKeyPressed((int32_t)265);
      bool hxc_tmp_short_circuit_result_n114 = hxc_tmp_native_call_result_n178;
      if (!hxc_tmp_native_call_result_n178)
      {
        bool hxc_tmp_native_call_result_n179 = IsKeyPressed((int32_t)264);
        hxc_tmp_short_circuit_result_n114 = hxc_tmp_native_call_result_n179;
      }
      if (hxc_tmp_short_circuit_result_n114)
      {
        int32_t hxc_tmp_load_result_n181 = hxc_selectedMode;
        int32_t hxc_tmp_conditional_result_n115 = 0;
        if (hxc_tmp_load_result_n181 == 0)
        {
          hxc_tmp_conditional_result_n115 = 1;
        }
        else
        {
          hxc_tmp_conditional_result_n115 = 0;
        }
        hxc_selectedMode = hxc_tmp_conditional_result_n115;
      }
      struct Vector2 hxc_tmp_native_call_result_n183 = GetMousePosition();
      struct Vector2 hxc_menuMouse = hxc_tmp_native_call_result_n183;
      struct Vector2 hxc_tmp_load_result_n184 = hxc_menuMouse;
      struct Vector2 hxc_tmp_load_result_n186 = hxc_menuMouse;
      int32_t hxc_tmp_native_call_result_n188 = GetScreenWidth();
      int32_t hxc_tmp_native_call_result_n189 = GetScreenHeight();
      int32_t hxc_tmp_call_result_n190 = hxc_caxecraft_app_TitleMenu_selectionAt((double)hxc_tmp_load_result_n184.x, (double)hxc_tmp_load_result_n186.y, (int32_t)hxc_tmp_native_call_result_n188, (int32_t)hxc_tmp_native_call_result_n189);
      int32_t hxc_hovered = hxc_tmp_call_result_n190;
      if (hxc_hovered == 0)
      {
        hxc_selectedMode = 0;
      }
      if (hxc_hovered == 1)
      {
        hxc_selectedMode = 1;
      }
      int32_t hxc_tmp_load_result_n193 = hxc_hovered;
      bool hxc_tmp_short_circuit_result_n119 = hxc_tmp_load_result_n193 >= 0;
      if (hxc_tmp_load_result_n193 >= 0)
      {
        bool hxc_tmp_native_call_result_n194 = IsMouseButtonPressed((int32_t)0);
        hxc_tmp_short_circuit_result_n119 = hxc_tmp_native_call_result_n194;
      }
      bool hxc_clickedChoice = hxc_tmp_short_circuit_result_n119;
      bool hxc_tmp_load_result_n196 = hxc_clickedChoice;
      bool hxc_tmp_short_circuit_result_n120 = hxc_tmp_load_result_n196;
      if (!hxc_tmp_load_result_n196)
      {
        bool hxc_tmp_native_call_result_n197 = IsKeyPressed((int32_t)257);
        hxc_tmp_short_circuit_result_n120 = hxc_tmp_native_call_result_n197;
      }
      if (hxc_tmp_short_circuit_result_n120)
      {
        hxc_onTitle = false;
        hxc_paused = false;
        hxc_captured = true;
        hxc_recapturedThisFrame = true;
        DisableCursor();
      }
    }
    bool hxc_tmp_load_result_n199 = hxc_onTitle;
    bool hxc_tmp_short_circuit_result_n121 = !hxc_tmp_load_result_n199;
    if (!hxc_tmp_load_result_n199)
    {
      hxc_tmp_short_circuit_result_n121 = !hxc_focused;
    }
    bool hxc_tmp_short_circuit_load_result_n201 = hxc_tmp_short_circuit_result_n121;
    bool hxc_tmp_short_circuit_result_n122 = hxc_tmp_short_circuit_load_result_n201;
    if (hxc_tmp_short_circuit_load_result_n201)
    {
      hxc_tmp_short_circuit_result_n122 = hxc_captured;
    }
    if (hxc_tmp_short_circuit_result_n122)
    {
      hxc_paused = true;
      hxc_captured = false;
      hxc_jumpQueued = false;
      EnableCursor();
    }
    bool hxc_tmp_load_result_n204 = hxc_onTitle;
    bool hxc_tmp_short_circuit_result_n123 = !hxc_tmp_load_result_n204;
    if (!hxc_tmp_load_result_n204)
    {
      hxc_tmp_short_circuit_result_n123 = hxc_focused;
    }
    bool hxc_tmp_short_circuit_load_result_n206 = hxc_tmp_short_circuit_result_n123;
    bool hxc_tmp_short_circuit_result_n124 = hxc_tmp_short_circuit_load_result_n206;
    if (hxc_tmp_short_circuit_load_result_n206)
    {
      hxc_tmp_short_circuit_result_n124 = hxc_pausePressed_h63c490f3b83e;
    }
    if (hxc_tmp_short_circuit_result_n124)
    {
      hxc_paused = !hxc_paused;
      hxc_captured = !hxc_paused;
      if (hxc_paused)
      {
        hxc_jumpQueued = false;
      }
      if (hxc_captured)
      {
        DisableCursor();
      }
      else
      {
        EnableCursor();
      }
    }
    bool hxc_tmp_load_result_n213 = hxc_onTitle;
    bool hxc_tmp_short_circuit_result_n125 = !hxc_tmp_load_result_n213;
    if (!hxc_tmp_load_result_n213)
    {
      hxc_tmp_short_circuit_result_n125 = hxc_focused;
    }
    bool hxc_tmp_short_circuit_load_result_n215 = hxc_tmp_short_circuit_result_n125;
    bool hxc_tmp_short_circuit_result_n126 = hxc_tmp_short_circuit_load_result_n215;
    if (hxc_tmp_short_circuit_load_result_n215)
    {
      hxc_tmp_short_circuit_result_n126 = hxc_paused;
    }
    bool hxc_tmp_short_circuit_load_result_n217 = hxc_tmp_short_circuit_result_n126;
    bool hxc_tmp_short_circuit_result_n127 = hxc_tmp_short_circuit_load_result_n217;
    if (hxc_tmp_short_circuit_load_result_n217)
    {
      hxc_tmp_short_circuit_result_n127 = hxc_capturePressed;
    }
    if (hxc_tmp_short_circuit_result_n127)
    {
      hxc_paused = false;
      hxc_captured = true;
      hxc_recapturedThisFrame = true;
      DisableCursor();
    }
    if (hxc_captured)
    {
      double hxc_yawDelta = hxc_lookYaw_h34b6981c82d6;
      if (hxc_yawDelta > 0.25)
      {
        hxc_yawDelta = 0.25;
      }
      if (hxc_yawDelta < -0.25)
      {
        hxc_yawDelta = -0.25;
      }
      double hxc_tmp_load_result_n224 = hxc_lookX;
      double hxc_tmp_load_result_n225 = hxc_yawDelta;
      double hxc_candidateX = hxc_tmp_load_result_n224 + hxc_tmp_load_result_n225 * hxc_lookZ;
      double hxc_tmp_load_result_n227 = hxc_lookZ;
      double hxc_tmp_load_result_n228 = hxc_yawDelta;
      double hxc_candidateZ = hxc_tmp_load_result_n227 - hxc_tmp_load_result_n228 * hxc_lookX;
      double hxc_tmp_load_result_n230 = hxc_candidateX;
      double hxc_tmp_load_result_n231 = hxc_candidateX;
      double hxc_tmp_load_result_n232 = hxc_candidateZ;
      double hxc_lengthSquared = hxc_tmp_load_result_n230 * hxc_tmp_load_result_n231 + hxc_tmp_load_result_n232 * hxc_candidateZ;
      double hxc_normalize = 1.5 - 0.5 * hxc_lengthSquared;
      double hxc_tmp_load_result_n235 = hxc_candidateX;
      hxc_lookX = hxc_tmp_load_result_n235 * hxc_normalize;
      double hxc_tmp_load_result_n237 = hxc_candidateZ;
      hxc_lookZ = hxc_tmp_load_result_n237 * hxc_normalize;
      double hxc_tmp_compound_load_result_n239 = hxc_lookY;
      hxc_lookY = hxc_tmp_compound_load_result_n239 + hxc_lookPitch_h0c3bc7abcfb3;
      if (hxc_lookY > 0.90)
      {
        hxc_lookY = 0.90;
      }
      if (hxc_lookY < -0.90)
      {
        hxc_lookY = -0.90;
      }
      if (hxc_jumpPressed_h2e1d4fd64f1f)
      {
        hxc_jumpQueued = true;
      }
    }
    float hxc_tmp_native_call_result_n244 = GetFrameTime();
    double hxc_frameSeconds = (double)hxc_tmp_native_call_result_n244;
    if (hxc_frameSeconds > 0.25)
    {
      hxc_frameSeconds = 0.25;
    }
    bool hxc_tmp_load_result_n246 = hxc_captured;
    bool hxc_tmp_short_circuit_result_n134 = hxc_tmp_load_result_n246;
    if (hxc_tmp_load_result_n246)
    {
      hxc_tmp_short_circuit_result_n134 = !hxc_recapturedThisFrame;
    }
    bool hxc_tmp_short_circuit_load_result_n248 = hxc_tmp_short_circuit_result_n134;
    bool hxc_tmp_short_circuit_result_n135 = hxc_tmp_short_circuit_load_result_n248;
    if (hxc_tmp_short_circuit_load_result_n248)
    {
      hxc_tmp_short_circuit_result_n135 = hxc_primaryPressed_h103917e2fe17;
    }
    bool hxc_tmp_short_circuit_load_result_n250 = hxc_tmp_short_circuit_result_n135;
    bool hxc_tmp_short_circuit_result_n136 = hxc_tmp_short_circuit_load_result_n250;
    if (hxc_tmp_short_circuit_load_result_n250)
    {
      hxc_tmp_short_circuit_result_n136 = hxc_selectedMode == 1;
    }
    bool hxc_tmp_short_circuit_load_result_n252 = hxc_tmp_short_circuit_result_n136;
    bool hxc_tmp_short_circuit_result_n137 = hxc_tmp_short_circuit_load_result_n252;
    if (hxc_tmp_short_circuit_load_result_n252)
    {
      bool hxc_tmp_call_result_n255 = hxc_caxecraft_gameplay_Inventory_selectedIs(hxc_inventory, hxc_caxecraft_gameplay_ItemKind_CopperSword);
      hxc_tmp_short_circuit_result_n137 = hxc_tmp_call_result_n255;
    }
    if (hxc_tmp_short_circuit_result_n137)
    {
      hxc_swordQueued = true;
    }
    if (!hxc_paused)
    {
      double hxc_tmp_compound_load_result_n258 = hxc_accumulator;
      hxc_accumulator = hxc_tmp_compound_load_result_n258 + hxc_frameSeconds;
    }
    while (1)
    {
      bool hxc_tmp_load_result_n260 = hxc_paused;
      bool hxc_tmp_short_circuit_result_n138 = !hxc_tmp_load_result_n260;
      if (!hxc_tmp_load_result_n260)
      {
        hxc_tmp_short_circuit_result_n138 = hxc_accumulator >= 0.05;
      }
      if (!hxc_tmp_short_circuit_result_n138)
      {
        break;
      }
      struct hxc_caxecraft_domain_WaterSimulation *hxc_tmp_load_result_n263 = hxc_water;
      if (hxc_tmp_load_result_n263 == NULL)
      {
        abort();
      }
      hxc_caxecraft_domain_WaterSimulation_tick(hxc_tmp_load_result_n263, hxc_cells, hxc_tmp_length_n1, hxc_pendingCells, hxc_tmp_length_n3, 64);
      double hxc_tmp_load_result_n264 = hxc_moveForward;
      double hxc_tmp_load_result_n265 = hxc_lookX;
      double hxc_tmp_load_result_n266 = hxc_moveRight;
      double hxc_moveX = hxc_tmp_load_result_n264 * hxc_tmp_load_result_n265 - hxc_tmp_load_result_n266 * hxc_lookZ;
      double hxc_tmp_load_result_n268 = hxc_moveForward;
      double hxc_tmp_load_result_n269 = hxc_lookZ;
      double hxc_tmp_load_result_n270 = hxc_moveRight;
      double hxc_moveZ = hxc_tmp_load_result_n268 * hxc_tmp_load_result_n269 + hxc_tmp_load_result_n270 * hxc_lookX;
      double hxc_tmp_load_result_n272 = hxc_moveForward;
      bool hxc_tmp_short_circuit_result_n141 = hxc_tmp_load_result_n272 != 0.0;
      if (hxc_tmp_load_result_n272 != 0.0)
      {
        hxc_tmp_short_circuit_result_n141 = hxc_moveRight != 0.0;
      }
      if (hxc_tmp_short_circuit_result_n141)
      {
        hxc_moveX = hxc_moveX * 0.7071067811865476;
        hxc_moveZ = hxc_moveZ * 0.7071067811865476;
      }
      bool hxc_drowningThisTick = false;
      if (hxc_vitals.hxc_health > 0)
      {
        struct hxc_caxecraft_domain_PlayerState hxc_tmp_load_result_n278 = hxc_player;
        struct hxc_caxecraft_domain_PlayerAquaticState hxc_tmp_load_result_n279 = hxc_aquatic;
        double hxc_tmp_load_result_n280 = hxc_moveX;
        double hxc_tmp_load_result_n281 = hxc_moveZ;
        bool hxc_tmp_load_result_n282 = hxc_jumpQueued;
        struct hxc_caxecraft_domain_AquaticInput hxc_tmp_call_result_n284 = hxc_caxecraft_domain_PlayerAquatics_input(hxc_tmp_load_result_n280, hxc_tmp_load_result_n281, hxc_tmp_load_result_n282, hxc_descendHeld);
        struct hxc_caxecraft_domain_PlayerAquaticStep hxc_tmp_call_result_n286 = hxc_caxecraft_domain_PlayerAquatics_step(hxc_cells, hxc_tmp_length_n1, hxc_tmp_load_result_n278, hxc_tmp_load_result_n279, hxc_tmp_call_result_n284, hxc_aquaticCapability);
        struct hxc_caxecraft_domain_PlayerAquaticStep hxc_aquaticStep = hxc_tmp_call_result_n286;
        hxc_player = hxc_aquaticStep.hxc_player;
        hxc_aquatic = hxc_aquaticStep.hxc_aquatic;
        hxc_cameraWaterBlend = hxc_aquaticStep.hxc_immersion.hxc_cameraBlend;
        hxc_drowningThisTick = hxc_aquaticStep.hxc_drowningDamage > 0;
      }
      if (hxc_selectedMode == 1)
      {
        if (hxc_vitals.hxc_health > 0)
        {
          struct hxc_caxecraft_gameplay_PlayerVitalsState hxc_tmp_call_result_n294 = hxc_caxecraft_gameplay_PlayerVitals_step(hxc_vitals);
          hxc_vitals = hxc_tmp_call_result_n294;
          struct hxc_caxecraft_gameplay_PlayerVitalsState hxc_tmp_load_result_n295 = hxc_vitals;
          struct hxc_caxecraft_gameplay_PlayerVitalsState hxc_tmp_call_result_n297 = hxc_caxecraft_gameplay_PlayerVitals_applyAttack(hxc_tmp_load_result_n295, hxc_drowningThisTick);
          hxc_vitals = hxc_tmp_call_result_n297;
          struct hxc_caxecraft_gameplay_MosslingState hxc_tmp_load_result_n298 = hxc_mossling;
          double hxc_tmp_record_field_load_result_n299 = hxc_player.hxc_x;
          bool hxc_tmp_call_result_n301 = hxc_caxecraft_gameplay_Mossling_attacksThisTick(hxc_tmp_load_result_n298, hxc_tmp_record_field_load_result_n299, hxc_player.hxc_z);
          bool hxc_mosslingAttacked = hxc_tmp_call_result_n301;
          struct hxc_caxecraft_gameplay_PlayerVitalsState hxc_tmp_load_result_n302 = hxc_vitals;
          struct hxc_caxecraft_gameplay_PlayerVitalsState hxc_tmp_call_result_n304 = hxc_caxecraft_gameplay_PlayerVitals_applyAttack(hxc_tmp_load_result_n302, hxc_mosslingAttacked);
          hxc_vitals = hxc_tmp_call_result_n304;
          if (hxc_mosslingAttacked)
          {
            hxc_enemyAttackFrames = 120;
          }
          struct hxc_caxecraft_gameplay_MosslingState hxc_tmp_load_result_n306 = hxc_mossling;
          double hxc_tmp_record_field_load_result_n307 = hxc_player.hxc_x;
          double hxc_tmp_record_field_load_result_n308 = hxc_player.hxc_z;
          struct hxc_caxecraft_gameplay_MosslingState hxc_tmp_call_result_n310 = hxc_caxecraft_gameplay_Mossling_step(hxc_cells, hxc_tmp_length_n1, hxc_tmp_load_result_n306, hxc_tmp_record_field_load_result_n307, hxc_tmp_record_field_load_result_n308, hxc_updateCount);
          hxc_mossling = hxc_tmp_call_result_n310;
        }
        struct hxc_caxecraft_gameplay_SwordCombatState hxc_tmp_call_result_n312 = hxc_caxecraft_gameplay_SwordCombat_step(hxc_swordCombat);
        hxc_swordCombat = hxc_tmp_call_result_n312;
        if (hxc_swordQueued)
        {
          struct hxc_caxecraft_gameplay_SwordCombatState hxc_tmp_load_result_n314 = hxc_swordCombat;
          struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_load_result_n315 = hxc_inventory;
          struct hxc_caxecraft_gameplay_PlayerVitalsState hxc_tmp_load_result_n316 = hxc_vitals;
          struct hxc_caxecraft_gameplay_MosslingState hxc_tmp_load_result_n317 = hxc_mossling;
          double hxc_tmp_record_field_load_result_n318 = hxc_player.hxc_x;
          double hxc_tmp_record_field_load_result_n319 = hxc_player.hxc_z;
          double hxc_tmp_load_result_n320 = hxc_lookX;
          int32_t hxc_tmp_call_result_n322 = hxc_caxecraft_gameplay_SwordCombat_decide(hxc_tmp_load_result_n314, hxc_tmp_load_result_n315, hxc_tmp_load_result_n316, hxc_tmp_load_result_n317, hxc_tmp_record_field_load_result_n318, hxc_tmp_record_field_load_result_n319, hxc_tmp_load_result_n320, hxc_lookZ);
          int32_t hxc_swordDecision = hxc_tmp_call_result_n322;
          if (hxc_swordDecision == 5)
          {
            struct hxc_caxecraft_gameplay_MosslingState hxc_tmp_call_result_n325 = hxc_caxecraft_gameplay_Mossling_strike(hxc_mossling);
            hxc_mossling = hxc_tmp_call_result_n325;
            hxc_strikeHitFrames = 16;
            if (hxc_mossling.hxc_health <= 0)
            {
              struct hxc_caxecraft_gameplay_BerryDropState hxc_tmp_call_result_n328 = hxc_caxecraft_gameplay_BerryDrop_fromDefeatedMossling(hxc_mossling);
              hxc_berryDrop = hxc_tmp_call_result_n328;
              hxc_enemyDefeatedFrames = 120;
            }
          }
          int32_t hxc_tmp_load_result_n329 = hxc_swordDecision;
          struct hxc_caxecraft_gameplay_SwordCombatState hxc_tmp_call_result_n331 = hxc_caxecraft_gameplay_SwordCombat_after(hxc_tmp_load_result_n329, hxc_swordCombat);
          hxc_swordCombat = hxc_tmp_call_result_n331;
          hxc_swordQueued = false;
        }
      }
      hxc_jumpQueued = false;
      hxc_accumulator = hxc_accumulator - 0.05;
      hxc_updateCount = hxc_i32_add_wrapping(hxc_updateCount, 1);
    }
    double hxc_eyeX = hxc_player.hxc_x;
    double hxc_eyeY = hxc_player.hxc_y + 1.62;
    double hxc_eyeZ = hxc_player.hxc_z;
    double hxc_tmp_load_result_n337 = hxc_eyeX;
    double hxc_tmp_load_result_n338 = hxc_eyeY;
    double hxc_tmp_load_result_n339 = hxc_eyeZ;
    double hxc_tmp_load_result_n340 = hxc_lookX;
    double hxc_tmp_load_result_n341 = hxc_lookY;
    struct hxc_caxecraft_domain_RaycastHit hxc_tmp_call_result_n343 = hxc_caxecraft_domain_VoxelRaycast_trace(hxc_cells, hxc_tmp_length_n1, hxc_tmp_load_result_n337, hxc_tmp_load_result_n338, hxc_tmp_load_result_n339, hxc_tmp_load_result_n340, hxc_tmp_load_result_n341, hxc_lookZ, 7.0);
    struct hxc_caxecraft_domain_RaycastHit hxc_hit = hxc_tmp_call_result_n343;
    bool hxc_tmp_load_result_n344 = hxc_captured;
    bool hxc_tmp_short_circuit_result_n150 = hxc_tmp_load_result_n344;
    if (hxc_tmp_load_result_n344)
    {
      hxc_tmp_short_circuit_result_n150 = !hxc_recapturedThisFrame;
    }
    bool hxc_tmp_short_circuit_load_result_n346 = hxc_tmp_short_circuit_result_n150;
    bool hxc_tmp_short_circuit_result_n151 = hxc_tmp_short_circuit_load_result_n346;
    if (hxc_tmp_short_circuit_load_result_n346)
    {
      hxc_tmp_short_circuit_result_n151 = hxc_primaryPressed_h103917e2fe17;
    }
    if (hxc_tmp_short_circuit_result_n151)
    {
      if (hxc_vitals.hxc_health > 0)
      {
        if (hxc_selectedMode == 1)
        {
          bool hxc_tmp_call_result_n353 = hxc_caxecraft_gameplay_Inventory_selectedIs(hxc_inventory, hxc_caxecraft_gameplay_ItemKind_CopperSword);
          bool hxc_tmp_short_circuit_result_n152 = !hxc_tmp_call_result_n353;
          if (!hxc_tmp_call_result_n353)
          {
            hxc_tmp_short_circuit_result_n152 = hxc_hit.hxc_hit;
          }
          bool hxc_tmp_short_circuit_load_result_n355 = hxc_tmp_short_circuit_result_n152;
          bool hxc_tmp_short_circuit_result_n153 = hxc_tmp_short_circuit_load_result_n355;
          if (hxc_tmp_short_circuit_load_result_n355)
          {
            struct hxc_caxecraft_domain_PlayerAquaticState hxc_tmp_load_result_n356 = hxc_aquatic;
            bool hxc_tmp_call_result_n358 = hxc_caxecraft_domain_PlayerAquatics_canMine(hxc_tmp_load_result_n356, hxc_aquaticCapability);
            hxc_tmp_short_circuit_result_n153 = hxc_tmp_call_result_n358;
          }
          if (hxc_tmp_short_circuit_result_n153)
          {
            int32_t hxc_tmp_record_field_load_result_n360 = hxc_hit.hxc_cellX;
            int32_t hxc_tmp_record_field_load_result_n361 = hxc_hit.hxc_cellY;
            struct hxc_caxecraft_domain_BlockCoord hxc_tmp_call_result_n363 = hxc_caxecraft_domain_World_coord(hxc_tmp_record_field_load_result_n360, hxc_tmp_record_field_load_result_n361, hxc_hit.hxc_cellZ);
            struct hxc_caxecraft_domain_BlockCoord hxc_minedCoordinate = hxc_tmp_call_result_n363;
            struct hxc_caxecraft_domain_BlockCoord hxc_tmp_load_result_n364 = hxc_minedCoordinate;
            struct hxc_caxecraft_gameplay_MiningResult hxc_tmp_call_result_n366 = hxc_caxecraft_gameplay_Mining_attempt(hxc_cells, hxc_tmp_length_n1, hxc_tmp_load_result_n364, hxc_inventory);
            struct hxc_caxecraft_gameplay_MiningResult hxc_mining = hxc_tmp_call_result_n366;
            hxc_inventory = hxc_mining.hxc_inventory;
            if (hxc_mining.hxc_outcome == 2)
            {
              struct hxc_caxecraft_domain_WaterSimulation *hxc_tmp_load_result_n369 = hxc_water;
              if (hxc_tmp_load_result_n369 == NULL)
              {
                abort();
              }
              hxc_caxecraft_domain_WaterSimulation_terrainChanged(hxc_tmp_load_result_n369, hxc_pendingCells, hxc_tmp_length_n3, hxc_minedCoordinate);
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
            int32_t hxc_tmp_record_field_load_result_n373 = hxc_hit.hxc_cellX;
            int32_t hxc_tmp_record_field_load_result_n374 = hxc_hit.hxc_cellY;
            struct hxc_caxecraft_domain_BlockCoord hxc_tmp_call_result_n376 = hxc_caxecraft_domain_World_coord(hxc_tmp_record_field_load_result_n373, hxc_tmp_record_field_load_result_n374, hxc_hit.hxc_cellZ);
            struct hxc_caxecraft_domain_BlockCoord hxc_removedCoordinate = hxc_tmp_call_result_n376;
            struct hxc_caxecraft_domain_WaterSimulation *hxc_tmp_load_result_n377 = hxc_water;
            if (hxc_tmp_load_result_n377 == NULL)
            {
              abort();
            }
            hxc_caxecraft_domain_WaterSimulation_removeTerrain(hxc_tmp_load_result_n377, hxc_cells, hxc_tmp_length_n1, hxc_pendingCells, hxc_tmp_length_n3, hxc_removedCoordinate);
          }
        }
      }
    }
    bool hxc_tmp_load_result_n379 = hxc_captured;
    bool hxc_tmp_short_circuit_result_n157 = hxc_tmp_load_result_n379;
    if (hxc_tmp_load_result_n379)
    {
      hxc_tmp_short_circuit_result_n157 = hxc_secondaryPressed_hb044c653932c;
    }
    if (hxc_tmp_short_circuit_result_n157)
    {
      if (hxc_vitals.hxc_health > 0)
      {
        struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_load_result_n383 = hxc_inventory;
        int32_t hxc_tmp_call_result_n385 = hxc_caxecraft_gameplay_Recovery_decide(hxc_tmp_load_result_n383, hxc_vitals);
        int32_t hxc_recoveryDecision = hxc_tmp_call_result_n385;
        if (hxc_recoveryDecision != 0)
        {
          hxc_recoveryFeedback = hxc_recoveryDecision;
          hxc_recoveryFeedbackFrames = 90;
          int32_t hxc_tmp_load_result_n388 = hxc_recoveryDecision;
          struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_call_result_n390 = hxc_caxecraft_gameplay_Recovery_applyInventory(hxc_tmp_load_result_n388, hxc_inventory);
          hxc_inventory = hxc_tmp_call_result_n390;
          int32_t hxc_tmp_load_result_n391 = hxc_recoveryDecision;
          struct hxc_caxecraft_gameplay_PlayerVitalsState hxc_tmp_call_result_n393 = hxc_caxecraft_gameplay_Recovery_applyVitals(hxc_tmp_load_result_n391, hxc_vitals);
          hxc_vitals = hxc_tmp_call_result_n393;
        }
        else
        {
          if (hxc_hit.hxc_hit)
          {
            int32_t hxc_tmp_record_field_load_result_n395 = hxc_hit.hxc_previousX;
            int32_t hxc_tmp_record_field_load_result_n396 = hxc_hit.hxc_previousY;
            struct hxc_caxecraft_domain_BlockCoord hxc_tmp_call_result_n398 = hxc_caxecraft_domain_World_coord(hxc_tmp_record_field_load_result_n395, hxc_tmp_record_field_load_result_n396, hxc_hit.hxc_previousZ);
            struct hxc_caxecraft_domain_BlockCoord hxc_placement = hxc_tmp_call_result_n398;
            enum hxc_caxecraft_domain_BlockKind hxc_tmp_call_result_n400 = hxc_caxecraft_gameplay_Inventory_selectedBlock(hxc_inventory);
            enum hxc_caxecraft_domain_BlockKind hxc_selectedBlock = hxc_tmp_call_result_n400;
            struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_load_result_n401 = hxc_inventory;
            int32_t hxc_tmp_call_result_n403 = hxc_caxecraft_gameplay_Inventory_countAt(hxc_tmp_load_result_n401, hxc_inventory.hxc_selected);
            bool hxc_hasItem = hxc_tmp_call_result_n403 > 0;
            bool hxc_tmp_load_result_n404 = hxc_hasItem;
            bool hxc_tmp_short_circuit_result_n162 = !hxc_tmp_load_result_n404;
            if (!!hxc_tmp_load_result_n404)
            {
              bool hxc_tmp_call_result_n406 = hxc_caxecraft_domain_World_isPlaceable(hxc_selectedBlock);
              hxc_tmp_short_circuit_result_n162 = !hxc_tmp_call_result_n406;
            }
            bool hxc_tmp_short_circuit_load_result_n407 = hxc_tmp_short_circuit_result_n162;
            bool hxc_tmp_short_circuit_result_n163 = hxc_tmp_short_circuit_load_result_n407;
            if (!hxc_tmp_short_circuit_load_result_n407)
            {
              struct hxc_caxecraft_domain_PlayerState hxc_tmp_load_result_n408 = hxc_player;
              bool hxc_tmp_call_result_n410 = hxc_caxecraft_domain_PlayerPhysics_canPlaceAt(hxc_tmp_load_result_n408, hxc_placement);
              hxc_tmp_short_circuit_result_n163 = !hxc_tmp_call_result_n410;
            }
            bool hxc_tmp_short_circuit_load_result_n411 = hxc_tmp_short_circuit_result_n163;
            bool hxc_tmp_short_circuit_result_n164 = hxc_tmp_short_circuit_load_result_n411;
            if (!hxc_tmp_short_circuit_load_result_n411)
            {
              struct hxc_caxecraft_domain_WaterSimulation *hxc_tmp_load_result_n412 = hxc_water;
              if (hxc_tmp_load_result_n412 == NULL)
              {
                abort();
              }
              struct hxc_caxecraft_domain_BlockCoord hxc_tmp_load_result_n413 = hxc_placement;
              bool hxc_tmp_instance_call_result_n415 = hxc_caxecraft_domain_WaterSimulation_placeTerrain(hxc_tmp_load_result_n412, hxc_cells, hxc_tmp_length_n1, hxc_pendingCells, hxc_tmp_length_n3, hxc_tmp_load_result_n413, hxc_selectedBlock);
              hxc_tmp_short_circuit_result_n164 = !hxc_tmp_instance_call_result_n415;
            }
            if (hxc_tmp_short_circuit_result_n164)
            {
              hxc_placementBlockedFrames = 60;
            }
            else
            {
              if (hxc_selectedMode == 1)
              {
                struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_call_result_n419 = hxc_caxecraft_gameplay_Inventory_consumeSelected(hxc_inventory);
                hxc_inventory = hxc_tmp_call_result_n419;
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
    if (!hxc_paused)
    {
      struct hxc_caxecraft_gameplay_BerryDropState hxc_tmp_load_result_n423 = hxc_berryDrop;
      double hxc_tmp_record_field_load_result_n424 = hxc_player.hxc_x;
      double hxc_tmp_record_field_load_result_n425 = hxc_player.hxc_y;
      bool hxc_tmp_call_result_n427 = hxc_caxecraft_gameplay_BerryDrop_isInRange(hxc_tmp_load_result_n423, hxc_tmp_record_field_load_result_n424, hxc_tmp_record_field_load_result_n425, hxc_player.hxc_z);
      if (hxc_tmp_call_result_n427)
      {
        struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_load_result_n428 = hxc_inventory;
        int32_t hxc_tmp_call_result_n431 = hxc_caxecraft_gameplay_Inventory_acceptedAmount(hxc_tmp_load_result_n428, hxc_caxecraft_gameplay_ItemKind_Berries, hxc_berryDrop.hxc_amount);
        int32_t hxc_acceptedDrop = hxc_tmp_call_result_n431;
        if (hxc_acceptedDrop > 0)
        {
          struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_load_result_n433 = hxc_inventory;
          struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_call_result_n436 = hxc_caxecraft_gameplay_Inventory_collectItem(hxc_tmp_load_result_n433, hxc_caxecraft_gameplay_ItemKind_Berries, hxc_acceptedDrop);
          hxc_inventory = hxc_tmp_call_result_n436;
          struct hxc_caxecraft_gameplay_BerryDropState hxc_tmp_load_result_n437 = hxc_berryDrop;
          struct hxc_caxecraft_gameplay_BerryDropState hxc_tmp_call_result_n439 = hxc_caxecraft_gameplay_BerryDrop_collectAmount(hxc_tmp_load_result_n437, hxc_acceptedDrop);
          hxc_berryDrop = hxc_tmp_call_result_n439;
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
    double hxc_tmp_load_result_n454 = hxc_eyeX;
    double hxc_tmp_load_result_n455 = hxc_eyeY;
    double hxc_tmp_load_result_n456 = hxc_eyeZ;
    double hxc_tmp_load_result_n458 = hxc_eyeX;
    double hxc_tmp_load_result_n459 = hxc_lookX;
    double hxc_tmp_load_result_n460 = hxc_eyeY;
    double hxc_tmp_load_result_n461 = hxc_lookY;
    double hxc_tmp_load_result_n462 = hxc_eyeZ;
    struct Camera3D hxc_camera = (struct Camera3D){ .position = (struct Vector3){ .x = (float)hxc_tmp_load_result_n454, .y = (float)hxc_tmp_load_result_n455, .z = (float)hxc_tmp_load_result_n456 }, .target = (struct Vector3){ .x = (float)(hxc_tmp_load_result_n458 + hxc_tmp_load_result_n459), .y = (float)(hxc_tmp_load_result_n460 + hxc_tmp_load_result_n461), .z = (float)(hxc_tmp_load_result_n462 + hxc_lookZ) }, .up = (struct Vector3){ .x = (float)0.0, .y = (float)1.0, .z = (float)0.0 }, .fovy = (float)70.0, .projection = 0 };
    BeginDrawing();
    if (hxc_onTitle)
    {
      int32_t hxc_tmp_native_call_result_n468 = GetScreenWidth();
      int32_t hxc_width_h1584a07854a1 = (int32_t)hxc_tmp_native_call_result_n468;
      int32_t hxc_tmp_native_call_result_n469 = GetScreenHeight();
      int32_t hxc_height_h33c3d217266c = (int32_t)hxc_tmp_native_call_result_n469;
      if (hxc_titleTextureReady)
      {
        struct Color hxc_this1_h45e7cd6f2d61 = (struct Color){ .r = 255, .g = 255, .b = 255, .a = 255 };
        struct Texture hxc_tmp_load_result_n472 = hxc_titleTexture;
        double hxc_imageAspect = hxc_f64_divide_zero_safe((double)(int32_t)hxc_tmp_load_result_n472.width, (double)(int32_t)hxc_titleTexture.height);
        int32_t hxc_tmp_load_result_n476 = hxc_width_h1584a07854a1;
        double hxc_screenAspect = hxc_f64_divide_zero_safe((double)hxc_tmp_load_result_n476, (double)hxc_height_h33c3d217266c);
        double hxc_sourceX = 0.0;
        double hxc_sourceY = 0.0;
        double hxc_sourceWidth = (double)(int32_t)hxc_titleTexture.width;
        double hxc_sourceHeight = (double)(int32_t)hxc_titleTexture.height;
        double hxc_tmp_load_result_n482 = hxc_screenAspect;
        if (hxc_tmp_load_result_n482 > hxc_imageAspect)
        {
          struct Texture hxc_tmp_load_result_n484 = hxc_titleTexture;
          hxc_sourceHeight = hxc_f64_divide_zero_safe((double)(int32_t)hxc_tmp_load_result_n484.width, hxc_screenAspect);
          struct Texture hxc_tmp_load_result_n487 = hxc_titleTexture;
          hxc_sourceY = ((double)(int32_t)hxc_tmp_load_result_n487.height - hxc_sourceHeight) * 0.5;
        }
        else
        {
          struct Texture hxc_tmp_load_result_n490 = hxc_titleTexture;
          hxc_sourceWidth = (double)(int32_t)hxc_tmp_load_result_n490.height * hxc_screenAspect;
          struct Texture hxc_tmp_load_result_n493 = hxc_titleTexture;
          hxc_sourceX = ((double)(int32_t)hxc_tmp_load_result_n493.width - hxc_sourceWidth) * 0.5;
        }
        struct Texture hxc_tmp_load_result_n496 = hxc_titleTexture;
        double hxc_tmp_load_result_n497 = hxc_sourceX;
        double hxc_tmp_load_result_n498 = hxc_sourceY;
        double hxc_tmp_load_result_n499 = hxc_sourceWidth;
        double hxc_tmp_load_result_n500 = hxc_sourceHeight;
        int32_t hxc_tmp_load_result_n502 = hxc_width_h1584a07854a1;
        int32_t hxc_tmp_load_result_n503 = hxc_height_h33c3d217266c;
        DrawTexturePro(hxc_tmp_load_result_n496, (struct Rectangle){ .x = (float)hxc_tmp_load_result_n497, .y = (float)hxc_tmp_load_result_n498, .width = (float)hxc_tmp_load_result_n499, .height = (float)hxc_tmp_load_result_n500 }, (struct Rectangle){ .x = (float)0.0, .y = (float)0.0, .width = (float)((double)hxc_tmp_load_result_n502 + 0.0), .height = (float)((double)hxc_tmp_load_result_n503 + 0.0) }, (struct Vector2){ .x = (float)0.0, .y = (float)0.0 }, (float)0.0, hxc_this1_h45e7cd6f2d61);
      }
      else
      {
        ClearBackground((struct Color){ .r = 126, .g = 190, .b = 201, .a = 255 });
      }
      struct Color hxc_this1_h00d59f9d7c58 = (struct Color){ .r = 4, .g = 14, .b = 23, .a = 132 };
      int32_t hxc_tmp_load_result_n509 = hxc_width_h1584a07854a1;
      int32_t hxc_tmp_load_result_n510 = hxc_height_h33c3d217266c;
      DrawRectangle((int32_t)0, (int32_t)0, (int32_t)hxc_tmp_load_result_n509, (int32_t)hxc_tmp_load_result_n510, hxc_this1_h00d59f9d7c58);
      if (hxc_wordmarkTextureReady)
      {
        int32_t hxc_centerX = hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_width_h1584a07854a1, (double)2));
        struct Color hxc_this1_h8d4a171cde40 = (struct Color){ .r = 255, .g = 255, .b = 255, .a = 255 };
        double hxc_scale = hxc_f64_divide_zero_safe((double)560, (double)(int32_t)hxc_wordmarkTexture.width);
        double hxc_heightScale = hxc_f64_divide_zero_safe((double)132, (double)(int32_t)hxc_wordmarkTexture.height);
        double hxc_tmp_load_result_n519 = hxc_heightScale;
        if (hxc_tmp_load_result_n519 < hxc_scale)
        {
          hxc_scale = hxc_heightScale;
        }
        struct Texture hxc_tmp_load_result_n522 = hxc_wordmarkTexture;
        double hxc_width_h9e241b729c86 = (double)(int32_t)hxc_tmp_load_result_n522.width * hxc_scale;
        struct Texture hxc_tmp_load_result_n525 = hxc_wordmarkTexture;
        double hxc_height_h83949345ac1d = (double)(int32_t)hxc_tmp_load_result_n525.height * hxc_scale;
        double hxc_width1 = (double)(int32_t)hxc_wordmarkTexture.width + 0.0;
        double hxc_height1 = (double)(int32_t)hxc_wordmarkTexture.height + 0.0;
        struct Texture hxc_tmp_load_result_n532 = hxc_wordmarkTexture;
        double hxc_tmp_load_result_n533 = hxc_width1;
        double hxc_tmp_load_result_n534 = hxc_height1;
        int32_t hxc_tmp_load_result_n536 = hxc_centerX;
        double hxc_tmp_load_result_n537 = hxc_width_h9e241b729c86;
        double hxc_tmp_load_result_n538 = hxc_width_h9e241b729c86;
        double hxc_tmp_load_result_n539 = hxc_height_h83949345ac1d;
        DrawTexturePro(hxc_tmp_load_result_n532, (struct Rectangle){ .x = (float)0.0, .y = (float)0.0, .width = (float)hxc_tmp_load_result_n533, .height = (float)hxc_tmp_load_result_n534 }, (struct Rectangle){ .x = (float)((double)hxc_tmp_load_result_n536 - hxc_tmp_load_result_n537 * 0.5), .y = (float)28.0, .width = (float)hxc_tmp_load_result_n538, .height = (float)hxc_tmp_load_result_n539 }, (struct Vector2){ .x = (float)0.0, .y = (float)0.0 }, (float)0.0, hxc_this1_h8d4a171cde40);
      }
      else
      {
        int32_t hxc_tmp_load_result_n543 = hxc_locale;
        hxc_caxecraft_localization_UiCatalog_draw(hxc_tmp_load_result_n543, 16, hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_width_h1584a07854a1, (double)2)), 92), 52, 34, (struct Color){ .r = 242, .g = 249, .b = 245, .a = 255 });
      }
      int32_t hxc_firstTop = hxc_i32_subtract_wrapping(hxc_height_h33c3d217266c, 225);
      int32_t hxc_left_hb55d1a887bd9 = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_width_h1584a07854a1, (double)2)), 180);
      if (hxc_selectedMode == 0)
      {
        struct Color hxc_this1_h383548821bce = (struct Color){ .r = 16, .g = 88, .b = 102, .a = 232 };
        int32_t hxc_tmp_load_result_n550 = hxc_left_hb55d1a887bd9;
        int32_t hxc_tmp_load_result_n551 = hxc_firstTop;
        DrawRectangle((int32_t)hxc_tmp_load_result_n550, (int32_t)hxc_tmp_load_result_n551, (int32_t)360, (int32_t)54, hxc_this1_h383548821bce);
        struct Color hxc_this1_hb0ed34032af3 = (struct Color){ .r = 255, .g = 190, .b = 55, .a = 255 };
        int32_t hxc_tmp_load_result_n554 = hxc_left_hb55d1a887bd9;
        int32_t hxc_tmp_load_result_n555 = hxc_firstTop;
        DrawRectangleLines((int32_t)hxc_tmp_load_result_n554, (int32_t)hxc_tmp_load_result_n555, (int32_t)360, (int32_t)54, hxc_this1_hb0ed34032af3);
      }
      else
      {
        struct Color hxc_this1_hac44919a9119 = (struct Color){ .r = 6, .g = 26, .b = 36, .a = 218 };
        int32_t hxc_tmp_load_result_n558 = hxc_left_hb55d1a887bd9;
        int32_t hxc_tmp_load_result_n559 = hxc_firstTop;
        DrawRectangle((int32_t)hxc_tmp_load_result_n558, (int32_t)hxc_tmp_load_result_n559, (int32_t)360, (int32_t)54, hxc_this1_hac44919a9119);
        struct Color hxc_this1_he55cb32edb09 = (struct Color){ .r = 92, .g = 194, .b = 188, .a = 255 };
        int32_t hxc_tmp_load_result_n562 = hxc_left_hb55d1a887bd9;
        int32_t hxc_tmp_load_result_n563 = hxc_firstTop;
        DrawRectangleLines((int32_t)hxc_tmp_load_result_n562, (int32_t)hxc_tmp_load_result_n563, (int32_t)360, (int32_t)54, hxc_this1_he55cb32edb09);
      }
      int32_t hxc_tmp_load_result_n565 = hxc_locale;
      int32_t hxc_tmp_load_result_n566 = hxc_left_hb55d1a887bd9;
      hxc_caxecraft_localization_UiCatalog_draw(hxc_tmp_load_result_n565, 10, hxc_i32_add_wrapping(hxc_tmp_load_result_n566, 28), hxc_i32_add_wrapping(hxc_firstTop, 16), 21, (struct Color){ .r = 242, .g = 249, .b = 245, .a = 255 });
      int32_t hxc_top = hxc_i32_add_wrapping(hxc_i32_add_wrapping(hxc_firstTop, 54), 14);
      int32_t hxc_left_h97528466f741 = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_width_h1584a07854a1, (double)2)), 180);
      if (hxc_selectedMode == 1)
      {
        struct Color hxc_this1_h80dbf65504c7 = (struct Color){ .r = 16, .g = 88, .b = 102, .a = 232 };
        int32_t hxc_tmp_load_result_n573 = hxc_left_h97528466f741;
        int32_t hxc_tmp_load_result_n574 = hxc_top;
        DrawRectangle((int32_t)hxc_tmp_load_result_n573, (int32_t)hxc_tmp_load_result_n574, (int32_t)360, (int32_t)54, hxc_this1_h80dbf65504c7);
        struct Color hxc_this1_h7809470c02a5 = (struct Color){ .r = 255, .g = 190, .b = 55, .a = 255 };
        int32_t hxc_tmp_load_result_n577 = hxc_left_h97528466f741;
        int32_t hxc_tmp_load_result_n578 = hxc_top;
        DrawRectangleLines((int32_t)hxc_tmp_load_result_n577, (int32_t)hxc_tmp_load_result_n578, (int32_t)360, (int32_t)54, hxc_this1_h7809470c02a5);
      }
      else
      {
        struct Color hxc_this1_h5da40a40a706 = (struct Color){ .r = 6, .g = 26, .b = 36, .a = 218 };
        int32_t hxc_tmp_load_result_n581 = hxc_left_h97528466f741;
        int32_t hxc_tmp_load_result_n582 = hxc_top;
        DrawRectangle((int32_t)hxc_tmp_load_result_n581, (int32_t)hxc_tmp_load_result_n582, (int32_t)360, (int32_t)54, hxc_this1_h5da40a40a706);
        struct Color hxc_this1_hba30c56da08c = (struct Color){ .r = 92, .g = 194, .b = 188, .a = 255 };
        int32_t hxc_tmp_load_result_n585 = hxc_left_h97528466f741;
        int32_t hxc_tmp_load_result_n586 = hxc_top;
        DrawRectangleLines((int32_t)hxc_tmp_load_result_n585, (int32_t)hxc_tmp_load_result_n586, (int32_t)360, (int32_t)54, hxc_this1_hba30c56da08c);
      }
      int32_t hxc_tmp_load_result_n588 = hxc_locale;
      int32_t hxc_tmp_load_result_n589 = hxc_left_h97528466f741;
      hxc_caxecraft_localization_UiCatalog_draw(hxc_tmp_load_result_n588, 9, hxc_i32_add_wrapping(hxc_tmp_load_result_n589, 28), hxc_i32_add_wrapping(hxc_top, 16), 21, (struct Color){ .r = 242, .g = 249, .b = 245, .a = 255 });
      int32_t hxc_tmp_load_result_n592 = hxc_locale;
      int32_t hxc_tmp_load_result_n593 = hxc_width_h1584a07854a1;
      hxc_caxecraft_localization_UiCatalog_draw(hxc_tmp_load_result_n592, 11, hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_tmp_load_result_n593, (double)2)), 285), hxc_i32_subtract_wrapping(hxc_height_h33c3d217266c, 58), 16, (struct Color){ .r = 229, .g = 241, .b = 235, .a = 255 });
      if (hxc_selectedMode == 1)
      {
        int32_t hxc_tmp_load_result_n597 = hxc_locale;
        int32_t hxc_tmp_load_result_n598 = hxc_width_h1584a07854a1;
        hxc_caxecraft_localization_FirstPlayableCatalog_draw(hxc_tmp_load_result_n597, 1, hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_tmp_load_result_n598, (double)2)), 230), hxc_i32_subtract_wrapping(hxc_firstTop, 36), 17, (struct Color){ .r = 255, .g = 205, .b = 91, .a = 255 });
      }
    }
    else
    {
      ClearBackground((struct Color){ .r = 126, .g = 190, .b = 201, .a = 255 });
      int32_t hxc_tmp_native_call_result_n602 = GetScreenWidth();
      int32_t hxc_sunX = hxc_i32_subtract_wrapping((int32_t)hxc_tmp_native_call_result_n602, 260);
      float hxc_radius_hfce059f48f78 = (float)42.0;
      struct Color hxc_this1_h01aabed7e28a = (struct Color){ .r = 255, .g = 226, .b = 136, .a = 255 };
      int32_t hxc_tmp_load_result_n604 = hxc_sunX;
      float hxc_tmp_load_result_n605 = hxc_radius_hfce059f48f78;
      DrawCircle((int32_t)hxc_tmp_load_result_n604, (int32_t)86, hxc_tmp_load_result_n605, hxc_this1_h01aabed7e28a);
      float hxc_radius_h1ef0942844b9 = (float)30.0;
      struct Color hxc_this1_h15cae6f38156 = (struct Color){ .r = 255, .g = 246, .b = 205, .a = 255 };
      int32_t hxc_tmp_load_result_n608 = hxc_sunX;
      float hxc_tmp_load_result_n609 = hxc_radius_h1ef0942844b9;
      DrawCircle((int32_t)hxc_tmp_load_result_n608, (int32_t)86, hxc_tmp_load_result_n609, hxc_this1_h15cae6f38156);
      BeginMode3D(hxc_camera);
      struct Texture hxc_tmp_load_result_n612 = hxc_terrainTexture;
      bool hxc_tmp_load_result_n613 = hxc_terrainTextureReady;
      double hxc_tmp_record_field_load_result_n614 = hxc_player.hxc_x;
      struct hxc_caxecraft_app_TerrainRenderCounters hxc_tmp_call_result_n616 = hxc_caxecraft_app_TerrainRenderer_draw(hxc_cells, hxc_tmp_length_n1, hxc_tmp_load_result_n612, hxc_tmp_load_result_n613, hxc_tmp_record_field_load_result_n614, hxc_player.hxc_z);
      struct hxc_caxecraft_app_TerrainRenderCounters hxc_renderCounters = hxc_tmp_call_result_n616;
      struct Texture hxc_tmp_load_result_n617 = hxc_terrainTexture;
      bool hxc_tmp_load_result_n618 = hxc_terrainTextureReady;
      struct hxc_caxecraft_app_TerrainRenderCounters hxc_tmp_call_result_n620 = hxc_caxecraft_app_WaterRenderer_draw(hxc_cells, hxc_tmp_length_n1, hxc_tmp_load_result_n617, hxc_tmp_load_result_n618, hxc_waterPresentationCell);
      struct hxc_caxecraft_app_TerrainRenderCounters hxc_waterCounters = hxc_tmp_call_result_n620;
      int32_t hxc_tmp_record_field_load_result_n621 = hxc_renderCounters.hxc_visible;
      int32_t hxc_totalVisible = hxc_i32_add_wrapping(hxc_tmp_record_field_load_result_n621, hxc_waterCounters.hxc_visible);
      int32_t hxc_tmp_record_field_load_result_n623 = hxc_renderCounters.hxc_drawCalls;
      int32_t hxc_totalDrawCalls = hxc_i32_add_wrapping(hxc_tmp_record_field_load_result_n623, hxc_waterCounters.hxc_drawCalls);
      struct Camera3D hxc_tmp_load_result_n625 = hxc_camera;
      struct Texture hxc_tmp_load_result_n626 = hxc_entityTexture;
      bool hxc_tmp_load_result_n627 = hxc_entityTextureReady;
      struct hxc_caxecraft_gameplay_GuideState hxc_tmp_load_result_n628 = hxc_guide;
      struct hxc_caxecraft_gameplay_MosslingState hxc_tmp_load_result_n629 = hxc_mossling;
      hxc_caxecraft_app_Main_drawActors(hxc_tmp_load_result_n625, hxc_tmp_load_result_n626, hxc_tmp_load_result_n627, hxc_tmp_load_result_n628, hxc_tmp_load_result_n629, hxc_berryDrop);
      if (hxc_hit.hxc_hit)
      {
        double hxc_y = (double)hxc_hit.hxc_cellY + 0.5;
        double hxc_z = (double)hxc_hit.hxc_cellZ + 0.5;
        int32_t hxc_tmp_record_field_load_result_n634 = hxc_hit.hxc_cellX;
        double hxc_tmp_load_result_n635 = hxc_y;
        DrawCubeWires((struct Vector3){ .x = (float)((double)hxc_tmp_record_field_load_result_n634 + 0.5), .y = (float)hxc_tmp_load_result_n635, .z = (float)hxc_z }, (float)1.04, (float)1.04, (float)1.04, (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 });
      }
      EndMode3D();
      if (hxc_cameraWaterBlend > 0.0)
      {
        int32_t hxc_overlayAlpha = hxc_f64_to_i32_saturating(105.0 * hxc_cameraWaterBlend);
        int32_t hxc_tmp_native_call_result_n641 = GetScreenWidth();
        int32_t hxc_width_hf73249a900ff = (int32_t)hxc_tmp_native_call_result_n641;
        int32_t hxc_tmp_native_call_result_n642 = GetScreenHeight();
        int32_t hxc_height_hdcd90abc61bf = (int32_t)hxc_tmp_native_call_result_n642;
        uint8_t hxc_red = (uint8_t)31;
        uint8_t hxc_green = (uint8_t)115;
        uint8_t hxc_blue = (uint8_t)154;
        int32_t hxc_tmp_load_result_n643 = hxc_overlayAlpha;
        int32_t hxc_tmp_conditional_result_n215 = 0;
        if (hxc_tmp_load_result_n643 < 0)
        {
          hxc_tmp_conditional_result_n215 = 0;
        }
        else
        {
          int32_t hxc_tmp_load_result_n644 = hxc_overlayAlpha;
          int32_t hxc_tmp_conditional_result_n216 = 0;
          if (hxc_tmp_load_result_n644 > 255)
          {
            hxc_tmp_conditional_result_n216 = 255;
          }
          else
          {
            hxc_tmp_conditional_result_n216 = hxc_overlayAlpha;
          }
          hxc_tmp_conditional_result_n215 = hxc_tmp_conditional_result_n216;
        }
        uint8_t hxc_alpha = (uint8_t)hxc_tmp_conditional_result_n215;
        uint8_t hxc_tmp_load_result_n648 = hxc_red;
        uint8_t hxc_tmp_load_result_n649 = hxc_green;
        uint8_t hxc_tmp_load_result_n650 = hxc_blue;
        struct Color hxc_this1_h769edf4631d3 = (struct Color){ .r = hxc_tmp_load_result_n648, .g = hxc_tmp_load_result_n649, .b = hxc_tmp_load_result_n650, .a = hxc_alpha };
        int32_t hxc_tmp_load_result_n653 = hxc_width_hf73249a900ff;
        int32_t hxc_tmp_load_result_n654 = hxc_height_hdcd90abc61bf;
        DrawRectangle((int32_t)0, (int32_t)0, (int32_t)hxc_tmp_load_result_n653, (int32_t)hxc_tmp_load_result_n654, hxc_this1_h769edf4631d3);
      }
      int32_t hxc_tmp_load_result_n656 = hxc_totalVisible;
      int32_t hxc_tmp_load_result_n657 = hxc_totalDrawCalls;
      int32_t hxc_tmp_load_result_n658 = hxc_frameCount;
      int32_t hxc_tmp_load_result_n659 = hxc_updateCount;
      bool hxc_tmp_load_result_n660 = hxc_paused;
      bool hxc_tmp_load_result_n661 = hxc_captured;
      int32_t hxc_tmp_load_result_n662 = hxc_placementBlockedFrames;
      struct hxc_caxecraft_domain_RaycastHit hxc_tmp_load_result_n663 = hxc_hit;
      double hxc_tmp_record_field_load_result_n664 = hxc_player.hxc_x;
      double hxc_tmp_record_field_load_result_n665 = hxc_player.hxc_z;
      int32_t hxc_tmp_load_result_n666 = hxc_selectedMode;
      int32_t hxc_tmp_load_result_n667 = hxc_locale;
      struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_load_result_n668 = hxc_inventory;
      struct hxc_caxecraft_gameplay_GuideState hxc_tmp_load_result_n669 = hxc_guide;
      struct hxc_caxecraft_gameplay_MosslingState hxc_tmp_load_result_n670 = hxc_mossling;
      struct hxc_caxecraft_gameplay_PlayerVitalsState hxc_tmp_load_result_n671 = hxc_vitals;
      int32_t hxc_tmp_load_result_n672 = hxc_strikeHitFrames;
      int32_t hxc_tmp_load_result_n673 = hxc_enemyDefeatedFrames;
      int32_t hxc_tmp_load_result_n674 = hxc_enemyAttackFrames;
      int32_t hxc_tmp_load_result_n675 = hxc_pickupFrames;
      int32_t hxc_tmp_load_result_n676 = hxc_pickupAmount;
      int32_t hxc_tmp_load_result_n677 = hxc_inventoryFullReason;
      int32_t hxc_tmp_load_result_n678 = hxc_recoveryFeedback;
      int32_t hxc_tmp_load_result_n679 = hxc_recoveryFeedbackFrames;
      struct Texture hxc_tmp_load_result_n680 = hxc_hudTexture;
      bool hxc_tmp_load_result_n681 = hxc_hudTextureReady;
      struct Texture hxc_tmp_load_result_n682 = hxc_itemTexture;
      bool hxc_tmp_load_result_n683 = hxc_itemTextureReady;
      bool hxc_tmp_record_field_load_result_n684 = hxc_aquatic.hxc_headSubmerged;
      int32_t hxc_tmp_record_field_load_result_n685 = hxc_aquatic.hxc_breathTicks;
      hxc_caxecraft_app_Main_drawHud(hxc_tmp_load_result_n656, hxc_tmp_load_result_n657, hxc_tmp_load_result_n658, hxc_tmp_load_result_n659, hxc_tmp_load_result_n660, hxc_tmp_load_result_n661, hxc_tmp_load_result_n662 > 0, hxc_tmp_load_result_n663, hxc_tmp_record_field_load_result_n664, hxc_tmp_record_field_load_result_n665, hxc_tmp_load_result_n666, hxc_tmp_load_result_n667, hxc_tmp_load_result_n668, hxc_tmp_load_result_n669, hxc_tmp_load_result_n670, hxc_tmp_load_result_n671, hxc_tmp_load_result_n672 > 0, hxc_tmp_load_result_n673 > 0, hxc_tmp_load_result_n674 > 0, hxc_tmp_load_result_n675 > 0, hxc_tmp_load_result_n676, hxc_tmp_load_result_n677, hxc_tmp_load_result_n678, hxc_tmp_load_result_n679 > 0, hxc_tmp_load_result_n680, hxc_tmp_load_result_n681, hxc_tmp_load_result_n682, hxc_tmp_load_result_n683, hxc_tmp_record_field_load_result_n684, hxc_tmp_record_field_load_result_n685, hxc_aquaticCapability.hxc_maximumBreathTicks);
    }
    EndDrawing();
    hxc_frameCount = hxc_i32_add_wrapping(hxc_frameCount, 1);
  }
  EnableCursor();
  if (hxc_terrainTextureReady)
  {
    UnloadTexture(hxc_terrainTexture);
  }
  if (hxc_entityTextureReady)
  {
    UnloadTexture(hxc_entityTexture);
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

struct hxc_caxecraft_domain_PlayerState hxc_caxecraft_app_Main_spawnPlayer(uint8_t *hxc_cells, size_t hxc_length)
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
  struct hxc_caxecraft_domain_PlayerState hxc_tmp_call_result_n3 = hxc_caxecraft_domain_PlayerPhysics_player(hxc_tmp_load_result_n0, hxc_tmp_load_result_n1, hxc_spawnZ);
  struct hxc_caxecraft_domain_PlayerState hxc_tmp_call_result_n4 = hxc_caxecraft_domain_PlayerPhysics_recoverSpawn(hxc_cells, hxc_length, hxc_tmp_call_result_n3);
  return hxc_tmp_call_result_n4;
}
