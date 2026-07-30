#include "hxc/program.h"

struct hxc_caxecraft_content_FirstPlayableSessionLoad hxc_caxecraft_content_FirstPlayableSessionLoader_loadCandidate(struct hxc_caxecraft_domain_GameSession *hxc_session, int32_t hxc_initialHealth)
{
  const void *hxc_gc_roots[2] = { NULL, NULL };
  struct hxc_gc_root_frame hxc_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_gc_roots, 2, &hxc_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_caxecraft_content_CharacterSpawnPlan hxc_actor = { 0 };
  struct hxc_caxecraft_content_CharacterSpawnPlan hxc_tmp_array_element_owner_n11 = { 0 };
  struct hxc_caxecraft_content_LoadedActorBinding hxc_tmp_array_push_element_owner_n14 = { 0 };
  struct hxc_array_ref *hxc_tmp_record_field_actors_owner_n23 = { 0 };
  struct hxc_caxecraft_content_ActorCompositionResult hxc_g_he441670ec403 = { 0 };
  struct hxc_array_ref *hxc_tmp_static_call_argument_0_owner_n5 = { 0 };
  struct hxc_array_ref *hxc_tmp_managed_flow_owner_n7 = { 0 };
  struct hxc_array_ref *hxc_actorBindings = { 0 };
  int32_t hxc_tmp_call_result_n0 = hxc_caxecraft_content_FirstPlayableSessionLoader_validateFacts(hxc_initialHealth);
  int32_t hxc_presentationCell = hxc_tmp_call_result_n0;
  if (hxc_presentationCell < 0)
  {
    struct hxc_array_ref *hxc_tmp_array_create_result_n2 = NULL;
    if (hxc_array_ref_create(hxc_default_allocator(), (hxc_array_element_ops){ sizeof(struct hxc_caxecraft_content_LoadedActorBinding), _Alignof(struct hxc_caxecraft_content_LoadedActorBinding), NULL, hxc_array_7c0765de_element_copy, hxc_array_7c0765de_element_assign, hxc_array_7c0765de_element_destroy }, &hxc_tmp_array_create_result_n2) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_gc_root_frame_pop(&hxc_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return (struct hxc_caxecraft_content_FirstPlayableSessionLoad){ .hxc_actors = hxc_tmp_array_create_result_n2, .hxc_valid = false, .hxc_waterPresentationCell = -1 };
  }
  struct hxc_array_ref *hxc_actorPlans;
  struct hxc_array_ref *hxc_tmp_call_result_n4 = hxc_caxecraft_content_FirstPlayableLevel_actorObjects();
  hxc_tmp_static_call_argument_0_owner_n5 = hxc_tmp_call_result_n4;
  struct hxc_array_ref *hxc_tmp_static_call_argument_0_borrow_result_n5 = hxc_tmp_static_call_argument_0_owner_n5;
  struct hxc_caxecraft_content_BaseContentRegistry *hxc_tmp_managed_class_result_n6 = NULL;
  if (hxc_gc_allocate(&hxc_program_gc, &hxc_caxecraft_content_BaseContentRegistry_descriptor, (void **)&hxc_tmp_managed_class_result_n6) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_gc_roots[0] = (const void *)hxc_tmp_managed_class_result_n6;
  hxc_gc_roots[1] = (const void *)hxc_tmp_managed_class_result_n6;
  if (hxc_tmp_managed_class_result_n6 == NULL)
  {
    abort();
  }
  struct hxc_compiler_interface_dispatch_caxecraft_content_ActorContentResolver_value hxc_tmp_interface_value_n7 = (struct hxc_compiler_interface_dispatch_caxecraft_content_ActorContentResolver_value){ .object = hxc_tmp_managed_class_result_n6, .table = &hxc_itable_compiler_interface_dispatch_caxecraft_content_BaseContentRegistry_itable_layout_caxecraft_c_h846983c3b6751ecb };
  struct hxc_caxecraft_content_ActorCompositionResult hxc_tmp_call_result_n8 = hxc_caxecraft_content_ActorCompositionPlanner_planActorComposition(hxc_tmp_static_call_argument_0_borrow_result_n5, hxc_tmp_interface_value_n7);
  hxc_g_he441670ec403 = hxc_tmp_call_result_n8;
  switch (hxc_g_he441670ec403.hxc_tag) {
    case hxc_caxecraft_content_ActorCompositionResult_ActorCompositionPlanned:
      {
        break;
      }
    case hxc_caxecraft_content_ActorCompositionResult_ActorCompositionRejected:
      {
        struct hxc_array_ref *hxc_tmp_array_create_result_n13 = NULL;
        if (hxc_array_ref_create(hxc_default_allocator(), (hxc_array_element_ops){ sizeof(struct hxc_caxecraft_content_LoadedActorBinding), _Alignof(struct hxc_caxecraft_content_LoadedActorBinding), NULL, hxc_array_7c0765de_element_copy, hxc_array_7c0765de_element_assign, hxc_array_7c0765de_element_destroy }, &hxc_tmp_array_create_result_n13) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_enum_65c03c54_destroy(&hxc_g_he441670ec403);
        if (hxc_array_ref_release(hxc_tmp_static_call_argument_0_owner_n5) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_gc_root_frame_pop(&hxc_gc_frame) != HXC_STATUS_OK)
        {
          abort();
        }
        return (struct hxc_caxecraft_content_FirstPlayableSessionLoad){ .hxc_actors = hxc_tmp_array_create_result_n13, .hxc_valid = false, .hxc_waterPresentationCell = -1 };
      }
  }
  if (hxc_g_he441670ec403.hxc_tag != hxc_caxecraft_content_ActorCompositionResult_ActorCompositionPlanned)
  {
    abort();
  }
  struct hxc_array_ref *hxc_tmp_enum_payload_project_n11 = hxc_g_he441670ec403.hxc_payload.hxc_ActorCompositionPlanned.hxc_plans;
  struct hxc_array_ref *hxc_plans = hxc_tmp_enum_payload_project_n11;
  (void)hxc_plans;
  hxc_actorPlans = hxc_plans;
  if (hxc_array_ref_retain(hxc_actorPlans) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_array_ref *hxc_tmp_managed_flow_carrier_move_result_n15 = hxc_actorPlans;
  hxc_tmp_managed_flow_owner_n7 = hxc_tmp_managed_flow_carrier_move_result_n15;
  struct hxc_array_ref *hxc_tmp_array_create_result_n16 = NULL;
  if (hxc_array_ref_create(hxc_default_allocator(), (hxc_array_element_ops){ sizeof(struct hxc_caxecraft_content_LoadedActorBinding), _Alignof(struct hxc_caxecraft_content_LoadedActorBinding), NULL, hxc_array_7c0765de_element_copy, hxc_array_7c0765de_element_assign, hxc_array_7c0765de_element_destroy }, &hxc_tmp_array_create_result_n16) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_actorBindings = hxc_tmp_array_create_result_n16;
  int32_t hxc_g_hfd3f23e9ac63 = 0;
  while (1)
  {
    int32_t hxc_tmp_load_result_n17 = hxc_g_hfd3f23e9ac63;
    int32_t hxc_tmp_array_length_result_n19;
    if (hxc_array_ref_length(hxc_tmp_managed_flow_owner_n7, &hxc_tmp_array_length_result_n19) != HXC_STATUS_OK)
    {
      abort();
    }
    if (!(hxc_tmp_load_result_n17 < hxc_tmp_array_length_result_n19))
    {
      break;
    }
    struct hxc_array_ref *hxc_tmp_managed_flow_owner_load_result_n20 = hxc_tmp_managed_flow_owner_n7;
    struct hxc_caxecraft_content_CharacterSpawnPlan hxc_tmp_array_get_result_n22;
    if (hxc_array_ref_get_copy(hxc_tmp_managed_flow_owner_load_result_n20, (size_t)hxc_g_hfd3f23e9ac63, &hxc_tmp_array_get_result_n22) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_tmp_array_element_owner_n11 = hxc_tmp_array_get_result_n22;
    hxc_actor = hxc_tmp_array_element_owner_n11;
    if (hxc_record_831d5670_retain(&hxc_actor) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_g_hfd3f23e9ac63 = hxc_i32_add_wrapping(hxc_g_hfd3f23e9ac63, 1);
    struct hxc_array_ref *hxc_tmp_load_result_n25 = hxc_actorBindings;
    hxc_string hxc_tmp_record_field_authoredId_owner_n12 = hxc_actor.hxc_authoredId;
    if (hxc_string_retain(hxc_tmp_record_field_authoredId_owner_n12) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_string hxc_tmp_record_field_authoredId_owned_load_result_n27 = hxc_tmp_record_field_authoredId_owner_n12;
    int32_t hxc_tmp_record_field_load_result_n28 = hxc_actor.hxc_entityId;
    struct hxc_caxecraft_content_CharacterSpawnRole hxc_tmp_record_field_role_owner_n13 = hxc_actor.hxc_role;
    if (hxc_enum_b133bfa7_retain(&hxc_tmp_record_field_role_owner_n13) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_tmp_array_push_element_owner_n14 = (struct hxc_caxecraft_content_LoadedActorBinding){ .hxc_authoredId = hxc_tmp_record_field_authoredId_owned_load_result_n27, .hxc_entityId = hxc_tmp_record_field_load_result_n28, .hxc_role = hxc_tmp_record_field_role_owner_n13 };
    int32_t hxc_tmp_array_push_result_n33;
    if (hxc_array_ref_push_copy(hxc_tmp_load_result_n25, &hxc_tmp_array_push_element_owner_n14, &hxc_tmp_array_push_result_n33) != HXC_STATUS_OK)
    {
      abort();
    }
    (void)hxc_tmp_array_push_result_n33;
    hxc_record_8de0419a_destroy(&hxc_tmp_array_push_element_owner_n14);
    hxc_record_831d5670_destroy(&hxc_actor);
    hxc_array_78e5c5d1_element_destroy(NULL, &hxc_tmp_array_element_owner_n11);
  }
  if (hxc_session == NULL)
  {
    abort();
  }
  hxc_caxecraft_domain_GameSession_resetEmptyWorld(hxc_session);
  bool hxc_tmp_call_result_n34 = hxc_caxecraft_content_FirstPlayableLevel_loadTerrain(hxc_session);
  if (!hxc_tmp_call_result_n34)
  {
    struct hxc_array_ref *hxc_tmp_array_create_result_n35 = NULL;
    if (hxc_array_ref_create(hxc_default_allocator(), (hxc_array_element_ops){ sizeof(struct hxc_caxecraft_content_LoadedActorBinding), _Alignof(struct hxc_caxecraft_content_LoadedActorBinding), NULL, hxc_array_7c0765de_element_copy, hxc_array_7c0765de_element_assign, hxc_array_7c0765de_element_destroy }, &hxc_tmp_array_create_result_n35) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_actorBindings) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_tmp_managed_flow_owner_n7) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_enum_65c03c54_destroy(&hxc_g_he441670ec403);
    if (hxc_array_ref_release(hxc_tmp_static_call_argument_0_owner_n5) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_gc_root_frame_pop(&hxc_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return (struct hxc_caxecraft_content_FirstPlayableSessionLoad){ .hxc_actors = hxc_tmp_array_create_result_n35, .hxc_valid = false, .hxc_waterPresentationCell = -1 };
  }
  int32_t hxc_fluidIndex = 0;
  while (1)
  {
    if (!(hxc_fluidIndex < 2))
    {
      break;
    }
    int32_t hxc_tmp_call_result_n39 = hxc_caxecraft_content_FirstPlayableLevel_fluidX(hxc_fluidIndex);
    int32_t hxc_tmp_call_result_n41 = hxc_caxecraft_content_FirstPlayableLevel_fluidY(hxc_fluidIndex);
    int32_t hxc_tmp_call_result_n43 = hxc_caxecraft_content_FirstPlayableLevel_fluidZ(hxc_fluidIndex);
    struct hxc_caxecraft_scenario_VoxelPoint hxc_tmp_call_result_n44 = hxc_caxecraft_domain_World_coord(hxc_tmp_call_result_n39, hxc_tmp_call_result_n41, hxc_tmp_call_result_n43);
    struct hxc_caxecraft_scenario_VoxelPoint hxc_coordinate = hxc_tmp_call_result_n44;
    int32_t hxc_tmp_call_result_n46 = hxc_caxecraft_content_FirstPlayableLevel_fluidKind(hxc_fluidIndex);
    bool hxc_tmp_conditional_result_n18 = false;
    if (hxc_tmp_call_result_n46 == 0)
    {
      struct hxc_caxecraft_scenario_VoxelPoint hxc_tmp_load_result_n47 = hxc_coordinate;
      int32_t hxc_tmp_call_result_n49 = hxc_caxecraft_content_FirstPlayableLevel_fluidWidth(hxc_fluidIndex);
      int32_t hxc_tmp_call_result_n51 = hxc_caxecraft_content_FirstPlayableLevel_fluidHeight(hxc_fluidIndex);
      int32_t hxc_tmp_call_result_n53 = hxc_caxecraft_content_FirstPlayableLevel_fluidDepth(hxc_fluidIndex);
      bool hxc_tmp_instance_call_result_n54 = hxc_caxecraft_domain_GameSession_placeInitialWaterVolume(hxc_session, hxc_tmp_load_result_n47, hxc_tmp_call_result_n49, hxc_tmp_call_result_n51, hxc_tmp_call_result_n53);
      hxc_tmp_conditional_result_n18 = hxc_tmp_instance_call_result_n54;
    }
    else
    {
      bool hxc_tmp_instance_call_result_n56 = hxc_caxecraft_domain_GameSession_placeWaterSource(hxc_session, hxc_coordinate);
      hxc_tmp_conditional_result_n18 = hxc_tmp_instance_call_result_n56;
    }
    bool hxc_loaded = hxc_tmp_conditional_result_n18;
    if (!hxc_loaded)
    {
      struct hxc_array_ref *hxc_tmp_array_create_result_n59 = NULL;
      if (hxc_array_ref_create(hxc_default_allocator(), (hxc_array_element_ops){ sizeof(struct hxc_caxecraft_content_LoadedActorBinding), _Alignof(struct hxc_caxecraft_content_LoadedActorBinding), NULL, hxc_array_7c0765de_element_copy, hxc_array_7c0765de_element_assign, hxc_array_7c0765de_element_destroy }, &hxc_tmp_array_create_result_n59) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_actorBindings) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_tmp_managed_flow_owner_n7) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_enum_65c03c54_destroy(&hxc_g_he441670ec403);
      if (hxc_array_ref_release(hxc_tmp_static_call_argument_0_owner_n5) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_gc_root_frame_pop(&hxc_gc_frame) != HXC_STATUS_OK)
      {
        abort();
      }
      return (struct hxc_caxecraft_content_FirstPlayableSessionLoad){ .hxc_actors = hxc_tmp_array_create_result_n59, .hxc_valid = false, .hxc_waterPresentationCell = -1 };
    }
    hxc_fluidIndex = hxc_i32_add_wrapping(hxc_fluidIndex, 1);
  }
  int32_t hxc_itemIndex = 0;
  while (1)
  {
    if (!(hxc_itemIndex < 1))
    {
      break;
    }
    bool hxc_tmp_instance_call_result_n64 = hxc_caxecraft_domain_GameSession_activateAuthoredItemDuringLoad(hxc_session, hxc_itemIndex);
    if (!hxc_tmp_instance_call_result_n64)
    {
      struct hxc_array_ref *hxc_tmp_array_create_result_n65 = NULL;
      if (hxc_array_ref_create(hxc_default_allocator(), (hxc_array_element_ops){ sizeof(struct hxc_caxecraft_content_LoadedActorBinding), _Alignof(struct hxc_caxecraft_content_LoadedActorBinding), NULL, hxc_array_7c0765de_element_copy, hxc_array_7c0765de_element_assign, hxc_array_7c0765de_element_destroy }, &hxc_tmp_array_create_result_n65) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_actorBindings) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_tmp_managed_flow_owner_n7) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_enum_65c03c54_destroy(&hxc_g_he441670ec403);
      if (hxc_array_ref_release(hxc_tmp_static_call_argument_0_owner_n5) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_gc_root_frame_pop(&hxc_gc_frame) != HXC_STATUS_OK)
      {
        abort();
      }
      return (struct hxc_caxecraft_content_FirstPlayableSessionLoad){ .hxc_actors = hxc_tmp_array_create_result_n65, .hxc_valid = false, .hxc_waterPresentationCell = -1 };
    }
    hxc_itemIndex = hxc_i32_add_wrapping(hxc_itemIndex, 1);
  }
  size_t hxc_tmp_length_n69;
  const uint8_t *hxc_tmp_instance_call_result_n68 = hxc_caxecraft_domain_GameSession_worldView(hxc_session, &hxc_tmp_length_n69);
  struct hxc_caxecraft_domain_CharacterBody hxc_tmp_call_result_n70 = hxc_caxecraft_domain_CharacterPhysics_body(16.5, 5.0, 16.5);
  struct hxc_caxecraft_domain_CharacterBody hxc_tmp_call_result_n71 = hxc_caxecraft_domain_CharacterPhysics_recoverSpawn(hxc_tmp_instance_call_result_n68, hxc_tmp_length_n69, hxc_tmp_call_result_n70);
  struct hxc_caxecraft_domain_CharacterBody hxc_spawnBody = hxc_tmp_call_result_n71;
  struct hxc_caxecraft_domain_CharacterBody hxc_tmp_load_result_n72 = hxc_spawnBody;
  struct hxc_caxecraft_domain_AquaticProfile hxc_tmp_call_result_n73 = hxc_caxecraft_content_BaseContentPack_aquaticProfile(0);
  struct hxc_caxecraft_domain_Character hxc_tmp_call_result_n74 = hxc_caxecraft_domain_Character_start(1, hxc_tmp_load_result_n72, hxc_tmp_call_result_n73, hxc_initialHealth);
  bool hxc_tmp_instance_call_result_n75 = hxc_caxecraft_domain_GameSession_bindLocalPlayer(hxc_session, hxc_tmp_call_result_n74);
  if (!hxc_tmp_instance_call_result_n75)
  {
    struct hxc_array_ref *hxc_tmp_array_create_result_n76 = NULL;
    if (hxc_array_ref_create(hxc_default_allocator(), (hxc_array_element_ops){ sizeof(struct hxc_caxecraft_content_LoadedActorBinding), _Alignof(struct hxc_caxecraft_content_LoadedActorBinding), NULL, hxc_array_7c0765de_element_copy, hxc_array_7c0765de_element_assign, hxc_array_7c0765de_element_destroy }, &hxc_tmp_array_create_result_n76) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_actorBindings) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_tmp_managed_flow_owner_n7) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_enum_65c03c54_destroy(&hxc_g_he441670ec403);
    if (hxc_array_ref_release(hxc_tmp_static_call_argument_0_owner_n5) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_gc_root_frame_pop(&hxc_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return (struct hxc_caxecraft_content_FirstPlayableSessionLoad){ .hxc_actors = hxc_tmp_array_create_result_n76, .hxc_valid = false, .hxc_waterPresentationCell = -1 };
  }
  struct hxc_caxecraft_content_ActorPublicationResult hxc_tmp_call_result_n79 = hxc_caxecraft_content_ActorPublication_publishActorPlans(hxc_session, hxc_tmp_managed_flow_owner_n7);
  struct hxc_caxecraft_content_ActorPublicationResult hxc_g_h0b2ba883f2b5 = hxc_tmp_call_result_n79;
  switch (hxc_g_h0b2ba883f2b5.hxc_tag) {
    case hxc_caxecraft_content_ActorPublicationResult_ActorsPublished:
      {
        break;
      }
    case hxc_caxecraft_content_ActorPublicationResult_ActorPublicationRejected:
      {
        struct hxc_array_ref *hxc_tmp_array_create_result_n88 = NULL;
        if (hxc_array_ref_create(hxc_default_allocator(), (hxc_array_element_ops){ sizeof(struct hxc_caxecraft_content_LoadedActorBinding), _Alignof(struct hxc_caxecraft_content_LoadedActorBinding), NULL, hxc_array_7c0765de_element_copy, hxc_array_7c0765de_element_assign, hxc_array_7c0765de_element_destroy }, &hxc_tmp_array_create_result_n88) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_array_ref_release(hxc_actorBindings) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_array_ref_release(hxc_tmp_managed_flow_owner_n7) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_enum_65c03c54_destroy(&hxc_g_he441670ec403);
        if (hxc_array_ref_release(hxc_tmp_static_call_argument_0_owner_n5) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_gc_root_frame_pop(&hxc_gc_frame) != HXC_STATUS_OK)
        {
          abort();
        }
        return (struct hxc_caxecraft_content_FirstPlayableSessionLoad){ .hxc_actors = hxc_tmp_array_create_result_n88, .hxc_valid = false, .hxc_waterPresentationCell = -1 };
      }
  }
  if (hxc_g_h0b2ba883f2b5.hxc_tag != hxc_caxecraft_content_ActorPublicationResult_ActorsPublished)
  {
    abort();
  }
  int32_t hxc_tmp_enum_payload_project_n82 = hxc_g_h0b2ba883f2b5.hxc_payload.hxc_ActorsPublished.hxc_count;
  int32_t hxc_count = hxc_tmp_enum_payload_project_n82;
  int32_t hxc_tmp_load_result_n83 = hxc_count;
  int32_t hxc_tmp_array_length_result_n85;
  if (hxc_array_ref_length(hxc_actorBindings, &hxc_tmp_array_length_result_n85) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_tmp_load_result_n83 != hxc_tmp_array_length_result_n85)
  {
    struct hxc_array_ref *hxc_tmp_array_create_result_n86 = NULL;
    if (hxc_array_ref_create(hxc_default_allocator(), (hxc_array_element_ops){ sizeof(struct hxc_caxecraft_content_LoadedActorBinding), _Alignof(struct hxc_caxecraft_content_LoadedActorBinding), NULL, hxc_array_7c0765de_element_copy, hxc_array_7c0765de_element_assign, hxc_array_7c0765de_element_destroy }, &hxc_tmp_array_create_result_n86) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_actorBindings) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_tmp_managed_flow_owner_n7) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_enum_65c03c54_destroy(&hxc_g_he441670ec403);
    if (hxc_array_ref_release(hxc_tmp_static_call_argument_0_owner_n5) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_gc_root_frame_pop(&hxc_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return (struct hxc_caxecraft_content_FirstPlayableSessionLoad){ .hxc_actors = hxc_tmp_array_create_result_n86, .hxc_valid = false, .hxc_waterPresentationCell = -1 };
  }
  int32_t hxc_tmp_load_result_n90 = hxc_presentationCell;
  hxc_tmp_record_field_actors_owner_n23 = hxc_actorBindings;
  if (hxc_array_ref_retain(hxc_tmp_record_field_actors_owner_n23) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_array_ref *hxc_tmp_record_field_actors_owned_load_result_n92 = hxc_tmp_record_field_actors_owner_n23;
  if (hxc_array_ref_release(hxc_actorBindings) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_ref_release(hxc_tmp_managed_flow_owner_n7) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_enum_65c03c54_destroy(&hxc_g_he441670ec403);
  if (hxc_array_ref_release(hxc_tmp_static_call_argument_0_owner_n5) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_gc_root_frame_pop(&hxc_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return (struct hxc_caxecraft_content_FirstPlayableSessionLoad){ .hxc_actors = hxc_tmp_record_field_actors_owned_load_result_n92, .hxc_valid = true, .hxc_waterPresentationCell = hxc_tmp_load_result_n90 };
}

int32_t hxc_caxecraft_content_FirstPlayableSessionLoader_validateFacts(int32_t hxc_initialHealth)
{
  bool hxc_tmp_short_circuit_result_n1 = hxc_initialHealth <= 0;
  if (!(hxc_initialHealth <= 0))
  {
    hxc_tmp_short_circuit_result_n1 = hxc_initialHealth > 6;
  }
  if (hxc_tmp_short_circuit_result_n1)
  {
    return -1;
  }
  int32_t hxc_itemIndex = 0;
  while (1)
  {
    if (!(hxc_itemIndex < 1))
    {
      break;
    }
    bool hxc_tmp = false;
    int32_t hxc_tmp_call_result_n3 = hxc_caxecraft_content_FirstPlayableLevel_itemStorageCode(hxc_itemIndex);
    int32_t hxc_code = hxc_tmp_call_result_n3;
    int32_t hxc_tmp_load_result_n4 = hxc_code;
    bool hxc_tmp_short_circuit_result_n5 = hxc_tmp_load_result_n4 >= 0;
    if (hxc_tmp_load_result_n4 >= 0)
    {
      hxc_tmp_short_circuit_result_n5 = hxc_code < 9;
    }
    bool hxc_tmp_short_circuit_load_result_n6 = hxc_tmp_short_circuit_result_n5;
    bool hxc_tmp_short_circuit_result_n6 = !hxc_tmp_short_circuit_load_result_n6;
    if (!!hxc_tmp_short_circuit_load_result_n6)
    {
      int32_t hxc_tmp_call_result_n8 = hxc_caxecraft_content_FirstPlayableLevel_itemQuantity(hxc_itemIndex);
      hxc_tmp_short_circuit_result_n6 = hxc_tmp_call_result_n8 <= 0;
    }
    if (!hxc_tmp_short_circuit_result_n6)
    {
      int32_t hxc_tmp_call_result_n11 = hxc_caxecraft_content_FirstPlayableLevel_itemXMilli(hxc_itemIndex);
      int32_t hxc_x_hda1ca9b658a0 = hxc_tmp_call_result_n11;
      int32_t hxc_tmp_call_result_n13 = hxc_caxecraft_content_FirstPlayableLevel_itemYMilli(hxc_itemIndex);
      int32_t hxc_y_h5bf47e58c4f8 = hxc_tmp_call_result_n13;
      int32_t hxc_tmp_call_result_n15 = hxc_caxecraft_content_FirstPlayableLevel_itemZMilli(hxc_itemIndex);
      int32_t hxc_z_he9f25ed500ea = hxc_tmp_call_result_n15;
      int32_t hxc_tmp_load_result_n16 = hxc_x_hda1ca9b658a0;
      bool hxc_tmp_short_circuit_result_n10 = hxc_tmp_load_result_n16 >= 0;
      if (hxc_tmp_load_result_n16 >= 0)
      {
        hxc_tmp_short_circuit_result_n10 = hxc_x_hda1ca9b658a0 < 32000;
      }
      bool hxc_tmp_short_circuit_load_result_n18 = hxc_tmp_short_circuit_result_n10;
      bool hxc_tmp_short_circuit_result_n11 = hxc_tmp_short_circuit_load_result_n18;
      if (hxc_tmp_short_circuit_load_result_n18)
      {
        hxc_tmp_short_circuit_result_n11 = hxc_y_h5bf47e58c4f8 >= 0;
      }
      bool hxc_tmp_short_circuit_load_result_n20 = hxc_tmp_short_circuit_result_n11;
      bool hxc_tmp_short_circuit_result_n12 = hxc_tmp_short_circuit_load_result_n20;
      if (hxc_tmp_short_circuit_load_result_n20)
      {
        hxc_tmp_short_circuit_result_n12 = hxc_y_h5bf47e58c4f8 < 16000;
      }
      bool hxc_tmp_short_circuit_load_result_n22 = hxc_tmp_short_circuit_result_n12;
      bool hxc_tmp_short_circuit_result_n13 = hxc_tmp_short_circuit_load_result_n22;
      if (hxc_tmp_short_circuit_load_result_n22)
      {
        hxc_tmp_short_circuit_result_n13 = hxc_z_he9f25ed500ea >= 0;
      }
      bool hxc_tmp_short_circuit_load_result_n24 = hxc_tmp_short_circuit_result_n13;
      bool hxc_tmp_short_circuit_result_n14 = hxc_tmp_short_circuit_load_result_n24;
      if (hxc_tmp_short_circuit_load_result_n24)
      {
        hxc_tmp_short_circuit_result_n14 = hxc_z_he9f25ed500ea < 32000;
      }
      hxc_tmp = !hxc_tmp_short_circuit_result_n14;
    }
    else
    {
      hxc_tmp = true;
    }
    if (hxc_tmp)
    {
      return -1;
    }
    hxc_itemIndex = hxc_i32_add_wrapping(hxc_itemIndex, 1);
  }
  int32_t hxc_presentationCell = -1;
  int32_t hxc_fluidIndex = 0;
  while (1)
  {
    if (!(hxc_fluidIndex < 2))
    {
      break;
    }
    int32_t hxc_tmp_call_result_n31 = hxc_caxecraft_content_FirstPlayableLevel_fluidKind(hxc_fluidIndex);
    int32_t hxc_kind = hxc_tmp_call_result_n31;
    int32_t hxc_tmp_call_result_n33 = hxc_caxecraft_content_FirstPlayableLevel_fluidWidth(hxc_fluidIndex);
    int32_t hxc_width = hxc_tmp_call_result_n33;
    int32_t hxc_tmp_call_result_n35 = hxc_caxecraft_content_FirstPlayableLevel_fluidHeight(hxc_fluidIndex);
    int32_t hxc_height = hxc_tmp_call_result_n35;
    int32_t hxc_tmp_call_result_n37 = hxc_caxecraft_content_FirstPlayableLevel_fluidDepth(hxc_fluidIndex);
    int32_t hxc_depth = hxc_tmp_call_result_n37;
    int32_t hxc_tmp_call_result_n39 = hxc_caxecraft_content_FirstPlayableLevel_fluidX(hxc_fluidIndex);
    int32_t hxc_x_h4a9eee62312f = hxc_tmp_call_result_n39;
    int32_t hxc_tmp_call_result_n41 = hxc_caxecraft_content_FirstPlayableLevel_fluidY(hxc_fluidIndex);
    int32_t hxc_y_h83ae04cf40f7 = hxc_tmp_call_result_n41;
    int32_t hxc_tmp_call_result_n43 = hxc_caxecraft_content_FirstPlayableLevel_fluidZ(hxc_fluidIndex);
    int32_t hxc_z_h1d5cc68a379f = hxc_tmp_call_result_n43;
    int32_t hxc_tmp_call_result_n45 = hxc_caxecraft_content_FirstPlayableLevel_fluidPresentationCell(hxc_fluidIndex);
    int32_t hxc_cell = hxc_tmp_call_result_n45;
    int32_t hxc_tmp_load_result_n46 = hxc_kind;
    bool hxc_tmp_short_circuit_result_n25 = hxc_tmp_load_result_n46 != 0;
    if (hxc_tmp_load_result_n46 != 0)
    {
      hxc_tmp_short_circuit_result_n25 = hxc_kind != 1;
    }
    bool hxc_tmp_short_circuit_load_result_n48 = hxc_tmp_short_circuit_result_n25;
    bool hxc_tmp_short_circuit_result_n26 = hxc_tmp_short_circuit_load_result_n48;
    if (!hxc_tmp_short_circuit_load_result_n48)
    {
      hxc_tmp_short_circuit_result_n26 = hxc_width <= 0;
    }
    bool hxc_tmp_short_circuit_load_result_n50 = hxc_tmp_short_circuit_result_n26;
    bool hxc_tmp_short_circuit_result_n27 = hxc_tmp_short_circuit_load_result_n50;
    if (!hxc_tmp_short_circuit_load_result_n50)
    {
      hxc_tmp_short_circuit_result_n27 = hxc_height <= 0;
    }
    bool hxc_tmp_short_circuit_load_result_n52 = hxc_tmp_short_circuit_result_n27;
    bool hxc_tmp_short_circuit_result_n28 = hxc_tmp_short_circuit_load_result_n52;
    if (!hxc_tmp_short_circuit_load_result_n52)
    {
      hxc_tmp_short_circuit_result_n28 = hxc_depth <= 0;
    }
    bool hxc_tmp_short_circuit_load_result_n54 = hxc_tmp_short_circuit_result_n28;
    bool hxc_tmp_short_circuit_result_n29 = hxc_tmp_short_circuit_load_result_n54;
    if (!hxc_tmp_short_circuit_load_result_n54)
    {
      hxc_tmp_short_circuit_result_n29 = hxc_x_h4a9eee62312f < 0;
    }
    bool hxc_tmp_short_circuit_load_result_n56 = hxc_tmp_short_circuit_result_n29;
    bool hxc_tmp_short_circuit_result_n30 = hxc_tmp_short_circuit_load_result_n56;
    if (!hxc_tmp_short_circuit_load_result_n56)
    {
      hxc_tmp_short_circuit_result_n30 = hxc_y_h83ae04cf40f7 < 0;
    }
    bool hxc_tmp_short_circuit_load_result_n58 = hxc_tmp_short_circuit_result_n30;
    bool hxc_tmp_short_circuit_result_n31 = hxc_tmp_short_circuit_load_result_n58;
    if (!hxc_tmp_short_circuit_load_result_n58)
    {
      hxc_tmp_short_circuit_result_n31 = hxc_z_h1d5cc68a379f < 0;
    }
    bool hxc_tmp_short_circuit_load_result_n60 = hxc_tmp_short_circuit_result_n31;
    bool hxc_tmp_short_circuit_result_n32 = hxc_tmp_short_circuit_load_result_n60;
    if (!hxc_tmp_short_circuit_load_result_n60)
    {
      int32_t hxc_tmp_load_result_n61 = hxc_x_h4a9eee62312f;
      hxc_tmp_short_circuit_result_n32 = hxc_tmp_load_result_n61 > hxc_i32_subtract_wrapping(32, hxc_width);
    }
    bool hxc_tmp_short_circuit_load_result_n63 = hxc_tmp_short_circuit_result_n32;
    bool hxc_tmp_short_circuit_result_n33 = hxc_tmp_short_circuit_load_result_n63;
    if (!hxc_tmp_short_circuit_load_result_n63)
    {
      int32_t hxc_tmp_load_result_n64 = hxc_y_h83ae04cf40f7;
      hxc_tmp_short_circuit_result_n33 = hxc_tmp_load_result_n64 > hxc_i32_subtract_wrapping(16, hxc_height);
    }
    bool hxc_tmp_short_circuit_load_result_n66 = hxc_tmp_short_circuit_result_n33;
    bool hxc_tmp_short_circuit_result_n34 = hxc_tmp_short_circuit_load_result_n66;
    if (!hxc_tmp_short_circuit_load_result_n66)
    {
      int32_t hxc_tmp_load_result_n67 = hxc_z_h1d5cc68a379f;
      hxc_tmp_short_circuit_result_n34 = hxc_tmp_load_result_n67 > hxc_i32_subtract_wrapping(32, hxc_depth);
    }
    bool hxc_tmp_short_circuit_load_result_n69 = hxc_tmp_short_circuit_result_n34;
    bool hxc_tmp_short_circuit_result_n35 = hxc_tmp_short_circuit_load_result_n69;
    if (!hxc_tmp_short_circuit_load_result_n69)
    {
      hxc_tmp_short_circuit_result_n35 = hxc_cell < 0;
    }
    if (hxc_tmp_short_circuit_result_n35)
    {
      return -1;
    }
    if (hxc_presentationCell < 0)
    {
      hxc_presentationCell = hxc_cell;
    }
    else
    {
      int32_t hxc_tmp_load_result_n74 = hxc_presentationCell;
      if (hxc_tmp_load_result_n74 != hxc_cell)
      {
        return -1;
      }
    }
    hxc_fluidIndex = hxc_i32_add_wrapping(hxc_fluidIndex, 1);
  }
  return hxc_presentationCell;
}
