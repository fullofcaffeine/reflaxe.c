#include "hxc/program.h"

void hxc_compiler_constructor_caxecraft_domain_GameSession(struct hxc_caxecraft_domain_GameSession *hxc_l_self)
{
  const void *volatile hxc_l_gc_roots[3] = { NULL, NULL, NULL };
  hxc_l_gc_roots[0] = (const void *)hxc_l_self;
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 3, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_caxecraft_domain_WaterSimulation *hxc_l_tmp_owned_class_field_address_n0 = &(*hxc_l_self).hxc_water;
  hxc_compiler_constructor_caxecraft_domain_WaterSimulation(hxc_l_tmp_owned_class_field_address_n0);
  struct hxc_array_ref *hxc_l_tmp_array_create_result_n1 = NULL;
  if (hxc_array_ref_create_trivial(hxc_default_allocator(), sizeof(int32_t), _Alignof(int32_t), &hxc_l_tmp_array_create_result_n1) != HXC_STATUS_OK)
  {
    abort();
  }
  (*hxc_l_self).hxc_triggerActorPreviousValid = hxc_l_tmp_array_create_result_n1;
  struct hxc_array_ref *hxc_l_tmp_array_create_result_n2 = NULL;
  if (hxc_array_ref_create_trivial(hxc_default_allocator(), sizeof(int32_t), _Alignof(int32_t), &hxc_l_tmp_array_create_result_n2) != HXC_STATUS_OK)
  {
    abort();
  }
  (*hxc_l_self).hxc_triggerActorPrevious = hxc_l_tmp_array_create_result_n2;
  struct hxc_array_ref *hxc_l_tmp_array_create_result_n3 = NULL;
  if (hxc_array_ref_create_trivial(hxc_default_allocator(), sizeof(int32_t), _Alignof(int32_t), &hxc_l_tmp_array_create_result_n3) != HXC_STATUS_OK)
  {
    abort();
  }
  (*hxc_l_self).hxc_triggerZoneInside = hxc_l_tmp_array_create_result_n3;
  struct hxc_array_ref *hxc_l_tmp_array_create_result_n4 = NULL;
  if (hxc_array_ref_create(hxc_default_allocator(), (hxc_array_element_ops){ sizeof(hxc_string), _Alignof(hxc_string), NULL, hxc_array_4c9e169c_element_copy, hxc_array_4c9e169c_element_assign, hxc_array_4c9e169c_element_destroy }, &hxc_l_tmp_array_create_result_n4) != HXC_STATUS_OK)
  {
    abort();
  }
  (*hxc_l_self).hxc_triggerActorIds = hxc_l_tmp_array_create_result_n4;
  struct hxc_array_ref *hxc_l_tmp_array_create_result_n5 = NULL;
  if (hxc_array_ref_create_trivial(hxc_default_allocator(), sizeof(int32_t), _Alignof(int32_t), &hxc_l_tmp_array_create_result_n5) != HXC_STATUS_OK)
  {
    abort();
  }
  (*hxc_l_self).hxc_triggerActorEntities = hxc_l_tmp_array_create_result_n5;
  struct hxc_array_ref *hxc_l_tmp_array_create_result_n6 = NULL;
  if (hxc_array_ref_create_trivial(hxc_default_allocator(), sizeof(int32_t), _Alignof(int32_t), &hxc_l_tmp_array_create_result_n6) != HXC_STATUS_OK)
  {
    abort();
  }
  (*hxc_l_self).hxc_triggerZoneBounds = hxc_l_tmp_array_create_result_n6;
  struct hxc_array_ref *hxc_l_tmp_array_create_result_n7 = NULL;
  if (hxc_array_ref_create(hxc_default_allocator(), (hxc_array_element_ops){ sizeof(hxc_string), _Alignof(hxc_string), NULL, hxc_array_4c9e169c_element_copy, hxc_array_4c9e169c_element_assign, hxc_array_4c9e169c_element_destroy }, &hxc_l_tmp_array_create_result_n7) != HXC_STATUS_OK)
  {
    abort();
  }
  (*hxc_l_self).hxc_triggerZoneIds = hxc_l_tmp_array_create_result_n7;
  hxc_string hxc_l_tmp_TField_localPlayerAuthoredId_managed_String_initializer_owner_n1 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
  if (hxc_string_retain(hxc_l_tmp_TField_localPlayerAuthoredId_managed_String_initializer_owner_n1) != HXC_STATUS_OK)
  {
    abort();
  }
  (*hxc_l_self).hxc_localPlayerAuthoredId = hxc_l_tmp_TField_localPlayerAuthoredId_managed_String_initializer_owner_n1;
  struct hxc_array_ref *hxc_l_tmp_array_create_result_n9 = NULL;
  if (hxc_array_ref_create(hxc_default_allocator(), (hxc_array_element_ops){ sizeof(struct hxc_caxecraft_scenario_FlowEventOccurrence), _Alignof(struct hxc_caxecraft_scenario_FlowEventOccurrence), NULL, hxc_array_c6ef4080_element_copy, hxc_array_c6ef4080_element_assign, hxc_array_c6ef4080_element_destroy }, &hxc_l_tmp_array_create_result_n9) != HXC_STATUS_OK)
  {
    abort();
  }
  (*hxc_l_self).hxc_pendingFlowEvents = hxc_l_tmp_array_create_result_n9;
  struct hxc_array_ref *hxc_l_tmp_array_create_result_n10 = NULL;
  if (hxc_array_ref_create_trivial(hxc_default_allocator(), sizeof(struct hxc_caxecraft_domain_DynamicCollisionBox), _Alignof(struct hxc_caxecraft_domain_DynamicCollisionBox), &hxc_l_tmp_array_create_result_n10) != HXC_STATUS_OK)
  {
    abort();
  }
  (*hxc_l_self).hxc_activeLocalPlayerCollision = hxc_l_tmp_array_create_result_n10;
  struct hxc_array_ref *hxc_l_tmp_array_create_result_n11 = NULL;
  if (hxc_array_ref_create_trivial(hxc_default_allocator(), sizeof(struct hxc_caxecraft_domain_DynamicCollisionBox), _Alignof(struct hxc_caxecraft_domain_DynamicCollisionBox), &hxc_l_tmp_array_create_result_n11) != HXC_STATUS_OK)
  {
    abort();
  }
  (*hxc_l_self).hxc_activeStatefulCollision = hxc_l_tmp_array_create_result_n11;
  struct hxc_array_ref *hxc_l_tmp_array_create_result_n12 = NULL;
  if (hxc_array_ref_create_trivial(hxc_default_allocator(), sizeof(int32_t), _Alignof(int32_t), &hxc_l_tmp_array_create_result_n12) != HXC_STATUS_OK)
  {
    abort();
  }
  (*hxc_l_self).hxc_statefulObjectCollisionSolid = hxc_l_tmp_array_create_result_n12;
  struct hxc_array_ref *hxc_l_tmp_array_create_result_n13 = NULL;
  if (hxc_array_ref_create(hxc_default_allocator(), (hxc_array_element_ops){ sizeof(hxc_string), _Alignof(hxc_string), NULL, hxc_array_42a16fde_element_copy, hxc_array_42a16fde_element_assign, hxc_array_42a16fde_element_destroy }, &hxc_l_tmp_array_create_result_n13) != HXC_STATUS_OK)
  {
    abort();
  }
  (*hxc_l_self).hxc_statefulObjectCollisionStates = hxc_l_tmp_array_create_result_n13;
  struct hxc_array_ref *hxc_l_tmp_array_create_result_n14 = NULL;
  if (hxc_array_ref_create_trivial(hxc_default_allocator(), sizeof(int32_t), _Alignof(int32_t), &hxc_l_tmp_array_create_result_n14) != HXC_STATUS_OK)
  {
    abort();
  }
  (*hxc_l_self).hxc_statefulObjectStateCounts = hxc_l_tmp_array_create_result_n14;
  struct hxc_array_ref *hxc_l_tmp_array_create_result_n15 = NULL;
  if (hxc_array_ref_create_trivial(hxc_default_allocator(), sizeof(int32_t), _Alignof(int32_t), &hxc_l_tmp_array_create_result_n15) != HXC_STATUS_OK)
  {
    abort();
  }
  (*hxc_l_self).hxc_statefulObjectStateStarts = hxc_l_tmp_array_create_result_n15;
  struct hxc_array_ref *hxc_l_tmp_array_create_result_n16 = NULL;
  if (hxc_array_ref_create_trivial(hxc_default_allocator(), sizeof(int32_t), _Alignof(int32_t), &hxc_l_tmp_array_create_result_n16) != HXC_STATUS_OK)
  {
    abort();
  }
  (*hxc_l_self).hxc_statefulObjectBoundsMilli = hxc_l_tmp_array_create_result_n16;
  struct hxc_array_ref *hxc_l_tmp_array_create_result_n17 = NULL;
  if (hxc_array_ref_create_trivial(hxc_default_allocator(), sizeof(int32_t), _Alignof(int32_t), &hxc_l_tmp_array_create_result_n17) != HXC_STATUS_OK)
  {
    abort();
  }
  (*hxc_l_self).hxc_statefulObjectRadiiMilli = hxc_l_tmp_array_create_result_n17;
  struct hxc_array_ref *hxc_l_tmp_array_create_result_n18 = NULL;
  if (hxc_array_ref_create_trivial(hxc_default_allocator(), sizeof(int32_t), _Alignof(int32_t), &hxc_l_tmp_array_create_result_n18) != HXC_STATUS_OK)
  {
    abort();
  }
  (*hxc_l_self).hxc_statefulObjectPositionsMilli = hxc_l_tmp_array_create_result_n18;
  struct hxc_array_ref *hxc_l_tmp_array_create_result_n19 = NULL;
  if (hxc_array_ref_create(hxc_default_allocator(), (hxc_array_element_ops){ sizeof(hxc_string), _Alignof(hxc_string), NULL, hxc_array_4c9e169c_element_copy, hxc_array_4c9e169c_element_assign, hxc_array_4c9e169c_element_destroy }, &hxc_l_tmp_array_create_result_n19) != HXC_STATUS_OK)
  {
    abort();
  }
  (*hxc_l_self).hxc_statefulObjectIds = hxc_l_tmp_array_create_result_n19;
  struct hxc_array_ref *hxc_l_tmp_array_create_result_n20 = NULL;
  if (hxc_array_ref_create(hxc_default_allocator(), (hxc_array_element_ops){ sizeof(hxc_string), _Alignof(hxc_string), NULL, hxc_array_42a16fde_element_copy, hxc_array_42a16fde_element_assign, hxc_array_42a16fde_element_destroy }, &hxc_l_tmp_array_create_result_n20) != HXC_STATUS_OK)
  {
    abort();
  }
  (*hxc_l_self).hxc_authoredItemContentIds = hxc_l_tmp_array_create_result_n20;
  struct hxc_array_ref *hxc_l_tmp_array_create_result_n21 = NULL;
  if (hxc_array_ref_create(hxc_default_allocator(), (hxc_array_element_ops){ sizeof(hxc_string), _Alignof(hxc_string), NULL, hxc_array_4c9e169c_element_copy, hxc_array_4c9e169c_element_assign, hxc_array_4c9e169c_element_destroy }, &hxc_l_tmp_array_create_result_n21) != HXC_STATUS_OK)
  {
    abort();
  }
  (*hxc_l_self).hxc_authoredActorIds = hxc_l_tmp_array_create_result_n21;
  struct hxc_array_ref *hxc_l_tmp_array_create_result_n22 = NULL;
  if (hxc_array_ref_create_trivial(hxc_default_allocator(), sizeof(int32_t), _Alignof(int32_t), &hxc_l_tmp_array_create_result_n22) != HXC_STATUS_OK)
  {
    abort();
  }
  (*hxc_l_self).hxc_authoredActorEntities = hxc_l_tmp_array_create_result_n22;
  struct hxc_array_ref *hxc_l_tmp_array_create_result_n23 = NULL;
  if (hxc_array_ref_create(hxc_default_allocator(), (hxc_array_element_ops){ sizeof(hxc_string), _Alignof(hxc_string), NULL, hxc_array_42a16fde_element_copy, hxc_array_42a16fde_element_assign, hxc_array_42a16fde_element_destroy }, &hxc_l_tmp_array_create_result_n23) != HXC_STATUS_OK)
  {
    abort();
  }
  (*hxc_l_self).hxc_flowBlockContentIds = hxc_l_tmp_array_create_result_n23;
  struct hxc_array_ref *hxc_l_tmp_array_create_result_n24 = NULL;
  if (hxc_array_ref_create_trivial(hxc_default_allocator(), sizeof(int32_t), _Alignof(int32_t), &hxc_l_tmp_array_create_result_n24) != HXC_STATUS_OK)
  {
    abort();
  }
  (*hxc_l_self).hxc_flowBlockStorageCodes = hxc_l_tmp_array_create_result_n24;
  hxc_l_gc_roots[1] = (const void *)NULL;
  (*hxc_l_self).hxc_flowExecutor = NULL;
  (*hxc_l_self).hxc_completedTicks = 0;
  struct hxc_array_ref *hxc_l_tmp_array_create_result_n25 = NULL;
  if (hxc_array_ref_create(hxc_default_allocator(), (hxc_array_element_ops){ sizeof(struct hxc_caxecraft_domain_ActorControllerEvent), _Alignof(struct hxc_caxecraft_domain_ActorControllerEvent), NULL, hxc_array_43e9f8ca_element_copy, hxc_array_43e9f8ca_element_assign, hxc_array_43e9f8ca_element_destroy }, &hxc_l_tmp_array_create_result_n25) != HXC_STATUS_OK)
  {
    abort();
  }
  (*hxc_l_self).hxc_actorControllerEvents = hxc_l_tmp_array_create_result_n25;
  struct hxc_array_ref *hxc_l_tmp_array_create_result_n26 = NULL;
  if (hxc_array_ref_create(hxc_default_allocator(), (hxc_array_element_ops){ sizeof(struct hxc_caxecraft_domain_ActorControllerState), _Alignof(struct hxc_caxecraft_domain_ActorControllerState), NULL, hxc_array_3767ae6c_element_copy, hxc_array_3767ae6c_element_assign, hxc_array_3767ae6c_element_destroy }, &hxc_l_tmp_array_create_result_n26) != HXC_STATUS_OK)
  {
    abort();
  }
  (*hxc_l_self).hxc_actorControllers = hxc_l_tmp_array_create_result_n26;
  struct hxc_caxecraft_domain_EntityStore *hxc_l_tmp_owned_class_field_address_n27 = &(*hxc_l_self).hxc_entities;
  hxc_compiler_constructor_caxecraft_domain_EntityStore(hxc_l_tmp_owned_class_field_address_n27);
  hxc_l_gc_roots[2] = (const void *)&hxc_l_self->hxc_base;
  hxc_compiler_constructor_caxecraft_domain_SessionStorageBase(&hxc_l_self->hxc_base);
  struct hxc_caxecraft_domain_PlayerAgent hxc_l_tmp_call_result_n28 = hxc_caxecraft_domain_PlayerAgent_bind(0);
  (*hxc_l_self).hxc_localPlayer = hxc_l_tmp_call_result_n28;
  struct hxc_caxecraft_domain_WaterSimulation *hxc_l_tmp_owned_class_field_address_n29 = &(*hxc_l_self).hxc_water;
  if (hxc_l_tmp_owned_class_field_address_n29 == NULL)
  {
    abort();
  }
  hxc_caxecraft_domain_WaterSimulation_resetPending(hxc_l_tmp_owned_class_field_address_n29);
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return;
}

struct hxc_caxecraft_scenario_FlowEventPosition hxc_caxecraft_domain_GameSession_flowEventPosition(struct hxc_caxecraft_domain_Character hxc_l_character)
{
  return (struct hxc_caxecraft_scenario_FlowEventPosition){ .hxc_xMilli = hxc_f64_to_i32_saturating(hxc_l_character.hxc_body.hxc_x * 1000.0), .hxc_yMilli = hxc_f64_to_i32_saturating(hxc_l_character.hxc_body.hxc_y * 1000.0), .hxc_zMilli = hxc_f64_to_i32_saturating(hxc_l_character.hxc_body.hxc_z * 1000.0) };
}

struct hxc_optional_record_h7aeb14c30427 hxc_caxecraft_domain_GameSession_intersectTriggerAxis(int32_t hxc_l_start, int32_t hxc_l_finish, int32_t hxc_l_minimum, int32_t hxc_l_maximum, double hxc_l_enter, double hxc_l_leave)
{
  int32_t hxc_l_delta = hxc_i32_subtract_wrapping(hxc_l_finish, hxc_l_start);
  if (!(hxc_l_delta == 0))
  {
    double hxc_l_first = hxc_f64_divide_zero_safe((double)hxc_i32_subtract_wrapping(hxc_l_minimum, hxc_l_start), (double)hxc_l_delta);
    double hxc_l_second = hxc_f64_divide_zero_safe((double)hxc_i32_subtract_wrapping(hxc_l_maximum, hxc_l_start), (double)hxc_l_delta);
    double hxc_l_tmp_load_result_n6 = hxc_l_first;
    double hxc_l_tmp_load_result_n7 = hxc_l_second;
    double hxc_l_tmp_conditional_result_n11 = 0.0;
    if (hxc_l_tmp_load_result_n6 < hxc_l_tmp_load_result_n7)
    {
      hxc_l_tmp_conditional_result_n11 = hxc_l_first;
    }
    else
    {
      hxc_l_tmp_conditional_result_n11 = hxc_l_second;
    }
    double hxc_l_axisEnter = hxc_l_tmp_conditional_result_n11;
    double hxc_l_tmp_load_result_n11 = hxc_l_first;
    double hxc_l_tmp_load_result_n12 = hxc_l_second;
    double hxc_l_tmp_conditional_result_n13 = 0.0;
    if (hxc_l_tmp_load_result_n11 < hxc_l_tmp_load_result_n12)
    {
      hxc_l_tmp_conditional_result_n13 = hxc_l_second;
    }
    else
    {
      hxc_l_tmp_conditional_result_n13 = hxc_l_first;
    }
    double hxc_l_axisLeave = hxc_l_tmp_conditional_result_n13;
    double hxc_l_tmp_load_result_n16 = hxc_l_axisEnter;
    double hxc_l_tmp_conditional_result_n15 = 0.0;
    if (hxc_l_tmp_load_result_n16 > hxc_l_enter)
    {
      hxc_l_tmp_conditional_result_n15 = hxc_l_axisEnter;
    }
    else
    {
      hxc_l_tmp_conditional_result_n15 = hxc_l_enter;
    }
    double hxc_l_narrowedEnter = hxc_l_tmp_conditional_result_n15;
    double hxc_l_tmp_load_result_n19 = hxc_l_axisLeave;
    double hxc_l_tmp_conditional_result_n17 = 0.0;
    if (hxc_l_tmp_load_result_n19 < hxc_l_leave)
    {
      hxc_l_tmp_conditional_result_n17 = hxc_l_axisLeave;
    }
    else
    {
      hxc_l_tmp_conditional_result_n17 = hxc_l_leave;
    }
    double hxc_l_narrowedLeave = hxc_l_tmp_conditional_result_n17;
    double hxc_l_tmp_load_result_n22 = hxc_l_narrowedEnter;
    if (!(hxc_l_tmp_load_result_n22 < hxc_l_narrowedLeave))
    {
      return (struct hxc_optional_record_h7aeb14c30427){ .hxc_has_value = false };
    }
    double hxc_l_tmp_load_result_n24 = hxc_l_narrowedEnter;
    return (struct hxc_optional_record_h7aeb14c30427){ .hxc_has_value = true, .hxc_value = (struct hxc_anonymous_record_41f65f0a1c59){ .hxc_enter = hxc_l_tmp_load_result_n24, .hxc_leave = hxc_l_narrowedLeave } };
  }
  bool hxc_l_tmp_short_circuit_result_n7 = hxc_l_start < hxc_l_minimum;
  if (!(hxc_l_start < hxc_l_minimum))
  {
    hxc_l_tmp_short_circuit_result_n7 = hxc_l_start >= hxc_l_maximum;
  }
  if (!hxc_l_tmp_short_circuit_result_n7)
  {
    return (struct hxc_optional_record_h7aeb14c30427){ .hxc_has_value = true, .hxc_value = (struct hxc_anonymous_record_41f65f0a1c59){ .hxc_enter = hxc_l_enter, .hxc_leave = hxc_l_leave } };
  }
  return (struct hxc_optional_record_h7aeb14c30427){ .hxc_has_value = false };
}

void hxc_caxecraft_domain_GameSession_writeTriggerActorPosition(struct hxc_array_ref *hxc_l_storage, int32_t hxc_l_actorIndex, struct hxc_caxecraft_scenario_FlowEventPosition hxc_l_value)
{
  int32_t hxc_l_offset = hxc_i32_multiply_wrapping(hxc_l_actorIndex, 3);
  if (hxc_array_ref_set_grow_copy(hxc_l_storage, hxc_l_offset, &(int32_t){ hxc_l_value.hxc_xMilli }, &(int32_t){ 0 }) != HXC_STATUS_OK)
  {
    abort();
  }
  (void)hxc_l_value.hxc_xMilli;
  if (hxc_array_ref_set_grow_copy(hxc_l_storage, hxc_i32_add_wrapping(hxc_l_offset, 1), &(int32_t){ hxc_l_value.hxc_yMilli }, &(int32_t){ 0 }) != HXC_STATUS_OK)
  {
    abort();
  }
  (void)hxc_l_value.hxc_yMilli;
  if (hxc_array_ref_set_grow_copy(hxc_l_storage, hxc_i32_add_wrapping(hxc_l_offset, 2), &(int32_t){ hxc_l_value.hxc_zMilli }, &(int32_t){ 0 }) != HXC_STATUS_OK)
  {
    abort();
  }
  (void)hxc_l_value.hxc_zMilli;
  return;
}

bool hxc_caxecraft_domain_GameSession_activateAuthoredItemDuringLoad(struct hxc_caxecraft_domain_GameSession *hxc_l_self, int32_t hxc_l_index)
{
  const void *volatile hxc_l_gc_roots[1] = { NULL };
  hxc_l_gc_roots[0] = (const void *)hxc_l_self;
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
    (*hxc_l_self).hxc_base.hxc_authoredItemStorage[(size_t)hxc_l_index] = 1;
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
  const void *volatile hxc_l_gc_roots[1] = { NULL };
  hxc_l_gc_roots[0] = (const void *)hxc_l_self;
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
  const void *volatile hxc_l_gc_roots[1] = { NULL };
  hxc_l_gc_roots[0] = (const void *)hxc_l_self;
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
  const void *volatile hxc_l_gc_roots[1] = { NULL };
  hxc_l_gc_roots[0] = (const void *)hxc_l_self;
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
  const void *volatile hxc_l_gc_roots[1] = { NULL };
  hxc_l_gc_roots[0] = (const void *)hxc_l_self;
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

hxc_string hxc_caxecraft_domain_GameSession_authoredEnemyId(struct hxc_caxecraft_domain_GameSession *hxc_l_self, int32_t hxc_l_id)
{
  const void *volatile hxc_l_gc_roots[3] = { NULL, NULL, NULL };
  hxc_l_gc_roots[0] = (const void *)hxc_l_self;
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 3, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_string hxc_l_tmp_array_element_owner_n10 = { 0 };
  hxc_string hxc_l_tmp_returned_string_owner_n2 = { 0 };
  struct hxc_caxecraft_domain_ActorControllerState hxc_l_tmp_array_element_owner_n6 = { 0 };
  struct hxc_caxecraft_domain_ActorControllerState hxc_l_tmp_array_element_owner_n7 = { 0 };
  hxc_string hxc_l_tmp_array_element_owner_n9 = { 0 };
  if (hxc_l_self == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_scenario_CaxeFlowExecutor *hxc_l_tmp_class_field_load_result_n0 = (*hxc_l_self).hxc_flowExecutor;
  hxc_l_gc_roots[1] = (const void *)hxc_l_tmp_class_field_load_result_n0;
  hxc_l_gc_roots[2] = (const void *)NULL;
  if (hxc_l_tmp_class_field_load_result_n0 == NULL)
  {
    hxc_l_tmp_returned_string_owner_n2 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_retain(hxc_l_tmp_returned_string_owner_n2) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return hxc_l_tmp_returned_string_owner_n2;
  }
  int32_t hxc_l_g = 0;
  struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n2 = (*hxc_l_self).hxc_actorControllers;
  int32_t hxc_l_tmp_array_length_result_n3;
  if (hxc_array_ref_length(hxc_l_tmp_class_field_load_result_n2, &hxc_l_tmp_array_length_result_n3) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_g1 = hxc_l_tmp_array_length_result_n3;
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n4 = hxc_l_g;
    if (!(hxc_l_tmp_load_result_n4 < hxc_l_g1))
    {
      break;
    }
    int32_t hxc_l_tmp_increment_load_result_n6 = hxc_l_g;
    hxc_l_g = hxc_i32_add_wrapping(hxc_l_tmp_increment_load_result_n6, 1);
    int32_t hxc_l_index = hxc_l_tmp_increment_load_result_n6;
    struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n7 = (*hxc_l_self).hxc_actorControllers;
    struct hxc_caxecraft_domain_ActorControllerState hxc_l_tmp_array_get_result_n9;
    if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n7, (size_t)hxc_l_index, &hxc_l_tmp_array_get_result_n9) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_array_element_owner_n6 = hxc_l_tmp_array_get_result_n9;
    struct hxc_caxecraft_domain_ActorControllerState hxc_l_tmp_array_element_borrow_result_n10 = hxc_l_tmp_array_element_owner_n6;
    hxc_array_3767ae6c_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n6);
    if (!(hxc_l_tmp_array_element_borrow_result_n10.hxc_characterId == hxc_l_id))
    {
      continue;
    }
    struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n12 = (*hxc_l_self).hxc_actorControllers;
    struct hxc_caxecraft_domain_ActorControllerState hxc_l_tmp_array_get_result_n14;
    if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n12, (size_t)hxc_l_index, &hxc_l_tmp_array_get_result_n14) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_array_element_owner_n7 = hxc_l_tmp_array_get_result_n14;
    switch (hxc_l_tmp_array_element_owner_n7.hxc_profile.hxc_tag) {
      case hxc_caxecraft_domain_ActorControllerProfile_StationaryDialogue:
        {
          break;
        }
      case hxc_caxecraft_domain_ActorControllerProfile_WanderChaseMelee:
        {
          struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n18 = (*hxc_l_self).hxc_authoredActorIds;
          hxc_string hxc_l_tmp_array_get_result_n20;
          if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n18, (size_t)hxc_l_index, &hxc_l_tmp_array_get_result_n20) != HXC_STATUS_OK)
          {
            abort();
          }
          hxc_l_tmp_array_element_owner_n9 = hxc_l_tmp_array_get_result_n20;
          hxc_string hxc_l_tmp_array_element_borrow_result_n21 = hxc_l_tmp_array_element_owner_n9;
          hxc_array_3767ae6c_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n7);
          if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
          {
            abort();
          }
          return hxc_l_tmp_array_element_borrow_result_n21;
        }
      case hxc_caxecraft_domain_ActorControllerProfile_TelegraphedCharge:
        {
          struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n22 = (*hxc_l_self).hxc_authoredActorIds;
          hxc_string hxc_l_tmp_array_get_result_n24;
          if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n22, (size_t)hxc_l_index, &hxc_l_tmp_array_get_result_n24) != HXC_STATUS_OK)
          {
            abort();
          }
          hxc_l_tmp_array_element_owner_n10 = hxc_l_tmp_array_get_result_n24;
          hxc_string hxc_l_tmp_array_element_borrow_result_n25 = hxc_l_tmp_array_element_owner_n10;
          hxc_array_3767ae6c_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n7);
          if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
          {
            abort();
          }
          return hxc_l_tmp_array_element_borrow_result_n25;
        }
      default:
        {
          abort();
        }
    }
    hxc_string hxc_l_tmp_returned_string_owner_n8 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_retain(hxc_l_tmp_returned_string_owner_n8) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_string hxc_l_tmp_returned_string_owned_load_result_n17 = hxc_l_tmp_returned_string_owner_n8;
    hxc_array_3767ae6c_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n7);
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return hxc_l_tmp_returned_string_owned_load_result_n17;
  }
  hxc_string hxc_l_tmp_returned_string_owner_n11 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
  if (hxc_string_retain(hxc_l_tmp_returned_string_owner_n11) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return hxc_l_tmp_returned_string_owner_n11;
}

bool hxc_caxecraft_domain_GameSession_authoredItemCollectionEventAvailable(struct hxc_caxecraft_domain_GameSession *hxc_l_self, int32_t hxc_l_index)
{
  const void *volatile hxc_l_gc_roots[3] = { NULL, NULL, NULL };
  hxc_l_gc_roots[0] = (const void *)hxc_l_self;
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 3, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_optional_caxecraft_scenario_FlowEventOccurrence hxc_l_occurrence = { 0 };
  hxc_string hxc_l_tmp_array_element_owner_n4 = { 0 };
  struct hxc_caxecraft_scenario_FlowEvent hxc_l_tmp_instance_call_argument_0_owner_n6 = { 0 };
  struct hxc_array_ref *hxc_l_tmp_instance_call_argument_0_owner_n8 = { 0 };
  if (hxc_l_self == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_scenario_CaxeFlowExecutor *hxc_l_tmp_class_field_load_result_n0 = (*hxc_l_self).hxc_flowExecutor;
  hxc_l_gc_roots[1] = (const void *)hxc_l_tmp_class_field_load_result_n0;
  hxc_l_gc_roots[2] = (const void *)NULL;
  if (hxc_l_tmp_class_field_load_result_n0 == NULL)
  {
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return true;
  }
  bool hxc_l_tmp_short_circuit_result_n2 = hxc_l_index < 0;
  if (!(hxc_l_index < 0))
  {
    struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n1 = (*hxc_l_self).hxc_authoredItemContentIds;
    int32_t hxc_l_tmp_array_length_result_n2;
    if (hxc_array_ref_length(hxc_l_tmp_class_field_load_result_n1, &hxc_l_tmp_array_length_result_n2) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_short_circuit_result_n2 = hxc_l_index >= hxc_l_tmp_array_length_result_n2;
  }
  if (hxc_l_tmp_short_circuit_result_n2)
  {
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return false;
  }
  struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n4 = (*hxc_l_self).hxc_authoredItemContentIds;
  hxc_string hxc_l_tmp_array_get_result_n5;
  if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n4, (size_t)hxc_l_index, &hxc_l_tmp_array_get_result_n5) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_tmp_array_element_owner_n4 = hxc_l_tmp_array_get_result_n5;
  hxc_string hxc_l_tmp_enum_payload_0_owner_n5 = hxc_l_tmp_array_element_owner_n4;
  if (hxc_string_retain(hxc_l_tmp_enum_payload_0_owner_n5) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_tmp_instance_call_argument_0_owner_n6 = (struct hxc_caxecraft_scenario_FlowEvent){ .hxc_tag = hxc_caxecraft_scenario_FlowEvent_ItemCollected, .hxc_payload.hxc_ItemCollected.hxc_itemType = hxc_l_tmp_enum_payload_0_owner_n5 };
  struct hxc_optional_caxecraft_scenario_FlowEventOccurrence hxc_l_tmp_instance_call_result_n10 = hxc_caxecraft_domain_GameSession_localActorOccurrence(hxc_l_self, hxc_l_tmp_instance_call_argument_0_owner_n6);
  hxc_l_occurrence = hxc_l_tmp_instance_call_result_n10;
  struct hxc_optional_caxecraft_scenario_FlowEventOccurrence hxc_l_tmp_load_result_n11 = hxc_l_occurrence;
  bool hxc_l_tmp_short_circuit_result_n7 = hxc_l_tmp_load_result_n11.hxc_has_value;
  if (hxc_l_tmp_load_result_n11.hxc_has_value)
  {
    struct hxc_optional_caxecraft_scenario_FlowEventOccurrence hxc_l_tmp_load_result_n12 = hxc_l_occurrence;
    if (!hxc_l_tmp_load_result_n12.hxc_has_value)
    {
      abort();
    }
    struct hxc_array_ref *hxc_l_tmp_array_create_result_n14 = NULL;
    if (hxc_array_ref_create(hxc_default_allocator(), (hxc_array_element_ops){ sizeof(struct hxc_caxecraft_scenario_FlowEventOccurrence), _Alignof(struct hxc_caxecraft_scenario_FlowEventOccurrence), NULL, hxc_array_c6ef4080_element_copy, hxc_array_c6ef4080_element_assign, hxc_array_c6ef4080_element_destroy }, &hxc_l_tmp_array_create_result_n14) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n14->value, &hxc_l_tmp_load_result_n12.hxc_value) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_instance_call_argument_0_owner_n8 = hxc_l_tmp_array_create_result_n14;
    bool hxc_l_tmp_instance_call_result_n16 = hxc_caxecraft_domain_GameSession_flowEventsCanCommit(hxc_l_self, hxc_l_tmp_instance_call_argument_0_owner_n8);
    hxc_l_tmp_short_circuit_result_n7 = hxc_l_tmp_instance_call_result_n16;
    if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_0_owner_n8) != HXC_STATUS_OK)
    {
      abort();
    }
  }
  bool hxc_l_tmp_short_circuit_load_result_n17 = hxc_l_tmp_short_circuit_result_n7;
  hxc_optional_1cc4d348_destroy(&hxc_l_occurrence);
  hxc_enum_10047054_destroy(&hxc_l_tmp_instance_call_argument_0_owner_n6);
  if (hxc_string_release(&hxc_l_tmp_array_element_owner_n4) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return hxc_l_tmp_short_circuit_load_result_n17;
}

bool hxc_caxecraft_domain_GameSession_authoredItemIsActive(struct hxc_caxecraft_domain_GameSession *hxc_l_self, int32_t hxc_l_index)
{
  const void *volatile hxc_l_gc_roots[1] = { NULL };
  hxc_l_gc_roots[0] = (const void *)hxc_l_self;
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
    int32_t hxc_l_tmp_collection_index_load_result_n1 = (*hxc_l_self).hxc_base.hxc_authoredItemStorage[(size_t)hxc_l_index];
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
  const void *volatile hxc_l_gc_roots[1] = { NULL };
  hxc_l_gc_roots[0] = (const void *)hxc_l_self;
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
  return (*hxc_l_self).hxc_base.hxc_authoredItemStorage;
}

bool hxc_caxecraft_domain_GameSession_bindLocalPlayer(struct hxc_caxecraft_domain_GameSession *hxc_l_self, struct hxc_caxecraft_domain_Character hxc_l_character)
{
  const void *volatile hxc_l_gc_roots[1] = { NULL };
  hxc_l_gc_roots[0] = (const void *)hxc_l_self;
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

struct hxc_array_ref *hxc_caxecraft_domain_GameSession_blockChangedOccurrences(struct hxc_caxecraft_domain_GameSession *hxc_l_self, struct hxc_caxecraft_scenario_VoxelPoint hxc_l_coord, enum hxc_caxecraft_domain_BlockKind hxc_l_kind)
{
  const void *volatile hxc_l_gc_roots[3] = { NULL, NULL, NULL };
  hxc_l_gc_roots[0] = (const void *)hxc_l_self;
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 3, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_string hxc_l_tmp_array_element_owner_n11 = { 0 };
  struct hxc_caxecraft_scenario_FlowEvent hxc_l_tmp_static_call_argument_0_owner_n14 = { 0 };
  struct hxc_caxecraft_scenario_FlowEventContext hxc_l_tmp_static_call_argument_1_owner_n16 = { 0 };
  struct hxc_caxecraft_scenario_FlowEventOccurrence hxc_l_tmp_array_push_element_owner_n17 = { 0 };
  hxc_string hxc_l_actor = { 0 };
  hxc_string hxc_l_blockType = { 0 };
  struct hxc_array_ref *hxc_l_events = { 0 };
  if (hxc_l_self == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_scenario_CaxeFlowExecutor *hxc_l_tmp_class_field_load_result_n0 = (*hxc_l_self).hxc_flowExecutor;
  hxc_l_gc_roots[1] = (const void *)hxc_l_tmp_class_field_load_result_n0;
  hxc_l_gc_roots[2] = (const void *)NULL;
  if (hxc_l_tmp_class_field_load_result_n0 == NULL)
  {
    struct hxc_array_ref *hxc_l_tmp_array_create_result_n1 = NULL;
    if (hxc_array_ref_create(hxc_default_allocator(), (hxc_array_element_ops){ sizeof(struct hxc_caxecraft_scenario_FlowEventOccurrence), _Alignof(struct hxc_caxecraft_scenario_FlowEventOccurrence), NULL, hxc_array_c6ef4080_element_copy, hxc_array_c6ef4080_element_assign, hxc_array_c6ef4080_element_destroy }, &hxc_l_tmp_array_create_result_n1) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return hxc_l_tmp_array_create_result_n1;
  }
  hxc_string hxc_l_tmp_class_field_load_result_n2 = (*hxc_l_self).hxc_localPlayerAuthoredId;
  hxc_l_actor = hxc_l_tmp_class_field_load_result_n2;
  if (hxc_string_retain(hxc_l_actor) != HXC_STATUS_OK)
  {
    abort();
  }
  bool hxc_l_tmp_string_null_equality_result_n4 = hxc_l_actor.data == NULL;
  if (hxc_l_tmp_string_null_equality_result_n4)
  {
    if (hxc_string_release(&hxc_l_actor) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return NULL;
  }
  int32_t hxc_l_tmp_call_result_n5 = hxc_caxecraft_domain_World_kindCode(hxc_l_kind);
  hxc_string hxc_l_tmp_instance_call_result_n6 = hxc_caxecraft_domain_GameSession_flowBlockContentId(hxc_l_self, hxc_l_tmp_call_result_n5);
  hxc_l_blockType = hxc_l_tmp_instance_call_result_n6;
  bool hxc_l_tmp_string_null_equality_result_n6 = hxc_l_blockType.data == NULL;
  if (hxc_l_tmp_string_null_equality_result_n6)
  {
    if (hxc_string_release(&hxc_l_blockType) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_actor) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return NULL;
  }
  struct hxc_array_ref *hxc_l_tmp_array_create_result_n9 = NULL;
  if (hxc_array_ref_create(hxc_default_allocator(), (hxc_array_element_ops){ sizeof(struct hxc_caxecraft_scenario_FlowEventOccurrence), _Alignof(struct hxc_caxecraft_scenario_FlowEventOccurrence), NULL, hxc_array_c6ef4080_element_copy, hxc_array_c6ef4080_element_assign, hxc_array_c6ef4080_element_destroy }, &hxc_l_tmp_array_create_result_n9) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_events = hxc_l_tmp_array_create_result_n9;
  int32_t hxc_l_g = 0;
  struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n10 = (*hxc_l_self).hxc_triggerZoneIds;
  int32_t hxc_l_tmp_array_length_result_n11;
  if (hxc_array_ref_length(hxc_l_tmp_class_field_load_result_n10, &hxc_l_tmp_array_length_result_n11) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_g1 = hxc_l_tmp_array_length_result_n11;
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n12 = hxc_l_g;
    if (!(hxc_l_tmp_load_result_n12 < hxc_l_g1))
    {
      break;
    }
    int32_t hxc_l_tmp_increment_load_result_n14 = hxc_l_g;
    hxc_l_g = hxc_i32_add_wrapping(hxc_l_tmp_increment_load_result_n14, 1);
    int32_t hxc_l_zoneIndex = hxc_l_tmp_increment_load_result_n14;
    bool hxc_l_tmp_instance_call_result_n16 = hxc_caxecraft_domain_GameSession_triggerZoneContainsBlock(hxc_l_self, hxc_l_zoneIndex, hxc_l_coord);
    if (hxc_l_tmp_instance_call_result_n16)
    {
      struct hxc_array_ref *hxc_l_tmp_load_result_n17 = hxc_l_events;
      struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n18 = (*hxc_l_self).hxc_triggerZoneIds;
      hxc_string hxc_l_tmp_array_get_result_n20;
      if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n18, (size_t)hxc_l_zoneIndex, &hxc_l_tmp_array_get_result_n20) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_l_tmp_array_element_owner_n11 = hxc_l_tmp_array_get_result_n20;
      hxc_string hxc_l_tmp_enum_payload_0_owner_n12 = hxc_l_tmp_array_element_owner_n11;
      if (hxc_string_retain(hxc_l_tmp_enum_payload_0_owner_n12) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_string hxc_l_tmp_enum_payload_0_owned_load_result_n22 = hxc_l_tmp_enum_payload_0_owner_n12;
      hxc_string hxc_l_tmp_enum_payload_1_owner_n13 = hxc_l_blockType;
      if (hxc_string_retain(hxc_l_tmp_enum_payload_1_owner_n13) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_l_tmp_static_call_argument_0_owner_n14 = (struct hxc_caxecraft_scenario_FlowEvent){ .hxc_tag = hxc_caxecraft_scenario_FlowEvent_BlockChanged, .hxc_payload.hxc_BlockChanged.hxc_zone = hxc_l_tmp_enum_payload_0_owned_load_result_n22, .hxc_payload.hxc_BlockChanged.hxc_blockType = hxc_l_tmp_enum_payload_1_owner_n13 };
      struct hxc_caxecraft_scenario_FlowEvent hxc_l_tmp_static_call_argument_0_borrow_result_n26 = hxc_l_tmp_static_call_argument_0_owner_n14;
      hxc_string hxc_l_tmp_enum_payload_0_owner_n15 = hxc_l_actor;
      if (hxc_string_retain(hxc_l_tmp_enum_payload_0_owner_n15) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_l_tmp_static_call_argument_1_owner_n16 = (struct hxc_caxecraft_scenario_FlowEventContext){ .hxc_tag = hxc_caxecraft_scenario_FlowEventContext_ActorEventContext, .hxc_payload.hxc_ActorEventContext.hxc_actor = hxc_l_tmp_enum_payload_0_owner_n15 };
      struct hxc_caxecraft_scenario_FlowEventOccurrence hxc_l_tmp_call_result_n31 = hxc_caxecraft_scenario_CaxeFlowEventRegistry_flowEventOccurrence(hxc_l_tmp_static_call_argument_0_borrow_result_n26, hxc_l_tmp_static_call_argument_1_owner_n16);
      hxc_l_tmp_array_push_element_owner_n17 = hxc_l_tmp_call_result_n31;
      int32_t hxc_l_tmp_array_push_result_n33;
      if (hxc_array_ref_push_copy(hxc_l_tmp_load_result_n17, &hxc_l_tmp_array_push_element_owner_n17, &hxc_l_tmp_array_push_result_n33) != HXC_STATUS_OK)
      {
        abort();
      }
      (void)hxc_l_tmp_array_push_result_n33;
      hxc_record_7a90fbbd_destroy(&hxc_l_tmp_array_push_element_owner_n17);
      hxc_enum_276ca5e9_destroy(&hxc_l_tmp_static_call_argument_1_owner_n16);
      hxc_enum_10047054_destroy(&hxc_l_tmp_static_call_argument_0_owner_n14);
      if (hxc_string_release(&hxc_l_tmp_array_element_owner_n11) != HXC_STATUS_OK)
      {
        abort();
      }
    }
  }
  struct hxc_array_ref *hxc_l_tmp_load_result_n34 = hxc_l_events;
  if (hxc_string_release(&hxc_l_blockType) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_string_release(&hxc_l_actor) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return hxc_l_tmp_load_result_n34;
}

bool hxc_caxecraft_domain_GameSession_characterAcceptsMeleeDamage(struct hxc_caxecraft_domain_GameSession *hxc_l_self, int32_t hxc_l_id)
{
  const void *volatile hxc_l_gc_roots[1] = { NULL };
  hxc_l_gc_roots[0] = (const void *)hxc_l_self;
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 1, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_caxecraft_domain_ActorControllerState hxc_l_state = { 0 };
  struct hxc_caxecraft_domain_ActorControllerState hxc_l_tmp_array_element_owner_n6 = { 0 };
  if (hxc_l_self == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n0 = hxc_caxecraft_domain_GameSession_readCharacter(hxc_l_self, hxc_l_id);
  struct hxc_caxecraft_domain_Character hxc_l_character = hxc_l_tmp_instance_call_result_n0;
  int32_t hxc_l_g = 0;
  struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n1 = (*hxc_l_self).hxc_actorControllers;
  struct hxc_array_ref *hxc_l_g1 = hxc_l_tmp_class_field_load_result_n1;
  if (hxc_array_ref_retain(hxc_l_g1) != HXC_STATUS_OK)
  {
    abort();
  }
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n2 = hxc_l_g;
    int32_t hxc_l_tmp_array_length_result_n4;
    if (hxc_array_ref_length(hxc_l_g1, &hxc_l_tmp_array_length_result_n4) != HXC_STATUS_OK)
    {
      abort();
    }
    if (!(hxc_l_tmp_load_result_n2 < hxc_l_tmp_array_length_result_n4))
    {
      break;
    }
    struct hxc_array_ref *hxc_l_tmp_load_result_n5 = hxc_l_g1;
    struct hxc_caxecraft_domain_ActorControllerState hxc_l_tmp_array_get_result_n7;
    if (hxc_array_ref_get_copy(hxc_l_tmp_load_result_n5, (size_t)hxc_l_g, &hxc_l_tmp_array_get_result_n7) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_array_element_owner_n6 = hxc_l_tmp_array_get_result_n7;
    hxc_l_state = hxc_l_tmp_array_element_owner_n6;
    if (hxc_record_50dff1c7_retain(&hxc_l_state) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_g = hxc_i32_add_wrapping(hxc_l_g, 1);
    if (hxc_l_state.hxc_characterId == hxc_l_id)
    {
      struct hxc_caxecraft_domain_ActorControllerState hxc_l_tmp_load_result_n11 = hxc_l_state;
      bool hxc_l_tmp_call_result_n13 = hxc_caxecraft_domain_ActorControllerScheduler_acceptsMeleeDamage(hxc_l_tmp_load_result_n11, hxc_l_character);
      hxc_record_50dff1c7_destroy(&hxc_l_state);
      hxc_array_3767ae6c_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n6);
      if (hxc_array_ref_release(hxc_l_g1) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
      {
        abort();
      }
      return hxc_l_tmp_call_result_n13;
    }
    hxc_record_50dff1c7_destroy(&hxc_l_state);
    hxc_array_3767ae6c_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n6);
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

void hxc_caxecraft_domain_GameSession_clearActorControllerEvents(struct hxc_caxecraft_domain_GameSession *hxc_l_self)
{
  const void *volatile hxc_l_gc_roots[1] = { NULL };
  hxc_l_gc_roots[0] = (const void *)hxc_l_self;
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
  const void *volatile hxc_l_gc_roots[1] = { NULL };
  hxc_l_gc_roots[0] = (const void *)hxc_l_self;
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
  bool hxc_l_tmp_instance_call_result_n10 = hxc_caxecraft_domain_GameSession_authoredItemCollectionEventAvailable(hxc_l_self, hxc_l_index);
  if (!hxc_l_tmp_instance_call_result_n10)
  {
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return (struct hxc_caxecraft_domain_AuthoredAquaticEquipmentResult){ .hxc_character = hxc_l_original, .hxc_collected = false, .hxc_resolved = false };
  }
  struct hxc_caxecraft_domain_Character hxc_l_tmp_load_result_n13 = hxc_l_original;
  struct hxc_caxecraft_domain_Character hxc_l_tmp_call_result_n15 = hxc_caxecraft_domain_Character_adoptProfile(hxc_l_original, hxc_l_replacement);
  struct hxc_caxecraft_domain_LocalCharacterCommandResult hxc_l_tmp_instance_call_result_n16 = hxc_caxecraft_domain_GameSession_commitLocalCharacter(hxc_l_self, hxc_l_tmp_load_result_n13, hxc_l_tmp_call_result_n15);
  struct hxc_caxecraft_domain_LocalCharacterCommandResult hxc_l_committed = hxc_l_tmp_instance_call_result_n16;
  if (hxc_l_committed.hxc_resolved)
  {
    if (hxc_l_index < 0 || (size_t)hxc_l_index >= 256)
    {
      abort();
    }
    (*hxc_l_self).hxc_base.hxc_authoredItemStorage[(size_t)hxc_l_index] = 0;
    hxc_caxecraft_domain_GameSession_queueAuthoredItemCollected(hxc_l_self, hxc_l_index);
  }
  struct hxc_caxecraft_domain_Character hxc_l_tmp_record_field_load_result_n18 = hxc_l_committed.hxc_character;
  bool hxc_l_tmp_record_field_load_result_n19 = hxc_l_committed.hxc_resolved;
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return (struct hxc_caxecraft_domain_AuthoredAquaticEquipmentResult){ .hxc_character = hxc_l_tmp_record_field_load_result_n18, .hxc_collected = hxc_l_tmp_record_field_load_result_n19, .hxc_resolved = hxc_l_committed.hxc_resolved };
}

struct hxc_caxecraft_domain_AuthoredInventoryItemResult hxc_caxecraft_domain_GameSession_collectAuthoredInventoryItem(struct hxc_caxecraft_domain_GameSession *hxc_l_self, int32_t hxc_l_index, struct hxc_caxecraft_gameplay_InventoryState hxc_l_inventory, enum hxc_caxecraft_gameplay_ItemKind hxc_l_kind, int32_t hxc_l_quantity)
{
  const void *volatile hxc_l_gc_roots[1] = { NULL };
  hxc_l_gc_roots[0] = (const void *)hxc_l_self;
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
  if (hxc_l_tmp_short_circuit_result_n8)
  {
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return (struct hxc_caxecraft_domain_AuthoredInventoryItemResult){ .hxc_collected = 0, .hxc_inventory = hxc_l_inventory, .hxc_resolved = true };
  }
  bool hxc_l_tmp_instance_call_result_n8 = hxc_caxecraft_domain_GameSession_authoredItemCollectionEventAvailable(hxc_l_self, hxc_l_index);
  if (!!hxc_l_tmp_instance_call_result_n8)
  {
    struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n10 = hxc_caxecraft_gameplay_Inventory_collectItem(hxc_l_inventory, hxc_l_kind, hxc_l_quantity);
    hxc_l_replacement = hxc_l_tmp_call_result_n10;
    if (hxc_l_index < 0 || (size_t)hxc_l_index >= 256)
    {
      abort();
    }
    (*hxc_l_self).hxc_base.hxc_authoredItemStorage[(size_t)hxc_l_index] = 0;
    hxc_caxecraft_domain_GameSession_queueAuthoredItemCollected(hxc_l_self, hxc_l_index);
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
  return (struct hxc_caxecraft_domain_AuthoredInventoryItemResult){ .hxc_collected = 0, .hxc_inventory = hxc_l_inventory, .hxc_resolved = false };
}

void hxc_caxecraft_domain_GameSession_commitFlowEvents(struct hxc_caxecraft_domain_GameSession *hxc_l_self, struct hxc_array_ref *hxc_l_events)
{
  const void *volatile hxc_l_gc_roots[1] = { NULL };
  hxc_l_gc_roots[0] = (const void *)hxc_l_self;
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 1, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_caxecraft_scenario_FlowEventOccurrence hxc_l_event = { 0 };
  struct hxc_caxecraft_scenario_FlowEventOccurrence hxc_l_tmp_array_element_owner_n4 = { 0 };
  int32_t hxc_l_g = 0;
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n0 = hxc_l_g;
    int32_t hxc_l_tmp_array_length_result_n1;
    if (hxc_array_ref_length(hxc_l_events, &hxc_l_tmp_array_length_result_n1) != HXC_STATUS_OK)
    {
      abort();
    }
    if (!(hxc_l_tmp_load_result_n0 < hxc_l_tmp_array_length_result_n1))
    {
      break;
    }
    struct hxc_caxecraft_scenario_FlowEventOccurrence hxc_l_tmp_array_get_result_n3;
    if (hxc_array_ref_get_copy(hxc_l_events, (size_t)hxc_l_g, &hxc_l_tmp_array_get_result_n3) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_array_element_owner_n4 = hxc_l_tmp_array_get_result_n3;
    hxc_l_event = hxc_l_tmp_array_element_owner_n4;
    if (hxc_record_7a90fbbd_retain(&hxc_l_event) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_g = hxc_i32_add_wrapping(hxc_l_g, 1);
    if (hxc_l_self == NULL)
    {
      abort();
    }
    struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n6 = (*hxc_l_self).hxc_pendingFlowEvents;
    int32_t hxc_l_tmp_array_push_result_n8;
    if (hxc_array_ref_push_copy(hxc_l_tmp_class_field_load_result_n6, &hxc_l_event, &hxc_l_tmp_array_push_result_n8) != HXC_STATUS_OK)
    {
      abort();
    }
    (void)hxc_l_tmp_array_push_result_n8;
    hxc_record_7a90fbbd_destroy(&hxc_l_event);
    hxc_array_c6ef4080_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n4);
  }
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return;
}

struct hxc_caxecraft_domain_LocalCharacterCommandResult hxc_caxecraft_domain_GameSession_commitLocalCharacter(struct hxc_caxecraft_domain_GameSession *hxc_l_self, struct hxc_caxecraft_domain_Character hxc_l_original, struct hxc_caxecraft_domain_Character hxc_l_replacement)
{
  const void *volatile hxc_l_gc_roots[1] = { NULL };
  hxc_l_gc_roots[0] = (const void *)hxc_l_self;
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

struct hxc_array_ref *hxc_caxecraft_domain_GameSession_currentFlowPositions(struct hxc_caxecraft_domain_GameSession *hxc_l_self, struct hxc_caxecraft_scenario_CaxeFlowExecutor *hxc_l_executor)
{
  const void *volatile hxc_l_gc_roots[3] = { NULL, NULL, NULL };
  hxc_l_gc_roots[0] = (const void *)hxc_l_self;
  hxc_l_gc_roots[1] = (const void *)hxc_l_executor;
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 3, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_string hxc_l_tmp_array_element_owner_n11 = { 0 };
  struct hxc_caxecraft_scenario_FlowPosition hxc_l_tmp_array_push_element_owner_n13 = { 0 };
  hxc_string hxc_l_tmp_array_element_owner_n9 = { 0 };
  struct hxc_array_ref *hxc_l_tmp_array_create_result_n0 = NULL;
  if (hxc_array_ref_create(hxc_default_allocator(), (hxc_array_element_ops){ sizeof(struct hxc_caxecraft_scenario_FlowPosition), _Alignof(struct hxc_caxecraft_scenario_FlowPosition), NULL, hxc_array_456c7a78_element_copy, hxc_array_456c7a78_element_assign, hxc_array_456c7a78_element_destroy }, &hxc_l_tmp_array_create_result_n0) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_result = hxc_l_tmp_array_create_result_n0;
  int32_t hxc_l_g = 0;
  if (hxc_l_self == NULL)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n1 = (*hxc_l_self).hxc_triggerActorIds;
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
    int32_t hxc_l_actorIndex = hxc_l_tmp_increment_load_result_n5;
    struct hxc_caxecraft_domain_EntityStore *hxc_l_tmp_owned_class_field_address_n6 = &(*hxc_l_self).hxc_entities;
    if (hxc_l_tmp_owned_class_field_address_n6 == NULL)
    {
      abort();
    }
    struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n7 = (*hxc_l_self).hxc_triggerActorEntities;
    int32_t hxc_l_tmp_array_get_result_n9;
    if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n7, (size_t)hxc_l_actorIndex, &hxc_l_tmp_array_get_result_n9) != HXC_STATUS_OK)
    {
      abort();
    }
    struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n10 = hxc_caxecraft_domain_EntityStore_read(hxc_l_tmp_owned_class_field_address_n6, hxc_l_tmp_array_get_result_n9);
    struct hxc_caxecraft_domain_Character hxc_l_character = hxc_l_tmp_instance_call_result_n10;
    int32_t hxc_l_tmp_record_field_load_result_n11 = hxc_l_character.hxc_id;
    bool hxc_l_tmp_short_circuit_result_n7 = hxc_l_tmp_record_field_load_result_n11 > 0;
    if (hxc_l_tmp_record_field_load_result_n11 > 0)
    {
      hxc_l_tmp_short_circuit_result_n7 = hxc_l_character.hxc_vitals.hxc_health > 0;
    }
    bool hxc_l_tmp_short_circuit_load_result_n13 = hxc_l_tmp_short_circuit_result_n7;
    bool hxc_l_tmp_short_circuit_result_n8 = hxc_l_tmp_short_circuit_load_result_n13;
    if (hxc_l_tmp_short_circuit_load_result_n13)
    {
      if (hxc_l_executor == NULL)
      {
        abort();
      }
      struct hxc_caxecraft_scenario_CaxeFlowState *hxc_l_tmp_class_field_load_result_n14 = (*hxc_l_executor).hxc_state;
      hxc_l_gc_roots[2] = (const void *)hxc_l_tmp_class_field_load_result_n14;
      if (hxc_l_tmp_class_field_load_result_n14 == NULL)
      {
        abort();
      }
      struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n15 = (*hxc_l_self).hxc_triggerActorIds;
      hxc_string hxc_l_tmp_array_get_result_n17;
      if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n15, (size_t)hxc_l_actorIndex, &hxc_l_tmp_array_get_result_n17) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_l_tmp_array_element_owner_n9 = hxc_l_tmp_array_get_result_n17;
      bool hxc_l_tmp_instance_call_result_n19 = hxc_caxecraft_scenario_CaxeFlowState_objectActive(hxc_l_tmp_class_field_load_result_n14, hxc_l_tmp_array_element_owner_n9);
      hxc_l_tmp_short_circuit_result_n8 = hxc_l_tmp_instance_call_result_n19;
      if (hxc_string_release(&hxc_l_tmp_array_element_owner_n9) != HXC_STATUS_OK)
      {
        abort();
      }
    }
    if (hxc_l_tmp_short_circuit_result_n8)
    {
      struct hxc_caxecraft_scenario_FlowEventPosition hxc_l_tmp_call_result_n22 = hxc_caxecraft_domain_GameSession_flowEventPosition(hxc_l_character);
      struct hxc_caxecraft_scenario_FlowEventPosition hxc_l_position = hxc_l_tmp_call_result_n22;
      struct hxc_array_ref *hxc_l_tmp_load_result_n23 = hxc_l_result;
      struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n24 = (*hxc_l_self).hxc_triggerActorIds;
      hxc_string hxc_l_tmp_array_get_result_n26;
      if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n24, (size_t)hxc_l_actorIndex, &hxc_l_tmp_array_get_result_n26) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_l_tmp_array_element_owner_n11 = hxc_l_tmp_array_get_result_n26;
      hxc_string hxc_l_tmp_record_field_objectId_owner_n12 = hxc_l_tmp_array_element_owner_n11;
      if (hxc_string_retain(hxc_l_tmp_record_field_objectId_owner_n12) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_string hxc_l_tmp_record_field_objectId_owned_load_result_n28 = hxc_l_tmp_record_field_objectId_owner_n12;
      int32_t hxc_l_tmp_record_field_load_result_n29 = hxc_l_position.hxc_xMilli;
      int32_t hxc_l_tmp_record_field_load_result_n30 = hxc_l_position.hxc_yMilli;
      hxc_l_tmp_array_push_element_owner_n13 = (struct hxc_caxecraft_scenario_FlowPosition){ .hxc_objectId = hxc_l_tmp_record_field_objectId_owned_load_result_n28, .hxc_xMilli = hxc_l_tmp_record_field_load_result_n29, .hxc_yMilli = hxc_l_tmp_record_field_load_result_n30, .hxc_zMilli = hxc_l_position.hxc_zMilli };
      int32_t hxc_l_tmp_array_push_result_n34;
      if (hxc_array_ref_push_copy(hxc_l_tmp_load_result_n23, &hxc_l_tmp_array_push_element_owner_n13, &hxc_l_tmp_array_push_result_n34) != HXC_STATUS_OK)
      {
        abort();
      }
      (void)hxc_l_tmp_array_push_result_n34;
      hxc_record_24407721_destroy(&hxc_l_tmp_array_push_element_owner_n13);
      if (hxc_string_release(&hxc_l_tmp_array_element_owner_n11) != HXC_STATUS_OK)
      {
        abort();
      }
    }
  }
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return hxc_l_result;
}

struct hxc_caxecraft_domain_CharacterDamageResult hxc_caxecraft_domain_GameSession_damageCharacter(struct hxc_caxecraft_domain_GameSession *hxc_l_self, struct hxc_caxecraft_domain_CharacterDamageCommand hxc_l_command)
{
  const void *volatile hxc_l_gc_roots[3] = { NULL, NULL, NULL };
  hxc_l_gc_roots[0] = (const void *)hxc_l_self;
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 3, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_caxecraft_domain_CharacterDamageCause hxc_l_g = { 0 };
  hxc_string hxc_l_itemType = { 0 };
  struct hxc_optional_caxecraft_scenario_FlowEventOccurrence hxc_l_use = { 0 };
  struct hxc_caxecraft_scenario_FlowEvent hxc_l_tmp_instance_call_argument_0_owner_n15 = { 0 };
  hxc_string hxc_l_actor = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n17 = { 0 };
  struct hxc_caxecraft_scenario_FlowEventContext hxc_l_context = { 0 };
  struct hxc_caxecraft_scenario_FlowEvent hxc_l_tmp_static_call_argument_0_owner_n24 = { 0 };
  struct hxc_caxecraft_scenario_FlowEventOccurrence hxc_l_tmp_array_push_element_owner_n25 = { 0 };
  hxc_string hxc_l_defeatedAuthoredId = { 0 };
  struct hxc_array_ref *hxc_l_stagedEvents = { 0 };
  if (hxc_l_self == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n1 = hxc_caxecraft_domain_GameSession_readCharacter(hxc_l_self, hxc_l_command.hxc_target);
  struct hxc_caxecraft_domain_Character hxc_l_original = hxc_l_tmp_instance_call_result_n1;
  int32_t hxc_l_tmp_record_field_load_result_n2 = hxc_l_original.hxc_id;
  bool hxc_l_tmp_short_circuit_result_n3 = hxc_l_tmp_record_field_load_result_n2 <= 0;
  if (!(hxc_l_tmp_record_field_load_result_n2 <= 0))
  {
    hxc_l_tmp_short_circuit_result_n3 = hxc_l_command.hxc_amount <= 0;
  }
  if (hxc_l_tmp_short_circuit_result_n3)
  {
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return (struct hxc_caxecraft_domain_CharacterDamageResult){ .hxc_character = hxc_l_original, .hxc_damageApplied = 0, .hxc_defeated = false, .hxc_resolved = false };
  }
  struct hxc_caxecraft_domain_Character hxc_l_tmp_call_result_n9 = hxc_caxecraft_domain_Character_applyDamage(hxc_l_original, hxc_l_command.hxc_amount);
  struct hxc_caxecraft_domain_Character hxc_l_replacement = hxc_l_tmp_call_result_n9;
  int32_t hxc_l_tmp_record_field_load_result_n10 = hxc_l_original.hxc_vitals.hxc_health;
  bool hxc_l_tmp_short_circuit_result_n6 = hxc_l_tmp_record_field_load_result_n10 > 0;
  if (hxc_l_tmp_record_field_load_result_n10 > 0)
  {
    hxc_l_tmp_short_circuit_result_n6 = hxc_l_replacement.hxc_vitals.hxc_health <= 0;
  }
  bool hxc_l_newlyDefeated = hxc_l_tmp_short_circuit_result_n6;
  bool hxc_l_tmp_load_result_n13 = hxc_l_newlyDefeated;
  hxc_string hxc_l_tmp_conditional_managed_result_n8 = { 0 };
  if (hxc_l_tmp_load_result_n13)
  {
    hxc_string hxc_l_tmp_instance_call_result_n15 = hxc_caxecraft_domain_GameSession_authoredEnemyId(hxc_l_self, hxc_l_command.hxc_target);
    hxc_l_tmp_conditional_managed_result_n8 = hxc_l_tmp_instance_call_result_n15;
  }
  else
  {
    hxc_l_tmp_conditional_managed_result_n8 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_retain(hxc_l_tmp_conditional_managed_result_n8) != HXC_STATUS_OK)
    {
      abort();
    }
  }
  hxc_string hxc_l_tmp_conditional_managed_move_result_n16 = hxc_l_tmp_conditional_managed_result_n8;
  hxc_l_defeatedAuthoredId = hxc_l_tmp_conditional_managed_move_result_n16;
  struct hxc_array_ref *hxc_l_tmp_array_create_result_n17 = NULL;
  if (hxc_array_ref_create(hxc_default_allocator(), (hxc_array_element_ops){ sizeof(struct hxc_caxecraft_scenario_FlowEventOccurrence), _Alignof(struct hxc_caxecraft_scenario_FlowEventOccurrence), NULL, hxc_array_c6ef4080_element_copy, hxc_array_c6ef4080_element_assign, hxc_array_c6ef4080_element_destroy }, &hxc_l_tmp_array_create_result_n17) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_stagedEvents = hxc_l_tmp_array_create_result_n17;
  struct hxc_caxecraft_scenario_CaxeFlowExecutor *hxc_l_tmp_class_field_load_result_n18 = (*hxc_l_self).hxc_flowExecutor;
  hxc_l_gc_roots[1] = (const void *)hxc_l_tmp_class_field_load_result_n18;
  hxc_l_gc_roots[2] = (const void *)NULL;
  if (hxc_l_tmp_class_field_load_result_n18 != NULL)
  {
    hxc_string hxc_l_causeActor = { 0 };
    hxc_l_g = hxc_l_command.hxc_cause;
    if (hxc_enum_2d8c6351_retain(&hxc_l_g) != HXC_STATUS_OK)
    {
      abort();
    }
    switch (hxc_l_g.hxc_tag) {
      case hxc_caxecraft_domain_CharacterDamageCause_LocalPlayerItem:
        {
          if (hxc_l_g.hxc_tag != hxc_caxecraft_domain_CharacterDamageCause_LocalPlayerItem)
          {
            abort();
          }
          hxc_string hxc_l_tmp_enum_payload_project_n22 = hxc_l_g.hxc_payload.hxc_LocalPlayerItem.hxc_itemType;
          hxc_l_itemType = hxc_l_tmp_enum_payload_project_n22;
          if (hxc_string_retain(hxc_l_itemType) != HXC_STATUS_OK)
          {
            abort();
          }
          hxc_string hxc_l_tmp_enum_payload_0_owner_n14 = hxc_l_itemType;
          if (hxc_string_retain(hxc_l_tmp_enum_payload_0_owner_n14) != HXC_STATUS_OK)
          {
            abort();
          }
          hxc_l_tmp_instance_call_argument_0_owner_n15 = (struct hxc_caxecraft_scenario_FlowEvent){ .hxc_tag = hxc_caxecraft_scenario_FlowEvent_UseItem, .hxc_payload.hxc_UseItem.hxc_itemType = hxc_l_tmp_enum_payload_0_owner_n14 };
          struct hxc_optional_caxecraft_scenario_FlowEventOccurrence hxc_l_tmp_instance_call_result_n27 = hxc_caxecraft_domain_GameSession_localActorOccurrence(hxc_l_self, hxc_l_tmp_instance_call_argument_0_owner_n15);
          hxc_l_use = hxc_l_tmp_instance_call_result_n27;
          if (!hxc_l_use.hxc_has_value)
          {
            struct hxc_caxecraft_domain_CharacterDamageResult hxc_l_tmp_instance_call_result_n30 = hxc_caxecraft_domain_GameSession_rejectedCharacterDamage(hxc_l_self, hxc_l_original);
            hxc_optional_1cc4d348_destroy(&hxc_l_use);
            hxc_enum_10047054_destroy(&hxc_l_tmp_instance_call_argument_0_owner_n15);
            if (hxc_string_release(&hxc_l_itemType) != HXC_STATUS_OK)
            {
              abort();
            }
            hxc_enum_2d8c6351_destroy(&hxc_l_g);
            if (hxc_array_ref_release(hxc_l_stagedEvents) != HXC_STATUS_OK)
            {
              abort();
            }
            if (hxc_string_release(&hxc_l_defeatedAuthoredId) != HXC_STATUS_OK)
            {
              abort();
            }
            if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
            {
              abort();
            }
            return hxc_l_tmp_instance_call_result_n30;
          }
          else
          {
            struct hxc_array_ref *hxc_l_tmp_load_result_n31 = hxc_l_stagedEvents;
            struct hxc_optional_caxecraft_scenario_FlowEventOccurrence hxc_l_tmp_load_result_n32 = hxc_l_use;
            if (!hxc_l_tmp_load_result_n32.hxc_has_value)
            {
              abort();
            }
            int32_t hxc_l_tmp_array_push_result_n34;
            if (hxc_array_ref_push_copy(hxc_l_tmp_load_result_n31, &hxc_l_tmp_load_result_n32.hxc_value, &hxc_l_tmp_array_push_result_n34) != HXC_STATUS_OK)
            {
              abort();
            }
            (void)hxc_l_tmp_array_push_result_n34;
            hxc_string hxc_l_tmp_class_field_load_result_n35 = (*hxc_l_self).hxc_localPlayerAuthoredId;
            (void)hxc_l_tmp_class_field_load_result_n35;
            hxc_l_causeActor = hxc_l_tmp_class_field_load_result_n35;
            if (hxc_string_retain(hxc_l_causeActor) != HXC_STATUS_OK)
            {
              abort();
            }
            hxc_optional_1cc4d348_destroy(&hxc_l_use);
            hxc_enum_10047054_destroy(&hxc_l_tmp_instance_call_argument_0_owner_n15);
            if (hxc_string_release(&hxc_l_itemType) != HXC_STATUS_OK)
            {
              abort();
            }
          }
          break;
        }
      case hxc_caxecraft_domain_CharacterDamageCause_AuthoredActor:
        {
          if (hxc_l_g.hxc_tag != hxc_caxecraft_domain_CharacterDamageCause_AuthoredActor)
          {
            abort();
          }
          hxc_string hxc_l_tmp_enum_payload_project_n37 = hxc_l_g.hxc_payload.hxc_AuthoredActor.hxc_actor;
          hxc_l_actor = hxc_l_tmp_enum_payload_project_n37;
          if (hxc_string_retain(hxc_l_actor) != HXC_STATUS_OK)
          {
            abort();
          }
          (void)hxc_l_actor;
          hxc_l_causeActor = hxc_l_actor;
          if (hxc_string_retain(hxc_l_causeActor) != HXC_STATUS_OK)
          {
            abort();
          }
          if (hxc_string_release(&hxc_l_actor) != HXC_STATUS_OK)
          {
            abort();
          }
          break;
        }
      case hxc_caxecraft_domain_CharacterDamageCause_Environment:
        {
          hxc_l_causeActor = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
          if (hxc_string_retain(hxc_l_causeActor) != HXC_STATUS_OK)
          {
            abort();
          }
          break;
        }
      default:
        {
          abort();
        }
    }
    hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n39 = hxc_l_causeActor;
    hxc_l_tmp_managed_flow_owner_n17 = hxc_l_tmp_managed_flow_carrier_move_result_n39;
    bool hxc_l_tmp_string_null_equality_result_n18 = hxc_l_defeatedAuthoredId.data != NULL;
    if (hxc_l_tmp_string_null_equality_result_n18)
    {
      bool hxc_l_tmp_string_null_equality_result_n20 = hxc_l_tmp_managed_flow_owner_n17.data == NULL;
      bool hxc_l_tmp_string_null_equality_result_result_n43 = hxc_l_tmp_string_null_equality_result_n20;
      struct hxc_caxecraft_scenario_FlowEventContext hxc_l_tmp_conditional_managed_result_n21 = { 0 };
      if (hxc_l_tmp_string_null_equality_result_result_n43)
      {
        (void)(struct hxc_caxecraft_scenario_FlowEventContext){ .hxc_tag = hxc_caxecraft_scenario_FlowEventContext_NoEventContext };
        hxc_l_tmp_conditional_managed_result_n21 = (struct hxc_caxecraft_scenario_FlowEventContext){ .hxc_tag = hxc_caxecraft_scenario_FlowEventContext_NoEventContext };
      }
      else
      {
        hxc_string hxc_l_tmp_enum_payload_0_owner_n22 = hxc_l_tmp_managed_flow_owner_n17;
        if (hxc_string_retain(hxc_l_tmp_enum_payload_0_owner_n22) != HXC_STATUS_OK)
        {
          abort();
        }
        (void)(struct hxc_caxecraft_scenario_FlowEventContext){ .hxc_tag = hxc_caxecraft_scenario_FlowEventContext_ActorEventContext, .hxc_payload.hxc_ActorEventContext.hxc_actor = hxc_l_tmp_enum_payload_0_owner_n22 };
        hxc_l_tmp_conditional_managed_result_n21 = (struct hxc_caxecraft_scenario_FlowEventContext){ .hxc_tag = hxc_caxecraft_scenario_FlowEventContext_ActorEventContext, .hxc_payload.hxc_ActorEventContext.hxc_actor = hxc_l_tmp_enum_payload_0_owner_n22 };
      }
      struct hxc_caxecraft_scenario_FlowEventContext hxc_l_tmp_conditional_managed_move_result_n48 = hxc_l_tmp_conditional_managed_result_n21;
      hxc_l_context = hxc_l_tmp_conditional_managed_move_result_n48;
      struct hxc_array_ref *hxc_l_tmp_load_result_n49 = hxc_l_stagedEvents;
      hxc_string hxc_l_tmp_enum_payload_0_owner_n23 = hxc_l_defeatedAuthoredId;
      if (hxc_string_retain(hxc_l_tmp_enum_payload_0_owner_n23) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_l_tmp_static_call_argument_0_owner_n24 = (struct hxc_caxecraft_scenario_FlowEvent){ .hxc_tag = hxc_caxecraft_scenario_FlowEvent_EntityDefeated, .hxc_payload.hxc_EntityDefeated.hxc_entity = hxc_l_tmp_enum_payload_0_owner_n23 };
      struct hxc_caxecraft_scenario_FlowEvent hxc_l_tmp_static_call_argument_0_borrow_result_n53 = hxc_l_tmp_static_call_argument_0_owner_n24;
      struct hxc_caxecraft_scenario_FlowEventOccurrence hxc_l_tmp_call_result_n55 = hxc_caxecraft_scenario_CaxeFlowEventRegistry_flowEventOccurrence(hxc_l_tmp_static_call_argument_0_borrow_result_n53, hxc_l_context);
      hxc_l_tmp_array_push_element_owner_n25 = hxc_l_tmp_call_result_n55;
      int32_t hxc_l_tmp_array_push_result_n57;
      if (hxc_array_ref_push_copy(hxc_l_tmp_load_result_n49, &hxc_l_tmp_array_push_element_owner_n25, &hxc_l_tmp_array_push_result_n57) != HXC_STATUS_OK)
      {
        abort();
      }
      (void)hxc_l_tmp_array_push_result_n57;
      hxc_record_7a90fbbd_destroy(&hxc_l_tmp_array_push_element_owner_n25);
      hxc_enum_10047054_destroy(&hxc_l_tmp_static_call_argument_0_owner_n24);
      hxc_enum_276ca5e9_destroy(&hxc_l_context);
    }
    if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n17) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_enum_2d8c6351_destroy(&hxc_l_g);
  }
  bool hxc_l_tmp_instance_call_result_n59 = hxc_caxecraft_domain_GameSession_flowEventsCanCommit(hxc_l_self, hxc_l_stagedEvents);
  if (!hxc_l_tmp_instance_call_result_n59)
  {
    struct hxc_caxecraft_domain_Character hxc_l_tmp_load_result_n60 = hxc_l_original;
    if (hxc_array_ref_release(hxc_l_stagedEvents) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_defeatedAuthoredId) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return (struct hxc_caxecraft_domain_CharacterDamageResult){ .hxc_character = hxc_l_tmp_load_result_n60, .hxc_damageApplied = 0, .hxc_defeated = false, .hxc_resolved = false };
  }
  struct hxc_caxecraft_domain_EntityStore *hxc_l_tmp_owned_class_field_address_n62 = &(*hxc_l_self).hxc_entities;
  if (hxc_l_tmp_owned_class_field_address_n62 == NULL)
  {
    abort();
  }
  bool hxc_l_tmp_instance_call_result_n65 = hxc_caxecraft_domain_EntityStore_replace(hxc_l_tmp_owned_class_field_address_n62, hxc_l_command.hxc_target, hxc_l_replacement);
  bool hxc_l_resolved = hxc_l_tmp_instance_call_result_n65;
  if (!!hxc_l_resolved)
  {
    hxc_caxecraft_domain_GameSession_commitFlowEvents(hxc_l_self, hxc_l_stagedEvents);
    struct hxc_caxecraft_domain_Character hxc_l_tmp_load_result_n70 = hxc_l_replacement;
    int32_t hxc_l_tmp_record_field_load_result_n71 = hxc_l_original.hxc_vitals.hxc_health;
    int32_t hxc_l_tmp_record_field_load_result_n72 = hxc_l_replacement.hxc_vitals.hxc_health;
    bool hxc_l_tmp_load_result_n73 = hxc_l_newlyDefeated;
    if (hxc_array_ref_release(hxc_l_stagedEvents) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_defeatedAuthoredId) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return (struct hxc_caxecraft_domain_CharacterDamageResult){ .hxc_character = hxc_l_tmp_load_result_n70, .hxc_damageApplied = hxc_i32_subtract_wrapping(hxc_l_tmp_record_field_load_result_n71, hxc_l_tmp_record_field_load_result_n72), .hxc_defeated = hxc_l_tmp_load_result_n73, .hxc_resolved = true };
  }
  struct hxc_caxecraft_domain_Character hxc_l_tmp_load_result_n67 = hxc_l_original;
  if (hxc_array_ref_release(hxc_l_stagedEvents) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_string_release(&hxc_l_defeatedAuthoredId) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return (struct hxc_caxecraft_domain_CharacterDamageResult){ .hxc_character = hxc_l_tmp_load_result_n67, .hxc_damageApplied = 0, .hxc_defeated = false, .hxc_resolved = false };
}

int32_t hxc_caxecraft_domain_GameSession_externalFlowEventCapacity(struct hxc_caxecraft_domain_GameSession *hxc_l_self)
{
  const void *volatile hxc_l_gc_roots[1] = { NULL };
  hxc_l_gc_roots[0] = (const void *)hxc_l_self;
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 1, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_l_self == NULL)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n0 = (*hxc_l_self).hxc_triggerZoneIds;
  int32_t hxc_l_tmp_array_length_result_n1;
  if (hxc_array_ref_length(hxc_l_tmp_class_field_load_result_n0, &hxc_l_tmp_array_length_result_n1) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n2 = (*hxc_l_self).hxc_triggerActorIds;
  int32_t hxc_l_tmp_array_length_result_n3;
  if (hxc_array_ref_length(hxc_l_tmp_class_field_load_result_n2, &hxc_l_tmp_array_length_result_n3) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_tmp_call_result_n4 = hxc_caxecraft_scenario_CaxeFlowEventRegistry_externalFlowEventCapacity(hxc_l_tmp_array_length_result_n1, hxc_l_tmp_array_length_result_n3);
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return hxc_l_tmp_call_result_n4;
}

hxc_string hxc_caxecraft_domain_GameSession_flowBlockContentId(struct hxc_caxecraft_domain_GameSession *hxc_l_self, int32_t hxc_l_code)
{
  const void *volatile hxc_l_gc_roots[1] = { NULL };
  hxc_l_gc_roots[0] = (const void *)hxc_l_self;
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 1, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_string hxc_l_tmp_array_element_owner_n5 = { 0 };
  int32_t hxc_l_g = 0;
  if (hxc_l_self == NULL)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n0 = (*hxc_l_self).hxc_flowBlockStorageCodes;
  int32_t hxc_l_tmp_array_length_result_n1;
  if (hxc_array_ref_length(hxc_l_tmp_class_field_load_result_n0, &hxc_l_tmp_array_length_result_n1) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_g1 = hxc_l_tmp_array_length_result_n1;
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n2 = hxc_l_g;
    if (!(hxc_l_tmp_load_result_n2 < hxc_l_g1))
    {
      break;
    }
    int32_t hxc_l_tmp_increment_load_result_n4 = hxc_l_g;
    hxc_l_g = hxc_i32_add_wrapping(hxc_l_tmp_increment_load_result_n4, 1);
    int32_t hxc_l_index = hxc_l_tmp_increment_load_result_n4;
    struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n5 = (*hxc_l_self).hxc_flowBlockStorageCodes;
    int32_t hxc_l_tmp_array_get_result_n7;
    if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n5, (size_t)hxc_l_index, &hxc_l_tmp_array_get_result_n7) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_l_tmp_array_get_result_n7 == hxc_l_code)
    {
      struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n8 = (*hxc_l_self).hxc_flowBlockContentIds;
      hxc_string hxc_l_tmp_array_get_result_n10;
      if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n8, (size_t)hxc_l_index, &hxc_l_tmp_array_get_result_n10) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_l_tmp_array_element_owner_n5 = hxc_l_tmp_array_get_result_n10;
      if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
      {
        abort();
      }
      return hxc_l_tmp_array_element_owner_n5;
    }
  }
  hxc_string hxc_l_tmp_returned_string_owner_n6 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
  if (hxc_string_retain(hxc_l_tmp_returned_string_owner_n6) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return hxc_l_tmp_returned_string_owner_n6;
}

bool hxc_caxecraft_domain_GameSession_flowEventsCanCommit(struct hxc_caxecraft_domain_GameSession *hxc_l_self, struct hxc_array_ref *hxc_l_events)
{
  const void *volatile hxc_l_gc_roots[6] = { NULL, NULL, NULL, NULL, NULL, NULL };
  hxc_l_gc_roots[0] = (const void *)hxc_l_self;
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 6, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_caxecraft_scenario_FlowEventOccurrence hxc_l_event = { 0 };
  struct hxc_caxecraft_scenario_FlowEventOccurrence hxc_l_tmp_array_element_owner_n7 = { 0 };
  int32_t hxc_l_tmp_array_length_result_n0;
  if (hxc_array_ref_length(hxc_l_events, &hxc_l_tmp_array_length_result_n0) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_l_tmp_array_length_result_n0 == 0)
  {
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return true;
  }
  if (hxc_l_self == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_scenario_CaxeFlowExecutor *hxc_l_tmp_class_field_load_result_n1 = (*hxc_l_self).hxc_flowExecutor;
  hxc_l_gc_roots[1] = (const void *)hxc_l_tmp_class_field_load_result_n1;
  struct hxc_caxecraft_scenario_CaxeFlowExecutor *hxc_l_executor = hxc_l_tmp_class_field_load_result_n1;
  struct hxc_caxecraft_scenario_CaxeFlowExecutor *hxc_l_tmp_load_result_n2 = hxc_l_executor;
  hxc_l_gc_roots[2] = (const void *)hxc_l_tmp_load_result_n2;
  hxc_l_gc_roots[3] = (const void *)NULL;
  bool hxc_l_tmp_short_circuit_result_n3 = hxc_l_tmp_load_result_n2 == NULL;
  if (!(hxc_l_tmp_load_result_n2 == NULL))
  {
    struct hxc_caxecraft_scenario_CaxeFlowExecutor *hxc_l_tmp_load_result_n3 = hxc_l_executor;
    hxc_l_gc_roots[4] = (const void *)hxc_l_tmp_load_result_n3;
    if (hxc_l_tmp_load_result_n3 == NULL)
    {
      abort();
    }
    struct hxc_optional_caxecraft_scenario_FlowRuntimeDiagnostic hxc_l_tmp_class_field_load_result_n4 = (*hxc_l_tmp_load_result_n3).hxc_runtimeFault;
    hxc_l_tmp_short_circuit_result_n3 = hxc_l_tmp_class_field_load_result_n4.hxc_has_value;
  }
  bool hxc_l_tmp_short_circuit_load_result_n5 = hxc_l_tmp_short_circuit_result_n3;
  bool hxc_l_tmp_short_circuit_result_n4 = hxc_l_tmp_short_circuit_load_result_n5;
  if (!hxc_l_tmp_short_circuit_load_result_n5)
  {
    struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n6 = (*hxc_l_self).hxc_pendingFlowEvents;
    int32_t hxc_l_tmp_array_length_result_n7;
    if (hxc_array_ref_length(hxc_l_tmp_class_field_load_result_n6, &hxc_l_tmp_array_length_result_n7) != HXC_STATUS_OK)
    {
      abort();
    }
    int32_t hxc_l_tmp_array_length_result_n8;
    if (hxc_array_ref_length(hxc_l_events, &hxc_l_tmp_array_length_result_n8) != HXC_STATUS_OK)
    {
      abort();
    }
    int32_t hxc_l_tmp_instance_call_result_n9 = hxc_caxecraft_domain_GameSession_externalFlowEventCapacity(hxc_l_self);
    hxc_l_tmp_short_circuit_result_n4 = hxc_i32_add_wrapping(hxc_l_tmp_array_length_result_n7, hxc_l_tmp_array_length_result_n8) > hxc_l_tmp_instance_call_result_n9;
  }
  if (hxc_l_tmp_short_circuit_result_n4)
  {
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return false;
  }
  int32_t hxc_l_g = 0;
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n11 = hxc_l_g;
    int32_t hxc_l_tmp_array_length_result_n12;
    if (hxc_array_ref_length(hxc_l_events, &hxc_l_tmp_array_length_result_n12) != HXC_STATUS_OK)
    {
      abort();
    }
    if (!(hxc_l_tmp_load_result_n11 < hxc_l_tmp_array_length_result_n12))
    {
      break;
    }
    struct hxc_caxecraft_scenario_FlowEventOccurrence hxc_l_tmp_array_get_result_n14;
    if (hxc_array_ref_get_copy(hxc_l_events, (size_t)hxc_l_g, &hxc_l_tmp_array_get_result_n14) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_array_element_owner_n7 = hxc_l_tmp_array_get_result_n14;
    hxc_l_event = hxc_l_tmp_array_element_owner_n7;
    if (hxc_record_7a90fbbd_retain(&hxc_l_event) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_g = hxc_i32_add_wrapping(hxc_l_g, 1);
    struct hxc_caxecraft_scenario_CaxeFlowExecutor *hxc_l_tmp_load_result_n17 = hxc_l_executor;
    hxc_l_gc_roots[5] = (const void *)hxc_l_tmp_load_result_n17;
    if (hxc_l_tmp_load_result_n17 == NULL)
    {
      abort();
    }
    bool hxc_l_tmp_instance_call_result_n19 = hxc_caxecraft_scenario_CaxeFlowExecutor_acceptsRuntimeEvent(hxc_l_tmp_load_result_n17, hxc_l_event);
    if (!hxc_l_tmp_instance_call_result_n19)
    {
      hxc_record_7a90fbbd_destroy(&hxc_l_event);
      hxc_array_c6ef4080_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n7);
      if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
      {
        abort();
      }
      return false;
    }
    hxc_record_7a90fbbd_destroy(&hxc_l_event);
    hxc_array_c6ef4080_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n7);
  }
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return true;
}

void hxc_caxecraft_domain_GameSession_installValidatedScenarioFlow(struct hxc_caxecraft_domain_GameSession *hxc_l_self, struct hxc_caxecraft_scenario_Scenario hxc_l_scenario, struct hxc_compiler_interface_dispatch_caxecraft_scenario_ScenarioContentRegistry_value hxc_l_registry, hxc_string hxc_l_playerId, struct hxc_array_ref *hxc_l_actorEntities, struct hxc_array_ref *hxc_l_actorIds, struct hxc_array_ref *hxc_l_itemContentIds, struct hxc_array_ref *hxc_l_objectIds, struct hxc_array_ref *hxc_l_objectPositionsMilli, struct hxc_array_ref *hxc_l_objectRadiiMilli, struct hxc_array_ref *hxc_l_objectBoundsMilli, struct hxc_array_ref *hxc_l_objectStateStarts, struct hxc_array_ref *hxc_l_objectStateCounts, struct hxc_array_ref *hxc_l_objectCollisionStates, struct hxc_array_ref *hxc_l_objectCollisionSolid, struct hxc_array_ref *hxc_l_zoneIds, struct hxc_array_ref *hxc_l_zoneBounds)
{
  const void *volatile hxc_l_gc_roots[7] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL };
  hxc_l_gc_roots[0] = (const void *)hxc_l_self;
  hxc_l_gc_roots[1] = (const void *)hxc_l_scenario.hxc_flow.hxc_rules;
  hxc_l_gc_roots[2] = (const void *)hxc_l_scenario.hxc_flow.hxc_sequences;
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 7, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_caxecraft_domain_ActorControllerState hxc_l_tmp_array_element_owner_n21 = { 0 };
  hxc_string hxc_l_blockType = { 0 };
  hxc_string hxc_l_tmp_array_element_owner_n57 = { 0 };
  struct hxc_array_ref *hxc_l_g1_he65afda5393d = { 0 };
  hxc_string hxc_l_symbol_h797018683ba1 = { 0 };
  hxc_string hxc_l_tmp_array_element_owner_n64 = { 0 };
  struct hxc_caxecraft_scenario_FlowEvent hxc_l_tmp_static_call_argument_0_owner_n66 = { 0 };
  struct hxc_caxecraft_scenario_FlowEventContext hxc_l_tmp_static_call_argument_1_owner_n67 = { 0 };
  struct hxc_caxecraft_scenario_FlowEventOccurrence hxc_l_tmp_array_push_element_owner_n68 = { 0 };
  if (hxc_l_self == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_scenario_CaxeFlowExecutor *hxc_l_tmp_class_field_load_result_n0 = (*hxc_l_self).hxc_flowExecutor;
  hxc_l_gc_roots[3] = (const void *)hxc_l_tmp_class_field_load_result_n0;
  hxc_l_gc_roots[4] = (const void *)NULL;
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
  bool hxc_l_tmp_short_circuit_result_n17 = hxc_l_tmp_array_length_result_n1 != hxc_l_tmp_array_length_result_n2;
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
    hxc_l_tmp_short_circuit_result_n17 = hxc_l_tmp_array_length_result_n3 != hxc_l_tmp_array_length_result_n5;
  }
  if (hxc_l_tmp_short_circuit_result_n17)
  {
    (void)(hxc_string){ (const uint8_t *)"CaxeFlow actor bindings do not match this GameSession", 53, true, NULL };
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    abort();
  }
  int32_t hxc_l_g_h7383ce96b1d9 = 0;
  int32_t hxc_l_tmp_array_length_result_n7;
  if (hxc_array_ref_length(hxc_l_actorEntities, &hxc_l_tmp_array_length_result_n7) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_g1_hb385fe151b96 = hxc_l_tmp_array_length_result_n7;
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n8 = hxc_l_g_h7383ce96b1d9;
    if (!(hxc_l_tmp_load_result_n8 < hxc_l_g1_hb385fe151b96))
    {
      break;
    }
    int32_t hxc_l_tmp_increment_load_result_n10 = hxc_l_g_h7383ce96b1d9;
    hxc_l_g_h7383ce96b1d9 = hxc_i32_add_wrapping(hxc_l_tmp_increment_load_result_n10, 1);
    int32_t hxc_l_index_hae15a284c2f4 = hxc_l_tmp_increment_load_result_n10;
    int32_t hxc_l_tmp_array_get_result_n12;
    if (hxc_array_ref_get_copy(hxc_l_actorEntities, (size_t)hxc_l_index_hae15a284c2f4, &hxc_l_tmp_array_get_result_n12) != HXC_STATUS_OK)
    {
      abort();
    }
    struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n13 = (*hxc_l_self).hxc_actorControllers;
    struct hxc_caxecraft_domain_ActorControllerState hxc_l_tmp_array_get_result_n15;
    if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n13, (size_t)hxc_l_index_hae15a284c2f4, &hxc_l_tmp_array_get_result_n15) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_array_element_owner_n21 = hxc_l_tmp_array_get_result_n15;
    struct hxc_caxecraft_domain_ActorControllerState hxc_l_tmp_array_element_borrow_result_n16 = hxc_l_tmp_array_element_owner_n21;
    hxc_array_3767ae6c_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n21);
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
  int32_t hxc_l_tmp_array_length_result_n18;
  if (hxc_array_ref_length(hxc_l_itemContentIds, &hxc_l_tmp_array_length_result_n18) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_l_tmp_array_length_result_n18 > 256)
  {
    (void)(hxc_string){ (const uint8_t *)"CaxeFlow item bindings exceed this GameSession", 46, true, NULL };
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    abort();
  }
  int32_t hxc_l_tmp_array_length_result_n19;
  if (hxc_array_ref_length(hxc_l_objectPositionsMilli, &hxc_l_tmp_array_length_result_n19) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_tmp_array_length_result_n20;
  if (hxc_array_ref_length(hxc_l_objectIds, &hxc_l_tmp_array_length_result_n20) != HXC_STATUS_OK)
  {
    abort();
  }
  bool hxc_l_tmp_short_circuit_result_n22 = hxc_l_tmp_array_length_result_n19 != hxc_i32_multiply_wrapping(hxc_l_tmp_array_length_result_n20, 3);
  if (!(hxc_l_tmp_array_length_result_n19 != hxc_i32_multiply_wrapping(hxc_l_tmp_array_length_result_n20, 3)))
  {
    int32_t hxc_l_tmp_array_length_result_n21;
    if (hxc_array_ref_length(hxc_l_objectRadiiMilli, &hxc_l_tmp_array_length_result_n21) != HXC_STATUS_OK)
    {
      abort();
    }
    int32_t hxc_l_tmp_array_length_result_n22;
    if (hxc_array_ref_length(hxc_l_objectIds, &hxc_l_tmp_array_length_result_n22) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_short_circuit_result_n22 = hxc_l_tmp_array_length_result_n21 != hxc_l_tmp_array_length_result_n22;
  }
  if (hxc_l_tmp_short_circuit_result_n22)
  {
    (void)(hxc_string){ (const uint8_t *)"CaxeFlow stateful-object bindings do not match this GameSession", 63, true, NULL };
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    abort();
  }
  int32_t hxc_l_tmp_array_length_result_n24;
  if (hxc_array_ref_length(hxc_l_objectBoundsMilli, &hxc_l_tmp_array_length_result_n24) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_tmp_array_length_result_n25;
  if (hxc_array_ref_length(hxc_l_objectIds, &hxc_l_tmp_array_length_result_n25) != HXC_STATUS_OK)
  {
    abort();
  }
  bool hxc_l_tmp_short_circuit_result_n23 = hxc_l_tmp_array_length_result_n24 != hxc_i32_multiply_wrapping(hxc_l_tmp_array_length_result_n25, 4);
  if (!(hxc_l_tmp_array_length_result_n24 != hxc_i32_multiply_wrapping(hxc_l_tmp_array_length_result_n25, 4)))
  {
    int32_t hxc_l_tmp_array_length_result_n26;
    if (hxc_array_ref_length(hxc_l_objectStateStarts, &hxc_l_tmp_array_length_result_n26) != HXC_STATUS_OK)
    {
      abort();
    }
    int32_t hxc_l_tmp_array_length_result_n27;
    if (hxc_array_ref_length(hxc_l_objectIds, &hxc_l_tmp_array_length_result_n27) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_short_circuit_result_n23 = hxc_l_tmp_array_length_result_n26 != hxc_l_tmp_array_length_result_n27;
  }
  bool hxc_l_tmp_short_circuit_load_result_n28 = hxc_l_tmp_short_circuit_result_n23;
  bool hxc_l_tmp_short_circuit_result_n24 = hxc_l_tmp_short_circuit_load_result_n28;
  if (!hxc_l_tmp_short_circuit_load_result_n28)
  {
    int32_t hxc_l_tmp_array_length_result_n29;
    if (hxc_array_ref_length(hxc_l_objectStateCounts, &hxc_l_tmp_array_length_result_n29) != HXC_STATUS_OK)
    {
      abort();
    }
    int32_t hxc_l_tmp_array_length_result_n30;
    if (hxc_array_ref_length(hxc_l_objectIds, &hxc_l_tmp_array_length_result_n30) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_short_circuit_result_n24 = hxc_l_tmp_array_length_result_n29 != hxc_l_tmp_array_length_result_n30;
  }
  bool hxc_l_tmp_short_circuit_load_result_n31 = hxc_l_tmp_short_circuit_result_n24;
  bool hxc_l_tmp_short_circuit_result_n25 = hxc_l_tmp_short_circuit_load_result_n31;
  if (!hxc_l_tmp_short_circuit_load_result_n31)
  {
    int32_t hxc_l_tmp_array_length_result_n32;
    if (hxc_array_ref_length(hxc_l_objectCollisionStates, &hxc_l_tmp_array_length_result_n32) != HXC_STATUS_OK)
    {
      abort();
    }
    int32_t hxc_l_tmp_array_length_result_n33;
    if (hxc_array_ref_length(hxc_l_objectCollisionSolid, &hxc_l_tmp_array_length_result_n33) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_short_circuit_result_n25 = hxc_l_tmp_array_length_result_n32 != hxc_l_tmp_array_length_result_n33;
  }
  if (hxc_l_tmp_short_circuit_result_n25)
  {
    (void)(hxc_string){ (const uint8_t *)"CaxeFlow stateful-object collision bindings do not match this GameSession", 73, true, NULL };
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    abort();
  }
  int32_t hxc_l_g_h0b1971e608a4 = 0;
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n35 = hxc_l_g_h0b1971e608a4;
    int32_t hxc_l_tmp_array_length_result_n36;
    if (hxc_array_ref_length(hxc_l_objectRadiiMilli, &hxc_l_tmp_array_length_result_n36) != HXC_STATUS_OK)
    {
      abort();
    }
    if (!(hxc_l_tmp_load_result_n35 < hxc_l_tmp_array_length_result_n36))
    {
      break;
    }
    int32_t hxc_l_tmp_array_get_result_n38;
    if (hxc_array_ref_get_copy(hxc_l_objectRadiiMilli, (size_t)hxc_l_g_h0b1971e608a4, &hxc_l_tmp_array_get_result_n38) != HXC_STATUS_OK)
    {
      abort();
    }
    int32_t hxc_l_radius = hxc_l_tmp_array_get_result_n38;
    hxc_l_g_h0b1971e608a4 = hxc_i32_add_wrapping(hxc_l_g_h0b1971e608a4, 1);
    if (hxc_l_radius < 0)
    {
      (void)(hxc_string){ (const uint8_t *)"CaxeFlow stateful-object interaction range is invalid", 53, true, NULL };
      if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
      {
        abort();
      }
      abort();
    }
  }
  int32_t hxc_l_expectedStateStart = 0;
  int32_t hxc_l_g_hd0998fe24ca8 = 0;
  int32_t hxc_l_tmp_array_length_result_n41;
  if (hxc_array_ref_length(hxc_l_objectIds, &hxc_l_tmp_array_length_result_n41) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_g1_h647b8cd4c4b2 = hxc_l_tmp_array_length_result_n41;
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n42 = hxc_l_g_hd0998fe24ca8;
    if (!(hxc_l_tmp_load_result_n42 < hxc_l_g1_h647b8cd4c4b2))
    {
      break;
    }
    int32_t hxc_l_tmp_increment_load_result_n44 = hxc_l_g_hd0998fe24ca8;
    hxc_l_g_hd0998fe24ca8 = hxc_i32_add_wrapping(hxc_l_tmp_increment_load_result_n44, 1);
    int32_t hxc_l_objectIndex = hxc_l_tmp_increment_load_result_n44;
    int32_t hxc_l_boundsOffset = hxc_i32_multiply_wrapping(hxc_l_objectIndex, 4);
    int32_t hxc_l_tmp_array_get_result_n47;
    if (hxc_array_ref_get_copy(hxc_l_objectBoundsMilli, (size_t)hxc_i32_add_wrapping(hxc_l_boundsOffset, 3), &hxc_l_tmp_array_get_result_n47) != HXC_STATUS_OK)
    {
      abort();
    }
    int32_t hxc_l_yaw = hxc_l_tmp_array_get_result_n47;
    int32_t hxc_l_tmp_array_get_result_n49;
    if (hxc_array_ref_get_copy(hxc_l_objectBoundsMilli, (size_t)hxc_l_boundsOffset, &hxc_l_tmp_array_get_result_n49) != HXC_STATUS_OK)
    {
      abort();
    }
    bool hxc_l_tmp_short_circuit_result_n34 = hxc_l_tmp_array_get_result_n49 <= 0;
    if (!(hxc_l_tmp_array_get_result_n49 <= 0))
    {
      int32_t hxc_l_tmp_array_get_result_n51;
      if (hxc_array_ref_get_copy(hxc_l_objectBoundsMilli, (size_t)hxc_i32_add_wrapping(hxc_l_boundsOffset, 1), &hxc_l_tmp_array_get_result_n51) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_l_tmp_short_circuit_result_n34 = hxc_l_tmp_array_get_result_n51 <= 0;
    }
    bool hxc_l_tmp_short_circuit_load_result_n52 = hxc_l_tmp_short_circuit_result_n34;
    bool hxc_l_tmp_short_circuit_result_n35 = hxc_l_tmp_short_circuit_load_result_n52;
    if (!hxc_l_tmp_short_circuit_load_result_n52)
    {
      int32_t hxc_l_tmp_array_get_result_n54;
      if (hxc_array_ref_get_copy(hxc_l_objectBoundsMilli, (size_t)hxc_i32_add_wrapping(hxc_l_boundsOffset, 2), &hxc_l_tmp_array_get_result_n54) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_l_tmp_short_circuit_result_n35 = hxc_l_tmp_array_get_result_n54 <= 0;
    }
    if (hxc_l_tmp_short_circuit_result_n35)
    {
      (void)(hxc_string){ (const uint8_t *)"CaxeFlow stateful-object collision bounds are invalid", 53, true, NULL };
      if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
      {
        abort();
      }
      abort();
    }
    int32_t hxc_l_tmp_load_result_n56 = hxc_l_yaw;
    bool hxc_l_tmp_short_circuit_result_n36 = hxc_l_tmp_load_result_n56 != 0;
    if (hxc_l_tmp_load_result_n56 != 0)
    {
      hxc_l_tmp_short_circuit_result_n36 = hxc_l_yaw != 90;
    }
    bool hxc_l_tmp_short_circuit_load_result_n58 = hxc_l_tmp_short_circuit_result_n36;
    bool hxc_l_tmp_short_circuit_result_n37 = hxc_l_tmp_short_circuit_load_result_n58;
    if (hxc_l_tmp_short_circuit_load_result_n58)
    {
      hxc_l_tmp_short_circuit_result_n37 = hxc_l_yaw != 180;
    }
    bool hxc_l_tmp_short_circuit_load_result_n60 = hxc_l_tmp_short_circuit_result_n37;
    bool hxc_l_tmp_short_circuit_result_n38 = hxc_l_tmp_short_circuit_load_result_n60;
    if (hxc_l_tmp_short_circuit_load_result_n60)
    {
      hxc_l_tmp_short_circuit_result_n38 = hxc_l_yaw != 270;
    }
    if (hxc_l_tmp_short_circuit_result_n38)
    {
      (void)(hxc_string){ (const uint8_t *)"CaxeFlow stateful-object collision yaw is invalid", 49, true, NULL };
      if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
      {
        abort();
      }
      abort();
    }
    int32_t hxc_l_tmp_array_get_result_n64;
    if (hxc_array_ref_get_copy(hxc_l_objectStateStarts, (size_t)hxc_l_objectIndex, &hxc_l_tmp_array_get_result_n64) != HXC_STATUS_OK)
    {
      abort();
    }
    int32_t hxc_l_tmp_load_result_n65 = hxc_l_expectedStateStart;
    bool hxc_l_tmp_short_circuit_result_n39 = hxc_l_tmp_array_get_result_n64 != hxc_l_tmp_load_result_n65;
    if (!(hxc_l_tmp_array_get_result_n64 != hxc_l_tmp_load_result_n65))
    {
      int32_t hxc_l_tmp_array_get_result_n67;
      if (hxc_array_ref_get_copy(hxc_l_objectStateCounts, (size_t)hxc_l_objectIndex, &hxc_l_tmp_array_get_result_n67) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_l_tmp_short_circuit_result_n39 = hxc_l_tmp_array_get_result_n67 <= 0;
    }
    if (hxc_l_tmp_short_circuit_result_n39)
    {
      (void)(hxc_string){ (const uint8_t *)"CaxeFlow stateful-object collision state range is invalid", 57, true, NULL };
      if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
      {
        abort();
      }
      abort();
    }
    int32_t hxc_l_tmp_compound_load_result_n69 = hxc_l_expectedStateStart;
    int32_t hxc_l_tmp_array_get_result_n71;
    if (hxc_array_ref_get_copy(hxc_l_objectStateCounts, (size_t)hxc_l_objectIndex, &hxc_l_tmp_array_get_result_n71) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_expectedStateStart = hxc_i32_add_wrapping(hxc_l_tmp_compound_load_result_n69, hxc_l_tmp_array_get_result_n71);
    int32_t hxc_l_tmp_load_result_n72 = hxc_l_expectedStateStart;
    int32_t hxc_l_tmp_array_length_result_n73;
    if (hxc_array_ref_length(hxc_l_objectCollisionStates, &hxc_l_tmp_array_length_result_n73) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_l_tmp_load_result_n72 > hxc_l_tmp_array_length_result_n73)
    {
      (void)(hxc_string){ (const uint8_t *)"CaxeFlow stateful-object collision state range exceeds its values", 65, true, NULL };
      if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
      {
        abort();
      }
      abort();
    }
  }
  int32_t hxc_l_tmp_load_result_n74 = hxc_l_expectedStateStart;
  int32_t hxc_l_tmp_array_length_result_n75;
  if (hxc_array_ref_length(hxc_l_objectCollisionStates, &hxc_l_tmp_array_length_result_n75) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_l_tmp_load_result_n74 != hxc_l_tmp_array_length_result_n75)
  {
    (void)(hxc_string){ (const uint8_t *)"CaxeFlow stateful-object collision states are not completely owned", 66, true, NULL };
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    abort();
  }
  int32_t hxc_l_g_hfb3e4789f870 = 0;
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n76 = hxc_l_g_hfb3e4789f870;
    int32_t hxc_l_tmp_array_length_result_n77;
    if (hxc_array_ref_length(hxc_l_objectCollisionSolid, &hxc_l_tmp_array_length_result_n77) != HXC_STATUS_OK)
    {
      abort();
    }
    if (!(hxc_l_tmp_load_result_n76 < hxc_l_tmp_array_length_result_n77))
    {
      break;
    }
    int32_t hxc_l_tmp_array_get_result_n79;
    if (hxc_array_ref_get_copy(hxc_l_objectCollisionSolid, (size_t)hxc_l_g_hfb3e4789f870, &hxc_l_tmp_array_get_result_n79) != HXC_STATUS_OK)
    {
      abort();
    }
    int32_t hxc_l_solid = hxc_l_tmp_array_get_result_n79;
    hxc_l_g_hfb3e4789f870 = hxc_i32_add_wrapping(hxc_l_g_hfb3e4789f870, 1);
    int32_t hxc_l_tmp_load_result_n81 = hxc_l_solid;
    bool hxc_l_tmp_short_circuit_result_n42 = hxc_l_tmp_load_result_n81 != 0;
    if (hxc_l_tmp_load_result_n81 != 0)
    {
      hxc_l_tmp_short_circuit_result_n42 = hxc_l_solid != 1;
    }
    if (hxc_l_tmp_short_circuit_result_n42)
    {
      (void)(hxc_string){ (const uint8_t *)"CaxeFlow stateful-object collision flag is invalid", 50, true, NULL };
      if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
      {
        abort();
      }
      abort();
    }
  }
  struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n84 = (*hxc_l_self).hxc_flowBlockStorageCodes;
  if (hxc_l_tmp_class_field_load_result_n84 == NULL)
  {
    abort();
  }
  if (hxc_array_resize(&hxc_l_tmp_class_field_load_result_n84->value, (size_t)0, NULL) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n85 = (*hxc_l_self).hxc_flowBlockContentIds;
  if (hxc_l_tmp_class_field_load_result_n85 == NULL)
  {
    abort();
  }
  if (hxc_array_resize(&hxc_l_tmp_class_field_load_result_n85->value, (size_t)0, NULL) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_g_hb52fceb2374d = 0;
  while (1)
  {
    if (!(hxc_l_g_hb52fceb2374d < 256))
    {
      break;
    }
    int32_t hxc_l_tmp_increment_load_result_n87 = hxc_l_g_hb52fceb2374d;
    hxc_l_g_hb52fceb2374d = hxc_i32_add_wrapping(hxc_l_tmp_increment_load_result_n87, 1);
    int32_t hxc_l_code = hxc_l_tmp_increment_load_result_n87;
    hxc_string hxc_l_tmp_instance_call_result_n89 = hxc_l_registry.table->hxc_interface_slot_caxecraft_scenario_ScenarioContentRegistry_blockContentIdForStorageCode(hxc_l_registry.object, hxc_l_code);
    hxc_l_blockType = hxc_l_tmp_instance_call_result_n89;
    bool hxc_l_tmp_string_null_equality_result_n46 = hxc_l_blockType.data != NULL;
    if (hxc_l_tmp_string_null_equality_result_n46)
    {
      int32_t hxc_l_tmp_instance_call_result_n93 = hxc_l_registry.table->hxc_interface_slot_caxecraft_scenario_ScenarioContentRegistry_blockStorageCode(hxc_l_registry.object, hxc_l_blockType);
      if (hxc_l_tmp_instance_call_result_n93 != hxc_l_code)
      {
        (void)(hxc_string){ (const uint8_t *)"CaxeFlow block content index is not reversible", 46, true, NULL };
        if (hxc_string_release(&hxc_l_blockType) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
        {
          abort();
        }
        abort();
      }
      else
      {
        struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n95 = (*hxc_l_self).hxc_flowBlockStorageCodes;
        int32_t hxc_l_tmp_array_push_result_n97;
        if (hxc_array_ref_push_copy(hxc_l_tmp_class_field_load_result_n95, &(int32_t){ hxc_l_code }, &hxc_l_tmp_array_push_result_n97) != HXC_STATUS_OK)
        {
          abort();
        }
        (void)hxc_l_tmp_array_push_result_n97;
        struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n98 = (*hxc_l_self).hxc_flowBlockContentIds;
        int32_t hxc_l_tmp_array_push_result_n100;
        if (hxc_array_ref_push_copy(hxc_l_tmp_class_field_load_result_n98, &hxc_l_blockType, &hxc_l_tmp_array_push_result_n100) != HXC_STATUS_OK)
        {
          abort();
        }
        (void)hxc_l_tmp_array_push_result_n100;
      }
    }
    if (hxc_string_release(&hxc_l_blockType) != HXC_STATUS_OK)
    {
      abort();
    }
  }
  int32_t hxc_l_tmp_array_length_result_n101;
  if (hxc_array_ref_length(hxc_l_zoneBounds, &hxc_l_tmp_array_length_result_n101) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_tmp_array_length_result_n102;
  if (hxc_array_ref_length(hxc_l_zoneIds, &hxc_l_tmp_array_length_result_n102) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_l_tmp_array_length_result_n101 != hxc_i32_multiply_wrapping(hxc_l_tmp_array_length_result_n102, 6))
  {
    (void)(hxc_string){ (const uint8_t *)"CaxeFlow trigger-zone bindings do not match this GameSession", 60, true, NULL };
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    abort();
  }
  int32_t hxc_l_g_h287d06a8ad81 = 0;
  int32_t hxc_l_tmp_array_length_result_n103;
  if (hxc_array_ref_length(hxc_l_zoneIds, &hxc_l_tmp_array_length_result_n103) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_g1_h7bc079747fc5 = hxc_l_tmp_array_length_result_n103;
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n104 = hxc_l_g_h287d06a8ad81;
    if (!(hxc_l_tmp_load_result_n104 < hxc_l_g1_h7bc079747fc5))
    {
      break;
    }
    int32_t hxc_l_tmp_increment_load_result_n106 = hxc_l_g_h287d06a8ad81;
    hxc_l_g_h287d06a8ad81 = hxc_i32_add_wrapping(hxc_l_tmp_increment_load_result_n106, 1);
    int32_t hxc_l_zoneIndex = hxc_l_tmp_increment_load_result_n106;
    int32_t hxc_l_offset = hxc_i32_multiply_wrapping(hxc_l_zoneIndex, 6);
    int32_t hxc_l_tmp_array_get_result_n109;
    if (hxc_array_ref_get_copy(hxc_l_zoneBounds, (size_t)hxc_i32_add_wrapping(hxc_l_offset, 3), &hxc_l_tmp_array_get_result_n109) != HXC_STATUS_OK)
    {
      abort();
    }
    bool hxc_l_tmp_short_circuit_result_n51 = hxc_l_tmp_array_get_result_n109 <= 0;
    if (!(hxc_l_tmp_array_get_result_n109 <= 0))
    {
      int32_t hxc_l_tmp_array_get_result_n111;
      if (hxc_array_ref_get_copy(hxc_l_zoneBounds, (size_t)hxc_i32_add_wrapping(hxc_l_offset, 4), &hxc_l_tmp_array_get_result_n111) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_l_tmp_short_circuit_result_n51 = hxc_l_tmp_array_get_result_n111 <= 0;
    }
    bool hxc_l_tmp_short_circuit_load_result_n112 = hxc_l_tmp_short_circuit_result_n51;
    bool hxc_l_tmp_short_circuit_result_n52 = hxc_l_tmp_short_circuit_load_result_n112;
    if (!hxc_l_tmp_short_circuit_load_result_n112)
    {
      int32_t hxc_l_tmp_array_get_result_n114;
      if (hxc_array_ref_get_copy(hxc_l_zoneBounds, (size_t)hxc_i32_add_wrapping(hxc_l_offset, 5), &hxc_l_tmp_array_get_result_n114) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_l_tmp_short_circuit_result_n52 = hxc_l_tmp_array_get_result_n114 <= 0;
    }
    if (hxc_l_tmp_short_circuit_result_n52)
    {
      (void)(hxc_string){ (const uint8_t *)"CaxeFlow trigger-zone bounds are invalid", 40, true, NULL };
      if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
      {
        abort();
      }
      abort();
    }
  }
  struct hxc_array_ref *hxc_l_tmp_array_copy_result_n116 = NULL;
  if (hxc_array_ref_copy(hxc_l_actorEntities, &hxc_l_tmp_array_copy_result_n116) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_ref_release((*hxc_l_self).hxc_authoredActorEntities) != HXC_STATUS_OK)
  {
    abort();
  }
  (*hxc_l_self).hxc_authoredActorEntities = hxc_l_tmp_array_copy_result_n116;
  struct hxc_array_ref *hxc_l_tmp_array_copy_result_n117 = NULL;
  if (hxc_array_ref_copy(hxc_l_actorIds, &hxc_l_tmp_array_copy_result_n117) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_ref_release((*hxc_l_self).hxc_authoredActorIds) != HXC_STATUS_OK)
  {
    abort();
  }
  (*hxc_l_self).hxc_authoredActorIds = hxc_l_tmp_array_copy_result_n117;
  hxc_string hxc_l_tmp_string_assignment_replacement_owner_n53 = hxc_l_playerId;
  if (hxc_string_retain(hxc_l_tmp_string_assignment_replacement_owner_n53) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_string hxc_l_tmp_string_assignment_replacement_owned_load_result_n118 = hxc_l_tmp_string_assignment_replacement_owner_n53;
  if (hxc_string_release(&(*hxc_l_self).hxc_localPlayerAuthoredId) != HXC_STATUS_OK)
  {
    abort();
  }
  (*hxc_l_self).hxc_localPlayerAuthoredId = hxc_l_tmp_string_assignment_replacement_owned_load_result_n118;
  struct hxc_array_ref *hxc_l_tmp_array_copy_result_n119 = NULL;
  if (hxc_array_ref_copy(hxc_l_itemContentIds, &hxc_l_tmp_array_copy_result_n119) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_ref_release((*hxc_l_self).hxc_authoredItemContentIds) != HXC_STATUS_OK)
  {
    abort();
  }
  (*hxc_l_self).hxc_authoredItemContentIds = hxc_l_tmp_array_copy_result_n119;
  struct hxc_array_ref *hxc_l_tmp_array_copy_result_n120 = NULL;
  if (hxc_array_ref_copy(hxc_l_objectIds, &hxc_l_tmp_array_copy_result_n120) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_ref_release((*hxc_l_self).hxc_statefulObjectIds) != HXC_STATUS_OK)
  {
    abort();
  }
  (*hxc_l_self).hxc_statefulObjectIds = hxc_l_tmp_array_copy_result_n120;
  struct hxc_array_ref *hxc_l_tmp_array_copy_result_n121 = NULL;
  if (hxc_array_ref_copy(hxc_l_objectPositionsMilli, &hxc_l_tmp_array_copy_result_n121) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_ref_release((*hxc_l_self).hxc_statefulObjectPositionsMilli) != HXC_STATUS_OK)
  {
    abort();
  }
  (*hxc_l_self).hxc_statefulObjectPositionsMilli = hxc_l_tmp_array_copy_result_n121;
  struct hxc_array_ref *hxc_l_tmp_array_copy_result_n122 = NULL;
  if (hxc_array_ref_copy(hxc_l_objectRadiiMilli, &hxc_l_tmp_array_copy_result_n122) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_ref_release((*hxc_l_self).hxc_statefulObjectRadiiMilli) != HXC_STATUS_OK)
  {
    abort();
  }
  (*hxc_l_self).hxc_statefulObjectRadiiMilli = hxc_l_tmp_array_copy_result_n122;
  struct hxc_array_ref *hxc_l_tmp_array_copy_result_n123 = NULL;
  if (hxc_array_ref_copy(hxc_l_objectBoundsMilli, &hxc_l_tmp_array_copy_result_n123) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_ref_release((*hxc_l_self).hxc_statefulObjectBoundsMilli) != HXC_STATUS_OK)
  {
    abort();
  }
  (*hxc_l_self).hxc_statefulObjectBoundsMilli = hxc_l_tmp_array_copy_result_n123;
  struct hxc_array_ref *hxc_l_tmp_array_copy_result_n124 = NULL;
  if (hxc_array_ref_copy(hxc_l_objectStateStarts, &hxc_l_tmp_array_copy_result_n124) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_ref_release((*hxc_l_self).hxc_statefulObjectStateStarts) != HXC_STATUS_OK)
  {
    abort();
  }
  (*hxc_l_self).hxc_statefulObjectStateStarts = hxc_l_tmp_array_copy_result_n124;
  struct hxc_array_ref *hxc_l_tmp_array_copy_result_n125 = NULL;
  if (hxc_array_ref_copy(hxc_l_objectStateCounts, &hxc_l_tmp_array_copy_result_n125) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_ref_release((*hxc_l_self).hxc_statefulObjectStateCounts) != HXC_STATUS_OK)
  {
    abort();
  }
  (*hxc_l_self).hxc_statefulObjectStateCounts = hxc_l_tmp_array_copy_result_n125;
  struct hxc_array_ref *hxc_l_tmp_array_copy_result_n126 = NULL;
  if (hxc_array_ref_copy(hxc_l_objectCollisionStates, &hxc_l_tmp_array_copy_result_n126) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_ref_release((*hxc_l_self).hxc_statefulObjectCollisionStates) != HXC_STATUS_OK)
  {
    abort();
  }
  (*hxc_l_self).hxc_statefulObjectCollisionStates = hxc_l_tmp_array_copy_result_n126;
  struct hxc_array_ref *hxc_l_tmp_array_copy_result_n127 = NULL;
  if (hxc_array_ref_copy(hxc_l_objectCollisionSolid, &hxc_l_tmp_array_copy_result_n127) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_ref_release((*hxc_l_self).hxc_statefulObjectCollisionSolid) != HXC_STATUS_OK)
  {
    abort();
  }
  (*hxc_l_self).hxc_statefulObjectCollisionSolid = hxc_l_tmp_array_copy_result_n127;
  struct hxc_array_ref *hxc_l_tmp_array_copy_result_n128 = NULL;
  if (hxc_array_ref_copy(hxc_l_zoneIds, &hxc_l_tmp_array_copy_result_n128) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_ref_release((*hxc_l_self).hxc_triggerZoneIds) != HXC_STATUS_OK)
  {
    abort();
  }
  (*hxc_l_self).hxc_triggerZoneIds = hxc_l_tmp_array_copy_result_n128;
  struct hxc_array_ref *hxc_l_tmp_array_copy_result_n129 = NULL;
  if (hxc_array_ref_copy(hxc_l_zoneBounds, &hxc_l_tmp_array_copy_result_n129) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_ref_release((*hxc_l_self).hxc_triggerZoneBounds) != HXC_STATUS_OK)
  {
    abort();
  }
  (*hxc_l_self).hxc_triggerZoneBounds = hxc_l_tmp_array_copy_result_n129;
  struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n130 = (*hxc_l_self).hxc_triggerActorEntities;
  if (hxc_l_tmp_class_field_load_result_n130 == NULL)
  {
    abort();
  }
  if (hxc_array_resize(&hxc_l_tmp_class_field_load_result_n130->value, (size_t)0, NULL) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n131 = (*hxc_l_self).hxc_triggerActorIds;
  if (hxc_l_tmp_class_field_load_result_n131 == NULL)
  {
    abort();
  }
  if (hxc_array_resize(&hxc_l_tmp_class_field_load_result_n131->value, (size_t)0, NULL) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n132 = (*hxc_l_self).hxc_triggerActorEntities;
  struct hxc_caxecraft_domain_PlayerAgent hxc_l_tmp_class_field_load_result_n133 = (*hxc_l_self).hxc_localPlayer;
  int32_t hxc_l_tmp_array_push_result_n135;
  if (hxc_array_ref_push_copy(hxc_l_tmp_class_field_load_result_n132, &(int32_t){ hxc_l_tmp_class_field_load_result_n133.hxc_characterId }, &hxc_l_tmp_array_push_result_n135) != HXC_STATUS_OK)
  {
    abort();
  }
  (void)hxc_l_tmp_array_push_result_n135;
  struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n136 = (*hxc_l_self).hxc_triggerActorIds;
  int32_t hxc_l_tmp_array_push_result_n137;
  if (hxc_array_ref_push_copy(hxc_l_tmp_class_field_load_result_n136, &hxc_l_playerId, &hxc_l_tmp_array_push_result_n137) != HXC_STATUS_OK)
  {
    abort();
  }
  (void)hxc_l_tmp_array_push_result_n137;
  int32_t hxc_l_g_h3aaa64a5a92d = 0;
  int32_t hxc_l_tmp_array_length_result_n138;
  if (hxc_array_ref_length(hxc_l_actorEntities, &hxc_l_tmp_array_length_result_n138) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_g1_h953e4afe2162 = hxc_l_tmp_array_length_result_n138;
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n139 = hxc_l_g_h3aaa64a5a92d;
    if (!(hxc_l_tmp_load_result_n139 < hxc_l_g1_h953e4afe2162))
    {
      break;
    }
    int32_t hxc_l_tmp_increment_load_result_n141 = hxc_l_g_h3aaa64a5a92d;
    hxc_l_g_h3aaa64a5a92d = hxc_i32_add_wrapping(hxc_l_tmp_increment_load_result_n141, 1);
    int32_t hxc_l_index_h0699cea12e6f = hxc_l_tmp_increment_load_result_n141;
    struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n142 = (*hxc_l_self).hxc_triggerActorEntities;
    int32_t hxc_l_tmp_array_get_result_n144;
    if (hxc_array_ref_get_copy(hxc_l_actorEntities, (size_t)hxc_l_index_h0699cea12e6f, &hxc_l_tmp_array_get_result_n144) != HXC_STATUS_OK)
    {
      abort();
    }
    int32_t hxc_l_tmp_array_push_result_n145;
    if (hxc_array_ref_push_copy(hxc_l_tmp_class_field_load_result_n142, &(int32_t){ hxc_l_tmp_array_get_result_n144 }, &hxc_l_tmp_array_push_result_n145) != HXC_STATUS_OK)
    {
      abort();
    }
    (void)hxc_l_tmp_array_push_result_n145;
    struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n146 = (*hxc_l_self).hxc_triggerActorIds;
    hxc_string hxc_l_tmp_array_get_result_n148;
    if (hxc_array_ref_get_copy(hxc_l_actorIds, (size_t)hxc_l_index_h0699cea12e6f, &hxc_l_tmp_array_get_result_n148) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_array_element_owner_n57 = hxc_l_tmp_array_get_result_n148;
    int32_t hxc_l_tmp_array_push_result_n150;
    if (hxc_array_ref_push_copy(hxc_l_tmp_class_field_load_result_n146, &hxc_l_tmp_array_element_owner_n57, &hxc_l_tmp_array_push_result_n150) != HXC_STATUS_OK)
    {
      abort();
    }
    (void)hxc_l_tmp_array_push_result_n150;
    if (hxc_string_release(&hxc_l_tmp_array_element_owner_n57) != HXC_STATUS_OK)
    {
      abort();
    }
  }
  struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n151 = (*hxc_l_self).hxc_triggerZoneInside;
  if (hxc_l_tmp_class_field_load_result_n151 == NULL)
  {
    abort();
  }
  if (hxc_array_resize(&hxc_l_tmp_class_field_load_result_n151->value, (size_t)0, NULL) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_g_h5acf37edb723 = 0;
  struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n152 = (*hxc_l_self).hxc_triggerZoneIds;
  int32_t hxc_l_tmp_array_length_result_n153;
  if (hxc_array_ref_length(hxc_l_tmp_class_field_load_result_n152, &hxc_l_tmp_array_length_result_n153) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n154 = (*hxc_l_self).hxc_triggerActorIds;
  int32_t hxc_l_tmp_array_length_result_n155;
  if (hxc_array_ref_length(hxc_l_tmp_class_field_load_result_n154, &hxc_l_tmp_array_length_result_n155) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_g1_h47320b3c2958 = hxc_i32_multiply_wrapping(hxc_l_tmp_array_length_result_n153, hxc_l_tmp_array_length_result_n155);
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n156 = hxc_l_g_h5acf37edb723;
    if (!(hxc_l_tmp_load_result_n156 < hxc_l_g1_h47320b3c2958))
    {
      break;
    }
    int32_t hxc_l_tmp_increment_load_result_n158 = hxc_l_g_h5acf37edb723;
    hxc_l_g_h5acf37edb723 = hxc_i32_add_wrapping(hxc_l_tmp_increment_load_result_n158, 1);
    int32_t hxc_l_symbol_h6746fd6a09e1 = hxc_l_tmp_increment_load_result_n158;
    (void)hxc_l_symbol_h6746fd6a09e1;
    struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n159 = (*hxc_l_self).hxc_triggerZoneInside;
    int32_t hxc_l_tmp_array_push_result_n160;
    if (hxc_array_ref_push_copy(hxc_l_tmp_class_field_load_result_n159, &(int32_t){ 0 }, &hxc_l_tmp_array_push_result_n160) != HXC_STATUS_OK)
    {
      abort();
    }
    (void)hxc_l_tmp_array_push_result_n160;
  }
  struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n161 = (*hxc_l_self).hxc_triggerActorPrevious;
  if (hxc_l_tmp_class_field_load_result_n161 == NULL)
  {
    abort();
  }
  if (hxc_array_resize(&hxc_l_tmp_class_field_load_result_n161->value, (size_t)0, NULL) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n162 = (*hxc_l_self).hxc_triggerActorPreviousValid;
  if (hxc_l_tmp_class_field_load_result_n162 == NULL)
  {
    abort();
  }
  if (hxc_array_resize(&hxc_l_tmp_class_field_load_result_n162->value, (size_t)0, NULL) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_g_he15035e768bf = 0;
  struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n163 = (*hxc_l_self).hxc_triggerActorIds;
  hxc_l_g1_he65afda5393d = hxc_l_tmp_class_field_load_result_n163;
  if (hxc_array_ref_retain(hxc_l_g1_he65afda5393d) != HXC_STATUS_OK)
  {
    abort();
  }
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n164 = hxc_l_g_he15035e768bf;
    int32_t hxc_l_tmp_array_length_result_n166;
    if (hxc_array_ref_length(hxc_l_g1_he65afda5393d, &hxc_l_tmp_array_length_result_n166) != HXC_STATUS_OK)
    {
      abort();
    }
    if (!(hxc_l_tmp_load_result_n164 < hxc_l_tmp_array_length_result_n166))
    {
      break;
    }
    struct hxc_array_ref *hxc_l_tmp_load_result_n167 = hxc_l_g1_he65afda5393d;
    hxc_string hxc_l_tmp_array_get_result_n169;
    if (hxc_array_ref_get_copy(hxc_l_tmp_load_result_n167, (size_t)hxc_l_g_he15035e768bf, &hxc_l_tmp_array_get_result_n169) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_array_element_owner_n64 = hxc_l_tmp_array_get_result_n169;
    hxc_l_symbol_h797018683ba1 = hxc_l_tmp_array_element_owner_n64;
    if (hxc_string_retain(hxc_l_symbol_h797018683ba1) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_g_he15035e768bf = hxc_i32_add_wrapping(hxc_l_g_he15035e768bf, 1);
    struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n172 = (*hxc_l_self).hxc_triggerActorPrevious;
    int32_t hxc_l_tmp_array_push_result_n173;
    if (hxc_array_ref_push_copy(hxc_l_tmp_class_field_load_result_n172, &(int32_t){ 0 }, &hxc_l_tmp_array_push_result_n173) != HXC_STATUS_OK)
    {
      abort();
    }
    (void)hxc_l_tmp_array_push_result_n173;
    struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n174 = (*hxc_l_self).hxc_triggerActorPrevious;
    int32_t hxc_l_tmp_array_push_result_n175;
    if (hxc_array_ref_push_copy(hxc_l_tmp_class_field_load_result_n174, &(int32_t){ 0 }, &hxc_l_tmp_array_push_result_n175) != HXC_STATUS_OK)
    {
      abort();
    }
    (void)hxc_l_tmp_array_push_result_n175;
    struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n176 = (*hxc_l_self).hxc_triggerActorPrevious;
    int32_t hxc_l_tmp_array_push_result_n177;
    if (hxc_array_ref_push_copy(hxc_l_tmp_class_field_load_result_n176, &(int32_t){ 0 }, &hxc_l_tmp_array_push_result_n177) != HXC_STATUS_OK)
    {
      abort();
    }
    (void)hxc_l_tmp_array_push_result_n177;
    struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n178 = (*hxc_l_self).hxc_triggerActorPreviousValid;
    int32_t hxc_l_tmp_array_push_result_n179;
    if (hxc_array_ref_push_copy(hxc_l_tmp_class_field_load_result_n178, &(int32_t){ 0 }, &hxc_l_tmp_array_push_result_n179) != HXC_STATUS_OK)
    {
      abort();
    }
    (void)hxc_l_tmp_array_push_result_n179;
    if (hxc_string_release(&hxc_l_symbol_h797018683ba1) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_array_element_owner_n64) != HXC_STATUS_OK)
    {
      abort();
    }
  }
  struct hxc_caxecraft_scenario_CaxeFlowExecutor *hxc_l_tmp_managed_class_result_n180 = NULL;
  if (hxc_gc_allocate(&hxc_program_gc, &hxc_caxecraft_scenario_CaxeFlowExecutor_descriptor, (void **)&hxc_l_tmp_managed_class_result_n180) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_gc_roots[5] = (const void *)hxc_l_tmp_managed_class_result_n180;
  if (!hxc_compiler_constructor_caxecraft_scenario_CaxeFlowExecutor(hxc_l_tmp_managed_class_result_n180, hxc_l_scenario, hxc_l_registry, (struct hxc_optional_caxecraft_scenario_FlowTick){ .hxc_has_value = false }))
  {
    if (hxc_array_ref_release(hxc_l_g1_he65afda5393d) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    abort();
  }
  hxc_l_gc_roots[6] = (const void *)hxc_l_tmp_managed_class_result_n180;
  (*hxc_l_self).hxc_flowExecutor = hxc_l_tmp_managed_class_result_n180;
  struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n181 = (*hxc_l_self).hxc_pendingFlowEvents;
  hxc_string hxc_l_tmp_enum_payload_0_owner_n65 = hxc_l_scenario.hxc_id;
  if (hxc_string_retain(hxc_l_tmp_enum_payload_0_owner_n65) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_tmp_static_call_argument_0_owner_n66 = (struct hxc_caxecraft_scenario_FlowEvent){ .hxc_tag = hxc_caxecraft_scenario_FlowEvent_LevelEntered, .hxc_payload.hxc_LevelEntered.hxc_level = hxc_l_tmp_enum_payload_0_owner_n65 };
  struct hxc_caxecraft_scenario_FlowEvent hxc_l_tmp_static_call_argument_0_borrow_result_n185 = hxc_l_tmp_static_call_argument_0_owner_n66;
  hxc_l_tmp_static_call_argument_1_owner_n67 = (struct hxc_caxecraft_scenario_FlowEventContext){ .hxc_tag = hxc_caxecraft_scenario_FlowEventContext_NoEventContext };
  struct hxc_caxecraft_scenario_FlowEventOccurrence hxc_l_tmp_call_result_n188 = hxc_caxecraft_scenario_CaxeFlowEventRegistry_flowEventOccurrence(hxc_l_tmp_static_call_argument_0_borrow_result_n185, hxc_l_tmp_static_call_argument_1_owner_n67);
  hxc_l_tmp_array_push_element_owner_n68 = hxc_l_tmp_call_result_n188;
  int32_t hxc_l_tmp_array_push_result_n190;
  if (hxc_array_ref_push_copy(hxc_l_tmp_class_field_load_result_n181, &hxc_l_tmp_array_push_element_owner_n68, &hxc_l_tmp_array_push_result_n190) != HXC_STATUS_OK)
  {
    abort();
  }
  (void)hxc_l_tmp_array_push_result_n190;
  hxc_caxecraft_domain_GameSession_refreshStatefulCollision(hxc_l_self);
  hxc_record_7a90fbbd_destroy(&hxc_l_tmp_array_push_element_owner_n68);
  hxc_enum_276ca5e9_destroy(&hxc_l_tmp_static_call_argument_1_owner_n67);
  hxc_enum_10047054_destroy(&hxc_l_tmp_static_call_argument_0_owner_n66);
  if (hxc_array_ref_release(hxc_l_g1_he65afda5393d) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return;
}

bool hxc_caxecraft_domain_GameSession_interactWithActor(struct hxc_caxecraft_domain_GameSession *hxc_l_self, int32_t hxc_l_id)
{
  const void *volatile hxc_l_gc_roots[1] = { NULL };
  hxc_l_gc_roots[0] = (const void *)hxc_l_self;
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
      bool hxc_l_tmp_instance_call_result_n16 = hxc_caxecraft_domain_GameSession_queueLocalActorEvent(hxc_l_self, hxc_l_tmp_instance_call_argument_0_owner_n7);
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

bool hxc_caxecraft_domain_GameSession_interactWithStatefulObject(struct hxc_caxecraft_domain_GameSession *hxc_l_self, hxc_string hxc_l_id)
{
  const void *volatile hxc_l_gc_roots[1] = { NULL };
  hxc_l_gc_roots[0] = (const void *)hxc_l_self;
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 1, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_caxecraft_scenario_FlowEvent hxc_l_tmp_instance_call_argument_0_owner_n4 = { 0 };
  if (hxc_l_self == NULL)
  {
    abort();
  }
  bool hxc_l_tmp_instance_call_result_n0 = hxc_caxecraft_domain_GameSession_statefulObjectInteractionAvailable(hxc_l_self, hxc_l_id);
  bool hxc_l_tmp_short_circuit_result_n2 = hxc_l_tmp_instance_call_result_n0;
  if (hxc_l_tmp_instance_call_result_n0)
  {
    hxc_string hxc_l_tmp_enum_payload_0_owner_n3 = hxc_l_id;
    if (hxc_string_retain(hxc_l_tmp_enum_payload_0_owner_n3) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_instance_call_argument_0_owner_n4 = (struct hxc_caxecraft_scenario_FlowEvent){ .hxc_tag = hxc_caxecraft_scenario_FlowEvent_Interact, .hxc_payload.hxc_Interact.hxc_objectId = hxc_l_tmp_enum_payload_0_owner_n3 };
    bool hxc_l_tmp_instance_call_result_n4 = hxc_caxecraft_domain_GameSession_queueLocalActorEvent(hxc_l_self, hxc_l_tmp_instance_call_argument_0_owner_n4);
    hxc_l_tmp_short_circuit_result_n2 = hxc_l_tmp_instance_call_result_n4;
    hxc_enum_10047054_destroy(&hxc_l_tmp_instance_call_argument_0_owner_n4);
  }
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return hxc_l_tmp_short_circuit_result_n2;
}

struct hxc_array_ref *hxc_caxecraft_domain_GameSession_itemUseOccurrences(struct hxc_caxecraft_domain_GameSession *hxc_l_self, hxc_string hxc_l_itemType)
{
  const void *volatile hxc_l_gc_roots[3] = { NULL, NULL, NULL };
  hxc_l_gc_roots[0] = (const void *)hxc_l_self;
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 3, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_optional_caxecraft_scenario_FlowEventOccurrence hxc_l_occurrence = { 0 };
  struct hxc_caxecraft_scenario_FlowEvent hxc_l_tmp_instance_call_argument_0_owner_n5 = { 0 };
  if (hxc_l_self == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_scenario_CaxeFlowExecutor *hxc_l_tmp_class_field_load_result_n0 = (*hxc_l_self).hxc_flowExecutor;
  hxc_l_gc_roots[1] = (const void *)hxc_l_tmp_class_field_load_result_n0;
  hxc_l_gc_roots[2] = (const void *)NULL;
  if (hxc_l_tmp_class_field_load_result_n0 == NULL)
  {
    struct hxc_array_ref *hxc_l_tmp_array_create_result_n1 = NULL;
    if (hxc_array_ref_create(hxc_default_allocator(), (hxc_array_element_ops){ sizeof(struct hxc_caxecraft_scenario_FlowEventOccurrence), _Alignof(struct hxc_caxecraft_scenario_FlowEventOccurrence), NULL, hxc_array_c6ef4080_element_copy, hxc_array_c6ef4080_element_assign, hxc_array_c6ef4080_element_destroy }, &hxc_l_tmp_array_create_result_n1) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return hxc_l_tmp_array_create_result_n1;
  }
  bool hxc_l_tmp_string_null_equality_result_n2 = hxc_l_itemType.data == NULL;
  if (hxc_l_tmp_string_null_equality_result_n2)
  {
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return NULL;
  }
  hxc_string hxc_l_tmp_enum_payload_0_owner_n4 = hxc_l_itemType;
  if (hxc_string_retain(hxc_l_tmp_enum_payload_0_owner_n4) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_tmp_instance_call_argument_0_owner_n5 = (struct hxc_caxecraft_scenario_FlowEvent){ .hxc_tag = hxc_caxecraft_scenario_FlowEvent_UseItem, .hxc_payload.hxc_UseItem.hxc_itemType = hxc_l_tmp_enum_payload_0_owner_n4 };
  struct hxc_optional_caxecraft_scenario_FlowEventOccurrence hxc_l_tmp_instance_call_result_n6 = hxc_caxecraft_domain_GameSession_localActorOccurrence(hxc_l_self, hxc_l_tmp_instance_call_argument_0_owner_n5);
  hxc_l_occurrence = hxc_l_tmp_instance_call_result_n6;
  if (!!hxc_l_occurrence.hxc_has_value)
  {
    struct hxc_optional_caxecraft_scenario_FlowEventOccurrence hxc_l_tmp_load_result_n8 = hxc_l_occurrence;
    if (!hxc_l_tmp_load_result_n8.hxc_has_value)
    {
      abort();
    }
    struct hxc_array_ref *hxc_l_tmp_array_create_result_n10 = NULL;
    if (hxc_array_ref_create(hxc_default_allocator(), (hxc_array_element_ops){ sizeof(struct hxc_caxecraft_scenario_FlowEventOccurrence), _Alignof(struct hxc_caxecraft_scenario_FlowEventOccurrence), NULL, hxc_array_c6ef4080_element_copy, hxc_array_c6ef4080_element_assign, hxc_array_c6ef4080_element_destroy }, &hxc_l_tmp_array_create_result_n10) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n10->value, &hxc_l_tmp_load_result_n8.hxc_value) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_optional_1cc4d348_destroy(&hxc_l_occurrence);
    hxc_enum_10047054_destroy(&hxc_l_tmp_instance_call_argument_0_owner_n5);
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return hxc_l_tmp_array_create_result_n10;
  }
  hxc_optional_1cc4d348_destroy(&hxc_l_occurrence);
  hxc_enum_10047054_destroy(&hxc_l_tmp_instance_call_argument_0_owner_n5);
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return NULL;
}

struct hxc_optional_caxecraft_scenario_FlowEventOccurrence hxc_caxecraft_domain_GameSession_localActorOccurrence(struct hxc_caxecraft_domain_GameSession *hxc_l_self, struct hxc_caxecraft_scenario_FlowEvent hxc_l_source)
{
  const void *volatile hxc_l_gc_roots[3] = { NULL, NULL, NULL };
  hxc_l_gc_roots[0] = (const void *)hxc_l_self;
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 3, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_string hxc_l_actor = { 0 };
  hxc_string hxc_l_tmp_enum_payload_0_owner_n4 = { 0 };
  struct hxc_caxecraft_scenario_FlowEventContext hxc_l_tmp_static_call_argument_1_owner_n5 = { 0 };
  if (hxc_l_self == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_scenario_CaxeFlowExecutor *hxc_l_tmp_class_field_load_result_n0 = (*hxc_l_self).hxc_flowExecutor;
  hxc_l_gc_roots[1] = (const void *)hxc_l_tmp_class_field_load_result_n0;
  hxc_l_gc_roots[2] = (const void *)NULL;
  if (hxc_l_tmp_class_field_load_result_n0 == NULL)
  {
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return (struct hxc_optional_caxecraft_scenario_FlowEventOccurrence){ .hxc_has_value = false };
  }
  hxc_string hxc_l_tmp_class_field_load_result_n1 = (*hxc_l_self).hxc_localPlayerAuthoredId;
  hxc_l_actor = hxc_l_tmp_class_field_load_result_n1;
  if (hxc_string_retain(hxc_l_actor) != HXC_STATUS_OK)
  {
    abort();
  }
  bool hxc_l_tmp_string_null_equality_result_n3 = hxc_l_actor.data == NULL;
  if (!hxc_l_tmp_string_null_equality_result_n3)
  {
    hxc_l_tmp_enum_payload_0_owner_n4 = hxc_l_actor;
    if (hxc_string_retain(hxc_l_tmp_enum_payload_0_owner_n4) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_static_call_argument_1_owner_n5 = (struct hxc_caxecraft_scenario_FlowEventContext){ .hxc_tag = hxc_caxecraft_scenario_FlowEventContext_ActorEventContext, .hxc_payload.hxc_ActorEventContext.hxc_actor = hxc_l_tmp_enum_payload_0_owner_n4 };
    struct hxc_caxecraft_scenario_FlowEventOccurrence hxc_l_tmp_call_result_n8 = hxc_caxecraft_scenario_CaxeFlowEventRegistry_flowEventOccurrence(hxc_l_source, hxc_l_tmp_static_call_argument_1_owner_n5);
    hxc_enum_276ca5e9_destroy(&hxc_l_tmp_static_call_argument_1_owner_n5);
    if (hxc_string_release(&hxc_l_actor) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return (struct hxc_optional_caxecraft_scenario_FlowEventOccurrence){ .hxc_has_value = true, .hxc_value = hxc_l_tmp_call_result_n8 };
  }
  if (hxc_string_release(&hxc_l_actor) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return (struct hxc_optional_caxecraft_scenario_FlowEventOccurrence){ .hxc_has_value = false };
}

struct hxc_caxecraft_gameplay_MiningResult hxc_caxecraft_domain_GameSession_mineTerrain(struct hxc_caxecraft_domain_GameSession *hxc_l_self, struct hxc_caxecraft_scenario_VoxelPoint hxc_l_coord, struct hxc_caxecraft_gameplay_InventoryState hxc_l_inventory)
{
  const void *volatile hxc_l_gc_roots[1] = { NULL };
  hxc_l_gc_roots[0] = (const void *)hxc_l_self;
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 1, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_tmp_managed_flow_owner_n10 = { 0 };
  struct hxc_array_ref *hxc_l_candidate = { 0 };
  if (hxc_l_self == NULL)
  {
    abort();
  }
  uint8_t *hxc_l_cells = (*hxc_l_self).hxc_base.hxc_worldStorage;
  size_t hxc_l_tmp_length_n3 = 32768;
  enum hxc_caxecraft_domain_BlockKind hxc_l_tmp_call_result_n0 = hxc_caxecraft_domain_World_query(hxc_l_cells, hxc_l_tmp_length_n3, hxc_l_coord);
  struct hxc_optional_caxecraft_gameplay_ItemKind hxc_l_tmp_call_result_n1 = hxc_caxecraft_gameplay_Mining_itemForCollectableBlock(hxc_l_tmp_call_result_n0);
  struct hxc_optional_caxecraft_gameplay_ItemKind hxc_l_item = hxc_l_tmp_call_result_n1;
  struct hxc_array_ref *hxc_l_stagedEvents = { 0 };
  struct hxc_optional_caxecraft_gameplay_ItemKind hxc_l_tmp_load_result_n2 = hxc_l_item;
  bool hxc_l_tmp_short_circuit_result_n6 = hxc_l_tmp_load_result_n2.hxc_has_value;
  if (hxc_l_tmp_load_result_n2.hxc_has_value)
  {
    struct hxc_optional_caxecraft_gameplay_ItemKind hxc_l_tmp_load_result_n3 = hxc_l_item;
    if (!hxc_l_tmp_load_result_n3.hxc_has_value)
    {
      abort();
    }
    int32_t hxc_l_tmp_call_result_n5 = hxc_caxecraft_gameplay_Inventory_acceptedAmount(hxc_l_inventory, hxc_l_tmp_load_result_n3.hxc_value, 1);
    hxc_l_tmp_short_circuit_result_n6 = hxc_l_tmp_call_result_n5 == 1;
  }
  if (hxc_l_tmp_short_circuit_result_n6)
  {
    struct hxc_array_ref *hxc_l_tmp_instance_call_result_n8 = hxc_caxecraft_domain_GameSession_blockChangedOccurrences(hxc_l_self, hxc_l_coord, hxc_caxecraft_domain_BlockKind_Air);
    hxc_l_candidate = hxc_l_tmp_instance_call_result_n8;
    bool hxc_l_tmp_array_reference_equality_result_n8 = hxc_l_candidate == NULL;
    bool hxc_l_tmp_array_reference_equality_result_result_n10 = hxc_l_tmp_array_reference_equality_result_n8;
    bool hxc_l_tmp_short_circuit_result_n9 = hxc_l_tmp_array_reference_equality_result_result_n10;
    if (!hxc_l_tmp_array_reference_equality_result_result_n10)
    {
      bool hxc_l_tmp_instance_call_result_n12 = hxc_caxecraft_domain_GameSession_flowEventsCanCommit(hxc_l_self, hxc_l_candidate);
      hxc_l_tmp_short_circuit_result_n9 = !hxc_l_tmp_instance_call_result_n12;
    }
    if (hxc_l_tmp_short_circuit_result_n9)
    {
      if (hxc_array_ref_release(hxc_l_candidate) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
      {
        abort();
      }
      return (struct hxc_caxecraft_gameplay_MiningResult){ .hxc_inventory = hxc_l_inventory, .hxc_outcome = 3 };
    }
    else
    {
      (void)hxc_l_candidate;
      hxc_l_stagedEvents = hxc_l_candidate;
      if (hxc_array_ref_retain(hxc_l_stagedEvents) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_l_candidate) != HXC_STATUS_OK)
      {
        abort();
      }
    }
  }
  else
  {
    struct hxc_array_ref *hxc_l_tmp_array_create_result_n16 = NULL;
    if (hxc_array_ref_create(hxc_default_allocator(), (hxc_array_element_ops){ sizeof(struct hxc_caxecraft_scenario_FlowEventOccurrence), _Alignof(struct hxc_caxecraft_scenario_FlowEventOccurrence), NULL, hxc_array_c6ef4080_element_copy, hxc_array_c6ef4080_element_assign, hxc_array_c6ef4080_element_destroy }, &hxc_l_tmp_array_create_result_n16) != HXC_STATUS_OK)
    {
      abort();
    }
    (void)hxc_l_tmp_array_create_result_n16;
    hxc_l_stagedEvents = hxc_l_tmp_array_create_result_n16;
  }
  struct hxc_array_ref *hxc_l_tmp_managed_flow_carrier_move_result_n17 = hxc_l_stagedEvents;
  hxc_l_tmp_managed_flow_owner_n10 = hxc_l_tmp_managed_flow_carrier_move_result_n17;
  struct hxc_caxecraft_gameplay_MiningResult hxc_l_tmp_call_result_n18 = hxc_caxecraft_gameplay_Mining_attempt(hxc_l_cells, hxc_l_tmp_length_n3, hxc_l_coord, hxc_l_inventory);
  struct hxc_caxecraft_gameplay_MiningResult hxc_l_result = hxc_l_tmp_call_result_n18;
  if (hxc_l_result.hxc_outcome == 2)
  {
    struct hxc_caxecraft_domain_WaterSimulation *hxc_l_tmp_owned_class_field_address_n20 = &(*hxc_l_self).hxc_water;
    if (hxc_l_tmp_owned_class_field_address_n20 == NULL)
    {
      abort();
    }
    hxc_caxecraft_domain_WaterSimulation_terrainChanged(hxc_l_tmp_owned_class_field_address_n20, hxc_l_coord);
    hxc_caxecraft_domain_GameSession_commitFlowEvents(hxc_l_self, hxc_l_tmp_managed_flow_owner_n10);
  }
  struct hxc_caxecraft_gameplay_MiningResult hxc_l_tmp_load_result_n22 = hxc_l_result;
  if (hxc_array_ref_release(hxc_l_tmp_managed_flow_owner_n10) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return hxc_l_tmp_load_result_n22;
}

bool hxc_caxecraft_domain_GameSession_placeInitialWaterVolume(struct hxc_caxecraft_domain_GameSession *hxc_l_self, struct hxc_caxecraft_scenario_VoxelPoint hxc_l_origin, int32_t hxc_l_width, int32_t hxc_l_height, int32_t hxc_l_depth)
{
  const void *volatile hxc_l_gc_roots[1] = { NULL };
  hxc_l_gc_roots[0] = (const void *)hxc_l_self;
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 1, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_l_self == NULL)
  {
    abort();
  }
  uint8_t *hxc_l_cells = (*hxc_l_self).hxc_base.hxc_worldStorage;
  size_t hxc_l_tmp_length_n5 = 32768;
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

bool hxc_caxecraft_domain_GameSession_placeTerrain(struct hxc_caxecraft_domain_GameSession *hxc_l_self, struct hxc_caxecraft_scenario_VoxelPoint hxc_l_coord, enum hxc_caxecraft_domain_BlockKind hxc_l_kind, hxc_string hxc_l_usedItem)
{
  const void *volatile hxc_l_gc_roots[1] = { NULL };
  hxc_l_gc_roots[0] = (const void *)hxc_l_self;
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 1, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_caxecraft_scenario_FlowEventOccurrence hxc_l_event = { 0 };
  struct hxc_caxecraft_scenario_FlowEventOccurrence hxc_l_tmp_array_element_owner_n13 = { 0 };
  struct hxc_array_ref *hxc_l_stagedEvents = { 0 };
  struct hxc_array_ref *hxc_l_used = { 0 };
  if (hxc_l_self == NULL)
  {
    abort();
  }
  uint8_t *hxc_l_cells = (*hxc_l_self).hxc_base.hxc_worldStorage;
  size_t hxc_l_tmp_length_n4 = 32768;
  struct hxc_array_ref *hxc_l_tmp_instance_call_result_n0 = hxc_caxecraft_domain_GameSession_blockChangedOccurrences(hxc_l_self, hxc_l_coord, hxc_l_kind);
  struct hxc_array_ref *hxc_l_changed = hxc_l_tmp_instance_call_result_n0;
  bool hxc_l_tmp_array_reference_equality_result_n6 = hxc_l_changed == NULL;
  if (hxc_l_tmp_array_reference_equality_result_n6)
  {
    if (hxc_array_ref_release(hxc_l_changed) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return false;
  }
  hxc_l_stagedEvents = hxc_l_changed;
  if (hxc_array_ref_retain(hxc_l_stagedEvents) != HXC_STATUS_OK)
  {
    abort();
  }
  bool hxc_l_tmp_string_null_equality_result_n8 = hxc_l_usedItem.data != NULL;
  if (hxc_l_tmp_string_null_equality_result_n8)
  {
    struct hxc_array_ref *hxc_l_tmp_instance_call_result_n5 = hxc_caxecraft_domain_GameSession_itemUseOccurrences(hxc_l_self, hxc_l_usedItem);
    hxc_l_used = hxc_l_tmp_instance_call_result_n5;
    bool hxc_l_tmp_array_reference_equality_result_n10 = hxc_l_used == NULL;
    if (hxc_l_tmp_array_reference_equality_result_n10)
    {
      if (hxc_array_ref_release(hxc_l_used) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_l_stagedEvents) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_l_changed) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
      {
        abort();
      }
      return false;
    }
    else
    {
      int32_t hxc_l_g = 0;
      while (1)
      {
        int32_t hxc_l_tmp_load_result_n8 = hxc_l_g;
        int32_t hxc_l_tmp_array_length_result_n10;
        if (hxc_array_ref_length(hxc_l_used, &hxc_l_tmp_array_length_result_n10) != HXC_STATUS_OK)
        {
          abort();
        }
        if (!(hxc_l_tmp_load_result_n8 < hxc_l_tmp_array_length_result_n10))
        {
          break;
        }
        struct hxc_array_ref *hxc_l_tmp_load_result_n11 = hxc_l_used;
        struct hxc_caxecraft_scenario_FlowEventOccurrence hxc_l_tmp_array_get_result_n13;
        if (hxc_array_ref_get_copy(hxc_l_tmp_load_result_n11, (size_t)hxc_l_g, &hxc_l_tmp_array_get_result_n13) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_l_tmp_array_element_owner_n13 = hxc_l_tmp_array_get_result_n13;
        hxc_l_event = hxc_l_tmp_array_element_owner_n13;
        if (hxc_record_7a90fbbd_retain(&hxc_l_event) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_l_g = hxc_i32_add_wrapping(hxc_l_g, 1);
        struct hxc_array_ref *hxc_l_tmp_load_result_n16 = hxc_l_stagedEvents;
        int32_t hxc_l_tmp_array_push_result_n18;
        if (hxc_array_ref_push_copy(hxc_l_tmp_load_result_n16, &hxc_l_event, &hxc_l_tmp_array_push_result_n18) != HXC_STATUS_OK)
        {
          abort();
        }
        (void)hxc_l_tmp_array_push_result_n18;
        hxc_record_7a90fbbd_destroy(&hxc_l_event);
        hxc_array_c6ef4080_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n13);
      }
      if (hxc_array_ref_release(hxc_l_used) != HXC_STATUS_OK)
      {
        abort();
      }
    }
  }
  bool hxc_l_tmp_instance_call_result_n20 = hxc_caxecraft_domain_GameSession_flowEventsCanCommit(hxc_l_self, hxc_l_stagedEvents);
  bool hxc_l_tmp_short_circuit_result_n14 = !hxc_l_tmp_instance_call_result_n20;
  if (!!hxc_l_tmp_instance_call_result_n20)
  {
    struct hxc_caxecraft_domain_WaterSimulation *hxc_l_tmp_owned_class_field_address_n21 = &(*hxc_l_self).hxc_water;
    if (hxc_l_tmp_owned_class_field_address_n21 == NULL)
    {
      abort();
    }
    bool hxc_l_tmp_instance_call_result_n22 = hxc_caxecraft_domain_WaterSimulation_placeTerrain(hxc_l_tmp_owned_class_field_address_n21, hxc_l_cells, hxc_l_tmp_length_n4, hxc_l_coord, hxc_l_kind);
    hxc_l_tmp_short_circuit_result_n14 = !hxc_l_tmp_instance_call_result_n22;
  }
  if (!hxc_l_tmp_short_circuit_result_n14)
  {
    hxc_caxecraft_domain_GameSession_commitFlowEvents(hxc_l_self, hxc_l_stagedEvents);
    if (hxc_array_ref_release(hxc_l_stagedEvents) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_changed) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return true;
  }
  if (hxc_array_ref_release(hxc_l_stagedEvents) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_ref_release(hxc_l_changed) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return false;
}

bool hxc_caxecraft_domain_GameSession_placeWaterSource(struct hxc_caxecraft_domain_GameSession *hxc_l_self, struct hxc_caxecraft_scenario_VoxelPoint hxc_l_coord)
{
  const void *volatile hxc_l_gc_roots[1] = { NULL };
  hxc_l_gc_roots[0] = (const void *)hxc_l_self;
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 1, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_l_self == NULL)
  {
    abort();
  }
  uint8_t *hxc_l_cells = (*hxc_l_self).hxc_base.hxc_worldStorage;
  size_t hxc_l_tmp_length_n2 = 32768;
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

struct hxc_caxecraft_domain_PlayerCameraView hxc_caxecraft_domain_GameSession_playerCamera(struct hxc_caxecraft_domain_GameSession *hxc_l_self, enum hxc_caxecraft_domain_PlayerCameraMode hxc_l_mode, struct hxc_caxecraft_domain_CharacterBody hxc_l_body, double hxc_l_lookX, double hxc_l_lookY, double hxc_l_lookZ)
{
  const void *volatile hxc_l_gc_roots[1] = { NULL };
  hxc_l_gc_roots[0] = (const void *)hxc_l_self;
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 1, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_l_self == NULL)
  {
    abort();
  }
  size_t hxc_l_tmp_length_n1;
  const uint8_t *hxc_l_tmp_instance_call_result_n0 = hxc_caxecraft_domain_GameSession_worldView(hxc_l_self, &hxc_l_tmp_length_n1);
  struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n2 = (*hxc_l_self).hxc_activeStatefulCollision;
  struct hxc_caxecraft_domain_PlayerCameraView hxc_l_tmp_call_result_n3 = hxc_caxecraft_domain_PlayerCamera_resolvePlayerCamera(hxc_l_tmp_instance_call_result_n0, hxc_l_tmp_length_n1, hxc_l_tmp_class_field_load_result_n2, hxc_l_mode, hxc_l_body, hxc_l_lookX, hxc_l_lookY, hxc_l_lookZ);
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return hxc_l_tmp_call_result_n3;
}

void hxc_caxecraft_domain_GameSession_queueAuthoredItemCollected(struct hxc_caxecraft_domain_GameSession *hxc_l_self, int32_t hxc_l_index)
{
  const void *volatile hxc_l_gc_roots[3] = { NULL, NULL, NULL };
  hxc_l_gc_roots[0] = (const void *)hxc_l_self;
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 3, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_optional_caxecraft_scenario_FlowEventOccurrence hxc_l_occurrence = { 0 };
  hxc_string hxc_l_tmp_array_element_owner_n3 = { 0 };
  struct hxc_caxecraft_scenario_FlowEvent hxc_l_tmp_instance_call_argument_0_owner_n5 = { 0 };
  struct hxc_array_ref *hxc_l_tmp_instance_call_argument_0_owner_n6 = { 0 };
  if (hxc_l_self == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_scenario_CaxeFlowExecutor *hxc_l_tmp_class_field_load_result_n0 = (*hxc_l_self).hxc_flowExecutor;
  hxc_l_gc_roots[1] = (const void *)hxc_l_tmp_class_field_load_result_n0;
  hxc_l_gc_roots[2] = (const void *)NULL;
  if (hxc_l_tmp_class_field_load_result_n0 != NULL)
  {
    struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n1 = (*hxc_l_self).hxc_authoredItemContentIds;
    hxc_string hxc_l_tmp_array_get_result_n2;
    if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n1, (size_t)hxc_l_index, &hxc_l_tmp_array_get_result_n2) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_array_element_owner_n3 = hxc_l_tmp_array_get_result_n2;
    hxc_string hxc_l_tmp_enum_payload_0_owner_n4 = hxc_l_tmp_array_element_owner_n3;
    if (hxc_string_retain(hxc_l_tmp_enum_payload_0_owner_n4) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_instance_call_argument_0_owner_n5 = (struct hxc_caxecraft_scenario_FlowEvent){ .hxc_tag = hxc_caxecraft_scenario_FlowEvent_ItemCollected, .hxc_payload.hxc_ItemCollected.hxc_itemType = hxc_l_tmp_enum_payload_0_owner_n4 };
    struct hxc_optional_caxecraft_scenario_FlowEventOccurrence hxc_l_tmp_instance_call_result_n7 = hxc_caxecraft_domain_GameSession_localActorOccurrence(hxc_l_self, hxc_l_tmp_instance_call_argument_0_owner_n5);
    hxc_l_occurrence = hxc_l_tmp_instance_call_result_n7;
    if (hxc_l_occurrence.hxc_has_value)
    {
      struct hxc_optional_caxecraft_scenario_FlowEventOccurrence hxc_l_tmp_load_result_n9 = hxc_l_occurrence;
      if (!hxc_l_tmp_load_result_n9.hxc_has_value)
      {
        abort();
      }
      struct hxc_array_ref *hxc_l_tmp_array_create_result_n11 = NULL;
      if (hxc_array_ref_create(hxc_default_allocator(), (hxc_array_element_ops){ sizeof(struct hxc_caxecraft_scenario_FlowEventOccurrence), _Alignof(struct hxc_caxecraft_scenario_FlowEventOccurrence), NULL, hxc_array_c6ef4080_element_copy, hxc_array_c6ef4080_element_assign, hxc_array_c6ef4080_element_destroy }, &hxc_l_tmp_array_create_result_n11) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n11->value, &hxc_l_tmp_load_result_n9.hxc_value) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_l_tmp_instance_call_argument_0_owner_n6 = hxc_l_tmp_array_create_result_n11;
      hxc_caxecraft_domain_GameSession_commitFlowEvents(hxc_l_self, hxc_l_tmp_instance_call_argument_0_owner_n6);
      if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_0_owner_n6) != HXC_STATUS_OK)
      {
        abort();
      }
    }
    hxc_optional_1cc4d348_destroy(&hxc_l_occurrence);
    hxc_enum_10047054_destroy(&hxc_l_tmp_instance_call_argument_0_owner_n5);
    if (hxc_string_release(&hxc_l_tmp_array_element_owner_n3) != HXC_STATUS_OK)
    {
      abort();
    }
  }
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return;
}

bool hxc_caxecraft_domain_GameSession_queueFlowEvent(struct hxc_caxecraft_domain_GameSession *hxc_l_self, struct hxc_caxecraft_scenario_FlowEventOccurrence hxc_l_event)
{
  const void *volatile hxc_l_gc_roots[6] = { NULL, NULL, NULL, NULL, NULL, NULL };
  hxc_l_gc_roots[0] = (const void *)hxc_l_self;
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 6, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_l_self == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_scenario_CaxeFlowExecutor *hxc_l_tmp_class_field_load_result_n0 = (*hxc_l_self).hxc_flowExecutor;
  hxc_l_gc_roots[1] = (const void *)hxc_l_tmp_class_field_load_result_n0;
  struct hxc_caxecraft_scenario_CaxeFlowExecutor *hxc_l_executor = hxc_l_tmp_class_field_load_result_n0;
  struct hxc_caxecraft_scenario_CaxeFlowExecutor *hxc_l_tmp_load_result_n1 = hxc_l_executor;
  hxc_l_gc_roots[2] = (const void *)hxc_l_tmp_load_result_n1;
  hxc_l_gc_roots[3] = (const void *)NULL;
  bool hxc_l_tmp_short_circuit_result_n3 = hxc_l_tmp_load_result_n1 == NULL;
  if (!(hxc_l_tmp_load_result_n1 == NULL))
  {
    struct hxc_caxecraft_scenario_CaxeFlowExecutor *hxc_l_tmp_load_result_n2 = hxc_l_executor;
    hxc_l_gc_roots[4] = (const void *)hxc_l_tmp_load_result_n2;
    if (hxc_l_tmp_load_result_n2 == NULL)
    {
      abort();
    }
    struct hxc_optional_caxecraft_scenario_FlowRuntimeDiagnostic hxc_l_tmp_class_field_load_result_n3 = (*hxc_l_tmp_load_result_n2).hxc_runtimeFault;
    hxc_l_tmp_short_circuit_result_n3 = hxc_l_tmp_class_field_load_result_n3.hxc_has_value;
  }
  bool hxc_l_tmp_short_circuit_load_result_n4 = hxc_l_tmp_short_circuit_result_n3;
  bool hxc_l_tmp_short_circuit_result_n4 = hxc_l_tmp_short_circuit_load_result_n4;
  if (!hxc_l_tmp_short_circuit_load_result_n4)
  {
    struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n5 = (*hxc_l_self).hxc_pendingFlowEvents;
    int32_t hxc_l_tmp_array_length_result_n6;
    if (hxc_array_ref_length(hxc_l_tmp_class_field_load_result_n5, &hxc_l_tmp_array_length_result_n6) != HXC_STATUS_OK)
    {
      abort();
    }
    int32_t hxc_l_tmp_instance_call_result_n7 = hxc_caxecraft_domain_GameSession_externalFlowEventCapacity(hxc_l_self);
    hxc_l_tmp_short_circuit_result_n4 = hxc_l_tmp_array_length_result_n6 >= hxc_l_tmp_instance_call_result_n7;
  }
  bool hxc_l_tmp_short_circuit_load_result_n8 = hxc_l_tmp_short_circuit_result_n4;
  bool hxc_l_tmp_short_circuit_result_n5 = hxc_l_tmp_short_circuit_load_result_n8;
  if (!hxc_l_tmp_short_circuit_load_result_n8)
  {
    struct hxc_caxecraft_scenario_CaxeFlowExecutor *hxc_l_tmp_load_result_n9 = hxc_l_executor;
    hxc_l_gc_roots[5] = (const void *)hxc_l_tmp_load_result_n9;
    if (hxc_l_tmp_load_result_n9 == NULL)
    {
      abort();
    }
    bool hxc_l_tmp_instance_call_result_n10 = hxc_caxecraft_scenario_CaxeFlowExecutor_acceptsRuntimeEvent(hxc_l_tmp_load_result_n9, hxc_l_event);
    hxc_l_tmp_short_circuit_result_n5 = !hxc_l_tmp_instance_call_result_n10;
  }
  if (!hxc_l_tmp_short_circuit_result_n5)
  {
    struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n12 = (*hxc_l_self).hxc_pendingFlowEvents;
    int32_t hxc_l_tmp_array_push_result_n13;
    if (hxc_array_ref_push_copy(hxc_l_tmp_class_field_load_result_n12, &hxc_l_event, &hxc_l_tmp_array_push_result_n13) != HXC_STATUS_OK)
    {
      abort();
    }
    (void)hxc_l_tmp_array_push_result_n13;
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

bool hxc_caxecraft_domain_GameSession_queueLocalActorEvent(struct hxc_caxecraft_domain_GameSession *hxc_l_self, struct hxc_caxecraft_scenario_FlowEvent hxc_l_source)
{
  const void *volatile hxc_l_gc_roots[1] = { NULL };
  hxc_l_gc_roots[0] = (const void *)hxc_l_self;
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 1, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_caxecraft_scenario_FlowEventContext hxc_l_tmp_static_call_argument_1_owner_n6 = { 0 };
  struct hxc_caxecraft_scenario_FlowEventOccurrence hxc_l_tmp_instance_call_argument_0_owner_n7 = { 0 };
  if (hxc_l_self == NULL)
  {
    abort();
  }
  hxc_string hxc_l_tmp_class_field_load_result_n0 = (*hxc_l_self).hxc_localPlayerAuthoredId;
  hxc_string hxc_l_actor = hxc_l_tmp_class_field_load_result_n0;
  if (hxc_string_retain(hxc_l_actor) != HXC_STATUS_OK)
  {
    abort();
  }
  bool hxc_l_tmp_string_null_equality_result_n3 = hxc_l_actor.data != NULL;
  bool hxc_l_tmp_string_null_equality_result_result_n2 = hxc_l_tmp_string_null_equality_result_n3;
  bool hxc_l_tmp_short_circuit_result_n4 = hxc_l_tmp_string_null_equality_result_result_n2;
  if (hxc_l_tmp_string_null_equality_result_result_n2)
  {
    hxc_string hxc_l_tmp_enum_payload_0_owner_n5 = hxc_l_actor;
    if (hxc_string_retain(hxc_l_tmp_enum_payload_0_owner_n5) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_static_call_argument_1_owner_n6 = (struct hxc_caxecraft_scenario_FlowEventContext){ .hxc_tag = hxc_caxecraft_scenario_FlowEventContext_ActorEventContext, .hxc_payload.hxc_ActorEventContext.hxc_actor = hxc_l_tmp_enum_payload_0_owner_n5 };
    struct hxc_caxecraft_scenario_FlowEventOccurrence hxc_l_tmp_call_result_n7 = hxc_caxecraft_scenario_CaxeFlowEventRegistry_flowEventOccurrence(hxc_l_source, hxc_l_tmp_static_call_argument_1_owner_n6);
    hxc_l_tmp_instance_call_argument_0_owner_n7 = hxc_l_tmp_call_result_n7;
    bool hxc_l_tmp_instance_call_result_n9 = hxc_caxecraft_domain_GameSession_queueFlowEvent(hxc_l_self, hxc_l_tmp_instance_call_argument_0_owner_n7);
    hxc_l_tmp_short_circuit_result_n4 = hxc_l_tmp_instance_call_result_n9;
    hxc_record_7a90fbbd_destroy(&hxc_l_tmp_instance_call_argument_0_owner_n7);
    hxc_enum_276ca5e9_destroy(&hxc_l_tmp_static_call_argument_1_owner_n6);
  }
  bool hxc_l_tmp_short_circuit_load_result_n10 = hxc_l_tmp_short_circuit_result_n4;
  if (hxc_string_release(&hxc_l_actor) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return hxc_l_tmp_short_circuit_load_result_n10;
}

void hxc_caxecraft_domain_GameSession_queueTriggerZoneTransitions(struct hxc_caxecraft_domain_GameSession *hxc_l_self)
{
  const void *volatile hxc_l_gc_roots[8] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
  hxc_l_gc_roots[0] = (const void *)hxc_l_self;
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 8, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_string hxc_l_tmp_array_element_owner_n12 = { 0 };
  struct hxc_array_ref *hxc_l_stagedEvents = { 0 };
  struct hxc_array_ref *hxc_l_nextInside = { 0 };
  struct hxc_array_ref *hxc_l_nextPrevious = { 0 };
  struct hxc_array_ref *hxc_l_nextPreviousValid = { 0 };
  hxc_string hxc_l_zone = { 0 };
  hxc_string hxc_l_tmp_array_element_owner_n22 = { 0 };
  struct hxc_array_ref *hxc_l_currentPositions = { 0 };
  struct hxc_caxecraft_scenario_FlowEventContext hxc_l_context = { 0 };
  struct hxc_array_ref *hxc_l_currentValid = { 0 };
  hxc_string hxc_l_tmp_array_element_owner_n40 = { 0 };
  struct hxc_caxecraft_scenario_FlowEvent hxc_l_tmp_static_call_argument_0_owner_n43 = { 0 };
  struct hxc_caxecraft_scenario_FlowEventOccurrence hxc_l_tmp_array_push_element_owner_n44 = { 0 };
  struct hxc_caxecraft_scenario_FlowEvent hxc_l_tmp_static_call_argument_0_owner_n46 = { 0 };
  struct hxc_caxecraft_scenario_FlowEventOccurrence hxc_l_tmp_array_push_element_owner_n47 = { 0 };
  struct hxc_caxecraft_scenario_FlowEvent hxc_l_source = { 0 };
  struct hxc_caxecraft_scenario_FlowEventOccurrence hxc_l_tmp_array_push_element_owner_n52 = { 0 };
  struct hxc_caxecraft_scenario_FlowEventOccurrence hxc_l_event = { 0 };
  struct hxc_caxecraft_scenario_FlowEventOccurrence hxc_l_tmp_array_element_owner_n58 = { 0 };
  if (hxc_l_self == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_scenario_CaxeFlowExecutor *hxc_l_tmp_class_field_load_result_n0 = (*hxc_l_self).hxc_flowExecutor;
  hxc_l_gc_roots[1] = (const void *)hxc_l_tmp_class_field_load_result_n0;
  struct hxc_caxecraft_scenario_CaxeFlowExecutor *hxc_l_executor = hxc_l_tmp_class_field_load_result_n0;
  hxc_l_gc_roots[2] = (const void *)hxc_l_executor;
  hxc_l_gc_roots[3] = (const void *)NULL;
  if (hxc_l_executor == NULL)
  {
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return;
  }
  struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n2 = (*hxc_l_self).hxc_triggerActorIds;
  int32_t hxc_l_tmp_array_length_result_n3;
  if (hxc_array_ref_length(hxc_l_tmp_class_field_load_result_n2, &hxc_l_tmp_array_length_result_n3) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_actorCount = hxc_l_tmp_array_length_result_n3;
  struct hxc_array_ref *hxc_l_tmp_array_create_result_n4 = NULL;
  if (hxc_array_ref_create_trivial(hxc_default_allocator(), sizeof(struct hxc_caxecraft_scenario_FlowEventPosition), _Alignof(struct hxc_caxecraft_scenario_FlowEventPosition), &hxc_l_tmp_array_create_result_n4) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_currentPositions = hxc_l_tmp_array_create_result_n4;
  struct hxc_array_ref *hxc_l_tmp_array_create_result_n5 = NULL;
  if (hxc_array_ref_create_trivial(hxc_default_allocator(), sizeof(int32_t), _Alignof(int32_t), &hxc_l_tmp_array_create_result_n5) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_currentValid = hxc_l_tmp_array_create_result_n5;
  int32_t hxc_l_g_hd0c1d13e7ae2 = 0;
  int32_t hxc_l_g1_h3e07d5c51324 = hxc_l_actorCount;
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n7 = hxc_l_g_hd0c1d13e7ae2;
    if (!(hxc_l_tmp_load_result_n7 < hxc_l_g1_h3e07d5c51324))
    {
      break;
    }
    int32_t hxc_l_tmp_increment_load_result_n9 = hxc_l_g_hd0c1d13e7ae2;
    hxc_l_g_hd0c1d13e7ae2 = hxc_i32_add_wrapping(hxc_l_tmp_increment_load_result_n9, 1);
    int32_t hxc_l_actorIndex_h857846214071 = hxc_l_tmp_increment_load_result_n9;
    struct hxc_caxecraft_domain_EntityStore *hxc_l_tmp_owned_class_field_address_n10 = &(*hxc_l_self).hxc_entities;
    if (hxc_l_tmp_owned_class_field_address_n10 == NULL)
    {
      abort();
    }
    struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n11 = (*hxc_l_self).hxc_triggerActorEntities;
    int32_t hxc_l_tmp_array_get_result_n13;
    if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n11, (size_t)hxc_l_actorIndex_h857846214071, &hxc_l_tmp_array_get_result_n13) != HXC_STATUS_OK)
    {
      abort();
    }
    struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n14 = hxc_caxecraft_domain_EntityStore_read(hxc_l_tmp_owned_class_field_address_n10, hxc_l_tmp_array_get_result_n13);
    struct hxc_caxecraft_domain_Character hxc_l_character = hxc_l_tmp_instance_call_result_n14;
    struct hxc_array_ref *hxc_l_tmp_array_push_receiver_n9 = hxc_l_currentValid;
    int32_t hxc_l_tmp_record_field_load_result_n16 = hxc_l_character.hxc_id;
    bool hxc_l_tmp_short_circuit_result_n10 = hxc_l_tmp_record_field_load_result_n16 > 0;
    if (hxc_l_tmp_record_field_load_result_n16 > 0)
    {
      hxc_l_tmp_short_circuit_result_n10 = hxc_l_character.hxc_vitals.hxc_health > 0;
    }
    bool hxc_l_tmp_short_circuit_load_result_n18 = hxc_l_tmp_short_circuit_result_n10;
    bool hxc_l_tmp_short_circuit_result_n11 = hxc_l_tmp_short_circuit_load_result_n18;
    if (hxc_l_tmp_short_circuit_load_result_n18)
    {
      struct hxc_caxecraft_scenario_CaxeFlowExecutor *hxc_l_tmp_load_result_n19 = hxc_l_executor;
      hxc_l_gc_roots[4] = (const void *)hxc_l_tmp_load_result_n19;
      if (hxc_l_tmp_load_result_n19 == NULL)
      {
        abort();
      }
      struct hxc_caxecraft_scenario_CaxeFlowState *hxc_l_tmp_class_field_load_result_n20 = (*hxc_l_tmp_load_result_n19).hxc_state;
      hxc_l_gc_roots[5] = (const void *)hxc_l_tmp_class_field_load_result_n20;
      if (hxc_l_tmp_class_field_load_result_n20 == NULL)
      {
        abort();
      }
      struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n21 = (*hxc_l_self).hxc_triggerActorIds;
      hxc_string hxc_l_tmp_array_get_result_n23;
      if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n21, (size_t)hxc_l_actorIndex_h857846214071, &hxc_l_tmp_array_get_result_n23) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_l_tmp_array_element_owner_n12 = hxc_l_tmp_array_get_result_n23;
      bool hxc_l_tmp_instance_call_result_n25 = hxc_caxecraft_scenario_CaxeFlowState_objectActive(hxc_l_tmp_class_field_load_result_n20, hxc_l_tmp_array_element_owner_n12);
      hxc_l_tmp_short_circuit_result_n11 = hxc_l_tmp_instance_call_result_n25;
      if (hxc_string_release(&hxc_l_tmp_array_element_owner_n12) != HXC_STATUS_OK)
      {
        abort();
      }
    }
    bool hxc_l_tmp_short_circuit_load_result_n26 = hxc_l_tmp_short_circuit_result_n11;
    int32_t hxc_l_tmp_conditional_result_n13 = 0;
    if (hxc_l_tmp_short_circuit_load_result_n26)
    {
      hxc_l_tmp_conditional_result_n13 = 1;
    }
    else
    {
      hxc_l_tmp_conditional_result_n13 = 0;
    }
    int32_t hxc_l_tmp_conditional_load_result_n27 = hxc_l_tmp_conditional_result_n13;
    int32_t hxc_l_tmp_array_push_result_n29;
    if (hxc_array_ref_push_copy(hxc_l_tmp_array_push_receiver_n9, &(int32_t){ hxc_l_tmp_conditional_load_result_n27 }, &hxc_l_tmp_array_push_result_n29) != HXC_STATUS_OK)
    {
      abort();
    }
    (void)hxc_l_tmp_array_push_result_n29;
    struct hxc_array_ref *hxc_l_tmp_load_result_n30 = hxc_l_currentPositions;
    struct hxc_caxecraft_scenario_FlowEventPosition hxc_l_tmp_call_result_n32 = hxc_caxecraft_domain_GameSession_flowEventPosition(hxc_l_character);
    int32_t hxc_l_tmp_array_push_result_n33;
    if (hxc_array_ref_push_copy(hxc_l_tmp_load_result_n30, &hxc_l_tmp_call_result_n32, &hxc_l_tmp_array_push_result_n33) != HXC_STATUS_OK)
    {
      abort();
    }
    (void)hxc_l_tmp_array_push_result_n33;
  }
  struct hxc_array_ref *hxc_l_tmp_array_create_result_n34 = NULL;
  if (hxc_array_ref_create(hxc_default_allocator(), (hxc_array_element_ops){ sizeof(struct hxc_caxecraft_scenario_FlowEventOccurrence), _Alignof(struct hxc_caxecraft_scenario_FlowEventOccurrence), NULL, hxc_array_c6ef4080_element_copy, hxc_array_c6ef4080_element_assign, hxc_array_c6ef4080_element_destroy }, &hxc_l_tmp_array_create_result_n34) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_stagedEvents = hxc_l_tmp_array_create_result_n34;
  struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n35 = (*hxc_l_self).hxc_triggerZoneInside;
  struct hxc_array_ref *hxc_l_tmp_array_copy_result_n36 = NULL;
  if (hxc_array_ref_copy(hxc_l_tmp_class_field_load_result_n35, &hxc_l_tmp_array_copy_result_n36) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_nextInside = hxc_l_tmp_array_copy_result_n36;
  struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n37 = (*hxc_l_self).hxc_triggerActorPrevious;
  struct hxc_array_ref *hxc_l_tmp_array_copy_result_n38 = NULL;
  if (hxc_array_ref_copy(hxc_l_tmp_class_field_load_result_n37, &hxc_l_tmp_array_copy_result_n38) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_nextPrevious = hxc_l_tmp_array_copy_result_n38;
  struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n39 = (*hxc_l_self).hxc_triggerActorPreviousValid;
  struct hxc_array_ref *hxc_l_tmp_array_copy_result_n40 = NULL;
  if (hxc_array_ref_copy(hxc_l_tmp_class_field_load_result_n39, &hxc_l_tmp_array_copy_result_n40) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_nextPreviousValid = hxc_l_tmp_array_copy_result_n40;
  int32_t hxc_l_g_he23e0ddc7d42 = 0;
  struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n41 = (*hxc_l_self).hxc_triggerZoneIds;
  int32_t hxc_l_tmp_array_length_result_n42;
  if (hxc_array_ref_length(hxc_l_tmp_class_field_load_result_n41, &hxc_l_tmp_array_length_result_n42) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_g1_ha331c0ae8216 = hxc_l_tmp_array_length_result_n42;
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n43 = hxc_l_g_he23e0ddc7d42;
    if (!(hxc_l_tmp_load_result_n43 < hxc_l_g1_ha331c0ae8216))
    {
      break;
    }
    int32_t hxc_l_tmp_increment_load_result_n45 = hxc_l_g_he23e0ddc7d42;
    hxc_l_g_he23e0ddc7d42 = hxc_i32_add_wrapping(hxc_l_tmp_increment_load_result_n45, 1);
    int32_t hxc_l_zoneIndex = hxc_l_tmp_increment_load_result_n45;
    struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n46 = (*hxc_l_self).hxc_triggerZoneIds;
    hxc_string hxc_l_tmp_array_get_result_n48;
    if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n46, (size_t)hxc_l_zoneIndex, &hxc_l_tmp_array_get_result_n48) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_array_element_owner_n22 = hxc_l_tmp_array_get_result_n48;
    hxc_l_zone = hxc_l_tmp_array_element_owner_n22;
    if (hxc_string_retain(hxc_l_zone) != HXC_STATUS_OK)
    {
      abort();
    }
    struct hxc_caxecraft_scenario_CaxeFlowExecutor *hxc_l_tmp_load_result_n50 = hxc_l_executor;
    hxc_l_gc_roots[6] = (const void *)hxc_l_tmp_load_result_n50;
    if (hxc_l_tmp_load_result_n50 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_scenario_CaxeFlowState *hxc_l_tmp_class_field_load_result_n51 = (*hxc_l_tmp_load_result_n50).hxc_state;
    hxc_l_gc_roots[7] = (const void *)hxc_l_tmp_class_field_load_result_n51;
    if (hxc_l_tmp_class_field_load_result_n51 == NULL)
    {
      abort();
    }
    bool hxc_l_tmp_instance_call_result_n53 = hxc_caxecraft_scenario_CaxeFlowState_objectActive(hxc_l_tmp_class_field_load_result_n51, hxc_l_zone);
    bool hxc_l_active = hxc_l_tmp_instance_call_result_n53;
    int32_t hxc_l_g_ha2787a6afcaa = 0;
    int32_t hxc_l_g1_hde0a3e42d9e6 = hxc_l_actorCount;
    while (1)
    {
      int32_t hxc_l_tmp_load_result_n55 = hxc_l_g_ha2787a6afcaa;
      if (!(hxc_l_tmp_load_result_n55 < hxc_l_g1_hde0a3e42d9e6))
      {
        break;
      }
      int32_t hxc_l_tmp_increment_load_result_n57 = hxc_l_g_ha2787a6afcaa;
      hxc_l_g_ha2787a6afcaa = hxc_i32_add_wrapping(hxc_l_tmp_increment_load_result_n57, 1);
      int32_t hxc_l_actorIndex_hdce0ac62cdc4 = hxc_l_tmp_increment_load_result_n57;
      int32_t hxc_l_tmp_load_result_n58 = hxc_l_zoneIndex;
      int32_t hxc_l_tmp_load_result_n59 = hxc_l_actorCount;
      int32_t hxc_l_membershipIndex = hxc_i32_add_wrapping(hxc_i32_multiply_wrapping(hxc_l_tmp_load_result_n58, hxc_l_tmp_load_result_n59), hxc_l_actorIndex_hdce0ac62cdc4);
      struct hxc_array_ref *hxc_l_tmp_load_result_n61 = hxc_l_currentValid;
      int32_t hxc_l_tmp_array_get_result_n63;
      if (hxc_array_ref_get_copy(hxc_l_tmp_load_result_n61, (size_t)hxc_l_actorIndex_hdce0ac62cdc4, &hxc_l_tmp_array_get_result_n63) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_l_tmp_array_get_result_n63 == 0)
      {
        struct hxc_array_ref *hxc_l_tmp_load_result_n64 = hxc_l_nextInside;
        if (hxc_array_ref_set_grow_copy(hxc_l_tmp_load_result_n64, hxc_l_membershipIndex, &(int32_t){ 0 }, &(int32_t){ 0 }) != HXC_STATUS_OK)
        {
          abort();
        }
        (void)0;
        struct hxc_array_ref *hxc_l_tmp_load_result_n67 = hxc_l_nextPreviousValid;
        if (hxc_array_ref_set_grow_copy(hxc_l_tmp_load_result_n67, hxc_l_actorIndex_hdce0ac62cdc4, &(int32_t){ 0 }, &(int32_t){ 0 }) != HXC_STATUS_OK)
        {
          abort();
        }
        (void)0;
        continue;
      }
      struct hxc_array_ref *hxc_l_tmp_load_result_n70 = hxc_l_currentPositions;
      struct hxc_caxecraft_scenario_FlowEventPosition hxc_l_tmp_array_get_result_n72;
      if (hxc_array_ref_get_copy(hxc_l_tmp_load_result_n70, (size_t)hxc_l_actorIndex_hdce0ac62cdc4, &hxc_l_tmp_array_get_result_n72) != HXC_STATUS_OK)
      {
        abort();
      }
      struct hxc_caxecraft_scenario_FlowEventPosition hxc_l_current = hxc_l_tmp_array_get_result_n72;
      if (!hxc_l_active)
      {
        struct hxc_array_ref *hxc_l_tmp_load_result_n74 = hxc_l_nextInside;
        if (hxc_array_ref_set_grow_copy(hxc_l_tmp_load_result_n74, hxc_l_membershipIndex, &(int32_t){ 0 }, &(int32_t){ 0 }) != HXC_STATUS_OK)
        {
          abort();
        }
        (void)0;
        struct hxc_array_ref *hxc_l_tmp_load_result_n77 = hxc_l_nextPrevious;
        int32_t hxc_l_tmp_load_result_n78 = hxc_l_actorIndex_hdce0ac62cdc4;
        hxc_caxecraft_domain_GameSession_writeTriggerActorPosition(hxc_l_tmp_load_result_n77, hxc_l_tmp_load_result_n78, hxc_l_current);
        struct hxc_array_ref *hxc_l_tmp_load_result_n80 = hxc_l_nextPreviousValid;
        if (hxc_array_ref_set_grow_copy(hxc_l_tmp_load_result_n80, hxc_l_actorIndex_hdce0ac62cdc4, &(int32_t){ 1 }, &(int32_t){ 0 }) != HXC_STATUS_OK)
        {
          abort();
        }
        (void)1;
        continue;
      }
      int32_t hxc_l_tmp_load_result_n83 = hxc_l_zoneIndex;
      bool hxc_l_tmp_instance_call_result_n85 = hxc_caxecraft_domain_GameSession_triggerZoneContains(hxc_l_self, hxc_l_tmp_load_result_n83, hxc_l_current);
      bool hxc_l_inside = hxc_l_tmp_instance_call_result_n85;
      struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n86 = (*hxc_l_self).hxc_triggerZoneInside;
      int32_t hxc_l_tmp_array_get_result_n88;
      if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n86, (size_t)hxc_l_membershipIndex, &hxc_l_tmp_array_get_result_n88) != HXC_STATUS_OK)
      {
        abort();
      }
      bool hxc_l_wasInside = hxc_l_tmp_array_get_result_n88 != 0;
      struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n89 = (*hxc_l_self).hxc_triggerActorPreviousValid;
      int32_t hxc_l_tmp_array_get_result_n91;
      if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n89, (size_t)hxc_l_actorIndex_hdce0ac62cdc4, &hxc_l_tmp_array_get_result_n91) != HXC_STATUS_OK)
      {
        abort();
      }
      bool hxc_l_hadPrevious = hxc_l_tmp_array_get_result_n91 != 0;
      bool hxc_l_tmp_load_result_n92 = hxc_l_hadPrevious;
      struct hxc_caxecraft_scenario_FlowEventPosition hxc_l_tmp_conditional_result_n33 = { 0 };
      if (hxc_l_tmp_load_result_n92)
      {
        struct hxc_caxecraft_scenario_FlowEventPosition hxc_l_tmp_instance_call_result_n94 = hxc_caxecraft_domain_GameSession_readTriggerActorPosition(hxc_l_self, hxc_l_actorIndex_hdce0ac62cdc4);
        hxc_l_tmp_conditional_result_n33 = hxc_l_tmp_instance_call_result_n94;
      }
      else
      {
        hxc_l_tmp_conditional_result_n33 = hxc_l_current;
      }
      struct hxc_caxecraft_scenario_FlowEventPosition hxc_l_previous = hxc_l_tmp_conditional_result_n33;
      int32_t hxc_l_eventCount = 0;
      bool hxc_l_swept = false;
      bool hxc_l_tmp_load_result_n97 = hxc_l_inside;
      if (hxc_l_tmp_load_result_n97 != hxc_l_wasInside)
      {
        hxc_l_eventCount = 1;
      }
      else
      {
        bool hxc_l_tmp_load_result_n99 = hxc_l_hadPrevious;
        bool hxc_l_tmp_short_circuit_result_n36 = hxc_l_tmp_load_result_n99;
        if (hxc_l_tmp_load_result_n99)
        {
          hxc_l_tmp_short_circuit_result_n36 = !hxc_l_inside;
        }
        bool hxc_l_tmp_short_circuit_load_result_n101 = hxc_l_tmp_short_circuit_result_n36;
        bool hxc_l_tmp_short_circuit_result_n37 = hxc_l_tmp_short_circuit_load_result_n101;
        if (hxc_l_tmp_short_circuit_load_result_n101)
        {
          hxc_l_tmp_short_circuit_result_n37 = !hxc_l_wasInside;
        }
        bool hxc_l_tmp_short_circuit_load_result_n103 = hxc_l_tmp_short_circuit_result_n37;
        bool hxc_l_tmp_short_circuit_result_n38 = hxc_l_tmp_short_circuit_load_result_n103;
        if (hxc_l_tmp_short_circuit_load_result_n103)
        {
          int32_t hxc_l_tmp_load_result_n104 = hxc_l_zoneIndex;
          struct hxc_caxecraft_scenario_FlowEventPosition hxc_l_tmp_load_result_n105 = hxc_l_previous;
          bool hxc_l_tmp_instance_call_result_n107 = hxc_caxecraft_domain_GameSession_triggerZoneSegmentCrosses(hxc_l_self, hxc_l_tmp_load_result_n104, hxc_l_tmp_load_result_n105, hxc_l_current);
          hxc_l_tmp_short_circuit_result_n38 = hxc_l_tmp_instance_call_result_n107;
        }
        if (hxc_l_tmp_short_circuit_result_n38)
        {
          hxc_l_eventCount = 2;
          hxc_l_swept = true;
        }
      }
      struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n109 = (*hxc_l_self).hxc_triggerActorIds;
      hxc_string hxc_l_tmp_array_get_result_n111;
      if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n109, (size_t)hxc_l_actorIndex_hdce0ac62cdc4, &hxc_l_tmp_array_get_result_n111) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_l_tmp_array_element_owner_n40 = hxc_l_tmp_array_get_result_n111;
      hxc_string hxc_l_tmp_enum_payload_0_owner_n41 = hxc_l_tmp_array_element_owner_n40;
      if (hxc_string_retain(hxc_l_tmp_enum_payload_0_owner_n41) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_string hxc_l_tmp_enum_payload_0_owned_load_result_n113 = hxc_l_tmp_enum_payload_0_owner_n41;
      struct hxc_caxecraft_scenario_FlowEventPosition hxc_l_tmp_load_result_n114 = hxc_l_previous;
      struct hxc_caxecraft_scenario_FlowEventPosition hxc_l_tmp_load_result_n115 = hxc_l_current;
      hxc_l_context = (struct hxc_caxecraft_scenario_FlowEventContext){ .hxc_tag = hxc_caxecraft_scenario_FlowEventContext_SpatialEventContext, .hxc_payload.hxc_SpatialEventContext.hxc_actor = hxc_l_tmp_enum_payload_0_owned_load_result_n113, .hxc_payload.hxc_SpatialEventContext.hxc_previous = hxc_l_tmp_load_result_n114, .hxc_payload.hxc_SpatialEventContext.hxc_current = hxc_l_tmp_load_result_n115, .hxc_payload.hxc_SpatialEventContext.hxc_swept = hxc_l_swept };
      if (hxc_l_eventCount == 2)
      {
        struct hxc_array_ref *hxc_l_tmp_load_result_n119 = hxc_l_stagedEvents;
        hxc_string hxc_l_tmp_enum_payload_0_owner_n42 = hxc_l_zone;
        if (hxc_string_retain(hxc_l_tmp_enum_payload_0_owner_n42) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_l_tmp_static_call_argument_0_owner_n43 = (struct hxc_caxecraft_scenario_FlowEvent){ .hxc_tag = hxc_caxecraft_scenario_FlowEvent_EnterZone, .hxc_payload.hxc_EnterZone.hxc_zone = hxc_l_tmp_enum_payload_0_owner_n42 };
        struct hxc_caxecraft_scenario_FlowEvent hxc_l_tmp_static_call_argument_0_borrow_result_n123 = hxc_l_tmp_static_call_argument_0_owner_n43;
        struct hxc_caxecraft_scenario_FlowEventOccurrence hxc_l_tmp_call_result_n125 = hxc_caxecraft_scenario_CaxeFlowEventRegistry_flowEventOccurrence(hxc_l_tmp_static_call_argument_0_borrow_result_n123, hxc_l_context);
        hxc_l_tmp_array_push_element_owner_n44 = hxc_l_tmp_call_result_n125;
        int32_t hxc_l_tmp_array_push_result_n127;
        if (hxc_array_ref_push_copy(hxc_l_tmp_load_result_n119, &hxc_l_tmp_array_push_element_owner_n44, &hxc_l_tmp_array_push_result_n127) != HXC_STATUS_OK)
        {
          abort();
        }
        (void)hxc_l_tmp_array_push_result_n127;
        struct hxc_array_ref *hxc_l_tmp_load_result_n128 = hxc_l_stagedEvents;
        hxc_string hxc_l_tmp_enum_payload_0_owner_n45 = hxc_l_zone;
        if (hxc_string_retain(hxc_l_tmp_enum_payload_0_owner_n45) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_l_tmp_static_call_argument_0_owner_n46 = (struct hxc_caxecraft_scenario_FlowEvent){ .hxc_tag = hxc_caxecraft_scenario_FlowEvent_LeaveZone, .hxc_payload.hxc_LeaveZone.hxc_zone = hxc_l_tmp_enum_payload_0_owner_n45 };
        struct hxc_caxecraft_scenario_FlowEvent hxc_l_tmp_static_call_argument_0_borrow_result_n132 = hxc_l_tmp_static_call_argument_0_owner_n46;
        struct hxc_caxecraft_scenario_FlowEventOccurrence hxc_l_tmp_call_result_n134 = hxc_caxecraft_scenario_CaxeFlowEventRegistry_flowEventOccurrence(hxc_l_tmp_static_call_argument_0_borrow_result_n132, hxc_l_context);
        hxc_l_tmp_array_push_element_owner_n47 = hxc_l_tmp_call_result_n134;
        int32_t hxc_l_tmp_array_push_result_n136;
        if (hxc_array_ref_push_copy(hxc_l_tmp_load_result_n128, &hxc_l_tmp_array_push_element_owner_n47, &hxc_l_tmp_array_push_result_n136) != HXC_STATUS_OK)
        {
          abort();
        }
        (void)hxc_l_tmp_array_push_result_n136;
        hxc_record_7a90fbbd_destroy(&hxc_l_tmp_array_push_element_owner_n47);
        hxc_enum_10047054_destroy(&hxc_l_tmp_static_call_argument_0_owner_n46);
        hxc_record_7a90fbbd_destroy(&hxc_l_tmp_array_push_element_owner_n44);
        hxc_enum_10047054_destroy(&hxc_l_tmp_static_call_argument_0_owner_n43);
      }
      else
      {
        if (hxc_l_eventCount == 1)
        {
          bool hxc_l_tmp_load_result_n138 = hxc_l_inside;
          struct hxc_caxecraft_scenario_FlowEvent hxc_l_tmp_conditional_managed_result_n49 = { 0 };
          if (hxc_l_tmp_load_result_n138)
          {
            hxc_string hxc_l_tmp_enum_payload_0_owner_n50 = hxc_l_zone;
            if (hxc_string_retain(hxc_l_tmp_enum_payload_0_owner_n50) != HXC_STATUS_OK)
            {
              abort();
            }
            (void)(struct hxc_caxecraft_scenario_FlowEvent){ .hxc_tag = hxc_caxecraft_scenario_FlowEvent_EnterZone, .hxc_payload.hxc_EnterZone.hxc_zone = hxc_l_tmp_enum_payload_0_owner_n50 };
            hxc_l_tmp_conditional_managed_result_n49 = (struct hxc_caxecraft_scenario_FlowEvent){ .hxc_tag = hxc_caxecraft_scenario_FlowEvent_EnterZone, .hxc_payload.hxc_EnterZone.hxc_zone = hxc_l_tmp_enum_payload_0_owner_n50 };
          }
          else
          {
            hxc_string hxc_l_tmp_enum_payload_0_owner_n51 = hxc_l_zone;
            if (hxc_string_retain(hxc_l_tmp_enum_payload_0_owner_n51) != HXC_STATUS_OK)
            {
              abort();
            }
            (void)(struct hxc_caxecraft_scenario_FlowEvent){ .hxc_tag = hxc_caxecraft_scenario_FlowEvent_LeaveZone, .hxc_payload.hxc_LeaveZone.hxc_zone = hxc_l_tmp_enum_payload_0_owner_n51 };
            hxc_l_tmp_conditional_managed_result_n49 = (struct hxc_caxecraft_scenario_FlowEvent){ .hxc_tag = hxc_caxecraft_scenario_FlowEvent_LeaveZone, .hxc_payload.hxc_LeaveZone.hxc_zone = hxc_l_tmp_enum_payload_0_owner_n51 };
          }
          struct hxc_caxecraft_scenario_FlowEvent hxc_l_tmp_conditional_managed_move_result_n145 = hxc_l_tmp_conditional_managed_result_n49;
          hxc_l_source = hxc_l_tmp_conditional_managed_move_result_n145;
          struct hxc_array_ref *hxc_l_tmp_load_result_n146 = hxc_l_stagedEvents;
          struct hxc_caxecraft_scenario_FlowEvent hxc_l_tmp_load_result_n147 = hxc_l_source;
          struct hxc_caxecraft_scenario_FlowEventOccurrence hxc_l_tmp_call_result_n149 = hxc_caxecraft_scenario_CaxeFlowEventRegistry_flowEventOccurrence(hxc_l_tmp_load_result_n147, hxc_l_context);
          hxc_l_tmp_array_push_element_owner_n52 = hxc_l_tmp_call_result_n149;
          int32_t hxc_l_tmp_array_push_result_n151;
          if (hxc_array_ref_push_copy(hxc_l_tmp_load_result_n146, &hxc_l_tmp_array_push_element_owner_n52, &hxc_l_tmp_array_push_result_n151) != HXC_STATUS_OK)
          {
            abort();
          }
          (void)hxc_l_tmp_array_push_result_n151;
          hxc_record_7a90fbbd_destroy(&hxc_l_tmp_array_push_element_owner_n52);
          hxc_enum_10047054_destroy(&hxc_l_source);
        }
      }
      struct hxc_array_ref *hxc_l_tmp_load_result_n152 = hxc_l_nextInside;
      int32_t hxc_l_tmp_load_result_n153 = hxc_l_membershipIndex;
      struct hxc_array_ref *hxc_l_tmp_array_set_value_receiver_n53 = hxc_l_tmp_load_result_n152;
      int32_t hxc_l_tmp_array_set_value_index_n54 = hxc_l_tmp_load_result_n153;
      bool hxc_l_tmp_load_result_n154 = hxc_l_inside;
      int32_t hxc_l_tmp_conditional_result_n55 = 0;
      if (hxc_l_tmp_load_result_n154)
      {
        hxc_l_tmp_conditional_result_n55 = 1;
      }
      else
      {
        hxc_l_tmp_conditional_result_n55 = 0;
      }
      int32_t hxc_l_tmp_conditional_load_result_n155 = hxc_l_tmp_conditional_result_n55;
      struct hxc_array_ref *hxc_l_tmp_array_set_value_receiver_load_result_n156 = hxc_l_tmp_array_set_value_receiver_n53;
      if (hxc_array_ref_set_grow_copy(hxc_l_tmp_array_set_value_receiver_load_result_n156, hxc_l_tmp_array_set_value_index_n54, &(int32_t){ hxc_l_tmp_conditional_load_result_n155 }, &(int32_t){ 0 }) != HXC_STATUS_OK)
      {
        abort();
      }
      (void)hxc_l_tmp_conditional_load_result_n155;
      struct hxc_array_ref *hxc_l_tmp_load_result_n159 = hxc_l_nextPrevious;
      int32_t hxc_l_tmp_load_result_n160 = hxc_l_actorIndex_hdce0ac62cdc4;
      hxc_caxecraft_domain_GameSession_writeTriggerActorPosition(hxc_l_tmp_load_result_n159, hxc_l_tmp_load_result_n160, hxc_l_current);
      struct hxc_array_ref *hxc_l_tmp_load_result_n162 = hxc_l_nextPreviousValid;
      if (hxc_array_ref_set_grow_copy(hxc_l_tmp_load_result_n162, hxc_l_actorIndex_hdce0ac62cdc4, &(int32_t){ 1 }, &(int32_t){ 0 }) != HXC_STATUS_OK)
      {
        abort();
      }
      (void)1;
      hxc_enum_276ca5e9_destroy(&hxc_l_context);
      if (hxc_string_release(&hxc_l_tmp_array_element_owner_n40) != HXC_STATUS_OK)
      {
        abort();
      }
    }
    if (hxc_string_release(&hxc_l_zone) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_array_element_owner_n22) != HXC_STATUS_OK)
    {
      abort();
    }
  }
  struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n165 = (*hxc_l_self).hxc_pendingFlowEvents;
  int32_t hxc_l_tmp_array_length_result_n166;
  if (hxc_array_ref_length(hxc_l_tmp_class_field_load_result_n165, &hxc_l_tmp_array_length_result_n166) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_tmp_array_length_result_n168;
  if (hxc_array_ref_length(hxc_l_stagedEvents, &hxc_l_tmp_array_length_result_n168) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_i32_add_wrapping(hxc_l_tmp_array_length_result_n166, hxc_l_tmp_array_length_result_n168) > 256)
  {
    if (hxc_array_ref_release(hxc_l_nextPreviousValid) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_nextPrevious) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_nextInside) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_stagedEvents) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_currentValid) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_currentPositions) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return;
  }
  int32_t hxc_l_g_ha9517fdeb5e0 = 0;
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n169 = hxc_l_g_ha9517fdeb5e0;
    int32_t hxc_l_tmp_array_length_result_n171;
    if (hxc_array_ref_length(hxc_l_stagedEvents, &hxc_l_tmp_array_length_result_n171) != HXC_STATUS_OK)
    {
      abort();
    }
    if (!(hxc_l_tmp_load_result_n169 < hxc_l_tmp_array_length_result_n171))
    {
      break;
    }
    struct hxc_array_ref *hxc_l_tmp_load_result_n172 = hxc_l_stagedEvents;
    struct hxc_caxecraft_scenario_FlowEventOccurrence hxc_l_tmp_array_get_result_n174;
    if (hxc_array_ref_get_copy(hxc_l_tmp_load_result_n172, (size_t)hxc_l_g_ha9517fdeb5e0, &hxc_l_tmp_array_get_result_n174) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_array_element_owner_n58 = hxc_l_tmp_array_get_result_n174;
    hxc_l_event = hxc_l_tmp_array_element_owner_n58;
    if (hxc_record_7a90fbbd_retain(&hxc_l_event) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_g_ha9517fdeb5e0 = hxc_i32_add_wrapping(hxc_l_g_ha9517fdeb5e0, 1);
    struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n177 = (*hxc_l_self).hxc_pendingFlowEvents;
    int32_t hxc_l_tmp_array_push_result_n179;
    if (hxc_array_ref_push_copy(hxc_l_tmp_class_field_load_result_n177, &hxc_l_event, &hxc_l_tmp_array_push_result_n179) != HXC_STATUS_OK)
    {
      abort();
    }
    (void)hxc_l_tmp_array_push_result_n179;
    hxc_record_7a90fbbd_destroy(&hxc_l_event);
    hxc_array_c6ef4080_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n58);
  }
  struct hxc_array_ref *hxc_l_tmp_array_field_assignment_replacement_owner_n59 = hxc_l_nextInside;
  if (hxc_array_ref_retain(hxc_l_tmp_array_field_assignment_replacement_owner_n59) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_tmp_array_field_assignment_replacement_owned_load_result_n181 = hxc_l_tmp_array_field_assignment_replacement_owner_n59;
  if (hxc_array_ref_release((*hxc_l_self).hxc_triggerZoneInside) != HXC_STATUS_OK)
  {
    abort();
  }
  (*hxc_l_self).hxc_triggerZoneInside = hxc_l_tmp_array_field_assignment_replacement_owned_load_result_n181;
  struct hxc_array_ref *hxc_l_tmp_array_field_assignment_replacement_owner_n60 = hxc_l_nextPrevious;
  if (hxc_array_ref_retain(hxc_l_tmp_array_field_assignment_replacement_owner_n60) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_tmp_array_field_assignment_replacement_owned_load_result_n183 = hxc_l_tmp_array_field_assignment_replacement_owner_n60;
  if (hxc_array_ref_release((*hxc_l_self).hxc_triggerActorPrevious) != HXC_STATUS_OK)
  {
    abort();
  }
  (*hxc_l_self).hxc_triggerActorPrevious = hxc_l_tmp_array_field_assignment_replacement_owned_load_result_n183;
  struct hxc_array_ref *hxc_l_tmp_array_field_assignment_replacement_owner_n61 = hxc_l_nextPreviousValid;
  if (hxc_array_ref_retain(hxc_l_tmp_array_field_assignment_replacement_owner_n61) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_tmp_array_field_assignment_replacement_owned_load_result_n185 = hxc_l_tmp_array_field_assignment_replacement_owner_n61;
  if (hxc_array_ref_release((*hxc_l_self).hxc_triggerActorPreviousValid) != HXC_STATUS_OK)
  {
    abort();
  }
  (*hxc_l_self).hxc_triggerActorPreviousValid = hxc_l_tmp_array_field_assignment_replacement_owned_load_result_n185;
  if (hxc_array_ref_release(hxc_l_nextPreviousValid) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_ref_release(hxc_l_nextPrevious) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_ref_release(hxc_l_nextInside) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_ref_release(hxc_l_stagedEvents) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_ref_release(hxc_l_currentValid) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_ref_release(hxc_l_currentPositions) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return;
}

struct hxc_caxecraft_domain_Character hxc_caxecraft_domain_GameSession_readCharacter(struct hxc_caxecraft_domain_GameSession *hxc_l_self, int32_t hxc_l_id)
{
  const void *volatile hxc_l_gc_roots[1] = { NULL };
  hxc_l_gc_roots[0] = (const void *)hxc_l_self;
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

struct hxc_caxecraft_scenario_FlowEventPosition hxc_caxecraft_domain_GameSession_readTriggerActorPosition(struct hxc_caxecraft_domain_GameSession *hxc_l_self, int32_t hxc_l_actorIndex)
{
  const void *volatile hxc_l_gc_roots[1] = { NULL };
  hxc_l_gc_roots[0] = (const void *)hxc_l_self;
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 1, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_offset = hxc_i32_multiply_wrapping(hxc_l_actorIndex, 3);
  if (hxc_l_self == NULL)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n0 = (*hxc_l_self).hxc_triggerActorPrevious;
  int32_t hxc_l_tmp_array_get_result_n2;
  if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n0, (size_t)hxc_l_offset, &hxc_l_tmp_array_get_result_n2) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n3 = (*hxc_l_self).hxc_triggerActorPrevious;
  int32_t hxc_l_tmp_array_get_result_n5;
  if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n3, (size_t)hxc_i32_add_wrapping(hxc_l_offset, 1), &hxc_l_tmp_array_get_result_n5) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n6 = (*hxc_l_self).hxc_triggerActorPrevious;
  int32_t hxc_l_tmp_array_get_result_n8;
  if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n6, (size_t)hxc_i32_add_wrapping(hxc_l_offset, 2), &hxc_l_tmp_array_get_result_n8) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return (struct hxc_caxecraft_scenario_FlowEventPosition){ .hxc_xMilli = hxc_l_tmp_array_get_result_n2, .hxc_yMilli = hxc_l_tmp_array_get_result_n5, .hxc_zMilli = hxc_l_tmp_array_get_result_n8 };
}

struct hxc_caxecraft_domain_LocalCharacterCommandResult hxc_caxecraft_domain_GameSession_receiveLocalPlayerAttack(struct hxc_caxecraft_domain_GameSession *hxc_l_self)
{
  const void *volatile hxc_l_gc_roots[1] = { NULL };
  hxc_l_gc_roots[0] = (const void *)hxc_l_self;
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

void hxc_caxecraft_domain_GameSession_refreshLocalPlayerCollision(struct hxc_caxecraft_domain_GameSession *hxc_l_self)
{
  const void *volatile hxc_l_gc_roots[1] = { NULL };
  hxc_l_gc_roots[0] = (const void *)hxc_l_self;
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 1, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_g1_h8dd7c23dfefa = { 0 };
  struct hxc_caxecraft_domain_ActorControllerState hxc_l_controller = { 0 };
  struct hxc_caxecraft_domain_ActorControllerState hxc_l_tmp_array_element_owner_n7 = { 0 };
  if (hxc_l_self == NULL)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n0 = (*hxc_l_self).hxc_activeLocalPlayerCollision;
  if (hxc_l_tmp_class_field_load_result_n0 == NULL)
  {
    abort();
  }
  if (hxc_array_resize(&hxc_l_tmp_class_field_load_result_n0->value, (size_t)0, NULL) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_g_hfa8ab3ab6238 = 0;
  struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n1 = (*hxc_l_self).hxc_activeStatefulCollision;
  struct hxc_array_ref *hxc_l_g1_hc947e65e96d7 = hxc_l_tmp_class_field_load_result_n1;
  if (hxc_array_ref_retain(hxc_l_g1_hc947e65e96d7) != HXC_STATUS_OK)
  {
    abort();
  }
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n2 = hxc_l_g_hfa8ab3ab6238;
    int32_t hxc_l_tmp_array_length_result_n4;
    if (hxc_array_ref_length(hxc_l_g1_hc947e65e96d7, &hxc_l_tmp_array_length_result_n4) != HXC_STATUS_OK)
    {
      abort();
    }
    if (!(hxc_l_tmp_load_result_n2 < hxc_l_tmp_array_length_result_n4))
    {
      break;
    }
    struct hxc_array_ref *hxc_l_tmp_load_result_n5 = hxc_l_g1_hc947e65e96d7;
    struct hxc_caxecraft_domain_DynamicCollisionBox hxc_l_tmp_array_get_result_n7;
    if (hxc_array_ref_get_copy(hxc_l_tmp_load_result_n5, (size_t)hxc_l_g_hfa8ab3ab6238, &hxc_l_tmp_array_get_result_n7) != HXC_STATUS_OK)
    {
      abort();
    }
    struct hxc_caxecraft_domain_DynamicCollisionBox hxc_l_box = hxc_l_tmp_array_get_result_n7;
    hxc_l_g_hfa8ab3ab6238 = hxc_i32_add_wrapping(hxc_l_g_hfa8ab3ab6238, 1);
    struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n9 = (*hxc_l_self).hxc_activeLocalPlayerCollision;
    int32_t hxc_l_tmp_array_push_result_n11;
    if (hxc_array_ref_push_copy(hxc_l_tmp_class_field_load_result_n9, &hxc_l_box, &hxc_l_tmp_array_push_result_n11) != HXC_STATUS_OK)
    {
      abort();
    }
    (void)hxc_l_tmp_array_push_result_n11;
  }
  int32_t hxc_l_g_h3e05eb322fd7 = 0;
  struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n12 = (*hxc_l_self).hxc_actorControllers;
  hxc_l_g1_h8dd7c23dfefa = hxc_l_tmp_class_field_load_result_n12;
  if (hxc_array_ref_retain(hxc_l_g1_h8dd7c23dfefa) != HXC_STATUS_OK)
  {
    abort();
  }
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n13 = hxc_l_g_h3e05eb322fd7;
    int32_t hxc_l_tmp_array_length_result_n15;
    if (hxc_array_ref_length(hxc_l_g1_h8dd7c23dfefa, &hxc_l_tmp_array_length_result_n15) != HXC_STATUS_OK)
    {
      abort();
    }
    if (!(hxc_l_tmp_load_result_n13 < hxc_l_tmp_array_length_result_n15))
    {
      break;
    }
    struct hxc_array_ref *hxc_l_tmp_load_result_n16 = hxc_l_g1_h8dd7c23dfefa;
    struct hxc_caxecraft_domain_ActorControllerState hxc_l_tmp_array_get_result_n18;
    if (hxc_array_ref_get_copy(hxc_l_tmp_load_result_n16, (size_t)hxc_l_g_h3e05eb322fd7, &hxc_l_tmp_array_get_result_n18) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_array_element_owner_n7 = hxc_l_tmp_array_get_result_n18;
    hxc_l_controller = hxc_l_tmp_array_element_owner_n7;
    if (hxc_record_50dff1c7_retain(&hxc_l_controller) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_g_h3e05eb322fd7 = hxc_i32_add_wrapping(hxc_l_g_h3e05eb322fd7, 1);
    switch (hxc_l_controller.hxc_profile.hxc_tag) {
      case hxc_caxecraft_domain_ActorControllerProfile_StationaryDialogue:
        {
          struct hxc_caxecraft_domain_EntityStore *hxc_l_tmp_owned_class_field_address_n22 = &(*hxc_l_self).hxc_entities;
          if (hxc_l_tmp_owned_class_field_address_n22 == NULL)
          {
            abort();
          }
          struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n24 = hxc_caxecraft_domain_EntityStore_read(hxc_l_tmp_owned_class_field_address_n22, hxc_l_controller.hxc_characterId);
          struct hxc_caxecraft_domain_Character hxc_l_actor = hxc_l_tmp_instance_call_result_n24;
          if (hxc_l_actor.hxc_id > 0)
          {
            struct hxc_caxecraft_domain_DynamicCollisionBox hxc_l_tmp_call_result_n27 = hxc_caxecraft_domain_CharacterPhysics_collisionBox(hxc_l_actor.hxc_body);
            struct hxc_caxecraft_domain_DynamicCollisionBox hxc_l_occupied = hxc_l_tmp_call_result_n27;
            double hxc_l_personalSpace = 0.6;
            struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n28 = (*hxc_l_self).hxc_activeLocalPlayerCollision;
            double hxc_l_tmp_record_field_load_result_n29 = hxc_l_occupied.hxc_minimumX;
            double hxc_l_tmp_load_result_n30 = hxc_l_personalSpace;
            double hxc_l_tmp_record_field_load_result_n31 = hxc_l_occupied.hxc_maximumX;
            double hxc_l_tmp_load_result_n32 = hxc_l_personalSpace;
            double hxc_l_tmp_record_field_load_result_n33 = hxc_l_occupied.hxc_minimumY;
            double hxc_l_tmp_record_field_load_result_n34 = hxc_l_occupied.hxc_maximumY;
            double hxc_l_tmp_record_field_load_result_n35 = hxc_l_occupied.hxc_minimumZ;
            double hxc_l_tmp_load_result_n36 = hxc_l_personalSpace;
            double hxc_l_tmp_record_field_load_result_n37 = hxc_l_occupied.hxc_maximumZ;
            int32_t hxc_l_tmp_array_push_result_n40;
            if (hxc_array_ref_push_copy(hxc_l_tmp_class_field_load_result_n28, &(struct hxc_caxecraft_domain_DynamicCollisionBox){ .hxc_maximumX = hxc_l_tmp_record_field_load_result_n31 + hxc_l_tmp_load_result_n32, .hxc_maximumY = hxc_l_tmp_record_field_load_result_n34, .hxc_maximumZ = hxc_l_tmp_record_field_load_result_n37 + hxc_l_personalSpace, .hxc_minimumX = hxc_l_tmp_record_field_load_result_n29 - hxc_l_tmp_load_result_n30, .hxc_minimumY = hxc_l_tmp_record_field_load_result_n33, .hxc_minimumZ = hxc_l_tmp_record_field_load_result_n35 - hxc_l_tmp_load_result_n36 }, &hxc_l_tmp_array_push_result_n40) != HXC_STATUS_OK)
            {
              abort();
            }
            (void)hxc_l_tmp_array_push_result_n40;
          }
          break;
        }
      case hxc_caxecraft_domain_ActorControllerProfile_WanderChaseMelee:
        {
          break;
        }
      case hxc_caxecraft_domain_ActorControllerProfile_TelegraphedCharge:
        {
          break;
        }
      default:
        {
          abort();
        }
    }
    hxc_record_50dff1c7_destroy(&hxc_l_controller);
    hxc_array_3767ae6c_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n7);
  }
  if (hxc_array_ref_release(hxc_l_g1_h8dd7c23dfefa) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_ref_release(hxc_l_g1_hc947e65e96d7) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return;
}

void hxc_caxecraft_domain_GameSession_refreshStatefulCollision(struct hxc_caxecraft_domain_GameSession *hxc_l_self)
{
  const void *volatile hxc_l_gc_roots[8] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
  hxc_l_gc_roots[0] = (const void *)hxc_l_self;
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 8, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_string hxc_l_tmp_array_element_owner_n13 = { 0 };
  hxc_string hxc_l_objectId = { 0 };
  hxc_string hxc_l_tmp_array_element_owner_n6 = { 0 };
  hxc_string hxc_l_currentState = { 0 };
  if (hxc_l_self == NULL)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n0 = (*hxc_l_self).hxc_activeStatefulCollision;
  if (hxc_l_tmp_class_field_load_result_n0 == NULL)
  {
    abort();
  }
  if (hxc_array_resize(&hxc_l_tmp_class_field_load_result_n0->value, (size_t)0, NULL) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_caxecraft_scenario_CaxeFlowExecutor *hxc_l_tmp_class_field_load_result_n1 = (*hxc_l_self).hxc_flowExecutor;
  hxc_l_gc_roots[1] = (const void *)hxc_l_tmp_class_field_load_result_n1;
  struct hxc_caxecraft_scenario_CaxeFlowExecutor *hxc_l_executor = hxc_l_tmp_class_field_load_result_n1;
  hxc_l_gc_roots[2] = (const void *)hxc_l_executor;
  hxc_l_gc_roots[3] = (const void *)NULL;
  if (hxc_l_executor != NULL)
  {
    int32_t hxc_l_g = 0;
    struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n3 = (*hxc_l_self).hxc_statefulObjectIds;
    int32_t hxc_l_tmp_array_length_result_n4;
    if (hxc_array_ref_length(hxc_l_tmp_class_field_load_result_n3, &hxc_l_tmp_array_length_result_n4) != HXC_STATUS_OK)
    {
      abort();
    }
    int32_t hxc_l_g1 = hxc_l_tmp_array_length_result_n4;
    while (1)
    {
      int32_t hxc_l_tmp_load_result_n5 = hxc_l_g;
      if (!(hxc_l_tmp_load_result_n5 < hxc_l_g1))
      {
        break;
      }
      int32_t hxc_l_tmp_increment_load_result_n7 = hxc_l_g;
      hxc_l_g = hxc_i32_add_wrapping(hxc_l_tmp_increment_load_result_n7, 1);
      int32_t hxc_l_objectIndex = hxc_l_tmp_increment_load_result_n7;
      struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n8 = (*hxc_l_self).hxc_statefulObjectIds;
      hxc_string hxc_l_tmp_array_get_result_n10;
      if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n8, (size_t)hxc_l_objectIndex, &hxc_l_tmp_array_get_result_n10) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_l_tmp_array_element_owner_n6 = hxc_l_tmp_array_get_result_n10;
      hxc_l_objectId = hxc_l_tmp_array_element_owner_n6;
      if (hxc_string_retain(hxc_l_objectId) != HXC_STATUS_OK)
      {
        abort();
      }
      struct hxc_caxecraft_scenario_CaxeFlowExecutor *hxc_l_tmp_load_result_n12 = hxc_l_executor;
      hxc_l_gc_roots[4] = (const void *)hxc_l_tmp_load_result_n12;
      if (hxc_l_tmp_load_result_n12 == NULL)
      {
        abort();
      }
      struct hxc_caxecraft_scenario_CaxeFlowState *hxc_l_tmp_class_field_load_result_n13 = (*hxc_l_tmp_load_result_n12).hxc_state;
      hxc_l_gc_roots[5] = (const void *)hxc_l_tmp_class_field_load_result_n13;
      if (hxc_l_tmp_class_field_load_result_n13 == NULL)
      {
        abort();
      }
      bool hxc_l_tmp_instance_call_result_n15 = hxc_caxecraft_scenario_CaxeFlowState_objectActive(hxc_l_tmp_class_field_load_result_n13, hxc_l_objectId);
      if (!hxc_l_tmp_instance_call_result_n15)
      {
        if (hxc_string_release(&hxc_l_objectId) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_string_release(&hxc_l_tmp_array_element_owner_n6) != HXC_STATUS_OK)
        {
          abort();
        }
        continue;
      }
      struct hxc_caxecraft_scenario_CaxeFlowExecutor *hxc_l_tmp_load_result_n16 = hxc_l_executor;
      hxc_l_gc_roots[6] = (const void *)hxc_l_tmp_load_result_n16;
      if (hxc_l_tmp_load_result_n16 == NULL)
      {
        abort();
      }
      struct hxc_caxecraft_scenario_CaxeFlowState *hxc_l_tmp_class_field_load_result_n17 = (*hxc_l_tmp_load_result_n16).hxc_state;
      hxc_l_gc_roots[7] = (const void *)hxc_l_tmp_class_field_load_result_n17;
      if (hxc_l_tmp_class_field_load_result_n17 == NULL)
      {
        abort();
      }
      hxc_string hxc_l_tmp_instance_call_result_n19 = hxc_caxecraft_scenario_CaxeFlowState_objectState(hxc_l_tmp_class_field_load_result_n17, hxc_l_objectId);
      hxc_l_currentState = hxc_l_tmp_instance_call_result_n19;
      bool hxc_l_tmp_string_null_equality_result_n8 = hxc_l_currentState.data == NULL;
      if (hxc_l_tmp_string_null_equality_result_n8)
      {
        if (hxc_string_release(&hxc_l_currentState) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_string_release(&hxc_l_objectId) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_string_release(&hxc_l_tmp_array_element_owner_n6) != HXC_STATUS_OK)
        {
          abort();
        }
        continue;
      }
      struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n22 = (*hxc_l_self).hxc_statefulObjectStateStarts;
      int32_t hxc_l_tmp_array_get_result_n24;
      if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n22, (size_t)hxc_l_objectIndex, &hxc_l_tmp_array_get_result_n24) != HXC_STATUS_OK)
      {
        abort();
      }
      int32_t hxc_l_start = hxc_l_tmp_array_get_result_n24;
      int32_t hxc_l_tmp_load_result_n25 = hxc_l_start;
      struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n26 = (*hxc_l_self).hxc_statefulObjectStateCounts;
      int32_t hxc_l_tmp_array_get_result_n28;
      if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n26, (size_t)hxc_l_objectIndex, &hxc_l_tmp_array_get_result_n28) != HXC_STATUS_OK)
      {
        abort();
      }
      int32_t hxc_l_end = hxc_i32_add_wrapping(hxc_l_tmp_load_result_n25, hxc_l_tmp_array_get_result_n28);
      int32_t hxc_l_stateIndex = hxc_l_start;
      bool hxc_l_solid = false;
      while (1)
      {
        int32_t hxc_l_tmp_load_result_n30 = hxc_l_stateIndex;
        if (!(hxc_l_tmp_load_result_n30 < hxc_l_end))
        {
          break;
        }
        struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n32 = (*hxc_l_self).hxc_statefulObjectCollisionStates;
        hxc_string hxc_l_tmp_array_get_result_n34;
        if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n32, (size_t)hxc_l_stateIndex, &hxc_l_tmp_array_get_result_n34) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_l_tmp_array_element_owner_n13 = hxc_l_tmp_array_get_result_n34;
        hxc_string hxc_l_tmp_array_element_borrow_result_n35 = hxc_l_tmp_array_element_owner_n13;
        bool hxc_l_tmp_string_equality_result_n14 = hxc_l_tmp_array_element_borrow_result_n35.data == NULL || hxc_l_currentState.data == NULL ? hxc_l_tmp_array_element_borrow_result_n35.data == hxc_l_currentState.data : hxc_l_tmp_array_element_borrow_result_n35.byte_length == hxc_l_currentState.byte_length && (hxc_l_tmp_array_element_borrow_result_n35.byte_length == 0 || memcmp(hxc_l_tmp_array_element_borrow_result_n35.data, hxc_l_currentState.data, hxc_l_tmp_array_element_borrow_result_n35.byte_length) == 0);
        bool hxc_l_tmp_string_equality_result_result_n38 = hxc_l_tmp_string_equality_result_n14;
        if (hxc_string_release(&hxc_l_tmp_array_element_owner_n13) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_l_tmp_string_equality_result_result_n38)
        {
          struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n39 = (*hxc_l_self).hxc_statefulObjectCollisionSolid;
          int32_t hxc_l_tmp_array_get_result_n41;
          if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n39, (size_t)hxc_l_stateIndex, &hxc_l_tmp_array_get_result_n41) != HXC_STATUS_OK)
          {
            abort();
          }
          hxc_l_solid = hxc_l_tmp_array_get_result_n41 != 0;
          hxc_l_stateIndex = hxc_l_end;
        }
        else
        {
          hxc_l_stateIndex = hxc_i32_add_wrapping(hxc_l_stateIndex, 1);
        }
      }
      if (!hxc_l_solid)
      {
        if (hxc_string_release(&hxc_l_currentState) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_string_release(&hxc_l_objectId) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_string_release(&hxc_l_tmp_array_element_owner_n6) != HXC_STATUS_OK)
        {
          abort();
        }
        continue;
      }
      int32_t hxc_l_positionOffset = hxc_i32_multiply_wrapping(hxc_l_objectIndex, 3);
      int32_t hxc_l_boundsOffset = hxc_i32_multiply_wrapping(hxc_l_objectIndex, 4);
      struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n47 = (*hxc_l_self).hxc_statefulObjectBoundsMilli;
      int32_t hxc_l_tmp_array_get_result_n49;
      if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n47, (size_t)hxc_l_boundsOffset, &hxc_l_tmp_array_get_result_n49) != HXC_STATUS_OK)
      {
        abort();
      }
      int32_t hxc_l_widthMilli = hxc_l_tmp_array_get_result_n49;
      struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n50 = (*hxc_l_self).hxc_statefulObjectBoundsMilli;
      int32_t hxc_l_tmp_array_get_result_n52;
      if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n50, (size_t)hxc_i32_add_wrapping(hxc_l_boundsOffset, 1), &hxc_l_tmp_array_get_result_n52) != HXC_STATUS_OK)
      {
        abort();
      }
      int32_t hxc_l_heightMilli = hxc_l_tmp_array_get_result_n52;
      struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n53 = (*hxc_l_self).hxc_statefulObjectBoundsMilli;
      int32_t hxc_l_tmp_array_get_result_n55;
      if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n53, (size_t)hxc_i32_add_wrapping(hxc_l_boundsOffset, 2), &hxc_l_tmp_array_get_result_n55) != HXC_STATUS_OK)
      {
        abort();
      }
      int32_t hxc_l_depthMilli = hxc_l_tmp_array_get_result_n55;
      struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n56 = (*hxc_l_self).hxc_statefulObjectBoundsMilli;
      int32_t hxc_l_tmp_array_get_result_n58;
      if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n56, (size_t)hxc_i32_add_wrapping(hxc_l_boundsOffset, 3), &hxc_l_tmp_array_get_result_n58) != HXC_STATUS_OK)
      {
        abort();
      }
      int32_t hxc_l_yaw = hxc_l_tmp_array_get_result_n58;
      int32_t hxc_l_tmp_load_result_n59 = hxc_l_yaw;
      bool hxc_l_tmp_short_circuit_result_n21 = hxc_l_tmp_load_result_n59 == 90;
      if (!(hxc_l_tmp_load_result_n59 == 90))
      {
        hxc_l_tmp_short_circuit_result_n21 = hxc_l_yaw == 270;
      }
      if (hxc_l_tmp_short_circuit_result_n21)
      {
        int32_t hxc_l_swap = hxc_l_widthMilli;
        hxc_l_widthMilli = hxc_l_depthMilli;
        hxc_l_depthMilli = hxc_l_swap;
      }
      struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n65 = (*hxc_l_self).hxc_statefulObjectPositionsMilli;
      int32_t hxc_l_tmp_array_get_result_n67;
      if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n65, (size_t)hxc_l_positionOffset, &hxc_l_tmp_array_get_result_n67) != HXC_STATUS_OK)
      {
        abort();
      }
      double hxc_l_centerX = hxc_f64_divide_zero_safe((double)hxc_l_tmp_array_get_result_n67, 1000.0);
      struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n68 = (*hxc_l_self).hxc_statefulObjectPositionsMilli;
      int32_t hxc_l_tmp_array_get_result_n70;
      if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n68, (size_t)hxc_i32_add_wrapping(hxc_l_positionOffset, 1), &hxc_l_tmp_array_get_result_n70) != HXC_STATUS_OK)
      {
        abort();
      }
      double hxc_l_minimumY = hxc_f64_divide_zero_safe((double)hxc_l_tmp_array_get_result_n70, 1000.0);
      struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n71 = (*hxc_l_self).hxc_statefulObjectPositionsMilli;
      int32_t hxc_l_tmp_array_get_result_n73;
      if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n71, (size_t)hxc_i32_add_wrapping(hxc_l_positionOffset, 2), &hxc_l_tmp_array_get_result_n73) != HXC_STATUS_OK)
      {
        abort();
      }
      double hxc_l_centerZ = hxc_f64_divide_zero_safe((double)hxc_l_tmp_array_get_result_n73, 1000.0);
      double hxc_l_halfWidth = hxc_f64_divide_zero_safe((double)hxc_l_widthMilli, 2000.0);
      double hxc_l_halfDepth = hxc_f64_divide_zero_safe((double)hxc_l_depthMilli, 2000.0);
      struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n76 = (*hxc_l_self).hxc_activeStatefulCollision;
      double hxc_l_tmp_load_result_n77 = hxc_l_centerX;
      double hxc_l_tmp_load_result_n78 = hxc_l_halfWidth;
      double hxc_l_tmp_load_result_n79 = hxc_l_centerX;
      double hxc_l_tmp_load_result_n80 = hxc_l_halfWidth;
      double hxc_l_tmp_load_result_n81 = hxc_l_minimumY;
      double hxc_l_tmp_load_result_n82 = hxc_l_minimumY;
      int32_t hxc_l_tmp_load_result_n83 = hxc_l_heightMilli;
      double hxc_l_tmp_load_result_n84 = hxc_l_centerZ;
      double hxc_l_tmp_load_result_n85 = hxc_l_halfDepth;
      double hxc_l_tmp_load_result_n86 = hxc_l_centerZ;
      int32_t hxc_l_tmp_array_push_result_n89;
      if (hxc_array_ref_push_copy(hxc_l_tmp_class_field_load_result_n76, &(struct hxc_caxecraft_domain_DynamicCollisionBox){ .hxc_maximumX = hxc_l_tmp_load_result_n79 + hxc_l_tmp_load_result_n80, .hxc_maximumY = hxc_l_tmp_load_result_n82 + hxc_f64_divide_zero_safe((double)hxc_l_tmp_load_result_n83, 1000.0), .hxc_maximumZ = hxc_l_tmp_load_result_n86 + hxc_l_halfDepth, .hxc_minimumX = hxc_l_tmp_load_result_n77 - hxc_l_tmp_load_result_n78, .hxc_minimumY = hxc_l_tmp_load_result_n81, .hxc_minimumZ = hxc_l_tmp_load_result_n84 - hxc_l_tmp_load_result_n85 }, &hxc_l_tmp_array_push_result_n89) != HXC_STATUS_OK)
      {
        abort();
      }
      (void)hxc_l_tmp_array_push_result_n89;
      if (hxc_string_release(&hxc_l_currentState) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_objectId) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_array_element_owner_n6) != HXC_STATUS_OK)
      {
        abort();
      }
    }
  }
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return;
}

struct hxc_caxecraft_domain_CharacterDamageResult hxc_caxecraft_domain_GameSession_rejectedCharacterDamage(struct hxc_caxecraft_domain_GameSession *hxc_l_self, struct hxc_caxecraft_domain_Character hxc_l_original)
{
  const void *volatile hxc_l_gc_roots[1] = { NULL };
  hxc_l_gc_roots[0] = (const void *)hxc_l_self;
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 1, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return (struct hxc_caxecraft_domain_CharacterDamageResult){ .hxc_character = hxc_l_original, .hxc_damageApplied = 0, .hxc_defeated = false, .hxc_resolved = false };
}

bool hxc_caxecraft_domain_GameSession_removeTerrain(struct hxc_caxecraft_domain_GameSession *hxc_l_self, struct hxc_caxecraft_scenario_VoxelPoint hxc_l_coord)
{
  const void *volatile hxc_l_gc_roots[1] = { NULL };
  hxc_l_gc_roots[0] = (const void *)hxc_l_self;
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 1, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_l_self == NULL)
  {
    abort();
  }
  uint8_t *hxc_l_cells = (*hxc_l_self).hxc_base.hxc_worldStorage;
  size_t hxc_l_tmp_length_n2 = 32768;
  struct hxc_array_ref *hxc_l_tmp_instance_call_result_n1 = hxc_caxecraft_domain_GameSession_blockChangedOccurrences(hxc_l_self, hxc_l_coord, hxc_caxecraft_domain_BlockKind_Air);
  struct hxc_array_ref *hxc_l_stagedEvents = hxc_l_tmp_instance_call_result_n1;
  bool hxc_l_tmp_array_reference_equality_result_n4 = hxc_l_stagedEvents == NULL;
  bool hxc_l_tmp_array_reference_equality_result_result_n3 = hxc_l_tmp_array_reference_equality_result_n4;
  bool hxc_l_tmp_short_circuit_result_n5 = hxc_l_tmp_array_reference_equality_result_result_n3;
  if (!hxc_l_tmp_array_reference_equality_result_result_n3)
  {
    bool hxc_l_tmp_instance_call_result_n5 = hxc_caxecraft_domain_GameSession_flowEventsCanCommit(hxc_l_self, hxc_l_stagedEvents);
    hxc_l_tmp_short_circuit_result_n5 = !hxc_l_tmp_instance_call_result_n5;
  }
  bool hxc_l_tmp_short_circuit_load_result_n6 = hxc_l_tmp_short_circuit_result_n5;
  bool hxc_l_tmp_short_circuit_result_n6 = hxc_l_tmp_short_circuit_load_result_n6;
  if (!hxc_l_tmp_short_circuit_load_result_n6)
  {
    bool hxc_l_tmp_call_result_n7 = hxc_caxecraft_domain_World_remove(hxc_l_cells, hxc_l_tmp_length_n2, hxc_l_coord);
    hxc_l_tmp_short_circuit_result_n6 = !hxc_l_tmp_call_result_n7;
  }
  if (!hxc_l_tmp_short_circuit_result_n6)
  {
    struct hxc_caxecraft_domain_WaterSimulation *hxc_l_tmp_owned_class_field_address_n9 = &(*hxc_l_self).hxc_water;
    if (hxc_l_tmp_owned_class_field_address_n9 == NULL)
    {
      abort();
    }
    hxc_caxecraft_domain_WaterSimulation_terrainChanged(hxc_l_tmp_owned_class_field_address_n9, hxc_l_coord);
    hxc_caxecraft_domain_GameSession_commitFlowEvents(hxc_l_self, hxc_l_stagedEvents);
    if (hxc_array_ref_release(hxc_l_stagedEvents) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return true;
  }
  if (hxc_array_ref_release(hxc_l_stagedEvents) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return false;
}

bool hxc_caxecraft_domain_GameSession_replaceAuthoredActors(struct hxc_caxecraft_domain_GameSession *hxc_l_self, struct hxc_array_ref *hxc_l_characters, struct hxc_array_ref *hxc_l_controllers)
{
  const void *volatile hxc_l_gc_roots[1] = { NULL };
  hxc_l_gc_roots[0] = (const void *)hxc_l_self;
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
  const void *volatile hxc_l_gc_roots[1] = { NULL };
  hxc_l_gc_roots[0] = (const void *)hxc_l_self;
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

bool hxc_caxecraft_domain_GameSession_statefulObjectInteractionAvailable(struct hxc_caxecraft_domain_GameSession *hxc_l_self, hxc_string hxc_l_id)
{
  const void *volatile hxc_l_gc_roots[6] = { NULL, NULL, NULL, NULL, NULL, NULL };
  hxc_l_gc_roots[0] = (const void *)hxc_l_self;
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 6, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_string hxc_l_tmp_array_element_owner_n8 = { 0 };
  if (hxc_l_self == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_scenario_CaxeFlowExecutor *hxc_l_tmp_class_field_load_result_n0 = (*hxc_l_self).hxc_flowExecutor;
  hxc_l_gc_roots[1] = (const void *)hxc_l_tmp_class_field_load_result_n0;
  struct hxc_caxecraft_scenario_CaxeFlowExecutor *hxc_l_executor = hxc_l_tmp_class_field_load_result_n0;
  struct hxc_caxecraft_scenario_CaxeFlowExecutor *hxc_l_tmp_load_result_n1 = hxc_l_executor;
  hxc_l_gc_roots[2] = (const void *)hxc_l_tmp_load_result_n1;
  hxc_l_gc_roots[3] = (const void *)NULL;
  bool hxc_l_tmp_short_circuit_result_n3 = hxc_l_tmp_load_result_n1 == NULL;
  if (!(hxc_l_tmp_load_result_n1 == NULL))
  {
    struct hxc_caxecraft_scenario_CaxeFlowExecutor *hxc_l_tmp_load_result_n2 = hxc_l_executor;
    hxc_l_gc_roots[4] = (const void *)hxc_l_tmp_load_result_n2;
    if (hxc_l_tmp_load_result_n2 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_scenario_CaxeFlowState *hxc_l_tmp_class_field_load_result_n3 = (*hxc_l_tmp_load_result_n2).hxc_state;
    hxc_l_gc_roots[5] = (const void *)hxc_l_tmp_class_field_load_result_n3;
    if (hxc_l_tmp_class_field_load_result_n3 == NULL)
    {
      abort();
    }
    bool hxc_l_tmp_instance_call_result_n4 = hxc_caxecraft_scenario_CaxeFlowState_objectActive(hxc_l_tmp_class_field_load_result_n3, hxc_l_id);
    hxc_l_tmp_short_circuit_result_n3 = !hxc_l_tmp_instance_call_result_n4;
  }
  if (hxc_l_tmp_short_circuit_result_n3)
  {
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return false;
  }
  struct hxc_caxecraft_domain_EntityStore *hxc_l_tmp_owned_class_field_address_n6 = &(*hxc_l_self).hxc_entities;
  if (hxc_l_tmp_owned_class_field_address_n6 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_domain_PlayerAgent hxc_l_tmp_class_field_load_result_n7 = (*hxc_l_self).hxc_localPlayer;
  struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n9 = hxc_caxecraft_domain_EntityStore_read(hxc_l_tmp_owned_class_field_address_n6, hxc_l_tmp_class_field_load_result_n7.hxc_characterId);
  struct hxc_caxecraft_domain_Character hxc_l_player = hxc_l_tmp_instance_call_result_n9;
  if (hxc_l_player.hxc_id <= 0)
  {
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return false;
  }
  int32_t hxc_l_g = 0;
  struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n11 = (*hxc_l_self).hxc_statefulObjectIds;
  int32_t hxc_l_tmp_array_length_result_n12;
  if (hxc_array_ref_length(hxc_l_tmp_class_field_load_result_n11, &hxc_l_tmp_array_length_result_n12) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_g1 = hxc_l_tmp_array_length_result_n12;
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n13 = hxc_l_g;
    if (!(hxc_l_tmp_load_result_n13 < hxc_l_g1))
    {
      break;
    }
    int32_t hxc_l_tmp_increment_load_result_n15 = hxc_l_g;
    hxc_l_g = hxc_i32_add_wrapping(hxc_l_tmp_increment_load_result_n15, 1);
    int32_t hxc_l_index = hxc_l_tmp_increment_load_result_n15;
    struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n16 = (*hxc_l_self).hxc_statefulObjectIds;
    hxc_string hxc_l_tmp_array_get_result_n18;
    if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n16, (size_t)hxc_l_index, &hxc_l_tmp_array_get_result_n18) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_array_element_owner_n8 = hxc_l_tmp_array_get_result_n18;
    bool hxc_l_tmp_string_equality_result_n9 = hxc_l_tmp_array_element_owner_n8.data == NULL || hxc_l_id.data == NULL ? hxc_l_tmp_array_element_owner_n8.data == hxc_l_id.data : hxc_l_tmp_array_element_owner_n8.byte_length == hxc_l_id.byte_length && (hxc_l_tmp_array_element_owner_n8.byte_length == 0 || memcmp(hxc_l_tmp_array_element_owner_n8.data, hxc_l_id.data, hxc_l_tmp_array_element_owner_n8.byte_length) == 0);
    bool hxc_l_tmp_string_equality_result_result_n21 = hxc_l_tmp_string_equality_result_n9;
    if (hxc_string_release(&hxc_l_tmp_array_element_owner_n8) != HXC_STATUS_OK)
    {
      abort();
    }
    if (!hxc_l_tmp_string_equality_result_result_n21)
    {
      continue;
    }
    int32_t hxc_l_offset = hxc_i32_multiply_wrapping(hxc_l_index, 3);
    double hxc_l_tmp_record_field_load_result_n23 = hxc_l_player.hxc_body.hxc_x;
    struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n24 = (*hxc_l_self).hxc_statefulObjectPositionsMilli;
    int32_t hxc_l_tmp_array_get_result_n26;
    if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n24, (size_t)hxc_l_offset, &hxc_l_tmp_array_get_result_n26) != HXC_STATUS_OK)
    {
      abort();
    }
    double hxc_l_dx = hxc_l_tmp_record_field_load_result_n23 * 1000.0 - (double)hxc_l_tmp_array_get_result_n26;
    double hxc_l_tmp_record_field_load_result_n27 = hxc_l_player.hxc_body.hxc_y;
    struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n28 = (*hxc_l_self).hxc_statefulObjectPositionsMilli;
    int32_t hxc_l_tmp_array_get_result_n30;
    if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n28, (size_t)hxc_i32_add_wrapping(hxc_l_offset, 1), &hxc_l_tmp_array_get_result_n30) != HXC_STATUS_OK)
    {
      abort();
    }
    double hxc_l_dy = hxc_l_tmp_record_field_load_result_n27 * 1000.0 - (double)hxc_l_tmp_array_get_result_n30;
    double hxc_l_tmp_record_field_load_result_n31 = hxc_l_player.hxc_body.hxc_z;
    struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n32 = (*hxc_l_self).hxc_statefulObjectPositionsMilli;
    int32_t hxc_l_tmp_array_get_result_n34;
    if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n32, (size_t)hxc_i32_add_wrapping(hxc_l_offset, 2), &hxc_l_tmp_array_get_result_n34) != HXC_STATUS_OK)
    {
      abort();
    }
    double hxc_l_dz = hxc_l_tmp_record_field_load_result_n31 * 1000.0 - (double)hxc_l_tmp_array_get_result_n34;
    struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n35 = (*hxc_l_self).hxc_statefulObjectRadiiMilli;
    int32_t hxc_l_tmp_array_get_result_n37;
    if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n35, (size_t)hxc_l_index, &hxc_l_tmp_array_get_result_n37) != HXC_STATUS_OK)
    {
      abort();
    }
    double hxc_l_radius = (double)hxc_l_tmp_array_get_result_n37;
    if (!(hxc_l_radius <= 0.0))
    {
      double hxc_l_tmp_load_result_n39 = hxc_l_dx;
      double hxc_l_tmp_load_result_n40 = hxc_l_dx;
      double hxc_l_tmp_load_result_n41 = hxc_l_dy;
      double hxc_l_tmp_load_result_n42 = hxc_l_dy;
      double hxc_l_tmp_load_result_n43 = hxc_l_dz;
      double hxc_l_tmp_load_result_n44 = hxc_l_dz;
      double hxc_l_tmp_load_result_n45 = hxc_l_radius;
      if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
      {
        abort();
      }
      return hxc_l_tmp_load_result_n39 * hxc_l_tmp_load_result_n40 + hxc_l_tmp_load_result_n41 * hxc_l_tmp_load_result_n42 + hxc_l_tmp_load_result_n43 * hxc_l_tmp_load_result_n44 <= hxc_l_tmp_load_result_n45 * hxc_l_radius;
    }
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return false;
  }
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return false;
}

bool hxc_caxecraft_domain_GameSession_statefulObjectIsActive(struct hxc_caxecraft_domain_GameSession *hxc_l_self, hxc_string hxc_l_id)
{
  const void *volatile hxc_l_gc_roots[6] = { NULL, NULL, NULL, NULL, NULL, NULL };
  hxc_l_gc_roots[0] = (const void *)hxc_l_self;
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 6, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_l_self == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_scenario_CaxeFlowExecutor *hxc_l_tmp_class_field_load_result_n0 = (*hxc_l_self).hxc_flowExecutor;
  hxc_l_gc_roots[1] = (const void *)hxc_l_tmp_class_field_load_result_n0;
  struct hxc_caxecraft_scenario_CaxeFlowExecutor *hxc_l_executor = hxc_l_tmp_class_field_load_result_n0;
  struct hxc_caxecraft_scenario_CaxeFlowExecutor *hxc_l_tmp_load_result_n1 = hxc_l_executor;
  hxc_l_gc_roots[2] = (const void *)hxc_l_tmp_load_result_n1;
  hxc_l_gc_roots[3] = (const void *)NULL;
  bool hxc_l_tmp_short_circuit_result_n3 = hxc_l_tmp_load_result_n1 != NULL;
  if (hxc_l_tmp_load_result_n1 != NULL)
  {
    struct hxc_caxecraft_scenario_CaxeFlowExecutor *hxc_l_tmp_load_result_n2 = hxc_l_executor;
    hxc_l_gc_roots[4] = (const void *)hxc_l_tmp_load_result_n2;
    if (hxc_l_tmp_load_result_n2 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_scenario_CaxeFlowState *hxc_l_tmp_class_field_load_result_n3 = (*hxc_l_tmp_load_result_n2).hxc_state;
    hxc_l_gc_roots[5] = (const void *)hxc_l_tmp_class_field_load_result_n3;
    if (hxc_l_tmp_class_field_load_result_n3 == NULL)
    {
      abort();
    }
    bool hxc_l_tmp_instance_call_result_n4 = hxc_caxecraft_scenario_CaxeFlowState_objectActive(hxc_l_tmp_class_field_load_result_n3, hxc_l_id);
    hxc_l_tmp_short_circuit_result_n3 = hxc_l_tmp_instance_call_result_n4;
  }
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return hxc_l_tmp_short_circuit_result_n3;
}

hxc_string hxc_caxecraft_domain_GameSession_statefulObjectState(struct hxc_caxecraft_domain_GameSession *hxc_l_self, hxc_string hxc_l_id)
{
  const void *volatile hxc_l_gc_roots[6] = { NULL, NULL, NULL, NULL, NULL, NULL };
  hxc_l_gc_roots[0] = (const void *)hxc_l_self;
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 6, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_l_self == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_scenario_CaxeFlowExecutor *hxc_l_tmp_class_field_load_result_n0 = (*hxc_l_self).hxc_flowExecutor;
  hxc_l_gc_roots[1] = (const void *)hxc_l_tmp_class_field_load_result_n0;
  struct hxc_caxecraft_scenario_CaxeFlowExecutor *hxc_l_executor = hxc_l_tmp_class_field_load_result_n0;
  hxc_l_gc_roots[2] = (const void *)hxc_l_executor;
  hxc_l_gc_roots[3] = (const void *)NULL;
  if (!(hxc_l_executor == NULL))
  {
    struct hxc_caxecraft_scenario_CaxeFlowExecutor *hxc_l_tmp_load_result_n3 = hxc_l_executor;
    hxc_l_gc_roots[4] = (const void *)hxc_l_tmp_load_result_n3;
    if (hxc_l_tmp_load_result_n3 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_scenario_CaxeFlowState *hxc_l_tmp_class_field_load_result_n4 = (*hxc_l_tmp_load_result_n3).hxc_state;
    hxc_l_gc_roots[5] = (const void *)hxc_l_tmp_class_field_load_result_n4;
    if (hxc_l_tmp_class_field_load_result_n4 == NULL)
    {
      abort();
    }
    hxc_string hxc_l_tmp_instance_call_result_n5 = hxc_caxecraft_scenario_CaxeFlowState_objectState(hxc_l_tmp_class_field_load_result_n4, hxc_l_id);
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return hxc_l_tmp_instance_call_result_n5;
  }
  hxc_string hxc_l_tmp_returned_string_owner_n3 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
  if (hxc_string_retain(hxc_l_tmp_returned_string_owner_n3) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return hxc_l_tmp_returned_string_owner_n3;
}

struct hxc_caxecraft_domain_ActorControllerTickResult hxc_caxecraft_domain_GameSession_stepAuthoredActorControllers(struct hxc_caxecraft_domain_GameSession *hxc_l_self, int32_t hxc_l_tickNumber, int32_t hxc_l_damagePolicy)
{
  const void *volatile hxc_l_gc_roots[1] = { NULL };
  hxc_l_gc_roots[0] = (const void *)hxc_l_self;
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
  const void *volatile hxc_l_gc_roots[1] = { NULL };
  hxc_l_gc_roots[0] = (const void *)hxc_l_self;
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 1, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_l_self == NULL)
  {
    abort();
  }
  const uint8_t *hxc_l_readCells = (*hxc_l_self).hxc_base.hxc_worldStorage;
  size_t hxc_l_tmp_length_n4 = 32768;
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
  const void *volatile hxc_l_gc_roots[1] = { NULL };
  hxc_l_gc_roots[0] = (const void *)hxc_l_self;
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 1, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_collision = { 0 };
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
  if (hxc_l_self == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_domain_PlayerAgent hxc_l_tmp_class_field_load_result_n5 = (*hxc_l_self).hxc_localPlayer;
  bool hxc_l_local = hxc_l_original.hxc_id == hxc_l_tmp_class_field_load_result_n5.hxc_characterId;
  if (hxc_l_local)
  {
    hxc_caxecraft_domain_GameSession_refreshLocalPlayerCollision(hxc_l_self);
  }
  bool hxc_l_tmp_load_result_n8 = hxc_l_local;
  struct hxc_array_ref *hxc_l_tmp_conditional_managed_result_n8 = { 0 };
  if (hxc_l_tmp_load_result_n8)
  {
    struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n9 = (*hxc_l_self).hxc_activeLocalPlayerCollision;
    (void)hxc_l_tmp_class_field_load_result_n9;
    hxc_l_tmp_conditional_managed_result_n8 = hxc_l_tmp_class_field_load_result_n9;
    if (hxc_array_ref_retain(hxc_l_tmp_conditional_managed_result_n8) != HXC_STATUS_OK)
    {
      abort();
    }
  }
  else
  {
    struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n10 = (*hxc_l_self).hxc_activeStatefulCollision;
    (void)hxc_l_tmp_class_field_load_result_n10;
    hxc_l_tmp_conditional_managed_result_n8 = hxc_l_tmp_class_field_load_result_n10;
    if (hxc_array_ref_retain(hxc_l_tmp_conditional_managed_result_n8) != HXC_STATUS_OK)
    {
      abort();
    }
  }
  struct hxc_array_ref *hxc_l_tmp_conditional_managed_move_result_n11 = hxc_l_tmp_conditional_managed_result_n8;
  hxc_l_collision = hxc_l_tmp_conditional_managed_move_result_n11;
  struct hxc_caxecraft_domain_CharacterStep hxc_l_tmp_call_result_n13 = hxc_caxecraft_domain_Character_stepWithCollisions(hxc_l_readCells, hxc_l_length, hxc_l_collision, hxc_l_original, hxc_l_intent, hxc_l_damagePolicy);
  struct hxc_caxecraft_domain_CharacterStep hxc_l_result = hxc_l_tmp_call_result_n13;
  struct hxc_caxecraft_domain_EntityStore *hxc_l_tmp_owned_class_field_address_n14 = &(*hxc_l_self).hxc_entities;
  if (hxc_l_tmp_owned_class_field_address_n14 == NULL)
  {
    abort();
  }
  bool hxc_l_tmp_instance_call_result_n17 = hxc_caxecraft_domain_EntityStore_replace(hxc_l_tmp_owned_class_field_address_n14, hxc_l_original.hxc_id, hxc_l_result.hxc_character);
  bool hxc_l_resolved = hxc_l_tmp_instance_call_result_n17;
  bool hxc_l_tmp_load_result_n18 = hxc_l_resolved;
  struct hxc_caxecraft_domain_Character hxc_l_tmp_conditional_result_n11 = { 0 };
  if (hxc_l_tmp_load_result_n18)
  {
    hxc_l_tmp_conditional_result_n11 = hxc_l_result.hxc_character;
  }
  else
  {
    hxc_l_tmp_conditional_result_n11 = hxc_l_original;
  }
  struct hxc_caxecraft_domain_Character hxc_l_tmp_conditional_load_result_n20 = hxc_l_tmp_conditional_result_n11;
  struct hxc_caxecraft_domain_Immersion hxc_l_tmp_record_field_load_result_n21 = hxc_l_result.hxc_immersion;
  int32_t hxc_l_tmp_record_field_load_result_n22 = hxc_l_result.hxc_drowningDamage;
  bool hxc_l_tmp_load_result_n23 = hxc_l_resolved;
  if (hxc_array_ref_release(hxc_l_collision) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return (struct hxc_caxecraft_domain_CharacterCommandStepResult){ .hxc_character = hxc_l_tmp_conditional_load_result_n20, .hxc_drowningDamage = hxc_l_tmp_record_field_load_result_n22, .hxc_immersion = hxc_l_tmp_record_field_load_result_n21, .hxc_resolved = hxc_l_tmp_load_result_n23 };
}

struct hxc_caxecraft_domain_GameTickResult hxc_caxecraft_domain_GameSession_tick(struct hxc_caxecraft_domain_GameSession *hxc_l_self, struct hxc_caxecraft_domain_GameTickInput hxc_l_input)
{
  const void *volatile hxc_l_gc_roots[6] = { NULL, NULL, NULL, NULL, NULL, NULL };
  hxc_l_gc_roots[0] = (const void *)hxc_l_self;
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 6, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_optional_caxecraft_scenario_FlowTickResult hxc_l_flowResult = { 0 };
  struct hxc_caxecraft_scenario_FlowTickInput hxc_l_tmp_instance_call_argument_0_owner_n14 = { 0 };
  if (hxc_l_self == NULL)
  {
    abort();
  }
  uint8_t *hxc_l_cells = (*hxc_l_self).hxc_base.hxc_worldStorage;
  size_t hxc_l_tmp_length_n2 = 32768;
  const uint8_t *hxc_l_readCells = (*hxc_l_self).hxc_base.hxc_worldStorage;
  size_t hxc_l_tmp_length_n3 = 32768;
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
      hxc_caxecraft_domain_GameSession_queueTriggerZoneTransitions(hxc_l_self);
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
      hxc_l_gc_roots[5] = (const void *)hxc_l_executor;
      struct hxc_array_ref *hxc_l_tmp_instance_call_result_n33 = hxc_caxecraft_domain_GameSession_currentFlowPositions(hxc_l_self, hxc_l_executor);
      hxc_l_tmp_instance_call_argument_0_owner_n14 = (struct hxc_caxecraft_scenario_FlowTickInput){ .hxc_events = hxc_l_tmp_record_field_events_owned_load_result_n31, .hxc_positions = hxc_l_tmp_instance_call_result_n33 };
      struct hxc_caxecraft_scenario_FlowTickResult hxc_l_tmp_instance_call_result_n36 = hxc_caxecraft_scenario_CaxeFlowExecutor_runTick(hxc_l_tmp_load_result_n29, hxc_l_tmp_instance_call_argument_0_owner_n14);
      hxc_optional_594996a9_destroy(&hxc_l_flowResult);
      hxc_l_flowResult = (struct hxc_optional_caxecraft_scenario_FlowTickResult){ .hxc_has_value = true, .hxc_value = hxc_l_tmp_instance_call_result_n36 };
      struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n38 = (*hxc_l_self).hxc_pendingFlowEvents;
      if (hxc_l_tmp_class_field_load_result_n38 == NULL)
      {
        abort();
      }
      if (hxc_array_resize(&hxc_l_tmp_class_field_load_result_n38->value, (size_t)0, NULL) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_caxecraft_domain_GameSession_refreshStatefulCollision(hxc_l_self);
      hxc_record_ee43f598_destroy(&hxc_l_tmp_instance_call_argument_0_owner_n14);
    }
  }
  int32_t hxc_l_tmp_load_result_n39 = hxc_l_tickIndex;
  struct hxc_caxecraft_domain_Character hxc_l_tmp_record_field_load_result_n40 = hxc_l_characterResult.hxc_character;
  struct hxc_caxecraft_domain_Immersion hxc_l_tmp_record_field_load_result_n41 = hxc_l_characterResult.hxc_immersion;
  int32_t hxc_l_tmp_record_field_load_result_n42 = hxc_l_characterResult.hxc_drowningDamage;
  struct hxc_caxecraft_domain_WaterTickResult hxc_l_tmp_load_result_n43 = hxc_l_waterResult;
  struct hxc_optional_caxecraft_scenario_FlowTickResult hxc_l_tmp_record_field_flow_owner_n15 = hxc_l_flowResult;
  if (hxc_optional_594996a9_retain(&hxc_l_tmp_record_field_flow_owner_n15) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_optional_caxecraft_scenario_FlowTickResult hxc_l_tmp_record_field_flow_owned_load_result_n45 = hxc_l_tmp_record_field_flow_owner_n15;
  bool hxc_l_tmp_load_result_n46 = hxc_l_committed;
  hxc_optional_594996a9_destroy(&hxc_l_flowResult);
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return (struct hxc_caxecraft_domain_GameTickResult){ .hxc_character = hxc_l_tmp_record_field_load_result_n40, .hxc_committed = hxc_l_tmp_load_result_n46, .hxc_drowningDamage = hxc_l_tmp_record_field_load_result_n42, .hxc_flow = hxc_l_tmp_record_field_flow_owned_load_result_n45, .hxc_immersion = hxc_l_tmp_record_field_load_result_n41, .hxc_tickIndex = hxc_l_tmp_load_result_n39, .hxc_water = hxc_l_tmp_load_result_n43 };
}

bool hxc_caxecraft_domain_GameSession_triggerZoneContains(struct hxc_caxecraft_domain_GameSession *hxc_l_self, int32_t hxc_l_zoneIndex, struct hxc_caxecraft_scenario_FlowEventPosition hxc_l_value)
{
  const void *volatile hxc_l_gc_roots[1] = { NULL };
  hxc_l_gc_roots[0] = (const void *)hxc_l_self;
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 1, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_offset = hxc_i32_multiply_wrapping(hxc_l_zoneIndex, 6);
  if (hxc_l_self == NULL)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n0 = (*hxc_l_self).hxc_triggerZoneBounds;
  int32_t hxc_l_tmp_array_get_result_n2;
  if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n0, (size_t)hxc_l_offset, &hxc_l_tmp_array_get_result_n2) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_minimumX = hxc_i32_multiply_wrapping(hxc_l_tmp_array_get_result_n2, 1000);
  struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n3 = (*hxc_l_self).hxc_triggerZoneBounds;
  int32_t hxc_l_tmp_array_get_result_n5;
  if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n3, (size_t)hxc_i32_add_wrapping(hxc_l_offset, 1), &hxc_l_tmp_array_get_result_n5) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_minimumY = hxc_i32_multiply_wrapping(hxc_l_tmp_array_get_result_n5, 1000);
  struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n6 = (*hxc_l_self).hxc_triggerZoneBounds;
  int32_t hxc_l_tmp_array_get_result_n8;
  if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n6, (size_t)hxc_i32_add_wrapping(hxc_l_offset, 2), &hxc_l_tmp_array_get_result_n8) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_minimumZ = hxc_i32_multiply_wrapping(hxc_l_tmp_array_get_result_n8, 1000);
  int32_t hxc_l_tmp_load_result_n10 = hxc_l_minimumX;
  bool hxc_l_tmp_short_circuit_result_n7 = hxc_l_value.hxc_xMilli >= hxc_l_tmp_load_result_n10;
  if (hxc_l_value.hxc_xMilli >= hxc_l_tmp_load_result_n10)
  {
    int32_t hxc_l_tmp_load_result_n12 = hxc_l_minimumX;
    struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n13 = (*hxc_l_self).hxc_triggerZoneBounds;
    int32_t hxc_l_tmp_array_get_result_n15;
    if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n13, (size_t)hxc_i32_add_wrapping(hxc_l_offset, 3), &hxc_l_tmp_array_get_result_n15) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_short_circuit_result_n7 = hxc_l_value.hxc_xMilli < hxc_i32_add_wrapping(hxc_l_tmp_load_result_n12, hxc_i32_multiply_wrapping(hxc_l_tmp_array_get_result_n15, 1000));
  }
  bool hxc_l_tmp_short_circuit_load_result_n16 = hxc_l_tmp_short_circuit_result_n7;
  bool hxc_l_tmp_short_circuit_result_n8 = hxc_l_tmp_short_circuit_load_result_n16;
  if (hxc_l_tmp_short_circuit_load_result_n16)
  {
    hxc_l_tmp_short_circuit_result_n8 = hxc_l_value.hxc_yMilli >= hxc_l_minimumY;
  }
  bool hxc_l_tmp_short_circuit_load_result_n19 = hxc_l_tmp_short_circuit_result_n8;
  bool hxc_l_tmp_short_circuit_result_n9 = hxc_l_tmp_short_circuit_load_result_n19;
  if (hxc_l_tmp_short_circuit_load_result_n19)
  {
    int32_t hxc_l_tmp_load_result_n21 = hxc_l_minimumY;
    struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n22 = (*hxc_l_self).hxc_triggerZoneBounds;
    int32_t hxc_l_tmp_array_get_result_n24;
    if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n22, (size_t)hxc_i32_add_wrapping(hxc_l_offset, 4), &hxc_l_tmp_array_get_result_n24) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_short_circuit_result_n9 = hxc_l_value.hxc_yMilli < hxc_i32_add_wrapping(hxc_l_tmp_load_result_n21, hxc_i32_multiply_wrapping(hxc_l_tmp_array_get_result_n24, 1000));
  }
  bool hxc_l_tmp_short_circuit_load_result_n25 = hxc_l_tmp_short_circuit_result_n9;
  bool hxc_l_tmp_short_circuit_result_n10 = hxc_l_tmp_short_circuit_load_result_n25;
  if (hxc_l_tmp_short_circuit_load_result_n25)
  {
    hxc_l_tmp_short_circuit_result_n10 = hxc_l_value.hxc_zMilli >= hxc_l_minimumZ;
  }
  bool hxc_l_tmp_short_circuit_load_result_n28 = hxc_l_tmp_short_circuit_result_n10;
  bool hxc_l_tmp_short_circuit_result_n11 = hxc_l_tmp_short_circuit_load_result_n28;
  if (hxc_l_tmp_short_circuit_load_result_n28)
  {
    int32_t hxc_l_tmp_load_result_n30 = hxc_l_minimumZ;
    struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n31 = (*hxc_l_self).hxc_triggerZoneBounds;
    int32_t hxc_l_tmp_array_get_result_n33;
    if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n31, (size_t)hxc_i32_add_wrapping(hxc_l_offset, 5), &hxc_l_tmp_array_get_result_n33) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_short_circuit_result_n11 = hxc_l_value.hxc_zMilli < hxc_i32_add_wrapping(hxc_l_tmp_load_result_n30, hxc_i32_multiply_wrapping(hxc_l_tmp_array_get_result_n33, 1000));
  }
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return hxc_l_tmp_short_circuit_result_n11;
}

bool hxc_caxecraft_domain_GameSession_triggerZoneContainsBlock(struct hxc_caxecraft_domain_GameSession *hxc_l_self, int32_t hxc_l_zoneIndex, struct hxc_caxecraft_scenario_VoxelPoint hxc_l_coord)
{
  const void *volatile hxc_l_gc_roots[1] = { NULL };
  hxc_l_gc_roots[0] = (const void *)hxc_l_self;
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 1, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_offset = hxc_i32_multiply_wrapping(hxc_l_zoneIndex, 6);
  if (hxc_l_self == NULL)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n1 = (*hxc_l_self).hxc_triggerZoneBounds;
  int32_t hxc_l_tmp_array_get_result_n3;
  if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n1, (size_t)hxc_l_offset, &hxc_l_tmp_array_get_result_n3) != HXC_STATUS_OK)
  {
    abort();
  }
  bool hxc_l_tmp_short_circuit_result_n4 = hxc_l_coord.hxc_x >= hxc_l_tmp_array_get_result_n3;
  if (hxc_l_coord.hxc_x >= hxc_l_tmp_array_get_result_n3)
  {
    struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n5 = (*hxc_l_self).hxc_triggerZoneBounds;
    int32_t hxc_l_tmp_array_get_result_n7;
    if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n5, (size_t)hxc_l_offset, &hxc_l_tmp_array_get_result_n7) != HXC_STATUS_OK)
    {
      abort();
    }
    struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n8 = (*hxc_l_self).hxc_triggerZoneBounds;
    int32_t hxc_l_tmp_array_get_result_n10;
    if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n8, (size_t)hxc_i32_add_wrapping(hxc_l_offset, 3), &hxc_l_tmp_array_get_result_n10) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_short_circuit_result_n4 = hxc_l_coord.hxc_x < hxc_i32_add_wrapping(hxc_l_tmp_array_get_result_n7, hxc_l_tmp_array_get_result_n10);
  }
  bool hxc_l_tmp_short_circuit_load_result_n11 = hxc_l_tmp_short_circuit_result_n4;
  bool hxc_l_tmp_short_circuit_result_n5 = hxc_l_tmp_short_circuit_load_result_n11;
  if (hxc_l_tmp_short_circuit_load_result_n11)
  {
    struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n13 = (*hxc_l_self).hxc_triggerZoneBounds;
    int32_t hxc_l_tmp_array_get_result_n15;
    if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n13, (size_t)hxc_i32_add_wrapping(hxc_l_offset, 1), &hxc_l_tmp_array_get_result_n15) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_short_circuit_result_n5 = hxc_l_coord.hxc_y >= hxc_l_tmp_array_get_result_n15;
  }
  bool hxc_l_tmp_short_circuit_load_result_n16 = hxc_l_tmp_short_circuit_result_n5;
  bool hxc_l_tmp_short_circuit_result_n6 = hxc_l_tmp_short_circuit_load_result_n16;
  if (hxc_l_tmp_short_circuit_load_result_n16)
  {
    struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n18 = (*hxc_l_self).hxc_triggerZoneBounds;
    int32_t hxc_l_tmp_array_get_result_n20;
    if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n18, (size_t)hxc_i32_add_wrapping(hxc_l_offset, 1), &hxc_l_tmp_array_get_result_n20) != HXC_STATUS_OK)
    {
      abort();
    }
    struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n21 = (*hxc_l_self).hxc_triggerZoneBounds;
    int32_t hxc_l_tmp_array_get_result_n23;
    if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n21, (size_t)hxc_i32_add_wrapping(hxc_l_offset, 4), &hxc_l_tmp_array_get_result_n23) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_short_circuit_result_n6 = hxc_l_coord.hxc_y < hxc_i32_add_wrapping(hxc_l_tmp_array_get_result_n20, hxc_l_tmp_array_get_result_n23);
  }
  bool hxc_l_tmp_short_circuit_load_result_n24 = hxc_l_tmp_short_circuit_result_n6;
  bool hxc_l_tmp_short_circuit_result_n7 = hxc_l_tmp_short_circuit_load_result_n24;
  if (hxc_l_tmp_short_circuit_load_result_n24)
  {
    struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n26 = (*hxc_l_self).hxc_triggerZoneBounds;
    int32_t hxc_l_tmp_array_get_result_n28;
    if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n26, (size_t)hxc_i32_add_wrapping(hxc_l_offset, 2), &hxc_l_tmp_array_get_result_n28) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_short_circuit_result_n7 = hxc_l_coord.hxc_z >= hxc_l_tmp_array_get_result_n28;
  }
  bool hxc_l_tmp_short_circuit_load_result_n29 = hxc_l_tmp_short_circuit_result_n7;
  bool hxc_l_tmp_short_circuit_result_n8 = hxc_l_tmp_short_circuit_load_result_n29;
  if (hxc_l_tmp_short_circuit_load_result_n29)
  {
    struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n31 = (*hxc_l_self).hxc_triggerZoneBounds;
    int32_t hxc_l_tmp_array_get_result_n33;
    if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n31, (size_t)hxc_i32_add_wrapping(hxc_l_offset, 2), &hxc_l_tmp_array_get_result_n33) != HXC_STATUS_OK)
    {
      abort();
    }
    struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n34 = (*hxc_l_self).hxc_triggerZoneBounds;
    int32_t hxc_l_tmp_array_get_result_n36;
    if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n34, (size_t)hxc_i32_add_wrapping(hxc_l_offset, 5), &hxc_l_tmp_array_get_result_n36) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_short_circuit_result_n8 = hxc_l_coord.hxc_z < hxc_i32_add_wrapping(hxc_l_tmp_array_get_result_n33, hxc_l_tmp_array_get_result_n36);
  }
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return hxc_l_tmp_short_circuit_result_n8;
}

bool hxc_caxecraft_domain_GameSession_triggerZoneSegmentCrosses(struct hxc_caxecraft_domain_GameSession *hxc_l_self, int32_t hxc_l_zoneIndex, struct hxc_caxecraft_scenario_FlowEventPosition hxc_l_previous, struct hxc_caxecraft_scenario_FlowEventPosition hxc_l_current)
{
  const void *volatile hxc_l_gc_roots[1] = { NULL };
  hxc_l_gc_roots[0] = (const void *)hxc_l_self;
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 1, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_offset = hxc_i32_multiply_wrapping(hxc_l_zoneIndex, 6);
  double hxc_l_enter = 0.0;
  double hxc_l_leave = 1.0;
  if (hxc_l_self == NULL)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n2 = (*hxc_l_self).hxc_triggerZoneBounds;
  int32_t hxc_l_tmp_array_get_result_n4;
  if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n2, (size_t)hxc_l_offset, &hxc_l_tmp_array_get_result_n4) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n5 = (*hxc_l_self).hxc_triggerZoneBounds;
  int32_t hxc_l_tmp_array_get_result_n7;
  if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n5, (size_t)hxc_l_offset, &hxc_l_tmp_array_get_result_n7) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n8 = (*hxc_l_self).hxc_triggerZoneBounds;
  int32_t hxc_l_tmp_array_get_result_n10;
  if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n8, (size_t)hxc_i32_add_wrapping(hxc_l_offset, 3), &hxc_l_tmp_array_get_result_n10) != HXC_STATUS_OK)
  {
    abort();
  }
  double hxc_l_tmp_load_result_n11 = hxc_l_enter;
  struct hxc_optional_record_h7aeb14c30427 hxc_l_tmp_call_result_n13 = hxc_caxecraft_domain_GameSession_intersectTriggerAxis(hxc_l_previous.hxc_xMilli, hxc_l_current.hxc_xMilli, hxc_i32_multiply_wrapping(hxc_l_tmp_array_get_result_n4, 1000), hxc_i32_multiply_wrapping(hxc_i32_add_wrapping(hxc_l_tmp_array_get_result_n7, hxc_l_tmp_array_get_result_n10), 1000), hxc_l_tmp_load_result_n11, hxc_l_leave);
  struct hxc_optional_record_h7aeb14c30427 hxc_l_x = hxc_l_tmp_call_result_n13;
  if (!hxc_l_x.hxc_has_value)
  {
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return false;
  }
  struct hxc_optional_record_h7aeb14c30427 hxc_l_tmp_load_result_n15 = hxc_l_x;
  if (!hxc_l_tmp_load_result_n15.hxc_has_value)
  {
    abort();
  }
  hxc_l_enter = hxc_l_tmp_load_result_n15.hxc_value.hxc_enter;
  struct hxc_optional_record_h7aeb14c30427 hxc_l_tmp_load_result_n18 = hxc_l_x;
  if (!hxc_l_tmp_load_result_n18.hxc_has_value)
  {
    abort();
  }
  hxc_l_leave = hxc_l_tmp_load_result_n18.hxc_value.hxc_leave;
  struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n23 = (*hxc_l_self).hxc_triggerZoneBounds;
  int32_t hxc_l_tmp_array_get_result_n25;
  if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n23, (size_t)hxc_i32_add_wrapping(hxc_l_offset, 1), &hxc_l_tmp_array_get_result_n25) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n26 = (*hxc_l_self).hxc_triggerZoneBounds;
  int32_t hxc_l_tmp_array_get_result_n28;
  if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n26, (size_t)hxc_i32_add_wrapping(hxc_l_offset, 1), &hxc_l_tmp_array_get_result_n28) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n29 = (*hxc_l_self).hxc_triggerZoneBounds;
  int32_t hxc_l_tmp_array_get_result_n31;
  if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n29, (size_t)hxc_i32_add_wrapping(hxc_l_offset, 4), &hxc_l_tmp_array_get_result_n31) != HXC_STATUS_OK)
  {
    abort();
  }
  double hxc_l_tmp_load_result_n32 = hxc_l_enter;
  struct hxc_optional_record_h7aeb14c30427 hxc_l_tmp_call_result_n34 = hxc_caxecraft_domain_GameSession_intersectTriggerAxis(hxc_l_previous.hxc_yMilli, hxc_l_current.hxc_yMilli, hxc_i32_multiply_wrapping(hxc_l_tmp_array_get_result_n25, 1000), hxc_i32_multiply_wrapping(hxc_i32_add_wrapping(hxc_l_tmp_array_get_result_n28, hxc_l_tmp_array_get_result_n31), 1000), hxc_l_tmp_load_result_n32, hxc_l_leave);
  struct hxc_optional_record_h7aeb14c30427 hxc_l_y = hxc_l_tmp_call_result_n34;
  if (!hxc_l_y.hxc_has_value)
  {
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return false;
  }
  struct hxc_optional_record_h7aeb14c30427 hxc_l_tmp_load_result_n36 = hxc_l_y;
  if (!hxc_l_tmp_load_result_n36.hxc_has_value)
  {
    abort();
  }
  hxc_l_enter = hxc_l_tmp_load_result_n36.hxc_value.hxc_enter;
  struct hxc_optional_record_h7aeb14c30427 hxc_l_tmp_load_result_n39 = hxc_l_y;
  if (!hxc_l_tmp_load_result_n39.hxc_has_value)
  {
    abort();
  }
  hxc_l_leave = hxc_l_tmp_load_result_n39.hxc_value.hxc_leave;
  struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n44 = (*hxc_l_self).hxc_triggerZoneBounds;
  int32_t hxc_l_tmp_array_get_result_n46;
  if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n44, (size_t)hxc_i32_add_wrapping(hxc_l_offset, 2), &hxc_l_tmp_array_get_result_n46) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n47 = (*hxc_l_self).hxc_triggerZoneBounds;
  int32_t hxc_l_tmp_array_get_result_n49;
  if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n47, (size_t)hxc_i32_add_wrapping(hxc_l_offset, 2), &hxc_l_tmp_array_get_result_n49) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n50 = (*hxc_l_self).hxc_triggerZoneBounds;
  int32_t hxc_l_tmp_array_get_result_n52;
  if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n50, (size_t)hxc_i32_add_wrapping(hxc_l_offset, 5), &hxc_l_tmp_array_get_result_n52) != HXC_STATUS_OK)
  {
    abort();
  }
  double hxc_l_tmp_load_result_n53 = hxc_l_enter;
  struct hxc_optional_record_h7aeb14c30427 hxc_l_tmp_call_result_n55 = hxc_caxecraft_domain_GameSession_intersectTriggerAxis(hxc_l_previous.hxc_zMilli, hxc_l_current.hxc_zMilli, hxc_i32_multiply_wrapping(hxc_l_tmp_array_get_result_n46, 1000), hxc_i32_multiply_wrapping(hxc_i32_add_wrapping(hxc_l_tmp_array_get_result_n49, hxc_l_tmp_array_get_result_n52), 1000), hxc_l_tmp_load_result_n53, hxc_l_leave);
  struct hxc_optional_record_h7aeb14c30427 hxc_l_z = hxc_l_tmp_call_result_n55;
  struct hxc_optional_record_h7aeb14c30427 hxc_l_tmp_load_result_n56 = hxc_l_z;
  bool hxc_l_tmp_short_circuit_result_n10 = hxc_l_tmp_load_result_n56.hxc_has_value;
  if (hxc_l_tmp_load_result_n56.hxc_has_value)
  {
    struct hxc_optional_record_h7aeb14c30427 hxc_l_tmp_load_result_n57 = hxc_l_z;
    if (!hxc_l_tmp_load_result_n57.hxc_has_value)
    {
      abort();
    }
    struct hxc_optional_record_h7aeb14c30427 hxc_l_tmp_load_result_n60 = hxc_l_z;
    if (!hxc_l_tmp_load_result_n60.hxc_has_value)
    {
      abort();
    }
    hxc_l_tmp_short_circuit_result_n10 = hxc_l_tmp_load_result_n57.hxc_value.hxc_enter < hxc_l_tmp_load_result_n60.hxc_value.hxc_leave;
  }
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return hxc_l_tmp_short_circuit_result_n10;
}

struct hxc_caxecraft_domain_LocalRecoveryResult hxc_caxecraft_domain_GameSession_useSelectedRecovery(struct hxc_caxecraft_domain_GameSession *hxc_l_self, struct hxc_caxecraft_gameplay_InventoryState hxc_l_inventory, hxc_string hxc_l_itemType)
{
  const void *volatile hxc_l_gc_roots[1] = { NULL };
  hxc_l_gc_roots[0] = (const void *)hxc_l_self;
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 1, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_stagedEvents = { 0 };
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
  struct hxc_array_ref *hxc_l_tmp_instance_call_result_n20 = hxc_caxecraft_domain_GameSession_itemUseOccurrences(hxc_l_self, hxc_l_itemType);
  hxc_l_stagedEvents = hxc_l_tmp_instance_call_result_n20;
  bool hxc_l_tmp_array_reference_equality_result_n8 = hxc_l_stagedEvents == NULL;
  bool hxc_l_tmp_array_reference_equality_result_result_n22 = hxc_l_tmp_array_reference_equality_result_n8;
  bool hxc_l_tmp_short_circuit_result_n9 = hxc_l_tmp_array_reference_equality_result_result_n22;
  if (!hxc_l_tmp_array_reference_equality_result_result_n22)
  {
    bool hxc_l_tmp_instance_call_result_n24 = hxc_caxecraft_domain_GameSession_flowEventsCanCommit(hxc_l_self, hxc_l_stagedEvents);
    hxc_l_tmp_short_circuit_result_n9 = !hxc_l_tmp_instance_call_result_n24;
  }
  if (hxc_l_tmp_short_circuit_result_n9)
  {
    int32_t hxc_l_tmp_load_result_n26 = hxc_l_decision;
    struct hxc_caxecraft_domain_Character hxc_l_tmp_load_result_n27 = hxc_l_original;
    if (hxc_array_ref_release(hxc_l_stagedEvents) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return (struct hxc_caxecraft_domain_LocalRecoveryResult){ .hxc_character = hxc_l_tmp_load_result_n27, .hxc_decision = hxc_l_tmp_load_result_n26, .hxc_inventory = hxc_l_inventory, .hxc_resolved = false };
  }
  struct hxc_caxecraft_domain_Character hxc_l_tmp_load_result_n29 = hxc_l_original;
  struct hxc_caxecraft_domain_LocalCharacterCommandResult hxc_l_tmp_instance_call_result_n31 = hxc_caxecraft_domain_GameSession_commitLocalCharacter(hxc_l_self, hxc_l_tmp_load_result_n29, hxc_l_nextCharacter);
  struct hxc_caxecraft_domain_LocalCharacterCommandResult hxc_l_committed = hxc_l_tmp_instance_call_result_n31;
  if (hxc_l_committed.hxc_resolved)
  {
    hxc_caxecraft_domain_GameSession_commitFlowEvents(hxc_l_self, hxc_l_stagedEvents);
  }
  int32_t hxc_l_tmp_record_field_decision_n11 = hxc_l_decision;
  bool hxc_l_tmp_record_field_load_result_n35 = hxc_l_committed.hxc_resolved;
  struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_conditional_result_n12 = { 0 };
  if (hxc_l_tmp_record_field_load_result_n35)
  {
    hxc_l_tmp_conditional_result_n12 = hxc_l_nextInventory;
  }
  else
  {
    hxc_l_tmp_conditional_result_n12 = hxc_l_inventory;
  }
  struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_conditional_load_result_n37 = hxc_l_tmp_conditional_result_n12;
  struct hxc_caxecraft_domain_Character hxc_l_tmp_record_field_load_result_n38 = hxc_l_committed.hxc_character;
  bool hxc_l_tmp_record_field_load_result_n39 = hxc_l_committed.hxc_resolved;
  int32_t hxc_l_tmp_record_field_decision_load_result_n40 = hxc_l_tmp_record_field_decision_n11;
  if (hxc_array_ref_release(hxc_l_stagedEvents) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return (struct hxc_caxecraft_domain_LocalRecoveryResult){ .hxc_character = hxc_l_tmp_record_field_load_result_n38, .hxc_decision = hxc_l_tmp_record_field_decision_load_result_n40, .hxc_inventory = hxc_l_tmp_conditional_load_result_n37, .hxc_resolved = hxc_l_tmp_record_field_load_result_n39 };
}

struct hxc_caxecraft_domain_GameView hxc_caxecraft_domain_GameSession_view(struct hxc_caxecraft_domain_GameSession *hxc_l_self)
{
  const void *volatile hxc_l_gc_roots[1] = { NULL };
  hxc_l_gc_roots[0] = (const void *)hxc_l_self;
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
  const void *volatile hxc_l_gc_roots[1] = { NULL };
  hxc_l_gc_roots[0] = (const void *)hxc_l_self;
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
  *hxc_l_returned_span_length = 32768;
  return (*hxc_l_self).hxc_base.hxc_worldStorage;
}

int32_t hxc_caxecraft_domain_GameSession_writeTerrainRunDuringLoad(struct hxc_caxecraft_domain_GameSession *hxc_l_self, int32_t hxc_l_startIndex, int32_t hxc_l_storageCode, int32_t hxc_l_count)
{
  const void *volatile hxc_l_gc_roots[1] = { NULL };
  hxc_l_gc_roots[0] = (const void *)hxc_l_self;
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
    hxc_l_tmp_short_circuit_result_n5 = hxc_l_startIndex > hxc_i32_subtract_wrapping(32768, hxc_l_count);
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
  uint8_t *hxc_l_cells = (*hxc_l_self).hxc_base.hxc_worldStorage;
  size_t hxc_l_tmp_length_n9 = 32768;
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
