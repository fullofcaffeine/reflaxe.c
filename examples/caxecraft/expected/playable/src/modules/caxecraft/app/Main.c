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

void hxc_caxecraft_app_Main_drawHud(int32_t hxc_visible, int32_t hxc_drawCalls, int32_t hxc_frames, int32_t hxc_updates, bool hxc_paused, bool hxc_captured, bool hxc_placementBlocked, struct hxc_caxecraft_domain_RaycastHit hxc_hit, double hxc_playerX, double hxc_playerZ, int32_t hxc_mode, int32_t hxc_language, struct hxc_caxecraft_gameplay_InventoryState hxc_inventory, struct hxc_caxecraft_gameplay_GuideState hxc_guide, struct hxc_caxecraft_gameplay_MosslingState hxc_mossling, struct hxc_caxecraft_gameplay_PlayerVitalsState hxc_vitals, bool hxc_strikeHit, bool hxc_enemyDefeated, bool hxc_enemyAttacked, bool hxc_pickedUp, int32_t hxc_pickupAmount, int32_t hxc_inventoryFullReason, int32_t hxc_recoveryFeedback, bool hxc_recoveryVisible, struct Texture hxc_hudTexture, bool hxc_hudTextureReady, struct Texture hxc_itemTexture, bool hxc_itemTextureReady)
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
  DrawText("CAXECRAFT  //  C + HAXE", (int32_t)32, (int32_t)28, (int32_t)20, hxc_text);
  DrawText("CELLS", (int32_t)32, (int32_t)58, (int32_t)14, hxc_text);
  hxc_caxecraft_app_HudDigits_drawNumber(16384, 82, 59, 5, (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 });
  DrawText("VISIBLE", (int32_t)160, (int32_t)58, (int32_t)14, hxc_text);
  hxc_caxecraft_app_HudDigits_drawNumber(hxc_visible, 230, 59, 5, (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 });
  DrawText("DRAWS", (int32_t)326, (int32_t)58, (int32_t)14, hxc_text);
  hxc_caxecraft_app_HudDigits_drawNumber(hxc_drawCalls, 382, 59, 5, (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 });
  DrawText("FRAME", (int32_t)32, (int32_t)86, (int32_t)12, hxc_text);
  hxc_caxecraft_app_HudDigits_drawNumber(hxc_frames, 82, 85, 6, hxc_text);
  DrawText("TICK", (int32_t)174, (int32_t)86, (int32_t)12, hxc_text);
  hxc_caxecraft_app_HudDigits_drawNumber(hxc_updates, 216, 85, 6, hxc_text);
  int32_t hxc_tmp_load_result_n40 = hxc_width;
  hxc_caxecraft_app_Main_drawHotbar(hxc_inventory, hxc_hudTexture, hxc_hudTextureReady, hxc_itemTexture, hxc_itemTextureReady, hxc_tmp_load_result_n40, hxc_height);
  hxc_caxecraft_app_Main_drawHealth(hxc_vitals, hxc_hudTexture, hxc_hudTextureReady, hxc_width);
  int32_t hxc_tmp_load_result_n43 = hxc_height;
  DrawText("WASD MOVE  1-8/WHEEL ITEMS  SPACE JUMP  E TALK  LMB MINE/STRIKE  RMB USE  ESC PAUSE  Q QUIT", (int32_t)20, (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n43, 22), (int32_t)14, hxc_text);
  if (hxc_mode == 1)
  {
    if (hxc_language == 1)
    {
      struct Color hxc_this1_h0533aeb39716 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
      DrawText("AVENTURA: PROLOGO EN CONSTRUCCION", (int32_t)32, (int32_t)110, (int32_t)14, hxc_this1_h0533aeb39716);
    }
    else
    {
      struct Color hxc_this1_h4cfc27606bf9 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
      DrawText("ADVENTURE: PROLOGUE IN PROGRESS", (int32_t)32, (int32_t)110, (int32_t)14, hxc_this1_h4cfc27606bf9);
    }
  }
  bool hxc_tmp_call_result_n49 = hxc_caxecraft_gameplay_GuideNpc_isInRange(hxc_guide, hxc_playerX, hxc_playerZ);
  if (hxc_tmp_call_result_n49)
  {
    struct Color hxc_this1_hc5c060f86f9d = (struct Color){ .r = 15, .g = 27, .b = 39, .a = 220 };
    int32_t hxc_tmp_load_result_n51 = hxc_centerX;
    int32_t hxc_tmp_load_result_n52 = hxc_centerY;
    DrawRectangle((int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n51, 260), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n52, 54), (int32_t)520, (int32_t)60, hxc_this1_hc5c060f86f9d);
    int32_t hxc_code_hd4357aa6d3df = hxc_guide.hxc_phaseCode;
    int32_t hxc_tmp_load_result_n55 = hxc_code_hd4357aa6d3df;
    int32_t hxc_tmp_conditional_result_n39 = 0;
    if (hxc_tmp_load_result_n55 == 1)
    {
      hxc_tmp_conditional_result_n39 = 1;
    }
    else
    {
      int32_t hxc_tmp_load_result_n56 = hxc_code_hd4357aa6d3df;
      int32_t hxc_tmp_conditional_result_n40 = 0;
      if (hxc_tmp_load_result_n56 == 2)
      {
        hxc_tmp_conditional_result_n40 = 2;
      }
      else
      {
        hxc_tmp_conditional_result_n40 = 0;
      }
      hxc_tmp_conditional_result_n39 = hxc_tmp_conditional_result_n40;
    }
    if (hxc_tmp_conditional_result_n39 == 0)
    {
      if (hxc_language == 1)
      {
        int32_t hxc_tmp_load_result_n59 = hxc_centerX;
        int32_t hxc_tmp_load_result_n60 = hxc_centerY;
        DrawText("E  HABLAR CON NIA", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n59, 92), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n60, 74), (int32_t)18, hxc_text);
      }
      else
      {
        int32_t hxc_tmp_load_result_n62 = hxc_centerX;
        int32_t hxc_tmp_load_result_n63 = hxc_centerY;
        DrawText("E  TALK TO NIA", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n62, 74), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n63, 74), (int32_t)18, hxc_text);
      }
    }
    else
    {
      int32_t hxc_code_he9e787805bd4 = hxc_guide.hxc_phaseCode;
      int32_t hxc_tmp_load_result_n66 = hxc_code_he9e787805bd4;
      int32_t hxc_tmp_conditional_result_n42 = 0;
      if (hxc_tmp_load_result_n66 == 1)
      {
        hxc_tmp_conditional_result_n42 = 1;
      }
      else
      {
        int32_t hxc_tmp_load_result_n67 = hxc_code_he9e787805bd4;
        int32_t hxc_tmp_conditional_result_n43 = 0;
        if (hxc_tmp_load_result_n67 == 2)
        {
          hxc_tmp_conditional_result_n43 = 2;
        }
        else
        {
          hxc_tmp_conditional_result_n43 = 0;
        }
        hxc_tmp_conditional_result_n42 = hxc_tmp_conditional_result_n43;
      }
      if (hxc_tmp_conditional_result_n42 == 1)
      {
        if (hxc_language == 1)
        {
          int32_t hxc_tmp_load_result_n70 = hxc_centerX;
          int32_t hxc_tmp_load_result_n71 = hxc_centerY;
          DrawText("NIA: EL BOSQUE TE ESCUCHA. E: REGALO", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n70, 220), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n71, 74), (int32_t)16, hxc_text);
        }
        else
        {
          int32_t hxc_tmp_load_result_n73 = hxc_centerX;
          int32_t hxc_tmp_load_result_n74 = hxc_centerY;
          DrawText("NIA: THE GROVE LISTENS. E: A SMALL GIFT", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n73, 225), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n74, 74), (int32_t)16, hxc_text);
        }
      }
      else
      {
        if (hxc_language == 1)
        {
          int32_t hxc_tmp_load_result_n76 = hxc_centerX;
          int32_t hxc_tmp_load_result_n77 = hxc_centerY;
          DrawText("NIA: BAYAS PARA EL CAMINO, HAXIRIO", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n76, 205), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n77, 74), (int32_t)16, hxc_text);
        }
        else
        {
          int32_t hxc_tmp_load_result_n79 = hxc_centerX;
          int32_t hxc_tmp_load_result_n80 = hxc_centerY;
          DrawText("NIA: BERRIES FOR THE ROAD, HAXIRIO", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n79, 205), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n80, 74), (int32_t)16, hxc_text);
        }
      }
    }
  }
  if (hxc_mossling.hxc_health > 0)
  {
    int32_t hxc_code_h06d7f23fb1e8 = hxc_mossling.hxc_modeCode;
    int32_t hxc_tmp_load_result_n84 = hxc_code_h06d7f23fb1e8;
    int32_t hxc_tmp_conditional_result_n45 = 0;
    if (hxc_tmp_load_result_n84 == 1)
    {
      hxc_tmp_conditional_result_n45 = 1;
    }
    else
    {
      int32_t hxc_tmp_load_result_n85 = hxc_code_h06d7f23fb1e8;
      int32_t hxc_tmp_conditional_result_n46 = 0;
      if (hxc_tmp_load_result_n85 == 2)
      {
        hxc_tmp_conditional_result_n46 = 2;
      }
      else
      {
        int32_t hxc_tmp_load_result_n86 = hxc_code_h06d7f23fb1e8;
        int32_t hxc_tmp_conditional_result_n47 = 0;
        if (hxc_tmp_load_result_n86 == 3)
        {
          hxc_tmp_conditional_result_n47 = 3;
        }
        else
        {
          int32_t hxc_tmp_load_result_n87 = hxc_code_h06d7f23fb1e8;
          int32_t hxc_tmp_conditional_result_n48 = 0;
          if (hxc_tmp_load_result_n87 == 4)
          {
            hxc_tmp_conditional_result_n48 = 4;
          }
          else
          {
            int32_t hxc_tmp_load_result_n88 = hxc_code_h06d7f23fb1e8;
            int32_t hxc_tmp_conditional_result_n49 = 0;
            if (hxc_tmp_load_result_n88 == 5)
            {
              hxc_tmp_conditional_result_n49 = 5;
            }
            else
            {
              hxc_tmp_conditional_result_n49 = 0;
            }
            hxc_tmp_conditional_result_n48 = hxc_tmp_conditional_result_n49;
          }
          hxc_tmp_conditional_result_n47 = hxc_tmp_conditional_result_n48;
        }
        hxc_tmp_conditional_result_n46 = hxc_tmp_conditional_result_n47;
      }
      hxc_tmp_conditional_result_n45 = hxc_tmp_conditional_result_n46;
    }
    if (hxc_tmp_conditional_result_n45 == 4)
    {
      if (hxc_language == 1)
      {
        struct Color hxc_this1_h64319991a54e = (struct Color){ .r = 218, .g = 65, .b = 72, .a = 255 };
        int32_t hxc_tmp_load_result_n95 = hxc_width;
        DrawText("MUSGUITO CARGANDO: ESQUIVA", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n95, 300), (int32_t)28, (int32_t)16, hxc_this1_h64319991a54e);
      }
      else
      {
        struct Color hxc_this1_hd91fd74c7be1 = (struct Color){ .r = 218, .g = 65, .b = 72, .a = 255 };
        int32_t hxc_tmp_load_result_n98 = hxc_width;
        DrawText("MOSSLING WINDUP: DODGE", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n98, 265), (int32_t)28, (int32_t)16, hxc_this1_hd91fd74c7be1);
      }
    }
    else
    {
      int32_t hxc_code_h4d18289427b0 = hxc_mossling.hxc_modeCode;
      int32_t hxc_tmp_load_result_n101 = hxc_code_h4d18289427b0;
      int32_t hxc_tmp_conditional_result_n53 = 0;
      if (hxc_tmp_load_result_n101 == 1)
      {
        hxc_tmp_conditional_result_n53 = 1;
      }
      else
      {
        int32_t hxc_tmp_load_result_n102 = hxc_code_h4d18289427b0;
        int32_t hxc_tmp_conditional_result_n54 = 0;
        if (hxc_tmp_load_result_n102 == 2)
        {
          hxc_tmp_conditional_result_n54 = 2;
        }
        else
        {
          int32_t hxc_tmp_load_result_n103 = hxc_code_h4d18289427b0;
          int32_t hxc_tmp_conditional_result_n55 = 0;
          if (hxc_tmp_load_result_n103 == 3)
          {
            hxc_tmp_conditional_result_n55 = 3;
          }
          else
          {
            int32_t hxc_tmp_load_result_n104 = hxc_code_h4d18289427b0;
            int32_t hxc_tmp_conditional_result_n56 = 0;
            if (hxc_tmp_load_result_n104 == 4)
            {
              hxc_tmp_conditional_result_n56 = 4;
            }
            else
            {
              int32_t hxc_tmp_load_result_n105 = hxc_code_h4d18289427b0;
              int32_t hxc_tmp_conditional_result_n57 = 0;
              if (hxc_tmp_load_result_n105 == 5)
              {
                hxc_tmp_conditional_result_n57 = 5;
              }
              else
              {
                hxc_tmp_conditional_result_n57 = 0;
              }
              hxc_tmp_conditional_result_n56 = hxc_tmp_conditional_result_n57;
            }
            hxc_tmp_conditional_result_n55 = hxc_tmp_conditional_result_n56;
          }
          hxc_tmp_conditional_result_n54 = hxc_tmp_conditional_result_n55;
        }
        hxc_tmp_conditional_result_n53 = hxc_tmp_conditional_result_n54;
      }
      if (hxc_tmp_conditional_result_n53 == 1)
      {
        if (hxc_language == 1)
        {
          struct Color hxc_this1_h7d1362dfa4a5 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
          int32_t hxc_tmp_load_result_n112 = hxc_width;
          DrawText("MUSGUITO ALERTA", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n112, 180), (int32_t)28, (int32_t)16, hxc_this1_h7d1362dfa4a5);
        }
        else
        {
          struct Color hxc_this1_hb1681ec75131 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
          int32_t hxc_tmp_load_result_n115 = hxc_width;
          DrawText("MOSSLING ALERT", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n115, 180), (int32_t)28, (int32_t)16, hxc_this1_hb1681ec75131);
        }
      }
    }
  }
  if (hxc_strikeHit)
  {
    if (hxc_language == 1)
    {
      struct Color hxc_this1_h53dbabd6237c = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
      int32_t hxc_tmp_load_result_n118 = hxc_centerX;
      int32_t hxc_tmp_load_result_n119 = hxc_centerY;
      DrawText("GOLPE DE COBRE", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n118, 70), (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n119, 54), (int32_t)18, hxc_this1_h53dbabd6237c);
    }
    else
    {
      struct Color hxc_this1_he9ef8ee609ac = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
      int32_t hxc_tmp_load_result_n122 = hxc_centerX;
      int32_t hxc_tmp_load_result_n123 = hxc_centerY;
      DrawText("COPPER STRIKE", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n122, 70), (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n123, 54), (int32_t)18, hxc_this1_he9ef8ee609ac);
    }
  }
  if (hxc_enemyDefeated)
  {
    if (hxc_language == 1)
    {
      struct Color hxc_this1_h15f59d93a2a9 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
      int32_t hxc_tmp_load_result_n126 = hxc_width;
      DrawText("EL MUSGUITO SOLTO BAYAS", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n126, 275), (int32_t)54, (int32_t)16, hxc_this1_h15f59d93a2a9);
    }
    else
    {
      struct Color hxc_this1_hdb31a468123b = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
      int32_t hxc_tmp_load_result_n129 = hxc_width;
      DrawText("MOSSLING DROPPED BERRIES", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n129, 270), (int32_t)54, (int32_t)16, hxc_this1_hdb31a468123b);
    }
  }
  if (hxc_enemyAttacked)
  {
    if (hxc_language == 1)
    {
      struct Color hxc_this1_hff331e7b7513 = (struct Color){ .r = 218, .g = 65, .b = 72, .a = 255 };
      int32_t hxc_tmp_load_result_n132 = hxc_width;
      DrawText("ATAQUE AVISADO: ESQUIVA EL PROXIMO", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n132, 330), (int32_t)82, (int32_t)16, hxc_this1_hff331e7b7513);
    }
    else
    {
      struct Color hxc_this1_h1c65ea4ec9b3 = (struct Color){ .r = 218, .g = 65, .b = 72, .a = 255 };
      int32_t hxc_tmp_load_result_n135 = hxc_width;
      DrawText("TELEGRAPHED HIT: DODGE THE NEXT", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n135, 315), (int32_t)82, (int32_t)16, hxc_this1_h1c65ea4ec9b3);
    }
  }
  if (hxc_pickedUp)
  {
    if (hxc_language == 1)
    {
      if (hxc_pickupAmount == 1)
      {
        struct Color hxc_this1_hbcdbfec882dd = (struct Color){ .r = 174, .g = 78, .b = 136, .a = 255 };
        int32_t hxc_tmp_load_result_n138 = hxc_centerX;
        int32_t hxc_tmp_load_result_n139 = hxc_centerY;
        DrawText("+1 BAYA", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n138, 42), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n139, 24), (int32_t)18, hxc_this1_hbcdbfec882dd);
      }
      else
      {
        struct Color hxc_this1_h1a52349bac97 = (struct Color){ .r = 174, .g = 78, .b = 136, .a = 255 };
        int32_t hxc_tmp_load_result_n142 = hxc_centerX;
        int32_t hxc_tmp_load_result_n143 = hxc_centerY;
        DrawText("+2 BAYAS", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n142, 42), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n143, 24), (int32_t)18, hxc_this1_h1a52349bac97);
      }
    }
    else
    {
      if (hxc_pickupAmount == 1)
      {
        struct Color hxc_this1_hd05aac214782 = (struct Color){ .r = 174, .g = 78, .b = 136, .a = 255 };
        int32_t hxc_tmp_load_result_n146 = hxc_centerX;
        int32_t hxc_tmp_load_result_n147 = hxc_centerY;
        DrawText("+1 BERRY", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n146, 48), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n147, 24), (int32_t)18, hxc_this1_hd05aac214782);
      }
      else
      {
        struct Color hxc_this1_h7e2d37512316 = (struct Color){ .r = 174, .g = 78, .b = 136, .a = 255 };
        int32_t hxc_tmp_load_result_n150 = hxc_centerX;
        int32_t hxc_tmp_load_result_n151 = hxc_centerY;
        DrawText("+2 BERRIES", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n150, 48), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n151, 24), (int32_t)18, hxc_this1_h7e2d37512316);
      }
    }
  }
  if (hxc_inventoryFullReason == 1)
  {
    if (hxc_language == 1)
    {
      struct Color hxc_this1_h4f99a5a80074 = (struct Color){ .r = 238, .g = 113, .b = 78, .a = 255 };
      int32_t hxc_tmp_load_result_n154 = hxc_centerX;
      int32_t hxc_tmp_load_result_n155 = hxc_centerY;
      DrawText("BAYAS LLENAS: USA UNA PRIMERO", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n154, 150), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n155, 48), (int32_t)16, hxc_this1_h4f99a5a80074);
    }
    else
    {
      struct Color hxc_this1_h1e9d0c462ffb = (struct Color){ .r = 238, .g = 113, .b = 78, .a = 255 };
      int32_t hxc_tmp_load_result_n158 = hxc_centerX;
      int32_t hxc_tmp_load_result_n159 = hxc_centerY;
      DrawText("BERRIES FULL: USE ONE FIRST", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n158, 140), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n159, 48), (int32_t)16, hxc_this1_h1e9d0c462ffb);
    }
  }
  else
  {
    if (hxc_inventoryFullReason == 2)
    {
      if (hxc_language == 1)
      {
        struct Color hxc_this1_h285798b14584 = (struct Color){ .r = 238, .g = 113, .b = 78, .a = 255 };
        int32_t hxc_tmp_load_result_n162 = hxc_centerX;
        int32_t hxc_tmp_load_result_n163 = hxc_centerY;
        DrawText("PILA DE BLOQUES LLENA: USA UNO", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n162, 155), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n163, 48), (int32_t)16, hxc_this1_h285798b14584);
      }
      else
      {
        struct Color hxc_this1_heb2914908a6b = (struct Color){ .r = 238, .g = 113, .b = 78, .a = 255 };
        int32_t hxc_tmp_load_result_n166 = hxc_centerX;
        int32_t hxc_tmp_load_result_n167 = hxc_centerY;
        DrawText("BLOCK STACK FULL: USE ONE FIRST", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n166, 155), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n167, 48), (int32_t)16, hxc_this1_heb2914908a6b);
      }
    }
  }
  if (hxc_recoveryVisible)
  {
    if (hxc_recoveryFeedback == 1)
    {
      if (hxc_language == 1)
      {
        struct Color hxc_this1_h774040a471c4 = (struct Color){ .r = 94, .g = 212, .b = 136, .a = 255 };
        int32_t hxc_tmp_load_result_n170 = hxc_centerX;
        int32_t hxc_tmp_load_result_n171 = hxc_centerY;
        DrawText("BAYAS: +1 CORAZON", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n170, 86), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n171, 24), (int32_t)18, hxc_this1_h774040a471c4);
      }
      else
      {
        struct Color hxc_this1_ha773393f9dd6 = (struct Color){ .r = 94, .g = 212, .b = 136, .a = 255 };
        int32_t hxc_tmp_load_result_n174 = hxc_centerX;
        int32_t hxc_tmp_load_result_n175 = hxc_centerY;
        DrawText("BERRIES: +1 HEART", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n174, 88), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n175, 24), (int32_t)18, hxc_this1_ha773393f9dd6);
      }
    }
    else
    {
      if (hxc_recoveryFeedback == 2)
      {
        if (hxc_language == 1)
        {
          struct Color hxc_this1_h679a465030f6 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
          int32_t hxc_tmp_load_result_n178 = hxc_centerX;
          int32_t hxc_tmp_load_result_n179 = hxc_centerY;
          DrawText("SALUD COMPLETA", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n178, 72), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n179, 24), (int32_t)18, hxc_this1_h679a465030f6);
        }
        else
        {
          struct Color hxc_this1_ha11faeda195f = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
          int32_t hxc_tmp_load_result_n182 = hxc_centerX;
          int32_t hxc_tmp_load_result_n183 = hxc_centerY;
          DrawText("HEALTH ALREADY FULL", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n182, 96), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n183, 24), (int32_t)18, hxc_this1_ha11faeda195f);
        }
      }
      else
      {
        if (hxc_recoveryFeedback == 3)
        {
          if (hxc_language == 1)
          {
            struct Color hxc_this1_hc34f80e196e5 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
            int32_t hxc_tmp_load_result_n186 = hxc_centerX;
            int32_t hxc_tmp_load_result_n187 = hxc_centerY;
            DrawText("NO QUEDAN BAYAS", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n186, 76), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n187, 24), (int32_t)18, hxc_this1_hc34f80e196e5);
          }
          else
          {
            struct Color hxc_this1_h437e1e069913 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
            int32_t hxc_tmp_load_result_n190 = hxc_centerX;
            int32_t hxc_tmp_load_result_n191 = hxc_centerY;
            DrawText("NO BERRIES LEFT", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n190, 76), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n191, 24), (int32_t)18, hxc_this1_h437e1e069913);
          }
        }
      }
    }
  }
  if (hxc_vitals.hxc_safeTicks > 15)
  {
    struct Color hxc_this1_hdb03431ef445 = (struct Color){ .r = 218, .g = 65, .b = 72, .a = 255 };
    int32_t hxc_tmp_load_result_n195 = hxc_width;
    int32_t hxc_tmp_load_result_n196 = hxc_height;
    DrawRectangleLines((int32_t)4, (int32_t)4, (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n195, 8), (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n196, 8), hxc_this1_hdb03431ef445);
  }
  if (hxc_vitals.hxc_health <= 0)
  {
    struct Color hxc_this1_hab6b7960626a = (struct Color){ .r = 15, .g = 27, .b = 39, .a = 220 };
    int32_t hxc_tmp_load_result_n200 = hxc_centerX;
    int32_t hxc_tmp_load_result_n201 = hxc_centerY;
    DrawRectangle((int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n200, 250), (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n201, 74), (int32_t)500, (int32_t)148, hxc_this1_hab6b7960626a);
    struct Color hxc_this1_h37ba335d4fad = (struct Color){ .r = 218, .g = 65, .b = 72, .a = 255 };
    int32_t hxc_tmp_load_result_n204 = hxc_centerX;
    int32_t hxc_tmp_load_result_n205 = hxc_centerY;
    DrawRectangleLines((int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n204, 250), (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n205, 74), (int32_t)500, (int32_t)148, hxc_this1_h37ba335d4fad);
    if (hxc_language == 1)
    {
      int32_t hxc_tmp_load_result_n207 = hxc_centerX;
      int32_t hxc_tmp_load_result_n208 = hxc_centerY;
      DrawText("HAXIRIO HA CAIDO", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n207, 112), (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n208, 42), (int32_t)24, hxc_text);
      struct Color hxc_this1_h1831952d1c46 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
      int32_t hxc_tmp_load_result_n211 = hxc_centerX;
      int32_t hxc_tmp_load_result_n212 = hxc_centerY;
      DrawText("E  VOLVER AL PRADO", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n211, 105), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n212, 10), (int32_t)18, hxc_this1_h1831952d1c46);
    }
    else
    {
      int32_t hxc_tmp_load_result_n214 = hxc_centerX;
      int32_t hxc_tmp_load_result_n215 = hxc_centerY;
      DrawText("HAXIRIO HAS FALLEN", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n214, 122), (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n215, 42), (int32_t)24, hxc_text);
      struct Color hxc_this1_h2555975b3c85 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
      int32_t hxc_tmp_load_result_n218 = hxc_centerX;
      int32_t hxc_tmp_load_result_n219 = hxc_centerY;
      DrawText("E  RETURN TO THE MEADOW", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n218, 125), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n219, 10), (int32_t)18, hxc_this1_h2555975b3c85);
    }
  }
  if (hxc_paused)
  {
    struct Color hxc_this1_hfe687bdd0705 = (struct Color){ .r = 15, .g = 27, .b = 39, .a = 220 };
    int32_t hxc_tmp_load_result_n222 = hxc_centerX;
    int32_t hxc_tmp_load_result_n223 = hxc_centerY;
    DrawRectangle((int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n222, 170), (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n223, 48), (int32_t)340, (int32_t)96, hxc_this1_hfe687bdd0705);
    struct Color hxc_this1_hafd184eefcda = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
    int32_t hxc_tmp_load_result_n226 = hxc_centerX;
    int32_t hxc_tmp_load_result_n227 = hxc_centerY;
    DrawRectangleLines((int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n226, 170), (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n227, 48), (int32_t)340, (int32_t)96, hxc_this1_hafd184eefcda);
    int32_t hxc_tmp_load_result_n229 = hxc_centerX;
    int32_t hxc_tmp_load_result_n230 = hxc_centerY;
    DrawText("PAUSED", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n229, 48), (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n230, 30), (int32_t)24, hxc_text);
    int32_t hxc_tmp_load_result_n232 = hxc_centerX;
    int32_t hxc_tmp_load_result_n233 = hxc_centerY;
    DrawText("CLICK TO CAPTURE  //  Q TO QUIT", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n232, 145), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n233, 8), (int32_t)16, hxc_text);
  }
  else
  {
    if (hxc_placementBlocked)
    {
      struct Color hxc_this1_h376da34fad20 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
      int32_t hxc_tmp_load_result_n236 = hxc_centerX;
      int32_t hxc_tmp_load_result_n237 = hxc_centerY;
      DrawText("PLACE BLOCKED: PLAYER OR WORLD EDGE", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n236, 150), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n237, 26), (int32_t)14, hxc_this1_h376da34fad20);
    }
    else
    {
      if (!hxc_captured)
      {
        int32_t hxc_tmp_load_result_n239 = hxc_centerX;
        int32_t hxc_tmp_load_result_n240 = hxc_centerY;
        DrawText("CLICK TO CAPTURE", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n239, 74), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n240, 26), (int32_t)14, hxc_text);
      }
      else
      {
        if (!hxc_hit.hxc_hit)
        {
          int32_t hxc_tmp_load_result_n243 = hxc_centerX;
          int32_t hxc_tmp_load_result_n244 = hxc_centerY;
          DrawText("NO BLOCK IN REACH", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n243, 78), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n244, 26), (int32_t)14, hxc_text);
        }
      }
    }
  }
  return;
}

struct hxc_caxecraft_app_Main_RenderCounters hxc_caxecraft_app_Main_drawWorld(uint8_t *hxc_cells, size_t hxc_length, double hxc_playerX, double hxc_playerZ)
{
  uint8_t *hxc_borrow = hxc_cells;
  size_t hxc_tmp_length_n3 = hxc_length;
  (void)hxc_borrow;
  (void)hxc_tmp_length_n3;
  int32_t hxc_visible = 0;
  int32_t hxc_drawCalls = 0;
  int32_t hxc_minimumX = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_playerX), 9);
  if (hxc_minimumX < 0)
  {
    hxc_minimumX = 0;
  }
  int32_t hxc_maximumX = hxc_i32_add_wrapping(hxc_f64_to_i32_saturating(hxc_playerX), 9);
  if (hxc_maximumX >= 32)
  {
    hxc_maximumX = 31;
  }
  int32_t hxc_minimumZ = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_playerZ), 9);
  if (hxc_minimumZ < 0)
  {
    hxc_minimumZ = 0;
  }
  int32_t hxc_maximumZ = hxc_i32_add_wrapping(hxc_f64_to_i32_saturating(hxc_playerZ), 9);
  if (hxc_maximumZ >= 32)
  {
    hxc_maximumZ = 31;
  }
  int32_t hxc_z = hxc_minimumZ;
  while (1)
  {
    int32_t hxc_tmp_load_result_n5 = hxc_z;
    if (!(hxc_tmp_load_result_n5 <= hxc_maximumZ))
    {
      break;
    }
    int32_t hxc_y = 0;
    while (1)
    {
      if (!(hxc_y < 16))
      {
        break;
      }
      int32_t hxc_x = hxc_minimumX;
      while (1)
      {
        int32_t hxc_tmp_load_result_n9 = hxc_x;
        if (!(hxc_tmp_load_result_n9 <= hxc_maximumX))
        {
          break;
        }
        int32_t hxc_tmp_load_result_n11 = hxc_x;
        int32_t hxc_tmp_load_result_n12 = hxc_y;
        struct hxc_caxecraft_domain_BlockCoord hxc_tmp_call_result_n14 = hxc_caxecraft_domain_World_coord(hxc_tmp_load_result_n11, hxc_tmp_load_result_n12, hxc_z);
        enum hxc_caxecraft_domain_BlockKind hxc_tmp_call_result_n15 = hxc_caxecraft_domain_World_query(hxc_cells, hxc_length, hxc_tmp_call_result_n14);
        enum hxc_caxecraft_domain_BlockKind hxc_kind = hxc_tmp_call_result_n15;
        bool hxc_tmp_call_result_n17 = hxc_caxecraft_domain_World_isSolid(hxc_kind);
        bool hxc_tmp_short_circuit_result_n14 = hxc_tmp_call_result_n17;
        if (hxc_tmp_call_result_n17)
        {
          int32_t hxc_tmp_load_result_n18 = hxc_x;
          int32_t hxc_tmp_load_result_n19 = hxc_y;
          bool hxc_tmp_call_result_n21 = hxc_caxecraft_app_Main_isExposed(hxc_cells, hxc_length, hxc_tmp_load_result_n18, hxc_tmp_load_result_n19, hxc_z);
          hxc_tmp_short_circuit_result_n14 = hxc_tmp_call_result_n21;
        }
        if (hxc_tmp_short_circuit_result_n14)
        {
          hxc_visible = hxc_i32_add_wrapping(hxc_visible, 1);
          hxc_drawCalls = hxc_i32_add_wrapping(hxc_drawCalls, 1);
          int32_t hxc_tmp_load_result_n25 = hxc_x;
          int32_t hxc_tmp_load_result_n26 = hxc_y;
          int32_t hxc_tmp_load_result_n27 = hxc_z;
          struct Color hxc_tmp_call_result_n30 = hxc_caxecraft_app_CaxecraftPalette_block(hxc_kind);
          DrawCube((struct Vector3){ .x = (float)((double)hxc_tmp_load_result_n25 + 0.5), .y = (float)((double)hxc_tmp_load_result_n26 + 0.5), .z = (float)((double)hxc_tmp_load_result_n27 + 0.5) }, (float)1.0, (float)1.0, (float)1.0, hxc_tmp_call_result_n30);
        }
        hxc_x = hxc_i32_add_wrapping(hxc_x, 1);
      }
      hxc_y = hxc_i32_add_wrapping(hxc_y, 1);
    }
    hxc_z = hxc_i32_add_wrapping(hxc_z, 1);
  }
  int32_t hxc_tmp_load_result_n34 = hxc_visible;
  return (struct hxc_caxecraft_app_Main_RenderCounters){ .hxc_drawCalls = hxc_drawCalls, .hxc_visible = hxc_tmp_load_result_n34 };
}

bool hxc_caxecraft_app_Main_isExposed(uint8_t *hxc_cells, size_t hxc_length, int32_t hxc_x, int32_t hxc_y, int32_t hxc_z)
{
  uint8_t *hxc_borrow = hxc_cells;
  size_t hxc_tmp_length_n4 = hxc_length;
  (void)hxc_borrow;
  (void)hxc_tmp_length_n4;
  struct hxc_caxecraft_domain_BlockCoord hxc_tmp_call_result_n0 = hxc_caxecraft_domain_World_coord(hxc_i32_subtract_wrapping(hxc_x, 1), hxc_y, hxc_z);
  enum hxc_caxecraft_domain_BlockKind hxc_tmp_call_result_n1 = hxc_caxecraft_domain_World_query(hxc_cells, hxc_length, hxc_tmp_call_result_n0);
  bool hxc_tmp_call_result_n2 = hxc_caxecraft_domain_World_isSolid(hxc_tmp_call_result_n1);
  bool hxc_tmp_short_circuit_result_n5 = !hxc_tmp_call_result_n2;
  if (!!hxc_tmp_call_result_n2)
  {
    struct hxc_caxecraft_domain_BlockCoord hxc_tmp_call_result_n3 = hxc_caxecraft_domain_World_coord(hxc_i32_add_wrapping(hxc_x, 1), hxc_y, hxc_z);
    enum hxc_caxecraft_domain_BlockKind hxc_tmp_call_result_n4 = hxc_caxecraft_domain_World_query(hxc_cells, hxc_length, hxc_tmp_call_result_n3);
    bool hxc_tmp_call_result_n5 = hxc_caxecraft_domain_World_isSolid(hxc_tmp_call_result_n4);
    hxc_tmp_short_circuit_result_n5 = !hxc_tmp_call_result_n5;
  }
  bool hxc_tmp_short_circuit_load_result_n6 = hxc_tmp_short_circuit_result_n5;
  bool hxc_tmp_short_circuit_result_n6 = hxc_tmp_short_circuit_load_result_n6;
  if (!hxc_tmp_short_circuit_load_result_n6)
  {
    struct hxc_caxecraft_domain_BlockCoord hxc_tmp_call_result_n7 = hxc_caxecraft_domain_World_coord(hxc_x, hxc_i32_subtract_wrapping(hxc_y, 1), hxc_z);
    enum hxc_caxecraft_domain_BlockKind hxc_tmp_call_result_n8 = hxc_caxecraft_domain_World_query(hxc_cells, hxc_length, hxc_tmp_call_result_n7);
    bool hxc_tmp_call_result_n9 = hxc_caxecraft_domain_World_isSolid(hxc_tmp_call_result_n8);
    hxc_tmp_short_circuit_result_n6 = !hxc_tmp_call_result_n9;
  }
  bool hxc_tmp_short_circuit_load_result_n10 = hxc_tmp_short_circuit_result_n6;
  bool hxc_tmp_short_circuit_result_n7 = hxc_tmp_short_circuit_load_result_n10;
  if (!hxc_tmp_short_circuit_load_result_n10)
  {
    struct hxc_caxecraft_domain_BlockCoord hxc_tmp_call_result_n11 = hxc_caxecraft_domain_World_coord(hxc_x, hxc_i32_add_wrapping(hxc_y, 1), hxc_z);
    enum hxc_caxecraft_domain_BlockKind hxc_tmp_call_result_n12 = hxc_caxecraft_domain_World_query(hxc_cells, hxc_length, hxc_tmp_call_result_n11);
    bool hxc_tmp_call_result_n13 = hxc_caxecraft_domain_World_isSolid(hxc_tmp_call_result_n12);
    hxc_tmp_short_circuit_result_n7 = !hxc_tmp_call_result_n13;
  }
  bool hxc_tmp_short_circuit_load_result_n14 = hxc_tmp_short_circuit_result_n7;
  bool hxc_tmp_short_circuit_result_n8 = hxc_tmp_short_circuit_load_result_n14;
  if (!hxc_tmp_short_circuit_load_result_n14)
  {
    struct hxc_caxecraft_domain_BlockCoord hxc_tmp_call_result_n15 = hxc_caxecraft_domain_World_coord(hxc_x, hxc_y, hxc_i32_subtract_wrapping(hxc_z, 1));
    enum hxc_caxecraft_domain_BlockKind hxc_tmp_call_result_n16 = hxc_caxecraft_domain_World_query(hxc_cells, hxc_length, hxc_tmp_call_result_n15);
    bool hxc_tmp_call_result_n17 = hxc_caxecraft_domain_World_isSolid(hxc_tmp_call_result_n16);
    hxc_tmp_short_circuit_result_n8 = !hxc_tmp_call_result_n17;
  }
  bool hxc_tmp_short_circuit_load_result_n18 = hxc_tmp_short_circuit_result_n8;
  bool hxc_tmp_short_circuit_result_n9 = hxc_tmp_short_circuit_load_result_n18;
  if (!hxc_tmp_short_circuit_load_result_n18)
  {
    struct hxc_caxecraft_domain_BlockCoord hxc_tmp_call_result_n19 = hxc_caxecraft_domain_World_coord(hxc_x, hxc_y, hxc_i32_add_wrapping(hxc_z, 1));
    enum hxc_caxecraft_domain_BlockKind hxc_tmp_call_result_n20 = hxc_caxecraft_domain_World_query(hxc_cells, hxc_length, hxc_tmp_call_result_n19);
    bool hxc_tmp_call_result_n21 = hxc_caxecraft_domain_World_isSolid(hxc_tmp_call_result_n20);
    hxc_tmp_short_circuit_result_n9 = !hxc_tmp_call_result_n21;
  }
  return hxc_tmp_short_circuit_result_n9;
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
  struct hxc_caxecraft_domain_PlayerState hxc_tmp_call_result_n17 = hxc_caxecraft_app_Main_spawnPlayer(hxc_cells, hxc_tmp_length_n1);
  struct hxc_caxecraft_domain_PlayerState hxc_player = hxc_tmp_call_result_n17;
  struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_call_result_n18 = hxc_caxecraft_gameplay_Inventory_starter();
  struct hxc_caxecraft_gameplay_InventoryState hxc_inventory = hxc_tmp_call_result_n18;
  struct hxc_caxecraft_gameplay_GuideState hxc_tmp_call_result_n19 = hxc_caxecraft_gameplay_GuideNpc_start(hxc_cells, hxc_tmp_length_n1, 17.5, 13.5);
  struct hxc_caxecraft_gameplay_GuideState hxc_guide = hxc_tmp_call_result_n19;
  struct hxc_caxecraft_gameplay_MosslingState hxc_tmp_call_result_n20 = hxc_caxecraft_gameplay_Mossling_start(hxc_cells, hxc_tmp_length_n1, 15.5, 13.8);
  struct hxc_caxecraft_gameplay_MosslingState hxc_mossling = hxc_tmp_call_result_n20;
  int32_t hxc_initialHealth = 6;
  struct hxc_caxecraft_gameplay_PlayerVitalsState hxc_tmp_call_result_n22 = hxc_caxecraft_gameplay_PlayerVitals_make(hxc_initialHealth, 0);
  struct hxc_caxecraft_gameplay_PlayerVitalsState hxc_vitals = hxc_tmp_call_result_n22;
  struct hxc_caxecraft_gameplay_SwordCombatState hxc_swordCombat = (struct hxc_caxecraft_gameplay_SwordCombatState){ .hxc_cooldownTicks = 0 };
  struct hxc_caxecraft_gameplay_BerryDropState hxc_berryDrop = (struct hxc_caxecraft_gameplay_BerryDropState){ .hxc_active = false, .hxc_amount = 0, .hxc_x = 0.0, .hxc_y = 0.0, .hxc_z = 0.0 };
  double hxc_lookX = 0.0;
  double hxc_lookY = -0.18;
  double hxc_lookZ = -1.0;
  double hxc_accumulator = 0.0;
  bool hxc_jumpQueued = false;
  bool hxc_swordQueued = false;
  int32_t hxc_selectedMode = 0;
  int32_t hxc_language = 0;
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
    bool hxc_tmp_load_result_n25 = hxc_quit;
    bool hxc_tmp_short_circuit_result_n45 = !hxc_tmp_load_result_n25;
    if (!hxc_tmp_load_result_n25)
    {
      bool hxc_tmp_native_call_result_n26 = WindowShouldClose();
      hxc_tmp_short_circuit_result_n45 = !hxc_tmp_native_call_result_n26;
    }
    if (!hxc_tmp_short_circuit_result_n45)
    {
      break;
    }
    bool hxc_recapturedThisFrame = false;
    bool hxc_tmp_native_call_result_n28 = IsWindowFocused();
    bool hxc_focused = hxc_tmp_native_call_result_n28;
    double hxc_forward = 0.0;
    double hxc_right = 0.0;
    bool hxc_tmp_native_call_result_n29 = IsKeyDown((int32_t)87);
    if (hxc_tmp_native_call_result_n29)
    {
      hxc_forward = hxc_forward + 1.0;
    }
    bool hxc_tmp_native_call_result_n31 = IsKeyDown((int32_t)83);
    if (hxc_tmp_native_call_result_n31)
    {
      hxc_forward = hxc_forward - 1.0;
    }
    bool hxc_tmp_native_call_result_n33 = IsKeyDown((int32_t)68);
    if (hxc_tmp_native_call_result_n33)
    {
      hxc_right = hxc_right + 1.0;
    }
    bool hxc_tmp_native_call_result_n35 = IsKeyDown((int32_t)65);
    if (hxc_tmp_native_call_result_n35)
    {
      hxc_right = hxc_right - 1.0;
    }
    double hxc_lookYaw_ha385bdd9fdd3 = 0.0;
    double hxc_lookPitch_h4b45b8f192d5 = 0.0;
    if (hxc_captured)
    {
      struct Vector2 hxc_tmp_native_call_result_n38 = GetMouseDelta();
      struct Vector2 hxc_mouse = hxc_tmp_native_call_result_n38;
      hxc_lookYaw_ha385bdd9fdd3 = -(double)hxc_mouse.x * 0.0025;
      hxc_lookPitch_h4b45b8f192d5 = -(double)hxc_mouse.y * 0.0025;
    }
    bool hxc_tmp_native_call_result_n43 = IsMouseButtonPressed((int32_t)0);
    bool hxc_leftPressed = hxc_tmp_native_call_result_n43;
    bool hxc_tmp_load_result_n44 = hxc_captured;
    bool hxc_tmp_short_circuit_result_n55 = hxc_tmp_load_result_n44;
    if (hxc_tmp_load_result_n44)
    {
      hxc_tmp_short_circuit_result_n55 = hxc_leftPressed;
    }
    bool hxc_primaryPressed_h7fcfc8272604 = hxc_tmp_short_circuit_result_n55;
    bool hxc_tmp_load_result_n47 = hxc_captured;
    bool hxc_tmp_short_circuit_result_n57 = hxc_tmp_load_result_n47;
    if (hxc_tmp_load_result_n47)
    {
      bool hxc_tmp_native_call_result_n48 = IsMouseButtonPressed((int32_t)1);
      hxc_tmp_short_circuit_result_n57 = hxc_tmp_native_call_result_n48;
    }
    bool hxc_secondaryPressed_h3cf9facec78b = hxc_tmp_short_circuit_result_n57;
    bool hxc_tmp_load_result_n50 = hxc_captured;
    bool hxc_tmp_short_circuit_result_n59 = hxc_tmp_load_result_n50;
    if (hxc_tmp_load_result_n50)
    {
      bool hxc_tmp_native_call_result_n51 = IsKeyPressed((int32_t)69);
      hxc_tmp_short_circuit_result_n59 = hxc_tmp_native_call_result_n51;
    }
    bool hxc_interactPressed_h17ebab6f5feb = hxc_tmp_short_circuit_result_n59;
    int32_t hxc_hotbarSelection_he7b00ccedb33 = -1;
    bool hxc_tmp_native_call_result_n53 = IsKeyPressed((int32_t)49);
    if (hxc_tmp_native_call_result_n53)
    {
      hxc_hotbarSelection_he7b00ccedb33 = 0;
    }
    bool hxc_tmp_native_call_result_n54 = IsKeyPressed((int32_t)50);
    if (hxc_tmp_native_call_result_n54)
    {
      hxc_hotbarSelection_he7b00ccedb33 = 1;
    }
    bool hxc_tmp_native_call_result_n55 = IsKeyPressed((int32_t)51);
    if (hxc_tmp_native_call_result_n55)
    {
      hxc_hotbarSelection_he7b00ccedb33 = 2;
    }
    bool hxc_tmp_native_call_result_n56 = IsKeyPressed((int32_t)52);
    if (hxc_tmp_native_call_result_n56)
    {
      hxc_hotbarSelection_he7b00ccedb33 = 3;
    }
    bool hxc_tmp_native_call_result_n57 = IsKeyPressed((int32_t)53);
    if (hxc_tmp_native_call_result_n57)
    {
      hxc_hotbarSelection_he7b00ccedb33 = 4;
    }
    bool hxc_tmp_native_call_result_n58 = IsKeyPressed((int32_t)54);
    if (hxc_tmp_native_call_result_n58)
    {
      hxc_hotbarSelection_he7b00ccedb33 = 5;
    }
    bool hxc_tmp_native_call_result_n59 = IsKeyPressed((int32_t)55);
    if (hxc_tmp_native_call_result_n59)
    {
      hxc_hotbarSelection_he7b00ccedb33 = 6;
    }
    bool hxc_tmp_native_call_result_n60 = IsKeyPressed((int32_t)56);
    if (hxc_tmp_native_call_result_n60)
    {
      hxc_hotbarSelection_he7b00ccedb33 = 7;
    }
    float hxc_tmp_native_call_result_n61 = GetMouseWheelMove();
    double hxc_wheel = (double)hxc_tmp_native_call_result_n61;
    int32_t hxc_hotbarCycle_h8e5a1c5883b8 = 0;
    if (hxc_wheel > 0.0)
    {
      hxc_hotbarCycle_h8e5a1c5883b8 = -1;
    }
    if (hxc_wheel < 0.0)
    {
      hxc_hotbarCycle_h8e5a1c5883b8 = 1;
    }
    bool hxc_tmp_native_call_result_n64 = IsKeyPressed((int32_t)32);
    bool hxc_jumpPressed_h96cdb8ef853f = hxc_tmp_native_call_result_n64;
    bool hxc_tmp_native_call_result_n65 = IsKeyPressed((int32_t)256);
    bool hxc_pausePressed_h7eef21e14758 = hxc_tmp_native_call_result_n65;
    bool hxc_tmp_native_call_result_n66 = IsKeyPressed((int32_t)81);
    bool hxc_quitPressed_hc1dfc82b5169 = hxc_tmp_native_call_result_n66;
    double hxc_frameInput_moveForward = hxc_forward;
    double hxc_frameInput_moveRight = hxc_right;
    double hxc_frameInput_lookYaw = hxc_lookYaw_ha385bdd9fdd3;
    double hxc_frameInput_lookPitch = hxc_lookPitch_h4b45b8f192d5;
    bool hxc_frameInput_jumpPressed = hxc_jumpPressed_h96cdb8ef853f;
    bool hxc_frameInput_primaryPressed = hxc_primaryPressed_h7fcfc8272604;
    bool hxc_frameInput_secondaryPressed = hxc_secondaryPressed_h3cf9facec78b;
    bool hxc_frameInput_interactPressed = hxc_interactPressed_h17ebab6f5feb;
    bool hxc_frameInput_pausePressed = hxc_pausePressed_h7eef21e14758;
    bool hxc_tmp_load_result_n76 = hxc_paused;
    bool hxc_tmp_short_circuit_result_n76 = hxc_tmp_load_result_n76;
    if (hxc_tmp_load_result_n76)
    {
      hxc_tmp_short_circuit_result_n76 = hxc_leftPressed;
    }
    bool hxc_frameInput_capturePressed = hxc_tmp_short_circuit_result_n76;
    bool hxc_frameInput_quitPressed = hxc_quitPressed_hc1dfc82b5169;
    int32_t hxc_frameInput_hotbarSelection = hxc_hotbarSelection_he7b00ccedb33;
    int32_t hxc_frameInput_hotbarCycle = hxc_hotbarCycle_h8e5a1c5883b8;
    double hxc_moveForward = hxc_frameInput_moveForward;
    double hxc_moveRight = hxc_frameInput_moveRight;
    double hxc_lookYaw_h91086d04ff49 = hxc_frameInput_lookYaw;
    double hxc_lookPitch_h6157bf7e15d6 = hxc_frameInput_lookPitch;
    bool hxc_jumpPressed_h3ed52bbc5eda = hxc_frameInput_jumpPressed;
    bool hxc_primaryPressed_hdccf93763d05 = hxc_frameInput_primaryPressed;
    bool hxc_secondaryPressed_hecd1642b0842 = hxc_frameInput_secondaryPressed;
    bool hxc_interactPressed_h8b42b2a6bd90 = hxc_frameInput_interactPressed;
    bool hxc_pausePressed_h64317ce61355 = hxc_frameInput_pausePressed;
    bool hxc_capturePressed = hxc_frameInput_capturePressed;
    bool hxc_quitPressed_hab196548b486 = hxc_frameInput_quitPressed;
    int32_t hxc_hotbarSelection_h91b8f6d45ed6 = hxc_frameInput_hotbarSelection;
    int32_t hxc_hotbarCycle_h089be628b6e0 = hxc_frameInput_hotbarCycle;
    if (hxc_quitPressed_hab196548b486)
    {
      hxc_quit = true;
    }
    if (hxc_hotbarSelection_h91b8f6d45ed6 >= 0)
    {
      struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_load_result_n97 = hxc_inventory;
      struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_call_result_n99 = hxc_caxecraft_gameplay_Inventory_select(hxc_tmp_load_result_n97, hxc_hotbarSelection_h91b8f6d45ed6);
      hxc_inventory = hxc_tmp_call_result_n99;
    }
    if (hxc_hotbarCycle_h089be628b6e0 != 0)
    {
      struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_load_result_n101 = hxc_inventory;
      struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_call_result_n103 = hxc_caxecraft_gameplay_Inventory_cycle(hxc_tmp_load_result_n101, hxc_hotbarCycle_h089be628b6e0);
      hxc_inventory = hxc_tmp_call_result_n103;
    }
    bool hxc_tmp_load_result_n104 = hxc_onTitle;
    bool hxc_tmp_short_circuit_result_n93 = !hxc_tmp_load_result_n104;
    if (!hxc_tmp_load_result_n104)
    {
      hxc_tmp_short_circuit_result_n93 = !hxc_paused;
    }
    bool hxc_tmp_short_circuit_load_result_n106 = hxc_tmp_short_circuit_result_n93;
    bool hxc_tmp_short_circuit_result_n94 = hxc_tmp_short_circuit_load_result_n106;
    if (hxc_tmp_short_circuit_load_result_n106)
    {
      hxc_tmp_short_circuit_result_n94 = hxc_interactPressed_h8b42b2a6bd90;
    }
    if (hxc_tmp_short_circuit_result_n94)
    {
      if (hxc_vitals.hxc_health <= 0)
      {
        struct hxc_caxecraft_gameplay_PlayerVitalsState hxc_tmp_call_result_n110 = hxc_caxecraft_gameplay_PlayerVitals_make(6, 0);
        hxc_vitals = hxc_tmp_call_result_n110;
        struct hxc_caxecraft_domain_PlayerState hxc_tmp_call_result_n111 = hxc_caxecraft_app_Main_spawnPlayer(hxc_cells, hxc_tmp_length_n1);
        hxc_player = hxc_tmp_call_result_n111;
      }
      else
      {
        struct hxc_caxecraft_gameplay_GuideState hxc_tmp_load_result_n112 = hxc_guide;
        double hxc_tmp_record_field_load_result_n113 = hxc_player.hxc_x;
        bool hxc_tmp_call_result_n115 = hxc_caxecraft_gameplay_GuideNpc_isInRange(hxc_tmp_load_result_n112, hxc_tmp_record_field_load_result_n113, hxc_player.hxc_z);
        if (hxc_tmp_call_result_n115)
        {
          bool hxc_tmp_call_result_n117 = hxc_caxecraft_gameplay_GuideNpc_sharesBerriesOnNextInteraction(hxc_guide);
          bool hxc_sharesBerries = hxc_tmp_call_result_n117;
          if (hxc_sharesBerries)
          {
            int32_t hxc_tmp_call_result_n121 = hxc_caxecraft_gameplay_Inventory_acceptedAmount(hxc_inventory, hxc_caxecraft_gameplay_ItemKind_Berries, 2);
            int32_t hxc_acceptedGift = hxc_tmp_call_result_n121;
            if (hxc_acceptedGift == 2)
            {
              struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_load_result_n123 = hxc_inventory;
              struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_call_result_n126 = hxc_caxecraft_gameplay_Inventory_collectItem(hxc_tmp_load_result_n123, hxc_caxecraft_gameplay_ItemKind_Berries, hxc_acceptedGift);
              hxc_inventory = hxc_tmp_call_result_n126;
              struct hxc_caxecraft_gameplay_GuideState hxc_tmp_call_result_n128 = hxc_caxecraft_gameplay_GuideNpc_interact(hxc_guide);
              hxc_guide = hxc_tmp_call_result_n128;
            }
            else
            {
              hxc_inventoryFullReason = 1;
              hxc_inventoryFullFrames = 90;
            }
          }
          else
          {
            struct hxc_caxecraft_gameplay_GuideState hxc_tmp_call_result_n130 = hxc_caxecraft_gameplay_GuideNpc_interact(hxc_guide);
            hxc_guide = hxc_tmp_call_result_n130;
          }
        }
      }
    }
    bool hxc_tmp_load_result_n131 = hxc_onTitle;
    bool hxc_tmp_short_circuit_result_n97 = hxc_tmp_load_result_n131;
    if (hxc_tmp_load_result_n131)
    {
      hxc_tmp_short_circuit_result_n97 = hxc_focused;
    }
    if (hxc_tmp_short_circuit_result_n97)
    {
      bool hxc_tmp_native_call_result_n134 = IsKeyPressed((int32_t)76);
      if (hxc_tmp_native_call_result_n134)
      {
        int32_t hxc_tmp_load_result_n135 = hxc_language;
        int32_t hxc_tmp_conditional_result_n98 = 0;
        if (hxc_tmp_load_result_n135 == 0)
        {
          hxc_tmp_conditional_result_n98 = 1;
        }
        else
        {
          hxc_tmp_conditional_result_n98 = 0;
        }
        hxc_language = hxc_tmp_conditional_result_n98;
      }
      bool hxc_tmp_native_call_result_n137 = IsKeyPressed((int32_t)265);
      bool hxc_tmp_short_circuit_result_n99 = hxc_tmp_native_call_result_n137;
      if (!hxc_tmp_native_call_result_n137)
      {
        bool hxc_tmp_native_call_result_n138 = IsKeyPressed((int32_t)264);
        hxc_tmp_short_circuit_result_n99 = hxc_tmp_native_call_result_n138;
      }
      if (hxc_tmp_short_circuit_result_n99)
      {
        int32_t hxc_tmp_load_result_n140 = hxc_selectedMode;
        int32_t hxc_tmp_conditional_result_n100 = 0;
        if (hxc_tmp_load_result_n140 == 0)
        {
          hxc_tmp_conditional_result_n100 = 1;
        }
        else
        {
          hxc_tmp_conditional_result_n100 = 0;
        }
        hxc_selectedMode = hxc_tmp_conditional_result_n100;
      }
      struct Vector2 hxc_tmp_native_call_result_n142 = GetMousePosition();
      struct Vector2 hxc_menuMouse = hxc_tmp_native_call_result_n142;
      struct Vector2 hxc_tmp_load_result_n143 = hxc_menuMouse;
      struct Vector2 hxc_tmp_load_result_n145 = hxc_menuMouse;
      int32_t hxc_tmp_native_call_result_n147 = GetScreenWidth();
      int32_t hxc_tmp_native_call_result_n148 = GetScreenHeight();
      int32_t hxc_tmp_call_result_n149 = hxc_caxecraft_app_TitleMenu_selectionAt((double)hxc_tmp_load_result_n143.x, (double)hxc_tmp_load_result_n145.y, (int32_t)hxc_tmp_native_call_result_n147, (int32_t)hxc_tmp_native_call_result_n148);
      int32_t hxc_hovered = hxc_tmp_call_result_n149;
      if (hxc_hovered == 0)
      {
        hxc_selectedMode = 0;
      }
      if (hxc_hovered == 1)
      {
        hxc_selectedMode = 1;
      }
      int32_t hxc_tmp_load_result_n152 = hxc_hovered;
      bool hxc_tmp_short_circuit_result_n104 = hxc_tmp_load_result_n152 >= 0;
      if (hxc_tmp_load_result_n152 >= 0)
      {
        bool hxc_tmp_native_call_result_n153 = IsMouseButtonPressed((int32_t)0);
        hxc_tmp_short_circuit_result_n104 = hxc_tmp_native_call_result_n153;
      }
      bool hxc_clickedChoice = hxc_tmp_short_circuit_result_n104;
      bool hxc_tmp_load_result_n155 = hxc_clickedChoice;
      bool hxc_tmp_short_circuit_result_n105 = hxc_tmp_load_result_n155;
      if (!hxc_tmp_load_result_n155)
      {
        bool hxc_tmp_native_call_result_n156 = IsKeyPressed((int32_t)257);
        hxc_tmp_short_circuit_result_n105 = hxc_tmp_native_call_result_n156;
      }
      if (hxc_tmp_short_circuit_result_n105)
      {
        hxc_onTitle = false;
        hxc_paused = false;
        hxc_captured = true;
        hxc_recapturedThisFrame = true;
        DisableCursor();
      }
    }
    bool hxc_tmp_load_result_n158 = hxc_onTitle;
    bool hxc_tmp_short_circuit_result_n106 = !hxc_tmp_load_result_n158;
    if (!hxc_tmp_load_result_n158)
    {
      hxc_tmp_short_circuit_result_n106 = !hxc_focused;
    }
    bool hxc_tmp_short_circuit_load_result_n160 = hxc_tmp_short_circuit_result_n106;
    bool hxc_tmp_short_circuit_result_n107 = hxc_tmp_short_circuit_load_result_n160;
    if (hxc_tmp_short_circuit_load_result_n160)
    {
      hxc_tmp_short_circuit_result_n107 = hxc_captured;
    }
    if (hxc_tmp_short_circuit_result_n107)
    {
      hxc_paused = true;
      hxc_captured = false;
      hxc_jumpQueued = false;
      EnableCursor();
    }
    bool hxc_tmp_load_result_n163 = hxc_onTitle;
    bool hxc_tmp_short_circuit_result_n108 = !hxc_tmp_load_result_n163;
    if (!hxc_tmp_load_result_n163)
    {
      hxc_tmp_short_circuit_result_n108 = hxc_focused;
    }
    bool hxc_tmp_short_circuit_load_result_n165 = hxc_tmp_short_circuit_result_n108;
    bool hxc_tmp_short_circuit_result_n109 = hxc_tmp_short_circuit_load_result_n165;
    if (hxc_tmp_short_circuit_load_result_n165)
    {
      hxc_tmp_short_circuit_result_n109 = hxc_pausePressed_h64317ce61355;
    }
    if (hxc_tmp_short_circuit_result_n109)
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
    bool hxc_tmp_load_result_n172 = hxc_onTitle;
    bool hxc_tmp_short_circuit_result_n110 = !hxc_tmp_load_result_n172;
    if (!hxc_tmp_load_result_n172)
    {
      hxc_tmp_short_circuit_result_n110 = hxc_focused;
    }
    bool hxc_tmp_short_circuit_load_result_n174 = hxc_tmp_short_circuit_result_n110;
    bool hxc_tmp_short_circuit_result_n111 = hxc_tmp_short_circuit_load_result_n174;
    if (hxc_tmp_short_circuit_load_result_n174)
    {
      hxc_tmp_short_circuit_result_n111 = hxc_paused;
    }
    bool hxc_tmp_short_circuit_load_result_n176 = hxc_tmp_short_circuit_result_n111;
    bool hxc_tmp_short_circuit_result_n112 = hxc_tmp_short_circuit_load_result_n176;
    if (hxc_tmp_short_circuit_load_result_n176)
    {
      hxc_tmp_short_circuit_result_n112 = hxc_capturePressed;
    }
    if (hxc_tmp_short_circuit_result_n112)
    {
      hxc_paused = false;
      hxc_captured = true;
      hxc_recapturedThisFrame = true;
      DisableCursor();
    }
    if (hxc_captured)
    {
      double hxc_yawDelta = hxc_lookYaw_h91086d04ff49;
      if (hxc_yawDelta > 0.25)
      {
        hxc_yawDelta = 0.25;
      }
      if (hxc_yawDelta < -0.25)
      {
        hxc_yawDelta = -0.25;
      }
      double hxc_tmp_load_result_n183 = hxc_lookX;
      double hxc_tmp_load_result_n184 = hxc_yawDelta;
      double hxc_candidateX = hxc_tmp_load_result_n183 + hxc_tmp_load_result_n184 * hxc_lookZ;
      double hxc_tmp_load_result_n186 = hxc_lookZ;
      double hxc_tmp_load_result_n187 = hxc_yawDelta;
      double hxc_candidateZ = hxc_tmp_load_result_n186 - hxc_tmp_load_result_n187 * hxc_lookX;
      double hxc_tmp_load_result_n189 = hxc_candidateX;
      double hxc_tmp_load_result_n190 = hxc_candidateX;
      double hxc_tmp_load_result_n191 = hxc_candidateZ;
      double hxc_lengthSquared = hxc_tmp_load_result_n189 * hxc_tmp_load_result_n190 + hxc_tmp_load_result_n191 * hxc_candidateZ;
      double hxc_normalize = 1.5 - 0.5 * hxc_lengthSquared;
      double hxc_tmp_load_result_n194 = hxc_candidateX;
      hxc_lookX = hxc_tmp_load_result_n194 * hxc_normalize;
      double hxc_tmp_load_result_n196 = hxc_candidateZ;
      hxc_lookZ = hxc_tmp_load_result_n196 * hxc_normalize;
      double hxc_tmp_compound_load_result_n198 = hxc_lookY;
      hxc_lookY = hxc_tmp_compound_load_result_n198 + hxc_lookPitch_h6157bf7e15d6;
      if (hxc_lookY > 0.90)
      {
        hxc_lookY = 0.90;
      }
      if (hxc_lookY < -0.90)
      {
        hxc_lookY = -0.90;
      }
      if (hxc_jumpPressed_h3ed52bbc5eda)
      {
        hxc_jumpQueued = true;
      }
    }
    float hxc_tmp_native_call_result_n203 = GetFrameTime();
    double hxc_frameSeconds = (double)hxc_tmp_native_call_result_n203;
    if (hxc_frameSeconds > 0.25)
    {
      hxc_frameSeconds = 0.25;
    }
    bool hxc_tmp_load_result_n205 = hxc_captured;
    bool hxc_tmp_short_circuit_result_n119 = hxc_tmp_load_result_n205;
    if (hxc_tmp_load_result_n205)
    {
      hxc_tmp_short_circuit_result_n119 = !hxc_recapturedThisFrame;
    }
    bool hxc_tmp_short_circuit_load_result_n207 = hxc_tmp_short_circuit_result_n119;
    bool hxc_tmp_short_circuit_result_n120 = hxc_tmp_short_circuit_load_result_n207;
    if (hxc_tmp_short_circuit_load_result_n207)
    {
      hxc_tmp_short_circuit_result_n120 = hxc_primaryPressed_hdccf93763d05;
    }
    bool hxc_tmp_short_circuit_load_result_n209 = hxc_tmp_short_circuit_result_n120;
    bool hxc_tmp_short_circuit_result_n121 = hxc_tmp_short_circuit_load_result_n209;
    if (hxc_tmp_short_circuit_load_result_n209)
    {
      hxc_tmp_short_circuit_result_n121 = hxc_selectedMode == 1;
    }
    bool hxc_tmp_short_circuit_load_result_n211 = hxc_tmp_short_circuit_result_n121;
    bool hxc_tmp_short_circuit_result_n122 = hxc_tmp_short_circuit_load_result_n211;
    if (hxc_tmp_short_circuit_load_result_n211)
    {
      bool hxc_tmp_call_result_n214 = hxc_caxecraft_gameplay_Inventory_selectedIs(hxc_inventory, hxc_caxecraft_gameplay_ItemKind_CopperSword);
      hxc_tmp_short_circuit_result_n122 = hxc_tmp_call_result_n214;
    }
    if (hxc_tmp_short_circuit_result_n122)
    {
      hxc_swordQueued = true;
    }
    if (!hxc_paused)
    {
      double hxc_tmp_compound_load_result_n217 = hxc_accumulator;
      hxc_accumulator = hxc_tmp_compound_load_result_n217 + hxc_frameSeconds;
    }
    while (1)
    {
      bool hxc_tmp_load_result_n219 = hxc_paused;
      bool hxc_tmp_short_circuit_result_n123 = !hxc_tmp_load_result_n219;
      if (!hxc_tmp_load_result_n219)
      {
        hxc_tmp_short_circuit_result_n123 = hxc_accumulator >= 0.05;
      }
      if (!hxc_tmp_short_circuit_result_n123)
      {
        break;
      }
      double hxc_tmp_load_result_n222 = hxc_moveForward;
      double hxc_tmp_load_result_n223 = hxc_lookX;
      double hxc_tmp_load_result_n224 = hxc_moveRight;
      double hxc_moveX = hxc_tmp_load_result_n222 * hxc_tmp_load_result_n223 - hxc_tmp_load_result_n224 * hxc_lookZ;
      double hxc_tmp_load_result_n226 = hxc_moveForward;
      double hxc_tmp_load_result_n227 = hxc_lookZ;
      double hxc_tmp_load_result_n228 = hxc_moveRight;
      double hxc_moveZ = hxc_tmp_load_result_n226 * hxc_tmp_load_result_n227 + hxc_tmp_load_result_n228 * hxc_lookX;
      double hxc_tmp_load_result_n230 = hxc_moveForward;
      bool hxc_tmp_short_circuit_result_n126 = hxc_tmp_load_result_n230 != 0.0;
      if (hxc_tmp_load_result_n230 != 0.0)
      {
        hxc_tmp_short_circuit_result_n126 = hxc_moveRight != 0.0;
      }
      if (hxc_tmp_short_circuit_result_n126)
      {
        hxc_moveX = hxc_moveX * 0.7071067811865476;
        hxc_moveZ = hxc_moveZ * 0.7071067811865476;
      }
      if (hxc_vitals.hxc_health > 0)
      {
        struct hxc_caxecraft_domain_PlayerState hxc_tmp_load_result_n236 = hxc_player;
        double hxc_tmp_load_result_n237 = hxc_moveX;
        double hxc_tmp_load_result_n238 = hxc_moveZ;
        struct hxc_caxecraft_domain_StepInput hxc_tmp_call_result_n240 = hxc_caxecraft_domain_PlayerPhysics_input(hxc_tmp_load_result_n237, hxc_tmp_load_result_n238, hxc_jumpQueued);
        struct hxc_caxecraft_domain_PlayerState hxc_tmp_call_result_n241 = hxc_caxecraft_domain_PlayerPhysics_step(hxc_cells, hxc_tmp_length_n1, hxc_tmp_load_result_n236, hxc_tmp_call_result_n240);
        hxc_player = hxc_tmp_call_result_n241;
      }
      if (hxc_selectedMode == 1)
      {
        if (hxc_vitals.hxc_health > 0)
        {
          struct hxc_caxecraft_gameplay_PlayerVitalsState hxc_tmp_call_result_n245 = hxc_caxecraft_gameplay_PlayerVitals_step(hxc_vitals);
          hxc_vitals = hxc_tmp_call_result_n245;
          struct hxc_caxecraft_gameplay_MosslingState hxc_tmp_load_result_n246 = hxc_mossling;
          double hxc_tmp_record_field_load_result_n247 = hxc_player.hxc_x;
          bool hxc_tmp_call_result_n249 = hxc_caxecraft_gameplay_Mossling_attacksThisTick(hxc_tmp_load_result_n246, hxc_tmp_record_field_load_result_n247, hxc_player.hxc_z);
          bool hxc_mosslingAttacked = hxc_tmp_call_result_n249;
          struct hxc_caxecraft_gameplay_PlayerVitalsState hxc_tmp_load_result_n250 = hxc_vitals;
          struct hxc_caxecraft_gameplay_PlayerVitalsState hxc_tmp_call_result_n252 = hxc_caxecraft_gameplay_PlayerVitals_applyAttack(hxc_tmp_load_result_n250, hxc_mosslingAttacked);
          hxc_vitals = hxc_tmp_call_result_n252;
          if (hxc_mosslingAttacked)
          {
            hxc_enemyAttackFrames = 120;
          }
          struct hxc_caxecraft_gameplay_MosslingState hxc_tmp_load_result_n254 = hxc_mossling;
          double hxc_tmp_record_field_load_result_n255 = hxc_player.hxc_x;
          double hxc_tmp_record_field_load_result_n256 = hxc_player.hxc_z;
          struct hxc_caxecraft_gameplay_MosslingState hxc_tmp_call_result_n258 = hxc_caxecraft_gameplay_Mossling_step(hxc_cells, hxc_tmp_length_n1, hxc_tmp_load_result_n254, hxc_tmp_record_field_load_result_n255, hxc_tmp_record_field_load_result_n256, hxc_updateCount);
          hxc_mossling = hxc_tmp_call_result_n258;
        }
        struct hxc_caxecraft_gameplay_SwordCombatState hxc_tmp_call_result_n260 = hxc_caxecraft_gameplay_SwordCombat_step(hxc_swordCombat);
        hxc_swordCombat = hxc_tmp_call_result_n260;
        if (hxc_swordQueued)
        {
          struct hxc_caxecraft_gameplay_SwordCombatState hxc_tmp_load_result_n262 = hxc_swordCombat;
          struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_load_result_n263 = hxc_inventory;
          struct hxc_caxecraft_gameplay_PlayerVitalsState hxc_tmp_load_result_n264 = hxc_vitals;
          struct hxc_caxecraft_gameplay_MosslingState hxc_tmp_load_result_n265 = hxc_mossling;
          double hxc_tmp_record_field_load_result_n266 = hxc_player.hxc_x;
          double hxc_tmp_record_field_load_result_n267 = hxc_player.hxc_z;
          double hxc_tmp_load_result_n268 = hxc_lookX;
          int32_t hxc_tmp_call_result_n270 = hxc_caxecraft_gameplay_SwordCombat_decide(hxc_tmp_load_result_n262, hxc_tmp_load_result_n263, hxc_tmp_load_result_n264, hxc_tmp_load_result_n265, hxc_tmp_record_field_load_result_n266, hxc_tmp_record_field_load_result_n267, hxc_tmp_load_result_n268, hxc_lookZ);
          int32_t hxc_swordDecision = hxc_tmp_call_result_n270;
          if (hxc_swordDecision == 5)
          {
            struct hxc_caxecraft_gameplay_MosslingState hxc_tmp_call_result_n273 = hxc_caxecraft_gameplay_Mossling_strike(hxc_mossling);
            hxc_mossling = hxc_tmp_call_result_n273;
            hxc_strikeHitFrames = 16;
            if (hxc_mossling.hxc_health <= 0)
            {
              struct hxc_caxecraft_gameplay_BerryDropState hxc_tmp_call_result_n276 = hxc_caxecraft_gameplay_BerryDrop_fromDefeatedMossling(hxc_mossling);
              hxc_berryDrop = hxc_tmp_call_result_n276;
              hxc_enemyDefeatedFrames = 120;
            }
          }
          int32_t hxc_tmp_load_result_n277 = hxc_swordDecision;
          struct hxc_caxecraft_gameplay_SwordCombatState hxc_tmp_call_result_n279 = hxc_caxecraft_gameplay_SwordCombat_after(hxc_tmp_load_result_n277, hxc_swordCombat);
          hxc_swordCombat = hxc_tmp_call_result_n279;
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
    double hxc_tmp_load_result_n285 = hxc_eyeX;
    double hxc_tmp_load_result_n286 = hxc_eyeY;
    double hxc_tmp_load_result_n287 = hxc_eyeZ;
    double hxc_tmp_load_result_n288 = hxc_lookX;
    double hxc_tmp_load_result_n289 = hxc_lookY;
    struct hxc_caxecraft_domain_RaycastHit hxc_tmp_call_result_n291 = hxc_caxecraft_domain_VoxelRaycast_trace(hxc_cells, hxc_tmp_length_n1, hxc_tmp_load_result_n285, hxc_tmp_load_result_n286, hxc_tmp_load_result_n287, hxc_tmp_load_result_n288, hxc_tmp_load_result_n289, hxc_lookZ, 7.0);
    struct hxc_caxecraft_domain_RaycastHit hxc_hit = hxc_tmp_call_result_n291;
    bool hxc_tmp_load_result_n292 = hxc_captured;
    bool hxc_tmp_short_circuit_result_n133 = hxc_tmp_load_result_n292;
    if (hxc_tmp_load_result_n292)
    {
      hxc_tmp_short_circuit_result_n133 = !hxc_recapturedThisFrame;
    }
    bool hxc_tmp_short_circuit_load_result_n294 = hxc_tmp_short_circuit_result_n133;
    bool hxc_tmp_short_circuit_result_n134 = hxc_tmp_short_circuit_load_result_n294;
    if (hxc_tmp_short_circuit_load_result_n294)
    {
      hxc_tmp_short_circuit_result_n134 = hxc_primaryPressed_hdccf93763d05;
    }
    if (hxc_tmp_short_circuit_result_n134)
    {
      if (hxc_vitals.hxc_health > 0)
      {
        if (hxc_selectedMode == 1)
        {
          bool hxc_tmp_call_result_n301 = hxc_caxecraft_gameplay_Inventory_selectedIs(hxc_inventory, hxc_caxecraft_gameplay_ItemKind_CopperSword);
          bool hxc_tmp_short_circuit_result_n135 = !hxc_tmp_call_result_n301;
          if (!hxc_tmp_call_result_n301)
          {
            hxc_tmp_short_circuit_result_n135 = hxc_hit.hxc_hit;
          }
          if (hxc_tmp_short_circuit_result_n135)
          {
            int32_t hxc_tmp_record_field_load_result_n304 = hxc_hit.hxc_cellX;
            int32_t hxc_tmp_record_field_load_result_n305 = hxc_hit.hxc_cellY;
            struct hxc_caxecraft_domain_BlockCoord hxc_tmp_call_result_n307 = hxc_caxecraft_domain_World_coord(hxc_tmp_record_field_load_result_n304, hxc_tmp_record_field_load_result_n305, hxc_hit.hxc_cellZ);
            struct hxc_caxecraft_gameplay_MiningResult hxc_tmp_call_result_n309 = hxc_caxecraft_gameplay_Mining_attempt(hxc_cells, hxc_tmp_length_n1, hxc_tmp_call_result_n307, hxc_inventory);
            struct hxc_caxecraft_gameplay_MiningResult hxc_mining = hxc_tmp_call_result_n309;
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
            int32_t hxc_tmp_record_field_load_result_n313 = hxc_hit.hxc_cellX;
            int32_t hxc_tmp_record_field_load_result_n314 = hxc_hit.hxc_cellY;
            struct hxc_caxecraft_domain_BlockCoord hxc_tmp_call_result_n316 = hxc_caxecraft_domain_World_coord(hxc_tmp_record_field_load_result_n313, hxc_tmp_record_field_load_result_n314, hxc_hit.hxc_cellZ);
            hxc_caxecraft_domain_World_remove(hxc_cells, hxc_tmp_length_n1, hxc_tmp_call_result_n316);
          }
        }
      }
    }
    bool hxc_tmp_load_result_n317 = hxc_captured;
    bool hxc_tmp_short_circuit_result_n137 = hxc_tmp_load_result_n317;
    if (hxc_tmp_load_result_n317)
    {
      hxc_tmp_short_circuit_result_n137 = hxc_secondaryPressed_hecd1642b0842;
    }
    if (hxc_tmp_short_circuit_result_n137)
    {
      if (hxc_vitals.hxc_health > 0)
      {
        struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_load_result_n321 = hxc_inventory;
        int32_t hxc_tmp_call_result_n323 = hxc_caxecraft_gameplay_Recovery_decide(hxc_tmp_load_result_n321, hxc_vitals);
        int32_t hxc_recoveryDecision = hxc_tmp_call_result_n323;
        if (hxc_recoveryDecision != 0)
        {
          hxc_recoveryFeedback = hxc_recoveryDecision;
          hxc_recoveryFeedbackFrames = 90;
          int32_t hxc_tmp_load_result_n326 = hxc_recoveryDecision;
          struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_call_result_n328 = hxc_caxecraft_gameplay_Recovery_applyInventory(hxc_tmp_load_result_n326, hxc_inventory);
          hxc_inventory = hxc_tmp_call_result_n328;
          int32_t hxc_tmp_load_result_n329 = hxc_recoveryDecision;
          struct hxc_caxecraft_gameplay_PlayerVitalsState hxc_tmp_call_result_n331 = hxc_caxecraft_gameplay_Recovery_applyVitals(hxc_tmp_load_result_n329, hxc_vitals);
          hxc_vitals = hxc_tmp_call_result_n331;
        }
        else
        {
          if (hxc_hit.hxc_hit)
          {
            int32_t hxc_tmp_record_field_load_result_n333 = hxc_hit.hxc_previousX;
            int32_t hxc_tmp_record_field_load_result_n334 = hxc_hit.hxc_previousY;
            struct hxc_caxecraft_domain_BlockCoord hxc_tmp_call_result_n336 = hxc_caxecraft_domain_World_coord(hxc_tmp_record_field_load_result_n333, hxc_tmp_record_field_load_result_n334, hxc_hit.hxc_previousZ);
            struct hxc_caxecraft_domain_BlockCoord hxc_placement = hxc_tmp_call_result_n336;
            enum hxc_caxecraft_domain_BlockKind hxc_tmp_call_result_n338 = hxc_caxecraft_gameplay_Inventory_selectedBlock(hxc_inventory);
            enum hxc_caxecraft_domain_BlockKind hxc_selectedBlock = hxc_tmp_call_result_n338;
            struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_load_result_n339 = hxc_inventory;
            int32_t hxc_tmp_call_result_n341 = hxc_caxecraft_gameplay_Inventory_countAt(hxc_tmp_load_result_n339, hxc_inventory.hxc_selected);
            bool hxc_hasItem = hxc_tmp_call_result_n341 > 0;
            bool hxc_tmp_load_result_n342 = hxc_hasItem;
            bool hxc_tmp_short_circuit_result_n142 = !hxc_tmp_load_result_n342;
            if (!!hxc_tmp_load_result_n342)
            {
              bool hxc_tmp_call_result_n344 = hxc_caxecraft_domain_World_isPlaceable(hxc_selectedBlock);
              hxc_tmp_short_circuit_result_n142 = !hxc_tmp_call_result_n344;
            }
            bool hxc_tmp_short_circuit_load_result_n345 = hxc_tmp_short_circuit_result_n142;
            bool hxc_tmp_short_circuit_result_n143 = hxc_tmp_short_circuit_load_result_n345;
            if (!hxc_tmp_short_circuit_load_result_n345)
            {
              struct hxc_caxecraft_domain_PlayerState hxc_tmp_load_result_n346 = hxc_player;
              bool hxc_tmp_call_result_n348 = hxc_caxecraft_domain_PlayerPhysics_canPlaceAt(hxc_tmp_load_result_n346, hxc_placement);
              hxc_tmp_short_circuit_result_n143 = !hxc_tmp_call_result_n348;
            }
            bool hxc_tmp_short_circuit_load_result_n349 = hxc_tmp_short_circuit_result_n143;
            bool hxc_tmp_short_circuit_result_n144 = hxc_tmp_short_circuit_load_result_n349;
            if (!hxc_tmp_short_circuit_load_result_n349)
            {
              struct hxc_caxecraft_domain_BlockCoord hxc_tmp_load_result_n350 = hxc_placement;
              bool hxc_tmp_call_result_n352 = hxc_caxecraft_domain_World_place(hxc_cells, hxc_tmp_length_n1, hxc_tmp_load_result_n350, hxc_selectedBlock);
              hxc_tmp_short_circuit_result_n144 = !hxc_tmp_call_result_n352;
            }
            if (hxc_tmp_short_circuit_result_n144)
            {
              hxc_placementBlockedFrames = 60;
            }
            else
            {
              if (hxc_selectedMode == 1)
              {
                struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_call_result_n356 = hxc_caxecraft_gameplay_Inventory_consumeSelected(hxc_inventory);
                hxc_inventory = hxc_tmp_call_result_n356;
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
      struct hxc_caxecraft_gameplay_BerryDropState hxc_tmp_load_result_n360 = hxc_berryDrop;
      double hxc_tmp_record_field_load_result_n361 = hxc_player.hxc_x;
      double hxc_tmp_record_field_load_result_n362 = hxc_player.hxc_y;
      bool hxc_tmp_call_result_n364 = hxc_caxecraft_gameplay_BerryDrop_isInRange(hxc_tmp_load_result_n360, hxc_tmp_record_field_load_result_n361, hxc_tmp_record_field_load_result_n362, hxc_player.hxc_z);
      if (hxc_tmp_call_result_n364)
      {
        struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_load_result_n365 = hxc_inventory;
        int32_t hxc_tmp_call_result_n368 = hxc_caxecraft_gameplay_Inventory_acceptedAmount(hxc_tmp_load_result_n365, hxc_caxecraft_gameplay_ItemKind_Berries, hxc_berryDrop.hxc_amount);
        int32_t hxc_acceptedDrop = hxc_tmp_call_result_n368;
        if (hxc_acceptedDrop > 0)
        {
          struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_load_result_n370 = hxc_inventory;
          struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_call_result_n373 = hxc_caxecraft_gameplay_Inventory_collectItem(hxc_tmp_load_result_n370, hxc_caxecraft_gameplay_ItemKind_Berries, hxc_acceptedDrop);
          hxc_inventory = hxc_tmp_call_result_n373;
          struct hxc_caxecraft_gameplay_BerryDropState hxc_tmp_load_result_n374 = hxc_berryDrop;
          struct hxc_caxecraft_gameplay_BerryDropState hxc_tmp_call_result_n376 = hxc_caxecraft_gameplay_BerryDrop_collectAmount(hxc_tmp_load_result_n374, hxc_acceptedDrop);
          hxc_berryDrop = hxc_tmp_call_result_n376;
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
    double hxc_tmp_load_result_n391 = hxc_eyeX;
    double hxc_tmp_load_result_n392 = hxc_eyeY;
    double hxc_tmp_load_result_n393 = hxc_eyeZ;
    double hxc_tmp_load_result_n395 = hxc_eyeX;
    double hxc_tmp_load_result_n396 = hxc_lookX;
    double hxc_tmp_load_result_n397 = hxc_eyeY;
    double hxc_tmp_load_result_n398 = hxc_lookY;
    double hxc_tmp_load_result_n399 = hxc_eyeZ;
    struct Camera3D hxc_camera = (struct Camera3D){ .position = (struct Vector3){ .x = (float)hxc_tmp_load_result_n391, .y = (float)hxc_tmp_load_result_n392, .z = (float)hxc_tmp_load_result_n393 }, .target = (struct Vector3){ .x = (float)(hxc_tmp_load_result_n395 + hxc_tmp_load_result_n396), .y = (float)(hxc_tmp_load_result_n397 + hxc_tmp_load_result_n398), .z = (float)(hxc_tmp_load_result_n399 + hxc_lookZ) }, .up = (struct Vector3){ .x = (float)0.0, .y = (float)1.0, .z = (float)0.0 }, .fovy = (float)70.0, .projection = 0 };
    BeginDrawing();
    if (hxc_onTitle)
    {
      int32_t hxc_tmp_native_call_result_n405 = GetScreenWidth();
      int32_t hxc_width_hbba54f7f7065 = (int32_t)hxc_tmp_native_call_result_n405;
      int32_t hxc_tmp_native_call_result_n406 = GetScreenHeight();
      int32_t hxc_height_hae0a52249093 = (int32_t)hxc_tmp_native_call_result_n406;
      if (hxc_titleTextureReady)
      {
        struct Color hxc_this1_h76ebd527ca60 = (struct Color){ .r = 255, .g = 255, .b = 255, .a = 255 };
        struct Texture hxc_tmp_load_result_n409 = hxc_titleTexture;
        double hxc_imageAspect = hxc_f64_divide_zero_safe((double)(int32_t)hxc_tmp_load_result_n409.width, (double)(int32_t)hxc_titleTexture.height);
        int32_t hxc_tmp_load_result_n413 = hxc_width_hbba54f7f7065;
        double hxc_screenAspect = hxc_f64_divide_zero_safe((double)hxc_tmp_load_result_n413, (double)hxc_height_hae0a52249093);
        double hxc_sourceX = 0.0;
        double hxc_sourceY = 0.0;
        double hxc_sourceWidth = (double)(int32_t)hxc_titleTexture.width;
        double hxc_sourceHeight = (double)(int32_t)hxc_titleTexture.height;
        double hxc_tmp_load_result_n419 = hxc_screenAspect;
        if (hxc_tmp_load_result_n419 > hxc_imageAspect)
        {
          struct Texture hxc_tmp_load_result_n421 = hxc_titleTexture;
          hxc_sourceHeight = hxc_f64_divide_zero_safe((double)(int32_t)hxc_tmp_load_result_n421.width, hxc_screenAspect);
          struct Texture hxc_tmp_load_result_n424 = hxc_titleTexture;
          hxc_sourceY = ((double)(int32_t)hxc_tmp_load_result_n424.height - hxc_sourceHeight) * 0.5;
        }
        else
        {
          struct Texture hxc_tmp_load_result_n427 = hxc_titleTexture;
          hxc_sourceWidth = (double)(int32_t)hxc_tmp_load_result_n427.height * hxc_screenAspect;
          struct Texture hxc_tmp_load_result_n430 = hxc_titleTexture;
          hxc_sourceX = ((double)(int32_t)hxc_tmp_load_result_n430.width - hxc_sourceWidth) * 0.5;
        }
        struct Texture hxc_tmp_load_result_n433 = hxc_titleTexture;
        double hxc_tmp_load_result_n434 = hxc_sourceX;
        double hxc_tmp_load_result_n435 = hxc_sourceY;
        double hxc_tmp_load_result_n436 = hxc_sourceWidth;
        double hxc_tmp_load_result_n437 = hxc_sourceHeight;
        int32_t hxc_tmp_load_result_n439 = hxc_width_hbba54f7f7065;
        int32_t hxc_tmp_load_result_n440 = hxc_height_hae0a52249093;
        DrawTexturePro(hxc_tmp_load_result_n433, (struct Rectangle){ .x = (float)hxc_tmp_load_result_n434, .y = (float)hxc_tmp_load_result_n435, .width = (float)hxc_tmp_load_result_n436, .height = (float)hxc_tmp_load_result_n437 }, (struct Rectangle){ .x = (float)0.0, .y = (float)0.0, .width = (float)((double)hxc_tmp_load_result_n439 + 0.0), .height = (float)((double)hxc_tmp_load_result_n440 + 0.0) }, (struct Vector2){ .x = (float)0.0, .y = (float)0.0 }, (float)0.0, hxc_this1_h76ebd527ca60);
      }
      else
      {
        ClearBackground((struct Color){ .r = 126, .g = 190, .b = 201, .a = 255 });
      }
      struct Color hxc_this1_hd91a64420a0e = (struct Color){ .r = 4, .g = 14, .b = 23, .a = 132 };
      int32_t hxc_tmp_load_result_n446 = hxc_width_hbba54f7f7065;
      int32_t hxc_tmp_load_result_n447 = hxc_height_hae0a52249093;
      DrawRectangle((int32_t)0, (int32_t)0, (int32_t)hxc_tmp_load_result_n446, (int32_t)hxc_tmp_load_result_n447, hxc_this1_hd91a64420a0e);
      if (hxc_wordmarkTextureReady)
      {
        int32_t hxc_centerX = hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_width_hbba54f7f7065, (double)2));
        struct Color hxc_this1_h742952063b58 = (struct Color){ .r = 255, .g = 255, .b = 255, .a = 255 };
        double hxc_scale = hxc_f64_divide_zero_safe((double)560, (double)(int32_t)hxc_wordmarkTexture.width);
        double hxc_heightScale = hxc_f64_divide_zero_safe((double)132, (double)(int32_t)hxc_wordmarkTexture.height);
        double hxc_tmp_load_result_n456 = hxc_heightScale;
        if (hxc_tmp_load_result_n456 < hxc_scale)
        {
          hxc_scale = hxc_heightScale;
        }
        struct Texture hxc_tmp_load_result_n459 = hxc_wordmarkTexture;
        double hxc_width_h1500e6232425 = (double)(int32_t)hxc_tmp_load_result_n459.width * hxc_scale;
        struct Texture hxc_tmp_load_result_n462 = hxc_wordmarkTexture;
        double hxc_height_h589c3d8feaf6 = (double)(int32_t)hxc_tmp_load_result_n462.height * hxc_scale;
        double hxc_width1 = (double)(int32_t)hxc_wordmarkTexture.width + 0.0;
        double hxc_height1 = (double)(int32_t)hxc_wordmarkTexture.height + 0.0;
        struct Texture hxc_tmp_load_result_n469 = hxc_wordmarkTexture;
        double hxc_tmp_load_result_n470 = hxc_width1;
        double hxc_tmp_load_result_n471 = hxc_height1;
        int32_t hxc_tmp_load_result_n473 = hxc_centerX;
        double hxc_tmp_load_result_n474 = hxc_width_h1500e6232425;
        double hxc_tmp_load_result_n475 = hxc_width_h1500e6232425;
        double hxc_tmp_load_result_n476 = hxc_height_h589c3d8feaf6;
        DrawTexturePro(hxc_tmp_load_result_n469, (struct Rectangle){ .x = (float)0.0, .y = (float)0.0, .width = (float)hxc_tmp_load_result_n470, .height = (float)hxc_tmp_load_result_n471 }, (struct Rectangle){ .x = (float)((double)hxc_tmp_load_result_n473 - hxc_tmp_load_result_n474 * 0.5), .y = (float)28.0, .width = (float)hxc_tmp_load_result_n475, .height = (float)hxc_tmp_load_result_n476 }, (struct Vector2){ .x = (float)0.0, .y = (float)0.0 }, (float)0.0, hxc_this1_h742952063b58);
      }
      else
      {
        int32_t hxc_x_h56c7f8b92541 = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_width_hbba54f7f7065, (double)2)), 92);
        struct Color hxc_this1_hc6235c1aaa0e = (struct Color){ .r = 242, .g = 249, .b = 245, .a = 255 };
        int32_t hxc_tmp_load_result_n482 = hxc_x_h56c7f8b92541;
        DrawText("CAXECRAFT", (int32_t)hxc_tmp_load_result_n482, (int32_t)52, (int32_t)34, hxc_this1_hc6235c1aaa0e);
      }
      int32_t hxc_firstTop = hxc_i32_subtract_wrapping(hxc_height_hae0a52249093, 225);
      if (hxc_language == 1)
      {
        int32_t hxc_left_h0beaf76c31b0 = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_width_hbba54f7f7065, (double)2)), 180);
        if (hxc_selectedMode == 0)
        {
          struct Color hxc_this1_h45e7cd6f2d61 = (struct Color){ .r = 16, .g = 88, .b = 102, .a = 232 };
          int32_t hxc_tmp_load_result_n489 = hxc_left_h0beaf76c31b0;
          int32_t hxc_tmp_load_result_n490 = hxc_firstTop;
          DrawRectangle((int32_t)hxc_tmp_load_result_n489, (int32_t)hxc_tmp_load_result_n490, (int32_t)360, (int32_t)54, hxc_this1_h45e7cd6f2d61);
          struct Color hxc_this1_h5ab6e6cdd588 = (struct Color){ .r = 255, .g = 190, .b = 55, .a = 255 };
          int32_t hxc_tmp_load_result_n493 = hxc_left_h0beaf76c31b0;
          int32_t hxc_tmp_load_result_n494 = hxc_firstTop;
          DrawRectangleLines((int32_t)hxc_tmp_load_result_n493, (int32_t)hxc_tmp_load_result_n494, (int32_t)360, (int32_t)54, hxc_this1_h5ab6e6cdd588);
        }
        else
        {
          struct Color hxc_this1_h922c566f2fec = (struct Color){ .r = 6, .g = 26, .b = 36, .a = 218 };
          int32_t hxc_tmp_load_result_n497 = hxc_left_h0beaf76c31b0;
          int32_t hxc_tmp_load_result_n498 = hxc_firstTop;
          DrawRectangle((int32_t)hxc_tmp_load_result_n497, (int32_t)hxc_tmp_load_result_n498, (int32_t)360, (int32_t)54, hxc_this1_h922c566f2fec);
          struct Color hxc_this1_h40172815f085 = (struct Color){ .r = 92, .g = 194, .b = 188, .a = 255 };
          int32_t hxc_tmp_load_result_n501 = hxc_left_h0beaf76c31b0;
          int32_t hxc_tmp_load_result_n502 = hxc_firstTop;
          DrawRectangleLines((int32_t)hxc_tmp_load_result_n501, (int32_t)hxc_tmp_load_result_n502, (int32_t)360, (int32_t)54, hxc_this1_h40172815f085);
        }
        struct Color hxc_this1_h82029f00bf2f = (struct Color){ .r = 242, .g = 249, .b = 245, .a = 255 };
        int32_t hxc_tmp_load_result_n505 = hxc_left_h0beaf76c31b0;
        int32_t hxc_tmp_load_result_n506 = hxc_firstTop;
        DrawText("MODO CREATIVO", (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n505, 28), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n506, 16), (int32_t)21, hxc_this1_h82029f00bf2f);
        int32_t hxc_top_h9bed6869cb5a = hxc_i32_add_wrapping(hxc_i32_add_wrapping(hxc_firstTop, 54), 14);
        int32_t hxc_left_h46be8694faff = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_width_hbba54f7f7065, (double)2)), 180);
        if (hxc_selectedMode == 1)
        {
          struct Color hxc_this1_h00d59f9d7c58 = (struct Color){ .r = 16, .g = 88, .b = 102, .a = 232 };
          int32_t hxc_tmp_load_result_n512 = hxc_left_h46be8694faff;
          int32_t hxc_tmp_load_result_n513 = hxc_top_h9bed6869cb5a;
          DrawRectangle((int32_t)hxc_tmp_load_result_n512, (int32_t)hxc_tmp_load_result_n513, (int32_t)360, (int32_t)54, hxc_this1_h00d59f9d7c58);
          struct Color hxc_this1_h0ee879d5f97c = (struct Color){ .r = 255, .g = 190, .b = 55, .a = 255 };
          int32_t hxc_tmp_load_result_n516 = hxc_left_h46be8694faff;
          int32_t hxc_tmp_load_result_n517 = hxc_top_h9bed6869cb5a;
          DrawRectangleLines((int32_t)hxc_tmp_load_result_n516, (int32_t)hxc_tmp_load_result_n517, (int32_t)360, (int32_t)54, hxc_this1_h0ee879d5f97c);
        }
        else
        {
          struct Color hxc_this1_h8d4a171cde40 = (struct Color){ .r = 6, .g = 26, .b = 36, .a = 218 };
          int32_t hxc_tmp_load_result_n520 = hxc_left_h46be8694faff;
          int32_t hxc_tmp_load_result_n521 = hxc_top_h9bed6869cb5a;
          DrawRectangle((int32_t)hxc_tmp_load_result_n520, (int32_t)hxc_tmp_load_result_n521, (int32_t)360, (int32_t)54, hxc_this1_h8d4a171cde40);
          struct Color hxc_this1_he31d35a0929d = (struct Color){ .r = 92, .g = 194, .b = 188, .a = 255 };
          int32_t hxc_tmp_load_result_n524 = hxc_left_h46be8694faff;
          int32_t hxc_tmp_load_result_n525 = hxc_top_h9bed6869cb5a;
          DrawRectangleLines((int32_t)hxc_tmp_load_result_n524, (int32_t)hxc_tmp_load_result_n525, (int32_t)360, (int32_t)54, hxc_this1_he31d35a0929d);
        }
        struct Color hxc_this1_hdf3d0dd71701 = (struct Color){ .r = 242, .g = 249, .b = 245, .a = 255 };
        int32_t hxc_tmp_load_result_n528 = hxc_left_h46be8694faff;
        int32_t hxc_tmp_load_result_n529 = hxc_top_h9bed6869cb5a;
        DrawText("AVENTURA", (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n528, 28), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n529, 16), (int32_t)21, hxc_this1_hdf3d0dd71701);
        int32_t hxc_x_hba336934bf60 = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_width_hbba54f7f7065, (double)2)), 270);
        struct Color hxc_this1_hc32b20afb966 = (struct Color){ .r = 229, .g = 241, .b = 235, .a = 255 };
        int32_t hxc_tmp_load_result_n533 = hxc_x_hba336934bf60;
        int32_t hxc_tmp_load_result_n534 = hxc_height_hae0a52249093;
        DrawText("FLECHAS / RATON PARA ELEGIR   ENTER PARA JUGAR   L: ENGLISH", (int32_t)hxc_tmp_load_result_n533, (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n534, 58), (int32_t)16, hxc_this1_hc32b20afb966);
      }
      else
      {
        int32_t hxc_left_h1e59ddc076fd = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_width_hbba54f7f7065, (double)2)), 180);
        if (hxc_selectedMode == 0)
        {
          struct Color hxc_this1_hb9cd5bc2da99 = (struct Color){ .r = 16, .g = 88, .b = 102, .a = 232 };
          int32_t hxc_tmp_load_result_n539 = hxc_left_h1e59ddc076fd;
          int32_t hxc_tmp_load_result_n540 = hxc_firstTop;
          DrawRectangle((int32_t)hxc_tmp_load_result_n539, (int32_t)hxc_tmp_load_result_n540, (int32_t)360, (int32_t)54, hxc_this1_hb9cd5bc2da99);
          struct Color hxc_this1_h8483b0afdf72 = (struct Color){ .r = 255, .g = 190, .b = 55, .a = 255 };
          int32_t hxc_tmp_load_result_n543 = hxc_left_h1e59ddc076fd;
          int32_t hxc_tmp_load_result_n544 = hxc_firstTop;
          DrawRectangleLines((int32_t)hxc_tmp_load_result_n543, (int32_t)hxc_tmp_load_result_n544, (int32_t)360, (int32_t)54, hxc_this1_h8483b0afdf72);
        }
        else
        {
          struct Color hxc_this1_h614bb4532ff9 = (struct Color){ .r = 6, .g = 26, .b = 36, .a = 218 };
          int32_t hxc_tmp_load_result_n547 = hxc_left_h1e59ddc076fd;
          int32_t hxc_tmp_load_result_n548 = hxc_firstTop;
          DrawRectangle((int32_t)hxc_tmp_load_result_n547, (int32_t)hxc_tmp_load_result_n548, (int32_t)360, (int32_t)54, hxc_this1_h614bb4532ff9);
          struct Color hxc_this1_h383548821bce = (struct Color){ .r = 92, .g = 194, .b = 188, .a = 255 };
          int32_t hxc_tmp_load_result_n551 = hxc_left_h1e59ddc076fd;
          int32_t hxc_tmp_load_result_n552 = hxc_firstTop;
          DrawRectangleLines((int32_t)hxc_tmp_load_result_n551, (int32_t)hxc_tmp_load_result_n552, (int32_t)360, (int32_t)54, hxc_this1_h383548821bce);
        }
        struct Color hxc_this1_hb0ed34032af3 = (struct Color){ .r = 242, .g = 249, .b = 245, .a = 255 };
        int32_t hxc_tmp_load_result_n555 = hxc_left_h1e59ddc076fd;
        int32_t hxc_tmp_load_result_n556 = hxc_firstTop;
        DrawText("CREATIVE MODE", (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n555, 28), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n556, 16), (int32_t)21, hxc_this1_hb0ed34032af3);
        int32_t hxc_top_hc17e01b4429b = hxc_i32_add_wrapping(hxc_i32_add_wrapping(hxc_firstTop, 54), 14);
        int32_t hxc_left_ha15eb709cfe3 = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_width_hbba54f7f7065, (double)2)), 180);
        if (hxc_selectedMode == 1)
        {
          struct Color hxc_this1_hd35901be76e6 = (struct Color){ .r = 16, .g = 88, .b = 102, .a = 232 };
          int32_t hxc_tmp_load_result_n562 = hxc_left_ha15eb709cfe3;
          int32_t hxc_tmp_load_result_n563 = hxc_top_hc17e01b4429b;
          DrawRectangle((int32_t)hxc_tmp_load_result_n562, (int32_t)hxc_tmp_load_result_n563, (int32_t)360, (int32_t)54, hxc_this1_hd35901be76e6);
          struct Color hxc_this1_h9cc2a7e11615 = (struct Color){ .r = 255, .g = 190, .b = 55, .a = 255 };
          int32_t hxc_tmp_load_result_n566 = hxc_left_ha15eb709cfe3;
          int32_t hxc_tmp_load_result_n567 = hxc_top_hc17e01b4429b;
          DrawRectangleLines((int32_t)hxc_tmp_load_result_n566, (int32_t)hxc_tmp_load_result_n567, (int32_t)360, (int32_t)54, hxc_this1_h9cc2a7e11615);
        }
        else
        {
          struct Color hxc_this1_h80dbf65504c7 = (struct Color){ .r = 6, .g = 26, .b = 36, .a = 218 };
          int32_t hxc_tmp_load_result_n570 = hxc_left_ha15eb709cfe3;
          int32_t hxc_tmp_load_result_n571 = hxc_top_hc17e01b4429b;
          DrawRectangle((int32_t)hxc_tmp_load_result_n570, (int32_t)hxc_tmp_load_result_n571, (int32_t)360, (int32_t)54, hxc_this1_h80dbf65504c7);
          struct Color hxc_this1_h7809470c02a5 = (struct Color){ .r = 92, .g = 194, .b = 188, .a = 255 };
          int32_t hxc_tmp_load_result_n574 = hxc_left_ha15eb709cfe3;
          int32_t hxc_tmp_load_result_n575 = hxc_top_hc17e01b4429b;
          DrawRectangleLines((int32_t)hxc_tmp_load_result_n574, (int32_t)hxc_tmp_load_result_n575, (int32_t)360, (int32_t)54, hxc_this1_h7809470c02a5);
        }
        struct Color hxc_this1_h5da40a40a706 = (struct Color){ .r = 242, .g = 249, .b = 245, .a = 255 };
        int32_t hxc_tmp_load_result_n578 = hxc_left_ha15eb709cfe3;
        int32_t hxc_tmp_load_result_n579 = hxc_top_hc17e01b4429b;
        DrawText("ADVENTURE", (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n578, 28), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n579, 16), (int32_t)21, hxc_this1_h5da40a40a706);
        int32_t hxc_x_h981e29606f46 = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_width_hbba54f7f7065, (double)2)), 270);
        struct Color hxc_this1_h62a900fdb424 = (struct Color){ .r = 229, .g = 241, .b = 235, .a = 255 };
        int32_t hxc_tmp_load_result_n583 = hxc_x_h981e29606f46;
        int32_t hxc_tmp_load_result_n584 = hxc_height_hae0a52249093;
        DrawText("ARROWS / MOUSE TO CHOOSE   ENTER TO PLAY   L: ESPANOL", (int32_t)hxc_tmp_load_result_n583, (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n584, 58), (int32_t)16, hxc_this1_h62a900fdb424);
      }
      if (hxc_selectedMode == 1)
      {
        if (hxc_language == 1)
        {
          int32_t hxc_x_h5f168ecb96c9 = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_width_hbba54f7f7065, (double)2)), 210);
          struct Color hxc_this1_h01aabed7e28a = (struct Color){ .r = 255, .g = 205, .b = 91, .a = 255 };
          int32_t hxc_tmp_load_result_n590 = hxc_x_h5f168ecb96c9;
          int32_t hxc_tmp_load_result_n591 = hxc_firstTop;
          DrawText("SALVA A CEESH. ENCUENTRA A IVVY. DETEN A BROWSER.", (int32_t)hxc_tmp_load_result_n590, (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n591, 36), (int32_t)17, hxc_this1_h01aabed7e28a);
        }
        else
        {
          int32_t hxc_x_h266a98785f73 = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_width_hbba54f7f7065, (double)2)), 210);
          struct Color hxc_this1_h15cae6f38156 = (struct Color){ .r = 255, .g = 205, .b = 91, .a = 255 };
          int32_t hxc_tmp_load_result_n595 = hxc_x_h266a98785f73;
          int32_t hxc_tmp_load_result_n596 = hxc_firstTop;
          DrawText("SAVE CEESH. FIND IVVY. STOP BROWSER.", (int32_t)hxc_tmp_load_result_n595, (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n596, 36), (int32_t)17, hxc_this1_h15cae6f38156);
        }
      }
    }
    else
    {
      ClearBackground((struct Color){ .r = 126, .g = 190, .b = 201, .a = 255 });
      BeginMode3D(hxc_camera);
      double hxc_tmp_record_field_load_result_n600 = hxc_player.hxc_x;
      struct hxc_caxecraft_app_Main_RenderCounters hxc_tmp_call_result_n602 = hxc_caxecraft_app_Main_drawWorld(hxc_cells, hxc_tmp_length_n1, hxc_tmp_record_field_load_result_n600, hxc_player.hxc_z);
      struct hxc_caxecraft_app_Main_RenderCounters hxc_renderCounters = hxc_tmp_call_result_n602;
      struct Camera3D hxc_tmp_load_result_n603 = hxc_camera;
      struct Texture hxc_tmp_load_result_n604 = hxc_entityTexture;
      bool hxc_tmp_load_result_n605 = hxc_entityTextureReady;
      struct hxc_caxecraft_gameplay_GuideState hxc_tmp_load_result_n606 = hxc_guide;
      struct hxc_caxecraft_gameplay_MosslingState hxc_tmp_load_result_n607 = hxc_mossling;
      hxc_caxecraft_app_Main_drawActors(hxc_tmp_load_result_n603, hxc_tmp_load_result_n604, hxc_tmp_load_result_n605, hxc_tmp_load_result_n606, hxc_tmp_load_result_n607, hxc_berryDrop);
      if (hxc_hit.hxc_hit)
      {
        double hxc_y = (double)hxc_hit.hxc_cellY + 0.5;
        double hxc_z = (double)hxc_hit.hxc_cellZ + 0.5;
        int32_t hxc_tmp_record_field_load_result_n612 = hxc_hit.hxc_cellX;
        double hxc_tmp_load_result_n613 = hxc_y;
        DrawCubeWires((struct Vector3){ .x = (float)((double)hxc_tmp_record_field_load_result_n612 + 0.5), .y = (float)hxc_tmp_load_result_n613, .z = (float)hxc_z }, (float)1.04, (float)1.04, (float)1.04, (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 });
      }
      EndMode3D();
      int32_t hxc_tmp_record_field_load_result_n617 = hxc_renderCounters.hxc_visible;
      int32_t hxc_tmp_record_field_load_result_n618 = hxc_renderCounters.hxc_drawCalls;
      int32_t hxc_tmp_load_result_n619 = hxc_frameCount;
      int32_t hxc_tmp_load_result_n620 = hxc_updateCount;
      bool hxc_tmp_load_result_n621 = hxc_paused;
      bool hxc_tmp_load_result_n622 = hxc_captured;
      int32_t hxc_tmp_load_result_n623 = hxc_placementBlockedFrames;
      struct hxc_caxecraft_domain_RaycastHit hxc_tmp_load_result_n624 = hxc_hit;
      double hxc_tmp_record_field_load_result_n625 = hxc_player.hxc_x;
      double hxc_tmp_record_field_load_result_n626 = hxc_player.hxc_z;
      int32_t hxc_tmp_load_result_n627 = hxc_selectedMode;
      int32_t hxc_tmp_load_result_n628 = hxc_language;
      struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_load_result_n629 = hxc_inventory;
      struct hxc_caxecraft_gameplay_GuideState hxc_tmp_load_result_n630 = hxc_guide;
      struct hxc_caxecraft_gameplay_MosslingState hxc_tmp_load_result_n631 = hxc_mossling;
      struct hxc_caxecraft_gameplay_PlayerVitalsState hxc_tmp_load_result_n632 = hxc_vitals;
      int32_t hxc_tmp_load_result_n633 = hxc_strikeHitFrames;
      int32_t hxc_tmp_load_result_n634 = hxc_enemyDefeatedFrames;
      int32_t hxc_tmp_load_result_n635 = hxc_enemyAttackFrames;
      int32_t hxc_tmp_load_result_n636 = hxc_pickupFrames;
      int32_t hxc_tmp_load_result_n637 = hxc_pickupAmount;
      int32_t hxc_tmp_load_result_n638 = hxc_inventoryFullReason;
      int32_t hxc_tmp_load_result_n639 = hxc_recoveryFeedback;
      int32_t hxc_tmp_load_result_n640 = hxc_recoveryFeedbackFrames;
      struct Texture hxc_tmp_load_result_n641 = hxc_hudTexture;
      bool hxc_tmp_load_result_n642 = hxc_hudTextureReady;
      struct Texture hxc_tmp_load_result_n643 = hxc_itemTexture;
      hxc_caxecraft_app_Main_drawHud(hxc_tmp_record_field_load_result_n617, hxc_tmp_record_field_load_result_n618, hxc_tmp_load_result_n619, hxc_tmp_load_result_n620, hxc_tmp_load_result_n621, hxc_tmp_load_result_n622, hxc_tmp_load_result_n623 > 0, hxc_tmp_load_result_n624, hxc_tmp_record_field_load_result_n625, hxc_tmp_record_field_load_result_n626, hxc_tmp_load_result_n627, hxc_tmp_load_result_n628, hxc_tmp_load_result_n629, hxc_tmp_load_result_n630, hxc_tmp_load_result_n631, hxc_tmp_load_result_n632, hxc_tmp_load_result_n633 > 0, hxc_tmp_load_result_n634 > 0, hxc_tmp_load_result_n635 > 0, hxc_tmp_load_result_n636 > 0, hxc_tmp_load_result_n637, hxc_tmp_load_result_n638, hxc_tmp_load_result_n639, hxc_tmp_load_result_n640 > 0, hxc_tmp_load_result_n641, hxc_tmp_load_result_n642, hxc_tmp_load_result_n643, hxc_itemTextureReady);
    }
    EndDrawing();
    hxc_frameCount = hxc_i32_add_wrapping(hxc_frameCount, 1);
  }
  EnableCursor();
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
