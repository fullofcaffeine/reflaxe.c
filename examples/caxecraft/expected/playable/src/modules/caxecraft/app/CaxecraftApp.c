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
  struct hxc_caxecraft_app_PlayableDialogueActor hxc_l_tmp_array_element_owner_n6 = { 0 };
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
    hxc_l_tmp_array_element_owner_n6 = hxc_l_tmp_array_get_result_n9;
    struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n12 = hxc_caxecraft_domain_GameSession_readCharacter(hxc_l_session, hxc_l_tmp_array_element_owner_n6.hxc_entityId);
    int32_t hxc_l_tmp_array_push_result_n13;
    if (hxc_array_ref_push_copy(hxc_l_tmp_load_result_n6, &hxc_l_tmp_instance_call_result_n12, &hxc_l_tmp_array_push_result_n13) != HXC_STATUS_OK)
    {
      abort();
    }
    (void)hxc_l_tmp_array_push_result_n13;
    hxc_array_809914b5_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n6);
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
  struct hxc_caxecraft_app_PlayableDialogueActor hxc_l_tmp_array_element_owner_n7 = { 0 };
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
    hxc_l_tmp_array_element_owner_n7 = hxc_l_tmp_array_get_result_n7;
    int32_t hxc_l_id = hxc_l_tmp_array_element_owner_n7.hxc_entityId;
    if (hxc_l_session == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n11 = hxc_caxecraft_domain_GameSession_readCharacter(hxc_l_session, hxc_l_id);
    bool hxc_l_tmp_short_circuit_result_n8 = hxc_l_tmp_instance_call_result_n11.hxc_id <= 0;
    if (!(hxc_l_tmp_instance_call_result_n11.hxc_id <= 0))
    {
      struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation hxc_l_tmp_call_result_n15 = hxc_caxecraft_app_CaxecraftApp_observeActorPhase(hxc_l_states, hxc_l_id, hxc_caxecraft_domain_ActorControllerPhase_Stationary);
      hxc_l_tmp_short_circuit_result_n8 = !hxc_l_tmp_call_result_n15.hxc_valid;
    }
    if (hxc_l_tmp_short_circuit_result_n8)
    {
      hxc_array_809914b5_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n7);
      if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
      {
        abort();
      }
      return false;
    }
    hxc_array_809914b5_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n7);
  }
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return true;
}

bool hxc_caxecraft_app_CaxecraftApp_drawActorPresentation(struct Camera3D hxc_l_camera, hxc_string hxc_l_asset, int32_t hxc_l_cellIndex, struct Vector3 hxc_l_position, double hxc_l_width, double hxc_l_height, struct Texture hxc_l_entityTexture, bool hxc_l_entityTextureReady, struct hxc_caxecraft_app_RuntimeTextureAtlasCatalog *hxc_l_runtimeTextures)
{
  const void *hxc_l_gc_roots[1] = { (const void *)hxc_l_runtimeTextures };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 1, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  bool hxc_l_tmp_short_circuit_result_n9 = hxc_l_asset.data != NULL && (hxc_l_asset.byte_length == (hxc_string){ (const uint8_t *)"entities", 8, true, NULL }.byte_length && (hxc_l_asset.byte_length == 0 || memcmp(hxc_l_asset.data, (hxc_string){ (const uint8_t *)"entities", 8, true, NULL }.data, hxc_l_asset.byte_length) == 0));
  if (hxc_l_asset.data != NULL && (hxc_l_asset.byte_length == (hxc_string){ (const uint8_t *)"entities", 8, true, NULL }.byte_length && (hxc_l_asset.byte_length == 0 || memcmp(hxc_l_asset.data, (hxc_string){ (const uint8_t *)"entities", 8, true, NULL }.data, hxc_l_asset.byte_length) == 0)))
  {
    hxc_l_tmp_short_circuit_result_n9 = hxc_l_entityTextureReady;
  }
  if (!hxc_l_tmp_short_circuit_result_n9)
  {
    if (hxc_l_runtimeTextures == NULL)
    {
      abort();
    }
    bool hxc_l_tmp_instance_call_result_n2 = hxc_caxecraft_app_RuntimeTextureAtlasCatalog_drawSprite(hxc_l_runtimeTextures, hxc_l_camera, hxc_l_asset, hxc_l_cellIndex, hxc_l_position, hxc_l_width, hxc_l_height);
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return hxc_l_tmp_instance_call_result_n2;
  }
  hxc_caxecraft_app_CaxecraftAtlas_drawEntitySprite(hxc_l_camera, hxc_l_entityTexture, hxc_l_cellIndex, hxc_l_position, hxc_l_width, hxc_l_height);
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return true;
}

void hxc_caxecraft_app_CaxecraftApp_drawActors(struct Camera3D hxc_l_camera, struct Texture hxc_l_entityTexture, bool hxc_l_entityTextureReady, struct hxc_caxecraft_app_RuntimeTextureAtlasCatalog *hxc_l_runtimeTextures, struct hxc_array_ref *hxc_l_dialogueActors, struct hxc_caxecraft_app_PlayableLevelView *hxc_l_level, struct hxc_caxecraft_domain_Character hxc_l_enemy, hxc_string hxc_l_enemyAsset, int32_t hxc_l_enemyCell, enum hxc_caxecraft_domain_ActorControllerPhase hxc_l_enemyPhase, struct hxc_caxecraft_gameplay_BerryDropState hxc_l_berryDrop)
{
  const void *hxc_l_gc_roots[2] = { (const void *)hxc_l_runtimeTextures, (const void *)hxc_l_level };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 2, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_caxecraft_app_PlayableDialogueActor hxc_l_tmp_array_element_owner_n18 = { 0 };
  struct hxc_caxecraft_app_PlayableDialogueActor hxc_l_tmp_array_element_owner_n19 = { 0 };
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
    double hxc_l_y_hb91fc71ee47d = hxc_l_actor.hxc_body.hxc_y + 0.76;
    double hxc_l_z_h5235e2136ed8 = hxc_l_actor.hxc_body.hxc_z;
    double hxc_l_tmp_record_field_load_result_n9 = hxc_l_actor.hxc_body.hxc_x;
    double hxc_l_tmp_load_result_n10 = hxc_l_y_hb91fc71ee47d;
    struct Vector3 hxc_l_position = (struct Vector3){ .x = (float)hxc_l_tmp_record_field_load_result_n9, .y = (float)hxc_l_tmp_load_result_n10, .z = (float)hxc_l_z_h5235e2136ed8 };
    struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n13 = (*hxc_l_level).hxc_dialogueActors;
    struct hxc_caxecraft_app_PlayableDialogueActor hxc_l_tmp_array_get_result_n15;
    if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n13, (size_t)hxc_l_index, &hxc_l_tmp_array_get_result_n15) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_array_element_owner_n18 = hxc_l_tmp_array_get_result_n15;
    struct hxc_caxecraft_app_PlayableDialogueActor hxc_l_tmp_array_element_borrow_result_n16 = hxc_l_tmp_array_element_owner_n18;
    struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n18 = (*hxc_l_level).hxc_dialogueActors;
    struct hxc_caxecraft_app_PlayableDialogueActor hxc_l_tmp_array_get_result_n20;
    if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n18, (size_t)hxc_l_index, &hxc_l_tmp_array_get_result_n20) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_array_element_owner_n19 = hxc_l_tmp_array_get_result_n20;
    struct hxc_caxecraft_app_PlayableDialogueActor hxc_l_tmp_array_element_borrow_result_n21 = hxc_l_tmp_array_element_owner_n19;
    bool hxc_l_tmp_call_result_n24 = hxc_caxecraft_app_CaxecraftApp_drawActorPresentation(hxc_l_camera, hxc_l_tmp_array_element_borrow_result_n16.hxc_presentationAsset, hxc_l_tmp_array_element_borrow_result_n21.hxc_presentationCellIndex, hxc_l_position, 0.95, 1.52, hxc_l_entityTexture, hxc_l_entityTextureReady, hxc_l_runtimeTextures);
    hxc_array_809914b5_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n19);
    hxc_array_809914b5_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n18);
    if (!hxc_l_tmp_call_result_n24)
    {
      double hxc_l_y_he268082b663b = hxc_l_actor.hxc_body.hxc_y + 0.54;
      double hxc_l_z_hd933ca46fb1e = hxc_l_actor.hxc_body.hxc_z;
      double hxc_l_tmp_record_field_load_result_n27 = hxc_l_actor.hxc_body.hxc_x;
      double hxc_l_tmp_load_result_n28 = hxc_l_y_he268082b663b;
      DrawCube((struct Vector3){ .x = (float)hxc_l_tmp_record_field_load_result_n27, .y = (float)hxc_l_tmp_load_result_n28, .z = (float)hxc_l_z_hd933ca46fb1e }, (float)0.50, (float)0.86, (float)0.42, (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 });
      double hxc_l_y_hd3b212f3bfc1 = hxc_l_actor.hxc_body.hxc_y + 1.18;
      double hxc_l_z_h456b1d7933d9 = hxc_l_actor.hxc_body.hxc_z;
      double hxc_l_tmp_record_field_load_result_n34 = hxc_l_actor.hxc_body.hxc_x;
      double hxc_l_tmp_load_result_n35 = hxc_l_y_hd3b212f3bfc1;
      DrawCube((struct Vector3){ .x = (float)hxc_l_tmp_record_field_load_result_n34, .y = (float)hxc_l_tmp_load_result_n35, .z = (float)hxc_l_z_h456b1d7933d9 }, (float)0.44, (float)0.44, (float)0.44, (struct Color){ .r = 228, .g = 239, .b = 227, .a = 255 });
      double hxc_l_y_h95e966b21144 = hxc_l_actor.hxc_body.hxc_y + 1.41;
      double hxc_l_z_h899701fa88e9 = hxc_l_actor.hxc_body.hxc_z;
      double hxc_l_tmp_record_field_load_result_n41 = hxc_l_actor.hxc_body.hxc_x;
      double hxc_l_tmp_load_result_n42 = hxc_l_y_h95e966b21144;
      DrawCube((struct Vector3){ .x = (float)hxc_l_tmp_record_field_load_result_n41, .y = (float)hxc_l_tmp_load_result_n42, .z = (float)hxc_l_z_h899701fa88e9 }, (float)0.48, (float)0.16, (float)0.48, (struct Color){ .r = 15, .g = 27, .b = 39, .a = 220 });
    }
  }
  if (hxc_l_enemy.hxc_vitals.hxc_health > 0)
  {
    double hxc_l_y_hf0f4ff6a8370 = hxc_l_enemy.hxc_body.hxc_y + 0.48;
    double hxc_l_z_h27fcf41ef419 = hxc_l_enemy.hxc_body.hxc_z;
    double hxc_l_tmp_load_result_n54 = hxc_l_y_hf0f4ff6a8370;
    bool hxc_l_tmp_call_result_n57 = hxc_caxecraft_app_CaxecraftApp_drawActorPresentation(hxc_l_camera, hxc_l_enemyAsset, hxc_l_enemyCell, (struct Vector3){ .x = (float)hxc_l_enemy.hxc_body.hxc_x, .y = (float)hxc_l_tmp_load_result_n54, .z = (float)hxc_l_z_h27fcf41ef419 }, 1.05, 0.96, hxc_l_entityTexture, hxc_l_entityTextureReady, hxc_l_runtimeTextures);
    if (!hxc_l_tmp_call_result_n57)
    {
      double hxc_l_y_ha0ba6c61c58f = hxc_l_enemy.hxc_body.hxc_y + 0.30;
      double hxc_l_z_h616141ab5434 = hxc_l_enemy.hxc_body.hxc_z;
      double hxc_l_tmp_load_result_n64 = hxc_l_y_ha0ba6c61c58f;
      DrawCube((struct Vector3){ .x = (float)hxc_l_enemy.hxc_body.hxc_x, .y = (float)hxc_l_tmp_load_result_n64, .z = (float)hxc_l_z_h616141ab5434 }, (float)0.70, (float)0.54, (float)0.70, (struct Color){ .r = 218, .g = 65, .b = 72, .a = 255 });
      double hxc_l_y_hcb0bba16ab64 = hxc_l_enemy.hxc_body.hxc_y + 0.66;
      double hxc_l_z_h85f0efe3c94c = hxc_l_enemy.hxc_body.hxc_z;
      double hxc_l_tmp_load_result_n74 = hxc_l_y_hcb0bba16ab64;
      DrawCube((struct Vector3){ .x = (float)hxc_l_enemy.hxc_body.hxc_x, .y = (float)hxc_l_tmp_load_result_n74, .z = (float)hxc_l_z_h85f0efe3c94c }, (float)0.50, (float)0.34, (float)0.50, (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 });
    }
    if (hxc_l_enemyPhase == hxc_caxecraft_domain_ActorControllerPhase_Windup)
    {
      double hxc_l_y_he5c94cb560ab = hxc_l_enemy.hxc_body.hxc_y + 1.02;
      double hxc_l_z_h5dbcdce6b557 = hxc_l_enemy.hxc_body.hxc_z;
      double hxc_l_tmp_load_result_n85 = hxc_l_y_he5c94cb560ab;
      DrawCube((struct Vector3){ .x = (float)hxc_l_enemy.hxc_body.hxc_x, .y = (float)hxc_l_tmp_load_result_n85, .z = (float)hxc_l_z_h5dbcdce6b557 }, (float)0.20, (float)0.20, (float)0.20, (struct Color){ .r = 218, .g = 65, .b = 72, .a = 255 });
    }
  }
  if (hxc_l_berryDrop.hxc_active)
  {
    double hxc_l_y_h476cb9ae6449 = hxc_l_berryDrop.hxc_y;
    double hxc_l_z_h1fed3bf06c9a = hxc_l_berryDrop.hxc_z;
    double hxc_l_tmp_load_result_n93 = hxc_l_y_h476cb9ae6449;
    DrawCube((struct Vector3){ .x = (float)(hxc_l_berryDrop.hxc_x - 0.12), .y = (float)hxc_l_tmp_load_result_n93, .z = (float)hxc_l_z_h1fed3bf06c9a }, (float)0.18, (float)0.18, (float)0.18, (struct Color){ .r = 174, .g = 78, .b = 136, .a = 255 });
    double hxc_l_y_hf8a02a237d75 = hxc_l_berryDrop.hxc_y;
    double hxc_l_z_hf8e9723b471d = hxc_l_berryDrop.hxc_z;
    double hxc_l_tmp_load_result_n100 = hxc_l_y_hf8a02a237d75;
    DrawCube((struct Vector3){ .x = (float)(hxc_l_berryDrop.hxc_x + 0.12), .y = (float)hxc_l_tmp_load_result_n100, .z = (float)hxc_l_z_hf8e9723b471d }, (float)0.18, (float)0.18, (float)0.18, (struct Color){ .r = 174, .g = 78, .b = 136, .a = 255 });
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

void hxc_caxecraft_app_CaxecraftApp_drawCampaignLoading(hxc_string hxc_l_destinationLabel, int32_t hxc_l_locale, struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_catalog)
{
  const void *hxc_l_gc_roots[1] = { (const void *)hxc_l_catalog };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 1, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_tmp_native_call_result_n0 = GetScreenWidth();
  int32_t hxc_l_width = (int32_t)hxc_l_tmp_native_call_result_n0;
  int32_t hxc_l_tmp_native_call_result_n1 = GetScreenHeight();
  int32_t hxc_l_height = (int32_t)hxc_l_tmp_native_call_result_n1;
  int32_t hxc_l_panelWidth = 460;
  int32_t hxc_l_panelHeight = 150;
  int32_t hxc_l_tmp_load_result_n2 = hxc_l_width;
  int32_t hxc_l_panelX = hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n2, hxc_l_panelWidth), (double)2));
  int32_t hxc_l_tmp_load_result_n4 = hxc_l_height;
  int32_t hxc_l_panelY = hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n4, hxc_l_panelHeight), (double)2));
  ClearBackground((struct Color){ .r = 126, .g = 190, .b = 201, .a = 255 });
  struct Color hxc_l_this1_h1eae82e59d2a = (struct Color){ .r = 15, .g = 27, .b = 39, .a = 220 };
  int32_t hxc_l_tmp_load_result_n8 = hxc_l_panelX;
  int32_t hxc_l_tmp_load_result_n9 = hxc_l_panelY;
  int32_t hxc_l_tmp_load_result_n10 = hxc_l_panelWidth;
  int32_t hxc_l_tmp_load_result_n11 = hxc_l_panelHeight;
  DrawRectangle((int32_t)hxc_l_tmp_load_result_n8, (int32_t)hxc_l_tmp_load_result_n9, (int32_t)hxc_l_tmp_load_result_n10, (int32_t)hxc_l_tmp_load_result_n11, hxc_l_this1_h1eae82e59d2a);
  struct Color hxc_l_this1_hf7cac3db3d49 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
  int32_t hxc_l_tmp_load_result_n14 = hxc_l_panelX;
  int32_t hxc_l_tmp_load_result_n15 = hxc_l_panelY;
  int32_t hxc_l_tmp_load_result_n16 = hxc_l_panelWidth;
  int32_t hxc_l_tmp_load_result_n17 = hxc_l_panelHeight;
  DrawRectangleLines((int32_t)hxc_l_tmp_load_result_n14, (int32_t)hxc_l_tmp_load_result_n15, (int32_t)hxc_l_tmp_load_result_n16, (int32_t)hxc_l_tmp_load_result_n17, hxc_l_this1_hf7cac3db3d49);
  struct Color hxc_l_this1_h85843f7d7a96 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
  if (hxc_l_catalog == NULL)
  {
    abort();
  }
  hxc_string hxc_l_tmp_instance_call_result_n20 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_catalog, hxc_l_locale, 26);
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n12 = hxc_l_tmp_instance_call_result_n20;
  hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n22 = HXC_BORROWED_CSTRING_INITIALIZER;
  if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n12, &hxc_l_tmp_string_borrow_cstring_result_n22) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_tmp_load_result_n23 = hxc_l_panelX;
  int32_t hxc_l_tmp_load_result_n24 = hxc_l_panelY;
  DrawText(hxc_l_tmp_string_borrow_cstring_result_n22.data, (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n23, 28), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n24, 24), (int32_t)22, hxc_l_this1_h85843f7d7a96);
  struct Color hxc_l_this1_hd6c4b58ab21e = (struct Color){ .r = 228, .g = 239, .b = 227, .a = 255 };
  hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n27 = HXC_BORROWED_CSTRING_INITIALIZER;
  if (hxc_string_borrow_cstring(&hxc_l_destinationLabel, &hxc_l_tmp_string_borrow_cstring_result_n27) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_tmp_load_result_n28 = hxc_l_panelX;
  int32_t hxc_l_tmp_load_result_n29 = hxc_l_panelY;
  DrawText(hxc_l_tmp_string_borrow_cstring_result_n27.data, (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n28, 28), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n29, 76), (int32_t)30, hxc_l_this1_hd6c4b58ab21e);
  if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n12) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
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
  hxc_string hxc_l_tmp_managed_flow_owner_n101 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n102 = { 0 };
  hxc_string hxc_l_text_hdc0cf8aa192d = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n106 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n107 = { 0 };
  hxc_string hxc_l_text_hd4ec764750c7 = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n111 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n112 = { 0 };
  hxc_string hxc_l_text_hdb3d27af9a67 = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n116 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n117 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n119 = { 0 };
  hxc_string hxc_l_text_h5fba5dfa15aa = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n123 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n124 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n126 = { 0 };
  hxc_string hxc_l_text1_hd2550a6ec056 = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n132 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n133 = { 0 };
  hxc_string hxc_l_text_h0ab14c22d666 = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n137 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n138 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n151 = { 0 };
  hxc_string hxc_l_text1_h4b6eb3af3313 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n154 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n156 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n157 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n158 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n55 = { 0 };
  hxc_string hxc_l_text_ha6928288a158 = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n62 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n63 = { 0 };
  hxc_string hxc_l_text1_h4f447cc52c76 = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n69 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n70 = { 0 };
  hxc_string hxc_l_text_h6f1ae2d53333 = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n74 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n75 = { 0 };
  hxc_string hxc_l_text_habd36cc1d10f = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n79 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n80 = { 0 };
  hxc_string hxc_l_text_h61ec78230735 = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n84 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n85 = { 0 };
  hxc_string hxc_l_text_h584b74a69193 = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n89 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n90 = { 0 };
  hxc_string hxc_l_text_h9c38f1b110a4 = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n94 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n95 = { 0 };
  hxc_string hxc_l_text_hefe7f2f7028d = { 0 };
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
  enum hxc_caxecraft_app_InteractionPrompt hxc_l_availableInteractionPrompt = hxc_l_view.hxc_interactionPrompt;
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
  struct Color hxc_l_text_hee4c52b2abdd = (struct Color){ .r = 228, .g = 239, .b = 227, .a = 255 };
  int32_t hxc_l_tmp_load_result_n60 = hxc_l_centerX;
  int32_t hxc_l_tmp_load_result_n61 = hxc_l_centerY;
  int32_t hxc_l_tmp_load_result_n62 = hxc_l_centerX;
  int32_t hxc_l_tmp_load_result_n63 = hxc_l_centerY;
  DrawLine((int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n60, 8), (int32_t)hxc_l_tmp_load_result_n61, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n62, 3), (int32_t)hxc_l_tmp_load_result_n63, hxc_l_text_hee4c52b2abdd);
  int32_t hxc_l_tmp_load_result_n65 = hxc_l_centerX;
  int32_t hxc_l_tmp_load_result_n66 = hxc_l_centerY;
  int32_t hxc_l_tmp_load_result_n67 = hxc_l_centerX;
  int32_t hxc_l_tmp_load_result_n68 = hxc_l_centerY;
  DrawLine((int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n65, 3), (int32_t)hxc_l_tmp_load_result_n66, (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n67, 8), (int32_t)hxc_l_tmp_load_result_n68, hxc_l_text_hee4c52b2abdd);
  int32_t hxc_l_tmp_load_result_n70 = hxc_l_centerX;
  int32_t hxc_l_tmp_load_result_n71 = hxc_l_centerY;
  int32_t hxc_l_tmp_load_result_n72 = hxc_l_centerX;
  int32_t hxc_l_tmp_load_result_n73 = hxc_l_centerY;
  DrawLine((int32_t)hxc_l_tmp_load_result_n70, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n71, 8), (int32_t)hxc_l_tmp_load_result_n72, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n73, 3), hxc_l_text_hee4c52b2abdd);
  int32_t hxc_l_tmp_load_result_n75 = hxc_l_centerX;
  int32_t hxc_l_tmp_load_result_n76 = hxc_l_centerY;
  int32_t hxc_l_tmp_load_result_n77 = hxc_l_centerX;
  int32_t hxc_l_tmp_load_result_n78 = hxc_l_centerY;
  DrawLine((int32_t)hxc_l_tmp_load_result_n75, (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n76, 3), (int32_t)hxc_l_tmp_load_result_n77, (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n78, 8), hxc_l_text_hee4c52b2abdd);
  struct Color hxc_l_this1_h6dd06065a80e = (struct Color){ .r = 15, .g = 27, .b = 39, .a = 220 };
  DrawRectangle((int32_t)18, (int32_t)18, (int32_t)460, (int32_t)108, hxc_l_this1_h6dd06065a80e);
  struct Color hxc_l_this1_h1569903d4c23 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
  DrawRectangleLines((int32_t)18, (int32_t)18, (int32_t)460, (int32_t)108, hxc_l_this1_h1569903d4c23);
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
  DrawText(hxc_l_tmp_string_borrow_cstring_result_n87.data, (int32_t)32, (int32_t)28, (int32_t)20, hxc_l_text_hee4c52b2abdd);
  hxc_string hxc_l_text1_hab11a2ba5ff6 = hxc_l_view.hxc_levelLabel;
  if (hxc_string_retain(hxc_l_text1_hab11a2ba5ff6) != HXC_STATUS_OK)
  {
    abort();
  }
  struct Color hxc_l_this1_h51b253bf6ed0 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
  hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n92 = HXC_BORROWED_CSTRING_INITIALIZER;
  if (hxc_string_borrow_cstring(&hxc_l_text1_hab11a2ba5ff6, &hxc_l_tmp_string_borrow_cstring_result_n92) != HXC_STATUS_OK)
  {
    abort();
  }
  DrawText(hxc_l_tmp_string_borrow_cstring_result_n92.data, (int32_t)250, (int32_t)30, (int32_t)16, hxc_l_this1_h51b253bf6ed0);
  hxc_string hxc_l_tmp_instance_call_result_n95 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_uiCatalog, hxc_l_locale, 4);
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n50 = hxc_l_tmp_instance_call_result_n95;
  hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n97 = HXC_BORROWED_CSTRING_INITIALIZER;
  if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n50, &hxc_l_tmp_string_borrow_cstring_result_n97) != HXC_STATUS_OK)
  {
    abort();
  }
  DrawText(hxc_l_tmp_string_borrow_cstring_result_n97.data, (int32_t)32, (int32_t)58, (int32_t)14, hxc_l_text_hee4c52b2abdd);
  hxc_caxecraft_app_HudDigits_drawNumber(32768, 82, 59, 5, (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 });
  hxc_string hxc_l_tmp_instance_call_result_n101 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_uiCatalog, hxc_l_locale, 8);
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n51 = hxc_l_tmp_instance_call_result_n101;
  hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n103 = HXC_BORROWED_CSTRING_INITIALIZER;
  if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n51, &hxc_l_tmp_string_borrow_cstring_result_n103) != HXC_STATUS_OK)
  {
    abort();
  }
  DrawText(hxc_l_tmp_string_borrow_cstring_result_n103.data, (int32_t)160, (int32_t)58, (int32_t)14, hxc_l_text_hee4c52b2abdd);
  hxc_caxecraft_app_HudDigits_drawNumber(hxc_l_visible, 230, 59, 5, (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 });
  hxc_string hxc_l_tmp_instance_call_result_n108 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_uiCatalog, hxc_l_locale, 5);
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n52 = hxc_l_tmp_instance_call_result_n108;
  hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n110 = HXC_BORROWED_CSTRING_INITIALIZER;
  if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n52, &hxc_l_tmp_string_borrow_cstring_result_n110) != HXC_STATUS_OK)
  {
    abort();
  }
  DrawText(hxc_l_tmp_string_borrow_cstring_result_n110.data, (int32_t)326, (int32_t)58, (int32_t)14, hxc_l_text_hee4c52b2abdd);
  hxc_caxecraft_app_HudDigits_drawNumber(hxc_l_drawCalls, 382, 59, 5, (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 });
  hxc_string hxc_l_tmp_instance_call_result_n115 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_uiCatalog, hxc_l_locale, 6);
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n53 = hxc_l_tmp_instance_call_result_n115;
  hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n117 = HXC_BORROWED_CSTRING_INITIALIZER;
  if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n53, &hxc_l_tmp_string_borrow_cstring_result_n117) != HXC_STATUS_OK)
  {
    abort();
  }
  DrawText(hxc_l_tmp_string_borrow_cstring_result_n117.data, (int32_t)32, (int32_t)86, (int32_t)12, hxc_l_text_hee4c52b2abdd);
  int32_t hxc_l_tmp_load_result_n119 = hxc_l_frames;
  hxc_caxecraft_app_HudDigits_drawNumber(hxc_l_tmp_load_result_n119, 82, 85, 6, hxc_l_text_hee4c52b2abdd);
  hxc_string hxc_l_tmp_instance_call_result_n122 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_uiCatalog, hxc_l_locale, 7);
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n54 = hxc_l_tmp_instance_call_result_n122;
  hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n124 = HXC_BORROWED_CSTRING_INITIALIZER;
  if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n54, &hxc_l_tmp_string_borrow_cstring_result_n124) != HXC_STATUS_OK)
  {
    abort();
  }
  DrawText(hxc_l_tmp_string_borrow_cstring_result_n124.data, (int32_t)174, (int32_t)86, (int32_t)12, hxc_l_text_hee4c52b2abdd);
  int32_t hxc_l_tmp_load_result_n126 = hxc_l_updates;
  hxc_caxecraft_app_HudDigits_drawNumber(hxc_l_tmp_load_result_n126, 216, 85, 6, hxc_l_text_hee4c52b2abdd);
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
  DrawText(hxc_l_tmp_string_borrow_cstring_result_n154.data, (int32_t)20, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n155, 22), (int32_t)14, hxc_l_text_hee4c52b2abdd);
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
    hxc_l_text_ha6928288a158 = hxc_l_view.hxc_objectiveTitle;
    if (hxc_string_retain(hxc_l_text_ha6928288a158) != HXC_STATUS_OK)
    {
      abort();
    }
    struct Color hxc_l_this1_h264d0e851a50 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
    hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n164 = HXC_BORROWED_CSTRING_INITIALIZER;
    if (hxc_string_borrow_cstring(&hxc_l_text_ha6928288a158, &hxc_l_tmp_string_borrow_cstring_result_n164) != HXC_STATUS_OK)
    {
      abort();
    }
    DrawText(hxc_l_tmp_string_borrow_cstring_result_n164.data, (int32_t)32, (int32_t)110, (int32_t)14, hxc_l_this1_h264d0e851a50);
    if (hxc_string_release(&hxc_l_text_ha6928288a158) != HXC_STATUS_OK)
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
    struct Color hxc_l_this1_h9e3b5e1e7d6c = (struct Color){ .r = 15, .g = 27, .b = 39, .a = 220 };
    int32_t hxc_l_tmp_load_result_n170 = hxc_l_centerX;
    int32_t hxc_l_tmp_load_result_n171 = hxc_l_centerY;
    DrawRectangle((int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n170, 260), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n171, 54), (int32_t)520, (int32_t)60, hxc_l_this1_h9e3b5e1e7d6c);
    hxc_string hxc_l_text1_hd3ff80b6cf90 = { 0 };
    switch (hxc_l_locale) {
      case 0:
        {
          hxc_l_text1_hd3ff80b6cf90 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
          if (hxc_string_retain(hxc_l_text1_hd3ff80b6cf90) != HXC_STATUS_OK)
          {
            abort();
          }
          break;
        }
      case 1:
        {
          hxc_l_text1_hd3ff80b6cf90 = (hxc_string){ (const uint8_t *)"es-mx", 5, true, NULL };
          if (hxc_string_retain(hxc_l_text1_hd3ff80b6cf90) != HXC_STATUS_OK)
          {
            abort();
          }
          break;
        }
      default:
        {
          hxc_l_text1_hd3ff80b6cf90 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
          if (hxc_string_retain(hxc_l_text1_hd3ff80b6cf90) != HXC_STATUS_OK)
          {
            abort();
          }
          break;
        }
    }
    hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n174 = hxc_l_text1_hd3ff80b6cf90;
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
    DrawText(hxc_l_tmp_string_borrow_cstring_result_n180.data, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n181, 225), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n182, 74), (int32_t)16, hxc_l_text_hee4c52b2abdd);
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
      hxc_l_tmp_short_circuit_result_n64 = hxc_l_availableInteractionPrompt != hxc_caxecraft_app_InteractionPrompt_NoInteractionPrompt;
    }
    if (hxc_l_tmp_short_circuit_result_n64)
    {
      struct Color hxc_l_this1_h80452de6a65c = (struct Color){ .r = 15, .g = 27, .b = 39, .a = 220 };
      int32_t hxc_l_tmp_load_result_n189 = hxc_l_centerX;
      int32_t hxc_l_tmp_load_result_n190 = hxc_l_centerY;
      DrawRectangle((int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n189, 260), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n190, 54), (int32_t)520, (int32_t)60, hxc_l_this1_h80452de6a65c);
      int32_t hxc_l_prompt = 0;
      switch (hxc_l_availableInteractionPrompt) {
        case hxc_caxecraft_app_InteractionPrompt_NoInteractionPrompt:
          {
            hxc_l_prompt = 11;
            break;
          }
        case hxc_caxecraft_app_InteractionPrompt_TalkInteractionPrompt:
          {
            hxc_l_prompt = 11;
            break;
          }
        case hxc_caxecraft_app_InteractionPrompt_UseInteractionPrompt:
          {
            hxc_l_prompt = 12;
            break;
          }
        default:
          {
            abort();
          }
      }
      hxc_string hxc_l_tmp_call_result_n194 = hxc_caxecraft_app_GameplayMessage_gameplayMessageId(hxc_l_prompt);
      hxc_l_text1_h4f447cc52c76 = hxc_l_tmp_call_result_n194;
      hxc_string hxc_l_text2_h1a6b00a717ca = { 0 };
      switch (hxc_l_locale) {
        case 0:
          {
            hxc_l_text2_h1a6b00a717ca = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
            if (hxc_string_retain(hxc_l_text2_h1a6b00a717ca) != HXC_STATUS_OK)
            {
              abort();
            }
            break;
          }
        case 1:
          {
            hxc_l_text2_h1a6b00a717ca = (hxc_string){ (const uint8_t *)"es-mx", 5, true, NULL };
            if (hxc_string_retain(hxc_l_text2_h1a6b00a717ca) != HXC_STATUS_OK)
            {
              abort();
            }
            break;
          }
        default:
          {
            hxc_l_text2_h1a6b00a717ca = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
            if (hxc_string_retain(hxc_l_text2_h1a6b00a717ca) != HXC_STATUS_OK)
            {
              abort();
            }
            break;
          }
      }
      hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n196 = hxc_l_text2_h1a6b00a717ca;
      hxc_l_tmp_managed_flow_owner_n69 = hxc_l_tmp_managed_flow_carrier_move_result_n196;
      struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_load_result_n197 = hxc_l_presentation;
      hxc_l_gc_roots[5] = (const void *)hxc_l_tmp_load_result_n197;
      if (hxc_l_tmp_load_result_n197 == NULL)
      {
        abort();
      }
      hxc_string hxc_l_tmp_load_result_n198 = hxc_l_text1_h4f447cc52c76;
      hxc_string hxc_l_tmp_instance_call_result_n200 = hxc_caxecraft_content_RuntimeLevelPresentation_message(hxc_l_tmp_load_result_n197, hxc_l_tmp_load_result_n198, hxc_l_tmp_managed_flow_owner_n69);
      hxc_l_tmp_cstring_ref_owner_owner_n70 = hxc_l_tmp_instance_call_result_n200;
      hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n202 = HXC_BORROWED_CSTRING_INITIALIZER;
      if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n70, &hxc_l_tmp_string_borrow_cstring_result_n202) != HXC_STATUS_OK)
      {
        abort();
      }
      int32_t hxc_l_tmp_load_result_n203 = hxc_l_centerX;
      int32_t hxc_l_tmp_load_result_n204 = hxc_l_centerY;
      DrawText(hxc_l_tmp_string_borrow_cstring_result_n202.data, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n203, 110), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n204, 74), (int32_t)18, hxc_l_text_hee4c52b2abdd);
      if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n70) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n69) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_text1_h4f447cc52c76) != HXC_STATUS_OK)
      {
        abort();
      }
    }
  }
  if (hxc_l_enemy.hxc_vitals.hxc_health > 0)
  {
    if (hxc_l_enemyPhase == hxc_caxecraft_domain_ActorControllerPhase_Windup)
    {
      struct Color hxc_l_color_h95dbbcd0add1 = (struct Color){ .r = 218, .g = 65, .b = 72, .a = 255 };
      hxc_string hxc_l_tmp_call_result_n210 = hxc_caxecraft_app_GameplayMessage_gameplayMessageId(9);
      hxc_l_text_h6f1ae2d53333 = hxc_l_tmp_call_result_n210;
      hxc_string hxc_l_text1_h5d126440af80 = { 0 };
      switch (hxc_l_locale) {
        case 0:
          {
            hxc_l_text1_h5d126440af80 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
            if (hxc_string_retain(hxc_l_text1_h5d126440af80) != HXC_STATUS_OK)
            {
              abort();
            }
            break;
          }
        case 1:
          {
            hxc_l_text1_h5d126440af80 = (hxc_string){ (const uint8_t *)"es-mx", 5, true, NULL };
            if (hxc_string_retain(hxc_l_text1_h5d126440af80) != HXC_STATUS_OK)
            {
              abort();
            }
            break;
          }
        default:
          {
            hxc_l_text1_h5d126440af80 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
            if (hxc_string_retain(hxc_l_text1_h5d126440af80) != HXC_STATUS_OK)
            {
              abort();
            }
            break;
          }
      }
      hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n212 = hxc_l_text1_h5d126440af80;
      hxc_l_tmp_managed_flow_owner_n74 = hxc_l_tmp_managed_flow_carrier_move_result_n212;
      struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_load_result_n213 = hxc_l_presentation;
      hxc_l_gc_roots[6] = (const void *)hxc_l_tmp_load_result_n213;
      if (hxc_l_tmp_load_result_n213 == NULL)
      {
        abort();
      }
      hxc_string hxc_l_tmp_load_result_n214 = hxc_l_text_h6f1ae2d53333;
      hxc_string hxc_l_tmp_instance_call_result_n216 = hxc_caxecraft_content_RuntimeLevelPresentation_message(hxc_l_tmp_load_result_n213, hxc_l_tmp_load_result_n214, hxc_l_tmp_managed_flow_owner_n74);
      hxc_l_tmp_cstring_ref_owner_owner_n75 = hxc_l_tmp_instance_call_result_n216;
      hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n218 = HXC_BORROWED_CSTRING_INITIALIZER;
      if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n75, &hxc_l_tmp_string_borrow_cstring_result_n218) != HXC_STATUS_OK)
      {
        abort();
      }
      int32_t hxc_l_tmp_load_result_n219 = hxc_l_width;
      DrawText(hxc_l_tmp_string_borrow_cstring_result_n218.data, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n219, 300), (int32_t)28, (int32_t)16, hxc_l_color_h95dbbcd0add1);
      if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n75) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n74) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_text_h6f1ae2d53333) != HXC_STATUS_OK)
      {
        abort();
      }
    }
    else
    {
      if (hxc_l_enemyPhase == hxc_caxecraft_domain_ActorControllerPhase_Chasing)
      {
        struct Color hxc_l_color_h7fd75f796912 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
        hxc_string hxc_l_tmp_call_result_n224 = hxc_caxecraft_app_GameplayMessage_gameplayMessageId(7);
        hxc_l_text_habd36cc1d10f = hxc_l_tmp_call_result_n224;
        hxc_string hxc_l_text1_h2dea0d05aa14 = { 0 };
        switch (hxc_l_locale) {
          case 0:
            {
              hxc_l_text1_h2dea0d05aa14 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
              if (hxc_string_retain(hxc_l_text1_h2dea0d05aa14) != HXC_STATUS_OK)
              {
                abort();
              }
              break;
            }
          case 1:
            {
              hxc_l_text1_h2dea0d05aa14 = (hxc_string){ (const uint8_t *)"es-mx", 5, true, NULL };
              if (hxc_string_retain(hxc_l_text1_h2dea0d05aa14) != HXC_STATUS_OK)
              {
                abort();
              }
              break;
            }
          default:
            {
              hxc_l_text1_h2dea0d05aa14 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
              if (hxc_string_retain(hxc_l_text1_h2dea0d05aa14) != HXC_STATUS_OK)
              {
                abort();
              }
              break;
            }
        }
        hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n226 = hxc_l_text1_h2dea0d05aa14;
        hxc_l_tmp_managed_flow_owner_n79 = hxc_l_tmp_managed_flow_carrier_move_result_n226;
        struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_load_result_n227 = hxc_l_presentation;
        hxc_l_gc_roots[7] = (const void *)hxc_l_tmp_load_result_n227;
        if (hxc_l_tmp_load_result_n227 == NULL)
        {
          abort();
        }
        hxc_string hxc_l_tmp_load_result_n228 = hxc_l_text_habd36cc1d10f;
        hxc_string hxc_l_tmp_instance_call_result_n230 = hxc_caxecraft_content_RuntimeLevelPresentation_message(hxc_l_tmp_load_result_n227, hxc_l_tmp_load_result_n228, hxc_l_tmp_managed_flow_owner_n79);
        hxc_l_tmp_cstring_ref_owner_owner_n80 = hxc_l_tmp_instance_call_result_n230;
        hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n232 = HXC_BORROWED_CSTRING_INITIALIZER;
        if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n80, &hxc_l_tmp_string_borrow_cstring_result_n232) != HXC_STATUS_OK)
        {
          abort();
        }
        int32_t hxc_l_tmp_load_result_n233 = hxc_l_width;
        DrawText(hxc_l_tmp_string_borrow_cstring_result_n232.data, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n233, 180), (int32_t)28, (int32_t)16, hxc_l_color_h7fd75f796912);
        if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n80) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n79) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_string_release(&hxc_l_text_habd36cc1d10f) != HXC_STATUS_OK)
        {
          abort();
        }
      }
    }
  }
  if (hxc_l_strikeHit)
  {
    struct Color hxc_l_color_h973cfb412c0d = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
    hxc_string hxc_l_tmp_call_result_n237 = hxc_caxecraft_app_GameplayMessage_gameplayMessageId(5);
    hxc_l_text_h61ec78230735 = hxc_l_tmp_call_result_n237;
    hxc_string hxc_l_text1_h4b1d5c689b96 = { 0 };
    switch (hxc_l_locale) {
      case 0:
        {
          hxc_l_text1_h4b1d5c689b96 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
          if (hxc_string_retain(hxc_l_text1_h4b1d5c689b96) != HXC_STATUS_OK)
          {
            abort();
          }
          break;
        }
      case 1:
        {
          hxc_l_text1_h4b1d5c689b96 = (hxc_string){ (const uint8_t *)"es-mx", 5, true, NULL };
          if (hxc_string_retain(hxc_l_text1_h4b1d5c689b96) != HXC_STATUS_OK)
          {
            abort();
          }
          break;
        }
      default:
        {
          hxc_l_text1_h4b1d5c689b96 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
          if (hxc_string_retain(hxc_l_text1_h4b1d5c689b96) != HXC_STATUS_OK)
          {
            abort();
          }
          break;
        }
    }
    hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n239 = hxc_l_text1_h4b1d5c689b96;
    hxc_l_tmp_managed_flow_owner_n84 = hxc_l_tmp_managed_flow_carrier_move_result_n239;
    struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_load_result_n240 = hxc_l_presentation;
    hxc_l_gc_roots[8] = (const void *)hxc_l_tmp_load_result_n240;
    if (hxc_l_tmp_load_result_n240 == NULL)
    {
      abort();
    }
    hxc_string hxc_l_tmp_load_result_n241 = hxc_l_text_h61ec78230735;
    hxc_string hxc_l_tmp_instance_call_result_n243 = hxc_caxecraft_content_RuntimeLevelPresentation_message(hxc_l_tmp_load_result_n240, hxc_l_tmp_load_result_n241, hxc_l_tmp_managed_flow_owner_n84);
    hxc_l_tmp_cstring_ref_owner_owner_n85 = hxc_l_tmp_instance_call_result_n243;
    hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n245 = HXC_BORROWED_CSTRING_INITIALIZER;
    if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n85, &hxc_l_tmp_string_borrow_cstring_result_n245) != HXC_STATUS_OK)
    {
      abort();
    }
    int32_t hxc_l_tmp_load_result_n246 = hxc_l_centerX;
    int32_t hxc_l_tmp_load_result_n247 = hxc_l_centerY;
    DrawText(hxc_l_tmp_string_borrow_cstring_result_n245.data, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n246, 70), (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n247, 54), (int32_t)18, hxc_l_color_h973cfb412c0d);
    if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n85) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n84) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_text_h61ec78230735) != HXC_STATUS_OK)
    {
      abort();
    }
  }
  if (hxc_l_enemyDefeated)
  {
    struct Color hxc_l_color_hb778355ef176 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
    hxc_string hxc_l_tmp_call_result_n251 = hxc_caxecraft_app_GameplayMessage_gameplayMessageId(8);
    hxc_l_text_h584b74a69193 = hxc_l_tmp_call_result_n251;
    hxc_string hxc_l_text1_h42247c77b586 = { 0 };
    switch (hxc_l_locale) {
      case 0:
        {
          hxc_l_text1_h42247c77b586 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
          if (hxc_string_retain(hxc_l_text1_h42247c77b586) != HXC_STATUS_OK)
          {
            abort();
          }
          break;
        }
      case 1:
        {
          hxc_l_text1_h42247c77b586 = (hxc_string){ (const uint8_t *)"es-mx", 5, true, NULL };
          if (hxc_string_retain(hxc_l_text1_h42247c77b586) != HXC_STATUS_OK)
          {
            abort();
          }
          break;
        }
      default:
        {
          hxc_l_text1_h42247c77b586 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
          if (hxc_string_retain(hxc_l_text1_h42247c77b586) != HXC_STATUS_OK)
          {
            abort();
          }
          break;
        }
    }
    hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n253 = hxc_l_text1_h42247c77b586;
    hxc_l_tmp_managed_flow_owner_n89 = hxc_l_tmp_managed_flow_carrier_move_result_n253;
    struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_load_result_n254 = hxc_l_presentation;
    hxc_l_gc_roots[9] = (const void *)hxc_l_tmp_load_result_n254;
    if (hxc_l_tmp_load_result_n254 == NULL)
    {
      abort();
    }
    hxc_string hxc_l_tmp_load_result_n255 = hxc_l_text_h584b74a69193;
    hxc_string hxc_l_tmp_instance_call_result_n257 = hxc_caxecraft_content_RuntimeLevelPresentation_message(hxc_l_tmp_load_result_n254, hxc_l_tmp_load_result_n255, hxc_l_tmp_managed_flow_owner_n89);
    hxc_l_tmp_cstring_ref_owner_owner_n90 = hxc_l_tmp_instance_call_result_n257;
    hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n259 = HXC_BORROWED_CSTRING_INITIALIZER;
    if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n90, &hxc_l_tmp_string_borrow_cstring_result_n259) != HXC_STATUS_OK)
    {
      abort();
    }
    int32_t hxc_l_tmp_load_result_n260 = hxc_l_width;
    DrawText(hxc_l_tmp_string_borrow_cstring_result_n259.data, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n260, 285), (int32_t)54, (int32_t)16, hxc_l_color_hb778355ef176);
    if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n90) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n89) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_text_h584b74a69193) != HXC_STATUS_OK)
    {
      abort();
    }
  }
  if (hxc_l_enemyAttacked)
  {
    struct Color hxc_l_color_h6dbc578aea7b = (struct Color){ .r = 218, .g = 65, .b = 72, .a = 255 };
    hxc_string hxc_l_tmp_call_result_n264 = hxc_caxecraft_app_GameplayMessage_gameplayMessageId(15);
    hxc_l_text_h9c38f1b110a4 = hxc_l_tmp_call_result_n264;
    hxc_string hxc_l_text1_h2fc24d23ddf0 = { 0 };
    switch (hxc_l_locale) {
      case 0:
        {
          hxc_l_text1_h2fc24d23ddf0 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
          if (hxc_string_retain(hxc_l_text1_h2fc24d23ddf0) != HXC_STATUS_OK)
          {
            abort();
          }
          break;
        }
      case 1:
        {
          hxc_l_text1_h2fc24d23ddf0 = (hxc_string){ (const uint8_t *)"es-mx", 5, true, NULL };
          if (hxc_string_retain(hxc_l_text1_h2fc24d23ddf0) != HXC_STATUS_OK)
          {
            abort();
          }
          break;
        }
      default:
        {
          hxc_l_text1_h2fc24d23ddf0 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
          if (hxc_string_retain(hxc_l_text1_h2fc24d23ddf0) != HXC_STATUS_OK)
          {
            abort();
          }
          break;
        }
    }
    hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n266 = hxc_l_text1_h2fc24d23ddf0;
    hxc_l_tmp_managed_flow_owner_n94 = hxc_l_tmp_managed_flow_carrier_move_result_n266;
    struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_load_result_n267 = hxc_l_presentation;
    hxc_l_gc_roots[10] = (const void *)hxc_l_tmp_load_result_n267;
    if (hxc_l_tmp_load_result_n267 == NULL)
    {
      abort();
    }
    hxc_string hxc_l_tmp_load_result_n268 = hxc_l_text_h9c38f1b110a4;
    hxc_string hxc_l_tmp_instance_call_result_n270 = hxc_caxecraft_content_RuntimeLevelPresentation_message(hxc_l_tmp_load_result_n267, hxc_l_tmp_load_result_n268, hxc_l_tmp_managed_flow_owner_n94);
    hxc_l_tmp_cstring_ref_owner_owner_n95 = hxc_l_tmp_instance_call_result_n270;
    hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n272 = HXC_BORROWED_CSTRING_INITIALIZER;
    if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n95, &hxc_l_tmp_string_borrow_cstring_result_n272) != HXC_STATUS_OK)
    {
      abort();
    }
    int32_t hxc_l_tmp_load_result_n273 = hxc_l_width;
    DrawText(hxc_l_tmp_string_borrow_cstring_result_n272.data, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n273, 330), (int32_t)82, (int32_t)16, hxc_l_color_h6dbc578aea7b);
    if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n95) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n94) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_text_h9c38f1b110a4) != HXC_STATUS_OK)
    {
      abort();
    }
  }
  if (hxc_l_pickedUp)
  {
    int32_t hxc_l_tmp_load_result_n276 = hxc_l_pickupAmount;
    int32_t hxc_l_tmp_conditional_result_n97 = 0;
    if (hxc_l_tmp_load_result_n276 == 1)
    {
      hxc_l_tmp_conditional_result_n97 = 0;
    }
    else
    {
      hxc_l_tmp_conditional_result_n97 = 1;
    }
    int32_t hxc_l_pickupMessage = hxc_l_tmp_conditional_result_n97;
    struct Color hxc_l_color_hae955e132070 = (struct Color){ .r = 174, .g = 78, .b = 136, .a = 255 };
    hxc_string hxc_l_tmp_call_result_n280 = hxc_caxecraft_app_GameplayMessage_gameplayMessageId(hxc_l_pickupMessage);
    hxc_l_text_hefe7f2f7028d = hxc_l_tmp_call_result_n280;
    hxc_string hxc_l_text1_hd8022ac5daf4 = { 0 };
    switch (hxc_l_locale) {
      case 0:
        {
          hxc_l_text1_hd8022ac5daf4 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
          if (hxc_string_retain(hxc_l_text1_hd8022ac5daf4) != HXC_STATUS_OK)
          {
            abort();
          }
          break;
        }
      case 1:
        {
          hxc_l_text1_hd8022ac5daf4 = (hxc_string){ (const uint8_t *)"es-mx", 5, true, NULL };
          if (hxc_string_retain(hxc_l_text1_hd8022ac5daf4) != HXC_STATUS_OK)
          {
            abort();
          }
          break;
        }
      default:
        {
          hxc_l_text1_hd8022ac5daf4 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
          if (hxc_string_retain(hxc_l_text1_hd8022ac5daf4) != HXC_STATUS_OK)
          {
            abort();
          }
          break;
        }
    }
    hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n282 = hxc_l_text1_hd8022ac5daf4;
    hxc_l_tmp_managed_flow_owner_n101 = hxc_l_tmp_managed_flow_carrier_move_result_n282;
    struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_load_result_n283 = hxc_l_presentation;
    hxc_l_gc_roots[11] = (const void *)hxc_l_tmp_load_result_n283;
    if (hxc_l_tmp_load_result_n283 == NULL)
    {
      abort();
    }
    hxc_string hxc_l_tmp_load_result_n284 = hxc_l_text_hefe7f2f7028d;
    hxc_string hxc_l_tmp_instance_call_result_n286 = hxc_caxecraft_content_RuntimeLevelPresentation_message(hxc_l_tmp_load_result_n283, hxc_l_tmp_load_result_n284, hxc_l_tmp_managed_flow_owner_n101);
    hxc_l_tmp_cstring_ref_owner_owner_n102 = hxc_l_tmp_instance_call_result_n286;
    hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n288 = HXC_BORROWED_CSTRING_INITIALIZER;
    if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n102, &hxc_l_tmp_string_borrow_cstring_result_n288) != HXC_STATUS_OK)
    {
      abort();
    }
    int32_t hxc_l_tmp_load_result_n289 = hxc_l_centerX;
    int32_t hxc_l_tmp_load_result_n290 = hxc_l_centerY;
    DrawText(hxc_l_tmp_string_borrow_cstring_result_n288.data, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n289, 48), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n290, 24), (int32_t)18, hxc_l_color_hae955e132070);
    if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n102) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n101) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_text_hefe7f2f7028d) != HXC_STATUS_OK)
    {
      abort();
    }
  }
  if (hxc_l_inventoryFullReason == 1)
  {
    struct Color hxc_l_color_he70f57e1b24d = (struct Color){ .r = 238, .g = 113, .b = 78, .a = 255 };
    hxc_string hxc_l_tmp_call_result_n294 = hxc_caxecraft_app_GameplayMessage_gameplayMessageId(3);
    hxc_l_text_hdc0cf8aa192d = hxc_l_tmp_call_result_n294;
    hxc_string hxc_l_text1_h3875b62cdea6 = { 0 };
    switch (hxc_l_locale) {
      case 0:
        {
          hxc_l_text1_h3875b62cdea6 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
          if (hxc_string_retain(hxc_l_text1_h3875b62cdea6) != HXC_STATUS_OK)
          {
            abort();
          }
          break;
        }
      case 1:
        {
          hxc_l_text1_h3875b62cdea6 = (hxc_string){ (const uint8_t *)"es-mx", 5, true, NULL };
          if (hxc_string_retain(hxc_l_text1_h3875b62cdea6) != HXC_STATUS_OK)
          {
            abort();
          }
          break;
        }
      default:
        {
          hxc_l_text1_h3875b62cdea6 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
          if (hxc_string_retain(hxc_l_text1_h3875b62cdea6) != HXC_STATUS_OK)
          {
            abort();
          }
          break;
        }
    }
    hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n296 = hxc_l_text1_h3875b62cdea6;
    hxc_l_tmp_managed_flow_owner_n106 = hxc_l_tmp_managed_flow_carrier_move_result_n296;
    struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_load_result_n297 = hxc_l_presentation;
    hxc_l_gc_roots[12] = (const void *)hxc_l_tmp_load_result_n297;
    if (hxc_l_tmp_load_result_n297 == NULL)
    {
      abort();
    }
    hxc_string hxc_l_tmp_load_result_n298 = hxc_l_text_hdc0cf8aa192d;
    hxc_string hxc_l_tmp_instance_call_result_n300 = hxc_caxecraft_content_RuntimeLevelPresentation_message(hxc_l_tmp_load_result_n297, hxc_l_tmp_load_result_n298, hxc_l_tmp_managed_flow_owner_n106);
    hxc_l_tmp_cstring_ref_owner_owner_n107 = hxc_l_tmp_instance_call_result_n300;
    hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n302 = HXC_BORROWED_CSTRING_INITIALIZER;
    if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n107, &hxc_l_tmp_string_borrow_cstring_result_n302) != HXC_STATUS_OK)
    {
      abort();
    }
    int32_t hxc_l_tmp_load_result_n303 = hxc_l_centerX;
    int32_t hxc_l_tmp_load_result_n304 = hxc_l_centerY;
    DrawText(hxc_l_tmp_string_borrow_cstring_result_n302.data, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n303, 150), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n304, 48), (int32_t)16, hxc_l_color_he70f57e1b24d);
    if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n107) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n106) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_text_hdc0cf8aa192d) != HXC_STATUS_OK)
    {
      abort();
    }
  }
  else
  {
    if (hxc_l_inventoryFullReason == 2)
    {
      struct Color hxc_l_color_h9b20dd002f15 = (struct Color){ .r = 238, .g = 113, .b = 78, .a = 255 };
      hxc_string hxc_l_tmp_call_result_n308 = hxc_caxecraft_app_GameplayMessage_gameplayMessageId(4);
      hxc_l_text_hd4ec764750c7 = hxc_l_tmp_call_result_n308;
      hxc_string hxc_l_text1_hf2103857ed6c = { 0 };
      switch (hxc_l_locale) {
        case 0:
          {
            hxc_l_text1_hf2103857ed6c = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
            if (hxc_string_retain(hxc_l_text1_hf2103857ed6c) != HXC_STATUS_OK)
            {
              abort();
            }
            break;
          }
        case 1:
          {
            hxc_l_text1_hf2103857ed6c = (hxc_string){ (const uint8_t *)"es-mx", 5, true, NULL };
            if (hxc_string_retain(hxc_l_text1_hf2103857ed6c) != HXC_STATUS_OK)
            {
              abort();
            }
            break;
          }
        default:
          {
            hxc_l_text1_hf2103857ed6c = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
            if (hxc_string_retain(hxc_l_text1_hf2103857ed6c) != HXC_STATUS_OK)
            {
              abort();
            }
            break;
          }
      }
      hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n310 = hxc_l_text1_hf2103857ed6c;
      hxc_l_tmp_managed_flow_owner_n111 = hxc_l_tmp_managed_flow_carrier_move_result_n310;
      struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_load_result_n311 = hxc_l_presentation;
      hxc_l_gc_roots[13] = (const void *)hxc_l_tmp_load_result_n311;
      if (hxc_l_tmp_load_result_n311 == NULL)
      {
        abort();
      }
      hxc_string hxc_l_tmp_load_result_n312 = hxc_l_text_hd4ec764750c7;
      hxc_string hxc_l_tmp_instance_call_result_n314 = hxc_caxecraft_content_RuntimeLevelPresentation_message(hxc_l_tmp_load_result_n311, hxc_l_tmp_load_result_n312, hxc_l_tmp_managed_flow_owner_n111);
      hxc_l_tmp_cstring_ref_owner_owner_n112 = hxc_l_tmp_instance_call_result_n314;
      hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n316 = HXC_BORROWED_CSTRING_INITIALIZER;
      if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n112, &hxc_l_tmp_string_borrow_cstring_result_n316) != HXC_STATUS_OK)
      {
        abort();
      }
      int32_t hxc_l_tmp_load_result_n317 = hxc_l_centerX;
      int32_t hxc_l_tmp_load_result_n318 = hxc_l_centerY;
      DrawText(hxc_l_tmp_string_borrow_cstring_result_n316.data, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n317, 155), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n318, 48), (int32_t)16, hxc_l_color_h9b20dd002f15);
      if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n112) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n111) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_text_hd4ec764750c7) != HXC_STATUS_OK)
      {
        abort();
      }
    }
  }
  if (hxc_l_recoveryVisible)
  {
    if (hxc_l_recoveryFeedback == 1)
    {
      struct Color hxc_l_color_h34632293f616 = (struct Color){ .r = 94, .g = 212, .b = 136, .a = 255 };
      hxc_string hxc_l_tmp_call_result_n323 = hxc_caxecraft_app_GameplayMessage_gameplayMessageId(2);
      hxc_l_text_hdb3d27af9a67 = hxc_l_tmp_call_result_n323;
      hxc_string hxc_l_text1_haedc0b0eb74b = { 0 };
      switch (hxc_l_locale) {
        case 0:
          {
            hxc_l_text1_haedc0b0eb74b = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
            if (hxc_string_retain(hxc_l_text1_haedc0b0eb74b) != HXC_STATUS_OK)
            {
              abort();
            }
            break;
          }
        case 1:
          {
            hxc_l_text1_haedc0b0eb74b = (hxc_string){ (const uint8_t *)"es-mx", 5, true, NULL };
            if (hxc_string_retain(hxc_l_text1_haedc0b0eb74b) != HXC_STATUS_OK)
            {
              abort();
            }
            break;
          }
        default:
          {
            hxc_l_text1_haedc0b0eb74b = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
            if (hxc_string_retain(hxc_l_text1_haedc0b0eb74b) != HXC_STATUS_OK)
            {
              abort();
            }
            break;
          }
      }
      hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n325 = hxc_l_text1_haedc0b0eb74b;
      hxc_l_tmp_managed_flow_owner_n116 = hxc_l_tmp_managed_flow_carrier_move_result_n325;
      struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_load_result_n326 = hxc_l_presentation;
      hxc_l_gc_roots[14] = (const void *)hxc_l_tmp_load_result_n326;
      if (hxc_l_tmp_load_result_n326 == NULL)
      {
        abort();
      }
      hxc_string hxc_l_tmp_load_result_n327 = hxc_l_text_hdb3d27af9a67;
      hxc_string hxc_l_tmp_instance_call_result_n329 = hxc_caxecraft_content_RuntimeLevelPresentation_message(hxc_l_tmp_load_result_n326, hxc_l_tmp_load_result_n327, hxc_l_tmp_managed_flow_owner_n116);
      hxc_l_tmp_cstring_ref_owner_owner_n117 = hxc_l_tmp_instance_call_result_n329;
      hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n331 = HXC_BORROWED_CSTRING_INITIALIZER;
      if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n117, &hxc_l_tmp_string_borrow_cstring_result_n331) != HXC_STATUS_OK)
      {
        abort();
      }
      int32_t hxc_l_tmp_load_result_n332 = hxc_l_centerX;
      int32_t hxc_l_tmp_load_result_n333 = hxc_l_centerY;
      DrawText(hxc_l_tmp_string_borrow_cstring_result_n331.data, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n332, 88), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n333, 24), (int32_t)18, hxc_l_color_h34632293f616);
      if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n117) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n116) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_text_hdb3d27af9a67) != HXC_STATUS_OK)
      {
        abort();
      }
    }
    else
    {
      if (hxc_l_recoveryFeedback == 2)
      {
        struct Color hxc_l_this1_h0528e43e9657 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
        hxc_string hxc_l_tmp_instance_call_result_n338 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_uiCatalog, hxc_l_locale, 25);
        hxc_l_tmp_cstring_ref_owner_owner_n119 = hxc_l_tmp_instance_call_result_n338;
        hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n340 = HXC_BORROWED_CSTRING_INITIALIZER;
        if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n119, &hxc_l_tmp_string_borrow_cstring_result_n340) != HXC_STATUS_OK)
        {
          abort();
        }
        int32_t hxc_l_tmp_load_result_n341 = hxc_l_centerX;
        int32_t hxc_l_tmp_load_result_n342 = hxc_l_centerY;
        DrawText(hxc_l_tmp_string_borrow_cstring_result_n340.data, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n341, 96), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n342, 24), (int32_t)18, hxc_l_this1_h0528e43e9657);
        if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n119) != HXC_STATUS_OK)
        {
          abort();
        }
      }
      else
      {
        if (hxc_l_recoveryFeedback == 3)
        {
          struct Color hxc_l_color_heb6e746e4aa8 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
          hxc_string hxc_l_tmp_call_result_n346 = hxc_caxecraft_app_GameplayMessage_gameplayMessageId(13);
          hxc_l_text_h5fba5dfa15aa = hxc_l_tmp_call_result_n346;
          hxc_string hxc_l_text1_h3756939ae4a8 = { 0 };
          switch (hxc_l_locale) {
            case 0:
              {
                hxc_l_text1_h3756939ae4a8 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
                if (hxc_string_retain(hxc_l_text1_h3756939ae4a8) != HXC_STATUS_OK)
                {
                  abort();
                }
                break;
              }
            case 1:
              {
                hxc_l_text1_h3756939ae4a8 = (hxc_string){ (const uint8_t *)"es-mx", 5, true, NULL };
                if (hxc_string_retain(hxc_l_text1_h3756939ae4a8) != HXC_STATUS_OK)
                {
                  abort();
                }
                break;
              }
            default:
              {
                hxc_l_text1_h3756939ae4a8 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
                if (hxc_string_retain(hxc_l_text1_h3756939ae4a8) != HXC_STATUS_OK)
                {
                  abort();
                }
                break;
              }
          }
          hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n348 = hxc_l_text1_h3756939ae4a8;
          hxc_l_tmp_managed_flow_owner_n123 = hxc_l_tmp_managed_flow_carrier_move_result_n348;
          struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_load_result_n349 = hxc_l_presentation;
          hxc_l_gc_roots[15] = (const void *)hxc_l_tmp_load_result_n349;
          if (hxc_l_tmp_load_result_n349 == NULL)
          {
            abort();
          }
          hxc_string hxc_l_tmp_load_result_n350 = hxc_l_text_h5fba5dfa15aa;
          hxc_string hxc_l_tmp_instance_call_result_n352 = hxc_caxecraft_content_RuntimeLevelPresentation_message(hxc_l_tmp_load_result_n349, hxc_l_tmp_load_result_n350, hxc_l_tmp_managed_flow_owner_n123);
          hxc_l_tmp_cstring_ref_owner_owner_n124 = hxc_l_tmp_instance_call_result_n352;
          hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n354 = HXC_BORROWED_CSTRING_INITIALIZER;
          if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n124, &hxc_l_tmp_string_borrow_cstring_result_n354) != HXC_STATUS_OK)
          {
            abort();
          }
          int32_t hxc_l_tmp_load_result_n355 = hxc_l_centerX;
          int32_t hxc_l_tmp_load_result_n356 = hxc_l_centerY;
          DrawText(hxc_l_tmp_string_borrow_cstring_result_n354.data, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n355, 76), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n356, 24), (int32_t)18, hxc_l_color_heb6e746e4aa8);
          if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n124) != HXC_STATUS_OK)
          {
            abort();
          }
          if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n123) != HXC_STATUS_OK)
          {
            abort();
          }
          if (hxc_string_release(&hxc_l_text_h5fba5dfa15aa) != HXC_STATUS_OK)
          {
            abort();
          }
        }
      }
    }
  }
  if (hxc_l_aquaticEquipmentVisible)
  {
    struct Color hxc_l_this1_hb9c36e1bc98c = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
    hxc_string hxc_l_tmp_instance_call_result_n361 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_uiCatalog, hxc_l_locale, 0);
    hxc_l_tmp_cstring_ref_owner_owner_n126 = hxc_l_tmp_instance_call_result_n361;
    hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n363 = HXC_BORROWED_CSTRING_INITIALIZER;
    if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n126, &hxc_l_tmp_string_borrow_cstring_result_n363) != HXC_STATUS_OK)
    {
      abort();
    }
    int32_t hxc_l_tmp_load_result_n364 = hxc_l_centerX;
    int32_t hxc_l_tmp_load_result_n365 = hxc_l_centerY;
    DrawText(hxc_l_tmp_string_borrow_cstring_result_n363.data, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n364, 128), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n365, 24), (int32_t)18, hxc_l_this1_hb9c36e1bc98c);
    if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n126) != HXC_STATUS_OK)
    {
      abort();
    }
  }
  if (hxc_l_vitals.hxc_safeTicks > 15)
  {
    struct Color hxc_l_this1_h603a7d3bfffa = (struct Color){ .r = 218, .g = 65, .b = 72, .a = 255 };
    int32_t hxc_l_tmp_load_result_n369 = hxc_l_width;
    int32_t hxc_l_tmp_load_result_n370 = hxc_l_height;
    DrawRectangleLines((int32_t)4, (int32_t)4, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n369, 8), (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n370, 8), hxc_l_this1_h603a7d3bfffa);
  }
  if (hxc_l_vitals.hxc_health <= 0)
  {
    struct Color hxc_l_this1_h5f813a3f4336 = (struct Color){ .r = 15, .g = 27, .b = 39, .a = 220 };
    int32_t hxc_l_tmp_load_result_n374 = hxc_l_centerX;
    int32_t hxc_l_tmp_load_result_n375 = hxc_l_centerY;
    DrawRectangle((int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n374, 250), (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n375, 74), (int32_t)500, (int32_t)148, hxc_l_this1_h5f813a3f4336);
    struct Color hxc_l_this1_h49d374a22a26 = (struct Color){ .r = 218, .g = 65, .b = 72, .a = 255 };
    int32_t hxc_l_tmp_load_result_n378 = hxc_l_centerX;
    int32_t hxc_l_tmp_load_result_n379 = hxc_l_centerY;
    DrawRectangleLines((int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n378, 250), (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n379, 74), (int32_t)500, (int32_t)148, hxc_l_this1_h49d374a22a26);
    hxc_string hxc_l_tmp_call_result_n381 = hxc_caxecraft_app_GameplayMessage_gameplayMessageId(6);
    hxc_l_text1_hd2550a6ec056 = hxc_l_tmp_call_result_n381;
    hxc_string hxc_l_text2_h9cf1cda7f46c = { 0 };
    switch (hxc_l_locale) {
      case 0:
        {
          hxc_l_text2_h9cf1cda7f46c = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
          if (hxc_string_retain(hxc_l_text2_h9cf1cda7f46c) != HXC_STATUS_OK)
          {
            abort();
          }
          break;
        }
      case 1:
        {
          hxc_l_text2_h9cf1cda7f46c = (hxc_string){ (const uint8_t *)"es-mx", 5, true, NULL };
          if (hxc_string_retain(hxc_l_text2_h9cf1cda7f46c) != HXC_STATUS_OK)
          {
            abort();
          }
          break;
        }
      default:
        {
          hxc_l_text2_h9cf1cda7f46c = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
          if (hxc_string_retain(hxc_l_text2_h9cf1cda7f46c) != HXC_STATUS_OK)
          {
            abort();
          }
          break;
        }
    }
    hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n383 = hxc_l_text2_h9cf1cda7f46c;
    hxc_l_tmp_managed_flow_owner_n132 = hxc_l_tmp_managed_flow_carrier_move_result_n383;
    struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_load_result_n384 = hxc_l_presentation;
    hxc_l_gc_roots[16] = (const void *)hxc_l_tmp_load_result_n384;
    if (hxc_l_tmp_load_result_n384 == NULL)
    {
      abort();
    }
    hxc_string hxc_l_tmp_load_result_n385 = hxc_l_text1_hd2550a6ec056;
    hxc_string hxc_l_tmp_instance_call_result_n387 = hxc_caxecraft_content_RuntimeLevelPresentation_message(hxc_l_tmp_load_result_n384, hxc_l_tmp_load_result_n385, hxc_l_tmp_managed_flow_owner_n132);
    hxc_l_tmp_cstring_ref_owner_owner_n133 = hxc_l_tmp_instance_call_result_n387;
    hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n389 = HXC_BORROWED_CSTRING_INITIALIZER;
    if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n133, &hxc_l_tmp_string_borrow_cstring_result_n389) != HXC_STATUS_OK)
    {
      abort();
    }
    int32_t hxc_l_tmp_load_result_n390 = hxc_l_centerX;
    int32_t hxc_l_tmp_load_result_n391 = hxc_l_centerY;
    DrawText(hxc_l_tmp_string_borrow_cstring_result_n389.data, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n390, 122), (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n391, 42), (int32_t)24, hxc_l_text_hee4c52b2abdd);
    struct Color hxc_l_color_h93153952c6f6 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
    hxc_string hxc_l_tmp_call_result_n394 = hxc_caxecraft_app_GameplayMessage_gameplayMessageId(14);
    hxc_l_text_h0ab14c22d666 = hxc_l_tmp_call_result_n394;
    hxc_string hxc_l_text1_h3e9c6c307e2c = { 0 };
    switch (hxc_l_locale) {
      case 0:
        {
          hxc_l_text1_h3e9c6c307e2c = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
          if (hxc_string_retain(hxc_l_text1_h3e9c6c307e2c) != HXC_STATUS_OK)
          {
            abort();
          }
          break;
        }
      case 1:
        {
          hxc_l_text1_h3e9c6c307e2c = (hxc_string){ (const uint8_t *)"es-mx", 5, true, NULL };
          if (hxc_string_retain(hxc_l_text1_h3e9c6c307e2c) != HXC_STATUS_OK)
          {
            abort();
          }
          break;
        }
      default:
        {
          hxc_l_text1_h3e9c6c307e2c = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
          if (hxc_string_retain(hxc_l_text1_h3e9c6c307e2c) != HXC_STATUS_OK)
          {
            abort();
          }
          break;
        }
    }
    hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n396 = hxc_l_text1_h3e9c6c307e2c;
    hxc_l_tmp_managed_flow_owner_n137 = hxc_l_tmp_managed_flow_carrier_move_result_n396;
    struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_load_result_n397 = hxc_l_presentation;
    hxc_l_gc_roots[17] = (const void *)hxc_l_tmp_load_result_n397;
    if (hxc_l_tmp_load_result_n397 == NULL)
    {
      abort();
    }
    hxc_string hxc_l_tmp_load_result_n398 = hxc_l_text_h0ab14c22d666;
    hxc_string hxc_l_tmp_instance_call_result_n400 = hxc_caxecraft_content_RuntimeLevelPresentation_message(hxc_l_tmp_load_result_n397, hxc_l_tmp_load_result_n398, hxc_l_tmp_managed_flow_owner_n137);
    hxc_l_tmp_cstring_ref_owner_owner_n138 = hxc_l_tmp_instance_call_result_n400;
    hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n402 = HXC_BORROWED_CSTRING_INITIALIZER;
    if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n138, &hxc_l_tmp_string_borrow_cstring_result_n402) != HXC_STATUS_OK)
    {
      abort();
    }
    int32_t hxc_l_tmp_load_result_n403 = hxc_l_centerX;
    int32_t hxc_l_tmp_load_result_n404 = hxc_l_centerY;
    DrawText(hxc_l_tmp_string_borrow_cstring_result_n402.data, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n403, 125), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n404, 10), (int32_t)18, hxc_l_color_h93153952c6f6);
    if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n138) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n137) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_text_h0ab14c22d666) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n133) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n132) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_text1_hd2550a6ec056) != HXC_STATUS_OK)
    {
      abort();
    }
  }
  if (hxc_l_paused)
  {
    hxc_string hxc_l_tmp_record_field_project_n407 = hxc_l_view.hxc_journalTitle;
    if (hxc_l_tmp_record_field_project_n407.data == NULL)
    {
      abort();
    }
    int32_t hxc_l_tmp_string_length_result_n408;
    if (hxc_string_haxe_length(hxc_l_tmp_record_field_project_n407, &hxc_l_tmp_string_length_result_n408) != HXC_STATUS_OK)
    {
      abort();
    }
    bool hxc_l_tmp_short_circuit_result_n140 = hxc_l_tmp_string_length_result_n408 > 0;
    if (!(hxc_l_tmp_string_length_result_n408 > 0))
    {
      hxc_string hxc_l_tmp_record_field_project_n409 = hxc_l_view.hxc_journalBody;
      if (hxc_l_tmp_record_field_project_n409.data == NULL)
      {
        abort();
      }
      int32_t hxc_l_tmp_string_length_result_n410;
      if (hxc_string_haxe_length(hxc_l_tmp_record_field_project_n409, &hxc_l_tmp_string_length_result_n410) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_l_tmp_short_circuit_result_n140 = hxc_l_tmp_string_length_result_n410 > 0;
    }
    bool hxc_l_hasJournal = hxc_l_tmp_short_circuit_result_n140;
    bool hxc_l_tmp_load_result_n412 = hxc_l_hasJournal;
    int32_t hxc_l_tmp_conditional_result_n142 = 0;
    if (hxc_l_tmp_load_result_n412)
    {
      hxc_l_tmp_conditional_result_n142 = hxc_i32_subtract_wrapping(hxc_l_centerX, 330);
    }
    else
    {
      hxc_l_tmp_conditional_result_n142 = hxc_i32_subtract_wrapping(hxc_l_centerX, 170);
    }
    int32_t hxc_l_panelX = hxc_l_tmp_conditional_result_n142;
    bool hxc_l_tmp_load_result_n416 = hxc_l_hasJournal;
    int32_t hxc_l_tmp_conditional_result_n144 = 0;
    if (hxc_l_tmp_load_result_n416)
    {
      hxc_l_tmp_conditional_result_n144 = hxc_i32_subtract_wrapping(hxc_l_centerY, 110);
    }
    else
    {
      hxc_l_tmp_conditional_result_n144 = hxc_i32_subtract_wrapping(hxc_l_centerY, 48);
    }
    int32_t hxc_l_panelY = hxc_l_tmp_conditional_result_n144;
    bool hxc_l_tmp_load_result_n420 = hxc_l_hasJournal;
    int32_t hxc_l_tmp_conditional_result_n146 = 0;
    if (hxc_l_tmp_load_result_n420)
    {
      hxc_l_tmp_conditional_result_n146 = 660;
    }
    else
    {
      hxc_l_tmp_conditional_result_n146 = 340;
    }
    int32_t hxc_l_panelWidth = hxc_l_tmp_conditional_result_n146;
    bool hxc_l_tmp_load_result_n422 = hxc_l_hasJournal;
    int32_t hxc_l_tmp_conditional_result_n148 = 0;
    if (hxc_l_tmp_load_result_n422)
    {
      hxc_l_tmp_conditional_result_n148 = 220;
    }
    else
    {
      hxc_l_tmp_conditional_result_n148 = 96;
    }
    int32_t hxc_l_panelHeight = hxc_l_tmp_conditional_result_n148;
    struct Color hxc_l_this1_had48584ac4da = (struct Color){ .r = 15, .g = 27, .b = 39, .a = 220 };
    int32_t hxc_l_tmp_load_result_n425 = hxc_l_panelX;
    int32_t hxc_l_tmp_load_result_n426 = hxc_l_panelY;
    int32_t hxc_l_tmp_load_result_n427 = hxc_l_panelWidth;
    int32_t hxc_l_tmp_load_result_n428 = hxc_l_panelHeight;
    DrawRectangle((int32_t)hxc_l_tmp_load_result_n425, (int32_t)hxc_l_tmp_load_result_n426, (int32_t)hxc_l_tmp_load_result_n427, (int32_t)hxc_l_tmp_load_result_n428, hxc_l_this1_had48584ac4da);
    struct Color hxc_l_this1_hf5480be824b1 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
    int32_t hxc_l_tmp_load_result_n431 = hxc_l_panelX;
    int32_t hxc_l_tmp_load_result_n432 = hxc_l_panelY;
    int32_t hxc_l_tmp_load_result_n433 = hxc_l_panelWidth;
    int32_t hxc_l_tmp_load_result_n434 = hxc_l_panelHeight;
    DrawRectangleLines((int32_t)hxc_l_tmp_load_result_n431, (int32_t)hxc_l_tmp_load_result_n432, (int32_t)hxc_l_tmp_load_result_n433, (int32_t)hxc_l_tmp_load_result_n434, hxc_l_this1_hf5480be824b1);
    hxc_string hxc_l_tmp_instance_call_result_n437 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_uiCatalog, hxc_l_locale, 32);
    hxc_l_tmp_cstring_ref_owner_owner_n151 = hxc_l_tmp_instance_call_result_n437;
    hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n439 = HXC_BORROWED_CSTRING_INITIALIZER;
    if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n151, &hxc_l_tmp_string_borrow_cstring_result_n439) != HXC_STATUS_OK)
    {
      abort();
    }
    int32_t hxc_l_tmp_load_result_n440 = hxc_l_centerX;
    int32_t hxc_l_tmp_load_result_n441 = hxc_l_panelY;
    DrawText(hxc_l_tmp_string_borrow_cstring_result_n439.data, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n440, 48), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n441, 18), (int32_t)24, hxc_l_text_hee4c52b2abdd);
    if (hxc_l_hasJournal)
    {
      hxc_l_text1_h4b6eb3af3313 = hxc_l_view.hxc_journalTitle;
      if (hxc_string_retain(hxc_l_text1_h4b6eb3af3313) != HXC_STATUS_OK)
      {
        abort();
      }
      struct Color hxc_l_this1_h30b532b90f78 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
      hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n447 = HXC_BORROWED_CSTRING_INITIALIZER;
      if (hxc_string_borrow_cstring(&hxc_l_text1_h4b6eb3af3313, &hxc_l_tmp_string_borrow_cstring_result_n447) != HXC_STATUS_OK)
      {
        abort();
      }
      int32_t hxc_l_tmp_load_result_n448 = hxc_l_panelX;
      int32_t hxc_l_tmp_load_result_n449 = hxc_l_panelY;
      DrawText(hxc_l_tmp_string_borrow_cstring_result_n447.data, (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n448, 30), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n449, 68), (int32_t)18, hxc_l_this1_h30b532b90f78);
      int32_t hxc_l_tmp_load_result_n452 = hxc_l_panelX;
      int32_t hxc_l_tmp_load_result_n453 = hxc_l_panelY;
      hxc_caxecraft_app_CaxecraftApp_drawWrappedText(hxc_l_view.hxc_journalBody, hxc_i32_add_wrapping(hxc_l_tmp_load_result_n452, 30), hxc_i32_add_wrapping(hxc_l_tmp_load_result_n453, 104), 16, 58, 22, 3, hxc_l_text_hee4c52b2abdd);
      if (hxc_string_release(&hxc_l_text1_h4b6eb3af3313) != HXC_STATUS_OK)
      {
        abort();
      }
    }
    hxc_string hxc_l_tmp_instance_call_result_n456 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_uiCatalog, hxc_l_locale, 31);
    hxc_l_tmp_cstring_ref_owner_owner_n154 = hxc_l_tmp_instance_call_result_n456;
    hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n458 = HXC_BORROWED_CSTRING_INITIALIZER;
    if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n154, &hxc_l_tmp_string_borrow_cstring_result_n458) != HXC_STATUS_OK)
    {
      abort();
    }
    int32_t hxc_l_tmp_load_result_n459 = hxc_l_centerX;
    int32_t hxc_l_tmp_load_result_n460 = hxc_l_panelY;
    int32_t hxc_l_tmp_load_result_n461 = hxc_l_panelHeight;
    DrawText(hxc_l_tmp_string_borrow_cstring_result_n458.data, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n459, 160), (int32_t)hxc_i32_subtract_wrapping(hxc_i32_add_wrapping(hxc_l_tmp_load_result_n460, hxc_l_tmp_load_result_n461), 34), (int32_t)16, hxc_l_text_hee4c52b2abdd);
    if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n154) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n151) != HXC_STATUS_OK)
    {
      abort();
    }
  }
  else
  {
    if (hxc_l_placementBlocked)
    {
      struct Color hxc_l_this1_ha97f6a7e3187 = (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 };
      hxc_string hxc_l_tmp_instance_call_result_n466 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_uiCatalog, hxc_l_locale, 33);
      hxc_l_tmp_cstring_ref_owner_owner_n156 = hxc_l_tmp_instance_call_result_n466;
      hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n468 = HXC_BORROWED_CSTRING_INITIALIZER;
      if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n156, &hxc_l_tmp_string_borrow_cstring_result_n468) != HXC_STATUS_OK)
      {
        abort();
      }
      int32_t hxc_l_tmp_load_result_n469 = hxc_l_centerX;
      int32_t hxc_l_tmp_load_result_n470 = hxc_l_centerY;
      DrawText(hxc_l_tmp_string_borrow_cstring_result_n468.data, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n469, 170), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n470, 26), (int32_t)14, hxc_l_this1_ha97f6a7e3187);
      if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n156) != HXC_STATUS_OK)
      {
        abort();
      }
    }
    else
    {
      if (!hxc_l_captured)
      {
        hxc_string hxc_l_tmp_instance_call_result_n474 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_uiCatalog, hxc_l_locale, 2);
        hxc_l_tmp_cstring_ref_owner_owner_n157 = hxc_l_tmp_instance_call_result_n474;
        hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n476 = HXC_BORROWED_CSTRING_INITIALIZER;
        if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n157, &hxc_l_tmp_string_borrow_cstring_result_n476) != HXC_STATUS_OK)
        {
          abort();
        }
        int32_t hxc_l_tmp_load_result_n477 = hxc_l_centerX;
        int32_t hxc_l_tmp_load_result_n478 = hxc_l_centerY;
        DrawText(hxc_l_tmp_string_borrow_cstring_result_n476.data, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n477, 90), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n478, 26), (int32_t)14, hxc_l_text_hee4c52b2abdd);
        if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n157) != HXC_STATUS_OK)
        {
          abort();
        }
      }
      else
      {
        if (!hxc_l_hit.hxc_hit)
        {
          hxc_string hxc_l_tmp_instance_call_result_n482 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_uiCatalog, hxc_l_locale, 30);
          hxc_l_tmp_cstring_ref_owner_owner_n158 = hxc_l_tmp_instance_call_result_n482;
          hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n484 = HXC_BORROWED_CSTRING_INITIALIZER;
          if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n158, &hxc_l_tmp_string_borrow_cstring_result_n484) != HXC_STATUS_OK)
          {
            abort();
          }
          int32_t hxc_l_tmp_load_result_n485 = hxc_l_centerX;
          int32_t hxc_l_tmp_load_result_n486 = hxc_l_centerY;
          DrawText(hxc_l_tmp_string_borrow_cstring_result_n484.data, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n485, 105), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n486, 26), (int32_t)14, hxc_l_text_hee4c52b2abdd);
          if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n158) != HXC_STATUS_OK)
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
  if (hxc_string_release(&hxc_l_text1_hab11a2ba5ff6) != HXC_STATUS_OK)
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

void hxc_caxecraft_app_CaxecraftApp_drawWorldSun(void)
{
  struct Vector3 hxc_l_center = (struct Vector3){ .x = (float)52.0, .y = (float)18.0, .z = (float)-38.0 };
  DrawSphere(hxc_l_center, (float)3.4, (struct Color){ .r = 255, .g = 246, .b = 205, .a = 255 });
  DrawSphere(hxc_l_center, (float)4.8, (struct Color){ .r = 255, .g = 226, .b = 136, .a = 88 });
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

struct hxc_caxecraft_app_CaxecraftApp_AvailableInteractionTarget hxc_caxecraft_app_CaxecraftApp_nearestAvailableInteraction(struct hxc_caxecraft_domain_GameSession *hxc_l_session, struct hxc_caxecraft_app_PlayableLevelView *hxc_l_level)
{
  const void *hxc_l_gc_roots[2] = { (const void *)hxc_l_session, (const void *)hxc_l_level };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 2, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_caxecraft_app_PlayableDialogueActor hxc_l_tmp_array_element_owner_n11 = { 0 };
  hxc_string hxc_l_id_h33cf02b74d7d = { 0 };
  struct hxc_caxecraft_content_LoadedStatefulObjectBinding hxc_l_tmp_array_element_owner_n21 = { 0 };
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
    hxc_l_tmp_array_element_owner_n11 = hxc_l_tmp_array_get_result_n12;
    int32_t hxc_l_id_h56714afcdfb2 = hxc_l_tmp_array_element_owner_n11.hxc_entityId;
    bool hxc_l_tmp_instance_call_result_n16 = hxc_caxecraft_domain_GameSession_actorInteractionAvailable(hxc_l_session, hxc_l_id_h56714afcdfb2);
    if (hxc_l_tmp_instance_call_result_n16)
    {
      struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n18 = hxc_caxecraft_domain_GameSession_readCharacter(hxc_l_session, hxc_l_id_h56714afcdfb2);
      struct hxc_caxecraft_domain_Character hxc_l_actor = hxc_l_tmp_instance_call_result_n18;
      double hxc_l_tmp_record_field_load_result_n19 = hxc_l_actor.hxc_body.hxc_x;
      double hxc_l_dx_h8617ebe438d1 = hxc_l_tmp_record_field_load_result_n19 - hxc_l_player.hxc_body.hxc_x;
      double hxc_l_tmp_record_field_load_result_n21 = hxc_l_actor.hxc_body.hxc_z;
      double hxc_l_dz_h9de9817f4f27 = hxc_l_tmp_record_field_load_result_n21 - hxc_l_player.hxc_body.hxc_z;
      double hxc_l_tmp_load_result_n23 = hxc_l_dx_h8617ebe438d1;
      double hxc_l_tmp_load_result_n24 = hxc_l_dx_h8617ebe438d1;
      double hxc_l_tmp_load_result_n25 = hxc_l_dz_h9de9817f4f27;
      double hxc_l_distance_hc7d13aad7729 = hxc_l_tmp_load_result_n23 * hxc_l_tmp_load_result_n24 + hxc_l_tmp_load_result_n25 * hxc_l_dz_h9de9817f4f27;
      bool hxc_l_tmp_load_result_n27 = hxc_l_hasSelection;
      bool hxc_l_tmp_short_circuit_result_n16 = !hxc_l_tmp_load_result_n27;
      if (!!hxc_l_tmp_load_result_n27)
      {
        double hxc_l_tmp_load_result_n28 = hxc_l_distance_hc7d13aad7729;
        hxc_l_tmp_short_circuit_result_n16 = hxc_l_tmp_load_result_n28 < hxc_l_selectedDistance;
      }
      if (hxc_l_tmp_short_circuit_result_n16)
      {
        int32_t hxc_l_tmp_load_result_n31 = hxc_l_id_h56714afcdfb2;
        hxc_enum_2c0e695d_destroy(&hxc_l_selected);
        hxc_l_selected = (struct hxc_caxecraft_app_CaxecraftApp_AvailableInteractionTarget){ .hxc_tag = hxc_caxecraft_app_CaxecraftApp_AvailableInteractionTarget_DialogueInteraction, .hxc_payload.hxc_DialogueInteraction.hxc_id = hxc_l_tmp_load_result_n31 };
        hxc_l_hasSelection = true;
        hxc_l_selectedDistance = hxc_l_distance_hc7d13aad7729;
      }
    }
    hxc_array_809914b5_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n11);
  }
  int32_t hxc_l_g_he9d1ba1f5d8e = 0;
  struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n34 = (*hxc_l_level).hxc_statefulObjects;
  int32_t hxc_l_tmp_array_length_result_n35;
  if (hxc_array_ref_length(hxc_l_tmp_class_field_load_result_n34, &hxc_l_tmp_array_length_result_n35) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_g1_h545ff4e7dafd = hxc_l_tmp_array_length_result_n35;
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n36 = hxc_l_g_he9d1ba1f5d8e;
    if (!(hxc_l_tmp_load_result_n36 < hxc_l_g1_h545ff4e7dafd))
    {
      break;
    }
    int32_t hxc_l_tmp_increment_load_result_n38 = hxc_l_g_he9d1ba1f5d8e;
    hxc_l_g_he9d1ba1f5d8e = hxc_i32_add_wrapping(hxc_l_tmp_increment_load_result_n38, 1);
    int32_t hxc_l_index_hc82eb94a18eb = hxc_l_tmp_increment_load_result_n38;
    struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n39 = (*hxc_l_level).hxc_statefulObjects;
    struct hxc_caxecraft_content_LoadedStatefulObjectBinding hxc_l_tmp_array_get_result_n41;
    if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n39, (size_t)hxc_l_index_hc82eb94a18eb, &hxc_l_tmp_array_get_result_n41) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_array_element_owner_n21 = hxc_l_tmp_array_get_result_n41;
    hxc_l_id_h33cf02b74d7d = hxc_l_tmp_array_element_owner_n21.hxc_authoredId;
    if (hxc_string_retain(hxc_l_id_h33cf02b74d7d) != HXC_STATUS_OK)
    {
      abort();
    }
    bool hxc_l_tmp_instance_call_result_n45 = hxc_caxecraft_domain_GameSession_statefulObjectInteractionAvailable(hxc_l_session, hxc_l_id_h33cf02b74d7d);
    if (hxc_l_tmp_instance_call_result_n45)
    {
      struct hxc_caxecraft_scenario_ScenarioTransform hxc_l_tmp_instance_call_result_n47 = hxc_caxecraft_app_PlayableLevelView_statefulObjectTransformAt(hxc_l_level, hxc_l_index_hc82eb94a18eb);
      struct hxc_caxecraft_scenario_ScenarioTransform hxc_l_transform = hxc_l_tmp_instance_call_result_n47;
      int32_t hxc_l_tmp_record_field_load_result_n48 = hxc_l_transform.hxc_xMilli;
      double hxc_l_dx_h80303f4628fd = hxc_f64_divide_zero_safe((double)hxc_l_tmp_record_field_load_result_n48, 1000.0) - hxc_l_player.hxc_body.hxc_x;
      int32_t hxc_l_tmp_record_field_load_result_n50 = hxc_l_transform.hxc_zMilli;
      double hxc_l_dz_h2f6487e3c701 = hxc_f64_divide_zero_safe((double)hxc_l_tmp_record_field_load_result_n50, 1000.0) - hxc_l_player.hxc_body.hxc_z;
      double hxc_l_tmp_load_result_n52 = hxc_l_dx_h80303f4628fd;
      double hxc_l_tmp_load_result_n53 = hxc_l_dx_h80303f4628fd;
      double hxc_l_tmp_load_result_n54 = hxc_l_dz_h2f6487e3c701;
      double hxc_l_distance_h8fb6aa6c9ec5 = hxc_l_tmp_load_result_n52 * hxc_l_tmp_load_result_n53 + hxc_l_tmp_load_result_n54 * hxc_l_dz_h2f6487e3c701;
      bool hxc_l_tmp_load_result_n56 = hxc_l_hasSelection;
      bool hxc_l_tmp_short_circuit_result_n26 = !hxc_l_tmp_load_result_n56;
      if (!!hxc_l_tmp_load_result_n56)
      {
        double hxc_l_tmp_load_result_n57 = hxc_l_distance_h8fb6aa6c9ec5;
        hxc_l_tmp_short_circuit_result_n26 = hxc_l_tmp_load_result_n57 < hxc_l_selectedDistance;
      }
      if (hxc_l_tmp_short_circuit_result_n26)
      {
        hxc_string hxc_l_tmp_enum_payload_0_owner_n27 = hxc_l_id_h33cf02b74d7d;
        if (hxc_string_retain(hxc_l_tmp_enum_payload_0_owner_n27) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_string hxc_l_tmp_enum_payload_0_owned_load_result_n61 = hxc_l_tmp_enum_payload_0_owner_n27;
        hxc_enum_2c0e695d_destroy(&hxc_l_selected);
        hxc_l_selected = (struct hxc_caxecraft_app_CaxecraftApp_AvailableInteractionTarget){ .hxc_tag = hxc_caxecraft_app_CaxecraftApp_AvailableInteractionTarget_StatefulObjectInteraction, .hxc_payload.hxc_StatefulObjectInteraction.hxc_id = hxc_l_tmp_enum_payload_0_owned_load_result_n61 };
        hxc_l_hasSelection = true;
        hxc_l_selectedDistance = hxc_l_distance_h8fb6aa6c9ec5;
      }
    }
    if (hxc_string_release(&hxc_l_id_h33cf02b74d7d) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_array_4d8613ea_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n21);
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

enum hxc_caxecraft_app_InteractionPrompt hxc_caxecraft_app_CaxecraftApp_promptForAvailableInteraction(struct hxc_caxecraft_app_CaxecraftApp_AvailableInteractionTarget hxc_l_target)
{
  enum hxc_caxecraft_app_InteractionTargetKind hxc_l_tmp = { 0 };
  switch (hxc_l_target.hxc_tag) {
    case hxc_caxecraft_app_CaxecraftApp_AvailableInteractionTarget_NoAvailableInteraction:
      {
        hxc_l_tmp = hxc_caxecraft_app_InteractionTargetKind_NoInteractionTarget;
        break;
      }
    case hxc_caxecraft_app_CaxecraftApp_AvailableInteractionTarget_DialogueInteraction:
      {
        hxc_l_tmp = hxc_caxecraft_app_InteractionTargetKind_DialogueInteractionTarget;
        break;
      }
    case hxc_caxecraft_app_CaxecraftApp_AvailableInteractionTarget_StatefulObjectInteraction:
      {
        hxc_l_tmp = hxc_caxecraft_app_InteractionTargetKind_MechanismInteractionTarget;
        break;
      }
    default:
      {
        abort();
      }
  }
  enum hxc_caxecraft_app_InteractionPrompt hxc_l_tmp_call_result_n4 = hxc_caxecraft_app_InteractionPrompt_interactionPrompt(hxc_l_tmp);
  return hxc_l_tmp_call_result_n4;
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
  const void *hxc_l_gc_roots[295] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 295, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_string hxc_l_tmp_sys_println_string_argument_owner_n187 = { 0 };
  hxc_string hxc_l_pendingCampaignLabel = { 0 };
  struct hxc_caxecraft_content_CampaignLevelLoadResult hxc_l_g_h2fc2c60edcf1 = { 0 };
  struct hxc_caxecraft_content_CampaignPackageLoadResult hxc_l_g_h1d08383de4f8 = { 0 };
  struct hxc_caxecraft_app_CaxecraftApp_AvailableInteractionTarget hxc_l_tmp_static_call_argument_0_owner_n231 = { 0 };
  struct hxc_array_ref *hxc_l_phases = { 0 };
  hxc_string hxc_l_tmp_string_concat_left_owner_n235 = { 0 };
  hxc_string hxc_l_tmp_string_concat_left_owner_n236 = { 0 };
  hxc_string hxc_l_tmp_string_concat_right_owner_n237 = { 0 };
  hxc_string hxc_l_tmp_sys_println_string_argument_owner_n238 = { 0 };
  struct hxc_caxecraft_content_CampaignLevelLoadError hxc_l_error = { 0 };
  hxc_string hxc_l_tmp_string_concat_right_owner_n240 = { 0 };
  hxc_string hxc_l_tmp_sys_println_string_argument_owner_n241 = { 0 };
  struct hxc_caxecraft_app_CaxecraftApp_AvailableInteractionTarget hxc_l_g_h3c7144a4d6d0 = { 0 };
  struct hxc_caxecraft_content_RuntimeLevelReceipt hxc_l_tmp_record_field_logicalPath_receiver_owner_n25 = { 0 };
  hxc_string hxc_l_id_h5c812fd13c48 = { 0 };
  struct hxc_caxecraft_content_CampaignPackageLoadError hxc_l_gerror = { 0 };
  hxc_string hxc_l_levelLabel = { 0 };
  struct hxc_caxecraft_domain_GameTickResult hxc_l_gameTick = { 0 };
  struct hxc_optional_caxecraft_scenario_FlowTickResult hxc_l_flow = { 0 };
  struct hxc_caxecraft_content_RuntimeLevelReceipt hxc_l_tmp_record_field_logicalPath_receiver_owner_n29 = { 0 };
  struct hxc_array_ref *hxc_l_g1_h86761ddd48e2 = { 0 };
  struct hxc_caxecraft_scenario_FlowPresentationEvent hxc_l_event_h6e7c030f6037 = { 0 };
  struct hxc_caxecraft_scenario_FlowPresentationEvent hxc_l_tmp_array_element_owner_n293 = { 0 };
  hxc_string hxc_l_id_h738abc8a43d4 = { 0 };
  hxc_string hxc_l_id_h8f15d9a191bc = { 0 };
  hxc_string hxc_l_exit = { 0 };
  hxc_string hxc_l_tmp_sys_println_string_argument_owner_n302 = { 0 };
  hxc_string hxc_l_tmp_sys_println_string_argument_owner_n304 = { 0 };
  hxc_string hxc_l_tmp_string_concat_left_owner_n306 = { 0 };
  hxc_string hxc_l_tmp_string_concat_left_owner_n307 = { 0 };
  hxc_string hxc_l_tmp_sys_println_string_argument_owner_n308 = { 0 };
  struct hxc_array_ref *hxc_l_g1_h9d1cad2799a4 = { 0 };
  struct hxc_caxecraft_domain_ActorControllerEvent hxc_l_event_h3bd8b64d343d = { 0 };
  struct hxc_caxecraft_domain_ActorControllerEvent hxc_l_tmp_array_element_owner_n325 = { 0 };
  hxc_string hxc_l_drop = { 0 };
  struct hxc_array_ref *hxc_l_actorPhases = { 0 };
  struct hxc_caxecraft_app_CaxecraftApp_AvailableInteractionTarget hxc_l_tmp_static_call_argument_0_owner_n361 = { 0 };
  struct hxc_array_ref *hxc_l_presentationActorPhases = { 0 };
  struct hxc_array_ref *hxc_l_dialogueActors = { 0 };
  struct hxc_caxecraft_content_RuntimeLevelReceipt hxc_l_receipt = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n373 = { 0 };
  hxc_string hxc_l_adventureTagline = { 0 };
  hxc_string hxc_l_tmp_sys_println_string_argument_owner_n38 = { 0 };
  hxc_string hxc_l_tmp_string_concat_right_owner_n39 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n395 = { 0 };
  hxc_string hxc_l_tmp_sys_println_string_argument_owner_n40 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n403 = { 0 };
  hxc_string hxc_l_tmp_string_concat_right_owner_n41 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n411 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n417 = { 0 };
  hxc_string hxc_l_tmp_sys_println_string_argument_owner_n42 = { 0 };
  hxc_string hxc_l_tmp_cstring_ref_owner_owner_n420 = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n426 = { 0 };
  hxc_string hxc_l_tmp = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n430 = { 0 };
  hxc_string hxc_l_tmp_static_call_argument_9_owner_n431 = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n457 = { 0 };
  hxc_string hxc_l_hudView7 = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n462 = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n463 = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n467 = { 0 };
  hxc_string hxc_l_tmp_managed_flow_owner_n468 = { 0 };
  struct hxc_caxecraft_app_HudView hxc_l_hudView_h05856689f4f8 = { 0 };
  struct hxc_caxecraft_content_RuntimeContentLoadResult hxc_l_g_h4ef171f4fe67 = { 0 };
  hxc_string hxc_l_activeDialogue = { 0 };
  hxc_string hxc_l_latestJournalId = { 0 };
  hxc_string hxc_l_currentObjectiveId = { 0 };
  struct hxc_caxecraft_app_CaxecraftApp_AvailableInteractionTarget hxc_l_tmp_static_call_argument_0_owner_n69 = { 0 };
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
  hxc_l_gc_roots[37] = (const void *)NULL;
  struct hxc_caxecraft_content_CampaignLevel *hxc_l_pendingCampaignLevel = NULL;
  hxc_l_pendingCampaignLabel = (hxc_string){ (const uint8_t *)"", 0, true, NULL };
  if (hxc_string_retain(hxc_l_pendingCampaignLabel) != HXC_STATUS_OK)
  {
    abort();
  }
  bool hxc_l_loadingFramePresented = false;
  hxc_l_gc_roots[38] = (const void *)hxc_l_contentStore;
  struct hxc_caxecraft_content_CampaignPackageLoadResult hxc_l_tmp_call_result_n33 = hxc_caxecraft_content_CampaignRuntime_loadCampaignManifest(hxc_l_contentStore, (hxc_string){ (const uint8_t *)"campaigns/first-adventure/campaign.json", 39, true, NULL });
  hxc_l_gc_roots[39] = hxc_l_tmp_call_result_n33.hxc_tag == hxc_caxecraft_content_CampaignPackageLoadResult_CampaignPackageReady ? (const void *)hxc_l_tmp_call_result_n33.hxc_payload.hxc_CampaignPackageReady.hxc_manifest : NULL;
  hxc_l_g_h1d08383de4f8 = hxc_l_tmp_call_result_n33;
  hxc_l_gc_roots[40] = hxc_l_g_h1d08383de4f8.hxc_tag == hxc_caxecraft_content_CampaignPackageLoadResult_CampaignPackageReady ? (const void *)hxc_l_g_h1d08383de4f8.hxc_payload.hxc_CampaignPackageReady.hxc_manifest : NULL;
  switch (hxc_l_g_h1d08383de4f8.hxc_tag) {
    case hxc_caxecraft_content_CampaignPackageLoadResult_CampaignPackageReady:
      {
        hxc_l_gc_roots[41] = hxc_l_g_h1d08383de4f8.hxc_tag == hxc_caxecraft_content_CampaignPackageLoadResult_CampaignPackageReady ? (const void *)hxc_l_g_h1d08383de4f8.hxc_payload.hxc_CampaignPackageReady.hxc_manifest : NULL;
        if (hxc_l_g_h1d08383de4f8.hxc_tag != hxc_caxecraft_content_CampaignPackageLoadResult_CampaignPackageReady)
        {
          abort();
        }
        struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_enum_payload_project_n36 = hxc_l_g_h1d08383de4f8.hxc_payload.hxc_CampaignPackageReady.hxc_manifest;
        hxc_l_gc_roots[42] = (const void *)hxc_l_tmp_enum_payload_project_n36;
        struct hxc_caxecraft_content_CampaignManifest *hxc_l_manifest = hxc_l_tmp_enum_payload_project_n36;
        struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n37 = hxc_l_manifest;
        hxc_l_gc_roots[43] = (const void *)hxc_l_tmp_load_result_n37;
        if (hxc_l_tmp_load_result_n37 == NULL)
        {
          abort();
        }
        struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n38 = (*hxc_l_tmp_load_result_n37).hxc_admittedLevels;
        hxc_l_gc_roots[44] = (const void *)hxc_l_tmp_class_field_load_result_n38;
        struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n39 = hxc_l_manifest;
        hxc_l_gc_roots[45] = (const void *)hxc_l_tmp_load_result_n39;
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
        hxc_l_gc_roots[46] = (const void *)hxc_l_tmp_array_get_result_n41;
        struct hxc_caxecraft_content_CampaignLevel *hxc_l_entry = hxc_l_tmp_array_get_result_n41;
        struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_load_result_n42 = hxc_l_entry;
        hxc_l_gc_roots[47] = (const void *)hxc_l_tmp_load_result_n42;
        if (hxc_l_tmp_load_result_n42 == NULL)
        {
          abort();
        }
        hxc_string hxc_l_tmp_class_field_load_result_n43 = (*hxc_l_tmp_load_result_n42).hxc_logicalPath;
        struct hxc_caxecraft_content_RuntimeLevelCandidate *hxc_l_tmp_load_result_n44 = hxc_l_loadedCandidate;
        hxc_l_gc_roots[48] = (const void *)hxc_l_tmp_load_result_n44;
        if (hxc_l_tmp_load_result_n44 == NULL)
        {
          abort();
        }
        struct hxc_caxecraft_content_RuntimeLevelReceipt hxc_l_tmp_instance_call_result_n45 = hxc_caxecraft_content_RuntimeLevelCandidate_receipt(hxc_l_tmp_load_result_n44);
        hxc_l_tmp_record_field_logicalPath_receiver_owner_n25 = hxc_l_tmp_instance_call_result_n45;
        struct hxc_caxecraft_content_RuntimeLevelReceipt hxc_l_tmp_record_field_logicalPath_receiver_borrow_result_n46 = hxc_l_tmp_record_field_logicalPath_receiver_owner_n25;
        hxc_record_f6127605_destroy(&hxc_l_tmp_record_field_logicalPath_receiver_owner_n25);
        if (!(hxc_l_tmp_class_field_load_result_n43.data == NULL || hxc_l_tmp_record_field_logicalPath_receiver_borrow_result_n46.hxc_logicalPath.data == NULL ? hxc_l_tmp_class_field_load_result_n43.data == hxc_l_tmp_record_field_logicalPath_receiver_borrow_result_n46.hxc_logicalPath.data : hxc_l_tmp_class_field_load_result_n43.byte_length == hxc_l_tmp_record_field_logicalPath_receiver_borrow_result_n46.hxc_logicalPath.byte_length && (hxc_l_tmp_class_field_load_result_n43.byte_length == 0 || memcmp(hxc_l_tmp_class_field_load_result_n43.data, hxc_l_tmp_record_field_logicalPath_receiver_borrow_result_n46.hxc_logicalPath.data, hxc_l_tmp_class_field_load_result_n43.byte_length) == 0)))
        {
          if (hxc_io_println((hxc_string){ (const uint8_t *)"caxecraft: campaign entry does not match the active map", 55, true, NULL }) != HXC_STATUS_OK)
          {
            abort();
          }
          hxc_enum_4b68fff2_destroy(&hxc_l_g_h1d08383de4f8);
          if (hxc_string_release(&hxc_l_pendingCampaignLabel) != HXC_STATUS_OK)
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
        else
        {
          hxc_l_gc_roots[50] = (const void *)hxc_l_manifest;
          hxc_l_campaign = hxc_l_manifest;
          hxc_l_gc_roots[51] = (const void *)hxc_l_entry;
          hxc_l_campaignLevel = hxc_l_entry;
        }
        break;
      }
    case hxc_caxecraft_content_CampaignPackageLoadResult_CampaignPackageRejected:
      {
        hxc_l_gc_roots[49] = hxc_l_g_h1d08383de4f8.hxc_tag == hxc_caxecraft_content_CampaignPackageLoadResult_CampaignPackageReady ? (const void *)hxc_l_g_h1d08383de4f8.hxc_payload.hxc_CampaignPackageReady.hxc_manifest : NULL;
        if (hxc_l_g_h1d08383de4f8.hxc_tag != hxc_caxecraft_content_CampaignPackageLoadResult_CampaignPackageRejected)
        {
          abort();
        }
        struct hxc_caxecraft_content_CampaignPackageLoadError hxc_l_tmp_enum_payload_project_n52 = hxc_l_g_h1d08383de4f8.hxc_payload.hxc_CampaignPackageRejected.hxc_error;
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
            hxc_enum_4b68fff2_destroy(&hxc_l_g_h1d08383de4f8);
            if (hxc_string_release(&hxc_l_pendingCampaignLabel) != HXC_STATUS_OK)
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
        }
        else
        {
          if (hxc_io_println((hxc_string){ (const uint8_t *)"caxecraft: campaign manifest rejected: campaigns/first-adventure/campaign.json", 78, true, NULL }) != HXC_STATUS_OK)
          {
            abort();
          }
          hxc_enum_7e02e9cb_destroy(&hxc_l_gerror);
          hxc_enum_4b68fff2_destroy(&hxc_l_g_h1d08383de4f8);
          if (hxc_string_release(&hxc_l_pendingCampaignLabel) != HXC_STATUS_OK)
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
        break;
      }
    default:
      {
        abort();
      }
  }
  struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_load_result_n56 = hxc_l_campaignLevel;
  hxc_l_gc_roots[52] = (const void *)hxc_l_tmp_load_result_n56;
  hxc_l_gc_roots[53] = (const void *)NULL;
  hxc_string hxc_l_tmp_conditional_managed_result_n28 = { 0 };
  if (hxc_l_tmp_load_result_n56 == NULL)
  {
    struct hxc_caxecraft_content_RuntimeLevelCandidate *hxc_l_tmp_load_result_n57 = hxc_l_loadedCandidate;
    hxc_l_gc_roots[54] = (const void *)hxc_l_tmp_load_result_n57;
    if (hxc_l_tmp_load_result_n57 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_content_RuntimeLevelReceipt hxc_l_tmp_instance_call_result_n58 = hxc_caxecraft_content_RuntimeLevelCandidate_receipt(hxc_l_tmp_load_result_n57);
    hxc_l_tmp_record_field_logicalPath_receiver_owner_n29 = hxc_l_tmp_instance_call_result_n58;
    (void)hxc_l_tmp_record_field_logicalPath_receiver_owner_n29.hxc_logicalPath;
    hxc_l_tmp_conditional_managed_result_n28 = hxc_l_tmp_record_field_logicalPath_receiver_owner_n29.hxc_logicalPath;
    if (hxc_string_retain(hxc_l_tmp_conditional_managed_result_n28) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_record_f6127605_destroy(&hxc_l_tmp_record_field_logicalPath_receiver_owner_n29);
  }
  else
  {
    struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_load_result_n61 = hxc_l_campaignLevel;
    hxc_l_gc_roots[55] = (const void *)hxc_l_tmp_load_result_n61;
    if (hxc_l_tmp_load_result_n61 == NULL)
    {
      abort();
    }
    hxc_string hxc_l_tmp_class_field_load_result_n62 = (*hxc_l_tmp_load_result_n61).hxc_id;
    (void)hxc_l_tmp_class_field_load_result_n62;
    hxc_l_tmp_conditional_managed_result_n28 = hxc_l_tmp_class_field_load_result_n62;
    if (hxc_string_retain(hxc_l_tmp_conditional_managed_result_n28) != HXC_STATUS_OK)
    {
      abort();
    }
  }
  hxc_string hxc_l_tmp_conditional_managed_move_result_n63 = hxc_l_tmp_conditional_managed_result_n28;
  hxc_l_levelLabel = hxc_l_tmp_conditional_managed_move_result_n63;
  int32_t hxc_l_selectedCampaignLevelIndex = 0;
  struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n64 = hxc_l_campaign;
  hxc_l_gc_roots[56] = (const void *)hxc_l_tmp_load_result_n64;
  hxc_l_gc_roots[57] = (const void *)NULL;
  bool hxc_l_tmp_short_circuit_result_n31 = hxc_l_tmp_load_result_n64 != NULL;
  if (hxc_l_tmp_load_result_n64 != NULL)
  {
    hxc_l_gc_roots[58] = (const void *)hxc_l_campaignLevel;
    hxc_l_gc_roots[59] = (const void *)NULL;
    hxc_l_tmp_short_circuit_result_n31 = hxc_l_campaignLevel != NULL;
  }
  if (hxc_l_tmp_short_circuit_result_n31)
  {
    int32_t hxc_l_g_hae784fdd09a4 = 0;
    struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n67 = hxc_l_campaign;
    hxc_l_gc_roots[60] = (const void *)hxc_l_tmp_load_result_n67;
    if (hxc_l_tmp_load_result_n67 == NULL)
    {
      abort();
    }
    struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n68 = (*hxc_l_tmp_load_result_n67).hxc_admittedLevels;
    hxc_l_gc_roots[61] = (const void *)hxc_l_tmp_class_field_load_result_n68;
    int32_t hxc_l_tmp_array_length_result_n69;
    if (hxc_array_ref_length(hxc_l_tmp_class_field_load_result_n68, &hxc_l_tmp_array_length_result_n69) != HXC_STATUS_OK)
    {
      abort();
    }
    int32_t hxc_l_g1_h57e82d4c3c59 = hxc_l_tmp_array_length_result_n69;
    while (1)
    {
      int32_t hxc_l_tmp_load_result_n70 = hxc_l_g_hae784fdd09a4;
      if (!(hxc_l_tmp_load_result_n70 < hxc_l_g1_h57e82d4c3c59))
      {
        break;
      }
      int32_t hxc_l_tmp_increment_load_result_n72 = hxc_l_g_hae784fdd09a4;
      hxc_l_g_hae784fdd09a4 = hxc_i32_add_wrapping(hxc_l_tmp_increment_load_result_n72, 1);
      int32_t hxc_l_index_h077fea9796b1 = hxc_l_tmp_increment_load_result_n72;
      struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n73 = hxc_l_campaign;
      hxc_l_gc_roots[72] = (const void *)hxc_l_tmp_load_result_n73;
      if (hxc_l_tmp_load_result_n73 == NULL)
      {
        abort();
      }
      struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n74 = (*hxc_l_tmp_load_result_n73).hxc_admittedLevels;
      hxc_l_gc_roots[73] = (const void *)hxc_l_tmp_class_field_load_result_n74;
      struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_array_get_result_n76;
      if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n74, (size_t)hxc_l_index_h077fea9796b1, &hxc_l_tmp_array_get_result_n76) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_l_gc_roots[74] = (const void *)hxc_l_tmp_array_get_result_n76;
      if (hxc_l_tmp_array_get_result_n76 == NULL)
      {
        abort();
      }
      hxc_string hxc_l_tmp_class_field_load_result_n77 = (*hxc_l_tmp_array_get_result_n76).hxc_id;
      struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_load_result_n78 = hxc_l_campaignLevel;
      hxc_l_gc_roots[75] = (const void *)hxc_l_tmp_load_result_n78;
      if (hxc_l_tmp_load_result_n78 == NULL)
      {
        abort();
      }
      hxc_string hxc_l_tmp_class_field_load_result_n79 = (*hxc_l_tmp_load_result_n78).hxc_id;
      if (hxc_l_tmp_class_field_load_result_n77.data == NULL || hxc_l_tmp_class_field_load_result_n79.data == NULL ? hxc_l_tmp_class_field_load_result_n77.data == hxc_l_tmp_class_field_load_result_n79.data : hxc_l_tmp_class_field_load_result_n77.byte_length == hxc_l_tmp_class_field_load_result_n79.byte_length && (hxc_l_tmp_class_field_load_result_n77.byte_length == 0 || memcmp(hxc_l_tmp_class_field_load_result_n77.data, hxc_l_tmp_class_field_load_result_n79.data, hxc_l_tmp_class_field_load_result_n77.byte_length) == 0))
      {
        hxc_l_selectedCampaignLevelIndex = hxc_l_index_h077fea9796b1;
      }
    }
  }
  struct hxc_caxecraft_app_ActivePlayableLevel *hxc_l_tmp_load_result_n82 = hxc_l_activeLevel;
  hxc_l_gc_roots[62] = (const void *)hxc_l_tmp_load_result_n82;
  if (hxc_l_tmp_load_result_n82 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_class_field_load_result_n83 = (*hxc_l_tmp_load_result_n82).hxc_activeView;
  hxc_l_gc_roots[63] = (const void *)hxc_l_tmp_class_field_load_result_n83;
  struct hxc_caxecraft_app_PlayableLevelView *hxc_l_initialLevel = hxc_l_tmp_class_field_load_result_n83;
  struct hxc_caxecraft_app_ActivePlayableLevel *hxc_l_tmp_load_result_n84 = hxc_l_activeLevel;
  hxc_l_gc_roots[64] = (const void *)hxc_l_tmp_load_result_n84;
  if (hxc_l_tmp_load_result_n84 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_content_ActiveContent *hxc_l_tmp_class_field_load_result_n85 = (*hxc_l_tmp_load_result_n84).hxc_content;
  hxc_l_gc_roots[65] = (const void *)hxc_l_tmp_class_field_load_result_n85;
  if (hxc_l_tmp_class_field_load_result_n85 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_content_LoadedContentGeneration *hxc_l_tmp_class_field_load_result_n86 = (*hxc_l_tmp_class_field_load_result_n85).hxc_active;
  hxc_l_gc_roots[66] = (const void *)hxc_l_tmp_class_field_load_result_n86;
  if (hxc_l_tmp_class_field_load_result_n86 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_class_field_load_result_n87 = (*hxc_l_tmp_class_field_load_result_n86).hxc_ownedSession;
  hxc_l_gc_roots[67] = (const void *)hxc_l_tmp_class_field_load_result_n87;
  struct hxc_caxecraft_domain_GameSession *hxc_l_initialSession = hxc_l_tmp_class_field_load_result_n87;
  struct hxc_caxecraft_content_RuntimeLevelCandidate *hxc_l_tmp_load_result_n88 = hxc_l_loadedCandidate;
  hxc_l_gc_roots[68] = (const void *)hxc_l_tmp_load_result_n88;
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
  hxc_l_tmp_sys_println_string_argument_owner_n38 = hxc_l_tmp_string_concat_result_n91;
  if (hxc_io_println(hxc_l_tmp_sys_println_string_argument_owner_n38) != HXC_STATUS_OK)
  {
    if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n38) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_record_f6127605_destroy(&hxc_l_receipt);
    if (hxc_string_release(&hxc_l_levelLabel) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_enum_4b68fff2_destroy(&hxc_l_g_h1d08383de4f8);
    if (hxc_string_release(&hxc_l_pendingCampaignLabel) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_enum_fcb4401b_destroy(&hxc_l_g_h4ef171f4fe67);
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    abort();
  }
  if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n38) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_string hxc_l_tmp_std_string_int_result_n94 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
  if (hxc_string_from_int32(hxc_l_receipt.hxc_inputHash, hxc_default_allocator(), &hxc_l_tmp_std_string_int_result_n94) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_tmp_string_concat_right_owner_n39 = hxc_l_tmp_std_string_int_result_n94;
  hxc_string hxc_l_tmp_string_concat_result_n96 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
  if (hxc_string_concat_ref((hxc_string){ (const uint8_t *)"caxecraft: content-input-hash=", 30, true, NULL }, hxc_l_tmp_string_concat_right_owner_n39, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n96) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_tmp_sys_println_string_argument_owner_n40 = hxc_l_tmp_string_concat_result_n96;
  if (hxc_io_println(hxc_l_tmp_sys_println_string_argument_owner_n40) != HXC_STATUS_OK)
  {
    if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n40) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n39) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_record_f6127605_destroy(&hxc_l_receipt);
    if (hxc_string_release(&hxc_l_levelLabel) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_enum_4b68fff2_destroy(&hxc_l_g_h1d08383de4f8);
    if (hxc_string_release(&hxc_l_pendingCampaignLabel) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_enum_fcb4401b_destroy(&hxc_l_g_h4ef171f4fe67);
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    abort();
  }
  if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n40) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n39) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_caxecraft_content_RuntimeContentGeneration *hxc_l_tmp_load_result_n98 = hxc_l_runtimeContent;
  hxc_l_gc_roots[69] = (const void *)hxc_l_tmp_load_result_n98;
  if (hxc_l_tmp_load_result_n98 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_content_RuntimeLevelCandidate *hxc_l_tmp_class_field_load_result_n99 = (*hxc_l_tmp_load_result_n98).hxc_loadedLevel;
  hxc_l_gc_roots[70] = (const void *)hxc_l_tmp_class_field_load_result_n99;
  if (hxc_l_tmp_class_field_load_result_n99 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_content_LoadedContentGeneration *hxc_l_tmp_class_field_load_result_n100 = (*hxc_l_tmp_class_field_load_result_n99).hxc_loadedGeneration;
  hxc_l_gc_roots[71] = (const void *)hxc_l_tmp_class_field_load_result_n100;
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
  hxc_l_tmp_string_concat_right_owner_n41 = hxc_l_tmp_std_string_int_result_n102;
  hxc_string hxc_l_tmp_string_concat_result_n104 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
  if (hxc_string_concat_ref((hxc_string){ (const uint8_t *)"caxecraft: content-generation=", 30, true, NULL }, hxc_l_tmp_string_concat_right_owner_n41, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n104) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_tmp_sys_println_string_argument_owner_n42 = hxc_l_tmp_string_concat_result_n104;
  if (hxc_io_println(hxc_l_tmp_sys_println_string_argument_owner_n42) != HXC_STATUS_OK)
  {
    if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n42) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n41) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_record_f6127605_destroy(&hxc_l_receipt);
    if (hxc_string_release(&hxc_l_levelLabel) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_enum_4b68fff2_destroy(&hxc_l_g_h1d08383de4f8);
    if (hxc_string_release(&hxc_l_pendingCampaignLabel) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_enum_fcb4401b_destroy(&hxc_l_g_h4ef171f4fe67);
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    abort();
  }
  if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n42) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n41) != HXC_STATUS_OK)
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
    hxc_enum_4b68fff2_destroy(&hxc_l_g_h1d08383de4f8);
    if (hxc_string_release(&hxc_l_pendingCampaignLabel) != HXC_STATUS_OK)
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
  SetWindowMinSize((int32_t)800, (int32_t)450);
  SetExitKey((int32_t)0);
  SetTargetFPS((int32_t)120);
  EnableCursor();
  hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n109 = HXC_BORROWED_CSTRING_INITIALIZER;
  if (hxc_string_borrow_cstring(&(hxc_string){ (const uint8_t *)"assets/showcase/title-panorama.png", 34, true, NULL }, &hxc_l_tmp_string_borrow_cstring_result_n109) != HXC_STATUS_OK)
  {
    abort();
  }
  struct Texture hxc_l_tmp_native_call_result_n110 = LoadTexture(hxc_l_tmp_string_borrow_cstring_result_n109.data);
  struct Texture hxc_l_titleTexture = hxc_l_tmp_native_call_result_n110;
  bool hxc_l_tmp_native_call_result_n112 = IsTextureValid(hxc_l_titleTexture);
  bool hxc_l_titleTextureReady = hxc_l_tmp_native_call_result_n112;
  hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n113 = HXC_BORROWED_CSTRING_INITIALIZER;
  if (hxc_string_borrow_cstring(&(hxc_string){ (const uint8_t *)"assets/branding/caxecraft-wordmark.png", 38, true, NULL }, &hxc_l_tmp_string_borrow_cstring_result_n113) != HXC_STATUS_OK)
  {
    abort();
  }
  struct Texture hxc_l_tmp_native_call_result_n114 = LoadTexture(hxc_l_tmp_string_borrow_cstring_result_n113.data);
  struct Texture hxc_l_wordmarkTexture = hxc_l_tmp_native_call_result_n114;
  bool hxc_l_tmp_native_call_result_n116 = IsTextureValid(hxc_l_wordmarkTexture);
  bool hxc_l_wordmarkTextureReady = hxc_l_tmp_native_call_result_n116;
  hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n117 = HXC_BORROWED_CSTRING_INITIALIZER;
  if (hxc_string_borrow_cstring(&(hxc_string){ (const uint8_t *)"assets/atlases/hud.png", 22, true, NULL }, &hxc_l_tmp_string_borrow_cstring_result_n117) != HXC_STATUS_OK)
  {
    abort();
  }
  struct Texture hxc_l_tmp_native_call_result_n118 = LoadTexture(hxc_l_tmp_string_borrow_cstring_result_n117.data);
  struct Texture hxc_l_hudTexture = hxc_l_tmp_native_call_result_n118;
  bool hxc_l_tmp_native_call_result_n120 = IsTextureValid(hxc_l_hudTexture);
  bool hxc_l_hudTextureReady = hxc_l_tmp_native_call_result_n120;
  hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n121 = HXC_BORROWED_CSTRING_INITIALIZER;
  if (hxc_string_borrow_cstring(&(hxc_string){ (const uint8_t *)"assets/atlases/items.png", 24, true, NULL }, &hxc_l_tmp_string_borrow_cstring_result_n121) != HXC_STATUS_OK)
  {
    abort();
  }
  struct Texture hxc_l_tmp_native_call_result_n122 = LoadTexture(hxc_l_tmp_string_borrow_cstring_result_n121.data);
  struct Texture hxc_l_itemTexture = hxc_l_tmp_native_call_result_n122;
  bool hxc_l_tmp_native_call_result_n124 = IsTextureValid(hxc_l_itemTexture);
  bool hxc_l_itemTextureReady = hxc_l_tmp_native_call_result_n124;
  hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n125 = HXC_BORROWED_CSTRING_INITIALIZER;
  if (hxc_string_borrow_cstring(&(hxc_string){ (const uint8_t *)"assets/atlases/adventure-items.png", 34, true, NULL }, &hxc_l_tmp_string_borrow_cstring_result_n125) != HXC_STATUS_OK)
  {
    abort();
  }
  struct Texture hxc_l_tmp_native_call_result_n126 = LoadTexture(hxc_l_tmp_string_borrow_cstring_result_n125.data);
  struct Texture hxc_l_adventureItemTexture = hxc_l_tmp_native_call_result_n126;
  bool hxc_l_tmp_native_call_result_n128 = IsTextureValid(hxc_l_adventureItemTexture);
  bool hxc_l_adventureItemTextureReady = hxc_l_tmp_native_call_result_n128;
  struct Texture hxc_l_tmp_load_result_n129 = hxc_l_hudTexture;
  bool hxc_l_tmp_load_result_n130 = hxc_l_hudTextureReady;
  struct Texture hxc_l_tmp_load_result_n131 = hxc_l_itemTexture;
  bool hxc_l_tmp_load_result_n132 = hxc_l_itemTextureReady;
  struct Texture hxc_l_tmp_load_result_n133 = hxc_l_adventureItemTexture;
  struct hxc_caxecraft_app_HudResources hxc_l_hudResources = (struct hxc_caxecraft_app_HudResources){ .hxc_adventureItemTexture = hxc_l_tmp_load_result_n133, .hxc_adventureItemTextureReady = hxc_l_adventureItemTextureReady, .hxc_hudTexture = hxc_l_tmp_load_result_n129, .hxc_hudTextureReady = hxc_l_tmp_load_result_n130, .hxc_itemTexture = hxc_l_tmp_load_result_n131, .hxc_itemTextureReady = hxc_l_tmp_load_result_n132 };
  hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n136 = HXC_BORROWED_CSTRING_INITIALIZER;
  if (hxc_string_borrow_cstring(&(hxc_string){ (const uint8_t *)"assets/atlases/entities.png", 27, true, NULL }, &hxc_l_tmp_string_borrow_cstring_result_n136) != HXC_STATUS_OK)
  {
    abort();
  }
  struct Texture hxc_l_tmp_native_call_result_n137 = LoadTexture(hxc_l_tmp_string_borrow_cstring_result_n136.data);
  struct Texture hxc_l_entityTexture = hxc_l_tmp_native_call_result_n137;
  bool hxc_l_tmp_native_call_result_n139 = IsTextureValid(hxc_l_entityTexture);
  bool hxc_l_entityTextureReady = hxc_l_tmp_native_call_result_n139;
  hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n140 = HXC_BORROWED_CSTRING_INITIALIZER;
  if (hxc_string_borrow_cstring(&(hxc_string){ (const uint8_t *)"assets/atlases/terrain.png", 26, true, NULL }, &hxc_l_tmp_string_borrow_cstring_result_n140) != HXC_STATUS_OK)
  {
    abort();
  }
  struct Texture hxc_l_tmp_native_call_result_n141 = LoadTexture(hxc_l_tmp_string_borrow_cstring_result_n140.data);
  struct Texture hxc_l_terrainTexture = hxc_l_tmp_native_call_result_n141;
  bool hxc_l_tmp_native_call_result_n143 = IsTextureValid(hxc_l_terrainTexture);
  bool hxc_l_terrainTextureReady = hxc_l_tmp_native_call_result_n143;
  hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n144 = HXC_BORROWED_CSTRING_INITIALIZER;
  if (hxc_string_borrow_cstring(&(hxc_string){ (const uint8_t *)"assets/atlases/adventure-terrain.png", 36, true, NULL }, &hxc_l_tmp_string_borrow_cstring_result_n144) != HXC_STATUS_OK)
  {
    abort();
  }
  struct Texture hxc_l_tmp_native_call_result_n145 = LoadTexture(hxc_l_tmp_string_borrow_cstring_result_n144.data);
  struct Texture hxc_l_adventureTerrainTexture = hxc_l_tmp_native_call_result_n145;
  bool hxc_l_tmp_native_call_result_n147 = IsTextureValid(hxc_l_adventureTerrainTexture);
  bool hxc_l_adventureTerrainTextureReady = hxc_l_tmp_native_call_result_n147;
  struct hxc_caxecraft_app_RuntimeTextureAtlasCatalog *hxc_l_tmp_call_result_n148 = hxc_caxecraft_app_RuntimeTextureAtlasCatalog_load((hxc_string){ (const uint8_t *)"assets/caxecraft-runtime-assets.json", 36, true, NULL });
  hxc_l_gc_roots[76] = (const void *)hxc_l_tmp_call_result_n148;
  struct hxc_caxecraft_app_RuntimeTextureAtlasCatalog *hxc_l_loadedRuntimeTextures = hxc_l_tmp_call_result_n148;
  hxc_l_gc_roots[77] = (const void *)hxc_l_loadedRuntimeTextures;
  hxc_l_gc_roots[78] = (const void *)NULL;
  if (!(hxc_l_loadedRuntimeTextures == NULL))
  {
    hxc_l_gc_roots[79] = (const void *)hxc_l_loadedRuntimeTextures;
    struct hxc_caxecraft_app_RuntimeTextureAtlasCatalog *hxc_l_runtimeTextures = hxc_l_loadedRuntimeTextures;
    double hxc_l_cameraWaterBlend = 0.0;
    struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n167 = hxc_caxecraft_gameplay_Inventory_starter();
    struct hxc_caxecraft_gameplay_InventoryState hxc_l_inventory = hxc_l_tmp_call_result_n167;
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
    struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n168 = hxc_l_initialLevel;
    hxc_l_gc_roots[80] = (const void *)hxc_l_tmp_load_result_n168;
    if (hxc_l_tmp_load_result_n168 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_class_field_load_result_n169 = (*hxc_l_tmp_load_result_n168).hxc_levelPresentation;
    hxc_l_gc_roots[81] = (const void *)hxc_l_tmp_class_field_load_result_n169;
    if (hxc_l_tmp_class_field_load_result_n169 == NULL)
    {
      abort();
    }
    hxc_string hxc_l_tmp_class_field_load_result_n170 = (*hxc_l_tmp_class_field_load_result_n169).hxc_startingObjective;
    hxc_l_currentObjectiveId = hxc_l_tmp_class_field_load_result_n170;
    if (hxc_string_retain(hxc_l_currentObjectiveId) != HXC_STATUS_OK)
    {
      abort();
    }
    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n171 = hxc_l_initialSession;
    hxc_l_gc_roots[82] = (const void *)hxc_l_tmp_load_result_n171;
    hxc_l_gc_roots[83] = (const void *)hxc_l_initialLevel;
    struct hxc_caxecraft_app_CaxecraftApp_AvailableInteractionTarget hxc_l_tmp_call_result_n173 = hxc_caxecraft_app_CaxecraftApp_nearestAvailableInteraction(hxc_l_tmp_load_result_n171, hxc_l_initialLevel);
    hxc_l_tmp_static_call_argument_0_owner_n69 = hxc_l_tmp_call_result_n173;
    enum hxc_caxecraft_app_InteractionPrompt hxc_l_tmp_call_result_n175 = hxc_caxecraft_app_CaxecraftApp_promptForAvailableInteraction(hxc_l_tmp_static_call_argument_0_owner_n69);
    enum hxc_caxecraft_app_InteractionPrompt hxc_l_availableInteractionPrompt = hxc_l_tmp_call_result_n175;
    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n176 = hxc_l_initialSession;
    hxc_l_gc_roots[84] = (const void *)hxc_l_tmp_load_result_n176;
    if (hxc_l_tmp_load_result_n176 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n177 = hxc_l_initialLevel;
    hxc_l_gc_roots[85] = (const void *)hxc_l_tmp_load_result_n177;
    if (hxc_l_tmp_load_result_n177 == NULL)
    {
      abort();
    }
    int32_t hxc_l_tmp_class_field_load_result_n178 = (*hxc_l_tmp_load_result_n177).hxc_enemyId;
    struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n179 = hxc_caxecraft_domain_GameSession_readCharacter(hxc_l_tmp_load_result_n176, hxc_l_tmp_class_field_load_result_n178);
    struct hxc_caxecraft_domain_Character hxc_l_enemyActor = hxc_l_tmp_instance_call_result_n179;
    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n180 = hxc_l_initialSession;
    hxc_l_gc_roots[86] = (const void *)hxc_l_tmp_load_result_n180;
    if (hxc_l_tmp_load_result_n180 == NULL)
    {
      abort();
    }
    struct hxc_array_ref *hxc_l_tmp_instance_call_result_n181 = hxc_caxecraft_domain_GameSession_actorControllerStateSnapshots(hxc_l_tmp_load_result_n180);
    hxc_l_initialActorPhases = hxc_l_tmp_instance_call_result_n181;
    struct hxc_array_ref *hxc_l_tmp_load_result_n182 = hxc_l_initialActorPhases;
    struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n183 = hxc_l_initialLevel;
    hxc_l_gc_roots[87] = (const void *)hxc_l_tmp_load_result_n183;
    if (hxc_l_tmp_load_result_n183 == NULL)
    {
      abort();
    }
    int32_t hxc_l_tmp_class_field_load_result_n184 = (*hxc_l_tmp_load_result_n183).hxc_enemyId;
    struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation hxc_l_tmp_call_result_n186 = hxc_caxecraft_app_CaxecraftApp_observeActorPhase(hxc_l_tmp_load_result_n182, hxc_l_tmp_class_field_load_result_n184, hxc_caxecraft_domain_ActorControllerPhase_Resting);
    struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation hxc_l_enemyPhase_h695cd3d136cc = hxc_l_tmp_call_result_n186;
    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n187 = hxc_l_initialSession;
    hxc_l_gc_roots[88] = (const void *)hxc_l_tmp_load_result_n187;
    struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n188 = hxc_l_initialLevel;
    hxc_l_gc_roots[89] = (const void *)hxc_l_tmp_load_result_n188;
    bool hxc_l_tmp_call_result_n190 = hxc_caxecraft_app_CaxecraftApp_dialogueActorsAreValid(hxc_l_tmp_load_result_n187, hxc_l_tmp_load_result_n188, hxc_l_initialActorPhases);
    bool hxc_l_tmp_short_circuit_result_n73 = !hxc_l_tmp_call_result_n190;
    if (!!hxc_l_tmp_call_result_n190)
    {
      hxc_l_tmp_short_circuit_result_n73 = hxc_l_enemyActor.hxc_id <= 0;
    }
    bool hxc_l_tmp_short_circuit_load_result_n192 = hxc_l_tmp_short_circuit_result_n73;
    bool hxc_l_tmp_short_circuit_result_n74 = hxc_l_tmp_short_circuit_load_result_n192;
    if (!hxc_l_tmp_short_circuit_load_result_n192)
    {
      hxc_l_tmp_short_circuit_result_n74 = !hxc_l_enemyPhase_h695cd3d136cc.hxc_valid;
    }
    if (hxc_l_tmp_short_circuit_result_n74)
    {
      if (hxc_array_ref_release(hxc_l_initialActorPhases) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_enum_2c0e695d_destroy(&hxc_l_tmp_static_call_argument_0_owner_n69);
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
      hxc_enum_4b68fff2_destroy(&hxc_l_g_h1d08383de4f8);
      if (hxc_string_release(&hxc_l_pendingCampaignLabel) != HXC_STATUS_OK)
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
    struct hxc_caxecraft_gameplay_SwordCombatState hxc_l_swordCombat = (struct hxc_caxecraft_gameplay_SwordCombatState){ .hxc_cooldownTicks = 0 };
    struct hxc_caxecraft_gameplay_BerryDropState hxc_l_berryDrop = (struct hxc_caxecraft_gameplay_BerryDropState){ .hxc_active = false, .hxc_amount = 0, .hxc_x = 0.0, .hxc_y = 0.0, .hxc_z = 0.0 };
    double hxc_l_lookX = 0.0;
    double hxc_l_lookY = -0.18;
    double hxc_l_lookZ = -1.0;
    double hxc_l_accumulator = 0.0;
    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n197 = hxc_l_initialSession;
    hxc_l_gc_roots[90] = (const void *)hxc_l_tmp_load_result_n197;
    if (hxc_l_tmp_load_result_n197 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_domain_GameView hxc_l_tmp_instance_call_result_n198 = hxc_caxecraft_domain_GameSession_view(hxc_l_tmp_load_result_n197);
    struct hxc_caxecraft_domain_GameView hxc_l_initialPresentation = hxc_l_tmp_instance_call_result_n198;
    if (!hxc_l_initialPresentation.hxc_valid)
    {
      if (hxc_array_ref_release(hxc_l_initialActorPhases) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_enum_2c0e695d_destroy(&hxc_l_tmp_static_call_argument_0_owner_n69);
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
      hxc_enum_4b68fff2_destroy(&hxc_l_g_h1d08383de4f8);
      if (hxc_string_release(&hxc_l_pendingCampaignLabel) != HXC_STATUS_OK)
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
    struct hxc_caxecraft_app_MotionHistory hxc_l_tmp_call_result_n201 = hxc_caxecraft_app_MotionInterpolation_start(hxc_l_initialPresentation.hxc_localPlayer.hxc_body);
    struct hxc_caxecraft_app_MotionHistory hxc_l_motionHistory = hxc_l_tmp_call_result_n201;
    bool hxc_l_jumpQueued = false;
    bool hxc_l_swordQueued = false;
    int32_t hxc_l_selectedMode = 0;
    int32_t hxc_l_locale = 0;
    bool hxc_l_showInitialTitle = true;
    enum hxc_caxecraft_app_AppScreen hxc_l_tmp_call_result_n203 = hxc_caxecraft_app_AppScreen_initialScreen(hxc_l_showInitialTitle);
    enum hxc_caxecraft_app_AppScreen hxc_l_screen = hxc_l_tmp_call_result_n203;
    bool hxc_l_tmp_call_result_n205 = hxc_caxecraft_app_AppScreen_capturesPointer(hxc_l_screen);
    if (hxc_l_tmp_call_result_n205)
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
      bool hxc_l_tmp_load_result_n206 = hxc_l_quit;
      bool hxc_l_tmp_short_circuit_result_n103 = !hxc_l_tmp_load_result_n206;
      if (!hxc_l_tmp_load_result_n206)
      {
        bool hxc_l_tmp_native_call_result_n207 = WindowShouldClose();
        hxc_l_tmp_short_circuit_result_n103 = !hxc_l_tmp_native_call_result_n207;
      }
      if (!hxc_l_tmp_short_circuit_result_n103)
      {
        break;
      }
      bool hxc_l_tmp_call_result_n210 = hxc_caxecraft_app_AppScreen_showsLoading(hxc_l_screen);
      bool hxc_l_loadingAtFrameStart = hxc_l_tmp_call_result_n210;
      struct hxc_caxecraft_app_ActivePlayableLevel *hxc_l_tmp_load_result_n211 = hxc_l_activeLevel;
      hxc_l_gc_roots[91] = (const void *)hxc_l_tmp_load_result_n211;
      if (hxc_l_tmp_load_result_n211 == NULL)
      {
        abort();
      }
      struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_class_field_load_result_n212 = (*hxc_l_tmp_load_result_n211).hxc_activeView;
      hxc_l_gc_roots[92] = (const void *)hxc_l_tmp_class_field_load_result_n212;
      struct hxc_caxecraft_app_PlayableLevelView *hxc_l_levelView_hc35c2f93673f = hxc_l_tmp_class_field_load_result_n212;
      struct hxc_caxecraft_app_ActivePlayableLevel *hxc_l_tmp_load_result_n213 = hxc_l_activeLevel;
      hxc_l_gc_roots[93] = (const void *)hxc_l_tmp_load_result_n213;
      if (hxc_l_tmp_load_result_n213 == NULL)
      {
        abort();
      }
      struct hxc_caxecraft_content_ActiveContent *hxc_l_tmp_class_field_load_result_n214 = (*hxc_l_tmp_load_result_n213).hxc_content;
      hxc_l_gc_roots[94] = (const void *)hxc_l_tmp_class_field_load_result_n214;
      if (hxc_l_tmp_class_field_load_result_n214 == NULL)
      {
        abort();
      }
      struct hxc_caxecraft_content_LoadedContentGeneration *hxc_l_tmp_class_field_load_result_n215 = (*hxc_l_tmp_class_field_load_result_n214).hxc_active;
      hxc_l_gc_roots[95] = (const void *)hxc_l_tmp_class_field_load_result_n215;
      if (hxc_l_tmp_class_field_load_result_n215 == NULL)
      {
        abort();
      }
      struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_class_field_load_result_n216 = (*hxc_l_tmp_class_field_load_result_n215).hxc_ownedSession;
      hxc_l_gc_roots[96] = (const void *)hxc_l_tmp_class_field_load_result_n216;
      struct hxc_caxecraft_domain_GameSession *hxc_l_session = hxc_l_tmp_class_field_load_result_n216;
      struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n217 = hxc_l_session;
      hxc_l_gc_roots[97] = (const void *)hxc_l_tmp_load_result_n217;
      if (hxc_l_tmp_load_result_n217 == NULL)
      {
        abort();
      }
      struct hxc_caxecraft_domain_GameView hxc_l_tmp_instance_call_result_n218 = hxc_caxecraft_domain_GameSession_view(hxc_l_tmp_load_result_n217);
      struct hxc_caxecraft_domain_GameView hxc_l_initialView = hxc_l_tmp_instance_call_result_n218;
      if (!hxc_l_initialView.hxc_valid)
      {
        hxc_l_quit = true;
      }
      struct hxc_caxecraft_domain_Character hxc_l_character = hxc_l_initialView.hxc_localPlayer;
      bool hxc_l_recapturedThisFrame = false;
      bool hxc_l_resetMotionThisFrame = false;
      float hxc_l_tmp_native_call_result_n221 = GetFrameTime();
      double hxc_l_frameSeconds = (double)hxc_l_tmp_native_call_result_n221;
      if (hxc_l_frameSeconds > 0.25)
      {
        hxc_l_frameSeconds = 0.25;
      }
      bool hxc_l_tmp_native_call_result_n223 = IsWindowFocused();
      bool hxc_l_focused = hxc_l_tmp_native_call_result_n223;
      bool hxc_l_tmp_call_result_n225 = hxc_caxecraft_app_AppScreen_capturesPointer(hxc_l_screen);
      bool hxc_l_captured_ha833b16e7710 = hxc_l_tmp_call_result_n225;
      bool hxc_l_tmp_call_result_n227 = hxc_caxecraft_app_AppScreen_pausesSimulation(hxc_l_screen);
      bool hxc_l_paused_h1cda9fdeba21 = hxc_l_tmp_call_result_n227;
      double hxc_l_forward = 0.0;
      double hxc_l_right = 0.0;
      bool hxc_l_tmp_native_call_result_n228 = IsKeyDown((int32_t)87);
      if (hxc_l_tmp_native_call_result_n228)
      {
        hxc_l_forward = hxc_l_forward + 1.0;
      }
      bool hxc_l_tmp_native_call_result_n230 = IsKeyDown((int32_t)83);
      if (hxc_l_tmp_native_call_result_n230)
      {
        hxc_l_forward = hxc_l_forward - 1.0;
      }
      bool hxc_l_tmp_native_call_result_n232 = IsKeyDown((int32_t)68);
      if (hxc_l_tmp_native_call_result_n232)
      {
        hxc_l_right = hxc_l_right + 1.0;
      }
      bool hxc_l_tmp_native_call_result_n234 = IsKeyDown((int32_t)65);
      if (hxc_l_tmp_native_call_result_n234)
      {
        hxc_l_right = hxc_l_right - 1.0;
      }
      double hxc_l_lookYaw_hb374210bee18 = 0.0;
      double hxc_l_lookPitch_h51e952fd4861 = 0.0;
      if (hxc_l_captured_ha833b16e7710)
      {
        struct Vector2 hxc_l_tmp_native_call_result_n237 = GetMouseDelta();
        struct Vector2 hxc_l_mouse = hxc_l_tmp_native_call_result_n237;
        hxc_l_lookYaw_hb374210bee18 = -(double)hxc_l_mouse.x * 0.0025;
        hxc_l_lookPitch_h51e952fd4861 = -(double)hxc_l_mouse.y * 0.0025;
      }
      bool hxc_l_tmp_native_call_result_n242 = IsMouseButtonPressed((int32_t)0);
      bool hxc_l_leftPressed = hxc_l_tmp_native_call_result_n242;
      bool hxc_l_tmp_load_result_n243 = hxc_l_captured_ha833b16e7710;
      bool hxc_l_tmp_short_circuit_result_n122 = hxc_l_tmp_load_result_n243;
      if (hxc_l_tmp_load_result_n243)
      {
        hxc_l_tmp_short_circuit_result_n122 = hxc_l_leftPressed;
      }
      bool hxc_l_primaryPressed_h9198d1e327f6 = hxc_l_tmp_short_circuit_result_n122;
      bool hxc_l_tmp_load_result_n246 = hxc_l_captured_ha833b16e7710;
      bool hxc_l_tmp_short_circuit_result_n124 = hxc_l_tmp_load_result_n246;
      if (hxc_l_tmp_load_result_n246)
      {
        bool hxc_l_tmp_native_call_result_n247 = IsMouseButtonPressed((int32_t)1);
        hxc_l_tmp_short_circuit_result_n124 = hxc_l_tmp_native_call_result_n247;
      }
      bool hxc_l_secondaryPressed_h26cf1d3bffcf = hxc_l_tmp_short_circuit_result_n124;
      bool hxc_l_tmp_load_result_n249 = hxc_l_captured_ha833b16e7710;
      bool hxc_l_tmp_short_circuit_result_n126 = hxc_l_tmp_load_result_n249;
      if (hxc_l_tmp_load_result_n249)
      {
        bool hxc_l_tmp_native_call_result_n250 = IsKeyPressed((int32_t)69);
        hxc_l_tmp_short_circuit_result_n126 = hxc_l_tmp_native_call_result_n250;
      }
      bool hxc_l_interactPressed_hd79f9b854fc7 = hxc_l_tmp_short_circuit_result_n126;
      bool hxc_l_tmp_load_result_n252 = hxc_l_captured_ha833b16e7710;
      bool hxc_l_tmp_short_circuit_result_n128 = hxc_l_tmp_load_result_n252;
      if (hxc_l_tmp_load_result_n252)
      {
        bool hxc_l_tmp_native_call_result_n253 = IsKeyPressed((int32_t)78);
        hxc_l_tmp_short_circuit_result_n128 = hxc_l_tmp_native_call_result_n253;
      }
      bool hxc_l_travelPressed_h8036e2dffcbb = hxc_l_tmp_short_circuit_result_n128;
      int32_t hxc_l_hotbarSelection_ha1d9e5c582c6 = -1;
      bool hxc_l_tmp_native_call_result_n255 = IsKeyPressed((int32_t)49);
      if (hxc_l_tmp_native_call_result_n255)
      {
        hxc_l_hotbarSelection_ha1d9e5c582c6 = 0;
      }
      bool hxc_l_tmp_native_call_result_n256 = IsKeyPressed((int32_t)50);
      if (hxc_l_tmp_native_call_result_n256)
      {
        hxc_l_hotbarSelection_ha1d9e5c582c6 = 1;
      }
      bool hxc_l_tmp_native_call_result_n257 = IsKeyPressed((int32_t)51);
      if (hxc_l_tmp_native_call_result_n257)
      {
        hxc_l_hotbarSelection_ha1d9e5c582c6 = 2;
      }
      bool hxc_l_tmp_native_call_result_n258 = IsKeyPressed((int32_t)52);
      if (hxc_l_tmp_native_call_result_n258)
      {
        hxc_l_hotbarSelection_ha1d9e5c582c6 = 3;
      }
      bool hxc_l_tmp_native_call_result_n259 = IsKeyPressed((int32_t)53);
      if (hxc_l_tmp_native_call_result_n259)
      {
        hxc_l_hotbarSelection_ha1d9e5c582c6 = 4;
      }
      bool hxc_l_tmp_native_call_result_n260 = IsKeyPressed((int32_t)54);
      if (hxc_l_tmp_native_call_result_n260)
      {
        hxc_l_hotbarSelection_ha1d9e5c582c6 = 5;
      }
      bool hxc_l_tmp_native_call_result_n261 = IsKeyPressed((int32_t)55);
      if (hxc_l_tmp_native_call_result_n261)
      {
        hxc_l_hotbarSelection_ha1d9e5c582c6 = 6;
      }
      bool hxc_l_tmp_native_call_result_n262 = IsKeyPressed((int32_t)56);
      if (hxc_l_tmp_native_call_result_n262)
      {
        hxc_l_hotbarSelection_ha1d9e5c582c6 = 7;
      }
      float hxc_l_tmp_native_call_result_n263 = GetMouseWheelMove();
      double hxc_l_wheel = (double)hxc_l_tmp_native_call_result_n263;
      int32_t hxc_l_hotbarCycle_h1f1831086d1b = 0;
      if (hxc_l_wheel > 0.0)
      {
        hxc_l_hotbarCycle_h1f1831086d1b = -1;
      }
      if (hxc_l_wheel < 0.0)
      {
        hxc_l_hotbarCycle_h1f1831086d1b = 1;
      }
      bool hxc_l_tmp_native_call_result_n266 = IsKeyPressed((int32_t)32);
      bool hxc_l_jumpPressed_h06abcde0d8eb = hxc_l_tmp_native_call_result_n266;
      bool hxc_l_tmp_native_call_result_n267 = IsKeyPressed((int32_t)256);
      bool hxc_l_pausePressed_h01f426b2d457 = hxc_l_tmp_native_call_result_n267;
      bool hxc_l_tmp_native_call_result_n268 = IsKeyPressed((int32_t)81);
      bool hxc_l_quitPressed_h41807ce29bf5 = hxc_l_tmp_native_call_result_n268;
      bool hxc_l_tmp_native_call_result_n269 = IsKeyDown((int32_t)340);
      bool hxc_l_descendHeld_h36938fd57578 = hxc_l_tmp_native_call_result_n269;
      bool hxc_l_tmp_native_call_result_n270 = IsKeyPressed((int32_t)265);
      bool hxc_l_tmp_short_circuit_result_n137 = hxc_l_tmp_native_call_result_n270;
      if (!hxc_l_tmp_native_call_result_n270)
      {
        bool hxc_l_tmp_native_call_result_n271 = IsKeyPressed((int32_t)264);
        hxc_l_tmp_short_circuit_result_n137 = hxc_l_tmp_native_call_result_n271;
      }
      bool hxc_l_menuNextPressed_h41b79e045827 = hxc_l_tmp_short_circuit_result_n137;
      bool hxc_l_tmp_native_call_result_n273 = IsKeyPressed((int32_t)257);
      bool hxc_l_menuConfirmPressed_h0a7bf5212b4a = hxc_l_tmp_native_call_result_n273;
      double hxc_l_frameInput_moveForward = hxc_l_forward;
      double hxc_l_frameInput_moveRight = hxc_l_right;
      double hxc_l_frameInput_lookYaw = hxc_l_lookYaw_hb374210bee18;
      double hxc_l_frameInput_lookPitch = hxc_l_lookPitch_h51e952fd4861;
      bool hxc_l_frameInput_jumpPressed = hxc_l_jumpPressed_h06abcde0d8eb;
      bool hxc_l_tmp_native_call_result_n279 = IsKeyDown((int32_t)32);
      bool hxc_l_frameInput_riseHeld = hxc_l_tmp_native_call_result_n279;
      bool hxc_l_frameInput_descendHeld = hxc_l_descendHeld_h36938fd57578;
      bool hxc_l_frameInput_primaryPressed = hxc_l_primaryPressed_h9198d1e327f6;
      bool hxc_l_frameInput_secondaryPressed = hxc_l_secondaryPressed_h26cf1d3bffcf;
      bool hxc_l_frameInput_interactPressed = hxc_l_interactPressed_hd79f9b854fc7;
      bool hxc_l_frameInput_travelPressed = hxc_l_travelPressed_h8036e2dffcbb;
      bool hxc_l_frameInput_menuNextPressed = hxc_l_menuNextPressed_h41b79e045827;
      bool hxc_l_frameInput_menuConfirmPressed = hxc_l_menuConfirmPressed_h0a7bf5212b4a;
      bool hxc_l_frameInput_pausePressed = hxc_l_pausePressed_h01f426b2d457;
      bool hxc_l_tmp_load_result_n288 = hxc_l_paused_h1cda9fdeba21;
      bool hxc_l_tmp_short_circuit_result_n154 = hxc_l_tmp_load_result_n288;
      if (hxc_l_tmp_load_result_n288)
      {
        hxc_l_tmp_short_circuit_result_n154 = hxc_l_leftPressed;
      }
      bool hxc_l_frameInput_capturePressed = hxc_l_tmp_short_circuit_result_n154;
      bool hxc_l_frameInput_quitPressed = hxc_l_quitPressed_h41807ce29bf5;
      int32_t hxc_l_frameInput_hotbarSelection = hxc_l_hotbarSelection_ha1d9e5c582c6;
      int32_t hxc_l_frameInput_hotbarCycle = hxc_l_hotbarCycle_h1f1831086d1b;
      double hxc_l_moveForward = hxc_l_frameInput_moveForward;
      double hxc_l_moveRight = hxc_l_frameInput_moveRight;
      double hxc_l_lookYaw_hbb153fd12aee = hxc_l_frameInput_lookYaw;
      double hxc_l_lookPitch_h64f9d3a8873d = hxc_l_frameInput_lookPitch;
      bool hxc_l_jumpPressed_h00e33ed98ee9 = hxc_l_frameInput_jumpPressed;
      bool hxc_l_riseHeld = hxc_l_frameInput_riseHeld;
      bool hxc_l_primaryPressed_hfeea7ddbdc7b = hxc_l_frameInput_primaryPressed;
      bool hxc_l_secondaryPressed_h98e6c168bbcb = hxc_l_frameInput_secondaryPressed;
      bool hxc_l_interactPressed_h601b412c6a26 = hxc_l_frameInput_interactPressed;
      bool hxc_l_travelPressed_hf3246b10c8c7 = hxc_l_frameInput_travelPressed;
      bool hxc_l_pausePressed_h373f04990d15 = hxc_l_frameInput_pausePressed;
      bool hxc_l_capturePressed = hxc_l_frameInput_capturePressed;
      bool hxc_l_quitPressed_h953b72f5287d = hxc_l_frameInput_quitPressed;
      int32_t hxc_l_hotbarSelection_h8960df509a08 = hxc_l_frameInput_hotbarSelection;
      int32_t hxc_l_hotbarCycle_hea5b398b21a2 = hxc_l_frameInput_hotbarCycle;
      bool hxc_l_menuNextPressed_h1cc22e76dd6c = hxc_l_frameInput_menuNextPressed;
      bool hxc_l_menuConfirmPressed_h54f2f72c654a = hxc_l_frameInput_menuConfirmPressed;
      bool hxc_l_descendHeld_h3bc0f8055fa9 = hxc_l_frameInput_descendHeld;
      hxc_l_gc_roots[99] = (const void *)NULL;
      struct hxc_caxecraft_content_CampaignLevel *hxc_l_requestedCampaignLevel = NULL;
      bool hxc_l_tmp_load_result_n312 = hxc_l_loadingAtFrameStart;
      bool hxc_l_tmp_short_circuit_result_n177 = hxc_l_tmp_load_result_n312;
      if (hxc_l_tmp_load_result_n312)
      {
        hxc_l_tmp_short_circuit_result_n177 = hxc_l_loadingFramePresented;
      }
      if (hxc_l_tmp_short_circuit_result_n177)
      {
        hxc_l_gc_roots[100] = (const void *)hxc_l_pendingCampaignLevel;
        hxc_l_requestedCampaignLevel = hxc_l_pendingCampaignLevel;
      }
      bool hxc_l_requestedFromCampaignMenu = false;
      struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_load_result_n316 = hxc_l_requestedCampaignLevel;
      hxc_l_gc_roots[101] = (const void *)hxc_l_tmp_load_result_n316;
      hxc_l_gc_roots[102] = (const void *)NULL;
      bool hxc_l_tmp_short_circuit_result_n180 = hxc_l_tmp_load_result_n316 != NULL;
      if (hxc_l_tmp_load_result_n316 != NULL)
      {
        hxc_l_tmp_short_circuit_result_n180 = hxc_l_loadingAtFrameStart;
      }
      bool hxc_l_requestedFromAuthoredExit = hxc_l_tmp_short_circuit_result_n180;
      bool hxc_l_tmp_load_result_n319 = hxc_l_quit;
      bool hxc_l_tmp_short_circuit_result_n181 = !hxc_l_tmp_load_result_n319;
      if (!hxc_l_tmp_load_result_n319)
      {
        enum hxc_caxecraft_app_AppScreen hxc_l_tmp_load_result_n320 = hxc_l_screen;
        bool hxc_l_tmp_call_result_n322 = hxc_caxecraft_app_TitleMenuFlow_allowsCampaignTravel(hxc_l_tmp_load_result_n320, hxc_l_selectedMode);
        hxc_l_tmp_short_circuit_result_n181 = hxc_l_tmp_call_result_n322;
      }
      bool hxc_l_tmp_short_circuit_load_result_n323 = hxc_l_tmp_short_circuit_result_n181;
      bool hxc_l_tmp_short_circuit_result_n182 = hxc_l_tmp_short_circuit_load_result_n323;
      if (hxc_l_tmp_short_circuit_load_result_n323)
      {
        hxc_l_tmp_short_circuit_result_n182 = hxc_l_travelPressed_hf3246b10c8c7;
      }
      if (hxc_l_tmp_short_circuit_result_n182)
      {
        hxc_l_gc_roots[103] = (const void *)hxc_l_campaign;
        struct hxc_caxecraft_content_CampaignManifest *hxc_l_selectedCampaign_h3b6948d6d534 = hxc_l_campaign;
        hxc_l_gc_roots[104] = (const void *)hxc_l_campaignLevel;
        struct hxc_caxecraft_content_CampaignLevel *hxc_l_sourceLevel_h577cff68ba67 = hxc_l_campaignLevel;
        struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n328 = hxc_l_selectedCampaign_h3b6948d6d534;
        hxc_l_gc_roots[105] = (const void *)hxc_l_tmp_load_result_n328;
        hxc_l_gc_roots[106] = (const void *)NULL;
        bool hxc_l_tmp_short_circuit_result_n185 = hxc_l_tmp_load_result_n328 != NULL;
        if (hxc_l_tmp_load_result_n328 != NULL)
        {
          hxc_l_gc_roots[107] = (const void *)hxc_l_sourceLevel_h577cff68ba67;
          hxc_l_gc_roots[108] = (const void *)NULL;
          hxc_l_tmp_short_circuit_result_n185 = hxc_l_sourceLevel_h577cff68ba67 != NULL;
        }
        if (hxc_l_tmp_short_circuit_result_n185)
        {
          struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n331 = hxc_l_selectedCampaign_h3b6948d6d534;
          hxc_l_gc_roots[109] = (const void *)hxc_l_tmp_load_result_n331;
          if (hxc_l_tmp_load_result_n331 == NULL)
          {
            abort();
          }
          struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_load_result_n332 = hxc_l_sourceLevel_h577cff68ba67;
          hxc_l_gc_roots[110] = (const void *)hxc_l_tmp_load_result_n332;
          if (hxc_l_tmp_load_result_n332 == NULL)
          {
            abort();
          }
          hxc_string hxc_l_tmp_class_field_load_result_n333 = (*hxc_l_tmp_load_result_n332).hxc_id;
          struct hxc_caxecraft_content_CampaignTransition *hxc_l_tmp_instance_call_result_n334 = hxc_caxecraft_content_CampaignManifest_unambiguousTransitionFrom(hxc_l_tmp_load_result_n331, hxc_l_tmp_class_field_load_result_n333);
          hxc_l_gc_roots[111] = (const void *)hxc_l_tmp_instance_call_result_n334;
          struct hxc_caxecraft_content_CampaignTransition *hxc_l_transition_h9166ba4cfa9f = hxc_l_tmp_instance_call_result_n334;
          hxc_l_gc_roots[112] = (const void *)hxc_l_transition_h9166ba4cfa9f;
          hxc_l_gc_roots[113] = (const void *)NULL;
          if (hxc_l_transition_h9166ba4cfa9f == NULL)
          {
            struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_load_result_n336 = hxc_l_sourceLevel_h577cff68ba67;
            hxc_l_gc_roots[114] = (const void *)hxc_l_tmp_load_result_n336;
            if (hxc_l_tmp_load_result_n336 == NULL)
            {
              abort();
            }
            hxc_string hxc_l_tmp_class_field_load_result_n337 = (*hxc_l_tmp_load_result_n336).hxc_id;
            hxc_string hxc_l_tmp_string_concat_result_n338 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
            if (hxc_string_concat_ref((hxc_string){ (const uint8_t *)"caxecraft: campaign travel requires exactly one outgoing transition from ", 73, true, NULL }, hxc_l_tmp_class_field_load_result_n337, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n338) != HXC_STATUS_OK)
            {
              abort();
            }
            hxc_l_tmp_sys_println_string_argument_owner_n187 = hxc_l_tmp_string_concat_result_n338;
            if (hxc_io_println(hxc_l_tmp_sys_println_string_argument_owner_n187) != HXC_STATUS_OK)
            {
              if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n187) != HXC_STATUS_OK)
              {
                abort();
              }
              if (hxc_array_ref_release(hxc_l_initialActorPhases) != HXC_STATUS_OK)
              {
                abort();
              }
              hxc_enum_2c0e695d_destroy(&hxc_l_tmp_static_call_argument_0_owner_n69);
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
              hxc_enum_4b68fff2_destroy(&hxc_l_g_h1d08383de4f8);
              if (hxc_string_release(&hxc_l_pendingCampaignLabel) != HXC_STATUS_OK)
              {
                abort();
              }
              hxc_enum_fcb4401b_destroy(&hxc_l_g_h4ef171f4fe67);
              if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
              {
                abort();
              }
              abort();
            }
            if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n187) != HXC_STATUS_OK)
            {
              abort();
            }
          }
          else
          {
            struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n340 = hxc_l_selectedCampaign_h3b6948d6d534;
            hxc_l_gc_roots[115] = (const void *)hxc_l_tmp_load_result_n340;
            if (hxc_l_tmp_load_result_n340 == NULL)
            {
              abort();
            }
            struct hxc_caxecraft_content_CampaignTransition *hxc_l_tmp_load_result_n341 = hxc_l_transition_h9166ba4cfa9f;
            hxc_l_gc_roots[116] = (const void *)hxc_l_tmp_load_result_n341;
            if (hxc_l_tmp_load_result_n341 == NULL)
            {
              abort();
            }
            hxc_string hxc_l_tmp_class_field_load_result_n342 = (*hxc_l_tmp_load_result_n341).hxc_destinationLevel;
            struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_instance_call_result_n343 = hxc_caxecraft_content_CampaignManifest_level(hxc_l_tmp_load_result_n340, hxc_l_tmp_class_field_load_result_n342);
            hxc_l_gc_roots[117] = (const void *)hxc_l_tmp_instance_call_result_n343;
            struct hxc_caxecraft_content_CampaignLevel *hxc_l_destination_h923629bdb0bb = hxc_l_tmp_instance_call_result_n343;
            hxc_l_gc_roots[118] = (const void *)hxc_l_destination_h923629bdb0bb;
            hxc_l_gc_roots[119] = (const void *)NULL;
            if (hxc_l_destination_h923629bdb0bb == NULL)
            {
              if (hxc_io_println((hxc_string){ (const uint8_t *)"caxecraft: campaign destination disappeared after manifest validation", 69, true, NULL }) != HXC_STATUS_OK)
              {
                abort();
              }
            }
            else
            {
              hxc_l_gc_roots[120] = (const void *)hxc_l_destination_h923629bdb0bb;
              hxc_l_requestedCampaignLevel = hxc_l_destination_h923629bdb0bb;
            }
          }
        }
      }
      bool hxc_l_tmp_call_result_n347 = hxc_caxecraft_app_AppScreen_showsCampaignSelection(hxc_l_screen);
      bool hxc_l_tmp_short_circuit_result_n189 = hxc_l_tmp_call_result_n347;
      if (hxc_l_tmp_call_result_n347)
      {
        hxc_l_tmp_short_circuit_result_n189 = hxc_l_focused;
      }
      if (hxc_l_tmp_short_circuit_result_n189)
      {
        bool hxc_l_campaignLaunchRequested = hxc_l_menuConfirmPressed_h54f2f72c654a;
        bool hxc_l_campaignBackRequested = hxc_l_pausePressed_h373f04990d15;
        bool hxc_l_tmp_load_result_n352 = hxc_l_menuNextPressed_h1cc22e76dd6c;
        bool hxc_l_tmp_short_circuit_result_n192 = hxc_l_tmp_load_result_n352;
        if (hxc_l_tmp_load_result_n352)
        {
          hxc_l_gc_roots[121] = (const void *)hxc_l_campaign;
          hxc_l_gc_roots[122] = (const void *)NULL;
          hxc_l_tmp_short_circuit_result_n192 = hxc_l_campaign != NULL;
        }
        bool hxc_l_tmp_short_circuit_load_result_n354 = hxc_l_tmp_short_circuit_result_n192;
        bool hxc_l_tmp_short_circuit_result_n193 = hxc_l_tmp_short_circuit_load_result_n354;
        if (hxc_l_tmp_short_circuit_load_result_n354)
        {
          struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n355 = hxc_l_campaign;
          hxc_l_gc_roots[123] = (const void *)hxc_l_tmp_load_result_n355;
          if (hxc_l_tmp_load_result_n355 == NULL)
          {
            abort();
          }
          struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n356 = (*hxc_l_tmp_load_result_n355).hxc_admittedLevels;
          hxc_l_gc_roots[124] = (const void *)hxc_l_tmp_class_field_load_result_n356;
          int32_t hxc_l_tmp_array_length_result_n357;
          if (hxc_array_ref_length(hxc_l_tmp_class_field_load_result_n356, &hxc_l_tmp_array_length_result_n357) != HXC_STATUS_OK)
          {
            abort();
          }
          hxc_l_tmp_short_circuit_result_n193 = hxc_l_tmp_array_length_result_n357 > 0;
        }
        if (hxc_l_tmp_short_circuit_result_n193)
        {
          int32_t hxc_l_tmp_load_result_n359 = hxc_l_selectedCampaignLevelIndex;
          struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n360 = hxc_l_campaign;
          hxc_l_gc_roots[125] = (const void *)hxc_l_tmp_load_result_n360;
          if (hxc_l_tmp_load_result_n360 == NULL)
          {
            abort();
          }
          struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n361 = (*hxc_l_tmp_load_result_n360).hxc_admittedLevels;
          hxc_l_gc_roots[126] = (const void *)hxc_l_tmp_class_field_load_result_n361;
          int32_t hxc_l_tmp_array_length_result_n362;
          if (hxc_array_ref_length(hxc_l_tmp_class_field_load_result_n361, &hxc_l_tmp_array_length_result_n362) != HXC_STATUS_OK)
          {
            abort();
          }
          hxc_l_selectedCampaignLevelIndex = hxc_i32_modulo_zero_safe(hxc_i32_add_wrapping(hxc_l_tmp_load_result_n359, 1), hxc_l_tmp_array_length_result_n362);
        }
        bool hxc_l_tmp_native_call_result_n363 = IsKeyPressed((int32_t)76);
        if (hxc_l_tmp_native_call_result_n363)
        {
          struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_tmp_load_result_n364 = hxc_l_uiCatalog;
          hxc_l_gc_roots[127] = (const void *)hxc_l_tmp_load_result_n364;
          if (hxc_l_tmp_load_result_n364 == NULL)
          {
            abort();
          }
          int32_t hxc_l_tmp_instance_call_result_n366 = hxc_caxecraft_localization_RuntimeUiCatalog_nextLocale(hxc_l_tmp_load_result_n364, hxc_l_locale);
          hxc_l_locale = hxc_l_tmp_instance_call_result_n366;
        }
        struct Vector2 hxc_l_tmp_native_call_result_n367 = GetMousePosition();
        struct Vector2 hxc_l_campaignMouse = hxc_l_tmp_native_call_result_n367;
        double hxc_l_tmp_static_call_argument_0_n196 = (double)hxc_l_campaignMouse.x;
        double hxc_l_tmp_static_call_argument_1_n197 = (double)hxc_l_campaignMouse.y;
        int32_t hxc_l_tmp_native_call_result_n372 = GetScreenWidth();
        int32_t hxc_l_tmp_static_call_argument_2_n198 = (int32_t)hxc_l_tmp_native_call_result_n372;
        int32_t hxc_l_tmp_native_call_result_n373 = GetScreenHeight();
        int32_t hxc_l_tmp_static_call_argument_3_n199 = (int32_t)hxc_l_tmp_native_call_result_n373;
        struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n374 = hxc_l_campaign;
        hxc_l_gc_roots[128] = (const void *)hxc_l_tmp_load_result_n374;
        hxc_l_gc_roots[129] = (const void *)NULL;
        int32_t hxc_l_tmp_conditional_result_n200 = 0;
        if (hxc_l_tmp_load_result_n374 == NULL)
        {
          hxc_l_tmp_conditional_result_n200 = 0;
        }
        else
        {
          struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n375 = hxc_l_campaign;
          hxc_l_gc_roots[130] = (const void *)hxc_l_tmp_load_result_n375;
          if (hxc_l_tmp_load_result_n375 == NULL)
          {
            abort();
          }
          struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n376 = (*hxc_l_tmp_load_result_n375).hxc_admittedLevels;
          hxc_l_gc_roots[131] = (const void *)hxc_l_tmp_class_field_load_result_n376;
          int32_t hxc_l_tmp_array_length_result_n377;
          if (hxc_array_ref_length(hxc_l_tmp_class_field_load_result_n376, &hxc_l_tmp_array_length_result_n377) != HXC_STATUS_OK)
          {
            abort();
          }
          hxc_l_tmp_conditional_result_n200 = hxc_l_tmp_array_length_result_n377;
        }
        int32_t hxc_l_tmp_conditional_load_result_n378 = hxc_l_tmp_conditional_result_n200;
        double hxc_l_tmp_static_call_argument_0_load_result_n379 = hxc_l_tmp_static_call_argument_0_n196;
        double hxc_l_tmp_static_call_argument_1_load_result_n380 = hxc_l_tmp_static_call_argument_1_n197;
        int32_t hxc_l_tmp_static_call_argument_2_load_result_n381 = hxc_l_tmp_static_call_argument_2_n198;
        struct hxc_caxecraft_app_CampaignMenuHit hxc_l_tmp_call_result_n383 = hxc_caxecraft_app_CampaignMenu_selectionAt(hxc_l_tmp_static_call_argument_0_load_result_n379, hxc_l_tmp_static_call_argument_1_load_result_n380, hxc_l_tmp_static_call_argument_2_load_result_n381, hxc_l_tmp_static_call_argument_3_n199, hxc_l_tmp_conditional_load_result_n378);
        struct hxc_caxecraft_app_CampaignMenuHit hxc_l_campaignChoice = hxc_l_tmp_call_result_n383;
        bool hxc_l_tmp_native_call_result_n384 = IsMouseButtonPressed((int32_t)0);
        if (hxc_l_tmp_native_call_result_n384)
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
                int32_t hxc_l_tmp_enum_payload_project_n387 = hxc_l_campaignChoice.hxc_payload.hxc_LevelHit.hxc_index;
                int32_t hxc_l_index_ha437a382871e = hxc_l_tmp_enum_payload_project_n387;
                hxc_l_selectedCampaignLevelIndex = hxc_l_index_ha437a382871e;
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
          enum hxc_caxecraft_app_AppScreen hxc_l_tmp_call_result_n391 = hxc_caxecraft_app_AppScreen_closeCampaignSelection(hxc_l_screen);
          hxc_l_screen = hxc_l_tmp_call_result_n391;
          hxc_l_accumulator = 0.0;
          hxc_l_resetMotionThisFrame = true;
        }
        else
        {
          struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n392 = hxc_l_campaign;
          hxc_l_gc_roots[132] = (const void *)hxc_l_tmp_load_result_n392;
          hxc_l_gc_roots[133] = (const void *)NULL;
          bool hxc_l_tmp_short_circuit_result_n202 = hxc_l_tmp_load_result_n392 != NULL;
          if (hxc_l_tmp_load_result_n392 != NULL)
          {
            hxc_l_tmp_short_circuit_result_n202 = hxc_l_campaignLaunchRequested;
          }
          if (hxc_l_tmp_short_circuit_result_n202)
          {
            struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n395 = hxc_l_campaign;
            hxc_l_gc_roots[134] = (const void *)hxc_l_tmp_load_result_n395;
            if (hxc_l_tmp_load_result_n395 == NULL)
            {
              abort();
            }
            struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n396 = (*hxc_l_tmp_load_result_n395).hxc_admittedLevels;
            hxc_l_gc_roots[135] = (const void *)hxc_l_tmp_class_field_load_result_n396;
            struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_array_get_result_n398;
            if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n396, (size_t)hxc_l_selectedCampaignLevelIndex, &hxc_l_tmp_array_get_result_n398) != HXC_STATUS_OK)
            {
              abort();
            }
            hxc_l_gc_roots[136] = (const void *)hxc_l_tmp_array_get_result_n398;
            hxc_l_requestedCampaignLevel = hxc_l_tmp_array_get_result_n398;
            hxc_l_requestedFromCampaignMenu = true;
          }
        }
      }
      bool hxc_l_tmp_call_result_n400 = hxc_caxecraft_app_AppScreen_showsTitle(hxc_l_screen);
      bool hxc_l_tmp_short_circuit_result_n203 = hxc_l_tmp_call_result_n400;
      if (hxc_l_tmp_call_result_n400)
      {
        hxc_l_tmp_short_circuit_result_n203 = hxc_l_focused;
      }
      if (hxc_l_tmp_short_circuit_result_n203)
      {
        enum hxc_caxecraft_app_AppScreen hxc_l_screenBeforeMenu = hxc_l_screen;
        int32_t hxc_l_modeBeforeMenu = hxc_l_selectedMode;
        enum hxc_caxecraft_app_AppScreen hxc_l_tmp_load_result_n405 = hxc_l_screen;
        struct hxc_caxecraft_app_TitleMenuState hxc_l_tmp_call_result_n407 = hxc_caxecraft_app_TitleMenuFlow_titleMenuState(hxc_l_tmp_load_result_n405, hxc_l_selectedMode);
        struct hxc_caxecraft_app_TitleMenuState hxc_l_menuState = hxc_l_tmp_call_result_n407;
        bool hxc_l_tmp_native_call_result_n408 = IsKeyPressed((int32_t)76);
        if (hxc_l_tmp_native_call_result_n408)
        {
          struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_tmp_load_result_n409 = hxc_l_uiCatalog;
          hxc_l_gc_roots[140] = (const void *)hxc_l_tmp_load_result_n409;
          if (hxc_l_tmp_load_result_n409 == NULL)
          {
            abort();
          }
          int32_t hxc_l_tmp_instance_call_result_n411 = hxc_caxecraft_localization_RuntimeUiCatalog_nextLocale(hxc_l_tmp_load_result_n409, hxc_l_locale);
          hxc_l_locale = hxc_l_tmp_instance_call_result_n411;
        }
        if (hxc_l_menuNextPressed_h1cc22e76dd6c)
        {
          struct hxc_caxecraft_app_TitleMenuState hxc_l_tmp_call_result_n414 = hxc_caxecraft_app_TitleMenuFlow_applyTitleMenuCommand(hxc_l_menuState, 1);
          hxc_l_menuState = hxc_l_tmp_call_result_n414;
        }
        struct Vector2 hxc_l_tmp_native_call_result_n415 = GetMousePosition();
        struct Vector2 hxc_l_menuMouse = hxc_l_tmp_native_call_result_n415;
        struct Vector2 hxc_l_tmp_load_result_n416 = hxc_l_menuMouse;
        struct Vector2 hxc_l_tmp_load_result_n418 = hxc_l_menuMouse;
        int32_t hxc_l_tmp_native_call_result_n420 = GetScreenWidth();
        int32_t hxc_l_tmp_native_call_result_n421 = GetScreenHeight();
        int32_t hxc_l_tmp_call_result_n422 = hxc_caxecraft_app_TitleMenu_selectionAt((double)hxc_l_tmp_load_result_n416.x, (double)hxc_l_tmp_load_result_n418.y, (int32_t)hxc_l_tmp_native_call_result_n420, (int32_t)hxc_l_tmp_native_call_result_n421);
        int32_t hxc_l_hovered = hxc_l_tmp_call_result_n422;
        if (hxc_l_hovered == 0)
        {
          struct hxc_caxecraft_app_TitleMenuState hxc_l_tmp_call_result_n425 = hxc_caxecraft_app_TitleMenuFlow_applyTitleMenuCommand(hxc_l_menuState, 2);
          hxc_l_menuState = hxc_l_tmp_call_result_n425;
        }
        if (hxc_l_hovered == 1)
        {
          struct hxc_caxecraft_app_TitleMenuState hxc_l_tmp_call_result_n428 = hxc_caxecraft_app_TitleMenuFlow_applyTitleMenuCommand(hxc_l_menuState, 3);
          hxc_l_menuState = hxc_l_tmp_call_result_n428;
        }
        int32_t hxc_l_tmp_load_result_n429 = hxc_l_hovered;
        bool hxc_l_tmp_short_circuit_result_n210 = hxc_l_tmp_load_result_n429 >= 0;
        if (hxc_l_tmp_load_result_n429 >= 0)
        {
          bool hxc_l_tmp_native_call_result_n430 = IsMouseButtonPressed((int32_t)0);
          hxc_l_tmp_short_circuit_result_n210 = hxc_l_tmp_native_call_result_n430;
        }
        bool hxc_l_clickedChoice = hxc_l_tmp_short_circuit_result_n210;
        if (hxc_l_clickedChoice)
        {
          struct hxc_caxecraft_app_TitleMenuState hxc_l_tmp_static_call_argument_0_n211 = hxc_l_menuState;
          int32_t hxc_l_tmp_load_result_n434 = hxc_l_hovered;
          int32_t hxc_l_tmp_conditional_result_n212 = 0;
          if (hxc_l_tmp_load_result_n434 == 2)
          {
            hxc_l_tmp_conditional_result_n212 = 5;
          }
          else
          {
            hxc_l_tmp_conditional_result_n212 = 4;
          }
          int32_t hxc_l_tmp_conditional_load_result_n435 = hxc_l_tmp_conditional_result_n212;
          struct hxc_caxecraft_app_TitleMenuState hxc_l_tmp_call_result_n437 = hxc_caxecraft_app_TitleMenuFlow_applyTitleMenuCommand(hxc_l_tmp_static_call_argument_0_n211, hxc_l_tmp_conditional_load_result_n435);
          hxc_l_menuState = hxc_l_tmp_call_result_n437;
        }
        else
        {
          if (hxc_l_menuConfirmPressed_h54f2f72c654a)
          {
            struct hxc_caxecraft_app_TitleMenuState hxc_l_tmp_call_result_n440 = hxc_caxecraft_app_TitleMenuFlow_applyTitleMenuCommand(hxc_l_menuState, 4);
            hxc_l_menuState = hxc_l_tmp_call_result_n440;
          }
        }
        hxc_l_screen = hxc_l_menuState.hxc_screen;
        hxc_l_selectedMode = hxc_l_menuState.hxc_mode;
        bool hxc_l_tmp_call_result_n444 = hxc_caxecraft_app_AppScreen_showsCampaignSelection(hxc_l_screen);
        bool hxc_l_tmp_short_circuit_result_n213 = hxc_l_tmp_call_result_n444;
        if (hxc_l_tmp_call_result_n444)
        {
          hxc_l_gc_roots[141] = (const void *)hxc_l_campaign;
          hxc_l_gc_roots[142] = (const void *)NULL;
          hxc_l_tmp_short_circuit_result_n213 = hxc_l_campaign == NULL;
        }
        if (hxc_l_tmp_short_circuit_result_n213)
        {
          hxc_l_screen = hxc_l_screenBeforeMenu;
        }
        enum hxc_caxecraft_app_AppScreen hxc_l_tmp_load_result_n448 = hxc_l_screen;
        enum hxc_caxecraft_app_AppScreen hxc_l_tmp_load_result_n449 = hxc_l_screenBeforeMenu;
        bool hxc_l_tmp_short_circuit_result_n214 = hxc_l_tmp_load_result_n448 != hxc_l_tmp_load_result_n449;
        if (!(hxc_l_tmp_load_result_n448 != hxc_l_tmp_load_result_n449))
        {
          int32_t hxc_l_tmp_load_result_n450 = hxc_l_selectedMode;
          hxc_l_tmp_short_circuit_result_n214 = hxc_l_tmp_load_result_n450 != hxc_l_modeBeforeMenu;
        }
        if (hxc_l_tmp_short_circuit_result_n214)
        {
          hxc_l_accumulator = 0.0;
          hxc_l_resetMotionThisFrame = true;
        }
        enum hxc_caxecraft_app_AppScreen hxc_l_tmp_load_result_n453 = hxc_l_screenBeforeMenu;
        bool hxc_l_tmp_short_circuit_result_n215 = hxc_l_tmp_load_result_n453 == hxc_caxecraft_app_AppScreen_Title;
        if (hxc_l_tmp_load_result_n453 == hxc_caxecraft_app_AppScreen_Title)
        {
          bool hxc_l_tmp_call_result_n456 = hxc_caxecraft_app_AppScreen_isPlaying(hxc_l_screen);
          hxc_l_tmp_short_circuit_result_n215 = hxc_l_tmp_call_result_n456;
        }
        if (hxc_l_tmp_short_circuit_result_n215)
        {
          hxc_l_recapturedThisFrame = true;
          DisableCursor();
        }
        else
        {
          bool hxc_l_tmp_call_result_n459 = hxc_caxecraft_app_AppScreen_showsEditor(hxc_l_screen);
          if (hxc_l_tmp_call_result_n459)
          {
            EnableCursor();
          }
        }
      }
      hxc_l_gc_roots[137] = (const void *)hxc_l_requestedCampaignLevel;
      struct hxc_caxecraft_content_CampaignLevel *hxc_l_destination_haaae67a44231 = hxc_l_requestedCampaignLevel;
      hxc_l_gc_roots[138] = (const void *)hxc_l_destination_haaae67a44231;
      hxc_l_gc_roots[139] = (const void *)NULL;
      if (hxc_l_destination_haaae67a44231 != NULL)
      {
        int32_t hxc_l_tmp_record_field_load_result_n462 = hxc_l_character.hxc_id;
        int32_t hxc_l_tmp_record_field_load_result_n463 = hxc_l_character.hxc_vitals.hxc_health;
        struct hxc_caxecraft_content_LevelPlayerOptions hxc_l_playerOptions = (struct hxc_caxecraft_content_LevelPlayerOptions){ .hxc_aquaticProfile = hxc_l_character.hxc_aquaticProfile, .hxc_entityId = hxc_l_tmp_record_field_load_result_n462, .hxc_initialHealth = hxc_l_tmp_record_field_load_result_n463 };
        struct hxc_caxecraft_app_ActivePlayableLevel *hxc_l_tmp_load_result_n466 = hxc_l_activeLevel;
        hxc_l_gc_roots[143] = (const void *)hxc_l_tmp_load_result_n466;
        if (hxc_l_tmp_load_result_n466 == NULL)
        {
          abort();
        }
        struct hxc_caxecraft_content_ActiveContent *hxc_l_tmp_class_field_load_result_n467 = (*hxc_l_tmp_load_result_n466).hxc_content;
        hxc_l_gc_roots[144] = (const void *)hxc_l_tmp_class_field_load_result_n467;
        if (hxc_l_tmp_class_field_load_result_n467 == NULL)
        {
          abort();
        }
        struct hxc_caxecraft_content_LoadedContentGeneration *hxc_l_tmp_class_field_load_result_n468 = (*hxc_l_tmp_class_field_load_result_n467).hxc_active;
        hxc_l_gc_roots[145] = (const void *)hxc_l_tmp_class_field_load_result_n468;
        if (hxc_l_tmp_class_field_load_result_n468 == NULL)
        {
          abort();
        }
        int32_t hxc_l_tmp_class_field_load_result_n469 = (*hxc_l_tmp_class_field_load_result_n468).hxc_generationIdValue;
        int32_t hxc_l_nextGeneration = hxc_i32_add_wrapping(hxc_l_tmp_class_field_load_result_n469, 1);
        struct hxc_caxecraft_content_ContentPackageStore *hxc_l_tmp_load_result_n470 = hxc_l_contentStore;
        hxc_l_gc_roots[146] = (const void *)hxc_l_tmp_load_result_n470;
        struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_load_result_n471 = hxc_l_destination_haaae67a44231;
        hxc_l_gc_roots[147] = (const void *)hxc_l_tmp_load_result_n471;
        int32_t hxc_l_tmp_load_result_n472 = hxc_l_nextGeneration;
        struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_tmp_load_result_n473 = hxc_l_contentRegistry;
        hxc_l_gc_roots[148] = (const void *)hxc_l_tmp_load_result_n473;
        if (hxc_l_tmp_load_result_n473 == NULL)
        {
          abort();
        }
        struct hxc_compiler_interface_dispatch_caxecraft_scenario_ScenarioContentRegistry_value hxc_l_tmp_interface_value_n474 = (struct hxc_compiler_interface_dispatch_caxecraft_scenario_ScenarioContentRegistry_value){ .object = hxc_l_tmp_load_result_n473, .table = &hxc_itable_compiler_interface_dispatch_caxecraft_content_RuntimeContentRegistry_itable_layout_caxecraf_hb3f384641e1a61bf };
        struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_tmp_load_result_n475 = hxc_l_contentRegistry;
        hxc_l_gc_roots[149] = (const void *)hxc_l_tmp_load_result_n475;
        if (hxc_l_tmp_load_result_n475 == NULL)
        {
          abort();
        }
        struct hxc_compiler_interface_dispatch_caxecraft_content_LevelContentResolver_value hxc_l_tmp_interface_value_n476 = (struct hxc_compiler_interface_dispatch_caxecraft_content_LevelContentResolver_value){ .object = hxc_l_tmp_load_result_n475, .table = &hxc_itable_compiler_interface_dispatch_caxecraft_content_RuntimeContentRegistry_itable_layout_caxecraf_h82ca4f24ca7a7b5a };
        struct hxc_caxecraft_content_CampaignLevelLoadResult hxc_l_tmp_call_result_n478 = hxc_caxecraft_content_CampaignRuntime_loadCampaignLevel(hxc_l_tmp_load_result_n470, hxc_l_tmp_load_result_n471, hxc_l_tmp_load_result_n472, hxc_l_tmp_interface_value_n474, hxc_l_tmp_interface_value_n476, hxc_l_playerOptions);
        hxc_l_gc_roots[150] = hxc_l_tmp_call_result_n478.hxc_tag == hxc_caxecraft_content_CampaignLevelLoadResult_CampaignLevelReady ? (const void *)hxc_l_tmp_call_result_n478.hxc_payload.hxc_CampaignLevelReady.hxc_candidate : NULL;
        hxc_l_g_h2fc2c60edcf1 = hxc_l_tmp_call_result_n478;
        hxc_l_gc_roots[151] = hxc_l_g_h2fc2c60edcf1.hxc_tag == hxc_caxecraft_content_CampaignLevelLoadResult_CampaignLevelReady ? (const void *)hxc_l_g_h2fc2c60edcf1.hxc_payload.hxc_CampaignLevelReady.hxc_candidate : NULL;
        switch (hxc_l_g_h2fc2c60edcf1.hxc_tag) {
          case hxc_caxecraft_content_CampaignLevelLoadResult_CampaignLevelReady:
            {
              hxc_l_gc_roots[154] = hxc_l_g_h2fc2c60edcf1.hxc_tag == hxc_caxecraft_content_CampaignLevelLoadResult_CampaignLevelReady ? (const void *)hxc_l_g_h2fc2c60edcf1.hxc_payload.hxc_CampaignLevelReady.hxc_candidate : NULL;
              if (hxc_l_g_h2fc2c60edcf1.hxc_tag != hxc_caxecraft_content_CampaignLevelLoadResult_CampaignLevelReady)
              {
                abort();
              }
              struct hxc_caxecraft_content_RuntimeLevelCandidate *hxc_l_tmp_enum_payload_project_n481 = hxc_l_g_h2fc2c60edcf1.hxc_payload.hxc_CampaignLevelReady.hxc_candidate;
              hxc_l_gc_roots[155] = (const void *)hxc_l_tmp_enum_payload_project_n481;
              struct hxc_caxecraft_content_RuntimeLevelCandidate *hxc_l_candidate_h1b5c173b8244 = hxc_l_tmp_enum_payload_project_n481;
              struct hxc_caxecraft_app_ActivePlayableLevel *hxc_l_tmp_load_result_n482 = hxc_l_activeLevel;
              hxc_l_gc_roots[156] = (const void *)hxc_l_tmp_load_result_n482;
              if (hxc_l_tmp_load_result_n482 == NULL)
              {
                abort();
              }
              hxc_l_gc_roots[157] = (const void *)hxc_l_candidate_h1b5c173b8244;
              struct hxc_caxecraft_app_PlayableLevelPublicationResult hxc_l_tmp_instance_call_result_n484 = hxc_caxecraft_app_ActivePlayableLevel_publish(hxc_l_tmp_load_result_n482, hxc_l_candidate_h1b5c173b8244);
              struct hxc_caxecraft_app_PlayableLevelPublicationResult hxc_l_g_hcaf973d3130d = hxc_l_tmp_instance_call_result_n484;
              switch (hxc_l_g_hcaf973d3130d.hxc_tag) {
                case hxc_caxecraft_app_PlayableLevelPublicationResult_PlayableLevelPublished:
                  {
                    if (hxc_l_g_hcaf973d3130d.hxc_tag != hxc_caxecraft_app_PlayableLevelPublicationResult_PlayableLevelPublished)
                    {
                      abort();
                    }
                    int32_t hxc_l_tmp_enum_payload_project_n487 = hxc_l_g_hcaf973d3130d.hxc_payload.hxc_PlayableLevelPublished.hxc_active;
                    int32_t hxc_l_selected = hxc_l_tmp_enum_payload_project_n487;
                    hxc_l_gc_roots[159] = (const void *)hxc_l_destination_haaae67a44231;
                    hxc_l_campaignLevel = hxc_l_destination_haaae67a44231;
                    struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_load_result_n489 = hxc_l_destination_haaae67a44231;
                    hxc_l_gc_roots[160] = (const void *)hxc_l_tmp_load_result_n489;
                    if (hxc_l_tmp_load_result_n489 == NULL)
                    {
                      abort();
                    }
                    hxc_string hxc_l_tmp_class_field_load_result_n490 = (*hxc_l_tmp_load_result_n489).hxc_id;
                    hxc_string hxc_l_tmp_string_assignment_replacement_owner_n223 = hxc_l_tmp_class_field_load_result_n490;
                    if (hxc_string_retain(hxc_l_tmp_string_assignment_replacement_owner_n223) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    hxc_string hxc_l_tmp_string_assignment_replacement_owned_load_result_n491 = hxc_l_tmp_string_assignment_replacement_owner_n223;
                    if (hxc_string_release(&hxc_l_levelLabel) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    hxc_l_levelLabel = hxc_l_tmp_string_assignment_replacement_owned_load_result_n491;
                    hxc_l_gc_roots[161] = (const void *)hxc_l_campaign;
                    struct hxc_caxecraft_content_CampaignManifest *hxc_l_selectedCampaign_h4ab5134c1166 = hxc_l_campaign;
                    hxc_l_gc_roots[162] = (const void *)hxc_l_selectedCampaign_h4ab5134c1166;
                    hxc_l_gc_roots[163] = (const void *)NULL;
                    if (hxc_l_selectedCampaign_h4ab5134c1166 != NULL)
                    {
                      int32_t hxc_l_g_h47f29604085a = 0;
                      struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n494 = hxc_l_selectedCampaign_h4ab5134c1166;
                      hxc_l_gc_roots[164] = (const void *)hxc_l_tmp_load_result_n494;
                      if (hxc_l_tmp_load_result_n494 == NULL)
                      {
                        abort();
                      }
                      struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n495 = (*hxc_l_tmp_load_result_n494).hxc_admittedLevels;
                      hxc_l_gc_roots[165] = (const void *)hxc_l_tmp_class_field_load_result_n495;
                      int32_t hxc_l_tmp_array_length_result_n496;
                      if (hxc_array_ref_length(hxc_l_tmp_class_field_load_result_n495, &hxc_l_tmp_array_length_result_n496) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      int32_t hxc_l_g1_h2642dd239f4c = hxc_l_tmp_array_length_result_n496;
                      while (1)
                      {
                        int32_t hxc_l_tmp_load_result_n497 = hxc_l_g_h47f29604085a;
                        if (!(hxc_l_tmp_load_result_n497 < hxc_l_g1_h2642dd239f4c))
                        {
                          break;
                        }
                        int32_t hxc_l_tmp_increment_load_result_n499 = hxc_l_g_h47f29604085a;
                        hxc_l_g_h47f29604085a = hxc_i32_add_wrapping(hxc_l_tmp_increment_load_result_n499, 1);
                        int32_t hxc_l_index_hf77d8bff3863 = hxc_l_tmp_increment_load_result_n499;
                        struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n500 = hxc_l_selectedCampaign_h4ab5134c1166;
                        hxc_l_gc_roots[173] = (const void *)hxc_l_tmp_load_result_n500;
                        if (hxc_l_tmp_load_result_n500 == NULL)
                        {
                          abort();
                        }
                        struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n501 = (*hxc_l_tmp_load_result_n500).hxc_admittedLevels;
                        hxc_l_gc_roots[174] = (const void *)hxc_l_tmp_class_field_load_result_n501;
                        struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_array_get_result_n503;
                        if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n501, (size_t)hxc_l_index_hf77d8bff3863, &hxc_l_tmp_array_get_result_n503) != HXC_STATUS_OK)
                        {
                          abort();
                        }
                        hxc_l_gc_roots[175] = (const void *)hxc_l_tmp_array_get_result_n503;
                        if (hxc_l_tmp_array_get_result_n503 == NULL)
                        {
                          abort();
                        }
                        hxc_string hxc_l_tmp_class_field_load_result_n504 = (*hxc_l_tmp_array_get_result_n503).hxc_id;
                        struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_load_result_n505 = hxc_l_destination_haaae67a44231;
                        hxc_l_gc_roots[176] = (const void *)hxc_l_tmp_load_result_n505;
                        if (hxc_l_tmp_load_result_n505 == NULL)
                        {
                          abort();
                        }
                        hxc_string hxc_l_tmp_class_field_load_result_n506 = (*hxc_l_tmp_load_result_n505).hxc_id;
                        if (hxc_l_tmp_class_field_load_result_n504.data == NULL || hxc_l_tmp_class_field_load_result_n506.data == NULL ? hxc_l_tmp_class_field_load_result_n504.data == hxc_l_tmp_class_field_load_result_n506.data : hxc_l_tmp_class_field_load_result_n504.byte_length == hxc_l_tmp_class_field_load_result_n506.byte_length && (hxc_l_tmp_class_field_load_result_n504.byte_length == 0 || memcmp(hxc_l_tmp_class_field_load_result_n504.data, hxc_l_tmp_class_field_load_result_n506.data, hxc_l_tmp_class_field_load_result_n504.byte_length) == 0))
                        {
                          hxc_l_selectedCampaignLevelIndex = hxc_l_index_hf77d8bff3863;
                        }
                      }
                    }
                    struct hxc_caxecraft_app_ActivePlayableLevel *hxc_l_tmp_load_result_n509 = hxc_l_activeLevel;
                    hxc_l_gc_roots[166] = (const void *)hxc_l_tmp_load_result_n509;
                    if (hxc_l_tmp_load_result_n509 == NULL)
                    {
                      abort();
                    }
                    struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_class_field_load_result_n510 = (*hxc_l_tmp_load_result_n509).hxc_activeView;
                    hxc_l_gc_roots[167] = (const void *)hxc_l_tmp_class_field_load_result_n510;
                    hxc_l_levelView_hc35c2f93673f = hxc_l_tmp_class_field_load_result_n510;
                    struct hxc_caxecraft_app_ActivePlayableLevel *hxc_l_tmp_load_result_n511 = hxc_l_activeLevel;
                    hxc_l_gc_roots[168] = (const void *)hxc_l_tmp_load_result_n511;
                    if (hxc_l_tmp_load_result_n511 == NULL)
                    {
                      abort();
                    }
                    struct hxc_caxecraft_content_ActiveContent *hxc_l_tmp_class_field_load_result_n512 = (*hxc_l_tmp_load_result_n511).hxc_content;
                    hxc_l_gc_roots[169] = (const void *)hxc_l_tmp_class_field_load_result_n512;
                    if (hxc_l_tmp_class_field_load_result_n512 == NULL)
                    {
                      abort();
                    }
                    struct hxc_caxecraft_content_LoadedContentGeneration *hxc_l_tmp_class_field_load_result_n513 = (*hxc_l_tmp_class_field_load_result_n512).hxc_active;
                    hxc_l_gc_roots[170] = (const void *)hxc_l_tmp_class_field_load_result_n513;
                    if (hxc_l_tmp_class_field_load_result_n513 == NULL)
                    {
                      abort();
                    }
                    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_class_field_load_result_n514 = (*hxc_l_tmp_class_field_load_result_n513).hxc_ownedSession;
                    hxc_l_gc_roots[171] = (const void *)hxc_l_tmp_class_field_load_result_n514;
                    hxc_l_session = hxc_l_tmp_class_field_load_result_n514;
                    struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n515 = hxc_l_session;
                    hxc_l_gc_roots[172] = (const void *)hxc_l_tmp_load_result_n515;
                    if (hxc_l_tmp_load_result_n515 == NULL)
                    {
                      abort();
                    }
                    struct hxc_caxecraft_domain_GameView hxc_l_tmp_instance_call_result_n516 = hxc_caxecraft_domain_GameSession_view(hxc_l_tmp_load_result_n515);
                    hxc_l_initialView = hxc_l_tmp_instance_call_result_n516;
                    if (!hxc_l_initialView.hxc_valid)
                    {
                      hxc_l_quit = true;
                    }
                    else
                    {
                      hxc_l_character = hxc_l_initialView.hxc_localPlayer;
                      hxc_string hxc_l_tmp_string_assignment_replacement_owner_n228 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
                      if (hxc_string_retain(hxc_l_tmp_string_assignment_replacement_owner_n228) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      hxc_string hxc_l_tmp_string_assignment_replacement_owned_load_result_n519 = hxc_l_tmp_string_assignment_replacement_owner_n228;
                      if (hxc_string_release(&hxc_l_activeDialogue) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      hxc_l_activeDialogue = hxc_l_tmp_string_assignment_replacement_owned_load_result_n519;
                      hxc_string hxc_l_tmp_string_assignment_replacement_owner_n229 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
                      if (hxc_string_retain(hxc_l_tmp_string_assignment_replacement_owner_n229) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      hxc_string hxc_l_tmp_string_assignment_replacement_owned_load_result_n520 = hxc_l_tmp_string_assignment_replacement_owner_n229;
                      if (hxc_string_release(&hxc_l_latestJournalId) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      hxc_l_latestJournalId = hxc_l_tmp_string_assignment_replacement_owned_load_result_n520;
                      struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n521 = hxc_l_levelView_hc35c2f93673f;
                      hxc_l_gc_roots[177] = (const void *)hxc_l_tmp_load_result_n521;
                      if (hxc_l_tmp_load_result_n521 == NULL)
                      {
                        abort();
                      }
                      struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_class_field_load_result_n522 = (*hxc_l_tmp_load_result_n521).hxc_levelPresentation;
                      hxc_l_gc_roots[178] = (const void *)hxc_l_tmp_class_field_load_result_n522;
                      if (hxc_l_tmp_class_field_load_result_n522 == NULL)
                      {
                        abort();
                      }
                      hxc_string hxc_l_tmp_class_field_load_result_n523 = (*hxc_l_tmp_class_field_load_result_n522).hxc_startingObjective;
                      hxc_string hxc_l_tmp_string_assignment_replacement_owner_n230 = hxc_l_tmp_class_field_load_result_n523;
                      if (hxc_string_retain(hxc_l_tmp_string_assignment_replacement_owner_n230) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      hxc_string hxc_l_tmp_string_assignment_replacement_owned_load_result_n524 = hxc_l_tmp_string_assignment_replacement_owner_n230;
                      if (hxc_string_release(&hxc_l_currentObjectiveId) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      hxc_l_currentObjectiveId = hxc_l_tmp_string_assignment_replacement_owned_load_result_n524;
                      struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n525 = hxc_l_session;
                      hxc_l_gc_roots[179] = (const void *)hxc_l_tmp_load_result_n525;
                      hxc_l_gc_roots[180] = (const void *)hxc_l_levelView_hc35c2f93673f;
                      struct hxc_caxecraft_app_CaxecraftApp_AvailableInteractionTarget hxc_l_tmp_call_result_n527 = hxc_caxecraft_app_CaxecraftApp_nearestAvailableInteraction(hxc_l_tmp_load_result_n525, hxc_l_levelView_hc35c2f93673f);
                      hxc_l_tmp_static_call_argument_0_owner_n231 = hxc_l_tmp_call_result_n527;
                      enum hxc_caxecraft_app_InteractionPrompt hxc_l_tmp_call_result_n529 = hxc_caxecraft_app_CaxecraftApp_promptForAvailableInteraction(hxc_l_tmp_static_call_argument_0_owner_n231);
                      hxc_l_availableInteractionPrompt = hxc_l_tmp_call_result_n529;
                      struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n530 = hxc_l_session;
                      hxc_l_gc_roots[181] = (const void *)hxc_l_tmp_load_result_n530;
                      if (hxc_l_tmp_load_result_n530 == NULL)
                      {
                        abort();
                      }
                      struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n531 = hxc_l_levelView_hc35c2f93673f;
                      hxc_l_gc_roots[182] = (const void *)hxc_l_tmp_load_result_n531;
                      if (hxc_l_tmp_load_result_n531 == NULL)
                      {
                        abort();
                      }
                      int32_t hxc_l_tmp_class_field_load_result_n532 = (*hxc_l_tmp_load_result_n531).hxc_enemyId;
                      struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n533 = hxc_caxecraft_domain_GameSession_readCharacter(hxc_l_tmp_load_result_n530, hxc_l_tmp_class_field_load_result_n532);
                      hxc_l_enemyActor = hxc_l_tmp_instance_call_result_n533;
                      struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n534 = hxc_l_session;
                      hxc_l_gc_roots[183] = (const void *)hxc_l_tmp_load_result_n534;
                      if (hxc_l_tmp_load_result_n534 == NULL)
                      {
                        abort();
                      }
                      struct hxc_array_ref *hxc_l_tmp_instance_call_result_n535 = hxc_caxecraft_domain_GameSession_actorControllerStateSnapshots(hxc_l_tmp_load_result_n534);
                      hxc_l_phases = hxc_l_tmp_instance_call_result_n535;
                      struct hxc_array_ref *hxc_l_tmp_load_result_n536 = hxc_l_phases;
                      struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n537 = hxc_l_levelView_hc35c2f93673f;
                      hxc_l_gc_roots[184] = (const void *)hxc_l_tmp_load_result_n537;
                      if (hxc_l_tmp_load_result_n537 == NULL)
                      {
                        abort();
                      }
                      int32_t hxc_l_tmp_class_field_load_result_n538 = (*hxc_l_tmp_load_result_n537).hxc_enemyId;
                      struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation hxc_l_tmp_call_result_n540 = hxc_caxecraft_app_CaxecraftApp_observeActorPhase(hxc_l_tmp_load_result_n536, hxc_l_tmp_class_field_load_result_n538, hxc_caxecraft_domain_ActorControllerPhase_Resting);
                      hxc_l_enemyPhase_h695cd3d136cc = hxc_l_tmp_call_result_n540;
                      struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n541 = hxc_l_session;
                      hxc_l_gc_roots[185] = (const void *)hxc_l_tmp_load_result_n541;
                      struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n542 = hxc_l_levelView_hc35c2f93673f;
                      hxc_l_gc_roots[186] = (const void *)hxc_l_tmp_load_result_n542;
                      bool hxc_l_tmp_call_result_n544 = hxc_caxecraft_app_CaxecraftApp_dialogueActorsAreValid(hxc_l_tmp_load_result_n541, hxc_l_tmp_load_result_n542, hxc_l_phases);
                      bool hxc_l_tmp_short_circuit_result_n233 = !hxc_l_tmp_call_result_n544;
                      if (!!hxc_l_tmp_call_result_n544)
                      {
                        hxc_l_tmp_short_circuit_result_n233 = hxc_l_enemyActor.hxc_id <= 0;
                      }
                      bool hxc_l_tmp_short_circuit_load_result_n546 = hxc_l_tmp_short_circuit_result_n233;
                      bool hxc_l_tmp_short_circuit_result_n234 = hxc_l_tmp_short_circuit_load_result_n546;
                      if (!hxc_l_tmp_short_circuit_load_result_n546)
                      {
                        hxc_l_tmp_short_circuit_result_n234 = !hxc_l_enemyPhase_h695cd3d136cc.hxc_valid;
                      }
                      if (hxc_l_tmp_short_circuit_result_n234)
                      {
                        hxc_l_quit = true;
                      }
                      hxc_l_swordCombat = (struct hxc_caxecraft_gameplay_SwordCombatState){ .hxc_cooldownTicks = 0 };
                      hxc_l_berryDrop = (struct hxc_caxecraft_gameplay_BerryDropState){ .hxc_active = false, .hxc_amount = 0, .hxc_x = 0.0, .hxc_y = 0.0, .hxc_z = 0.0 };
                      hxc_l_cameraWaterBlend = 0.0;
                      hxc_l_accumulator = 0.0;
                      struct hxc_caxecraft_app_MotionHistory hxc_l_tmp_call_result_n552 = hxc_caxecraft_app_MotionInterpolation_start(hxc_l_character.hxc_body);
                      hxc_l_motionHistory = hxc_l_tmp_call_result_n552;
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
                      struct hxc_caxecraft_app_TerrainRenderer *hxc_l_tmp_owned_class_field_address_n553 = &(*hxc_l_self).hxc_terrainRenderer;
                      struct hxc_caxecraft_app_TerrainChunkCache *hxc_l_tmp_owned_class_field_address_n554 = &(*hxc_l_tmp_owned_class_field_address_n553).hxc_cache;
                      if (hxc_l_tmp_owned_class_field_address_n554 == NULL)
                      {
                        abort();
                      }
                      hxc_caxecraft_app_TerrainChunkCache_invalidateAll(hxc_l_tmp_owned_class_field_address_n554);
                      hxc_l_resetMotionThisFrame = true;
                      if (hxc_l_requestedFromCampaignMenu)
                      {
                        enum hxc_caxecraft_app_AppScreen hxc_l_tmp_call_result_n557 = hxc_caxecraft_app_AppScreen_startSelectedCampaign(hxc_l_screen);
                        hxc_l_screen = hxc_l_tmp_call_result_n557;
                        hxc_l_recapturedThisFrame = true;
                        DisableCursor();
                      }
                      hxc_string hxc_l_tmp_string_concat_result_n559 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
                      if (hxc_string_concat_ref((hxc_string){ (const uint8_t *)"caxecraft: campaign-level=", 26, true, NULL }, hxc_l_levelLabel, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n559) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      hxc_l_tmp_string_concat_left_owner_n235 = hxc_l_tmp_string_concat_result_n559;
                      hxc_string hxc_l_tmp_string_concat_result_n561 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
                      if (hxc_string_concat_ref(hxc_l_tmp_string_concat_left_owner_n235, (hxc_string){ (const uint8_t *)" generation=", 12, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n561) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      hxc_l_tmp_string_concat_left_owner_n236 = hxc_l_tmp_string_concat_result_n561;
                      hxc_string hxc_l_tmp_string_concat_left_borrow_result_n562 = hxc_l_tmp_string_concat_left_owner_n236;
                      hxc_string hxc_l_tmp_std_string_int_result_n564 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
                      if (hxc_string_from_int32(hxc_l_selected, hxc_default_allocator(), &hxc_l_tmp_std_string_int_result_n564) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      hxc_l_tmp_string_concat_right_owner_n237 = hxc_l_tmp_std_string_int_result_n564;
                      hxc_string hxc_l_tmp_string_concat_result_n566 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
                      if (hxc_string_concat_ref(hxc_l_tmp_string_concat_left_borrow_result_n562, hxc_l_tmp_string_concat_right_owner_n237, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n566) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      hxc_l_tmp_sys_println_string_argument_owner_n238 = hxc_l_tmp_string_concat_result_n566;
                      if (hxc_io_println(hxc_l_tmp_sys_println_string_argument_owner_n238) != HXC_STATUS_OK)
                      {
                        if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n238) != HXC_STATUS_OK)
                        {
                          abort();
                        }
                        if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n237) != HXC_STATUS_OK)
                        {
                          abort();
                        }
                        if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n236) != HXC_STATUS_OK)
                        {
                          abort();
                        }
                        if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n235) != HXC_STATUS_OK)
                        {
                          abort();
                        }
                        if (hxc_array_ref_release(hxc_l_phases) != HXC_STATUS_OK)
                        {
                          abort();
                        }
                        hxc_enum_2c0e695d_destroy(&hxc_l_tmp_static_call_argument_0_owner_n231);
                        hxc_enum_5543d34a_destroy(&hxc_l_g_h2fc2c60edcf1);
                        if (hxc_array_ref_release(hxc_l_initialActorPhases) != HXC_STATUS_OK)
                        {
                          abort();
                        }
                        hxc_enum_2c0e695d_destroy(&hxc_l_tmp_static_call_argument_0_owner_n69);
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
                        hxc_enum_4b68fff2_destroy(&hxc_l_g_h1d08383de4f8);
                        if (hxc_string_release(&hxc_l_pendingCampaignLabel) != HXC_STATUS_OK)
                        {
                          abort();
                        }
                        hxc_enum_fcb4401b_destroy(&hxc_l_g_h4ef171f4fe67);
                        if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
                        {
                          abort();
                        }
                        abort();
                      }
                      if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n238) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n237) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n236) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n235) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      if (hxc_array_ref_release(hxc_l_phases) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      hxc_enum_2c0e695d_destroy(&hxc_l_tmp_static_call_argument_0_owner_n231);
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
              hxc_l_gc_roots[158] = hxc_l_g_h2fc2c60edcf1.hxc_tag == hxc_caxecraft_content_CampaignLevelLoadResult_CampaignLevelReady ? (const void *)hxc_l_g_h2fc2c60edcf1.hxc_payload.hxc_CampaignLevelReady.hxc_candidate : NULL;
              if (hxc_l_g_h2fc2c60edcf1.hxc_tag != hxc_caxecraft_content_CampaignLevelLoadResult_CampaignLevelRejected)
              {
                abort();
              }
              struct hxc_caxecraft_content_CampaignLevelLoadError hxc_l_tmp_enum_payload_project_n569 = hxc_l_g_h2fc2c60edcf1.hxc_payload.hxc_CampaignLevelRejected.hxc_error;
              hxc_l_error = hxc_l_tmp_enum_payload_project_n569;
              if (hxc_enum_40863044_retain(&hxc_l_error) != HXC_STATUS_OK)
              {
                abort();
              }
              hxc_string hxc_l_tmp_call_result_n571 = hxc_caxecraft_app_CaxecraftApp_campaignLevelLoadFailure(hxc_l_error);
              hxc_l_tmp_string_concat_right_owner_n240 = hxc_l_tmp_call_result_n571;
              hxc_string hxc_l_tmp_string_concat_result_n573 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
              if (hxc_string_concat_ref((hxc_string){ (const uint8_t *)"caxecraft: campaign level rejected: ", 36, true, NULL }, hxc_l_tmp_string_concat_right_owner_n240, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n573) != HXC_STATUS_OK)
              {
                abort();
              }
              hxc_l_tmp_sys_println_string_argument_owner_n241 = hxc_l_tmp_string_concat_result_n573;
              if (hxc_io_println(hxc_l_tmp_sys_println_string_argument_owner_n241) != HXC_STATUS_OK)
              {
                if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n241) != HXC_STATUS_OK)
                {
                  abort();
                }
                if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n240) != HXC_STATUS_OK)
                {
                  abort();
                }
                hxc_enum_40863044_destroy(&hxc_l_error);
                hxc_enum_5543d34a_destroy(&hxc_l_g_h2fc2c60edcf1);
                if (hxc_array_ref_release(hxc_l_initialActorPhases) != HXC_STATUS_OK)
                {
                  abort();
                }
                hxc_enum_2c0e695d_destroy(&hxc_l_tmp_static_call_argument_0_owner_n69);
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
                hxc_enum_4b68fff2_destroy(&hxc_l_g_h1d08383de4f8);
                if (hxc_string_release(&hxc_l_pendingCampaignLabel) != HXC_STATUS_OK)
                {
                  abort();
                }
                hxc_enum_fcb4401b_destroy(&hxc_l_g_h4ef171f4fe67);
                if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
                {
                  abort();
                }
                abort();
              }
              if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n241) != HXC_STATUS_OK)
              {
                abort();
              }
              if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n240) != HXC_STATUS_OK)
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
        if (hxc_l_requestedFromAuthoredExit)
        {
          hxc_l_gc_roots[187] = (const void *)NULL;
          hxc_l_pendingCampaignLevel = NULL;
          hxc_string hxc_l_tmp_string_assignment_replacement_owner_n242 = (hxc_string){ (const uint8_t *)"", 0, true, NULL };
          if (hxc_string_retain(hxc_l_tmp_string_assignment_replacement_owner_n242) != HXC_STATUS_OK)
          {
            abort();
          }
          hxc_string hxc_l_tmp_string_assignment_replacement_owned_load_result_n576 = hxc_l_tmp_string_assignment_replacement_owner_n242;
          if (hxc_string_release(&hxc_l_pendingCampaignLabel) != HXC_STATUS_OK)
          {
            abort();
          }
          hxc_l_pendingCampaignLabel = hxc_l_tmp_string_assignment_replacement_owned_load_result_n576;
          hxc_l_loadingFramePresented = false;
          enum hxc_caxecraft_app_AppScreen hxc_l_tmp_call_result_n578 = hxc_caxecraft_app_AppScreen_finishLoading(hxc_l_screen);
          hxc_l_screen = hxc_l_tmp_call_result_n578;
          hxc_l_recapturedThisFrame = true;
          DisableCursor();
        }
        hxc_enum_5543d34a_destroy(&hxc_l_g_h2fc2c60edcf1);
      }
      struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n579 = hxc_l_levelView_hc35c2f93673f;
      hxc_l_gc_roots[152] = (const void *)hxc_l_tmp_load_result_n579;
      if (hxc_l_tmp_load_result_n579 == NULL)
      {
        abort();
      }
      int32_t hxc_l_tmp_class_field_load_result_n580 = (*hxc_l_tmp_load_result_n579).hxc_enemyId;
      int32_t hxc_l_enemyActorId = hxc_l_tmp_class_field_load_result_n580;
      struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n581 = hxc_l_levelView_hc35c2f93673f;
      hxc_l_gc_roots[153] = (const void *)hxc_l_tmp_load_result_n581;
      if (hxc_l_tmp_load_result_n581 == NULL)
      {
        abort();
      }
      struct hxc_caxecraft_scenario_ScenarioTransform hxc_l_tmp_class_field_load_result_n582 = (*hxc_l_tmp_load_result_n581).hxc_playerSpawn;
      struct hxc_caxecraft_scenario_ScenarioTransform hxc_l_spawnTransform = hxc_l_tmp_class_field_load_result_n582;
      if (hxc_l_quitPressed_h953b72f5287d)
      {
        hxc_l_quit = true;
      }
      if (hxc_l_hotbarSelection_h8960df509a08 >= 0)
      {
        struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_load_result_n585 = hxc_l_inventory;
        struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n587 = hxc_caxecraft_gameplay_Inventory_select(hxc_l_tmp_load_result_n585, hxc_l_hotbarSelection_h8960df509a08);
        hxc_l_inventory = hxc_l_tmp_call_result_n587;
      }
      if (hxc_l_hotbarCycle_hea5b398b21a2 != 0)
      {
        struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_load_result_n589 = hxc_l_inventory;
        struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n591 = hxc_caxecraft_gameplay_Inventory_cycle(hxc_l_tmp_load_result_n589, hxc_l_hotbarCycle_hea5b398b21a2);
        hxc_l_inventory = hxc_l_tmp_call_result_n591;
      }
      bool hxc_l_tmp_call_result_n593 = hxc_caxecraft_app_AppScreen_isPlaying(hxc_l_screen);
      bool hxc_l_tmp_short_circuit_result_n245 = hxc_l_tmp_call_result_n593;
      if (hxc_l_tmp_call_result_n593)
      {
        hxc_l_tmp_short_circuit_result_n245 = hxc_l_interactPressed_h601b412c6a26;
      }
      if (hxc_l_tmp_short_circuit_result_n245)
      {
        if (hxc_l_character.hxc_vitals.hxc_health <= 0)
        {
          struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n597 = hxc_l_session;
          hxc_l_gc_roots[188] = (const void *)hxc_l_tmp_load_result_n597;
          if (hxc_l_tmp_load_result_n597 == NULL)
          {
            abort();
          }
          struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n598 = hxc_l_session;
          hxc_l_gc_roots[189] = (const void *)hxc_l_tmp_load_result_n598;
          if (hxc_l_tmp_load_result_n598 == NULL)
          {
            abort();
          }
          size_t hxc_l_tmp_length_n600;
          const uint8_t *hxc_l_tmp_instance_call_result_n599 = hxc_caxecraft_domain_GameSession_worldView(hxc_l_tmp_load_result_n598, &hxc_l_tmp_length_n600);
          struct hxc_caxecraft_domain_CharacterBody hxc_l_tmp_call_result_n602 = hxc_caxecraft_app_CaxecraftApp_spawnPlayer(hxc_l_tmp_instance_call_result_n599, hxc_l_tmp_length_n600, hxc_l_spawnTransform);
          struct hxc_caxecraft_domain_LocalCharacterCommandResult hxc_l_tmp_instance_call_result_n603 = hxc_caxecraft_domain_GameSession_reviveLocalPlayerAt(hxc_l_tmp_load_result_n597, hxc_l_tmp_call_result_n602);
          struct hxc_caxecraft_domain_LocalCharacterCommandResult hxc_l_revival = hxc_l_tmp_instance_call_result_n603;
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
            hxc_string hxc_l_tmp_string_assignment_replacement_owner_n247 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
            if (hxc_string_retain(hxc_l_tmp_string_assignment_replacement_owner_n247) != HXC_STATUS_OK)
            {
              abort();
            }
            hxc_string hxc_l_tmp_string_assignment_replacement_owned_load_result_n607 = hxc_l_tmp_string_assignment_replacement_owner_n247;
            if (hxc_string_release(&hxc_l_activeDialogue) != HXC_STATUS_OK)
            {
              abort();
            }
            hxc_l_activeDialogue = hxc_l_tmp_string_assignment_replacement_owned_load_result_n607;
          }
          else
          {
            struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n608 = hxc_l_session;
            hxc_l_gc_roots[190] = (const void *)hxc_l_tmp_load_result_n608;
            hxc_l_gc_roots[191] = (const void *)hxc_l_levelView_hc35c2f93673f;
            struct hxc_caxecraft_app_CaxecraftApp_AvailableInteractionTarget hxc_l_tmp_call_result_n610 = hxc_caxecraft_app_CaxecraftApp_nearestAvailableInteraction(hxc_l_tmp_load_result_n608, hxc_l_levelView_hc35c2f93673f);
            hxc_l_g_h3c7144a4d6d0 = hxc_l_tmp_call_result_n610;
            switch (hxc_l_g_h3c7144a4d6d0.hxc_tag) {
              case hxc_caxecraft_app_CaxecraftApp_AvailableInteractionTarget_NoAvailableInteraction:
                {
                  break;
                }
              case hxc_caxecraft_app_CaxecraftApp_AvailableInteractionTarget_DialogueInteraction:
                {
                  if (hxc_l_g_h3c7144a4d6d0.hxc_tag != hxc_caxecraft_app_CaxecraftApp_AvailableInteractionTarget_DialogueInteraction)
                  {
                    abort();
                  }
                  int32_t hxc_l_tmp_enum_payload_project_n613 = hxc_l_g_h3c7144a4d6d0.hxc_payload.hxc_DialogueInteraction.hxc_id;
                  int32_t hxc_l_id_hdc50ff41f247 = hxc_l_tmp_enum_payload_project_n613;
                  struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n614 = hxc_l_session;
                  hxc_l_gc_roots[192] = (const void *)hxc_l_tmp_load_result_n614;
                  if (hxc_l_tmp_load_result_n614 == NULL)
                  {
                    abort();
                  }
                  bool hxc_l_tmp_instance_call_result_n616 = hxc_caxecraft_domain_GameSession_interactWithActor(hxc_l_tmp_load_result_n614, hxc_l_id_hdc50ff41f247);
                  if (!hxc_l_tmp_instance_call_result_n616)
                  {
                    hxc_l_quit = true;
                  }
                  break;
                }
              case hxc_caxecraft_app_CaxecraftApp_AvailableInteractionTarget_StatefulObjectInteraction:
                {
                  if (hxc_l_g_h3c7144a4d6d0.hxc_tag != hxc_caxecraft_app_CaxecraftApp_AvailableInteractionTarget_StatefulObjectInteraction)
                  {
                    abort();
                  }
                  hxc_string hxc_l_tmp_enum_payload_project_n618 = hxc_l_g_h3c7144a4d6d0.hxc_payload.hxc_StatefulObjectInteraction.hxc_id;
                  hxc_l_id_h5c812fd13c48 = hxc_l_tmp_enum_payload_project_n618;
                  if (hxc_string_retain(hxc_l_id_h5c812fd13c48) != HXC_STATUS_OK)
                  {
                    abort();
                  }
                  struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n619 = hxc_l_session;
                  hxc_l_gc_roots[193] = (const void *)hxc_l_tmp_load_result_n619;
                  if (hxc_l_tmp_load_result_n619 == NULL)
                  {
                    abort();
                  }
                  bool hxc_l_tmp_instance_call_result_n621 = hxc_caxecraft_domain_GameSession_interactWithStatefulObject(hxc_l_tmp_load_result_n619, hxc_l_id_h5c812fd13c48);
                  if (!hxc_l_tmp_instance_call_result_n621)
                  {
                    hxc_l_quit = true;
                  }
                  if (hxc_string_release(&hxc_l_id_h5c812fd13c48) != HXC_STATUS_OK)
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
            hxc_enum_2c0e695d_destroy(&hxc_l_g_h3c7144a4d6d0);
          }
        }
      }
      bool hxc_l_tmp_load_result_n622 = hxc_l_focused;
      bool hxc_l_tmp_short_circuit_result_n251 = !hxc_l_tmp_load_result_n622;
      if (!hxc_l_tmp_load_result_n622)
      {
        bool hxc_l_tmp_call_result_n624 = hxc_caxecraft_app_AppScreen_isPlaying(hxc_l_screen);
        hxc_l_tmp_short_circuit_result_n251 = hxc_l_tmp_call_result_n624;
      }
      if (hxc_l_tmp_short_circuit_result_n251)
      {
        enum hxc_caxecraft_app_AppScreen hxc_l_tmp_call_result_n627 = hxc_caxecraft_app_AppScreen_loseFocus(hxc_l_screen);
        hxc_l_screen = hxc_l_tmp_call_result_n627;
        hxc_l_jumpQueued = false;
        hxc_l_accumulator = 0.0;
        hxc_l_resetMotionThisFrame = true;
        EnableCursor();
      }
      bool hxc_l_tmp_call_result_n629 = hxc_caxecraft_app_AppScreen_showsTitle(hxc_l_screen);
      bool hxc_l_tmp_short_circuit_result_n252 = !hxc_l_tmp_call_result_n629;
      if (!hxc_l_tmp_call_result_n629)
      {
        bool hxc_l_tmp_call_result_n631 = hxc_caxecraft_app_AppScreen_showsCampaignSelection(hxc_l_screen);
        hxc_l_tmp_short_circuit_result_n252 = !hxc_l_tmp_call_result_n631;
      }
      bool hxc_l_tmp_short_circuit_load_result_n632 = hxc_l_tmp_short_circuit_result_n252;
      bool hxc_l_tmp_short_circuit_result_n253 = hxc_l_tmp_short_circuit_load_result_n632;
      if (hxc_l_tmp_short_circuit_load_result_n632)
      {
        bool hxc_l_tmp_call_result_n634 = hxc_caxecraft_app_AppScreen_showsLoading(hxc_l_screen);
        hxc_l_tmp_short_circuit_result_n253 = !hxc_l_tmp_call_result_n634;
      }
      bool hxc_l_tmp_short_circuit_load_result_n635 = hxc_l_tmp_short_circuit_result_n253;
      bool hxc_l_tmp_short_circuit_result_n254 = hxc_l_tmp_short_circuit_load_result_n635;
      if (hxc_l_tmp_short_circuit_load_result_n635)
      {
        bool hxc_l_tmp_call_result_n637 = hxc_caxecraft_app_AppScreen_showsEditor(hxc_l_screen);
        hxc_l_tmp_short_circuit_result_n254 = !hxc_l_tmp_call_result_n637;
      }
      bool hxc_l_tmp_short_circuit_load_result_n638 = hxc_l_tmp_short_circuit_result_n254;
      bool hxc_l_tmp_short_circuit_result_n255 = hxc_l_tmp_short_circuit_load_result_n638;
      if (hxc_l_tmp_short_circuit_load_result_n638)
      {
        hxc_l_tmp_short_circuit_result_n255 = hxc_l_focused;
      }
      bool hxc_l_tmp_short_circuit_load_result_n640 = hxc_l_tmp_short_circuit_result_n255;
      bool hxc_l_tmp_short_circuit_result_n256 = hxc_l_tmp_short_circuit_load_result_n640;
      if (hxc_l_tmp_short_circuit_load_result_n640)
      {
        hxc_l_tmp_short_circuit_result_n256 = hxc_l_pausePressed_h373f04990d15;
      }
      if (hxc_l_tmp_short_circuit_result_n256)
      {
        enum hxc_caxecraft_app_AppScreen hxc_l_tmp_call_result_n644 = hxc_caxecraft_app_AppScreen_togglePause(hxc_l_screen);
        hxc_l_screen = hxc_l_tmp_call_result_n644;
        hxc_l_accumulator = 0.0;
        hxc_l_resetMotionThisFrame = true;
        bool hxc_l_tmp_call_result_n646 = hxc_caxecraft_app_AppScreen_pausesSimulation(hxc_l_screen);
        if (hxc_l_tmp_call_result_n646)
        {
          hxc_l_jumpQueued = false;
        }
        bool hxc_l_tmp_call_result_n648 = hxc_caxecraft_app_AppScreen_capturesPointer(hxc_l_screen);
        if (hxc_l_tmp_call_result_n648)
        {
          DisableCursor();
        }
        else
        {
          EnableCursor();
        }
      }
      bool hxc_l_tmp_call_result_n650 = hxc_caxecraft_app_AppScreen_showsTitle(hxc_l_screen);
      bool hxc_l_tmp_short_circuit_result_n257 = !hxc_l_tmp_call_result_n650;
      if (!hxc_l_tmp_call_result_n650)
      {
        bool hxc_l_tmp_call_result_n652 = hxc_caxecraft_app_AppScreen_showsCampaignSelection(hxc_l_screen);
        hxc_l_tmp_short_circuit_result_n257 = !hxc_l_tmp_call_result_n652;
      }
      bool hxc_l_tmp_short_circuit_load_result_n653 = hxc_l_tmp_short_circuit_result_n257;
      bool hxc_l_tmp_short_circuit_result_n258 = hxc_l_tmp_short_circuit_load_result_n653;
      if (hxc_l_tmp_short_circuit_load_result_n653)
      {
        bool hxc_l_tmp_call_result_n655 = hxc_caxecraft_app_AppScreen_showsLoading(hxc_l_screen);
        hxc_l_tmp_short_circuit_result_n258 = !hxc_l_tmp_call_result_n655;
      }
      bool hxc_l_tmp_short_circuit_load_result_n656 = hxc_l_tmp_short_circuit_result_n258;
      bool hxc_l_tmp_short_circuit_result_n259 = hxc_l_tmp_short_circuit_load_result_n656;
      if (hxc_l_tmp_short_circuit_load_result_n656)
      {
        bool hxc_l_tmp_call_result_n658 = hxc_caxecraft_app_AppScreen_showsEditor(hxc_l_screen);
        hxc_l_tmp_short_circuit_result_n259 = !hxc_l_tmp_call_result_n658;
      }
      bool hxc_l_tmp_short_circuit_load_result_n659 = hxc_l_tmp_short_circuit_result_n259;
      bool hxc_l_tmp_short_circuit_result_n260 = hxc_l_tmp_short_circuit_load_result_n659;
      if (hxc_l_tmp_short_circuit_load_result_n659)
      {
        hxc_l_tmp_short_circuit_result_n260 = hxc_l_focused;
      }
      bool hxc_l_tmp_short_circuit_load_result_n661 = hxc_l_tmp_short_circuit_result_n260;
      bool hxc_l_tmp_short_circuit_result_n261 = hxc_l_tmp_short_circuit_load_result_n661;
      if (hxc_l_tmp_short_circuit_load_result_n661)
      {
        bool hxc_l_tmp_call_result_n663 = hxc_caxecraft_app_AppScreen_pausesSimulation(hxc_l_screen);
        hxc_l_tmp_short_circuit_result_n261 = hxc_l_tmp_call_result_n663;
      }
      bool hxc_l_tmp_short_circuit_load_result_n664 = hxc_l_tmp_short_circuit_result_n261;
      bool hxc_l_tmp_short_circuit_result_n262 = hxc_l_tmp_short_circuit_load_result_n664;
      if (hxc_l_tmp_short_circuit_load_result_n664)
      {
        hxc_l_tmp_short_circuit_result_n262 = hxc_l_capturePressed;
      }
      if (hxc_l_tmp_short_circuit_result_n262)
      {
        enum hxc_caxecraft_app_AppScreen hxc_l_tmp_call_result_n668 = hxc_caxecraft_app_AppScreen_recapture(hxc_l_screen);
        hxc_l_screen = hxc_l_tmp_call_result_n668;
        hxc_l_recapturedThisFrame = true;
        hxc_l_accumulator = 0.0;
        hxc_l_resetMotionThisFrame = true;
        DisableCursor();
      }
      bool hxc_l_tmp_call_result_n670 = hxc_caxecraft_app_AppScreen_showsTitle(hxc_l_screen);
      bool hxc_l_onTitle = hxc_l_tmp_call_result_n670;
      bool hxc_l_tmp_call_result_n672 = hxc_caxecraft_app_AppScreen_showsCampaignSelection(hxc_l_screen);
      bool hxc_l_onCampaignSelect = hxc_l_tmp_call_result_n672;
      bool hxc_l_tmp_call_result_n674 = hxc_caxecraft_app_AppScreen_showsLoading(hxc_l_screen);
      bool hxc_l_onLoading = hxc_l_tmp_call_result_n674;
      bool hxc_l_tmp_call_result_n676 = hxc_caxecraft_app_AppScreen_showsEditor(hxc_l_screen);
      bool hxc_l_onEditor = hxc_l_tmp_call_result_n676;
      bool hxc_l_tmp_call_result_n678 = hxc_caxecraft_app_AppScreen_pausesSimulation(hxc_l_screen);
      bool hxc_l_paused_h86b65f99f244 = hxc_l_tmp_call_result_n678;
      bool hxc_l_tmp_call_result_n680 = hxc_caxecraft_app_AppScreen_capturesPointer(hxc_l_screen);
      bool hxc_l_captured_h44c4589a3844 = hxc_l_tmp_call_result_n680;
      int32_t hxc_l_editorNavigationCommand = 0;
      if (hxc_l_onEditor)
      {
        if (hxc_l_self == NULL)
        {
          abort();
        }
        struct hxc_caxecraft_input_NavigationRepeater *hxc_l_tmp_owned_class_field_address_n682 = &(*hxc_l_self).hxc_editorNavigation;
        if (hxc_l_tmp_owned_class_field_address_n682 == NULL)
        {
          abort();
        }
        struct hxc_caxecraft_input_NavigationSample hxc_l_tmp_call_result_n683 = hxc_caxecraft_app_RaylibNavigationInput_samplePrimaryGamepad();
        int32_t hxc_l_tmp_instance_call_result_n685 = hxc_caxecraft_input_NavigationRepeater_advance(hxc_l_tmp_owned_class_field_address_n682, hxc_l_tmp_call_result_n683, hxc_l_frameSeconds);
        hxc_l_editorNavigationCommand = hxc_l_tmp_instance_call_result_n685;
      }
      else
      {
        if (hxc_l_self == NULL)
        {
          abort();
        }
        struct hxc_caxecraft_input_NavigationRepeater *hxc_l_tmp_owned_class_field_address_n686 = &(*hxc_l_self).hxc_editorNavigation;
        if (hxc_l_tmp_owned_class_field_address_n686 == NULL)
        {
          abort();
        }
        hxc_caxecraft_input_NavigationRepeater_release(hxc_l_tmp_owned_class_field_address_n686);
      }
      if (hxc_l_captured_h44c4589a3844)
      {
        double hxc_l_yawDelta = hxc_l_lookYaw_hbb153fd12aee;
        if (hxc_l_yawDelta > 0.25)
        {
          hxc_l_yawDelta = 0.25;
        }
        if (hxc_l_yawDelta < -0.25)
        {
          hxc_l_yawDelta = -0.25;
        }
        double hxc_l_tmp_load_result_n691 = hxc_l_lookX;
        double hxc_l_tmp_load_result_n692 = hxc_l_yawDelta;
        double hxc_l_candidateX = hxc_l_tmp_load_result_n691 + hxc_l_tmp_load_result_n692 * hxc_l_lookZ;
        double hxc_l_tmp_load_result_n694 = hxc_l_lookZ;
        double hxc_l_tmp_load_result_n695 = hxc_l_yawDelta;
        double hxc_l_candidateZ = hxc_l_tmp_load_result_n694 - hxc_l_tmp_load_result_n695 * hxc_l_lookX;
        double hxc_l_tmp_load_result_n697 = hxc_l_candidateX;
        double hxc_l_tmp_load_result_n698 = hxc_l_candidateX;
        double hxc_l_tmp_load_result_n699 = hxc_l_candidateZ;
        double hxc_l_lengthSquared = hxc_l_tmp_load_result_n697 * hxc_l_tmp_load_result_n698 + hxc_l_tmp_load_result_n699 * hxc_l_candidateZ;
        double hxc_l_normalize = 1.5 - 0.5 * hxc_l_lengthSquared;
        double hxc_l_tmp_load_result_n702 = hxc_l_candidateX;
        hxc_l_lookX = hxc_l_tmp_load_result_n702 * hxc_l_normalize;
        double hxc_l_tmp_load_result_n704 = hxc_l_candidateZ;
        hxc_l_lookZ = hxc_l_tmp_load_result_n704 * hxc_l_normalize;
        double hxc_l_tmp_compound_load_result_n706 = hxc_l_lookY;
        hxc_l_lookY = hxc_l_tmp_compound_load_result_n706 + hxc_l_lookPitch_h64f9d3a8873d;
        if (hxc_l_lookY > 0.90)
        {
          hxc_l_lookY = 0.90;
        }
        if (hxc_l_lookY < -0.90)
        {
          hxc_l_lookY = -0.90;
        }
        if (hxc_l_jumpPressed_h00e33ed98ee9)
        {
          hxc_l_jumpQueued = true;
        }
      }
      bool hxc_l_tmp_load_result_n711 = hxc_l_captured_h44c4589a3844;
      bool hxc_l_tmp_short_circuit_result_n275 = hxc_l_tmp_load_result_n711;
      if (hxc_l_tmp_load_result_n711)
      {
        hxc_l_tmp_short_circuit_result_n275 = !hxc_l_recapturedThisFrame;
      }
      bool hxc_l_tmp_short_circuit_load_result_n713 = hxc_l_tmp_short_circuit_result_n275;
      bool hxc_l_tmp_short_circuit_result_n276 = hxc_l_tmp_short_circuit_load_result_n713;
      if (hxc_l_tmp_short_circuit_load_result_n713)
      {
        hxc_l_tmp_short_circuit_result_n276 = hxc_l_primaryPressed_hfeea7ddbdc7b;
      }
      bool hxc_l_tmp_short_circuit_load_result_n715 = hxc_l_tmp_short_circuit_result_n276;
      bool hxc_l_tmp_short_circuit_result_n277 = hxc_l_tmp_short_circuit_load_result_n715;
      if (hxc_l_tmp_short_circuit_load_result_n715)
      {
        hxc_l_tmp_short_circuit_result_n277 = hxc_l_selectedMode == 1;
      }
      bool hxc_l_tmp_short_circuit_load_result_n717 = hxc_l_tmp_short_circuit_result_n277;
      bool hxc_l_tmp_short_circuit_result_n278 = hxc_l_tmp_short_circuit_load_result_n717;
      if (hxc_l_tmp_short_circuit_load_result_n717)
      {
        bool hxc_l_tmp_call_result_n720 = hxc_caxecraft_gameplay_Inventory_selectedIs(hxc_l_inventory, hxc_caxecraft_gameplay_ItemKind_CopperSword);
        hxc_l_tmp_short_circuit_result_n278 = hxc_l_tmp_call_result_n720;
      }
      if (hxc_l_tmp_short_circuit_result_n278)
      {
        hxc_l_swordQueued = true;
      }
      if (!hxc_l_paused_h86b65f99f244)
      {
        double hxc_l_tmp_compound_load_result_n723 = hxc_l_accumulator;
        hxc_l_accumulator = hxc_l_tmp_compound_load_result_n723 + hxc_l_frameSeconds;
      }
      while (1)
      {
        bool hxc_l_tmp_call_result_n726 = hxc_caxecraft_app_AppScreen_pausesSimulation(hxc_l_screen);
        bool hxc_l_tmp_short_circuit_result_n279 = !hxc_l_tmp_call_result_n726;
        if (!hxc_l_tmp_call_result_n726)
        {
          hxc_l_tmp_short_circuit_result_n279 = hxc_l_accumulator >= 0.05;
        }
        if (!hxc_l_tmp_short_circuit_result_n279)
        {
          break;
        }
        double hxc_l_tmp_load_result_n729 = hxc_l_moveForward;
        double hxc_l_tmp_load_result_n730 = hxc_l_lookX;
        double hxc_l_tmp_load_result_n731 = hxc_l_moveRight;
        double hxc_l_moveX = hxc_l_tmp_load_result_n729 * hxc_l_tmp_load_result_n730 - hxc_l_tmp_load_result_n731 * hxc_l_lookZ;
        double hxc_l_tmp_load_result_n733 = hxc_l_moveForward;
        double hxc_l_tmp_load_result_n734 = hxc_l_lookZ;
        double hxc_l_tmp_load_result_n735 = hxc_l_moveRight;
        double hxc_l_moveZ = hxc_l_tmp_load_result_n733 * hxc_l_tmp_load_result_n734 + hxc_l_tmp_load_result_n735 * hxc_l_lookX;
        double hxc_l_tmp_load_result_n737 = hxc_l_moveForward;
        bool hxc_l_tmp_short_circuit_result_n282 = hxc_l_tmp_load_result_n737 != 0.0;
        if (hxc_l_tmp_load_result_n737 != 0.0)
        {
          hxc_l_tmp_short_circuit_result_n282 = hxc_l_moveRight != 0.0;
        }
        if (hxc_l_tmp_short_circuit_result_n282)
        {
          hxc_l_moveX = hxc_l_moveX * 0.7071067811865476;
          hxc_l_moveZ = hxc_l_moveZ * 0.7071067811865476;
        }
        int32_t hxc_l_tmp_load_result_n742 = hxc_l_selectedMode;
        int32_t hxc_l_tmp_conditional_result_n284 = 0;
        if (hxc_l_tmp_load_result_n742 == 1)
        {
          hxc_l_tmp_conditional_result_n284 = 1;
        }
        else
        {
          hxc_l_tmp_conditional_result_n284 = 0;
        }
        int32_t hxc_l_damagePolicy = hxc_l_tmp_conditional_result_n284;
        bool hxc_l_tmp_load_result_n744 = hxc_l_jumpQueued;
        bool hxc_l_tmp_short_circuit_result_n286 = hxc_l_tmp_load_result_n744;
        if (!hxc_l_tmp_load_result_n744)
        {
          int32_t hxc_l_tmp_record_field_load_result_n745 = hxc_l_character.hxc_aquatic.hxc_medium;
          bool hxc_l_tmp_short_circuit_result_n287 = hxc_l_tmp_record_field_load_result_n745 != 0;
          if (hxc_l_tmp_record_field_load_result_n745 != 0)
          {
            hxc_l_tmp_short_circuit_result_n287 = hxc_l_riseHeld;
          }
          hxc_l_tmp_short_circuit_result_n286 = hxc_l_tmp_short_circuit_result_n287;
        }
        bool hxc_l_rising = hxc_l_tmp_short_circuit_result_n286;
        struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n749 = hxc_l_session;
        hxc_l_gc_roots[195] = (const void *)hxc_l_tmp_load_result_n749;
        if (hxc_l_tmp_load_result_n749 == NULL)
        {
          abort();
        }
        double hxc_l_tmp_load_result_n750 = hxc_l_moveX;
        double hxc_l_tmp_load_result_n751 = hxc_l_moveZ;
        bool hxc_l_tmp_load_result_n752 = hxc_l_rising;
        struct hxc_caxecraft_domain_CharacterIntent hxc_l_tmp_call_result_n754 = hxc_caxecraft_domain_Aquatics_input(hxc_l_tmp_load_result_n750, hxc_l_tmp_load_result_n751, hxc_l_tmp_load_result_n752, hxc_l_descendHeld_h3bc0f8055fa9);
        struct hxc_caxecraft_domain_GameTickResult hxc_l_tmp_instance_call_result_n757 = hxc_caxecraft_domain_GameSession_tick(hxc_l_tmp_load_result_n749, (struct hxc_caxecraft_domain_GameTickInput){ .hxc_damagePolicy = hxc_l_damagePolicy, .hxc_intent = hxc_l_tmp_call_result_n754, .hxc_waterUpdateBudget = 64 });
        hxc_l_gameTick = hxc_l_tmp_instance_call_result_n757;
        hxc_l_character = hxc_l_gameTick.hxc_character;
        hxc_l_flow = hxc_l_gameTick.hxc_flow;
        if (hxc_optional_b8d3265c_retain(&hxc_l_flow) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_l_flow.hxc_has_value)
        {
          struct hxc_optional_caxecraft_scenario_FlowTickResult hxc_l_tmp_load_result_n761 = hxc_l_flow;
          if (!hxc_l_tmp_load_result_n761.hxc_has_value)
          {
            abort();
          }
          int32_t hxc_l_tmp_array_length_result_n764;
          if (hxc_array_ref_length(hxc_l_tmp_load_result_n761.hxc_value.hxc_diagnostics, &hxc_l_tmp_array_length_result_n764) != HXC_STATUS_OK)
          {
            abort();
          }
          if (hxc_l_tmp_array_length_result_n764 == 0)
          {
            int32_t hxc_l_g_h09ddfce788ab = 0;
            struct hxc_optional_caxecraft_scenario_FlowTickResult hxc_l_tmp_load_result_n765 = hxc_l_flow;
            if (!hxc_l_tmp_load_result_n765.hxc_has_value)
            {
              abort();
            }
            hxc_l_g1_h86761ddd48e2 = hxc_l_tmp_load_result_n765.hxc_value.hxc_presentation;
            if (hxc_array_ref_retain(hxc_l_g1_h86761ddd48e2) != HXC_STATUS_OK)
            {
              abort();
            }
            while (1)
            {
              int32_t hxc_l_tmp_load_result_n768 = hxc_l_g_h09ddfce788ab;
              int32_t hxc_l_tmp_array_length_result_n770;
              if (hxc_array_ref_length(hxc_l_g1_h86761ddd48e2, &hxc_l_tmp_array_length_result_n770) != HXC_STATUS_OK)
              {
                abort();
              }
              if (!(hxc_l_tmp_load_result_n768 < hxc_l_tmp_array_length_result_n770))
              {
                break;
              }
              struct hxc_array_ref *hxc_l_tmp_load_result_n771 = hxc_l_g1_h86761ddd48e2;
              struct hxc_caxecraft_scenario_FlowPresentationEvent hxc_l_tmp_array_get_result_n773;
              if (hxc_array_ref_get_copy(hxc_l_tmp_load_result_n771, (size_t)hxc_l_g_h09ddfce788ab, &hxc_l_tmp_array_get_result_n773) != HXC_STATUS_OK)
              {
                abort();
              }
              hxc_l_tmp_array_element_owner_n293 = hxc_l_tmp_array_get_result_n773;
              hxc_l_event_h6e7c030f6037 = hxc_l_tmp_array_element_owner_n293;
              if (hxc_enum_147eda7c_retain(&hxc_l_event_h6e7c030f6037) != HXC_STATUS_OK)
              {
                abort();
              }
              hxc_l_g_h09ddfce788ab = hxc_i32_add_wrapping(hxc_l_g_h09ddfce788ab, 1);
              switch (hxc_l_event_h6e7c030f6037.hxc_tag) {
                case hxc_caxecraft_scenario_FlowPresentationEvent_DialogueRequested:
                  {
                    if (hxc_l_event_h6e7c030f6037.hxc_tag != hxc_caxecraft_scenario_FlowPresentationEvent_DialogueRequested)
                    {
                      abort();
                    }
                    hxc_string hxc_l_tmp_enum_payload_project_n778 = hxc_l_event_h6e7c030f6037.hxc_payload.hxc_DialogueRequested.hxc_id;
                    hxc_l_id_h738abc8a43d4 = hxc_l_tmp_enum_payload_project_n778;
                    if (hxc_string_retain(hxc_l_id_h738abc8a43d4) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    hxc_string hxc_l_tmp_string_assignment_replacement_owner_n295 = hxc_l_id_h738abc8a43d4;
                    if (hxc_string_retain(hxc_l_tmp_string_assignment_replacement_owner_n295) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    hxc_string hxc_l_tmp_string_assignment_replacement_owned_load_result_n780 = hxc_l_tmp_string_assignment_replacement_owner_n295;
                    if (hxc_string_release(&hxc_l_activeDialogue) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    hxc_l_activeDialogue = hxc_l_tmp_string_assignment_replacement_owned_load_result_n780;
                    if (hxc_string_release(&hxc_l_id_h738abc8a43d4) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    break;
                  }
                case hxc_caxecraft_scenario_FlowPresentationEvent_JournalAdded:
                  {
                    if (hxc_l_event_h6e7c030f6037.hxc_tag != hxc_caxecraft_scenario_FlowPresentationEvent_JournalAdded)
                    {
                      abort();
                    }
                    hxc_string hxc_l_tmp_enum_payload_project_n782 = hxc_l_event_h6e7c030f6037.hxc_payload.hxc_JournalAdded.hxc_id;
                    hxc_l_id_h8f15d9a191bc = hxc_l_tmp_enum_payload_project_n782;
                    if (hxc_string_retain(hxc_l_id_h8f15d9a191bc) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    hxc_string hxc_l_tmp_string_assignment_replacement_owner_n297 = hxc_l_id_h8f15d9a191bc;
                    if (hxc_string_retain(hxc_l_tmp_string_assignment_replacement_owner_n297) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    hxc_string hxc_l_tmp_string_assignment_replacement_owned_load_result_n784 = hxc_l_tmp_string_assignment_replacement_owner_n297;
                    if (hxc_string_release(&hxc_l_latestJournalId) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    hxc_l_latestJournalId = hxc_l_tmp_string_assignment_replacement_owned_load_result_n784;
                    if (hxc_string_release(&hxc_l_id_h8f15d9a191bc) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    break;
                  }
                case hxc_caxecraft_scenario_FlowPresentationEvent_CampaignExitRequested:
                  {
                    if (hxc_l_event_h6e7c030f6037.hxc_tag != hxc_caxecraft_scenario_FlowPresentationEvent_CampaignExitRequested)
                    {
                      abort();
                    }
                    hxc_string hxc_l_tmp_enum_payload_project_n786 = hxc_l_event_h6e7c030f6037.hxc_payload.hxc_CampaignExitRequested.hxc_exit;
                    hxc_l_exit = hxc_l_tmp_enum_payload_project_n786;
                    if (hxc_string_retain(hxc_l_exit) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    hxc_l_gc_roots[196] = (const void *)hxc_l_campaign;
                    struct hxc_caxecraft_content_CampaignManifest *hxc_l_selectedCampaign_hb23a0d107c02 = hxc_l_campaign;
                    hxc_l_gc_roots[197] = (const void *)hxc_l_campaignLevel;
                    struct hxc_caxecraft_content_CampaignLevel *hxc_l_sourceLevel_h85921cb5ca4a = hxc_l_campaignLevel;
                    int32_t hxc_l_tmp_load_result_n789 = hxc_l_selectedMode;
                    bool hxc_l_tmp_short_circuit_result_n301 = hxc_l_tmp_load_result_n789 != 1;
                    if (!(hxc_l_tmp_load_result_n789 != 1))
                    {
                      bool hxc_l_tmp_call_result_n791 = hxc_caxecraft_app_AppScreen_isPlaying(hxc_l_screen);
                      hxc_l_tmp_short_circuit_result_n301 = !hxc_l_tmp_call_result_n791;
                    }
                    if (hxc_l_tmp_short_circuit_result_n301)
                    {
                      hxc_string hxc_l_tmp_string_concat_result_n794 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
                      if (hxc_string_concat_ref((hxc_string){ (const uint8_t *)"caxecraft: ignored campaign exit outside active Adventure play: ", 64, true, NULL }, hxc_l_exit, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n794) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      hxc_l_tmp_sys_println_string_argument_owner_n302 = hxc_l_tmp_string_concat_result_n794;
                      if (hxc_io_println(hxc_l_tmp_sys_println_string_argument_owner_n302) != HXC_STATUS_OK)
                      {
                        if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n302) != HXC_STATUS_OK)
                        {
                          abort();
                        }
                        if (hxc_string_release(&hxc_l_exit) != HXC_STATUS_OK)
                        {
                          abort();
                        }
                        hxc_enum_147eda7c_destroy(&hxc_l_event_h6e7c030f6037);
                        hxc_array_ce94fc36_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n293);
                        if (hxc_array_ref_release(hxc_l_g1_h86761ddd48e2) != HXC_STATUS_OK)
                        {
                          abort();
                        }
                        hxc_optional_b8d3265c_destroy(&hxc_l_flow);
                        hxc_record_9ab6a4ce_destroy(&hxc_l_gameTick);
                        if (hxc_array_ref_release(hxc_l_initialActorPhases) != HXC_STATUS_OK)
                        {
                          abort();
                        }
                        hxc_enum_2c0e695d_destroy(&hxc_l_tmp_static_call_argument_0_owner_n69);
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
                        hxc_enum_4b68fff2_destroy(&hxc_l_g_h1d08383de4f8);
                        if (hxc_string_release(&hxc_l_pendingCampaignLabel) != HXC_STATUS_OK)
                        {
                          abort();
                        }
                        hxc_enum_fcb4401b_destroy(&hxc_l_g_h4ef171f4fe67);
                        if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
                        {
                          abort();
                        }
                        abort();
                      }
                      if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n302) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                    }
                    else
                    {
                      struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n796 = hxc_l_selectedCampaign_hb23a0d107c02;
                      hxc_l_gc_roots[198] = (const void *)hxc_l_tmp_load_result_n796;
                      hxc_l_gc_roots[199] = (const void *)NULL;
                      bool hxc_l_tmp_short_circuit_result_n303 = hxc_l_tmp_load_result_n796 == NULL;
                      if (!(hxc_l_tmp_load_result_n796 == NULL))
                      {
                        hxc_l_gc_roots[200] = (const void *)hxc_l_sourceLevel_h85921cb5ca4a;
                        hxc_l_gc_roots[201] = (const void *)NULL;
                        hxc_l_tmp_short_circuit_result_n303 = hxc_l_sourceLevel_h85921cb5ca4a == NULL;
                      }
                      if (hxc_l_tmp_short_circuit_result_n303)
                      {
                        hxc_string hxc_l_tmp_string_concat_result_n800 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
                        if (hxc_string_concat_ref((hxc_string){ (const uint8_t *)"caxecraft: ignored campaign exit without an active campaign: ", 61, true, NULL }, hxc_l_exit, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n800) != HXC_STATUS_OK)
                        {
                          abort();
                        }
                        hxc_l_tmp_sys_println_string_argument_owner_n304 = hxc_l_tmp_string_concat_result_n800;
                        if (hxc_io_println(hxc_l_tmp_sys_println_string_argument_owner_n304) != HXC_STATUS_OK)
                        {
                          if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n304) != HXC_STATUS_OK)
                          {
                            abort();
                          }
                          if (hxc_string_release(&hxc_l_exit) != HXC_STATUS_OK)
                          {
                            abort();
                          }
                          hxc_enum_147eda7c_destroy(&hxc_l_event_h6e7c030f6037);
                          hxc_array_ce94fc36_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n293);
                          if (hxc_array_ref_release(hxc_l_g1_h86761ddd48e2) != HXC_STATUS_OK)
                          {
                            abort();
                          }
                          hxc_optional_b8d3265c_destroy(&hxc_l_flow);
                          hxc_record_9ab6a4ce_destroy(&hxc_l_gameTick);
                          if (hxc_array_ref_release(hxc_l_initialActorPhases) != HXC_STATUS_OK)
                          {
                            abort();
                          }
                          hxc_enum_2c0e695d_destroy(&hxc_l_tmp_static_call_argument_0_owner_n69);
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
                          hxc_enum_4b68fff2_destroy(&hxc_l_g_h1d08383de4f8);
                          if (hxc_string_release(&hxc_l_pendingCampaignLabel) != HXC_STATUS_OK)
                          {
                            abort();
                          }
                          hxc_enum_fcb4401b_destroy(&hxc_l_g_h4ef171f4fe67);
                          if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
                          {
                            abort();
                          }
                          abort();
                        }
                        if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n304) != HXC_STATUS_OK)
                        {
                          abort();
                        }
                      }
                      else
                      {
                        struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n802 = hxc_l_selectedCampaign_hb23a0d107c02;
                        hxc_l_gc_roots[202] = (const void *)hxc_l_tmp_load_result_n802;
                        if (hxc_l_tmp_load_result_n802 == NULL)
                        {
                          abort();
                        }
                        struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_load_result_n803 = hxc_l_sourceLevel_h85921cb5ca4a;
                        hxc_l_gc_roots[203] = (const void *)hxc_l_tmp_load_result_n803;
                        if (hxc_l_tmp_load_result_n803 == NULL)
                        {
                          abort();
                        }
                        hxc_string hxc_l_tmp_class_field_load_result_n804 = (*hxc_l_tmp_load_result_n803).hxc_id;
                        struct hxc_caxecraft_content_CampaignTransition *hxc_l_tmp_instance_call_result_n806 = hxc_caxecraft_content_CampaignManifest_transitionForRequest(hxc_l_tmp_load_result_n802, hxc_l_tmp_class_field_load_result_n804, hxc_l_exit);
                        hxc_l_gc_roots[204] = (const void *)hxc_l_tmp_instance_call_result_n806;
                        struct hxc_caxecraft_content_CampaignTransition *hxc_l_transition_haacfef5c13d5 = hxc_l_tmp_instance_call_result_n806;
                        hxc_l_gc_roots[205] = (const void *)hxc_l_transition_haacfef5c13d5;
                        hxc_l_gc_roots[206] = (const void *)NULL;
                        if (hxc_l_transition_haacfef5c13d5 == NULL)
                        {
                          hxc_string hxc_l_tmp_string_concat_result_n809 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
                          if (hxc_string_concat_ref((hxc_string){ (const uint8_t *)"caxecraft: ignored unknown campaign exit ", 41, true, NULL }, hxc_l_exit, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n809) != HXC_STATUS_OK)
                          {
                            abort();
                          }
                          hxc_l_tmp_string_concat_left_owner_n306 = hxc_l_tmp_string_concat_result_n809;
                          hxc_string hxc_l_tmp_string_concat_result_n811 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
                          if (hxc_string_concat_ref(hxc_l_tmp_string_concat_left_owner_n306, (hxc_string){ (const uint8_t *)" from ", 6, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n811) != HXC_STATUS_OK)
                          {
                            abort();
                          }
                          hxc_l_tmp_string_concat_left_owner_n307 = hxc_l_tmp_string_concat_result_n811;
                          hxc_string hxc_l_tmp_string_concat_left_borrow_result_n812 = hxc_l_tmp_string_concat_left_owner_n307;
                          struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_load_result_n813 = hxc_l_sourceLevel_h85921cb5ca4a;
                          hxc_l_gc_roots[207] = (const void *)hxc_l_tmp_load_result_n813;
                          if (hxc_l_tmp_load_result_n813 == NULL)
                          {
                            abort();
                          }
                          hxc_string hxc_l_tmp_class_field_load_result_n814 = (*hxc_l_tmp_load_result_n813).hxc_id;
                          hxc_string hxc_l_tmp_string_concat_result_n815 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
                          if (hxc_string_concat_ref(hxc_l_tmp_string_concat_left_borrow_result_n812, hxc_l_tmp_class_field_load_result_n814, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n815) != HXC_STATUS_OK)
                          {
                            abort();
                          }
                          hxc_l_tmp_sys_println_string_argument_owner_n308 = hxc_l_tmp_string_concat_result_n815;
                          if (hxc_io_println(hxc_l_tmp_sys_println_string_argument_owner_n308) != HXC_STATUS_OK)
                          {
                            if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n308) != HXC_STATUS_OK)
                            {
                              abort();
                            }
                            if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n307) != HXC_STATUS_OK)
                            {
                              abort();
                            }
                            if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n306) != HXC_STATUS_OK)
                            {
                              abort();
                            }
                            if (hxc_string_release(&hxc_l_exit) != HXC_STATUS_OK)
                            {
                              abort();
                            }
                            hxc_enum_147eda7c_destroy(&hxc_l_event_h6e7c030f6037);
                            hxc_array_ce94fc36_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n293);
                            if (hxc_array_ref_release(hxc_l_g1_h86761ddd48e2) != HXC_STATUS_OK)
                            {
                              abort();
                            }
                            hxc_optional_b8d3265c_destroy(&hxc_l_flow);
                            hxc_record_9ab6a4ce_destroy(&hxc_l_gameTick);
                            if (hxc_array_ref_release(hxc_l_initialActorPhases) != HXC_STATUS_OK)
                            {
                              abort();
                            }
                            hxc_enum_2c0e695d_destroy(&hxc_l_tmp_static_call_argument_0_owner_n69);
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
                            hxc_enum_4b68fff2_destroy(&hxc_l_g_h1d08383de4f8);
                            if (hxc_string_release(&hxc_l_pendingCampaignLabel) != HXC_STATUS_OK)
                            {
                              abort();
                            }
                            hxc_enum_fcb4401b_destroy(&hxc_l_g_h4ef171f4fe67);
                            if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
                            {
                              abort();
                            }
                            abort();
                          }
                          if (hxc_string_release(&hxc_l_tmp_sys_println_string_argument_owner_n308) != HXC_STATUS_OK)
                          {
                            abort();
                          }
                          if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n307) != HXC_STATUS_OK)
                          {
                            abort();
                          }
                          if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n306) != HXC_STATUS_OK)
                          {
                            abort();
                          }
                        }
                        else
                        {
                          struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n817 = hxc_l_selectedCampaign_hb23a0d107c02;
                          hxc_l_gc_roots[208] = (const void *)hxc_l_tmp_load_result_n817;
                          if (hxc_l_tmp_load_result_n817 == NULL)
                          {
                            abort();
                          }
                          struct hxc_caxecraft_content_CampaignTransition *hxc_l_tmp_load_result_n818 = hxc_l_transition_haacfef5c13d5;
                          hxc_l_gc_roots[209] = (const void *)hxc_l_tmp_load_result_n818;
                          if (hxc_l_tmp_load_result_n818 == NULL)
                          {
                            abort();
                          }
                          hxc_string hxc_l_tmp_class_field_load_result_n819 = (*hxc_l_tmp_load_result_n818).hxc_destinationLevel;
                          struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_instance_call_result_n820 = hxc_caxecraft_content_CampaignManifest_level(hxc_l_tmp_load_result_n817, hxc_l_tmp_class_field_load_result_n819);
                          hxc_l_gc_roots[210] = (const void *)hxc_l_tmp_instance_call_result_n820;
                          struct hxc_caxecraft_content_CampaignLevel *hxc_l_nextLevel = hxc_l_tmp_instance_call_result_n820;
                          hxc_l_gc_roots[211] = (const void *)hxc_l_nextLevel;
                          hxc_l_gc_roots[212] = (const void *)NULL;
                          if (hxc_l_nextLevel == NULL)
                          {
                            if (hxc_io_println((hxc_string){ (const uint8_t *)"caxecraft: campaign destination disappeared after manifest validation", 69, true, NULL }) != HXC_STATUS_OK)
                            {
                              abort();
                            }
                          }
                          else
                          {
                            hxc_l_gc_roots[213] = (const void *)hxc_l_nextLevel;
                            hxc_l_pendingCampaignLevel = hxc_l_nextLevel;
                            struct hxc_caxecraft_content_CampaignLevel *hxc_l_tmp_load_result_n823 = hxc_l_nextLevel;
                            hxc_l_gc_roots[214] = (const void *)hxc_l_tmp_load_result_n823;
                            if (hxc_l_tmp_load_result_n823 == NULL)
                            {
                              abort();
                            }
                            hxc_string hxc_l_tmp_class_field_load_result_n824 = (*hxc_l_tmp_load_result_n823).hxc_id;
                            hxc_string hxc_l_tmp_string_assignment_replacement_owner_n310 = hxc_l_tmp_class_field_load_result_n824;
                            if (hxc_string_retain(hxc_l_tmp_string_assignment_replacement_owner_n310) != HXC_STATUS_OK)
                            {
                              abort();
                            }
                            hxc_string hxc_l_tmp_string_assignment_replacement_owned_load_result_n825 = hxc_l_tmp_string_assignment_replacement_owner_n310;
                            if (hxc_string_release(&hxc_l_pendingCampaignLabel) != HXC_STATUS_OK)
                            {
                              abort();
                            }
                            hxc_l_pendingCampaignLabel = hxc_l_tmp_string_assignment_replacement_owned_load_result_n825;
                            hxc_l_loadingFramePresented = false;
                            enum hxc_caxecraft_app_AppScreen hxc_l_tmp_call_result_n827 = hxc_caxecraft_app_AppScreen_beginLoading(hxc_l_screen);
                            hxc_l_screen = hxc_l_tmp_call_result_n827;
                            hxc_l_accumulator = 0.0;
                            hxc_l_jumpQueued = false;
                            EnableCursor();
                          }
                        }
                      }
                    }
                    if (hxc_string_release(&hxc_l_exit) != HXC_STATUS_OK)
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
              hxc_enum_147eda7c_destroy(&hxc_l_event_h6e7c030f6037);
              hxc_array_ce94fc36_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n293);
            }
            struct hxc_optional_caxecraft_scenario_FlowTickResult hxc_l_tmp_load_result_n828 = hxc_l_flow;
            if (!hxc_l_tmp_load_result_n828.hxc_has_value)
            {
              abort();
            }
            hxc_string hxc_l_tmp_string_assignment_replacement_owner_n311 = hxc_l_tmp_load_result_n828.hxc_value.hxc_activeObjective;
            if (hxc_string_retain(hxc_l_tmp_string_assignment_replacement_owner_n311) != HXC_STATUS_OK)
            {
              abort();
            }
            hxc_string hxc_l_tmp_string_assignment_replacement_owned_load_result_n831 = hxc_l_tmp_string_assignment_replacement_owner_n311;
            if (hxc_string_release(&hxc_l_currentObjectiveId) != HXC_STATUS_OK)
            {
              abort();
            }
            hxc_l_currentObjectiveId = hxc_l_tmp_string_assignment_replacement_owned_load_result_n831;
            if (hxc_array_ref_release(hxc_l_g1_h86761ddd48e2) != HXC_STATUS_OK)
            {
              abort();
            }
          }
        }
        if (hxc_l_gameTick.hxc_committed)
        {
          struct hxc_caxecraft_app_MotionHistory hxc_l_tmp_load_result_n833 = hxc_l_motionHistory;
          struct hxc_caxecraft_app_MotionHistory hxc_l_tmp_call_result_n835 = hxc_caxecraft_app_MotionInterpolation_advance(hxc_l_tmp_load_result_n833, hxc_l_character.hxc_body);
          hxc_l_motionHistory = hxc_l_tmp_call_result_n835;
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
            int32_t hxc_l_tmp_load_result_n839 = hxc_l_pickupIndex;
            struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n840 = hxc_l_levelView_hc35c2f93673f;
            hxc_l_gc_roots[215] = (const void *)hxc_l_tmp_load_result_n840;
            if (hxc_l_tmp_load_result_n840 == NULL)
            {
              abort();
            }
            struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n841 = (*hxc_l_tmp_load_result_n840).hxc_items;
            int32_t hxc_l_tmp_array_length_result_n842;
            if (hxc_array_ref_length(hxc_l_tmp_class_field_load_result_n841, &hxc_l_tmp_array_length_result_n842) != HXC_STATUS_OK)
            {
              abort();
            }
            if (!(hxc_l_tmp_load_result_n839 < hxc_l_tmp_array_length_result_n842))
            {
              break;
            }
            struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n843 = hxc_l_levelView_hc35c2f93673f;
            hxc_l_gc_roots[216] = (const void *)hxc_l_tmp_load_result_n843;
            if (hxc_l_tmp_load_result_n843 == NULL)
            {
              abort();
            }
            struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n844 = (*hxc_l_tmp_load_result_n843).hxc_items;
            struct hxc_caxecraft_app_LoadedWorldItem hxc_l_tmp_array_get_result_n846;
            if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n844, (size_t)hxc_l_pickupIndex, &hxc_l_tmp_array_get_result_n846) != HXC_STATUS_OK)
            {
              abort();
            }
            struct hxc_caxecraft_app_LoadedWorldItem hxc_l_loadedItem = hxc_l_tmp_array_get_result_n846;
            struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n847 = hxc_l_session;
            hxc_l_gc_roots[217] = (const void *)hxc_l_tmp_load_result_n847;
            if (hxc_l_tmp_load_result_n847 == NULL)
            {
              abort();
            }
            bool hxc_l_tmp_instance_call_result_n849 = hxc_caxecraft_domain_GameSession_authoredItemIsActive(hxc_l_tmp_load_result_n847, hxc_l_pickupIndex);
            bool hxc_l_tmp_short_circuit_result_n314 = hxc_l_tmp_instance_call_result_n849;
            if (hxc_l_tmp_instance_call_result_n849)
            {
              double hxc_l_tmp_record_field_load_result_n850 = hxc_l_character.hxc_body.hxc_x;
              double hxc_l_tmp_record_field_load_result_n851 = hxc_l_character.hxc_body.hxc_y;
              double hxc_l_tmp_record_field_load_result_n852 = hxc_l_character.hxc_body.hxc_z;
              int32_t hxc_l_tmp_record_field_load_result_n853 = hxc_l_loadedItem.hxc_xMilli;
              int32_t hxc_l_tmp_record_field_load_result_n854 = hxc_l_loadedItem.hxc_yMilli;
              bool hxc_l_tmp_call_result_n856 = hxc_caxecraft_gameplay_WorldItemPickup_isInRange(hxc_l_tmp_record_field_load_result_n850, hxc_l_tmp_record_field_load_result_n851, hxc_l_tmp_record_field_load_result_n852, hxc_l_tmp_record_field_load_result_n853, hxc_l_tmp_record_field_load_result_n854, hxc_l_loadedItem.hxc_zMilli);
              hxc_l_tmp_short_circuit_result_n314 = hxc_l_tmp_call_result_n856;
            }
            if (hxc_l_tmp_short_circuit_result_n314)
            {
              int32_t hxc_l_itemCode = hxc_l_loadedItem.hxc_storageCode;
              struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_tmp_load_result_n859 = hxc_l_contentRegistry;
              hxc_l_gc_roots[218] = (const void *)hxc_l_tmp_load_result_n859;
              if (hxc_l_tmp_load_result_n859 == NULL)
              {
                abort();
              }
              enum hxc_caxecraft_content_RuntimeItemUseProfile hxc_l_tmp_instance_call_result_n861 = hxc_caxecraft_content_RuntimeContentRegistry_itemUseProfile(hxc_l_tmp_load_result_n859, hxc_l_itemCode);
              bool hxc_l_tmp_short_circuit_result_n316 = hxc_l_tmp_instance_call_result_n861 == hxc_caxecraft_content_RuntimeItemUseProfile_EquipAquatic;
              if (hxc_l_tmp_instance_call_result_n861 == hxc_caxecraft_content_RuntimeItemUseProfile_EquipAquatic)
              {
                struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_tmp_load_result_n863 = hxc_l_contentRegistry;
                hxc_l_gc_roots[219] = (const void *)hxc_l_tmp_load_result_n863;
                if (hxc_l_tmp_load_result_n863 == NULL)
                {
                  abort();
                }
                bool hxc_l_tmp_instance_call_result_n865 = hxc_caxecraft_content_RuntimeContentRegistry_itemProvidesAquaticProfile(hxc_l_tmp_load_result_n863, hxc_l_itemCode);
                hxc_l_tmp_short_circuit_result_n316 = hxc_l_tmp_instance_call_result_n865;
              }
              if (hxc_l_tmp_short_circuit_result_n316)
              {
                struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_tmp_load_result_n867 = hxc_l_contentRegistry;
                hxc_l_gc_roots[220] = (const void *)hxc_l_tmp_load_result_n867;
                if (hxc_l_tmp_load_result_n867 == NULL)
                {
                  abort();
                }
                struct hxc_caxecraft_domain_AquaticProfile hxc_l_tmp_instance_call_result_n869 = hxc_caxecraft_content_RuntimeContentRegistry_itemAquaticProfile(hxc_l_tmp_load_result_n867, hxc_l_itemCode);
                struct hxc_caxecraft_domain_AquaticProfile hxc_l_replacement = hxc_l_tmp_instance_call_result_n869;
                struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n870 = hxc_l_session;
                hxc_l_gc_roots[221] = (const void *)hxc_l_tmp_load_result_n870;
                if (hxc_l_tmp_load_result_n870 == NULL)
                {
                  abort();
                }
                int32_t hxc_l_tmp_load_result_n871 = hxc_l_pickupIndex;
                struct hxc_caxecraft_domain_AuthoredAquaticEquipmentResult hxc_l_tmp_instance_call_result_n873 = hxc_caxecraft_domain_GameSession_collectAuthoredAquaticEquipment(hxc_l_tmp_load_result_n870, hxc_l_tmp_load_result_n871, hxc_l_replacement);
                struct hxc_caxecraft_domain_AuthoredAquaticEquipmentResult hxc_l_equipment = hxc_l_tmp_instance_call_result_n873;
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
                struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_tmp_load_result_n878 = hxc_l_contentRegistry;
                hxc_l_gc_roots[222] = (const void *)hxc_l_tmp_load_result_n878;
                struct hxc_optional_caxecraft_gameplay_ItemKind hxc_l_tmp_call_result_n880 = hxc_caxecraft_app_RuntimeInventoryBinding_inventoryKindForRuntimeItem(hxc_l_tmp_load_result_n878, hxc_l_itemCode);
                struct hxc_optional_caxecraft_gameplay_ItemKind hxc_l_inventoryKind = hxc_l_tmp_call_result_n880;
                if (hxc_l_inventoryKind.hxc_has_value)
                {
                  struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n882 = hxc_l_session;
                  hxc_l_gc_roots[223] = (const void *)hxc_l_tmp_load_result_n882;
                  if (hxc_l_tmp_load_result_n882 == NULL)
                  {
                    abort();
                  }
                  int32_t hxc_l_tmp_load_result_n883 = hxc_l_pickupIndex;
                  struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_load_result_n884 = hxc_l_inventory;
                  struct hxc_optional_caxecraft_gameplay_ItemKind hxc_l_tmp_load_result_n885 = hxc_l_inventoryKind;
                  if (!hxc_l_tmp_load_result_n885.hxc_has_value)
                  {
                    abort();
                  }
                  struct hxc_caxecraft_domain_AuthoredInventoryItemResult hxc_l_tmp_instance_call_result_n888 = hxc_caxecraft_domain_GameSession_collectAuthoredInventoryItem(hxc_l_tmp_load_result_n882, hxc_l_tmp_load_result_n883, hxc_l_tmp_load_result_n884, hxc_l_tmp_load_result_n885.hxc_value, hxc_l_loadedItem.hxc_quantity);
                  struct hxc_caxecraft_domain_AuthoredInventoryItemResult hxc_l_pickup = hxc_l_tmp_instance_call_result_n888;
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
            struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n894 = hxc_l_session;
            hxc_l_gc_roots[224] = (const void *)hxc_l_tmp_load_result_n894;
            if (hxc_l_tmp_load_result_n894 == NULL)
            {
              abort();
            }
            int32_t hxc_l_tmp_record_field_load_result_n895 = hxc_l_gameTick.hxc_tickIndex;
            struct hxc_caxecraft_domain_ActorControllerTickResult hxc_l_tmp_instance_call_result_n897 = hxc_caxecraft_domain_GameSession_stepAuthoredActorControllers(hxc_l_tmp_load_result_n894, hxc_l_tmp_record_field_load_result_n895, hxc_l_damagePolicy);
            struct hxc_caxecraft_domain_ActorControllerTickResult hxc_l_actorTick = hxc_l_tmp_instance_call_result_n897;
            switch (hxc_l_actorTick.hxc_status.hxc_tag) {
              case hxc_caxecraft_domain_ActorControllerTickStatus_ControllersAdvanced:
                {
                  hxc_l_availableInteractionPrompt = hxc_caxecraft_app_InteractionPrompt_NoInteractionPrompt;
                  int32_t hxc_l_g_h2bb3a9b5c681 = 0;
                  struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n900 = hxc_l_session;
                  hxc_l_gc_roots[229] = (const void *)hxc_l_tmp_load_result_n900;
                  if (hxc_l_tmp_load_result_n900 == NULL)
                  {
                    abort();
                  }
                  struct hxc_array_ref *hxc_l_tmp_instance_call_result_n901 = hxc_caxecraft_domain_GameSession_actorControllerEventSnapshots(hxc_l_tmp_load_result_n900);
                  hxc_l_g1_h9d1cad2799a4 = hxc_l_tmp_instance_call_result_n901;
                  while (1)
                  {
                    int32_t hxc_l_tmp_load_result_n902 = hxc_l_g_h2bb3a9b5c681;
                    int32_t hxc_l_tmp_array_length_result_n904;
                    if (hxc_array_ref_length(hxc_l_g1_h9d1cad2799a4, &hxc_l_tmp_array_length_result_n904) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    if (!(hxc_l_tmp_load_result_n902 < hxc_l_tmp_array_length_result_n904))
                    {
                      break;
                    }
                    struct hxc_array_ref *hxc_l_tmp_load_result_n905 = hxc_l_g1_h9d1cad2799a4;
                    struct hxc_caxecraft_domain_ActorControllerEvent hxc_l_tmp_array_get_result_n907;
                    if (hxc_array_ref_get_copy(hxc_l_tmp_load_result_n905, (size_t)hxc_l_g_h2bb3a9b5c681, &hxc_l_tmp_array_get_result_n907) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    hxc_l_tmp_array_element_owner_n325 = hxc_l_tmp_array_get_result_n907;
                    hxc_l_event_h3bd8b64d343d = hxc_l_tmp_array_element_owner_n325;
                    if (hxc_enum_8b2a26b8_retain(&hxc_l_event_h3bd8b64d343d) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    hxc_l_g_h2bb3a9b5c681 = hxc_i32_add_wrapping(hxc_l_g_h2bb3a9b5c681, 1);
                    switch (hxc_l_event_h3bd8b64d343d.hxc_tag) {
                      case hxc_caxecraft_domain_ActorControllerEvent_NoControllerEvent:
                        {
                          break;
                        }
                      case hxc_caxecraft_domain_ActorControllerEvent_InteractionAvailable:
                        {
                          if (hxc_l_event_h3bd8b64d343d.hxc_tag != hxc_caxecraft_domain_ActorControllerEvent_InteractionAvailable)
                          {
                            abort();
                          }
                          int32_t hxc_l_tmp_enum_payload_project_n912 = hxc_l_event_h3bd8b64d343d.hxc_payload.hxc_InteractionAvailable.hxc_characterId;
                          int32_t hxc_l_source_hd1e03848252c = hxc_l_tmp_enum_payload_project_n912;
                          struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n913 = hxc_l_levelView_hc35c2f93673f;
                          hxc_l_gc_roots[230] = (const void *)hxc_l_tmp_load_result_n913;
                          if (hxc_l_tmp_load_result_n913 == NULL)
                          {
                            abort();
                          }
                          bool hxc_l_tmp_instance_call_result_n915 = hxc_caxecraft_app_PlayableLevelView_hasDialogueActor(hxc_l_tmp_load_result_n913, hxc_l_source_hd1e03848252c);
                          if (hxc_l_tmp_instance_call_result_n915)
                          {
                            hxc_l_availableInteractionPrompt = hxc_caxecraft_app_InteractionPrompt_TalkInteractionPrompt;
                          }
                          break;
                        }
                      case hxc_caxecraft_domain_ActorControllerEvent_LocalPlayerAttack:
                        {
                          if (hxc_l_event_h3bd8b64d343d.hxc_tag != hxc_caxecraft_domain_ActorControllerEvent_LocalPlayerAttack)
                          {
                            abort();
                          }
                          int32_t hxc_l_tmp_enum_payload_project_n918 = hxc_l_event_h3bd8b64d343d.hxc_payload.hxc_LocalPlayerAttack.hxc_characterId;
                          int32_t hxc_l_source_h10505d3c32cb = hxc_l_tmp_enum_payload_project_n918;
                          int32_t hxc_l_tmp_load_result_n919 = hxc_l_source_h10505d3c32cb;
                          if (hxc_l_tmp_load_result_n919 == hxc_l_enemyActorId)
                          {
                            hxc_l_enemyAttackFrames = 120;
                          }
                          break;
                        }
                      case hxc_caxecraft_domain_ActorControllerEvent_DropRequested:
                        {
                          if (hxc_l_event_h3bd8b64d343d.hxc_tag != hxc_caxecraft_domain_ActorControllerEvent_DropRequested)
                          {
                            abort();
                          }
                          int32_t hxc_l_tmp_enum_payload_project_n922 = hxc_l_event_h3bd8b64d343d.hxc_payload.hxc_DropRequested.hxc_characterId;
                          int32_t hxc_l_source_hbfe457c0a22c = hxc_l_tmp_enum_payload_project_n922;
                          if (hxc_l_event_h3bd8b64d343d.hxc_tag != hxc_caxecraft_domain_ActorControllerEvent_DropRequested)
                          {
                            abort();
                          }
                          hxc_string hxc_l_tmp_enum_payload_project_n924 = hxc_l_event_h3bd8b64d343d.hxc_payload.hxc_DropRequested.hxc_drop;
                          hxc_l_drop = hxc_l_tmp_enum_payload_project_n924;
                          if (hxc_string_retain(hxc_l_drop) != HXC_STATUS_OK)
                          {
                            abort();
                          }
                          struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_tmp_load_result_n925 = hxc_l_contentRegistry;
                          hxc_l_gc_roots[231] = (const void *)hxc_l_tmp_load_result_n925;
                          if (hxc_l_tmp_load_result_n925 == NULL)
                          {
                            abort();
                          }
                          int32_t hxc_l_tmp_instance_call_result_n927 = hxc_caxecraft_content_RuntimeContentRegistry_dropQuantityById(hxc_l_tmp_load_result_n925, hxc_l_drop);
                          int32_t hxc_l_quantity = hxc_l_tmp_instance_call_result_n927;
                          struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n928 = hxc_l_session;
                          hxc_l_gc_roots[232] = (const void *)hxc_l_tmp_load_result_n928;
                          if (hxc_l_tmp_load_result_n928 == NULL)
                          {
                            abort();
                          }
                          struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n930 = hxc_caxecraft_domain_GameSession_readCharacter(hxc_l_tmp_load_result_n928, hxc_l_source_hbfe457c0a22c);
                          struct hxc_caxecraft_domain_Character hxc_l_defeatedActor = hxc_l_tmp_instance_call_result_n930;
                          int32_t hxc_l_tmp_load_result_n931 = hxc_l_quantity;
                          bool hxc_l_tmp_short_circuit_result_n332 = hxc_l_tmp_load_result_n931 <= 0;
                          if (!(hxc_l_tmp_load_result_n931 <= 0))
                          {
                            hxc_l_tmp_short_circuit_result_n332 = hxc_l_defeatedActor.hxc_id <= 0;
                          }
                          if (hxc_l_tmp_short_circuit_result_n332)
                          {
                            hxc_l_quit = true;
                          }
                          else
                          {
                            if (!hxc_l_berryDrop.hxc_active)
                            {
                              struct hxc_caxecraft_domain_Character hxc_l_tmp_load_result_n935 = hxc_l_defeatedActor;
                              struct hxc_caxecraft_gameplay_BerryDropState hxc_l_tmp_call_result_n937 = hxc_caxecraft_gameplay_BerryDrop_fromDefeatedCharacter(hxc_l_tmp_load_result_n935, hxc_l_quantity);
                              hxc_l_berryDrop = hxc_l_tmp_call_result_n937;
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
                    hxc_enum_8b2a26b8_destroy(&hxc_l_event_h3bd8b64d343d);
                    hxc_array_43e9f8ca_element_destroy(NULL, &hxc_l_tmp_array_element_owner_n325);
                  }
                  if (hxc_array_ref_release(hxc_l_g1_h9d1cad2799a4) != HXC_STATUS_OK)
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
          struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n938 = hxc_l_session;
          hxc_l_gc_roots[225] = (const void *)hxc_l_tmp_load_result_n938;
          if (hxc_l_tmp_load_result_n938 == NULL)
          {
            abort();
          }
          struct hxc_array_ref *hxc_l_tmp_instance_call_result_n939 = hxc_caxecraft_domain_GameSession_actorControllerStateSnapshots(hxc_l_tmp_load_result_n938);
          hxc_l_actorPhases = hxc_l_tmp_instance_call_result_n939;
          struct hxc_array_ref *hxc_l_tmp_load_result_n940 = hxc_l_actorPhases;
          struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation hxc_l_tmp_call_result_n943 = hxc_caxecraft_app_CaxecraftApp_observeActorPhase(hxc_l_tmp_load_result_n940, hxc_l_enemyActorId, hxc_caxecraft_domain_ActorControllerPhase_Defeated);
          hxc_l_enemyPhase_h695cd3d136cc = hxc_l_tmp_call_result_n943;
          struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n944 = hxc_l_session;
          hxc_l_gc_roots[226] = (const void *)hxc_l_tmp_load_result_n944;
          if (hxc_l_tmp_load_result_n944 == NULL)
          {
            abort();
          }
          struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n946 = hxc_caxecraft_domain_GameSession_readCharacter(hxc_l_tmp_load_result_n944, hxc_l_enemyActorId);
          hxc_l_enemyActor = hxc_l_tmp_instance_call_result_n946;
          struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n947 = hxc_l_session;
          hxc_l_gc_roots[227] = (const void *)hxc_l_tmp_load_result_n947;
          struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n948 = hxc_l_levelView_hc35c2f93673f;
          hxc_l_gc_roots[228] = (const void *)hxc_l_tmp_load_result_n948;
          bool hxc_l_tmp_call_result_n950 = hxc_caxecraft_app_CaxecraftApp_dialogueActorsAreValid(hxc_l_tmp_load_result_n947, hxc_l_tmp_load_result_n948, hxc_l_actorPhases);
          bool hxc_l_tmp_short_circuit_result_n334 = !hxc_l_tmp_call_result_n950;
          if (!!hxc_l_tmp_call_result_n950)
          {
            hxc_l_tmp_short_circuit_result_n334 = !hxc_l_enemyPhase_h695cd3d136cc.hxc_valid;
          }
          bool hxc_l_tmp_short_circuit_load_result_n952 = hxc_l_tmp_short_circuit_result_n334;
          bool hxc_l_tmp_short_circuit_result_n335 = hxc_l_tmp_short_circuit_load_result_n952;
          if (!hxc_l_tmp_short_circuit_load_result_n952)
          {
            hxc_l_tmp_short_circuit_result_n335 = hxc_l_enemyActor.hxc_id <= 0;
          }
          if (hxc_l_tmp_short_circuit_result_n335)
          {
            hxc_l_quit = true;
          }
          struct hxc_caxecraft_gameplay_SwordCombatState hxc_l_tmp_call_result_n956 = hxc_caxecraft_gameplay_SwordCombat_step(hxc_l_swordCombat);
          hxc_l_swordCombat = hxc_l_tmp_call_result_n956;
          if (hxc_l_swordQueued)
          {
            struct hxc_caxecraft_gameplay_SwordCombatState hxc_l_tmp_load_result_n958 = hxc_l_swordCombat;
            struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_load_result_n959 = hxc_l_inventory;
            struct hxc_caxecraft_domain_VitalsState hxc_l_tmp_record_field_load_result_n960 = hxc_l_character.hxc_vitals;
            struct hxc_caxecraft_domain_Character hxc_l_tmp_load_result_n961 = hxc_l_enemyActor;
            double hxc_l_tmp_record_field_load_result_n962 = hxc_l_character.hxc_body.hxc_x;
            double hxc_l_tmp_record_field_load_result_n963 = hxc_l_character.hxc_body.hxc_z;
            double hxc_l_tmp_load_result_n964 = hxc_l_lookX;
            int32_t hxc_l_tmp_call_result_n966 = hxc_caxecraft_gameplay_SwordCombat_decide(hxc_l_tmp_load_result_n958, hxc_l_tmp_load_result_n959, hxc_l_tmp_record_field_load_result_n960, hxc_l_tmp_load_result_n961, hxc_l_tmp_record_field_load_result_n962, hxc_l_tmp_record_field_load_result_n963, hxc_l_tmp_load_result_n964, hxc_l_lookZ);
            int32_t hxc_l_swordDecision = hxc_l_tmp_call_result_n966;
            if (hxc_l_swordDecision == 5)
            {
              struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n968 = hxc_l_session;
              hxc_l_gc_roots[233] = (const void *)hxc_l_tmp_load_result_n968;
              if (hxc_l_tmp_load_result_n968 == NULL)
              {
                abort();
              }
              struct hxc_caxecraft_domain_CharacterDamageResult hxc_l_tmp_instance_call_result_n970 = hxc_caxecraft_domain_GameSession_damageCharacter(hxc_l_tmp_load_result_n968, hxc_l_enemyActorId, 1);
              struct hxc_caxecraft_domain_CharacterDamageResult hxc_l_damage = hxc_l_tmp_instance_call_result_n970;
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
            int32_t hxc_l_tmp_load_result_n974 = hxc_l_swordDecision;
            struct hxc_caxecraft_gameplay_SwordCombatState hxc_l_tmp_call_result_n976 = hxc_caxecraft_gameplay_SwordCombat_after(hxc_l_tmp_load_result_n974, hxc_l_swordCombat);
            hxc_l_swordCombat = hxc_l_tmp_call_result_n976;
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
      struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n981 = hxc_l_session;
      hxc_l_gc_roots[194] = (const void *)hxc_l_tmp_load_result_n981;
      if (hxc_l_tmp_load_result_n981 == NULL)
      {
        abort();
      }
      size_t hxc_l_tmp_length_n983;
      const uint8_t *hxc_l_tmp_instance_call_result_n982 = hxc_caxecraft_domain_GameSession_worldView(hxc_l_tmp_load_result_n981, &hxc_l_tmp_length_n983);
      double hxc_l_tmp_load_result_n984 = hxc_l_selectionEyeX;
      double hxc_l_tmp_load_result_n985 = hxc_l_selectionEyeY;
      double hxc_l_tmp_load_result_n986 = hxc_l_selectionEyeZ;
      double hxc_l_tmp_load_result_n987 = hxc_l_lookX;
      double hxc_l_tmp_load_result_n988 = hxc_l_lookY;
      struct hxc_caxecraft_domain_RaycastHit hxc_l_tmp_call_result_n990 = hxc_caxecraft_domain_VoxelRaycast_trace(hxc_l_tmp_instance_call_result_n982, hxc_l_tmp_length_n983, hxc_l_tmp_load_result_n984, hxc_l_tmp_load_result_n985, hxc_l_tmp_load_result_n986, hxc_l_tmp_load_result_n987, hxc_l_tmp_load_result_n988, hxc_l_lookZ, 7.0);
      struct hxc_caxecraft_domain_RaycastHit hxc_l_hit = hxc_l_tmp_call_result_n990;
      bool hxc_l_tmp_call_result_n992 = hxc_caxecraft_app_AppScreen_capturesPointer(hxc_l_screen);
      bool hxc_l_tmp_short_circuit_result_n342 = hxc_l_tmp_call_result_n992;
      if (hxc_l_tmp_call_result_n992)
      {
        hxc_l_tmp_short_circuit_result_n342 = !hxc_l_recapturedThisFrame;
      }
      bool hxc_l_tmp_short_circuit_load_result_n994 = hxc_l_tmp_short_circuit_result_n342;
      bool hxc_l_tmp_short_circuit_result_n343 = hxc_l_tmp_short_circuit_load_result_n994;
      if (hxc_l_tmp_short_circuit_load_result_n994)
      {
        hxc_l_tmp_short_circuit_result_n343 = hxc_l_primaryPressed_hfeea7ddbdc7b;
      }
      if (hxc_l_tmp_short_circuit_result_n343)
      {
        if (hxc_l_character.hxc_vitals.hxc_health > 0)
        {
          if (hxc_l_selectedMode == 1)
          {
            bool hxc_l_tmp_call_result_n1001 = hxc_caxecraft_gameplay_Inventory_selectedIs(hxc_l_inventory, hxc_caxecraft_gameplay_ItemKind_CopperSword);
            bool hxc_l_tmp_short_circuit_result_n344 = !hxc_l_tmp_call_result_n1001;
            if (!hxc_l_tmp_call_result_n1001)
            {
              hxc_l_tmp_short_circuit_result_n344 = hxc_l_hit.hxc_hit;
            }
            bool hxc_l_tmp_short_circuit_load_result_n1003 = hxc_l_tmp_short_circuit_result_n344;
            bool hxc_l_tmp_short_circuit_result_n345 = hxc_l_tmp_short_circuit_load_result_n1003;
            if (hxc_l_tmp_short_circuit_load_result_n1003)
            {
              struct hxc_caxecraft_domain_AquaticState hxc_l_tmp_record_field_load_result_n1004 = hxc_l_character.hxc_aquatic;
              bool hxc_l_tmp_call_result_n1006 = hxc_caxecraft_domain_Aquatics_canMine(hxc_l_tmp_record_field_load_result_n1004, hxc_l_character.hxc_aquaticProfile);
              hxc_l_tmp_short_circuit_result_n345 = hxc_l_tmp_call_result_n1006;
            }
            if (hxc_l_tmp_short_circuit_result_n345)
            {
              int32_t hxc_l_tmp_record_field_load_result_n1008 = hxc_l_hit.hxc_cellX;
              int32_t hxc_l_tmp_record_field_load_result_n1009 = hxc_l_hit.hxc_cellY;
              struct hxc_caxecraft_scenario_VoxelPoint hxc_l_tmp_call_result_n1011 = hxc_caxecraft_domain_World_coord(hxc_l_tmp_record_field_load_result_n1008, hxc_l_tmp_record_field_load_result_n1009, hxc_l_hit.hxc_cellZ);
              struct hxc_caxecraft_scenario_VoxelPoint hxc_l_minedCoordinate = hxc_l_tmp_call_result_n1011;
              int32_t hxc_l_sandBefore = hxc_l_inventory.hxc_sand;
              (void)hxc_l_sandBefore;
              struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1013 = hxc_l_session;
              hxc_l_gc_roots[234] = (const void *)hxc_l_tmp_load_result_n1013;
              if (hxc_l_tmp_load_result_n1013 == NULL)
              {
                abort();
              }
              struct hxc_caxecraft_scenario_VoxelPoint hxc_l_tmp_load_result_n1014 = hxc_l_minedCoordinate;
              struct hxc_caxecraft_gameplay_MiningResult hxc_l_tmp_instance_call_result_n1016 = hxc_caxecraft_domain_GameSession_mineTerrain(hxc_l_tmp_load_result_n1013, hxc_l_tmp_load_result_n1014, hxc_l_inventory);
              struct hxc_caxecraft_gameplay_MiningResult hxc_l_mining = hxc_l_tmp_instance_call_result_n1016;
              hxc_l_inventory = hxc_l_mining.hxc_inventory;
              if (hxc_l_mining.hxc_outcome == 2)
              {
                if (hxc_l_self == NULL)
                {
                  abort();
                }
                struct hxc_caxecraft_app_TerrainRenderer *hxc_l_tmp_owned_class_field_address_n1019 = &(*hxc_l_self).hxc_terrainRenderer;
                struct hxc_caxecraft_app_TerrainChunkCache *hxc_l_tmp_owned_class_field_address_n1020 = &(*hxc_l_tmp_owned_class_field_address_n1019).hxc_cache;
                if (hxc_l_tmp_owned_class_field_address_n1020 == NULL)
                {
                  abort();
                }
                hxc_caxecraft_app_TerrainChunkCache_invalidate(hxc_l_tmp_owned_class_field_address_n1020, hxc_l_minedCoordinate);
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
              int32_t hxc_l_tmp_record_field_load_result_n1024 = hxc_l_hit.hxc_cellX;
              int32_t hxc_l_tmp_record_field_load_result_n1025 = hxc_l_hit.hxc_cellY;
              struct hxc_caxecraft_scenario_VoxelPoint hxc_l_tmp_call_result_n1027 = hxc_caxecraft_domain_World_coord(hxc_l_tmp_record_field_load_result_n1024, hxc_l_tmp_record_field_load_result_n1025, hxc_l_hit.hxc_cellZ);
              struct hxc_caxecraft_scenario_VoxelPoint hxc_l_removedCoordinate = hxc_l_tmp_call_result_n1027;
              struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1028 = hxc_l_session;
              hxc_l_gc_roots[235] = (const void *)hxc_l_tmp_load_result_n1028;
              if (hxc_l_tmp_load_result_n1028 == NULL)
              {
                abort();
              }
              bool hxc_l_tmp_instance_call_result_n1030 = hxc_caxecraft_domain_GameSession_removeTerrain(hxc_l_tmp_load_result_n1028, hxc_l_removedCoordinate);
              bool hxc_l_removed = hxc_l_tmp_instance_call_result_n1030;
              if (hxc_l_removed)
              {
                if (hxc_l_self == NULL)
                {
                  abort();
                }
                struct hxc_caxecraft_app_TerrainRenderer *hxc_l_tmp_owned_class_field_address_n1032 = &(*hxc_l_self).hxc_terrainRenderer;
                struct hxc_caxecraft_app_TerrainChunkCache *hxc_l_tmp_owned_class_field_address_n1033 = &(*hxc_l_tmp_owned_class_field_address_n1032).hxc_cache;
                if (hxc_l_tmp_owned_class_field_address_n1033 == NULL)
                {
                  abort();
                }
                hxc_caxecraft_app_TerrainChunkCache_invalidate(hxc_l_tmp_owned_class_field_address_n1033, hxc_l_removedCoordinate);
              }
            }
          }
        }
      }
      bool hxc_l_tmp_call_result_n1036 = hxc_caxecraft_app_AppScreen_capturesPointer(hxc_l_screen);
      bool hxc_l_tmp_short_circuit_result_n351 = hxc_l_tmp_call_result_n1036;
      if (hxc_l_tmp_call_result_n1036)
      {
        hxc_l_tmp_short_circuit_result_n351 = hxc_l_secondaryPressed_h98e6c168bbcb;
      }
      if (hxc_l_tmp_short_circuit_result_n351)
      {
        if (hxc_l_character.hxc_vitals.hxc_health > 0)
        {
          struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1040 = hxc_l_session;
          hxc_l_gc_roots[236] = (const void *)hxc_l_tmp_load_result_n1040;
          if (hxc_l_tmp_load_result_n1040 == NULL)
          {
            abort();
          }
          struct hxc_caxecraft_domain_LocalRecoveryResult hxc_l_tmp_instance_call_result_n1042 = hxc_caxecraft_domain_GameSession_useSelectedRecovery(hxc_l_tmp_load_result_n1040, hxc_l_inventory);
          struct hxc_caxecraft_domain_LocalRecoveryResult hxc_l_recovery = hxc_l_tmp_instance_call_result_n1042;
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
                int32_t hxc_l_tmp_record_field_load_result_n1049 = hxc_l_hit.hxc_previousX;
                int32_t hxc_l_tmp_record_field_load_result_n1050 = hxc_l_hit.hxc_previousY;
                struct hxc_caxecraft_scenario_VoxelPoint hxc_l_tmp_call_result_n1052 = hxc_caxecraft_domain_World_coord(hxc_l_tmp_record_field_load_result_n1049, hxc_l_tmp_record_field_load_result_n1050, hxc_l_hit.hxc_previousZ);
                struct hxc_caxecraft_scenario_VoxelPoint hxc_l_placement = hxc_l_tmp_call_result_n1052;
                enum hxc_caxecraft_domain_BlockKind hxc_l_tmp_call_result_n1054 = hxc_caxecraft_gameplay_Inventory_selectedBlock(hxc_l_inventory);
                enum hxc_caxecraft_domain_BlockKind hxc_l_selectedBlock = hxc_l_tmp_call_result_n1054;
                struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_load_result_n1055 = hxc_l_inventory;
                int32_t hxc_l_tmp_call_result_n1057 = hxc_caxecraft_gameplay_Inventory_countAt(hxc_l_tmp_load_result_n1055, hxc_l_inventory.hxc_selected);
                bool hxc_l_hasItem = hxc_l_tmp_call_result_n1057 > 0;
                bool hxc_l_tmp_load_result_n1058 = hxc_l_hasItem;
                bool hxc_l_tmp_short_circuit_result_n356 = !hxc_l_tmp_load_result_n1058;
                if (!!hxc_l_tmp_load_result_n1058)
                {
                  bool hxc_l_tmp_call_result_n1060 = hxc_caxecraft_domain_World_isPlaceable(hxc_l_selectedBlock);
                  hxc_l_tmp_short_circuit_result_n356 = !hxc_l_tmp_call_result_n1060;
                }
                bool hxc_l_tmp_short_circuit_load_result_n1061 = hxc_l_tmp_short_circuit_result_n356;
                bool hxc_l_tmp_short_circuit_result_n357 = hxc_l_tmp_short_circuit_load_result_n1061;
                if (!hxc_l_tmp_short_circuit_load_result_n1061)
                {
                  struct hxc_caxecraft_domain_CharacterBody hxc_l_tmp_record_field_load_result_n1062 = hxc_l_character.hxc_body;
                  bool hxc_l_tmp_call_result_n1064 = hxc_caxecraft_domain_CharacterPhysics_canPlaceAt(hxc_l_tmp_record_field_load_result_n1062, hxc_l_placement);
                  hxc_l_tmp_short_circuit_result_n357 = !hxc_l_tmp_call_result_n1064;
                }
                bool hxc_l_tmp_short_circuit_load_result_n1065 = hxc_l_tmp_short_circuit_result_n357;
                bool hxc_l_tmp_short_circuit_result_n358 = hxc_l_tmp_short_circuit_load_result_n1065;
                if (!hxc_l_tmp_short_circuit_load_result_n1065)
                {
                  struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1066 = hxc_l_session;
                  hxc_l_gc_roots[237] = (const void *)hxc_l_tmp_load_result_n1066;
                  if (hxc_l_tmp_load_result_n1066 == NULL)
                  {
                    abort();
                  }
                  struct hxc_caxecraft_scenario_VoxelPoint hxc_l_tmp_load_result_n1067 = hxc_l_placement;
                  bool hxc_l_tmp_instance_call_result_n1069 = hxc_caxecraft_domain_GameSession_placeTerrain(hxc_l_tmp_load_result_n1066, hxc_l_tmp_load_result_n1067, hxc_l_selectedBlock);
                  hxc_l_tmp_short_circuit_result_n358 = !hxc_l_tmp_instance_call_result_n1069;
                }
                if (hxc_l_tmp_short_circuit_result_n358)
                {
                  hxc_l_placementBlockedFrames = 60;
                }
                else
                {
                  if (hxc_l_self == NULL)
                  {
                    abort();
                  }
                  struct hxc_caxecraft_app_TerrainRenderer *hxc_l_tmp_owned_class_field_address_n1071 = &(*hxc_l_self).hxc_terrainRenderer;
                  struct hxc_caxecraft_app_TerrainChunkCache *hxc_l_tmp_owned_class_field_address_n1072 = &(*hxc_l_tmp_owned_class_field_address_n1071).hxc_cache;
                  if (hxc_l_tmp_owned_class_field_address_n1072 == NULL)
                  {
                    abort();
                  }
                  hxc_caxecraft_app_TerrainChunkCache_invalidate(hxc_l_tmp_owned_class_field_address_n1072, hxc_l_placement);
                  if (hxc_l_selectedMode == 1)
                  {
                    struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n1076 = hxc_caxecraft_gameplay_Inventory_consumeSelected(hxc_l_inventory);
                    hxc_l_inventory = hxc_l_tmp_call_result_n1076;
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
      bool hxc_l_tmp_call_result_n1080 = hxc_caxecraft_app_AppScreen_pausesSimulation(hxc_l_screen);
      if (!hxc_l_tmp_call_result_n1080)
      {
        struct hxc_caxecraft_gameplay_BerryDropState hxc_l_tmp_load_result_n1081 = hxc_l_berryDrop;
        double hxc_l_tmp_record_field_load_result_n1082 = hxc_l_character.hxc_body.hxc_x;
        double hxc_l_tmp_record_field_load_result_n1083 = hxc_l_character.hxc_body.hxc_y;
        bool hxc_l_tmp_call_result_n1085 = hxc_caxecraft_gameplay_BerryDrop_isInRange(hxc_l_tmp_load_result_n1081, hxc_l_tmp_record_field_load_result_n1082, hxc_l_tmp_record_field_load_result_n1083, hxc_l_character.hxc_body.hxc_z);
        if (hxc_l_tmp_call_result_n1085)
        {
          struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_load_result_n1086 = hxc_l_inventory;
          int32_t hxc_l_tmp_call_result_n1089 = hxc_caxecraft_gameplay_Inventory_acceptedAmount(hxc_l_tmp_load_result_n1086, hxc_caxecraft_gameplay_ItemKind_Berries, hxc_l_berryDrop.hxc_amount);
          int32_t hxc_l_acceptedDrop = hxc_l_tmp_call_result_n1089;
          if (hxc_l_acceptedDrop > 0)
          {
            struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_load_result_n1091 = hxc_l_inventory;
            struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_call_result_n1094 = hxc_caxecraft_gameplay_Inventory_collectItem(hxc_l_tmp_load_result_n1091, hxc_caxecraft_gameplay_ItemKind_Berries, hxc_l_acceptedDrop);
            hxc_l_inventory = hxc_l_tmp_call_result_n1094;
            struct hxc_caxecraft_gameplay_BerryDropState hxc_l_tmp_load_result_n1095 = hxc_l_berryDrop;
            struct hxc_caxecraft_gameplay_BerryDropState hxc_l_tmp_call_result_n1097 = hxc_caxecraft_gameplay_BerryDrop_collectAmount(hxc_l_tmp_load_result_n1095, hxc_l_acceptedDrop);
            hxc_l_berryDrop = hxc_l_tmp_call_result_n1097;
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
      struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1114 = hxc_l_session;
      hxc_l_gc_roots[238] = (const void *)hxc_l_tmp_load_result_n1114;
      if (hxc_l_tmp_load_result_n1114 == NULL)
      {
        abort();
      }
      struct hxc_caxecraft_domain_GameView hxc_l_tmp_instance_call_result_n1115 = hxc_caxecraft_domain_GameSession_view(hxc_l_tmp_load_result_n1114);
      struct hxc_caxecraft_domain_GameView hxc_l_committedView = hxc_l_tmp_instance_call_result_n1115;
      if (!hxc_l_committedView.hxc_valid)
      {
        hxc_l_quit = true;
      }
      else
      {
        hxc_l_character = hxc_l_committedView.hxc_localPlayer;
      }
      struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1118 = hxc_l_session;
      hxc_l_gc_roots[239] = (const void *)hxc_l_tmp_load_result_n1118;
      hxc_l_gc_roots[240] = (const void *)hxc_l_levelView_hc35c2f93673f;
      struct hxc_caxecraft_app_CaxecraftApp_AvailableInteractionTarget hxc_l_tmp_call_result_n1120 = hxc_caxecraft_app_CaxecraftApp_nearestAvailableInteraction(hxc_l_tmp_load_result_n1118, hxc_l_levelView_hc35c2f93673f);
      hxc_l_tmp_static_call_argument_0_owner_n361 = hxc_l_tmp_call_result_n1120;
      enum hxc_caxecraft_app_InteractionPrompt hxc_l_tmp_call_result_n1122 = hxc_caxecraft_app_CaxecraftApp_promptForAvailableInteraction(hxc_l_tmp_static_call_argument_0_owner_n361);
      hxc_l_availableInteractionPrompt = hxc_l_tmp_call_result_n1122;
      struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1123 = hxc_l_session;
      hxc_l_gc_roots[241] = (const void *)hxc_l_tmp_load_result_n1123;
      if (hxc_l_tmp_load_result_n1123 == NULL)
      {
        abort();
      }
      struct hxc_array_ref *hxc_l_tmp_instance_call_result_n1124 = hxc_caxecraft_domain_GameSession_actorControllerStateSnapshots(hxc_l_tmp_load_result_n1123);
      hxc_l_presentationActorPhases = hxc_l_tmp_instance_call_result_n1124;
      struct hxc_array_ref *hxc_l_tmp_load_result_n1125 = hxc_l_presentationActorPhases;
      struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation hxc_l_tmp_call_result_n1128 = hxc_caxecraft_app_CaxecraftApp_observeActorPhase(hxc_l_tmp_load_result_n1125, hxc_l_enemyActorId, hxc_caxecraft_domain_ActorControllerPhase_Defeated);
      hxc_l_enemyPhase_h695cd3d136cc = hxc_l_tmp_call_result_n1128;
      struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1129 = hxc_l_session;
      hxc_l_gc_roots[242] = (const void *)hxc_l_tmp_load_result_n1129;
      if (hxc_l_tmp_load_result_n1129 == NULL)
      {
        abort();
      }
      struct hxc_caxecraft_domain_Character hxc_l_tmp_instance_call_result_n1131 = hxc_caxecraft_domain_GameSession_readCharacter(hxc_l_tmp_load_result_n1129, hxc_l_enemyActorId);
      hxc_l_enemyActor = hxc_l_tmp_instance_call_result_n1131;
      struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1132 = hxc_l_session;
      hxc_l_gc_roots[243] = (const void *)hxc_l_tmp_load_result_n1132;
      struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1133 = hxc_l_levelView_hc35c2f93673f;
      hxc_l_gc_roots[244] = (const void *)hxc_l_tmp_load_result_n1133;
      bool hxc_l_tmp_call_result_n1135 = hxc_caxecraft_app_CaxecraftApp_dialogueActorsAreValid(hxc_l_tmp_load_result_n1132, hxc_l_tmp_load_result_n1133, hxc_l_presentationActorPhases);
      bool hxc_l_tmp_short_circuit_result_n363 = !hxc_l_tmp_call_result_n1135;
      if (!!hxc_l_tmp_call_result_n1135)
      {
        hxc_l_tmp_short_circuit_result_n363 = !hxc_l_enemyPhase_h695cd3d136cc.hxc_valid;
      }
      bool hxc_l_tmp_short_circuit_load_result_n1137 = hxc_l_tmp_short_circuit_result_n363;
      bool hxc_l_tmp_short_circuit_result_n364 = hxc_l_tmp_short_circuit_load_result_n1137;
      if (!hxc_l_tmp_short_circuit_load_result_n1137)
      {
        hxc_l_tmp_short_circuit_result_n364 = hxc_l_enemyActor.hxc_id <= 0;
      }
      if (hxc_l_tmp_short_circuit_result_n364)
      {
        hxc_l_quit = true;
      }
      struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1140 = hxc_l_session;
      hxc_l_gc_roots[245] = (const void *)hxc_l_tmp_load_result_n1140;
      hxc_l_gc_roots[246] = (const void *)hxc_l_levelView_hc35c2f93673f;
      struct hxc_array_ref *hxc_l_tmp_call_result_n1142 = hxc_caxecraft_app_CaxecraftApp_dialogueActorSnapshots(hxc_l_tmp_load_result_n1140, hxc_l_levelView_hc35c2f93673f);
      hxc_l_dialogueActors = hxc_l_tmp_call_result_n1142;
      int32_t hxc_l_completedTicks = hxc_l_committedView.hxc_completedTicks;
      if (hxc_l_resetMotionThisFrame)
      {
        struct hxc_caxecraft_app_MotionHistory hxc_l_tmp_call_result_n1146 = hxc_caxecraft_app_MotionInterpolation_start(hxc_l_character.hxc_body);
        hxc_l_motionHistory = hxc_l_tmp_call_result_n1146;
      }
      struct hxc_caxecraft_app_MotionHistory hxc_l_tmp_load_result_n1147 = hxc_l_motionHistory;
      struct hxc_caxecraft_app_RenderPosition hxc_l_tmp_call_result_n1149 = hxc_caxecraft_app_MotionInterpolation_sample(hxc_l_tmp_load_result_n1147, hxc_l_accumulator, 0.05);
      struct hxc_caxecraft_app_RenderPosition hxc_l_renderPosition = hxc_l_tmp_call_result_n1149;
      double hxc_l_eyeX = hxc_l_renderPosition.hxc_x;
      double hxc_l_eyeY = hxc_l_renderPosition.hxc_y + 1.62;
      double hxc_l_eyeZ = hxc_l_renderPosition.hxc_z;
      double hxc_l_tmp_load_result_n1153 = hxc_l_eyeX;
      double hxc_l_tmp_load_result_n1154 = hxc_l_eyeY;
      double hxc_l_tmp_load_result_n1155 = hxc_l_eyeZ;
      double hxc_l_tmp_load_result_n1157 = hxc_l_eyeX;
      double hxc_l_tmp_load_result_n1158 = hxc_l_lookX;
      double hxc_l_tmp_load_result_n1159 = hxc_l_eyeY;
      double hxc_l_tmp_load_result_n1160 = hxc_l_lookY;
      double hxc_l_tmp_load_result_n1161 = hxc_l_eyeZ;
      struct Camera3D hxc_l_camera = (struct Camera3D){ .position = (struct Vector3){ .x = (float)hxc_l_tmp_load_result_n1153, .y = (float)hxc_l_tmp_load_result_n1154, .z = (float)hxc_l_tmp_load_result_n1155 }, .target = (struct Vector3){ .x = (float)(hxc_l_tmp_load_result_n1157 + hxc_l_tmp_load_result_n1158), .y = (float)(hxc_l_tmp_load_result_n1159 + hxc_l_tmp_load_result_n1160), .z = (float)(hxc_l_tmp_load_result_n1161 + hxc_l_lookZ) }, .up = (struct Vector3){ .x = (float)0.0, .y = (float)1.0, .z = (float)0.0 }, .fovy = (float)70.0, .projection = 0 };
      BeginDrawing();
      if (hxc_l_onTitle)
      {
        hxc_string hxc_l_locale1_hbfeeec43647f = { 0 };
        switch (hxc_l_locale) {
          case 0:
            {
              hxc_l_locale1_hbfeeec43647f = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
              if (hxc_string_retain(hxc_l_locale1_hbfeeec43647f) != HXC_STATUS_OK)
              {
                abort();
              }
              break;
            }
          case 1:
            {
              hxc_l_locale1_hbfeeec43647f = (hxc_string){ (const uint8_t *)"es-mx", 5, true, NULL };
              if (hxc_string_retain(hxc_l_locale1_hbfeeec43647f) != HXC_STATUS_OK)
              {
                abort();
              }
              break;
            }
          default:
            {
              hxc_l_locale1_hbfeeec43647f = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
              if (hxc_string_retain(hxc_l_locale1_hbfeeec43647f) != HXC_STATUS_OK)
              {
                abort();
              }
              break;
            }
        }
        hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n1168 = hxc_l_locale1_hbfeeec43647f;
        hxc_l_tmp_managed_flow_owner_n373 = hxc_l_tmp_managed_flow_carrier_move_result_n1168;
        struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1169 = hxc_l_levelView_hc35c2f93673f;
        hxc_l_gc_roots[247] = (const void *)hxc_l_tmp_load_result_n1169;
        if (hxc_l_tmp_load_result_n1169 == NULL)
        {
          abort();
        }
        struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_class_field_load_result_n1170 = (*hxc_l_tmp_load_result_n1169).hxc_levelPresentation;
        hxc_l_gc_roots[248] = (const void *)hxc_l_tmp_class_field_load_result_n1170;
        if (hxc_l_tmp_class_field_load_result_n1170 == NULL)
        {
          abort();
        }
        hxc_string hxc_l_tmp_instance_call_result_n1172 = hxc_caxecraft_content_RuntimeLevelPresentation_message(hxc_l_tmp_class_field_load_result_n1170, (hxc_string){ (const uint8_t *)"adventure_tagline", 17, true, NULL }, hxc_l_tmp_managed_flow_owner_n373);
        hxc_l_adventureTagline = hxc_l_tmp_instance_call_result_n1172;
        int32_t hxc_l_tmp_native_call_result_n1173 = GetScreenWidth();
        int32_t hxc_l_width_h3454f17fc029 = (int32_t)hxc_l_tmp_native_call_result_n1173;
        int32_t hxc_l_tmp_native_call_result_n1174 = GetScreenHeight();
        int32_t hxc_l_height_h496790184051 = (int32_t)hxc_l_tmp_native_call_result_n1174;
        if (hxc_l_titleTextureReady)
        {
          struct Color hxc_l_this1_he0c40ca71a96 = (struct Color){ .r = 255, .g = 255, .b = 255, .a = 255 };
          struct Texture hxc_l_tmp_load_result_n1177 = hxc_l_titleTexture;
          double hxc_l_imageAspect = hxc_f64_divide_zero_safe((double)(int32_t)hxc_l_tmp_load_result_n1177.width, (double)(int32_t)hxc_l_titleTexture.height);
          int32_t hxc_l_tmp_load_result_n1181 = hxc_l_width_h3454f17fc029;
          double hxc_l_screenAspect = hxc_f64_divide_zero_safe((double)hxc_l_tmp_load_result_n1181, (double)hxc_l_height_h496790184051);
          double hxc_l_sourceX = 0.0;
          double hxc_l_sourceY = 0.0;
          double hxc_l_sourceWidth = (double)(int32_t)hxc_l_titleTexture.width;
          double hxc_l_sourceHeight = (double)(int32_t)hxc_l_titleTexture.height;
          double hxc_l_tmp_load_result_n1187 = hxc_l_screenAspect;
          if (hxc_l_tmp_load_result_n1187 > hxc_l_imageAspect)
          {
            struct Texture hxc_l_tmp_load_result_n1189 = hxc_l_titleTexture;
            hxc_l_sourceHeight = hxc_f64_divide_zero_safe((double)(int32_t)hxc_l_tmp_load_result_n1189.width, hxc_l_screenAspect);
            struct Texture hxc_l_tmp_load_result_n1192 = hxc_l_titleTexture;
            hxc_l_sourceY = ((double)(int32_t)hxc_l_tmp_load_result_n1192.height - hxc_l_sourceHeight) * 0.5;
          }
          else
          {
            struct Texture hxc_l_tmp_load_result_n1195 = hxc_l_titleTexture;
            hxc_l_sourceWidth = (double)(int32_t)hxc_l_tmp_load_result_n1195.height * hxc_l_screenAspect;
            struct Texture hxc_l_tmp_load_result_n1198 = hxc_l_titleTexture;
            hxc_l_sourceX = ((double)(int32_t)hxc_l_tmp_load_result_n1198.width - hxc_l_sourceWidth) * 0.5;
          }
          struct Texture hxc_l_tmp_load_result_n1201 = hxc_l_titleTexture;
          double hxc_l_tmp_load_result_n1202 = hxc_l_sourceX;
          double hxc_l_tmp_load_result_n1203 = hxc_l_sourceY;
          double hxc_l_tmp_load_result_n1204 = hxc_l_sourceWidth;
          double hxc_l_tmp_load_result_n1205 = hxc_l_sourceHeight;
          int32_t hxc_l_tmp_load_result_n1207 = hxc_l_width_h3454f17fc029;
          int32_t hxc_l_tmp_load_result_n1208 = hxc_l_height_h496790184051;
          DrawTexturePro(hxc_l_tmp_load_result_n1201, (struct Rectangle){ .x = (float)hxc_l_tmp_load_result_n1202, .y = (float)hxc_l_tmp_load_result_n1203, .width = (float)hxc_l_tmp_load_result_n1204, .height = (float)hxc_l_tmp_load_result_n1205 }, (struct Rectangle){ .x = (float)0.0, .y = (float)0.0, .width = (float)((double)hxc_l_tmp_load_result_n1207 + 0.0), .height = (float)((double)hxc_l_tmp_load_result_n1208 + 0.0) }, (struct Vector2){ .x = (float)0.0, .y = (float)0.0 }, (float)0.0, hxc_l_this1_he0c40ca71a96);
        }
        else
        {
          ClearBackground((struct Color){ .r = 126, .g = 190, .b = 201, .a = 255 });
        }
        struct Color hxc_l_this1_h65e5e0b6262a = (struct Color){ .r = 4, .g = 14, .b = 23, .a = 132 };
        int32_t hxc_l_tmp_load_result_n1214 = hxc_l_width_h3454f17fc029;
        int32_t hxc_l_tmp_load_result_n1215 = hxc_l_height_h496790184051;
        DrawRectangle((int32_t)0, (int32_t)0, (int32_t)hxc_l_tmp_load_result_n1214, (int32_t)hxc_l_tmp_load_result_n1215, hxc_l_this1_h65e5e0b6262a);
        if (hxc_l_wordmarkTextureReady)
        {
          int32_t hxc_l_centerX = hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_l_width_h3454f17fc029, (double)2));
          struct Color hxc_l_this1_hfc659da9244b = (struct Color){ .r = 255, .g = 255, .b = 255, .a = 255 };
          double hxc_l_scale = hxc_f64_divide_zero_safe((double)560, (double)(int32_t)hxc_l_wordmarkTexture.width);
          double hxc_l_heightScale = hxc_f64_divide_zero_safe((double)132, (double)(int32_t)hxc_l_wordmarkTexture.height);
          double hxc_l_tmp_load_result_n1224 = hxc_l_heightScale;
          if (hxc_l_tmp_load_result_n1224 < hxc_l_scale)
          {
            hxc_l_scale = hxc_l_heightScale;
          }
          struct Texture hxc_l_tmp_load_result_n1227 = hxc_l_wordmarkTexture;
          double hxc_l_width_h28f9650d76c9 = (double)(int32_t)hxc_l_tmp_load_result_n1227.width * hxc_l_scale;
          struct Texture hxc_l_tmp_load_result_n1230 = hxc_l_wordmarkTexture;
          double hxc_l_height_h8302b7c1eff5 = (double)(int32_t)hxc_l_tmp_load_result_n1230.height * hxc_l_scale;
          double hxc_l_width1 = (double)(int32_t)hxc_l_wordmarkTexture.width + 0.0;
          double hxc_l_height1 = (double)(int32_t)hxc_l_wordmarkTexture.height + 0.0;
          struct Texture hxc_l_tmp_load_result_n1237 = hxc_l_wordmarkTexture;
          double hxc_l_tmp_load_result_n1238 = hxc_l_width1;
          double hxc_l_tmp_load_result_n1239 = hxc_l_height1;
          int32_t hxc_l_tmp_load_result_n1241 = hxc_l_centerX;
          double hxc_l_tmp_load_result_n1242 = hxc_l_width_h28f9650d76c9;
          double hxc_l_tmp_load_result_n1243 = hxc_l_width_h28f9650d76c9;
          double hxc_l_tmp_load_result_n1244 = hxc_l_height_h8302b7c1eff5;
          DrawTexturePro(hxc_l_tmp_load_result_n1237, (struct Rectangle){ .x = (float)0.0, .y = (float)0.0, .width = (float)hxc_l_tmp_load_result_n1238, .height = (float)hxc_l_tmp_load_result_n1239 }, (struct Rectangle){ .x = (float)((double)hxc_l_tmp_load_result_n1241 - hxc_l_tmp_load_result_n1242 * 0.5), .y = (float)28.0, .width = (float)hxc_l_tmp_load_result_n1243, .height = (float)hxc_l_tmp_load_result_n1244 }, (struct Vector2){ .x = (float)0.0, .y = (float)0.0 }, (float)0.0, hxc_l_this1_hfc659da9244b);
        }
        else
        {
          int32_t hxc_l_x_h59cd7e755b4c = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_l_width_h3454f17fc029, (double)2)), 92);
          struct Color hxc_l_this1_h2a55a9f4007e = (struct Color){ .r = 242, .g = 249, .b = 245, .a = 255 };
          struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_tmp_load_result_n1250 = hxc_l_uiCatalog;
          hxc_l_gc_roots[249] = (const void *)hxc_l_tmp_load_result_n1250;
          if (hxc_l_tmp_load_result_n1250 == NULL)
          {
            abort();
          }
          hxc_string hxc_l_tmp_instance_call_result_n1252 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_tmp_load_result_n1250, hxc_l_locale, 34);
          hxc_l_tmp_cstring_ref_owner_owner_n395 = hxc_l_tmp_instance_call_result_n1252;
          hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n1254 = HXC_BORROWED_CSTRING_INITIALIZER;
          if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n395, &hxc_l_tmp_string_borrow_cstring_result_n1254) != HXC_STATUS_OK)
          {
            abort();
          }
          int32_t hxc_l_tmp_load_result_n1255 = hxc_l_x_h59cd7e755b4c;
          DrawText(hxc_l_tmp_string_borrow_cstring_result_n1254.data, (int32_t)hxc_l_tmp_load_result_n1255, (int32_t)52, (int32_t)34, hxc_l_this1_h2a55a9f4007e);
          if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n395) != HXC_STATUS_OK)
          {
            abort();
          }
        }
        int32_t hxc_l_firstTop = hxc_i32_subtract_wrapping(hxc_l_height_h496790184051, 293);
        int32_t hxc_l_left_h7312932f3767 = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_l_width_h3454f17fc029, (double)2)), 180);
        if (hxc_l_selectedMode == 0)
        {
          struct Color hxc_l_this1_h0a1a062d0fcc = (struct Color){ .r = 16, .g = 88, .b = 102, .a = 232 };
          int32_t hxc_l_tmp_load_result_n1261 = hxc_l_left_h7312932f3767;
          int32_t hxc_l_tmp_load_result_n1262 = hxc_l_firstTop;
          DrawRectangle((int32_t)hxc_l_tmp_load_result_n1261, (int32_t)hxc_l_tmp_load_result_n1262, (int32_t)360, (int32_t)54, hxc_l_this1_h0a1a062d0fcc);
          struct Color hxc_l_this1_hae519ccb0a45 = (struct Color){ .r = 255, .g = 190, .b = 55, .a = 255 };
          int32_t hxc_l_tmp_load_result_n1265 = hxc_l_left_h7312932f3767;
          int32_t hxc_l_tmp_load_result_n1266 = hxc_l_firstTop;
          DrawRectangleLines((int32_t)hxc_l_tmp_load_result_n1265, (int32_t)hxc_l_tmp_load_result_n1266, (int32_t)360, (int32_t)54, hxc_l_this1_hae519ccb0a45);
        }
        else
        {
          struct Color hxc_l_this1_hed45fb762e42 = (struct Color){ .r = 6, .g = 26, .b = 36, .a = 218 };
          int32_t hxc_l_tmp_load_result_n1269 = hxc_l_left_h7312932f3767;
          int32_t hxc_l_tmp_load_result_n1270 = hxc_l_firstTop;
          DrawRectangle((int32_t)hxc_l_tmp_load_result_n1269, (int32_t)hxc_l_tmp_load_result_n1270, (int32_t)360, (int32_t)54, hxc_l_this1_hed45fb762e42);
          struct Color hxc_l_this1_hb09cfcb49ae0 = (struct Color){ .r = 92, .g = 194, .b = 188, .a = 255 };
          int32_t hxc_l_tmp_load_result_n1273 = hxc_l_left_h7312932f3767;
          int32_t hxc_l_tmp_load_result_n1274 = hxc_l_firstTop;
          DrawRectangleLines((int32_t)hxc_l_tmp_load_result_n1273, (int32_t)hxc_l_tmp_load_result_n1274, (int32_t)360, (int32_t)54, hxc_l_this1_hb09cfcb49ae0);
        }
        struct Color hxc_l_this1_h0ec82fa286d4 = (struct Color){ .r = 242, .g = 249, .b = 245, .a = 255 };
        struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_tmp_load_result_n1277 = hxc_l_uiCatalog;
        hxc_l_gc_roots[250] = (const void *)hxc_l_tmp_load_result_n1277;
        if (hxc_l_tmp_load_result_n1277 == NULL)
        {
          abort();
        }
        hxc_string hxc_l_tmp_instance_call_result_n1279 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_tmp_load_result_n1277, hxc_l_locale, 27);
        hxc_l_tmp_cstring_ref_owner_owner_n403 = hxc_l_tmp_instance_call_result_n1279;
        hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n1281 = HXC_BORROWED_CSTRING_INITIALIZER;
        if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n403, &hxc_l_tmp_string_borrow_cstring_result_n1281) != HXC_STATUS_OK)
        {
          abort();
        }
        int32_t hxc_l_tmp_load_result_n1282 = hxc_l_left_h7312932f3767;
        int32_t hxc_l_tmp_load_result_n1283 = hxc_l_firstTop;
        DrawText(hxc_l_tmp_string_borrow_cstring_result_n1281.data, (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n1282, 28), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n1283, 16), (int32_t)21, hxc_l_this1_h0ec82fa286d4);
        int32_t hxc_l_top_h9297aaf9d103 = hxc_i32_add_wrapping(hxc_i32_add_wrapping(hxc_l_firstTop, 54), 14);
        int32_t hxc_l_left_h9eb1ff025ba3 = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_l_width_h3454f17fc029, (double)2)), 180);
        if (hxc_l_selectedMode == 1)
        {
          struct Color hxc_l_this1_hec1771a09d88 = (struct Color){ .r = 16, .g = 88, .b = 102, .a = 232 };
          int32_t hxc_l_tmp_load_result_n1289 = hxc_l_left_h9eb1ff025ba3;
          int32_t hxc_l_tmp_load_result_n1290 = hxc_l_top_h9297aaf9d103;
          DrawRectangle((int32_t)hxc_l_tmp_load_result_n1289, (int32_t)hxc_l_tmp_load_result_n1290, (int32_t)360, (int32_t)54, hxc_l_this1_hec1771a09d88);
          struct Color hxc_l_this1_hbeb8fa39fc7a = (struct Color){ .r = 255, .g = 190, .b = 55, .a = 255 };
          int32_t hxc_l_tmp_load_result_n1293 = hxc_l_left_h9eb1ff025ba3;
          int32_t hxc_l_tmp_load_result_n1294 = hxc_l_top_h9297aaf9d103;
          DrawRectangleLines((int32_t)hxc_l_tmp_load_result_n1293, (int32_t)hxc_l_tmp_load_result_n1294, (int32_t)360, (int32_t)54, hxc_l_this1_hbeb8fa39fc7a);
        }
        else
        {
          struct Color hxc_l_this1_h3e74198a2b48 = (struct Color){ .r = 6, .g = 26, .b = 36, .a = 218 };
          int32_t hxc_l_tmp_load_result_n1297 = hxc_l_left_h9eb1ff025ba3;
          int32_t hxc_l_tmp_load_result_n1298 = hxc_l_top_h9297aaf9d103;
          DrawRectangle((int32_t)hxc_l_tmp_load_result_n1297, (int32_t)hxc_l_tmp_load_result_n1298, (int32_t)360, (int32_t)54, hxc_l_this1_h3e74198a2b48);
          struct Color hxc_l_this1_h890e67330fd6 = (struct Color){ .r = 92, .g = 194, .b = 188, .a = 255 };
          int32_t hxc_l_tmp_load_result_n1301 = hxc_l_left_h9eb1ff025ba3;
          int32_t hxc_l_tmp_load_result_n1302 = hxc_l_top_h9297aaf9d103;
          DrawRectangleLines((int32_t)hxc_l_tmp_load_result_n1301, (int32_t)hxc_l_tmp_load_result_n1302, (int32_t)360, (int32_t)54, hxc_l_this1_h890e67330fd6);
        }
        struct Color hxc_l_this1_hd89aa0fc2675 = (struct Color){ .r = 242, .g = 249, .b = 245, .a = 255 };
        struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_tmp_load_result_n1305 = hxc_l_uiCatalog;
        hxc_l_gc_roots[251] = (const void *)hxc_l_tmp_load_result_n1305;
        if (hxc_l_tmp_load_result_n1305 == NULL)
        {
          abort();
        }
        hxc_string hxc_l_tmp_instance_call_result_n1307 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_tmp_load_result_n1305, hxc_l_locale, 26);
        hxc_l_tmp_cstring_ref_owner_owner_n411 = hxc_l_tmp_instance_call_result_n1307;
        hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n1309 = HXC_BORROWED_CSTRING_INITIALIZER;
        if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n411, &hxc_l_tmp_string_borrow_cstring_result_n1309) != HXC_STATUS_OK)
        {
          abort();
        }
        int32_t hxc_l_tmp_load_result_n1310 = hxc_l_left_h9eb1ff025ba3;
        int32_t hxc_l_tmp_load_result_n1311 = hxc_l_top_h9297aaf9d103;
        DrawText(hxc_l_tmp_string_borrow_cstring_result_n1309.data, (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n1310, 28), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n1311, 16), (int32_t)21, hxc_l_this1_hd89aa0fc2675);
        int32_t hxc_l_top_hacada3e4c21c = hxc_i32_add_wrapping(hxc_l_firstTop, 136);
        int32_t hxc_l_left_h84cf3b2525f3 = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_l_width_h3454f17fc029, (double)2)), 180);
        struct Color hxc_l_this1_h68f410658fb0 = (struct Color){ .r = 6, .g = 26, .b = 36, .a = 218 };
        int32_t hxc_l_tmp_load_result_n1316 = hxc_l_left_h84cf3b2525f3;
        int32_t hxc_l_tmp_load_result_n1317 = hxc_l_top_hacada3e4c21c;
        DrawRectangle((int32_t)hxc_l_tmp_load_result_n1316, (int32_t)hxc_l_tmp_load_result_n1317, (int32_t)360, (int32_t)54, hxc_l_this1_h68f410658fb0);
        struct Color hxc_l_this1_hf37609706cee = (struct Color){ .r = 92, .g = 194, .b = 188, .a = 255 };
        int32_t hxc_l_tmp_load_result_n1320 = hxc_l_left_h84cf3b2525f3;
        int32_t hxc_l_tmp_load_result_n1321 = hxc_l_top_hacada3e4c21c;
        DrawRectangleLines((int32_t)hxc_l_tmp_load_result_n1320, (int32_t)hxc_l_tmp_load_result_n1321, (int32_t)360, (int32_t)54, hxc_l_this1_hf37609706cee);
        struct Color hxc_l_this1_hf661858f223e = (struct Color){ .r = 242, .g = 249, .b = 245, .a = 255 };
        struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_tmp_load_result_n1324 = hxc_l_uiCatalog;
        hxc_l_gc_roots[252] = (const void *)hxc_l_tmp_load_result_n1324;
        if (hxc_l_tmp_load_result_n1324 == NULL)
        {
          abort();
        }
        hxc_string hxc_l_tmp_instance_call_result_n1326 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_tmp_load_result_n1324, hxc_l_locale, 28);
        hxc_l_tmp_cstring_ref_owner_owner_n417 = hxc_l_tmp_instance_call_result_n1326;
        hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n1328 = HXC_BORROWED_CSTRING_INITIALIZER;
        if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n417, &hxc_l_tmp_string_borrow_cstring_result_n1328) != HXC_STATUS_OK)
        {
          abort();
        }
        int32_t hxc_l_tmp_load_result_n1329 = hxc_l_left_h84cf3b2525f3;
        int32_t hxc_l_tmp_load_result_n1330 = hxc_l_top_hacada3e4c21c;
        DrawText(hxc_l_tmp_string_borrow_cstring_result_n1328.data, (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n1329, 28), (int32_t)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n1330, 16), (int32_t)21, hxc_l_this1_hf661858f223e);
        int32_t hxc_l_x_h07f88ce40b67 = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_l_width_h3454f17fc029, (double)2)), 285);
        struct Color hxc_l_this1_h47f57427456f = (struct Color){ .r = 229, .g = 241, .b = 235, .a = 255 };
        struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_tmp_load_result_n1334 = hxc_l_uiCatalog;
        hxc_l_gc_roots[253] = (const void *)hxc_l_tmp_load_result_n1334;
        if (hxc_l_tmp_load_result_n1334 == NULL)
        {
          abort();
        }
        hxc_string hxc_l_tmp_instance_call_result_n1336 = hxc_caxecraft_localization_RuntimeUiCatalog_text(hxc_l_tmp_load_result_n1334, hxc_l_locale, 29);
        hxc_l_tmp_cstring_ref_owner_owner_n420 = hxc_l_tmp_instance_call_result_n1336;
        hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n1338 = HXC_BORROWED_CSTRING_INITIALIZER;
        if (hxc_string_borrow_cstring(&hxc_l_tmp_cstring_ref_owner_owner_n420, &hxc_l_tmp_string_borrow_cstring_result_n1338) != HXC_STATUS_OK)
        {
          abort();
        }
        int32_t hxc_l_tmp_load_result_n1339 = hxc_l_x_h07f88ce40b67;
        int32_t hxc_l_tmp_load_result_n1340 = hxc_l_height_h496790184051;
        DrawText(hxc_l_tmp_string_borrow_cstring_result_n1338.data, (int32_t)hxc_l_tmp_load_result_n1339, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n1340, 58), (int32_t)16, hxc_l_this1_h47f57427456f);
        int32_t hxc_l_tmp_load_result_n1342 = hxc_l_selectedMode;
        bool hxc_l_tmp_short_circuit_result_n421 = hxc_l_tmp_load_result_n1342 == 1;
        if (hxc_l_tmp_load_result_n1342 == 1)
        {
          hxc_string hxc_l_tmp_load_result_n1343 = hxc_l_adventureTagline;
          if (hxc_l_tmp_load_result_n1343.data == NULL)
          {
            abort();
          }
          int32_t hxc_l_tmp_string_length_result_n1344;
          if (hxc_string_haxe_length(hxc_l_tmp_load_result_n1343, &hxc_l_tmp_string_length_result_n1344) != HXC_STATUS_OK)
          {
            abort();
          }
          hxc_l_tmp_short_circuit_result_n421 = hxc_l_tmp_string_length_result_n1344 > 0;
        }
        if (hxc_l_tmp_short_circuit_result_n421)
        {
          int32_t hxc_l_x_h3774fe825689 = hxc_i32_subtract_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_l_width_h3454f17fc029, (double)2)), 230);
          struct Color hxc_l_this1_h0be1fc1440c9 = (struct Color){ .r = 255, .g = 205, .b = 91, .a = 255 };
          hxc_borrowed_cstring hxc_l_tmp_string_borrow_cstring_result_n1349 = HXC_BORROWED_CSTRING_INITIALIZER;
          if (hxc_string_borrow_cstring(&hxc_l_adventureTagline, &hxc_l_tmp_string_borrow_cstring_result_n1349) != HXC_STATUS_OK)
          {
            abort();
          }
          int32_t hxc_l_tmp_load_result_n1350 = hxc_l_x_h3774fe825689;
          int32_t hxc_l_tmp_load_result_n1351 = hxc_l_firstTop;
          DrawText(hxc_l_tmp_string_borrow_cstring_result_n1349.data, (int32_t)hxc_l_tmp_load_result_n1350, (int32_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n1351, 36), (int32_t)17, hxc_l_this1_h0be1fc1440c9);
        }
        if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n420) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n417) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n411) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_string_release(&hxc_l_tmp_cstring_ref_owner_owner_n403) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_string_release(&hxc_l_adventureTagline) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n373) != HXC_STATUS_OK)
        {
          abort();
        }
      }
      else
      {
        if (hxc_l_onCampaignSelect)
        {
          hxc_l_gc_roots[254] = (const void *)hxc_l_campaign;
          struct hxc_caxecraft_content_CampaignManifest *hxc_l_selectedCampaign_hc76ce5dd3d2d = hxc_l_campaign;
          hxc_l_gc_roots[255] = (const void *)hxc_l_selectedCampaign_hc76ce5dd3d2d;
          hxc_l_gc_roots[256] = (const void *)NULL;
          if (hxc_l_selectedCampaign_hc76ce5dd3d2d == NULL)
          {
            enum hxc_caxecraft_app_AppScreen hxc_l_tmp_call_result_n1357 = hxc_caxecraft_app_AppScreen_closeCampaignSelection(hxc_l_screen);
            hxc_l_screen = hxc_l_tmp_call_result_n1357;
          }
          else
          {
            hxc_string hxc_l_locale1_h2d7bf71b0e39 = { 0 };
            switch (hxc_l_locale) {
              case 0:
                {
                  hxc_l_locale1_h2d7bf71b0e39 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
                  if (hxc_string_retain(hxc_l_locale1_h2d7bf71b0e39) != HXC_STATUS_OK)
                  {
                    abort();
                  }
                  break;
                }
              case 1:
                {
                  hxc_l_locale1_h2d7bf71b0e39 = (hxc_string){ (const uint8_t *)"es-mx", 5, true, NULL };
                  if (hxc_string_retain(hxc_l_locale1_h2d7bf71b0e39) != HXC_STATUS_OK)
                  {
                    abort();
                  }
                  break;
                }
              default:
                {
                  hxc_l_locale1_h2d7bf71b0e39 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
                  if (hxc_string_retain(hxc_l_locale1_h2d7bf71b0e39) != HXC_STATUS_OK)
                  {
                    abort();
                  }
                  break;
                }
            }
            hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n1359 = hxc_l_locale1_h2d7bf71b0e39;
            hxc_l_tmp_managed_flow_owner_n426 = hxc_l_tmp_managed_flow_carrier_move_result_n1359;
            struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1360 = hxc_l_levelView_hc35c2f93673f;
            hxc_l_gc_roots[257] = (const void *)hxc_l_tmp_load_result_n1360;
            if (hxc_l_tmp_load_result_n1360 == NULL)
            {
              abort();
            }
            struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_class_field_load_result_n1361 = (*hxc_l_tmp_load_result_n1360).hxc_levelPresentation;
            hxc_l_gc_roots[258] = (const void *)hxc_l_tmp_class_field_load_result_n1361;
            struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_this = hxc_l_tmp_class_field_load_result_n1361;
            struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_load_result_n1362 = hxc_l_this;
            hxc_l_gc_roots[259] = (const void *)hxc_l_tmp_load_result_n1362;
            if (hxc_l_tmp_load_result_n1362 == NULL)
            {
              abort();
            }
            struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_load_result_n1363 = hxc_l_this;
            hxc_l_gc_roots[260] = (const void *)hxc_l_tmp_load_result_n1363;
            if (hxc_l_tmp_load_result_n1363 == NULL)
            {
              abort();
            }
            struct hxc_caxecraft_scenario_ScenarioText hxc_l_tmp_class_field_load_result_n1364 = (*hxc_l_tmp_load_result_n1363).hxc_title;
            hxc_string hxc_l_tmp_instance_call_result_n1366 = hxc_caxecraft_content_RuntimeLevelPresentation_resolve(hxc_l_tmp_load_result_n1362, hxc_l_tmp_class_field_load_result_n1364, hxc_l_tmp_managed_flow_owner_n426);
            hxc_l_tmp = hxc_l_tmp_instance_call_result_n1366;
            hxc_string hxc_l_locale1_hab7686f0e894 = { 0 };
            switch (hxc_l_locale) {
              case 0:
                {
                  hxc_l_locale1_hab7686f0e894 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
                  if (hxc_string_retain(hxc_l_locale1_hab7686f0e894) != HXC_STATUS_OK)
                  {
                    abort();
                  }
                  break;
                }
              case 1:
                {
                  hxc_l_locale1_hab7686f0e894 = (hxc_string){ (const uint8_t *)"es-mx", 5, true, NULL };
                  if (hxc_string_retain(hxc_l_locale1_hab7686f0e894) != HXC_STATUS_OK)
                  {
                    abort();
                  }
                  break;
                }
              default:
                {
                  hxc_l_locale1_hab7686f0e894 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
                  if (hxc_string_retain(hxc_l_locale1_hab7686f0e894) != HXC_STATUS_OK)
                  {
                    abort();
                  }
                  break;
                }
            }
            hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n1368 = hxc_l_locale1_hab7686f0e894;
            hxc_l_tmp_managed_flow_owner_n430 = hxc_l_tmp_managed_flow_carrier_move_result_n1368;
            struct Texture hxc_l_tmp_load_result_n1369 = hxc_l_titleTexture;
            bool hxc_l_tmp_load_result_n1370 = hxc_l_titleTextureReady;
            struct Texture hxc_l_tmp_load_result_n1371 = hxc_l_wordmarkTexture;
            bool hxc_l_tmp_load_result_n1372 = hxc_l_wordmarkTextureReady;
            struct hxc_caxecraft_content_CampaignManifest *hxc_l_tmp_load_result_n1373 = hxc_l_selectedCampaign_hc76ce5dd3d2d;
            hxc_l_gc_roots[261] = (const void *)hxc_l_tmp_load_result_n1373;
            int32_t hxc_l_tmp_load_result_n1374 = hxc_l_locale;
            struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_tmp_load_result_n1375 = hxc_l_uiCatalog;
            hxc_l_gc_roots[262] = (const void *)hxc_l_tmp_load_result_n1375;
            int32_t hxc_l_tmp_load_result_n1376 = hxc_l_selectedCampaignLevelIndex;
            hxc_string hxc_l_tmp_load_result_n1377 = hxc_l_tmp;
            struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1378 = hxc_l_levelView_hc35c2f93673f;
            hxc_l_gc_roots[263] = (const void *)hxc_l_tmp_load_result_n1378;
            if (hxc_l_tmp_load_result_n1378 == NULL)
            {
              abort();
            }
            struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_class_field_load_result_n1379 = (*hxc_l_tmp_load_result_n1378).hxc_levelPresentation;
            hxc_l_gc_roots[264] = (const void *)hxc_l_tmp_class_field_load_result_n1379;
            if (hxc_l_tmp_class_field_load_result_n1379 == NULL)
            {
              abort();
            }
            hxc_string hxc_l_tmp_instance_call_result_n1381 = hxc_caxecraft_content_RuntimeLevelPresentation_message(hxc_l_tmp_class_field_load_result_n1379, (hxc_string){ (const uint8_t *)"adventure_tagline", 17, true, NULL }, hxc_l_tmp_managed_flow_owner_n430);
            hxc_l_tmp_static_call_argument_9_owner_n431 = hxc_l_tmp_instance_call_result_n1381;
            hxc_caxecraft_app_CampaignMenu_draw(hxc_l_tmp_load_result_n1369, hxc_l_tmp_load_result_n1370, hxc_l_tmp_load_result_n1371, hxc_l_tmp_load_result_n1372, hxc_l_tmp_load_result_n1373, hxc_l_tmp_load_result_n1374, hxc_l_tmp_load_result_n1375, hxc_l_tmp_load_result_n1376, hxc_l_tmp_load_result_n1377, hxc_l_tmp_static_call_argument_9_owner_n431);
            if (hxc_string_release(&hxc_l_tmp_static_call_argument_9_owner_n431) != HXC_STATUS_OK)
            {
              abort();
            }
            if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n430) != HXC_STATUS_OK)
            {
              abort();
            }
            if (hxc_string_release(&hxc_l_tmp) != HXC_STATUS_OK)
            {
              abort();
            }
            if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n426) != HXC_STATUS_OK)
            {
              abort();
            }
          }
        }
        else
        {
          if (hxc_l_onLoading)
          {
            hxc_string hxc_l_tmp_load_result_n1384 = hxc_l_pendingCampaignLabel;
            int32_t hxc_l_tmp_load_result_n1385 = hxc_l_locale;
            hxc_l_gc_roots[265] = (const void *)hxc_l_uiCatalog;
            hxc_caxecraft_app_CaxecraftApp_drawCampaignLoading(hxc_l_tmp_load_result_n1384, hxc_l_tmp_load_result_n1385, hxc_l_uiCatalog);
          }
          else
          {
            if (hxc_l_onEditor)
            {
              struct hxc_caxecraft_app_CaxecraftEditorScreen *hxc_l_tmp_load_result_n1388 = hxc_l_editorScreen;
              hxc_l_gc_roots[266] = (const void *)hxc_l_tmp_load_result_n1388;
              if (hxc_l_tmp_load_result_n1388 == NULL)
              {
                abort();
              }
              int32_t hxc_l_tmp_load_result_n1389 = hxc_l_locale;
              enum hxc_caxecraft_app_EditorScreenAction hxc_l_tmp_instance_call_result_n1391 = hxc_caxecraft_app_CaxecraftEditorScreen_draw(hxc_l_tmp_load_result_n1388, hxc_l_tmp_load_result_n1389, hxc_l_editorNavigationCommand);
              if (hxc_l_tmp_instance_call_result_n1391 == hxc_caxecraft_app_EditorScreenAction_ReturnToTitle)
              {
                enum hxc_caxecraft_app_AppScreen hxc_l_tmp_call_result_n1394 = hxc_caxecraft_app_AppScreen_closeEditor(hxc_l_screen);
                hxc_l_screen = hxc_l_tmp_call_result_n1394;
              }
            }
            else
            {
              ClearBackground((struct Color){ .r = 126, .g = 190, .b = 201, .a = 255 });
              BeginMode3D(hxc_l_camera);
              hxc_caxecraft_app_CaxecraftApp_drawWorldSun();
              if (hxc_l_self == NULL)
              {
                abort();
              }
              struct hxc_caxecraft_app_TerrainRenderer *hxc_l_tmp_owned_class_field_address_n1397 = &(*hxc_l_self).hxc_terrainRenderer;
              if (hxc_l_tmp_owned_class_field_address_n1397 == NULL)
              {
                abort();
              }
              struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1398 = hxc_l_session;
              hxc_l_gc_roots[267] = (const void *)hxc_l_tmp_load_result_n1398;
              if (hxc_l_tmp_load_result_n1398 == NULL)
              {
                abort();
              }
              size_t hxc_l_tmp_length_n1400;
              const uint8_t *hxc_l_tmp_instance_call_result_n1399 = hxc_caxecraft_domain_GameSession_worldView(hxc_l_tmp_load_result_n1398, &hxc_l_tmp_length_n1400);
              struct Texture hxc_l_tmp_load_result_n1401 = hxc_l_terrainTexture;
              bool hxc_l_tmp_load_result_n1402 = hxc_l_terrainTextureReady;
              struct Texture hxc_l_tmp_load_result_n1403 = hxc_l_adventureTerrainTexture;
              bool hxc_l_tmp_load_result_n1404 = hxc_l_adventureTerrainTextureReady;
              double hxc_l_tmp_record_field_load_result_n1405 = hxc_l_renderPosition.hxc_x;
              struct hxc_caxecraft_app_TerrainRenderCounters hxc_l_tmp_instance_call_result_n1407 = hxc_caxecraft_app_TerrainRenderer_draw(hxc_l_tmp_owned_class_field_address_n1397, hxc_l_tmp_instance_call_result_n1399, hxc_l_tmp_length_n1400, hxc_l_tmp_load_result_n1401, hxc_l_tmp_load_result_n1402, hxc_l_tmp_load_result_n1403, hxc_l_tmp_load_result_n1404, hxc_l_tmp_record_field_load_result_n1405, hxc_l_renderPosition.hxc_z);
              struct hxc_caxecraft_app_TerrainRenderCounters hxc_l_renderCounters = hxc_l_tmp_instance_call_result_n1407;
              struct hxc_caxecraft_app_WaterRenderer *hxc_l_tmp_owned_class_field_address_n1408 = &(*hxc_l_self).hxc_waterRenderer;
              if (hxc_l_tmp_owned_class_field_address_n1408 == NULL)
              {
                abort();
              }
              struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1409 = hxc_l_session;
              hxc_l_gc_roots[268] = (const void *)hxc_l_tmp_load_result_n1409;
              if (hxc_l_tmp_load_result_n1409 == NULL)
              {
                abort();
              }
              size_t hxc_l_tmp_length_n1411;
              const uint8_t *hxc_l_tmp_instance_call_result_n1410 = hxc_caxecraft_domain_GameSession_worldView(hxc_l_tmp_load_result_n1409, &hxc_l_tmp_length_n1411);
              struct Texture hxc_l_tmp_load_result_n1412 = hxc_l_terrainTexture;
              bool hxc_l_tmp_load_result_n1413 = hxc_l_terrainTextureReady;
              struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1414 = hxc_l_levelView_hc35c2f93673f;
              hxc_l_gc_roots[269] = (const void *)hxc_l_tmp_load_result_n1414;
              if (hxc_l_tmp_load_result_n1414 == NULL)
              {
                abort();
              }
              int32_t hxc_l_tmp_class_field_load_result_n1415 = (*hxc_l_tmp_load_result_n1414).hxc_waterCell;
              double hxc_l_tmp_load_result_n1416 = hxc_l_eyeX;
              double hxc_l_tmp_load_result_n1417 = hxc_l_eyeY;
              struct hxc_caxecraft_app_WaterRenderCounters hxc_l_tmp_instance_call_result_n1419 = hxc_caxecraft_app_WaterRenderer_draw(hxc_l_tmp_owned_class_field_address_n1408, hxc_l_tmp_instance_call_result_n1410, hxc_l_tmp_length_n1411, hxc_l_tmp_load_result_n1412, hxc_l_tmp_load_result_n1413, hxc_l_tmp_class_field_load_result_n1415, hxc_l_tmp_load_result_n1416, hxc_l_tmp_load_result_n1417, hxc_l_eyeZ);
              struct hxc_caxecraft_app_WaterRenderCounters hxc_l_waterCounters = hxc_l_tmp_instance_call_result_n1419;
              int32_t hxc_l_tmp_record_field_load_result_n1420 = hxc_l_renderCounters.hxc_visible;
              int32_t hxc_l_totalVisible = hxc_i32_add_wrapping(hxc_l_tmp_record_field_load_result_n1420, hxc_l_waterCounters.hxc_visible);
              int32_t hxc_l_tmp_record_field_load_result_n1422 = hxc_l_renderCounters.hxc_drawCalls;
              int32_t hxc_l_totalDrawCalls = hxc_i32_add_wrapping(hxc_l_tmp_record_field_load_result_n1422, hxc_l_waterCounters.hxc_drawCalls);
              struct Camera3D hxc_l_tmp_load_result_n1424 = hxc_l_camera;
              struct Texture hxc_l_tmp_load_result_n1425 = hxc_l_entityTexture;
              bool hxc_l_tmp_load_result_n1426 = hxc_l_entityTextureReady;
              struct hxc_caxecraft_app_RuntimeTextureAtlasCatalog *hxc_l_tmp_load_result_n1427 = hxc_l_runtimeTextures;
              hxc_l_gc_roots[270] = (const void *)hxc_l_tmp_load_result_n1427;
              struct hxc_array_ref *hxc_l_tmp_load_result_n1428 = hxc_l_dialogueActors;
              struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1429 = hxc_l_levelView_hc35c2f93673f;
              hxc_l_gc_roots[271] = (const void *)hxc_l_tmp_load_result_n1429;
              struct hxc_caxecraft_domain_Character hxc_l_tmp_load_result_n1430 = hxc_l_enemyActor;
              struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1431 = hxc_l_levelView_hc35c2f93673f;
              hxc_l_gc_roots[272] = (const void *)hxc_l_tmp_load_result_n1431;
              if (hxc_l_tmp_load_result_n1431 == NULL)
              {
                abort();
              }
              hxc_string hxc_l_tmp_class_field_load_result_n1432 = (*hxc_l_tmp_load_result_n1431).hxc_enemyAsset;
              struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1433 = hxc_l_levelView_hc35c2f93673f;
              hxc_l_gc_roots[273] = (const void *)hxc_l_tmp_load_result_n1433;
              if (hxc_l_tmp_load_result_n1433 == NULL)
              {
                abort();
              }
              int32_t hxc_l_tmp_class_field_load_result_n1434 = (*hxc_l_tmp_load_result_n1433).hxc_enemyCell;
              enum hxc_caxecraft_domain_ActorControllerPhase hxc_l_tmp_record_field_load_result_n1435 = hxc_l_enemyPhase_h695cd3d136cc.hxc_phase;
              hxc_caxecraft_app_CaxecraftApp_drawActors(hxc_l_tmp_load_result_n1424, hxc_l_tmp_load_result_n1425, hxc_l_tmp_load_result_n1426, hxc_l_tmp_load_result_n1427, hxc_l_tmp_load_result_n1428, hxc_l_tmp_load_result_n1429, hxc_l_tmp_load_result_n1430, hxc_l_tmp_class_field_load_result_n1432, hxc_l_tmp_class_field_load_result_n1434, hxc_l_tmp_record_field_load_result_n1435, hxc_l_berryDrop);
              struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_tmp_load_result_n1437 = hxc_l_contentRegistry;
              hxc_l_gc_roots[274] = (const void *)hxc_l_tmp_load_result_n1437;
              struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1438 = hxc_l_session;
              hxc_l_gc_roots[275] = (const void *)hxc_l_tmp_load_result_n1438;
              struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1439 = hxc_l_levelView_hc35c2f93673f;
              hxc_l_gc_roots[276] = (const void *)hxc_l_tmp_load_result_n1439;
              struct Camera3D hxc_l_tmp_load_result_n1440 = hxc_l_camera;
              struct Texture hxc_l_tmp_load_result_n1441 = hxc_l_entityTexture;
              bool hxc_l_tmp_load_result_n1442 = hxc_l_entityTextureReady;
              struct Texture hxc_l_tmp_load_result_n1443 = hxc_l_itemTexture;
              bool hxc_l_tmp_load_result_n1444 = hxc_l_itemTextureReady;
              struct Texture hxc_l_tmp_load_result_n1445 = hxc_l_adventureItemTexture;
              bool hxc_l_tmp_load_result_n1446 = hxc_l_adventureItemTextureReady;
              struct Texture hxc_l_tmp_load_result_n1447 = hxc_l_terrainTexture;
              bool hxc_l_tmp_load_result_n1448 = hxc_l_terrainTextureReady;
              hxc_l_gc_roots[277] = (const void *)hxc_l_runtimeTextures;
              hxc_caxecraft_app_StatefulObjectRenderer_drawStatefulObjects(hxc_l_tmp_load_result_n1437, hxc_l_tmp_load_result_n1438, hxc_l_tmp_load_result_n1439, hxc_l_tmp_load_result_n1440, hxc_l_tmp_load_result_n1441, hxc_l_tmp_load_result_n1442, hxc_l_tmp_load_result_n1443, hxc_l_tmp_load_result_n1444, hxc_l_tmp_load_result_n1445, hxc_l_tmp_load_result_n1446, hxc_l_tmp_load_result_n1447, hxc_l_tmp_load_result_n1448, hxc_l_runtimeTextures);
              struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_tmp_load_result_n1450 = hxc_l_contentRegistry;
              hxc_l_gc_roots[278] = (const void *)hxc_l_tmp_load_result_n1450;
              struct Camera3D hxc_l_tmp_load_result_n1451 = hxc_l_camera;
              struct hxc_caxecraft_domain_GameSession *hxc_l_tmp_load_result_n1452 = hxc_l_session;
              hxc_l_gc_roots[279] = (const void *)hxc_l_tmp_load_result_n1452;
              if (hxc_l_tmp_load_result_n1452 == NULL)
              {
                abort();
              }
              size_t hxc_l_tmp_length_n1454;
              const int32_t *hxc_l_tmp_instance_call_result_n1453 = hxc_caxecraft_domain_GameSession_authoredItemsView(hxc_l_tmp_load_result_n1452, &hxc_l_tmp_length_n1454);
              struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1455 = hxc_l_levelView_hc35c2f93673f;
              hxc_l_gc_roots[280] = (const void *)hxc_l_tmp_load_result_n1455;
              struct Texture hxc_l_tmp_load_result_n1456 = hxc_l_itemTexture;
              bool hxc_l_tmp_load_result_n1457 = hxc_l_itemTextureReady;
              struct Texture hxc_l_tmp_load_result_n1458 = hxc_l_adventureItemTexture;
              hxc_caxecraft_app_AuthoredItemRenderer_drawWorldItems(hxc_l_tmp_load_result_n1450, hxc_l_tmp_load_result_n1451, hxc_l_tmp_instance_call_result_n1453, hxc_l_tmp_length_n1454, hxc_l_tmp_load_result_n1455, hxc_l_tmp_load_result_n1456, hxc_l_tmp_load_result_n1457, hxc_l_tmp_load_result_n1458, hxc_l_adventureItemTextureReady);
              if (hxc_l_hit.hxc_hit)
              {
                double hxc_l_y = (double)hxc_l_hit.hxc_cellY + 0.5;
                double hxc_l_z = (double)hxc_l_hit.hxc_cellZ + 0.5;
                int32_t hxc_l_tmp_record_field_load_result_n1463 = hxc_l_hit.hxc_cellX;
                double hxc_l_tmp_load_result_n1464 = hxc_l_y;
                DrawCubeWires((struct Vector3){ .x = (float)((double)hxc_l_tmp_record_field_load_result_n1463 + 0.5), .y = (float)hxc_l_tmp_load_result_n1464, .z = (float)hxc_l_z }, (float)1.04, (float)1.04, (float)1.04, (struct Color){ .r = 255, .g = 132, .b = 47, .a = 255 });
              }
              EndMode3D();
              if (hxc_l_cameraWaterBlend > 0.0)
              {
                int32_t hxc_l_overlayAlpha = hxc_f64_to_i32_saturating(105.0 * hxc_l_cameraWaterBlend);
                int32_t hxc_l_tmp_native_call_result_n1470 = GetScreenWidth();
                int32_t hxc_l_width_h310053c207d4 = (int32_t)hxc_l_tmp_native_call_result_n1470;
                int32_t hxc_l_tmp_native_call_result_n1471 = GetScreenHeight();
                int32_t hxc_l_height_h4f61c03683bc = (int32_t)hxc_l_tmp_native_call_result_n1471;
                uint8_t hxc_l_red = (uint8_t)31;
                uint8_t hxc_l_green = (uint8_t)115;
                uint8_t hxc_l_blue = (uint8_t)154;
                int32_t hxc_l_tmp_load_result_n1472 = hxc_l_overlayAlpha;
                int32_t hxc_l_tmp_conditional_result_n445 = 0;
                if (hxc_l_tmp_load_result_n1472 < 0)
                {
                  hxc_l_tmp_conditional_result_n445 = 0;
                }
                else
                {
                  int32_t hxc_l_tmp_load_result_n1473 = hxc_l_overlayAlpha;
                  int32_t hxc_l_tmp_conditional_result_n446 = 0;
                  if (hxc_l_tmp_load_result_n1473 > 255)
                  {
                    hxc_l_tmp_conditional_result_n446 = 255;
                  }
                  else
                  {
                    hxc_l_tmp_conditional_result_n446 = hxc_l_overlayAlpha;
                  }
                  hxc_l_tmp_conditional_result_n445 = hxc_l_tmp_conditional_result_n446;
                }
                uint8_t hxc_l_alpha = (uint8_t)hxc_l_tmp_conditional_result_n445;
                uint8_t hxc_l_tmp_load_result_n1477 = hxc_l_red;
                uint8_t hxc_l_tmp_load_result_n1478 = hxc_l_green;
                uint8_t hxc_l_tmp_load_result_n1479 = hxc_l_blue;
                struct Color hxc_l_this1_h11d754a6fe9c = (struct Color){ .r = hxc_l_tmp_load_result_n1477, .g = hxc_l_tmp_load_result_n1478, .b = hxc_l_tmp_load_result_n1479, .a = hxc_l_alpha };
                int32_t hxc_l_tmp_load_result_n1482 = hxc_l_width_h310053c207d4;
                int32_t hxc_l_tmp_load_result_n1483 = hxc_l_height_h4f61c03683bc;
                DrawRectangle((int32_t)0, (int32_t)0, (int32_t)hxc_l_tmp_load_result_n1482, (int32_t)hxc_l_tmp_load_result_n1483, hxc_l_this1_h11d754a6fe9c);
              }
              double hxc_l_tmp_record_field_load_result_n1485 = hxc_l_character.hxc_body.hxc_x;
              double hxc_l_tmp_record_field_load_result_n1486 = hxc_l_character.hxc_body.hxc_z;
              struct hxc_caxecraft_domain_VitalsState hxc_l_tmp_record_field_load_result_n1487 = hxc_l_character.hxc_vitals;
              int32_t hxc_l_tmp_load_result_n1488 = hxc_l_aquaticEquipmentCode;
              int32_t hxc_l_tmp_load_result_n1489 = hxc_l_aquaticEquipmentFrames;
              bool hxc_l_tmp_record_field_load_result_n1490 = hxc_l_character.hxc_aquatic.hxc_headSubmerged;
              int32_t hxc_l_tmp_record_field_load_result_n1491 = hxc_l_character.hxc_aquatic.hxc_breathTicks;
              struct hxc_caxecraft_app_HudCharacterView hxc_l_hudView_hbda7460f80cb = (struct hxc_caxecraft_app_HudCharacterView){ .hxc_aquaticEquipmentCode = hxc_l_tmp_load_result_n1488, .hxc_aquaticEquipmentVisible = hxc_l_tmp_load_result_n1489 > 0, .hxc_breathTicks = hxc_l_tmp_record_field_load_result_n1491, .hxc_headSubmerged = hxc_l_tmp_record_field_load_result_n1490, .hxc_maximumBreathTicks = hxc_l_character.hxc_aquaticProfile.hxc_maximumBreathTicks, .hxc_vitals = hxc_l_tmp_record_field_load_result_n1487, .hxc_x = hxc_l_tmp_record_field_load_result_n1485, .hxc_z = hxc_l_tmp_record_field_load_result_n1486 };
              bool hxc_l_hudView1 = hxc_l_placementBlockedFrames > 0;
              bool hxc_l_hudView2 = hxc_l_strikeHitFrames > 0;
              bool hxc_l_hudView3 = hxc_l_enemyDefeatedFrames > 0;
              bool hxc_l_hudView4 = hxc_l_enemyAttackFrames > 0;
              bool hxc_l_hudView5 = hxc_l_pickupFrames > 0;
              bool hxc_l_hudView6 = hxc_l_recoveryFeedbackFrames > 0;
              enum hxc_caxecraft_domain_ActorControllerPhase hxc_l_enemyPhase_h551fd7fa3cd5 = hxc_l_enemyPhase_h695cd3d136cc.hxc_phase;
              hxc_string hxc_l_locale1_h2fc3c9f51881 = { 0 };
              switch (hxc_l_locale) {
                case 0:
                  {
                    hxc_l_locale1_h2fc3c9f51881 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
                    if (hxc_string_retain(hxc_l_locale1_h2fc3c9f51881) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    break;
                  }
                case 1:
                  {
                    hxc_l_locale1_h2fc3c9f51881 = (hxc_string){ (const uint8_t *)"es-mx", 5, true, NULL };
                    if (hxc_string_retain(hxc_l_locale1_h2fc3c9f51881) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    break;
                  }
                default:
                  {
                    hxc_l_locale1_h2fc3c9f51881 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
                    if (hxc_string_retain(hxc_l_locale1_h2fc3c9f51881) != HXC_STATUS_OK)
                    {
                      abort();
                    }
                    break;
                  }
              }
              hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n1502 = hxc_l_locale1_h2fc3c9f51881;
              hxc_l_tmp_managed_flow_owner_n457 = hxc_l_tmp_managed_flow_carrier_move_result_n1502;
              struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1503 = hxc_l_levelView_hc35c2f93673f;
              hxc_l_gc_roots[281] = (const void *)hxc_l_tmp_load_result_n1503;
              if (hxc_l_tmp_load_result_n1503 == NULL)
              {
                abort();
              }
              struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_class_field_load_result_n1504 = (*hxc_l_tmp_load_result_n1503).hxc_levelPresentation;
              hxc_l_gc_roots[282] = (const void *)hxc_l_tmp_class_field_load_result_n1504;
              if (hxc_l_tmp_class_field_load_result_n1504 == NULL)
              {
                abort();
              }
              hxc_string hxc_l_tmp_load_result_n1505 = hxc_l_currentObjectiveId;
              hxc_string hxc_l_tmp_instance_call_result_n1507 = hxc_caxecraft_content_RuntimeLevelPresentation_objectiveTitle(hxc_l_tmp_class_field_load_result_n1504, hxc_l_tmp_load_result_n1505, hxc_l_tmp_managed_flow_owner_n457);
              hxc_l_hudView7 = hxc_l_tmp_instance_call_result_n1507;
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
                struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1509 = hxc_l_levelView_hc35c2f93673f;
                hxc_l_gc_roots[283] = (const void *)hxc_l_tmp_load_result_n1509;
                if (hxc_l_tmp_load_result_n1509 == NULL)
                {
                  abort();
                }
                struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_class_field_load_result_n1510 = (*hxc_l_tmp_load_result_n1509).hxc_levelPresentation;
                hxc_l_gc_roots[284] = (const void *)hxc_l_tmp_class_field_load_result_n1510;
                struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_levelView_ha03840803931 = hxc_l_tmp_class_field_load_result_n1510;
                hxc_string hxc_l_hudView_h0f98362d9690 = { 0 };
                switch (hxc_l_locale) {
                  case 0:
                    {
                      hxc_l_hudView_h0f98362d9690 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
                      if (hxc_string_retain(hxc_l_hudView_h0f98362d9690) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      break;
                    }
                  case 1:
                    {
                      hxc_l_hudView_h0f98362d9690 = (hxc_string){ (const uint8_t *)"es-mx", 5, true, NULL };
                      if (hxc_string_retain(hxc_l_hudView_h0f98362d9690) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      break;
                    }
                  default:
                    {
                      hxc_l_hudView_h0f98362d9690 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
                      if (hxc_string_retain(hxc_l_hudView_h0f98362d9690) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      break;
                    }
                }
                hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n1512 = hxc_l_hudView_h0f98362d9690;
                hxc_l_tmp_managed_flow_owner_n462 = hxc_l_tmp_managed_flow_carrier_move_result_n1512;
                struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_load_result_n1513 = hxc_l_levelView_ha03840803931;
                hxc_l_gc_roots[285] = (const void *)hxc_l_tmp_load_result_n1513;
                if (hxc_l_tmp_load_result_n1513 == NULL)
                {
                  abort();
                }
                hxc_string hxc_l_tmp_load_result_n1514 = hxc_l_latestJournalId;
                hxc_string hxc_l_tmp_instance_call_result_n1516 = hxc_caxecraft_content_RuntimeLevelPresentation_journalTitle(hxc_l_tmp_load_result_n1513, hxc_l_tmp_load_result_n1514, hxc_l_tmp_managed_flow_owner_n462);
                hxc_l_hudView8 = hxc_l_tmp_instance_call_result_n1516;
                if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n462) != HXC_STATUS_OK)
                {
                  abort();
                }
              }
              hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n1517 = hxc_l_hudView8;
              hxc_l_tmp_managed_flow_owner_n463 = hxc_l_tmp_managed_flow_carrier_move_result_n1517;
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
                struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1519 = hxc_l_levelView_hc35c2f93673f;
                hxc_l_gc_roots[286] = (const void *)hxc_l_tmp_load_result_n1519;
                if (hxc_l_tmp_load_result_n1519 == NULL)
                {
                  abort();
                }
                struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_class_field_load_result_n1520 = (*hxc_l_tmp_load_result_n1519).hxc_levelPresentation;
                hxc_l_gc_roots[287] = (const void *)hxc_l_tmp_class_field_load_result_n1520;
                struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_levelView_h566c5254a7c5 = hxc_l_tmp_class_field_load_result_n1520;
                hxc_string hxc_l_hudView_h3102c83a00a0 = { 0 };
                switch (hxc_l_locale) {
                  case 0:
                    {
                      hxc_l_hudView_h3102c83a00a0 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
                      if (hxc_string_retain(hxc_l_hudView_h3102c83a00a0) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      break;
                    }
                  case 1:
                    {
                      hxc_l_hudView_h3102c83a00a0 = (hxc_string){ (const uint8_t *)"es-mx", 5, true, NULL };
                      if (hxc_string_retain(hxc_l_hudView_h3102c83a00a0) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      break;
                    }
                  default:
                    {
                      hxc_l_hudView_h3102c83a00a0 = (hxc_string){ (const uint8_t *)"en", 2, true, NULL };
                      if (hxc_string_retain(hxc_l_hudView_h3102c83a00a0) != HXC_STATUS_OK)
                      {
                        abort();
                      }
                      break;
                    }
                }
                hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n1522 = hxc_l_hudView_h3102c83a00a0;
                hxc_l_tmp_managed_flow_owner_n467 = hxc_l_tmp_managed_flow_carrier_move_result_n1522;
                struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_load_result_n1523 = hxc_l_levelView_h566c5254a7c5;
                hxc_l_gc_roots[288] = (const void *)hxc_l_tmp_load_result_n1523;
                if (hxc_l_tmp_load_result_n1523 == NULL)
                {
                  abort();
                }
                hxc_string hxc_l_tmp_load_result_n1524 = hxc_l_latestJournalId;
                hxc_string hxc_l_tmp_instance_call_result_n1526 = hxc_caxecraft_content_RuntimeLevelPresentation_journalBody(hxc_l_tmp_load_result_n1523, hxc_l_tmp_load_result_n1524, hxc_l_tmp_managed_flow_owner_n467);
                hxc_l_hudView9 = hxc_l_tmp_instance_call_result_n1526;
                if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n467) != HXC_STATUS_OK)
                {
                  abort();
                }
              }
              hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n1527 = hxc_l_hudView9;
              hxc_l_tmp_managed_flow_owner_n468 = hxc_l_tmp_managed_flow_carrier_move_result_n1527;
              int32_t hxc_l_tmp_load_result_n1528 = hxc_l_totalVisible;
              int32_t hxc_l_tmp_load_result_n1529 = hxc_l_totalDrawCalls;
              int32_t hxc_l_tmp_load_result_n1530 = hxc_l_frameCount;
              int32_t hxc_l_tmp_load_result_n1531 = hxc_l_completedTicks;
              struct hxc_caxecraft_app_HudCharacterView hxc_l_tmp_load_result_n1533 = hxc_l_hudView_hbda7460f80cb;
              bool hxc_l_tmp_load_result_n1534 = hxc_l_hudView1;
              bool hxc_l_tmp_load_result_n1535 = hxc_l_hudView2;
              bool hxc_l_tmp_load_result_n1536 = hxc_l_hudView3;
              bool hxc_l_tmp_load_result_n1537 = hxc_l_hudView4;
              bool hxc_l_tmp_load_result_n1538 = hxc_l_hudView5;
              int32_t hxc_l_tmp_load_result_n1539 = hxc_l_pickupAmount;
              int32_t hxc_l_tmp_load_result_n1540 = hxc_l_inventoryFullReason;
              int32_t hxc_l_tmp_load_result_n1541 = hxc_l_recoveryFeedback;
              bool hxc_l_tmp_load_result_n1542 = hxc_l_hudView6;
              bool hxc_l_tmp_load_result_n1544 = hxc_l_paused_h86b65f99f244;
              bool hxc_l_tmp_load_result_n1545 = hxc_l_captured_h44c4589a3844;
              struct hxc_caxecraft_domain_RaycastHit hxc_l_tmp_load_result_n1546 = hxc_l_hit;
              int32_t hxc_l_tmp_load_result_n1547 = hxc_l_selectedMode;
              int32_t hxc_l_tmp_load_result_n1548 = hxc_l_locale;
              struct hxc_caxecraft_gameplay_InventoryState hxc_l_tmp_load_result_n1549 = hxc_l_inventory;
              hxc_string hxc_l_tmp_record_field_activeDialogue_owner_n470 = hxc_l_activeDialogue;
              if (hxc_string_retain(hxc_l_tmp_record_field_activeDialogue_owner_n470) != HXC_STATUS_OK)
              {
                abort();
              }
              hxc_string hxc_l_tmp_record_field_activeDialogue_owned_load_result_n1551 = hxc_l_tmp_record_field_activeDialogue_owner_n470;
              enum hxc_caxecraft_app_InteractionPrompt hxc_l_tmp_load_result_n1552 = hxc_l_availableInteractionPrompt;
              struct hxc_caxecraft_domain_Character hxc_l_tmp_load_result_n1553 = hxc_l_enemyActor;
              enum hxc_caxecraft_domain_ActorControllerPhase hxc_l_tmp_load_result_n1554 = hxc_l_enemyPhase_h551fd7fa3cd5;
              hxc_string hxc_l_tmp_record_field_levelLabel_owner_n471 = hxc_l_levelLabel;
              if (hxc_string_retain(hxc_l_tmp_record_field_levelLabel_owner_n471) != HXC_STATUS_OK)
              {
                abort();
              }
              hxc_string hxc_l_tmp_record_field_levelLabel_owned_load_result_n1556 = hxc_l_tmp_record_field_levelLabel_owner_n471;
              hxc_string hxc_l_tmp_record_field_objectiveTitle_owner_n472 = hxc_l_hudView7;
              if (hxc_string_retain(hxc_l_tmp_record_field_objectiveTitle_owner_n472) != HXC_STATUS_OK)
              {
                abort();
              }
              hxc_string hxc_l_tmp_record_field_objectiveTitle_owned_load_result_n1558 = hxc_l_tmp_record_field_objectiveTitle_owner_n472;
              hxc_string hxc_l_tmp_record_field_journalTitle_owner_n473 = hxc_l_tmp_managed_flow_owner_n463;
              if (hxc_string_retain(hxc_l_tmp_record_field_journalTitle_owner_n473) != HXC_STATUS_OK)
              {
                abort();
              }
              hxc_string hxc_l_tmp_record_field_journalTitle_owned_load_result_n1560 = hxc_l_tmp_record_field_journalTitle_owner_n473;
              hxc_string hxc_l_tmp_record_field_journalBody_owner_n474 = hxc_l_tmp_managed_flow_owner_n468;
              if (hxc_string_retain(hxc_l_tmp_record_field_journalBody_owner_n474) != HXC_STATUS_OK)
              {
                abort();
              }
              hxc_string hxc_l_tmp_record_field_journalBody_owned_load_result_n1562 = hxc_l_tmp_record_field_journalBody_owner_n474;
              struct hxc_caxecraft_app_PlayableLevelView *hxc_l_tmp_load_result_n1563 = hxc_l_levelView_hc35c2f93673f;
              hxc_l_gc_roots[289] = (const void *)hxc_l_tmp_load_result_n1563;
              if (hxc_l_tmp_load_result_n1563 == NULL)
              {
                abort();
              }
              struct hxc_caxecraft_content_RuntimeLevelPresentation *hxc_l_tmp_class_field_load_result_n1564 = (*hxc_l_tmp_load_result_n1563).hxc_levelPresentation;
              hxc_l_gc_roots[290] = (const void *)hxc_l_tmp_class_field_load_result_n1564;
              hxc_l_gc_roots[291] = (const void *)(struct hxc_caxecraft_app_HudView){ .hxc_activeDialogue = hxc_l_tmp_record_field_activeDialogue_owned_load_result_n1551, .hxc_character = hxc_l_tmp_load_result_n1533, .hxc_enemy = hxc_l_tmp_load_result_n1553, .hxc_enemyPhase = hxc_l_tmp_load_result_n1554, .hxc_feedback = (struct hxc_caxecraft_app_HudFeedbackView){ .hxc_enemyAttacked = hxc_l_tmp_load_result_n1537, .hxc_enemyDefeated = hxc_l_tmp_load_result_n1536, .hxc_inventoryFullReason = hxc_l_tmp_load_result_n1540, .hxc_pickedUp = hxc_l_tmp_load_result_n1538, .hxc_pickupAmount = hxc_l_tmp_load_result_n1539, .hxc_placementBlocked = hxc_l_tmp_load_result_n1534, .hxc_recoveryDecision = hxc_l_tmp_load_result_n1541, .hxc_recoveryVisible = hxc_l_tmp_load_result_n1542, .hxc_strikeHit = hxc_l_tmp_load_result_n1535 }, .hxc_hit = hxc_l_tmp_load_result_n1546, .hxc_interactionPrompt = hxc_l_tmp_load_result_n1552, .hxc_inventory = hxc_l_tmp_load_result_n1549, .hxc_journalBody = hxc_l_tmp_record_field_journalBody_owned_load_result_n1562, .hxc_journalTitle = hxc_l_tmp_record_field_journalTitle_owned_load_result_n1560, .hxc_levelLabel = hxc_l_tmp_record_field_levelLabel_owned_load_result_n1556, .hxc_locale = hxc_l_tmp_load_result_n1548, .hxc_metrics = (struct hxc_caxecraft_app_HudMetricsView){ .hxc_completedTicks = hxc_l_tmp_load_result_n1531, .hxc_drawCalls = hxc_l_tmp_load_result_n1529, .hxc_renderedFrames = hxc_l_tmp_load_result_n1530, .hxc_visibleBlocks = hxc_l_tmp_load_result_n1528 }, .hxc_mode = hxc_l_tmp_load_result_n1547, .hxc_objectiveTitle = hxc_l_tmp_record_field_objectiveTitle_owned_load_result_n1558, .hxc_paused = hxc_l_tmp_load_result_n1544, .hxc_pointerCaptured = hxc_l_tmp_load_result_n1545, .hxc_presentation = hxc_l_tmp_class_field_load_result_n1564 }.hxc_presentation;
              hxc_l_hudView_h05856689f4f8 = (struct hxc_caxecraft_app_HudView){ .hxc_activeDialogue = hxc_l_tmp_record_field_activeDialogue_owned_load_result_n1551, .hxc_character = hxc_l_tmp_load_result_n1533, .hxc_enemy = hxc_l_tmp_load_result_n1553, .hxc_enemyPhase = hxc_l_tmp_load_result_n1554, .hxc_feedback = (struct hxc_caxecraft_app_HudFeedbackView){ .hxc_enemyAttacked = hxc_l_tmp_load_result_n1537, .hxc_enemyDefeated = hxc_l_tmp_load_result_n1536, .hxc_inventoryFullReason = hxc_l_tmp_load_result_n1540, .hxc_pickedUp = hxc_l_tmp_load_result_n1538, .hxc_pickupAmount = hxc_l_tmp_load_result_n1539, .hxc_placementBlocked = hxc_l_tmp_load_result_n1534, .hxc_recoveryDecision = hxc_l_tmp_load_result_n1541, .hxc_recoveryVisible = hxc_l_tmp_load_result_n1542, .hxc_strikeHit = hxc_l_tmp_load_result_n1535 }, .hxc_hit = hxc_l_tmp_load_result_n1546, .hxc_interactionPrompt = hxc_l_tmp_load_result_n1552, .hxc_inventory = hxc_l_tmp_load_result_n1549, .hxc_journalBody = hxc_l_tmp_record_field_journalBody_owned_load_result_n1562, .hxc_journalTitle = hxc_l_tmp_record_field_journalTitle_owned_load_result_n1560, .hxc_levelLabel = hxc_l_tmp_record_field_levelLabel_owned_load_result_n1556, .hxc_locale = hxc_l_tmp_load_result_n1548, .hxc_metrics = (struct hxc_caxecraft_app_HudMetricsView){ .hxc_completedTicks = hxc_l_tmp_load_result_n1531, .hxc_drawCalls = hxc_l_tmp_load_result_n1529, .hxc_renderedFrames = hxc_l_tmp_load_result_n1530, .hxc_visibleBlocks = hxc_l_tmp_load_result_n1528 }, .hxc_mode = hxc_l_tmp_load_result_n1547, .hxc_objectiveTitle = hxc_l_tmp_record_field_objectiveTitle_owned_load_result_n1558, .hxc_paused = hxc_l_tmp_load_result_n1544, .hxc_pointerCaptured = hxc_l_tmp_load_result_n1545, .hxc_presentation = hxc_l_tmp_class_field_load_result_n1564 };
              struct hxc_caxecraft_app_HudView hxc_l_tmp_load_result_n1566 = hxc_l_hudView_h05856689f4f8;
              hxc_l_gc_roots[292] = (const void *)hxc_l_tmp_load_result_n1566.hxc_presentation;
              struct hxc_caxecraft_app_HudResources hxc_l_tmp_load_result_n1567 = hxc_l_hudResources;
              struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_tmp_load_result_n1568 = hxc_l_contentRegistry;
              hxc_l_gc_roots[293] = (const void *)hxc_l_tmp_load_result_n1568;
              hxc_l_gc_roots[294] = (const void *)hxc_l_uiCatalog;
              hxc_caxecraft_app_CaxecraftApp_drawHud(hxc_l_tmp_load_result_n1566, hxc_l_tmp_load_result_n1567, hxc_l_tmp_load_result_n1568, hxc_l_uiCatalog);
              hxc_record_67a8bd73_destroy(&hxc_l_hudView_h05856689f4f8);
              if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n468) != HXC_STATUS_OK)
              {
                abort();
              }
              if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n463) != HXC_STATUS_OK)
              {
                abort();
              }
              if (hxc_string_release(&hxc_l_hudView7) != HXC_STATUS_OK)
              {
                abort();
              }
              if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n457) != HXC_STATUS_OK)
              {
                abort();
              }
            }
          }
        }
      }
      EndDrawing();
      if (hxc_l_onLoading)
      {
        hxc_l_loadingFramePresented = true;
      }
      hxc_l_frameCount = hxc_i32_add_wrapping(hxc_l_frameCount, 1);
      if (hxc_array_ref_release(hxc_l_dialogueActors) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_l_presentationActorPhases) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_enum_2c0e695d_destroy(&hxc_l_tmp_static_call_argument_0_owner_n361);
    }
    EnableCursor();
    struct hxc_caxecraft_app_RuntimeTextureAtlasCatalog *hxc_l_tmp_load_result_n1572 = hxc_l_runtimeTextures;
    hxc_l_gc_roots[98] = (const void *)hxc_l_tmp_load_result_n1572;
    if (hxc_l_tmp_load_result_n1572 == NULL)
    {
      abort();
    }
    hxc_caxecraft_app_RuntimeTextureAtlasCatalog_unload(hxc_l_tmp_load_result_n1572);
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
    hxc_enum_2c0e695d_destroy(&hxc_l_tmp_static_call_argument_0_owner_n69);
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
    hxc_enum_4b68fff2_destroy(&hxc_l_g_h1d08383de4f8);
    if (hxc_string_release(&hxc_l_pendingCampaignLabel) != HXC_STATUS_OK)
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
  hxc_record_f6127605_destroy(&hxc_l_receipt);
  if (hxc_string_release(&hxc_l_levelLabel) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_enum_4b68fff2_destroy(&hxc_l_g_h1d08383de4f8);
  if (hxc_string_release(&hxc_l_pendingCampaignLabel) != HXC_STATUS_OK)
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
