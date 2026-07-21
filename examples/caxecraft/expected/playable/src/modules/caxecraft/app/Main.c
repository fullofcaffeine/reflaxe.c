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

void hxc_caxecraft_app_Main_drawHud(int32_t hxc_visible, int32_t hxc_drawCalls, int32_t hxc_frames, int32_t hxc_updates, bool hxc_paused, bool hxc_captured, bool hxc_placementBlocked, struct hxc_caxecraft_domain_RaycastHit hxc_hit, double hxc_playerX, double hxc_playerZ, int32_t hxc_mode, int32_t hxc_language, struct hxc_caxecraft_gameplay_InventoryState hxc_inventory, struct hxc_caxecraft_gameplay_GuideState hxc_guide, struct hxc_caxecraft_gameplay_MosslingState hxc_mossling, struct hxc_caxecraft_gameplay_PlayerVitalsState hxc_vitals, bool hxc_strikeHit, bool hxc_enemyDefeated, bool hxc_pickedUp, int32_t hxc_pickupAmount, bool hxc_inventoryFull, int32_t hxc_recoveryFeedback, bool hxc_recoveryVisible, struct Texture hxc_hudTexture, bool hxc_hudTextureReady, struct Texture hxc_itemTexture, bool hxc_itemTextureReady)
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
  struct Color hxc_this1_h3acf251b8f01 = (struct Color){ .r = 15, .g = 27, .b = 39, .a = 220 };
  DrawRectangle((int32_t)18, (int32_t)18, (int32_t)460, (int32_t)108, hxc_this1_h3acf251b8f01);
  struct Color hxc_this1_hc871808efac0 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
  DrawRectangleLines((int32_t)18, (int32_t)18, (int32_t)460, (int32_t)108, hxc_this1_hc871808efac0);
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
      struct Color hxc_this1_he3dc70b18399 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
      DrawText("AVENTURA: PROLOGO EN CONSTRUCCION", (int32_t)32, (int32_t)110, (int32_t)14, hxc_this1_he3dc70b18399);
    }
    else
    {
      struct Color hxc_this1_h55ef83531dfc = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
      DrawText("ADVENTURE: PROLOGUE IN PROGRESS", (int32_t)32, (int32_t)110, (int32_t)14, hxc_this1_h55ef83531dfc);
    }
  }
  bool hxc_tmp_call_result_n49 = hxc_caxecraft_gameplay_GuideNpc_isInRange(hxc_guide, hxc_playerX, hxc_playerZ);
  if (hxc_tmp_call_result_n49)
  {
    struct Color hxc_this1_ha912d4c6567b = (struct Color){ .r = 15, .g = 27, .b = 39, .a = 220 };
    int32_t hxc_tmp_load_result_n51 = hxc_centerX;
    int32_t hxc_tmp_load_result_n52 = hxc_centerY;
    DrawRectangle((int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n51, 260), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n52, 54), (int32_t)520, (int32_t)60, hxc_this1_ha912d4c6567b);
    int32_t hxc_code_h4f6d88f054e8 = hxc_guide.hxc_phaseCode;
    int32_t hxc_tmp_load_result_n55 = hxc_code_h4f6d88f054e8;
    int32_t hxc_tmp_conditional_result_n38 = 0;
    if (hxc_tmp_load_result_n55 == 1)
    {
      hxc_tmp_conditional_result_n38 = 1;
    }
    else
    {
      int32_t hxc_tmp_load_result_n56 = hxc_code_h4f6d88f054e8;
      int32_t hxc_tmp_conditional_result_n39 = 0;
      if (hxc_tmp_load_result_n56 == 2)
      {
        hxc_tmp_conditional_result_n39 = 2;
      }
      else
      {
        hxc_tmp_conditional_result_n39 = 0;
      }
      hxc_tmp_conditional_result_n38 = hxc_tmp_conditional_result_n39;
    }
    if (hxc_tmp_conditional_result_n38 == 0)
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
      int32_t hxc_code_he98d0bdd49c4 = hxc_guide.hxc_phaseCode;
      int32_t hxc_tmp_load_result_n66 = hxc_code_he98d0bdd49c4;
      int32_t hxc_tmp_conditional_result_n41 = 0;
      if (hxc_tmp_load_result_n66 == 1)
      {
        hxc_tmp_conditional_result_n41 = 1;
      }
      else
      {
        int32_t hxc_tmp_load_result_n67 = hxc_code_he98d0bdd49c4;
        int32_t hxc_tmp_conditional_result_n42 = 0;
        if (hxc_tmp_load_result_n67 == 2)
        {
          hxc_tmp_conditional_result_n42 = 2;
        }
        else
        {
          hxc_tmp_conditional_result_n42 = 0;
        }
        hxc_tmp_conditional_result_n41 = hxc_tmp_conditional_result_n42;
      }
      if (hxc_tmp_conditional_result_n41 == 1)
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
    int32_t hxc_code_hcc86cee3ebf3 = hxc_mossling.hxc_modeCode;
    int32_t hxc_tmp_load_result_n84 = hxc_code_hcc86cee3ebf3;
    int32_t hxc_tmp_conditional_result_n44 = 0;
    if (hxc_tmp_load_result_n84 == 1)
    {
      hxc_tmp_conditional_result_n44 = 1;
    }
    else
    {
      int32_t hxc_tmp_load_result_n85 = hxc_code_hcc86cee3ebf3;
      int32_t hxc_tmp_conditional_result_n45 = 0;
      if (hxc_tmp_load_result_n85 == 2)
      {
        hxc_tmp_conditional_result_n45 = 2;
      }
      else
      {
        hxc_tmp_conditional_result_n45 = 0;
      }
      hxc_tmp_conditional_result_n44 = hxc_tmp_conditional_result_n45;
    }
    if (hxc_tmp_conditional_result_n44 == 1)
    {
      if (hxc_language == 1)
      {
        struct Color hxc_this1_h61038cae5aed = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
        int32_t hxc_tmp_load_result_n89 = hxc_width;
        DrawText("MUSGUITO ALERTA", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n89, 180), (int32_t)28, (int32_t)16, hxc_this1_h61038cae5aed);
      }
      else
      {
        struct Color hxc_this1_hbec14e23154f = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
        int32_t hxc_tmp_load_result_n92 = hxc_width;
        DrawText("MOSSLING ALERT", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n92, 180), (int32_t)28, (int32_t)16, hxc_this1_hbec14e23154f);
      }
    }
  }
  if (hxc_strikeHit)
  {
    if (hxc_language == 1)
    {
      struct Color hxc_this1_h32dc4652a04f = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
      int32_t hxc_tmp_load_result_n95 = hxc_centerX;
      int32_t hxc_tmp_load_result_n96 = hxc_centerY;
      DrawText("GOLPE DE COBRE", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n95, 70), (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n96, 54), (int32_t)18, hxc_this1_h32dc4652a04f);
    }
    else
    {
      struct Color hxc_this1_h21724e401543 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
      int32_t hxc_tmp_load_result_n99 = hxc_centerX;
      int32_t hxc_tmp_load_result_n100 = hxc_centerY;
      DrawText("COPPER STRIKE", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n99, 70), (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n100, 54), (int32_t)18, hxc_this1_h21724e401543);
    }
  }
  if (hxc_enemyDefeated)
  {
    if (hxc_language == 1)
    {
      struct Color hxc_this1_hefd02d94ccc2 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
      int32_t hxc_tmp_load_result_n103 = hxc_width;
      DrawText("EL MUSGUITO SOLTO BAYAS", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n103, 275), (int32_t)54, (int32_t)16, hxc_this1_hefd02d94ccc2);
    }
    else
    {
      struct Color hxc_this1_h8511e5a297c8 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
      int32_t hxc_tmp_load_result_n106 = hxc_width;
      DrawText("MOSSLING DROPPED BERRIES", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n106, 270), (int32_t)54, (int32_t)16, hxc_this1_h8511e5a297c8);
    }
  }
  if (hxc_pickedUp)
  {
    if (hxc_language == 1)
    {
      if (hxc_pickupAmount == 1)
      {
        struct Color hxc_this1_hb6c3705a5605 = (struct Color){ .r = 174, .g = 78, .b = 136, .a = 255 };
        int32_t hxc_tmp_load_result_n109 = hxc_centerX;
        int32_t hxc_tmp_load_result_n110 = hxc_centerY;
        DrawText("+1 BAYA", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n109, 42), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n110, 24), (int32_t)18, hxc_this1_hb6c3705a5605);
      }
      else
      {
        struct Color hxc_this1_h5a42031e0684 = (struct Color){ .r = 174, .g = 78, .b = 136, .a = 255 };
        int32_t hxc_tmp_load_result_n113 = hxc_centerX;
        int32_t hxc_tmp_load_result_n114 = hxc_centerY;
        DrawText("+2 BAYAS", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n113, 42), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n114, 24), (int32_t)18, hxc_this1_h5a42031e0684);
      }
    }
    else
    {
      if (hxc_pickupAmount == 1)
      {
        struct Color hxc_this1_h419cefcdc681 = (struct Color){ .r = 174, .g = 78, .b = 136, .a = 255 };
        int32_t hxc_tmp_load_result_n117 = hxc_centerX;
        int32_t hxc_tmp_load_result_n118 = hxc_centerY;
        DrawText("+1 BERRY", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n117, 48), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n118, 24), (int32_t)18, hxc_this1_h419cefcdc681);
      }
      else
      {
        struct Color hxc_this1_ha06dac8e8d66 = (struct Color){ .r = 174, .g = 78, .b = 136, .a = 255 };
        int32_t hxc_tmp_load_result_n121 = hxc_centerX;
        int32_t hxc_tmp_load_result_n122 = hxc_centerY;
        DrawText("+2 BERRIES", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n121, 48), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n122, 24), (int32_t)18, hxc_this1_ha06dac8e8d66);
      }
    }
  }
  if (hxc_inventoryFull)
  {
    if (hxc_language == 1)
    {
      struct Color hxc_this1_h304f07ce5fd8 = (struct Color){ .r = 238, .g = 113, .b = 78, .a = 255 };
      int32_t hxc_tmp_load_result_n125 = hxc_centerX;
      int32_t hxc_tmp_load_result_n126 = hxc_centerY;
      DrawText("BAYAS LLENAS: USA UNA PRIMERO", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n125, 150), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n126, 48), (int32_t)16, hxc_this1_h304f07ce5fd8);
    }
    else
    {
      struct Color hxc_this1_h8279793553af = (struct Color){ .r = 238, .g = 113, .b = 78, .a = 255 };
      int32_t hxc_tmp_load_result_n129 = hxc_centerX;
      int32_t hxc_tmp_load_result_n130 = hxc_centerY;
      DrawText("BERRIES FULL: USE ONE FIRST", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n129, 140), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n130, 48), (int32_t)16, hxc_this1_h8279793553af);
    }
  }
  if (hxc_recoveryVisible)
  {
    if (hxc_recoveryFeedback == 1)
    {
      if (hxc_language == 1)
      {
        struct Color hxc_this1_ha0a09668d749 = (struct Color){ .r = 94, .g = 212, .b = 136, .a = 255 };
        int32_t hxc_tmp_load_result_n133 = hxc_centerX;
        int32_t hxc_tmp_load_result_n134 = hxc_centerY;
        DrawText("BAYAS: +1 CORAZON", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n133, 86), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n134, 24), (int32_t)18, hxc_this1_ha0a09668d749);
      }
      else
      {
        struct Color hxc_this1_h1253f4916bea = (struct Color){ .r = 94, .g = 212, .b = 136, .a = 255 };
        int32_t hxc_tmp_load_result_n137 = hxc_centerX;
        int32_t hxc_tmp_load_result_n138 = hxc_centerY;
        DrawText("BERRIES: +1 HEART", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n137, 88), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n138, 24), (int32_t)18, hxc_this1_h1253f4916bea);
      }
    }
    else
    {
      if (hxc_recoveryFeedback == 2)
      {
        if (hxc_language == 1)
        {
          struct Color hxc_this1_hb09beccd9cae = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
          int32_t hxc_tmp_load_result_n141 = hxc_centerX;
          int32_t hxc_tmp_load_result_n142 = hxc_centerY;
          DrawText("SALUD COMPLETA", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n141, 72), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n142, 24), (int32_t)18, hxc_this1_hb09beccd9cae);
        }
        else
        {
          struct Color hxc_this1_h79969382e99e = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
          int32_t hxc_tmp_load_result_n145 = hxc_centerX;
          int32_t hxc_tmp_load_result_n146 = hxc_centerY;
          DrawText("HEALTH ALREADY FULL", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n145, 96), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n146, 24), (int32_t)18, hxc_this1_h79969382e99e);
        }
      }
      else
      {
        if (hxc_recoveryFeedback == 3)
        {
          if (hxc_language == 1)
          {
            struct Color hxc_this1_h7dbf3aa828c2 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
            int32_t hxc_tmp_load_result_n149 = hxc_centerX;
            int32_t hxc_tmp_load_result_n150 = hxc_centerY;
            DrawText("NO QUEDAN BAYAS", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n149, 76), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n150, 24), (int32_t)18, hxc_this1_h7dbf3aa828c2);
          }
          else
          {
            struct Color hxc_this1_h8041e68167cf = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
            int32_t hxc_tmp_load_result_n153 = hxc_centerX;
            int32_t hxc_tmp_load_result_n154 = hxc_centerY;
            DrawText("NO BERRIES LEFT", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n153, 76), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n154, 24), (int32_t)18, hxc_this1_h8041e68167cf);
          }
        }
      }
    }
  }
  if (hxc_vitals.hxc_safeTicks > 15)
  {
    struct Color hxc_this1_h26cff9265e7d = (struct Color){ .r = 218, .g = 65, .b = 72, .a = 255 };
    int32_t hxc_tmp_load_result_n158 = hxc_width;
    int32_t hxc_tmp_load_result_n159 = hxc_height;
    DrawRectangleLines((int32_t)4, (int32_t)4, (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n158, 8), (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n159, 8), hxc_this1_h26cff9265e7d);
  }
  if (hxc_vitals.hxc_health <= 0)
  {
    struct Color hxc_this1_h97e2af335da7 = (struct Color){ .r = 15, .g = 27, .b = 39, .a = 220 };
    int32_t hxc_tmp_load_result_n163 = hxc_centerX;
    int32_t hxc_tmp_load_result_n164 = hxc_centerY;
    DrawRectangle((int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n163, 250), (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n164, 74), (int32_t)500, (int32_t)148, hxc_this1_h97e2af335da7);
    struct Color hxc_this1_h83f91ea4cbeb = (struct Color){ .r = 218, .g = 65, .b = 72, .a = 255 };
    int32_t hxc_tmp_load_result_n167 = hxc_centerX;
    int32_t hxc_tmp_load_result_n168 = hxc_centerY;
    DrawRectangleLines((int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n167, 250), (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n168, 74), (int32_t)500, (int32_t)148, hxc_this1_h83f91ea4cbeb);
    if (hxc_language == 1)
    {
      int32_t hxc_tmp_load_result_n170 = hxc_centerX;
      int32_t hxc_tmp_load_result_n171 = hxc_centerY;
      DrawText("HAXIRIO HA CAIDO", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n170, 112), (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n171, 42), (int32_t)24, hxc_text);
      struct Color hxc_this1_hddaf31dbe708 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
      int32_t hxc_tmp_load_result_n174 = hxc_centerX;
      int32_t hxc_tmp_load_result_n175 = hxc_centerY;
      DrawText("E  VOLVER AL PRADO", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n174, 105), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n175, 10), (int32_t)18, hxc_this1_hddaf31dbe708);
    }
    else
    {
      int32_t hxc_tmp_load_result_n177 = hxc_centerX;
      int32_t hxc_tmp_load_result_n178 = hxc_centerY;
      DrawText("HAXIRIO HAS FALLEN", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n177, 122), (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n178, 42), (int32_t)24, hxc_text);
      struct Color hxc_this1_h8bfb75066a50 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
      int32_t hxc_tmp_load_result_n181 = hxc_centerX;
      int32_t hxc_tmp_load_result_n182 = hxc_centerY;
      DrawText("E  RETURN TO THE MEADOW", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n181, 125), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n182, 10), (int32_t)18, hxc_this1_h8bfb75066a50);
    }
  }
  if (hxc_paused)
  {
    struct Color hxc_this1_h04063ea28cd4 = (struct Color){ .r = 15, .g = 27, .b = 39, .a = 220 };
    int32_t hxc_tmp_load_result_n185 = hxc_centerX;
    int32_t hxc_tmp_load_result_n186 = hxc_centerY;
    DrawRectangle((int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n185, 170), (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n186, 48), (int32_t)340, (int32_t)96, hxc_this1_h04063ea28cd4);
    struct Color hxc_this1_h4636bb841979 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
    int32_t hxc_tmp_load_result_n189 = hxc_centerX;
    int32_t hxc_tmp_load_result_n190 = hxc_centerY;
    DrawRectangleLines((int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n189, 170), (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n190, 48), (int32_t)340, (int32_t)96, hxc_this1_h4636bb841979);
    int32_t hxc_tmp_load_result_n192 = hxc_centerX;
    int32_t hxc_tmp_load_result_n193 = hxc_centerY;
    DrawText("PAUSED", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n192, 48), (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n193, 30), (int32_t)24, hxc_text);
    int32_t hxc_tmp_load_result_n195 = hxc_centerX;
    int32_t hxc_tmp_load_result_n196 = hxc_centerY;
    DrawText("CLICK TO CAPTURE  //  Q TO QUIT", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n195, 145), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n196, 8), (int32_t)16, hxc_text);
  }
  else
  {
    if (hxc_placementBlocked)
    {
      struct Color hxc_this1_hf68e8055bd2d = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
      int32_t hxc_tmp_load_result_n199 = hxc_centerX;
      int32_t hxc_tmp_load_result_n200 = hxc_centerY;
      DrawText("PLACE BLOCKED: PLAYER OR WORLD EDGE", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n199, 150), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n200, 26), (int32_t)14, hxc_this1_hf68e8055bd2d);
    }
    else
    {
      if (!hxc_captured)
      {
        int32_t hxc_tmp_load_result_n202 = hxc_centerX;
        int32_t hxc_tmp_load_result_n203 = hxc_centerY;
        DrawText("CLICK TO CAPTURE", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n202, 74), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n203, 26), (int32_t)14, hxc_text);
      }
      else
      {
        if (!hxc_hit.hxc_hit)
        {
          int32_t hxc_tmp_load_result_n206 = hxc_centerX;
          int32_t hxc_tmp_load_result_n207 = hxc_centerY;
          DrawText("NO BLOCK IN REACH", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n206, 78), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n207, 26), (int32_t)14, hxc_text);
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
  int32_t hxc_pickupAmount = 0;
  int32_t hxc_inventoryFullFrames = 0;
  int32_t hxc_recoveryFeedback = 0;
  int32_t hxc_recoveryFeedbackFrames = 0;
  while (1)
  {
    bool hxc_tmp_load_result_n21 = hxc_quit;
    bool hxc_tmp_short_circuit_result_n39 = !hxc_tmp_load_result_n21;
    if (!hxc_tmp_load_result_n21)
    {
      bool hxc_tmp_native_call_result_n22 = WindowShouldClose();
      hxc_tmp_short_circuit_result_n39 = !hxc_tmp_native_call_result_n22;
    }
    if (!hxc_tmp_short_circuit_result_n39)
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
    double hxc_lookYaw_h0be9978166d5 = 0.0;
    double hxc_lookPitch_ha02a556046e6 = 0.0;
    if (hxc_captured)
    {
      struct Vector2 hxc_tmp_native_call_result_n34 = GetMouseDelta();
      struct Vector2 hxc_mouse = hxc_tmp_native_call_result_n34;
      hxc_lookYaw_h0be9978166d5 = -(double)hxc_mouse.x * 0.0025;
      hxc_lookPitch_ha02a556046e6 = -(double)hxc_mouse.y * 0.0025;
    }
    bool hxc_tmp_native_call_result_n39 = IsMouseButtonPressed((int32_t)0);
    bool hxc_leftPressed = hxc_tmp_native_call_result_n39;
    bool hxc_tmp_load_result_n40 = hxc_captured;
    bool hxc_tmp_short_circuit_result_n49 = hxc_tmp_load_result_n40;
    if (hxc_tmp_load_result_n40)
    {
      hxc_tmp_short_circuit_result_n49 = hxc_leftPressed;
    }
    bool hxc_primaryPressed_he546759256a4 = hxc_tmp_short_circuit_result_n49;
    bool hxc_tmp_load_result_n43 = hxc_captured;
    bool hxc_tmp_short_circuit_result_n51 = hxc_tmp_load_result_n43;
    if (hxc_tmp_load_result_n43)
    {
      bool hxc_tmp_native_call_result_n44 = IsMouseButtonPressed((int32_t)1);
      hxc_tmp_short_circuit_result_n51 = hxc_tmp_native_call_result_n44;
    }
    bool hxc_secondaryPressed_h14703ed11408 = hxc_tmp_short_circuit_result_n51;
    bool hxc_tmp_load_result_n46 = hxc_captured;
    bool hxc_tmp_short_circuit_result_n53 = hxc_tmp_load_result_n46;
    if (hxc_tmp_load_result_n46)
    {
      bool hxc_tmp_native_call_result_n47 = IsKeyPressed((int32_t)69);
      hxc_tmp_short_circuit_result_n53 = hxc_tmp_native_call_result_n47;
    }
    bool hxc_interactPressed_h4acb3b711275 = hxc_tmp_short_circuit_result_n53;
    int32_t hxc_hotbarSelection_h5ec8f016ebcc = -1;
    bool hxc_tmp_native_call_result_n49 = IsKeyPressed((int32_t)49);
    if (hxc_tmp_native_call_result_n49)
    {
      hxc_hotbarSelection_h5ec8f016ebcc = 0;
    }
    bool hxc_tmp_native_call_result_n50 = IsKeyPressed((int32_t)50);
    if (hxc_tmp_native_call_result_n50)
    {
      hxc_hotbarSelection_h5ec8f016ebcc = 1;
    }
    bool hxc_tmp_native_call_result_n51 = IsKeyPressed((int32_t)51);
    if (hxc_tmp_native_call_result_n51)
    {
      hxc_hotbarSelection_h5ec8f016ebcc = 2;
    }
    bool hxc_tmp_native_call_result_n52 = IsKeyPressed((int32_t)52);
    if (hxc_tmp_native_call_result_n52)
    {
      hxc_hotbarSelection_h5ec8f016ebcc = 3;
    }
    bool hxc_tmp_native_call_result_n53 = IsKeyPressed((int32_t)53);
    if (hxc_tmp_native_call_result_n53)
    {
      hxc_hotbarSelection_h5ec8f016ebcc = 4;
    }
    bool hxc_tmp_native_call_result_n54 = IsKeyPressed((int32_t)54);
    if (hxc_tmp_native_call_result_n54)
    {
      hxc_hotbarSelection_h5ec8f016ebcc = 5;
    }
    bool hxc_tmp_native_call_result_n55 = IsKeyPressed((int32_t)55);
    if (hxc_tmp_native_call_result_n55)
    {
      hxc_hotbarSelection_h5ec8f016ebcc = 6;
    }
    bool hxc_tmp_native_call_result_n56 = IsKeyPressed((int32_t)56);
    if (hxc_tmp_native_call_result_n56)
    {
      hxc_hotbarSelection_h5ec8f016ebcc = 7;
    }
    float hxc_tmp_native_call_result_n57 = GetMouseWheelMove();
    double hxc_wheel = (double)hxc_tmp_native_call_result_n57;
    int32_t hxc_hotbarCycle_h4dbd15feb39b = 0;
    if (hxc_wheel > 0.0)
    {
      hxc_hotbarCycle_h4dbd15feb39b = -1;
    }
    if (hxc_wheel < 0.0)
    {
      hxc_hotbarCycle_h4dbd15feb39b = 1;
    }
    bool hxc_tmp_native_call_result_n60 = IsKeyPressed((int32_t)32);
    bool hxc_jumpPressed_ha97f86150079 = hxc_tmp_native_call_result_n60;
    bool hxc_tmp_native_call_result_n61 = IsKeyPressed((int32_t)256);
    bool hxc_pausePressed_habe35e6055cb = hxc_tmp_native_call_result_n61;
    bool hxc_tmp_native_call_result_n62 = IsKeyPressed((int32_t)81);
    bool hxc_quitPressed_h91ddfe8fa43e = hxc_tmp_native_call_result_n62;
    double hxc_frameInput_moveForward = hxc_forward;
    double hxc_frameInput_moveRight = hxc_right;
    double hxc_frameInput_lookYaw = hxc_lookYaw_h0be9978166d5;
    double hxc_frameInput_lookPitch = hxc_lookPitch_ha02a556046e6;
    bool hxc_frameInput_jumpPressed = hxc_jumpPressed_ha97f86150079;
    bool hxc_frameInput_primaryPressed = hxc_primaryPressed_he546759256a4;
    bool hxc_frameInput_secondaryPressed = hxc_secondaryPressed_h14703ed11408;
    bool hxc_frameInput_interactPressed = hxc_interactPressed_h4acb3b711275;
    bool hxc_frameInput_pausePressed = hxc_pausePressed_habe35e6055cb;
    bool hxc_tmp_load_result_n72 = hxc_paused;
    bool hxc_tmp_short_circuit_result_n70 = hxc_tmp_load_result_n72;
    if (hxc_tmp_load_result_n72)
    {
      hxc_tmp_short_circuit_result_n70 = hxc_leftPressed;
    }
    bool hxc_frameInput_capturePressed = hxc_tmp_short_circuit_result_n70;
    bool hxc_frameInput_quitPressed = hxc_quitPressed_h91ddfe8fa43e;
    int32_t hxc_frameInput_hotbarSelection = hxc_hotbarSelection_h5ec8f016ebcc;
    int32_t hxc_frameInput_hotbarCycle = hxc_hotbarCycle_h4dbd15feb39b;
    double hxc_moveForward = hxc_frameInput_moveForward;
    double hxc_moveRight = hxc_frameInput_moveRight;
    double hxc_lookYaw_h4c7f81fbce4b = hxc_frameInput_lookYaw;
    double hxc_lookPitch_h38da1126a4a1 = hxc_frameInput_lookPitch;
    bool hxc_jumpPressed_h322089573d8d = hxc_frameInput_jumpPressed;
    bool hxc_primaryPressed_h0b61b55dc346 = hxc_frameInput_primaryPressed;
    bool hxc_secondaryPressed_haf031cf26c58 = hxc_frameInput_secondaryPressed;
    bool hxc_interactPressed_hebddb02f1d3a = hxc_frameInput_interactPressed;
    bool hxc_pausePressed_hb81543af5557 = hxc_frameInput_pausePressed;
    bool hxc_capturePressed = hxc_frameInput_capturePressed;
    bool hxc_quitPressed_h237472d9b803 = hxc_frameInput_quitPressed;
    int32_t hxc_hotbarSelection_h6ff5b8ca2e1e = hxc_frameInput_hotbarSelection;
    int32_t hxc_hotbarCycle_h22a7600e279c = hxc_frameInput_hotbarCycle;
    if (hxc_quitPressed_h237472d9b803)
    {
      hxc_quit = true;
    }
    if (hxc_hotbarSelection_h6ff5b8ca2e1e >= 0)
    {
      struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_load_result_n93 = hxc_inventory;
      struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_call_result_n95 = hxc_caxecraft_gameplay_Inventory_select(hxc_tmp_load_result_n93, hxc_hotbarSelection_h6ff5b8ca2e1e);
      hxc_inventory = hxc_tmp_call_result_n95;
    }
    if (hxc_hotbarCycle_h22a7600e279c != 0)
    {
      struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_load_result_n97 = hxc_inventory;
      struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_call_result_n99 = hxc_caxecraft_gameplay_Inventory_cycle(hxc_tmp_load_result_n97, hxc_hotbarCycle_h22a7600e279c);
      hxc_inventory = hxc_tmp_call_result_n99;
    }
    bool hxc_tmp_load_result_n100 = hxc_onTitle;
    bool hxc_tmp_short_circuit_result_n87 = !hxc_tmp_load_result_n100;
    if (!hxc_tmp_load_result_n100)
    {
      hxc_tmp_short_circuit_result_n87 = !hxc_paused;
    }
    bool hxc_tmp_short_circuit_load_result_n102 = hxc_tmp_short_circuit_result_n87;
    bool hxc_tmp_short_circuit_result_n88 = hxc_tmp_short_circuit_load_result_n102;
    if (hxc_tmp_short_circuit_load_result_n102)
    {
      hxc_tmp_short_circuit_result_n88 = hxc_interactPressed_hebddb02f1d3a;
    }
    if (hxc_tmp_short_circuit_result_n88)
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
          if (hxc_sharesBerries)
          {
            int32_t hxc_tmp_call_result_n117 = hxc_caxecraft_gameplay_Inventory_acceptedAmount(hxc_inventory, hxc_caxecraft_gameplay_ItemKind_Berries, 2);
            int32_t hxc_acceptedGift = hxc_tmp_call_result_n117;
            if (hxc_acceptedGift == 2)
            {
              struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_load_result_n119 = hxc_inventory;
              struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_call_result_n122 = hxc_caxecraft_gameplay_Inventory_collectItem(hxc_tmp_load_result_n119, hxc_caxecraft_gameplay_ItemKind_Berries, hxc_acceptedGift);
              hxc_inventory = hxc_tmp_call_result_n122;
              struct hxc_caxecraft_gameplay_GuideState hxc_tmp_call_result_n124 = hxc_caxecraft_gameplay_GuideNpc_interact(hxc_guide);
              hxc_guide = hxc_tmp_call_result_n124;
            }
            else
            {
              hxc_inventoryFullFrames = 90;
            }
          }
          else
          {
            struct hxc_caxecraft_gameplay_GuideState hxc_tmp_call_result_n126 = hxc_caxecraft_gameplay_GuideNpc_interact(hxc_guide);
            hxc_guide = hxc_tmp_call_result_n126;
          }
        }
      }
    }
    bool hxc_tmp_load_result_n127 = hxc_onTitle;
    bool hxc_tmp_short_circuit_result_n91 = hxc_tmp_load_result_n127;
    if (hxc_tmp_load_result_n127)
    {
      hxc_tmp_short_circuit_result_n91 = hxc_focused;
    }
    if (hxc_tmp_short_circuit_result_n91)
    {
      bool hxc_tmp_native_call_result_n130 = IsKeyPressed((int32_t)76);
      if (hxc_tmp_native_call_result_n130)
      {
        int32_t hxc_tmp_load_result_n131 = hxc_language;
        int32_t hxc_tmp_conditional_result_n92 = 0;
        if (hxc_tmp_load_result_n131 == 0)
        {
          hxc_tmp_conditional_result_n92 = 1;
        }
        else
        {
          hxc_tmp_conditional_result_n92 = 0;
        }
        hxc_language = hxc_tmp_conditional_result_n92;
      }
      bool hxc_tmp_native_call_result_n133 = IsKeyPressed((int32_t)265);
      bool hxc_tmp_short_circuit_result_n93 = hxc_tmp_native_call_result_n133;
      if (!hxc_tmp_native_call_result_n133)
      {
        bool hxc_tmp_native_call_result_n134 = IsKeyPressed((int32_t)264);
        hxc_tmp_short_circuit_result_n93 = hxc_tmp_native_call_result_n134;
      }
      if (hxc_tmp_short_circuit_result_n93)
      {
        int32_t hxc_tmp_load_result_n136 = hxc_selectedMode;
        int32_t hxc_tmp_conditional_result_n94 = 0;
        if (hxc_tmp_load_result_n136 == 0)
        {
          hxc_tmp_conditional_result_n94 = 1;
        }
        else
        {
          hxc_tmp_conditional_result_n94 = 0;
        }
        hxc_selectedMode = hxc_tmp_conditional_result_n94;
      }
      struct Vector2 hxc_tmp_native_call_result_n138 = GetMousePosition();
      struct Vector2 hxc_menuMouse = hxc_tmp_native_call_result_n138;
      struct Vector2 hxc_tmp_load_result_n139 = hxc_menuMouse;
      struct Vector2 hxc_tmp_load_result_n141 = hxc_menuMouse;
      int32_t hxc_tmp_native_call_result_n143 = GetScreenWidth();
      int32_t hxc_tmp_native_call_result_n144 = GetScreenHeight();
      int32_t hxc_tmp_call_result_n145 = hxc_caxecraft_app_TitleMenu_selectionAt((double)hxc_tmp_load_result_n139.x, (double)hxc_tmp_load_result_n141.y, (int32_t)hxc_tmp_native_call_result_n143, (int32_t)hxc_tmp_native_call_result_n144);
      int32_t hxc_hovered = hxc_tmp_call_result_n145;
      if (hxc_hovered == 0)
      {
        hxc_selectedMode = 0;
      }
      if (hxc_hovered == 1)
      {
        hxc_selectedMode = 1;
      }
      int32_t hxc_tmp_load_result_n148 = hxc_hovered;
      bool hxc_tmp_short_circuit_result_n98 = hxc_tmp_load_result_n148 >= 0;
      if (hxc_tmp_load_result_n148 >= 0)
      {
        bool hxc_tmp_native_call_result_n149 = IsMouseButtonPressed((int32_t)0);
        hxc_tmp_short_circuit_result_n98 = hxc_tmp_native_call_result_n149;
      }
      bool hxc_clickedChoice = hxc_tmp_short_circuit_result_n98;
      bool hxc_tmp_load_result_n151 = hxc_clickedChoice;
      bool hxc_tmp_short_circuit_result_n99 = hxc_tmp_load_result_n151;
      if (!hxc_tmp_load_result_n151)
      {
        bool hxc_tmp_native_call_result_n152 = IsKeyPressed((int32_t)257);
        hxc_tmp_short_circuit_result_n99 = hxc_tmp_native_call_result_n152;
      }
      if (hxc_tmp_short_circuit_result_n99)
      {
        hxc_onTitle = false;
        hxc_paused = false;
        hxc_captured = true;
        hxc_recapturedThisFrame = true;
        DisableCursor();
      }
    }
    bool hxc_tmp_load_result_n154 = hxc_onTitle;
    bool hxc_tmp_short_circuit_result_n100 = !hxc_tmp_load_result_n154;
    if (!hxc_tmp_load_result_n154)
    {
      hxc_tmp_short_circuit_result_n100 = !hxc_focused;
    }
    bool hxc_tmp_short_circuit_load_result_n156 = hxc_tmp_short_circuit_result_n100;
    bool hxc_tmp_short_circuit_result_n101 = hxc_tmp_short_circuit_load_result_n156;
    if (hxc_tmp_short_circuit_load_result_n156)
    {
      hxc_tmp_short_circuit_result_n101 = hxc_captured;
    }
    if (hxc_tmp_short_circuit_result_n101)
    {
      hxc_paused = true;
      hxc_captured = false;
      hxc_jumpQueued = false;
      EnableCursor();
    }
    bool hxc_tmp_load_result_n159 = hxc_onTitle;
    bool hxc_tmp_short_circuit_result_n102 = !hxc_tmp_load_result_n159;
    if (!hxc_tmp_load_result_n159)
    {
      hxc_tmp_short_circuit_result_n102 = hxc_focused;
    }
    bool hxc_tmp_short_circuit_load_result_n161 = hxc_tmp_short_circuit_result_n102;
    bool hxc_tmp_short_circuit_result_n103 = hxc_tmp_short_circuit_load_result_n161;
    if (hxc_tmp_short_circuit_load_result_n161)
    {
      hxc_tmp_short_circuit_result_n103 = hxc_pausePressed_hb81543af5557;
    }
    if (hxc_tmp_short_circuit_result_n103)
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
    bool hxc_tmp_load_result_n168 = hxc_onTitle;
    bool hxc_tmp_short_circuit_result_n104 = !hxc_tmp_load_result_n168;
    if (!hxc_tmp_load_result_n168)
    {
      hxc_tmp_short_circuit_result_n104 = hxc_focused;
    }
    bool hxc_tmp_short_circuit_load_result_n170 = hxc_tmp_short_circuit_result_n104;
    bool hxc_tmp_short_circuit_result_n105 = hxc_tmp_short_circuit_load_result_n170;
    if (hxc_tmp_short_circuit_load_result_n170)
    {
      hxc_tmp_short_circuit_result_n105 = hxc_paused;
    }
    bool hxc_tmp_short_circuit_load_result_n172 = hxc_tmp_short_circuit_result_n105;
    bool hxc_tmp_short_circuit_result_n106 = hxc_tmp_short_circuit_load_result_n172;
    if (hxc_tmp_short_circuit_load_result_n172)
    {
      hxc_tmp_short_circuit_result_n106 = hxc_capturePressed;
    }
    if (hxc_tmp_short_circuit_result_n106)
    {
      hxc_paused = false;
      hxc_captured = true;
      hxc_recapturedThisFrame = true;
      DisableCursor();
    }
    if (hxc_captured)
    {
      double hxc_yawDelta = hxc_lookYaw_h4c7f81fbce4b;
      if (hxc_yawDelta > 0.25)
      {
        hxc_yawDelta = 0.25;
      }
      if (hxc_yawDelta < -0.25)
      {
        hxc_yawDelta = -0.25;
      }
      double hxc_tmp_load_result_n179 = hxc_lookX;
      double hxc_tmp_load_result_n180 = hxc_yawDelta;
      double hxc_candidateX = hxc_tmp_load_result_n179 + hxc_tmp_load_result_n180 * hxc_lookZ;
      double hxc_tmp_load_result_n182 = hxc_lookZ;
      double hxc_tmp_load_result_n183 = hxc_yawDelta;
      double hxc_candidateZ = hxc_tmp_load_result_n182 - hxc_tmp_load_result_n183 * hxc_lookX;
      double hxc_tmp_load_result_n185 = hxc_candidateX;
      double hxc_tmp_load_result_n186 = hxc_candidateX;
      double hxc_tmp_load_result_n187 = hxc_candidateZ;
      double hxc_lengthSquared = hxc_tmp_load_result_n185 * hxc_tmp_load_result_n186 + hxc_tmp_load_result_n187 * hxc_candidateZ;
      double hxc_normalize = 1.5 - 0.5 * hxc_lengthSquared;
      double hxc_tmp_load_result_n190 = hxc_candidateX;
      hxc_lookX = hxc_tmp_load_result_n190 * hxc_normalize;
      double hxc_tmp_load_result_n192 = hxc_candidateZ;
      hxc_lookZ = hxc_tmp_load_result_n192 * hxc_normalize;
      double hxc_tmp_compound_load_result_n194 = hxc_lookY;
      hxc_lookY = hxc_tmp_compound_load_result_n194 + hxc_lookPitch_h38da1126a4a1;
      if (hxc_lookY > 0.90)
      {
        hxc_lookY = 0.90;
      }
      if (hxc_lookY < -0.90)
      {
        hxc_lookY = -0.90;
      }
      if (hxc_jumpPressed_h322089573d8d)
      {
        hxc_jumpQueued = true;
      }
    }
    float hxc_tmp_native_call_result_n199 = GetFrameTime();
    double hxc_frameSeconds = (double)hxc_tmp_native_call_result_n199;
    if (hxc_frameSeconds > 0.25)
    {
      hxc_frameSeconds = 0.25;
    }
    if (!hxc_paused)
    {
      double hxc_tmp_compound_load_result_n202 = hxc_accumulator;
      hxc_accumulator = hxc_tmp_compound_load_result_n202 + hxc_frameSeconds;
    }
    while (1)
    {
      bool hxc_tmp_load_result_n204 = hxc_paused;
      bool hxc_tmp_short_circuit_result_n113 = !hxc_tmp_load_result_n204;
      if (!hxc_tmp_load_result_n204)
      {
        hxc_tmp_short_circuit_result_n113 = hxc_accumulator >= 0.05;
      }
      if (!hxc_tmp_short_circuit_result_n113)
      {
        break;
      }
      double hxc_tmp_load_result_n207 = hxc_moveForward;
      double hxc_tmp_load_result_n208 = hxc_lookX;
      double hxc_tmp_load_result_n209 = hxc_moveRight;
      double hxc_moveX = hxc_tmp_load_result_n207 * hxc_tmp_load_result_n208 - hxc_tmp_load_result_n209 * hxc_lookZ;
      double hxc_tmp_load_result_n211 = hxc_moveForward;
      double hxc_tmp_load_result_n212 = hxc_lookZ;
      double hxc_tmp_load_result_n213 = hxc_moveRight;
      double hxc_moveZ = hxc_tmp_load_result_n211 * hxc_tmp_load_result_n212 + hxc_tmp_load_result_n213 * hxc_lookX;
      double hxc_tmp_load_result_n215 = hxc_moveForward;
      bool hxc_tmp_short_circuit_result_n116 = hxc_tmp_load_result_n215 != 0.0;
      if (hxc_tmp_load_result_n215 != 0.0)
      {
        hxc_tmp_short_circuit_result_n116 = hxc_moveRight != 0.0;
      }
      if (hxc_tmp_short_circuit_result_n116)
      {
        hxc_moveX = hxc_moveX * 0.7071067811865476;
        hxc_moveZ = hxc_moveZ * 0.7071067811865476;
      }
      if (hxc_vitals.hxc_health > 0)
      {
        struct hxc_caxecraft_domain_PlayerState hxc_tmp_load_result_n221 = hxc_player;
        double hxc_tmp_load_result_n222 = hxc_moveX;
        double hxc_tmp_load_result_n223 = hxc_moveZ;
        struct hxc_caxecraft_domain_StepInput hxc_tmp_call_result_n225 = hxc_caxecraft_domain_PlayerPhysics_input(hxc_tmp_load_result_n222, hxc_tmp_load_result_n223, hxc_jumpQueued);
        struct hxc_caxecraft_domain_PlayerState hxc_tmp_call_result_n226 = hxc_caxecraft_domain_PlayerPhysics_step(hxc_cells, hxc_tmp_length_n1, hxc_tmp_load_result_n221, hxc_tmp_call_result_n225);
        hxc_player = hxc_tmp_call_result_n226;
      }
      if (hxc_selectedMode == 1)
      {
        if (hxc_vitals.hxc_health > 0)
        {
          struct hxc_caxecraft_gameplay_MosslingState hxc_tmp_load_result_n229 = hxc_mossling;
          double hxc_tmp_record_field_load_result_n230 = hxc_player.hxc_x;
          struct hxc_caxecraft_gameplay_MosslingState hxc_tmp_call_result_n232 = hxc_caxecraft_gameplay_Mossling_step(hxc_cells, hxc_tmp_length_n1, hxc_tmp_load_result_n229, hxc_tmp_record_field_load_result_n230, hxc_player.hxc_z);
          hxc_mossling = hxc_tmp_call_result_n232;
          struct hxc_caxecraft_gameplay_PlayerVitalsState hxc_tmp_load_result_n233 = hxc_vitals;
          double hxc_tmp_record_field_load_result_n234 = hxc_player.hxc_x;
          double hxc_tmp_record_field_load_result_n235 = hxc_player.hxc_z;
          double hxc_tmp_record_field_load_result_n236 = hxc_mossling.hxc_x;
          double hxc_tmp_record_field_load_result_n237 = hxc_mossling.hxc_z;
          struct hxc_caxecraft_gameplay_PlayerVitalsState hxc_tmp_call_result_n239 = hxc_caxecraft_gameplay_PlayerVitals_step(hxc_tmp_load_result_n233, hxc_tmp_record_field_load_result_n234, hxc_tmp_record_field_load_result_n235, hxc_tmp_record_field_load_result_n236, hxc_tmp_record_field_load_result_n237, hxc_mossling.hxc_health > 0);
          hxc_vitals = hxc_tmp_call_result_n239;
        }
      }
      hxc_jumpQueued = false;
      hxc_accumulator = hxc_accumulator - 0.05;
      hxc_updateCount = hxc_i32_add_wrapping(hxc_updateCount, 1);
    }
    double hxc_eyeX = hxc_player.hxc_x;
    double hxc_eyeY = hxc_player.hxc_y + 1.62;
    double hxc_eyeZ = hxc_player.hxc_z;
    double hxc_tmp_load_result_n245 = hxc_eyeX;
    double hxc_tmp_load_result_n246 = hxc_eyeY;
    double hxc_tmp_load_result_n247 = hxc_eyeZ;
    double hxc_tmp_load_result_n248 = hxc_lookX;
    double hxc_tmp_load_result_n249 = hxc_lookY;
    struct hxc_caxecraft_domain_RaycastHit hxc_tmp_call_result_n251 = hxc_caxecraft_domain_VoxelRaycast_trace(hxc_cells, hxc_tmp_length_n1, hxc_tmp_load_result_n245, hxc_tmp_load_result_n246, hxc_tmp_load_result_n247, hxc_tmp_load_result_n248, hxc_tmp_load_result_n249, hxc_lookZ, 7.0);
    struct hxc_caxecraft_domain_RaycastHit hxc_hit = hxc_tmp_call_result_n251;
    bool hxc_tmp_load_result_n252 = hxc_captured;
    bool hxc_tmp_short_circuit_result_n121 = hxc_tmp_load_result_n252;
    if (hxc_tmp_load_result_n252)
    {
      hxc_tmp_short_circuit_result_n121 = !hxc_recapturedThisFrame;
    }
    bool hxc_tmp_short_circuit_load_result_n254 = hxc_tmp_short_circuit_result_n121;
    bool hxc_tmp_short_circuit_result_n122 = hxc_tmp_short_circuit_load_result_n254;
    if (hxc_tmp_short_circuit_load_result_n254)
    {
      hxc_tmp_short_circuit_result_n122 = hxc_primaryPressed_h0b61b55dc346;
    }
    if (hxc_tmp_short_circuit_result_n122)
    {
      if (hxc_vitals.hxc_health > 0)
      {
        if (hxc_selectedMode == 1)
        {
          bool hxc_tmp_call_result_n261 = hxc_caxecraft_gameplay_Inventory_selectedIs(hxc_inventory, hxc_caxecraft_gameplay_ItemKind_CopperSword);
          if (hxc_tmp_call_result_n261)
          {
            if (hxc_inventory.hxc_sword > 0)
            {
              struct hxc_caxecraft_gameplay_MosslingState hxc_tmp_load_result_n263 = hxc_mossling;
              double hxc_tmp_record_field_load_result_n264 = hxc_player.hxc_x;
              double hxc_tmp_record_field_load_result_n265 = hxc_player.hxc_z;
              double hxc_tmp_load_result_n266 = hxc_lookX;
              bool hxc_tmp_call_result_n268 = hxc_caxecraft_gameplay_Mossling_canStrike(hxc_tmp_load_result_n263, hxc_tmp_record_field_load_result_n264, hxc_tmp_record_field_load_result_n265, hxc_tmp_load_result_n266, hxc_lookZ);
              if (hxc_tmp_call_result_n268)
              {
                struct hxc_caxecraft_gameplay_MosslingState hxc_tmp_call_result_n270 = hxc_caxecraft_gameplay_Mossling_strike(hxc_mossling);
                hxc_mossling = hxc_tmp_call_result_n270;
                hxc_strikeHitFrames = 16;
                if (hxc_mossling.hxc_health <= 0)
                {
                  struct hxc_caxecraft_gameplay_BerryDropState hxc_tmp_call_result_n273 = hxc_caxecraft_gameplay_BerryDrop_fromDefeatedMossling(hxc_mossling);
                  hxc_berryDrop = hxc_tmp_call_result_n273;
                  hxc_enemyDefeatedFrames = 120;
                }
              }
            }
          }
          else
          {
            if (hxc_hit.hxc_hit)
            {
              int32_t hxc_tmp_record_field_load_result_n275 = hxc_hit.hxc_cellX;
              int32_t hxc_tmp_record_field_load_result_n276 = hxc_hit.hxc_cellY;
              struct hxc_caxecraft_domain_BlockCoord hxc_tmp_call_result_n278 = hxc_caxecraft_domain_World_coord(hxc_tmp_record_field_load_result_n275, hxc_tmp_record_field_load_result_n276, hxc_hit.hxc_cellZ);
              enum hxc_caxecraft_domain_BlockKind hxc_tmp_call_result_n279 = hxc_caxecraft_domain_World_query(hxc_cells, hxc_tmp_length_n1, hxc_tmp_call_result_n278);
              enum hxc_caxecraft_domain_BlockKind hxc_removedKind = hxc_tmp_call_result_n279;
              int32_t hxc_tmp_record_field_load_result_n280 = hxc_hit.hxc_cellX;
              int32_t hxc_tmp_record_field_load_result_n281 = hxc_hit.hxc_cellY;
              struct hxc_caxecraft_domain_BlockCoord hxc_tmp_call_result_n283 = hxc_caxecraft_domain_World_coord(hxc_tmp_record_field_load_result_n280, hxc_tmp_record_field_load_result_n281, hxc_hit.hxc_cellZ);
              bool hxc_tmp_call_result_n284 = hxc_caxecraft_domain_World_remove(hxc_cells, hxc_tmp_length_n1, hxc_tmp_call_result_n283);
              if (hxc_tmp_call_result_n284)
              {
                struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_load_result_n285 = hxc_inventory;
                struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_call_result_n287 = hxc_caxecraft_gameplay_Inventory_collectBlock(hxc_tmp_load_result_n285, hxc_removedKind);
                hxc_inventory = hxc_tmp_call_result_n287;
              }
            }
          }
        }
        else
        {
          if (hxc_hit.hxc_hit)
          {
            int32_t hxc_tmp_record_field_load_result_n289 = hxc_hit.hxc_cellX;
            int32_t hxc_tmp_record_field_load_result_n290 = hxc_hit.hxc_cellY;
            struct hxc_caxecraft_domain_BlockCoord hxc_tmp_call_result_n292 = hxc_caxecraft_domain_World_coord(hxc_tmp_record_field_load_result_n289, hxc_tmp_record_field_load_result_n290, hxc_hit.hxc_cellZ);
            hxc_caxecraft_domain_World_remove(hxc_cells, hxc_tmp_length_n1, hxc_tmp_call_result_n292);
          }
        }
      }
    }
    bool hxc_tmp_load_result_n293 = hxc_captured;
    bool hxc_tmp_short_circuit_result_n124 = hxc_tmp_load_result_n293;
    if (hxc_tmp_load_result_n293)
    {
      hxc_tmp_short_circuit_result_n124 = hxc_secondaryPressed_haf031cf26c58;
    }
    if (hxc_tmp_short_circuit_result_n124)
    {
      if (hxc_vitals.hxc_health > 0)
      {
        struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_load_result_n297 = hxc_inventory;
        int32_t hxc_tmp_call_result_n299 = hxc_caxecraft_gameplay_Recovery_decide(hxc_tmp_load_result_n297, hxc_vitals);
        int32_t hxc_recoveryDecision = hxc_tmp_call_result_n299;
        if (hxc_recoveryDecision != 0)
        {
          hxc_recoveryFeedback = hxc_recoveryDecision;
          hxc_recoveryFeedbackFrames = 90;
          int32_t hxc_tmp_load_result_n302 = hxc_recoveryDecision;
          struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_call_result_n304 = hxc_caxecraft_gameplay_Recovery_applyInventory(hxc_tmp_load_result_n302, hxc_inventory);
          hxc_inventory = hxc_tmp_call_result_n304;
          int32_t hxc_tmp_load_result_n305 = hxc_recoveryDecision;
          struct hxc_caxecraft_gameplay_PlayerVitalsState hxc_tmp_call_result_n307 = hxc_caxecraft_gameplay_Recovery_applyVitals(hxc_tmp_load_result_n305, hxc_vitals);
          hxc_vitals = hxc_tmp_call_result_n307;
        }
        else
        {
          if (hxc_hit.hxc_hit)
          {
            int32_t hxc_tmp_record_field_load_result_n309 = hxc_hit.hxc_previousX;
            int32_t hxc_tmp_record_field_load_result_n310 = hxc_hit.hxc_previousY;
            struct hxc_caxecraft_domain_BlockCoord hxc_tmp_call_result_n312 = hxc_caxecraft_domain_World_coord(hxc_tmp_record_field_load_result_n309, hxc_tmp_record_field_load_result_n310, hxc_hit.hxc_previousZ);
            struct hxc_caxecraft_domain_BlockCoord hxc_placement = hxc_tmp_call_result_n312;
            enum hxc_caxecraft_domain_BlockKind hxc_tmp_call_result_n314 = hxc_caxecraft_gameplay_Inventory_selectedBlock(hxc_inventory);
            enum hxc_caxecraft_domain_BlockKind hxc_selectedBlock = hxc_tmp_call_result_n314;
            struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_load_result_n315 = hxc_inventory;
            int32_t hxc_tmp_call_result_n317 = hxc_caxecraft_gameplay_Inventory_countAt(hxc_tmp_load_result_n315, hxc_inventory.hxc_selected);
            bool hxc_hasItem = hxc_tmp_call_result_n317 > 0;
            bool hxc_tmp_load_result_n318 = hxc_hasItem;
            bool hxc_tmp_short_circuit_result_n129 = !hxc_tmp_load_result_n318;
            if (!!hxc_tmp_load_result_n318)
            {
              bool hxc_tmp_call_result_n320 = hxc_caxecraft_domain_World_isPlaceable(hxc_selectedBlock);
              hxc_tmp_short_circuit_result_n129 = !hxc_tmp_call_result_n320;
            }
            bool hxc_tmp_short_circuit_load_result_n321 = hxc_tmp_short_circuit_result_n129;
            bool hxc_tmp_short_circuit_result_n130 = hxc_tmp_short_circuit_load_result_n321;
            if (!hxc_tmp_short_circuit_load_result_n321)
            {
              struct hxc_caxecraft_domain_PlayerState hxc_tmp_load_result_n322 = hxc_player;
              bool hxc_tmp_call_result_n324 = hxc_caxecraft_domain_PlayerPhysics_canPlaceAt(hxc_tmp_load_result_n322, hxc_placement);
              hxc_tmp_short_circuit_result_n130 = !hxc_tmp_call_result_n324;
            }
            bool hxc_tmp_short_circuit_load_result_n325 = hxc_tmp_short_circuit_result_n130;
            bool hxc_tmp_short_circuit_result_n131 = hxc_tmp_short_circuit_load_result_n325;
            if (!hxc_tmp_short_circuit_load_result_n325)
            {
              struct hxc_caxecraft_domain_BlockCoord hxc_tmp_load_result_n326 = hxc_placement;
              bool hxc_tmp_call_result_n328 = hxc_caxecraft_domain_World_place(hxc_cells, hxc_tmp_length_n1, hxc_tmp_load_result_n326, hxc_selectedBlock);
              hxc_tmp_short_circuit_result_n131 = !hxc_tmp_call_result_n328;
            }
            if (hxc_tmp_short_circuit_result_n131)
            {
              hxc_placementBlockedFrames = 60;
            }
            else
            {
              if (hxc_selectedMode == 1)
              {
                struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_call_result_n332 = hxc_caxecraft_gameplay_Inventory_consumeSelected(hxc_inventory);
                hxc_inventory = hxc_tmp_call_result_n332;
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
      struct hxc_caxecraft_gameplay_BerryDropState hxc_tmp_load_result_n336 = hxc_berryDrop;
      double hxc_tmp_record_field_load_result_n337 = hxc_player.hxc_x;
      double hxc_tmp_record_field_load_result_n338 = hxc_player.hxc_y;
      bool hxc_tmp_call_result_n340 = hxc_caxecraft_gameplay_BerryDrop_isInRange(hxc_tmp_load_result_n336, hxc_tmp_record_field_load_result_n337, hxc_tmp_record_field_load_result_n338, hxc_player.hxc_z);
      if (hxc_tmp_call_result_n340)
      {
        struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_load_result_n341 = hxc_inventory;
        int32_t hxc_tmp_call_result_n344 = hxc_caxecraft_gameplay_Inventory_acceptedAmount(hxc_tmp_load_result_n341, hxc_caxecraft_gameplay_ItemKind_Berries, hxc_berryDrop.hxc_amount);
        int32_t hxc_acceptedDrop = hxc_tmp_call_result_n344;
        if (hxc_acceptedDrop > 0)
        {
          struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_load_result_n346 = hxc_inventory;
          struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_call_result_n349 = hxc_caxecraft_gameplay_Inventory_collectItem(hxc_tmp_load_result_n346, hxc_caxecraft_gameplay_ItemKind_Berries, hxc_acceptedDrop);
          hxc_inventory = hxc_tmp_call_result_n349;
          struct hxc_caxecraft_gameplay_BerryDropState hxc_tmp_load_result_n350 = hxc_berryDrop;
          struct hxc_caxecraft_gameplay_BerryDropState hxc_tmp_call_result_n352 = hxc_caxecraft_gameplay_BerryDrop_collectAmount(hxc_tmp_load_result_n350, hxc_acceptedDrop);
          hxc_berryDrop = hxc_tmp_call_result_n352;
          hxc_pickupAmount = hxc_acceptedDrop;
          hxc_pickupFrames = 90;
        }
        else
        {
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
    if (hxc_pickupFrames > 0)
    {
      hxc_pickupFrames = hxc_i32_subtract_wrapping(hxc_pickupFrames, 1);
    }
    if (hxc_inventoryFullFrames > 0)
    {
      hxc_inventoryFullFrames = hxc_i32_subtract_wrapping(hxc_inventoryFullFrames, 1);
    }
    if (hxc_recoveryFeedbackFrames > 0)
    {
      hxc_recoveryFeedbackFrames = hxc_i32_subtract_wrapping(hxc_recoveryFeedbackFrames, 1);
    }
    double hxc_tmp_load_result_n364 = hxc_eyeX;
    double hxc_tmp_load_result_n365 = hxc_eyeY;
    double hxc_tmp_load_result_n366 = hxc_eyeZ;
    double hxc_tmp_load_result_n368 = hxc_eyeX;
    double hxc_tmp_load_result_n369 = hxc_lookX;
    double hxc_tmp_load_result_n370 = hxc_eyeY;
    double hxc_tmp_load_result_n371 = hxc_lookY;
    double hxc_tmp_load_result_n372 = hxc_eyeZ;
    struct Camera3D hxc_camera = (struct Camera3D){ .position = (struct Vector3){ .x = (float)hxc_tmp_load_result_n364, .y = (float)hxc_tmp_load_result_n365, .z = (float)hxc_tmp_load_result_n366 }, .target = (struct Vector3){ .x = (float)(hxc_tmp_load_result_n368 + hxc_tmp_load_result_n369), .y = (float)(hxc_tmp_load_result_n370 + hxc_tmp_load_result_n371), .z = (float)(hxc_tmp_load_result_n372 + hxc_lookZ) }, .up = (struct Vector3){ .x = (float)0.0, .y = (float)1.0, .z = (float)0.0 }, .fovy = (float)70.0, .projection = 0 };
    BeginDrawing();
    if (hxc_onTitle)
    {
      int32_t hxc_tmp_native_call_result_n378 = GetScreenWidth();
      int32_t hxc_width_hedea17c9b893 = (int32_t)hxc_tmp_native_call_result_n378;
      int32_t hxc_tmp_native_call_result_n379 = GetScreenHeight();
      int32_t hxc_height_ha2a0df528277 = (int32_t)hxc_tmp_native_call_result_n379;
      if (hxc_titleTextureReady)
      {
        struct Color hxc_this1_h1e10c3259526 = (struct Color){ .r = 255, .g = 255, .b = 255, .a = 255 };
        struct Texture hxc_tmp_load_result_n382 = hxc_titleTexture;
        double hxc_imageAspect = hxc_f64_divide_zero_safe((double)(int32_t)hxc_tmp_load_result_n382.width, (double)(int32_t)hxc_titleTexture.height);
        int32_t hxc_tmp_load_result_n386 = hxc_width_hedea17c9b893;
        double hxc_screenAspect = hxc_f64_divide_zero_safe((double)hxc_tmp_load_result_n386, (double)hxc_height_ha2a0df528277);
        double hxc_sourceX = 0.0;
        double hxc_sourceY = 0.0;
        double hxc_sourceWidth = (double)(int32_t)hxc_titleTexture.width;
        double hxc_sourceHeight = (double)(int32_t)hxc_titleTexture.height;
        double hxc_tmp_load_result_n392 = hxc_screenAspect;
        if (hxc_tmp_load_result_n392 > hxc_imageAspect)
        {
          struct Texture hxc_tmp_load_result_n394 = hxc_titleTexture;
          hxc_sourceHeight = hxc_f64_divide_zero_safe((double)(int32_t)hxc_tmp_load_result_n394.width, hxc_screenAspect);
          struct Texture hxc_tmp_load_result_n397 = hxc_titleTexture;
          hxc_sourceY = ((double)(int32_t)hxc_tmp_load_result_n397.height - hxc_sourceHeight) * 0.5;
        }
        else
        {
          struct Texture hxc_tmp_load_result_n400 = hxc_titleTexture;
          hxc_sourceWidth = (double)(int32_t)hxc_tmp_load_result_n400.height * hxc_screenAspect;
          struct Texture hxc_tmp_load_result_n403 = hxc_titleTexture;
          hxc_sourceX = ((double)(int32_t)hxc_tmp_load_result_n403.width - hxc_sourceWidth) * 0.5;
        }
        struct Texture hxc_tmp_load_result_n406 = hxc_titleTexture;
        double hxc_tmp_load_result_n407 = hxc_sourceX;
        double hxc_tmp_load_result_n408 = hxc_sourceY;
        double hxc_tmp_load_result_n409 = hxc_sourceWidth;
        double hxc_tmp_load_result_n410 = hxc_sourceHeight;
        int32_t hxc_tmp_load_result_n412 = hxc_width_hedea17c9b893;
        int32_t hxc_tmp_load_result_n413 = hxc_height_ha2a0df528277;
        DrawTexturePro(hxc_tmp_load_result_n406, (struct Rectangle){ .x = (float)hxc_tmp_load_result_n407, .y = (float)hxc_tmp_load_result_n408, .width = (float)hxc_tmp_load_result_n409, .height = (float)hxc_tmp_load_result_n410 }, (struct Rectangle){ .x = (float)0.0, .y = (float)0.0, .width = (float)((double)hxc_tmp_load_result_n412 + 0.0), .height = (float)((double)hxc_tmp_load_result_n413 + 0.0) }, (struct Vector2){ .x = (float)0.0, .y = (float)0.0 }, (float)0.0, hxc_this1_h1e10c3259526);
      }
      else
      {
        ClearBackground((struct Color){ .r = 126, .g = 190, .b = 201, .a = 255 });
      }
      struct Color hxc_this1_hb539e2bcacdf = (struct Color){ .r = 4, .g = 14, .b = 23, .a = 132 };
      int32_t hxc_tmp_load_result_n419 = hxc_width_hedea17c9b893;
      int32_t hxc_tmp_load_result_n420 = hxc_height_ha2a0df528277;
      DrawRectangle((int32_t)0, (int32_t)0, (int32_t)hxc_tmp_load_result_n419, (int32_t)hxc_tmp_load_result_n420, hxc_this1_hb539e2bcacdf);
      if (hxc_wordmarkTextureReady)
      {
        int32_t hxc_centerX = hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_width_hedea17c9b893, (double)2));
        struct Color hxc_this1_h15ba6270c219 = (struct Color){ .r = 255, .g = 255, .b = 255, .a = 255 };
        double hxc_scale = hxc_f64_divide_zero_safe((double)560, (double)(int32_t)hxc_wordmarkTexture.width);
        double hxc_heightScale = hxc_f64_divide_zero_safe((double)132, (double)(int32_t)hxc_wordmarkTexture.height);
        double hxc_tmp_load_result_n429 = hxc_heightScale;
        if (hxc_tmp_load_result_n429 < hxc_scale)
        {
          hxc_scale = hxc_heightScale;
        }
        struct Texture hxc_tmp_load_result_n432 = hxc_wordmarkTexture;
        double hxc_width_h69bfd750f525 = (double)(int32_t)hxc_tmp_load_result_n432.width * hxc_scale;
        struct Texture hxc_tmp_load_result_n435 = hxc_wordmarkTexture;
        double hxc_height_hfab986491444 = (double)(int32_t)hxc_tmp_load_result_n435.height * hxc_scale;
        double hxc_width1 = (double)(int32_t)hxc_wordmarkTexture.width + 0.0;
        double hxc_height1 = (double)(int32_t)hxc_wordmarkTexture.height + 0.0;
        struct Texture hxc_tmp_load_result_n442 = hxc_wordmarkTexture;
        double hxc_tmp_load_result_n443 = hxc_width1;
        double hxc_tmp_load_result_n444 = hxc_height1;
        int32_t hxc_tmp_load_result_n446 = hxc_centerX;
        double hxc_tmp_load_result_n447 = hxc_width_h69bfd750f525;
        double hxc_tmp_load_result_n448 = hxc_width_h69bfd750f525;
        double hxc_tmp_load_result_n449 = hxc_height_hfab986491444;
        DrawTexturePro(hxc_tmp_load_result_n442, (struct Rectangle){ .x = (float)0.0, .y = (float)0.0, .width = (float)hxc_tmp_load_result_n443, .height = (float)hxc_tmp_load_result_n444 }, (struct Rectangle){ .x = (float)((double)hxc_tmp_load_result_n446 - hxc_tmp_load_result_n447 * 0.5), .y = (float)28.0, .width = (float)hxc_tmp_load_result_n448, .height = (float)hxc_tmp_load_result_n449 }, (struct Vector2){ .x = (float)0.0, .y = (float)0.0 }, (float)0.0, hxc_this1_h15ba6270c219);
      }
      else
      {
        int32_t hxc_x_hf725d5fcbdd6 = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_width_hedea17c9b893, (double)2)), 92);
        struct Color hxc_this1_h002fc1f66805 = (struct Color){ .r = 242, .g = 249, .b = 245, .a = 255 };
        int32_t hxc_tmp_load_result_n455 = hxc_x_hf725d5fcbdd6;
        DrawText("CAXECRAFT", (int32_t)hxc_tmp_load_result_n455, (int32_t)52, (int32_t)34, hxc_this1_h002fc1f66805);
      }
      int32_t hxc_firstTop = hxc_i32_subtract_wrapping(hxc_height_ha2a0df528277, 225);
      if (hxc_language == 1)
      {
        int32_t hxc_left_h3bfc016f0156 = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_width_hedea17c9b893, (double)2)), 180);
        if (hxc_selectedMode == 0)
        {
          struct Color hxc_this1_hd91a64420a0e = (struct Color){ .r = 16, .g = 88, .b = 102, .a = 232 };
          int32_t hxc_tmp_load_result_n462 = hxc_left_h3bfc016f0156;
          int32_t hxc_tmp_load_result_n463 = hxc_firstTop;
          DrawRectangle((int32_t)hxc_tmp_load_result_n462, (int32_t)hxc_tmp_load_result_n463, (int32_t)360, (int32_t)54, hxc_this1_hd91a64420a0e);
          struct Color hxc_this1_h6986c66fd29d = (struct Color){ .r = 255, .g = 190, .b = 55, .a = 255 };
          int32_t hxc_tmp_load_result_n466 = hxc_left_h3bfc016f0156;
          int32_t hxc_tmp_load_result_n467 = hxc_firstTop;
          DrawRectangleLines((int32_t)hxc_tmp_load_result_n466, (int32_t)hxc_tmp_load_result_n467, (int32_t)360, (int32_t)54, hxc_this1_h6986c66fd29d);
        }
        else
        {
          struct Color hxc_this1_h742952063b58 = (struct Color){ .r = 6, .g = 26, .b = 36, .a = 218 };
          int32_t hxc_tmp_load_result_n470 = hxc_left_h3bfc016f0156;
          int32_t hxc_tmp_load_result_n471 = hxc_firstTop;
          DrawRectangle((int32_t)hxc_tmp_load_result_n470, (int32_t)hxc_tmp_load_result_n471, (int32_t)360, (int32_t)54, hxc_this1_h742952063b58);
          struct Color hxc_this1_h07780785cb2c = (struct Color){ .r = 92, .g = 194, .b = 188, .a = 255 };
          int32_t hxc_tmp_load_result_n474 = hxc_left_h3bfc016f0156;
          int32_t hxc_tmp_load_result_n475 = hxc_firstTop;
          DrawRectangleLines((int32_t)hxc_tmp_load_result_n474, (int32_t)hxc_tmp_load_result_n475, (int32_t)360, (int32_t)54, hxc_this1_h07780785cb2c);
        }
        struct Color hxc_this1_h17bbeb228c01 = (struct Color){ .r = 242, .g = 249, .b = 245, .a = 255 };
        int32_t hxc_tmp_load_result_n478 = hxc_left_h3bfc016f0156;
        int32_t hxc_tmp_load_result_n479 = hxc_firstTop;
        DrawText("MODO CREATIVO", (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n478, 28), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n479, 16), (int32_t)21, hxc_this1_h17bbeb228c01);
        int32_t hxc_top_hd68c8076b636 = hxc_i32_add_wrapping(hxc_i32_add_wrapping(hxc_firstTop, 54), 14);
        int32_t hxc_left_h59e66e62cca2 = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_width_hedea17c9b893, (double)2)), 180);
        if (hxc_selectedMode == 1)
        {
          struct Color hxc_this1_hf8da672797dd = (struct Color){ .r = 16, .g = 88, .b = 102, .a = 232 };
          int32_t hxc_tmp_load_result_n485 = hxc_left_h59e66e62cca2;
          int32_t hxc_tmp_load_result_n486 = hxc_top_hd68c8076b636;
          DrawRectangle((int32_t)hxc_tmp_load_result_n485, (int32_t)hxc_tmp_load_result_n486, (int32_t)360, (int32_t)54, hxc_this1_hf8da672797dd);
          struct Color hxc_this1_h79577861e1b3 = (struct Color){ .r = 255, .g = 190, .b = 55, .a = 255 };
          int32_t hxc_tmp_load_result_n489 = hxc_left_h59e66e62cca2;
          int32_t hxc_tmp_load_result_n490 = hxc_top_hd68c8076b636;
          DrawRectangleLines((int32_t)hxc_tmp_load_result_n489, (int32_t)hxc_tmp_load_result_n490, (int32_t)360, (int32_t)54, hxc_this1_h79577861e1b3);
        }
        else
        {
          struct Color hxc_this1_he648b63556d3 = (struct Color){ .r = 6, .g = 26, .b = 36, .a = 218 };
          int32_t hxc_tmp_load_result_n493 = hxc_left_h59e66e62cca2;
          int32_t hxc_tmp_load_result_n494 = hxc_top_hd68c8076b636;
          DrawRectangle((int32_t)hxc_tmp_load_result_n493, (int32_t)hxc_tmp_load_result_n494, (int32_t)360, (int32_t)54, hxc_this1_he648b63556d3);
          struct Color hxc_this1_hc6235c1aaa0e = (struct Color){ .r = 92, .g = 194, .b = 188, .a = 255 };
          int32_t hxc_tmp_load_result_n497 = hxc_left_h59e66e62cca2;
          int32_t hxc_tmp_load_result_n498 = hxc_top_hd68c8076b636;
          DrawRectangleLines((int32_t)hxc_tmp_load_result_n497, (int32_t)hxc_tmp_load_result_n498, (int32_t)360, (int32_t)54, hxc_this1_hc6235c1aaa0e);
        }
        struct Color hxc_this1_h3c3e9e95b1c8 = (struct Color){ .r = 242, .g = 249, .b = 245, .a = 255 };
        int32_t hxc_tmp_load_result_n501 = hxc_left_h59e66e62cca2;
        int32_t hxc_tmp_load_result_n502 = hxc_top_hd68c8076b636;
        DrawText("AVENTURA", (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n501, 28), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n502, 16), (int32_t)21, hxc_this1_h3c3e9e95b1c8);
        int32_t hxc_x_h58f57733077f = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_width_hedea17c9b893, (double)2)), 270);
        struct Color hxc_this1_h45e7cd6f2d61 = (struct Color){ .r = 229, .g = 241, .b = 235, .a = 255 };
        int32_t hxc_tmp_load_result_n506 = hxc_x_h58f57733077f;
        int32_t hxc_tmp_load_result_n507 = hxc_height_ha2a0df528277;
        DrawText("FLECHAS / RATON PARA ELEGIR   ENTER PARA JUGAR   L: ENGLISH", (int32_t)hxc_tmp_load_result_n506, (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n507, 58), (int32_t)16, hxc_this1_h45e7cd6f2d61);
      }
      else
      {
        int32_t hxc_left_hf9c86d4b154b = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_width_hedea17c9b893, (double)2)), 180);
        if (hxc_selectedMode == 0)
        {
          struct Color hxc_this1_h922c566f2fec = (struct Color){ .r = 16, .g = 88, .b = 102, .a = 232 };
          int32_t hxc_tmp_load_result_n512 = hxc_left_hf9c86d4b154b;
          int32_t hxc_tmp_load_result_n513 = hxc_firstTop;
          DrawRectangle((int32_t)hxc_tmp_load_result_n512, (int32_t)hxc_tmp_load_result_n513, (int32_t)360, (int32_t)54, hxc_this1_h922c566f2fec);
          struct Color hxc_this1_h40172815f085 = (struct Color){ .r = 255, .g = 190, .b = 55, .a = 255 };
          int32_t hxc_tmp_load_result_n516 = hxc_left_hf9c86d4b154b;
          int32_t hxc_tmp_load_result_n517 = hxc_firstTop;
          DrawRectangleLines((int32_t)hxc_tmp_load_result_n516, (int32_t)hxc_tmp_load_result_n517, (int32_t)360, (int32_t)54, hxc_this1_h40172815f085);
        }
        else
        {
          struct Color hxc_this1_h82029f00bf2f = (struct Color){ .r = 6, .g = 26, .b = 36, .a = 218 };
          int32_t hxc_tmp_load_result_n520 = hxc_left_hf9c86d4b154b;
          int32_t hxc_tmp_load_result_n521 = hxc_firstTop;
          DrawRectangle((int32_t)hxc_tmp_load_result_n520, (int32_t)hxc_tmp_load_result_n521, (int32_t)360, (int32_t)54, hxc_this1_h82029f00bf2f);
          struct Color hxc_this1_hc18468f5e813 = (struct Color){ .r = 92, .g = 194, .b = 188, .a = 255 };
          int32_t hxc_tmp_load_result_n524 = hxc_left_hf9c86d4b154b;
          int32_t hxc_tmp_load_result_n525 = hxc_firstTop;
          DrawRectangleLines((int32_t)hxc_tmp_load_result_n524, (int32_t)hxc_tmp_load_result_n525, (int32_t)360, (int32_t)54, hxc_this1_hc18468f5e813);
        }
        struct Color hxc_this1_hfe494e7f13a4 = (struct Color){ .r = 242, .g = 249, .b = 245, .a = 255 };
        int32_t hxc_tmp_load_result_n528 = hxc_left_hf9c86d4b154b;
        int32_t hxc_tmp_load_result_n529 = hxc_firstTop;
        DrawText("CREATIVE MODE", (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n528, 28), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n529, 16), (int32_t)21, hxc_this1_hfe494e7f13a4);
        int32_t hxc_top_hdb34d0e2aed8 = hxc_i32_add_wrapping(hxc_i32_add_wrapping(hxc_firstTop, 54), 14);
        int32_t hxc_left_h967219f7779d = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_width_hedea17c9b893, (double)2)), 180);
        if (hxc_selectedMode == 1)
        {
          struct Color hxc_this1_h8d4a171cde40 = (struct Color){ .r = 16, .g = 88, .b = 102, .a = 232 };
          int32_t hxc_tmp_load_result_n535 = hxc_left_h967219f7779d;
          int32_t hxc_tmp_load_result_n536 = hxc_top_hdb34d0e2aed8;
          DrawRectangle((int32_t)hxc_tmp_load_result_n535, (int32_t)hxc_tmp_load_result_n536, (int32_t)360, (int32_t)54, hxc_this1_h8d4a171cde40);
          struct Color hxc_this1_he31d35a0929d = (struct Color){ .r = 255, .g = 190, .b = 55, .a = 255 };
          int32_t hxc_tmp_load_result_n539 = hxc_left_h967219f7779d;
          int32_t hxc_tmp_load_result_n540 = hxc_top_hdb34d0e2aed8;
          DrawRectangleLines((int32_t)hxc_tmp_load_result_n539, (int32_t)hxc_tmp_load_result_n540, (int32_t)360, (int32_t)54, hxc_this1_he31d35a0929d);
        }
        else
        {
          struct Color hxc_this1_hdf3d0dd71701 = (struct Color){ .r = 6, .g = 26, .b = 36, .a = 218 };
          int32_t hxc_tmp_load_result_n543 = hxc_left_h967219f7779d;
          int32_t hxc_tmp_load_result_n544 = hxc_top_hdb34d0e2aed8;
          DrawRectangle((int32_t)hxc_tmp_load_result_n543, (int32_t)hxc_tmp_load_result_n544, (int32_t)360, (int32_t)54, hxc_this1_hdf3d0dd71701);
          struct Color hxc_this1_h5603729fd84a = (struct Color){ .r = 92, .g = 194, .b = 188, .a = 255 };
          int32_t hxc_tmp_load_result_n547 = hxc_left_h967219f7779d;
          int32_t hxc_tmp_load_result_n548 = hxc_top_hdb34d0e2aed8;
          DrawRectangleLines((int32_t)hxc_tmp_load_result_n547, (int32_t)hxc_tmp_load_result_n548, (int32_t)360, (int32_t)54, hxc_this1_h5603729fd84a);
        }
        struct Color hxc_this1_hc32b20afb966 = (struct Color){ .r = 242, .g = 249, .b = 245, .a = 255 };
        int32_t hxc_tmp_load_result_n551 = hxc_left_h967219f7779d;
        int32_t hxc_tmp_load_result_n552 = hxc_top_hdb34d0e2aed8;
        DrawText("ADVENTURE", (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n551, 28), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n552, 16), (int32_t)21, hxc_this1_hc32b20afb966);
        int32_t hxc_x_hc162a90cba74 = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_width_hedea17c9b893, (double)2)), 270);
        struct Color hxc_this1_hb9cd5bc2da99 = (struct Color){ .r = 229, .g = 241, .b = 235, .a = 255 };
        int32_t hxc_tmp_load_result_n556 = hxc_x_hc162a90cba74;
        int32_t hxc_tmp_load_result_n557 = hxc_height_ha2a0df528277;
        DrawText("ARROWS / MOUSE TO CHOOSE   ENTER TO PLAY   L: ESPANOL", (int32_t)hxc_tmp_load_result_n556, (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n557, 58), (int32_t)16, hxc_this1_hb9cd5bc2da99);
      }
      if (hxc_selectedMode == 1)
      {
        if (hxc_language == 1)
        {
          int32_t hxc_x_hafc734996eea = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_width_hedea17c9b893, (double)2)), 210);
          struct Color hxc_this1_h614bb4532ff9 = (struct Color){ .r = 255, .g = 205, .b = 91, .a = 255 };
          int32_t hxc_tmp_load_result_n563 = hxc_x_hafc734996eea;
          int32_t hxc_tmp_load_result_n564 = hxc_firstTop;
          DrawText("SALVA A CEESH. ENCUENTRA A IVVY. DETEN A BROWSER.", (int32_t)hxc_tmp_load_result_n563, (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n564, 36), (int32_t)17, hxc_this1_h614bb4532ff9);
        }
        else
        {
          int32_t hxc_x_haf9f2930fbe3 = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_width_hedea17c9b893, (double)2)), 210);
          struct Color hxc_this1_hb0ed34032af3 = (struct Color){ .r = 255, .g = 205, .b = 91, .a = 255 };
          int32_t hxc_tmp_load_result_n568 = hxc_x_haf9f2930fbe3;
          int32_t hxc_tmp_load_result_n569 = hxc_firstTop;
          DrawText("SAVE CEESH. FIND IVVY. STOP BROWSER.", (int32_t)hxc_tmp_load_result_n568, (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n569, 36), (int32_t)17, hxc_this1_hb0ed34032af3);
        }
      }
    }
    else
    {
      ClearBackground((struct Color){ .r = 126, .g = 190, .b = 201, .a = 255 });
      BeginMode3D(hxc_camera);
      double hxc_tmp_record_field_load_result_n573 = hxc_player.hxc_x;
      struct hxc_caxecraft_app_Main_RenderCounters hxc_tmp_call_result_n575 = hxc_caxecraft_app_Main_drawWorld(hxc_cells, hxc_tmp_length_n1, hxc_tmp_record_field_load_result_n573, hxc_player.hxc_z);
      struct hxc_caxecraft_app_Main_RenderCounters hxc_renderCounters = hxc_tmp_call_result_n575;
      struct hxc_caxecraft_gameplay_GuideState hxc_tmp_load_result_n576 = hxc_guide;
      struct hxc_caxecraft_gameplay_MosslingState hxc_tmp_load_result_n577 = hxc_mossling;
      hxc_caxecraft_app_Main_drawActors(hxc_tmp_load_result_n576, hxc_tmp_load_result_n577, hxc_berryDrop);
      if (hxc_hit.hxc_hit)
      {
        double hxc_y = (double)hxc_hit.hxc_cellY + 0.5;
        double hxc_z = (double)hxc_hit.hxc_cellZ + 0.5;
        int32_t hxc_tmp_record_field_load_result_n582 = hxc_hit.hxc_cellX;
        double hxc_tmp_load_result_n583 = hxc_y;
        DrawCubeWires((struct Vector3){ .x = (float)((double)hxc_tmp_record_field_load_result_n582 + 0.5), .y = (float)hxc_tmp_load_result_n583, .z = (float)hxc_z }, (float)1.04, (float)1.04, (float)1.04, (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 });
      }
      EndMode3D();
      int32_t hxc_tmp_record_field_load_result_n587 = hxc_renderCounters.hxc_visible;
      int32_t hxc_tmp_record_field_load_result_n588 = hxc_renderCounters.hxc_drawCalls;
      int32_t hxc_tmp_load_result_n589 = hxc_frameCount;
      int32_t hxc_tmp_load_result_n590 = hxc_updateCount;
      bool hxc_tmp_load_result_n591 = hxc_paused;
      bool hxc_tmp_load_result_n592 = hxc_captured;
      int32_t hxc_tmp_load_result_n593 = hxc_placementBlockedFrames;
      struct hxc_caxecraft_domain_RaycastHit hxc_tmp_load_result_n594 = hxc_hit;
      double hxc_tmp_record_field_load_result_n595 = hxc_player.hxc_x;
      double hxc_tmp_record_field_load_result_n596 = hxc_player.hxc_z;
      int32_t hxc_tmp_load_result_n597 = hxc_selectedMode;
      int32_t hxc_tmp_load_result_n598 = hxc_language;
      struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_load_result_n599 = hxc_inventory;
      struct hxc_caxecraft_gameplay_GuideState hxc_tmp_load_result_n600 = hxc_guide;
      struct hxc_caxecraft_gameplay_MosslingState hxc_tmp_load_result_n601 = hxc_mossling;
      struct hxc_caxecraft_gameplay_PlayerVitalsState hxc_tmp_load_result_n602 = hxc_vitals;
      int32_t hxc_tmp_load_result_n603 = hxc_strikeHitFrames;
      int32_t hxc_tmp_load_result_n604 = hxc_enemyDefeatedFrames;
      int32_t hxc_tmp_load_result_n605 = hxc_pickupFrames;
      int32_t hxc_tmp_load_result_n606 = hxc_pickupAmount;
      int32_t hxc_tmp_load_result_n607 = hxc_inventoryFullFrames;
      int32_t hxc_tmp_load_result_n608 = hxc_recoveryFeedback;
      int32_t hxc_tmp_load_result_n609 = hxc_recoveryFeedbackFrames;
      struct Texture hxc_tmp_load_result_n610 = hxc_hudTexture;
      bool hxc_tmp_load_result_n611 = hxc_hudTextureReady;
      struct Texture hxc_tmp_load_result_n612 = hxc_itemTexture;
      hxc_caxecraft_app_Main_drawHud(hxc_tmp_record_field_load_result_n587, hxc_tmp_record_field_load_result_n588, hxc_tmp_load_result_n589, hxc_tmp_load_result_n590, hxc_tmp_load_result_n591, hxc_tmp_load_result_n592, hxc_tmp_load_result_n593 > 0, hxc_tmp_load_result_n594, hxc_tmp_record_field_load_result_n595, hxc_tmp_record_field_load_result_n596, hxc_tmp_load_result_n597, hxc_tmp_load_result_n598, hxc_tmp_load_result_n599, hxc_tmp_load_result_n600, hxc_tmp_load_result_n601, hxc_tmp_load_result_n602, hxc_tmp_load_result_n603 > 0, hxc_tmp_load_result_n604 > 0, hxc_tmp_load_result_n605 > 0, hxc_tmp_load_result_n606, hxc_tmp_load_result_n607 > 0, hxc_tmp_load_result_n608, hxc_tmp_load_result_n609 > 0, hxc_tmp_load_result_n610, hxc_tmp_load_result_n611, hxc_tmp_load_result_n612, hxc_itemTextureReady);
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
