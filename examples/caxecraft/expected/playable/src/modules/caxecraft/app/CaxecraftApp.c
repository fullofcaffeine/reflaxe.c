#include "hxc/program.h"

void hxc_compiler_constructor_caxecraft_app_CaxecraftApp(struct hxc_caxecraft_app_CaxecraftApp *hxc_l_self)
{
  struct hxc_caxecraft_app_WaterRenderer *hxc_l_tmp_owned_class_field_address_n0 = &(*hxc_l_self).hxc_waterRenderer;
  hxc_compiler_constructor_caxecraft_app_WaterRenderer(hxc_l_tmp_owned_class_field_address_n0);
  struct hxc_caxecraft_app_TerrainRenderer *hxc_l_tmp_owned_class_field_address_n1 = &(*hxc_l_self).hxc_terrainRenderer;
  hxc_compiler_constructor_caxecraft_app_TerrainRenderer(hxc_l_tmp_owned_class_field_address_n1);
  struct hxc_caxecraft_input_NavigationRepeater *hxc_l_tmp_owned_class_field_address_n2 = &(*hxc_l_self).hxc_editorNavigation;
  hxc_compiler_constructor_caxecraft_input_NavigationRepeater(hxc_l_tmp_owned_class_field_address_n2);
  return;
}

hxc_string hxc_caxecraft_app_CaxecraftApp_campaignLevelLoadFailure(struct hxc_caxecraft_content_CampaignLevelLoadError hxc_l_error)
{
  hxc_string hxc_l_path_h58ce759b9caf = { 0 };
  hxc_string hxc_l_tmp_string_concat_right_owner_n10 = { 0 };
  hxc_string hxc_l_tmp_string_concat_left_owner_n11 = { 0 };
  hxc_string hxc_l_path_h91a2fe8434cc = { 0 };
  hxc_string hxc_l_path_h1f0969577fa7 = { 0 };
  hxc_string hxc_l_path_hd8c174ddef11 = { 0 };
  int32_t hxc_l_expected = { 0 };
  int32_t hxc_l_actual = { 0 };
  hxc_string hxc_l_tmp_string_concat_left_owner_n5 = { 0 };
  hxc_string hxc_l_tmp_string_concat_left_owner_n6 = { 0 };
  hxc_string hxc_l_tmp_string_concat_right_owner_n7 = { 0 };
  hxc_string hxc_l_tmp_string_concat_left_owner_n8 = { 0 };
  hxc_string hxc_l_tmp_string_concat_left_owner_n9 = { 0 };
  switch (hxc_l_error.hxc_tag) {
    case hxc_caxecraft_content_CampaignLevelLoadError_CampaignLevelSourceRejected:
      {
        break;
      }
    case hxc_caxecraft_content_CampaignLevelLoadError_CampaignLevelLengthMismatch:
      {
        if (hxc_l_error.hxc_tag != hxc_caxecraft_content_CampaignLevelLoadError_CampaignLevelLengthMismatch)
        {
          abort();
        }
        hxc_string hxc_l_tmp_enum_payload_project_n3 = hxc_l_error.hxc_payload.hxc_CampaignLevelLengthMismatch.hxc_logicalPath;
        hxc_l_path_hd8c174ddef11 = hxc_l_tmp_enum_payload_project_n3;
        if (hxc_string_retain(hxc_l_path_hd8c174ddef11) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_l_error.hxc_tag != hxc_caxecraft_content_CampaignLevelLoadError_CampaignLevelLengthMismatch)
        {
          abort();
        }
        int32_t hxc_l_tmp_enum_payload_project_n4 = hxc_l_error.hxc_payload.hxc_CampaignLevelLengthMismatch.hxc_expected;
        hxc_l_expected = hxc_l_tmp_enum_payload_project_n4;
        if (hxc_l_error.hxc_tag != hxc_caxecraft_content_CampaignLevelLoadError_CampaignLevelLengthMismatch)
        {
          abort();
        }
        int32_t hxc_l_tmp_enum_payload_project_n5 = hxc_l_error.hxc_payload.hxc_CampaignLevelLengthMismatch.hxc_actual;
        hxc_l_actual = hxc_l_tmp_enum_payload_project_n5;
        hxc_string hxc_l_tmp_string_concat_result_n7 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
        if (hxc_string_concat_ref((hxc_string){ (const uint8_t *)"length changed for ", 19, true, NULL }, hxc_l_path_hd8c174ddef11, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n7) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_l_tmp_string_concat_left_owner_n5 = hxc_l_tmp_string_concat_result_n7;
        hxc_string hxc_l_tmp_string_concat_result_n9 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
        if (hxc_string_concat_ref(hxc_l_tmp_string_concat_left_owner_n5, (hxc_string){ (const uint8_t *)" (expected ", 11, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n9) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_l_tmp_string_concat_left_owner_n6 = hxc_l_tmp_string_concat_result_n9;
        hxc_string hxc_l_tmp_string_concat_left_borrow_result_n10 = hxc_l_tmp_string_concat_left_owner_n6;
        hxc_string hxc_l_tmp_std_string_int_result_n12 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
        if (hxc_string_from_int32(hxc_l_expected, hxc_default_allocator(), &hxc_l_tmp_std_string_int_result_n12) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_l_tmp_string_concat_right_owner_n7 = hxc_l_tmp_std_string_int_result_n12;
        hxc_string hxc_l_tmp_string_concat_result_n14 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
        if (hxc_string_concat_ref(hxc_l_tmp_string_concat_left_borrow_result_n10, hxc_l_tmp_string_concat_right_owner_n7, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n14) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_l_tmp_string_concat_left_owner_n8 = hxc_l_tmp_string_concat_result_n14;
        hxc_string hxc_l_tmp_string_concat_result_n16 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
        if (hxc_string_concat_ref(hxc_l_tmp_string_concat_left_owner_n8, (hxc_string){ (const uint8_t *)", found ", 8, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n16) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_l_tmp_string_concat_left_owner_n9 = hxc_l_tmp_string_concat_result_n16;
        hxc_string hxc_l_tmp_string_concat_left_borrow_result_n17 = hxc_l_tmp_string_concat_left_owner_n9;
        hxc_string hxc_l_tmp_std_string_int_result_n19 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
        if (hxc_string_from_int32(hxc_l_actual, hxc_default_allocator(), &hxc_l_tmp_std_string_int_result_n19) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_l_tmp_string_concat_right_owner_n10 = hxc_l_tmp_std_string_int_result_n19;
        hxc_string hxc_l_tmp_string_concat_result_n21 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
        if (hxc_string_concat_ref(hxc_l_tmp_string_concat_left_borrow_result_n17, hxc_l_tmp_string_concat_right_owner_n10, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n21) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_l_tmp_string_concat_left_owner_n11 = hxc_l_tmp_string_concat_result_n21;
        hxc_string hxc_l_tmp_string_concat_result_n23 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
        if (hxc_string_concat_ref(hxc_l_tmp_string_concat_left_owner_n11, (hxc_string){ (const uint8_t *)")", 1, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n23) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n11) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n10) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n9) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n8) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n7) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n6) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n5) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_string_release(&hxc_l_path_hd8c174ddef11) != HXC_STATUS_OK)
        {
          abort();
        }
        return hxc_l_tmp_string_concat_result_n23;
      }
    case hxc_caxecraft_content_CampaignLevelLoadError_CampaignLevelHashMismatch:
      {
        if (hxc_l_error.hxc_tag != hxc_caxecraft_content_CampaignLevelLoadError_CampaignLevelHashMismatch)
        {
          abort();
        }
        hxc_string hxc_l_tmp_enum_payload_project_n24 = hxc_l_error.hxc_payload.hxc_CampaignLevelHashMismatch.hxc_logicalPath;
        hxc_l_path_h91a2fe8434cc = hxc_l_tmp_enum_payload_project_n24;
        if (hxc_string_retain(hxc_l_path_h91a2fe8434cc) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_string hxc_l_tmp_string_concat_result_n26 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
        if (hxc_string_concat_ref((hxc_string){ (const uint8_t *)"SHA-256 receipt changed for ", 28, true, NULL }, hxc_l_path_h91a2fe8434cc, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n26) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_string_release(&hxc_l_path_h91a2fe8434cc) != HXC_STATUS_OK)
        {
          abort();
        }
        return hxc_l_tmp_string_concat_result_n26;
      }
    case hxc_caxecraft_content_CampaignLevelLoadError_CampaignLevelRuntimeRejected:
      {
        if (hxc_l_error.hxc_tag != hxc_caxecraft_content_CampaignLevelLoadError_CampaignLevelRuntimeRejected)
        {
          abort();
        }
        hxc_string hxc_l_tmp_enum_payload_project_n27 = hxc_l_error.hxc_payload.hxc_CampaignLevelRuntimeRejected.hxc_logicalPath;
        hxc_l_path_h1f0969577fa7 = hxc_l_tmp_enum_payload_project_n27;
        if (hxc_string_retain(hxc_l_path_h1f0969577fa7) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_string hxc_l_tmp_string_concat_result_n29 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
        if (hxc_string_concat_ref((hxc_string){ (const uint8_t *)"runtime validation failed for ", 30, true, NULL }, hxc_l_path_h1f0969577fa7, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n29) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_string_release(&hxc_l_path_h1f0969577fa7) != HXC_STATUS_OK)
        {
          abort();
        }
        return hxc_l_tmp_string_concat_result_n29;
      }
    default:
      {
        abort();
      }
  }
  if (hxc_l_error.hxc_tag != hxc_caxecraft_content_CampaignLevelLoadError_CampaignLevelSourceRejected)
  {
    abort();
  }
  hxc_string hxc_l_tmp_enum_payload_project_n0 = hxc_l_error.hxc_payload.hxc_CampaignLevelSourceRejected.hxc_logicalPath;
  hxc_l_path_h58ce759b9caf = hxc_l_tmp_enum_payload_project_n0;
  if (hxc_string_retain(hxc_l_path_h58ce759b9caf) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_string hxc_l_tmp_string_concat_result_n2 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
  if (hxc_string_concat_ref((hxc_string){ (const uint8_t *)"source read failed for ", 23, true, NULL }, hxc_l_path_h58ce759b9caf, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n2) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_string_release(&hxc_l_path_h58ce759b9caf) != HXC_STATUS_OK)
  {
    abort();
  }
  return hxc_l_tmp_string_concat_result_n2;
}

struct hxc_array_ref *hxc_caxecraft_app_CaxecraftApp_dialogueActorSnapshots(struct hxc_caxecraft_domain_GameSession *hxc_l_session, struct hxc_caxecraft_app_PlayableLevelView *hxc_l_level)
{
  const void *hxc_l_gc_roots[2] = { (const void *)hxc_l_session, (const void *)hxc_l_level };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 2, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_tmp_array_create_result_n0 = NULL;
  if (hxc_array_ref_create_trivial(hxc_default_allocator(), sizeof(struct hxc_caxecraft_domain_Character), _Alignof(struct hxc_caxecraft_domain_Character), &hxc_l_tmp_array_create_result_n0) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_actors = hxc_l_tmp_array_create_result_n0;
  int32_t hxc_l_g = 0;
  if (hxc_l_level == NULL)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n1 = (*hxc_l_level).hxc_dialogueActors;
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
    struct hxc_array_ref *hxc_l_tmp_load_result_n6 = hxc_l_actors;
    if (hxc_l_session == NULL)
    {
      abort();
    }
    struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n7 = (*hxc_l_level).hxc_dialogueActors;
    struct hxc_caxecraft_app_PlayableDialogueActor hxc_l_tmp_array_get_result_n9;
    if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n7, (size_t)hxc_l_index, &hxc_l_tmp_array_get_result_n9) != HXC_STATUS_OK)
    {
      abort();
    }
    struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n11 = hxc_caxecraft_domain_GameSession_readCharacter(hxc_l_session, hxc_l_tmp_array_get_result_n9.hxc_entityId);
    int32_t hxc_l_tmp_array_push_result_n12;
    if (hxc_array_ref_push_copy(hxc_l_tmp_load_result_n6, &hxc_l_tmp_instance_call_result_n11, &hxc_l_tmp_array_push_result_n12) != HXC_STATUS_OK)
    {
      abort();
    }
    (void)hxc_l_tmp_array_push_result_n12;
  }
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return hxc_l_actors;
}

bool hxc_caxecraft_app_CaxecraftApp_dialogueActorsAreValid(struct hxc_caxecraft_domain_GameSession *hxc_l_session, struct hxc_caxecraft_app_PlayableLevelView *hxc_l_level, struct hxc_array_ref *hxc_l_states)
{
  const void *hxc_l_gc_roots[2] = { (const void *)hxc_l_session, (const void *)hxc_l_level };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 2, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_g = 0;
  if (hxc_l_level == NULL)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n0 = (*hxc_l_level).hxc_dialogueActors;
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
    struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n5 = (*hxc_l_level).hxc_dialogueActors;
    struct hxc_caxecraft_app_PlayableDialogueActor hxc_l_tmp_array_get_result_n7;
    if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n5, (size_t)hxc_l_index, &hxc_l_tmp_array_get_result_n7) != HXC_STATUS_OK)
    {
      abort();
    }
    int32_t hxc_l_id = hxc_l_tmp_array_get_result_n7.hxc_entityId;
    if (hxc_l_session == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n10 = hxc_caxecraft_domain_GameSession_readCharacter(hxc_l_session, hxc_l_id);
    bool hxc_l_tmp_short_circuit_result_n7 = hxc_l_tmp_instance_call_result_n10.hxc_id <= 0;
    if (!(hxc_l_tmp_instance_call_result_n10.hxc_id <= 0))
    {
      struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation hxc_l_tmp_call_result_n14 = hxc_caxecraft_app_CaxecraftApp_observeActorPhase(hxc_l_states, hxc_l_id, hxc_caxecraft_domain_ActorControllerPhase_Stationary);
      hxc_l_tmp_short_circuit_result_n7 = !hxc_l_tmp_call_result_n14.hxc_valid;
    }
    if (hxc_l_tmp_short_circuit_result_n7)
    {
      if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
      {
        abort();
      }
      return false;
    }
  }
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return true;
}

void hxc_caxecraft_app_CaxecraftApp_drawActors(struct Camera3D hxc_l_camera, struct Texture hxc_l_entityTexture, bool hxc_l_entityTextureReady, struct hxc_array_ref *hxc_l_dialogueActors, struct hxc_caxecraft_app_PlayableLevelView *hxc_l_level, struct hxc_caxecraft_domain_Character hxc_l_enemy, int32_t hxc_l_enemyCell, enum hxc_caxecraft_domain_ActorControllerPhase hxc_l_enemyPhase, struct hxc_caxecraft_gameplay_BerryDropState hxc_l_berryDrop)
{
  const void *hxc_l_gc_roots[1] = { (const void *)hxc_l_level };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 1, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_g = 0;
  if (hxc_l_level == NULL)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n0 = (*hxc_l_level).hxc_dialogueActors;
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
    struct hxc_caxecraft_domain_Character hxc_l_tmp_array_get_result_n6;
    if (hxc_array_ref_get_copy(hxc_l_dialogueActors, (size_t)hxc_l_index, &hxc_l_tmp_array_get_result_n6) != HXC_STATUS_OK)
    {
      abort();
    }
    struct hxc_caxecraft_domain_Character hxc_l_actor = hxc_l_tmp_array_get_result_n6;
    if (hxc_l_entityTextureReady)
    {
      double hxc_l_y_h20d5daea5e54 = hxc_l_actor.hxc_body.hxc_y + 0.76;
      double hxc_l_z_h71bf271acc09 = hxc_l_actor.hxc_body.hxc_z;
      struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n9 = (*hxc_l_level).hxc_dialogueActors;
      struct hxc_caxecraft_app_PlayableDialogueActor hxc_l_tmp_array_get_result_n11;
      if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n9, (size_t)hxc_l_index, &hxc_l_tmp_array_get_result_n11) != HXC_STATUS_OK)
      {
        abort();
      }
      double hxc_l_tmp_record_field_load_result_n13 = hxc_l_actor.hxc_body.hxc_x;
      double hxc_l_tmp_load_result_n14 = hxc_l_y_h20d5daea5e54;
      hxc_caxecraft_app_CaxecraftAtlas_drawEntitySprite(hxc_l_camera, hxc_l_entityTexture, hxc_l_tmp_array_get_result_n11.hxc_presentationCellIndex, (struct Vector3){ .x = (float)hxc_l_tmp_record_field_load_result_n13, .y = (float)hxc_l_tmp_load_result_n14, .z = (float)hxc_l_z_h71bf271acc09 }, 0.95, 1.52);
    }
    else
    {
      double hxc_l_y_h12357af9af6c = hxc_l_actor.hxc_body.hxc_y + 0.54;
      double hxc_l_z_he1719da73fca = hxc_l_actor.hxc_body.hxc_z;
      double hxc_l_tmp_record_field_load_result_n19 = hxc_l_actor.hxc_body.hxc_x;
      double hxc_l_tmp_load_result_n20 = hxc_l_y_h12357af9af6c;
      DrawCube((struct Vector3){ .x = (float)hxc_l_tmp_record_field_load_result_n19, .y = (float)hxc_l_tmp_load_result_n20, .z = (float)hxc_l_z_he1719da73fca }, (float)0.50, (float)0.86, (float)0.42, (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 });
      double hxc_l_y_h0e3ef71cdaee = hxc_l_actor.hxc_body.hxc_y + 1.18;
      double hxc_l_z_h11b947972f59 = hxc_l_actor.hxc_body.hxc_z;
      double hxc_l_tmp_record_field_load_result_n26 = hxc_l_actor.hxc_body.hxc_x;
      double hxc_l_tmp_load_result_n27 = hxc_l_y_h0e3ef71cdaee;
      DrawCube((struct Vector3){ .x = (float)hxc_l_tmp_record_field_load_result_n26, .y = (float)hxc_l_tmp_load_result_n27, .z = (float)hxc_l_z_h11b947972f59 }, (float)0.44, (float)0.44, (float)0.44, (struct Color){ .r = 228, .g = 239, .b = 227, .a = 255 });
      double hxc_l_y_h12f1c102ad06 = hxc_l_actor.hxc_body.hxc_y + 1.41;
      double hxc_l_z_hf03b380332ed = hxc_l_actor.hxc_body.hxc_z;
      double hxc_l_tmp_record_field_load_result_n33 = hxc_l_actor.hxc_body.hxc_x;
      double hxc_l_tmp_load_result_n34 = hxc_l_y_h12f1c102ad06;
      DrawCube((struct Vector3){ .x = (float)hxc_l_tmp_record_field_load_result_n33, .y = (float)hxc_l_tmp_load_result_n34, .z = (float)hxc_l_z_hf03b380332ed }, (float)0.48, (float)0.16, (float)0.48, (struct Color){ .r = 15, .g = 27, .b = 39, .a = 220 });
    }
  }
  if (hxc_l_enemy.hxc_vitals.hxc_health > 0)
  {
    if (hxc_l_entityTextureReady)
    {
      double hxc_l_y_hce02091a3278 = hxc_l_enemy.hxc_body.hxc_y + 0.48;
      double hxc_l_z_h01979b7e385c = hxc_l_enemy.hxc_body.hxc_z;
      double hxc_l_tmp_load_result_n46 = hxc_l_y_hce02091a3278;
      hxc_caxecraft_app_CaxecraftAtlas_drawEntitySprite(hxc_l_camera, hxc_l_entityTexture, hxc_l_enemyCell, (struct Vector3){ .x = (float)hxc_l_enemy.hxc_body.hxc_x, .y = (float)hxc_l_tmp_load_result_n46, .z = (float)hxc_l_z_h01979b7e385c }, 1.05, 0.96);
    }
    else
    {
      double hxc_l_y_hc132791dc534 = hxc_l_enemy.hxc_body.hxc_y + 0.30;
      double hxc_l_z_h9b93f783c5c8 = hxc_l_enemy.hxc_body.hxc_z;
      double hxc_l_tmp_load_result_n55 = hxc_l_y_hc132791dc534;
      DrawCube((struct Vector3){ .x = (float)hxc_l_enemy.hxc_body.hxc_x, .y = (float)hxc_l_tmp_load_result_n55, .z = (float)hxc_l_z_h9b93f783c5c8 }, (float)0.70, (float)0.54, (float)0.70, (struct Color){ .r = 218, .g = 65, .b = 72, .a = 255 });
      double hxc_l_y_h851778ce048c = hxc_l_enemy.hxc_body.hxc_y + 0.66;
      double hxc_l_z_h8f6dcf7b95d1 = hxc_l_enemy.hxc_body.hxc_z;
      double hxc_l_tmp_load_result_n65 = hxc_l_y_h851778ce048c;
      DrawCube((struct Vector3){ .x = (float)hxc_l_enemy.hxc_body.hxc_x, .y = (float)hxc_l_tmp_load_result_n65, .z = (float)hxc_l_z_h8f6dcf7b95d1 }, (float)0.50, (float)0.34, (float)0.50, (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 });
    }
    if (hxc_l_enemyPhase == hxc_caxecraft_domain_ActorControllerPhase_Windup)
    {
      double hxc_l_y_hc0f53f629029 = hxc_l_enemy.hxc_body.hxc_y + 1.02;
      double hxc_l_z_h2119e166f005 = hxc_l_enemy.hxc_body.hxc_z;
      double hxc_l_tmp_load_result_n76 = hxc_l_y_hc0f53f629029;
      DrawCube((struct Vector3){ .x = (float)hxc_l_enemy.hxc_body.hxc_x, .y = (float)hxc_l_tmp_load_result_n76, .z = (float)hxc_l_z_h2119e166f005 }, (float)0.20, (float)0.20, (float)0.20, (struct Color){ .r = 218, .g = 65, .b = 72, .a = 255 });
    }
  }
  if (hxc_l_berryDrop.hxc_active)
  {
    double hxc_l_y_h7ef6a5fa414e = hxc_l_berryDrop.hxc_y;
    double hxc_l_z_h7ef019ea7ba6 = hxc_l_berryDrop.hxc_z;
    double hxc_l_tmp_load_result_n84 = hxc_l_y_h7ef6a5fa414e;
    DrawCube((struct Vector3){ .x = (float)(hxc_l_berryDrop.hxc_x - 0.12), .y = (float)hxc_l_tmp_load_result_n84, .z = (float)hxc_l_z_h7ef019ea7ba6 }, (float)0.18, (float)0.18, (float)0.18, (struct Color){ .r = 174, .g = 78, .b = 136, .a = 255 });
    double hxc_l_y_h4e66566cc3f0 = hxc_l_berryDrop.hxc_y;
    double hxc_l_z_hebcfd3524877 = hxc_l_berryDrop.hxc_z;
    double hxc_l_tmp_load_result_n91 = hxc_l_y_h4e66566cc3f0;
    DrawCube((struct Vector3){ .x = (float)(hxc_l_berryDrop.hxc_x + 0.12), .y = (float)hxc_l_tmp_load_result_n91, .z = (float)hxc_l_z_hebcfd3524877 }, (float)0.18, (float)0.18, (float)0.18, (struct Color){ .r = 174, .g = 78, .b = 136, .a = 255 });
  }
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return;
}

void hxc_caxecraft_app_CaxecraftApp_drawBreath(int32_t hxc_l_breathTicks, int32_t hxc_l_maximumBreathTicks, int32_t hxc_l_width, int32_t hxc_l_height)
{
  int32_t hxc_l_bubbleCount = 10;
  int32_t hxc_l_filled = 0;
  if (hxc_l_maximumBreathTicks > 0)
  {
    hxc_l_filled = hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_i32_subtract_wrapping(hxc_i32_add_wrapping(hxc_i32_multiply_wrapping(hxc_l_breathTicks, hxc_l_bubbleCount), hxc_l_maximumBreathTicks), 1), (double)hxc_l_maximumBreathTicks));
  }
  if (hxc_l_filled < 0)
  {
    hxc_l_filled = 0;
  }
  int32_t hxc_l_tmp_load_result_n2 = hxc_l_filled;
  if (hxc_l_tmp_load_result_n2 > hxc_l_bubbleCount)
  {
    hxc_l_filled = hxc_l_bubbleCount;
  }
  int32_t hxc_l_startX = hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_i32_subtract_wrapping(hxc_l_width, hxc_i32_subtract_wrapping(hxc_i32_multiply_wrapping(hxc_l_bubbleCount, 18), 4)), (double)2));
  int32_t hxc_l_y = hxc_i32_subtract_wrapping(hxc_l_height, 128);
  int32_t hxc_l_bubble = 0;
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n6 = hxc_l_bubble;
    if (!(hxc_l_tmp_load_result_n6 < hxc_l_bubbleCount))
    {
      break;
    }
    int32_t hxc_l_tmp_load_result_n8 = hxc_l_bubble;
    if (hxc_l_tmp_load_result_n8 < hxc_l_filled)
    {
      float hxc_l_radius_h475c1ba65823 = (float)6.0;
      struct Color hxc_l_this1_h90b60f3789b8 = (struct Color){ .r = 164, .g = 235, .b = 246, .a = 255 };
      int32_t hxc_l_tmp_load_result_n11 = hxc_l_startX;
      int32_t hxc_l_tmp_load_result_n12 = hxc_l_bubble;
      int32_t hxc_l_tmp_load_result_n13 = hxc_l_y;
      float hxc_l_tmp_load_result_n14 = hxc_l_radius_h475c1ba65823;
      DrawCircle((int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n11, hxc_i32_multiply_wrapping(hxc_l_tmp_load_result_n12, 18)), (int32_t)hxc_l_tmp_load_result_n13, hxc_l_tmp_load_result_n14, hxc_l_this1_h90b60f3789b8);
    }
    else
    {
      float hxc_l_radius_h99fc1d2f51e4 = (float)6.0;
      struct Color hxc_l_this1_ha0ecb373d9cf = (struct Color){ .r = 49, .g = 82, .b = 103, .a = 210 };
      int32_t hxc_l_tmp_load_result_n17 = hxc_l_startX;
      int32_t hxc_l_tmp_load_result_n18 = hxc_l_bubble;
      int32_t hxc_l_tmp_load_result_n19 = hxc_l_y;
      float hxc_l_tmp_load_result_n20 = hxc_l_radius_h99fc1d2f51e4;
      DrawCircle((int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n17, hxc_i32_multiply_wrapping(hxc_l_tmp_load_result_n18, 18)), (int32_t)hxc_l_tmp_load_result_n19, hxc_l_tmp_load_result_n20, hxc_l_this1_ha0ecb373d9cf);
    }
    hxc_l_bubble = hxc_i32_add_wrapping(hxc_l_bubble, 1);
  }
  return;
}

void hxc_caxecraft_app_CaxecraftApp_drawHealth(struct hxc_caxecraft_domain_VitalsState hxc_l_vitals, struct Texture hxc_l_hudTexture, bool hxc_l_hudTextureReady, int32_t hxc_l_width)
{
  int32_t hxc_l_heart = 0;
  while (1)
  {
    if (!(hxc_l_heart < 3))
    {
      break;
    }
    int32_t hxc_l_x = hxc_i32_add_wrapping(hxc_i32_subtract_wrapping(hxc_l_width, 170), hxc_i32_multiply_wrapping(hxc_l_heart, 50));
    int32_t hxc_l_points = hxc_i32_subtract_wrapping(hxc_l_vitals.hxc_health, hxc_i32_multiply_wrapping(hxc_l_heart, 2));
    if (hxc_l_hudTextureReady)
    {
      if (hxc_l_points >= 2)
      {
        hxc_caxecraft_app_CaxecraftAtlas_drawHudGlyph(hxc_l_hudTexture, hxc_caxecraft_app_HudGlyph_HealthFull, hxc_l_x, 18, 42);
      }
      else
      {
        if (hxc_l_points == 1)
        {
          hxc_caxecraft_app_CaxecraftAtlas_drawHudGlyph(hxc_l_hudTexture, hxc_caxecraft_app_HudGlyph_HealthHalf, hxc_l_x, 18, 42);
        }
        else
        {
          hxc_caxecraft_app_CaxecraftAtlas_drawHudGlyph(hxc_l_hudTexture, hxc_caxecraft_app_HudGlyph_HealthEmpty, hxc_l_x, 18, 42);
        }
      }
    }
    else
    {
      if (hxc_l_points > 0)
      {
        struct Color hxc_l_this1_h4bf1e3cb0db1 = (struct Color){ .r = 218, .g = 65, .b = 72, .a = 255 };
        int32_t hxc_l_tmp_load_result_n14 = hxc_l_x;
        DrawRectangle((int32_t)hxc_l_tmp_load_result_n14, (int32_t)22, (int32_t)34, (int32_t)26, hxc_l_this1_h4bf1e3cb0db1);
      }
      else
      {
        struct Color hxc_l_this1_h62a6326a039a = (struct Color){ .r = 228, .g = 239, .b = 227, .a = 255 };
        int32_t hxc_l_tmp_load_result_n17 = hxc_l_x;
        DrawRectangleLines((int32_t)hxc_l_tmp_load_result_n17, (int32_t)22, (int32_t)34, (int32_t)26, hxc_l_this1_h62a6326a039a);
      }
    }
    hxc_l_heart = hxc_i32_add_wrapping(hxc_l_heart, 1);
  }
  return;
}

void hxc_caxecraft_app_CaxecraftApp_drawHotbar(struct hxc_caxecraft_gameplay_InventoryState hxc_l_inventory, struct Texture hxc_l_hudTexture, bool hxc_l_hudTextureReady, struct Texture hxc_l_itemTexture, bool hxc_l_itemTextureReady, int32_t hxc_l_width, int32_t hxc_l_height)
{
  int32_t hxc_l_slotSize = 64;
  int32_t hxc_l_gap = 4;
  int32_t hxc_l_tmp_load_result_n0 = hxc_l_slotSize;
  int32_t hxc_l_totalWidth = hxc_i32_add_wrapping(hxc_i32_multiply_wrapping(9, hxc_l_tmp_load_result_n0), hxc_i32_multiply_wrapping(8, hxc_l_gap));
  int32_t hxc_l_startX = hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_i32_subtract_wrapping(hxc_l_width, hxc_l_totalWidth), (double)2));
  int32_t hxc_l_y = hxc_i32_subtract_wrapping(hxc_l_height, 96);
  int32_t hxc_l_slot = 0;
  while (1)
  {
    if (!(hxc_l_slot < 9))
    {
      break;
    }
    int32_t hxc_l_tmp_load_result_n4 = hxc_l_startX;
    int32_t hxc_l_tmp_load_result_n5 = hxc_l_slot;
    int32_t hxc_l_tmp_load_result_n6 = hxc_l_slotSize;
    int32_t hxc_l_x = hxc_i32_add_wrapping(hxc_l_tmp_load_result_n4, hxc_i32_multiply_wrapping(hxc_l_tmp_load_result_n5, hxc_i32_add_wrapping(hxc_l_tmp_load_result_n6, hxc_l_gap)));
    if (hxc_l_hudTextureReady)
    {
      if (hxc_l_slot == hxc_l_inventory.hxc_selected)
      {
        int32_t hxc_l_tmp_load_result_n11 = hxc_l_x;
        int32_t hxc_l_tmp_load_result_n12 = hxc_l_y;
        hxc_caxecraft_app_CaxecraftAtlas_drawHotbarFrame(hxc_l_hudTexture, hxc_caxecraft_app_HotbarFrame_Selected, hxc_l_tmp_load_result_n11, hxc_l_tmp_load_result_n12, hxc_l_slotSize);
      }
      else
      {
        int32_t hxc_l_tmp_load_result_n15 = hxc_l_x;
        int32_t hxc_l_tmp_load_result_n16 = hxc_l_y;
        hxc_caxecraft_app_CaxecraftAtlas_drawHotbarFrame(hxc_l_hudTexture, hxc_caxecraft_app_HotbarFrame_Normal, hxc_l_tmp_load_result_n15, hxc_l_tmp_load_result_n16, hxc_l_slotSize);
      }
    }
    else
    {
      struct Color hxc_l_this1 = (struct Color){ .r = 15, .g = 27, .b = 39, .a = 220 };
      int32_t hxc_l_tmp_load_result_n19 = hxc_l_x;
      int32_t hxc_l_tmp_load_result_n20 = hxc_l_y;
      int32_t hxc_l_tmp_load_result_n21 = hxc_l_slotSize;
      int32_t hxc_l_tmp_load_result_n22 = hxc_l_slotSize;
      DrawRectangle((int32_t)hxc_l_tmp_load_result_n19, (int32_t)hxc_l_tmp_load_result_n20, (int32_t)hxc_l_tmp_load_result_n21, (int32_t)hxc_l_tmp_load_result_n22, hxc_l_this1);
      int32_t hxc_l_tmp_load_result_n24 = hxc_l_slot;
      struct Color hxc_l_tmp_conditional_result_n16 = { 0 };
      if (hxc_l_tmp_load_result_n24 == hxc_l_inventory.hxc_selected)
      {
        hxc_l_tmp_conditional_result_n16 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
      }
      else
      {
        hxc_l_tmp_conditional_result_n16 = (struct Color){ .r = 228, .g = 239, .b = 227, .a = 255 };
      }
      struct Color hxc_l_color = hxc_l_tmp_conditional_result_n16;
      int32_t hxc_l_tmp_load_result_n29 = hxc_l_x;
      int32_t hxc_l_tmp_load_result_n30 = hxc_l_y;
      int32_t hxc_l_tmp_load_result_n31 = hxc_l_slotSize;
      int32_t hxc_l_tmp_load_result_n32 = hxc_l_slotSize;
      DrawRectangleLines((int32_t)hxc_l_tmp_load_result_n29, (int32_t)hxc_l_tmp_load_result_n30, (int32_t)hxc_l_tmp_load_result_n31, (int32_t)hxc_l_tmp_load_result_n32, hxc_l_color);
    }
    if (hxc_l_itemTextureReady)
    {
      enum hxc_caxecraft_gameplay_ItemKind hxc_l_tmp_call_result_n35 = hxc_caxecraft_gameplay_Inventory_itemAt(hxc_l_slot);
      int32_t hxc_l_tmp_load_result_n36 = hxc_l_x;
      int32_t hxc_l_tmp_load_result_n37 = hxc_l_y;
      hxc_caxecraft_app_CaxecraftAtlas_drawItem(hxc_l_itemTexture, hxc_l_tmp_call_result_n35, hxc_i32_add_wrapping(hxc_l_tmp_load_result_n36, 6), hxc_i32_add_wrapping(hxc_l_tmp_load_result_n37, 4), hxc_i32_subtract_wrapping(hxc_l_slotSize, 12));
    }
    int32_t hxc_l_tmp_call_result_n40 = hxc_caxecraft_gameplay_Inventory_countAt(hxc_l_inventory, hxc_l_slot);
    int32_t hxc_l_tmp_load_result_n41 = hxc_l_x;
    hxc_caxecraft_app_HudDigits_drawNumber(hxc_l_tmp_call_result_n40, hxc_i32_add_wrapping(hxc_l_tmp_load_result_n41, 39), hxc_i32_add_wrapping(hxc_l_y, 44), 2, (struct Color){ .r = 228, .g = 239, .b = 227, .a = 255 });
    hxc_l_slot = hxc_i32_add_wrapping(hxc_l_slot, 1);
  }
  return;
}

void hxc_caxecraft_app_CaxecraftApp_drawHud(struct hxc_caxecraft_app_HudView hxc_l_view, struct hxc_caxecraft_app_HudResources hxc_l_resources, struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_contentRegistry, struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_uiCatalog)
{
  const void *hxc_l_gc_roots[18] = { (const void *)hxc_l_view.hxc_presentation, (const void *)hxc_l_contentRegistry, (const void *)hxc_l_uiCatalog, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 18, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_string hxc_l_tmp_managed_flow_owner_n100 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n101 = { 0 };
  hxc_string hxc_l_text_h7ba79ce41ad0 = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n105 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n106 = { 0 };
  hxc_string hxc_l_text_ha3dc2968044e = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n110 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n111 = { 0 };
  hxc_string hxc_l_text_h1bfa3b41bef0 = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n115 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n116 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n118 = { 0 };
  hxc_string hxc_l_text_h51828ddbb0fa = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n122 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n123 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n125 = { 0 };
  hxc_string hxc_l_text1_h6cdf5fedfbfd = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n131 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n132 = { 0 };
  hxc_string hxc_l_text_h409c3cedb1e0 = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n136 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n137 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n150 = { 0 };
  hxc_string hxc_l_text1_h2995ec10316e = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n153 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n155 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n156 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n157 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n55 = { 0 };
  hxc_string hxc_l_text_h4eb81d03c0d6 = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n62 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n63 = { 0 };
  hxc_string hxc_l_text1_h49616dfec549 = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n68 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n69 = { 0 };
  hxc_string hxc_l_text_hd61815552d14 = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n73 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n74 = { 0 };
  hxc_string hxc_l_text_had7867538e76 = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n78 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n79 = { 0 };
  hxc_string hxc_l_text_hc297d493ace3 = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n83 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n84 = { 0 };
  hxc_string hxc_l_text_h22b2ee51f74d = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n88 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n89 = { 0 };
  hxc_string hxc_l_text_ha2bb5953880b = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n93 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n94 = { 0 };
  hxc_string hxc_l_text_h5f0724396197 = { 0 };
  int32_t hxc_l_visible = hxc_l_view.hxc_metrics.hxc_visibleBlocks;
  int32_t hxc_l_drawCalls = hxc_l_view.hxc_metrics.hxc_drawCalls;
  int32_t hxc_l_frames = hxc_l_view.hxc_metrics.hxc_renderedFrames;
  int32_t hxc_l_updates = hxc_l_view.hxc_metrics.hxc_completedTicks;
  bool hxc_l_paused = hxc_l_view.hxc_paused;
  bool hxc_l_captured = hxc_l_view.hxc_pointerCaptured;
  bool hxc_l_placementBlocked = hxc_l_view.hxc_feedback.hxc_placementBlocked;
  struct hxc_caxecraft_domain_RaycastHit hxc_l_hit = hxc_l_view.hxc_hit;
  int32_t hxc_l_mode = hxc_l_view.hxc_mode;
  int32_t hxc_l_locale = hxc_l_view.hxc_locale;
  struct hxc_caxecraft_gameplay_InventoryState hxc_l_inventory = hxc_l_view.hxc_inventory;
  hxc_string hxc_l_activeDialogue = hxc_l_view.hxc_activeDialogue;
  if (hxc_string_retain(hxc_l_activeDialogue) != HXC_STATUS_OK)
  {
    abort();
  }
  bool hxc_l_guideInteractionAvailable = hxc_l_view.hxc_guideInteractionAvailable;
  struct hxc_caxecraft_domain_Character hxc_l_enemy = hxc_l_view.hxc_enemy;
  enum hxc_caxecraft_domain_ActorControllerPhase hxc_l_enemyPhase = hxc_l_view.hxc_enemyPhase;
  struct hxc_caxecraft_domain_VitalsState hxc_l_vitals = hxc_l_view.hxc_character.hxc_vitals;
  bool hxc_l_strikeHit = hxc_l_view.hxc_feedback.hxc_strikeHit;
  bool hxc_l_enemyDefeated = hxc_l_view.hxc_feedback.hxc_enemyDefeated;
  bool hxc_l_enemyAttacked = hxc_l_view.hxc_feedback.hxc_enemyAttacked;
  bool hxc_l_pickedUp = hxc_l_view.hxc_feedback.hxc_pickedUp;
  int32_t hxc_l_pickupAmount = hxc_l_view.hxc_feedback.hxc_pickupAmount;
  int32_t hxc_l_inventoryFullReason = hxc_l_view.hxc_feedback.hxc_inventoryFullReason;
  int32_t hxc_l_recoveryFeedback = hxc_l_view.hxc_feedback.hxc_recoveryDecision;
  bool hxc_l_recoveryVisible = hxc_l_view.hxc_feedback.hxc_recoveryVisible;
  struct Texture hxc_l_hudTexture = hxc_l_resources.hxc_hudTexture;
  bool hxc_l_hudTextureReady = hxc_l_resources.hxc_hudTextureReady;
  struct Texture hxc_l_itemTexture = hxc_l_resources.hxc_itemTexture;
  bool hxc_l_itemTextureReady = hxc_l_resources.hxc_itemTextureReady;
  struct Texture hxc_l_adventureItemTexture = hxc_l_resources.hxc_adventureItemTexture;
  bool hxc_l_adventureItemTextureReady = hxc_l_resources.hxc_adventureItemTextureReady;
  int32_t hxc_l_aquaticEquipmentCode = hxc_l_view.hxc_character.hxc_aquaticEquipmentCode;
  bool hxc_l_aquaticEquipmentVisible = hxc_l_view.hxc_character.hxc_aquaticEquipmentVisible;
  bool hxc_l_headSubmerged = hxc_l_view.hxc_character.hxc_headSubmerged;
  int32_t hxc_l_breathTicks = hxc_l_view.hxc_character.hxc_breathTicks;
  int32_t hxc_l_maximumBreathTicks = hxc_l_view.hxc_character.hxc_maximumBreathTicks;
  hxc_l_gc_roots[3] = (const void *)hxc_l_view.hxc_presentation;
  struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_presentation = hxc_l_view.hxc_presentation;
  int32_t hxc_l_tmp_native_call_result_n55 = GetScreenWidth();
  int32_t hxc_l_width = (int32_t)hxc_l_tmp_native_call_result_n55;
  int32_t hxc_l_tmp_native_call_result_n56 = GetScreenHeight();
  int32_t hxc_l_height = (int32_t)hxc_l_tmp_native_call_result_n56;
  int32_t hxc_l_centerX = hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_l_width, (double)2));
  int32_t hxc_l_centerY = hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_l_height, (double)2));
  struct Color hxc_l_text_h774b5c1af5bc = (struct Color){ .r = 228, .g = 239, .b = 227, .a = 255 };
  int32_t hxc_l_tmp_load_result_n60 = hxc_l_centerX;
  int32_t hxc_l_tmp_load_result_n61 = hxc_l_centerY;
  int32_t hxc_l_tmp_load_result_n62 = hxc_l_centerX;
  int32_t hxc_l_tmp_load_result_n63 = hxc_l_centerY;
  DrawLine((int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n60, 8), (int32_t)hxc_l_tmp_load_result_n61, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n62, 3), (int32_t)hxc_l_tmp_load_result_n63, hxc_l_text_h774b5c1af5bc);
  int32_t hxc_l_tmp_load_result_n65 = hxc_l_centerX;
  int32_t hxc_l_tmp_load_result_n66 = hxc_l_centerY;
  int32_t hxc_l_tmp_load_result_n67 = hxc_l_centerX;
  int32_t hxc_l_tmp_load_result_n68 = hxc_l_centerY;
  DrawLine((int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n65, 3), (int32_t)hxc_l_tmp_load_result_n66, (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n67, 8), (int32_t)hxc_l_tmp_load_result_n68, hxc_l_text_h774b5c1af5bc);
  int32_t hxc_l_tmp_load_result_n70 = hxc_l_centerX;
  int32_t hxc_l_tmp_load_result_n71 = hxc_l_centerY;
  int32_t hxc_l_tmp_load_result_n72 = hxc_l_centerX;
  int32_t hxc_l_tmp_load_result_n73 = hxc_l_centerY;
  DrawLine((int32_t)hxc_l_tmp_load_result_n70, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n71, 8), (int32_t)hxc_l_tmp_load_result_n72, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n73, 3), hxc_l_text_h774b5c1af5bc);
  int32_t hxc_l_tmp_load_result_n75 = hxc_l_centerX;
  int32_t hxc_l_tmp_load_result_n76 = hxc_l_centerY;
  int32_t hxc_l_tmp_load_result_n77 = hxc_l_centerX;
  int32_t hxc_l_tmp_load_result_n78 = hxc_l_centerY;
  DrawLine((int32_t)hxc_l_tmp_load_result_n75, (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n76, 3), (int32_t)hxc_l_tmp_load_result_n77, (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n78, 8), hxc_l_text_h774b5c1af5bc);
  struct Color hxc_l_this1_he51a4df7f67d = (struct Color){ .r = 15, .g = 27, .b = 39, .a = 220 };
  DrawRectangle((int32_t)18, (int32_t)18, (int32_t)460, (int32_t)108, hxc_l_this1_he51a4df7f67d);
  struct Color hxc_l_this1_h9884e0f00611 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
  DrawRectangleLines((int32_t)18, (int32_t)18, (int32_t)460, (int32_t)108, hxc_l_this1_h9884e0f00611);
  if (hxc_l_uiCatalog == NULL)
  {
    abort();
  }
  hxc_string hxc_l_tmp_instance_call_result_n85 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_uiCatalog, hxc_l_locale, 1);
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n47 = hxc_l_tmp_instance_call_result_n85;
  hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n87 = HXC_BORROWED_CSTRING_INITIALIZER;
  if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n47, &hxc_l_tmp_string_borrow_cstring_result_n87) != HXC_STATUS_OK)
  {
    abort();
  }
  DrawText(hxc_l_tmp_string_borrow_cstring_result_n87.data, (int32_t)32, (int32_t)28, (int32_t)20, hxc_l_text_h774b5c1af5bc);
  hxc_string hxc_l_text1_h9ace1a5abd84 = hxc_l_view.hxc_levelLabel;
  if (hxc_string_retain(hxc_l_text1_h9ace1a5abd84) != HXC_STATUS_OK)
  {
    abort();
  }
  struct Color hxc_l_this1_hba66f2992b15 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
  hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n92 = HXC_BORROWED_CSTRING_INITIALIZER;
  if (hxc_string_borrow_cstring(&hxc_l_text1_h9ace1a5abd84, &hxc_l_tmp_string_borrow_cstring_result_n92) != HXC_STATUS_OK)
  {
    abort();
  }
  DrawText(hxc_l_tmp_string_borrow_cstring_result_n92.data, (int32_t)250, (int32_t)30, (int32_t)16, hxc_l_this1_hba66f2992b15);
  hxc_string hxc_l_tmp_instance_call_result_n95 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_uiCatalog, hxc_l_locale, 4);
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n50 = hxc_l_tmp_instance_call_result_n95;
  hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n97 = HXC_BORROWED_CSTRING_INITIALIZER;
  if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n50, &hxc_l_tmp_string_borrow_cstring_result_n97) != HXC_STATUS_OK)
  {
    abort();
  }
  DrawText(hxc_l_tmp_string_borrow_cstring_result_n97.data, (int32_t)32, (int32_t)58, (int32_t)14, hxc_l_text_h774b5c1af5bc);
  hxc_caxecraft_app_HudDigits_drawNumber(16384, 82, 59, 5, (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 });
  hxc_string hxc_l_tmp_instance_call_result_n101 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_uiCatalog, hxc_l_locale, 8);
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n51 = hxc_l_tmp_instance_call_result_n101;
  hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n103 = HXC_BORROWED_CSTRING_INITIALIZER;
  if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n51, &hxc_l_tmp_string_borrow_cstring_result_n103) != HXC_STATUS_OK)
  {
    abort();
  }
  DrawText(hxc_l_tmp_string_borrow_cstring_result_n103.data, (int32_t)160, (int32_t)58, (int32_t)14, hxc_l_text_h774b5c1af5bc);
  hxc_caxecraft_app_HudDigits_drawNumber(hxc_l_visible, 230, 59, 5, (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 });
  hxc_string hxc_l_tmp_instance_call_result_n108 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_uiCatalog, hxc_l_locale, 5);
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n52 = hxc_l_tmp_instance_call_result_n108;
  hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n110 = HXC_BORROWED_CSTRING_INITIALIZER;
  if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n52, &hxc_l_tmp_string_borrow_cstring_result_n110) != HXC_STATUS_OK)
  {
    abort();
  }
  DrawText(hxc_l_tmp_string_borrow_cstring_result_n110.data, (int32_t)326, (int32_t)58, (int32_t)14, hxc_l_text_h774b5c1af5bc);
  hxc_caxecraft_app_HudDigits_drawNumber(hxc_l_drawCalls, 382, 59, 5, (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 });
  hxc_string hxc_l_tmp_instance_call_result_n115 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_uiCatalog, hxc_l_locale, 6);
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n53 = hxc_l_tmp_instance_call_result_n115;
  hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n117 = HXC_BORROWED_CSTRING_INITIALIZER;
  if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n53, &hxc_l_tmp_string_borrow_cstring_result_n117) != HXC_STATUS_OK)
  {
    abort();
  }
  DrawText(hxc_l_tmp_string_borrow_cstring_result_n117.data, (int32_t)32, (int32_t)86, (int32_t)12, hxc_l_text_h774b5c1af5bc);
  int32_t hxc_l_tmp_load_result_n119 = hxc_l_frames;
  hxc_caxecraft_app_HudDigits_drawNumber(hxc_l_tmp_load_result_n119, 82, 85, 6, hxc_l_text_h774b5c1af5bc);
  hxc_string hxc_l_tmp_instance_call_result_n122 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_uiCatalog, hxc_l_locale, 7);
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n54 = hxc_l_tmp_instance_call_result_n122;
  hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n124 = HXC_BORROWED_CSTRING_INITIALIZER;
  if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n54, &hxc_l_tmp_string_borrow_cstring_result_n124) != HXC_STATUS_OK)
  {
    abort();
  }
  DrawText(hxc_l_tmp_string_borrow_cstring_result_n124.data, (int32_t)174, (int32_t)86, (int32_t)12, hxc_l_text_h774b5c1af5bc);
  int32_t hxc_l_tmp_load_result_n126 = hxc_l_updates;
  hxc_caxecraft_app_HudDigits_drawNumber(hxc_l_tmp_load_result_n126, 216, 85, 6, hxc_l_text_h774b5c1af5bc);
  struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_load_result_n128 = hxc_l_inventory;
  struct Texture hxc_l_tmp_load_result_n129 = hxc_l_hudTexture;
  bool hxc_l_tmp_load_result_n130 = hxc_l_hudTextureReady;
  struct Texture hxc_l_tmp_load_result_n131 = hxc_l_itemTexture;
  bool hxc_l_tmp_load_result_n132 = hxc_l_itemTextureReady;
  int32_t hxc_l_tmp_load_result_n133 = hxc_l_width;
  hxc_caxecraft_app_CaxecraftApp_drawHotbar(hxc_l_tmp_load_result_n128, hxc_l_tmp_load_result_n129, hxc_l_tmp_load_result_n130, hxc_l_tmp_load_result_n131, hxc_l_tmp_load_result_n132, hxc_l_tmp_load_result_n133, hxc_l_height);
  struct hxc_caxecraft_domain_VitalsState hxc_l_tmp_load_result_n135 = hxc_l_vitals;
  struct Texture hxc_l_tmp_load_result_n136 = hxc_l_hudTexture;
  bool hxc_l_tmp_load_result_n137 = hxc_l_hudTextureReady;
  hxc_caxecraft_app_CaxecraftApp_drawHealth(hxc_l_tmp_load_result_n135, hxc_l_tmp_load_result_n136, hxc_l_tmp_load_result_n137, hxc_l_width);
  if (hxc_l_aquaticEquipmentCode >= 0)
  {
    int32_t hxc_l_tmp_load_result_n140 = hxc_l_aquaticEquipmentCode;
    struct Texture hxc_l_tmp_load_result_n141 = hxc_l_itemTexture;
    bool hxc_l_tmp_load_result_n142 = hxc_l_itemTextureReady;
    struct Texture hxc_l_tmp_load_result_n143 = hxc_l_adventureItemTexture;
    bool hxc_l_tmp_load_result_n144 = hxc_l_adventureItemTextureReady;
    hxc_caxecraft_app_AuthoredItemRenderer_drawEquippedIcon(hxc_l_contentRegistry, hxc_l_tmp_load_result_n140, hxc_l_tmp_load_result_n141, hxc_l_tmp_load_result_n142, hxc_l_tmp_load_result_n143, hxc_l_tmp_load_result_n144, hxc_i32_subtract_wrapping(hxc_l_width, 226), 18, 42);
  }
  if (hxc_l_headSubmerged)
  {
    int32_t hxc_l_tmp_load_result_n147 = hxc_l_breathTicks;
    int32_t hxc_l_tmp_load_result_n148 = hxc_l_maximumBreathTicks;
    int32_t hxc_l_tmp_load_result_n149 = hxc_l_width;
    hxc_caxecraft_app_CaxecraftApp_drawBreath(hxc_l_tmp_load_result_n147, hxc_l_tmp_load_result_n148, hxc_l_tmp_load_result_n149, hxc_l_height);
  }
  hxc_string hxc_l_tmp_instance_call_result_n152 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_uiCatalog, hxc_l_locale, 3);
  hxc_l_tmp_cstring_ref_owner_owner_n55 = hxc_l_tmp_instance_call_result_n152;
  hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n154 = HXC_BORROWED_CSTRING_INITIALIZER;
  if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n55, &hxc_l_tmp_string_borrow_cstring_result_n154) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_tmp_load_result_n155 = hxc_l_height;
  DrawText(hxc_l_tmp_string_borrow_cstring_result_n154.data, (int32_t)20, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n155, 22), (int32_t)14, hxc_l_text_h774b5c1af5bc);
  int32_t hxc_l_tmp_load_result_n157 = hxc_l_mode;
  bool hxc_l_tmp_short_circuit_result_n56 = hxc_l_tmp_load_result_n157 == 1;
  if (hxc_l_tmp_load_result_n157 == 1)
  {
    hxc_string hxc_l_tmp_record_field_project_n158 = hxc_l_view.hxc_objectiveTitle;
    if (hxc_l_tmp_record_field_project_n158.data == NULL)
    {
      abort();
    }
    int32_t hxc_l_tmp_string_length_result_n159;
    if (hxc_string_haxe_length(hxc_l_tmp_record_field_project_n158, &hxc_l_tmp_string_length_result_n159) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_short_circuit_result_n56 = hxc_l_tmp_string_length_result_n159 > 0;
  }
  if (hxc_l_tmp_short_circuit_result_n56)
  {
    hxc_l_text_h4eb81d03c0d6 = hxc_l_view.hxc_objectiveTitle;
    if (hxc_string_retain(hxc_l_text_h4eb81d03c0d6) != HXC_STATUS_OK)
    {
      abort();
    }
    struct Color hxc_l_this1_h4cd50989ded1 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
    hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n164 = HXC_BORROWED_CSTRING_INITIALIZER;
    if (hxc_string_borrow_cstring(&hxc_l_text_h4eb81d03c0d6, &hxc_l_tmp_string_borrow_cstring_result_n164) != HXC_STATUS_OK)
    {
      abort();
    }
    DrawText(hxc_l_tmp_string_borrow_cstring_result_n164.data, (int32_t)32, (int32_t)110, (int32_t)14, hxc_l_this1_h4cd50989ded1);
    if (hxc_string_release(&hxc_l_text_h4eb81d03c0d6) != HXC_STATUS_OK)
    {
      abort();
    }
  }
  bool hxc_l_tmp_load_result_n166 = hxc_l_paused;
  bool hxc_l_tmp_short_circuit_result_n59 = !hxc_l_tmp_load_result_n166;
  if (!hxc_l_tmp_load_result_n166)
  {
    hxc_l_tmp_short_circuit_result_n59 = hxc_l_activeDialogue.data != NULL;
  }
  if (hxc_l_tmp_short_circuit_result_n59)
  {
    struct Color hxc_l_this1_h9866dd2dc646 = (struct Color){ .r = 15, .g = 27, .b = 39, .a = 220 };
    int32_t hxc_l_tmp_load_result_n170 = hxc_l_centerX;
    int32_t hxc_l_tmp_load_result_n171 = hxc_l_centerY;
    DrawRectangle((int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n170, 260), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n171, 54), (int32_t)520, (int32_t)60, hxc_l_this1_h9866dd2dc646);
    hxc_string hxc_l_text1_h3d75b131a59b = { 0 };
    switch (hxc_l_locale) {
      case 0:
        {
          hxc_l_text1_h3d75b131a59b = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
          if (hxc_string_retain(hxc_l_text1_h3d75b131a59b) != HXC_STATUS_OK)
          {
            abort();
          }
          break;
        }
      case 1:
        {
          hxc_l_text1_h3d75b131a59b = (hxc_string){ (const uint8_t *)"es-mx", 5, true, NULL };
          if (hxc_string_retain(hxc_l_text1_h3d75b131a59b) != HXC_STATUS_OK)
          {
            abort();
          }
          break;
        }
      default:
        {
          hxc_l_text1_h3d75b131a59b = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
          if (hxc_string_retain(hxc_l_text1_h3d75b131a59b) != HXC_STATUS_OK)
          {
            abort();
          }
          break;
        }
    }
    hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n174 = hxc_l_text1_h3d75b131a59b;
    hxc_l_tmp_managed_flow_owner_n62 = hxc_l_tmp_managed_flow_carrier_move_result_n174;
    struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_load_result_n175 = hxc_l_presentation;
    hxc_l_gc_roots[4] = (const void *)hxc_l_tmp_load_result_n175;
    if (hxc_l_tmp_load_result_n175 == NULL)
    {
      abort();
    }
    hxc_string hxc_l_tmp_load_result_n176 = hxc_l_activeDialogue;
    hxc_string hxc_l_tmp_instance_call_result_n178 = hxc_caxecraft_content_RuntimeLevelPresentation_dialogueLine(hxc_l_tmp_load_result_n175, hxc_l_tmp_load_result_n176, 0, hxc_l_tmp_managed_flow_owner_n62);
    hxc_l_tmp_cstring_ref_owner_owner_n63 = hxc_l_tmp_instance_call_result_n178;
    hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n180 = HXC_BORROWED_CSTRING_INITIALIZER;
    if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n63, &hxc_l_tmp_string_borrow_cstring_result_n180) != HXC_STATUS_OK)
    {
      abort();
    }
    int32_t hxc_l_tmp_load_result_n181 = hxc_l_centerX;
    int32_t hxc_l_tmp_load_result_n182 = hxc_l_centerY;
    DrawText(hxc_l_tmp_string_borrow_cstring_result_n180.data, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n181, 225), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n182, 74), (int32_t)16, hxc_l_text_h774b5c1af5bc);
    if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n63) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n62) != HXC_STATUS_OK)
    {
      abort();
    }
  }
  else
  {
    bool hxc_l_tmp_load_result_n184 = hxc_l_paused;
    bool hxc_l_tmp_short_circuit_result_n64 = !hxc_l_tmp_load_result_n184;
    if (!hxc_l_tmp_load_result_n184)
    {
      hxc_l_tmp_short_circuit_result_n64 = hxc_l_guideInteractionAvailable;
    }
    if (hxc_l_tmp_short_circuit_result_n64)
    {
      struct Color hxc_l_this1_h78adce0fcb63 = (struct Color){ .r = 15, .g = 27, .b = 39, .a = 220 };
      int32_t hxc_l_tmp_load_result_n188 = hxc_l_centerX;
      int32_t hxc_l_tmp_load_result_n189 = hxc_l_centerY;
      DrawRectangle((int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n188, 260), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n189, 54), (int32_t)520, (int32_t)60, hxc_l_this1_h78adce0fcb63);
      hxc_string hxc_l_tmp_call_result_n191 = hxc_caxecraft_app_GameplayMessage_gameplayMessageId(11);
      hxc_l_text1_h49616dfec549 = hxc_l_tmp_call_result_n191;
      hxc_string hxc_l_text2_h0e0c15cc7960 = { 0 };
      switch (hxc_l_locale) {
        case 0:
          {
            hxc_l_text2_h0e0c15cc7960 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
            if (hxc_string_retain(hxc_l_text2_h0e0c15cc7960) != HXC_STATUS_OK)
            {
              abort();
            }
            break;
          }
        case 1:
          {
            hxc_l_text2_h0e0c15cc7960 = (hxc_string){ (const uint8_t *)"es-mx", 5, true, NULL };
            if (hxc_string_retain(hxc_l_text2_h0e0c15cc7960) != HXC_STATUS_OK)
            {
              abort();
            }
            break;
          }
        default:
          {
            hxc_l_text2_h0e0c15cc7960 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
            if (hxc_string_retain(hxc_l_text2_h0e0c15cc7960) != HXC_STATUS_OK)
            {
              abort();
            }
            break;
          }
      }
      hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n193 = hxc_l_text2_h0e0c15cc7960;
      hxc_l_tmp_managed_flow_owner_n68 = hxc_l_tmp_managed_flow_carrier_move_result_n193;
      struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_load_result_n194 = hxc_l_presentation;
      hxc_l_gc_roots[5] = (const void *)hxc_l_tmp_load_result_n194;
      if (hxc_l_tmp_load_result_n194 == NULL)
      {
        abort();
      }
      hxc_string hxc_l_tmp_load_result_n195 = hxc_l_text1_h49616dfec549;
      hxc_string hxc_l_tmp_instance_call_result_n197 = hxc_caxecraft_content_RuntimeLevelPresentation_message(hxc_l_tmp_load_result_n194, hxc_l_tmp_load_result_n195, hxc_l_tmp_managed_flow_owner_n68);
      hxc_l_tmp_cstring_ref_owner_owner_n69 = hxc_l_tmp_instance_call_result_n197;
      hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n199 = HXC_BORROWED_CSTRING_INITIALIZER;
      if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n69, &hxc_l_tmp_string_borrow_cstring_result_n199) != HXC_STATUS_OK)
      {
        abort();
      }
      int32_t hxc_l_tmp_load_result_n200 = hxc_l_centerX;
      int32_t hxc_l_tmp_load_result_n201 = hxc_l_centerY;
      DrawText(hxc_l_tmp_string_borrow_cstring_result_n199.data, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n200, 110), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n201, 74), (int32_t)18, hxc_l_text_h774b5c1af5bc);
      if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n69) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n68) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_text1_h49616dfec549) != HXC_STATUS_OK)
      {
        abort();
      }
    }
  }
  if (hxc_l_enemy.hxc_vitals.hxc_health > 0)
  {
    if (hxc_l_enemyPhase == hxc_caxecraft_domain_ActorControllerPhase_Windup)
    {
      struct Color hxc_l_color_hf83254c34ee7 = (struct Color){ .r = 218, .g = 65, .b = 72, .a = 255 };
      hxc_string hxc_l_tmp_call_result_n207 = hxc_caxecraft_app_GameplayMessage_gameplayMessageId(9);
      hxc_l_text_hd61815552d14 = hxc_l_tmp_call_result_n207;
      hxc_string hxc_l_text1_h6b31d160539d = { 0 };
      switch (hxc_l_locale) {
        case 0:
          {
            hxc_l_text1_h6b31d160539d = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
            if (hxc_string_retain(hxc_l_text1_h6b31d160539d) != HXC_STATUS_OK)
            {
              abort();
            }
            break;
          }
        case 1:
          {
            hxc_l_text1_h6b31d160539d = (hxc_string){ (const uint8_t *)"es-mx", 5, true, NULL };
            if (hxc_string_retain(hxc_l_text1_h6b31d160539d) != HXC_STATUS_OK)
            {
              abort();
            }
            break;
          }
        default:
          {
            hxc_l_text1_h6b31d160539d = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
            if (hxc_string_retain(hxc_l_text1_h6b31d160539d) != HXC_STATUS_OK)
            {
              abort();
            }
            break;
          }
      }
      hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n209 = hxc_l_text1_h6b31d160539d;
      hxc_l_tmp_managed_flow_owner_n73 = hxc_l_tmp_managed_flow_carrier_move_result_n209;
      struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_load_result_n210 = hxc_l_presentation;
      hxc_l_gc_roots[6] = (const void *)hxc_l_tmp_load_result_n210;
      if (hxc_l_tmp_load_result_n210 == NULL)
      {
        abort();
      }
      hxc_string hxc_l_tmp_load_result_n211 = hxc_l_text_hd61815552d14;
      hxc_string hxc_l_tmp_instance_call_result_n213 = hxc_caxecraft_content_RuntimeLevelPresentation_message(hxc_l_tmp_load_result_n210, hxc_l_tmp_load_result_n211, hxc_l_tmp_managed_flow_owner_n73);
      hxc_l_tmp_cstring_ref_owner_owner_n74 = hxc_l_tmp_instance_call_result_n213;
      hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n215 = HXC_BORROWED_CSTRING_INITIALIZER;
      if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n74, &hxc_l_tmp_string_borrow_cstring_result_n215) != HXC_STATUS_OK)
      {
        abort();
      }
      int32_t hxc_l_tmp_load_result_n216 = hxc_l_width;
      DrawText(hxc_l_tmp_string_borrow_cstring_result_n215.data, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n216, 300), (int32_t)28, (int32_t)16, hxc_l_color_hf83254c34ee7);
      if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n74) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n73) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_text_hd61815552d14) != HXC_STATUS_OK)
      {
        abort();
      }
    }
    else
    {
      if (hxc_l_enemyPhase == hxc_caxecraft_domain_ActorControllerPhase_Chasing)
      {
        struct Color hxc_l_color_hebd63cac65de = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
        hxc_string hxc_l_tmp_call_result_n221 = hxc_caxecraft_app_GameplayMessage_gameplayMessageId(7);
        hxc_l_text_had7867538e76 = hxc_l_tmp_call_result_n221;
        hxc_string hxc_l_text1_h48d0f3b197d7 = { 0 };
        switch (hxc_l_locale) {
          case 0:
            {
              hxc_l_text1_h48d0f3b197d7 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
              if (hxc_string_retain(hxc_l_text1_h48d0f3b197d7) != HXC_STATUS_OK)
              {
                abort();
              }
              break;
            }
          case 1:
            {
              hxc_l_text1_h48d0f3b197d7 = (hxc_string){ (const uint8_t *)"es-mx", 5, true, NULL };
              if (hxc_string_retain(hxc_l_text1_h48d0f3b197d7) != HXC_STATUS_OK)
              {
                abort();
              }
              break;
            }
          default:
            {
              hxc_l_text1_h48d0f3b197d7 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
              if (hxc_string_retain(hxc_l_text1_h48d0f3b197d7) != HXC_STATUS_OK)
              {
                abort();
              }
              break;
            }
        }
        hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n223 = hxc_l_text1_h48d0f3b197d7;
        hxc_l_tmp_managed_flow_owner_n78 = hxc_l_tmp_managed_flow_carrier_move_result_n223;
        struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_load_result_n224 = hxc_l_presentation;
        hxc_l_gc_roots[7] = (const void *)hxc_l_tmp_load_result_n224;
        if (hxc_l_tmp_load_result_n224 == NULL)
        {
          abort();
        }
        hxc_string hxc_l_tmp_load_result_n225 = hxc_l_text_had7867538e76;
        hxc_string hxc_l_tmp_instance_call_result_n227 = hxc_caxecraft_content_RuntimeLevelPresentation_message(hxc_l_tmp_load_result_n224, hxc_l_tmp_load_result_n225, hxc_l_tmp_managed_flow_owner_n78);
        hxc_l_tmp_cstring_ref_owner_owner_n79 = hxc_l_tmp_instance_call_result_n227;
        hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n229 = HXC_BORROWED_CSTRING_INITIALIZER;
        if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n79, &hxc_l_tmp_string_borrow_cstring_result_n229) != HXC_STATUS_OK)
        {
          abort();
        }
        int32_t hxc_l_tmp_load_result_n230 = hxc_l_width;
        DrawText(hxc_l_tmp_string_borrow_cstring_result_n229.data, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n230, 180), (int32_t)28, (int32_t)16, hxc_l_color_hebd63cac65de);
        if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n79) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n78) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_string_release(&hxc_l_text_had7867538e76) != HXC_STATUS_OK)
        {
          abort();
        }
      }
    }
  }
  if (hxc_l_strikeHit)
  {
    struct Color hxc_l_color_hefdafd34dc12 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
    hxc_string hxc_l_tmp_call_result_n234 = hxc_caxecraft_app_GameplayMessage_gameplayMessageId(5);
    hxc_l_text_hc297d493ace3 = hxc_l_tmp_call_result_n234;
    hxc_string hxc_l_text1_h6386656cfeca = { 0 };
    switch (hxc_l_locale) {
      case 0:
        {
          hxc_l_text1_h6386656cfeca = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
          if (hxc_string_retain(hxc_l_text1_h6386656cfeca) != HXC_STATUS_OK)
          {
            abort();
          }
          break;
        }
      case 1:
        {
          hxc_l_text1_h6386656cfeca = (hxc_string){ (const uint8_t *)"es-mx", 5, true, NULL };
          if (hxc_string_retain(hxc_l_text1_h6386656cfeca) != HXC_STATUS_OK)
          {
            abort();
          }
          break;
        }
      default:
        {
          hxc_l_text1_h6386656cfeca = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
          if (hxc_string_retain(hxc_l_text1_h6386656cfeca) != HXC_STATUS_OK)
          {
            abort();
          }
          break;
        }
    }
    hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n236 = hxc_l_text1_h6386656cfeca;
    hxc_l_tmp_managed_flow_owner_n83 = hxc_l_tmp_managed_flow_carrier_move_result_n236;
    struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_load_result_n237 = hxc_l_presentation;
    hxc_l_gc_roots[8] = (const void *)hxc_l_tmp_load_result_n237;
    if (hxc_l_tmp_load_result_n237 == NULL)
    {
      abort();
    }
    hxc_string hxc_l_tmp_load_result_n238 = hxc_l_text_hc297d493ace3;
    hxc_string hxc_l_tmp_instance_call_result_n240 = hxc_caxecraft_content_RuntimeLevelPresentation_message(hxc_l_tmp_load_result_n237, hxc_l_tmp_load_result_n238, hxc_l_tmp_managed_flow_owner_n83);
    hxc_l_tmp_cstring_ref_owner_owner_n84 = hxc_l_tmp_instance_call_result_n240;
    hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n242 = HXC_BORROWED_CSTRING_INITIALIZER;
    if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n84, &hxc_l_tmp_string_borrow_cstring_result_n242) != HXC_STATUS_OK)
    {
      abort();
    }
    int32_t hxc_l_tmp_load_result_n243 = hxc_l_centerX;
    int32_t hxc_l_tmp_load_result_n244 = hxc_l_centerY;
    DrawText(hxc_l_tmp_string_borrow_cstring_result_n242.data, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n243, 70), (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n244, 54), (int32_t)18, hxc_l_color_hefdafd34dc12);
    if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n84) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n83) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_text_hc297d493ace3) != HXC_STATUS_OK)
    {
      abort();
    }
  }
  if (hxc_l_enemyDefeated)
  {
    struct Color hxc_l_color_h0e9c70bbccb8 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
    hxc_string hxc_l_tmp_call_result_n248 = hxc_caxecraft_app_GameplayMessage_gameplayMessageId(8);
    hxc_l_text_h22b2ee51f74d = hxc_l_tmp_call_result_n248;
    hxc_string hxc_l_text1_h7650e09f174b = { 0 };
    switch (hxc_l_locale) {
      case 0:
        {
          hxc_l_text1_h7650e09f174b = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
          if (hxc_string_retain(hxc_l_text1_h7650e09f174b) != HXC_STATUS_OK)
          {
            abort();
          }
          break;
        }
      case 1:
        {
          hxc_l_text1_h7650e09f174b = (hxc_string){ (const uint8_t *)"es-mx", 5, true, NULL };
          if (hxc_string_retain(hxc_l_text1_h7650e09f174b) != HXC_STATUS_OK)
          {
            abort();
          }
          break;
        }
      default:
        {
          hxc_l_text1_h7650e09f174b = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
          if (hxc_string_retain(hxc_l_text1_h7650e09f174b) != HXC_STATUS_OK)
          {
            abort();
          }
          break;
        }
    }
    hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n250 = hxc_l_text1_h7650e09f174b;
    hxc_l_tmp_managed_flow_owner_n88 = hxc_l_tmp_managed_flow_carrier_move_result_n250;
    struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_load_result_n251 = hxc_l_presentation;
    hxc_l_gc_roots[9] = (const void *)hxc_l_tmp_load_result_n251;
    if (hxc_l_tmp_load_result_n251 == NULL)
    {
      abort();
    }
    hxc_string hxc_l_tmp_load_result_n252 = hxc_l_text_h22b2ee51f74d;
    hxc_string hxc_l_tmp_instance_call_result_n254 = hxc_caxecraft_content_RuntimeLevelPresentation_message(hxc_l_tmp_load_result_n251, hxc_l_tmp_load_result_n252, hxc_l_tmp_managed_flow_owner_n88);
    hxc_l_tmp_cstring_ref_owner_owner_n89 = hxc_l_tmp_instance_call_result_n254;
    hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n256 = HXC_BORROWED_CSTRING_INITIALIZER;
    if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n89, &hxc_l_tmp_string_borrow_cstring_result_n256) != HXC_STATUS_OK)
    {
      abort();
    }
    int32_t hxc_l_tmp_load_result_n257 = hxc_l_width;
    DrawText(hxc_l_tmp_string_borrow_cstring_result_n256.data, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n257, 285), (int32_t)54, (int32_t)16, hxc_l_color_h0e9c70bbccb8);
    if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n89) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n88) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_text_h22b2ee51f74d) != HXC_STATUS_OK)
    {
      abort();
    }
  }
  if (hxc_l_enemyAttacked)
  {
    struct Color hxc_l_color_h9f682169deb3 = (struct Color){ .r = 218, .g = 65, .b = 72, .a = 255 };
    hxc_string hxc_l_tmp_call_result_n261 = hxc_caxecraft_app_GameplayMessage_gameplayMessageId(15);
    hxc_l_text_ha2bb5953880b = hxc_l_tmp_call_result_n261;
    hxc_string hxc_l_text1_h9c574af8a460 = { 0 };
    switch (hxc_l_locale) {
      case 0:
        {
          hxc_l_text1_h9c574af8a460 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
          if (hxc_string_retain(hxc_l_text1_h9c574af8a460) != HXC_STATUS_OK)
          {
            abort();
          }
          break;
        }
      case 1:
        {
          hxc_l_text1_h9c574af8a460 = (hxc_string){ (const uint8_t *)"es-mx", 5, true, NULL };
          if (hxc_string_retain(hxc_l_text1_h9c574af8a460) != HXC_STATUS_OK)
          {
            abort();
          }
          break;
        }
      default:
        {
          hxc_l_text1_h9c574af8a460 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
          if (hxc_string_retain(hxc_l_text1_h9c574af8a460) != HXC_STATUS_OK)
          {
            abort();
          }
          break;
        }
    }
    hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n263 = hxc_l_text1_h9c574af8a460;
    hxc_l_tmp_managed_flow_owner_n93 = hxc_l_tmp_managed_flow_carrier_move_result_n263;
    struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_load_result_n264 = hxc_l_presentation;
    hxc_l_gc_roots[10] = (const void *)hxc_l_tmp_load_result_n264;
    if (hxc_l_tmp_load_result_n264 == NULL)
    {
      abort();
    }
    hxc_string hxc_l_tmp_load_result_n265 = hxc_l_text_ha2bb5953880b;
    hxc_string hxc_l_tmp_instance_call_result_n267 = hxc_caxecraft_content_RuntimeLevelPresentation_message(hxc_l_tmp_load_result_n264, hxc_l_tmp_load_result_n265, hxc_l_tmp_managed_flow_owner_n93);
    hxc_l_tmp_cstring_ref_owner_owner_n94 = hxc_l_tmp_instance_call_result_n267;
    hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n269 = HXC_BORROWED_CSTRING_INITIALIZER;
    if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n94, &hxc_l_tmp_string_borrow_cstring_result_n269) != HXC_STATUS_OK)
    {
      abort();
    }
    int32_t hxc_l_tmp_load_result_n270 = hxc_l_width;
    DrawText(hxc_l_tmp_string_borrow_cstring_result_n269.data, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n270, 330), (int32_t)82, (int32_t)16, hxc_l_color_h9f682169deb3);
    if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n94) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n93) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_text_ha2bb5953880b) != HXC_STATUS_OK)
    {
      abort();
    }
  }
  if (hxc_l_pickedUp)
  {
    int32_t hxc_l_tmp_load_result_n273 = hxc_l_pickupAmount;
    int32_t hxc_l_tmp_conditional_result_n96 = 0;
    if (hxc_l_tmp_load_result_n273 == 1)
    {
      hxc_l_tmp_conditional_result_n96 = 0;
    }
    else
    {
      hxc_l_tmp_conditional_result_n96 = 1;
    }
    int32_t hxc_l_pickupMessage = hxc_l_tmp_conditional_result_n96;
    struct Color hxc_l_color_h70f37734bccc = (struct Color){ .r = 174, .g = 78, .b = 136, .a = 255 };
    hxc_string hxc_l_tmp_call_result_n277 = hxc_caxecraft_app_GameplayMessage_gameplayMessageId(hxc_l_pickupMessage);
    hxc_l_text_h5f0724396197 = hxc_l_tmp_call_result_n277;
    hxc_string hxc_l_text1_h2e009be06740 = { 0 };
    switch (hxc_l_locale) {
      case 0:
        {
          hxc_l_text1_h2e009be06740 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
          if (hxc_string_retain(hxc_l_text1_h2e009be06740) != HXC_STATUS_OK)
          {
            abort();
          }
          break;
        }
      case 1:
        {
          hxc_l_text1_h2e009be06740 = (hxc_string){ (const uint8_t *)"es-mx", 5, true, NULL };
          if (hxc_string_retain(hxc_l_text1_h2e009be06740) != HXC_STATUS_OK)
          {
            abort();
          }
          break;
        }
      default:
        {
          hxc_l_text1_h2e009be06740 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
          if (hxc_string_retain(hxc_l_text1_h2e009be06740) != HXC_STATUS_OK)
          {
            abort();
          }
          break;
        }
    }
    hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n279 = hxc_l_text1_h2e009be06740;
    hxc_l_tmp_managed_flow_owner_n100 = hxc_l_tmp_managed_flow_carrier_move_result_n279;
    struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_load_result_n280 = hxc_l_presentation;
    hxc_l_gc_roots[11] = (const void *)hxc_l_tmp_load_result_n280;
    if (hxc_l_tmp_load_result_n280 == NULL)
    {
      abort();
    }
    hxc_string hxc_l_tmp_load_result_n281 = hxc_l_text_h5f0724396197;
    hxc_string hxc_l_tmp_instance_call_result_n283 = hxc_caxecraft_content_RuntimeLevelPresentation_message(hxc_l_tmp_load_result_n280, hxc_l_tmp_load_result_n281, hxc_l_tmp_managed_flow_owner_n100);
    hxc_l_tmp_cstring_ref_owner_owner_n101 = hxc_l_tmp_instance_call_result_n283;
    hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n285 = HXC_BORROWED_CSTRING_INITIALIZER;
    if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n101, &hxc_l_tmp_string_borrow_cstring_result_n285) != HXC_STATUS_OK)
    {
      abort();
    }
    int32_t hxc_l_tmp_load_result_n286 = hxc_l_centerX;
    int32_t hxc_l_tmp_load_result_n287 = hxc_l_centerY;
    DrawText(hxc_l_tmp_string_borrow_cstring_result_n285.data, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n286, 48), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n287, 24), (int32_t)18, hxc_l_color_h70f37734bccc);
    if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n101) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n100) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_text_h5f0724396197) != HXC_STATUS_OK)
    {
      abort();
    }
  }
  if (hxc_l_inventoryFullReason == 1)
  {
    struct Color hxc_l_color_hff6c78f923f7 = (struct Color){ .r = 238, .g = 113, .b = 78, .a = 255 };
    hxc_string hxc_l_tmp_call_result_n291 = hxc_caxecraft_app_GameplayMessage_gameplayMessageId(3);
    hxc_l_text_h7ba79ce41ad0 = hxc_l_tmp_call_result_n291;
    hxc_string hxc_l_text1_h46d05efc080e = { 0 };
    switch (hxc_l_locale) {
      case 0:
        {
          hxc_l_text1_h46d05efc080e = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
          if (hxc_string_retain(hxc_l_text1_h46d05efc080e) != HXC_STATUS_OK)
          {
            abort();
          }
          break;
        }
      case 1:
        {
          hxc_l_text1_h46d05efc080e = (hxc_string){ (const uint8_t *)"es-mx", 5, true, NULL };
          if (hxc_string_retain(hxc_l_text1_h46d05efc080e) != HXC_STATUS_OK)
          {
            abort();
          }
          break;
        }
      default:
        {
          hxc_l_text1_h46d05efc080e = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
          if (hxc_string_retain(hxc_l_text1_h46d05efc080e) != HXC_STATUS_OK)
          {
            abort();
          }
          break;
        }
    }
    hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n293 = hxc_l_text1_h46d05efc080e;
    hxc_l_tmp_managed_flow_owner_n105 = hxc_l_tmp_managed_flow_carrier_move_result_n293;
    struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_load_result_n294 = hxc_l_presentation;
    hxc_l_gc_roots[12] = (const void *)hxc_l_tmp_load_result_n294;
    if (hxc_l_tmp_load_result_n294 == NULL)
    {
      abort();
    }
    hxc_string hxc_l_tmp_load_result_n295 = hxc_l_text_h7ba79ce41ad0;
    hxc_string hxc_l_tmp_instance_call_result_n297 = hxc_caxecraft_content_RuntimeLevelPresentation_message(hxc_l_tmp_load_result_n294, hxc_l_tmp_load_result_n295, hxc_l_tmp_managed_flow_owner_n105);
    hxc_l_tmp_cstring_ref_owner_owner_n106 = hxc_l_tmp_instance_call_result_n297;
    hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n299 = HXC_BORROWED_CSTRING_INITIALIZER;
    if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n106, &hxc_l_tmp_string_borrow_cstring_result_n299) != HXC_STATUS_OK)
    {
      abort();
    }
    int32_t hxc_l_tmp_load_result_n300 = hxc_l_centerX;
    int32_t hxc_l_tmp_load_result_n301 = hxc_l_centerY;
    DrawText(hxc_l_tmp_string_borrow_cstring_result_n299.data, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n300, 150), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n301, 48), (int32_t)16, hxc_l_color_hff6c78f923f7);
    if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n106) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n105) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_text_h7ba79ce41ad0) != HXC_STATUS_OK)
    {
      abort();
    }
  }
  else
  {
    if (hxc_l_inventoryFullReason == 2)
    {
      struct Color hxc_l_color_hf7c0ca1b0243 = (struct Color){ .r = 238, .g = 113, .b = 78, .a = 255 };
      hxc_string hxc_l_tmp_call_result_n305 = hxc_caxecraft_app_GameplayMessage_gameplayMessageId(4);
      hxc_l_text_ha3dc2968044e = hxc_l_tmp_call_result_n305;
      hxc_string hxc_l_text1_h8832945ee298 = { 0 };
      switch (hxc_l_locale) {
        case 0:
          {
            hxc_l_text1_h8832945ee298 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
            if (hxc_string_retain(hxc_l_text1_h8832945ee298) != HXC_STATUS_OK)
            {
              abort();
            }
            break;
          }
        case 1:
          {
            hxc_l_text1_h8832945ee298 = (hxc_string){ (const uint8_t *)"es-mx", 5, true, NULL };
            if (hxc_string_retain(hxc_l_text1_h8832945ee298) != HXC_STATUS_OK)
            {
              abort();
            }
            break;
          }
        default:
          {
            hxc_l_text1_h8832945ee298 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
            if (hxc_string_retain(hxc_l_text1_h8832945ee298) != HXC_STATUS_OK)
            {
              abort();
            }
            break;
          }
      }
      hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n307 = hxc_l_text1_h8832945ee298;
      hxc_l_tmp_managed_flow_owner_n110 = hxc_l_tmp_managed_flow_carrier_move_result_n307;
      struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_load_result_n308 = hxc_l_presentation;
      hxc_l_gc_roots[13] = (const void *)hxc_l_tmp_load_result_n308;
      if (hxc_l_tmp_load_result_n308 == NULL)
      {
        abort();
      }
      hxc_string hxc_l_tmp_load_result_n309 = hxc_l_text_ha3dc2968044e;
      hxc_string hxc_l_tmp_instance_call_result_n311 = hxc_caxecraft_content_RuntimeLevelPresentation_message(hxc_l_tmp_load_result_n308, hxc_l_tmp_load_result_n309, hxc_l_tmp_managed_flow_owner_n110);
      hxc_l_tmp_cstring_ref_owner_owner_n111 = hxc_l_tmp_instance_call_result_n311;
      hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n313 = HXC_BORROWED_CSTRING_INITIALIZER;
      if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n111, &hxc_l_tmp_string_borrow_cstring_result_n313) != HXC_STATUS_OK)
      {
        abort();
      }
      int32_t hxc_l_tmp_load_result_n314 = hxc_l_centerX;
      int32_t hxc_l_tmp_load_result_n315 = hxc_l_centerY;
      DrawText(hxc_l_tmp_string_borrow_cstring_result_n313.data, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n314, 155), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n315, 48), (int32_t)16, hxc_l_color_hf7c0ca1b0243);
      if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n111) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n110) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_text_ha3dc2968044e) != HXC_STATUS_OK)
      {
        abort();
      }
    }
  }
  if (hxc_l_recoveryVisible)
  {
    if (hxc_l_recoveryFeedback == 1)
    {
      struct Color hxc_l_color_h6266913df5a6 = (struct Color){ .r = 94, .g = 212, .b = 136, .a = 255 };
      hxc_string hxc_l_tmp_call_result_n320 = hxc_caxecraft_app_GameplayMessage_gameplayMessageId(2);
      hxc_l_text_h1bfa3b41bef0 = hxc_l_tmp_call_result_n320;
      hxc_string hxc_l_text1_h06d57c2cbebb = { 0 };
      switch (hxc_l_locale) {
        case 0:
          {
            hxc_l_text1_h06d57c2cbebb = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
            if (hxc_string_retain(hxc_l_text1_h06d57c2cbebb) != HXC_STATUS_OK)
            {
              abort();
            }
            break;
          }
        case 1:
          {
            hxc_l_text1_h06d57c2cbebb = (hxc_string){ (const uint8_t *)"es-mx", 5, true, NULL };
            if (hxc_string_retain(hxc_l_text1_h06d57c2cbebb) != HXC_STATUS_OK)
            {
              abort();
            }
            break;
          }
        default:
          {
            hxc_l_text1_h06d57c2cbebb = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
            if (hxc_string_retain(hxc_l_text1_h06d57c2cbebb) != HXC_STATUS_OK)
            {
              abort();
            }
            break;
          }
      }
      hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n322 = hxc_l_text1_h06d57c2cbebb;
      hxc_l_tmp_managed_flow_owner_n115 = hxc_l_tmp_managed_flow_carrier_move_result_n322;
      struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_load_result_n323 = hxc_l_presentation;
      hxc_l_gc_roots[14] = (const void *)hxc_l_tmp_load_result_n323;
      if (hxc_l_tmp_load_result_n323 == NULL)
      {
        abort();
      }
      hxc_string hxc_l_tmp_load_result_n324 = hxc_l_text_h1bfa3b41bef0;
      hxc_string hxc_l_tmp_instance_call_result_n326 = hxc_caxecraft_content_RuntimeLevelPresentation_message(hxc_l_tmp_load_result_n323, hxc_l_tmp_load_result_n324, hxc_l_tmp_managed_flow_owner_n115);
      hxc_l_tmp_cstring_ref_owner_owner_n116 = hxc_l_tmp_instance_call_result_n326;
      hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n328 = HXC_BORROWED_CSTRING_INITIALIZER;
      if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n116, &hxc_l_tmp_string_borrow_cstring_result_n328) != HXC_STATUS_OK)
      {
        abort();
      }
      int32_t hxc_l_tmp_load_result_n329 = hxc_l_centerX;
      int32_t hxc_l_tmp_load_result_n330 = hxc_l_centerY;
      DrawText(hxc_l_tmp_string_borrow_cstring_result_n328.data, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n329, 88), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n330, 24), (int32_t)18, hxc_l_color_h6266913df5a6);
      if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n116) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n115) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_text_h1bfa3b41bef0) != HXC_STATUS_OK)
      {
        abort();
      }
    }
    else
    {
      if (hxc_l_recoveryFeedback == 2)
      {
        struct Color hxc_l_this1_h81c8f9e6a7e9 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
        hxc_string hxc_l_tmp_instance_call_result_n335 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_uiCatalog, hxc_l_locale, 25);
        hxc_l_tmp_cstring_ref_owner_owner_n118 = hxc_l_tmp_instance_call_result_n335;
        hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n337 = HXC_BORROWED_CSTRING_INITIALIZER;
        if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n118, &hxc_l_tmp_string_borrow_cstring_result_n337) != HXC_STATUS_OK)
        {
          abort();
        }
        int32_t hxc_l_tmp_load_result_n338 = hxc_l_centerX;
        int32_t hxc_l_tmp_load_result_n339 = hxc_l_centerY;
        DrawText(hxc_l_tmp_string_borrow_cstring_result_n337.data, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n338, 96), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n339, 24), (int32_t)18, hxc_l_this1_h81c8f9e6a7e9);
        if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n118) != HXC_STATUS_OK)
        {
          abort();
        }
      }
      else
      {
        if (hxc_l_recoveryFeedback == 3)
        {
          struct Color hxc_l_color_hdaf46fe325e2 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
          hxc_string hxc_l_tmp_call_result_n343 = hxc_caxecraft_app_GameplayMessage_gameplayMessageId(13);
          hxc_l_text_h51828ddbb0fa = hxc_l_tmp_call_result_n343;
          hxc_string hxc_l_text1_h93f8b6e5564a = { 0 };
          switch (hxc_l_locale) {
            case 0:
              {
                hxc_l_text1_h93f8b6e5564a = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
                if (hxc_string_retain(hxc_l_text1_h93f8b6e5564a) != HXC_STATUS_OK)
                {
                  abort();
                }
                break;
              }
            case 1:
              {
                hxc_l_text1_h93f8b6e5564a = (hxc_string){ (const uint8_t *)"es-mx", 5, true, NULL };
                if (hxc_string_retain(hxc_l_text1_h93f8b6e5564a) != HXC_STATUS_OK)
                {
                  abort();
                }
                break;
              }
            default:
              {
                hxc_l_text1_h93f8b6e5564a = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
                if (hxc_string_retain(hxc_l_text1_h93f8b6e5564a) != HXC_STATUS_OK)
                {
                  abort();
                }
                break;
              }
          }
          hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n345 = hxc_l_text1_h93f8b6e5564a;
          hxc_l_tmp_managed_flow_owner_n122 = hxc_l_tmp_managed_flow_carrier_move_result_n345;
          struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_load_result_n346 = hxc_l_presentation;
          hxc_l_gc_roots[15] = (const void *)hxc_l_tmp_load_result_n346;
          if (hxc_l_tmp_load_result_n346 == NULL)
          {
            abort();
          }
          hxc_string hxc_l_tmp_load_result_n347 = hxc_l_text_h51828ddbb0fa;
          hxc_string hxc_l_tmp_instance_call_result_n349 = hxc_caxecraft_content_RuntimeLevelPresentation_message(hxc_l_tmp_load_result_n346, hxc_l_tmp_load_result_n347, hxc_l_tmp_managed_flow_owner_n122);
          hxc_l_tmp_cstring_ref_owner_owner_n123 = hxc_l_tmp_instance_call_result_n349;
          hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n351 = HXC_BORROWED_CSTRING_INITIALIZER;
          if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n123, &hxc_l_tmp_string_borrow_cstring_result_n351) != HXC_STATUS_OK)
          {
            abort();
          }
          int32_t hxc_l_tmp_load_result_n352 = hxc_l_centerX;
          int32_t hxc_l_tmp_load_result_n353 = hxc_l_centerY;
          DrawText(hxc_l_tmp_string_borrow_cstring_result_n351.data, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n352, 76), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n353, 24), (int32_t)18, hxc_l_color_hdaf46fe325e2);
          if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n123) != HXC_STATUS_OK)
          {
            abort();
          }
          if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n122) != HXC_STATUS_OK)
          {
            abort();
          }
          if (hxc_string_release(&hxc_l_text_h51828ddbb0fa) != HXC_STATUS_OK)
          {
            abort();
          }
        }
      }
    }
  }
  if (hxc_l_aquaticEquipmentVisible)
  {
    struct Color hxc_l_this1_hfb075f4c0648 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
    hxc_string hxc_l_tmp_instance_call_result_n358 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_uiCatalog, hxc_l_locale, 0);
    hxc_l_tmp_cstring_ref_owner_owner_n125 = hxc_l_tmp_instance_call_result_n358;
    hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n360 = HXC_BORROWED_CSTRING_INITIALIZER;
    if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n125, &hxc_l_tmp_string_borrow_cstring_result_n360) != HXC_STATUS_OK)
    {
      abort();
    }
    int32_t hxc_l_tmp_load_result_n361 = hxc_l_centerX;
    int32_t hxc_l_tmp_load_result_n362 = hxc_l_centerY;
    DrawText(hxc_l_tmp_string_borrow_cstring_result_n360.data, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n361, 128), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n362, 24), (int32_t)18, hxc_l_this1_hfb075f4c0648);
    if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n125) != HXC_STATUS_OK)
    {
      abort();
    }
  }
  if (hxc_l_vitals.hxc_safeTicks > 15)
  {
    struct Color hxc_l_this1_hd2874c1ba736 = (struct Color){ .r = 218, .g = 65, .b = 72, .a = 255 };
    int32_t hxc_l_tmp_load_result_n366 = hxc_l_width;
    int32_t hxc_l_tmp_load_result_n367 = hxc_l_height;
    DrawRectangleLines((int32_t)4, (int32_t)4, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n366, 8), (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n367, 8), hxc_l_this1_hd2874c1ba736);
  }
  if (hxc_l_vitals.hxc_health <= 0)
  {
    struct Color hxc_l_this1_h04d3ca08a7c8 = (struct Color){ .r = 15, .g = 27, .b = 39, .a = 220 };
    int32_t hxc_l_tmp_load_result_n371 = hxc_l_centerX;
    int32_t hxc_l_tmp_load_result_n372 = hxc_l_centerY;
    DrawRectangle((int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n371, 250), (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n372, 74), (int32_t)500, (int32_t)148, hxc_l_this1_h04d3ca08a7c8);
    struct Color hxc_l_this1_h3815581f9d16 = (struct Color){ .r = 218, .g = 65, .b = 72, .a = 255 };
    int32_t hxc_l_tmp_load_result_n375 = hxc_l_centerX;
    int32_t hxc_l_tmp_load_result_n376 = hxc_l_centerY;
    DrawRectangleLines((int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n375, 250), (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n376, 74), (int32_t)500, (int32_t)148, hxc_l_this1_h3815581f9d16);
    hxc_string hxc_l_tmp_call_result_n378 = hxc_caxecraft_app_GameplayMessage_gameplayMessageId(6);
    hxc_l_text1_h6cdf5fedfbfd = hxc_l_tmp_call_result_n378;
    hxc_string hxc_l_text2_ha14803ce6589 = { 0 };
    switch (hxc_l_locale) {
      case 0:
        {
          hxc_l_text2_ha14803ce6589 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
          if (hxc_string_retain(hxc_l_text2_ha14803ce6589) != HXC_STATUS_OK)
          {
            abort();
          }
          break;
        }
      case 1:
        {
          hxc_l_text2_ha14803ce6589 = (hxc_string){ (const uint8_t *)"es-mx", 5, true, NULL };
          if (hxc_string_retain(hxc_l_text2_ha14803ce6589) != HXC_STATUS_OK)
          {
            abort();
          }
          break;
        }
      default:
        {
          hxc_l_text2_ha14803ce6589 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
          if (hxc_string_retain(hxc_l_text2_ha14803ce6589) != HXC_STATUS_OK)
          {
            abort();
          }
          break;
        }
    }
    hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n380 = hxc_l_text2_ha14803ce6589;
    hxc_l_tmp_managed_flow_owner_n131 = hxc_l_tmp_managed_flow_carrier_move_result_n380;
    struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_load_result_n381 = hxc_l_presentation;
    hxc_l_gc_roots[16] = (const void *)hxc_l_tmp_load_result_n381;
    if (hxc_l_tmp_load_result_n381 == NULL)
    {
      abort();
    }
    hxc_string hxc_l_tmp_load_result_n382 = hxc_l_text1_h6cdf5fedfbfd;
    hxc_string hxc_l_tmp_instance_call_result_n384 = hxc_caxecraft_content_RuntimeLevelPresentation_message(hxc_l_tmp_load_result_n381, hxc_l_tmp_load_result_n382, hxc_l_tmp_managed_flow_owner_n131);
    hxc_l_tmp_cstring_ref_owner_owner_n132 = hxc_l_tmp_instance_call_result_n384;
    hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n386 = HXC_BORROWED_CSTRING_INITIALIZER;
    if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n132, &hxc_l_tmp_string_borrow_cstring_result_n386) != HXC_STATUS_OK)
    {
      abort();
    }
    int32_t hxc_l_tmp_load_result_n387 = hxc_l_centerX;
    int32_t hxc_l_tmp_load_result_n388 = hxc_l_centerY;
    DrawText(hxc_l_tmp_string_borrow_cstring_result_n386.data, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n387, 122), (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n388, 42), (int32_t)24, hxc_l_text_h774b5c1af5bc);
    struct Color hxc_l_color_hdc49e94c17f0 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
    hxc_string hxc_l_tmp_call_result_n391 = hxc_caxecraft_app_GameplayMessage_gameplayMessageId(14);
    hxc_l_text_h409c3cedb1e0 = hxc_l_tmp_call_result_n391;
    hxc_string hxc_l_text1_hbba66327ca7e = { 0 };
    switch (hxc_l_locale) {
      case 0:
        {
          hxc_l_text1_hbba66327ca7e = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
          if (hxc_string_retain(hxc_l_text1_hbba66327ca7e) != HXC_STATUS_OK)
          {
            abort();
          }
          break;
        }
      case 1:
        {
          hxc_l_text1_hbba66327ca7e = (hxc_string){ (const uint8_t *)"es-mx", 5, true, NULL };
          if (hxc_string_retain(hxc_l_text1_hbba66327ca7e) != HXC_STATUS_OK)
          {
            abort();
          }
          break;
        }
      default:
        {
          hxc_l_text1_hbba66327ca7e = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
          if (hxc_string_retain(hxc_l_text1_hbba66327ca7e) != HXC_STATUS_OK)
          {
            abort();
          }
          break;
        }
    }
    hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n393 = hxc_l_text1_hbba66327ca7e;
    hxc_l_tmp_managed_flow_owner_n136 = hxc_l_tmp_managed_flow_carrier_move_result_n393;
    struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_load_result_n394 = hxc_l_presentation;
    hxc_l_gc_roots[17] = (const void *)hxc_l_tmp_load_result_n394;
    if (hxc_l_tmp_load_result_n394 == NULL)
    {
      abort();
    }
    hxc_string hxc_l_tmp_load_result_n395 = hxc_l_text_h409c3cedb1e0;
    hxc_string hxc_l_tmp_instance_call_result_n397 = hxc_caxecraft_content_RuntimeLevelPresentation_message(hxc_l_tmp_load_result_n394, hxc_l_tmp_load_result_n395, hxc_l_tmp_managed_flow_owner_n136);
    hxc_l_tmp_cstring_ref_owner_owner_n137 = hxc_l_tmp_instance_call_result_n397;
    hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n399 = HXC_BORROWED_CSTRING_INITIALIZER;
    if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n137, &hxc_l_tmp_string_borrow_cstring_result_n399) != HXC_STATUS_OK)
    {
      abort();
    }
    int32_t hxc_l_tmp_load_result_n400 = hxc_l_centerX;
    int32_t hxc_l_tmp_load_result_n401 = hxc_l_centerY;
    DrawText(hxc_l_tmp_string_borrow_cstring_result_n399.data, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n400, 125), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n401, 10), (int32_t)18, hxc_l_color_hdc49e94c17f0);
    if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n137) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n136) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_text_h409c3cedb1e0) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n132) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n131) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_text1_h6cdf5fedfbfd) != HXC_STATUS_OK)
    {
      abort();
    }
  }
  if (hxc_l_paused)
  {
    hxc_string hxc_l_tmp_record_field_project_n404 = hxc_l_view.hxc_journalTitle;
    if (hxc_l_tmp_record_field_project_n404.data == NULL)
    {
      abort();
    }
    int32_t hxc_l_tmp_string_length_result_n405;
    if (hxc_string_haxe_length(hxc_l_tmp_record_field_project_n404, &hxc_l_tmp_string_length_result_n405) != HXC_STATUS_OK)
    {
      abort();
    }
    bool hxc_l_tmp_short_circuit_result_n139 = hxc_l_tmp_string_length_result_n405 > 0;
    if (!(hxc_l_tmp_string_length_result_n405 > 0))
    {
      hxc_string hxc_l_tmp_record_field_project_n406 = hxc_l_view.hxc_journalBody;
      if (hxc_l_tmp_record_field_project_n406.data == NULL)
      {
        abort();
      }
      int32_t hxc_l_tmp_string_length_result_n407;
      if (hxc_string_haxe_length(hxc_l_tmp_record_field_project_n406, &hxc_l_tmp_string_length_result_n407) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_l_tmp_short_circuit_result_n139 = hxc_l_tmp_string_length_result_n407 > 0;
    }
    bool hxc_l_hasJournal = hxc_l_tmp_short_circuit_result_n139;
    bool hxc_l_tmp_load_result_n409 = hxc_l_hasJournal;
    int32_t hxc_l_tmp_conditional_result_n141 = 0;
    if (hxc_l_tmp_load_result_n409)
    {
      hxc_l_tmp_conditional_result_n141 = hxc_i32_subtract_wrapping(hxc_l_centerX, 330);
    }
    else
    {
      hxc_l_tmp_conditional_result_n141 = hxc_i32_subtract_wrapping(hxc_l_centerX, 170);
    }
    int32_t hxc_l_panelX = hxc_l_tmp_conditional_result_n141;
    bool hxc_l_tmp_load_result_n413 = hxc_l_hasJournal;
    int32_t hxc_l_tmp_conditional_result_n143 = 0;
    if (hxc_l_tmp_load_result_n413)
    {
      hxc_l_tmp_conditional_result_n143 = hxc_i32_subtract_wrapping(hxc_l_centerY, 110);
    }
    else
    {
      hxc_l_tmp_conditional_result_n143 = hxc_i32_subtract_wrapping(hxc_l_centerY, 48);
    }
    int32_t hxc_l_panelY = hxc_l_tmp_conditional_result_n143;
    bool hxc_l_tmp_load_result_n417 = hxc_l_hasJournal;
    int32_t hxc_l_tmp_conditional_result_n145 = 0;
    if (hxc_l_tmp_load_result_n417)
    {
      hxc_l_tmp_conditional_result_n145 = 660;
    }
    else
    {
      hxc_l_tmp_conditional_result_n145 = 340;
    }
    int32_t hxc_l_panelWidth = hxc_l_tmp_conditional_result_n145;
    bool hxc_l_tmp_load_result_n419 = hxc_l_hasJournal;
    int32_t hxc_l_tmp_conditional_result_n147 = 0;
    if (hxc_l_tmp_load_result_n419)
    {
      hxc_l_tmp_conditional_result_n147 = 220;
    }
    else
    {
      hxc_l_tmp_conditional_result_n147 = 96;
    }
    int32_t hxc_l_panelHeight = hxc_l_tmp_conditional_result_n147;
    struct Color hxc_l_this1_hdfcfdebc8136 = (struct Color){ .r = 15, .g = 27, .b = 39, .a = 220 };
    int32_t hxc_l_tmp_load_result_n422 = hxc_l_panelX;
    int32_t hxc_l_tmp_load_result_n423 = hxc_l_panelY;
    int32_t hxc_l_tmp_load_result_n424 = hxc_l_panelWidth;
    int32_t hxc_l_tmp_load_result_n425 = hxc_l_panelHeight;
    DrawRectangle((int32_t)hxc_l_tmp_load_result_n422, (int32_t)hxc_l_tmp_load_result_n423, (int32_t)hxc_l_tmp_load_result_n424, (int32_t)hxc_l_tmp_load_result_n425, hxc_l_this1_hdfcfdebc8136);
    struct Color hxc_l_this1_hba8589999395 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
    int32_t hxc_l_tmp_load_result_n428 = hxc_l_panelX;
    int32_t hxc_l_tmp_load_result_n429 = hxc_l_panelY;
    int32_t hxc_l_tmp_load_result_n430 = hxc_l_panelWidth;
    int32_t hxc_l_tmp_load_result_n431 = hxc_l_panelHeight;
    DrawRectangleLines((int32_t)hxc_l_tmp_load_result_n428, (int32_t)hxc_l_tmp_load_result_n429, (int32_t)hxc_l_tmp_load_result_n430, (int32_t)hxc_l_tmp_load_result_n431, hxc_l_this1_hba8589999395);
    hxc_string hxc_l_tmp_instance_call_result_n434 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_uiCatalog, hxc_l_locale, 32);
    hxc_l_tmp_cstring_ref_owner_owner_n150 = hxc_l_tmp_instance_call_result_n434;
    hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n436 = HXC_BORROWED_CSTRING_INITIALIZER;
    if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n150, &hxc_l_tmp_string_borrow_cstring_result_n436) != HXC_STATUS_OK)
    {
      abort();
    }
    int32_t hxc_l_tmp_load_result_n437 = hxc_l_centerX;
    int32_t hxc_l_tmp_load_result_n438 = hxc_l_panelY;
    DrawText(hxc_l_tmp_string_borrow_cstring_result_n436.data, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n437, 48), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n438, 18), (int32_t)24, hxc_l_text_h774b5c1af5bc);
    if (hxc_l_hasJournal)
    {
      hxc_l_text1_h2995ec10316e = hxc_l_view.hxc_journalTitle;
      if (hxc_string_retain(hxc_l_text1_h2995ec10316e) != HXC_STATUS_OK)
      {
        abort();
      }
      struct Color hxc_l_this1_h287b99155e13 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
      hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n444 = HXC_BORROWED_CSTRING_INITIALIZER;
      if (hxc_string_borrow_cstring(&hxc_l_text1_h2995ec10316e, &hxc_l_tmp_string_borrow_cstring_result_n444) != HXC_STATUS_OK)
      {
        abort();
      }
      int32_t hxc_l_tmp_load_result_n445 = hxc_l_panelX;
      int32_t hxc_l_tmp_load_result_n446 = hxc_l_panelY;
      DrawText(hxc_l_tmp_string_borrow_cstring_result_n444.data, (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n445, 30), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n446, 68), (int32_t)18, hxc_l_this1_h287b99155e13);
      int32_t hxc_l_tmp_load_result_n449 = hxc_l_panelX;
      int32_t hxc_l_tmp_load_result_n450 = hxc_l_panelY;
      hxc_caxecraft_app_CaxecraftApp_drawWrappedText(hxc_l_view.hxc_journalBody, hxc_i32_add_wrapping(hxc_l_tmp_load_result_n449, 30), hxc_i32_add_wrapping(hxc_l_tmp_load_result_n450, 104), 16, 58, 22, 3, hxc_l_text_h774b5c1af5bc);
      if (hxc_string_release(&hxc_l_text1_h2995ec10316e) != HXC_STATUS_OK)
      {
        abort();
      }
    }
    hxc_string hxc_l_tmp_instance_call_result_n453 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_uiCatalog, hxc_l_locale, 31);
    hxc_l_tmp_cstring_ref_owner_owner_n153 = hxc_l_tmp_instance_call_result_n453;
    hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n455 = HXC_BORROWED_CSTRING_INITIALIZER;
    if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n153, &hxc_l_tmp_string_borrow_cstring_result_n455) != HXC_STATUS_OK)
    {
      abort();
    }
    int32_t hxc_l_tmp_load_result_n456 = hxc_l_centerX;
    int32_t hxc_l_tmp_load_result_n457 = hxc_l_panelY;
    int32_t hxc_l_tmp_load_result_n458 = hxc_l_panelHeight;
    DrawText(hxc_l_tmp_string_borrow_cstring_result_n455.data, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n456, 160), (int32_t)hxc_i32_subtract_wrapping(hxc_i32_add_wrapping(hxc_l_tmp_load_result_n457, hxc_l_tmp_load_result_n458), 34), (int32_t)16, hxc_l_text_h774b5c1af5bc);
    if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n153) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n150) != HXC_STATUS_OK)
    {
      abort();
    }
  }
  else
  {
    if (hxc_l_placementBlocked)
    {
      struct Color hxc_l_this1_h42e4d18ee81e = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
      hxc_string hxc_l_tmp_instance_call_result_n463 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_uiCatalog, hxc_l_locale, 33);
      hxc_l_tmp_cstring_ref_owner_owner_n155 = hxc_l_tmp_instance_call_result_n463;
      hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n465 = HXC_BORROWED_CSTRING_INITIALIZER;
      if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n155, &hxc_l_tmp_string_borrow_cstring_result_n465) != HXC_STATUS_OK)
      {
        abort();
      }
      int32_t hxc_l_tmp_load_result_n466 = hxc_l_centerX;
      int32_t hxc_l_tmp_load_result_n467 = hxc_l_centerY;
      DrawText(hxc_l_tmp_string_borrow_cstring_result_n465.data, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n466, 170), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n467, 26), (int32_t)14, hxc_l_this1_h42e4d18ee81e);
      if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n155) != HXC_STATUS_OK)
      {
        abort();
      }
    }
    else
    {
      if (!hxc_l_captured)
      {
        hxc_string hxc_l_tmp_instance_call_result_n471 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_uiCatalog, hxc_l_locale, 2);
        hxc_l_tmp_cstring_ref_owner_owner_n156 = hxc_l_tmp_instance_call_result_n471;
        hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n473 = HXC_BORROWED_CSTRING_INITIALIZER;
        if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n156, &hxc_l_tmp_string_borrow_cstring_result_n473) != HXC_STATUS_OK)
        {
          abort();
        }
        int32_t hxc_l_tmp_load_result_n474 = hxc_l_centerX;
        int32_t hxc_l_tmp_load_result_n475 = hxc_l_centerY;
        DrawText(hxc_l_tmp_string_borrow_cstring_result_n473.data, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n474, 90), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n475, 26), (int32_t)14, hxc_l_text_h774b5c1af5bc);
        if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n156) != HXC_STATUS_OK)
        {
          abort();
        }
      }
      else
      {
        if (!hxc_l_hit.hxc_hit)
        {
          hxc_string hxc_l_tmp_instance_call_result_n479 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_uiCatalog, hxc_l_locale, 30);
          hxc_l_tmp_cstring_ref_owner_owner_n157 = hxc_l_tmp_instance_call_result_n479;
          hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n481 = HXC_BORROWED_CSTRING_INITIALIZER;
          if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n157, &hxc_l_tmp_string_borrow_cstring_result_n481) != HXC_STATUS_OK)
          {
            abort();
          }
          int32_t hxc_l_tmp_load_result_n482 = hxc_l_centerX;
          int32_t hxc_l_tmp_load_result_n483 = hxc_l_centerY;
          DrawText(hxc_l_tmp_string_borrow_cstring_result_n481.data, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n482, 105), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n483, 26), (int32_t)14, hxc_l_text_h774b5c1af5bc);
          if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n157) != HXC_STATUS_OK)
          {
            abort();
          }
        }
      }
    }
  }
  if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n55) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n54) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n53) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n52) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n51) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n50) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_string_release(&hxc_l_text1_h9ace1a5abd84) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n47) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_string_release(&hxc_l_activeDialogue) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return;
}

void hxc_caxecraft_app_CaxecraftApp_drawWrappedText(hxc_string hxc_l_value, int32_t hxc_l_x, int32_t hxc_l_y, int32_t hxc_l_fontSize, int32_t hxc_l_maximumCharacters, int32_t hxc_l_lineHeight, int32_t hxc_l_maximumLines, struct Color hxc_l_color)
{
  hxc_string hxc_l_word = { 0 };
  hxc_string hxc_l_tmp_array_element_owner_n13 = { 0 };
  hxc_string hxc_l_candidate = { 0 };
  hxc_string hxc_l_tmp_string_concat_left_owner_n16 = { 0 };
  if (hxc_l_value.data == NULL)
  {
    abort();
  }
  if ((hxc_string){ (const uint8_t *)" ", 1, true, NULL }.data == NULL)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_tmp_string_split_result_n0 = NULL;
  if (hxc_string_split(hxc_l_value, (hxc_string){ (const uint8_t *)" ", 1, true, NULL }, hxc_default_allocator(), (hxc_array_element_ops){ sizeof(hxc_string), _Alignof(hxc_string), NULL, hxc_array_87af1a2c_element_copy, hxc_array_87af1a2c_element_assign, hxc_array_87af1a2c_element_destroy }, &hxc_l_tmp_string_split_result_n0) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_words = hxc_l_tmp_string_split_result_n0;
  hxc_string hxc_l_line = (hxc_string){ (const uint8_t *)"", 0, true, NULL };
  if (hxc_string_retain(hxc_l_line) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_lineIndex = 0;
  int32_t hxc_l_g = 0;
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n1 = hxc_l_g;
    int32_t hxc_l_tmp_array_length_result_n3;
    if (hxc_array_ref_length(hxc_l_words, &hxc_l_tmp_array_length_result_n3) != HXC_STATUS_OK)
    {
      abort();
    }
    if (!(hxc_l_tmp_load_result_n1 < hxc_l_tmp_array_length_result_n3))
    {
      break;
    }
    struct hxc_array_ref *hxc_l_tmp_load_result_n4 = hxc_l_words;
    hxc_string hxc_l_tmp_array_get_result_n6;
    if (hxc_array_ref_get_copy(hxc_l_tmp_load_result_n4, (size_t)hxc_l_g, &hxc_l_tmp_array_get_result_n6) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_array_element_owner_n13 = hxc_l_tmp_array_get_result_n6;
    hxc_l_word = hxc_l_tmp_array_element_owner_n13;
    if (hxc_string_retain(hxc_l_word) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_g = hxc_i32_add_wrapping(hxc_l_g, 1);
    hxc_string hxc_l_tmp_load_result_n9 = hxc_l_line;
    if (hxc_l_tmp_load_result_n9.data == NULL)
    {
      abort();
    }
    int32_t hxc_l_tmp_string_length_result_n10;
    if (hxc_string_haxe_length(hxc_l_tmp_load_result_n9, &hxc_l_tmp_string_length_result_n10) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_string hxc_l_tmp_conditional_managed_result_n15 = { 0 };
    if (hxc_l_tmp_string_length_result_n10 == 0)
    {
      (void)hxc_l_word;
      hxc_l_tmp_conditional_managed_result_n15 = hxc_l_word;
      if (hxc_string_retain(hxc_l_tmp_conditional_managed_result_n15) != HXC_STATUS_OK)
      {
        abort();
      }
    }
    else
    {
      hxc_string hxc_l_tmp_string_concat_result_n13 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
      if (hxc_string_concat_ref(hxc_l_line, (hxc_string){ (const uint8_t *)" ", 1, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n13) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_l_tmp_string_concat_left_owner_n16 = hxc_l_tmp_string_concat_result_n13;
      hxc_string hxc_l_tmp_string_concat_left_borrow_result_n14 = hxc_l_tmp_string_concat_left_owner_n16;
      hxc_string hxc_l_tmp_string_concat_result_n16 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
      if (hxc_string_concat_ref(hxc_l_tmp_string_concat_left_borrow_result_n14, hxc_l_word, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n16) != HXC_STATUS_OK)
      {
        abort();
      }
      (void)hxc_l_tmp_string_concat_result_n16;
      hxc_l_tmp_conditional_managed_result_n15 = hxc_l_tmp_string_concat_result_n16;
      if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n16) != HXC_STATUS_OK)
      {
        abort();
      }
    }
    hxc_string hxc_l_tmp_conditional_managed_move_result_n17 = hxc_l_tmp_conditional_managed_result_n15;
    hxc_l_candidate = hxc_l_tmp_conditional_managed_move_result_n17;
    hxc_string hxc_l_tmp_load_result_n18 = hxc_l_line;
    if (hxc_l_tmp_load_result_n18.data == NULL)
    {
      abort();
    }
    int32_t hxc_l_tmp_string_length_result_n19;
    if (hxc_string_haxe_length(hxc_l_tmp_load_result_n18, &hxc_l_tmp_string_length_result_n19) != HXC_STATUS_OK)
    {
      abort();
    }
    bool hxc_l_tmp_short_circuit_result_n17 = hxc_l_tmp_string_length_result_n19 > 0;
    if (hxc_l_tmp_string_length_result_n19 > 0)
    {
      hxc_string hxc_l_tmp_load_result_n20 = hxc_l_candidate;
      if (hxc_l_tmp_load_result_n20.data == NULL)
      {
        abort();
      }
      int32_t hxc_l_tmp_string_length_result_n21;
      if (hxc_string_haxe_length(hxc_l_tmp_load_result_n20, &hxc_l_tmp_string_length_result_n21) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_l_tmp_short_circuit_result_n17 = hxc_l_tmp_string_length_result_n21 > hxc_l_maximumCharacters;
    }
    if (hxc_l_tmp_short_circuit_result_n17)
    {
      if (hxc_l_lineIndex >= hxc_l_maximumLines)
      {
        if (hxc_string_release(&hxc_l_candidate) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_string_release(&hxc_l_word) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_string_release(&hxc_l_tmp_array_element_owner_n13) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_string_release(&hxc_l_line) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_array_ref_release(hxc_l_words) != HXC_STATUS_OK)
        {
          abort();
        }
        return;
      }
      else
      {
        hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n25 = HXC_BORROWED_CSTRING_INITIALIZER;
        if (hxc_string_borrow_cstring(&hxc_l_line, &hxc_l_tmp_string_borrow_cstring_result_n25) != HXC_STATUS_OK)
        {
          abort();
        }
        DrawText(hxc_l_tmp_string_borrow_cstring_result_n25.data, (int32_t)hxc_l_x, (int32_t)hxc_i32_add_wrapping(hxc_l_y, hxc_i32_multiply_wrapping(hxc_l_lineIndex, hxc_l_lineHeight)), (int32_t)hxc_l_fontSize, hxc_l_color);
        hxc_l_lineIndex = hxc_i32_add_wrapping(hxc_l_lineIndex, 1);
        hxc_string hxc_l_tmp_string_assignment_replacement_owner_n18 = hxc_l_word;
        if (hxc_string_retain(hxc_l_tmp_string_assignment_replacement_owner_n18) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_string hxc_l_tmp_string_assignment_replacement_owned_load_result_n29 = hxc_l_tmp_string_assignment_replacement_owner_n18;
        if (hxc_string_release(&hxc_l_line) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_l_line = hxc_l_tmp_string_assignment_replacement_owned_load_result_n29;
      }
    }
    else
    {
      hxc_string hxc_l_tmp_string_assignment_replacement_owner_n19 = hxc_l_candidate;
      if (hxc_string_retain(hxc_l_tmp_string_assignment_replacement_owner_n19) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_string hxc_l_tmp_string_assignment_replacement_owned_load_result_n31 = hxc_l_tmp_string_assignment_replacement_owner_n19;
      if (hxc_string_release(&hxc_l_line) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_l_line = hxc_l_tmp_string_assignment_replacement_owned_load_result_n31;
    }
    if (hxc_string_release(&hxc_l_candidate) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_word) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_array_element_owner_n13) != HXC_STATUS_OK)
    {
      abort();
    }
  }
  hxc_string hxc_l_tmp_load_result_n32 = hxc_l_line;
  if (hxc_l_tmp_load_result_n32.data == NULL)
  {
    abort();
  }
  int32_t hxc_l_tmp_string_length_result_n33;
  if (hxc_string_haxe_length(hxc_l_tmp_load_result_n32, &hxc_l_tmp_string_length_result_n33) != HXC_STATUS_OK)
  {
    abort();
  }
  bool hxc_l_tmp_short_circuit_result_n20 = hxc_l_tmp_string_length_result_n33 > 0;
  if (hxc_l_tmp_string_length_result_n33 > 0)
  {
    hxc_l_tmp_short_circuit_result_n20 = hxc_l_lineIndex < hxc_l_maximumLines;
  }
  if (hxc_l_tmp_short_circuit_result_n20)
  {
    hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n37 = HXC_BORROWED_CSTRING_INITIALIZER;
    if (hxc_string_borrow_cstring(&hxc_l_line, &hxc_l_tmp_string_borrow_cstring_result_n37) != HXC_STATUS_OK)
    {
      abort();
    }
    DrawText(hxc_l_tmp_string_borrow_cstring_result_n37.data, (int32_t)hxc_l_x, (int32_t)hxc_i32_add_wrapping(hxc_l_y, hxc_i32_multiply_wrapping(hxc_l_lineIndex, hxc_l_lineHeight)), (int32_t)hxc_l_fontSize, hxc_l_color);
  }
  if (hxc_string_release(&hxc_l_line) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_ref_release(hxc_l_words) != HXC_STATUS_OK)
  {
    abort();
  }
  return;
}

bool hxc_caxecraft_app_CaxecraftApp_hasAvailableInteraction(struct hxc_caxecraft_app_CaxecraftApp_AvailableInteractionTarget hxc_l_target)
{
  switch (hxc_l_target.hxc_tag) {
    case hxc_caxecraft_app_CaxecraftApp_AvailableInteractionTarget_NoAvailableInteraction:
      {
        break;
      }
    case hxc_caxecraft_app_CaxecraftApp_AvailableInteractionTarget_DialogueInteraction:
      {
        return true;
      }
    case hxc_caxecraft_app_CaxecraftApp_AvailableInteractionTarget_StatefulObjectInteraction:
      {
        return true;
      }
    default:
      {
        abort();
      }
  }
  return false;
}

struct hxc_caxecraft_app_CaxecraftApp_AvailableInteractionTarget hxc_caxecraft_app_CaxecraftApp_nearestAvailableInteraction(struct hxc_caxecraft_domain_GameSession *hxc_l_session, struct hxc_caxecraft_app_PlayableLevelView *hxc_l_level)
{
  const void *hxc_l_gc_roots[2] = { (const void *)hxc_l_session, (const void *)hxc_l_level };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 2, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_string hxc_l_id_h01350e3ebad9 = { 0 };
  struct hxc_caxecraft_content_LoadedStatefulObjectBinding hxc_l_tmp_array_element_owner_n20 = { 0 };
  struct hxc_caxecraft_app_CaxecraftApp_AvailableInteractionTarget hxc_l_selected = { 0 };
  if (hxc_l_session == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_domain_GameView hxc_l_tmp_instance_call_result_n0 = hxc_caxecraft_domain_GameSession_view(hxc_l_session);
  struct hxc_caxecraft_domain_GameView hxc_l_view = hxc_l_tmp_instance_call_result_n0;
  if (!hxc_l_view.hxc_valid)
  {
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return (struct hxc_caxecraft_app_CaxecraftApp_AvailableInteractionTarget){ .hxc_tag = hxc_caxecraft_app_CaxecraftApp_AvailableInteractionTarget_NoAvailableInteraction };
  }
  struct hxc_caxecraft_domain_Character hxc_l_player = hxc_l_view.hxc_localPlayer;
  hxc_l_selected = (struct hxc_caxecraft_app_CaxecraftApp_AvailableInteractionTarget){ .hxc_tag = hxc_caxecraft_app_CaxecraftApp_AvailableInteractionTarget_NoAvailableInteraction };
  bool hxc_l_hasSelection = false;
  double hxc_l_selectedDistance = 0.0;
  int32_t hxc_l_g_h765e84b05247 = 0;
  if (hxc_l_level == NULL)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n5 = (*hxc_l_level).hxc_dialogueActors;
  int32_t hxc_l_tmp_array_length_result_n6;
  if (hxc_array_ref_length(hxc_l_tmp_class_field_load_result_n5, &hxc_l_tmp_array_length_result_n6) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_g1_hed0f9a52ee6f = hxc_l_tmp_array_length_result_n6;
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n7 = hxc_l_g_h765e84b05247;
    if (!(hxc_l_tmp_load_result_n7 < hxc_l_g1_hed0f9a52ee6f))
    {
      break;
    }
    int32_t hxc_l_tmp_increment_load_result_n9 = hxc_l_g_h765e84b05247;
    hxc_l_g_h765e84b05247 = hxc_i32_add_wrapping(hxc_l_tmp_increment_load_result_n9, 1);
    int32_t hxc_l_index_h8f1651a3f1fe = hxc_l_tmp_increment_load_result_n9;
    struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n10 = (*hxc_l_level).hxc_dialogueActors;
    struct hxc_caxecraft_app_PlayableDialogueActor hxc_l_tmp_array_get_result_n12;
    if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n10, (size_t)hxc_l_index_h8f1651a3f1fe, &hxc_l_tmp_array_get_result_n12) != HXC_STATUS_OK)
    {
      abort();
    }
    int32_t hxc_l_id_h56714afcdfb2 = hxc_l_tmp_array_get_result_n12.hxc_entityId;
    bool hxc_l_tmp_instance_call_result_n15 = hxc_caxecraft_domain_GameSession_actorInteractionAvailable(hxc_l_session, hxc_l_id_h56714afcdfb2);
    if (hxc_l_tmp_instance_call_result_n15)
    {
      struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n17 = hxc_caxecraft_domain_GameSession_readCharacter(hxc_l_session, hxc_l_id_h56714afcdfb2);
      struct hxc_caxecraft_domain_Character hxc_l_actor = hxc_l_tmp_instance_call_result_n17;
      double hxc_l_tmp_record_field_load_result_n18 = hxc_l_actor.hxc_body.hxc_x;
      double hxc_l_dx_h285c0c212747 = hxc_l_tmp_record_field_load_result_n18 - hxc_l_player.hxc_body.hxc_x;
      double hxc_l_tmp_record_field_load_result_n20 = hxc_l_actor.hxc_body.hxc_z;
      double hxc_l_dz_hfde19aab36a5 = hxc_l_tmp_record_field_load_result_n20 - hxc_l_player.hxc_body.hxc_z;
      double hxc_l_tmp_load_result_n22 = hxc_l_dx_h285c0c212747;
      double hxc_l_tmp_load_result_n23 = hxc_l_dx_h285c0c212747;
      double hxc_l_tmp_load_result_n24 = hxc_l_dz_hfde19aab36a5;
      double hxc_l_distance_he1dd0d623f03 = hxc_l_tmp_load_result_n22 * hxc_l_tmp_load_result_n23 + hxc_l_tmp_load_result_n24 * hxc_l_dz_hfde19aab36a5;
      bool hxc_l_tmp_load_result_n26 = hxc_l_hasSelection;
      bool hxc_l_tmp_short_circuit_result_n15 = !hxc_l_tmp_load_result_n26;
      if (!!hxc_l_tmp_load_result_n26)
      {
        double hxc_l_tmp_load_result_n27 = hxc_l_distance_he1dd0d623f03;
        hxc_l_tmp_short_circuit_result_n15 = hxc_l_tmp_load_result_n27 < hxc_l_selectedDistance;
      }
      if (hxc_l_tmp_short_circuit_result_n15)
      {
        int32_t hxc_l_tmp_load_result_n30 = hxc_l_id_h56714afcdfb2;
        hxc_enum_2c0e695d_destroy(&hxc_l_selected);
        hxc_l_selected = (struct hxc_caxecraft_app_CaxecraftApp_AvailableInteractionTarget){ .hxc_tag = hxc_caxecraft_app_CaxecraftApp_AvailableInteractionTarget_DialogueInteraction, .hxc_payload.hxc_DialogueInteraction.hxc_id = hxc_l_tmp_load_result_n30 };
        hxc_l_hasSelection = true;
        hxc_l_selectedDistance = hxc_l_distance_he1dd0d623f03;
      }
    }
  }
  int32_t hxc_l_g_h600c3d23bcb6 = 0;
  struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n33 = (*hxc_l_level).hxc_statefulObjects;
  int32_t hxc_l_tmp_array_length_result_n34;
  if (hxc_array_ref_length(hxc_l_tmp_class_field_load_result_n33, &hxc_l_tmp_array_length_result_n34) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_g1_h74addb1c4913 = hxc_l_tmp_array_length_result_n34;
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n35 = hxc_l_g_h600c3d23bcb6;
    if (!(hxc_l_tmp_load_result_n35 < hxc_l_g1_h74addb1c4913))
    {
      break;
    }
    int32_t hxc_l_tmp_increment_load_result_n37 = hxc_l_g_h600c3d23bcb6;
    hxc_l_g_h600c3d23bcb6 = hxc_i32_add_wrapping(hxc_l_tmp_increment_load_result_n37, 1);
    int32_t hxc_l_index_hb412918816d7 = hxc_l_tmp_increment_load_result_n37;
    struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n38 = (*hxc_l_level).hxc_statefulObjects;
    struct hxc_caxecraft_content_LoadedStatefulObjectBinding hxc_l_tmp_array_get_result_n40;
    if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n38, (size_t)hxc_l_index_hb412918816d7, &hxc_l_tmp_array_get_result_n40) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_array_element_owner_n20 = hxc_l_tmp_array_get_result_n40;
    hxc_l_id_h01350e3ebad9 = hxc_l_tmp_array_element_owner_n20.hxc_authoredId;
    if (hxc_string_retain(hxc_l_id_h01350e3ebad9) != HXC_STATUS_OK)
    {
      abort();
    }
    bool hxc_l_tmp_instance_call_result_n44 = hxc_caxecraft_domain_GameSession_statefulObjectInteractionAvailable(hxc_l_session, hxc_l_id_h01350e3ebad9);
    if (hxc_l_tmp_instance_call_result_n44)
    {
      struct hxc_caxecraft_scenario_ScenarioTransform hxc_l_tmp_instance_call_result_n46 = hxc_caxecraft_app_PlayableLevelView_statefulObjectTransformAt(hxc_l_level, hxc_l_index_hb412918816d7);
      struct hxc_caxecraft_scenario_ScenarioTransform hxc_l_transform = hxc_l_tmp_instance_call_result_n46;
      int32_t hxc_l_tmp_record_field_load_result_n47 = hxc_l_transform.hxc_xMilli;
      double hxc_l_dx_h00a5fa6f3ad8 = hxc_f64_divide_zero_safe((double)hxc_l_tmp_record_field_load_result_n47, 1000.0) - hxc_l_player.hxc_body.hxc_x;
      int32_t hxc_l_tmp_record_field_load_result_n49 = hxc_l_transform.hxc_zMilli;
      double hxc_l_dz_h4655569ee05d = hxc_f64_divide_zero_safe((double)hxc_l_tmp_record_field_load_result_n49, 1000.0) - hxc_l_player.hxc_body.hxc_z;
      double hxc_l_tmp_load_result_n51 = hxc_l_dx_h00a5fa6f3ad8;
      double hxc_l_tmp_load_result_n52 = hxc_l_dx_h00a5fa6f3ad8;
      double hxc_l_tmp_load_result_n53 = hxc_l_dz_h4655569ee05d;
      double hxc_l_distance_h4294df4bd0f7 = hxc_l_tmp_load_result_n51 * hxc_l_tmp_load_result_n52 + hxc_l_tmp_load_result_n53 * hxc_l_dz_h4655569ee05d;
      bool hxc_l_tmp_load_result_n55 = hxc_l_hasSelection;
      bool hxc_l_tmp_short_circuit_result_n25 = !hxc_l_tmp_load_result_n55;
      if (!!hxc_l_tmp_load_result_n55)
      {
        double hxc_l_tmp_load_result_n56 = hxc_l_distance_h4294df4bd0f7;
        hxc_l_tmp_short_circuit_result_n25 = hxc_l_tmp_load_result_n56 < hxc_l_selectedDistance;
      }
      if (hxc_l_tmp_short_circuit_result_n25)
      {
        hxc_string hxc_l_tmp_enum_payload_0_owner_n26 = hxc_l_id_h01350e3ebad9;
        if (hxc_string_retain(hxc_l_tmp_enum_payload_0_owner_n26) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_string hxc_l_tmp_enum_payload_0_owned_load_result_n60 = hxc_l_tmp_enum_payload_0_owner_n26;
        hxc_enum_2c0e695d_destroy(&hxc_l_selected);
        hxc_l_selected = (struct hxc_caxecraft_app_CaxecraftApp_AvailableInteractionTarget){ .hxc_tag = hxc_caxecraft_app_CaxecraftApp_AvailableInteractionTarget_StatefulObjectInteraction, .hxc_payload.hxc_StatefulObjectInteraction.hxc_id = hxc_l_tmp_enum_payload_0_owned_load_result_n60 };
        hxc_l_hasSelection = true;
        hxc_l_selectedDistance = hxc_l_distance_h4294df4bd0f7;
      }
    }
    if (hxc_string_release(&hxc_l_id_h01350e3ebad9) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_array_4d8613ea_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n20);
  }
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return hxc_l_selected;
}

struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation hxc_caxecraft_app_CaxecraftApp_observeActorPhase(struct hxc_array_ref *hxc_l_states, int32_t hxc_l_id, enum hxc_caxecraft_domain_ActorControllerPhase hxc_l_fallback)
{
  struct hxc_caxecraft_domain_ActorControllerState hxc_l_state = { 0 };
  struct hxc_caxecraft_domain_ActorControllerState hxc_l_tmp_array_element_owner_n5 = { 0 };
  int32_t hxc_l_g = 0;
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n0 = hxc_l_g;
    int32_t hxc_l_tmp_array_length_result_n1;
    if (hxc_array_ref_length(hxc_l_states, &hxc_l_tmp_array_length_result_n1) != HXC_STATUS_OK)
    {
      abort();
    }
    if (!(hxc_l_tmp_load_result_n0 < hxc_l_tmp_array_length_result_n1))
    {
      break;
    }
    struct hxc_caxecraft_domain_ActorControllerState hxc_l_tmp_array_get_result_n3;
    if (hxc_array_ref_get_copy(hxc_l_states, (size_t)hxc_l_g, &hxc_l_tmp_array_get_result_n3) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_array_element_owner_n5 = hxc_l_tmp_array_get_result_n3;
    hxc_l_state = hxc_l_tmp_array_element_owner_n5;
    if (hxc_record_50dff1c7_retain(&hxc_l_state) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_g = hxc_i32_add_wrapping(hxc_l_g, 1);
    if (hxc_l_state.hxc_characterId == hxc_l_id)
    {
      enum hxc_caxecraft_domain_ActorControllerPhase hxc_l_tmp_record_field_load_result_n7 = hxc_l_state.hxc_phase;
      hxc_record_50dff1c7_destroy(&hxc_l_state);
      hxc_array_3767ae6c_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n5);
      return (struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation){ .hxc_phase = hxc_l_tmp_record_field_load_result_n7, .hxc_valid = true };
    }
    hxc_record_50dff1c7_destroy(&hxc_l_state);
    hxc_array_3767ae6c_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n5);
  }
  return (struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation){ .hxc_phase = hxc_l_fallback, .hxc_valid = false };
}

struct hxc_caxecraft_domain_CharacterBody hxc_caxecraft_app_CaxecraftApp_spawnPlayer(const uint8_t *hxc_l_cells, size_t hxc_l_length, struct hxc_caxecraft_scenario_ScenarioTransform hxc_l_transform)
{
  const uint8_t *hxc_l_borrow = hxc_l_cells;
  size_t hxc_l_tmp_length_n2 = hxc_l_length;
  (void)hxc_l_borrow;
  (void)hxc_l_tmp_length_n2;
  double hxc_l_spawnX = hxc_f64_divide_zero_safe((double)hxc_l_transform.hxc_xMilli, 1000.0);
  double hxc_l_spawnY = hxc_f64_divide_zero_safe((double)hxc_l_transform.hxc_yMilli, 1000.0);
  double hxc_l_spawnZ = hxc_f64_divide_zero_safe((double)hxc_l_transform.hxc_zMilli, 1000.0);
  double hxc_l_tmp_load_result_n3 = hxc_l_spawnX;
  double hxc_l_tmp_load_result_n4 = hxc_l_spawnY;
  struct hxc_caxecraft_domain_CharacterBody hxc_l_tmp_call_result_n6 = hxc_caxecraft_domain_CharacterPhysics_body(hxc_l_tmp_load_result_n3, hxc_l_tmp_load_result_n4, hxc_l_spawnZ);
  struct hxc_caxecraft_domain_CharacterBody hxc_l_tmp_call_result_n7 = hxc_caxecraft_domain_CharacterPhysics_recoverSpawn(hxc_l_cells, hxc_l_length, hxc_l_tmp_call_result_n6);
  return hxc_l_tmp_call_result_n7;
}

void hxc_caxecraft_app_CaxecraftApp_run(struct hxc_caxecraft_app_CaxecraftApp *hxc_l_self)
{
  const void *hxc_l_gc_roots[262] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 262, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_string hxc_l_tmp_sys_println_string_argument_owner_n178 = { 0 };
  struct hxc_caxecraft_content_CampaignPackageLoadResult hxc_l_g_h6904c3060e30 = { 0 };
  struct hxc_caxecraft_content_CampaignLevelLoadResult hxc_l_g_hbd7f92d003e5 = { 0 };
  struct hxc_caxecraft_content_RuntimeLevelReceipt hxc_l_tmp_record_field_logicalPath_receiver_owner_n22 = { 0 };
  struct hxc_caxecraft_app_CaxecraftApp_AvailableInteractionTarget hxc_l_tmp_static_call_argument_0_owner_n222 = { 0 };
  struct hxc_array_ref *hxc_l_phases = { 0 };
  hxc_string hxc_l_tmp_string_concat_left_owner_n226 = { 0 };
  hxc_string hxc_l_tmp_string_concat_left_owner_n227 = { 0 };
  hxc_string hxc_l_tmp_string_concat_right_owner_n228 = { 0 };
  hxc_string hxc_l_tmp_sys_println_string_argument_owner_n229 = { 0 };
  struct hxc_caxecraft_content_CampaignPackageLoadError hxc_l_gerror = { 0 };
  struct hxc_caxecraft_content_CampaignLevelLoadError hxc_l_error = { 0 };
  hxc_string hxc_l_tmp_string_concat_right_owner_n231 = { 0 };
  hxc_string hxc_l_tmp_sys_println_string_argument_owner_n232 = { 0 };
  struct hxc_caxecraft_app_CaxecraftApp_AvailableInteractionTarget hxc_l_g_hdd9f9dcfea76 = { 0 };
  hxc_string hxc_l_levelLabel = { 0 };
  hxc_string hxc_l_id_h994c8bd3392d = { 0 };
  struct hxc_caxecraft_content_RuntimeLevelReceipt hxc_l_tmp_record_field_logicalPath_receiver_owner_n26 = { 0 };
  struct hxc_caxecraft_domain_GameTickResult hxc_l_gameTick = { 0 };
  struct hxc_optional_caxecraft_scenario_FlowTickResult hxc_l_flow = { 0 };
  struct hxc_array_ref *hxc_l_g1_h7bf1eee5d9d8 = { 0 };
  struct hxc_caxecraft_scenario_FlowPresentationEvent hxc_l_event_h3072cd2e0f94 = { 0 };
  struct hxc_caxecraft_scenario_FlowPresentationEvent hxc_l_tmp_array_element_owner_n280 = { 0 };
  hxc_string hxc_l_id_hd4b4b6738bb4 = { 0 };
  hxc_string hxc_l_id_h268fb64469d6 = { 0 };
  struct hxc_array_ref *hxc_l_g1_hc8e9ed09509e = { 0 };
  struct hxc_caxecraft_domain_ActorControllerEvent hxc_l_event_h5d214aacdd3f = { 0 };
  struct hxc_caxecraft_domain_ActorControllerEvent hxc_l_tmp_array_element_owner_n299 = { 0 };
  hxc_string hxc_l_drop = { 0 };
  struct hxc_array_ref *hxc_l_actorPhases = { 0 };
  struct hxc_caxecraft_app_CaxecraftApp_AvailableInteractionTarget hxc_l_tmp_static_call_argument_0_owner_n335 = { 0 };
  struct hxc_array_ref *hxc_l_presentationActorPhases = { 0 };
  struct hxc_array_ref *hxc_l_dialogueActors = { 0 };
  struct hxc_caxecraft_content_RuntimeLevelReceipt hxc_l_receipt = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n347 = { 0 };
  hxc_string hxc_l_adventureTagline = { 0 };
  hxc_string hxc_l_tmp_sys_println_string_argument_owner_n35 = { 0 };
  hxc_string hxc_l_tmp_string_concat_right_owner_n36 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n369 = { 0 };
  hxc_string hxc_l_tmp_sys_println_string_argument_owner_n37 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n377 = { 0 };
  hxc_string hxc_l_tmp_string_concat_right_owner_n38 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n385 = { 0 };
  hxc_string hxc_l_tmp_sys_println_string_argument_owner_n39 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n391 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n394 = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n400 = { 0 };
  hxc_string hxc_l_tmp = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n404 = { 0 };
  hxc_string hxc_l_tmp_static_call_argument_9_owner_n405 = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n436 = { 0 };
  hxc_string hxc_l_hudView7 = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n441 = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n442 = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n446 = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n447 = { 0 };
  struct hxc_caxecraft_app_HudView hxc_l_hudView_hbda7460f80cb = { 0 };
  struct hxc_caxecraft_content_RuntimeContentLoadResult hxc_l_g_h4ef171f4fe67 = { 0 };
  hxc_string hxc_l_activeDialogue = { 0 };
  hxc_string hxc_l_latestJournalId = { 0 };
  hxc_string hxc_l_currentObjectiveId = { 0 };
  struct hxc_caxecraft_app_CaxecraftApp_AvailableInteractionTarget hxc_l_tmp_static_call_argument_0_owner_n64 = { 0 };
  struct hxc_array_ref *hxc_l_initialActorPhases = { 0 };
  int32_t hxc_l_initialHealth = 6;
  hxc_l_gc_roots[0] = (const void *)NULL;
  struct hxc_caxecraft_content_ContentPackageStore *hxc_l_contentStore = NULL;
  struct hxc_caxecraft_content_ContentPackageOpenResult hxc_l_tmp_call_result_n0 = hxc_caxecraft_content_ContentPackageStore_open((hxc_string){ (const uint8_t *)"content", 7, true, NULL }, (hxc_string){ (const uint8_t *)"staged-content", 14, true, NULL }, 16777216);
  hxc_l_gc_roots[1] = hxc_l_tmp_call_result_n0.hxc_tag == hxc_caxecraft_content_ContentPackageOpenResult_PackageStoreOpened ? (const void *)hxc_l_tmp_call_result_n0.hxc_payload.hxc_PackageStoreOpened.hxc_store : NULL;
  struct hxc_caxecraft_content_ContentPackageOpenResult hxc_l_g_h0b6eadae1b17 = hxc_l_tmp_call_result_n0;
  hxc_l_gc_roots[2] = hxc_l_g_h0b6eadae1b17.hxc_tag == hxc_caxecraft_content_ContentPackageOpenResult_PackageStoreOpened ? (const void *)hxc_l_g_h0b6eadae1b17.hxc_payload.hxc_PackageStoreOpened.hxc_store : NULL;
  switch (hxc_l_g_h0b6eadae1b17.hxc_tag) {
    case hxc_caxecraft_content_ContentPackageOpenResult_PackageStoreOpened:
      {
        break;
      }
    case hxc_caxecraft_content_ContentPackageOpenResult_PackageStoreRejected:
      {
        if (hxc_io_println((hxc_string){ (const uint8_t *)"caxecraft: runtime content root rejected", 40, true, NULL }) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
        {
          abort();
        }
        return;
      }
    default:
      {
        abort();
      }
  }
  hxc_l_gc_roots[3] = hxc_l_g_h0b6eadae1b17.hxc_tag == hxc_caxecraft_content_ContentPackageOpenResult_PackageStoreOpened ? (const void *)hxc_l_g_h0b6eadae1b17.hxc_payload.hxc_PackageStoreOpened.hxc_store : NULL;
  if (hxc_l_g_h0b6eadae1b17.hxc_tag != hxc_caxecraft_content_ContentPackageOpenResult_PackageStoreOpened)
  {
    abort();
  }
  struct hxc_caxecraft_content_ContentPackageStore *hxc_l_tmp_enum_payload_project_n3 = hxc_l_g_h0b6eadae1b17.hxc_payload.hxc_PackageStoreOpened.hxc_store;
  hxc_l_gc_roots[4] = (const void *)hxc_l_tmp_enum_payload_project_n3;
  struct hxc_caxecraft_content_ContentPackageStore *hxc_l_store = hxc_l_tmp_enum_payload_project_n3;
  hxc_l_gc_roots[5] = (const void *)hxc_l_store;
  hxc_l_contentStore = hxc_l_store;
  hxc_l_gc_roots[6] = (const void *)NULL;
  struct hxc_caxecraft_content_RuntimeContentGeneration *hxc_l_completeCandidate = NULL;
  struct hxc_caxecraft_content_ContentPackageStore *hxc_l_tmp_load_result_n5 = hxc_l_contentStore;
  hxc_l_gc_roots[7] = (const void *)hxc_l_tmp_load_result_n5;
  struct hxc_caxecraft_content_RuntimeContentLoadResult hxc_l_tmp_call_result_n8 = hxc_caxecraft_content_RuntimeContentGeneration_loadRuntimeContent(hxc_l_tmp_load_result_n5, 1, (struct hxc_caxecraft_content_RuntimeContentPlayerOptions){ .hxc_entityId = 1, .hxc_initialHealth = hxc_l_initialHealth });
  hxc_l_gc_roots[8] = hxc_l_tmp_call_result_n8.hxc_tag == hxc_caxecraft_content_RuntimeContentLoadResult_RuntimeContentReady ? (const void *)hxc_l_tmp_call_result_n8.hxc_payload.hxc_RuntimeContentReady.hxc_candidate : NULL;
  hxc_l_g_h4ef171f4fe67 = hxc_l_tmp_call_result_n8;
  hxc_l_gc_roots[9] = hxc_l_g_h4ef171f4fe67.hxc_tag == hxc_caxecraft_content_RuntimeContentLoadResult_RuntimeContentReady ? (const void *)hxc_l_g_h4ef171f4fe67.hxc_payload.hxc_RuntimeContentReady.hxc_candidate : NULL;
  switch (hxc_l_g_h4ef171f4fe67.hxc_tag) {
    case hxc_caxecraft_content_RuntimeContentLoadResult_RuntimeContentReady:
      {
        break;
      }
    case hxc_caxecraft_content_RuntimeContentLoadResult_RuntimeContentRejected:
      {
        if (hxc_io_println((hxc_string){ (const uint8_t *)"caxecraft: runtime content rejected", 35, true, NULL }) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_enum_fcb4401b_destroy(&hxc_l_g_h4ef171f4fe67);
        if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
        {
          abort();
        }
        return;
      }
    default:
      {
        abort();
      }
  }
  hxc_l_gc_roots[10] = hxc_l_g_h4ef171f4fe67.hxc_tag == hxc_caxecraft_content_RuntimeContentLoadResult_RuntimeContentReady ? (const void *)hxc_l_g_h4ef171f4fe67.hxc_payload.hxc_RuntimeContentReady.hxc_candidate : NULL;
  if (hxc_l_g_h4ef171f4fe67.hxc_tag != hxc_caxecraft_content_RuntimeContentLoadResult_RuntimeContentReady)
  {
    abort();
  }
  struct hxc_caxecraft_content_RuntimeContentGeneration *hxc_l_tmp_enum_payload_project_n11 = hxc_l_g_h4ef171f4fe67.hxc_payload.hxc_RuntimeContentReady.hxc_candidate;
  hxc_l_gc_roots[11] = (const void *)hxc_l_tmp_enum_payload_project_n11;
  struct hxc_caxecraft_content_RuntimeContentGeneration *hxc_l_candidate_hcf9d4ad136bd = hxc_l_tmp_enum_payload_project_n11;
  hxc_l_gc_roots[12] = (const void *)hxc_l_candidate_hcf9d4ad136bd;
  hxc_l_completeCandidate = hxc_l_candidate_hcf9d4ad136bd;
  struct hxc_caxecraft_content_RuntimeContentGeneration *hxc_l_tmp_load_result_n13 = hxc_l_completeCandidate;
  hxc_l_gc_roots[13] = (const void *)hxc_l_tmp_load_result_n13;
  struct hxc_caxecraft_content_ActiveRuntimeContent *hxc_l_tmp_managed_class_result_n14 = NULL;
  if (hxc_gc_allocate(&hxc_program_gc, &hxc_caxecraft_content_ActiveRuntimeContent_descriptor, (void **)&hxc_l_tmp_managed_class_result_n14) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_gc_roots[14] = (const void *)hxc_l_tmp_managed_class_result_n14;
  hxc_compiler_constructor_caxecraft_content_ActiveRuntimeContent(hxc_l_tmp_managed_class_result_n14, hxc_l_tmp_load_result_n13);
  hxc_l_gc_roots[15] = (const void *)hxc_l_tmp_managed_class_result_n14;
  struct hxc_caxecraft_content_ActiveRuntimeContent *hxc_l_activeRuntimeContent = hxc_l_tmp_managed_class_result_n14;
  struct hxc_caxecraft_content_ActiveRuntimeContent *hxc_l_tmp_load_result_n15 = hxc_l_activeRuntimeContent;
  hxc_l_gc_roots[16] = (const void *)hxc_l_tmp_load_result_n15;
  if (hxc_l_tmp_load_result_n15 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_content_RuntimeContentGeneration *hxc_l_tmp_class_field_load_result_n16 = (*hxc_l_tmp_load_result_n15).hxc_active;
  hxc_l_gc_roots[17] = (const void *)hxc_l_tmp_class_field_load_result_n16;
  struct hxc_caxecraft_content_RuntimeContentGeneration *hxc_l_runtimeContent = hxc_l_tmp_class_field_load_result_n16;
  struct hxc_caxecraft_content_RuntimeContentGeneration *hxc_l_tmp_load_result_n17 = hxc_l_runtimeContent;
  hxc_l_gc_roots[18] = (const void *)hxc_l_tmp_load_result_n17;
  if (hxc_l_tmp_load_result_n17 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_tmp_class_field_load_result_n18 = (*hxc_l_tmp_load_result_n17).hxc_contentRegistry;
  hxc_l_gc_roots[19] = (const void *)hxc_l_tmp_class_field_load_result_n18;
  struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_contentRegistry = hxc_l_tmp_class_field_load_result_n18;
  struct hxc_caxecraft_content_RuntimeContentGeneration *hxc_l_tmp_load_result_n19 = hxc_l_runtimeContent;
  hxc_l_gc_roots[20] = (const void *)hxc_l_tmp_load_result_n19;
  if (hxc_l_tmp_load_result_n19 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_tmp_class_field_load_result_n20 = (*hxc_l_tmp_load_result_n19).hxc_uiCatalog;
  hxc_l_gc_roots[21] = (const void *)hxc_l_tmp_class_field_load_result_n20;
  struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_uiCatalog = hxc_l_tmp_class_field_load_result_n20;
  struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_tmp_load_result_n21 = hxc_l_contentRegistry;
  hxc_l_gc_roots[22] = (const void *)hxc_l_tmp_load_result_n21;
  struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_tmp_load_result_n22 = hxc_l_uiCatalog;
  hxc_l_gc_roots[23] = (const void *)hxc_l_tmp_load_result_n22;
  struct hxc_caxecraft_app_CaxecraftEditorScreen *hxc_l_tmp_managed_class_result_n23 = NULL;
  if (hxc_gc_allocate(&hxc_program_gc, &hxc_caxecraft_app_CaxecraftEditorScreen_descriptor, (void **)&hxc_l_tmp_managed_class_result_n23) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_gc_roots[24] = (const void *)hxc_l_tmp_managed_class_result_n23;
  hxc_compiler_constructor_caxecraft_app_CaxecraftEditorScreen(hxc_l_tmp_managed_class_result_n23, hxc_l_tmp_load_result_n21, hxc_l_tmp_load_result_n22);
  hxc_l_gc_roots[25] = (const void *)hxc_l_tmp_managed_class_result_n23;
  struct hxc_caxecraft_app_CaxecraftEditorScreen *hxc_l_editorScreen = hxc_l_tmp_managed_class_result_n23;
  struct hxc_caxecraft_content_RuntimeContentGeneration *hxc_l_tmp_load_result_n24 = hxc_l_runtimeContent;
  hxc_l_gc_roots[26] = (const void *)hxc_l_tmp_load_result_n24;
  if (hxc_l_tmp_load_result_n24 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_content_RuntimeLevelCandidate *hxc_l_tmp_class_field_load_result_n25 = (*hxc_l_tmp_load_result_n24).hxc_loadedLevel;
  hxc_l_gc_roots[27] = (const void *)hxc_l_tmp_class_field_load_result_n25;
  struct hxc_caxecraft_content_RuntimeLevelCandidate *hxc_l_loadedCandidate = hxc_l_tmp_class_field_load_result_n25;
  hxc_l_gc_roots[28] = (const void *)NULL;
  struct hxc_caxecraft_app_ActivePlayableLevel *hxc_l_activeLevel = NULL;
  hxc_l_gc_roots[29] = (const void *)hxc_l_loadedCandidate;
  struct hxc_caxecraft_app_PlayableLevelCreationResult hxc_l_tmp_call_result_n27 = hxc_caxecraft_app_ActivePlayableLevel_create(hxc_l_loadedCandidate);
  hxc_l_gc_roots[30] = hxc_l_tmp_call_result_n27.hxc_tag == hxc_caxecraft_app_PlayableLevelCreationResult_PlayableLevelCreated ? (const void *)hxc_l_tmp_call_result_n27.hxc_payload.hxc_PlayableLevelCreated.hxc_owner : NULL;
  struct hxc_caxecraft_app_PlayableLevelCreationResult hxc_l_g_h1d9290112915 = hxc_l_tmp_call_result_n27;
  hxc_l_gc_roots[31] = hxc_l_g_h1d9290112915.hxc_tag == hxc_caxecraft_app_PlayableLevelCreationResult_PlayableLevelCreated ? (const void *)hxc_l_g_h1d9290112915.hxc_payload.hxc_PlayableLevelCreated.hxc_owner : NULL;
  switch (hxc_l_g_h1d9290112915.hxc_tag) {
    case hxc_caxecraft_app_PlayableLevelCreationResult_PlayableLevelCreated:
      {
        break;
      }
    case hxc_caxecraft_app_PlayableLevelCreationResult_PlayableLevelCreationRejected:
      {
        if (hxc_io_println((hxc_string){ (const uint8_t *)"caxecraft: initial level lacks required playable bindings", 57, true, NULL }) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_enum_fcb4401b_destroy(&hxc_l_g_h4ef171f4fe67);
        if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
        {
          abort();
        }
        return;
      }
    default:
      {
        abort();
      }
  }
  hxc_l_gc_roots[32] = hxc_l_g_h1d9290112915.hxc_tag == hxc_caxecraft_app_PlayableLevelCreationResult_PlayableLevelCreated ? (const void *)hxc_l_g_h1d9290112915.hxc_payload.hxc_PlayableLevelCreated.hxc_owner : NULL;
  if (hxc_l_g_h1d9290112915.hxc_tag != hxc_caxecraft_app_PlayableLevelCreationResult_PlayableLevelCreated)
  {
    abort();
  }
  struct hxc_caxecraft_app_ActivePlayableLevel *hxc_l_tmp_enum_payload_project_n30 = hxc_l_g_h1d9290112915.hxc_payload.hxc_PlayableLevelCreated.hxc_owner;
  hxc_l_gc_roots[33] = (const void *)hxc_l_tmp_enum_payload_project_n30;
  struct hxc_caxecraft_app_ActivePlayableLevel *hxc_l_value = hxc_l_tmp_enum_payload_project_n30;
  hxc_l_gc_roots[34] = (const void *)hxc_l_value;
  hxc_l_activeLevel = hxc_l_value;
  hxc_l_gc_roots[35] = (const void *)NULL;
  struct hxc_caxecraft_content_CampaignManifest *hxc_l_campaign = NULL;
  hxc_l_gc_roots[36] = (const void *)NULL;
  struct hxc_caxecraft_content_CampaignLevel *hxc_l_campaignLevel = NULL;
  hxc_l_gc_roots[37] = (const void *)hxc_l_contentStore;
  struct hxc_caxecraft_content_CampaignPackageLoadResult hxc_l_tmp_call_result_n33 = hxc_caxecraft_content_CampaignRuntime_loadCampaignManifest(hxc_l_contentStore, (hxc_string){ (const uint8_t *)"campaigns/first-adventure/campaign.json", 39, true, NULL });
  hxc_l_gc_roots[38] = hxc_l_tmp_call_result_n33.hxc_tag == hxc_caxecraft_content_CampaignPackageLoadResult_CampaignPackageReady ? (const void *)hxc_l_tmp_call_result_n33.hxc_payload.hxc_CampaignPackageReady.hxc_manifest : NULL;
  hxc_l_g_h6904c3060e30 = hxc_l_tmp_call_result_n33;
  hxc_l_gc_roots[39] = hxc_l_g_h6904c3060e30.hxc_tag == hxc_caxecraft_content_CampaignPackageLoadResult_CampaignPackageReady ? (const void *)hxc_l_g_h6904c3060e30.hxc_payload.hxc_CampaignPackageReady.hxc_manifest : NULL;
  switch (hxc_l_g_h6904c3060e30.hxc_tag) {
    case hxc_caxecraft_content_CampaignPackageLoadResult_CampaignPackageReady:
      {
        hxc_l_gc_roots[40] = hxc_l_g_h6904c3060e30.hxc_tag == hxc_caxecraft_content_CampaignPackageLoadResult_CampaignPackageReady ? (const void *)hxc_l_g_h6904c3060e30.hxc_payload.hxc_CampaignPackageReady.hxc_manifest : NULL;
        if (hxc_l_g_h6904c3060e30.hxc_tag != hxc_caxecraft_content_CampaignPackageLoadResult_CampaignPackageReady)
        {
          abort();
        }
        struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_enum_payload_project_n36 = hxc_l_g_h6904c3060e30.hxc_payload.hxc_CampaignPackageReady.hxc_manifest;
        hxc_l_gc_roots[41] = (const void *)hxc_l_tmp_enum_payload_project_n36;
        struct hxc_caxecraft_content_CampaignManifest *hxc_l_manifest = hxc_l_tmp_enum_payload_project_n36;
        struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n37 = hxc_l_manifest;
        hxc_l_gc_roots[42] = (const void *)hxc_l_tmp_load_result_n37;
        if (hxc_l_tmp_load_result_n37 == NULL)
        {
          abort();
        }
        struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n38 = (*hxc_l_tmp_load_result_n37).hxc_admittedLevels;
        hxc_l_gc_roots[43] = (const void *)hxc_l_tmp_class_field_load_result_n38;
        struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n39 = hxc_l_manifest;
        hxc_l_gc_roots[44] = (const void *)hxc_l_tmp_load_result_n39;
        if (hxc_l_tmp_load_result_n39 == NULL)
        {
          abort();
        }
        int32_t hxc_l_tmp_class_field_load_result_n40 = (*hxc_l_tmp_load_result_n39).hxc_entryIndexValue;
        struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_array_get_result_n41;
        if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n38, (size_t)hxc_l_tmp_class_field_load_result_n40, &hxc_l_tmp_array_get_result_n41) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_l_gc_roots[45] = (const void *)hxc_l_tmp_array_get_result_n41;
        struct hxc_caxecraft_content_CampaignLevel *hxc_l_entry = hxc_l_tmp_array_get_result_n41;
        struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_load_result_n42 = hxc_l_entry;
        hxc_l_gc_roots[46] = (const void *)hxc_l_tmp_load_result_n42;
        if (hxc_l_tmp_load_result_n42 == NULL)
        {
          abort();
        }
        hxc_string hxc_l_tmp_class_field_load_result_n43 = (*hxc_l_tmp_load_result_n42).hxc_logicalPath;
        struct hxc_caxecraft_content_RuntimeLevelCandidate *hxc_l_tmp_load_result_n44 = hxc_l_loadedCandidate;
        hxc_l_gc_roots[47] = (const void *)hxc_l_tmp_load_result_n44;
        if (hxc_l_tmp_load_result_n44 == NULL)
        {
          abort();
        }
        struct hxc_caxecraft_content_RuntimeLevelReceipt hxc_l_tmp_instance_call_result_n45 = hxc_caxecraft_content_RuntimeLevelCandidate_receipt(hxc_l_tmp_load_result_n44);
        hxc_l_tmp_record_field_logicalPath_receiver_owner_n22 = hxc_l_tmp_instance_call_result_n45;
        struct hxc_caxecraft_content_RuntimeLevelReceipt hxc_l_tmp_record_field_logicalPath_receiver_borrow_result_n46 = hxc_l_tmp_record_field_logicalPath_receiver_owner_n22;
        hxc_record_f6127605_destroy(&hxc_l_tmp_record_field_logicalPath_receiver_owner_n22);
        if (!(hxc_l_tmp_class_field_load_result_n43.data == NULL || hxc_l_tmp_record_field_logicalPath_receiver_borrow_result_n46.hxc_logicalPath.data == NULL ? hxc_l_tmp_class_field_load_result_n43.data == hxc_l_tmp_record_field_logicalPath_receiver_borrow_result_n46.hxc_logicalPath.data : hxc_l_tmp_class_field_load_result_n43.byte_length == hxc_l_tmp_record_field_logicalPath_receiver_borrow_result_n46.hxc_logicalPath.byte_length && (hxc_l_tmp_class_field_load_result_n43.byte_length == 0 || memcmp(hxc_l_tmp_class_field_load_result_n43.data, hxc_l_tmp_record_field_logicalPath_receiver_borrow_result_n46.hxc_logicalPath.data, hxc_l_tmp_class_field_load_result_n43.byte_length) == 0)))
        {
          if (hxc_io_println((hxc_string){ (const uint8_t *)"caxecraft: campaign entry does not match the active map", 55, true, NULL }) != HXC_STATUS_OK)
          {
            abort();
          }
          hxc_enum_4b68fff2_destroy(&hxc_l_g_h6904c3060e30);
          hxc_enum_fcb4401b_destroy(&hxc_l_g_h4ef171f4fe67);
          if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
          {
            abort();
          }
          return;
        }
        else
        {
          hxc_l_gc_roots[49] = (const void *)hxc_l_manifest;
          hxc_l_campaign = hxc_l_manifest;
          hxc_l_gc_roots[50] = (const void *)hxc_l_entry;
          hxc_l_campaignLevel = hxc_l_entry;
        }
        break;
      }
    case hxc_caxecraft_content_CampaignPackageLoadResult_CampaignPackageRejected:
      {
        hxc_l_gc_roots[48] = hxc_l_g_h6904c3060e30.hxc_tag == hxc_caxecraft_content_CampaignPackageLoadResult_CampaignPackageReady ? (const void *)hxc_l_g_h6904c3060e30.hxc_payload.hxc_CampaignPackageReady.hxc_manifest : NULL;
        if (hxc_l_g_h6904c3060e30.hxc_tag != hxc_caxecraft_content_CampaignPackageLoadResult_CampaignPackageRejected)
        {
          abort();
        }
        struct hxc_caxecraft_content_CampaignPackageLoadError hxc_l_tmp_enum_payload_project_n52 = hxc_l_g_h6904c3060e30.hxc_payload.hxc_CampaignPackageRejected.hxc_error;
        hxc_l_gerror = hxc_l_tmp_enum_payload_project_n52;
        if (hxc_enum_7e02e9cb_retain(&hxc_l_gerror) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_l_gerror.hxc_tag == hxc_caxecraft_content_CampaignPackageLoadError_CampaignManifestSourceRejected)
        {
          if (hxc_l_gerror.hxc_tag != hxc_caxecraft_content_CampaignPackageLoadError_CampaignManifestSourceRejected)
          {
            abort();
          }
          struct hxc_caxecraft_content_ContentPackageError hxc_l_tmp_enum_payload_project_n55 = hxc_l_gerror.hxc_payload.hxc_CampaignManifestSourceRejected.hxc_error;
          if (hxc_l_tmp_enum_payload_project_n55.hxc_tag == hxc_caxecraft_content_ContentPackageError_EntryMissing)
          {
            if (hxc_io_println((hxc_string){ (const uint8_t *)"caxecraft: campaign-source=standalone-map", 41, true, NULL }) != HXC_STATUS_OK)
            {
              abort();
            }
            hxc_enum_7e02e9cb_destroy(&hxc_l_gerror);
          }
          else
          {
            if (hxc_io_println((hxc_string){ (const uint8_t *)"caxecraft: campaign manifest rejected: campaigns/first-adventure/campaign.json", 78, true, NULL }) != HXC_STATUS_OK)
            {
              abort();
            }
            hxc_enum_7e02e9cb_destroy(&hxc_l_gerror);
            hxc_enum_4b68fff2_destroy(&hxc_l_g_h6904c3060e30);
            hxc_enum_fcb4401b_destroy(&hxc_l_g_h4ef171f4fe67);
            if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
            {
              abort();
            }
            return;
          }
        }
        else
        {
          if (hxc_io_println((hxc_string){ (const uint8_t *)"caxecraft: campaign manifest rejected: campaigns/first-adventure/campaign.json", 78, true, NULL }) != HXC_STATUS_OK)
          {
            abort();
          }
          hxc_enum_7e02e9cb_destroy(&hxc_l_gerror);
          hxc_enum_4b68fff2_destroy(&hxc_l_g_h6904c3060e30);
          hxc_enum_fcb4401b_destroy(&hxc_l_g_h4ef171f4fe67);
          if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
          {
            abort();
          }
          return;
        }
        break;
      }
    default:
      {
        abort();
      }
  }
  struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_load_result_n56 = hxc_l_campaignLevel;
  hxc_l_gc_roots[51] = (const void *)hxc_l_tmp_load_result_n56;
  hxc_l_gc_roots[52] = (const void *)NULL;
  hxc_string hxc_l_tmp_conditional_managed_result_n25 = { 0 };
  if (hxc_l_tmp_load_result_n56 == NULL)
  {
    struct hxc_caxecraft_content_RuntimeLevelCandidate *hxc_l_tmp_load_result_n57 = hxc_l_loadedCandidate;
    hxc_l_gc_roots[53] = (const void *)hxc_l_tmp_load_result_n57;
    if (hxc_l_tmp_load_result_n57 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_content_RuntimeLevelReceipt hxc_l_tmp_instance_call_result_n58 = hxc_caxecraft_content_RuntimeLevelCandidate_receipt(hxc_l_tmp_load_result_n57);
    hxc_l_tmp_record_field_logicalPath_receiver_owner_n26 = hxc_l_tmp_instance_call_result_n58;
    (void)hxc_l_tmp_record_field_logicalPath_receiver_owner_n26.hxc_logicalPath;
    hxc_l_tmp_conditional_managed_result_n25 = hxc_l_tmp_record_field_logicalPath_receiver_owner_n26.hxc_logicalPath;
    if (hxc_string_retain(hxc_l_tmp_conditional_managed_result_n25) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_record_f6127605_destroy(&hxc_l_tmp_record_field_logicalPath_receiver_owner_n26);
  }
  else
  {
    struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_load_result_n61 = hxc_l_campaignLevel;
    hxc_l_gc_roots[54] = (const void *)hxc_l_tmp_load_result_n61;
    if (hxc_l_tmp_load_result_n61 == NULL)
    {
      abort();
    }
    hxc_string hxc_l_tmp_class_field_load_result_n62 = (*hxc_l_tmp_load_result_n61).hxc_id;
    (void)hxc_l_tmp_class_field_load_result_n62;
    hxc_l_tmp_conditional_managed_result_n25 = hxc_l_tmp_class_field_load_result_n62;
    if (hxc_string_retain(hxc_l_tmp_conditional_managed_result_n25) != HXC_STATUS_OK)
    {
      abort();
    }
  }
  hxc_string hxc_l_tmp_conditional_managed_move_result_n63 = hxc_l_tmp_conditional_managed_result_n25;
  hxc_l_levelLabel = hxc_l_tmp_conditional_managed_move_result_n63;
  int32_t hxc_l_selectedCampaignLevelIndex = 0;
  struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n64 = hxc_l_campaign;
  hxc_l_gc_roots[55] = (const void *)hxc_l_tmp_load_result_n64;
  hxc_l_gc_roots[56] = (const void *)NULL;
  bool hxc_l_tmp_short_circuit_result_n28 = hxc_l_tmp_load_result_n64 != NULL;
  if (hxc_l_tmp_load_result_n64 != NULL)
  {
    hxc_l_gc_roots[57] = (const void *)hxc_l_campaignLevel;
    hxc_l_gc_roots[58] = (const void *)NULL;
    hxc_l_tmp_short_circuit_result_n28 = hxc_l_campaignLevel != NULL;
  }
  if (hxc_l_tmp_short_circuit_result_n28)
  {
    int32_t hxc_l_g_hfd304fde43bf = 0;
    struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n67 = hxc_l_campaign;
    hxc_l_gc_roots[59] = (const void *)hxc_l_tmp_load_result_n67;
    if (hxc_l_tmp_load_result_n67 == NULL)
    {
      abort();
    }
    struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n68 = (*hxc_l_tmp_load_result_n67).hxc_admittedLevels;
    hxc_l_gc_roots[60] = (const void *)hxc_l_tmp_class_field_load_result_n68;
    int32_t hxc_l_tmp_array_length_result_n69;
    if (hxc_array_ref_length(hxc_l_tmp_class_field_load_result_n68, &hxc_l_tmp_array_length_result_n69) != HXC_STATUS_OK)
    {
      abort();
    }
    int32_t hxc_l_g1_ha7d8dc1c4b54 = hxc_l_tmp_array_length_result_n69;
    while (1)
    {
      int32_t hxc_l_tmp_load_result_n70 = hxc_l_g_hfd304fde43bf;
      if (!(hxc_l_tmp_load_result_n70 < hxc_l_g1_ha7d8dc1c4b54))
      {
        break;
      }
      int32_t hxc_l_tmp_increment_load_result_n72 = hxc_l_g_hfd304fde43bf;
      hxc_l_g_hfd304fde43bf = hxc_i32_add_wrapping(hxc_l_tmp_increment_load_result_n72, 1);
      int32_t hxc_l_index_h62f7caa859d1 = hxc_l_tmp_increment_load_result_n72;
      struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n73 = hxc_l_campaign;
      hxc_l_gc_roots[71] = (const void *)hxc_l_tmp_load_result_n73;
      if (hxc_l_tmp_load_result_n73 == NULL)
      {
        abort();
      }
      struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n74 = (*hxc_l_tmp_load_result_n73).hxc_admittedLevels;
      hxc_l_gc_roots[72] = (const void *)hxc_l_tmp_class_field_load_result_n74;
      struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_array_get_result_n76;
      if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n74, (size_t)hxc_l_index_h62f7caa859d1, &hxc_l_tmp_array_get_result_n76) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_l_gc_roots[73] = (const void *)hxc_l_tmp_array_get_result_n76;
      if (hxc_l_tmp_array_get_result_n76 == NULL)
      {
        abort();
      }
      hxc_string hxc_l_tmp_class_field_load_result_n77 = (*hxc_l_tmp_array_get_result_n76).hxc_id;
      struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_load_result_n78 = hxc_l_campaignLevel;
      hxc_l_gc_roots[74] = (const void *)hxc_l_tmp_load_result_n78;
      if (hxc_l_tmp_load_result_n78 == NULL)
      {
        abort();
      }
      hxc_string hxc_l_tmp_class_field_load_result_n79 = (*hxc_l_tmp_load_result_n78).hxc_id;
      if (hxc_l_tmp_class_field_load_result_n77.data == NULL || hxc_l_tmp_class_field_load_result_n79.data == NULL ? hxc_l_tmp_class_field_load_result_n77.data == hxc_l_tmp_class_field_load_result_n79.data : hxc_l_tmp_class_field_load_result_n77.byte_length == hxc_l_tmp_class_field_load_result_n79.byte_length && (hxc_l_tmp_class_field_load_result_n77.byte_length == 0 || memcmp(hxc_l_tmp_class_field_load_result_n77.data, hxc_l_tmp_class_field_load_result_n79.data, hxc_l_tmp_class_field_load_result_n77.byte_length) == 0))
      {
        hxc_l_selectedCampaignLevelIndex = hxc_l_index_h62f7caa859d1;
      }
    }
  }
  struct hxc_caxecraft_app_ActivePlayableLevel *hxc_l_tmp_load_result_n82 = hxc_l_activeLevel;
  hxc_l_gc_roots[61] = (const void *)hxc_l_tmp_load_result_n82;
  if (hxc_l_tmp_load_result_n82 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_class_field_load_result_n83 = (*hxc_l_tmp_load_result_n82).hxc_activeView;
  hxc_l_gc_roots[62] = (const void *)hxc_l_tmp_class_field_load_result_n83;
  struct hxc_caxecraft_app_PlayableLevelView *hxc_l_initialLevel = hxc_l_tmp_class_field_load_result_n83;
  struct hxc_caxecraft_app_ActivePlayableLevel *hxc_l_tmp_load_result_n84 = hxc_l_activeLevel;
  hxc_l_gc_roots[63] = (const void *)hxc_l_tmp_load_result_n84;
  if (hxc_l_tmp_load_result_n84 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_content_ActiveContent *hxc_l_tmp_class_field_load_result_n85 = (*hxc_l_tmp_load_result_n84).hxc_content;
  hxc_l_gc_roots[64] = (const void *)hxc_l_tmp_class_field_load_result_n85;
  if (hxc_l_tmp_class_field_load_result_n85 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_content_LoadedContentGeneration *hxc_l_tmp_class_field_load_result_n86 = (*hxc_l_tmp_class_field_load_result_n85).hxc_active;
  hxc_l_gc_roots[65] = (const void *)hxc_l_tmp_class_field_load_result_n86;
  if (hxc_l_tmp_class_field_load_result_n86 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_class_field_load_result_n87 = (*hxc_l_tmp_class_field_load_result_n86).hxc_ownedSession;
  hxc_l_gc_roots[66] = (const void *)hxc_l_tmp_class_field_load_result_n87;
  struct hxc_caxecraft_domain_GameSession *hxc_l_initialSession = hxc_l_tmp_class_field_load_result_n87;
  struct hxc_caxecraft_content_RuntimeLevelCandidate *hxc_l_tmp_load_result_n88 = hxc_l_loadedCandidate;
  hxc_l_gc_roots[67] = (const void *)hxc_l_tmp_load_result_n88;
  if (hxc_l_tmp_load_result_n88 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_content_RuntimeLevelReceipt hxc_l_tmp_instance_call_result_n89 = hxc_caxecraft_content_RuntimeLevelCandidate_receipt(hxc_l_tmp_load_result_n88);
  hxc_l_receipt = hxc_l_tmp_instance_call_result_n89;
  if (hxc_io_println((hxc_string){ (const uint8_t *)"caxecraft: content-source=runtime-package", 41, true, NULL }) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_string hxc_l_tmp_string_concat_result_n91 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
  if (hxc_string_concat_ref((hxc_string){ (const uint8_t *)"caxecraft: content-path=", 24, true, NULL }, hxc_l_receipt.hxc_logicalPath, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n91) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_tmp_sys_println_string_argument_owner_n35 = hxc_l_tmp_string_concat_result_n91;
  if (hxc_io_println(hxc_l_tmp_sys_println_string_argument_owner_n35) != HXC_STATUS_OK)
  {
    if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n35) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_record_f6127605_destroy(&hxc_l_receipt);
    if (hxc_string_release(&hxc_l_levelLabel) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_enum_4b68fff2_destroy(&hxc_l_g_h6904c3060e30);
    hxc_enum_fcb4401b_destroy(&hxc_l_g_h4ef171f4fe67);
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    abort();
  }
  if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n35) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_string hxc_l_tmp_std_string_int_result_n94 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
  if (hxc_string_from_int32(hxc_l_receipt.hxc_inputHash, hxc_default_allocator(), &hxc_l_tmp_std_string_int_result_n94) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_tmp_string_concat_right_owner_n36 = hxc_l_tmp_std_string_int_result_n94;
  hxc_string hxc_l_tmp_string_concat_result_n96 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
  if (hxc_string_concat_ref((hxc_string){ (const uint8_t *)"caxecraft: content-input-hash=", 30, true, NULL }, hxc_l_tmp_string_concat_right_owner_n36, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n96) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_tmp_sys_println_string_argument_owner_n37 = hxc_l_tmp_string_concat_result_n96;
  if (hxc_io_println(hxc_l_tmp_sys_println_string_argument_owner_n37) != HXC_STATUS_OK)
  {
    if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n37) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n36) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_record_f6127605_destroy(&hxc_l_receipt);
    if (hxc_string_release(&hxc_l_levelLabel) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_enum_4b68fff2_destroy(&hxc_l_g_h6904c3060e30);
    hxc_enum_fcb4401b_destroy(&hxc_l_g_h4ef171f4fe67);
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    abort();
  }
  if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n37) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n36) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_caxecraft_content_RuntimeContentGeneration *hxc_l_tmp_load_result_n98 = hxc_l_runtimeContent;
  hxc_l_gc_roots[68] = (const void *)hxc_l_tmp_load_result_n98;
  if (hxc_l_tmp_load_result_n98 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_content_RuntimeLevelCandidate *hxc_l_tmp_class_field_load_result_n99 = (*hxc_l_tmp_load_result_n98).hxc_loadedLevel;
  hxc_l_gc_roots[69] = (const void *)hxc_l_tmp_class_field_load_result_n99;
  if (hxc_l_tmp_class_field_load_result_n99 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_content_LoadedContentGeneration *hxc_l_tmp_class_field_load_result_n100 = (*hxc_l_tmp_class_field_load_result_n99).hxc_loadedGeneration;
  hxc_l_gc_roots[70] = (const void *)hxc_l_tmp_class_field_load_result_n100;
  if (hxc_l_tmp_class_field_load_result_n100 == NULL)
  {
    abort();
  }
  int32_t hxc_l_tmp_class_field_load_result_n101 = (*hxc_l_tmp_class_field_load_result_n100).hxc_generationIdValue;
  hxc_string hxc_l_tmp_std_string_int_result_n102 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
  if (hxc_string_from_int32(hxc_l_tmp_class_field_load_result_n101, hxc_default_allocator(), &hxc_l_tmp_std_string_int_result_n102) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_tmp_string_concat_right_owner_n38 = hxc_l_tmp_std_string_int_result_n102;
  hxc_string hxc_l_tmp_string_concat_result_n104 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
  if (hxc_string_concat_ref((hxc_string){ (const uint8_t *)"caxecraft: content-generation=", 30, true, NULL }, hxc_l_tmp_string_concat_right_owner_n38, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n104) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_tmp_sys_println_string_argument_owner_n39 = hxc_l_tmp_string_concat_result_n104;
  if (hxc_io_println(hxc_l_tmp_sys_println_string_argument_owner_n39) != HXC_STATUS_OK)
  {
    if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n39) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n38) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_record_f6127605_destroy(&hxc_l_receipt);
    if (hxc_string_release(&hxc_l_levelLabel) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_enum_4b68fff2_destroy(&hxc_l_g_h6904c3060e30);
    hxc_enum_fcb4401b_destroy(&hxc_l_g_h4ef171f4fe67);
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    abort();
  }
  if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n39) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n38) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_windowFlags = hxc_i32_bit_or(64, 4);
  hxc_l_windowFlags = hxc_i32_bit_or(hxc_l_windowFlags, 8192);
  SetConfigFlags((uint32_t)hxc_l_windowFlags);
  InitWindow((int32_t)1280, (int32_t)720, "Caxecraft \342\200\224 Haxe shaped into careful C");
  bool hxc_l_tmp_native_call_result_n108 = IsWindowReady();
  if (!hxc_l_tmp_native_call_result_n108)
  {
    hxc_record_f6127605_destroy(&hxc_l_receipt);
    if (hxc_string_release(&hxc_l_levelLabel) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_enum_4b68fff2_destroy(&hxc_l_g_h6904c3060e30);
    hxc_enum_fcb4401b_destroy(&hxc_l_g_h4ef171f4fe67);
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return;
  }
  SetWindowMinSize((int32_t)800, (int32_t)450);
  SetExitKey((int32_t)0);
  SetTargetFPS((int32_t)120);
  EnableCursor();
  struct Texture hxc_l_tmp_native_call_result_n109 = LoadTexture("assets/showcase/title-panorama.png");
  struct Texture hxc_l_titleTexture = hxc_l_tmp_native_call_result_n109;
  bool hxc_l_tmp_native_call_result_n111 = IsTextureValid(hxc_l_titleTexture);
  bool hxc_l_titleTextureReady = hxc_l_tmp_native_call_result_n111;
  struct Texture hxc_l_tmp_native_call_result_n112 = LoadTexture("assets/branding/caxecraft-wordmark.png");
  struct Texture hxc_l_wordmarkTexture = hxc_l_tmp_native_call_result_n112;
  bool hxc_l_tmp_native_call_result_n114 = IsTextureValid(hxc_l_wordmarkTexture);
  bool hxc_l_wordmarkTextureReady = hxc_l_tmp_native_call_result_n114;
  struct Texture hxc_l_tmp_native_call_result_n115 = LoadTexture("assets/atlases/hud.png");
  struct Texture hxc_l_hudTexture = hxc_l_tmp_native_call_result_n115;
  bool hxc_l_tmp_native_call_result_n117 = IsTextureValid(hxc_l_hudTexture);
  bool hxc_l_hudTextureReady = hxc_l_tmp_native_call_result_n117;
  struct Texture hxc_l_tmp_native_call_result_n118 = LoadTexture("assets/atlases/items.png");
  struct Texture hxc_l_itemTexture = hxc_l_tmp_native_call_result_n118;
  bool hxc_l_tmp_native_call_result_n120 = IsTextureValid(hxc_l_itemTexture);
  bool hxc_l_itemTextureReady = hxc_l_tmp_native_call_result_n120;
  struct Texture hxc_l_tmp_native_call_result_n121 = LoadTexture("assets/atlases/adventure-items.png");
  struct Texture hxc_l_adventureItemTexture = hxc_l_tmp_native_call_result_n121;
  bool hxc_l_tmp_native_call_result_n123 = IsTextureValid(hxc_l_adventureItemTexture);
  bool hxc_l_adventureItemTextureReady = hxc_l_tmp_native_call_result_n123;
  struct Texture hxc_l_tmp_load_result_n124 = hxc_l_hudTexture;
  bool hxc_l_tmp_load_result_n125 = hxc_l_hudTextureReady;
  struct Texture hxc_l_tmp_load_result_n126 = hxc_l_itemTexture;
  bool hxc_l_tmp_load_result_n127 = hxc_l_itemTextureReady;
  struct Texture hxc_l_tmp_load_result_n128 = hxc_l_adventureItemTexture;
  struct hxc_caxecraft_app_HudResources hxc_l_hudResources = (struct hxc_caxecraft_app_HudResources){ .hxc_adventureItemTexture = hxc_l_tmp_load_result_n128, .hxc_adventureItemTextureReady = hxc_l_adventureItemTextureReady, .hxc_hudTexture = hxc_l_tmp_load_result_n124, .hxc_hudTextureReady = hxc_l_tmp_load_result_n125, .hxc_itemTexture = hxc_l_tmp_load_result_n126, .hxc_itemTextureReady = hxc_l_tmp_load_result_n127 };
  struct Texture hxc_l_tmp_native_call_result_n131 = LoadTexture("assets/atlases/entities.png");
  struct Texture hxc_l_entityTexture = hxc_l_tmp_native_call_result_n131;
  bool hxc_l_tmp_native_call_result_n133 = IsTextureValid(hxc_l_entityTexture);
  bool hxc_l_entityTextureReady = hxc_l_tmp_native_call_result_n133;
  struct Texture hxc_l_tmp_native_call_result_n134 = LoadTexture("assets/atlases/terrain.png");
  struct Texture hxc_l_terrainTexture = hxc_l_tmp_native_call_result_n134;
  bool hxc_l_tmp_native_call_result_n136 = IsTextureValid(hxc_l_terrainTexture);
  bool hxc_l_terrainTextureReady = hxc_l_tmp_native_call_result_n136;
  struct Texture hxc_l_tmp_native_call_result_n137 = LoadTexture("assets/atlases/adventure-terrain.png");
  struct Texture hxc_l_adventureTerrainTexture = hxc_l_tmp_native_call_result_n137;
  bool hxc_l_tmp_native_call_result_n139 = IsTextureValid(hxc_l_adventureTerrainTexture);
  bool hxc_l_adventureTerrainTextureReady = hxc_l_tmp_native_call_result_n139;
  double hxc_l_cameraWaterBlend = 0.0;
  struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n140 = hxc_caxecraft_gameplay_Inventory_starter();
  struct hxc_caxecraft_gameplay_InventoryState hxc_l_inventory = hxc_l_tmp_call_result_n140;
  hxc_l_activeDialogue = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
  if (hxc_string_retain(hxc_l_activeDialogue) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_latestJournalId = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
  if (hxc_string_retain(hxc_l_latestJournalId) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n141 = hxc_l_initialLevel;
  hxc_l_gc_roots[75] = (const void *)hxc_l_tmp_load_result_n141;
  if (hxc_l_tmp_load_result_n141 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_class_field_load_result_n142 = (*hxc_l_tmp_load_result_n141).hxc_levelPresentation;
  hxc_l_gc_roots[76] = (const void *)hxc_l_tmp_class_field_load_result_n142;
  if (hxc_l_tmp_class_field_load_result_n142 == NULL)
  {
    abort();
  }
  hxc_string hxc_l_tmp_class_field_load_result_n143 = (*hxc_l_tmp_class_field_load_result_n142).hxc_startingObjective;
  hxc_l_currentObjectiveId = hxc_l_tmp_class_field_load_result_n143;
  if (hxc_string_retain(hxc_l_currentObjectiveId) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n144 = hxc_l_initialSession;
  hxc_l_gc_roots[77] = (const void *)hxc_l_tmp_load_result_n144;
  hxc_l_gc_roots[78] = (const void *)hxc_l_initialLevel;
  struct hxc_caxecraft_app_CaxecraftApp_AvailableInteractionTarget hxc_l_tmp_call_result_n146 = hxc_caxecraft_app_CaxecraftApp_nearestAvailableInteraction(hxc_l_tmp_load_result_n144, hxc_l_initialLevel);
  hxc_l_tmp_static_call_argument_0_owner_n64 = hxc_l_tmp_call_result_n146;
  bool hxc_l_tmp_call_result_n148 = hxc_caxecraft_app_CaxecraftApp_hasAvailableInteraction(hxc_l_tmp_static_call_argument_0_owner_n64);
  bool hxc_l_guideInteractionAvailable = hxc_l_tmp_call_result_n148;
  struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n149 = hxc_l_initialSession;
  hxc_l_gc_roots[79] = (const void *)hxc_l_tmp_load_result_n149;
  if (hxc_l_tmp_load_result_n149 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n150 = hxc_l_initialLevel;
  hxc_l_gc_roots[80] = (const void *)hxc_l_tmp_load_result_n150;
  if (hxc_l_tmp_load_result_n150 == NULL)
  {
    abort();
  }
  int32_t hxc_l_tmp_class_field_load_result_n151 = (*hxc_l_tmp_load_result_n150).hxc_enemyId;
  struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n152 = hxc_caxecraft_domain_GameSession_readCharacter(hxc_l_tmp_load_result_n149, hxc_l_tmp_class_field_load_result_n151);
  struct hxc_caxecraft_domain_Character hxc_l_enemyActor = hxc_l_tmp_instance_call_result_n152;
  struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n153 = hxc_l_initialSession;
  hxc_l_gc_roots[81] = (const void *)hxc_l_tmp_load_result_n153;
  if (hxc_l_tmp_load_result_n153 == NULL)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_tmp_instance_call_result_n154 = hxc_caxecraft_domain_GameSession_actorControllerStateSnapshots(hxc_l_tmp_load_result_n153);
  hxc_l_initialActorPhases = hxc_l_tmp_instance_call_result_n154;
  struct hxc_array_ref *hxc_l_tmp_load_result_n155 = hxc_l_initialActorPhases;
  struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n156 = hxc_l_initialLevel;
  hxc_l_gc_roots[82] = (const void *)hxc_l_tmp_load_result_n156;
  if (hxc_l_tmp_load_result_n156 == NULL)
  {
    abort();
  }
  int32_t hxc_l_tmp_class_field_load_result_n157 = (*hxc_l_tmp_load_result_n156).hxc_enemyId;
  struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation hxc_l_tmp_call_result_n159 = hxc_caxecraft_app_CaxecraftApp_observeActorPhase(hxc_l_tmp_load_result_n155, hxc_l_tmp_class_field_load_result_n157, hxc_caxecraft_domain_ActorControllerPhase_Resting);
  struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation hxc_l_enemyPhase_h70382f3bdc66 = hxc_l_tmp_call_result_n159;
  struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n160 = hxc_l_initialSession;
  hxc_l_gc_roots[83] = (const void *)hxc_l_tmp_load_result_n160;
  struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n161 = hxc_l_initialLevel;
  hxc_l_gc_roots[84] = (const void *)hxc_l_tmp_load_result_n161;
  bool hxc_l_tmp_call_result_n163 = hxc_caxecraft_app_CaxecraftApp_dialogueActorsAreValid(hxc_l_tmp_load_result_n160, hxc_l_tmp_load_result_n161, hxc_l_initialActorPhases);
  bool hxc_l_tmp_short_circuit_result_n68 = !hxc_l_tmp_call_result_n163;
  if (!!hxc_l_tmp_call_result_n163)
  {
    hxc_l_tmp_short_circuit_result_n68 = hxc_l_enemyActor.hxc_id <= 0;
  }
  bool hxc_l_tmp_short_circuit_load_result_n165 = hxc_l_tmp_short_circuit_result_n68;
  bool hxc_l_tmp_short_circuit_result_n69 = hxc_l_tmp_short_circuit_load_result_n165;
  if (!hxc_l_tmp_short_circuit_load_result_n165)
  {
    hxc_l_tmp_short_circuit_result_n69 = !hxc_l_enemyPhase_h70382f3bdc66.hxc_valid;
  }
  if (hxc_l_tmp_short_circuit_result_n69)
  {
    if (hxc_array_ref_release(hxc_l_initialActorPhases) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_enum_2c0e695d_destroy(&hxc_l_tmp_static_call_argument_0_owner_n64);
    if (hxc_string_release(&hxc_l_currentObjectiveId) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_latestJournalId) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_activeDialogue) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_record_f6127605_destroy(&hxc_l_receipt);
    if (hxc_string_release(&hxc_l_levelLabel) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_enum_4b68fff2_destroy(&hxc_l_g_h6904c3060e30);
    hxc_enum_fcb4401b_destroy(&hxc_l_g_h4ef171f4fe67);
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return;
  }
  struct hxc_caxecraft_gameplay_SwordCombatState hxc_l_swordCombat = (struct hxc_caxecraft_gameplay_SwordCombatState){ .hxc_cooldownTicks = 0 };
  struct hxc_caxecraft_gameplay_BerryDropState hxc_l_berryDrop = (struct hxc_caxecraft_gameplay_BerryDropState){ .hxc_active = false, .hxc_amount = 0, .hxc_x = 0.0, .hxc_y = 0.0, .hxc_z = 0.0 };
  double hxc_l_lookX = 0.0;
  double hxc_l_lookY = -0.18;
  double hxc_l_lookZ = -1.0;
  double hxc_l_accumulator = 0.0;
  struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n170 = hxc_l_initialSession;
  hxc_l_gc_roots[85] = (const void *)hxc_l_tmp_load_result_n170;
  if (hxc_l_tmp_load_result_n170 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_domain_GameView hxc_l_tmp_instance_call_result_n171 = hxc_caxecraft_domain_GameSession_view(hxc_l_tmp_load_result_n170);
  struct hxc_caxecraft_domain_GameView hxc_l_initialPresentation = hxc_l_tmp_instance_call_result_n171;
  if (!hxc_l_initialPresentation.hxc_valid)
  {
    if (hxc_array_ref_release(hxc_l_initialActorPhases) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_enum_2c0e695d_destroy(&hxc_l_tmp_static_call_argument_0_owner_n64);
    if (hxc_string_release(&hxc_l_currentObjectiveId) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_latestJournalId) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_activeDialogue) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_record_f6127605_destroy(&hxc_l_receipt);
    if (hxc_string_release(&hxc_l_levelLabel) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_enum_4b68fff2_destroy(&hxc_l_g_h6904c3060e30);
    hxc_enum_fcb4401b_destroy(&hxc_l_g_h4ef171f4fe67);
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return;
  }
  struct hxc_caxecraft_app_MotionHistory hxc_l_tmp_call_result_n174 = hxc_caxecraft_app_MotionInterpolation_start(hxc_l_initialPresentation.hxc_localPlayer.hxc_body);
  struct hxc_caxecraft_app_MotionHistory hxc_l_motionHistory = hxc_l_tmp_call_result_n174;
  bool hxc_l_jumpQueued = false;
  bool hxc_l_swordQueued = false;
  int32_t hxc_l_selectedMode = 0;
  int32_t hxc_l_locale = 0;
  bool hxc_l_showInitialTitle = true;
  enum hxc_caxecraft_app_AppScreen hxc_l_tmp_call_result_n176 = hxc_caxecraft_app_AppScreen_initialScreen(hxc_l_showInitialTitle);
  enum hxc_caxecraft_app_AppScreen hxc_l_screen = hxc_l_tmp_call_result_n176;
  bool hxc_l_tmp_call_result_n178 = hxc_caxecraft_app_AppScreen_capturesPointer(hxc_l_screen);
  if (hxc_l_tmp_call_result_n178)
  {
    DisableCursor();
  }
  bool hxc_l_quit = false;
  int32_t hxc_l_frameCount = 0;
  int32_t hxc_l_placementBlockedFrames = 0;
  int32_t hxc_l_strikeHitFrames = 0;
  int32_t hxc_l_enemyDefeatedFrames = 0;
  int32_t hxc_l_enemyAttackFrames = 0;
  int32_t hxc_l_pickupFrames = 0;
  int32_t hxc_l_pickupAmount = 0;
  int32_t hxc_l_inventoryFullFrames = 0;
  int32_t hxc_l_inventoryFullReason = 0;
  int32_t hxc_l_recoveryFeedback = 0;
  int32_t hxc_l_recoveryFeedbackFrames = 0;
  int32_t hxc_l_aquaticEquipmentCode = -1;
  int32_t hxc_l_aquaticEquipmentFrames = 0;
  while (1)
  {
    bool hxc_l_tmp_load_result_n179 = hxc_l_quit;
    bool hxc_l_tmp_short_circuit_result_n98 = !hxc_l_tmp_load_result_n179;
    if (!hxc_l_tmp_load_result_n179)
    {
      bool hxc_l_tmp_native_call_result_n180 = WindowShouldClose();
      hxc_l_tmp_short_circuit_result_n98 = !hxc_l_tmp_native_call_result_n180;
    }
    if (!hxc_l_tmp_short_circuit_result_n98)
    {
      break;
    }
    struct hxc_caxecraft_app_ActivePlayableLevel *hxc_l_tmp_load_result_n182 = hxc_l_activeLevel;
    hxc_l_gc_roots[86] = (const void *)hxc_l_tmp_load_result_n182;
    if (hxc_l_tmp_load_result_n182 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_class_field_load_result_n183 = (*hxc_l_tmp_load_result_n182).hxc_activeView;
    hxc_l_gc_roots[87] = (const void *)hxc_l_tmp_class_field_load_result_n183;
    struct hxc_caxecraft_app_PlayableLevelView *hxc_l_levelView_ha9fa307295b9 = hxc_l_tmp_class_field_load_result_n183;
    struct hxc_caxecraft_app_ActivePlayableLevel *hxc_l_tmp_load_result_n184 = hxc_l_activeLevel;
    hxc_l_gc_roots[88] = (const void *)hxc_l_tmp_load_result_n184;
    if (hxc_l_tmp_load_result_n184 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_content_ActiveContent *hxc_l_tmp_class_field_load_result_n185 = (*hxc_l_tmp_load_result_n184).hxc_content;
    hxc_l_gc_roots[89] = (const void *)hxc_l_tmp_class_field_load_result_n185;
    if (hxc_l_tmp_class_field_load_result_n185 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_content_LoadedContentGeneration *hxc_l_tmp_class_field_load_result_n186 = (*hxc_l_tmp_class_field_load_result_n185).hxc_active;
    hxc_l_gc_roots[90] = (const void *)hxc_l_tmp_class_field_load_result_n186;
    if (hxc_l_tmp_class_field_load_result_n186 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_class_field_load_result_n187 = (*hxc_l_tmp_class_field_load_result_n186).hxc_ownedSession;
    hxc_l_gc_roots[91] = (const void *)hxc_l_tmp_class_field_load_result_n187;
    struct hxc_caxecraft_domain_GameSession *hxc_l_session = hxc_l_tmp_class_field_load_result_n187;
    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n188 = hxc_l_session;
    hxc_l_gc_roots[92] = (const void *)hxc_l_tmp_load_result_n188;
    if (hxc_l_tmp_load_result_n188 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_domain_GameView hxc_l_tmp_instance_call_result_n189 = hxc_caxecraft_domain_GameSession_view(hxc_l_tmp_load_result_n188);
    struct hxc_caxecraft_domain_GameView hxc_l_initialView = hxc_l_tmp_instance_call_result_n189;
    if (!hxc_l_initialView.hxc_valid)
    {
      hxc_l_quit = true;
    }
    struct hxc_caxecraft_domain_Character hxc_l_character = hxc_l_initialView.hxc_localPlayer;
    bool hxc_l_recapturedThisFrame = false;
    bool hxc_l_resetMotionThisFrame = false;
    float hxc_l_tmp_native_call_result_n192 = GetFrameTime();
    double hxc_l_frameSeconds = (double)hxc_l_tmp_native_call_result_n192;
    if (hxc_l_frameSeconds > 0.25)
    {
      hxc_l_frameSeconds = 0.25;
    }
    bool hxc_l_tmp_native_call_result_n194 = IsWindowFocused();
    bool hxc_l_focused = hxc_l_tmp_native_call_result_n194;
    bool hxc_l_tmp_call_result_n196 = hxc_caxecraft_app_AppScreen_capturesPointer(hxc_l_screen);
    bool hxc_l_captured_h891cd0b43970 = hxc_l_tmp_call_result_n196;
    bool hxc_l_tmp_call_result_n198 = hxc_caxecraft_app_AppScreen_pausesSimulation(hxc_l_screen);
    bool hxc_l_paused_hd9b792e32d4e = hxc_l_tmp_call_result_n198;
    double hxc_l_forward = 0.0;
    double hxc_l_right = 0.0;
    bool hxc_l_tmp_native_call_result_n199 = IsKeyDown((int32_t)87);
    if (hxc_l_tmp_native_call_result_n199)
    {
      hxc_l_forward = hxc_l_forward + 1.0;
    }
    bool hxc_l_tmp_native_call_result_n201 = IsKeyDown((int32_t)83);
    if (hxc_l_tmp_native_call_result_n201)
    {
      hxc_l_forward = hxc_l_forward - 1.0;
    }
    bool hxc_l_tmp_native_call_result_n203 = IsKeyDown((int32_t)68);
    if (hxc_l_tmp_native_call_result_n203)
    {
      hxc_l_right = hxc_l_right + 1.0;
    }
    bool hxc_l_tmp_native_call_result_n205 = IsKeyDown((int32_t)65);
    if (hxc_l_tmp_native_call_result_n205)
    {
      hxc_l_right = hxc_l_right - 1.0;
    }
    double hxc_l_lookYaw_h5bdd5686635d = 0.0;
    double hxc_l_lookPitch_ha7ac9f537d74 = 0.0;
    if (hxc_l_captured_h891cd0b43970)
    {
      struct Vector2 hxc_l_tmp_native_call_result_n208 = GetMouseDelta();
      struct Vector2 hxc_l_mouse = hxc_l_tmp_native_call_result_n208;
      hxc_l_lookYaw_h5bdd5686635d = -(double)hxc_l_mouse.x * 0.0025;
      hxc_l_lookPitch_ha7ac9f537d74 = -(double)hxc_l_mouse.y * 0.0025;
    }
    bool hxc_l_tmp_native_call_result_n213 = IsMouseButtonPressed((int32_t)0);
    bool hxc_l_leftPressed = hxc_l_tmp_native_call_result_n213;
    bool hxc_l_tmp_load_result_n214 = hxc_l_captured_h891cd0b43970;
    bool hxc_l_tmp_short_circuit_result_n116 = hxc_l_tmp_load_result_n214;
    if (hxc_l_tmp_load_result_n214)
    {
      hxc_l_tmp_short_circuit_result_n116 = hxc_l_leftPressed;
    }
    bool hxc_l_primaryPressed_h1af73905bdff = hxc_l_tmp_short_circuit_result_n116;
    bool hxc_l_tmp_load_result_n217 = hxc_l_captured_h891cd0b43970;
    bool hxc_l_tmp_short_circuit_result_n118 = hxc_l_tmp_load_result_n217;
    if (hxc_l_tmp_load_result_n217)
    {
      bool hxc_l_tmp_native_call_result_n218 = IsMouseButtonPressed((int32_t)1);
      hxc_l_tmp_short_circuit_result_n118 = hxc_l_tmp_native_call_result_n218;
    }
    bool hxc_l_secondaryPressed_ha8c84c3f2476 = hxc_l_tmp_short_circuit_result_n118;
    bool hxc_l_tmp_load_result_n220 = hxc_l_captured_h891cd0b43970;
    bool hxc_l_tmp_short_circuit_result_n120 = hxc_l_tmp_load_result_n220;
    if (hxc_l_tmp_load_result_n220)
    {
      bool hxc_l_tmp_native_call_result_n221 = IsKeyPressed((int32_t)69);
      hxc_l_tmp_short_circuit_result_n120 = hxc_l_tmp_native_call_result_n221;
    }
    bool hxc_l_interactPressed_h709cc3e1ed4a = hxc_l_tmp_short_circuit_result_n120;
    bool hxc_l_tmp_load_result_n223 = hxc_l_captured_h891cd0b43970;
    bool hxc_l_tmp_short_circuit_result_n122 = hxc_l_tmp_load_result_n223;
    if (hxc_l_tmp_load_result_n223)
    {
      bool hxc_l_tmp_native_call_result_n224 = IsKeyPressed((int32_t)78);
      hxc_l_tmp_short_circuit_result_n122 = hxc_l_tmp_native_call_result_n224;
    }
    bool hxc_l_travelPressed_h8bd5ddb8a405 = hxc_l_tmp_short_circuit_result_n122;
    int32_t hxc_l_hotbarSelection_hb4d44b3bf26a = -1;
    bool hxc_l_tmp_native_call_result_n226 = IsKeyPressed((int32_t)49);
    if (hxc_l_tmp_native_call_result_n226)
    {
      hxc_l_hotbarSelection_hb4d44b3bf26a = 0;
    }
    bool hxc_l_tmp_native_call_result_n227 = IsKeyPressed((int32_t)50);
    if (hxc_l_tmp_native_call_result_n227)
    {
      hxc_l_hotbarSelection_hb4d44b3bf26a = 1;
    }
    bool hxc_l_tmp_native_call_result_n228 = IsKeyPressed((int32_t)51);
    if (hxc_l_tmp_native_call_result_n228)
    {
      hxc_l_hotbarSelection_hb4d44b3bf26a = 2;
    }
    bool hxc_l_tmp_native_call_result_n229 = IsKeyPressed((int32_t)52);
    if (hxc_l_tmp_native_call_result_n229)
    {
      hxc_l_hotbarSelection_hb4d44b3bf26a = 3;
    }
    bool hxc_l_tmp_native_call_result_n230 = IsKeyPressed((int32_t)53);
    if (hxc_l_tmp_native_call_result_n230)
    {
      hxc_l_hotbarSelection_hb4d44b3bf26a = 4;
    }
    bool hxc_l_tmp_native_call_result_n231 = IsKeyPressed((int32_t)54);
    if (hxc_l_tmp_native_call_result_n231)
    {
      hxc_l_hotbarSelection_hb4d44b3bf26a = 5;
    }
    bool hxc_l_tmp_native_call_result_n232 = IsKeyPressed((int32_t)55);
    if (hxc_l_tmp_native_call_result_n232)
    {
      hxc_l_hotbarSelection_hb4d44b3bf26a = 6;
    }
    bool hxc_l_tmp_native_call_result_n233 = IsKeyPressed((int32_t)56);
    if (hxc_l_tmp_native_call_result_n233)
    {
      hxc_l_hotbarSelection_hb4d44b3bf26a = 7;
    }
    float hxc_l_tmp_native_call_result_n234 = GetMouseWheelMove();
    double hxc_l_wheel = (double)hxc_l_tmp_native_call_result_n234;
    int32_t hxc_l_hotbarCycle_ha5fc89c5537e = 0;
    if (hxc_l_wheel > 0.0)
    {
      hxc_l_hotbarCycle_ha5fc89c5537e = -1;
    }
    if (hxc_l_wheel < 0.0)
    {
      hxc_l_hotbarCycle_ha5fc89c5537e = 1;
    }
    bool hxc_l_tmp_native_call_result_n237 = IsKeyPressed((int32_t)32);
    bool hxc_l_jumpPressed_he77b75182b6a = hxc_l_tmp_native_call_result_n237;
    bool hxc_l_tmp_native_call_result_n238 = IsKeyPressed((int32_t)256);
    bool hxc_l_pausePressed_hc02c74ed5d48 = hxc_l_tmp_native_call_result_n238;
    bool hxc_l_tmp_native_call_result_n239 = IsKeyPressed((int32_t)81);
    bool hxc_l_quitPressed_h4a48ed0f0cc2 = hxc_l_tmp_native_call_result_n239;
    bool hxc_l_tmp_native_call_result_n240 = IsKeyDown((int32_t)340);
    bool hxc_l_descendHeld_hd0168cfe3edc = hxc_l_tmp_native_call_result_n240;
    bool hxc_l_tmp_native_call_result_n241 = IsKeyPressed((int32_t)265);
    bool hxc_l_tmp_short_circuit_result_n131 = hxc_l_tmp_native_call_result_n241;
    if (!hxc_l_tmp_native_call_result_n241)
    {
      bool hxc_l_tmp_native_call_result_n242 = IsKeyPressed((int32_t)264);
      hxc_l_tmp_short_circuit_result_n131 = hxc_l_tmp_native_call_result_n242;
    }
    bool hxc_l_menuNextPressed_hf7c2e9ba9e0d = hxc_l_tmp_short_circuit_result_n131;
    bool hxc_l_tmp_native_call_result_n244 = IsKeyPressed((int32_t)257);
    bool hxc_l_menuConfirmPressed_hed3d6ffbd7c7 = hxc_l_tmp_native_call_result_n244;
    double hxc_l_frameInput_moveForward = hxc_l_forward;
    double hxc_l_frameInput_moveRight = hxc_l_right;
    double hxc_l_frameInput_lookYaw = hxc_l_lookYaw_h5bdd5686635d;
    double hxc_l_frameInput_lookPitch = hxc_l_lookPitch_ha7ac9f537d74;
    bool hxc_l_frameInput_jumpPressed = hxc_l_jumpPressed_he77b75182b6a;
    bool hxc_l_tmp_native_call_result_n250 = IsKeyDown((int32_t)32);
    bool hxc_l_frameInput_riseHeld = hxc_l_tmp_native_call_result_n250;
    bool hxc_l_frameInput_descendHeld = hxc_l_descendHeld_hd0168cfe3edc;
    bool hxc_l_frameInput_primaryPressed = hxc_l_primaryPressed_h1af73905bdff;
    bool hxc_l_frameInput_secondaryPressed = hxc_l_secondaryPressed_ha8c84c3f2476;
    bool hxc_l_frameInput_interactPressed = hxc_l_interactPressed_h709cc3e1ed4a;
    bool hxc_l_frameInput_travelPressed = hxc_l_travelPressed_h8bd5ddb8a405;
    bool hxc_l_frameInput_menuNextPressed = hxc_l_menuNextPressed_hf7c2e9ba9e0d;
    bool hxc_l_frameInput_menuConfirmPressed = hxc_l_menuConfirmPressed_hed3d6ffbd7c7;
    bool hxc_l_frameInput_pausePressed = hxc_l_pausePressed_hc02c74ed5d48;
    bool hxc_l_tmp_load_result_n259 = hxc_l_paused_hd9b792e32d4e;
    bool hxc_l_tmp_short_circuit_result_n148 = hxc_l_tmp_load_result_n259;
    if (hxc_l_tmp_load_result_n259)
    {
      hxc_l_tmp_short_circuit_result_n148 = hxc_l_leftPressed;
    }
    bool hxc_l_frameInput_capturePressed = hxc_l_tmp_short_circuit_result_n148;
    bool hxc_l_frameInput_quitPressed = hxc_l_quitPressed_h4a48ed0f0cc2;
    int32_t hxc_l_frameInput_hotbarSelection = hxc_l_hotbarSelection_hb4d44b3bf26a;
    int32_t hxc_l_frameInput_hotbarCycle = hxc_l_hotbarCycle_ha5fc89c5537e;
    double hxc_l_moveForward = hxc_l_frameInput_moveForward;
    double hxc_l_moveRight = hxc_l_frameInput_moveRight;
    double hxc_l_lookYaw_hb85067cf1c5a = hxc_l_frameInput_lookYaw;
    double hxc_l_lookPitch_h7ef5aa1a8cda = hxc_l_frameInput_lookPitch;
    bool hxc_l_jumpPressed_hc40fd78ea0d6 = hxc_l_frameInput_jumpPressed;
    bool hxc_l_riseHeld = hxc_l_frameInput_riseHeld;
    bool hxc_l_primaryPressed_h4a0a8b20cf08 = hxc_l_frameInput_primaryPressed;
    bool hxc_l_secondaryPressed_h3c635e3846cc = hxc_l_frameInput_secondaryPressed;
    bool hxc_l_interactPressed_h550f33a83547 = hxc_l_frameInput_interactPressed;
    bool hxc_l_travelPressed_h11a77c091cdf = hxc_l_frameInput_travelPressed;
    bool hxc_l_pausePressed_h952cc9de954a = hxc_l_frameInput_pausePressed;
    bool hxc_l_capturePressed = hxc_l_frameInput_capturePressed;
    bool hxc_l_quitPressed_hee357f1e842f = hxc_l_frameInput_quitPressed;
    int32_t hxc_l_hotbarSelection_h4d15508de09d = hxc_l_frameInput_hotbarSelection;
    int32_t hxc_l_hotbarCycle_he98d5700e593 = hxc_l_frameInput_hotbarCycle;
    bool hxc_l_menuNextPressed_h4b7fca18ecc3 = hxc_l_frameInput_menuNextPressed;
    bool hxc_l_menuConfirmPressed_h30f1449eeac0 = hxc_l_frameInput_menuConfirmPressed;
    bool hxc_l_descendHeld_h962d79adb546 = hxc_l_frameInput_descendHeld;
    hxc_l_gc_roots[93] = (const void *)NULL;
    struct hxc_caxecraft_content_CampaignLevel *hxc_l_requestedCampaignLevel = NULL;
    bool hxc_l_requestedFromCampaignMenu = false;
    bool hxc_l_tmp_load_result_n283 = hxc_l_quit;
    bool hxc_l_tmp_short_circuit_result_n172 = !hxc_l_tmp_load_result_n283;
    if (!hxc_l_tmp_load_result_n283)
    {
      enum hxc_caxecraft_app_AppScreen hxc_l_tmp_load_result_n284 = hxc_l_screen;
      bool hxc_l_tmp_call_result_n286 = hxc_caxecraft_app_TitleMenuFlow_allowsCampaignTravel(hxc_l_tmp_load_result_n284, hxc_l_selectedMode);
      hxc_l_tmp_short_circuit_result_n172 = hxc_l_tmp_call_result_n286;
    }
    bool hxc_l_tmp_short_circuit_load_result_n287 = hxc_l_tmp_short_circuit_result_n172;
    bool hxc_l_tmp_short_circuit_result_n173 = hxc_l_tmp_short_circuit_load_result_n287;
    if (hxc_l_tmp_short_circuit_load_result_n287)
    {
      hxc_l_tmp_short_circuit_result_n173 = hxc_l_travelPressed_h11a77c091cdf;
    }
    if (hxc_l_tmp_short_circuit_result_n173)
    {
      hxc_l_gc_roots[94] = (const void *)hxc_l_campaign;
      struct hxc_caxecraft_content_CampaignManifest *hxc_l_selectedCampaign_h28bf179e07a3 = hxc_l_campaign;
      hxc_l_gc_roots[95] = (const void *)hxc_l_campaignLevel;
      struct hxc_caxecraft_content_CampaignLevel *hxc_l_sourceLevel = hxc_l_campaignLevel;
      struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n292 = hxc_l_selectedCampaign_h28bf179e07a3;
      hxc_l_gc_roots[96] = (const void *)hxc_l_tmp_load_result_n292;
      hxc_l_gc_roots[97] = (const void *)NULL;
      bool hxc_l_tmp_short_circuit_result_n176 = hxc_l_tmp_load_result_n292 != NULL;
      if (hxc_l_tmp_load_result_n292 != NULL)
      {
        hxc_l_gc_roots[98] = (const void *)hxc_l_sourceLevel;
        hxc_l_gc_roots[99] = (const void *)NULL;
        hxc_l_tmp_short_circuit_result_n176 = hxc_l_sourceLevel != NULL;
      }
      if (hxc_l_tmp_short_circuit_result_n176)
      {
        struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n295 = hxc_l_selectedCampaign_h28bf179e07a3;
        hxc_l_gc_roots[100] = (const void *)hxc_l_tmp_load_result_n295;
        if (hxc_l_tmp_load_result_n295 == NULL)
        {
          abort();
        }
        struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_load_result_n296 = hxc_l_sourceLevel;
        hxc_l_gc_roots[101] = (const void *)hxc_l_tmp_load_result_n296;
        if (hxc_l_tmp_load_result_n296 == NULL)
        {
          abort();
        }
        hxc_string hxc_l_tmp_class_field_load_result_n297 = (*hxc_l_tmp_load_result_n296).hxc_id;
        struct hxc_caxecraft_content_CampaignTransition *hxc_l_tmp_instance_call_result_n298 = hxc_caxecraft_content_CampaignManifest_unambiguousTransitionFrom(hxc_l_tmp_load_result_n295, hxc_l_tmp_class_field_load_result_n297);
        hxc_l_gc_roots[102] = (const void *)hxc_l_tmp_instance_call_result_n298;
        struct hxc_caxecraft_content_CampaignTransition *hxc_l_transition = hxc_l_tmp_instance_call_result_n298;
        hxc_l_gc_roots[103] = (const void *)hxc_l_transition;
        hxc_l_gc_roots[104] = (const void *)NULL;
        if (hxc_l_transition == NULL)
        {
          struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_load_result_n300 = hxc_l_sourceLevel;
          hxc_l_gc_roots[105] = (const void *)hxc_l_tmp_load_result_n300;
          if (hxc_l_tmp_load_result_n300 == NULL)
          {
            abort();
          }
          hxc_string hxc_l_tmp_class_field_load_result_n301 = (*hxc_l_tmp_load_result_n300).hxc_id;
          hxc_string hxc_l_tmp_string_concat_result_n302 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
          if (hxc_string_concat_ref((hxc_string){ (const uint8_t *)"caxecraft: campaign travel requires exactly one outgoing transition from ", 73, true, NULL }, hxc_l_tmp_class_field_load_result_n301, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n302) != HXC_STATUS_OK)
          {
            abort();
          }
          hxc_l_tmp_sys_println_string_argument_owner_n178 = hxc_l_tmp_string_concat_result_n302;
          if (hxc_io_println(hxc_l_tmp_sys_println_string_argument_owner_n178) != HXC_STATUS_OK)
          {
            if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n178) != HXC_STATUS_OK)
            {
              abort();
            }
            if (hxc_array_ref_release(hxc_l_initialActorPhases) != HXC_STATUS_OK)
            {
              abort();
            }
            hxc_enum_2c0e695d_destroy(&hxc_l_tmp_static_call_argument_0_owner_n64);
            if (hxc_string_release(&hxc_l_currentObjectiveId) != HXC_STATUS_OK)
            {
              abort();
            }
            if (hxc_string_release(&hxc_l_latestJournalId) != HXC_STATUS_OK)
            {
              abort();
            }
            if (hxc_string_release(&hxc_l_activeDialogue) != HXC_STATUS_OK)
            {
              abort();
            }
            hxc_record_f6127605_destroy(&hxc_l_receipt);
            if (hxc_string_release(&hxc_l_levelLabel) != HXC_STATUS_OK)
            {
              abort();
            }
            hxc_enum_4b68fff2_destroy(&hxc_l_g_h6904c3060e30);
            hxc_enum_fcb4401b_destroy(&hxc_l_g_h4ef171f4fe67);
            if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
            {
              abort();
            }
            abort();
          }
          if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n178) != HXC_STATUS_OK)
          {
            abort();
          }
        }
        else
        {
          struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n304 = hxc_l_selectedCampaign_h28bf179e07a3;
          hxc_l_gc_roots[106] = (const void *)hxc_l_tmp_load_result_n304;
          if (hxc_l_tmp_load_result_n304 == NULL)
          {
            abort();
          }
          struct hxc_caxecraft_content_CampaignTransition *hxc_l_tmp_load_result_n305 = hxc_l_transition;
          hxc_l_gc_roots[107] = (const void *)hxc_l_tmp_load_result_n305;
          if (hxc_l_tmp_load_result_n305 == NULL)
          {
            abort();
          }
          hxc_string hxc_l_tmp_class_field_load_result_n306 = (*hxc_l_tmp_load_result_n305).hxc_destinationLevel;
          struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_instance_call_result_n307 = hxc_caxecraft_content_CampaignManifest_level(hxc_l_tmp_load_result_n304, hxc_l_tmp_class_field_load_result_n306);
          hxc_l_gc_roots[108] = (const void *)hxc_l_tmp_instance_call_result_n307;
          struct hxc_caxecraft_content_CampaignLevel *hxc_l_destination_ha5debc4bb7cb = hxc_l_tmp_instance_call_result_n307;
          hxc_l_gc_roots[109] = (const void *)hxc_l_destination_ha5debc4bb7cb;
          hxc_l_gc_roots[110] = (const void *)NULL;
          if (hxc_l_destination_ha5debc4bb7cb == NULL)
          {
            if (hxc_io_println((hxc_string){ (const uint8_t *)"caxecraft: campaign destination disappeared after manifest validation", 69, true, NULL }) != HXC_STATUS_OK)
            {
              abort();
            }
          }
          else
          {
            hxc_l_gc_roots[111] = (const void *)hxc_l_destination_ha5debc4bb7cb;
            hxc_l_requestedCampaignLevel = hxc_l_destination_ha5debc4bb7cb;
          }
        }
      }
    }
    bool hxc_l_tmp_call_result_n311 = hxc_caxecraft_app_AppScreen_showsCampaignSelection(hxc_l_screen);
    bool hxc_l_tmp_short_circuit_result_n180 = hxc_l_tmp_call_result_n311;
    if (hxc_l_tmp_call_result_n311)
    {
      hxc_l_tmp_short_circuit_result_n180 = hxc_l_focused;
    }
    if (hxc_l_tmp_short_circuit_result_n180)
    {
      bool hxc_l_campaignLaunchRequested = hxc_l_menuConfirmPressed_h30f1449eeac0;
      bool hxc_l_campaignBackRequested = hxc_l_pausePressed_h952cc9de954a;
      bool hxc_l_tmp_load_result_n316 = hxc_l_menuNextPressed_h4b7fca18ecc3;
      bool hxc_l_tmp_short_circuit_result_n183 = hxc_l_tmp_load_result_n316;
      if (hxc_l_tmp_load_result_n316)
      {
        hxc_l_gc_roots[112] = (const void *)hxc_l_campaign;
        hxc_l_gc_roots[113] = (const void *)NULL;
        hxc_l_tmp_short_circuit_result_n183 = hxc_l_campaign != NULL;
      }
      bool hxc_l_tmp_short_circuit_load_result_n318 = hxc_l_tmp_short_circuit_result_n183;
      bool hxc_l_tmp_short_circuit_result_n184 = hxc_l_tmp_short_circuit_load_result_n318;
      if (hxc_l_tmp_short_circuit_load_result_n318)
      {
        struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n319 = hxc_l_campaign;
        hxc_l_gc_roots[114] = (const void *)hxc_l_tmp_load_result_n319;
        if (hxc_l_tmp_load_result_n319 == NULL)
        {
          abort();
        }
        struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n320 = (*hxc_l_tmp_load_result_n319).hxc_admittedLevels;
        hxc_l_gc_roots[115] = (const void *)hxc_l_tmp_class_field_load_result_n320;
        int32_t hxc_l_tmp_array_length_result_n321;
        if (hxc_array_ref_length(hxc_l_tmp_class_field_load_result_n320, &hxc_l_tmp_array_length_result_n321) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_l_tmp_short_circuit_result_n184 = hxc_l_tmp_array_length_result_n321 > 0;
      }
      if (hxc_l_tmp_short_circuit_result_n184)
      {
        int32_t hxc_l_tmp_load_result_n323 = hxc_l_selectedCampaignLevelIndex;
        struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n324 = hxc_l_campaign;
        hxc_l_gc_roots[116] = (const void *)hxc_l_tmp_load_result_n324;
        if (hxc_l_tmp_load_result_n324 == NULL)
        {
          abort();
        }
        struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n325 = (*hxc_l_tmp_load_result_n324).hxc_admittedLevels;
        hxc_l_gc_roots[117] = (const void *)hxc_l_tmp_class_field_load_result_n325;
        int32_t hxc_l_tmp_array_length_result_n326;
        if (hxc_array_ref_length(hxc_l_tmp_class_field_load_result_n325, &hxc_l_tmp_array_length_result_n326) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_l_selectedCampaignLevelIndex = hxc_i32_modulo_zero_safe(hxc_i32_add_wrapping(hxc_l_tmp_load_result_n323, 1), hxc_l_tmp_array_length_result_n326);
      }
      bool hxc_l_tmp_native_call_result_n327 = IsKeyPressed((int32_t)76);
      if (hxc_l_tmp_native_call_result_n327)
      {
        struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_tmp_load_result_n328 = hxc_l_uiCatalog;
        hxc_l_gc_roots[118] = (const void *)hxc_l_tmp_load_result_n328;
        if (hxc_l_tmp_load_result_n328 == NULL)
        {
          abort();
        }
        int32_t hxc_l_tmp_instance_call_result_n330 = hxc_caxecraft_localization_RuntimeUiCatalog_nextLocale(hxc_l_tmp_load_result_n328, hxc_l_locale);
        hxc_l_locale = hxc_l_tmp_instance_call_result_n330;
      }
      struct Vector2 hxc_l_tmp_native_call_result_n331 = GetMousePosition();
      struct Vector2 hxc_l_campaignMouse = hxc_l_tmp_native_call_result_n331;
      double hxc_l_tmp_static_call_argument_0_n187 = (double)hxc_l_campaignMouse.x;
      double hxc_l_tmp_static_call_argument_1_n188 = (double)hxc_l_campaignMouse.y;
      int32_t hxc_l_tmp_native_call_result_n336 = GetScreenWidth();
      int32_t hxc_l_tmp_static_call_argument_2_n189 = (int32_t)hxc_l_tmp_native_call_result_n336;
      int32_t hxc_l_tmp_native_call_result_n337 = GetScreenHeight();
      int32_t hxc_l_tmp_static_call_argument_3_n190 = (int32_t)hxc_l_tmp_native_call_result_n337;
      struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n338 = hxc_l_campaign;
      hxc_l_gc_roots[119] = (const void *)hxc_l_tmp_load_result_n338;
      hxc_l_gc_roots[120] = (const void *)NULL;
      int32_t hxc_l_tmp_conditional_result_n191 = 0;
      if (hxc_l_tmp_load_result_n338 == NULL)
      {
        hxc_l_tmp_conditional_result_n191 = 0;
      }
      else
      {
        struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n339 = hxc_l_campaign;
        hxc_l_gc_roots[121] = (const void *)hxc_l_tmp_load_result_n339;
        if (hxc_l_tmp_load_result_n339 == NULL)
        {
          abort();
        }
        struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n340 = (*hxc_l_tmp_load_result_n339).hxc_admittedLevels;
        hxc_l_gc_roots[122] = (const void *)hxc_l_tmp_class_field_load_result_n340;
        int32_t hxc_l_tmp_array_length_result_n341;
        if (hxc_array_ref_length(hxc_l_tmp_class_field_load_result_n340, &hxc_l_tmp_array_length_result_n341) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_l_tmp_conditional_result_n191 = hxc_l_tmp_array_length_result_n341;
      }
      int32_t hxc_l_tmp_conditional_load_result_n342 = hxc_l_tmp_conditional_result_n191;
      double hxc_l_tmp_static_call_argument_0_load_result_n343 = hxc_l_tmp_static_call_argument_0_n187;
      double hxc_l_tmp_static_call_argument_1_load_result_n344 = hxc_l_tmp_static_call_argument_1_n188;
      int32_t hxc_l_tmp_static_call_argument_2_load_result_n345 = hxc_l_tmp_static_call_argument_2_n189;
      struct hxc_caxecraft_app_CampaignMenuHit hxc_l_tmp_call_result_n347 = hxc_caxecraft_app_CampaignMenu_selectionAt(hxc_l_tmp_static_call_argument_0_load_result_n343, hxc_l_tmp_static_call_argument_1_load_result_n344, hxc_l_tmp_static_call_argument_2_load_result_n345, hxc_l_tmp_static_call_argument_3_n190, hxc_l_tmp_conditional_load_result_n342);
      struct hxc_caxecraft_app_CampaignMenuHit hxc_l_campaignChoice = hxc_l_tmp_call_result_n347;
      bool hxc_l_tmp_native_call_result_n348 = IsMouseButtonPressed((int32_t)0);
      if (hxc_l_tmp_native_call_result_n348)
      {
        switch (hxc_l_campaignChoice.hxc_tag) {
          case hxc_caxecraft_app_CampaignMenuHit_NoCampaignMenuHit:
            {
              break;
            }
          case hxc_caxecraft_app_CampaignMenuHit_LevelHit:
            {
              if (hxc_l_campaignChoice.hxc_tag != hxc_caxecraft_app_CampaignMenuHit_LevelHit)
              {
                abort();
              }
              int32_t hxc_l_tmp_enum_payload_project_n351 = hxc_l_campaignChoice.hxc_payload.hxc_LevelHit.hxc_index;
              int32_t hxc_l_index_h0f8b3659eb87 = hxc_l_tmp_enum_payload_project_n351;
              hxc_l_selectedCampaignLevelIndex = hxc_l_index_h0f8b3659eb87;
              break;
            }
          case hxc_caxecraft_app_CampaignMenuHit_LaunchHit:
            {
              hxc_l_campaignLaunchRequested = true;
              break;
            }
          case hxc_caxecraft_app_CampaignMenuHit_BackHit:
            {
              hxc_l_campaignBackRequested = true;
              break;
            }
          default:
            {
              abort();
            }
        }
      }
      if (hxc_l_campaignBackRequested)
      {
        enum hxc_caxecraft_app_AppScreen hxc_l_tmp_call_result_n355 = hxc_caxecraft_app_AppScreen_closeCampaignSelection(hxc_l_screen);
        hxc_l_screen = hxc_l_tmp_call_result_n355;
        hxc_l_accumulator = 0.0;
        hxc_l_resetMotionThisFrame = true;
      }
      else
      {
        struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n356 = hxc_l_campaign;
        hxc_l_gc_roots[123] = (const void *)hxc_l_tmp_load_result_n356;
        hxc_l_gc_roots[124] = (const void *)NULL;
        bool hxc_l_tmp_short_circuit_result_n193 = hxc_l_tmp_load_result_n356 != NULL;
        if (hxc_l_tmp_load_result_n356 != NULL)
        {
          hxc_l_tmp_short_circuit_result_n193 = hxc_l_campaignLaunchRequested;
        }
        if (hxc_l_tmp_short_circuit_result_n193)
        {
          struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n359 = hxc_l_campaign;
          hxc_l_gc_roots[125] = (const void *)hxc_l_tmp_load_result_n359;
          if (hxc_l_tmp_load_result_n359 == NULL)
          {
            abort();
          }
          struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n360 = (*hxc_l_tmp_load_result_n359).hxc_admittedLevels;
          hxc_l_gc_roots[126] = (const void *)hxc_l_tmp_class_field_load_result_n360;
          struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_array_get_result_n362;
          if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n360, (size_t)hxc_l_selectedCampaignLevelIndex, &hxc_l_tmp_array_get_result_n362) != HXC_STATUS_OK)
          {
            abort();
          }
          hxc_l_gc_roots[127] = (const void *)hxc_l_tmp_array_get_result_n362;
          hxc_l_requestedCampaignLevel = hxc_l_tmp_array_get_result_n362;
          hxc_l_requestedFromCampaignMenu = true;
        }
      }
    }
    bool hxc_l_tmp_call_result_n364 = hxc_caxecraft_app_AppScreen_showsTitle(hxc_l_screen);
    bool hxc_l_tmp_short_circuit_result_n194 = hxc_l_tmp_call_result_n364;
    if (hxc_l_tmp_call_result_n364)
    {
      hxc_l_tmp_short_circuit_result_n194 = hxc_l_focused;
    }
    if (hxc_l_tmp_short_circuit_result_n194)
    {
      enum hxc_caxecraft_app_AppScreen hxc_l_screenBeforeMenu = hxc_l_screen;
      int32_t hxc_l_modeBeforeMenu = hxc_l_selectedMode;
      enum hxc_caxecraft_app_AppScreen hxc_l_tmp_load_result_n369 = hxc_l_screen;
      struct hxc_caxecraft_app_TitleMenuState hxc_l_tmp_call_result_n371 = hxc_caxecraft_app_TitleMenuFlow_titleMenuState(hxc_l_tmp_load_result_n369, hxc_l_selectedMode);
      struct hxc_caxecraft_app_TitleMenuState hxc_l_menuState = hxc_l_tmp_call_result_n371;
      bool hxc_l_tmp_native_call_result_n372 = IsKeyPressed((int32_t)76);
      if (hxc_l_tmp_native_call_result_n372)
      {
        struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_tmp_load_result_n373 = hxc_l_uiCatalog;
        hxc_l_gc_roots[131] = (const void *)hxc_l_tmp_load_result_n373;
        if (hxc_l_tmp_load_result_n373 == NULL)
        {
          abort();
        }
        int32_t hxc_l_tmp_instance_call_result_n375 = hxc_caxecraft_localization_RuntimeUiCatalog_nextLocale(hxc_l_tmp_load_result_n373, hxc_l_locale);
        hxc_l_locale = hxc_l_tmp_instance_call_result_n375;
      }
      if (hxc_l_menuNextPressed_h4b7fca18ecc3)
      {
        struct hxc_caxecraft_app_TitleMenuState hxc_l_tmp_call_result_n378 = hxc_caxecraft_app_TitleMenuFlow_applyTitleMenuCommand(hxc_l_menuState, 1);
        hxc_l_menuState = hxc_l_tmp_call_result_n378;
      }
      struct Vector2 hxc_l_tmp_native_call_result_n379 = GetMousePosition();
      struct Vector2 hxc_l_menuMouse = hxc_l_tmp_native_call_result_n379;
      struct Vector2 hxc_l_tmp_load_result_n380 = hxc_l_menuMouse;
      struct Vector2 hxc_l_tmp_load_result_n382 = hxc_l_menuMouse;
      int32_t hxc_l_tmp_native_call_result_n384 = GetScreenWidth();
      int32_t hxc_l_tmp_native_call_result_n385 = GetScreenHeight();
      int32_t hxc_l_tmp_call_result_n386 = hxc_caxecraft_app_TitleMenu_selectionAt((double)hxc_l_tmp_load_result_n380.x, (double)hxc_l_tmp_load_result_n382.y, (int32_t)hxc_l_tmp_native_call_result_n384, (int32_t)hxc_l_tmp_native_call_result_n385);
      int32_t hxc_l_hovered = hxc_l_tmp_call_result_n386;
      if (hxc_l_hovered == 0)
      {
        struct hxc_caxecraft_app_TitleMenuState hxc_l_tmp_call_result_n389 = hxc_caxecraft_app_TitleMenuFlow_applyTitleMenuCommand(hxc_l_menuState, 2);
        hxc_l_menuState = hxc_l_tmp_call_result_n389;
      }
      if (hxc_l_hovered == 1)
      {
        struct hxc_caxecraft_app_TitleMenuState hxc_l_tmp_call_result_n392 = hxc_caxecraft_app_TitleMenuFlow_applyTitleMenuCommand(hxc_l_menuState, 3);
        hxc_l_menuState = hxc_l_tmp_call_result_n392;
      }
      int32_t hxc_l_tmp_load_result_n393 = hxc_l_hovered;
      bool hxc_l_tmp_short_circuit_result_n201 = hxc_l_tmp_load_result_n393 >= 0;
      if (hxc_l_tmp_load_result_n393 >= 0)
      {
        bool hxc_l_tmp_native_call_result_n394 = IsMouseButtonPressed((int32_t)0);
        hxc_l_tmp_short_circuit_result_n201 = hxc_l_tmp_native_call_result_n394;
      }
      bool hxc_l_clickedChoice = hxc_l_tmp_short_circuit_result_n201;
      if (hxc_l_clickedChoice)
      {
        struct hxc_caxecraft_app_TitleMenuState hxc_l_tmp_static_call_argument_0_n202 = hxc_l_menuState;
        int32_t hxc_l_tmp_load_result_n398 = hxc_l_hovered;
        int32_t hxc_l_tmp_conditional_result_n203 = 0;
        if (hxc_l_tmp_load_result_n398 == 2)
        {
          hxc_l_tmp_conditional_result_n203 = 5;
        }
        else
        {
          hxc_l_tmp_conditional_result_n203 = 4;
        }
        int32_t hxc_l_tmp_conditional_load_result_n399 = hxc_l_tmp_conditional_result_n203;
        struct hxc_caxecraft_app_TitleMenuState hxc_l_tmp_call_result_n401 = hxc_caxecraft_app_TitleMenuFlow_applyTitleMenuCommand(hxc_l_tmp_static_call_argument_0_n202, hxc_l_tmp_conditional_load_result_n399);
        hxc_l_menuState = hxc_l_tmp_call_result_n401;
      }
      else
      {
        if (hxc_l_menuConfirmPressed_h30f1449eeac0)
        {
          struct hxc_caxecraft_app_TitleMenuState hxc_l_tmp_call_result_n404 = hxc_caxecraft_app_TitleMenuFlow_applyTitleMenuCommand(hxc_l_menuState, 4);
          hxc_l_menuState = hxc_l_tmp_call_result_n404;
        }
      }
      hxc_l_screen = hxc_l_menuState.hxc_screen;
      hxc_l_selectedMode = hxc_l_menuState.hxc_mode;
      bool hxc_l_tmp_call_result_n408 = hxc_caxecraft_app_AppScreen_showsCampaignSelection(hxc_l_screen);
      bool hxc_l_tmp_short_circuit_result_n204 = hxc_l_tmp_call_result_n408;
      if (hxc_l_tmp_call_result_n408)
      {
        hxc_l_gc_roots[132] = (const void *)hxc_l_campaign;
        hxc_l_gc_roots[133] = (const void *)NULL;
        hxc_l_tmp_short_circuit_result_n204 = hxc_l_campaign == NULL;
      }
      if (hxc_l_tmp_short_circuit_result_n204)
      {
        hxc_l_screen = hxc_l_screenBeforeMenu;
      }
      enum hxc_caxecraft_app_AppScreen hxc_l_tmp_load_result_n412 = hxc_l_screen;
      enum hxc_caxecraft_app_AppScreen hxc_l_tmp_load_result_n413 = hxc_l_screenBeforeMenu;
      bool hxc_l_tmp_short_circuit_result_n205 = hxc_l_tmp_load_result_n412 != hxc_l_tmp_load_result_n413;
      if (!(hxc_l_tmp_load_result_n412 != hxc_l_tmp_load_result_n413))
      {
        int32_t hxc_l_tmp_load_result_n414 = hxc_l_selectedMode;
        hxc_l_tmp_short_circuit_result_n205 = hxc_l_tmp_load_result_n414 != hxc_l_modeBeforeMenu;
      }
      if (hxc_l_tmp_short_circuit_result_n205)
      {
        hxc_l_accumulator = 0.0;
        hxc_l_resetMotionThisFrame = true;
      }
      enum hxc_caxecraft_app_AppScreen hxc_l_tmp_load_result_n417 = hxc_l_screenBeforeMenu;
      bool hxc_l_tmp_short_circuit_result_n206 = hxc_l_tmp_load_result_n417 == hxc_caxecraft_app_AppScreen_Title;
      if (hxc_l_tmp_load_result_n417 == hxc_caxecraft_app_AppScreen_Title)
      {
        bool hxc_l_tmp_call_result_n420 = hxc_caxecraft_app_AppScreen_isPlaying(hxc_l_screen);
        hxc_l_tmp_short_circuit_result_n206 = hxc_l_tmp_call_result_n420;
      }
      if (hxc_l_tmp_short_circuit_result_n206)
      {
        hxc_l_recapturedThisFrame = true;
        DisableCursor();
      }
      else
      {
        bool hxc_l_tmp_call_result_n423 = hxc_caxecraft_app_AppScreen_showsEditor(hxc_l_screen);
        if (hxc_l_tmp_call_result_n423)
        {
          EnableCursor();
        }
      }
    }
    hxc_l_gc_roots[128] = (const void *)hxc_l_requestedCampaignLevel;
    struct hxc_caxecraft_content_CampaignLevel *hxc_l_destination_hc14d4a882c25 = hxc_l_requestedCampaignLevel;
    hxc_l_gc_roots[129] = (const void *)hxc_l_destination_hc14d4a882c25;
    hxc_l_gc_roots[130] = (const void *)NULL;
    if (hxc_l_destination_hc14d4a882c25 != NULL)
    {
      int32_t hxc_l_tmp_record_field_load_result_n426 = hxc_l_character.hxc_id;
      int32_t hxc_l_tmp_record_field_load_result_n427 = hxc_l_character.hxc_vitals.hxc_health;
      struct hxc_caxecraft_content_LevelPlayerOptions hxc_l_playerOptions = (struct hxc_caxecraft_content_LevelPlayerOptions){ .hxc_aquaticProfile = hxc_l_character.hxc_aquaticProfile, .hxc_entityId = hxc_l_tmp_record_field_load_result_n426, .hxc_initialHealth = hxc_l_tmp_record_field_load_result_n427 };
      struct hxc_caxecraft_app_ActivePlayableLevel *hxc_l_tmp_load_result_n430 = hxc_l_activeLevel;
      hxc_l_gc_roots[134] = (const void *)hxc_l_tmp_load_result_n430;
      if (hxc_l_tmp_load_result_n430 == NULL)
      {
        abort();
      }
      struct hxc_caxecraft_content_ActiveContent *hxc_l_tmp_class_field_load_result_n431 = (*hxc_l_tmp_load_result_n430).hxc_content;
      hxc_l_gc_roots[135] = (const void *)hxc_l_tmp_class_field_load_result_n431;
      if (hxc_l_tmp_class_field_load_result_n431 == NULL)
      {
        abort();
      }
      struct hxc_caxecraft_content_LoadedContentGeneration *hxc_l_tmp_class_field_load_result_n432 = (*hxc_l_tmp_class_field_load_result_n431).hxc_active;
      hxc_l_gc_roots[136] = (const void *)hxc_l_tmp_class_field_load_result_n432;
      if (hxc_l_tmp_class_field_load_result_n432 == NULL)
      {
        abort();
      }
      int32_t hxc_l_tmp_class_field_load_result_n433 = (*hxc_l_tmp_class_field_load_result_n432).hxc_generationIdValue;
      int32_t hxc_l_nextGeneration = hxc_i32_add_wrapping(hxc_l_tmp_class_field_load_result_n433, 1);
      struct hxc_caxecraft_content_ContentPackageStore *hxc_l_tmp_load_result_n434 = hxc_l_contentStore;
      hxc_l_gc_roots[137] = (const void *)hxc_l_tmp_load_result_n434;
      struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_load_result_n435 = hxc_l_destination_hc14d4a882c25;
      hxc_l_gc_roots[138] = (const void *)hxc_l_tmp_load_result_n435;
      int32_t hxc_l_tmp_load_result_n436 = hxc_l_nextGeneration;
      struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_tmp_load_result_n437 = hxc_l_contentRegistry;
      hxc_l_gc_roots[139] = (const void *)hxc_l_tmp_load_result_n437;
      if (hxc_l_tmp_load_result_n437 == NULL)
      {
        abort();
      }
      struct hxc_compiler_interface_dispatch_caxecraft_scenario_ScenarioContentRegistry_value hxc_l_tmp_interface_value_n438 = (struct hxc_compiler_interface_dispatch_caxecraft_scenario_ScenarioContentRegistry_value){ .object = hxc_l_tmp_load_result_n437, .table = &hxc_itable_compiler_interface_dispatch_caxecraft_content_RuntimeContentRegistry_itable_layout_caxecraf_hb3f384641e1a61bf };
      struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_tmp_load_result_n439 = hxc_l_contentRegistry;
      hxc_l_gc_roots[140] = (const void *)hxc_l_tmp_load_result_n439;
      if (hxc_l_tmp_load_result_n439 == NULL)
      {
        abort();
      }
      struct hxc_compiler_interface_dispatch_caxecraft_content_LevelContentResolver_value hxc_l_tmp_interface_value_n440 = (struct hxc_compiler_interface_dispatch_caxecraft_content_LevelContentResolver_value){ .object = hxc_l_tmp_load_result_n439, .table = &hxc_itable_compiler_interface_dispatch_caxecraft_content_RuntimeContentRegistry_itable_layout_caxecraf_h82ca4f24ca7a7b5a };
      struct hxc_caxecraft_content_CampaignLevelLoadResult hxc_l_tmp_call_result_n442 = hxc_caxecraft_content_CampaignRuntime_loadCampaignLevel(hxc_l_tmp_load_result_n434, hxc_l_tmp_load_result_n435, hxc_l_tmp_load_result_n436, hxc_l_tmp_interface_value_n438, hxc_l_tmp_interface_value_n440, hxc_l_playerOptions);
      hxc_l_gc_roots[141] = hxc_l_tmp_call_result_n442.hxc_tag == hxc_caxecraft_content_CampaignLevelLoadResult_CampaignLevelReady ? (const void *)hxc_l_tmp_call_result_n442.hxc_payload.hxc_CampaignLevelReady.hxc_candidate : NULL;
      hxc_l_g_hbd7f92d003e5 = hxc_l_tmp_call_result_n442;
      hxc_l_gc_roots[142] = hxc_l_g_hbd7f92d003e5.hxc_tag == hxc_caxecraft_content_CampaignLevelLoadResult_CampaignLevelReady ? (const void *)hxc_l_g_hbd7f92d003e5.hxc_payload.hxc_CampaignLevelReady.hxc_candidate : NULL;
      switch (hxc_l_g_hbd7f92d003e5.hxc_tag) {
        case hxc_caxecraft_content_CampaignLevelLoadResult_CampaignLevelReady:
          {
            hxc_l_gc_roots[145] = hxc_l_g_hbd7f92d003e5.hxc_tag == hxc_caxecraft_content_CampaignLevelLoadResult_CampaignLevelReady ? (const void *)hxc_l_g_hbd7f92d003e5.hxc_payload.hxc_CampaignLevelReady.hxc_candidate : NULL;
            if (hxc_l_g_hbd7f92d003e5.hxc_tag != hxc_caxecraft_content_CampaignLevelLoadResult_CampaignLevelReady)
            {
              abort();
            }
            struct hxc_caxecraft_content_RuntimeLevelCandidate *hxc_l_tmp_enum_payload_project_n445 = hxc_l_g_hbd7f92d003e5.hxc_payload.hxc_CampaignLevelReady.hxc_candidate;
            hxc_l_gc_roots[146] = (const void *)hxc_l_tmp_enum_payload_project_n445;
            struct hxc_caxecraft_content_RuntimeLevelCandidate *hxc_l_candidate_ha950d4aaf402 = hxc_l_tmp_enum_payload_project_n445;
            struct hxc_caxecraft_app_ActivePlayableLevel *hxc_l_tmp_load_result_n446 = hxc_l_activeLevel;
            hxc_l_gc_roots[147] = (const void *)hxc_l_tmp_load_result_n446;
            if (hxc_l_tmp_load_result_n446 == NULL)
            {
              abort();
            }
            hxc_l_gc_roots[148] = (const void *)hxc_l_candidate_ha950d4aaf402;
            struct hxc_caxecraft_app_PlayableLevelPublicationResult hxc_l_tmp_instance_call_result_n448 = hxc_caxecraft_app_ActivePlayableLevel_publish(hxc_l_tmp_load_result_n446, hxc_l_candidate_ha950d4aaf402);
            struct hxc_caxecraft_app_PlayableLevelPublicationResult hxc_l_g_h01c092e1f7b6 = hxc_l_tmp_instance_call_result_n448;
            switch (hxc_l_g_h01c092e1f7b6.hxc_tag) {
              case hxc_caxecraft_app_PlayableLevelPublicationResult_PlayableLevelPublished:
                {
                  if (hxc_l_g_h01c092e1f7b6.hxc_tag != hxc_caxecraft_app_PlayableLevelPublicationResult_PlayableLevelPublished)
                  {
                    abort();
                  }
                  int32_t hxc_l_tmp_enum_payload_project_n451 = hxc_l_g_h01c092e1f7b6.hxc_payload.hxc_PlayableLevelPublished.hxc_active;
                  int32_t hxc_l_selected = hxc_l_tmp_enum_payload_project_n451;
                  hxc_l_gc_roots[150] = (const void *)hxc_l_destination_hc14d4a882c25;
                  hxc_l_campaignLevel = hxc_l_destination_hc14d4a882c25;
                  struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_load_result_n453 = hxc_l_destination_hc14d4a882c25;
                  hxc_l_gc_roots[151] = (const void *)hxc_l_tmp_load_result_n453;
                  if (hxc_l_tmp_load_result_n453 == NULL)
                  {
                    abort();
                  }
                  hxc_string hxc_l_tmp_class_field_load_result_n454 = (*hxc_l_tmp_load_result_n453).hxc_id;
                  hxc_string hxc_l_tmp_string_assignment_replacement_owner_n214 = hxc_l_tmp_class_field_load_result_n454;
                  if (hxc_string_retain(hxc_l_tmp_string_assignment_replacement_owner_n214) != HXC_STATUS_OK)
                  {
                    abort();
                  }
                  hxc_string hxc_l_tmp_string_assignment_replacement_owned_load_result_n455 = hxc_l_tmp_string_assignment_replacement_owner_n214;
                  if (hxc_string_release(&hxc_l_levelLabel) != HXC_STATUS_OK)
                  {
                    abort();
                  }
                  hxc_l_levelLabel = hxc_l_tmp_string_assignment_replacement_owned_load_result_n455;
                  hxc_l_gc_roots[152] = (const void *)hxc_l_campaign;
                  struct hxc_caxecraft_content_CampaignManifest *hxc_l_selectedCampaign_h6414e5cfe6b9 = hxc_l_campaign;
                  hxc_l_gc_roots[153] = (const void *)hxc_l_selectedCampaign_h6414e5cfe6b9;
                  hxc_l_gc_roots[154] = (const void *)NULL;
                  if (hxc_l_selectedCampaign_h6414e5cfe6b9 != NULL)
                  {
                    int32_t hxc_l_g_h9321c79ec6b3 = 0;
                    struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n458 = hxc_l_selectedCampaign_h6414e5cfe6b9;
                    hxc_l_gc_roots[155] = (const void *)hxc_l_tmp_load_result_n458;
                    if (hxc_l_tmp_load_result_n458 == NULL)
                    {
                      abort();
                    }
                    struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n459 = (*hxc_l_tmp_load_result_n458).hxc_admittedLevels;
                    hxc_l_gc_roots[156] = (const void *)hxc_l_tmp_class_field_load_result_n459;
                    int32_t hxc_l_tmp_array_length_result_n460;
                    if (hxc_array_ref_length(hxc_l_tmp_class_field_load_result_n459, &hxc_l_tmp_array_length_result_n460) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    int32_t hxc_l_g1_hfe047ded78fa = hxc_l_tmp_array_length_result_n460;
                    while (1)
                    {
                      int32_t hxc_l_tmp_load_result_n461 = hxc_l_g_h9321c79ec6b3;
                      if (!(hxc_l_tmp_load_result_n461 < hxc_l_g1_hfe047ded78fa))
                      {
                        break;
                      }
                      int32_t hxc_l_tmp_increment_load_result_n463 = hxc_l_g_h9321c79ec6b3;
                      hxc_l_g_h9321c79ec6b3 = hxc_i32_add_wrapping(hxc_l_tmp_increment_load_result_n463, 1);
                      int32_t hxc_l_index_hded83cefec22 = hxc_l_tmp_increment_load_result_n463;
                      struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n464 = hxc_l_selectedCampaign_h6414e5cfe6b9;
                      hxc_l_gc_roots[164] = (const void *)hxc_l_tmp_load_result_n464;
                      if (hxc_l_tmp_load_result_n464 == NULL)
                      {
                        abort();
                      }
                      struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n465 = (*hxc_l_tmp_load_result_n464).hxc_admittedLevels;
                      hxc_l_gc_roots[165] = (const void *)hxc_l_tmp_class_field_load_result_n465;
                      struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_array_get_result_n467;
                      if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n465, (size_t)hxc_l_index_hded83cefec22, &hxc_l_tmp_array_get_result_n467) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      hxc_l_gc_roots[166] = (const void *)hxc_l_tmp_array_get_result_n467;
                      if (hxc_l_tmp_array_get_result_n467 == NULL)
                      {
                        abort();
                      }
                      hxc_string hxc_l_tmp_class_field_load_result_n468 = (*hxc_l_tmp_array_get_result_n467).hxc_id;
                      struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_load_result_n469 = hxc_l_destination_hc14d4a882c25;
                      hxc_l_gc_roots[167] = (const void *)hxc_l_tmp_load_result_n469;
                      if (hxc_l_tmp_load_result_n469 == NULL)
                      {
                        abort();
                      }
                      hxc_string hxc_l_tmp_class_field_load_result_n470 = (*hxc_l_tmp_load_result_n469).hxc_id;
                      if (hxc_l_tmp_class_field_load_result_n468.data == NULL || hxc_l_tmp_class_field_load_result_n470.data == NULL ? hxc_l_tmp_class_field_load_result_n468.data == hxc_l_tmp_class_field_load_result_n470.data : hxc_l_tmp_class_field_load_result_n468.byte_length == hxc_l_tmp_class_field_load_result_n470.byte_length && (hxc_l_tmp_class_field_load_result_n468.byte_length == 0 || memcmp(hxc_l_tmp_class_field_load_result_n468.data, hxc_l_tmp_class_field_load_result_n470.data, hxc_l_tmp_class_field_load_result_n468.byte_length) == 0))
                      {
                        hxc_l_selectedCampaignLevelIndex = hxc_l_index_hded83cefec22;
                      }
                    }
                  }
                  struct hxc_caxecraft_app_ActivePlayableLevel *hxc_l_tmp_load_result_n473 = hxc_l_activeLevel;
                  hxc_l_gc_roots[157] = (const void *)hxc_l_tmp_load_result_n473;
                  if (hxc_l_tmp_load_result_n473 == NULL)
                  {
                    abort();
                  }
                  struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_class_field_load_result_n474 = (*hxc_l_tmp_load_result_n473).hxc_activeView;
                  hxc_l_gc_roots[158] = (const void *)hxc_l_tmp_class_field_load_result_n474;
                  hxc_l_levelView_ha9fa307295b9 = hxc_l_tmp_class_field_load_result_n474;
                  struct hxc_caxecraft_app_ActivePlayableLevel *hxc_l_tmp_load_result_n475 = hxc_l_activeLevel;
                  hxc_l_gc_roots[159] = (const void *)hxc_l_tmp_load_result_n475;
                  if (hxc_l_tmp_load_result_n475 == NULL)
                  {
                    abort();
                  }
                  struct hxc_caxecraft_content_ActiveContent *hxc_l_tmp_class_field_load_result_n476 = (*hxc_l_tmp_load_result_n475).hxc_content;
                  hxc_l_gc_roots[160] = (const void *)hxc_l_tmp_class_field_load_result_n476;
                  if (hxc_l_tmp_class_field_load_result_n476 == NULL)
                  {
                    abort();
                  }
                  struct hxc_caxecraft_content_LoadedContentGeneration *hxc_l_tmp_class_field_load_result_n477 = (*hxc_l_tmp_class_field_load_result_n476).hxc_active;
                  hxc_l_gc_roots[161] = (const void *)hxc_l_tmp_class_field_load_result_n477;
                  if (hxc_l_tmp_class_field_load_result_n477 == NULL)
                  {
                    abort();
                  }
                  struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_class_field_load_result_n478 = (*hxc_l_tmp_class_field_load_result_n477).hxc_ownedSession;
                  hxc_l_gc_roots[162] = (const void *)hxc_l_tmp_class_field_load_result_n478;
                  hxc_l_session = hxc_l_tmp_class_field_load_result_n478;
                  struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n479 = hxc_l_session;
                  hxc_l_gc_roots[163] = (const void *)hxc_l_tmp_load_result_n479;
                  if (hxc_l_tmp_load_result_n479 == NULL)
                  {
                    abort();
                  }
                  struct hxc_caxecraft_domain_GameView hxc_l_tmp_instance_call_result_n480 = hxc_caxecraft_domain_GameSession_view(hxc_l_tmp_load_result_n479);
                  hxc_l_initialView = hxc_l_tmp_instance_call_result_n480;
                  if (!hxc_l_initialView.hxc_valid)
                  {
                    hxc_l_quit = true;
                  }
                  else
                  {
                    hxc_l_character = hxc_l_initialView.hxc_localPlayer;
                    hxc_string hxc_l_tmp_string_assignment_replacement_owner_n219 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
                    if (hxc_string_retain(hxc_l_tmp_string_assignment_replacement_owner_n219) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    hxc_string hxc_l_tmp_string_assignment_replacement_owned_load_result_n483 = hxc_l_tmp_string_assignment_replacement_owner_n219;
                    if (hxc_string_release(&hxc_l_activeDialogue) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    hxc_l_activeDialogue = hxc_l_tmp_string_assignment_replacement_owned_load_result_n483;
                    hxc_string hxc_l_tmp_string_assignment_replacement_owner_n220 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
                    if (hxc_string_retain(hxc_l_tmp_string_assignment_replacement_owner_n220) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    hxc_string hxc_l_tmp_string_assignment_replacement_owned_load_result_n484 = hxc_l_tmp_string_assignment_replacement_owner_n220;
                    if (hxc_string_release(&hxc_l_latestJournalId) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    hxc_l_latestJournalId = hxc_l_tmp_string_assignment_replacement_owned_load_result_n484;
                    struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n485 = hxc_l_levelView_ha9fa307295b9;
                    hxc_l_gc_roots[168] = (const void *)hxc_l_tmp_load_result_n485;
                    if (hxc_l_tmp_load_result_n485 == NULL)
                    {
                      abort();
                    }
                    struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_class_field_load_result_n486 = (*hxc_l_tmp_load_result_n485).hxc_levelPresentation;
                    hxc_l_gc_roots[169] = (const void *)hxc_l_tmp_class_field_load_result_n486;
                    if (hxc_l_tmp_class_field_load_result_n486 == NULL)
                    {
                      abort();
                    }
                    hxc_string hxc_l_tmp_class_field_load_result_n487 = (*hxc_l_tmp_class_field_load_result_n486).hxc_startingObjective;
                    hxc_string hxc_l_tmp_string_assignment_replacement_owner_n221 = hxc_l_tmp_class_field_load_result_n487;
                    if (hxc_string_retain(hxc_l_tmp_string_assignment_replacement_owner_n221) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    hxc_string hxc_l_tmp_string_assignment_replacement_owned_load_result_n488 = hxc_l_tmp_string_assignment_replacement_owner_n221;
                    if (hxc_string_release(&hxc_l_currentObjectiveId) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    hxc_l_currentObjectiveId = hxc_l_tmp_string_assignment_replacement_owned_load_result_n488;
                    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n489 = hxc_l_session;
                    hxc_l_gc_roots[170] = (const void *)hxc_l_tmp_load_result_n489;
                    hxc_l_gc_roots[171] = (const void *)hxc_l_levelView_ha9fa307295b9;
                    struct hxc_caxecraft_app_CaxecraftApp_AvailableInteractionTarget hxc_l_tmp_call_result_n491 = hxc_caxecraft_app_CaxecraftApp_nearestAvailableInteraction(hxc_l_tmp_load_result_n489, hxc_l_levelView_ha9fa307295b9);
                    hxc_l_tmp_static_call_argument_0_owner_n222 = hxc_l_tmp_call_result_n491;
                    bool hxc_l_tmp_call_result_n493 = hxc_caxecraft_app_CaxecraftApp_hasAvailableInteraction(hxc_l_tmp_static_call_argument_0_owner_n222);
                    hxc_l_guideInteractionAvailable = hxc_l_tmp_call_result_n493;
                    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n494 = hxc_l_session;
                    hxc_l_gc_roots[172] = (const void *)hxc_l_tmp_load_result_n494;
                    if (hxc_l_tmp_load_result_n494 == NULL)
                    {
                      abort();
                    }
                    struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n495 = hxc_l_levelView_ha9fa307295b9;
                    hxc_l_gc_roots[173] = (const void *)hxc_l_tmp_load_result_n495;
                    if (hxc_l_tmp_load_result_n495 == NULL)
                    {
                      abort();
                    }
                    int32_t hxc_l_tmp_class_field_load_result_n496 = (*hxc_l_tmp_load_result_n495).hxc_enemyId;
                    struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n497 = hxc_caxecraft_domain_GameSession_readCharacter(hxc_l_tmp_load_result_n494, hxc_l_tmp_class_field_load_result_n496);
                    hxc_l_enemyActor = hxc_l_tmp_instance_call_result_n497;
                    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n498 = hxc_l_session;
                    hxc_l_gc_roots[174] = (const void *)hxc_l_tmp_load_result_n498;
                    if (hxc_l_tmp_load_result_n498 == NULL)
                    {
                      abort();
                    }
                    struct hxc_array_ref *hxc_l_tmp_instance_call_result_n499 = hxc_caxecraft_domain_GameSession_actorControllerStateSnapshots(hxc_l_tmp_load_result_n498);
                    hxc_l_phases = hxc_l_tmp_instance_call_result_n499;
                    struct hxc_array_ref *hxc_l_tmp_load_result_n500 = hxc_l_phases;
                    struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n501 = hxc_l_levelView_ha9fa307295b9;
                    hxc_l_gc_roots[175] = (const void *)hxc_l_tmp_load_result_n501;
                    if (hxc_l_tmp_load_result_n501 == NULL)
                    {
                      abort();
                    }
                    int32_t hxc_l_tmp_class_field_load_result_n502 = (*hxc_l_tmp_load_result_n501).hxc_enemyId;
                    struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation hxc_l_tmp_call_result_n504 = hxc_caxecraft_app_CaxecraftApp_observeActorPhase(hxc_l_tmp_load_result_n500, hxc_l_tmp_class_field_load_result_n502, hxc_caxecraft_domain_ActorControllerPhase_Resting);
                    hxc_l_enemyPhase_h70382f3bdc66 = hxc_l_tmp_call_result_n504;
                    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n505 = hxc_l_session;
                    hxc_l_gc_roots[176] = (const void *)hxc_l_tmp_load_result_n505;
                    struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n506 = hxc_l_levelView_ha9fa307295b9;
                    hxc_l_gc_roots[177] = (const void *)hxc_l_tmp_load_result_n506;
                    bool hxc_l_tmp_call_result_n508 = hxc_caxecraft_app_CaxecraftApp_dialogueActorsAreValid(hxc_l_tmp_load_result_n505, hxc_l_tmp_load_result_n506, hxc_l_phases);
                    bool hxc_l_tmp_short_circuit_result_n224 = !hxc_l_tmp_call_result_n508;
                    if (!!hxc_l_tmp_call_result_n508)
                    {
                      hxc_l_tmp_short_circuit_result_n224 = hxc_l_enemyActor.hxc_id <= 0;
                    }
                    bool hxc_l_tmp_short_circuit_load_result_n510 = hxc_l_tmp_short_circuit_result_n224;
                    bool hxc_l_tmp_short_circuit_result_n225 = hxc_l_tmp_short_circuit_load_result_n510;
                    if (!hxc_l_tmp_short_circuit_load_result_n510)
                    {
                      hxc_l_tmp_short_circuit_result_n225 = !hxc_l_enemyPhase_h70382f3bdc66.hxc_valid;
                    }
                    if (hxc_l_tmp_short_circuit_result_n225)
                    {
                      hxc_l_quit = true;
                    }
                    hxc_l_swordCombat = (struct hxc_caxecraft_gameplay_SwordCombatState){ .hxc_cooldownTicks = 0 };
                    hxc_l_berryDrop = (struct hxc_caxecraft_gameplay_BerryDropState){ .hxc_active = false, .hxc_amount = 0, .hxc_x = 0.0, .hxc_y = 0.0, .hxc_z = 0.0 };
                    hxc_l_cameraWaterBlend = 0.0;
                    hxc_l_accumulator = 0.0;
                    struct hxc_caxecraft_app_MotionHistory hxc_l_tmp_call_result_n516 = hxc_caxecraft_app_MotionInterpolation_start(hxc_l_character.hxc_body);
                    hxc_l_motionHistory = hxc_l_tmp_call_result_n516;
                    hxc_l_jumpQueued = false;
                    hxc_l_swordQueued = false;
                    hxc_l_placementBlockedFrames = 0;
                    hxc_l_strikeHitFrames = 0;
                    hxc_l_enemyDefeatedFrames = 0;
                    hxc_l_enemyAttackFrames = 0;
                    hxc_l_pickupFrames = 0;
                    hxc_l_inventoryFullFrames = 0;
                    hxc_l_inventoryFullReason = 0;
                    hxc_l_recoveryFeedbackFrames = 0;
                    hxc_l_aquaticEquipmentFrames = 0;
                    if (hxc_l_self == NULL)
                    {
                      abort();
                    }
                    struct hxc_caxecraft_app_TerrainRenderer *hxc_l_tmp_owned_class_field_address_n517 = &(*hxc_l_self).hxc_terrainRenderer;
                    struct hxc_caxecraft_app_TerrainChunkCache *hxc_l_tmp_owned_class_field_address_n518 = &(*hxc_l_tmp_owned_class_field_address_n517).hxc_cache;
                    if (hxc_l_tmp_owned_class_field_address_n518 == NULL)
                    {
                      abort();
                    }
                    hxc_caxecraft_app_TerrainChunkCache_invalidateAll(hxc_l_tmp_owned_class_field_address_n518);
                    hxc_l_resetMotionThisFrame = true;
                    if (hxc_l_requestedFromCampaignMenu)
                    {
                      enum hxc_caxecraft_app_AppScreen hxc_l_tmp_call_result_n521 = hxc_caxecraft_app_AppScreen_startSelectedCampaign(hxc_l_screen);
                      hxc_l_screen = hxc_l_tmp_call_result_n521;
                      hxc_l_recapturedThisFrame = true;
                      DisableCursor();
                    }
                    hxc_string hxc_l_tmp_string_concat_result_n523 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
                    if (hxc_string_concat_ref((hxc_string){ (const uint8_t *)"caxecraft: campaign-level=", 26, true, NULL }, hxc_l_levelLabel, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n523) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    hxc_l_tmp_string_concat_left_owner_n226 = hxc_l_tmp_string_concat_result_n523;
                    hxc_string hxc_l_tmp_string_concat_result_n525 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
                    if (hxc_string_concat_ref(hxc_l_tmp_string_concat_left_owner_n226, (hxc_string){ (const uint8_t *)" generation=", 12, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n525) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    hxc_l_tmp_string_concat_left_owner_n227 = hxc_l_tmp_string_concat_result_n525;
                    hxc_string hxc_l_tmp_string_concat_left_borrow_result_n526 = hxc_l_tmp_string_concat_left_owner_n227;
                    hxc_string hxc_l_tmp_std_string_int_result_n528 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
                    if (hxc_string_from_int32(hxc_l_selected, hxc_default_allocator(), &hxc_l_tmp_std_string_int_result_n528) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    hxc_l_tmp_string_concat_right_owner_n228 = hxc_l_tmp_std_string_int_result_n528;
                    hxc_string hxc_l_tmp_string_concat_result_n530 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
                    if (hxc_string_concat_ref(hxc_l_tmp_string_concat_left_borrow_result_n526, hxc_l_tmp_string_concat_right_owner_n228, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n530) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    hxc_l_tmp_sys_println_string_argument_owner_n229 = hxc_l_tmp_string_concat_result_n530;
                    if (hxc_io_println(hxc_l_tmp_sys_println_string_argument_owner_n229) != HXC_STATUS_OK)
                    {
                      if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n229) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n228) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n227) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n226) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      if (hxc_array_ref_release(hxc_l_phases) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      hxc_enum_2c0e695d_destroy(&hxc_l_tmp_static_call_argument_0_owner_n222);
                      hxc_enum_5543d34a_destroy(&hxc_l_g_hbd7f92d003e5);
                      if (hxc_array_ref_release(hxc_l_initialActorPhases) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      hxc_enum_2c0e695d_destroy(&hxc_l_tmp_static_call_argument_0_owner_n64);
                      if (hxc_string_release(&hxc_l_currentObjectiveId) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      if (hxc_string_release(&hxc_l_latestJournalId) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      if (hxc_string_release(&hxc_l_activeDialogue) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      hxc_record_f6127605_destroy(&hxc_l_receipt);
                      if (hxc_string_release(&hxc_l_levelLabel) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      hxc_enum_4b68fff2_destroy(&hxc_l_g_h6904c3060e30);
                      hxc_enum_fcb4401b_destroy(&hxc_l_g_h4ef171f4fe67);
                      if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      abort();
                    }
                    if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n229) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n228) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n227) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n226) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    if (hxc_array_ref_release(hxc_l_phases) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    hxc_enum_2c0e695d_destroy(&hxc_l_tmp_static_call_argument_0_owner_n222);
                  }
                  break;
                }
              case hxc_caxecraft_app_PlayableLevelPublicationResult_PlayableLevelPublicationRejected:
                {
                  if (hxc_io_println((hxc_string){ (const uint8_t *)"caxecraft: campaign level could not replace the active level", 60, true, NULL }) != HXC_STATUS_OK)
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
            break;
          }
        case hxc_caxecraft_content_CampaignLevelLoadResult_CampaignLevelRejected:
          {
            hxc_l_gc_roots[149] = hxc_l_g_hbd7f92d003e5.hxc_tag == hxc_caxecraft_content_CampaignLevelLoadResult_CampaignLevelReady ? (const void *)hxc_l_g_hbd7f92d003e5.hxc_payload.hxc_CampaignLevelReady.hxc_candidate : NULL;
            if (hxc_l_g_hbd7f92d003e5.hxc_tag != hxc_caxecraft_content_CampaignLevelLoadResult_CampaignLevelRejected)
            {
              abort();
            }
            struct hxc_caxecraft_content_CampaignLevelLoadError hxc_l_tmp_enum_payload_project_n533 = hxc_l_g_hbd7f92d003e5.hxc_payload.hxc_CampaignLevelRejected.hxc_error;
            hxc_l_error = hxc_l_tmp_enum_payload_project_n533;
            if (hxc_enum_40863044_retain(&hxc_l_error) != HXC_STATUS_OK)
            {
              abort();
            }
            hxc_string hxc_l_tmp_call_result_n535 = hxc_caxecraft_app_CaxecraftApp_campaignLevelLoadFailure(hxc_l_error);
            hxc_l_tmp_string_concat_right_owner_n231 = hxc_l_tmp_call_result_n535;
            hxc_string hxc_l_tmp_string_concat_result_n537 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
            if (hxc_string_concat_ref((hxc_string){ (const uint8_t *)"caxecraft: campaign level rejected: ", 36, true, NULL }, hxc_l_tmp_string_concat_right_owner_n231, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n537) != HXC_STATUS_OK)
            {
              abort();
            }
            hxc_l_tmp_sys_println_string_argument_owner_n232 = hxc_l_tmp_string_concat_result_n537;
            if (hxc_io_println(hxc_l_tmp_sys_println_string_argument_owner_n232) != HXC_STATUS_OK)
            {
              if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n232) != HXC_STATUS_OK)
              {
                abort();
              }
              if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n231) != HXC_STATUS_OK)
              {
                abort();
              }
              hxc_enum_40863044_destroy(&hxc_l_error);
              hxc_enum_5543d34a_destroy(&hxc_l_g_hbd7f92d003e5);
              if (hxc_array_ref_release(hxc_l_initialActorPhases) != HXC_STATUS_OK)
              {
                abort();
              }
              hxc_enum_2c0e695d_destroy(&hxc_l_tmp_static_call_argument_0_owner_n64);
              if (hxc_string_release(&hxc_l_currentObjectiveId) != HXC_STATUS_OK)
              {
                abort();
              }
              if (hxc_string_release(&hxc_l_latestJournalId) != HXC_STATUS_OK)
              {
                abort();
              }
              if (hxc_string_release(&hxc_l_activeDialogue) != HXC_STATUS_OK)
              {
                abort();
              }
              hxc_record_f6127605_destroy(&hxc_l_receipt);
              if (hxc_string_release(&hxc_l_levelLabel) != HXC_STATUS_OK)
              {
                abort();
              }
              hxc_enum_4b68fff2_destroy(&hxc_l_g_h6904c3060e30);
              hxc_enum_fcb4401b_destroy(&hxc_l_g_h4ef171f4fe67);
              if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
              {
                abort();
              }
              abort();
            }
            if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n232) != HXC_STATUS_OK)
            {
              abort();
            }
            if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n231) != HXC_STATUS_OK)
            {
              abort();
            }
            hxc_enum_40863044_destroy(&hxc_l_error);
            break;
          }
        default:
          {
            abort();
          }
      }
      hxc_enum_5543d34a_destroy(&hxc_l_g_hbd7f92d003e5);
    }
    struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n539 = hxc_l_levelView_ha9fa307295b9;
    hxc_l_gc_roots[143] = (const void *)hxc_l_tmp_load_result_n539;
    if (hxc_l_tmp_load_result_n539 == NULL)
    {
      abort();
    }
    int32_t hxc_l_tmp_class_field_load_result_n540 = (*hxc_l_tmp_load_result_n539).hxc_enemyId;
    int32_t hxc_l_enemyActorId = hxc_l_tmp_class_field_load_result_n540;
    struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n541 = hxc_l_levelView_ha9fa307295b9;
    hxc_l_gc_roots[144] = (const void *)hxc_l_tmp_load_result_n541;
    if (hxc_l_tmp_load_result_n541 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_scenario_ScenarioTransform hxc_l_tmp_class_field_load_result_n542 = (*hxc_l_tmp_load_result_n541).hxc_playerSpawn;
    struct hxc_caxecraft_scenario_ScenarioTransform hxc_l_spawnTransform = hxc_l_tmp_class_field_load_result_n542;
    if (hxc_l_quitPressed_hee357f1e842f)
    {
      hxc_l_quit = true;
    }
    if (hxc_l_hotbarSelection_h4d15508de09d >= 0)
    {
      struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_load_result_n545 = hxc_l_inventory;
      struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n547 = hxc_caxecraft_gameplay_Inventory_select(hxc_l_tmp_load_result_n545, hxc_l_hotbarSelection_h4d15508de09d);
      hxc_l_inventory = hxc_l_tmp_call_result_n547;
    }
    if (hxc_l_hotbarCycle_he98d5700e593 != 0)
    {
      struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_load_result_n549 = hxc_l_inventory;
      struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n551 = hxc_caxecraft_gameplay_Inventory_cycle(hxc_l_tmp_load_result_n549, hxc_l_hotbarCycle_he98d5700e593);
      hxc_l_inventory = hxc_l_tmp_call_result_n551;
    }
    bool hxc_l_tmp_call_result_n553 = hxc_caxecraft_app_AppScreen_isPlaying(hxc_l_screen);
    bool hxc_l_tmp_short_circuit_result_n235 = hxc_l_tmp_call_result_n553;
    if (hxc_l_tmp_call_result_n553)
    {
      hxc_l_tmp_short_circuit_result_n235 = hxc_l_interactPressed_h550f33a83547;
    }
    if (hxc_l_tmp_short_circuit_result_n235)
    {
      if (hxc_l_character.hxc_vitals.hxc_health <= 0)
      {
        struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n557 = hxc_l_session;
        hxc_l_gc_roots[178] = (const void *)hxc_l_tmp_load_result_n557;
        if (hxc_l_tmp_load_result_n557 == NULL)
        {
          abort();
        }
        struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n558 = hxc_l_session;
        hxc_l_gc_roots[179] = (const void *)hxc_l_tmp_load_result_n558;
        if (hxc_l_tmp_load_result_n558 == NULL)
        {
          abort();
        }
        size_t hxc_l_tmp_length_n560;
        const uint8_t *hxc_l_tmp_instance_call_result_n559 = hxc_caxecraft_domain_GameSession_worldView(hxc_l_tmp_load_result_n558, &hxc_l_tmp_length_n560);
        struct hxc_caxecraft_domain_CharacterBody hxc_l_tmp_call_result_n562 = hxc_caxecraft_app_CaxecraftApp_spawnPlayer(hxc_l_tmp_instance_call_result_n559, hxc_l_tmp_length_n560, hxc_l_spawnTransform);
        struct hxc_caxecraft_domain_LocalCharacterCommandResult hxc_l_tmp_instance_call_result_n563 = hxc_caxecraft_domain_GameSession_reviveLocalPlayerAt(hxc_l_tmp_load_result_n557, hxc_l_tmp_call_result_n562);
        struct hxc_caxecraft_domain_LocalCharacterCommandResult hxc_l_revival = hxc_l_tmp_instance_call_result_n563;
        hxc_l_character = hxc_l_revival.hxc_character;
        if (!hxc_l_revival.hxc_resolved)
        {
          hxc_l_quit = true;
        }
        else
        {
          hxc_l_cameraWaterBlend = 0.0;
          hxc_l_accumulator = 0.0;
          hxc_l_resetMotionThisFrame = true;
        }
      }
      else
      {
        if (hxc_l_activeDialogue.data != NULL)
        {
          hxc_string hxc_l_tmp_string_assignment_replacement_owner_n237 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
          if (hxc_string_retain(hxc_l_tmp_string_assignment_replacement_owner_n237) != HXC_STATUS_OK)
          {
            abort();
          }
          hxc_string hxc_l_tmp_string_assignment_replacement_owned_load_result_n567 = hxc_l_tmp_string_assignment_replacement_owner_n237;
          if (hxc_string_release(&hxc_l_activeDialogue) != HXC_STATUS_OK)
          {
            abort();
          }
          hxc_l_activeDialogue = hxc_l_tmp_string_assignment_replacement_owned_load_result_n567;
        }
        else
        {
          struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n568 = hxc_l_session;
          hxc_l_gc_roots[180] = (const void *)hxc_l_tmp_load_result_n568;
          hxc_l_gc_roots[181] = (const void *)hxc_l_levelView_ha9fa307295b9;
          struct hxc_caxecraft_app_CaxecraftApp_AvailableInteractionTarget hxc_l_tmp_call_result_n570 = hxc_caxecraft_app_CaxecraftApp_nearestAvailableInteraction(hxc_l_tmp_load_result_n568, hxc_l_levelView_ha9fa307295b9);
          hxc_l_g_hdd9f9dcfea76 = hxc_l_tmp_call_result_n570;
          switch (hxc_l_g_hdd9f9dcfea76.hxc_tag) {
            case hxc_caxecraft_app_CaxecraftApp_AvailableInteractionTarget_NoAvailableInteraction:
              {
                break;
              }
            case hxc_caxecraft_app_CaxecraftApp_AvailableInteractionTarget_DialogueInteraction:
              {
                if (hxc_l_g_hdd9f9dcfea76.hxc_tag != hxc_caxecraft_app_CaxecraftApp_AvailableInteractionTarget_DialogueInteraction)
                {
                  abort();
                }
                int32_t hxc_l_tmp_enum_payload_project_n573 = hxc_l_g_hdd9f9dcfea76.hxc_payload.hxc_DialogueInteraction.hxc_id;
                int32_t hxc_l_id_h6c2b29a016e9 = hxc_l_tmp_enum_payload_project_n573;
                struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n574 = hxc_l_session;
                hxc_l_gc_roots[182] = (const void *)hxc_l_tmp_load_result_n574;
                if (hxc_l_tmp_load_result_n574 == NULL)
                {
                  abort();
                }
                bool hxc_l_tmp_instance_call_result_n576 = hxc_caxecraft_domain_GameSession_interactWithActor(hxc_l_tmp_load_result_n574, hxc_l_id_h6c2b29a016e9);
                if (!hxc_l_tmp_instance_call_result_n576)
                {
                  hxc_l_quit = true;
                }
                break;
              }
            case hxc_caxecraft_app_CaxecraftApp_AvailableInteractionTarget_StatefulObjectInteraction:
              {
                if (hxc_l_g_hdd9f9dcfea76.hxc_tag != hxc_caxecraft_app_CaxecraftApp_AvailableInteractionTarget_StatefulObjectInteraction)
                {
                  abort();
                }
                hxc_string hxc_l_tmp_enum_payload_project_n578 = hxc_l_g_hdd9f9dcfea76.hxc_payload.hxc_StatefulObjectInteraction.hxc_id;
                hxc_l_id_h994c8bd3392d = hxc_l_tmp_enum_payload_project_n578;
                if (hxc_string_retain(hxc_l_id_h994c8bd3392d) != HXC_STATUS_OK)
                {
                  abort();
                }
                struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n579 = hxc_l_session;
                hxc_l_gc_roots[183] = (const void *)hxc_l_tmp_load_result_n579;
                if (hxc_l_tmp_load_result_n579 == NULL)
                {
                  abort();
                }
                bool hxc_l_tmp_instance_call_result_n581 = hxc_caxecraft_domain_GameSession_interactWithStatefulObject(hxc_l_tmp_load_result_n579, hxc_l_id_h994c8bd3392d);
                if (!hxc_l_tmp_instance_call_result_n581)
                {
                  hxc_l_quit = true;
                }
                if (hxc_string_release(&hxc_l_id_h994c8bd3392d) != HXC_STATUS_OK)
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
          hxc_enum_2c0e695d_destroy(&hxc_l_g_hdd9f9dcfea76);
        }
      }
    }
    bool hxc_l_tmp_load_result_n582 = hxc_l_focused;
    bool hxc_l_tmp_short_circuit_result_n241 = !hxc_l_tmp_load_result_n582;
    if (!hxc_l_tmp_load_result_n582)
    {
      bool hxc_l_tmp_call_result_n584 = hxc_caxecraft_app_AppScreen_isPlaying(hxc_l_screen);
      hxc_l_tmp_short_circuit_result_n241 = hxc_l_tmp_call_result_n584;
    }
    if (hxc_l_tmp_short_circuit_result_n241)
    {
      enum hxc_caxecraft_app_AppScreen hxc_l_tmp_call_result_n587 = hxc_caxecraft_app_AppScreen_loseFocus(hxc_l_screen);
      hxc_l_screen = hxc_l_tmp_call_result_n587;
      hxc_l_jumpQueued = false;
      hxc_l_accumulator = 0.0;
      hxc_l_resetMotionThisFrame = true;
      EnableCursor();
    }
    bool hxc_l_tmp_call_result_n589 = hxc_caxecraft_app_AppScreen_showsTitle(hxc_l_screen);
    bool hxc_l_tmp_short_circuit_result_n242 = !hxc_l_tmp_call_result_n589;
    if (!hxc_l_tmp_call_result_n589)
    {
      bool hxc_l_tmp_call_result_n591 = hxc_caxecraft_app_AppScreen_showsCampaignSelection(hxc_l_screen);
      hxc_l_tmp_short_circuit_result_n242 = !hxc_l_tmp_call_result_n591;
    }
    bool hxc_l_tmp_short_circuit_load_result_n592 = hxc_l_tmp_short_circuit_result_n242;
    bool hxc_l_tmp_short_circuit_result_n243 = hxc_l_tmp_short_circuit_load_result_n592;
    if (hxc_l_tmp_short_circuit_load_result_n592)
    {
      bool hxc_l_tmp_call_result_n594 = hxc_caxecraft_app_AppScreen_showsEditor(hxc_l_screen);
      hxc_l_tmp_short_circuit_result_n243 = !hxc_l_tmp_call_result_n594;
    }
    bool hxc_l_tmp_short_circuit_load_result_n595 = hxc_l_tmp_short_circuit_result_n243;
    bool hxc_l_tmp_short_circuit_result_n244 = hxc_l_tmp_short_circuit_load_result_n595;
    if (hxc_l_tmp_short_circuit_load_result_n595)
    {
      hxc_l_tmp_short_circuit_result_n244 = hxc_l_focused;
    }
    bool hxc_l_tmp_short_circuit_load_result_n597 = hxc_l_tmp_short_circuit_result_n244;
    bool hxc_l_tmp_short_circuit_result_n245 = hxc_l_tmp_short_circuit_load_result_n597;
    if (hxc_l_tmp_short_circuit_load_result_n597)
    {
      hxc_l_tmp_short_circuit_result_n245 = hxc_l_pausePressed_h952cc9de954a;
    }
    if (hxc_l_tmp_short_circuit_result_n245)
    {
      enum hxc_caxecraft_app_AppScreen hxc_l_tmp_call_result_n601 = hxc_caxecraft_app_AppScreen_togglePause(hxc_l_screen);
      hxc_l_screen = hxc_l_tmp_call_result_n601;
      hxc_l_accumulator = 0.0;
      hxc_l_resetMotionThisFrame = true;
      bool hxc_l_tmp_call_result_n603 = hxc_caxecraft_app_AppScreen_pausesSimulation(hxc_l_screen);
      if (hxc_l_tmp_call_result_n603)
      {
        hxc_l_jumpQueued = false;
      }
      bool hxc_l_tmp_call_result_n605 = hxc_caxecraft_app_AppScreen_capturesPointer(hxc_l_screen);
      if (hxc_l_tmp_call_result_n605)
      {
        DisableCursor();
      }
      else
      {
        EnableCursor();
      }
    }
    bool hxc_l_tmp_call_result_n607 = hxc_caxecraft_app_AppScreen_showsTitle(hxc_l_screen);
    bool hxc_l_tmp_short_circuit_result_n246 = !hxc_l_tmp_call_result_n607;
    if (!hxc_l_tmp_call_result_n607)
    {
      bool hxc_l_tmp_call_result_n609 = hxc_caxecraft_app_AppScreen_showsCampaignSelection(hxc_l_screen);
      hxc_l_tmp_short_circuit_result_n246 = !hxc_l_tmp_call_result_n609;
    }
    bool hxc_l_tmp_short_circuit_load_result_n610 = hxc_l_tmp_short_circuit_result_n246;
    bool hxc_l_tmp_short_circuit_result_n247 = hxc_l_tmp_short_circuit_load_result_n610;
    if (hxc_l_tmp_short_circuit_load_result_n610)
    {
      bool hxc_l_tmp_call_result_n612 = hxc_caxecraft_app_AppScreen_showsEditor(hxc_l_screen);
      hxc_l_tmp_short_circuit_result_n247 = !hxc_l_tmp_call_result_n612;
    }
    bool hxc_l_tmp_short_circuit_load_result_n613 = hxc_l_tmp_short_circuit_result_n247;
    bool hxc_l_tmp_short_circuit_result_n248 = hxc_l_tmp_short_circuit_load_result_n613;
    if (hxc_l_tmp_short_circuit_load_result_n613)
    {
      hxc_l_tmp_short_circuit_result_n248 = hxc_l_focused;
    }
    bool hxc_l_tmp_short_circuit_load_result_n615 = hxc_l_tmp_short_circuit_result_n248;
    bool hxc_l_tmp_short_circuit_result_n249 = hxc_l_tmp_short_circuit_load_result_n615;
    if (hxc_l_tmp_short_circuit_load_result_n615)
    {
      bool hxc_l_tmp_call_result_n617 = hxc_caxecraft_app_AppScreen_pausesSimulation(hxc_l_screen);
      hxc_l_tmp_short_circuit_result_n249 = hxc_l_tmp_call_result_n617;
    }
    bool hxc_l_tmp_short_circuit_load_result_n618 = hxc_l_tmp_short_circuit_result_n249;
    bool hxc_l_tmp_short_circuit_result_n250 = hxc_l_tmp_short_circuit_load_result_n618;
    if (hxc_l_tmp_short_circuit_load_result_n618)
    {
      hxc_l_tmp_short_circuit_result_n250 = hxc_l_capturePressed;
    }
    if (hxc_l_tmp_short_circuit_result_n250)
    {
      enum hxc_caxecraft_app_AppScreen hxc_l_tmp_call_result_n622 = hxc_caxecraft_app_AppScreen_recapture(hxc_l_screen);
      hxc_l_screen = hxc_l_tmp_call_result_n622;
      hxc_l_recapturedThisFrame = true;
      hxc_l_accumulator = 0.0;
      hxc_l_resetMotionThisFrame = true;
      DisableCursor();
    }
    bool hxc_l_tmp_call_result_n624 = hxc_caxecraft_app_AppScreen_showsTitle(hxc_l_screen);
    bool hxc_l_onTitle = hxc_l_tmp_call_result_n624;
    bool hxc_l_tmp_call_result_n626 = hxc_caxecraft_app_AppScreen_showsCampaignSelection(hxc_l_screen);
    bool hxc_l_onCampaignSelect = hxc_l_tmp_call_result_n626;
    bool hxc_l_tmp_call_result_n628 = hxc_caxecraft_app_AppScreen_showsEditor(hxc_l_screen);
    bool hxc_l_onEditor = hxc_l_tmp_call_result_n628;
    bool hxc_l_tmp_call_result_n630 = hxc_caxecraft_app_AppScreen_pausesSimulation(hxc_l_screen);
    bool hxc_l_paused_h8182f296b655 = hxc_l_tmp_call_result_n630;
    bool hxc_l_tmp_call_result_n632 = hxc_caxecraft_app_AppScreen_capturesPointer(hxc_l_screen);
    bool hxc_l_captured_h599154fa3a25 = hxc_l_tmp_call_result_n632;
    int32_t hxc_l_editorNavigationCommand = 0;
    if (hxc_l_onEditor)
    {
      if (hxc_l_self == NULL)
      {
        abort();
      }
      struct hxc_caxecraft_input_NavigationRepeater *hxc_l_tmp_owned_class_field_address_n634 = &(*hxc_l_self).hxc_editorNavigation;
      if (hxc_l_tmp_owned_class_field_address_n634 == NULL)
      {
        abort();
      }
      struct hxc_caxecraft_input_NavigationSample hxc_l_tmp_call_result_n635 = hxc_caxecraft_app_RaylibNavigationInput_samplePrimaryGamepad();
      int32_t hxc_l_tmp_instance_call_result_n637 = hxc_caxecraft_input_NavigationRepeater_advance(hxc_l_tmp_owned_class_field_address_n634, hxc_l_tmp_call_result_n635, hxc_l_frameSeconds);
      hxc_l_editorNavigationCommand = hxc_l_tmp_instance_call_result_n637;
    }
    else
    {
      if (hxc_l_self == NULL)
      {
        abort();
      }
      struct hxc_caxecraft_input_NavigationRepeater *hxc_l_tmp_owned_class_field_address_n638 = &(*hxc_l_self).hxc_editorNavigation;
      if (hxc_l_tmp_owned_class_field_address_n638 == NULL)
      {
        abort();
      }
      hxc_caxecraft_input_NavigationRepeater_release(hxc_l_tmp_owned_class_field_address_n638);
    }
    if (hxc_l_captured_h599154fa3a25)
    {
      double hxc_l_yawDelta = hxc_l_lookYaw_hb85067cf1c5a;
      if (hxc_l_yawDelta > 0.25)
      {
        hxc_l_yawDelta = 0.25;
      }
      if (hxc_l_yawDelta < -0.25)
      {
        hxc_l_yawDelta = -0.25;
      }
      double hxc_l_tmp_load_result_n643 = hxc_l_lookX;
      double hxc_l_tmp_load_result_n644 = hxc_l_yawDelta;
      double hxc_l_candidateX = hxc_l_tmp_load_result_n643 + hxc_l_tmp_load_result_n644 * hxc_l_lookZ;
      double hxc_l_tmp_load_result_n646 = hxc_l_lookZ;
      double hxc_l_tmp_load_result_n647 = hxc_l_yawDelta;
      double hxc_l_candidateZ = hxc_l_tmp_load_result_n646 - hxc_l_tmp_load_result_n647 * hxc_l_lookX;
      double hxc_l_tmp_load_result_n649 = hxc_l_candidateX;
      double hxc_l_tmp_load_result_n650 = hxc_l_candidateX;
      double hxc_l_tmp_load_result_n651 = hxc_l_candidateZ;
      double hxc_l_lengthSquared = hxc_l_tmp_load_result_n649 * hxc_l_tmp_load_result_n650 + hxc_l_tmp_load_result_n651 * hxc_l_candidateZ;
      double hxc_l_normalize = 1.5 - 0.5 * hxc_l_lengthSquared;
      double hxc_l_tmp_load_result_n654 = hxc_l_candidateX;
      hxc_l_lookX = hxc_l_tmp_load_result_n654 * hxc_l_normalize;
      double hxc_l_tmp_load_result_n656 = hxc_l_candidateZ;
      hxc_l_lookZ = hxc_l_tmp_load_result_n656 * hxc_l_normalize;
      double hxc_l_tmp_compound_load_result_n658 = hxc_l_lookY;
      hxc_l_lookY = hxc_l_tmp_compound_load_result_n658 + hxc_l_lookPitch_h7ef5aa1a8cda;
      if (hxc_l_lookY > 0.90)
      {
        hxc_l_lookY = 0.90;
      }
      if (hxc_l_lookY < -0.90)
      {
        hxc_l_lookY = -0.90;
      }
      if (hxc_l_jumpPressed_hc40fd78ea0d6)
      {
        hxc_l_jumpQueued = true;
      }
    }
    bool hxc_l_tmp_load_result_n663 = hxc_l_captured_h599154fa3a25;
    bool hxc_l_tmp_short_circuit_result_n262 = hxc_l_tmp_load_result_n663;
    if (hxc_l_tmp_load_result_n663)
    {
      hxc_l_tmp_short_circuit_result_n262 = !hxc_l_recapturedThisFrame;
    }
    bool hxc_l_tmp_short_circuit_load_result_n665 = hxc_l_tmp_short_circuit_result_n262;
    bool hxc_l_tmp_short_circuit_result_n263 = hxc_l_tmp_short_circuit_load_result_n665;
    if (hxc_l_tmp_short_circuit_load_result_n665)
    {
      hxc_l_tmp_short_circuit_result_n263 = hxc_l_primaryPressed_h4a0a8b20cf08;
    }
    bool hxc_l_tmp_short_circuit_load_result_n667 = hxc_l_tmp_short_circuit_result_n263;
    bool hxc_l_tmp_short_circuit_result_n264 = hxc_l_tmp_short_circuit_load_result_n667;
    if (hxc_l_tmp_short_circuit_load_result_n667)
    {
      hxc_l_tmp_short_circuit_result_n264 = hxc_l_selectedMode == 1;
    }
    bool hxc_l_tmp_short_circuit_load_result_n669 = hxc_l_tmp_short_circuit_result_n264;
    bool hxc_l_tmp_short_circuit_result_n265 = hxc_l_tmp_short_circuit_load_result_n669;
    if (hxc_l_tmp_short_circuit_load_result_n669)
    {
      bool hxc_l_tmp_call_result_n672 = hxc_caxecraft_gameplay_Inventory_selectedIs(hxc_l_inventory, hxc_caxecraft_gameplay_ItemKind_CopperSword);
      hxc_l_tmp_short_circuit_result_n265 = hxc_l_tmp_call_result_n672;
    }
    if (hxc_l_tmp_short_circuit_result_n265)
    {
      hxc_l_swordQueued = true;
    }
    if (!hxc_l_paused_h8182f296b655)
    {
      double hxc_l_tmp_compound_load_result_n675 = hxc_l_accumulator;
      hxc_l_accumulator = hxc_l_tmp_compound_load_result_n675 + hxc_l_frameSeconds;
    }
    while (1)
    {
      bool hxc_l_tmp_load_result_n677 = hxc_l_paused_h8182f296b655;
      bool hxc_l_tmp_short_circuit_result_n266 = !hxc_l_tmp_load_result_n677;
      if (!hxc_l_tmp_load_result_n677)
      {
        hxc_l_tmp_short_circuit_result_n266 = hxc_l_accumulator >= 0.05;
      }
      if (!hxc_l_tmp_short_circuit_result_n266)
      {
        break;
      }
      double hxc_l_tmp_load_result_n680 = hxc_l_moveForward;
      double hxc_l_tmp_load_result_n681 = hxc_l_lookX;
      double hxc_l_tmp_load_result_n682 = hxc_l_moveRight;
      double hxc_l_moveX = hxc_l_tmp_load_result_n680 * hxc_l_tmp_load_result_n681 - hxc_l_tmp_load_result_n682 * hxc_l_lookZ;
      double hxc_l_tmp_load_result_n684 = hxc_l_moveForward;
      double hxc_l_tmp_load_result_n685 = hxc_l_lookZ;
      double hxc_l_tmp_load_result_n686 = hxc_l_moveRight;
      double hxc_l_moveZ = hxc_l_tmp_load_result_n684 * hxc_l_tmp_load_result_n685 + hxc_l_tmp_load_result_n686 * hxc_l_lookX;
      double hxc_l_tmp_load_result_n688 = hxc_l_moveForward;
      bool hxc_l_tmp_short_circuit_result_n269 = hxc_l_tmp_load_result_n688 != 0.0;
      if (hxc_l_tmp_load_result_n688 != 0.0)
      {
        hxc_l_tmp_short_circuit_result_n269 = hxc_l_moveRight != 0.0;
      }
      if (hxc_l_tmp_short_circuit_result_n269)
      {
        hxc_l_moveX = hxc_l_moveX * 0.7071067811865476;
        hxc_l_moveZ = hxc_l_moveZ * 0.7071067811865476;
      }
      int32_t hxc_l_tmp_load_result_n693 = hxc_l_selectedMode;
      int32_t hxc_l_tmp_conditional_result_n271 = 0;
      if (hxc_l_tmp_load_result_n693 == 1)
      {
        hxc_l_tmp_conditional_result_n271 = 1;
      }
      else
      {
        hxc_l_tmp_conditional_result_n271 = 0;
      }
      int32_t hxc_l_damagePolicy = hxc_l_tmp_conditional_result_n271;
      bool hxc_l_tmp_load_result_n695 = hxc_l_jumpQueued;
      bool hxc_l_tmp_short_circuit_result_n273 = hxc_l_tmp_load_result_n695;
      if (!hxc_l_tmp_load_result_n695)
      {
        int32_t hxc_l_tmp_record_field_load_result_n696 = hxc_l_character.hxc_aquatic.hxc_medium;
        bool hxc_l_tmp_short_circuit_result_n274 = hxc_l_tmp_record_field_load_result_n696 != 0;
        if (hxc_l_tmp_record_field_load_result_n696 != 0)
        {
          hxc_l_tmp_short_circuit_result_n274 = hxc_l_riseHeld;
        }
        hxc_l_tmp_short_circuit_result_n273 = hxc_l_tmp_short_circuit_result_n274;
      }
      bool hxc_l_rising = hxc_l_tmp_short_circuit_result_n273;
      struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n700 = hxc_l_session;
      hxc_l_gc_roots[185] = (const void *)hxc_l_tmp_load_result_n700;
      if (hxc_l_tmp_load_result_n700 == NULL)
      {
        abort();
      }
      double hxc_l_tmp_load_result_n701 = hxc_l_moveX;
      double hxc_l_tmp_load_result_n702 = hxc_l_moveZ;
      bool hxc_l_tmp_load_result_n703 = hxc_l_rising;
      struct hxc_caxecraft_domain_CharacterIntent hxc_l_tmp_call_result_n705 = hxc_caxecraft_domain_Aquatics_input(hxc_l_tmp_load_result_n701, hxc_l_tmp_load_result_n702, hxc_l_tmp_load_result_n703, hxc_l_descendHeld_h962d79adb546);
      struct hxc_caxecraft_domain_GameTickResult hxc_l_tmp_instance_call_result_n708 = hxc_caxecraft_domain_GameSession_tick(hxc_l_tmp_load_result_n700, (struct hxc_caxecraft_domain_GameTickInput){ .hxc_damagePolicy = hxc_l_damagePolicy, .hxc_intent = hxc_l_tmp_call_result_n705, .hxc_waterUpdateBudget = 64 });
      hxc_l_gameTick = hxc_l_tmp_instance_call_result_n708;
      hxc_l_character = hxc_l_gameTick.hxc_character;
      hxc_l_flow = hxc_l_gameTick.hxc_flow;
      if (hxc_optional_b8d3265c_retain(&hxc_l_flow) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_l_flow.hxc_has_value)
      {
        struct hxc_optional_caxecraft_scenario_FlowTickResult hxc_l_tmp_load_result_n712 = hxc_l_flow;
        if (!hxc_l_tmp_load_result_n712.hxc_has_value)
        {
          abort();
        }
        int32_t hxc_l_tmp_array_length_result_n715;
        if (hxc_array_ref_length(hxc_l_tmp_load_result_n712.hxc_value.hxc_diagnostics, &hxc_l_tmp_array_length_result_n715) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_l_tmp_array_length_result_n715 == 0)
        {
          int32_t hxc_l_g_ha92280dd19c1 = 0;
          struct hxc_optional_caxecraft_scenario_FlowTickResult hxc_l_tmp_load_result_n716 = hxc_l_flow;
          if (!hxc_l_tmp_load_result_n716.hxc_has_value)
          {
            abort();
          }
          hxc_l_g1_h7bf1eee5d9d8 = hxc_l_tmp_load_result_n716.hxc_value.hxc_presentation;
          if (hxc_array_ref_retain(hxc_l_g1_h7bf1eee5d9d8) != HXC_STATUS_OK)
          {
            abort();
          }
          while (1)
          {
            int32_t hxc_l_tmp_load_result_n719 = hxc_l_g_ha92280dd19c1;
            int32_t hxc_l_tmp_array_length_result_n721;
            if (hxc_array_ref_length(hxc_l_g1_h7bf1eee5d9d8, &hxc_l_tmp_array_length_result_n721) != HXC_STATUS_OK)
            {
              abort();
            }
            if (!(hxc_l_tmp_load_result_n719 < hxc_l_tmp_array_length_result_n721))
            {
              break;
            }
            struct hxc_array_ref *hxc_l_tmp_load_result_n722 = hxc_l_g1_h7bf1eee5d9d8;
            struct hxc_caxecraft_scenario_FlowPresentationEvent hxc_l_tmp_array_get_result_n724;
            if (hxc_array_ref_get_copy(hxc_l_tmp_load_result_n722, (size_t)hxc_l_g_ha92280dd19c1, &hxc_l_tmp_array_get_result_n724) != HXC_STATUS_OK)
            {
              abort();
            }
            hxc_l_tmp_array_element_owner_n280 = hxc_l_tmp_array_get_result_n724;
            hxc_l_event_h3072cd2e0f94 = hxc_l_tmp_array_element_owner_n280;
            if (hxc_enum_147eda7c_retain(&hxc_l_event_h3072cd2e0f94) != HXC_STATUS_OK)
            {
              abort();
            }
            hxc_l_g_ha92280dd19c1 = hxc_i32_add_wrapping(hxc_l_g_ha92280dd19c1, 1);
            switch (hxc_l_event_h3072cd2e0f94.hxc_tag) {
              case hxc_caxecraft_scenario_FlowPresentationEvent_DialogueRequested:
                {
                  if (hxc_l_event_h3072cd2e0f94.hxc_tag != hxc_caxecraft_scenario_FlowPresentationEvent_DialogueRequested)
                  {
                    abort();
                  }
                  hxc_string hxc_l_tmp_enum_payload_project_n729 = hxc_l_event_h3072cd2e0f94.hxc_payload.hxc_DialogueRequested.hxc_id;
                  hxc_l_id_hd4b4b6738bb4 = hxc_l_tmp_enum_payload_project_n729;
                  if (hxc_string_retain(hxc_l_id_hd4b4b6738bb4) != HXC_STATUS_OK)
                  {
                    abort();
                  }
                  hxc_string hxc_l_tmp_string_assignment_replacement_owner_n282 = hxc_l_id_hd4b4b6738bb4;
                  if (hxc_string_retain(hxc_l_tmp_string_assignment_replacement_owner_n282) != HXC_STATUS_OK)
                  {
                    abort();
                  }
                  hxc_string hxc_l_tmp_string_assignment_replacement_owned_load_result_n731 = hxc_l_tmp_string_assignment_replacement_owner_n282;
                  if (hxc_string_release(&hxc_l_activeDialogue) != HXC_STATUS_OK)
                  {
                    abort();
                  }
                  hxc_l_activeDialogue = hxc_l_tmp_string_assignment_replacement_owned_load_result_n731;
                  if (hxc_string_release(&hxc_l_id_hd4b4b6738bb4) != HXC_STATUS_OK)
                  {
                    abort();
                  }
                  break;
                }
              case hxc_caxecraft_scenario_FlowPresentationEvent_JournalAdded:
                {
                  if (hxc_l_event_h3072cd2e0f94.hxc_tag != hxc_caxecraft_scenario_FlowPresentationEvent_JournalAdded)
                  {
                    abort();
                  }
                  hxc_string hxc_l_tmp_enum_payload_project_n733 = hxc_l_event_h3072cd2e0f94.hxc_payload.hxc_JournalAdded.hxc_id;
                  hxc_l_id_h268fb64469d6 = hxc_l_tmp_enum_payload_project_n733;
                  if (hxc_string_retain(hxc_l_id_h268fb64469d6) != HXC_STATUS_OK)
                  {
                    abort();
                  }
                  hxc_string hxc_l_tmp_string_assignment_replacement_owner_n284 = hxc_l_id_h268fb64469d6;
                  if (hxc_string_retain(hxc_l_tmp_string_assignment_replacement_owner_n284) != HXC_STATUS_OK)
                  {
                    abort();
                  }
                  hxc_string hxc_l_tmp_string_assignment_replacement_owned_load_result_n735 = hxc_l_tmp_string_assignment_replacement_owner_n284;
                  if (hxc_string_release(&hxc_l_latestJournalId) != HXC_STATUS_OK)
                  {
                    abort();
                  }
                  hxc_l_latestJournalId = hxc_l_tmp_string_assignment_replacement_owned_load_result_n735;
                  if (hxc_string_release(&hxc_l_id_h268fb64469d6) != HXC_STATUS_OK)
                  {
                    abort();
                  }
                  break;
                }
              default:
                {
                  break;
                }
            }
            hxc_enum_147eda7c_destroy(&hxc_l_event_h3072cd2e0f94);
            hxc_array_ce94fc36_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n280);
          }
          struct hxc_optional_caxecraft_scenario_FlowTickResult hxc_l_tmp_load_result_n736 = hxc_l_flow;
          if (!hxc_l_tmp_load_result_n736.hxc_has_value)
          {
            abort();
          }
          hxc_string hxc_l_tmp_string_assignment_replacement_owner_n285 = hxc_l_tmp_load_result_n736.hxc_value.hxc_activeObjective;
          if (hxc_string_retain(hxc_l_tmp_string_assignment_replacement_owner_n285) != HXC_STATUS_OK)
          {
            abort();
          }
          hxc_string hxc_l_tmp_string_assignment_replacement_owned_load_result_n739 = hxc_l_tmp_string_assignment_replacement_owner_n285;
          if (hxc_string_release(&hxc_l_currentObjectiveId) != HXC_STATUS_OK)
          {
            abort();
          }
          hxc_l_currentObjectiveId = hxc_l_tmp_string_assignment_replacement_owned_load_result_n739;
          if (hxc_array_ref_release(hxc_l_g1_h7bf1eee5d9d8) != HXC_STATUS_OK)
          {
            abort();
          }
        }
      }
      if (hxc_l_gameTick.hxc_committed)
      {
        struct hxc_caxecraft_app_MotionHistory hxc_l_tmp_load_result_n741 = hxc_l_motionHistory;
        struct hxc_caxecraft_app_MotionHistory hxc_l_tmp_call_result_n743 = hxc_caxecraft_app_MotionInterpolation_advance(hxc_l_tmp_load_result_n741, hxc_l_character.hxc_body);
        hxc_l_motionHistory = hxc_l_tmp_call_result_n743;
      }
      hxc_l_cameraWaterBlend = hxc_l_gameTick.hxc_immersion.hxc_cameraBlend;
      if (!hxc_l_gameTick.hxc_committed)
      {
        hxc_l_quit = true;
      }
      if (hxc_l_character.hxc_vitals.hxc_health > 0)
      {
        int32_t hxc_l_pickupIndex = 0;
        while (1)
        {
          int32_t hxc_l_tmp_load_result_n747 = hxc_l_pickupIndex;
          struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n748 = hxc_l_levelView_ha9fa307295b9;
          hxc_l_gc_roots[186] = (const void *)hxc_l_tmp_load_result_n748;
          if (hxc_l_tmp_load_result_n748 == NULL)
          {
            abort();
          }
          struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n749 = (*hxc_l_tmp_load_result_n748).hxc_items;
          int32_t hxc_l_tmp_array_length_result_n750;
          if (hxc_array_ref_length(hxc_l_tmp_class_field_load_result_n749, &hxc_l_tmp_array_length_result_n750) != HXC_STATUS_OK)
          {
            abort();
          }
          if (!(hxc_l_tmp_load_result_n747 < hxc_l_tmp_array_length_result_n750))
          {
            break;
          }
          struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n751 = hxc_l_levelView_ha9fa307295b9;
          hxc_l_gc_roots[187] = (const void *)hxc_l_tmp_load_result_n751;
          if (hxc_l_tmp_load_result_n751 == NULL)
          {
            abort();
          }
          struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n752 = (*hxc_l_tmp_load_result_n751).hxc_items;
          struct hxc_caxecraft_app_LoadedWorldItem hxc_l_tmp_array_get_result_n754;
          if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n752, (size_t)hxc_l_pickupIndex, &hxc_l_tmp_array_get_result_n754) != HXC_STATUS_OK)
          {
            abort();
          }
          struct hxc_caxecraft_app_LoadedWorldItem hxc_l_loadedItem = hxc_l_tmp_array_get_result_n754;
          struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n755 = hxc_l_session;
          hxc_l_gc_roots[188] = (const void *)hxc_l_tmp_load_result_n755;
          if (hxc_l_tmp_load_result_n755 == NULL)
          {
            abort();
          }
          bool hxc_l_tmp_instance_call_result_n757 = hxc_caxecraft_domain_GameSession_authoredItemIsActive(hxc_l_tmp_load_result_n755, hxc_l_pickupIndex);
          bool hxc_l_tmp_short_circuit_result_n288 = hxc_l_tmp_instance_call_result_n757;
          if (hxc_l_tmp_instance_call_result_n757)
          {
            double hxc_l_tmp_record_field_load_result_n758 = hxc_l_character.hxc_body.hxc_x;
            double hxc_l_tmp_record_field_load_result_n759 = hxc_l_character.hxc_body.hxc_y;
            double hxc_l_tmp_record_field_load_result_n760 = hxc_l_character.hxc_body.hxc_z;
            int32_t hxc_l_tmp_record_field_load_result_n761 = hxc_l_loadedItem.hxc_xMilli;
            int32_t hxc_l_tmp_record_field_load_result_n762 = hxc_l_loadedItem.hxc_yMilli;
            bool hxc_l_tmp_call_result_n764 = hxc_caxecraft_gameplay_WorldItemPickup_isInRange(hxc_l_tmp_record_field_load_result_n758, hxc_l_tmp_record_field_load_result_n759, hxc_l_tmp_record_field_load_result_n760, hxc_l_tmp_record_field_load_result_n761, hxc_l_tmp_record_field_load_result_n762, hxc_l_loadedItem.hxc_zMilli);
            hxc_l_tmp_short_circuit_result_n288 = hxc_l_tmp_call_result_n764;
          }
          if (hxc_l_tmp_short_circuit_result_n288)
          {
            int32_t hxc_l_itemCode = hxc_l_loadedItem.hxc_storageCode;
            struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_tmp_load_result_n767 = hxc_l_contentRegistry;
            hxc_l_gc_roots[189] = (const void *)hxc_l_tmp_load_result_n767;
            if (hxc_l_tmp_load_result_n767 == NULL)
            {
              abort();
            }
            enum hxc_caxecraft_content_RuntimeItemUseProfile hxc_l_tmp_instance_call_result_n769 = hxc_caxecraft_content_RuntimeContentRegistry_itemUseProfile(hxc_l_tmp_load_result_n767, hxc_l_itemCode);
            bool hxc_l_tmp_short_circuit_result_n290 = hxc_l_tmp_instance_call_result_n769 == hxc_caxecraft_content_RuntimeItemUseProfile_EquipAquatic;
            if (hxc_l_tmp_instance_call_result_n769 == hxc_caxecraft_content_RuntimeItemUseProfile_EquipAquatic)
            {
              struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_tmp_load_result_n771 = hxc_l_contentRegistry;
              hxc_l_gc_roots[190] = (const void *)hxc_l_tmp_load_result_n771;
              if (hxc_l_tmp_load_result_n771 == NULL)
              {
                abort();
              }
              bool hxc_l_tmp_instance_call_result_n773 = hxc_caxecraft_content_RuntimeContentRegistry_itemProvidesAquaticProfile(hxc_l_tmp_load_result_n771, hxc_l_itemCode);
              hxc_l_tmp_short_circuit_result_n290 = hxc_l_tmp_instance_call_result_n773;
            }
            if (hxc_l_tmp_short_circuit_result_n290)
            {
              struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_tmp_load_result_n775 = hxc_l_contentRegistry;
              hxc_l_gc_roots[191] = (const void *)hxc_l_tmp_load_result_n775;
              if (hxc_l_tmp_load_result_n775 == NULL)
              {
                abort();
              }
              struct hxc_caxecraft_domain_AquaticProfile hxc_l_tmp_instance_call_result_n777 = hxc_caxecraft_content_RuntimeContentRegistry_itemAquaticProfile(hxc_l_tmp_load_result_n775, hxc_l_itemCode);
              struct hxc_caxecraft_domain_AquaticProfile hxc_l_replacement = hxc_l_tmp_instance_call_result_n777;
              struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n778 = hxc_l_session;
              hxc_l_gc_roots[192] = (const void *)hxc_l_tmp_load_result_n778;
              if (hxc_l_tmp_load_result_n778 == NULL)
              {
                abort();
              }
              int32_t hxc_l_tmp_load_result_n779 = hxc_l_pickupIndex;
              struct hxc_caxecraft_domain_AuthoredAquaticEquipmentResult hxc_l_tmp_instance_call_result_n781 = hxc_caxecraft_domain_GameSession_collectAuthoredAquaticEquipment(hxc_l_tmp_load_result_n778, hxc_l_tmp_load_result_n779, hxc_l_replacement);
              struct hxc_caxecraft_domain_AuthoredAquaticEquipmentResult hxc_l_equipment = hxc_l_tmp_instance_call_result_n781;
              hxc_l_character = hxc_l_equipment.hxc_character;
              if (!hxc_l_equipment.hxc_resolved)
              {
                hxc_l_quit = true;
              }
              else
              {
                if (hxc_l_equipment.hxc_collected)
                {
                  hxc_l_aquaticEquipmentCode = hxc_l_itemCode;
                  hxc_l_aquaticEquipmentFrames = 120;
                }
              }
            }
            else
            {
              struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_tmp_load_result_n786 = hxc_l_contentRegistry;
              hxc_l_gc_roots[193] = (const void *)hxc_l_tmp_load_result_n786;
              struct hxc_optional_caxecraft_gameplay_ItemKind hxc_l_tmp_call_result_n788 = hxc_caxecraft_app_RuntimeInventoryBinding_inventoryKindForRuntimeItem(hxc_l_tmp_load_result_n786, hxc_l_itemCode);
              struct hxc_optional_caxecraft_gameplay_ItemKind hxc_l_inventoryKind = hxc_l_tmp_call_result_n788;
              if (hxc_l_inventoryKind.hxc_has_value)
              {
                struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n790 = hxc_l_session;
                hxc_l_gc_roots[194] = (const void *)hxc_l_tmp_load_result_n790;
                if (hxc_l_tmp_load_result_n790 == NULL)
                {
                  abort();
                }
                int32_t hxc_l_tmp_load_result_n791 = hxc_l_pickupIndex;
                struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_load_result_n792 = hxc_l_inventory;
                struct hxc_optional_caxecraft_gameplay_ItemKind hxc_l_tmp_load_result_n793 = hxc_l_inventoryKind;
                if (!hxc_l_tmp_load_result_n793.hxc_has_value)
                {
                  abort();
                }
                struct hxc_caxecraft_domain_AuthoredInventoryItemResult hxc_l_tmp_instance_call_result_n796 = hxc_caxecraft_domain_GameSession_collectAuthoredInventoryItem(hxc_l_tmp_load_result_n790, hxc_l_tmp_load_result_n791, hxc_l_tmp_load_result_n792, hxc_l_tmp_load_result_n793.hxc_value, hxc_l_loadedItem.hxc_quantity);
                struct hxc_caxecraft_domain_AuthoredInventoryItemResult hxc_l_pickup = hxc_l_tmp_instance_call_result_n796;
                hxc_l_inventory = hxc_l_pickup.hxc_inventory;
                if (!hxc_l_pickup.hxc_resolved)
                {
                  hxc_l_quit = true;
                }
              }
            }
          }
          hxc_l_pickupIndex = hxc_i32_add_wrapping(hxc_l_pickupIndex, 1);
        }
      }
      if (hxc_l_selectedMode == 1)
      {
        if (hxc_l_character.hxc_vitals.hxc_health > 0)
        {
          struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n802 = hxc_l_session;
          hxc_l_gc_roots[195] = (const void *)hxc_l_tmp_load_result_n802;
          if (hxc_l_tmp_load_result_n802 == NULL)
          {
            abort();
          }
          int32_t hxc_l_tmp_record_field_load_result_n803 = hxc_l_gameTick.hxc_tickIndex;
          struct hxc_caxecraft_domain_ActorControllerTickResult hxc_l_tmp_instance_call_result_n805 = hxc_caxecraft_domain_GameSession_stepAuthoredActorControllers(hxc_l_tmp_load_result_n802, hxc_l_tmp_record_field_load_result_n803, hxc_l_damagePolicy);
          struct hxc_caxecraft_domain_ActorControllerTickResult hxc_l_actorTick = hxc_l_tmp_instance_call_result_n805;
          switch (hxc_l_actorTick.hxc_status.hxc_tag) {
            case hxc_caxecraft_domain_ActorControllerTickStatus_ControllersAdvanced:
              {
                hxc_l_guideInteractionAvailable = false;
                int32_t hxc_l_g_h6e7333cdfcce = 0;
                struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n807 = hxc_l_session;
                hxc_l_gc_roots[200] = (const void *)hxc_l_tmp_load_result_n807;
                if (hxc_l_tmp_load_result_n807 == NULL)
                {
                  abort();
                }
                struct hxc_array_ref *hxc_l_tmp_instance_call_result_n808 = hxc_caxecraft_domain_GameSession_actorControllerEventSnapshots(hxc_l_tmp_load_result_n807);
                hxc_l_g1_hc8e9ed09509e = hxc_l_tmp_instance_call_result_n808;
                while (1)
                {
                  int32_t hxc_l_tmp_load_result_n809 = hxc_l_g_h6e7333cdfcce;
                  int32_t hxc_l_tmp_array_length_result_n811;
                  if (hxc_array_ref_length(hxc_l_g1_hc8e9ed09509e, &hxc_l_tmp_array_length_result_n811) != HXC_STATUS_OK)
                  {
                    abort();
                  }
                  if (!(hxc_l_tmp_load_result_n809 < hxc_l_tmp_array_length_result_n811))
                  {
                    break;
                  }
                  struct hxc_array_ref *hxc_l_tmp_load_result_n812 = hxc_l_g1_hc8e9ed09509e;
                  struct hxc_caxecraft_domain_ActorControllerEvent hxc_l_tmp_array_get_result_n814;
                  if (hxc_array_ref_get_copy(hxc_l_tmp_load_result_n812, (size_t)hxc_l_g_h6e7333cdfcce, &hxc_l_tmp_array_get_result_n814) != HXC_STATUS_OK)
                  {
                    abort();
                  }
                  hxc_l_tmp_array_element_owner_n299 = hxc_l_tmp_array_get_result_n814;
                  hxc_l_event_h5d214aacdd3f = hxc_l_tmp_array_element_owner_n299;
                  if (hxc_enum_8b2a26b8_retain(&hxc_l_event_h5d214aacdd3f) != HXC_STATUS_OK)
                  {
                    abort();
                  }
                  hxc_l_g_h6e7333cdfcce = hxc_i32_add_wrapping(hxc_l_g_h6e7333cdfcce, 1);
                  switch (hxc_l_event_h5d214aacdd3f.hxc_tag) {
                    case hxc_caxecraft_domain_ActorControllerEvent_NoControllerEvent:
                      {
                        break;
                      }
                    case hxc_caxecraft_domain_ActorControllerEvent_InteractionAvailable:
                      {
                        if (hxc_l_event_h5d214aacdd3f.hxc_tag != hxc_caxecraft_domain_ActorControllerEvent_InteractionAvailable)
                        {
                          abort();
                        }
                        int32_t hxc_l_tmp_enum_payload_project_n819 = hxc_l_event_h5d214aacdd3f.hxc_payload.hxc_InteractionAvailable.hxc_characterId;
                        int32_t hxc_l_source_h215a2e9c231e = hxc_l_tmp_enum_payload_project_n819;
                        struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n820 = hxc_l_levelView_ha9fa307295b9;
                        hxc_l_gc_roots[201] = (const void *)hxc_l_tmp_load_result_n820;
                        if (hxc_l_tmp_load_result_n820 == NULL)
                        {
                          abort();
                        }
                        bool hxc_l_tmp_instance_call_result_n822 = hxc_caxecraft_app_PlayableLevelView_hasDialogueActor(hxc_l_tmp_load_result_n820, hxc_l_source_h215a2e9c231e);
                        if (hxc_l_tmp_instance_call_result_n822)
                        {
                          hxc_l_guideInteractionAvailable = true;
                        }
                        break;
                      }
                    case hxc_caxecraft_domain_ActorControllerEvent_LocalPlayerAttack:
                      {
                        if (hxc_l_event_h5d214aacdd3f.hxc_tag != hxc_caxecraft_domain_ActorControllerEvent_LocalPlayerAttack)
                        {
                          abort();
                        }
                        int32_t hxc_l_tmp_enum_payload_project_n824 = hxc_l_event_h5d214aacdd3f.hxc_payload.hxc_LocalPlayerAttack.hxc_characterId;
                        int32_t hxc_l_source_h575e8399fe43 = hxc_l_tmp_enum_payload_project_n824;
                        int32_t hxc_l_tmp_load_result_n825 = hxc_l_source_h575e8399fe43;
                        if (hxc_l_tmp_load_result_n825 == hxc_l_enemyActorId)
                        {
                          hxc_l_enemyAttackFrames = 120;
                        }
                        break;
                      }
                    case hxc_caxecraft_domain_ActorControllerEvent_DropRequested:
                      {
                        if (hxc_l_event_h5d214aacdd3f.hxc_tag != hxc_caxecraft_domain_ActorControllerEvent_DropRequested)
                        {
                          abort();
                        }
                        int32_t hxc_l_tmp_enum_payload_project_n828 = hxc_l_event_h5d214aacdd3f.hxc_payload.hxc_DropRequested.hxc_characterId;
                        int32_t hxc_l_source_h10ca7b552359 = hxc_l_tmp_enum_payload_project_n828;
                        if (hxc_l_event_h5d214aacdd3f.hxc_tag != hxc_caxecraft_domain_ActorControllerEvent_DropRequested)
                        {
                          abort();
                        }
                        hxc_string hxc_l_tmp_enum_payload_project_n830 = hxc_l_event_h5d214aacdd3f.hxc_payload.hxc_DropRequested.hxc_drop;
                        hxc_l_drop = hxc_l_tmp_enum_payload_project_n830;
                        if (hxc_string_retain(hxc_l_drop) != HXC_STATUS_OK)
                        {
                          abort();
                        }
                        struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_tmp_load_result_n831 = hxc_l_contentRegistry;
                        hxc_l_gc_roots[202] = (const void *)hxc_l_tmp_load_result_n831;
                        if (hxc_l_tmp_load_result_n831 == NULL)
                        {
                          abort();
                        }
                        int32_t hxc_l_tmp_instance_call_result_n833 = hxc_caxecraft_content_RuntimeContentRegistry_dropQuantityById(hxc_l_tmp_load_result_n831, hxc_l_drop);
                        int32_t hxc_l_quantity = hxc_l_tmp_instance_call_result_n833;
                        struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n834 = hxc_l_session;
                        hxc_l_gc_roots[203] = (const void *)hxc_l_tmp_load_result_n834;
                        if (hxc_l_tmp_load_result_n834 == NULL)
                        {
                          abort();
                        }
                        struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n836 = hxc_caxecraft_domain_GameSession_readCharacter(hxc_l_tmp_load_result_n834, hxc_l_source_h10ca7b552359);
                        struct hxc_caxecraft_domain_Character hxc_l_defeatedActor = hxc_l_tmp_instance_call_result_n836;
                        int32_t hxc_l_tmp_load_result_n837 = hxc_l_quantity;
                        bool hxc_l_tmp_short_circuit_result_n306 = hxc_l_tmp_load_result_n837 <= 0;
                        if (!(hxc_l_tmp_load_result_n837 <= 0))
                        {
                          hxc_l_tmp_short_circuit_result_n306 = hxc_l_defeatedActor.hxc_id <= 0;
                        }
                        if (hxc_l_tmp_short_circuit_result_n306)
                        {
                          hxc_l_quit = true;
                        }
                        else
                        {
                          if (!hxc_l_berryDrop.hxc_active)
                          {
                            struct hxc_caxecraft_domain_Character hxc_l_tmp_load_result_n841 = hxc_l_defeatedActor;
                            struct hxc_caxecraft_gameplay_BerryDropState hxc_l_tmp_call_result_n843 = hxc_caxecraft_gameplay_BerryDrop_fromDefeatedCharacter(hxc_l_tmp_load_result_n841, hxc_l_quantity);
                            hxc_l_berryDrop = hxc_l_tmp_call_result_n843;
                            hxc_l_enemyDefeatedFrames = 120;
                          }
                        }
                        if (hxc_string_release(&hxc_l_drop) != HXC_STATUS_OK)
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
                  hxc_enum_8b2a26b8_destroy(&hxc_l_event_h5d214aacdd3f);
                  hxc_array_43e9f8ca_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n299);
                }
                if (hxc_array_ref_release(hxc_l_g1_hc8e9ed09509e) != HXC_STATUS_OK)
                {
                  abort();
                }
                break;
              }
            case hxc_caxecraft_domain_ActorControllerTickStatus_ControllerModelRejected:
              {
                hxc_l_quit = true;
                break;
              }
            case hxc_caxecraft_domain_ActorControllerTickStatus_ControlledCharacterMissing:
              {
                hxc_l_quit = true;
                break;
              }
            case hxc_caxecraft_domain_ActorControllerTickStatus_CharacterCommandRejected:
              {
                hxc_l_quit = true;
                break;
              }
            case hxc_caxecraft_domain_ActorControllerTickStatus_LocalAttackCommandRejected:
              {
                hxc_l_quit = true;
                break;
              }
            default:
              {
                abort();
              }
          }
        }
        struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n844 = hxc_l_session;
        hxc_l_gc_roots[196] = (const void *)hxc_l_tmp_load_result_n844;
        if (hxc_l_tmp_load_result_n844 == NULL)
        {
          abort();
        }
        struct hxc_array_ref *hxc_l_tmp_instance_call_result_n845 = hxc_caxecraft_domain_GameSession_actorControllerStateSnapshots(hxc_l_tmp_load_result_n844);
        hxc_l_actorPhases = hxc_l_tmp_instance_call_result_n845;
        struct hxc_array_ref *hxc_l_tmp_load_result_n846 = hxc_l_actorPhases;
        struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation hxc_l_tmp_call_result_n849 = hxc_caxecraft_app_CaxecraftApp_observeActorPhase(hxc_l_tmp_load_result_n846, hxc_l_enemyActorId, hxc_caxecraft_domain_ActorControllerPhase_Defeated);
        hxc_l_enemyPhase_h70382f3bdc66 = hxc_l_tmp_call_result_n849;
        struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n850 = hxc_l_session;
        hxc_l_gc_roots[197] = (const void *)hxc_l_tmp_load_result_n850;
        if (hxc_l_tmp_load_result_n850 == NULL)
        {
          abort();
        }
        struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n852 = hxc_caxecraft_domain_GameSession_readCharacter(hxc_l_tmp_load_result_n850, hxc_l_enemyActorId);
        hxc_l_enemyActor = hxc_l_tmp_instance_call_result_n852;
        struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n853 = hxc_l_session;
        hxc_l_gc_roots[198] = (const void *)hxc_l_tmp_load_result_n853;
        struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n854 = hxc_l_levelView_ha9fa307295b9;
        hxc_l_gc_roots[199] = (const void *)hxc_l_tmp_load_result_n854;
        bool hxc_l_tmp_call_result_n856 = hxc_caxecraft_app_CaxecraftApp_dialogueActorsAreValid(hxc_l_tmp_load_result_n853, hxc_l_tmp_load_result_n854, hxc_l_actorPhases);
        bool hxc_l_tmp_short_circuit_result_n308 = !hxc_l_tmp_call_result_n856;
        if (!!hxc_l_tmp_call_result_n856)
        {
          hxc_l_tmp_short_circuit_result_n308 = !hxc_l_enemyPhase_h70382f3bdc66.hxc_valid;
        }
        bool hxc_l_tmp_short_circuit_load_result_n858 = hxc_l_tmp_short_circuit_result_n308;
        bool hxc_l_tmp_short_circuit_result_n309 = hxc_l_tmp_short_circuit_load_result_n858;
        if (!hxc_l_tmp_short_circuit_load_result_n858)
        {
          hxc_l_tmp_short_circuit_result_n309 = hxc_l_enemyActor.hxc_id <= 0;
        }
        if (hxc_l_tmp_short_circuit_result_n309)
        {
          hxc_l_quit = true;
        }
        struct hxc_caxecraft_gameplay_SwordCombatState hxc_l_tmp_call_result_n862 = hxc_caxecraft_gameplay_SwordCombat_step(hxc_l_swordCombat);
        hxc_l_swordCombat = hxc_l_tmp_call_result_n862;
        if (hxc_l_swordQueued)
        {
          struct hxc_caxecraft_gameplay_SwordCombatState hxc_l_tmp_load_result_n864 = hxc_l_swordCombat;
          struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_load_result_n865 = hxc_l_inventory;
          struct hxc_caxecraft_domain_VitalsState hxc_l_tmp_record_field_load_result_n866 = hxc_l_character.hxc_vitals;
          struct hxc_caxecraft_domain_Character hxc_l_tmp_load_result_n867 = hxc_l_enemyActor;
          double hxc_l_tmp_record_field_load_result_n868 = hxc_l_character.hxc_body.hxc_x;
          double hxc_l_tmp_record_field_load_result_n869 = hxc_l_character.hxc_body.hxc_z;
          double hxc_l_tmp_load_result_n870 = hxc_l_lookX;
          int32_t hxc_l_tmp_call_result_n872 = hxc_caxecraft_gameplay_SwordCombat_decide(hxc_l_tmp_load_result_n864, hxc_l_tmp_load_result_n865, hxc_l_tmp_record_field_load_result_n866, hxc_l_tmp_load_result_n867, hxc_l_tmp_record_field_load_result_n868, hxc_l_tmp_record_field_load_result_n869, hxc_l_tmp_load_result_n870, hxc_l_lookZ);
          int32_t hxc_l_swordDecision = hxc_l_tmp_call_result_n872;
          if (hxc_l_swordDecision == 5)
          {
            struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n874 = hxc_l_session;
            hxc_l_gc_roots[204] = (const void *)hxc_l_tmp_load_result_n874;
            if (hxc_l_tmp_load_result_n874 == NULL)
            {
              abort();
            }
            struct hxc_caxecraft_domain_CharacterDamageResult hxc_l_tmp_instance_call_result_n876 = hxc_caxecraft_domain_GameSession_damageCharacter(hxc_l_tmp_load_result_n874, hxc_l_enemyActorId, 1);
            struct hxc_caxecraft_domain_CharacterDamageResult hxc_l_damage = hxc_l_tmp_instance_call_result_n876;
            if (!hxc_l_damage.hxc_resolved)
            {
              hxc_l_quit = true;
            }
            else
            {
              if (hxc_l_damage.hxc_damageApplied > 0)
              {
                hxc_l_enemyActor = hxc_l_damage.hxc_character;
                hxc_l_strikeHitFrames = 16;
              }
            }
          }
          int32_t hxc_l_tmp_load_result_n880 = hxc_l_swordDecision;
          struct hxc_caxecraft_gameplay_SwordCombatState hxc_l_tmp_call_result_n882 = hxc_caxecraft_gameplay_SwordCombat_after(hxc_l_tmp_load_result_n880, hxc_l_swordCombat);
          hxc_l_swordCombat = hxc_l_tmp_call_result_n882;
          hxc_l_swordQueued = false;
        }
        if (hxc_array_ref_release(hxc_l_actorPhases) != HXC_STATUS_OK)
        {
          abort();
        }
      }
      hxc_l_jumpQueued = false;
      hxc_l_accumulator = hxc_l_accumulator - 0.05;
      hxc_optional_b8d3265c_destroy(&hxc_l_flow);
      hxc_record_9ab6a4ce_destroy(&hxc_l_gameTick);
    }
    double hxc_l_selectionEyeX = hxc_l_character.hxc_body.hxc_x;
    double hxc_l_selectionEyeY = hxc_l_character.hxc_body.hxc_y + 1.62;
    double hxc_l_selectionEyeZ = hxc_l_character.hxc_body.hxc_z;
    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n887 = hxc_l_session;
    hxc_l_gc_roots[184] = (const void *)hxc_l_tmp_load_result_n887;
    if (hxc_l_tmp_load_result_n887 == NULL)
    {
      abort();
    }
    size_t hxc_l_tmp_length_n889;
    const uint8_t *hxc_l_tmp_instance_call_result_n888 = hxc_caxecraft_domain_GameSession_worldView(hxc_l_tmp_load_result_n887, &hxc_l_tmp_length_n889);
    double hxc_l_tmp_load_result_n890 = hxc_l_selectionEyeX;
    double hxc_l_tmp_load_result_n891 = hxc_l_selectionEyeY;
    double hxc_l_tmp_load_result_n892 = hxc_l_selectionEyeZ;
    double hxc_l_tmp_load_result_n893 = hxc_l_lookX;
    double hxc_l_tmp_load_result_n894 = hxc_l_lookY;
    struct hxc_caxecraft_domain_RaycastHit hxc_l_tmp_call_result_n896 = hxc_caxecraft_domain_VoxelRaycast_trace(hxc_l_tmp_instance_call_result_n888, hxc_l_tmp_length_n889, hxc_l_tmp_load_result_n890, hxc_l_tmp_load_result_n891, hxc_l_tmp_load_result_n892, hxc_l_tmp_load_result_n893, hxc_l_tmp_load_result_n894, hxc_l_lookZ, 7.0);
    struct hxc_caxecraft_domain_RaycastHit hxc_l_hit = hxc_l_tmp_call_result_n896;
    bool hxc_l_tmp_load_result_n897 = hxc_l_captured_h599154fa3a25;
    bool hxc_l_tmp_short_circuit_result_n316 = hxc_l_tmp_load_result_n897;
    if (hxc_l_tmp_load_result_n897)
    {
      hxc_l_tmp_short_circuit_result_n316 = !hxc_l_recapturedThisFrame;
    }
    bool hxc_l_tmp_short_circuit_load_result_n899 = hxc_l_tmp_short_circuit_result_n316;
    bool hxc_l_tmp_short_circuit_result_n317 = hxc_l_tmp_short_circuit_load_result_n899;
    if (hxc_l_tmp_short_circuit_load_result_n899)
    {
      hxc_l_tmp_short_circuit_result_n317 = hxc_l_primaryPressed_h4a0a8b20cf08;
    }
    if (hxc_l_tmp_short_circuit_result_n317)
    {
      if (hxc_l_character.hxc_vitals.hxc_health > 0)
      {
        if (hxc_l_selectedMode == 1)
        {
          bool hxc_l_tmp_call_result_n906 = hxc_caxecraft_gameplay_Inventory_selectedIs(hxc_l_inventory, hxc_caxecraft_gameplay_ItemKind_CopperSword);
          bool hxc_l_tmp_short_circuit_result_n318 = !hxc_l_tmp_call_result_n906;
          if (!hxc_l_tmp_call_result_n906)
          {
            hxc_l_tmp_short_circuit_result_n318 = hxc_l_hit.hxc_hit;
          }
          bool hxc_l_tmp_short_circuit_load_result_n908 = hxc_l_tmp_short_circuit_result_n318;
          bool hxc_l_tmp_short_circuit_result_n319 = hxc_l_tmp_short_circuit_load_result_n908;
          if (hxc_l_tmp_short_circuit_load_result_n908)
          {
            struct hxc_caxecraft_domain_AquaticState hxc_l_tmp_record_field_load_result_n909 = hxc_l_character.hxc_aquatic;
            bool hxc_l_tmp_call_result_n911 = hxc_caxecraft_domain_Aquatics_canMine(hxc_l_tmp_record_field_load_result_n909, hxc_l_character.hxc_aquaticProfile);
            hxc_l_tmp_short_circuit_result_n319 = hxc_l_tmp_call_result_n911;
          }
          if (hxc_l_tmp_short_circuit_result_n319)
          {
            int32_t hxc_l_tmp_record_field_load_result_n913 = hxc_l_hit.hxc_cellX;
            int32_t hxc_l_tmp_record_field_load_result_n914 = hxc_l_hit.hxc_cellY;
            struct hxc_caxecraft_scenario_VoxelPoint hxc_l_tmp_call_result_n916 = hxc_caxecraft_domain_World_coord(hxc_l_tmp_record_field_load_result_n913, hxc_l_tmp_record_field_load_result_n914, hxc_l_hit.hxc_cellZ);
            struct hxc_caxecraft_scenario_VoxelPoint hxc_l_minedCoordinate = hxc_l_tmp_call_result_n916;
            int32_t hxc_l_sandBefore = hxc_l_inventory.hxc_sand;
            (void)hxc_l_sandBefore;
            struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n918 = hxc_l_session;
            hxc_l_gc_roots[205] = (const void *)hxc_l_tmp_load_result_n918;
            if (hxc_l_tmp_load_result_n918 == NULL)
            {
              abort();
            }
            struct hxc_caxecraft_scenario_VoxelPoint hxc_l_tmp_load_result_n919 = hxc_l_minedCoordinate;
            struct hxc_caxecraft_gameplay_MiningResult hxc_l_tmp_instance_call_result_n921 = hxc_caxecraft_domain_GameSession_mineTerrain(hxc_l_tmp_load_result_n918, hxc_l_tmp_load_result_n919, hxc_l_inventory);
            struct hxc_caxecraft_gameplay_MiningResult hxc_l_mining = hxc_l_tmp_instance_call_result_n921;
            hxc_l_inventory = hxc_l_mining.hxc_inventory;
            if (hxc_l_mining.hxc_outcome == 2)
            {
              if (hxc_l_self == NULL)
              {
                abort();
              }
              struct hxc_caxecraft_app_TerrainRenderer *hxc_l_tmp_owned_class_field_address_n924 = &(*hxc_l_self).hxc_terrainRenderer;
              struct hxc_caxecraft_app_TerrainChunkCache *hxc_l_tmp_owned_class_field_address_n925 = &(*hxc_l_tmp_owned_class_field_address_n924).hxc_cache;
              if (hxc_l_tmp_owned_class_field_address_n925 == NULL)
              {
                abort();
              }
              hxc_caxecraft_app_TerrainChunkCache_invalidate(hxc_l_tmp_owned_class_field_address_n925, hxc_l_minedCoordinate);
            }
            if (hxc_l_mining.hxc_outcome == 1)
            {
              hxc_l_inventoryFullReason = 2;
              hxc_l_inventoryFullFrames = 90;
            }
          }
        }
        else
        {
          if (hxc_l_hit.hxc_hit)
          {
            int32_t hxc_l_tmp_record_field_load_result_n929 = hxc_l_hit.hxc_cellX;
            int32_t hxc_l_tmp_record_field_load_result_n930 = hxc_l_hit.hxc_cellY;
            struct hxc_caxecraft_scenario_VoxelPoint hxc_l_tmp_call_result_n932 = hxc_caxecraft_domain_World_coord(hxc_l_tmp_record_field_load_result_n929, hxc_l_tmp_record_field_load_result_n930, hxc_l_hit.hxc_cellZ);
            struct hxc_caxecraft_scenario_VoxelPoint hxc_l_removedCoordinate = hxc_l_tmp_call_result_n932;
            struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n933 = hxc_l_session;
            hxc_l_gc_roots[206] = (const void *)hxc_l_tmp_load_result_n933;
            if (hxc_l_tmp_load_result_n933 == NULL)
            {
              abort();
            }
            bool hxc_l_tmp_instance_call_result_n935 = hxc_caxecraft_domain_GameSession_removeTerrain(hxc_l_tmp_load_result_n933, hxc_l_removedCoordinate);
            bool hxc_l_removed = hxc_l_tmp_instance_call_result_n935;
            if (hxc_l_removed)
            {
              if (hxc_l_self == NULL)
              {
                abort();
              }
              struct hxc_caxecraft_app_TerrainRenderer *hxc_l_tmp_owned_class_field_address_n937 = &(*hxc_l_self).hxc_terrainRenderer;
              struct hxc_caxecraft_app_TerrainChunkCache *hxc_l_tmp_owned_class_field_address_n938 = &(*hxc_l_tmp_owned_class_field_address_n937).hxc_cache;
              if (hxc_l_tmp_owned_class_field_address_n938 == NULL)
              {
                abort();
              }
              hxc_caxecraft_app_TerrainChunkCache_invalidate(hxc_l_tmp_owned_class_field_address_n938, hxc_l_removedCoordinate);
            }
          }
        }
      }
    }
    bool hxc_l_tmp_load_result_n940 = hxc_l_captured_h599154fa3a25;
    bool hxc_l_tmp_short_circuit_result_n325 = hxc_l_tmp_load_result_n940;
    if (hxc_l_tmp_load_result_n940)
    {
      hxc_l_tmp_short_circuit_result_n325 = hxc_l_secondaryPressed_h3c635e3846cc;
    }
    if (hxc_l_tmp_short_circuit_result_n325)
    {
      if (hxc_l_character.hxc_vitals.hxc_health > 0)
      {
        struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n944 = hxc_l_session;
        hxc_l_gc_roots[207] = (const void *)hxc_l_tmp_load_result_n944;
        if (hxc_l_tmp_load_result_n944 == NULL)
        {
          abort();
        }
        struct hxc_caxecraft_domain_LocalRecoveryResult hxc_l_tmp_instance_call_result_n946 = hxc_caxecraft_domain_GameSession_useSelectedRecovery(hxc_l_tmp_load_result_n944, hxc_l_inventory);
        struct hxc_caxecraft_domain_LocalRecoveryResult hxc_l_recovery = hxc_l_tmp_instance_call_result_n946;
        hxc_l_character = hxc_l_recovery.hxc_character;
        if (!hxc_l_recovery.hxc_resolved)
        {
          hxc_l_quit = true;
        }
        else
        {
          if (hxc_l_recovery.hxc_decision != 0)
          {
            hxc_l_recoveryFeedback = hxc_l_recovery.hxc_decision;
            hxc_l_recoveryFeedbackFrames = 90;
            hxc_l_inventory = hxc_l_recovery.hxc_inventory;
          }
          else
          {
            if (hxc_l_hit.hxc_hit)
            {
              int32_t hxc_l_tmp_record_field_load_result_n953 = hxc_l_hit.hxc_previousX;
              int32_t hxc_l_tmp_record_field_load_result_n954 = hxc_l_hit.hxc_previousY;
              struct hxc_caxecraft_scenario_VoxelPoint hxc_l_tmp_call_result_n956 = hxc_caxecraft_domain_World_coord(hxc_l_tmp_record_field_load_result_n953, hxc_l_tmp_record_field_load_result_n954, hxc_l_hit.hxc_previousZ);
              struct hxc_caxecraft_scenario_VoxelPoint hxc_l_placement = hxc_l_tmp_call_result_n956;
              enum hxc_caxecraft_domain_BlockKind hxc_l_tmp_call_result_n958 = hxc_caxecraft_gameplay_Inventory_selectedBlock(hxc_l_inventory);
              enum hxc_caxecraft_domain_BlockKind hxc_l_selectedBlock = hxc_l_tmp_call_result_n958;
              struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_load_result_n959 = hxc_l_inventory;
              int32_t hxc_l_tmp_call_result_n961 = hxc_caxecraft_gameplay_Inventory_countAt(hxc_l_tmp_load_result_n959, hxc_l_inventory.hxc_selected);
              bool hxc_l_hasItem = hxc_l_tmp_call_result_n961 > 0;
              bool hxc_l_tmp_load_result_n962 = hxc_l_hasItem;
              bool hxc_l_tmp_short_circuit_result_n330 = !hxc_l_tmp_load_result_n962;
              if (!!hxc_l_tmp_load_result_n962)
              {
                bool hxc_l_tmp_call_result_n964 = hxc_caxecraft_domain_World_isPlaceable(hxc_l_selectedBlock);
                hxc_l_tmp_short_circuit_result_n330 = !hxc_l_tmp_call_result_n964;
              }
              bool hxc_l_tmp_short_circuit_load_result_n965 = hxc_l_tmp_short_circuit_result_n330;
              bool hxc_l_tmp_short_circuit_result_n331 = hxc_l_tmp_short_circuit_load_result_n965;
              if (!hxc_l_tmp_short_circuit_load_result_n965)
              {
                struct hxc_caxecraft_domain_CharacterBody hxc_l_tmp_record_field_load_result_n966 = hxc_l_character.hxc_body;
                bool hxc_l_tmp_call_result_n968 = hxc_caxecraft_domain_CharacterPhysics_canPlaceAt(hxc_l_tmp_record_field_load_result_n966, hxc_l_placement);
                hxc_l_tmp_short_circuit_result_n331 = !hxc_l_tmp_call_result_n968;
              }
              bool hxc_l_tmp_short_circuit_load_result_n969 = hxc_l_tmp_short_circuit_result_n331;
              bool hxc_l_tmp_short_circuit_result_n332 = hxc_l_tmp_short_circuit_load_result_n969;
              if (!hxc_l_tmp_short_circuit_load_result_n969)
              {
                struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n970 = hxc_l_session;
                hxc_l_gc_roots[208] = (const void *)hxc_l_tmp_load_result_n970;
                if (hxc_l_tmp_load_result_n970 == NULL)
                {
                  abort();
                }
                struct hxc_caxecraft_scenario_VoxelPoint hxc_l_tmp_load_result_n971 = hxc_l_placement;
                bool hxc_l_tmp_instance_call_result_n973 = hxc_caxecraft_domain_GameSession_placeTerrain(hxc_l_tmp_load_result_n970, hxc_l_tmp_load_result_n971, hxc_l_selectedBlock);
                hxc_l_tmp_short_circuit_result_n332 = !hxc_l_tmp_instance_call_result_n973;
              }
              if (hxc_l_tmp_short_circuit_result_n332)
              {
                hxc_l_placementBlockedFrames = 60;
              }
              else
              {
                if (hxc_l_self == NULL)
                {
                  abort();
                }
                struct hxc_caxecraft_app_TerrainRenderer *hxc_l_tmp_owned_class_field_address_n975 = &(*hxc_l_self).hxc_terrainRenderer;
                struct hxc_caxecraft_app_TerrainChunkCache *hxc_l_tmp_owned_class_field_address_n976 = &(*hxc_l_tmp_owned_class_field_address_n975).hxc_cache;
                if (hxc_l_tmp_owned_class_field_address_n976 == NULL)
                {
                  abort();
                }
                hxc_caxecraft_app_TerrainChunkCache_invalidate(hxc_l_tmp_owned_class_field_address_n976, hxc_l_placement);
                if (hxc_l_selectedMode == 1)
                {
                  struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n980 = hxc_caxecraft_gameplay_Inventory_consumeSelected(hxc_l_inventory);
                  hxc_l_inventory = hxc_l_tmp_call_result_n980;
                }
              }
            }
          }
        }
      }
    }
    if (hxc_l_placementBlockedFrames > 0)
    {
      hxc_l_placementBlockedFrames = hxc_i32_subtract_wrapping(hxc_l_placementBlockedFrames, 1);
    }
    if (!hxc_l_paused_h8182f296b655)
    {
      struct hxc_caxecraft_gameplay_BerryDropState hxc_l_tmp_load_result_n984 = hxc_l_berryDrop;
      double hxc_l_tmp_record_field_load_result_n985 = hxc_l_character.hxc_body.hxc_x;
      double hxc_l_tmp_record_field_load_result_n986 = hxc_l_character.hxc_body.hxc_y;
      bool hxc_l_tmp_call_result_n988 = hxc_caxecraft_gameplay_BerryDrop_isInRange(hxc_l_tmp_load_result_n984, hxc_l_tmp_record_field_load_result_n985, hxc_l_tmp_record_field_load_result_n986, hxc_l_character.hxc_body.hxc_z);
      if (hxc_l_tmp_call_result_n988)
      {
        struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_load_result_n989 = hxc_l_inventory;
        int32_t hxc_l_tmp_call_result_n992 = hxc_caxecraft_gameplay_Inventory_acceptedAmount(hxc_l_tmp_load_result_n989, hxc_caxecraft_gameplay_ItemKind_Berries, hxc_l_berryDrop.hxc_amount);
        int32_t hxc_l_acceptedDrop = hxc_l_tmp_call_result_n992;
        if (hxc_l_acceptedDrop > 0)
        {
          struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_load_result_n994 = hxc_l_inventory;
          struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n997 = hxc_caxecraft_gameplay_Inventory_collectItem(hxc_l_tmp_load_result_n994, hxc_caxecraft_gameplay_ItemKind_Berries, hxc_l_acceptedDrop);
          hxc_l_inventory = hxc_l_tmp_call_result_n997;
          struct hxc_caxecraft_gameplay_BerryDropState hxc_l_tmp_load_result_n998 = hxc_l_berryDrop;
          struct hxc_caxecraft_gameplay_BerryDropState hxc_l_tmp_call_result_n1000 = hxc_caxecraft_gameplay_BerryDrop_collectAmount(hxc_l_tmp_load_result_n998, hxc_l_acceptedDrop);
          hxc_l_berryDrop = hxc_l_tmp_call_result_n1000;
          hxc_l_pickupAmount = hxc_l_acceptedDrop;
          hxc_l_pickupFrames = 90;
        }
        else
        {
          hxc_l_inventoryFullReason = 1;
          hxc_l_inventoryFullFrames = 90;
        }
      }
    }
    if (hxc_l_strikeHitFrames > 0)
    {
      hxc_l_strikeHitFrames = hxc_i32_subtract_wrapping(hxc_l_strikeHitFrames, 1);
    }
    if (hxc_l_enemyDefeatedFrames > 0)
    {
      hxc_l_enemyDefeatedFrames = hxc_i32_subtract_wrapping(hxc_l_enemyDefeatedFrames, 1);
    }
    if (hxc_l_enemyAttackFrames > 0)
    {
      hxc_l_enemyAttackFrames = hxc_i32_subtract_wrapping(hxc_l_enemyAttackFrames, 1);
    }
    if (hxc_l_pickupFrames > 0)
    {
      hxc_l_pickupFrames = hxc_i32_subtract_wrapping(hxc_l_pickupFrames, 1);
    }
    if (hxc_l_inventoryFullFrames > 0)
    {
      hxc_l_inventoryFullFrames = hxc_i32_subtract_wrapping(hxc_l_inventoryFullFrames, 1);
      if (hxc_l_inventoryFullFrames == 0)
      {
        hxc_l_inventoryFullReason = 0;
      }
    }
    if (hxc_l_recoveryFeedbackFrames > 0)
    {
      hxc_l_recoveryFeedbackFrames = hxc_i32_subtract_wrapping(hxc_l_recoveryFeedbackFrames, 1);
    }
    if (hxc_l_aquaticEquipmentFrames > 0)
    {
      hxc_l_aquaticEquipmentFrames = hxc_i32_subtract_wrapping(hxc_l_aquaticEquipmentFrames, 1);
    }
    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1017 = hxc_l_session;
    hxc_l_gc_roots[209] = (const void *)hxc_l_tmp_load_result_n1017;
    if (hxc_l_tmp_load_result_n1017 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_domain_GameView hxc_l_tmp_instance_call_result_n1018 = hxc_caxecraft_domain_GameSession_view(hxc_l_tmp_load_result_n1017);
    struct hxc_caxecraft_domain_GameView hxc_l_committedView = hxc_l_tmp_instance_call_result_n1018;
    if (!hxc_l_committedView.hxc_valid)
    {
      hxc_l_quit = true;
    }
    else
    {
      hxc_l_character = hxc_l_committedView.hxc_localPlayer;
    }
    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1021 = hxc_l_session;
    hxc_l_gc_roots[210] = (const void *)hxc_l_tmp_load_result_n1021;
    hxc_l_gc_roots[211] = (const void *)hxc_l_levelView_ha9fa307295b9;
    struct hxc_caxecraft_app_CaxecraftApp_AvailableInteractionTarget hxc_l_tmp_call_result_n1023 = hxc_caxecraft_app_CaxecraftApp_nearestAvailableInteraction(hxc_l_tmp_load_result_n1021, hxc_l_levelView_ha9fa307295b9);
    hxc_l_tmp_static_call_argument_0_owner_n335 = hxc_l_tmp_call_result_n1023;
    bool hxc_l_tmp_call_result_n1025 = hxc_caxecraft_app_CaxecraftApp_hasAvailableInteraction(hxc_l_tmp_static_call_argument_0_owner_n335);
    hxc_l_guideInteractionAvailable = hxc_l_tmp_call_result_n1025;
    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1026 = hxc_l_session;
    hxc_l_gc_roots[212] = (const void *)hxc_l_tmp_load_result_n1026;
    if (hxc_l_tmp_load_result_n1026 == NULL)
    {
      abort();
    }
    struct hxc_array_ref *hxc_l_tmp_instance_call_result_n1027 = hxc_caxecraft_domain_GameSession_actorControllerStateSnapshots(hxc_l_tmp_load_result_n1026);
    hxc_l_presentationActorPhases = hxc_l_tmp_instance_call_result_n1027;
    struct hxc_array_ref *hxc_l_tmp_load_result_n1028 = hxc_l_presentationActorPhases;
    struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation hxc_l_tmp_call_result_n1031 = hxc_caxecraft_app_CaxecraftApp_observeActorPhase(hxc_l_tmp_load_result_n1028, hxc_l_enemyActorId, hxc_caxecraft_domain_ActorControllerPhase_Defeated);
    hxc_l_enemyPhase_h70382f3bdc66 = hxc_l_tmp_call_result_n1031;
    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1032 = hxc_l_session;
    hxc_l_gc_roots[213] = (const void *)hxc_l_tmp_load_result_n1032;
    if (hxc_l_tmp_load_result_n1032 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n1034 = hxc_caxecraft_domain_GameSession_readCharacter(hxc_l_tmp_load_result_n1032, hxc_l_enemyActorId);
    hxc_l_enemyActor = hxc_l_tmp_instance_call_result_n1034;
    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1035 = hxc_l_session;
    hxc_l_gc_roots[214] = (const void *)hxc_l_tmp_load_result_n1035;
    struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1036 = hxc_l_levelView_ha9fa307295b9;
    hxc_l_gc_roots[215] = (const void *)hxc_l_tmp_load_result_n1036;
    bool hxc_l_tmp_call_result_n1038 = hxc_caxecraft_app_CaxecraftApp_dialogueActorsAreValid(hxc_l_tmp_load_result_n1035, hxc_l_tmp_load_result_n1036, hxc_l_presentationActorPhases);
    bool hxc_l_tmp_short_circuit_result_n337 = !hxc_l_tmp_call_result_n1038;
    if (!!hxc_l_tmp_call_result_n1038)
    {
      hxc_l_tmp_short_circuit_result_n337 = !hxc_l_enemyPhase_h70382f3bdc66.hxc_valid;
    }
    bool hxc_l_tmp_short_circuit_load_result_n1040 = hxc_l_tmp_short_circuit_result_n337;
    bool hxc_l_tmp_short_circuit_result_n338 = hxc_l_tmp_short_circuit_load_result_n1040;
    if (!hxc_l_tmp_short_circuit_load_result_n1040)
    {
      hxc_l_tmp_short_circuit_result_n338 = hxc_l_enemyActor.hxc_id <= 0;
    }
    if (hxc_l_tmp_short_circuit_result_n338)
    {
      hxc_l_quit = true;
    }
    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1043 = hxc_l_session;
    hxc_l_gc_roots[216] = (const void *)hxc_l_tmp_load_result_n1043;
    hxc_l_gc_roots[217] = (const void *)hxc_l_levelView_ha9fa307295b9;
    struct hxc_array_ref *hxc_l_tmp_call_result_n1045 = hxc_caxecraft_app_CaxecraftApp_dialogueActorSnapshots(hxc_l_tmp_load_result_n1043, hxc_l_levelView_ha9fa307295b9);
    hxc_l_dialogueActors = hxc_l_tmp_call_result_n1045;
    int32_t hxc_l_completedTicks = hxc_l_committedView.hxc_completedTicks;
    if (hxc_l_resetMotionThisFrame)
    {
      struct hxc_caxecraft_app_MotionHistory hxc_l_tmp_call_result_n1049 = hxc_caxecraft_app_MotionInterpolation_start(hxc_l_character.hxc_body);
      hxc_l_motionHistory = hxc_l_tmp_call_result_n1049;
    }
    struct hxc_caxecraft_app_MotionHistory hxc_l_tmp_load_result_n1050 = hxc_l_motionHistory;
    struct hxc_caxecraft_app_RenderPosition hxc_l_tmp_call_result_n1052 = hxc_caxecraft_app_MotionInterpolation_sample(hxc_l_tmp_load_result_n1050, hxc_l_accumulator, 0.05);
    struct hxc_caxecraft_app_RenderPosition hxc_l_renderPosition = hxc_l_tmp_call_result_n1052;
    double hxc_l_eyeX = hxc_l_renderPosition.hxc_x;
    double hxc_l_eyeY = hxc_l_renderPosition.hxc_y + 1.62;
    double hxc_l_eyeZ = hxc_l_renderPosition.hxc_z;
    double hxc_l_tmp_load_result_n1056 = hxc_l_eyeX;
    double hxc_l_tmp_load_result_n1057 = hxc_l_eyeY;
    double hxc_l_tmp_load_result_n1058 = hxc_l_eyeZ;
    double hxc_l_tmp_load_result_n1060 = hxc_l_eyeX;
    double hxc_l_tmp_load_result_n1061 = hxc_l_lookX;
    double hxc_l_tmp_load_result_n1062 = hxc_l_eyeY;
    double hxc_l_tmp_load_result_n1063 = hxc_l_lookY;
    double hxc_l_tmp_load_result_n1064 = hxc_l_eyeZ;
    struct Camera3D hxc_l_camera = (struct Camera3D){ .position = (struct Vector3){ .x = (float)hxc_l_tmp_load_result_n1056, .y = (float)hxc_l_tmp_load_result_n1057, .z = (float)hxc_l_tmp_load_result_n1058 }, .target = (struct Vector3){ .x = (float)(hxc_l_tmp_load_result_n1060 + hxc_l_tmp_load_result_n1061), .y = (float)(hxc_l_tmp_load_result_n1062 + hxc_l_tmp_load_result_n1063), .z = (float)(hxc_l_tmp_load_result_n1064 + hxc_l_lookZ) }, .up = (struct Vector3){ .x = (float)0.0, .y = (float)1.0, .z = (float)0.0 }, .fovy = (float)70.0, .projection = 0 };
    BeginDrawing();
    if (hxc_l_onTitle)
    {
      hxc_string hxc_l_locale1_h076ea791ac85 = { 0 };
      switch (hxc_l_locale) {
        case 0:
          {
            hxc_l_locale1_h076ea791ac85 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
            if (hxc_string_retain(hxc_l_locale1_h076ea791ac85) != HXC_STATUS_OK)
            {
              abort();
            }
            break;
          }
        case 1:
          {
            hxc_l_locale1_h076ea791ac85 = (hxc_string){ (const uint8_t *)"es-mx", 5, true, NULL };
            if (hxc_string_retain(hxc_l_locale1_h076ea791ac85) != HXC_STATUS_OK)
            {
              abort();
            }
            break;
          }
        default:
          {
            hxc_l_locale1_h076ea791ac85 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
            if (hxc_string_retain(hxc_l_locale1_h076ea791ac85) != HXC_STATUS_OK)
            {
              abort();
            }
            break;
          }
      }
      hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n1071 = hxc_l_locale1_h076ea791ac85;
      hxc_l_tmp_managed_flow_owner_n347 = hxc_l_tmp_managed_flow_carrier_move_result_n1071;
      struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1072 = hxc_l_levelView_ha9fa307295b9;
      hxc_l_gc_roots[218] = (const void *)hxc_l_tmp_load_result_n1072;
      if (hxc_l_tmp_load_result_n1072 == NULL)
      {
        abort();
      }
      struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_class_field_load_result_n1073 = (*hxc_l_tmp_load_result_n1072).hxc_levelPresentation;
      hxc_l_gc_roots[219] = (const void *)hxc_l_tmp_class_field_load_result_n1073;
      if (hxc_l_tmp_class_field_load_result_n1073 == NULL)
      {
        abort();
      }
      hxc_string hxc_l_tmp_instance_call_result_n1075 = hxc_caxecraft_content_RuntimeLevelPresentation_message(hxc_l_tmp_class_field_load_result_n1073, (hxc_string){ (const uint8_t *)"adventure_tagline", 17, true, NULL }, hxc_l_tmp_managed_flow_owner_n347);
      hxc_l_adventureTagline = hxc_l_tmp_instance_call_result_n1075;
      int32_t hxc_l_tmp_native_call_result_n1076 = GetScreenWidth();
      int32_t hxc_l_width_h0d6a9e51bd05 = (int32_t)hxc_l_tmp_native_call_result_n1076;
      int32_t hxc_l_tmp_native_call_result_n1077 = GetScreenHeight();
      int32_t hxc_l_height_h4ce7f7275f62 = (int32_t)hxc_l_tmp_native_call_result_n1077;
      if (hxc_l_titleTextureReady)
      {
        struct Color hxc_l_this1_h7fb0ccf6eb78 = (struct Color){ .r = 255, .g = 255, .b = 255, .a = 255 };
        struct Texture hxc_l_tmp_load_result_n1080 = hxc_l_titleTexture;
        double hxc_l_imageAspect = hxc_f64_divide_zero_safe((double)(int32_t)hxc_l_tmp_load_result_n1080.width, (double)(int32_t)hxc_l_titleTexture.height);
        int32_t hxc_l_tmp_load_result_n1084 = hxc_l_width_h0d6a9e51bd05;
        double hxc_l_screenAspect = hxc_f64_divide_zero_safe((double)hxc_l_tmp_load_result_n1084, (double)hxc_l_height_h4ce7f7275f62);
        double hxc_l_sourceX = 0.0;
        double hxc_l_sourceY = 0.0;
        double hxc_l_sourceWidth = (double)(int32_t)hxc_l_titleTexture.width;
        double hxc_l_sourceHeight = (double)(int32_t)hxc_l_titleTexture.height;
        double hxc_l_tmp_load_result_n1090 = hxc_l_screenAspect;
        if (hxc_l_tmp_load_result_n1090 > hxc_l_imageAspect)
        {
          struct Texture hxc_l_tmp_load_result_n1092 = hxc_l_titleTexture;
          hxc_l_sourceHeight = hxc_f64_divide_zero_safe((double)(int32_t)hxc_l_tmp_load_result_n1092.width, hxc_l_screenAspect);
          struct Texture hxc_l_tmp_load_result_n1095 = hxc_l_titleTexture;
          hxc_l_sourceY = ((double)(int32_t)hxc_l_tmp_load_result_n1095.height - hxc_l_sourceHeight) * 0.5;
        }
        else
        {
          struct Texture hxc_l_tmp_load_result_n1098 = hxc_l_titleTexture;
          hxc_l_sourceWidth = (double)(int32_t)hxc_l_tmp_load_result_n1098.height * hxc_l_screenAspect;
          struct Texture hxc_l_tmp_load_result_n1101 = hxc_l_titleTexture;
          hxc_l_sourceX = ((double)(int32_t)hxc_l_tmp_load_result_n1101.width - hxc_l_sourceWidth) * 0.5;
        }
        struct Texture hxc_l_tmp_load_result_n1104 = hxc_l_titleTexture;
        double hxc_l_tmp_load_result_n1105 = hxc_l_sourceX;
        double hxc_l_tmp_load_result_n1106 = hxc_l_sourceY;
        double hxc_l_tmp_load_result_n1107 = hxc_l_sourceWidth;
        double hxc_l_tmp_load_result_n1108 = hxc_l_sourceHeight;
        int32_t hxc_l_tmp_load_result_n1110 = hxc_l_width_h0d6a9e51bd05;
        int32_t hxc_l_tmp_load_result_n1111 = hxc_l_height_h4ce7f7275f62;
        DrawTexturePro(hxc_l_tmp_load_result_n1104, (struct Rectangle){ .x = (float)hxc_l_tmp_load_result_n1105, .y = (float)hxc_l_tmp_load_result_n1106, .width = (float)hxc_l_tmp_load_result_n1107, .height = (float)hxc_l_tmp_load_result_n1108 }, (struct Rectangle){ .x = (float)0.0, .y = (float)0.0, .width = (float)((double)hxc_l_tmp_load_result_n1110 + 0.0), .height = (float)((double)hxc_l_tmp_load_result_n1111 + 0.0) }, (struct Vector2){ .x = (float)0.0, .y = (float)0.0 }, (float)0.0, hxc_l_this1_h7fb0ccf6eb78);
      }
      else
      {
        ClearBackground((struct Color){ .r = 126, .g = 190, .b = 201, .a = 255 });
      }
      struct Color hxc_l_this1_haa1b293590d2 = (struct Color){ .r = 4, .g = 14, .b = 23, .a = 132 };
      int32_t hxc_l_tmp_load_result_n1117 = hxc_l_width_h0d6a9e51bd05;
      int32_t hxc_l_tmp_load_result_n1118 = hxc_l_height_h4ce7f7275f62;
      DrawRectangle((int32_t)0, (int32_t)0, (int32_t)hxc_l_tmp_load_result_n1117, (int32_t)hxc_l_tmp_load_result_n1118, hxc_l_this1_haa1b293590d2);
      if (hxc_l_wordmarkTextureReady)
      {
        int32_t hxc_l_centerX = hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_l_width_h0d6a9e51bd05, (double)2));
        struct Color hxc_l_this1_h0714c48529f6 = (struct Color){ .r = 255, .g = 255, .b = 255, .a = 255 };
        double hxc_l_scale = hxc_f64_divide_zero_safe((double)560, (double)(int32_t)hxc_l_wordmarkTexture.width);
        double hxc_l_heightScale = hxc_f64_divide_zero_safe((double)132, (double)(int32_t)hxc_l_wordmarkTexture.height);
        double hxc_l_tmp_load_result_n1127 = hxc_l_heightScale;
        if (hxc_l_tmp_load_result_n1127 < hxc_l_scale)
        {
          hxc_l_scale = hxc_l_heightScale;
        }
        struct Texture hxc_l_tmp_load_result_n1130 = hxc_l_wordmarkTexture;
        double hxc_l_width_h82a3b8b307d2 = (double)(int32_t)hxc_l_tmp_load_result_n1130.width * hxc_l_scale;
        struct Texture hxc_l_tmp_load_result_n1133 = hxc_l_wordmarkTexture;
        double hxc_l_height_hd0b2dcac9c5b = (double)(int32_t)hxc_l_tmp_load_result_n1133.height * hxc_l_scale;
        double hxc_l_width1 = (double)(int32_t)hxc_l_wordmarkTexture.width + 0.0;
        double hxc_l_height1 = (double)(int32_t)hxc_l_wordmarkTexture.height + 0.0;
        struct Texture hxc_l_tmp_load_result_n1140 = hxc_l_wordmarkTexture;
        double hxc_l_tmp_load_result_n1141 = hxc_l_width1;
        double hxc_l_tmp_load_result_n1142 = hxc_l_height1;
        int32_t hxc_l_tmp_load_result_n1144 = hxc_l_centerX;
        double hxc_l_tmp_load_result_n1145 = hxc_l_width_h82a3b8b307d2;
        double hxc_l_tmp_load_result_n1146 = hxc_l_width_h82a3b8b307d2;
        double hxc_l_tmp_load_result_n1147 = hxc_l_height_hd0b2dcac9c5b;
        DrawTexturePro(hxc_l_tmp_load_result_n1140, (struct Rectangle){ .x = (float)0.0, .y = (float)0.0, .width = (float)hxc_l_tmp_load_result_n1141, .height = (float)hxc_l_tmp_load_result_n1142 }, (struct Rectangle){ .x = (float)((double)hxc_l_tmp_load_result_n1144 - hxc_l_tmp_load_result_n1145 * 0.5), .y = (float)28.0, .width = (float)hxc_l_tmp_load_result_n1146, .height = (float)hxc_l_tmp_load_result_n1147 }, (struct Vector2){ .x = (float)0.0, .y = (float)0.0 }, (float)0.0, hxc_l_this1_h0714c48529f6);
      }
      else
      {
        int32_t hxc_l_x_ha56414ae853c = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_l_width_h0d6a9e51bd05, (double)2)), 92);
        struct Color hxc_l_this1_h2bf46de51f33 = (struct Color){ .r = 242, .g = 249, .b = 245, .a = 255 };
        struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_tmp_load_result_n1153 = hxc_l_uiCatalog;
        hxc_l_gc_roots[220] = (const void *)hxc_l_tmp_load_result_n1153;
        if (hxc_l_tmp_load_result_n1153 == NULL)
        {
          abort();
        }
        hxc_string hxc_l_tmp_instance_call_result_n1155 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_tmp_load_result_n1153, hxc_l_locale, 34);
        hxc_l_tmp_cstring_ref_owner_owner_n369 = hxc_l_tmp_instance_call_result_n1155;
        hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n1157 = HXC_BORROWED_CSTRING_INITIALIZER;
        if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n369, &hxc_l_tmp_string_borrow_cstring_result_n1157) != HXC_STATUS_OK)
        {
          abort();
        }
        int32_t hxc_l_tmp_load_result_n1158 = hxc_l_x_ha56414ae853c;
        DrawText(hxc_l_tmp_string_borrow_cstring_result_n1157.data, (int32_t)hxc_l_tmp_load_result_n1158, (int32_t)52, (int32_t)34, hxc_l_this1_h2bf46de51f33);
        if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n369) != HXC_STATUS_OK)
        {
          abort();
        }
      }
      int32_t hxc_l_firstTop = hxc_i32_subtract_wrapping(hxc_l_height_h4ce7f7275f62, 293);
      int32_t hxc_l_left_h642fb0a00944 = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_l_width_h0d6a9e51bd05, (double)2)), 180);
      if (hxc_l_selectedMode == 0)
      {
        struct Color hxc_l_this1_hf1ebe3fa4b17 = (struct Color){ .r = 16, .g = 88, .b = 102, .a = 232 };
        int32_t hxc_l_tmp_load_result_n1164 = hxc_l_left_h642fb0a00944;
        int32_t hxc_l_tmp_load_result_n1165 = hxc_l_firstTop;
        DrawRectangle((int32_t)hxc_l_tmp_load_result_n1164, (int32_t)hxc_l_tmp_load_result_n1165, (int32_t)360, (int32_t)54, hxc_l_this1_hf1ebe3fa4b17);
        struct Color hxc_l_this1_hc15ddba62631 = (struct Color){ .r = 255, .g = 190, .b = 55, .a = 255 };
        int32_t hxc_l_tmp_load_result_n1168 = hxc_l_left_h642fb0a00944;
        int32_t hxc_l_tmp_load_result_n1169 = hxc_l_firstTop;
        DrawRectangleLines((int32_t)hxc_l_tmp_load_result_n1168, (int32_t)hxc_l_tmp_load_result_n1169, (int32_t)360, (int32_t)54, hxc_l_this1_hc15ddba62631);
      }
      else
      {
        struct Color hxc_l_this1_h075d852a32a1 = (struct Color){ .r = 6, .g = 26, .b = 36, .a = 218 };
        int32_t hxc_l_tmp_load_result_n1172 = hxc_l_left_h642fb0a00944;
        int32_t hxc_l_tmp_load_result_n1173 = hxc_l_firstTop;
        DrawRectangle((int32_t)hxc_l_tmp_load_result_n1172, (int32_t)hxc_l_tmp_load_result_n1173, (int32_t)360, (int32_t)54, hxc_l_this1_h075d852a32a1);
        struct Color hxc_l_this1_h0fb0cd788821 = (struct Color){ .r = 92, .g = 194, .b = 188, .a = 255 };
        int32_t hxc_l_tmp_load_result_n1176 = hxc_l_left_h642fb0a00944;
        int32_t hxc_l_tmp_load_result_n1177 = hxc_l_firstTop;
        DrawRectangleLines((int32_t)hxc_l_tmp_load_result_n1176, (int32_t)hxc_l_tmp_load_result_n1177, (int32_t)360, (int32_t)54, hxc_l_this1_h0fb0cd788821);
      }
      struct Color hxc_l_this1_h97e2d84fe1a6 = (struct Color){ .r = 242, .g = 249, .b = 245, .a = 255 };
      struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_tmp_load_result_n1180 = hxc_l_uiCatalog;
      hxc_l_gc_roots[221] = (const void *)hxc_l_tmp_load_result_n1180;
      if (hxc_l_tmp_load_result_n1180 == NULL)
      {
        abort();
      }
      hxc_string hxc_l_tmp_instance_call_result_n1182 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_tmp_load_result_n1180, hxc_l_locale, 27);
      hxc_l_tmp_cstring_ref_owner_owner_n377 = hxc_l_tmp_instance_call_result_n1182;
      hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n1184 = HXC_BORROWED_CSTRING_INITIALIZER;
      if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n377, &hxc_l_tmp_string_borrow_cstring_result_n1184) != HXC_STATUS_OK)
      {
        abort();
      }
      int32_t hxc_l_tmp_load_result_n1185 = hxc_l_left_h642fb0a00944;
      int32_t hxc_l_tmp_load_result_n1186 = hxc_l_firstTop;
      DrawText(hxc_l_tmp_string_borrow_cstring_result_n1184.data, (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n1185, 28), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n1186, 16), (int32_t)21, hxc_l_this1_h97e2d84fe1a6);
      int32_t hxc_l_top_h0816c0ac00dc = hxc_i32_add_wrapping(hxc_i32_add_wrapping(hxc_l_firstTop, 54), 14);
      int32_t hxc_l_left_h29638ea69142 = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_l_width_h0d6a9e51bd05, (double)2)), 180);
      if (hxc_l_selectedMode == 1)
      {
        struct Color hxc_l_this1_h9b7d45581483 = (struct Color){ .r = 16, .g = 88, .b = 102, .a = 232 };
        int32_t hxc_l_tmp_load_result_n1192 = hxc_l_left_h29638ea69142;
        int32_t hxc_l_tmp_load_result_n1193 = hxc_l_top_h0816c0ac00dc;
        DrawRectangle((int32_t)hxc_l_tmp_load_result_n1192, (int32_t)hxc_l_tmp_load_result_n1193, (int32_t)360, (int32_t)54, hxc_l_this1_h9b7d45581483);
        struct Color hxc_l_this1_h1141ffe7fd46 = (struct Color){ .r = 255, .g = 190, .b = 55, .a = 255 };
        int32_t hxc_l_tmp_load_result_n1196 = hxc_l_left_h29638ea69142;
        int32_t hxc_l_tmp_load_result_n1197 = hxc_l_top_h0816c0ac00dc;
        DrawRectangleLines((int32_t)hxc_l_tmp_load_result_n1196, (int32_t)hxc_l_tmp_load_result_n1197, (int32_t)360, (int32_t)54, hxc_l_this1_h1141ffe7fd46);
      }
      else
      {
        struct Color hxc_l_this1_h1f91afaa7099 = (struct Color){ .r = 6, .g = 26, .b = 36, .a = 218 };
        int32_t hxc_l_tmp_load_result_n1200 = hxc_l_left_h29638ea69142;
        int32_t hxc_l_tmp_load_result_n1201 = hxc_l_top_h0816c0ac00dc;
        DrawRectangle((int32_t)hxc_l_tmp_load_result_n1200, (int32_t)hxc_l_tmp_load_result_n1201, (int32_t)360, (int32_t)54, hxc_l_this1_h1f91afaa7099);
        struct Color hxc_l_this1_h6533ab967d3f = (struct Color){ .r = 92, .g = 194, .b = 188, .a = 255 };
        int32_t hxc_l_tmp_load_result_n1204 = hxc_l_left_h29638ea69142;
        int32_t hxc_l_tmp_load_result_n1205 = hxc_l_top_h0816c0ac00dc;
        DrawRectangleLines((int32_t)hxc_l_tmp_load_result_n1204, (int32_t)hxc_l_tmp_load_result_n1205, (int32_t)360, (int32_t)54, hxc_l_this1_h6533ab967d3f);
      }
      struct Color hxc_l_this1_h65e5e0b6262a = (struct Color){ .r = 242, .g = 249, .b = 245, .a = 255 };
      struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_tmp_load_result_n1208 = hxc_l_uiCatalog;
      hxc_l_gc_roots[222] = (const void *)hxc_l_tmp_load_result_n1208;
      if (hxc_l_tmp_load_result_n1208 == NULL)
      {
        abort();
      }
      hxc_string hxc_l_tmp_instance_call_result_n1210 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_tmp_load_result_n1208, hxc_l_locale, 26);
      hxc_l_tmp_cstring_ref_owner_owner_n385 = hxc_l_tmp_instance_call_result_n1210;
      hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n1212 = HXC_BORROWED_CSTRING_INITIALIZER;
      if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n385, &hxc_l_tmp_string_borrow_cstring_result_n1212) != HXC_STATUS_OK)
      {
        abort();
      }
      int32_t hxc_l_tmp_load_result_n1213 = hxc_l_left_h29638ea69142;
      int32_t hxc_l_tmp_load_result_n1214 = hxc_l_top_h0816c0ac00dc;
      DrawText(hxc_l_tmp_string_borrow_cstring_result_n1212.data, (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n1213, 28), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n1214, 16), (int32_t)21, hxc_l_this1_h65e5e0b6262a);
      int32_t hxc_l_top_hc72515ffac6b = hxc_i32_add_wrapping(hxc_l_firstTop, 136);
      int32_t hxc_l_left_hb1b960ee6983 = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_l_width_h0d6a9e51bd05, (double)2)), 180);
      struct Color hxc_l_this1_h786009724bf2 = (struct Color){ .r = 6, .g = 26, .b = 36, .a = 218 };
      int32_t hxc_l_tmp_load_result_n1219 = hxc_l_left_hb1b960ee6983;
      int32_t hxc_l_tmp_load_result_n1220 = hxc_l_top_hc72515ffac6b;
      DrawRectangle((int32_t)hxc_l_tmp_load_result_n1219, (int32_t)hxc_l_tmp_load_result_n1220, (int32_t)360, (int32_t)54, hxc_l_this1_h786009724bf2);
      struct Color hxc_l_this1_h7fd8cce904ea = (struct Color){ .r = 92, .g = 194, .b = 188, .a = 255 };
      int32_t hxc_l_tmp_load_result_n1223 = hxc_l_left_hb1b960ee6983;
      int32_t hxc_l_tmp_load_result_n1224 = hxc_l_top_hc72515ffac6b;
      DrawRectangleLines((int32_t)hxc_l_tmp_load_result_n1223, (int32_t)hxc_l_tmp_load_result_n1224, (int32_t)360, (int32_t)54, hxc_l_this1_h7fd8cce904ea);
      struct Color hxc_l_this1_h43976a9091aa = (struct Color){ .r = 242, .g = 249, .b = 245, .a = 255 };
      struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_tmp_load_result_n1227 = hxc_l_uiCatalog;
      hxc_l_gc_roots[223] = (const void *)hxc_l_tmp_load_result_n1227;
      if (hxc_l_tmp_load_result_n1227 == NULL)
      {
        abort();
      }
      hxc_string hxc_l_tmp_instance_call_result_n1229 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_tmp_load_result_n1227, hxc_l_locale, 28);
      hxc_l_tmp_cstring_ref_owner_owner_n391 = hxc_l_tmp_instance_call_result_n1229;
      hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n1231 = HXC_BORROWED_CSTRING_INITIALIZER;
      if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n391, &hxc_l_tmp_string_borrow_cstring_result_n1231) != HXC_STATUS_OK)
      {
        abort();
      }
      int32_t hxc_l_tmp_load_result_n1232 = hxc_l_left_hb1b960ee6983;
      int32_t hxc_l_tmp_load_result_n1233 = hxc_l_top_hc72515ffac6b;
      DrawText(hxc_l_tmp_string_borrow_cstring_result_n1231.data, (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n1232, 28), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n1233, 16), (int32_t)21, hxc_l_this1_h43976a9091aa);
      int32_t hxc_l_x_h124422bda379 = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_l_width_h0d6a9e51bd05, (double)2)), 285);
      struct Color hxc_l_this1_h986025d66f8b = (struct Color){ .r = 229, .g = 241, .b = 235, .a = 255 };
      struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_tmp_load_result_n1237 = hxc_l_uiCatalog;
      hxc_l_gc_roots[224] = (const void *)hxc_l_tmp_load_result_n1237;
      if (hxc_l_tmp_load_result_n1237 == NULL)
      {
        abort();
      }
      hxc_string hxc_l_tmp_instance_call_result_n1239 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_tmp_load_result_n1237, hxc_l_locale, 29);
      hxc_l_tmp_cstring_ref_owner_owner_n394 = hxc_l_tmp_instance_call_result_n1239;
      hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n1241 = HXC_BORROWED_CSTRING_INITIALIZER;
      if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n394, &hxc_l_tmp_string_borrow_cstring_result_n1241) != HXC_STATUS_OK)
      {
        abort();
      }
      int32_t hxc_l_tmp_load_result_n1242 = hxc_l_x_h124422bda379;
      int32_t hxc_l_tmp_load_result_n1243 = hxc_l_height_h4ce7f7275f62;
      DrawText(hxc_l_tmp_string_borrow_cstring_result_n1241.data, (int32_t)hxc_l_tmp_load_result_n1242, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n1243, 58), (int32_t)16, hxc_l_this1_h986025d66f8b);
      int32_t hxc_l_tmp_load_result_n1245 = hxc_l_selectedMode;
      bool hxc_l_tmp_short_circuit_result_n395 = hxc_l_tmp_load_result_n1245 == 1;
      if (hxc_l_tmp_load_result_n1245 == 1)
      {
        hxc_string hxc_l_tmp_load_result_n1246 = hxc_l_adventureTagline;
        if (hxc_l_tmp_load_result_n1246.data == NULL)
        {
          abort();
        }
        int32_t hxc_l_tmp_string_length_result_n1247;
        if (hxc_string_haxe_length(hxc_l_tmp_load_result_n1246, &hxc_l_tmp_string_length_result_n1247) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_l_tmp_short_circuit_result_n395 = hxc_l_tmp_string_length_result_n1247 > 0;
      }
      if (hxc_l_tmp_short_circuit_result_n395)
      {
        int32_t hxc_l_x_h5b0c3c0056ea = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_l_width_h0d6a9e51bd05, (double)2)), 230);
        struct Color hxc_l_this1_h473ab2ceeeff = (struct Color){ .r = 255, .g = 205, .b = 91, .a = 255 };
        hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n1252 = HXC_BORROWED_CSTRING_INITIALIZER;
        if (hxc_string_borrow_cstring(&hxc_l_adventureTagline, &hxc_l_tmp_string_borrow_cstring_result_n1252) != HXC_STATUS_OK)
        {
          abort();
        }
        int32_t hxc_l_tmp_load_result_n1253 = hxc_l_x_h5b0c3c0056ea;
        int32_t hxc_l_tmp_load_result_n1254 = hxc_l_firstTop;
        DrawText(hxc_l_tmp_string_borrow_cstring_result_n1252.data, (int32_t)hxc_l_tmp_load_result_n1253, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n1254, 36), (int32_t)17, hxc_l_this1_h473ab2ceeeff);
      }
      if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n394) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n391) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n385) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n377) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_adventureTagline) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n347) != HXC_STATUS_OK)
      {
        abort();
      }
    }
    else
    {
      if (hxc_l_onCampaignSelect)
      {
        hxc_l_gc_roots[225] = (const void *)hxc_l_campaign;
        struct hxc_caxecraft_content_CampaignManifest *hxc_l_selectedCampaign_hc4ad8b465e2a = hxc_l_campaign;
        hxc_l_gc_roots[226] = (const void *)hxc_l_selectedCampaign_hc4ad8b465e2a;
        hxc_l_gc_roots[227] = (const void *)NULL;
        if (hxc_l_selectedCampaign_hc4ad8b465e2a == NULL)
        {
          enum hxc_caxecraft_app_AppScreen hxc_l_tmp_call_result_n1260 = hxc_caxecraft_app_AppScreen_closeCampaignSelection(hxc_l_screen);
          hxc_l_screen = hxc_l_tmp_call_result_n1260;
        }
        else
        {
          hxc_string hxc_l_locale1_hfa244cde12a1 = { 0 };
          switch (hxc_l_locale) {
            case 0:
              {
                hxc_l_locale1_hfa244cde12a1 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
                if (hxc_string_retain(hxc_l_locale1_hfa244cde12a1) != HXC_STATUS_OK)
                {
                  abort();
                }
                break;
              }
            case 1:
              {
                hxc_l_locale1_hfa244cde12a1 = (hxc_string){ (const uint8_t *)"es-mx", 5, true, NULL };
                if (hxc_string_retain(hxc_l_locale1_hfa244cde12a1) != HXC_STATUS_OK)
                {
                  abort();
                }
                break;
              }
            default:
              {
                hxc_l_locale1_hfa244cde12a1 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
                if (hxc_string_retain(hxc_l_locale1_hfa244cde12a1) != HXC_STATUS_OK)
                {
                  abort();
                }
                break;
              }
          }
          hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n1262 = hxc_l_locale1_hfa244cde12a1;
          hxc_l_tmp_managed_flow_owner_n400 = hxc_l_tmp_managed_flow_carrier_move_result_n1262;
          struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1263 = hxc_l_levelView_ha9fa307295b9;
          hxc_l_gc_roots[228] = (const void *)hxc_l_tmp_load_result_n1263;
          if (hxc_l_tmp_load_result_n1263 == NULL)
          {
            abort();
          }
          struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_class_field_load_result_n1264 = (*hxc_l_tmp_load_result_n1263).hxc_levelPresentation;
          hxc_l_gc_roots[229] = (const void *)hxc_l_tmp_class_field_load_result_n1264;
          struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_this = hxc_l_tmp_class_field_load_result_n1264;
          struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_load_result_n1265 = hxc_l_this;
          hxc_l_gc_roots[230] = (const void *)hxc_l_tmp_load_result_n1265;
          if (hxc_l_tmp_load_result_n1265 == NULL)
          {
            abort();
          }
          struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_load_result_n1266 = hxc_l_this;
          hxc_l_gc_roots[231] = (const void *)hxc_l_tmp_load_result_n1266;
          if (hxc_l_tmp_load_result_n1266 == NULL)
          {
            abort();
          }
          struct hxc_caxecraft_scenario_ScenarioText hxc_l_tmp_class_field_load_result_n1267 = (*hxc_l_tmp_load_result_n1266).hxc_title;
          hxc_string hxc_l_tmp_instance_call_result_n1269 = hxc_caxecraft_content_RuntimeLevelPresentation_resolve(hxc_l_tmp_load_result_n1265, hxc_l_tmp_class_field_load_result_n1267, hxc_l_tmp_managed_flow_owner_n400);
          hxc_l_tmp = hxc_l_tmp_instance_call_result_n1269;
          hxc_string hxc_l_locale1_h5bb4753074c7 = { 0 };
          switch (hxc_l_locale) {
            case 0:
              {
                hxc_l_locale1_h5bb4753074c7 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
                if (hxc_string_retain(hxc_l_locale1_h5bb4753074c7) != HXC_STATUS_OK)
                {
                  abort();
                }
                break;
              }
            case 1:
              {
                hxc_l_locale1_h5bb4753074c7 = (hxc_string){ (const uint8_t *)"es-mx", 5, true, NULL };
                if (hxc_string_retain(hxc_l_locale1_h5bb4753074c7) != HXC_STATUS_OK)
                {
                  abort();
                }
                break;
              }
            default:
              {
                hxc_l_locale1_h5bb4753074c7 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
                if (hxc_string_retain(hxc_l_locale1_h5bb4753074c7) != HXC_STATUS_OK)
                {
                  abort();
                }
                break;
              }
          }
          hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n1271 = hxc_l_locale1_h5bb4753074c7;
          hxc_l_tmp_managed_flow_owner_n404 = hxc_l_tmp_managed_flow_carrier_move_result_n1271;
          struct Texture hxc_l_tmp_load_result_n1272 = hxc_l_titleTexture;
          bool hxc_l_tmp_load_result_n1273 = hxc_l_titleTextureReady;
          struct Texture hxc_l_tmp_load_result_n1274 = hxc_l_wordmarkTexture;
          bool hxc_l_tmp_load_result_n1275 = hxc_l_wordmarkTextureReady;
          struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n1276 = hxc_l_selectedCampaign_hc4ad8b465e2a;
          hxc_l_gc_roots[232] = (const void *)hxc_l_tmp_load_result_n1276;
          int32_t hxc_l_tmp_load_result_n1277 = hxc_l_locale;
          struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_tmp_load_result_n1278 = hxc_l_uiCatalog;
          hxc_l_gc_roots[233] = (const void *)hxc_l_tmp_load_result_n1278;
          int32_t hxc_l_tmp_load_result_n1279 = hxc_l_selectedCampaignLevelIndex;
          hxc_string hxc_l_tmp_load_result_n1280 = hxc_l_tmp;
          struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1281 = hxc_l_levelView_ha9fa307295b9;
          hxc_l_gc_roots[234] = (const void *)hxc_l_tmp_load_result_n1281;
          if (hxc_l_tmp_load_result_n1281 == NULL)
          {
            abort();
          }
          struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_class_field_load_result_n1282 = (*hxc_l_tmp_load_result_n1281).hxc_levelPresentation;
          hxc_l_gc_roots[235] = (const void *)hxc_l_tmp_class_field_load_result_n1282;
          if (hxc_l_tmp_class_field_load_result_n1282 == NULL)
          {
            abort();
          }
          hxc_string hxc_l_tmp_instance_call_result_n1284 = hxc_caxecraft_content_RuntimeLevelPresentation_message(hxc_l_tmp_class_field_load_result_n1282, (hxc_string){ (const uint8_t *)"adventure_tagline", 17, true, NULL }, hxc_l_tmp_managed_flow_owner_n404);
          hxc_l_tmp_static_call_argument_9_owner_n405 = hxc_l_tmp_instance_call_result_n1284;
          hxc_caxecraft_app_CampaignMenu_draw(hxc_l_tmp_load_result_n1272, hxc_l_tmp_load_result_n1273, hxc_l_tmp_load_result_n1274, hxc_l_tmp_load_result_n1275, hxc_l_tmp_load_result_n1276, hxc_l_tmp_load_result_n1277, hxc_l_tmp_load_result_n1278, hxc_l_tmp_load_result_n1279, hxc_l_tmp_load_result_n1280, hxc_l_tmp_static_call_argument_9_owner_n405);
          if (hxc_string_release(&hxc_l_tmp_static_call_argument_9_owner_n405) != HXC_STATUS_OK)
          {
            abort();
          }
          if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n404) != HXC_STATUS_OK)
          {
            abort();
          }
          if (hxc_string_release(&hxc_l_tmp) != HXC_STATUS_OK)
          {
            abort();
          }
          if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n400) != HXC_STATUS_OK)
          {
            abort();
          }
        }
      }
      else
      {
        if (hxc_l_onEditor)
        {
          struct hxc_caxecraft_app_CaxecraftEditorScreen *hxc_l_tmp_load_result_n1287 = hxc_l_editorScreen;
          hxc_l_gc_roots[236] = (const void *)hxc_l_tmp_load_result_n1287;
          if (hxc_l_tmp_load_result_n1287 == NULL)
          {
            abort();
          }
          int32_t hxc_l_tmp_load_result_n1288 = hxc_l_locale;
          enum hxc_caxecraft_app_EditorScreenAction hxc_l_tmp_instance_call_result_n1290 = hxc_caxecraft_app_CaxecraftEditorScreen_draw(hxc_l_tmp_load_result_n1287, hxc_l_tmp_load_result_n1288, hxc_l_editorNavigationCommand);
          if (hxc_l_tmp_instance_call_result_n1290 == hxc_caxecraft_app_EditorScreenAction_ReturnToTitle)
          {
            enum hxc_caxecraft_app_AppScreen hxc_l_tmp_call_result_n1293 = hxc_caxecraft_app_AppScreen_closeEditor(hxc_l_screen);
            hxc_l_screen = hxc_l_tmp_call_result_n1293;
          }
        }
        else
        {
          ClearBackground((struct Color){ .r = 126, .g = 190, .b = 201, .a = 255 });
          int32_t hxc_l_tmp_native_call_result_n1295 = GetScreenWidth();
          int32_t hxc_l_sunX = hxc_i32_subtract_wrapping((int32_t)hxc_l_tmp_native_call_result_n1295, 260);
          float hxc_l_radius_hcca2537a8c47 = (float)42.0;
          struct Color hxc_l_this1_h3e74198a2b48 = (struct Color){ .r = 255, .g = 226, .b = 136, .a = 255 };
          int32_t hxc_l_tmp_load_result_n1297 = hxc_l_sunX;
          float hxc_l_tmp_load_result_n1298 = hxc_l_radius_hcca2537a8c47;
          DrawCircle((int32_t)hxc_l_tmp_load_result_n1297, (int32_t)86, hxc_l_tmp_load_result_n1298, hxc_l_this1_h3e74198a2b48);
          float hxc_l_radius_hd38df5b7bfe3 = (float)30.0;
          struct Color hxc_l_this1_hd89aa0fc2675 = (struct Color){ .r = 255, .g = 246, .b = 205, .a = 255 };
          int32_t hxc_l_tmp_load_result_n1301 = hxc_l_sunX;
          float hxc_l_tmp_load_result_n1302 = hxc_l_radius_hd38df5b7bfe3;
          DrawCircle((int32_t)hxc_l_tmp_load_result_n1301, (int32_t)86, hxc_l_tmp_load_result_n1302, hxc_l_this1_hd89aa0fc2675);
          BeginMode3D(hxc_l_camera);
          if (hxc_l_self == NULL)
          {
            abort();
          }
          struct hxc_caxecraft_app_TerrainRenderer *hxc_l_tmp_owned_class_field_address_n1305 = &(*hxc_l_self).hxc_terrainRenderer;
          if (hxc_l_tmp_owned_class_field_address_n1305 == NULL)
          {
            abort();
          }
          struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1306 = hxc_l_session;
          hxc_l_gc_roots[237] = (const void *)hxc_l_tmp_load_result_n1306;
          if (hxc_l_tmp_load_result_n1306 == NULL)
          {
            abort();
          }
          size_t hxc_l_tmp_length_n1308;
          const uint8_t *hxc_l_tmp_instance_call_result_n1307 = hxc_caxecraft_domain_GameSession_worldView(hxc_l_tmp_load_result_n1306, &hxc_l_tmp_length_n1308);
          struct Texture hxc_l_tmp_load_result_n1309 = hxc_l_terrainTexture;
          bool hxc_l_tmp_load_result_n1310 = hxc_l_terrainTextureReady;
          struct Texture hxc_l_tmp_load_result_n1311 = hxc_l_adventureTerrainTexture;
          bool hxc_l_tmp_load_result_n1312 = hxc_l_adventureTerrainTextureReady;
          double hxc_l_tmp_record_field_load_result_n1313 = hxc_l_renderPosition.hxc_x;
          struct hxc_caxecraft_app_TerrainRenderCounters hxc_l_tmp_instance_call_result_n1315 = hxc_caxecraft_app_TerrainRenderer_draw(hxc_l_tmp_owned_class_field_address_n1305, hxc_l_tmp_instance_call_result_n1307, hxc_l_tmp_length_n1308, hxc_l_tmp_load_result_n1309, hxc_l_tmp_load_result_n1310, hxc_l_tmp_load_result_n1311, hxc_l_tmp_load_result_n1312, hxc_l_tmp_record_field_load_result_n1313, hxc_l_renderPosition.hxc_z);
          struct hxc_caxecraft_app_TerrainRenderCounters hxc_l_renderCounters = hxc_l_tmp_instance_call_result_n1315;
          struct hxc_caxecraft_app_WaterRenderer *hxc_l_tmp_owned_class_field_address_n1316 = &(*hxc_l_self).hxc_waterRenderer;
          if (hxc_l_tmp_owned_class_field_address_n1316 == NULL)
          {
            abort();
          }
          struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1317 = hxc_l_session;
          hxc_l_gc_roots[238] = (const void *)hxc_l_tmp_load_result_n1317;
          if (hxc_l_tmp_load_result_n1317 == NULL)
          {
            abort();
          }
          size_t hxc_l_tmp_length_n1319;
          const uint8_t *hxc_l_tmp_instance_call_result_n1318 = hxc_caxecraft_domain_GameSession_worldView(hxc_l_tmp_load_result_n1317, &hxc_l_tmp_length_n1319);
          struct Texture hxc_l_tmp_load_result_n1320 = hxc_l_terrainTexture;
          bool hxc_l_tmp_load_result_n1321 = hxc_l_terrainTextureReady;
          struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1322 = hxc_l_levelView_ha9fa307295b9;
          hxc_l_gc_roots[239] = (const void *)hxc_l_tmp_load_result_n1322;
          if (hxc_l_tmp_load_result_n1322 == NULL)
          {
            abort();
          }
          int32_t hxc_l_tmp_class_field_load_result_n1323 = (*hxc_l_tmp_load_result_n1322).hxc_waterCell;
          double hxc_l_tmp_load_result_n1324 = hxc_l_eyeX;
          double hxc_l_tmp_load_result_n1325 = hxc_l_eyeY;
          struct hxc_caxecraft_app_WaterRenderCounters hxc_l_tmp_instance_call_result_n1327 = hxc_caxecraft_app_WaterRenderer_draw(hxc_l_tmp_owned_class_field_address_n1316, hxc_l_tmp_instance_call_result_n1318, hxc_l_tmp_length_n1319, hxc_l_tmp_load_result_n1320, hxc_l_tmp_load_result_n1321, hxc_l_tmp_class_field_load_result_n1323, hxc_l_tmp_load_result_n1324, hxc_l_tmp_load_result_n1325, hxc_l_eyeZ);
          struct hxc_caxecraft_app_WaterRenderCounters hxc_l_waterCounters = hxc_l_tmp_instance_call_result_n1327;
          int32_t hxc_l_tmp_record_field_load_result_n1328 = hxc_l_renderCounters.hxc_visible;
          int32_t hxc_l_totalVisible = hxc_i32_add_wrapping(hxc_l_tmp_record_field_load_result_n1328, hxc_l_waterCounters.hxc_visible);
          int32_t hxc_l_tmp_record_field_load_result_n1330 = hxc_l_renderCounters.hxc_drawCalls;
          int32_t hxc_l_totalDrawCalls = hxc_i32_add_wrapping(hxc_l_tmp_record_field_load_result_n1330, hxc_l_waterCounters.hxc_drawCalls);
          struct Camera3D hxc_l_tmp_load_result_n1332 = hxc_l_camera;
          struct Texture hxc_l_tmp_load_result_n1333 = hxc_l_entityTexture;
          bool hxc_l_tmp_load_result_n1334 = hxc_l_entityTextureReady;
          struct hxc_array_ref *hxc_l_tmp_load_result_n1335 = hxc_l_dialogueActors;
          struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1336 = hxc_l_levelView_ha9fa307295b9;
          hxc_l_gc_roots[240] = (const void *)hxc_l_tmp_load_result_n1336;
          struct hxc_caxecraft_domain_Character hxc_l_tmp_load_result_n1337 = hxc_l_enemyActor;
          struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1338 = hxc_l_levelView_ha9fa307295b9;
          hxc_l_gc_roots[241] = (const void *)hxc_l_tmp_load_result_n1338;
          if (hxc_l_tmp_load_result_n1338 == NULL)
          {
            abort();
          }
          int32_t hxc_l_tmp_class_field_load_result_n1339 = (*hxc_l_tmp_load_result_n1338).hxc_enemyCell;
          enum hxc_caxecraft_domain_ActorControllerPhase hxc_l_tmp_record_field_load_result_n1340 = hxc_l_enemyPhase_h70382f3bdc66.hxc_phase;
          hxc_caxecraft_app_CaxecraftApp_drawActors(hxc_l_tmp_load_result_n1332, hxc_l_tmp_load_result_n1333, hxc_l_tmp_load_result_n1334, hxc_l_tmp_load_result_n1335, hxc_l_tmp_load_result_n1336, hxc_l_tmp_load_result_n1337, hxc_l_tmp_class_field_load_result_n1339, hxc_l_tmp_record_field_load_result_n1340, hxc_l_berryDrop);
          struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_tmp_load_result_n1342 = hxc_l_contentRegistry;
          hxc_l_gc_roots[242] = (const void *)hxc_l_tmp_load_result_n1342;
          struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1343 = hxc_l_session;
          hxc_l_gc_roots[243] = (const void *)hxc_l_tmp_load_result_n1343;
          struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1344 = hxc_l_levelView_ha9fa307295b9;
          hxc_l_gc_roots[244] = (const void *)hxc_l_tmp_load_result_n1344;
          struct Camera3D hxc_l_tmp_load_result_n1345 = hxc_l_camera;
          struct Texture hxc_l_tmp_load_result_n1346 = hxc_l_entityTexture;
          bool hxc_l_tmp_load_result_n1347 = hxc_l_entityTextureReady;
          struct Texture hxc_l_tmp_load_result_n1348 = hxc_l_itemTexture;
          bool hxc_l_tmp_load_result_n1349 = hxc_l_itemTextureReady;
          struct Texture hxc_l_tmp_load_result_n1350 = hxc_l_adventureItemTexture;
          bool hxc_l_tmp_load_result_n1351 = hxc_l_adventureItemTextureReady;
          struct Texture hxc_l_tmp_load_result_n1352 = hxc_l_terrainTexture;
          hxc_caxecraft_app_StatefulObjectRenderer_drawStatefulObjects(hxc_l_tmp_load_result_n1342, hxc_l_tmp_load_result_n1343, hxc_l_tmp_load_result_n1344, hxc_l_tmp_load_result_n1345, hxc_l_tmp_load_result_n1346, hxc_l_tmp_load_result_n1347, hxc_l_tmp_load_result_n1348, hxc_l_tmp_load_result_n1349, hxc_l_tmp_load_result_n1350, hxc_l_tmp_load_result_n1351, hxc_l_tmp_load_result_n1352, hxc_l_terrainTextureReady);
          struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_tmp_load_result_n1354 = hxc_l_contentRegistry;
          hxc_l_gc_roots[245] = (const void *)hxc_l_tmp_load_result_n1354;
          struct Camera3D hxc_l_tmp_load_result_n1355 = hxc_l_camera;
          struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1356 = hxc_l_session;
          hxc_l_gc_roots[246] = (const void *)hxc_l_tmp_load_result_n1356;
          if (hxc_l_tmp_load_result_n1356 == NULL)
          {
            abort();
          }
          size_t hxc_l_tmp_length_n1358;
          const int32_t *hxc_l_tmp_instance_call_result_n1357 = hxc_caxecraft_domain_GameSession_authoredItemsView(hxc_l_tmp_load_result_n1356, &hxc_l_tmp_length_n1358);
          struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1359 = hxc_l_levelView_ha9fa307295b9;
          hxc_l_gc_roots[247] = (const void *)hxc_l_tmp_load_result_n1359;
          struct Texture hxc_l_tmp_load_result_n1360 = hxc_l_itemTexture;
          bool hxc_l_tmp_load_result_n1361 = hxc_l_itemTextureReady;
          struct Texture hxc_l_tmp_load_result_n1362 = hxc_l_adventureItemTexture;
          hxc_caxecraft_app_AuthoredItemRenderer_drawWorldItems(hxc_l_tmp_load_result_n1354, hxc_l_tmp_load_result_n1355, hxc_l_tmp_instance_call_result_n1357, hxc_l_tmp_length_n1358, hxc_l_tmp_load_result_n1359, hxc_l_tmp_load_result_n1360, hxc_l_tmp_load_result_n1361, hxc_l_tmp_load_result_n1362, hxc_l_adventureItemTextureReady);
          if (hxc_l_hit.hxc_hit)
          {
            double hxc_l_y = (double)hxc_l_hit.hxc_cellY + 0.5;
            double hxc_l_z = (double)hxc_l_hit.hxc_cellZ + 0.5;
            int32_t hxc_l_tmp_record_field_load_result_n1367 = hxc_l_hit.hxc_cellX;
            double hxc_l_tmp_load_result_n1368 = hxc_l_y;
            DrawCubeWires((struct Vector3){ .x = (float)((double)hxc_l_tmp_record_field_load_result_n1367 + 0.5), .y = (float)hxc_l_tmp_load_result_n1368, .z = (float)hxc_l_z }, (float)1.04, (float)1.04, (float)1.04, (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 });
          }
          EndMode3D();
          if (hxc_l_cameraWaterBlend > 0.0)
          {
            int32_t hxc_l_overlayAlpha = hxc_f64_to_i32_saturating(105.0 * hxc_l_cameraWaterBlend);
            int32_t hxc_l_tmp_native_call_result_n1374 = GetScreenWidth();
            int32_t hxc_l_width_h278cf360e1fe = (int32_t)hxc_l_tmp_native_call_result_n1374;
            int32_t hxc_l_tmp_native_call_result_n1375 = GetScreenHeight();
            int32_t hxc_l_height_hdb1c38ad6004 = (int32_t)hxc_l_tmp_native_call_result_n1375;
            uint8_t hxc_l_red = (uint8_t)31;
            uint8_t hxc_l_green = (uint8_t)115;
            uint8_t hxc_l_blue = (uint8_t)154;
            int32_t hxc_l_tmp_load_result_n1376 = hxc_l_overlayAlpha;
            int32_t hxc_l_tmp_conditional_result_n424 = 0;
            if (hxc_l_tmp_load_result_n1376 < 0)
            {
              hxc_l_tmp_conditional_result_n424 = 0;
            }
            else
            {
              int32_t hxc_l_tmp_load_result_n1377 = hxc_l_overlayAlpha;
              int32_t hxc_l_tmp_conditional_result_n425 = 0;
              if (hxc_l_tmp_load_result_n1377 > 255)
              {
                hxc_l_tmp_conditional_result_n425 = 255;
              }
              else
              {
                hxc_l_tmp_conditional_result_n425 = hxc_l_overlayAlpha;
              }
              hxc_l_tmp_conditional_result_n424 = hxc_l_tmp_conditional_result_n425;
            }
            uint8_t hxc_l_alpha = (uint8_t)hxc_l_tmp_conditional_result_n424;
            uint8_t hxc_l_tmp_load_result_n1381 = hxc_l_red;
            uint8_t hxc_l_tmp_load_result_n1382 = hxc_l_green;
            uint8_t hxc_l_tmp_load_result_n1383 = hxc_l_blue;
            struct Color hxc_l_this1_h97073453f54d = (struct Color){ .r = hxc_l_tmp_load_result_n1381, .g = hxc_l_tmp_load_result_n1382, .b = hxc_l_tmp_load_result_n1383, .a = hxc_l_alpha };
            int32_t hxc_l_tmp_load_result_n1386 = hxc_l_width_h278cf360e1fe;
            int32_t hxc_l_tmp_load_result_n1387 = hxc_l_height_hdb1c38ad6004;
            DrawRectangle((int32_t)0, (int32_t)0, (int32_t)hxc_l_tmp_load_result_n1386, (int32_t)hxc_l_tmp_load_result_n1387, hxc_l_this1_h97073453f54d);
          }
          double hxc_l_tmp_record_field_load_result_n1389 = hxc_l_character.hxc_body.hxc_x;
          double hxc_l_tmp_record_field_load_result_n1390 = hxc_l_character.hxc_body.hxc_z;
          struct hxc_caxecraft_domain_VitalsState hxc_l_tmp_record_field_load_result_n1391 = hxc_l_character.hxc_vitals;
          int32_t hxc_l_tmp_load_result_n1392 = hxc_l_aquaticEquipmentCode;
          int32_t hxc_l_tmp_load_result_n1393 = hxc_l_aquaticEquipmentFrames;
          bool hxc_l_tmp_record_field_load_result_n1394 = hxc_l_character.hxc_aquatic.hxc_headSubmerged;
          int32_t hxc_l_tmp_record_field_load_result_n1395 = hxc_l_character.hxc_aquatic.hxc_breathTicks;
          struct hxc_caxecraft_app_HudCharacterView hxc_l_hudView_h9f545fec4c77 = (struct hxc_caxecraft_app_HudCharacterView){ .hxc_aquaticEquipmentCode = hxc_l_tmp_load_result_n1392, .hxc_aquaticEquipmentVisible = hxc_l_tmp_load_result_n1393 > 0, .hxc_breathTicks = hxc_l_tmp_record_field_load_result_n1395, .hxc_headSubmerged = hxc_l_tmp_record_field_load_result_n1394, .hxc_maximumBreathTicks = hxc_l_character.hxc_aquaticProfile.hxc_maximumBreathTicks, .hxc_vitals = hxc_l_tmp_record_field_load_result_n1391, .hxc_x = hxc_l_tmp_record_field_load_result_n1389, .hxc_z = hxc_l_tmp_record_field_load_result_n1390 };
          bool hxc_l_hudView1 = hxc_l_placementBlockedFrames > 0;
          bool hxc_l_hudView2 = hxc_l_strikeHitFrames > 0;
          bool hxc_l_hudView3 = hxc_l_enemyDefeatedFrames > 0;
          bool hxc_l_hudView4 = hxc_l_enemyAttackFrames > 0;
          bool hxc_l_hudView5 = hxc_l_pickupFrames > 0;
          bool hxc_l_hudView6 = hxc_l_recoveryFeedbackFrames > 0;
          enum hxc_caxecraft_domain_ActorControllerPhase hxc_l_enemyPhase_h16a08849dacc = hxc_l_enemyPhase_h70382f3bdc66.hxc_phase;
          hxc_string hxc_l_locale1_h24537e34de3b = { 0 };
          switch (hxc_l_locale) {
            case 0:
              {
                hxc_l_locale1_h24537e34de3b = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
                if (hxc_string_retain(hxc_l_locale1_h24537e34de3b) != HXC_STATUS_OK)
                {
                  abort();
                }
                break;
              }
            case 1:
              {
                hxc_l_locale1_h24537e34de3b = (hxc_string){ (const uint8_t *)"es-mx", 5, true, NULL };
                if (hxc_string_retain(hxc_l_locale1_h24537e34de3b) != HXC_STATUS_OK)
                {
                  abort();
                }
                break;
              }
            default:
              {
                hxc_l_locale1_h24537e34de3b = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
                if (hxc_string_retain(hxc_l_locale1_h24537e34de3b) != HXC_STATUS_OK)
                {
                  abort();
                }
                break;
              }
          }
          hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n1406 = hxc_l_locale1_h24537e34de3b;
          hxc_l_tmp_managed_flow_owner_n436 = hxc_l_tmp_managed_flow_carrier_move_result_n1406;
          struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1407 = hxc_l_levelView_ha9fa307295b9;
          hxc_l_gc_roots[248] = (const void *)hxc_l_tmp_load_result_n1407;
          if (hxc_l_tmp_load_result_n1407 == NULL)
          {
            abort();
          }
          struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_class_field_load_result_n1408 = (*hxc_l_tmp_load_result_n1407).hxc_levelPresentation;
          hxc_l_gc_roots[249] = (const void *)hxc_l_tmp_class_field_load_result_n1408;
          if (hxc_l_tmp_class_field_load_result_n1408 == NULL)
          {
            abort();
          }
          hxc_string hxc_l_tmp_load_result_n1409 = hxc_l_currentObjectiveId;
          hxc_string hxc_l_tmp_instance_call_result_n1411 = hxc_caxecraft_content_RuntimeLevelPresentation_objectiveTitle(hxc_l_tmp_class_field_load_result_n1408, hxc_l_tmp_load_result_n1409, hxc_l_tmp_managed_flow_owner_n436);
          hxc_l_hudView7 = hxc_l_tmp_instance_call_result_n1411;
          hxc_string hxc_l_hudView8 = { 0 };
          if (hxc_l_latestJournalId.data == NULL)
          {
            hxc_l_hudView8 = (hxc_string){ (const uint8_t *)"", 0, true, NULL };
            if (hxc_string_retain(hxc_l_hudView8) != HXC_STATUS_OK)
            {
              abort();
            }
          }
          else
          {
            struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1413 = hxc_l_levelView_ha9fa307295b9;
            hxc_l_gc_roots[250] = (const void *)hxc_l_tmp_load_result_n1413;
            if (hxc_l_tmp_load_result_n1413 == NULL)
            {
              abort();
            }
            struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_class_field_load_result_n1414 = (*hxc_l_tmp_load_result_n1413).hxc_levelPresentation;
            hxc_l_gc_roots[251] = (const void *)hxc_l_tmp_class_field_load_result_n1414;
            struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_levelView_h6f85225a0bc0 = hxc_l_tmp_class_field_load_result_n1414;
            hxc_string hxc_l_hudView_h1ce7afe10661 = { 0 };
            switch (hxc_l_locale) {
              case 0:
                {
                  hxc_l_hudView_h1ce7afe10661 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
                  if (hxc_string_retain(hxc_l_hudView_h1ce7afe10661) != HXC_STATUS_OK)
                  {
                    abort();
                  }
                  break;
                }
              case 1:
                {
                  hxc_l_hudView_h1ce7afe10661 = (hxc_string){ (const uint8_t *)"es-mx", 5, true, NULL };
                  if (hxc_string_retain(hxc_l_hudView_h1ce7afe10661) != HXC_STATUS_OK)
                  {
                    abort();
                  }
                  break;
                }
              default:
                {
                  hxc_l_hudView_h1ce7afe10661 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
                  if (hxc_string_retain(hxc_l_hudView_h1ce7afe10661) != HXC_STATUS_OK)
                  {
                    abort();
                  }
                  break;
                }
            }
            hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n1416 = hxc_l_hudView_h1ce7afe10661;
            hxc_l_tmp_managed_flow_owner_n441 = hxc_l_tmp_managed_flow_carrier_move_result_n1416;
            struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_load_result_n1417 = hxc_l_levelView_h6f85225a0bc0;
            hxc_l_gc_roots[252] = (const void *)hxc_l_tmp_load_result_n1417;
            if (hxc_l_tmp_load_result_n1417 == NULL)
            {
              abort();
            }
            hxc_string hxc_l_tmp_load_result_n1418 = hxc_l_latestJournalId;
            hxc_string hxc_l_tmp_instance_call_result_n1420 = hxc_caxecraft_content_RuntimeLevelPresentation_journalTitle(hxc_l_tmp_load_result_n1417, hxc_l_tmp_load_result_n1418, hxc_l_tmp_managed_flow_owner_n441);
            hxc_l_hudView8 = hxc_l_tmp_instance_call_result_n1420;
            if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n441) != HXC_STATUS_OK)
            {
              abort();
            }
          }
          hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n1421 = hxc_l_hudView8;
          hxc_l_tmp_managed_flow_owner_n442 = hxc_l_tmp_managed_flow_carrier_move_result_n1421;
          hxc_string hxc_l_hudView9 = { 0 };
          if (hxc_l_latestJournalId.data == NULL)
          {
            hxc_l_hudView9 = (hxc_string){ (const uint8_t *)"", 0, true, NULL };
            if (hxc_string_retain(hxc_l_hudView9) != HXC_STATUS_OK)
            {
              abort();
            }
          }
          else
          {
            struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1423 = hxc_l_levelView_ha9fa307295b9;
            hxc_l_gc_roots[253] = (const void *)hxc_l_tmp_load_result_n1423;
            if (hxc_l_tmp_load_result_n1423 == NULL)
            {
              abort();
            }
            struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_class_field_load_result_n1424 = (*hxc_l_tmp_load_result_n1423).hxc_levelPresentation;
            hxc_l_gc_roots[254] = (const void *)hxc_l_tmp_class_field_load_result_n1424;
            struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_levelView_h3606f58639e8 = hxc_l_tmp_class_field_load_result_n1424;
            hxc_string hxc_l_hudView_h4f6632a18b09 = { 0 };
            switch (hxc_l_locale) {
              case 0:
                {
                  hxc_l_hudView_h4f6632a18b09 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
                  if (hxc_string_retain(hxc_l_hudView_h4f6632a18b09) != HXC_STATUS_OK)
                  {
                    abort();
                  }
                  break;
                }
              case 1:
                {
                  hxc_l_hudView_h4f6632a18b09 = (hxc_string){ (const uint8_t *)"es-mx", 5, true, NULL };
                  if (hxc_string_retain(hxc_l_hudView_h4f6632a18b09) != HXC_STATUS_OK)
                  {
                    abort();
                  }
                  break;
                }
              default:
                {
                  hxc_l_hudView_h4f6632a18b09 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
                  if (hxc_string_retain(hxc_l_hudView_h4f6632a18b09) != HXC_STATUS_OK)
                  {
                    abort();
                  }
                  break;
                }
            }
            hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n1426 = hxc_l_hudView_h4f6632a18b09;
            hxc_l_tmp_managed_flow_owner_n446 = hxc_l_tmp_managed_flow_carrier_move_result_n1426;
            struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_load_result_n1427 = hxc_l_levelView_h3606f58639e8;
            hxc_l_gc_roots[255] = (const void *)hxc_l_tmp_load_result_n1427;
            if (hxc_l_tmp_load_result_n1427 == NULL)
            {
              abort();
            }
            hxc_string hxc_l_tmp_load_result_n1428 = hxc_l_latestJournalId;
            hxc_string hxc_l_tmp_instance_call_result_n1430 = hxc_caxecraft_content_RuntimeLevelPresentation_journalBody(hxc_l_tmp_load_result_n1427, hxc_l_tmp_load_result_n1428, hxc_l_tmp_managed_flow_owner_n446);
            hxc_l_hudView9 = hxc_l_tmp_instance_call_result_n1430;
            if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n446) != HXC_STATUS_OK)
            {
              abort();
            }
          }
          hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n1431 = hxc_l_hudView9;
          hxc_l_tmp_managed_flow_owner_n447 = hxc_l_tmp_managed_flow_carrier_move_result_n1431;
          int32_t hxc_l_tmp_load_result_n1432 = hxc_l_totalVisible;
          int32_t hxc_l_tmp_load_result_n1433 = hxc_l_totalDrawCalls;
          int32_t hxc_l_tmp_load_result_n1434 = hxc_l_frameCount;
          int32_t hxc_l_tmp_load_result_n1435 = hxc_l_completedTicks;
          struct hxc_caxecraft_app_HudCharacterView hxc_l_tmp_load_result_n1437 = hxc_l_hudView_h9f545fec4c77;
          bool hxc_l_tmp_load_result_n1438 = hxc_l_hudView1;
          bool hxc_l_tmp_load_result_n1439 = hxc_l_hudView2;
          bool hxc_l_tmp_load_result_n1440 = hxc_l_hudView3;
          bool hxc_l_tmp_load_result_n1441 = hxc_l_hudView4;
          bool hxc_l_tmp_load_result_n1442 = hxc_l_hudView5;
          int32_t hxc_l_tmp_load_result_n1443 = hxc_l_pickupAmount;
          int32_t hxc_l_tmp_load_result_n1444 = hxc_l_inventoryFullReason;
          int32_t hxc_l_tmp_load_result_n1445 = hxc_l_recoveryFeedback;
          bool hxc_l_tmp_load_result_n1446 = hxc_l_hudView6;
          bool hxc_l_tmp_load_result_n1448 = hxc_l_paused_h8182f296b655;
          bool hxc_l_tmp_load_result_n1449 = hxc_l_captured_h599154fa3a25;
          struct hxc_caxecraft_domain_RaycastHit hxc_l_tmp_load_result_n1450 = hxc_l_hit;
          int32_t hxc_l_tmp_load_result_n1451 = hxc_l_selectedMode;
          int32_t hxc_l_tmp_load_result_n1452 = hxc_l_locale;
          struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_load_result_n1453 = hxc_l_inventory;
          hxc_string hxc_l_tmp_record_field_activeDialogue_owner_n449 = hxc_l_activeDialogue;
          if (hxc_string_retain(hxc_l_tmp_record_field_activeDialogue_owner_n449) != HXC_STATUS_OK)
          {
            abort();
          }
          hxc_string hxc_l_tmp_record_field_activeDialogue_owned_load_result_n1455 = hxc_l_tmp_record_field_activeDialogue_owner_n449;
          bool hxc_l_tmp_load_result_n1456 = hxc_l_guideInteractionAvailable;
          struct hxc_caxecraft_domain_Character hxc_l_tmp_load_result_n1457 = hxc_l_enemyActor;
          enum hxc_caxecraft_domain_ActorControllerPhase hxc_l_tmp_load_result_n1458 = hxc_l_enemyPhase_h16a08849dacc;
          hxc_string hxc_l_tmp_record_field_levelLabel_owner_n450 = hxc_l_levelLabel;
          if (hxc_string_retain(hxc_l_tmp_record_field_levelLabel_owner_n450) != HXC_STATUS_OK)
          {
            abort();
          }
          hxc_string hxc_l_tmp_record_field_levelLabel_owned_load_result_n1460 = hxc_l_tmp_record_field_levelLabel_owner_n450;
          hxc_string hxc_l_tmp_record_field_objectiveTitle_owner_n451 = hxc_l_hudView7;
          if (hxc_string_retain(hxc_l_tmp_record_field_objectiveTitle_owner_n451) != HXC_STATUS_OK)
          {
            abort();
          }
          hxc_string hxc_l_tmp_record_field_objectiveTitle_owned_load_result_n1462 = hxc_l_tmp_record_field_objectiveTitle_owner_n451;
          hxc_string hxc_l_tmp_record_field_journalTitle_owner_n452 = hxc_l_tmp_managed_flow_owner_n442;
          if (hxc_string_retain(hxc_l_tmp_record_field_journalTitle_owner_n452) != HXC_STATUS_OK)
          {
            abort();
          }
          hxc_string hxc_l_tmp_record_field_journalTitle_owned_load_result_n1464 = hxc_l_tmp_record_field_journalTitle_owner_n452;
          hxc_string hxc_l_tmp_record_field_journalBody_owner_n453 = hxc_l_tmp_managed_flow_owner_n447;
          if (hxc_string_retain(hxc_l_tmp_record_field_journalBody_owner_n453) != HXC_STATUS_OK)
          {
            abort();
          }
          hxc_string hxc_l_tmp_record_field_journalBody_owned_load_result_n1466 = hxc_l_tmp_record_field_journalBody_owner_n453;
          struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1467 = hxc_l_levelView_ha9fa307295b9;
          hxc_l_gc_roots[256] = (const void *)hxc_l_tmp_load_result_n1467;
          if (hxc_l_tmp_load_result_n1467 == NULL)
          {
            abort();
          }
          struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_class_field_load_result_n1468 = (*hxc_l_tmp_load_result_n1467).hxc_levelPresentation;
          hxc_l_gc_roots[257] = (const void *)hxc_l_tmp_class_field_load_result_n1468;
          hxc_l_gc_roots[258] = (const void *)(struct hxc_caxecraft_app_HudView){ .hxc_activeDialogue = hxc_l_tmp_record_field_activeDialogue_owned_load_result_n1455, .hxc_character = hxc_l_tmp_load_result_n1437, .hxc_enemy = hxc_l_tmp_load_result_n1457, .hxc_enemyPhase = hxc_l_tmp_load_result_n1458, .hxc_feedback = (struct hxc_caxecraft_app_HudFeedbackView){ .hxc_enemyAttacked = hxc_l_tmp_load_result_n1441, .hxc_enemyDefeated = hxc_l_tmp_load_result_n1440, .hxc_inventoryFullReason = hxc_l_tmp_load_result_n1444, .hxc_pickedUp = hxc_l_tmp_load_result_n1442, .hxc_pickupAmount = hxc_l_tmp_load_result_n1443, .hxc_placementBlocked = hxc_l_tmp_load_result_n1438, .hxc_recoveryDecision = hxc_l_tmp_load_result_n1445, .hxc_recoveryVisible = hxc_l_tmp_load_result_n1446, .hxc_strikeHit = hxc_l_tmp_load_result_n1439 }, .hxc_guideInteractionAvailable = hxc_l_tmp_load_result_n1456, .hxc_hit = hxc_l_tmp_load_result_n1450, .hxc_inventory = hxc_l_tmp_load_result_n1453, .hxc_journalBody = hxc_l_tmp_record_field_journalBody_owned_load_result_n1466, .hxc_journalTitle = hxc_l_tmp_record_field_journalTitle_owned_load_result_n1464, .hxc_levelLabel = hxc_l_tmp_record_field_levelLabel_owned_load_result_n1460, .hxc_locale = hxc_l_tmp_load_result_n1452, .hxc_metrics = (struct hxc_caxecraft_app_HudMetricsView){ .hxc_completedTicks = hxc_l_tmp_load_result_n1435, .hxc_drawCalls = hxc_l_tmp_load_result_n1433, .hxc_renderedFrames = hxc_l_tmp_load_result_n1434, .hxc_visibleBlocks = hxc_l_tmp_load_result_n1432 }, .hxc_mode = hxc_l_tmp_load_result_n1451, .hxc_objectiveTitle = hxc_l_tmp_record_field_objectiveTitle_owned_load_result_n1462, .hxc_paused = hxc_l_tmp_load_result_n1448, .hxc_pointerCaptured = hxc_l_tmp_load_result_n1449, .hxc_presentation = hxc_l_tmp_class_field_load_result_n1468 }.hxc_presentation;
          hxc_l_hudView_hbda7460f80cb = (struct hxc_caxecraft_app_HudView){ .hxc_activeDialogue = hxc_l_tmp_record_field_activeDialogue_owned_load_result_n1455, .hxc_character = hxc_l_tmp_load_result_n1437, .hxc_enemy = hxc_l_tmp_load_result_n1457, .hxc_enemyPhase = hxc_l_tmp_load_result_n1458, .hxc_feedback = (struct hxc_caxecraft_app_HudFeedbackView){ .hxc_enemyAttacked = hxc_l_tmp_load_result_n1441, .hxc_enemyDefeated = hxc_l_tmp_load_result_n1440, .hxc_inventoryFullReason = hxc_l_tmp_load_result_n1444, .hxc_pickedUp = hxc_l_tmp_load_result_n1442, .hxc_pickupAmount = hxc_l_tmp_load_result_n1443, .hxc_placementBlocked = hxc_l_tmp_load_result_n1438, .hxc_recoveryDecision = hxc_l_tmp_load_result_n1445, .hxc_recoveryVisible = hxc_l_tmp_load_result_n1446, .hxc_strikeHit = hxc_l_tmp_load_result_n1439 }, .hxc_guideInteractionAvailable = hxc_l_tmp_load_result_n1456, .hxc_hit = hxc_l_tmp_load_result_n1450, .hxc_inventory = hxc_l_tmp_load_result_n1453, .hxc_journalBody = hxc_l_tmp_record_field_journalBody_owned_load_result_n1466, .hxc_journalTitle = hxc_l_tmp_record_field_journalTitle_owned_load_result_n1464, .hxc_levelLabel = hxc_l_tmp_record_field_levelLabel_owned_load_result_n1460, .hxc_locale = hxc_l_tmp_load_result_n1452, .hxc_metrics = (struct hxc_caxecraft_app_HudMetricsView){ .hxc_completedTicks = hxc_l_tmp_load_result_n1435, .hxc_drawCalls = hxc_l_tmp_load_result_n1433, .hxc_renderedFrames = hxc_l_tmp_load_result_n1434, .hxc_visibleBlocks = hxc_l_tmp_load_result_n1432 }, .hxc_mode = hxc_l_tmp_load_result_n1451, .hxc_objectiveTitle = hxc_l_tmp_record_field_objectiveTitle_owned_load_result_n1462, .hxc_paused = hxc_l_tmp_load_result_n1448, .hxc_pointerCaptured = hxc_l_tmp_load_result_n1449, .hxc_presentation = hxc_l_tmp_class_field_load_result_n1468 };
          struct hxc_caxecraft_app_HudView hxc_l_tmp_load_result_n1470 = hxc_l_hudView_hbda7460f80cb;
          hxc_l_gc_roots[259] = (const void *)hxc_l_tmp_load_result_n1470.hxc_presentation;
          struct hxc_caxecraft_app_HudResources hxc_l_tmp_load_result_n1471 = hxc_l_hudResources;
          struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_tmp_load_result_n1472 = hxc_l_contentRegistry;
          hxc_l_gc_roots[260] = (const void *)hxc_l_tmp_load_result_n1472;
          hxc_l_gc_roots[261] = (const void *)hxc_l_uiCatalog;
          hxc_caxecraft_app_CaxecraftApp_drawHud(hxc_l_tmp_load_result_n1470, hxc_l_tmp_load_result_n1471, hxc_l_tmp_load_result_n1472, hxc_l_uiCatalog);
          hxc_record_8205a8e1_destroy(&hxc_l_hudView_hbda7460f80cb);
          if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n447) != HXC_STATUS_OK)
          {
            abort();
          }
          if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n442) != HXC_STATUS_OK)
          {
            abort();
          }
          if (hxc_string_release(&hxc_l_hudView7) != HXC_STATUS_OK)
          {
            abort();
          }
          if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n436) != HXC_STATUS_OK)
          {
            abort();
          }
        }
      }
    }
    EndDrawing();
    hxc_l_frameCount = hxc_i32_add_wrapping(hxc_l_frameCount, 1);
    if (hxc_array_ref_release(hxc_l_dialogueActors) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_presentationActorPhases) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_enum_2c0e695d_destroy(&hxc_l_tmp_static_call_argument_0_owner_n335);
  }
  EnableCursor();
  if (hxc_l_adventureTerrainTextureReady)
  {
    UnloadTexture(hxc_l_adventureTerrainTexture);
  }
  if (hxc_l_terrainTextureReady)
  {
    UnloadTexture(hxc_l_terrainTexture);
  }
  if (hxc_l_entityTextureReady)
  {
    UnloadTexture(hxc_l_entityTexture);
  }
  if (hxc_l_adventureItemTextureReady)
  {
    UnloadTexture(hxc_l_adventureItemTexture);
  }
  if (hxc_l_itemTextureReady)
  {
    UnloadTexture(hxc_l_itemTexture);
  }
  if (hxc_l_hudTextureReady)
  {
    UnloadTexture(hxc_l_hudTexture);
  }
  if (hxc_l_wordmarkTextureReady)
  {
    UnloadTexture(hxc_l_wordmarkTexture);
  }
  if (hxc_l_titleTextureReady)
  {
    UnloadTexture(hxc_l_titleTexture);
  }
  CloseWindow();
  if (hxc_array_ref_release(hxc_l_initialActorPhases) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_enum_2c0e695d_destroy(&hxc_l_tmp_static_call_argument_0_owner_n64);
  if (hxc_string_release(&hxc_l_currentObjectiveId) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_string_release(&hxc_l_latestJournalId) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_string_release(&hxc_l_activeDialogue) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_record_f6127605_destroy(&hxc_l_receipt);
  if (hxc_string_release(&hxc_l_levelLabel) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_enum_4b68fff2_destroy(&hxc_l_g_h6904c3060e30);
  hxc_enum_fcb4401b_destroy(&hxc_l_g_h4ef171f4fe67);
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return;
}
