#include "hxc/program.h"

struct hxc_caxecraft_gameplay_InventoryState hxc_caxecraft_gameplay_Recovery_applyInventory(int32_t hxc_decision, struct hxc_caxecraft_gameplay_InventoryState hxc_inventory)
{
  if (!(hxc_decision == 1))
  {
    return hxc_inventory;
  }
  struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_call_result_n0 = hxc_caxecraft_gameplay_Inventory_consumeSelected(hxc_inventory);
  return hxc_tmp_call_result_n0;
}

struct hxc_caxecraft_domain_VitalsState hxc_caxecraft_gameplay_Recovery_applyVitals(int32_t hxc_decision, struct hxc_caxecraft_domain_VitalsState hxc_vitals)
{
  if (!(hxc_decision == 1))
  {
    return hxc_vitals;
  }
  struct hxc_caxecraft_domain_VitalsState hxc_tmp_call_result_n0 = hxc_caxecraft_domain_Vitals_recover(hxc_vitals, 2);
  return hxc_tmp_call_result_n0;
}

int32_t hxc_caxecraft_gameplay_Recovery_decide(struct hxc_caxecraft_gameplay_InventoryState hxc_inventory, struct hxc_caxecraft_domain_VitalsState hxc_vitals)
{
  bool hxc_tmp_call_result_n1 = hxc_caxecraft_gameplay_Inventory_selectedIs(hxc_inventory, hxc_caxecraft_gameplay_ItemKind_Berries);
  if (!!hxc_tmp_call_result_n1)
  {
    if (!(hxc_vitals.hxc_health <= 0))
    {
      if (!(hxc_inventory.hxc_berries <= 0))
      {
        if (!(hxc_vitals.hxc_health >= 6))
        {
          return 1;
        }
        return 2;
      }
      return 3;
    }
    return 4;
  }
  return 0;
}
