#include "hxc/program.h"

void hxc_caxecraft_app_Main_drawHud(int32_t hxc_visible, int32_t hxc_drawCalls, int32_t hxc_frames, int32_t hxc_updates, bool hxc_paused, bool hxc_captured, bool hxc_placementBlocked, struct hxc_caxecraft_domain_RaycastHit hxc_hit)
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
  struct Color hxc_this1_h04a341c13bbd = (struct Color){ .r = 15, .g = 27, .b = 39, .a = 220 };
  DrawRectangle((int32_t)18, (int32_t)18, (int32_t)460, (int32_t)108, hxc_this1_h04a341c13bbd);
  struct Color hxc_this1_h4e660661f14b = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
  DrawRectangleLines((int32_t)18, (int32_t)18, (int32_t)460, (int32_t)108, hxc_this1_h4e660661f14b);
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
  int32_t hxc_tmp_load_result_n40 = hxc_height;
  DrawText("WASD MOVE  SPACE JUMP  LMB MINE  RMB PLACE  ESC PAUSE  Q QUIT", (int32_t)20, (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n40, 30), (int32_t)16, hxc_text);
  if (hxc_paused)
  {
    struct Color hxc_this1_he83b083eab07 = (struct Color){ .r = 15, .g = 27, .b = 39, .a = 220 };
    int32_t hxc_tmp_load_result_n43 = hxc_centerX;
    int32_t hxc_tmp_load_result_n44 = hxc_centerY;
    DrawRectangle((int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n43, 170), (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n44, 48), (int32_t)340, (int32_t)96, hxc_this1_he83b083eab07);
    struct Color hxc_this1_h92ebea122c8f = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
    int32_t hxc_tmp_load_result_n47 = hxc_centerX;
    int32_t hxc_tmp_load_result_n48 = hxc_centerY;
    DrawRectangleLines((int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n47, 170), (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n48, 48), (int32_t)340, (int32_t)96, hxc_this1_h92ebea122c8f);
    int32_t hxc_tmp_load_result_n50 = hxc_centerX;
    int32_t hxc_tmp_load_result_n51 = hxc_centerY;
    DrawText("PAUSED", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n50, 48), (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n51, 30), (int32_t)24, hxc_text);
    int32_t hxc_tmp_load_result_n53 = hxc_centerX;
    int32_t hxc_tmp_load_result_n54 = hxc_centerY;
    DrawText("CLICK TO CAPTURE  //  Q TO QUIT", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n53, 145), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n54, 8), (int32_t)16, hxc_text);
  }
  else
  {
    if (hxc_placementBlocked)
    {
      struct Color hxc_this1_hbb53ac86a7a0 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
      int32_t hxc_tmp_load_result_n57 = hxc_centerX;
      int32_t hxc_tmp_load_result_n58 = hxc_centerY;
      DrawText("PLACE BLOCKED: PLAYER OR WORLD EDGE", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n57, 150), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n58, 26), (int32_t)14, hxc_this1_hbb53ac86a7a0);
    }
    else
    {
      if (!hxc_captured)
      {
        int32_t hxc_tmp_load_result_n60 = hxc_centerX;
        int32_t hxc_tmp_load_result_n61 = hxc_centerY;
        DrawText("CLICK TO CAPTURE", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n60, 74), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n61, 26), (int32_t)14, hxc_text);
      }
      else
      {
        if (!hxc_hit.hxc_hit)
        {
          int32_t hxc_tmp_load_result_n64 = hxc_centerX;
          int32_t hxc_tmp_load_result_n65 = hxc_centerY;
          DrawText("NO BLOCK IN REACH", (int32_t)hxc_i32_subtract_wrapping(hxc_tmp_load_result_n64, 78), (int32_t)hxc_i32_add_wrapping(hxc_tmp_load_result_n65, 26), (int32_t)14, hxc_text);
        }
      }
    }
  }
  return;
}

struct hxc_caxecraft_app_Main_RenderCounters hxc_caxecraft_app_Main_drawWorld(uint8_t *hxc_cells, size_t hxc_length)
{
  uint8_t *hxc_borrow = hxc_cells;
  size_t hxc_tmp_length_n1 = hxc_length;
  (void)hxc_borrow;
  (void)hxc_tmp_length_n1;
  int32_t hxc_visible = 0;
  int32_t hxc_drawCalls = 0;
  int32_t hxc_z = 0;
  while (1)
  {
    if (!(hxc_z < 32))
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
      int32_t hxc_x = 0;
      while (1)
      {
        if (!(hxc_x < 32))
        {
          break;
        }
        int32_t hxc_tmp_load_result_n3 = hxc_x;
        int32_t hxc_tmp_load_result_n4 = hxc_y;
        struct hxc_caxecraft_domain_BlockCoord hxc_tmp_call_result_n6 = hxc_caxecraft_domain_World_coord(hxc_tmp_load_result_n3, hxc_tmp_load_result_n4, hxc_z);
        enum hxc_caxecraft_domain_BlockKind hxc_tmp_call_result_n7 = hxc_caxecraft_domain_World_query(hxc_cells, hxc_length, hxc_tmp_call_result_n6);
        enum hxc_caxecraft_domain_BlockKind hxc_kind = hxc_tmp_call_result_n7;
        bool hxc_tmp_call_result_n9 = hxc_caxecraft_domain_World_isSolid(hxc_kind);
        bool hxc_tmp_short_circuit_result_n8 = hxc_tmp_call_result_n9;
        if (hxc_tmp_call_result_n9)
        {
          int32_t hxc_tmp_load_result_n10 = hxc_x;
          int32_t hxc_tmp_load_result_n11 = hxc_y;
          bool hxc_tmp_call_result_n13 = hxc_caxecraft_app_Main_isExposed(hxc_cells, hxc_length, hxc_tmp_load_result_n10, hxc_tmp_load_result_n11, hxc_z);
          hxc_tmp_short_circuit_result_n8 = hxc_tmp_call_result_n13;
        }
        if (hxc_tmp_short_circuit_result_n8)
        {
          hxc_visible = hxc_i32_add_wrapping(hxc_visible, 1);
          hxc_drawCalls = hxc_i32_add_wrapping(hxc_drawCalls, 2);
          int32_t hxc_tmp_load_result_n17 = hxc_x;
          int32_t hxc_tmp_load_result_n18 = hxc_y;
          int32_t hxc_tmp_load_result_n19 = hxc_z;
          struct Color hxc_tmp_call_result_n22 = hxc_caxecraft_app_CaxecraftPalette_block(hxc_kind);
          DrawCube((struct Vector3){ .x = (float)((double)hxc_tmp_load_result_n17 + 0.5), .y = (float)((double)hxc_tmp_load_result_n18 + 0.5), .z = (float)((double)hxc_tmp_load_result_n19 + 0.5) }, (float)1.0, (float)1.0, (float)1.0, hxc_tmp_call_result_n22);
          int32_t hxc_tmp_load_result_n23 = hxc_x;
          int32_t hxc_tmp_load_result_n24 = hxc_y;
          int32_t hxc_tmp_load_result_n25 = hxc_z;
          struct Color hxc_tmp_call_result_n28 = hxc_caxecraft_app_CaxecraftPalette_blockOutline(hxc_kind);
          DrawCubeWires((struct Vector3){ .x = (float)((double)hxc_tmp_load_result_n23 + 0.5), .y = (float)((double)hxc_tmp_load_result_n24 + 0.5), .z = (float)((double)hxc_tmp_load_result_n25 + 0.5) }, (float)1.002, (float)1.002, (float)1.002, hxc_tmp_call_result_n28);
        }
        hxc_x = hxc_i32_add_wrapping(hxc_x, 1);
      }
      hxc_y = hxc_i32_add_wrapping(hxc_y, 1);
    }
    hxc_z = hxc_i32_add_wrapping(hxc_z, 1);
  }
  int32_t hxc_tmp_load_result_n32 = hxc_visible;
  return (struct hxc_caxecraft_app_Main_RenderCounters){ .hxc_drawCalls = hxc_drawCalls, .hxc_visible = hxc_tmp_load_result_n32 };
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
  SetConfigFlags((uint32_t)hxc_i32_bit_or(hxc_i32_bit_or(64, 4), 8192));
  InitWindow((int32_t)1280, (int32_t)720, "Caxecraft \342\200\224 Haxe shaped into careful C");
  SetWindowMinSize((int32_t)800, (int32_t)450);
  SetExitKey((int32_t)0);
  SetTargetFPS((int32_t)120);
  DisableCursor();
  struct hxc_caxecraft_domain_PlayerState hxc_tmp_call_result_n0 = hxc_caxecraft_domain_PlayerPhysics_player(16.5, 10.0, 16.5);
  struct hxc_caxecraft_domain_PlayerState hxc_tmp_call_result_n1 = hxc_caxecraft_domain_PlayerPhysics_recoverSpawn(hxc_cells, hxc_tmp_length_n1, hxc_tmp_call_result_n0);
  struct hxc_caxecraft_domain_PlayerState hxc_player = hxc_tmp_call_result_n1;
  double hxc_lookX = 0.0;
  double hxc_lookY = -0.18;
  double hxc_lookZ = -1.0;
  double hxc_accumulator = 0.0;
  bool hxc_jumpQueued = false;
  bool hxc_paused = false;
  bool hxc_captured = true;
  bool hxc_quit = false;
  int32_t hxc_frameCount = 0;
  int32_t hxc_updateCount = 0;
  int32_t hxc_placementBlockedFrames = 0;
  while (1)
  {
    bool hxc_tmp_load_result_n2 = hxc_quit;
    bool hxc_tmp_short_circuit_result_n14 = !hxc_tmp_load_result_n2;
    if (!hxc_tmp_load_result_n2)
    {
      bool hxc_tmp_native_call_result_n3 = WindowShouldClose();
      hxc_tmp_short_circuit_result_n14 = !hxc_tmp_native_call_result_n3;
    }
    if (!hxc_tmp_short_circuit_result_n14)
    {
      break;
    }
    bool hxc_recapturedThisFrame = false;
    bool hxc_tmp_native_call_result_n5 = IsWindowFocused();
    bool hxc_focused = hxc_tmp_native_call_result_n5;
    bool hxc_tmp_load_result_n6 = hxc_focused;
    bool hxc_tmp_short_circuit_result_n17 = !hxc_tmp_load_result_n6;
    if (!hxc_tmp_load_result_n6)
    {
      hxc_tmp_short_circuit_result_n17 = hxc_captured;
    }
    if (hxc_tmp_short_circuit_result_n17)
    {
      hxc_paused = true;
      hxc_captured = false;
      hxc_jumpQueued = false;
      EnableCursor();
    }
    bool hxc_tmp_native_call_result_n9 = IsKeyPressed((int32_t)81);
    if (hxc_tmp_native_call_result_n9)
    {
      hxc_quit = true;
    }
    bool hxc_tmp_load_result_n10 = hxc_focused;
    bool hxc_tmp_short_circuit_result_n18 = hxc_tmp_load_result_n10;
    if (hxc_tmp_load_result_n10)
    {
      bool hxc_tmp_native_call_result_n11 = IsKeyPressed((int32_t)256);
      hxc_tmp_short_circuit_result_n18 = hxc_tmp_native_call_result_n11;
    }
    if (hxc_tmp_short_circuit_result_n18)
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
    bool hxc_tmp_load_result_n17 = hxc_focused;
    bool hxc_tmp_short_circuit_result_n19 = hxc_tmp_load_result_n17;
    if (hxc_tmp_load_result_n17)
    {
      hxc_tmp_short_circuit_result_n19 = hxc_paused;
    }
    bool hxc_tmp_short_circuit_load_result_n19 = hxc_tmp_short_circuit_result_n19;
    bool hxc_tmp_short_circuit_result_n20 = hxc_tmp_short_circuit_load_result_n19;
    if (hxc_tmp_short_circuit_load_result_n19)
    {
      bool hxc_tmp_native_call_result_n20 = IsMouseButtonPressed((int32_t)0);
      hxc_tmp_short_circuit_result_n20 = hxc_tmp_native_call_result_n20;
    }
    if (hxc_tmp_short_circuit_result_n20)
    {
      hxc_paused = false;
      hxc_captured = true;
      hxc_recapturedThisFrame = true;
      DisableCursor();
    }
    if (hxc_captured)
    {
      struct Vector2 hxc_tmp_native_call_result_n23 = GetMouseDelta();
      struct Vector2 hxc_mouse = hxc_tmp_native_call_result_n23;
      double hxc_yawDelta = (double)hxc_mouse.x * 0.0025;
      if (hxc_yawDelta > 0.25)
      {
        hxc_yawDelta = 0.25;
      }
      if (hxc_yawDelta < -0.25)
      {
        hxc_yawDelta = -0.25;
      }
      double hxc_tmp_load_result_n28 = hxc_lookX;
      double hxc_tmp_load_result_n29 = hxc_yawDelta;
      double hxc_candidateX = hxc_tmp_load_result_n28 + hxc_tmp_load_result_n29 * hxc_lookZ;
      double hxc_tmp_load_result_n31 = hxc_lookZ;
      double hxc_tmp_load_result_n32 = hxc_yawDelta;
      double hxc_candidateZ = hxc_tmp_load_result_n31 - hxc_tmp_load_result_n32 * hxc_lookX;
      double hxc_tmp_load_result_n34 = hxc_candidateX;
      double hxc_tmp_load_result_n35 = hxc_candidateX;
      double hxc_tmp_load_result_n36 = hxc_candidateZ;
      double hxc_lengthSquared = hxc_tmp_load_result_n34 * hxc_tmp_load_result_n35 + hxc_tmp_load_result_n36 * hxc_candidateZ;
      double hxc_normalize = 1.5 - 0.5 * hxc_lengthSquared;
      double hxc_tmp_load_result_n39 = hxc_candidateX;
      hxc_lookX = hxc_tmp_load_result_n39 * hxc_normalize;
      double hxc_tmp_load_result_n41 = hxc_candidateZ;
      hxc_lookZ = hxc_tmp_load_result_n41 * hxc_normalize;
      double hxc_tmp_compound_load_result_n43 = hxc_lookY;
      hxc_lookY = hxc_tmp_compound_load_result_n43 - (double)hxc_mouse.y * 0.0025;
      if (hxc_lookY > 0.90)
      {
        hxc_lookY = 0.90;
      }
      if (hxc_lookY < -0.90)
      {
        hxc_lookY = -0.90;
      }
      bool hxc_tmp_native_call_result_n48 = IsKeyPressed((int32_t)32);
      if (hxc_tmp_native_call_result_n48)
      {
        hxc_jumpQueued = true;
      }
    }
    float hxc_tmp_native_call_result_n49 = GetFrameTime();
    double hxc_frameSeconds = (double)hxc_tmp_native_call_result_n49;
    if (hxc_frameSeconds > 0.25)
    {
      hxc_frameSeconds = 0.25;
    }
    if (!hxc_paused)
    {
      double hxc_tmp_compound_load_result_n52 = hxc_accumulator;
      hxc_accumulator = hxc_tmp_compound_load_result_n52 + hxc_frameSeconds;
    }
    while (1)
    {
      bool hxc_tmp_load_result_n54 = hxc_paused;
      bool hxc_tmp_short_circuit_result_n28 = !hxc_tmp_load_result_n54;
      if (!hxc_tmp_load_result_n54)
      {
        hxc_tmp_short_circuit_result_n28 = hxc_accumulator >= 0.05;
      }
      if (!hxc_tmp_short_circuit_result_n28)
      {
        break;
      }
      double hxc_moveX = 0.0;
      double hxc_moveZ = 0.0;
      bool hxc_tmp_native_call_result_n57 = IsKeyDown((int32_t)87);
      if (hxc_tmp_native_call_result_n57)
      {
        double hxc_tmp_compound_load_result_n58 = hxc_moveX;
        hxc_moveX = hxc_tmp_compound_load_result_n58 + hxc_lookX;
        double hxc_tmp_compound_load_result_n60 = hxc_moveZ;
        hxc_moveZ = hxc_tmp_compound_load_result_n60 + hxc_lookZ;
      }
      bool hxc_tmp_native_call_result_n62 = IsKeyDown((int32_t)83);
      if (hxc_tmp_native_call_result_n62)
      {
        double hxc_tmp_compound_load_result_n63 = hxc_moveX;
        hxc_moveX = hxc_tmp_compound_load_result_n63 - hxc_lookX;
        double hxc_tmp_compound_load_result_n65 = hxc_moveZ;
        hxc_moveZ = hxc_tmp_compound_load_result_n65 - hxc_lookZ;
      }
      bool hxc_tmp_native_call_result_n67 = IsKeyDown((int32_t)65);
      if (hxc_tmp_native_call_result_n67)
      {
        double hxc_tmp_compound_load_result_n68 = hxc_moveX;
        hxc_moveX = hxc_tmp_compound_load_result_n68 + hxc_lookZ;
        double hxc_tmp_compound_load_result_n70 = hxc_moveZ;
        hxc_moveZ = hxc_tmp_compound_load_result_n70 - hxc_lookX;
      }
      bool hxc_tmp_native_call_result_n72 = IsKeyDown((int32_t)68);
      if (hxc_tmp_native_call_result_n72)
      {
        double hxc_tmp_compound_load_result_n73 = hxc_moveX;
        hxc_moveX = hxc_tmp_compound_load_result_n73 - hxc_lookZ;
        double hxc_tmp_compound_load_result_n75 = hxc_moveZ;
        hxc_moveZ = hxc_tmp_compound_load_result_n75 + hxc_lookX;
      }
      bool hxc_tmp_native_call_result_n77 = IsKeyDown((int32_t)87);
      bool hxc_tmp_short_circuit_result_n32 = hxc_tmp_native_call_result_n77;
      if (!hxc_tmp_native_call_result_n77)
      {
        bool hxc_tmp_native_call_result_n78 = IsKeyDown((int32_t)83);
        hxc_tmp_short_circuit_result_n32 = hxc_tmp_native_call_result_n78;
      }
      bool hxc_forwardHeld = hxc_tmp_short_circuit_result_n32;
      bool hxc_tmp_native_call_result_n80 = IsKeyDown((int32_t)65);
      bool hxc_tmp_short_circuit_result_n34 = hxc_tmp_native_call_result_n80;
      if (!hxc_tmp_native_call_result_n80)
      {
        bool hxc_tmp_native_call_result_n81 = IsKeyDown((int32_t)68);
        hxc_tmp_short_circuit_result_n34 = hxc_tmp_native_call_result_n81;
      }
      bool hxc_sideHeld = hxc_tmp_short_circuit_result_n34;
      bool hxc_tmp_load_result_n83 = hxc_forwardHeld;
      bool hxc_tmp_short_circuit_result_n35 = hxc_tmp_load_result_n83;
      if (hxc_tmp_load_result_n83)
      {
        hxc_tmp_short_circuit_result_n35 = hxc_sideHeld;
      }
      if (hxc_tmp_short_circuit_result_n35)
      {
        hxc_moveX = hxc_moveX * 0.7071067811865476;
        hxc_moveZ = hxc_moveZ * 0.7071067811865476;
      }
      struct hxc_caxecraft_domain_PlayerState hxc_tmp_load_result_n88 = hxc_player;
      double hxc_tmp_load_result_n89 = hxc_moveX;
      double hxc_tmp_load_result_n90 = hxc_moveZ;
      struct hxc_caxecraft_domain_StepInput hxc_tmp_call_result_n92 = hxc_caxecraft_domain_PlayerPhysics_input(hxc_tmp_load_result_n89, hxc_tmp_load_result_n90, hxc_jumpQueued);
      struct hxc_caxecraft_domain_PlayerState hxc_tmp_call_result_n93 = hxc_caxecraft_domain_PlayerPhysics_step(hxc_cells, hxc_tmp_length_n1, hxc_tmp_load_result_n88, hxc_tmp_call_result_n92);
      hxc_player = hxc_tmp_call_result_n93;
      hxc_jumpQueued = false;
      hxc_accumulator = hxc_accumulator - 0.05;
      hxc_updateCount = hxc_i32_add_wrapping(hxc_updateCount, 1);
    }
    double hxc_eyeX = hxc_player.hxc_x;
    double hxc_eyeY = hxc_player.hxc_y + 1.62;
    double hxc_eyeZ = hxc_player.hxc_z;
    double hxc_tmp_load_result_n99 = hxc_eyeX;
    double hxc_tmp_load_result_n100 = hxc_eyeY;
    double hxc_tmp_load_result_n101 = hxc_eyeZ;
    double hxc_tmp_load_result_n102 = hxc_lookX;
    double hxc_tmp_load_result_n103 = hxc_lookY;
    struct hxc_caxecraft_domain_RaycastHit hxc_tmp_call_result_n105 = hxc_caxecraft_domain_VoxelRaycast_trace(hxc_cells, hxc_tmp_length_n1, hxc_tmp_load_result_n99, hxc_tmp_load_result_n100, hxc_tmp_load_result_n101, hxc_tmp_load_result_n102, hxc_tmp_load_result_n103, hxc_lookZ, 7.0);
    struct hxc_caxecraft_domain_RaycastHit hxc_hit = hxc_tmp_call_result_n105;
    bool hxc_tmp_load_result_n106 = hxc_captured;
    bool hxc_tmp_short_circuit_result_n40 = hxc_tmp_load_result_n106;
    if (hxc_tmp_load_result_n106)
    {
      hxc_tmp_short_circuit_result_n40 = !hxc_recapturedThisFrame;
    }
    bool hxc_tmp_short_circuit_load_result_n108 = hxc_tmp_short_circuit_result_n40;
    bool hxc_tmp_short_circuit_result_n41 = hxc_tmp_short_circuit_load_result_n108;
    if (hxc_tmp_short_circuit_load_result_n108)
    {
      hxc_tmp_short_circuit_result_n41 = hxc_hit.hxc_hit;
    }
    bool hxc_tmp_short_circuit_load_result_n110 = hxc_tmp_short_circuit_result_n41;
    bool hxc_tmp_short_circuit_result_n42 = hxc_tmp_short_circuit_load_result_n110;
    if (hxc_tmp_short_circuit_load_result_n110)
    {
      bool hxc_tmp_native_call_result_n111 = IsMouseButtonPressed((int32_t)0);
      hxc_tmp_short_circuit_result_n42 = hxc_tmp_native_call_result_n111;
    }
    if (hxc_tmp_short_circuit_result_n42)
    {
      int32_t hxc_tmp_record_field_load_result_n113 = hxc_hit.hxc_cellX;
      int32_t hxc_tmp_record_field_load_result_n114 = hxc_hit.hxc_cellY;
      struct hxc_caxecraft_domain_BlockCoord hxc_tmp_call_result_n116 = hxc_caxecraft_domain_World_coord(hxc_tmp_record_field_load_result_n113, hxc_tmp_record_field_load_result_n114, hxc_hit.hxc_cellZ);
      hxc_caxecraft_domain_World_remove(hxc_cells, hxc_tmp_length_n1, hxc_tmp_call_result_n116);
    }
    bool hxc_tmp_load_result_n117 = hxc_captured;
    bool hxc_tmp_short_circuit_result_n43 = hxc_tmp_load_result_n117;
    if (hxc_tmp_load_result_n117)
    {
      hxc_tmp_short_circuit_result_n43 = hxc_hit.hxc_hit;
    }
    bool hxc_tmp_short_circuit_load_result_n119 = hxc_tmp_short_circuit_result_n43;
    bool hxc_tmp_short_circuit_result_n44 = hxc_tmp_short_circuit_load_result_n119;
    if (hxc_tmp_short_circuit_load_result_n119)
    {
      bool hxc_tmp_native_call_result_n120 = IsMouseButtonPressed((int32_t)1);
      hxc_tmp_short_circuit_result_n44 = hxc_tmp_native_call_result_n120;
    }
    if (hxc_tmp_short_circuit_result_n44)
    {
      int32_t hxc_tmp_record_field_load_result_n122 = hxc_hit.hxc_previousX;
      int32_t hxc_tmp_record_field_load_result_n123 = hxc_hit.hxc_previousY;
      struct hxc_caxecraft_domain_BlockCoord hxc_tmp_call_result_n125 = hxc_caxecraft_domain_World_coord(hxc_tmp_record_field_load_result_n122, hxc_tmp_record_field_load_result_n123, hxc_hit.hxc_previousZ);
      struct hxc_caxecraft_domain_BlockCoord hxc_placement = hxc_tmp_call_result_n125;
      struct hxc_caxecraft_domain_PlayerState hxc_tmp_load_result_n126 = hxc_player;
      bool hxc_tmp_call_result_n128 = hxc_caxecraft_domain_PlayerPhysics_canPlaceAt(hxc_tmp_load_result_n126, hxc_placement);
      bool hxc_tmp_short_circuit_result_n46 = !hxc_tmp_call_result_n128;
      if (!!hxc_tmp_call_result_n128)
      {
        bool hxc_tmp_call_result_n131 = hxc_caxecraft_domain_World_place(hxc_cells, hxc_tmp_length_n1, hxc_placement, hxc_caxecraft_domain_BlockKind_Dirt);
        hxc_tmp_short_circuit_result_n46 = !hxc_tmp_call_result_n131;
      }
      if (hxc_tmp_short_circuit_result_n46)
      {
        hxc_placementBlockedFrames = 60;
      }
    }
    if (hxc_placementBlockedFrames > 0)
    {
      hxc_placementBlockedFrames = hxc_i32_subtract_wrapping(hxc_placementBlockedFrames, 1);
    }
    double hxc_tmp_load_result_n135 = hxc_eyeX;
    double hxc_tmp_load_result_n136 = hxc_eyeY;
    double hxc_tmp_load_result_n137 = hxc_eyeZ;
    double hxc_tmp_load_result_n139 = hxc_eyeX;
    double hxc_tmp_load_result_n140 = hxc_lookX;
    double hxc_tmp_load_result_n141 = hxc_eyeY;
    double hxc_tmp_load_result_n142 = hxc_lookY;
    double hxc_tmp_load_result_n143 = hxc_eyeZ;
    struct Camera3D hxc_camera = (struct Camera3D){ .position = (struct Vector3){ .x = (float)hxc_tmp_load_result_n135, .y = (float)hxc_tmp_load_result_n136, .z = (float)hxc_tmp_load_result_n137 }, .target = (struct Vector3){ .x = (float)(hxc_tmp_load_result_n139 + hxc_tmp_load_result_n140), .y = (float)(hxc_tmp_load_result_n141 + hxc_tmp_load_result_n142), .z = (float)(hxc_tmp_load_result_n143 + hxc_lookZ) }, .up = (struct Vector3){ .x = (float)0.0, .y = (float)1.0, .z = (float)0.0 }, .fovy = (float)70.0, .projection = 0 };
    BeginDrawing();
    ClearBackground((struct Color){ .r = 126, .g = 190, .b = 201, .a = 255 });
    BeginMode3D(hxc_camera);
    struct hxc_caxecraft_app_Main_RenderCounters hxc_tmp_call_result_n150 = hxc_caxecraft_app_Main_drawWorld(hxc_cells, hxc_tmp_length_n1);
    struct hxc_caxecraft_app_Main_RenderCounters hxc_renderCounters = hxc_tmp_call_result_n150;
    if (hxc_hit.hxc_hit)
    {
      double hxc_y = (double)hxc_hit.hxc_cellY + 0.5;
      double hxc_z = (double)hxc_hit.hxc_cellZ + 0.5;
      int32_t hxc_tmp_record_field_load_result_n154 = hxc_hit.hxc_cellX;
      double hxc_tmp_load_result_n155 = hxc_y;
      DrawCubeWires((struct Vector3){ .x = (float)((double)hxc_tmp_record_field_load_result_n154 + 0.5), .y = (float)hxc_tmp_load_result_n155, .z = (float)hxc_z }, (float)1.04, (float)1.04, (float)1.04, (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 });
    }
    EndMode3D();
    int32_t hxc_tmp_record_field_load_result_n159 = hxc_renderCounters.hxc_visible;
    int32_t hxc_tmp_record_field_load_result_n160 = hxc_renderCounters.hxc_drawCalls;
    int32_t hxc_tmp_load_result_n161 = hxc_frameCount;
    int32_t hxc_tmp_load_result_n162 = hxc_updateCount;
    bool hxc_tmp_load_result_n163 = hxc_paused;
    bool hxc_tmp_load_result_n164 = hxc_captured;
    int32_t hxc_tmp_load_result_n165 = hxc_placementBlockedFrames;
    hxc_caxecraft_app_Main_drawHud(hxc_tmp_record_field_load_result_n159, hxc_tmp_record_field_load_result_n160, hxc_tmp_load_result_n161, hxc_tmp_load_result_n162, hxc_tmp_load_result_n163, hxc_tmp_load_result_n164, hxc_tmp_load_result_n165 > 0, hxc_hit);
    EndDrawing();
    hxc_frameCount = hxc_i32_add_wrapping(hxc_frameCount, 1);
  }
  EnableCursor();
  CloseWindow();
  return;
}
