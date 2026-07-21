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

void hxc_caxecraft_app_Main_drawHud(int32_t hxc_visible, int32_t hxc_drawCalls, int32_t hxc_frames, int32_t hxc_updates, bool hxc_paused, bool hxc_captured, bool hxc_placementBlocked, struct hxc_caxecraft_domain_RaycastHit hxc_hit, double hxc_playerX, double hxc_playerZ, int32_t hxc_mode, int32_t hxc_language, struct hxc_caxecraft_gameplay_InventoryState hxc_inventory, struct hxc_caxecraft_gameplay_GuideState hxc_guide, struct hxc_caxecraft_gameplay_MosslingState hxc_mossling, struct hxc_caxecraft_gameplay_PlayerVitalsState hxc_vitals, bool hxc_strikeHit, bool hxc_enemyDefeated, bool hxc_pickedUp, struct Texture hxc_hudTexture, bool hxc_hudTextureReady, struct Texture hxc_itemTexture, bool hxc_itemTextureReady)
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
  struct Color hxc_this1_h32050098c249 = (struct Color){ .r = 15, .g = 27, .b = 39, .a = 220 };
  DrawRectangle((int32_t)18, (int32_t)18, (int32_t)460, (int32_t)108, hxc_this1_h32050098c249);
  struct Color hxc_this1_he27fefe95eaf = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
  DrawRectangleLines((int32_t)18, (int32_t)18, (int32_t)460, (int32_t)108, hxc_this1_he27fefe95eaf);
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
      struct Color hxc_this1_h6ea3c8179546 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
      DrawText("AVENTURA: PROLOGO EN CONSTRUCCION", (int32_t)32, (int32_t)110, (int32_t)14, hxc_this1_h6ea3c8179546);
    }
    else
    {
      struct Color hxc_this1_hd37d68c43b00 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
      DrawText("ADVENTURE: PROLOGUE IN PROGRESS", (int32_t)32, (int32_t)110, (int32_t)14, hxc_this1_hd37d68c43b00);
    }
  }
  bool hxc_tmp_call_result_n49 = hxc_caxecraft_gameplay_GuideNpc_isInRange(hxc_guide, hxc_playerX, hxc_playerZ);
  if (hxc_tmp_call_result_n49)
  {
    struct Color hxc_this1_h374d17360ee4 = (struct Color){ .r = 15, .g = 27, .b = 39, .a = 220 };
    int32_t hxc_tmp_load_result_n51 = hxc_centerX;
    int32_t hxc_tmp_load_result_n52 = hxc_centerY;
    DrawRectangle((int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n51, 260), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n52, 54), (int32_t)520, (int32_t)60, hxc_this1_h374d17360ee4);
    int32_t hxc_code_hf67506cdc7cf = hxc_guide.hxc_phaseCode;
    int32_t hxc_tmp_load_result_n55 = hxc_code_hf67506cdc7cf;
    int32_t hxc_tmp_conditional_result_n34 = 0;
    if (hxc_tmp_load_result_n55 == 1)
    {
      hxc_tmp_conditional_result_n34 = 1;
    }
    else
    {
      int32_t hxc_tmp_load_result_n56 = hxc_code_hf67506cdc7cf;
      int32_t hxc_tmp_conditional_result_n35 = 0;
      if (hxc_tmp_load_result_n56 == 2)
      {
        hxc_tmp_conditional_result_n35 = 2;
      }
      else
      {
        hxc_tmp_conditional_result_n35 = 0;
      }
      hxc_tmp_conditional_result_n34 = hxc_tmp_conditional_result_n35;
    }
    if (hxc_tmp_conditional_result_n34 == 0)
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
      int32_t hxc_code_h21049e5cb078 = hxc_guide.hxc_phaseCode;
      int32_t hxc_tmp_load_result_n66 = hxc_code_h21049e5cb078;
      int32_t hxc_tmp_conditional_result_n37 = 0;
      if (hxc_tmp_load_result_n66 == 1)
      {
        hxc_tmp_conditional_result_n37 = 1;
      }
      else
      {
        int32_t hxc_tmp_load_result_n67 = hxc_code_h21049e5cb078;
        int32_t hxc_tmp_conditional_result_n38 = 0;
        if (hxc_tmp_load_result_n67 == 2)
        {
          hxc_tmp_conditional_result_n38 = 2;
        }
        else
        {
          hxc_tmp_conditional_result_n38 = 0;
        }
        hxc_tmp_conditional_result_n37 = hxc_tmp_conditional_result_n38;
      }
      if (hxc_tmp_conditional_result_n37 == 1)
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
    int32_t hxc_code_h6ce3d1edb9e0 = hxc_mossling.hxc_modeCode;
    int32_t hxc_tmp_load_result_n84 = hxc_code_h6ce3d1edb9e0;
    int32_t hxc_tmp_conditional_result_n40 = 0;
    if (hxc_tmp_load_result_n84 == 1)
    {
      hxc_tmp_conditional_result_n40 = 1;
    }
    else
    {
      int32_t hxc_tmp_load_result_n85 = hxc_code_h6ce3d1edb9e0;
      int32_t hxc_tmp_conditional_result_n41 = 0;
      if (hxc_tmp_load_result_n85 == 2)
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
      if (hxc_language == 1)
      {
        struct Color hxc_this1_hac7d99ff5a89 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
        int32_t hxc_tmp_load_result_n89 = hxc_width;
        DrawText("MUSGUITO ALERTA", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n89, 180), (int32_t)28, (int32_t)16, hxc_this1_hac7d99ff5a89);
      }
      else
      {
        struct Color hxc_this1_h5d31e73bdb49 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
        int32_t hxc_tmp_load_result_n92 = hxc_width;
        DrawText("MOSSLING ALERT", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n92, 180), (int32_t)28, (int32_t)16, hxc_this1_h5d31e73bdb49);
      }
    }
  }
  if (hxc_strikeHit)
  {
    if (hxc_language == 1)
    {
      struct Color hxc_this1_h3f800ed25b5a = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
      int32_t hxc_tmp_load_result_n95 = hxc_centerX;
      int32_t hxc_tmp_load_result_n96 = hxc_centerY;
      DrawText("GOLPE DE COBRE", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n95, 70), (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n96, 54), (int32_t)18, hxc_this1_h3f800ed25b5a);
    }
    else
    {
      struct Color hxc_this1_h020d92720e66 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
      int32_t hxc_tmp_load_result_n99 = hxc_centerX;
      int32_t hxc_tmp_load_result_n100 = hxc_centerY;
      DrawText("COPPER STRIKE", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n99, 70), (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n100, 54), (int32_t)18, hxc_this1_h020d92720e66);
    }
  }
  if (hxc_enemyDefeated)
  {
    if (hxc_language == 1)
    {
      struct Color hxc_this1_ha7f146f4508f = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
      int32_t hxc_tmp_load_result_n103 = hxc_width;
      DrawText("EL MUSGUITO SOLTO BAYAS", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n103, 275), (int32_t)54, (int32_t)16, hxc_this1_ha7f146f4508f);
    }
    else
    {
      struct Color hxc_this1_h4822667117c8 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
      int32_t hxc_tmp_load_result_n106 = hxc_width;
      DrawText("MOSSLING DROPPED BERRIES", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n106, 270), (int32_t)54, (int32_t)16, hxc_this1_h4822667117c8);
    }
  }
  if (hxc_pickedUp)
  {
    if (hxc_language == 1)
    {
      struct Color hxc_this1_hf44ea768c254 = (struct Color){ .r = 174, .g = 78, .b = 136, .a = 255 };
      int32_t hxc_tmp_load_result_n109 = hxc_centerX;
      int32_t hxc_tmp_load_result_n110 = hxc_centerY;
      DrawText("+2 BAYAS", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n109, 42), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n110, 24), (int32_t)18, hxc_this1_hf44ea768c254);
    }
    else
    {
      struct Color hxc_this1_h7dd34c34b14d = (struct Color){ .r = 174, .g = 78, .b = 136, .a = 255 };
      int32_t hxc_tmp_load_result_n113 = hxc_centerX;
      int32_t hxc_tmp_load_result_n114 = hxc_centerY;
      DrawText("+2 BERRIES", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n113, 48), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n114, 24), (int32_t)18, hxc_this1_h7dd34c34b14d);
    }
  }
  if (hxc_vitals.hxc_safeTicks > 15)
  {
    struct Color hxc_this1_ha1cb09db5615 = (struct Color){ .r = 218, .g = 65, .b = 72, .a = 255 };
    int32_t hxc_tmp_load_result_n118 = hxc_width;
    int32_t hxc_tmp_load_result_n119 = hxc_height;
    DrawRectangleLines((int32_t)4, (int32_t)4, (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n118, 8), (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n119, 8), hxc_this1_ha1cb09db5615);
  }
  if (hxc_vitals.hxc_health <= 0)
  {
    struct Color hxc_this1_hf52665e46f38 = (struct Color){ .r = 15, .g = 27, .b = 39, .a = 220 };
    int32_t hxc_tmp_load_result_n123 = hxc_centerX;
    int32_t hxc_tmp_load_result_n124 = hxc_centerY;
    DrawRectangle((int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n123, 250), (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n124, 74), (int32_t)500, (int32_t)148, hxc_this1_hf52665e46f38);
    struct Color hxc_this1_hf7a3d8dfe303 = (struct Color){ .r = 218, .g = 65, .b = 72, .a = 255 };
    int32_t hxc_tmp_load_result_n127 = hxc_centerX;
    int32_t hxc_tmp_load_result_n128 = hxc_centerY;
    DrawRectangleLines((int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n127, 250), (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n128, 74), (int32_t)500, (int32_t)148, hxc_this1_hf7a3d8dfe303);
    if (hxc_language == 1)
    {
      int32_t hxc_tmp_load_result_n130 = hxc_centerX;
      int32_t hxc_tmp_load_result_n131 = hxc_centerY;
      DrawText("HAXIRIO HA CAIDO", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n130, 112), (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n131, 42), (int32_t)24, hxc_text);
      struct Color hxc_this1_h70634aa469c9 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
      int32_t hxc_tmp_load_result_n134 = hxc_centerX;
      int32_t hxc_tmp_load_result_n135 = hxc_centerY;
      DrawText("E  VOLVER AL PRADO", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n134, 105), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n135, 10), (int32_t)18, hxc_this1_h70634aa469c9);
    }
    else
    {
      int32_t hxc_tmp_load_result_n137 = hxc_centerX;
      int32_t hxc_tmp_load_result_n138 = hxc_centerY;
      DrawText("HAXIRIO HAS FALLEN", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n137, 122), (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n138, 42), (int32_t)24, hxc_text);
      struct Color hxc_this1_h5de87faffce9 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
      int32_t hxc_tmp_load_result_n141 = hxc_centerX;
      int32_t hxc_tmp_load_result_n142 = hxc_centerY;
      DrawText("E  RETURN TO THE MEADOW", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n141, 125), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n142, 10), (int32_t)18, hxc_this1_h5de87faffce9);
    }
  }
  if (hxc_paused)
  {
    struct Color hxc_this1_h758606c2c8f0 = (struct Color){ .r = 15, .g = 27, .b = 39, .a = 220 };
    int32_t hxc_tmp_load_result_n145 = hxc_centerX;
    int32_t hxc_tmp_load_result_n146 = hxc_centerY;
    DrawRectangle((int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n145, 170), (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n146, 48), (int32_t)340, (int32_t)96, hxc_this1_h758606c2c8f0);
    struct Color hxc_this1_h6c434134faa6 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
    int32_t hxc_tmp_load_result_n149 = hxc_centerX;
    int32_t hxc_tmp_load_result_n150 = hxc_centerY;
    DrawRectangleLines((int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n149, 170), (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n150, 48), (int32_t)340, (int32_t)96, hxc_this1_h6c434134faa6);
    int32_t hxc_tmp_load_result_n152 = hxc_centerX;
    int32_t hxc_tmp_load_result_n153 = hxc_centerY;
    DrawText("PAUSED", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n152, 48), (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n153, 30), (int32_t)24, hxc_text);
    int32_t hxc_tmp_load_result_n155 = hxc_centerX;
    int32_t hxc_tmp_load_result_n156 = hxc_centerY;
    DrawText("CLICK TO CAPTURE  //  Q TO QUIT", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n155, 145), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n156, 8), (int32_t)16, hxc_text);
  }
  else
  {
    if (hxc_placementBlocked)
    {
      struct Color hxc_this1_h45d100ccbb4f = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
      int32_t hxc_tmp_load_result_n159 = hxc_centerX;
      int32_t hxc_tmp_load_result_n160 = hxc_centerY;
      DrawText("PLACE BLOCKED: PLAYER OR WORLD EDGE", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n159, 150), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n160, 26), (int32_t)14, hxc_this1_h45d100ccbb4f);
    }
    else
    {
      if (!hxc_captured)
      {
        int32_t hxc_tmp_load_result_n162 = hxc_centerX;
        int32_t hxc_tmp_load_result_n163 = hxc_centerY;
        DrawText("CLICK TO CAPTURE", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n162, 74), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n163, 26), (int32_t)14, hxc_text);
      }
      else
      {
        if (!hxc_hit.hxc_hit)
        {
          int32_t hxc_tmp_load_result_n166 = hxc_centerX;
          int32_t hxc_tmp_load_result_n167 = hxc_centerY;
          DrawText("NO BLOCK IN REACH", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n166, 78), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n167, 26), (int32_t)14, hxc_text);
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
  struct hxc_caxecraft_gameplay_PlayerVitalsState hxc_tmp_call_result_n18 = hxc_caxecraft_gameplay_PlayerVitals_make(6, 0);
  struct hxc_caxecraft_gameplay_PlayerVitalsState hxc_vitals = hxc_tmp_call_result_n18;
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
  while (1)
  {
    bool hxc_tmp_load_result_n20 = hxc_quit;
    bool hxc_tmp_short_circuit_result_n34 = !hxc_tmp_load_result_n20;
    if (!hxc_tmp_load_result_n20)
    {
      bool hxc_tmp_native_call_result_n21 = WindowShouldClose();
      hxc_tmp_short_circuit_result_n34 = !hxc_tmp_native_call_result_n21;
    }
    if (!hxc_tmp_short_circuit_result_n34)
    {
      break;
    }
    bool hxc_recapturedThisFrame = false;
    bool hxc_tmp_native_call_result_n23 = IsWindowFocused();
    bool hxc_focused = hxc_tmp_native_call_result_n23;
    double hxc_forward = 0.0;
    double hxc_right = 0.0;
    bool hxc_tmp_native_call_result_n24 = IsKeyDown((int32_t)87);
    if (hxc_tmp_native_call_result_n24)
    {
      hxc_forward = hxc_forward + 1.0;
    }
    bool hxc_tmp_native_call_result_n26 = IsKeyDown((int32_t)83);
    if (hxc_tmp_native_call_result_n26)
    {
      hxc_forward = hxc_forward - 1.0;
    }
    bool hxc_tmp_native_call_result_n28 = IsKeyDown((int32_t)68);
    if (hxc_tmp_native_call_result_n28)
    {
      hxc_right = hxc_right + 1.0;
    }
    bool hxc_tmp_native_call_result_n30 = IsKeyDown((int32_t)65);
    if (hxc_tmp_native_call_result_n30)
    {
      hxc_right = hxc_right - 1.0;
    }
    double hxc_lookYaw_h51b88c281041 = 0.0;
    double hxc_lookPitch_h3f8b406de94f = 0.0;
    if (hxc_captured)
    {
      struct Vector2 hxc_tmp_native_call_result_n33 = GetMouseDelta();
      struct Vector2 hxc_mouse = hxc_tmp_native_call_result_n33;
      hxc_lookYaw_h51b88c281041 = -(double)hxc_mouse.x * 0.0025;
      hxc_lookPitch_h3f8b406de94f = -(double)hxc_mouse.y * 0.0025;
    }
    bool hxc_tmp_native_call_result_n38 = IsMouseButtonPressed((int32_t)0);
    bool hxc_leftPressed = hxc_tmp_native_call_result_n38;
    int32_t hxc_hotbarSelection_hb710617603e6 = -1;
    bool hxc_tmp_native_call_result_n39 = IsKeyPressed((int32_t)49);
    if (hxc_tmp_native_call_result_n39)
    {
      hxc_hotbarSelection_hb710617603e6 = 0;
    }
    bool hxc_tmp_native_call_result_n40 = IsKeyPressed((int32_t)50);
    if (hxc_tmp_native_call_result_n40)
    {
      hxc_hotbarSelection_hb710617603e6 = 1;
    }
    bool hxc_tmp_native_call_result_n41 = IsKeyPressed((int32_t)51);
    if (hxc_tmp_native_call_result_n41)
    {
      hxc_hotbarSelection_hb710617603e6 = 2;
    }
    bool hxc_tmp_native_call_result_n42 = IsKeyPressed((int32_t)52);
    if (hxc_tmp_native_call_result_n42)
    {
      hxc_hotbarSelection_hb710617603e6 = 3;
    }
    bool hxc_tmp_native_call_result_n43 = IsKeyPressed((int32_t)53);
    if (hxc_tmp_native_call_result_n43)
    {
      hxc_hotbarSelection_hb710617603e6 = 4;
    }
    bool hxc_tmp_native_call_result_n44 = IsKeyPressed((int32_t)54);
    if (hxc_tmp_native_call_result_n44)
    {
      hxc_hotbarSelection_hb710617603e6 = 5;
    }
    bool hxc_tmp_native_call_result_n45 = IsKeyPressed((int32_t)55);
    if (hxc_tmp_native_call_result_n45)
    {
      hxc_hotbarSelection_hb710617603e6 = 6;
    }
    bool hxc_tmp_native_call_result_n46 = IsKeyPressed((int32_t)56);
    if (hxc_tmp_native_call_result_n46)
    {
      hxc_hotbarSelection_hb710617603e6 = 7;
    }
    float hxc_tmp_native_call_result_n47 = GetMouseWheelMove();
    double hxc_wheel = (double)hxc_tmp_native_call_result_n47;
    int32_t hxc_hotbarCycle_h055fcbac1032 = 0;
    if (hxc_wheel > 0.0)
    {
      hxc_hotbarCycle_h055fcbac1032 = -1;
    }
    if (hxc_wheel < 0.0)
    {
      hxc_hotbarCycle_h055fcbac1032 = 1;
    }
    bool hxc_tmp_native_call_result_n50 = IsKeyPressed((int32_t)32);
    bool hxc_jumpPressed_h3ff8ff5a5f61 = hxc_tmp_native_call_result_n50;
    bool hxc_tmp_load_result_n51 = hxc_captured;
    bool hxc_tmp_short_circuit_result_n48 = hxc_tmp_load_result_n51;
    if (hxc_tmp_load_result_n51)
    {
      bool hxc_tmp_native_call_result_n52 = IsMouseButtonPressed((int32_t)1);
      hxc_tmp_short_circuit_result_n48 = hxc_tmp_native_call_result_n52;
    }
    bool hxc_placePressed_hd8bb2a261abd = hxc_tmp_short_circuit_result_n48;
    bool hxc_tmp_load_result_n54 = hxc_captured;
    bool hxc_tmp_short_circuit_result_n50 = hxc_tmp_load_result_n54;
    if (hxc_tmp_load_result_n54)
    {
      bool hxc_tmp_native_call_result_n55 = IsKeyPressed((int32_t)69);
      hxc_tmp_short_circuit_result_n50 = hxc_tmp_native_call_result_n55;
    }
    bool hxc_interactPressed_h23e415747a66 = hxc_tmp_short_circuit_result_n50;
    bool hxc_tmp_native_call_result_n57 = IsKeyPressed((int32_t)256);
    bool hxc_pausePressed_hae4bd589d3ea = hxc_tmp_native_call_result_n57;
    bool hxc_tmp_native_call_result_n58 = IsKeyPressed((int32_t)81);
    bool hxc_quitPressed_hf086f16619d4 = hxc_tmp_native_call_result_n58;
    double hxc_frameInput_moveForward = hxc_forward;
    double hxc_frameInput_moveRight = hxc_right;
    double hxc_frameInput_lookYaw = hxc_lookYaw_h51b88c281041;
    double hxc_frameInput_lookPitch = hxc_lookPitch_h3f8b406de94f;
    bool hxc_frameInput_jumpPressed = hxc_jumpPressed_h3ff8ff5a5f61;
    bool hxc_tmp_load_result_n64 = hxc_captured;
    bool hxc_tmp_short_circuit_result_n59 = hxc_tmp_load_result_n64;
    if (hxc_tmp_load_result_n64)
    {
      hxc_tmp_short_circuit_result_n59 = hxc_leftPressed;
    }
    bool hxc_frameInput_primaryPressed = hxc_tmp_short_circuit_result_n59;
    bool hxc_frameInput_placePressed = hxc_placePressed_hd8bb2a261abd;
    bool hxc_frameInput_interactPressed = hxc_interactPressed_h23e415747a66;
    bool hxc_frameInput_pausePressed = hxc_pausePressed_hae4bd589d3ea;
    bool hxc_tmp_load_result_n70 = hxc_paused;
    bool hxc_tmp_short_circuit_result_n64 = hxc_tmp_load_result_n70;
    if (hxc_tmp_load_result_n70)
    {
      hxc_tmp_short_circuit_result_n64 = hxc_leftPressed;
    }
    bool hxc_frameInput_capturePressed = hxc_tmp_short_circuit_result_n64;
    bool hxc_frameInput_quitPressed = hxc_quitPressed_hf086f16619d4;
    int32_t hxc_frameInput_hotbarSelection = hxc_hotbarSelection_hb710617603e6;
    int32_t hxc_frameInput_hotbarCycle = hxc_hotbarCycle_h055fcbac1032;
    double hxc_moveForward = hxc_frameInput_moveForward;
    double hxc_moveRight = hxc_frameInput_moveRight;
    double hxc_lookYaw_h085ea8e3070e = hxc_frameInput_lookYaw;
    double hxc_lookPitch_h04af9d69d094 = hxc_frameInput_lookPitch;
    bool hxc_jumpPressed_ha5d1d186b77d = hxc_frameInput_jumpPressed;
    bool hxc_primaryPressed = hxc_frameInput_primaryPressed;
    bool hxc_placePressed_h7fc691556e9b = hxc_frameInput_placePressed;
    bool hxc_interactPressed_h2aa8097bf51e = hxc_frameInput_interactPressed;
    bool hxc_pausePressed_h5303a21ea41a = hxc_frameInput_pausePressed;
    bool hxc_capturePressed = hxc_frameInput_capturePressed;
    bool hxc_quitPressed_h9f3f4cda1dec = hxc_frameInput_quitPressed;
    int32_t hxc_hotbarSelection_hed96830dcc02 = hxc_frameInput_hotbarSelection;
    int32_t hxc_hotbarCycle_h42c9ec69e121 = hxc_frameInput_hotbarCycle;
    if (hxc_quitPressed_h9f3f4cda1dec)
    {
      hxc_quit = true;
    }
    if (hxc_hotbarSelection_hed96830dcc02 >= 0)
    {
      struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_load_result_n91 = hxc_inventory;
      struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_call_result_n93 = hxc_caxecraft_gameplay_Inventory_select(hxc_tmp_load_result_n91, hxc_hotbarSelection_hed96830dcc02);
      hxc_inventory = hxc_tmp_call_result_n93;
    }
    if (hxc_hotbarCycle_h42c9ec69e121 != 0)
    {
      struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_load_result_n95 = hxc_inventory;
      struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_call_result_n97 = hxc_caxecraft_gameplay_Inventory_cycle(hxc_tmp_load_result_n95, hxc_hotbarCycle_h42c9ec69e121);
      hxc_inventory = hxc_tmp_call_result_n97;
    }
    bool hxc_tmp_load_result_n98 = hxc_onTitle;
    bool hxc_tmp_short_circuit_result_n81 = !hxc_tmp_load_result_n98;
    if (!hxc_tmp_load_result_n98)
    {
      hxc_tmp_short_circuit_result_n81 = !hxc_paused;
    }
    bool hxc_tmp_short_circuit_load_result_n100 = hxc_tmp_short_circuit_result_n81;
    bool hxc_tmp_short_circuit_result_n82 = hxc_tmp_short_circuit_load_result_n100;
    if (hxc_tmp_short_circuit_load_result_n100)
    {
      hxc_tmp_short_circuit_result_n82 = hxc_interactPressed_h2aa8097bf51e;
    }
    if (hxc_tmp_short_circuit_result_n82)
    {
      if (hxc_vitals.hxc_health <= 0)
      {
        struct hxc_caxecraft_gameplay_PlayerVitalsState hxc_tmp_call_result_n104 = hxc_caxecraft_gameplay_PlayerVitals_make(6, 0);
        hxc_vitals = hxc_tmp_call_result_n104;
        struct hxc_caxecraft_domain_PlayerState hxc_tmp_call_result_n105 = hxc_caxecraft_app_Main_spawnPlayer(hxc_cells, hxc_tmp_length_n1);
        hxc_player = hxc_tmp_call_result_n105;
      }
      else
      {
        struct hxc_caxecraft_gameplay_GuideState hxc_tmp_load_result_n106 = hxc_guide;
        double hxc_tmp_record_field_load_result_n107 = hxc_player.hxc_x;
        bool hxc_tmp_call_result_n109 = hxc_caxecraft_gameplay_GuideNpc_isInRange(hxc_tmp_load_result_n106, hxc_tmp_record_field_load_result_n107, hxc_player.hxc_z);
        if (hxc_tmp_call_result_n109)
        {
          bool hxc_tmp_call_result_n111 = hxc_caxecraft_gameplay_GuideNpc_sharesBerriesOnNextInteraction(hxc_guide);
          bool hxc_sharesBerries = hxc_tmp_call_result_n111;
          struct hxc_caxecraft_gameplay_GuideState hxc_tmp_call_result_n113 = hxc_caxecraft_gameplay_GuideNpc_interact(hxc_guide);
          hxc_guide = hxc_tmp_call_result_n113;
          if (hxc_sharesBerries)
          {
            struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_call_result_n117 = hxc_caxecraft_gameplay_Inventory_collectItem(hxc_inventory, hxc_caxecraft_gameplay_ItemKind_Berries, 2);
            hxc_inventory = hxc_tmp_call_result_n117;
          }
        }
      }
    }
    bool hxc_tmp_load_result_n118 = hxc_onTitle;
    bool hxc_tmp_short_circuit_result_n84 = hxc_tmp_load_result_n118;
    if (hxc_tmp_load_result_n118)
    {
      hxc_tmp_short_circuit_result_n84 = hxc_focused;
    }
    if (hxc_tmp_short_circuit_result_n84)
    {
      bool hxc_tmp_native_call_result_n121 = IsKeyPressed((int32_t)76);
      if (hxc_tmp_native_call_result_n121)
      {
        int32_t hxc_tmp_load_result_n122 = hxc_language;
        int32_t hxc_tmp_conditional_result_n85 = 0;
        if (hxc_tmp_load_result_n122 == 0)
        {
          hxc_tmp_conditional_result_n85 = 1;
        }
        else
        {
          hxc_tmp_conditional_result_n85 = 0;
        }
        hxc_language = hxc_tmp_conditional_result_n85;
      }
      bool hxc_tmp_native_call_result_n124 = IsKeyPressed((int32_t)265);
      bool hxc_tmp_short_circuit_result_n86 = hxc_tmp_native_call_result_n124;
      if (!hxc_tmp_native_call_result_n124)
      {
        bool hxc_tmp_native_call_result_n125 = IsKeyPressed((int32_t)264);
        hxc_tmp_short_circuit_result_n86 = hxc_tmp_native_call_result_n125;
      }
      if (hxc_tmp_short_circuit_result_n86)
      {
        int32_t hxc_tmp_load_result_n127 = hxc_selectedMode;
        int32_t hxc_tmp_conditional_result_n87 = 0;
        if (hxc_tmp_load_result_n127 == 0)
        {
          hxc_tmp_conditional_result_n87 = 1;
        }
        else
        {
          hxc_tmp_conditional_result_n87 = 0;
        }
        hxc_selectedMode = hxc_tmp_conditional_result_n87;
      }
      struct Vector2 hxc_tmp_native_call_result_n129 = GetMousePosition();
      struct Vector2 hxc_menuMouse = hxc_tmp_native_call_result_n129;
      struct Vector2 hxc_tmp_load_result_n130 = hxc_menuMouse;
      struct Vector2 hxc_tmp_load_result_n132 = hxc_menuMouse;
      int32_t hxc_tmp_native_call_result_n134 = GetScreenWidth();
      int32_t hxc_tmp_native_call_result_n135 = GetScreenHeight();
      int32_t hxc_tmp_call_result_n136 = hxc_caxecraft_app_TitleMenu_selectionAt((double)hxc_tmp_load_result_n130.x, (double)hxc_tmp_load_result_n132.y, (int32_t)hxc_tmp_native_call_result_n134, (int32_t)hxc_tmp_native_call_result_n135);
      int32_t hxc_hovered = hxc_tmp_call_result_n136;
      if (hxc_hovered == 0)
      {
        hxc_selectedMode = 0;
      }
      if (hxc_hovered == 1)
      {
        hxc_selectedMode = 1;
      }
      int32_t hxc_tmp_load_result_n139 = hxc_hovered;
      bool hxc_tmp_short_circuit_result_n91 = hxc_tmp_load_result_n139 >= 0;
      if (hxc_tmp_load_result_n139 >= 0)
      {
        bool hxc_tmp_native_call_result_n140 = IsMouseButtonPressed((int32_t)0);
        hxc_tmp_short_circuit_result_n91 = hxc_tmp_native_call_result_n140;
      }
      bool hxc_clickedChoice = hxc_tmp_short_circuit_result_n91;
      bool hxc_tmp_load_result_n142 = hxc_clickedChoice;
      bool hxc_tmp_short_circuit_result_n92 = hxc_tmp_load_result_n142;
      if (!hxc_tmp_load_result_n142)
      {
        bool hxc_tmp_native_call_result_n143 = IsKeyPressed((int32_t)257);
        hxc_tmp_short_circuit_result_n92 = hxc_tmp_native_call_result_n143;
      }
      if (hxc_tmp_short_circuit_result_n92)
      {
        hxc_onTitle = false;
        hxc_paused = false;
        hxc_captured = true;
        hxc_recapturedThisFrame = true;
        DisableCursor();
      }
    }
    bool hxc_tmp_load_result_n145 = hxc_onTitle;
    bool hxc_tmp_short_circuit_result_n93 = !hxc_tmp_load_result_n145;
    if (!hxc_tmp_load_result_n145)
    {
      hxc_tmp_short_circuit_result_n93 = !hxc_focused;
    }
    bool hxc_tmp_short_circuit_load_result_n147 = hxc_tmp_short_circuit_result_n93;
    bool hxc_tmp_short_circuit_result_n94 = hxc_tmp_short_circuit_load_result_n147;
    if (hxc_tmp_short_circuit_load_result_n147)
    {
      hxc_tmp_short_circuit_result_n94 = hxc_captured;
    }
    if (hxc_tmp_short_circuit_result_n94)
    {
      hxc_paused = true;
      hxc_captured = false;
      hxc_jumpQueued = false;
      EnableCursor();
    }
    bool hxc_tmp_load_result_n150 = hxc_onTitle;
    bool hxc_tmp_short_circuit_result_n95 = !hxc_tmp_load_result_n150;
    if (!hxc_tmp_load_result_n150)
    {
      hxc_tmp_short_circuit_result_n95 = hxc_focused;
    }
    bool hxc_tmp_short_circuit_load_result_n152 = hxc_tmp_short_circuit_result_n95;
    bool hxc_tmp_short_circuit_result_n96 = hxc_tmp_short_circuit_load_result_n152;
    if (hxc_tmp_short_circuit_load_result_n152)
    {
      hxc_tmp_short_circuit_result_n96 = hxc_pausePressed_h5303a21ea41a;
    }
    if (hxc_tmp_short_circuit_result_n96)
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
    bool hxc_tmp_load_result_n159 = hxc_onTitle;
    bool hxc_tmp_short_circuit_result_n97 = !hxc_tmp_load_result_n159;
    if (!hxc_tmp_load_result_n159)
    {
      hxc_tmp_short_circuit_result_n97 = hxc_focused;
    }
    bool hxc_tmp_short_circuit_load_result_n161 = hxc_tmp_short_circuit_result_n97;
    bool hxc_tmp_short_circuit_result_n98 = hxc_tmp_short_circuit_load_result_n161;
    if (hxc_tmp_short_circuit_load_result_n161)
    {
      hxc_tmp_short_circuit_result_n98 = hxc_paused;
    }
    bool hxc_tmp_short_circuit_load_result_n163 = hxc_tmp_short_circuit_result_n98;
    bool hxc_tmp_short_circuit_result_n99 = hxc_tmp_short_circuit_load_result_n163;
    if (hxc_tmp_short_circuit_load_result_n163)
    {
      hxc_tmp_short_circuit_result_n99 = hxc_capturePressed;
    }
    if (hxc_tmp_short_circuit_result_n99)
    {
      hxc_paused = false;
      hxc_captured = true;
      hxc_recapturedThisFrame = true;
      DisableCursor();
    }
    if (hxc_captured)
    {
      double hxc_yawDelta = hxc_lookYaw_h085ea8e3070e;
      if (hxc_yawDelta > 0.25)
      {
        hxc_yawDelta = 0.25;
      }
      if (hxc_yawDelta < -0.25)
      {
        hxc_yawDelta = -0.25;
      }
      double hxc_tmp_load_result_n170 = hxc_lookX;
      double hxc_tmp_load_result_n171 = hxc_yawDelta;
      double hxc_candidateX = hxc_tmp_load_result_n170 + hxc_tmp_load_result_n171 * hxc_lookZ;
      double hxc_tmp_load_result_n173 = hxc_lookZ;
      double hxc_tmp_load_result_n174 = hxc_yawDelta;
      double hxc_candidateZ = hxc_tmp_load_result_n173 - hxc_tmp_load_result_n174 * hxc_lookX;
      double hxc_tmp_load_result_n176 = hxc_candidateX;
      double hxc_tmp_load_result_n177 = hxc_candidateX;
      double hxc_tmp_load_result_n178 = hxc_candidateZ;
      double hxc_lengthSquared = hxc_tmp_load_result_n176 * hxc_tmp_load_result_n177 + hxc_tmp_load_result_n178 * hxc_candidateZ;
      double hxc_normalize = 1.5 - 0.5 * hxc_lengthSquared;
      double hxc_tmp_load_result_n181 = hxc_candidateX;
      hxc_lookX = hxc_tmp_load_result_n181 * hxc_normalize;
      double hxc_tmp_load_result_n183 = hxc_candidateZ;
      hxc_lookZ = hxc_tmp_load_result_n183 * hxc_normalize;
      double hxc_tmp_compound_load_result_n185 = hxc_lookY;
      hxc_lookY = hxc_tmp_compound_load_result_n185 + hxc_lookPitch_h04af9d69d094;
      if (hxc_lookY > 0.90)
      {
        hxc_lookY = 0.90;
      }
      if (hxc_lookY < -0.90)
      {
        hxc_lookY = -0.90;
      }
      if (hxc_jumpPressed_ha5d1d186b77d)
      {
        hxc_jumpQueued = true;
      }
    }
    float hxc_tmp_native_call_result_n190 = GetFrameTime();
    double hxc_frameSeconds = (double)hxc_tmp_native_call_result_n190;
    if (hxc_frameSeconds > 0.25)
    {
      hxc_frameSeconds = 0.25;
    }
    if (!hxc_paused)
    {
      double hxc_tmp_compound_load_result_n193 = hxc_accumulator;
      hxc_accumulator = hxc_tmp_compound_load_result_n193 + hxc_frameSeconds;
    }
    while (1)
    {
      bool hxc_tmp_load_result_n195 = hxc_paused;
      bool hxc_tmp_short_circuit_result_n106 = !hxc_tmp_load_result_n195;
      if (!hxc_tmp_load_result_n195)
      {
        hxc_tmp_short_circuit_result_n106 = hxc_accumulator >= 0.05;
      }
      if (!hxc_tmp_short_circuit_result_n106)
      {
        break;
      }
      double hxc_tmp_load_result_n198 = hxc_moveForward;
      double hxc_tmp_load_result_n199 = hxc_lookX;
      double hxc_tmp_load_result_n200 = hxc_moveRight;
      double hxc_moveX = hxc_tmp_load_result_n198 * hxc_tmp_load_result_n199 - hxc_tmp_load_result_n200 * hxc_lookZ;
      double hxc_tmp_load_result_n202 = hxc_moveForward;
      double hxc_tmp_load_result_n203 = hxc_lookZ;
      double hxc_tmp_load_result_n204 = hxc_moveRight;
      double hxc_moveZ = hxc_tmp_load_result_n202 * hxc_tmp_load_result_n203 + hxc_tmp_load_result_n204 * hxc_lookX;
      double hxc_tmp_load_result_n206 = hxc_moveForward;
      bool hxc_tmp_short_circuit_result_n109 = hxc_tmp_load_result_n206 != 0.0;
      if (hxc_tmp_load_result_n206 != 0.0)
      {
        hxc_tmp_short_circuit_result_n109 = hxc_moveRight != 0.0;
      }
      if (hxc_tmp_short_circuit_result_n109)
      {
        hxc_moveX = hxc_moveX * 0.7071067811865476;
        hxc_moveZ = hxc_moveZ * 0.7071067811865476;
      }
      if (hxc_vitals.hxc_health > 0)
      {
        struct hxc_caxecraft_domain_PlayerState hxc_tmp_load_result_n212 = hxc_player;
        double hxc_tmp_load_result_n213 = hxc_moveX;
        double hxc_tmp_load_result_n214 = hxc_moveZ;
        struct hxc_caxecraft_domain_StepInput hxc_tmp_call_result_n216 = hxc_caxecraft_domain_PlayerPhysics_input(hxc_tmp_load_result_n213, hxc_tmp_load_result_n214, hxc_jumpQueued);
        struct hxc_caxecraft_domain_PlayerState hxc_tmp_call_result_n217 = hxc_caxecraft_domain_PlayerPhysics_step(hxc_cells, hxc_tmp_length_n1, hxc_tmp_load_result_n212, hxc_tmp_call_result_n216);
        hxc_player = hxc_tmp_call_result_n217;
      }
      if (hxc_selectedMode == 1)
      {
        if (hxc_vitals.hxc_health > 0)
        {
          struct hxc_caxecraft_gameplay_MosslingState hxc_tmp_load_result_n220 = hxc_mossling;
          double hxc_tmp_record_field_load_result_n221 = hxc_player.hxc_x;
          struct hxc_caxecraft_gameplay_MosslingState hxc_tmp_call_result_n223 = hxc_caxecraft_gameplay_Mossling_step(hxc_cells, hxc_tmp_length_n1, hxc_tmp_load_result_n220, hxc_tmp_record_field_load_result_n221, hxc_player.hxc_z);
          hxc_mossling = hxc_tmp_call_result_n223;
          struct hxc_caxecraft_gameplay_PlayerVitalsState hxc_tmp_load_result_n224 = hxc_vitals;
          double hxc_tmp_record_field_load_result_n225 = hxc_player.hxc_x;
          double hxc_tmp_record_field_load_result_n226 = hxc_player.hxc_z;
          double hxc_tmp_record_field_load_result_n227 = hxc_mossling.hxc_x;
          double hxc_tmp_record_field_load_result_n228 = hxc_mossling.hxc_z;
          struct hxc_caxecraft_gameplay_PlayerVitalsState hxc_tmp_call_result_n230 = hxc_caxecraft_gameplay_PlayerVitals_step(hxc_tmp_load_result_n224, hxc_tmp_record_field_load_result_n225, hxc_tmp_record_field_load_result_n226, hxc_tmp_record_field_load_result_n227, hxc_tmp_record_field_load_result_n228, hxc_mossling.hxc_health > 0);
          hxc_vitals = hxc_tmp_call_result_n230;
        }
      }
      hxc_jumpQueued = false;
      hxc_accumulator = hxc_accumulator - 0.05;
      hxc_updateCount = hxc_i32_add_wrapping(hxc_updateCount, 1);
    }
    double hxc_eyeX = hxc_player.hxc_x;
    double hxc_eyeY = hxc_player.hxc_y + 1.62;
    double hxc_eyeZ = hxc_player.hxc_z;
    double hxc_tmp_load_result_n236 = hxc_eyeX;
    double hxc_tmp_load_result_n237 = hxc_eyeY;
    double hxc_tmp_load_result_n238 = hxc_eyeZ;
    double hxc_tmp_load_result_n239 = hxc_lookX;
    double hxc_tmp_load_result_n240 = hxc_lookY;
    struct hxc_caxecraft_domain_RaycastHit hxc_tmp_call_result_n242 = hxc_caxecraft_domain_VoxelRaycast_trace(hxc_cells, hxc_tmp_length_n1, hxc_tmp_load_result_n236, hxc_tmp_load_result_n237, hxc_tmp_load_result_n238, hxc_tmp_load_result_n239, hxc_tmp_load_result_n240, hxc_lookZ, 7.0);
    struct hxc_caxecraft_domain_RaycastHit hxc_hit = hxc_tmp_call_result_n242;
    bool hxc_tmp_load_result_n243 = hxc_captured;
    bool hxc_tmp_short_circuit_result_n114 = hxc_tmp_load_result_n243;
    if (hxc_tmp_load_result_n243)
    {
      hxc_tmp_short_circuit_result_n114 = !hxc_recapturedThisFrame;
    }
    bool hxc_tmp_short_circuit_load_result_n245 = hxc_tmp_short_circuit_result_n114;
    bool hxc_tmp_short_circuit_result_n115 = hxc_tmp_short_circuit_load_result_n245;
    if (hxc_tmp_short_circuit_load_result_n245)
    {
      hxc_tmp_short_circuit_result_n115 = hxc_primaryPressed;
    }
    if (hxc_tmp_short_circuit_result_n115)
    {
      if (hxc_vitals.hxc_health > 0)
      {
        if (hxc_selectedMode == 1)
        {
          bool hxc_tmp_call_result_n252 = hxc_caxecraft_gameplay_Inventory_selectedIs(hxc_inventory, hxc_caxecraft_gameplay_ItemKind_CopperSword);
          if (hxc_tmp_call_result_n252)
          {
            if (hxc_inventory.hxc_sword > 0)
            {
              struct hxc_caxecraft_gameplay_MosslingState hxc_tmp_load_result_n254 = hxc_mossling;
              double hxc_tmp_record_field_load_result_n255 = hxc_player.hxc_x;
              double hxc_tmp_record_field_load_result_n256 = hxc_player.hxc_z;
              double hxc_tmp_load_result_n257 = hxc_lookX;
              bool hxc_tmp_call_result_n259 = hxc_caxecraft_gameplay_Mossling_canStrike(hxc_tmp_load_result_n254, hxc_tmp_record_field_load_result_n255, hxc_tmp_record_field_load_result_n256, hxc_tmp_load_result_n257, hxc_lookZ);
              if (hxc_tmp_call_result_n259)
              {
                struct hxc_caxecraft_gameplay_MosslingState hxc_tmp_call_result_n261 = hxc_caxecraft_gameplay_Mossling_strike(hxc_mossling);
                hxc_mossling = hxc_tmp_call_result_n261;
                hxc_strikeHitFrames = 16;
                if (hxc_mossling.hxc_health <= 0)
                {
                  struct hxc_caxecraft_gameplay_BerryDropState hxc_tmp_call_result_n264 = hxc_caxecraft_gameplay_BerryDrop_fromDefeatedMossling(hxc_mossling);
                  hxc_berryDrop = hxc_tmp_call_result_n264;
                  hxc_enemyDefeatedFrames = 120;
                }
              }
            }
          }
          else
          {
            if (hxc_hit.hxc_hit)
            {
              int32_t hxc_tmp_record_field_load_result_n266 = hxc_hit.hxc_cellX;
              int32_t hxc_tmp_record_field_load_result_n267 = hxc_hit.hxc_cellY;
              struct hxc_caxecraft_domain_BlockCoord hxc_tmp_call_result_n269 = hxc_caxecraft_domain_World_coord(hxc_tmp_record_field_load_result_n266, hxc_tmp_record_field_load_result_n267, hxc_hit.hxc_cellZ);
              enum hxc_caxecraft_domain_BlockKind hxc_tmp_call_result_n270 = hxc_caxecraft_domain_World_query(hxc_cells, hxc_tmp_length_n1, hxc_tmp_call_result_n269);
              enum hxc_caxecraft_domain_BlockKind hxc_removedKind = hxc_tmp_call_result_n270;
              int32_t hxc_tmp_record_field_load_result_n271 = hxc_hit.hxc_cellX;
              int32_t hxc_tmp_record_field_load_result_n272 = hxc_hit.hxc_cellY;
              struct hxc_caxecraft_domain_BlockCoord hxc_tmp_call_result_n274 = hxc_caxecraft_domain_World_coord(hxc_tmp_record_field_load_result_n271, hxc_tmp_record_field_load_result_n272, hxc_hit.hxc_cellZ);
              bool hxc_tmp_call_result_n275 = hxc_caxecraft_domain_World_remove(hxc_cells, hxc_tmp_length_n1, hxc_tmp_call_result_n274);
              if (hxc_tmp_call_result_n275)
              {
                struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_load_result_n276 = hxc_inventory;
                struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_call_result_n278 = hxc_caxecraft_gameplay_Inventory_collectBlock(hxc_tmp_load_result_n276, hxc_removedKind);
                hxc_inventory = hxc_tmp_call_result_n278;
              }
            }
          }
        }
        else
        {
          if (hxc_hit.hxc_hit)
          {
            int32_t hxc_tmp_record_field_load_result_n280 = hxc_hit.hxc_cellX;
            int32_t hxc_tmp_record_field_load_result_n281 = hxc_hit.hxc_cellY;
            struct hxc_caxecraft_domain_BlockCoord hxc_tmp_call_result_n283 = hxc_caxecraft_domain_World_coord(hxc_tmp_record_field_load_result_n280, hxc_tmp_record_field_load_result_n281, hxc_hit.hxc_cellZ);
            hxc_caxecraft_domain_World_remove(hxc_cells, hxc_tmp_length_n1, hxc_tmp_call_result_n283);
          }
        }
      }
    }
    bool hxc_tmp_load_result_n284 = hxc_captured;
    bool hxc_tmp_short_circuit_result_n117 = hxc_tmp_load_result_n284;
    if (hxc_tmp_load_result_n284)
    {
      hxc_tmp_short_circuit_result_n117 = hxc_hit.hxc_hit;
    }
    bool hxc_tmp_short_circuit_load_result_n286 = hxc_tmp_short_circuit_result_n117;
    bool hxc_tmp_short_circuit_result_n118 = hxc_tmp_short_circuit_load_result_n286;
    if (hxc_tmp_short_circuit_load_result_n286)
    {
      hxc_tmp_short_circuit_result_n118 = hxc_placePressed_h7fc691556e9b;
    }
    if (hxc_tmp_short_circuit_result_n118)
    {
      int32_t hxc_tmp_record_field_load_result_n289 = hxc_hit.hxc_previousX;
      int32_t hxc_tmp_record_field_load_result_n290 = hxc_hit.hxc_previousY;
      struct hxc_caxecraft_domain_BlockCoord hxc_tmp_call_result_n292 = hxc_caxecraft_domain_World_coord(hxc_tmp_record_field_load_result_n289, hxc_tmp_record_field_load_result_n290, hxc_hit.hxc_previousZ);
      struct hxc_caxecraft_domain_BlockCoord hxc_placement = hxc_tmp_call_result_n292;
      enum hxc_caxecraft_domain_BlockKind hxc_tmp_call_result_n294 = hxc_caxecraft_gameplay_Inventory_selectedBlock(hxc_inventory);
      enum hxc_caxecraft_domain_BlockKind hxc_selectedBlock = hxc_tmp_call_result_n294;
      struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_load_result_n295 = hxc_inventory;
      int32_t hxc_tmp_call_result_n297 = hxc_caxecraft_gameplay_Inventory_countAt(hxc_tmp_load_result_n295, hxc_inventory.hxc_selected);
      bool hxc_hasItem = hxc_tmp_call_result_n297 > 0;
      if (hxc_vitals.hxc_health > 0)
      {
        bool hxc_tmp_load_result_n299 = hxc_hasItem;
        bool hxc_tmp_short_circuit_result_n122 = !hxc_tmp_load_result_n299;
        if (!!hxc_tmp_load_result_n299)
        {
          bool hxc_tmp_call_result_n301 = hxc_caxecraft_domain_World_isPlaceable(hxc_selectedBlock);
          hxc_tmp_short_circuit_result_n122 = !hxc_tmp_call_result_n301;
        }
        bool hxc_tmp_short_circuit_load_result_n302 = hxc_tmp_short_circuit_result_n122;
        bool hxc_tmp_short_circuit_result_n123 = hxc_tmp_short_circuit_load_result_n302;
        if (!hxc_tmp_short_circuit_load_result_n302)
        {
          struct hxc_caxecraft_domain_PlayerState hxc_tmp_load_result_n303 = hxc_player;
          bool hxc_tmp_call_result_n305 = hxc_caxecraft_domain_PlayerPhysics_canPlaceAt(hxc_tmp_load_result_n303, hxc_placement);
          hxc_tmp_short_circuit_result_n123 = !hxc_tmp_call_result_n305;
        }
        bool hxc_tmp_short_circuit_load_result_n306 = hxc_tmp_short_circuit_result_n123;
        bool hxc_tmp_short_circuit_result_n124 = hxc_tmp_short_circuit_load_result_n306;
        if (!hxc_tmp_short_circuit_load_result_n306)
        {
          struct hxc_caxecraft_domain_BlockCoord hxc_tmp_load_result_n307 = hxc_placement;
          bool hxc_tmp_call_result_n309 = hxc_caxecraft_domain_World_place(hxc_cells, hxc_tmp_length_n1, hxc_tmp_load_result_n307, hxc_selectedBlock);
          hxc_tmp_short_circuit_result_n124 = !hxc_tmp_call_result_n309;
        }
        if (hxc_tmp_short_circuit_result_n124)
        {
          hxc_placementBlockedFrames = 60;
        }
        else
        {
          if (hxc_selectedMode == 1)
          {
            struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_call_result_n313 = hxc_caxecraft_gameplay_Inventory_consumeSelected(hxc_inventory);
            hxc_inventory = hxc_tmp_call_result_n313;
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
      struct hxc_caxecraft_gameplay_BerryDropState hxc_tmp_load_result_n317 = hxc_berryDrop;
      double hxc_tmp_record_field_load_result_n318 = hxc_player.hxc_x;
      double hxc_tmp_record_field_load_result_n319 = hxc_player.hxc_y;
      bool hxc_tmp_call_result_n321 = hxc_caxecraft_gameplay_BerryDrop_isInRange(hxc_tmp_load_result_n317, hxc_tmp_record_field_load_result_n318, hxc_tmp_record_field_load_result_n319, hxc_player.hxc_z);
      if (hxc_tmp_call_result_n321)
      {
        struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_load_result_n322 = hxc_inventory;
        struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_call_result_n325 = hxc_caxecraft_gameplay_Inventory_collectItem(hxc_tmp_load_result_n322, hxc_caxecraft_gameplay_ItemKind_Berries, hxc_berryDrop.hxc_amount);
        hxc_inventory = hxc_tmp_call_result_n325;
        double hxc_tmp_record_field_load_result_n326 = hxc_berryDrop.hxc_x;
        double hxc_tmp_record_field_load_result_n327 = hxc_berryDrop.hxc_y;
        hxc_berryDrop = (struct hxc_caxecraft_gameplay_BerryDropState){ .hxc_active = false, .hxc_amount = 0, .hxc_x = hxc_tmp_record_field_load_result_n326, .hxc_y = hxc_tmp_record_field_load_result_n327, .hxc_z = hxc_berryDrop.hxc_z };
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
    double hxc_tmp_load_result_n336 = hxc_eyeX;
    double hxc_tmp_load_result_n337 = hxc_eyeY;
    double hxc_tmp_load_result_n338 = hxc_eyeZ;
    double hxc_tmp_load_result_n340 = hxc_eyeX;
    double hxc_tmp_load_result_n341 = hxc_lookX;
    double hxc_tmp_load_result_n342 = hxc_eyeY;
    double hxc_tmp_load_result_n343 = hxc_lookY;
    double hxc_tmp_load_result_n344 = hxc_eyeZ;
    struct Camera3D hxc_camera = (struct Camera3D){ .position = (struct Vector3){ .x = (float)hxc_tmp_load_result_n336, .y = (float)hxc_tmp_load_result_n337, .z = (float)hxc_tmp_load_result_n338 }, .target = (struct Vector3){ .x = (float)(hxc_tmp_load_result_n340 + hxc_tmp_load_result_n341), .y = (float)(hxc_tmp_load_result_n342 + hxc_tmp_load_result_n343), .z = (float)(hxc_tmp_load_result_n344 + hxc_lookZ) }, .up = (struct Vector3){ .x = (float)0.0, .y = (float)1.0, .z = (float)0.0 }, .fovy = (float)70.0, .projection = 0 };
    BeginDrawing();
    if (hxc_onTitle)
    {
      int32_t hxc_tmp_native_call_result_n350 = GetScreenWidth();
      int32_t hxc_width_h4c39cc537e03 = (int32_t)hxc_tmp_native_call_result_n350;
      int32_t hxc_tmp_native_call_result_n351 = GetScreenHeight();
      int32_t hxc_height_h5d3d97c53946 = (int32_t)hxc_tmp_native_call_result_n351;
      if (hxc_titleTextureReady)
      {
        struct Color hxc_this1_hff38116cc26f = (struct Color){ .r = 255, .g = 255, .b = 255, .a = 255 };
        struct Texture hxc_tmp_load_result_n354 = hxc_titleTexture;
        double hxc_imageAspect = hxc_f64_divide_zero_safe((double)(int32_t)hxc_tmp_load_result_n354.width, (double)(int32_t)hxc_titleTexture.height);
        int32_t hxc_tmp_load_result_n358 = hxc_width_h4c39cc537e03;
        double hxc_screenAspect = hxc_f64_divide_zero_safe((double)hxc_tmp_load_result_n358, (double)hxc_height_h5d3d97c53946);
        double hxc_sourceX = 0.0;
        double hxc_sourceY = 0.0;
        double hxc_sourceWidth = (double)(int32_t)hxc_titleTexture.width;
        double hxc_sourceHeight = (double)(int32_t)hxc_titleTexture.height;
        double hxc_tmp_load_result_n364 = hxc_screenAspect;
        if (hxc_tmp_load_result_n364 > hxc_imageAspect)
        {
          struct Texture hxc_tmp_load_result_n366 = hxc_titleTexture;
          hxc_sourceHeight = hxc_f64_divide_zero_safe((double)(int32_t)hxc_tmp_load_result_n366.width, hxc_screenAspect);
          struct Texture hxc_tmp_load_result_n369 = hxc_titleTexture;
          hxc_sourceY = ((double)(int32_t)hxc_tmp_load_result_n369.height - hxc_sourceHeight) * 0.5;
        }
        else
        {
          struct Texture hxc_tmp_load_result_n372 = hxc_titleTexture;
          hxc_sourceWidth = (double)(int32_t)hxc_tmp_load_result_n372.height * hxc_screenAspect;
          struct Texture hxc_tmp_load_result_n375 = hxc_titleTexture;
          hxc_sourceX = ((double)(int32_t)hxc_tmp_load_result_n375.width - hxc_sourceWidth) * 0.5;
        }
        struct Texture hxc_tmp_load_result_n378 = hxc_titleTexture;
        double hxc_tmp_load_result_n379 = hxc_sourceX;
        double hxc_tmp_load_result_n380 = hxc_sourceY;
        double hxc_tmp_load_result_n381 = hxc_sourceWidth;
        double hxc_tmp_load_result_n382 = hxc_sourceHeight;
        int32_t hxc_tmp_load_result_n384 = hxc_width_h4c39cc537e03;
        int32_t hxc_tmp_load_result_n385 = hxc_height_h5d3d97c53946;
        DrawTexturePro(hxc_tmp_load_result_n378, (struct Rectangle){ .x = (float)hxc_tmp_load_result_n379, .y = (float)hxc_tmp_load_result_n380, .width = (float)hxc_tmp_load_result_n381, .height = (float)hxc_tmp_load_result_n382 }, (struct Rectangle){ .x = (float)0.0, .y = (float)0.0, .width = (float)((double)hxc_tmp_load_result_n384 + 0.0), .height = (float)((double)hxc_tmp_load_result_n385 + 0.0) }, (struct Vector2){ .x = (float)0.0, .y = (float)0.0 }, (float)0.0, hxc_this1_hff38116cc26f);
      }
      else
      {
        ClearBackground((struct Color){ .r = 126, .g = 190, .b = 201, .a = 255 });
      }
      struct Color hxc_this1_h15df3d82c263 = (struct Color){ .r = 4, .g = 14, .b = 23, .a = 132 };
      int32_t hxc_tmp_load_result_n391 = hxc_width_h4c39cc537e03;
      int32_t hxc_tmp_load_result_n392 = hxc_height_h5d3d97c53946;
      DrawRectangle((int32_t)0, (int32_t)0, (int32_t)hxc_tmp_load_result_n391, (int32_t)hxc_tmp_load_result_n392, hxc_this1_h15df3d82c263);
      if (hxc_wordmarkTextureReady)
      {
        int32_t hxc_centerX = hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_width_h4c39cc537e03, (double)2));
        struct Color hxc_this1_h58a1e48a2f21 = (struct Color){ .r = 255, .g = 255, .b = 255, .a = 255 };
        double hxc_scale = hxc_f64_divide_zero_safe((double)560, (double)(int32_t)hxc_wordmarkTexture.width);
        double hxc_heightScale = hxc_f64_divide_zero_safe((double)132, (double)(int32_t)hxc_wordmarkTexture.height);
        double hxc_tmp_load_result_n401 = hxc_heightScale;
        if (hxc_tmp_load_result_n401 < hxc_scale)
        {
          hxc_scale = hxc_heightScale;
        }
        struct Texture hxc_tmp_load_result_n404 = hxc_wordmarkTexture;
        double hxc_width_h92fe29ffcd02 = (double)(int32_t)hxc_tmp_load_result_n404.width * hxc_scale;
        struct Texture hxc_tmp_load_result_n407 = hxc_wordmarkTexture;
        double hxc_height_h8f916b833931 = (double)(int32_t)hxc_tmp_load_result_n407.height * hxc_scale;
        double hxc_width1 = (double)(int32_t)hxc_wordmarkTexture.width + 0.0;
        double hxc_height1 = (double)(int32_t)hxc_wordmarkTexture.height + 0.0;
        struct Texture hxc_tmp_load_result_n414 = hxc_wordmarkTexture;
        double hxc_tmp_load_result_n415 = hxc_width1;
        double hxc_tmp_load_result_n416 = hxc_height1;
        int32_t hxc_tmp_load_result_n418 = hxc_centerX;
        double hxc_tmp_load_result_n419 = hxc_width_h92fe29ffcd02;
        double hxc_tmp_load_result_n420 = hxc_width_h92fe29ffcd02;
        double hxc_tmp_load_result_n421 = hxc_height_h8f916b833931;
        DrawTexturePro(hxc_tmp_load_result_n414, (struct Rectangle){ .x = (float)0.0, .y = (float)0.0, .width = (float)hxc_tmp_load_result_n415, .height = (float)hxc_tmp_load_result_n416 }, (struct Rectangle){ .x = (float)((double)hxc_tmp_load_result_n418 - hxc_tmp_load_result_n419 * 0.5), .y = (float)28.0, .width = (float)hxc_tmp_load_result_n420, .height = (float)hxc_tmp_load_result_n421 }, (struct Vector2){ .x = (float)0.0, .y = (float)0.0 }, (float)0.0, hxc_this1_h58a1e48a2f21);
      }
      else
      {
        int32_t hxc_x_hde7b0a698174 = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_width_h4c39cc537e03, (double)2)), 92);
        struct Color hxc_this1_h15ba6270c219 = (struct Color){ .r = 242, .g = 249, .b = 245, .a = 255 };
        int32_t hxc_tmp_load_result_n427 = hxc_x_hde7b0a698174;
        DrawText("CAXECRAFT", (int32_t)hxc_tmp_load_result_n427, (int32_t)52, (int32_t)34, hxc_this1_h15ba6270c219);
      }
      int32_t hxc_firstTop = hxc_i32_subtract_wrapping(hxc_height_h5d3d97c53946, 225);
      if (hxc_language == 1)
      {
        int32_t hxc_left_h4c339000052f = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_width_h4c39cc537e03, (double)2)), 180);
        if (hxc_selectedMode == 0)
        {
          struct Color hxc_this1_h1150cd733bd0 = (struct Color){ .r = 16, .g = 88, .b = 102, .a = 232 };
          int32_t hxc_tmp_load_result_n434 = hxc_left_h4c339000052f;
          int32_t hxc_tmp_load_result_n435 = hxc_firstTop;
          DrawRectangle((int32_t)hxc_tmp_load_result_n434, (int32_t)hxc_tmp_load_result_n435, (int32_t)360, (int32_t)54, hxc_this1_h1150cd733bd0);
          struct Color hxc_this1_h76ebd527ca60 = (struct Color){ .r = 255, .g = 190, .b = 55, .a = 255 };
          int32_t hxc_tmp_load_result_n438 = hxc_left_h4c339000052f;
          int32_t hxc_tmp_load_result_n439 = hxc_firstTop;
          DrawRectangleLines((int32_t)hxc_tmp_load_result_n438, (int32_t)hxc_tmp_load_result_n439, (int32_t)360, (int32_t)54, hxc_this1_h76ebd527ca60);
        }
        else
        {
          struct Color hxc_this1_he5197d02d4da = (struct Color){ .r = 6, .g = 26, .b = 36, .a = 218 };
          int32_t hxc_tmp_load_result_n442 = hxc_left_h4c339000052f;
          int32_t hxc_tmp_load_result_n443 = hxc_firstTop;
          DrawRectangle((int32_t)hxc_tmp_load_result_n442, (int32_t)hxc_tmp_load_result_n443, (int32_t)360, (int32_t)54, hxc_this1_he5197d02d4da);
          struct Color hxc_this1_haed7460f78d4 = (struct Color){ .r = 92, .g = 194, .b = 188, .a = 255 };
          int32_t hxc_tmp_load_result_n446 = hxc_left_h4c339000052f;
          int32_t hxc_tmp_load_result_n447 = hxc_firstTop;
          DrawRectangleLines((int32_t)hxc_tmp_load_result_n446, (int32_t)hxc_tmp_load_result_n447, (int32_t)360, (int32_t)54, hxc_this1_haed7460f78d4);
        }
        struct Color hxc_this1_h84b4d4ef8ed6 = (struct Color){ .r = 242, .g = 249, .b = 245, .a = 255 };
        int32_t hxc_tmp_load_result_n450 = hxc_left_h4c339000052f;
        int32_t hxc_tmp_load_result_n451 = hxc_firstTop;
        DrawText("MODO CREATIVO", (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n450, 28), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n451, 16), (int32_t)21, hxc_this1_h84b4d4ef8ed6);
        int32_t hxc_top_hf39829239f55 = hxc_i32_add_wrapping(hxc_i32_add_wrapping(hxc_firstTop, 54), 14);
        int32_t hxc_left_h37a232294f36 = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_width_h4c39cc537e03, (double)2)), 180);
        if (hxc_selectedMode == 1)
        {
          struct Color hxc_this1_hbe58d2c8ebc0 = (struct Color){ .r = 16, .g = 88, .b = 102, .a = 232 };
          int32_t hxc_tmp_load_result_n457 = hxc_left_h37a232294f36;
          int32_t hxc_tmp_load_result_n458 = hxc_top_hf39829239f55;
          DrawRectangle((int32_t)hxc_tmp_load_result_n457, (int32_t)hxc_tmp_load_result_n458, (int32_t)360, (int32_t)54, hxc_this1_hbe58d2c8ebc0);
          struct Color hxc_this1_hd91a64420a0e = (struct Color){ .r = 255, .g = 190, .b = 55, .a = 255 };
          int32_t hxc_tmp_load_result_n461 = hxc_left_h37a232294f36;
          int32_t hxc_tmp_load_result_n462 = hxc_top_hf39829239f55;
          DrawRectangleLines((int32_t)hxc_tmp_load_result_n461, (int32_t)hxc_tmp_load_result_n462, (int32_t)360, (int32_t)54, hxc_this1_hd91a64420a0e);
        }
        else
        {
          struct Color hxc_this1_h6986c66fd29d = (struct Color){ .r = 6, .g = 26, .b = 36, .a = 218 };
          int32_t hxc_tmp_load_result_n465 = hxc_left_h37a232294f36;
          int32_t hxc_tmp_load_result_n466 = hxc_top_hf39829239f55;
          DrawRectangle((int32_t)hxc_tmp_load_result_n465, (int32_t)hxc_tmp_load_result_n466, (int32_t)360, (int32_t)54, hxc_this1_h6986c66fd29d);
          struct Color hxc_this1_h742952063b58 = (struct Color){ .r = 92, .g = 194, .b = 188, .a = 255 };
          int32_t hxc_tmp_load_result_n469 = hxc_left_h37a232294f36;
          int32_t hxc_tmp_load_result_n470 = hxc_top_hf39829239f55;
          DrawRectangleLines((int32_t)hxc_tmp_load_result_n469, (int32_t)hxc_tmp_load_result_n470, (int32_t)360, (int32_t)54, hxc_this1_h742952063b58);
        }
        struct Color hxc_this1_h07780785cb2c = (struct Color){ .r = 242, .g = 249, .b = 245, .a = 255 };
        int32_t hxc_tmp_load_result_n473 = hxc_left_h37a232294f36;
        int32_t hxc_tmp_load_result_n474 = hxc_top_hf39829239f55;
        DrawText("AVENTURA", (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n473, 28), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n474, 16), (int32_t)21, hxc_this1_h07780785cb2c);
        int32_t hxc_x_hbcd9bcbb8cd5 = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_width_h4c39cc537e03, (double)2)), 270);
        struct Color hxc_this1_h6e409dfdcc67 = (struct Color){ .r = 229, .g = 241, .b = 235, .a = 255 };
        int32_t hxc_tmp_load_result_n478 = hxc_x_hbcd9bcbb8cd5;
        int32_t hxc_tmp_load_result_n479 = hxc_height_h5d3d97c53946;
        DrawText("FLECHAS / RATON PARA ELEGIR   ENTER PARA JUGAR   L: ENGLISH", (int32_t)hxc_tmp_load_result_n478, (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n479, 58), (int32_t)16, hxc_this1_h6e409dfdcc67);
      }
      else
      {
        int32_t hxc_left_h59e66e62cca2 = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_width_h4c39cc537e03, (double)2)), 180);
        if (hxc_selectedMode == 0)
        {
          struct Color hxc_this1_hf8da672797dd = (struct Color){ .r = 16, .g = 88, .b = 102, .a = 232 };
          int32_t hxc_tmp_load_result_n484 = hxc_left_h59e66e62cca2;
          int32_t hxc_tmp_load_result_n485 = hxc_firstTop;
          DrawRectangle((int32_t)hxc_tmp_load_result_n484, (int32_t)hxc_tmp_load_result_n485, (int32_t)360, (int32_t)54, hxc_this1_hf8da672797dd);
          struct Color hxc_this1_h79577861e1b3 = (struct Color){ .r = 255, .g = 190, .b = 55, .a = 255 };
          int32_t hxc_tmp_load_result_n488 = hxc_left_h59e66e62cca2;
          int32_t hxc_tmp_load_result_n489 = hxc_firstTop;
          DrawRectangleLines((int32_t)hxc_tmp_load_result_n488, (int32_t)hxc_tmp_load_result_n489, (int32_t)360, (int32_t)54, hxc_this1_h79577861e1b3);
        }
        else
        {
          struct Color hxc_this1_he648b63556d3 = (struct Color){ .r = 6, .g = 26, .b = 36, .a = 218 };
          int32_t hxc_tmp_load_result_n492 = hxc_left_h59e66e62cca2;
          int32_t hxc_tmp_load_result_n493 = hxc_firstTop;
          DrawRectangle((int32_t)hxc_tmp_load_result_n492, (int32_t)hxc_tmp_load_result_n493, (int32_t)360, (int32_t)54, hxc_this1_he648b63556d3);
          struct Color hxc_this1_hc6235c1aaa0e = (struct Color){ .r = 92, .g = 194, .b = 188, .a = 255 };
          int32_t hxc_tmp_load_result_n496 = hxc_left_h59e66e62cca2;
          int32_t hxc_tmp_load_result_n497 = hxc_firstTop;
          DrawRectangleLines((int32_t)hxc_tmp_load_result_n496, (int32_t)hxc_tmp_load_result_n497, (int32_t)360, (int32_t)54, hxc_this1_hc6235c1aaa0e);
        }
        struct Color hxc_this1_h3c3e9e95b1c8 = (struct Color){ .r = 242, .g = 249, .b = 245, .a = 255 };
        int32_t hxc_tmp_load_result_n500 = hxc_left_h59e66e62cca2;
        int32_t hxc_tmp_load_result_n501 = hxc_firstTop;
        DrawText("CREATIVE MODE", (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n500, 28), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n501, 16), (int32_t)21, hxc_this1_h3c3e9e95b1c8);
        int32_t hxc_top_h362eb72bb915 = hxc_i32_add_wrapping(hxc_i32_add_wrapping(hxc_firstTop, 54), 14);
        int32_t hxc_left_h01c37ab32b98 = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_width_h4c39cc537e03, (double)2)), 180);
        if (hxc_selectedMode == 1)
        {
          struct Color hxc_this1_h5ab6e6cdd588 = (struct Color){ .r = 16, .g = 88, .b = 102, .a = 232 };
          int32_t hxc_tmp_load_result_n507 = hxc_left_h01c37ab32b98;
          int32_t hxc_tmp_load_result_n508 = hxc_top_h362eb72bb915;
          DrawRectangle((int32_t)hxc_tmp_load_result_n507, (int32_t)hxc_tmp_load_result_n508, (int32_t)360, (int32_t)54, hxc_this1_h5ab6e6cdd588);
          struct Color hxc_this1_h922c566f2fec = (struct Color){ .r = 255, .g = 190, .b = 55, .a = 255 };
          int32_t hxc_tmp_load_result_n511 = hxc_left_h01c37ab32b98;
          int32_t hxc_tmp_load_result_n512 = hxc_top_h362eb72bb915;
          DrawRectangleLines((int32_t)hxc_tmp_load_result_n511, (int32_t)hxc_tmp_load_result_n512, (int32_t)360, (int32_t)54, hxc_this1_h922c566f2fec);
        }
        else
        {
          struct Color hxc_this1_h40172815f085 = (struct Color){ .r = 6, .g = 26, .b = 36, .a = 218 };
          int32_t hxc_tmp_load_result_n515 = hxc_left_h01c37ab32b98;
          int32_t hxc_tmp_load_result_n516 = hxc_top_h362eb72bb915;
          DrawRectangle((int32_t)hxc_tmp_load_result_n515, (int32_t)hxc_tmp_load_result_n516, (int32_t)360, (int32_t)54, hxc_this1_h40172815f085);
          struct Color hxc_this1_h82029f00bf2f = (struct Color){ .r = 92, .g = 194, .b = 188, .a = 255 };
          int32_t hxc_tmp_load_result_n519 = hxc_left_h01c37ab32b98;
          int32_t hxc_tmp_load_result_n520 = hxc_top_h362eb72bb915;
          DrawRectangleLines((int32_t)hxc_tmp_load_result_n519, (int32_t)hxc_tmp_load_result_n520, (int32_t)360, (int32_t)54, hxc_this1_h82029f00bf2f);
        }
        struct Color hxc_this1_hc18468f5e813 = (struct Color){ .r = 242, .g = 249, .b = 245, .a = 255 };
        int32_t hxc_tmp_load_result_n523 = hxc_left_h01c37ab32b98;
        int32_t hxc_tmp_load_result_n524 = hxc_top_h362eb72bb915;
        DrawText("ADVENTURE", (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n523, 28), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n524, 16), (int32_t)21, hxc_this1_hc18468f5e813);
        int32_t hxc_x_haef0b7366c12 = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_width_h4c39cc537e03, (double)2)), 270);
        struct Color hxc_this1_h00d59f9d7c58 = (struct Color){ .r = 229, .g = 241, .b = 235, .a = 255 };
        int32_t hxc_tmp_load_result_n528 = hxc_x_haef0b7366c12;
        int32_t hxc_tmp_load_result_n529 = hxc_height_h5d3d97c53946;
        DrawText("ARROWS / MOUSE TO CHOOSE   ENTER TO PLAY   L: ESPANOL", (int32_t)hxc_tmp_load_result_n528, (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n529, 58), (int32_t)16, hxc_this1_h00d59f9d7c58);
      }
      if (hxc_selectedMode == 1)
      {
        if (hxc_language == 1)
        {
          int32_t hxc_x_h580f0a1a1d61 = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_width_h4c39cc537e03, (double)2)), 210);
          struct Color hxc_this1_h8d4a171cde40 = (struct Color){ .r = 255, .g = 205, .b = 91, .a = 255 };
          int32_t hxc_tmp_load_result_n535 = hxc_x_h580f0a1a1d61;
          int32_t hxc_tmp_load_result_n536 = hxc_firstTop;
          DrawText("SALVA A CEESH. ENCUENTRA A IVVY. DETEN A BROWSER.", (int32_t)hxc_tmp_load_result_n535, (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n536, 36), (int32_t)17, hxc_this1_h8d4a171cde40);
        }
        else
        {
          int32_t hxc_x_h4b5cecc841b9 = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_width_h4c39cc537e03, (double)2)), 210);
          struct Color hxc_this1_hdf3d0dd71701 = (struct Color){ .r = 255, .g = 205, .b = 91, .a = 255 };
          int32_t hxc_tmp_load_result_n540 = hxc_x_h4b5cecc841b9;
          int32_t hxc_tmp_load_result_n541 = hxc_firstTop;
          DrawText("SAVE CEESH. FIND IVVY. STOP BROWSER.", (int32_t)hxc_tmp_load_result_n540, (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n541, 36), (int32_t)17, hxc_this1_hdf3d0dd71701);
        }
      }
    }
    else
    {
      ClearBackground((struct Color){ .r = 126, .g = 190, .b = 201, .a = 255 });
      BeginMode3D(hxc_camera);
      double hxc_tmp_record_field_load_result_n545 = hxc_player.hxc_x;
      struct hxc_caxecraft_app_Main_RenderCounters hxc_tmp_call_result_n547 = hxc_caxecraft_app_Main_drawWorld(hxc_cells, hxc_tmp_length_n1, hxc_tmp_record_field_load_result_n545, hxc_player.hxc_z);
      struct hxc_caxecraft_app_Main_RenderCounters hxc_renderCounters = hxc_tmp_call_result_n547;
      struct hxc_caxecraft_gameplay_GuideState hxc_tmp_load_result_n548 = hxc_guide;
      struct hxc_caxecraft_gameplay_MosslingState hxc_tmp_load_result_n549 = hxc_mossling;
      hxc_caxecraft_app_Main_drawActors(hxc_tmp_load_result_n548, hxc_tmp_load_result_n549, hxc_berryDrop);
      if (hxc_hit.hxc_hit)
      {
        double hxc_y = (double)hxc_hit.hxc_cellY + 0.5;
        double hxc_z = (double)hxc_hit.hxc_cellZ + 0.5;
        int32_t hxc_tmp_record_field_load_result_n554 = hxc_hit.hxc_cellX;
        double hxc_tmp_load_result_n555 = hxc_y;
        DrawCubeWires((struct Vector3){ .x = (float)((double)hxc_tmp_record_field_load_result_n554 + 0.5), .y = (float)hxc_tmp_load_result_n555, .z = (float)hxc_z }, (float)1.04, (float)1.04, (float)1.04, (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 });
      }
      EndMode3D();
      int32_t hxc_tmp_record_field_load_result_n559 = hxc_renderCounters.hxc_visible;
      int32_t hxc_tmp_record_field_load_result_n560 = hxc_renderCounters.hxc_drawCalls;
      int32_t hxc_tmp_load_result_n561 = hxc_frameCount;
      int32_t hxc_tmp_load_result_n562 = hxc_updateCount;
      bool hxc_tmp_load_result_n563 = hxc_paused;
      bool hxc_tmp_load_result_n564 = hxc_captured;
      int32_t hxc_tmp_load_result_n565 = hxc_placementBlockedFrames;
      struct hxc_caxecraft_domain_RaycastHit hxc_tmp_load_result_n566 = hxc_hit;
      double hxc_tmp_record_field_load_result_n567 = hxc_player.hxc_x;
      double hxc_tmp_record_field_load_result_n568 = hxc_player.hxc_z;
      int32_t hxc_tmp_load_result_n569 = hxc_selectedMode;
      int32_t hxc_tmp_load_result_n570 = hxc_language;
      struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_load_result_n571 = hxc_inventory;
      struct hxc_caxecraft_gameplay_GuideState hxc_tmp_load_result_n572 = hxc_guide;
      struct hxc_caxecraft_gameplay_MosslingState hxc_tmp_load_result_n573 = hxc_mossling;
      struct hxc_caxecraft_gameplay_PlayerVitalsState hxc_tmp_load_result_n574 = hxc_vitals;
      int32_t hxc_tmp_load_result_n575 = hxc_strikeHitFrames;
      int32_t hxc_tmp_load_result_n576 = hxc_enemyDefeatedFrames;
      int32_t hxc_tmp_load_result_n577 = hxc_pickupFrames;
      struct Texture hxc_tmp_load_result_n578 = hxc_hudTexture;
      bool hxc_tmp_load_result_n579 = hxc_hudTextureReady;
      struct Texture hxc_tmp_load_result_n580 = hxc_itemTexture;
      hxc_caxecraft_app_Main_drawHud(hxc_tmp_record_field_load_result_n559, hxc_tmp_record_field_load_result_n560, hxc_tmp_load_result_n561, hxc_tmp_load_result_n562, hxc_tmp_load_result_n563, hxc_tmp_load_result_n564, hxc_tmp_load_result_n565 > 0, hxc_tmp_load_result_n566, hxc_tmp_record_field_load_result_n567, hxc_tmp_record_field_load_result_n568, hxc_tmp_load_result_n569, hxc_tmp_load_result_n570, hxc_tmp_load_result_n571, hxc_tmp_load_result_n572, hxc_tmp_load_result_n573, hxc_tmp_load_result_n574, hxc_tmp_load_result_n575 > 0, hxc_tmp_load_result_n576 > 0, hxc_tmp_load_result_n577 > 0, hxc_tmp_load_result_n578, hxc_tmp_load_result_n579, hxc_tmp_load_result_n580, hxc_itemTextureReady);
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
