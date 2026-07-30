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
        struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n19 = hxc_caxecraft_gameplay_Inventory_make(hxc_l_state.hxc_selected, hxc_l_state.hxc_grass, hxc_i32_add_wrapping(hxc_l_state.hxc_dirt, 1), hxc_l_state.hxc_stone, hxc_l_state.hxc_haxeforge, hxc_l_state.hxc_sword, hxc_l_state.hxc_berries, hxc_l_state.hxc_bread, hxc_l_state.hxc_lantern);
        return hxc_l_tmp_call_result_n19;
      }
    case hxc_caxecraft_domain_BlockKind_Stone:
      {
        struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n29 = hxc_caxecraft_gameplay_Inventory_make(hxc_l_state.hxc_selected, hxc_l_state.hxc_grass, hxc_l_state.hxc_dirt, hxc_i32_add_wrapping(hxc_l_state.hxc_stone, 1), hxc_l_state.hxc_haxeforge, hxc_l_state.hxc_sword, hxc_l_state.hxc_berries, hxc_l_state.hxc_bread, hxc_l_state.hxc_lantern);
        return hxc_l_tmp_call_result_n29;
      }
    case hxc_caxecraft_domain_BlockKind_Air:
    case hxc_caxecraft_domain_BlockKind_Bedrock:
    case hxc_caxecraft_domain_BlockKind_Sand:
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
  struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n9 = hxc_caxecraft_gameplay_Inventory_make(hxc_l_state.hxc_selected, hxc_i32_add_wrapping(hxc_l_state.hxc_grass, 1), hxc_l_state.hxc_dirt, hxc_l_state.hxc_stone, hxc_l_state.hxc_haxeforge, hxc_l_state.hxc_sword, hxc_l_state.hxc_berries, hxc_l_state.hxc_bread, hxc_l_state.hxc_lantern);
  return hxc_l_tmp_call_result_n9;
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
        struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n23 = hxc_caxecraft_gameplay_Inventory_make(hxc_l_state.hxc_selected, hxc_l_state.hxc_grass, hxc_i32_add_wrapping(hxc_l_state.hxc_dirt, hxc_l_accepted), hxc_l_state.hxc_stone, hxc_l_state.hxc_haxeforge, hxc_l_state.hxc_sword, hxc_l_state.hxc_berries, hxc_l_state.hxc_bread, hxc_l_state.hxc_lantern);
        return hxc_l_tmp_call_result_n23;
      }
    case hxc_caxecraft_gameplay_ItemKind_StoneBlock:
      {
        struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n34 = hxc_caxecraft_gameplay_Inventory_make(hxc_l_state.hxc_selected, hxc_l_state.hxc_grass, hxc_l_state.hxc_dirt, hxc_i32_add_wrapping(hxc_l_state.hxc_stone, hxc_l_accepted), hxc_l_state.hxc_haxeforge, hxc_l_state.hxc_sword, hxc_l_state.hxc_berries, hxc_l_state.hxc_bread, hxc_l_state.hxc_lantern);
        return hxc_l_tmp_call_result_n34;
      }
    case hxc_caxecraft_gameplay_ItemKind_Haxeforge:
      {
        struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n45 = hxc_caxecraft_gameplay_Inventory_make(hxc_l_state.hxc_selected, hxc_l_state.hxc_grass, hxc_l_state.hxc_dirt, hxc_l_state.hxc_stone, hxc_i32_add_wrapping(hxc_l_state.hxc_haxeforge, hxc_l_accepted), hxc_l_state.hxc_sword, hxc_l_state.hxc_berries, hxc_l_state.hxc_bread, hxc_l_state.hxc_lantern);
        return hxc_l_tmp_call_result_n45;
      }
    case hxc_caxecraft_gameplay_ItemKind_CopperSword:
      {
        struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n56 = hxc_caxecraft_gameplay_Inventory_make(hxc_l_state.hxc_selected, hxc_l_state.hxc_grass, hxc_l_state.hxc_dirt, hxc_l_state.hxc_stone, hxc_l_state.hxc_haxeforge, hxc_i32_add_wrapping(hxc_l_state.hxc_sword, hxc_l_accepted), hxc_l_state.hxc_berries, hxc_l_state.hxc_bread, hxc_l_state.hxc_lantern);
        return hxc_l_tmp_call_result_n56;
      }
    case hxc_caxecraft_gameplay_ItemKind_Berries:
      {
        struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n67 = hxc_caxecraft_gameplay_Inventory_make(hxc_l_state.hxc_selected, hxc_l_state.hxc_grass, hxc_l_state.hxc_dirt, hxc_l_state.hxc_stone, hxc_l_state.hxc_haxeforge, hxc_l_state.hxc_sword, hxc_i32_add_wrapping(hxc_l_state.hxc_berries, hxc_l_accepted), hxc_l_state.hxc_bread, hxc_l_state.hxc_lantern);
        return hxc_l_tmp_call_result_n67;
      }
    case hxc_caxecraft_gameplay_ItemKind_Bread:
      {
        struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n78 = hxc_caxecraft_gameplay_Inventory_make(hxc_l_state.hxc_selected, hxc_l_state.hxc_grass, hxc_l_state.hxc_dirt, hxc_l_state.hxc_stone, hxc_l_state.hxc_haxeforge, hxc_l_state.hxc_sword, hxc_l_state.hxc_berries, hxc_i32_add_wrapping(hxc_l_state.hxc_bread, hxc_l_accepted), hxc_l_state.hxc_lantern);
        return hxc_l_tmp_call_result_n78;
      }
    case hxc_caxecraft_gameplay_ItemKind_Lantern:
      {
        struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n89 = hxc_caxecraft_gameplay_Inventory_make(hxc_l_state.hxc_selected, hxc_l_state.hxc_grass, hxc_l_state.hxc_dirt, hxc_l_state.hxc_stone, hxc_l_state.hxc_haxeforge, hxc_l_state.hxc_sword, hxc_l_state.hxc_berries, hxc_l_state.hxc_bread, hxc_i32_add_wrapping(hxc_l_state.hxc_lantern, hxc_l_accepted));
        return hxc_l_tmp_call_result_n89;
      }
    default:
      {
        abort();
      }
  }
  struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n12 = hxc_caxecraft_gameplay_Inventory_make(hxc_l_state.hxc_selected, hxc_i32_add_wrapping(hxc_l_state.hxc_grass, hxc_l_accepted), hxc_l_state.hxc_dirt, hxc_l_state.hxc_stone, hxc_l_state.hxc_haxeforge, hxc_l_state.hxc_sword, hxc_l_state.hxc_berries, hxc_l_state.hxc_bread, hxc_l_state.hxc_lantern);
  return hxc_l_tmp_call_result_n12;
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
        struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n22 = hxc_caxecraft_gameplay_Inventory_make(hxc_l_state.hxc_selected, hxc_l_state.hxc_grass, hxc_i32_subtract_wrapping(hxc_l_state.hxc_dirt, 1), hxc_l_state.hxc_stone, hxc_l_state.hxc_haxeforge, hxc_l_state.hxc_sword, hxc_l_state.hxc_berries, hxc_l_state.hxc_bread, hxc_l_state.hxc_lantern);
        return hxc_l_tmp_call_result_n22;
      }
    case 2:
      {
        struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n32 = hxc_caxecraft_gameplay_Inventory_make(hxc_l_state.hxc_selected, hxc_l_state.hxc_grass, hxc_l_state.hxc_dirt, hxc_i32_subtract_wrapping(hxc_l_state.hxc_stone, 1), hxc_l_state.hxc_haxeforge, hxc_l_state.hxc_sword, hxc_l_state.hxc_berries, hxc_l_state.hxc_bread, hxc_l_state.hxc_lantern);
        return hxc_l_tmp_call_result_n32;
      }
    case 3:
      {
        struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n42 = hxc_caxecraft_gameplay_Inventory_make(hxc_l_state.hxc_selected, hxc_l_state.hxc_grass, hxc_l_state.hxc_dirt, hxc_l_state.hxc_stone, hxc_i32_subtract_wrapping(hxc_l_state.hxc_haxeforge, 1), hxc_l_state.hxc_sword, hxc_l_state.hxc_berries, hxc_l_state.hxc_bread, hxc_l_state.hxc_lantern);
        return hxc_l_tmp_call_result_n42;
      }
    case 4:
      {
        struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n52 = hxc_caxecraft_gameplay_Inventory_make(hxc_l_state.hxc_selected, hxc_l_state.hxc_grass, hxc_l_state.hxc_dirt, hxc_l_state.hxc_stone, hxc_l_state.hxc_haxeforge, hxc_i32_subtract_wrapping(hxc_l_state.hxc_sword, 1), hxc_l_state.hxc_berries, hxc_l_state.hxc_bread, hxc_l_state.hxc_lantern);
        return hxc_l_tmp_call_result_n52;
      }
    case 5:
      {
        struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n62 = hxc_caxecraft_gameplay_Inventory_make(hxc_l_state.hxc_selected, hxc_l_state.hxc_grass, hxc_l_state.hxc_dirt, hxc_l_state.hxc_stone, hxc_l_state.hxc_haxeforge, hxc_l_state.hxc_sword, hxc_i32_subtract_wrapping(hxc_l_state.hxc_berries, 1), hxc_l_state.hxc_bread, hxc_l_state.hxc_lantern);
        return hxc_l_tmp_call_result_n62;
      }
    case 6:
      {
        struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n72 = hxc_caxecraft_gameplay_Inventory_make(hxc_l_state.hxc_selected, hxc_l_state.hxc_grass, hxc_l_state.hxc_dirt, hxc_l_state.hxc_stone, hxc_l_state.hxc_haxeforge, hxc_l_state.hxc_sword, hxc_l_state.hxc_berries, hxc_i32_subtract_wrapping(hxc_l_state.hxc_bread, 1), hxc_l_state.hxc_lantern);
        return hxc_l_tmp_call_result_n72;
      }
    case 7:
      {
        struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n82 = hxc_caxecraft_gameplay_Inventory_make(hxc_l_state.hxc_selected, hxc_l_state.hxc_grass, hxc_l_state.hxc_dirt, hxc_l_state.hxc_stone, hxc_l_state.hxc_haxeforge, hxc_l_state.hxc_sword, hxc_l_state.hxc_berries, hxc_l_state.hxc_bread, hxc_i32_subtract_wrapping(hxc_l_state.hxc_lantern, 1));
        return hxc_l_tmp_call_result_n82;
      }
    default:
      {
        return hxc_l_state;
      }
  }
  struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n12 = hxc_caxecraft_gameplay_Inventory_make(hxc_l_state.hxc_selected, hxc_i32_subtract_wrapping(hxc_l_state.hxc_grass, 1), hxc_l_state.hxc_dirt, hxc_l_state.hxc_stone, hxc_l_state.hxc_haxeforge, hxc_l_state.hxc_sword, hxc_l_state.hxc_berries, hxc_l_state.hxc_bread, hxc_l_state.hxc_lantern);
  return hxc_l_tmp_call_result_n12;
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
    hxc_l_next = hxc_i32_add_wrapping(hxc_l_next, 8);
  }
  while (1)
  {
    if (!(hxc_l_next >= 8))
    {
      break;
    }
    hxc_l_next = hxc_i32_subtract_wrapping(hxc_l_next, 8);
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
    default:
      {
        return hxc_caxecraft_gameplay_ItemKind_Lantern;
      }
  }
  return hxc_caxecraft_gameplay_ItemKind_GrassBlock;
}

struct hxc_caxecraft_gameplay_InventoryState hxc_caxecraft_gameplay_Inventory_make(int32_t hxc_l_selected, int32_t hxc_l_grass, int32_t hxc_l_dirt, int32_t hxc_l_stone, int32_t hxc_l_haxeforge, int32_t hxc_l_sword, int32_t hxc_l_berries, int32_t hxc_l_bread, int32_t hxc_l_lantern)
{
  int32_t hxc_l_tmp_record_field_selected_n9 = hxc_l_selected;
  int32_t hxc_l_tmp_conditional_result_n10 = 0;
  if (hxc_l_grass < 0)
  {
    hxc_l_tmp_conditional_result_n10 = 0;
  }
  else
  {
    int32_t hxc_l_tmp_conditional_result_n11 = 0;
    if (hxc_l_grass > 64)
    {
      hxc_l_tmp_conditional_result_n11 = 64;
    }
    else
    {
      hxc_l_tmp_conditional_result_n11 = hxc_l_grass;
    }
    hxc_l_tmp_conditional_result_n10 = hxc_l_tmp_conditional_result_n11;
  }
  int32_t hxc_l_tmp_record_field_grass_n12 = hxc_l_tmp_conditional_result_n10;
  int32_t hxc_l_tmp_conditional_result_n13 = 0;
  if (hxc_l_dirt < 0)
  {
    hxc_l_tmp_conditional_result_n13 = 0;
  }
  else
  {
    int32_t hxc_l_tmp_conditional_result_n14 = 0;
    if (hxc_l_dirt > 64)
    {
      hxc_l_tmp_conditional_result_n14 = 64;
    }
    else
    {
      hxc_l_tmp_conditional_result_n14 = hxc_l_dirt;
    }
    hxc_l_tmp_conditional_result_n13 = hxc_l_tmp_conditional_result_n14;
  }
  int32_t hxc_l_tmp_record_field_dirt_n15 = hxc_l_tmp_conditional_result_n13;
  int32_t hxc_l_tmp_conditional_result_n16 = 0;
  if (hxc_l_stone < 0)
  {
    hxc_l_tmp_conditional_result_n16 = 0;
  }
  else
  {
    int32_t hxc_l_tmp_conditional_result_n17 = 0;
    if (hxc_l_stone > 64)
    {
      hxc_l_tmp_conditional_result_n17 = 64;
    }
    else
    {
      hxc_l_tmp_conditional_result_n17 = hxc_l_stone;
    }
    hxc_l_tmp_conditional_result_n16 = hxc_l_tmp_conditional_result_n17;
  }
  int32_t hxc_l_tmp_record_field_stone_n18 = hxc_l_tmp_conditional_result_n16;
  int32_t hxc_l_tmp_conditional_result_n19 = 0;
  if (hxc_l_haxeforge < 0)
  {
    hxc_l_tmp_conditional_result_n19 = 0;
  }
  else
  {
    int32_t hxc_l_tmp_conditional_result_n20 = 0;
    if (hxc_l_haxeforge > 64)
    {
      hxc_l_tmp_conditional_result_n20 = 64;
    }
    else
    {
      hxc_l_tmp_conditional_result_n20 = hxc_l_haxeforge;
    }
    hxc_l_tmp_conditional_result_n19 = hxc_l_tmp_conditional_result_n20;
  }
  int32_t hxc_l_tmp_record_field_haxeforge_n21 = hxc_l_tmp_conditional_result_n19;
  int32_t hxc_l_tmp_conditional_result_n22 = 0;
  if (hxc_l_sword < 0)
  {
    hxc_l_tmp_conditional_result_n22 = 0;
  }
  else
  {
    int32_t hxc_l_tmp_conditional_result_n23 = 0;
    if (hxc_l_sword > 64)
    {
      hxc_l_tmp_conditional_result_n23 = 64;
    }
    else
    {
      hxc_l_tmp_conditional_result_n23 = hxc_l_sword;
    }
    hxc_l_tmp_conditional_result_n22 = hxc_l_tmp_conditional_result_n23;
  }
  int32_t hxc_l_tmp_record_field_sword_n24 = hxc_l_tmp_conditional_result_n22;
  int32_t hxc_l_tmp_conditional_result_n25 = 0;
  if (hxc_l_berries < 0)
  {
    hxc_l_tmp_conditional_result_n25 = 0;
  }
  else
  {
    int32_t hxc_l_tmp_conditional_result_n26 = 0;
    if (hxc_l_berries > 64)
    {
      hxc_l_tmp_conditional_result_n26 = 64;
    }
    else
    {
      hxc_l_tmp_conditional_result_n26 = hxc_l_berries;
    }
    hxc_l_tmp_conditional_result_n25 = hxc_l_tmp_conditional_result_n26;
  }
  int32_t hxc_l_tmp_record_field_berries_n27 = hxc_l_tmp_conditional_result_n25;
  int32_t hxc_l_tmp_conditional_result_n28 = 0;
  if (hxc_l_bread < 0)
  {
    hxc_l_tmp_conditional_result_n28 = 0;
  }
  else
  {
    int32_t hxc_l_tmp_conditional_result_n29 = 0;
    if (hxc_l_bread > 64)
    {
      hxc_l_tmp_conditional_result_n29 = 64;
    }
    else
    {
      hxc_l_tmp_conditional_result_n29 = hxc_l_bread;
    }
    hxc_l_tmp_conditional_result_n28 = hxc_l_tmp_conditional_result_n29;
  }
  int32_t hxc_l_tmp_record_field_bread_n30 = hxc_l_tmp_conditional_result_n28;
  int32_t hxc_l_tmp_conditional_result_n31 = 0;
  if (hxc_l_lantern < 0)
  {
    hxc_l_tmp_conditional_result_n31 = 0;
  }
  else
  {
    int32_t hxc_l_tmp_conditional_result_n32 = 0;
    if (hxc_l_lantern > 64)
    {
      hxc_l_tmp_conditional_result_n32 = 64;
    }
    else
    {
      hxc_l_tmp_conditional_result_n32 = hxc_l_lantern;
    }
    hxc_l_tmp_conditional_result_n31 = hxc_l_tmp_conditional_result_n32;
  }
  int32_t hxc_l_tmp_conditional_load_result_n15 = hxc_l_tmp_conditional_result_n31;
  int32_t hxc_l_tmp_record_field_berries_load_result_n16 = hxc_l_tmp_record_field_berries_n27;
  int32_t hxc_l_tmp_record_field_bread_load_result_n17 = hxc_l_tmp_record_field_bread_n30;
  int32_t hxc_l_tmp_record_field_dirt_load_result_n18 = hxc_l_tmp_record_field_dirt_n15;
  int32_t hxc_l_tmp_record_field_grass_load_result_n19 = hxc_l_tmp_record_field_grass_n12;
  int32_t hxc_l_tmp_record_field_haxeforge_load_result_n20 = hxc_l_tmp_record_field_haxeforge_n21;
  int32_t hxc_l_tmp_record_field_selected_load_result_n21 = hxc_l_tmp_record_field_selected_n9;
  int32_t hxc_l_tmp_record_field_stone_load_result_n22 = hxc_l_tmp_record_field_stone_n18;
  return (struct hxc_caxecraft_gameplay_InventoryState){ .hxc_berries = hxc_l_tmp_record_field_berries_load_result_n16, .hxc_bread = hxc_l_tmp_record_field_bread_load_result_n17, .hxc_dirt = hxc_l_tmp_record_field_dirt_load_result_n18, .hxc_grass = hxc_l_tmp_record_field_grass_load_result_n19, .hxc_haxeforge = hxc_l_tmp_record_field_haxeforge_load_result_n20, .hxc_lantern = hxc_l_tmp_conditional_load_result_n15, .hxc_selected = hxc_l_tmp_record_field_selected_load_result_n21, .hxc_stone = hxc_l_tmp_record_field_stone_load_result_n22, .hxc_sword = hxc_l_tmp_record_field_sword_n24 };
}

struct hxc_caxecraft_gameplay_InventoryState hxc_caxecraft_gameplay_Inventory_select(struct hxc_caxecraft_gameplay_InventoryState hxc_l_state, int32_t hxc_l_slot)
{
  bool hxc_l_tmp_short_circuit_result_n2 = hxc_l_slot < 0;
  if (!(hxc_l_slot < 0))
  {
    hxc_l_tmp_short_circuit_result_n2 = hxc_l_slot >= 8;
  }
  bool hxc_l_tmp_short_circuit_load_result_n0 = hxc_l_tmp_short_circuit_result_n2;
  bool hxc_l_tmp_short_circuit_result_n3 = hxc_l_tmp_short_circuit_load_result_n0;
  if (!hxc_l_tmp_short_circuit_load_result_n0)
  {
    hxc_l_tmp_short_circuit_result_n3 = hxc_l_slot == hxc_l_state.hxc_selected;
  }
  if (!hxc_l_tmp_short_circuit_result_n3)
  {
    struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n11 = hxc_caxecraft_gameplay_Inventory_make(hxc_l_slot, hxc_l_state.hxc_grass, hxc_l_state.hxc_dirt, hxc_l_state.hxc_stone, hxc_l_state.hxc_haxeforge, hxc_l_state.hxc_sword, hxc_l_state.hxc_berries, hxc_l_state.hxc_bread, hxc_l_state.hxc_lantern);
    return hxc_l_tmp_call_result_n11;
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
    default:
      {
        abort();
      }
  }
  return hxc_l_state.hxc_selected == 0;
}

struct hxc_caxecraft_gameplay_InventoryState hxc_caxecraft_gameplay_Inventory_starter(void)
{
  struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n0 = hxc_caxecraft_gameplay_Inventory_make(0, 16, 24, 12, 1, 1, 6, 3, 1);
  return hxc_l_tmp_call_result_n0;
}
