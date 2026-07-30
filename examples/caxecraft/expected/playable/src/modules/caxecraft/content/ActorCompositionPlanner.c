#include "hxc/program.h"

struct hxc_caxecraft_content_ActorCompositionResult hxc_caxecraft_content_ActorCompositionPlanner_planActorComposition(struct hxc_array_ref *hxc_l_objects, struct hxc_compiler_interface_dispatch_caxecraft_content_ActorContentResolver_value hxc_l_resolver)
{
  hxc_string hxc_l_npcType = { 0 };
  hxc_string hxc_l_dialogue = { 0 };
  struct hxc_caxecraft_content_ActorCompositionPlanner_CharacterCandidate hxc_l_tmp_managed_flow_owner_n19 = { 0 };
  struct hxc_caxecraft_content_ActorIdentityPlanResult hxc_l_g_h2cf03206ee40 = { 0 };
  struct hxc_caxecraft_content_ActorIdentityPlanError hxc_l_error = { 0 };
  struct hxc_caxecraft_content_ActorIdentityPlanError hxc_l_tmp_enum_payload_0_owner_n24 = { 0 };
  struct hxc_array_ref *hxc_l_tmp_managed_flow_owner_n25 = { 0 };
  struct hxc_array_ref *hxc_l_plans = { 0 };
  struct hxc_caxecraft_content_ActorCompositionPlanner_CharacterCandidate hxc_l_candidate_h194daa8e982a = { 0 };
  struct hxc_caxecraft_content_ActorCompositionPlanner_CharacterCandidate hxc_l_tmp_array_element_owner_n31 = { 0 };
  struct hxc_caxecraft_content_ActorContentResolution hxc_l_tmp_managed_flow_owner_n33 = { 0 };
  struct hxc_caxecraft_content_ActorMechanicsProfile hxc_l_profile = { 0 };
  hxc_string hxc_l_tmp_enum_payload_0_owner_n36 = { 0 };
  hxc_string hxc_l_tmp_enum_payload_1_owner_n37 = { 0 };
  enum hxc_caxecraft_content_ActorContentKind hxc_l_actual = { 0 };
  hxc_string hxc_l_tmp_enum_payload_0_owner_n39 = { 0 };
  hxc_string hxc_l_tmp_enum_payload_1_owner_n40 = { 0 };
  struct hxc_caxecraft_content_ActorMechanicsProfile hxc_l_tmp_managed_flow_owner_n41 = { 0 };
  hxc_string hxc_l_tmp_enum_payload_0_owner_n42 = { 0 };
  hxc_string hxc_l_tmp_enum_payload_1_owner_n43 = { 0 };
  struct hxc_caxecraft_content_ActorIdentityBinding hxc_l_tmp_array_element_owner_n45 = { 0 };
  struct hxc_caxecraft_content_CharacterSpawnPlan hxc_l_tmp_array_push_element_owner_n49 = { 0 };
  struct hxc_caxecraft_scenario_ScenarioObject hxc_l_object = { 0 };
  struct hxc_caxecraft_scenario_ScenarioObject hxc_l_tmp_array_element_owner_n6 = { 0 };
  struct hxc_caxecraft_scenario_ObjectPlacement hxc_l_g_h326d6465628d = { 0 };
  hxc_string hxc_l_entityType = { 0 };
  struct hxc_array_ref *hxc_l_tmp_array_create_result_n0 = NULL;
  if (hxc_array_ref_create(hxc_default_allocator(), (hxc_array_element_ops){ sizeof(struct hxc_caxecraft_content_ActorCompositionPlanner_CharacterCandidate), _Alignof(struct hxc_caxecraft_content_ActorCompositionPlanner_CharacterCandidate), NULL, hxc_array_d75ef385_element_copy, hxc_array_d75ef385_element_assign, hxc_array_d75ef385_element_destroy }, &hxc_l_tmp_array_create_result_n0) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_candidates = hxc_l_tmp_array_create_result_n0;
  struct hxc_array_ref *hxc_l_tmp_array_create_result_n1 = NULL;
  if (hxc_array_ref_create(hxc_default_allocator(), (hxc_array_element_ops){ sizeof(hxc_string), _Alignof(hxc_string), NULL, hxc_array_4c9e169c_element_copy, hxc_array_4c9e169c_element_assign, hxc_array_4c9e169c_element_destroy }, &hxc_l_tmp_array_create_result_n1) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_authoredIds = hxc_l_tmp_array_create_result_n1;
  int32_t hxc_l_g_h8e61e2e2bc1e = 0;
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n2 = hxc_l_g_h8e61e2e2bc1e;
    int32_t hxc_l_tmp_array_length_result_n3;
    if (hxc_array_ref_length(hxc_l_objects, &hxc_l_tmp_array_length_result_n3) != HXC_STATUS_OK)
    {
      abort();
    }
    if (!(hxc_l_tmp_load_result_n2 < hxc_l_tmp_array_length_result_n3))
    {
      break;
    }
    struct hxc_caxecraft_scenario_ScenarioObject hxc_l_tmp_array_get_result_n5;
    if (hxc_array_ref_get_copy(hxc_l_objects, (size_t)hxc_l_g_h8e61e2e2bc1e, &hxc_l_tmp_array_get_result_n5) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_array_element_owner_n6 = hxc_l_tmp_array_get_result_n5;
    hxc_l_object = hxc_l_tmp_array_element_owner_n6;
    if (hxc_record_86003d77_retain(&hxc_l_object) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_g_h8e61e2e2bc1e = hxc_i32_add_wrapping(hxc_l_g_h8e61e2e2bc1e, 1);
    struct hxc_caxecraft_content_ActorCompositionPlanner_CharacterCandidate hxc_l_candidate_h4af786a80451 = { 0 };
    hxc_l_g_h326d6465628d = hxc_l_object.hxc_placement;
    if (hxc_enum_65d6d89d_retain(&hxc_l_g_h326d6465628d) != HXC_STATUS_OK)
    {
      abort();
    }
    switch (hxc_l_g_h326d6465628d.hxc_tag) {
      case hxc_caxecraft_scenario_ObjectPlacement_Entity:
        {
          if (hxc_l_g_h326d6465628d.hxc_tag != hxc_caxecraft_scenario_ObjectPlacement_Entity)
          {
            abort();
          }
          hxc_string hxc_l_tmp_enum_payload_project_n11 = hxc_l_g_h326d6465628d.hxc_payload.hxc_Entity.hxc_entityType;
          hxc_l_entityType = hxc_l_tmp_enum_payload_project_n11;
          if (hxc_string_retain(hxc_l_entityType) != HXC_STATUS_OK)
          {
            abort();
          }
          if (hxc_l_g_h326d6465628d.hxc_tag != hxc_caxecraft_scenario_ObjectPlacement_Entity)
          {
            abort();
          }
          struct hxc_caxecraft_scenario_ScenarioTransform hxc_l_tmp_enum_payload_project_n13 = hxc_l_g_h326d6465628d.hxc_payload.hxc_Entity.hxc_transform;
          struct hxc_caxecraft_scenario_ScenarioTransform hxc_l_transform_hf99cbe0f1295 = hxc_l_tmp_enum_payload_project_n13;
          hxc_string hxc_l_tmp_record_field_authoredId_owner_n11 = hxc_l_object.hxc_id;
          if (hxc_string_retain(hxc_l_tmp_record_field_authoredId_owner_n11) != HXC_STATUS_OK)
          {
            abort();
          }
          hxc_string hxc_l_tmp_record_field_authoredId_owned_load_result_n15 = hxc_l_tmp_record_field_authoredId_owner_n11;
          hxc_string hxc_l_tmp_record_field_contentId_owner_n12 = hxc_l_entityType;
          if (hxc_string_retain(hxc_l_tmp_record_field_contentId_owner_n12) != HXC_STATUS_OK)
          {
            abort();
          }
          hxc_string hxc_l_tmp_record_field_contentId_owned_load_result_n17 = hxc_l_tmp_record_field_contentId_owner_n12;
          (void)(struct hxc_caxecraft_content_ActorCompositionPlanner_CharacterCandidate){ .hxc_authoredId = hxc_l_tmp_record_field_authoredId_owned_load_result_n15, .hxc_contentId = hxc_l_tmp_record_field_contentId_owned_load_result_n17, .hxc_expected = hxc_caxecraft_content_ActorContentKind_EnemyContent, .hxc_role = (struct hxc_caxecraft_content_CharacterSpawnRole){ .hxc_tag = hxc_caxecraft_content_CharacterSpawnRole_EnemyActor }, .hxc_transform = hxc_l_transform_hf99cbe0f1295 };
          hxc_l_candidate_h4af786a80451 = (struct hxc_caxecraft_content_ActorCompositionPlanner_CharacterCandidate){ .hxc_authoredId = hxc_l_tmp_record_field_authoredId_owned_load_result_n15, .hxc_contentId = hxc_l_tmp_record_field_contentId_owned_load_result_n17, .hxc_expected = hxc_caxecraft_content_ActorContentKind_EnemyContent, .hxc_role = (struct hxc_caxecraft_content_CharacterSpawnRole){ .hxc_tag = hxc_caxecraft_content_CharacterSpawnRole_EnemyActor }, .hxc_transform = hxc_l_transform_hf99cbe0f1295 };
          if (hxc_string_release(&hxc_l_entityType) != HXC_STATUS_OK)
          {
            abort();
          }
          break;
        }
      case hxc_caxecraft_scenario_ObjectPlacement_Npc:
        {
          if (hxc_l_g_h326d6465628d.hxc_tag != hxc_caxecraft_scenario_ObjectPlacement_Npc)
          {
            abort();
          }
          hxc_string hxc_l_tmp_enum_payload_project_n23 = hxc_l_g_h326d6465628d.hxc_payload.hxc_Npc.hxc_npcType;
          hxc_l_npcType = hxc_l_tmp_enum_payload_project_n23;
          if (hxc_string_retain(hxc_l_npcType) != HXC_STATUS_OK)
          {
            abort();
          }
          if (hxc_l_g_h326d6465628d.hxc_tag != hxc_caxecraft_scenario_ObjectPlacement_Npc)
          {
            abort();
          }
          hxc_string hxc_l_tmp_enum_payload_project_n25 = hxc_l_g_h326d6465628d.hxc_payload.hxc_Npc.hxc_dialogue;
          hxc_l_dialogue = hxc_l_tmp_enum_payload_project_n25;
          if (hxc_string_retain(hxc_l_dialogue) != HXC_STATUS_OK)
          {
            abort();
          }
          if (hxc_l_g_h326d6465628d.hxc_tag != hxc_caxecraft_scenario_ObjectPlacement_Npc)
          {
            abort();
          }
          struct hxc_caxecraft_scenario_ScenarioTransform hxc_l_tmp_enum_payload_project_n27 = hxc_l_g_h326d6465628d.hxc_payload.hxc_Npc.hxc_transform;
          struct hxc_caxecraft_scenario_ScenarioTransform hxc_l_transform_hf1553301d735 = hxc_l_tmp_enum_payload_project_n27;
          hxc_string hxc_l_tmp_record_field_authoredId_owner_n16 = hxc_l_object.hxc_id;
          if (hxc_string_retain(hxc_l_tmp_record_field_authoredId_owner_n16) != HXC_STATUS_OK)
          {
            abort();
          }
          hxc_string hxc_l_tmp_record_field_authoredId_owned_load_result_n29 = hxc_l_tmp_record_field_authoredId_owner_n16;
          hxc_string hxc_l_tmp_record_field_contentId_owner_n17 = hxc_l_npcType;
          if (hxc_string_retain(hxc_l_tmp_record_field_contentId_owner_n17) != HXC_STATUS_OK)
          {
            abort();
          }
          hxc_string hxc_l_tmp_record_field_contentId_owned_load_result_n31 = hxc_l_tmp_record_field_contentId_owner_n17;
          struct hxc_caxecraft_scenario_ScenarioTransform hxc_l_tmp_load_result_n32 = hxc_l_transform_hf1553301d735;
          hxc_string hxc_l_tmp_enum_payload_0_owner_n18 = hxc_l_dialogue;
          if (hxc_string_retain(hxc_l_tmp_enum_payload_0_owner_n18) != HXC_STATUS_OK)
          {
            abort();
          }
          (void)(struct hxc_caxecraft_content_ActorCompositionPlanner_CharacterCandidate){ .hxc_authoredId = hxc_l_tmp_record_field_authoredId_owned_load_result_n29, .hxc_contentId = hxc_l_tmp_record_field_contentId_owned_load_result_n31, .hxc_expected = hxc_caxecraft_content_ActorContentKind_NpcContent, .hxc_role = (struct hxc_caxecraft_content_CharacterSpawnRole){ .hxc_tag = hxc_caxecraft_content_CharacterSpawnRole_DialogueNpc, .hxc_payload.hxc_DialogueNpc.hxc_dialogue = hxc_l_tmp_enum_payload_0_owner_n18 }, .hxc_transform = hxc_l_tmp_load_result_n32 };
          hxc_l_candidate_h4af786a80451 = (struct hxc_caxecraft_content_ActorCompositionPlanner_CharacterCandidate){ .hxc_authoredId = hxc_l_tmp_record_field_authoredId_owned_load_result_n29, .hxc_contentId = hxc_l_tmp_record_field_contentId_owned_load_result_n31, .hxc_expected = hxc_caxecraft_content_ActorContentKind_NpcContent, .hxc_role = (struct hxc_caxecraft_content_CharacterSpawnRole){ .hxc_tag = hxc_caxecraft_content_CharacterSpawnRole_DialogueNpc, .hxc_payload.hxc_DialogueNpc.hxc_dialogue = hxc_l_tmp_enum_payload_0_owner_n18 }, .hxc_transform = hxc_l_tmp_load_result_n32 };
          if (hxc_string_release(&hxc_l_dialogue) != HXC_STATUS_OK)
          {
            abort();
          }
          if (hxc_string_release(&hxc_l_npcType) != HXC_STATUS_OK)
          {
            abort();
          }
          break;
        }
      default:
        {
          hxc_enum_65d6d89d_destroy(&hxc_l_g_h326d6465628d);
          hxc_record_86003d77_destroy(&hxc_l_object);
          hxc_array_223dc263_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n6);
          continue;
        }
    }
    struct hxc_caxecraft_content_ActorCompositionPlanner_CharacterCandidate hxc_l_tmp_managed_flow_carrier_move_result_n38 = hxc_l_candidate_h4af786a80451;
    hxc_l_tmp_managed_flow_owner_n19 = hxc_l_tmp_managed_flow_carrier_move_result_n38;
    struct hxc_array_ref *hxc_l_tmp_load_result_n39 = hxc_l_candidates;
    int32_t hxc_l_tmp_array_push_result_n41;
    if (hxc_array_ref_push_copy(hxc_l_tmp_load_result_n39, &hxc_l_tmp_managed_flow_owner_n19, &hxc_l_tmp_array_push_result_n41) != HXC_STATUS_OK)
    {
      abort();
    }
    (void)hxc_l_tmp_array_push_result_n41;
    struct hxc_array_ref *hxc_l_tmp_load_result_n42 = hxc_l_authoredIds;
    int32_t hxc_l_tmp_array_push_result_n44;
    if (hxc_array_ref_push_copy(hxc_l_tmp_load_result_n42, &hxc_l_tmp_managed_flow_owner_n19.hxc_authoredId, &hxc_l_tmp_array_push_result_n44) != HXC_STATUS_OK)
    {
      abort();
    }
    (void)hxc_l_tmp_array_push_result_n44;
    hxc_record_fc698661_destroy(&hxc_l_tmp_managed_flow_owner_n19);
    hxc_enum_65d6d89d_destroy(&hxc_l_g_h326d6465628d);
    hxc_record_86003d77_destroy(&hxc_l_object);
    hxc_array_223dc263_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n6);
  }
  struct hxc_array_ref *hxc_l_identities = { 0 };
  struct hxc_caxecraft_content_ActorIdentityPlanResult hxc_l_tmp_call_result_n46 = hxc_caxecraft_content_ActorIdentityPlanner_planActorIdentities(hxc_l_authoredIds);
  hxc_l_g_h2cf03206ee40 = hxc_l_tmp_call_result_n46;
  switch (hxc_l_g_h2cf03206ee40.hxc_tag) {
    case hxc_caxecraft_content_ActorIdentityPlanResult_Planned:
      {
        break;
      }
    case hxc_caxecraft_content_ActorIdentityPlanResult_Rejected:
      {
        if (hxc_l_g_h2cf03206ee40.hxc_tag != hxc_caxecraft_content_ActorIdentityPlanResult_Rejected)
        {
          abort();
        }
        struct hxc_caxecraft_content_ActorIdentityPlanError hxc_l_tmp_enum_payload_project_n52 = hxc_l_g_h2cf03206ee40.hxc_payload.hxc_Rejected.hxc_error;
        hxc_l_error = hxc_l_tmp_enum_payload_project_n52;
        if (hxc_enum_c16d2c34_retain(&hxc_l_error) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_l_tmp_enum_payload_0_owner_n24 = hxc_l_error;
        if (hxc_enum_c16d2c34_retain(&hxc_l_tmp_enum_payload_0_owner_n24) != HXC_STATUS_OK)
        {
          abort();
        }
        struct hxc_caxecraft_content_ActorIdentityPlanError hxc_l_tmp_enum_payload_0_owned_load_result_n54 = hxc_l_tmp_enum_payload_0_owner_n24;
        hxc_enum_c16d2c34_destroy(&hxc_l_error);
        hxc_enum_67dc6ad1_destroy(&hxc_l_g_h2cf03206ee40);
        if (hxc_array_ref_release(hxc_l_authoredIds) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_array_ref_release(hxc_l_candidates) != HXC_STATUS_OK)
        {
          abort();
        }
        return (struct hxc_caxecraft_content_ActorCompositionResult){ .hxc_tag = hxc_caxecraft_content_ActorCompositionResult_ActorCompositionRejected, .hxc_payload.hxc_ActorCompositionRejected.hxc_error = (struct hxc_caxecraft_content_ActorCompositionError){ .hxc_tag = hxc_caxecraft_content_ActorCompositionError_ActorIdentityRejected, .hxc_payload.hxc_ActorIdentityRejected.hxc_error = hxc_l_tmp_enum_payload_0_owned_load_result_n54 } };
      }
    default:
      {
        abort();
      }
  }
  if (hxc_l_g_h2cf03206ee40.hxc_tag != hxc_caxecraft_content_ActorIdentityPlanResult_Planned)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_tmp_enum_payload_project_n49 = hxc_l_g_h2cf03206ee40.hxc_payload.hxc_Planned.hxc_bindings;
  struct hxc_array_ref *hxc_l_bindings = hxc_l_tmp_enum_payload_project_n49;
  (void)hxc_l_bindings;
  hxc_l_identities = hxc_l_bindings;
  if (hxc_array_ref_retain(hxc_l_identities) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_tmp_managed_flow_carrier_move_result_n57 = hxc_l_identities;
  hxc_l_tmp_managed_flow_owner_n25 = hxc_l_tmp_managed_flow_carrier_move_result_n57;
  struct hxc_array_ref *hxc_l_tmp_array_create_result_n58 = NULL;
  if (hxc_array_ref_create(hxc_default_allocator(), (hxc_array_element_ops){ sizeof(struct hxc_caxecraft_content_CharacterSpawnPlan), _Alignof(struct hxc_caxecraft_content_CharacterSpawnPlan), NULL, hxc_array_78e5c5d1_element_copy, hxc_array_78e5c5d1_element_assign, hxc_array_78e5c5d1_element_destroy }, &hxc_l_tmp_array_create_result_n58) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_plans = hxc_l_tmp_array_create_result_n58;
  int32_t hxc_l_g_h738bba3c77cc = 0;
  int32_t hxc_l_tmp_array_length_result_n60;
  if (hxc_array_ref_length(hxc_l_candidates, &hxc_l_tmp_array_length_result_n60) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_g1 = hxc_l_tmp_array_length_result_n60;
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n61 = hxc_l_g_h738bba3c77cc;
    if (!(hxc_l_tmp_load_result_n61 < hxc_l_g1))
    {
      break;
    }
    int32_t hxc_l_tmp_increment_load_result_n63 = hxc_l_g_h738bba3c77cc;
    hxc_l_g_h738bba3c77cc = hxc_i32_add_wrapping(hxc_l_tmp_increment_load_result_n63, 1);
    int32_t hxc_l_index = hxc_l_tmp_increment_load_result_n63;
    struct hxc_array_ref *hxc_l_tmp_load_result_n64 = hxc_l_candidates;
    struct hxc_caxecraft_content_ActorCompositionPlanner_CharacterCandidate hxc_l_tmp_array_get_result_n66;
    if (hxc_array_ref_get_copy(hxc_l_tmp_load_result_n64, (size_t)hxc_l_index, &hxc_l_tmp_array_get_result_n66) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_array_element_owner_n31 = hxc_l_tmp_array_get_result_n66;
    hxc_l_candidate_h194daa8e982a = hxc_l_tmp_array_element_owner_n31;
    if (hxc_record_fc698661_retain(&hxc_l_candidate_h194daa8e982a) != HXC_STATUS_OK)
    {
      abort();
    }
    struct hxc_caxecraft_content_ActorContentResolution hxc_l_resolution = { 0 };
    switch (hxc_l_candidate_h194daa8e982a.hxc_expected) {
      case hxc_caxecraft_content_ActorContentKind_NpcContent:
        {
          struct hxc_caxecraft_content_ActorContentResolution hxc_l_tmp_instance_call_result_n70 = hxc_l_resolver.table->hxc_interface_slot_caxecraft_content_ActorContentResolver_resolveNpc(hxc_l_resolver.object, hxc_l_candidate_h194daa8e982a.hxc_contentId);
          hxc_l_resolution = hxc_l_tmp_instance_call_result_n70;
          break;
        }
      case hxc_caxecraft_content_ActorContentKind_EnemyContent:
        {
          struct hxc_caxecraft_content_ActorContentResolution hxc_l_tmp_instance_call_result_n72 = hxc_l_resolver.table->hxc_interface_slot_caxecraft_content_ActorContentResolver_resolveEnemy(hxc_l_resolver.object, hxc_l_candidate_h194daa8e982a.hxc_contentId);
          hxc_l_resolution = hxc_l_tmp_instance_call_result_n72;
          break;
        }
      default:
        {
          abort();
        }
    }
    struct hxc_caxecraft_content_ActorContentResolution hxc_l_tmp_managed_flow_carrier_move_result_n73 = hxc_l_resolution;
    hxc_l_tmp_managed_flow_owner_n33 = hxc_l_tmp_managed_flow_carrier_move_result_n73;
    struct hxc_caxecraft_content_ActorMechanicsProfile hxc_l_mechanics = { 0 };
    switch (hxc_l_tmp_managed_flow_owner_n33.hxc_tag) {
      case hxc_caxecraft_content_ActorContentResolution_ActorContentResolved:
        {
          break;
        }
      case hxc_caxecraft_content_ActorContentResolution_UnknownActorContent:
        {
          hxc_l_tmp_enum_payload_0_owner_n36 = hxc_l_candidate_h194daa8e982a.hxc_authoredId;
          if (hxc_string_retain(hxc_l_tmp_enum_payload_0_owner_n36) != HXC_STATUS_OK)
          {
            abort();
          }
          hxc_string hxc_l_tmp_enum_payload_0_owned_load_result_n79 = hxc_l_tmp_enum_payload_0_owner_n36;
          hxc_l_tmp_enum_payload_1_owner_n37 = hxc_l_candidate_h194daa8e982a.hxc_contentId;
          if (hxc_string_retain(hxc_l_tmp_enum_payload_1_owner_n37) != HXC_STATUS_OK)
          {
            abort();
          }
          hxc_string hxc_l_tmp_enum_payload_1_owned_load_result_n81 = hxc_l_tmp_enum_payload_1_owner_n37;
          enum hxc_caxecraft_content_ActorContentKind hxc_l_tmp_record_field_load_result_n82 = hxc_l_candidate_h194daa8e982a.hxc_expected;
          hxc_enum_23f1e692_destroy(&hxc_l_tmp_managed_flow_owner_n33);
          hxc_record_fc698661_destroy(&hxc_l_candidate_h194daa8e982a);
          hxc_array_d75ef385_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n31);
          if (hxc_array_ref_release(hxc_l_plans) != HXC_STATUS_OK)
          {
            abort();
          }
          if (hxc_array_ref_release(hxc_l_tmp_managed_flow_owner_n25) != HXC_STATUS_OK)
          {
            abort();
          }
          hxc_enum_67dc6ad1_destroy(&hxc_l_g_h2cf03206ee40);
          if (hxc_array_ref_release(hxc_l_authoredIds) != HXC_STATUS_OK)
          {
            abort();
          }
          if (hxc_array_ref_release(hxc_l_candidates) != HXC_STATUS_OK)
          {
            abort();
          }
          return (struct hxc_caxecraft_content_ActorCompositionResult){ .hxc_tag = hxc_caxecraft_content_ActorCompositionResult_ActorCompositionRejected, .hxc_payload.hxc_ActorCompositionRejected.hxc_error = (struct hxc_caxecraft_content_ActorCompositionError){ .hxc_tag = hxc_caxecraft_content_ActorCompositionError_UnknownPlacedActor, .hxc_payload.hxc_UnknownPlacedActor.hxc_authoredId = hxc_l_tmp_enum_payload_0_owned_load_result_n79, .hxc_payload.hxc_UnknownPlacedActor.hxc_contentId = hxc_l_tmp_enum_payload_1_owned_load_result_n81, .hxc_payload.hxc_UnknownPlacedActor.hxc_expected = hxc_l_tmp_record_field_load_result_n82 } };
        }
      case hxc_caxecraft_content_ActorContentResolution_WrongActorContentKind:
        {
          if (hxc_l_tmp_managed_flow_owner_n33.hxc_tag != hxc_caxecraft_content_ActorContentResolution_WrongActorContentKind)
          {
            abort();
          }
          enum hxc_caxecraft_content_ActorContentKind hxc_l_tmp_enum_payload_project_n86 = hxc_l_tmp_managed_flow_owner_n33.hxc_payload.hxc_WrongActorContentKind.hxc_actual;
          hxc_l_actual = hxc_l_tmp_enum_payload_project_n86;
          hxc_l_tmp_enum_payload_0_owner_n39 = hxc_l_candidate_h194daa8e982a.hxc_authoredId;
          if (hxc_string_retain(hxc_l_tmp_enum_payload_0_owner_n39) != HXC_STATUS_OK)
          {
            abort();
          }
          hxc_string hxc_l_tmp_enum_payload_0_owned_load_result_n88 = hxc_l_tmp_enum_payload_0_owner_n39;
          hxc_l_tmp_enum_payload_1_owner_n40 = hxc_l_candidate_h194daa8e982a.hxc_contentId;
          if (hxc_string_retain(hxc_l_tmp_enum_payload_1_owner_n40) != HXC_STATUS_OK)
          {
            abort();
          }
          hxc_string hxc_l_tmp_enum_payload_1_owned_load_result_n90 = hxc_l_tmp_enum_payload_1_owner_n40;
          enum hxc_caxecraft_content_ActorContentKind hxc_l_tmp_record_field_load_result_n91 = hxc_l_candidate_h194daa8e982a.hxc_expected;
          enum hxc_caxecraft_content_ActorContentKind hxc_l_tmp_load_result_n92 = hxc_l_actual;
          hxc_enum_23f1e692_destroy(&hxc_l_tmp_managed_flow_owner_n33);
          hxc_record_fc698661_destroy(&hxc_l_candidate_h194daa8e982a);
          hxc_array_d75ef385_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n31);
          if (hxc_array_ref_release(hxc_l_plans) != HXC_STATUS_OK)
          {
            abort();
          }
          if (hxc_array_ref_release(hxc_l_tmp_managed_flow_owner_n25) != HXC_STATUS_OK)
          {
            abort();
          }
          hxc_enum_67dc6ad1_destroy(&hxc_l_g_h2cf03206ee40);
          if (hxc_array_ref_release(hxc_l_authoredIds) != HXC_STATUS_OK)
          {
            abort();
          }
          if (hxc_array_ref_release(hxc_l_candidates) != HXC_STATUS_OK)
          {
            abort();
          }
          return (struct hxc_caxecraft_content_ActorCompositionResult){ .hxc_tag = hxc_caxecraft_content_ActorCompositionResult_ActorCompositionRejected, .hxc_payload.hxc_ActorCompositionRejected.hxc_error = (struct hxc_caxecraft_content_ActorCompositionError){ .hxc_tag = hxc_caxecraft_content_ActorCompositionError_PlacedActorKindMismatch, .hxc_payload.hxc_PlacedActorKindMismatch.hxc_authoredId = hxc_l_tmp_enum_payload_0_owned_load_result_n88, .hxc_payload.hxc_PlacedActorKindMismatch.hxc_contentId = hxc_l_tmp_enum_payload_1_owned_load_result_n90, .hxc_payload.hxc_PlacedActorKindMismatch.hxc_expected = hxc_l_tmp_record_field_load_result_n91, .hxc_payload.hxc_PlacedActorKindMismatch.hxc_actual = hxc_l_tmp_load_result_n92 } };
        }
      default:
        {
          abort();
        }
    }
    if (hxc_l_tmp_managed_flow_owner_n33.hxc_tag != hxc_caxecraft_content_ActorContentResolution_ActorContentResolved)
    {
      abort();
    }
    struct hxc_caxecraft_content_ActorMechanicsProfile hxc_l_tmp_enum_payload_project_n76 = hxc_l_tmp_managed_flow_owner_n33.hxc_payload.hxc_ActorContentResolved.hxc_profile;
    hxc_l_profile = hxc_l_tmp_enum_payload_project_n76;
    if (hxc_record_2b4afaaf_retain(&hxc_l_profile) != HXC_STATUS_OK)
    {
      abort();
    }
    (void)hxc_l_profile;
    hxc_l_mechanics = hxc_l_profile;
    if (hxc_record_2b4afaaf_retain(&hxc_l_mechanics) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_record_2b4afaaf_destroy(&hxc_l_profile);
    struct hxc_caxecraft_content_ActorMechanicsProfile hxc_l_tmp_managed_flow_carrier_move_result_n95 = hxc_l_mechanics;
    hxc_l_tmp_managed_flow_owner_n41 = hxc_l_tmp_managed_flow_carrier_move_result_n95;
    bool hxc_l_tmp_call_result_n97 = hxc_caxecraft_content_ActorMechanicsValidation_isValidActorMechanics(hxc_l_tmp_managed_flow_owner_n41);
    if (!hxc_l_tmp_call_result_n97)
    {
      hxc_l_tmp_enum_payload_0_owner_n42 = hxc_l_candidate_h194daa8e982a.hxc_authoredId;
      if (hxc_string_retain(hxc_l_tmp_enum_payload_0_owner_n42) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_string hxc_l_tmp_enum_payload_0_owned_load_result_n99 = hxc_l_tmp_enum_payload_0_owner_n42;
      hxc_l_tmp_enum_payload_1_owner_n43 = hxc_l_candidate_h194daa8e982a.hxc_contentId;
      if (hxc_string_retain(hxc_l_tmp_enum_payload_1_owner_n43) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_string hxc_l_tmp_enum_payload_1_owned_load_result_n101 = hxc_l_tmp_enum_payload_1_owner_n43;
      hxc_record_2b4afaaf_destroy(&hxc_l_tmp_managed_flow_owner_n41);
      hxc_enum_23f1e692_destroy(&hxc_l_tmp_managed_flow_owner_n33);
      hxc_record_fc698661_destroy(&hxc_l_candidate_h194daa8e982a);
      hxc_array_d75ef385_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n31);
      if (hxc_array_ref_release(hxc_l_plans) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_l_tmp_managed_flow_owner_n25) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_enum_67dc6ad1_destroy(&hxc_l_g_h2cf03206ee40);
      if (hxc_array_ref_release(hxc_l_authoredIds) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_l_candidates) != HXC_STATUS_OK)
      {
        abort();
      }
      return (struct hxc_caxecraft_content_ActorCompositionResult){ .hxc_tag = hxc_caxecraft_content_ActorCompositionResult_ActorCompositionRejected, .hxc_payload.hxc_ActorCompositionRejected.hxc_error = (struct hxc_caxecraft_content_ActorCompositionError){ .hxc_tag = hxc_caxecraft_content_ActorCompositionError_InvalidActorMechanics, .hxc_payload.hxc_InvalidActorMechanics.hxc_authoredId = hxc_l_tmp_enum_payload_0_owned_load_result_n99, .hxc_payload.hxc_InvalidActorMechanics.hxc_contentId = hxc_l_tmp_enum_payload_1_owned_load_result_n101 } };
    }
    struct hxc_array_ref *hxc_l_tmp_load_result_n104 = hxc_l_plans;
    hxc_string hxc_l_tmp_record_field_authoredId_owner_n44 = hxc_l_candidate_h194daa8e982a.hxc_authoredId;
    if (hxc_string_retain(hxc_l_tmp_record_field_authoredId_owner_n44) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_string hxc_l_tmp_record_field_authoredId_owned_load_result_n106 = hxc_l_tmp_record_field_authoredId_owner_n44;
    struct hxc_array_ref *hxc_l_tmp_managed_flow_owner_load_result_n107 = hxc_l_tmp_managed_flow_owner_n25;
    struct hxc_caxecraft_content_ActorIdentityBinding hxc_l_tmp_array_get_result_n109;
    if (hxc_array_ref_get_copy(hxc_l_tmp_managed_flow_owner_load_result_n107, (size_t)hxc_l_index, &hxc_l_tmp_array_get_result_n109) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_array_element_owner_n45 = hxc_l_tmp_array_get_result_n109;
    struct hxc_caxecraft_content_ActorIdentityBinding hxc_l_tmp_array_element_borrow_result_n110 = hxc_l_tmp_array_element_owner_n45;
    hxc_string hxc_l_tmp_record_field_contentId_owner_n46 = hxc_l_candidate_h194daa8e982a.hxc_contentId;
    if (hxc_string_retain(hxc_l_tmp_record_field_contentId_owner_n46) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_string hxc_l_tmp_record_field_contentId_owned_load_result_n113 = hxc_l_tmp_record_field_contentId_owner_n46;
    struct hxc_caxecraft_scenario_ScenarioTransform hxc_l_tmp_record_field_load_result_n114 = hxc_l_candidate_h194daa8e982a.hxc_transform;
    int32_t hxc_l_tmp_record_field_load_result_n115 = hxc_l_tmp_managed_flow_owner_n41.hxc_maximumHealth;
    struct hxc_caxecraft_domain_AquaticProfile hxc_l_tmp_record_field_load_result_n116 = hxc_l_tmp_managed_flow_owner_n41.hxc_aquaticProfile;
    struct hxc_caxecraft_domain_ActorControllerProfile hxc_l_tmp_record_field_controller_owner_n47 = hxc_l_tmp_managed_flow_owner_n41.hxc_controller;
    if (hxc_enum_25b991ce_retain(&hxc_l_tmp_record_field_controller_owner_n47) != HXC_STATUS_OK)
    {
      abort();
    }
    struct hxc_caxecraft_domain_ActorControllerProfile hxc_l_tmp_record_field_controller_owned_load_result_n118 = hxc_l_tmp_record_field_controller_owner_n47;
    struct hxc_caxecraft_content_CharacterSpawnRole hxc_l_tmp_record_field_role_owner_n48 = hxc_l_candidate_h194daa8e982a.hxc_role;
    if (hxc_enum_b133bfa7_retain(&hxc_l_tmp_record_field_role_owner_n48) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_array_push_element_owner_n49 = (struct hxc_caxecraft_content_CharacterSpawnPlan){ .hxc_aquaticProfile = hxc_l_tmp_record_field_load_result_n116, .hxc_authoredId = hxc_l_tmp_record_field_authoredId_owned_load_result_n106, .hxc_contentId = hxc_l_tmp_record_field_contentId_owned_load_result_n113, .hxc_controller = hxc_l_tmp_record_field_controller_owned_load_result_n118, .hxc_entityId = hxc_l_tmp_array_element_borrow_result_n110.hxc_entityId, .hxc_maximumHealth = hxc_l_tmp_record_field_load_result_n115, .hxc_role = hxc_l_tmp_record_field_role_owner_n48, .hxc_transform = hxc_l_tmp_record_field_load_result_n114 };
    int32_t hxc_l_tmp_array_push_result_n123;
    if (hxc_array_ref_push_copy(hxc_l_tmp_load_result_n104, &hxc_l_tmp_array_push_element_owner_n49, &hxc_l_tmp_array_push_result_n123) != HXC_STATUS_OK)
    {
      abort();
    }
    (void)hxc_l_tmp_array_push_result_n123;
    hxc_record_831d5670_destroy(&hxc_l_tmp_array_push_element_owner_n49);
    hxc_array_e4f8e97a_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n45);
    hxc_record_2b4afaaf_destroy(&hxc_l_tmp_managed_flow_owner_n41);
    hxc_enum_23f1e692_destroy(&hxc_l_tmp_managed_flow_owner_n33);
    hxc_record_fc698661_destroy(&hxc_l_candidate_h194daa8e982a);
    hxc_array_d75ef385_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n31);
  }
  struct hxc_array_ref *hxc_l_tmp_enum_payload_0_owner_n50 = hxc_l_plans;
  if (hxc_array_ref_retain(hxc_l_tmp_enum_payload_0_owner_n50) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_tmp_enum_payload_0_owned_load_result_n125 = hxc_l_tmp_enum_payload_0_owner_n50;
  if (hxc_array_ref_release(hxc_l_plans) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_ref_release(hxc_l_tmp_managed_flow_owner_n25) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_enum_67dc6ad1_destroy(&hxc_l_g_h2cf03206ee40);
  if (hxc_array_ref_release(hxc_l_authoredIds) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_ref_release(hxc_l_candidates) != HXC_STATUS_OK)
  {
    abort();
  }
  return (struct hxc_caxecraft_content_ActorCompositionResult){ .hxc_tag = hxc_caxecraft_content_ActorCompositionResult_ActorCompositionPlanned, .hxc_payload.hxc_ActorCompositionPlanned.hxc_plans = hxc_l_tmp_enum_payload_0_owned_load_result_n125 };
}
