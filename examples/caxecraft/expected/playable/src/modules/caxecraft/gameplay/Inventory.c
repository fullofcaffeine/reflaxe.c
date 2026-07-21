#include "hxc/program.h"

struct hxc_caxecraft_gameplay_InventoryState hxc_caxecraft_gameplay_Inventory_collectBlock(struct hxc_caxecraft_gameplay_InventoryState hxc_state, enum hxc_caxecraft_domain_BlockKind hxc_kind)
{
  switch (hxc_kind) {
    case hxc_caxecraft_domain_BlockKind_Grass:
      {
        break;
      }
    case hxc_caxecraft_domain_BlockKind_Dirt:
      {
        struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_call_result_n19 = hxc_caxecraft_gameplay_Inventory_make(hxc_state.hxc_selected, hxc_state.hxc_grass, hxc_i32_add_wrapping(hxc_state.hxc_dirt, 1), hxc_state.hxc_stone, hxc_state.hxc_haxeforge, hxc_state.hxc_sword, hxc_state.hxc_berries, hxc_state.hxc_bread, hxc_state.hxc_lantern);
        return hxc_tmp_call_result_n19;
      }
    case hxc_caxecraft_domain_BlockKind_Stone:
      {
        struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_call_result_n29 = hxc_caxecraft_gameplay_Inventory_make(hxc_state.hxc_selected, hxc_state.hxc_grass, hxc_state.hxc_dirt, hxc_i32_add_wrapping(hxc_state.hxc_stone, 1), hxc_state.hxc_haxeforge, hxc_state.hxc_sword, hxc_state.hxc_berries, hxc_state.hxc_bread, hxc_state.hxc_lantern);
        return hxc_tmp_call_result_n29;
      }
    case hxc_caxecraft_domain_BlockKind_Air:
    case hxc_caxecraft_domain_BlockKind_Bedrock:
      {
        return hxc_state;
      }
  }
  struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_call_result_n9 = hxc_caxecraft_gameplay_Inventory_make(hxc_state.hxc_selected, hxc_i32_add_wrapping(hxc_state.hxc_grass, 1), hxc_state.hxc_dirt, hxc_state.hxc_stone, hxc_state.hxc_haxeforge, hxc_state.hxc_sword, hxc_state.hxc_berries, hxc_state.hxc_bread, hxc_state.hxc_lantern);
  return hxc_tmp_call_result_n9;
}

struct hxc_caxecraft_gameplay_InventoryState hxc_caxecraft_gameplay_Inventory_collectItem(struct hxc_caxecraft_gameplay_InventoryState hxc_state, enum hxc_caxecraft_gameplay_ItemKind hxc_kind, int32_t hxc_amount)
{
  if (!(hxc_amount <= 0))
  {
    switch (hxc_kind) {
      case hxc_caxecraft_gameplay_ItemKind_GrassBlock:
        {
          break;
        }
      case hxc_caxecraft_gameplay_ItemKind_DirtBlock:
        {
          struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_call_result_n19 = hxc_caxecraft_gameplay_Inventory_make(hxc_state.hxc_selected, hxc_state.hxc_grass, hxc_i32_add_wrapping(hxc_state.hxc_dirt, hxc_amount), hxc_state.hxc_stone, hxc_state.hxc_haxeforge, hxc_state.hxc_sword, hxc_state.hxc_berries, hxc_state.hxc_bread, hxc_state.hxc_lantern);
          return hxc_tmp_call_result_n19;
        }
      case hxc_caxecraft_gameplay_ItemKind_StoneBlock:
        {
          struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_call_result_n29 = hxc_caxecraft_gameplay_Inventory_make(hxc_state.hxc_selected, hxc_state.hxc_grass, hxc_state.hxc_dirt, hxc_i32_add_wrapping(hxc_state.hxc_stone, hxc_amount), hxc_state.hxc_haxeforge, hxc_state.hxc_sword, hxc_state.hxc_berries, hxc_state.hxc_bread, hxc_state.hxc_lantern);
          return hxc_tmp_call_result_n29;
        }
      case hxc_caxecraft_gameplay_ItemKind_Haxeforge:
        {
          struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_call_result_n39 = hxc_caxecraft_gameplay_Inventory_make(hxc_state.hxc_selected, hxc_state.hxc_grass, hxc_state.hxc_dirt, hxc_state.hxc_stone, hxc_i32_add_wrapping(hxc_state.hxc_haxeforge, hxc_amount), hxc_state.hxc_sword, hxc_state.hxc_berries, hxc_state.hxc_bread, hxc_state.hxc_lantern);
          return hxc_tmp_call_result_n39;
        }
      case hxc_caxecraft_gameplay_ItemKind_CopperSword:
        {
          struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_call_result_n49 = hxc_caxecraft_gameplay_Inventory_make(hxc_state.hxc_selected, hxc_state.hxc_grass, hxc_state.hxc_dirt, hxc_state.hxc_stone, hxc_state.hxc_haxeforge, hxc_i32_add_wrapping(hxc_state.hxc_sword, hxc_amount), hxc_state.hxc_berries, hxc_state.hxc_bread, hxc_state.hxc_lantern);
          return hxc_tmp_call_result_n49;
        }
      case hxc_caxecraft_gameplay_ItemKind_Berries:
        {
          struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_call_result_n59 = hxc_caxecraft_gameplay_Inventory_make(hxc_state.hxc_selected, hxc_state.hxc_grass, hxc_state.hxc_dirt, hxc_state.hxc_stone, hxc_state.hxc_haxeforge, hxc_state.hxc_sword, hxc_i32_add_wrapping(hxc_state.hxc_berries, hxc_amount), hxc_state.hxc_bread, hxc_state.hxc_lantern);
          return hxc_tmp_call_result_n59;
        }
      case hxc_caxecraft_gameplay_ItemKind_Bread:
        {
          struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_call_result_n69 = hxc_caxecraft_gameplay_Inventory_make(hxc_state.hxc_selected, hxc_state.hxc_grass, hxc_state.hxc_dirt, hxc_state.hxc_stone, hxc_state.hxc_haxeforge, hxc_state.hxc_sword, hxc_state.hxc_berries, hxc_i32_add_wrapping(hxc_state.hxc_bread, hxc_amount), hxc_state.hxc_lantern);
          return hxc_tmp_call_result_n69;
        }
      case hxc_caxecraft_gameplay_ItemKind_Lantern:
        {
          struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_call_result_n79 = hxc_caxecraft_gameplay_Inventory_make(hxc_state.hxc_selected, hxc_state.hxc_grass, hxc_state.hxc_dirt, hxc_state.hxc_stone, hxc_state.hxc_haxeforge, hxc_state.hxc_sword, hxc_state.hxc_berries, hxc_state.hxc_bread, hxc_i32_add_wrapping(hxc_state.hxc_lantern, hxc_amount));
          return hxc_tmp_call_result_n79;
        }
    }
    struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_call_result_n9 = hxc_caxecraft_gameplay_Inventory_make(hxc_state.hxc_selected, hxc_i32_add_wrapping(hxc_state.hxc_grass, hxc_amount), hxc_state.hxc_dirt, hxc_state.hxc_stone, hxc_state.hxc_haxeforge, hxc_state.hxc_sword, hxc_state.hxc_berries, hxc_state.hxc_bread, hxc_state.hxc_lantern);
    return hxc_tmp_call_result_n9;
  }
  return hxc_state;
}

struct hxc_caxecraft_gameplay_InventoryState hxc_caxecraft_gameplay_Inventory_consumeSelected(struct hxc_caxecraft_gameplay_InventoryState hxc_state)
{
  int32_t hxc_tmp_call_result_n1 = hxc_caxecraft_gameplay_Inventory_countAt(hxc_state, hxc_state.hxc_selected);
  if (!(hxc_tmp_call_result_n1 <= 0))
  {
    switch (hxc_state.hxc_selected) {
      case 0:
        {
          break;
        }
      case 1:
        {
          struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_call_result_n22 = hxc_caxecraft_gameplay_Inventory_make(hxc_state.hxc_selected, hxc_state.hxc_grass, hxc_i32_subtract_wrapping(hxc_state.hxc_dirt, 1), hxc_state.hxc_stone, hxc_state.hxc_haxeforge, hxc_state.hxc_sword, hxc_state.hxc_berries, hxc_state.hxc_bread, hxc_state.hxc_lantern);
          return hxc_tmp_call_result_n22;
        }
      case 2:
        {
          struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_call_result_n32 = hxc_caxecraft_gameplay_Inventory_make(hxc_state.hxc_selected, hxc_state.hxc_grass, hxc_state.hxc_dirt, hxc_i32_subtract_wrapping(hxc_state.hxc_stone, 1), hxc_state.hxc_haxeforge, hxc_state.hxc_sword, hxc_state.hxc_berries, hxc_state.hxc_bread, hxc_state.hxc_lantern);
          return hxc_tmp_call_result_n32;
        }
      case 3:
        {
          struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_call_result_n42 = hxc_caxecraft_gameplay_Inventory_make(hxc_state.hxc_selected, hxc_state.hxc_grass, hxc_state.hxc_dirt, hxc_state.hxc_stone, hxc_i32_subtract_wrapping(hxc_state.hxc_haxeforge, 1), hxc_state.hxc_sword, hxc_state.hxc_berries, hxc_state.hxc_bread, hxc_state.hxc_lantern);
          return hxc_tmp_call_result_n42;
        }
      case 4:
        {
          struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_call_result_n52 = hxc_caxecraft_gameplay_Inventory_make(hxc_state.hxc_selected, hxc_state.hxc_grass, hxc_state.hxc_dirt, hxc_state.hxc_stone, hxc_state.hxc_haxeforge, hxc_i32_subtract_wrapping(hxc_state.hxc_sword, 1), hxc_state.hxc_berries, hxc_state.hxc_bread, hxc_state.hxc_lantern);
          return hxc_tmp_call_result_n52;
        }
      case 5:
        {
          struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_call_result_n62 = hxc_caxecraft_gameplay_Inventory_make(hxc_state.hxc_selected, hxc_state.hxc_grass, hxc_state.hxc_dirt, hxc_state.hxc_stone, hxc_state.hxc_haxeforge, hxc_state.hxc_sword, hxc_i32_subtract_wrapping(hxc_state.hxc_berries, 1), hxc_state.hxc_bread, hxc_state.hxc_lantern);
          return hxc_tmp_call_result_n62;
        }
      case 6:
        {
          struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_call_result_n72 = hxc_caxecraft_gameplay_Inventory_make(hxc_state.hxc_selected, hxc_state.hxc_grass, hxc_state.hxc_dirt, hxc_state.hxc_stone, hxc_state.hxc_haxeforge, hxc_state.hxc_sword, hxc_state.hxc_berries, hxc_i32_subtract_wrapping(hxc_state.hxc_bread, 1), hxc_state.hxc_lantern);
          return hxc_tmp_call_result_n72;
        }
      case 7:
        {
          struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_call_result_n82 = hxc_caxecraft_gameplay_Inventory_make(hxc_state.hxc_selected, hxc_state.hxc_grass, hxc_state.hxc_dirt, hxc_state.hxc_stone, hxc_state.hxc_haxeforge, hxc_state.hxc_sword, hxc_state.hxc_berries, hxc_state.hxc_bread, hxc_i32_subtract_wrapping(hxc_state.hxc_lantern, 1));
          return hxc_tmp_call_result_n82;
        }
      default:
        {
          return hxc_state;
        }
    }
    struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_call_result_n12 = hxc_caxecraft_gameplay_Inventory_make(hxc_state.hxc_selected, hxc_i32_subtract_wrapping(hxc_state.hxc_grass, 1), hxc_state.hxc_dirt, hxc_state.hxc_stone, hxc_state.hxc_haxeforge, hxc_state.hxc_sword, hxc_state.hxc_berries, hxc_state.hxc_bread, hxc_state.hxc_lantern);
    return hxc_tmp_call_result_n12;
  }
  return hxc_state;
}

int32_t hxc_caxecraft_gameplay_Inventory_countAt(struct hxc_caxecraft_gameplay_InventoryState hxc_state, int32_t hxc_slot)
{
  switch (hxc_slot) {
    case 0:
      {
        break;
      }
    case 1:
      {
        return hxc_state.hxc_dirt;
      }
    case 2:
      {
        return hxc_state.hxc_stone;
      }
    case 3:
      {
        return hxc_state.hxc_haxeforge;
      }
    case 4:
      {
        return hxc_state.hxc_sword;
      }
    case 5:
      {
        return hxc_state.hxc_berries;
      }
    case 6:
      {
        return hxc_state.hxc_bread;
      }
    case 7:
      {
        return hxc_state.hxc_lantern;
      }
    default:
      {
        return 0;
      }
  }
  return hxc_state.hxc_grass;
}

struct hxc_caxecraft_gameplay_InventoryState hxc_caxecraft_gameplay_Inventory_cycle(struct hxc_caxecraft_gameplay_InventoryState hxc_state, int32_t hxc_delta)
{
  if (hxc_delta == 0)
  {
    return hxc_state;
  }
  int32_t hxc_next = hxc_i32_add_wrapping(hxc_state.hxc_selected, hxc_delta);
  while (1)
  {
    if (!(hxc_next < 0))
    {
      break;
    }
    hxc_next = hxc_i32_add_wrapping(hxc_next, 8);
  }
  while (1)
  {
    if (!(hxc_next >= 8))
    {
      break;
    }
    hxc_next = hxc_i32_subtract_wrapping(hxc_next, 8);
  }
  struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_call_result_n6 = hxc_caxecraft_gameplay_Inventory_select(hxc_state, hxc_next);
  return hxc_tmp_call_result_n6;
}

enum hxc_caxecraft_gameplay_ItemKind hxc_caxecraft_gameplay_Inventory_itemAt(int32_t hxc_slot)
{
  switch (hxc_slot) {
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

struct hxc_caxecraft_gameplay_InventoryState hxc_caxecraft_gameplay_Inventory_make(int32_t hxc_selected, int32_t hxc_grass, int32_t hxc_dirt, int32_t hxc_stone, int32_t hxc_haxeforge, int32_t hxc_sword, int32_t hxc_berries, int32_t hxc_bread, int32_t hxc_lantern)
{
  int32_t hxc_tmp_conditional_result_n10 = 0;
  if (hxc_grass < 0)
  {
    hxc_tmp_conditional_result_n10 = 0;
  }
  else
  {
    int32_t hxc_tmp_conditional_result_n11 = 0;
    if (hxc_grass > 64)
    {
      hxc_tmp_conditional_result_n11 = 64;
    }
    else
    {
      hxc_tmp_conditional_result_n11 = hxc_grass;
    }
    hxc_tmp_conditional_result_n10 = hxc_tmp_conditional_result_n11;
  }
  int32_t hxc_safeGrass = hxc_tmp_conditional_result_n10;
  int32_t hxc_tmp_conditional_result_n13 = 0;
  if (hxc_dirt < 0)
  {
    hxc_tmp_conditional_result_n13 = 0;
  }
  else
  {
    int32_t hxc_tmp_conditional_result_n14 = 0;
    if (hxc_dirt > 64)
    {
      hxc_tmp_conditional_result_n14 = 64;
    }
    else
    {
      hxc_tmp_conditional_result_n14 = hxc_dirt;
    }
    hxc_tmp_conditional_result_n13 = hxc_tmp_conditional_result_n14;
  }
  int32_t hxc_safeDirt = hxc_tmp_conditional_result_n13;
  int32_t hxc_tmp_conditional_result_n16 = 0;
  if (hxc_stone < 0)
  {
    hxc_tmp_conditional_result_n16 = 0;
  }
  else
  {
    int32_t hxc_tmp_conditional_result_n17 = 0;
    if (hxc_stone > 64)
    {
      hxc_tmp_conditional_result_n17 = 64;
    }
    else
    {
      hxc_tmp_conditional_result_n17 = hxc_stone;
    }
    hxc_tmp_conditional_result_n16 = hxc_tmp_conditional_result_n17;
  }
  int32_t hxc_safeStone = hxc_tmp_conditional_result_n16;
  int32_t hxc_tmp_conditional_result_n19 = 0;
  if (hxc_haxeforge < 0)
  {
    hxc_tmp_conditional_result_n19 = 0;
  }
  else
  {
    int32_t hxc_tmp_conditional_result_n20 = 0;
    if (hxc_haxeforge > 64)
    {
      hxc_tmp_conditional_result_n20 = 64;
    }
    else
    {
      hxc_tmp_conditional_result_n20 = hxc_haxeforge;
    }
    hxc_tmp_conditional_result_n19 = hxc_tmp_conditional_result_n20;
  }
  int32_t hxc_safeHaxeforge = hxc_tmp_conditional_result_n19;
  int32_t hxc_tmp_conditional_result_n22 = 0;
  if (hxc_sword < 0)
  {
    hxc_tmp_conditional_result_n22 = 0;
  }
  else
  {
    int32_t hxc_tmp_conditional_result_n23 = 0;
    if (hxc_sword > 64)
    {
      hxc_tmp_conditional_result_n23 = 64;
    }
    else
    {
      hxc_tmp_conditional_result_n23 = hxc_sword;
    }
    hxc_tmp_conditional_result_n22 = hxc_tmp_conditional_result_n23;
  }
  int32_t hxc_safeSword = hxc_tmp_conditional_result_n22;
  int32_t hxc_tmp_conditional_result_n25 = 0;
  if (hxc_berries < 0)
  {
    hxc_tmp_conditional_result_n25 = 0;
  }
  else
  {
    int32_t hxc_tmp_conditional_result_n26 = 0;
    if (hxc_berries > 64)
    {
      hxc_tmp_conditional_result_n26 = 64;
    }
    else
    {
      hxc_tmp_conditional_result_n26 = hxc_berries;
    }
    hxc_tmp_conditional_result_n25 = hxc_tmp_conditional_result_n26;
  }
  int32_t hxc_safeBerries = hxc_tmp_conditional_result_n25;
  int32_t hxc_tmp_conditional_result_n28 = 0;
  if (hxc_bread < 0)
  {
    hxc_tmp_conditional_result_n28 = 0;
  }
  else
  {
    int32_t hxc_tmp_conditional_result_n29 = 0;
    if (hxc_bread > 64)
    {
      hxc_tmp_conditional_result_n29 = 64;
    }
    else
    {
      hxc_tmp_conditional_result_n29 = hxc_bread;
    }
    hxc_tmp_conditional_result_n28 = hxc_tmp_conditional_result_n29;
  }
  int32_t hxc_safeBread = hxc_tmp_conditional_result_n28;
  int32_t hxc_tmp_conditional_result_n31 = 0;
  if (hxc_lantern < 0)
  {
    hxc_tmp_conditional_result_n31 = 0;
  }
  else
  {
    int32_t hxc_tmp_conditional_result_n32 = 0;
    if (hxc_lantern > 64)
    {
      hxc_tmp_conditional_result_n32 = 64;
    }
    else
    {
      hxc_tmp_conditional_result_n32 = hxc_lantern;
    }
    hxc_tmp_conditional_result_n31 = hxc_tmp_conditional_result_n32;
  }
  int32_t hxc_safeLantern = hxc_tmp_conditional_result_n31;
  int32_t hxc_tmp_load_result_n16 = hxc_safeGrass;
  int32_t hxc_tmp_load_result_n17 = hxc_safeDirt;
  int32_t hxc_tmp_load_result_n18 = hxc_safeStone;
  int32_t hxc_tmp_load_result_n19 = hxc_safeHaxeforge;
  int32_t hxc_tmp_load_result_n20 = hxc_safeSword;
  int32_t hxc_tmp_load_result_n21 = hxc_safeBerries;
  int32_t hxc_tmp_load_result_n22 = hxc_safeBread;
  return (struct hxc_caxecraft_gameplay_InventoryState){ .hxc_berries = hxc_tmp_load_result_n21, .hxc_bread = hxc_tmp_load_result_n22, .hxc_dirt = hxc_tmp_load_result_n17, .hxc_grass = hxc_tmp_load_result_n16, .hxc_haxeforge = hxc_tmp_load_result_n19, .hxc_lantern = hxc_safeLantern, .hxc_selected = hxc_selected, .hxc_stone = hxc_tmp_load_result_n18, .hxc_sword = hxc_tmp_load_result_n20 };
}

struct hxc_caxecraft_gameplay_InventoryState hxc_caxecraft_gameplay_Inventory_select(struct hxc_caxecraft_gameplay_InventoryState hxc_state, int32_t hxc_slot)
{
  bool hxc_tmp_short_circuit_result_n2 = hxc_slot < 0;
  if (!(hxc_slot < 0))
  {
    hxc_tmp_short_circuit_result_n2 = hxc_slot >= 8;
  }
  bool hxc_tmp_short_circuit_load_result_n0 = hxc_tmp_short_circuit_result_n2;
  bool hxc_tmp_short_circuit_result_n3 = hxc_tmp_short_circuit_load_result_n0;
  if (!hxc_tmp_short_circuit_load_result_n0)
  {
    hxc_tmp_short_circuit_result_n3 = hxc_slot == hxc_state.hxc_selected;
  }
  if (!hxc_tmp_short_circuit_result_n3)
  {
    struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_call_result_n11 = hxc_caxecraft_gameplay_Inventory_make(hxc_slot, hxc_state.hxc_grass, hxc_state.hxc_dirt, hxc_state.hxc_stone, hxc_state.hxc_haxeforge, hxc_state.hxc_sword, hxc_state.hxc_berries, hxc_state.hxc_bread, hxc_state.hxc_lantern);
    return hxc_tmp_call_result_n11;
  }
  return hxc_state;
}

enum hxc_caxecraft_domain_BlockKind hxc_caxecraft_gameplay_Inventory_selectedBlock(struct hxc_caxecraft_gameplay_InventoryState hxc_state)
{
  switch (hxc_state.hxc_selected) {
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

struct hxc_caxecraft_gameplay_InventoryState hxc_caxecraft_gameplay_Inventory_starter(void)
{
  struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_call_result_n0 = hxc_caxecraft_gameplay_Inventory_make(0, 16, 24, 12, 1, 1, 6, 3, 1);
  return hxc_tmp_call_result_n0;
}
