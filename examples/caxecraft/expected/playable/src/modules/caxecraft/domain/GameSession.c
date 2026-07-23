#include "hxc/program.h"

void hxc_compiler_constructor_caxecraft_domain_GameSession(struct hxc_caxecraft_domain_GameSession *hxc_self)
{
  struct hxc_caxecraft_domain_WaterSimulation *hxc_tmp_owned_class_field_address_n0 = &(*hxc_self).hxc_water;
  hxc_compiler_constructor_caxecraft_domain_WaterSimulation(hxc_tmp_owned_class_field_address_n0);
  (*hxc_self).hxc_completedTicks = 0;
  struct hxc_caxecraft_domain_EntityStore *hxc_tmp_owned_class_field_address_n1 = &(*hxc_self).hxc_entities;
  hxc_compiler_constructor_caxecraft_domain_EntityStore(hxc_tmp_owned_class_field_address_n1);
  struct hxc_caxecraft_domain_PlayerAgent hxc_tmp_call_result_n2 = hxc_caxecraft_domain_PlayerAgent_bind(0);
  (*hxc_self).hxc_localPlayer = hxc_tmp_call_result_n2;
  uint8_t *hxc_pending = (*hxc_self).hxc_pendingWaterStorage;
  size_t hxc_tmp_length_n1 = 16384;
  struct hxc_caxecraft_domain_WaterSimulation *hxc_tmp_owned_class_field_address_n3 = &(*hxc_self).hxc_water;
  if (hxc_tmp_owned_class_field_address_n3 == NULL)
  {
    abort();
  }
  hxc_caxecraft_domain_WaterSimulation_resetPending(hxc_tmp_owned_class_field_address_n3, hxc_pending, hxc_tmp_length_n1);
  return;
}

bool hxc_caxecraft_domain_GameSession_activateAuthoredItemDuringLoad(struct hxc_caxecraft_domain_GameSession *hxc_self, int32_t hxc_index)
{
  bool hxc_tmp_short_circuit_result_n2 = hxc_index < 0;
  if (!(hxc_index < 0))
  {
    hxc_tmp_short_circuit_result_n2 = hxc_index >= 256;
  }
  if (!hxc_tmp_short_circuit_result_n2)
  {
    if (hxc_self == NULL)
    {
      abort();
    }
    if (hxc_index < 0 || (size_t)hxc_index >= 256)
    {
      abort();
    }
    (*hxc_self).hxc_authoredItemStorage[(size_t)hxc_index] = 1;
    return true;
  }
  return false;
}

bool hxc_caxecraft_domain_GameSession_bindLocalPlayer(struct hxc_caxecraft_domain_GameSession *hxc_self, struct hxc_caxecraft_domain_Character hxc_character)
{
  if (hxc_self == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_domain_EntityStore *hxc_tmp_owned_class_field_address_n0 = &(*hxc_self).hxc_entities;
  if (hxc_tmp_owned_class_field_address_n0 == NULL)
  {
    abort();
  }
  bool hxc_tmp_instance_call_result_n1 = hxc_caxecraft_domain_EntityStore_put(hxc_tmp_owned_class_field_address_n0, hxc_character);
  if (!!hxc_tmp_instance_call_result_n1)
  {
    struct hxc_caxecraft_domain_PlayerAgent hxc_tmp_call_result_n3 = hxc_caxecraft_domain_PlayerAgent_bind(hxc_character.hxc_id);
    (*hxc_self).hxc_localPlayer = hxc_tmp_call_result_n3;
    return true;
  }
  return false;
}

bool hxc_caxecraft_domain_GameSession_placeInitialWaterVolume(struct hxc_caxecraft_domain_GameSession *hxc_self, struct hxc_caxecraft_domain_BlockCoord hxc_origin, int32_t hxc_width, int32_t hxc_height, int32_t hxc_depth)
{
  if (hxc_self == NULL)
  {
    abort();
  }
  uint8_t *hxc_cells = (*hxc_self).hxc_worldStorage;
  size_t hxc_tmp_length_n5 = 16384;
  uint8_t *hxc_pending = (*hxc_self).hxc_pendingWaterStorage;
  size_t hxc_tmp_length_n6 = 16384;
  struct hxc_caxecraft_domain_WaterSimulation *hxc_tmp_owned_class_field_address_n0 = &(*hxc_self).hxc_water;
  if (hxc_tmp_owned_class_field_address_n0 == NULL)
  {
    abort();
  }
  bool hxc_tmp_instance_call_result_n1 = hxc_caxecraft_domain_WaterSimulation_placeInitialVolume(hxc_tmp_owned_class_field_address_n0, hxc_cells, hxc_tmp_length_n5, hxc_pending, hxc_tmp_length_n6, hxc_origin, hxc_width, hxc_height, hxc_depth);
  return hxc_tmp_instance_call_result_n1;
}

bool hxc_caxecraft_domain_GameSession_placeWaterSource(struct hxc_caxecraft_domain_GameSession *hxc_self, struct hxc_caxecraft_domain_BlockCoord hxc_coord)
{
  if (hxc_self == NULL)
  {
    abort();
  }
  uint8_t *hxc_cells = (*hxc_self).hxc_worldStorage;
  size_t hxc_tmp_length_n2 = 16384;
  uint8_t *hxc_pending = (*hxc_self).hxc_pendingWaterStorage;
  size_t hxc_tmp_length_n3 = 16384;
  struct hxc_caxecraft_domain_WaterSimulation *hxc_tmp_owned_class_field_address_n0 = &(*hxc_self).hxc_water;
  if (hxc_tmp_owned_class_field_address_n0 == NULL)
  {
    abort();
  }
  bool hxc_tmp_instance_call_result_n1 = hxc_caxecraft_domain_WaterSimulation_placeSource(hxc_tmp_owned_class_field_address_n0, hxc_cells, hxc_tmp_length_n2, hxc_pending, hxc_tmp_length_n3, hxc_coord);
  return hxc_tmp_instance_call_result_n1;
}

void hxc_caxecraft_domain_GameSession_resetEmptyWorld(struct hxc_caxecraft_domain_GameSession *hxc_self)
{
  if (hxc_self == NULL)
  {
    abort();
  }
  uint8_t *hxc_cells = (*hxc_self).hxc_worldStorage;
  size_t hxc_tmp_length_n1 = 16384;
  uint8_t *hxc_pending = (*hxc_self).hxc_pendingWaterStorage;
  size_t hxc_tmp_length_n2 = 16384;
  int32_t hxc_index = 0;
  while (1)
  {
    if (!(hxc_index < 16384))
    {
      break;
    }
    hxc_caxecraft_domain_WorldStorage_writeCode(hxc_cells, hxc_tmp_length_n1, hxc_index, 0);
    hxc_index = hxc_i32_add_wrapping(hxc_index, 1);
  }
  struct hxc_caxecraft_domain_WaterSimulation *hxc_tmp_owned_class_field_address_n3 = &(*hxc_self).hxc_water;
  if (hxc_tmp_owned_class_field_address_n3 == NULL)
  {
    abort();
  }
  hxc_caxecraft_domain_WaterSimulation_resetPending(hxc_tmp_owned_class_field_address_n3, hxc_pending, hxc_tmp_length_n2);
  return;
}

struct hxc_caxecraft_domain_GameTickResult hxc_caxecraft_domain_GameSession_tick(struct hxc_caxecraft_domain_GameSession *hxc_self, struct hxc_caxecraft_domain_GameTickInput hxc_input)
{
  if (hxc_self == NULL)
  {
    abort();
  }
  uint8_t *hxc_cells = (*hxc_self).hxc_worldStorage;
  size_t hxc_tmp_length_n2 = 16384;
  uint8_t *hxc_pending = (*hxc_self).hxc_pendingWaterStorage;
  size_t hxc_tmp_length_n3 = 16384;
  struct hxc_caxecraft_domain_PlayerAgent hxc_tmp_class_field_load_result_n0 = (*hxc_self).hxc_localPlayer;
  int32_t hxc_characterId = hxc_tmp_class_field_load_result_n0.hxc_characterId;
  struct hxc_caxecraft_domain_EntityStore *hxc_tmp_owned_class_field_address_n2 = &(*hxc_self).hxc_entities;
  if (hxc_tmp_owned_class_field_address_n2 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_domain_Character hxc_tmp_instance_call_result_n4 = hxc_caxecraft_domain_EntityStore_read(hxc_tmp_owned_class_field_address_n2, hxc_characterId);
  struct hxc_caxecraft_domain_Character hxc_original = hxc_tmp_instance_call_result_n4;
  if (!(hxc_original.hxc_id <= 0))
  {
    struct hxc_caxecraft_domain_WaterSimulation *hxc_tmp_owned_class_field_address_n13 = &(*hxc_self).hxc_water;
    if (hxc_tmp_owned_class_field_address_n13 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_domain_WaterTickResult hxc_tmp_instance_call_result_n15 = hxc_caxecraft_domain_WaterSimulation_tick(hxc_tmp_owned_class_field_address_n13, hxc_cells, hxc_tmp_length_n2, hxc_pending, hxc_tmp_length_n3, hxc_input.hxc_waterUpdateBudget);
    struct hxc_caxecraft_domain_WaterTickResult hxc_waterResult = hxc_tmp_instance_call_result_n15;
    struct hxc_caxecraft_domain_CharacterStep hxc_tmp_call_result_n19 = hxc_caxecraft_domain_Character_step(hxc_cells, hxc_tmp_length_n2, hxc_original, hxc_input.hxc_intent, hxc_input.hxc_damagePolicy);
    struct hxc_caxecraft_domain_CharacterStep hxc_characterResult = hxc_tmp_call_result_n19;
    struct hxc_caxecraft_domain_EntityStore *hxc_tmp_owned_class_field_address_n20 = &(*hxc_self).hxc_entities;
    if (hxc_tmp_owned_class_field_address_n20 == NULL)
    {
      abort();
    }
    int32_t hxc_tmp_load_result_n21 = hxc_characterId;
    bool hxc_tmp_instance_call_result_n23 = hxc_caxecraft_domain_EntityStore_replace(hxc_tmp_owned_class_field_address_n20, hxc_tmp_load_result_n21, hxc_characterResult.hxc_character);
    bool hxc_committed = hxc_tmp_instance_call_result_n23;
    bool hxc_tmp_load_result_n24 = hxc_committed;
    int32_t hxc_tmp_conditional_result_n10 = 0;
    if (hxc_tmp_load_result_n24)
    {
      int32_t hxc_tmp_class_field_load_result_n25 = (*hxc_self).hxc_completedTicks;
      hxc_tmp_conditional_result_n10 = hxc_tmp_class_field_load_result_n25;
    }
    else
    {
      hxc_tmp_conditional_result_n10 = -1;
    }
    int32_t hxc_tickIndex = hxc_tmp_conditional_result_n10;
    if (hxc_committed)
    {
      int32_t hxc_tmp_increment_load_result_n28 = (*hxc_self).hxc_completedTicks;
      (*hxc_self).hxc_completedTicks = hxc_i32_add_wrapping(hxc_tmp_increment_load_result_n28, 1);
    }
    int32_t hxc_tmp_load_result_n29 = hxc_tickIndex;
    struct hxc_caxecraft_domain_Character hxc_tmp_record_field_load_result_n30 = hxc_characterResult.hxc_character;
    struct hxc_caxecraft_domain_Immersion hxc_tmp_record_field_load_result_n31 = hxc_characterResult.hxc_immersion;
    int32_t hxc_tmp_record_field_load_result_n32 = hxc_characterResult.hxc_drowningDamage;
    struct hxc_caxecraft_domain_WaterTickResult hxc_tmp_load_result_n33 = hxc_waterResult;
    return (struct hxc_caxecraft_domain_GameTickResult){ .hxc_character = hxc_tmp_record_field_load_result_n30, .hxc_committed = hxc_committed, .hxc_drowningDamage = hxc_tmp_record_field_load_result_n32, .hxc_immersion = hxc_tmp_record_field_load_result_n31, .hxc_tickIndex = hxc_tmp_load_result_n29, .hxc_water = hxc_tmp_load_result_n33 };
  }
  struct hxc_caxecraft_domain_Character hxc_tmp_load_result_n6 = hxc_original;
  struct hxc_caxecraft_domain_Immersion hxc_tmp_call_result_n8 = hxc_caxecraft_domain_Aquatics_observe(hxc_cells, hxc_tmp_length_n2, hxc_original.hxc_body);
  struct hxc_caxecraft_domain_WaterSimulation *hxc_tmp_owned_class_field_address_n9 = &(*hxc_self).hxc_water;
  int32_t hxc_tmp_class_field_load_result_n10 = (*hxc_tmp_owned_class_field_address_n9).hxc_pendingCount;
  return (struct hxc_caxecraft_domain_GameTickResult){ .hxc_character = hxc_tmp_load_result_n6, .hxc_committed = false, .hxc_drowningDamage = 0, .hxc_immersion = hxc_tmp_call_result_n8, .hxc_tickIndex = -1, .hxc_water = (struct hxc_caxecraft_domain_WaterTickResult){ .hxc_changed = 0, .hxc_processed = 0, .hxc_remaining = hxc_tmp_class_field_load_result_n10 } };
}

struct hxc_caxecraft_domain_GameView hxc_caxecraft_domain_GameSession_view(struct hxc_caxecraft_domain_GameSession *hxc_self)
{
  if (hxc_self == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_domain_EntityStore *hxc_tmp_owned_class_field_address_n0 = &(*hxc_self).hxc_entities;
  if (hxc_tmp_owned_class_field_address_n0 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_domain_PlayerAgent hxc_tmp_class_field_load_result_n1 = (*hxc_self).hxc_localPlayer;
  struct hxc_caxecraft_domain_Character hxc_tmp_instance_call_result_n3 = hxc_caxecraft_domain_EntityStore_read(hxc_tmp_owned_class_field_address_n0, hxc_tmp_class_field_load_result_n1.hxc_characterId);
  struct hxc_caxecraft_domain_Character hxc_character = hxc_tmp_instance_call_result_n3;
  int32_t hxc_tmp_record_field_load_result_n4 = hxc_character.hxc_id;
  struct hxc_caxecraft_domain_Character hxc_tmp_load_result_n5 = hxc_character;
  int32_t hxc_tmp_class_field_load_result_n6 = (*hxc_self).hxc_completedTicks;
  return (struct hxc_caxecraft_domain_GameView){ .hxc_completedTicks = hxc_tmp_class_field_load_result_n6, .hxc_localPlayer = hxc_tmp_load_result_n5, .hxc_valid = hxc_tmp_record_field_load_result_n4 > 0 };
}

int32_t hxc_caxecraft_domain_GameSession_writeTerrainRunDuringLoad(struct hxc_caxecraft_domain_GameSession *hxc_self, int32_t hxc_startIndex, int32_t hxc_storageCode, int32_t hxc_count)
{
  bool hxc_tmp_short_circuit_result_n4 = hxc_startIndex < 0;
  if (!(hxc_startIndex < 0))
  {
    hxc_tmp_short_circuit_result_n4 = hxc_count <= 0;
  }
  bool hxc_tmp_short_circuit_load_result_n0 = hxc_tmp_short_circuit_result_n4;
  bool hxc_tmp_short_circuit_result_n5 = hxc_tmp_short_circuit_load_result_n0;
  if (!hxc_tmp_short_circuit_load_result_n0)
  {
    hxc_tmp_short_circuit_result_n5 = hxc_startIndex > hxc_i32_subtract_wrapping(16384, hxc_count);
  }
  bool hxc_tmp_short_circuit_load_result_n1 = hxc_tmp_short_circuit_result_n5;
  bool hxc_tmp_short_circuit_result_n6 = hxc_tmp_short_circuit_load_result_n1;
  if (!hxc_tmp_short_circuit_load_result_n1)
  {
    bool hxc_tmp_short_circuit_result_n7 = hxc_storageCode != 0;
    if (hxc_storageCode != 0)
    {
      bool hxc_tmp_short_circuit_result_n8 = hxc_storageCode >= 1;
      if (hxc_storageCode >= 1)
      {
        hxc_tmp_short_circuit_result_n8 = hxc_storageCode <= 9;
      }
      hxc_tmp_short_circuit_result_n7 = !hxc_tmp_short_circuit_result_n8;
    }
    hxc_tmp_short_circuit_result_n6 = hxc_tmp_short_circuit_result_n7;
  }
  if (hxc_tmp_short_circuit_result_n6)
  {
    return -1;
  }
  if (hxc_self == NULL)
  {
    abort();
  }
  uint8_t *hxc_cells = (*hxc_self).hxc_worldStorage;
  size_t hxc_tmp_length_n9 = 16384;
  int32_t hxc_end = hxc_i32_add_wrapping(hxc_startIndex, hxc_count);
  int32_t hxc_index = hxc_startIndex;
  while (1)
  {
    int32_t hxc_tmp_load_result_n5 = hxc_index;
    if (!(hxc_tmp_load_result_n5 < hxc_end))
    {
      break;
    }
    hxc_caxecraft_domain_WorldStorage_writeCode(hxc_cells, hxc_tmp_length_n9, hxc_index, hxc_storageCode);
    hxc_index = hxc_i32_add_wrapping(hxc_index, 1);
  }
  return hxc_end;
}
