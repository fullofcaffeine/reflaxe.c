#include "hxc/program.h"

struct hxc_caxecraft_gameplay_InventoryState hxc_caxecraft_gameplay_Recovery_applyInventory(int32_t hxc_l_decision, struct hxc_caxecraft_gameplay_InventoryState hxc_l_inventory)
{
  if (!(hxc_l_decision == 1))
  {
    return hxc_l_inventory;
  }
  struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n0 = hxc_caxecraft_gameplay_Inventory_consumeSelected(hxc_l_inventory);
  return hxc_l_tmp_call_result_n0;
}

struct hxc_caxecraft_domain_VitalsState hxc_caxecraft_gameplay_Recovery_applyVitals(int32_t hxc_l_decision, struct hxc_caxecraft_domain_VitalsState hxc_l_vitals)
{
  if (!(hxc_l_decision == 1))
  {
    return hxc_l_vitals;
  }
  struct hxc_caxecraft_domain_VitalsState hxc_l_tmp_call_result_n0 = hxc_caxecraft_domain_Vitals_recover(hxc_l_vitals, 2);
  return hxc_l_tmp_call_result_n0;
}

int32_t hxc_caxecraft_gameplay_Recovery_decide(struct hxc_caxecraft_gameplay_InventoryState hxc_l_inventory, struct hxc_caxecraft_domain_VitalsState hxc_l_vitals)
{
  bool hxc_l_tmp_call_result_n1 = hxc_caxecraft_gameplay_Inventory_selectedIs(hxc_l_inventory, hxc_caxecraft_gameplay_ItemKind_Berries);
  if (!hxc_l_tmp_call_result_n1)
  {
    return 0;
  }
  if (hxc_l_vitals.hxc_health <= 0)
  {
    return 4;
  }
  if (hxc_l_inventory.hxc_berries <= 0)
  {
    return 3;
  }
  if (!(hxc_l_vitals.hxc_health >= 6))
  {
    return 1;
  }
  return 2;
}
