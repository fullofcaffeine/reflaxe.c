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

void hxc_caxecraft_app_Main_drawHud(int32_t hxc_visible, int32_t hxc_drawCalls, int32_t hxc_frames, int32_t hxc_updates, bool hxc_paused, bool hxc_captured, bool hxc_placementBlocked, struct hxc_caxecraft_domain_RaycastHit hxc_hit, double hxc_playerX, double hxc_playerZ, int32_t hxc_mode, int32_t hxc_locale, struct hxc_caxecraft_gameplay_InventoryState hxc_inventory, struct hxc_caxecraft_gameplay_GuideState hxc_guide, struct hxc_caxecraft_gameplay_MosslingState hxc_mossling, struct hxc_caxecraft_gameplay_PlayerVitalsState hxc_vitals, bool hxc_strikeHit, bool hxc_enemyDefeated, bool hxc_enemyAttacked, bool hxc_pickedUp, int32_t hxc_pickupAmount, int32_t hxc_inventoryFullReason, int32_t hxc_recoveryFeedback, bool hxc_recoveryVisible, struct Texture hxc_hudTexture, bool hxc_hudTextureReady, struct Texture hxc_itemTexture, bool hxc_itemTextureReady)
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
  struct Color hxc_this1_h2eb2503632b1 = (struct Color){ .r = 15, .g = 27, .b = 39, .a = 220 };
  DrawRectangle((int32_t)18, (int32_t)18, (int32_t)460, (int32_t)108, hxc_this1_h2eb2503632b1);
  struct Color hxc_this1_h19d837ac740a = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
  DrawRectangleLines((int32_t)18, (int32_t)18, (int32_t)460, (int32_t)108, hxc_this1_h19d837ac740a);
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
  int32_t hxc_tmp_load_result_n43 = hxc_height;
  hxc_caxecraft_localization_UiCatalog_draw(hxc_locale, 2, 20, hxc_i32_subtract_wrapping(hxc_tmp_load_result_n43, 22), 14, hxc_text);
  if (hxc_mode == 1)
  {
    hxc_caxecraft_localization_FirstPlayableCatalog_draw(hxc_locale, 0, 32, 110, 14, (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 });
  }
  bool hxc_tmp_call_result_n46 = hxc_caxecraft_gameplay_GuideNpc_isInRange(hxc_guide, hxc_playerX, hxc_playerZ);
  if (hxc_tmp_call_result_n46)
  {
    struct Color hxc_this1_h0533aeb39716 = (struct Color){ .r = 15, .g = 27, .b = 39, .a = 220 };
    int32_t hxc_tmp_load_result_n48 = hxc_centerX;
    int32_t hxc_tmp_load_result_n49 = hxc_centerY;
    DrawRectangle((int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n48, 260), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n49, 54), (int32_t)520, (int32_t)60, hxc_this1_h0533aeb39716);
    int32_t hxc_code_he09f0bda5f06 = hxc_guide.hxc_phaseCode;
    int32_t hxc_tmp_load_result_n52 = hxc_code_he09f0bda5f06;
    int32_t hxc_tmp_conditional_result_n37 = 0;
    if (hxc_tmp_load_result_n52 == 1)
    {
      hxc_tmp_conditional_result_n37 = 1;
    }
    else
    {
      int32_t hxc_tmp_load_result_n53 = hxc_code_he09f0bda5f06;
      int32_t hxc_tmp_conditional_result_n38 = 0;
      if (hxc_tmp_load_result_n53 == 2)
      {
        hxc_tmp_conditional_result_n38 = 2;
      }
      else
      {
        hxc_tmp_conditional_result_n38 = 0;
      }
      hxc_tmp_conditional_result_n37 = hxc_tmp_conditional_result_n38;
    }
    if (hxc_tmp_conditional_result_n37 == 0)
    {
      int32_t hxc_tmp_load_result_n56 = hxc_centerX;
      int32_t hxc_tmp_load_result_n57 = hxc_centerY;
      hxc_caxecraft_localization_FirstPlayableCatalog_draw(hxc_locale, 13, hxc_i32_subtract_wrapping(hxc_tmp_load_result_n56, 110), hxc_i32_add_wrapping(hxc_tmp_load_result_n57, 74), 18, hxc_text);
    }
    else
    {
      int32_t hxc_code_h653a7b68424d = hxc_guide.hxc_phaseCode;
      int32_t hxc_tmp_load_result_n60 = hxc_code_h653a7b68424d;
      int32_t hxc_tmp_conditional_result_n40 = 0;
      if (hxc_tmp_load_result_n60 == 1)
      {
        hxc_tmp_conditional_result_n40 = 1;
      }
      else
      {
        int32_t hxc_tmp_load_result_n61 = hxc_code_h653a7b68424d;
        int32_t hxc_tmp_conditional_result_n41 = 0;
        if (hxc_tmp_load_result_n61 == 2)
        {
          hxc_tmp_conditional_result_n41 = 2;
        }
        else
        {
          hxc_tmp_conditional_result_n41 = 0;
        }
        hxc_tmp_conditional_result_n40 = hxc_tmp_conditional_result_n41;
      }
      if (hxc_tmp_conditional_result_n40 == 1)
      {
        int32_t hxc_tmp_load_result_n64 = hxc_centerX;
        int32_t hxc_tmp_load_result_n65 = hxc_centerY;
        hxc_caxecraft_localization_FirstPlayableCatalog_draw(hxc_locale, 14, hxc_i32_subtract_wrapping(hxc_tmp_load_result_n64, 225), hxc_i32_add_wrapping(hxc_tmp_load_result_n65, 74), 16, hxc_text);
      }
      else
      {
        int32_t hxc_tmp_load_result_n67 = hxc_centerX;
        int32_t hxc_tmp_load_result_n68 = hxc_centerY;
        hxc_caxecraft_localization_FirstPlayableCatalog_draw(hxc_locale, 12, hxc_i32_subtract_wrapping(hxc_tmp_load_result_n67, 205), hxc_i32_add_wrapping(hxc_tmp_load_result_n68, 74), 16, hxc_text);
      }
    }
  }
  if (hxc_mossling.hxc_health > 0)
  {
    int32_t hxc_code_h4b0ca02c94e0 = hxc_mossling.hxc_modeCode;
    int32_t hxc_tmp_load_result_n72 = hxc_code_h4b0ca02c94e0;
    int32_t hxc_tmp_conditional_result_n43 = 0;
    if (hxc_tmp_load_result_n72 == 1)
    {
      hxc_tmp_conditional_result_n43 = 1;
    }
    else
    {
      int32_t hxc_tmp_load_result_n73 = hxc_code_h4b0ca02c94e0;
      int32_t hxc_tmp_conditional_result_n44 = 0;
      if (hxc_tmp_load_result_n73 == 2)
      {
        hxc_tmp_conditional_result_n44 = 2;
      }
      else
      {
        int32_t hxc_tmp_load_result_n74 = hxc_code_h4b0ca02c94e0;
        int32_t hxc_tmp_conditional_result_n45 = 0;
        if (hxc_tmp_load_result_n74 == 3)
        {
          hxc_tmp_conditional_result_n45 = 3;
        }
        else
        {
          int32_t hxc_tmp_load_result_n75 = hxc_code_h4b0ca02c94e0;
          int32_t hxc_tmp_conditional_result_n46 = 0;
          if (hxc_tmp_load_result_n75 == 4)
          {
            hxc_tmp_conditional_result_n46 = 4;
          }
          else
          {
            int32_t hxc_tmp_load_result_n76 = hxc_code_h4b0ca02c94e0;
            int32_t hxc_tmp_conditional_result_n47 = 0;
            if (hxc_tmp_load_result_n76 == 5)
            {
              hxc_tmp_conditional_result_n47 = 5;
            }
            else
            {
              hxc_tmp_conditional_result_n47 = 0;
            }
            hxc_tmp_conditional_result_n46 = hxc_tmp_conditional_result_n47;
          }
          hxc_tmp_conditional_result_n45 = hxc_tmp_conditional_result_n46;
        }
        hxc_tmp_conditional_result_n44 = hxc_tmp_conditional_result_n45;
      }
      hxc_tmp_conditional_result_n43 = hxc_tmp_conditional_result_n44;
    }
    if (hxc_tmp_conditional_result_n43 == 4)
    {
      hxc_caxecraft_localization_FirstPlayableCatalog_draw(hxc_locale, 11, hxc_i32_subtract_wrapping(hxc_width, 300), 28, 16, (struct Color){ .r = 218, .g = 65, .b = 72, .a = 255 });
    }
    else
    {
      int32_t hxc_code_hab9ea0476743 = hxc_mossling.hxc_modeCode;
      int32_t hxc_tmp_load_result_n85 = hxc_code_hab9ea0476743;
      int32_t hxc_tmp_conditional_result_n49 = 0;
      if (hxc_tmp_load_result_n85 == 1)
      {
        hxc_tmp_conditional_result_n49 = 1;
      }
      else
      {
        int32_t hxc_tmp_load_result_n86 = hxc_code_hab9ea0476743;
        int32_t hxc_tmp_conditional_result_n50 = 0;
        if (hxc_tmp_load_result_n86 == 2)
        {
          hxc_tmp_conditional_result_n50 = 2;
        }
        else
        {
          int32_t hxc_tmp_load_result_n87 = hxc_code_hab9ea0476743;
          int32_t hxc_tmp_conditional_result_n51 = 0;
          if (hxc_tmp_load_result_n87 == 3)
          {
            hxc_tmp_conditional_result_n51 = 3;
          }
          else
          {
            int32_t hxc_tmp_load_result_n88 = hxc_code_hab9ea0476743;
            int32_t hxc_tmp_conditional_result_n52 = 0;
            if (hxc_tmp_load_result_n88 == 4)
            {
              hxc_tmp_conditional_result_n52 = 4;
            }
            else
            {
              int32_t hxc_tmp_load_result_n89 = hxc_code_hab9ea0476743;
              int32_t hxc_tmp_conditional_result_n53 = 0;
              if (hxc_tmp_load_result_n89 == 5)
              {
                hxc_tmp_conditional_result_n53 = 5;
              }
              else
              {
                hxc_tmp_conditional_result_n53 = 0;
              }
              hxc_tmp_conditional_result_n52 = hxc_tmp_conditional_result_n53;
            }
            hxc_tmp_conditional_result_n51 = hxc_tmp_conditional_result_n52;
          }
          hxc_tmp_conditional_result_n50 = hxc_tmp_conditional_result_n51;
        }
        hxc_tmp_conditional_result_n49 = hxc_tmp_conditional_result_n50;
      }
      if (hxc_tmp_conditional_result_n49 == 1)
      {
        hxc_caxecraft_localization_FirstPlayableCatalog_draw(hxc_locale, 9, hxc_i32_subtract_wrapping(hxc_width, 180), 28, 16, (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 });
      }
    }
  }
  if (hxc_strikeHit)
  {
    int32_t hxc_tmp_load_result_n97 = hxc_centerX;
    hxc_caxecraft_localization_FirstPlayableCatalog_draw(hxc_locale, 7, hxc_i32_subtract_wrapping(hxc_tmp_load_result_n97, 70), hxc_i32_subtract_wrapping(hxc_centerY, 54), 18, (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 });
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
    int32_t hxc_tmp_conditional_result_n55 = 0;
    if (hxc_pickupAmount == 1)
    {
      hxc_tmp_conditional_result_n55 = 2;
    }
    else
    {
      hxc_tmp_conditional_result_n55 = 3;
    }
    int32_t hxc_pickupMessage = hxc_tmp_conditional_result_n55;
    int32_t hxc_tmp_load_result_n105 = hxc_pickupMessage;
    int32_t hxc_tmp_load_result_n106 = hxc_centerX;
    hxc_caxecraft_localization_FirstPlayableCatalog_draw(hxc_locale, hxc_tmp_load_result_n105, hxc_i32_subtract_wrapping(hxc_tmp_load_result_n106, 48), hxc_i32_add_wrapping(hxc_centerY, 24), 18, (struct Color){ .r = 174, .g = 78, .b = 136, .a = 255 });
  }
  if (hxc_inventoryFullReason == 1)
  {
    int32_t hxc_tmp_load_result_n109 = hxc_centerX;
    hxc_caxecraft_localization_FirstPlayableCatalog_draw(hxc_locale, 5, hxc_i32_subtract_wrapping(hxc_tmp_load_result_n109, 150), hxc_i32_add_wrapping(hxc_centerY, 48), 16, (struct Color){ .r = 238, .g = 113, .b = 78, .a = 255 });
  }
  else
  {
    if (hxc_inventoryFullReason == 2)
    {
      int32_t hxc_tmp_load_result_n112 = hxc_centerX;
      hxc_caxecraft_localization_FirstPlayableCatalog_draw(hxc_locale, 6, hxc_i32_subtract_wrapping(hxc_tmp_load_result_n112, 155), hxc_i32_add_wrapping(hxc_centerY, 48), 16, (struct Color){ .r = 238, .g = 113, .b = 78, .a = 255 });
    }
  }
  if (hxc_recoveryVisible)
  {
    if (hxc_recoveryFeedback == 1)
    {
      int32_t hxc_tmp_load_result_n115 = hxc_centerX;
      hxc_caxecraft_localization_FirstPlayableCatalog_draw(hxc_locale, 4, hxc_i32_subtract_wrapping(hxc_tmp_load_result_n115, 88), hxc_i32_add_wrapping(hxc_centerY, 24), 18, (struct Color){ .r = 94, .g = 212, .b = 136, .a = 255 });
    }
    else
    {
      if (hxc_recoveryFeedback == 2)
      {
        int32_t hxc_tmp_load_result_n118 = hxc_centerX;
        hxc_caxecraft_localization_UiCatalog_draw(hxc_locale, 8, hxc_i32_subtract_wrapping(hxc_tmp_load_result_n118, 96), hxc_i32_add_wrapping(hxc_centerY, 24), 18, (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 });
      }
      else
      {
        if (hxc_recoveryFeedback == 3)
        {
          int32_t hxc_tmp_load_result_n121 = hxc_centerX;
          hxc_caxecraft_localization_FirstPlayableCatalog_draw(hxc_locale, 15, hxc_i32_subtract_wrapping(hxc_tmp_load_result_n121, 76), hxc_i32_add_wrapping(hxc_centerY, 24), 18, (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 });
        }
      }
    }
  }
  if (hxc_vitals.hxc_safeTicks > 15)
  {
    struct Color hxc_this1_h8635c5531bee = (struct Color){ .r = 218, .g = 65, .b = 72, .a = 255 };
    int32_t hxc_tmp_load_result_n126 = hxc_width;
    int32_t hxc_tmp_load_result_n127 = hxc_height;
    DrawRectangleLines((int32_t)4, (int32_t)4, (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n126, 8), (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n127, 8), hxc_this1_h8635c5531bee);
  }
  if (hxc_vitals.hxc_health <= 0)
  {
    struct Color hxc_this1_hccc6915d0852 = (struct Color){ .r = 15, .g = 27, .b = 39, .a = 220 };
    int32_t hxc_tmp_load_result_n131 = hxc_centerX;
    int32_t hxc_tmp_load_result_n132 = hxc_centerY;
    DrawRectangle((int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n131, 250), (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n132, 74), (int32_t)500, (int32_t)148, hxc_this1_hccc6915d0852);
    struct Color hxc_this1_h7d1362dfa4a5 = (struct Color){ .r = 218, .g = 65, .b = 72, .a = 255 };
    int32_t hxc_tmp_load_result_n135 = hxc_centerX;
    int32_t hxc_tmp_load_result_n136 = hxc_centerY;
    DrawRectangleLines((int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n135, 250), (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n136, 74), (int32_t)500, (int32_t)148, hxc_this1_h7d1362dfa4a5);
    int32_t hxc_tmp_load_result_n138 = hxc_centerX;
    int32_t hxc_tmp_load_result_n139 = hxc_centerY;
    hxc_caxecraft_localization_FirstPlayableCatalog_draw(hxc_locale, 8, hxc_i32_subtract_wrapping(hxc_tmp_load_result_n138, 122), hxc_i32_subtract_wrapping(hxc_tmp_load_result_n139, 42), 24, hxc_text);
    int32_t hxc_tmp_load_result_n141 = hxc_centerX;
    hxc_caxecraft_localization_FirstPlayableCatalog_draw(hxc_locale, 16, hxc_i32_subtract_wrapping(hxc_tmp_load_result_n141, 125), hxc_i32_add_wrapping(hxc_centerY, 10), 18, (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 });
  }
  if (hxc_paused)
  {
    struct Color hxc_this1_hb1681ec75131 = (struct Color){ .r = 15, .g = 27, .b = 39, .a = 220 };
    int32_t hxc_tmp_load_result_n145 = hxc_centerX;
    int32_t hxc_tmp_load_result_n146 = hxc_centerY;
    DrawRectangle((int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n145, 170), (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n146, 48), (int32_t)340, (int32_t)96, hxc_this1_hb1681ec75131);
    struct Color hxc_this1_h53dbabd6237c = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
    int32_t hxc_tmp_load_result_n149 = hxc_centerX;
    int32_t hxc_tmp_load_result_n150 = hxc_centerY;
    DrawRectangleLines((int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n149, 170), (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n150, 48), (int32_t)340, (int32_t)96, hxc_this1_h53dbabd6237c);
    int32_t hxc_tmp_load_result_n152 = hxc_centerX;
    int32_t hxc_tmp_load_result_n153 = hxc_centerY;
    hxc_caxecraft_localization_UiCatalog_draw(hxc_locale, 14, hxc_i32_subtract_wrapping(hxc_tmp_load_result_n152, 48), hxc_i32_subtract_wrapping(hxc_tmp_load_result_n153, 30), 24, hxc_text);
    int32_t hxc_tmp_load_result_n155 = hxc_centerX;
    int32_t hxc_tmp_load_result_n156 = hxc_centerY;
    hxc_caxecraft_localization_UiCatalog_draw(hxc_locale, 13, hxc_i32_subtract_wrapping(hxc_tmp_load_result_n155, 160), hxc_i32_add_wrapping(hxc_tmp_load_result_n156, 8), 16, hxc_text);
  }
  else
  {
    if (hxc_placementBlocked)
    {
      int32_t hxc_tmp_load_result_n158 = hxc_centerX;
      hxc_caxecraft_localization_UiCatalog_draw(hxc_locale, 15, hxc_i32_subtract_wrapping(hxc_tmp_load_result_n158, 170), hxc_i32_add_wrapping(hxc_centerY, 26), 14, (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 });
    }
    else
    {
      if (!hxc_captured)
      {
        int32_t hxc_tmp_load_result_n161 = hxc_centerX;
        int32_t hxc_tmp_load_result_n162 = hxc_centerY;
        hxc_caxecraft_localization_UiCatalog_draw(hxc_locale, 1, hxc_i32_subtract_wrapping(hxc_tmp_load_result_n161, 90), hxc_i32_add_wrapping(hxc_tmp_load_result_n162, 26), 14, hxc_text);
      }
      else
      {
        if (!hxc_hit.hxc_hit)
        {
          int32_t hxc_tmp_load_result_n165 = hxc_centerX;
          int32_t hxc_tmp_load_result_n166 = hxc_centerY;
          hxc_caxecraft_localization_UiCatalog_draw(hxc_locale, 12, hxc_i32_subtract_wrapping(hxc_tmp_load_result_n165, 105), hxc_i32_add_wrapping(hxc_tmp_load_result_n166, 26), 14, hxc_text);
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
  hxc_caxecraft_domain_World_generate(hxc_cells, hxc_tmp_length_n1, 51966);
  hxc_caxecraft_domain_World_prepareSpawnMeadow(hxc_cells, hxc_tmp_length_n1);
  int32_t hxc_windowFlags = hxc_i32_bit_or(64, 4);
  hxc_windowFlags = hxc_i32_bit_or(hxc_windowFlags, 8192);
  SetConfigFlags((uint32_t)hxc_windowFlags);
  InitWindow((int32_t)1280, (int32_t)720, "Caxecraft \342\200\224 Haxe shaped into careful C");
  SetWindowMinSize((int32_t)800, (int32_t)450);
  SetExitKey((int32_t)0);
  SetTargetFPS((int32_t)120);
  EnableCursor();
  struct Texture hxc_tmp_native_call_result_n2 = LoadTexture("assets/showcase/title-panorama.png");
  struct Texture hxc_titleTexture = hxc_tmp_native_call_result_n2;
  bool hxc_tmp_native_call_result_n4 = IsTextureValid(hxc_titleTexture);
  bool hxc_titleTextureReady = hxc_tmp_native_call_result_n4;
  struct Texture hxc_tmp_native_call_result_n5 = LoadTexture("assets/branding/caxecraft-wordmark.png");
  struct Texture hxc_wordmarkTexture = hxc_tmp_native_call_result_n5;
  bool hxc_tmp_native_call_result_n7 = IsTextureValid(hxc_wordmarkTexture);
  bool hxc_wordmarkTextureReady = hxc_tmp_native_call_result_n7;
  struct Texture hxc_tmp_native_call_result_n8 = LoadTexture("assets/atlases/hud.png");
  struct Texture hxc_hudTexture = hxc_tmp_native_call_result_n8;
  bool hxc_tmp_native_call_result_n10 = IsTextureValid(hxc_hudTexture);
  bool hxc_hudTextureReady = hxc_tmp_native_call_result_n10;
  struct Texture hxc_tmp_native_call_result_n11 = LoadTexture("assets/atlases/items.png");
  struct Texture hxc_itemTexture = hxc_tmp_native_call_result_n11;
  bool hxc_tmp_native_call_result_n13 = IsTextureValid(hxc_itemTexture);
  bool hxc_itemTextureReady = hxc_tmp_native_call_result_n13;
  struct Texture hxc_tmp_native_call_result_n14 = LoadTexture("assets/atlases/entities.png");
  struct Texture hxc_entityTexture = hxc_tmp_native_call_result_n14;
  bool hxc_tmp_native_call_result_n16 = IsTextureValid(hxc_entityTexture);
  bool hxc_entityTextureReady = hxc_tmp_native_call_result_n16;
  struct Texture hxc_tmp_native_call_result_n17 = LoadTexture("assets/atlases/terrain.png");
  struct Texture hxc_terrainTexture = hxc_tmp_native_call_result_n17;
  bool hxc_tmp_native_call_result_n19 = IsTextureValid(hxc_terrainTexture);
  bool hxc_terrainTextureReady = hxc_tmp_native_call_result_n19;
  struct hxc_caxecraft_domain_PlayerState hxc_tmp_call_result_n20 = hxc_caxecraft_app_Main_spawnPlayer(hxc_cells, hxc_tmp_length_n1);
  struct hxc_caxecraft_domain_PlayerState hxc_player = hxc_tmp_call_result_n20;
  struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_call_result_n21 = hxc_caxecraft_gameplay_Inventory_starter();
  struct hxc_caxecraft_gameplay_InventoryState hxc_inventory = hxc_tmp_call_result_n21;
  struct hxc_caxecraft_gameplay_GuideState hxc_tmp_call_result_n22 = hxc_caxecraft_gameplay_GuideNpc_start(hxc_cells, hxc_tmp_length_n1, 17.5, 13.5);
  struct hxc_caxecraft_gameplay_GuideState hxc_guide = hxc_tmp_call_result_n22;
  struct hxc_caxecraft_gameplay_MosslingState hxc_tmp_call_result_n23 = hxc_caxecraft_gameplay_Mossling_start(hxc_cells, hxc_tmp_length_n1, 15.5, 13.8);
  struct hxc_caxecraft_gameplay_MosslingState hxc_mossling = hxc_tmp_call_result_n23;
  int32_t hxc_initialHealth = 6;
  struct hxc_caxecraft_gameplay_PlayerVitalsState hxc_tmp_call_result_n25 = hxc_caxecraft_gameplay_PlayerVitals_make(hxc_initialHealth, 0);
  struct hxc_caxecraft_gameplay_PlayerVitalsState hxc_vitals = hxc_tmp_call_result_n25;
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
    bool hxc_tmp_load_result_n28 = hxc_quit;
    bool hxc_tmp_short_circuit_result_n47 = !hxc_tmp_load_result_n28;
    if (!hxc_tmp_load_result_n28)
    {
      bool hxc_tmp_native_call_result_n29 = WindowShouldClose();
      hxc_tmp_short_circuit_result_n47 = !hxc_tmp_native_call_result_n29;
    }
    if (!hxc_tmp_short_circuit_result_n47)
    {
      break;
    }
    bool hxc_recapturedThisFrame = false;
    bool hxc_tmp_native_call_result_n31 = IsWindowFocused();
    bool hxc_focused = hxc_tmp_native_call_result_n31;
    double hxc_forward = 0.0;
    double hxc_right = 0.0;
    bool hxc_tmp_native_call_result_n32 = IsKeyDown((int32_t)87);
    if (hxc_tmp_native_call_result_n32)
    {
      hxc_forward = hxc_forward + 1.0;
    }
    bool hxc_tmp_native_call_result_n34 = IsKeyDown((int32_t)83);
    if (hxc_tmp_native_call_result_n34)
    {
      hxc_forward = hxc_forward - 1.0;
    }
    bool hxc_tmp_native_call_result_n36 = IsKeyDown((int32_t)68);
    if (hxc_tmp_native_call_result_n36)
    {
      hxc_right = hxc_right + 1.0;
    }
    bool hxc_tmp_native_call_result_n38 = IsKeyDown((int32_t)65);
    if (hxc_tmp_native_call_result_n38)
    {
      hxc_right = hxc_right - 1.0;
    }
    double hxc_lookYaw_h5de48019064f = 0.0;
    double hxc_lookPitch_h89b3984ba953 = 0.0;
    if (hxc_captured)
    {
      struct Vector2 hxc_tmp_native_call_result_n41 = GetMouseDelta();
      struct Vector2 hxc_mouse = hxc_tmp_native_call_result_n41;
      hxc_lookYaw_h5de48019064f = -(double)hxc_mouse.x * 0.0025;
      hxc_lookPitch_h89b3984ba953 = -(double)hxc_mouse.y * 0.0025;
    }
    bool hxc_tmp_native_call_result_n46 = IsMouseButtonPressed((int32_t)0);
    bool hxc_leftPressed = hxc_tmp_native_call_result_n46;
    bool hxc_tmp_load_result_n47 = hxc_captured;
    bool hxc_tmp_short_circuit_result_n57 = hxc_tmp_load_result_n47;
    if (hxc_tmp_load_result_n47)
    {
      hxc_tmp_short_circuit_result_n57 = hxc_leftPressed;
    }
    bool hxc_primaryPressed_h235cb64355b3 = hxc_tmp_short_circuit_result_n57;
    bool hxc_tmp_load_result_n50 = hxc_captured;
    bool hxc_tmp_short_circuit_result_n59 = hxc_tmp_load_result_n50;
    if (hxc_tmp_load_result_n50)
    {
      bool hxc_tmp_native_call_result_n51 = IsMouseButtonPressed((int32_t)1);
      hxc_tmp_short_circuit_result_n59 = hxc_tmp_native_call_result_n51;
    }
    bool hxc_secondaryPressed_h8dc1f8e9ed66 = hxc_tmp_short_circuit_result_n59;
    bool hxc_tmp_load_result_n53 = hxc_captured;
    bool hxc_tmp_short_circuit_result_n61 = hxc_tmp_load_result_n53;
    if (hxc_tmp_load_result_n53)
    {
      bool hxc_tmp_native_call_result_n54 = IsKeyPressed((int32_t)69);
      hxc_tmp_short_circuit_result_n61 = hxc_tmp_native_call_result_n54;
    }
    bool hxc_interactPressed_h8f846d8228b5 = hxc_tmp_short_circuit_result_n61;
    int32_t hxc_hotbarSelection_hb8b697ce24e9 = -1;
    bool hxc_tmp_native_call_result_n56 = IsKeyPressed((int32_t)49);
    if (hxc_tmp_native_call_result_n56)
    {
      hxc_hotbarSelection_hb8b697ce24e9 = 0;
    }
    bool hxc_tmp_native_call_result_n57 = IsKeyPressed((int32_t)50);
    if (hxc_tmp_native_call_result_n57)
    {
      hxc_hotbarSelection_hb8b697ce24e9 = 1;
    }
    bool hxc_tmp_native_call_result_n58 = IsKeyPressed((int32_t)51);
    if (hxc_tmp_native_call_result_n58)
    {
      hxc_hotbarSelection_hb8b697ce24e9 = 2;
    }
    bool hxc_tmp_native_call_result_n59 = IsKeyPressed((int32_t)52);
    if (hxc_tmp_native_call_result_n59)
    {
      hxc_hotbarSelection_hb8b697ce24e9 = 3;
    }
    bool hxc_tmp_native_call_result_n60 = IsKeyPressed((int32_t)53);
    if (hxc_tmp_native_call_result_n60)
    {
      hxc_hotbarSelection_hb8b697ce24e9 = 4;
    }
    bool hxc_tmp_native_call_result_n61 = IsKeyPressed((int32_t)54);
    if (hxc_tmp_native_call_result_n61)
    {
      hxc_hotbarSelection_hb8b697ce24e9 = 5;
    }
    bool hxc_tmp_native_call_result_n62 = IsKeyPressed((int32_t)55);
    if (hxc_tmp_native_call_result_n62)
    {
      hxc_hotbarSelection_hb8b697ce24e9 = 6;
    }
    bool hxc_tmp_native_call_result_n63 = IsKeyPressed((int32_t)56);
    if (hxc_tmp_native_call_result_n63)
    {
      hxc_hotbarSelection_hb8b697ce24e9 = 7;
    }
    float hxc_tmp_native_call_result_n64 = GetMouseWheelMove();
    double hxc_wheel = (double)hxc_tmp_native_call_result_n64;
    int32_t hxc_hotbarCycle_h0b6bb91f312f = 0;
    if (hxc_wheel > 0.0)
    {
      hxc_hotbarCycle_h0b6bb91f312f = -1;
    }
    if (hxc_wheel < 0.0)
    {
      hxc_hotbarCycle_h0b6bb91f312f = 1;
    }
    bool hxc_tmp_native_call_result_n67 = IsKeyPressed((int32_t)32);
    bool hxc_jumpPressed_h42df7d293ab5 = hxc_tmp_native_call_result_n67;
    bool hxc_tmp_native_call_result_n68 = IsKeyPressed((int32_t)256);
    bool hxc_pausePressed_habad51b9d56f = hxc_tmp_native_call_result_n68;
    bool hxc_tmp_native_call_result_n69 = IsKeyPressed((int32_t)81);
    bool hxc_quitPressed_h959b714c645c = hxc_tmp_native_call_result_n69;
    double hxc_frameInput_moveForward = hxc_forward;
    double hxc_frameInput_moveRight = hxc_right;
    double hxc_frameInput_lookYaw = hxc_lookYaw_h5de48019064f;
    double hxc_frameInput_lookPitch = hxc_lookPitch_h89b3984ba953;
    bool hxc_frameInput_jumpPressed = hxc_jumpPressed_h42df7d293ab5;
    bool hxc_frameInput_primaryPressed = hxc_primaryPressed_h235cb64355b3;
    bool hxc_frameInput_secondaryPressed = hxc_secondaryPressed_h8dc1f8e9ed66;
    bool hxc_frameInput_interactPressed = hxc_interactPressed_h8f846d8228b5;
    bool hxc_frameInput_pausePressed = hxc_pausePressed_habad51b9d56f;
    bool hxc_tmp_load_result_n79 = hxc_paused;
    bool hxc_tmp_short_circuit_result_n78 = hxc_tmp_load_result_n79;
    if (hxc_tmp_load_result_n79)
    {
      hxc_tmp_short_circuit_result_n78 = hxc_leftPressed;
    }
    bool hxc_frameInput_capturePressed = hxc_tmp_short_circuit_result_n78;
    bool hxc_frameInput_quitPressed = hxc_quitPressed_h959b714c645c;
    int32_t hxc_frameInput_hotbarSelection = hxc_hotbarSelection_hb8b697ce24e9;
    int32_t hxc_frameInput_hotbarCycle = hxc_hotbarCycle_h0b6bb91f312f;
    double hxc_moveForward = hxc_frameInput_moveForward;
    double hxc_moveRight = hxc_frameInput_moveRight;
    double hxc_lookYaw_h450d4c7c43c6 = hxc_frameInput_lookYaw;
    double hxc_lookPitch_h4f6fcca950c1 = hxc_frameInput_lookPitch;
    bool hxc_jumpPressed_h1bf6f1bcab7d = hxc_frameInput_jumpPressed;
    bool hxc_primaryPressed_h6b5af3f34586 = hxc_frameInput_primaryPressed;
    bool hxc_secondaryPressed_h9f1e7ee32377 = hxc_frameInput_secondaryPressed;
    bool hxc_interactPressed_h5ff9e7f3fca6 = hxc_frameInput_interactPressed;
    bool hxc_pausePressed_h24b6715bdf38 = hxc_frameInput_pausePressed;
    bool hxc_capturePressed = hxc_frameInput_capturePressed;
    bool hxc_quitPressed_hb6dad4c886ab = hxc_frameInput_quitPressed;
    int32_t hxc_hotbarSelection_h8d0c132f788c = hxc_frameInput_hotbarSelection;
    int32_t hxc_hotbarCycle_h035a60ff5651 = hxc_frameInput_hotbarCycle;
    if (hxc_quitPressed_hb6dad4c886ab)
    {
      hxc_quit = true;
    }
    if (hxc_hotbarSelection_h8d0c132f788c >= 0)
    {
      struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_load_result_n100 = hxc_inventory;
      struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_call_result_n102 = hxc_caxecraft_gameplay_Inventory_select(hxc_tmp_load_result_n100, hxc_hotbarSelection_h8d0c132f788c);
      hxc_inventory = hxc_tmp_call_result_n102;
    }
    if (hxc_hotbarCycle_h035a60ff5651 != 0)
    {
      struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_load_result_n104 = hxc_inventory;
      struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_call_result_n106 = hxc_caxecraft_gameplay_Inventory_cycle(hxc_tmp_load_result_n104, hxc_hotbarCycle_h035a60ff5651);
      hxc_inventory = hxc_tmp_call_result_n106;
    }
    bool hxc_tmp_load_result_n107 = hxc_onTitle;
    bool hxc_tmp_short_circuit_result_n95 = !hxc_tmp_load_result_n107;
    if (!hxc_tmp_load_result_n107)
    {
      hxc_tmp_short_circuit_result_n95 = !hxc_paused;
    }
    bool hxc_tmp_short_circuit_load_result_n109 = hxc_tmp_short_circuit_result_n95;
    bool hxc_tmp_short_circuit_result_n96 = hxc_tmp_short_circuit_load_result_n109;
    if (hxc_tmp_short_circuit_load_result_n109)
    {
      hxc_tmp_short_circuit_result_n96 = hxc_interactPressed_h5ff9e7f3fca6;
    }
    if (hxc_tmp_short_circuit_result_n96)
    {
      if (hxc_vitals.hxc_health <= 0)
      {
        struct hxc_caxecraft_gameplay_PlayerVitalsState hxc_tmp_call_result_n113 = hxc_caxecraft_gameplay_PlayerVitals_make(6, 0);
        hxc_vitals = hxc_tmp_call_result_n113;
        struct hxc_caxecraft_domain_PlayerState hxc_tmp_call_result_n114 = hxc_caxecraft_app_Main_spawnPlayer(hxc_cells, hxc_tmp_length_n1);
        hxc_player = hxc_tmp_call_result_n114;
      }
      else
      {
        struct hxc_caxecraft_gameplay_GuideState hxc_tmp_load_result_n115 = hxc_guide;
        double hxc_tmp_record_field_load_result_n116 = hxc_player.hxc_x;
        bool hxc_tmp_call_result_n118 = hxc_caxecraft_gameplay_GuideNpc_isInRange(hxc_tmp_load_result_n115, hxc_tmp_record_field_load_result_n116, hxc_player.hxc_z);
        if (hxc_tmp_call_result_n118)
        {
          bool hxc_tmp_call_result_n120 = hxc_caxecraft_gameplay_GuideNpc_sharesBerriesOnNextInteraction(hxc_guide);
          bool hxc_sharesBerries = hxc_tmp_call_result_n120;
          if (hxc_sharesBerries)
          {
            int32_t hxc_tmp_call_result_n124 = hxc_caxecraft_gameplay_Inventory_acceptedAmount(hxc_inventory, hxc_caxecraft_gameplay_ItemKind_Berries, 2);
            int32_t hxc_acceptedGift = hxc_tmp_call_result_n124;
            if (hxc_acceptedGift == 2)
            {
              struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_load_result_n126 = hxc_inventory;
              struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_call_result_n129 = hxc_caxecraft_gameplay_Inventory_collectItem(hxc_tmp_load_result_n126, hxc_caxecraft_gameplay_ItemKind_Berries, hxc_acceptedGift);
              hxc_inventory = hxc_tmp_call_result_n129;
              struct hxc_caxecraft_gameplay_GuideState hxc_tmp_call_result_n131 = hxc_caxecraft_gameplay_GuideNpc_interact(hxc_guide);
              hxc_guide = hxc_tmp_call_result_n131;
            }
            else
            {
              hxc_inventoryFullReason = 1;
              hxc_inventoryFullFrames = 90;
            }
          }
          else
          {
            struct hxc_caxecraft_gameplay_GuideState hxc_tmp_call_result_n133 = hxc_caxecraft_gameplay_GuideNpc_interact(hxc_guide);
            hxc_guide = hxc_tmp_call_result_n133;
          }
        }
      }
    }
    bool hxc_tmp_load_result_n134 = hxc_onTitle;
    bool hxc_tmp_short_circuit_result_n99 = hxc_tmp_load_result_n134;
    if (hxc_tmp_load_result_n134)
    {
      hxc_tmp_short_circuit_result_n99 = hxc_focused;
    }
    if (hxc_tmp_short_circuit_result_n99)
    {
      bool hxc_tmp_native_call_result_n137 = IsKeyPressed((int32_t)76);
      if (hxc_tmp_native_call_result_n137)
      {
        int32_t hxc_tmp_call_result_n139 = hxc_caxecraft_localization_UiCatalog_nextLocale(hxc_locale);
        hxc_locale = hxc_tmp_call_result_n139;
      }
      bool hxc_tmp_native_call_result_n140 = IsKeyPressed((int32_t)265);
      bool hxc_tmp_short_circuit_result_n100 = hxc_tmp_native_call_result_n140;
      if (!hxc_tmp_native_call_result_n140)
      {
        bool hxc_tmp_native_call_result_n141 = IsKeyPressed((int32_t)264);
        hxc_tmp_short_circuit_result_n100 = hxc_tmp_native_call_result_n141;
      }
      if (hxc_tmp_short_circuit_result_n100)
      {
        int32_t hxc_tmp_load_result_n143 = hxc_selectedMode;
        int32_t hxc_tmp_conditional_result_n101 = 0;
        if (hxc_tmp_load_result_n143 == 0)
        {
          hxc_tmp_conditional_result_n101 = 1;
        }
        else
        {
          hxc_tmp_conditional_result_n101 = 0;
        }
        hxc_selectedMode = hxc_tmp_conditional_result_n101;
      }
      struct Vector2 hxc_tmp_native_call_result_n145 = GetMousePosition();
      struct Vector2 hxc_menuMouse = hxc_tmp_native_call_result_n145;
      struct Vector2 hxc_tmp_load_result_n146 = hxc_menuMouse;
      struct Vector2 hxc_tmp_load_result_n148 = hxc_menuMouse;
      int32_t hxc_tmp_native_call_result_n150 = GetScreenWidth();
      int32_t hxc_tmp_native_call_result_n151 = GetScreenHeight();
      int32_t hxc_tmp_call_result_n152 = hxc_caxecraft_app_TitleMenu_selectionAt((double)hxc_tmp_load_result_n146.x, (double)hxc_tmp_load_result_n148.y, (int32_t)hxc_tmp_native_call_result_n150, (int32_t)hxc_tmp_native_call_result_n151);
      int32_t hxc_hovered = hxc_tmp_call_result_n152;
      if (hxc_hovered == 0)
      {
        hxc_selectedMode = 0;
      }
      if (hxc_hovered == 1)
      {
        hxc_selectedMode = 1;
      }
      int32_t hxc_tmp_load_result_n155 = hxc_hovered;
      bool hxc_tmp_short_circuit_result_n105 = hxc_tmp_load_result_n155 >= 0;
      if (hxc_tmp_load_result_n155 >= 0)
      {
        bool hxc_tmp_native_call_result_n156 = IsMouseButtonPressed((int32_t)0);
        hxc_tmp_short_circuit_result_n105 = hxc_tmp_native_call_result_n156;
      }
      bool hxc_clickedChoice = hxc_tmp_short_circuit_result_n105;
      bool hxc_tmp_load_result_n158 = hxc_clickedChoice;
      bool hxc_tmp_short_circuit_result_n106 = hxc_tmp_load_result_n158;
      if (!hxc_tmp_load_result_n158)
      {
        bool hxc_tmp_native_call_result_n159 = IsKeyPressed((int32_t)257);
        hxc_tmp_short_circuit_result_n106 = hxc_tmp_native_call_result_n159;
      }
      if (hxc_tmp_short_circuit_result_n106)
      {
        hxc_onTitle = false;
        hxc_paused = false;
        hxc_captured = true;
        hxc_recapturedThisFrame = true;
        DisableCursor();
      }
    }
    bool hxc_tmp_load_result_n161 = hxc_onTitle;
    bool hxc_tmp_short_circuit_result_n107 = !hxc_tmp_load_result_n161;
    if (!hxc_tmp_load_result_n161)
    {
      hxc_tmp_short_circuit_result_n107 = !hxc_focused;
    }
    bool hxc_tmp_short_circuit_load_result_n163 = hxc_tmp_short_circuit_result_n107;
    bool hxc_tmp_short_circuit_result_n108 = hxc_tmp_short_circuit_load_result_n163;
    if (hxc_tmp_short_circuit_load_result_n163)
    {
      hxc_tmp_short_circuit_result_n108 = hxc_captured;
    }
    if (hxc_tmp_short_circuit_result_n108)
    {
      hxc_paused = true;
      hxc_captured = false;
      hxc_jumpQueued = false;
      EnableCursor();
    }
    bool hxc_tmp_load_result_n166 = hxc_onTitle;
    bool hxc_tmp_short_circuit_result_n109 = !hxc_tmp_load_result_n166;
    if (!hxc_tmp_load_result_n166)
    {
      hxc_tmp_short_circuit_result_n109 = hxc_focused;
    }
    bool hxc_tmp_short_circuit_load_result_n168 = hxc_tmp_short_circuit_result_n109;
    bool hxc_tmp_short_circuit_result_n110 = hxc_tmp_short_circuit_load_result_n168;
    if (hxc_tmp_short_circuit_load_result_n168)
    {
      hxc_tmp_short_circuit_result_n110 = hxc_pausePressed_h24b6715bdf38;
    }
    if (hxc_tmp_short_circuit_result_n110)
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
    bool hxc_tmp_load_result_n175 = hxc_onTitle;
    bool hxc_tmp_short_circuit_result_n111 = !hxc_tmp_load_result_n175;
    if (!hxc_tmp_load_result_n175)
    {
      hxc_tmp_short_circuit_result_n111 = hxc_focused;
    }
    bool hxc_tmp_short_circuit_load_result_n177 = hxc_tmp_short_circuit_result_n111;
    bool hxc_tmp_short_circuit_result_n112 = hxc_tmp_short_circuit_load_result_n177;
    if (hxc_tmp_short_circuit_load_result_n177)
    {
      hxc_tmp_short_circuit_result_n112 = hxc_paused;
    }
    bool hxc_tmp_short_circuit_load_result_n179 = hxc_tmp_short_circuit_result_n112;
    bool hxc_tmp_short_circuit_result_n113 = hxc_tmp_short_circuit_load_result_n179;
    if (hxc_tmp_short_circuit_load_result_n179)
    {
      hxc_tmp_short_circuit_result_n113 = hxc_capturePressed;
    }
    if (hxc_tmp_short_circuit_result_n113)
    {
      hxc_paused = false;
      hxc_captured = true;
      hxc_recapturedThisFrame = true;
      DisableCursor();
    }
    if (hxc_captured)
    {
      double hxc_yawDelta = hxc_lookYaw_h450d4c7c43c6;
      if (hxc_yawDelta > 0.25)
      {
        hxc_yawDelta = 0.25;
      }
      if (hxc_yawDelta < -0.25)
      {
        hxc_yawDelta = -0.25;
      }
      double hxc_tmp_load_result_n186 = hxc_lookX;
      double hxc_tmp_load_result_n187 = hxc_yawDelta;
      double hxc_candidateX = hxc_tmp_load_result_n186 + hxc_tmp_load_result_n187 * hxc_lookZ;
      double hxc_tmp_load_result_n189 = hxc_lookZ;
      double hxc_tmp_load_result_n190 = hxc_yawDelta;
      double hxc_candidateZ = hxc_tmp_load_result_n189 - hxc_tmp_load_result_n190 * hxc_lookX;
      double hxc_tmp_load_result_n192 = hxc_candidateX;
      double hxc_tmp_load_result_n193 = hxc_candidateX;
      double hxc_tmp_load_result_n194 = hxc_candidateZ;
      double hxc_lengthSquared = hxc_tmp_load_result_n192 * hxc_tmp_load_result_n193 + hxc_tmp_load_result_n194 * hxc_candidateZ;
      double hxc_normalize = 1.5 - 0.5 * hxc_lengthSquared;
      double hxc_tmp_load_result_n197 = hxc_candidateX;
      hxc_lookX = hxc_tmp_load_result_n197 * hxc_normalize;
      double hxc_tmp_load_result_n199 = hxc_candidateZ;
      hxc_lookZ = hxc_tmp_load_result_n199 * hxc_normalize;
      double hxc_tmp_compound_load_result_n201 = hxc_lookY;
      hxc_lookY = hxc_tmp_compound_load_result_n201 + hxc_lookPitch_h4f6fcca950c1;
      if (hxc_lookY > 0.90)
      {
        hxc_lookY = 0.90;
      }
      if (hxc_lookY < -0.90)
      {
        hxc_lookY = -0.90;
      }
      if (hxc_jumpPressed_h1bf6f1bcab7d)
      {
        hxc_jumpQueued = true;
      }
    }
    float hxc_tmp_native_call_result_n206 = GetFrameTime();
    double hxc_frameSeconds = (double)hxc_tmp_native_call_result_n206;
    if (hxc_frameSeconds > 0.25)
    {
      hxc_frameSeconds = 0.25;
    }
    bool hxc_tmp_load_result_n208 = hxc_captured;
    bool hxc_tmp_short_circuit_result_n120 = hxc_tmp_load_result_n208;
    if (hxc_tmp_load_result_n208)
    {
      hxc_tmp_short_circuit_result_n120 = !hxc_recapturedThisFrame;
    }
    bool hxc_tmp_short_circuit_load_result_n210 = hxc_tmp_short_circuit_result_n120;
    bool hxc_tmp_short_circuit_result_n121 = hxc_tmp_short_circuit_load_result_n210;
    if (hxc_tmp_short_circuit_load_result_n210)
    {
      hxc_tmp_short_circuit_result_n121 = hxc_primaryPressed_h6b5af3f34586;
    }
    bool hxc_tmp_short_circuit_load_result_n212 = hxc_tmp_short_circuit_result_n121;
    bool hxc_tmp_short_circuit_result_n122 = hxc_tmp_short_circuit_load_result_n212;
    if (hxc_tmp_short_circuit_load_result_n212)
    {
      hxc_tmp_short_circuit_result_n122 = hxc_selectedMode == 1;
    }
    bool hxc_tmp_short_circuit_load_result_n214 = hxc_tmp_short_circuit_result_n122;
    bool hxc_tmp_short_circuit_result_n123 = hxc_tmp_short_circuit_load_result_n214;
    if (hxc_tmp_short_circuit_load_result_n214)
    {
      bool hxc_tmp_call_result_n217 = hxc_caxecraft_gameplay_Inventory_selectedIs(hxc_inventory, hxc_caxecraft_gameplay_ItemKind_CopperSword);
      hxc_tmp_short_circuit_result_n123 = hxc_tmp_call_result_n217;
    }
    if (hxc_tmp_short_circuit_result_n123)
    {
      hxc_swordQueued = true;
    }
    if (!hxc_paused)
    {
      double hxc_tmp_compound_load_result_n220 = hxc_accumulator;
      hxc_accumulator = hxc_tmp_compound_load_result_n220 + hxc_frameSeconds;
    }
    while (1)
    {
      bool hxc_tmp_load_result_n222 = hxc_paused;
      bool hxc_tmp_short_circuit_result_n124 = !hxc_tmp_load_result_n222;
      if (!hxc_tmp_load_result_n222)
      {
        hxc_tmp_short_circuit_result_n124 = hxc_accumulator >= 0.05;
      }
      if (!hxc_tmp_short_circuit_result_n124)
      {
        break;
      }
      double hxc_tmp_load_result_n225 = hxc_moveForward;
      double hxc_tmp_load_result_n226 = hxc_lookX;
      double hxc_tmp_load_result_n227 = hxc_moveRight;
      double hxc_moveX = hxc_tmp_load_result_n225 * hxc_tmp_load_result_n226 - hxc_tmp_load_result_n227 * hxc_lookZ;
      double hxc_tmp_load_result_n229 = hxc_moveForward;
      double hxc_tmp_load_result_n230 = hxc_lookZ;
      double hxc_tmp_load_result_n231 = hxc_moveRight;
      double hxc_moveZ = hxc_tmp_load_result_n229 * hxc_tmp_load_result_n230 + hxc_tmp_load_result_n231 * hxc_lookX;
      double hxc_tmp_load_result_n233 = hxc_moveForward;
      bool hxc_tmp_short_circuit_result_n127 = hxc_tmp_load_result_n233 != 0.0;
      if (hxc_tmp_load_result_n233 != 0.0)
      {
        hxc_tmp_short_circuit_result_n127 = hxc_moveRight != 0.0;
      }
      if (hxc_tmp_short_circuit_result_n127)
      {
        hxc_moveX = hxc_moveX * 0.7071067811865476;
        hxc_moveZ = hxc_moveZ * 0.7071067811865476;
      }
      if (hxc_vitals.hxc_health > 0)
      {
        struct hxc_caxecraft_domain_PlayerState hxc_tmp_load_result_n239 = hxc_player;
        double hxc_tmp_load_result_n240 = hxc_moveX;
        double hxc_tmp_load_result_n241 = hxc_moveZ;
        struct hxc_caxecraft_domain_StepInput hxc_tmp_call_result_n243 = hxc_caxecraft_domain_PlayerPhysics_input(hxc_tmp_load_result_n240, hxc_tmp_load_result_n241, hxc_jumpQueued);
        struct hxc_caxecraft_domain_PlayerState hxc_tmp_call_result_n244 = hxc_caxecraft_domain_PlayerPhysics_step(hxc_cells, hxc_tmp_length_n1, hxc_tmp_load_result_n239, hxc_tmp_call_result_n243);
        hxc_player = hxc_tmp_call_result_n244;
      }
      if (hxc_selectedMode == 1)
      {
        if (hxc_vitals.hxc_health > 0)
        {
          struct hxc_caxecraft_gameplay_PlayerVitalsState hxc_tmp_call_result_n248 = hxc_caxecraft_gameplay_PlayerVitals_step(hxc_vitals);
          hxc_vitals = hxc_tmp_call_result_n248;
          struct hxc_caxecraft_gameplay_MosslingState hxc_tmp_load_result_n249 = hxc_mossling;
          double hxc_tmp_record_field_load_result_n250 = hxc_player.hxc_x;
          bool hxc_tmp_call_result_n252 = hxc_caxecraft_gameplay_Mossling_attacksThisTick(hxc_tmp_load_result_n249, hxc_tmp_record_field_load_result_n250, hxc_player.hxc_z);
          bool hxc_mosslingAttacked = hxc_tmp_call_result_n252;
          struct hxc_caxecraft_gameplay_PlayerVitalsState hxc_tmp_load_result_n253 = hxc_vitals;
          struct hxc_caxecraft_gameplay_PlayerVitalsState hxc_tmp_call_result_n255 = hxc_caxecraft_gameplay_PlayerVitals_applyAttack(hxc_tmp_load_result_n253, hxc_mosslingAttacked);
          hxc_vitals = hxc_tmp_call_result_n255;
          if (hxc_mosslingAttacked)
          {
            hxc_enemyAttackFrames = 120;
          }
          struct hxc_caxecraft_gameplay_MosslingState hxc_tmp_load_result_n257 = hxc_mossling;
          double hxc_tmp_record_field_load_result_n258 = hxc_player.hxc_x;
          double hxc_tmp_record_field_load_result_n259 = hxc_player.hxc_z;
          struct hxc_caxecraft_gameplay_MosslingState hxc_tmp_call_result_n261 = hxc_caxecraft_gameplay_Mossling_step(hxc_cells, hxc_tmp_length_n1, hxc_tmp_load_result_n257, hxc_tmp_record_field_load_result_n258, hxc_tmp_record_field_load_result_n259, hxc_updateCount);
          hxc_mossling = hxc_tmp_call_result_n261;
        }
        struct hxc_caxecraft_gameplay_SwordCombatState hxc_tmp_call_result_n263 = hxc_caxecraft_gameplay_SwordCombat_step(hxc_swordCombat);
        hxc_swordCombat = hxc_tmp_call_result_n263;
        if (hxc_swordQueued)
        {
          struct hxc_caxecraft_gameplay_SwordCombatState hxc_tmp_load_result_n265 = hxc_swordCombat;
          struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_load_result_n266 = hxc_inventory;
          struct hxc_caxecraft_gameplay_PlayerVitalsState hxc_tmp_load_result_n267 = hxc_vitals;
          struct hxc_caxecraft_gameplay_MosslingState hxc_tmp_load_result_n268 = hxc_mossling;
          double hxc_tmp_record_field_load_result_n269 = hxc_player.hxc_x;
          double hxc_tmp_record_field_load_result_n270 = hxc_player.hxc_z;
          double hxc_tmp_load_result_n271 = hxc_lookX;
          int32_t hxc_tmp_call_result_n273 = hxc_caxecraft_gameplay_SwordCombat_decide(hxc_tmp_load_result_n265, hxc_tmp_load_result_n266, hxc_tmp_load_result_n267, hxc_tmp_load_result_n268, hxc_tmp_record_field_load_result_n269, hxc_tmp_record_field_load_result_n270, hxc_tmp_load_result_n271, hxc_lookZ);
          int32_t hxc_swordDecision = hxc_tmp_call_result_n273;
          if (hxc_swordDecision == 5)
          {
            struct hxc_caxecraft_gameplay_MosslingState hxc_tmp_call_result_n276 = hxc_caxecraft_gameplay_Mossling_strike(hxc_mossling);
            hxc_mossling = hxc_tmp_call_result_n276;
            hxc_strikeHitFrames = 16;
            if (hxc_mossling.hxc_health <= 0)
            {
              struct hxc_caxecraft_gameplay_BerryDropState hxc_tmp_call_result_n279 = hxc_caxecraft_gameplay_BerryDrop_fromDefeatedMossling(hxc_mossling);
              hxc_berryDrop = hxc_tmp_call_result_n279;
              hxc_enemyDefeatedFrames = 120;
            }
          }
          int32_t hxc_tmp_load_result_n280 = hxc_swordDecision;
          struct hxc_caxecraft_gameplay_SwordCombatState hxc_tmp_call_result_n282 = hxc_caxecraft_gameplay_SwordCombat_after(hxc_tmp_load_result_n280, hxc_swordCombat);
          hxc_swordCombat = hxc_tmp_call_result_n282;
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
    double hxc_tmp_load_result_n288 = hxc_eyeX;
    double hxc_tmp_load_result_n289 = hxc_eyeY;
    double hxc_tmp_load_result_n290 = hxc_eyeZ;
    double hxc_tmp_load_result_n291 = hxc_lookX;
    double hxc_tmp_load_result_n292 = hxc_lookY;
    struct hxc_caxecraft_domain_RaycastHit hxc_tmp_call_result_n294 = hxc_caxecraft_domain_VoxelRaycast_trace(hxc_cells, hxc_tmp_length_n1, hxc_tmp_load_result_n288, hxc_tmp_load_result_n289, hxc_tmp_load_result_n290, hxc_tmp_load_result_n291, hxc_tmp_load_result_n292, hxc_lookZ, 7.0);
    struct hxc_caxecraft_domain_RaycastHit hxc_hit = hxc_tmp_call_result_n294;
    bool hxc_tmp_load_result_n295 = hxc_captured;
    bool hxc_tmp_short_circuit_result_n134 = hxc_tmp_load_result_n295;
    if (hxc_tmp_load_result_n295)
    {
      hxc_tmp_short_circuit_result_n134 = !hxc_recapturedThisFrame;
    }
    bool hxc_tmp_short_circuit_load_result_n297 = hxc_tmp_short_circuit_result_n134;
    bool hxc_tmp_short_circuit_result_n135 = hxc_tmp_short_circuit_load_result_n297;
    if (hxc_tmp_short_circuit_load_result_n297)
    {
      hxc_tmp_short_circuit_result_n135 = hxc_primaryPressed_h6b5af3f34586;
    }
    if (hxc_tmp_short_circuit_result_n135)
    {
      if (hxc_vitals.hxc_health > 0)
      {
        if (hxc_selectedMode == 1)
        {
          bool hxc_tmp_call_result_n304 = hxc_caxecraft_gameplay_Inventory_selectedIs(hxc_inventory, hxc_caxecraft_gameplay_ItemKind_CopperSword);
          bool hxc_tmp_short_circuit_result_n136 = !hxc_tmp_call_result_n304;
          if (!hxc_tmp_call_result_n304)
          {
            hxc_tmp_short_circuit_result_n136 = hxc_hit.hxc_hit;
          }
          if (hxc_tmp_short_circuit_result_n136)
          {
            int32_t hxc_tmp_record_field_load_result_n307 = hxc_hit.hxc_cellX;
            int32_t hxc_tmp_record_field_load_result_n308 = hxc_hit.hxc_cellY;
            struct hxc_caxecraft_domain_BlockCoord hxc_tmp_call_result_n310 = hxc_caxecraft_domain_World_coord(hxc_tmp_record_field_load_result_n307, hxc_tmp_record_field_load_result_n308, hxc_hit.hxc_cellZ);
            struct hxc_caxecraft_gameplay_MiningResult hxc_tmp_call_result_n312 = hxc_caxecraft_gameplay_Mining_attempt(hxc_cells, hxc_tmp_length_n1, hxc_tmp_call_result_n310, hxc_inventory);
            struct hxc_caxecraft_gameplay_MiningResult hxc_mining = hxc_tmp_call_result_n312;
            hxc_inventory = hxc_mining.hxc_inventory;
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
            int32_t hxc_tmp_record_field_load_result_n316 = hxc_hit.hxc_cellX;
            int32_t hxc_tmp_record_field_load_result_n317 = hxc_hit.hxc_cellY;
            struct hxc_caxecraft_domain_BlockCoord hxc_tmp_call_result_n319 = hxc_caxecraft_domain_World_coord(hxc_tmp_record_field_load_result_n316, hxc_tmp_record_field_load_result_n317, hxc_hit.hxc_cellZ);
            hxc_caxecraft_domain_World_remove(hxc_cells, hxc_tmp_length_n1, hxc_tmp_call_result_n319);
          }
        }
      }
    }
    bool hxc_tmp_load_result_n320 = hxc_captured;
    bool hxc_tmp_short_circuit_result_n138 = hxc_tmp_load_result_n320;
    if (hxc_tmp_load_result_n320)
    {
      hxc_tmp_short_circuit_result_n138 = hxc_secondaryPressed_h9f1e7ee32377;
    }
    if (hxc_tmp_short_circuit_result_n138)
    {
      if (hxc_vitals.hxc_health > 0)
      {
        struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_load_result_n324 = hxc_inventory;
        int32_t hxc_tmp_call_result_n326 = hxc_caxecraft_gameplay_Recovery_decide(hxc_tmp_load_result_n324, hxc_vitals);
        int32_t hxc_recoveryDecision = hxc_tmp_call_result_n326;
        if (hxc_recoveryDecision != 0)
        {
          hxc_recoveryFeedback = hxc_recoveryDecision;
          hxc_recoveryFeedbackFrames = 90;
          int32_t hxc_tmp_load_result_n329 = hxc_recoveryDecision;
          struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_call_result_n331 = hxc_caxecraft_gameplay_Recovery_applyInventory(hxc_tmp_load_result_n329, hxc_inventory);
          hxc_inventory = hxc_tmp_call_result_n331;
          int32_t hxc_tmp_load_result_n332 = hxc_recoveryDecision;
          struct hxc_caxecraft_gameplay_PlayerVitalsState hxc_tmp_call_result_n334 = hxc_caxecraft_gameplay_Recovery_applyVitals(hxc_tmp_load_result_n332, hxc_vitals);
          hxc_vitals = hxc_tmp_call_result_n334;
        }
        else
        {
          if (hxc_hit.hxc_hit)
          {
            int32_t hxc_tmp_record_field_load_result_n336 = hxc_hit.hxc_previousX;
            int32_t hxc_tmp_record_field_load_result_n337 = hxc_hit.hxc_previousY;
            struct hxc_caxecraft_domain_BlockCoord hxc_tmp_call_result_n339 = hxc_caxecraft_domain_World_coord(hxc_tmp_record_field_load_result_n336, hxc_tmp_record_field_load_result_n337, hxc_hit.hxc_previousZ);
            struct hxc_caxecraft_domain_BlockCoord hxc_placement = hxc_tmp_call_result_n339;
            enum hxc_caxecraft_domain_BlockKind hxc_tmp_call_result_n341 = hxc_caxecraft_gameplay_Inventory_selectedBlock(hxc_inventory);
            enum hxc_caxecraft_domain_BlockKind hxc_selectedBlock = hxc_tmp_call_result_n341;
            struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_load_result_n342 = hxc_inventory;
            int32_t hxc_tmp_call_result_n344 = hxc_caxecraft_gameplay_Inventory_countAt(hxc_tmp_load_result_n342, hxc_inventory.hxc_selected);
            bool hxc_hasItem = hxc_tmp_call_result_n344 > 0;
            bool hxc_tmp_load_result_n345 = hxc_hasItem;
            bool hxc_tmp_short_circuit_result_n143 = !hxc_tmp_load_result_n345;
            if (!!hxc_tmp_load_result_n345)
            {
              bool hxc_tmp_call_result_n347 = hxc_caxecraft_domain_World_isPlaceable(hxc_selectedBlock);
              hxc_tmp_short_circuit_result_n143 = !hxc_tmp_call_result_n347;
            }
            bool hxc_tmp_short_circuit_load_result_n348 = hxc_tmp_short_circuit_result_n143;
            bool hxc_tmp_short_circuit_result_n144 = hxc_tmp_short_circuit_load_result_n348;
            if (!hxc_tmp_short_circuit_load_result_n348)
            {
              struct hxc_caxecraft_domain_PlayerState hxc_tmp_load_result_n349 = hxc_player;
              bool hxc_tmp_call_result_n351 = hxc_caxecraft_domain_PlayerPhysics_canPlaceAt(hxc_tmp_load_result_n349, hxc_placement);
              hxc_tmp_short_circuit_result_n144 = !hxc_tmp_call_result_n351;
            }
            bool hxc_tmp_short_circuit_load_result_n352 = hxc_tmp_short_circuit_result_n144;
            bool hxc_tmp_short_circuit_result_n145 = hxc_tmp_short_circuit_load_result_n352;
            if (!hxc_tmp_short_circuit_load_result_n352)
            {
              struct hxc_caxecraft_domain_BlockCoord hxc_tmp_load_result_n353 = hxc_placement;
              bool hxc_tmp_call_result_n355 = hxc_caxecraft_domain_World_place(hxc_cells, hxc_tmp_length_n1, hxc_tmp_load_result_n353, hxc_selectedBlock);
              hxc_tmp_short_circuit_result_n145 = !hxc_tmp_call_result_n355;
            }
            if (hxc_tmp_short_circuit_result_n145)
            {
              hxc_placementBlockedFrames = 60;
            }
            else
            {
              if (hxc_selectedMode == 1)
              {
                struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_call_result_n359 = hxc_caxecraft_gameplay_Inventory_consumeSelected(hxc_inventory);
                hxc_inventory = hxc_tmp_call_result_n359;
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
      struct hxc_caxecraft_gameplay_BerryDropState hxc_tmp_load_result_n363 = hxc_berryDrop;
      double hxc_tmp_record_field_load_result_n364 = hxc_player.hxc_x;
      double hxc_tmp_record_field_load_result_n365 = hxc_player.hxc_y;
      bool hxc_tmp_call_result_n367 = hxc_caxecraft_gameplay_BerryDrop_isInRange(hxc_tmp_load_result_n363, hxc_tmp_record_field_load_result_n364, hxc_tmp_record_field_load_result_n365, hxc_player.hxc_z);
      if (hxc_tmp_call_result_n367)
      {
        struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_load_result_n368 = hxc_inventory;
        int32_t hxc_tmp_call_result_n371 = hxc_caxecraft_gameplay_Inventory_acceptedAmount(hxc_tmp_load_result_n368, hxc_caxecraft_gameplay_ItemKind_Berries, hxc_berryDrop.hxc_amount);
        int32_t hxc_acceptedDrop = hxc_tmp_call_result_n371;
        if (hxc_acceptedDrop > 0)
        {
          struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_load_result_n373 = hxc_inventory;
          struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_call_result_n376 = hxc_caxecraft_gameplay_Inventory_collectItem(hxc_tmp_load_result_n373, hxc_caxecraft_gameplay_ItemKind_Berries, hxc_acceptedDrop);
          hxc_inventory = hxc_tmp_call_result_n376;
          struct hxc_caxecraft_gameplay_BerryDropState hxc_tmp_load_result_n377 = hxc_berryDrop;
          struct hxc_caxecraft_gameplay_BerryDropState hxc_tmp_call_result_n379 = hxc_caxecraft_gameplay_BerryDrop_collectAmount(hxc_tmp_load_result_n377, hxc_acceptedDrop);
          hxc_berryDrop = hxc_tmp_call_result_n379;
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
    double hxc_tmp_load_result_n394 = hxc_eyeX;
    double hxc_tmp_load_result_n395 = hxc_eyeY;
    double hxc_tmp_load_result_n396 = hxc_eyeZ;
    double hxc_tmp_load_result_n398 = hxc_eyeX;
    double hxc_tmp_load_result_n399 = hxc_lookX;
    double hxc_tmp_load_result_n400 = hxc_eyeY;
    double hxc_tmp_load_result_n401 = hxc_lookY;
    double hxc_tmp_load_result_n402 = hxc_eyeZ;
    struct Camera3D hxc_camera = (struct Camera3D){ .position = (struct Vector3){ .x = (float)hxc_tmp_load_result_n394, .y = (float)hxc_tmp_load_result_n395, .z = (float)hxc_tmp_load_result_n396 }, .target = (struct Vector3){ .x = (float)(hxc_tmp_load_result_n398 + hxc_tmp_load_result_n399), .y = (float)(hxc_tmp_load_result_n400 + hxc_tmp_load_result_n401), .z = (float)(hxc_tmp_load_result_n402 + hxc_lookZ) }, .up = (struct Vector3){ .x = (float)0.0, .y = (float)1.0, .z = (float)0.0 }, .fovy = (float)70.0, .projection = 0 };
    BeginDrawing();
    if (hxc_onTitle)
    {
      int32_t hxc_tmp_native_call_result_n408 = GetScreenWidth();
      int32_t hxc_width_h69bfd750f525 = (int32_t)hxc_tmp_native_call_result_n408;
      int32_t hxc_tmp_native_call_result_n409 = GetScreenHeight();
      int32_t hxc_height_hfab986491444 = (int32_t)hxc_tmp_native_call_result_n409;
      if (hxc_titleTextureReady)
      {
        struct Color hxc_this1_he5197d02d4da = (struct Color){ .r = 255, .g = 255, .b = 255, .a = 255 };
        struct Texture hxc_tmp_load_result_n412 = hxc_titleTexture;
        double hxc_imageAspect = hxc_f64_divide_zero_safe((double)(int32_t)hxc_tmp_load_result_n412.width, (double)(int32_t)hxc_titleTexture.height);
        int32_t hxc_tmp_load_result_n416 = hxc_width_h69bfd750f525;
        double hxc_screenAspect = hxc_f64_divide_zero_safe((double)hxc_tmp_load_result_n416, (double)hxc_height_hfab986491444);
        double hxc_sourceX = 0.0;
        double hxc_sourceY = 0.0;
        double hxc_sourceWidth = (double)(int32_t)hxc_titleTexture.width;
        double hxc_sourceHeight = (double)(int32_t)hxc_titleTexture.height;
        double hxc_tmp_load_result_n422 = hxc_screenAspect;
        if (hxc_tmp_load_result_n422 > hxc_imageAspect)
        {
          struct Texture hxc_tmp_load_result_n424 = hxc_titleTexture;
          hxc_sourceHeight = hxc_f64_divide_zero_safe((double)(int32_t)hxc_tmp_load_result_n424.width, hxc_screenAspect);
          struct Texture hxc_tmp_load_result_n427 = hxc_titleTexture;
          hxc_sourceY = ((double)(int32_t)hxc_tmp_load_result_n427.height - hxc_sourceHeight) * 0.5;
        }
        else
        {
          struct Texture hxc_tmp_load_result_n430 = hxc_titleTexture;
          hxc_sourceWidth = (double)(int32_t)hxc_tmp_load_result_n430.height * hxc_screenAspect;
          struct Texture hxc_tmp_load_result_n433 = hxc_titleTexture;
          hxc_sourceX = ((double)(int32_t)hxc_tmp_load_result_n433.width - hxc_sourceWidth) * 0.5;
        }
        struct Texture hxc_tmp_load_result_n436 = hxc_titleTexture;
        double hxc_tmp_load_result_n437 = hxc_sourceX;
        double hxc_tmp_load_result_n438 = hxc_sourceY;
        double hxc_tmp_load_result_n439 = hxc_sourceWidth;
        double hxc_tmp_load_result_n440 = hxc_sourceHeight;
        int32_t hxc_tmp_load_result_n442 = hxc_width_h69bfd750f525;
        int32_t hxc_tmp_load_result_n443 = hxc_height_hfab986491444;
        DrawTexturePro(hxc_tmp_load_result_n436, (struct Rectangle){ .x = (float)hxc_tmp_load_result_n437, .y = (float)hxc_tmp_load_result_n438, .width = (float)hxc_tmp_load_result_n439, .height = (float)hxc_tmp_load_result_n440 }, (struct Rectangle){ .x = (float)0.0, .y = (float)0.0, .width = (float)((double)hxc_tmp_load_result_n442 + 0.0), .height = (float)((double)hxc_tmp_load_result_n443 + 0.0) }, (struct Vector2){ .x = (float)0.0, .y = (float)0.0 }, (float)0.0, hxc_this1_he5197d02d4da);
      }
      else
      {
        ClearBackground((struct Color){ .r = 126, .g = 190, .b = 201, .a = 255 });
      }
      struct Color hxc_this1_h6986c66fd29d = (struct Color){ .r = 4, .g = 14, .b = 23, .a = 132 };
      int32_t hxc_tmp_load_result_n449 = hxc_width_h69bfd750f525;
      int32_t hxc_tmp_load_result_n450 = hxc_height_hfab986491444;
      DrawRectangle((int32_t)0, (int32_t)0, (int32_t)hxc_tmp_load_result_n449, (int32_t)hxc_tmp_load_result_n450, hxc_this1_h6986c66fd29d);
      if (hxc_wordmarkTextureReady)
      {
        int32_t hxc_centerX = hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_width_h69bfd750f525, (double)2));
        struct Color hxc_this1_h07780785cb2c = (struct Color){ .r = 255, .g = 255, .b = 255, .a = 255 };
        double hxc_scale = hxc_f64_divide_zero_safe((double)560, (double)(int32_t)hxc_wordmarkTexture.width);
        double hxc_heightScale = hxc_f64_divide_zero_safe((double)132, (double)(int32_t)hxc_wordmarkTexture.height);
        double hxc_tmp_load_result_n459 = hxc_heightScale;
        if (hxc_tmp_load_result_n459 < hxc_scale)
        {
          hxc_scale = hxc_heightScale;
        }
        struct Texture hxc_tmp_load_result_n462 = hxc_wordmarkTexture;
        double hxc_width_h94b4f5f56085 = (double)(int32_t)hxc_tmp_load_result_n462.width * hxc_scale;
        struct Texture hxc_tmp_load_result_n465 = hxc_wordmarkTexture;
        double hxc_height_h87d9622e2051 = (double)(int32_t)hxc_tmp_load_result_n465.height * hxc_scale;
        double hxc_width1 = (double)(int32_t)hxc_wordmarkTexture.width + 0.0;
        double hxc_height1 = (double)(int32_t)hxc_wordmarkTexture.height + 0.0;
        struct Texture hxc_tmp_load_result_n472 = hxc_wordmarkTexture;
        double hxc_tmp_load_result_n473 = hxc_width1;
        double hxc_tmp_load_result_n474 = hxc_height1;
        int32_t hxc_tmp_load_result_n476 = hxc_centerX;
        double hxc_tmp_load_result_n477 = hxc_width_h94b4f5f56085;
        double hxc_tmp_load_result_n478 = hxc_width_h94b4f5f56085;
        double hxc_tmp_load_result_n479 = hxc_height_h87d9622e2051;
        DrawTexturePro(hxc_tmp_load_result_n472, (struct Rectangle){ .x = (float)0.0, .y = (float)0.0, .width = (float)hxc_tmp_load_result_n473, .height = (float)hxc_tmp_load_result_n474 }, (struct Rectangle){ .x = (float)((double)hxc_tmp_load_result_n476 - hxc_tmp_load_result_n477 * 0.5), .y = (float)28.0, .width = (float)hxc_tmp_load_result_n478, .height = (float)hxc_tmp_load_result_n479 }, (struct Vector2){ .x = (float)0.0, .y = (float)0.0 }, (float)0.0, hxc_this1_h07780785cb2c);
      }
      else
      {
        int32_t hxc_tmp_load_result_n483 = hxc_locale;
        hxc_caxecraft_localization_UiCatalog_draw(hxc_tmp_load_result_n483, 16, hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_width_h69bfd750f525, (double)2)), 92), 52, 34, (struct Color){ .r = 242, .g = 249, .b = 245, .a = 255 });
      }
      int32_t hxc_firstTop = hxc_i32_subtract_wrapping(hxc_height_hfab986491444, 225);
      int32_t hxc_left_ha53f7c7eed50 = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_width_h69bfd750f525, (double)2)), 180);
      if (hxc_selectedMode == 0)
      {
        struct Color hxc_this1_hf72bbf808c96 = (struct Color){ .r = 16, .g = 88, .b = 102, .a = 232 };
        int32_t hxc_tmp_load_result_n490 = hxc_left_ha53f7c7eed50;
        int32_t hxc_tmp_load_result_n491 = hxc_firstTop;
        DrawRectangle((int32_t)hxc_tmp_load_result_n490, (int32_t)hxc_tmp_load_result_n491, (int32_t)360, (int32_t)54, hxc_this1_hf72bbf808c96);
        struct Color hxc_this1_h45e7cd6f2d61 = (struct Color){ .r = 255, .g = 190, .b = 55, .a = 255 };
        int32_t hxc_tmp_load_result_n494 = hxc_left_ha53f7c7eed50;
        int32_t hxc_tmp_load_result_n495 = hxc_firstTop;
        DrawRectangleLines((int32_t)hxc_tmp_load_result_n494, (int32_t)hxc_tmp_load_result_n495, (int32_t)360, (int32_t)54, hxc_this1_h45e7cd6f2d61);
      }
      else
      {
        struct Color hxc_this1_h5ab6e6cdd588 = (struct Color){ .r = 6, .g = 26, .b = 36, .a = 218 };
        int32_t hxc_tmp_load_result_n498 = hxc_left_ha53f7c7eed50;
        int32_t hxc_tmp_load_result_n499 = hxc_firstTop;
        DrawRectangle((int32_t)hxc_tmp_load_result_n498, (int32_t)hxc_tmp_load_result_n499, (int32_t)360, (int32_t)54, hxc_this1_h5ab6e6cdd588);
        struct Color hxc_this1_h922c566f2fec = (struct Color){ .r = 92, .g = 194, .b = 188, .a = 255 };
        int32_t hxc_tmp_load_result_n502 = hxc_left_ha53f7c7eed50;
        int32_t hxc_tmp_load_result_n503 = hxc_firstTop;
        DrawRectangleLines((int32_t)hxc_tmp_load_result_n502, (int32_t)hxc_tmp_load_result_n503, (int32_t)360, (int32_t)54, hxc_this1_h922c566f2fec);
      }
      int32_t hxc_tmp_load_result_n505 = hxc_locale;
      int32_t hxc_tmp_load_result_n506 = hxc_left_ha53f7c7eed50;
      hxc_caxecraft_localization_UiCatalog_draw(hxc_tmp_load_result_n505, 10, hxc_i32_add_wrapping(hxc_tmp_load_result_n506, 28), hxc_i32_add_wrapping(hxc_firstTop, 16), 21, (struct Color){ .r = 242, .g = 249, .b = 245, .a = 255 });
      int32_t hxc_top = hxc_i32_add_wrapping(hxc_i32_add_wrapping(hxc_firstTop, 54), 14);
      int32_t hxc_left_h09350339afd4 = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_width_h69bfd750f525, (double)2)), 180);
      if (hxc_selectedMode == 1)
      {
        struct Color hxc_this1_hc18468f5e813 = (struct Color){ .r = 16, .g = 88, .b = 102, .a = 232 };
        int32_t hxc_tmp_load_result_n513 = hxc_left_h09350339afd4;
        int32_t hxc_tmp_load_result_n514 = hxc_top;
        DrawRectangle((int32_t)hxc_tmp_load_result_n513, (int32_t)hxc_tmp_load_result_n514, (int32_t)360, (int32_t)54, hxc_this1_hc18468f5e813);
        struct Color hxc_this1_hfe494e7f13a4 = (struct Color){ .r = 255, .g = 190, .b = 55, .a = 255 };
        int32_t hxc_tmp_load_result_n517 = hxc_left_h09350339afd4;
        int32_t hxc_tmp_load_result_n518 = hxc_top;
        DrawRectangleLines((int32_t)hxc_tmp_load_result_n517, (int32_t)hxc_tmp_load_result_n518, (int32_t)360, (int32_t)54, hxc_this1_hfe494e7f13a4);
      }
      else
      {
        struct Color hxc_this1_h00d59f9d7c58 = (struct Color){ .r = 6, .g = 26, .b = 36, .a = 218 };
        int32_t hxc_tmp_load_result_n521 = hxc_left_h09350339afd4;
        int32_t hxc_tmp_load_result_n522 = hxc_top;
        DrawRectangle((int32_t)hxc_tmp_load_result_n521, (int32_t)hxc_tmp_load_result_n522, (int32_t)360, (int32_t)54, hxc_this1_h00d59f9d7c58);
        struct Color hxc_this1_h0ee879d5f97c = (struct Color){ .r = 92, .g = 194, .b = 188, .a = 255 };
        int32_t hxc_tmp_load_result_n525 = hxc_left_h09350339afd4;
        int32_t hxc_tmp_load_result_n526 = hxc_top;
        DrawRectangleLines((int32_t)hxc_tmp_load_result_n525, (int32_t)hxc_tmp_load_result_n526, (int32_t)360, (int32_t)54, hxc_this1_h0ee879d5f97c);
      }
      int32_t hxc_tmp_load_result_n528 = hxc_locale;
      int32_t hxc_tmp_load_result_n529 = hxc_left_h09350339afd4;
      hxc_caxecraft_localization_UiCatalog_draw(hxc_tmp_load_result_n528, 9, hxc_i32_add_wrapping(hxc_tmp_load_result_n529, 28), hxc_i32_add_wrapping(hxc_top, 16), 21, (struct Color){ .r = 242, .g = 249, .b = 245, .a = 255 });
      int32_t hxc_tmp_load_result_n532 = hxc_locale;
      int32_t hxc_tmp_load_result_n533 = hxc_width_h69bfd750f525;
      hxc_caxecraft_localization_UiCatalog_draw(hxc_tmp_load_result_n532, 11, hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_tmp_load_result_n533, (double)2)), 285), hxc_i32_subtract_wrapping(hxc_height_hfab986491444, 58), 16, (struct Color){ .r = 229, .g = 241, .b = 235, .a = 255 });
      if (hxc_selectedMode == 1)
      {
        int32_t hxc_tmp_load_result_n537 = hxc_locale;
        int32_t hxc_tmp_load_result_n538 = hxc_width_h69bfd750f525;
        hxc_caxecraft_localization_FirstPlayableCatalog_draw(hxc_tmp_load_result_n537, 1, hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_tmp_load_result_n538, (double)2)), 230), hxc_i32_subtract_wrapping(hxc_firstTop, 36), 17, (struct Color){ .r = 255, .g = 205, .b = 91, .a = 255 });
      }
    }
    else
    {
      ClearBackground((struct Color){ .r = 126, .g = 190, .b = 201, .a = 255 });
      int32_t hxc_tmp_native_call_result_n542 = GetScreenWidth();
      int32_t hxc_sunX = hxc_i32_subtract_wrapping((int32_t)hxc_tmp_native_call_result_n542, 260);
      float hxc_radius_h46bab057cad6 = (float)42.0;
      struct Color hxc_this1_hdf3d0dd71701 = (struct Color){ .r = 255, .g = 226, .b = 136, .a = 255 };
      int32_t hxc_tmp_load_result_n544 = hxc_sunX;
      float hxc_tmp_load_result_n545 = hxc_radius_h46bab057cad6;
      DrawCircle((int32_t)hxc_tmp_load_result_n544, (int32_t)86, hxc_tmp_load_result_n545, hxc_this1_hdf3d0dd71701);
      float hxc_radius_he05f4e8a549e = (float)30.0;
      struct Color hxc_this1_hc32b20afb966 = (struct Color){ .r = 255, .g = 246, .b = 205, .a = 255 };
      int32_t hxc_tmp_load_result_n548 = hxc_sunX;
      float hxc_tmp_load_result_n549 = hxc_radius_he05f4e8a549e;
      DrawCircle((int32_t)hxc_tmp_load_result_n548, (int32_t)86, hxc_tmp_load_result_n549, hxc_this1_hc32b20afb966);
      BeginMode3D(hxc_camera);
      struct Texture hxc_tmp_load_result_n552 = hxc_terrainTexture;
      bool hxc_tmp_load_result_n553 = hxc_terrainTextureReady;
      double hxc_tmp_record_field_load_result_n554 = hxc_player.hxc_x;
      struct hxc_caxecraft_app_TerrainRenderCounters hxc_tmp_call_result_n556 = hxc_caxecraft_app_TerrainRenderer_draw(hxc_cells, hxc_tmp_length_n1, hxc_tmp_load_result_n552, hxc_tmp_load_result_n553, hxc_tmp_record_field_load_result_n554, hxc_player.hxc_z);
      struct hxc_caxecraft_app_TerrainRenderCounters hxc_renderCounters = hxc_tmp_call_result_n556;
      struct Camera3D hxc_tmp_load_result_n557 = hxc_camera;
      struct Texture hxc_tmp_load_result_n558 = hxc_entityTexture;
      bool hxc_tmp_load_result_n559 = hxc_entityTextureReady;
      struct hxc_caxecraft_gameplay_GuideState hxc_tmp_load_result_n560 = hxc_guide;
      struct hxc_caxecraft_gameplay_MosslingState hxc_tmp_load_result_n561 = hxc_mossling;
      hxc_caxecraft_app_Main_drawActors(hxc_tmp_load_result_n557, hxc_tmp_load_result_n558, hxc_tmp_load_result_n559, hxc_tmp_load_result_n560, hxc_tmp_load_result_n561, hxc_berryDrop);
      if (hxc_hit.hxc_hit)
      {
        double hxc_y = (double)hxc_hit.hxc_cellY + 0.5;
        double hxc_z = (double)hxc_hit.hxc_cellZ + 0.5;
        int32_t hxc_tmp_record_field_load_result_n566 = hxc_hit.hxc_cellX;
        double hxc_tmp_load_result_n567 = hxc_y;
        DrawCubeWires((struct Vector3){ .x = (float)((double)hxc_tmp_record_field_load_result_n566 + 0.5), .y = (float)hxc_tmp_load_result_n567, .z = (float)hxc_z }, (float)1.04, (float)1.04, (float)1.04, (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 });
      }
      EndMode3D();
      int32_t hxc_tmp_record_field_load_result_n571 = hxc_renderCounters.hxc_visible;
      int32_t hxc_tmp_record_field_load_result_n572 = hxc_renderCounters.hxc_drawCalls;
      int32_t hxc_tmp_load_result_n573 = hxc_frameCount;
      int32_t hxc_tmp_load_result_n574 = hxc_updateCount;
      bool hxc_tmp_load_result_n575 = hxc_paused;
      bool hxc_tmp_load_result_n576 = hxc_captured;
      int32_t hxc_tmp_load_result_n577 = hxc_placementBlockedFrames;
      struct hxc_caxecraft_domain_RaycastHit hxc_tmp_load_result_n578 = hxc_hit;
      double hxc_tmp_record_field_load_result_n579 = hxc_player.hxc_x;
      double hxc_tmp_record_field_load_result_n580 = hxc_player.hxc_z;
      int32_t hxc_tmp_load_result_n581 = hxc_selectedMode;
      int32_t hxc_tmp_load_result_n582 = hxc_locale;
      struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_load_result_n583 = hxc_inventory;
      struct hxc_caxecraft_gameplay_GuideState hxc_tmp_load_result_n584 = hxc_guide;
      struct hxc_caxecraft_gameplay_MosslingState hxc_tmp_load_result_n585 = hxc_mossling;
      struct hxc_caxecraft_gameplay_PlayerVitalsState hxc_tmp_load_result_n586 = hxc_vitals;
      int32_t hxc_tmp_load_result_n587 = hxc_strikeHitFrames;
      int32_t hxc_tmp_load_result_n588 = hxc_enemyDefeatedFrames;
      int32_t hxc_tmp_load_result_n589 = hxc_enemyAttackFrames;
      int32_t hxc_tmp_load_result_n590 = hxc_pickupFrames;
      int32_t hxc_tmp_load_result_n591 = hxc_pickupAmount;
      int32_t hxc_tmp_load_result_n592 = hxc_inventoryFullReason;
      int32_t hxc_tmp_load_result_n593 = hxc_recoveryFeedback;
      int32_t hxc_tmp_load_result_n594 = hxc_recoveryFeedbackFrames;
      struct Texture hxc_tmp_load_result_n595 = hxc_hudTexture;
      bool hxc_tmp_load_result_n596 = hxc_hudTextureReady;
      struct Texture hxc_tmp_load_result_n597 = hxc_itemTexture;
      hxc_caxecraft_app_Main_drawHud(hxc_tmp_record_field_load_result_n571, hxc_tmp_record_field_load_result_n572, hxc_tmp_load_result_n573, hxc_tmp_load_result_n574, hxc_tmp_load_result_n575, hxc_tmp_load_result_n576, hxc_tmp_load_result_n577 > 0, hxc_tmp_load_result_n578, hxc_tmp_record_field_load_result_n579, hxc_tmp_record_field_load_result_n580, hxc_tmp_load_result_n581, hxc_tmp_load_result_n582, hxc_tmp_load_result_n583, hxc_tmp_load_result_n584, hxc_tmp_load_result_n585, hxc_tmp_load_result_n586, hxc_tmp_load_result_n587 > 0, hxc_tmp_load_result_n588 > 0, hxc_tmp_load_result_n589 > 0, hxc_tmp_load_result_n590 > 0, hxc_tmp_load_result_n591, hxc_tmp_load_result_n592, hxc_tmp_load_result_n593, hxc_tmp_load_result_n594 > 0, hxc_tmp_load_result_n595, hxc_tmp_load_result_n596, hxc_tmp_load_result_n597, hxc_itemTextureReady);
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
  int32_t hxc_tmp_call_result_n0 = hxc_caxecraft_domain_World_surfaceY(hxc_cells, hxc_length, 16, 16);
  double hxc_spawnY = (double)hxc_tmp_call_result_n0 + 1.0;
  struct hxc_caxecraft_domain_PlayerState hxc_tmp_call_result_n2 = hxc_caxecraft_domain_PlayerPhysics_player(16.5, hxc_spawnY, 16.5);
  struct hxc_caxecraft_domain_PlayerState hxc_tmp_call_result_n3 = hxc_caxecraft_domain_PlayerPhysics_recoverSpawn(hxc_cells, hxc_length, hxc_tmp_call_result_n2);
  return hxc_tmp_call_result_n3;
}
