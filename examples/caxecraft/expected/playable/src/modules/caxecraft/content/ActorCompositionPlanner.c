#include "hxc/program.h"

struct hxc_caxecraft_content_ActorCompositionResult hxc_caxecraft_content_ActorCompositionPlanner_planActorComposition(struct hxc_array_ref *hxc_objects, struct hxc_compiler_interface_dispatch_caxecraft_content_ActorContentResolver_value hxc_resolver)
{
  hxc_string hxc_npcType = { 0 };
  hxc_string hxc_dialogue = { 0 };
  struct hxc_caxecraft_content_ActorCompositionPlanner_CharacterCandidate hxc_tmp_managed_flow_owner_n19 = { 0 };
  struct hxc_caxecraft_content_ActorIdentityPlanResult hxc_g_h2cf03206ee40 = { 0 };
  struct hxc_caxecraft_content_ActorIdentityPlanError hxc_error = { 0 };
  struct hxc_caxecraft_content_ActorIdentityPlanError hxc_tmp_enum_payload_0_owner_n24 = { 0 };
  struct hxc_array_ref *hxc_tmp_managed_flow_owner_n25 = { 0 };
  struct hxc_array_ref *hxc_plans = { 0 };
  struct hxc_caxecraft_content_ActorCompositionPlanner_CharacterCandidate hxc_candidate_h194daa8e982a = { 0 };
  struct hxc_caxecraft_content_ActorCompositionPlanner_CharacterCandidate hxc_tmp_array_element_owner_n31 = { 0 };
  struct hxc_caxecraft_content_ActorContentResolution hxc_tmp_managed_flow_owner_n33 = { 0 };
  struct hxc_caxecraft_content_ActorMechanicsProfile hxc_profile = { 0 };
  hxc_string hxc_tmp_enum_payload_0_owner_n36 = { 0 };
  hxc_string hxc_tmp_enum_payload_1_owner_n37 = { 0 };
  enum hxc_caxecraft_content_ActorContentKind hxc_actual = { 0 };
  hxc_string hxc_tmp_enum_payload_0_owner_n39 = { 0 };
  hxc_string hxc_tmp_enum_payload_1_owner_n40 = { 0 };
  struct hxc_caxecraft_content_ActorMechanicsProfile hxc_tmp_managed_flow_owner_n41 = { 0 };
  hxc_string hxc_tmp_enum_payload_0_owner_n42 = { 0 };
  hxc_string hxc_tmp_enum_payload_1_owner_n43 = { 0 };
  struct hxc_caxecraft_content_ActorIdentityBinding hxc_tmp_array_element_owner_n45 = { 0 };
  struct hxc_caxecraft_content_CharacterSpawnPlan hxc_tmp_array_push_element_owner_n49 = { 0 };
  struct hxc_caxecraft_scenario_ScenarioObject hxc_object = { 0 };
  struct hxc_caxecraft_scenario_ScenarioObject hxc_tmp_array_element_owner_n6 = { 0 };
  struct hxc_caxecraft_scenario_ObjectPlacement hxc_g_h326d6465628d = { 0 };
  hxc_string hxc_entityType = { 0 };
  struct hxc_array_ref *hxc_tmp_array_create_result_n0 = NULL;
  if (hxc_array_ref_create(hxc_default_allocator(), (hxc_array_element_ops){ sizeof(struct hxc_caxecraft_content_ActorCompositionPlanner_CharacterCandidate), _Alignof(struct hxc_caxecraft_content_ActorCompositionPlanner_CharacterCandidate), NULL, hxc_array_d75ef385_element_copy, hxc_array_d75ef385_element_assign, hxc_array_d75ef385_element_destroy }, &hxc_tmp_array_create_result_n0) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_array_ref *hxc_candidates = hxc_tmp_array_create_result_n0;
  struct hxc_array_ref *hxc_tmp_array_create_result_n1 = NULL;
  if (hxc_array_ref_create(hxc_default_allocator(), (hxc_array_element_ops){ sizeof(hxc_string), _Alignof(hxc_string), NULL, hxc_array_4c9e169c_element_copy, hxc_array_4c9e169c_element_assign, hxc_array_4c9e169c_element_destroy }, &hxc_tmp_array_create_result_n1) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_array_ref *hxc_authoredIds = hxc_tmp_array_create_result_n1;
  int32_t hxc_g_h8e61e2e2bc1e = 0;
  while (1)
  {
    int32_t hxc_tmp_load_result_n2 = hxc_g_h8e61e2e2bc1e;
    int32_t hxc_tmp_array_length_result_n3;
    if (hxc_array_ref_length(hxc_objects, &hxc_tmp_array_length_result_n3) != HXC_STATUS_OK)
    {
      abort();
    }
    if (!(hxc_tmp_load_result_n2 < hxc_tmp_array_length_result_n3))
    {
      break;
    }
    struct hxc_caxecraft_scenario_ScenarioObject hxc_tmp_array_get_result_n5;
    if (hxc_array_ref_get_copy(hxc_objects, (size_t)hxc_g_h8e61e2e2bc1e, &hxc_tmp_array_get_result_n5) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_tmp_array_element_owner_n6 = hxc_tmp_array_get_result_n5;
    hxc_object = hxc_tmp_array_element_owner_n6;
    if (hxc_record_86003d77_retain(&hxc_object) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_g_h8e61e2e2bc1e = hxc_i32_add_wrapping(hxc_g_h8e61e2e2bc1e, 1);
    struct hxc_caxecraft_content_ActorCompositionPlanner_CharacterCandidate hxc_candidate_h4af786a80451;
    hxc_g_h326d6465628d = hxc_object.hxc_placement;
    if (hxc_enum_65d6d89d_retain(&hxc_g_h326d6465628d) != HXC_STATUS_OK)
    {
      abort();
    }
    switch (hxc_g_h326d6465628d.hxc_tag) {
      case hxc_caxecraft_scenario_ObjectPlacement_Entity:
        {
          if (hxc_g_h326d6465628d.hxc_tag != hxc_caxecraft_scenario_ObjectPlacement_Entity)
          {
            abort();
          }
          hxc_string hxc_tmp_enum_payload_project_n11 = hxc_g_h326d6465628d.hxc_payload.hxc_Entity.hxc_entityType;
          hxc_entityType = hxc_tmp_enum_payload_project_n11;
          if (hxc_string_retain(hxc_entityType) != HXC_STATUS_OK)
          {
            abort();
          }
          if (hxc_g_h326d6465628d.hxc_tag != hxc_caxecraft_scenario_ObjectPlacement_Entity)
          {
            abort();
          }
          struct hxc_caxecraft_scenario_ScenarioTransform hxc_tmp_enum_payload_project_n13 = hxc_g_h326d6465628d.hxc_payload.hxc_Entity.hxc_transform;
          struct hxc_caxecraft_scenario_ScenarioTransform hxc_transform_hf99cbe0f1295 = hxc_tmp_enum_payload_project_n13;
          hxc_string hxc_tmp_record_field_authoredId_owner_n11 = hxc_object.hxc_id;
          if (hxc_string_retain(hxc_tmp_record_field_authoredId_owner_n11) != HXC_STATUS_OK)
          {
            abort();
          }
          hxc_string hxc_tmp_record_field_authoredId_owned_load_result_n15 = hxc_tmp_record_field_authoredId_owner_n11;
          hxc_string hxc_tmp_record_field_contentId_owner_n12 = hxc_entityType;
          if (hxc_string_retain(hxc_tmp_record_field_contentId_owner_n12) != HXC_STATUS_OK)
          {
            abort();
          }
          hxc_string hxc_tmp_record_field_contentId_owned_load_result_n17 = hxc_tmp_record_field_contentId_owner_n12;
          (void)(struct hxc_caxecraft_content_ActorCompositionPlanner_CharacterCandidate){ .hxc_authoredId = hxc_tmp_record_field_authoredId_owned_load_result_n15, .hxc_contentId = hxc_tmp_record_field_contentId_owned_load_result_n17, .hxc_expected = hxc_caxecraft_content_ActorContentKind_EnemyContent, .hxc_role = (struct hxc_caxecraft_content_CharacterSpawnRole){ .hxc_tag = hxc_caxecraft_content_CharacterSpawnRole_EnemyActor }, .hxc_transform = hxc_transform_hf99cbe0f1295 };
          hxc_candidate_h4af786a80451 = (struct hxc_caxecraft_content_ActorCompositionPlanner_CharacterCandidate){ .hxc_authoredId = hxc_tmp_record_field_authoredId_owned_load_result_n15, .hxc_contentId = hxc_tmp_record_field_contentId_owned_load_result_n17, .hxc_expected = hxc_caxecraft_content_ActorContentKind_EnemyContent, .hxc_role = (struct hxc_caxecraft_content_CharacterSpawnRole){ .hxc_tag = hxc_caxecraft_content_CharacterSpawnRole_EnemyActor }, .hxc_transform = hxc_transform_hf99cbe0f1295 };
          if (hxc_string_release(&hxc_entityType) != HXC_STATUS_OK)
          {
            abort();
          }
          break;
        }
      case hxc_caxecraft_scenario_ObjectPlacement_Npc:
        {
          if (hxc_g_h326d6465628d.hxc_tag != hxc_caxecraft_scenario_ObjectPlacement_Npc)
          {
            abort();
          }
          hxc_string hxc_tmp_enum_payload_project_n23 = hxc_g_h326d6465628d.hxc_payload.hxc_Npc.hxc_npcType;
          hxc_npcType = hxc_tmp_enum_payload_project_n23;
          if (hxc_string_retain(hxc_npcType) != HXC_STATUS_OK)
          {
            abort();
          }
          if (hxc_g_h326d6465628d.hxc_tag != hxc_caxecraft_scenario_ObjectPlacement_Npc)
          {
            abort();
          }
          hxc_string hxc_tmp_enum_payload_project_n25 = hxc_g_h326d6465628d.hxc_payload.hxc_Npc.hxc_dialogue;
          hxc_dialogue = hxc_tmp_enum_payload_project_n25;
          if (hxc_string_retain(hxc_dialogue) != HXC_STATUS_OK)
          {
            abort();
          }
          if (hxc_g_h326d6465628d.hxc_tag != hxc_caxecraft_scenario_ObjectPlacement_Npc)
          {
            abort();
          }
          struct hxc_caxecraft_scenario_ScenarioTransform hxc_tmp_enum_payload_project_n27 = hxc_g_h326d6465628d.hxc_payload.hxc_Npc.hxc_transform;
          struct hxc_caxecraft_scenario_ScenarioTransform hxc_transform_hf1553301d735 = hxc_tmp_enum_payload_project_n27;
          hxc_string hxc_tmp_record_field_authoredId_owner_n16 = hxc_object.hxc_id;
          if (hxc_string_retain(hxc_tmp_record_field_authoredId_owner_n16) != HXC_STATUS_OK)
          {
            abort();
          }
          hxc_string hxc_tmp_record_field_authoredId_owned_load_result_n29 = hxc_tmp_record_field_authoredId_owner_n16;
          hxc_string hxc_tmp_record_field_contentId_owner_n17 = hxc_npcType;
          if (hxc_string_retain(hxc_tmp_record_field_contentId_owner_n17) != HXC_STATUS_OK)
          {
            abort();
          }
          hxc_string hxc_tmp_record_field_contentId_owned_load_result_n31 = hxc_tmp_record_field_contentId_owner_n17;
          struct hxc_caxecraft_scenario_ScenarioTransform hxc_tmp_load_result_n32 = hxc_transform_hf1553301d735;
          hxc_string hxc_tmp_enum_payload_0_owner_n18 = hxc_dialogue;
          if (hxc_string_retain(hxc_tmp_enum_payload_0_owner_n18) != HXC_STATUS_OK)
          {
            abort();
          }
          (void)(struct hxc_caxecraft_content_ActorCompositionPlanner_CharacterCandidate){ .hxc_authoredId = hxc_tmp_record_field_authoredId_owned_load_result_n29, .hxc_contentId = hxc_tmp_record_field_contentId_owned_load_result_n31, .hxc_expected = hxc_caxecraft_content_ActorContentKind_NpcContent, .hxc_role = (struct hxc_caxecraft_content_CharacterSpawnRole){ .hxc_tag = hxc_caxecraft_content_CharacterSpawnRole_DialogueNpc, .hxc_payload.hxc_DialogueNpc.hxc_dialogue = hxc_tmp_enum_payload_0_owner_n18 }, .hxc_transform = hxc_tmp_load_result_n32 };
          hxc_candidate_h4af786a80451 = (struct hxc_caxecraft_content_ActorCompositionPlanner_CharacterCandidate){ .hxc_authoredId = hxc_tmp_record_field_authoredId_owned_load_result_n29, .hxc_contentId = hxc_tmp_record_field_contentId_owned_load_result_n31, .hxc_expected = hxc_caxecraft_content_ActorContentKind_NpcContent, .hxc_role = (struct hxc_caxecraft_content_CharacterSpawnRole){ .hxc_tag = hxc_caxecraft_content_CharacterSpawnRole_DialogueNpc, .hxc_payload.hxc_DialogueNpc.hxc_dialogue = hxc_tmp_enum_payload_0_owner_n18 }, .hxc_transform = hxc_tmp_load_result_n32 };
          if (hxc_string_release(&hxc_dialogue) != HXC_STATUS_OK)
          {
            abort();
          }
          if (hxc_string_release(&hxc_npcType) != HXC_STATUS_OK)
          {
            abort();
          }
          break;
        }
      default:
        {
          hxc_enum_65d6d89d_destroy(&hxc_g_h326d6465628d);
          hxc_record_86003d77_destroy(&hxc_object);
          hxc_array_223dc263_element_destroy(NULL, &hxc_tmp_array_element_owner_n6);
          continue;
        }
    }
    struct hxc_caxecraft_content_ActorCompositionPlanner_CharacterCandidate hxc_tmp_managed_flow_carrier_move_result_n38 = hxc_candidate_h4af786a80451;
    hxc_tmp_managed_flow_owner_n19 = hxc_tmp_managed_flow_carrier_move_result_n38;
    struct hxc_array_ref *hxc_tmp_load_result_n39 = hxc_candidates;
    int32_t hxc_tmp_array_push_result_n41;
    if (hxc_array_ref_push_copy(hxc_tmp_load_result_n39, &hxc_tmp_managed_flow_owner_n19, &hxc_tmp_array_push_result_n41) != HXC_STATUS_OK)
    {
      abort();
    }
    (void)hxc_tmp_array_push_result_n41;
    struct hxc_array_ref *hxc_tmp_load_result_n42 = hxc_authoredIds;
    int32_t hxc_tmp_array_push_result_n44;
    if (hxc_array_ref_push_copy(hxc_tmp_load_result_n42, &hxc_tmp_managed_flow_owner_n19.hxc_authoredId, &hxc_tmp_array_push_result_n44) != HXC_STATUS_OK)
    {
      abort();
    }
    (void)hxc_tmp_array_push_result_n44;
    hxc_record_fc698661_destroy(&hxc_tmp_managed_flow_owner_n19);
    hxc_enum_65d6d89d_destroy(&hxc_g_h326d6465628d);
    hxc_record_86003d77_destroy(&hxc_object);
    hxc_array_223dc263_element_destroy(NULL, &hxc_tmp_array_element_owner_n6);
  }
  struct hxc_array_ref *hxc_identities;
  struct hxc_caxecraft_content_ActorIdentityPlanResult hxc_tmp_call_result_n46 = hxc_caxecraft_content_ActorIdentityPlanner_planActorIdentities(hxc_authoredIds);
  hxc_g_h2cf03206ee40 = hxc_tmp_call_result_n46;
  switch (hxc_g_h2cf03206ee40.hxc_tag) {
    case hxc_caxecraft_content_ActorIdentityPlanResult_Planned:
      {
        break;
      }
    case hxc_caxecraft_content_ActorIdentityPlanResult_Rejected:
      {
        if (hxc_g_h2cf03206ee40.hxc_tag != hxc_caxecraft_content_ActorIdentityPlanResult_Rejected)
        {
          abort();
        }
        struct hxc_caxecraft_content_ActorIdentityPlanError hxc_tmp_enum_payload_project_n52 = hxc_g_h2cf03206ee40.hxc_payload.hxc_Rejected.hxc_error;
        hxc_error = hxc_tmp_enum_payload_project_n52;
        if (hxc_enum_c16d2c34_retain(&hxc_error) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_tmp_enum_payload_0_owner_n24 = hxc_error;
        if (hxc_enum_c16d2c34_retain(&hxc_tmp_enum_payload_0_owner_n24) != HXC_STATUS_OK)
        {
          abort();
        }
        struct hxc_caxecraft_content_ActorIdentityPlanError hxc_tmp_enum_payload_0_owned_load_result_n54 = hxc_tmp_enum_payload_0_owner_n24;
        hxc_enum_c16d2c34_destroy(&hxc_error);
        hxc_enum_67dc6ad1_destroy(&hxc_g_h2cf03206ee40);
        if (hxc_array_ref_release(hxc_authoredIds) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_array_ref_release(hxc_candidates) != HXC_STATUS_OK)
        {
          abort();
        }
        return (struct hxc_caxecraft_content_ActorCompositionResult){ .hxc_tag = hxc_caxecraft_content_ActorCompositionResult_ActorCompositionRejected, .hxc_payload.hxc_ActorCompositionRejected.hxc_error = (struct hxc_caxecraft_content_ActorCompositionError){ .hxc_tag = hxc_caxecraft_content_ActorCompositionError_ActorIdentityRejected, .hxc_payload.hxc_ActorIdentityRejected.hxc_error = hxc_tmp_enum_payload_0_owned_load_result_n54 } };
      }
  }
  if (hxc_g_h2cf03206ee40.hxc_tag != hxc_caxecraft_content_ActorIdentityPlanResult_Planned)
  {
    abort();
  }
  struct hxc_array_ref *hxc_tmp_enum_payload_project_n49 = hxc_g_h2cf03206ee40.hxc_payload.hxc_Planned.hxc_bindings;
  struct hxc_array_ref *hxc_bindings = hxc_tmp_enum_payload_project_n49;
  (void)hxc_bindings;
  hxc_identities = hxc_bindings;
  if (hxc_array_ref_retain(hxc_identities) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_array_ref *hxc_tmp_managed_flow_carrier_move_result_n57 = hxc_identities;
  hxc_tmp_managed_flow_owner_n25 = hxc_tmp_managed_flow_carrier_move_result_n57;
  struct hxc_array_ref *hxc_tmp_array_create_result_n58 = NULL;
  if (hxc_array_ref_create(hxc_default_allocator(), (hxc_array_element_ops){ sizeof(struct hxc_caxecraft_content_CharacterSpawnPlan), _Alignof(struct hxc_caxecraft_content_CharacterSpawnPlan), NULL, hxc_array_78e5c5d1_element_copy, hxc_array_78e5c5d1_element_assign, hxc_array_78e5c5d1_element_destroy }, &hxc_tmp_array_create_result_n58) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_plans = hxc_tmp_array_create_result_n58;
  int32_t hxc_g_h738bba3c77cc = 0;
  int32_t hxc_tmp_array_length_result_n60;
  if (hxc_array_ref_length(hxc_candidates, &hxc_tmp_array_length_result_n60) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_g1 = hxc_tmp_array_length_result_n60;
  while (1)
  {
    int32_t hxc_tmp_load_result_n61 = hxc_g_h738bba3c77cc;
    if (!(hxc_tmp_load_result_n61 < hxc_g1))
    {
      break;
    }
    int32_t hxc_tmp_increment_load_result_n63 = hxc_g_h738bba3c77cc;
    hxc_g_h738bba3c77cc = hxc_i32_add_wrapping(hxc_tmp_increment_load_result_n63, 1);
    int32_t hxc_index = hxc_tmp_increment_load_result_n63;
    struct hxc_array_ref *hxc_tmp_load_result_n64 = hxc_candidates;
    struct hxc_caxecraft_content_ActorCompositionPlanner_CharacterCandidate hxc_tmp_array_get_result_n66;
    if (hxc_array_ref_get_copy(hxc_tmp_load_result_n64, (size_t)hxc_index, &hxc_tmp_array_get_result_n66) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_tmp_array_element_owner_n31 = hxc_tmp_array_get_result_n66;
    hxc_candidate_h194daa8e982a = hxc_tmp_array_element_owner_n31;
    if (hxc_record_fc698661_retain(&hxc_candidate_h194daa8e982a) != HXC_STATUS_OK)
    {
      abort();
    }
    struct hxc_caxecraft_content_ActorContentResolution hxc_resolution;
    switch (hxc_candidate_h194daa8e982a.hxc_expected) {
      case hxc_caxecraft_content_ActorContentKind_NpcContent:
        {
          struct hxc_caxecraft_content_ActorContentResolution hxc_tmp_instance_call_result_n70 = hxc_resolver.table->hxc_interface_slot_caxecraft_content_ActorContentResolver_resolveNpc(hxc_resolver.object, hxc_candidate_h194daa8e982a.hxc_contentId);
          hxc_resolution = hxc_tmp_instance_call_result_n70;
          break;
        }
      case hxc_caxecraft_content_ActorContentKind_EnemyContent:
        {
          struct hxc_caxecraft_content_ActorContentResolution hxc_tmp_instance_call_result_n72 = hxc_resolver.table->hxc_interface_slot_caxecraft_content_ActorContentResolver_resolveEnemy(hxc_resolver.object, hxc_candidate_h194daa8e982a.hxc_contentId);
          hxc_resolution = hxc_tmp_instance_call_result_n72;
          break;
        }
    }
    struct hxc_caxecraft_content_ActorContentResolution hxc_tmp_managed_flow_carrier_move_result_n73 = hxc_resolution;
    hxc_tmp_managed_flow_owner_n33 = hxc_tmp_managed_flow_carrier_move_result_n73;
    struct hxc_caxecraft_content_ActorMechanicsProfile hxc_mechanics;
    switch (hxc_tmp_managed_flow_owner_n33.hxc_tag) {
      case hxc_caxecraft_content_ActorContentResolution_ActorContentResolved:
        {
          break;
        }
      case hxc_caxecraft_content_ActorContentResolution_UnknownActorContent:
        {
          hxc_tmp_enum_payload_0_owner_n36 = hxc_candidate_h194daa8e982a.hxc_authoredId;
          if (hxc_string_retain(hxc_tmp_enum_payload_0_owner_n36) != HXC_STATUS_OK)
          {
            abort();
          }
          hxc_string hxc_tmp_enum_payload_0_owned_load_result_n79 = hxc_tmp_enum_payload_0_owner_n36;
          hxc_tmp_enum_payload_1_owner_n37 = hxc_candidate_h194daa8e982a.hxc_contentId;
          if (hxc_string_retain(hxc_tmp_enum_payload_1_owner_n37) != HXC_STATUS_OK)
          {
            abort();
          }
          hxc_string hxc_tmp_enum_payload_1_owned_load_result_n81 = hxc_tmp_enum_payload_1_owner_n37;
          enum hxc_caxecraft_content_ActorContentKind hxc_tmp_record_field_load_result_n82 = hxc_candidate_h194daa8e982a.hxc_expected;
          hxc_enum_23f1e692_destroy(&hxc_tmp_managed_flow_owner_n33);
          hxc_record_fc698661_destroy(&hxc_candidate_h194daa8e982a);
          hxc_array_d75ef385_element_destroy(NULL, &hxc_tmp_array_element_owner_n31);
          if (hxc_array_ref_release(hxc_plans) != HXC_STATUS_OK)
          {
            abort();
          }
          if (hxc_array_ref_release(hxc_tmp_managed_flow_owner_n25) != HXC_STATUS_OK)
          {
            abort();
          }
          hxc_enum_67dc6ad1_destroy(&hxc_g_h2cf03206ee40);
          if (hxc_array_ref_release(hxc_authoredIds) != HXC_STATUS_OK)
          {
            abort();
          }
          if (hxc_array_ref_release(hxc_candidates) != HXC_STATUS_OK)
          {
            abort();
          }
          return (struct hxc_caxecraft_content_ActorCompositionResult){ .hxc_tag = hxc_caxecraft_content_ActorCompositionResult_ActorCompositionRejected, .hxc_payload.hxc_ActorCompositionRejected.hxc_error = (struct hxc_caxecraft_content_ActorCompositionError){ .hxc_tag = hxc_caxecraft_content_ActorCompositionError_UnknownPlacedActor, .hxc_payload.hxc_UnknownPlacedActor.hxc_authoredId = hxc_tmp_enum_payload_0_owned_load_result_n79, .hxc_payload.hxc_UnknownPlacedActor.hxc_contentId = hxc_tmp_enum_payload_1_owned_load_result_n81, .hxc_payload.hxc_UnknownPlacedActor.hxc_expected = hxc_tmp_record_field_load_result_n82 } };
        }
      case hxc_caxecraft_content_ActorContentResolution_WrongActorContentKind:
        {
          if (hxc_tmp_managed_flow_owner_n33.hxc_tag != hxc_caxecraft_content_ActorContentResolution_WrongActorContentKind)
          {
            abort();
          }
          enum hxc_caxecraft_content_ActorContentKind hxc_tmp_enum_payload_project_n86 = hxc_tmp_managed_flow_owner_n33.hxc_payload.hxc_WrongActorContentKind.hxc_actual;
          hxc_actual = hxc_tmp_enum_payload_project_n86;
          hxc_tmp_enum_payload_0_owner_n39 = hxc_candidate_h194daa8e982a.hxc_authoredId;
          if (hxc_string_retain(hxc_tmp_enum_payload_0_owner_n39) != HXC_STATUS_OK)
          {
            abort();
          }
          hxc_string hxc_tmp_enum_payload_0_owned_load_result_n88 = hxc_tmp_enum_payload_0_owner_n39;
          hxc_tmp_enum_payload_1_owner_n40 = hxc_candidate_h194daa8e982a.hxc_contentId;
          if (hxc_string_retain(hxc_tmp_enum_payload_1_owner_n40) != HXC_STATUS_OK)
          {
            abort();
          }
          hxc_string hxc_tmp_enum_payload_1_owned_load_result_n90 = hxc_tmp_enum_payload_1_owner_n40;
          enum hxc_caxecraft_content_ActorContentKind hxc_tmp_record_field_load_result_n91 = hxc_candidate_h194daa8e982a.hxc_expected;
          enum hxc_caxecraft_content_ActorContentKind hxc_tmp_load_result_n92 = hxc_actual;
          hxc_enum_23f1e692_destroy(&hxc_tmp_managed_flow_owner_n33);
          hxc_record_fc698661_destroy(&hxc_candidate_h194daa8e982a);
          hxc_array_d75ef385_element_destroy(NULL, &hxc_tmp_array_element_owner_n31);
          if (hxc_array_ref_release(hxc_plans) != HXC_STATUS_OK)
          {
            abort();
          }
          if (hxc_array_ref_release(hxc_tmp_managed_flow_owner_n25) != HXC_STATUS_OK)
          {
            abort();
          }
          hxc_enum_67dc6ad1_destroy(&hxc_g_h2cf03206ee40);
          if (hxc_array_ref_release(hxc_authoredIds) != HXC_STATUS_OK)
          {
            abort();
          }
          if (hxc_array_ref_release(hxc_candidates) != HXC_STATUS_OK)
          {
            abort();
          }
          return (struct hxc_caxecraft_content_ActorCompositionResult){ .hxc_tag = hxc_caxecraft_content_ActorCompositionResult_ActorCompositionRejected, .hxc_payload.hxc_ActorCompositionRejected.hxc_error = (struct hxc_caxecraft_content_ActorCompositionError){ .hxc_tag = hxc_caxecraft_content_ActorCompositionError_PlacedActorKindMismatch, .hxc_payload.hxc_PlacedActorKindMismatch.hxc_authoredId = hxc_tmp_enum_payload_0_owned_load_result_n88, .hxc_payload.hxc_PlacedActorKindMismatch.hxc_contentId = hxc_tmp_enum_payload_1_owned_load_result_n90, .hxc_payload.hxc_PlacedActorKindMismatch.hxc_expected = hxc_tmp_record_field_load_result_n91, .hxc_payload.hxc_PlacedActorKindMismatch.hxc_actual = hxc_tmp_load_result_n92 } };
        }
    }
    if (hxc_tmp_managed_flow_owner_n33.hxc_tag != hxc_caxecraft_content_ActorContentResolution_ActorContentResolved)
    {
      abort();
    }
    struct hxc_caxecraft_content_ActorMechanicsProfile hxc_tmp_enum_payload_project_n76 = hxc_tmp_managed_flow_owner_n33.hxc_payload.hxc_ActorContentResolved.hxc_profile;
    hxc_profile = hxc_tmp_enum_payload_project_n76;
    if (hxc_record_2b4afaaf_retain(&hxc_profile) != HXC_STATUS_OK)
    {
      abort();
    }
    (void)hxc_profile;
    hxc_mechanics = hxc_profile;
    if (hxc_record_2b4afaaf_retain(&hxc_mechanics) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_record_2b4afaaf_destroy(&hxc_profile);
    struct hxc_caxecraft_content_ActorMechanicsProfile hxc_tmp_managed_flow_carrier_move_result_n95 = hxc_mechanics;
    hxc_tmp_managed_flow_owner_n41 = hxc_tmp_managed_flow_carrier_move_result_n95;
    bool hxc_tmp_call_result_n97 = hxc_caxecraft_content_ActorMechanicsValidation_isValidActorMechanics(hxc_tmp_managed_flow_owner_n41);
    if (!hxc_tmp_call_result_n97)
    {
      hxc_tmp_enum_payload_0_owner_n42 = hxc_candidate_h194daa8e982a.hxc_authoredId;
      if (hxc_string_retain(hxc_tmp_enum_payload_0_owner_n42) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_string hxc_tmp_enum_payload_0_owned_load_result_n99 = hxc_tmp_enum_payload_0_owner_n42;
      hxc_tmp_enum_payload_1_owner_n43 = hxc_candidate_h194daa8e982a.hxc_contentId;
      if (hxc_string_retain(hxc_tmp_enum_payload_1_owner_n43) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_string hxc_tmp_enum_payload_1_owned_load_result_n101 = hxc_tmp_enum_payload_1_owner_n43;
      hxc_record_2b4afaaf_destroy(&hxc_tmp_managed_flow_owner_n41);
      hxc_enum_23f1e692_destroy(&hxc_tmp_managed_flow_owner_n33);
      hxc_record_fc698661_destroy(&hxc_candidate_h194daa8e982a);
      hxc_array_d75ef385_element_destroy(NULL, &hxc_tmp_array_element_owner_n31);
      if (hxc_array_ref_release(hxc_plans) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_tmp_managed_flow_owner_n25) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_enum_67dc6ad1_destroy(&hxc_g_h2cf03206ee40);
      if (hxc_array_ref_release(hxc_authoredIds) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_candidates) != HXC_STATUS_OK)
      {
        abort();
      }
      return (struct hxc_caxecraft_content_ActorCompositionResult){ .hxc_tag = hxc_caxecraft_content_ActorCompositionResult_ActorCompositionRejected, .hxc_payload.hxc_ActorCompositionRejected.hxc_error = (struct hxc_caxecraft_content_ActorCompositionError){ .hxc_tag = hxc_caxecraft_content_ActorCompositionError_InvalidActorMechanics, .hxc_payload.hxc_InvalidActorMechanics.hxc_authoredId = hxc_tmp_enum_payload_0_owned_load_result_n99, .hxc_payload.hxc_InvalidActorMechanics.hxc_contentId = hxc_tmp_enum_payload_1_owned_load_result_n101 } };
    }
    struct hxc_array_ref *hxc_tmp_load_result_n104 = hxc_plans;
    hxc_string hxc_tmp_record_field_authoredId_owner_n44 = hxc_candidate_h194daa8e982a.hxc_authoredId;
    if (hxc_string_retain(hxc_tmp_record_field_authoredId_owner_n44) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_string hxc_tmp_record_field_authoredId_owned_load_result_n106 = hxc_tmp_record_field_authoredId_owner_n44;
    struct hxc_array_ref *hxc_tmp_managed_flow_owner_load_result_n107 = hxc_tmp_managed_flow_owner_n25;
    struct hxc_caxecraft_content_ActorIdentityBinding hxc_tmp_array_get_result_n109;
    if (hxc_array_ref_get_copy(hxc_tmp_managed_flow_owner_load_result_n107, (size_t)hxc_index, &hxc_tmp_array_get_result_n109) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_tmp_array_element_owner_n45 = hxc_tmp_array_get_result_n109;
    struct hxc_caxecraft_content_ActorIdentityBinding hxc_tmp_array_element_borrow_result_n110 = hxc_tmp_array_element_owner_n45;
    hxc_string hxc_tmp_record_field_contentId_owner_n46 = hxc_candidate_h194daa8e982a.hxc_contentId;
    if (hxc_string_retain(hxc_tmp_record_field_contentId_owner_n46) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_string hxc_tmp_record_field_contentId_owned_load_result_n113 = hxc_tmp_record_field_contentId_owner_n46;
    struct hxc_caxecraft_scenario_ScenarioTransform hxc_tmp_record_field_load_result_n114 = hxc_candidate_h194daa8e982a.hxc_transform;
    int32_t hxc_tmp_record_field_load_result_n115 = hxc_tmp_managed_flow_owner_n41.hxc_maximumHealth;
    struct hxc_caxecraft_domain_AquaticProfile hxc_tmp_record_field_load_result_n116 = hxc_tmp_managed_flow_owner_n41.hxc_aquaticProfile;
    struct hxc_caxecraft_domain_ActorControllerProfile hxc_tmp_record_field_controller_owner_n47 = hxc_tmp_managed_flow_owner_n41.hxc_controller;
    if (hxc_enum_25b991ce_retain(&hxc_tmp_record_field_controller_owner_n47) != HXC_STATUS_OK)
    {
      abort();
    }
    struct hxc_caxecraft_domain_ActorControllerProfile hxc_tmp_record_field_controller_owned_load_result_n118 = hxc_tmp_record_field_controller_owner_n47;
    struct hxc_caxecraft_content_CharacterSpawnRole hxc_tmp_record_field_role_owner_n48 = hxc_candidate_h194daa8e982a.hxc_role;
    if (hxc_enum_b133bfa7_retain(&hxc_tmp_record_field_role_owner_n48) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_tmp_array_push_element_owner_n49 = (struct hxc_caxecraft_content_CharacterSpawnPlan){ .hxc_aquaticProfile = hxc_tmp_record_field_load_result_n116, .hxc_authoredId = hxc_tmp_record_field_authoredId_owned_load_result_n106, .hxc_contentId = hxc_tmp_record_field_contentId_owned_load_result_n113, .hxc_controller = hxc_tmp_record_field_controller_owned_load_result_n118, .hxc_entityId = hxc_tmp_array_element_borrow_result_n110.hxc_entityId, .hxc_maximumHealth = hxc_tmp_record_field_load_result_n115, .hxc_role = hxc_tmp_record_field_role_owner_n48, .hxc_transform = hxc_tmp_record_field_load_result_n114 };
    int32_t hxc_tmp_array_push_result_n123;
    if (hxc_array_ref_push_copy(hxc_tmp_load_result_n104, &hxc_tmp_array_push_element_owner_n49, &hxc_tmp_array_push_result_n123) != HXC_STATUS_OK)
    {
      abort();
    }
    (void)hxc_tmp_array_push_result_n123;
    hxc_record_831d5670_destroy(&hxc_tmp_array_push_element_owner_n49);
    hxc_array_e4f8e97a_element_destroy(NULL, &hxc_tmp_array_element_owner_n45);
    hxc_record_2b4afaaf_destroy(&hxc_tmp_managed_flow_owner_n41);
    hxc_enum_23f1e692_destroy(&hxc_tmp_managed_flow_owner_n33);
    hxc_record_fc698661_destroy(&hxc_candidate_h194daa8e982a);
    hxc_array_d75ef385_element_destroy(NULL, &hxc_tmp_array_element_owner_n31);
  }
  struct hxc_array_ref *hxc_tmp_enum_payload_0_owner_n50 = hxc_plans;
  if (hxc_array_ref_retain(hxc_tmp_enum_payload_0_owner_n50) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_array_ref *hxc_tmp_enum_payload_0_owned_load_result_n125 = hxc_tmp_enum_payload_0_owner_n50;
  if (hxc_array_ref_release(hxc_plans) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_ref_release(hxc_tmp_managed_flow_owner_n25) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_enum_67dc6ad1_destroy(&hxc_g_h2cf03206ee40);
  if (hxc_array_ref_release(hxc_authoredIds) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_ref_release(hxc_candidates) != HXC_STATUS_OK)
  {
    abort();
  }
  return (struct hxc_caxecraft_content_ActorCompositionResult){ .hxc_tag = hxc_caxecraft_content_ActorCompositionResult_ActorCompositionPlanned, .hxc_payload.hxc_ActorCompositionPlanned.hxc_plans = hxc_tmp_enum_payload_0_owned_load_result_n125 };
}
