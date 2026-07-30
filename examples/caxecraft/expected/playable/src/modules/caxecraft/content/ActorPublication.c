#include "hxc/program.h"

struct hxc_caxecraft_content_ActorPublicationResult hxc_caxecraft_content_ActorPublication_publishActorPlans(struct hxc_caxecraft_domain_GameSession *hxc_l_session, struct hxc_array_ref *hxc_l_plans)
{
  const void *hxc_l_gc_roots[1] = { (const void *)hxc_l_session };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 1, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_caxecraft_domain_ActorControllerBinding hxc_l_tmp_array_push_element_owner_n11 = { 0 };
  struct hxc_caxecraft_content_CharacterSpawnPlan hxc_l_plan = { 0 };
  struct hxc_caxecraft_content_CharacterSpawnPlan hxc_l_tmp_array_element_owner_n8 = { 0 };
  struct hxc_array_ref *hxc_l_tmp_array_create_result_n0 = NULL;
  if (hxc_array_ref_create_trivial(hxc_default_allocator(), sizeof(struct hxc_caxecraft_domain_Character), _Alignof(struct hxc_caxecraft_domain_Character), &hxc_l_tmp_array_create_result_n0) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_characters = hxc_l_tmp_array_create_result_n0;
  struct hxc_array_ref *hxc_l_tmp_array_create_result_n1 = NULL;
  if (hxc_array_ref_create(hxc_default_allocator(), (hxc_array_element_ops){ sizeof(struct hxc_caxecraft_domain_ActorControllerBinding), _Alignof(struct hxc_caxecraft_domain_ActorControllerBinding), NULL, hxc_array_622f12db_element_copy, hxc_array_622f12db_element_assign, hxc_array_622f12db_element_destroy }, &hxc_l_tmp_array_create_result_n1) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_controllers = hxc_l_tmp_array_create_result_n1;
  int32_t hxc_l_g = 0;
  int32_t hxc_l_tmp_array_length_result_n2;
  if (hxc_array_ref_length(hxc_l_plans, &hxc_l_tmp_array_length_result_n2) != HXC_STATUS_OK)
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
    struct hxc_caxecraft_content_CharacterSpawnPlan hxc_l_tmp_array_get_result_n7;
    if (hxc_array_ref_get_copy(hxc_l_plans, (size_t)hxc_l_index, &hxc_l_tmp_array_get_result_n7) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_array_element_owner_n8 = hxc_l_tmp_array_get_result_n7;
    hxc_l_plan = hxc_l_tmp_array_element_owner_n8;
    if (hxc_record_831d5670_retain(&hxc_l_plan) != HXC_STATUS_OK)
    {
      abort();
    }
    bool hxc_l_tmp_call_result_n10 = hxc_caxecraft_content_ActorPublication_validPlan(hxc_l_plan);
    if (!hxc_l_tmp_call_result_n10)
    {
      int32_t hxc_l_tmp_load_result_n11 = hxc_l_index;
      hxc_record_831d5670_destroy(&hxc_l_plan);
      hxc_array_78e5c5d1_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n8);
      if (hxc_array_ref_release(hxc_l_controllers) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_l_characters) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
      {
        abort();
      }
      return (struct hxc_caxecraft_content_ActorPublicationResult){ .hxc_tag = hxc_caxecraft_content_ActorPublicationResult_ActorPublicationRejected, .hxc_payload.hxc_ActorPublicationRejected.hxc_error = (struct hxc_caxecraft_content_ActorPublicationError){ .hxc_tag = hxc_caxecraft_content_ActorPublicationError_InvalidRuntimePlan, .hxc_payload.hxc_InvalidRuntimePlan.hxc_index = hxc_l_tmp_load_result_n11 } };
    }
    int32_t hxc_l_tmp_record_field_load_result_n14 = hxc_l_plan.hxc_entityId;
    int32_t hxc_l_tmp_record_field_load_result_n15 = hxc_l_plan.hxc_transform.hxc_xMilli;
    int32_t hxc_l_tmp_record_field_load_result_n16 = hxc_l_plan.hxc_transform.hxc_yMilli;
    struct hxc_caxecraft_domain_CharacterBody hxc_l_tmp_call_result_n18 = hxc_caxecraft_domain_CharacterPhysics_body(hxc_f64_divide_zero_safe((double)hxc_l_tmp_record_field_load_result_n15, 1000.0), hxc_f64_divide_zero_safe((double)hxc_l_tmp_record_field_load_result_n16, 1000.0), hxc_f64_divide_zero_safe((double)hxc_l_plan.hxc_transform.hxc_zMilli, 1000.0));
    struct hxc_caxecraft_domain_AquaticProfile hxc_l_tmp_record_field_load_result_n19 = hxc_l_plan.hxc_aquaticProfile;
    struct hxc_caxecraft_domain_Character hxc_l_tmp_call_result_n21 = hxc_caxecraft_domain_Character_start(hxc_l_tmp_record_field_load_result_n14, hxc_l_tmp_call_result_n18, hxc_l_tmp_record_field_load_result_n19, hxc_l_plan.hxc_maximumHealth);
    struct hxc_caxecraft_domain_Character hxc_l_character = hxc_l_tmp_call_result_n21;
    struct hxc_array_ref *hxc_l_tmp_load_result_n22 = hxc_l_characters;
    int32_t hxc_l_tmp_array_push_result_n24;
    if (hxc_array_ref_push_copy(hxc_l_tmp_load_result_n22, &hxc_l_character, &hxc_l_tmp_array_push_result_n24) != HXC_STATUS_OK)
    {
      abort();
    }
    (void)hxc_l_tmp_array_push_result_n24;
    struct hxc_array_ref *hxc_l_tmp_load_result_n25 = hxc_l_controllers;
    int32_t hxc_l_tmp_record_field_load_result_n26 = hxc_l_plan.hxc_entityId;
    struct hxc_caxecraft_domain_ActorControllerProfile hxc_l_tmp_record_field_profile_owner_n10 = hxc_l_plan.hxc_controller;
    if (hxc_enum_25b991ce_retain(&hxc_l_tmp_record_field_profile_owner_n10) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_array_push_element_owner_n11 = (struct hxc_caxecraft_domain_ActorControllerBinding){ .hxc_characterId = hxc_l_tmp_record_field_load_result_n26, .hxc_profile = hxc_l_tmp_record_field_profile_owner_n10 };
    int32_t hxc_l_tmp_array_push_result_n31;
    if (hxc_array_ref_push_copy(hxc_l_tmp_load_result_n25, &hxc_l_tmp_array_push_element_owner_n11, &hxc_l_tmp_array_push_result_n31) != HXC_STATUS_OK)
    {
      abort();
    }
    (void)hxc_l_tmp_array_push_result_n31;
    hxc_record_5530b001_destroy(&hxc_l_tmp_array_push_element_owner_n11);
    hxc_record_831d5670_destroy(&hxc_l_plan);
    hxc_array_78e5c5d1_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n8);
  }
  if (hxc_l_session == NULL)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_tmp_load_result_n32 = hxc_l_characters;
  bool hxc_l_tmp_instance_call_result_n34 = hxc_caxecraft_domain_GameSession_replaceAuthoredActors(hxc_l_session, hxc_l_tmp_load_result_n32, hxc_l_controllers);
  if (!hxc_l_tmp_instance_call_result_n34)
  {
    if (hxc_array_ref_release(hxc_l_controllers) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_characters) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return (struct hxc_caxecraft_content_ActorPublicationResult){ .hxc_tag = hxc_caxecraft_content_ActorPublicationResult_ActorPublicationRejected, .hxc_payload.hxc_ActorPublicationRejected.hxc_error = (struct hxc_caxecraft_content_ActorPublicationError){ .hxc_tag = hxc_caxecraft_content_ActorPublicationError_SessionPublicationRejected } };
  }
  int32_t hxc_l_tmp_array_length_result_n36;
  if (hxc_array_ref_length(hxc_l_characters, &hxc_l_tmp_array_length_result_n36) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_ref_release(hxc_l_controllers) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_ref_release(hxc_l_characters) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return (struct hxc_caxecraft_content_ActorPublicationResult){ .hxc_tag = hxc_caxecraft_content_ActorPublicationResult_ActorsPublished, .hxc_payload.hxc_ActorsPublished.hxc_count = hxc_l_tmp_array_length_result_n36 };
}

bool hxc_caxecraft_content_ActorPublication_validPlan(struct hxc_caxecraft_content_CharacterSpawnPlan hxc_l_plan)
{
  struct hxc_caxecraft_content_ActorMechanicsProfile hxc_l_tmp_static_call_argument_0_owner_n3 = { 0 };
  bool hxc_l_tmp_short_circuit_result_n1 = hxc_l_plan.hxc_entityId > 0;
  if (hxc_l_plan.hxc_entityId > 0)
  {
    struct hxc_caxecraft_domain_ActorControllerProfile hxc_l_tmp_record_field_controller_owner_n2 = hxc_l_plan.hxc_controller;
    if (hxc_enum_25b991ce_retain(&hxc_l_tmp_record_field_controller_owner_n2) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_static_call_argument_0_owner_n3 = (struct hxc_caxecraft_content_ActorMechanicsProfile){ .hxc_aquaticProfile = hxc_l_plan.hxc_aquaticProfile, .hxc_controller = hxc_l_tmp_record_field_controller_owner_n2, .hxc_maximumHealth = hxc_l_plan.hxc_maximumHealth };
    bool hxc_l_tmp_call_result_n7 = hxc_caxecraft_content_ActorMechanicsValidation_isValidActorMechanics(hxc_l_tmp_static_call_argument_0_owner_n3);
    hxc_l_tmp_short_circuit_result_n1 = hxc_l_tmp_call_result_n7;
    hxc_record_2b4afaaf_destroy(&hxc_l_tmp_static_call_argument_0_owner_n3);
  }
  bool hxc_l_tmp_short_circuit_load_result_n8 = hxc_l_tmp_short_circuit_result_n1;
  bool hxc_l_tmp_short_circuit_result_n4 = hxc_l_tmp_short_circuit_load_result_n8;
  if (hxc_l_tmp_short_circuit_load_result_n8)
  {
    hxc_l_tmp_short_circuit_result_n4 = hxc_l_plan.hxc_transform.hxc_xMilli >= 0;
  }
  bool hxc_l_tmp_short_circuit_load_result_n11 = hxc_l_tmp_short_circuit_result_n4;
  bool hxc_l_tmp_short_circuit_result_n5 = hxc_l_tmp_short_circuit_load_result_n11;
  if (hxc_l_tmp_short_circuit_load_result_n11)
  {
    hxc_l_tmp_short_circuit_result_n5 = hxc_l_plan.hxc_transform.hxc_xMilli < 32000;
  }
  bool hxc_l_tmp_short_circuit_load_result_n14 = hxc_l_tmp_short_circuit_result_n5;
  bool hxc_l_tmp_short_circuit_result_n6 = hxc_l_tmp_short_circuit_load_result_n14;
  if (hxc_l_tmp_short_circuit_load_result_n14)
  {
    hxc_l_tmp_short_circuit_result_n6 = hxc_l_plan.hxc_transform.hxc_yMilli >= 0;
  }
  bool hxc_l_tmp_short_circuit_load_result_n17 = hxc_l_tmp_short_circuit_result_n6;
  bool hxc_l_tmp_short_circuit_result_n7 = hxc_l_tmp_short_circuit_load_result_n17;
  if (hxc_l_tmp_short_circuit_load_result_n17)
  {
    hxc_l_tmp_short_circuit_result_n7 = hxc_l_plan.hxc_transform.hxc_yMilli < 16000;
  }
  bool hxc_l_tmp_short_circuit_load_result_n20 = hxc_l_tmp_short_circuit_result_n7;
  bool hxc_l_tmp_short_circuit_result_n8 = hxc_l_tmp_short_circuit_load_result_n20;
  if (hxc_l_tmp_short_circuit_load_result_n20)
  {
    hxc_l_tmp_short_circuit_result_n8 = hxc_l_plan.hxc_transform.hxc_zMilli >= 0;
  }
  bool hxc_l_tmp_short_circuit_load_result_n23 = hxc_l_tmp_short_circuit_result_n8;
  bool hxc_l_tmp_short_circuit_result_n9 = hxc_l_tmp_short_circuit_load_result_n23;
  if (hxc_l_tmp_short_circuit_load_result_n23)
  {
    hxc_l_tmp_short_circuit_result_n9 = hxc_l_plan.hxc_transform.hxc_zMilli < 32000;
  }
  return hxc_l_tmp_short_circuit_result_n9;
}
