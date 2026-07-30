#include "hxc/program.h"

void hxc_compiler_constructor_caxecraft_domain_GameSession(struct hxc_caxecraft_domain_GameSession *hxc_self)
{
  struct hxc_caxecraft_domain_WaterSimulation *hxc_tmp_owned_class_field_address_n0 = &(*hxc_self).hxc_water;
  hxc_compiler_constructor_caxecraft_domain_WaterSimulation(hxc_tmp_owned_class_field_address_n0);
  (*hxc_self).hxc_completedTicks = 0;
  struct hxc_array_ref *hxc_tmp_array_create_result_n1 = NULL;
  if (hxc_array_ref_create(hxc_default_allocator(), (hxc_array_element_ops){ sizeof(struct hxc_caxecraft_domain_ActorControllerEvent), _Alignof(struct hxc_caxecraft_domain_ActorControllerEvent), NULL, hxc_array_43e9f8ca_element_copy, hxc_array_43e9f8ca_element_assign, hxc_array_43e9f8ca_element_destroy }, &hxc_tmp_array_create_result_n1) != HXC_STATUS_OK)
  {
    abort();
  }
  (*hxc_self).hxc_actorControllerEvents = hxc_tmp_array_create_result_n1;
  struct hxc_array_ref *hxc_tmp_array_create_result_n2 = NULL;
  if (hxc_array_ref_create(hxc_default_allocator(), (hxc_array_element_ops){ sizeof(struct hxc_caxecraft_domain_ActorControllerState), _Alignof(struct hxc_caxecraft_domain_ActorControllerState), NULL, hxc_array_3767ae6c_element_copy, hxc_array_3767ae6c_element_assign, hxc_array_3767ae6c_element_destroy }, &hxc_tmp_array_create_result_n2) != HXC_STATUS_OK)
  {
    abort();
  }
  (*hxc_self).hxc_actorControllers = hxc_tmp_array_create_result_n2;
  struct hxc_caxecraft_domain_EntityStore *hxc_tmp_owned_class_field_address_n3 = &(*hxc_self).hxc_entities;
  hxc_compiler_constructor_caxecraft_domain_EntityStore(hxc_tmp_owned_class_field_address_n3);
  struct hxc_caxecraft_domain_PlayerAgent hxc_tmp_call_result_n4 = hxc_caxecraft_domain_PlayerAgent_bind(0);
  (*hxc_self).hxc_localPlayer = hxc_tmp_call_result_n4;
  struct hxc_caxecraft_domain_WaterSimulation *hxc_tmp_owned_class_field_address_n5 = &(*hxc_self).hxc_water;
  if (hxc_tmp_owned_class_field_address_n5 == NULL)
  {
    abort();
  }
  hxc_caxecraft_domain_WaterSimulation_resetPending(hxc_tmp_owned_class_field_address_n5);
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

struct hxc_array_ref *hxc_caxecraft_domain_GameSession_actorControllerEventSnapshots(struct hxc_caxecraft_domain_GameSession *hxc_self)
{
  if (hxc_self == NULL)
  {
    abort();
  }
  struct hxc_array_ref *hxc_tmp_class_field_load_result_n0 = (*hxc_self).hxc_actorControllerEvents;
  struct hxc_array_ref *hxc_tmp_array_copy_result_n1 = NULL;
  if (hxc_array_ref_copy(hxc_tmp_class_field_load_result_n0, &hxc_tmp_array_copy_result_n1) != HXC_STATUS_OK)
  {
    abort();
  }
  return hxc_tmp_array_copy_result_n1;
}

struct hxc_array_ref *hxc_caxecraft_domain_GameSession_actorControllerStateSnapshots(struct hxc_caxecraft_domain_GameSession *hxc_self)
{
  if (hxc_self == NULL)
  {
    abort();
  }
  struct hxc_array_ref *hxc_tmp_class_field_load_result_n0 = (*hxc_self).hxc_actorControllers;
  struct hxc_array_ref *hxc_tmp_array_copy_result_n1 = NULL;
  if (hxc_array_ref_copy(hxc_tmp_class_field_load_result_n0, &hxc_tmp_array_copy_result_n1) != HXC_STATUS_OK)
  {
    abort();
  }
  return hxc_tmp_array_copy_result_n1;
}

struct hxc_caxecraft_domain_ActorControllerTickResult hxc_caxecraft_domain_GameSession_actorControllerTick(struct hxc_caxecraft_domain_GameSession *hxc_self, struct hxc_caxecraft_domain_ActorControllerTickStatus hxc_status, int32_t hxc_processed)
{
  if (hxc_self == NULL)
  {
    abort();
  }
  struct hxc_array_ref *hxc_tmp_class_field_load_result_n0 = (*hxc_self).hxc_actorControllerEvents;
  int32_t hxc_tmp_array_length_result_n1;
  if (hxc_array_ref_length(hxc_tmp_class_field_load_result_n0, &hxc_tmp_array_length_result_n1) != HXC_STATUS_OK)
  {
    abort();
  }
  return (struct hxc_caxecraft_domain_ActorControllerTickResult){ .hxc_emittedEvents = hxc_tmp_array_length_result_n1, .hxc_processed = hxc_processed, .hxc_status = hxc_status };
}

bool hxc_caxecraft_domain_GameSession_actorInteractionAvailable(struct hxc_caxecraft_domain_GameSession *hxc_self, int32_t hxc_id)
{
  struct hxc_caxecraft_domain_ActorControllerState hxc_controller = { 0 };
  struct hxc_caxecraft_domain_ActorControllerState hxc_tmp_array_element_owner_n7 = { 0 };
  if (hxc_self == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_domain_Character hxc_tmp_instance_call_result_n0 = hxc_caxecraft_domain_GameSession_readCharacter(hxc_self, hxc_id);
  struct hxc_caxecraft_domain_Character hxc_actor = hxc_tmp_instance_call_result_n0;
  struct hxc_caxecraft_domain_EntityStore *hxc_tmp_owned_class_field_address_n1 = &(*hxc_self).hxc_entities;
  if (hxc_tmp_owned_class_field_address_n1 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_domain_PlayerAgent hxc_tmp_class_field_load_result_n2 = (*hxc_self).hxc_localPlayer;
  struct hxc_caxecraft_domain_Character hxc_tmp_instance_call_result_n4 = hxc_caxecraft_domain_EntityStore_read(hxc_tmp_owned_class_field_address_n1, hxc_tmp_class_field_load_result_n2.hxc_characterId);
  struct hxc_caxecraft_domain_Character hxc_player = hxc_tmp_instance_call_result_n4;
  int32_t hxc_g = 0;
  struct hxc_array_ref *hxc_tmp_class_field_load_result_n5 = (*hxc_self).hxc_actorControllers;
  struct hxc_array_ref *hxc_g1 = hxc_tmp_class_field_load_result_n5;
  if (hxc_array_ref_retain(hxc_g1) != HXC_STATUS_OK)
  {
    abort();
  }
  while (1)
  {
    int32_t hxc_tmp_load_result_n6 = hxc_g;
    int32_t hxc_tmp_array_length_result_n8;
    if (hxc_array_ref_length(hxc_g1, &hxc_tmp_array_length_result_n8) != HXC_STATUS_OK)
    {
      abort();
    }
    if (!(hxc_tmp_load_result_n6 < hxc_tmp_array_length_result_n8))
    {
      break;
    }
    struct hxc_array_ref *hxc_tmp_load_result_n9 = hxc_g1;
    struct hxc_caxecraft_domain_ActorControllerState hxc_tmp_array_get_result_n11;
    if (hxc_array_ref_get_copy(hxc_tmp_load_result_n9, (size_t)hxc_g, &hxc_tmp_array_get_result_n11) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_tmp_array_element_owner_n7 = hxc_tmp_array_get_result_n11;
    hxc_controller = hxc_tmp_array_element_owner_n7;
    if (hxc_record_50dff1c7_retain(&hxc_controller) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_g = hxc_i32_add_wrapping(hxc_g, 1);
    if (hxc_controller.hxc_characterId == hxc_id)
    {
      struct hxc_caxecraft_domain_ActorControllerState hxc_tmp_load_result_n15 = hxc_controller;
      struct hxc_caxecraft_domain_Character hxc_tmp_load_result_n16 = hxc_actor;
      bool hxc_tmp_call_result_n18 = hxc_caxecraft_domain_ActorControllerScheduler_interactionAvailable(hxc_tmp_load_result_n15, hxc_tmp_load_result_n16, hxc_player);
      hxc_record_50dff1c7_destroy(&hxc_controller);
      hxc_array_3767ae6c_element_destroy(NULL, &hxc_tmp_array_element_owner_n7);
      if (hxc_array_ref_release(hxc_g1) != HXC_STATUS_OK)
      {
        abort();
      }
      return hxc_tmp_call_result_n18;
    }
    hxc_record_50dff1c7_destroy(&hxc_controller);
    hxc_array_3767ae6c_element_destroy(NULL, &hxc_tmp_array_element_owner_n7);
  }
  if (hxc_array_ref_release(hxc_g1) != HXC_STATUS_OK)
  {
    abort();
  }
  return false;
}

bool hxc_caxecraft_domain_GameSession_authoredItemIsActive(struct hxc_caxecraft_domain_GameSession *hxc_self, int32_t hxc_index)
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
    int32_t hxc_tmp_collection_index_load_result_n1 = (*hxc_self).hxc_authoredItemStorage[(size_t)hxc_index];
    return hxc_tmp_collection_index_load_result_n1 != 0;
  }
  return false;
}

const int32_t *hxc_caxecraft_domain_GameSession_authoredItemsView(struct hxc_caxecraft_domain_GameSession *hxc_self, size_t *hxc_returned_span_length)
{
  if (hxc_self == NULL)
  {
    abort();
  }
  *hxc_returned_span_length = 256;
  return (*hxc_self).hxc_authoredItemStorage;
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

void hxc_caxecraft_domain_GameSession_clearActorControllerEvents(struct hxc_caxecraft_domain_GameSession *hxc_self)
{
  while (1)
  {
    if (hxc_self == NULL)
    {
      abort();
    }
    struct hxc_array_ref *hxc_tmp_class_field_load_result_n0 = (*hxc_self).hxc_actorControllerEvents;
    int32_t hxc_tmp_array_length_result_n1;
    if (hxc_array_ref_length(hxc_tmp_class_field_load_result_n0, &hxc_tmp_array_length_result_n1) != HXC_STATUS_OK)
    {
      abort();
    }
    if (!(hxc_tmp_array_length_result_n1 > 0))
    {
      break;
    }
    struct hxc_array_ref *hxc_tmp_class_field_load_result_n2 = (*hxc_self).hxc_actorControllerEvents;
    struct hxc_optional_caxecraft_domain_ActorControllerEvent hxc_tmp_array_pop_result_n3 = (struct hxc_optional_caxecraft_domain_ActorControllerEvent){ .hxc_has_value = false };
    if (hxc_array_ref_pop_move(hxc_tmp_class_field_load_result_n2, &hxc_tmp_array_pop_result_n3.hxc_value, &hxc_tmp_array_pop_result_n3.hxc_has_value) != HXC_STATUS_OK)
    {
      abort();
    }
    struct hxc_optional_caxecraft_domain_ActorControllerEvent hxc_tmp_discarded_optional_owner_n1 = hxc_tmp_array_pop_result_n3;
    (void)hxc_tmp_discarded_optional_owner_n1;
    hxc_optional_cb78aeb6_destroy(&hxc_tmp_discarded_optional_owner_n1);
  }
  return;
}

struct hxc_caxecraft_domain_AuthoredAquaticEquipmentResult hxc_caxecraft_domain_GameSession_collectAuthoredAquaticEquipment(struct hxc_caxecraft_domain_GameSession *hxc_self, int32_t hxc_index, struct hxc_caxecraft_domain_AquaticProfile hxc_replacement)
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
  struct hxc_caxecraft_domain_Character hxc_original = hxc_tmp_instance_call_result_n3;
  if (hxc_original.hxc_id <= 0)
  {
    return (struct hxc_caxecraft_domain_AuthoredAquaticEquipmentResult){ .hxc_character = hxc_original, .hxc_collected = false, .hxc_resolved = false };
  }
  bool hxc_tmp_instance_call_result_n7 = hxc_caxecraft_domain_GameSession_authoredItemIsActive(hxc_self, hxc_index);
  if (!hxc_tmp_instance_call_result_n7)
  {
    return (struct hxc_caxecraft_domain_AuthoredAquaticEquipmentResult){ .hxc_character = hxc_original, .hxc_collected = false, .hxc_resolved = true };
  }
  struct hxc_caxecraft_domain_Character hxc_tmp_load_result_n10 = hxc_original;
  struct hxc_caxecraft_domain_Character hxc_tmp_call_result_n12 = hxc_caxecraft_domain_Character_adoptProfile(hxc_original, hxc_replacement);
  struct hxc_caxecraft_domain_LocalCharacterCommandResult hxc_tmp_instance_call_result_n13 = hxc_caxecraft_domain_GameSession_commitLocalCharacter(hxc_self, hxc_tmp_load_result_n10, hxc_tmp_call_result_n12);
  struct hxc_caxecraft_domain_LocalCharacterCommandResult hxc_committed = hxc_tmp_instance_call_result_n13;
  if (hxc_committed.hxc_resolved)
  {
    if (hxc_index < 0 || (size_t)hxc_index >= 256)
    {
      abort();
    }
    (*hxc_self).hxc_authoredItemStorage[(size_t)hxc_index] = 0;
  }
  struct hxc_caxecraft_domain_Character hxc_tmp_record_field_load_result_n15 = hxc_committed.hxc_character;
  bool hxc_tmp_record_field_load_result_n16 = hxc_committed.hxc_resolved;
  return (struct hxc_caxecraft_domain_AuthoredAquaticEquipmentResult){ .hxc_character = hxc_tmp_record_field_load_result_n15, .hxc_collected = hxc_tmp_record_field_load_result_n16, .hxc_resolved = hxc_committed.hxc_resolved };
}

struct hxc_caxecraft_domain_LocalCharacterCommandResult hxc_caxecraft_domain_GameSession_commitLocalCharacter(struct hxc_caxecraft_domain_GameSession *hxc_self, struct hxc_caxecraft_domain_Character hxc_original, struct hxc_caxecraft_domain_Character hxc_replacement)
{
  bool hxc_tmp_short_circuit_result_n4 = hxc_original.hxc_id > 0;
  if (hxc_original.hxc_id > 0)
  {
    if (hxc_self == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_domain_EntityStore *hxc_tmp_owned_class_field_address_n1 = &(*hxc_self).hxc_entities;
    if (hxc_tmp_owned_class_field_address_n1 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_domain_PlayerAgent hxc_tmp_class_field_load_result_n2 = (*hxc_self).hxc_localPlayer;
    bool hxc_tmp_instance_call_result_n4 = hxc_caxecraft_domain_EntityStore_replace(hxc_tmp_owned_class_field_address_n1, hxc_tmp_class_field_load_result_n2.hxc_characterId, hxc_replacement);
    hxc_tmp_short_circuit_result_n4 = hxc_tmp_instance_call_result_n4;
  }
  bool hxc_resolved = hxc_tmp_short_circuit_result_n4;
  bool hxc_tmp_load_result_n6 = hxc_resolved;
  struct hxc_caxecraft_domain_Character hxc_tmp_conditional_result_n5;
  if (hxc_tmp_load_result_n6)
  {
    hxc_tmp_conditional_result_n5 = hxc_replacement;
  }
  else
  {
    hxc_tmp_conditional_result_n5 = hxc_original;
  }
  struct hxc_caxecraft_domain_Character hxc_tmp_conditional_load_result_n7 = hxc_tmp_conditional_result_n5;
  return (struct hxc_caxecraft_domain_LocalCharacterCommandResult){ .hxc_character = hxc_tmp_conditional_load_result_n7, .hxc_resolved = hxc_resolved };
}

struct hxc_caxecraft_domain_CharacterDamageResult hxc_caxecraft_domain_GameSession_damageCharacter(struct hxc_caxecraft_domain_GameSession *hxc_self, int32_t hxc_id, int32_t hxc_amount)
{
  if (hxc_self == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_domain_Character hxc_tmp_instance_call_result_n0 = hxc_caxecraft_domain_GameSession_readCharacter(hxc_self, hxc_id);
  struct hxc_caxecraft_domain_Character hxc_original = hxc_tmp_instance_call_result_n0;
  int32_t hxc_tmp_record_field_load_result_n1 = hxc_original.hxc_id;
  bool hxc_tmp_short_circuit_result_n4 = hxc_tmp_record_field_load_result_n1 <= 0;
  if (!(hxc_tmp_record_field_load_result_n1 <= 0))
  {
    hxc_tmp_short_circuit_result_n4 = hxc_amount <= 0;
  }
  if (hxc_tmp_short_circuit_result_n4)
  {
    return (struct hxc_caxecraft_domain_CharacterDamageResult){ .hxc_character = hxc_original, .hxc_damageApplied = 0, .hxc_defeated = false, .hxc_resolved = false };
  }
  struct hxc_caxecraft_domain_Character hxc_tmp_call_result_n6 = hxc_caxecraft_domain_Character_applyDamage(hxc_original, hxc_amount);
  struct hxc_caxecraft_domain_Character hxc_replacement = hxc_tmp_call_result_n6;
  struct hxc_caxecraft_domain_EntityStore *hxc_tmp_owned_class_field_address_n7 = &(*hxc_self).hxc_entities;
  if (hxc_tmp_owned_class_field_address_n7 == NULL)
  {
    abort();
  }
  bool hxc_tmp_instance_call_result_n9 = hxc_caxecraft_domain_EntityStore_replace(hxc_tmp_owned_class_field_address_n7, hxc_id, hxc_replacement);
  bool hxc_resolved = hxc_tmp_instance_call_result_n9;
  if (!hxc_resolved)
  {
    return (struct hxc_caxecraft_domain_CharacterDamageResult){ .hxc_character = hxc_original, .hxc_damageApplied = 0, .hxc_defeated = false, .hxc_resolved = false };
  }
  struct hxc_caxecraft_domain_Character hxc_tmp_record_field_character_n7 = hxc_replacement;
  int32_t hxc_tmp_record_field_load_result_n14 = hxc_original.hxc_vitals.hxc_health;
  int32_t hxc_tmp_record_field_damageApplied_n8 = hxc_i32_subtract_wrapping(hxc_tmp_record_field_load_result_n14, hxc_replacement.hxc_vitals.hxc_health);
  int32_t hxc_tmp_record_field_load_result_n16 = hxc_original.hxc_vitals.hxc_health;
  bool hxc_tmp_short_circuit_result_n9 = hxc_tmp_record_field_load_result_n16 > 0;
  if (hxc_tmp_record_field_load_result_n16 > 0)
  {
    hxc_tmp_short_circuit_result_n9 = hxc_replacement.hxc_vitals.hxc_health <= 0;
  }
  bool hxc_tmp_short_circuit_load_result_n18 = hxc_tmp_short_circuit_result_n9;
  struct hxc_caxecraft_domain_Character hxc_tmp_record_field_character_load_result_n19 = hxc_tmp_record_field_character_n7;
  return (struct hxc_caxecraft_domain_CharacterDamageResult){ .hxc_character = hxc_tmp_record_field_character_load_result_n19, .hxc_damageApplied = hxc_tmp_record_field_damageApplied_n8, .hxc_defeated = hxc_tmp_short_circuit_load_result_n18, .hxc_resolved = true };
}

struct hxc_caxecraft_gameplay_MiningResult hxc_caxecraft_domain_GameSession_mineTerrain(struct hxc_caxecraft_domain_GameSession *hxc_self, struct hxc_caxecraft_scenario_VoxelPoint hxc_coord, struct hxc_caxecraft_gameplay_InventoryState hxc_inventory)
{
  if (hxc_self == NULL)
  {
    abort();
  }
  uint8_t *hxc_cells = (*hxc_self).hxc_worldStorage;
  size_t hxc_tmp_length_n3 = 16384;
  struct hxc_caxecraft_gameplay_MiningResult hxc_tmp_call_result_n0 = hxc_caxecraft_gameplay_Mining_attempt(hxc_cells, hxc_tmp_length_n3, hxc_coord, hxc_inventory);
  struct hxc_caxecraft_gameplay_MiningResult hxc_result = hxc_tmp_call_result_n0;
  if (hxc_result.hxc_outcome == 2)
  {
    struct hxc_caxecraft_domain_WaterSimulation *hxc_tmp_owned_class_field_address_n2 = &(*hxc_self).hxc_water;
    if (hxc_tmp_owned_class_field_address_n2 == NULL)
    {
      abort();
    }
    hxc_caxecraft_domain_WaterSimulation_terrainChanged(hxc_tmp_owned_class_field_address_n2, hxc_coord);
  }
  return hxc_result;
}

bool hxc_caxecraft_domain_GameSession_placeInitialWaterVolume(struct hxc_caxecraft_domain_GameSession *hxc_self, struct hxc_caxecraft_scenario_VoxelPoint hxc_origin, int32_t hxc_width, int32_t hxc_height, int32_t hxc_depth)
{
  if (hxc_self == NULL)
  {
    abort();
  }
  uint8_t *hxc_cells = (*hxc_self).hxc_worldStorage;
  size_t hxc_tmp_length_n5 = 16384;
  struct hxc_caxecraft_domain_WaterSimulation *hxc_tmp_owned_class_field_address_n0 = &(*hxc_self).hxc_water;
  if (hxc_tmp_owned_class_field_address_n0 == NULL)
  {
    abort();
  }
  bool hxc_tmp_instance_call_result_n1 = hxc_caxecraft_domain_WaterSimulation_placeInitialVolume(hxc_tmp_owned_class_field_address_n0, hxc_cells, hxc_tmp_length_n5, hxc_origin, hxc_width, hxc_height, hxc_depth);
  return hxc_tmp_instance_call_result_n1;
}

bool hxc_caxecraft_domain_GameSession_placeTerrain(struct hxc_caxecraft_domain_GameSession *hxc_self, struct hxc_caxecraft_scenario_VoxelPoint hxc_coord, enum hxc_caxecraft_domain_BlockKind hxc_kind)
{
  if (hxc_self == NULL)
  {
    abort();
  }
  uint8_t *hxc_cells = (*hxc_self).hxc_worldStorage;
  size_t hxc_tmp_length_n3 = 16384;
  struct hxc_caxecraft_domain_WaterSimulation *hxc_tmp_owned_class_field_address_n0 = &(*hxc_self).hxc_water;
  if (hxc_tmp_owned_class_field_address_n0 == NULL)
  {
    abort();
  }
  bool hxc_tmp_instance_call_result_n1 = hxc_caxecraft_domain_WaterSimulation_placeTerrain(hxc_tmp_owned_class_field_address_n0, hxc_cells, hxc_tmp_length_n3, hxc_coord, hxc_kind);
  return hxc_tmp_instance_call_result_n1;
}

bool hxc_caxecraft_domain_GameSession_placeWaterSource(struct hxc_caxecraft_domain_GameSession *hxc_self, struct hxc_caxecraft_scenario_VoxelPoint hxc_coord)
{
  if (hxc_self == NULL)
  {
    abort();
  }
  uint8_t *hxc_cells = (*hxc_self).hxc_worldStorage;
  size_t hxc_tmp_length_n2 = 16384;
  struct hxc_caxecraft_domain_WaterSimulation *hxc_tmp_owned_class_field_address_n0 = &(*hxc_self).hxc_water;
  if (hxc_tmp_owned_class_field_address_n0 == NULL)
  {
    abort();
  }
  bool hxc_tmp_instance_call_result_n1 = hxc_caxecraft_domain_WaterSimulation_placeSource(hxc_tmp_owned_class_field_address_n0, hxc_cells, hxc_tmp_length_n2, hxc_coord);
  return hxc_tmp_instance_call_result_n1;
}

struct hxc_caxecraft_domain_Character hxc_caxecraft_domain_GameSession_readCharacter(struct hxc_caxecraft_domain_GameSession *hxc_self, int32_t hxc_id)
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
  struct hxc_caxecraft_domain_Character hxc_tmp_instance_call_result_n1 = hxc_caxecraft_domain_EntityStore_read(hxc_tmp_owned_class_field_address_n0, hxc_id);
  return hxc_tmp_instance_call_result_n1;
}

struct hxc_caxecraft_domain_LocalCharacterCommandResult hxc_caxecraft_domain_GameSession_receiveLocalPlayerAttack(struct hxc_caxecraft_domain_GameSession *hxc_self)
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
  struct hxc_caxecraft_domain_Character hxc_original = hxc_tmp_instance_call_result_n3;
  if (!(hxc_original.hxc_id <= 0))
  {
    struct hxc_caxecraft_domain_Character hxc_tmp_load_result_n7 = hxc_original;
    struct hxc_caxecraft_domain_Character hxc_tmp_call_result_n9 = hxc_caxecraft_domain_Character_applyAttack(hxc_original, true);
    struct hxc_caxecraft_domain_LocalCharacterCommandResult hxc_tmp_instance_call_result_n10 = hxc_caxecraft_domain_GameSession_commitLocalCharacter(hxc_self, hxc_tmp_load_result_n7, hxc_tmp_call_result_n9);
    return hxc_tmp_instance_call_result_n10;
  }
  return (struct hxc_caxecraft_domain_LocalCharacterCommandResult){ .hxc_character = hxc_original, .hxc_resolved = false };
}

bool hxc_caxecraft_domain_GameSession_removeTerrain(struct hxc_caxecraft_domain_GameSession *hxc_self, struct hxc_caxecraft_scenario_VoxelPoint hxc_coord)
{
  if (hxc_self == NULL)
  {
    abort();
  }
  uint8_t *hxc_cells = (*hxc_self).hxc_worldStorage;
  size_t hxc_tmp_length_n2 = 16384;
  bool hxc_tmp_call_result_n0 = hxc_caxecraft_domain_World_remove(hxc_cells, hxc_tmp_length_n2, hxc_coord);
  if (!!hxc_tmp_call_result_n0)
  {
    struct hxc_caxecraft_domain_WaterSimulation *hxc_tmp_owned_class_field_address_n1 = &(*hxc_self).hxc_water;
    if (hxc_tmp_owned_class_field_address_n1 == NULL)
    {
      abort();
    }
    hxc_caxecraft_domain_WaterSimulation_terrainChanged(hxc_tmp_owned_class_field_address_n1, hxc_coord);
    return true;
  }
  return false;
}

bool hxc_caxecraft_domain_GameSession_replaceAuthoredActors(struct hxc_caxecraft_domain_GameSession *hxc_self, struct hxc_array_ref *hxc_characters, struct hxc_array_ref *hxc_controllers)
{
  struct hxc_caxecraft_domain_ActorControllerBinding hxc_tmp_array_element_owner_n11 = { 0 };
  struct hxc_caxecraft_domain_ActorControllerBinding hxc_tmp_array_element_owner_n12 = { 0 };
  struct hxc_caxecraft_domain_ActorControllerState hxc_tmp_array_push_element_owner_n13 = { 0 };
  struct hxc_caxecraft_domain_ActorControllerState hxc_state = { 0 };
  struct hxc_caxecraft_domain_ActorControllerState hxc_tmp_array_element_owner_n17 = { 0 };
  struct hxc_array_ref *hxc_ownedCharacters = { 0 };
  struct hxc_array_ref *hxc_ownedControllers = { 0 };
  struct hxc_array_ref *hxc_ownedStates = { 0 };
  if (hxc_self == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_domain_PlayerAgent hxc_tmp_class_field_load_result_n0 = (*hxc_self).hxc_localPlayer;
  bool hxc_tmp_short_circuit_result_n3 = hxc_tmp_class_field_load_result_n0.hxc_characterId > 0;
  if (hxc_tmp_class_field_load_result_n0.hxc_characterId > 0)
  {
    struct hxc_caxecraft_domain_EntityStore *hxc_tmp_owned_class_field_address_n2 = &(*hxc_self).hxc_entities;
    if (hxc_tmp_owned_class_field_address_n2 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_domain_PlayerAgent hxc_tmp_class_field_load_result_n3 = (*hxc_self).hxc_localPlayer;
    bool hxc_tmp_instance_call_result_n5 = hxc_caxecraft_domain_EntityStore_contains(hxc_tmp_owned_class_field_address_n2, hxc_tmp_class_field_load_result_n3.hxc_characterId);
    hxc_tmp_short_circuit_result_n3 = hxc_tmp_instance_call_result_n5;
  }
  bool hxc_tmp_short_circuit_load_result_n6 = hxc_tmp_short_circuit_result_n3;
  bool hxc_tmp_short_circuit_result_n4 = !hxc_tmp_short_circuit_load_result_n6;
  if (!!hxc_tmp_short_circuit_load_result_n6)
  {
    int32_t hxc_tmp_array_length_result_n7;
    if (hxc_array_ref_length(hxc_characters, &hxc_tmp_array_length_result_n7) != HXC_STATUS_OK)
    {
      abort();
    }
    int32_t hxc_tmp_array_length_result_n8;
    if (hxc_array_ref_length(hxc_controllers, &hxc_tmp_array_length_result_n8) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_tmp_short_circuit_result_n4 = hxc_tmp_array_length_result_n7 != hxc_tmp_array_length_result_n8;
  }
  if (hxc_tmp_short_circuit_result_n4)
  {
    return false;
  }
  struct hxc_array_ref *hxc_tmp_array_copy_result_n10 = NULL;
  if (hxc_array_ref_copy(hxc_characters, &hxc_tmp_array_copy_result_n10) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_ownedCharacters = hxc_tmp_array_copy_result_n10;
  struct hxc_array_ref *hxc_tmp_array_copy_result_n11 = NULL;
  if (hxc_array_ref_copy(hxc_controllers, &hxc_tmp_array_copy_result_n11) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_ownedControllers = hxc_tmp_array_copy_result_n11;
  struct hxc_array_ref *hxc_tmp_array_create_result_n12 = NULL;
  if (hxc_array_ref_create(hxc_default_allocator(), (hxc_array_element_ops){ sizeof(struct hxc_caxecraft_domain_ActorControllerState), _Alignof(struct hxc_caxecraft_domain_ActorControllerState), NULL, hxc_array_3767ae6c_element_copy, hxc_array_3767ae6c_element_assign, hxc_array_3767ae6c_element_destroy }, &hxc_tmp_array_create_result_n12) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_ownedStates = hxc_tmp_array_create_result_n12;
  int32_t hxc_g_h88094f548a88 = 0;
  int32_t hxc_tmp_array_length_result_n14;
  if (hxc_array_ref_length(hxc_ownedCharacters, &hxc_tmp_array_length_result_n14) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_g1 = hxc_tmp_array_length_result_n14;
  while (1)
  {
    int32_t hxc_tmp_load_result_n15 = hxc_g_h88094f548a88;
    if (!(hxc_tmp_load_result_n15 < hxc_g1))
    {
      break;
    }
    int32_t hxc_tmp_increment_load_result_n17 = hxc_g_h88094f548a88;
    hxc_g_h88094f548a88 = hxc_i32_add_wrapping(hxc_tmp_increment_load_result_n17, 1);
    int32_t hxc_index = hxc_tmp_increment_load_result_n17;
    struct hxc_array_ref *hxc_tmp_load_result_n18 = hxc_ownedControllers;
    struct hxc_caxecraft_domain_ActorControllerBinding hxc_tmp_array_get_result_n20;
    if (hxc_array_ref_get_copy(hxc_tmp_load_result_n18, (size_t)hxc_index, &hxc_tmp_array_get_result_n20) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_tmp_array_element_owner_n11 = hxc_tmp_array_get_result_n20;
    struct hxc_caxecraft_domain_ActorControllerBinding hxc_tmp_array_element_borrow_result_n21 = hxc_tmp_array_element_owner_n11;
    struct hxc_array_ref *hxc_tmp_load_result_n23 = hxc_ownedCharacters;
    struct hxc_caxecraft_domain_Character hxc_tmp_array_get_result_n25;
    if (hxc_array_ref_get_copy(hxc_tmp_load_result_n23, (size_t)hxc_index, &hxc_tmp_array_get_result_n25) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_array_622f12db_element_destroy(NULL, &hxc_tmp_array_element_owner_n11);
    if (hxc_tmp_array_element_borrow_result_n21.hxc_characterId != hxc_tmp_array_get_result_n25.hxc_id)
    {
      if (hxc_array_ref_release(hxc_ownedStates) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_ownedControllers) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_ownedCharacters) != HXC_STATUS_OK)
      {
        abort();
      }
      return false;
    }
    struct hxc_array_ref *hxc_tmp_load_result_n27 = hxc_ownedStates;
    struct hxc_array_ref *hxc_tmp_load_result_n28 = hxc_ownedControllers;
    struct hxc_caxecraft_domain_ActorControllerBinding hxc_tmp_array_get_result_n30;
    if (hxc_array_ref_get_copy(hxc_tmp_load_result_n28, (size_t)hxc_index, &hxc_tmp_array_get_result_n30) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_tmp_array_element_owner_n12 = hxc_tmp_array_get_result_n30;
    struct hxc_caxecraft_domain_ActorControllerBinding hxc_tmp_array_element_borrow_result_n31 = hxc_tmp_array_element_owner_n12;
    struct hxc_array_ref *hxc_tmp_load_result_n32 = hxc_ownedCharacters;
    struct hxc_caxecraft_domain_Character hxc_tmp_array_get_result_n34;
    if (hxc_array_ref_get_copy(hxc_tmp_load_result_n32, (size_t)hxc_index, &hxc_tmp_array_get_result_n34) != HXC_STATUS_OK)
    {
      abort();
    }
    struct hxc_caxecraft_domain_ActorControllerState hxc_tmp_call_result_n35 = hxc_caxecraft_domain_ActorControllerScheduler_startActorController(hxc_tmp_array_element_borrow_result_n31, hxc_tmp_array_get_result_n34);
    hxc_tmp_array_push_element_owner_n13 = hxc_tmp_call_result_n35;
    int32_t hxc_tmp_array_push_result_n37;
    if (hxc_array_ref_push_copy(hxc_tmp_load_result_n27, &hxc_tmp_array_push_element_owner_n13, &hxc_tmp_array_push_result_n37) != HXC_STATUS_OK)
    {
      abort();
    }
    (void)hxc_tmp_array_push_result_n37;
    hxc_record_50dff1c7_destroy(&hxc_tmp_array_push_element_owner_n13);
    hxc_array_622f12db_element_destroy(NULL, &hxc_tmp_array_element_owner_n12);
  }
  struct hxc_caxecraft_domain_EntityStore *hxc_tmp_owned_class_field_address_n38 = &(*hxc_self).hxc_entities;
  if (hxc_tmp_owned_class_field_address_n38 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_domain_PlayerAgent hxc_tmp_class_field_load_result_n39 = (*hxc_self).hxc_localPlayer;
  bool hxc_tmp_instance_call_result_n42 = hxc_caxecraft_domain_EntityStore_replaceOthers(hxc_tmp_owned_class_field_address_n38, hxc_tmp_class_field_load_result_n39.hxc_characterId, hxc_ownedCharacters);
  if (!hxc_tmp_instance_call_result_n42)
  {
    if (hxc_array_ref_release(hxc_ownedStates) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_ownedControllers) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_ownedCharacters) != HXC_STATUS_OK)
    {
      abort();
    }
    return false;
  }
  while (1)
  {
    struct hxc_array_ref *hxc_tmp_class_field_load_result_n43 = (*hxc_self).hxc_actorControllers;
    int32_t hxc_tmp_array_length_result_n44;
    if (hxc_array_ref_length(hxc_tmp_class_field_load_result_n43, &hxc_tmp_array_length_result_n44) != HXC_STATUS_OK)
    {
      abort();
    }
    if (!(hxc_tmp_array_length_result_n44 > 0))
    {
      break;
    }
    struct hxc_array_ref *hxc_tmp_class_field_load_result_n45 = (*hxc_self).hxc_actorControllers;
    struct hxc_optional_caxecraft_domain_ActorControllerState hxc_tmp_array_pop_result_n46 = (struct hxc_optional_caxecraft_domain_ActorControllerState){ .hxc_has_value = false };
    if (hxc_array_ref_pop_move(hxc_tmp_class_field_load_result_n45, &hxc_tmp_array_pop_result_n46.hxc_value, &hxc_tmp_array_pop_result_n46.hxc_has_value) != HXC_STATUS_OK)
    {
      abort();
    }
    struct hxc_optional_caxecraft_domain_ActorControllerState hxc_tmp_discarded_optional_owner_n14 = hxc_tmp_array_pop_result_n46;
    (void)hxc_tmp_discarded_optional_owner_n14;
    hxc_optional_294b4763_destroy(&hxc_tmp_discarded_optional_owner_n14);
  }
  int32_t hxc_g_h8024990fba0a = 0;
  while (1)
  {
    int32_t hxc_tmp_load_result_n47 = hxc_g_h8024990fba0a;
    int32_t hxc_tmp_array_length_result_n49;
    if (hxc_array_ref_length(hxc_ownedStates, &hxc_tmp_array_length_result_n49) != HXC_STATUS_OK)
    {
      abort();
    }
    if (!(hxc_tmp_load_result_n47 < hxc_tmp_array_length_result_n49))
    {
      break;
    }
    struct hxc_array_ref *hxc_tmp_load_result_n50 = hxc_ownedStates;
    struct hxc_caxecraft_domain_ActorControllerState hxc_tmp_array_get_result_n52;
    if (hxc_array_ref_get_copy(hxc_tmp_load_result_n50, (size_t)hxc_g_h8024990fba0a, &hxc_tmp_array_get_result_n52) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_tmp_array_element_owner_n17 = hxc_tmp_array_get_result_n52;
    hxc_state = hxc_tmp_array_element_owner_n17;
    if (hxc_record_50dff1c7_retain(&hxc_state) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_g_h8024990fba0a = hxc_i32_add_wrapping(hxc_g_h8024990fba0a, 1);
    struct hxc_array_ref *hxc_tmp_class_field_load_result_n55 = (*hxc_self).hxc_actorControllers;
    int32_t hxc_tmp_array_push_result_n57;
    if (hxc_array_ref_push_copy(hxc_tmp_class_field_load_result_n55, &hxc_state, &hxc_tmp_array_push_result_n57) != HXC_STATUS_OK)
    {
      abort();
    }
    (void)hxc_tmp_array_push_result_n57;
    hxc_record_50dff1c7_destroy(&hxc_state);
    hxc_array_3767ae6c_element_destroy(NULL, &hxc_tmp_array_element_owner_n17);
  }
  hxc_caxecraft_domain_GameSession_clearActorControllerEvents(hxc_self);
  if (hxc_array_ref_release(hxc_ownedStates) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_ref_release(hxc_ownedControllers) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_ref_release(hxc_ownedCharacters) != HXC_STATUS_OK)
  {
    abort();
  }
  return true;
}

void hxc_caxecraft_domain_GameSession_resetEmptyWorld(struct hxc_caxecraft_domain_GameSession *hxc_self)
{
  if (hxc_self == NULL)
  {
    abort();
  }
  uint8_t *hxc_cells = (*hxc_self).hxc_worldStorage;
  size_t hxc_tmp_length_n1 = 16384;
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
  hxc_caxecraft_domain_WaterSimulation_resetPending(hxc_tmp_owned_class_field_address_n3);
  return;
}

struct hxc_caxecraft_domain_LocalCharacterCommandResult hxc_caxecraft_domain_GameSession_reviveLocalPlayerAt(struct hxc_caxecraft_domain_GameSession *hxc_self, struct hxc_caxecraft_domain_CharacterBody hxc_body)
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
  struct hxc_caxecraft_domain_Character hxc_original = hxc_tmp_instance_call_result_n3;
  if (!(hxc_original.hxc_id <= 0))
  {
    struct hxc_caxecraft_domain_Character hxc_tmp_load_result_n7 = hxc_original;
    struct hxc_caxecraft_domain_Character hxc_tmp_call_result_n9 = hxc_caxecraft_domain_Character_reviveAt(hxc_original, hxc_body);
    struct hxc_caxecraft_domain_LocalCharacterCommandResult hxc_tmp_instance_call_result_n10 = hxc_caxecraft_domain_GameSession_commitLocalCharacter(hxc_self, hxc_tmp_load_result_n7, hxc_tmp_call_result_n9);
    return hxc_tmp_instance_call_result_n10;
  }
  return (struct hxc_caxecraft_domain_LocalCharacterCommandResult){ .hxc_character = hxc_original, .hxc_resolved = false };
}

struct hxc_caxecraft_domain_ActorControllerTickResult hxc_caxecraft_domain_GameSession_stepAuthoredActorControllers(struct hxc_caxecraft_domain_GameSession *hxc_self, int32_t hxc_tickNumber, int32_t hxc_damagePolicy)
{
  struct hxc_caxecraft_domain_ActorControllerDecision hxc_decision = { 0 };
  struct hxc_caxecraft_domain_ActorControllerState hxc_next = { 0 };
  struct hxc_caxecraft_domain_ActorControllerEvent hxc_event = { 0 };
  enum hxc_caxecraft_domain_ActorControllerPlanError hxc_error = { 0 };
  struct hxc_caxecraft_domain_ActorControllerState hxc_state = { 0 };
  struct hxc_caxecraft_domain_ActorControllerState hxc_tmp_array_element_owner_n9 = { 0 };
  if (hxc_self == NULL)
  {
    abort();
  }
  hxc_caxecraft_domain_GameSession_clearActorControllerEvents(hxc_self);
  struct hxc_caxecraft_domain_EntityStore *hxc_tmp_owned_class_field_address_n0 = &(*hxc_self).hxc_entities;
  if (hxc_tmp_owned_class_field_address_n0 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_domain_PlayerAgent hxc_tmp_class_field_load_result_n1 = (*hxc_self).hxc_localPlayer;
  struct hxc_caxecraft_domain_Character hxc_tmp_instance_call_result_n3 = hxc_caxecraft_domain_EntityStore_read(hxc_tmp_owned_class_field_address_n0, hxc_tmp_class_field_load_result_n1.hxc_characterId);
  struct hxc_caxecraft_domain_Character hxc_observedLocalPlayer = hxc_tmp_instance_call_result_n3;
  int32_t hxc_processed = 0;
  int32_t hxc_g = 0;
  struct hxc_array_ref *hxc_tmp_class_field_load_result_n4 = (*hxc_self).hxc_actorControllers;
  int32_t hxc_tmp_array_length_result_n5;
  if (hxc_array_ref_length(hxc_tmp_class_field_load_result_n4, &hxc_tmp_array_length_result_n5) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_g1 = hxc_tmp_array_length_result_n5;
  while (1)
  {
    int32_t hxc_tmp_load_result_n6 = hxc_g;
    if (!(hxc_tmp_load_result_n6 < hxc_g1))
    {
      break;
    }
    int32_t hxc_tmp_increment_load_result_n8 = hxc_g;
    hxc_g = hxc_i32_add_wrapping(hxc_tmp_increment_load_result_n8, 1);
    int32_t hxc_index = hxc_tmp_increment_load_result_n8;
    struct hxc_array_ref *hxc_tmp_class_field_load_result_n9 = (*hxc_self).hxc_actorControllers;
    struct hxc_caxecraft_domain_ActorControllerState hxc_tmp_array_get_result_n11;
    if (hxc_array_ref_get_copy(hxc_tmp_class_field_load_result_n9, (size_t)hxc_index, &hxc_tmp_array_get_result_n11) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_tmp_array_element_owner_n9 = hxc_tmp_array_get_result_n11;
    hxc_state = hxc_tmp_array_element_owner_n9;
    if (hxc_record_50dff1c7_retain(&hxc_state) != HXC_STATUS_OK)
    {
      abort();
    }
    struct hxc_caxecraft_domain_Character hxc_tmp_instance_call_result_n14 = hxc_caxecraft_domain_GameSession_readCharacter(hxc_self, hxc_state.hxc_characterId);
    struct hxc_caxecraft_domain_Character hxc_character = hxc_tmp_instance_call_result_n14;
    if (hxc_character.hxc_id <= 0)
    {
      int32_t hxc_tmp_record_field_load_result_n16 = hxc_state.hxc_characterId;
      struct hxc_caxecraft_domain_ActorControllerTickResult hxc_tmp_instance_call_result_n19 = hxc_caxecraft_domain_GameSession_actorControllerTick(hxc_self, (struct hxc_caxecraft_domain_ActorControllerTickStatus){ .hxc_tag = hxc_caxecraft_domain_ActorControllerTickStatus_ControlledCharacterMissing, .hxc_payload.hxc_ControlledCharacterMissing.hxc_characterId = hxc_tmp_record_field_load_result_n16 }, hxc_processed);
      hxc_record_50dff1c7_destroy(&hxc_state);
      hxc_array_3767ae6c_element_destroy(NULL, &hxc_tmp_array_element_owner_n9);
      return hxc_tmp_instance_call_result_n19;
    }
    struct hxc_caxecraft_domain_ActorControllerState hxc_tmp_load_result_n20 = hxc_state;
    struct hxc_caxecraft_domain_Character hxc_tmp_load_result_n21 = hxc_character;
    struct hxc_caxecraft_domain_ActorControllerDecision hxc_tmp_call_result_n23 = hxc_caxecraft_domain_ActorControllerScheduler_planActorController(hxc_tmp_load_result_n20, hxc_tmp_load_result_n21, hxc_observedLocalPlayer, hxc_tickNumber);
    hxc_decision = hxc_tmp_call_result_n23;
    switch (hxc_decision.hxc_tag) {
      case hxc_caxecraft_domain_ActorControllerDecision_ControllerPlanned:
        {
          break;
        }
      case hxc_caxecraft_domain_ActorControllerDecision_ControllerPlanRejected:
        {
          if (hxc_decision.hxc_tag != hxc_caxecraft_domain_ActorControllerDecision_ControllerPlanRejected)
          {
            abort();
          }
          enum hxc_caxecraft_domain_ActorControllerPlanError hxc_tmp_enum_payload_project_n64 = hxc_decision.hxc_payload.hxc_ControllerPlanRejected.hxc_error;
          hxc_error = hxc_tmp_enum_payload_project_n64;
          int32_t hxc_tmp_record_field_load_result_n65 = hxc_state.hxc_characterId;
          enum hxc_caxecraft_domain_ActorControllerPlanError hxc_tmp_load_result_n66 = hxc_error;
          struct hxc_caxecraft_domain_ActorControllerTickResult hxc_tmp_instance_call_result_n69 = hxc_caxecraft_domain_GameSession_actorControllerTick(hxc_self, (struct hxc_caxecraft_domain_ActorControllerTickStatus){ .hxc_tag = hxc_caxecraft_domain_ActorControllerTickStatus_ControllerModelRejected, .hxc_payload.hxc_ControllerModelRejected.hxc_characterId = hxc_tmp_record_field_load_result_n65, .hxc_payload.hxc_ControllerModelRejected.hxc_error = hxc_tmp_load_result_n66 }, hxc_processed);
          hxc_enum_8199493a_destroy(&hxc_decision);
          hxc_record_50dff1c7_destroy(&hxc_state);
          hxc_array_3767ae6c_element_destroy(NULL, &hxc_tmp_array_element_owner_n9);
          return hxc_tmp_instance_call_result_n69;
        }
    }
    if (hxc_decision.hxc_tag != hxc_caxecraft_domain_ActorControllerDecision_ControllerPlanned)
    {
      abort();
    }
    struct hxc_caxecraft_domain_ActorControllerState hxc_tmp_enum_payload_project_n26 = hxc_decision.hxc_payload.hxc_ControllerPlanned.hxc_next;
    hxc_next = hxc_tmp_enum_payload_project_n26;
    if (hxc_record_50dff1c7_retain(&hxc_next) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_decision.hxc_tag != hxc_caxecraft_domain_ActorControllerDecision_ControllerPlanned)
    {
      abort();
    }
    struct hxc_caxecraft_domain_CharacterIntent hxc_tmp_enum_payload_project_n28 = hxc_decision.hxc_payload.hxc_ControllerPlanned.hxc_intent;
    struct hxc_caxecraft_domain_CharacterIntent hxc_intent = hxc_tmp_enum_payload_project_n28;
    if (hxc_decision.hxc_tag != hxc_caxecraft_domain_ActorControllerDecision_ControllerPlanned)
    {
      abort();
    }
    struct hxc_caxecraft_domain_ActorControllerEvent hxc_tmp_enum_payload_project_n30 = hxc_decision.hxc_payload.hxc_ControllerPlanned.hxc_event;
    hxc_event = hxc_tmp_enum_payload_project_n30;
    if (hxc_enum_8b2a26b8_retain(&hxc_event) != HXC_STATUS_OK)
    {
      abort();
    }
    switch (hxc_event.hxc_tag) {
      case hxc_caxecraft_domain_ActorControllerEvent_NoControllerEvent:
        {
          break;
        }
      case hxc_caxecraft_domain_ActorControllerEvent_InteractionAvailable:
        {
          break;
        }
      case hxc_caxecraft_domain_ActorControllerEvent_LocalPlayerAttack:
        {
          if (hxc_event.hxc_tag != hxc_caxecraft_domain_ActorControllerEvent_LocalPlayerAttack)
          {
            abort();
          }
          int32_t hxc_tmp_enum_payload_project_n33 = hxc_event.hxc_payload.hxc_LocalPlayerAttack.hxc_characterId;
          int32_t hxc_source = hxc_tmp_enum_payload_project_n33;
          struct hxc_caxecraft_domain_LocalCharacterCommandResult hxc_tmp_instance_call_result_n34 = hxc_caxecraft_domain_GameSession_receiveLocalPlayerAttack(hxc_self);
          struct hxc_caxecraft_domain_LocalCharacterCommandResult hxc_attack = hxc_tmp_instance_call_result_n34;
          if (!hxc_attack.hxc_resolved)
          {
            int32_t hxc_tmp_load_result_n36 = hxc_source;
            struct hxc_caxecraft_domain_ActorControllerTickResult hxc_tmp_instance_call_result_n39 = hxc_caxecraft_domain_GameSession_actorControllerTick(hxc_self, (struct hxc_caxecraft_domain_ActorControllerTickStatus){ .hxc_tag = hxc_caxecraft_domain_ActorControllerTickStatus_LocalAttackCommandRejected, .hxc_payload.hxc_LocalAttackCommandRejected.hxc_characterId = hxc_tmp_load_result_n36 }, hxc_processed);
            hxc_enum_8b2a26b8_destroy(&hxc_event);
            hxc_record_50dff1c7_destroy(&hxc_next);
            hxc_enum_8199493a_destroy(&hxc_decision);
            hxc_record_50dff1c7_destroy(&hxc_state);
            hxc_array_3767ae6c_element_destroy(NULL, &hxc_tmp_array_element_owner_n9);
            return hxc_tmp_instance_call_result_n39;
          }
          break;
        }
      case hxc_caxecraft_domain_ActorControllerEvent_DropRequested:
        {
          break;
        }
    }
    int32_t hxc_tmp_record_field_load_result_n40 = hxc_state.hxc_characterId;
    struct hxc_caxecraft_domain_CharacterCommandStepResult hxc_tmp_instance_call_result_n42 = hxc_caxecraft_domain_GameSession_stepCharacter(hxc_self, hxc_tmp_record_field_load_result_n40, hxc_intent, hxc_damagePolicy);
    struct hxc_caxecraft_domain_CharacterCommandStepResult hxc_step = hxc_tmp_instance_call_result_n42;
    if (!hxc_step.hxc_resolved)
    {
      int32_t hxc_tmp_record_field_load_result_n44 = hxc_state.hxc_characterId;
      struct hxc_caxecraft_domain_ActorControllerTickResult hxc_tmp_instance_call_result_n47 = hxc_caxecraft_domain_GameSession_actorControllerTick(hxc_self, (struct hxc_caxecraft_domain_ActorControllerTickStatus){ .hxc_tag = hxc_caxecraft_domain_ActorControllerTickStatus_CharacterCommandRejected, .hxc_payload.hxc_CharacterCommandRejected.hxc_characterId = hxc_tmp_record_field_load_result_n44 }, hxc_processed);
      hxc_enum_8b2a26b8_destroy(&hxc_event);
      hxc_record_50dff1c7_destroy(&hxc_next);
      hxc_enum_8199493a_destroy(&hxc_decision);
      hxc_record_50dff1c7_destroy(&hxc_state);
      hxc_array_3767ae6c_element_destroy(NULL, &hxc_tmp_array_element_owner_n9);
      return hxc_tmp_instance_call_result_n47;
    }
    struct hxc_array_ref *hxc_tmp_class_field_load_result_n48 = (*hxc_self).hxc_actorControllers;
    int32_t hxc_tmp_load_result_n49 = hxc_index;
    if (hxc_array_ref_set_copy(hxc_tmp_class_field_load_result_n48, (size_t)hxc_tmp_load_result_n49, &hxc_next) != HXC_STATUS_OK)
    {
      abort();
    }
    (void)hxc_next;
    switch (hxc_event.hxc_tag) {
      case hxc_caxecraft_domain_ActorControllerEvent_NoControllerEvent:
        {
          break;
        }
      case hxc_caxecraft_domain_ActorControllerEvent_InteractionAvailable:
        {
          struct hxc_array_ref *hxc_tmp_class_field_load_result_n53 = (*hxc_self).hxc_actorControllerEvents;
          int32_t hxc_tmp_array_push_result_n55;
          if (hxc_array_ref_push_copy(hxc_tmp_class_field_load_result_n53, &hxc_event, &hxc_tmp_array_push_result_n55) != HXC_STATUS_OK)
          {
            abort();
          }
          (void)hxc_tmp_array_push_result_n55;
          break;
        }
      case hxc_caxecraft_domain_ActorControllerEvent_LocalPlayerAttack:
        {
          struct hxc_array_ref *hxc_tmp_class_field_load_result_n56 = (*hxc_self).hxc_actorControllerEvents;
          int32_t hxc_tmp_array_push_result_n58;
          if (hxc_array_ref_push_copy(hxc_tmp_class_field_load_result_n56, &hxc_event, &hxc_tmp_array_push_result_n58) != HXC_STATUS_OK)
          {
            abort();
          }
          (void)hxc_tmp_array_push_result_n58;
          break;
        }
      case hxc_caxecraft_domain_ActorControllerEvent_DropRequested:
        {
          struct hxc_array_ref *hxc_tmp_class_field_load_result_n59 = (*hxc_self).hxc_actorControllerEvents;
          int32_t hxc_tmp_array_push_result_n61;
          if (hxc_array_ref_push_copy(hxc_tmp_class_field_load_result_n59, &hxc_event, &hxc_tmp_array_push_result_n61) != HXC_STATUS_OK)
          {
            abort();
          }
          (void)hxc_tmp_array_push_result_n61;
          break;
        }
    }
    hxc_processed = hxc_i32_add_wrapping(hxc_processed, 1);
    hxc_enum_8b2a26b8_destroy(&hxc_event);
    hxc_record_50dff1c7_destroy(&hxc_next);
    hxc_enum_8199493a_destroy(&hxc_decision);
    hxc_record_50dff1c7_destroy(&hxc_state);
    hxc_array_3767ae6c_element_destroy(NULL, &hxc_tmp_array_element_owner_n9);
  }
  struct hxc_caxecraft_domain_ActorControllerTickResult hxc_tmp_instance_call_result_n72 = hxc_caxecraft_domain_GameSession_actorControllerTick(hxc_self, (struct hxc_caxecraft_domain_ActorControllerTickStatus){ .hxc_tag = hxc_caxecraft_domain_ActorControllerTickStatus_ControllersAdvanced }, hxc_processed);
  return hxc_tmp_instance_call_result_n72;
}

struct hxc_caxecraft_domain_CharacterCommandStepResult hxc_caxecraft_domain_GameSession_stepCharacter(struct hxc_caxecraft_domain_GameSession *hxc_self, int32_t hxc_id, struct hxc_caxecraft_domain_CharacterIntent hxc_intent, int32_t hxc_damagePolicy)
{
  if (hxc_self == NULL)
  {
    abort();
  }
  const uint8_t *hxc_readCells = (*hxc_self).hxc_worldStorage;
  size_t hxc_tmp_length_n4 = 16384;
  struct hxc_caxecraft_domain_EntityStore *hxc_tmp_owned_class_field_address_n0 = &(*hxc_self).hxc_entities;
  if (hxc_tmp_owned_class_field_address_n0 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_domain_Character hxc_tmp_instance_call_result_n1 = hxc_caxecraft_domain_EntityStore_read(hxc_tmp_owned_class_field_address_n0, hxc_id);
  struct hxc_caxecraft_domain_CharacterCommandStepResult hxc_tmp_instance_call_result_n2 = hxc_caxecraft_domain_GameSession_stepStoredCharacter(hxc_self, hxc_readCells, hxc_tmp_length_n4, hxc_tmp_instance_call_result_n1, hxc_intent, hxc_damagePolicy);
  return hxc_tmp_instance_call_result_n2;
}

struct hxc_caxecraft_domain_CharacterCommandStepResult hxc_caxecraft_domain_GameSession_stepStoredCharacter(struct hxc_caxecraft_domain_GameSession *hxc_self, const uint8_t *hxc_readCells, size_t hxc_length, struct hxc_caxecraft_domain_Character hxc_original, struct hxc_caxecraft_domain_CharacterIntent hxc_intent, int32_t hxc_damagePolicy)
{
  const uint8_t *hxc_borrow = hxc_readCells;
  size_t hxc_tmp_length_n5 = hxc_length;
  (void)hxc_borrow;
  (void)hxc_tmp_length_n5;
  if (hxc_original.hxc_id <= 0)
  {
    struct hxc_caxecraft_domain_Immersion hxc_tmp_call_result_n2 = hxc_caxecraft_domain_Aquatics_observe(hxc_readCells, hxc_length, hxc_original.hxc_body);
    return (struct hxc_caxecraft_domain_CharacterCommandStepResult){ .hxc_character = hxc_original, .hxc_drowningDamage = 0, .hxc_immersion = hxc_tmp_call_result_n2, .hxc_resolved = false };
  }
  struct hxc_caxecraft_domain_CharacterStep hxc_tmp_call_result_n4 = hxc_caxecraft_domain_Character_step(hxc_readCells, hxc_length, hxc_original, hxc_intent, hxc_damagePolicy);
  struct hxc_caxecraft_domain_CharacterStep hxc_result = hxc_tmp_call_result_n4;
  if (hxc_self == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_domain_EntityStore *hxc_tmp_owned_class_field_address_n5 = &(*hxc_self).hxc_entities;
  if (hxc_tmp_owned_class_field_address_n5 == NULL)
  {
    abort();
  }
  bool hxc_tmp_instance_call_result_n8 = hxc_caxecraft_domain_EntityStore_replace(hxc_tmp_owned_class_field_address_n5, hxc_original.hxc_id, hxc_result.hxc_character);
  bool hxc_resolved = hxc_tmp_instance_call_result_n8;
  bool hxc_tmp_load_result_n9 = hxc_resolved;
  struct hxc_caxecraft_domain_Character hxc_tmp_conditional_result_n8;
  if (hxc_tmp_load_result_n9)
  {
    hxc_tmp_conditional_result_n8 = hxc_result.hxc_character;
  }
  else
  {
    hxc_tmp_conditional_result_n8 = hxc_original;
  }
  struct hxc_caxecraft_domain_Character hxc_tmp_conditional_load_result_n11 = hxc_tmp_conditional_result_n8;
  struct hxc_caxecraft_domain_Immersion hxc_tmp_record_field_load_result_n12 = hxc_result.hxc_immersion;
  int32_t hxc_tmp_record_field_load_result_n13 = hxc_result.hxc_drowningDamage;
  return (struct hxc_caxecraft_domain_CharacterCommandStepResult){ .hxc_character = hxc_tmp_conditional_load_result_n11, .hxc_drowningDamage = hxc_tmp_record_field_load_result_n13, .hxc_immersion = hxc_tmp_record_field_load_result_n12, .hxc_resolved = hxc_resolved };
}

struct hxc_caxecraft_domain_GameTickResult hxc_caxecraft_domain_GameSession_tick(struct hxc_caxecraft_domain_GameSession *hxc_self, struct hxc_caxecraft_domain_GameTickInput hxc_input)
{
  if (hxc_self == NULL)
  {
    abort();
  }
  uint8_t *hxc_cells = (*hxc_self).hxc_worldStorage;
  size_t hxc_tmp_length_n2 = 16384;
  const uint8_t *hxc_readCells = (*hxc_self).hxc_worldStorage;
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
  if (hxc_original.hxc_id <= 0)
  {
    struct hxc_caxecraft_domain_Character hxc_tmp_load_result_n6 = hxc_original;
    struct hxc_caxecraft_domain_Immersion hxc_tmp_call_result_n8 = hxc_caxecraft_domain_Aquatics_observe(hxc_readCells, hxc_tmp_length_n3, hxc_original.hxc_body);
    struct hxc_caxecraft_domain_WaterSimulation *hxc_tmp_owned_class_field_address_n9 = &(*hxc_self).hxc_water;
    int32_t hxc_tmp_class_field_load_result_n10 = (*hxc_tmp_owned_class_field_address_n9).hxc_pendingCount;
    return (struct hxc_caxecraft_domain_GameTickResult){ .hxc_character = hxc_tmp_load_result_n6, .hxc_committed = false, .hxc_drowningDamage = 0, .hxc_immersion = hxc_tmp_call_result_n8, .hxc_tickIndex = -1, .hxc_water = (struct hxc_caxecraft_domain_WaterTickResult){ .hxc_changed = 0, .hxc_processed = 0, .hxc_remaining = hxc_tmp_class_field_load_result_n10 } };
  }
  struct hxc_caxecraft_domain_WaterSimulation *hxc_tmp_owned_class_field_address_n13 = &(*hxc_self).hxc_water;
  if (hxc_tmp_owned_class_field_address_n13 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_domain_WaterTickResult hxc_tmp_instance_call_result_n15 = hxc_caxecraft_domain_WaterSimulation_tick(hxc_tmp_owned_class_field_address_n13, hxc_cells, hxc_tmp_length_n2, hxc_input.hxc_waterUpdateBudget);
  struct hxc_caxecraft_domain_WaterTickResult hxc_waterResult = hxc_tmp_instance_call_result_n15;
  struct hxc_caxecraft_domain_CharacterCommandStepResult hxc_tmp_instance_call_result_n19 = hxc_caxecraft_domain_GameSession_stepStoredCharacter(hxc_self, hxc_readCells, hxc_tmp_length_n3, hxc_original, hxc_input.hxc_intent, hxc_input.hxc_damagePolicy);
  struct hxc_caxecraft_domain_CharacterCommandStepResult hxc_characterResult = hxc_tmp_instance_call_result_n19;
  bool hxc_committed = hxc_characterResult.hxc_resolved;
  bool hxc_tmp_load_result_n21 = hxc_committed;
  int32_t hxc_tmp_conditional_result_n10 = 0;
  if (hxc_tmp_load_result_n21)
  {
    int32_t hxc_tmp_class_field_load_result_n22 = (*hxc_self).hxc_completedTicks;
    hxc_tmp_conditional_result_n10 = hxc_tmp_class_field_load_result_n22;
  }
  else
  {
    hxc_tmp_conditional_result_n10 = -1;
  }
  int32_t hxc_tickIndex = hxc_tmp_conditional_result_n10;
  if (hxc_committed)
  {
    int32_t hxc_tmp_increment_load_result_n25 = (*hxc_self).hxc_completedTicks;
    (*hxc_self).hxc_completedTicks = hxc_i32_add_wrapping(hxc_tmp_increment_load_result_n25, 1);
  }
  int32_t hxc_tmp_load_result_n26 = hxc_tickIndex;
  struct hxc_caxecraft_domain_Character hxc_tmp_record_field_load_result_n27 = hxc_characterResult.hxc_character;
  struct hxc_caxecraft_domain_Immersion hxc_tmp_record_field_load_result_n28 = hxc_characterResult.hxc_immersion;
  int32_t hxc_tmp_record_field_load_result_n29 = hxc_characterResult.hxc_drowningDamage;
  struct hxc_caxecraft_domain_WaterTickResult hxc_tmp_load_result_n30 = hxc_waterResult;
  return (struct hxc_caxecraft_domain_GameTickResult){ .hxc_character = hxc_tmp_record_field_load_result_n27, .hxc_committed = hxc_committed, .hxc_drowningDamage = hxc_tmp_record_field_load_result_n29, .hxc_immersion = hxc_tmp_record_field_load_result_n28, .hxc_tickIndex = hxc_tmp_load_result_n26, .hxc_water = hxc_tmp_load_result_n30 };
}

struct hxc_caxecraft_domain_LocalRecoveryResult hxc_caxecraft_domain_GameSession_useSelectedRecovery(struct hxc_caxecraft_domain_GameSession *hxc_self, struct hxc_caxecraft_gameplay_InventoryState hxc_inventory)
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
  struct hxc_caxecraft_domain_Character hxc_original = hxc_tmp_instance_call_result_n3;
  if (hxc_original.hxc_id <= 0)
  {
    return (struct hxc_caxecraft_domain_LocalRecoveryResult){ .hxc_character = hxc_original, .hxc_decision = 0, .hxc_inventory = hxc_inventory, .hxc_resolved = false };
  }
  int32_t hxc_tmp_call_result_n8 = hxc_caxecraft_gameplay_Recovery_decide(hxc_inventory, hxc_original.hxc_vitals);
  int32_t hxc_decision = hxc_tmp_call_result_n8;
  if (hxc_decision != 1)
  {
    int32_t hxc_tmp_load_result_n10 = hxc_decision;
    return (struct hxc_caxecraft_domain_LocalRecoveryResult){ .hxc_character = hxc_original, .hxc_decision = hxc_tmp_load_result_n10, .hxc_inventory = hxc_inventory, .hxc_resolved = true };
  }
  struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_call_result_n14 = hxc_caxecraft_gameplay_Recovery_applyInventory(hxc_decision, hxc_inventory);
  struct hxc_caxecraft_gameplay_InventoryState hxc_nextInventory = hxc_tmp_call_result_n14;
  struct hxc_caxecraft_domain_Character hxc_tmp_load_result_n15 = hxc_original;
  int32_t hxc_tmp_load_result_n16 = hxc_decision;
  struct hxc_caxecraft_domain_VitalsState hxc_tmp_call_result_n18 = hxc_caxecraft_gameplay_Recovery_applyVitals(hxc_tmp_load_result_n16, hxc_original.hxc_vitals);
  struct hxc_caxecraft_domain_Character hxc_tmp_call_result_n19 = hxc_caxecraft_domain_Character_withVitals(hxc_tmp_load_result_n15, hxc_tmp_call_result_n18);
  struct hxc_caxecraft_domain_Character hxc_nextCharacter = hxc_tmp_call_result_n19;
  struct hxc_caxecraft_domain_Character hxc_tmp_load_result_n20 = hxc_original;
  struct hxc_caxecraft_domain_LocalCharacterCommandResult hxc_tmp_instance_call_result_n22 = hxc_caxecraft_domain_GameSession_commitLocalCharacter(hxc_self, hxc_tmp_load_result_n20, hxc_nextCharacter);
  struct hxc_caxecraft_domain_LocalCharacterCommandResult hxc_committed = hxc_tmp_instance_call_result_n22;
  int32_t hxc_tmp_record_field_decision_n7 = hxc_decision;
  bool hxc_tmp_record_field_load_result_n24 = hxc_committed.hxc_resolved;
  struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_conditional_result_n8;
  if (hxc_tmp_record_field_load_result_n24)
  {
    hxc_tmp_conditional_result_n8 = hxc_nextInventory;
  }
  else
  {
    hxc_tmp_conditional_result_n8 = hxc_inventory;
  }
  struct hxc_caxecraft_gameplay_InventoryState hxc_tmp_conditional_load_result_n26 = hxc_tmp_conditional_result_n8;
  struct hxc_caxecraft_domain_Character hxc_tmp_record_field_load_result_n27 = hxc_committed.hxc_character;
  bool hxc_tmp_record_field_load_result_n28 = hxc_committed.hxc_resolved;
  return (struct hxc_caxecraft_domain_LocalRecoveryResult){ .hxc_character = hxc_tmp_record_field_load_result_n27, .hxc_decision = hxc_tmp_record_field_decision_n7, .hxc_inventory = hxc_tmp_conditional_load_result_n26, .hxc_resolved = hxc_tmp_record_field_load_result_n28 };
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

const uint8_t *hxc_caxecraft_domain_GameSession_worldView(struct hxc_caxecraft_domain_GameSession *hxc_self, size_t *hxc_returned_span_length)
{
  if (hxc_self == NULL)
  {
    abort();
  }
  *hxc_returned_span_length = 16384;
  return (*hxc_self).hxc_worldStorage;
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
