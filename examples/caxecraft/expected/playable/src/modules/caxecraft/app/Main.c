#include "hxc/program.h"

void hxc_caxecraft_app_Main_drawActors(struct hxc_caxecraft_gameplay_GuideState hxc_guide, struct hxc_caxecraft_gameplay_MosslingState hxc_mossling)
{
  double hxc_y_h4468788ea7e1 = hxc_guide.hxc_y + 0.54;
  double hxc_z_h86dbd9b4a9d3 = hxc_guide.hxc_z;
  double hxc_tmp_load_result_n3 = hxc_y_h4468788ea7e1;
  DrawCube((struct Vector3){ .x = (float)hxc_guide.hxc_x, .y = (float)hxc_tmp_load_result_n3, .z = (float)hxc_z_h86dbd9b4a9d3 }, (float)0.50, (float)0.86, (float)0.42, (struct Color){ .r = 42, .g = 150, .b = 160, .a = 255 });
  double hxc_y_h28fe74fb9f76 = hxc_guide.hxc_y + 1.18;
  double hxc_z_h710ddba9c139 = hxc_guide.hxc_z;
  double hxc_tmp_load_result_n10 = hxc_y_h28fe74fb9f76;
  DrawCube((struct Vector3){ .x = (float)hxc_guide.hxc_x, .y = (float)hxc_tmp_load_result_n10, .z = (float)hxc_z_h710ddba9c139 }, (float)0.44, (float)0.44, (float)0.44, (struct Color){ .r = 205, .g = 139, .b = 88, .a = 255 });
  double hxc_y_hcd64767a3726 = hxc_guide.hxc_y + 1.41;
  double hxc_z_h9e2e4f56b16f = hxc_guide.hxc_z;
  double hxc_tmp_load_result_n17 = hxc_y_hcd64767a3726;
  DrawCube((struct Vector3){ .x = (float)hxc_guide.hxc_x, .y = (float)hxc_tmp_load_result_n17, .z = (float)hxc_z_h9e2e4f56b16f }, (float)0.48, (float)0.16, (float)0.48, (struct Color){ .r = 62, .g = 40, .b = 47, .a = 255 });
  double hxc_y_hd237e58016fe = hxc_mossling.hxc_y + 0.30;
  double hxc_z_h7a017ff3a2db = hxc_mossling.hxc_z;
  double hxc_tmp_load_result_n24 = hxc_y_hd237e58016fe;
  DrawCube((struct Vector3){ .x = (float)hxc_mossling.hxc_x, .y = (float)hxc_tmp_load_result_n24, .z = (float)hxc_z_h7a017ff3a2db }, (float)0.70, (float)0.54, (float)0.70, (struct Color){ .r = 74, .g = 119, .b = 67, .a = 255 });
  double hxc_y_h0c0f53391483 = hxc_mossling.hxc_y + 0.66;
  double hxc_z_hfd85640cbd88 = hxc_mossling.hxc_z;
  double hxc_tmp_load_result_n31 = hxc_y_h0c0f53391483;
  DrawCube((struct Vector3){ .x = (float)hxc_mossling.hxc_x, .y = (float)hxc_tmp_load_result_n31, .z = (float)hxc_z_hfd85640cbd88 }, (float)0.50, (float)0.34, (float)0.50, (struct Color){ .r = 157, .g = 190, .b = 82, .a = 255 });
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

void hxc_caxecraft_app_Main_drawHud(int32_t hxc_visible, int32_t hxc_drawCalls, int32_t hxc_frames, int32_t hxc_updates, bool hxc_paused, bool hxc_captured, bool hxc_placementBlocked, struct hxc_caxecraft_domain_RaycastHit hxc_hit, double hxc_playerX, double hxc_playerZ, int32_t hxc_mode, int32_t hxc_language, struct hxc_caxecraft_gameplay_InventoryState hxc_inventory, struct hxc_caxecraft_gameplay_GuideState hxc_guide, struct hxc_caxecraft_gameplay_MosslingState hxc_mossling, struct Texture hxc_hudTexture, bool hxc_hudTextureReady, struct Texture hxc_itemTexture, bool hxc_itemTextureReady)
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
  struct Color hxc_this1_hb1fd2257577c = (struct Color){ .r = 15, .g = 27, .b = 39, .a = 220 };
  DrawRectangle((int32_t)18, (int32_t)18, (int32_t)460, (int32_t)108, hxc_this1_hb1fd2257577c);
  struct Color hxc_this1_h6b5a611609a1 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
  DrawRectangleLines((int32_t)18, (int32_t)18, (int32_t)460, (int32_t)108, hxc_this1_h6b5a611609a1);
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
  int32_t hxc_tmp_load_result_n42 = hxc_height;
  DrawText("WASD MOVE  1-8/WHEEL ITEMS  SPACE JUMP  E TALK  LMB MINE  RMB USE  ESC PAUSE  Q QUIT", (int32_t)20, (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n42, 22), (int32_t)14, hxc_text);
  if (hxc_mode == 1)
  {
    if (hxc_language == 1)
    {
      struct Color hxc_this1_h8c731a113d4c = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
      DrawText("AVENTURA: PROLOGO EN CONSTRUCCION", (int32_t)32, (int32_t)110, (int32_t)14, hxc_this1_h8c731a113d4c);
    }
    else
    {
      struct Color hxc_this1_had2c5fbe1dc9 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
      DrawText("ADVENTURE: PROLOGUE IN PROGRESS", (int32_t)32, (int32_t)110, (int32_t)14, hxc_this1_had2c5fbe1dc9);
    }
  }
  bool hxc_tmp_call_result_n48 = hxc_caxecraft_gameplay_GuideNpc_isInRange(hxc_guide, hxc_playerX, hxc_playerZ);
  if (hxc_tmp_call_result_n48)
  {
    struct Color hxc_this1_h1ddc3e057a66 = (struct Color){ .r = 15, .g = 27, .b = 39, .a = 220 };
    int32_t hxc_tmp_load_result_n50 = hxc_centerX;
    int32_t hxc_tmp_load_result_n51 = hxc_centerY;
    DrawRectangle((int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n50, 260), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n51, 54), (int32_t)520, (int32_t)60, hxc_this1_h1ddc3e057a66);
    int32_t hxc_code_h9746a675e925 = hxc_guide.hxc_phaseCode;
    int32_t hxc_tmp_load_result_n54 = hxc_code_h9746a675e925;
    int32_t hxc_tmp_conditional_result_n30 = 0;
    if (hxc_tmp_load_result_n54 == 1)
    {
      hxc_tmp_conditional_result_n30 = 1;
    }
    else
    {
      int32_t hxc_tmp_load_result_n55 = hxc_code_h9746a675e925;
      int32_t hxc_tmp_conditional_result_n31 = 0;
      if (hxc_tmp_load_result_n55 == 2)
      {
        hxc_tmp_conditional_result_n31 = 2;
      }
      else
      {
        hxc_tmp_conditional_result_n31 = 0;
      }
      hxc_tmp_conditional_result_n30 = hxc_tmp_conditional_result_n31;
    }
    if (hxc_tmp_conditional_result_n30 == 0)
    {
      if (hxc_language == 1)
      {
        int32_t hxc_tmp_load_result_n58 = hxc_centerX;
        int32_t hxc_tmp_load_result_n59 = hxc_centerY;
        DrawText("E  HABLAR CON NIA", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n58, 92), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n59, 74), (int32_t)18, hxc_text);
      }
      else
      {
        int32_t hxc_tmp_load_result_n61 = hxc_centerX;
        int32_t hxc_tmp_load_result_n62 = hxc_centerY;
        DrawText("E  TALK TO NIA", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n61, 74), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n62, 74), (int32_t)18, hxc_text);
      }
    }
    else
    {
      int32_t hxc_code_hfac721a636f6 = hxc_guide.hxc_phaseCode;
      int32_t hxc_tmp_load_result_n65 = hxc_code_hfac721a636f6;
      int32_t hxc_tmp_conditional_result_n33 = 0;
      if (hxc_tmp_load_result_n65 == 1)
      {
        hxc_tmp_conditional_result_n33 = 1;
      }
      else
      {
        int32_t hxc_tmp_load_result_n66 = hxc_code_hfac721a636f6;
        int32_t hxc_tmp_conditional_result_n34 = 0;
        if (hxc_tmp_load_result_n66 == 2)
        {
          hxc_tmp_conditional_result_n34 = 2;
        }
        else
        {
          hxc_tmp_conditional_result_n34 = 0;
        }
        hxc_tmp_conditional_result_n33 = hxc_tmp_conditional_result_n34;
      }
      if (hxc_tmp_conditional_result_n33 == 1)
      {
        if (hxc_language == 1)
        {
          int32_t hxc_tmp_load_result_n69 = hxc_centerX;
          int32_t hxc_tmp_load_result_n70 = hxc_centerY;
          DrawText("NIA: EL BOSQUE TE ESCUCHA. E: REGALO", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n69, 220), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n70, 74), (int32_t)16, hxc_text);
        }
        else
        {
          int32_t hxc_tmp_load_result_n72 = hxc_centerX;
          int32_t hxc_tmp_load_result_n73 = hxc_centerY;
          DrawText("NIA: THE GROVE LISTENS. E: A SMALL GIFT", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n72, 225), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n73, 74), (int32_t)16, hxc_text);
        }
      }
      else
      {
        if (hxc_language == 1)
        {
          int32_t hxc_tmp_load_result_n75 = hxc_centerX;
          int32_t hxc_tmp_load_result_n76 = hxc_centerY;
          DrawText("NIA: BAYAS PARA EL CAMINO, HAXIRIO", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n75, 205), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n76, 74), (int32_t)16, hxc_text);
        }
        else
        {
          int32_t hxc_tmp_load_result_n78 = hxc_centerX;
          int32_t hxc_tmp_load_result_n79 = hxc_centerY;
          DrawText("NIA: BERRIES FOR THE ROAD, HAXIRIO", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n78, 205), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n79, 74), (int32_t)16, hxc_text);
        }
      }
    }
  }
  int32_t hxc_code_he3b5802ebc43 = hxc_mossling.hxc_modeCode;
  int32_t hxc_tmp_load_result_n82 = hxc_code_he3b5802ebc43;
  int32_t hxc_tmp_conditional_result_n36 = 0;
  if (hxc_tmp_load_result_n82 == 1)
  {
    hxc_tmp_conditional_result_n36 = 1;
  }
  else
  {
    int32_t hxc_tmp_load_result_n83 = hxc_code_he3b5802ebc43;
    int32_t hxc_tmp_conditional_result_n37 = 0;
    if (hxc_tmp_load_result_n83 == 2)
    {
      hxc_tmp_conditional_result_n37 = 2;
    }
    else
    {
      hxc_tmp_conditional_result_n37 = 0;
    }
    hxc_tmp_conditional_result_n36 = hxc_tmp_conditional_result_n37;
  }
  if (hxc_tmp_conditional_result_n36 == 1)
  {
    if (hxc_language == 1)
    {
      struct Color hxc_this1_hfc2b876fd41c = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
      int32_t hxc_tmp_load_result_n87 = hxc_width;
      DrawText("MUSGUITO ALERTA", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n87, 180), (int32_t)28, (int32_t)16, hxc_this1_hfc2b876fd41c);
    }
    else
    {
      struct Color hxc_this1_h93dfd28f492d = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
      int32_t hxc_tmp_load_result_n90 = hxc_width;
      DrawText("MOSSLING ALERT", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n90, 180), (int32_t)28, (int32_t)16, hxc_this1_h93dfd28f492d);
    }
  }
  if (hxc_paused)
  {
    struct Color hxc_this1_h13400c87e7b9 = (struct Color){ .r = 15, .g = 27, .b = 39, .a = 220 };
    int32_t hxc_tmp_load_result_n93 = hxc_centerX;
    int32_t hxc_tmp_load_result_n94 = hxc_centerY;
    DrawRectangle((int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n93, 170), (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n94, 48), (int32_t)340, (int32_t)96, hxc_this1_h13400c87e7b9);
    struct Color hxc_this1_h77ee9daecda8 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
    int32_t hxc_tmp_load_result_n97 = hxc_centerX;
    int32_t hxc_tmp_load_result_n98 = hxc_centerY;
    DrawRectangleLines((int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n97, 170), (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n98, 48), (int32_t)340, (int32_t)96, hxc_this1_h77ee9daecda8);
    int32_t hxc_tmp_load_result_n100 = hxc_centerX;
    int32_t hxc_tmp_load_result_n101 = hxc_centerY;
    DrawText("PAUSED", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n100, 48), (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n101, 30), (int32_t)24, hxc_text);
    int32_t hxc_tmp_load_result_n103 = hxc_centerX;
    int32_t hxc_tmp_load_result_n104 = hxc_centerY;
    DrawText("CLICK TO CAPTURE  //  Q TO QUIT", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n103, 145), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n104, 8), (int32_t)16, hxc_text);
  }
  else
  {
    if (hxc_placementBlocked)
    {
      struct Color hxc_this1_h9213d105e89d = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
      int32_t hxc_tmp_load_result_n107 = hxc_centerX;
      int32_t hxc_tmp_load_result_n108 = hxc_centerY;
      DrawText("PLACE BLOCKED: PLAYER OR WORLD EDGE", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n107, 150), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n108, 26), (int32_t)14, hxc_this1_h9213d105e89d);
    }
    else
    {
      if (!hxc_captured)
      {
        int32_t hxc_tmp_load_result_n110 = hxc_centerX;
        int32_t hxc_tmp_load_result_n111 = hxc_centerY;
        DrawText("CLICK TO CAPTURE", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n110, 74), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n111, 26), (int32_t)14, hxc_text);
      }
      else
      {
        if (!hxc_hit.hxc_hit)
        {
          int32_t hxc_tmp_load_result_n114 = hxc_centerX;
          int32_t hxc_tmp_load_result_n115 = hxc_centerY;
          DrawText("NO BLOCK IN REACH", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n114, 78), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n115, 26), (int32_t)14, hxc_text);
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
  int32_t hxc_tmp_call_result_n14 = hxc_caxecraft_domain_World_surfaceY(hxc_cells, hxc_tmp_length_n1, 16, 16);
  double hxc_spawnY = (double)hxc_tmp_call_result_n14 + 1.0;
  struct hxc_caxecraft_domain_PlayerState hxc_tmp_call_result_n16 = hxc_caxecraft_domain_PlayerPhysics_player(16.5, hxc_spawnY, 16.5);
  struct hxc_caxecraft_domain_PlayerState hxc_tmp_call_result_n17 = hxc_caxecraft_domain_PlayerPhysics_recoverSpawn(hxc_cells, hxc_tmp_length_n1, hxc_tmp_call_result_n16);
  struct hxc_caxecraft_domain_PlayerState hxc_player = hxc_tmp_call_result_n17;
  struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_call_result_n18 = hxc_caxecraft_gameplay_Inventory_starter();
  struct hxc_caxecraft_gameplay_InventoryState hxc_inventory = hxc_tmp_call_result_n18;
  struct hxc_caxecraft_gameplay_GuideState hxc_tmp_call_result_n19 = hxc_caxecraft_gameplay_GuideNpc_start(hxc_cells, hxc_tmp_length_n1, 17.5, 13.5);
  struct hxc_caxecraft_gameplay_GuideState hxc_guide = hxc_tmp_call_result_n19;
  struct hxc_caxecraft_gameplay_MosslingState hxc_tmp_call_result_n20 = hxc_caxecraft_gameplay_Mossling_start(hxc_cells, hxc_tmp_length_n1, 14.5, 11.5);
  struct hxc_caxecraft_gameplay_MosslingState hxc_mossling = hxc_tmp_call_result_n20;
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
  while (1)
  {
    bool hxc_tmp_load_result_n21 = hxc_quit;
    bool hxc_tmp_short_circuit_result_n30 = !hxc_tmp_load_result_n21;
    if (!hxc_tmp_load_result_n21)
    {
      bool hxc_tmp_native_call_result_n22 = WindowShouldClose();
      hxc_tmp_short_circuit_result_n30 = !hxc_tmp_native_call_result_n22;
    }
    if (!hxc_tmp_short_circuit_result_n30)
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
    double hxc_lookYaw_h154d42d96d9f = 0.0;
    double hxc_lookPitch_h6ad177afdf69 = 0.0;
    if (hxc_captured)
    {
      struct Vector2 hxc_tmp_native_call_result_n34 = GetMouseDelta();
      struct Vector2 hxc_mouse = hxc_tmp_native_call_result_n34;
      hxc_lookYaw_h154d42d96d9f = -(double)hxc_mouse.x * 0.0025;
      hxc_lookPitch_h6ad177afdf69 = -(double)hxc_mouse.y * 0.0025;
    }
    bool hxc_tmp_native_call_result_n39 = IsMouseButtonPressed((int32_t)0);
    bool hxc_leftPressed = hxc_tmp_native_call_result_n39;
    int32_t hxc_hotbarSelection_hda2ecb7f245c = -1;
    bool hxc_tmp_native_call_result_n40 = IsKeyPressed((int32_t)49);
    if (hxc_tmp_native_call_result_n40)
    {
      hxc_hotbarSelection_hda2ecb7f245c = 0;
    }
    bool hxc_tmp_native_call_result_n41 = IsKeyPressed((int32_t)50);
    if (hxc_tmp_native_call_result_n41)
    {
      hxc_hotbarSelection_hda2ecb7f245c = 1;
    }
    bool hxc_tmp_native_call_result_n42 = IsKeyPressed((int32_t)51);
    if (hxc_tmp_native_call_result_n42)
    {
      hxc_hotbarSelection_hda2ecb7f245c = 2;
    }
    bool hxc_tmp_native_call_result_n43 = IsKeyPressed((int32_t)52);
    if (hxc_tmp_native_call_result_n43)
    {
      hxc_hotbarSelection_hda2ecb7f245c = 3;
    }
    bool hxc_tmp_native_call_result_n44 = IsKeyPressed((int32_t)53);
    if (hxc_tmp_native_call_result_n44)
    {
      hxc_hotbarSelection_hda2ecb7f245c = 4;
    }
    bool hxc_tmp_native_call_result_n45 = IsKeyPressed((int32_t)54);
    if (hxc_tmp_native_call_result_n45)
    {
      hxc_hotbarSelection_hda2ecb7f245c = 5;
    }
    bool hxc_tmp_native_call_result_n46 = IsKeyPressed((int32_t)55);
    if (hxc_tmp_native_call_result_n46)
    {
      hxc_hotbarSelection_hda2ecb7f245c = 6;
    }
    bool hxc_tmp_native_call_result_n47 = IsKeyPressed((int32_t)56);
    if (hxc_tmp_native_call_result_n47)
    {
      hxc_hotbarSelection_hda2ecb7f245c = 7;
    }
    float hxc_tmp_native_call_result_n48 = GetMouseWheelMove();
    double hxc_wheel = (double)hxc_tmp_native_call_result_n48;
    int32_t hxc_hotbarCycle_h4a295f9fbc67 = 0;
    if (hxc_wheel > 0.0)
    {
      hxc_hotbarCycle_h4a295f9fbc67 = -1;
    }
    if (hxc_wheel < 0.0)
    {
      hxc_hotbarCycle_h4a295f9fbc67 = 1;
    }
    bool hxc_tmp_native_call_result_n51 = IsKeyPressed((int32_t)32);
    bool hxc_jumpPressed_h982824f2dc3d = hxc_tmp_native_call_result_n51;
    bool hxc_tmp_load_result_n52 = hxc_captured;
    bool hxc_tmp_short_circuit_result_n44 = hxc_tmp_load_result_n52;
    if (hxc_tmp_load_result_n52)
    {
      bool hxc_tmp_native_call_result_n53 = IsMouseButtonPressed((int32_t)1);
      hxc_tmp_short_circuit_result_n44 = hxc_tmp_native_call_result_n53;
    }
    bool hxc_placePressed_hf45aaf92dcee = hxc_tmp_short_circuit_result_n44;
    bool hxc_tmp_load_result_n55 = hxc_captured;
    bool hxc_tmp_short_circuit_result_n46 = hxc_tmp_load_result_n55;
    if (hxc_tmp_load_result_n55)
    {
      bool hxc_tmp_native_call_result_n56 = IsKeyPressed((int32_t)69);
      hxc_tmp_short_circuit_result_n46 = hxc_tmp_native_call_result_n56;
    }
    bool hxc_interactPressed_h8424c1a59d1d = hxc_tmp_short_circuit_result_n46;
    bool hxc_tmp_native_call_result_n58 = IsKeyPressed((int32_t)256);
    bool hxc_pausePressed_h4359755e9f95 = hxc_tmp_native_call_result_n58;
    bool hxc_tmp_native_call_result_n59 = IsKeyPressed((int32_t)81);
    bool hxc_quitPressed_hfee691840107 = hxc_tmp_native_call_result_n59;
    double hxc_frameInput_moveForward = hxc_forward;
    double hxc_frameInput_moveRight = hxc_right;
    double hxc_frameInput_lookYaw = hxc_lookYaw_h154d42d96d9f;
    double hxc_frameInput_lookPitch = hxc_lookPitch_h6ad177afdf69;
    bool hxc_frameInput_jumpPressed = hxc_jumpPressed_h982824f2dc3d;
    bool hxc_tmp_load_result_n65 = hxc_captured;
    bool hxc_tmp_short_circuit_result_n55 = hxc_tmp_load_result_n65;
    if (hxc_tmp_load_result_n65)
    {
      hxc_tmp_short_circuit_result_n55 = hxc_leftPressed;
    }
    bool hxc_frameInput_minePressed = hxc_tmp_short_circuit_result_n55;
    bool hxc_frameInput_placePressed = hxc_placePressed_hf45aaf92dcee;
    bool hxc_frameInput_interactPressed = hxc_interactPressed_h8424c1a59d1d;
    bool hxc_frameInput_pausePressed = hxc_pausePressed_h4359755e9f95;
    bool hxc_tmp_load_result_n71 = hxc_paused;
    bool hxc_tmp_short_circuit_result_n60 = hxc_tmp_load_result_n71;
    if (hxc_tmp_load_result_n71)
    {
      hxc_tmp_short_circuit_result_n60 = hxc_leftPressed;
    }
    bool hxc_frameInput_capturePressed = hxc_tmp_short_circuit_result_n60;
    bool hxc_frameInput_quitPressed = hxc_quitPressed_hfee691840107;
    int32_t hxc_frameInput_hotbarSelection = hxc_hotbarSelection_hda2ecb7f245c;
    int32_t hxc_frameInput_hotbarCycle = hxc_hotbarCycle_h4a295f9fbc67;
    double hxc_moveForward = hxc_frameInput_moveForward;
    double hxc_moveRight = hxc_frameInput_moveRight;
    double hxc_lookYaw_h0a85ec43fdb6 = hxc_frameInput_lookYaw;
    double hxc_lookPitch_h3956e722b5a3 = hxc_frameInput_lookPitch;
    bool hxc_jumpPressed_h1bd345eb47b9 = hxc_frameInput_jumpPressed;
    bool hxc_minePressed = hxc_frameInput_minePressed;
    bool hxc_placePressed_hf80257589607 = hxc_frameInput_placePressed;
    bool hxc_interactPressed_hd71576c6d483 = hxc_frameInput_interactPressed;
    bool hxc_pausePressed_h7960da62159e = hxc_frameInput_pausePressed;
    bool hxc_capturePressed = hxc_frameInput_capturePressed;
    bool hxc_quitPressed_h323a144adf89 = hxc_frameInput_quitPressed;
    int32_t hxc_hotbarSelection_h6178d0068f41 = hxc_frameInput_hotbarSelection;
    int32_t hxc_hotbarCycle_h4e4851143c62 = hxc_frameInput_hotbarCycle;
    if (hxc_quitPressed_h323a144adf89)
    {
      hxc_quit = true;
    }
    if (hxc_hotbarSelection_h6178d0068f41 >= 0)
    {
      struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_load_result_n92 = hxc_inventory;
      struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_call_result_n94 = hxc_caxecraft_gameplay_Inventory_select(hxc_tmp_load_result_n92, hxc_hotbarSelection_h6178d0068f41);
      hxc_inventory = hxc_tmp_call_result_n94;
    }
    if (hxc_hotbarCycle_h4e4851143c62 != 0)
    {
      struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_load_result_n96 = hxc_inventory;
      struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_call_result_n98 = hxc_caxecraft_gameplay_Inventory_cycle(hxc_tmp_load_result_n96, hxc_hotbarCycle_h4e4851143c62);
      hxc_inventory = hxc_tmp_call_result_n98;
    }
    bool hxc_tmp_load_result_n99 = hxc_onTitle;
    bool hxc_tmp_short_circuit_result_n77 = !hxc_tmp_load_result_n99;
    if (!hxc_tmp_load_result_n99)
    {
      hxc_tmp_short_circuit_result_n77 = !hxc_paused;
    }
    bool hxc_tmp_short_circuit_load_result_n101 = hxc_tmp_short_circuit_result_n77;
    bool hxc_tmp_short_circuit_result_n78 = hxc_tmp_short_circuit_load_result_n101;
    if (hxc_tmp_short_circuit_load_result_n101)
    {
      hxc_tmp_short_circuit_result_n78 = hxc_interactPressed_hd71576c6d483;
    }
    bool hxc_tmp_short_circuit_load_result_n103 = hxc_tmp_short_circuit_result_n78;
    bool hxc_tmp_short_circuit_result_n79 = hxc_tmp_short_circuit_load_result_n103;
    if (hxc_tmp_short_circuit_load_result_n103)
    {
      struct hxc_caxecraft_gameplay_GuideState hxc_tmp_load_result_n104 = hxc_guide;
      double hxc_tmp_record_field_load_result_n105 = hxc_player.hxc_x;
      bool hxc_tmp_call_result_n107 = hxc_caxecraft_gameplay_GuideNpc_isInRange(hxc_tmp_load_result_n104, hxc_tmp_record_field_load_result_n105, hxc_player.hxc_z);
      hxc_tmp_short_circuit_result_n79 = hxc_tmp_call_result_n107;
    }
    if (hxc_tmp_short_circuit_result_n79)
    {
      bool hxc_tmp_call_result_n110 = hxc_caxecraft_gameplay_GuideNpc_sharesBerriesOnNextInteraction(hxc_guide);
      bool hxc_sharesBerries = hxc_tmp_call_result_n110;
      struct hxc_caxecraft_gameplay_GuideState hxc_tmp_call_result_n112 = hxc_caxecraft_gameplay_GuideNpc_interact(hxc_guide);
      hxc_guide = hxc_tmp_call_result_n112;
      if (hxc_sharesBerries)
      {
        struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_call_result_n116 = hxc_caxecraft_gameplay_Inventory_collectItem(hxc_inventory, hxc_caxecraft_gameplay_ItemKind_Berries, 2);
        hxc_inventory = hxc_tmp_call_result_n116;
      }
    }
    bool hxc_tmp_load_result_n117 = hxc_onTitle;
    bool hxc_tmp_short_circuit_result_n81 = hxc_tmp_load_result_n117;
    if (hxc_tmp_load_result_n117)
    {
      hxc_tmp_short_circuit_result_n81 = hxc_focused;
    }
    if (hxc_tmp_short_circuit_result_n81)
    {
      bool hxc_tmp_native_call_result_n120 = IsKeyPressed((int32_t)76);
      if (hxc_tmp_native_call_result_n120)
      {
        int32_t hxc_tmp_load_result_n121 = hxc_language;
        int32_t hxc_tmp_conditional_result_n82 = 0;
        if (hxc_tmp_load_result_n121 == 0)
        {
          hxc_tmp_conditional_result_n82 = 1;
        }
        else
        {
          hxc_tmp_conditional_result_n82 = 0;
        }
        hxc_language = hxc_tmp_conditional_result_n82;
      }
      bool hxc_tmp_native_call_result_n123 = IsKeyPressed((int32_t)265);
      bool hxc_tmp_short_circuit_result_n83 = hxc_tmp_native_call_result_n123;
      if (!hxc_tmp_native_call_result_n123)
      {
        bool hxc_tmp_native_call_result_n124 = IsKeyPressed((int32_t)264);
        hxc_tmp_short_circuit_result_n83 = hxc_tmp_native_call_result_n124;
      }
      if (hxc_tmp_short_circuit_result_n83)
      {
        int32_t hxc_tmp_load_result_n126 = hxc_selectedMode;
        int32_t hxc_tmp_conditional_result_n84 = 0;
        if (hxc_tmp_load_result_n126 == 0)
        {
          hxc_tmp_conditional_result_n84 = 1;
        }
        else
        {
          hxc_tmp_conditional_result_n84 = 0;
        }
        hxc_selectedMode = hxc_tmp_conditional_result_n84;
      }
      struct Vector2 hxc_tmp_native_call_result_n128 = GetMousePosition();
      struct Vector2 hxc_menuMouse = hxc_tmp_native_call_result_n128;
      struct Vector2 hxc_tmp_load_result_n129 = hxc_menuMouse;
      struct Vector2 hxc_tmp_load_result_n131 = hxc_menuMouse;
      int32_t hxc_tmp_native_call_result_n133 = GetScreenWidth();
      int32_t hxc_tmp_native_call_result_n134 = GetScreenHeight();
      int32_t hxc_tmp_call_result_n135 = hxc_caxecraft_app_TitleMenu_selectionAt((double)hxc_tmp_load_result_n129.x, (double)hxc_tmp_load_result_n131.y, (int32_t)hxc_tmp_native_call_result_n133, (int32_t)hxc_tmp_native_call_result_n134);
      int32_t hxc_hovered = hxc_tmp_call_result_n135;
      if (hxc_hovered == 0)
      {
        hxc_selectedMode = 0;
      }
      if (hxc_hovered == 1)
      {
        hxc_selectedMode = 1;
      }
      int32_t hxc_tmp_load_result_n138 = hxc_hovered;
      bool hxc_tmp_short_circuit_result_n88 = hxc_tmp_load_result_n138 >= 0;
      if (hxc_tmp_load_result_n138 >= 0)
      {
        bool hxc_tmp_native_call_result_n139 = IsMouseButtonPressed((int32_t)0);
        hxc_tmp_short_circuit_result_n88 = hxc_tmp_native_call_result_n139;
      }
      bool hxc_clickedChoice = hxc_tmp_short_circuit_result_n88;
      bool hxc_tmp_load_result_n141 = hxc_clickedChoice;
      bool hxc_tmp_short_circuit_result_n89 = hxc_tmp_load_result_n141;
      if (!hxc_tmp_load_result_n141)
      {
        bool hxc_tmp_native_call_result_n142 = IsKeyPressed((int32_t)257);
        hxc_tmp_short_circuit_result_n89 = hxc_tmp_native_call_result_n142;
      }
      if (hxc_tmp_short_circuit_result_n89)
      {
        hxc_onTitle = false;
        hxc_paused = false;
        hxc_captured = true;
        hxc_recapturedThisFrame = true;
        DisableCursor();
      }
    }
    bool hxc_tmp_load_result_n144 = hxc_onTitle;
    bool hxc_tmp_short_circuit_result_n90 = !hxc_tmp_load_result_n144;
    if (!hxc_tmp_load_result_n144)
    {
      hxc_tmp_short_circuit_result_n90 = !hxc_focused;
    }
    bool hxc_tmp_short_circuit_load_result_n146 = hxc_tmp_short_circuit_result_n90;
    bool hxc_tmp_short_circuit_result_n91 = hxc_tmp_short_circuit_load_result_n146;
    if (hxc_tmp_short_circuit_load_result_n146)
    {
      hxc_tmp_short_circuit_result_n91 = hxc_captured;
    }
    if (hxc_tmp_short_circuit_result_n91)
    {
      hxc_paused = true;
      hxc_captured = false;
      hxc_jumpQueued = false;
      EnableCursor();
    }
    bool hxc_tmp_load_result_n149 = hxc_onTitle;
    bool hxc_tmp_short_circuit_result_n92 = !hxc_tmp_load_result_n149;
    if (!hxc_tmp_load_result_n149)
    {
      hxc_tmp_short_circuit_result_n92 = hxc_focused;
    }
    bool hxc_tmp_short_circuit_load_result_n151 = hxc_tmp_short_circuit_result_n92;
    bool hxc_tmp_short_circuit_result_n93 = hxc_tmp_short_circuit_load_result_n151;
    if (hxc_tmp_short_circuit_load_result_n151)
    {
      hxc_tmp_short_circuit_result_n93 = hxc_pausePressed_h7960da62159e;
    }
    if (hxc_tmp_short_circuit_result_n93)
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
    bool hxc_tmp_load_result_n158 = hxc_onTitle;
    bool hxc_tmp_short_circuit_result_n94 = !hxc_tmp_load_result_n158;
    if (!hxc_tmp_load_result_n158)
    {
      hxc_tmp_short_circuit_result_n94 = hxc_focused;
    }
    bool hxc_tmp_short_circuit_load_result_n160 = hxc_tmp_short_circuit_result_n94;
    bool hxc_tmp_short_circuit_result_n95 = hxc_tmp_short_circuit_load_result_n160;
    if (hxc_tmp_short_circuit_load_result_n160)
    {
      hxc_tmp_short_circuit_result_n95 = hxc_paused;
    }
    bool hxc_tmp_short_circuit_load_result_n162 = hxc_tmp_short_circuit_result_n95;
    bool hxc_tmp_short_circuit_result_n96 = hxc_tmp_short_circuit_load_result_n162;
    if (hxc_tmp_short_circuit_load_result_n162)
    {
      hxc_tmp_short_circuit_result_n96 = hxc_capturePressed;
    }
    if (hxc_tmp_short_circuit_result_n96)
    {
      hxc_paused = false;
      hxc_captured = true;
      hxc_recapturedThisFrame = true;
      DisableCursor();
    }
    if (hxc_captured)
    {
      double hxc_yawDelta = hxc_lookYaw_h0a85ec43fdb6;
      if (hxc_yawDelta > 0.25)
      {
        hxc_yawDelta = 0.25;
      }
      if (hxc_yawDelta < -0.25)
      {
        hxc_yawDelta = -0.25;
      }
      double hxc_tmp_load_result_n169 = hxc_lookX;
      double hxc_tmp_load_result_n170 = hxc_yawDelta;
      double hxc_candidateX = hxc_tmp_load_result_n169 + hxc_tmp_load_result_n170 * hxc_lookZ;
      double hxc_tmp_load_result_n172 = hxc_lookZ;
      double hxc_tmp_load_result_n173 = hxc_yawDelta;
      double hxc_candidateZ = hxc_tmp_load_result_n172 - hxc_tmp_load_result_n173 * hxc_lookX;
      double hxc_tmp_load_result_n175 = hxc_candidateX;
      double hxc_tmp_load_result_n176 = hxc_candidateX;
      double hxc_tmp_load_result_n177 = hxc_candidateZ;
      double hxc_lengthSquared = hxc_tmp_load_result_n175 * hxc_tmp_load_result_n176 + hxc_tmp_load_result_n177 * hxc_candidateZ;
      double hxc_normalize = 1.5 - 0.5 * hxc_lengthSquared;
      double hxc_tmp_load_result_n180 = hxc_candidateX;
      hxc_lookX = hxc_tmp_load_result_n180 * hxc_normalize;
      double hxc_tmp_load_result_n182 = hxc_candidateZ;
      hxc_lookZ = hxc_tmp_load_result_n182 * hxc_normalize;
      double hxc_tmp_compound_load_result_n184 = hxc_lookY;
      hxc_lookY = hxc_tmp_compound_load_result_n184 + hxc_lookPitch_h3956e722b5a3;
      if (hxc_lookY > 0.90)
      {
        hxc_lookY = 0.90;
      }
      if (hxc_lookY < -0.90)
      {
        hxc_lookY = -0.90;
      }
      if (hxc_jumpPressed_h1bd345eb47b9)
      {
        hxc_jumpQueued = true;
      }
    }
    float hxc_tmp_native_call_result_n189 = GetFrameTime();
    double hxc_frameSeconds = (double)hxc_tmp_native_call_result_n189;
    if (hxc_frameSeconds > 0.25)
    {
      hxc_frameSeconds = 0.25;
    }
    if (!hxc_paused)
    {
      double hxc_tmp_compound_load_result_n192 = hxc_accumulator;
      hxc_accumulator = hxc_tmp_compound_load_result_n192 + hxc_frameSeconds;
    }
    while (1)
    {
      bool hxc_tmp_load_result_n194 = hxc_paused;
      bool hxc_tmp_short_circuit_result_n103 = !hxc_tmp_load_result_n194;
      if (!hxc_tmp_load_result_n194)
      {
        hxc_tmp_short_circuit_result_n103 = hxc_accumulator >= 0.05;
      }
      if (!hxc_tmp_short_circuit_result_n103)
      {
        break;
      }
      double hxc_tmp_load_result_n197 = hxc_moveForward;
      double hxc_tmp_load_result_n198 = hxc_lookX;
      double hxc_tmp_load_result_n199 = hxc_moveRight;
      double hxc_moveX = hxc_tmp_load_result_n197 * hxc_tmp_load_result_n198 - hxc_tmp_load_result_n199 * hxc_lookZ;
      double hxc_tmp_load_result_n201 = hxc_moveForward;
      double hxc_tmp_load_result_n202 = hxc_lookZ;
      double hxc_tmp_load_result_n203 = hxc_moveRight;
      double hxc_moveZ = hxc_tmp_load_result_n201 * hxc_tmp_load_result_n202 + hxc_tmp_load_result_n203 * hxc_lookX;
      double hxc_tmp_load_result_n205 = hxc_moveForward;
      bool hxc_tmp_short_circuit_result_n106 = hxc_tmp_load_result_n205 != 0.0;
      if (hxc_tmp_load_result_n205 != 0.0)
      {
        hxc_tmp_short_circuit_result_n106 = hxc_moveRight != 0.0;
      }
      if (hxc_tmp_short_circuit_result_n106)
      {
        hxc_moveX = hxc_moveX * 0.7071067811865476;
        hxc_moveZ = hxc_moveZ * 0.7071067811865476;
      }
      struct hxc_caxecraft_domain_PlayerState hxc_tmp_load_result_n210 = hxc_player;
      double hxc_tmp_load_result_n211 = hxc_moveX;
      double hxc_tmp_load_result_n212 = hxc_moveZ;
      struct hxc_caxecraft_domain_StepInput hxc_tmp_call_result_n214 = hxc_caxecraft_domain_PlayerPhysics_input(hxc_tmp_load_result_n211, hxc_tmp_load_result_n212, hxc_jumpQueued);
      struct hxc_caxecraft_domain_PlayerState hxc_tmp_call_result_n215 = hxc_caxecraft_domain_PlayerPhysics_step(hxc_cells, hxc_tmp_length_n1, hxc_tmp_load_result_n210, hxc_tmp_call_result_n214);
      hxc_player = hxc_tmp_call_result_n215;
      if (hxc_selectedMode == 1)
      {
        struct hxc_caxecraft_gameplay_MosslingState hxc_tmp_load_result_n217 = hxc_mossling;
        double hxc_tmp_record_field_load_result_n218 = hxc_player.hxc_x;
        struct hxc_caxecraft_gameplay_MosslingState hxc_tmp_call_result_n220 = hxc_caxecraft_gameplay_Mossling_step(hxc_cells, hxc_tmp_length_n1, hxc_tmp_load_result_n217, hxc_tmp_record_field_load_result_n218, hxc_player.hxc_z);
        hxc_mossling = hxc_tmp_call_result_n220;
      }
      hxc_jumpQueued = false;
      hxc_accumulator = hxc_accumulator - 0.05;
      hxc_updateCount = hxc_i32_add_wrapping(hxc_updateCount, 1);
    }
    double hxc_eyeX = hxc_player.hxc_x;
    double hxc_eyeY = hxc_player.hxc_y + 1.62;
    double hxc_eyeZ = hxc_player.hxc_z;
    double hxc_tmp_load_result_n226 = hxc_eyeX;
    double hxc_tmp_load_result_n227 = hxc_eyeY;
    double hxc_tmp_load_result_n228 = hxc_eyeZ;
    double hxc_tmp_load_result_n229 = hxc_lookX;
    double hxc_tmp_load_result_n230 = hxc_lookY;
    struct hxc_caxecraft_domain_RaycastHit hxc_tmp_call_result_n232 = hxc_caxecraft_domain_VoxelRaycast_trace(hxc_cells, hxc_tmp_length_n1, hxc_tmp_load_result_n226, hxc_tmp_load_result_n227, hxc_tmp_load_result_n228, hxc_tmp_load_result_n229, hxc_tmp_load_result_n230, hxc_lookZ, 7.0);
    struct hxc_caxecraft_domain_RaycastHit hxc_hit = hxc_tmp_call_result_n232;
    bool hxc_tmp_load_result_n233 = hxc_captured;
    bool hxc_tmp_short_circuit_result_n111 = hxc_tmp_load_result_n233;
    if (hxc_tmp_load_result_n233)
    {
      hxc_tmp_short_circuit_result_n111 = !hxc_recapturedThisFrame;
    }
    bool hxc_tmp_short_circuit_load_result_n235 = hxc_tmp_short_circuit_result_n111;
    bool hxc_tmp_short_circuit_result_n112 = hxc_tmp_short_circuit_load_result_n235;
    if (hxc_tmp_short_circuit_load_result_n235)
    {
      hxc_tmp_short_circuit_result_n112 = hxc_hit.hxc_hit;
    }
    bool hxc_tmp_short_circuit_load_result_n237 = hxc_tmp_short_circuit_result_n112;
    bool hxc_tmp_short_circuit_result_n113 = hxc_tmp_short_circuit_load_result_n237;
    if (hxc_tmp_short_circuit_load_result_n237)
    {
      hxc_tmp_short_circuit_result_n113 = hxc_minePressed;
    }
    if (hxc_tmp_short_circuit_result_n113)
    {
      int32_t hxc_tmp_record_field_load_result_n240 = hxc_hit.hxc_cellX;
      int32_t hxc_tmp_record_field_load_result_n241 = hxc_hit.hxc_cellY;
      struct hxc_caxecraft_domain_BlockCoord hxc_tmp_call_result_n243 = hxc_caxecraft_domain_World_coord(hxc_tmp_record_field_load_result_n240, hxc_tmp_record_field_load_result_n241, hxc_hit.hxc_cellZ);
      enum hxc_caxecraft_domain_BlockKind hxc_tmp_call_result_n244 = hxc_caxecraft_domain_World_query(hxc_cells, hxc_tmp_length_n1, hxc_tmp_call_result_n243);
      enum hxc_caxecraft_domain_BlockKind hxc_removedKind = hxc_tmp_call_result_n244;
      int32_t hxc_tmp_record_field_load_result_n245 = hxc_hit.hxc_cellX;
      int32_t hxc_tmp_record_field_load_result_n246 = hxc_hit.hxc_cellY;
      struct hxc_caxecraft_domain_BlockCoord hxc_tmp_call_result_n248 = hxc_caxecraft_domain_World_coord(hxc_tmp_record_field_load_result_n245, hxc_tmp_record_field_load_result_n246, hxc_hit.hxc_cellZ);
      bool hxc_tmp_call_result_n249 = hxc_caxecraft_domain_World_remove(hxc_cells, hxc_tmp_length_n1, hxc_tmp_call_result_n248);
      bool hxc_tmp_short_circuit_result_n115 = hxc_tmp_call_result_n249;
      if (hxc_tmp_call_result_n249)
      {
        hxc_tmp_short_circuit_result_n115 = hxc_selectedMode == 1;
      }
      if (hxc_tmp_short_circuit_result_n115)
      {
        struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_load_result_n252 = hxc_inventory;
        struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_call_result_n254 = hxc_caxecraft_gameplay_Inventory_collectBlock(hxc_tmp_load_result_n252, hxc_removedKind);
        hxc_inventory = hxc_tmp_call_result_n254;
      }
    }
    bool hxc_tmp_load_result_n255 = hxc_captured;
    bool hxc_tmp_short_circuit_result_n116 = hxc_tmp_load_result_n255;
    if (hxc_tmp_load_result_n255)
    {
      hxc_tmp_short_circuit_result_n116 = hxc_hit.hxc_hit;
    }
    bool hxc_tmp_short_circuit_load_result_n257 = hxc_tmp_short_circuit_result_n116;
    bool hxc_tmp_short_circuit_result_n117 = hxc_tmp_short_circuit_load_result_n257;
    if (hxc_tmp_short_circuit_load_result_n257)
    {
      hxc_tmp_short_circuit_result_n117 = hxc_placePressed_hf80257589607;
    }
    if (hxc_tmp_short_circuit_result_n117)
    {
      int32_t hxc_tmp_record_field_load_result_n260 = hxc_hit.hxc_previousX;
      int32_t hxc_tmp_record_field_load_result_n261 = hxc_hit.hxc_previousY;
      struct hxc_caxecraft_domain_BlockCoord hxc_tmp_call_result_n263 = hxc_caxecraft_domain_World_coord(hxc_tmp_record_field_load_result_n260, hxc_tmp_record_field_load_result_n261, hxc_hit.hxc_previousZ);
      struct hxc_caxecraft_domain_BlockCoord hxc_placement = hxc_tmp_call_result_n263;
      enum hxc_caxecraft_domain_BlockKind hxc_tmp_call_result_n265 = hxc_caxecraft_gameplay_Inventory_selectedBlock(hxc_inventory);
      enum hxc_caxecraft_domain_BlockKind hxc_selectedBlock = hxc_tmp_call_result_n265;
      struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_load_result_n266 = hxc_inventory;
      int32_t hxc_tmp_call_result_n268 = hxc_caxecraft_gameplay_Inventory_countAt(hxc_tmp_load_result_n266, hxc_inventory.hxc_selected);
      bool hxc_hasItem = hxc_tmp_call_result_n268 > 0;
      bool hxc_tmp_load_result_n269 = hxc_hasItem;
      bool hxc_tmp_short_circuit_result_n121 = !hxc_tmp_load_result_n269;
      if (!!hxc_tmp_load_result_n269)
      {
        bool hxc_tmp_call_result_n271 = hxc_caxecraft_domain_World_isPlaceable(hxc_selectedBlock);
        hxc_tmp_short_circuit_result_n121 = !hxc_tmp_call_result_n271;
      }
      bool hxc_tmp_short_circuit_load_result_n272 = hxc_tmp_short_circuit_result_n121;
      bool hxc_tmp_short_circuit_result_n122 = hxc_tmp_short_circuit_load_result_n272;
      if (!hxc_tmp_short_circuit_load_result_n272)
      {
        struct hxc_caxecraft_domain_PlayerState hxc_tmp_load_result_n273 = hxc_player;
        bool hxc_tmp_call_result_n275 = hxc_caxecraft_domain_PlayerPhysics_canPlaceAt(hxc_tmp_load_result_n273, hxc_placement);
        hxc_tmp_short_circuit_result_n122 = !hxc_tmp_call_result_n275;
      }
      bool hxc_tmp_short_circuit_load_result_n276 = hxc_tmp_short_circuit_result_n122;
      bool hxc_tmp_short_circuit_result_n123 = hxc_tmp_short_circuit_load_result_n276;
      if (!hxc_tmp_short_circuit_load_result_n276)
      {
        struct hxc_caxecraft_domain_BlockCoord hxc_tmp_load_result_n277 = hxc_placement;
        bool hxc_tmp_call_result_n279 = hxc_caxecraft_domain_World_place(hxc_cells, hxc_tmp_length_n1, hxc_tmp_load_result_n277, hxc_selectedBlock);
        hxc_tmp_short_circuit_result_n123 = !hxc_tmp_call_result_n279;
      }
      if (hxc_tmp_short_circuit_result_n123)
      {
        hxc_placementBlockedFrames = 60;
      }
      else
      {
        if (hxc_selectedMode == 1)
        {
          struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_call_result_n283 = hxc_caxecraft_gameplay_Inventory_consumeSelected(hxc_inventory);
          hxc_inventory = hxc_tmp_call_result_n283;
        }
      }
    }
    if (hxc_placementBlockedFrames > 0)
    {
      hxc_placementBlockedFrames = hxc_i32_subtract_wrapping(hxc_placementBlockedFrames, 1);
    }
    double hxc_tmp_load_result_n286 = hxc_eyeX;
    double hxc_tmp_load_result_n287 = hxc_eyeY;
    double hxc_tmp_load_result_n288 = hxc_eyeZ;
    double hxc_tmp_load_result_n290 = hxc_eyeX;
    double hxc_tmp_load_result_n291 = hxc_lookX;
    double hxc_tmp_load_result_n292 = hxc_eyeY;
    double hxc_tmp_load_result_n293 = hxc_lookY;
    double hxc_tmp_load_result_n294 = hxc_eyeZ;
    struct Camera3D hxc_camera = (struct Camera3D){ .position = (struct Vector3){ .x = (float)hxc_tmp_load_result_n286, .y = (float)hxc_tmp_load_result_n287, .z = (float)hxc_tmp_load_result_n288 }, .target = (struct Vector3){ .x = (float)(hxc_tmp_load_result_n290 + hxc_tmp_load_result_n291), .y = (float)(hxc_tmp_load_result_n292 + hxc_tmp_load_result_n293), .z = (float)(hxc_tmp_load_result_n294 + hxc_lookZ) }, .up = (struct Vector3){ .x = (float)0.0, .y = (float)1.0, .z = (float)0.0 }, .fovy = (float)70.0, .projection = 0 };
    BeginDrawing();
    if (hxc_onTitle)
    {
      int32_t hxc_tmp_native_call_result_n300 = GetScreenWidth();
      int32_t hxc_width_h0214ba48447b = (int32_t)hxc_tmp_native_call_result_n300;
      int32_t hxc_tmp_native_call_result_n301 = GetScreenHeight();
      int32_t hxc_height_h5a762c27aa21 = (int32_t)hxc_tmp_native_call_result_n301;
      if (hxc_titleTextureReady)
      {
        struct Color hxc_this1_hc49cde33eed5 = (struct Color){ .r = 255, .g = 255, .b = 255, .a = 255 };
        struct Texture hxc_tmp_load_result_n304 = hxc_titleTexture;
        double hxc_imageAspect = hxc_f64_divide_zero_safe((double)(int32_t)hxc_tmp_load_result_n304.width, (double)(int32_t)hxc_titleTexture.height);
        int32_t hxc_tmp_load_result_n308 = hxc_width_h0214ba48447b;
        double hxc_screenAspect = hxc_f64_divide_zero_safe((double)hxc_tmp_load_result_n308, (double)hxc_height_h5a762c27aa21);
        double hxc_sourceX = 0.0;
        double hxc_sourceY = 0.0;
        double hxc_sourceWidth = (double)(int32_t)hxc_titleTexture.width;
        double hxc_sourceHeight = (double)(int32_t)hxc_titleTexture.height;
        double hxc_tmp_load_result_n314 = hxc_screenAspect;
        if (hxc_tmp_load_result_n314 > hxc_imageAspect)
        {
          struct Texture hxc_tmp_load_result_n316 = hxc_titleTexture;
          hxc_sourceHeight = hxc_f64_divide_zero_safe((double)(int32_t)hxc_tmp_load_result_n316.width, hxc_screenAspect);
          struct Texture hxc_tmp_load_result_n319 = hxc_titleTexture;
          hxc_sourceY = ((double)(int32_t)hxc_tmp_load_result_n319.height - hxc_sourceHeight) * 0.5;
        }
        else
        {
          struct Texture hxc_tmp_load_result_n322 = hxc_titleTexture;
          hxc_sourceWidth = (double)(int32_t)hxc_tmp_load_result_n322.height * hxc_screenAspect;
          struct Texture hxc_tmp_load_result_n325 = hxc_titleTexture;
          hxc_sourceX = ((double)(int32_t)hxc_tmp_load_result_n325.width - hxc_sourceWidth) * 0.5;
        }
        struct Texture hxc_tmp_load_result_n328 = hxc_titleTexture;
        double hxc_tmp_load_result_n329 = hxc_sourceX;
        double hxc_tmp_load_result_n330 = hxc_sourceY;
        double hxc_tmp_load_result_n331 = hxc_sourceWidth;
        double hxc_tmp_load_result_n332 = hxc_sourceHeight;
        int32_t hxc_tmp_load_result_n334 = hxc_width_h0214ba48447b;
        int32_t hxc_tmp_load_result_n335 = hxc_height_h5a762c27aa21;
        DrawTexturePro(hxc_tmp_load_result_n328, (struct Rectangle){ .x = (float)hxc_tmp_load_result_n329, .y = (float)hxc_tmp_load_result_n330, .width = (float)hxc_tmp_load_result_n331, .height = (float)hxc_tmp_load_result_n332 }, (struct Rectangle){ .x = (float)0.0, .y = (float)0.0, .width = (float)((double)hxc_tmp_load_result_n334 + 0.0), .height = (float)((double)hxc_tmp_load_result_n335 + 0.0) }, (struct Vector2){ .x = (float)0.0, .y = (float)0.0 }, (float)0.0, hxc_this1_hc49cde33eed5);
      }
      else
      {
        ClearBackground((struct Color){ .r = 126, .g = 190, .b = 201, .a = 255 });
      }
      struct Color hxc_this1_h9385c0f82e45 = (struct Color){ .r = 4, .g = 14, .b = 23, .a = 132 };
      int32_t hxc_tmp_load_result_n341 = hxc_width_h0214ba48447b;
      int32_t hxc_tmp_load_result_n342 = hxc_height_h5a762c27aa21;
      DrawRectangle((int32_t)0, (int32_t)0, (int32_t)hxc_tmp_load_result_n341, (int32_t)hxc_tmp_load_result_n342, hxc_this1_h9385c0f82e45);
      if (hxc_wordmarkTextureReady)
      {
        int32_t hxc_centerX = hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_width_h0214ba48447b, (double)2));
        struct Color hxc_this1_h1e10c3259526 = (struct Color){ .r = 255, .g = 255, .b = 255, .a = 255 };
        double hxc_scale = hxc_f64_divide_zero_safe((double)560, (double)(int32_t)hxc_wordmarkTexture.width);
        double hxc_heightScale = hxc_f64_divide_zero_safe((double)132, (double)(int32_t)hxc_wordmarkTexture.height);
        double hxc_tmp_load_result_n351 = hxc_heightScale;
        if (hxc_tmp_load_result_n351 < hxc_scale)
        {
          hxc_scale = hxc_heightScale;
        }
        struct Texture hxc_tmp_load_result_n354 = hxc_wordmarkTexture;
        double hxc_width_h7789146b3909 = (double)(int32_t)hxc_tmp_load_result_n354.width * hxc_scale;
        struct Texture hxc_tmp_load_result_n357 = hxc_wordmarkTexture;
        double hxc_height_hbe33d4cdeb0d = (double)(int32_t)hxc_tmp_load_result_n357.height * hxc_scale;
        double hxc_width1 = (double)(int32_t)hxc_wordmarkTexture.width + 0.0;
        double hxc_height1 = (double)(int32_t)hxc_wordmarkTexture.height + 0.0;
        struct Texture hxc_tmp_load_result_n364 = hxc_wordmarkTexture;
        double hxc_tmp_load_result_n365 = hxc_width1;
        double hxc_tmp_load_result_n366 = hxc_height1;
        int32_t hxc_tmp_load_result_n368 = hxc_centerX;
        double hxc_tmp_load_result_n369 = hxc_width_h7789146b3909;
        double hxc_tmp_load_result_n370 = hxc_width_h7789146b3909;
        double hxc_tmp_load_result_n371 = hxc_height_hbe33d4cdeb0d;
        DrawTexturePro(hxc_tmp_load_result_n364, (struct Rectangle){ .x = (float)0.0, .y = (float)0.0, .width = (float)hxc_tmp_load_result_n365, .height = (float)hxc_tmp_load_result_n366 }, (struct Rectangle){ .x = (float)((double)hxc_tmp_load_result_n368 - hxc_tmp_load_result_n369 * 0.5), .y = (float)28.0, .width = (float)hxc_tmp_load_result_n370, .height = (float)hxc_tmp_load_result_n371 }, (struct Vector2){ .x = (float)0.0, .y = (float)0.0 }, (float)0.0, hxc_this1_h1e10c3259526);
      }
      else
      {
        int32_t hxc_x_h17b43afb9b72 = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_width_h0214ba48447b, (double)2)), 92);
        struct Color hxc_this1_h56648e42e3a2 = (struct Color){ .r = 242, .g = 249, .b = 245, .a = 255 };
        int32_t hxc_tmp_load_result_n377 = hxc_x_h17b43afb9b72;
        DrawText("CAXECRAFT", (int32_t)hxc_tmp_load_result_n377, (int32_t)52, (int32_t)34, hxc_this1_h56648e42e3a2);
      }
      int32_t hxc_firstTop = hxc_i32_subtract_wrapping(hxc_height_h5a762c27aa21, 225);
      if (hxc_language == 1)
      {
        int32_t hxc_left_h7946f370e9d6 = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_width_h0214ba48447b, (double)2)), 180);
        if (hxc_selectedMode == 0)
        {
          struct Color hxc_this1_hb97479ef854e = (struct Color){ .r = 16, .g = 88, .b = 102, .a = 232 };
          int32_t hxc_tmp_load_result_n384 = hxc_left_h7946f370e9d6;
          int32_t hxc_tmp_load_result_n385 = hxc_firstTop;
          DrawRectangle((int32_t)hxc_tmp_load_result_n384, (int32_t)hxc_tmp_load_result_n385, (int32_t)360, (int32_t)54, hxc_this1_hb97479ef854e);
          struct Color hxc_this1_h1150cd733bd0 = (struct Color){ .r = 255, .g = 190, .b = 55, .a = 255 };
          int32_t hxc_tmp_load_result_n388 = hxc_left_h7946f370e9d6;
          int32_t hxc_tmp_load_result_n389 = hxc_firstTop;
          DrawRectangleLines((int32_t)hxc_tmp_load_result_n388, (int32_t)hxc_tmp_load_result_n389, (int32_t)360, (int32_t)54, hxc_this1_h1150cd733bd0);
        }
        else
        {
          struct Color hxc_this1_h76ebd527ca60 = (struct Color){ .r = 6, .g = 26, .b = 36, .a = 218 };
          int32_t hxc_tmp_load_result_n392 = hxc_left_h7946f370e9d6;
          int32_t hxc_tmp_load_result_n393 = hxc_firstTop;
          DrawRectangle((int32_t)hxc_tmp_load_result_n392, (int32_t)hxc_tmp_load_result_n393, (int32_t)360, (int32_t)54, hxc_this1_h76ebd527ca60);
          struct Color hxc_this1_he5197d02d4da = (struct Color){ .r = 92, .g = 194, .b = 188, .a = 255 };
          int32_t hxc_tmp_load_result_n396 = hxc_left_h7946f370e9d6;
          int32_t hxc_tmp_load_result_n397 = hxc_firstTop;
          DrawRectangleLines((int32_t)hxc_tmp_load_result_n396, (int32_t)hxc_tmp_load_result_n397, (int32_t)360, (int32_t)54, hxc_this1_he5197d02d4da);
        }
        struct Color hxc_this1_haed7460f78d4 = (struct Color){ .r = 242, .g = 249, .b = 245, .a = 255 };
        int32_t hxc_tmp_load_result_n400 = hxc_left_h7946f370e9d6;
        int32_t hxc_tmp_load_result_n401 = hxc_firstTop;
        DrawText("MODO CREATIVO", (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n400, 28), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n401, 16), (int32_t)21, hxc_this1_haed7460f78d4);
        int32_t hxc_top_hc7c5eaaf4a76 = hxc_i32_add_wrapping(hxc_i32_add_wrapping(hxc_firstTop, 54), 14);
        int32_t hxc_left_h7c3cfd30012a = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_width_h0214ba48447b, (double)2)), 180);
        if (hxc_selectedMode == 1)
        {
          struct Color hxc_this1_h9f9078c8c23e = (struct Color){ .r = 16, .g = 88, .b = 102, .a = 232 };
          int32_t hxc_tmp_load_result_n407 = hxc_left_h7c3cfd30012a;
          int32_t hxc_tmp_load_result_n408 = hxc_top_hc7c5eaaf4a76;
          DrawRectangle((int32_t)hxc_tmp_load_result_n407, (int32_t)hxc_tmp_load_result_n408, (int32_t)360, (int32_t)54, hxc_this1_h9f9078c8c23e);
          struct Color hxc_this1_hbe58d2c8ebc0 = (struct Color){ .r = 255, .g = 190, .b = 55, .a = 255 };
          int32_t hxc_tmp_load_result_n411 = hxc_left_h7c3cfd30012a;
          int32_t hxc_tmp_load_result_n412 = hxc_top_hc7c5eaaf4a76;
          DrawRectangleLines((int32_t)hxc_tmp_load_result_n411, (int32_t)hxc_tmp_load_result_n412, (int32_t)360, (int32_t)54, hxc_this1_hbe58d2c8ebc0);
        }
        else
        {
          struct Color hxc_this1_hd91a64420a0e = (struct Color){ .r = 6, .g = 26, .b = 36, .a = 218 };
          int32_t hxc_tmp_load_result_n415 = hxc_left_h7c3cfd30012a;
          int32_t hxc_tmp_load_result_n416 = hxc_top_hc7c5eaaf4a76;
          DrawRectangle((int32_t)hxc_tmp_load_result_n415, (int32_t)hxc_tmp_load_result_n416, (int32_t)360, (int32_t)54, hxc_this1_hd91a64420a0e);
          struct Color hxc_this1_h6986c66fd29d = (struct Color){ .r = 92, .g = 194, .b = 188, .a = 255 };
          int32_t hxc_tmp_load_result_n419 = hxc_left_h7c3cfd30012a;
          int32_t hxc_tmp_load_result_n420 = hxc_top_hc7c5eaaf4a76;
          DrawRectangleLines((int32_t)hxc_tmp_load_result_n419, (int32_t)hxc_tmp_load_result_n420, (int32_t)360, (int32_t)54, hxc_this1_h6986c66fd29d);
        }
        struct Color hxc_this1_h742952063b58 = (struct Color){ .r = 242, .g = 249, .b = 245, .a = 255 };
        int32_t hxc_tmp_load_result_n423 = hxc_left_h7c3cfd30012a;
        int32_t hxc_tmp_load_result_n424 = hxc_top_hc7c5eaaf4a76;
        DrawText("AVENTURA", (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n423, 28), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n424, 16), (int32_t)21, hxc_this1_h742952063b58);
        int32_t hxc_x_h49002ef2a852 = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_width_h0214ba48447b, (double)2)), 270);
        struct Color hxc_this1_h17bbeb228c01 = (struct Color){ .r = 229, .g = 241, .b = 235, .a = 255 };
        int32_t hxc_tmp_load_result_n428 = hxc_x_h49002ef2a852;
        int32_t hxc_tmp_load_result_n429 = hxc_height_h5a762c27aa21;
        DrawText("FLECHAS / RATON PARA ELEGIR   ENTER PARA JUGAR   L: ENGLISH", (int32_t)hxc_tmp_load_result_n428, (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n429, 58), (int32_t)16, hxc_this1_h17bbeb228c01);
      }
      else
      {
        int32_t hxc_left_h213a7753d5d9 = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_width_h0214ba48447b, (double)2)), 180);
        if (hxc_selectedMode == 0)
        {
          struct Color hxc_this1_h497240767d75 = (struct Color){ .r = 16, .g = 88, .b = 102, .a = 232 };
          int32_t hxc_tmp_load_result_n434 = hxc_left_h213a7753d5d9;
          int32_t hxc_tmp_load_result_n435 = hxc_firstTop;
          DrawRectangle((int32_t)hxc_tmp_load_result_n434, (int32_t)hxc_tmp_load_result_n435, (int32_t)360, (int32_t)54, hxc_this1_h497240767d75);
          struct Color hxc_this1_hf8da672797dd = (struct Color){ .r = 255, .g = 190, .b = 55, .a = 255 };
          int32_t hxc_tmp_load_result_n438 = hxc_left_h213a7753d5d9;
          int32_t hxc_tmp_load_result_n439 = hxc_firstTop;
          DrawRectangleLines((int32_t)hxc_tmp_load_result_n438, (int32_t)hxc_tmp_load_result_n439, (int32_t)360, (int32_t)54, hxc_this1_hf8da672797dd);
        }
        else
        {
          struct Color hxc_this1_h79577861e1b3 = (struct Color){ .r = 6, .g = 26, .b = 36, .a = 218 };
          int32_t hxc_tmp_load_result_n442 = hxc_left_h213a7753d5d9;
          int32_t hxc_tmp_load_result_n443 = hxc_firstTop;
          DrawRectangle((int32_t)hxc_tmp_load_result_n442, (int32_t)hxc_tmp_load_result_n443, (int32_t)360, (int32_t)54, hxc_this1_h79577861e1b3);
          struct Color hxc_this1_he648b63556d3 = (struct Color){ .r = 92, .g = 194, .b = 188, .a = 255 };
          int32_t hxc_tmp_load_result_n446 = hxc_left_h213a7753d5d9;
          int32_t hxc_tmp_load_result_n447 = hxc_firstTop;
          DrawRectangleLines((int32_t)hxc_tmp_load_result_n446, (int32_t)hxc_tmp_load_result_n447, (int32_t)360, (int32_t)54, hxc_this1_he648b63556d3);
        }
        struct Color hxc_this1_hc6235c1aaa0e = (struct Color){ .r = 242, .g = 249, .b = 245, .a = 255 };
        int32_t hxc_tmp_load_result_n450 = hxc_left_h213a7753d5d9;
        int32_t hxc_tmp_load_result_n451 = hxc_firstTop;
        DrawText("CREATIVE MODE", (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n450, 28), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n451, 16), (int32_t)21, hxc_this1_hc6235c1aaa0e);
        int32_t hxc_top_hc34b0fec3157 = hxc_i32_add_wrapping(hxc_i32_add_wrapping(hxc_firstTop, 54), 14);
        int32_t hxc_left_h0beaf76c31b0 = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_width_h0214ba48447b, (double)2)), 180);
        if (hxc_selectedMode == 1)
        {
          struct Color hxc_this1_h45e7cd6f2d61 = (struct Color){ .r = 16, .g = 88, .b = 102, .a = 232 };
          int32_t hxc_tmp_load_result_n457 = hxc_left_h0beaf76c31b0;
          int32_t hxc_tmp_load_result_n458 = hxc_top_hc34b0fec3157;
          DrawRectangle((int32_t)hxc_tmp_load_result_n457, (int32_t)hxc_tmp_load_result_n458, (int32_t)360, (int32_t)54, hxc_this1_h45e7cd6f2d61);
          struct Color hxc_this1_h5ab6e6cdd588 = (struct Color){ .r = 255, .g = 190, .b = 55, .a = 255 };
          int32_t hxc_tmp_load_result_n461 = hxc_left_h0beaf76c31b0;
          int32_t hxc_tmp_load_result_n462 = hxc_top_hc34b0fec3157;
          DrawRectangleLines((int32_t)hxc_tmp_load_result_n461, (int32_t)hxc_tmp_load_result_n462, (int32_t)360, (int32_t)54, hxc_this1_h5ab6e6cdd588);
        }
        else
        {
          struct Color hxc_this1_h922c566f2fec = (struct Color){ .r = 6, .g = 26, .b = 36, .a = 218 };
          int32_t hxc_tmp_load_result_n465 = hxc_left_h0beaf76c31b0;
          int32_t hxc_tmp_load_result_n466 = hxc_top_hc34b0fec3157;
          DrawRectangle((int32_t)hxc_tmp_load_result_n465, (int32_t)hxc_tmp_load_result_n466, (int32_t)360, (int32_t)54, hxc_this1_h922c566f2fec);
          struct Color hxc_this1_h40172815f085 = (struct Color){ .r = 92, .g = 194, .b = 188, .a = 255 };
          int32_t hxc_tmp_load_result_n469 = hxc_left_h0beaf76c31b0;
          int32_t hxc_tmp_load_result_n470 = hxc_top_hc34b0fec3157;
          DrawRectangleLines((int32_t)hxc_tmp_load_result_n469, (int32_t)hxc_tmp_load_result_n470, (int32_t)360, (int32_t)54, hxc_this1_h40172815f085);
        }
        struct Color hxc_this1_h82029f00bf2f = (struct Color){ .r = 242, .g = 249, .b = 245, .a = 255 };
        int32_t hxc_tmp_load_result_n473 = hxc_left_h0beaf76c31b0;
        int32_t hxc_tmp_load_result_n474 = hxc_top_hc34b0fec3157;
        DrawText("ADVENTURE", (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n473, 28), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n474, 16), (int32_t)21, hxc_this1_h82029f00bf2f);
        int32_t hxc_x_h27f26600c5be = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_width_h0214ba48447b, (double)2)), 270);
        struct Color hxc_this1_hfe494e7f13a4 = (struct Color){ .r = 229, .g = 241, .b = 235, .a = 255 };
        int32_t hxc_tmp_load_result_n478 = hxc_x_h27f26600c5be;
        int32_t hxc_tmp_load_result_n479 = hxc_height_h5a762c27aa21;
        DrawText("ARROWS / MOUSE TO CHOOSE   ENTER TO PLAY   L: ESPANOL", (int32_t)hxc_tmp_load_result_n478, (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n479, 58), (int32_t)16, hxc_this1_hfe494e7f13a4);
      }
      if (hxc_selectedMode == 1)
      {
        if (hxc_language == 1)
        {
          int32_t hxc_x_h4fa99e976aea = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_width_h0214ba48447b, (double)2)), 210);
          struct Color hxc_this1_h0ee879d5f97c = (struct Color){ .r = 255, .g = 205, .b = 91, .a = 255 };
          int32_t hxc_tmp_load_result_n485 = hxc_x_h4fa99e976aea;
          int32_t hxc_tmp_load_result_n486 = hxc_firstTop;
          DrawText("SALVA A CEESH. ENCUENTRA A IVVY. DETEN A BROWSER.", (int32_t)hxc_tmp_load_result_n485, (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n486, 36), (int32_t)17, hxc_this1_h0ee879d5f97c);
        }
        else
        {
          int32_t hxc_x_h86fa438a2ba6 = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_width_h0214ba48447b, (double)2)), 210);
          struct Color hxc_this1_he31d35a0929d = (struct Color){ .r = 255, .g = 205, .b = 91, .a = 255 };
          int32_t hxc_tmp_load_result_n490 = hxc_x_h86fa438a2ba6;
          int32_t hxc_tmp_load_result_n491 = hxc_firstTop;
          DrawText("SAVE CEESH. FIND IVVY. STOP BROWSER.", (int32_t)hxc_tmp_load_result_n490, (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n491, 36), (int32_t)17, hxc_this1_he31d35a0929d);
        }
      }
    }
    else
    {
      ClearBackground((struct Color){ .r = 126, .g = 190, .b = 201, .a = 255 });
      BeginMode3D(hxc_camera);
      double hxc_tmp_record_field_load_result_n495 = hxc_player.hxc_x;
      struct hxc_caxecraft_app_Main_RenderCounters hxc_tmp_call_result_n497 = hxc_caxecraft_app_Main_drawWorld(hxc_cells, hxc_tmp_length_n1, hxc_tmp_record_field_load_result_n495, hxc_player.hxc_z);
      struct hxc_caxecraft_app_Main_RenderCounters hxc_renderCounters = hxc_tmp_call_result_n497;
      struct hxc_caxecraft_gameplay_GuideState hxc_tmp_load_result_n498 = hxc_guide;
      hxc_caxecraft_app_Main_drawActors(hxc_tmp_load_result_n498, hxc_mossling);
      if (hxc_hit.hxc_hit)
      {
        double hxc_y = (double)hxc_hit.hxc_cellY + 0.5;
        double hxc_z = (double)hxc_hit.hxc_cellZ + 0.5;
        int32_t hxc_tmp_record_field_load_result_n503 = hxc_hit.hxc_cellX;
        double hxc_tmp_load_result_n504 = hxc_y;
        DrawCubeWires((struct Vector3){ .x = (float)((double)hxc_tmp_record_field_load_result_n503 + 0.5), .y = (float)hxc_tmp_load_result_n504, .z = (float)hxc_z }, (float)1.04, (float)1.04, (float)1.04, (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 });
      }
      EndMode3D();
      int32_t hxc_tmp_record_field_load_result_n508 = hxc_renderCounters.hxc_visible;
      int32_t hxc_tmp_record_field_load_result_n509 = hxc_renderCounters.hxc_drawCalls;
      int32_t hxc_tmp_load_result_n510 = hxc_frameCount;
      int32_t hxc_tmp_load_result_n511 = hxc_updateCount;
      bool hxc_tmp_load_result_n512 = hxc_paused;
      bool hxc_tmp_load_result_n513 = hxc_captured;
      int32_t hxc_tmp_load_result_n514 = hxc_placementBlockedFrames;
      struct hxc_caxecraft_domain_RaycastHit hxc_tmp_load_result_n515 = hxc_hit;
      double hxc_tmp_record_field_load_result_n516 = hxc_player.hxc_x;
      double hxc_tmp_record_field_load_result_n517 = hxc_player.hxc_z;
      int32_t hxc_tmp_load_result_n518 = hxc_selectedMode;
      int32_t hxc_tmp_load_result_n519 = hxc_language;
      struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_load_result_n520 = hxc_inventory;
      struct hxc_caxecraft_gameplay_GuideState hxc_tmp_load_result_n521 = hxc_guide;
      struct hxc_caxecraft_gameplay_MosslingState hxc_tmp_load_result_n522 = hxc_mossling;
      struct Texture hxc_tmp_load_result_n523 = hxc_hudTexture;
      bool hxc_tmp_load_result_n524 = hxc_hudTextureReady;
      struct Texture hxc_tmp_load_result_n525 = hxc_itemTexture;
      hxc_caxecraft_app_Main_drawHud(hxc_tmp_record_field_load_result_n508, hxc_tmp_record_field_load_result_n509, hxc_tmp_load_result_n510, hxc_tmp_load_result_n511, hxc_tmp_load_result_n512, hxc_tmp_load_result_n513, hxc_tmp_load_result_n514 > 0, hxc_tmp_load_result_n515, hxc_tmp_record_field_load_result_n516, hxc_tmp_record_field_load_result_n517, hxc_tmp_load_result_n518, hxc_tmp_load_result_n519, hxc_tmp_load_result_n520, hxc_tmp_load_result_n521, hxc_tmp_load_result_n522, hxc_tmp_load_result_n523, hxc_tmp_load_result_n524, hxc_tmp_load_result_n525, hxc_itemTextureReady);
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
