#include "hxc/program.h"

struct hxc_caxecraft_gameplay_MiningResult hxc_caxecraft_gameplay_Mining_attempt(uint8_t *hxc_l_cells, size_t hxc_l_length, struct hxc_caxecraft_scenario_VoxelPoint hxc_l_coordinate, struct hxc_caxecraft_gameplay_InventoryState hxc_l_inventory)
{
  uint8_t *hxc_l_borrow = hxc_l_cells;
  size_t hxc_l_tmp_length_n3 = hxc_l_length;
  (void)hxc_l_borrow;
  (void)hxc_l_tmp_length_n3;
  enum hxc_caxecraft_domain_BlockKind hxc_l_tmp_call_result_n0 = hxc_caxecraft_domain_World_query(hxc_l_cells, hxc_l_length, hxc_l_coordinate);
  enum hxc_caxecraft_domain_BlockKind hxc_l_kind = hxc_l_tmp_call_result_n0;
  struct hxc_optional_caxecraft_gameplay_ItemKind hxc_l_tmp_call_result_n2 = hxc_caxecraft_gameplay_Mining_itemForCollectableBlock(hxc_l_kind);
  struct hxc_optional_caxecraft_gameplay_ItemKind hxc_l_item = hxc_l_tmp_call_result_n2;
  if (!!hxc_l_item.hxc_has_value)
  {
    enum hxc_caxecraft_domain_BlockKind hxc_l_tmp_load_result_n5 = hxc_l_kind;
    struct hxc_optional_caxecraft_gameplay_ItemKind hxc_l_tmp_load_result_n6 = hxc_l_item;
    if (!hxc_l_tmp_load_result_n6.hxc_has_value)
    {
      abort();
    }
    struct hxc_caxecraft_gameplay_MiningResult hxc_l_tmp_call_result_n8 = hxc_caxecraft_gameplay_Mining_collect(hxc_l_cells, hxc_l_length, hxc_l_coordinate, hxc_l_inventory, hxc_l_tmp_load_result_n5, hxc_l_tmp_load_result_n6.hxc_value);
    return hxc_l_tmp_call_result_n8;
  }
  return (struct hxc_caxecraft_gameplay_MiningResult){ .hxc_inventory = hxc_l_inventory, .hxc_outcome = 0 };
}

struct hxc_caxecraft_gameplay_MiningResult hxc_caxecraft_gameplay_Mining_collect(uint8_t *hxc_l_cells, size_t hxc_l_length, struct hxc_caxecraft_scenario_VoxelPoint hxc_l_coordinate, struct hxc_caxecraft_gameplay_InventoryState hxc_l_inventory, enum hxc_caxecraft_domain_BlockKind hxc_l_kind, enum hxc_caxecraft_gameplay_ItemKind hxc_l_item)
{
  uint8_t *hxc_l_borrow = hxc_l_cells;
  size_t hxc_l_tmp_length_n5 = hxc_l_length;
  (void)hxc_l_borrow;
  (void)hxc_l_tmp_length_n5;
  int32_t hxc_l_tmp_call_result_n0 = hxc_caxecraft_gameplay_Inventory_acceptedAmount(hxc_l_inventory, hxc_l_item, 1);
  if (hxc_l_tmp_call_result_n0 != 1)
  {
    return (struct hxc_caxecraft_gameplay_MiningResult){ .hxc_inventory = hxc_l_inventory, .hxc_outcome = 1 };
  }
  bool hxc_l_tmp_call_result_n2 = hxc_caxecraft_domain_World_remove(hxc_l_cells, hxc_l_length, hxc_l_coordinate);
  if (!!hxc_l_tmp_call_result_n2)
  {
    struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n4 = hxc_caxecraft_gameplay_Inventory_collectBlock(hxc_l_inventory, hxc_l_kind);
    return (struct hxc_caxecraft_gameplay_MiningResult){ .hxc_inventory = hxc_l_tmp_call_result_n4, .hxc_outcome = 2 };
  }
  return (struct hxc_caxecraft_gameplay_MiningResult){ .hxc_inventory = hxc_l_inventory, .hxc_outcome = 0 };
}

struct hxc_optional_caxecraft_gameplay_ItemKind hxc_caxecraft_gameplay_Mining_itemForCollectableBlock(enum hxc_caxecraft_domain_BlockKind hxc_l_kind)
{
  switch (hxc_l_kind) {
    case hxc_caxecraft_domain_BlockKind_Grass:
      {
        break;
      }
    case hxc_caxecraft_domain_BlockKind_Dirt:
      {
        return (struct hxc_optional_caxecraft_gameplay_ItemKind){ .hxc_has_value = true, .hxc_value = hxc_caxecraft_gameplay_ItemKind_DirtBlock };
      }
    case hxc_caxecraft_domain_BlockKind_Stone:
      {
        return (struct hxc_optional_caxecraft_gameplay_ItemKind){ .hxc_has_value = true, .hxc_value = hxc_caxecraft_gameplay_ItemKind_StoneBlock };
      }
    case hxc_caxecraft_domain_BlockKind_Sand:
      {
        return (struct hxc_optional_caxecraft_gameplay_ItemKind){ .hxc_has_value = true, .hxc_value = hxc_caxecraft_gameplay_ItemKind_SandBlock };
      }
    case hxc_caxecraft_domain_BlockKind_Air:
    case hxc_caxecraft_domain_BlockKind_Bedrock:
    case hxc_caxecraft_domain_BlockKind_Wood:
    case hxc_caxecraft_domain_BlockKind_Leaves:
    case hxc_caxecraft_domain_BlockKind_Snow:
    case hxc_caxecraft_domain_BlockKind_Ash:
      {
        return (struct hxc_optional_caxecraft_gameplay_ItemKind){ .hxc_has_value = false };
      }
    default:
      {
        abort();
      }
  }
  return (struct hxc_optional_caxecraft_gameplay_ItemKind){ .hxc_has_value = true, .hxc_value = hxc_caxecraft_gameplay_ItemKind_GrassBlock };
}
