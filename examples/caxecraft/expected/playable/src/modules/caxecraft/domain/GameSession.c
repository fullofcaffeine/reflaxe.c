#include "hxc/program.h"

void hxc_compiler_constructor_caxecraft_domain_GameSession(struct hxc_caxecraft_domain_GameSession *hxc_l_self)
{
  const void *hxc_l_gc_roots[2] = { (const void *)hxc_l_self, NULL };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 2, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_caxecraft_domain_WaterSimulation *hxc_l_tmp_owned_class_field_address_n0 = &(*hxc_l_self).hxc_water;
  hxc_compiler_constructor_caxecraft_domain_WaterSimulation(hxc_l_tmp_owned_class_field_address_n0);
  struct hxc_array_ref *hxc_l_tmp_array_create_result_n1 = NULL;
  if (hxc_array_ref_create(hxc_default_allocator(), (hxc_array_element_ops){ sizeof(struct hxc_caxecraft_scenario_FlowEvent), _Alignof(struct hxc_caxecraft_scenario_FlowEvent), NULL, hxc_array_8721c25d_element_copy, hxc_array_8721c25d_element_assign, hxc_array_8721c25d_element_destroy }, &hxc_l_tmp_array_create_result_n1) != HXC_STATUS_OK)
  {
    abort();
  }
  (*hxc_l_self).hxc_pendingFlowEvents = hxc_l_tmp_array_create_result_n1;
  struct hxc_array_ref *hxc_l_tmp_array_create_result_n2 = NULL;
  if (hxc_array_ref_create(hxc_default_allocator(), (hxc_array_element_ops){ sizeof(hxc_string), _Alignof(hxc_string), NULL, hxc_array_4c9e169c_element_copy, hxc_array_4c9e169c_element_assign, hxc_array_4c9e169c_element_destroy }, &hxc_l_tmp_array_create_result_n2) != HXC_STATUS_OK)
  {
    abort();
  }
  (*hxc_l_self).hxc_authoredActorIds = hxc_l_tmp_array_create_result_n2;
  struct hxc_array_ref *hxc_l_tmp_array_create_result_n3 = NULL;
  if (hxc_array_ref_create_trivial(hxc_default_allocator(), sizeof(int32_t), _Alignof(int32_t), &hxc_l_tmp_array_create_result_n3) != HXC_STATUS_OK)
  {
    abort();
  }
  (*hxc_l_self).hxc_authoredActorEntities = hxc_l_tmp_array_create_result_n3;
  hxc_l_gc_roots[1] = (const void *)NULL;
  (*hxc_l_self).hxc_flowExecutor = NULL;
  (*hxc_l_self).hxc_completedTicks = 0;
  struct hxc_array_ref *hxc_l_tmp_array_create_result_n4 = NULL;
  if (hxc_array_ref_create(hxc_default_allocator(), (hxc_array_element_ops){ sizeof(struct hxc_caxecraft_domain_ActorControllerEvent), _Alignof(struct hxc_caxecraft_domain_ActorControllerEvent), NULL, hxc_array_43e9f8ca_element_copy, hxc_array_43e9f8ca_element_assign, hxc_array_43e9f8ca_element_destroy }, &hxc_l_tmp_array_create_result_n4) != HXC_STATUS_OK)
  {
    abort();
  }
  (*hxc_l_self).hxc_actorControllerEvents = hxc_l_tmp_array_create_result_n4;
  struct hxc_array_ref *hxc_l_tmp_array_create_result_n5 = NULL;
  if (hxc_array_ref_create(hxc_default_allocator(), (hxc_array_element_ops){ sizeof(struct hxc_caxecraft_domain_ActorControllerState), _Alignof(struct hxc_caxecraft_domain_ActorControllerState), NULL, hxc_array_3767ae6c_element_copy, hxc_array_3767ae6c_element_assign, hxc_array_3767ae6c_element_destroy }, &hxc_l_tmp_array_create_result_n5) != HXC_STATUS_OK)
  {
    abort();
  }
  (*hxc_l_self).hxc_actorControllers = hxc_l_tmp_array_create_result_n5;
  struct hxc_caxecraft_domain_EntityStore *hxc_l_tmp_owned_class_field_address_n6 = &(*hxc_l_self).hxc_entities;
  hxc_compiler_constructor_caxecraft_domain_EntityStore(hxc_l_tmp_owned_class_field_address_n6);
  struct hxc_caxecraft_domain_PlayerAgent hxc_l_tmp_call_result_n7 = hxc_caxecraft_domain_PlayerAgent_bind(0);
  (*hxc_l_self).hxc_localPlayer = hxc_l_tmp_call_result_n7;
  struct hxc_caxecraft_domain_WaterSimulation *hxc_l_tmp_owned_class_field_address_n8 = &(*hxc_l_self).hxc_water;
  if (hxc_l_tmp_owned_class_field_address_n8 == NULL)
  {
    abort();
  }
  hxc_caxecraft_domain_WaterSimulation_resetPending(hxc_l_tmp_owned_class_field_address_n8);
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return;
}

bool hxc_caxecraft_domain_GameSession_activateAuthoredItemDuringLoad(struct hxc_caxecraft_domain_GameSession *hxc_l_self, int32_t hxc_l_index)
{
  const void *hxc_l_gc_roots[1] = { (const void *)hxc_l_self };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 1, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  bool hxc_l_tmp_short_circuit_result_n2 = hxc_l_index < 0;
  if (!(hxc_l_index < 0))
  {
    hxc_l_tmp_short_circuit_result_n2 = hxc_l_index >= 256;
  }
  if (!hxc_l_tmp_short_circuit_result_n2)
  {
    if (hxc_l_self == NULL)
    {
      abort();
    }
    if (hxc_l_index < 0 || (size_t)hxc_l_index >= 256)
    {
      abort();
    }
    (*hxc_l_self).hxc_authoredItemStorage[(size_t)hxc_l_index] = 1;
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return true;
  }
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return false;
}

struct hxc_array_ref *hxc_caxecraft_domain_GameSession_actorControllerEventSnapshots(struct hxc_caxecraft_domain_GameSession *hxc_l_self)
{
  const void *hxc_l_gc_roots[1] = { (const void *)hxc_l_self };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 1, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_l_self == NULL)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n0 = (*hxc_l_self).hxc_actorControllerEvents;
  struct hxc_array_ref *hxc_l_tmp_array_copy_result_n1 = NULL;
  if (hxc_array_ref_copy(hxc_l_tmp_class_field_load_result_n0, &hxc_l_tmp_array_copy_result_n1) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return hxc_l_tmp_array_copy_result_n1;
}

struct hxc_array_ref *hxc_caxecraft_domain_GameSession_actorControllerStateSnapshots(struct hxc_caxecraft_domain_GameSession *hxc_l_self)
{
  const void *hxc_l_gc_roots[1] = { (const void *)hxc_l_self };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 1, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_l_self == NULL)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n0 = (*hxc_l_self).hxc_actorControllers;
  struct hxc_array_ref *hxc_l_tmp_array_copy_result_n1 = NULL;
  if (hxc_array_ref_copy(hxc_l_tmp_class_field_load_result_n0, &hxc_l_tmp_array_copy_result_n1) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return hxc_l_tmp_array_copy_result_n1;
}

struct hxc_caxecraft_domain_ActorControllerTickResult hxc_caxecraft_domain_GameSession_actorControllerTick(struct hxc_caxecraft_domain_GameSession *hxc_l_self, struct hxc_caxecraft_domain_ActorControllerTickStatus hxc_l_status, int32_t hxc_l_processed)
{
  const void *hxc_l_gc_roots[1] = { (const void *)hxc_l_self };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 1, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_l_self == NULL)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n0 = (*hxc_l_self).hxc_actorControllerEvents;
  int32_t hxc_l_tmp_array_length_result_n1;
  if (hxc_array_ref_length(hxc_l_tmp_class_field_load_result_n0, &hxc_l_tmp_array_length_result_n1) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return (struct hxc_caxecraft_domain_ActorControllerTickResult){ .hxc_emittedEvents = hxc_l_tmp_array_length_result_n1, .hxc_processed = hxc_l_processed, .hxc_status = hxc_l_status };
}

bool hxc_caxecraft_domain_GameSession_actorInteractionAvailable(struct hxc_caxecraft_domain_GameSession *hxc_l_self, int32_t hxc_l_id)
{
  const void *hxc_l_gc_roots[1] = { (const void *)hxc_l_self };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 1, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_caxecraft_domain_ActorControllerState hxc_l_controller = { 0 };
  struct hxc_caxecraft_domain_ActorControllerState hxc_l_tmp_array_element_owner_n7 = { 0 };
  if (hxc_l_self == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n0 = hxc_caxecraft_domain_GameSession_readCharacter(hxc_l_self, hxc_l_id);
  struct hxc_caxecraft_domain_Character hxc_l_actor = hxc_l_tmp_instance_call_result_n0;
  struct hxc_caxecraft_domain_EntityStore *hxc_l_tmp_owned_class_field_address_n1 = &(*hxc_l_self).hxc_entities;
  if (hxc_l_tmp_owned_class_field_address_n1 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_domain_PlayerAgent hxc_l_tmp_class_field_load_result_n2 = (*hxc_l_self).hxc_localPlayer;
  struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n4 = hxc_caxecraft_domain_EntityStore_read(hxc_l_tmp_owned_class_field_address_n1, hxc_l_tmp_class_field_load_result_n2.hxc_characterId);
  struct hxc_caxecraft_domain_Character hxc_l_player = hxc_l_tmp_instance_call_result_n4;
  int32_t hxc_l_g = 0;
  struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n5 = (*hxc_l_self).hxc_actorControllers;
  struct hxc_array_ref *hxc_l_g1 = hxc_l_tmp_class_field_load_result_n5;
  if (hxc_array_ref_retain(hxc_l_g1) != HXC_STATUS_OK)
  {
    abort();
  }
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n6 = hxc_l_g;
    int32_t hxc_l_tmp_array_length_result_n8;
    if (hxc_array_ref_length(hxc_l_g1, &hxc_l_tmp_array_length_result_n8) != HXC_STATUS_OK)
    {
      abort();
    }
    if (!(hxc_l_tmp_load_result_n6 < hxc_l_tmp_array_length_result_n8))
    {
      break;
    }
    struct hxc_array_ref *hxc_l_tmp_load_result_n9 = hxc_l_g1;
    struct hxc_caxecraft_domain_ActorControllerState hxc_l_tmp_array_get_result_n11;
    if (hxc_array_ref_get_copy(hxc_l_tmp_load_result_n9, (size_t)hxc_l_g, &hxc_l_tmp_array_get_result_n11) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_array_element_owner_n7 = hxc_l_tmp_array_get_result_n11;
    hxc_l_controller = hxc_l_tmp_array_element_owner_n7;
    if (hxc_record_50dff1c7_retain(&hxc_l_controller) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_g = hxc_i32_add_wrapping(hxc_l_g, 1);
    if (hxc_l_controller.hxc_characterId == hxc_l_id)
    {
      struct hxc_caxecraft_domain_ActorControllerState hxc_l_tmp_load_result_n15 = hxc_l_controller;
      struct hxc_caxecraft_domain_Character hxc_l_tmp_load_result_n16 = hxc_l_actor;
      bool hxc_l_tmp_call_result_n18 = hxc_caxecraft_domain_ActorControllerScheduler_interactionAvailable(hxc_l_tmp_load_result_n15, hxc_l_tmp_load_result_n16, hxc_l_player);
      hxc_record_50dff1c7_destroy(&hxc_l_controller);
      hxc_array_3767ae6c_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n7);
      if (hxc_array_ref_release(hxc_l_g1) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
      {
        abort();
      }
      return hxc_l_tmp_call_result_n18;
    }
    hxc_record_50dff1c7_destroy(&hxc_l_controller);
    hxc_array_3767ae6c_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n7);
  }
  if (hxc_array_ref_release(hxc_l_g1) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return false;
}

bool hxc_caxecraft_domain_GameSession_authoredItemIsActive(struct hxc_caxecraft_domain_GameSession *hxc_l_self, int32_t hxc_l_index)
{
  const void *hxc_l_gc_roots[1] = { (const void *)hxc_l_self };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 1, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  bool hxc_l_tmp_short_circuit_result_n2 = hxc_l_index < 0;
  if (!(hxc_l_index < 0))
  {
    hxc_l_tmp_short_circuit_result_n2 = hxc_l_index >= 256;
  }
  if (!hxc_l_tmp_short_circuit_result_n2)
  {
    if (hxc_l_self == NULL)
    {
      abort();
    }
    if (hxc_l_index < 0 || (size_t)hxc_l_index >= 256)
    {
      abort();
    }
    int32_t hxc_l_tmp_collection_index_load_result_n1 = (*hxc_l_self).hxc_authoredItemStorage[(size_t)hxc_l_index];
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return hxc_l_tmp_collection_index_load_result_n1 != 0;
  }
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return false;
}

const int32_t *hxc_caxecraft_domain_GameSession_authoredItemsView(struct hxc_caxecraft_domain_GameSession *hxc_l_self, size_t *hxc_l_returned_span_length)
{
  const void *hxc_l_gc_roots[1] = { (const void *)hxc_l_self };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 1, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_l_self == NULL)
  {
    abort();
  }
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  *hxc_l_returned_span_length = 256;
  return (*hxc_l_self).hxc_authoredItemStorage;
}

bool hxc_caxecraft_domain_GameSession_bindLocalPlayer(struct hxc_caxecraft_domain_GameSession *hxc_l_self, struct hxc_caxecraft_domain_Character hxc_l_character)
{
  const void *hxc_l_gc_roots[1] = { (const void *)hxc_l_self };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 1, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_l_self == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_domain_EntityStore *hxc_l_tmp_owned_class_field_address_n0 = &(*hxc_l_self).hxc_entities;
  if (hxc_l_tmp_owned_class_field_address_n0 == NULL)
  {
    abort();
  }
  bool hxc_l_tmp_instance_call_result_n1 = hxc_caxecraft_domain_EntityStore_put(hxc_l_tmp_owned_class_field_address_n0, hxc_l_character);
  if (!!hxc_l_tmp_instance_call_result_n1)
  {
    struct hxc_caxecraft_domain_PlayerAgent hxc_l_tmp_call_result_n3 = hxc_caxecraft_domain_PlayerAgent_bind(hxc_l_character.hxc_id);
    (*hxc_l_self).hxc_localPlayer = hxc_l_tmp_call_result_n3;
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return true;
  }
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return false;
}

void hxc_caxecraft_domain_GameSession_clearActorControllerEvents(struct hxc_caxecraft_domain_GameSession *hxc_l_self)
{
  const void *hxc_l_gc_roots[1] = { (const void *)hxc_l_self };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 1, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  while (1)
  {
    if (hxc_l_self == NULL)
    {
      abort();
    }
    struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n0 = (*hxc_l_self).hxc_actorControllerEvents;
    int32_t hxc_l_tmp_array_length_result_n1;
    if (hxc_array_ref_length(hxc_l_tmp_class_field_load_result_n0, &hxc_l_tmp_array_length_result_n1) != HXC_STATUS_OK)
    {
      abort();
    }
    if (!(hxc_l_tmp_array_length_result_n1 > 0))
    {
      break;
    }
    struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n2 = (*hxc_l_self).hxc_actorControllerEvents;
    struct hxc_optional_caxecraft_domain_ActorControllerEvent hxc_l_tmp_array_pop_result_n3 = (struct hxc_optional_caxecraft_domain_ActorControllerEvent){ .hxc_has_value = false };
    if (hxc_array_ref_pop_move(hxc_l_tmp_class_field_load_result_n2, &hxc_l_tmp_array_pop_result_n3.hxc_value, &hxc_l_tmp_array_pop_result_n3.hxc_has_value) != HXC_STATUS_OK)
    {
      abort();
    }
    struct hxc_optional_caxecraft_domain_ActorControllerEvent hxc_l_tmp_discarded_optional_owner_n1 = hxc_l_tmp_array_pop_result_n3;
    (void)hxc_l_tmp_discarded_optional_owner_n1;
    hxc_optional_cb78aeb6_destroy(&hxc_l_tmp_discarded_optional_owner_n1);
  }
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return;
}

struct hxc_caxecraft_domain_AuthoredAquaticEquipmentResult hxc_caxecraft_domain_GameSession_collectAuthoredAquaticEquipment(struct hxc_caxecraft_domain_GameSession *hxc_l_self, int32_t hxc_l_index, struct hxc_caxecraft_domain_AquaticProfile hxc_l_replacement)
{
  const void *hxc_l_gc_roots[1] = { (const void *)hxc_l_self };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 1, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_l_self == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_domain_EntityStore *hxc_l_tmp_owned_class_field_address_n0 = &(*hxc_l_self).hxc_entities;
  if (hxc_l_tmp_owned_class_field_address_n0 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_domain_PlayerAgent hxc_l_tmp_class_field_load_result_n1 = (*hxc_l_self).hxc_localPlayer;
  struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n3 = hxc_caxecraft_domain_EntityStore_read(hxc_l_tmp_owned_class_field_address_n0, hxc_l_tmp_class_field_load_result_n1.hxc_characterId);
  struct hxc_caxecraft_domain_Character hxc_l_original = hxc_l_tmp_instance_call_result_n3;
  if (hxc_l_original.hxc_id <= 0)
  {
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return (struct hxc_caxecraft_domain_AuthoredAquaticEquipmentResult){ .hxc_character = hxc_l_original, .hxc_collected = false, .hxc_resolved = false };
  }
  bool hxc_l_tmp_instance_call_result_n7 = hxc_caxecraft_domain_GameSession_authoredItemIsActive(hxc_l_self, hxc_l_index);
  if (!hxc_l_tmp_instance_call_result_n7)
  {
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return (struct hxc_caxecraft_domain_AuthoredAquaticEquipmentResult){ .hxc_character = hxc_l_original, .hxc_collected = false, .hxc_resolved = true };
  }
  struct hxc_caxecraft_domain_Character hxc_l_tmp_load_result_n10 = hxc_l_original;
  struct hxc_caxecraft_domain_Character hxc_l_tmp_call_result_n12 = hxc_caxecraft_domain_Character_adoptProfile(hxc_l_original, hxc_l_replacement);
  struct hxc_caxecraft_domain_LocalCharacterCommandResult hxc_l_tmp_instance_call_result_n13 = hxc_caxecraft_domain_GameSession_commitLocalCharacter(hxc_l_self, hxc_l_tmp_load_result_n10, hxc_l_tmp_call_result_n12);
  struct hxc_caxecraft_domain_LocalCharacterCommandResult hxc_l_committed = hxc_l_tmp_instance_call_result_n13;
  if (hxc_l_committed.hxc_resolved)
  {
    if (hxc_l_index < 0 || (size_t)hxc_l_index >= 256)
    {
      abort();
    }
    (*hxc_l_self).hxc_authoredItemStorage[(size_t)hxc_l_index] = 0;
  }
  struct hxc_caxecraft_domain_Character hxc_l_tmp_record_field_load_result_n15 = hxc_l_committed.hxc_character;
  bool hxc_l_tmp_record_field_load_result_n16 = hxc_l_committed.hxc_resolved;
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return (struct hxc_caxecraft_domain_AuthoredAquaticEquipmentResult){ .hxc_character = hxc_l_tmp_record_field_load_result_n15, .hxc_collected = hxc_l_tmp_record_field_load_result_n16, .hxc_resolved = hxc_l_committed.hxc_resolved };
}

struct hxc_caxecraft_domain_AuthoredInventoryItemResult hxc_caxecraft_domain_GameSession_collectAuthoredInventoryItem(struct hxc_caxecraft_domain_GameSession *hxc_l_self, int32_t hxc_l_index, struct hxc_caxecraft_gameplay_InventoryState hxc_l_inventory, enum hxc_caxecraft_gameplay_ItemKind hxc_l_kind, int32_t hxc_l_quantity)
{
  const void *hxc_l_gc_roots[1] = { (const void *)hxc_l_self };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 1, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_caxecraft_gameplay_InventoryState hxc_l_replacement = { 0 };
  bool hxc_l_tmp_short_circuit_result_n5 = hxc_l_index < 0;
  if (!(hxc_l_index < 0))
  {
    hxc_l_tmp_short_circuit_result_n5 = hxc_l_index >= 256;
  }
  bool hxc_l_tmp_short_circuit_load_result_n0 = hxc_l_tmp_short_circuit_result_n5;
  bool hxc_l_tmp_short_circuit_result_n6 = hxc_l_tmp_short_circuit_load_result_n0;
  if (!hxc_l_tmp_short_circuit_load_result_n0)
  {
    hxc_l_tmp_short_circuit_result_n6 = hxc_l_quantity <= 0;
  }
  bool hxc_l_tmp_short_circuit_load_result_n1 = hxc_l_tmp_short_circuit_result_n6;
  bool hxc_l_tmp_short_circuit_result_n7 = hxc_l_tmp_short_circuit_load_result_n1;
  if (!hxc_l_tmp_short_circuit_load_result_n1)
  {
    hxc_l_tmp_short_circuit_result_n7 = hxc_l_quantity > 64;
  }
  if (hxc_l_tmp_short_circuit_result_n7)
  {
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return (struct hxc_caxecraft_domain_AuthoredInventoryItemResult){ .hxc_collected = 0, .hxc_inventory = hxc_l_inventory, .hxc_resolved = false };
  }
  if (hxc_l_self == NULL)
  {
    abort();
  }
  bool hxc_l_tmp_instance_call_result_n4 = hxc_caxecraft_domain_GameSession_authoredItemIsActive(hxc_l_self, hxc_l_index);
  bool hxc_l_tmp_short_circuit_result_n8 = !hxc_l_tmp_instance_call_result_n4;
  if (!!hxc_l_tmp_instance_call_result_n4)
  {
    int32_t hxc_l_tmp_call_result_n5 = hxc_caxecraft_gameplay_Inventory_acceptedAmount(hxc_l_inventory, hxc_l_kind, hxc_l_quantity);
    hxc_l_tmp_short_circuit_result_n8 = hxc_l_tmp_call_result_n5 != hxc_l_quantity;
  }
  if (!hxc_l_tmp_short_circuit_result_n8)
  {
    struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n8 = hxc_caxecraft_gameplay_Inventory_collectItem(hxc_l_inventory, hxc_l_kind, hxc_l_quantity);
    hxc_l_replacement = hxc_l_tmp_call_result_n8;
    if (hxc_l_index < 0 || (size_t)hxc_l_index >= 256)
    {
      abort();
    }
    (*hxc_l_self).hxc_authoredItemStorage[(size_t)hxc_l_index] = 0;
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return (struct hxc_caxecraft_domain_AuthoredInventoryItemResult){ .hxc_collected = hxc_l_quantity, .hxc_inventory = hxc_l_replacement, .hxc_resolved = true };
  }
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return (struct hxc_caxecraft_domain_AuthoredInventoryItemResult){ .hxc_collected = 0, .hxc_inventory = hxc_l_inventory, .hxc_resolved = true };
}

struct hxc_caxecraft_domain_LocalCharacterCommandResult hxc_caxecraft_domain_GameSession_commitLocalCharacter(struct hxc_caxecraft_domain_GameSession *hxc_l_self, struct hxc_caxecraft_domain_Character hxc_l_original, struct hxc_caxecraft_domain_Character hxc_l_replacement)
{
  const void *hxc_l_gc_roots[1] = { (const void *)hxc_l_self };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 1, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  bool hxc_l_tmp_short_circuit_result_n4 = hxc_l_original.hxc_id > 0;
  if (hxc_l_original.hxc_id > 0)
  {
    if (hxc_l_self == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_domain_EntityStore *hxc_l_tmp_owned_class_field_address_n1 = &(*hxc_l_self).hxc_entities;
    if (hxc_l_tmp_owned_class_field_address_n1 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_domain_PlayerAgent hxc_l_tmp_class_field_load_result_n2 = (*hxc_l_self).hxc_localPlayer;
    bool hxc_l_tmp_instance_call_result_n4 = hxc_caxecraft_domain_EntityStore_replace(hxc_l_tmp_owned_class_field_address_n1, hxc_l_tmp_class_field_load_result_n2.hxc_characterId, hxc_l_replacement);
    hxc_l_tmp_short_circuit_result_n4 = hxc_l_tmp_instance_call_result_n4;
  }
  bool hxc_l_resolved = hxc_l_tmp_short_circuit_result_n4;
  bool hxc_l_tmp_load_result_n6 = hxc_l_resolved;
  struct hxc_caxecraft_domain_Character hxc_l_tmp_conditional_result_n5 = { 0 };
  if (hxc_l_tmp_load_result_n6)
  {
    hxc_l_tmp_conditional_result_n5 = hxc_l_replacement;
  }
  else
  {
    hxc_l_tmp_conditional_result_n5 = hxc_l_original;
  }
  struct hxc_caxecraft_domain_Character hxc_l_tmp_conditional_load_result_n7 = hxc_l_tmp_conditional_result_n5;
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return (struct hxc_caxecraft_domain_LocalCharacterCommandResult){ .hxc_character = hxc_l_tmp_conditional_load_result_n7, .hxc_resolved = hxc_l_resolved };
}

struct hxc_caxecraft_domain_CharacterDamageResult hxc_caxecraft_domain_GameSession_damageCharacter(struct hxc_caxecraft_domain_GameSession *hxc_l_self, int32_t hxc_l_id, int32_t hxc_l_amount)
{
  const void *hxc_l_gc_roots[1] = { (const void *)hxc_l_self };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 1, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_l_self == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n0 = hxc_caxecraft_domain_GameSession_readCharacter(hxc_l_self, hxc_l_id);
  struct hxc_caxecraft_domain_Character hxc_l_original = hxc_l_tmp_instance_call_result_n0;
  int32_t hxc_l_tmp_record_field_load_result_n1 = hxc_l_original.hxc_id;
  bool hxc_l_tmp_short_circuit_result_n4 = hxc_l_tmp_record_field_load_result_n1 <= 0;
  if (!(hxc_l_tmp_record_field_load_result_n1 <= 0))
  {
    hxc_l_tmp_short_circuit_result_n4 = hxc_l_amount <= 0;
  }
  if (hxc_l_tmp_short_circuit_result_n4)
  {
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return (struct hxc_caxecraft_domain_CharacterDamageResult){ .hxc_character = hxc_l_original, .hxc_damageApplied = 0, .hxc_defeated = false, .hxc_resolved = false };
  }
  struct hxc_caxecraft_domain_Character hxc_l_tmp_call_result_n6 = hxc_caxecraft_domain_Character_applyDamage(hxc_l_original, hxc_l_amount);
  struct hxc_caxecraft_domain_Character hxc_l_replacement = hxc_l_tmp_call_result_n6;
  struct hxc_caxecraft_domain_EntityStore *hxc_l_tmp_owned_class_field_address_n7 = &(*hxc_l_self).hxc_entities;
  if (hxc_l_tmp_owned_class_field_address_n7 == NULL)
  {
    abort();
  }
  bool hxc_l_tmp_instance_call_result_n9 = hxc_caxecraft_domain_EntityStore_replace(hxc_l_tmp_owned_class_field_address_n7, hxc_l_id, hxc_l_replacement);
  bool hxc_l_resolved = hxc_l_tmp_instance_call_result_n9;
  if (!hxc_l_resolved)
  {
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return (struct hxc_caxecraft_domain_CharacterDamageResult){ .hxc_character = hxc_l_original, .hxc_damageApplied = 0, .hxc_defeated = false, .hxc_resolved = false };
  }
  struct hxc_caxecraft_domain_Character hxc_l_tmp_record_field_character_n7 = hxc_l_replacement;
  int32_t hxc_l_tmp_record_field_load_result_n14 = hxc_l_original.hxc_vitals.hxc_health;
  int32_t hxc_l_tmp_record_field_damageApplied_n8 = hxc_i32_subtract_wrapping(hxc_l_tmp_record_field_load_result_n14, hxc_l_replacement.hxc_vitals.hxc_health);
  int32_t hxc_l_tmp_record_field_load_result_n16 = hxc_l_original.hxc_vitals.hxc_health;
  bool hxc_l_tmp_short_circuit_result_n9 = hxc_l_tmp_record_field_load_result_n16 > 0;
  if (hxc_l_tmp_record_field_load_result_n16 > 0)
  {
    hxc_l_tmp_short_circuit_result_n9 = hxc_l_replacement.hxc_vitals.hxc_health <= 0;
  }
  bool hxc_l_tmp_short_circuit_load_result_n18 = hxc_l_tmp_short_circuit_result_n9;
  struct hxc_caxecraft_domain_Character hxc_l_tmp_record_field_character_load_result_n19 = hxc_l_tmp_record_field_character_n7;
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return (struct hxc_caxecraft_domain_CharacterDamageResult){ .hxc_character = hxc_l_tmp_record_field_character_load_result_n19, .hxc_damageApplied = hxc_l_tmp_record_field_damageApplied_n8, .hxc_defeated = hxc_l_tmp_short_circuit_load_result_n18, .hxc_resolved = true };
}

void hxc_caxecraft_domain_GameSession_installValidatedScenarioFlow(struct hxc_caxecraft_domain_GameSession *hxc_l_self, struct hxc_caxecraft_scenario_Scenario hxc_l_scenario, struct hxc_array_ref *hxc_l_actorEntities, struct hxc_array_ref *hxc_l_actorIds)
{
  const void *hxc_l_gc_roots[5] = { (const void *)hxc_l_self, NULL, NULL, NULL, NULL };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 5, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_caxecraft_domain_ActorControllerState hxc_l_tmp_array_element_owner_n8 = { 0 };
  if (hxc_l_self == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_scenario_CaxeFlowExecutor *hxc_l_tmp_class_field_load_result_n0 = (*hxc_l_self).hxc_flowExecutor;
  hxc_l_gc_roots[1] = (const void *)hxc_l_tmp_class_field_load_result_n0;
  hxc_l_gc_roots[2] = (const void *)NULL;
  if (hxc_l_tmp_class_field_load_result_n0 != NULL)
  {
    (void)(hxc_string){ (const uint8_t *)"CaxeFlow is already installed for this GameSession", 50, true, NULL };
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    abort();
  }
  int32_t hxc_l_tmp_array_length_result_n1;
  if (hxc_array_ref_length(hxc_l_actorEntities, &hxc_l_tmp_array_length_result_n1) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_tmp_array_length_result_n2;
  if (hxc_array_ref_length(hxc_l_actorIds, &hxc_l_tmp_array_length_result_n2) != HXC_STATUS_OK)
  {
    abort();
  }
  bool hxc_l_tmp_short_circuit_result_n4 = hxc_l_tmp_array_length_result_n1 != hxc_l_tmp_array_length_result_n2;
  if (!(hxc_l_tmp_array_length_result_n1 != hxc_l_tmp_array_length_result_n2))
  {
    int32_t hxc_l_tmp_array_length_result_n3;
    if (hxc_array_ref_length(hxc_l_actorEntities, &hxc_l_tmp_array_length_result_n3) != HXC_STATUS_OK)
    {
      abort();
    }
    struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n4 = (*hxc_l_self).hxc_actorControllers;
    int32_t hxc_l_tmp_array_length_result_n5;
    if (hxc_array_ref_length(hxc_l_tmp_class_field_load_result_n4, &hxc_l_tmp_array_length_result_n5) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_short_circuit_result_n4 = hxc_l_tmp_array_length_result_n3 != hxc_l_tmp_array_length_result_n5;
  }
  if (hxc_l_tmp_short_circuit_result_n4)
  {
    (void)(hxc_string){ (const uint8_t *)"CaxeFlow actor bindings do not match this GameSession", 53, true, NULL };
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    abort();
  }
  int32_t hxc_l_g = 0;
  int32_t hxc_l_tmp_array_length_result_n7;
  if (hxc_array_ref_length(hxc_l_actorEntities, &hxc_l_tmp_array_length_result_n7) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_g1 = hxc_l_tmp_array_length_result_n7;
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n8 = hxc_l_g;
    if (!(hxc_l_tmp_load_result_n8 < hxc_l_g1))
    {
      break;
    }
    int32_t hxc_l_tmp_increment_load_result_n10 = hxc_l_g;
    hxc_l_g = hxc_i32_add_wrapping(hxc_l_tmp_increment_load_result_n10, 1);
    int32_t hxc_l_index = hxc_l_tmp_increment_load_result_n10;
    int32_t hxc_l_tmp_array_get_result_n12;
    if (hxc_array_ref_get_copy(hxc_l_actorEntities, (size_t)hxc_l_index, &hxc_l_tmp_array_get_result_n12) != HXC_STATUS_OK)
    {
      abort();
    }
    struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n13 = (*hxc_l_self).hxc_actorControllers;
    struct hxc_caxecraft_domain_ActorControllerState hxc_l_tmp_array_get_result_n15;
    if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n13, (size_t)hxc_l_index, &hxc_l_tmp_array_get_result_n15) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_array_element_owner_n8 = hxc_l_tmp_array_get_result_n15;
    struct hxc_caxecraft_domain_ActorControllerState hxc_l_tmp_array_element_borrow_result_n16 = hxc_l_tmp_array_element_owner_n8;
    hxc_array_3767ae6c_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n8);
    if (hxc_l_tmp_array_get_result_n12 != hxc_l_tmp_array_element_borrow_result_n16.hxc_characterId)
    {
      (void)(hxc_string){ (const uint8_t *)"CaxeFlow actor binding order does not match this GameSession", 60, true, NULL };
      if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
      {
        abort();
      }
      abort();
    }
  }
  struct hxc_array_ref *hxc_l_tmp_array_copy_result_n18 = NULL;
  if (hxc_array_ref_copy(hxc_l_actorEntities, &hxc_l_tmp_array_copy_result_n18) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_ref_release((*hxc_l_self).hxc_authoredActorEntities) != HXC_STATUS_OK)
  {
    abort();
  }
  (*hxc_l_self).hxc_authoredActorEntities = hxc_l_tmp_array_copy_result_n18;
  struct hxc_array_ref *hxc_l_tmp_array_copy_result_n19 = NULL;
  if (hxc_array_ref_copy(hxc_l_actorIds, &hxc_l_tmp_array_copy_result_n19) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_ref_release((*hxc_l_self).hxc_authoredActorIds) != HXC_STATUS_OK)
  {
    abort();
  }
  (*hxc_l_self).hxc_authoredActorIds = hxc_l_tmp_array_copy_result_n19;
  struct hxc_caxecraft_scenario_CaxeFlowExecutor *hxc_l_tmp_managed_class_result_n20 = NULL;
  if (hxc_gc_allocate(&hxc_program_gc, &hxc_caxecraft_scenario_CaxeFlowExecutor_descriptor, (void **)&hxc_l_tmp_managed_class_result_n20) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_gc_roots[3] = (const void *)hxc_l_tmp_managed_class_result_n20;
  if (!hxc_compiler_constructor_caxecraft_scenario_CaxeFlowExecutor(hxc_l_tmp_managed_class_result_n20, hxc_l_scenario, (struct hxc_optional_caxecraft_scenario_FlowTick){ .hxc_has_value = false }))
  {
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    abort();
  }
  hxc_l_gc_roots[4] = (const void *)hxc_l_tmp_managed_class_result_n20;
  (*hxc_l_self).hxc_flowExecutor = hxc_l_tmp_managed_class_result_n20;
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return;
}

bool hxc_caxecraft_domain_GameSession_interactWithActor(struct hxc_caxecraft_domain_GameSession *hxc_l_self, int32_t hxc_l_id)
{
  const void *hxc_l_gc_roots[1] = { (const void *)hxc_l_self };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 1, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_string hxc_l_tmp_array_element_owner_n5 = { 0 };
  hxc_string hxc_l_tmp_enum_payload_0_owner_n6 = { 0 };
  struct hxc_caxecraft_scenario_FlowEvent hxc_l_tmp_instance_call_argument_0_owner_n7 = { 0 };
  if (hxc_l_self == NULL)
  {
    abort();
  }
  bool hxc_l_tmp_instance_call_result_n0 = hxc_caxecraft_domain_GameSession_actorInteractionAvailable(hxc_l_self, hxc_l_id);
  if (!hxc_l_tmp_instance_call_result_n0)
  {
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return false;
  }
  int32_t hxc_l_g = 0;
  struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n1 = (*hxc_l_self).hxc_authoredActorEntities;
  int32_t hxc_l_tmp_array_length_result_n2;
  if (hxc_array_ref_length(hxc_l_tmp_class_field_load_result_n1, &hxc_l_tmp_array_length_result_n2) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_g1 = hxc_l_tmp_array_length_result_n2;
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n3 = hxc_l_g;
    if (!(hxc_l_tmp_load_result_n3 < hxc_l_g1))
    {
      break;
    }
    int32_t hxc_l_tmp_increment_load_result_n5 = hxc_l_g;
    hxc_l_g = hxc_i32_add_wrapping(hxc_l_tmp_increment_load_result_n5, 1);
    int32_t hxc_l_index = hxc_l_tmp_increment_load_result_n5;
    struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n6 = (*hxc_l_self).hxc_authoredActorEntities;
    int32_t hxc_l_tmp_array_get_result_n8;
    if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n6, (size_t)hxc_l_index, &hxc_l_tmp_array_get_result_n8) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_l_tmp_array_get_result_n8 == hxc_l_id)
    {
      struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n9 = (*hxc_l_self).hxc_authoredActorIds;
      hxc_string hxc_l_tmp_array_get_result_n11;
      if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n9, (size_t)hxc_l_index, &hxc_l_tmp_array_get_result_n11) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_l_tmp_array_element_owner_n5 = hxc_l_tmp_array_get_result_n11;
      hxc_l_tmp_enum_payload_0_owner_n6 = hxc_l_tmp_array_element_owner_n5;
      if (hxc_string_retain(hxc_l_tmp_enum_payload_0_owner_n6) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_l_tmp_instance_call_argument_0_owner_n7 = (struct hxc_caxecraft_scenario_FlowEvent){ .hxc_tag = hxc_caxecraft_scenario_FlowEvent_Interact, .hxc_payload.hxc_Interact.hxc_objectId = hxc_l_tmp_enum_payload_0_owner_n6 };
      bool hxc_l_tmp_instance_call_result_n16 = hxc_caxecraft_domain_GameSession_queueFlowEvent(hxc_l_self, hxc_l_tmp_instance_call_argument_0_owner_n7);
      hxc_enum_10047054_destroy(&hxc_l_tmp_instance_call_argument_0_owner_n7);
      if (hxc_string_release(&hxc_l_tmp_array_element_owner_n5) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
      {
        abort();
      }
      return hxc_l_tmp_instance_call_result_n16;
    }
  }
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return false;
}

struct hxc_caxecraft_gameplay_MiningResult hxc_caxecraft_domain_GameSession_mineTerrain(struct hxc_caxecraft_domain_GameSession *hxc_l_self, struct hxc_caxecraft_scenario_VoxelPoint hxc_l_coord, struct hxc_caxecraft_gameplay_InventoryState hxc_l_inventory)
{
  const void *hxc_l_gc_roots[1] = { (const void *)hxc_l_self };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 1, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_l_self == NULL)
  {
    abort();
  }
  uint8_t *hxc_l_cells = (*hxc_l_self).hxc_worldStorage;
  size_t hxc_l_tmp_length_n3 = 16384;
  struct hxc_caxecraft_gameplay_MiningResult hxc_l_tmp_call_result_n0 = hxc_caxecraft_gameplay_Mining_attempt(hxc_l_cells, hxc_l_tmp_length_n3, hxc_l_coord, hxc_l_inventory);
  struct hxc_caxecraft_gameplay_MiningResult hxc_l_result = hxc_l_tmp_call_result_n0;
  if (hxc_l_result.hxc_outcome == 2)
  {
    struct hxc_caxecraft_domain_WaterSimulation *hxc_l_tmp_owned_class_field_address_n2 = &(*hxc_l_self).hxc_water;
    if (hxc_l_tmp_owned_class_field_address_n2 == NULL)
    {
      abort();
    }
    hxc_caxecraft_domain_WaterSimulation_terrainChanged(hxc_l_tmp_owned_class_field_address_n2, hxc_l_coord);
  }
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return hxc_l_result;
}

bool hxc_caxecraft_domain_GameSession_placeInitialWaterVolume(struct hxc_caxecraft_domain_GameSession *hxc_l_self, struct hxc_caxecraft_scenario_VoxelPoint hxc_l_origin, int32_t hxc_l_width, int32_t hxc_l_height, int32_t hxc_l_depth)
{
  const void *hxc_l_gc_roots[1] = { (const void *)hxc_l_self };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 1, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_l_self == NULL)
  {
    abort();
  }
  uint8_t *hxc_l_cells = (*hxc_l_self).hxc_worldStorage;
  size_t hxc_l_tmp_length_n5 = 16384;
  struct hxc_caxecraft_domain_WaterSimulation *hxc_l_tmp_owned_class_field_address_n0 = &(*hxc_l_self).hxc_water;
  if (hxc_l_tmp_owned_class_field_address_n0 == NULL)
  {
    abort();
  }
  bool hxc_l_tmp_instance_call_result_n1 = hxc_caxecraft_domain_WaterSimulation_placeInitialVolume(hxc_l_tmp_owned_class_field_address_n0, hxc_l_cells, hxc_l_tmp_length_n5, hxc_l_origin, hxc_l_width, hxc_l_height, hxc_l_depth);
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return hxc_l_tmp_instance_call_result_n1;
}

bool hxc_caxecraft_domain_GameSession_placeTerrain(struct hxc_caxecraft_domain_GameSession *hxc_l_self, struct hxc_caxecraft_scenario_VoxelPoint hxc_l_coord, enum hxc_caxecraft_domain_BlockKind hxc_l_kind)
{
  const void *hxc_l_gc_roots[1] = { (const void *)hxc_l_self };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 1, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_l_self == NULL)
  {
    abort();
  }
  uint8_t *hxc_l_cells = (*hxc_l_self).hxc_worldStorage;
  size_t hxc_l_tmp_length_n3 = 16384;
  struct hxc_caxecraft_domain_WaterSimulation *hxc_l_tmp_owned_class_field_address_n0 = &(*hxc_l_self).hxc_water;
  if (hxc_l_tmp_owned_class_field_address_n0 == NULL)
  {
    abort();
  }
  bool hxc_l_tmp_instance_call_result_n1 = hxc_caxecraft_domain_WaterSimulation_placeTerrain(hxc_l_tmp_owned_class_field_address_n0, hxc_l_cells, hxc_l_tmp_length_n3, hxc_l_coord, hxc_l_kind);
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return hxc_l_tmp_instance_call_result_n1;
}

bool hxc_caxecraft_domain_GameSession_placeWaterSource(struct hxc_caxecraft_domain_GameSession *hxc_l_self, struct hxc_caxecraft_scenario_VoxelPoint hxc_l_coord)
{
  const void *hxc_l_gc_roots[1] = { (const void *)hxc_l_self };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 1, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_l_self == NULL)
  {
    abort();
  }
  uint8_t *hxc_l_cells = (*hxc_l_self).hxc_worldStorage;
  size_t hxc_l_tmp_length_n2 = 16384;
  struct hxc_caxecraft_domain_WaterSimulation *hxc_l_tmp_owned_class_field_address_n0 = &(*hxc_l_self).hxc_water;
  if (hxc_l_tmp_owned_class_field_address_n0 == NULL)
  {
    abort();
  }
  bool hxc_l_tmp_instance_call_result_n1 = hxc_caxecraft_domain_WaterSimulation_placeSource(hxc_l_tmp_owned_class_field_address_n0, hxc_l_cells, hxc_l_tmp_length_n2, hxc_l_coord);
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return hxc_l_tmp_instance_call_result_n1;
}

bool hxc_caxecraft_domain_GameSession_queueFlowEvent(struct hxc_caxecraft_domain_GameSession *hxc_l_self, struct hxc_caxecraft_scenario_FlowEvent hxc_l_event)
{
  const void *hxc_l_gc_roots[3] = { (const void *)hxc_l_self, NULL, NULL };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 3, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_l_self == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_scenario_CaxeFlowExecutor *hxc_l_tmp_class_field_load_result_n0 = (*hxc_l_self).hxc_flowExecutor;
  hxc_l_gc_roots[1] = (const void *)hxc_l_tmp_class_field_load_result_n0;
  hxc_l_gc_roots[2] = (const void *)NULL;
  bool hxc_l_tmp_short_circuit_result_n2 = hxc_l_tmp_class_field_load_result_n0 == NULL;
  if (!(hxc_l_tmp_class_field_load_result_n0 == NULL))
  {
    struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n1 = (*hxc_l_self).hxc_pendingFlowEvents;
    int32_t hxc_l_tmp_array_length_result_n2;
    if (hxc_array_ref_length(hxc_l_tmp_class_field_load_result_n1, &hxc_l_tmp_array_length_result_n2) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_short_circuit_result_n2 = hxc_l_tmp_array_length_result_n2 >= 256;
  }
  if (!hxc_l_tmp_short_circuit_result_n2)
  {
    struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n4 = (*hxc_l_self).hxc_pendingFlowEvents;
    int32_t hxc_l_tmp_array_push_result_n5;
    if (hxc_array_ref_push_copy(hxc_l_tmp_class_field_load_result_n4, &hxc_l_event, &hxc_l_tmp_array_push_result_n5) != HXC_STATUS_OK)
    {
      abort();
    }
    (void)hxc_l_tmp_array_push_result_n5;
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return true;
  }
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return false;
}

struct hxc_caxecraft_domain_Character hxc_caxecraft_domain_GameSession_readCharacter(struct hxc_caxecraft_domain_GameSession *hxc_l_self, int32_t hxc_l_id)
{
  const void *hxc_l_gc_roots[1] = { (const void *)hxc_l_self };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 1, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_l_self == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_domain_EntityStore *hxc_l_tmp_owned_class_field_address_n0 = &(*hxc_l_self).hxc_entities;
  if (hxc_l_tmp_owned_class_field_address_n0 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n1 = hxc_caxecraft_domain_EntityStore_read(hxc_l_tmp_owned_class_field_address_n0, hxc_l_id);
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return hxc_l_tmp_instance_call_result_n1;
}

struct hxc_caxecraft_domain_LocalCharacterCommandResult hxc_caxecraft_domain_GameSession_receiveLocalPlayerAttack(struct hxc_caxecraft_domain_GameSession *hxc_l_self)
{
  const void *hxc_l_gc_roots[1] = { (const void *)hxc_l_self };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 1, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_l_self == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_domain_EntityStore *hxc_l_tmp_owned_class_field_address_n0 = &(*hxc_l_self).hxc_entities;
  if (hxc_l_tmp_owned_class_field_address_n0 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_domain_PlayerAgent hxc_l_tmp_class_field_load_result_n1 = (*hxc_l_self).hxc_localPlayer;
  struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n3 = hxc_caxecraft_domain_EntityStore_read(hxc_l_tmp_owned_class_field_address_n0, hxc_l_tmp_class_field_load_result_n1.hxc_characterId);
  struct hxc_caxecraft_domain_Character hxc_l_original = hxc_l_tmp_instance_call_result_n3;
  if (!(hxc_l_original.hxc_id <= 0))
  {
    struct hxc_caxecraft_domain_Character hxc_l_tmp_load_result_n7 = hxc_l_original;
    struct hxc_caxecraft_domain_Character hxc_l_tmp_call_result_n9 = hxc_caxecraft_domain_Character_applyAttack(hxc_l_original, true);
    struct hxc_caxecraft_domain_LocalCharacterCommandResult hxc_l_tmp_instance_call_result_n10 = hxc_caxecraft_domain_GameSession_commitLocalCharacter(hxc_l_self, hxc_l_tmp_load_result_n7, hxc_l_tmp_call_result_n9);
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return hxc_l_tmp_instance_call_result_n10;
  }
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return (struct hxc_caxecraft_domain_LocalCharacterCommandResult){ .hxc_character = hxc_l_original, .hxc_resolved = false };
}

bool hxc_caxecraft_domain_GameSession_removeTerrain(struct hxc_caxecraft_domain_GameSession *hxc_l_self, struct hxc_caxecraft_scenario_VoxelPoint hxc_l_coord)
{
  const void *hxc_l_gc_roots[1] = { (const void *)hxc_l_self };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 1, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_l_self == NULL)
  {
    abort();
  }
  uint8_t *hxc_l_cells = (*hxc_l_self).hxc_worldStorage;
  size_t hxc_l_tmp_length_n2 = 16384;
  bool hxc_l_tmp_call_result_n0 = hxc_caxecraft_domain_World_remove(hxc_l_cells, hxc_l_tmp_length_n2, hxc_l_coord);
  if (!!hxc_l_tmp_call_result_n0)
  {
    struct hxc_caxecraft_domain_WaterSimulation *hxc_l_tmp_owned_class_field_address_n1 = &(*hxc_l_self).hxc_water;
    if (hxc_l_tmp_owned_class_field_address_n1 == NULL)
    {
      abort();
    }
    hxc_caxecraft_domain_WaterSimulation_terrainChanged(hxc_l_tmp_owned_class_field_address_n1, hxc_l_coord);
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return true;
  }
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return false;
}

bool hxc_caxecraft_domain_GameSession_replaceAuthoredActors(struct hxc_caxecraft_domain_GameSession *hxc_l_self, struct hxc_array_ref *hxc_l_characters, struct hxc_array_ref *hxc_l_controllers)
{
  const void *hxc_l_gc_roots[1] = { (const void *)hxc_l_self };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 1, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_caxecraft_domain_ActorControllerBinding hxc_l_tmp_array_element_owner_n11 = { 0 };
  struct hxc_caxecraft_domain_ActorControllerBinding hxc_l_tmp_array_element_owner_n12 = { 0 };
  struct hxc_caxecraft_domain_ActorControllerState hxc_l_tmp_array_push_element_owner_n13 = { 0 };
  struct hxc_caxecraft_domain_ActorControllerState hxc_l_state = { 0 };
  struct hxc_caxecraft_domain_ActorControllerState hxc_l_tmp_array_element_owner_n17 = { 0 };
  struct hxc_array_ref *hxc_l_ownedCharacters = { 0 };
  struct hxc_array_ref *hxc_l_ownedControllers = { 0 };
  struct hxc_array_ref *hxc_l_ownedStates = { 0 };
  if (hxc_l_self == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_domain_PlayerAgent hxc_l_tmp_class_field_load_result_n0 = (*hxc_l_self).hxc_localPlayer;
  bool hxc_l_tmp_short_circuit_result_n3 = hxc_l_tmp_class_field_load_result_n0.hxc_characterId > 0;
  if (hxc_l_tmp_class_field_load_result_n0.hxc_characterId > 0)
  {
    struct hxc_caxecraft_domain_EntityStore *hxc_l_tmp_owned_class_field_address_n2 = &(*hxc_l_self).hxc_entities;
    if (hxc_l_tmp_owned_class_field_address_n2 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_domain_PlayerAgent hxc_l_tmp_class_field_load_result_n3 = (*hxc_l_self).hxc_localPlayer;
    bool hxc_l_tmp_instance_call_result_n5 = hxc_caxecraft_domain_EntityStore_contains(hxc_l_tmp_owned_class_field_address_n2, hxc_l_tmp_class_field_load_result_n3.hxc_characterId);
    hxc_l_tmp_short_circuit_result_n3 = hxc_l_tmp_instance_call_result_n5;
  }
  bool hxc_l_tmp_short_circuit_load_result_n6 = hxc_l_tmp_short_circuit_result_n3;
  bool hxc_l_tmp_short_circuit_result_n4 = !hxc_l_tmp_short_circuit_load_result_n6;
  if (!!hxc_l_tmp_short_circuit_load_result_n6)
  {
    int32_t hxc_l_tmp_array_length_result_n7;
    if (hxc_array_ref_length(hxc_l_characters, &hxc_l_tmp_array_length_result_n7) != HXC_STATUS_OK)
    {
      abort();
    }
    int32_t hxc_l_tmp_array_length_result_n8;
    if (hxc_array_ref_length(hxc_l_controllers, &hxc_l_tmp_array_length_result_n8) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_short_circuit_result_n4 = hxc_l_tmp_array_length_result_n7 != hxc_l_tmp_array_length_result_n8;
  }
  if (hxc_l_tmp_short_circuit_result_n4)
  {
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return false;
  }
  struct hxc_array_ref *hxc_l_tmp_array_copy_result_n10 = NULL;
  if (hxc_array_ref_copy(hxc_l_characters, &hxc_l_tmp_array_copy_result_n10) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_ownedCharacters = hxc_l_tmp_array_copy_result_n10;
  struct hxc_array_ref *hxc_l_tmp_array_copy_result_n11 = NULL;
  if (hxc_array_ref_copy(hxc_l_controllers, &hxc_l_tmp_array_copy_result_n11) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_ownedControllers = hxc_l_tmp_array_copy_result_n11;
  struct hxc_array_ref *hxc_l_tmp_array_create_result_n12 = NULL;
  if (hxc_array_ref_create(hxc_default_allocator(), (hxc_array_element_ops){ sizeof(struct hxc_caxecraft_domain_ActorControllerState), _Alignof(struct hxc_caxecraft_domain_ActorControllerState), NULL, hxc_array_3767ae6c_element_copy, hxc_array_3767ae6c_element_assign, hxc_array_3767ae6c_element_destroy }, &hxc_l_tmp_array_create_result_n12) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_ownedStates = hxc_l_tmp_array_create_result_n12;
  int32_t hxc_l_g_h88094f548a88 = 0;
  int32_t hxc_l_tmp_array_length_result_n14;
  if (hxc_array_ref_length(hxc_l_ownedCharacters, &hxc_l_tmp_array_length_result_n14) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_g1 = hxc_l_tmp_array_length_result_n14;
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n15 = hxc_l_g_h88094f548a88;
    if (!(hxc_l_tmp_load_result_n15 < hxc_l_g1))
    {
      break;
    }
    int32_t hxc_l_tmp_increment_load_result_n17 = hxc_l_g_h88094f548a88;
    hxc_l_g_h88094f548a88 = hxc_i32_add_wrapping(hxc_l_tmp_increment_load_result_n17, 1);
    int32_t hxc_l_index = hxc_l_tmp_increment_load_result_n17;
    struct hxc_array_ref *hxc_l_tmp_load_result_n18 = hxc_l_ownedControllers;
    struct hxc_caxecraft_domain_ActorControllerBinding hxc_l_tmp_array_get_result_n20;
    if (hxc_array_ref_get_copy(hxc_l_tmp_load_result_n18, (size_t)hxc_l_index, &hxc_l_tmp_array_get_result_n20) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_array_element_owner_n11 = hxc_l_tmp_array_get_result_n20;
    struct hxc_caxecraft_domain_ActorControllerBinding hxc_l_tmp_array_element_borrow_result_n21 = hxc_l_tmp_array_element_owner_n11;
    struct hxc_array_ref *hxc_l_tmp_load_result_n23 = hxc_l_ownedCharacters;
    struct hxc_caxecraft_domain_Character hxc_l_tmp_array_get_result_n25;
    if (hxc_array_ref_get_copy(hxc_l_tmp_load_result_n23, (size_t)hxc_l_index, &hxc_l_tmp_array_get_result_n25) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_array_622f12db_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n11);
    if (hxc_l_tmp_array_element_borrow_result_n21.hxc_characterId != hxc_l_tmp_array_get_result_n25.hxc_id)
    {
      if (hxc_array_ref_release(hxc_l_ownedStates) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_l_ownedControllers) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_l_ownedCharacters) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
      {
        abort();
      }
      return false;
    }
    struct hxc_array_ref *hxc_l_tmp_load_result_n27 = hxc_l_ownedStates;
    struct hxc_array_ref *hxc_l_tmp_load_result_n28 = hxc_l_ownedControllers;
    struct hxc_caxecraft_domain_ActorControllerBinding hxc_l_tmp_array_get_result_n30;
    if (hxc_array_ref_get_copy(hxc_l_tmp_load_result_n28, (size_t)hxc_l_index, &hxc_l_tmp_array_get_result_n30) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_array_element_owner_n12 = hxc_l_tmp_array_get_result_n30;
    struct hxc_caxecraft_domain_ActorControllerBinding hxc_l_tmp_array_element_borrow_result_n31 = hxc_l_tmp_array_element_owner_n12;
    struct hxc_array_ref *hxc_l_tmp_load_result_n32 = hxc_l_ownedCharacters;
    struct hxc_caxecraft_domain_Character hxc_l_tmp_array_get_result_n34;
    if (hxc_array_ref_get_copy(hxc_l_tmp_load_result_n32, (size_t)hxc_l_index, &hxc_l_tmp_array_get_result_n34) != HXC_STATUS_OK)
    {
      abort();
    }
    struct hxc_caxecraft_domain_ActorControllerState hxc_l_tmp_call_result_n35 = hxc_caxecraft_domain_ActorControllerScheduler_startActorController(hxc_l_tmp_array_element_borrow_result_n31, hxc_l_tmp_array_get_result_n34);
    hxc_l_tmp_array_push_element_owner_n13 = hxc_l_tmp_call_result_n35;
    int32_t hxc_l_tmp_array_push_result_n37;
    if (hxc_array_ref_push_copy(hxc_l_tmp_load_result_n27, &hxc_l_tmp_array_push_element_owner_n13, &hxc_l_tmp_array_push_result_n37) != HXC_STATUS_OK)
    {
      abort();
    }
    (void)hxc_l_tmp_array_push_result_n37;
    hxc_record_50dff1c7_destroy(&hxc_l_tmp_array_push_element_owner_n13);
    hxc_array_622f12db_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n12);
  }
  struct hxc_caxecraft_domain_EntityStore *hxc_l_tmp_owned_class_field_address_n38 = &(*hxc_l_self).hxc_entities;
  if (hxc_l_tmp_owned_class_field_address_n38 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_domain_PlayerAgent hxc_l_tmp_class_field_load_result_n39 = (*hxc_l_self).hxc_localPlayer;
  bool hxc_l_tmp_instance_call_result_n42 = hxc_caxecraft_domain_EntityStore_replaceOthers(hxc_l_tmp_owned_class_field_address_n38, hxc_l_tmp_class_field_load_result_n39.hxc_characterId, hxc_l_ownedCharacters);
  if (!hxc_l_tmp_instance_call_result_n42)
  {
    if (hxc_array_ref_release(hxc_l_ownedStates) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_ownedControllers) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_ownedCharacters) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return false;
  }
  while (1)
  {
    struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n43 = (*hxc_l_self).hxc_actorControllers;
    int32_t hxc_l_tmp_array_length_result_n44;
    if (hxc_array_ref_length(hxc_l_tmp_class_field_load_result_n43, &hxc_l_tmp_array_length_result_n44) != HXC_STATUS_OK)
    {
      abort();
    }
    if (!(hxc_l_tmp_array_length_result_n44 > 0))
    {
      break;
    }
    struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n45 = (*hxc_l_self).hxc_actorControllers;
    struct hxc_optional_caxecraft_domain_ActorControllerState hxc_l_tmp_array_pop_result_n46 = (struct hxc_optional_caxecraft_domain_ActorControllerState){ .hxc_has_value = false };
    if (hxc_array_ref_pop_move(hxc_l_tmp_class_field_load_result_n45, &hxc_l_tmp_array_pop_result_n46.hxc_value, &hxc_l_tmp_array_pop_result_n46.hxc_has_value) != HXC_STATUS_OK)
    {
      abort();
    }
    struct hxc_optional_caxecraft_domain_ActorControllerState hxc_l_tmp_discarded_optional_owner_n14 = hxc_l_tmp_array_pop_result_n46;
    (void)hxc_l_tmp_discarded_optional_owner_n14;
    hxc_optional_294b4763_destroy(&hxc_l_tmp_discarded_optional_owner_n14);
  }
  int32_t hxc_l_g_h8024990fba0a = 0;
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n47 = hxc_l_g_h8024990fba0a;
    int32_t hxc_l_tmp_array_length_result_n49;
    if (hxc_array_ref_length(hxc_l_ownedStates, &hxc_l_tmp_array_length_result_n49) != HXC_STATUS_OK)
    {
      abort();
    }
    if (!(hxc_l_tmp_load_result_n47 < hxc_l_tmp_array_length_result_n49))
    {
      break;
    }
    struct hxc_array_ref *hxc_l_tmp_load_result_n50 = hxc_l_ownedStates;
    struct hxc_caxecraft_domain_ActorControllerState hxc_l_tmp_array_get_result_n52;
    if (hxc_array_ref_get_copy(hxc_l_tmp_load_result_n50, (size_t)hxc_l_g_h8024990fba0a, &hxc_l_tmp_array_get_result_n52) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_array_element_owner_n17 = hxc_l_tmp_array_get_result_n52;
    hxc_l_state = hxc_l_tmp_array_element_owner_n17;
    if (hxc_record_50dff1c7_retain(&hxc_l_state) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_g_h8024990fba0a = hxc_i32_add_wrapping(hxc_l_g_h8024990fba0a, 1);
    struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n55 = (*hxc_l_self).hxc_actorControllers;
    int32_t hxc_l_tmp_array_push_result_n57;
    if (hxc_array_ref_push_copy(hxc_l_tmp_class_field_load_result_n55, &hxc_l_state, &hxc_l_tmp_array_push_result_n57) != HXC_STATUS_OK)
    {
      abort();
    }
    (void)hxc_l_tmp_array_push_result_n57;
    hxc_record_50dff1c7_destroy(&hxc_l_state);
    hxc_array_3767ae6c_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n17);
  }
  hxc_caxecraft_domain_GameSession_clearActorControllerEvents(hxc_l_self);
  if (hxc_array_ref_release(hxc_l_ownedStates) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_ref_release(hxc_l_ownedControllers) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_ref_release(hxc_l_ownedCharacters) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return true;
}

struct hxc_caxecraft_domain_LocalCharacterCommandResult hxc_caxecraft_domain_GameSession_reviveLocalPlayerAt(struct hxc_caxecraft_domain_GameSession *hxc_l_self, struct hxc_caxecraft_domain_CharacterBody hxc_l_body)
{
  const void *hxc_l_gc_roots[1] = { (const void *)hxc_l_self };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 1, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_l_self == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_domain_EntityStore *hxc_l_tmp_owned_class_field_address_n0 = &(*hxc_l_self).hxc_entities;
  if (hxc_l_tmp_owned_class_field_address_n0 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_domain_PlayerAgent hxc_l_tmp_class_field_load_result_n1 = (*hxc_l_self).hxc_localPlayer;
  struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n3 = hxc_caxecraft_domain_EntityStore_read(hxc_l_tmp_owned_class_field_address_n0, hxc_l_tmp_class_field_load_result_n1.hxc_characterId);
  struct hxc_caxecraft_domain_Character hxc_l_original = hxc_l_tmp_instance_call_result_n3;
  if (!(hxc_l_original.hxc_id <= 0))
  {
    struct hxc_caxecraft_domain_Character hxc_l_tmp_load_result_n7 = hxc_l_original;
    struct hxc_caxecraft_domain_Character hxc_l_tmp_call_result_n9 = hxc_caxecraft_domain_Character_reviveAt(hxc_l_original, hxc_l_body);
    struct hxc_caxecraft_domain_LocalCharacterCommandResult hxc_l_tmp_instance_call_result_n10 = hxc_caxecraft_domain_GameSession_commitLocalCharacter(hxc_l_self, hxc_l_tmp_load_result_n7, hxc_l_tmp_call_result_n9);
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return hxc_l_tmp_instance_call_result_n10;
  }
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return (struct hxc_caxecraft_domain_LocalCharacterCommandResult){ .hxc_character = hxc_l_original, .hxc_resolved = false };
}

struct hxc_caxecraft_domain_ActorControllerTickResult hxc_caxecraft_domain_GameSession_stepAuthoredActorControllers(struct hxc_caxecraft_domain_GameSession *hxc_l_self, int32_t hxc_l_tickNumber, int32_t hxc_l_damagePolicy)
{
  const void *hxc_l_gc_roots[1] = { (const void *)hxc_l_self };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 1, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_caxecraft_domain_ActorControllerDecision hxc_l_decision = { 0 };
  struct hxc_caxecraft_domain_ActorControllerState hxc_l_next = { 0 };
  struct hxc_caxecraft_domain_ActorControllerEvent hxc_l_event = { 0 };
  enum hxc_caxecraft_domain_ActorControllerPlanError hxc_l_error = { 0 };
  struct hxc_caxecraft_domain_ActorControllerState hxc_l_state = { 0 };
  struct hxc_caxecraft_domain_ActorControllerState hxc_l_tmp_array_element_owner_n9 = { 0 };
  if (hxc_l_self == NULL)
  {
    abort();
  }
  hxc_caxecraft_domain_GameSession_clearActorControllerEvents(hxc_l_self);
  struct hxc_caxecraft_domain_EntityStore *hxc_l_tmp_owned_class_field_address_n0 = &(*hxc_l_self).hxc_entities;
  if (hxc_l_tmp_owned_class_field_address_n0 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_domain_PlayerAgent hxc_l_tmp_class_field_load_result_n1 = (*hxc_l_self).hxc_localPlayer;
  struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n3 = hxc_caxecraft_domain_EntityStore_read(hxc_l_tmp_owned_class_field_address_n0, hxc_l_tmp_class_field_load_result_n1.hxc_characterId);
  struct hxc_caxecraft_domain_Character hxc_l_observedLocalPlayer = hxc_l_tmp_instance_call_result_n3;
  int32_t hxc_l_processed = 0;
  int32_t hxc_l_g = 0;
  struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n4 = (*hxc_l_self).hxc_actorControllers;
  int32_t hxc_l_tmp_array_length_result_n5;
  if (hxc_array_ref_length(hxc_l_tmp_class_field_load_result_n4, &hxc_l_tmp_array_length_result_n5) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_g1 = hxc_l_tmp_array_length_result_n5;
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n6 = hxc_l_g;
    if (!(hxc_l_tmp_load_result_n6 < hxc_l_g1))
    {
      break;
    }
    int32_t hxc_l_tmp_increment_load_result_n8 = hxc_l_g;
    hxc_l_g = hxc_i32_add_wrapping(hxc_l_tmp_increment_load_result_n8, 1);
    int32_t hxc_l_index = hxc_l_tmp_increment_load_result_n8;
    struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n9 = (*hxc_l_self).hxc_actorControllers;
    struct hxc_caxecraft_domain_ActorControllerState hxc_l_tmp_array_get_result_n11;
    if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n9, (size_t)hxc_l_index, &hxc_l_tmp_array_get_result_n11) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_array_element_owner_n9 = hxc_l_tmp_array_get_result_n11;
    hxc_l_state = hxc_l_tmp_array_element_owner_n9;
    if (hxc_record_50dff1c7_retain(&hxc_l_state) != HXC_STATUS_OK)
    {
      abort();
    }
    struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n14 = hxc_caxecraft_domain_GameSession_readCharacter(hxc_l_self, hxc_l_state.hxc_characterId);
    struct hxc_caxecraft_domain_Character hxc_l_character = hxc_l_tmp_instance_call_result_n14;
    if (hxc_l_character.hxc_id <= 0)
    {
      int32_t hxc_l_tmp_record_field_load_result_n16 = hxc_l_state.hxc_characterId;
      struct hxc_caxecraft_domain_ActorControllerTickResult hxc_l_tmp_instance_call_result_n19 = hxc_caxecraft_domain_GameSession_actorControllerTick(hxc_l_self, (struct hxc_caxecraft_domain_ActorControllerTickStatus){ .hxc_tag = hxc_caxecraft_domain_ActorControllerTickStatus_ControlledCharacterMissing, .hxc_payload.hxc_ControlledCharacterMissing.hxc_characterId = hxc_l_tmp_record_field_load_result_n16 }, hxc_l_processed);
      hxc_record_50dff1c7_destroy(&hxc_l_state);
      hxc_array_3767ae6c_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n9);
      if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
      {
        abort();
      }
      return hxc_l_tmp_instance_call_result_n19;
    }
    struct hxc_caxecraft_domain_ActorControllerState hxc_l_tmp_load_result_n20 = hxc_l_state;
    struct hxc_caxecraft_domain_Character hxc_l_tmp_load_result_n21 = hxc_l_character;
    struct hxc_caxecraft_domain_ActorControllerDecision hxc_l_tmp_call_result_n23 = hxc_caxecraft_domain_ActorControllerScheduler_planActorController(hxc_l_tmp_load_result_n20, hxc_l_tmp_load_result_n21, hxc_l_observedLocalPlayer, hxc_l_tickNumber);
    hxc_l_decision = hxc_l_tmp_call_result_n23;
    switch (hxc_l_decision.hxc_tag) {
      case hxc_caxecraft_domain_ActorControllerDecision_ControllerPlanned:
        {
          break;
        }
      case hxc_caxecraft_domain_ActorControllerDecision_ControllerPlanRejected:
        {
          if (hxc_l_decision.hxc_tag != hxc_caxecraft_domain_ActorControllerDecision_ControllerPlanRejected)
          {
            abort();
          }
          enum hxc_caxecraft_domain_ActorControllerPlanError hxc_l_tmp_enum_payload_project_n64 = hxc_l_decision.hxc_payload.hxc_ControllerPlanRejected.hxc_error;
          hxc_l_error = hxc_l_tmp_enum_payload_project_n64;
          int32_t hxc_l_tmp_record_field_load_result_n65 = hxc_l_state.hxc_characterId;
          enum hxc_caxecraft_domain_ActorControllerPlanError hxc_l_tmp_load_result_n66 = hxc_l_error;
          struct hxc_caxecraft_domain_ActorControllerTickResult hxc_l_tmp_instance_call_result_n69 = hxc_caxecraft_domain_GameSession_actorControllerTick(hxc_l_self, (struct hxc_caxecraft_domain_ActorControllerTickStatus){ .hxc_tag = hxc_caxecraft_domain_ActorControllerTickStatus_ControllerModelRejected, .hxc_payload.hxc_ControllerModelRejected.hxc_characterId = hxc_l_tmp_record_field_load_result_n65, .hxc_payload.hxc_ControllerModelRejected.hxc_error = hxc_l_tmp_load_result_n66 }, hxc_l_processed);
          hxc_enum_8199493a_destroy(&hxc_l_decision);
          hxc_record_50dff1c7_destroy(&hxc_l_state);
          hxc_array_3767ae6c_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n9);
          if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
          {
            abort();
          }
          return hxc_l_tmp_instance_call_result_n69;
        }
      default:
        {
          abort();
        }
    }
    if (hxc_l_decision.hxc_tag != hxc_caxecraft_domain_ActorControllerDecision_ControllerPlanned)
    {
      abort();
    }
    struct hxc_caxecraft_domain_ActorControllerState hxc_l_tmp_enum_payload_project_n26 = hxc_l_decision.hxc_payload.hxc_ControllerPlanned.hxc_next;
    hxc_l_next = hxc_l_tmp_enum_payload_project_n26;
    if (hxc_record_50dff1c7_retain(&hxc_l_next) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_l_decision.hxc_tag != hxc_caxecraft_domain_ActorControllerDecision_ControllerPlanned)
    {
      abort();
    }
    struct hxc_caxecraft_domain_CharacterIntent hxc_l_tmp_enum_payload_project_n28 = hxc_l_decision.hxc_payload.hxc_ControllerPlanned.hxc_intent;
    struct hxc_caxecraft_domain_CharacterIntent hxc_l_intent = hxc_l_tmp_enum_payload_project_n28;
    if (hxc_l_decision.hxc_tag != hxc_caxecraft_domain_ActorControllerDecision_ControllerPlanned)
    {
      abort();
    }
    struct hxc_caxecraft_domain_ActorControllerEvent hxc_l_tmp_enum_payload_project_n30 = hxc_l_decision.hxc_payload.hxc_ControllerPlanned.hxc_event;
    hxc_l_event = hxc_l_tmp_enum_payload_project_n30;
    if (hxc_enum_8b2a26b8_retain(&hxc_l_event) != HXC_STATUS_OK)
    {
      abort();
    }
    switch (hxc_l_event.hxc_tag) {
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
          if (hxc_l_event.hxc_tag != hxc_caxecraft_domain_ActorControllerEvent_LocalPlayerAttack)
          {
            abort();
          }
          int32_t hxc_l_tmp_enum_payload_project_n33 = hxc_l_event.hxc_payload.hxc_LocalPlayerAttack.hxc_characterId;
          int32_t hxc_l_source = hxc_l_tmp_enum_payload_project_n33;
          struct hxc_caxecraft_domain_LocalCharacterCommandResult hxc_l_tmp_instance_call_result_n34 = hxc_caxecraft_domain_GameSession_receiveLocalPlayerAttack(hxc_l_self);
          struct hxc_caxecraft_domain_LocalCharacterCommandResult hxc_l_attack = hxc_l_tmp_instance_call_result_n34;
          if (!hxc_l_attack.hxc_resolved)
          {
            int32_t hxc_l_tmp_load_result_n36 = hxc_l_source;
            struct hxc_caxecraft_domain_ActorControllerTickResult hxc_l_tmp_instance_call_result_n39 = hxc_caxecraft_domain_GameSession_actorControllerTick(hxc_l_self, (struct hxc_caxecraft_domain_ActorControllerTickStatus){ .hxc_tag = hxc_caxecraft_domain_ActorControllerTickStatus_LocalAttackCommandRejected, .hxc_payload.hxc_LocalAttackCommandRejected.hxc_characterId = hxc_l_tmp_load_result_n36 }, hxc_l_processed);
            hxc_enum_8b2a26b8_destroy(&hxc_l_event);
            hxc_record_50dff1c7_destroy(&hxc_l_next);
            hxc_enum_8199493a_destroy(&hxc_l_decision);
            hxc_record_50dff1c7_destroy(&hxc_l_state);
            hxc_array_3767ae6c_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n9);
            if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
            {
              abort();
            }
            return hxc_l_tmp_instance_call_result_n39;
          }
          break;
        }
      case hxc_caxecraft_domain_ActorControllerEvent_DropRequested:
        {
          break;
        }
      default:
        {
          abort();
        }
    }
    int32_t hxc_l_tmp_record_field_load_result_n40 = hxc_l_state.hxc_characterId;
    struct hxc_caxecraft_domain_CharacterCommandStepResult hxc_l_tmp_instance_call_result_n42 = hxc_caxecraft_domain_GameSession_stepCharacter(hxc_l_self, hxc_l_tmp_record_field_load_result_n40, hxc_l_intent, hxc_l_damagePolicy);
    struct hxc_caxecraft_domain_CharacterCommandStepResult hxc_l_step = hxc_l_tmp_instance_call_result_n42;
    if (!hxc_l_step.hxc_resolved)
    {
      int32_t hxc_l_tmp_record_field_load_result_n44 = hxc_l_state.hxc_characterId;
      struct hxc_caxecraft_domain_ActorControllerTickResult hxc_l_tmp_instance_call_result_n47 = hxc_caxecraft_domain_GameSession_actorControllerTick(hxc_l_self, (struct hxc_caxecraft_domain_ActorControllerTickStatus){ .hxc_tag = hxc_caxecraft_domain_ActorControllerTickStatus_CharacterCommandRejected, .hxc_payload.hxc_CharacterCommandRejected.hxc_characterId = hxc_l_tmp_record_field_load_result_n44 }, hxc_l_processed);
      hxc_enum_8b2a26b8_destroy(&hxc_l_event);
      hxc_record_50dff1c7_destroy(&hxc_l_next);
      hxc_enum_8199493a_destroy(&hxc_l_decision);
      hxc_record_50dff1c7_destroy(&hxc_l_state);
      hxc_array_3767ae6c_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n9);
      if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
      {
        abort();
      }
      return hxc_l_tmp_instance_call_result_n47;
    }
    struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n48 = (*hxc_l_self).hxc_actorControllers;
    int32_t hxc_l_tmp_load_result_n49 = hxc_l_index;
    if (hxc_array_ref_set_copy(hxc_l_tmp_class_field_load_result_n48, (size_t)hxc_l_tmp_load_result_n49, &hxc_l_next) != HXC_STATUS_OK)
    {
      abort();
    }
    (void)hxc_l_next;
    switch (hxc_l_event.hxc_tag) {
      case hxc_caxecraft_domain_ActorControllerEvent_NoControllerEvent:
        {
          break;
        }
      case hxc_caxecraft_domain_ActorControllerEvent_InteractionAvailable:
        {
          struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n53 = (*hxc_l_self).hxc_actorControllerEvents;
          int32_t hxc_l_tmp_array_push_result_n55;
          if (hxc_array_ref_push_copy(hxc_l_tmp_class_field_load_result_n53, &hxc_l_event, &hxc_l_tmp_array_push_result_n55) != HXC_STATUS_OK)
          {
            abort();
          }
          (void)hxc_l_tmp_array_push_result_n55;
          break;
        }
      case hxc_caxecraft_domain_ActorControllerEvent_LocalPlayerAttack:
        {
          struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n56 = (*hxc_l_self).hxc_actorControllerEvents;
          int32_t hxc_l_tmp_array_push_result_n58;
          if (hxc_array_ref_push_copy(hxc_l_tmp_class_field_load_result_n56, &hxc_l_event, &hxc_l_tmp_array_push_result_n58) != HXC_STATUS_OK)
          {
            abort();
          }
          (void)hxc_l_tmp_array_push_result_n58;
          break;
        }
      case hxc_caxecraft_domain_ActorControllerEvent_DropRequested:
        {
          struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n59 = (*hxc_l_self).hxc_actorControllerEvents;
          int32_t hxc_l_tmp_array_push_result_n61;
          if (hxc_array_ref_push_copy(hxc_l_tmp_class_field_load_result_n59, &hxc_l_event, &hxc_l_tmp_array_push_result_n61) != HXC_STATUS_OK)
          {
            abort();
          }
          (void)hxc_l_tmp_array_push_result_n61;
          break;
        }
      default:
        {
          abort();
        }
    }
    hxc_l_processed = hxc_i32_add_wrapping(hxc_l_processed, 1);
    hxc_enum_8b2a26b8_destroy(&hxc_l_event);
    hxc_record_50dff1c7_destroy(&hxc_l_next);
    hxc_enum_8199493a_destroy(&hxc_l_decision);
    hxc_record_50dff1c7_destroy(&hxc_l_state);
    hxc_array_3767ae6c_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n9);
  }
  struct hxc_caxecraft_domain_ActorControllerTickResult hxc_l_tmp_instance_call_result_n72 = hxc_caxecraft_domain_GameSession_actorControllerTick(hxc_l_self, (struct hxc_caxecraft_domain_ActorControllerTickStatus){ .hxc_tag = hxc_caxecraft_domain_ActorControllerTickStatus_ControllersAdvanced }, hxc_l_processed);
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return hxc_l_tmp_instance_call_result_n72;
}

struct hxc_caxecraft_domain_CharacterCommandStepResult hxc_caxecraft_domain_GameSession_stepCharacter(struct hxc_caxecraft_domain_GameSession *hxc_l_self, int32_t hxc_l_id, struct hxc_caxecraft_domain_CharacterIntent hxc_l_intent, int32_t hxc_l_damagePolicy)
{
  const void *hxc_l_gc_roots[1] = { (const void *)hxc_l_self };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 1, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_l_self == NULL)
  {
    abort();
  }
  const uint8_t *hxc_l_readCells = (*hxc_l_self).hxc_worldStorage;
  size_t hxc_l_tmp_length_n4 = 16384;
  struct hxc_caxecraft_domain_EntityStore *hxc_l_tmp_owned_class_field_address_n0 = &(*hxc_l_self).hxc_entities;
  if (hxc_l_tmp_owned_class_field_address_n0 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n1 = hxc_caxecraft_domain_EntityStore_read(hxc_l_tmp_owned_class_field_address_n0, hxc_l_id);
  struct hxc_caxecraft_domain_CharacterCommandStepResult hxc_l_tmp_instance_call_result_n2 = hxc_caxecraft_domain_GameSession_stepStoredCharacter(hxc_l_self, hxc_l_readCells, hxc_l_tmp_length_n4, hxc_l_tmp_instance_call_result_n1, hxc_l_intent, hxc_l_damagePolicy);
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return hxc_l_tmp_instance_call_result_n2;
}

struct hxc_caxecraft_domain_CharacterCommandStepResult hxc_caxecraft_domain_GameSession_stepStoredCharacter(struct hxc_caxecraft_domain_GameSession *hxc_l_self, const uint8_t *hxc_l_readCells, size_t hxc_l_length, struct hxc_caxecraft_domain_Character hxc_l_original, struct hxc_caxecraft_domain_CharacterIntent hxc_l_intent, int32_t hxc_l_damagePolicy)
{
  const void *hxc_l_gc_roots[1] = { (const void *)hxc_l_self };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 1, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  const uint8_t *hxc_l_borrow = hxc_l_readCells;
  size_t hxc_l_tmp_length_n5 = hxc_l_length;
  (void)hxc_l_borrow;
  (void)hxc_l_tmp_length_n5;
  if (hxc_l_original.hxc_id <= 0)
  {
    struct hxc_caxecraft_domain_Immersion hxc_l_tmp_call_result_n2 = hxc_caxecraft_domain_Aquatics_observe(hxc_l_readCells, hxc_l_length, hxc_l_original.hxc_body);
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return (struct hxc_caxecraft_domain_CharacterCommandStepResult){ .hxc_character = hxc_l_original, .hxc_drowningDamage = 0, .hxc_immersion = hxc_l_tmp_call_result_n2, .hxc_resolved = false };
  }
  struct hxc_caxecraft_domain_CharacterStep hxc_l_tmp_call_result_n4 = hxc_caxecraft_domain_Character_step(hxc_l_readCells, hxc_l_length, hxc_l_original, hxc_l_intent, hxc_l_damagePolicy);
  struct hxc_caxecraft_domain_CharacterStep hxc_l_result = hxc_l_tmp_call_result_n4;
  if (hxc_l_self == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_domain_EntityStore *hxc_l_tmp_owned_class_field_address_n5 = &(*hxc_l_self).hxc_entities;
  if (hxc_l_tmp_owned_class_field_address_n5 == NULL)
  {
    abort();
  }
  bool hxc_l_tmp_instance_call_result_n8 = hxc_caxecraft_domain_EntityStore_replace(hxc_l_tmp_owned_class_field_address_n5, hxc_l_original.hxc_id, hxc_l_result.hxc_character);
  bool hxc_l_resolved = hxc_l_tmp_instance_call_result_n8;
  bool hxc_l_tmp_load_result_n9 = hxc_l_resolved;
  struct hxc_caxecraft_domain_Character hxc_l_tmp_conditional_result_n8 = { 0 };
  if (hxc_l_tmp_load_result_n9)
  {
    hxc_l_tmp_conditional_result_n8 = hxc_l_result.hxc_character;
  }
  else
  {
    hxc_l_tmp_conditional_result_n8 = hxc_l_original;
  }
  struct hxc_caxecraft_domain_Character hxc_l_tmp_conditional_load_result_n11 = hxc_l_tmp_conditional_result_n8;
  struct hxc_caxecraft_domain_Immersion hxc_l_tmp_record_field_load_result_n12 = hxc_l_result.hxc_immersion;
  int32_t hxc_l_tmp_record_field_load_result_n13 = hxc_l_result.hxc_drowningDamage;
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return (struct hxc_caxecraft_domain_CharacterCommandStepResult){ .hxc_character = hxc_l_tmp_conditional_load_result_n11, .hxc_drowningDamage = hxc_l_tmp_record_field_load_result_n13, .hxc_immersion = hxc_l_tmp_record_field_load_result_n12, .hxc_resolved = hxc_l_resolved };
}

struct hxc_caxecraft_domain_GameTickResult hxc_caxecraft_domain_GameSession_tick(struct hxc_caxecraft_domain_GameSession *hxc_l_self, struct hxc_caxecraft_domain_GameTickInput hxc_l_input)
{
  const void *hxc_l_gc_roots[5] = { (const void *)hxc_l_self, NULL, NULL, NULL, NULL };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 5, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_optional_caxecraft_scenario_FlowTickResult hxc_l_flowResult = { 0 };
  struct hxc_caxecraft_scenario_FlowTickInput hxc_l_tmp_instance_call_argument_0_owner_n14 = { 0 };
  if (hxc_l_self == NULL)
  {
    abort();
  }
  uint8_t *hxc_l_cells = (*hxc_l_self).hxc_worldStorage;
  size_t hxc_l_tmp_length_n2 = 16384;
  const uint8_t *hxc_l_readCells = (*hxc_l_self).hxc_worldStorage;
  size_t hxc_l_tmp_length_n3 = 16384;
  struct hxc_caxecraft_domain_PlayerAgent hxc_l_tmp_class_field_load_result_n0 = (*hxc_l_self).hxc_localPlayer;
  int32_t hxc_l_characterId = hxc_l_tmp_class_field_load_result_n0.hxc_characterId;
  struct hxc_caxecraft_domain_EntityStore *hxc_l_tmp_owned_class_field_address_n2 = &(*hxc_l_self).hxc_entities;
  if (hxc_l_tmp_owned_class_field_address_n2 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n4 = hxc_caxecraft_domain_EntityStore_read(hxc_l_tmp_owned_class_field_address_n2, hxc_l_characterId);
  struct hxc_caxecraft_domain_Character hxc_l_original = hxc_l_tmp_instance_call_result_n4;
  if (hxc_l_original.hxc_id <= 0)
  {
    struct hxc_caxecraft_domain_Character hxc_l_tmp_load_result_n6 = hxc_l_original;
    struct hxc_caxecraft_domain_Immersion hxc_l_tmp_call_result_n8 = hxc_caxecraft_domain_Aquatics_observe(hxc_l_readCells, hxc_l_tmp_length_n3, hxc_l_original.hxc_body);
    struct hxc_caxecraft_domain_WaterSimulation *hxc_l_tmp_owned_class_field_address_n9 = &(*hxc_l_self).hxc_water;
    int32_t hxc_l_tmp_class_field_load_result_n10 = (*hxc_l_tmp_owned_class_field_address_n9).hxc_pendingCount;
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return (struct hxc_caxecraft_domain_GameTickResult){ .hxc_character = hxc_l_tmp_load_result_n6, .hxc_committed = false, .hxc_drowningDamage = 0, .hxc_flow = (struct hxc_optional_caxecraft_scenario_FlowTickResult){ .hxc_has_value = false }, .hxc_immersion = hxc_l_tmp_call_result_n8, .hxc_tickIndex = -1, .hxc_water = (struct hxc_caxecraft_domain_WaterTickResult){ .hxc_changed = 0, .hxc_processed = 0, .hxc_remaining = hxc_l_tmp_class_field_load_result_n10 } };
  }
  struct hxc_caxecraft_domain_WaterSimulation *hxc_l_tmp_owned_class_field_address_n13 = &(*hxc_l_self).hxc_water;
  if (hxc_l_tmp_owned_class_field_address_n13 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_domain_WaterTickResult hxc_l_tmp_instance_call_result_n15 = hxc_caxecraft_domain_WaterSimulation_tick(hxc_l_tmp_owned_class_field_address_n13, hxc_l_cells, hxc_l_tmp_length_n2, hxc_l_input.hxc_waterUpdateBudget);
  struct hxc_caxecraft_domain_WaterTickResult hxc_l_waterResult = hxc_l_tmp_instance_call_result_n15;
  struct hxc_caxecraft_domain_CharacterCommandStepResult hxc_l_tmp_instance_call_result_n19 = hxc_caxecraft_domain_GameSession_stepStoredCharacter(hxc_l_self, hxc_l_readCells, hxc_l_tmp_length_n3, hxc_l_original, hxc_l_input.hxc_intent, hxc_l_input.hxc_damagePolicy);
  struct hxc_caxecraft_domain_CharacterCommandStepResult hxc_l_characterResult = hxc_l_tmp_instance_call_result_n19;
  bool hxc_l_committed = hxc_l_characterResult.hxc_resolved;
  bool hxc_l_tmp_load_result_n21 = hxc_l_committed;
  int32_t hxc_l_tmp_conditional_result_n10 = 0;
  if (hxc_l_tmp_load_result_n21)
  {
    int32_t hxc_l_tmp_class_field_load_result_n22 = (*hxc_l_self).hxc_completedTicks;
    hxc_l_tmp_conditional_result_n10 = hxc_l_tmp_class_field_load_result_n22;
  }
  else
  {
    hxc_l_tmp_conditional_result_n10 = -1;
  }
  int32_t hxc_l_tickIndex = hxc_l_tmp_conditional_result_n10;
  if (hxc_l_committed)
  {
    int32_t hxc_l_tmp_increment_load_result_n25 = (*hxc_l_self).hxc_completedTicks;
    (*hxc_l_self).hxc_completedTicks = hxc_i32_add_wrapping(hxc_l_tmp_increment_load_result_n25, 1);
  }
  hxc_l_flowResult = (struct hxc_optional_caxecraft_scenario_FlowTickResult){ .hxc_has_value = false };
  if (hxc_l_committed)
  {
    struct hxc_caxecraft_scenario_CaxeFlowExecutor *hxc_l_tmp_class_field_load_result_n27 = (*hxc_l_self).hxc_flowExecutor;
    hxc_l_gc_roots[1] = (const void *)hxc_l_tmp_class_field_load_result_n27;
    struct hxc_caxecraft_scenario_CaxeFlowExecutor *hxc_l_executor = hxc_l_tmp_class_field_load_result_n27;
    hxc_l_gc_roots[2] = (const void *)hxc_l_executor;
    hxc_l_gc_roots[3] = (const void *)NULL;
    if (hxc_l_executor != NULL)
    {
      struct hxc_caxecraft_scenario_CaxeFlowExecutor *hxc_l_tmp_load_result_n29 = hxc_l_executor;
      hxc_l_gc_roots[4] = (const void *)hxc_l_tmp_load_result_n29;
      if (hxc_l_tmp_load_result_n29 == NULL)
      {
        abort();
      }
      struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n30 = (*hxc_l_self).hxc_pendingFlowEvents;
      struct hxc_array_ref *hxc_l_tmp_record_field_events_owner_n13 = hxc_l_tmp_class_field_load_result_n30;
      if (hxc_array_ref_retain(hxc_l_tmp_record_field_events_owner_n13) != HXC_STATUS_OK)
      {
        abort();
      }
      struct hxc_array_ref *hxc_l_tmp_record_field_events_owned_load_result_n31 = hxc_l_tmp_record_field_events_owner_n13;
      struct hxc_array_ref *hxc_l_tmp_array_create_result_n32 = NULL;
      if (hxc_array_ref_create(hxc_default_allocator(), (hxc_array_element_ops){ sizeof(struct hxc_caxecraft_scenario_FlowPosition), _Alignof(struct hxc_caxecraft_scenario_FlowPosition), NULL, hxc_array_456c7a78_element_copy, hxc_array_456c7a78_element_assign, hxc_array_456c7a78_element_destroy }, &hxc_l_tmp_array_create_result_n32) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_l_tmp_instance_call_argument_0_owner_n14 = (struct hxc_caxecraft_scenario_FlowTickInput){ .hxc_events = hxc_l_tmp_record_field_events_owned_load_result_n31, .hxc_positions = hxc_l_tmp_array_create_result_n32 };
      struct hxc_caxecraft_scenario_FlowTickResult hxc_l_tmp_instance_call_result_n35 = hxc_caxecraft_scenario_CaxeFlowExecutor_runTick(hxc_l_tmp_load_result_n29, hxc_l_tmp_instance_call_argument_0_owner_n14);
      hxc_optional_b8d3265c_destroy(&hxc_l_flowResult);
      hxc_l_flowResult = (struct hxc_optional_caxecraft_scenario_FlowTickResult){ .hxc_has_value = true, .hxc_value = hxc_l_tmp_instance_call_result_n35 };
      struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n37 = (*hxc_l_self).hxc_pendingFlowEvents;
      if (hxc_l_tmp_class_field_load_result_n37 == NULL)
      {
        abort();
      }
      if (hxc_array_resize(&hxc_l_tmp_class_field_load_result_n37->value, (size_t)0, NULL) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_record_a1b589a4_destroy(&hxc_l_tmp_instance_call_argument_0_owner_n14);
    }
  }
  int32_t hxc_l_tmp_load_result_n38 = hxc_l_tickIndex;
  struct hxc_caxecraft_domain_Character hxc_l_tmp_record_field_load_result_n39 = hxc_l_characterResult.hxc_character;
  struct hxc_caxecraft_domain_Immersion hxc_l_tmp_record_field_load_result_n40 = hxc_l_characterResult.hxc_immersion;
  int32_t hxc_l_tmp_record_field_load_result_n41 = hxc_l_characterResult.hxc_drowningDamage;
  struct hxc_caxecraft_domain_WaterTickResult hxc_l_tmp_load_result_n42 = hxc_l_waterResult;
  struct hxc_optional_caxecraft_scenario_FlowTickResult hxc_l_tmp_record_field_flow_owner_n15 = hxc_l_flowResult;
  if (hxc_optional_b8d3265c_retain(&hxc_l_tmp_record_field_flow_owner_n15) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_optional_caxecraft_scenario_FlowTickResult hxc_l_tmp_record_field_flow_owned_load_result_n44 = hxc_l_tmp_record_field_flow_owner_n15;
  bool hxc_l_tmp_load_result_n45 = hxc_l_committed;
  hxc_optional_b8d3265c_destroy(&hxc_l_flowResult);
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return (struct hxc_caxecraft_domain_GameTickResult){ .hxc_character = hxc_l_tmp_record_field_load_result_n39, .hxc_committed = hxc_l_tmp_load_result_n45, .hxc_drowningDamage = hxc_l_tmp_record_field_load_result_n41, .hxc_flow = hxc_l_tmp_record_field_flow_owned_load_result_n44, .hxc_immersion = hxc_l_tmp_record_field_load_result_n40, .hxc_tickIndex = hxc_l_tmp_load_result_n38, .hxc_water = hxc_l_tmp_load_result_n42 };
}

struct hxc_caxecraft_domain_LocalRecoveryResult hxc_caxecraft_domain_GameSession_useSelectedRecovery(struct hxc_caxecraft_domain_GameSession *hxc_l_self, struct hxc_caxecraft_gameplay_InventoryState hxc_l_inventory)
{
  const void *hxc_l_gc_roots[1] = { (const void *)hxc_l_self };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 1, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_l_self == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_domain_EntityStore *hxc_l_tmp_owned_class_field_address_n0 = &(*hxc_l_self).hxc_entities;
  if (hxc_l_tmp_owned_class_field_address_n0 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_domain_PlayerAgent hxc_l_tmp_class_field_load_result_n1 = (*hxc_l_self).hxc_localPlayer;
  struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n3 = hxc_caxecraft_domain_EntityStore_read(hxc_l_tmp_owned_class_field_address_n0, hxc_l_tmp_class_field_load_result_n1.hxc_characterId);
  struct hxc_caxecraft_domain_Character hxc_l_original = hxc_l_tmp_instance_call_result_n3;
  if (hxc_l_original.hxc_id <= 0)
  {
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return (struct hxc_caxecraft_domain_LocalRecoveryResult){ .hxc_character = hxc_l_original, .hxc_decision = 0, .hxc_inventory = hxc_l_inventory, .hxc_resolved = false };
  }
  int32_t hxc_l_tmp_call_result_n8 = hxc_caxecraft_gameplay_Recovery_decide(hxc_l_inventory, hxc_l_original.hxc_vitals);
  int32_t hxc_l_decision = hxc_l_tmp_call_result_n8;
  if (hxc_l_decision != 1)
  {
    int32_t hxc_l_tmp_load_result_n10 = hxc_l_decision;
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return (struct hxc_caxecraft_domain_LocalRecoveryResult){ .hxc_character = hxc_l_original, .hxc_decision = hxc_l_tmp_load_result_n10, .hxc_inventory = hxc_l_inventory, .hxc_resolved = true };
  }
  struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n14 = hxc_caxecraft_gameplay_Recovery_applyInventory(hxc_l_decision, hxc_l_inventory);
  struct hxc_caxecraft_gameplay_InventoryState hxc_l_nextInventory = hxc_l_tmp_call_result_n14;
  struct hxc_caxecraft_domain_Character hxc_l_tmp_load_result_n15 = hxc_l_original;
  int32_t hxc_l_tmp_load_result_n16 = hxc_l_decision;
  struct hxc_caxecraft_domain_VitalsState hxc_l_tmp_call_result_n18 = hxc_caxecraft_gameplay_Recovery_applyVitals(hxc_l_tmp_load_result_n16, hxc_l_original.hxc_vitals);
  struct hxc_caxecraft_domain_Character hxc_l_tmp_call_result_n19 = hxc_caxecraft_domain_Character_withVitals(hxc_l_tmp_load_result_n15, hxc_l_tmp_call_result_n18);
  struct hxc_caxecraft_domain_Character hxc_l_nextCharacter = hxc_l_tmp_call_result_n19;
  struct hxc_caxecraft_domain_Character hxc_l_tmp_load_result_n20 = hxc_l_original;
  struct hxc_caxecraft_domain_LocalCharacterCommandResult hxc_l_tmp_instance_call_result_n22 = hxc_caxecraft_domain_GameSession_commitLocalCharacter(hxc_l_self, hxc_l_tmp_load_result_n20, hxc_l_nextCharacter);
  struct hxc_caxecraft_domain_LocalCharacterCommandResult hxc_l_committed = hxc_l_tmp_instance_call_result_n22;
  int32_t hxc_l_tmp_record_field_decision_n7 = hxc_l_decision;
  bool hxc_l_tmp_record_field_load_result_n24 = hxc_l_committed.hxc_resolved;
  struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_conditional_result_n8 = { 0 };
  if (hxc_l_tmp_record_field_load_result_n24)
  {
    hxc_l_tmp_conditional_result_n8 = hxc_l_nextInventory;
  }
  else
  {
    hxc_l_tmp_conditional_result_n8 = hxc_l_inventory;
  }
  struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_conditional_load_result_n26 = hxc_l_tmp_conditional_result_n8;
  struct hxc_caxecraft_domain_Character hxc_l_tmp_record_field_load_result_n27 = hxc_l_committed.hxc_character;
  bool hxc_l_tmp_record_field_load_result_n28 = hxc_l_committed.hxc_resolved;
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return (struct hxc_caxecraft_domain_LocalRecoveryResult){ .hxc_character = hxc_l_tmp_record_field_load_result_n27, .hxc_decision = hxc_l_tmp_record_field_decision_n7, .hxc_inventory = hxc_l_tmp_conditional_load_result_n26, .hxc_resolved = hxc_l_tmp_record_field_load_result_n28 };
}

struct hxc_caxecraft_domain_GameView hxc_caxecraft_domain_GameSession_view(struct hxc_caxecraft_domain_GameSession *hxc_l_self)
{
  const void *hxc_l_gc_roots[1] = { (const void *)hxc_l_self };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 1, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_l_self == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_domain_EntityStore *hxc_l_tmp_owned_class_field_address_n0 = &(*hxc_l_self).hxc_entities;
  if (hxc_l_tmp_owned_class_field_address_n0 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_domain_PlayerAgent hxc_l_tmp_class_field_load_result_n1 = (*hxc_l_self).hxc_localPlayer;
  struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n3 = hxc_caxecraft_domain_EntityStore_read(hxc_l_tmp_owned_class_field_address_n0, hxc_l_tmp_class_field_load_result_n1.hxc_characterId);
  struct hxc_caxecraft_domain_Character hxc_l_character = hxc_l_tmp_instance_call_result_n3;
  int32_t hxc_l_tmp_record_field_load_result_n4 = hxc_l_character.hxc_id;
  struct hxc_caxecraft_domain_Character hxc_l_tmp_load_result_n5 = hxc_l_character;
  int32_t hxc_l_tmp_class_field_load_result_n6 = (*hxc_l_self).hxc_completedTicks;
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return (struct hxc_caxecraft_domain_GameView){ .hxc_completedTicks = hxc_l_tmp_class_field_load_result_n6, .hxc_localPlayer = hxc_l_tmp_load_result_n5, .hxc_valid = hxc_l_tmp_record_field_load_result_n4 > 0 };
}

const uint8_t *hxc_caxecraft_domain_GameSession_worldView(struct hxc_caxecraft_domain_GameSession *hxc_l_self, size_t *hxc_l_returned_span_length)
{
  const void *hxc_l_gc_roots[1] = { (const void *)hxc_l_self };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 1, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_l_self == NULL)
  {
    abort();
  }
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  *hxc_l_returned_span_length = 16384;
  return (*hxc_l_self).hxc_worldStorage;
}

int32_t hxc_caxecraft_domain_GameSession_writeTerrainRunDuringLoad(struct hxc_caxecraft_domain_GameSession *hxc_l_self, int32_t hxc_l_startIndex, int32_t hxc_l_storageCode, int32_t hxc_l_count)
{
  const void *hxc_l_gc_roots[1] = { (const void *)hxc_l_self };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 1, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  bool hxc_l_tmp_short_circuit_result_n4 = hxc_l_startIndex < 0;
  if (!(hxc_l_startIndex < 0))
  {
    hxc_l_tmp_short_circuit_result_n4 = hxc_l_count <= 0;
  }
  bool hxc_l_tmp_short_circuit_load_result_n0 = hxc_l_tmp_short_circuit_result_n4;
  bool hxc_l_tmp_short_circuit_result_n5 = hxc_l_tmp_short_circuit_load_result_n0;
  if (!hxc_l_tmp_short_circuit_load_result_n0)
  {
    hxc_l_tmp_short_circuit_result_n5 = hxc_l_startIndex > hxc_i32_subtract_wrapping(16384, hxc_l_count);
  }
  bool hxc_l_tmp_short_circuit_load_result_n1 = hxc_l_tmp_short_circuit_result_n5;
  bool hxc_l_tmp_short_circuit_result_n6 = hxc_l_tmp_short_circuit_load_result_n1;
  if (!hxc_l_tmp_short_circuit_load_result_n1)
  {
    bool hxc_l_tmp_short_circuit_result_n7 = hxc_l_storageCode != 0;
    if (hxc_l_storageCode != 0)
    {
      bool hxc_l_tmp_short_circuit_result_n8 = hxc_l_storageCode >= 1;
      if (hxc_l_storageCode >= 1)
      {
        hxc_l_tmp_short_circuit_result_n8 = hxc_l_storageCode <= 9;
      }
      hxc_l_tmp_short_circuit_result_n7 = !hxc_l_tmp_short_circuit_result_n8;
    }
    hxc_l_tmp_short_circuit_result_n6 = hxc_l_tmp_short_circuit_result_n7;
  }
  if (hxc_l_tmp_short_circuit_result_n6)
  {
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return -1;
  }
  if (hxc_l_self == NULL)
  {
    abort();
  }
  uint8_t *hxc_l_cells = (*hxc_l_self).hxc_worldStorage;
  size_t hxc_l_tmp_length_n9 = 16384;
  int32_t hxc_l_end = hxc_i32_add_wrapping(hxc_l_startIndex, hxc_l_count);
  int32_t hxc_l_index = hxc_l_startIndex;
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n5 = hxc_l_index;
    if (!(hxc_l_tmp_load_result_n5 < hxc_l_end))
    {
      break;
    }
    hxc_caxecraft_domain_WorldStorage_writeCode(hxc_l_cells, hxc_l_tmp_length_n9, hxc_l_index, hxc_l_storageCode);
    hxc_l_index = hxc_i32_add_wrapping(hxc_l_index, 1);
  }
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return hxc_l_end;
}
