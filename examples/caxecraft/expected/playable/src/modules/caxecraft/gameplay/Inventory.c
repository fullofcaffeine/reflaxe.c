#include "hxc/program.h"

int32_t hxc_caxecraft_gameplay_Inventory_acceptedAmount(struct hxc_caxecraft_gameplay_InventoryState hxc_l_state, enum hxc_caxecraft_gameplay_ItemKind hxc_l_item, int32_t hxc_l_offered)
{
  if (hxc_l_offered <= 0)
  {
    return 0;
  }
  int32_t hxc_l_tmp_call_result_n0 = hxc_caxecraft_gameplay_Inventory_countItem(hxc_l_state, hxc_l_item);
  int32_t hxc_l_space = hxc_i32_subtract_wrapping(64, hxc_l_tmp_call_result_n0);
  if (hxc_l_space <= 0)
  {
    return 0;
  }
  if (!(hxc_l_offered < hxc_l_space))
  {
    return hxc_l_space;
  }
  return hxc_l_offered;
}

struct hxc_caxecraft_gameplay_InventoryState hxc_caxecraft_gameplay_Inventory_collectBlock(struct hxc_caxecraft_gameplay_InventoryState hxc_l_state, enum hxc_caxecraft_domain_BlockKind hxc_l_kind)
{
  switch (hxc_l_kind) {
    case hxc_caxecraft_domain_BlockKind_Grass:
      {
        break;
      }
    case hxc_caxecraft_domain_BlockKind_Dirt:
      {
        struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n21 = hxc_caxecraft_gameplay_Inventory_make(hxc_l_state.hxc_selected, hxc_l_state.hxc_grass, hxc_i32_add_wrapping(hxc_l_state.hxc_dirt, 1), hxc_l_state.hxc_stone, hxc_l_state.hxc_haxeforge, hxc_l_state.hxc_sword, hxc_l_state.hxc_berries, hxc_l_state.hxc_bread, hxc_l_state.hxc_lantern, hxc_l_state.hxc_sand);
        return hxc_l_tmp_call_result_n21;
      }
    case hxc_caxecraft_domain_BlockKind_Stone:
      {
        struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n32 = hxc_caxecraft_gameplay_Inventory_make(hxc_l_state.hxc_selected, hxc_l_state.hxc_grass, hxc_l_state.hxc_dirt, hxc_i32_add_wrapping(hxc_l_state.hxc_stone, 1), hxc_l_state.hxc_haxeforge, hxc_l_state.hxc_sword, hxc_l_state.hxc_berries, hxc_l_state.hxc_bread, hxc_l_state.hxc_lantern, hxc_l_state.hxc_sand);
        return hxc_l_tmp_call_result_n32;
      }
    case hxc_caxecraft_domain_BlockKind_Sand:
      {
        struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n43 = hxc_caxecraft_gameplay_Inventory_make(hxc_l_state.hxc_selected, hxc_l_state.hxc_grass, hxc_l_state.hxc_dirt, hxc_l_state.hxc_stone, hxc_l_state.hxc_haxeforge, hxc_l_state.hxc_sword, hxc_l_state.hxc_berries, hxc_l_state.hxc_bread, hxc_l_state.hxc_lantern, hxc_i32_add_wrapping(hxc_l_state.hxc_sand, 1));
        return hxc_l_tmp_call_result_n43;
      }
    case hxc_caxecraft_domain_BlockKind_Air:
    case hxc_caxecraft_domain_BlockKind_Bedrock:
    case hxc_caxecraft_domain_BlockKind_Wood:
    case hxc_caxecraft_domain_BlockKind_Leaves:
    case hxc_caxecraft_domain_BlockKind_Snow:
    case hxc_caxecraft_domain_BlockKind_Ash:
      {
        return hxc_l_state;
      }
    default:
      {
        abort();
      }
  }
  struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n10 = hxc_caxecraft_gameplay_Inventory_make(hxc_l_state.hxc_selected, hxc_i32_add_wrapping(hxc_l_state.hxc_grass, 1), hxc_l_state.hxc_dirt, hxc_l_state.hxc_stone, hxc_l_state.hxc_haxeforge, hxc_l_state.hxc_sword, hxc_l_state.hxc_berries, hxc_l_state.hxc_bread, hxc_l_state.hxc_lantern, hxc_l_state.hxc_sand);
  return hxc_l_tmp_call_result_n10;
}

struct hxc_caxecraft_gameplay_InventoryState hxc_caxecraft_gameplay_Inventory_collectItem(struct hxc_caxecraft_gameplay_InventoryState hxc_l_state, enum hxc_caxecraft_gameplay_ItemKind hxc_l_kind, int32_t hxc_l_amount)
{
  int32_t hxc_l_tmp_call_result_n0 = hxc_caxecraft_gameplay_Inventory_acceptedAmount(hxc_l_state, hxc_l_kind, hxc_l_amount);
  int32_t hxc_l_accepted = hxc_l_tmp_call_result_n0;
  if (hxc_l_accepted <= 0)
  {
    return hxc_l_state;
  }
  switch (hxc_l_kind) {
    case hxc_caxecraft_gameplay_ItemKind_GrassBlock:
      {
        break;
      }
    case hxc_caxecraft_gameplay_ItemKind_DirtBlock:
      {
        struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n25 = hxc_caxecraft_gameplay_Inventory_make(hxc_l_state.hxc_selected, hxc_l_state.hxc_grass, hxc_i32_add_wrapping(hxc_l_state.hxc_dirt, hxc_l_accepted), hxc_l_state.hxc_stone, hxc_l_state.hxc_haxeforge, hxc_l_state.hxc_sword, hxc_l_state.hxc_berries, hxc_l_state.hxc_bread, hxc_l_state.hxc_lantern, hxc_l_state.hxc_sand);
        return hxc_l_tmp_call_result_n25;
      }
    case hxc_caxecraft_gameplay_ItemKind_StoneBlock:
      {
        struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n37 = hxc_caxecraft_gameplay_Inventory_make(hxc_l_state.hxc_selected, hxc_l_state.hxc_grass, hxc_l_state.hxc_dirt, hxc_i32_add_wrapping(hxc_l_state.hxc_stone, hxc_l_accepted), hxc_l_state.hxc_haxeforge, hxc_l_state.hxc_sword, hxc_l_state.hxc_berries, hxc_l_state.hxc_bread, hxc_l_state.hxc_lantern, hxc_l_state.hxc_sand);
        return hxc_l_tmp_call_result_n37;
      }
    case hxc_caxecraft_gameplay_ItemKind_Haxeforge:
      {
        struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n49 = hxc_caxecraft_gameplay_Inventory_make(hxc_l_state.hxc_selected, hxc_l_state.hxc_grass, hxc_l_state.hxc_dirt, hxc_l_state.hxc_stone, hxc_i32_add_wrapping(hxc_l_state.hxc_haxeforge, hxc_l_accepted), hxc_l_state.hxc_sword, hxc_l_state.hxc_berries, hxc_l_state.hxc_bread, hxc_l_state.hxc_lantern, hxc_l_state.hxc_sand);
        return hxc_l_tmp_call_result_n49;
      }
    case hxc_caxecraft_gameplay_ItemKind_CopperSword:
      {
        struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n61 = hxc_caxecraft_gameplay_Inventory_make(hxc_l_state.hxc_selected, hxc_l_state.hxc_grass, hxc_l_state.hxc_dirt, hxc_l_state.hxc_stone, hxc_l_state.hxc_haxeforge, hxc_i32_add_wrapping(hxc_l_state.hxc_sword, hxc_l_accepted), hxc_l_state.hxc_berries, hxc_l_state.hxc_bread, hxc_l_state.hxc_lantern, hxc_l_state.hxc_sand);
        return hxc_l_tmp_call_result_n61;
      }
    case hxc_caxecraft_gameplay_ItemKind_Berries:
      {
        struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n73 = hxc_caxecraft_gameplay_Inventory_make(hxc_l_state.hxc_selected, hxc_l_state.hxc_grass, hxc_l_state.hxc_dirt, hxc_l_state.hxc_stone, hxc_l_state.hxc_haxeforge, hxc_l_state.hxc_sword, hxc_i32_add_wrapping(hxc_l_state.hxc_berries, hxc_l_accepted), hxc_l_state.hxc_bread, hxc_l_state.hxc_lantern, hxc_l_state.hxc_sand);
        return hxc_l_tmp_call_result_n73;
      }
    case hxc_caxecraft_gameplay_ItemKind_Bread:
      {
        struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n85 = hxc_caxecraft_gameplay_Inventory_make(hxc_l_state.hxc_selected, hxc_l_state.hxc_grass, hxc_l_state.hxc_dirt, hxc_l_state.hxc_stone, hxc_l_state.hxc_haxeforge, hxc_l_state.hxc_sword, hxc_l_state.hxc_berries, hxc_i32_add_wrapping(hxc_l_state.hxc_bread, hxc_l_accepted), hxc_l_state.hxc_lantern, hxc_l_state.hxc_sand);
        return hxc_l_tmp_call_result_n85;
      }
    case hxc_caxecraft_gameplay_ItemKind_Lantern:
      {
        struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n97 = hxc_caxecraft_gameplay_Inventory_make(hxc_l_state.hxc_selected, hxc_l_state.hxc_grass, hxc_l_state.hxc_dirt, hxc_l_state.hxc_stone, hxc_l_state.hxc_haxeforge, hxc_l_state.hxc_sword, hxc_l_state.hxc_berries, hxc_l_state.hxc_bread, hxc_i32_add_wrapping(hxc_l_state.hxc_lantern, hxc_l_accepted), hxc_l_state.hxc_sand);
        return hxc_l_tmp_call_result_n97;
      }
    case hxc_caxecraft_gameplay_ItemKind_SandBlock:
      {
        struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n109 = hxc_caxecraft_gameplay_Inventory_make(hxc_l_state.hxc_selected, hxc_l_state.hxc_grass, hxc_l_state.hxc_dirt, hxc_l_state.hxc_stone, hxc_l_state.hxc_haxeforge, hxc_l_state.hxc_sword, hxc_l_state.hxc_berries, hxc_l_state.hxc_bread, hxc_l_state.hxc_lantern, hxc_i32_add_wrapping(hxc_l_state.hxc_sand, hxc_l_accepted));
        return hxc_l_tmp_call_result_n109;
      }
    default:
      {
        abort();
      }
  }
  struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n13 = hxc_caxecraft_gameplay_Inventory_make(hxc_l_state.hxc_selected, hxc_i32_add_wrapping(hxc_l_state.hxc_grass, hxc_l_accepted), hxc_l_state.hxc_dirt, hxc_l_state.hxc_stone, hxc_l_state.hxc_haxeforge, hxc_l_state.hxc_sword, hxc_l_state.hxc_berries, hxc_l_state.hxc_bread, hxc_l_state.hxc_lantern, hxc_l_state.hxc_sand);
  return hxc_l_tmp_call_result_n13;
}

struct hxc_caxecraft_gameplay_InventoryState hxc_caxecraft_gameplay_Inventory_consumeSelected(struct hxc_caxecraft_gameplay_InventoryState hxc_l_state)
{
  int32_t hxc_l_tmp_call_result_n1 = hxc_caxecraft_gameplay_Inventory_countAt(hxc_l_state, hxc_l_state.hxc_selected);
  if (hxc_l_tmp_call_result_n1 <= 0)
  {
    return hxc_l_state;
  }
  switch (hxc_l_state.hxc_selected) {
    case 0:
      {
        break;
      }
    case 1:
      {
        struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n24 = hxc_caxecraft_gameplay_Inventory_make(hxc_l_state.hxc_selected, hxc_l_state.hxc_grass, hxc_i32_subtract_wrapping(hxc_l_state.hxc_dirt, 1), hxc_l_state.hxc_stone, hxc_l_state.hxc_haxeforge, hxc_l_state.hxc_sword, hxc_l_state.hxc_berries, hxc_l_state.hxc_bread, hxc_l_state.hxc_lantern, hxc_l_state.hxc_sand);
        return hxc_l_tmp_call_result_n24;
      }
    case 2:
      {
        struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n35 = hxc_caxecraft_gameplay_Inventory_make(hxc_l_state.hxc_selected, hxc_l_state.hxc_grass, hxc_l_state.hxc_dirt, hxc_i32_subtract_wrapping(hxc_l_state.hxc_stone, 1), hxc_l_state.hxc_haxeforge, hxc_l_state.hxc_sword, hxc_l_state.hxc_berries, hxc_l_state.hxc_bread, hxc_l_state.hxc_lantern, hxc_l_state.hxc_sand);
        return hxc_l_tmp_call_result_n35;
      }
    case 3:
      {
        struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n46 = hxc_caxecraft_gameplay_Inventory_make(hxc_l_state.hxc_selected, hxc_l_state.hxc_grass, hxc_l_state.hxc_dirt, hxc_l_state.hxc_stone, hxc_i32_subtract_wrapping(hxc_l_state.hxc_haxeforge, 1), hxc_l_state.hxc_sword, hxc_l_state.hxc_berries, hxc_l_state.hxc_bread, hxc_l_state.hxc_lantern, hxc_l_state.hxc_sand);
        return hxc_l_tmp_call_result_n46;
      }
    case 4:
      {
        struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n57 = hxc_caxecraft_gameplay_Inventory_make(hxc_l_state.hxc_selected, hxc_l_state.hxc_grass, hxc_l_state.hxc_dirt, hxc_l_state.hxc_stone, hxc_l_state.hxc_haxeforge, hxc_i32_subtract_wrapping(hxc_l_state.hxc_sword, 1), hxc_l_state.hxc_berries, hxc_l_state.hxc_bread, hxc_l_state.hxc_lantern, hxc_l_state.hxc_sand);
        return hxc_l_tmp_call_result_n57;
      }
    case 5:
      {
        struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n68 = hxc_caxecraft_gameplay_Inventory_make(hxc_l_state.hxc_selected, hxc_l_state.hxc_grass, hxc_l_state.hxc_dirt, hxc_l_state.hxc_stone, hxc_l_state.hxc_haxeforge, hxc_l_state.hxc_sword, hxc_i32_subtract_wrapping(hxc_l_state.hxc_berries, 1), hxc_l_state.hxc_bread, hxc_l_state.hxc_lantern, hxc_l_state.hxc_sand);
        return hxc_l_tmp_call_result_n68;
      }
    case 6:
      {
        struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n79 = hxc_caxecraft_gameplay_Inventory_make(hxc_l_state.hxc_selected, hxc_l_state.hxc_grass, hxc_l_state.hxc_dirt, hxc_l_state.hxc_stone, hxc_l_state.hxc_haxeforge, hxc_l_state.hxc_sword, hxc_l_state.hxc_berries, hxc_i32_subtract_wrapping(hxc_l_state.hxc_bread, 1), hxc_l_state.hxc_lantern, hxc_l_state.hxc_sand);
        return hxc_l_tmp_call_result_n79;
      }
    case 7:
      {
        struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n90 = hxc_caxecraft_gameplay_Inventory_make(hxc_l_state.hxc_selected, hxc_l_state.hxc_grass, hxc_l_state.hxc_dirt, hxc_l_state.hxc_stone, hxc_l_state.hxc_haxeforge, hxc_l_state.hxc_sword, hxc_l_state.hxc_berries, hxc_l_state.hxc_bread, hxc_i32_subtract_wrapping(hxc_l_state.hxc_lantern, 1), hxc_l_state.hxc_sand);
        return hxc_l_tmp_call_result_n90;
      }
    case 8:
      {
        struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n101 = hxc_caxecraft_gameplay_Inventory_make(hxc_l_state.hxc_selected, hxc_l_state.hxc_grass, hxc_l_state.hxc_dirt, hxc_l_state.hxc_stone, hxc_l_state.hxc_haxeforge, hxc_l_state.hxc_sword, hxc_l_state.hxc_berries, hxc_l_state.hxc_bread, hxc_l_state.hxc_lantern, hxc_i32_subtract_wrapping(hxc_l_state.hxc_sand, 1));
        return hxc_l_tmp_call_result_n101;
      }
    default:
      {
        return hxc_l_state;
      }
  }
  struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n13 = hxc_caxecraft_gameplay_Inventory_make(hxc_l_state.hxc_selected, hxc_i32_subtract_wrapping(hxc_l_state.hxc_grass, 1), hxc_l_state.hxc_dirt, hxc_l_state.hxc_stone, hxc_l_state.hxc_haxeforge, hxc_l_state.hxc_sword, hxc_l_state.hxc_berries, hxc_l_state.hxc_bread, hxc_l_state.hxc_lantern, hxc_l_state.hxc_sand);
  return hxc_l_tmp_call_result_n13;
}

int32_t hxc_caxecraft_gameplay_Inventory_countAt(struct hxc_caxecraft_gameplay_InventoryState hxc_l_state, int32_t hxc_l_slot)
{
  switch (hxc_l_slot) {
    case 0:
      {
        break;
      }
    case 1:
      {
        return hxc_l_state.hxc_dirt;
      }
    case 2:
      {
        return hxc_l_state.hxc_stone;
      }
    case 3:
      {
        return hxc_l_state.hxc_haxeforge;
      }
    case 4:
      {
        return hxc_l_state.hxc_sword;
      }
    case 5:
      {
        return hxc_l_state.hxc_berries;
      }
    case 6:
      {
        return hxc_l_state.hxc_bread;
      }
    case 7:
      {
        return hxc_l_state.hxc_lantern;
      }
    case 8:
      {
        return hxc_l_state.hxc_sand;
      }
    default:
      {
        return 0;
      }
  }
  return hxc_l_state.hxc_grass;
}

int32_t hxc_caxecraft_gameplay_Inventory_countItem(struct hxc_caxecraft_gameplay_InventoryState hxc_l_state, enum hxc_caxecraft_gameplay_ItemKind hxc_l_item)
{
  switch (hxc_l_item) {
    case hxc_caxecraft_gameplay_ItemKind_GrassBlock:
      {
        break;
      }
    case hxc_caxecraft_gameplay_ItemKind_DirtBlock:
      {
        return hxc_l_state.hxc_dirt;
      }
    case hxc_caxecraft_gameplay_ItemKind_StoneBlock:
      {
        return hxc_l_state.hxc_stone;
      }
    case hxc_caxecraft_gameplay_ItemKind_Haxeforge:
      {
        return hxc_l_state.hxc_haxeforge;
      }
    case hxc_caxecraft_gameplay_ItemKind_CopperSword:
      {
        return hxc_l_state.hxc_sword;
      }
    case hxc_caxecraft_gameplay_ItemKind_Berries:
      {
        return hxc_l_state.hxc_berries;
      }
    case hxc_caxecraft_gameplay_ItemKind_Bread:
      {
        return hxc_l_state.hxc_bread;
      }
    case hxc_caxecraft_gameplay_ItemKind_Lantern:
      {
        return hxc_l_state.hxc_lantern;
      }
    case hxc_caxecraft_gameplay_ItemKind_SandBlock:
      {
        return hxc_l_state.hxc_sand;
      }
    default:
      {
        abort();
      }
  }
  return hxc_l_state.hxc_grass;
}

struct hxc_caxecraft_gameplay_InventoryState hxc_caxecraft_gameplay_Inventory_cycle(struct hxc_caxecraft_gameplay_InventoryState hxc_l_state, int32_t hxc_l_delta)
{
  if (hxc_l_delta == 0)
  {
    return hxc_l_state;
  }
  int32_t hxc_l_next = hxc_i32_add_wrapping(hxc_l_state.hxc_selected, hxc_l_delta);
  while (1)
  {
    if (!(hxc_l_next < 0))
    {
      break;
    }
    hxc_l_next = hxc_i32_add_wrapping(hxc_l_next, 9);
  }
  while (1)
  {
    if (!(hxc_l_next >= 9))
    {
      break;
    }
    hxc_l_next = hxc_i32_subtract_wrapping(hxc_l_next, 9);
  }
  struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n6 = hxc_caxecraft_gameplay_Inventory_select(hxc_l_state, hxc_l_next);
  return hxc_l_tmp_call_result_n6;
}

enum hxc_caxecraft_gameplay_ItemKind hxc_caxecraft_gameplay_Inventory_itemAt(int32_t hxc_l_slot)
{
  switch (hxc_l_slot) {
    case 0:
      {
        break;
      }
    case 1:
      {
        return hxc_caxecraft_gameplay_ItemKind_DirtBlock;
      }
    case 2:
      {
        return hxc_caxecraft_gameplay_ItemKind_StoneBlock;
      }
    case 3:
      {
        return hxc_caxecraft_gameplay_ItemKind_Haxeforge;
      }
    case 4:
      {
        return hxc_caxecraft_gameplay_ItemKind_CopperSword;
      }
    case 5:
      {
        return hxc_caxecraft_gameplay_ItemKind_Berries;
      }
    case 6:
      {
        return hxc_caxecraft_gameplay_ItemKind_Bread;
      }
    case 7:
      {
        return hxc_caxecraft_gameplay_ItemKind_Lantern;
      }
    default:
      {
        return hxc_caxecraft_gameplay_ItemKind_SandBlock;
      }
  }
  return hxc_caxecraft_gameplay_ItemKind_GrassBlock;
}

struct hxc_caxecraft_gameplay_InventoryState hxc_caxecraft_gameplay_Inventory_make(int32_t hxc_l_selected, int32_t hxc_l_grass, int32_t hxc_l_dirt, int32_t hxc_l_stone, int32_t hxc_l_haxeforge, int32_t hxc_l_sword, int32_t hxc_l_berries, int32_t hxc_l_bread, int32_t hxc_l_lantern, int32_t hxc_l_sand)
{
  int32_t hxc_l_tmp_record_field_selected_n10 = hxc_l_selected;
  int32_t hxc_l_tmp_conditional_result_n11 = 0;
  if (hxc_l_grass < 0)
  {
    hxc_l_tmp_conditional_result_n11 = 0;
  }
  else
  {
    int32_t hxc_l_tmp_conditional_result_n12 = 0;
    if (hxc_l_grass > 64)
    {
      hxc_l_tmp_conditional_result_n12 = 64;
    }
    else
    {
      hxc_l_tmp_conditional_result_n12 = hxc_l_grass;
    }
    hxc_l_tmp_conditional_result_n11 = hxc_l_tmp_conditional_result_n12;
  }
  int32_t hxc_l_tmp_record_field_grass_n13 = hxc_l_tmp_conditional_result_n11;
  int32_t hxc_l_tmp_conditional_result_n14 = 0;
  if (hxc_l_dirt < 0)
  {
    hxc_l_tmp_conditional_result_n14 = 0;
  }
  else
  {
    int32_t hxc_l_tmp_conditional_result_n15 = 0;
    if (hxc_l_dirt > 64)
    {
      hxc_l_tmp_conditional_result_n15 = 64;
    }
    else
    {
      hxc_l_tmp_conditional_result_n15 = hxc_l_dirt;
    }
    hxc_l_tmp_conditional_result_n14 = hxc_l_tmp_conditional_result_n15;
  }
  int32_t hxc_l_tmp_record_field_dirt_n16 = hxc_l_tmp_conditional_result_n14;
  int32_t hxc_l_tmp_conditional_result_n17 = 0;
  if (hxc_l_stone < 0)
  {
    hxc_l_tmp_conditional_result_n17 = 0;
  }
  else
  {
    int32_t hxc_l_tmp_conditional_result_n18 = 0;
    if (hxc_l_stone > 64)
    {
      hxc_l_tmp_conditional_result_n18 = 64;
    }
    else
    {
      hxc_l_tmp_conditional_result_n18 = hxc_l_stone;
    }
    hxc_l_tmp_conditional_result_n17 = hxc_l_tmp_conditional_result_n18;
  }
  int32_t hxc_l_tmp_record_field_stone_n19 = hxc_l_tmp_conditional_result_n17;
  int32_t hxc_l_tmp_conditional_result_n20 = 0;
  if (hxc_l_haxeforge < 0)
  {
    hxc_l_tmp_conditional_result_n20 = 0;
  }
  else
  {
    int32_t hxc_l_tmp_conditional_result_n21 = 0;
    if (hxc_l_haxeforge > 64)
    {
      hxc_l_tmp_conditional_result_n21 = 64;
    }
    else
    {
      hxc_l_tmp_conditional_result_n21 = hxc_l_haxeforge;
    }
    hxc_l_tmp_conditional_result_n20 = hxc_l_tmp_conditional_result_n21;
  }
  int32_t hxc_l_tmp_record_field_haxeforge_n22 = hxc_l_tmp_conditional_result_n20;
  int32_t hxc_l_tmp_conditional_result_n23 = 0;
  if (hxc_l_sword < 0)
  {
    hxc_l_tmp_conditional_result_n23 = 0;
  }
  else
  {
    int32_t hxc_l_tmp_conditional_result_n24 = 0;
    if (hxc_l_sword > 64)
    {
      hxc_l_tmp_conditional_result_n24 = 64;
    }
    else
    {
      hxc_l_tmp_conditional_result_n24 = hxc_l_sword;
    }
    hxc_l_tmp_conditional_result_n23 = hxc_l_tmp_conditional_result_n24;
  }
  int32_t hxc_l_tmp_record_field_sword_n25 = hxc_l_tmp_conditional_result_n23;
  int32_t hxc_l_tmp_conditional_result_n26 = 0;
  if (hxc_l_berries < 0)
  {
    hxc_l_tmp_conditional_result_n26 = 0;
  }
  else
  {
    int32_t hxc_l_tmp_conditional_result_n27 = 0;
    if (hxc_l_berries > 64)
    {
      hxc_l_tmp_conditional_result_n27 = 64;
    }
    else
    {
      hxc_l_tmp_conditional_result_n27 = hxc_l_berries;
    }
    hxc_l_tmp_conditional_result_n26 = hxc_l_tmp_conditional_result_n27;
  }
  int32_t hxc_l_tmp_record_field_berries_n28 = hxc_l_tmp_conditional_result_n26;
  int32_t hxc_l_tmp_conditional_result_n29 = 0;
  if (hxc_l_bread < 0)
  {
    hxc_l_tmp_conditional_result_n29 = 0;
  }
  else
  {
    int32_t hxc_l_tmp_conditional_result_n30 = 0;
    if (hxc_l_bread > 64)
    {
      hxc_l_tmp_conditional_result_n30 = 64;
    }
    else
    {
      hxc_l_tmp_conditional_result_n30 = hxc_l_bread;
    }
    hxc_l_tmp_conditional_result_n29 = hxc_l_tmp_conditional_result_n30;
  }
  int32_t hxc_l_tmp_record_field_bread_n31 = hxc_l_tmp_conditional_result_n29;
  int32_t hxc_l_tmp_conditional_result_n32 = 0;
  if (hxc_l_lantern < 0)
  {
    hxc_l_tmp_conditional_result_n32 = 0;
  }
  else
  {
    int32_t hxc_l_tmp_conditional_result_n33 = 0;
    if (hxc_l_lantern > 64)
    {
      hxc_l_tmp_conditional_result_n33 = 64;
    }
    else
    {
      hxc_l_tmp_conditional_result_n33 = hxc_l_lantern;
    }
    hxc_l_tmp_conditional_result_n32 = hxc_l_tmp_conditional_result_n33;
  }
  int32_t hxc_l_tmp_record_field_lantern_n34 = hxc_l_tmp_conditional_result_n32;
  int32_t hxc_l_tmp_conditional_result_n35 = 0;
  if (hxc_l_sand < 0)
  {
    hxc_l_tmp_conditional_result_n35 = 0;
  }
  else
  {
    int32_t hxc_l_tmp_conditional_result_n36 = 0;
    if (hxc_l_sand > 64)
    {
      hxc_l_tmp_conditional_result_n36 = 64;
    }
    else
    {
      hxc_l_tmp_conditional_result_n36 = hxc_l_sand;
    }
    hxc_l_tmp_conditional_result_n35 = hxc_l_tmp_conditional_result_n36;
  }
  int32_t hxc_l_tmp_conditional_load_result_n17 = hxc_l_tmp_conditional_result_n35;
  int32_t hxc_l_tmp_record_field_berries_load_result_n18 = hxc_l_tmp_record_field_berries_n28;
  int32_t hxc_l_tmp_record_field_bread_load_result_n19 = hxc_l_tmp_record_field_bread_n31;
  int32_t hxc_l_tmp_record_field_dirt_load_result_n20 = hxc_l_tmp_record_field_dirt_n16;
  int32_t hxc_l_tmp_record_field_grass_load_result_n21 = hxc_l_tmp_record_field_grass_n13;
  int32_t hxc_l_tmp_record_field_haxeforge_load_result_n22 = hxc_l_tmp_record_field_haxeforge_n22;
  int32_t hxc_l_tmp_record_field_lantern_load_result_n23 = hxc_l_tmp_record_field_lantern_n34;
  int32_t hxc_l_tmp_record_field_selected_load_result_n24 = hxc_l_tmp_record_field_selected_n10;
  int32_t hxc_l_tmp_record_field_stone_load_result_n25 = hxc_l_tmp_record_field_stone_n19;
  return (struct hxc_caxecraft_gameplay_InventoryState){ .hxc_berries = hxc_l_tmp_record_field_berries_load_result_n18, .hxc_bread = hxc_l_tmp_record_field_bread_load_result_n19, .hxc_dirt = hxc_l_tmp_record_field_dirt_load_result_n20, .hxc_grass = hxc_l_tmp_record_field_grass_load_result_n21, .hxc_haxeforge = hxc_l_tmp_record_field_haxeforge_load_result_n22, .hxc_lantern = hxc_l_tmp_record_field_lantern_load_result_n23, .hxc_sand = hxc_l_tmp_conditional_load_result_n17, .hxc_selected = hxc_l_tmp_record_field_selected_load_result_n24, .hxc_stone = hxc_l_tmp_record_field_stone_load_result_n25, .hxc_sword = hxc_l_tmp_record_field_sword_n25 };
}

struct hxc_caxecraft_gameplay_InventoryState hxc_caxecraft_gameplay_Inventory_select(struct hxc_caxecraft_gameplay_InventoryState hxc_l_state, int32_t hxc_l_slot)
{
  bool hxc_l_tmp_short_circuit_result_n2 = hxc_l_slot < 0;
  if (!(hxc_l_slot < 0))
  {
    hxc_l_tmp_short_circuit_result_n2 = hxc_l_slot >= 9;
  }
  bool hxc_l_tmp_short_circuit_load_result_n0 = hxc_l_tmp_short_circuit_result_n2;
  bool hxc_l_tmp_short_circuit_result_n3 = hxc_l_tmp_short_circuit_load_result_n0;
  if (!hxc_l_tmp_short_circuit_load_result_n0)
  {
    hxc_l_tmp_short_circuit_result_n3 = hxc_l_slot == hxc_l_state.hxc_selected;
  }
  if (!hxc_l_tmp_short_circuit_result_n3)
  {
    struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n12 = hxc_caxecraft_gameplay_Inventory_make(hxc_l_slot, hxc_l_state.hxc_grass, hxc_l_state.hxc_dirt, hxc_l_state.hxc_stone, hxc_l_state.hxc_haxeforge, hxc_l_state.hxc_sword, hxc_l_state.hxc_berries, hxc_l_state.hxc_bread, hxc_l_state.hxc_lantern, hxc_l_state.hxc_sand);
    return hxc_l_tmp_call_result_n12;
  }
  return hxc_l_state;
}

enum hxc_caxecraft_domain_BlockKind hxc_caxecraft_gameplay_Inventory_selectedBlock(struct hxc_caxecraft_gameplay_InventoryState hxc_l_state)
{
  switch (hxc_l_state.hxc_selected) {
    case 0:
      {
        break;
      }
    case 1:
      {
        return hxc_caxecraft_domain_BlockKind_Dirt;
      }
    case 2:
      {
        return hxc_caxecraft_domain_BlockKind_Stone;
      }
    case 8:
      {
        return hxc_caxecraft_domain_BlockKind_Sand;
      }
    default:
      {
        return hxc_caxecraft_domain_BlockKind_Air;
      }
  }
  return hxc_caxecraft_domain_BlockKind_Grass;
}

bool hxc_caxecraft_gameplay_Inventory_selectedIs(struct hxc_caxecraft_gameplay_InventoryState hxc_l_state, enum hxc_caxecraft_gameplay_ItemKind hxc_l_item)
{
  switch (hxc_l_item) {
    case hxc_caxecraft_gameplay_ItemKind_GrassBlock:
      {
        break;
      }
    case hxc_caxecraft_gameplay_ItemKind_DirtBlock:
      {
        return hxc_l_state.hxc_selected == 1;
      }
    case hxc_caxecraft_gameplay_ItemKind_StoneBlock:
      {
        return hxc_l_state.hxc_selected == 2;
      }
    case hxc_caxecraft_gameplay_ItemKind_Haxeforge:
      {
        return hxc_l_state.hxc_selected == 3;
      }
    case hxc_caxecraft_gameplay_ItemKind_CopperSword:
      {
        return hxc_l_state.hxc_selected == 4;
      }
    case hxc_caxecraft_gameplay_ItemKind_Berries:
      {
        return hxc_l_state.hxc_selected == 5;
      }
    case hxc_caxecraft_gameplay_ItemKind_Bread:
      {
        return hxc_l_state.hxc_selected == 6;
      }
    case hxc_caxecraft_gameplay_ItemKind_Lantern:
      {
        return hxc_l_state.hxc_selected == 7;
      }
    case hxc_caxecraft_gameplay_ItemKind_SandBlock:
      {
        return hxc_l_state.hxc_selected == 8;
      }
    default:
      {
        abort();
      }
  }
  return hxc_l_state.hxc_selected == 0;
}

struct hxc_caxecraft_gameplay_InventoryState hxc_caxecraft_gameplay_Inventory_starter(void)
{
  struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n0 = hxc_caxecraft_gameplay_Inventory_make(0, 16, 24, 12, 1, 1, 6, 3, 1, 0);
  return hxc_l_tmp_call_result_n0;
}
