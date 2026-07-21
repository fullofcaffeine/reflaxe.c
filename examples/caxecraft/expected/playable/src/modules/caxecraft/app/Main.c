#include "hxc/program.h"

void hxc_caxecraft_app_Main_drawActors(struct hxc_caxecraft_gameplay_GuideState hxc_guide, struct hxc_caxecraft_gameplay_MosslingState hxc_mossling, struct hxc_caxecraft_gameplay_BerryDropState hxc_berryDrop)
{
  double hxc_y_h2134e3c1b839 = hxc_guide.hxc_y + 0.54;
  double hxc_z_h1f7bc378b4b2 = hxc_guide.hxc_z;
  double hxc_tmp_load_result_n3 = hxc_y_h2134e3c1b839;
  DrawCube((struct Vector3){ .x = (float)hxc_guide.hxc_x, .y = (float)hxc_tmp_load_result_n3, .z = (float)hxc_z_h1f7bc378b4b2 }, (float)0.50, (float)0.86, (float)0.42, (struct Color){ .r = 42, .g = 150, .b = 160, .a = 255 });
  double hxc_y_h7167c4eac9f5 = hxc_guide.hxc_y + 1.18;
  double hxc_z_hce9e194f5371 = hxc_guide.hxc_z;
  double hxc_tmp_load_result_n10 = hxc_y_h7167c4eac9f5;
  DrawCube((struct Vector3){ .x = (float)hxc_guide.hxc_x, .y = (float)hxc_tmp_load_result_n10, .z = (float)hxc_z_hce9e194f5371 }, (float)0.44, (float)0.44, (float)0.44, (struct Color){ .r = 205, .g = 139, .b = 88, .a = 255 });
  double hxc_y_h3d42125255cb = hxc_guide.hxc_y + 1.41;
  double hxc_z_h7f6f7c05e484 = hxc_guide.hxc_z;
  double hxc_tmp_load_result_n17 = hxc_y_h3d42125255cb;
  DrawCube((struct Vector3){ .x = (float)hxc_guide.hxc_x, .y = (float)hxc_tmp_load_result_n17, .z = (float)hxc_z_h7f6f7c05e484 }, (float)0.48, (float)0.16, (float)0.48, (struct Color){ .r = 62, .g = 40, .b = 47, .a = 255 });
  if (hxc_mossling.hxc_health > 0)
  {
    double hxc_y_ha0ae358edfb8 = hxc_mossling.hxc_y + 0.30;
    double hxc_z_h597bfdbdbaba = hxc_mossling.hxc_z;
    double hxc_tmp_load_result_n25 = hxc_y_ha0ae358edfb8;
    DrawCube((struct Vector3){ .x = (float)hxc_mossling.hxc_x, .y = (float)hxc_tmp_load_result_n25, .z = (float)hxc_z_h597bfdbdbaba }, (float)0.70, (float)0.54, (float)0.70, (struct Color){ .r = 74, .g = 119, .b = 67, .a = 255 });
    double hxc_y_h821fdb2bc6d9 = hxc_mossling.hxc_y + 0.66;
    double hxc_z_h3c859b2455ad = hxc_mossling.hxc_z;
    double hxc_tmp_load_result_n32 = hxc_y_h821fdb2bc6d9;
    DrawCube((struct Vector3){ .x = (float)hxc_mossling.hxc_x, .y = (float)hxc_tmp_load_result_n32, .z = (float)hxc_z_h3c859b2455ad }, (float)0.50, (float)0.34, (float)0.50, (struct Color){ .r = 157, .g = 190, .b = 82, .a = 255 });
  }
  if (hxc_berryDrop.hxc_active)
  {
    double hxc_y_hc495cb733d6d = hxc_berryDrop.hxc_y;
    double hxc_z_h1b0de7e5de9e = hxc_berryDrop.hxc_z;
    double hxc_tmp_load_result_n40 = hxc_y_hc495cb733d6d;
    DrawCube((struct Vector3){ .x = (float)(hxc_berryDrop.hxc_x - 0.12), .y = (float)hxc_tmp_load_result_n40, .z = (float)hxc_z_h1b0de7e5de9e }, (float)0.18, (float)0.18, (float)0.18, (struct Color){ .r = 174, .g = 78, .b = 136, .a = 255 });
    double hxc_y_hb276ff851bd5 = hxc_berryDrop.hxc_y;
    double hxc_z_h4b55cc50e58e = hxc_berryDrop.hxc_z;
    double hxc_tmp_load_result_n47 = hxc_y_hb276ff851bd5;
    DrawCube((struct Vector3){ .x = (float)(hxc_berryDrop.hxc_x + 0.12), .y = (float)hxc_tmp_load_result_n47, .z = (float)hxc_z_h4b55cc50e58e }, (float)0.18, (float)0.18, (float)0.18, (struct Color){ .r = 174, .g = 78, .b = 136, .a = 255 });
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

void hxc_caxecraft_app_Main_drawHud(int32_t hxc_visible, int32_t hxc_drawCalls, int32_t hxc_frames, int32_t hxc_updates, bool hxc_paused, bool hxc_captured, bool hxc_placementBlocked, struct hxc_caxecraft_domain_RaycastHit hxc_hit, double hxc_playerX, double hxc_playerZ, int32_t hxc_mode, int32_t hxc_language, struct hxc_caxecraft_gameplay_InventoryState hxc_inventory, struct hxc_caxecraft_gameplay_GuideState hxc_guide, struct hxc_caxecraft_gameplay_MosslingState hxc_mossling, struct hxc_caxecraft_gameplay_PlayerVitalsState hxc_vitals, bool hxc_strikeHit, bool hxc_enemyDefeated, bool hxc_pickedUp, int32_t hxc_recoveryFeedback, bool hxc_recoveryVisible, struct Texture hxc_hudTexture, bool hxc_hudTextureReady, struct Texture hxc_itemTexture, bool hxc_itemTextureReady)
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
  struct Color hxc_this1_hd89476e43ea4 = (struct Color){ .r = 15, .g = 27, .b = 39, .a = 220 };
  DrawRectangle((int32_t)18, (int32_t)18, (int32_t)460, (int32_t)108, hxc_this1_hd89476e43ea4);
  struct Color hxc_this1_hf7190421da1b = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
  DrawRectangleLines((int32_t)18, (int32_t)18, (int32_t)460, (int32_t)108, hxc_this1_hf7190421da1b);
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
      struct Color hxc_this1_hdb58de3efebe = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
      DrawText("AVENTURA: PROLOGO EN CONSTRUCCION", (int32_t)32, (int32_t)110, (int32_t)14, hxc_this1_hdb58de3efebe);
    }
    else
    {
      struct Color hxc_this1_he23c5d72ffd1 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
      DrawText("ADVENTURE: PROLOGUE IN PROGRESS", (int32_t)32, (int32_t)110, (int32_t)14, hxc_this1_he23c5d72ffd1);
    }
  }
  bool hxc_tmp_call_result_n49 = hxc_caxecraft_gameplay_GuideNpc_isInRange(hxc_guide, hxc_playerX, hxc_playerZ);
  if (hxc_tmp_call_result_n49)
  {
    struct Color hxc_this1_h7af923e856ba = (struct Color){ .r = 15, .g = 27, .b = 39, .a = 220 };
    int32_t hxc_tmp_load_result_n51 = hxc_centerX;
    int32_t hxc_tmp_load_result_n52 = hxc_centerY;
    DrawRectangle((int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n51, 260), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n52, 54), (int32_t)520, (int32_t)60, hxc_this1_h7af923e856ba);
    int32_t hxc_code_hcdb5898043c2 = hxc_guide.hxc_phaseCode;
    int32_t hxc_tmp_load_result_n55 = hxc_code_hcdb5898043c2;
    int32_t hxc_tmp_conditional_result_n36 = 0;
    if (hxc_tmp_load_result_n55 == 1)
    {
      hxc_tmp_conditional_result_n36 = 1;
    }
    else
    {
      int32_t hxc_tmp_load_result_n56 = hxc_code_hcdb5898043c2;
      int32_t hxc_tmp_conditional_result_n37 = 0;
      if (hxc_tmp_load_result_n56 == 2)
      {
        hxc_tmp_conditional_result_n37 = 2;
      }
      else
      {
        hxc_tmp_conditional_result_n37 = 0;
      }
      hxc_tmp_conditional_result_n36 = hxc_tmp_conditional_result_n37;
    }
    if (hxc_tmp_conditional_result_n36 == 0)
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
      int32_t hxc_code_hf59e061683f7 = hxc_guide.hxc_phaseCode;
      int32_t hxc_tmp_load_result_n66 = hxc_code_hf59e061683f7;
      int32_t hxc_tmp_conditional_result_n39 = 0;
      if (hxc_tmp_load_result_n66 == 1)
      {
        hxc_tmp_conditional_result_n39 = 1;
      }
      else
      {
        int32_t hxc_tmp_load_result_n67 = hxc_code_hf59e061683f7;
        int32_t hxc_tmp_conditional_result_n40 = 0;
        if (hxc_tmp_load_result_n67 == 2)
        {
          hxc_tmp_conditional_result_n40 = 2;
        }
        else
        {
          hxc_tmp_conditional_result_n40 = 0;
        }
        hxc_tmp_conditional_result_n39 = hxc_tmp_conditional_result_n40;
      }
      if (hxc_tmp_conditional_result_n39 == 1)
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
    int32_t hxc_code_he8a643d5c2b7 = hxc_mossling.hxc_modeCode;
    int32_t hxc_tmp_load_result_n84 = hxc_code_he8a643d5c2b7;
    int32_t hxc_tmp_conditional_result_n42 = 0;
    if (hxc_tmp_load_result_n84 == 1)
    {
      hxc_tmp_conditional_result_n42 = 1;
    }
    else
    {
      int32_t hxc_tmp_load_result_n85 = hxc_code_he8a643d5c2b7;
      int32_t hxc_tmp_conditional_result_n43 = 0;
      if (hxc_tmp_load_result_n85 == 2)
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
        struct Color hxc_this1_hc58fa1497b86 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
        int32_t hxc_tmp_load_result_n89 = hxc_width;
        DrawText("MUSGUITO ALERTA", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n89, 180), (int32_t)28, (int32_t)16, hxc_this1_hc58fa1497b86);
      }
      else
      {
        struct Color hxc_this1_hc1bd1e72c042 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
        int32_t hxc_tmp_load_result_n92 = hxc_width;
        DrawText("MOSSLING ALERT", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n92, 180), (int32_t)28, (int32_t)16, hxc_this1_hc1bd1e72c042);
      }
    }
  }
  if (hxc_strikeHit)
  {
    if (hxc_language == 1)
    {
      struct Color hxc_this1_h86c190906b0c = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
      int32_t hxc_tmp_load_result_n95 = hxc_centerX;
      int32_t hxc_tmp_load_result_n96 = hxc_centerY;
      DrawText("GOLPE DE COBRE", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n95, 70), (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n96, 54), (int32_t)18, hxc_this1_h86c190906b0c);
    }
    else
    {
      struct Color hxc_this1_hc43cec510ebe = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
      int32_t hxc_tmp_load_result_n99 = hxc_centerX;
      int32_t hxc_tmp_load_result_n100 = hxc_centerY;
      DrawText("COPPER STRIKE", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n99, 70), (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n100, 54), (int32_t)18, hxc_this1_hc43cec510ebe);
    }
  }
  if (hxc_enemyDefeated)
  {
    if (hxc_language == 1)
    {
      struct Color hxc_this1_hd8d753eae913 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
      int32_t hxc_tmp_load_result_n103 = hxc_width;
      DrawText("EL MUSGUITO SOLTO BAYAS", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n103, 275), (int32_t)54, (int32_t)16, hxc_this1_hd8d753eae913);
    }
    else
    {
      struct Color hxc_this1_h52962b754abe = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
      int32_t hxc_tmp_load_result_n106 = hxc_width;
      DrawText("MOSSLING DROPPED BERRIES", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n106, 270), (int32_t)54, (int32_t)16, hxc_this1_h52962b754abe);
    }
  }
  if (hxc_pickedUp)
  {
    if (hxc_language == 1)
    {
      struct Color hxc_this1_he99ec2a6e941 = (struct Color){ .r = 174, .g = 78, .b = 136, .a = 255 };
      int32_t hxc_tmp_load_result_n109 = hxc_centerX;
      int32_t hxc_tmp_load_result_n110 = hxc_centerY;
      DrawText("+2 BAYAS", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n109, 42), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n110, 24), (int32_t)18, hxc_this1_he99ec2a6e941);
    }
    else
    {
      struct Color hxc_this1_hc93c31ee53d1 = (struct Color){ .r = 174, .g = 78, .b = 136, .a = 255 };
      int32_t hxc_tmp_load_result_n113 = hxc_centerX;
      int32_t hxc_tmp_load_result_n114 = hxc_centerY;
      DrawText("+2 BERRIES", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n113, 48), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n114, 24), (int32_t)18, hxc_this1_hc93c31ee53d1);
    }
  }
  if (hxc_recoveryVisible)
  {
    if (hxc_recoveryFeedback == 1)
    {
      if (hxc_language == 1)
      {
        struct Color hxc_this1_ha2174812eb7a = (struct Color){ .r = 94, .g = 212, .b = 136, .a = 255 };
        int32_t hxc_tmp_load_result_n117 = hxc_centerX;
        int32_t hxc_tmp_load_result_n118 = hxc_centerY;
        DrawText("BAYAS: +1 CORAZON", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n117, 86), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n118, 24), (int32_t)18, hxc_this1_ha2174812eb7a);
      }
      else
      {
        struct Color hxc_this1_hf801f4b7a3a0 = (struct Color){ .r = 94, .g = 212, .b = 136, .a = 255 };
        int32_t hxc_tmp_load_result_n121 = hxc_centerX;
        int32_t hxc_tmp_load_result_n122 = hxc_centerY;
        DrawText("BERRIES: +1 HEART", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n121, 88), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n122, 24), (int32_t)18, hxc_this1_hf801f4b7a3a0);
      }
    }
    else
    {
      if (hxc_recoveryFeedback == 2)
      {
        if (hxc_language == 1)
        {
          struct Color hxc_this1_h5b9db85f17ba = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
          int32_t hxc_tmp_load_result_n125 = hxc_centerX;
          int32_t hxc_tmp_load_result_n126 = hxc_centerY;
          DrawText("SALUD COMPLETA", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n125, 72), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n126, 24), (int32_t)18, hxc_this1_h5b9db85f17ba);
        }
        else
        {
          struct Color hxc_this1_h797c33fccfa4 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
          int32_t hxc_tmp_load_result_n129 = hxc_centerX;
          int32_t hxc_tmp_load_result_n130 = hxc_centerY;
          DrawText("HEALTH ALREADY FULL", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n129, 96), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n130, 24), (int32_t)18, hxc_this1_h797c33fccfa4);
        }
      }
      else
      {
        if (hxc_recoveryFeedback == 3)
        {
          if (hxc_language == 1)
          {
            struct Color hxc_this1_h60b5437b1b94 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
            int32_t hxc_tmp_load_result_n133 = hxc_centerX;
            int32_t hxc_tmp_load_result_n134 = hxc_centerY;
            DrawText("NO QUEDAN BAYAS", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n133, 76), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n134, 24), (int32_t)18, hxc_this1_h60b5437b1b94);
          }
          else
          {
            struct Color hxc_this1_hf3186c77aede = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
            int32_t hxc_tmp_load_result_n137 = hxc_centerX;
            int32_t hxc_tmp_load_result_n138 = hxc_centerY;
            DrawText("NO BERRIES LEFT", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n137, 76), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n138, 24), (int32_t)18, hxc_this1_hf3186c77aede);
          }
        }
      }
    }
  }
  if (hxc_vitals.hxc_safeTicks > 15)
  {
    struct Color hxc_this1_hf85691db48df = (struct Color){ .r = 218, .g = 65, .b = 72, .a = 255 };
    int32_t hxc_tmp_load_result_n142 = hxc_width;
    int32_t hxc_tmp_load_result_n143 = hxc_height;
    DrawRectangleLines((int32_t)4, (int32_t)4, (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n142, 8), (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n143, 8), hxc_this1_hf85691db48df);
  }
  if (hxc_vitals.hxc_health <= 0)
  {
    struct Color hxc_this1_hafef3e127380 = (struct Color){ .r = 15, .g = 27, .b = 39, .a = 220 };
    int32_t hxc_tmp_load_result_n147 = hxc_centerX;
    int32_t hxc_tmp_load_result_n148 = hxc_centerY;
    DrawRectangle((int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n147, 250), (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n148, 74), (int32_t)500, (int32_t)148, hxc_this1_hafef3e127380);
    struct Color hxc_this1_h69b16996cd81 = (struct Color){ .r = 218, .g = 65, .b = 72, .a = 255 };
    int32_t hxc_tmp_load_result_n151 = hxc_centerX;
    int32_t hxc_tmp_load_result_n152 = hxc_centerY;
    DrawRectangleLines((int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n151, 250), (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n152, 74), (int32_t)500, (int32_t)148, hxc_this1_h69b16996cd81);
    if (hxc_language == 1)
    {
      int32_t hxc_tmp_load_result_n154 = hxc_centerX;
      int32_t hxc_tmp_load_result_n155 = hxc_centerY;
      DrawText("HAXIRIO HA CAIDO", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n154, 112), (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n155, 42), (int32_t)24, hxc_text);
      struct Color hxc_this1_h9e7ddbf4858a = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
      int32_t hxc_tmp_load_result_n158 = hxc_centerX;
      int32_t hxc_tmp_load_result_n159 = hxc_centerY;
      DrawText("E  VOLVER AL PRADO", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n158, 105), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n159, 10), (int32_t)18, hxc_this1_h9e7ddbf4858a);
    }
    else
    {
      int32_t hxc_tmp_load_result_n161 = hxc_centerX;
      int32_t hxc_tmp_load_result_n162 = hxc_centerY;
      DrawText("HAXIRIO HAS FALLEN", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n161, 122), (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n162, 42), (int32_t)24, hxc_text);
      struct Color hxc_this1_h6c17c8cc4d2d = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
      int32_t hxc_tmp_load_result_n165 = hxc_centerX;
      int32_t hxc_tmp_load_result_n166 = hxc_centerY;
      DrawText("E  RETURN TO THE MEADOW", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n165, 125), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n166, 10), (int32_t)18, hxc_this1_h6c17c8cc4d2d);
    }
  }
  if (hxc_paused)
  {
    struct Color hxc_this1_habc8f1419da5 = (struct Color){ .r = 15, .g = 27, .b = 39, .a = 220 };
    int32_t hxc_tmp_load_result_n169 = hxc_centerX;
    int32_t hxc_tmp_load_result_n170 = hxc_centerY;
    DrawRectangle((int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n169, 170), (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n170, 48), (int32_t)340, (int32_t)96, hxc_this1_habc8f1419da5);
    struct Color hxc_this1_hfd4763b95ed5 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
    int32_t hxc_tmp_load_result_n173 = hxc_centerX;
    int32_t hxc_tmp_load_result_n174 = hxc_centerY;
    DrawRectangleLines((int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n173, 170), (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n174, 48), (int32_t)340, (int32_t)96, hxc_this1_hfd4763b95ed5);
    int32_t hxc_tmp_load_result_n176 = hxc_centerX;
    int32_t hxc_tmp_load_result_n177 = hxc_centerY;
    DrawText("PAUSED", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n176, 48), (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n177, 30), (int32_t)24, hxc_text);
    int32_t hxc_tmp_load_result_n179 = hxc_centerX;
    int32_t hxc_tmp_load_result_n180 = hxc_centerY;
    DrawText("CLICK TO CAPTURE  //  Q TO QUIT", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n179, 145), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n180, 8), (int32_t)16, hxc_text);
  }
  else
  {
    if (hxc_placementBlocked)
    {
      struct Color hxc_this1_h7db4a9dd62b8 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
      int32_t hxc_tmp_load_result_n183 = hxc_centerX;
      int32_t hxc_tmp_load_result_n184 = hxc_centerY;
      DrawText("PLACE BLOCKED: PLAYER OR WORLD EDGE", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n183, 150), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n184, 26), (int32_t)14, hxc_this1_h7db4a9dd62b8);
    }
    else
    {
      if (!hxc_captured)
      {
        int32_t hxc_tmp_load_result_n186 = hxc_centerX;
        int32_t hxc_tmp_load_result_n187 = hxc_centerY;
        DrawText("CLICK TO CAPTURE", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n186, 74), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n187, 26), (int32_t)14, hxc_text);
      }
      else
      {
        if (!hxc_hit.hxc_hit)
        {
          int32_t hxc_tmp_load_result_n190 = hxc_centerX;
          int32_t hxc_tmp_load_result_n191 = hxc_centerY;
          DrawText("NO BLOCK IN REACH", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n190, 78), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n191, 26), (int32_t)14, hxc_text);
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
  struct hxc_caxecraft_domain_PlayerState hxc_tmp_call_result_n14 = hxc_caxecraft_app_Main_spawnPlayer(hxc_cells, hxc_tmp_length_n1);
  struct hxc_caxecraft_domain_PlayerState hxc_player = hxc_tmp_call_result_n14;
  struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_call_result_n15 = hxc_caxecraft_gameplay_Inventory_starter();
  struct hxc_caxecraft_gameplay_InventoryState hxc_inventory = hxc_tmp_call_result_n15;
  struct hxc_caxecraft_gameplay_GuideState hxc_tmp_call_result_n16 = hxc_caxecraft_gameplay_GuideNpc_start(hxc_cells, hxc_tmp_length_n1, 17.5, 13.5);
  struct hxc_caxecraft_gameplay_GuideState hxc_guide = hxc_tmp_call_result_n16;
  struct hxc_caxecraft_gameplay_MosslingState hxc_tmp_call_result_n17 = hxc_caxecraft_gameplay_Mossling_start(hxc_cells, hxc_tmp_length_n1, 15.5, 13.8);
  struct hxc_caxecraft_gameplay_MosslingState hxc_mossling = hxc_tmp_call_result_n17;
  int32_t hxc_initialHealth = 6;
  struct hxc_caxecraft_gameplay_PlayerVitalsState hxc_tmp_call_result_n19 = hxc_caxecraft_gameplay_PlayerVitals_make(hxc_initialHealth, 0);
  struct hxc_caxecraft_gameplay_PlayerVitalsState hxc_vitals = hxc_tmp_call_result_n19;
  struct hxc_caxecraft_gameplay_BerryDropState hxc_berryDrop = (struct hxc_caxecraft_gameplay_BerryDropState){ .hxc_active = false, .hxc_amount = 0, .hxc_x = 0.0, .hxc_y = 0.0, .hxc_z = 0.0 };
  double hxc_lookX = 0.0;
  double hxc_lookY = -0.18;
  double hxc_lookZ = -1.0;
  double hxc_accumulator = 0.0;
  bool hxc_jumpQueued = false;
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
  int32_t hxc_pickupFrames = 0;
  int32_t hxc_recoveryFeedback = 0;
  int32_t hxc_recoveryFeedbackFrames = 0;
  while (1)
  {
    bool hxc_tmp_load_result_n21 = hxc_quit;
    bool hxc_tmp_short_circuit_result_n37 = !hxc_tmp_load_result_n21;
    if (!hxc_tmp_load_result_n21)
    {
      bool hxc_tmp_native_call_result_n22 = WindowShouldClose();
      hxc_tmp_short_circuit_result_n37 = !hxc_tmp_native_call_result_n22;
    }
    if (!hxc_tmp_short_circuit_result_n37)
    {
      break;
    }
    bool hxc_recapturedThisFrame = false;
    bool hxc_tmp_native_call_result_n24 = IsWindowFocused();
    bool hxc_focused = hxc_tmp_native_call_result_n24;
    double hxc_forward = 0.0;
    double hxc_right = 0.0;
    bool hxc_tmp_native_call_result_n25 = IsKeyDown((int32_t)87);
    if (hxc_tmp_native_call_result_n25)
    {
      hxc_forward = hxc_forward + 1.0;
    }
    bool hxc_tmp_native_call_result_n27 = IsKeyDown((int32_t)83);
    if (hxc_tmp_native_call_result_n27)
    {
      hxc_forward = hxc_forward - 1.0;
    }
    bool hxc_tmp_native_call_result_n29 = IsKeyDown((int32_t)68);
    if (hxc_tmp_native_call_result_n29)
    {
      hxc_right = hxc_right + 1.0;
    }
    bool hxc_tmp_native_call_result_n31 = IsKeyDown((int32_t)65);
    if (hxc_tmp_native_call_result_n31)
    {
      hxc_right = hxc_right - 1.0;
    }
    double hxc_lookYaw_h588b142be2e3 = 0.0;
    double hxc_lookPitch_hdd80ca5aa736 = 0.0;
    if (hxc_captured)
    {
      struct Vector2 hxc_tmp_native_call_result_n34 = GetMouseDelta();
      struct Vector2 hxc_mouse = hxc_tmp_native_call_result_n34;
      hxc_lookYaw_h588b142be2e3 = -(double)hxc_mouse.x * 0.0025;
      hxc_lookPitch_hdd80ca5aa736 = -(double)hxc_mouse.y * 0.0025;
    }
    bool hxc_tmp_native_call_result_n39 = IsMouseButtonPressed((int32_t)0);
    bool hxc_leftPressed = hxc_tmp_native_call_result_n39;
    bool hxc_tmp_load_result_n40 = hxc_captured;
    bool hxc_tmp_short_circuit_result_n47 = hxc_tmp_load_result_n40;
    if (hxc_tmp_load_result_n40)
    {
      hxc_tmp_short_circuit_result_n47 = hxc_leftPressed;
    }
    bool hxc_primaryPressed_h36b30f56e49a = hxc_tmp_short_circuit_result_n47;
    bool hxc_tmp_load_result_n43 = hxc_captured;
    bool hxc_tmp_short_circuit_result_n49 = hxc_tmp_load_result_n43;
    if (hxc_tmp_load_result_n43)
    {
      bool hxc_tmp_native_call_result_n44 = IsMouseButtonPressed((int32_t)1);
      hxc_tmp_short_circuit_result_n49 = hxc_tmp_native_call_result_n44;
    }
    bool hxc_secondaryPressed_h847ea36d088d = hxc_tmp_short_circuit_result_n49;
    bool hxc_tmp_load_result_n46 = hxc_captured;
    bool hxc_tmp_short_circuit_result_n51 = hxc_tmp_load_result_n46;
    if (hxc_tmp_load_result_n46)
    {
      bool hxc_tmp_native_call_result_n47 = IsKeyPressed((int32_t)69);
      hxc_tmp_short_circuit_result_n51 = hxc_tmp_native_call_result_n47;
    }
    bool hxc_interactPressed_h67f7f318449e = hxc_tmp_short_circuit_result_n51;
    int32_t hxc_hotbarSelection_h24c4a4fc4cb5 = -1;
    bool hxc_tmp_native_call_result_n49 = IsKeyPressed((int32_t)49);
    if (hxc_tmp_native_call_result_n49)
    {
      hxc_hotbarSelection_h24c4a4fc4cb5 = 0;
    }
    bool hxc_tmp_native_call_result_n50 = IsKeyPressed((int32_t)50);
    if (hxc_tmp_native_call_result_n50)
    {
      hxc_hotbarSelection_h24c4a4fc4cb5 = 1;
    }
    bool hxc_tmp_native_call_result_n51 = IsKeyPressed((int32_t)51);
    if (hxc_tmp_native_call_result_n51)
    {
      hxc_hotbarSelection_h24c4a4fc4cb5 = 2;
    }
    bool hxc_tmp_native_call_result_n52 = IsKeyPressed((int32_t)52);
    if (hxc_tmp_native_call_result_n52)
    {
      hxc_hotbarSelection_h24c4a4fc4cb5 = 3;
    }
    bool hxc_tmp_native_call_result_n53 = IsKeyPressed((int32_t)53);
    if (hxc_tmp_native_call_result_n53)
    {
      hxc_hotbarSelection_h24c4a4fc4cb5 = 4;
    }
    bool hxc_tmp_native_call_result_n54 = IsKeyPressed((int32_t)54);
    if (hxc_tmp_native_call_result_n54)
    {
      hxc_hotbarSelection_h24c4a4fc4cb5 = 5;
    }
    bool hxc_tmp_native_call_result_n55 = IsKeyPressed((int32_t)55);
    if (hxc_tmp_native_call_result_n55)
    {
      hxc_hotbarSelection_h24c4a4fc4cb5 = 6;
    }
    bool hxc_tmp_native_call_result_n56 = IsKeyPressed((int32_t)56);
    if (hxc_tmp_native_call_result_n56)
    {
      hxc_hotbarSelection_h24c4a4fc4cb5 = 7;
    }
    float hxc_tmp_native_call_result_n57 = GetMouseWheelMove();
    double hxc_wheel = (double)hxc_tmp_native_call_result_n57;
    int32_t hxc_hotbarCycle_hceb3392a1b61 = 0;
    if (hxc_wheel > 0.0)
    {
      hxc_hotbarCycle_hceb3392a1b61 = -1;
    }
    if (hxc_wheel < 0.0)
    {
      hxc_hotbarCycle_hceb3392a1b61 = 1;
    }
    bool hxc_tmp_native_call_result_n60 = IsKeyPressed((int32_t)32);
    bool hxc_jumpPressed_hdcf1df9aeaa1 = hxc_tmp_native_call_result_n60;
    bool hxc_tmp_native_call_result_n61 = IsKeyPressed((int32_t)256);
    bool hxc_pausePressed_hb82c8b255c7c = hxc_tmp_native_call_result_n61;
    bool hxc_tmp_native_call_result_n62 = IsKeyPressed((int32_t)81);
    bool hxc_quitPressed_h3cb23639ab79 = hxc_tmp_native_call_result_n62;
    double hxc_frameInput_moveForward = hxc_forward;
    double hxc_frameInput_moveRight = hxc_right;
    double hxc_frameInput_lookYaw = hxc_lookYaw_h588b142be2e3;
    double hxc_frameInput_lookPitch = hxc_lookPitch_hdd80ca5aa736;
    bool hxc_frameInput_jumpPressed = hxc_jumpPressed_hdcf1df9aeaa1;
    bool hxc_frameInput_primaryPressed = hxc_primaryPressed_h36b30f56e49a;
    bool hxc_frameInput_secondaryPressed = hxc_secondaryPressed_h847ea36d088d;
    bool hxc_frameInput_interactPressed = hxc_interactPressed_h67f7f318449e;
    bool hxc_frameInput_pausePressed = hxc_pausePressed_hb82c8b255c7c;
    bool hxc_tmp_load_result_n72 = hxc_paused;
    bool hxc_tmp_short_circuit_result_n68 = hxc_tmp_load_result_n72;
    if (hxc_tmp_load_result_n72)
    {
      hxc_tmp_short_circuit_result_n68 = hxc_leftPressed;
    }
    bool hxc_frameInput_capturePressed = hxc_tmp_short_circuit_result_n68;
    bool hxc_frameInput_quitPressed = hxc_quitPressed_h3cb23639ab79;
    int32_t hxc_frameInput_hotbarSelection = hxc_hotbarSelection_h24c4a4fc4cb5;
    int32_t hxc_frameInput_hotbarCycle = hxc_hotbarCycle_hceb3392a1b61;
    double hxc_moveForward = hxc_frameInput_moveForward;
    double hxc_moveRight = hxc_frameInput_moveRight;
    double hxc_lookYaw_hc9c3902167f1 = hxc_frameInput_lookYaw;
    double hxc_lookPitch_h17c6c0519fbe = hxc_frameInput_lookPitch;
    bool hxc_jumpPressed_h6a6f2e35b6f8 = hxc_frameInput_jumpPressed;
    bool hxc_primaryPressed_h9f9c138d17c0 = hxc_frameInput_primaryPressed;
    bool hxc_secondaryPressed_h400b391dc5af = hxc_frameInput_secondaryPressed;
    bool hxc_interactPressed_h9e53b893ba59 = hxc_frameInput_interactPressed;
    bool hxc_pausePressed_h78002f09b1a9 = hxc_frameInput_pausePressed;
    bool hxc_capturePressed = hxc_frameInput_capturePressed;
    bool hxc_quitPressed_haa26bcd5efc6 = hxc_frameInput_quitPressed;
    int32_t hxc_hotbarSelection_h23c788af54d7 = hxc_frameInput_hotbarSelection;
    int32_t hxc_hotbarCycle_h9c7ebc3323a4 = hxc_frameInput_hotbarCycle;
    if (hxc_quitPressed_haa26bcd5efc6)
    {
      hxc_quit = true;
    }
    if (hxc_hotbarSelection_h23c788af54d7 >= 0)
    {
      struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_load_result_n93 = hxc_inventory;
      struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_call_result_n95 = hxc_caxecraft_gameplay_Inventory_select(hxc_tmp_load_result_n93, hxc_hotbarSelection_h23c788af54d7);
      hxc_inventory = hxc_tmp_call_result_n95;
    }
    if (hxc_hotbarCycle_h9c7ebc3323a4 != 0)
    {
      struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_load_result_n97 = hxc_inventory;
      struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_call_result_n99 = hxc_caxecraft_gameplay_Inventory_cycle(hxc_tmp_load_result_n97, hxc_hotbarCycle_h9c7ebc3323a4);
      hxc_inventory = hxc_tmp_call_result_n99;
    }
    bool hxc_tmp_load_result_n100 = hxc_onTitle;
    bool hxc_tmp_short_circuit_result_n85 = !hxc_tmp_load_result_n100;
    if (!hxc_tmp_load_result_n100)
    {
      hxc_tmp_short_circuit_result_n85 = !hxc_paused;
    }
    bool hxc_tmp_short_circuit_load_result_n102 = hxc_tmp_short_circuit_result_n85;
    bool hxc_tmp_short_circuit_result_n86 = hxc_tmp_short_circuit_load_result_n102;
    if (hxc_tmp_short_circuit_load_result_n102)
    {
      hxc_tmp_short_circuit_result_n86 = hxc_interactPressed_h9e53b893ba59;
    }
    if (hxc_tmp_short_circuit_result_n86)
    {
      if (hxc_vitals.hxc_health <= 0)
      {
        struct hxc_caxecraft_gameplay_PlayerVitalsState hxc_tmp_call_result_n106 = hxc_caxecraft_gameplay_PlayerVitals_make(6, 0);
        hxc_vitals = hxc_tmp_call_result_n106;
        struct hxc_caxecraft_domain_PlayerState hxc_tmp_call_result_n107 = hxc_caxecraft_app_Main_spawnPlayer(hxc_cells, hxc_tmp_length_n1);
        hxc_player = hxc_tmp_call_result_n107;
      }
      else
      {
        struct hxc_caxecraft_gameplay_GuideState hxc_tmp_load_result_n108 = hxc_guide;
        double hxc_tmp_record_field_load_result_n109 = hxc_player.hxc_x;
        bool hxc_tmp_call_result_n111 = hxc_caxecraft_gameplay_GuideNpc_isInRange(hxc_tmp_load_result_n108, hxc_tmp_record_field_load_result_n109, hxc_player.hxc_z);
        if (hxc_tmp_call_result_n111)
        {
          bool hxc_tmp_call_result_n113 = hxc_caxecraft_gameplay_GuideNpc_sharesBerriesOnNextInteraction(hxc_guide);
          bool hxc_sharesBerries = hxc_tmp_call_result_n113;
          struct hxc_caxecraft_gameplay_GuideState hxc_tmp_call_result_n115 = hxc_caxecraft_gameplay_GuideNpc_interact(hxc_guide);
          hxc_guide = hxc_tmp_call_result_n115;
          if (hxc_sharesBerries)
          {
            struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_call_result_n119 = hxc_caxecraft_gameplay_Inventory_collectItem(hxc_inventory, hxc_caxecraft_gameplay_ItemKind_Berries, 2);
            hxc_inventory = hxc_tmp_call_result_n119;
          }
        }
      }
    }
    bool hxc_tmp_load_result_n120 = hxc_onTitle;
    bool hxc_tmp_short_circuit_result_n88 = hxc_tmp_load_result_n120;
    if (hxc_tmp_load_result_n120)
    {
      hxc_tmp_short_circuit_result_n88 = hxc_focused;
    }
    if (hxc_tmp_short_circuit_result_n88)
    {
      bool hxc_tmp_native_call_result_n123 = IsKeyPressed((int32_t)76);
      if (hxc_tmp_native_call_result_n123)
      {
        int32_t hxc_tmp_load_result_n124 = hxc_language;
        int32_t hxc_tmp_conditional_result_n89 = 0;
        if (hxc_tmp_load_result_n124 == 0)
        {
          hxc_tmp_conditional_result_n89 = 1;
        }
        else
        {
          hxc_tmp_conditional_result_n89 = 0;
        }
        hxc_language = hxc_tmp_conditional_result_n89;
      }
      bool hxc_tmp_native_call_result_n126 = IsKeyPressed((int32_t)265);
      bool hxc_tmp_short_circuit_result_n90 = hxc_tmp_native_call_result_n126;
      if (!hxc_tmp_native_call_result_n126)
      {
        bool hxc_tmp_native_call_result_n127 = IsKeyPressed((int32_t)264);
        hxc_tmp_short_circuit_result_n90 = hxc_tmp_native_call_result_n127;
      }
      if (hxc_tmp_short_circuit_result_n90)
      {
        int32_t hxc_tmp_load_result_n129 = hxc_selectedMode;
        int32_t hxc_tmp_conditional_result_n91 = 0;
        if (hxc_tmp_load_result_n129 == 0)
        {
          hxc_tmp_conditional_result_n91 = 1;
        }
        else
        {
          hxc_tmp_conditional_result_n91 = 0;
        }
        hxc_selectedMode = hxc_tmp_conditional_result_n91;
      }
      struct Vector2 hxc_tmp_native_call_result_n131 = GetMousePosition();
      struct Vector2 hxc_menuMouse = hxc_tmp_native_call_result_n131;
      struct Vector2 hxc_tmp_load_result_n132 = hxc_menuMouse;
      struct Vector2 hxc_tmp_load_result_n134 = hxc_menuMouse;
      int32_t hxc_tmp_native_call_result_n136 = GetScreenWidth();
      int32_t hxc_tmp_native_call_result_n137 = GetScreenHeight();
      int32_t hxc_tmp_call_result_n138 = hxc_caxecraft_app_TitleMenu_selectionAt((double)hxc_tmp_load_result_n132.x, (double)hxc_tmp_load_result_n134.y, (int32_t)hxc_tmp_native_call_result_n136, (int32_t)hxc_tmp_native_call_result_n137);
      int32_t hxc_hovered = hxc_tmp_call_result_n138;
      if (hxc_hovered == 0)
      {
        hxc_selectedMode = 0;
      }
      if (hxc_hovered == 1)
      {
        hxc_selectedMode = 1;
      }
      int32_t hxc_tmp_load_result_n141 = hxc_hovered;
      bool hxc_tmp_short_circuit_result_n95 = hxc_tmp_load_result_n141 >= 0;
      if (hxc_tmp_load_result_n141 >= 0)
      {
        bool hxc_tmp_native_call_result_n142 = IsMouseButtonPressed((int32_t)0);
        hxc_tmp_short_circuit_result_n95 = hxc_tmp_native_call_result_n142;
      }
      bool hxc_clickedChoice = hxc_tmp_short_circuit_result_n95;
      bool hxc_tmp_load_result_n144 = hxc_clickedChoice;
      bool hxc_tmp_short_circuit_result_n96 = hxc_tmp_load_result_n144;
      if (!hxc_tmp_load_result_n144)
      {
        bool hxc_tmp_native_call_result_n145 = IsKeyPressed((int32_t)257);
        hxc_tmp_short_circuit_result_n96 = hxc_tmp_native_call_result_n145;
      }
      if (hxc_tmp_short_circuit_result_n96)
      {
        hxc_onTitle = false;
        hxc_paused = false;
        hxc_captured = true;
        hxc_recapturedThisFrame = true;
        DisableCursor();
      }
    }
    bool hxc_tmp_load_result_n147 = hxc_onTitle;
    bool hxc_tmp_short_circuit_result_n97 = !hxc_tmp_load_result_n147;
    if (!hxc_tmp_load_result_n147)
    {
      hxc_tmp_short_circuit_result_n97 = !hxc_focused;
    }
    bool hxc_tmp_short_circuit_load_result_n149 = hxc_tmp_short_circuit_result_n97;
    bool hxc_tmp_short_circuit_result_n98 = hxc_tmp_short_circuit_load_result_n149;
    if (hxc_tmp_short_circuit_load_result_n149)
    {
      hxc_tmp_short_circuit_result_n98 = hxc_captured;
    }
    if (hxc_tmp_short_circuit_result_n98)
    {
      hxc_paused = true;
      hxc_captured = false;
      hxc_jumpQueued = false;
      EnableCursor();
    }
    bool hxc_tmp_load_result_n152 = hxc_onTitle;
    bool hxc_tmp_short_circuit_result_n99 = !hxc_tmp_load_result_n152;
    if (!hxc_tmp_load_result_n152)
    {
      hxc_tmp_short_circuit_result_n99 = hxc_focused;
    }
    bool hxc_tmp_short_circuit_load_result_n154 = hxc_tmp_short_circuit_result_n99;
    bool hxc_tmp_short_circuit_result_n100 = hxc_tmp_short_circuit_load_result_n154;
    if (hxc_tmp_short_circuit_load_result_n154)
    {
      hxc_tmp_short_circuit_result_n100 = hxc_pausePressed_h78002f09b1a9;
    }
    if (hxc_tmp_short_circuit_result_n100)
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
    bool hxc_tmp_load_result_n161 = hxc_onTitle;
    bool hxc_tmp_short_circuit_result_n101 = !hxc_tmp_load_result_n161;
    if (!hxc_tmp_load_result_n161)
    {
      hxc_tmp_short_circuit_result_n101 = hxc_focused;
    }
    bool hxc_tmp_short_circuit_load_result_n163 = hxc_tmp_short_circuit_result_n101;
    bool hxc_tmp_short_circuit_result_n102 = hxc_tmp_short_circuit_load_result_n163;
    if (hxc_tmp_short_circuit_load_result_n163)
    {
      hxc_tmp_short_circuit_result_n102 = hxc_paused;
    }
    bool hxc_tmp_short_circuit_load_result_n165 = hxc_tmp_short_circuit_result_n102;
    bool hxc_tmp_short_circuit_result_n103 = hxc_tmp_short_circuit_load_result_n165;
    if (hxc_tmp_short_circuit_load_result_n165)
    {
      hxc_tmp_short_circuit_result_n103 = hxc_capturePressed;
    }
    if (hxc_tmp_short_circuit_result_n103)
    {
      hxc_paused = false;
      hxc_captured = true;
      hxc_recapturedThisFrame = true;
      DisableCursor();
    }
    if (hxc_captured)
    {
      double hxc_yawDelta = hxc_lookYaw_hc9c3902167f1;
      if (hxc_yawDelta > 0.25)
      {
        hxc_yawDelta = 0.25;
      }
      if (hxc_yawDelta < -0.25)
      {
        hxc_yawDelta = -0.25;
      }
      double hxc_tmp_load_result_n172 = hxc_lookX;
      double hxc_tmp_load_result_n173 = hxc_yawDelta;
      double hxc_candidateX = hxc_tmp_load_result_n172 + hxc_tmp_load_result_n173 * hxc_lookZ;
      double hxc_tmp_load_result_n175 = hxc_lookZ;
      double hxc_tmp_load_result_n176 = hxc_yawDelta;
      double hxc_candidateZ = hxc_tmp_load_result_n175 - hxc_tmp_load_result_n176 * hxc_lookX;
      double hxc_tmp_load_result_n178 = hxc_candidateX;
      double hxc_tmp_load_result_n179 = hxc_candidateX;
      double hxc_tmp_load_result_n180 = hxc_candidateZ;
      double hxc_lengthSquared = hxc_tmp_load_result_n178 * hxc_tmp_load_result_n179 + hxc_tmp_load_result_n180 * hxc_candidateZ;
      double hxc_normalize = 1.5 - 0.5 * hxc_lengthSquared;
      double hxc_tmp_load_result_n183 = hxc_candidateX;
      hxc_lookX = hxc_tmp_load_result_n183 * hxc_normalize;
      double hxc_tmp_load_result_n185 = hxc_candidateZ;
      hxc_lookZ = hxc_tmp_load_result_n185 * hxc_normalize;
      double hxc_tmp_compound_load_result_n187 = hxc_lookY;
      hxc_lookY = hxc_tmp_compound_load_result_n187 + hxc_lookPitch_h17c6c0519fbe;
      if (hxc_lookY > 0.90)
      {
        hxc_lookY = 0.90;
      }
      if (hxc_lookY < -0.90)
      {
        hxc_lookY = -0.90;
      }
      if (hxc_jumpPressed_h6a6f2e35b6f8)
      {
        hxc_jumpQueued = true;
      }
    }
    float hxc_tmp_native_call_result_n192 = GetFrameTime();
    double hxc_frameSeconds = (double)hxc_tmp_native_call_result_n192;
    if (hxc_frameSeconds > 0.25)
    {
      hxc_frameSeconds = 0.25;
    }
    if (!hxc_paused)
    {
      double hxc_tmp_compound_load_result_n195 = hxc_accumulator;
      hxc_accumulator = hxc_tmp_compound_load_result_n195 + hxc_frameSeconds;
    }
    while (1)
    {
      bool hxc_tmp_load_result_n197 = hxc_paused;
      bool hxc_tmp_short_circuit_result_n110 = !hxc_tmp_load_result_n197;
      if (!hxc_tmp_load_result_n197)
      {
        hxc_tmp_short_circuit_result_n110 = hxc_accumulator >= 0.05;
      }
      if (!hxc_tmp_short_circuit_result_n110)
      {
        break;
      }
      double hxc_tmp_load_result_n200 = hxc_moveForward;
      double hxc_tmp_load_result_n201 = hxc_lookX;
      double hxc_tmp_load_result_n202 = hxc_moveRight;
      double hxc_moveX = hxc_tmp_load_result_n200 * hxc_tmp_load_result_n201 - hxc_tmp_load_result_n202 * hxc_lookZ;
      double hxc_tmp_load_result_n204 = hxc_moveForward;
      double hxc_tmp_load_result_n205 = hxc_lookZ;
      double hxc_tmp_load_result_n206 = hxc_moveRight;
      double hxc_moveZ = hxc_tmp_load_result_n204 * hxc_tmp_load_result_n205 + hxc_tmp_load_result_n206 * hxc_lookX;
      double hxc_tmp_load_result_n208 = hxc_moveForward;
      bool hxc_tmp_short_circuit_result_n113 = hxc_tmp_load_result_n208 != 0.0;
      if (hxc_tmp_load_result_n208 != 0.0)
      {
        hxc_tmp_short_circuit_result_n113 = hxc_moveRight != 0.0;
      }
      if (hxc_tmp_short_circuit_result_n113)
      {
        hxc_moveX = hxc_moveX * 0.7071067811865476;
        hxc_moveZ = hxc_moveZ * 0.7071067811865476;
      }
      if (hxc_vitals.hxc_health > 0)
      {
        struct hxc_caxecraft_domain_PlayerState hxc_tmp_load_result_n214 = hxc_player;
        double hxc_tmp_load_result_n215 = hxc_moveX;
        double hxc_tmp_load_result_n216 = hxc_moveZ;
        struct hxc_caxecraft_domain_StepInput hxc_tmp_call_result_n218 = hxc_caxecraft_domain_PlayerPhysics_input(hxc_tmp_load_result_n215, hxc_tmp_load_result_n216, hxc_jumpQueued);
        struct hxc_caxecraft_domain_PlayerState hxc_tmp_call_result_n219 = hxc_caxecraft_domain_PlayerPhysics_step(hxc_cells, hxc_tmp_length_n1, hxc_tmp_load_result_n214, hxc_tmp_call_result_n218);
        hxc_player = hxc_tmp_call_result_n219;
      }
      if (hxc_selectedMode == 1)
      {
        if (hxc_vitals.hxc_health > 0)
        {
          struct hxc_caxecraft_gameplay_MosslingState hxc_tmp_load_result_n222 = hxc_mossling;
          double hxc_tmp_record_field_load_result_n223 = hxc_player.hxc_x;
          struct hxc_caxecraft_gameplay_MosslingState hxc_tmp_call_result_n225 = hxc_caxecraft_gameplay_Mossling_step(hxc_cells, hxc_tmp_length_n1, hxc_tmp_load_result_n222, hxc_tmp_record_field_load_result_n223, hxc_player.hxc_z);
          hxc_mossling = hxc_tmp_call_result_n225;
          struct hxc_caxecraft_gameplay_PlayerVitalsState hxc_tmp_load_result_n226 = hxc_vitals;
          double hxc_tmp_record_field_load_result_n227 = hxc_player.hxc_x;
          double hxc_tmp_record_field_load_result_n228 = hxc_player.hxc_z;
          double hxc_tmp_record_field_load_result_n229 = hxc_mossling.hxc_x;
          double hxc_tmp_record_field_load_result_n230 = hxc_mossling.hxc_z;
          struct hxc_caxecraft_gameplay_PlayerVitalsState hxc_tmp_call_result_n232 = hxc_caxecraft_gameplay_PlayerVitals_step(hxc_tmp_load_result_n226, hxc_tmp_record_field_load_result_n227, hxc_tmp_record_field_load_result_n228, hxc_tmp_record_field_load_result_n229, hxc_tmp_record_field_load_result_n230, hxc_mossling.hxc_health > 0);
          hxc_vitals = hxc_tmp_call_result_n232;
        }
      }
      hxc_jumpQueued = false;
      hxc_accumulator = hxc_accumulator - 0.05;
      hxc_updateCount = hxc_i32_add_wrapping(hxc_updateCount, 1);
    }
    double hxc_eyeX = hxc_player.hxc_x;
    double hxc_eyeY = hxc_player.hxc_y + 1.62;
    double hxc_eyeZ = hxc_player.hxc_z;
    double hxc_tmp_load_result_n238 = hxc_eyeX;
    double hxc_tmp_load_result_n239 = hxc_eyeY;
    double hxc_tmp_load_result_n240 = hxc_eyeZ;
    double hxc_tmp_load_result_n241 = hxc_lookX;
    double hxc_tmp_load_result_n242 = hxc_lookY;
    struct hxc_caxecraft_domain_RaycastHit hxc_tmp_call_result_n244 = hxc_caxecraft_domain_VoxelRaycast_trace(hxc_cells, hxc_tmp_length_n1, hxc_tmp_load_result_n238, hxc_tmp_load_result_n239, hxc_tmp_load_result_n240, hxc_tmp_load_result_n241, hxc_tmp_load_result_n242, hxc_lookZ, 7.0);
    struct hxc_caxecraft_domain_RaycastHit hxc_hit = hxc_tmp_call_result_n244;
    bool hxc_tmp_load_result_n245 = hxc_captured;
    bool hxc_tmp_short_circuit_result_n118 = hxc_tmp_load_result_n245;
    if (hxc_tmp_load_result_n245)
    {
      hxc_tmp_short_circuit_result_n118 = !hxc_recapturedThisFrame;
    }
    bool hxc_tmp_short_circuit_load_result_n247 = hxc_tmp_short_circuit_result_n118;
    bool hxc_tmp_short_circuit_result_n119 = hxc_tmp_short_circuit_load_result_n247;
    if (hxc_tmp_short_circuit_load_result_n247)
    {
      hxc_tmp_short_circuit_result_n119 = hxc_primaryPressed_h9f9c138d17c0;
    }
    if (hxc_tmp_short_circuit_result_n119)
    {
      if (hxc_vitals.hxc_health > 0)
      {
        if (hxc_selectedMode == 1)
        {
          bool hxc_tmp_call_result_n254 = hxc_caxecraft_gameplay_Inventory_selectedIs(hxc_inventory, hxc_caxecraft_gameplay_ItemKind_CopperSword);
          if (hxc_tmp_call_result_n254)
          {
            if (hxc_inventory.hxc_sword > 0)
            {
              struct hxc_caxecraft_gameplay_MosslingState hxc_tmp_load_result_n256 = hxc_mossling;
              double hxc_tmp_record_field_load_result_n257 = hxc_player.hxc_x;
              double hxc_tmp_record_field_load_result_n258 = hxc_player.hxc_z;
              double hxc_tmp_load_result_n259 = hxc_lookX;
              bool hxc_tmp_call_result_n261 = hxc_caxecraft_gameplay_Mossling_canStrike(hxc_tmp_load_result_n256, hxc_tmp_record_field_load_result_n257, hxc_tmp_record_field_load_result_n258, hxc_tmp_load_result_n259, hxc_lookZ);
              if (hxc_tmp_call_result_n261)
              {
                struct hxc_caxecraft_gameplay_MosslingState hxc_tmp_call_result_n263 = hxc_caxecraft_gameplay_Mossling_strike(hxc_mossling);
                hxc_mossling = hxc_tmp_call_result_n263;
                hxc_strikeHitFrames = 16;
                if (hxc_mossling.hxc_health <= 0)
                {
                  struct hxc_caxecraft_gameplay_BerryDropState hxc_tmp_call_result_n266 = hxc_caxecraft_gameplay_BerryDrop_fromDefeatedMossling(hxc_mossling);
                  hxc_berryDrop = hxc_tmp_call_result_n266;
                  hxc_enemyDefeatedFrames = 120;
                }
              }
            }
          }
          else
          {
            if (hxc_hit.hxc_hit)
            {
              int32_t hxc_tmp_record_field_load_result_n268 = hxc_hit.hxc_cellX;
              int32_t hxc_tmp_record_field_load_result_n269 = hxc_hit.hxc_cellY;
              struct hxc_caxecraft_domain_BlockCoord hxc_tmp_call_result_n271 = hxc_caxecraft_domain_World_coord(hxc_tmp_record_field_load_result_n268, hxc_tmp_record_field_load_result_n269, hxc_hit.hxc_cellZ);
              enum hxc_caxecraft_domain_BlockKind hxc_tmp_call_result_n272 = hxc_caxecraft_domain_World_query(hxc_cells, hxc_tmp_length_n1, hxc_tmp_call_result_n271);
              enum hxc_caxecraft_domain_BlockKind hxc_removedKind = hxc_tmp_call_result_n272;
              int32_t hxc_tmp_record_field_load_result_n273 = hxc_hit.hxc_cellX;
              int32_t hxc_tmp_record_field_load_result_n274 = hxc_hit.hxc_cellY;
              struct hxc_caxecraft_domain_BlockCoord hxc_tmp_call_result_n276 = hxc_caxecraft_domain_World_coord(hxc_tmp_record_field_load_result_n273, hxc_tmp_record_field_load_result_n274, hxc_hit.hxc_cellZ);
              bool hxc_tmp_call_result_n277 = hxc_caxecraft_domain_World_remove(hxc_cells, hxc_tmp_length_n1, hxc_tmp_call_result_n276);
              if (hxc_tmp_call_result_n277)
              {
                struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_load_result_n278 = hxc_inventory;
                struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_call_result_n280 = hxc_caxecraft_gameplay_Inventory_collectBlock(hxc_tmp_load_result_n278, hxc_removedKind);
                hxc_inventory = hxc_tmp_call_result_n280;
              }
            }
          }
        }
        else
        {
          if (hxc_hit.hxc_hit)
          {
            int32_t hxc_tmp_record_field_load_result_n282 = hxc_hit.hxc_cellX;
            int32_t hxc_tmp_record_field_load_result_n283 = hxc_hit.hxc_cellY;
            struct hxc_caxecraft_domain_BlockCoord hxc_tmp_call_result_n285 = hxc_caxecraft_domain_World_coord(hxc_tmp_record_field_load_result_n282, hxc_tmp_record_field_load_result_n283, hxc_hit.hxc_cellZ);
            hxc_caxecraft_domain_World_remove(hxc_cells, hxc_tmp_length_n1, hxc_tmp_call_result_n285);
          }
        }
      }
    }
    bool hxc_tmp_load_result_n286 = hxc_captured;
    bool hxc_tmp_short_circuit_result_n121 = hxc_tmp_load_result_n286;
    if (hxc_tmp_load_result_n286)
    {
      hxc_tmp_short_circuit_result_n121 = hxc_secondaryPressed_h400b391dc5af;
    }
    if (hxc_tmp_short_circuit_result_n121)
    {
      if (hxc_vitals.hxc_health > 0)
      {
        struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_load_result_n290 = hxc_inventory;
        int32_t hxc_tmp_call_result_n292 = hxc_caxecraft_gameplay_Recovery_decide(hxc_tmp_load_result_n290, hxc_vitals);
        int32_t hxc_recoveryDecision = hxc_tmp_call_result_n292;
        if (hxc_recoveryDecision != 0)
        {
          hxc_recoveryFeedback = hxc_recoveryDecision;
          hxc_recoveryFeedbackFrames = 90;
          int32_t hxc_tmp_load_result_n295 = hxc_recoveryDecision;
          struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_call_result_n297 = hxc_caxecraft_gameplay_Recovery_applyInventory(hxc_tmp_load_result_n295, hxc_inventory);
          hxc_inventory = hxc_tmp_call_result_n297;
          int32_t hxc_tmp_load_result_n298 = hxc_recoveryDecision;
          struct hxc_caxecraft_gameplay_PlayerVitalsState hxc_tmp_call_result_n300 = hxc_caxecraft_gameplay_Recovery_applyVitals(hxc_tmp_load_result_n298, hxc_vitals);
          hxc_vitals = hxc_tmp_call_result_n300;
        }
        else
        {
          if (hxc_hit.hxc_hit)
          {
            int32_t hxc_tmp_record_field_load_result_n302 = hxc_hit.hxc_previousX;
            int32_t hxc_tmp_record_field_load_result_n303 = hxc_hit.hxc_previousY;
            struct hxc_caxecraft_domain_BlockCoord hxc_tmp_call_result_n305 = hxc_caxecraft_domain_World_coord(hxc_tmp_record_field_load_result_n302, hxc_tmp_record_field_load_result_n303, hxc_hit.hxc_previousZ);
            struct hxc_caxecraft_domain_BlockCoord hxc_placement = hxc_tmp_call_result_n305;
            enum hxc_caxecraft_domain_BlockKind hxc_tmp_call_result_n307 = hxc_caxecraft_gameplay_Inventory_selectedBlock(hxc_inventory);
            enum hxc_caxecraft_domain_BlockKind hxc_selectedBlock = hxc_tmp_call_result_n307;
            struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_load_result_n308 = hxc_inventory;
            int32_t hxc_tmp_call_result_n310 = hxc_caxecraft_gameplay_Inventory_countAt(hxc_tmp_load_result_n308, hxc_inventory.hxc_selected);
            bool hxc_hasItem = hxc_tmp_call_result_n310 > 0;
            bool hxc_tmp_load_result_n311 = hxc_hasItem;
            bool hxc_tmp_short_circuit_result_n126 = !hxc_tmp_load_result_n311;
            if (!!hxc_tmp_load_result_n311)
            {
              bool hxc_tmp_call_result_n313 = hxc_caxecraft_domain_World_isPlaceable(hxc_selectedBlock);
              hxc_tmp_short_circuit_result_n126 = !hxc_tmp_call_result_n313;
            }
            bool hxc_tmp_short_circuit_load_result_n314 = hxc_tmp_short_circuit_result_n126;
            bool hxc_tmp_short_circuit_result_n127 = hxc_tmp_short_circuit_load_result_n314;
            if (!hxc_tmp_short_circuit_load_result_n314)
            {
              struct hxc_caxecraft_domain_PlayerState hxc_tmp_load_result_n315 = hxc_player;
              bool hxc_tmp_call_result_n317 = hxc_caxecraft_domain_PlayerPhysics_canPlaceAt(hxc_tmp_load_result_n315, hxc_placement);
              hxc_tmp_short_circuit_result_n127 = !hxc_tmp_call_result_n317;
            }
            bool hxc_tmp_short_circuit_load_result_n318 = hxc_tmp_short_circuit_result_n127;
            bool hxc_tmp_short_circuit_result_n128 = hxc_tmp_short_circuit_load_result_n318;
            if (!hxc_tmp_short_circuit_load_result_n318)
            {
              struct hxc_caxecraft_domain_BlockCoord hxc_tmp_load_result_n319 = hxc_placement;
              bool hxc_tmp_call_result_n321 = hxc_caxecraft_domain_World_place(hxc_cells, hxc_tmp_length_n1, hxc_tmp_load_result_n319, hxc_selectedBlock);
              hxc_tmp_short_circuit_result_n128 = !hxc_tmp_call_result_n321;
            }
            if (hxc_tmp_short_circuit_result_n128)
            {
              hxc_placementBlockedFrames = 60;
            }
            else
            {
              if (hxc_selectedMode == 1)
              {
                struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_call_result_n325 = hxc_caxecraft_gameplay_Inventory_consumeSelected(hxc_inventory);
                hxc_inventory = hxc_tmp_call_result_n325;
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
      struct hxc_caxecraft_gameplay_BerryDropState hxc_tmp_load_result_n329 = hxc_berryDrop;
      double hxc_tmp_record_field_load_result_n330 = hxc_player.hxc_x;
      double hxc_tmp_record_field_load_result_n331 = hxc_player.hxc_y;
      bool hxc_tmp_call_result_n333 = hxc_caxecraft_gameplay_BerryDrop_isInRange(hxc_tmp_load_result_n329, hxc_tmp_record_field_load_result_n330, hxc_tmp_record_field_load_result_n331, hxc_player.hxc_z);
      if (hxc_tmp_call_result_n333)
      {
        struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_load_result_n334 = hxc_inventory;
        struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_call_result_n337 = hxc_caxecraft_gameplay_Inventory_collectItem(hxc_tmp_load_result_n334, hxc_caxecraft_gameplay_ItemKind_Berries, hxc_berryDrop.hxc_amount);
        hxc_inventory = hxc_tmp_call_result_n337;
        double hxc_tmp_record_field_load_result_n338 = hxc_berryDrop.hxc_x;
        double hxc_tmp_record_field_load_result_n339 = hxc_berryDrop.hxc_y;
        hxc_berryDrop = (struct hxc_caxecraft_gameplay_BerryDropState){ .hxc_active = false, .hxc_amount = 0, .hxc_x = hxc_tmp_record_field_load_result_n338, .hxc_y = hxc_tmp_record_field_load_result_n339, .hxc_z = hxc_berryDrop.hxc_z };
        hxc_pickupFrames = 90;
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
    if (hxc_pickupFrames > 0)
    {
      hxc_pickupFrames = hxc_i32_subtract_wrapping(hxc_pickupFrames, 1);
    }
    if (hxc_recoveryFeedbackFrames > 0)
    {
      hxc_recoveryFeedbackFrames = hxc_i32_subtract_wrapping(hxc_recoveryFeedbackFrames, 1);
    }
    double hxc_tmp_load_result_n350 = hxc_eyeX;
    double hxc_tmp_load_result_n351 = hxc_eyeY;
    double hxc_tmp_load_result_n352 = hxc_eyeZ;
    double hxc_tmp_load_result_n354 = hxc_eyeX;
    double hxc_tmp_load_result_n355 = hxc_lookX;
    double hxc_tmp_load_result_n356 = hxc_eyeY;
    double hxc_tmp_load_result_n357 = hxc_lookY;
    double hxc_tmp_load_result_n358 = hxc_eyeZ;
    struct Camera3D hxc_camera = (struct Camera3D){ .position = (struct Vector3){ .x = (float)hxc_tmp_load_result_n350, .y = (float)hxc_tmp_load_result_n351, .z = (float)hxc_tmp_load_result_n352 }, .target = (struct Vector3){ .x = (float)(hxc_tmp_load_result_n354 + hxc_tmp_load_result_n355), .y = (float)(hxc_tmp_load_result_n356 + hxc_tmp_load_result_n357), .z = (float)(hxc_tmp_load_result_n358 + hxc_lookZ) }, .up = (struct Vector3){ .x = (float)0.0, .y = (float)1.0, .z = (float)0.0 }, .fovy = (float)70.0, .projection = 0 };
    BeginDrawing();
    if (hxc_onTitle)
    {
      int32_t hxc_tmp_native_call_result_n364 = GetScreenWidth();
      int32_t hxc_width_h1d135a3d7233 = (int32_t)hxc_tmp_native_call_result_n364;
      int32_t hxc_tmp_native_call_result_n365 = GetScreenHeight();
      int32_t hxc_height_h3e43d3a00fe8 = (int32_t)hxc_tmp_native_call_result_n365;
      if (hxc_titleTextureReady)
      {
        struct Color hxc_this1_hcdc4da10d3a1 = (struct Color){ .r = 255, .g = 255, .b = 255, .a = 255 };
        struct Texture hxc_tmp_load_result_n368 = hxc_titleTexture;
        double hxc_imageAspect = hxc_f64_divide_zero_safe((double)(int32_t)hxc_tmp_load_result_n368.width, (double)(int32_t)hxc_titleTexture.height);
        int32_t hxc_tmp_load_result_n372 = hxc_width_h1d135a3d7233;
        double hxc_screenAspect = hxc_f64_divide_zero_safe((double)hxc_tmp_load_result_n372, (double)hxc_height_h3e43d3a00fe8);
        double hxc_sourceX = 0.0;
        double hxc_sourceY = 0.0;
        double hxc_sourceWidth = (double)(int32_t)hxc_titleTexture.width;
        double hxc_sourceHeight = (double)(int32_t)hxc_titleTexture.height;
        double hxc_tmp_load_result_n378 = hxc_screenAspect;
        if (hxc_tmp_load_result_n378 > hxc_imageAspect)
        {
          struct Texture hxc_tmp_load_result_n380 = hxc_titleTexture;
          hxc_sourceHeight = hxc_f64_divide_zero_safe((double)(int32_t)hxc_tmp_load_result_n380.width, hxc_screenAspect);
          struct Texture hxc_tmp_load_result_n383 = hxc_titleTexture;
          hxc_sourceY = ((double)(int32_t)hxc_tmp_load_result_n383.height - hxc_sourceHeight) * 0.5;
        }
        else
        {
          struct Texture hxc_tmp_load_result_n386 = hxc_titleTexture;
          hxc_sourceWidth = (double)(int32_t)hxc_tmp_load_result_n386.height * hxc_screenAspect;
          struct Texture hxc_tmp_load_result_n389 = hxc_titleTexture;
          hxc_sourceX = ((double)(int32_t)hxc_tmp_load_result_n389.width - hxc_sourceWidth) * 0.5;
        }
        struct Texture hxc_tmp_load_result_n392 = hxc_titleTexture;
        double hxc_tmp_load_result_n393 = hxc_sourceX;
        double hxc_tmp_load_result_n394 = hxc_sourceY;
        double hxc_tmp_load_result_n395 = hxc_sourceWidth;
        double hxc_tmp_load_result_n396 = hxc_sourceHeight;
        int32_t hxc_tmp_load_result_n398 = hxc_width_h1d135a3d7233;
        int32_t hxc_tmp_load_result_n399 = hxc_height_h3e43d3a00fe8;
        DrawTexturePro(hxc_tmp_load_result_n392, (struct Rectangle){ .x = (float)hxc_tmp_load_result_n393, .y = (float)hxc_tmp_load_result_n394, .width = (float)hxc_tmp_load_result_n395, .height = (float)hxc_tmp_load_result_n396 }, (struct Rectangle){ .x = (float)0.0, .y = (float)0.0, .width = (float)((double)hxc_tmp_load_result_n398 + 0.0), .height = (float)((double)hxc_tmp_load_result_n399 + 0.0) }, (struct Vector2){ .x = (float)0.0, .y = (float)0.0 }, (float)0.0, hxc_this1_hcdc4da10d3a1);
      }
      else
      {
        ClearBackground((struct Color){ .r = 126, .g = 190, .b = 201, .a = 255 });
      }
      struct Color hxc_this1_h54b78bbe5a3d = (struct Color){ .r = 4, .g = 14, .b = 23, .a = 132 };
      int32_t hxc_tmp_load_result_n405 = hxc_width_h1d135a3d7233;
      int32_t hxc_tmp_load_result_n406 = hxc_height_h3e43d3a00fe8;
      DrawRectangle((int32_t)0, (int32_t)0, (int32_t)hxc_tmp_load_result_n405, (int32_t)hxc_tmp_load_result_n406, hxc_this1_h54b78bbe5a3d);
      if (hxc_wordmarkTextureReady)
      {
        int32_t hxc_centerX = hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_width_h1d135a3d7233, (double)2));
        struct Color hxc_this1_he1b0a11dbe20 = (struct Color){ .r = 255, .g = 255, .b = 255, .a = 255 };
        double hxc_scale = hxc_f64_divide_zero_safe((double)560, (double)(int32_t)hxc_wordmarkTexture.width);
        double hxc_heightScale = hxc_f64_divide_zero_safe((double)132, (double)(int32_t)hxc_wordmarkTexture.height);
        double hxc_tmp_load_result_n415 = hxc_heightScale;
        if (hxc_tmp_load_result_n415 < hxc_scale)
        {
          hxc_scale = hxc_heightScale;
        }
        struct Texture hxc_tmp_load_result_n418 = hxc_wordmarkTexture;
        double hxc_width_hcf44dec82322 = (double)(int32_t)hxc_tmp_load_result_n418.width * hxc_scale;
        struct Texture hxc_tmp_load_result_n421 = hxc_wordmarkTexture;
        double hxc_height_hc9f56c1b504a = (double)(int32_t)hxc_tmp_load_result_n421.height * hxc_scale;
        double hxc_width1 = (double)(int32_t)hxc_wordmarkTexture.width + 0.0;
        double hxc_height1 = (double)(int32_t)hxc_wordmarkTexture.height + 0.0;
        struct Texture hxc_tmp_load_result_n428 = hxc_wordmarkTexture;
        double hxc_tmp_load_result_n429 = hxc_width1;
        double hxc_tmp_load_result_n430 = hxc_height1;
        int32_t hxc_tmp_load_result_n432 = hxc_centerX;
        double hxc_tmp_load_result_n433 = hxc_width_hcf44dec82322;
        double hxc_tmp_load_result_n434 = hxc_width_hcf44dec82322;
        double hxc_tmp_load_result_n435 = hxc_height_hc9f56c1b504a;
        DrawTexturePro(hxc_tmp_load_result_n428, (struct Rectangle){ .x = (float)0.0, .y = (float)0.0, .width = (float)hxc_tmp_load_result_n429, .height = (float)hxc_tmp_load_result_n430 }, (struct Rectangle){ .x = (float)((double)hxc_tmp_load_result_n432 - hxc_tmp_load_result_n433 * 0.5), .y = (float)28.0, .width = (float)hxc_tmp_load_result_n434, .height = (float)hxc_tmp_load_result_n435 }, (struct Vector2){ .x = (float)0.0, .y = (float)0.0 }, (float)0.0, hxc_this1_he1b0a11dbe20);
      }
      else
      {
        int32_t hxc_x_h28b75e73ec4f = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_width_h1d135a3d7233, (double)2)), 92);
        struct Color hxc_this1_h76ebd527ca60 = (struct Color){ .r = 242, .g = 249, .b = 245, .a = 255 };
        int32_t hxc_tmp_load_result_n441 = hxc_x_h28b75e73ec4f;
        DrawText("CAXECRAFT", (int32_t)hxc_tmp_load_result_n441, (int32_t)52, (int32_t)34, hxc_this1_h76ebd527ca60);
      }
      int32_t hxc_firstTop = hxc_i32_subtract_wrapping(hxc_height_h3e43d3a00fe8, 225);
      if (hxc_language == 1)
      {
        int32_t hxc_left_h079519d17091 = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_width_h1d135a3d7233, (double)2)), 180);
        if (hxc_selectedMode == 0)
        {
          struct Color hxc_this1_h84b4d4ef8ed6 = (struct Color){ .r = 16, .g = 88, .b = 102, .a = 232 };
          int32_t hxc_tmp_load_result_n448 = hxc_left_h079519d17091;
          int32_t hxc_tmp_load_result_n449 = hxc_firstTop;
          DrawRectangle((int32_t)hxc_tmp_load_result_n448, (int32_t)hxc_tmp_load_result_n449, (int32_t)360, (int32_t)54, hxc_this1_h84b4d4ef8ed6);
          struct Color hxc_this1_h002fc1f66805 = (struct Color){ .r = 255, .g = 190, .b = 55, .a = 255 };
          int32_t hxc_tmp_load_result_n452 = hxc_left_h079519d17091;
          int32_t hxc_tmp_load_result_n453 = hxc_firstTop;
          DrawRectangleLines((int32_t)hxc_tmp_load_result_n452, (int32_t)hxc_tmp_load_result_n453, (int32_t)360, (int32_t)54, hxc_this1_h002fc1f66805);
        }
        else
        {
          struct Color hxc_this1_h9f9078c8c23e = (struct Color){ .r = 6, .g = 26, .b = 36, .a = 218 };
          int32_t hxc_tmp_load_result_n456 = hxc_left_h079519d17091;
          int32_t hxc_tmp_load_result_n457 = hxc_firstTop;
          DrawRectangle((int32_t)hxc_tmp_load_result_n456, (int32_t)hxc_tmp_load_result_n457, (int32_t)360, (int32_t)54, hxc_this1_h9f9078c8c23e);
          struct Color hxc_this1_hbe58d2c8ebc0 = (struct Color){ .r = 92, .g = 194, .b = 188, .a = 255 };
          int32_t hxc_tmp_load_result_n460 = hxc_left_h079519d17091;
          int32_t hxc_tmp_load_result_n461 = hxc_firstTop;
          DrawRectangleLines((int32_t)hxc_tmp_load_result_n460, (int32_t)hxc_tmp_load_result_n461, (int32_t)360, (int32_t)54, hxc_this1_hbe58d2c8ebc0);
        }
        struct Color hxc_this1_hd91a64420a0e = (struct Color){ .r = 242, .g = 249, .b = 245, .a = 255 };
        int32_t hxc_tmp_load_result_n464 = hxc_left_h079519d17091;
        int32_t hxc_tmp_load_result_n465 = hxc_firstTop;
        DrawText("MODO CREATIVO", (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n464, 28), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n465, 16), (int32_t)21, hxc_this1_hd91a64420a0e);
        int32_t hxc_top_ha7de3c5f1ceb = hxc_i32_add_wrapping(hxc_i32_add_wrapping(hxc_firstTop, 54), 14);
        int32_t hxc_left_h8b8ad2b1a707 = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_width_h1d135a3d7233, (double)2)), 180);
        if (hxc_selectedMode == 1)
        {
          struct Color hxc_this1_h07780785cb2c = (struct Color){ .r = 16, .g = 88, .b = 102, .a = 232 };
          int32_t hxc_tmp_load_result_n471 = hxc_left_h8b8ad2b1a707;
          int32_t hxc_tmp_load_result_n472 = hxc_top_ha7de3c5f1ceb;
          DrawRectangle((int32_t)hxc_tmp_load_result_n471, (int32_t)hxc_tmp_load_result_n472, (int32_t)360, (int32_t)54, hxc_this1_h07780785cb2c);
          struct Color hxc_this1_h17bbeb228c01 = (struct Color){ .r = 255, .g = 190, .b = 55, .a = 255 };
          int32_t hxc_tmp_load_result_n475 = hxc_left_h8b8ad2b1a707;
          int32_t hxc_tmp_load_result_n476 = hxc_top_ha7de3c5f1ceb;
          DrawRectangleLines((int32_t)hxc_tmp_load_result_n475, (int32_t)hxc_tmp_load_result_n476, (int32_t)360, (int32_t)54, hxc_this1_h17bbeb228c01);
        }
        else
        {
          struct Color hxc_this1_h6e409dfdcc67 = (struct Color){ .r = 6, .g = 26, .b = 36, .a = 218 };
          int32_t hxc_tmp_load_result_n479 = hxc_left_h8b8ad2b1a707;
          int32_t hxc_tmp_load_result_n480 = hxc_top_ha7de3c5f1ceb;
          DrawRectangle((int32_t)hxc_tmp_load_result_n479, (int32_t)hxc_tmp_load_result_n480, (int32_t)360, (int32_t)54, hxc_this1_h6e409dfdcc67);
          struct Color hxc_this1_h497240767d75 = (struct Color){ .r = 92, .g = 194, .b = 188, .a = 255 };
          int32_t hxc_tmp_load_result_n483 = hxc_left_h8b8ad2b1a707;
          int32_t hxc_tmp_load_result_n484 = hxc_top_ha7de3c5f1ceb;
          DrawRectangleLines((int32_t)hxc_tmp_load_result_n483, (int32_t)hxc_tmp_load_result_n484, (int32_t)360, (int32_t)54, hxc_this1_h497240767d75);
        }
        struct Color hxc_this1_hf8da672797dd = (struct Color){ .r = 242, .g = 249, .b = 245, .a = 255 };
        int32_t hxc_tmp_load_result_n487 = hxc_left_h8b8ad2b1a707;
        int32_t hxc_tmp_load_result_n488 = hxc_top_ha7de3c5f1ceb;
        DrawText("AVENTURA", (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n487, 28), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n488, 16), (int32_t)21, hxc_this1_hf8da672797dd);
        int32_t hxc_x_h499f44f6f2a4 = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_width_h1d135a3d7233, (double)2)), 270);
        struct Color hxc_this1_he648b63556d3 = (struct Color){ .r = 229, .g = 241, .b = 235, .a = 255 };
        int32_t hxc_tmp_load_result_n492 = hxc_x_h499f44f6f2a4;
        int32_t hxc_tmp_load_result_n493 = hxc_height_h3e43d3a00fe8;
        DrawText("FLECHAS / RATON PARA ELEGIR   ENTER PARA JUGAR   L: ENGLISH", (int32_t)hxc_tmp_load_result_n492, (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n493, 58), (int32_t)16, hxc_this1_he648b63556d3);
      }
      else
      {
        int32_t hxc_left_h92384728143e = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_width_h1d135a3d7233, (double)2)), 180);
        if (hxc_selectedMode == 0)
        {
          struct Color hxc_this1_h3c3e9e95b1c8 = (struct Color){ .r = 16, .g = 88, .b = 102, .a = 232 };
          int32_t hxc_tmp_load_result_n498 = hxc_left_h92384728143e;
          int32_t hxc_tmp_load_result_n499 = hxc_firstTop;
          DrawRectangle((int32_t)hxc_tmp_load_result_n498, (int32_t)hxc_tmp_load_result_n499, (int32_t)360, (int32_t)54, hxc_this1_h3c3e9e95b1c8);
          struct Color hxc_this1_hf72bbf808c96 = (struct Color){ .r = 255, .g = 190, .b = 55, .a = 255 };
          int32_t hxc_tmp_load_result_n502 = hxc_left_h92384728143e;
          int32_t hxc_tmp_load_result_n503 = hxc_firstTop;
          DrawRectangleLines((int32_t)hxc_tmp_load_result_n502, (int32_t)hxc_tmp_load_result_n503, (int32_t)360, (int32_t)54, hxc_this1_hf72bbf808c96);
        }
        else
        {
          struct Color hxc_this1_h45e7cd6f2d61 = (struct Color){ .r = 6, .g = 26, .b = 36, .a = 218 };
          int32_t hxc_tmp_load_result_n506 = hxc_left_h92384728143e;
          int32_t hxc_tmp_load_result_n507 = hxc_firstTop;
          DrawRectangle((int32_t)hxc_tmp_load_result_n506, (int32_t)hxc_tmp_load_result_n507, (int32_t)360, (int32_t)54, hxc_this1_h45e7cd6f2d61);
          struct Color hxc_this1_h5ab6e6cdd588 = (struct Color){ .r = 92, .g = 194, .b = 188, .a = 255 };
          int32_t hxc_tmp_load_result_n510 = hxc_left_h92384728143e;
          int32_t hxc_tmp_load_result_n511 = hxc_firstTop;
          DrawRectangleLines((int32_t)hxc_tmp_load_result_n510, (int32_t)hxc_tmp_load_result_n511, (int32_t)360, (int32_t)54, hxc_this1_h5ab6e6cdd588);
        }
        struct Color hxc_this1_h922c566f2fec = (struct Color){ .r = 242, .g = 249, .b = 245, .a = 255 };
        int32_t hxc_tmp_load_result_n514 = hxc_left_h92384728143e;
        int32_t hxc_tmp_load_result_n515 = hxc_firstTop;
        DrawText("CREATIVE MODE", (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n514, 28), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n515, 16), (int32_t)21, hxc_this1_h922c566f2fec);
        int32_t hxc_top_h6b7a7ae36359 = hxc_i32_add_wrapping(hxc_i32_add_wrapping(hxc_firstTop, 54), 14);
        int32_t hxc_left_h09350339afd4 = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_width_h1d135a3d7233, (double)2)), 180);
        if (hxc_selectedMode == 1)
        {
          struct Color hxc_this1_hc18468f5e813 = (struct Color){ .r = 16, .g = 88, .b = 102, .a = 232 };
          int32_t hxc_tmp_load_result_n521 = hxc_left_h09350339afd4;
          int32_t hxc_tmp_load_result_n522 = hxc_top_h6b7a7ae36359;
          DrawRectangle((int32_t)hxc_tmp_load_result_n521, (int32_t)hxc_tmp_load_result_n522, (int32_t)360, (int32_t)54, hxc_this1_hc18468f5e813);
          struct Color hxc_this1_hfe494e7f13a4 = (struct Color){ .r = 255, .g = 190, .b = 55, .a = 255 };
          int32_t hxc_tmp_load_result_n525 = hxc_left_h09350339afd4;
          int32_t hxc_tmp_load_result_n526 = hxc_top_h6b7a7ae36359;
          DrawRectangleLines((int32_t)hxc_tmp_load_result_n525, (int32_t)hxc_tmp_load_result_n526, (int32_t)360, (int32_t)54, hxc_this1_hfe494e7f13a4);
        }
        else
        {
          struct Color hxc_this1_h00d59f9d7c58 = (struct Color){ .r = 6, .g = 26, .b = 36, .a = 218 };
          int32_t hxc_tmp_load_result_n529 = hxc_left_h09350339afd4;
          int32_t hxc_tmp_load_result_n530 = hxc_top_h6b7a7ae36359;
          DrawRectangle((int32_t)hxc_tmp_load_result_n529, (int32_t)hxc_tmp_load_result_n530, (int32_t)360, (int32_t)54, hxc_this1_h00d59f9d7c58);
          struct Color hxc_this1_h0ee879d5f97c = (struct Color){ .r = 92, .g = 194, .b = 188, .a = 255 };
          int32_t hxc_tmp_load_result_n533 = hxc_left_h09350339afd4;
          int32_t hxc_tmp_load_result_n534 = hxc_top_h6b7a7ae36359;
          DrawRectangleLines((int32_t)hxc_tmp_load_result_n533, (int32_t)hxc_tmp_load_result_n534, (int32_t)360, (int32_t)54, hxc_this1_h0ee879d5f97c);
        }
        struct Color hxc_this1_h8d4a171cde40 = (struct Color){ .r = 242, .g = 249, .b = 245, .a = 255 };
        int32_t hxc_tmp_load_result_n537 = hxc_left_h09350339afd4;
        int32_t hxc_tmp_load_result_n538 = hxc_top_h6b7a7ae36359;
        DrawText("ADVENTURE", (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n537, 28), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n538, 16), (int32_t)21, hxc_this1_h8d4a171cde40);
        int32_t hxc_x_h4b5cecc841b9 = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_width_h1d135a3d7233, (double)2)), 270);
        struct Color hxc_this1_hdf3d0dd71701 = (struct Color){ .r = 229, .g = 241, .b = 235, .a = 255 };
        int32_t hxc_tmp_load_result_n542 = hxc_x_h4b5cecc841b9;
        int32_t hxc_tmp_load_result_n543 = hxc_height_h3e43d3a00fe8;
        DrawText("ARROWS / MOUSE TO CHOOSE   ENTER TO PLAY   L: ESPANOL", (int32_t)hxc_tmp_load_result_n542, (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n543, 58), (int32_t)16, hxc_this1_hdf3d0dd71701);
      }
      if (hxc_selectedMode == 1)
      {
        if (hxc_language == 1)
        {
          int32_t hxc_x_hba336934bf60 = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_width_h1d135a3d7233, (double)2)), 210);
          struct Color hxc_this1_hc32b20afb966 = (struct Color){ .r = 255, .g = 205, .b = 91, .a = 255 };
          int32_t hxc_tmp_load_result_n549 = hxc_x_hba336934bf60;
          int32_t hxc_tmp_load_result_n550 = hxc_firstTop;
          DrawText("SALVA A CEESH. ENCUENTRA A IVVY. DETEN A BROWSER.", (int32_t)hxc_tmp_load_result_n549, (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n550, 36), (int32_t)17, hxc_this1_hc32b20afb966);
        }
        else
        {
          int32_t hxc_x_hc162a90cba74 = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_width_h1d135a3d7233, (double)2)), 210);
          struct Color hxc_this1_hb9cd5bc2da99 = (struct Color){ .r = 255, .g = 205, .b = 91, .a = 255 };
          int32_t hxc_tmp_load_result_n554 = hxc_x_hc162a90cba74;
          int32_t hxc_tmp_load_result_n555 = hxc_firstTop;
          DrawText("SAVE CEESH. FIND IVVY. STOP BROWSER.", (int32_t)hxc_tmp_load_result_n554, (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n555, 36), (int32_t)17, hxc_this1_hb9cd5bc2da99);
        }
      }
    }
    else
    {
      ClearBackground((struct Color){ .r = 126, .g = 190, .b = 201, .a = 255 });
      BeginMode3D(hxc_camera);
      double hxc_tmp_record_field_load_result_n559 = hxc_player.hxc_x;
      struct hxc_caxecraft_app_Main_RenderCounters hxc_tmp_call_result_n561 = hxc_caxecraft_app_Main_drawWorld(hxc_cells, hxc_tmp_length_n1, hxc_tmp_record_field_load_result_n559, hxc_player.hxc_z);
      struct hxc_caxecraft_app_Main_RenderCounters hxc_renderCounters = hxc_tmp_call_result_n561;
      struct hxc_caxecraft_gameplay_GuideState hxc_tmp_load_result_n562 = hxc_guide;
      struct hxc_caxecraft_gameplay_MosslingState hxc_tmp_load_result_n563 = hxc_mossling;
      hxc_caxecraft_app_Main_drawActors(hxc_tmp_load_result_n562, hxc_tmp_load_result_n563, hxc_berryDrop);
      if (hxc_hit.hxc_hit)
      {
        double hxc_y = (double)hxc_hit.hxc_cellY + 0.5;
        double hxc_z = (double)hxc_hit.hxc_cellZ + 0.5;
        int32_t hxc_tmp_record_field_load_result_n568 = hxc_hit.hxc_cellX;
        double hxc_tmp_load_result_n569 = hxc_y;
        DrawCubeWires((struct Vector3){ .x = (float)((double)hxc_tmp_record_field_load_result_n568 + 0.5), .y = (float)hxc_tmp_load_result_n569, .z = (float)hxc_z }, (float)1.04, (float)1.04, (float)1.04, (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 });
      }
      EndMode3D();
      int32_t hxc_tmp_record_field_load_result_n573 = hxc_renderCounters.hxc_visible;
      int32_t hxc_tmp_record_field_load_result_n574 = hxc_renderCounters.hxc_drawCalls;
      int32_t hxc_tmp_load_result_n575 = hxc_frameCount;
      int32_t hxc_tmp_load_result_n576 = hxc_updateCount;
      bool hxc_tmp_load_result_n577 = hxc_paused;
      bool hxc_tmp_load_result_n578 = hxc_captured;
      int32_t hxc_tmp_load_result_n579 = hxc_placementBlockedFrames;
      struct hxc_caxecraft_domain_RaycastHit hxc_tmp_load_result_n580 = hxc_hit;
      double hxc_tmp_record_field_load_result_n581 = hxc_player.hxc_x;
      double hxc_tmp_record_field_load_result_n582 = hxc_player.hxc_z;
      int32_t hxc_tmp_load_result_n583 = hxc_selectedMode;
      int32_t hxc_tmp_load_result_n584 = hxc_language;
      struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_load_result_n585 = hxc_inventory;
      struct hxc_caxecraft_gameplay_GuideState hxc_tmp_load_result_n586 = hxc_guide;
      struct hxc_caxecraft_gameplay_MosslingState hxc_tmp_load_result_n587 = hxc_mossling;
      struct hxc_caxecraft_gameplay_PlayerVitalsState hxc_tmp_load_result_n588 = hxc_vitals;
      int32_t hxc_tmp_load_result_n589 = hxc_strikeHitFrames;
      int32_t hxc_tmp_load_result_n590 = hxc_enemyDefeatedFrames;
      int32_t hxc_tmp_load_result_n591 = hxc_pickupFrames;
      int32_t hxc_tmp_load_result_n592 = hxc_recoveryFeedback;
      int32_t hxc_tmp_load_result_n593 = hxc_recoveryFeedbackFrames;
      struct Texture hxc_tmp_load_result_n594 = hxc_hudTexture;
      bool hxc_tmp_load_result_n595 = hxc_hudTextureReady;
      struct Texture hxc_tmp_load_result_n596 = hxc_itemTexture;
      hxc_caxecraft_app_Main_drawHud(hxc_tmp_record_field_load_result_n573, hxc_tmp_record_field_load_result_n574, hxc_tmp_load_result_n575, hxc_tmp_load_result_n576, hxc_tmp_load_result_n577, hxc_tmp_load_result_n578, hxc_tmp_load_result_n579 > 0, hxc_tmp_load_result_n580, hxc_tmp_record_field_load_result_n581, hxc_tmp_record_field_load_result_n582, hxc_tmp_load_result_n583, hxc_tmp_load_result_n584, hxc_tmp_load_result_n585, hxc_tmp_load_result_n586, hxc_tmp_load_result_n587, hxc_tmp_load_result_n588, hxc_tmp_load_result_n589 > 0, hxc_tmp_load_result_n590 > 0, hxc_tmp_load_result_n591 > 0, hxc_tmp_load_result_n592, hxc_tmp_load_result_n593 > 0, hxc_tmp_load_result_n594, hxc_tmp_load_result_n595, hxc_tmp_load_result_n596, hxc_itemTextureReady);
    }
    EndDrawing();
    hxc_frameCount = hxc_i32_add_wrapping(hxc_frameCount, 1);
  }
  EnableCursor();
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
